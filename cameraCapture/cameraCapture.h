#ifndef VIDEO_PREPROCESS_H
#define VIDEO_PREPROCESS_H

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <cstring>
#include <vector>
#include <thread>
#include <atomic>
#include <functional>
#include <mutex>


class CameraCapture
{
public:
    using FrameCallback = std::function<void(const std::vector<uint8_t>& frame)>;

    CameraCapture();
    ~CameraCapture();

    void start(FrameCallback callback);
    void stop();
private:
    void captureLoop();

    int fd_;
    std::vector<Buffer> buffers_;
    std::string devName_;
    int width_;
    int height_;
    std::atomic<bool> running_;
    std::thread captureThread_;
    std::mutex mutex_;

    struct Buffer {
        void* start;
        size_t length;
    };
};

struct Buffer {
    void* start;
    size_t length;
};

#endif //VIDEO_PREPROCESS_H