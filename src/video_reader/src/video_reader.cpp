#include <chrono>
#include <memory>
#include <stdexcept>
#include <string>

#include <cv_bridge/cv_bridge.hpp>
#include <opencv2/videoio.hpp>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <std_msgs/msg/header.hpp>

using Image = sensor_msgs::msg::Image;

class XinhuoRos : public rclcpp::Node
{
public:
  XinhuoRos() : Node("video_reader")
  {
    const auto path = declare_parameter<std::string>("video_path", "video.mp4");
    if (!capture_.open(path)) {
      throw std::runtime_error("无法打开视频：" + path);
    }

    double fps = declare_parameter<double>("fps", 0.0);
    if (fps <= 0.0) {
      fps = capture_.get(cv::CAP_PROP_FPS);
    }
    if (fps <= 0.0) {
      fps = 30.0;
    }

    image_pub_ = create_publisher<Image>(
      "image_raw", rclcpp::SensorDataQoS().keep_last(1));

    timer_ = create_wall_timer(
      std::chrono::duration<double>(1.0 / fps),
      [this]() { publish_frame(); });
  }

private:
  void publish_frame()
  {
    cv::Mat frame;
    if (!capture_.read(frame)) {
      capture_.set(cv::CAP_PROP_POS_FRAMES, 0);
      capture_.read(frame);
    }
    if (frame.empty()) {
      return;
    }

    std_msgs::msg::Header header;
    header.stamp = now();
    auto message = cv_bridge::CvImage(header, "bgr8", frame).toImageMsg();
    image_pub_->publish(*message);
  }

  cv::VideoCapture capture_;
  rclcpp::Publisher<Image>::SharedPtr image_pub_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<XinhuoRos>());
  rclcpp::shutdown();
  return 0;
}
