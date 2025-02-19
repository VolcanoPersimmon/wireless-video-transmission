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
#include <libavutil/opt.h>
#include <libavcodec/avcodec.h>
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

// 辅助函数：将一个 AVFrame 编码成 H.264 并写入文件
int encodeFrame(AVCodecContext* codec_context, AVFrame* frame, std::ofstream &outfile) {
    int ret = avcodec_send_frame(codec_context, frame);
    if (ret < 0) {
        std::cerr << "Error sending frame for encoding: " << ret << std::endl;
        return ret;
    }
    
    AVPacket* pkt = av_packet_alloc();
    if (!pkt) {
        std::cerr << "Could not allocate packet" << std::endl;
        return -1;
    }
    
    // 从编码器中读取编码数据包
    while (ret >= 0) {
        ret = avcodec_receive_packet(codec_context, pkt);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            av_packet_free(&pkt);
            return 0;
        } else if (ret < 0) {
            std::cerr << "Error during encoding: " << ret << std::endl;
            av_packet_free(&pkt);
            return ret;
        }
        // 将编码后的数据写入文件
        outfile.write(reinterpret_cast<char*>(pkt->data), pkt->size);
        av_packet_unref(pkt);
    }
    
    av_packet_free(&pkt);
    return 0;
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

//查找h264编码器
    const AVCodec* codec = avcodec_find_encoder(AV_CODEC_ID_H264);
    if (!codec) {
        std::cerr << "Codec not found" << std::endl;
        return -1;
    }

    //创建编码器上下文
    AVCodecContext* codec_context = avcodec_alloc_context3(codec);
    if(!codec_context) {
        std::cerr << "Could not allocate video codec context!" << std::endl;
        return -1;
    }

    //设置编码参数;
    codec_context->bit_rate = 400000;
    codec_context->width = 640;
    codec_context->height = 480;
    codec_context->time_base = {1, 30};            // 30 fps
    codec_context->framerate = {30, 1};
    codec_context->gop_size = 10;                  // 组内关键帧间隔
    codec_context->max_b_frames = 1;               // 最大 B 帧数
    codec_context->pix_fmt = AV_PIX_FMT_YUV420P;     // H.264 编码器通常要求输入 YUV420P 格式

    // 可选：设置一些编码选项，比如编码速度和质量
    av_opt_set(codec_context->priv_data, "preset", "fast", 0);

    //打开编码器
    if (avcodec_open2(codec_context, codec, NULL) < 0) {
        std::cerr << "Could not open codec!" << std::endl;
        return -1;
    }

    // 打开输出文件，用于写入 H.264 数据
    std::ofstream outfile("output.h264", std::ios::binary);
    if (!outfile) {
        std::cerr << "Could not open output file" << std::endl;
        return -1;
    }
    
    /////////////////////////////////主循环///////////////////////////////////////

    // 9. 进入采集循环（示例采集 100 帧）
    for (int i = 0; i < 150; i++) {
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
        
        out_frame->pts = i;
        if (!out_frame || !out_frame->data[0]) {
            std::cerr << "输出帧无效或数据为空" << std::endl;
            av_frame_free(&out_frame);
            break;
        }
        if (codec_context->pix_fmt != out_frame->format) {
            std::cerr << "像素格式不匹配！" << std::endl;
            break;
        }
        // 此时，out_frame 包含转换后的 YUV420P 数据，可供后续处理或编码使用
        std::cout << "捕获第 " << i << " 帧，并已转换为 YUV420P" << std::endl;

        // 假设 width = 640, height = 480, out_frame 已经转换为 YUV420P 格式
        // saveFrameAsYUV(out_frame, 640, 480, "output.yuv");

        // 对当前帧进行编码，并写入到输出文件
        if (encodeFrame(codec_context, out_frame, outfile) < 0) {
            std::cerr << "Failed to encode frame " << i << std::endl;
            av_frame_free(&out_frame);
            break;
        }
        std::cout << "Encoded frame " << i << std::endl;
        // av_frame_free(&out_frame);
        av_frame_free(&in_frame);

        // 将缓冲区重新入队，以便下次使用
        if (ioctl(fd, VIDIOC_QBUF, &buf) == -1) {
            perror("重新入队缓冲区失败");
            break;
        }
    }
    /////////////////////////////////////////////////////////////////////////

    // 10. 停止视频流
    if (ioctl(fd, VIDIOC_STREAMOFF, &type) == -1) {
        perror("停止视频流失败");
    }

     // 7. 刷新编码器（送入 NULL 帧以获得延迟的包）
    encodeFrame(codec_context, nullptr, outfile);

    // 11. 释放资源
    sws_freeContext(sws_ctx);
    av_freep(&out_frame->data[0]);
    av_frame_free(&out_frame);
    for (unsigned int i = 0; i < req.count; i++) {
        munmap(buffers[i].start, buffers[i].length);
    }
    close(fd);
    avcodec_free_context(&codec_context);
    std::cout << "H.264 编码完成，输出文件为 output.h264" << std::endl;
    return 0;
}
