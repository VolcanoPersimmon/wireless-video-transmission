#include "cameraCapture.h"

CameraCapture::CameraCapture(int width = 640, int height = 480, const char* devName = "/dev/video0") : width_(width), height_(height), devName_(devName), running_(false){}

CameraCapture::~CameraCapture(){
    stop();
}

void CameraCapture::start(FrameCallback callback){
     std::lock_guard<std::mutex> lock(mutex_);
        if (running_) return;

        // 初始化设备
        fd_ = open(devName_.c_str(), O_RDWR);
        if (fd_ == -1) throw std::runtime_error("无法打开摄像头设备");

        // 查询设备能力
        struct v4l2_capability cap;
        if (ioctl(fd_, VIDIOC_QUERYCAP, &cap) == -1) throw std::runtime_error("查询设备能力失败");
        std::cout << "摄像头设备： " << cap.card << std::endl;

        // 设置视频格式
        struct v4l2_format fmt;
        memset(&fmt, 0, sizeof(fmt));
        fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        fmt.fmt.pix.width = width_;
        fmt.fmt.pix.height = height_;
        fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
        fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
        if (ioctl(fd_, VIDIOC_S_FMT, &fmt) == -1) throw std::runtime_error("设置视频格式失败");

        std::cout << "视频格式设置为 YUYV 640x480" << std::endl;

        // 请求缓冲区
        struct v4l2_requestbuffers req;
        memset(&req, 0, sizeof(req));
        req.count = 4;
        req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        req.memory = V4L2_MEMORY_MMAP;
        if (ioctl(fd_, VIDIOC_REQBUFS, &req) == -1) throw std::runtime_error("请求缓冲区失败");

        // 映射缓冲区
        for (unsigned int i = 0; i < req.count; ++i) {
            struct v4l2_buffer buf;
            memset(&buf, 0, sizeof(buf));
            buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            buf.memory = V4L2_MEMORY_MMAP;
            buf.index = i;
            if (ioctl(fd_, VIDIOC_QUERYBUF, &buf) == -1) throw std::runtime_error("查询缓冲区失败");
            buffers_[i].length = buf.length;
            buffers_[i].start = (mmap(nullptr, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd_, buf.m.offset));
            if (buffers_[i].start == MAP_FAILED) throw std::runtime_error("映射缓冲区失败");
        }

            // 将缓冲区入队
        for (unsigned int i = 0; i < req.count; i++) {
            struct v4l2_buffer buf;
            memset(&buf, 0, sizeof(buf));
            buf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            buf.memory = V4L2_MEMORY_MMAP;
            buf.index  = i;
            if (ioctl(fd_, VIDIOC_QBUF, &buf) == -1) {
                throw std::runtime_error("缓冲区入队失败");
                
            }
        }

        // 启动视频流
        type_ = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        int ret = ioctl(fd_, VIDIOC_STREAMON, &type_);
        // if (ioctl(fd_, VIDIOC_STREAMON, type) == -1) throw std::runtime_error("启动视频流失败");
        if (ret == -1) {
            std::cerr << "VIDIOC_STREAMON failed: " 
                    << strerror(errno) << " (" << errno << ")" << std::endl;
            throw std::runtime_error("启动视频流失败");
        }


        running_ = true;
        captureThread_ = std::thread(&CameraCapture::captureLoop, this, std::move(callback));
}

void CameraCapture::stop(){
    std::lock_guard<std::mutex> lock(mutex_);
    if (!running_) return;

    running_ = false;
    if (captureThread_.joinable()) captureThread_.join();

        // 停止视频流
        if (ioctl(fd_, VIDIOC_STREAMOFF, &type_) == -1) perror("停止视频流失败");

        // 释放缓冲区
        for (auto& buffer : buffers_) {
            munmap(buffer.start, buffer.length);
        }
        close(fd_);
}

void CameraCapture::captureLoop(FrameCallback callback){
    while (running_) {
        struct v4l2_buffer buf;
        memset(&buf, 0, sizeof(buf));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

        if (ioctl(fd_, VIDIOC_DQBUF, &buf) == -1) {
            perror("取出缓冲区失败");
            break;
        }

        std::vector<uint8_t> frame(
            static_cast<uint8_t*>(buffers_[buf.index].start),
            static_cast<uint8_t*>(buffers_[buf.index].start) + buffers_[buf.index].length
        );
        callback(frame);

        // 重新入队缓冲区
        if (ioctl(fd_, VIDIOC_QBUF, &buf) == -1) {
            perror("重新入队缓冲区失败");
            break;
        }
    }
}

