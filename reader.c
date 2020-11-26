#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <unistd.h>
#include <fcntl.h>


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

void read_file(const char *filename) {
    int fd = open(filename, O_RDONLY);

    char buf[1024];
    int bytes_read;

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
                    printf("%s\n", s);
                }
                tok_clear(&tok);
                continue;
            }
            // check for punctuation
            if (!isalnum(buf[i])) {continue;}
            
            tok_insert_char(&tok, buf[i]);
        }

    } while (bytes_read == 1024);

    char *s = tok_to_string(&tok);
    if (s != NULL) {
        printf("%s\n", s);
    }

    close(fd);
}

int main() {
    read_file("test.txt");
}