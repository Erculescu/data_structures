#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

struct PackageDelivery {
	unsigned int bugid;
	char* reporterName;
	char* description;
	float severity;
	char resolved[2];
};
typedef struct PackageDelivery PackageDelivery;

struct NodABC {
	PackageDelivery pachet;
	struct NodABC* stanga;
	struct NodABC* dreapta;
};
typedef struct NodABC NodABC;

NodABC* inserare_Nod(NodABC* r, PackageDelivery data, unsigned char* flag) {
	if (r != NULL) {
		if (data.bugid < r->pachet.bugid) {
			r->stanga = inserare_Nod(r->stanga, data, flag);
		}
		else {
			if (data.bugid > r->pachet.bugid) {
				r->dreapta = inserare_Nod(r->dreapta, data, flag);
			}
			else {
				*flag = 0;
			}
		}
	}
	else {
		r = malloc(sizeof(NodABC));
		r->pachet = data;
		r->stanga = NULL;
		r->dreapta = NULL;
		*flag = 1;
	}
	return r;
}
void traversare_inordine(NodABC* r) {
	if (r != NULL) {
		traversare_inordine(r->stanga);
		printf("Bug ID: %u, Reporter: %s, Descriere: %s, Severitate: %.2f, Rezolvat?: %s\n", r->pachet.bugid, r->pachet.reporterName, r->pachet.description, r->pachet.severity, r->pachet.resolved);
		traversare_inordine(r->dreapta);
	}
}

NodABC* dezalocare_Arbore(NodABC* r) {
	if (r != NULL) {
		dezalocare_Arbore(r->stanga);
		dezalocare_Arbore(r->dreapta);
		free(r->pachet.reporterName);
		free(r->pachet.description);
		free(r);
		r = NULL;
	}
	return r;
}

int inaltime(NodABC* r) {
	if (r == NULL) {
		return -1;
	}
	else {
		int stanga = inaltime(r->stanga);
		int dreapta = inaltime(r->dreapta);
		if (stanga > dreapta) {
			return stanga + 1;
		}
		else {
			return dreapta + 1;
		}
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

NodABC* dezalocare_Noduri_frunza(NodABC* r) {
	if (r != NULL) {
		if (r->stanga == NULL && r->dreapta == NULL) {
			free(r->pachet.reporterName);
			free(r->pachet.description);
			free(r);
			r = NULL;
		}
		else {
			r->stanga = dezalocare_Noduri_frunza(r->stanga);
			r->dreapta = dezalocare_Noduri_frunza(r->dreapta);
		}
	}
	return r;
}


NodABC* stergere_Nod(NodABC* r, unsigned int cheie, unsigned char* flag) {
	if (r != NULL) {
		if (cheie < r->pachet.bugid) {
			r->stanga = stergere_Nod(r->stanga, cheie, flag);
		}
		else {
			if (cheie > r->pachet.bugid) {
				r->dreapta = stergere_Nod(r->dreapta, cheie, flag);
			}
			else {
				*flag = 1;
				if (r->stanga == NULL && r->dreapta == NULL) {
					free(r->pachet.reporterName);
					free(r->pachet.description);
					free(r);
					r = NULL;
				}
				else {
					if (r->stanga != NULL && r->dreapta != NULL) {
						NodABC* succesor = r->dreapta;
						while (succesor->stanga != NULL) {
							succesor = succesor->stanga;
						}
						succesor->pachet.bugid = r->pachet.bugid;
						succesor->pachet.reporterName = malloc(strlen(r->pachet.reporterName) + 1);
						strcpy(succesor->pachet.reporterName, r->pachet.reporterName);
						succesor->pachet.description = malloc(strlen(r->pachet.description) + 1);
						strcpy(succesor->pachet.description, r->pachet.description);
						succesor->pachet.severity = r->pachet.severity;
						strcpy(succesor->pachet.resolved, r->pachet.resolved);
						free(r->pachet.reporterName);
						free(r->pachet.description);
						r->dreapta = stergere_Nod(r->dreapta, succesor->pachet.bugid, flag);
					}
					else {
						NodABC* temp = NULL;
						if (r->stanga != NULL) {
							temp = r->stanga;
						}
						else {
							temp = r->dreapta;
						}
						free(r->pachet.reporterName);
						free(r->pachet.description);
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

PackageDelivery* cautare_pachet(NodABC* r, unsigned int cheie) {
	if (r != NULL) {
		if (cheie < r->pachet.bugid) {
			return cautare_pachet(r->stanga, cheie);
		}
		else {
			if (cheie > r->pachet.bugid) {
				return cautare_pachet(r->dreapta, cheie);
			}
			else {
				return &r->pachet;
			}
		}
	}
	else {
		return NULL;
	}
}

int main() {
	FILE* f = fopen("bugs.txt", "r");
	PackageDelivery pachet;
	NodABC* root = NULL;
	char buffer[256], separator[] = ",", * token;
	if (f != NULL) {
		while (fgets(buffer, sizeof(buffer), f) != NULL) {
			unsigned char flag = 0;
			token = strtok(buffer, separator);
			pachet.bugid = atoi(token);
			token = strtok(NULL, separator);
			pachet.reporterName = malloc(strlen(token) + 1);
			strcpy(pachet.reporterName, token);
			token = strtok(NULL, separator);
			pachet.description = malloc(strlen(token) + 1);
			strcpy(pachet.description, token);
			token = strtok(NULL, separator);
			pachet.severity = atof(token);
			token = strtok(NULL, separator);
			strcpy(pachet.resolved, token);
			root = inserare_Nod(root, pachet, &flag);
			if (flag != 0) {
				printf("Bug ID %u inserat cu succes.\n", pachet.bugid);
			}
			else {
				printf("Inserare esuata pentru Bug ID %u\n", pachet.bugid);
			}
		}
		fclose(f);
	}
	printf("\nTraversare in ordine a arborelui:\n");
	traversare_inordine(root);

	int inaltimeArbore = inaltime(root);
	printf("\nInaltimea arborelui este: %d\n", inaltimeArbore);

	printf("\nNumarul de noduri pe nivelul 4 este: %d\n", noduri_pe_nivel(root, 4));

	root = dezalocare_Noduri_frunza(root);
	printf("\nParcurgere arbore dupa dezalocarea nodurilor frunza:\n");
	traversare_inordine(root);

	unsigned char flag = 0;
	stergere_Nod(root, 3, &flag);
	if (flag != 0) {
		printf("\nNodul cu Bug ID 3 a fost sters.\n");
		traversare_inordine(root);
	}
	else {
		printf("\nNu s-a putut sterge nodul cu Bug ID 3.\n");
	}
	PackageDelivery* cautat = cautare_pachet(root, 2);
	if (cautat != NULL) {
		printf("\nPachetul cu Bug ID 2 a fost gasit: %s\n", cautat->description);
	}
	else {
		printf("\nPachetul cu Bug ID 2 nu a fost gasit.\n");
	}

	root = dezalocare_Arbore(root);
	printf("\nParcurgere a arborelui dupa dezalocare:\n");
	traversare_inordine(root);
}