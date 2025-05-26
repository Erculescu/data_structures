#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>

struct Rezervare {
	unsigned int id;
	char* nume_hotel;
	unsigned char nr_camere_rezervate;
	char* nume_client;
	float pret_total;
};
typedef struct Rezervare Rezervare;
struct NodABC {
	Rezervare rezervare;
	struct NodABC* stanga, * dreapta;
};
typedef struct NodABC NodABC;

NodABC* inserare_nod(NodABC* r, Rezervare data, unsigned char* flag) {
	if (r != NULL) {
		if (data.id < r->rezervare.id) {
			r->stanga = inserare_nod(r->stanga, data, flag);
		}
		else {
			if (data.id > r->rezervare.id) {
				r->dreapta = inserare_nod(r->dreapta, data, flag);
			}
			else {
				*flag = 0;
			}
		}
	}
	else {
		r = malloc(sizeof(NodABC));
		r->rezervare = data;
		r->stanga = NULL;
		r->dreapta = NULL;
		*flag = 1;
	}
	return r;
}

void traversare_inordine(NodABC* r) {
	if (r != NULL) {
		traversare_inordine(r->stanga);
		printf("%u %s %u %s %.f\n", r->rezervare.id, r->rezervare.nume_hotel, r->rezervare.nr_camere_rezervate, r->rezervare.nume_client, r->rezervare.pret_total);
		traversare_inordine(r->dreapta);
	}
}

int nr_camere_rezervate(NodABC* r) {
	if (r != NULL) {
		if (r->stanga == NULL && r->dreapta == NULL) {
			return r->rezervare.nr_camere_rezervate;
		}
		else {
			return nr_camere_rezervate(r->stanga) + nr_camere_rezervate(r->dreapta);
		}
	}
	else {
		return 0;
	}
}

int nr_niveluri(NodABC* r) {
	if (r == NULL) {
		return 0;
	}
	
		int stanga = nr_niveluri(r->stanga), dreapta = nr_niveluri(r->dreapta);
		if (stanga > dreapta) {
			return stanga + 1;
		}
		else {
			return dreapta + 1;
		}
	
}

NodABC* stergere_nod(NodABC* r, unsigned int id, unsigned char* flag) {
	if (r != NULL) {
		if (id < r->rezervare.id) {
			r->stanga = stergere_nod(r->stanga, id, flag);
		}
		else {
			if (id > r->rezervare.id) {
				r->dreapta = stergere_nod(r->dreapta, id, flag);
			}
			else {
				*flag = 1;
				if (r->stanga == NULL && r->dreapta == NULL) {
					free(r->rezervare.nume_hotel);
					free(r->rezervare.nume_client);
					free(r);
					r = NULL;

				}
				else {
					if (r->stanga != NULL && r->dreapta != NULL) {
						NodABC* succesor = r->dreapta;
						while (succesor->stanga != NULL) {
							succesor = succesor->stanga;
						}
						succesor->rezervare.id = r->rezervare.id;
						succesor->rezervare.nume_hotel = malloc(strlen(r->rezervare.nume_hotel) + 1);
						strcpy(succesor->rezervare.nume_hotel, r->rezervare.nume_hotel);
						succesor->rezervare.nr_camere_rezervate = r->rezervare.nr_camere_rezervate;
						succesor->rezervare.nume_client = malloc(strlen(r->rezervare.nume_client) + 1);
						strcpy(succesor->rezervare.nume_client, r->rezervare.nume_client);
						succesor->rezervare.pret_total = r->rezervare.pret_total;
						free(r->rezervare.nume_hotel);
						free(r->rezervare.nume_client);
						r->dreapta = stergere_nod(r->dreapta, succesor->rezervare.id, flag);
					}
					else {
						NodABC* temp = NULL;
						if (r->stanga != NULL) {
							temp = r->stanga;
						}
						else {
							temp = r->dreapta;
						}
						free(r->rezervare.nume_hotel);
						free(r->rezervare.nume_client);
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
		free(r->rezervare.nume_hotel);
		free(r->rezervare.nume_client);
		free(r);
		r = NULL;
	}
	return r;
}

int* drum(NodABC* r, unsigned int id,int* marime) {
	int* drum = malloc(100 * sizeof(int));
	if (!drum) {
		printf("\nEroare la alocarea memoriei pentru drum.\n");
		return NULL;
	}
	*marime = 0;
	NodABC* curent = r;
	while (curent != NULL) {
		drum[*marime] = curent->rezervare.id;
		*marime += 1;
		if (id == curent->rezervare.id) {
			return drum;
		}
		if (id < curent->rezervare.id) {
			curent = curent->stanga;
		}
		else {
			curent = curent->dreapta;
		}
	}
	free(drum);
	return NULL;

}

int main() {
	FILE* f = fopen("rezervari.txt", "r");
	Rezervare rezervare;
	NodABC* root = NULL;
	char buffer[256], * token, separator[] = ",";
	if (f != NULL) {
		while (fgets(buffer, sizeof(buffer), f) != NULL) {
			token = strtok(buffer, separator);
			rezervare.id = atoi(token);
			token = strtok(NULL, separator);
			rezervare.nume_hotel = malloc(strlen(token) + 1);
			strcpy(rezervare.nume_hotel, token);
			token = strtok(NULL, separator);
			rezervare.nr_camere_rezervate = (unsigned char)atoi(token);
			token = strtok(NULL, separator);
			rezervare.nume_client = malloc(strlen(token) + 1);
			strcpy(rezervare.nume_client, token);
			token = strtok(NULL, separator);
			rezervare.pret_total = atof(token);
			unsigned char flag;
			root = inserare_nod(root, rezervare, &flag);
			if (flag == 1) {
				printf("\nNod adaugat cu succes.\n");
			}
			else {
				printf("\nNodul nu a fost adaugat\n");
			}
		}
		fclose(f);
	}
	printf("\nNodurile arborelui sunt:\n");
	traversare_inordine(root);

	printf("\nNumarul total de camere rezervate aflate pe frunze este: %d\n", nr_camere_rezervate(root));

	printf("\nNumarul de niveluri al arborelui: %d\n", nr_niveluri(root));

	int marime, * drum_rezervare = drum(root, 3, &marime);
	if (drum != NULL) {
		printf("\nDrumul de id-uri este:\n");
		for (int i = 0; i < marime; i++) {
			printf("%d ", drum_rezervare[i]);
		}
		free(drum_rezervare);
	}
	else {
		printf("\nNu s-a gasit un drum.\n");
	}

	unsigned int id_stergere = root->rezervare.id;
	unsigned char flag;
	root = stergere_nod(root, id_stergere, &flag);
	if (flag == 1) {
		printf("\nRadacina stearsa cu succes.\n");
		traversare_inordine(root);
	}
	else {
		printf("\nStergere esuata.\n");
	}
}