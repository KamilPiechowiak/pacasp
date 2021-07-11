import os, sys
import glob
import json
import re

PATH = "datasets/strip2"

if __name__ == "__main__":

    def parse_line(line):
        return map(int, re.sub(" +", " ", line).strip().split(" "))

    mapping = {}
    i = int(sys.argv[1])
    for a in range(4, 21):
        for b in range(1, 11):
            filename = f"i{a}-{b}"
            with open(f"{PATH}/{filename}") as instance:
                with open(f"instances/{i}.in", "w") as result:
                    mapping[i] = filename
                    i+=1
                    n, strip_w = parse_line(instance.readline())
                    result.write(f"{strip_w} {strip_w}\n{n}\n")
                    for _ in range(n):
                        _, w, h = parse_line(instance.readline())
                        result.write(f"{w} {h}\n")
                    print(n, w)

    print(json.dumps(mapping, indent=2))

    # for name in glob.glob(f"{PATH}/*.zip"):
    #     if name.endswith("Data.zip"):
    #         continue
    #     tmp = f"{PATH}/tmp"
    #     os.system(f"unzip {name} -d {tmp} 2>&1 > /dev/null")
    #     filenames = os.listdir(tmp)
    #     filenames.sort()
    #     for filename in filenames:
    #         # print(filename)
    #         os.system(f"cp {tmp}/{filename} instances/{i}.in")
    #         mapping[i] = filename
    #         i+=1
    #     os.system(f"rm -r {tmp}")
    # print(json.dumps(mapping, indent=2))