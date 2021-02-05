#include<bits/stdc++.h>

using namespace std;

map<pair<int, int>, int> pos;

double norm(vector<double> v){
	double ret = 0;
	for(double x : v){
		ret += x * x;
	}
	return sqrt(ret);
}

double cdist(vector<double> a, vector<double> b){
	double dot = 0, magA = 0, magB = 0;
	for(int i = 0; i < (int)a.size(); i++){
		dot += a[i] * b[i];
		magA += a[i] * a[i];
		magB += b[i] * b[i];
	}
	return dot / sqrt(magA) / sqrt(magB);
}

double edist(vector<double> a, vector<double> b){
	double dist = 0;
	for(int i = 0; i < (int)a.size(); i++){
		dist += (a[i] - b[i]) * (a[i] - b[i]);
	}
	return -dist;
}

void solve(vector<string> &text, vector<vector<double>> tVec, vector<int> ids, bool showpos = false){
	int n = tVec.size();
	for(int idx : ids){
		cerr << idx << endl;
		vector<pair<double, int>> dists;
		for(int i = 0; i < n; i++){
			dists.push_back({edist(tVec[idx], tVec[i]), i});
		}
		sort(dists.rbegin(), dists.rend());
		printf("Compared text %d : %s\n", idx, text[idx].c_str());
		printf("Closest 20 --------------------------\n");
		for(int i = 1; i <= 20; i++){
			if(showpos){
				printf("OldPos %d | ", pos[{idx, dists[i].second}]);
			}
			printf("%d %s\n", i, text[dists[i].second].c_str());
		}
		printf("\n");
		if(showpos){
			printf("New positions of full vectors 20 closest\n");
			for(int i = 1; i < n; i++){
				if(pos[{idx, dists[i].second}] < 20){
					printf("OldPos %d NewPos %d\n", pos[{idx, dists[i].second}], i);
				}
			}
		}
		for(int i = 0; i < n; i++){
			pos[{idx, dists[i].second}] = i;
		}
		if(showpos){
			printf("\n");
		}
	}
}

void normalize(vector<vector<double>> &vecs){
	for(auto &v : vecs){
		double vnorm = norm(v);
		for(auto &x : v){
			x /= vnorm;
		}
	}
}

double evaluate(vector<vector<double>> &Hvecs, vector<vector<double>> &Lvecs){
	double ans = 0;
	int n = (int)Hvecs.size();
	vector<int> pos(n);
	for(int i = 0; i < n; i++){
		cerr << i << endl;
		vector<pair<double, int>> dists;
		for(int j = 0; j < n; j++){
			dists.push_back({edist(Hvecs[i], Hvecs[j]), j});
		}
		sort(dists.rbegin(), dists.rend());
		for(int j = 0; j < n; j++){
			pos[dists[j].second] = j;
		}

		dists.clear();
		for(int j = 0; j < n; j++){
			dists.push_back({edist(Lvecs[i], Lvecs[j]), j});
		}
		sort(dists.rbegin(), dists.rend());
		assert(dists[0].second == i);
		for(int j = 1; j < 20; j++){
			ans += log2(abs(j - pos[dists[i].second])+1);
		}
	}
	return ans / n;
}

int main() {
	FILE *fl;
	fl = fopen("resultNormT02P20.dat", "rb");
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
	normalize(tVec);
	fclose(fl);
	cerr << "FIM" << endl;
	vector<int> ids;
	for(int i = 0; i < 20; i++){
		ids.push_back(rand()%n);
	}
	printf("Score %.15lf\n\n", evaluate(tVec, tVec2d));
	printf("Full vectors\n");
	printf("------------------\n\n");
	solve(text, tVec, ids);
	printf("2D vectors\n");
	printf("------------------\n\n");
	solve(text, tVec2d, ids, true);
	return 0;
}