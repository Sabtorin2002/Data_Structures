#include <stdio.h>
#include<malloc.h>

typedef struct
{
	int prioritate;
	char* denumire;
	float cost;
}cheltuiala;

typedef struct
{
	cheltuiala* vect;
	int nrElem;
}heap;
//Filtrare element maxim
void filtrare(heap h, int index)
{
	int indexRad = index;//nod radacina
	int indexSt = 2 * index + 1;//fiul stanga
	int indexDr = 2 * index + 2;//fiul dreapta

	if (indexSt<h.nrElem && h.vect[indexSt].prioritate>h.vect[indexRad].prioritate)//indexRad
		indexRad = indexSt;//interschimbare indici
	if (indexDr<h.nrElem && h.vect[indexDr].prioritate >h.vect[indexRad].prioritate)
		indexRad = indexDr;

	if (index != indexRad)//interschimbare elemente din vector, daca s-a modificat radacina
	{
		cheltuiala temp = h.vect[index];
		h.vect[index] = h.vect[indexRad];
		h.vect[indexRad] = temp;

		filtrare(h, indexRad);//se reia filtrarea de la pozitia ramasa
	}
}

heap inserare(heap h, cheltuiala noua)
{
	cheltuiala* vectNou = (cheltuiala*)malloc((h.nrElem + 1) * sizeof(cheltuiala));//alocare spatiu(5+1 in exemplu)
	for (int i = 0; i < h.nrElem; i++)
		vectNou[i] = h.vect[i];//se itereaza un nou vector(o copie)
	h.nrElem++;//se mareste nr de elemente cu 1
	vectNou[h.nrElem - 1] = noua;//vectorul de pozitie 6-1=5 ia valoarea paramentrului de intrare noua
	free(h.vect);//se elibereaza memoria in vectorul initial(care contine prioritate, denumire, cost)
	h.vect = vectNou;//si apoi vectorul initial ia valoarea vectorului nou
	for (int i = (h.nrElem - 1) / 2; i >= 0; i--)
		filtrare(h, i);
	return h;//returnare strucutra heap modificata

}

heap extragere(heap h, cheltuiala* extrasa)//extragere element cu prioritate maxima
{
	cheltuiala* vectNou = (cheltuiala*)malloc((h.nrElem - 1) * sizeof(cheltuiala));//se aloca spatiu pentru un nou vector
	//de tip cheltuiala 5-1=4
	//radacina e prima, radacina are valoare maxima
	cheltuiala temp = h.vect[0];//valoarea auxiliara ia valoarea primului vector
	h.vect[0] = h.vect[h.nrElem - 1];//primul vector ia valoarea ultimului
	h.vect[h.nrElem - 1] = temp;//ultimul ia valoarea auxiliarului
	//se realizeaza un swap dintre primul si ultimul element din vectorul h.vect(nodul radacina si ult element din vector,
	//astefel elementul maxim este mutat la finalul vectorului)
	*extrasa = h.vect[h.nrElem - 1];//pointerul de tip  ch.extrasa ia valoarea ultimului element din vector
	//adica maximul 
	
	h.nrElem--;//se micsoreaza numarul de elemente cu 1

	for (int i = 0; i < h.nrElem; i++)//se itereaza vectorul h.vect 
		vectNou[i] = h.vect[i];//se copiaza elementele in noul vector vectNou
	free(h.vect);//se elibereaza spatiul de memorie ocupat anterior de vectorul initial
	h.vect = vectNou;//vectorul intial primeste adresa noului vector vectNou
	for (int i = (h.nrElem - 1) / 2; i >= 0; i--)
		filtrare(h, i);
	return h;//returnare strucutura heap modificata
}
//Filtrare element minim
void filtrareMin(heap h, int index)
{
	int indexRad = index;
	int indexSt = index * 2 + 1;
	int indexDr = index * 2 + 1;
	if (indexSt < h.nrElem && h.vect[indexSt].prioritate < h.vect[indexRad].prioritate)
		indexRad = indexSt;
	if (indexDr < h.nrElem && h.vect[indexDr].prioritate < h.vect[indexRad].prioritate)
		indexRad = indexDr;
	
	if (indexRad != index)
	{
		cheltuiala temp = h.vect[index];
		h.vect[index] = h.vect[indexRad];
		h.vect[indexRad] = temp;

		filtrareMin(h, indexRad);
	}
}
heap extragereMin(heap h1, cheltuiala* extrasaMin )
{
	cheltuiala* vectNou = (cheltuiala*)malloc((h1.nrElem - 1) * sizeof(cheltuiala));
	cheltuiala temp = h1.vect[0];
	h1.vect[0] = h1.vect[h1.nrElem - 1];
	h1.vect[h1.nrElem - 1] = temp;

	*extrasaMin = h1.vect[h1.nrElem - 1];
	h1.nrElem--;

	for (int i = 0; i < h1.nrElem; i++)
		vectNou[i] = h1.vect[i];
	free(h1.vect);
	h1.vect = vectNou;
	for (int i = (h1.nrElem - 1) / 2; i >= 0; i--)
		filtrareMin(h1, i);
	return h1;

}

