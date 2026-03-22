#include "AVL.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

void assertAirport(AVLtree* tree, char* iata, char* name)
{
    Node* root = getRoot(tree);
    Node* node = find(root, iata);
    assert(node != NULL);
    assert(strcmp(getName(node), name) == 0);
}

void testBasic(void)
{
    AVLtree* tree = createTree();

    addAirport(tree, "SVO", "Sheremetyevo");
    addAirport(tree, "DME", "Domodedovo");
    addAirport(tree, "LED", "Pulkovo");

    assert(getTreeSize(tree) == 3);
    assertAirport(tree, "SVO", "Sheremetyevo");
    assertAirport(tree, "LED", "Pulkovo");

    Node* root = getRoot(tree);
    assert(find(root, "JFK") == NULL);

    destroyTree(tree);
}

void testBalanceRotations(void)
{
    AVLtree* tree = createTree();
    addAirport(tree, "CCC", "C");
    addAirport(tree, "BBB", "B");
    addAirport(tree, "AAA", "A");
    assert(getTreeSize(tree) == 3);
    assertAirport(tree, "BBB", "B");

    addAirport(tree, "XXX", "X");
    addAirport(tree, "YYY", "Y");
    addAirport(tree, "ZZZ", "Z");
    assert(getTreeSize(tree) == 6);
    destroyTree(tree);
}

void testDuplicates(void)
{

    AVLtree* tree = createTree();

    addAirport(tree, "JFK", "John F. Kennedy");
    int sizeBefore = getTreeSize(tree);

    addAirport(tree, "JFK", "Kennedy New Name");

    assert(getTreeSize(tree) == sizeBefore);
    assertAirport(tree, "JFK", "Kennedy New Name");

    destroyTree(tree);
}

void testDeletion(void)
{
    AVLtree* tree = createTree();

    addAirport(tree, "B", "Airport B");
    addAirport(tree, "A", "Airport A");
    addAirport(tree, "D", "Airport D");
    addAirport(tree, "C", "Airport C");
    addAirport(tree, "E", "Airport E");

    removeNode(tree, "A");
    assert(getTreeSize(tree) == 4);
    Node* root = getRoot(tree);
    assert(find(root, "A") == NULL);

    removeNode(tree, "D");
    assert(getTreeSize(tree) == 3);
    assert(find(root, "D") == NULL);
    assertAirport(tree, "C", "Airport C");
    addAirport(tree, "F", "Airport F");
    removeNode(tree, "B");
    assert(getTreeSize(tree) == 3);
    assert(find(root, "B") == NULL);

    destroyTree(tree);
}

void testLargeData(void)
{
    AVLtree* tree = createTree();
    char code[10];

    for (int i = 0; i < 1000; i++) {
        sprintf(code, "%03d", i);
        addAirport(tree, code, "Test Airport");
    }

    assert(getTreeSize(tree) == 1000);
    assertAirport(tree, "500", "Test Airport");

    for (int i = 0; i < 500; i++) {
        sprintf(code, "%03d", i);
        removeNode(tree, code);
    }

    assert(getTreeSize(tree) == 500);
    Node* root = getRoot(tree);
    assert(find(root, "000") == NULL);
    assertAirport(tree, "999", "Test Airport");

    destroyTree(tree);
}

int main(void)
{

    testBasic();
    testBalanceRotations();
    testDuplicates();
    testDeletion();
    testLargeData();
    return 0;
}
