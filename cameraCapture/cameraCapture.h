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
#include <sys/mman.h>

struct Buffer {
    void* start;
    size_t length;
};

class CameraCapture
{
public:
    using FrameCallback = std::function<void(const std::vector<uint8_t>& frame)>;

    CameraCapture(int width, int height, const char* devName);
    ~CameraCapture();

    void start(FrameCallback callback);
    void stop();
private:
    void captureLoop(FrameCallback callback);

    int fd_;
    int type_;
    Buffer buffers_[4];
    std::string devName_;
    int width_;
    int height_;
    std::atomic<bool> running_;
    std::thread captureThread_;
    std::mutex mutex_;

    
};


#endif //VIDEO_PREPROCESS_H