/** @file
 * Implementacja operacji na strukturze trie.
 *
 * @author Szymon Potrzebowski <sp438683@students.mimuw.edu.pl>
 * @date 2022
 */

#include "trie.h"

Trie *trieNewSon(Trie *parent)
{
    Trie *tmp = calloc(1, sizeof(Trie));
    if (tmp)
    {
        tmp->parent = parent;
        (parent->realChildren)++;
    }
    return tmp;
}

void branchDelete(Trie *pf)
{
    size_t i;
    Trie *tmp;
    while (pf && pf->parent && pf->realChildren == 0 && !pf->fwdList)
    {
        tmp = pf->parent;
        i = 0;
        while (tmp->children[i] != pf)
            i++;
        tmp->children[i] = NULL;
        tmp->realChildren--;
        free(pf);
        pf = tmp;
    }
}

/**
 * @brief Funkcja usuwająca trie.
 * Funkcja zwalnia pamięć potrzebną do zapisania drzewa o korzeniu w @p root.
 * @param root korzeń drzewa.
 */
void trieDelete(Trie *root)
{
    if (!root)
        return;
    Trie *tmp = root;
    Trie *tmp2;
    if (root)
        root->parent = NULL;
    size_t i;
    while (tmp)
    {
        if (tmp->realChildren == 0)
        {
            tmp2 = tmp;
            tmp = tmp->parent;
            deleteList(tmp2->fwdList, true);
            tmp2->fwdList = NULL;
            deleteNode(tmp2->listNode, true);
            tmp2->listNode = NULL;
            if (tmp2->reverseNode)
            {
                if (isEmpty(tmp2->reverseNode->fwdList))
                {
                    deleteList(tmp2->reverseNode->fwdList, true);
                    tmp2->reverseNode->fwdList = NULL;
                    branchDelete(tmp2->reverseNode);
                }
            }
            free(tmp2);
        }
        else
        {
            i = 0;
            while (!tmp->children[i])
                i++;
            (tmp->realChildren)--;
            tmp2 = tmp->children[i];
            tmp->children[i] = NULL;
            tmp = tmp2;
        }
    }
}