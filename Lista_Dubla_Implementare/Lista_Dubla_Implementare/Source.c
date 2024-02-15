#include<stdio.h>
#include<malloc.h>
typedef struct
{
	int id;
	char* denumire;
	float valoare;

}produs;

typedef struct
{
	produs inf;
	struct nodLS* next, * prev;
}nodLS;

nodLS* inserareNod(nodLS* cap, nodLS** ultim, produs p)
{
	nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
	nou->inf.id = p.id;
	nou->inf.denumire = (char*)malloc((strlen(p.denumire) + 1) * sizeof(char));
	strcpy(nou->inf.denumire, p.denumire);
	nou->inf.valoare = p.valoare;
	nou->next = NULL;
	nou->prev = NULL;
	if (cap == NULL)
	{
		cap = nou;
		*ultim = nou;
	}
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
		printf("\nID=%d, Denumire=%s, Valoare=%5.2f", temp->inf.id,
			temp->inf.denumire, temp->inf.valoare);
		temp = temp->next;
	}
}

void traversareUltim(nodLS* ultim)
{
	nodLS* temp = ultim;
	while (temp != NULL)
	{
		printf("\nID=%d, Denumire=%s, Valoare=%5.2f",
			temp->inf.id,temp->inf.denumire, temp->inf.valoare);
		temp = temp->prev;
	}
}

void dezalocare(nodLS* cap)
{
	nodLS* temp = cap;
	while (temp != NULL)
	{
		nodLS* temp2 = temp->next;
		free(temp->inf.denumire);
		free(temp);
		temp = temp2;
	}
}

void main()
{
	int nrProd;
	produs p;
	nodLS* cap = NULL;
	nodLS* ultim = NULL;
	char buffer[20];
	FILE* f = fopen("fisier.txt", "r");
	fscanf(f, "%d", &nrProd);
	for (int i = 0; i < nrProd; i++)
	{
		fscanf(f, "%d", &p.id);
		fscanf(f, "%s", buffer);
		p.denumire = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(p.denumire, buffer);
		fscanf(f, "%f", &p.valoare);

		cap = inserareNod(cap,&ultim,p);
		free(p.denumire);
	}

	fclose(f);
	traversare(cap);
	printf("\n------------------\n");
	traversareUltim(ultim);
	
	
dezalocare(cap);
}