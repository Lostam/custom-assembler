GCC = gcc -g -ansi -Wall
LINE_BREAK = "____________________________"


fibon:  fibon.o
	$(GCC) -o fibon fibon.o

fibon.o:  fibon.c fibon.h
	$(GCC) -c fibon.c

run_tests :
	@for file in inputs/*; do \
  		echo "${LINE_BREAK} \nRunning test: $$file  \nArgs and User Input : " \
		 && echo "," | cat $$file/args - $$file/userInput  && echo "\n" && ./fibon $(cat $$file/args) < $$file/userInput; \
	done


