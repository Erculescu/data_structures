#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#define DIM_NR_CARD 16
#define DIM_DATA_EXPIRARE 5

struct CardBancar
{
	char* titular;
	char nr_card[DIM_NR_CARD + 1];
	float sold;
	char* emitent;
	char data_expirare[DIM_DATA_EXPIRARE + 1];
};

typedef struct CardBancar CardBancar;

struct NodABC
{
	CardBancar cb;
	struct NodABC* stanga, * dreapta;
};

typedef struct NodABC NodABC;

NodABC* inserare_nod_ABC(NodABC* r, CardBancar data, unsigned char* inserat)
{
	if (r != NULL) // r este nod curent in ABC
	{
		// exista nod curent care trebuie verificat in raport cu cheia din data
		if (strcmp(data.nr_card, r->cb.nr_card) < 0)
		{
			// se continua cautare locaului de inserat pe descendentul stanga a nodului curent r
			r->stanga = inserare_nod_ABC(r->stanga, data, inserat);
		}
		else
		{
			if (strcmp(data.nr_card, r->cb.nr_card) > 0)
			{
				// se continua cautarea locului de inserat pe descendentul dreapta a nodului curent r
				r->dreapta = inserare_nod_ABC(r->dreapta, data, inserat);
			}
			else
			{
				// cheie de inserat este identificata in ABC
				// se abandoneaza operatia de inserare
				*inserat = 0;
			}
		}
	}
	else {
		// locul de inserat a fost identificat in ABC (este pozitie de NULL in structura ABC)
		r = malloc(sizeof(NodABC)); // alocare nod 

		r->cb = data;
		r->stanga = NULL;
		r->dreapta = NULL;

		// comutare flag inserare
		*inserat = 1;
	}

	return r;
}

void traversare_Inordine(NodABC* r)
{
	if (r != NULL)
	{
		traversare_Inordine(r->stanga);
		printf("Card %s\n", r->cb.nr_card);
		traversare_Inordine(r->dreapta);
	}
}

// dezalocare de ABC
NodABC* dezalocare_ABC(NodABC* r) {

	if (r != NULL) {
		//1.dezalocare noduri din subarbore stanga
		r->stanga=dezalocare_ABC(r->stanga);
		//2.dezalocare noduri din subarbore dreapta
		r->dreapta = dezalocare_ABC(r->dreapta);
		//3. prelucrare nod curent r: dezalocare/ stergere nod din ABC
		printf("Stergere nod %s\n", r->cb.nr_card);
		free(r->cb.titular);
		free(r->cb.emitent);
		free(r);
		r = NULL;
	}

	return r;
}

// determinare inaltime ABC (nr niveluri)
int inaltime(NodABC* r) {
	if (r == NULL) {
		return 0;
		
	}
	int inaltime_stanga = inaltime(r->stanga);
	int inaltime_dreapta = inaltime(r->dreapta);
	if (inaltime_stanga > inaltime_dreapta) {
		return inaltime_stanga + 1;
	}
	else {
		return inaltime_dreapta + 1;
	}
}

// determinare numar de noduri de pe nivel specificat
int nr_noduri_pe_nivel(NodABC* r, int nivel) {
	if (r == NULL) {
		return 0;
	}
	if (nivel == 1) { return 1; }
	else {
		return nr_noduri_pe_nivel(r->stanga, nivel - 1) + nr_noduri_pe_nivel(r->dreapta, nivel - 1);
	}
}

//stergere de noduri frunza in ABC

NodABC* dezalocare_noduri_frunza(NodABC* r) {
	if (r != NULL) {
		if (r->stanga == NULL && r->dreapta == NULL) {
			printf("Stergere nod %s\n", r->cb.nr_card);
			free(r->cb.titular);
			free(r->cb.emitent);
			free(r);
			r = NULL;
		}
		else {
			//r nu este frunza, continua alocarea pe cei doi descendenti
			r->stanga = dezalocare_noduri_frunza(r->stanga);
			r->dreapta = dezalocare_noduri_frunza(r->dreapta);
		}
	}
	return r;
}


