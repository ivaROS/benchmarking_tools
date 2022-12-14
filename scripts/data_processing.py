#!/usr/bin/env python
import rospy
import rospkg
import rosbag
import math
import os
from shutil import move
import re
import pickle

def get_location(tgt_pkg = "nav_scripts", data_location = "log_data", optional_div = None, bag_file_names = ["seed_0.launch"]):
    pkg_dir = rospkg.RosPack().get_path(tgt_pkg)
    ret_list = list()
    for bag_file in bag_file_names:
        this_dir = os.path.join(os.path.dirname(pkg_dir), data_location, bag_file)
        if optional_div is None:
            for dir_name in os.listdir(this_dir):
                ret_list.append(os.path.join(this_dir, dir_name))
        else:
            for optional_key in optional_div:
                total_dir = os.path.join(this_dir, optional_key)
                for dir_name in os.listdir(total_dir):
                    ret_list.append(os.path.join(total_dir, dir_name))
    return ret_list

def get_file(log_path, target_keys = set(["egocircle_node", "move_base"])):
    val_dict = dict()
    for uid in os.listdir(log_path):
        full_path = os.path.join(log_path, uid)
        for log_file in os.listdir(full_path):
            tar_name = log_file.split("-")
            if tar_name[0] in target_keys:
                val = get_data(tar_name[0], os.path.join(full_path, log_file))
                val_dict.update(val)
    return val_dict

def get_data(log_type, log_file):
    ret_dict = dict()
    if log_type == "egocircle_node":
        insert_val = 0
        insert_count = 0
        update_val = 0
        update_count = 0

        with open(log_file, 'r') as file:
            content = file.readlines()
        for log_content in content:
            log_statement = log_content.split(": ")[1]
            parsed = log_statement.split(", ")
            if parsed[0] == "insertPoints":
                insert_count += 1
                insert_val += float(parsed[1].split(",")[0].split(" ")[-1])

            if parsed[0] == "update":
                update_count += 1
                update_val += float(parsed[1].split(",")[0].split(" ")[-1])

        ret_dict.update(
            {
                "insertPoints" : insert_val / insert_count,
                "update" :  update_val / update_count
            }
        )
    
    if log_type == "move_base":
        num_gaps = 1
        obsCount = 1
        calculateEquivalenceClass = 1
        graphGen = 1
        optimizeGraph = 1
        num_gaps_count = 1
        obsCount_count = 1
        calculateEquivalenceClass_count = 1
        graphGen_count = 1
        optimizeGraph_count = 1
        feasibility = 1
        feasibility_count = 1
        numteb = 1
        numteb_count = 1

        with open(log_file, 'r') as file:
            content = file.readlines()
        for log_content in content:
            log_statement = log_content.split(": ")[1]
            parsed = log_statement.split(", ")
            if parsed[0] == "calculateEquivalenceClass":
                calculateEquivalenceClass_count += 1
                calculateEquivalenceClass += float(parsed[1].split(",")[0].split(" ")[-1])

            if parsed[0] == "graphGen":
                graphGen_count += 1
                graphGen += float(parsed[1].split(",")[0].split(" ")[-1])

            if parsed[0] == "optimizeGraph":
                optimizeGraph_count += 1
                optimizeGraph += float(parsed[1].split(",")[0].split(" ")[-1])
            
            if parsed[0] == "num_gaps":
                num_gaps_count += 1
                val = parsed[1][:-5]
                num_gaps += float(val)

            if parsed[0] == "newTeb":
                numteb_count += 1
                val = parsed[1][:-5]
                numteb += float(val)

            if parsed[0] == "obsCount":
                obsCount_count += 1
                val = parsed[1][:-5]
                obsCount += float(val)

            if parsed[0] == "egoFeasibility" or parsed[0] == "costmapFeasibility":
                feasibility_count += 1
                feasibility += float(parsed[1].split(",")[0].split(" ")[-1])

        ret_dict.update(
            {
                "calculateEquivalenceClass" : calculateEquivalenceClass / calculateEquivalenceClass_count,
                "graphGen" :  graphGen / graphGen_count,
                "optimizeGraph" : optimizeGraph / optimizeGraph_count,
                "num_gaps" : num_gaps / num_gaps_count,
                "obsCount" : obsCount / obsCount_count,
                "feasibility" : feasibility / feasibility_count
            }
        )

    return ret_dict


if __name__ == "__main__":
    dump_file = "data.p"
    terminal_dict = dict()
    all_log_location = get_location(data_location = "log_data_fri", optional_div = ['costmap'], bag_file_names = ["seed_1.launch"])
    for idx, log in enumerate(all_log_location):
        this_dict = get_file(log_path = log)
        val = log.split("/")[-1].split(",")
        controller_type = log.split("/")[-2]

        print(val)

        seed_attr = val[0]
        seed = float(seed_attr.split("_")[1])
        this_dict.update(
            {
                "seed" : seed
            }
        )

        ego_attr = val[1]
        radius = float(ego_attr.split("_")[1])
        resolu = float(ego_attr.split("_")[2])
        this_dict.update(
            {
                "egoradius" : radius,
                "egoresolu" : resolu
            }
        )

        model_radius = val[2]
        this_dict.update(
            {
                "modelradius" : model_radius,
            }
        )

        costmap_resoln = float(val[3].split("_")[0]) + float(val[3].split("_")[1]) / pow(10, float(len(val[3].split("_")[1])))

        this_dict.update(
            {
                "costmap_resoln" : costmap_resoln
            }
        )

        costmap_size = float(val[4])

        this_dict.update(
            {
                "costmap_size" : costmap_size
            }
        )

        this_dict.update(
            {
                "controller_type" : controller_type
            }
        )

        terminal_dict.update({str(idx) : this_dict})
    # print(terminal_dict['6'])
    pickle.dump(terminal_dict, open("costmap.p", 'wb'))
