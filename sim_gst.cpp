#include <gst/gst.h>
#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <vector>

// 线程安全队列
std::queue<std::vector<unsigned char>> bufferQueue;
std::mutex bufferMutex;
std::condition_variable bufferCond;


// 向队列中添加数据
void push_data(const std::vector<unsigned char>& data) {
    std::lock_guard<std::mutex> lock(bufferMutex);
    // if (bufferQueue.size() >= 100) {
    //     bufferCond.wait(lock, [] { return bufferQueue.size() < 100; });
    // }
    bufferQueue.push(data);
    std::cout << "Queue size: " << bufferQueue.size() << std::endl;
    bufferCond.notify_one();
    // std::this_thread::sleep_for(std::chrono::milliseconds(33)); // 模拟 30FPS

}

// 从队列中取数据
std::vector<unsigned char> pop_data() {
    std::unique_lock<std::mutex> lock(bufferMutex);
    bufferCond.wait(lock, [] { return !bufferQueue.empty(); });
    auto data = bufferQueue.front();
    bufferQueue.pop();
    // bufferCond.notify_one(); 
    return data;
}

// 编码回调函数：从 appsink 获取编码后的数据
static GstFlowReturn new_sample_callback(GstElement *sink, gpointer user_data) {
    auto now = std::chrono::high_resolution_clock::now();
    static auto last_time = now;
    
    GstSample *sample;
    GstBuffer *buffer;
    GstMapInfo map;

    // 从 appsink 拉取 sample
    g_signal_emit_by_name(sink, "pull-sample", &sample);
    if (!sample) {
        return GST_FLOW_ERROR;
    }

    buffer = gst_sample_get_buffer(sample);
    if (gst_buffer_map(buffer, &map, GST_MAP_READ)) {
        // 将编码数据放入队列
        std::vector<unsigned char> encoded_data(map.data, map.data + map.size);
        push_data(encoded_data);

        gst_buffer_unmap(buffer, &map);
    }

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_time).count();
    std::cout << "Time since last sample: " << duration << " ms" << std::endl;
    last_time = now;

    std::cout << "PTS: " << GST_BUFFER_PTS(buffer) << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(33)); // 模拟30FPS


    gst_sample_unref(sample);
    return GST_FLOW_OK;
}

// 解码线程：从队列中取数据并推送到 appsrc
void decode_thread(GstElement *appsrc) {
    while (true) {
        // 从队列获取编码数据块
        std::vector<unsigned char> data = pop_data();

        // 创建 GStreamer Buffer 并填充数据
        GstBuffer *buffer = gst_buffer_new_allocate(NULL, data.size(), NULL);
        gst_buffer_fill(buffer, 0, data.data(), data.size());

        // 推送数据到 appsrc
        GstFlowReturn ret;
        g_signal_emit_by_name(appsrc, "push-buffer", buffer, &ret);

        std::cout << "PTS: " << GST_BUFFER_PTS(buffer) << std::endl;
        gst_buffer_unref(buffer);

        if (ret != GST_FLOW_OK) {
            std::cerr << "Error pushing buffer to appsrc!" << std::endl;
            break;
        }
    }
}

int main(int argc, char *argv[]) {
    gst_init(&argc, &argv);

    // 编码管道：摄像头 -> H.264 编码 -> appsink
    GstElement *encode_pipeline = gst_parse_launch(
        "v4l2src ! video/x-raw,format=YUY2,width=640,height=480,framerate=30/1 ! "
        "videoconvert ! x264enc tune=zerolatency bitrate=500 ! "
        "video/x-h264,stream-format=byte-stream ! appsink name=enc_sink",
        nullptr);

    if (!encode_pipeline) {
        std::cerr << "Failed to create encoding pipeline!" << std::endl;
        return -1;
    }

    GstElement *appsink = gst_bin_get_by_name(GST_BIN(encode_pipeline), "enc_sink");
    g_object_set(appsink, "emit-signals", TRUE, "sync", FALSE, NULL);
    g_signal_connect(appsink, "new-sample", G_CALLBACK(new_sample_callback), NULL);

    // 解码管道：appsrc -> H.264 解码 -> 显示
    GstElement *decode_pipeline = gst_parse_launch(
        "appsrc name=src format=time is-live=true block=true ! "
        "h264parse ! avdec_h264 ! videoconvert ! autovideosink",
        nullptr);

    if (!decode_pipeline) {
        std::cerr << "Failed to create decoding pipeline!" << std::endl;
        gst_object_unref(encode_pipeline);
        return -1;
    }

    GstElement *appsrc = gst_bin_get_by_name(GST_BIN(decode_pipeline), "src");
    g_object_set(appsrc, "caps",
                 gst_caps_from_string("video/x-h264,stream-format=byte-stream"),
                 NULL);

    // 启动编码管道
    if (gst_element_set_state(encode_pipeline, GST_STATE_PLAYING) == GST_STATE_CHANGE_FAILURE) {
        std::cerr << "Failed to set encoding pipeline to PLAYING state!" << std::endl;
        gst_object_unref(encode_pipeline);
        gst_object_unref(decode_pipeline);
        return -1;
    }

    // 启动解码管道
    if (gst_element_set_state(decode_pipeline, GST_STATE_PLAYING) == GST_STATE_CHANGE_FAILURE) {
        std::cerr << "Failed to set decoding pipeline to PLAYING state!" << std::endl;
        gst_object_unref(encode_pipeline);
        gst_object_unref(decode_pipeline);
        return -1;
    }

    // 启动解码线程
    std::thread decoder(decode_thread, appsrc);

    // 主线程阻塞，等待用户终止程序
    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();

    // 停止管道并释放资源
    gst_element_set_state(encode_pipeline, GST_STATE_NULL);
    gst_element_set_state(decode_pipeline, GST_STATE_NULL);

    gst_object_unref(encode_pipeline);
    gst_object_unref(decode_pipeline);

    decoder.join();
    return 0;
}
