// write method which gets a binary number and converts it to base64 string

int main()
{
    char *binary = "01101101011000010110111001100101";
    char *base64 = "bWVhc3VyZS4=";

    char *result = binary_to_base64(binary);

    printf("Binary: %s\n", binary);
    printf("Base64: %s\n", base64);
    printf("Result: %s\n", result);

    return 0;
}

// write binary_to_base64 function

char *binary_to_base64(char *binary)
{
    int length = strlen(binary);
    int base64_length = length / 6;
    char *base64 = (char *)malloc(base64_length + 1);
    int i = 0;
    int j = 0;
    int k = 0;
    int l = 0;
    int m = 0;
    int n = 0;
    int o = 0;
    int p = 0;

    for (i = 0; i < length; i += 6) {
        j = i + 1;
        k = i + 2;
        l = i + 3;
        m = i + 4;
        n = i + 5;
        o = i + 6;
        p = i + 7;

        if (o > length) {
            o = length;
        }

        if (p > length) {
            p = length;
        }

        if (binary[i] == '0' && binary[j] == '0' && binary[k] == '0' && binary[l] == '0' && binary[m] == '0' && binary[n] == '0') {
            base64[i / 6] = 'A';
        } else if (binary[i] == '0' && binary[j] == '0' && binary[k] == '0' && binary[l] == '0' && binary[m] == '0' && binary[n] == '1') {
            base64[i / 6] = 'B';
        } else if (binary[i] == '0' && binary[j] == '0' && binary[k] == '0' && binary[l] == '0' && binary[m] == '1' && binary[n] == '0') {
            base64[i / 6] = 'C';
        } else if (binary[i] == '0' && binary[j] == '0' && binary[k] == '0' && binary[l] == '0' && binary[m] == '1' && binary[n] == '1') {
            base64[i / 6] = 'D';
        } else if (binary[i] == '0' && binary[j] == '0' && binary[k] == '0' && binary[l] == '1' &&