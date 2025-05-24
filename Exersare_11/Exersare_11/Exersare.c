#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

#define CAPACITATE_STOCARE_HEAP 8

struct Student {
	int nr_matricol;
	char* nume;
	float medie;
	unsigned char an_studiu;
};

typedef struct Student Student;

Student* realocare_structuri_heap(Student* heap, unsigned char* capacitate, unsigned char plus_elem) {
	unsigned char nr_chei = *capacitate;
	*capacitate += plus_elem;
	Student* new_heap = malloc(*capacitate * sizeof(Student));
	for (unsigned char i = 0; i < nr_chei; i++) {
		new_heap[i].nr_matricol = heap[i].nr_matricol;
		new_heap[i].an_studiu = heap[i].an_studiu;
		new_heap[i].medie = heap[i].medie;
		new_heap[i].nume = malloc(strlen(heap[i].nume) + 1);
		strcpy(new_heap[i].nume, heap[i].nume);
			
	}
	free(heap);
	return new_heap;
}

void heapify(Student* heap, unsigned char marime_heap, unsigned char index) {
	unsigned char maxim = index;
	unsigned char stanga = 2 * index + 1;
	unsigned char dreapta = 2 * index + 2;
	if (stanga<marime_heap && heap[stanga].nr_matricol>heap[maxim].nr_matricol) {
		maxim = stanga;
	}
	if (dreapta<marime_heap && heap[dreapta].nr_matricol>heap[maxim].nr_matricol) {
		maxim = dreapta;
	}
	if (maxim != index) {
		Student aux;
		aux.nr_matricol = heap[index].nr_matricol;
		aux.an_studiu = heap[index].an_studiu;
		aux.medie = heap[index].medie;
		aux.nume = malloc(strlen(heap[index].nume) + 1);
		strcpy(aux.nume, heap[index].nume);
		heap[index].nr_matricol = heap[maxim].nr_matricol;
		heap[index].an_studiu = heap[maxim].an_studiu;
		heap[index].medie = heap[maxim].medie;
		heap[index].nume = malloc(strlen(heap[maxim].nume) + 1);
		strcpy(heap[index].nume, heap[maxim].nume);
		heap[maxim].nr_matricol = aux.nr_matricol;
		heap[maxim].an_studiu = aux.an_studiu;
		heap[maxim].medie = aux.medie;
		heap[maxim].nume = malloc(strlen(aux.nume) + 1);
		strcpy(heap[maxim].nume, aux.nume);
		heapify(heap, marime_heap, maxim);
	}
}

void construire_heap(Student* heap, unsigned char marime_heap) {
	for (int i = marime_heap / 2 - 1; i >= 0; i--) {
		heapify(heap, marime_heap, i);
	}
}

Student* agregare_structuri_heap(Student* h1, unsigned char size_h1, Student* h2, unsigned char size_h2, Student* h_rez, unsigned char* size_hrez,
	unsigned char* capacitate, Student cheie_noua) {
	h_rez = malloc(*capacitate * sizeof(Student));
	h_rez[0].nr_matricol = cheie_noua.nr_matricol;
	h_rez[0].an_studiu = cheie_noua.an_studiu;
	h_rez[0].medie = cheie_noua.medie;
	h_rez[0].nume = malloc(strlen(cheie_noua.nume) + 1);
	strcpy(h_rez[0].nume, cheie_noua.nume);
	*size_hrez = 1;
	if (size_h1 < size_h2) {
		Student* paux = h1;
		h1 = h2;
		h2 = paux;
		unsigned char saux = size_h1;
		size_h1 = size_h2;
		size_h2 = saux;

	}
	unsigned char i = 0, j = 0, nr_chei_h_curent = 1, l = 0, gata = 1;
	while (gata == 1) {
		l = i;
		for (; i < size_h1 && i < l + nr_chei_h_curent; i++) {
			h_rez[*size_hrez].nr_matricol = h1[i].nr_matricol;
			h_rez[*size_hrez].an_studiu = h1[i].an_studiu;
			h_rez[*size_hrez].medie = h1[i].medie;
			h_rez[*size_hrez].nume = malloc(strlen(h1[i].nume) + 1);
			strcpy(h_rez[*size_hrez].nume, h1[i].nume);
			(*size_hrez)++;
			if (*size_hrez == *capacitate) {
				h_rez = realocare_structuri_heap(h_rez, capacitate, 8);
			}
		}
		for (; j < size_h1 && j < l + nr_chei_h_curent; j++) {
			h_rez[*size_hrez].nr_matricol = h2[j].nr_matricol;
			h_rez[*size_hrez].an_studiu = h2[j].an_studiu;
			h_rez[*size_hrez].medie = h2[j].medie;
			h_rez[*size_hrez].nume = malloc(strlen(h2[j].nume) + 1);
			strcpy(h_rez[*size_hrez].nume, h2[j].nume);
			(*size_hrez)++;
			if (*size_hrez == *capacitate) {
				h_rez = realocare_structuri_heap(h_rez, capacitate, 8);
			}
		}
		if (i == size_h1 && j == size_h2) {
			gata = 0;
		}
		nr_chei_h_curent = nr_chei_h_curent * 2;

	}
	construire_heap(h_rez, *size_hrez);
	return h_rez;
}


