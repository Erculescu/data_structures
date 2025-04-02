#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#define DIM_NR_CARD 16
#define DIM_DATA_EXPIRARE 5
#define DIM_TABELA_HASH 53

struct CardBancar
{
	char* titular;
	char nr_card[DIM_NR_CARD + 1];
	float sold;
	char* emitent;
	char data_expirare[DIM_DATA_EXPIRARE + 1];
};

typedef struct CardBancar CardBancar;

struct Nod
{
	CardBancar cb;
	struct Nod* next;
};

typedef struct Nod Nod;

unsigned char functie_hash(char* cheie, unsigned char dim_tabela)
{
	unsigned int suma_ascii = 0;
	for (unsigned char i = 0; i < strlen(cheie); i++)
		suma_ascii += cheie[i];

	return (suma_ascii % dim_tabela); // translatare la dimensiunea tabelei de dispersie
}

Nod* inserare_nod_sfarsit(Nod* lista, CardBancar data)
{
	Nod* nou = malloc(sizeof(Nod));

	nou->cb = data;
	nou->next = NULL;

	if (lista == NULL)
	{
		return nou;
	}
	else
	{
		Nod* temp = lista;
		while (temp->next != NULL)
			temp = temp->next;

		temp->next = nou;

	}

	return lista;
}


unsigned char inserare_HT(Nod** tabela, unsigned char dim_tabela, CardBancar data_card)
{
	// 1. determinare pozitie in tabela hash cu ajutorul functiei hash
	unsigned char pozitie_tabela = functie_hash(data_card.nr_card, dim_tabela);

	// 2. inserare date din card in lista cu pozitie_tabela in tabela
	tabela[pozitie_tabela] = inserare_nod_sfarsit(tabela[pozitie_tabela], data_card);

	return (pozitie_tabela + 1);
}

// cautare card bancar pe baza de cheie (nr card bancar)
CardBancar* cautare_HT(char* cheie, Nod** tabela, unsigned char dim_tabela)
{
	unsigned char pozitie_tabela = functie_hash(cheie, dim_tabela);

	Nod* temp = tabela[pozitie_tabela]; // card bancar cautat ar trebuie sa fie stocat in lista pozitie_tabela
	while (temp != NULL)
	{
		if (strcmp(cheie, temp->cb.nr_card) == 0)
		{
			// cardul bancar cu nr dat de cheie a fost identificat in tabela
			return &(temp->cb);
		}

		temp = temp->next;
	}

	return NULL;
}
//functia de stergere a unui card bancar din tabela de dispersie.
int stergere_HT(char* cheie, Nod** tabela, unsigned char dim_tabela) {
	unsigned char pozitie_tabela = functie_hash(cheie, dim_tabela);
	Nod* temp = tabela[pozitie_tabela];
	Nod* prev = NULL;
	
	while (temp != NULL) {
		if (strcmp(cheie, temp->cb.nr_card) == 0) {
			if (prev == NULL) {
				tabela[pozitie_tabela] = temp->next;
				
			}
			else {
				prev->next = temp->next;
			}
			free(temp->cb.titular);
			free(temp->cb.emitent);
			free(temp);
			return 1;

		}
		prev = temp;
		
		temp = temp->next;

	}
	return 0;
}

int main()
{
	FILE* f = NULL;
	f = fopen("CarduriBancare.txt", "r");

	CardBancar card;
	Nod** HT = NULL; // vector alocat la runtime cu elemente de tip Nod*

	HT = malloc(DIM_TABELA_HASH * sizeof(Nod*)); // alocare vector suport pentru tabela de dispersie
	for (unsigned char i = 0; i < DIM_TABELA_HASH; i++)
		HT[i] = NULL; // HT[i] adresa de inceput a listei simple i din tabela de dispersie

	char temp_buff[100]; // buffer temporar pentru stocare linii preluate din fisier text
	fgets(temp_buff, sizeof(temp_buff), f);

	while (!feof(f))
	{
		strtok(temp_buff, "\n");
		card.titular = malloc(strlen(temp_buff) + 1);
		strcpy(card.titular, temp_buff);

		fgets(temp_buff, sizeof(temp_buff), f);
		strtok(temp_buff, "\n");
		strcpy(card.nr_card, temp_buff);

		fgets(temp_buff, sizeof(temp_buff), f);
		strtok(temp_buff, "\n");
		card.sold = (float)atof(temp_buff); // conversie ASCII-to-Binary pentru float

		fgets(temp_buff, sizeof(temp_buff), f);
		strtok(temp_buff, "\n");
		card.emitent = malloc(strlen(temp_buff) + 1);
		strcpy(card.emitent, temp_buff);

		fgets(temp_buff, sizeof(temp_buff), f);
		strtok(temp_buff, "\n");
		strcpy(card.data_expirare, temp_buff);

		// inserare card in tabela de dispersie
		unsigned char poz_tabela = inserare_HT(HT, DIM_TABELA_HASH, card);

		printf("Cardul %s inserat in lista simpla %u din tabela hash cu chaining\n", card.nr_card, poz_tabela);

		// incercare preluare titular card urmator
		fgets(temp_buff, sizeof(temp_buff), f);
	}
	fclose(f);

	CardBancar* pCard = cautare_HT("6234999919870000", HT, DIM_TABELA_HASH);
	if (pCard != NULL)
	{
		printf("\nCard bancar identificat in tabela de dispersie: %s %s\n", pCard->nr_card, pCard->titular);
	}
	else
	{
		printf("\nCardul bancar cautat nu exista in tabela de dispersie!\n");
	}

	// stergere card bancar pe baza de cheie
	int sters = stergere_HT("6234999919870005", HT, DIM_TABELA_HASH);
	if (sters)
	{
		printf("\nCard bancar identificat in tabela de dispersie a fost sters.\n");
	}
	else
	{
		printf("\nCardul bancar cautat pentru a fi sters nu exista in tabela de dispersie!\n");
	}
}