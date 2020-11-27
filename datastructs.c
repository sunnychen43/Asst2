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
    double total; //number of tokens
} file_token;

/* FREQUENCY CHAIN */

void insert_word (word_token** freq_list, const char* input, double freq) {
    char* str = malloc(sizeof(input));
    strcpy (str, input);
    word_token* list = *freq_list;
    word_token* new_token = (word_token*)malloc(sizeof(word_token));
    new_token->freq = freq;
    new_token->word = str;
    // first element
    if (list == NULL || strcmp(str, list->word) < 0) {
        new_token->next = list;
        *freq_list = new_token;
        return;
    }
    while (list->next != NULL) {
        if (strcmp(str, (list->next)->word) < 0) {
            new_token->next = list->next;
            list->next = new_token;
            return;
        }
        list = list->next;
    }
    list->next = new_token;
    new_token->next = NULL;
    return;
}

void print_list(word_token* list) {
    if (list == NULL) {
        printf("Empty list\n");
    }
    while (list->next != NULL) {
        printf("%s\n", list->word);
        list = list->next;
    }
    printf("%s\n", list->word);
}

/* FILE FREQUENCY */

void insert_file (file_token** file_list, word_token* word_list, char* input, double total) {
    char* str = malloc(sizeof(input));
    strcpy (str, input);
    file_token* list = *file_list;
    file_token* new_token = (file_token*)malloc(sizeof(file_token));
    new_token->freq_chain = word_list;
    new_token->total = total;
    new_token->file_name = str;
    // first element
    if (list == NULL) {
        new_token->next = list;
        *file_list = new_token;
        return;
    }
    while (list->next != NULL) {
        list = list->next;
    }
    list->next = new_token;
    new_token->next = NULL;
    return;
}

void print_file(file_token* list) {
    if (list == NULL) {
        printf("Empty list\n");
    }
    while (list->next != NULL) {
        print_list(&(list->freq_chain));
        list = list->next;
    }
    print_list(&(list->freq_chain));
}

int main() {
    // word_token* t1 = (word_token*)malloc(sizeof(word_token));
    // word_token* t2 = (word_token*)malloc(sizeof(word_token));
    // word_token* t3 = (word_token*)malloc(sizeof(word_token));
    char* word1 = "hello";
    char* word2 = "shine";
    char* word3 = "sun";
    char* word4 = "abc";
    char* word5 = "hel";
    char* word6 = "iota";
    word_token* t = NULL;
    word_token** t1 = &t;
    file_token* f = NULL;
    file_token** f1 = &f;
    // if (t1 == NULL) {
    //     t1 = (word_token*)malloc(sizeof(word_token));
    //     t1->word = word1;
    //     t1->freq = 1;
    //     t1->next = NULL;
    // }
    // t1->next = t2;
    // t2->next = t3;
    // t3->next = NULL;
    // t1->freq = 1;
    // t2->freq = 1;
    // t3->freq = 1;
    // t1->word = word1;
    // t2->word = word2;
    // t3->word = word3;
    insert_word(t1, word1, 1);
    insert_word(t1, word2, 1);
    insert_word(t1, word4, 1);
    insert_word(t1, word3, 1);
    insert_word(t1, word5, 1);
    insert_word(t1, word6, 1);
    insert_file(f1, t, word1, 1);
    print_file(f1);
}