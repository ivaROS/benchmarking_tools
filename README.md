# benchmarking_tools

`benchmarking_tools` is a performance benchmarking toolbox built around around `rosconsole` that provides convenient benchmarking capabilities to ros packages, and experiment launching system through `rospy`.

## Benchmarking Logging Insertions

It provides three base types of benchmarking functions, each at 3 different verbosity levels. The naming syntax convention follows:

``
<FUNCTION>_<VERBOSITY_LEVEL>_STREAM(_THROTTLED)
``

### Current Functions
Default library provides the following three functions that measures the following
- `DURATION` The execution time of the scope of the inserted line.
- `RATE` The rate at the insertion is being called
- `DELAY` The delay between the passed in message and the current time, measures system latency overhead. 

### Extending Features
The repo works through an overarching `Tracker` class that holds the data and provides the functionality for log benchmarking messages to ROS.

To extend the feature, a measurement class need to be created for the specific measurement. This class should provide an `addValue` function that returns the desired measurement to the `Tracker` class, and a `getString` function that takes in a `list` of datatypes, compute the average and return a string to `Tracker` class.

For minimal implementation example, see `include/benchmarking_tools/delay.h`

### Examples

```
#include "benchmarking_tools.h"
while (true)
{
    // measures the rate of this line being called.
    RATE_INFO_STREAM("name1", 50);
    {
        // This block of code will be Timed
        DURATION_INFO_STREAM("name2", 50);
        EXECUTION_CODE
    }

    {
        Duration of this block of code will not be timed
    }
}
```

## Launching Benchmarking Experiments

