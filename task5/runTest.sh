LINE_BREAK="____________________________New Input____________________________"
OUTPUT_START="_____________Output_____________\n"
OUTPUT_END="\n_____________End Output_____________\n"

for file in inputs/*; do \
  		echo "${LINE_BREAK} \nRunning test: $file  \nArgs and User Input : " \
		 && echo "command line input: $(cat $file/args) | user input: $(cat $file/userInput)"  && printf "\n" && \
		  ./fibon $(cat "${file}/args") < "$file/userInput" && echo ${OUTPUT_START} && cat $(cat "$file/args") && echo ${OUTPUT_END};\
done