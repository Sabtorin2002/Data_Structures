#include<stdio.h>
#include<malloc.h>

typedef struct
{
	unsigned int id;
	char* denumireHotel;
	char* camereRezervate;
	char* numeClient;
	float sumaRon;
}rezervare;

typedef struct
{
	rezervare inf;
	struct nodArb* left, * right;

}nodArb;

nodArb* creareNod(rezervare r, nodArb* st, nodArb* dr)
{
	nodArb* nou = (nodArb*)malloc(sizeof(nodArb));
	nou->inf.id = r.id;
	nou->inf.denumireHotel = (char*)malloc((strlen(r.denumireHotel) + 1) * sizeof(char));
	strcpy(nou->inf.denumireHotel, r.denumireHotel);
	nou->inf.camereRezervate = (char*)malloc((strlen(r.camereRezervate) + 1) * sizeof(char));
	strcpy(nou->inf.camereRezervate, r.camereRezervate);
	nou->inf.numeClient = (char*)malloc((strlen(r.numeClient) + 1) * sizeof(char));
	strcpy(nou->inf.numeClient, r.numeClient);
	nou->inf.sumaRon = r.sumaRon;
	nou->left = st;
	nou->right = dr;
	
	return nou;

}

nodArb* inserareNod(nodArb* rad, rezervare r)
{
	if (rad != NULL)
	{
		if (r.id < rad->inf.id)
		{
			rad->left = inserareNod(rad->left, r);
			return rad;
		}
		else
			if (r.id > rad->inf.id)
			{
				rad->right = inserareNod(rad->right, r);
				return rad;
			}
			else
				return rad;
	}
	else
		return creareNod(r, NULL, NULL);
}

void preordine(nodArb* rad)//R S D
{
	if (rad != NULL)
	{
		printf("\nID:%d, Nume client:%s",
			rad->inf.id, rad->inf.numeClient);
		preordine(rad->left);
		preordine(rad->right);
	}
}

void inordine(nodArb* rad)//S R D
{
	if (rad != NULL)
	{
		inordine(rad->left);
		printf("\nID:%d, Nume client:%s",
			rad->inf.id, rad->inf.numeClient);
		inordine(rad->right);
	}
}

void postordine(nodArb* rad)//S D R
{
	if (rad != NULL)
	{
		postordine(rad->left);
		postordine(rad->right);
		printf("\nID:%d, Nume client:%s",
			rad->inf.id, rad->inf.numeClient);
	}
}

void dezalocare(nodArb* rad)
{
	if (rad != NULL)
	{
		free(rad->inf.denumireHotel);
		free(rad->inf.camereRezervate);
		free(rad->inf.numeClient);
		free(rad->left);
		free(rad->right);
		free(rad);
	}
}

float totalRezervari(nodArb* rad, char nume[20], float s)
{
	if (rad != NULL)
	{	
		
		if (strcmp(rad->inf.numeClient, nume) == 0)
		{
			s = s + rad->inf.sumaRon;
		}
		s=totalRezervari(rad->left, nume,s);
		s=totalRezervari(rad->right, nume,s);
	}
	return s;
}

