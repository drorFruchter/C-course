#include "sort_bus_lines.h"


void swap(BusLine *b1, BusLine *b2);

void swap(BusLine *b1, BusLine *b2)
{
  BusLine tmp = *b1;
  *b1 = *b2;
  *b2 = tmp;
}

void bubble_sort (BusLine *start, BusLine *end)
{
  for (BusLine *j=start; j < end - 1; j++)
  {
    for(BusLine *i=start; i < end - 1; i++)
    {
      if(i->distance > (i+1)->distance)
      {
        swap (i, i+1);
      }
    }
  }
}

void quick_sort (BusLine *start, BusLine *end)
{
  if (start < end)
  {
    BusLine *pivot = partition (start, end);
    quick_sort (start, pivot);
    quick_sort (pivot+1, end);
  }
}

BusLine *partition (BusLine *start, BusLine *end)
{
  BusLine *pivot = end;
  pivot--;
  BusLine *left = start-1;
  for (BusLine *i = start; i < pivot; i++)
  {
    if (i->duration <= pivot->duration)
    {
      left++;
      swap (left,i);
    }
  }
  left++;
  swap (left,pivot);
  return left;
}