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
v = [28911, 17433, 6623, 12561, 2188, 18979, 28025, 24626, 19355, 26429, 10540, 463, 8864, 18813, 24266, 5434, 20839, 23186, 6501, 2299]
colors = []
for i in range(n):
	colors.append(rgb_heatmap(0, n, i));
cmap = mpl.colors.ListedColormap(colors)
norm = mpl.colors.Normalize(vmin=0, vmax=n)
for idx in v:
	dists = []
	for i in range(n):
		dists.append((spatial.distance.cosine(hdata[idx], hdata[i]), i))
	dists.sort()
	for i in range(n):
		pos[dists[i][1]] = i
	dists = []
	for i in range(n):
		dists.append((np.linalg.norm(ldata[idx]-ldata[i]), i))
	dists.sort()
	ids = []
	for i in range(200):
		ids.append(dists[i][1])
	x = []
	y = []
	c = []
	for i in ids:
		x.append(ldata[i][0])
		y.append(ldata[i][1])
		c.append(colors[pos[i]])
	plt.scatter(x[1:], y[1:], c=c[1:])
	plt.colorbar(mpl.cm.ScalarMappable(norm=norm, cmap=cmap))
	plt.plot(x[0], y[0], 's', c=[0, 0, 0])
	plt.savefig(sys.argv[3] + '/' + str(idx) + '.png')
	plt.clf()
