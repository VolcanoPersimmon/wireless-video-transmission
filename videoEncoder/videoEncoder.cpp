#include "videoEncoder.h"

VideoEncoder::VideoEncoder(int width = 640, int height = 480, AVPixelFormat in_pix_fmt = AV_PIX_FMT_YUYV422,
    AVPixelFormat out_pix_fmt = AV_PIX_FMT_YUV420P)
: width_(width), height_(height), in_pix_fmt_(in_pix_fmt), out_pix_fmt_(out_pix_fmt),
sws_ctx_(nullptr), codec_context_(nullptr), encode_thread_(), running_(false) {}

VideoEncoder::~VideoEncoder() {
    stop();
    if (sws_ctx_) sws_freeContext(sws_ctx_);
    avcodec_free_context(&codec_context_);
    av_frame_free(&out_frame_);
}

void VideoEncoder::init() {
    // 初始化 Swscale 上下文
    sws_ctx_ = sws_getContext(width_, height_, in_pix_fmt_, width_, height_, out_pix_fmt_,
                             SWS_BILINEAR, nullptr, nullptr, nullptr);
    if (!sws_ctx_) throw std::runtime_error("创建转换上下文失败");

    // 分配输出帧
    out_frame_ = av_frame_alloc();
    if (!out_frame_) throw std::runtime_error("分配输出帧失败");
    int ret = av_image_alloc(out_frame_->data, out_frame_->linesize, width_, height_, out_pix_fmt_, 1);
    if (ret < 0) throw std::runtime_error("分配图像数据失败");

    // 查找 H.264 编码器
    const AVCodec* codec = avcodec_find_encoder(AV_CODEC_ID_H264);
    if (!codec) throw std::runtime_error("未找到 H.264 编码器");

    // 创建编码器上下文
    codec_context_ = avcodec_alloc_context3(codec);
    if (!codec_context_) throw std::runtime_error("创建编码器上下文失败");

    // 设置编码参数
    codec_context_->bit_rate = 400000;
    codec_context_->width = width_;
    codec_context_->height = height_;
    codec_context_->time_base = {1, 30};
    codec_context_->framerate = {30, 1};
    codec_context_->gop_size = 10;
    codec_context_->max_b_frames = 1;
    codec_context_->pix_fmt = out_pix_fmt_;

    av_opt_set(codec_context_->priv_data, "preset", "fast", 0);

    // 打开编码器
    if (avcodec_open2(codec_context_, codec, nullptr) < 0)
        throw std::runtime_error("打开编码器失败");
}

void VideoEncoder::start(EncodeCallback callback) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (running_) return;

    encode_callback_ = std::move(callback);
    running_ = true;
    encode_thread_ = std::thread(&VideoEncoder::encode_loop, this);
}

void VideoEncoder::stop() {
    std::lock_guard<std::mutex> lock(mutex_);
            if (!running_) return;
    
            running_ = false;
            if (encode_thread_.joinable()) encode_thread_.join();
}

void VideoEncoder::process_frame(const std::vector<uint8_t>& frame) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (!running_) return;

    // 创建输入 AVFrame
    AVFrame* in_frame = av_frame_alloc();
    if (!in_frame) return;

    in_frame->width = width_;
    in_frame->height = height_;
    in_frame->format = in_pix_fmt_;

    int ret = av_image_fill_arrays(in_frame->data, in_frame->linesize,
                                   frame.data(), in_pix_fmt_, width_, height_, 1);
    if (ret < 0) {
        av_frame_free(&in_frame);
        return;
    }

    // 转换像素格式
    sws_scale(sws_ctx_, in_frame->data, in_frame->linesize,
              0, height_, out_frame_->data, out_frame_->linesize);

    av_frame_unref(out_frame_);

    // 编码帧
    encode_frame(out_frame_);
    av_frame_free(&in_frame);
}

void VideoEncoder::encode_loop() {
    std::ofstream outfile("output.h264", std::ios::binary);
            if (!outfile) throw std::runtime_error("无法打开输出文件");
    
            while (running_) {
                std::this_thread::sleep_for(std::chrono::microseconds(10000)); // 适当休眠避免忙等待
            }
    
            // 刷新编码器
            this->encode_frame(nullptr);
}

void VideoEncoder::encode_frame(AVFrame* frame) {
    if (!frame) {
        AVPacket* pkt = av_packet_alloc();
        if (!pkt) return;
        av_packet_unref(pkt);
        return;
    }

    frame->pts = current_pts_++;
    int ret = avcodec_send_frame(codec_context_, frame);
    if (ret < 0) {
        std::cerr << "发送帧失败" << std::endl;
        return;
    }

    while (true) {
        AVPacket* pkt = av_packet_alloc();
        if (!pkt) break;

        ret = avcodec_receive_packet(codec_context_, pkt);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            av_packet_free(&pkt);
            break;
        } else if (ret < 0) {
            std::cerr << "接收编码数据失败" << std::endl;
            av_packet_free(&pkt);
            break;
        }

        encode_callback_(std::vector<uint8_t>(pkt->data, pkt->data + pkt->size));
        av_packet_unref(pkt);
    }
}