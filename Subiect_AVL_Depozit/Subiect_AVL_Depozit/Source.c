#include<stdio.h>
#include<malloc.h>

typedef struct
{
	int cod;
	char* locatie;
	int capacitate;
	int nrCategoriiCereale;
	char** denumiriCereale;
}depozit;

typedef struct
{
	int BF;
	depozit inf;
	struct nodArb* left, * right;
}nodArb;

nodArb* creareNod(depozit d, nodArb* st, nodArb* dr)
{
	nodArb* nou = (nodArb*)malloc(sizeof(nodArb));
	nou->inf.cod = d.cod;
	nou->inf.locatie = (char*)malloc((strlen(d.locatie) + 1) * sizeof(char));
	strcpy(nou->inf.locatie, d.locatie);
	nou->inf.capacitate = d.capacitate;
	nou->inf.nrCategoriiCereale = d.nrCategoriiCereale;
	nou->inf.denumiriCereale = (char**)malloc(nou->inf.nrCategoriiCereale * sizeof(char*));
	for (int i = 0; i < nou->inf.nrCategoriiCereale; i++)
	{
		nou->inf.denumiriCereale[i] = (char*)malloc((strlen(d.denumiriCereale[i]) + 1) * sizeof(char));
		strcpy(nou->inf.denumiriCereale[i], d.denumiriCereale[i]);
	}
	nou->left = st;
	nou->right = dr;

	return nou;
}

nodArb* inserareNod(nodArb* rad, depozit d)
{
	if (rad != NULL)
	{
		if (d.cod < rad->inf.cod)
		{
			rad->left = inserareNod(rad->left, d);
			return rad;
		}
		else
			if (d.cod > rad->inf.cod)
			{
				rad->right = inserareNod(rad->right, d);
				return rad;
			}
			else
				return rad;
	}
	else
		return creareNod(d, NULL, NULL);
}

void preordine(nodArb* rad)
{
	if (rad != NULL)
	{
		printf("\nCod=%d, Locatie=%s, Capacitate=%d, Numar categorii=%d, BF=%d",
			rad->inf.cod, rad->inf.locatie, rad->inf.capacitate, rad->inf.nrCategoriiCereale,
			rad->BF);
		printf("\nCereale:");
		for (int i = 0; i < rad->inf.nrCategoriiCereale; i++)
		{
			printf("%s ", rad->inf.denumiriCereale[i]);
		}
		preordine(rad->left);
		preordine(rad->right);
	}
}


void inordine(nodArb* rad)
{
	if (rad != NULL)
	{
		inordine(rad->left);
		printf("\nCod=%d, Locatie=%s, Capacitate=%d, Numar categorii=%d ,BF=%d",
			rad->inf.cod, rad->inf.locatie, rad->inf.capacitate, rad->inf.nrCategoriiCereale,
			rad->BF);
		printf(" Cereale:");
		for (int i = 0; i < rad->inf.nrCategoriiCereale; i++)
		{
			printf("%s ", rad->inf.denumiriCereale[i]);
		}
		inordine(rad->right);
	}
}

void postordine(nodArb* rad)
{
	if (rad != NULL)
	{
		postordine(rad->left);
		postordine(rad->right);
		printf("\nCod=%d, Locatie=%s, Capacitate=%d, Numar categorii=%d, BF=%d",
			rad->inf.cod, rad->inf.locatie, rad->inf.capacitate, rad->inf.nrCategoriiCereale,
			rad->BF);
		printf("\ Cereale:");
		for (int i = 0; i < rad->inf.nrCategoriiCereale; i++)
		{
			printf("%s ", rad->inf.denumiriCereale[i]);
		}
	}
}

void dezalocare(nodArb* rad)
{
	if (rad != NULL)
	{
		free(rad->inf.locatie);
		for (int i = 0; i < rad->inf.nrCategoriiCereale; i++)
		{
			free(rad->inf.denumiriCereale[i]);
		}
		free(rad->inf.denumiriCereale);
		dezalocare(rad->left);
		dezalocare(rad->right);
		free(rad);
	}
}

