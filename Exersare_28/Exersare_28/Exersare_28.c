#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>

struct Camera {
	char Serie[10];
	char* Marca;
	char* Model;
	float Pret;
};

typedef struct Camera Camera;
struct Nod {
	Camera camera;
	struct Nod* next;
};
typedef struct Nod Nod;

Nod* creareNod(Camera camera) {
	Nod* nod = (Nod*)malloc(sizeof(Nod));
	nod->camera = camera;
	nod->next = NULL;
	return nod;
}
Nod* inserareNodSfarsit(Nod* cap, Camera camera) {
	Nod* nod = creareNod(camera);
	if (cap == NULL) {
		return nod;
	}
	else {
		Nod* temp = cap;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = nod;
		return cap;

	}
}

void afisareLista(Nod* cap) {
	Nod* temp = cap;
	while (temp != NULL) {
		printf("Serie: %s, Marca: %s, Model: %s, Pret: %.2f\n", temp->camera.Serie, temp->camera.Marca, temp->camera.Model, temp->camera.Pret);
		temp = temp->next;
	}
}

void eliberareLista(Nod* cap) {
	Nod* temp=cap;
	while (cap != NULL) {
		temp = cap;
		cap = cap->next;
		free(temp->camera.Marca);
		free(temp->camera.Model);
		free(temp);
	}
}

Nod* swapNoduri(Nod* cap, char* serie1, char* serie2) {
	if (strcmp(serie1, serie2) == 0) {
		return cap;
	}
	Nod* prev1 = NULL, * prev2 = NULL, * ncurent1 = cap, * ncurent2 = cap;
	while (ncurent1 != NULL && strcmp(ncurent1->camera.Serie, serie1) != 0) {
		prev1 = ncurent1;
		ncurent1 = ncurent1->next;
	}
	while (ncurent2 != NULL && strcmp(ncurent2->camera.Serie, serie2) != 0) {
		prev2 = ncurent2;
		ncurent2 = ncurent2->next;
	}
	if (ncurent1 == NULL || ncurent2 == NULL) {
		return cap;
	}
	if (prev1 != NULL) {
		prev1->next = ncurent2;
	}
	else {
		cap = ncurent2;
	}
	if (prev2 != NULL) {
		prev2->next = ncurent1;
	}
	else {
		cap = ncurent1;
	}
	Nod* temp = ncurent1->next;
	ncurent1->next = ncurent2->next;
	ncurent2->next = temp;
	return cap;
}

Nod* stergereNod(Nod* cap, char* serie) {
	Nod* temp = cap, * prev = NULL;
	while (temp != NULL && strcmp(temp->camera.Serie, serie) != 0) {
		prev = temp;
		temp = temp->next;
	}
	if (temp == NULL) {
		return cap;
	}
	if (prev == NULL) {
		cap = temp->next;
	}
	else {
		prev->next = temp->next;
	}
	free(temp->camera.Marca);
	free(temp->camera.Model);
	free(temp);
	return cap;
}


int main() {
	FILE* f = fopen("camere.txt", "r");
	Nod* cap = NULL;
	char buffer[256], * token, separator[] = ",";
	Camera camera;
	if (f != NULL) {
		while (fgets(buffer, sizeof(buffer), f) != NULL) {
			token = strtok(buffer, separator);
			strcpy(camera.Serie, token);
			token = strtok(NULL, separator);
			camera.Marca = (char*)malloc(strlen(token) + 1);
			strcpy(camera.Marca, token);
			token = strtok(NULL, separator);
			camera.Model = (char*)malloc(strlen(token) + 1);
			strcpy(camera.Model, token);
			token = strtok(NULL, separator);
			camera.Pret = atof(token);
			cap = inserareNodSfarsit(cap, camera);
		}
		fclose(f);
	}
	afisareLista(cap);
	cap = swapNoduri(cap, "b35rollei", "canon1000");
	printf("\n\n\n");
	afisareLista(cap);
	printf("\n\n\n");
	cap = stergereNod(cap, "b35rollei");
	afisareLista(cap);
	eliberareLista(cap);
}