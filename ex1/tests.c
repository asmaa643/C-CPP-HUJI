#include "tests.h"
#include "cipher.h"
#include <string.h>

#define K_1 3
#define K_2 2
#define K_3 -1
#define K_4 -3
#define K_5 29

// See full documentation in header file
int test_encode_non_cyclic_lower_case_positive_k ()
{
  char in[] = "abc";
  char out[] = "def";
  encode (in, K_1);
  char e[] = "safaa";
  encode (e, K_1);
  return strcmp (in, out) != 0 || strcmp (e, "vdidd") != 0;
}

// See full documentation in header file
int test_encode_cyclic_lower_case_special_char_positive_k ()
{
  char e[] = "sa, az";
  char w[] = "abcdefg hijklmnopq rstuvwxyz";
  encode (w, K_2);
  encode (e, K_2);
  int s1 = strcmp (e, "uc, cb");
  int s2 = strcmp ("cdefghi jklmnopqrs tuvwxyzab", w);
  return s1 != 0 || s2 != 0;
}

// See full documentation in header file
int test_encode_non_cyclic_lower_case_special_char_negative_k ()
{
  char in[] = "bc. d,";
  char out[] = "ab. c,";
  encode (in, K_3);
  char e[] = "sbfbz";
  encode (e, K_3);
  return strcmp (in, out) != 0 || strcmp (e, "raeay") != 0;
}

// See full documentation in header file
int test_encode_cyclic_lower_case_negative_k ()
{
  // your code goes here
  char in[] = "abcf";
  char out[] = "xyzc";
  encode (in, K_4);
  char e[] = "sbfbz";
  encode (e, K_4);
  return strcmp (in, out) != 0 || strcmp (e, "pycyw") != 0;
}

int test_encode_cyclic_upper_case_positive_k ()
{
  // your code goes here
  char w2[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  char w[] = "AQJ";
  encode (w, K_5);
  encode (w2, K_5);
  return strcmp (w2, "DEFGHIJKLMNOPQRSTUVWXYZABC") != 0 ||
         strcmp (w, "DTM") != 0;
}

int test_decode_non_cyclic_lower_case_positive_k ()
{
  char in[] = "def";
  char out[] = "abc";
  decode (in, K_1);
  char e[] = "vdidd";
  decode (e, K_1);
  return strcmp (in, out) != 0 || strcmp (e, "safaa") != 0;
}

int test_decode_cyclic_lower_case_special_char_positive_k ()
{
  // your code goes here
  char e[] = "uc, cb";
  char w[] = "cdefghi jklmnopqrs tuvwxyzab";
  decode (w, K_2);
  decode (e, K_2);
  int s1 = strcmp (e, "sa, az");
  int s2 = strcmp ("abcdefg hijklmnopq rstuvwxyz", w);
  return s1 != 0 || s2 != 0;
}

int test_decode_non_cyclic_lower_case_special_char_negative_k ()
{
  char in[] = "ab. c,";
  char out[] = "bc. d,";
  decode (in, K_3);
  char e[] = "raeay";
  decode (e, K_3);
  return strcmp (in, out) != 0 || strcmp (e, "sbfbz") != 0;
}

// See full documentation in header file
int test_decode_cyclic_lower_case_negative_k ()
{
  char in[] = "xyzc";
  char out[] = "abcf";
  decode (in, K_4);
  char e[] = "pycyw";
  decode (e, K_4);
  return strcmp (in, out) != 0 || strcmp (e, "sbfbz") != 0;
}

// See full documentation in header file
int test_decode_cyclic_upper_case_positive_k ()
{
  char w2[] = "DEFGHIJKLMNOPQRSTUVWXYZABC";
  char w[] = "DTM";
  decode (w, K_5);
  decode (w2, K_5);
  return strcmp (w2, "ABCDEFGHIJKLMNOPQRSTUVWXYZ") != 0 ||
         strcmp (w, "AQJ") != 0;
}