int maxim(int a, int b)
{
	int maxim = a;
	if (maxim < b)
		maxim = b;
	return maxim;
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

nodArb* rotatie_dreapta(nodArb* rad)
{
	if (rad != NULL)
	{
		printf("\nRotatie dreapta");
		nodArb* nod1 = rad->left;
		rad->left = nod1->right;
		nod1->right = rad;
		rad = nod1;

		return rad;

	}
}
nodArb* rotatie_stanga(nodArb* rad)
{
	if (rad != NULL)
	{
		printf("\nRotatie stanga");
		nodArb* nod1 = rad->right;
		rad->right = nod1->left;
		nod1->left = rad;
		rad = nod1;

		return rad;
	}
}
nodArb* rotatie_stanga_dreapta(nodArb* rad)
{
	nodArb* nod1 = rad->left;
	nodArb* nod2 = nod1->right;
	nod1->right = nod2->left;
	nod2->left = nod1;
	rad->left = nod2->right;
	nod2->right = rad;
	rad = nod2;
	
	return rad;
}
nodArb* rotatie_dreapta_stanga(nodArb* rad)
{
	nodArb* nod1 = rad->right;
	nodArb* nod2 = nod1->left;
	nod1->left = nod2->right;
	nod2->right = nod1;
	rad->right = nod2->left;
	nod2->left = rad;
	rad = nod2;

	return rad;
}

nodArb* reechilibrare(nodArb* rad)
{
	calculBF(rad);
	nodArb* fiuSt = rad->left;
	nodArb* fiuDr = rad->right;
	if (rad->BF == -2 && fiuSt ==-1)
	{
		rad = rotatie_dreapta(rad);
		calculBF(rad);
	}
	else
		if (rad->BF == 2 && fiuDr->BF == 1)
		{
			rad = rotatie_stanga(rad);
			calculBF(rad);
		}
		else
			if (rad->BF == 2 && fiuDr->BF == -1)
			{
				rad = rotatie_dreapta_stanga(rad);
				calculBF(rad);
			}
			else
				if (rad->BF == -2 && fiuSt->BF == 1)
				{
					rad = rotatie_stanga_dreapta(rad);
					calculBF(rad);
				}
	return rad;
}

void numarSecara(nodArb* rad, char cereale[20], int *c)
{
	if (rad != NULL)
	{
		for (int i = 0; i < rad->inf.nrCategoriiCereale; i++)
		{
			if (strcmp(rad->inf.denumiriCereale[i], cereale) == 0)
			{
				(*c)++;
			}
		}
		numarSecara(rad->left, cereale,c);
		numarSecara(rad->right, cereale,c);
	}

}

void numarareCategorii(nodArb* rad, int* cat)
{
	if (rad != NULL)
	{
		*cat = *cat + rad->inf.nrCategoriiCereale;
		numarareCategorii(rad->left, cat);
		numarareCategorii(rad->right, cat);
	}
}
void multimeCategoriiCereale(nodArb* rad, char*** vect, int* nr)
{
	if (rad != NULL)
	{
		for (int i = 0; i < rad->inf.nrCategoriiCereale; i++)
		{
			strcpy((*vect)[(*nr)], rad->inf.denumiriCereale[i]);
			(*nr)++;
		}
		multimeCategoriiCereale(rad->left, vect, nr);
		multimeCategoriiCereale(rad->right, vect, nr);
	}
}

nodArb* cautareInArbore(nodArb* rad, int cheie)
{
	if (rad != NULL)
	{
		if (rad->inf.cod == cheie)
		{
			return rad;
		}
		else
			if (cheie < rad->inf.cod)
			{
				cautareInArbore(rad->left, cheie);
			}
			else
					cautareInArbore(rad->right, cheie);
	}
	else
		return NULL;
}

void afisareCoduri(nodArb* rad, int** coduri, int* numar, int cheie)
{
	if (rad != NULL)
	{
		(*coduri)[(*numar)] = rad->inf.cod;
		(*numar)++;
		if (cheie < rad->inf.cod)
		{
			afisareCoduri(rad->left, coduri, numar, cheie);
		}
		else
			if (cheie > rad->inf.cod)
			{
				afisareCoduri(rad->right, coduri, numar, cheie);
			}
			else
				if (cheie == rad->inf.cod)
				{
					(*coduri)[(*numar)] = rad->inf.cod;
					(*numar)++;
				}
	}
}

typedef struct
{
	depozit inf;
	struct nodStiva* next;
}nodStiva;

nodStiva* push(nodStiva* varf, depozit d)
{
	nodStiva* nou = (nodStiva*)malloc(sizeof(nodStiva));
	nou->inf.cod = d.cod;
	nou->inf.locatie = (char*)malloc((strlen(d.locatie) + 1) * sizeof(char));
	strcpy(nou->inf.locatie, d.locatie);
	nou->inf.capacitate = d.capacitate;
	nou->inf.nrCategoriiCereale = d.nrCategoriiCereale;
	nou->inf.denumiriCereale = (char**)malloc(nou->inf.nrCategoriiCereale * sizeof(char*));
	for (int i = 0; i < nou->inf.nrCategoriiCereale; i++)
	{
		nou->inf.denumiriCereale[i] = (char*)malloc((strlen(d.denumiriCereale[i]) + 1) * sizeof(char));
		strcpy(nou->inf.denumiriCereale[i], d.denumiriCereale[i]);
	}

	nou->next = NULL;
	if (varf == NULL)
	{
		varf = nou;
	}
	else
	{
		nou->next = varf;
		varf = nou;
	}

	return varf;
}

int pop(nodStiva** varf)
{
	if (*varf == NULL)
		return -1;
	else
	{
		nodStiva* temp = *varf;
		*varf = (*varf)->next;
		free(temp->inf.locatie);
		for (int i = 0; i < temp->inf.nrCategoriiCereale; i++)
		{
			free(temp->inf.denumiriCereale[i]);
		}
		free(temp);
		return 0;
	}
}

void copiere(nodArb* rad, nodStiva** varf, int cod)
{
	if (rad != NULL)
	{
		if (rad->inf.cod == cod)
		{
			return;
		}
		*varf = push(*varf, rad->inf);
		copiere(rad->left, varf, cod);
		copiere(rad->right, varf, cod);
	}
}

void afisare(nodStiva* varf)
{
	nodStiva* temp = varf;
	while (temp != NULL)
	{
		printf("\nCod = % d, Locatie = % s, Capacitate = % d, Nr.Cat. = % d",
			temp->inf.cod, temp->inf.locatie, temp->inf.capacitate, temp->inf.nrCategoriiCereale);
		printf("\nCereale:");
		for (int i = 0; i < temp->inf.nrCategoriiCereale; i++)
		{
			printf("%s ",temp->inf.denumiriCereale[i]);
		}
		temp = temp->next;
	}
}
void main()
{
	int nrDep;
	depozit d;
	char buffer1[20];
	char buffer2[20][20];
	nodArb* rad = NULL;
	FILE* f = fopen("fisier.txt", "r");
	fscanf(f, "%d", &nrDep);
	for (int i = 0; i < nrDep; i++)
	{
		fscanf(f, "%d", &d.cod);
		fscanf(f, "%s", buffer1);
		d.locatie = (char*)malloc((strlen(buffer1) + 1) * sizeof(char));
		strcpy(d.locatie, buffer1);
		fscanf(f, "%d", &d.capacitate);
		fscanf(f, "%d", &d.nrCategoriiCereale);
		d.denumiriCereale = (char**)malloc(d.nrCategoriiCereale * sizeof(char*));
		for (int i = 0; i < d.nrCategoriiCereale; i++)
		{
			fscanf(f, "%s", buffer2[i]);
			d.denumiriCereale[i] = (char*)malloc((strlen(buffer2[i]) + 1) * sizeof(char));
			strcpy(d.denumiriCereale[i], buffer2[i]);
		}
		
		rad = inserareNod(rad, d);
		
		free(d.locatie);
		for (int i = 0; i < d.nrCategoriiCereale; i++)
		{
			free(d.denumiriCereale[i]);
		}
		free(d.denumiriCereale);

		rad = reechilibrare(rad);
	}

	fclose(f);
	calculBF(rad);
	printf("\n------------preordine------------\n");
	preordine(rad);
	printf("\n------------inordine-------------\n");
	inordine(rad);

	printf("\n-----------Depozite secara---------");
	int numarDepoziteSecara = 0;
	numarSecara(rad, "secara",&numarDepoziteSecara);
	printf("\nNumarul de depozite de secara este: %d", numarDepoziteSecara);

	int numarCategorii = 0;
	numarareCategorii(rad, &numarCategorii);
	printf("\nNumar categorii:%d", numarCategorii);
	
	char** multime = (char**)malloc(numarCategorii* sizeof(char*));
	for (int i = 0; i < numarCategorii; i++)
	{
		multime[i] = (char*)malloc(10 * sizeof(char));
	}
	
	int nr = 0;
	multimeCategoriiCereale(rad, &multime, &nr);
	for (int i = 0; i < numarCategorii; i++)
	{
		printf("\nDenumiri: %s,",multime[i]);
	}

	for (int i = 0; i < numarCategorii; i++)
	{
		free(multime[i]);
	}
	free(multime);

	int numarCoduri = 0;
	int* coduri = (int*)malloc(nrDep * sizeof(int));
	afisareCoduri(rad, &coduri, &numarCoduri, "15");

	for (int i = 0; i < numarCoduri; i++)
	{
		printf("\nCoduri: %d ", coduri[i]);
	}

	printf("\n---------Stiva---------\n");
	nodStiva* varf = (nodStiva*)malloc(nrDep * sizeof(nodStiva)); 
	varf = NULL;
	copiere(rad, &varf, "15");
	afisare(varf);


	dezalocare(rad);
}