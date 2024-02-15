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
	produs inf;
	struct nodArb* left, * right;
}nodArb;

nodArb* creareNod(produs p, nodArb* st, nodArb* dr)
{
	nodArb* nou = (nodArb*)malloc(sizeof(nodArb));//alocare spatiu pentru nod
	nou->inf.cod = p.cod;
	nou->inf.denumire = (char*)malloc((strlen(p.denumire) + 1) * sizeof(char));
	strcpy(nou->inf.denumire, p.denumire);
	nou->inf.pret = p.pret;
	nou->left = st;//initializare nod stanga fiu
	nou->right = dr;//initializare nod dreapta fiu
	return nou;
}

nodArb* inserareNod(nodArb* rad, produs p)
{
	if (rad != NULL)//pentru a ne asigura ca arborele nu este gol si ca putem incepe inserarea
	{
		if (p.cod < rad->inf.cod)//daca valoarea codului nodului e mai mica decat cea din radacina, insereaza in stanga
		{
			rad->left = inserareNod(rad->left, p);
			return rad;//se returneaza pointerul la nodul radacina
		}
		else
			if (p.cod > rad->inf.cod)//daca val codului nodului e mai mare decat cea din radacina, insereaza in dreapta
			{
				rad->right = inserareNod(rad->right, p);
				return rad;
			}
			else
				return rad;//se returneaza pointerul la nodul radacina
	}
	else
		return creareNod(p, NULL, NULL);//indicarea unui arbore gol
}

void preordine(nodArb* rad)//R S D
{
	if (rad != NULL)
	{
		printf("\nCod=%d, Denumire=%s, Pret=%5.2f",//afisare informatii din nodul radacina
			rad->inf.cod, rad->inf.denumire, rad->inf.pret);
		preordine(rad->left);//apel recursiv pentru subarborele stanga
		preordine(rad->right);//apel recursiv pentru subarborele dreptq
	}
}
void inordine(nodArb* rad)//S R D
{
	if (rad != NULL)
	{
		inordine(rad->left);
		printf("\nCod=%d, Denumire=%s, Pret=%5.2f",
			rad->inf.cod, rad->inf.denumire, rad->inf.pret);
		inordine(rad->right);
	}
}

void postordine(nodArb* rad)//S D R
{
	if (rad != NULL)
	{
		postordine(rad->left);
		postordine(rad->right);
		printf("\nCod=%d, Denumire=%s, Pret=%5.2f",
			rad->inf.cod, rad->inf.denumire, rad->inf.pret);
	}
}
void dezalocare(nodArb* rad)
{
	if (rad != NULL)
	{
		free(rad->inf.denumire);
		dezalocare(rad->left);
		dezalocare(rad->right);
		free(rad);
	}
}

void numarareNoduriFrunza(nodArb* rad, int* nr)
{
	if (rad != NULL)
	{
		if (rad->left == NULL && rad->right == NULL)//verifacare daca nodul are un copil
			(*nr)++;
		numarareNoduriFrunza(rad->left, nr);//apelare pentru copilul din stanga al nodului curent
		numarareNoduriFrunza(rad->right, nr);//apelare pentru copilul din dreapta al nodului curent
	}
}

void salvareVector(nodArb* rad, produs* vect, int* nr, float prag)//salavarea in vectorul vect toate produsele din
//arborele binar care au pret mai mare sau egal cu pragul specificat
{
	if (rad != NULL)
	{
		if (rad->inf.pret >= prag)//verificare daca pretul din nodul curent, aici radacina, este mai mare sau egal 
			//cu pragul
		{
			vect[*nr] = rad->inf;//produsul din nodul curent este copiat in vectorul vect la pozitia indcata de valoarea
			//stocata in variabila nr
			(*nr)++;
		}
		salvareVector(rad->left, vect, nr, prag);//apelare recursiva pentru copilul stanga
		salvareVector(rad->right, vect, nr, prag);//apelare recursiva pentru copilul dreapta
	}
}

nodArb* cautare(nodArb* rad, int cheie)//scopul functiei este de a cauta si returna un pointer catre nodul de informatie
//egal cu cheia specificata in arborele binar
{
	if (rad != NULL)
	{
		if (rad->inf.cod == cheie)//se compara cheia cu inf. din radacina si daca sunt egale alg se incheie
		{
			return rad;
		}
		else
		{
			if (cheie < rad->inf.cod)//daca cheia specificata este mai mica decat inf.utila din radacina
				//se cauta in subarborele stanga, si daca nu se gaseste atunci ca informatia nu este in arbore
				return cautare(rad->left, cheie);
			else
				return cautare(rad->right, cheie);//in caz contrar, se cauta in subarborele drept
		}
	}
	else
		return NULL;
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
	}
	fclose(f);

	preordine(rad);

	printf("\n----------------------------------\n");
	inordine(rad);

	printf("\n----------------------------------\n");
	postordine(rad);

	int numarFrunze = 0;
	numarareNoduriFrunza(rad, &numarFrunze);
	printf("\nNumar noduri frunza: %d", numarFrunze);

	nodArb* nodCautat = cautare(rad, 35);
	if (nodCautat != NULL)
		printf("\nNodul cautate are denumirea: %s", nodCautat->inf.denumire);
	else
		printf("\nNu exista cheia cautata");

	int nr = 0;
	produs* vect = (produs*)malloc(nrProd * sizeof(produs));
	salvareVector(rad, vect, &nr, 7);
	for (int i = 0; i < nr; i++)
		printf("\nCod=%d, Denumire=%s, Pret=%5.2f",
			vect[i].cod, vect[i].denumire, vect[i].pret);
	free(vect);

	dezalocare(rad);
}