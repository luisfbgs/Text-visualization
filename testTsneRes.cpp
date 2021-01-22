#include<bits/stdc++.h>

using namespace std;

double cdist(vector<double> a, vector<double> b){
	double dot = 0, magA = 0, magB = 0;
	for(int i = 0; i < (int)a.size(); i++){
		dot += a[i] * b[i];
		magA += a[i] * a[i];
		magB += b[i] * b[i];
	}
	return dot / sqrt(magA) / sqrt(magB);
}

void solve(vector<string> &text, vector<vector<double>> tVec, vector<int> ids){
	int n = tVec.size();
	for(int idx : ids){
		cerr << idx << endl;
		vector<pair<double, int>> dists;
		for(int i = 0; i < n; i++){
			dists.push_back({cdist(tVec[idx], tVec[i]), i});
		}
		sort(dists.rbegin(), dists.rend());
		printf("Compared text %d : %s\n", idx, text[idx].c_str());
		printf("Closest 10 --------------------------\n");
		for(int i = 0; i < 10; i++){
			printf("%lf %s\n", dists[i].first, text[dists[i].second].c_str());
		}
		printf("\n");
	}
}

// Function that runs the Barnes-Hut implementation of t-SNE
int main() {
	FILE *fl;
	fl = fopen("result.dat", "rb");
	int n, d;
	fread(&n, sizeof(int), 1, fl); // n ~ #vectors
	fread(&d, sizeof(int), 1, fl); // d ~ #dimensions
	cerr << n << " " << d << endl;
	char s[100005];
	vector<string> text;
	vector<vector<double>> tVec2d, tVec;
	while(scanf("%[^\n]", s) != EOF && tVec.size() < n){
		getchar();
		text.push_back(s);
		tVec.push_back(vector<double>(300));
		tVec2d.push_back(vector<double>(2));
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
	for(int i = 0; i < n; i++){
		for(int j = 0; j < d; j++){
			fread(&tVec2d[i][j], sizeof(double), 1, fl);
		}
	}
	fclose(fl);
	cerr << "FIM" << endl;
	vector<int> ids;
	for(int i = 0; i < 20; i++){
		ids.push_back(rand()%n);
	}
	printf("Full vectors\n");
	printf("------------------\n\n");
	solve(text, tVec, ids);
	printf("2D vectors\n");
	printf("------------------\n\n");
	solve(text, tVec2d, ids);
	return 0;
}