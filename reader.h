#ifndef READER_H
#define READER_H

#include <stddef.h>
#include "datastructs.h"
#define HASHSIZE 53


typedef struct Token {
    char *token;
    size_t size;
    int index;
} Token;

typedef struct HashItem {
	char *val;
    int freq;
    struct HashItem *next;
} HashItem;


void tok_insert_char(Token *tok ,char c);
char *tok_to_string(Token *tok);
void tok_clear(Token *tok);

void ht_add(HashItem **ht_table, const char *s);
HashItem *ht_lookup(HashItem **ht_table, const char *s);
void ht_free(HashItem **ht_table);
TokenList *ht_read_all(HashItem **ht_table, int total);

#endif