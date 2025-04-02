#define _CRT_SECURE_NO_WARNINGS


#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>

#define LUNGIME_CNP 10
#define DIM_TAB_HT 50

struct  Student
{
	char cnp[LUNGIME_CNP];
	char* nume;
	int varsta;
	float medie;

};

typedef struct Student Student;

struct Nod {
	Student stud;
	struct Nod* next;
};

typedef struct Nod Nod;

unsigned char functie_hash(char* cheie, unsigned char dim_tabela) {
	unsigned int suma_ascii = 0;
	for (unsigned char i = 0; i < strlen(cheie); i++) {
		suma_ascii += cheie[i];
	}
	return (suma_ascii % dim_tabela);
}

Nod* inserare_nod_sf(Nod* lista, Student stud) {

	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->stud = stud;
	nou->next = NULL;
	if (lista == NULL) {
		return nou;
	}
	else {
		Nod* temp = lista;
		while (temp->next) {
			temp=temp->next;
		}
		temp->next = nou;
	}
	return lista;
}

unsigned char inserare_HT(Nod** tabela, unsigned char dim_tab, Student stud) {
	unsigned char poz = functie_hash(stud.cnp, dim_tab);
	tabela[poz] = inserare_nod_sf(tabela[poz], stud);
	return (poz+1);
}

Student* cautare_HT(char* cheie, Nod** tabela, unsigned char dim_tab) {
	unsigned char poz = functie_hash(cheie, dim_tab);
	Nod* temp = tabela[poz];
	while (temp) {
		if (strcmp(temp->stud.cnp, cheie) == 0) {
			return &(temp->stud);
		}
		temp = temp->next;
	}
	return NULL;
}


void traversare_lista(Nod* lista) {
	Nod* temp = lista;
	while (temp) {
		printf("Studentul %s are varsta %d si media %5.2f\n", temp->stud.nume, temp->stud.varsta, temp->stud.medie);
		temp = temp->next;
	}
}

void traversare_HT(Nod** tabela, unsigned char dim_tab) {
	for (unsigned char i = 0; i < dim_tab; i++) {
		if (tabela[i] != NULL) {
			printf("Pozitia %d din hash table contine:\n", i);
			traversare_lista(tabela[i]);
		}
	}
}


void dezalocare_lista(Nod* lista) {
	Nod* temp = lista;
	while (temp) {
		Nod* temp2 = temp;
		temp = temp->next;
		free(temp2->stud.nume);
		free(temp2);
	}

}

int stergere_HT(char* cheie, Nod** tabela, unsigned char dim_tab) {
	unsigned char poz = functie_hash(cheie, dim_tab);
	Nod* temp = tabela[poz];
	Nod* prev = NULL;
	
	while (temp != NULL) {
		if (strcmp(cheie, temp->stud.cnp) == 0) {
			if (prev == NULL) {
				tabela[poz] = temp->next;
			}
			else {
				prev->next = temp->next;
			}
			free(temp->stud.nume);
			free(temp);
			return 1;
		}
		prev = temp;
		temp = temp->next;
	}
	return 0;
}


int main() {

	FILE* f = NULL;
	f = fopen("C:\\Users\\ercul\\source\\repos\\SDD\\Exersare_2\\Exersare_2\\studenti.txt", "r");

	Student stud;
	Nod** HT = NULL;

	HT = (Nod**)malloc(DIM_TAB_HT * sizeof(Nod*));
	for (unsigned char i = 0; i < DIM_TAB_HT; i++) {
		HT[i] = NULL;
	}
	char buffer[256];
	char* token, separator[] = ",";
	if (f != NULL) {
		while (fgets(buffer, sizeof(buffer), f)) {
			token = strtok(buffer, separator);
			strcpy(stud.cnp, token);
			token = strtok(NULL, separator);
			stud.nume = (char*)malloc((strlen(token) + 1) * sizeof(char));
			strcpy(stud.nume, token);
			token = strtok(NULL, separator);
			stud.varsta = atoi(token);
			token = strtok(NULL, separator);
			stud.medie = atof(token);
			inserare_HT(HT, DIM_TAB_HT, stud);

		}
		fclose(f);
	}
	else {
		printf("Fisierul nu a putut fi deschis");
	}

	printf("Traversare hash table\n");
	traversare_HT(HT, DIM_TAB_HT);

	if (stergere_HT("223454323", HT, DIM_TAB_HT) == 1) {
		printf("Studentul a fost sters din HT.\n");
	}
	else {
		printf("Studentul nu a fost sters din HT.\n");
	}
	

	for (unsigned char i = 0; i < DIM_TAB_HT; i++) {
		if (HT[i] != NULL)
			dezalocare_lista(HT[i]);
	}
	free(HT);


	return 0;
}
