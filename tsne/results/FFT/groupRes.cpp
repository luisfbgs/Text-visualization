#include <bits/stdc++.h>
namespace fs = std::filesystem;
using namespace std;

int main() {
    std::string path = "./";
    for (const auto & entry : fs::directory_iterator(path)){
    	std::string s = entry.path().string();
    	if(s.back() == 'p' || s.back() == 'e' || s.back() == 't' || s.back() == 'y') continue;
        printf("%s\n", s.c_str());
    	s += + "/presultsL.txt";
        std::cerr << s << std::endl;
        FILE *f;
        f = fopen(s.c_str(), "r");
        char line[300];
        fscanf(f, "%[^\n]", line);
        for(int i = 0; i < 600; i++){
        	int x;
        	double a, b;
        	fscanf(f, "%d - %lf - %lf\n", &x, &a, &b);
        	printf("%lf", b);
        }
        printf("\n");
        fscanf(f, "%[^\n]", line);
        cerr << string(line) << endl;
        for(int i = 0; i < 600; i++){
        	int x;
        	double b;
        	fscanf(f, "%d - %lf", &x, &b);
        	printf("%lf", b);
        }
        printf("\n");
    }
}