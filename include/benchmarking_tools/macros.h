#ifndef BENCHMARKING_MACRO_H
#define BENCHMARKING_MACRO_H

#include <benchmarking_tools/tracker.h>

#include <benchmarking_tools/rate.h>
#include <benchmarking_tools/delay.h>
#include <benchmarking_tools/timingduration.h>

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

#define DELAY_LOG_THROTTLED(name, period, level, ws, header)\
    static std::map<std::string, benchmarking_tools::Tracker<benchmarking_tools::Delay>> dup_map_delay;\
    if (dup_map_delay.find(name) == dup_map_delay.end()){\
        benchmarking_tools::Tracker<benchmarking_tools::Delay> tracker(name, ws, level, period);\
        dup_map_delay.insert(std::pair<std::string, benchmarking_tools::Tracker<benchmarking_tools::Delay>>(name, tracker));\
    }\
    static benchmarking_tools::Delay delay_obj;\
    dup_map_delay.find(name)->second.addValue(delay_obj.addValue(header));\
    dup_map_delay.find(name)->second.getStringThrottled(delay_obj);\


#define DURATION_LOG(name, level, ws)\
    static std::map<std::string, benchmarking_tools::Tracker<benchmarking_tools::TimingDuration>> dup_map_duration;\
    if (dup_map_duration.find(name) == dup_map_duration.end()){\
        benchmarking_tools::Tracker<benchmarking_tools::TimingDuration> tracker(name, ws, level, 0);\
        dup_map_duration.insert(std::pair<std::string, benchmarking_tools::Tracker<benchmarking_tools::TimingDuration>>(name, tracker));
    }\
    benchmarking_tools::TimingDuration duration(&dup_map_duration.find(name)->second);\


#define DURATION_LOG_THROTTLED(name, level, ws, period)\
    static std::map<std::string, benchmarking_tools::Tracker<benchmarking_tools::TimingDuration>> dup_map_duration;\
    if (dup_map_duration.find(name) == dup_map_duration.end()){\
        benchmarking_tools::Tracker<benchmarking_tools::TimingDuration> tracker(name, ws, level, period);\
        dup_map_duration.insert(std::pair<std::string, benchmarking_tools::Tracker<benchmarking_tools::TimingDuration>>(name, tracker));
    }\
    benchmarking_tools::TimingDuration duration(&dup_map_duration.find(name)->second, true);\


#endif