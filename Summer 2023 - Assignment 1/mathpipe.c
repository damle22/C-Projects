#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <unistd.h>
#include <stdarg.h>
#include "aggregate.h"
#include "singular.h"
#define MAX_COL 256
#define MAX_CHAR 2048   // Safe estimate of max buffer size
#define LENGTH(x) (sizeof(x)/sizeof(x[0]))

int max_array_size = MAX_COL;
int output_prec = -1;   // Default is -1. Means no precision is specified

void setFinalCommand(char *finalCommand, char *command, int size) {
    finalCommand = realloc(finalCommand, size * sizeof(char));
    for (int i=0; i < size; i++) {
        finalCommand[i] = command[i];
    }
}

int isDouble(const char* str) {
    char* endptr = 0;
    strtod(str, &endptr);

    if(*endptr != '\0' || endptr == str)
        return 0;
    return 1;
}

int isInt(const char* str) {
    char* endptr;
    int errno = 0;

    long val = strtol(str, &endptr, 10);

    if (errno != 0 || endptr == str) {
        // An error occurred during conversion or no digits were parsed
        return 0;
    }

    // Check if any non-whitespace characters follow the number
    while (*endptr != '\0') {
        if (!isspace(*endptr)) {
            return 0;
        }
        endptr++;
    }

    // Check if the value is within the range of an int
    if (val < INT_MIN || val > INT_MAX) {
        return 0;
    }

    return 1;
}

int my_strlen(char* str) {
    int i;
    for (i=0; str[i] != '\0'; i++);
    return i;
}

void strToUpper(char* str) {
    for (int i=0; str[i] != '\0'; i++) {
        str[i] = toupper(str[i]);
    }
}

void printErrorMsg(FILE *stream, char * format, ...) {
    va_list args;

    va_start(args, format);
    vfprintf(stream, format, args);
    va_end(args);
    exit(1);
}

