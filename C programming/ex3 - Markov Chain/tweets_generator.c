#include "stdlib.h"
#include "stdio.h"
#include "linked_list.h"
#include "markov_chain.h"


void tweet_generate(MarkovChain *markov_chain, unsigned int
tweets_to_generate)
{
  for (unsigned int i = 1; i <= tweets_to_generate; i++)
  {
    fprintf (stdout,"Tweet %d: ", i);
    generate_random_sequence (markov_chain, NULL, MAX_LENGTH);
    fprintf (stdout, "\n");
  }
}

MarkovChain* create_markov_chain()
{
  MarkovChain *markov_chain = malloc (sizeof (MarkovChain));
  LinkedList* linked_list = malloc (sizeof (LinkedList));
  if (markov_chain == NULL || linked_list == NULL)
  {
    fprintf (stdout,ALLOCATION_ERROR_MASSAGE);
    return NULL;
  }
  *linked_list = (LinkedList){NULL, NULL, 0};
  markov_chain->database = linked_list;
  markov_chain->total_words_read = 0;
  return markov_chain;
}

int handle_line(char* line, MarkovChain *markov_chain, int words_to_read)
{
  char* word = strtok (line, DELIM);
  Node *last_node = NULL;
  while(word)
  {
    Node *new_node = add_to_database (markov_chain, word);
    if (last_node && new_node) //TODO function
    {
      if (add_node_to_counter_list (last_node->data, new_node->data) == false)
      {
        return EXIT_FAILURE;
      }
    }
    last_node = (word[strlen (word)-1] == '.') ? NULL :new_node;
//    if (word[strlen (word)-1] != '.')
//    {
//      last_node = new_node;
//    }
//    else
//    {
//      last_node = NULL;
//    }
    word = strtok (NULL, DELIM);
    if(words_to_read != -1 && markov_chain->total_words_read >= words_to_read)
    {
      return EXIT_SUCCESS;
    }
  }
  return EXIT_SUCCESS;
}


int read_all_file(FILE *input_file, MarkovChain *markov_chain)
{
  char line[MAX_FOR_LINE];
  while (fgets (line, MAX_FOR_LINE, input_file) != NULL)
  {
//    fprintf (stdout,"%s\n", line);
    if (handle_line(line, markov_chain,-1) == EXIT_FAILURE)
    {
      return EXIT_FAILURE;
    }
//    print_markov_chain (markov_chain);
  }
  return EXIT_SUCCESS;
}

int fill_database(FILE* input_file, int words_to_read, MarkovChain
*markov_chain)
{
  if (words_to_read == -1)
  {
    return read_all_file(input_file, markov_chain);
  }
  char line[MAX_FOR_LINE];
  while (fgets (line, MAX_FOR_LINE, input_file) != NULL &&
  markov_chain->total_words_read < words_to_read)
  {
    if (handle_line (line, markov_chain, words_to_read) == EXIT_FAILURE)
    {
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}

void run_program (char *const *argv, MarkovChain **markov_chain)
{
  unsigned int seed = strtol (SEED, NULL, BASE);  //TODO function
  unsigned int tweets_to_generate = strtol (TWEETS_TO_GENERATE, NULL, BASE);
  srand (seed);
  tweet_generate((*markov_chain), tweets_to_generate);
  free_markov_chain (markov_chain);
}
int main(int argc, char *argv[])
{
  if (CHECK_NUM_OF_ARGS)
  {
    fprintf (stdout,USAGE_MESSAGE);
    return EXIT_FAILURE;
  }
  FILE* input_file = fopen (PATH,"r");
  if (input_file == NULL)
  {
    fprintf (stdout,"Error: file path is invalid");
    return EXIT_FAILURE;
  }
  MarkovChain *markov_chain = create_markov_chain();
  if (markov_chain == NULL)
  {
    return EXIT_FAILURE;
  }
  if (argc == 4) //TODO function
  {
    if (fill_database(input_file, -1, markov_chain) == EXIT_FAILURE)
    {
      return EXIT_FAILURE;
    }
  }
  else
  {
    int words_to_read = (int)strtol (WORDS_TO_READ, NULL, BASE);
    if (fill_database(input_file, words_to_read, markov_chain) == EXIT_FAILURE)
    {
      return EXIT_FAILURE;
    }
  }
  print_markov_chain (markov_chain);
  run_program (argv, &markov_chain);
  fclose (input_file);
  return EXIT_SUCCESS;
}