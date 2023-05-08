#ifndef _MARKOV_CHAIN_H
#define _MARKOV_CHAIN_H

#include "linked_list.h"
#include <stdio.h>  // For printf(), sscanf()
#include <stdlib.h> // For exit(), malloc()
#include <stdbool.h> // for bool

#define \
ALLOCATION_ERROR_MASSAGE "Allocation failure: Failed to allocate new memory\n"


/***************************/
/*   insert typedefs here  */
/***************************/
typedef void (*print_func)(void*);
typedef int (*comp_func)(void*, void*);
typedef void (*free_data)(void*);
typedef void* (*copy_func)(void*);
typedef bool (*is_last)(void*);

/***************************/


/***************************/
/*        STRUCTS          */
/***************************/

typedef struct MarkovNode {
    void* data;
    int size_of_list;
    struct NextNodeCounter* counter_list;
} MarkovNode;

typedef struct NextNodeCounter {
    struct MarkovNode *markov_node;
    int frequency;
} NextNodeCounter;

/* DO NOT ADD or CHANGE variable names in this struct */
typedef struct MarkovChain {
    LinkedList *database;

    // pointer to a func that receives data from a generic type and prints it
    // returns void.
    print_func print_func;

    // pointer to a func that gets 2 pointers of generic
    // data type(same one) and compare between them */
    // returns: - a positive value if the first is bigger
    //          - a negative value if the second is bigger
    //          - 0 if equal
    comp_func comp_func;

    // a pointer to a function that gets a pointer
    // of generic data type and frees it.
    // returns void.
    free_data free_data;

    // a pointer to a function that  gets a pointer of generic
    // data type and returns a newly allocated copy of it
    // returns a generic pointer.
    copy_func copy_func;

    //  a pointer to function that gets a pointer
    //
    //
    //
    //  of generic data type and returns:
    //      - true if it's the last state.
    //      - false otherwise.
    is_last is_last;
} MarkovChain;

int total_data_read;
//static bool last_in_chain = false;

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
 * if NULL- choose a random markov_node
 * @param  max_length maximum length of chain to generate
 */
void generate_random_sequence(MarkovChain *markov_chain, MarkovNode *
first_node, int max_length);

/**
 * Free markov_chain and all of it's content from memory
 * @param markov_chain markov_chain to free
 */
void free_markov_chain(MarkovChain **markov_chain);

/**
 * Add the second markov_node to the counter list of the first markov_node.
 * If already in list, update it's counter value.
 * @param first_node
 * @param second_node
 * @param markov_chain
 * @return success/failure: true if the process was successful, false if in
 * case of allocation error.
 */
bool add_node_to_counter_list(MarkovNode *first_node, MarkovNode
*second_node, MarkovChain *markov_chain);

/**
* Check if data_ptr is in database. If so,
 * return the markov_node wrapping it in
 * the markov_chain, otherwise return NULL.
 * @param markov_chain the chain to look in its database
 * @param data_ptr the state to look for
 * @return Pointer to the Node wrapping given state, NULL if state not in
 * database.
 */
Node* get_node_from_database(MarkovChain *markov_chain, void *data_ptr);

/**
* If data_ptr in markov_chain, return it's markov_node. Otherwise, create new
 * markov_node, add to end of markov_chain's database and return it.
 * @param markov_chain the chain to look in its database
 * @param data_ptr the state to look for
 * @return markov_node wrapping given data_ptr in given chain's database
 */
Node* add_to_database(MarkovChain *markov_chain, void *data_ptr);

/**
 * using rand to get num from 1 to max_number
 * @param max_number
 * @return random int
 */
int get_random_number(int max_number);

/**
 * returning the ith node in chain
 * @param markov_chain
 * @param index
 * @return pointer to the node at the ith place
 */
Node* get_i_of_chain(MarkovChain *markov_chain, int index);

/**
 * counting all the next counter with all the frequencies
 * @param counter_list
 * @param size_of_int
 * @return sum of total in the list
 */
int count_all_words(NextNodeCounter* counter_list, int size_of_int);

/**
 * frees specific markov node
 * @param markov_node
 */
void free_markov_node(MarkovNode** markov_node, free_data);

/**
 * returning the ith markov node by its frequency
 * @param counter_list
 * @param index
 * @param max_number
 * @return pointer to markov node
 */
MarkovNode* get_index_by_frequency(NextNodeCounter* counter_list, int index,
                                   int max_number);

/**
 * addind new node to the counter list
 * @param first_node
 * @param second_node
 * @return true if succeed, false else
 */
bool new_node_to_counter_list(MarkovNode* first_node, MarkovNode* second_node);

/**
 *
 * @param input_file
 * @param words_to_read
 * @param markov_chain
 * @return
 */

#endif /* _MARKOV_CHAIN_H */
