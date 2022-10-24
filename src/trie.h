/** @file
 * Struktura trie i operacje na niej.
 *
 * @author Szymon Potrzebowski <sp438683@students.mimuw.edu.pl>
 * @date 2022
 */
#ifndef __TRIE_H__
#define __TRIE_H__

#include <stdlib.h>
#include "phone_forward.h"
#include "list.h"
#include "string_number.h"

#define FORWARD 0 /**< Flaga informująca że mamy doczynienia z trie forward */
#define REVERSE 1 /**< Flaga informująca że mamy doczynienia z trie reverse */

/**
 * @brief Struktura reprezentująca węzeł w trie.
 * Struktura reprezentująca węzeł w trie.
 */
typedef struct Trie
{
    List *fwdList;       /**< Przechowuje na co jest przekierowanie, w przypadku
                              reverse trie, co przekierowuje*/
    struct Trie *parent;           /**< Wskaźnik na ojca. Ojciec korzenia to
                                        NULL.*/
    struct Trie *children[DIGITS]; /**< Przechowuje synów według kolejnego
                                        znaku prefiksu. Jeśli nie ma
                                        przekierowania z kolejnym znakiem i
                                        to children[i] = NULL.*/
    struct Trie *reverseNode;      /**< Wskaźnik na odpowiadający node w reverse
                                        trie, w przypadku reverse trie - NULL.*/
    List *listNode;        /**< Wskaźnik na odpowiedni node w liście reverseNode
                           przechowujący przekierowany napis,
                                        w reverse trie - NULL.*/
    int realChildren;              /**< Liczba niepustych synów.*/
} Trie;

/**
 * @brief Tworzy nowego syna drzewa.
 * Jeżeli uda się zaalokować pamięć, tworzy nowego syna poddrzewa i zwiększa
 * @p realChildren w @p parent o jeden.
 * @param parent Ojciec nowego węzła.
 * @return Nowy syn drzewa. NULL, jeżeli nie udało się zaalokować pamięci.
 */
Trie *trieNewSon(Trie *parent);

/** @brief Usuwa gałąź pustych przekierowań.
 * Usuwa gałąź pustych przekierowań, które prowadziły do liścia @p pf.
 * Jeżeli @p pf nie jest liściem, niczego nie usunie.
 * @param pf Liść na gałęzi.
 */
void branchDelete(Trie *pf);

/**
 * @brief Funkcja usuwająca trie.
 * Funkcja zwalnia pamięć potrzebną do zapisania drzewa o korzeniu w @p root.
 * @param root korzeń drzewa.
 */
void trieDelete(Trie *root);

#endif