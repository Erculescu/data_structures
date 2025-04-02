#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>

#define LUNGIME_CNP 11

struct Student {
	char cnp[LUNGIME_CNP];
	char* nume;
	int varsta;
	float medie;
};

typedef struct Student Student;

struct NodD {
	Student student;
	struct NodD* prev;
	struct NodD* next;
};
typedef struct NodD NodD;

NodD* adauga_inceput(NodD* lista, Student stud) {
	NodD* nod_nou = (NodD*)malloc(sizeof(NodD));
	strcpy(nod_nou->student.cnp, stud.cnp);
	nod_nou->student.nume = (char*)malloc(strlen(stud.nume) + 1);
	strcpy(nod_nou->student.nume, stud.nume);
	nod_nou->student.varsta = stud.varsta;
	nod_nou->student.medie = stud.medie;
	nod_nou->prev = NULL;
	nod_nou->next = lista;
	if (lista != NULL) {
		lista->prev = nod_nou;
	}
	return nod_nou;

}

NodD* stergere_nod(NodD* lista, char* cnp) {
	NodD* temp = lista;
	while (temp != NULL) {
		if (strcmp(temp->student.cnp, cnp) == 0) {
			if (temp->prev != NULL) {
				temp->prev->next = temp->next;
			}
			else {
				lista = temp->next;
			}
			if (temp->next != NULL) {
				temp->next->prev = temp->prev;
			}
			free(temp->student.nume);
			free(temp);
			return lista;
		}
		temp = temp->next;
	}
	return lista;
}

void traversare_lista(NodD* lista) {
	NodD* temp = lista;
	printf("Lista studentilor traversata de la inceput: \n");
	while (temp != NULL) {
		printf("CNP: %s, Nume: %s, Varsta %d, Medie: %5.2f\n", temp->student.cnp, temp->student.nume, temp->student.varsta, temp->student.medie);
		temp = temp->next;
	}

	printf("\nLista studentilor traversata de la sfarsit: \n");
	temp = lista;
	while (temp->next != NULL) {
		temp = temp->next;
	}
	while(temp!=NULL){
		printf("CNP: %s, Nume: %s, Varsta %d, Medie: %5.2f\n", temp->student.cnp, temp->student.nume, temp->student.varsta, temp->student.medie);
		temp = temp->prev;
	}
}

void interschimbare(NodD* lista, int i) {
	if (lista == NULL || i < 0) {
		return;
	}
	NodD* temp = lista;
	int poz = 0;
	while (temp != NULL && poz < i) {
		temp = temp->next;
		poz++;
	}
	if (temp == NULL || temp->next == NULL) {
		return;
	}
	NodD* urmator = temp->next;
	if (temp->prev != NULL) {
		temp->prev->next = urmator;
	}
	if (urmator->next != NULL) {
		urmator->next->prev = temp;
	}
	urmator->prev = temp->prev;
	temp->next = urmator->next;
	urmator->next = temp;
	temp->prev = urmator;

	if (temp == lista) {
		lista = urmator;
	}

}

int main() {
	FILE* f = fopen("studenti.txt", "r");

	Student stud;
	NodD* lista = NULL;
	

	char buffer[256];
	char* token, separator[] = ",";
	if (f != NULL) {
		while (fgets(buffer, sizeof(buffer), f) != NULL) {
			token = strtok(buffer, separator);
			strcpy(stud.cnp, token);
			token = strtok(NULL, separator);
			stud.nume = (char*)malloc(strlen(token) * sizeof(char) + 1);
			strcpy(stud.nume, token);
			token = strtok(NULL, separator);
			stud.varsta = atoi(token);
			token = strtok(NULL, separator);
			stud.medie = atof(token);
			lista = adauga_inceput(lista, stud);
		}
		fclose(f);
	}
	else {
		printf("Fisierul nu a putut fi deschis.\n");
		return 1;
	}
	printf("Lista initiala:\n");
	traversare_lista(lista);
	char cnp_de_sters[LUNGIME_CNP] = "8765456782";
	lista = stergere_nod(lista, cnp_de_sters);
	printf("\nLista dupa stergere:\n");
	traversare_lista(lista);
	NodD* temp = lista;
	int i = 1;
	printf("\nLista dupa interschimbare:\n");
	interschimbare(lista, i);
	traversare_lista(lista);
	while (temp != NULL) {
		NodD* next = temp->next;
		free(temp->student.nume);
		free(temp);
		temp = next;
	}
	return 0;
}