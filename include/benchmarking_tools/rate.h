#ifndef RATE_H
#define RATE_H

#include <ros/ros.h>
#include <string>
#include <boost/algorithm/string.hpp>
#include <boost/circular_buffer.hpp>
#include <chrono>
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

            std::string getString(const boost::circular_buffer<S> &vals)
            {
                if (vals.size() < 2)
                    return "callrate 0";

                ros::Duration dt = vals.back() - vals.front();
                double rate = ((double)vals.size() - 1) / dt.toSec();
                std::string return_str = "callrate " + std::to_string(rate);
                return return_str;
            }
            
    };
}


#endif