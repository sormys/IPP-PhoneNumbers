/** @file
 * Implementacja operacji na przekierowaniach numerów.
 *
 * @author Szymon Potrzebowski <sp438683@students.mimuw.edu.pl>
 * @date 2022
 */

#include "phone_forward.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arrayUtils.h"
#include "list.h"
#include "string_number.h"
#include "trie.h"

/**
 * To jest struktura przechowująca przekierowania numerów telefonów.
 */
typedef struct PhoneForward {
    Trie *fwdRoot; /**< Wskaźnik na korzeń drzewa przekierowań*/
    Trie *revRoot; /**< Wskaźnik na korzeń drzewa odwrotności przekierowań*/
} PhoneForward;

/**
 * To jest struktura przechowująca przekierowania numerów telefonów.
 */
struct PhoneNumbers {
    char **numbers;  /**< Przechowuje wskaźniki na przechowywane numery.*/
    size_t quantity; /**< Liczba przechowywanych numerów.*/
};

/**
 * @brief Tworzy pusty obiekt struktury PhoneNumbers.
 * Tworzy pusty obiekt struktury PhoneNumbers.
 * @return Wskaźnik na pusty obiekt struktury PhoneNumbers.
 * NULL jeżeli nie udało się zaalokować pamięci.
 */
static PhoneNumbers *phnumNew(void) { return calloc(1, sizeof(PhoneNumbers)); }

PhoneForward *phfwdNew(void) {
    PhoneForward *tmp;
    if ((tmp = malloc(sizeof(PhoneForward)))) {
        if ((tmp->fwdRoot = calloc(1, sizeof(Trie)))) {
            if ((tmp->revRoot = calloc(1, sizeof(Trie)))) return tmp;
            free(tmp->fwdRoot);
        }
        free(tmp);
        return NULL;
    }
    return NULL;
}

/**
 * @brief Dodaje przekierowanie w konkretnym rodzaju trie.
 * Dodaje przekierowanie w konkretnym rodzaju trie. Jeżeli jest to drzewo
 * przekierowań po zaalokowaniu potrzebnej pamięci doda odpowiednie
 * przekierowanie w drzewie reverse. Dla drzewa typu reverse doda jedynie
 * przekierwanie z @p num1 na @p num2 .
 *
 * @param pf Przechowuje trie, do których chcemy dodać przekierowanie.
 * @param num1 Prefix przekierowywany.
 * @param num2 Numer na jaki ma zostać przekierowany prefix @p num1 .
 * @param node Węzeł na który zostanie zapisany węzeł trie z przekierwaniem.
 * @param flag Informacja do którego trie chcemy dodać przekierowanie.
 * @return Węzeł listy zawierający przekierowany numer. NULL jeżeli zabraknie
 * pamięci.
 */
static List *fwdAdd(PhoneForward *pf, char const *num1, char const *num2,
                    Trie **node, int flag) {
    size_t i = 0;
    size_t created = 0;  // Ile utworzyliśmy węzłów trie.
    size_t num1Len = stringLength(num1);
    size_t num2Len = stringLength(num2);
    *node = NULL;
    char *newFwd = NULL;
    if (!pf || !num2Len || !num1Len ||
        (num2Len == num1Len && !stringDiff(num1, num2)))
        return NULL;
    Trie *pfRoot = (flag == FORWARD) ? pf->fwdRoot : pf->revRoot;
    while (num1[i] != '\0' && pfRoot) {
        if (!pfRoot->children[getIndex(num1[i])]) {
            if (!(pfRoot->children[getIndex(num1[i])] = trieNewSon(pfRoot))) {
                branchDelete(pfRoot);
                return NULL;
            }
            if (pfRoot->children[getIndex(num1[i])]) created++;
        }
        pfRoot = pfRoot->children[getIndex(num1[i++])];
    }
    newFwd = malloc(sizeof(char) * (num2Len + 1));
    List *newFwdNode = NULL;
    if (num1[i] != '\0' || !pfRoot || !newFwd ||
        (!pfRoot->fwdList && !(pfRoot->fwdList = listNew())) ||
        // Tworzymy miejsce w liście na nowe przekierowanie.
        (!(newFwdNode = addString(pfRoot->fwdList, newFwd)))) {
        free(newFwd);
        if (pfRoot && isEmpty(pfRoot->fwdList)) {
            // Musimy usunąć stworzoną pustą listę.
            deleteList(pfRoot->fwdList, true);
            pfRoot->fwdList = NULL;
        }
        if (created > 0) branchDelete(pfRoot);
        return NULL;
    }
    List *listNode = NULL;
    if (flag == FORWARD) {
        listNode = fwdAdd(pf, num2, num1, &(pfRoot->reverseNode), REVERSE);
        if (!pfRoot->reverseNode) {
            deleteNode(newFwdNode, false);
            free(newFwd);
            if (isEmpty(pfRoot->fwdList)) {
                deleteList(pfRoot->fwdList, true);
                pfRoot->fwdList = NULL;
            }
            if (created > 0) branchDelete(pfRoot);
            return NULL;
        }
        if (listLength(pfRoot->fwdList) > 1) {
            // Usuwamy stare przekierwoanie.
            deleteString(pfRoot->fwdList, getFromList(pfRoot->fwdList, 1));
            deleteNode(pfRoot->listNode, true);
        }
    }
    pfRoot->listNode = listNode;
    for (i = 0; i < num2Len + 1; i++) newFwd[i] = num2[i];
    *node = pfRoot;
    return pfRoot->fwdList->next;
}

