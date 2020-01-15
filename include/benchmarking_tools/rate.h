#ifndef RATE_H
#define RATE_H

#include <ros/ros.h>
#include <string>
#include <boost/algorithm/string.hpp>
#include <chrono>
#include <map>

#include <list>
#include <std_msgs/Header.h>

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

        S addTime()
        {
            return ros::Time::now();
        }

        void getString(const std::list<S> vals, ::ros::console::Level level)
        {
            
        }
        
};


#endif