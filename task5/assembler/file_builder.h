#ifndef FILE_BUILDER_H
#define FILE_BUILDER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "assembler.h"
#include "file_handler.h"

/**
 * @brief Generates .obj/.ent/.ext files if needed
*/
void generate_output_files(Assembler *);
/**
 * @brief Generates the content of obj file
*/
FileContent *get_obj_file_content(Assembler *);
/**
 * @brief Generates the content of ent file
*/
FileContent *get_ent_file_content(Assembler *);
/**
 * @brief Generates the content of ext file
*/
FileContent *get_ext_file_content(Assembler *);
/**
 * @brief Converts a word content to base64
*/
char *word_to_base64(char *);
    
#endif /* FILE_BUILDER_H */


