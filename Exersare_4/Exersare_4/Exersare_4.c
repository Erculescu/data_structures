#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>

#define LUNGIME_COD 11
#define LUNGIME_DATA 5
#define LUNGIME_HT 101



struct Firma {
	char cui[LUNGIME_COD];
	char* denumire;
	char data_infiintare[LUNGIME_DATA];
	char* adresa;

};
typedef struct Firma Firma;

struct Nod {
	Firma firma;
	struct Nod* next;
};
typedef struct Nod Nod;

unsigned char functie_HT(char* cheie, unsigned char dim_tabela) {
	unsigned int suma = 0;
	for (unsigned char i = 0; i < strlen(cheie); i++) {
		suma = suma + cheie[i];
	}
	return(suma % dim_tabela);
}



Nod* inserare_nod_sf(Nod* lista, Firma firma) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->firma = firma;
	nou->next = NULL;
	if (lista == NULL) {
		return nou;
	}
	else {
		Nod* temp = lista;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = nou;
		return lista;
	}
}

unsigned char inserare_HT(Nod** tabela, unsigned char dim_tab, Firma firma) {
	unsigned char poz = functie_HT(firma.cui, dim_tab);
	tabela[poz] = inserare_nod_sf(tabela[poz], firma);
	return (poz + 1);
}

Firma* cautare_HT(char* cheie, Nod** tabela, unsigned char dim_tabela) {
	unsigned char poz = functie_HT(cheie, dim_tabela);
	Nod* temp = tabela[poz];
	while (temp) {
		if (strcmp(temp->firma.cui, cheie) == 0) {
			return &(temp->firma);
		}
		temp = temp->next;
	}
	return NULL;
}

void traversare_lista(Nod* lista) {
	Nod* temp = lista;
	while (temp != NULL) {
		printf("Firma %s, are cui: %s, data infiintarii: %s, adresa: %s\n", temp->firma.denumire, temp->firma.cui, temp->firma.data_infiintare, temp->firma.adresa);
		temp = temp->next;
	}
}

void traversare_HT(Nod** tabela, unsigned char dim_tabela) {
	for (unsigned char i = 0; i < dim_tabela; i++) {
		if (tabela[i] != NULL) {
			printf("Pozitia %d contine urmatoarele variabile: \n ",i);
			traversare_lista(tabela[i]);
		}
	}
}

void stergere_lista(Nod* lista, char* cui) {
	Nod* temp = lista;
	Nod* temp2 = NULL;
	while (temp != NULL && strcmp(temp->firma.cui, cui) != 0) {
		temp2 = temp;
		temp = temp->next;
	}
	if (temp == NULL) {
		printf("Nu s-a gasit firma cu cui-ul %s\n", cui);
		return;
	}
	if (temp2 == NULL) {
		lista = temp->next;

	}
	else {
		temp2->next = temp->next;

	}
	free(temp->firma.denumire);
	free(temp->firma.adresa);
	free(temp);

}

void stergere_lista_completa(Nod* lista) {
	Nod* temp = lista;
	while (temp != NULL) {
		Nod* temp2 = temp;
		temp = temp->next;
		free(temp2->firma.denumire);
		free(temp2->firma.adresa);
		free(temp2);

	}

}

void stergere_HT(char* cheie, Nod** tabela, unsigned char dim_tabela) {
	unsigned char poz = functie_HT(cheie, dim_tabela);
	Nod* temp = tabela[poz];
	Nod* prev = NULL;
	while (temp != NULL) {
		if (strcmp(cheie, temp->firma.cui) == 0) {
			if (prev == NULL) {
				tabela[poz] = temp->next;
			}
			else {
				prev->next = temp->next;
			}
			free(temp->firma.denumire);
			free(temp->firma.adresa);
			free(temp);
			return 1;
		}
	}
	return 0;
}

int main() {
	FILE* f = fopen("firme.txt", "r");
	Firma firma;
	Nod* lista = NULL;
	
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
			token=strtok(NULL, separator);
			firma.adresa = (char*)malloc(strlen(token) + 1);
			strcpy(firma.adresa, token);
			lista = inserare_nod_sf(lista, firma);

		}
		fclose(f);

	}
	else {
		printf("Fisierul nu a putut fi deschis.\n");
		return 1;
	}
	printf("Lista initiala: \n");
	traversare_lista(lista);
	char cui[] = "1112234564";
	stergere_lista(lista, cui);
	printf("Lista dupa stergere: \n");
	traversare_lista(lista);
	stergere_lista_completa(lista);

	f = fopen("firme.txt", "r");
	Nod** HT = NULL;
	HT = (Nod**)malloc(LUNGIME_HT * sizeof(Nod*));
	for (unsigned char i = 0; i < LUNGIME_HT; i++) {
		HT[i] = NULL;
	}
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
			inserare_HT(HT, LUNGIME_HT, firma);
		}
		fclose(f);
	}
	printf("Hash table: \n");
	traversare_HT(HT, LUNGIME_HT);
	stergere_HT(cui, HT, LUNGIME_HT);
	printf("Hash table dupa stergere: \n");
	traversare_HT(HT, LUNGIME_HT);
	for (unsigned char i = 0; i < LUNGIME_HT; i++) {
		stergere_lista_completa(HT[i]);
	}


}