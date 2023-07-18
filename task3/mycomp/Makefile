GCC = gcc -g -ansi -Wall
LINE_BREAK = "____________________________"


mycomp:  mycomp.o complex.o validator.o
	$(GCC) -o mycomp mycomp.o complex.o validator.o

mycomp.o:  mycomp.c mycomp.h
	$(GCC) -c mycomp.c

validator.o:  validator.c validator.h
	$(GCC) -c validator.c

complex.o :  complex.c complex.h
	$(GCC) -c complex.c




run_tests :
	@for file in inputs/*; do \
  		echo "${LINE_BREAK} \nRunning file: $$file  \nFile Input : " \
		 && cat $$file && echo "\n" && ./mycomp < $$file; \
	done


