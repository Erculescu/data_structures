#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define LUNGIME_DATA 5
#define LUNGIME_HT 101

struct Student {
	int nr_mat;
	char* nume;
	float medie;
	char data_inscriere[LUNGIME_DATA];
};
typedef struct Student Student;

struct Nod {
	Student student;
	struct Nod* next;
};
typedef struct Nod Nod;

unsigned char functie_HT(int cheie, unsigned char dim_tabela) {
	return (cheie % dim_tabela);
}

Nod* adauga_SF(Nod* lista, Student student) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->student = student;
	nou->next = NULL;
	if (lista == NULL) {
		nou->next = nou;
		return nou;
	}
	else {
		Nod* temp = lista;
		while (temp->next != lista) {
			temp = temp->next;
		}
		temp->next = nou;
		nou->next = lista;
		return lista;
	}

}
unsigned char inserare_HT(Nod** tabela, Student student, unsigned char dim_tabela) {
	unsigned char poz = functie_HT(student.nr_mat, dim_tabela);
	tabela[poz] = adauga_SF(tabela[poz], student);
	return (poz + 1);
}


void traversare_lista(Nod* lista) {
	Nod* temp = lista;
	printf("Nr matricol: %d, Nume: %s, Medie: %5.2f, Data inscriere: %s\n", temp->student.nr_mat, temp->student.nume, temp->student.medie, temp->student.data_inscriere);
	temp = temp->next;
	while(temp!=lista){
		printf("Nr matricol: %d, Nume: %s, Medie: %5.2f, Data inscriere: %s\n", temp->student.nr_mat, temp->student.nume, temp->student.medie, temp->student.data_inscriere);
		temp = temp->next;
	}
}
void traversare_HT(Nod** tabela, unsigned char dim_tabela) {
	for (unsigned char i = 0; i < dim_tabela; i++) {
		if (tabela[i] != NULL) {
			printf("Pe pozitia %d se afla lista:\n", i);
			traversare_lista(tabela[i]);
		}
	}
}

void eliberare_lista(Nod* lista) {

	if (lista == NULL) {
		return;
	}
	Nod* temp = lista->next;
	Nod* next;
	while (temp != lista) {
		next = temp->next;
		free(temp->student.nume);
		free(temp);
		temp = next;
	}
	free(lista->student.nume);
	free(lista);

}
void eliberare_HT(Nod** tabela, unsigned char dim_tabela) {
	for (unsigned char i = 0; i < dim_tabela; i++) {
		if (tabela[i] != NULL) {
			eliberare_lista(tabela[i]);
			tabela[i] = NULL;
		}
		
	}
	free(tabela);
}



Nod* cautare_student(Nod* lista, int nr_mat) {
	Nod* temp = lista;
	if (temp == NULL) {
		return NULL;
	}
	if (temp->student.nr_mat == nr_mat) {
		return temp;
	}
	temp = temp->next;
	while (temp != lista) {
		if (temp->student.nr_mat == nr_mat) {
			return temp;
		}
		temp = temp->next;
	}
	return NULL;

}

void stergere_student(Nod* lista, int nr_mat) {
	Nod* temp = cautare_student(lista, nr_mat);
	if (temp == NULL) {
		printf("Studentul cu nr matricol %d nu exista in lista.\n", nr_mat);
		return;
	}
	if (temp == lista) {
		Nod* ultim = lista;
		while (ultim->next != lista) {
			ultim = ultim->next;
		}
		if (temp->next == lista) {
			free(temp->student.nume);
			free(temp);
			lista = NULL;
		}
		else {
			ultim->next = temp->next;
			free(temp->student.nume);
			free(temp);
			lista = ultim->next;

		}

	}
	else {
		Nod* temp2 = lista;
		while (temp2->next != temp) {
			temp2 = temp2->next;
		}
		temp2->next = temp->next;
		free(temp->student.nume);
		free(temp);

	}
}

