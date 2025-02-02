#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <chrono>
#include <math.h>

using namespace std;
using namespace chrono;

typedef struct supported {
    int value;
    int supported;
} supported; 

vector<bool> MDS;
int MDS_size = 0;

void displayMDS() {
    cout << "MDS ("<< MDS_size << ") = [";
    bool first = true;
    for (int i = 1; i < MDS.size(); i++) {
        if (MDS[i]) {
            if (!first) cout << ", ";
            cout << i;
            first = false;
        }
    }
    cout << "]\n";
}

vector<int> buildDegreeByBase(vector<vector<int>> &adj, int base) {
    vector<int> deg(0);
    for (size_t i = 0; i < adj.size(); i++)
    {
        int j = 1; 
        while(pow(base, j) < adj[i].size())
            j++;
        deg.push_back((int)pow(base, --j == 0 ? 1 : j));
    }
    return deg;
}

bool isAdj(vector<vector<int>> &adj, int u, int v) {
    for(int vAdj : adj[u]) {
        if(v == vAdj)
            return true;
    }
    return false;
}

vector<vector<int>> buildCandidateByBasedDegree(vector<vector<int>> &adj, vector<int> &basedDegree) {
    vector<vector<int>> candidates = vector<vector<int>>(adj.size());
    for (size_t i = 0; i < adj.size(); i++)
        for (size_t j = 0; j < adj.size(); j++)
            if(!isAdj(adj, i, j) && basedDegree[i] <= basedDegree[j])
                candidates[i].push_back(j);
    return candidates;    
}

vector<int> buildSupport(vector<vector<int>> &adj, vector<vector<int>> candidates) {
    vector<int> v(0);
    for (size_t i = 0; i < adj.size(); i++)
    {
        int qtd = 0;
        for (size_t j = 0; j < candidates.size(); j++)
        {
            for(auto candidate : candidates[j])
                if(i == candidate) {
                    qtd++;
                    break;
                }
        }
        v.push_back(qtd);
        
    }
    
    return v;
}


vector<float> solve(vector<vector<int>> &candidates, vector<int> &supports) {
    vector<float> D(0);
    for (size_t i = 0; i < candidates.size(); i++)
    {
        int qtd = 0;
        for(int v : candidates[i])
            qtd+=supports[v];
        D.push_back((float)1/qtd);
    }
    
    return D;
}

vector<vector<int>> getGraph(const string &filename) {
    vector<vector<int>> adj(0);
    ifstream file(filename);
    if(!file.is_open()) {
        cerr << "Erro ao abrir o arquivo: " << filename << endl;
        return adj;
    }

    int size;
    file >> size;
    if(size <= 0) {
        file.close();
        return adj;
    }
    
    adj = vector<vector<int>>(size+1);
    int u,v;
    while(file >> u >> v) {
        if (u <= 0 || v <= 0 || u > size || v > size) {
            cerr << "Vértice inválido: " << u << " ou " << v << endl;
            continue;
        }

        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    file.close();
    return adj;
} 

int main(int argc, char* argv[]) {
    if(argc < 2) {
        cerr << "Erro: arquivos nao especificados\n";
        return EXIT_FAILURE;
    }


    for(int i = 1; i < argc; i++) {
        
        cout << argv[i] << '\n';
        vector<vector<int>> adj = getGraph(argv[i]);
        if(adj.size() == 0) {
            cerr << "Erro ao criar lista de adjascencia!\n";
            continue;
        }
        int base = 2;

        auto start = high_resolution_clock::now();
   
        int size = adj.size()-1;
        vector<int> degBased= buildDegreeByBase(adj, base);
        vector<vector<int>> candidates = buildCandidateByBasedDegree(adj, degBased);
        vector<int> supportedCanidates = buildSupport(adj, candidates);
        vector<float> dominantes = solve(candidates, supportedCanidates);

        auto stop = high_resolution_clock::now();

        /*
        int c = 0;
        for(auto k : supportedCanidates) {
            cout << c << ": " << k << '\n';
            c++;
        }

        int c=0;
        for(auto d : candidates) {
            cout << c << "[";
            for(auto k : d)
                cout << k << ',';
            c++;
            cout << "]\n";
        }
        */
        cout << "Dominantes: \n";
        int c = 0;
        for(auto k : dominantes) {
            cout << c << ": " << k << '\n';
            c++;
        }

        auto duration = duration_cast<seconds>(stop - start);
        long long duration_ms = duration_cast<milliseconds>(stop - start).count();
        
        long long hours = duration_ms / 3600000;
        long long minutes = (duration_ms % 3600000) / 60000;
        long long seconds = (duration_ms % 60000) / 1000;

        cout << "Filename: " << argv[i] << endl;
        cout << "Tempo de execucao: " 
            << hours << " horas, "
            << minutes << " minutos e "
            << seconds << " segundos." << endl;
    }

    return 0;
}
