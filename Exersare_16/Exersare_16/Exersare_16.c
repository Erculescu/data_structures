#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>

#define CAPACITATE_HEAP 8

struct camera {
	char serie[11];
	char* denumire;
	float pret;
	char an_fabricatie[5];
};
typedef struct camera Camera;

Camera* realocare_vector_heap(Camera* heap, unsigned char* capacitate, unsigned char plus_elem) {
	unsigned char nr_chei = *capacitate;
	*capacitate += plus_elem;
	Camera* heap_nou = malloc(*capacitate * sizeof(Camera));
	for (unsigned char i = 0; i < nr_chei; i++) {
		heap_nou[i] = heap[i];
	}
	free(heap);
	return heap_nou;
}

void heapify(Camera* heap, unsigned char capacitate, unsigned char index) {
	unsigned char maxim = index, stanga = 2 * index + 1, dreapta = 2 * index + 2;
	if (stanga < capacitate && strcmp(heap[stanga].serie, heap[maxim].serie) < 0) {
		maxim = stanga;
	}
	if (dreapta < capacitate && strcmp(heap[dreapta].serie, heap[maxim].serie) < 0) {
		maxim = dreapta;
	}
	if (maxim != index) {
		Camera aux = heap[index];
		heap[index] = heap[maxim];
		heap[maxim] = aux;
		heapify(heap, capacitate, maxim);
	}

}
void construire_heap(Camera* heap, unsigned char capacitate) {
	for (int i = capacitate / 2 - 1; i >= 0; i--) {
		heapify(heap, capacitate, i);
	}
}

void agregare_structuri_heap(Camera* heap1, unsigned char capacitate1, Camera* heap2, unsigned char capacitate2, Camera** h_rez, unsigned char* capacitate_rez,
	unsigned char* dimensiune, Camera cheie_noua) {
	*h_rez = malloc(*dimensiune * sizeof(Camera));
	(*h_rez)[0] = cheie_noua;
	*capacitate_rez = 1;
	if (capacitate1 < capacitate2) {
		Camera* paux = heap1;
		heap1 = heap2;
		heap2 = paux;
		unsigned char saux = capacitate1;
		capacitate1 = capacitate2;
		capacitate2 = saux;
	}
	unsigned char i = 0, j = 0, nr_chei_hc = 1, l = 0, gata = 1;
	while (gata == 1) {
		l = i;
		for (; i < capacitate1 && i < l + nr_chei_hc; i++) {
			(*h_rez)[*capacitate_rez] = heap1[i];
			*capacitate_rez += 1;
			if (*capacitate_rez >= *dimensiune) {
				*h_rez = realocare_vector_heap(h_rez, capacitate_rez, CAPACITATE_HEAP);
			}
		}
		for (; j < capacitate2 && j < l + nr_chei_hc; j++) {
			(*h_rez)[*capacitate_rez] = heap2[j];
			*capacitate_rez += 1;
			if (*capacitate_rez >= *dimensiune) {
				*h_rez = realocare_vector_heap(h_rez, capacitate_rez, CAPACITATE_HEAP);
			}
		}
		if (i == capacitate1 && j == capacitate2) {
			gata = 0;
		}
		nr_chei_hc = nr_chei_hc * 2;
	}
	construire_heap(*h_rez, *capacitate_rez);
	return *h_rez;
}


int main() {
	FILE* f = fopen("camere.txt", "r");
	char buffer[256], * token, separator[] = ",";
	Camera* heap = malloc(6 * sizeof(Camera));
	if (f != NULL) {
		int i = 0;
		while (i < 6 && fgets(buffer, sizeof(buffer), f) != NULL) {

			token = strtok(buffer, separator);
			strcpy(heap[i].serie, token);
			token = strtok(NULL, separator);
			heap[i].denumire = malloc((strlen(token) + 1));
			strcpy(heap[i].denumire, token);
			token = strtok(NULL, separator);
			heap[i].pret = atof(token);
			token = strtok(NULL, separator);
			strcpy(heap[i].an_fabricatie, token);

			i++;
		}
		fclose(f);
	}
	construire_heap(heap, 6);
	for (int i = 0; i < 6; i++) {
		printf("\nSerie: %s, Denumire: %s, Pret: %.2f, An fabricatie: %s\n", heap[i].serie, heap[i].denumire, heap[i].pret, heap[i].an_fabricatie);
	}
	Camera cam1 = { "1234565411","Nikon D5100",4000,"2013" };
	Camera cam2 = { "1234565412","Canon EOS 600D",3800,"2014" };
	Camera cam3 = { "1234565413","Sony Alpha a3",7000,"2016" };
	Camera cam4 = { "1234565414","Fujifilm X-T1",5000,"2015" };
	Camera camere[] = { cam1,cam2,cam3 };
	construire_heap(camere, 3);
	Camera* h_rez = NULL;
	unsigned char capacitate_rez = 0,dimensiune=CAPACITATE_HEAP;
	agregare_structuri_heap(heap, sizeof(heap) / sizeof(Camera), camere, sizeof(camere) / sizeof(Camera), &h_rez, &capacitate_rez, &dimensiune, cam4);
	printf("\n\nHeap agregat:\n");
	for(unsigned char i=0;i<capacitate_rez;i++)
	{
		printf("\nSerie: %s, Denumire: %s, Pret: %.2f, An fabricatie: %s\n", h_rez[i].serie, h_rez[i].denumire, h_rez[i].pret, h_rez[i].an_fabricatie);
	}
	for (int i = 0; i < 6; i++) {
		free(heap[i].denumire);
	}
	free(heap);
}