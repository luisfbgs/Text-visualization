import sys
import struct
from scipy import spatial
import matplotlib.pyplot as plt
import matplotlib as mpl
from sklearn.cluster import Birch
from sklearn.cluster import KMeans
from sklearn.metrics import silhouette_score
import numpy as np
import random

acolors = [(230, 25, 75), (60, 180, 75), (255, 225, 25), (0, 130, 200), (245, 130, 48), (70, 240, 240), (240, 50, 230), (250, 190, 212), (0, 128, 128), (220, 190, 255), (170, 110, 40), (255, 250, 200), (128, 0, 0), (170, 255, 195), (0, 0, 128), (128, 128, 128), (0, 0, 0)]

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
	print(nh, n)
	print(dh)
	hdata = np.zeros((n, dh), dtype=np.float64)
	for i in range(n):
		for j in range(dh):
			hdata[i][j] = struct.unpack('d', file.read(8))[0]
		hdata[i] /= np.linalg.norm(hdata[i])


pos = np.zeros(n, dtype=int)
colors = []
#for i in range(n):
#	colors.append(rgb_heatmap(0, 1, i/n));
for i in range(len(acolors)):
	aux = acolors[i]
	pa = (aux[0] / 255, aux[1] / 255, aux[2] / 255)
	colors.append(pa);
cmap = mpl.colors.ListedColormap(colors)
norm = mpl.colors.Normalize(vmin=0, vmax=n)
distortions = []
#for k in [12, 17]:
	#clustering = KMeans(n_clusters=k, init='random', max_iter=1000).fit(hdata)
	#clustering = Birch().fit(hdata)
	#distortions.append(clustering.inertia_)
#	print(clustering.labels_)
colors = []
with open(sys.argv[4], 'rb') as file:
	for i in range(n):
		aux = struct.unpack('<d', file.read(8))[0]
		colors.append(rgb_heatmap(0, 1, aux))
		#aux = acolors[clustering.labels_[i]]
		#pa = (aux[0] / 255, aux[1] / 255, aux[2] / 255)
		#colors.append(pa)
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
'''
plt.plot(range(1, 17), distortions, marker='o')
plt.xlabel('Number of clusters')
plt.ylabel('Distortion')
plt.show()
'''
