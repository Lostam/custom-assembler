GCC = gcc -g -ansi -Wall
program_NAME := assembler
SOURCES := main.c assembler.c directive_handler.c statement_handler.c word_handler.c logger.c instruction_example.c binary_utils.c instruction_handler.c file_reader.c string_utils.c symbol_handler.c operation_handler.c
program_OBJS := ${SOURCES:.c=.o}
LINE_BREAK = "____________________________"


$(program_NAME): $(program_OBJS)
	@ $(LINK.c) $(SOURCES) -o $@

run_tests :
	@for file in inputs/*; do \
  		echo "${LINE_BREAK} \nRunning test: $$file  \nArgs and User Input : " \
		 && echo "," | cat $$file/args - $$file/userInput  && echo "\n" && ./assembler $(cat $$file/args) < $$file/userInput; \
	done


