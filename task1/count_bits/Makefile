GCC = gcc -g -ansi -Wall
LINE_BREAK = "____________________________"

count_bits:  count_bits.o
	$(GCC) -o count_bits count_bits.o

count_bits.o  :  count_bits.c count_bits.h
	$(GCC) -c count_bits.c

run_tests :
	@for file in inputs/*; do \
  		echo "${LINE_BREAK} \nRunning file: $$file  \nFile Input : " \
		 && cat $$file && echo "\n" && ./count_bits < $$file; \
	done
