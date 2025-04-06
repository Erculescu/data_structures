#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct Vagon {
	int numar;
	int numar_locuri;
	int pasageri;
	char* companie;
	float masa_maxima;
};
typedef struct Vagon Vagon;

struct NodD {
	Vagon vagon;
	struct NodD* prev;
	struct NodD* next;
};
typedef struct NodD NodD;

NodD* adaugare_vagon(NodD* lista, Vagon vagon) {
	NodD* nou = (NodD*)malloc(sizeof(NodD));
	nou->vagon = vagon;
	nou->prev = NULL;
	nou->next = NULL;
	if (lista == NULL) {
		return nou;
	}
	else {
		NodD* temp = lista;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = nou;
		nou->prev = temp;
		return lista;
	}

}
void traversare_lista(NodD* lista) {
	NodD* temp = lista;
	printf("Lista vagoanelor parcursa de la inceput la sfarsit\n");
	while (temp!= NULL) {
		printf("Vagonul %d are %d locuri, %d pasageri, compania %s si masa maxima autorizata %.2f\n", temp->vagon.numar, temp->vagon.numar_locuri, temp->vagon.pasageri, temp->vagon.companie, temp->vagon.masa_maxima);
		temp = temp->next;
	}
	
	temp = lista;
	printf("Lista vagoanelor parcursa de la sfarsit la inceput\n");
	while (temp->next != NULL) {
		temp = temp->next;
	}
	while (temp != NULL) {
		printf("Vagonul %d are %d locuri, %d pasageri, compania %s si masa maxima autorizata %.2f\n", temp->vagon.numar, temp->vagon.numar_locuri, temp->vagon.pasageri, temp->vagon.companie, temp->vagon.masa_maxima);
		temp = temp->prev;
	}
}

void eliminare_lista(NodD* lista) {
	NodD* temp = lista;
	while (temp != NULL) {
		NodD* de_eliminat = temp;
		free(temp->vagon.companie);
		free(temp);
		temp = de_eliminat->next;
	}
}

void interschimbare(NodD* lista, int k) {
	int poz = 0;
	NodD* temp = lista;
	while (temp != NULL && poz < k - 1) {
		temp = temp->next;
		poz++;
	}
	if (temp != NULL) {
		NodD* temp2 = temp->next;
		if (temp2 != NULL) {
			Vagon aux = temp->vagon;
			temp->vagon.pasageri = temp2->vagon.pasageri;
			temp2->vagon.pasageri = aux.pasageri;

		}
		

	}
	else {
		printf("Nu exista un vagon pentru a face interschimbarea\n");
	}

}

int verifica_simetrie(NodD* lista) {
	NodD* temp = lista;
	int nr = 0;
	while (temp != NULL) {
		nr++;
		temp = temp->next;
	}
	int* pasageri = (int*)malloc(nr * sizeof(int));
	temp = lista;
	int i = nr-1;
	while (temp != NULL) {
		pasageri[i] = temp->vagon.pasageri;
		temp = temp->next;
		i--;
	}
	int k = 0;
	i = 0;
	temp = lista;
	while (temp != NULL) {
		if (temp->vagon.pasageri != pasageri[i]) {
			k = 1;
			break;
		}
		temp = temp->next;
		i++;
	}
	free(pasageri);
	return k;

}



int main() {

	FILE* f = fopen("vagoane.txt", "r");
	Vagon vagon;
	NodD* lista = NULL;

	char buffer[256];
	char* token, separator[] = ",";
	if (f != NULL) {
		while (fgets(buffer, sizeof(buffer), f) != NULL) {
			token = strtok(buffer, separator);
			vagon.numar = atoi(token);
			token = strtok(NULL, separator);
			vagon.numar_locuri = atoi(token);
			token = strtok(NULL, separator);
			vagon.pasageri = atoi(token);
			token = strtok(NULL, separator);
			vagon.companie = (char*)malloc(strlen(token) + 1);
			strcpy(vagon.companie, token);
			token = strtok(NULL, separator);
			vagon.masa_maxima = atof(token);
			lista = adaugare_vagon(lista, vagon);

		}
		fclose(f);

	}
	printf("Parcurgere lista initiala:\n");
	traversare_lista(lista);


	if (verifica_simetrie(lista) == 0) {
		printf("Lista este simetrica\n");
	}
	else {
		printf("Lista nu este simetrica\n");
	}
	interschimbare(lista, 2);
	printf("Parcurgere lista dupa interschimbare:\n");
	traversare_lista(lista);

	/*printf("Parcurgere lista dupa eliminare: \n");
	eliminare_lista(lista);
	traversare_lista(lista);*/


}