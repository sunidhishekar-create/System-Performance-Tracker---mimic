#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>
#define MAX1 10 // max processes + resources

typedef struct {
    int adj[MAX1][MAX1]; // adjacency matrix
    int vertices;
} graph;

// Function prototypes
void initGraph(graph *g, int vertices);
bool addEdge(graph *g, int u, int v);
void addVertex(graph*g);
void displayGraph(graph *g);
bool detectCycle(graph *g);

#endif
