import os, sys
import glob
import json
import re
import subprocess as sp
from tqdm import tqdm
import numpy as np

PATH = "used_instances/tuning_new_format/"

if __name__ == "__main__":
    n = 80
    size = 2
    res = np.zeros(size)
    for i in tqdm(range(n)):
        with open(f"{PATH}/{i}.in") as instance:
            p = sp.Popen(["./sp"], stdin=instance, stdout=sp.PIPE)
            out, _ = p.communicate()
            out = out.decode("utf-8").split("\n")
            # print(out)
            lower_bound = int(out[0])
            instance_res = np.array([int(x) for x in out[1:1+size]])
            res+=instance_res/lower_bound
    print(res/n)