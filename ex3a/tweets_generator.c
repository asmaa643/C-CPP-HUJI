
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

int fill_database (FILE *fp, int words_to_read, MarkovChain *markov_chain)
{
  char line[MAX_LINE];
  while (fgets (line, sizeof (line), fp) != NULL
         && (words_to_read > ZERO || words_to_read <= NO_WORDS))
  {
    char *one_word;
    MarkovNode *before = NULL;
    one_word = strtok (line, " \n\r");
    while ((words_to_read > ZERO || words_to_read <= NO_WORDS)
           && one_word != NULL)
    {
      Node *cur = add_to_database (markov_chain, one_word);
      if (cur == NULL)
      {
        return 1;
      }
      if (before != NULL)
      {
        if (add_node_to_counter_list (before, cur->data) == false)
        {
          return 1;
        }
      }
      before = cur->data;
      one_word = strtok (NULL, " \n\r");
      words_to_read--;
    }
  }
  return 0;
}

int main (int argc, char *argv[])
{
  if (argc != FOUR_ARGS && argc != FIVE_ARGS)
  {
    fprintf (stdout, "Usage: You have to give 3 or 4 arguments!");
    return EXIT_FAILURE;
  }
  long int seed = strtol (argv[1], NULL, DECIMAL);
  srand (seed);
  long int tweets = strtol (argv[2], NULL, DECIMAL);
  FILE *text_corpus = fopen (argv[3], "r");
  if (text_corpus == NULL)
  {
    fprintf (stdout, "Error: File doesn't exist.");
    return EXIT_FAILURE;
  }
  long int words = -1;
  if (argc == FIVE_ARGS)
  {
    words = strtol (argv[4], NULL, DECIMAL);
  }
  MarkovChain *markov = malloc (sizeof (MarkovChain));
  if (markov == NULL)
  {
    fprintf (stdout, "Allocation failure: Failed while allocation");
    return EXIT_FAILURE;
  }
  markov->database = malloc (sizeof (LinkedList));
  markov->database->size = 0;
  if (markov->database == NULL)
  {
    fprintf (stdout, "Allocation failure: Failed while allocation");
    return EXIT_FAILURE;
  }
  markov->database->first = NULL;
  if (fill_database (text_corpus, (int)words, markov) == 1)
  {
    free_markov_chain (&markov);
    return EXIT_FAILURE;
  }
  for (int i = 0; i < tweets; i++)
  {
    MarkovNode *node = get_first_random_node (markov);
    printf ("Tweet %d: ", i + 1);
    generate_random_sequence (markov, node, MAX_TWEET);
  }
  free_markov_chain (&markov);
  return EXIT_SUCCESS;
}
