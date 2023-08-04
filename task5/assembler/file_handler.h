#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assembler.h"

#define INITIAL_FILE_CONTENT_SIZE 100
#define INITIAL_MACRO_SIZE 100

typedef struct {
    char** content;
    char* filename;
    size_t size;
    size_t capacity;
} FileContent;

void set_open_file(Assembler *);

int validate_has_no_extension(const char*);

char* get_as_filename(const char*);
char* get_am_filename(const char*);
char* get_ob_filename(const char*);
char* get_ent_filename(const char*);
char* get_ext_filename(const char*);

char* get_filename_with_extension(const char*, const char*);

/*
    Write content to the filename in FileContent and later free's the memory of FileContent;
*/
void write_to_file(Assembler *, FileContent *);
FileContent* new_file_content(char *);
void add_line_to_content(FileContent *file_content, char *line);
void free_file_content(FileContent *file);

#endif /* FILE_UTILS_H */