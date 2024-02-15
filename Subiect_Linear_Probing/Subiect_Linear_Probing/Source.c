#include<stdio.h>
#include<malloc.h>

typedef struct
{
	unsigned int id;
	char* denumireHotel;
	unsigned char numarCamere;
	unsigned char camereRezervate;
	char* numeClient;
	char* perioada;
	float pretCamera;
}rezervare;

typedef struct
{
	rezervare** vect;
	int nrElem;
}hashT;

int functieDispersie(hashT tabela, char nume[20])
{
	return nume[0] % tabela.nrElem;
}

void inserareHashTable(hashT tabela, rezervare* r)
{
	if (tabela.vect != NULL)
	{
		int pozitie = functieDispersie(tabela, (*r).denumireHotel);
		if (tabela.vect[pozitie] == NULL)
		{
			tabela.vect[pozitie] = r;
		}
		else
		{
			int index = 1;
			while (pozitie + index < tabela.nrElem)
			{
				if (tabela.vect[pozitie+index] == NULL)
				{
					pozitie = pozitie + index;
					tabela.vect[pozitie] = r;
					break;
				}
				index++;
			}
		}
	}
}
void traversareTabela(hashT tabela)
{
	if (tabela.vect != NULL)
	{
		for(int i=0;i<tabela.nrElem;i++)
			if (tabela.vect[i] != NULL)
			{
				printf("\nPozitie:%d ", i);
				printf("\nID:%d, Denumire hotel:%s, Numar camere:%u, Camere Rezervate:%u, Nume client:%s, Perioada=%s, Pret=%5.2f",
					tabela.vect[i]->id, tabela.vect[i]->denumireHotel,tabela.vect[i]->numarCamere,
					tabela.vect[i]->camereRezervate,
					tabela.vect[i]->numeClient, tabela.vect[i]->perioada, tabela.vect[i]->pretCamera);
			}
	}
}

void dezalocareTabela(hashT tabela)
{
	if (tabela.vect != NULL)
	{
		for (int i = 0; i < tabela.nrElem; i++)
			if (tabela.vect[i] != NULL)
			{
				//free(tabela.vect[i]->denumireHotel);
				//free(tabela.vect[i]->numeClient);
				//free(tabela.vect[i]->perioada);
				free(tabela.vect[i]);
			}
		free(tabela.vect);
	}
}

float gradOcupare(hashT tabela, char numeHotel[20], char perioadaTimp[20])
{
	float grad = 0;
	if (tabela.vect != NULL)
	{
		for(int i=0;i<tabela.nrElem;i++)
			if (tabela.vect[i] != NULL)
			{
				if (strcmp(tabela.vect[i]->perioada, perioadaTimp) == 0)
				{
					if(strcmp(tabela.vect[i]->denumireHotel, numeHotel)==0)
					{
						float numar = ((float)tabela.vect[i]->camereRezervate / (float)tabela.vect[i]->numarCamere);
						grad = grad + numar * 100;
					}
				}
			}
	}

	return grad;
}

rezervare* salvareVector(hashT tabela, int* nr, char hotel[30], char client[30])
{
	rezervare* vect = (rezervare*)malloc(tabela.nrElem * sizeof(rezervare));
	if (tabela.vect != NULL)
	{
		for (int i = 0; i < tabela.nrElem; i++)
		{
			if (tabela.vect[i] != NULL)
			{
				if (strcmp(tabela.vect[i]->denumireHotel, hotel) == 0)
				{
					if (strcmp(tabela.vect[i]->numeClient, client) == 0)
					{
						//vect[*nr] = *(tabela.vect[i]);
						//(*nr)++;
						vect[(*nr)].id = tabela.vect[i]->id;
						vect[(*nr)].pretCamera = tabela.vect[i]->pretCamera;
						(*nr)++;
					}
				}
			}
		}
	}
	return vect;
}

float incasari(hashT tabela, rezervare** vect, char perioada[30])
{
	float s = 0;
	if (tabela.vect != NULL)
	{
		for (int i = 0; i < tabela.nrElem; i++)
		{
			if (tabela.vect[i] != NULL)
			{
				if (strcmp(tabela.vect[i]->perioada, perioada) == 0)
				{
					s = s + tabela.vect[i]->pretCamera;
				}
			}
		}
	}
	return s;
}

int numarCamere(hashT tabela, char numeClient[20])
{
	int suma = 0;
	if (tabela.vect != NULL)
	{
		for (int i = 0; i < tabela.nrElem; i++)
		{
			if (tabela.vect[i] != NULL)
			{
				if (strcmp(tabela.vect[i]->numeClient, numeClient) == 0)
				{
					suma = suma + tabela.vect[i]->camereRezervate;
				}
			}
		}
	}
	return suma;
}
void main()
{
	int nrRez;
	hashT tabela;
	tabela.nrElem = 250;
	tabela.vect = (rezervare**)malloc(tabela.nrElem * sizeof(rezervare*));
	for (int i = 0; i < tabela.nrElem; i++)
		tabela.vect[i] = NULL;

	char buffer1[20];
	char buffer2[20];
	char buffer3[20];
	char buffer4[20];
	char buffer5[20];

	rezervare* r;
	FILE* f = fopen("fisier.txt", "r");
	fscanf(f, "%d", &nrRez);
	for (int i = 0; i < nrRez; i++)
	{
		r = (rezervare*)malloc(sizeof(rezervare));
		fscanf(f, "%d", &r->id);
		fscanf(f, "%s", buffer1);
		r->denumireHotel = (char*)malloc((strlen(buffer1) + 1) * sizeof(char));
		strcpy(r->denumireHotel, buffer1);
		fscanf(f, "%u", &r->numarCamere);
		fscanf(f, "%u", &r->camereRezervate);
		fscanf(f, "%s", buffer4);
		r->numeClient= (char*)malloc((strlen(buffer4) + 1) * sizeof(char));
		strcpy(r->numeClient, buffer4);
		fscanf(f, "%s", buffer5);
		r->perioada= (char*)malloc((strlen(buffer5) + 1) * sizeof(char));
		strcpy(r->perioada, buffer5);
		fscanf(f, "%f", &r->pretCamera);

		inserareHashTable(tabela, r);
	}

	fclose(f);
	traversareTabela(tabela);

	float gradOcupareEx3 = gradOcupare(tabela, "Kayus", "3nopti");
	printf("\nGradul de ocupare al hotelului este:%5.2f", gradOcupareEx3);
	

	printf("\n---------SalvareVector--------");
	int nr = 0;
	rezervare* vect = (rezervare*)malloc(tabela.nrElem * sizeof(rezervare));
	vect = salvareVector(tabela, &nr, "Vega", "Dorian");
	for (int i = 0; i < nr; i++)
	{
		printf("\nID:%d,Suma:%5.2f", vect[i].id, vect[i].pretCamera);
	}

	printf("\n---------INCASARI---------");
	float suma = 0;
	suma = incasari(tabela, &vect, "4nopti");
	printf("\nSuma incasata este %f", suma);


	printf("\n--------Camere_Rezervate------");
	int camere = 0;
	camere = numarCamere(tabela, "Mircea");
	printf("\nNumarul de camere ocupate este %d", camere);
	for (int i = 0; i < nr; i++)
	{
		free(vect[i].denumireHotel);
		free(vect[i].numeClient);
		free(vect[i].perioada);
	}
	free(vect);
	dezalocareTabela(tabela);
}