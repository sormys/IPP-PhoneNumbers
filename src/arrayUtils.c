/** @file
 * Implementacja operacji tablicach znaków.
 *
 * @author Szymon Potrzebowski <sp438683@students.mimuw.edu.pl>
 * @date 2022
 */

#include "arrayUtils.h"
#include "string_number.h"

char **deleteDuplicates(char **array, size_t length, size_t *newLength)
{
    size_t duplicates = 0;
    *newLength = 0;
    for (size_t i = 0; i + 1 < length; i++)
    {
        if (!stringDiff(array[i], array[i + 1]))
        {
            free(array[i]);
            array[i] = NULL;
            duplicates++;
        }
    }
    char **arrayCopy = malloc((length - duplicates) * sizeof(char *));
    if (arrayCopy)
    {
        *newLength = length - duplicates;
        size_t j = 0;
        for (size_t i = 0; i < length; i++)
            if (array[i])
                arrayCopy[j++] = array[i];
    }
    return arrayCopy;
}

void freeArray(char **array, size_t length)
{
    for (size_t i = 0; i < length; i++)
        free(array[i]);
    free(array);
}

bool addToDynamicArray(char ***array, size_t *size, size_t index, char *toAdd)
{
    char **tmp;
    if (index >= (*size))
    {
        if ((tmp = realloc(*array, (((*size) + 1) * 2) * sizeof(char *))))
        {
            *size = ((*size) + 1) * 2;
            *array = tmp;
        }
        else
        {
            return false;
        }
    }
    (*array)[index] = toAdd;
    return true;
}