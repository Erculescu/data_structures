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

struct NodD
{
	CardBancar cb;
	struct NodD* next, * prev;
};

typedef struct NodD NodD;

struct ListaDubla {
	NodD* prim, * ultim;
};
typedef struct ListaDubla ListaDubla;

ListaDubla inserare_nod_sfarsit(ListaDubla lista, CardBancar data)
{
	NodD* nou = (NodD*)malloc(sizeof(NodD));
	nou->cb = data;
	nou->next = NULL;
	nou->prev = lista.ultim;
	if (lista.prim == NULL)
	{//lista este empty
		lista.prim = nou;

	}
	else {//lista contine cel putin un nod
		lista.ultim->next = nou;//atasarea lui nou la lista

	}
	lista.ultim = nou;
	return lista;

}
void traversare_listadubla(ListaDubla lista) {
	//traversare lista dubla in ambele sensuri
	NodD* temp = lista.prim;
	printf("\nTraversare lista dubla de la primul la ultimul :\n");
	while (temp) {
		printf("%s  %s\n", temp->cb.titular, temp->cb.nr_card);
		temp = temp->next;
	}
	temp = lista.ultim;
	printf("\nTraversare lista dubla de la ultimul la primul :\n");
	while (temp)
	{
		printf("%s  %s\n", temp->cb.titular, temp->cb.nr_card);
		temp = temp->prev;

	}
}

ListaDubla stergere_inceput(ListaDubla lista) {
	if (lista.prim != NULL) {
		NodD* temp = lista.prim;
		lista.prim = lista.prim->next;
		if (lista.prim == NULL) {
			//lista devine empty
			lista.ultim = NULL;
		}
		else {
			//lista contine cel putin 2 noduri inainte de stergere
			lista.prim->prev = NULL;

		}
		free(temp->cb.titular);//dezalocare 
		free(temp->cb.emitent);
		free(temp);//dezalocare nod de tip NodD
	}
	return lista;
}

int main()
{
	FILE* f = NULL;
	f = fopen("CarduriBancare.txt", "r");

	CardBancar card;
	ListaDubla listaD;
	listaD.prim = NULL;
	listaD.ultim = NULL;

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

		// inserare date card in lista dubla
		listaD = inserare_nod_sfarsit(listaD, card);


		// incercare preluare titular card urmator
		fgets(temp_buff, sizeof(temp_buff), f);
	}
	fclose(f);
	printf("Lista dubla dupa creare: \n");
	traversare_listadubla(listaD);
	//stergere nod la inceput
	listaD = stergere_inceput(listaD);
	printf("\nLista dubla dupa stergere nod la inceput: \n");
	traversare_listadubla(listaD);

	//dezalocare lista dubla
	while (listaD.prim != NULL) {
		//exista cel putin un nod in lista dubla care poate fi sters din lista
		listaD = stergere_inceput(listaD);

	}
	printf("\nLista dubla dupa dezalocare: \n");
	traversare_listadubla(listaD);

	// la interior (titular card bancar)
	//algoritmul quickSort
	return 0;
}