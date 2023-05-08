#include "cipher.h"
#include "tests.h"
#include "string.h"
#include <stdlib.h>
#include <stdio.h>
#include "ctype.h"

#define BUFFER_LENGTH 40
#define WRONG_NUMBER_OF_ARGUMENTS \
"The program receives 1 or 4 arguments only.\n"
#define WRONG_TEST "Usage: cipher test\n"
#define COMMAND_INVALID "The given command is invalid.\n"
#define SHIFT_INVALID "The given shift value is invalid.\n"
#define FILE_INVALID "The given file is invalid.\n"
#define COMMAND argv[1]
#define SHIFT argv[2]
#define INPUT_DIR argv[3]
#define OUTPUT_DIR argv[4]
#define CORRECT_NUM_OF_ARG 5
#define CORRECT_NUM_OF_ARG_TEST 2
#define BASE_10 10

int tests ();
int check_input (int argc, char *argv[]);
int check_open_files (FILE *input_file, FILE *output_file);
int check_k_is_number (const char num_str[]);
void encode_file (FILE *input_file, FILE *output_file, int k);
void decode_file (FILE *input_file, FILE *output_file, int k);

void encode_file (FILE *input_file, FILE *output_file, int k)
{
  char input[BUFFER_LENGTH];
  while (fgets (input, sizeof (input), input_file) != NULL)
  {
    encode (input, k);
    fprintf (output_file, "%s", input);
  }
}

void decode_file (FILE *input_file, FILE *output_file, int k)
{
  char input[BUFFER_LENGTH];
  while (fgets (input, sizeof (input), input_file) != NULL)
  {
    decode (input, k);
    fprintf (output_file, "%s", input);
  }
}

int check_k_is_number (const char num_str[])
{
  int i = 0;
  if (num_str[i] == '-')
  {
    i++;
  }
  while (num_str[i] != '\0')
  {
    if (isdigit(num_str[i]) == 0)
    {
      return EXIT_FAILURE;
    }
    i++;
  }
  return EXIT_SUCCESS;
}

int check_open_files (FILE *input_file, FILE *output_file)
{
  if (input_file != NULL && output_file != NULL)
  {
    return EXIT_SUCCESS;
  }
  else
  {
    if (input_file != NULL)
    {
      fclose (input_file);
    }
    if (output_file != NULL)
    {
      fclose (output_file);
    }
    return EXIT_FAILURE;
  }
}

int check_input (int argc, char *argv[])
{
  if (argc != CORRECT_NUM_OF_ARG && argc != CORRECT_NUM_OF_ARG_TEST)
  {
    fprintf (stderr, WRONG_NUMBER_OF_ARGUMENTS);
    return EXIT_FAILURE;
  }
  if (argc == CORRECT_NUM_OF_ARG_TEST && strcmp (COMMAND, "test") != 0)
  {
    fprintf (stderr, WRONG_TEST);
    return EXIT_FAILURE;
  }
  if (argc == CORRECT_NUM_OF_ARG)
  {
    if (strcmp (COMMAND, "encode") != 0 && strcmp (COMMAND, "decode") != 0)
    {
      fprintf (stderr, COMMAND_INVALID);
      return EXIT_FAILURE;
    }
    if (check_k_is_number (SHIFT) != 0)
    {
      fprintf (stderr, SHIFT_INVALID);
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}

int tests ()
{
  int num_errors = 0;
  num_errors += test_encode_non_cyclic_lower_case_positive_k ();
  num_errors += test_encode_cyclic_lower_case_special_char_positive_k ();
  num_errors += test_encode_non_cyclic_lower_case_special_char_negative_k ();
  num_errors += test_encode_cyclic_lower_case_negative_k ();
  num_errors += test_encode_cyclic_upper_case_positive_k ();
  num_errors += test_decode_non_cyclic_lower_case_positive_k ();
  num_errors += test_decode_cyclic_lower_case_special_char_positive_k ();
  num_errors += test_decode_non_cyclic_lower_case_special_char_negative_k ();
  num_errors += test_decode_cyclic_lower_case_negative_k ();
  num_errors += test_decode_cyclic_upper_case_positive_k ();

  if (num_errors != 0)
  {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
// your code goes here

int main (int argc, char *argv[])
{
  if (check_input (argc, argv) != 0)
  {
    return EXIT_FAILURE;
  }
  if (argc == CORRECT_NUM_OF_ARG_TEST && strcmp (COMMAND, "test") == 0)
  {
    if (tests () != 0)
    {
      return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
  }
  FILE *input_file = fopen (INPUT_DIR, "r");
  FILE *output_file = fopen (OUTPUT_DIR, "w");
  if (check_open_files (input_file, output_file) != 0)
  {
    fprintf (stderr, FILE_INVALID);
    return EXIT_FAILURE;
  }
  int k = (int) strtol (SHIFT, &INPUT_DIR, BASE_10);

  if (strcmp (COMMAND, "encode") == 0)
  {
    encode_file (input_file, output_file, k);
  }
  if (strcmp (COMMAND, "decode") == 0)
  {
    decode_file (input_file, output_file, k);
  }
  fclose (input_file);
  fclose (output_file);
  return EXIT_SUCCESS;
}
