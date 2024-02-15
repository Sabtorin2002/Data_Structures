#include<stdio.h>
#include<malloc.h>

typedef struct
{
	unsigned int cod;
	char* titlu;
	char* beneficiar;
	unsigned char* nr_executanti;
	float buget;
}proiect;

typedef struct
{
	proiect inf;
	struct nodLS* next;
}nodLS;

typedef struct
{
	nodLS** vect;//pointer la lista alocata dinamic
	int nrElem;
	
}hashT;

int functieDispersie(hashT tabela, char beneficiar[20])
{
	return beneficiar[0] % tabela.nrElem;
}

void inserareHashTable(hashT tabela, proiect p)
{
	if (tabela.vect != NULL)//daca tabela de dispersie nu este goala
	{
		int pozitie = functieDispersie(tabela, p.beneficiar);//se gaseste o pozitie
		nodLS* nou = (nodLS*)malloc(sizeof(nodLS));//implementare lista
		nou->inf.cod = p.cod;
		nou->inf.titlu = (char*)malloc((strlen(p.titlu) + 1) * sizeof(char));
		strcpy(nou->inf.titlu, p.titlu);
		nou->inf.beneficiar = (char*)malloc((strlen(p.beneficiar) + 1) * sizeof(char));
		strcpy(nou->inf.beneficiar, p.beneficiar);
		nou->inf.nr_executanti = (char*)malloc((strlen(p.nr_executanti) + 1) * sizeof(char));
		strcpy(nou->inf.nr_executanti, p.nr_executanti);
		nou->inf.buget = p.buget;
		nou->next = NULL;

		if (tabela.vect[pozitie] == NULL)
			tabela.vect[pozitie] = nou;
		else
		{
			nodLS* temp = tabela.vect[pozitie];
			while (temp->next != NULL)
				temp = temp->next;
			temp->next = nou;
		}

	}
}

void traversareLS(nodLS* capLS)
{
	nodLS* temp = capLS;
	while (temp != NULL)
	{
		printf("\nCod=%d, Beneficiar=%s, Buget=%5.2f",
			temp->inf.cod, temp->inf.beneficiar, temp->inf.buget);
		temp = temp->next;
	}
}

void traversareHashTable(hashT tabela)
{
	if (tabela.vect != NULL)
	{
		for (int i = 0; i < tabela.nrElem; i++)
		{
			if (tabela.vect[i] != NULL)
			{
				printf("\nPozitie: %d", i);
				traversareLS(tabela.vect[i]);
			}
		}
	}
}

void dezalocareLS(nodLS* capLS)
{
	nodLS* temp = capLS;
	while (temp != NULL)
	{
		nodLS* temp2 = temp->next;
		free(temp->inf.titlu);
		free(temp->inf.beneficiar);
		free(temp->inf.nr_executanti);
		free(temp);
		temp = temp2;
	}
}

void dezalocareHashTable(hashT tabela)
{
	if (tabela.vect != NULL)
	{
		for (int i = 0; i < tabela.nrElem; i++)
			if (tabela.vect[i] != NULL)
				dezalocareLS(tabela.vect[i]);
	}
	free(tabela.vect);
}

void dezalocareNumeBeneficiar(nodLS* capLS)
{
	nodLS* temp = capLS;
	while (temp != NULL)
	{
		nodLS* temp2 = temp->next;
		free(temp->inf.beneficiar);
		temp = temp2;
	}
}

nodLS* inserareNod(nodLS* cap, proiect p)
{
	nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
	nou->inf.cod = p.cod;
	nou->inf.titlu = (char*)malloc((strlen(p.titlu) + 1) * sizeof(char));
	strcpy(nou->inf.titlu, p.titlu);
	nou->inf.beneficiar = (char*)malloc((strlen(p.beneficiar) + 1) * sizeof(char));
	strcpy(nou->inf.beneficiar, p.beneficiar);
	nou->inf.nr_executanti = (char*)malloc((strlen(p.nr_executanti) + 1) * sizeof(char));
	strcpy(nou->inf.nr_executanti, p.nr_executanti);
	nou->inf.buget = p.buget;
	nou->next = NULL;
	if (cap == NULL)
		cap = nou;
	else
	{
		nodLS* temp = cap;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = nou;
	}
	return cap;
}

