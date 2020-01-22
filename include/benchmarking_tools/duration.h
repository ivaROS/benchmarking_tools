#ifndef DURATION_H
#define DURATION_H


#include <ros/ros.h>
#include <string>
#include <boost/algorithm/string.hpp>
#include <benchmarking_tools/tracker.h>
#include <chrono>
#include <map>
#include <list>
#include <std_msgs/Header.h>

namespace benchmarking_tools
{
    class TimingDuration
    {
        // public:
        private:
            ros::WallTime start_time;
            float elapsed_time;
            benchmarking_tools::Tracker<benchmarking_tools::TimingDuration> * _parentobj;
        public:
            typedef float S;
            TimingDuration(benchmarking_tools::Tracker<benchmarking_tools::TimingDuration> * parentobj)
            {
                ROS_INFO_STREAM("Constructor started");
                start_time = ros::WallTime::now();
                ROS_INFO_STREAM("Constructor finished");
                _parentobj = parentobj;
            }

            S addValue()
            {
                return this->elapsed_time;
            }

            ~TimingDuration()
            {
                ROS_INFO_STREAM("destructor1 reached");
                this->elapsed_time = (ros::WallTime::now() - this->start_time).toSec() * 1e3;
                ROS_INFO_STREAM("destructor2 reached");
                ROS_INFO_STREAM(this->addValue());
                _parentobj->addValue(this->addValue());
                ROS_INFO_STREAM("destructor3 reached");
                _parentobj->getString(*this);
            }

            static std::string getString(const std::list<S> & vals)
            {
                if (vals.size() < 2)
                    return "ave duration 0";

                double ave_delay = std::accumulate(std::begin(vals), std::end(vals), 0) / ((double)vals.size());
                std::string return_str = "ave duration " + std::to_string(ave_delay);
                return return_str;
            }
    };
}

#endif