#include <stdio.h>
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
	struct nodLS* next;
}nodLS;

typedef struct
{
	struct nodLS* inf;
	struct nodLP* next;
}nodLP;

nodLS* inserareNod(nodLS* capLS, produs p)
{
	nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
	nou->inf.id = p.id;
	nou->inf.denumire = (char*)malloc((strlen(p.denumire) + 1) * sizeof(char));
	strcpy(nou->inf.denumire, p.denumire);
	nou->inf.valoare = p.valoare;
	nou->next = NULL;
	if (capLS == NULL)
		capLS = nou;
	else
	{
		nodLS* temp = capLS;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = nou;
	}

	return capLS;
}

nodLP* inserareNodLP(nodLP* capLP, nodLS* capLS)
{
	nodLP* nou = (nodLP*)malloc(sizeof(nodLP));
	nou->inf = capLS;
	nou->next = NULL;
	if (capLP == NULL)
	{
		capLP = nou;
	}
	else
	{
		nodLP* temp = capLP;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = nou;
	}
	return capLP;
}


void traversareLS(nodLS* capLS)
{
	nodLS* temp = capLS;
	while (temp != NULL)
	{
		printf("\nCod:%d, Denumire:%s, Valoare:%5.2f",
			temp->inf.id, temp->inf.denumire, temp->inf.valoare);
		temp = temp->next;
	}
}

void traversareLP(nodLP* capLP)
{
	nodLP* temp = capLP;
	int i = 1;
	while (temp != NULL)
	{
		printf("\nPozitie:%d", i);
		traversareLS(temp->inf);
		temp = temp->next;
		i++;
	}
}

void dezalocareLS(nodLS* capLS)
{
	nodLS* temp = capLS;
	while (temp != NULL)
	{
		nodLS* temp2 = temp->next;
		free(temp->inf.denumire);
		free(temp);
		temp = temp2;
	}
}

void dezalocareLP(nodLP* capLP)
{
	nodLP* temp = capLP;
	while (temp != NULL)
	{
		nodLP* temp2 = temp->next;
		dezalocareLS(temp->inf);
		free(temp);
		temp = temp2;
	}
}


typedef struct
{
	int BF;
	produs inf;
	struct nodArb* left, *right;

}nodArb;

nodArb* creareNod(produs p, nodArb* stanga, nodArb* dreapta)
{
	nodArb* nou = (nodArb*)malloc(sizeof(nodArb));
	nou->inf.id = p.id;
	nou->inf.denumire = (char*)malloc((strlen(p.denumire) + 1) * sizeof(char));
	strcpy(nou->inf.denumire, p.denumire);
	nou->inf.valoare = p.valoare;
	nou->left = stanga;
	nou->right =dreapta;

	return nou;
}

nodArb* inserareNodArbore(nodArb* rad, produs p)
{
	if (rad != NULL)
	{
		if (p.id< rad->inf.id)
		{
			rad->left = inserareNodArbore(rad->left, p);
			return rad;
		}
		else
			if (p.id > rad->inf.id)
			{
				rad->right = inserareNodArbore(rad->right, p);
				return rad;
			}
			else
				return rad;
	}
	else
		return rad=creareNod(p, NULL, NULL);
}

void preordine(nodArb* rad)
{
	if (rad != NULL)
	{
		printf("\nID:%d, Denumire:%s, Valoare=%5.2f, BF=%d",
			rad->inf.id, rad->inf.denumire, rad->inf.valoare,rad->BF);
		preordine(rad->left);
		preordine(rad->right);
	}
}

void inordine(nodArb* rad)
{
	if (rad != NULL)
	{
		inordine(rad->left);
		printf("\nID:%d, Denumire:%s, Valoare=%5.2f, BF=%d",
			rad->inf.id, rad->inf.denumire, rad->inf.valoare, rad->BF);
		inordine(rad->right);

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

/*void conversieListaDublaArbore(nodLP* capLP, nodArb** rad)
{
	nodLP* temp = capLP;
	while (temp != NULL)
	{
		nodLS* temp2 = temp->inf;
		while (temp2 != NULL)
		{
			*rad = inserareNodArbore(*rad, temp2->inf);
			temp2 = temp2->next;
		}
		temp = temp->next;
	}
}
*/
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
	printf("\nRotatie stanga");
	nodArb* nod1 = rad->right;
	rad->right = nod1->left;
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

nodArb* reechilibrare(nodArb* rad)
{
	calculBF(rad);
	nodArb* fiuSt = rad->left;
	nodArb* fiuDr = rad->right;
	if (rad->BF == -2 && fiuSt->BF== -1)
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
	return rad;
}
//de fapt e Lista de liste
void conversieListaDublaArbore(nodLP* capLP, nodArb** rad)
{
	nodLP* temp = capLP;
	while (temp != NULL)
	{
		nodLS* temp2 = temp->inf;
		while (temp2 != NULL)
		{
			*rad = inserareNodArbore(*rad, temp2->inf);
			*rad = reechilibrare(*rad);
			temp2 = temp2->next;
		}
		temp = temp->next;
	}
}
void main()
{
	int nrProd;
	FILE* f = fopen("fisier.txt", "r");
	nodLS* preturiMici = NULL;
	nodLS* preturiMari = NULL;
	nodLP* capLP = NULL;
	produs p;
	nodArb* rad = NULL;
	nodArb* radacina=NULL;
	char buffer[20];
	fscanf(f, "%d", &nrProd);
	for (int i = 0; i < nrProd; i++)
	{
		fscanf(f, "%d", &p.id);
		fscanf(f, "%s", buffer);
		p.denumire = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(p.denumire, buffer);
		fscanf(f, "%f", &p.valoare);

		if (p.valoare < 11)
			preturiMici = inserareNod(preturiMici, p);
		else
			preturiMari = inserareNod(preturiMari, p);
	}
	fclose(f);

	capLP = inserareNodLP(capLP, preturiMici);
	capLP = inserareNodLP(capLP, preturiMari);
	
	printf("\n---------Conversie Arbore---------");
	printf("\nPreordine");
	conversieListaDublaArbore(capLP, &rad);
	preordine(rad);
	printf("\nInordine");
	inordine(rad);
	
	printf("\n---------REECHILIBRARE------------------\n");
	rad = reechilibrare(rad);
	preordine(rad);
	calculBF(rad);

	printf("\n---------Lista de Lista------------");
	traversareLP(capLP);
	dezalocareLP(capLP);
	dezalocareArbore(rad);

}