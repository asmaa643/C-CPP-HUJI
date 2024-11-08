#include <string.h> // For strlen(), strcmp(), strcpy()
#include "markov_chain.h"

#define MAX(X, Y) (((X) < (Y)) ? (Y) : (X))

#define EMPTY -1
#define BOARD_SIZE 100
#define MAX_GENERATION_LENGTH 60

#define DICE_MAX 6
#define NUM_OF_TRANSITIONS 20
#define DECIMAL 10
#define ARGUMENTS 3
#define LAST 100
#define SNAKE "-snake to %d"
#define LADDER "-ladder to %d"
/**
 * represents the transitions by ladders and snakes in the game
 * each tuple (x,y) represents a ladder from x to if x<y or a snake otherwise
 */
const int transitions[][2] = {{13, 4},
                              {85, 17},
                              {95, 67},
                              {97, 58},
                              {66, 89},
                              {87, 31},
                              {57, 83},
                              {91, 25},
                              {28, 50},
                              {35, 11},
                              {8,  30},
                              {41, 62},
                              {81, 43},
                              {69, 32},
                              {20, 39},
                              {33, 70},
                              {79, 99},
                              {23, 76},
                              {15, 47},
                              {61, 14}};

/**
 * struct represents a Cell in the game board
 */
typedef struct Cell
{
    int number; // Cell number 1-100
    int ladder_to;  // ladder_to represents the jump of the ladder in case
    // there is one from this square
    int snake_to;  // snake_to represents the jump of the snake in case
    // there is one from this square both ladder_to and snake_to should be -1
    // if the Cell doesn't have them
} Cell;

/** Error handler **/
static int handle_error (char *error_msg, MarkovChain **database)
{
  printf ("%s", error_msg);
  if (database != NULL)
  {
    free_markov_chain (database);
  }
  return EXIT_FAILURE;
}

static int create_board (Cell *cells[BOARD_SIZE])
{
  for (int i = 0; i < BOARD_SIZE; i++)
  {
    cells[i] = malloc (sizeof (Cell));
    if (cells[i] == NULL)
    {
      for (int j = 0; j < i; j++)
      {
        free (cells[j]);
      }
      handle_error (ALLOCATION_ERROR_MASSAGE, NULL);
      return EXIT_FAILURE;
    }
    *(cells[i]) = (Cell) {i + 1, EMPTY, EMPTY};
  }

  for (int i = 0; i < NUM_OF_TRANSITIONS; i++)
  {
    int from = transitions[i][0];
    int to = transitions[i][1];
    if (from < to)
    {
      cells[from - 1]->ladder_to = to;
    }
    else
    {
      cells[from - 1]->snake_to = to;
    }
  }
  return EXIT_SUCCESS;
}

/**
 * fills database
 * @param markov_chain
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
static int fill_database (MarkovChain *markov_chain)
{
  Cell *cells[BOARD_SIZE];
  if (create_board (cells) == EXIT_FAILURE)
  {
    return EXIT_FAILURE;
  }
  MarkovNode *from_node = NULL, *to_node = NULL;
  size_t index_to;
  for (size_t i = 0; i < BOARD_SIZE; i++)
  {
    add_to_database (markov_chain, cells[i]);
  }

  for (size_t i = 0; i < BOARD_SIZE; i++)
  {
    from_node = get_node_from_database (markov_chain, cells[i])->data;

    if (cells[i]->snake_to != EMPTY || cells[i]->ladder_to != EMPTY)
    {
      index_to = MAX(cells[i]->snake_to, cells[i]->ladder_to) - 1;
      to_node = get_node_from_database (markov_chain, cells[index_to])
          ->data;
      add_node_to_counter_list (from_node, to_node, markov_chain);
    }
    else
    {
      for (int j = 1; j <= DICE_MAX; j++)
      {
        index_to = ((Cell *) (from_node->data))->number + j - 1;
        if (index_to >= BOARD_SIZE)
        {
          break;
        }
        to_node = get_node_from_database (markov_chain, cells[index_to])
            ->data;
        add_node_to_counter_list (from_node, to_node, markov_chain);
      }
    }
  }
  // free temp arr
  for (size_t i = 0; i < BOARD_SIZE; i++)
  {
    free (cells[i]);
  }
  return EXIT_SUCCESS;
}

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
  Cell *c = (Cell *) data;
  printf ("[%d]", c->number);
  if (c->ladder_to != -1)
  {
    printf (LADDER, c->ladder_to);
  }
  else if (c->snake_to != -1)
  {
    printf (SNAKE, c->snake_to);
  }
  if (c->number != LAST)
  {
    printf (" -> ");
  }
}

static int comp (void *data1, void *data2)
{
  Cell *c1 = (Cell *) data1;
  Cell *c2 = (Cell *) data2;
  if (c1->number == c2->number)
  {
    return 0;
  }
  else if (c1->number > c2->number)
  {
    return 1;
  }
  return -1;
}

static void free_d (void *data)
{
  free ((Cell *) data);
}

static void *copy (void *data)
{
  Cell *new_cell = malloc (sizeof (Cell));
  if (new_cell != NULL)
  {
    new_cell->number = ((Cell *) data)->number;
    new_cell->snake_to = ((Cell *) data)->snake_to;
    new_cell->ladder_to = ((Cell *) data)->ladder_to;
    return new_cell;
  }
  return NULL;
}

static bool last (void *data)
{
  return ((Cell *) data)->number == LAST;
}

static void give_sentences (long int sentences, MarkovChain *markov)
{
  for (int i = 0; i < sentences; i++)
  {
    MarkovNode *node = markov->database->first->data;
    printf ("Random Walk %d: ", i + 1);
    generate_random_sequence (markov, node, MAX_GENERATION_LENGTH);
    printf ("\n");
  }
}

static int set_our_chain_and_run (MarkovChain *markov, long int sentences)
{
  markov->database->first = NULL;
  markov->database->last = markov->database->first;
  set_functions_for_chain (markov);
  if (fill_database (markov) == EXIT_FAILURE)
  {
    handle_error (ALLOCATION_ERROR_MASSAGE, &markov);
    return EXIT_FAILURE;
  }
  give_sentences (sentences, markov);
  return EXIT_SUCCESS;
}

/**
 * @param argc num of arguments
 * @param argv 1) Seed
 *             2) Number of sentences to generate
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int main (int argc, char *argv[])
{
  if (argc != ARGUMENTS)
  {
    fprintf (stdout, "Usage: You have to give 2 arguments!");
    return EXIT_FAILURE;
  }
  long int seed = strtol (argv[1], NULL, DECIMAL);
  srand (seed);
  long int sentences = strtol (argv[2], NULL, DECIMAL);
  MarkovChain *markov = malloc (sizeof (MarkovChain));
  if (markov == NULL)
  {
    fprintf (stdout, ALLOCATION_ERROR_MASSAGE);
    return EXIT_FAILURE;
  }
  markov->database = malloc (sizeof (LinkedList));
  markov->database->size = 0;
  if (markov->database == NULL)
  {
    fprintf (stdout, ALLOCATION_ERROR_MASSAGE);
    free (markov);
    markov = NULL;
    return EXIT_FAILURE;
  }
  if (set_our_chain_and_run (markov, sentences) == EXIT_FAILURE)
  {
    return EXIT_FAILURE;
  }
  free_markov_chain (&markov);
  return EXIT_SUCCESS;
}
