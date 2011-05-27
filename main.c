#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <limits.h>

int n_input;
int n_cycles;
int n_components;
int *position;
int *component_id;
int *n_reality_edges;
int **cycle_id;
int **components;
signed int *sequence;
signed int **reality_graph;
signed int **desire_graph;
signed int ***cycles;

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
    sequence = (signed int *)calloc(n_input, sizeof(signed int));
    position = (int *)calloc(n_input + 2, sizeof(int));

    // converting to signed int array
    // +i means that i's orientation is -->
    // -i means that i's orientation is <--
    counter = 0;
    last_space = -1;
    for (i = 0; input[i] != '\0'; i++)
    {
        if (isspace(input[i]) || (i == file_len - 1))
        {
            for (j = 0; j < (i - last_space); j++)
                element[j] = input[last_space + j + 1];

            sequence[counter] = atoi(element);
            position[abs(atoi(element))] = counter;
            counter++;
            last_space = i;
        }
    }

    position[n_input + 1] = n_input + 1;

    free(input);
    free(element);
}

void printSequence()
{
    int i;

    printf("\n\nSequencia\n");

    for (i = 0; i < n_input; i++)
	{
		printf("%d, ", sequence[i]); 
	}
}

void createDesireGraph()
{
    int i;

    // desire_graph is a matrix (n_input + 2) by 2
    // desire_graph[i][0] stores the gene which -i is connected with, considering desire edges
    // desire_graph[i][1] stores the gene which +i is connected with, considering desire edges
    desire_graph = (signed int **)calloc(n_input + 2, sizeof(*desire_graph));

    for (i = 0; i <= n_input + 2; i++)
        desire_graph[i] = calloc(2, sizeof(signed int));

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
    reality_graph = (signed int **)calloc(n_input + 2, sizeof(*reality_graph));
    
    // cycle_id is a matrix (n_input + 2) by 2
    // cycle_id[i][0] stores the cycle id where the edge (-i, reality_graph[i][0]) is, plus 1
    // cycle_id[i][1] stores the cycle id where the edge (+i, reality_graph[i][1]) is, plus 1
    cycle_id = (int **)calloc(n_input + 2, sizeof(*cycle_id));
    
    for (i = 0; i <= n_input + 2; i++)
    {
        reality_graph[i] = calloc(2, sizeof(signed int));
        cycle_id[i] = calloc(2, sizeof(int));
    }

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
	int k;
	int middle;
    int temp_pos;
    signed int temp;

    printf("\n\nReversao(%d, ", i);
    printf("%d)\n", len);

	for (k = 0; k < len/2; k++)
	{
        temp_pos = position[abs(sequence[(i + len) - k - 1])];
		temp = sequence[(i + len) - k - 1];

        position[abs(sequence[(i + len) - k - 1])] = position[abs(sequence[i + k])];
		sequence[(i + len) - k - 1] = -sequence[i + k];

        position[abs(sequence[i + k])] = temp_pos;
		sequence[i + k] = -temp;
	}

	middle = (int)floor(len/2);
	if (len%2 == 1)
    {
        temp = sequence[i + middle];
        sequence[i + middle] = -temp;
    }

    updateRealityGraph(i, len);

    printSequence();
}

