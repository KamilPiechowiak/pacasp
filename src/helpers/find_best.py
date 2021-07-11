import json
import os
from tqdm import tqdm

def find_best_algorithms():
    best_algorithms = {}
    second_best = {}
    entries = list(os.scandir("out"))
    entries.sort(key=lambda x: x.name)
    for entry in tqdm(entries):
        if "bounds" in entry.name.lower():
            continue
        instance_id = int(entry.name.split("_")[0])
        algorithm_name = ".".join("_".join(entry.name.split("_")[1:]).split(".")[:-1])
        height = None
        with open(entry.path) as file:
            for line in file.readlines():
                height = int(line.split(" ")[1])
            
        if height is not None:
            if instance_id not in best_algorithms or best_algorithms[instance_id][0] > height:
                if instance_id in best_algorithms:
                    second_best[instance_id] = best_algorithms[instance_id]
                best_algorithms[instance_id] = (height, algorithm_name)
            elif instance_id not in second_best or second_best[instance_id][0] > height:
                second_best[instance_id] = (height, algorithm_name)
    return best_algorithms, second_best

if  __name__ == "__main__":
    best, second_best = find_best_algorithms()
    # print(json.dumps(best))
    print(json.dumps(second_best))