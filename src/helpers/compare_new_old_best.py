import json
import os
from tqdm import tqdm
import numpy as np

def get_best_height_and_first_time(filename):
    height = None
    first_time = None
    with open(filename) as file:
        for line in file.readlines():
            if first_time is None:
                first_time = int(line.split(" ")[0])
            height = int(line.split(" ")[1])
    return height, first_time

def find_differences():
    with open("config/second_best.json") as second_best_file:
        second_best = json.load(second_best_file)
    best_algorithms = {}
    entries = list(os.scandir("placements_validated"))
    entries.sort(key=lambda x: int(x.name.split("_")[0]))
    diffs = []
    relative_diffs = []
    relative_time_diffs = []
    worse_than_second_best = 0
    for entry in tqdm(entries):
        if "bounds" in entry.name.lower():
            continue
        instance_id = int(entry.name.split("_")[0])
        # if instance_id < 2000:
        #     continue
        new_best_height, new_first_time = get_best_height_and_first_time(f"out_validated/{entry.name}")
        old_best_height, old_first_time = get_best_height_and_first_time(f"out/{entry.name}")
        diffs+= [new_best_height-old_best_height]
        relative_diffs+= [(new_best_height-old_best_height)/old_best_height]
        relative_time_diffs+= [(new_first_time/old_first_time, entry.name)]
        if new_best_height > second_best[str(instance_id)][0]:
            worse_than_second_best+=1
    return np.array(diffs), worse_than_second_best, np.array(relative_diffs), relative_time_diffs

def summarize_arr(arr):
    print("quantiles:")
    for q in [0.001, 0.01, 0.1, 0.5, 0.9, 0.99, 0.999]:
        print(q, np.quantile(arr, q))
    print(f"min: {np.min(arr)} max: {np.max(arr)}")

if  __name__ == "__main__":
    res, worse_than_second_best, relative, relative_times = find_differences()
    u = np.unique(res, return_counts=True)
    s = list(zip(u[0], u[1]))
    s.sort(key=lambda x: -x[1])
    print(s)
    print(np.sum(res > 0), np.sum(res == 0), np.sum(res < 0))
    print(worse_than_second_best)
    summarize_arr(relative)
    print(np.where(np.abs(relative) > 0.01))
    # summarize_arr(relative_times)
    relative_times.sort(key=lambda x: x[0])
    filtered_times = relative_times
    print(filtered_times[-20:])
    print(filtered_times[:20])
    summarize_arr(np.array(list(map(lambda x: x[0], filtered_times))))
    