void stergere_studenti(Nod** lista, float prag1, float prag2) {

	if (*lista == NULL) {
		printf("Lista este goala.\n");
		return;
	}
	Nod* curent = *lista;
	Nod* prev = NULL;
	Nod* ultim = *lista;
	while (ultim->next != *lista) {
		ultim = ultim->next;
	}
	while (curent != ultim || prev == NULL)
	{
		if (curent->student.medie<prag1 || curent->student.medie>prag2) {
			int nr_mat_curent = curent->student.nr_mat;
			printf("Studentul cu nr matricol %d nu este in interval.\n", nr_mat_curent);
			stergere_student(*lista, nr_mat_curent);
			if (curent == *lista) {
				*lista = ultim->next;
				curent = *lista;
				continue;
			}
			else {
				curent = prev->next;
			}

		}
		else {
			prev = curent;
			curent = curent->next;
		}
	}
	if (curent == ultim && curent->student.medie<prag1 || curent->student.medie>prag2) {
		int nr_mat_curent = curent->student.nr_mat;
		printf("Studentul cu nr matricol %d nu este in interval.\n", nr_mat_curent);
		stergere_student(*lista, nr_mat_curent);
		if (*lista == ultim) {
			*lista = NULL;
		}
	}
}

int main() {
	FILE* f = fopen("studenti.txt", "r");
	Student student;
	Nod* lista = NULL;
	
	int n;
	printf("Introduceti numarul de studenti:");
	scanf("%d", &n);

	char buffer[256];
	char* token, separator[] = ",";

	if (f != NULL) {
		int i = 0;
		while ((fgets(buffer, sizeof(buffer), f) != NULL)&&i<n) {
			token = strtok(buffer, separator);
			student.nr_mat = atoi(token);
			token = strtok(NULL, separator);
			student.nume = (char*)malloc(strlen(token) + 1);
			strcpy(student.nume, token);
			token = strtok(NULL, separator);
			student.medie = atof(token);
			token = strtok(NULL, separator);
			strcpy(student.data_inscriere, token);
			lista = adauga_SF(lista, student);
			i++;
		}
		fclose(f);
	}
	printf("Lista initiala:\n");
	traversare_lista(lista);
	float prag1 = 5, prag2 = 10;
	printf("Lista dupa stergere:\n");
	stergere_studenti(&lista, prag1, prag2);
	traversare_lista(lista);

	eliberare_lista(lista);

	Nod** HT = (Nod**)malloc(LUNGIME_HT * sizeof(Nod*));
	for (unsigned char i = 0; i < LUNGIME_HT; i++) {
		HT[i] = NULL;
	}
	f = fopen("studenti.txt", "r");
	if (f != NULL) {
		int i = 0;
		while ((fgets(buffer, sizeof(buffer), f) != NULL) && i < n) {
			token = strtok(buffer, separator);
			student.nr_mat = atoi(token);
			token = strtok(NULL, separator);
			student.nume = (char*)malloc(strlen(token) + 1);
			strcpy(student.nume, token);
			token = strtok(NULL, separator);
			student.medie = atof(token);
			token = strtok(NULL, separator);
			strcpy(student.data_inscriere, token);
			inserare_HT(HT, student, LUNGIME_HT);
			i++;

		}
		fclose(f);
	}
	printf("\nHash table:\n");
	traversare_HT(HT, LUNGIME_HT);
	eliberare_HT(HT, LUNGIME_HT);
	/*f = fopen("rezultat.txt", "w");
	if (f != NULL) {
		for (unsigned char i = 0; i < LUNGIME_HT; i++) {
			if (HT[i] != NULL) {
				Nod* temp = HT[i];
				fprintf(f, "Nr matricol: %d, Nume: %s, Medie: %5.2f, Data inscriere: %s\n", temp->student.nr_mat, temp->student.nume, temp->student.medie, temp->student.data_inscriere);
				temp = temp->next;
				while (temp != HT[i]) {
					fprintf(f, "Nr matricol: %d, Nume: %s, Medie: %5.2f, Data inscriere: %s\n", temp->student.nr_mat, temp->student.nume, temp->student.medie, temp->student.data_inscriere);
					temp = temp->next;
				}
				fprintf(f, "\n");
			}

		}
		fclose(f);
	}*/
	return 0;
}