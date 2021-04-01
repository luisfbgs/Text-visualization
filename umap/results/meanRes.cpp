#include <bits/stdc++.h>
using namespace std;

#define pb push_back

map<string, vector<double>> mPres, mMrpd;
map<string, int> cnt;

int main() {
    FILE *f;
    f = fopen("umapresults.txt", "r");
    char lines[300];
    while(fscanf(f, "%[^\n]", lines) != EOF){
        string line = lines;
        vector<double> nbPres, nbMrpd;
        double x;
        for(int i = 0; i < 10; i++){
            fscanf(f, "%lf", &x);
            nbMrpd.pb(x);
        }
        for(int i = 0; i < 10; i++){
            fscanf(f, "%lf", &x);
            nbPres.pb(x);
        }
        string s = "";
        
        if(line.find("Dens") != string::npos) s = "Dens";
        else s = "base";
        if(line.find("News") != string::npos) s += "News";
        cnt[s]++;
        if(mPres[s].empty()) mPres[s].resize(10);
        if(mMrpd[s].empty()) mMrpd[s].resize(10);
        for(int i = 0; i < 10; i++) {
            mPres[s][i] += nbPres[i];
            mMrpd[s][i] += nbMrpd[i];
        }

        if(line.find("300") != string::npos) s += "300";
        else if(line.find("500") != string::npos) s += "500";
        else if(line.find("1000") != string::npos) s += "1000";
        else if(line.find("BaseBert") != string::npos) s += "BaseBert";
        else if(line.find("LargeBert") != string::npos) s += "LargeBert";
        cnt[s]++;
        if(mPres[s].empty()) mPres[s].resize(10);
        if(mMrpd[s].empty()) mMrpd[s].resize(10);
        for(int i = 0; i < 10; i++) {
            mPres[s][i] += nbPres[i];
            mMrpd[s][i] += nbMrpd[i];
        }
    }
    for(auto [key, value] : mPres){
        printf("%s\n", key.c_str());
        int c = cnt[key];
        for(auto i : mMrpd[key]){
            printf("%lf ", i/c);
        }
        printf("\n");
        for(auto i : mPres[key]){
            printf("%lf ", i/c);
        }
        printf("\n");
    }
}