#include "ros/ros.h"
#include "benchmarking_tools/macros.h"
#include "std_msgs/String.h"


int main(int argc, char ** argv)
{
    ros::init(argc, argv, "topic_tester");
    ros::NodeHandle n;
    ros::Rate loop_rate(2);

    while (ros::ok()){
        RATE_LOG("example_rate node_2", ros::console::Level::Info, 5, std::string(ROSCONSOLE_NAME_PREFIX));
        ros::Duration(0.5).sleep();
        ros::spinOnce();
    }
    return 0;
}
