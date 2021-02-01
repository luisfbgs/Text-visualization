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

int main(){
	FILE *fl;
	fl = fopen("data.dat", "wb");
	char s[100005];
	vector<vector<double>> tVec;
	while(scanf("%[^\n]", s) != EOF && tVec.size() < 3e4){
		getchar();
		tVec.push_back(vector<double>(300));
		for(int j = 0; j < 300; j++){
			double x;
			scanf("%lf", &x);
			tVec.back()[j] = x;
		}
		scanf("%[^\n]", s);
		getchar();
		if(tVec.size() % 1000 == 0){
			cerr << tVec.size() << endl;
		}
	}
	normalize(tVec);
	int aux = tVec.size();
	fwrite(&aux, sizeof(int), 1, fl); // N ~ #vectors
	aux = 300;
	fwrite(&aux, sizeof(int), 1, fl); // D ~ #dimensions 
	double daux = .5;
	fwrite(&daux, sizeof(double), 1, fl); // theta
	daux = 30;
	fwrite(&daux, sizeof(double), 1, fl); // perplexity
	aux = 2;
	fwrite(&aux, sizeof(int), 1, fl); // no_dims
	aux = 2000;
	fwrite(&aux, sizeof(int), 1, fl); // max_iter
	// NxD matrix
	for(auto x : tVec){
		for(auto y : x){
			fwrite(&y, sizeof(double), 1, fl);
		}
	}
	fclose(fl);
	return 0;
}