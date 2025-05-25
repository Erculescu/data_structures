#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>

struct Carte {
	char ISBN[10];
	char* titlu;
	char* autor;
	int nrPagini;
	float pret;
};
typedef struct Carte Carte;

struct NodABC {
	Carte carte;
	struct NodABC* stanga, *dreapta;
};
typedef struct NodABC NodABC;

NodABC* inserare_nod(NodABC* r, Carte data, unsigned char* flag) {
	if (r != NULL) {
		if (strcmp(data.ISBN, r->carte.ISBN) < 0) {
			r->stanga = inserare_nod(r->stanga, data, flag);
		}
		else {
			if (strcmp(data.ISBN, r->carte.ISBN) > 0) {
				r->dreapta = inserare_nod(r->dreapta, data, flag);
			}
			else {
				*flag = 0;
			}
		}
	}
	else {
		r = malloc(sizeof(NodABC));
		r->carte = data;
		r->stanga = NULL;
		r->dreapta = NULL;
	}
	return r;
}

void traversare_inordine(NodABC* r) {
	if (r != NULL) {
		traversare_inordine(r->stanga);
		printf("%s %s %s %d %.2f\n", r->carte.ISBN, r->carte.titlu, r->carte.autor, r->carte.nrPagini, r->carte.pret);
		traversare_inordine(r->dreapta);
	}

}

NodABC* dezalocare(NodABC* r) {
	if (r != NULL) {
		r->stanga = dezalocare(r->stanga);
		r->dreapta = dezalocare(r->dreapta);
		free(r->carte.titlu);
		free(r->carte.autor);
		free(r);
		r = NULL;
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

NodABC* dezalocare_noduri_frunza(NodABC* r) {
	if (r != NULL) {
		if (r->stanga == NULL && r->dreapta == NULL) {
			free(r->carte.titlu);
			free(r->carte.autor);
			free(r);
			r = NULL;
		}
		else {
			r->stanga = dezalocare_noduri_frunza(r->stanga);
			r->dreapta = dezalocare_noduri_frunza(r->dreapta);
		}

	}
	return r;
}

NodABC* stergere_nod(NodABC* r, char* cheie, unsigned char* flag) {
	if (r != NULL) {
		if (strcmp(cheie, r->carte.ISBN) < 0) {
			r->stanga = stergere_nod(r->stanga, cheie, flag);
		}
		else {
			if (strcmp(cheie, r->carte.ISBN) > 0) {
				r->dreapta = stergere_nod(r->dreapta, cheie, flag);
			}
			else {
				*flag = 1;
				if (r->stanga == NULL && r->dreapta == NULL) {
					free(r->carte.titlu);
					free(r->carte.autor);
					free(r);
					r = NULL;
				}
				else {
					if (r->stanga != NULL && r->dreapta != NULL) {
						NodABC* succesor = r->dreapta;
						while (succesor->stanga != NULL) {
							succesor = succesor->stanga;
						}
						strcpy(succesor->carte.ISBN, r->carte.ISBN);
						succesor->carte.titlu = malloc(strlen(r->carte.titlu) + 1);
						strcpy(succesor->carte.titlu, r->carte.titlu);
						succesor->carte.autor = malloc(strlen(r->carte.autor) + 1);
						strcpy(succesor->carte.autor, r->carte.autor);
						succesor->carte.nrPagini = r->carte.nrPagini;
						succesor->carte.pret = r->carte.pret;
						free(r->carte.titlu);
						free(r->carte.autor);
						r->dreapta = stergere_nod(r->dreapta, succesor->carte.ISBN, flag);
					}
					else {
						NodABC* temp = NULL;
						if (r->stanga != NULL) {
							temp = r->stanga;
						}
						else { temp = r->dreapta; }
						free(r->carte.titlu);
						free(r->carte.autor);
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

int main() {
	FILE* f = fopen("carti.txt", "r");
	Carte c;
	NodABC* root = NULL;
	char buffer[256], * token, separator[] = ",";
	if (f != NULL) {
		while (fgets(buffer, sizeof(buffer), f) != NULL) {
			token = strtok(buffer, separator);
			strcpy(c.ISBN, token);
			token = strtok(NULL, separator);
			c.titlu = malloc(strlen(token) + 1);
			strcpy(c.titlu, token);
			token = strtok(NULL, separator);
			c.autor = malloc(strlen(token) + 1);
			strcpy(c.autor, token);
			token = strtok(NULL, separator);
			c.nrPagini = atoi(token);
			token = strtok(NULL, separator);
			c.pret = atof(token);
			unsigned char flag = 1;
			root = inserare_nod(root, c, &flag);
			if (flag == 1) {
				printf("\nInserare reusita.\n");
			}
			else { printf("\nNu s-a putut face inserarea.\n"); }
		}
		fclose(f);
	}
	printf("\n\nTraversare in ordine:\n");
	traversare_inordine(root);

	printf("\nInaltimea arborelui este: %d\n", inaltime(root));

	printf("\nExista %d noduri la nivelul 3\n", nr_noduri_nivel(root, 3));

	unsigned char flag;
	root = stergere_nod(root, "010294325", &flag);
	if (flag == 1) {
		printf("\nStergere reusita.\n");
		traversare_inordine(root);
	}
	else {
		printf("\nStergere esuata.\n");
	}
	root = dezalocare_noduri_frunza(root);
	printf("\nArborele dupa dezalocarea frunzelor:\n");
	traversare_inordine(root);
	root = dezalocare(root);
	printf("\nArborele dupa dezalocare:\n");
	traversare_inordine(root);

}