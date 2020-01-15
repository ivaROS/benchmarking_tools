#ifndef TRACKER_H
#define TRACKER_H

#include <ros/ros.h>
#include <string>
#include <boost/algorithm/string.hpp>
#include <chrono>
#include <map>

#include <list>
#include <std_msgs/Header.h>


template <typename T>
class Tracker
{
private:
    std::string _name;
    size_t max_size = 50;
    size_t num_samples = 0;
    ::ros::console::Level _level;
    std::list<typename T:: S> _vals;
public:
    Tracker(std::string name, size_t window_size, ::ros::console::Level level):
    _name(name), max_size(window_size), _level(level) {}

    ~Tracker(){}

    void addValue(const typename T:: S& val)
    {
        if (_vals.size() >= max_size)
        {
            _vals.pop_front();
        }

        _vals.push_back(val);
    }

    void getString(T interface_object)
    {
        return interface_object.getString(_vals, _level);
    }
};




#endif