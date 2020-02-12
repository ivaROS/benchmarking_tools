#!/usr/bin/env python
import execution

execution.run_test(rosbag_launch_dir = 'launch/rosbag', tests = ["scalability"], logging_location = "log_data", test_host_pkg = 'nav_scripts')
