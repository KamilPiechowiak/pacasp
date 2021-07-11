import json

with open("config/run_shelf.json") as config_file:
    config = json.load(config_file)
    new_config = {
        "max_time": config["max_time"],
        "output_path": config["output_path"]
    }
    j = 0
    for i in range(3306):
        if 3194 <= i and i <= 3263:
            continue
        new_config[j] = config[str(i)]
        j+=1
    print(json.dumps(new_config))