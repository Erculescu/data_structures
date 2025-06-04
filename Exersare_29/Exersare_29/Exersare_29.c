#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>

struct Camera {
	char Serie[10];
	char* Marca;
	char* Model;
	float Pret;
};
typedef struct Camera Camera;
struct NodD {
	Camera info;
	struct NodD* next, * prev;
};
typedef struct NodD NodD;

struct ListaDubla {
	NodD* prim, * ultim;
};
typedef struct ListaDubla ListaDubla;

ListaDubla inserare_nod_sfarsit(ListaDubla lista, Camera info) {
	NodD* nou = malloc(sizeof(NodD));
	nou->info = info;
	nou->next = NULL;
	nou->prev = lista.ultim;
	if (lista.prim == NULL) {
		lista.prim = nou;
	}
	else {
		lista.ultim->next = nou;
	}
	lista.ultim = nou;
	return lista;
}

void traversare_lista(ListaDubla lista) {
	printf("\nTraversare normala:\n");
	NodD* p = lista.prim;
	while (p != NULL) {
		printf("Serie:%s, Marca: %s, Model: %s, Pret: %.2f\n", p->info.Serie, p->info.Marca, p->info.Model, p->info.Pret);
		p = p->next;
	}
	printf("\nTraversare inversa:\n");
	p = lista.ultim;
	while (p != NULL) {
		printf("Serie:%s, Marca: %s, Model: %s, Pret: %.2f\n", p->info.Serie, p->info.Marca, p->info.Model, p->info.Pret);
		p = p->prev;
	}
}

void Lista_Swap(ListaDubla *lista, char* serie1, char* serie2) {
	if (lista == NULL || serie1 == NULL || serie2 == NULL || strcmp(serie1, serie2) == 0)return;
	NodD* nod1 = NULL, * nod2 = NULL, * p = lista->prim;
	while (p) {
		if (strcmp(p->info.Serie, serie1) == 0) {
			nod1 = p;
		}
		if (strcmp(p->info.Serie, serie2) == 0) {
			nod2 = p;
		}
		p = p->next;
	}
	if (nod1->next == nod2) {

		if (nod1->prev) nod1->prev->next = nod2;
		if (nod2->next) nod2->next->prev = nod1;
		nod1->next = nod2->next;
		nod2->prev = nod1->prev;
		nod2->next = nod1;
		nod1->prev = nod2;
	}
	else if (nod2->next == nod1) {

		if (nod2->prev) nod2->prev->next = nod1;
		if (nod1->next) nod1->next->prev = nod2;
		nod2->next = nod1->next;
		nod1->prev = nod2->prev;
		nod1->next = nod2;
		nod2->prev = nod1;
	}
	else {

		NodD* tempPrev = nod1->prev;
		NodD* tempNext = nod1->next;

		if (nod1->prev) nod1->prev->next = nod2;
		if (nod1->next) nod1->next->prev = nod2;
		if (nod2->prev) nod2->prev->next = nod1;
		if (nod2->next) nod2->next->prev = nod1;

		nod1->prev = nod2->prev;
		nod1->next = nod2->next;
		nod2->prev = tempPrev;
		nod2->next = tempNext;
	}
	if (lista->prim == nod1) {
		lista->prim = nod2;
	}
	else if (lista->prim == nod2) {
		lista->prim = nod1;
	}
	if (lista->ultim == nod1) {
		lista->ultim = nod2;
	}
	else if (lista->ultim == nod2) {
		lista->ultim = nod2;
	}
}


void dezalocareLista(ListaDubla lista) {
	NodD* p = lista.prim;
	while (p != NULL) {
		NodD* temp = p;
		p = p->next;
		free(temp->info.Marca);
		free(temp->info.Model);
		free(temp);
	}
	lista.prim = NULL;
	lista.ultim = NULL;
}

ListaDubla stergere_nod(ListaDubla lista, char* serie) {
	if (serie == NULL) {
		return lista;
	}
	NodD* p = lista.prim;
	while (p != NULL) {
		if (strcmp(p->info.Serie, serie) == 0) {
			if (p->prev != NULL) {
				p->prev->next = p->next;
			}
			else {
				lista.prim = p->next;
			}
			if (p->next) {
				p->next->prev = p->prev;
			}
			else {
				lista.ultim = p->prev;
			}
			free(p->info.Marca);
			free(p->info.Model);
			free(p);
			break;
		}
		p = p->next;
	}
	return lista;
}

int main() {
	FILE* f = fopen("camere.txt", "r");
	Camera cam;
	char buffer[256], * token, separator[] = ",";
	ListaDubla lista;
	lista.prim = NULL;
	lista.ultim = NULL;
	if (f != NULL) {
		while (fgets(buffer, sizeof(buffer), f)) {
			token = strtok(buffer, separator);
			strcpy(cam.Serie, token);
			token = strtok(NULL, separator);
			cam.Marca = malloc(strlen(token) + 1);
			strcpy(cam.Marca, token);
			token = strtok(NULL, separator);
			cam.Model = malloc(strlen(token) + 1);
			strcpy(cam.Model, token);
			token = strtok(NULL, separator);
			cam.Pret = atof(token);
			lista = inserare_nod_sfarsit(lista, cam);
		}
		fclose(f);
	}
	traversare_lista(lista);
	printf("\n\n\n");
	Lista_Swap(&lista, "minolta21", "nikonf608");
	traversare_lista(lista);
	lista = stergere_nod(lista, "b35rollei");
	printf("\n\n\n");
	traversare_lista(lista);
	dezalocareLista(lista);
}