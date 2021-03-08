import struct
from numpy import dot
from numpy.linalg import norm
import pandas as pd
import numpy as np
import torch
from transformers import BertTokenizer, BertModel

# OPTIONAL: if you want to have more information on what's happening, activate the logger as follows
import logging
#logging.basicConfig(level=logging.INFO)

import matplotlib.pyplot as plt

# Load pre-trained model tokenizer (vocabulary)
tokenizer = BertTokenizer.from_pretrained('bert-large-uncased')

# Load pre-trained model (weights)
model = BertModel.from_pretrained('bert-large-uncased',
                                  output_hidden_states = True, # Whether the model returns all hidden-states.
                                  )

# Put the model in "evaluation" mode, meaning feed-forward operation.
model.eval()

def bert(marked_text):
	# Split the sentence into tokens.
	tokenized_text = tokenizer.tokenize(marked_text)

	# Map the token strings to their vocabulary indeces.
	indexed_tokens = tokenizer.convert_tokens_to_ids(tokenized_text)

	segments_ids = [1] * len(tokenized_text)

	# Convert inputs to PyTorch tensors
	tokens_tensor = torch.tensor([indexed_tokens])
	segments_tensors = torch.tensor([segments_ids])


	# Run the text through BERT, and collect all of the hidden states produced
	# from all 12 layers. 
	with torch.no_grad():

	    outputs = model(tokens_tensor, segments_tensors)

	    # Evaluating the model will return a different number of objects larged on 
	    # how it's  configured in the `from_pretrained` call earlier. In this case, 
	    # becase we set `output_hidden_states = True`, the third item will be the 
	    # hidden states from all layers. See the documentation for more details:
	    # https://huggingface.co/transformers/model_doc/bert.html#bertmodel
	    hidden_states = outputs[2]
	'''
	print ("Number of layers:", len(hidden_states), "  (initial embeddings + 12 BERT layers)")
	layer_i = 0

	print ("Number of batches:", len(hidden_states[layer_i]))
	batch_i = 0

	print ("Number of tokens:", len(hidden_states[layer_i][batch_i]))
	token_i = 0

	print ("Number of hidden units:", len(hidden_states[layer_i][batch_i][token_i]))
	'''

	# Concatenate the tensors for all layers. We use `stack` here to
	# create a new dimension in the tensor.
	token_embeddings = torch.stack(hidden_states, dim=0)

	# Remove dimension 1, the "batches".
	token_embeddings = torch.squeeze(token_embeddings, dim=1)

	# `hidden_states` has shape [13 x 1 x 22 x 768]

	# `token_vecs` is a tensor with shape [22 x 768]
	token_vecs = hidden_states[-2][0]

	# Calculate the average of all 22 token vectors.
	sentence_embedding = torch.mean(token_vecs, dim=0)

	'''
	print ("Our final sentence embedding vector of shape:", sentence_embedding.size())
	print (sentence_embedding)
	'''
	return sentence_embedding

df = pd.read_csv('preProc1mTweets.csv')
vecs = []
for i in range(len(df['text'])):
	if len(vecs) == 30000:
		break
	df['text'][i] = "[CLS] " + df.loc[i, 'text'] + " [SEP]"
	vecs.append(np.array(bert(df['text'][i])))

fout = open('largeBert_' + str(len(vecs[0])) + '.bin', 'wb')
fout.write(struct.pack('<i', len(vecs)))
fout.write(struct.pack('<i', len(vecs[0])))
for i in range(len(vecs)):
    #print('df['text'][i].encode(sys.stdout.encoding', errors='replace'))
    for j in vecs[i]:
        fout.write(struct.pack('<d', j))
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