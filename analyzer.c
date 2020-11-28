/*
For each pair of file distributions:
Make mean distribution linked list by iterating through other two distributions with pointers
Calculate each Kullback-Leibler Divergence
Calculate the Jensen Shannon Distance
Outputs the Jensen Shannon distances for each pair
Color coded outputs
Output in order from smallest to largest number of tokens
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "datastructs.c"

/* FREQUENCY CHAIN */

TokenList *mean_dist(TokenList* list_1, TokenList* list_2) {

    TokenList* avg_list = malloc(sizeof(TokenList));
    TokenList* curr = avg_list;

    while (list_1 != NULL && list_2 != NULL) {
        TokenList* new_token = malloc(sizeof(TokenList));
        new_token->next = NULL;

        if (strcmp(list_1->word, list_2->word) < 0) {
            new_token->freq = list_1->freq/2;
            new_token->word = list_1->word;
            curr->next = new_token;

            list_1 = list_1->next;
        }
        else if (strcmp(list_1->word, list_2->word) == 0) {
            new_token->freq = (list_1->freq+list_2->freq)/2;
            new_token->word = list_1->word;
            curr->next = new_token;

            list_1 = list_1->next;
            list_2 = list_2->next;
        }
        else {
            new_token->freq = list_2->freq/2;
            new_token->word = list_2->word;
            curr->next = new_token;

            list_2 = list_2->next;
        }

        curr = curr->next;
    }

    while (list_1 != NULL) {
        TokenList* new_token = malloc(sizeof(TokenList));
        new_token->next = NULL;
        new_token->freq = list_1->freq/2;
        new_token->word = list_1->word;

        curr->next = new_token;
        list_1 = list_1->next;

        curr = curr->next;
    }
    while (list_2 != NULL) {
        TokenList* new_token = (TokenList*)malloc(sizeof(TokenList));
        new_token->next = NULL;
        new_token->freq = list_2->freq/2;
        new_token->word = list_2->word;

        curr->next = new_token;
        list_2 = list_2->next;

        curr = curr->next;
    }

    TokenList *ret = avg_list->next;
    free(avg_list);

    return ret;
}

double kullbeck(TokenList* mean_list, TokenList* dist_list) {
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

double jensen(double a, double b) {
    return (a+b)/2;
}

void output(double j, FileList* list_1, FileList* list_2) {
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
    printf("%f ", j);
    printf("\033[1;30m");
    printf("\"%s\" and \"%s\"", list_1->file_name, list_2->file_name);
}

int main() {
    char* word1 = "hi";
    char* word2 = "out";
    char* word3 = "sun";
    char* word4 = "there";
    char* word5 = "hel";
    char* word6 = "iota";
    TokenList* t = NULL;
    TokenList** t1 = &t;
    insert_word(t1, word1, 0.5);
    insert_word(t1, word2, 0.25);
    insert_word(t1, word4, 0.25);
    TokenList* tt = NULL;
    TokenList** t2 = &tt;
    insert_word(t2, word1, 0.5);
    insert_word(t2, word4, 0.5);
    FileList* f = NULL;
    FileList** f1 = &f;
    insert_file(f1, *t1, "helloworld.txt", 300);
    insert_file(f1, *t2, "sunny.txt", 200);
    TokenList* result = NULL;
    TokenList** tresult = &result;
    mean_dist(tresult, t1, t2);
    TokenList* list = result;
    double a = kullbeck(tresult, t1);
    double b = kullbeck(tresult, t2);
    //print_list(*tresult);
    output(jensen(a,b), *f1, (*f1)->next); //haven't done the ordering from smallest to highest token number yet
}