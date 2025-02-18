#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <linux/videodev2.h>
#include <fstream>

extern "C" {
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include <libavutil/pixfmt.h>
}

struct Buffer {
    void* start;
    size_t length;
};

void saveFrameAsYUV(AVFrame* out_frame, int width, int height, const char* filename) {
    std::ofstream outFile(filename, std::ios::binary | std::ios::app);
    if (!outFile) {
        std::cerr << "无法打开文件 " << filename << " 进行写入" << std::endl;
        return;
    }

    // 写入 Y 平面
    for (int i = 0; i < height; i++) {
        // out_frame->data[0] 为 Y 平面的起始地址
        // 每行写入 width 个字节（假设行内数据前 width 个字节为图像数据）
        outFile.write(reinterpret_cast<const char*>(out_frame->data[0] + i * out_frame->linesize[0]), width);
    }
    // 写入 U 平面
    for (int i = 0; i < height / 2; i++) {
        outFile.write(reinterpret_cast<const char*>(out_frame->data[1] + i * out_frame->linesize[1]), width / 2);
    }
    // 写入 V 平面
    for (int i = 0; i < height / 2; i++) {
        outFile.write(reinterpret_cast<const char*>(out_frame->data[2] + i * out_frame->linesize[2]), width / 2);
    }
    outFile.close();
    std::cout << "已将帧数据保存到 " << filename << std::endl;
}

int main() {
    // 1. 打开摄像头设备
    const char* devName = "/dev/video0";
    int fd = open(devName, O_RDWR);
    if (fd == -1) {
        perror("打开设备失败");
        return 1;
    }

    // 2. 查询设备能力
    struct v4l2_capability cap;
    if (ioctl(fd, VIDIOC_QUERYCAP, &cap) == -1) {
        perror("查询设备能力失败");
        close(fd);
        return 1;
    }
    std::cout << "摄像头设备: " << cap.card << std::endl;

    // 3. 设置视频格式（选择 YUYV 格式，分辨率 640x480）
    struct v4l2_format fmt;
    memset(&fmt, 0, sizeof(fmt));
    fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width       = 640;
    fmt.fmt.pix.height      = 480;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV; // YUYV 格式
    fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;
    if (ioctl(fd, VIDIOC_S_FMT, &fmt) == -1) {
        perror("设置视频格式失败");
        close(fd);
        return 1;
    }
    std::cout << "视频格式设置为 YUYV 640x480" << std::endl;

    // 4. 请求缓冲区（这里申请 4 个缓冲区）
    struct v4l2_requestbuffers req;
    memset(&req, 0, sizeof(req));
    req.count  = 4;
    req.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;
    if (ioctl(fd, VIDIOC_REQBUFS, &req) == -1) {
        perror("请求缓冲区失败");
        close(fd);
        return 1;
    }

    // 5. 映射缓冲区
    Buffer buffers[4];
    for (unsigned int i = 0; i < req.count; i++) {
        struct v4l2_buffer buf;
        memset(&buf, 0, sizeof(buf));
        buf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index  = i;
        if (ioctl(fd, VIDIOC_QUERYBUF, &buf) == -1) {
            perror("查询缓冲区失败");
            close(fd);
            return 1;
        }
        buffers[i].length = buf.length;
        buffers[i].start = mmap(nullptr, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);
        if (buffers[i].start == MAP_FAILED) {
            perror("映射缓冲区失败");
            close(fd);
            return 1;
        }
    }

    // 6. 将缓冲区入队
    for (unsigned int i = 0; i < req.count; i++) {
        struct v4l2_buffer buf;
        memset(&buf, 0, sizeof(buf));
        buf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index  = i;
        if (ioctl(fd, VIDIOC_QBUF, &buf) == -1) {
            perror("缓冲区入队失败");
            close(fd);
            return 1;
        }
    }

    // 7. 启动视频流
    int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(fd, VIDIOC_STREAMON, &type) == -1) {
        perror("启动视频流失败");
        close(fd);
        return 1;
    }

    // 8. 初始化 libswscale 转换上下文：将 YUYV422 转为 YUV420P
    //    注意：V4L2 中 YUYV 格式对应的 FFmpeg 像素格式是 AV_PIX_FMT_YUYV422
    int width = 640, height = 480;
    AVPixelFormat in_pix_fmt  = AV_PIX_FMT_YUYV422;
    AVPixelFormat out_pix_fmt = AV_PIX_FMT_YUV420P;
    SwsContext* sws_ctx = sws_getContext(width, height,
                                         in_pix_fmt,
                                         width, height,
                                         out_pix_fmt,
                                         SWS_BILINEAR, nullptr, nullptr, nullptr);
    if (!sws_ctx) {
        std::cerr << "无法创建转换上下文" << std::endl;
        close(fd);
        return 1;
    }

    // 分配输出 AVFrame（YUV420P 格式）
    AVFrame* out_frame = av_frame_alloc();
    out_frame->width  = width;
    out_frame->height = height;
    out_frame->format = out_pix_fmt;
    int ret = av_image_alloc(out_frame->data, out_frame->linesize, width, height, out_pix_fmt, 1);
    if (ret < 0) {
        std::cerr << "无法分配输出帧内存" << std::endl;
        sws_freeContext(sws_ctx);
        close(fd);
        return 1;
    }

    // 9. 进入采集循环（示例采集 100 帧）
    for (int i = 0; i < 100; i++) {
        struct v4l2_buffer buf;
        memset(&buf, 0, sizeof(buf));
        buf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        if (ioctl(fd, VIDIOC_DQBUF, &buf) == -1) {
            perror("取出缓冲区数据失败");
            break;
        }

        // 创建一个输入 AVFrame 用来包装摄像头采集到的数据
        AVFrame* in_frame = av_frame_alloc();
        in_frame->width  = width;
        in_frame->height = height;
        in_frame->format = in_pix_fmt;
        // 使用 av_image_fill_arrays 将缓冲区数据填充到 in_frame 中
        ret = av_image_fill_arrays(in_frame->data, in_frame->linesize,
                                   static_cast<uint8_t*>(buffers[buf.index].start),
                                   in_pix_fmt, width, height, 1);
        if (ret < 0) {
            std::cerr << "填充输入帧数据失败" << std::endl;
            av_frame_free(&in_frame);
            break;
        }

        // 进行像素格式转换：YUYV422 -> YUV420P
        sws_scale(sws_ctx, in_frame->data, in_frame->linesize,
                  0, height, out_frame->data, out_frame->linesize);

        // 此时，out_frame 包含转换后的 YUV420P 数据，可供后续处理或编码使用
        std::cout << "捕获第 " << i << " 帧，并已转换为 YUV420P" << std::endl;

        // 假设 width = 640, height = 480, out_frame 已经转换为 YUV420P 格式
        saveFrameAsYUV(out_frame, 640, 480, "output.yuv");


        av_frame_free(&in_frame);

        // 将缓冲区重新入队，以便下次使用
        if (ioctl(fd, VIDIOC_QBUF, &buf) == -1) {
            perror("重新入队缓冲区失败");
            break;
        }
    }

    // 10. 停止视频流
    if (ioctl(fd, VIDIOC_STREAMOFF, &type) == -1) {
        perror("停止视频流失败");
    }

    // 11. 释放资源
    sws_freeContext(sws_ctx);
    av_freep(&out_frame->data[0]);
    av_frame_free(&out_frame);
    for (unsigned int i = 0; i < req.count; i++) {
        munmap(buffers[i].start, buffers[i].length);
    }
    close(fd);

    std::cout << "采集结束" << std::endl;
    return 0;
}
