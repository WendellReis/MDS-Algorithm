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

vector<int> buildDegreeByBase(vector<vector<int>> &adj, float base) {
    vector<int> deg(0);
    for (size_t i = 0; i < adj.size(); i++)
    {
        int j = 1; 
        while(pow(base, j) < adj[i].size())
            j++;
        deg.push_back((int)pow(base, j));
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

vector<set<int>> buildCandidateByBasedDegree(vector<vector<int>> &adj, vector<int> &basedDegree) {
    vector<set<int>> candidates = vector<set<int>>(adj.size());
    for (size_t v = 0; v < adj.size(); v++) {
        int dv = basedDegree[v];
        for (size_t w = 0; w < adj[v].size(); w++) {
            int dw = basedDegree[adj[v][w]];
            if(v != adj[v][w]) {
                if(dv >= dw) {
                    candidates[v].insert(adj[v][w]);
                }   
            }
            for(size_t l=0; l < adj[adj[v][w]].size();l++) {
                if(v != adj[adj[v][w]][l]) {
                    dw = basedDegree[adj[adj[v][w]][l]];
                    if(dv >= dw) {
                        candidates[v].insert(adj[adj[v][w]][l]);
                    }
                }
            }
        }
    }

    return candidates;    
}

vector<pair<int, vector<int>>> buildSupport(vector<vector<int>> &adj, vector<set<int>> candidates) {
    vector<pair<int, vector<int>>> v(0);
    for (size_t i = 0; i < adj.size(); i++)
    {
        int qtd = 0;
        vector<int> suporteds(0);
        for (size_t j = 0; j < candidates.size(); j++)
        {
            for(auto candidate : candidates[j])
                if(i == candidate) {
                    suporteds.push_back(j);
                    qtd++;
                    break;
                }
        }
        v.push_back({i, suporteds});
    }

    return v;
}


vector<float> solve(vector<set<int>> &candidates, vector<pair<int, vector<int>>> &supports) {
    vector<float> D(0);
    for (size_t v = 0; v < candidates.size(); v++)
    {
        int medv = 0;
        for(int candidate : candidates[v])
            medv+=supports[candidate].second.size();
        D.push_back(medv > 0 ? (float)1/medv : 0);
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
        
        vector<vector<int>> adj = getGraph(argv[i]);
        if(adj.size() == 0) {
            cerr << "Erro ao criar lista de adjascencia!\n";
            continue;
        }
        float base = 10;

        auto start = high_resolution_clock::now();
   
        int size = adj.size()-1;
        vector<int> degBased= buildDegreeByBase(adj, base);
        vector<set<int>> candidates = buildCandidateByBasedDegree(adj, degBased);
        vector<pair<int, vector<int>>> supportedCanidates = buildSupport(adj, candidates);
        vector<float> dominantes = solve(candidates, supportedCanidates);
        
        vector<pair<float,int>> dProbability(0);
        int k=0;
        for(auto d: dominantes) {
            if(k > 0 && d == 0)
                d=1;
            dProbability.push_back({d, k});
            k++;
        }

        sort(dProbability.begin(), dProbability.end());
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
            if(coveredVs.size() + domination.size() == size)
                break;
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
