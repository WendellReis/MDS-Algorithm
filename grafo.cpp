#include "grafo.h"
#include <iostream>

using namespace std;

int Aresta::getPeso()
{
    return this->_peso;
}

int Aresta::getDe()
{
    return this->_u;
}

int Aresta::getPara()
{
    return this->_v;
}

Aresta::Aresta(size_t u, size_t v, size_t p)
{
    this->_u = u;
    this->_u = v;
    this->_u = p;
}

Grafo::Grafo(size_t size)
{
    this->_numVertices = size;
    this->_listaAdj = vector<vector<pair<size_t, size_t>>>(size, vector<pair<size_t, size_t>>(1, {0,0}));
}

size_t Grafo::getNumVertices() {
    return this->_numVertices;
}

vector<vector<pair<size_t, size_t>>> Grafo::getListaAdj() {
    return this->_listaAdj;
}

void Grafo::insereAresta(Aresta a) {
    this->_listaAdj[a.getDe()].push_back({a.getPara(), a.getPeso()});
}

void Grafo::insereAresta(size_t u, size_t v, size_t p) {
    this->_listaAdj[u].push_back({v, p});
}

void Grafo::imprime() {
    for (size_t i = 0; i < this->getNumVertices(); i++)
    {
        cout << "[" << i << "]: [";
        size_t j = 0;
        for (; j < this->_listaAdj[i].size()-1; j++)
            cout << this->_listaAdj[i][j].first << ", ";
        cout << this->_listaAdj[i][j].first << "]\n";
    }
    
}