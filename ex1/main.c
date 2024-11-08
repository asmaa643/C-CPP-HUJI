#include "cipher.h"
#include "tests.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_LINE_LENGTH 1024
#define DECIMAL_BASE 10

int do_tests ()
{
  return test_encode_non_cyclic_lower_case_positive_k () ||
         test_encode_cyclic_lower_case_special_char_positive_k () ||
         test_encode_non_cyclic_lower_case_special_char_negative_k () ||
         test_encode_cyclic_lower_case_negative_k () ||
         test_encode_cyclic_upper_case_positive_k () ||
         test_decode_non_cyclic_lower_case_positive_k () ||
         test_decode_cyclic_lower_case_special_char_positive_k () ||
         test_decode_cyclic_upper_case_positive_k () ||
         test_decode_non_cyclic_lower_case_special_char_negative_k () ||
         test_decode_cyclic_lower_case_negative_k ();
}

/**
 * Decode or Encode the File (according to the command).
 */
void do_the_job (char *arr[], FILE *in, FILE *out, int move)
{
  char line[MAX_LINE_LENGTH];
  while (fgets (line, sizeof (line), in) != NULL)
  {
    if (strcmp (arr[1], "encode") == 0)
    {
      encode (line, move);
    }
    else
    {
      decode (line, move);
    }
    fputs (line, out);
  }
}

/**
 * Checks the validity of the program gets one argument.
 * @return 0 upon success.
 */
int check_one_argument (char one_command[])
{
  if (strcmp (one_command, "test") != 0)
  {
    fprintf (stderr, "Usage: cipher test\n");
  }
  else
  {
    if (do_tests () == 0)
    {
      return 0;
    }
  }
  return 1;
}

/**
* Checks the validity of the program gets more than one argument.
* @return "S" upon success, "F" for Failure.
*/
char *check2 (char *arr[], int num_arguments)
{
  const int program_num_arg = 5;
  if (num_arguments == program_num_arg)
  {
    if (strcmp (arr[1], "encode") != 0 && strcmp (arr[1], "decode") != 0)
    {
      fprintf (stderr, "The given command is invalid.\n");
      return "F";
    }
    return "S";
  }
  else
  {
    fprintf (stderr, "The program receives 1 or 4 arguments only.\n");
    return "F";
  }
}

/**
 * Checks rest of the arguments and writes the changed text.
 * @return "S" upon success.
 */
char *check_the_rest_and_run (char *arr[])
{
  char *remains;
  long int shift = strtol (arr[2], &remains, DECIMAL_BASE);
  if (strcmp (remains, "") != 0)
  {
    fprintf (stderr, "The given shift value is invalid.\n");
    return "F";
  }
  FILE *in = fopen (arr[3], "r");
  FILE *out = fopen (arr[4], "w");
  if (in == NULL || out == NULL)
  {
    fprintf (stderr, "The given file is invalid.\n");
    if (out != NULL)
    {
      fclose (out);
    }
    return "F";
  }
  do_the_job (arr, in, out, shift);
  fclose (in);
  fclose (out);
  return "S";
}

/**
* Main function: runs the program.
* @return 0 upon success.
*/
int main (int argc, char *argv[])
{
  if (argc == 2)
  {
    if (check_one_argument (argv[1]) == 0)
    {
      return EXIT_SUCCESS;
    }
    else
    {
      return EXIT_FAILURE;
    }
  }
  if (strcmp (check2 (argv, argc), "F") == 0)
  {
    return EXIT_FAILURE;
  }
  else
  {
    char *last_job = check_the_rest_and_run (argv);
    if (strcmp (last_job, "F") == 0)
    {
      return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
  }
}
