#include "markov_chain.h"
#include <string.h>
/**
* Get random number between 0 and max_number [0, max_number).
* @param max_number maximal number to return (not including)
* @return Random number
*/
int get_random_number (int max_number)
{
  return rand () % max_number;
}

MarkovNode *get_first_random_node (MarkovChain *markov_chain)
{
  int random = get_random_number (markov_chain->database->size);
  while (1)
  {
    Node *n = markov_chain->database->first;
    int i = 0;
    while (i < random && n)
    {
      n = n->next;
      i++;
    }
    int len = n->data->len;
    if (n->data->data[len - 1] != '.')
    {
      return n->data;
    }
    random = get_random_number (markov_chain->database->size);
  }
}

MarkovNode *get_next_random_node (MarkovNode *state_struct_ptr)
{
  int random = get_random_number (state_struct_ptr->all_comp);
  int i = 0;
  int j = 0;
  while (i < random)
  {
    NextNodeCounter first = state_struct_ptr->counter_list[j];
    i += first.frequency;
    if (i > random)
    {
      return first.markov_node;
    }
    j++;
  }
  return state_struct_ptr->counter_list[j].markov_node;
}

void generate_random_sequence (MarkovChain *markov_chain, MarkovNode *
first_node, int max_length)
{
  char *printed = malloc (sizeof (char) * (strlen (first_node->data) + 1));
  if (printed == NULL)
  {
    fprintf (stdout, "Allocation failure: Failed while allocation");
    return;
  }
  strcpy (printed, first_node->data);
  int i = 1;
  while (i < max_length && (printed[strlen (printed) - 1] != '.'))
  {
    MarkovNode *next = get_next_random_node (first_node);
    char *s = realloc (printed, sizeof (char)
                                * (strlen (printed) + strlen (next->data)
                                   + 2));
    if (s == NULL)
    {
      fprintf (stdout, "Allocation failure: Failed while allocation");
      free (printed);
      return;
    }
    printed = s;
    strcat (printed, " ");
    Node *node = get_node_from_database(markov_chain, next->data);
    strcat (printed, node->data->data);
    first_node = next;
    i++;
  }
  printf ("%s\n", printed);
  free (printed);
}

void free_markov_chain (MarkovChain **ptr_chain)
{
  if (ptr_chain == NULL)
  {
    return;
  }
  Node *temp;
  for (int i = 0; i < (*ptr_chain)->database->size; i++)
  {
    temp = (*ptr_chain)->database->first;
    (*ptr_chain)->database->first = (*ptr_chain)->database->first->next;
    free (temp->data->counter_list);
    free (temp->data->data);
    free (temp->data);
    free (temp);
  }
  free ((*ptr_chain)->database);
  ptr_chain = NULL;
}

bool add_node_to_counter_list (MarkovNode *first_node, MarkovNode *second_node)
{
  for (int i = 0; i < first_node->list_len; i++)
  {
    char * first = first_node->counter_list[i].markov_node->data;
    if (strcmp (first, second_node->data)== 0)
    {
      first_node->counter_list[i].frequency += 1;
      return true;
    }
  }
  if (first_node->counter_list != NULL)
  {
    NextNodeCounter *next = realloc (first_node->counter_list,
                                     sizeof (NextNodeCounter)
                                     * (first_node->list_len + 1));
    if (next == NULL)
    {
      fprintf (stdout, "Allocation failure: Failed while allocation");
      free (first_node->counter_list);
      first_node->counter_list = NULL;
      return false;
    }
    first_node->counter_list = next;
  }
  else
  {
    first_node->counter_list = malloc (sizeof (NextNodeCounter));
    if (first_node->counter_list == NULL)
    {
      fprintf (stdout, "Allocation failure: Failed while allocation");
      return false;
    }
  }
  first_node->counter_list[first_node->list_len].markov_node = second_node;
  first_node->counter_list[first_node->list_len].frequency = 1;
  first_node->list_len++;
  return true;
}

Node *get_node_from_database (MarkovChain *markov_chain, char *data_ptr)
{
  Node *temp = markov_chain->database->first;
  while (temp != NULL)
  {
    if (strcmp (temp->data->data, data_ptr) == 0)
    {
      return temp;
    }
    temp = temp->next;
  }
  return NULL;
}

Node *add_to_database (MarkovChain *markov_chain, char *data_ptr)
{
  Node *r = get_node_from_database (markov_chain, data_ptr);
  if (r != NULL)
  {
    r->data->all_comp++;
    return r;
  }
  MarkovNode *node = malloc (sizeof (*node));
  if (node == NULL)
  {
    fprintf (stdout, "Allocation failure: Failed while allocation");
    return NULL;
  }
  node->data = malloc (sizeof (char) * (strlen (data_ptr) + 1));
  if (node->data == NULL)
  {
    fprintf (stdout, "Allocation failure: Failed while allocation");
    return NULL;
  }
  strcpy (node->data, data_ptr);
  node->len = (int) strlen (data_ptr);
  node->all_comp = 1;
  node->list_len = 0;
  node->counter_list = NULL;
  if (add (markov_chain->database, node) == 1)
  {
    return NULL;
  }
  return markov_chain->database->last;
}
