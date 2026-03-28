#include "graphStates.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void testOneCity()
{
    Graph* graph = createGraph();

    int capitals[] = { 1 };

    char* res = modifiedDijkstra(graph, capitals, 1, 1);
    assert(strcmp(res, "Государство 1: 1\n") == 0);

    free(res);
    freeGraph(graph);
}

void testTwoCapitals()
{
    Graph* graph = createGraph();
    int capitals[] = { 1, 2 };

    addEdge(graph, 1, 2, 10);
    addEdge(graph, 2, 1, 10);

    char* res = modifiedDijkstra(graph, capitals, 2, 2);

    assert(strcmp(res, "Госудаство 1: 1\nГосудаство 2: 2\n") == 0);

    free(graph);
    freeGraph(graph);
}

void testTwoCityOneCapital()
{
    Graph* graph = createGraph();

    int capitals[] = { 1 };

    addEdge(graph, 1, 2, 5);
    addEdge(graph, 2, 1, 5);

    char* res = modifiedDijkstra(graph, capitals, 2, 1);

    assert(strcmp(res, "Госудаство 1: 1 2\n"));

    free(graph);
    freeGraph(graph);
}

void testLineCities()
{
    Graph* graph = createGraph();
    int capitals[] = { 1, 3 };

    addEdge(graph, 1, 2, 5);
    addEdge(graph, 2, 1, 5);
    addEdge(graph, 2, 3, 3);
    addEdge(graph, 3, 2, 3);

    char* result = modifiedDijkstra(graph, capitals, 3, 2);

    assert(strcmp(result, "Государство 1: 1 2\nГосударство 2: 3\n") == 0);

    free(result);
    freeGraph(graph);
}

void testEqualDistance()
{
    Graph* graph = createGraph();
    int capitals[] = { 1, 3 };

    addEdge(graph, 1, 2, 5);
    addEdge(graph, 2, 1, 5);
    addEdge(graph, 2, 3, 5);
    addEdge(graph, 3, 2, 5);

    char* result = modifiedDijkstra(graph, capitals, 3, 2);

    assert(strcmp(result, "Государство 1: 1 2\nГосударство 2: 3\n") == 0);

    free(result);
    freeGraph(graph);
}