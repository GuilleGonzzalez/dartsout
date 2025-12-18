import subprocess
import sys

try:
    n = int(sys.argv[1])
except (IndexError, ValueError):
    n = 1

processes = []

for i in range(n):
    print(f"Launching simulator #{i+1} ...")
    p = subprocess.Popen(["python", "sim/sim.py"])
    processes.append(p)

# Wait to all finish
for p in processes:
    p.wait()