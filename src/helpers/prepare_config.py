import json

with open("config/best_algorithms.json") as f:
    best_algorithms = json.load(f)
config = {}
instances = list(best_algorithms.keys())
instances.sort(key=lambda x: int(x))

res = {}

for i, instance_id in enumerate(instances):
    res[i] = {
        "instance_id": instance_id,
        "algorithms": [best_algorithms[instance_id][1]]
    }

print(json.dumps(res))