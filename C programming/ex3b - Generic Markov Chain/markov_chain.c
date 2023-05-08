#include "markov_chain.h"

/**
 * counts total of objects after one object
 * @param counter_list
 * @param size_of_list
 * @return int num of how many objects totally
 */
int count_all_words(NextNodeCounter* counter_list, int size_of_list)
{
  int total = 0;
  for (int i = 0; i < size_of_list; i++)
  {
    total += counter_list[i].frequency;
  }
  return total;
}

/**
 * returning the ith node in chain
 * @param markov_chain
 * @param index
 * @return node in ith place
 */
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

/**
 *
 * @param max_number
 * @return random number between 1 to max number
 */
int get_random_number(int max_number)
{
  return rand() % max_number;
}

/**
 * randomly getting the first node to start from
 * @param markov_chain
 * @return pointer to markov node
 */
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

/**
 * getting the node in right index according to frequencies
 * @param counter_list
 * @param index
 * @param max_number
 * @return pointer to the right markov node
 */
MarkovNode* get_index_by_frequency(NextNodeCounter* counter_list, int index,
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

/**
 * from the state node getting randomly the next node
 * @param state_struct_ptr
 * @return pointer to the next markov node
 */
MarkovNode* get_next_random_node(MarkovNode *state_struct_ptr)
{
  int max_number = count_all_words(state_struct_ptr->counter_list,
                                   state_struct_ptr->size_of_list);
  int index = get_random_number (max_number);
  return get_index_by_frequency(state_struct_ptr->counter_list, index,
                                max_number);
}

/**
 * generate one sequence randomly
 * @param markov_chain
 * @param first_node
 * @param max_length
 */
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
      markov_chain->print_func(first_node->data);
      return;
    }
    markov_chain->print_func(first_node->data);
    first_node = get_next_random_node (first_node);
  }
}

/**
 * frees on markov node and all its content
 * @param ptr_markov_node
 * @param free_data_func
 */
void free_markov_node(MarkovNode** ptr_markov_node, free_data free_data_func)
{
  MarkovNode* markov_node = *ptr_markov_node;
  NextNodeCounter* counter_list = markov_node->counter_list;
  free(counter_list);
  counter_list = NULL;
  free_data_func(markov_node->data);
  free (markov_node);
  markov_node = NULL;
}

/**
 * frees all the chain
 * @param ptr_chain
 */
void free_markov_chain(MarkovChain** ptr_chain)
{
  Node* cur_node = (*ptr_chain)->database->first;
  while(cur_node != NULL)
  {
    free_markov_node(&(cur_node->data), (*ptr_chain)->free_data);
    Node* temp = cur_node;
    cur_node = cur_node->next;
    free (temp);
    temp = NULL;
  }
  free ((*ptr_chain)->database);
  free(*ptr_chain);
}

/**
 * adding new memory to the list and adding new node to it
 * @param first_node
 * @param second_node
 * @return true if succeed, false else
 */
bool new_node_to_counter_list(MarkovNode* first_node, MarkovNode*
second_node)
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

/**
 * adding node to counter list of previous node
 * @param first_node
 * @param second_node
 * @param markov_chain
 * @return true if succeed, false else
 */
bool add_node_to_counter_list(MarkovNode *first_node, MarkovNode
*second_node, MarkovChain* markov_chain)
{
  if(markov_chain->is_last(first_node->data))
  {
//    last_in_chain = true;
    return true;
  }
  NextNodeCounter* counter_list = first_node->counter_list;
  comp_func comp_func1 = markov_chain->comp_func;
  for (int i = 0; i < first_node->size_of_list; i++)
  {
    if (comp_func1(counter_list[i].markov_node->data,second_node->data) == 0)
    {
      counter_list[i].frequency++;
      return true;
    }
  }
  return new_node_to_counter_list(first_node, second_node);
}

/**
 * checking if data is already in chain
 * @param markov_chain
 * @param data_ptr
 * @return the right node if the data in it, NULL else
 */
Node* get_node_from_database(MarkovChain *markov_chain, void *data_ptr)
{
  Node* cur_node = markov_chain->database->first;
  while (cur_node)
  {
    if (markov_chain->comp_func(cur_node->data->data,data_ptr) == 0)
    {
      return cur_node;
    }
    cur_node = cur_node->next;
  }
  return NULL;
}

/**
 * adding node to the database
 * @param markov_chain
 * @param data_ptr
 * @return the right node if it was already there, else adding new one
 * NULL if didn't succeed
 */
Node* add_to_database(MarkovChain *markov_chain, void *data_ptr)
{
  Node* searched_node = get_node_from_database (markov_chain, data_ptr);
  if(searched_node != NULL)
  {
    total_data_read++;
    return searched_node;
  }
  MarkovNode* new_markov_node = malloc (sizeof (MarkovNode));
  if (new_markov_node == NULL)
  {
    fprintf (stdout,ALLOCATION_ERROR_MASSAGE);
    return NULL;
  }
  new_markov_node->data = markov_chain->copy_func(data_ptr);
  if (new_markov_node->data == NULL)
  {
    fprintf (stdout,ALLOCATION_ERROR_MASSAGE);
    return NULL;
  }
  new_markov_node->counter_list = NULL;
  new_markov_node->size_of_list = 0;
  if (add (markov_chain->database, new_markov_node) == 1)
  {
    fprintf (stdout,ALLOCATION_ERROR_MASSAGE);
    return NULL;
  }
  total_data_read++;
  return markov_chain->database->last;
}