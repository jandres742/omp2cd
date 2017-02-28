#ifndef __DARTS_
#include <omp.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Node_st Node_t;

struct Node_st {
    int ID;
    int level;
    Node_t* parent;
    Node_t* leftChild;
    Node_t* rightChild;
    int numChildren;
    int distanceToRoot;
    int distanceToRootTask;
};

void constructGraph(Node_t* node, int maxNodes, int* countNodes);
void printSpaces(int numSpaces);
void displayGraph(Node_t* node);
void deleteGraph(Node_t* node);
void getDistanceToRoot(Node_t* node);
void getDistanceToRootTasks(Node_t* node);
int main(int argc, char** argv);