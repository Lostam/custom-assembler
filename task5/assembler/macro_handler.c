#include "macro_handler.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler.h"
#include "file_handler.h"
#include "logger.h"
#include "macros.h"
#include "string_utils.h"
#include "word_handler.h"

void parse_macros(Assembler *assembler) {
    FileContent *parsed_file = get_parsed_file(assembler);
    if (parsed_file == NULL) {
        return;
    }

    write_to_file(assembler, parsed_file);
}

FileContent *get_parsed_file(Assembler *assembler) {
    char *filename = get_as_filename(assembler->file_basename);
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        add_error(assembler, "Failed opening %s", filename);
        free(filename);
        return NULL;
    }
    free(filename);
    MacroTable *table = new_macro_table();
    FileContent *parsed_file = new_file_content(get_am_filename(assembler->file_basename));
    int in_macro = 0;
    char line[MAX_LINE_LENGTH];

    // todo :: decrease the size of this while, too big
    // todo :: maybe use feof with getc
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        size_t length = strlen(line);
        assembler->current_line_number++;
        if (line[length - 1] != '\n') {
            add_error(assembler, "line number [%d] which start with [%s] is longer then 80 chars", length, line);
            // todo :: are the two lines needed?
            
            // keep reading until end of line
            int c;
            while ((c = fgetc(file)) != '\n' && c != EOF);
            continue;
        }

        if (length > 0 && line[length - 1] == '\n') {
            line[length - 1] = '\0';
        }
        printf("[%s]\n", line);
        char *x = trim_spaces(line);
        printf("[%s]\n", x);
        printf("=======");

        if (in_macro) {
            if (is_macro(line)) {
                add_error(assembler, "Nested macros are forbidden! found %s", line);
                break;
            }
            if (is_end_macro(line)) {
                in_macro = 0;
                continue;
            }
            add_line_to_macro(table->tail, line);
        } else if (is_macro(line)) {
            if (get_number_of_words(line, ' ') != 2) {
                add_error(assembler, "Wrong number of paramters to mcro, expected 2 but got %s", line);
                continue;
            }
            in_macro = 1;
            Macro *macro = new_macro(line);
            add_macro_to_table(table, macro);
        } else if (is_mcro_usage(line)) {
            Macro *macro = find_macro(table, line);
            if (macro == NULL) {
                add_error(assembler, "macro %s used without defining it", line);
                continue;
            }
            add_macro_to_content(parsed_file, macro);
        } else {  // regular line
            add_line_to_content(parsed_file, line);
        }
    }
    free(table);
    fclose(file);
    return parsed_file;
}

Macro *new_macro(char *header_line) {
    Macro *macro = (Macro *)malloc(sizeof(Macro));
    if (macro == NULL) {
        printf("Memory allocation error.\n");
        exit(1);
    }
    macro->next = NULL;
    macro->size = 0;
    macro->capacity = INITIAL_MACRO_SIZE;
    macro->content = (char **)malloc(INITIAL_MACRO_SIZE * sizeof(char *));

    header_line = remove_first_n_words(header_line, 1);
    header_line = trim_spaces(header_line);

    strcpy(macro->name, header_line);
    return macro;
}

MacroTable *new_macro_table() {
    MacroTable *macro_table = (MacroTable *)malloc(sizeof(MacroTable));
    macro_table->head = NULL;
    macro_table->tail = NULL;
    return macro_table;
}

void add_macro_to_table(MacroTable *table, Macro *macro) {
    if (table->head == NULL) {
        table->head = macro;
        table->tail = macro;
        return;
    }
    table->tail->next = macro;
    table->tail = macro;
}

Macro *find_macro(MacroTable *table, const char *string) {
    Macro *current = table->head;
    Macro *found = NULL;
    while (current != NULL) {
        if (strcmp(current->name, string) == 0) {
            found = current;
            break;
        }
        current = current->next;
    }
    return found;
}

int is_mcro_usage(const char *line) {
    int flag = 1;
    if (get_number_of_words(line, ' ') != 1) {
        flag = 0;
    }
    if (is_reserved_word(line)) {
        flag = 0;
    }
    return flag;
}

int is_macro(const char line[MAX_LINE_LENGTH]) {
    return strncmp(line, "mcro ", 5) == 0;
}

int is_end_macro(const char line[MAX_LINE_LENGTH]) {
    return strncmp(line, "endmcro", 5) == 0;
}

void add_macro_to_content(FileContent *file_content, Macro *macro) {
    int i;
    for (i = 0; i < macro->size; i++) {
        add_line_to_content(file_content, macro->content[i]);
    }
}

void add_line_to_macro(Macro *macro, char *line) {
    if (macro->size == macro->capacity) {
        macro->capacity *= 2;
        char **temp = (char **)realloc(macro->content, macro->capacity * sizeof(char *));
        if (temp == NULL) {
            printf("Memory reallocation error.\n");
            exit(1);
        }
        macro->content = temp;
    }

    macro->content[macro->size] = strdup(line);
    if (macro->content[macro->size] == NULL) {
        printf("Memory allocation error.\n");
        exit(1);
    }

    macro->size++;
}

void free_macro_table(MacroTable *table) {
    while (table->head != NULL) {
        Macro *tmp = table->head->next;
        free(table->head);
        table->head = tmp;
    }
    free(table);
}

void free_macro(Macro *macro) {
    for (size_t i = 0; i < macro->size; i++) {
        free(macro->content[i]);
    }
    free(macro->content);
    free(macro);
}
