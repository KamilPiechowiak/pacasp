import re

for i in range(3306):
    print(i)
    with open(f"instances/{i}.in") as f:
        lb = int(re.findall(r"[\d]+", f.readline())[1])
    with open(f"out/{i}_BoundsbyWiderThanHalf.log") as f:
        lb1 = int(f.readline().split(" ")[1])
    with open(f"out/{i}_BoundsByArea.log") as f:
        lb2 = int(f.readline().split(" ")[1])
    if max(lb1, lb2) != lb:
        print(i, lb1, lb2, lb)
        # break