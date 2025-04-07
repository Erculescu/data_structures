#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>


struct PackageDelivery {
	unsigned int bugid;
	char* reporterName;
	char* description;
	float severity;
	char resolved[2];
};

typedef struct PackageDelivery PackageDelivery;

struct NodD {
	PackageDelivery pachet;
	struct NodD* prev;
	struct NodD* next;
};
typedef struct NodD NodD;

struct Nod {
	PackageDelivery pachet;
	struct Nod* next;
};
typedef struct Nod Nod;


NodD* adauga_el(NodD* lista, PackageDelivery pachet) {
	NodD* nou = (NodD*)malloc(sizeof(NodD));
	nou->pachet = pachet;
	nou->prev = NULL;
	nou->next = NULL;
	if (lista == NULL) { return nou; }
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
	printf("Traversare lista de la inceput la sfarsit.\n");
	while (temp != NULL) {
		printf("Bug: %u, raportat de: %s, descriere: %s, severitate: %.2f, rezolvat? %s\n", temp->pachet.bugid, temp->pachet.reporterName,
			temp->pachet.description, temp->pachet.severity, temp->pachet.resolved);
		temp = temp->next;
	}

	printf("Traversare lista de la sfarsit la inceput.\n");
	temp = lista;
	while (temp->next != NULL) {
		temp = temp->next;
	}
	while (temp != NULL) {
		printf("Bug: %u, raportat de: %s, descriere: %s, severitate: %.2f, rezolvat? %s\n", temp->pachet.bugid, temp->pachet.reporterName,
			temp->pachet.description, temp->pachet.severity, temp->pachet.resolved);
		temp = temp->prev;
	}

}

void dezalocare_lista(NodD* lista) {
	NodD* temp=lista;
	while (temp != NULL) {
		NodD* de_sters = temp;
		temp = temp->next;
		free(de_sters->pachet.reporterName);
		free(de_sters->pachet.description);
		free(de_sters);
	}
}

int raport(NodD* lista, char* subiect) {
	NodD* temp = lista;
	int contor = 0;
	while (temp != NULL) {
		if (strcmp(temp->pachet.reporterName, subiect) == 0) {
			contor++;
		}
		temp = temp->next;
	}
	return contor;
}


void cleanup(NodD** lista) {
	NodD* temp = *lista;
	while (temp != NULL) {
		if (strcmp(temp->pachet.resolved, "Y") == 0) {
			NodD* de_sters = temp;
			if (temp->prev != NULL) {
				temp->prev->next = temp->next;
			}
			else {
				*lista = temp->next;
			}
			if (temp->next != NULL) {
				temp->next->prev = temp->prev;

			}
			else {
				temp->prev->next = NULL;
			}
			temp = temp->next;
			free(de_sters->pachet.reporterName);
			free(de_sters->pachet.description);
			free(de_sters);

		}
		else {
			temp = temp->next;
		}
		
	}
}

Nod* adauga_el_List_S(Nod* lista, PackageDelivery pachet) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->pachet = pachet;
	nou->next = NULL;
	if (lista == NULL) { return nou; }
	else {
		NodD* temp = lista;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = nou;
		return lista;
	}
}

void LowThreat(NodD* lista, float severity) {
	NodD* temp = lista;
	Nod* listaLow = NULL;
	while (temp != NULL) {
		if (temp->pachet.severity < severity) {
			listaLow = adauga_el_List_S(listaLow, temp->pachet);
		}
		temp = temp->next;
	}
	printf("\nLista simpla de bug-uri cu severitate scazuta:\n");
	Nod* temp2 = listaLow;
	while (temp2 != NULL) {
		printf("Bug: %u, raportat de: %s, descriere: %s, severitate: %.2f, rezolvat? %s\n", temp2->pachet.bugid, temp2->pachet.reporterName,
			temp2->pachet.description, temp2->pachet.severity, temp2->pachet.resolved);
		temp2 = temp2->next;
	}

}



int main() {

	FILE* f = fopen("bugs.txt", "r");
	PackageDelivery pachet;
	NodD* lista = NULL;

	char buffer[256];
	char* token, separator[] = ",";

	if (f != NULL) {
		while (fgets(buffer, sizeof(buffer), f) != NULL) {
			token = strtok(buffer, separator);
			pachet.bugid = atoi(token);
			token = strtok(NULL, separator);
			pachet.reporterName = (char*)malloc(strlen(token) + 1);
			strcpy(pachet.reporterName, token);
			token = strtok(NULL, separator);
			pachet.description = (char*)malloc(strlen(token) + 1);
			strcpy(pachet.description, token);
			token = strtok(NULL, separator);
			pachet.severity = atof(token);
			token = strtok(NULL, separator);
			strcpy(pachet.resolved, token);
			lista = adauga_el(lista, pachet);
		}
		fclose(f);
	}

	printf("\nLista de bug-uri:\n");
	traversare_lista(lista);
	char subiect[6] = "Rares";
	int contor = raport(lista, subiect);
	printf("\nNumarul de bug-uri raportate de %s este de: %d\n", subiect, contor);
	cleanup(&lista);
	traversare_lista(lista);
	LowThreat(lista, 5.0);
	dezalocare_lista(lista);
}