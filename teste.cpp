#include "grafo.h"
#include <iostream>

using namespace std;

int main() {
    Grafo g = Grafo(5);
    for (size_t i = 0; i < 4; i++)
        g.insereAresta(i, i+1, i+3);
    g.imprime();
    
}