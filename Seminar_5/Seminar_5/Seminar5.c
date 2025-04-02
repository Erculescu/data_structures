#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#define DIM_NR_CARD 16
#define DIM_DATA_EXPIRARE 5

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

Nod* inserare_nod_sfarsit(Nod* lista, CardBancar data)
{
	// cautare sfarsit de lista
	// alocare nod
	Nod* nou = malloc(sizeof(Nod));
	// populre nod cu data
	nou->cb = data;//partajare temporara memorie heap pt titular si emitent
	nou->next = NULL;//nu exista succesor nou, va deveni ultimul nod din lista
	
	if (lista == NULL) {
		//nu exista niciun nod in lista simpla
		return nou;//sau lista=nou; astfel incat lista se returneaza la finalul functiei
	}
	else {
		//exista cel putin un nod
		//cautare sfarsit de lista
		Nod* temp = lista;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		//temp este plasat pe ultimul nod din lista simpla
		temp->next = nou;//legarea lui nou la sfarsitul listei simple
		

	}
	// return lista actualizata daca este cazul
	return lista;
}
void traversare_listasimpla(Nod* lista) {
	Nod* temp = lista;
	while (temp) {
		printf("%s %s\n", temp->cb.nr_card, temp->cb.titular);
		temp = temp->next; //actualizez temp pe nodul succesor
	}
}

Nod* stergere_inceput(Nod* lista) {
	if (lista) {
		//exista cel putin un nod in lista simpla
		Nod* temp = lista;
		lista = lista->next;//actualizare inceput de lista pe nodul 2(succesor)
		free(temp->cb.titular);//dezalocare extensie in heap pentru CardBancar
		free(temp->cb.emitent);//dezalocare extensie in heap pentru CardBancar
		free(temp);//dezalocare not

	}
	return lista;
}

int main()
{
	FILE* f = NULL;
	f = fopen("CarduriBancare.txt", "r");

	CardBancar card;
	Nod* prim = NULL; // variabila pointer acces la lista simpla

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

		// inserare date card in lista simpla
		prim = inserare_nod_sfarsit(prim, card);


		// incercare preluare titular card urmator
		fgets(temp_buff, sizeof(temp_buff), f);
	}
	fclose(f);
	printf("Lista simpla dupa creare: \n");
	traversare_listasimpla(prim);
	// stergere nosd la inceput
	prim = stergere_inceput(prim);
	printf("\nLista simpla dupa stergere nod la inceput: \n");
	traversare_listasimpla(prim);

	//dezalocare lista simpla
	while (prim != NULL) {
		//exista cel putin un nod in lista simpla care poate fi sters din lista
		prim = stergere_inceput(prim);
		
	}
	printf("\nLista simpla dupa dezalocare: \n");
	traversare_listasimpla(prim);

	// la interior (criteriu definit)

	return 0;
}