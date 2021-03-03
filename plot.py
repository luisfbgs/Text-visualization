import sys
import struct
from scipy import spatial
import matplotlib.pyplot as plt
import matplotlib as mpl
import numpy as np
import random

def rgb_heatmap(minimum, maximum, value):
    minimum, maximum = float(minimum), float(maximum)
    ratio = 2 * (value-minimum) / (maximum - minimum)
    r = int(max(0, 255*(1 - ratio)))
    b = int(max(0, 255*(ratio - 1)))
    g = 255 - b - r
    r /= 255
    g /= 255
    b /= 255
    return r, g, b

with open(sys.argv[1], 'rb') as file:
	n = struct.unpack('<i', file.read(4))[0]
	d = struct.unpack('<i', file.read(4))[0]
	ldata = np.zeros((n, d), dtype=np.float64)
	for i in range(n):
		for j in range(d):
			ldata[i][j] = struct.unpack('d', file.read(8))[0]

with open(sys.argv[2], 'rb') as file:
	nh = struct.unpack('<i', file.read(4))[0]
	dh = struct.unpack('<i', file.read(4))[0]
	hdata = np.zeros((n, dh), dtype=np.float64)
	for i in range(n):
		for j in range(dh):
			hdata[i][j] = struct.unpack('d', file.read(8))[0]


pos = np.zeros(n, dtype=int)
colors = []
for i in range(n):
	colors.append(rgb_heatmap(0, n, i));
cmap = mpl.colors.ListedColormap(colors)
norm = mpl.colors.Normalize(vmin=0, vmax=n)

colors = []
with open(sys.argv[4], 'rb') as file:
	for i in range(n):
		aux = struct.unpack('<d', file.read(8))[0]
		colors.append(rgb_heatmap(0, 1, aux))
x = []
y = []
for i in range(n):
	x.append(ldata[i][0])
	y.append(ldata[i][1])
plt.figure(figsize=(18.0, 15.0))
plt.scatter(x, y, c=colors, s=10, alpha=0.7)
plt.colorbar(mpl.cm.ScalarMappable(norm=norm, cmap=cmap))
plt.savefig(sys.argv[3], dpi=200)
plt.clf()
