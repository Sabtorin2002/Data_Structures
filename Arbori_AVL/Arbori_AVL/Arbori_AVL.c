#include <stdio.h>
#include <malloc.h>

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
	struct nodArb* left, *right;
}nodArb;

nodArb* creareNod(produs p, nodArb* st, nodArb* dr)
{
	nodArb* nou = (nodArb*)malloc(sizeof(nodArb));//alocare memorie nod
	nou->inf.cod = p.cod;
	nou->inf.denumire = (char*)malloc((strlen(p.denumire) + 1) * sizeof(char));
	strcpy(nou->inf.denumire, p.denumire);
	nou->inf.pret = p.pret;
	nou->left = st;//initializare nod fiu stanga
	nou->right = dr;//initializare nod fiu dreapta
	return nou;
}

nodArb* inserareNod(nodArb* rad, produs p)
{
	if (rad != NULL)//daca radacina e diferita de NULL
	{
		if (p.cod < rad->inf.cod)//daca codul este mai mic decat al radacinii
		{
			rad->left = inserareNod(rad->left, p);//inserare nod in stanga
			return rad;
		}
		else
			if (p.cod > rad->inf.cod)//daca codul este mai mare decat la radacinii
			{
				rad->right=inserareNod(rad->right, p);//inserare nod in dreapta
				return rad;
			}
			else
				return rad;
	}
	else
		return creareNod(p, NULL, NULL);//acel nod e radacina deoarece este primul nod inserat
}

void preordine(nodArb* rad) //R S D
{
	if (rad != NULL)
	{
		printf("\nCod=%d, Denumire=%s, Pret=%5.2f, BF=%d",
			rad->inf.cod, rad->inf.denumire, rad->inf.pret, rad->BF);//afisare valori
		preordine(rad->left);
		preordine(rad->right);
	}
}

void inordine(nodArb* rad)//S R D
{
	if (rad != NULL)
	{
		inordine(rad->left);
		printf("\nCod=%d, Denumire=%s, Pret=%5.2f, BF=%d",
			rad->inf.cod, rad->inf.denumire, rad->inf.pret, rad->BF);
		inordine(rad->right);
	}
}

void postordine(nodArb* rad)//S D R
{
	if (rad != NULL)
	{
		postordine(rad->left);
		postordine(rad->right);
		printf("\nCod=%d, Denumire=%s, Pret=%5.2f, BF=%d",
			rad->inf.cod, rad->inf.denumire, rad->inf.pret, rad->BF);
	}
}

void dezalocare(nodArb* rad)
{
	if (rad != NULL)
	{
		free(rad->inf.denumire);//dezalocare denumiri
		dezalocare(rad->left);//dezalocare subarbore stanga
		dezalocare(rad->right);//dezalocare subarbore dreapta
		free(rad);//dezalocare radacina
	}
}

void numarareNoduriFrunza(nodArb* rad, int* nr)
{
	if (rad != NULL)
	{
		if (rad->left == NULL && rad->right == NULL)//daca nodul stanga fiu e gol si nodul dreapta fiu e gol
			(*nr)++;//incrementare variabila
		numarareNoduriFrunza(rad->left, nr);
		numarareNoduriFrunza(rad->right, nr);
	}
}

void salvareVector(nodArb* rad, produs* vect, int* nr, float prag)
{
	if (rad != NULL)
	{
		if (rad->inf.cod >= prag)//daca codul nodului e mai mare decat pragul
		{
			vect[*nr] = rad->inf;//salvarea informatiei nodului intr=un vector
			(*nr)++;//incrementare numar
		}
		salvareVector(rad->left, vect, nr, prag);//apel recurisv nod stanga fiu
		salvareVector(rad->right, vect, nr, prag);//apel recursiv nod dreapta fiu
	}
}

void conversieArboreVector(nodArb* rad, produs* vect, int* nr)
{
	if (rad != NULL)
	{	//aceeasi pasi ca la creare nod
		vect[*nr].cod = rad->inf.cod;
		vect[*nr].denumire = (char*)malloc((strlen(rad->inf.denumire) + 1) * sizeof(char));
		strcpy(vect[*nr].denumire, rad->inf.denumire);
		vect[*nr].pret = rad->inf.pret;
		(*nr)++;//incrementare numar

		nodArb* st = rad->left;
		nodArb* dr = rad->right;
		free(rad->inf.denumire);
		free(rad);
		conversieArboreVector(st, vect, nr);
		conversieArboreVector(dr, vect, nr);
	}
}

nodArb* cautare(nodArb* rad, int cheie)
{
	if (rad != NULL)
	{
		if (rad->inf.cod == cheie)//daca codul cautat este la radacina 
			return rad;//returnez radacina
		else
			if (cheie < rad->inf.cod)//daca codul cautat este este mai mic decat cheia
				return cautare(rad->left, cheie);//cautam in stanga
			else return cautare(rad->right, cheie);//alfetl, cautam in dreapta
	}
	else
		return NULL;//cheia cautata nu a fost gasita
}

