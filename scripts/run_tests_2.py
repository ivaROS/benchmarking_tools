#!/usr/bin/env python
import execution

execution.run_test(rosbag_launch_dir = 'launch/rosbag_copy', tests = ["costmap_2"], second_tests = ['egocircle_copy'], logging_location = "log_data_fri_2", test_host_pkg = 'nav_scripts')
