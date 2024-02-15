#include<stdio.h>
#include<malloc.h>

typedef struct
{
	int priortitate;
	char* denumire;
	float cost;
}cheltuiala;

typedef struct
{
	cheltuiala* vect;
	int nrElem;
} heap;

void filtrare(heap h, int index)
{
	int indexRad = index;//nod radacina
	int indexSt = 2 * index + 1;//nod stanga
	int indexDr = 2 * index + 2;//index dreapta

	if (indexSt<h.nrELem && h.vect[indexSt].priortitate>h.vect[indexRad].priortitate)
		indexRad = indexSt;
	if (indexDr<h.nrELem && h.vect[indexDr].priortitate>h.vect[indexRad].priortitate)
		indexRad = indexDr;

	if (index != indexRad)//daca s-a modificat radacina
	{
		//interschimbare elemente din vector
		cheltuiala temp = h.vect[index];
		h.vect[index] = h.vect[indexRad];
		h.vect[indexRad] = temp;

		filtrare(h, indexRad);//se reia filtrarea de la pozitia ramasa
	}
}

heap inserare(heap h, cheltuiala noua)
{
	cheltuiala* vectNou = (cheltuiala*)malloc((h.nrELem + 1) * sizeof(cheltuiala));
	for (int i = 0; i < h.nrELem; i++)//se itereaza 
		vectNou[i] = h.vect[i];//se copiaza un vector, se creeaza o copie
	
	h.nrELem++;//se mareste nr de elemnte cu 1
	vectNou[h.nrELem - 1] = noua;//vectorul de pozitie 6-1=5 ia valoarea parametrului de intrare noua
	free(h.vect);//se elibereaza memoria din vectorul initial

	h.vect = vectNou;
	for (int i = (h.nrELem - 1) / 2; i >= 0; i--)
		filtrare(h, i);
	
	return h;
}

heap extragere(heap h, cheltuiala* extrasa)//extragere radacina, element cu prioritate maxima
{
	cheltuiala* vectNou = (cheltuiala*)malloc((h.nrELem - 1) * sizeof(cheltuiala));
	//se aloca spatiu pentru un nou vector(5-1 pentru ca se extrage si se va sterge un element)
	//radacina e prima, radacina are valoarea maxima
	cheltuiala temp = h.vect[0];
	h.vect[0] = h.vect[h.nrELem - 1];
	h.vect[h.nrELem - 1] = temp;
	//se realizeaza un swap dintre primul si ultimul element din vectorul h.vect(nodul 
	//radacina si ult element din vector, astfel elementul maxim este mutat la finalul vectorului
	*extrasa = h.vect[h.nrELem - 1];//pointerul de tip cheltuiala extrasa ia valoarea ultimului
	//element din vector adica maximul, s-a salvat nodul extras

	h.nrELem--;
	for (int i = 0; i < h.nrELem; i++)
		vectNou[i] = h.vect[i];//se copiaza elementele in noul vector
	free(h.vect);//se elibereaza spatiul de memorie ocupat anterior de vectorul initial
	h.vect = vectNou;//vectorul initial primeste adresa noului vector vectNou
	for (int i = (h.nrELem - 1) / 2; i >= 0; i--)
		filtrare(h, i);

	return h;//returnare structura heap modificata

}

void afisare(heap h)
{
	for (int i = 0; i < h.nrELem; i++)
		printf("\nPrioritate=%d, Denumire=%s, Cost=%5.2f",
			h.vect[i].priortitate, h.vect[i].denumire, h.vect[i].cost);
}

void dezalocare(heap h)
{
	for (int i = 0; i < h.nrELem; i++)
		free(h.vect[i].denumire);
	free(h.vect);
}

void main()
{/*
	heap h;
	char buffer[20];
	FILE* f = fopen("fisier.txt", "r");
	fscanf(f, "%d", &h.nrELem);
	h.vect = (cheltuiala*)malloc((h.nrELem + 1) * sizeof(char));
	for (int i = 0; i < h.nrELem; i++)
	{
		fscanf(f, "%d", &h.vect[i].priortitate);
		fscanf(f, "%s", buffer);
		h.vect[i].denumire = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(h.vect[i].denumire, buffer);
		fscanf(f, "%f", &h.vect[i].cost);
	}

	fclose(f);
	for (int i = (h.nrELem - 1) / 2; i >= 0; i--)
		filtrare(h, i);
	afisare(h);

	printf("\n--------------------------\n");

	cheltuiala noua;
	printf("\nPrioritate: ");
	scanf("%d", &noua.priortitate);
	printf("\nDenumire: ");
	scanf("%s", buffer);
	noua.denumire = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
	strcpy(noua.denumire, buffer);
	printf("\nCost: ");
	scanf("%f", &noua.cost);

	h = inserare(h, noua);
	afisare(h);

	printf("\n--------------------------\n");

	cheltuiala extrasa;
	h = extragere(h, &extrasa);
	afisare(h);
	printf("\nExtras-> Prioritate:%d, Denumire:%s, Cost:%5.2f",
		extrasa.priortitate, extrasa.denumire, extrasa.cost);

	printf("\n--------------------------\n");

	cheltuiala* vectSortat = (cheltuiala*)malloc(h.nrELem * sizeof(cheltuiala));
	int nr = h.nrELem;
	for (int i = 0; i < nr; i++)
	{
		h = extragere(h, &extrasa);
		vectSortat[i] = extrasa;
	}

	for (int i = 0; i < nr; i++)
		printf("\nPrioritate:%d, Denumire:%s, Cost:%5.2f",
			vectSortat[i].priortitate, vectSortat[i].denumire, vectSortat[i].cost);
			*/
}