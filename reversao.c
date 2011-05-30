#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#define MAX_PERM			10001
#define	MAX_REALIDADE		20002
#define NAO_ACHEI			-1
#define TAM_PERMUTACAO_EST	(2 * tam_permutacao)

typedef struct _ARCO
{
	int inicio;
	int fim;
	int orientacao;
	int em_s2;
} ARCO;

/* Indica a permutacao que estamos trabalhando */
int permutacao[MAX_PERM];

/* Vetor permutacao estendido, com a posicao das partes positivas e negativas */
int perm_estendida[MAX_REALIDADE];

/* Vetor com os arcos que trabalharemos, na matriz de adjacência, alem de sua orientação.
Na posicao 0, o inicio indica onde esta 0+ e o fim indica onde está 1-. É o arco v0. E segue para os outros arcos.
Quando fim = inicio + 1, significa que o arco está isolado. 
Quando a orientacao é 1, significa que é orientado. Quando é -1, significa que é não orientado. Quando é 0, temos um arco isolado. */
ARCO arcos[MAX_PERM];

/* Vetores S1 e S2, para armazenar as reversoões. Os elementos indicam a posicao  */
int s1[MAX_REALIDADE], s2[MAX_REALIDADE], final[MAX_PERM];

/* Tamanho da permutacao */
int tam_permutacao;

/* Tamanhos de s1 e s2 */
int tam_s1, tam_s2;

/* "Ponteiros" para s1, s2 e final */
int inic_s1, fim_s1;
int inic_s2, fim_s2;
int fim_final;

void ipe(void);
void iarcos(void);
void inicializacao_programa(void);
void le_entrada(void);
void sistema(void);
void gera_arcos(void);
int acha_vertice_orientado(void);
void reverte(ARCO arco);
int tudo_isolado(void);
int primeira_pos_s2_nao_orientado(void);
void insere_em_s1(int pos);
void remove_de_s1(int pos);
void insere_em_s2(int pos);
void gera_final(void);

int main(int argc, char *argv[])
{
	int i;
	
//	ARCO temp;
//	temp.inicio = 1;
//	temp.fim = 4;
	
	
	inicializacao_programa();
	le_entrada();
	sistema();

	for (i = 0; i < fim_final; i++)
	{
		printf("( %d %d )\n", arcos[i].inicio, arcos[i].fim);
		ipe();
	}

//	gera_arcos();
//	reverte(temp);

//	for (i = 0; i < tam_permutacao; i++)
//	{
//		printf("Arco %d = (%d %d) orientacao %d\n", i, arcos[i].inicio, arcos[i].fim, arcos[i].orientacao);
//	}
	
	return 0;
}

/* IPE = Imprime Permutacao Estendida. Ê preguiça. ;-D */
void ipe(void)
{
	int i;
	printf("\n");
	for (i = 0; i < tam_permutacao * 2; i++)
	{
		printf("%d ", perm_estendida[i]);
	}
	printf("\n\n");
}

/* Imprime Arcos */
void iarcos(void)
{
	int i;
	
	for (i = 0; i < tam_permutacao; i++)
	{
		printf("Arco %d = (%d %d) orientacao %d\n", i, arcos[i].inicio, arcos[i].fim, arcos[i].orientacao);
	}
}
/* Zera tudo */
void inicializacao_programa(void)
{
	int i;
	
	memset(permutacao, 0, sizeof(permutacao));
	memset(perm_estendida, 0, sizeof(perm_estendida));
	
	for (i = 0; i < MAX_PERM; i++)
	{
		arcos[i].inicio = arcos[i].fim = arcos[i].orientacao = arcos[i].em_s2 = 0;
	}
	
	memset(s1, 0, sizeof(s1));
	memset(s2, 0, sizeof(s2));
	memset(final, 0, sizeof(final));
	
	fim_s1 = inic_s1 = 0;
	inic_s2 = fim_s2 = MAX_REALIDADE;
	fim_final = 0;
}

