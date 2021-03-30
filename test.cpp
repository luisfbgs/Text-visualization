#include<bits/stdc++.h>

using namespace std;

FILE *colors, *colors2, *out;
bool labeled;
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
		fprintf(out, "Compared text %d : %s\n", idx, text[idx].c_str());
		fprintf(out, "Closest 20 --------------------------\n");
		for(int i = 1; i <= 20; i++){
			if(showpos){
				fprintf(out, "OldPos %d | ", pos[{idx, dists[i].second}]);
			}
			cerr << "i " << i << " d "<< dists[i].second << endl;
			fprintf(out, "%d %s\n", i, text[dists[i].second].c_str());
		}
		fprintf(out, "\n");
		if(showpos){
			fprintf(out, "New positions of full vectors 20 closest\n");
			for(int i = 1; i < n; i++){
				if(pos[{idx, dists[i].second}] < 20){
					fprintf(out, "OldPos %d NewPos %d\n", pos[{idx, dists[i].second}], i);
				}
			}
		}
		for(int i = 0; i < n; i++){
			pos[{idx, dists[i].second}] = i;
		}
		if(showpos){
			fprintf(out, "\n");
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

void evaluate(vector<vector<double>> &Hvecs, vector<vector<double>> &Lvecs){
	const int szs = 10, step = 5;
	double sum[szs * step+1];
	double np[szs * step+1];
	int n = (int)Hvecs.size();
	vector<int> pos(n), rpos(n);
	vector<pair<double, int>> dists(n);
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			dists[j] = {edist(Hvecs[i], Hvecs[j]), j};
		}
		sort(dists.rbegin(), dists.rend());
		for(int j = 0; j < n; j++){
			pos[dists[j].second] = j;
			rpos[j] = dists[j].second;
		}

		for(int j = 0; j < n; j++){
			dists[j] = {edist(Lvecs[i], Lvecs[j]), j};
		}
		sort(dists.rbegin(), dists.rend());
		long long atsum = 0;
		set<int> idxs;
		for(int j = 1; j <= step * szs; j++){
			atsum += abs(j - pos[dists[j].second]);
			idxs.insert(dists[j].second);
			idxs.insert(rpos[j]);
			if(!(j%step)){
				sum[j] += 1. * atsum / j;
				np[j] += (2. * j -  idxs.size()) / j;
			}
			if(j == 20){
				double aux = atsum / n / 20.;
				fwrite(&aux, sizeof(double), 1, colors);
			}
		}
	}
	fprintf(out, "K - Mean K closest position differences\n");
	for(int i = 1; i <= szs; i++){
		fprintf(out, "%d - %lf - %lf\n", i*step, sum[i*step] / n, 1 - sum[i*step] / n / n);
	}
	fprintf(out, "K - Neighborhood preservation\n");
	for(int i = 1; i <= szs; i++){
		fprintf(out, "%d - %lf\n", i*step, np[i*step] / n);
	}
}

int main(int argc, char *argv[]) {
	FILE *lowD, *highD, *highDText;
	lowD = fopen((string(argv[1]) + "results.dat").c_str(), "rb");
	highD = fopen(argv[2], "rb");
	highDText = fopen(argv[3], "r");
	labeled = stoi(argv[4]);
	colors = fopen((string(argv[1]) + "labels.bin").c_str(), "wb");
	lowD = fopen((string(argv[1]) + "results.dat").c_str(), "rb");
	out = fopen((string(argv[1]) + "presults.txt").c_str(), "w");
	if(labeled)
		colors2 = fopen((string(argv[1]) + "olabes.bin").c_str(), "wb");
	int n, d;
	fread(&n, sizeof(int), 1, lowD); // n ~ #vectors
	fread(&d, sizeof(int), 1, lowD); // d ~ #dimensions
	cerr << n << " " << d << endl;
	
	int nh, dh;
	fread(&nh, sizeof(int), 1, highD); // n ~ #vectors
	fread(&dh, sizeof(int), 1, highD); // d ~ #dimensions
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
			fread(&x, sizeof(double), 1, highD);
			tVec.back()[j] = x;
		}
		if(tVec.size() % 1000 == 0){
			cerr << tVec.size() << endl;
		}
	}
	for(int i = 0; i < n; i++){
		fread(&tVecld[i][0], sizeof(double), d, lowD);
	}
	if(labeled){
		vector<int> c(n);
		fread(&c[0], sizeof(int), n, highD);
		int maxi = -1e9;
		for(int i : c){
			maxi = max(maxi, i);
		}
		for(int i : c){
			double aux = 1. * i / maxi;
			fwrite(&aux, sizeof(double), 1, colors2);
		}
	}
	normalize(tVec);
	fclose(lowD);
	fclose(highD);
	vector<int> ids = {5933, 7743, 16262, 1529, 29700, 5508, 29752, 17256, 7256, 8119, 9711, 8351, 12843, 8705, 24108, 26393, 18330, 27366, 22169, 27932};
	for(int &i : ids)
		i = i%n;
	evaluate(tVec, tVecld);
	fprintf(out, "Full vectors\n");
	fprintf(out, "------------------\n\n");
	solve(text, tVec, ids);
	fprintf(out, "2D vectors\n");
	fprintf(out, "------------------\n\n");
	solve(text, tVecld, ids, true);
	return 0;
}