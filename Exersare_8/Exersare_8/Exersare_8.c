#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

enum Status
{
	Boarding, On_Tarmack, In_Air

};

struct CursaAeriana {
	char* cod;
	char* destinatie;
	char* data;
	char* ora;
	enum Status status;
};

typedef struct CursaAeriana CursaAeriana;

struct Nod {
	CursaAeriana cursa;
	struct Nod* next;
};
typedef struct Nod Nod;

Nod* adauga_lista(Nod* lista, CursaAeriana cursa) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->cursa = cursa;
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

void traversare_lista(Nod* lista) {
	Nod* temp = lista;
	while (temp != NULL) {
		printf("Zborul %s, cu destinatia %s, din data de %s, ora %s are statusul %d\n",temp->cursa.cod,temp->cursa.destinatie,temp->cursa.data,temp->cursa.ora,temp->cursa.status);
		temp = temp->next;
	}
}

void stergere_zboruri(Nod** lista, enum Status status) {

	Nod* temp = *lista;
	Nod* prev = NULL;
	while (temp != NULL) {
		if (temp->cursa.status == status) {
			Nod* de_sters = temp;
			if (prev == NULL) {
				*lista = temp->next;
			}
			else {
				prev->next = temp->next;
			}
			temp = temp->next;
			free(de_sters->cursa.cod);
			free(de_sters->cursa.destinatie);
			free(de_sters->cursa.data);
			free(de_sters->cursa.ora);
			free(de_sters);
		}
		else {
			prev = temp;
			temp = temp->next;
		}
	}
	
}

void dezalocare_lista(Nod* lista) {
	Nod* temp = lista;
	while (temp != NULL) {
		Nod* de_sters = temp;
		temp = temp->next;
		free(de_sters->cursa.cod);
		free(de_sters->cursa.destinatie);
		free(de_sters->cursa.data);
		free(de_sters->cursa.ora);
		free(de_sters);
	}

}
void sortare_lista(Nod** lista) {
	unsigned char nr_el = 0;
	Nod* temp = *lista;
	while (temp != NULL) {
		nr_el++;
		temp = temp->next;
	}
	CursaAeriana* curse = (CursaAeriana*)malloc(nr_el * sizeof(CursaAeriana));
	temp = *lista;
	for (unsigned char i = 0; i < nr_el; i++) {
		curse[i].cod = (char*)malloc(strlen(temp->cursa.cod) + 1);
		strcpy(curse[i].cod, temp->cursa.cod);
		curse[i].destinatie = (char*)malloc(strlen(temp->cursa.destinatie) + 1);
		strcpy(curse[i].destinatie, temp->cursa.destinatie);
		curse[i].data = (char*)malloc(strlen(temp->cursa.data) + 1);
		strcpy(curse[i].data, temp->cursa.data);
		curse[i].ora = (char*)malloc(strlen(temp->cursa.ora) + 1);
		strcpy(curse[i].ora, temp->cursa.ora);
		temp = temp->next;
	}
	for (unsigned char i = 0; i < nr_el-1; i++) {
		for (unsigned char j = i + 1; j < nr_el; j++) {
			if (strcmp(curse[i].cod, curse[j].cod) > 0) {
				CursaAeriana aux = curse[i];
				curse[i] = curse[j];
				curse[j] = aux;
			}
		}
	}
	dezalocare_lista(*lista);
	*lista = NULL;
	for (unsigned char i = 0; i < nr_el; i++) {
		*lista = adauga_lista(*lista, curse[i]);
	}
	for (unsigned char i = 0; i < nr_el; i++) {
		free(curse[i].cod);
		free(curse[i].destinatie);
		free(curse[i].data);
		free(curse[i].ora);
	}
	free(curse);
}


int main() {
	FILE* f = fopen("Text.txt", "r");
	CursaAeriana cursa;
	Nod* lista = NULL;

	char buffer[256];
	char* token, separator[] = ",";
	if (f != NULL) {
		while (fgets(buffer, sizeof(buffer), f) != NULL) {
			token = strtok(buffer, separator);
			cursa.cod = (char*)malloc(strlen(token) + 1);
			strcpy(cursa.cod, token);
			token = strtok(NULL, separator);
			cursa.destinatie = (char*)malloc(strlen(token) + 1);
			strcpy(cursa.destinatie, token);
			token = strtok(NULL, separator);
			cursa.data = (char*)malloc(strlen(token) + 1);
			strcpy(cursa.data, token);
			token = strtok(NULL, separator);
			cursa.ora = (char*)malloc(strlen(token) + 1);
			strcpy(cursa.ora, token);
			token = strtok(NULL, separator);
			cursa.status = atoi(token);
			lista = adauga_lista(lista, cursa);
		}
		fclose(f);
	}
	printf("Traversare lista curse aeriene: \n");
	traversare_lista(lista);
	printf("\n");
	printf("Traversare lista curse aeriene dupa stergere:\n");
	stergere_zboruri(&lista, 0);
	traversare_lista(lista);
	printf("\n");
	/*printf("Traversare lista curse aeriene dupa sortare:\n");
	sortare_lista(&lista);
	traversare_lista(lista);*/
	dezalocare_lista(lista);

	return 0;
}