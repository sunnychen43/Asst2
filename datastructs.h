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

typedef struct OutputList {
    char *file1, *file2;
    int sum;
    double jensen;

    struct OutputList* next;

} OutputList;


TokenList *new_toklist(double freq, const char *word);
void free_toklist(TokenList *tl);
void insert_word(TokenList **token_list, const char *word, double freq);

FileList *new_filelist(TokenList *token_list, const char *file_name, int total);
void free_filelist(FileList *fl);
void insert_file(FileList **file_list, TokenList *token_list, const char *file_name, int total);

OutputList *new_outlist(FileList *f1, FileList *f2, double jensen);
void insert_output (OutputList **output_list, FileList *f1, FileList *f2, double jensen);
void free_output(OutputList *ol);

#endif