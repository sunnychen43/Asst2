#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "datastructs.h"

/*------------------------------TokenList-------------------------------------*/

/* 
Creates and returns the pointer to a new TokenList object, and sets the frequency and 
word accordingly strcpy() is used to ensure changes to the parameter word will not 
affect new_token->word and to avoid double free()ing
 */
TokenList *new_toklist(double freq, const char *word) {
    TokenList *new_token = malloc(sizeof(TokenList));
    new_token->freq = freq;
    new_token->word = malloc(strlen(word)+1);
    strcpy(new_token->word, word);
    new_token->next = NULL;
    return new_token;
}
/* 
Frees all tokens and token->words for the given head of a TokenList linked list
 */
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

/* 
Creates and inserts new TokenList instance in linked list corresponding to the token_list parameter double pointer
Sets word and frequency accordingly, while placing TokenList such that the linked list remains in alphabetical
order. Can be called repeatedly to create a TokenList linked list to store tokens for a file
 */
void insert_word (TokenList **token_list, const char *word, double freq) {
    TokenList *new_token = new_toklist(freq, word);
    TokenList *curr = *token_list;
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


/*------------------------------FileList-------------------------------------*/

/* 
Creates and returns the pointer to a new FileList object, and sets the total token count,
TokenList linked list head and file name accordingly. strcpy() is used to ensure changes 
to the parameter word will not affect new_token->word and to avoid double free()ing
 */
FileList *new_filelist(TokenList *token_list, const char *file_name, int total) {
    FileList *new_list = malloc(sizeof(FileList));

    new_list->token_list = token_list;
    new_list->file_name = malloc(strlen(file_name)+1);
    strcpy(new_list->file_name, file_name);
    new_list->total = total;
    new_list->next = NULL;

    return new_list;
}

/* 
Frees all tokens and token->words for the given head of a FileList linked list
 */
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

/* 
Creates and inserts new FileList instance in linked list corresponding to the file_list parameter double pointer
Sets word, TokenList linked list and frequency accordingly, while adding the new FileList struct to the linked list. 
Can be called repeatedly to create a TokenList linked list to store tokens for a file
 */
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


/*------------------------------OutputList-------------------------------------*/

/*
Creates and returns a pointer to a new OutputList object for the given pair of files and their calculated 
Jensen distance. malloc and strcpy file_names for each file so that double freeing will not occur.
 */
OutputList *new_outlist(FileList *f1, FileList *f2, double jensen) {
    OutputList *new_list = (OutputList*)malloc(sizeof(OutputList));

    new_list->file1 = malloc(strlen(f1->file_name)+1);
    strcpy(new_list->file1, f1->file_name);

    new_list->file2 = malloc(strlen(f2->file_name)+1);
    strcpy(new_list->file2, f2->file_name);

    new_list->sum = f1->total + f2->total;
    new_list->jensen = jensen;
    new_list->next = NULL;
    return new_list;
}

/*
Given OutputList linked list head, free all OutputLists in the linked list to avoid memory leaks
 */
void free_output(OutputList *ol) {
    OutputList *p = ol;
    OutputList *tmp;
    while (p != NULL) {
        tmp = p->next;
        free(p->file1);
        free(p->file2);
        free(p);
        p = tmp;
    }
}

/*
Inserts a new OutputList for the given file parameters and Jensen distance to the OutputList link list parameter. 
Uses insertion sort to keep the output_list sorted from least number of total tokens to most number of total tokens.
 */
void insert_output (OutputList **output_list, FileList *f1, FileList *f2, double jensen) {
    OutputList *curr = *output_list;
    OutputList *new_list = new_outlist(f1, f2, jensen);
    // check if new_list belongs in beginning of LL
    if (curr == NULL || new_list->sum < curr->sum) {
        new_list->next = curr;
        *output_list = new_list;
        return;
    }
    //insertion sort from smallest to greatest
    while (curr->next != NULL) {
        if (new_list->sum < curr->next->sum) {
            break;
        }
        curr = curr->next;
    }
    new_list->next = curr->next;
    curr->next = new_list;
}

