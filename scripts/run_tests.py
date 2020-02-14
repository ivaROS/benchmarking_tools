#!/usr/bin/env python
import execution

execution.run_test(rosbag_launch_dir = 'launch/rosbag_copy', tests = ["egoteb"], second_tests = ['egocircle'], logging_location = "log_data_fri_ego", test_host_pkg = 'nav_scripts')
