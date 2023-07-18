typedef enum{
NUMBER,
LETTER,
SEPARATOR,
} InputTypeEnum;

int startWith(char *, char *);
int letter_to_index(char);
void get_next_section(char *, char *, int, InputTypeEnum);
int validate_input(char *, InputTypeEnum *,int, char *, char **);
int isnumeric(char *);
int is_letter_valid(char *, int);
int is_numeric_valid(char *, int);
int is_comma_valid(char *, int);