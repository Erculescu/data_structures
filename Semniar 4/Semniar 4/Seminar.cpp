#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>

#define DIM_NR_CARD 17
#define DIM_DATA_EX 5
#define NR_ELEM 3

struct CardBancar {
	char* titular;//4 bytes
	char nr_card[DIM_NR_CARD];//17 bytes + 3 bytes padding
	float sold;//4 bytes
	char data_expirare[DIM_DATA_EX + 1]; //formatul este LL/AA 8bytes
	char* emitent;//4 bytes


};
CardBancar creareCard(const char* titular, const char* nr_card, float sold, const char* data_expirare, const char* emitent) {
	CardBancar card;
	card.titular = (char*)malloc(strlen(titular) + 1);
	card.emitent = (char*)malloc(strlen(emitent) + 1);
	if (card.emitent && card.titular) {
		strcpy(card.titular, titular);
		strcpy(card.nr_card, nr_card);
		card.sold = sold;
		strcpy(card.data_expirare, data_expirare);
		strcpy(card.emitent, emitent);

	}
	return card;
}
//INCEPUT TEMA:

struct Nod {
	CardBancar cb;
	struct Nod* next;
};

Nod* creareNod(CardBancar cb) {
	Nod* nodNou = (Nod*)malloc(sizeof(Nod));
	if (!nodNou) {
		printf("Alocare esuata");
		exit(1);
	}
	nodNou->cb = cb;
	nodNou->next = NULL;
	return nodNou;
}


typedef struct Nod Nod;
typedef struct CardBancar CardBancar;

Nod* inserare_nod_sfarsit(Nod* lista,CardBancar data) {
	//cautare sfarsit de lista
	//alocare nod
	//populare nod cu data
	//return lista actualizata daca este cazul
	Nod* nodNou = creareNod(data);
	if (lista == NULL) {
		return nodNou;
	}
	else
	{
		Nod* temp = lista;
		while (temp->next) {
			temp = temp->next;
		}
		temp->next = nodNou;
		return lista;
	}

}
void afisareLista(Nod* lista) {
	Nod* temp = lista;
	while (temp != NULL) {
		printf("Titular: %s\n", temp->cb.titular);
		printf("Nr card: %s\n", temp->cb.nr_card);
		printf("Sold: %5.2f\n", temp->cb.sold);
		printf("Data expirare: %s\n", temp->cb.data_expirare);
		printf("Emitent: %s\n", temp->cb.emitent);
		temp = temp->next;
	}
}

void golireLista(Nod* lista) {
	Nod* temp = lista;
	while (lista) {
		temp = lista;
		lista = lista->next;
		free(temp->cb.titular);
		free(temp->cb.emitent);
		free(temp);
	}
}

int comparareDate(const char* data1, const char* data2) {
	int luna1, an1, luna2, an2;
	sscanf(data1, "%d/%d", &luna1, &an1);
	sscanf(data2, "%d/%d", &luna2, &an2);
	if (an1 < an2 || (an1 == an2 && luna1 < luna2)) {
		return -1;
	}
	else if (an1 == an2 && luna1 == luna2) {
		return 0;
	}
	else { return 1; }
}
void stergereNoduri(Nod** lista, const char* criteriu) {
	Nod* temp = *lista;
	Nod* anterior = NULL;
	while (temp != NULL) {
		if (comparareDate(temp->cb.data_expirare, criteriu) == -1) {
			if (anterior == NULL) {
				*lista = temp->next;
			}
			else {
				anterior->next = temp->next;

			}
			Nod* deSters = temp;
			temp = temp->next;
			free(deSters->cb.titular);
			free(deSters->cb.emitent);
			free(deSters);
		}
		else {
			anterior = temp;
			temp = temp->next;
		}
	}
}


int main() {
	FILE* f = NULL;
	f = fopen("carduriBancare.txt", "r");
	CardBancar card;
	char buffer_temp[100];//buffer temporar pentru stocare linii preluate din fisier text
	fgets(buffer_temp, sizeof(buffer_temp), f);
	Nod* lista = NULL;
	

	while (!feof(f)) {
		strtok(buffer_temp, "\n");
		card.titular = (char*)malloc(strlen(buffer_temp) + 1);//alocare mem heap titular element contor
		strcpy(card.titular, buffer_temp);//copiere din buffer in mem heap

		fgets(buffer_temp, sizeof(buffer_temp), f);
		strtok(buffer_temp, "\n");
		strcpy(card.nr_card, buffer_temp);

		fgets(buffer_temp, sizeof(buffer_temp), f);
		strtok(buffer_temp, "\n");
		card.sold = (float)atof(buffer_temp);

		fgets(buffer_temp, sizeof(buffer_temp), f);
		strtok(buffer_temp, "\n");
		strcpy(card.data_expirare, buffer_temp);

		fgets(buffer_temp, sizeof(buffer_temp), f);
		strtok(buffer_temp, "\n");
		card.emitent = (char*)malloc(strlen(buffer_temp) + 1);
		strcpy(card.emitent, buffer_temp);
		
		//inserare date card in lista simpla
		lista=inserare_nod_sfarsit(lista, card);
		//incercare prelucrare titular card urmator
		fgets(buffer_temp, sizeof(buffer_temp), f);
	}
	afisareLista(lista);
	stergereNoduri(&lista, "03/25");
	printf("\n___________________________\n");
	afisareLista(lista);
	golireLista(lista);
	

	fclose(f);
	//stergere nod la inceput si la interior cu un criteriu definit
	return 0;
}