int main(int argc, char *argv[])  {
    /* ----- Parse the command line arguments ----- */

    // Check to see if anything was piped into the programs stdin. 1 means terminal, 0 is piped
    if (isatty(fileno(stdin)) == 1) {
        printErrorMsg(stderr, "Error: Nothing was piped into the program as input. in file: %s, on line: %d.\n", __FILE__, __LINE__);
    }

    //const enum commands {COUNT, MIN, MAX, SUM, AVG, PAVG, PRINT, FILTER, SHIFT} command;
    const char *validAggregateCommands[] = {"COUNT", "MIN", "MAX", "SUM", "AVG", "PAVG"};

    extern char *filter_types[];
    extern int filter_types_count;
    double threshold;
    enum filter_type t;
    double shiftValue;
    char *finalCommand = (char *)malloc(sizeof(char));
    int commandFlag = 0;
    int aggregateCommandFlag = 0;
    for (int i=1; i < argc; i++) {
        if (argv[i][0] == '-') {
            char *argument = strtok(argv[i], "=");
            if (strcmp(argument, "-size") == 0) {
                char *size_str = strtok(NULL, "=");
                if (size_str == NULL || !isInt(size_str)) {
                    fprintf(stderr, "Error: -size=%s is not an int. in file: %s, on line: %d.\n", size_str, __FILE__ ,__LINE__);
                    exit(1);
                }
                char* endptr;
                max_array_size = strtol(size_str, &endptr, 10);
                if (max_array_size <= 0) {
                    fprintf(stderr, "Error: -size=%d must be greater than 0. in file: %s, on line: %d.\n", max_array_size, __FILE__ ,__LINE__);
                    exit(1);
                }
            } else if (strcmp(argument, "-prec") == 0) {
                char *prec_str = strtok(NULL, "=");
                if (prec_str == NULL || !isInt(prec_str)) {
                    fprintf(stderr, "Error: -prec=%s is not an int. in file: %s, on line: %d.\n", prec_str, __FILE__ ,__LINE__);
                    exit(1);
                }
                char* endptr;
                output_prec = strtol(prec_str, &endptr, 10);
            } else {
                fprintf (stderr, "Error: %s is not a valid flag. in file: %s, on line: %d.\n", argument, __FILE__, __LINE__);
                exit(1);
            }
        } else {
            char *command = argv[i];
            strToUpper(command);    // Convert passed function name to uppercase
            if (commandFlag) {
                fprintf (stderr, "Error: A command was already passed before %s. in file: %s, on line: %d.\n", command, __FILE__, __LINE__);
                exit(1);
            }
            for (int j=0; j < LENGTH(validAggregateCommands); j++) {
                if (strcmp(command, validAggregateCommands[j]) == 0) {
                    commandFlag = 1;
                    aggregateCommandFlag = 1;
                    setFinalCommand(finalCommand, command, my_strlen(command));
                    break;
                }
            }
            if (strcmp(command, "PRINT") == 0) {
                commandFlag = 1;
                setFinalCommand(finalCommand, command, my_strlen(command));
                continue;
            }
            if (strcmp(command, "FILTER") == 0) {
                if ((i + 1) + 2 > argc) {
                    fprintf (stderr, "Error: Too few arguments for command %s. in file: %s, on line: %d.\n", command, __FILE__, __LINE__);
                    exit(1);
                }
                int validType = 0;
                char *type = argv[i+1];
                strToUpper(type);
                for (int j=0; j < filter_types_count; j++) {
                    if (strcmp(type, filter_types[j]) == 0) {
                        t = j;  // enum is just an int
                        validType = 1;
                        break;
                    }
                }
                if (!validType) {
                    fprintf (stderr, "Error: %s is not a valid type for the %s command. in file: %s, on line: %d.\n", type, command, __FILE__, __LINE__);
                    exit(1);
                }
                
                char *threshold_str = argv[i+2];
                if (!isDouble(threshold_str)) {
                    fprintf (stderr, "Error: %s is not double. in file: %s, on line: %d.\n", threshold_str, __FILE__, __LINE__);
                    exit(1);
                }
                sscanf(threshold_str, "%lf", &threshold);
                commandFlag = 1;
                i += 2;    // Skip the next 2 arguments as they were just evaluated
                setFinalCommand(finalCommand, command, my_strlen(command));
                continue;
            }
            if (strcmp(command, "SHIFT") == 0) {
                if ((i + 1) + 1 > argc) {
                    fprintf (stderr, "Error: Too few arguments for command %s. in file: %s, on line: %d.\n", command, __FILE__, __LINE__);
                    exit(1);
                }
                
                if (isDouble(argv[i+1])) {
                    sscanf(argv[i+1], "%lf", &shiftValue);
                } else {
                    fprintf (stderr, "Error: %s is not a double. in file: %s, on line: %d.\n", argv[i+1], __FILE__, __LINE__);
                    exit(1);
                }
                commandFlag = 1;
                i++;    // Skip the next argument as it was just evaluated
                setFinalCommand(finalCommand, command, my_strlen(command));
                continue;
            }
        }
    }

    if (!commandFlag) {
        fprintf (stderr, "Error: No command was passed\n");
        exit(1);
    }    
    
    /* ----- Dynamically allocate the contents of stdin to a 2d array ----- */
    char input[MAX_CHAR];
    char delimiter[] = " \n";
    double **arr = (double **)malloc(1 * sizeof(double *));
    int *sizes = (int *)malloc(1 * sizeof(int));
    double *innerArr;

    int firstRowIteration = 1;
    int row = 0;
    while (fgets(input, sizeof(input), stdin) != NULL) {
        if (strcmp(input, "\n") == 0) {continue;}
        innerArr = (double *)malloc(max_array_size * sizeof(double));
        if (innerArr == NULL) {
            printf("Failed to allocate memory for the array.\n");
            return 1;
        }

        int col = 0;
        double scanned;
        char* token = strtok(input, delimiter);
        while (token != NULL) {
            if (col == max_array_size) {
                arr[row] = innerArr;
                sizes[row] = col;  
                if (firstRowIteration) {
                    max_array_size = sizes[row];
                    firstRowIteration = 0;
                }
                row++;
                sizes = (int *)realloc(sizes, (row + 1) * sizeof(int));
                arr = (double **)realloc(arr, (row + 1) * sizeof(double *));
                col = 0;
                innerArr = (double *)malloc(max_array_size * sizeof(double));
            }
            sscanf(token, "%lf", &scanned);
            innerArr[col] = scanned;
            col++;

            token = strtok(NULL, delimiter);
        }
        innerArr = (double *)realloc(innerArr, (col) * sizeof(double));
        arr[row] = innerArr;
        sizes[row] = col;  
        if (firstRowIteration) {
            if (sizes[row] > 0) {
                max_array_size = sizes[row];
                firstRowIteration = 0;
            }
        }
        row++;
        sizes = (int *)realloc(sizes, (row + 1) * sizeof(int));
        arr = (double **)realloc(arr, (row + 1) * sizeof(double *));
    }

    /* ----- Run the command ----- */

    if (aggregateCommandFlag) {
        for (int i=0; i < row; i++) {
            if (arr[i] == NULL || sizes[i] == 0) {continue;}    // Skip arrays that are empty or null
            formatedDoublePrint(aggregate(finalCommand, arr[i], sizes[i]), output_prec);
        }
        printf("\n");
    } else if (strcmp(finalCommand, "PRINT") == 0) {
        for (int i=0; i < row; i++) {
            print(arr[i], sizes[i]);
        }
    } else if (strcmp(finalCommand, "FILTER") == 0) {
        for (int i=0; i < row; i++) {
            sizes[i] = filter(arr[i], sizes[i], t, threshold);
            if (sizes[i] == 0) {
                arr[i] = NULL;
            } else {
                arr[i] = (double *)realloc(arr[i], sizes[i] * sizeof(double));
            }
            print(arr[i], sizes[i]);
        }
    } else if (strcmp(finalCommand, "SHIFT") == 0) {
        for (int i=0; i < row; i++) {
            shift(arr[i], sizes[i], shiftValue);
            print(arr[i], sizes[i]);
        }
    }

    /* ----- Free dynamic memory ----- */

    for (int i=0; i < row; i++) {
        free(arr[row]);
    }
    free(arr);
    free(finalCommand);
}