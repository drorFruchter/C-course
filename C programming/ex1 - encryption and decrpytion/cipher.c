#include "cipher.h"
#include "ctype.h"

/// IN THIS FILE, IMPLEMENT EVERY FUNCTION THAT'S DECLARED IN cipher.h.

#define TOTAL_LETTERS 26

int fix_k_encode (int k, char cur_char, char first_char);

int fix_k_decode (int k, char cur_char, char first_char);

int fix_k_encode (int k, char cur_char, char first_char)
{
  int new_k;
  if (k < 0)
  {
    k *= -1;
    new_k = k % TOTAL_LETTERS;
    new_k *= -1;
  }
  else
  {
    new_k = k % TOTAL_LETTERS;
  }
  if (cur_char + new_k < first_char)
  {
    return ((cur_char - first_char) + new_k) %
           TOTAL_LETTERS + first_char + TOTAL_LETTERS;
  }
  return ((cur_char - first_char) + new_k) % TOTAL_LETTERS + first_char;
}

int fix_k_decode (int k, char cur_char, char first_char)
{
  int new_k;
  if (k < 0)
  {
    k *= -1;
    new_k = k % TOTAL_LETTERS;
    new_k *= -1;
  }
  else
  {
    new_k = k % TOTAL_LETTERS;
  }
  if (cur_char - new_k < first_char)
  {
    return ((cur_char - first_char) - new_k) %
           TOTAL_LETTERS + first_char + TOTAL_LETTERS;
  }
  return ((cur_char - first_char) - new_k) % TOTAL_LETTERS + first_char;
}

// See full documentation in header file
void encode (char s[], int k)
{
  int i = 0;
  while (s[i] != '\0')
  {
    char cur_char = s[i];
    char enc_char = s[i];
    if (isalpha(s[i]))
    {
      enc_char = (char) fix_k_encode (k, cur_char, 'a');
    }
    if (isupper(s[i]))
    {
      enc_char = (char) fix_k_encode (k, cur_char, 'A');
    }
    s[i] = enc_char;
    i++;
  }
}

// See full documentation in header file
void decode (char s[], int k)
{
  int i = 0;
  while (s[i] != '\0')
  {
    char cur_char = s[i];
    char dec_char = s[i];
    if (isalpha(s[i]))
    {
      dec_char = (char) fix_k_decode (k, cur_char, 'a');
    }
    if (isupper(s[i]))
    {
      dec_char = (char) fix_k_decode (k, cur_char, 'A');
    }
    s[i] = dec_char;
    i++;
  }
}