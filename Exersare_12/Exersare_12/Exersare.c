#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

struct  Student {
	int nr_matricol;
	char* nume;
	float medie;
	int varsta;
};
typedef struct Student Student;
struct NodABC {
	Student stud;
	struct NodABC* stanga, * dreapta;
};
typedef struct NodABC NodABC;


NodABC* inserare_nod(NodABC* r, Student data, unsigned char* inserat) {
	if (r != NULL) {
		if (data.nr_matricol < r->stud.nr_matricol) {
			r->stanga = inserare_nod(r->stanga, data, inserat);
		}
		else {
			if (data.nr_matricol > r->stud.nr_matricol) {
				r->dreapta = inserare_nod(r->dreapta, data, inserat);
			}
			else {
				*inserat = 0;
			}
		}
	}
	else
	{
		r = malloc(sizeof(NodABC));
		r->stud = data;
		r->stanga = NULL;
		r->dreapta = NULL;
		*inserat = 1;
	}
	return r;
}

void traversare_inordine(NodABC* r) {
	if (r != NULL) {
		traversare_inordine(r->stanga);
		printf("\n|%d, %s, %.2f, %d|", r->stud.nr_matricol, r->stud.nume, r->stud.medie, r->stud.varsta);
		traversare_inordine(r->dreapta);
	}
}

NodABC* dezalocare(NodABC* r) {
	if (r != NULL) {
		r->stanga = dezalocare(r->stanga);
		r->dreapta = dezalocare(r->dreapta);
		free(r->stud.nume);
		free(r);
		r = NULL;
	}
	return r;
}

int inaltime(NodABC* r) {
	if (r == 0) { return -1; }
	else {
		int inaltime_stanga = inaltime(r->stanga);
		int inaltime_dreapta = inaltime(r->dreapta);
		if (inaltime_stanga > inaltime_dreapta)
		{
			return inaltime_stanga + 1;
		}
		else {
			return inaltime_dreapta + 1;
		}
	}
}

int noduri_pe_nivel(NodABC* r, int nivel) {
	if (r == NULL) {
		return 0;
	}
	if (nivel == 1) {
		return 1;
	}
	else {
		return noduri_pe_nivel(r->stanga, nivel - 1) + noduri_pe_nivel(r->dreapta, nivel - 1);
	}
	
}

NodABC* dezalocare_noduri_frunza(NodABC* r) {
	if (r != NULL) {
		if (r->stanga == NULL && r->dreapta == NULL) {
			free(r->stud.nume);
			free(r);
			return NULL;
		}
		else {
			r->stanga = dezalocare_noduri_frunza(r->stanga);
			r->dreapta = dezalocare_noduri_frunza(r->dreapta);
		}
	}
	return r;
}

NodABC* stergere_nod_ABC(NodABC* r, int cheie, unsigned char* flag) {
	if (r != NULL) {
		if (cheie < r->stud.nr_matricol) {
			r->stanga = stergere_nod_ABC(r->stanga, cheie, flag);
		}
		else {
			if (cheie > r->stud.nr_matricol) {
				r->dreapta = stergere_nod_ABC(r->dreapta, cheie, flag);
			}
			else {
				*flag = 1;
				if (r->stanga == NULL && r->dreapta == NULL) {
					free(r->stud.nume);
					free(r);
					r = NULL;
				}
				else {
					if (r->stanga != NULL && r->dreapta != NULL) {
						NodABC* succesor = r->dreapta;
						while (succesor->stanga != NULL) {
							succesor = succesor->stanga;
						}
						succesor->stud.nr_matricol = r->stud.nr_matricol;
						succesor->stud.nume = malloc(strlen(r->stud.nume) + 1);
						strcpy(succesor->stud.nume, r->stud.nume);
						succesor->stud.medie = r->stud.medie;
						succesor->stud.varsta = r->stud.varsta;
						free(r->stud.nume);
						r->dreapta = stergere_nod_ABC(r->dreapta, succesor->stud.nr_matricol, flag);
					}
					else {
						NodABC* temp = NULL;
						if (r->stanga != NULL) {
							temp = r->stanga;
						}
						else { temp = r->dreapta; }
						free(r->stud.nume);
						free(r);
						r = temp;
					}
				}
			}
		}
	}
	else {
		*flag = 0;
	}
	return r;
}

Student* cautare_student(NodABC* r, int cheie) {
	if (r != NULL) {
		if (cheie < r->stud.nr_matricol) {
			return cautare_student(r->stanga, cheie);
		}
		else {
			if (cheie > r->stud.nr_matricol) {
				return cautare_student(r->dreapta, cheie);
			}
			else {
				return &r->stud;
			}
		}
	}
	else {
		return NULL;
	}
}
 
int main() {
	FILE* f = fopen("studenti.txt", "r");
	Student student;
	NodABC* root = NULL;

	char buffer[256], separator[] = ",", *token;
	if (f != NULL) {
		while (fgets(buffer, sizeof(buffer), f) != NULL) {
			token = strtok(buffer, separator);
			student.nr_matricol = atoi(token);
			token = strtok(NULL, separator);
			student.nume = malloc(strlen(token) + 1);
			strcpy(student.nume, token);
			token = strtok(NULL, separator);
			student.medie = atof(token);
			token = strtok(NULL, separator);
			student.varsta = atoi(token);
			unsigned char inserat = 0;
			root = inserare_nod(root, student, &inserat);
			if (inserat != 0) {
				printf("Inserare cu succes a studentului cu nr matricol %d\n", student.nr_matricol);
			}
			else {
				printf("Inserare esuata!\n");
				free(student.nume);
			}
		}
		fclose(f);
	}

	printf("\n\n Traversare in ordine:\n");
	traversare_inordine(root);

	printf("\n\nArborele are inaltimea %d", inaltime(root));
	
	printf("\n\nNumarul de noduri pe nivelul 4 este %d", noduri_pe_nivel(root, 4));

	root = dezalocare_noduri_frunza(root);
	printf("\n\nArborele dupa dezalocarea nodurilor funza:\n");
	traversare_inordine(root);
	unsigned char flag;
	root = stergere_nod_ABC(root, 3, &flag);
	if (flag == 1) {
		printf("\n\nArborele dupa stergerea nodului cu nr matricol 3:\n");
		traversare_inordine(root);
	}
	else {
		printf("\n\nNodul cu nr matricol 3 nu a putut fi sters");
	}
	if (cautare_student(root, 4) != NULL) {
		printf("\n\nNodul cu cheia 4 a fost gasit in arborele binar de cautare %d", cautare_student(root, 4)->nr_matricol);
	}
	else {
		printf("\n\nNodul cu cheia 4 nu a fost gasit in arborele binar de cautare");
	}
	root=dezalocare(root);
	return 0;
}