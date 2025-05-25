	#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<malloc.h>
#include<string.h>

#define CPACITATE_HEAP 8

char** realocare_vector_heap(char** heap, unsigned char* capacitate, unsigned char plus_elemente) {
	unsigned char nr_chei = *capacitate;
	*capacitate += plus_elemente;
	char** new_heap = malloc(*capacitate * sizeof(char*));
	for (unsigned char i = 0; i < nr_chei; i++) {
		new_heap[i] = malloc(strlen(heap[i]) + 1);
		strcpy(new_heap[i], heap[i]);
		/*free(heap[i]);*/
	}
	free(heap);
	return new_heap;
}

void heapify(char** heap, unsigned char marime_heap, unsigned char index) {
	unsigned char maxim = index, stanga = 2 * index + 1, dreapta = 2 * index + 2;
	if (stanga < marime_heap && strcmp(heap[stanga], heap[maxim])>0) {
		maxim = stanga;
	}
	if (dreapta < marime_heap && strcmp(heap[dreapta], heap[maxim])>0)
	{
		maxim = dreapta;
	}
	if (maxim != index) {
		char* aux = heap[index];
		heap[index] = heap[maxim];
		heap[maxim] = aux;
		heapify(heap, marime_heap, maxim);
	}

}

void construire_heap(char** heap, unsigned char marime_heap) {
	for (int i = marime_heap / 2 - 1; i >= 0; i--) {
		heapify(heap, marime_heap, i);
	}
}

char** agregare_structuri_heap(char** h1, unsigned char size_h1, char** h2, unsigned char size_h2,
	char** h_rez, unsigned char* size_hrez, unsigned char* capacitate, char* cheie_noua) {
	h_rez = malloc(*capacitate * sizeof(char*));
	h_rez[0] = malloc(strlen(cheie_noua) + 1);
	strcpy(h_rez[0], cheie_noua);
	*size_hrez = 1;
	if (size_h1 < size_h2) {
		char** paux = h1;
		h1 = h2;
		h2 = paux;
		unsigned char saux = size_h1;
		size_h1 = size_h2;
		size_h2 = saux;
	}
	unsigned char i = 0, j = 0, nr_chei_hcurent = 1, l = 0, gata = 1;
	while (gata == 1) {
		l = i;
		for (; i < size_h1 && i < l + nr_chei_hcurent; i++) {
			h_rez[*size_hrez] = malloc(strlen(h1[i]) + 1);
			strcpy(h_rez[*size_hrez], h1[i]);
			*size_hrez += 1;
			if (*size_hrez == *capacitate) {
				h_rez = realocare_vector_heap(h_rez, capacitate, CPACITATE_HEAP);
			}
		}
		for (; j < size_h2 && j < l + nr_chei_hcurent; j++) {
			h_rez[*size_hrez] = malloc(strlen(h2[j]) + 1);
			strcpy(h_rez[*size_hrez], h2[j]);
			*size_hrez += 1;
			if (*size_hrez == *capacitate) {
				h_rez = realocare_vector_heap(h_rez, capacitate, CPACITATE_HEAP);
			}
		}
		if (i == size_h1 && j == size_h2) {
			gata = 0;
		}
		nr_chei_hcurent = nr_chei_hcurent * 2;
	}
	construire_heap(h_rez, *size_hrez);
	return h_rez;
}

int main() {
	char** heap = malloc(CPACITATE_HEAP * sizeof(char*));
	heap[0] = "Ana";
	heap[1] = "are";
	heap[2] = "mere";
	heap[3] = "si";
	heap[4] = "pere";
	heap[5] = "si";
	heap[6] = "prune";
	heap[7] = "adevarate";
	construire_heap(heap, 8);
	for(int i=0; i < 8; i++) {
		printf("%s ", heap[i]);
	}
	char* heap2[] = { "The", "quick", "brown", "fox", "jumps", "over", "the", "lazy", "dog" };
	char** h_rez = NULL;
	unsigned char capacitate_stocare = CPACITATE_HEAP, nr_chei;
	printf("\n\nHeap Agregat:\n");
	h_rez = agregare_structuri_heap(heap, 8, heap2, sizeof(heap2) / sizeof(char*)
		, h_rez, &nr_chei, &capacitate_stocare, "Ion");
	for (int i = 0; i < nr_chei; i++) {
		printf("%s ", h_rez[i]);
		free(h_rez[i]);
	}
	free(h_rez);

}