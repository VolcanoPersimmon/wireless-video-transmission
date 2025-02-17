#include <iostream>
extern "C" {
#include <libavcodec/avcodec.h>
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


    //查找h264编码器
    const AVCodec* codec = avcodec_find_encoder(AV_CODEC_ID_H264);
    if (!codec) {
        std::cerr << "Codec not found" << std::endl;
        return -1;
    }
    //创建编码器上下文
    AVCodecContext* codec_ctx = avcodec_alloc_context3(codec);
    if(!codec_ctx) {
        std::cerr << "Could not allocate video codec context!" << std::endl;
        return -1;
    }
    codec_ctx->width = 1280;
    codec_ctx->height = 720;
    codec_ctx->time_base = (AVRational){1, 30}; // 帧率30fps
    codec_ctx->pix_fmt = AV_PIX_FMT_YUV420P;
    codec_ctx->bit_rate = 4000000; // 4 Mbps
    if (avcodec_open2(codec_ctx, codec, NULL) < 0) {
        std::cerr << "无法打开编码器！" << std::endl;
        return -1;
    }
    return 0;
}
