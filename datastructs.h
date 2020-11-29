#ifndef DATASTRUCTS_H
#define DATASTRUCTS_H

typedef struct TokenList {
    double freq; //frequency of token in file
    char *word;

    struct TokenList* next;

} TokenList;

typedef struct FileList {
    TokenList *token_list;
    char *file_name;
    int total; //number of tokens

    struct FileList* next;

} FileList;

void print_file(FileList *list);
void insert_file(FileList **file_list, TokenList *token_list, const char *file_name, int total);
void print_list(TokenList *list);
void insert_word(TokenList **token_list, const char *word, double freq);
FileList *new_filelist(TokenList *token_list, const char *file_name, int total);
TokenList *new_toklist(double freq, const char *word);
void free_toklist(TokenList *tl);
void free_filelist(FileList *fl);

#endif