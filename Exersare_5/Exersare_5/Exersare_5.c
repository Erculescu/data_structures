#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LUNGIME_COD 10
#define LUNGIME_DATA 5

struct Firma {
	char cui[LUNGIME_COD];
	char* denumire;
	char data_infiintare[LUNGIME_DATA];
	char* adresa;
};
typedef struct Firma Firma;

struct NodD {
	Firma firma;
	struct NodD* prev;
	struct NodD* next;
};
typedef struct NodD NodD;

NodD* inserare_nod(NodD* lista, Firma firma) {
	NodD* nou = (NodD*)malloc(sizeof(NodD));
	nou->firma = firma;
	nou->prev = NULL;
	nou->next = NULL;
	if (lista == NULL) {
		return nou;
	}
	else {
		NodD* temp = lista;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = nou;
		nou->prev = temp;
		return lista;
	}
}

void traversare_lista(NodD* lista) {
	NodD* temp = lista;
	printf("Traversare lista de la cap la coada: \n");
	while (temp != NULL) {
		printf("CUI: %s, Denumire: %s, Data infiintare: %s , Adresa:%s\n", temp->firma.cui, temp->firma.denumire, temp->firma.data_infiintare, temp->firma.adresa);
		temp = temp->next;
	}
	printf("\nTraversare lista de la coada la cap: \n");
	temp = lista;
	while (temp->next != NULL) {
		temp = temp->next;
	}
	while(temp!=NULL){
		printf("CUI: %s, Denumire: %s, Data infiintare: %s , Adresa:%s\n", temp->firma.cui, temp->firma.denumire, temp->firma.data_infiintare, temp->firma.adresa);
		temp = temp->prev;
	}
}
NodD* cautare_firma(NodD* lista, char* cui) {
	NodD* temp = lista;
	while (temp != NULL) {
		if (strcmp(temp->firma.cui, cui) == 0) {
			return temp;
		}
		temp = temp->next;
	}
	return NULL;
}

void stergere_firma(NodD* lista, char* cui) {
	NodD* temp = cautare_firma(lista, cui);
	if (temp != NULL) {
		if (temp->prev != NULL) {
			temp->prev->next = temp->next;

		}
		if (temp->next == NULL) {
			temp->prev->next = NULL;
		}
		else {
			temp->next->prev = temp->prev;
		}
		free(temp->firma.denumire);
		free(temp->firma.adresa);
		free(temp);
	}
	else {
		printf("Firma cu CUI %s nu a fost gasita in lista.\n", cui);
	}
}

void interschimbare_firme(NodD* lista, int i) {
	int poz = 0;
	NodD* temp = lista;
	while (temp != NULL && poz < i) {
		temp = temp->next;
		poz++;
	}
	if (temp == NULL && temp->next==NULL) {
		return;
		}
	NodD* urmator = temp->next;
	if (temp->prev != NULL) {
		temp->prev->next = urmator;
	}
	if (urmator->next != NULL) {
		urmator->next->prev = temp;
	}
	urmator->prev = temp->prev;
	temp->next = urmator->next;
	urmator->next = temp;
	temp->prev = urmator;
	if (temp == lista) {
		lista = urmator;
	}

}


void eliberare_lista(NodD* lista) {
	NodD* temp;
	while (lista != NULL) {
		temp = lista;
		lista = lista->next;
		free(temp->firma.denumire);
		free(temp->firma.adresa);
		free(temp);

	}

}

int main() {
	FILE* f = fopen("firme.txt", "r");

	Firma firma;
	NodD* lista = NULL;

	char buffer[256];
	char* token, separator[] = ",";

	if (f != NULL) {
		while (fgets(buffer, sizeof(buffer), f) != NULL) {
			token = strtok(buffer, separator);
			strcpy(firma.cui, token);
			token = strtok(NULL, separator);
			firma.denumire = (char*)malloc(strlen(token) + 1);
			strcpy(firma.denumire, token);
			token = strtok(NULL, separator);
			strcpy(firma.data_infiintare, token);
			token = strtok(NULL, separator);
			firma.adresa = (char*)malloc(strlen(token) + 1);
			strcpy(firma.adresa, token);
			lista = inserare_nod(lista, firma);

		}
		fclose(f);
	}
	printf("Lista initiala:\n");
	traversare_lista(lista);
	char cui[] = "1112222221";
	stergere_firma(lista, cui);
	printf("Lista dupa stergere:\n");
	traversare_lista(lista);
	int i = 2;
	printf("Lista dupa interschimbare:\n");
	interschimbare_firme(lista, i);
	traversare_lista(lista);
	eliberare_lista(lista);
}