bool phfwdAdd(PhoneForward *pf, char const *num1, char const *num2) {
    Trie *tmp;
    return fwdAdd(pf, num1, num2, &tmp, FORWARD) == NULL ? false : true;
}

void phfwdDelete(PhoneForward *pf) {
    if (!pf) return;
    trieDelete(pf->fwdRoot);
    trieDelete(pf->revRoot);
    free(pf);
}

void phfwdRemove(PhoneForward *pf, char const *num) {
    size_t numLength = stringLength(num);
    if (!pf || !pf->fwdRoot || !numLength) return;
    Trie *pfRoot = pf->fwdRoot;
    size_t i = 0;
    Trie *tmp = NULL;
    while (i < numLength - 1 && pfRoot) {
        pfRoot = pfRoot->children[getIndex(num[i++])];
    }
    if (pfRoot) {
        if (pfRoot->children[getIndex(num[i])]) {
            pfRoot->realChildren--;
            tmp = pfRoot;
            pfRoot = pfRoot->children[getIndex(num[i])];
            tmp->children[getIndex(num[i])] = NULL;
        } else
            // Nie ma takiego przekierwania.
            pfRoot = NULL;
    }
    trieDelete(pfRoot);
    branchDelete(tmp);
}

void phnumDelete(PhoneNumbers *pnum) {
    if (pnum) {
        for (size_t i = 0; i < pnum->quantity; i++) free(pnum->numbers[i]);
        free(pnum->numbers);
        free(pnum);
    }
}

PhoneNumbers *phfwdGet(PhoneForward const *pf, char const *num) {
    size_t firstLength = stringLength(num);
    if (!pf || !pf->fwdRoot) return NULL;
    Trie *pfRoot = pf->fwdRoot;
    PhoneNumbers *result = phnumNew();
    if (!result || !firstLength) return result;
    size_t bestPrefixLength = 0;  // Długość najdłuższego prefiksu
                                  // z przekierowaniem.
    size_t prefixLength = 0;      // Długość obecnego prefiksu.
    size_t fwdLength = 0;         // Długość przekierowania prefiksu.
    char *newPrefix = NULL;
    if (!(result->numbers = malloc(sizeof(char *)))) {
        phnumDelete(result);
        return NULL;
    }
    result->quantity = 1;
    while (num[prefixLength] != '\0' && pfRoot &&
           (pfRoot->children[getIndex(num[prefixLength])])) {
        // Znajdujemy najdłuższy prefiks, który ma przekierowanie.
        pfRoot = pfRoot->children[getIndex(num[prefixLength++])];
        if (pfRoot->fwdList) {
            // Jeżeli ma przekierwanie to obecnie najlepszy.
            bestPrefixLength = prefixLength;
            newPrefix = getFromList(pfRoot->fwdList, 0);
        }
    }
    if (!newPrefix) {
        // Nie znaleźliśmy żadnego prefiksu z przekierowaniem.
        result->numbers[0] = copyString(num, firstLength);
        if (!result->numbers[0]) {
            phnumDelete(result);
            return NULL;
        }
        return result;
    }
    fwdLength = stringLength(newPrefix);
    size_t newLength = firstLength - bestPrefixLength + fwdLength + 1;
    // Długość napisu po przekierowaniu (ze znakiem '\0').
    result->numbers[0] = malloc(sizeof(char) * newLength);
    if (!result->numbers[0]) {
        phnumDelete(result);
        return NULL;
    } else {
        forwardNumber(result->numbers[0], newPrefix, fwdLength, num,
                      bestPrefixLength, firstLength);
        return result;
    }
}

/**
 * @brief Tworzy i dodaje przekierowania numery z prefiksami z listy.
 * Funkcja dostaje listę prefiksów @p listToAdd z których mogły być
 * przekierowane pierwsze @p prefixLength znaków numeru @p num.
 * @param list Lista z której dodajemy elementy.
 * @param listToAdd Lista do której będziemy dodawać elementy.
 * @param num Numer który przekierowujemy.
 * @param numLen Długość @p num.
 * @param prefixLength Długość przekierowywanego prefiksu @p num.
 * @return Listę z numerami które tworzą podane w @p list prefiksy w połączeniu
 * z ostatnimi @p numLen - @p prefixLength znakami @p num.
 */
