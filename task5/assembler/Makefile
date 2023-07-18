GCC = gcc -g -ansi -Wall
program_NAME := assembler
program_SRCS := assembler.c operation_handler.c statementer.c string_utils.c directive_handler.c logger.c
program_OBJS := ${program_SRCS:.c=.o}
LINE_BREAK = "____________________________"


$(program_NAME): $(program_OBJS)
	@ $(LINK.c) $(program_SRCS) -o $@

run_tests :
	@for file in inputs/*; do \
  		echo "${LINE_BREAK} \nRunning test: $$file  \nArgs and User Input : " \
		 && echo "," | cat $$file/args - $$file/userInput  && echo "\n" && ./assembler $(cat $$file/args) < $$file/userInput; \
	done