int main() {
	FILE* f = fopen("studenti.txt", "r");
	Student* heap = NULL;
	unsigned char capacitate = CAPACITATE_STOCARE_HEAP;
	heap = malloc(capacitate * sizeof(Student));
	unsigned char nr_chei = 0;
	char buffer[256];
	char* token, separator[] = ",";
	if (f != NULL) {
		while (fgets(buffer, sizeof(buffer), f) != NULL) {
			token = strtok(buffer, separator);
			heap[nr_chei].nr_matricol = atoi(token);
			token = strtok(NULL, separator);
			heap[nr_chei].nume = (char*)malloc(strlen(token) + 1);
			strcpy(heap[nr_chei].nume, token);
			token = strtok(NULL, separator);
			heap[nr_chei].medie = atof(token);
			token = strtok(NULL, separator);
			heap[nr_chei].an_studiu = atoi(token);
			nr_chei++;
			if (nr_chei == capacitate) {
				heap = realocare_structuri_heap(heap, &capacitate, 8);
			}
		}
		fclose(f);
	}
	construire_heap(heap, nr_chei);
	for (int i = 0; i < nr_chei; i++) {
		printf("Nr matricol: %d, nume: %s, medie: %.2f, an studiu: %d\n", heap[i].nr_matricol, heap[i].nume, heap[i].medie, heap[i].an_studiu);
	}
	Student s1, s2, s3;
	s1.nr_matricol = 5;
	s1.an_studiu = '1';
	s1.medie = 7.25;
	s1.nume = malloc(strlen("Popescu") + 1);
	strcpy(s1.nume, "Popescu");
	s2.nr_matricol = 6;
	s2.an_studiu = '3';
	s2.medie = 4.25;
	s2.nume = malloc(strlen("Ionescu") + 1);
	strcpy(s2.nume, "Ionescu");
	Student heap2[] ={s1, s2};
	s3.nr_matricol = 7;
	s3.an_studiu = '2';
	s3.medie = 9.25;
	s3.nume = malloc(strlen("Georgescu") + 1);
	strcpy(s3.nume, "Georgescu");

	Student* heap_agregat = NULL;
	unsigned char capacitate_stocare=8, nr_chei2=2,nr_chei3;
	heap_agregat = agregare_structuri_heap(heap, nr_chei, heap2, nr_chei2, heap_agregat, &nr_chei3, &capacitate_stocare, s3);
	printf("\nHeapul agregat este: \n");
	for(int i=0;i<nr_chei3;i++){
		printf("Nr matricol: %d, nume: %s, medie: %.2f, an studiu: %d\n", heap_agregat[i].nr_matricol, heap_agregat[i].nume, heap_agregat[i].medie, heap_agregat[i].an_studiu);
	}

	for (int i = 0; i < nr_chei; i++) {
		free(heap[i].nume);
	}
}