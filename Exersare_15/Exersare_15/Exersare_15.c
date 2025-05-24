#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>

struct camera {
	char serie[10];
	char* denumire;
	float pret;
	char an_fabricatie[5];
};
typedef struct camera camera;

struct NodABC {
	camera camera;
	struct NodABC* stanga, * dreapta;
};
typedef struct NodABC NodABC;

NodABC* inserare_Nod(NodABC* r, camera data, unsigned char* flag) {
	if (r != NULL) {
		if (strcmp(data.serie, r->camera.serie) < 0) {
			r->stanga = inserare_Nod(r->stanga, data, flag);
		}
		else {
			if (strcmp(data.serie, r->camera.serie) > 0) {
				r->dreapta = inserare_Nod(r->dreapta, data, flag);
			}
			else {
				*flag = 0;
			}
		}
	}
	else {
		r = malloc(sizeof(NodABC));
		r->camera = data;
		r->stanga = NULL;
		r->dreapta = NULL;
		*flag = 1;
	}
	return r;
}

void traversare_inordine(NodABC* r) {
	if (r != NULL) {
		traversare_inordine(r->stanga);
		printf("%s %s %.2f %s\n", r->camera.serie, r->camera.denumire, r->camera.pret, r->camera.an_fabricatie);
		traversare_inordine(r->dreapta);
	}
}

NodABC* dezalocare(NodABC* r) {
	if (r != NULL) {
		r->stanga = dezalocare(r->stanga);
		r->dreapta = dezalocare(r->dreapta);
		free(r->camera.denumire);
		free(r);
		r = NULL;
	}
	return r;
}

int inaltime(NodABC* r) {
	if (r == NULL) {
		return 0;
	}
	else {
		int stanga = inaltime(r->stanga), dreapta = inaltime(r->dreapta);
		if (stanga < dreapta) {
			return dreapta + 1;
		}
		else {
			return stanga + 1;
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
			free(r->camera.denumire);
			free(r);
			r = NULL;
		}
		else {
			r->stanga = dezalocare_noduri_frunza(r->stanga);
			r->dreapta = dezalocare_noduri_frunza(r->dreapta);
		}
	}
	return r;
}

NodABC* stergere_Nod(NodABC* r, char* cheie, unsigned char* flag) {
	if (r != NULL) {
		if (strcmp(cheie, r->camera.serie) < 0) {
			r->stanga = stergere_Nod(r->stanga, cheie, flag);
		}
		else {
			if (strcmp(cheie, r->camera.serie) > 0) {
				r->dreapta = stergere_Nod(r->dreapta, cheie, flag);
			}
			else {
				*flag = 1;
				if (r->stanga == NULL && r->dreapta == NULL) {
					free(r->camera.denumire);
					free(r);
					r = NULL;
				}
				else {
					if (r->stanga != NULL && r->dreapta != NULL) {
						NodABC* succesor = r->dreapta;
						while (succesor->stanga != NULL) {
							succesor = succesor->stanga;
						}
						strcpy(succesor->camera.serie, r->camera.serie);
						succesor->camera.denumire = malloc(strlen(r->camera.denumire) + 1);
						strcpy(succesor->camera.denumire, r->camera.denumire);
						succesor->camera.pret = r->camera.pret;
						strcpy(succesor->camera.an_fabricatie, r->camera.an_fabricatie);
						free(r->camera.denumire);
						r->dreapta = stergere_Nod(r->dreapta, succesor->camera.serie, flag);
					}
					else {
						NodABC* temp = NULL;
						if (r->stanga != NULL) {
							temp = r->stanga;
						}
						else { temp = r->dreapta; }
						free(r->camera.denumire);
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


camera* cautare_camera(NodABC* r, char* cheie) {
	if (r != NULL) {
		if (strcmp(cheie, r->camera.serie) < 0) {
			return cautare_camera(r->stanga, cheie);
		}
		else {
			if (strcmp(cheie, r->camera.serie) > 0) {
				return cautare_camera(r->dreapta, cheie);
			}
			else {
				return &r->camera;
			}
		}
	}
	else {
		return NULL;
	}
}

int main() {
	FILE* f = fopen("camere.txt", "r");
	camera Camera;
	NodABC* root = NULL;
	char buffer[256], * token, separator[] = ",";
	if (f != NULL) {
		while (fgets(buffer, sizeof(buffer), f) != NULL) {
			token = strtok(buffer, separator);
			strcpy(Camera.serie, token);
			token = strtok(NULL, separator);
			Camera.denumire = malloc(strlen(token) + 1);
			strcpy(Camera.denumire, token);
			token = strtok(NULL, separator);
			Camera.pret = atof(token);
			token = strtok(NULL, separator);
			strcpy(Camera.an_fabricatie, token);
			unsigned char flag = 1;
			root = inserare_Nod(root, Camera, &flag);
			if (flag != 0) {
				printf("\nCamera inregistrata cu succes.\n");
			}
			else {
				printf("\nCamera nu a putut fi inregistrata.\n");
			}
		}
		fclose(f);
	}
	printf("\nCamerele inregistrate sunt:\n");
	traversare_inordine(root);

	printf("\nInaltimea arborelui este: %d\n", inaltime(root));

	printf("\nExista %d noduri pe nivelul 2.\n", noduri_pe_nivel(root, 2));

	unsigned char flag;
	root = stergere_Nod(root, "3214567890", &flag);
	if (flag == 1) {
		printf("\nCamera cu seria 3214567890 a fost stearsa.\n");
		traversare_inordine(root);
	}
	else {
		printf("\nCamera cu seria 3214567890 nu a fost gasita.\n");
	}

	root = dezalocare_noduri_frunza(root);
	printf("\nArborele dupa eliminarea nodurilor frunza:\n");
	traversare_inordine(root);

	camera* cautata = cautare_camera(root, "1234567890");
	if (cautata != NULL) {
		printf("\nCamera cautata este: %s\n", cautata->denumire);
	}
	else {
		printf("\nCamera cautata nu a fost gasita.\n");
	}

	root = dezalocare(root);
	printf("\nAroborele dupa eliberarea memoriei:\n");
	traversare_inordine(root);
}