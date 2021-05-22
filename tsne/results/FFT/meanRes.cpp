#include <bits/stdc++.h>
using namespace std;

#define pb push_back

map<string, vector<double>> mPres, mMrpd;
map<string, int> cnt;

int main() {
    FILE *f;
    f = fopen("resultsL.txt", "r");
    char lines[300];
    while(fscanf(f, "%[^\n]", lines) != EOF){
        string line = lines;
        cerr << line << endl;
        vector<double> nbPres, nbMrpd;
        double x;
        for(int i = 0; i < 600; i++){
            fscanf(f, "%lf", &x);
            nbMrpd.pb(x);
        }
        for(int i = 0; i < 600; i++){
            fscanf(f, "%lf", &x);
            nbPres.pb(x);
        }
        string s = "";
        
        if(line.find("FFT") != string::npos) s = "fft";
        else s = "bh";
        if(line.find("News") != string::npos) s += "News";
        if(mPres[s].empty()) mPres[s].resize(600);
        if(mMrpd[s].empty()) mMrpd[s].resize(600);
        for(int i = 0; i < 600; i++) {
            mPres[s][i] += nbPres[i];
            mMrpd[s][i] += nbMrpd[i];
        }

        if(line.find("300") != string::npos) s += "pvector";
        else if(line.find("500") != string::npos) s += "pvector";
        else if(line.find("BaseBert") != string::npos) s += "Bert";
        else if(line.find("LargeBert") != string::npos) s += "Bert";
        cnt[s]++;
        if(mPres[s].empty()) mPres[s].resize(600);
        if(mMrpd[s].empty()) mMrpd[s].resize(600);
        for(int i = 0; i < 600; i++) {
            mPres[s][i] += nbPres[i];
            mMrpd[s][i] += nbMrpd[i];
        }
    }
    for(auto [key, value] : mPres){
        printf("%s\n", key.c_str());
        int a = 1;
        int c = cnt[key];
        printf("[");
        for(auto i : mMrpd[key]){
            printf("%lf%s ", i/c, a < 600 ? ", " : "");
            a += 1;            
        }
        printf("]\n");
        a = 1;
        printf("[");
        for(auto i : mPres[key]){
            printf("%lf%s ", i/c, a < 600 ? ", " : "");
            a += 1;            
        }
        printf("]\n");
    }
}