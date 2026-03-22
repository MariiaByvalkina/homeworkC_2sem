#include <stdio.h>
#pragma once

typedef struct Node Node;

typedef struct AVLtree AVLtree;

AVLtree* createTree();

void destroyTree(AVLtree* tree);

void addAirport(AVLtree* tree, char* iata, char* name);

void removeNode(AVLtree* tree, char* iata);

Node* find(Node* node, char* iata);

int getTreeSize(AVLtree* tree);

void saveTree(AVLtree* tree, const char* filename);

void readFromFile(AVLtree* tree, const char* filename);

char* getName(Node* node);

char* getIata(Node* node);

Node* getRoot(AVLtree* tree);