#include "reader.h"
#include "datastructs.h"
#include "analyzer.h"
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

static FileList *master;
static pthread_mutex_t lock;
static pthread_t tid[100];
int tid_count = 0;

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
    ht_free(ht_table);

    pthread_mutex_lock(&lock); 
    insert_file(&master, tok_list, filename, count);
    pthread_mutex_unlock(&lock);

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
            pthread_create(&tid[tid_count], NULL, read_file, ent_path);
            tid_count++;
        }

        free(ent_path);
    }
    closedir(dir);
}

int main() {
    read_dir("test");
    pthread_mutex_init(&lock, NULL);
    for (int i=0; i <= tid_count; i++) {
        pthread_join(tid[i], NULL);
    }
    pthread_mutex_destroy(&lock);

    anal_file(master);
    free_filelist(master);
}