#include "include/DFG/DFG.h"
#include "include/DFG/DFGNode.h"
#include "include/DFG/TPRegion.h"

using namespace llvm;
using namespace clang;
using namespace clang::tooling;
using namespace std;

namespace omp2cd_space {

size_t NodeList::size() { return this->length; }

DFGNode* NodeList::front() { return this->head; }

DFGNode* NodeList::back() { return this->tail; }

void NodeList::replace(DFGNode* oldNode, DFGNode* newNode)
{
    newNode->prev = oldNode->prev;
    if (oldNode->prev)
        oldNode->prev->next = newNode;

    newNode->next = oldNode->next;
    if (oldNode->next)
        oldNode->next->prev = newNode;

    newNode->parent = oldNode->parent;

    if (this->head == oldNode)
        this->head = newNode;

    if (this->tail == oldNode)
        this->tail = newNode;
}

void NodeList::insertBefore(DFGNode* node, DFGNode* newNode)
{
    newNode->prev = node->prev;
    if (node->prev)
        node->prev->next = newNode;

    newNode->next = node;

    node->prev = newNode;

    if (this->head == node)
        this->head = newNode;

    newNode->parent = node->parent;

    this->length += 1;
}

void NodeList::insertAfter(DFGNode* node, DFGNode* newNode)
{
    newNode->next = node->next;
    if (node->next)
        node->next->prev = newNode;

    newNode->prev = node;

    node->next = newNode;

    if (this->tail == node)
        this->tail = newNode;

    newNode->parent = node->parent;

    this->length += 1;
}

void NodeList::insert(DFGNode* newNode)
{
    if (this->head == nullptr)
        this->head = newNode;

    if (this->tail == nullptr) {
        this->tail = newNode;
    } else {
        this->tail->next = newNode;
        newNode->prev = this->tail;
        this->tail = newNode;
    }
    this->length += 1;
}

void NodeList::swap(DFGNode* oldNode, DFGNode* newNode)
{
    if (oldNode->prev)
        oldNode->prev->next = newNode;

    newNode->prev = oldNode->prev;
    oldNode->prev = nullptr;

    if (oldNode->next)
        oldNode->next->prev = newNode;

    newNode->next = oldNode->next;
    oldNode->next = nullptr;

    if (this->head == oldNode)
        this->head = newNode;

    if (this->tail == oldNode)
        this->tail = newNode;
}

void NodeList::remove(DFGNode* node)
{
    if (node->prev)
        node->prev->next = node->next;

    if (node->next)
        node->next->prev = node->prev;

    if (node == this->head)
        this->head = node->next;

    if (node == this->tail)
        this->tail = node->prev;

    this->length -= 1;
}

void NodeList::clear()
{
    DFGNode* childNode = this->tail;
    while (childNode) {
        DFGNode* tempNode = childNode->prev;
        this->remove(childNode);
        delete (childNode);
        childNode = tempNode;
    }
}
}
