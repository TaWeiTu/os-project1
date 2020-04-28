import re

with open("output/TIME_MEASUREMENT_dmesg.txt", "r") as f:
    avg = 0.0
    for line in f:
        data = list(map(float, re.sub(r"\[[A-za-z0-9\.\ ]*\]", "", line).split()))
        avg += (data[2] - data[1]) / 500
    avg /= 10

print("time unit = %f" % avg)

for policy in ["FIFO", "RR", "SJF", "PSJF"]:
    average = 0.0
    for fid in range(1, 6):
        expected = {}
        with open("test/%s_%d.txt" % (policy, fid), "r") as f:
            for idx, line in enumerate(f):
                if idx < 2: continue
                line = line.split(" ")
                expected[line[0]] = int(line[2])
        pids = {}
        with open("output/%s_%d_stdout.txt" % (policy, fid), "r") as f:
            for line in f:
                line = line.split(" ")
                pids[int(line[1])] = line[0]
        diff, cnt = 0.0, 0
        with open("output/%s_%d_dmesg.txt" % (policy, fid), "r") as f:
            for line in f:
                data = list(map(float, re.sub(r"\[[A-za-z0-9\.\ ]*\]", "", line).split()))
                actual = (data[2] - data[1]) / avg
                pid = int(data[0])
                diff += abs(actual - expected[pids[pid]]) / expected[pids[pid]]
                cnt += 1
        average += diff / cnt
        print("file: %s_%d.txt average diff: %f" % (policy, fid, diff / cnt))
    print("%s: %f" % (policy, average / 5))

