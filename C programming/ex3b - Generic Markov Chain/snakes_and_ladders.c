#include <string.h> // For strlen(), strcmp(), strcpy()
#include "markov_chain.h"

#define MAX(X, Y) (((X) < (Y)) ? (Y) : (X))

#define EMPTY -1
#define BOARD_SIZE 100
#define MAX_GENERATION_LENGTH 60
#define BASE_10 10

#define DICE_MAX 6
#define NUM_OF_TRANSITIONS 20
#define USAGE_MESSAGE "Usage: snakes_and_ladders <seed> <num_of_trails>"


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
typedef struct Cell {
    int number; // Cell number 1-100
    int ladder_to;  // ladder_to represents the jump of the ladder
    // in case there is one from this square
    int snake_to;  // snake_to represents the jump of the snake
    // in case there is one from this square
    //both ladder_to and snake_to should be -1 if the Cell doesn't have them
} Cell;

/** Error handler **/
static int handle_error(char *error_msg, MarkovChain **database)
{
    printf("%s", error_msg);
    if (database != NULL)
    {
        free_markov_chain(database);
    }
    return EXIT_FAILURE;
}


static int create_board(Cell *cells[BOARD_SIZE])
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        cells[i] = malloc(sizeof(Cell));
        if (cells[i] == NULL)
        {
            for (int j = 0; j < i; j++) {
                free(cells[j]);
            }
            handle_error(ALLOCATION_ERROR_MASSAGE,NULL);
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
static int fill_database(MarkovChain *markov_chain)
{
    Cell* cells[BOARD_SIZE];
    if(create_board(cells) == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }
    MarkovNode *from_node = NULL, *to_node = NULL;
    size_t index_to;
    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        add_to_database(markov_chain, cells[i]);
    }

    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        from_node = get_node_from_database(markov_chain,cells[i])->data;

        if (cells[i]->snake_to != EMPTY || cells[i]->ladder_to != EMPTY)
        {
            index_to = MAX(cells[i]->snake_to,cells[i]->ladder_to) - 1;
            to_node = get_node_from_database(markov_chain, cells[index_to])
                    ->data;
            add_node_to_counter_list(from_node, to_node, markov_chain);
        }
        else
        {
            for (int j = 1; j <= DICE_MAX; j++)
            {
                index_to = ((Cell*) (from_node->data))->number + j - 1;
                if (index_to >= BOARD_SIZE)
                {
                    break;
                }
                to_node = get_node_from_database(markov_chain, cells[index_to])
                        ->data;
                add_node_to_counter_list(from_node, to_node, markov_chain);
            }
        }
    }
    // free temp arr
    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        free(cells[i]);
    }
    return EXIT_SUCCESS;
}

/**
 * copies the data of one cell to new allocated memory
 * @param data_ptr
 * @return pointer to the new cell in memory
 */
static void* sal_copy_func(void* data_ptr)
{
  Cell* cell_ptr = (Cell*)data_ptr;
  Cell* new_cell_ptr = malloc (sizeof (*cell_ptr));
  if (new_cell_ptr == NULL)
  {
    return NULL;
  }
  memcpy(new_cell_ptr, cell_ptr, sizeof (*cell_ptr));
  return (void*)new_cell_ptr;
}

/**
 * compares between two cells number
 * @param ptr1
 * @param ptr2
 * @return 0 if equal, negative if cell1 is smaller, positive else
 */
static int sal_comp_func(void* ptr1, void* ptr2)
{
  const Cell *c1 = (Cell*)ptr1;
  const Cell *c2 = (Cell*)ptr2;
  return c1->number - c2->number;
}

/**
 * checking if the pointer points to last cell in chain(cell 100)
 * @param data_ptr
 * @return true if it's last, false else
 */
static bool sal_is_last(void* data_ptr)
{
  Cell* c1 = (Cell*)data_ptr;
  return (c1->number == BOARD_SIZE);
}

/**
 * prints the data in the cell
 * @param data_ptr
 */
static void sal_print(void* data_ptr)
{
  Cell* c1 = (Cell*)data_ptr;
  fprintf (stdout,"[%d]", c1->number);
  if (c1->ladder_to != -1)
  {
    fprintf (stdout, "-ladder to %d",c1->ladder_to);
  }
  if (c1->snake_to != -1)
  {
    fprintf (stdout,"-snake to %d", c1->snake_to);
  }
  if (c1->number != BOARD_SIZE)
  {
    fprintf (stdout," -> ");
  }
}

/**
 * initializing the markov chain with all the relevant data
 * @return pointer to the chain
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
  markov_chain->copy_func = sal_copy_func;
  markov_chain->comp_func = sal_comp_func;
  markov_chain->is_last = sal_is_last;
  markov_chain->free_data = free;
  markov_chain->print_func = sal_print;
  return markov_chain;
}

/**
 * genarating walks according to the data in the chain and print them
 * @param markov_chain
 * @param num_of_trails
 */
static void generate_trails(MarkovChain* markov_chain,int num_of_trails)
{
  for(int i = 0; i < num_of_trails; i++)
  {
    fprintf (stdout, "Random Walk %d: ", i+1);
    generate_random_sequence(markov_chain,markov_chain->database->first->data,
                             MAX_GENERATION_LENGTH);
    fprintf (stdout,"\n");
  }
}


/**
 * @param argc num of arguments
 * @param argv 1) Seed
 *             2) Number of sentences to generate
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    handle_error (USAGE_MESSAGE, NULL);
  }
  MarkovChain* markov_chain = create_markov_chain();
  if (markov_chain == NULL)
  {
    handle_error (ALLOCATION_ERROR_MASSAGE, &markov_chain);
  }
  if (fill_database (markov_chain) == EXIT_FAILURE)
  {
    handle_error (ALLOCATION_ERROR_MASSAGE, &markov_chain);
  }
  int seed = (int) strtol (argv[1], NULL, BASE_10);
  srand (seed);
  int num_of_trails = (int) strtol (argv[2], NULL, BASE_10);
  generate_trails(markov_chain, num_of_trails);
  free_markov_chain (&markov_chain);
  return EXIT_SUCCESS;
}
