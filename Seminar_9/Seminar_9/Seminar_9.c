//Structura Heap
#include<stdio.h>
#include<malloc.h>

#define CAPACITATE_STOCARE_HEAP 8

int* realocare_vector_heap(int* heap, unsigned char* capacitate, unsigned char plus_elem) {
	unsigned char nr_chei = *capacitate;//nr chei continute de heap
	*capacitate += plus_elem; //crestere capacitate de stocare pt heap
	int* new_heap = malloc(*capacitate * sizeof(int));//alocare nou vector pentru heap cu capacitate marita
	for (unsigned char i = 0; i < nr_chei; i++) {
		new_heap[i] = heap[i];//salvare elemente heap pe noua structura heap (mai mare)
	}
	free(heap);//dezalocare structura curenta heap
	return new_heap;//return noua structura heap
}

void heapify(int* heap, unsigned char marime_heap, unsigned char index) {
	unsigned char maxim = index;
	unsigned char stanga = 2 * index + 1;
	unsigned char dreapta = 2 * index + 2;
	if (stanga<marime_heap && heap[stanga]>heap[maxim]) {
		maxim = stanga;
	}
	if (dreapta<marime_heap && heap[dreapta]>heap[maxim]) {
		maxim = dreapta;
	}
	if (maxim != index) {
		int aux = heap[index];
		heap[index] = heap[maxim];
		heap[maxim] = aux;
		heapify(heap, marime_heap, maxim);
	}
}


void construire_heap(int* heap, unsigned char marime_heap) {
	for (int i = marime_heap / 2 - 1; i >= 0; i--) {
		heapify(heap, marime_heap, i);
	}
	
}

int* agregare_structuri_heap(int* h1, unsigned char size_h1, int* h2, unsigned char size_h2, int* h_rez, unsigned char* size_hrez,
	unsigned char* capacitate, int cheie_noua) {
		h_rez = malloc(*capacitate * sizeof(int));
		h_rez[0] = cheie_noua;//inserare cheie noua in heap agregat
		*size_hrez = 1;//actualizare nr chei din heap agregat
		if (size_h1 < size_h2) //structura heap2 este mai mare (nr chei si implicit inaltime) decat structura heap1
		//agregarea se efectueaza pe heap1 mai mare decat heap2
		{
			//se interschimba cele doua structuri heap (input pt structura agregata)
			int* paux = h1;
			h1 = h2;
			h2 = paux;
			unsigned char saux = size_h1;
			size_h1 = size_h2;
			size_h2 = saux;

		}
		unsigned char i = 0;//offset traversare h1
		unsigned char j = 0;//offset traversare h2
		unsigned char nr_chei_hcurent = 1;//nr de chei de pe nivel curent care se salveaza in heap agregat
		unsigned char l = 0;//nr chei salvate anterior in structura heap agregata
		unsigned char gata = 1;
		while (gata==1)/*Asigura traversarea pe niveluri in tandem pentru h1 si h2*/ {
			l = i;//nr de elemente salvate anterior in structura agregata
			//salvare chei de pe nivel curent in h1
			for (; i < size_h1 && i<l+nr_chei_hcurent;i++) {
				h_rez[*size_hrez] = h1[i];
				*size_hrez += 1;
				if (*size_hrez == *capacitate) {
					h_rez = realocare_vector_heap(h_rez, capacitate, CAPACITATE_STOCARE_HEAP);
				}
			}
			//salvare chei de pe nivel curent in h2
			for (; j < size_h2 && j<l+nr_chei_hcurent;j++) {
				h_rez[*size_hrez] = h2[j];
				*size_hrez += 1;
				if (*size_hrez == *capacitate) {
					h_rez = realocare_vector_heap(h_rez,capacitate,CAPACITATE_STOCARE_HEAP);
				}
			}

			if (i == size_h1 && j == size_h2) /*Este suficient i== size_h1 deoarece h1 este intotodeauna mai mare decat h2*/{
				gata = 0;//ambele structuri au fost salvate in heap agregat
			}
			nr_chei_hcurent = nr_chei_hcurent * 2;//calcul nr de chei de pe nivelul urmator
		}
		construire_heap(h_rez, *size_hrez);
		return h_rez;
}

int main() {
	int heap1[] = {32,12,24,7,9,17};
	int heap2[] = {8,2,3};
	int* heap_agregat = NULL;
	unsigned char capacitate_stocare, nr_chei;
	capacitate_stocare = CAPACITATE_STOCARE_HEAP;
	heap_agregat = agregare_structuri_heap(heap1, sizeof(heap1) / sizeof(int), heap2, sizeof(heap2) / sizeof(int), heap_agregat, &nr_chei, &capacitate_stocare,23);
	printf("Heapul agregat este: ");
	for (unsigned char i = 0; i < nr_chei; i++) {
		printf("%d ", heap_agregat[i]);
	}
	printf("\n");
	//TEMA:de continuat ordonarea cheilor din heapul agregat
}