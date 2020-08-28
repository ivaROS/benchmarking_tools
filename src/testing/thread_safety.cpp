
#include "ros/ros.h"
#include "benchmarking_tools/benchmarking_tools2.h"
#include "std_msgs/String.h"
#include <thread>


int busywork(int size)
{
  int sum = 0;
  for(int i = 0; i < pow(2,size); i++)
  {
    for(int j=0; j < size + sum % 8; j++)
    {
      sum += j;
    }
  }
  return sum;
}


int timedwork(int size)
{
  DURATION_INFO_STREAM("busywork", 20);
  return busywork(size);
}


void worker(int size)
{
  while (ros::ok())
  {
    int rv = timedwork(size);
    ROS_INFO_STREAM("Did work! " << rv);
    ros::Duration(0.005).sleep();
    ros::spinOnce();
  } 
}


int main(int argc, char ** argv)
{
    ros::init(argc, argv, "rate_tester");
    ros::NodeHandle n;
    std_msgs::Header last_msgs;
    last_msgs.stamp = ros::Time::now();
    
    int size = 20;
    
    std::thread threadObj([size]{
      worker(size);
    });
    
    //worker(size);
    
    threadObj.join();
    return 0;
}
