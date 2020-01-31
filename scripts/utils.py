#!/usr/bin/env python2

import os
import re
import rospkg

def get_logger_names(package_name = "std_msgs"):
    print("Package name:\n" + package_name)
    potential_logger = set(['DURATION', 'DELAY', 'RATE'])

    rospack = rospkg.RosPack()
    pkg_path = rospack.get_path(package_name)
    print("---------\nAcquired package path:\n" + pkg_path)
    search_dir = ["include/" + package_name, 'src']
    logger_names = set()
    search_files = []
    for local_dir in search_dir:
        curr_dir = os.path.join(pkg_path, local_dir)
        search_files.extend([os.path.join(curr_dir, f) for f in os.listdir(curr_dir) if os.path.isfile(os.path.join(curr_dir, f))])

    for search_file in search_files:
        with open(search_file, 'r') as src_file:
            src_lines = src_file.readlines()
        for line in src_lines:
            # remove comments and white spaces
            line, _, _ = line.partition("//")
            line = line.strip()
            val = line.split("_")
            if val[0] in potential_logger:
                val = re.split("\"", line)
                logger_names.add(val[1])
                
    logger_names = list(logger_names)            
    logger_names.sort()
    print("---------\nList of loggers: ")
    print("\n".join(logger_names))

    return logger_names

if __name__ == "__main__": 
    get_logger_names("benchmarking_tools")
