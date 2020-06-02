#ifndef DELAY_H
#define DELAY_H

#include <ros/ros.h>
#include <string>
#include <boost/algorithm/string.hpp>
#include <boost/circular_buffer.hpp>
#include <numeric>
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

            std::string getString(const boost::circular_buffer<S> &vals)
            {
                if (vals.size() < 2)
                    return "delay 0";

                double ave_delay = (double) (std::accumulate(std::begin(vals), std::end(vals), ros::Duration(0))).toSec() / ((double)vals.size());
                std::string return_str = "delay " + std::to_string(ave_delay);
                return return_str;
            }
            
    };
}


#endif