nodArb* cautareNod(nodArb* rad, int cheie)
{
	if (rad != NULL)
	{
		if (rad->inf.id == cheie)
		{
			return rad;
		}
		else
		{
			if (rad->inf.id < cheie)
			{
				return cautareNod(rad->right, cheie);
			}
			else
			{
				return cautareNod(rad->left, cheie);
			}
		}
	}
	else
	{
		return NULL;
	}
}
nodArb* stergeRad(nodArb* rad)
{
	nodArb* aux = rad;
	if (rad->left != NULL)
	{
		rad = aux->left;
		if (aux->right != NULL)//ca sa se respecte strucutra arborelui
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
	free(aux->inf.denumireHotel);
	free(aux->inf.camereRezervate);
	free(aux->inf.numeClient);
	free(aux);

	return rad;
}

nodArb* stergeNod(nodArb* rad, int cheie)
{
	if (rad != NULL)
	{
		if (cheie < rad->inf.id)
			rad->left = stergeNod(rad->left, cheie);
		else
			if (cheie > rad->inf.id)
				rad->right = stergeNod(rad->right, cheie);
			else
				rad = stergeRad(rad);
		return rad;
	}
	else 
		return NULL;
}

nodArb* stergereNodMinim(nodArb* rad)
{
	if (rad == NULL)
	{
		return NULL;
	}

	if (rad->left == NULL)
	{
		nodArb* temp = rad;
		rad = rad->right;
		free(temp->inf.denumireHotel);
		free(temp->inf.camereRezervate);
		free(temp->inf.numeClient);
		free(temp);
	}
	else
	{
		nodArb* parinte = rad;
		nodArb* nodMinim = rad->left;

		while (nodMinim->left != NULL)
		{
			parinte = nodMinim;
			nodMinim = nodMinim->left;
		}

		parinte->left = nodMinim->right;
		free(nodMinim->inf.denumireHotel);
		free(nodMinim->inf.camereRezervate);
		free(nodMinim->inf.numeClient);
		free(nodMinim);
	}

	return rad;
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

void noduriPeNivel(nodArb* rad, int nivel, int** numarNoduri)
{
	if (rad == NULL)
	{
		return;
	}
	else
	{
		(*numarNoduri)[nivel]++;
		noduriPeNivel(rad->left, nivel + 1, numarNoduri);
		noduriPeNivel(rad->right, nivel + 1, numarNoduri);
	}

}
void main()
{
	int nrRez;
	rezervare r;
	char buffer1[20];
	char buffer2[20];
	char buffer3[20];
	nodArb* rad = NULL;

	FILE* f = fopen("fisier.txt", "r");
	fscanf(f, "%d", &nrRez);
	for (int i = 0; i < nrRez; i++)
	{
		fscanf(f, "%d", &r.id);
		fscanf(f, "%s", buffer1);
		r.denumireHotel = (char*)malloc((strlen(buffer1) + 1) * sizeof(char));
		strcpy(r.denumireHotel, buffer1);
		fscanf(f, "%s", buffer2);
		r.camereRezervate = (char*)malloc((strlen(buffer2) + 1) * sizeof(char));
		strcpy(r.camereRezervate, buffer2);
		fscanf(f, "%s", buffer3);
		r.numeClient = (char*)malloc((strlen(buffer3) + 1) * sizeof(char));
		strcpy(r.numeClient, buffer3);
		fscanf(f, "%f", &r.sumaRon);

		rad = inserareNod(rad, r);
		free(r.denumireHotel);
		free(r.camereRezervate);
		free(r.numeClient);
	}

	fclose(f);
	printf("\n----------Afisare preordine------\n");
	preordine(rad);
	//printf("\n----------Afisare inordine-------\n");
	//inordine(rad);

	float sumaPlatita=0;
	char numeClientEx2[20] = "Sabin";
	sumaPlatita=totalRezervari(rad, numeClientEx2,sumaPlatita);
	printf("\nSuma platitisa de clientul %s este %f", numeClientEx2, sumaPlatita);

	
	nodArb* nodMinim = cautareNod(rad, 15);
	if (nodMinim != NULL)
		printf("\nNodul cautat are ID-ul: %d", nodMinim->inf.id);
	else
		printf("\nNu exista cheia cautata");


	//rad = stergeNod(rad,15);
	rad = stergereNodMinim(rad);
	printf("\n-------Arbore dupa stergere------");
	preordine(rad);
	printf("\n----------------------\n");
	inordine(rad);

	int inaltime = inaltimeArbore(rad);
	int* numarNoduri = (int*)malloc(inaltime * sizeof(int));
	for (int i = 0; i < inaltime; i++)
	{
		numarNoduri[i] = 0;
	}
	noduriPeNivel(rad, 0, &numarNoduri);
	for (int i = 0; i < inaltime; i++)
	{
		printf("\nNivel %d:%d noduri\n", i, numarNoduri[i]);
	}
	free(numarNoduri);

	dezalocare(rad);
}