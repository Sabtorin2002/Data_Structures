#include<stdio.h>
#include<malloc.h>

typedef struct
{
	int id;
	char* denumire;
	int nrLocuitori;
}oras;

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

typedef struct
{
	int idNodStart, idNodFinal, greutateArc;
	struct nodLista* next;
}nodLista;

void push(nodStiva** varf, int val)//adaugarea unei valori in varful unei stive
{
	nodStiva* nou = (nodStiva*)malloc(sizeof(nodStiva));//se aloca spatiu pentru un nod noou de tip nodStiva
	nou->inf = val;//se atribuie valoarea val campului inf
	nou->next = NULL;//se seteaza campul next al noului nod cu NULL deoarece acesta va fi adaugat in varful stivei
	if (*varf == NULL)//se verifica daca stiva este goala
		*varf = nou;//daca este goala, se actualizeaza pointerul varf cu noul nod creat, devenid varful stivei
	else
	{
		nou->next = *varf;//noul nod va deveni primul element din stiva, urmatorul nod va deveni varful
		*varf = nou;//noul nod devine varful stivei, iar nodurile anterioare sunt reorganizate in josul stivei
	}
}

int pop(nodStiva** varf, int* val)
{
	if (*varf == NULL)//se verifica daca stiva este goala
		return -1;//daca este goala, returneaza -1
	else
	{
		*val = (*varf)->inf;//se atribuie valoare din inf al nodului curent de la varf pentru a returna valoarea extrasa
		//prin referinta, extrag inf din varf in variabila val
		nodStiva* temp = *varf;//un pointer temp care retine adresa nodului varf
		(*varf) = (*varf)->next;//se actualizeaza pointerul varf cu urmatorul nod din stiva, astfel eliminand
		//nodul curent de la varf din stiva, se trasnmite adresa nodului varf la urmatorul nod din stiva
		free(temp);//se elibereaza memoria ocupata de la nodul curent de la varf
		return 0;//se returneaza 0 pentru a indica ca extragerea a fost realizata
	}
}

void put(nodCoada** prim, nodCoada** ultim, int val)
{
	nodCoada* nou = (nodCoada*)malloc(sizeof(nodCoada));//se aloca memorie pentru un nod nou de tip Coada
	nou->inf = val;//se atribuie valoarea specificata val campului inf al noului nod
	nou->next = NULL;//se seteaza campul next al umratorului nod cu NULL deoarece acesta va fi adaugat la sfarsitul cozii
	if (*prim == NULL || *ultim == NULL)//se verifica daca lista este goala
	{
		*prim = nou;//se actualizeaza pointerii la primul si ultimul nod cu noul nod creat nou
		*ultim = nou;//deoarece acesta va fi singurul nod din coada
	}
	else//in caz contrar
	{
		(*ultim)->next = nou;//urmatorul nod din coada ia valorile ultimului nod nou
		*ultim=nou;//iar nodul nou este marcat ca ultim
	}
}

int get(nodCoada** prim, nodCoada** ultim, int* val)
{
	if (*prim != NULL && *ultim != NULL)//se verifica daca coada este goala
	{
		*val = (*prim)->inf;//se atribuie campul inf al primului nod in variabila val pentru a returna valoarea
		//extrasa prin referinta
		nodCoada* temp = *prim;//se creeaza un pointer temp care retine adresa primului nod
		*prim = (*prim)->next;//se actualizeaza adresa primului nod cu adresa urmatorului
		free(temp);//se elibereaza memorii ocupata de primul nod
		return 0;//se returneaza 0 pentru a indica extragerea
	}
	if (*prim == NULL)//daca primul nod din coada este gol
	{
		*ultim == NULL;//se actulizeaza pointerul ultim ca fiind null
		return -1;//se returneaza -1 pentru a indica ca nu a avut loc extragerea
	}
}

