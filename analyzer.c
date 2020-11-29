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

// should change to another name, i use a struct called token in reader.c
typedef struct word_token {
    struct word_token* next;
    double freq; //frequency of token in file
    char* word;
} word_token;

typedef struct file_token {
    struct word_token* freq_chain;
    struct file_token* next;
    char* file_name;
    double total; //number of tokens
} file_token;

typedef struct output_token {
    struct file_token* f1;
    struct file_token* f2;
    double sum; //number of tokens
    double jensen;
    struct output_token* next;
} output_token;

void insert_output (output_token** output_list, file_token* f1, file_token* f2, double sum, double jensen) {
    output_token* new_token = (output_token*)malloc(sizeof(output_token));
    new_token->f1 = f1;
    new_token->f2 = f2;
    new_token->sum = sum;
    new_token->jensen = jensen;
    new_token->next = NULL;
    output_token* list = *output_list;
    // first element
    if (list == NULL || sum < list->sum) {
        new_token->next = list;
        *output_list = new_token;
        return;
    }
    //insertion sort
    while (list->next != NULL) {
        if (sum > list->sum) {
            break;
        }
        list = list->next;
    }
    new_token->next = list->next;
    list->next = new_token;
}

/* FREQUENCY CHAIN */
word_token* construct_word (double freq, const char* input) {
    word_token* new_token = (word_token*)malloc(sizeof(word_token));
    new_token->freq = freq;
    new_token->word = malloc(strlen(input)+1);
    strcpy(new_token->word, input);
}

void insert_word (word_token** freq_list, const char* input, double freq) {
    word_token* new_token = (word_token*)malloc(sizeof(word_token));
    new_token->freq = freq;
    new_token->word = malloc(strlen(input)+1);
    strcpy(new_token->word, input);
    word_token* list = *freq_list;
    // first element
    if (list == NULL || strcmp(input, list->word) < 0) {
        new_token->next = list;
        *freq_list = new_token;
        return;
    }
    while (list->next != NULL) {
        if (strcmp(input, (list->next)->word) < 0) {
            break;
        }
        list = list->next;
    }
    new_token->next = list->next;
    list->next = new_token;
}

void print_list(word_token* list) {
    if (list == NULL) {
        printf("Empty list\n");
    }
    while (list->next != NULL) {
        printf("%s %f\n", list->word, list->freq);
        list = list->next;
    }
    printf("%s %f\n", list->word, list->freq);
}

/* FILE FREQUENCY */
file_token* construct_file(word_token* word_list, double total, const char* input) {
    file_token* new_token = (file_token*)malloc(sizeof(file_token));
    new_token->freq_chain = word_list;
    new_token->total = total;
    new_token->file_name = malloc(strlen(input)+1);
    strcpy(new_token->file_name, input);
    new_token->next = NULL;
    return new_token;
}

void insert_file (file_token** file_list, word_token* word_list, const char* input, double total) {
    file_token* new_token = (file_token*)malloc(sizeof(file_token));
    new_token->freq_chain = word_list;
    new_token->total = total;
    new_token->file_name = malloc(strlen(input)+1);
    strcpy(new_token->file_name, input);
    new_token->next = NULL;
    file_token* list = *file_list;
    // first element
    if (list == NULL) {
        *file_list = new_token;
        return;
    }

    while (list->next != NULL) {
        list = list->next;
    }
    list->next = new_token;
}

void print_file(file_token* list) {
    if (list == NULL) {
        printf("Empty list\n");
    }
    while (list->next != NULL) {
        print_list(list->freq_chain);
        list = list->next;
    }
    print_list(list->freq_chain);
}

word_token *mean_dist(word_token* list_1, word_token* list_2) {

    word_token* avg_list = malloc(sizeof(word_token));
    word_token* curr = avg_list;

    while (list_1 != NULL && list_2 != NULL) {
        word_token* new_token = malloc(sizeof(word_token));
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
        word_token* new_token = malloc(sizeof(word_token));
        new_token->next = NULL;
        new_token->freq = list_1->freq/2;
        new_token->word = list_1->word;

        curr->next = new_token;
        list_1 = list_1->next;

        curr = curr->next;
    }
    while (list_2 != NULL) {
        word_token* new_token = (word_token*)malloc(sizeof(word_token));
        new_token->next = NULL;
        new_token->freq = list_2->freq/2;
        new_token->word = list_2->word;

        curr->next = new_token;
        list_2 = list_2->next;

        curr = curr->next;
    }

    word_token *ret = avg_list->next;
    free(avg_list);

    return ret;
}

double kullbeck(word_token* mean_list, word_token* dist_list) {
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

void output(double j, file_token* list_1, file_token* list_2) {
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
    printf("\"%s\" and \"%s\"\n", list_1->file_name, list_2->file_name);
}

void anal_file(file_token* f) {
    file_token* files = f;
    // int file_number = 0;
    // while (files != NULL) {
    //     files = files->next;
    //     file_number += 1;
    // }
    output_token* final_output = NULL;
    while (files != NULL) {
        file_token* subfiles = files->next;
        while (subfiles != NULL) {
            word_token* result = mean_dist(files->freq_chain, subfiles->freq_chain);
            double a = kullbeck(result, files->freq_chain);
            double b = kullbeck(result, subfiles->freq_chain);
            insert_output(&final_output, files, subfiles, files->total + subfiles->total, jensen(a,b));
            subfiles = subfiles->next;
        }
        files = files->next;
    }
    // print
    while (final_output != NULL) {
        output(final_output->jensen, final_output->f1, final_output->f2);
        final_output = final_output->next;
    }
}

int main() {
    char* word1 = "hi";
    char* word2 = "out";
    char* word3 = "sun";
    char* word4 = "there";
    char* word5 = "hel";
    char* word6 = "iota";
    word_token* t = NULL;
    word_token** t1 = &t;
    insert_word(t1, word1, 0.5);
    insert_word(t1, word2, 0.25);
    insert_word(t1, word4, 0.25);
    word_token* tt = NULL;
    word_token** t2 = &tt;
    insert_word(t2, word1, 0.5);
    insert_word(t2, word4, 0.5);
    file_token* f = NULL;
    file_token** f1 = &f;
    insert_file(f1, *t1, "helloworld.txt", 300);
    insert_file(f1, *t2, "sunny.txt", 200);
    insert_file(f1, *t1, "disney.txt", 20);
    insert_file(f1, *t2, "hun.txt", 100);
    word_token* result = NULL;
    word_token** tresult = &result;
    //print_list(*tresult);
    anal_file(*f1);
    //output(jensen(a,b), *f1, (*f1)->next); //haven't done the ordering from smallest to highest token number yet
}