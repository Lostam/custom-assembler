GCC = gcc -g -ansi -Wall
SOURCES := main.c assembler.c macros.c directive_handler.c statement_handler.c executor.c word_handler.c logger.c instruction_handler.c string_utils.c symbol_handler.c operation_handler.c macro_handler.c file_handler.c file_builder.c utils.c
OBJECT_FILES := ${SOURCES:.c=.o}
LINE_BREAK = "____________________________"
# 
# C Preprocessor Flags
# CPPFLAGS += 
# # compiler flags for gcc
# CFLAGS += -ansi -Wall -Wextra -pedantic-errors
# # some flags to make things easier on ValGrind, add to CFLAGS
# VGFLAGS := -g -fno-inline-functions -fno-inline-functions-called-once \
#         -fno-optimize-sibling-calls

assembler: $(OBJECT_FILES)
	@ $(LINK.c) $(SOURCES) -o $@

run_tests :
	@for file in inputs/*; do \
  		echo "${LINE_BREAK} \nRunning test: $$file  \nArgs and User Input : " \
		 && echo "," | cat $$file/args - $$file/userInput  && echo "\n" && ./assembler $(cat $$file/args) < $$file/userInput; \
	done


