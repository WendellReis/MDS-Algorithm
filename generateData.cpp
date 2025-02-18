#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

#define SEED 4321
#define PATH "archives/"

int main() {
    srand(SEED);

    vector<int> SIZES = {5};

    for(auto s : SIZES) {
        string filename = string(PATH) + "g" + to_string(s) + "_" + to_string(SEED) +  ".txt";
        ofstream file(filename);
        if(!file.is_open()) {
            cerr << "Erro ao abrir o arquivo: " << filename << endl;
            break;
        }

        file << s << endl;
        for(int i = 1; i <= s; i++)
            for(int j = i+1; j <= s; j++)
                if(rand()%2)
                    file << i << " " << j << endl;
        file.close();
    }

    return 0;
}