void parcurgereAdancime(int** mat, int nrNoduri, oras* vect, int* vizitat, int idNodStart)
{
	for (int i = 0; i < nrNoduri; i++)//se itereaza un vector vizitat cu valori initiale 0
		vizitat[i] = 0;
	
	nodStiva* varf = NULL;//se o variabiala "varf" de tip "nodStiva" initializata cu NULL
	//pentru parcurgerea in adancime, este utilizata stiva pentu a tine evidenta nodurilor de explorat

	push(&varf, idNodStart);//se adauga nodul de start in stiva, folosind functia push 
	vizitat[idNodStart] = 1;//si se marcheaza ca vizitat
	
	while (varf != NULL)//cat timp mai sunt valori in stiva
	{
		pop(&varf, &idNodStart);//la fiecare iteratie, se extrage un nod din varful stivei folosind functia pop
		printf("\n%d %s %d", idNodStart + 1, vect[idNodStart].denumire, vect[idNodStart].nrLocuitori);
		//se afiseaza informatiile despre nodul respectiv

		for(int k=0;k<nrNoduri;k++)//se parcurg toate nodurile 
			if (mat[idNodStart][k] != 0 && vizitat[k] == 0)//se verifica daca exista o muchie intre nodul idNodStart 
				//si nodul k si daca nodul k nu a fost vizitat inca
			{
				push(&varf, k);//se adauga nodul k in stiva
				vizitat[k] = 1;//se marcheaza nodul k ca vizitat
			}
	}
}

void parcurgereLatime(int** mat, int nrNoduri, oras* vect, int* vizitat, int idNodStart)
{
	for (int i = 0; i < nrNoduri; i++)//se itereaza vectorul vizitat
		vizitat[i] = 0;//se initilizeaza vectorul cu valori de 0

	nodCoada* prim = NULL, *ultim = NULL;//se initializeaza doua variabile "prim" si "ultim" de tip nodCoada cu NULL
	//pentru a parcurge graful in latime, este utilizata coada pentru a tine cont nodurile ce trebuiesc explorate
	
	put(&prim, &ultim, idNodStart);//se adauga nodul de start in coada, folosind functia put
	vizitat[idNodStart] = 1;//si se marcheaza nodul de start ca fiind vizitat

	while (prim != NULL)//cat timp mai sunt noduri in coada
	{
		get(&prim, &ultim, &idNodStart);//la fiecare iteratie se extrage un nod din inceputul cozii folosind functia get
		printf("\n%d %s %d", idNodStart + 1, vect[idNodStart].denumire, vect[idNodStart].nrLocuitori);
		//se afiseaza informatii despre nodul respectiv

		for (int k = 0; k < nrNoduri; k++)//se itereaza toate nodurile
			if (mat[idNodStart][k] != 0 && vizitat[k] == 0)//se verifica daca exista o muchie intre nodul idNodStart
				//si nodul k inca n a fost vizitat
			{
				put(&prim, &ultim, k);//se adauga nodul k in coada
				vizitat[k] = 1;//si se marcheaza ca fiind vizitat
			}
	}

}

/*void parcurgereLatime(int** mat, int nrNoduri, oras* vect,
	int* vizitat, int idNodStart)
{
	for (int i = 0; i < nrNoduri; i++)
		vizitat[i] = 0;

	nodCoada* prim = NULL, * ultim = NULL;

	put(&prim, &ultim, idNodStart);
	vizitat[idNodStart] = 1;
	while (prim != NULL)
	{
		get(&prim, &ultim, &idNodStart);
		printf("\n%d %s %d", idNodStart + 1,
			vect[idNodStart].denumire, vect[idNodStart].nrLocuitori);
		for (int k = 0; k < nrNoduri; k++)
			if (mat[idNodStart][k] != 0 && vizitat[k] == 0)
			{
				put(&prim, &ultim, k);
				vizitat[k] = 1;
			}
	}
}
*/
nodLista* inserareLista(nodLista* cap, int idNodS, int idNodF, int gr)
{
	nodLista* nou = (nodLista*)malloc(sizeof(nodLista));//se aloca spatiu in memorie pentru un nod de tip "nodLista"
	nou->idNodStart = idNodS;//se completeaza informatiile din noul nod cu valorile primite ca parametri
	nou->idNodFinal = idNodF;
	nou->greutateArc = gr;
	nou->next = NULL;//se seteaza urmatorul nod din lista ca fiind NULL

	if (cap == NULL)//daca lista este goala
		cap = nou;//nodul nou devine capul listei
	else
	{
		nodLista* temp = cap;//se initializeaza o variabila temp ce retine adresa nodului cap
		while (temp->next != NULL)//cat timp temp de urmator e diferit de NULL
			temp = temp->next;//temp ia valoarea urmatorului temp
		temp->next = nou;//temp de urmator ia valoarea nodului inserat
	}

	return cap;
}

