#ifndef BENCHMARKING_H
#define BENCHMARKING_H


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


#include <benchmarking_tools/rate.h>
#include <benchmarking_tools/delay.h>
#include <benchmarking_tools/tracker.h>

#define RATE_LOG(name, level, ws)\
    static std::map<std::string, benchmarking_tools::Tracker<benchmarking_tools::Rate>> dup_map;\
    if (dup_map.find(name) == dup_map.end()){\
        benchmarking_tools::Tracker<benchmarking_tools::Rate> tracker(name, ws, level, 0);\
        dup_map.insert(std::pair<std::string, benchmarking_tools::Tracker<benchmarking_tools::Rate>>(name, tracker));\
    }\
    benchmarking_tools::Rate rate_obj;\
    dup_map.find(name)->second.addValue(rate_obj.addValue());\
    dup_map.find(name)->second.getString(rate_obj);\

#define RATE_LOG_THROTTLED(name, period, level, ws)\
    static std::map<std::string, benchmarking_tools::Tracker<benchmarking_tools::Rate>> dup_map;\
    if (dup_map.find(name) == dup_map.end()){\
        benchmarking_tools::Tracker<benchmarking_tools::Rate> tracker(name, ws, level, period);\
        dup_map.insert(std::pair<std::string, benchmarking_tools::Tracker<benchmarking_tools::Rate>>(name, tracker));\
    }\
    static benchmarking_tools::Rate rate_obj;\
    dup_map.find(name)->second.addValue(rate_obj.addValue());\
    dup_map.find(name)->second.getStringThrottled(rate_obj);\

#define DELAY_LOG(name, level, ws, header)\
    static std::map<std::string, benchmarking_tools::Tracker<benchmarking_tools::Delay>> dup_map_delay;\
    if (dup_map_delay.find(name) == dup_map_delay.end()){\
        benchmarking_tools::Tracker<benchmarking_tools::Delay> tracker(name, ws, level, 0);\
        dup_map_delay.insert(std::pair<std::string, benchmarking_tools::Tracker<benchmarking_tools::Delay>>(name, tracker));\
    }\
    static benchmarking_tools::Delay delay_obj;\
    dup_map_delay.find(name)->second.addValue(delay_obj.addValue(header));\
    dup_map_delay.find(name)->second.getString(delay_obj);\

#define DELAY_LOG_THROTTLED(name, period, level, ws)\
    static std::map<std::string, benchmarking_tools::Tracker<benchmarking_tools::Delay>> dup_map_delay;\
    if (dup_map_delay.find(name) == dup_map_delay.end()){\
        benchmarking_tools::Tracker<benchmarking_tools::Delay> tracker(name, ws, level, period);\
        dup_map_delay.insert(std::pair<std::string, benchmarking_tools::Tracker<benchmarking_tools::Delay>>(name, tracker));\
    }\
    static benchmarking_tools::Delay delay_obj;\
    dup_map_delay.find(name)->second.addValue(delay_obj.addValue(header));\
    dup_map_delay.find(name)->second.getStringThrottled(delay_obj);\

#pragma pop_macro("ROSCONSOLE_DEFINE_LOCATION(cond, level, name)")
#endif