#include "file_builder.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler.h"
#include "file_handler.h"
#include "logger.h"
#include "macros.h"
#include "symbol_handler.h"
#include "word_handler.h"

void generate_output_files(Assembler *assembler) {
    FileContent *parsed_file = get_obj_file_content(assembler);
    if (parsed_file == NULL) {
        return;
    }
    write_to_file(assembler, parsed_file);

    FileContent *ent_file = get_ent_file_content(assembler);
    if (ent_file != NULL) {
        if (ent_file->size != 0) {
            write_to_file(assembler, ent_file);
        } else {
            info("Entry content is empty and so will not write %s file", ent_file->filename);
            free(ent_file);
        }
    } else {
        warn("Failed creating .ent file content");
    }

    FileContent *ext_file = get_ext_file_content(assembler);
    if (ext_file != NULL) {
        if (ext_file->size != 0) {
            write_to_file(assembler, ext_file);
        } else {
            info("External content is empty and so will not write %s file", ext_file->filename);
            free(ext_file);
        }
    } else {
        warn("Failed creating .ext file content");
    }
}

FileContent *get_obj_file_content(Assembler *assembler) {
    FileContent *obj_file = new_file_content(get_ob_filename(assembler->file_basename));

    Node *current_word = assembler->words_l_list->head;

    char *header = (char *)malloc(80);
    if (header == NULL) {
        error("Memory allocation failed!");
        exit(1);
    }
    sprintf(header, "%d %d", assembler->IC, assembler->DC);

    add_line_to_content(obj_file, header);
    while (current_word != NULL) {
        char *line = word_to_base64(current_word->content);
        add_line_to_content(obj_file, line);
        current_word = current_word->next;
    }
    return obj_file;
}

char *word_to_base64(char *binary) {
    char base_64_array[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    char *first = (char *)malloc(7);
    char *second = (char *)malloc(7);
    char *line = (char *)malloc(3);
    if (!first || !second || !line) {
        error("Memory allocation failed.\n");
        return NULL;
    }
    strncpy(first, binary, 6);
    strncpy(second, binary + 6, 6);
    first[6] = '\0';
    second[6] = '\0';
    sprintf(line, "%c%c", base_64_array[strtol(first, NULL, 2)], base_64_array[strtol(second, NULL, 2)]);
    free(first);
    free(second);
    return line;
}

FileContent *get_ent_file_content(Assembler *assembler) {
    FileContent *file = new_file_content(get_ent_filename(assembler->file_basename));

    SymbolTable *table = assembler->symbol_table;
    int i;
    for (i = 0; i < table->size; i++) {
        if (table->symbols[i]->is_entry) {
            char *line = (char *)malloc(MAX_LINE_LENGTH);
            if (line == NULL) {
                error("Memory allocation failed!");
                exit(1);
            }
            sprintf(line, "%s %d", table->symbols[i]->name, table->symbols[i]->line_number);
            add_line_to_content(file, line);
        }
    }
    return file;
}

FileContent *get_ext_file_content(Assembler *assembler) {
    FileContent *file = new_file_content(get_ext_filename(assembler->file_basename));

    SymbolTable *table = assembler->symbol_table;
    int i;
    for (i = 0; i < table->size; i++) {
        if (table->symbols[i]->type == SYMBOL_TYPE_EXTERNAL) {
            Node *current = table->symbols[i]->used_at->head;
            while (current != NULL) {
                add_line_to_content(file, current->content);
                current = current->next;
            }
        }
    }
    return file;
}