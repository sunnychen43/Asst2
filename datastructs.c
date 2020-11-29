#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "datastructs.h"

// should change to another name, i use a struct called token in reader.c

TokenList *new_toklist(double freq, const char *word) {
    TokenList *new_token = malloc(sizeof(TokenList));

    new_token->freq = freq;
    new_token->word = malloc(strlen(word)+1);
    strcpy(new_token->word, word);
    new_token->next = NULL;

    return new_token;
}

void free_toklist(TokenList *tl) {
    TokenList *p = tl;
    TokenList *tmp;
    while (p != NULL) {
        tmp = p->next;
        free(p->word);
        free(p);
        p = tmp;
    }
}

FileList *new_filelist(TokenList *token_list, const char *file_name, int total) {
    FileList *new_list = malloc(sizeof(FileList));

    new_list->token_list = token_list;
    new_list->file_name = malloc(strlen(file_name)+1);
    strcpy(new_list->file_name, file_name);
    new_list->total = total;
    new_list->next = NULL;

    return new_list;
}

void free_filelist(FileList *fl) {
    FileList *p = fl;
    FileList *tmp;
    while (p != NULL) {
        tmp = p->next;
        free(p->file_name);
        free_toklist(p->token_list);
        free(p);
        p = tmp;
    }
}

/* FREQUENCY CHAIN */
void insert_word (TokenList **token_list, const char *word, double freq) {
    TokenList *new_token = new_toklist(freq, word);

    TokenList *curr = *token_list;
    // first element
    if (curr == NULL || strcmp(word, curr->word) < 0) {
        new_token->next = curr;
        *token_list = new_token;
        return;
    }

    while (curr->next != NULL) {
        if (strcmp(word, (curr->next)->word) < 0) {
            break;
        }
        curr = curr->next;
    }

    new_token->next = curr->next;
    curr->next = new_token;
}

void print_list(TokenList *list) {
    while (list != NULL) {
        printf("%s: %f\n", list->word, list->freq);
        list = list->next;
    }
}

/* FILE FREQUENCY */

void insert_file(FileList **file_list, TokenList *token_list, const char *file_name, int total) {
    FileList *new_token = new_filelist(token_list, file_name, total);
    FileList *list = *file_list;
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

void print_file(FileList *list) {
    while (list != NULL) {
        printf("%s\n", list->file_name);
        print_list(list->token_list);
        list = list->next;
    }
}