# 薪火培训 ROS 2 示例

1. `video_reader` 定时读取视频并发布 `/image_raw`
2. `video_viewer` 订阅 `/image_raw` 并显示图像
3. 两端使用 `SensorDataQoS().keep_last(1)`
4. `demo.launch.py` 一次启动两个节点

## 准备视频
视频路径为 `xinhuo_example/video.mp4`。

```text
xinhuo_example/video.mp4
```

运行命令需要在 `xinhuo_example` 根目录执行。若文件名不同，可以通过 `video_path` 参数指定。

## 编译

在 `xinhuo_example` 根目录执行：

```bash
source /opt/ros/jazzy/setup.bash
colcon build
source install/setup.bash
```

Ubuntu 24.04 安装依赖：

```bash
sudo apt update
sudo apt install ros-jazzy-ros-base ros-jazzy-cv-bridge \
  libopencv-dev python3-colcon-common-extensions
```

不要把其他版本的 OpenCV 安装到 `/usr/local`。工程会使用 Ubuntu 24.04 自带的 OpenCV。

## 分别运行两个节点

终端 A：

```bash
source install/setup.bash
ros2 run video_reader videor_reader
```

如果文件名不是 `video.mp4`：

```bash
ros2 run video_reader videor_reader --ros-args \
  -p video_path:="其他视频文件.mp4"
```

终端 B：

```bash
source install/setup.bash
ros2 run video_viewer videor_viewer
```

需要指定播放帧率时，在读取节点后增加 `-p fps:=5.0`。不指定时使用视频自身的帧率。

## 查看 Topic

```bash
ros2 topic list -t
ros2 topic info /image_raw --verbose
ros2 topic hz /image_raw
ros2 topic echo /image_raw --no-arr --once \
  --qos-reliability best_effort
```

## QoS 不匹配实验

先停止显示节点，再运行：

```bash
ros2 run video_viewer videor_viewer --ros-args \
  -p use_reliable:=true
```

读取节点使用 Best Effort，显示节点改为 Reliable 后无法匹配，因此没有画面。去掉参数重新启动即可恢复。

## 使用 Launch

在项目根目录运行：

```bash
ros2 launch video_viewer demo.launch.py
```

## 课堂练习：显示灰度图

在 `video_viewer.cpp` 的 `on_image()` 中，把：

```cpp
cv::imshow("ROS 2 video", image->image);
```

替换为：

```cpp
cv::Mat gray;
cv::cvtColor(image->image, gray, cv::COLOR_BGR2GRAY);
cv::imshow("ROS 2 video", gray);
```

然后重新编译显示包：

```bash
colcon build --packages-select video_viewer
source install/setup.bash
ros2 run video_viewer videor_viewer
```
