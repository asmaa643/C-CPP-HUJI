#include "cipher.h"
#include <string.h>
#define FIRST_LOWER 'a'
#define LAST_LOWER 'z'
#define FIRST_UPPER 'A'
#define LAST_UPPER 'Z'

/// IN THIS FILE, IMPLEMENT EVERY FUNCTION THAT'S DECLARED IN cipher.h.

/**
* Updates a char to its valid char range.
* @return updated number of the char.
*/
int update_letter_range (int letter_to_update, char start, char end)
{
  while (letter_to_update < start)
  {
    letter_to_update = letter_to_update + end - start + 1;
  }
  while (letter_to_update > end)
  {
    letter_to_update = letter_to_update - end + start - 1;
  }
  return letter_to_update;
}

/**
* Checks if a char is a letter and changes it.
* @return updated char if it has changed, the original one if not.
*/
char change_one_char (char original, int shifted)
{
  if ((original >= FIRST_LOWER) && (original <= LAST_LOWER))
  {
    shifted = update_letter_range (shifted, FIRST_LOWER, LAST_LOWER);
    return (char) shifted;
  }
  if ((original >= FIRST_UPPER) && (original <= LAST_UPPER))
  {
    shifted = update_letter_range (shifted, FIRST_UPPER, LAST_UPPER);
    return (char) shifted;
  }
  return original;
}

// See full documentation in header file
void encode (char s[], int k)
{
  for (size_t i = 0; i < strlen (s); i++)
  {
    int c = s[i] + k;
    s[i] = change_one_char (s[i], c);
  }
}

// See full documentation in header file
void decode (char s[], int k)
{
  for (size_t i = 0; i < strlen (s); i++)
  {
    int c = s[i] - k;
    s[i] = change_one_char (s[i], c);
  }
}
