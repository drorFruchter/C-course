#include "sort_bus_lines.h"
#include "test_bus_lines.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define MAX_INPUT 60
#define MIN_LINE_NUMBER 1
#define MAX_LINE_NUMBER 999
#define MIN_DISTANCE 0
#define MAX_DISTANCE 1000
#define MIN_DURATION 10
#define MAX_DURATION 100

int get_num_of_lines ();
int fill_array (BusLine *bus_arr, int num);
int check_input (int line_number, int distance, int duration);
int run (BusLine *start, BusLine *end, char command[]);
void print_arr (BusLine *start, BusLine *end);
void run_tests (BusLine *start, BusLine *end, BusLine *original_start,
                BusLine *original_end);
void quick_sort_test (BusLine *start, BusLine *end, BusLine *original_start,
                      BusLine *original_end);
void bubble_sort_test (BusLine *start, BusLine *end, BusLine
*original_start, BusLine *original_end);
int command_test (BusLine *start, BusLine *end);

void print_arr (BusLine *start, BusLine *end)
{
  for (BusLine *i = start; i < end; i++)
  {
    fprintf (stdout, "%d,%d,%d\n",
             i->line_number, i->distance, i->duration);
  }
}

int get_num_of_lines ()
{
  char input[MAX_INPUT];
  int num = 0;
  while (num <= 0)
  {
    fprintf (stdout, "Enter number of lines. Then enter\n");
    fgets (input, MAX_INPUT, stdin);
    if (sscanf (input, "%d", &num) == EOF)
    {
      return 0;
    }
    if (num <= 0)
    {
      fprintf (stdout, "ERROR: number of lines has to be positive number\n");
    }
  }
  return num;
}

int check_input (int line_number, int distance, int duration)
{
  if (MIN_LINE_NUMBER > line_number || line_number > MAX_LINE_NUMBER)
  {
    fprintf (stdout,
             "ERROR: Line number should be an integer between 1 and 999 "
             "(includes)\n");
    return EXIT_FAILURE;
  }
  if (MIN_DISTANCE > distance || distance > MAX_DISTANCE)
  {
    fprintf (stdout, "ERROR: Distance should be an integer between 0 and 1000 "
                     "(includes)\n");
    return EXIT_FAILURE;
  }
  if (MIN_DURATION > duration || duration > MAX_DURATION)
  {
    fprintf (stdout, "ERROR: Duration should be an integer between 10 and 100 "
                     "(includes)\n");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int fill_array (BusLine *bus_arr, int num)
{
  int cur_fill = 0;
  while (cur_fill < num)
  {
    char input[MAX_INPUT];
    int line_number, distance, duration;
    fprintf (stdout, "Enter line info. Then enter\n");
    fgets (input, MAX_INPUT, stdin);
    if (sscanf (input, "%d,%d,%d", &line_number, &distance, &duration) == EOF)
    {
      return EXIT_FAILURE;
    }
    if (check_input (line_number, distance, duration) == 0)
    {
      BusLine new_bus = {line_number, distance, duration};
      *bus_arr = new_bus;
      bus_arr++;
      cur_fill++;
    }
  }
  return EXIT_SUCCESS;
}

void bubble_sort_test (BusLine *start, BusLine *end, BusLine
*original_start, BusLine *original_end)
{
  bubble_sort (start, end);
  if (is_sorted_by_distance (start, end) == 0)
  {
    fprintf (stdout, "TEST 1 FAILED: array isn't sorted by distance\n");
  }
  else
  {
    fprintf (stdout, "TEST 1 PASSED: array is sorted by distance\n");
  }
  if (is_equal (start, end, original_start, original_end) == 0)
  {
    fprintf (stdout, "TEST 2 FAILED: sorted array doesn't have the same"
                     " elements\n");
  }
  else
  {
    fprintf (stdout, "TEST 2 PASSED: sorted array have the same "
                     "elements\n");
  }
}

void quick_sort_test (BusLine *start, BusLine *end, BusLine *original_start,
                      BusLine *original_end)
{
  quick_sort (start, end);
  if (is_sorted_by_duration (start, end) == 0)
  {
    fprintf (stdout, "TEST 3 FAILED: array isn't sorted by duration\n");
  }
  else
  {
    fprintf (stdout, "TEST 3 PASSED: array is sorted by duration\n");
  }
  if (is_equal (start, end, original_start, original_end) == 0)
  {
    fprintf (stdout, "TEST 4 FAILED: sorted array doesn't have the same"
                     " elements\n");
  }
  else
  {
    fprintf (stdout, "TEST 4 PASSED: sorted array have the same "
                     "elements\n");
  }
}

void run_tests (BusLine *start, BusLine *end, BusLine *original_start, BusLine
*original_end)
{
  bubble_sort_test (start, end, original_start, original_end);
  quick_sort_test (start, end, original_start, original_end);
}

int command_test (BusLine *start, BusLine *end)
{
  long length = end - start;
  BusLine *original = malloc (length * sizeof (BusLine));
  if (original == NULL)
  {
    return EXIT_FAILURE;
  }
  memcpy (original, start, length * sizeof (BusLine));
  run_tests (start, end, original, original + length);
  free (original);
  original = NULL;
  return EXIT_SUCCESS;
}

int run (BusLine *start, BusLine *end, char command[])
{
  if (strcmp (command, "bubble") == 0)
  {
    bubble_sort (start, end);
    print_arr (start, end);
    return EXIT_SUCCESS;
  }
  if (strcmp (command, "quick") == 0)
  {
    quick_sort (start, end);
    print_arr (start, end);
    return EXIT_SUCCESS;
  }
  if (strcmp (command, "test") == 0)
  {
    return command_test (start, end);
  }
  fprintf (stdout, "USAGE: sort_lines bubble/quick/test");
  return EXIT_FAILURE;
}

int main (int argc, char *argv[])
{
  if (argc > 2)
  {
    fprintf (stdout, "USAGE: sort_lines bubble/quick/test");
    return EXIT_FAILURE;
  }
  int num = get_num_of_lines ();
  if (num == 0)
  {
    return EXIT_FAILURE;
  }
  BusLine *bus_arr = malloc (num * sizeof (*bus_arr));
  if (bus_arr == NULL)
  {
    return EXIT_FAILURE;
  }
  if (fill_array (bus_arr, num) == EXIT_FAILURE)
  {
    free (bus_arr);
    bus_arr = NULL;
    return EXIT_FAILURE;
  }
  if (run (bus_arr, bus_arr + num, argv[1]) == EXIT_FAILURE)
  {
    free (bus_arr);
    bus_arr = NULL;
    return EXIT_FAILURE;
  }
  free (bus_arr);
  bus_arr = NULL;
  return EXIT_SUCCESS;
}