NodABC* stergere_Nod_ABC(NodABC* r, char* cheie, unsigned char* flag_stergere) {

	if (r != NULL) {
		if (strcmp(cheie, r->cb.nr_card) < 0) {
			r->stanga = stergere_Nod_ABC(r->stanga, cheie, flag_stergere);
		}
		else {
			if (strcmp(cheie, r->cb.nr_card) > 0) {
				r->dreapta = stergere_Nod_ABC(r->dreapta, cheie, flag_stergere);
			}
			else {
				//a fost identificat nodul de sters
				*flag_stergere = 1;
				if (r->stanga == NULL && r->dreapta == NULL) {
					//r este nod frunza
					free(r->cb.emitent);
					free(r->cb.titular);
					free(r);
					r = NULL;
				}
				else {
					if (r->stanga != NULL && r->dreapta != NULL) {
						//TEMA: r are 2 descendenti
						NodABC* succesor = r->dreapta;
						while (succesor->stanga != NULL) {
							succesor = succesor->stanga;
						}
						strcpy(r->cb.nr_card, succesor->cb.nr_card);
						succesor->cb.titular = malloc(strlen(r->cb.titular) + 1);
						strcpy(r->cb.titular, succesor->cb.titular);
						succesor->cb.emitent = malloc(strlen(r->cb.emitent) + 1);
						strcpy(r->cb.emitent, succesor->cb.emitent);
						succesor->cb.sold = r->cb.sold;
						strcpy(r->cb.data_expirare, succesor->cb.data_expirare);
						succesor->cb.sold = r->cb.sold;
						free(r->cb.titular);
						free(r->cb.emitent);
						r->dreapta = stergere_Nod_ABC(r->dreapta, succesor->cb.nr_card, flag_stergere);
					}
					else
					{
					// r are 1 descendent

						NodABC* temp = NULL;
						if (r->stanga != NULL) {
							temp = r->stanga;
						}
						else {
							temp = r->dreapta;
						}

						free(r->cb.titular);
						free(r->cb.emitent);
						free(r);
						r = temp;//r se actualizeaza cu descendentul sau pe care il trimitem la parinte
					}
				}
			}
		}
	}
	else {
		*flag_stergere = 0;//nu a fost identificat nodul de sters in ABC
	}

	return r;
}

NodABC* cautare_Nod_ABC(NodABC* r, char* cheie) {
	if (r == NULL || strcmp(cheie, r->cb.nr_card) == 0) {
		return r;
	}
	if (strcmp(cheie, r->cb.nr_card) < 0) {
		return cautare_Nod_ABC(r->stanga, cheie);
	}
	else {
		return cautare_Nod_ABC(r->dreapta, cheie);
	}
}


int main()
{
	FILE* f = NULL;
	f = fopen("CarduriBancare.txt", "r");
	CardBancar card; // variabila stocare temporara a datelor unui card

	NodABC* root = NULL;

	char temp_buff[100]; // buffer temporar pentru stocare linii preluate din fisier text
	fgets(temp_buff, sizeof(temp_buff), f);

	while (!feof(f))
	{
		strtok(temp_buff, "\n");
		card.titular = malloc(strlen(temp_buff) + 1); // rescriere card.titular; mem leak potential
		strcpy(card.titular, temp_buff);

		fgets(temp_buff, sizeof(temp_buff), f);
		strtok(temp_buff, "\n");
		strcpy(card.nr_card, temp_buff);

		fgets(temp_buff, sizeof(temp_buff), f);
		strtok(temp_buff, "\n");
		card.sold = (float)atof(temp_buff); // conversie ASCII-to-Binary pentru float

		fgets(temp_buff, sizeof(temp_buff), f);
		strtok(temp_buff, "\n");
		strcpy(card.data_expirare, temp_buff);

		fgets(temp_buff, sizeof(temp_buff), f); //citirea emitent si data_expirare sunt inversate in implementarea mea
		strtok(temp_buff, "\n");
		card.emitent = malloc(strlen(temp_buff) + 1); // rescriere card.titular; mem leak potential
		strcpy(card.emitent, temp_buff);

		// inserare nod in arbore binar de cautare
		unsigned char inserat = 0; // 0 - esec inserare, 1 - succes inserare
		root = inserare_nod_ABC(root, card, &inserat);

		if (inserat != 0)
		{
			printf("Inserare cu succes card %s \n", card.nr_card);
		}
		else
		{
			printf("Inserare esuata card %s \n", card.nr_card);
			// dezalocare extensii card (pointerii emitent si titular) in mem heap
			free(card.emitent);
			free(card.titular);
		}

		// incercare preluare titular card urmator
		fgets(temp_buff, sizeof(temp_buff), f);
	}
	fclose(f);

	printf("\nArbore Binar de Cautare traversat in Inordine:\n");
	traversare_Inordine(root);

	printf("\nInaltimea arborelui este: %d\n", inaltime(root));

	printf("\nNumarul de noduri pe nivelul 2 este:%d\n", nr_noduri_pe_nivel(root, 2));

	NodABC* nod_cautat = NULL;
	nod_cautat = cautare_Nod_ABC(root, "0123415135364901");
	if (nod_cautat != NULL) {
		printf("\nCardul %s a fost gasit in arbore\n", nod_cautat->cb.nr_card);
	}
	else {
		printf("\nCardul nu a fost gasit in arbore\n");
	}

	printf("\nDezalocare noduri frunza:\n");
	root = dezalocare_noduri_frunza(root);
	traversare_Inordine(root);

	printf("\nInaltimea arborelui este: %d\n", inaltime(root));
		
	printf("\nDezalocare Arbore Binar de Cautare traversat in Inordine:\n");
	root = dezalocare_ABC(root);

	printf("\nArbore Binar de Cautare traversat in Inordine Dupa Dezalocare:\n");
	traversare_Inordine(root);

	

}