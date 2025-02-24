#include <gtest/gtest.h>
#include <vector>
#include <fstream>
#include <thread>
#include <atomic>
#include "videoEncoder.h"

using namespace std;

TEST(VideoEncoderTest, PixelFormatConversion) {
    VideoEncoder encoder(640, 480, AV_PIX_FMT_YUYV422, AV_PIX_FMT_YUV420P);
    encoder.init();

    // 生成测试输入帧（YUYV422）
    vector<uint8_t> yuyv_frame(640 * 480 * 2, 0);
    for (int i = 0; i < 640 * 480; ++i) {
        yuyv_frame[i] = i % 256; // 简单填充测试数据
    }

    // 转换为 YUV420P
    vector<uint8_t> yuv_frame;
    encoder.process_frame(yuyv_frame, yuv_frame);

    // 验证输出尺寸
    EXPECT_EQ(yuv_frame.size(), 640 * 480 * 3 / 2);

    // 使用 ffmpeg 验证像素格式（需集成到测试框架中或手动执行）
}

TEST(VideoEncoderTest, H264Encoding) {
    VideoEncoder encoder(640, 480, AV_PIX_FMT_YUV420P, AV_PIX_FMT_YUV420P);
    encoder.init();

    // 生成测试输入帧（YUV420P）
    vector<uint8_t> yuv_frame(640 * 480 * 3 / 2, 0);
    for (int i = 0; i < 640 * 480 * 3 / 2; ++i) {
        yuv_frame[i] = i % 256;
    }

    // 编码为 H.264
    vector<uint8_t> encoded_data;
    encoder.encode_frame(yuv_frame, encoded_data);

    // 验证输出是否为 H.264 NALU
    EXPECT_NE(encoded_data.empty(), true);
    EXPECT_EQ(encoded_data[0], 0x01); // H.264 NALU Start Code
}

TEST(VideoEncoderTest, MultiThread) {
    VideoEncoder encoder(640, 480, AV_PIX_FMT_YUYV422, AV_PIX_FMT_YUV420P);
    encoder.init();

    atomic<int> frame_count(0);
    thread producer([&encoder, &frame_count]() {
        while (frame_count < 100) {
            vector<uint8_t> frame(640 * 480 * 2, 0);
            encoder.process_frame(frame);
            frame_count++;
            this_thread::sleep_for(chrono::milliseconds(10));
        }
    });

    thread consumer([&encoder, &frame_count]() {
        while (frame_count < 100) {
            // 模拟编码延迟
            this_thread::sleep_for(chrono::milliseconds(15));
        }
    });

    producer.join();
    consumer.join();

    // 验证无数据丢失（需根据具体实现检查编码队列）
}