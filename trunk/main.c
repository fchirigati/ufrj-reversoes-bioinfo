#include <stdio.h>
#include <stdlib.h>

char *input;
long file_len;

int **reality_matrix;
int **desire_matrix;

void readInput(char * filename)
{
    FILE *file = fopen(filename, "r");

    if (file == 0)
    {
        printf("Error while opening the input file.");
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    file_len = ftell(file);
    rewind(file);

    input = (char*)calloc(file_len + 1, sizeof(char));

    if (input == NULL)
    {
        printf("Insufficient memory to read the file.");
        exit(1);
    }

    fread(input, file_len, 1, file);
    input[file_len] = '\0';

    fclose(file);
}

void createDiagram()
{
    // guardar diagrama
    free(input);
}

int main(int argc, char *argv[])
{
    readInput(argv[1]);
    createDiagram();
    return 0;
}