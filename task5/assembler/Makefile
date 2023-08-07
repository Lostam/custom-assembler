GCC = gcc -g -ansi -Wall
SOURCES := main.c assembler.c macros.c directive_handler.c statement_handler.c executor.c word_handler.c logger.c instruction_handler.c string_utils.c symbol_handler.c operation_handler.c macro_handler.c file_handler.c file_builder.c utils.c
OBJECT_FILES := ${SOURCES:.c=.o}

assembler: $(OBJECT_FILES)
	@ $(LINK.c) $(SOURCES) -o $@


