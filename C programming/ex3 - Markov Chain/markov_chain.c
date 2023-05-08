#include "markov_chain.h"


int count_all_words(NextNodeCounter* counter_list, int size_of_int)
{
  int total = 0;
  for (int i = 0; i < size_of_int; i++)
  {
    total += counter_list[i].frequency;
  }
  return total;
}

Node* get_i_of_chain(MarkovChain *markov_chain, int index)
{
  int counter = 0;
  Node* cur_node = markov_chain->database->first;
  while (counter < index)
  {
    cur_node = cur_node->next;
    counter++;
  }
  return cur_node;
}

int get_random_number(int max_number)
{
  return rand() % max_number;
}

MarkovNode* get_first_random_node(MarkovChain *markov_chain)
{
  int max_number = markov_chain->database->size;
  int i = get_random_number (max_number);
  Node *cur_node = get_i_of_chain(markov_chain, i);
  while (cur_node->data->counter_list == NULL)
  {
    i = get_random_number (max_number);
    cur_node = get_i_of_chain(markov_chain, i);
  }
  return cur_node->data;
}

MarkovNode* get_index_by_frequecny(NextNodeCounter* counter_list, int index,
                                   int max_number)
{
  if (!counter_list || index < 0 || index >= max_number)
  {
    return NULL;
  }
  const NextNodeCounter* current = counter_list;
  while (index >= current->frequency)
  {
    index -= current->frequency;
    current++;
  }
  return current->markov_node;
}


MarkovNode* get_next_random_node(MarkovNode *state_struct_ptr)
{
  int max_number = count_all_words(state_struct_ptr->counter_list,
                                   state_struct_ptr->size_of_list);
  int index = get_random_number (max_number);
  return get_index_by_frequecny(state_struct_ptr->counter_list, index,
                                max_number);
}

void generate_random_sequence(MarkovChain *markov_chain, MarkovNode *
first_node, int max_length)
{
  if(first_node == NULL)
  {
    first_node = get_first_random_node (markov_chain);
  }
  for(int i = 0; i < max_length; i++)
  {
    if (first_node->counter_list == NULL)
    {
      fprintf (stdout,"%s", first_node->data);
      return;
    }
    fprintf (stdout,"%s ", first_node->data);
    first_node = get_next_random_node (first_node);
  }
}

void free_markov_node(MarkovNode** ptr_markov_node)
{
  MarkovNode* markov_node = *ptr_markov_node;
  NextNodeCounter* counter_list = markov_node->counter_list;
  free (counter_list);
  counter_list = NULL;
  free (markov_node->data);
  free (markov_node);
  markov_node = NULL;
}

void free_markov_chain(MarkovChain** ptr_chain)
{
  Node* cur_node = (*ptr_chain)->database->first;
  while(cur_node != NULL)
  {
    free_markov_node(&(cur_node->data));
    Node* temp = cur_node;
    cur_node = cur_node->next;
    free (temp);
    temp = NULL;
  }
  free ((*ptr_chain)->database);
  free(*ptr_chain);
}

bool new_node_to_counter_list(MarkovNode* first_node, MarkovNode* second_node)
{
  NextNodeCounter* counter_list = first_node->counter_list;
  NextNodeCounter* resized_counter_list;
  resized_counter_list = realloc (counter_list, ((first_node->size_of_list)+1)
  *sizeof(NextNodeCounter));
  if (resized_counter_list == NULL)
  {
    free (counter_list);
    fprintf (stdout,ALLOCATION_ERROR_MASSAGE);
    return false;
  }
  NextNodeCounter new_node = {second_node, 1};
  resized_counter_list[(first_node->size_of_list)] = new_node;
  first_node->counter_list = resized_counter_list;
  (first_node->size_of_list)++;
  return true;
}

bool add_node_to_counter_list(MarkovNode *first_node, MarkovNode *second_node)
{
  NextNodeCounter* counter_list = first_node->counter_list;
  for (int i = 0; i < first_node->size_of_list; i++)
  {
    if (counter_list[i].markov_node == second_node)
    {
      counter_list[i].frequency++;
      return true;
    }
  }
  return new_node_to_counter_list(first_node, second_node);
}

Node* get_node_from_database(MarkovChain *markov_chain, char *data_ptr)
{
  Node* cur_node = markov_chain->database->first;
  while (cur_node)
  {
    if (strcmp (cur_node->data->data,data_ptr) == 0)
    {
      return cur_node;
    }
    cur_node = cur_node->next;
  }
  return NULL;
}

Node* add_to_database(MarkovChain *markov_chain, char *data_ptr)
{
  Node* searched_node = get_node_from_database (markov_chain, data_ptr);
  if(searched_node != NULL)
  {
    markov_chain->total_words_read++;
    return searched_node;
  }
  MarkovNode* new_markov_node = malloc (sizeof (MarkovNode));
  if (new_markov_node == NULL)
  {
    return NULL;
  }
  new_markov_node->data = malloc (strlen (data_ptr)+1);
  strcpy (new_markov_node->data, data_ptr);
  new_markov_node->counter_list = NULL;
  new_markov_node->size_of_list = 0;
  if (add (markov_chain->database, new_markov_node) == 1)
  {
    return NULL;
  }
  markov_chain->total_words_read++;
  return markov_chain->database->last;
}

void print_markov_chain(MarkovChain* markov_chain)
{
  Node *cur_node = markov_chain->database->first;
  Node *first_node = cur_node;
  while(cur_node)
  {
    fprintf (stdout, "%s -> ",cur_node->data->data);
    cur_node = cur_node->next;
  }
  fprintf (stdout,"\n");
  cur_node = first_node;
  while (cur_node)
  {
    fprintf (stdout,"%d,%s: ", cur_node->data->size_of_list,
             cur_node->data->data);
    for (int i = 0; i < cur_node->data->size_of_list; i++)
    {
      fprintf (stdout,"%s ,%d", cur_node->data->counter_list[i]
      .markov_node->data, cur_node->data->counter_list[i].frequency);
    }
    fprintf (stdout,"\n");
    cur_node = cur_node->next;
  }
}