#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "reader.h"
#include "datastructs.h"
#include "analyzer.h"


/* Global variables for multithreading */
static FileList *master;  // shared storage
static pthread_mutex_t lock;
static pthread_t tid[1024];
int tid_count = 0;

/* Tokenizer function. Accepts a file path and returns NULL. Tokenizes all the words in the given 
file, storing them in a hashtable to keep track of frequencies. We use a Token data structure defined 
in reader.h to store tokens, which can handle words of variable size. */
void *read_file(void *args) {
    const char *filename = (const char *)args;

    /* hash table to store frequencies */
    HashItem *ht_table[HASHSIZE];
    memset(ht_table, 0, HASHSIZE*sizeof(*ht_table));  // clear hashtable memory

    int fd = open(filename, O_RDONLY);

    char buf[1024];  // buffer to read into
    int bytes_read;
    int count = 0;

    Token tok;  // variable-size storage for tokens, will automatically adjust size
    tok.token = malloc(128);
    tok.size = 128;
    tok.index = 0;

    do {
        bytes_read = read(fd, &buf, 1024);  // check how many bytes read to see if at EOF
        
        for (int i=0; i < bytes_read; i++) {
            /* check for delim */
            if (buf[i] == ' ' || buf[i] == '\n') {
                /* try to turn current token into string */
                char *s = tok_to_string(&tok);
                if (s != NULL) {
                    ht_add(ht_table, s);
                    free(s);
                    count++;
                }
                tok_clear(&tok);
                continue;
            }
            /* valid chars can only be letter or hyphen */
            if (isalpha(buf[i]) || buf[i] == '-') {
                tok_insert_char(&tok, tolower(buf[i]));
            }
        }

    } while (bytes_read == 1024);  // if less than 1024 bytes read, we're at EOF

    /* check if theres token left in tok */
    char *s = tok_to_string(&tok);
    if (s != NULL) {
        ht_add(ht_table, s);
        free(s);
        count++;
    }
    close(fd);
    free(tok.token);

    /* generate TokenList from all token frequencies in hash table */
    TokenList *tok_list = ht_read_all(ht_table, count);
    ht_free(ht_table);

    pthread_mutex_lock(&lock);  // mutex lock required bc master is modified by multiple threads at once
    insert_file(&master, tok_list, filename, count);
    pthread_mutex_unlock(&lock);

    free(args);
    return NULL;
}

/* Directory handler scans given directory for all files. If a subdirectory is found, 
process it recursivly. If a file is found, spawn a new pthread to process it*/
void read_dir(const char *path) {
    DIR *dir = opendir(path);
    if (dir == NULL) {  // check if dir can be accessed
        printf("Can't open dir: %s\n", path);
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        char *ent_path = malloc(strlen(entry->d_name)+strlen(path)+2);  // variable size path
        sprintf(ent_path, "%s/%s", path, entry->d_name);  // make new path based on old path

        if (entry->d_type == DT_DIR) {  // directory
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                free(ent_path);
                continue;
            }
            read_dir(ent_path);
            free(ent_path);
        }
        else if (entry->d_type == DT_REG){  // valid file
            pthread_create(&tid[tid_count], NULL, read_file, ent_path);
            tid_count++;
        }
        else {  // invalid file
            free(ent_path);
        }
    }
    closedir(dir);
}

/* main routine, accepts one argument which is the directory to scan */
int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Invalid arguments, 1 required\n");
        return -1;
    }

    pthread_mutex_init(&lock, NULL);
    read_dir(argv[1]);
    
    /* read_dir will spawn all pthreads. wait until all threads are created then start joining them. if we
    join within read_dir, this will create blocking, as all threads in one directory must be processed before 
    another directory */
    for (int i=0; i <= tid_count; i++) {
        pthread_join(tid[i], NULL);
    }

    pthread_mutex_destroy(&lock);

    analyze(master);  // run analysis on all files
    free_filelist(master);
}