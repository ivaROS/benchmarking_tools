import os
import re

def get_logger_names(package_dir = "/home/alex/catkin_essential_pkg/src/teb_local_planner", local_search_dirs = ["include/teb_local_planner", "src"]):
    # Change to package + workspace environment path
    search_files = []
    logger_names = set()
    for local_dir in local_search_dirs:
        curr_dir = os.path.join(package_dir, local_dir)
        search_files.extend([os.path.join(curr_dir, f) for f in os.listdir(curr_dir) if os.path.isfile(os.path.join(curr_dir, f))])

    for search_file in search_files:
        with open(search_file, 'r') as src_file:
            src_lines = src_file.readlines()
        for line in src_lines:
            # remove comments and white spaces
            line, _, _ = line.partition("//")
            line = line.strip()
            val = re.split("[|(|)|]|,", line)
            if val[0] == "TIMING":
                logger_names.add(val[1][1:-1])
                
    logger_names = list(logger_names)            
    logger_names.sort()
    print("\n".join(logger_names))

    return logger_names

if __name__ == "__main__": 
    get_logger_names()