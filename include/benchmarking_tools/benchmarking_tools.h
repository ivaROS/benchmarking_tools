#ifndef BENCHMARKING_H
#define BENCHMARKING_H


#include <ros/ros.h>

// #ifdef ROS_PACKAGE_NAME
// #define ROSCONSOLE_PACKAGE_NAME ROS_PACKAGE_NAME
// #else
// #define ROSCONSOLE_PACKAGE_NAME "unknown_package"
// #endif

// #define ROSCONSOLE_ROOT_LOGGER_NAME "ros"
// #define ROSCONSOLE_NAME_PREFIX ROSCONSOLE_ROOT_LOGGER_NAME "." ROSCONSOLE_PACKAGE_NAME

#pragma push_macro("ROSCONSOLE_DEFINE_LOCATION(cond, level, name)")

// Redefined so there might exists multiple rate logger names
#undef ROSCONSOLE_DEFINE_LOCATION

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

#include <benchmarking_tools/macros.h>

#define RATE_DEBUG_STREAM(name, ws) RATE_LOG(name, ::ros::console::Level::Debug, ws, std::string(ROSCONSOLE_NAME_PREFIX))
#define RATE_INFO_STREAM(name, ws) RATE_LOG(name, ::ros::console::Level::Info, ws, std::string(ROSCONSOLE_NAME_PREFIX))
#define RATE_WARN_STREAM(name, ws) RATE_LOG(name, ::ros::console::Level::Warn, w, std::string(ROSCONSOLE_NAME_PREFIX))
#define RATE_DEBUG_STREAM_THROTTLED(name, ws, period) RATE_LOG_THROTTLED(name, period, ::ros::console::Level::Debug, ws, std::string(ROSCONSOLE_NAME_PREFIX))
#define RATE_INFO_STREAM_THROTTLED(name, ws, period) RATE_LOG_THROTTLED(name, period, ::ros::console::Level::Info, ws, std::string(ROSCONSOLE_NAME_PREFIX))
#define RATE_WARN_STREAM_THROTTLED(name, ws, period) RATE_LOG_THROTTLED(name, period, ::ros::console::Level::Warn, ws, std::string(ROSCONSOLE_NAME_PREFIX))

#define DELAY_DEBUG_STREAM(name, ws, header) DELAY_LOG(name, ::ros::console::Level::Debug, ws, header, std::string(ROSCONSOLE_NAME_PREFIX))
#define DELAY_INFO_STREAM(name, ws, header) DELAY_LOG(name, ::ros::console::Level::Info, ws, header, std::string(ROSCONSOLE_NAME_PREFIX))
#define DELAY_WARN_STREAM(name, ws, header) DELAY_LOG(name, ::ros::console::Level::Warn, ws, header, std::string(ROSCONSOLE_NAME_PREFIX))
#define DELAY_DEBUG_STREAM_THROTTLED(name, ws, header, period) DELAY_LOG_THROTTLED(name, period, ::ros::console::Level::Debug, ws, header, std::string(ROSCONSOLE_NAME_PREFIX))
#define DELAY_INFO_STREAM_THROTTLED(name, ws, header, period) DELAY_LOG_THROTTLED(name, period, ::ros::console::Level::Info, ws, header, std::string(ROSCONSOLE_NAME_PREFIX))
#define DELAY_WARN_STREAM_THROTTLED(name, ws, header, period) DELAY_LOG_THROTTLED(name, period, ::ros::console::Level::Warn, ws, header, std::string(ROSCONSOLE_NAME_PREFIX))

#define DURATION_DEBUG_STREAM(name, ws) DURATION_LOG(name, ::ros::console::Level::Debug, ws, std::string(ROSCONSOLE_NAME_PREFIX))
#define DURATION_INFO_STREAM(name, ws) DURATION_LOG(name, ::ros::console::Level::Info, ws, std::string(ROSCONSOLE_NAME_PREFIX))
#define DURATION_WARN_STREAM(name, ws) DURATION_LOG(name, ::ros::console::Level::Warn, ws, std::string(ROSCONSOLE_NAME_PREFIX))
#define DURATION_DEBUG_STREAM_THROTTLED(name, ws, period) DURATION_LOG_THROTTLED(name, period, ::ros::console::Level::Debug, ws, std::string(ROSCONSOLE_NAME_PREFIX))
#define DURATION_INFO_STREAM_THROTTLED(name, ws, period) DURATION_LOG_THROTTLED(name, period, ::ros::console::Level::Info, ws, std::string(ROSCONSOLE_NAME_PREFIX))
#define DURATION_WARN_STREAM_THROTTLED(name, ws, period) DURATION_LOG_THROTTLED(name, period, ::ros::console::Level::Warn, ws, std::string(ROSCONSOLE_NAME_PREFIX))



#pragma pop_macro("ROSCONSOLE_DEFINE_LOCATION(cond, level, name)")


#endif