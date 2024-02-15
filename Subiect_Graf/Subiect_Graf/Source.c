#include<stdio.h>
#include<malloc.h>

typedef struct
{
	int id;
	char* denumire;
	char* localitate;
	float suprafata;
	int nrAngajati;
}magazin;

typedef struct
{
	int inf;
	struct nodStiva* next;
}nodStiva;

typedef struct
{
	int inf;
	struct nodCoada* next;
}nodCoada;

void push(nodStiva** varf, int val)
{
	nodStiva* nou = (nodStiva*)malloc(sizeof(nodStiva));
	nou->inf = val;
	nou->next = NULL;
	if (*varf == NULL)
	{
		*varf = nou;
	}
	else
	{
		nou->next = *varf;
		*varf = nou;
	}
}
int pop(nodStiva** varf, int* val)
{
	if (*varf == NULL)
		return -1;
	else
	{
		*val = (*varf)->inf;
		nodStiva* temp = *varf;
		*varf = (*varf)->next;
		free(temp);
		return 0;
	}
}

void put(nodCoada** prim, nodCoada** ultim, int val)
{
	nodCoada* nou = (nodCoada*)malloc(sizeof(nodCoada));
	nou->inf = val;
	nou->next = NULL;
	if (*prim == NULL || *ultim == NULL)
	{
		*prim = nou;
		*ultim = nou;
	}
	else
	{
		(*ultim)->next = nou;
		*ultim = nou;
	}
}

int get(nodCoada** prim, nodCoada** ultim, int* val)
{
	if (*prim != NULL && *ultim != NULL)
	{
		*val = (*prim)->inf;
		nodCoada* temp = *prim;
		*prim = (*prim)->next;
		free(temp);
		return 0;
	}
	if (*prim == NULL)
	{
		*ultim = NULL;
		return -1;
	}
}

void parcurgereAdancime(int** mat, int nrNoduri, magazin* vect, int* vizitat, int idNodstart)
{
	for (int i = 0; i < nrNoduri; i++)
		vizitat[i] = 0;//initializam vectorul cu zerouri

	nodStiva* varf = NULL;//initializare varf cu NULL
	
	push(&varf, idNodstart);//primul element din stiva
	vizitat[idNodstart] = 1;//il marcam ca vizitat
	while (varf != NULL)
	{
		pop(&varf, &idNodstart);//il extragem
		printf("\n %d %d %s %d", idNodstart + 1, vect[idNodstart].id, vect[idNodstart].denumire,
		vect[idNodstart].nrAngajati);//afisare
		for (int k = 0; k < nrNoduri; k++)//parcurgere vecini
		{
			if (mat[idNodstart][k] != 0 && vizitat[k] == 0)//daca e leg intre ei si nodul vecin nu a mai
				//fost vizitat
			{
				push(&varf, k);//introducere in stiva
				vizitat[k] = 1;//marcare ca vizitat
			}
		}

	}
}

void parcurgereLatime(int** mat, int nrNoduri, magazin* vect, int* vizitat, int idNodStart)
{
	for (int i = 0; i < nrNoduri; i++)
		vizitat[i] = 0;

	nodCoada* prim, * ultim = NULL;
	put(&prim, &ultim, idNodStart);
	vizitat[idNodStart] = 1;

	while (prim != NULL)
	{
		get(&prim, &ultim, &idNodStart);
		printf("\n%d %d %s", idNodStart + 1, vect[idNodStart].id, vect[idNodStart].denumire);

		for (int k = 0; k < nrNoduri; k++)
		{
			if (mat[idNodStart][k] != 0 && vizitat[k] == 0)
			{
				put(&prim, &ultim, k);
				vizitat[k] = 1;
			}
		}
	}
}

float suprafataTotala(magazin* vect, int nrNoduri)
{
	float suma = 0;
	for (int i = 0; i < nrNoduri; i++)
	{
		suma = suma + vect[i].suprafata;
	}
	return suma;
}

void valoareChirii(magazin* vect, int** v, int nrNoduri)
{
	float suma = 0;
	for (int i = 0; i < nrNoduri; i++)
	{
		(*v)[i] = 9 * vect[i].suprafata;
	}
}

