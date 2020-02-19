#!/usr/bin/env python
import roslaunch
import rospy
import rospkg
import rosbag
import math
import os
from shutil import move
import datetime
import dynamic_reconfigure.client

# TODO: dynamic_reconfigure times out indefinitely, should fix later

# Feasibility Check
# Comparison: egocircle vs non-egocircle
# param: egocircle vs noegocircle launchfile
# Varying parameter: no. of poses to be checked
# param: feasibility_check_no_poses
# Output for comparison:
# 1. Computation time

# HCP
# simple_exploration
# gap_exploration
# Enable simple_exploration causes to use lrKeyPointGraph
# Enable gap_exploration (missing from cfg file for the moment) enables
# Both be false leads to PRM
# max_number_classes looks into how they scale with # classes

# Comparison: lrkey vs PRM vs Gap
# TODO: run lrkey and PRM w/out converter and abandon them
# NOTE: of homotopies prob. have no effect. 
# NOTE: infeasible path prob. also have minimal effect since PRM throws out all once infeasible
# param: simple_exploration, gap_exploration
# Varying Parameter: How computation time scales with number of homotopies
# paran max_number_classes
# Output for comparison:
# 1. Computation time (Graph generation and optimization), # of tebs
# 2. # of infeasiblt path (from feasibility check) - NOT NEEDED MAYBE

# Homotopy Classes Signature
# Comparison: Gap-based HSignature generation vs OG obstacle-path HSignature Generation
# param: egocircle launch file vs non-egocircle launch file
# also run OG under the same environment (because # of obstacles could be different)
# TODO: ground truth(?) - NEED HELP

# Costmap
# TODO: tweak egocircle code to reconfigure egocircle resolution - NEED HELP
# Comparison: Egocircle Update time vs Costmap Update time
# param: egocircle launch file vs non-egocircle launch file


# Input:
# 1.
# Bag file location
# Launch file for bag file
# bag file directory
# 2. 
# test types
# navigation_test pkg
# launch file with corresponding test type
# 3.
# Target data location

def get_rosbag_name_fromlaunch(launch_file):
    with open(launch_file) as f:
        lines = f.readlines()
    
    for line in lines:
        if line.find("rosbag") != -1:
            rosbag_name = (line.split("args=\"")[-1])[:-4]
            return rosbag_name
    
    print("failed to parse target rosbag name!")
    return None

def get_rosbag_duration(bag_file_dir):
    if bag_file_dir is None:
        return 5 # arbitrary wait
    try:
        bag = rosbag.Bag(bag_file_dir)
    except:
        print("No such bag file {}".format(bag_file_dir))
        return 5
    duration = bag.get_end_time() - bag.get_start_time()
    bag.close()
    return duration

def run_test(rosbag_launch_dir = 'launch/rosbag', tests = ["scalability"], second_tests = ['egocircle'], logging_location = "log_data", test_host_pkg = 'nav_scripts'):

    # Getting log export location
    pkg_dir = rospkg.RosPack().get_path(test_host_pkg)
    data_location = os.path.join(os.path.dirname(pkg_dir), logging_location)
    if not os.path.exists(data_location):
        os.makedirs(data_location)

    egocircle_launchfile = os.path.join(pkg_dir, 'launch', second_tests[0])
    
    for testtype in tests:
        for ego_launch_file in os.listdir(egocircle_launchfile):
            ego_path = os.path.join(egocircle_launchfile, ego_launch_file)
            print("--------Starting {} test--------".format(testtype))

            controller_launchfile_dir = os.path.join(pkg_dir, 'launch', testtype)
            # rosbag_file_dir = os.path.join(pkg_dir, 'launch', "rosbag")
            rosbag_file_dir = os.path.join(pkg_dir, rosbag_launch_dir)
            list_of_log_directorys = list()

            for rosbag_file_name in os.listdir(rosbag_file_dir):
                for launch_file_name in os.listdir(controller_launchfile_dir):
                    # Launch the planner
                    launch_full_path = os.path.join(controller_launchfile_dir, launch_file_name)
                    # play the bag file
                    bag_launch_path = os.path.join(rosbag_file_dir, rosbag_file_name)
                    rosbag_name = get_rosbag_name_fromlaunch(bag_launch_path)
                    rosbag_time = get_rosbag_duration(rosbag_name)
                    
                    # Get ROS logging location
                    uuid = roslaunch.rlutil.get_or_generate_uuid(None, False)
                    list_of_log_directorys.append(uuid)
                    roslaunch.configure_logging(uuid)
                    launch = roslaunch.parent.ROSLaunchParent(uuid, [launch_full_path, ego_path, bag_launch_path])
                    rospy.loginfo(launch_full_path)
                    rospy.loginfo(bag_launch_path)
                    rospy.loginfo(ego_path)

                    # Run the experiment
                    launch.start()
                    rospy.loginfo("started")
                    rospy.sleep(math.ceil(rosbag_time) + 10)
                    # print([launch_full_path, ego_path, bag_launch_path])
                    rospy.sleep(1)

                    launch.shutdown()
                    rospy.sleep(1)

                    # Finish up
                    print("All {} run for {} ended, moving files".format(testtype, launch_file_name))
                    timestamp = str(datetime.datetime.now()).replace(' ', '-').replace(':', '-').replace('.', '-').replace('/', '-')
                    print(timestamp)

                    outputpath = os.path.join(data_location, rosbag_file_name, testtype, rosbag_file_name.split(".")[0] + "," + ego_launch_file.split(".")[0] + "," + launch_file_name.split(".")[0])
                    # print(outputpath)
                    if not os.path.exists(outputpath):
                        os.makedirs(outputpath)
                    for x in list_of_log_directorys:
                        file_name = str(x)
                        move(os.path.join("/home/alex/.ros/log", x), outputpath)
                    list_of_log_directorys = list()
