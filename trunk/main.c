#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int n_input;
char *input;
signed int *sequence;
signed int *reality_graph;
signed int *desire_graph;

void readInput(char *filename)
{
    int i;
    int j;
    int n_spaces = 0;
    long file_len;
    
    char *gene;
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
	{
            n_spaces++;
	}	
	        
    }
    
// input size is the number of spaces plus one
    n_input = n_spaces++;

    char *element = (char *)calloc(2, sizeof(char));
    sequence = (int *)calloc(n_input, sizeof(int));
   // converting to the signed int array
    int counter = 0;
    for (i = 0; input[i] != '\0'; i++)
    {
        if (isspace(input[i]))
	{
            element[0] = input[i-2];
	    element[1] = input[i-1];
	    
	    sequence[counter] = atoi(element);	
            printf("%d,", sequence[counter]);
	    counter++;
	}	
    }

    

/*
    // getting the sequence from the input
    sequence = (signed int *)calloc(n_input, sizeof(signed int));
    gene = (char *)calloc(6, sizeof(char));
    j = 0;
    for (i = 0; input[i] != '\0'; i++)
    {
        if (!isspace(input[i]))
        {
            printf("%c\n", input[i]);
            printf("gene: '%s'\n", gene);
            gene = strcat(gene, (char *)input[i]);
        }
        else
        {
            sequence[j] = atoi(gene);
            gene = strcpy(gene, "");
            j++;
        }
    }
*/
}

void createDesireGraph()
{
    int i;

    // allocating memmory for the graphs
    desire_graph = (signed int*)calloc(2*n_input + 2, sizeof(signed int));

    // creating desire edges
    int next = 0;    
    printf("\nARESTAS DE DESEJO\n");   
    for (i = 1; i < n_input+2; i++)
    {
        desire_graph[i] = next;
	desire_graph[i+1] = -i;
	next = i;
        printf("(%d",desire_graph[i]);
	printf(",%d)",desire_graph[i+1]); 
    }
}

void createRealityGraph()
{
    int i;
    int next = 0;
    reality_graph = (signed int*)calloc(2*n_input + 2, sizeof(signed int));

    printf("\nARESTAS DE REALIDADE\n");
    for (i = 0; i < 2*n_input; i=i+2)
    {
        reality_graph[i] = next;
        reality_graph[i+1] = -sequence[i/2];
        next = sequence[i/2];
	printf("(%d",reality_graph[i]);
	printf(",%d)",reality_graph[i+1]);
    }
    reality_graph[i] = next;
    reality_graph[i+1] = -(n_input+1);
    printf("(%d",reality_graph[i]);
    printf(",%d)",reality_graph[i+1]);
}

int main(int argc, char *argv[])
{
    readInput(argv[1]);
    createDesireGraph();
    createRealityGraph();
    return 0;
}
