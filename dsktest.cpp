#include <iostream>
extern "C" {
#include <libavformat/avformat.h>
#include <libavutil/dict.h>
#include <libavdevice/avdevice.h>
}

int main() {
    // 注册设备（必要时）
    avdevice_register_all();

    AVFormatContext* fmt_ctx = nullptr;
    AVDictionary* options = nullptr;
    av_dict_set(&options, "video_size", "1280x720", 0);
    av_dict_set(&options, "framerate", "30", 0);

    // 显式指定输入格式，使用 "v4l2"
    const AVInputFormat* ifmt = av_find_input_format("v4l2");
    if (!ifmt) {
        std::cerr << "找不到 v4l2 输入格式！" << std::endl;
        return -1;
    }
    
    // 传入设备文件路径
    const char* device = "/dev/video0";
    if (avformat_open_input(&fmt_ctx, device, ifmt, &options) < 0) {
        std::cerr << "无法打开摄像头！" << std::endl;
        return -1;
    }

    if (avformat_find_stream_info(fmt_ctx, nullptr) < 0) {
        std::cerr << "无法获取流信息！" << std::endl;
        return -1;
    }

    int video_stream_idx = -1;
    for (unsigned int i = 0; i < fmt_ctx->nb_streams; i++) {
        if (fmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            video_stream_idx = i;
            break;
        }
    }
    if (video_stream_idx == -1) {
        std::cerr << "未找到视频流！" << std::endl;
        return -1;
    }

    std::cout << "摄像头打开成功！" << std::endl;
    return 0;
}
