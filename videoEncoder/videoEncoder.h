#ifndef VIDEO_ENCODER_H
#define VIDEO_ENCODER_H

extern "C" {
    #include <libavutil/opt.h>
    #include <libavcodec/avcodec.h>
    #include <libavutil/imgutils.h>
    #include <libswscale/swscale.h>
    #include <libavutil/pixfmt.h>
    }
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <atomic>
#include <functional>
#include <mutex>

class VideoEncoder {
    public:
        using EncodeCallback = std::function<void(const std::vector<uint8_t>&)>;
    
        VideoEncoder(int width, int height, AVPixelFormat in_pix_fmt, AVPixelFormat out_pix_fmt);
        ~VideoEncoder();
    
        void init();
    
        void start(EncodeCallback callback);
    
        void stop();
    
        void process_frame(const std::vector<uint8_t>& frame);
    
    private:
        void encode_loop();
    
        void encode_frame(AVFrame* frame);
    
        int width_, height_;
        AVPixelFormat in_pix_fmt_, out_pix_fmt_;
        SwsContext* sws_ctx_;
        AVCodecContext* codec_context_;
        AVFrame* out_frame_;
        std::thread encode_thread_;
        std::atomic<bool> running_;
        std::function<void(const std::vector<uint8_t>&)> encode_callback_;
        uint64_t current_pts_ = 0;
        std::mutex mutex_;
    };

#endif //VIDEO_ENCODER_H