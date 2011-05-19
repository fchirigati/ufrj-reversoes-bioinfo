#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int n_input;
char *input;
signed int *sequence;
signed int *reality_graph;
signed int *desire_graph;

char * readInput(char * filename)
{
    int i;
    int j = 0;
    int n_spaces = 0;
    long file_len;
    
    char gene[5];
    char s_gene;
    char temp[2];
    FILE *file = fopen(filename, "r");

    if (file == 0)
    {
        printf("Error while opening the input file.");
        exit(1);
    }

    // checking input file size
    fseek(file, 0, SEEK_END);
    file_len = ftell(file);
    rewind(file);

    // allocating memory for the input
    input = (char*)calloc(file_len + 1, sizeof(char));

    if (input == NULL)
    {
        printf("Insufficient memory to read the file.");
        exit(1);
    }

    // getting the input
    fread(input, file_len, 1, file);
    input[file_len] = '\0';

    fclose(file);

    // counting the number of spaces
    for (i = 0; input[i] != '\0'; i++)
    {
        if (isspace(input[i]))
            n_spaces++;
    }

    // input size is the number of spaces plus one
    n_input = n_spaces++;

    // getting the sequence from the input
    //sequence = (signed int*)calloc(n_input, sizeof(signed int));
    //for (i = 0; input[i] != '\0'; i++)
    //{
    //    if (!isspace(input[i]))
    //        strcat(gene, input[i]);
    //    else
    //    {
    //        temp[0] = gene;
    //        temp[1] = '\0';
    //        strcat(s_gene, temp);
    //        sequence[j] = atoi(s_gene);
    //        j++;
    //    }
    //}

    return input;
}

void createDesireGraph()
{
    int i;

    // allocating memmory for the graphs
    
    desire_graph = (signed int*)calloc(2*n_input + 4, sizeof(signed int));

    // creating desire edges
    for (i = 0; i < 2*n_input + 4; i++)
    {
        if (i%2 == 0)
            desire_graph[i] = -(i/2 + 1);
        else
            desire_graph[i] = (i-1)/2;    
       
    }
}

void createRealityGraph(){
  int i;
  int n_teste = 2;
  reality_graph = (signed int*)calloc(2*n_teste + 4, sizeof(signed int));

  // TIRAR ESSA PARTE AQUI, QUANDO DEFINIRMOS A ESTRUTURA EXATA DA ENTRADA
  signed int *teste;
  teste = (signed int*)calloc(3, sizeof(signed int));
  teste[0] = -1;
  teste[1] = 2;
  teste[2] = 3;
  
  
  int next = 0;
  for (i = 0; i < 2*n_teste + 2; i=i+2)
    {
        reality_graph[i] = next;
        reality_graph[i+1] = -teste[i/2];
        next = teste[i/2];
    }
    reality_graph[i] = next;
    reality_graph[i+1] = -4;
}

int main(int argc, char *argv[])
{
    readInput(argv[1]);
    createDesireGraph();
    createRealityGraph();
    return 0;
}
