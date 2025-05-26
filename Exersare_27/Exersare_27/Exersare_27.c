#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>

struct Comanda {
	int id;
	char* client;
	char* produs;
	int dioptrie;
	float pret;
};
typedef struct Comanda Comanda;
struct NodABC {
	Comanda info;
	struct NodABC* stanga, * dreapta;
};
typedef struct NodABC NodABC;

struct Inventar {
	int dioptrie;
	int* comenzi;
};
typedef struct Inventar Inventar;

NodABC* inserare_nod(NodABC* r, Comanda data, unsigned char* flag) {
	if (r != NULL) {
		if (data.id < r->info.id) {
			r->stanga = inserare_nod(r->stanga, data, flag);
		}
		else {
			if (data.id > r->info.id) {
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
		printf("%d %s %s %d %.2f\n", r->info.id, r->info.client, r->info.produs, r->info.dioptrie, r->info.pret);
		traversare_preordine(r->stanga);
		traversare_preordine(r->dreapta);
	}
}

void interschimbare(NodABC* r, int cod1, int cod2) {
	NodABC* nod1 = NULL, * nod2 = NULL, * temp = r;
	if (r != NULL) {
		while (temp != NULL) {
			if (temp->info.id == cod1) {
				nod1 = temp;
			}
			if (temp->info.id == cod2) {
				nod2 = temp;
			}
			if (nod1 != NULL && nod2 != NULL) {
				break;
			}
			if (cod1 < temp->info.id || cod2 < temp->info.id) {
				temp = temp->stanga;
			}
			else {
				temp = temp->dreapta;
			}
		}
	}
	else { return; }
	if (nod1 != NULL && nod2 != NULL) {
		Comanda aux = nod1->info;
		nod1->info = nod2->info;
		nod2->info = aux;

	}
	else {
		printf("Nu s-au gasit destule noduri pentru interschimbare.\n");
	}
}

int numara_noduri(NodABC* r, int dioptrie) {
	if (r == NULL) {
		return 0;
	}
	int contor = 0;
	if (r->info.dioptrie == dioptrie) {
		contor = 1;
	}
	contor += numara_noduri(r->stanga, dioptrie);
	contor += numara_noduri(r->dreapta, dioptrie);
	return contor;
}

void calculeaza_coduri(NodABC* r, Inventar* inv, int dioptrie, int* index) {
	if (r == NULL) {
		return;
	}
	if (r->info.dioptrie == dioptrie) {
		inv->comenzi[*index] = r->info.id;
		*index += 1;
	}
	calculeaza_coduri(r->stanga, inv, dioptrie, index);
	calculeaza_coduri(r->dreapta, inv, dioptrie, index);

}

Inventar* creeaza_inventar(NodABC* r, int dioptrie,Inventar* inv) {
	int nr_elem = numara_noduri(r, dioptrie);
	if (nr_elem == 0) { return NULL; }
	inv = malloc(sizeof(Inventar));
	inv->dioptrie = dioptrie;
	inv->comenzi = malloc(nr_elem * sizeof(int));
	int index = 0;
	calculeaza_coduri(r, inv, dioptrie, &index);
	return inv;
}


int inaltime(NodABC* r) {
	if (r == NULL) {
		return 0;
	}
	int stanga = inaltime(r->stanga), dreapta = inaltime(r->dreapta);
	if (stanga > dreapta) {
		return stanga + 1;
	}
	else {
		return dreapta + 1;
	}
}
int nr_noduri_nivel(NodABC* r, int nivel) {
	if (r == NULL) {
		return 0;
	}
	if (nivel == 1) {
		return 1;
	}
	else {
		return nr_noduri_nivel(r->stanga, nivel - 1) + nr_noduri_nivel(r->dreapta, nivel - 1);
	}
}

NodABC* dezalocare_frunze(NodABC* r) {
	if (r != NULL) {
		if (r->stanga == NULL && r->dreapta == NULL) {
			free(r->info.client);
			free(r->info.produs);
			free(r);
			r = NULL;
		}
		else {
			r->stanga=dezalocare_frunze(r->stanga);
			r->dreapta=dezalocare_frunze(r->dreapta);
		}
	}
	return r;
}

NodABC* stergere_nod(NodABC* r, int id, unsigned char* flag) {
	if (r != NULL) {
		if (id < r->info.id) {
			r->stanga = stergere_nod(r->stanga, id, flag);
		}
		else {
			if (id > r->info.id) {
				r->dreapta = stergere_nod(r->dreapta, id, flag);
			}
			else {
				*flag = 1;
				if (r->stanga == NULL && r->dreapta == NULL) {
					free(r->info.client);
					free(r->info.produs);
					free(r);
					r = NULL;
				}
				else {
					if (r->stanga != NULL && r->dreapta != NULL) {
						NodABC* succesor = r->dreapta;
						while (succesor->stanga != NULL) {
							succesor = succesor->stanga;
						}
						succesor->info.id = r->info.id;
						succesor->info.client = malloc(strlen(r->info.client) + 1);
						strcpy(succesor->info.client, r->info.client);
						succesor->info.produs = malloc(strlen(r->info.produs) + 1);
						strcpy(succesor->info.produs, r->info.produs);
						succesor->info.dioptrie = r->info.dioptrie;
						succesor->info.pret = r->info.pret;
						free(r->info.client);
						free(r->info.produs);
						r->dreapta = stergere_nod(r->dreapta, succesor->info.id, flag);
					}
					else {
						NodABC* temp = NULL;
						if (r->stanga != NULL) {
							temp = r->stanga;
						}
						else {
							temp = r->dreapta;
						}
						free(r->info.client);
						free(r->info.produs);
						free(r);
						r = temp;

					}
				}
			}
		}
	}
	else {
		*flag = 0;
	}
	return r;
}
NodABC* dezalocare(NodABC* r) {
	if (r != NULL) {
		r->stanga = dezalocare(r->stanga);
		r->dreapta = dezalocare(r->dreapta);
		free(r->info.client);
		free(r->info.produs);
		free(r);
		r = NULL;
	}
	return r;
}

int main() {
	FILE* f = fopen("optica.txt", "r");
	Comanda comanda;
	NodABC* root = NULL;
	char buffer[256], * token, separator[] = ",";
	unsigned char flag;
	if (f != NULL) {
		while (fgets(buffer, sizeof(buffer), f) != NULL) {
			token = strtok(buffer, separator);
			comanda.id = atoi(token);
			token = strtok(NULL, separator);
			comanda.client = malloc(strlen(token) + 1);
			strcpy(comanda.client, token);
			token = strtok(NULL, separator);
			comanda.produs = malloc(strlen(token) + 1);
			strcpy(comanda.produs, token);
			token = strtok(NULL, separator);
			comanda.dioptrie = atoi(token);
			token = strtok(NULL, separator);
			comanda.pret = atof(token);
			root = inserare_nod(root, comanda, &flag);
			if (flag == 1) {
				printf("\nComanda inserata cu succes.\n");
			}
			else {
				printf("\nComanda inserata deja.\n");
			}

		}
		fclose(f);
	}
	printf("\nTraversare preordine:\n");
	traversare_preordine(root);

	interschimbare(root, 2, 3);
	printf("\nTraversare preordine dupa interschimbare:\n");
	traversare_preordine(root);
	Inventar* inv = NULL;
	int dioptrie = -1;
	inv = creeaza_inventar(root, dioptrie, inv);
	for (int i = 0; i < numara_noduri(root, dioptrie); i++) {
		printf("\nComanda cu dioptrie %d: %d\n", dioptrie, inv->comenzi[i]);
	}
	printf("\nInaltimea arborelui: %d\n", inaltime(root));

	printf("\nNoduri la nivelul 2: %d\n", nr_noduri_nivel(root, 2));
	flag = 0;
	root = stergere_nod(root, 3, &flag);
	if (flag == 1) {
		traversare_preordine(root);
	}
	printf("\nDupa dezalocare:\n");
	root=dezalocare(root);
	traversare_preordine(root);
}