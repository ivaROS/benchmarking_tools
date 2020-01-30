#ifndef RATE_H
#define RATE_H

#include <ros/ros.h>
#include <string>
#include <boost/algorithm/string.hpp>
#include <chrono>
#include <map>
#include <list>
#include <std_msgs/Header.h>

namespace benchmarking_tools
{
    class Rate
    {
        private:
        public:
            typedef ros::Time S; 
            Rate()
            {

            }

            ~Rate(){

            }

            S addValue()
            {
                return ros::Time::now();
            }

            std::string getString(const std::list<S> &vals)
            {
                if (vals.size() < 2)
                    return "ave_callrate 0";

                ros::Duration dt = vals.back() - vals.front();
                double rate = ((double)vals.size() - 1) / dt.toSec();
                std::string return_str = "ave_callrate " + std::to_string(rate);
                return return_str;
            }
            
    };
}


#endif