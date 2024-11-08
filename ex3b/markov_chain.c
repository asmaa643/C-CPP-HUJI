//
// Created by asmaa on 03/05/2022.
//
#include "markov_chain.h"
int get_random_number (int max_number)
{
  return rand () % max_number;
}

MarkovNode *get_first_random_node (MarkovChain *markov_chain)
{
  int random = get_random_number (markov_chain->database->size);
  Node *n = markov_chain->database->first;
  int i = 0;
  while (1)
  {
    while (i < random && n)
    {
      n = n->next;
      i++;
    }
    if (!(markov_chain->is_last (n->data->data)))
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
  if (first_node == NULL)
  {
    first_node = get_first_random_node (markov_chain);
  }
  markov_chain->print_func (first_node->data);
  MarkovNode *next = get_next_random_node (first_node);
  markov_chain->print_func (next->data);
  first_node = next;
  int i = 2;
  while (i < max_length && first_node->counter_list != NULL)
  {
    next = get_next_random_node (first_node);
    markov_chain->print_func (next->data);
    first_node = next;
    i++;
  }
}

void free_markov_chain (MarkovChain **ptr_chain)
{
  if (ptr_chain == NULL)
  {
    return;
  }
  if (*ptr_chain == NULL)
  {
    return;
  }
  Node *temp;
  for (int i = 0; i < (*ptr_chain)->database->size; i++)
  {
    temp = (*ptr_chain)->database->first;
    (*ptr_chain)->database->first = (*ptr_chain)->database->first->next;
    free (temp->data->counter_list);
    (*ptr_chain)->free_data ((temp->data->data));
    free (temp->data);
    free (temp);
  }
  free ((*ptr_chain)->database);
  free (*ptr_chain);
  *ptr_chain = NULL;
}

bool
check_if_before (MarkovNode *first_node,
                 MarkovNode *second_node, MarkovChain *markov_chain)
{
  for (int i = 0; i < first_node->list_len; i++)
  {
    void *first = first_node->counter_list[i].markov_node->data;
    if (markov_chain->comp_func (first, second_node->data) == 0)
    {
      first_node->counter_list[i].frequency += 1;
      return true;
    }
  }
  return false;
}

bool allocate_node_counter_list (MarkovNode *first_node)
{
  if (first_node->counter_list != NULL)
  {
    NextNodeCounter *next = realloc (first_node->counter_list,
                                     sizeof (NextNodeCounter)
                                     * (first_node->list_len + 1));
    if (next == NULL)
    {
      fprintf (stdout, ALLOCATION_ERROR_MASSAGE);
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
      fprintf (stdout, ALLOCATION_ERROR_MASSAGE);
      return false;
    }
  }
  return true;
}

bool
add_node_to_counter_list (MarkovNode *first_node,
                          MarkovNode *second_node, MarkovChain *markov_chain)
{
  if (markov_chain->is_last (first_node->data))
  {
    return true;
  }
  first_node->all_comp++;
  if (check_if_before (first_node, second_node, markov_chain)
      == true)
  {
    return true;
  }
  else
  {
    if (allocate_node_counter_list (first_node) == false)
    {
      return false;
    }
  }
  first_node->counter_list[first_node->list_len].markov_node = second_node;
  first_node->counter_list[first_node->list_len].frequency = 1;
  first_node->list_len++;
  return true;
}

Node *get_node_from_database (MarkovChain *markov_chain, void *data_ptr)
{
  Node *temp = markov_chain->database->first;
  while (temp != NULL)
  {
    if (markov_chain->comp_func (temp->data->data, data_ptr) == 0)
    {
      return temp;
    }
    temp = temp->next;
  }
  return NULL;
}

Node *add_to_database (MarkovChain *markov_chain, void *data_ptr)
{
  Node *r = get_node_from_database (markov_chain, data_ptr);
  if (r != NULL)
  {
    return r;
  }
  MarkovNode *node = malloc (sizeof (MarkovNode));
  if (node == NULL)
  {
    fprintf (stdout, ALLOCATION_ERROR_MASSAGE);
    return NULL;
  }
  node->data = markov_chain->copy_func (data_ptr);
  if (node->data == NULL)
  {
    fprintf (stdout, ALLOCATION_ERROR_MASSAGE);
    return NULL;
  }
  node->all_comp = 0;
  node->list_len = 0;
  node->counter_list = NULL;
  if (add (markov_chain->database, node) == 1)
  {
    return NULL;
  }
  return markov_chain->database->last;
}
