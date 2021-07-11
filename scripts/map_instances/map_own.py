import os, sys
import glob
import json
import re
import subprocess as sp
from tqdm import tqdm

PATH = "used_instances/training_and_testing_instances"
# PATH = "datasets/tag-clouds"

if __name__ == "__main__":

    def parse_line(line):
        return map(int, re.sub(" +", " ", line).strip().split(" "))

    mapping = {}
    i = int(sys.argv[1])
    for a in tqdm(range(0, 2000)):
        filename = f"{a}.in"
        with open(f"{PATH}/{filename}") as instance:
            p = sp.Popen(["./sp"], stdin=instance, stdout=sp.PIPE)
            out, _ = p.communicate()
            lower_bound = int(out.decode('utf-8'))
            
        with open(f"{PATH}/{filename}") as instance:
            with open(f"instances2/{i}.in", "w") as result:
                mapping[i] = filename
                i+=1
                n, strip_w = parse_line(instance.readline())
                result.write(f"{strip_w} {lower_bound}\n{n}\n")
                for _ in range(n):
                    w, h = parse_line(instance.readline())
                    result.write(f"{w} {h}\n")
                print(n, strip_w)

    print(json.dumps(mapping, indent=2))