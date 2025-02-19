# 目标
通过usrp实现无线图传功能
# 流程
## 发送端
A[摄像头采集的YUYV422原始数据] --> B[YUV420P]
B --> C[FFmpeg编码H264封装为AVPacket]
C --> D[提取NAL单元]
D --> E[RTP封装]
E --> F[OFDM调制]
F --> G[USRP B210发送]
## 接收端
G --> H[USRP B210接收]
H --> I[OFDM解调]
I --> J[通过UDP管道播放RTP视频流]
