#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<string.H>
#include<malloc.h>

struct Produs
{
	int id;
	char* denumire;
	float pret;
	int cantitate;
};
typedef struct Produs Produs;

struct NodABC {
	Produs produs;
	struct NodABC* stanga, *dreapta;
};
typedef struct NodABC NodABC;

NodABC* inserare_nod(NodABC* r, Produs data, unsigned char* flag) {
	if (r != NULL) {
		if (data.id < r->produs.id) {
			r->stanga = inserare_nod(r->stanga, data, flag);
		}
		else {
			if (data.id > r->produs.id) {
				r->dreapta = inserare_nod(r->dreapta, data, flag);
			}
			else {
				*flag = 0;
			}
		}
	}
	else {
		r = malloc(sizeof(NodABC));
		r->produs = data;
		r->stanga = NULL;
		r->dreapta = NULL;
	}
	return r;
}

void traversare_inordine(NodABC* r) {
	if (r != NULL) {
		traversare_inordine(r->stanga);
		printf("%d %s %.2f %d\n", r->produs.id, r->produs.denumire, r->produs.pret, r->produs.cantitate);
		traversare_inordine(r->dreapta);
	}
}

NodABC* dezalocare(NodABC* r) {
	if (r != NULL) {
		r->stanga = dezalocare(r->stanga);
		r->dreapta = dezalocare(r->dreapta);
		free(r->produs.denumire);
		free(r);
		r = NULL;
	}
	return r;
}

NodABC* dezalocare_noduri_frunza(NodABC* r) {
	if (r != NULL) {
		if (r->stanga == NULL && r->dreapta == NULL) {
			free(r->produs.denumire);
			free(r);
			r=NULL;
		}
		else {
			r->stanga = dezalocare_noduri_frunza(r->stanga);
			r->dreapta = dezalocare_noduri_frunza(r->dreapta);
		}
	}
	return r;
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

int nr_noduri_pe_nivel(NodABC* r, int nivel) {
	if (r == NULL) {
		return 0;
	}
	if (nivel == 1) {
		return 1;
	}
	else {
		return nr_noduri_pe_nivel(r->stanga, nivel - 1) + nr_noduri_pe_nivel(r->dreapta, nivel - 1);
	}
}

NodABC* stergere_Nod(NodABC* r, int id, unsigned char* flag) {
	if (r != NULL) {
		if (id < r->produs.id) {
			r->stanga = stergere_Nod(r->stanga, id, flag);
		}
		else {
			if (id > r->produs.id) {
				r->dreapta = stergere_Nod(r->dreapta, id, flag);
			}
			else {
				*flag = 1;
				if (r->stanga == NULL && r->dreapta == NULL) {
					free(r->produs.denumire);
					free(r);
					r = NULL;
				}
				else {
					if (r->stanga != NULL && r->dreapta != NULL) {
						NodABC* succesor = r->dreapta;
						while (succesor->stanga != NULL) {
							succesor = succesor->stanga;
						}
						succesor->produs.id = r->produs.id;
						succesor->produs.denumire = malloc(strlen(r->produs.denumire) + 1);
						strcpy(succesor->produs.denumire, r->produs.denumire);
						succesor->produs.pret = r->produs.pret;
						succesor->produs.cantitate = r->produs.cantitate;
						free(r->produs.denumire);
						r->dreapta = stergere_Nod(r->dreapta, succesor->produs.id, flag);
					}else{
						NodABC* temp = NULL;
						if (r->stanga != NULL) {
							temp = r->stanga;
						}
						else {
							temp = r->dreapta;
						}
						free(r->produs.denumire);
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

Produs* cautare_produs(NodABC* r, int id) {
	if (r != NULL) {
		if (id < r->produs.id) {
			return cautare_produs(r->stanga, id);
		}
		else {
			if (id > r->produs.id) {
				return cautare_produs(r->dreapta, id);
			}
			else {
				return &r->produs;
			}
		}
	}
	else {
		return NULL;
	}
}

int main() {
	FILE* f = fopen("produse.txt", "r");
	Produs produs;
	NodABC* root = NULL;
	char buffer[256], * token, separator[] = ",";
	if (f != NULL) {
		while (fgets(buffer, sizeof(buffer), f) != NULL) {
			token = strtok(buffer, separator);
			produs.id = atoi(token);
			token = strtok(NULL, separator);
			produs.denumire = malloc(strlen(token) + 1);
			strcpy(produs.denumire, token);
			token = strtok(NULL, separator);
			produs.pret = atof(token);
			token = strtok(NULL, separator);
			produs.cantitate = atoi(token);
			unsigned char flag=1;
			root = inserare_nod(root, produs, &flag);
			if (flag == 1) {
				printf("\nProdus inserat cu succes.\n");
			}
			else {
				printf("\nProdusul nu a putut fi inserat.\n");
			}
		}
		fclose(f);
	}
	printf("\nTraversare in ordine:\n");
	traversare_inordine(root);

	printf("\nInaltimea arborelui: %d\n", inaltime(root));

	printf("\nNumarul de noduri de pe nivelul 5: %d\n", nr_noduri_pe_nivel(root, 4));

	printf("\nDupa dezalocarea frunzelor:\n");
	root = dezalocare_noduri_frunza(root);
	traversare_inordine(root);

	unsigned char flag;
	root = stergere_Nod(root, 3, &flag);
	if (flag == 1) {
		printf("\nNodul a fost sters cu succes.\n");
		traversare_inordine(root);
	}
	else {
		printf("\nNodul nu a putut fi sters.\n");
	}

	Produs* cautat = cautare_produs(root, 2);
	if (cautat != NULL) {
		printf("\nProdusul cautat: %d %s\n", cautat->id, cautat->denumire);

	}
	else {
		printf("\nProdusul nu a fost gasit.\n");
	}

	root = dezalocare(root);
	printf("\nArborele dupa dezalocare:\n");
	traversare_inordine(root);
}