import struct
import sys
import pandas as pd
import numpy as np
from numpy import dot
from numpy.linalg import norm
# Load the model in fairseq
from fairseq.models.roberta import RobertaModel
import torch

roberta = RobertaModel.from_pretrained('./roberta.large', checkpoint_file='model.pt')
roberta.eval()  # disable dropout (or leave in train mode to finetune)
df = pd.read_csv('preproc_20newsgroups.csv')
vecs = []
print("INIT", file=sys.stderr)
for i in range(len(df['text'])):
    if len(vecs) == 30000:
        break
    if i%100 == 0:
    	print(i, file=sys.stderr)
    tokens = roberta.encode(df['text'][i])[-512:]
    all_layers = roberta.extract_features(tokens)
    vecs.append(all_layers[0][0].detach().numpy())


fout = open('20newsgroups_largeRoBertA_' + str(len(vecs[0])) + '.bin', 'wb')
fout.write(struct.pack('<i', len(vecs)))
fout.write(struct.pack('<i', len(vecs[0])))
for i in range(len(vecs)):
    #print('df['text'][i].encode(sys.stdout.encoding', errors='replace'))
    for j in vecs[i]:
        fout.write(struct.pack('<d', j))
for i in range(len(vecs)):
    fout.write(struct.pack('<i', df['label'][i]))
fout.close()

ids = [5933, 7743, 16262, 1529, 29700, 5508, 29752, 17256, 7256, 8119, 9711, 8351, 12843, 8705, 24108, 26393, 18330, 27366, 22169, 27932]
for idx in ids:
    idx = idx % len(vecs)
    dists = []
    for i in range(len(vecs)):
        dists.append(( 1 - dot(vecs[idx], vecs[i]) / (norm(vecs[i]) * norm(vecs[idx])), i ))
    dists.sort()
    print('Compared Text:')
    print(df['text'][idx]);
    print()
    print("Closest 10:")
    for i in range(10):
        print(dists[i], df['text'][dists[i][1]])
