GCC = gcc -g -ansi -Wall
LINE_BREAK = "____________________________"

my_set:  my_set.o #executors.o methods.o
	$(GCC) -o my_set  my_set.o # executors.o methods.o

my_set.o  :  my_set.c #executors.h
	$(GCC) -c my_set.c

run_tests :
	@for file in inputs/*; do \
  		echo "${LINE_BREAK} \nRunning file: $$file  \nFile Input : " \
		 && cat $$file && echo "\n" && ./my_set < $$file; \
	done


