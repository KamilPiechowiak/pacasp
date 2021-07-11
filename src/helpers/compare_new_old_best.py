import json
import os
from tqdm import tqdm
import numpy as np

def get_best_height(filename):
    height = None
    with open(filename) as file:
        for line in file.readlines():
            height = int(line.split(" ")[1])
    return height

def find_differences():
    with open("config/second_best.json") as second_best_file:
        second_best = json.load(second_best_file)
    best_algorithms = {}
    entries = list(os.scandir("out_validated"))
    entries.sort(key=lambda x: x.name)
    diffs = []
    worse_than_second_best = 0
    for entry in tqdm(entries):
        if "bounds" in entry.name.lower():
            continue
        instance_id = int(entry.name.split("_")[0])
        # if instance_id < 2000:
        #     continue
        new_best_height = get_best_height(entry.path)
        diffs+= [new_best_height-get_best_height(f"out/{entry.name}")]
        if new_best_height > second_best[str(instance_id)][0]:
            worse_than_second_best+=1
    return np.array(diffs), worse_than_second_best

if  __name__ == "__main__":
    res, worse_than_second_best = find_differences()
    u = np.unique(res, return_counts=True)
    s = list(zip(u[0], u[1]))
    s.sort(key=lambda x: -x[1])
    print(s)
    print(worse_than_second_best)