/*void schimbareNumarAngajati(magazin** vect, int idEX6, int nrNoduri, int numarNouAng)
{
	for (int i = 0; i < nrNoduri; i++)
	{
		if ((*vect)[i].id == idEX6)
		{
			(*vect)[i].nrAngajati = numarNouAng;
			printf("\muie");
		}
	}
}
*/
void modificareAngajati(magazin** vect, int nrNoduri, int id, int nouAngajati)
{
	for (int i = 0; i < nrNoduri; i++)
	{
		if ((*vect)[i].id == id)
		{
			(*vect)[i].nrAngajati = nouAngajati;
		}
	}
}
void main()
{
	FILE* f = fopen("fisier.txt", "r");
	int nrNoduri, nrArce;
	fscanf(f, "%d", &nrNoduri);
	int** mat = (int**)malloc(nrNoduri * sizeof(int*));//alocare matrice
	for (int i = 0; i < nrNoduri; i++)
		mat[i] = (int*)malloc(nrNoduri * sizeof(int));
	for (int i = 0; i < nrNoduri; i++)//initializare matrice cu 0
		for (int j = 0; j < nrNoduri; j++)
			mat[i][j] = 0;
	
	//citire noduri din fisier
	magazin* vect = (magazin*)malloc(nrNoduri * sizeof(magazin));
	char buffer1[20];
	char buffer2[20];
	for (int i = 0; i < nrNoduri; i++)
	{
		fscanf(f, "%d", &vect[i].id);
		fscanf(f, "%s", buffer1);
		vect[i].denumire = (char*)malloc((strlen(buffer1) + 1) * sizeof(char));
		strcpy(vect[i].denumire, buffer1);
		fscanf(f, "%s", buffer2);
		vect[i].localitate = (char*)malloc((strlen(buffer2) + 1) * sizeof(char));
		strcpy(vect[i].localitate, buffer2);
		fscanf(f, "%f", &vect[i].suprafata);
		fscanf(f, "%d", &vect[i].nrAngajati);
	}
	//citire arce

	fscanf(f, "%d", &nrArce);
	int ii, jj, distanta;
	for (int i = 0; i < nrArce; i++)
	{
		fscanf(f, "%d", &ii);
		fscanf(f, "%d", &jj);
		fscanf(f, "%d", &distanta);
		mat[ii - 1][jj - 1] = distanta;
		mat[jj - 1][ii - 1] = distanta;
	}

	fclose(f);
	int* vizitat = (int*)malloc(nrNoduri * sizeof(int));
	int idNodStart;
	printf("\nNod start parcurgere adancime: ");
	scanf("%d", &idNodStart);
	//parcurgereAdancime(mat, nrNoduri, vect, vizitat, idNodStart);

	//printf("\nNod start parcurgere latima: ");
	//scanf("%d", &idNodStart);
	//parcurgereLatime(mat, nrNoduri, vect, vizitat, idNodStart);

	printf("\n----------Suprafata totala------------");
	float suprafataT = suprafataTotala(vect, nrNoduri);
	printf("\nSuprafata totala a magazinelor este: %5.2f", suprafataT);

	printf("\n-------------Chirii------------");
	int* chirii = (int*)malloc(nrNoduri * sizeof(int));
	valoareChirii(vect, &chirii, nrNoduri);
	for (int i = 0; i < nrNoduri; i++)
	{
		printf("\n %d ", chirii[i]);
	}

	//scriere intr-un fisier
	FILE* g = fopen("fisierscris.txt", "w");
	for (int i = 0; i < nrNoduri; i++)
	{
		printf("\nChirie%d=%d", i, chirii[i]);
		fprintf(g, "\n%d", chirii[i]);
	}

	fclose(g);
	

	printf("\n--------------MODIFICARE ANGAJATI------------\n");
	modificareAngajati(&vect, nrNoduri, 2, 100);
	parcurgereAdancime(mat, nrNoduri, vect, vizitat, idNodStart);
}