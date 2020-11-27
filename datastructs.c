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

word_token* insert_word(word_token* freq_list, char* str, int freq) {
    word_token* list = freq_list;
    // first element
    if (list == NULL || strcmp(str, list->word) < 0) {
        word_token* new_token = (word_token*)malloc(sizeof(word_token));
        new_token->next = list;
        new_token->freq = freq;
        new_token->word = str;
        return new_token;
    }
    while (list->next != NULL) {
        if (strcmp(str, (list->next)->word) < 0) {
            word_token* new_token = (word_token*)malloc(sizeof(word_token));
            new_token->next = list->next;
            new_token->freq = freq;
            new_token->word = str;
            list->next = new_token;
            return list;
        }
        list = list->next;
    }
    word_token* new_token = (word_token*)malloc(sizeof(word_token));
    new_token->next = list->next;
    new_token->freq = freq;
    new_token->word = str;
    list->next = new_token;
    return freq_list;
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

file_token* insert_file(file_token* file_list, word_token* word_list, char* str, int total) {
    file_token* list = file_list;
    // first element
    if (list == NULL) {
        file_token* new_token = (file_token*)malloc(sizeof(file_token));
        new_token->next = list;
        new_token->freq_chain = word_list;
        new_token->total = total;
        new_token->file_name = str;
        return new_token;
    }
    while (list->next != NULL) {
        list = list->next;
    }
    file_token* new_token = (file_token*)malloc(sizeof(file_token));
    new_token->next = list->next;
    new_token->freq_chain = word_list;
    new_token->total = total;
    new_token->file_name = str;
    list->next = new_token;
    return file_list;
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

int main() {
    // word_token* t1 = (word_token*)malloc(sizeof(word_token));
    // word_token* t2 = (word_token*)malloc(sizeof(word_token));
    // word_token* t3 = (word_token*)malloc(sizeof(word_token));
    char* word1 = "hello";
    char* word2 = "shine";
    char* word3 = "sun";
    char* word4 = "abc";
    char* word5 = "hel";
    word_token* t1 = NULL;
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
    t1 = insert_word(t1, word1, 1);
    t1 = insert_word(t1, word2, 1);
    t1 = insert_word(t1, word4, 1);
    t1 = insert_word(t1, word3, 1);
    t1 = insert_word(t1, word5, 1);
    print_list(t1);
    return 0;
}