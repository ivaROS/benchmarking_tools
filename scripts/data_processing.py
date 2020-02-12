#!/usr/bin/env python
import rospy
import rospkg
import rosbag
import math
import os
from shutil import move
import re
import pickle

def get_location(tgt_pkg = "nav_scripts", data_location = "log_data", bag_file_names = ["seed_0.launch"]):
    pkg_dir = rospkg.RosPack().get_path(tgt_pkg)
    ret_list = list()
    for bag_file in bag_file_names:
        this_dir = os.path.join(os.path.dirname(pkg_dir), data_location, bag_file)
        for dir_name in os.listdir(this_dir):
            ret_list.append(os.path.join(this_dir, dir_name))
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
        num_gaps = 0
        obsCount = 0
        calculateEquivalenceClass = 0
        graphGen = 0
        optimizeGraph = 0
        num_gaps_count = 0
        obsCount_count = 0
        calculateEquivalenceClass_count = 0
        graphGen_count = 0
        optimizeGraph_count = 0


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

            if parsed[0] == "obsCount":
                obsCount_count += 1
                val = parsed[1][:-5]
                obsCount += float(val)

        ret_dict.update(
            {
                "calculateEquivalenceClass" : calculateEquivalenceClass / calculateEquivalenceClass_count,
                "graphGen" :  graphGen / graphGen_count,
                "optimizeGraph" : optimizeGraph / optimizeGraph_count,
                "num_gaps" : num_gaps / num_gaps_count,
                "obsCount" : obsCount / obsCount_count
            }
        )

    return ret_dict


if __name__ == "__main__":
    dump_file = "data.p"
    terminal_dict = dict()
    all_log_location = get_location()
    for idx, log in enumerate(all_log_location):
        this_dict = get_file(log_path = log)
        val = log.split("/")[-1].split(",")
        for attr in val:
            attr_iden = attr.split("_")[0]
            if attr_iden == "ego":
                radius = float(attr.split("_")[1])
                resolu = float(attr.split("_")[2])
                this_dict.update(
                    {
                        "egoradius" : radius,
                        "egoresolu" : resolu
                    }
                )

            if attr_iden == "seed":
                seed = float(attr.split("_")[1])
                this_dict.update(
                    {
                        "seed" : seed
                    }
                )


            if attr_iden == "teb":
                radius = attr.split("_")[1]
                this_dict.update(
                    {
                        "modelradius" : radius,
                    }
                )
                
        terminal_dict.update({str(idx) : this_dict})
    pickle.dump(terminal_dict, open(dump_file, 'wb'))
