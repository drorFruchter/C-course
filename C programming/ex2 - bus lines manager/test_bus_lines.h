#ifndef EX2_REPO_TESTBUSLINES_H
#define EX2_REPO_TESTBUSLINES_H
// write only between #define EX2_REPO_TESTBUSLINES_H
// and #endif //EX2_REPO_TESTBUSLINES_H
#include "sort_bus_lines.h"
/**
 * checking if the array is sorted by distance after bubble sort
 */
int is_sorted_by_distance (BusLine *start, BusLine *end);
/**
 * checking if the array is sorted by duration after quick sort
 */
int is_sorted_by_duration (BusLine *start, BusLine *end);
/**
 * chceking if two arrays has the same values based on the line number
 */
int is_equal (BusLine *start_sorted,
              BusLine *end_sorted, BusLine *start_original,
              BusLine *end_original);
// write only between #define EX2_REPO_TESTBUSLINES_H
// and #endif //EX2_REPO_TESTBUSLINES_H
#endif //EX2_REPO_TESTBUSLINES_H