void traversare(nodLS* cap)
{
	nodLS* temp = cap;
	while (temp != NULL)
	{
		printf("Cod=%d, Titlu=%s, Beneficiar=%s, Nr_executanti=%s, Buget=%f",
			temp->inf.cod, temp->inf.titlu, temp->inf.beneficiar, temp->inf.nr_executanti,
			temp->inf.buget);
		temp = temp->next;
	}
}
float bugetInvesitii(hashT tabela,char nume[20])
{

	float s = 0;
	if (tabela.vect != NULL)
	{
		for (int i = 0; i < tabela.nrElem; i++)
		{
			if (tabela.vect[i] != NULL)
			{
				nodLS* temp = tabela.vect[i];
				while (temp != NULL)
				{
					if (strcmp(temp->inf.beneficiar, nume) == 0)
					{
						s = s + temp->inf.buget;
					}
					temp = temp->next;
				}

			}
		}
	}
	return s;
}

void schimbareNume(hashT tabela, char numeVechi[20], char numeNou[20])
{
	if (tabela.vect != NULL)
	{
		for (int i = 0; i < tabela.nrElem; i++)
		{
			if (tabela.vect[i] != NULL)
			{
				nodLS* temp = tabela.vect[i];
				if (strcmp(temp->inf.beneficiar, numeVechi) == 0)
				{
					strcpy(temp->inf.beneficiar, numeNou);
				}
				temp = temp->next;

			}
		}
	}
}

nodLS* conversieTabelaLista(hashT tabela, nodLS*cap, float bugetMinim)
{
	if (tabela.vect != NULL)
	{
		for (int i = 0; i < tabela.nrElem; i++)
		{
			if (tabela.vect[i] != NULL)
			{
				nodLS* temp = tabela.vect[i];
				while (temp != NULL)
				{
					if (temp->inf.buget < bugetMinim)
					{
						cap = inserareNod(cap, tabela.vect[i]->inf);
					}
					temp = temp->next;
				}
			}
		}
	}
	return cap;
}


void main()
{
	int nrPro;
	hashT tabela;
	tabela.nrElem = 31;
	tabela.vect = (nodLS**)malloc(tabela.nrElem * sizeof(nodLS*));
	for (int i = 0; i < tabela.nrElem; i++)
		tabela.vect[i] = NULL;

	char buffer1[20];
	char buffer2[20];
	char buffer3[20];
	proiect p;
	FILE* f = fopen("fisier.txt", "r");
	fscanf(f, "%d", &nrPro);
	for (int i = 0; i < nrPro; i++)
	{
		fscanf(f, "%d", &p.cod);
		fscanf(f, "%s", buffer1);
		p.titlu = (char*)malloc((strlen(buffer1) + 1) * sizeof(char));
		strcpy(p.titlu, buffer1);
		fscanf(f, "%s", buffer2);
		p.beneficiar = (char*)malloc((strlen(buffer2) + 1) * sizeof(char));
		strcpy(p.beneficiar, buffer2);
		fscanf(f, "%s", buffer3);
		p.nr_executanti= (char*)malloc((strlen(buffer3) + 1) * sizeof(char));
		strcpy(p.nr_executanti, buffer3);
		fscanf(f, "%f", &p.buget);
		inserareHashTable(tabela, p);
		free(p.titlu);
		free(p.beneficiar);
		free(p.nr_executanti);

	}

	fclose(f);
	traversareHashTable(tabela);
	printf("\n");
	char nume[20]="Sabin";
	float sumaInvestita = bugetInvesitii(tabela,nume);
	printf("\nSuma investita este de %f\n",sumaInvestita);

	char numeVechi[20] = "Sabin";
	char numeNou[20] = "Ion";
	schimbareNume(tabela, numeVechi, numeNou);
	printf("\n----------Schimbarea numelui---------");
	traversareHashTable(tabela);

	printf("\n-----------Buget Minim------------");
	float bugetMinim = 1501;
	nodLS* cap = NULL;
	cap= conversieTabelaLista(tabela,cap, bugetMinim);
	traversareLS(cap);
	
	dezalocareHashTable(tabela);
}