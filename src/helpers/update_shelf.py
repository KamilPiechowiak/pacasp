import os

def getline(file):
    try:
        return list(map(int, file.readline().split(" ")))
    except:
        return None

new_counter, old_counter = 0, 0
old_used = 0
old_has_best_value = 0
total_files = 0
for entry in os.scandir("out_shelf"):
    new_log = open(entry.path)
    if os.path.exists(f"out_validated/{entry.name}") == False:
        continue
    old_log = open(f"out_validated/{entry.name}")
    total_files+=1
    prev_old_counter = old_counter

    combined_log = []
    new_line = getline(new_log)
    old_line = getline(old_log)
    best_new = None
    best_old = None
    while new_line is not None or old_line is not None:
        if new_line is not None:
            best_new = new_line[1]
        if old_line is not None:
            best_old = old_line[1]
        if new_line is None:
            if len(combined_log) == 0 or combined_log[-1][1] > old_line[1]:
                combined_log.append(old_line)
                old_counter+=1
            old_line = getline(old_log)
        elif old_line is None or new_line[0] < old_line[0]:
            if len(combined_log) == 0 or combined_log[-1][1] > new_line[1]:
                combined_log.append(new_line)
                new_counter+=1
            new_line = getline(new_log)
        else:
            if len(combined_log) == 0 or combined_log[-1][1] > old_line[1]:
                combined_log.append(old_line)
                old_counter+=1
            old_line = getline(old_log)
    with open(f"out_updated_shelf/{entry.name}", "w") as res_file:
        res_file.write(
            "\n".join([f"{line[0]} {line[1]}" for line in combined_log])
        )
    if prev_old_counter != old_counter:
        old_used+=1
    if best_new >= best_old:
        old_has_best_value+=1

print(total_files)
print(old_used, old_has_best_value, old_counter, new_counter)
