#ifndef VIDEO_PREPROCESS_H
#define VIDEO_PREPROCESS_H

#include <iostream>


class videoProcess
{
public:
    videoProcess();
    ~videoProcess();

    int videoCapture(char name);

    int frameProcess();

    int camera_fd;



};

struct Buffer {
    void* start;
    size_t length;
};

#endif //VIDEO_PREPROCESS_H