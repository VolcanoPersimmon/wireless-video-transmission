#include <iostream>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstring>
#include "videoProcess.h"
extern "C" {
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include <libavutil/pixfmt.h>
}


videoProcess::videoProcess(){}

videoProcess::~videoProcess(){}

int videoProcess::videoCapture(char name){

    //打开摄像头 open()
    const char* devName = &name;
    camera_fd = open(devName, O_RDWR);
    if (camera_fd == -1){
        perror("打开设备失败");
        return -1;
    }

    //查询设备能力 VIDIOC_QUERYCAP
    struct v4l2_capability cap;
    if (ioctl(camera_fd, VIDIOC_QUERYCAP, &cap) == -1) {
        perror("查询设备能力失败");
        close(camera_fd);
        return -1;
    }
    std::cout << "摄像头设备: " << cap.card << std::endl;

    //设置视频格式 选择 YUYV 格式，分辨率 640x480）VIDIOC_S_FMT
     struct v4l2_format fmt;
    memset(&fmt, 0, sizeof(fmt));
    fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width       = 640;
    fmt.fmt.pix.height      = 480;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV; // YUYV 格式
    fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;
    if (ioctl(camera_fd, VIDIOC_S_FMT, &fmt) == -1) {
        perror("设置视频格式失败");
        close(camera_fd);
        return -1;
    }
    std::cout << "视频格式设置为 YUYV 640x480" << std::endl;

    // 请求缓冲区（这里申请 4 个缓冲区）VIDIOC_REQBUFS
    struct v4l2_requestbuffers req;
    memset(&req, 0, sizeof(req));
    req.count  = 4;
    req.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;
    if (ioctl(camera_fd, VIDIOC_REQBUFS, &req) == -1) {
        perror("请求缓冲区失败");
        close(camera_fd);
        return -1;
    }

    // 映射缓冲区 mmap()
    Buffer buffers[4];
    for (unsigned int i = 0; i < req.count; i++) {
        struct v4l2_buffer buf;
        memset(&buf, 0, sizeof(buf));
        buf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index  = i;
        if (ioctl(camera_fd, VIDIOC_QUERYBUF, &buf) == -1) {
            perror("查询缓冲区失败");
            close(camera_fd);
            return -1;
        }
        buffers[i].length = buf.length;
        buffers[i].start = mmap(nullptr, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, camera_fd, buf.m.offset);
        if (buffers[i].start == MAP_FAILED) {
            perror("映射缓冲区失败");
            close(camera_fd);
            return -1;
        }
    }

    // 启动视频流
    int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(camera_fd, VIDIOC_STREAMON, &type) == -1) {
        perror("启动视频流失败");
        close(camera_fd);
        return -1;
    }


}

int videoProcess::frameProcess(){
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
        close(camera_fd);
        return -1;
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
        close(camera_fd);
        return 1;
    }

}