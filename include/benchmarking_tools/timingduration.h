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
        public:
            typedef float S;
        private:
            ros::WallTime start_time;
            float elapsed_time;
            benchmarking_tools::Tracker<benchmarking_tools::TimingDuration> * _parentobj;
            bool _throttled = false;
        public:
            TimingDuration(benchmarking_tools::Tracker<benchmarking_tools::TimingDuration> * parentobj, bool throttled = false)
            {
                start_time = ros::WallTime::now();
                _parentobj = parentobj;
                _throttled = throttled;
            }

            S addValue()
            {
                return this->elapsed_time;
            }

            ~TimingDuration()
            {
                this->elapsed_time = (ros::WallTime::now() - this->start_time).toSec() * 1e3;
                ROS_INFO_STREAM(this->addValue());
                _parentobj->addValue(this->addValue());

                if (!_throttled){
                    _parentobj->getString(*this);
                } else {
                    _parentobj->getStringThrottled(*this);
                }
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