#ifndef FILE_BUILDER_H
#define FILE_BUILDER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "assembler.h"
#include "file_handler.h"

void generate_output_files(Assembler *);
FileContent *get_obj_file_content(Assembler *);
FileContent *get_ent_file_content(Assembler *);
FileContent *get_ext_file_content(Assembler *);
char *word_to_base64(char *);
    
#endif /* FILE_BUILDER_H */


