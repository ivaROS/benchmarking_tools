
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
    ROS_INFO_STREAM("Did work! " << rv << " [" << size << "]");
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
    
    std::vector<unsigned int> sizes({14,16,18,20,22,24});
    for(int i = 0; i < 10; i++)
    {
      //sizes.push_back(10);
    }
        
    auto threads = std::vector<std::thread>();
    
    for(auto size : sizes)
    {    
      threads.emplace_back([size]{
        worker(size);
      });
    }
    
    for(auto& threadObj : threads)
    {
      threadObj.join();
    }
    ROS_INFO_STREAM("Threads joined!");
    
    return 0;
}
