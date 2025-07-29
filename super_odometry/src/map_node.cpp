#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/io/pcd_io.h>
#include <memory>
#include <string>

// Include the new service definition
#include "super_odometry/srv/save_point_map.hpp"

using std::placeholders::_1;
using std::placeholders::_2;

class MapNode : public rclcpp::Node {
public:
    MapNode() : Node("map_node") {
        // Subscribe to the map topic (adjust topic name if needed)
        map_sub_ = this->create_subscription<sensor_msgs::msg::PointCloud2>(
            "laser_cloud_map", 10,
            std::bind(&MapNode::mapCallback, this, _1));

        // Create the save_point_map service
        save_srv_ = this->create_service<super_odometry::srv::SavePointMap>(
            "save_point_map",
            std::bind(&MapNode::savePointMap, this, _1, _2));

        map_.reset(new pcl::PointCloud<pcl::PointXYZI>());
    }

private:
    void mapCallback(const sensor_msgs::msg::PointCloud2::SharedPtr msg) {
        pcl::fromROSMsg(*msg, *map_);
    }

    void savePointMap(
        const std::shared_ptr<super_odometry::srv::SavePointMap::Request> req,
        std::shared_ptr<super_odometry::srv::SavePointMap::Response> res)
    {
        RCLCPP_INFO(this->get_logger(), "Saving map to %s with leaf size %.2f...", req->save_path.c_str(), req->leaf_size);
        auto filtered_map = pcl::PointCloud<pcl::PointXYZI>::Ptr(new pcl::PointCloud<pcl::PointXYZI>(*map_));
        pcl::VoxelGrid<pcl::PointXYZI> vg;
        vg.setLeafSize(req->leaf_size, req->leaf_size, req->leaf_size);
        vg.setInputCloud(filtered_map);
        vg.filter(*filtered_map);

        std::string filename = req->save_path + "/superodom_map.pcd";
        int ret = pcl::io::savePCDFileBinary(filename, *filtered_map);
        res->success = (ret == 0);
        RCLCPP_INFO(this->get_logger(), "Saving map to %s with leaf size %.2f... %s",
                    filename.c_str(), req->leaf_size, res->success ? "done" : "failed");
    }

    rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr map_sub_;
    rclcpp::Service<super_odometry::srv::SavePointMap>::SharedPtr save_srv_;
    pcl::PointCloud<pcl::PointXYZI>::Ptr map_;
};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<MapNode>());
    rclcpp::shutdown();
    return 0;
} 