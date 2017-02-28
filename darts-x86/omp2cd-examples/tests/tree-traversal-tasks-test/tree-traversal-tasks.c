#include "tree-traversal-tasks.h"

Node_t* createNode(int* countNodes)
{
    Node_t* node = (Node_t*)malloc(sizeof(Node_t));
    node->ID = *countNodes;
    *countNodes += 1;
    node->parent = (Node_t*)NULL;
    node->leftChild = (Node_t*)NULL;
    node->rightChild = (Node_t*)NULL;
    node->level = 0;
    node->numChildren = 0;
    node->distanceToRoot = 0;
    node->distanceToRootTask = 0;
    return node;
}

void constructGraph(Node_t* node, int maxNodes, int* countNodes)
{
    static int level = 0;
    level++;

    if (*countNodes >= maxNodes)
        return;

    int createLeftChild = (rand() % 100 - 50);
    if (createLeftChild > 0) {
        node->leftChild = createNode(countNodes);
        node->leftChild->level = level;
        node->leftChild->parent = node;

        int createRightChild = (rand() % 100 - 50);
        if (createRightChild > 0) {
            node->rightChild = createNode(countNodes);
            node->rightChild->level = node->leftChild->level;
            node->rightChild->parent = node;
            constructGraph(node->rightChild, maxNodes, countNodes);
        }

        constructGraph(node->leftChild, maxNodes, countNodes);
    } else {
        node->rightChild = createNode(countNodes);
        node->rightChild->level = level;
        node->rightChild->parent = node;
        constructGraph(node->rightChild, maxNodes, countNodes);
    }
}

void printSpaces(int numSpaces)
{
    for (int i = 0; i < numSpaces; i++)
        fprintf(stderr, " ");
}

void displayGraph(Node_t* node)
{
    static int num_errors = 0;

    printSpaces(node->level);
    fprintf(stderr, "Node %d, distance = %d:%d: \n", node->ID, node->distanceToRoot,
        node->distanceToRootTask);

    if (node->distanceToRoot != node->distanceToRootTask)
        num_errors++;

    if (node->leftChild) {
        printSpaces(node->level);
        fprintf(stderr, "leftChild %d ", node->leftChild->ID);
    }
    if (node->rightChild) {
        if (node->leftChild == NULL)
            printSpaces(node->level);
        fprintf(stderr, "rightChild %d", node->rightChild->ID);
    }
    fprintf(stderr, "\n");

    if (node->leftChild) {
        displayGraph(node->leftChild);
    }
    if (node->rightChild) {
        displayGraph(node->rightChild);
    }

    if (node->parent == (Node_t*)NULL && num_errors)
        fprintf(stderr, "Errors = %d\n", num_errors);
}

void deleteGraph(Node_t* node)
{
    if (node->leftChild)
        deleteGraph(node->leftChild);
    if (node->rightChild)
        deleteGraph(node->rightChild);
    free(node);
}

void getDistanceToRoot(Node_t* node)
{
    if (node->parent)
        node->distanceToRoot = node->parent->distanceToRoot + 1;

    if (node->leftChild)
        getDistanceToRoot(node->leftChild);
    if (node->rightChild)
        getDistanceToRoot(node->rightChild);
}

void getDistanceToRootTasks(Node_t* node)
{
    if (node->parent)
        node->distanceToRootTask = node->parent->distanceToRootTask + 1;

    if (node->leftChild) {
#pragma omp task
        {
            getDistanceToRootTasks(node->leftChild);

            // #pragma omp taskwait

            (void)node;
        }
    }

#pragma omp taskwait

    if (node->rightChild) {
#pragma omp task
        getDistanceToRootTasks(node->rightChild);
    }

    // #pragma omp barrier
}

int main(int argc, char** argv)
{
    srand(time((time_t*)NULL));

    int maxNodes = 10;
    if (argc > 1)
        maxNodes = atoi(argv[1]);

    int countNodes = 0;

    Node_t* root = createNode(&countNodes);
    root->level = 0;
    constructGraph(root, maxNodes, &countNodes);
    getDistanceToRoot(root);

#pragma omp parallel
    {
#pragma omp single nowait
        {
            getDistanceToRootTasks(root);
            (void)root;
        }
    }
    displayGraph(root);
    deleteGraph(root);

    return 0;
}
