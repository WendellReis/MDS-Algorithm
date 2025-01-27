#include <bits/stdc++.h>

using namespace std;

vector<int> MDS;
int one_idx = 1;

void inicializeMDS(int size) {
    MDS = vector<int>(size);

   for (size_t i = 0; i < size; i++)
        MDS[i] = i;
}

bool isDominant(vector<int> &ds, vector<vector<int>> &adj, int size) {
    printf("k = %d\n",(int)ds.size());
    for(int v = one_idx; v < size + one_idx; v++) {
        for(auto k : ds) {
            if(k == v || find(adj[v].begin(),adj[v].end(),k) != adj[v].begin())
                continue;
            return false;
        }
    }
    return true;
}

bool updateDominant(vector<int> &ds) {
    if(ds.size() > MDS.size())
        return false;

    MDS = ds;
    return true;
}

void solve(vector<int> ds, vector<vector<int>> &adj, int size) {
    if(ds.size() > MDS.size())
        return;
    if(isDominant(ds,adj,size)) {
        updateDominant(ds);
        return;
    }

    for(int v = one_idx; v < size + one_idx; v++) {
        if(find(ds.begin(),ds.end(),v) != ds.end())
            continue;
        
        ds.push_back(v);
        solve(ds,adj,size);
        ds.pop_back();
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
        if(u == 0 || v == 0)
            one_idx = 0;
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

        int size = adj.size() - one_idx;
        for(int i = one_idx; i < size + one_idx; i++) {
            printf("%d: ",i);
            for(auto u : adj[i])
                cout << u << " ";
            cout << endl;
        }

        vector<int> ds;
        inicializeMDS(size);
        solve(ds,adj,size);
        printf("MDS = %d\n", (int)MDS.size());
    }

    return 0;
}