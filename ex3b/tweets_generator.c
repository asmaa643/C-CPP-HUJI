//
// Created by asmaa on 03/05/2022.
//
#include <string.h>
#include "markov_chain.h"
#include <stdio.h>
#include <stdlib.h>
#define MAX_LINE 1000
#define NO_WORDS -1
#define ZERO 0
#define FIVE_ARGS 5
#define FOUR_ARGS 4
#define DECIMAL 10
#define MAX_TWEET 20

static void print (void *data);
static int comp (void *data1, void *data2);
static void free_d (void *data);
static void *copy (void *data);
static bool last (void *data);

static void set_functions_for_chain (MarkovChain *markov_chain)
{
  markov_chain->print_func = print;
  markov_chain->comp_func = comp;
  markov_chain->free_data = free_d;
  markov_chain->copy_func = copy;
  markov_chain->is_last = last;
}

static void print (void *data)
{
  printf (" %s", (char *) data);
}

static int comp (void *data1, void *data2)
{
  char *str1 = (char *) data1;
  char *str2 = (char *) data2;
  return strcmp (str1, str2);
}

static void free_d (void *data)
{
  free ((char *) data);
}

static void *copy (void *data)
{
  char *new_str = malloc ((strlen ((char *) data) + 1) * sizeof (char));
  if (new_str != NULL)
  {
    strcpy (new_str, (char *) data);
    return new_str;
  }
  return NULL;
}

static long int get_length (void *data)
{
  return (long) strlen ((char *) data);
}

static bool last (void *data)
{
  return ((char *) data)[get_length (data) - 1] == '.';
}

static int
fill_database (FILE *fp, int words_to_read, MarkovChain *markov_chain)
{
  char line[MAX_LINE];
  while (fgets (line, sizeof (line), fp) != NULL
         && (words_to_read > ZERO || words_to_read == NO_WORDS))
  {
    char *word = strtok (line, " \n\r");
    MarkovNode *before = NULL;
    while ((words_to_read > ZERO || words_to_read == NO_WORDS) && word != NULL)
    {
      Node *cur = add_to_database (markov_chain, word);
      if (cur == NULL)
      {
        return 1;
      }
      if (before != NULL)
      {
        if (!(add_node_to_counter_list (before, cur->data, markov_chain)))
        {
          return 1;
        }
      }
      before = cur->data;
      word = strtok (NULL, " \n\r");
      if (words_to_read != -1)
      {
        words_to_read--;
      }
    }
  }
  return 0;
}

/**
 * Write our tweets.
 */
static void write_tweets (int tweets, MarkovChain *markov)
{
  for (int i = 0; i < tweets; i++)
  {
    MarkovNode *node = get_first_random_node (markov);
    printf ("Tweet %d:", i + 1);
    generate_random_sequence (markov, node, MAX_TWEET);
    printf ("\n");
  }
}

/**
 * Check the validity of the arguments number.
 * @return 0 upon success.
 */
static int check_args (int argc)
{
  if (argc != FOUR_ARGS && argc != FIVE_ARGS)
  {
    fprintf (stdout, "Usage: You have to give 3 or 4 arguments!");
    return 1;
  }
  return 0;
}

/**
 * @param s string of the seed value
 * @param t string of the number ot tweets to write
 * @return hoe many tweets to write.
 */
long int arguments_to_run (char *s, char *t)
{
  long int seed = strtol (s, NULL, DECIMAL);
  srand (seed);
  long int tweets = strtol (t, NULL, DECIMAL);
  return tweets;
}

/**
 * This function sets the words to read
 * @return words to read and -1 if there is no words parameter.
 */
static long int how_many_words (int argc, char *argv[])
{
  long int words = -1;
  if (argc == FIVE_ARGS)
  {
    words = strtol (argv[4], NULL, DECIMAL);
  }
  return words;
}

/**
 * This function sets the database parameters.
 * @param markov the chain that was built.
 */
static void set_our_markov (MarkovChain *markov)
{
  markov->database->size = 0;
  markov->database->first = NULL;
  markov->database->last = markov->database->first;
  set_functions_for_chain (markov);
}
FILE * open_file(char * file)
{
  FILE *text_corpus = fopen (file, "r");
  if (text_corpus == NULL)
  {
    fprintf (stdout, "Error: File doesn't exist.\n");
  }
  return text_corpus;
}

void handle_error_and_close_file(FILE* text_corpus)
{
  fprintf (stdout, ALLOCATION_ERROR_MASSAGE);
  fclose (text_corpus);
}

int main (int argc, char *argv[])
{
  if (check_args (argc) == 1)
  {
    return EXIT_FAILURE;
  }
  long int tweets = arguments_to_run (argv[1], argv[2]);
  FILE *text_corpus = open_file (argv[3]);
  if (text_corpus == NULL)
  {
    return EXIT_FAILURE;
  }
  long int words = how_many_words (argc, argv);
  MarkovChain *markov = malloc (sizeof (MarkovChain));
  if (markov == NULL)
  {
    handle_error_and_close_file (text_corpus);
    return EXIT_FAILURE;
  }
  markov->database = malloc (sizeof (LinkedList));
  if (markov->database == NULL)
  {
    handle_error_and_close_file (text_corpus);
    free (markov);
    return EXIT_FAILURE;
  }
  set_our_markov (markov);
  if (fill_database (text_corpus, (int) words, markov) == 1)
  {
    handle_error_and_close_file (text_corpus);
    free_markov_chain (&markov);
    return EXIT_FAILURE;
  }
  fclose (text_corpus);
  write_tweets ((int) tweets, markov);
  free_markov_chain (&markov);
  return EXIT_SUCCESS;
}
