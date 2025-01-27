#include <vector>

using namespace std;

class Aresta {
    private:
        size_t _peso;
        size_t _u;
        size_t _v;

    public:
        int getPeso();

        int getDe();

        int getPara();

        Aresta(size_t u, size_t v, size_t p);
};

class Grafo {
    private:
        vector<vector<pair<size_t, size_t>>> _listaAdj;
        size_t _numVertices;

    public:
        Grafo( size_t size);
        vector<vector<pair<size_t, size_t>>> getListaAdj();
        size_t getNumVertices();
        void insereAresta(Aresta a);
        void insereAresta(size_t u, size_t v, size_t p);
        void imprime();
};