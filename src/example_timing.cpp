#include "ros/ros.h"
#include "benchmarking_tools/timing.h"
#include "benchmarking_tools/rate_tracker.h"
#include "std_msgs/String.h"


int main(int argc, char ** argv)
{
    ros::init(argc, argv, "topic_tester");
    ros::NodeHandle n;
    ros::Rate loop_rate(2);

    while (ros::ok()){
        {
            TIMING("logger1","INFO");
            ros::Duration(0.05).sleep();
        }

        {
            TIMING("logger2", "INFO");
            ros::Duration(0.05).sleep();
        }

        RATE_TRACKING_INFO_NAMED(50, "example_rate");

        ros::spinOnce();
    }
    return 0;
}
