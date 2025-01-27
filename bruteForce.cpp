#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

vector<int> MDS;

void inicializeMDS(int size) {
    MDS = vector<int>(size);

   for (size_t i = 0; i < size; i++)
        MDS[i] = i;
}

bool isDominant(vector<int> &ds, vector<vector<int>> &adj, int size) {
    for(int v = 0; v < size; v++) {
        for(auto k : ds) {
            if(find(adj[v].begin(),adj[v].end(),k) != adj[v].begin())
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

    for(int v = 0; v < size; v++) {
        if(find(ds.begin(),ds.end(),v) != ds.end())
            continue;
        
        ds.push_back(v);
        solve(ds,adj,size);
        ds.pop_back();
    }
}

int main() {
    vector<int> ds = {1,2,3};
    inicializeMDS(5);
    updateDominant(ds);
    cout << MDS.size() << endl;
    return 0;
}