static List *addForwards(List *list, List *listToAdd, const char *num,
                         size_t numLen, size_t prefixLength) {
    size_t reverseLen;
    char *reverseNumber;
    while (list->string) {
        reverseLen = stringLength(list->string);
        reverseNumber =
            malloc((numLen - prefixLength + reverseLen + 1) * sizeof(char));
        if (!reverseNumber || !addString(listToAdd, reverseNumber)) {
            if (reverseNumber) free(reverseNumber);
            deleteList(listToAdd, true);
            return NULL;
        }
        forwardNumber(reverseNumber, list->string, reverseLen, num,
                      prefixLength, numLen);
        list = list->next;
    }
    return list;
}

PhoneNumbers *phfwdReverse(PhoneForward const *pf, char const *num) {
    if (!pf || !pf->revRoot) return NULL;
    size_t numLen = stringLength(num);
    if (numLen == 0) return calloc(1, sizeof(PhoneNumbers));
    size_t prefixLength = 0;
    Trie *pfRoot = pf->revRoot;
    List *possible = listNew();
    if (!possible) return NULL;
    char *copyOfNum = copyString(num, numLen);
    if (!addString(possible, copyOfNum)) {
        free(possible);
        free(copyOfNum);
        return NULL;
    }
    while (num[prefixLength] != '\0' && pfRoot &&
           (pfRoot->children[getIndex(num[prefixLength])])) {
        pfRoot = pfRoot->children[getIndex(num[prefixLength++])];
        if (pfRoot->fwdList) {
            if (!addForwards(pfRoot->fwdList->next, possible, num, numLen,
                             prefixLength))
                return NULL;
        }
    }
    size_t numberCount = listLength(possible);
    char **numbers = malloc(numberCount * sizeof(char *));
    if (!numbers) {
        deleteList(possible, true);
        return NULL;
    }
    for (size_t i = 0; i < numberCount; i++) {
        possible = possible->next;
        numbers[i] = possible->string;
    }
    deleteList(possible, false);
    qsort(numbers, numberCount, sizeof(char *), stringComparator);
    char **finalNumbers = NULL;
    size_t newLength;
    if (!(finalNumbers = deleteDuplicates(numbers, numberCount, &newLength))) {
        freeArray(numbers, numberCount);
        return NULL;
    }
    free(numbers);
    PhoneNumbers *result = phnumNew();
    if (!result) {
        freeArray(finalNumbers, newLength);
        return NULL;
    }
    result->numbers = finalNumbers;
    result->quantity = newLength;
    return result;
}

PhoneNumbers *phfwdGetReverse(PhoneForward const *pf, char const *num) {
    PhoneNumbers *reverse = phfwdReverse(pf, num);
    if (!reverse || reverse->quantity == 0) return reverse;
    PhoneNumbers *getReverse = phnumNew();
    char **getReverseNumbers = NULL;
    size_t getReverseSize = 0;
    size_t getReverseQuantity = 0;
    if (!getReverse) {
        phnumDelete(reverse);
        return NULL;
    }
    PhoneNumbers *getNumbers;
    for (size_t i = 0; i < reverse->quantity; i++) {
        getNumbers = phfwdGet(pf, reverse->numbers[i]);
        if (!getNumbers || !stringDiff(getNumbers->numbers[0], num)) {
            if (getNumbers &&
                addToDynamicArray(&getReverseNumbers, &getReverseSize,
                                  getReverseQuantity, reverse->numbers[i])) {
                getReverseQuantity++;
                reverse->numbers[i] = NULL;
            } else {
                // Albo nie udało się dodać do tablicy albo getNumbers to NULL.
                freeArray(getReverseNumbers, getReverseQuantity);
                phnumDelete(getReverse);
                phnumDelete(reverse);
                phnumDelete(getNumbers);
                return NULL;
            }
        }
        phnumDelete(getNumbers);
    }
    getReverseNumbers =
        realloc(getReverseNumbers, getReverseQuantity * sizeof(char *));
    // Nie może się nie udać bo getReverseQuantity <= getReverseSize.
    getReverse->numbers = getReverseNumbers;
    getReverse->quantity = getReverseQuantity;
    phnumDelete(reverse);
    return getReverse;
}

char const *phnumGet(PhoneNumbers const *pnum, size_t idx) {
    if (!pnum || idx >= pnum->quantity)
        return NULL;
    else
        return (char const *)pnum->numbers[idx];
}