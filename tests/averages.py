import glob
import os
import statistics as stat

for filename in glob.glob("raw" + os.path.sep + "*_*_*_*"):
    with open(filename, 'r') as file:
        lines = [lines.rstrip() for lines in file.readlines()]
        closed_itemsets = int(lines[2].split(" ")[-1])
        p1 = int(lines[3].split(" ")[-1])
        p2 = int(lines[4].split(" ")[-1])
        p3 = int(lines[5].split(" ")[-1])
        p4 = int(lines[6].split(" ")[-1])
        itemset_time_idx = [0 + i * 7 for i in range(10)]
        algorithm_time_idx = [1 + i * 7 for i in range(10)]
        itemset_mean = stat.mean([int(lines[i].split(" ")[-1]) for i in itemset_time_idx])
        itemset_std = stat.stdev([int(lines[i].split(" ")[-1]) for i in itemset_time_idx])
        algo_mean = stat.mean([int(lines[i].split(" ")[-1]) for i in algorithm_time_idx])
        algo_std = stat.stdev([int(lines[i].split(" ")[-1]) for i in algorithm_time_idx])
        savelines = [f"Number of closed itemsets: {closed_itemsets}", 
        f"Property 1: {p1}", f"Property 2: {p2}", f"Property 3: {p3}", f"Property 4: {p4}",
        f"Mean frequent items mining time: {itemset_mean}",
        f"Std frequent items mining time: {itemset_std}",
        f"Mean algorithm time: {algo_mean}",
        f"Std algorithm time: {algo_std}",]
        with open("averages" + os.path.sep + filename + "_avg", 'w') as savefile:
            savefile.writelines('\n'.join(savelines))

