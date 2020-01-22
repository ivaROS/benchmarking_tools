
#include "ros/ros.h"
#include "benchmarking_tools/macros.h"
#include "std_msgs/String.h"


int main(int argc, char ** argv)
{
    ros::init(argc, argv, "rate_tester");
    ros::NodeHandle n;
    ros::Rate loop_rate(2);
    std_msgs::Header last_msgs;
    last_msgs.stamp = ros::Time::now();
    while (ros::ok()){
        RATE_LOG("example_rate node", ros::console::Level::Info, 50);
        ros::Duration(0.05).sleep();
        DELAY_LOG("delay log", ros::console::Level::Info, 50, last_msgs);
        last_msgs.stamp = ros::Time::now();
        {
            ROS_INFO_STREAM("Timing reached");
            DURATION_LOG("duration", ros::console::Level::Info, 50);
            ros::Duration(0.5).sleep();
        }
        ros::spinOnce();
    }
    return 0;
}
