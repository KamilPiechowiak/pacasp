import os
from tqdm import tqdm
import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt

def find_max_sh_time():
    best_algorithms = {}
    entries = list(os.scandir("out"))
    entries.sort(key=lambda x: x.name)
    times = []
    for entry in tqdm(entries):
        if "SHsa" not in entry.name:
            continue
        instance_id = int(entry.name.split("_")[0])
        algorithm_name = ".".join("_".join(entry.name.split("_")[1:]).split(".")[:-1])
        instance_time = None
        with open(entry.path) as file:
            for line in file.readlines():
                instance_time = int(line.split(" ")[0])
                break
            
        if instance_time is not None:
            times.append(instance_time)
        else:
            print("is none")
    
    return np.array(times)

if  __name__ == "__main__":
    times = find_max_sh_time()
    sns.displot(np.log(times), kind="kde")
    for q in [0.9, 0.99, 0.995, 0.999]:
        print(q, np.quantile(times, q))
    plt.show()