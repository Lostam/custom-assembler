#ifndef DIRECTIVE_HANDLER_H
#define DIRECTIVE_HANDLER_H

#include "assembler.h"
#include "string_utils.h"

typedef enum
{
    DIRECTIVE_TYPE_DATA,
    DIRECTIVE_TYPE_STRING,
    DIRECTIVE_TYPE_ENTRY,
    DIRECTIVE_TYPE_EXTERNAL
} DirectiveType;

typedef struct
{
    const char *str;
    DirectiveType type;
} DirectiveMap;

typedef struct
{
    const char *symbol;
    DirectiveType type;
    char *type_str;// maybe join with type
    const char *params; // fixme make sure to remove whitespaces
} Directive;

void validate_directive(Assembler *, Statement *);
Directive *new_directive(Statement *);
char *get_directive_name(Statement *);
char *get_directive_params(Statement *);
DirectiveType string_to_directive(const char *);
void validate_data_params(Assembler *, Directive *);
int validate_string_params(Assembler *, Directive *);
void validate_entry_extern_params(Assembler *, Directive *);
void free_directive(Directive *);



#endif /* DIRECTIVE_HANDLER_H */
