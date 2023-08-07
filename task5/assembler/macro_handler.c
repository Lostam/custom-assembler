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

void parse_macros(Assembler *assembler) {
    FileContent *parsed_file = get_parsed_file(assembler);
    if (parsed_file == NULL) {
        return;
    }
    info("Succesfully parsed the %s.as file", assembler->file_basename);

    write_to_file(assembler, parsed_file);
}

FileContent *get_parsed_file(Assembler *assembler) {
    char *filename = get_as_filename(assembler->file_basename);
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        add_error(assembler, "Failed opening %s, it either do not exist or you don't have permission to open the file", filename);
        free(filename);
        return NULL;
    }
    info("Started parsing %s content", filename);
    free(filename);
    MacroTable *table = new_macro_table();
    FileContent *parsed_file = new_file_content(get_am_filename(assembler->file_basename));
    int in_macro = 0;
    char line[MAX_LINE_LENGTH];
    while (!feof(file)) {
        memset(line, '\0', MAX_LINE_LENGTH);
        assembler->current_line_number++;
        char c;
        int i;
        int arr_i = 0;
        int found_first_string = 0;
        for (i = 0; i <= MAX_LINE_LENGTH; i++) {
            c = fgetc(file);
            if (!found_first_string) {
                if (isspace(c)) {
                    continue;
                } else {
                    found_first_string = 1;
                }
            }
            if (c == '\n' || c == EOF) {
                line[i] = '\0';
                break;
            }
            line[arr_i++] = c;
        }
        if (line[sizeof(line) - 1] != '\0') {
            add_error(assembler, "Invalid longer then 80 letters line found in line number : [%d] and starting with : [%s]", assembler->current_line_number, line);
            while ((c = fgetc(file)) != '\n' && c != EOF) {
            }
            continue;
        }

        assembler->current_line = line;
        if (in_macro && is_macro(line)) {
            add_error(assembler, "Nested macros are forbidden, found [%s], stopping operation", line);
            break;
        }
        parse_line(assembler, table, parsed_file, line, &in_macro);
    }
    free_macro_table(table);
    fclose(file);
    return parsed_file;
}

void parse_line(Assembler *assembler, MacroTable *table, FileContent *parsed_file, char *line, int *in_macro) {
    if (*in_macro) {
        if (is_end_macro(line)) {
            debug("emdmcro found in line [%d] with content [%s]", assembler->current_line_number, line);
            *in_macro = 0;
            return;
        }
        add_line_to_macro(table->tail, line);
    } else if (is_macro(line)) {
        debug("Macro found in line %d with content %s", assembler->current_line_number, line);
        char delmiter[] = " ";
        if (get_number_of_words(line, delmiter) != 2) {
            add_error(assembler, "Wrong number of paramters to mcro, expected 2 but got %s", line);
            return;
        }
        *in_macro = 1;
        Macro *macro = new_macro(line);
        add_macro_to_table(table, macro);
    } else if (is_mcro_usage(line)) {
        Macro *macro = find_macro(table, line);
        if (macro == NULL) {
            add_error(assembler, "macro %s used without defining it in line %d", line, assembler->current_line_number);
            return;
        }
        add_macro_to_content(parsed_file, macro);
    } else {  // regular line
        add_line_to_content(parsed_file, line);
    }
}

Macro *new_macro(const char *header_line) {
    char *copy = remove_first_n_words(header_line, 1);
    Macro *macro = (Macro *)malloc(sizeof(Macro));
    if (macro == NULL) {
        error("Memory allocation error");
        exit(1);
    }
    macro->next = NULL;
    macro->size = 0;
    macro->capacity = INITIAL_MACRO_SIZE;
    macro->content = (char **)malloc(INITIAL_MACRO_SIZE * sizeof(char *));
    if (macro->content == NULL) {
        error("Memory allocation failed!");
        exit(1);
    }
    strcpy(macro->name, copy);
    free(copy);
    return macro;
}

MacroTable *new_macro_table() {
    MacroTable *macro_table = (MacroTable *)malloc(sizeof(MacroTable));
    if (macro_table == NULL) {
        error("Memory allocation failed!");
        exit(1);
    }
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
    char delmiter[] = " ";
    if (get_number_of_words(line, delmiter) != 1) {
        return 0;
    }
    if (is_reserved_word(line)) {
        return 0;
    }
    return 1;
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

void add_line_to_macro(Macro *macro, const char *line) {
    if (macro->size == macro->capacity) {
        info("Resizing macros file from %d to %d", macro->capacity, macro->capacity * 2);
        macro->capacity *= 2;
        char **temp = (char **)realloc(macro->content, macro->capacity * sizeof(char *));
        if (temp == NULL) {
            error("Memory reallocation error.\n");
            exit(1);
        }
        macro->content = temp;
    }

    macro->content[macro->size] = strdup(line);
    if (macro->content[macro->size] == NULL) {
        error("Memory allocation error");
        exit(1);
    }

    macro->size++;
}

void free_macro_table(MacroTable *table) {
    while (table->head != NULL) {
        Macro *tmp = table->head->next;
        free_macro(table->head);
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
