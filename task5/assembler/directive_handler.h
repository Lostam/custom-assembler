#ifndef DIRECTIVE_HANDLER_H
#define DIRECTIVE_HANDLER_H

typedef struct directive_s Directive;
typedef struct directive_map_s DirectiveMap;
typedef enum directive_type_s DirectiveType;

#include "assembler.h"
#include "statement_handler.h"
#include "string_utils.h"

enum directive_type_s {
    DIRECTIVE_TYPE_DATA,
    DIRECTIVE_TYPE_STRING,
    DIRECTIVE_TYPE_ENTRY,
    DIRECTIVE_TYPE_EXTERNAL
};

struct directive_map_s {
    const char *str;
    DirectiveType type;
};

struct directive_s {
    const char *symbol;
    DirectiveType type;
    char *type_str;      // maybe join with type
    const char *params;  // fixme make sure to remove whitespaces
};

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
