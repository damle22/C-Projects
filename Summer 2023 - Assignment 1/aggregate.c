#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "aggregate.h"
#define LENGTH(x) (sizeof(x)/sizeof(x[0]))

static double _count(double* arr, int size) {
    return size;
}

static double _min(double* arr, int size) {
    // if (size == 0 || arr == NULL) {return;} 
    double min = arr[0];
    for (int i=1; i < size; i++) {
        if (arr[i] < min) {
            min = arr[i];
        }
    }
    return min;
}

static double _max(double* arr, int size) {
    double max = arr[0];
    for (int i=1; i < size; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}

static double _sum(double* arr, int size) {
    double sum = 0;
    for (int i=0; i < size; i++) {
        sum += arr[i];
    }
    return sum;
}

static double _avg(double* arr, int size) {
    return _sum(arr, size) / size;
}

static double _pavg(double* arr, int size) {
    return (_min(arr, size) + _max(arr, size)) / 2;
}

// The following declares an array of pointer
typedef double (*aggregate_func[])(double*, int);
aggregate_func farray = {&_count, &_min, &_max, &_sum, &_avg, &_pavg};
// The following declares an array of strings
static const char* funcnames[] = {"COUNT", "MIN", "MAX", "SUM", "AVG", "PAVG"};

double aggregate(const char* func, double* arr, int size) {
    if (arr == NULL || size < 0) {
        fprintf (stderr, "Fatal Error: inputed array is Null or size is negative, in file: %s, on line: %d.\n", __FILE__, __LINE__);
        exit(1);
    }

    for (int i=0; i < LENGTH(funcnames); i++) {
        if (strcmp(func, funcnames[i]) == 0) {
            return (*farray[i])(arr, size);
        }
    }
    
    // No aggregate function with that name was found, diplay an error
    fprintf (stderr, "Fatal Error: no function with the name %s was found, in file: %s, on line: %d.\n", func, __FILE__, __LINE__);
    exit(1);
}