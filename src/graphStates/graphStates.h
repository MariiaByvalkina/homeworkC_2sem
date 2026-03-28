#pragma once

typedef struct Graph Graph;
typedef struct PriorityQueue PriorityQueue;

Graph* createGraph(int n);
void addEdge(Graph* graph, int start, int finish, int dist);
void freeGraph(Graph* graph);
PriorityQueue* createQueue(int size);
char* modifiedDijkstra(Graph* graph, int* capitals, int n, int k);
void readFile(const char* filename);
