#include <stdio.h>
#include <stdlib.h>
#include "singular.h"
#define filterTypeCount 6

const int filter_types_count = filterTypeCount;
char *filter_types[filterTypeCount] = {"EQ", "NEQ", "GEQ", "LEQ", "LESS", "GREATER"};

void formatedDoublePrint(double num, int prec) {
    if (prec == -1) {
        printf("%f ", num);
    } else {
        printf("%.*f ", output_prec, num);
    }
}

void print(double a[], size_t size) {
    if (size == 0 || a == NULL) {return;}
    for (int i=0; i < size; i++) {
        formatedDoublePrint(a[i], output_prec);
    }
    printf("\n");
}

void shift(double a[], size_t size, double by) {
    for (int i=0; i < size; i++) {
        a[i] += by;
    }
}

// Need to resize the array with realloc after this func returns
size_t filter(double a[], size_t count, enum filter_type t, double threshold) {
    int matches = 0;
    for (int i=0; i < count; i++) {
        switch(t) {
            case EQ:
                if (a[i] == threshold) {
                    a[matches] = a[i];
                    matches++;
                }
                break; 
            case NEQ:
                if (a[i] != threshold) {
                    a[matches] = a[i];
                    matches++;
                }
                break;     
            case GEQ:
                if (a[i] >= threshold) {
                    a[matches] = a[i];
                    matches++;
                }
                break; 
            case LEQ:
                if (a[i] <= threshold) {
                    a[matches] = a[i];
                    matches++;
                }
                break; 
            case LESS:
                if (a[i] < threshold) {
                    a[matches] = a[i];
                    matches++;
                }
                break; 
            case GREATER:
                if (a[i] > threshold) {
                    a[matches] = a[i];
                    matches++;
                }
                break; 
            default:
                break;
        }
    }
    return matches;
}