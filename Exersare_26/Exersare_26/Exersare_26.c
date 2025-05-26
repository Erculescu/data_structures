#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>

#define CAPACITATE_HEAP 8

float* realocare_vector_heap(float* heap, unsigned char* capacitate, unsigned char plus_elem) {
	unsigned char nr_chei = *capacitate;
	*capacitate += plus_elem;
	float* heap_nou = malloc(*capacitate * sizeof(float));
	for (unsigned char i = 0; i < nr_chei; i++) {
		heap_nou[i] = heap[i];
	}
	free(heap);
	return heap_nou;
}

void heapify(float* heap, unsigned char capacitate, unsigned char index) {
	unsigned char max = index, stanga = 2 * index + 1, dreapta = 2 * index + 2;
	if (stanga<capacitate && heap[stanga]>heap[max]) {
		max = stanga;
	}
	if (dreapta<capacitate && heap[dreapta]>heap[max]) {
		max = dreapta;
	}
	if (max != index) {
		float aux = heap[index];
		heap[index] = heap[max];
		heap[max] = aux;
		heapify(heap, capacitate, max);
	}
}

void construire_heap(float* heap, unsigned char capacitate) {
	for (int i = capacitate / 2 - 1; i >= 0; i--) {
		heapify(heap, capacitate, i);
	}
}

float* stergere_cheie(float* heap, unsigned char *capacitate,float cheie){
	unsigned char flag = 0;
	for (int i = 0; i < *capacitate; i++) {
		if (heap[i] == cheie) {
			flag = 1;
		}
	}
	if (flag == 1) {
		float* heap_nou = malloc((*capacitate - 1) * sizeof(float));
		for (int i = 0, j = 0; i < *capacitate; i++) {
			if (heap[i] != cheie) {
				heap_nou[j] = heap[i];
				j++;
			}
		}
		free(heap);
		*capacitate -= 1;
		construire_heap(heap_nou, *capacitate);
		return heap_nou;
	}
	return heap;
}

float* agregare_structuri_heap(float* h1, unsigned char size_h1, float* h2, unsigned char size_h2, float* h_rez, unsigned char* size_rez,
	unsigned char* capacitate, float cheie) {
	h_rez = malloc(*capacitate * sizeof(float));
	h_rez[0] = cheie;
	*size_rez = 1;
	if (size_h1 < size_h2) {
		float* paux = h1;
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
			h_rez[*size_rez] = h1[i];
			*size_rez += 1;
			if (*size_rez == *capacitate) {
				h_rez = realocare_vector_heap(h_rez, capacitate, CAPACITATE_HEAP);
			}
		}
		for (; j < size_h2 && j < l + nr_chei_hcurent; j++) {
			h_rez[*size_rez] = h2[j];
			*size_rez += 1;
			if (*size_rez == *capacitate) {
				h_rez = realocare_vector_heap(h_rez, capacitate, CAPACITATE_HEAP);
			}
		}
		if (i == size_h1 && j == size_h2) {
			gata = 0;
		}
		nr_chei_hcurent = nr_chei_hcurent * 2;
	}
	construire_heap(h_rez, *size_rez);
	return h_rez;
}

int main() {
	FILE* f = fopen("heaps.txt", "r");
	float* heap = malloc(CAPACITATE_HEAP * sizeof(float));
	unsigned char capacitate = CAPACITATE_HEAP, nr_chei = 0;
	char buffer[256], * token, separator[] = ",";
	if (f != NULL) {
		while (fgets(buffer, sizeof(buffer), f)) {
			token = strtok(buffer, separator);
			while (token != NULL) {
				heap[nr_chei] = atof(token);
				nr_chei++;
				if (nr_chei == capacitate) {
					heap = realocare_vector_heap(heap, &capacitate, CAPACITATE_HEAP);
				}
				token = strtok(NULL, separator);
			}
		}
		fclose(f);
	}
	construire_heap(heap, nr_chei);
	for (int i = 0; i < nr_chei; i++) {
		printf("%.2f ", heap[i]);
	}
	heap = stergere_cheie(heap, &nr_chei, 6543);
	printf("\n\n");
	for (int i = 0; i < nr_chei; i++) {
		printf("%.2f ", heap[i]);
	}
	printf("\n\n");
	float heap2[] = { 2.21,3.15,4.44,452,443 };
	float* heap_rez = NULL;
	unsigned char capacitate_rez = CAPACITATE_HEAP, nr_chei_rez;
	heap_rez = agregare_structuri_heap(heap, nr_chei, heap2, sizeof(heap2) / sizeof(heap),heap_rez,&nr_chei_rez,&capacitate_rez,1200);
	for (int i = 0; i < nr_chei_rez; i++) {
		printf("%.2f ", heap_rez[i]);
	}
}