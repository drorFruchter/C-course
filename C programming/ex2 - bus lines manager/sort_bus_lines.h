#ifndef EX2_REPO_SORTBUSLINES_H
#define EX2_REPO_SORTBUSLINES_H
// write only between #define EX2_REPO_SORTBUSLINES_H
// and #endif //EX2_REPO_SORTBUSLINES_H
/**
 * struct of BUS LINE which includes three values
 * line number, distance and duration
 */
typedef struct BusLine
{
    int line_number, distance, duration;
} BusLine;

/**
 * sorting array of bus lines by distance using bubble sort
 */
void bubble_sort (BusLine *start, BusLine *end);

/**
 * sorting array of bus lines by duration using quick sort
 */
void quick_sort (BusLine *start, BusLine *end);

/**
 * partition used by quick sort to put pivot value in the right place
 */
BusLine *partition (BusLine *start, BusLine *end);
// write only between #define EX2_REPO_SORTBUSLINES_H
// and #endif //EX2_REPO_SORTBUSLINES_H
#endif //EX2_REPO_SORTBUSLINES_H
