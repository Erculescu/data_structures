#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>

struct Electro {
	unsigned short int cod;
	char* marca;
	char* model;
	char* tip;
	float pret;

};
typedef struct Electro Electro;
struct NodABC {
	Electro info;
	struct NodABC* stanga, * dreapta;
};
typedef struct NodABC NodABC;

struct Inventar {
	unsigned short int* coduri;
};
typedef struct Inventar Inventar;

NodABC* inserare_nod(NodABC* r, Electro data, unsigned char* flag) {
	if (r != NULL) {
		if (r->info.cod > data.cod) {
			r->stanga = inserare_nod(r->stanga, data, flag);
		}
		else {
			if (data.cod > r->info.cod) {
				r->dreapta = inserare_nod(r->dreapta, data, flag);
			}
			else {
				*flag = 0;
			}
		}
	}
	else {
		r = malloc(sizeof(NodABC));
		r->info = data;
		r->stanga = NULL;
		r->dreapta = NULL;
		*flag = 1;
	}
	return r;
}

void traversare_preordine(NodABC* r) {
	if (r != NULL) {
		printf("%d %s %s %s %.2f\n", r->info.cod, r->info.marca, r->info.model, r->info.tip, r->info.pret);
		traversare_preordine(r->stanga);
		traversare_preordine(r->dreapta);
	}
}

NodABC* dezalocare(NodABC* r) {
	if (r != NULL) {
		r->stanga = dezalocare(r->stanga);
		r->dreapta = dezalocare(r->dreapta);
		free(r->info.marca);
		free(r->info.model);
		free(r->info.tip);
		free(r);
		r = NULL;
	}
	return r;
}

void interschimbare(NodABC* r, unsigned short int cod1, unsigned short int cod2) {
	NodABC* nod1 = NULL, * nod2 = NULL, * temp = r;
	if(r!=NULL){
		
		while (temp != NULL) {
			if (temp->info.cod == cod1) {
				nod1 = temp;
			}
			if (temp->info.cod == cod2) {
				nod2 = temp;
			}
			if (nod1 != NULL && nod2 != NULL) {
				break;
			}
			if (cod1 < temp->info.cod || cod2 < temp->info.cod) {
				temp = temp->stanga;
			}
			else {
				temp = temp->dreapta;
			}
		}
	}
	else {
		return;
	}
	if (nod1 != NULL && nod2 != NULL) {
		Electro aux = nod1->info;
		nod1->info = nod2->info;
		nod2->info = aux;

	}
	else { printf("\nUnul sau ambele noduri nu exista!.\n"); }
}

int numara_noduri_marca(NodABC* r, char* marca) {
	if (r == NULL) {
		return 0;
	}
	int contor = 0;
	if (strcmp(r->info.marca, marca) == 0) {
		contor = 1;
	}
	contor += numara_noduri_marca(r->stanga, marca);
	contor += numara_noduri_marca(r->dreapta, marca);
	return contor;
}

void calculeaza_coduri(NodABC* nod, Inventar* inv, char* marca, int* index) {
	if (nod == NULL) {
		return;
	}
	if (strcmp(nod->info.marca, marca) == 0) {
		inv->coduri[*index] = nod->info.cod;
		(*index)++;
	}
	calculeaza_coduri(nod->stanga, inv, marca, index);
	calculeaza_coduri(nod->dreapta, inv, marca, index);
}
Inventar* vector_coduri(NodABC* r, Inventar* inv, char* marca) {
	int nr_elem = numara_noduri_marca(r, marca);
	if (nr_elem == 0) {
		return NULL;
	}
	inv = malloc(sizeof(Inventar));
	inv->coduri = malloc(nr_elem * sizeof(unsigned short int));
	NodABC* temp = r;
	int index = 0;
	
	calculeaza_coduri(r, inv, marca, &index);
	return inv;
}

int main() {
	FILE* f = fopen("electro.txt", "r");
	Electro data;
	NodABC* root = NULL;
	unsigned char flag;
	char buffer[256], * token, separator[] = ",";
	if (f != NULL) {
		while (fgets(buffer, sizeof(buffer), f) != NULL) {
			token = strtok(buffer, separator);
			data.cod = (unsigned short int)atoi(token);
			token = strtok(NULL, separator);
			data.marca = malloc(strlen(token) + 1);
			strcpy(data.marca, token);
			token = strtok(NULL, separator);
			data.model = malloc(strlen(token) + 1);
			strcpy(data.model, token);
			token = strtok(NULL, separator);
			data.tip = malloc(strlen(token) + 1);
			strcpy(data.tip, token);
			token = strtok(NULL, separator);
			data.pret = (float)atof(token);
			root = inserare_nod(root, data, &flag);
			if (flag == 1) {
				printf("\nNod inserat.\n");
			}
			else {
				printf("\nNodul nu a fost inserat.\n");
			}
		}
		fclose(f);
	}
	printf("\nTraversare preordine:\n");
	traversare_preordine(root);

	printf("\nInterschimbare noduri:\n");
	interschimbare(root, 2, 3);
	traversare_preordine(root);

	Inventar* inv = NULL;
	inv = vector_coduri(root, inv, "Apple");
	for (int i = 0; i < numara_noduri_marca(root, "Apple"); i++) {
		printf("%d ", inv->coduri[i]);
	}
	free(inv->coduri);
	free(inv);
	root=dezalocare(root);
	free(root);
	return 0;
}