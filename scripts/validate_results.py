import os
import glob
from tqdm import tqdm

PATH = "out"
N = 3306

def get_min_height(filename):
    with open(filename) as file:
        for line in file.readlines():
            res = int(line.split(" ")[1])
    if 'res' not in locals():
        print(filename)
        return 100000000000000000000
    return res

lower_bounds = {}
for i in range(N):
    lower_bounds[i] = min(get_min_height(f"{PATH}/{i}_BoundsByArea.log"), get_min_height(f"{PATH}/{i}_BoundsbyWiderThanHalf.log"))

for filename in tqdm(glob.glob(f"{PATH}/*")):
    i = int(filename.split("/")[-1].split("_")[0])
    if lower_bounds[i] > get_min_height(filename):
        print("Error", filename)
        exit(0)