void afisare(heap h)
{
	for (int i = 0; i < h.nrElem; i++)
		printf("\nPrioritate:%d, Denumire:%s, Cost:%5.2f",
			h.vect[i].prioritate, h.vect[i].denumire, h.vect[i].cost);
}

void dezalocare(heap h)
{
	for (int i = 0; i < h.nrElem; i++)
		free(h.vect[i].denumire);
	free(h.vect);
}

void main()
{
	heap h;
	char buffer[20];
	FILE* f = fopen("fisier.txt", "r");
	fscanf(f, "%d", &h.nrElem);
	h.vect = (cheltuiala*)malloc((h.nrElem + 1) * sizeof(cheltuiala));
	for (int i = 0; i < h.nrElem; i++)
	{
		fscanf(f, "%d", &h.vect[i].prioritate);
		fscanf(f, "%s", buffer);
		h.vect[i].denumire = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(h.vect[i].denumire, buffer);
		fscanf(f, "%f", &h.vect[i].cost);
	}
	fclose(f);
	
	for (int i = (h.nrElem - 1) / 2; i >= 0; i--)
		filtrare(h, i);
	afisare(h);

	printf("\n--------------------------\n");

	cheltuiala noua;
	printf("\nPrioritate: ");
	scanf("%d", &noua.prioritate);
	printf("\nDenumire: ", &noua.denumire);
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
	printf("\nExtras -> Prioritate: %d, Denumire: %s, Cost:%5.2f",
		extrasa.prioritate, extrasa.denumire, extrasa.cost);
	

	printf("\n--------------------------\n");

	cheltuiala* vectSortat = (cheltuiala*)malloc(h.nrElem * sizeof(cheltuiala));
	int nr = h.nrElem;
	for (int i = 0; i < nr; i++)
	{
		h = extragere(h, &extrasa);//extragerea pe rand a elementului cu valoarea maxima din vector
		vectSortat[i] = extrasa;//adaugarea elementului in vector
	}

	for (int i = 0; i < nr; i++)
		printf("\nPrioritate: %d, Denumire: %s, Cost:%5.2f",
			vectSortat[i].prioritate, vectSortat[i].denumire, vectSortat[i].cost);

	//for (int i = 0; i < nr; i++)
		//free(vectSortat[i].denumire);
	//free(vectSortat);

	
	/*
	for (int i = (h.nrElem - 1) / 2; i >= 0; i--)
		filtrareMin(h, i);
	afisare(h);
	printf("\n");

	cheltuiala noua;
	printf("\nPrioritate: ");
	scanf("%d", &noua.prioritate);
	printf("\nDenumire: ");
	scanf("%s",buffer);
	noua.denumire = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
	strcpy(noua.denumire, buffer);
	printf("\nCost: ");
	scanf("%f",&noua.cost);
	
	h = inserare(h, noua);
	afisare(h);
	printf("\n--------------------------\n");

	cheltuiala extrasa;
	h = extragereMin(h, &extrasa);
	afisare(h);

	printf("\nExtras -> Prioritate: %d, Denumire: %s, Cost: %5.2f",
		extrasa.prioritate,
		extrasa.denumire, extrasa.cost);

	printf("\n--------------------------\n");

	cheltuiala* vectSortat = (cheltuiala*)malloc(h.nrElem *
		sizeof(cheltuiala));
	int nr = h.nrElem;
	for (int i = 0; i < nr; i++)
	{
		h = extragere(h, &extrasa);
		vectSortat[i] = extrasa;
	}

	for (int i = 0; i < nr; i++)
		printf("\nPrioritate: %d, Denumire: %s, Cost: %5.2f",
			vectSortat[i].prioritate,
			vectSortat[i].denumire, vectSortat[i].cost);

	for (int i = 0; i < nr; i++)
		free(vectSortat[i].denumire);
	free(vectSortat);
	*/
}