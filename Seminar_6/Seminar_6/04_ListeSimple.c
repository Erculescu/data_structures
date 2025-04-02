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
	char* titular; // 4 bytes
	char nr_card[DIM_NR_CARD + 1]; // +1 pt byte terminator de string; 20 bytes (17 + 3 padding)
	float sold; // 4 bytes
	char* emitent; // 4 bytes
	char data_expirare[DIM_DATA_EXPIRARE + 1]; // LL/AA; 8 bytes (6 + 2 padding)
};

typedef struct CardBancar CardBancar;

struct Nod
{
	CardBancar cb;
	struct Nod* next;
};

typedef struct Nod Nod;

unsigned char functie_hash(char* cheie, unsigned char dim_tabela) {

	unsigned int suma_ascii = 0;
	for (unsigned char i = 0; i < strlen(cheie); i++) {
		suma_ascii += cheie[i];
	}
	return (suma_ascii % dim_tabela);//translatare la dimensiunea tabelei de dispersie
}




Nod* inserare_nod_sfarsit(Nod* lista, CardBancar data)
{
	// alocare nod
	Nod* nou = malloc(sizeof(Nod));

	// populare nod cu data
	nou->cb = data; // partajare temporara mem heap pt titular si emitent
	nou->next = NULL; // nu exista succesor; nou va deveni ultimul nod in lista simpla

	if (lista == NULL)
	{
		// nu exista nici un nod in lista simpla
		return nou;
		// lista = nou; // lista se returneaza la finalul executiei functiei
	}
	else
	{
		// exista cel putin un nod in lista simpla
		// cautare sfarsit de lista
		Nod* temp = lista;
		while (temp->next != NULL)
			temp = temp->next; // actualizez temp pe nodul succesor

		// temp este plasat pe ultimul nod din lista simpla
		temp->next = nou; // legarea/atasarea lui nou la sfarsitul listei simple

	}
	// return lista actualizata daca este cazul
	return lista;
}

unsigned char inserare_HT(Nod** tabela, unsigned char dim_tabela, CardBancar data_card) {
	// 1. determinare pozitie in tabela de hash cu ajutorul functiei hash
	unsigned char pozitie_tabela = functie_hash(data_card.nr_card, dim_tabela);


	// 2. inserare date din card in lista cu pozitie_tabela in tabela
	tabela[pozitie_tabela] = inserare_nod_sfarsit(tabela[pozitie_tabela], data_card);

	return (pozitie_tabela+1);
}

void traversare_listasimpla(Nod* lista)
{
	Nod* temp = lista;
	while (temp)
	{
		printf("%s %s\n", temp->cb.nr_card, temp->cb.titular);
		temp = temp->next;
	}
}

Nod* stergere_inceput(Nod* lista)
{
	if (lista != NULL)
	{
		// exista cel putin 1 nod in lista simpla
		Nod* temp = lista;
		lista = lista->next; // actualizare inceput de lista pe nodul 2 (succesor)

		free(temp->cb.emitent); // dezalocare extensie in heap pentru CardBancar
		free(temp->cb.titular); // dezalocare extensie in heap pentru CardBancar

		free(temp); // dezalocare nod 
	}

	return lista;
}

// cautare card bancar pe baza de cheie(nrCard bancar)

CardBancar* cautare_HT(char* cheie, Nod* *tabela, unsigned char dim_tabela) {
	unsigned char pozitie_tabela = functie_hash(cheie, dim_tabela);
	
	Nod* temp = tabela[pozitie_tabela];//card bancar cautat ar trebui sa fie stocat in lista pozitie_tabela
	while (temp) {
		if (strcmp(cheie, temp->cb.nr_card) == 0)
		{//cardul bancar cu nr dat de cheie a fost identificat in tabela
			return &(temp->cb);
		}
		temp = temp ->next;
	}
	return NULL;
}



int main()
{
	FILE* f = NULL;
	f = fopen("CarduriBancare.txt", "r");
	if (f == NULL) {
		perror("Eroare la deschiderea fisierului.");
		return EXIT_FAILURE;
	}

	CardBancar card;
	Nod** HT = NULL;

	HT = (Nod**)malloc(DIM_TABELA_HASH * sizeof(Nod*));//alocare vector suport pentru tabela de dispersie
	
	if (HT == NULL) {
		perror("Eroare la alocarea memoriei.");
		fclose(f);
		return EXIT_FAILURE;
	}

	for (unsigned char i = 0; i < DIM_TABELA_HASH; i++) {
		HT[i] = NULL;//HT[i] este adresa de inceput a listei simple i din tabela de dispersie

	}

	char temp_buff[200]; // buffer temporar pentru stocare linii preluate din fisier text
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
		printf("Cardul %s inserat inserat in lista simpla %u din tabela hash cu chaining\n", card.nr_card, poz_tabela);

		// incercare preluare titular card urmator
		if (fgets(temp_buff, sizeof(temp_buff), f) == NULL)break;
	}
	fclose(f);

	// stergere la interior (criteriu definit)

	CardBancar* pCard = cautare_HT("0987676854321980", HT, DIM_TABELA_HASH);
	if (pCard != NULL) {
		printf("Card bancar identificat in tabela de dispersie: %s %s \n", pCard->nr_card, pCard->titular);
	}
	else {
		printf("Cardul bancar nu a fost identificat. \n");
	}
	return 0;
	//stergere card bancar pe baza de cheie!!!!
}