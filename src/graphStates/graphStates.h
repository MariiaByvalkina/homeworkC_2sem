#pragma once

typedef struct Graph Graph;
typedef struct priorityQueue priorityQueue;

Graph* createGraph();
void addEdge(Graph* graph, int start, int finish, int dist);
void freeGraph(Graph* graph);
priorityQueue* createQueue(int size);
char* modifiedDijkstra(Graph* graph, int* capitals, int n, int k);
void readFile(const char* filename);