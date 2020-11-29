#include "datastructs.h"

typedef struct OutputList {
    struct FileList *f1;
    struct FileList *f2;
    int sum; //number of tokens
    double jensen;
    struct OutputList* next;
} OutputList;

void anal_file(FileList *files);
void output(double j, const char *file1, const char *file2);
double kullbeck(TokenList *mean_list, TokenList *dist_list);
TokenList *mean_dist(TokenList *list_1, TokenList *list_2);
double jensen(double a,double b);
OutputList *new_outlist(FileList *f1, FileList *f2, double jensen);
