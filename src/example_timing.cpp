#include "ros/ros.h"
#include "benchmarking_tools/timing.h"
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

        ros::spinOnce();
    }
    return 0;
}
