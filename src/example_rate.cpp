
#include "ros/ros.h"
#include "benchmarking_tools/benchmarking_tools.h"
#include "std_msgs/String.h"


int main(int argc, char ** argv)
{
    ros::init(argc, argv, "rate_tester");
    ros::NodeHandle n;
    std_msgs::Header last_msgs;
    last_msgs.stamp = ros::Time::now();
    while (ros::ok()){
        // ROS_INFO_STREAM(ROS_PACKAGE_NAME);
        // PLACEHOLDER;
        RATE_INFO_STREAM("rate_node", 50);
        DELAY_INFO_STREAM("delay_log", 50, last_msgs);
        last_msgs.stamp = ros::Time::now();
        {
            DURATION_INFO_STREAM("duration", 50);
            ros::Duration(0.005).sleep();
        }
        ros::spinOnce();
    }
    return 0;
}
