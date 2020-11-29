#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include "datastructs.c"


/*------------------------------HASHTABLE-------------------------------------*/

#define HASHSIZE 53
typedef struct HashItem {
	char *val;
    int freq;
    struct HashItem *next;
} HashItem;

/* djb2 hash function for strings by dan bernstein */
int _hash(const char *s) {
    unsigned long hash = 5381;
    int c;

    while ((c = *s++)) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash % HASHSIZE;
}

HashItem *ht_lookup(HashItem **ht_table, const char *s) {
	HashItem *ptr = ht_table[_hash(s)];
	for (; ptr != NULL; ptr = ptr->next) {
		if (strcmp(s, ptr->val) == 0) {
			return ptr;
		}
	}
	return NULL;
}

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

typedef struct Token {
    char *token;
    size_t size;
    int index;
} Token;

void tok_insert_char (Token *tok, char c) {
    if (tok->index >= (tok->size-1)) {
        tok->token = realloc(tok->token, tok->size*2);
        tok->size *= 2;
    }

    tok->token[tok->index] = c;
    tok->index++;
}

char *tok_to_string (Token *tok) {
    if (tok->index == 0) {
        return NULL;
    }

    // set null terminator
    tok->token[tok->index] = 0;
    char *s = malloc(tok->index+2);
    strcpy(s, tok->token);
    return s;
}

void tok_clear (Token *tok) {
    // no need to clear tok buffer, will be overwritten
    tok->index = 0;
}

static FileList *master;

void *read_file(void *args) {
    const char *filename = (const char *)args;

    HashItem *ht_table[HASHSIZE];
    memset(ht_table, 0, HASHSIZE*sizeof(*ht_table));

    int fd = open(filename, O_RDONLY);

    char buf[1024];
    int bytes_read;
    int count = 0;

    Token tok;
    tok.token = malloc(128);
    tok.size = 128;
    tok.index = 0;

    do {
        bytes_read = read(fd, &buf, 1024);
        
        for (int i=0; i < bytes_read; i++) {
            // check for delim
            if (buf[i] == ' ' || buf[i] == '\n') {
                char *s = tok_to_string(&tok);
                if (s != NULL) {
                    ht_add(ht_table, s);
                    count++;
                }
                tok_clear(&tok);
                continue;
            }
            // check for punctuation
            if (isalpha(buf[i]) || buf[i] == '-') {
                tok_insert_char(&tok, tolower(buf[i]));
            }
        }

    } while (bytes_read == 1024);

    char *s = tok_to_string(&tok);
    if (s != NULL) {
        ht_add(ht_table, s);
        count++;
    }
    close(fd);
    free(tok.token);

    TokenList *tok_list = ht_read_all(ht_table, count);
    insert_file(&master, tok_list, filename, count);

    return NULL;
}

void read_dir(const char *path) {
    DIR *dir = opendir(path);
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        char *ent_path = malloc(strlen(entry->d_name)+strlen(path)+2);
        sprintf(ent_path, "%s/%s", path, entry->d_name);

        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }
            read_dir(ent_path);
        }
        else {
            read_file(ent_path);
        }
    }
}

int main() {
    read_dir("test");
    print_file(master);
}