void afisareLista(nodLista* cap)
{
	nodLista* temp = cap;//se initializeaza variabila temp ce ia valoarea adresei noduui cap
	while (temp != NULL)//cat timp mai sunt noduri in lista
	{
		printf("\n(%d, %d, %d) -> ", temp->idNodStart, temp->idNodFinal, temp->greutateArc);//se afiseaza informatiile
		//pentru nodul respectiv
		temp = temp->next;//dupa afisare, pointerul temp este mutat la urmatorul nod din lista prin atribuirea valorii
		//campului next 
	}
}

void transformListaArce(nodLista** cap, int** mat, int nrNoduri)
{
	for (int i = 0; i < nrNoduri; i++)//se itereaza toate perechile de noduri din matricea de adiacenta
		for (int j = 0; j < nrNoduri; j++)//utilizand 2 bucle imbricate
			//prima pentru a itera prin randuri, a doua pentru a iterea prin coloane
			if (mat[i][j] != 0)//se verifica daca este o conexiune intre nodurile i si j
				*cap = inserareLista(*cap, i + 1, j + 1, mat[i][j]);//in cazul in care exista o legatura, se foloseste
	//functia inserareLista pentru a adauga un nou arc in lista de arce
}

nodLista* arcGreutateMinima(nodLista* cap)
{
	nodLista* temp = cap;
	nodLista* minim = cap;
	while (temp != NULL)
	{
		if (minim->greutateArc > temp->greutateArc)
			minim = temp;
		temp = temp->next;
	}
	return minim;
}
void main()
{
	FILE* f = fopen("fisier.txt", "r");
	int nrNoduri, nrArce;
	fscanf(f, "%d", &nrNoduri);
	int** mat = (int**)malloc(nrNoduri * sizeof(int*));
	for (int i = 0; i < nrNoduri; i++)
		mat[i] = (int*)malloc(nrNoduri * sizeof(int));
	for (int i = 0; i < nrNoduri; i++)
		for (int j = 0; j < nrNoduri; j++)
			mat[i][j] = 0;

	oras* vect = (oras*)malloc(nrNoduri * sizeof(oras));
	char buffer[20];
	for (int i = 0; i < nrNoduri; i++)
	{
		fscanf(f, "%d", &vect[i].id);
		fscanf(f, "%s", buffer);
		vect[i].denumire = (char*)malloc((strlen(buffer) + 1) *
			sizeof(char));
		strcpy(vect[i].denumire, buffer);
		fscanf(f, "%d", &vect[i].nrLocuitori);
	}
	fscanf(f, "%d", &nrArce);
	int ii, jj, greutate;
	for (int i = 0; i < nrArce; i++)
	{
		fscanf(f, "%d", &ii);
		fscanf(f, "%d", &jj);
		fscanf(f, "%d", &greutate);
		mat[ii - 1][jj - 1] = greutate;
		mat[jj - 1][ii - 1] = greutate;
	}
	fclose(f);

	int* vizitat = (int*)malloc(nrNoduri * sizeof(int));
	int idNodStart;
	printf("\nNod start parcurgere adancime: ");
	scanf("%d", &idNodStart);
	parcurgereAdancime(mat, nrNoduri, vect, vizitat, idNodStart);
	printf("\nNod start parcurgere latime: ");
	scanf("%d", &idNodStart);
	parcurgereLatime(mat, nrNoduri, vect, vizitat, idNodStart);

	nodLista* cap = NULL;
	transformListaArce(&cap, mat, nrNoduri);
	afisareLista(cap);

	nodLista* min = arcGreutateMinima(cap);
	printf("\nArc greutate minima: (%d, %d, %d)",
		min->idNodStart, min->idNodFinal, min->greutateArc);
}