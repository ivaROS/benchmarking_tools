#ifndef DELAY_H
#define DELAY_H

#include <ros/ros.h>
#include <string>
#include <boost/algorithm/string.hpp>
#include <chrono>
#include <map>
#include <list>
#include <std_msgs/Header.h>

namespace benchmarking_tools
{
    class Delay
    {
        private:
        public:
            typedef ros::Duration S; 
            Delay()
            {

            }

            ~Delay(){

            }

            S addValue(std_msgs::Header header)
            {
                ros::Duration delay = ros::Time::now() - header.stamp;
                return delay;
            }

            std::string getString(const std::list<S> &vals)
            {
                if (vals.size() < 2)
                    return "ave_delay 0";

                double ave_delay = (double) (std::accumulate(std::begin(vals), std::end(vals), ros::Duration(0))).toSec() / ((double)vals.size());
                std::string return_str = "ave_delay " + std::to_string(ave_delay);
                return return_str;
            }
            
    };
}


#endif