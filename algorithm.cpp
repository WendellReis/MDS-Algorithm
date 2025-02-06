#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <chrono>
#include <math.h>
#include <set>

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
    for (size_t i = 0; i < adj.size(); i++) {
        set<int> conjunto;
        for (size_t j = 0; j < adj[i].size(); j++) {
            conjunto.insert(adj[i][j]);
        }

        for (size_t j = 0; j < adj[i].size(); j++) {
            for(size_t l=0; l < adj[adj[i][j]].size();l++) {
                if(conjunto.find(adj[adj[i][j]][l]) == conjunto.end()) {
                    candidates[i].push_back(adj[adj[i][j]][l]);
                }
            }
        }
    }
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

vector<float> getProbability(vector<float> dominators, vector<vector<int>> candidates) {
    vector<float> probability(0);
    for(auto v: dominators) {
        int qtd = 0;
        float sum = 0;
        for(auto candidate : candidates[v])
            sum+=dominators[candidate];
        probability.push_back(sum/qtd);
    }
    return probability;
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

        vector<float> probability = getProbability(dominantes, candidates);
        
        vector<pair<float,int>> dProbability(0);
        int c = 0;
        for(auto k : probability) {
            dProbability.push_back({k, c});
            c++;
        }

        sort(dProbability.rbegin(), dProbability.rend());

        set<int> coveredVs;
        set<int> domination;
        for(auto v : dProbability) {
            if(v.second == 0)
                continue;
            if(coveredVs.find(v.second) == coveredVs.end()) {
                for (size_t i = 0; i < adj[v.second].size(); i++)
                    coveredVs.insert(adj[v.second][i]);
                domination.insert(v.second);
            }
        }

        auto stop = high_resolution_clock::now();

        auto duration = duration_cast<seconds>(stop - start);
        long long duration_ms = duration_cast<milliseconds>(stop - start).count();
        
        long long hours = duration_ms / 3600000;
        long long minutes = (duration_ms % 3600000) / 60000;
        long long seconds = (duration_ms % 60000) / 1000;

        cout << "Domination set: [";
        auto iterator = domination.begin();
        for(int i=0;i<domination.size();i++) {
            if(i+1 == domination.size())
                cout << *iterator;
            else
                cout << *iterator << ", ";
            iterator++;
        }
        cout << "]\n";
        cout << "Domination size: " << domination.size() << '\n';

        cout << "Size of covered: " << domination.size() + coveredVs.size() << '\n';

        cout << "Filename: " << argv[i] << endl;
        cout << "Tempo de execucao: " 
            << hours << " horas, "
            << minutes << " minutos e "
            << seconds << " segundos." << endl;
    }

    return 0;
}
