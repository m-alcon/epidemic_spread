#!/usr/bin/python3

import argparse
import matplotlib
import matplotlib.pyplot as plt

parser = argparse.ArgumentParser()
parser.add_argument('file', help='File to parse')
args = parser.parse_args()

time, infected, recovered = [], [], []
with open(args.file, 'r') as file:
    for l in file.readlines():
        row = l.split()
        time.append(int(row[0]))
        infected.append(int(row[1]))
        if (len(row) == 3):
            recovered.append(int(row[2]))

print(len(time), len(infected))
print(infected)
fig, ax = plt.subplots()
ax.plot(time, infected)

ax.set(xlabel='Time', ylabel='Infected nodes')
#       title='About as simple as it gets, folks')
#ax.grid()

# fig.savefig("test.png")
plt.show()
