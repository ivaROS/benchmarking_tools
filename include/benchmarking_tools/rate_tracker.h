#ifndef RATE_TRACK_H
#define RATE_TRACK_H

#include <ros/ros.h>
#include <string>
#include <boost/algorithm/string.hpp>
#include <chrono>
#include <map>

#include <list>
#include <std_msgs/Header.h>



#define RATE_TRACKING_INFO_NAMED(ws, name)\
    static std::map<std::string, benchmarking_tools::RateTracker> dup_map;\
    if (dup_map.find(name) == dup_map.end()){\
        benchmarking_tools::RateTracker tracker(ws, name);\
        dup_map.insert(std::pair<std::string, benchmarking_tools::RateTracker>(name, tracker));\
    }\
    dup_map[name].addTimeAndRun();\

// Pass in message level to not duplicate w/ if else functions
// build flag to disable

#pragma push_macro("ROSCONSOLE_DEFINE_LOCATION(cond, level, name)")

// Redefined so there might exists multiple rate logger names
#define ROSCONSOLE_DEFINE_LOCATION(cond, level, name) \
    ROSCONSOLE_AUTOINIT; \
    static std::map<std::string, ::ros::console::LogLocation> dup_map;\
    if (dup_map.find(name) == dup_map.end()){\
        dup_map.insert(std::pair<std::string, ::ros::console::LogLocation>(name, {false, false, ::ros::console::levels::Count, NULL}));\
    }\
    ::ros::console::LogLocation& __rosconsole_define_location__loc = dup_map[name];\
    if (ROS_UNLIKELY(!__rosconsole_define_location__loc.initialized_)) \
    { \
        initializeLogLocation(&__rosconsole_define_location__loc, name, level); \
    } \
    if (ROS_UNLIKELY(__rosconsole_define_location__loc.level_ != level)) \
    { \
        setLogLocationLevel(&__rosconsole_define_location__loc, level); \
        checkLogLocationEnabled(&__rosconsole_define_location__loc); \
    } \
    bool __rosconsole_define_location__enabled = __rosconsole_define_location__loc.logger_enabled_ && (cond);

namespace benchmarking_tools 
{
    class RateTracker
    {
        private:
            std::string name;
            size_t max_size = 50;
            size_t num_samples = 0;
            std::list<ros::Time> times;
            std::list<ros::Duration> delays;
        
        public:
            RateTracker(size_t window_size = 50, std::string tracker_name = "undefined") : max_size(window_size), name(tracker_name)
            {
                ROS_INFO_STREAM("new added " << max_size << " sample");
            }

            ~RateTracker(){};

            void addTime(ros::Time time)
            {
                if (times.size() == max_size)
                    times.pop_front();

                times.push_back(time);
            }

            void addDuration(ros::Time msgTime, ros::Time actualTime)
            {
                if (delays.size() == max_size)
                    delays.pop_front();

                ros::Duration delay = actualTime - msgTime;
                delays.push_back(delay);
            }

            void addTime(std_msgs::Header header)
            {
                addTime(header.stamp);
                addDuration(header.stamp, ros::Time::now());
            }

            double getRate()
            {
                if (times.size() < 2)
                    return 0;

                ros::Duration dt = times.back() - times.front();
                double rate = ((double)times.size() - 1) / dt.toSec();
                return rate;
            }

            double getLastDelay()
            {
                if (delays.size() == 0) {
                    return 0;
                } else {
                    return delays.back().toSec();
                }
            }

            double getAverageDelay()
            {
                ros::Duration total_delay = ros::Duration(0);
                for (std::list<ros::Duration>::iterator it = delays.begin(); it != delays.end(); ++it)
                {
                    total_delay += *it;
                }

                return total_delay.toSec() / ((double)delays.size());
            }

            size_t getNumSamples()
            {
                return times.size();
            }

            void addTimeAndRun()
            {
                addTime(ros::Time::now());
                ROS_INFO_STREAM_NAMED("rate." + this->name, this->name + " callback rate: "
                << this->getRate()
                << "s, (" << this->getNumSamples() << " samples)." );
            }
    };
}

#pragma pop_macro("ROSCONSOLE_DEFINE_LOCATION(cond, level, name)")
#endif