void le_entrada(void)
{
	int i = 0;
	
	perm_estendida[0] = 0;
	
	while(1)
	{
		if (scanf("%d", &permutacao[i]) == -1) break;
		
		perm_estendida[2 * i + 1] = permutacao[i] * -1;
		perm_estendida[2 * i + 2] = permutacao[i];
		i++;
	}
	perm_estendida[2 * i + 1] = (i + 1) * -1;
	
	tam_permutacao = i + 1;
	
	printf("inicio\n");
	ipe();
}

/* Chamada para execucao do sistema. Aqui o algoritmo é rodado */
void sistema(void)
{
	int pos_elemento;
	int i;
	int ultima_reversao_inicio;
	int ultima_reversao_final;
	int v_orientado;
	
	/* Passo 0 */
	gera_arcos();
	
	while(1)
	{
		/* Passo 1 */
		while(1)
		{
			pos_elemento = acha_vertice_orientado();
			printf("acha_vertice_orientado() = %d\n", pos_elemento);
			if (pos_elemento == NAO_ACHEI) break;
			
			insere_em_s1(pos_elemento);
			
			reverte(arcos[pos_elemento]);
			printf("reversao1\n");
			ipe();
			iarcos();
		}
		/* caso primeiro elemento de s2 seja nao orientado, reverte de novo */
		if (primeira_pos_s2_nao_orientado())
		{
			reverte(arcos[pos_elemento]);
			printf("reversao2\n");
			ipe();
			iarcos();
		}
		
		/* Passo 2 */
		if (tudo_isolado()) break;
		
		for (i = fim_s1; i >= inic_s1; i--)
		{
			printf("pos do elemento = %d\n", s1[i]);
			printf("arcos do inicio = %d arcos do fim = %d\n", arcos[s1[i]].inicio, arcos[s1[i]].fim);
			reverte(arcos[s1[i]]);
			printf("reversao3\n");
			ipe();
			iarcos();
			v_orientado = acha_vertice_orientado();
			if (v_orientado != NAO_ACHEI) break;
		}
		
		remove_de_s1(i);
		
		//ipe();
	}
	
	/* Passo 3 */
	
	gera_final();

}

void gera_arcos(void)
{
	int i;
	
	arcos[0].inicio = 0;
	
	/* Gero o arco */
	for (i = 1; i < TAM_PERMUTACAO_EST; i++)
	{
		if (perm_estendida[i] < 0)
		{
			arcos[abs(perm_estendida[i]) - 1].fim = i;
		}
		else
		{
			arcos[perm_estendida[i]].inicio = i;
		}
	}
	
	/* Se é orientado ou nao */
	for(i = 0; i < tam_permutacao; i++)
	{		
		if (perm_estendida[arcos[i].inicio] == (perm_estendida[arcos[i].fim - 1]))
		{
			arcos[i].orientacao = 0;
		}
		else if (abs(perm_estendida[arcos[i].inicio]) == abs(perm_estendida[arcos[i].inicio + 1]))
		{
			if (i == (tam_permutacao - 1))
			{
				arcos[i].orientacao = 1;
			}
			else if (abs(perm_estendida[arcos[i].fim]) == abs(perm_estendida[arcos[i].fim + 1]))
			{
				arcos[i].orientacao = 1;
			}
			else
			{
				arcos[i].orientacao = -1;
			}
		}
		else
		{
			if (i == (tam_permutacao - 1))
			{
				arcos[i].orientacao = -1;
			}
			else if (abs(perm_estendida[arcos[i].fim]) == abs(perm_estendida[arcos[i].fim + 1]))
			{
				arcos[i].orientacao = -1;
			}
			else
			{
				arcos[i].orientacao = 1;
			}
		}	
	}

	iarcos();
}

int acha_vertice_orientado(void)
{
	int i;
	
	for (i = 0; i < tam_permutacao; i++)
	{
		if (arcos[i].orientacao == 1)
		{
			if (arcos[i].em_s2 == 0)
			{			
				return i;
			}
		}
	}
	return NAO_ACHEI;
}

