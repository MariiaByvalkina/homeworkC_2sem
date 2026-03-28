#include "graphStates.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Edge {
    int finish;
    int dist;
    struct Edge* next;
} Edge;

typedef struct Graph {
    struct Edge** lists;
    int size;
} Graph;

typedef struct queueNode {
    int vertex;
    int dist;
    int state;
} queueNode;

typedef struct priorityQueue {
    queueNode* elements;
    int size;
    int capacity;
} priorityQueue;

Graph* createGraph(int n)
{
    Graph* graph = malloc(sizeof(Graph));
    graph->size = n + 1;
    graph->lists = calloc(n + 1, sizeof(Edge*));
    return graph;
}

void addEdge(Graph* graph, int start, int finish, int dist)
{
    Edge* newEdge = malloc(sizeof(Edge));
    newEdge->finish = finish;
    newEdge->dist = dist;
    newEdge->next = graph->lists[start];

    graph->lists[start] = newEdge;
}

void freeGraph(Graph* graph)
{
    for (int i = 0; i < graph->size; i++) {
        Edge* current = graph->lists[i];
        while (current) {
            Edge* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(graph->lists);
    free(graph);
}
priorityQueue* createQueue(int capacity)
{
    priorityQueue* queue = malloc(sizeof(priorityQueue));
    queue->size = 0;
    queue->capacity = capacity;
    queue->elements = malloc(capacity * sizeof(queueNode));
    return queue;
}

void freeQueue(priorityQueue* queue)
{
    free(queue->elements);
    free(queue);
}

int getSize(priorityQueue* queue)
{
    return queue->size;
}

bool isEmpty(priorityQueue* queue)
{
    return (queue->size == 0);
}

void heapifyUp(priorityQueue* queue, int index)
{
    while (index > 0) {
        int parent = (index - 1) / 2;

        if (queue->elements[index].dist < queue->elements[parent].dist) {
            queueNode temp = queue->elements[parent];
            queue->elements[parent] = queue->elements[index];
            queue->elements[index] = temp;

            index = parent;
        } else {
            break;
        }
    }
}

void push(priorityQueue* queue, int vertex, int dist, int state)
{
    int size = queue->size;

    queue->elements[size].vertex = vertex;
    queue->elements[size].dist = dist;
    queue->elements[size].state = state;

    heapifyUp(queue, size);

    queue->size++;
}

void heapifyDown(priorityQueue* queue, int index)
{
    while (1) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int small = index;

        if (left < queue->size && queue->elements[left].dist < queue->elements[small].dist) {
            small = left;
        }

        if (left < queue->size && queue->elements[right].dist < queue->elements[small].dist) {
            small = right;
        }

        if (small != index) {
            queueNode temp = queue->elements[small];
            queue->elements[small] = queue->elements[index];
            queue->elements[index] = temp;

            index = small;

        }

        else {
            break;
        }
    }
}

queueNode pop(priorityQueue* queue)
{
    queueNode upper = queue->elements[0];

    queue->size--;
    queue->elements[0] = queue->elements[queue->size];

    heapifyDown(queue, 0);

    return upper;
}

char* modifiedDijkstra(Graph* graph, int* capitals, int n, int k)
{
    priorityQueue* queue = createQueue(graph->size);

    int* state = calloc(n + 1, sizeof(int));
    int* dist = malloc(sizeof(int) * (n + 1));

    for (int i = 0; i <= n; i++) {
        dist[i] = 100;
    }

    for (int i = 0; i < k; i++) {
        dist[capitals[i]] = 0;
        push(queue, capitals[i], 0, i + 1);
    }

    while (!isEmpty(queue)) {
        queueNode current = pop(queue);

        if (current.dist > dist[current.vertex]) {
            continue;
        }

        if (state[current.vertex] == 0) {
            state[current.vertex] = current.state;

            Edge* edge = graph->lists[current.vertex];
            while (edge) {
                int neigh = edge->finish;
                int newDist = current.dist + edge->dist;

                if (state[neigh] == 0 && (newDist < dist[neigh])) {
                    dist[neigh] = newDist;
                    push(queue, neigh, newDist, current.state);
                }
                edge = edge->next;
            }
        }
    }

    char* res = malloc(100 * sizeof(char));
    res[0] = 0;

    for (int i = 1; i <= k; i++) {
        sprintf(res + strlen(res), "Государство %d:", i);
        for (int j = 1; j <= n; j++) {
            if (state[j] == i) {
                sprintf(res + strlen(res), " %d", j);
            }
        }
        strcat(res, "\n");
    }

    free(state);
    free(dist);
    freeQueue(queue);

    return res;
}