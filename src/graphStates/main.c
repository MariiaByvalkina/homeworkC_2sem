#include "graphStates.h"
#include <stdio.h>
#include <stdlib.h>

void readFile(const char* filename)
{
    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        printf("Ошибка чтения файла");
        return;
    }

    int n = 0;
    int m = 0;

    fscanf(file, "%d %d", &n, &m);

    Graph* graph = createGraph(n);

    for (int p = 0; p < m; p++) {
        int i = 0;
        int j = 0;
        int len = 0;
        fscanf(file, "%d %d %d", &i, &j, &len);
        addEdge(graph, i, j, len);
    }

    int k = 0;
    fscanf(file, "%d", &k);

    int* capitals = malloc(k * sizeof(int));

    for (int i = 0; i < k; i++) {
        fscanf(file, "%d", &capitals[i]);
    }

    fclose(file);

    char* res = modifiedDijkstra(graph, capitals, n, k);
    printf("%s", res);

    freeGraph(graph);
    free(capitals);
}

int main(void)
{
    readFile("input.txt");
    return 0;
}