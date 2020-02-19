#!/usr/bin/env python
import execution

execution.run_test(rosbag_launch_dir = 'launch/rosbag_copy', tests = ["egoteb"], second_tests = ['egocircle'], logging_location = "log_feb_18_ego", test_host_pkg = 'nav_scripts')
execution.run_test(rosbag_launch_dir = 'launch/rosbag_copy', tests = ["costmap"], second_tests = ['egocircle_copy'], logging_location = "log_feb_18_cost", test_host_pkg = 'nav_scripts')
