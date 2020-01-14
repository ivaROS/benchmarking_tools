#include "ros/ros.h"
#include "benchmarking_tools/timing.h"
#include "benchmarking_tools/rate_tracker.h"
#include "std_msgs/String.h"


int main(int argc, char ** argv)
{
    ros::init(argc, argv, "rate_tester");
    ros::NodeHandle n;
    ros::Rate loop_rate(2);
    // benchmarking_tools::RateTracker example(50, "Example_Rate");
    while (ros::ok()){
        RATE_TRACKING_INFO_NAMED(50, "example_rate");
        ros::Duration(0.05).sleep();
        ros::spinOnce();
    }
    return 0;
}
