/** @file
 * Implementacja operacji na cyklicznych listach napisów.
 *
 * @author Szymon Potrzebowski <sp438683@students.mimuw.edu.pl>
 * @date 2022
 */

#include "list.h"
#include "phone_forward.h"

List *listNew(void)
{
    List *tmp;
    if ((tmp = malloc(sizeof(List))))
    {
        tmp->prev = tmp;
        tmp->next = tmp;
        tmp->string = NULL;
    }
    return tmp;
}

void deleteList(List *list, bool freeStrings)
{
    if (list == NULL)
        return;
    List *tmp;
    while (list->next != list)
    {
        tmp = list;
        list = list->next;
        deleteNode(tmp, freeStrings);
    }
    if (freeStrings)
        free(list->string);
    free(list);
}

bool isEmpty(List *list)
{
    return !list || list->next == list;
}

List *addString(List *list, char *string)
{
    if (list == NULL || string == NULL)
        return NULL;
    List *tmp;

    if ((tmp = malloc(sizeof(List))))
    {
        tmp->string = string;
        tmp->next = list->next;
        list->next->prev = tmp;
        tmp->prev = list;
        list->next = tmp;
    }
    return tmp;
}

char *getFromList(List *list, int index)
{
    if (list == NULL)
        return NULL;
    int i = 0;
    while (list->next->string != NULL && i < index)
    {
        list = list->next;
        i++;
    }
    if (i != index)
        return NULL;
    return list->next->string;
}

int listLength(List *list)
{
    int length = 0;
    if (list)
    {
        while (list->next->string)
        {
            list = list->next;
            length++;
        }
    }
    return length;
}

void deleteNode(List *list, bool freeString)
{
    if (!list || !list->string)
        return;
    list->next->prev = list->prev;
    list->prev->next = list->next;
    if (freeString)
        free(list->string);
    free(list);
}

void deleteString(List *list, char *string)
{
    if (list == NULL)
    {
        return;
    }
    while (list->next->string)
    {
        list = list->next;
        if (list->string == string)
        {
            list->next->prev = list->prev;
            list->prev->next = list->next;
            free(list->string);
            free(list);
            break;
        }
    }
}