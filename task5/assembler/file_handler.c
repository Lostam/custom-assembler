#include "file_handler.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler.h"
#include "logger.h"
#include "macros.h"
#include "string_utils.h"
#include "word_handler.h"

void set_open_file(Assembler *assembler) {
    char *filename = get_am_filename(assembler->file_basename);
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        add_error(assembler, "Failed opening file %s", filename);
        free(filename);
        return;
    }
    free(filename);
    assembler->as_file = file;
}

int validate_has_no_extension(const char *filename) {
    size_t len = strlen(filename);

    for (int i = len - 1; i >= 0; i--) {
        if (filename[i] == '.') {
            return 1;
        }
    }

    return 0;
}

char *get_as_filename(const char *filename) {
    return get_filename_with_extension(filename, ".as");
}

char *get_am_filename(const char *filename) {
    return get_filename_with_extension(filename, ".am");
}

char *get_ob_filename(const char *filename) {
    return get_filename_with_extension(filename, ".ob");
}

char *get_ent_filename(const char *filename) {
    return get_filename_with_extension(filename, ".ent");
}

char *get_ext_filename(const char *filename) {
    return get_filename_with_extension(filename, ".ext");
}

char *get_filename_with_extension(const char *filename, const char *extension) {
    size_t original_len = strlen(filename);
    size_t extension_len = strlen(extension);
    size_t new_len = original_len + extension_len + 1;  // +1 for the null terminator
    char *new_filename = (char *)malloc(new_len);
    strcpy(new_filename, filename);
    strcat(new_filename, extension);
    new_filename[new_len - 1] = '\0';
    return new_filename;
}

void write_to_file(Assembler *assembler, FileContent *file_content) {
    FILE *file;
    file = fopen(file_content->filename, "w");
    if (file == NULL) {
        error("Error opening the file [%s]", file_content->filename);
        add_error(assembler, "Failed opening [%s] file for writing, will skip assembler for this file", file_content->filename);
        free(file_content);
        return;
    }
    int i;
    for (i = 0; i < file_content->size; i++) {
        fprintf(file, "%s\n", file_content->content[i]);
    }
    info("Wrote file %s", file_content->filename);
    free(file_content);
    fclose(file);
}

FileContent *new_file_content(char *filename) {
    FileContent *file_content = (FileContent *)malloc(sizeof(FileContent));
    if (file_content == NULL) {
        printf("Memory allocation error.\n");
        exit(1);
    }
    file_content->content = (char **)malloc(INITIAL_FILE_CONTENT_SIZE * sizeof(char *));
    if (file_content->content == NULL) {
        printf("Memory allocation error.\n");
        exit(1);
    }
    file_content->size = 0;
    file_content->capacity = INITIAL_FILE_CONTENT_SIZE;
    file_content->filename = filename;
    return file_content;
}

void add_line_to_content(FileContent *file_content, char *line) {
    if (file_content->size == file_content->capacity) {
        file_content->capacity *= 2;
        char **temp = (char **)realloc(file_content->content, file_content->capacity * sizeof(char *));
        if (temp == NULL) {
            printf("Memory reallocation error.\n");
            exit(1);
        }
        file_content->content = temp;
    }

    file_content->content[file_content->size] = strdup(line);
    if (file_content->content[file_content->size] == NULL) {
        printf("Memory allocation error.\n");
        exit(1);
    }

    file_content->size++;
}

void free_file_content(FileContent *file) {
    for (size_t i = 0; i < file->size; i++) {
        free(file->content[i]);
    }
    free(file->content);
    free(file->filename);
}
