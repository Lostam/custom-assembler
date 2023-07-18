GCC = gcc -g -ansi -Wall
LINE_BREAK = "____________________________"

my_string:  my_string.o executors.o methods.o
	$(GCC) -o my_string my_string.o executors.o methods.o

my_string.o  :  my_string.c executors.h
	$(GCC) -c my_string.c

executors.o :  executors.c executors.h methods.h
	$(GCC) -c executors.c

methods.o : methods.c methods.h
	$(GCC) -c methods.c

run_tests :
	@for file in inputs/*; do \
  		echo "${LINE_BREAK} \nRunning file: $$file  \nFile Input : " \
		 && cat $$file && echo "\n" && ./my_string < $$file; \
	done
