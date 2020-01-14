#ifndef TIMING_H
#define TIMING_H

#include <ros/ros.h>
#include <string>
#include <boost/algorithm/string.hpp>
#include <chrono>
#include <map>

#define TESTER "sample_str"
#define TIMING(FUNC, LEVEL) TimerObject timer = TimerObject(FUNC, LEVEL)

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

class TimerObject
{
private:
    ros::WallTime start_time;
    float elapsed;
    std::string scope;
    std::string function;
    std::string info_level;
public:
    TimerObject(std::string func, std::string level)
    {
        this->function = func;
        this->info_level = level;
        boost::to_lower(this->info_level);
        start_time = ros::WallTime::now();
    }

    ~TimerObject(){
        elapsed = (ros::WallTime::now() - this->start_time).toSec() * 1e3;
        ROS_INFO_STREAM_NAMED("timing." + this->function, this->function + "," + std::to_string(elapsed) + "ms");
    }
};

#pragma pop_macro("ROSCONSOLE_DEFINE_LOCATION(cond, level, name)")

#endif