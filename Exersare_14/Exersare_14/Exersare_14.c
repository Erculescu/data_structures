#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<malloc.h>

#define CAPACITATE_STOCARE_HEAP 8

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
	unsigned char max = index;
	unsigned char stanga = 2 * index + 1, dreapta = 2 * index + 2;
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

float* agregare_structuri_heap(float* h1, unsigned char capacitate1, float* h2, unsigned char capacitate2, float* h_rez, unsigned char* capacitate_hrez,
	unsigned char* dimensiune, float cheie_noua) {
	h_rez = malloc(*dimensiune * sizeof(float));
	h_rez[0] = cheie_noua;
	*capacitate_hrez = 1;
	if (capacitate1 < capacitate2) {
		float* paux = h1;
		h1 = h2;
		h2 = paux;
		unsigned char caux = capacitate1;
		capacitate1 = capacitate2;
		capacitate2 = caux;
	}
	unsigned char i = 0, j = 0, nr_chei_hc = 1, l = 0, gata = 1;
	while (gata == 1) {
		l = i;
		for (; i < capacitate1 && i < l + nr_chei_hc; i++) {
			h_rez[*capacitate_hrez] = h1[i];
			*capacitate_hrez += 1;
			if (*capacitate_hrez >= *dimensiune) {
				h_rez = realocare_vector_heap(h_rez, dimensiune, CAPACITATE_STOCARE_HEAP);
			}
		}
		for (; j < capacitate2 && j < l + nr_chei_hc; j++) {
			h_rez[*capacitate_hrez] = h2[j];
			*capacitate_hrez += 1;
			if (*capacitate_hrez >= *dimensiune) {
				h_rez = realocare_vector_heap(h_rez, dimensiune, CAPACITATE_STOCARE_HEAP);
			}
		}
		if (i == capacitate1 && j == capacitate2) {
			gata = 0;
		}
		nr_chei_hc = nr_chei_hc * 2;
	}
	construire_heap(h_rez, *capacitate_hrez);
	return h_rez;
}


int main() {
	/*int n;
	printf("Introduceti numarul de elemente: ");
	scanf("%d", &n);
	float* heap1 = malloc(n * sizeof(float));
	for (int i = 0; i < n; i++) {
		printf("Elementul %d:", i + 1);
		scanf("%f", &heap1[i]);
	}*/
	float heap1[] = { 1.3,22.2,6.3,3.1,112,0.3,3 };
	float heap2[] = { 6.5,8.2,7.2,11.3 };
	float* heap_rez = NULL;
	unsigned char capacitate_stocare=CAPACITATE_STOCARE_HEAP, nr_chei;
	heap_rez = agregare_structuri_heap(heap1, sizeof(heap1)/sizeof(float), heap2, sizeof(heap2) / sizeof(float), heap_rez, &nr_chei, &capacitate_stocare, 23.7);
	printf("Heapul rezultat este:\n");
	for (unsigned char i = 0; i < nr_chei; i++) {
		printf("%.2f ", heap_rez[i]);
	}
}