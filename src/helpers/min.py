import os

path = "out_local"
logs = os.listdir(path)

arr = []
for log in logs:
    # if log.startswith("2054_") == False:
    #     continue
    h = 0
    with open(f"{path}/{log}") as file:
        for line in file.readlines():
            h = int(line.split(" ")[1])
    arr.append((log, h))
arr.sort(key=lambda x: x[1])
for x in arr:
    print(x)