void reverte(ARCO arco)
{
	int i;
	int j;
	int temp;
	
	
/*
	if (perm_estendida[arco.inicio] == 0)
	{
		arco.inicio++;
	}
	
	if (perm_estendida[arco.fim] == TAM_PERMUTACAO_EST)
	{
		arco.fim--;
	}
*/
	
	if (abs(perm_estendida[arco.inicio]) == abs(perm_estendida[arco.inicio + 1]))
	{
		if (abs(perm_estendida[arco.fim]) == abs(perm_estendida[arco.fim + 1]))
		{
			//caso 2
			arco.fim--;
		}
		// caso 4. Nao altera nada.
	}
	else
	{
		if (abs(perm_estendida[arco.fim]) == abs(perm_estendida[arco.fim + 1]))
		{
			//caso 1
			arco.inicio++;
			arco.fim--;
		}
		else
		{
			//caso 3
			arco.inicio++;
		}
		
	}
/*			
//		printf("entrei1\n");
		arco.inicio--;
	}
		
	if (abs(perm_estendida[arco.fim]) == abs(perm_estendida[arco.fim + 1]))
	{
//		printf("entrei2\n");
		arco.fim++;
	}
*/

	if (arco.inicio > arco.fim)
	{
		i = arco.fim;
		j = arco.inicio;
	}
	else
	{
		i = arco.inicio;
		j = arco.fim;
	}
	printf("arco.inicio = %d arco.fim = %d\n", arco.inicio, arco.fim);
	
	//arcos[perm_estendida[i]].orientacao *= -1;
	
	while (i < j)
	{
		if (perm_estendida[i] < 0)
		{
			arcos[abs(perm_estendida[i]) - 1].orientacao *= -1;
		}
		else
		{
			arcos[abs(perm_estendida[i])].orientacao *= -1;
		}
		
		if (perm_estendida[j] < 0)
		{
			arcos[abs(perm_estendida[i]) - 1].orientacao *= -1;
		}
		else
		{
			arcos[abs(perm_estendida[j])].orientacao *= -1;
		}
		
		printf("arcos[%d].orientacao = %d\n", abs(perm_estendida[i]), arcos[abs(perm_estendida[i])].orientacao);
		printf("arcos[%d].orientacao = %d\n", abs(perm_estendida[j]), arcos[abs(perm_estendida[j])].orientacao);
			
		temp = perm_estendida[j];
			
		printf("perm_estendida[%d] = %d\n", i, perm_estendida[i]);
			
		if (perm_estendida[i] < 0)
		{
			printf("menor que 0.\narcos[%d].fim = %d -- recebe j = %d\n",abs(perm_estendida[i]) - 1, arcos[abs(perm_estendida[i]) - 1].fim, j);
			arcos[abs(perm_estendida[i]) - 1].fim = j;
		}
		else
		{
			printf("maior que 0.\narcos[%d].inicio = %d -- recebe j = %d\n",abs(perm_estendida[i]), arcos[abs(perm_estendida[i])].inicio, j);
			arcos[perm_estendida[i]].inicio = j;
		}

		if (temp < 0)
		{
			arcos[abs(temp) - 1].fim = i;
		}
		else
		{
			arcos[temp].inicio = i;
		}
			
		temp = perm_estendida[j];
		perm_estendida[j] = perm_estendida[i];
		perm_estendida[i] = temp;
			
		i++;
		j--;
	}
	
	//ipe();	
}

int tudo_isolado(void)
{
	int i;
	
	for (i = 0; i < tam_permutacao; i++)
	{
		if (arcos[i].orientacao != 0)
		{
			return 0;
		}
	}
	return 1;
}

int primeira_pos_s2_nao_orientado(void)
{
	if (arcos[inic_s2].orientacao == -1) return 1;
	return 0;
}

void insere_em_s1(int pos)
{
	s1[fim_s1] = pos;
	fim_s1++;
}

void remove_de_s1(int pos)
{
	int i;
	
	for(i = pos; i < fim_s1; i++)
	{
		insere_em_s2(pos);
	}
	
	fim_s1 = pos;
}

void insere_em_s2(int pos)
{
	arcos[perm_estendida[pos]].em_s2 = 1;
	s2[inic_s2] = pos;
	inic_s2--;
}

void gera_final(void)
{
	int pos = 0;
	int i;
	
	for (i = inic_s1; i < fim_s1; i++)
	{
		final[pos] = s1[i];
		pos++;
	}
	for (i = inic_s2 + 1; i <= fim_s2; i++)
	{
		final[pos] = s2[i];
		pos++;
	}
	
	fim_final = pos;
}

