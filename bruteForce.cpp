#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <chrono>
#include <filesystem>

using namespace std;
using namespace chrono;

string PATH = "results/";

vector<bool> MDS;
int MDS_size = 0;

string getFilename(const std::string& caminho) {
    std::filesystem::path p(caminho);
    return p.stem().string();
}

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

bool isDominant(vector<bool> &ds, vector<vector<int>> &adj, int size) {
    for(int v = 1; v <= size; v++) {
        if(ds[v])
            continue;
        
        bool dominated = false;
        for(auto u : adj[v])
            if(ds[u]) {
                dominated = true;
                break;
            }
        if(!dominated)
            return false;
    }
    return true;
}

bool updateDominant(vector<bool> &ds, int sizeds) {
    if(sizeds >= MDS_size)
        return false;

    MDS = ds;
    MDS_size = sizeds;
    displayMDS();
    return true;
}

void solve(vector<bool> ds, vector<vector<int>> &adj,int size, int sizeds = 0, int idx = 0) {
    if (sizeds >= MDS_size) return;

    if (isDominant(ds, adj, size)) {
        updateDominant(ds, sizeds);
        return;
    }

    for (int v = idx; v <= size; v++) {
        ds[v] = true;
        solve(ds, adj, size, sizeds + 1, v + 1);
        ds[v] = false;
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

        auto start = high_resolution_clock::now();
   
        int size = adj.size()-1;
        vector<bool> ds(size+1,0);
        MDS = vector<bool>(size+1,1);
        MDS_size = size;
        solve(ds,adj,MDS_size); 

        auto stop = high_resolution_clock::now();

        auto duration = duration_cast<seconds>(stop - start);
        long long duration_ms = duration_cast<milliseconds>(stop - start).count();
        
        long long hours = duration_ms / 3600000;
        long long minutes = (duration_ms % 3600000) / 60000;
        long long seconds = (duration_ms % 60000) / 1000;

        string f = argv[i];
        string filename = PATH + "fb" + "_" + getFilename(f) + ".txt";
        ofstream file(filename);
        if(!file.is_open()) {
            cerr << "Erro ao abrir o arquivo: " << filename << endl;
            break;
        }

        file << "Filename: " << argv[i] << endl;
        cout << MDS_size << endl;
        file << "MDS SIZE: " << MDS_size << endl;
        file << "MDS = [";
        for(int i = 1; i <= size; i++) {
            if(MDS[i] == 1) {
                file << i;
                if(MDS_size > 1)
                    file << ", ";
                MDS_size--;
            }
        }
        file << "]\n";
            
        file << "Tempo de execucao: " 
            << hours << " horas, "
            << minutes << " minutos e "
            << seconds << " segundos." << endl;
        file.close();
    }

    return 0;
}
