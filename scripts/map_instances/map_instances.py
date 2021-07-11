import os, sys
import glob
import json

PATH = "datasets/Data"

if __name__ == "__main__":
    mapping = {}
    i = int(sys.argv[1])
    for name in glob.glob(f"{PATH}/*.zip"):
        if name.endswith("Data.zip"):
            continue
        tmp = f"{PATH}/tmp"
        os.system(f"unzip {name} -d {tmp} 2>&1 > /dev/null")
        filenames = os.listdir(tmp)
        filenames.sort()
        for filename in filenames:
            # print(filename)
            os.system(f"cp {tmp}/{filename} instances/{i}.in")
            mapping[i] = filename
            i+=1
        os.system(f"rm -r {tmp}")
    print(json.dumps(mapping, indent=2))