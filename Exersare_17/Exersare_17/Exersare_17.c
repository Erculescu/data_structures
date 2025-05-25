#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>

struct Masina {
	char vin[8];
	char* marca;
	char* model;
	int an_fabricatie;
	float putere;
	float pret;
};
typedef struct Masina Masina;

struct NodABC {
	Masina masina;
	struct NodABC* stanga, * dreapta;
};
typedef struct NodABC NodABC;

NodABC* inserare_Nod(NodABC* r, Masina data, unsigned char* flag) {
	if (r != NULL) {
		if (strcmp(data.vin, r->masina.vin) < 0) {
			r->stanga = inserare_Nod(r->stanga, data, flag);
		}
		else {
			if (strcmp(data.vin, r->masina.vin) > 0) {
				r->dreapta = inserare_Nod(r->dreapta, data, flag);
			}
			else {
				*flag = 0;
			}
		}
	}
	else {
		r = malloc(sizeof(NodABC));
		strcpy(r->masina.vin, data.vin);
		r->masina.marca = malloc(strlen(data.marca) + 1);
		strcpy(r->masina.marca, data.marca);
		r->masina.model = malloc(strlen(data.model) + 1);
		strcpy(r->masina.model, data.model);
		r->masina.an_fabricatie = data.an_fabricatie;
		r->masina.putere = data.putere;
		r->masina.pret = data.pret;
		r->stanga = NULL;
		r->dreapta = NULL;
	}
	return r;
}


void traversare_inordine(NodABC* r) {
	if (r != NULL) {
		traversare_inordine(r->stanga);
		printf("%s %s %s %d %.2f %.2f\n", r->masina.vin, r->masina.marca, r->masina.model, r->masina.an_fabricatie, r->masina.putere, r->masina.pret);
		traversare_inordine(r->dreapta);
	}
}

NodABC* dezalocare(NodABC* r) {
	if (r != NULL) {
		r->stanga = dezalocare(r->stanga);
		r->dreapta = dezalocare(r->dreapta);
		free(r->masina.marca);
		free(r->masina.model);
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

int noduri_pe_nivel(NodABC* r, int nivel) {
	if (r == NULL) {
		return 0;
	}
	if (nivel == 1) {
		return 1;
	}
	else {
		return noduri_pe_nivel(r->stanga, nivel - 1) + noduri_pe_nivel(r->dreapta, nivel - 1);
	}
}

NodABC* dezalocare_noduri_frunza(NodABC* r) {
	if (r != NULL) {
		if (r->stanga == NULL && r->dreapta == NULL) {
			free(r->masina.model);
			free(r->masina.marca);
			free(r);
			r = NULL;
		}
		else {
			r->stanga=dezalocare_noduri_frunza(r->stanga);
			r->dreapta=dezalocare_noduri_frunza(r->dreapta);
		}
	}
	return r;
}

NodABC* stergere_Nod(NodABC* r, char* vin, unsigned char* flag) {
	if (r != NULL) {
		if (strcmp(vin, r->masina.vin) < 0) {
			r->stanga = stergere_Nod(r->stanga, vin, flag);
		}
		else {
			if (strcmp(vin, r->masina.vin) > 0) {
				r->dreapta = stergere_Nod(r->dreapta, vin, flag);
			}
			else {
				*flag = 1;
				if (r->stanga == NULL && r->dreapta == NULL) {
					free(r->masina.marca);
					free(r->masina.model);
					free(r);
					r = NULL;
				}
				else {
					if (r->stanga != NULL && r->dreapta != NULL) {
						NodABC* succesor = r->dreapta;
						while (succesor->stanga != NULL) {
							succesor = succesor->stanga;
						}
						strcpy(succesor->masina.vin, r->masina.vin);
						succesor->masina.marca = malloc(strlen(r->masina.marca) + 1);
						strcpy(succesor->masina.marca, r->masina.marca);
						succesor->masina.model = malloc(strlen(r->masina.model) + 1);
						strcpy(succesor->masina.model, r->masina.model);
						succesor->masina.an_fabricatie = r->masina.an_fabricatie;
						succesor->masina.putere = r->masina.putere;
						succesor->masina.pret = r->masina.pret;
						free(r->masina.marca);
						free(r->masina.model);
						r->dreapta = stergere_Nod(r->dreapta, succesor->masina.vin, flag);
					}
					else {
						NodABC* temp = NULL;
						if (r->stanga != NULL) {
							temp = r->stanga;
						}
						else {
							temp = r->dreapta;
						}
						free(r->masina.marca);
						free(r->masina.model);
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

Masina* cautare_masina(NodABC* r, char* vin) {
	if (r != NULL) {
		if (strcmp(vin, r->masina.vin) < 0) {
			return cautare_masina(r->stanga, vin);
		}
		else {
			if (strcmp(vin, r->masina.vin) > 0) {
				return cautare_masina(r->dreapta, vin);
			}
			else {
				return &r->masina;
			}
		}
	}
	else {
		return NULL;
	}
}

int main() {
	FILE* f = fopen("masini.txt", "r");
	Masina masina;
	NodABC* root = NULL;

	char buffer[256], * token, separator[] = ",";
	if (f != NULL) {
		while (fgets(buffer, sizeof(buffer), f) != NULL) {
			token = strtok(buffer, separator);
			strcpy(masina.vin, token);
			token = strtok(NULL, separator);
			masina.marca = malloc(strlen(token) + 1);
			strcpy(masina.marca, token);
			token = strtok(NULL, separator);
			masina.model = malloc(strlen(token) + 1);
			strcpy(masina.model, token);
			token = strtok(NULL, separator);
			masina.an_fabricatie = atoi(token);
			token = strtok(NULL, separator);
			masina.putere = atof(token);
			token = strtok(NULL, separator);
			masina.pret = atof(token);
			unsigned char flag;
			root = inserare_Nod(root, masina, &flag);
			if (flag != 0) {
				printf("\nMasina inserata cu succes.\n");
			}
			else {
				printf("\nMasina nu a putut fi inserata.\n");
			}
		}
		fclose(f);
	}
	printf("\n\nTraversare in ordine arbore binar de cautare masini:\n");
	traversare_inordine(root);
	
	printf("\nInaltimea arborelui este: %d\n", inaltime(root));

	printf("Exista %d noduri pe nivelul 3.\n", noduri_pe_nivel(root, 3));

	Masina* cautata = cautare_masina(root, "1234567");
	if (cautata != NULL) {
		printf("\nMasina cautata cu vin 1234567:");
		printf("%s %s\n\n", cautata->marca, cautata->model);
	}
	else {
		printf("\nMasina cu VIN 1234567 nu exista in arbore.\n");
	}

	printf("\Arborele binar de cautare dupa dezalocarea nodurilor frunza:\n");
	root = dezalocare_noduri_frunza(root);
	traversare_inordine(root);

	unsigned char sters;
	printf("\n\nStergere nod cu VIN-ul 7654321\n");
	root = stergere_Nod(root, "7654321", &sters);
	if (sters != 0) {
		printf("Nodul cu VIN 7654321 a fost sters.\n");
		traversare_inordine(root);
	}
	else {
		printf("Nodul cu VIN 7654321 nu a putut fi sters.\n");
	}
}