#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "reader.h"
#include "datastructs.h"


/*------------------------------HASHTABLE-------------------------------------*/

/* djb2 hash function for strings by dan bernstein */
int _hash(const char *s) {
    unsigned long hash = 5381;
    int c;

    while ((c = *s++)) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash % HASHSIZE;
}

/* search hashtable for an entry that has val=s */
HashItem *ht_lookup(HashItem **ht_table, const char *s) {
	HashItem *ptr = ht_table[_hash(s)];
	for (; ptr != NULL; ptr = ptr->next) {
		if (strcmp(s, ptr->val) == 0) {
			return ptr;
		}
	}
	return NULL;
}

/* insert s into the hash table. if s is already present, then increment associated freq */
void ht_add(HashItem **ht_table, const char *s) {
    if (s == NULL) {return;}

    HashItem *ptr = ht_lookup(ht_table, s);
    if (ptr == NULL) {  // not found
        HashItem *new_item = malloc(sizeof(HashItem));
        new_item->val = malloc(strlen(s)+1);
        strcpy(new_item->val, s);
        new_item->freq = 1;

        int hashval = _hash(s);
        new_item->next = ht_table[hashval];
        ht_table[hashval] = new_item;
    }

    else {
        ptr->freq++;
    }
}

/* free all elements in ht */
void ht_free(HashItem **ht_table) {
	for (int i=0; i < HASHSIZE; i++) {
		HashItem *tmp, *p = ht_table[i];
		while (p != NULL) {
			tmp = p->next;
			free(p->val);
			free(p);
			p = tmp;
		}
	}
}

/* go through all elements in hashtable and generate a TokenList based on freq distribution */
TokenList *ht_read_all(HashItem **ht_table, int total) {
    TokenList *freq_list = NULL;

    for (int i=0; i < HASHSIZE; i++) {
        HashItem *curr = ht_table[i];
        while (curr != NULL) {
            insert_word(&freq_list, curr->val, ((double)curr->freq)/total);
            curr = curr->next;
        }
    }

    return freq_list;
}


/*------------------------------Token-------------------------------------*/

void tok_insert_char(Token *tok, char c) {
    if (tok->index >= (tok->size-1)) {  // if tok is too small to hold token, double its size
        tok->token = realloc(tok->token, tok->size*2);
        tok->size *= 2;
    }
    tok->token[tok->index] = c;
    tok->index++;
}

char *tok_to_string (Token *tok) {
    if (tok->index == 0) {  // make sure tok isnt length 0
        return NULL;
    }

    // set null terminator
    tok->token[tok->index] = 0;
    char *s = malloc(tok->index+2);  // malloc new string to store token
    strcpy(s, tok->token);
    return s;
}

void tok_clear (Token *tok) {
    // no need to clear tok buffer, will be overwritten
    tok->index = 0;
}