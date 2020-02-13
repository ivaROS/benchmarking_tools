#!/usr/bin/env python
import execution

execution.run_test(rosbag_launch_dir = 'launch/rosbag', tests = ["scalability", "costmapver"], logging_location = "log_data_wed", test_host_pkg = 'nav_scripts')
