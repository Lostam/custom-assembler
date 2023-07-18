typedef struct complex {
  double real;
  double imaginary;
} Complex;

void print(Complex *);
void add(Complex *, Complex *);
void subtract(Complex *, Complex *);
void multiply(Complex *, Complex *);
void multiply_comp_img(Complex *, int);
void multiply_comp_real(Complex *, int);
void absolute(Complex *);