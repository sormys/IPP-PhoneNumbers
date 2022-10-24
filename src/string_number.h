/** @file
 * Operacje na numerach zapisanych jako ciągi znaków.
 *
 * @author Szymon Potrzebowski <sp438683@students.mimuw.edu.pl>
 * @date 2022
 */

#ifndef __STRING_NUMBER_H__
#define __STRING_NUMBER_H__

#include <stdbool.h>
#include <stddef.h>

#define DIGITS 12 /**< Liczba dozwolonych znaków*/

/**
 * @brief Wyznacza indeks w tablicy synów PhoneForward.
 * Wyznacza indeks w tablicy synów PhoneForward na posdstawie znaku numeru.
 * UWAGA: musi to być znak dozwolony w numerze.
 * @return indeks w tablicy PhoneForward.
 */
size_t getIndex(char const c);

/** @brief Określa czy znak jest dozowlony.
 *  Określa czy znak @p c jest dozowlony.
 *  @return 1 jeżeli jest dozwolony, 0 jeżeli nie.
 */
bool isLegal(int c);

/** @brief Określa długość dozowolonego ciągu znaków.
 *  Określa długość dozowolonego ciągu znaków @p string.
 *  @return Długość ciągu znaków, 0 jeżeli ciąg znaków jest niedozwolony.
 */
size_t stringLength(char const *string);

/**
 * @brief Tworzy kopie ciągu znaków.
 * Funkcja tworzy kopie ciągu znaków o określonej długości.
 * @param string ciąg znaków do przekopiowania.
 * @param size długość ciągu znaków bez '\0'.
 * @return Kopię ciągu znaków. NULL jeżeli nie udało się zaalokować pamięci.
 */
char *copyString(char const *string, size_t size);

/**
 * @brief Porównuje dwa ciągi znaków.
 * Porównuje dwa ciągiu znaków o tej samej długości.
 * @return true jeżeli ciągi znaków się różnią.
 * @return false jeżeli ciągi znaków są takie same.
 */
bool stringDiff(char const *str1, char const *str2);

/**
 * @brief Łączy pierwsze @p lastIdx1 znaków @p num1 ze znakami od @p firstIdx2
 * z @p num2.
 * Łączy pierwsze @p lastIdx1 znaków @p num1 z ostatnimi @p lastIdx2 -
 * @p firstIdx2 znakami @p num2 i tworzy nowy nowy napis. UWAGA: musi otrzymać
 * w @p result wskaźnik na zaalokowaną o długości uzyskanego w wyniku tej
 * operacji napisu.
 *
 * @param result Wskaźnik na zaalokowane miejsce w pamięci na napis wynikowy.
 * @param num1 Pierwszy napis.
 * @param lastIdx1 Liczba znaków pierwszego napisu do przekopiowania.
 * @param num2 Drugi napis.
 * @param firstIdx2 Indeks pierwszego znaku drugiego napisu do przekopiowania.
 * @param lastIdx2 Indeks ostatniego znaku drugiego napisu do przekopiowania.
 */
void forwardNumber(char *result, const char *num1,size_t lastIdx1, 
                   const char *num2, size_t firstIdx2, size_t lastIdx2);

/**
 * @brief Komparator napisów potrzebny do qsort.
 * Komparator napisów potrzebny do qsort.
 * @return 1 jeżeli @p a1 > @p a2, -1 jeżeli @p a1 < @p a2, 0 jeżeli są równe.
 */
int stringComparator(const void *a1, const void *a2);

#endif