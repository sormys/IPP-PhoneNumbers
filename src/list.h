/** @file
 * Struktura cyklicznej listy napisów i operacje na niej.
 *
 * @author Szymon Potrzebowski <sp438683@students.mimuw.edu.pl>
 * @date 2022
 */

#ifndef __LIST_H__
#define __LIST_H__
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * @brief Struktura przechowująca węzeł listy.
 * Struktura przechowująca węzeł listy dwukierunkowej, cyklicznej,
 *  ze strażnikiem.
 */
typedef struct List
{
    struct List *prev; /**< Wskaźnik na poprzedni element listy **/
    struct List *next; /**< Wskaźnik na następny element listy **/
    char *string;      /**< napis przechowywany w węźle **/
} List;

/** @brief Tworzy nową listę cykliczną napisów.
 * Tworzy nową listę cykliczną napisów ze strażnikiem.
 * @return Wskaźnik na utworzoną listę lub NULL, gdy nie udało się
 *         alokować pamięci.
 */
List *listNew(void);

/** @brief Dodaje napis do listy.
 * Dodaje napis do listy, jeżeli uda się zaalokować miejsce.
 * @return true jeżeli dodanie powiodło się, false w przeciwnym wypadku.
 */
List *addString(List *list, char *string);

/** @brief Usuwa napis z listy.
 * Usuwa napis z listy. Zwalnia pamięć zaalokowaną na string.
 * @param list lista z której chcemy usunąć napis.
 * @param string napis który chcemy usunąć.
 */
void deleteString(List *list, char *string);

/**
 * @brief Usuwa listę.
 * Usuwa listę i zależnie od @p freeStrings zwalnia lub nie napisy.
 * @param list Strażnik listy.
 * @param freeStrings Informacja czy chcemy zwalniać napisy.
 */
void deleteList(List *list, bool freeStrings);

/** @brief Usuwa węzeł z listy.
 * Usuwa węzeł z listy. UWAGA: @p list nie może wskazywać na strażnika listy!
 */
void deleteNode(List *list, bool freeString);
// bool printfList(List *list);

/**
 * @brief Sprawdza czy lista jest pusta.
 * Sprawdza czy lista jest pusta.
 * @returns True - lista jest pusta, false - lista nie jest pusta.
 */
bool isEmpty(List *list);

/**
 * @brief Znajduje napis przechowywany przez @p index -ty element listy.
 * Znajduje napis przechowywany przez @p index -ty element listy.
 * @param list Lis
 * @param index Pozycja elementu na liście (zakres od 0 do długość listy -1).
 * @return Napis przechowywany przez @p index -ty element listy. NULL jeżeli
 * index poza zakresem lub lista jest NULLem.
 */
char *getFromList(List *list, int index);

/**
 * @brief Wyznacza długość listy.
 * Wyznacza długość listy.
 * @param list Lista której długość chcemy poznać
 * @return Długość listy.
 */
int listLength(List *list);
#endif