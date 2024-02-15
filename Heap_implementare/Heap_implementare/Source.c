#include <stdio.h>
#include <malloc.h>

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

void filtrare(heap h, int index)
{
	int indexRad = index;
	int indexSt = 2 * index + 1;
	int indexDr = 2 * index + 2;
	if (indexSt<h.nrElem && h.vect[indexSt].prioritate>h.vect[indexRad].prioritate)
		indexRad = indexSt;
	if (indexDr<h.nrElem && h.vect[indexDr].prioritate>h.vect[indexRad].prioritate)
		indexRad = indexDr;
	if (indexRad != index)
	{
		cheltuiala temp =h.vect[index];
		h.vect[index] = h.vect[indexRad];
		h.vect[indexRad] = temp;

		filtrare(h, indexRad);
	}
}

heap inserare(heap h, cheltuiala noua)
{
	cheltuiala* vectNou = (cheltuiala*)malloc((h.nrElem + 1) * sizeof(cheltuiala);
	for (int i = 0; i < h.nrElem; i++)
	{
		vectNou[i] = h.vect[i];
	}

	h.nrElem++;
	h.vect[h.nrElem - 1] = noua;

	free(h.vect);
	h.vect = vectNou;
	for (int i = (h.nrElem - 1) / 2; i >= 0; i--)
	{
		filtrare(h, i);
	}	

	return h;
	
}

heap extragere(heap h, cheltuiala* extrasa)//extragere element maxim pe care l mutam la final
{
	cheltuiala* vectNou = (cheltuiala*)malloc((h.nrElem - 1) * sizeof(cheltuiala));
	cheltuiala temp = h.vect[0];
	h.vect[0] = h.vect[h.nrElem - 1];
	h.vect[h.nrElem - 1] = temp;

	*extrasa = h.vect[h.nrElem - 1];
	h.nrElem--;

	for (int i = 0; i < h.nrElem; i++)
		vectNou[i] = h.vect[i];

	free(h.vect);
	h.vect = vectNou;

	for (int i = (h.nrElem - 1) / 2; i >= 0; i--)
		filtrare(h, i);

	return h;


}

void afisareHeap(heap h)
{
	for (int i = 0; i < h.nrElem; i++)
	{
		printf("\nPrioritate: %d, Denumire: %s, Cost: %5.2f",
			h.vect[i].prioritate,
			h.vect[i].denumire, h.vect[i].cost);
	}
}