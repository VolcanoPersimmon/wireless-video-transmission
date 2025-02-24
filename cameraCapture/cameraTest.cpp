#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <atomic>
#include "cameraCapture.h" // 包含你的 CameraCapture 类定义

using namespace std;

void test_camera_capture() {
    try {
        // 创建 CameraCapture 实例（640x480，YUYV 格式）
        CameraCapture camera(640, 480, "/dev/video0");

        // 定义回调函数：将捕获的帧保存到文件
        auto callback = [](const vector<uint8_t>& frame) {
            static int frame_count = 0;
            const char* filename = "test_frame.yuv";
            ofstream outFile(filename, ios::binary | ios::app);
            if (!outFile) {
                cerr << "无法打开文件 " << filename << endl;
                return;
            }
            outFile.write(reinterpret_cast<const char*>(frame.data()), frame.size());
            cout << "已保存第 " << ++frame_count << " 帧（大小: " << frame.size() << " bytes）" << endl;
        };

        // 启动摄像头采集
        camera.start(callback);

        // 等待一段时间（例如 5 秒）
        this_thread::sleep_for(chrono::seconds(5));

        // 停止摄像头
        camera.stop();

        cout << "测试完成！检查生成的 test_frame.yuv 文件" << endl;
    } catch (const exception& e) {
        cerr << "CameraCapture 测试失败: " << e.what() << endl;
        return;
    }
}

int main() {
    test_camera_capture();
    return 0;
}