signed int **findCycle(int initial_pos, int id)
{
    int i;
    int j;
    signed int current_pos;

    // 'cycle' is the array for one cycle
    // cycle[i] stores a reality edge of this cycle
    // ex.: cycle[i][0] stores one node of the edge
    //      cycle[i][1] stores another node of the edge
    //      cycle[i][2] stores whether the edge is oriented from left to right (1) or from right to left (-1)
    //      cycle[i][3] stores its component id
    // as we do not know initially how many reality edges the cycle has, we first use 'temp_cycle', and then,
    // we create 'cycle' with the correct size
    signed int **cycle;
    signed int **temp_cycle;

    temp_cycle = (signed int **)calloc(n_input + 1, sizeof(*temp_cycle));
    for (i = 0; i != (n_input + 1); i++)
        temp_cycle[i] = calloc(3, sizeof(signed int));

    i = 0;
    j = 0;
    current_pos = initial_pos;
    while (i != 1)
    {
        // reality graph
        temp_cycle[j][0] = current_pos;
        if (current_pos >= 0)
        {
            temp_cycle[j][1] = reality_graph[current_pos][1];
            cycle_id[current_pos][1] = id;
        }
        else
        {
            temp_cycle[j][1] = reality_graph[-current_pos][0];
            cycle_id[-current_pos][0] = id;
        }

        if (temp_cycle[j][1] >= 0)
            cycle_id[temp_cycle[j][1]][1] = id;
        else
            cycle_id[-temp_cycle[j][1]][0] = id;
        
        if (position[abs(temp_cycle[j][0])] < position[abs(temp_cycle[j][1])])
            temp_cycle[j][2] = 1;
        else
            temp_cycle[j][2] = -1;

        // desire graph
        current_pos = temp_cycle[j][1];
        if (current_pos >= 0)
            current_pos = desire_graph[current_pos][1];
        else
            current_pos = desire_graph[-current_pos][0];

        if (current_pos == initial_pos)
            i++;

        j++;
    }

    cycle = (signed int **)calloc(j, sizeof(*cycle));
    for (i = 0; i != j; i++)
        cycle[i] = temp_cycle[i];

    free(temp_cycle);

    printf("\n\nCycle %d:\n", id - 1);
    printf("[ ");
    for (i = 0; i != j; i++)
    {
        printf("[%d", cycle[i][0]);
        printf(", %d", cycle[i][1]);
        printf(", %d] ", cycle[i][2]);
    }
    printf("]\n");

    return cycle;
}

void findAllCycles()
{
    int i;
    int j;

    // 'cycles' stores all the current cycles
    // as we do not know initially how many cycles there are, we first use 'temp_cycles', and then,
    // we create 'cycles' with the correct size
    signed int ***temp_cycles;

    temp_cycles = (signed int ***)calloc(n_input + 1, sizeof(**temp_cycles));

    j = 0;
    for (i = 0; i < n_input; i++)
    {
        if (cycle_id[abs(sequence[i])][0] == 0)
        {
            if (sequence[i] > 0)
                temp_cycles[j] = findCycle(-sequence[i], j + 1);
            else
                temp_cycles[j] = findCycle(sequence[i], j + 1);
            j++;
        }

        if (cycle_id[abs(sequence[i])][1] == 0)
        {
            if (sequence[i] > 0)
                temp_cycles[j] = findCycle(sequence[i], j + 1);
            else
                temp_cycles[j] = findCycle(-sequence[i], j + 1);
            j++;
        }
    }

    cycles = (signed int ***)calloc(j, sizeof(**cycles));
    for (i = 0; i != j; i++)
        cycles[i] = temp_cycles[i];

    n_cycles = j;

    free(temp_cycles);
}

