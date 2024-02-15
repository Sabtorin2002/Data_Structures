#include<stdio.h>
#include<malloc.h>

typedef struct
{
	int cod;
	char* denumire;
	float pret;
}produs;

typedef struct
{
	int BF;
	produs inf;
	struct nodArb* left, * right;
}nodArb;

nodArb* creareNod(produs p, nodArb* st, nodArb* dr)
{
	nodArb* nou = (nodArb*)malloc(sizeof(nodArb));
	nou->inf.cod = p.cod;
	nou->inf.denumire = (char*)malloc((strlen(p.denumire) + 1) * sizeof(char));
	strcpy(nou->inf.denumire, p.denumire);
	nou->inf.pret = p.pret;
	nou->left = st;
	nou->right = dr;
	return nou;
}

nodArb* inserareNod(nodArb* rad, produs p)
{
	if (rad != NULL)
	{
		if (p.cod < rad->inf.cod)
		{
			rad->left = inserareNod(rad->left, p);
			return rad;
		}
		else
			if (p.cod > rad->inf.cod)
			{
				rad->right = inserareNod(rad->right, p);
				return rad;
			}
			else
				return rad;
	}
	else
		return creareNod(p, NULL, NULL);
}

void preordine(nodArb* rad)
{
	if (rad!= NULL)
	{
		printf("\nCod=%d, Denumire=%s, Pret=%5.2f, BF=%d",
			rad->inf.cod, rad->inf.denumire, rad->inf.pret,
			rad->BF);
		preordine(rad->left);
		preordine(rad->right);
	}
}

void dezalocareArbore(nodArb* rad)
{
	if (rad != NULL)
	{
		free(rad->inf.denumire);
		dezalocareArbore(rad->left);
		dezalocareArbore(rad->right);
		free(rad);
	}
}

int maxim(int a, int b)
{
	int max = a;
	if (max < b)
		max = b;
	return max;
}

int inaltimeArbore(nodArb* rad)
{
	if (rad != NULL)
		return 1 + maxim(inaltimeArbore(rad->left), inaltimeArbore(rad->right));
	else
		return 0;
}

void calculBF(nodArb* rad)
{
	if (rad != NULL)
	{
		rad->BF = inaltimeArbore(rad->right) - inaltimeArbore(rad->left);
		calculBF(rad->left);
		calculBF(rad->right);
	}
}

nodArb* rotatieStanga(nodArb* rad)
{
	printf("Rotatie stanga");
	nodArb* nod1 = rad->right;
	rad->right= nod1->left;
	nod1->left = rad;
	rad = nod1;
	return rad;
}

nodArb* rotatieDreapta(nodArb* rad)
{
	printf("\nRotatie dreapta");
	nodArb* nod1 = rad->left;
	rad->left = nod1->right;
	nod1->right = rad;
	rad = nod1;
	return rad;
}

nodArb* rotatieDreaptaStanga(nodArb* rad)
{
	printf("\n Rotatie dreapta stanga");
	nodArb* nod1 = rad->right;
	nodArb* nod2 = nod1->left;
	nod1->left = nod2->right;
	nod2->right = nod1;
	rad->right = nod2->left;
	nod2->left = rad;
	rad = nod2;
	return rad;

}

nodArb* rotatieStangaDreapta(nodArb* rad)
{
	printf("\nRotatie stanga dreapta");
	nodArb* nod1 = rad->left;
	nodArb* nod2 = nod1->right;
	nod1->right = nod2->left;
	nod2->left = nod1;
	rad->left = nod2->right;
	nod2->right = rad;
	rad = nod2;
	return rad;
}

nodArb* reechilibrare(nodArb* rad)
{
	calculBF(rad);
	nodArb* fiuSt = rad->left;
	nodArb* fiuDr = rad->right;
	if (rad->BF == -2 && fiuSt->BF == -1)
	{
		rad = rotatieDreapta(rad);
		calculBF(rad);
	}
	else
		if (rad->BF == 2 && fiuDr->BF == 1)
		{
			rad = rotatieStanga(rad);
			calculBF(rad);
		}
		else
			if (rad->BF == -2 && fiuSt->BF == 1)
			{
				rad = rotatieStangaDreapta(rad);
				calculBF(rad);
			}
			else
				if (rad->BF == 2 && fiuDr == -1)
				{
					rad = rotatieDreaptaStanga(rad);
					calculBF(rad);
				}
	return rad;
}
void numarareNoduriFrunza(nodArb* rad, int* nr)
{
	if (rad != NULL)
	{
		if (rad->left == NULL && rad->right == NULL)
		{
			(*nr)++;
		}
		numarareNoduriFrunza(rad->left, nr);
		numarareNoduriFrunza(rad->right, nr);
	}
}

void salvareVector(nodArb* rad, int* nr, produs** vect, float prag)
{
	if (rad != NULL)
	{
		if (rad->inf.pret >=prag)
		{
			(*vect)[(*nr)] = rad->inf;
			(*nr)++;
		}
		salvareVector(rad->left, nr, vect, prag);
		salvareVector(rad->right, nr, vect, prag);
	}
}
nodArb* stergeRad(nodArb* rad)
{
	if (rad != NULL)
	{
		nodArb* aux = rad;
		if (aux->left != NULL)
		{
			rad = aux->left;
			if (aux->right != NULL)
			{
				nodArb* max = aux->left;
				while (max->right != NULL)
					max = max->right;
				max->right = aux->right;
			}
		}
		else
			if (aux->right != NULL)
			{
				rad = aux->right;
			}
			else
				rad = NULL;
		free(aux->inf.denumire);
		free(aux);
		return rad;
	}
}
void main()
{
	int nrProd;
	produs p;
	char buffer[20];
	FILE* f = fopen("fisier.txt", "r");
	fscanf(f, "%d", &nrProd);
	nodArb* rad = NULL;
	for (int i = 0; i < nrProd; i++)
	{
		fscanf(f, "%d", &p.cod);
		fscanf(f, "%s", buffer);
		p.denumire = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(p.denumire, buffer);
		fscanf(f, "%f", &p.pret);

		rad = inserareNod(rad, p);
		free(p.denumire);

		rad = reechilibrare(rad);
	}
	printf("\nPreordine");
	preordine(rad);

	int numarFrunze = 0;
	numarareNoduriFrunza(rad, &numarFrunze);
	printf("\nNumarul de frunze este %d", numarFrunze);

	printf("\nSalvare vector");
	int nr = 0;
	produs* vect = (produs*)malloc(nrProd * sizeof(produs));
	salvareVector(rad, &nr, &vect, 7);
	for (int i = 0; i < nr; i++)
	{
		printf("\n Cod=%d, Denumire=%s, Pret=%5.2f",
			vect[i].cod, vect[i].denumire, vect[i].pret);
	}
	free(vect);

	printf("\nStergere radacina");
	rad = stergeRad(rad);
	preordine(rad);
	dezalocareArbore(rad);
}