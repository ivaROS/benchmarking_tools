
#include "ros/ros.h"
#include "ros/console.h"
#include "benchmarking_tools/rate_tracker.h"
#include "benchmarking_tools/rate.h"
#include "benchmarking_tools/tracker.h"
#include "std_msgs/String.h"


int main(int argc, char ** argv)
{
    ros::init(argc, argv, "rate_tester");
    ros::NodeHandle n;
    ros::Rate loop_rate(2);

    Tracker<Rate> tracker_obj("temp_name", 50, ros::console::levels::Info);

    while (ros::ok()){
        
    }
    return 0;
}
