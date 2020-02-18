#ifndef TRACKER_H
#define TRACKER_H

#include <ros/ros.h>
#include <string>
#include <boost/algorithm/string.hpp>
#include <chrono>
#include <map>
#include <list>
#include <std_msgs/Header.h>

namespace benchmarking_tools{
    template <typename T>
    class Tracker
    {
    private:
        std::string _name;
        std::string _name_prefix;
        size_t max_size = 50;
        size_t num_samples = 0;
        ::ros::console::Level _level;
        std::list<typename T:: S> _vals;
        float _period;
    public:
        // NOTE: If this default constructor is required by compiler error,
        // it indicates that the tracker is not being inserted into the map.
        // Tracker(){}
        
        Tracker(std::string name, size_t window_size, ::ros::console::Level level, float period, std::string name_prefix):
        _name(name), max_size(window_size), _level(level), _period(period), _name_prefix(name_prefix) {}

        ~Tracker(){}

        void addValue(const typename T:: S val)
        {
            if (_vals.size() >= max_size)
            {
                _vals.pop_front();
            }

            _vals.push_back(val);
        }

        void getString(T& interface_object)
        {
            std::string value = interface_object.getString(_vals);
            ROS_LOG_STREAM(_level, _name_prefix + "." + _name, _name << ", " << value << "," << _vals.size() << " samples");
        }


        void getStringThrottled(T& interface_object)
        {
            std::string value = interface_object.getString(_vals);
            ROS_LOG_STREAM_THROTTLE(_period, _level, _name_prefix + "." + _name, _name << ", " << value << "," << _vals.size() << " samples");
        }
    };
}




#endif
