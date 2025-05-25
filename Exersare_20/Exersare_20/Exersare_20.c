#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<malloc.h>

#define CAPACITATE_HEAP 8

int* realocare_vector_heap(int* heap, unsigned char* capacitate, unsigned char plus_elem) {
	unsigned char nr_chei = *capacitate;
	*capacitate += plus_elem;
	int* heap_nou = malloc(*capacitate * sizeof(int));
	for (unsigned char i = 0; i < nr_chei; i++) {
		heap_nou[i] = heap[i];
	}
	free(heap);
	return heap_nou;
}

void heapify(int* heap, unsigned char capacitate, unsigned char index) {
	unsigned char maxim = index, stanga = 2 * index + 1, dreapta = 2 * index + 2;
	if (stanga<capacitate && heap[stanga]>heap[maxim]) {
		maxim = stanga;
	}
	if (dreapta<capacitate && heap[dreapta]>heap[maxim]) {
		maxim = dreapta;
	}
	if (maxim != index) {
		int aux = heap[index];
		heap[index] = heap[maxim];
		heap[maxim] = aux;
		heapify(heap, capacitate, maxim);
	}
}

void construire_heap(int* heap, unsigned char capacitate) {
	for (int i = capacitate / 2 - 1; i >= 0; i--) {
		heapify(heap, capacitate, i);
	}
}

int* agregare_structuri_heap(int* h1, unsigned char size_h1, int* h2, unsigned char size_h2, int* h_rez, unsigned char* size_rez,
	unsigned char* capacitate, int cheie_noua) {
	h_rez = malloc(*capacitate * sizeof(int));
	h_rez[0] = cheie_noua;
	*size_rez = 1;
	if (size_h1 < size_h2) {
		int* aux = h1;
		h1 = h2;
		h2 = aux;
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
	construire_heap(h_rez, *capacitate);
	return h_rez;
}

int main() {
	int heap[] = { 1,2,3,4,5,6,7,8 };
	construire_heap(heap, CAPACITATE_HEAP);
	int heap2[] = { 9,10,11,12,13,14,15,16,17,18 };
	int* h_rez = NULL;
	unsigned char capacitate_stocare = CAPACITATE_HEAP, nr_chei;
	printf("\nHeap agregat:\n");
	h_rez = agregare_structuri_heap(heap, CAPACITATE_HEAP, heap2, sizeof(heap2) / sizeof(int), h_rez, &nr_chei, &capacitate_stocare, 21);
	for (int i = 0; i < nr_chei; i++) {
		printf("%d ", h_rez[i]);
	}

}