#include "stdlib.h"
#include "stdio.h"
#include "linked_list.h"
#include "markov_chain.h"
#include "string.h"

#define SEED argv[1]
#define TWEETS_TO_GENERATE argv[2]
#define PATH argv[3]
#define WORDS_TO_READ argv[4]
#define MAX_FOR_LINE 1000
#define DELIM " \n\r"
#define MAX_LENGTH 20
#define CHECK_NUM_OF_ARGS argc != 4 && argc != 5
#define BASE 10
#define USAGE_MESSAGE \
"Usage: tweets_generator <seed> <tweets_to_generate> <path> <words_to_read>"

/**
 * copying the data in data_ptr to new allocated memory
 * @param data_ptr
 * @return pointer to the copied memory
 */
static void* tweet_copy_func(void* data_ptr)
{
  char* word_ptr = (char*)data_ptr;
  char* new_word_ptr = malloc (strlen (word_ptr)+1);
  if (new_word_ptr == NULL)
  {
    return NULL;
  }
  strcpy (new_word_ptr,word_ptr);
  return (void*)new_word_ptr;
}

/**
 * compares between two strings
 * @param ptr1
 * @param ptr2
 * @return 0 if equal, not 0 else
 */
static int tweet_comp_func(void* ptr1, void* ptr2)
{
  const char *s1 = (char*)ptr1;
  const char *s2 = (char*)ptr2;
  return strcmp (s1, s2);
}

/**
 * checking if the pointer points to data which is last in tweet
 * @param data_ptr
 * @return true if so, false else
 */
static bool tweet_is_last(void* data_ptr)
{
  char* word = (char*)data_ptr;
  return (word[strlen (word)-1] == '.') ? true :false;
}

/**
 * print the tweet data
 * @param data_ptr
 */
static void tweet_print(void* data_ptr)
{
  char* word = (char*)data_ptr;
  fprintf (stdout,"%s", word);
  if (word[strlen (word)-1] != '.')
  {
    fprintf (stdout," ");
  }
}

/**
 * generating <tweets_to_generate> tweets and prints them
 * @param markov_chain
 * @param tweets_to_generate
 */
static void tweet_generate(MarkovChain *markov_chain, unsigned int
tweets_to_generate)
{
  for (unsigned int i = 1; i <= tweets_to_generate; i++)
  {
    fprintf (stdout,"Tweet %d: ", i);
    generate_random_sequence (markov_chain, NULL, MAX_LENGTH);
    fprintf (stdout, "\n");
  }
}

/**
 * initializing the markov chain
 * @return pointer to markov chain
 */
static MarkovChain* create_markov_chain()
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
  markov_chain->copy_func = tweet_copy_func;
  markov_chain->comp_func = tweet_comp_func;
  markov_chain->is_last = tweet_is_last;
  markov_chain->free_data = free;
  markov_chain->print_func = tweet_print;
  return markov_chain;
}

/**
 * handling one line. split to words and adding to database
 * @param line
 * @param markov_chain
 * @param words_to_read
 * @return 0 if succeed, 1 else
 */
static int handle_line(char* line, MarkovChain *markov_chain,
                       int words_to_read)
{
  char* word = strtok (line, DELIM);
  Node *new_node = add_to_database (markov_chain, word);
  Node *last_node = new_node;
  word = strtok (NULL, DELIM);
  do
  {
    new_node = add_to_database (markov_chain, (void*)word);
    if (add_node_to_counter_list (last_node->data, new_node->data,
                                  markov_chain) == false)
      {
        return EXIT_FAILURE;
      }
    last_node = new_node;
    word = strtok (NULL, DELIM);
    if(words_to_read != -1 && total_data_read >= words_to_read)
    {
      return EXIT_SUCCESS;
    }
  }while(word);
  return EXIT_SUCCESS;
}

/**
 * read all the file until reaches E0F
 * @param input_file
 * @param markov_chain
 * @return 0 if succeed, 1 else
 */
static int read_all_file(FILE *input_file, MarkovChain *markov_chain)
{
  char line[MAX_FOR_LINE];
  while (fgets (line, MAX_FOR_LINE, input_file) != NULL)
  {
    if (handle_line(line, markov_chain,-1) == EXIT_FAILURE)
    {
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}

/**
 * filling all the database in markov chain
 * @param input_file
 * @param words_to_read
 * @param markov_chain
 * @return 0 if succeed, 1 else
 */
static int fill_database(FILE* input_file, int words_to_read, MarkovChain
*markov_chain)
{
  if (words_to_read == -1)
  {
    return read_all_file(input_file, markov_chain);
  }
  char line[MAX_FOR_LINE];
  while (fgets (line, MAX_FOR_LINE, input_file) != NULL &&
         total_data_read < words_to_read)
  {
    if (handle_line (line, markov_chain, words_to_read) == EXIT_FAILURE)
    {
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}

/**
 * checking which type of filling we need and calling fill_database func
 * @param argc
 * @param markov_chain
 * @param input_file
 * @param arg4
 * @return 0 if succeed, 1 else
 */
static int fill_database_via_argc(int argc, MarkovChain* markov_chain, FILE*
input_file, char* arg4)
{
  if (argc == 4)
  {
    if (fill_database(input_file, -1, markov_chain) == EXIT_FAILURE)
    {
      free_markov_chain (&markov_chain);
      fclose(input_file);
      return EXIT_FAILURE;
    }
  }
  else
  {
    int words_to_read = (int)strtol (arg4, NULL, BASE);
    if (fill_database(input_file, words_to_read, markov_chain) == EXIT_FAILURE)
    {
      free_markov_chain (&markov_chain);
      fclose(input_file);
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}

/**
 * the actual manager of tweets generating
 * @param argv
 * @param markov_chain
 */
static void run_program (char *const *argv, MarkovChain **markov_chain)
{
  unsigned int seed = strtol (SEED, NULL, BASE);
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
  if (fill_database_via_argc (argc, markov_chain,
                              input_file,WORDS_TO_READ) == EXIT_FAILURE)
  {
    return EXIT_FAILURE;
  }
  run_program (argv, &markov_chain);
  fclose (input_file);
  return EXIT_SUCCESS;
}