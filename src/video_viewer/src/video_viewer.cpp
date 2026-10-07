#include <chrono>
#include <memory>

#include <cv_bridge/cv_bridge.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>

using Image = sensor_msgs::msg::Image;

class XinhuoRos : public rclcpp::Node
{
public:
  XinhuoRos() : Node("video_viewer")
  {
    auto qos = rclcpp::SensorDataQoS().keep_last(1);
    if (declare_parameter<bool>("use_reliable", false)) {
      qos.reliable();
    }

    image_sub_ = create_subscription<Image>(
      "image_raw", qos,
      [this](Image::ConstSharedPtr message) { on_image(message); });

    gui_timer_ = create_wall_timer(
      std::chrono::milliseconds(20),
      []() { cv::waitKey(1); });
  }

private:
  void on_image(const Image::ConstSharedPtr & message)
  {
    const auto image = cv_bridge::toCvShare(message, "bgr8");
    cv::imshow("ROS 2 video", image->image);
  }

  rclcpp::Subscription<Image>::SharedPtr image_sub_;
  rclcpp::TimerBase::SharedPtr gui_timer_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<XinhuoRos>());
  cv::destroyAllWindows();
  rclcpp::shutdown();
  return 0;
}
