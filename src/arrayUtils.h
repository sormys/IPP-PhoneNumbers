/** @file
 * Operacje na tablicach znaków.
 *
 * @author Szymon Potrzebowski <sp438683@students.mimuw.edu.pl>
 * @date 2022
 */

#ifndef __UTLIS_H__
#define __UTLIS_H__

#include <stdlib.h>
#include <stdbool.h>

/**
 * @brief Tworzy nową tablice niepowtarzających się napisów tablicy @p array.
 * Tworzy nową tablice napisów usuwając duplikaty z posortowanej tablicy
 * @p array. Zwalnia duplikaty i zamienia je w orginalej tablicy na NULL.
 * @param array Tablica z napisami.
 * @param length Długość talbicy.
 * @param newLength Tutaj zostanie zapisana długość nowej talbicy.
 * @return Wkaźnik na nową tablice napisów.
 */
char **deleteDuplicates(char **array, size_t length, size_t *newLength);

/**
 * @brief Zwalnia tablice napisów.
 * Zwalnia napisy w tablicy i samą tablice.
 * @param array Tablica do zwolnienia.
 * @param length Długość tablicy.
 */
void freeArray(char **array, size_t length);

/**
 * @brief Dodaje napis do tablicy na daną pozycję.
 * Dodaje napis do tablicy na daną pozycję. Jeżeli obecnie tablica jest za
 * mała aby dodać napis na daną pozycję spróbuje zaalokować pamięć na
 * powiększenie tablicy. Jeżeli się powiedzie zmieni wartość przechowywaną
 * przez @p *size. Jeżeli nie, nie zwiększy tablicy i nie doda napisu.
 * @param array Tablica do której ma zostać dodany napis.
 * @param size Wskaźnik na zmienną przechowującą obecny rozmiar tablicy.
 * @param index Na które miejsce ma zostać dodany napis @p toAdd.
 * @param toAdd Napis do dodania.
 * @return true - jeżeli udało się dodać do tablicy.
 * @return false - jeżeli nie udało się dodać do talbicy.
 */
bool addToDynamicArray(char ***array, size_t *size, size_t index, char *toAdd);
#endif