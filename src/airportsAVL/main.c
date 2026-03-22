#include "AVL.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv)
{
    if (argc < 2) {
        return 1;
    }

    AVLtree* tree = createTree();
    readFromFile(tree, argv[1]);

    char cmd[16], buffer[512];
    while (printf("> ") && scanf("%s", cmd) != EOF) {

        if (!strcmp(cmd, "find")) {
            scanf("%s", buffer);
            Node* root = getRoot(tree);
            Node* res = find(root, buffer);

            if (res) {
                printf("%s → %s\n", getIata(res), getName(res));
            } else {
                printf("Аэропорт '%s' не найден в базе.\n", buffer);
            }

        } else if (!strcmp(cmd, "add")) {
            scanf(" %[^\n]", buffer);
            char* iata = strtok(buffer, ":");
            char* name = strtok(NULL, "");

            if (iata && name) {
                addAirport(tree, iata, name);
                printf("Аэропорт '%s' добавлен в базу.\n", iata);
            }
        } else if (!strcmp(cmd, "delete")) {
            scanf("%s", buffer);
            removeNode(tree, buffer);
            printf("Аэропорт '%s' удален из базы.\n", buffer);

        } else if (!strcmp(cmd, "save")) {
            saveTree(tree, argv[1]);
            printf("База сохранена: %d аэропортов.\n", getTreeSize(tree));

        } else if (!strcmp(cmd, "quit")) {
            break;
        }
    }

    destroyTree(tree);
    return 0;
}
