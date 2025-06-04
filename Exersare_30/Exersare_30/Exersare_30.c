#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>

#define DIMENSIUNE_HASH 100

struct Camera {
	char serie[10];
	char* Marca;
	char* Model;
	float pret;
};
typedef struct Camera Camera;
struct Nod {
	Camera cam;
	struct Nod* next;
};
typedef struct Nod Nod;

Nod* adaugaNod(Nod* cap, Camera cam) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->cam = cam;
	nou->next = NULL;
	if (cap == NULL) {
		return nou;
	}
	Nod* p = cap;
	while (p->next != NULL) {
		p = p->next;
	}
	p->next = nou;
	return cap;
}
void traversare_lista(Nod* cap) {
	Nod* p = cap;
	while (p != NULL) {
		printf("\nSerie: %s, Marca: %s, Model: %s, Pret: %.2f", p->cam.serie, p->cam.Marca, p->cam.Model, p->cam.pret);
		p = p->next;
	}
}

unsigned char functie_hash(char* cheie, unsigned int dimensiune) {
	unsigned char hash = 0;
	for (int i = 0; i < strlen(cheie); i++) {
		hash += cheie[i];
	}
	return hash % dimensiune;
}
void adauga_in_hash(Nod** hash_table, Camera cam, unsigned int dimensiune) {
	unsigned char poz = functie_hash(cam.serie, dimensiune);
	hash_table[poz] = adaugaNod(hash_table[poz], cam);
}
void traversare_hash(Nod** hash_table, unsigned int dimensiune) {
	for (unsigned int i = 0; i < dimensiune; i++) {
		if (hash_table[i] != NULL) {
			printf("\nLista pentru hash %d:", i);
			traversare_lista(hash_table[i]);
		}
	}
}

float valoare_stoc(Nod** hash_table, unsigned int dimensiune) {
	float total = 0.0f;
	for (unsigned int i = 0; i < dimensiune; i++) {
		Nod* p = hash_table[i];
		while (p != NULL) {
			total += p->cam.pret;
			p = p->next;
		}

	}
	return total;
}

void elibereaza_lista(Nod* cap) {
	Nod* p = cap;
	while (p != NULL) {
		Nod* temp = p;
		p = p->next;
		free(temp->cam.Marca);
		free(temp->cam.Model);
		free(temp);
	}

}
void elibereaza_hash(Nod** hash_table, unsigned int dimensiune) {
	for (unsigned int i = 0; i < dimensiune; i++) {
		if (hash_table[i] != NULL) {
			elibereaza_lista(hash_table[i]);
			hash_table[i] = NULL;
		}

	}

}

void stergere_marca(Nod** hash_table, unsigned int dimensiune, char* marca) {
	for (unsigned int i = 0; i < dimensiune; i++) {
		Nod* p = hash_table[i];
		Nod* prev = NULL;
		while (p != NULL) {
			if (strcmp(p->cam.Marca, marca) == 0) {
				Nod* de_sters = p;
				if (prev == NULL) {
					hash_table[i] = p->next;
					p = p->next;
				}
				else {
					prev->next = p->next;
					p = p->next;
				}
				free(de_sters->cam.Marca);
				free(de_sters->cam.Model);
				free(de_sters);

			}
			else {
				prev = p;
				p = p->next;
			}
		}
	}
}

int main() {
	FILE* f = fopen("camere.txt", "r");
	Camera cam;
	Nod** hash_table = (Nod**)malloc(DIMENSIUNE_HASH * sizeof(Nod*));
	for (unsigned int i = 0; i < DIMENSIUNE_HASH; i++) {
		hash_table[i] = NULL;
	}
	char buffer[256], * token, separator[] = ",";
	if (f != NULL) {
		while (fgets(buffer, sizeof(buffer), f) != NULL) {
			token = strtok(buffer, separator);
			strcpy(cam.serie, token);
			token = strtok(NULL, separator);
			cam.Marca = malloc(strlen(token) + 1);
			strcpy(cam.Marca, token);
			token = strtok(NULL, separator);
			cam.Model = malloc(strlen(token) + 1);
			strcpy(cam.Model, token);
			token = strtok(NULL, separator);
			cam.pret = atof(token);
			adauga_in_hash(hash_table, cam, DIMENSIUNE_HASH);
		}
		fclose(f);
	}
	printf("\nContinut:\n");
	traversare_hash(hash_table, DIMENSIUNE_HASH);
	printf("\nValoarea totala a stocului: %.2f", valoare_stoc(hash_table, DIMENSIUNE_HASH));
	printf("\nStergere marca Minolta\n");
	stergere_marca(hash_table, DIMENSIUNE_HASH, "Minolta");
	traversare_hash(hash_table, DIMENSIUNE_HASH);
	elibereaza_hash(hash_table, DIMENSIUNE_HASH);
	free(hash_table);
}