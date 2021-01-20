import sys
import random
import pandas as pd
import numpy as np
from gensim.models import Doc2Vec
from sklearn import utils
import gensim
from gensim.test.utils import get_tmpfile
from gensim.models.doc2vec import TaggedDocument
import re
import matplotlib.pyplot as plt
import nltk
from nltk.corpus import stopwords
from nltk.stem import PorterStemmer  
from bs4 import BeautifulSoup
import multiprocessing
from scipy.spatial import distance
from zipfile import ZipFile

snow = nltk.stem.SnowballStemmer('english')

def cleanText(text):
    text = BeautifulSoup(text, "lxml").text
    text = re.sub(r'\|\|\|', r' ', text) 
    text = re.sub(r'/', r' ', text)
    text = re.sub(r'http\S+', r'<URL>', text)
    text = re.sub(r'[?|!|\'|"|#]',r' ',text)
    text = re.sub(r'[.|,|)|(|\|/]',r' ',text)  
    text = text.lower()
    words = [snow.stem(word) for word in text.split() if word not in stopwords.words('english')]
    return words

def removeQuot(text):
    text = re.sub(r'\'', r'', text) 
    return text

def vec_for_learning(model, tagged_docs):
    sents = tagged_docs.values
    targets, regressors = zip(*[(doc.tags[0], model.infer_vector(doc.words, steps=20)) for doc in sents])
    return targets, regressors

def vec_text(model, tagged_docs, text):
    return [(model.infer_vector(doc[0].words, steps=20), doc[0], doc[1]) for doc in zip(tagged_docs, text)]

df = pd.read_csv('preProc1mTweets.csv')
#df['text2'] = df['text'].apply(cleanText)
df['text2'] = df['text2'].apply(removeQuot)
df['text2'] = df['text2'].apply(lambda s : s.strip('][').split(', '))
tagged_data = [TaggedDocument(words=_d, tags=[str(i)]) for i, _d in enumerate(df['text2'])]
cores = multiprocessing.cpu_count()
model_dbow = Doc2Vec(vector_size=300, dm=1, min_count=2, epochs=120, window=2)
model_dbow.build_vocab(tagged_data)
model_dbow.train(tagged_data, total_examples=model_dbow.corpus_count, epochs=model_dbow.epochs)
#fname = 'my_doc2vecBw_model120it'
#model_dbow = Doc2Vec.save(fname)
vecs = vec_text(model_dbow, tagged_data, df['text'])
for i in range(len(vecs)):
	print(df['text'][i].encode(sys.stdout.encoding, errors='replace'))
	for j in range(len(vecs[i][0])):
		print("%.15f" % vecs[i][0][j], end= '\n' if j == len(vecs[i][0]) - 1 else ' ')
