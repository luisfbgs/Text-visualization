#include<bits/stdc++.h>

using namespace std;

double norm(vector<double> v){
	double ret = 0;
	for(double x : v){
		ret += x * x;
	}
	return sqrt(ret);
}

void normalize(vector<vector<double>> &vecs){
	for(auto &v : vecs){
		double vnorm = norm(v);
		for(auto &x : v){
			x /= vnorm;
		}
	}
}

int main(int argc, char *argv[]){
	FILE *lowD, *highD;
	lowD = fopen(argv[1], "wb");
	highD = fopen(argv[2], "r");
	char s[100005];
	vector<vector<double>> tVec;
	while(fscanf(highD, " %[^\n]", s) != EOF && tVec.size() < 3e4){
		tVec.push_back(vector<double>(300));
		for(int j = 0; j < 300; j++){
			double x;
			fscanf(highD, "%lf", &x);
			tVec.back()[j] = x;
		}
		if(tVec.size() % 1000 == 0){
			cerr << tVec.size() << endl;
		}
	}
	normalize(tVec);
	int aux = tVec.size();
	fwrite(&aux, sizeof(int), 1, lowD); // N ~ #vectors
	aux = 300;
	fwrite(&aux, sizeof(int), 1, lowD); // D ~ #dimensions 
	double daux = .5;
	fwrite(&daux, sizeof(double), 1, lowD); // theta
	daux = 30;
	fwrite(&daux, sizeof(double), 1, lowD); // perplexity
	aux = 2;
	fwrite(&aux, sizeof(int), 1, lowD); // no_dims
	aux = 2000;
	fwrite(&aux, sizeof(int), 1, lowD); // max_iter
	// NxD matrix
	for(auto x : tVec){
		for(auto y : x){
			fwrite(&y, sizeof(double), 1, lowD);
		}
	}
	fclose(lowD);
	return 0;
}