typedef struct {
  int *numbers;
  int size;
  int current_index;
} MySet;

/*
Receives a set struct contain an array and an integer
returns 1 if the integer is in the array and 0 otherwise
*/
int is_in_set(MySet *, int);

/*
Receives a set struct contain an array and an integer and prints all it variables
*/
void print_set(MySet *);

/*
Receives a set struct contain an array and an integer
will add the integer to the array and resize the array if necessary
*/
void add_to_array(MySet *, int);
