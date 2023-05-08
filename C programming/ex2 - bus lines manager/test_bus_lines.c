#include "test_bus_lines.h"
#include "sort_bus_lines.h"

#define FALSE 0
#define TRUE 1

int is_sorted_by_distance (BusLine *start, BusLine *end)
{
  while(start < (end - 1))
  {
    BusLine *next = start + 1;
    if (start -> distance > next -> distance)
    {
      return FALSE;
    }
    start++;
  }
  return TRUE;
}

int is_equal (BusLine *start_sorted, BusLine *end_sorted,
              BusLine *start_original, BusLine *end_original)
{
  long length_of_sorted = end_sorted - start_sorted;
  long length_of_original = end_original - start_original;
  if (length_of_original != length_of_sorted)
  {
    return FALSE;
  }
  for (BusLine *i = start_original; i < end_original; i++)
  {
    int is_found = FALSE;
    for (BusLine *j = start_sorted; j < end_sorted; j++)
    {
      if (i->line_number == j->line_number)
      {
        is_found = TRUE;
        break;
      }
    }
    if (is_found == FALSE)
    {
      return FALSE;
    }
  }
  return TRUE;
  }

int is_sorted_by_duration (BusLine *start, BusLine *end)
{
  while(start < (end - 1))
  {
    BusLine *next = start + 1;
    if (start -> duration > next -> duration)
    {
      return FALSE;
    }
    start++;
  }
  return TRUE;
}