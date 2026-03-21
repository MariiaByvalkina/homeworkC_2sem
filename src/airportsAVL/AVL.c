#include "AVL.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char iata[5];
    char* name;
    struct Node* rightChild;
    struct Node* leftChild;
    int balance;
} Node;

typedef struct AVLtree {
    Node* root;
    int size;
} AVLtree;

AVLtree* createTree()
{
    return calloc(1, sizeof(AVLtree));
}

int nodeHeight(Node* node)
{
    if (node == NULL) {
        return 0;
    }

    int left = nodeHeight(node->leftChild);
    int right = nodeHeight(node->rightChild);
    return (left > right ? left : right) + 1;
}

void updateBalance(Node* node)
{
    if (node != NULL) {
        node->balance = nodeHeight(node->rightChild) - nodeHeight(node->leftChild);
    }
}
Node* rotateLeft(Node* a)
{
    Node* b = a->rightChild;
    Node* c = b->leftChild;
    b->leftChild = a;
    a->rightChild = c;

    updateBalance(a);
    updateBalance(b);

    return b;
}

Node* rotateRight(Node* a)
{
    Node* b = a->leftChild;
    Node* c = b->rightChild;
    b->rightChild = a;
    a->leftChild = c;

    updateBalance(a);
    updateBalance(b);

    return b;
}

Node* bigRotateLeft(Node* a)
{
    a->leftChild = rotateLeft(a->leftChild);
    return rotateRight(a);
}

Node* bigRotateRight(Node* a)
{
    a->rightChild = rotateRight(a->rightChild);
    return rotateLeft(a);
}

Node* balance(Node* node)
{
    updateBalance(node);

    if (node->balance == 2) {
        updateBalance(node->rightChild);
        if (node->rightChild->balance >= 0)
            return rotateLeft(node);
        return bigRotateLeft(node);
    }

    if (node->balance == -2) {
        updateBalance(node->leftChild);
        if (node->leftChild->balance <= 0)
            return rotateRight(node);
        return bigRotateRight(node);
    }
    return node;
}

Node* insert(Node* node, char* iata, char* name, int* size)
{
    if (node == NULL) {
        Node* newNode = calloc(1, sizeof(Node));
        strcpy(newNode->iata, iata);
        newNode->name = strdup(name);
        (*size)++;
        return newNode;
    }

    int cmp = strcmp(iata, node->iata);
    if (cmp < 0) {
        node->leftChild = insert(node->leftChild, iata, name, size);
    } else if (cmp > 0) {
        node->rightChild = insert(node->rightChild, iata, name, size);
    } else {
        free(node->name);
        node->name = strdup(name);
        return node;
    }

    updateBalance(node);
    return balance(node);
}

Node* find(Node* node, char* iata)
{
    if (node == NULL) {
        return NULL;
    }

    int cmp = strcmp(iata, node->iata);

    if (cmp < 0) {
        return find(node->leftChild, iata);
    } else if (cmp > 0) {
        return find(node->rightChild, iata);
    } else {
        return node;
    }
}

Node* findMin(Node* node)
{
    Node* current = node;
    while (current && current->leftChild) {
        current = current->leftChild;
    }
    return current;
}

Node* deleteNode(Node* node, char* iata, int* size)
{
    if (node == NULL) {
        return NULL;
    }

    int cmp = strcmp(iata, node->iata);

    if (cmp < 0) {
        node->leftChild = deleteNode(node->leftChild, iata, size);
    } else if (cmp > 0) {
        node->rightChild = deleteNode(node->rightChild, iata, size);
    } else {
        if (node->leftChild == NULL || node->rightChild == NULL) {
            Node* temp = node->leftChild ? node->leftChild : node->rightChild;

            if (temp == NULL) {
                free(node->name);
                free(node);
                (*size)--;
                return NULL;
            } else {
                *node = *temp;
                free(temp);
                (*size)--;
            }
        } else {
            Node* temp = findMin(node->rightChild);
            strcpy(node->iata, temp->iata);
            free(node->name);
            node->name = strdup(temp->name);
            node->rightChild = deleteNode(node->rightChild, temp->iata, size);
        }
    }

    if (node == NULL) {
        return NULL;
    }

    updateBalance(node);
    return balance(node);
}

void saveToFile(Node* node, FILE* file)
{
    if (node == NULL) {
        return;
    }

    saveToFile(node->leftChild, file);
    fprintf(file, "%s:%s\n", node->iata, node->name);
    saveToFile(node->rightChild, file);
}

void freeTree(Node* node)
{
    if (node == NULL) {
        return;
    }

    freeTree(node->leftChild);
    freeTree(node->rightChild);
    free(node->name);
    free(node);
}

void readFromFile(AVLtree* tree, const char* filename)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Ошибка чтения файла");
        return;
    }

    char line[512];

    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = '\0';

        char* iata = strtok(line, ":");
        char* name = strtok(NULL, "");
        if (iata && name) {
            tree->root = insert(tree->root, iata, name, &tree->size);
        }
    }

    fclose(file);
    printf("Загружено %d аэропортов. Система готова к работе.\n", tree->size);
}