nodArb* stergeRad(nodArb* rad)
{
	nodArb* aux = rad;//variabila aux care ia valoarea adresei radacinii
	if (rad->left != NULL)//se verifica daca radacina are un nod fiu in stanga, si daca are
	{
		rad = aux->left;//radacina se actualizeaza ca fiind fiul din stanga
		if (aux->right != NULL)//se verifica daca e un nod fiu in dreapta al fostei radacinii, si daca e
		{
			nodArb* max = aux->left;//se initializeaza un nou nod max care cauta in subarborele din stanga
			//max este fiul radacinii initiale din stanga
			while (max->right != NULL)//cat timp exista un fiu in dreapta
				//adica se parcurge arborele din stanga pana la nodul cu cheia maxima
				max = max->right;//max ia valoarea fiului sau din dreapta
			max->right = aux->right;//nodul cu cheia maxima devine acum fiul din dreapta al nodului radacina
			//pentru a pastra ordinea corecta a valorilor in arbore
		}
	}
	else
	{
		if (aux->right != NULL)//daca nu exista nodul fiu stanga al radacinii si exista un nod fiu dreapta al radacinii
		{
			rad = aux->right;//radacina devine nodul fiu din dreapta acesteia
		}
		else
			rad = NULL;//indicarea arborelui ca este gol
	}
	free(aux->inf.denumire);//se elibereaza memoria alocata pentru campul denumire
	free(aux);//se elibereaza memoria alocata pentru nodul curent
	return rad;
}

nodArb* stergeNod(nodArb* rad, int cheie)
{
	if (rad != NULL)//se verifica daca arborele nu este gol
	{
		if (cheie < rad->inf.cod)//se verifica daca cheia specificata e mai mica decat cheia radacinii
			rad->left = stergeNod(rad->left, cheie);//daca conditie este indeplinita, se continua cautarea si steregerea
		//nodului in subarborele din stanga
		else
			if (cheie > rad->inf.cod)//se verifca daca cheia specificata este mai mare decat cheia radacinii
				rad->right = stergeNod(rad->right, cheie);//daca da, se continua cautarea si stergerea nodului
		//in subarborele din dreapta
			else
				rad = stergeRad(rad);//cazul ramas in care cheia este egala cu codul radacinii si se sterge radacina
	}
	else
		return NULL;
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
		return 1 + maxim(inaltimeArbore(rad->left), inaltimeArbore(rad->right));//comparatia dintre subarborele stang
	//si subarborele drept
	else
		return 0;
}

void calculBF(nodArb* rad)
{
	if (rad != NULL)
	{
		rad->BF = inaltimeArbore(rad->right) - inaltimeArbore(rad->left);//gradul de echilibru pentru radacina
		calculBF(rad->left);//gradurile de echilibru pentru toate nodurile din subarborele stang
		calculBF(rad->right);//gradurile de echilibru pentru toate nodurile din subarborele drept
	}
}

nodArb* rotatie_dreapta(nodArb* rad)
{
	printf("\nRotatie dreapta");
	nodArb* nod1 = rad->left;
	rad->left = nod1->right;
	nod1->right = rad;
	rad = nod1;
	return rad;
}

nodArb* rotatie_stanga(nodArb* rad)
{
	printf("\nRotatie stanga");
	nodArb* nod1 = rad->right;
	rad->right = nod1->left;
	nod1->left = rad;
	rad = nod1;
	return rad;
}

nodArb* rotatie_stanga_dreapta(nodArb* rad)
{
	printf("\nRotatie stanga-dreapta");
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
	printf("\nRotatie dreapta-stanga");
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
	if (rad->BF == -2 && fiuSt->BF ==-1)
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
		{
			if (rad->BF == -2 && fiuSt->BF == 1)
			{
				rad = rotatie_stanga_dreapta(rad);
				calculBF(rad);
			}
			else
				if(rad->BF == 2 && fiuDr->BF == -1)
				{
				rad = rotatie_dreapta_stanga(rad);
				calculBF(rad);
				}
		}
	return rad;
}
void main()
{
	int nrProd;
	produs p;
	char buffer[20];
	nodArb* rad = NULL;
	FILE* f = fopen("fisier.txt", "r");
	fscanf(f, "%d", &nrProd);
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

	fclose(f);

	//calculBF(rad);

	printf("\n---------preordine---------");
	preordine(rad);
	printf("\n---------inordine---------");
	inordine(rad);
	printf("\n---------postordine---------");
	postordine(rad);
	
	int numarFrunze = 0;
	numarareNoduriFrunza(rad, &numarFrunze);
	printf("\nNumar noduri frunza: %d", numarFrunze);

	nodArb* nodCautat = cautare(rad, 23);
	if (nodCautat != NULL)
		printf("\nNodul cautat are denumire: %s", nodCautat->inf.denumire);
	else
		printf("\nNu exista cheia cautata");
	
	int nr = 0;
	produs* vect = (produs*)malloc(nrProd * sizeof(produs));
	salvareVector(rad, vect, &nr, 27);
	for (int i = 0; i < nr; i++)
	{
		printf("\nCod=%d, Denumire=%s, Pret=%5.2f",
			vect[i].cod, vect[i].denumire, vect[i].pret);
	}

	free(vect);

	rad = stergeRad(rad);
	//rad = stergeNod(rad,20);
	//calculBF(rad);
	printf("\n");
	printf("\n---------preordine---------");
	preordine(rad);
	printf("\n---------inordine---------");
	inordine(rad);
	printf("\n---------postordine---------");
	postordine(rad);

	//printf("\nInaltime arbore: %d", inaltimeArbore(rad));
	//printf("\nInaltime subarbore stang: %d", inaltimeArbore(rad->left));
	//printf("\nInaltime subarbore drept: %d", inaltimeArbore(rad->right));

	dezalocare(rad);
}
