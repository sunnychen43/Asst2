#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    int total; //number of tokens
} file_token;

/* FREQUENCY CHAIN */

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
    while (list != NULL) {
        printf("%s: %f\n", list->word, list->freq);
        list = list->next;
    }
}

/* FILE FREQUENCY */

void insert_file (file_token** file_list, word_token* word_list, char* input, int total) {
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
    while (list != NULL) {
        printf("%s\n", list->file_name);
        print_list(list->freq_chain);
        list = list->next;
    }
}