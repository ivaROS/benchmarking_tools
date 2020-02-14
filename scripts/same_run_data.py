import os
import pickle
import pandas as pd

def get_data(log_type, log_file, size, resolution, rbt_size, seed, costmap_radius, costmap_resoln):
    ret_dict = dict()
    # print("get size {}, resol {}, rbt {}".format(size, resolution, rbt_size))
    if log_type == "egocircle_node":
        insert_val = 0
        insert_count = 0
        update_val = 0
        update_count = 0

        holder = pd.DataFrame.from_dict(dict(), orient='index',
                       columns=['insertPoints', "update", "size", "resolution", "seed"])

        val = dict(
            {
                "size" : size,
                "resolution" : resolution,
                'insertPoints' : 0,
                'update' : 0,
                "seed" : seed
                # "costmap_radius" : costmap_radius, 
                # "costmap_resoln" : costmap_resoln
            }
        )

        ind = 0

        # print(holder.append(val))

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


                # print(val)
                val["update"] = update_val / update_count
                val["insertPoints"] = insert_val / insert_count
                temp_pd = pd.DataFrame(data = val, index = [ind])
                ind = ind + 1

                # print(temp_pd)

                holder = holder.append(temp_pd)

                insert_val = 1
                insert_count = 1
                update_val = 1
                update_count = 1

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
        costmapUpdate = 1
        costmapUpdate_count = 1
        costmapInsert = 1
        costmapInsert_count = 1
        

        holder = pd.DataFrame.from_dict(dict(), orient='index',
                       columns=['num_gaps', 'obsCount', 'calculateEquivalenceClass', 'graphGen',
                       'optimizeGraph', 'feasibility', "size", "resolution",
                       "rbt_radius", "seed", "costmap_radius", "costmap_resoln",
                       'costmapInsert',
                       'costmapUpdate'])

        ind = 0
        with open(log_file, 'r') as file:
            content = file.readlines()

        val_dict = dict(
            {
                'num_gaps' : 0,
                'obsCount' : 0,
                'calculateEquivalenceClass'  : 0,
                'graphGen' : 0,
                'optimizeGraph' : 0,
                'feasibility' : 0,
                'costmapInsert' : 0,
                'costmapUpdate' : 0,
                'newTeb' : 0,
                "size" : size,
                "resolution" : resolution,
                "rbt_radius" : rbt_size,
                "seed" : seed,
                "costmap_radius" : costmap_radius, 
                "costmap_resoln" : costmap_resoln
            }
        )

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


            if parsed[0] == "costmapInsert":
                costmapInsert_count += 1
                costmapInsert += float(parsed[1].split(",")[0].split(" ")[-1])
            
            if parsed[0] == "costmapUpdate":
                costmapUpdate_count += 1
                costmapUpdate += float(parsed[1].split(",")[0].split(" ")[-1])

            if parsed[0] == "num_gaps":
                num_gaps_count += 1
                val = parsed[1][:-5]
                num_gaps += float(val)

                # val = dict(
                #     {
                #         'numgaps' : 0,
                #         'obsCount' : 0,
                #         'calculateEquivalenceClass'  : 0,
                #         'graphGen' : 0,
                #         'optimizeGraph' : 0,
                #         'feasibility' : 0
                #     }
                # )

            if parsed[0] == "newTeb":
                numteb_count += 1
                val = parsed[1][:-5]
                numteb += float(val)

            if parsed[0] == "obsCount":
                obsCount_count += 1
                val = parsed[1][:-5]
                obsCount += float(val)

                val_dict["num_gaps"] = num_gaps
                val_dict['calculateEquivalenceClass'] = calculateEquivalenceClass / calculateEquivalenceClass_count
                val_dict['graphGen'] = graphGen / graphGen_count
                val_dict['optimizeGraph'] = optimizeGraph / optimizeGraph_count
                val_dict['newTeb'] = numteb / numteb_count
                val_dict['obsCount'] = obsCount / obsCount_count
                val_dict['feasibility'] = feasibility / feasibility_count
                val_dict['costmapUpdate'] = costmapUpdate / costmapUpdate_count
                val_dict['costmapInsert'] = costmapInsert / costmapInsert_count

                if ind > 10:
                    temp_pd = pd.DataFrame(data = val_dict, index = [ind])
                    holder = holder.append(temp_pd)
                ind = ind + 1

                # print(temp_pd)

                
                num_gaps = 1e-3
                obsCount = 1e-3
                calculateEquivalenceClass = 1e-3
                graphGen = 1e-3
                optimizeGraph = 1e-3
                num_gaps_count = 1e-3
                obsCount_count = 1e-3
                calculateEquivalenceClass_count = 1e-3
                graphGen_count = 1e-3
                optimizeGraph_count = 1e-3
                feasibility = 1e-3
                feasibility_count = 1e-3
                numteb = 1e-3
                numteb_count = 1e-3

            if parsed[0] == "egoFeasibility" or parsed[0] == "costmapFeasibility":
                feasibility_count += 1
                feasibility += float(parsed[1].split(",")[0].split(" ")[-1])

    # pickle.dump(holder, open("move_base.p", 'wb'))
    return holder

