#include<stdio.h>
#include<malloc.h>
//cerinte1: creare lista dubla, inserare la interior?, dupa un criteriu
//cerinta2:afisare din ambele sensuri
//cerinta3:nr de comenzi care se livreaza intr-o zi(data de livrare)
//cerinta4:suma media a comenzilor unui client dat ca paramentu
//inserarea in vector din lista a elementelor pereche numeClient, sumaComanda
typedef struct
{
	int idComanda;
	char* dataPlasare;
	char* dataLivrare;
	char* numeClient;
	float sumaComanda;
}comanda;

typedef struct
{
	comanda inf;
	struct nodLS* next, * prev;
}nodLS;

nodLS* inserareNod(nodLS* cap, nodLS** ultim, comanda c)
{
	nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
	nou->inf.idComanda = c.idComanda;
	nou->inf.dataPlasare = (char*)malloc((strlen(c.dataPlasare) + 1) * sizeof(char));
	strcpy(nou->inf.dataPlasare, c.dataPlasare);
	nou->inf.dataLivrare = (char*)malloc((strlen(c.dataLivrare) + 1) * sizeof(char));
	strcpy(nou->inf.dataLivrare, c.dataLivrare);
	nou->inf.numeClient = (char*)malloc((strlen(c.numeClient) + 1) * sizeof(char));
	strcpy(nou->inf.numeClient, c.numeClient);
	nou->inf.sumaComanda = c.sumaComanda;
	nou->next = NULL;
	nou->prev = NULL;
	if (cap == NULL)
		cap = nou;
	else
	{
		nodLS* temp = cap;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = nou;
		nou->prev = temp;
		*ultim = nou;
	}

	return cap;
}

void traversare(nodLS* cap)
{
	nodLS* temp = cap;
	while (temp != NULL)
	{
		printf("\nID comanda=%d, Data plasare=%s, Data livrare=%s, Nume client=%s, Suma=%5.2f",
			temp->inf.idComanda, temp->inf.dataPlasare, temp->inf.dataLivrare,
			temp->inf.numeClient, temp->inf.sumaComanda);
		temp = temp->next;
	}
}

int nrComenzi(nodLS* cap, char zi[20])
{	
	int c=0;
	nodLS* temp = cap;
	while (temp != NULL)
	{
		if (strcmp(temp->inf.dataLivrare, zi) == 0)
		{
			c++;
		}
		temp = temp->next;
	}
	return c;
}
void traversareInvers(nodLS* ultim)
{
	nodLS* temp = ultim;
	while (temp != NULL)
	{
		printf("\nID comanda=%d, Data plasare=%s, Data livrare=%s, Nume client=%s, Suma=%5.2f",
			temp->inf.idComanda, temp->inf.dataPlasare, temp->inf.dataLivrare,
			temp->inf.numeClient, temp->inf.sumaComanda);
		temp = temp->prev;
	}
}

void dezalocare(nodLS* cap)
{
	nodLS* temp = cap;
	while (temp != NULL)
	{
		nodLS* temp2 = temp->next;
		free(temp->inf.dataPlasare);
		free(temp->inf.dataLivrare);
		free(temp->inf.numeClient);
		free(temp);
		temp = temp2;
	}
}

float sumaMedie(nodLS* cap, char nume[20], int*nr)
{
	float s = 0;
	nodLS* temp = cap;
	while (temp != NULL)
	{
		if (strcmp(temp->inf.numeClient, nume) == 0)
		{
			s = s + temp->inf.sumaComanda;
			(*nr)++;
		}
		temp = temp->next;
	}
	s = s / *nr;
	return s;
}

void salvareVector(nodLS* cap, comanda* vect, int* nr)
{
	nodLS* temp = cap;
	while (temp != NULL)
	{
			vect[*nr].numeClient = (char*)malloc((strlen(temp->inf.numeClient) + 1) * sizeof(char));
			strcpy(vect[*nr].numeClient, temp->inf.numeClient);
			vect[*nr].sumaComanda = temp->inf.sumaComanda;
			(*nr)++;
			temp = temp->next;
	}
}

void main()
{
	int n;
	comanda c;
	FILE* f = fopen("fisier.txt", "r");
	fscanf(f, "%d", &n);
	nodLS* cap = NULL, * ultim = NULL;
	char buffer1[20];
	char buffer2[20];
	char buffer3[20];
	for (int i = 0; i < n; i++)
	{
		fscanf(f, "%d", &c.idComanda);
		fscanf(f, "%s", buffer1);
		c.dataPlasare = (char*)malloc((strlen(buffer1) + 1) * sizeof(char));
		strcpy(c.dataPlasare, buffer1);
		fscanf(f, "%s", buffer2);
		c.dataLivrare = (char*)malloc((strlen(buffer2) + 1) * sizeof(char));
		strcpy(c.dataLivrare, buffer2);
		fscanf(f, "%s", buffer3);
		c.numeClient = (char*)malloc((strlen(buffer3) + 1) * sizeof(char));
		strcpy(c.numeClient, buffer3);
		fscanf(f, "%f", &c.sumaComanda);

		cap = inserareNod(cap, &ultim, c);
		free(c.dataPlasare);
		free(c.dataLivrare);
		free(c.numeClient);
	}

	fclose(f);
	printf("\n------------Traversare----------------\n");
	traversare(cap);
	printf("\n");
	printf("\n-----------Traversare Inversa---------\n");
	traversareInvers(ultim);
	

	int numarComenzi = 0;
	numarComenzi = nrComenzi(cap, "15Martie");
	if (numarComenzi > 0)
		printf("\nNumarul de comenzi este: %d", numarComenzi);
	else
		printf("\nNu a fost nicio comanda in ziua respectiva");
	
	float totalComanda = 0;
	int nr = 0;
	char nume[20] = "Sabin";	
	totalComanda =sumaMedie(cap, nume, &nr);
	printf("\nSuma medie a clientului %s este %5.2f",nume,totalComanda);

	printf("\n---------SalvareVector----------\n");
	comanda* vect = (comanda*)malloc(n * sizeof(comanda));
	int numarV = 0;
	salvareVector(cap, vect, &numarV);
	for (int i = 0; i < numarV; i++)
	{
		printf("\nNume_Client=%s, Suma_Comanda=%5.2f ",
			vect[i].numeClient, vect[i].sumaComanda);
	}

	for (int i = 0; i < numarV; i++)
		free(vect[i].numeClient);
	free(vect);
	
	dezalocare(cap);
}