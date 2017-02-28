#include "rbt.output.darts.h"
using namespace darts;
using namespace std;
/*Function: KeyComp, ID: 42*/
int32 KeyComp(const uint32* a, const uint32* b, uint32 n)
{
    /*KeyComp:42*/
    /*CompoundStmt:3610*/
    uint32 i;
    for (i = 0; i < n; i++) {
        if (a[i] < b[i])
            return (-1);
        else if (a[i] > b[i])
            return (1);
    }
    return (0);
}
/*Function: TreeInsert, ID: 43*/
int32 TreeInsert(RBTree* tree, uint32* attrs)
{
    /*TreeInsert:43*/
    /*CompoundStmt:3628*/
    uint32 sl = 1;
    uint32* attrsP;
    int32 cmpres;
    treeNode *xNd, *yNd, *tmp;
    tmp = &tree->root;
    xNd = tmp->left;
    if (xNd == ((void*)0)) {
        tree->count++;
        tree->mp = (struct treeNode*)(tree->memPool + tree->memaddr);
        tree->memaddr += tree->treeNodeSize;
        (tree->freeNodeCounter)--;
        if (tree->freeNodeCounter == 0) {
            tree->memoryIsFull = 1;
        }
        xNd = tmp->left = tree->mp;
        memcpy(&(xNd->nodeMemPool[0]), &attrs[0], tree->nodeDataSize);
        xNd->left = xNd->right = ((void*)0);
        xNd->clr = BLACK;
        return 0;
    }
    tree->drcts[0] = 0;
    tree->nodes[0] = &tree->root;
    while (1) {
        attrsP = (uint32*)&(xNd->nodeMemPool[tree->nm]);
        cmpres = KeyComp(&attrs[tree->nm << 1], attrsP, tree->nd);
        if (cmpres < 0) {
            tree->nodes[sl] = xNd;
            tree->drcts[sl++] = 0;
            yNd = xNd->left;
            if (yNd == ((void*)0)) {
                tree->mp = (struct treeNode*)(tree->memPool + tree->memaddr);
                tree->memaddr += tree->treeNodeSize;
                (tree->freeNodeCounter)--;
                if (tree->freeNodeCounter == 0) {
                    tree->memoryIsFull = 1;
                }
                xNd = xNd->left = tree->mp;
                break;
            }
        } else if (cmpres > 0) {
            tree->nodes[sl] = xNd;
            tree->drcts[sl++] = 1;
            yNd = xNd->right;
            if (yNd == ((void*)0)) {
                tree->mp = (struct treeNode*)(tree->memPool + tree->memaddr);
                tree->memaddr += tree->treeNodeSize;
                (tree->freeNodeCounter)--;
                if (tree->freeNodeCounter == 0) {
                    tree->memoryIsFull = 1;
                }
                xNd = xNd->right = tree->mp;
                break;
            }
        } else {
            uint64 ii;
            int64* mx;
            mx = (int64*)&attrs[0];
            for (ii = 0; ii < tree->nm; ii++)
                xNd->nodeMemPool[ii] += mx[ii];
            return 0;
        }
        xNd = yNd;
    }
    tree->count++;
    memcpy(&(xNd->nodeMemPool[0]), &attrs[0], tree->nodeDataSize);
    xNd->left = xNd->right = ((void*)0);
    xNd->clr = RED;
    while (1) {
        if (tree->nodes[sl - 1]->clr != RED || sl < 3)
            break;
        if (tree->drcts[sl - 2] == 0) {
            yNd = tree->nodes[sl - 2]->right;
            if (yNd != ((void*)0) && yNd->clr == RED) {
                tree->nodes[sl - 1]->clr = BLACK;
                yNd->clr = BLACK;
                tree->nodes[sl - 2]->clr = RED;
                sl -= 2;
            } else {
                if (tree->drcts[sl - 1] == 1) {
                    xNd = tree->nodes[sl - 1];
                    yNd = xNd->right;
                    xNd->right = yNd->left;
                    yNd->left = xNd;
                    tree->nodes[sl - 2]->left = yNd;
                } else
                    yNd = tree->nodes[sl - 1];
                xNd = tree->nodes[sl - 2];
                xNd->clr = RED;
                yNd->clr = BLACK;
                xNd->left = yNd->right;
                yNd->right = xNd;
                if (tree->drcts[sl - 3])
                    tree->nodes[sl - 3]->right = yNd;
                else
                    tree->nodes[sl - 3]->left = yNd;
                break;
            }
        } else {
            yNd = tree->nodes[sl - 2]->left;
            if (yNd != ((void*)0) && yNd->clr == RED) {
                tree->nodes[sl - 1]->clr = BLACK;
                yNd->clr = BLACK;
                tree->nodes[sl - 2]->clr = RED;
                sl -= 2;
            } else {
                if (tree->drcts[sl - 1] == 0) {
                    xNd = tree->nodes[sl - 1];
                    yNd = xNd->left;
                    xNd->left = yNd->right;
                    yNd->right = xNd;
                    tree->nodes[sl - 2]->right = yNd;
                } else
                    yNd = tree->nodes[sl - 1];
                xNd = tree->nodes[sl - 2];
                xNd->clr = RED;
                yNd->clr = BLACK;
                xNd->right = yNd->left;
                yNd->left = xNd;
                if (tree->drcts[sl - 3])
                    tree->nodes[sl - 3]->right = yNd;
                else
                    tree->nodes[sl - 3]->left = yNd;
                break;
            }
        }
    }
    tree->root.left->clr = BLACK;
    return 0;
}
/*Function: WriteViewToDisk, ID: 44*/
int32 WriteViewToDisk(ADC_VIEW_CNTL* avp, treeNode* t)
{
    /*WriteViewToDisk:44*/
    /*CompoundStmt:3973*/
    uint32 i;
    if (!t)
        return 0;
    if (WriteViewToDisk(avp, t->left))
        return 1;
    for (i = 0; i < avp->nm; i++) {
        avp->mSums[i] += t->nodeMemPool[i];
    }
    if (fwrite(t->nodeMemPool, avp->outRecSize, 1, avp->viewFile) != 1) {
        fprintf(stderr, "\n Write error from WriteToFile()\n");
        return 1;
    }
    if (WriteViewToDisk(avp, t->right))
        return 1;
    return 0;
}
/*Function: WriteViewToDiskCS, ID: 45*/
int32 WriteViewToDiskCS(ADC_VIEW_CNTL* avp, treeNode* t, uint64* ordern)
{
    /*WriteViewToDiskCS:45*/
    /*CompoundStmt:4004*/
    uint32 i;
    if (!t)
        return 0;
    if (WriteViewToDiskCS(avp, t->left, ordern))
        return 1;
    for (i = 0; i < avp->nm; i++) {
        avp->mSums[i] += t->nodeMemPool[i];
        avp->checksums[i] += (++(*ordern)) * t->nodeMemPool[i] % measbound;
    }
    if (fwrite(t->nodeMemPool, avp->outRecSize, 1, avp->viewFile) != 1) {
        fprintf(stderr, "\n Write error from WriteToFile()\n");
        return 1;
    }
    if (WriteViewToDiskCS(avp, t->right, ordern))
        return 1;
    return 0;
}
/*Function: computeChecksum, ID: 46*/
int32 computeChecksum(ADC_VIEW_CNTL* avp, treeNode* t, uint64* ordern)
{
    /*computeChecksum:46*/
    /*CompoundStmt:4044*/
    uint32 i;
    if (!t)
        return 0;
    if (computeChecksum(avp, t->left, ordern))
        return 1;
    for (i = 0; i < avp->nm; i++) {
        avp->checksums[i] += (++(*ordern)) * t->nodeMemPool[i] % measbound;
    }
    if (computeChecksum(avp, t->right, ordern))
        return 1;
    return 0;
}
/*Function: WriteChunkToDisk, ID: 47*/
int32 WriteChunkToDisk(uint32 recordSize, FILE* fileOfChunks, treeNode* t, FILE* logFile)
{
    /*WriteChunkToDisk:47*/
    /*CompoundStmt:4074*/
    if (!t)
        return 0;
    if (WriteChunkToDisk(recordSize, fileOfChunks, t->left, logFile))
        return 1;
    if (fwrite(t->nodeMemPool, recordSize, 1, fileOfChunks) != 1) {
        fprintf(stderr, "\n Write error from WriteToFile()\n");
        return 1;
    }
    if (WriteChunkToDisk(recordSize, fileOfChunks, t->right, logFile))
        return 1;
    return 0;
}
/*Function: CreateEmptyTree, ID: 48*/
RBTree* CreateEmptyTree(uint32 nd, uint32 nm, uint32 memoryLimit, unsigned char* memPool)
{
    /*CreateEmptyTree:48*/
    /*CompoundStmt:4093*/
    RBTree* tree = (RBTree*)malloc(sizeof(RBTree));
    if (!tree)
        return ((void*)0);
    tree->root.left = ((void*)0);
    tree->root.right = ((void*)0);
    tree->count = 0;
    tree->memaddr = 0;
    tree->treeNodeSize = sizeof(struct treeNode) + 4 * (nd - 1) + 8 * nm;
    if (tree->treeNodeSize % 8 != 0)
        tree->treeNodeSize += 4;
    tree->memoryLimit = memoryLimit;
    tree->memoryIsFull = 0;
    tree->nodeDataSize = 4 * nd + 8 * nm;
    tree->mp = ((void*)0);
    tree->nNodesLimit = tree->memoryLimit / tree->treeNodeSize;
    tree->freeNodeCounter = tree->nNodesLimit;
    tree->nd = nd;
    tree->nm = nm;
    tree->memPool = memPool;
    tree->nodes = (treeNode**)malloc(sizeof(treeNode*) * 64);
    if (!(tree->nodes))
        return ((void*)0);
    tree->drcts = (uint32*)malloc(sizeof(uint32) * 64);
    if (!(tree->drcts))
        return ((void*)0);
    return tree;
}
/*Function: InitializeTree, ID: 49*/
void InitializeTree(RBTree* tree, uint32 nd, uint32 nm)
{
    /*InitializeTree:49*/
    /*CompoundStmt:4177*/
    tree->root.left = ((void*)0);
    tree->root.right = ((void*)0);
    tree->count = 0;
    tree->memaddr = 0;
    tree->treeNodeSize = sizeof(struct treeNode) + 4 * (nd - 1) + 8 * nm;
    if (tree->treeNodeSize % 8 != 0)
        tree->treeNodeSize += 4;
    tree->memoryIsFull = 0;
    tree->nodeDataSize = 4 * nd + 8 * nm;
    tree->mp = ((void*)0);
    tree->nNodesLimit = tree->memoryLimit / tree->treeNodeSize;
    tree->freeNodeCounter = tree->nNodesLimit;
    tree->nd = nd;
    tree->nm = nm;
}
/*Function: DestroyTree, ID: 50*/
int32 DestroyTree(RBTree* tree)
{
    /*DestroyTree:50*/
    /*CompoundStmt:4225*/
    if (tree == ((void*)0))
        return 3;
    if (tree->memPool != ((void*)0))
        free(tree->memPool);
    if (tree->nodes)
        free(tree->nodes);
    if (tree->drcts)
        free(tree->drcts);
    free(tree);
    return 0;
}
