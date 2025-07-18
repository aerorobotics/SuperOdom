#!/usr/bin/env python3

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare
from pathlib import Path


def generate_launch_description():

    # Declare arguments
    declare_leaf_size_arg = DeclareLaunchArgument(
        "leaf_size",
        default_value="0.2",
        description="Voxel grid leaf size for map filtering",
    )

    declare_save_path_arg = DeclareLaunchArgument(
        "save_path", default_value="maps", description="Path to save PCD files"
    )

    # Get package path
    pkg_share = FindPackageShare("super_odometry")
    current_pkg = PathJoinSubstitution([pkg_share])

    # SuperOdometry Map Node
    superodom_map_node = Node(
        package="super_odometry",
        executable="map_node",
        output="screen",
        parameters=[
            {
                "map/sparse/leafSize": LaunchConfiguration("leaf_size"),
                "odom/odom_frame": "odom",
            }
        ],
        remappings=[
            ("laser_cloud_map", "/laser_cloud_map"),
            ("map", "/superodom/map"),
        ],
    )

    return LaunchDescription(
        [
            declare_leaf_size_arg,
            declare_save_path_arg,
            superodom_map_node,
        ]
    )
