import os

d = {}
for entry in os.scandir("instances"):
    with open(entry.path) as instance:
        instance.readline()
        n = int(instance.readline())
        d[n] = d.get(n, 0)+1
keys = list(d.keys())
keys.sort()
for key in keys[-20:]:
    print(key, d[key])