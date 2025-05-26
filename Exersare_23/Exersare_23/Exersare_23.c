#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>

#define CAPACITATE_HEAP 8

struct Carte {
	char ISBN[10];
	char* titlu;
	char* autor;
	int nrPagini;
	float pret;
};
typedef struct Carte Carte;

Carte* realocare_vector_heap(Carte* heap, unsigned char* capacitate, unsigned char plus_elem) {
	unsigned char nr_chei = *capacitate;
	*capacitate += plus_elem;
	Carte* heap_nou = malloc(*capacitate * sizeof(Carte));
	for (unsigned char i = 0; i < nr_chei; i++) {
		heap_nou[i] = heap[i];
	}
	free(heap);
	return heap_nou;
}

void heapify(Carte* heap, unsigned char capacitate, unsigned char index) {
	unsigned char maxim = index, stanga = 2 * index + 1, dreapta = 2 * index + 2;
	if (stanga<capacitate && strcmp(heap[stanga].ISBN,heap[maxim].ISBN)<0) {
		maxim = stanga;
	}
	if (dreapta<capacitate && strcmp(heap[dreapta].ISBN,heap[maxim].ISBN)>0) {
		maxim = dreapta;
	}
	if (maxim != index) {
		Carte aux = heap[index];
		heap[index] = heap[maxim];
		heap[maxim] = aux;
		heapify(heap, capacitate, maxim);
	}
}

void construire_heap(Carte* heap, unsigned char capacitate) {
	for (int i = capacitate / 2 - 1; i >= 0; i--) {
		heapify(heap, capacitate, i);
	}
}

Carte* sterger_cheie(Carte* heap, unsigned char* capacitate, char* cheie, unsigned char* flag) {
	for (int i = 0; i < *capacitate; i++) {
		if (strcmp(cheie, heap[i].ISBN) == 0) {
			*flag = 1;
		}
	}

	if (*flag == 1) {
		*capacitate -= 1;
		Carte* heap_nou = malloc((*capacitate) * sizeof(Carte));
		for (int i = 0, j = 0; i < *capacitate + 1; i++) {
			if (strcmp(cheie, heap[i].ISBN) != 0) {
				heap_nou[j] = heap[i];
				j++;
			}
			else {
				free(heap[i].titlu);
				free(heap[i].autor);
			}
		}
		free(heap);
		construire_heap(heap_nou, *capacitate);
		return heap_nou;
	}
	else {
		return heap;
	}
}

Carte* agregare_structuri_heap(Carte* h1, unsigned char size_h1, Carte* h2, unsigned char size_h2,
	Carte** h_rez, unsigned char* size_hrez, unsigned char* capacitate, Carte cheie_noua) {
	*h_rez = malloc(*capacitate * sizeof(Carte));
	(*h_rez)[0] = cheie_noua;
	*size_hrez = 1;
	if (size_h1 < size_h2) {
		Carte* paux = h1;
		h1 = h2;
		h2 = paux;
		unsigned char saux = size_h1;
		size_h1 = size_h2;
		size_h2 = saux;
	}
	unsigned char i = 0, j = 0, l = 0, gata = 1, nr_chei_hcurent = 1;
	while (gata == 1) {
		l = i;
		for (; i < size_h1 && i < l + nr_chei_hcurent; i++) {
			(*h_rez)[*size_hrez] = h1[i];
			*size_hrez += 1;
			if (*size_hrez == *capacitate) {
				*h_rez = realocare_vector_heap(*h_rez, capacitate, CAPACITATE_HEAP);
			}
		}
		for (; j < size_h2 && j < l + nr_chei_hcurent; j++) {
			(*h_rez)[*size_hrez] = h2[j];
			*size_hrez += 1;
			if (*size_hrez == *capacitate) {
				*h_rez = realocare_vector_heap(*h_rez, capacitate, CAPACITATE_HEAP);
			}
		}
		if (i == size_h1 && j == size_h2) {
			gata = 0;
		}
		nr_chei_hcurent = nr_chei_hcurent * 2;
	}
	construire_heap(*h_rez, *capacitate);
	return *h_rez;
}

int main() {
	FILE* f = fopen("carti.txt", "r");
	Carte carte;
	Carte* heap = malloc(CAPACITATE_HEAP * sizeof(Carte));
	unsigned char capacitate_heap = CAPACITATE_HEAP, nr_chei = 0;
	char buffer[256], * token, separator[] = ",";
	if (f != NULL) {
		while (fgets(buffer, sizeof(buffer), f) != NULL) {
			token = strtok(buffer, separator);
			strcpy(carte.ISBN, token);
			token = strtok(NULL, separator);
			carte.titlu = malloc(strlen(token) + 1);
			strcpy(carte.titlu, token);
			token = strtok(NULL, separator);
			carte.autor = malloc(strlen(token) + 1);
			strcpy(carte.autor, token);
			token = strtok(NULL, separator);
			carte.nrPagini = atoi(token);
			token = strtok(NULL, separator);
			carte.pret = atof(token);
			heap[nr_chei] = carte;
			nr_chei++;
			if (nr_chei == capacitate_heap) {
				heap = realocare_vector_heap(heap, &capacitate_heap, CAPACITATE_HEAP);
			}
		}
		fclose(f);
	}
	construire_heap(heap, nr_chei);
	for (int i = 0; i < nr_chei; i++) {
		printf("%s, %s, %s, %d, %.2f\n", heap[i].ISBN, heap[i].titlu, heap[i].autor, heap[i].nrPagini, heap[i].pret);
	}
	unsigned char flag=0;
	heap = sterger_cheie(heap, &nr_chei, "987654321", &flag);
	if (flag == 1) {
		printf("\nStergere reusita!\n");
		for (int i = 0; i < nr_chei; i++) {
			printf("%s, %s, %s, %d, %.2f\n", heap[i].ISBN, heap[i].titlu, heap[i].autor, heap[i].nrPagini, heap[i].pret);
		}
	}
	else {
		printf("\nStergere esuata!\n");
	}
	
	Carte* h_rez = NULL;
	Carte c1 = { "12345678", "Carte A", "Autor A", 200, 50.0 }, c2 = { "23456790", "Carte B", "Autor B", 300, 75.0 },
		c3 = { "34567890", "Carte C", "Autor C", 150, 30.0 };
	Carte heap2[] = {c1,c2,c3};
	construire_heap(heap2, 3);
	Carte cheie_noua = { "45678901", "Carte D", "Autor D", 250, 60.0 };
	unsigned char size_rez = 0;
	h_rez = agregare_structuri_heap(heap, nr_chei, heap2, 3, &h_rez, &size_rez, &capacitate_heap, cheie_noua);
	for(int i=0;i<size_rez;i++){
		printf("%s, %s, %s, %d, %.2f\n", h_rez[i].ISBN, h_rez[i].titlu, h_rez[i].autor, h_rez[i].nrPagini, h_rez[i].pret);
	}
}