# Egocircles

# log_file = "/home/alex/catkin_ws/src/navigation_test/log_data_wed/seed_0.launch/scalability/seed_0,ego_3_0512,0_18/f406b8fc-4df7-11ea-8e56-f8b156bbe800/move_base-393-stdout.log"
# egocircle_pd = get_data("ego_cirle", log_file, size = 3, resolution = 512)

# get_data("move_base", log_file)

def get_location(tgt_pkg = "nav_scripts", data_location = "log_data", optional_div = None, bag_file_names = ["seed_0.launch"]):
    # pkg_dir = rospkg.RosPack().get_path(tgt_pkg)
    pkg_dir = "/home/alex/catkin_ws/src/navigation_test"
    ret_list = list()
    for bag_file in bag_file_names:
        this_dir = os.path.join(pkg_dir, data_location, bag_file)
        if optional_div is None:
            for dir_name in os.listdir(this_dir):
                ret_list.append(os.path.join(this_dir, dir_name))
        else:
            for optional_key in optional_div:
                total_dir = os.path.join(this_dir, optional_key)
                for dir_name in os.listdir(total_dir):
                    ret_list.append(os.path.join(total_dir, dir_name))
    return ret_list

all_log_location = get_location(data_location = "log_data_fri_ego", optional_div = ['egoteb'], bag_file_names = ["seed_1.launch"])

ego_pd = None
move_base_pd = None

# all_log_location = ["/home/alex/catkin_ws/src/navigation_test/log_data_wed/seed_0.launch/scalability/seed_0,ego_5_1024,0_27"]
# print(all_log_location)
for folder in all_log_location:
    # print(folder)
    arg = folder.split('/')[-1]

    experiment_type = 'ego'
    if (experiment_type == 'costmap'):
        seed = float(arg.split(',')[0].split("_")[1])

        ego_config = arg.split(',')[1]
        ego_radius = float(ego_config.split('_')[1])
        ego_resolu = float(ego_config.split('_')[-1])

        rbt_radius = arg.split(',')[2]
        rbt_radius = float(rbt_radius.replace('_', '.'))

        costmap_radius = float(arg.split(',')[-1])
        costmap_resoln = float(arg.split(',')[-2].replace("_", '.'))

        print("Seed {}, egoradius {}, eagoreoln {}, rbtradius {}, costmap r {}, costmap resoln {}".format(seed, ego_radius, ego_resolu, rbt_radius, costmap_radius, costmap_resoln))
    else:
        seed = float(arg.split(',')[0].split("_")[1])
        ego_config = arg.split(',')[1]
        ego_radius = float(ego_config.split('_')[1])
        ego_resolu = float(ego_config.split('_')[-1])
        rbt_radius = arg.split(',')[2]
        rbt_radius = float(rbt_radius.replace('_radius', '').replace("_",'.'))
        print("Seed {}, egoradius {}, eagoreoln {}, rbtradius {}".format(seed, ego_radius, ego_resolu, rbt_radius))
        costmap_radius = 0
        costmap_resoln = 0
    # continue
    # print(ego_pd)
    # print(folder)
    # this_folder_dir = os.path.join(this_dir, folder)
    for logfile in os.listdir(folder):
        logfile_dir = os.path.join(folder, logfile)
        for actuallog in os.listdir(logfile_dir):
            log_file_full_dir = os.path.join(logfile_dir, actuallog)
            if actuallog.split("-")[0] == "egocircle_node":
                # print("egorad {}, egoresolu {}, rbt_rad {}".format(ego_radius, ego_resolu, rbt_radius))
                # print(log_file_full_dir)
                new_pd = get_data("egocircle_node", log_file_full_dir, ego_radius, ego_resolu, rbt_radius, seed, costmap_radius, costmap_resoln)
                if ego_pd is None:
                    ego_pd = new_pd
                else:
                    # ego_pd = pd.concat([ego_pd, new_pd])
                    ego_pd = ego_pd.append(new_pd, ignore_index = True)

            if actuallog.split("-")[0] == "move_base":
                new_pd = get_data("move_base", log_file_full_dir, ego_radius, ego_resolu, rbt_radius, seed, costmap_radius, costmap_resoln)
                if move_base_pd is None:
                    move_base_pd = new_pd
                else:
                    # move_base_pd = pd.concat([move_base_pd, new_pd])
                    move_base_pd = move_base_pd.append(new_pd, ignore_index = True)

pickle.dump(move_base_pd, open("ego.p", 'wb'))
# pickle.dump(ego_pd, open("ego.p", 'wb'))

# print(move_base_pd.size)
