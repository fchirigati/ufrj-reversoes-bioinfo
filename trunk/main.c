#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <limits.h>

int n_input;
signed int *sequence;
signed int **reality_graph;
signed int **desire_graph;

void readInput(char *filename)
{
    int i;
    int j;
    int n_spaces;
    int counter;
    int last_space;
    long file_len;
    char *input;
    char *element;
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
    n_spaces = 0;
    for (i = 0; input[i] != '\0'; i++)
    {

        if (isspace(input[i]) && (input[i+1] != '\0'))
            n_spaces++;   
    }
    
    // input size is the number of spaces plus one
    n_input = n_spaces + 1;

    element = (char *)calloc(n_input, sizeof(char));
    sequence = (int *)calloc(n_input, sizeof(int));

    // converting to signed int array
    // +i means that i's orientation is -->
    // -i means that i's orientation is <--
    counter = 0;
    last_space = -1;
    for (i = 0; input[i] != '\0'; i++)
    {
        if (isspace(input[i]))
        {
            for (j = 0; j < (i - last_space); j++)
                element[j] = input[last_space + j + 1];

            sequence[counter] = atoi(element);
            counter++;
            last_space = i;
        }
    }

    i++;
}

void printSequence()
{
    int i;

    printf("\n\nSequencia\n");

    for (i = 0; i < n_input; i++)
	{
		printf("%d, ",sequence[i]); 
	}
}

void createDesireGraph()
{
    int i;

    // desire_graph is a matrix (n_input + 2) by 2
    // desire_graph[i][0] stores the gene which -i is connected with, considering desire edges
    // desire_graph[i][1] stores the gene which +i is connected with, considering desire edges
    desire_graph = (signed int **)malloc((n_input + 2) * sizeof(*desire_graph));

    for (i = 0; i <= n_input + 2; i++)
        desire_graph[i] = malloc(2 * sizeof(signed int));

    desire_graph[0][0] = INT_MAX;
    desire_graph[n_input + 1][1] = INT_MAX;

    for (i = 0; i < n_input + 2; i++)
    {
        if (i == 0)
            desire_graph[i][1] = -(i + 1);
        else if (i == n_input + 1)
            desire_graph[i][0] = +(i - 1);
        else
        {
            desire_graph[i][0] = +(i - 1);
            desire_graph[i][1] = -(i + 1);
        }
    }
}

void printDesireGraph()
{
    int i;

    printf("\n\nArestas de Desejo\n");

    for (i = 0; i <= n_input + 1; i++)
    {
        printf("(%d, ", desire_graph[i][0]);
        printf("%d)", desire_graph[i][1]);	
    }
}

void createRealityGraph()
{
    int i;
    int next;
    signed int temp;
    
    // reality_graph is a matrix (n_input + 2) by 2
    // reality_graph[i][0] stores the gene which -i is connected with, considering reality edges
    // reality_graph[i][1] stores the gene which +i is connected with, considering reality edges
    reality_graph = (signed int **)malloc((n_input + 2) * sizeof(*reality_graph));
    
    for (i = 0; i <= n_input + 2; i++)
        reality_graph[i] = malloc(2 * sizeof(signed int));

    reality_graph[0][0] = INT_MAX;
    next = 0;
    for (i = 0; i < n_input; i++)
    {
        temp = -sequence[i];

        if (next >= 0)
            reality_graph[next][1] = temp;
        else
            reality_graph[-next][0] = temp;
	
        if(temp >= 0)
            reality_graph[temp][1] = next;
        else
            reality_graph[-temp][0] = next;

        next = sequence[i];
    }

    temp = -(n_input + 1);

    if(next >= 0)
        reality_graph[next][1] = temp;
    else
        reality_graph[-next][0] = temp;

    reality_graph[-temp][0] = next;
    reality_graph[-temp][1] = INT_MAX;
}

void printRealityGraph()
{
    int i;

    printf("\n\nArestas de Realidade\n");

    for (i = 0; i <= n_input + 1; i++)
    {
        printf("(%d, ", reality_graph[i][0]);
        printf("%d)", reality_graph[i][1]);	
    }
}

void updateRealityGraph(int i, int len)
{
    // first edge
    if (i == 0)
        reality_graph[0][1] = -sequence[i];
    else
    {
        if (sequence[i - 1] > 0)
            reality_graph[sequence[i - 1]][1] = -sequence[i];
        else
            reality_graph[-sequence[i - 1]][0] = -sequence[i];
    }

    if (sequence[i] > 0)
    {
        if (i == 0)
            reality_graph[sequence[i]][0] = 0;
        else
            reality_graph[sequence[i]][0] = sequence[i - 1];
    }
    else
    {
        if (i == 0)
            reality_graph[sequence[i]][1] = 0;
        else
            reality_graph[-sequence[i]][1] = sequence[i - 1];
    }

    // second edge
    if ((i + len) == n_input)
        reality_graph[n_input + 1][0] = sequence[n_input - 1];
    else
    {
        if (sequence[i + len] > 0)
            reality_graph[sequence[i + len]][0] = sequence[i + len - 1];
        else
            reality_graph[-sequence[i + len]][1] = sequence[i + len - 1];
    }

    if (sequence[i + len - 1] > 0)
    {
        if ((i + len) == n_input)
            reality_graph[sequence[i + len - 1]][1] = -(i + len + 1);
        else
            reality_graph[sequence[i + len - 1]][1] = -sequence[i + len];
    }
    else
    {
        if ((i + len) == n_input)
            reality_graph[-sequence[i + len - 1]][0] = -(i + len + 1);
        else
            reality_graph[-sequence[i + len - 1]][0] = -sequence[i + len];
    }
}

void revert(int i, int len)
{
	signed int temp;
	int k;
	int middle;

    printf("\n\nReversao(%d, ", i);
    printf("%d)\n", len);

	for (k = 0; k < len/2; k++)
	{
		temp = sequence[(i + len) - k - 1];
		sequence[(i + len) - k - 1] = -sequence[i + k];  	
		sequence[i + k] = -temp;
	}

	middle = (int)floor(len/2);
	if (len%2 == 1)
    {
        temp = sequence[i + middle];
        sequence[i + middle] = -temp;
    }

    updateRealityGraph(i, len);
} 


int main(int argc, char *argv[])
{
    readInput(argv[1]);
    printSequence();

    createDesireGraph();
    printDesireGraph();

    createRealityGraph();
    printRealityGraph();

    //revert(0,5);
    //printSequence();
    //printRealityGraph();

	return 0;
}
