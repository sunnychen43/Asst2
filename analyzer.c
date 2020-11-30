#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "analyzer.h"
#include "datastructs.h"

/*
Create and calculate new mean distribution given two TokenList linked list parameters. mean_dist merges the two TokenList linked list parameters with the
new frequencies being the average between shared tokens and half of individual tokens while maintaining alphabetical order
 */
TokenList *mean_dist(TokenList *list_1, TokenList *list_2) {

    TokenList *avg_list = malloc(sizeof(TokenList));
    avg_list->next = NULL;
    TokenList *curr = avg_list;

    while (list_1 != NULL && list_2 != NULL) {
        TokenList *new_token;
        // List 1 has a word token alphabetically before List 2's word, so List 1's word is added to the mean_dist and List 1 is incremented.
        if (strcmp(list_1->word, list_2->word) < 0) {
            new_token = new_toklist(list_1->freq/2, list_1->word);
            curr->next = new_token;

            list_1 = list_1->next;
        }
        // List 1 and List 2 share the same word token, it's frequency is calculated and it is added to mean_dist and both lists are incremented.
        else if (strcmp(list_1->word, list_2->word) == 0) {
            new_token = new_toklist((list_1->freq+list_2->freq)/2, list_1->word);
            curr->next = new_token;

            list_1 = list_1->next;
            list_2 = list_2->next;
        }
        // List 2 has a word token alphabetically before List 1's word, so List 2's word is added to the mean_dist and List 2 is incremented.
        else {
            new_token = new_toklist(list_2->freq/2, list_2->word);
            curr->next = new_token;

            list_2 = list_2->next;
        }

        curr = curr->next;
    }
    // Finishes incrementing through List 1 since List 2 is at null
    while (list_1 != NULL) {
        TokenList *new_token = new_toklist(list_1->freq/2, list_1->word);

        curr->next = new_token;
        list_1 = list_1->next;

        curr = curr->next;
    }
    // Finishes incrementing through List 2 since List 1 is at null
    while (list_2 != NULL) {
        TokenList *new_token = new_toklist(list_2->freq/2, list_2->word);

        curr->next = new_token;
        list_2 = list_2->next;

        curr = curr->next;
    }
    // Returns avg_list->next because the head to the mean distrubution list is stored in avg_list->next
    TokenList *ret = avg_list->next;
    free(avg_list);

    return ret;
}

/*
Calculates and returns the Kullbeck distance given the mean distrubtion and the probability 
distribution for a given word frequency linked list.
 */
double kullbeck(TokenList *mean_list, TokenList *dist_list) {
    double result = 0;
    while (dist_list != NULL) {
        while (mean_list != NULL && strcmp(mean_list->word, dist_list->word) != 0) {
            mean_list = mean_list->next;
        }
        result = result + (dist_list->freq * log10((dist_list->freq)/(mean_list->freq)));
        dist_list = dist_list->next;
    }
    return result;
}

/*
Calculates and returns the Jensen distance given the Kullbeck distances for two files.
 */
double jensen(double a, double b) {
    return (a+b)/2;
}

/*
Outputs all OutputTokens in the linked list and prints the Jensen distance with the corresponding color code.
 */
void output(double j, const char *file1, const char *file2) {
    if (0 <= j && j <= 0.1) {
        printf("\033[0;31m"); //red
    }
    else if (0.1 < j && j <= 0.15) {
        printf("\033[0;33m"); //yellow
    }
    else if (0.15 < j && j <= 0.2) {
        printf("\033[0;32m"); //green
    }
    else if (0.2 < j && j <= 0.25) {
        printf("\033[0;36m"); //cyan
    }
    else if (0.25 < j && j <= 0.3) {
        printf("\033[0;34m"); //blue
    }
    else {
        printf("\033[0;37m"); //white
    }
    printf("%f ", j); //prints JSD
    printf("\033[0m"); //resets font color to default
    printf("\"%s\" and \"%s\"\n", file1, file2); //prints file names
}

/*
Implements methods in datastructs.c and analyzer.c to create every possible file pair from the FileList linked
list parameter, calculate the Kullbeck and Jensen distances, and output color coded Jensen distances and 
file names from lowest to greatest number of tokens.
 */
void analyze(FileList* files) {
    OutputList* final_output = NULL;
    //files is the first FileList struct to be compared
    while (files != NULL) {
        //subfiles is the second FileList struct to be compared
        FileList* subfiles = files->next;
        while (subfiles != NULL) {
            //mean distrubtion is calculated
            TokenList* result = mean_dist(files->token_list, subfiles->token_list);
            //kullbeck distances are calculated
            double a = kullbeck(result, files->token_list);
            double b = kullbeck(result, subfiles->token_list);
            free_toklist(result);
            //add output for this file pair to final_output, the linked list of all outputs
            insert_output(&final_output, files, subfiles, jensen(a,b));
            subfiles = subfiles->next;
        }
        files = files->next;
    }

    // print all structs stored in the final_output linked list
    OutputList *curr = final_output;
    while (curr != NULL) {
        output(curr->jensen, curr->file1, curr->file2);
        curr = curr->next;
    }

    free_output(final_output);
}
