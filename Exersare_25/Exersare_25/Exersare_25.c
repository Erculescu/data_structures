#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>

#define CAPACITATE_HEAP 8
struct Electro {
	unsigned short int cod;
	char* marca;
	char* model;
	char* tip;
	float pret;
};
typedef struct Electro Electro;

Electro* realocare_vector_heap(Electro* heap, int* capacitate, int plus_elem) {
	unsigned char nr_chei=*capacitate;
	*capacitate += plus_elem;
	Electro* heap_nou = (Electro*)malloc(*capacitate * sizeof(Electro));
	for (unsigned char i = 0; i < nr_chei; i++) {
		heap_nou[i] = heap[i];
	}
	free(heap);
	return heap_nou;
}

void heapify(Electro* h1, unsigned char* capacitate, unsigned char index) {
	unsigned char maxim = index, stanga = 2 * index + 1, dreapta = 2 * index + 2;
	if (stanga<*capacitate && h1[stanga].cod>h1[maxim].cod) {
		maxim = stanga;
	}
	if (dreapta<*capacitate && h1[dreapta].cod>h1[maxim].cod) {
		maxim = dreapta;
	}
	if (maxim != index) {
		Electro aux = h1[index];
		h1[index] = h1[maxim];
		h1[maxim] = aux;
		heapify(h1, capacitate, maxim);
	}

}

void construire_heap(Electro* h1, unsigned char* capacitate) {
	for (int i = *capacitate / 2 - 1; i >= 0; i--) {

		heapify(h1, capacitate, i);
	}
}

Electro* agregare_structuri_heap(Electro* h1, unsigned char size_h1, Electro* h2, unsigned char size_h2, Electro** h_rez, unsigned char* size_hrez, unsigned char* capacitate, Electro cheie_noua)
{
	*h_rez = malloc(*capacitate * sizeof(Electro));
	(*h_rez)[0] = cheie_noua;
	*size_hrez = 1;
	if (size_h1 < size_h2) {
		Electro* temp = h1;
		h1 = h2;
		h2 = temp;
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
				*h_rez = realocare_vector_heap(h_rez, capacitate, CAPACITATE_HEAP);
			}
		}
		for (; j < size_h2 && j < l + nr_chei_hcurent; j++) {
			(*h_rez)[*size_hrez] = h2[j];
			*size_hrez += 1;
			if (*size_hrez == *capacitate) {
				*h_rez = realocare_vector_heap(h_rez, capacitate, CAPACITATE_HEAP);
			}
		}
		if (i == size_h1 && j == size_h2) {
			gata = 0;

		}
		nr_chei_hcurent = nr_chei_hcurent * 2;
	}
	construire_heap(*h_rez, size_hrez);
	return *h_rez;
}

int main() {
	FILE* f = fopen("electro.txt", "r");
	Electro electro;
	Electro* heap = malloc(CAPACITATE_HEAP * sizeof(Electro));
	unsigned char capacitate = CAPACITATE_HEAP, nr_chei = 0;
	char buffer[256], * token, separator[] = ",";
	if (f != NULL) {
		while (fgets(buffer, sizeof(buffer), f) != NULL) {
			token = strtok(buffer, separator);
			electro.cod = atoi(token);
			token = strtok(NULL, separator);
			electro.marca = malloc(strlen(token) + 1);
			strcpy(electro.marca, token);
			token = strtok(NULL, separator);
			electro.model = malloc(strlen(token) + 1);
			strcpy(electro.model, token);
			token = strtok(NULL, separator);
			electro.tip = malloc(strlen(token) + 1);
			strcpy(electro.tip, token);
			token = strtok(NULL, separator);
			electro.pret = atof(token);
			heap[nr_chei] = electro;
			nr_chei++;
			if (nr_chei == capacitate) {
				heap = realocare_vector_heap(heap, &capacitate, CAPACITATE_HEAP);
			}
		}
		fclose(f);
	}
	construire_heap(heap, &nr_chei);
	for (int i = 0; i < nr_chei; i++) {
		printf("\n%d %s %s %s %.2f\n", heap[i].cod, heap[i].marca, heap[i].model, heap[i].tip, heap[i].pret);
	}

	Electro e1 = { 100, "Samsung", "Galaxy S21", "Smartphone", 2999.99 }, e2 = { 101, "LG", "OLED55CX", "Televizor", 4999.99 }, e3 = { 102, "Bosch", "Serie 4", "Masina de spalat", 1999.99 },
		e4 = { 103, "Philips", "Airfryer", "Friteuza", 799.99 }, e5 = { 104, "Sony", "WH-1000XM4", "Casti", 1299.99 };
	Electro heap_2[] = { e1, e2, e3, e4 };
	Electro* heap_rez = NULL;
	unsigned char size_h1 = nr_chei, size_h2 = 4, size_hrez = 0, capacitate_rez = CAPACITATE_HEAP;
	heap_rez = agregare_structuri_heap(heap, size_h1, heap_2, size_h2, &heap_rez, &size_hrez, &capacitate_rez, e5);
	for(int i=0;i<size_hrez;i++){
		printf("\n%d %s %s %s %.2f\n", heap_rez[i].cod, heap_rez[i].marca, heap_rez[i].model, heap_rez[i].tip, heap_rez[i].pret);
	}
}