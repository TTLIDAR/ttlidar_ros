/*
 *  TTldiar System
 *  Driver Interface
 *
 *  Copyright 2021 TT Team
 *  All rights reserved. 
 *
 *	Author: Titans  2021-2-21
 *
 */


#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"

#define RAD2DEG(x) ((x)*180./M_PI)

void scanCallback(const sensor_msgs::LaserScan::ConstPtr& scan)
{
    int count = scan->scan_time / scan->time_increment;
    ROS_INFO("recv a laser scan %s[%d]*********************", scan->header.frame_id.c_str(), count);
    ROS_INFO("angle_range, %f, %f     ***************", RAD2DEG(scan->angle_min), RAD2DEG(scan->angle_max));
  
    for(int i = 0; i < count; i++) {
        float degree = RAD2DEG(scan->angle_min + scan->angle_increment * i);
        ROS_INFO(": [%f, %f]", degree, scan->ranges[i]);
    }
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "subscribe_demo_node"); 
    ros::NodeHandle n;

    ros::Subscriber sub = n.subscribe<sensor_msgs::LaserScan>("/scan", 1000, scanCallback);

    ros::spin();

    return 0;
}
