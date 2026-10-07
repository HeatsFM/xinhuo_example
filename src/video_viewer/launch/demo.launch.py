from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    return LaunchDescription([
        Node(package="video_reader", executable="videor_reader",
             output="screen"),
        Node(package="video_viewer", executable="videor_viewer",
             output="screen"),
    ])
