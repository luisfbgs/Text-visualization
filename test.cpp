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
			cerr << "i " << i << " d "<< dists[i].second << endl;
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
	long long ans = 0;
	int n = (int)Hvecs.size();
	vector<int> pos(n);
	vector<pair<double, int>> dists(n);
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			dists[j] = {edist(Hvecs[i], Hvecs[j]), j};
		}
		sort(dists.rbegin(), dists.rend());
		for(int j = 0; j < n; j++){
			pos[dists[j].second] = j;
		}

		for(int j = 0; j < n; j++){
			dists[j] = {edist(Lvecs[i], Lvecs[j]), j};
		}
		sort(dists.rbegin(), dists.rend());
		for(int j = 1; j <= 20; j++){
			ans += abs(j - pos[dists[i].second]);
		}
	}
	return ans / 20.0 / n;
}

int main(int argc, char *argv[]) {
	FILE *lowD, *highD, *highDText;
	lowD = fopen(argv[1], "rb");
	highD = fopen(argv[2], "rb");
	highDText = fopen(argv[3], "r");
	int n, d;
	fread(&n, sizeof(int), 1, lowD); // n ~ #vectors
	fread(&d, sizeof(int), 1, lowD); // d ~ #dimensions
	
	int nh, dh;
	fread(&nh, sizeof(int), 1, highD); // n ~ #vectors
	fread(&dh, sizeof(int), 1, highD); // d ~ #dimensions
	cerr << n << " " << d << endl;
	cerr << nh << " " << dh << endl;
	
	char s[100005];
	vector<string> text;
	vector<vector<double>> tVecld, tVec;
	while(fscanf(highDText, " %[^\n]", s) != EOF && tVec.size() < n){
		text.push_back(s);
		tVec.push_back(vector<double>(dh));
		tVecld.push_back(vector<double>(d));
		for(int j = 0; j < dh; j++){
			double x;
			fscanf(highD, "%lf", &x);
			tVec.back()[j] = x;
		}
		if(tVec.size() % 1000 == 0){
			cerr << tVec.size() << endl;
		}
	}
	for(int i = 0; i < n; i++){
		fread(&tVecld[i][0], sizeof(double), d, lowD);
	}
	normalize(tVec);
	fclose(lowD);
	fclose(highD);
	vector<int> ids = {5933, 7743, 16262, 1529, 29700, 5508, 29752, 17256, 7256, 8119, 9711, 8351, 12843, 8705, 24108, 26393, 18330, 27366, 22169, 27932};
/*	for(int i = 0; i < 20; i++){
		ids.push_back(rand()%n);
	}
	*/
	printf("Score %.15lf\n\n", evaluate(tVec, tVecld));
	printf("Full vectors\n");
	printf("------------------\n\n");
	solve(text, tVec, ids);
	printf("2D vectors\n");
	printf("------------------\n\n");
	solve(text, tVecld, ids, true);
	return 0;
}