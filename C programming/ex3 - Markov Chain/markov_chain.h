#ifndef _MARKOV_CHAIN_H
#define _MARKOV_CHAIN_H

#include "linked_list.h"
#include <stdio.h>  // For printf(), sscanf()
#include <stdlib.h> // For exit(), malloc()
#include <stdbool.h> // for bool
#include <string.h> // for strcmp

#define ALLOCATION_ERROR_MASSAGE "Allocation failure: Failed to allocate"\
            "new memory\n"
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
"Usage: tweets_generator seed tweets_to_generate path words_to_read"



/**
 * insert MarkovChain struct
 */
typedef struct MarkovNode {
    char *data;
    int size_of_list;
    struct NextNodeCounter* counter_list;
} MarkovNode;

typedef struct NextNodeCounter {
    struct MarkovNode *markov_node;
    int frequency;
} NextNodeCounter;

typedef struct MarkovChain {
    struct LinkedList *database;
    int total_words_read;
} MarkovChain;


/**
 * Get one random state from the given markov_chain's database.
 * @param markov_chain
 * @return
 */
MarkovNode* get_first_random_node(MarkovChain *markov_chain);

/**
 * Choose randomly the next state, depend on it's occurrence frequency.
 * @param state_struct_ptr MarkovNode to choose from
 * @return MarkovNode of the chosen state
 */
MarkovNode* get_next_random_node(MarkovNode *state_struct_ptr);

/**
 * Receive markov_chain, generate and print random sentence out of it. The
 * sentence most have at least 2 words in it.
 * @param markov_chain
 * @param first_node markov_node to start with,
 *                   if NULL- choose a random markov_node
 * @param  max_length maximum length of chain to generate
 */
void generate_random_sequence(MarkovChain *markov_chain, MarkovNode *
first_node, int max_length);

/**
 * Free markov_chain and all of it's content from memory
 * @param markov_chain markov_chain to free
 */
void free_markov_chain(MarkovChain ** ptr_chain);

/**
 * Add the second markov_node to the counter list of the first markov_node.
 * If already in list, update it's counter value.
 * @param first_node
 * @param second_node
 * @param markov_chain
 * @return success/failure: true if the process was successful, false if in
 * case of allocation error.
 */
bool add_node_to_counter_list(MarkovNode *first_node, MarkovNode *second_node);

/**
* Check if data_ptr is in database. If so, return the markov_node
 * wrapping it in the markov_chain, otherwise return NULL.
 * @param markov_chain the chain to look in its database
 * @param data_ptr the state to look for
 * @return Pointer to the Node wrapping given state, NULL if state not in
 * database.
 */
Node* get_node_from_database(MarkovChain *markov_chain, char *data_ptr);

/**
* If data_ptr in markov_chain, return it's markov_node. Otherwise, create new
 * markov_node, add to end of markov_chain's database and return it.
 * @param markov_chain the chain to look in its database
 * @param data_ptr the state to look for
 * @return markov_node wrapping given data_ptr in given chain's database,
 * returns NULL in case of memory allocation failure.
 */
Node* add_to_database(MarkovChain *markov_chain, char *data_ptr);

int get_random_number(int max_number);
Node* get_i_of_chain(MarkovChain *markov_chain, int index);
int count_all_words(NextNodeCounter* counter_list, int size_of_int);
void free_markov_node(MarkovNode** markov_node);
MarkovNode* get_index_by_frequecny(NextNodeCounter* counter_list, int index,
int max_number);
bool new_node_to_counter_list(MarkovNode* first_node, MarkovNode* second_node);
void print_markov_chain(MarkovChain* markov_chain);

int fill_database(FILE* input_file, int words_to_read,MarkovChain
*markov_chain);
MarkovChain* create_markov_chain();
int read_all_file(FILE *input_file, MarkovChain *markov_chain);
int handle_line(char* line, MarkovChain *markov_chain, int words_to_read);
void tweet_generate(MarkovChain *markov_chain, unsigned int
tweets_to_generate);

#endif /* markovChain_h */