int findComponents(int check)
{
    int i;
    int j;
    int k;
    int x;
    int y;
    int id;
    int last_cycle;
    int bad_cycles;
    int conv_edges;
    signed int direction;
    signed int last_element;
    signed int current_element;

    // 'components' stores all the good components
    // if a bad component is found, the algorithm will finish its execution
    // as we do not know initially how many good components there are, we first use 'temp_components',
    // and then, we create 'components' with the correct size
    int **temp_components;

    // 'component_size' stores the size of all components
    int *component_size;

    // finding all cycles first
    findAllCycles();

    // 'component_id' stores the component id of each cycle
    component_id = calloc(n_cycles, sizeof(int));

    // 'n_reality_edges' stores how many reality edges are in each cycle
    n_reality_edges = calloc(n_cycles, sizeof(int));
    
    temp_components = (int **)calloc(n_cycles, sizeof(*temp_components));
    component_size = calloc(n_cycles, sizeof(int));

    for (i = 0; i < n_cycles; i++)
        temp_components[i] = calloc(n_cycles, sizeof(int));

    // following the reality edges in order
    j = 0;
    last_cycle = 0;
    current_element = 0;
    last_element = 0;
    for (i = 0; i < n_input + 1; i++)
    {
        if (current_element >= 0)
            id = cycle_id[current_element][1] - 1;
        else
            id = cycle_id[-current_element][0] - 1;

        // if the cycle was found at least once
        if (n_reality_edges[id] > 0)
        {
            // if the cycle does not have a component yet
            if (component_id[id] == 0)
            {
                // if the last edge does not belong to the same cycle
                if (id != last_cycle)
                {
                    // if the cycle of the last edge does not have a component yet
                    // in that case, we associate both cycles to a new component
                    if (component_id[last_cycle] == 0)
                    {
                        temp_components[j][component_size[j]] = id;
                        component_id[id] = j + 1;
                        component_size[j]++;
                        temp_components[j][component_size[j]] = last_cycle;
                        component_id[last_cycle] = j + 1;
                        component_size[j]++;
                        j++;
                    }
                    // if the cycle of the last edge does not have a component yet
                    // in that case, we associate the cycle of the current edge to the component of
                    // of the last edge --> the cycles cross each other, so they belong to the same
                    // component
                    else
                    {
                        k = component_id[last_cycle] - 1;
                        temp_components[k][component_size[k]] = id;
                        component_id[id] = component_id[last_cycle];
                        component_size[k]++;
                    }
                }
                // if the last edge belongs to the same cycle
                // in that case, we associate the cycle to a new component
                else
                {
                    temp_components[j][component_size[j]] = id;
                    component_id[id] = j + 1;
                    component_size[j]++;
                    j++;
                }
            }
            
        }
        else
        {
            // if the reality edge is equal to the desire edge
            if (current_element >= 0)
            {
                if (reality_graph[current_element][1] == desire_graph[current_element][1])
                {
                    temp_components[j][component_size[j]] = id;
                    component_id[id] = j + 1;
                    component_size[j]++;
                    j++;
                }
            }
            else
            {
                if (reality_graph[-current_element][0] == desire_graph[-current_element][0])
                {
                    temp_components[j][component_size[j]] = id;
                    component_id[id] = j + 1;
                    component_size[j]++;
                    j++;
                }
            }
        }

        n_reality_edges[id]++;
        last_element = current_element;
        last_cycle = id;

        if (current_element >= 0)
            current_element = -reality_graph[current_element][1];
        else
            current_element = -reality_graph[-current_element][0];
    }

    n_components = j;
    components = (int **)calloc(n_components, sizeof(*components));

    for (i = 0; i < n_components; i++)
    {
        // checking if the component is bad
        bad_cycles = 0;
        for (x = 0; x < component_size[i]; x++)
        {
            // checking whether the cycle is good or bad
            id = temp_components[i][x];
            direction = cycles[id][0][2];
            conv_edges = 1;

            // cycles with only one reality edge are not taken into account
            if (n_reality_edges[id] != 1)
            {
                for (y = 1; y < n_reality_edges[id]; y++)
                {
                    if (cycles[id][y][2] == direction)
                        conv_edges++;
                }

                if (conv_edges == n_reality_edges[id])
                    bad_cycles++;
            }
        }

        if (bad_cycles == component_size[i])
        {
            if (check == 1)
                return 1;
            else
            {
                printf("\nUma componente ruim foi encontrada. O algoritmo desenvolvido ");
                printf("nao trata sequencias que apresentem componentes ruins.\n");
                printf("Componente ruim:\n[ ");
                for (x = 0; x < component_size[i]; x++)
                    printf("%d ", temp_components[i][x]);
                printf("]\n\nO programa sera finalizado. Bye!\n");
                exit(1);

                return 0;
            }
        }

        components[i] = calloc(component_size[i], sizeof(int));
        for (k = 0; k < component_size[i]; k++)
            components[i][k] = temp_components[i][k];
    }

    printf("\n\nComponents:\n");
    printf("[ ");
    for (i = 0; i < n_components; i++)
    {
        printf("[");
        for (k = 0; k < component_size[i]; k++)
            printf("%d, ", components[i][k]);
        printf("]");
    }
    printf(" ]\n");

    free(temp_components);
    free(component_size);
    free(n_reality_edges);

    return 0;
}

void sortReversal()
{
    int output;

    // finding all the components
    output = findComponents(0);

    if (n_components == n_cycles)
        printf("Done!");
    else
    {

    }
}

int main(int argc, char *argv[])
{
    readInput(argv[1]);
    printSequence();

    createRealityGraph();
    //printRealityGraph();

    createDesireGraph();
    //printDesireGraph();

    sortReversal();

    //revert(0,3);
    //printSequence();
    //printRealityGraph();

	return 0;
}
