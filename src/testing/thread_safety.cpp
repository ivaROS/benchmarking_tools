
#include "ros/ros.h"
#include "benchmarking_tools/benchmarking_tools2.h"
#include "std_msgs/String.h"
#include <thread>         // std::thread, std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

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

int pause(int size)
{
  std::this_thread::sleep_for (std::chrono::nanoseconds(size));
  return size;
}


int timedwork(int size)
{
  DURATION_INFO_STREAM_THROTTLED("busywork", 1000000, 1);
  return busywork(size);
  //return pause(size);
}


void worker(int size)
{
  while (ros::ok())
  {
    int rv = timedwork(size);
    //ROS_INFO_STREAM("Did work! " << rv << " [" << size << "]");
    //ros::Duration(0.005).sleep();
    //ros::spinOnce();
  } 
}


int main(int argc, char ** argv)
{
    ros::init(argc, argv, "rate_tester");
    ros::NodeHandle n;
    std_msgs::Header last_msgs;
    last_msgs.stamp = ros::Time::now();
    
    std::vector<unsigned int> sizes; //({20,22});
    for(int i = 0; i < 200; i++)
    {
      sizes.push_back(1);
    }
        
    auto threads = std::vector<std::thread>();
    
    for(auto size : sizes)
    {    
      threads.emplace_back([size]{
        worker(size);
      });
    }
    
    ros::spin();
    for(auto& threadObj : threads)
    {
      threadObj.join();
    }
    ROS_INFO_STREAM("Threads joined!");
    
    return 0;
}
