/** @file
 * Implementacja operacji na numerach zapisanych jako ciągi znaków.
 *
 * @author Szymon Potrzebowski <sp438683@students.mimuw.edu.pl>
 * @date 2022
 */

#include "string_number.h"

#include <stdio.h>
#include <stdlib.h>

size_t getIndex(char const c) {
    if ('0' <= c && c <= '9')
        return c - '0';
    else if (c == '*')
        return 10;
    else if (c == '#') {
        return 11;
    }
    return -1;
}

bool isLegal(int c) { return (('0' <= c && c <= '9') || c == '#' || c == '*'); }

size_t stringLength(char const *string) {
    size_t i = 0;
    if (!string) return 0;
    while (isLegal(string[i])) i++;
    if (string[i] != '\0') return 0;
    return i;
}

char *copyString(char const *string, size_t size) {
    char *tmp = malloc(sizeof(char) * (size + 1));
    if (tmp) {
        for (size_t i = 0; i < size + 1; i++) {
            tmp[i] = string[i];
        }
    }
    return tmp;
}

bool stringDiff(char const *str1, char const *str2) {
    size_t i = 0;
    while (str1[i] != '\0' && str1[i] == str2[i]) i++;
    return str1[i] != str2[i];
}

int stringComparator(const void *a1, const void *a2) {
    const char **str1 = (const char **)a1;
    const char **str2 = (const char **)a2;
    size_t i = 0;
    while ((*str1)[i] != '\0' && (*str2)[i] != '\0') {
        if (getIndex((*str1)[i]) > getIndex((*str2)[i]))
            return 1;
        else if (getIndex((*str1)[i]) < getIndex((*str2)[i]))
            return -1;
        i++;
    }
    if ((*str1)[i] == '\0' && (*str2)[i] == '\0')
        return 0;
    else if ((*str1)[i] == '\0')
        return -1;
    return 1;
}

void forwardNumber(char *result, const char *num1, size_t lastIdx1,
                   const char *num2, size_t firstIdx2, size_t lastIdx2) {
    size_t resultLength = lastIdx1 + lastIdx2 - firstIdx2;
    for (size_t i = 0; i < lastIdx1; i++) result[i] = num1[i];
    for (size_t i = 0; i + lastIdx1 < resultLength; i++)
        result[i + lastIdx1] = num2[i + firstIdx2];
    result[resultLength] = '\0';
}