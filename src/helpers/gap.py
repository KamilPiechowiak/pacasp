import subprocess as sp
import numpy as np
from tqdm import tqdm

for i in tqdm(range(4, 16)):
    arr = np.zeros(6)
    min_arr = np.ones_like(arr)*1e18
    maxk = 0
    for j in range(1, 11):
        with open(f"datasets/strip2/i{i}-{j}") as file:
            p = sp.Popen("./sp", stdin=file, stdout=sp.PIPE)
            out, _ = p.communicate()
            for k, line in enumerate(out.decode("utf-8").split("\n")):
                if line == "":
                    break
                arr[k]+=int(line)
                min_arr[k] = min(min_arr[k], int(line))
                maxk = max(k, maxk)
    print(i, arr/10)
    print((arr-arr[0])/arr[0])
    print((10*min_arr-arr[0])/arr[0], end="\n\n")
