#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <string>

using namespace std;

unordered_set<int> MDS;

void inicializeMDS(int size) {
   for (size_t i = 1; i <= size; i++)
        MDS.insert(i);
}

bool isDominant(unordered_set<int> &ds, vector<vector<int>> &adj, int size) {
    for(int v = 1; v <= size; v++) {
        if(ds.find(v) != ds.end())
            continue;
        
        bool dominated = false;
        for(auto u : adj[v])
            if(ds.find(u) != ds.end()) {
                dominated = true;
                break;
            }
        if(!dominated)
            return false;
    }
    return true;
}

bool updateDominant(unordered_set<int> &ds) {
    if(ds.size() > MDS.size())
        return false;

    MDS = ds;
    return true;
}

void solve(unordered_set<int> ds, vector<vector<int>> &adj, int size, int idx = 0) {
    if(ds.size() > MDS.size()) {
        if(isDominant(ds,adj,size))
            updateDominant(ds);
        return;
    }

    for(int v = idx+1; v <= size; v++) {
        if (ds.size() >= MDS.size() && !MDS.empty()) return;

        if (isDominant(ds, adj, size)) {
            updateDominant(ds);
            return;
        }
        
        ds.insert(v);
        solve(ds,adj,size,v);
        ds.erase(v);
    }
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

        int size = adj.size()-1;

        unordered_set<int> ds;
        inicializeMDS(size);
        solve(ds,adj,size);
        printf("MDS = %d : [ ", (int)MDS.size());
        for(auto v : MDS)
            cout << v << " ";
        cout << "]\n";
    }

    return 0;
}