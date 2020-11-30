#ifndef ANALYZER_H
#define ANALYZER_H

#include "datastructs.h"


TokenList *mean_dist(TokenList *list_1, TokenList *list_2);
double kullbeck(TokenList *mean_list, TokenList *dist_list);
double jensen(double a,double b);

void analyze(FileList *files);
void output(double j, const char *file1, const char *file2);

#endif