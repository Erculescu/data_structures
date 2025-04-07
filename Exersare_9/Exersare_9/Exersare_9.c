#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <stdlib.h>
#include<string.h>

struct PackageDelivery {
	unsigned int trackingN;
	char* recipientName;
	char* address;
	float weight;
	char delivered[2];
};
typedef struct PackageDelivery PackageDelivery;

struct Nod {
	PackageDelivery pachet;
	struct Nod* next;
};
typedef struct Nod Nod;

Nod* adauga_pachet(Nod* lista, PackageDelivery pachet) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->pachet = pachet;
	nou->next = NULL;
	if (lista == NULL) {
		return nou;
	}
	else {
		Nod* temp = lista;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = nou;
		return lista;
	}

}

void traversare_pachete(Nod* lista) {
	Nod* temp = lista;
	while (temp != NULL) {
		printf("Pachetul %d, cu destinatarul %s, la adresa %s, cu greutatea %.2f kg, a fost livrat:%s\n",temp->pachet.trackingN,temp->pachet.recipientName
		,temp->pachet.address,temp->pachet.weight,temp->pachet.delivered);
		temp = temp->next;
	}

}

unsigned char functie_hash(unsigned int cheie, unsigned int dimensiune) {
	return (cheie % dimensiune);
}

void adauga_pachet_HT(Nod** tabela, PackageDelivery pachet, unsigned int dimensiune) {
	unsigned char poz = functie_hash(pachet.trackingN, dimensiune);
	tabela[poz] = adauga_pachet(tabela[poz], pachet);
}

void traversare_HT(Nod** tabela, unsigned int dimensiune) {
	for (unsigned int i = 0; i < dimensiune; i++) {
		if (tabela[i] != NULL) {
			printf("Lista %d:\n", i);
			traversare_pachete(tabela[i]);
		}
	}
}

int nr_livrari(Nod** tabela, char* client,unsigned int n) {
	int contor = 0;
	for (int i = 0; i < n; i++) {
		if (tabela[i] != NULL) {
			Nod* temp = tabela[i];

			while (temp != NULL) {
				if (strcmp(temp->pachet.recipientName, client) == 0 && strcmp(temp->pachet.delivered, "Y") == 0) {
					contor++;
				}
				temp = temp->next;
			}
		}
	}
	return contor;
}

void stergere_comenzi(Nod** tabela, unsigned int dimensiune) {
	for (unsigned int i = 0; i < dimensiune; i++) {
		if (tabela[i] != NULL) {
			Nod* temp = tabela[i];
			Nod* prev = NULL;
			while (temp != NULL) {
				if (strcmp(temp->pachet.delivered, "Y") == 0) {
					Nod* de_sters = temp;
					if (prev == NULL) {
						tabela[i] = temp->next;
					}
					else {
						prev->next = temp->next;
					}
					temp = temp->next;
					free(de_sters->pachet.recipientName);
					free(de_sters->pachet.address);
					free(de_sters);
				}
				else {
					prev = temp;
					temp = temp->next;
				}
			}
		}
	}
}

void vector_greutate(Nod** tabela, unsigned int dimensiune, float greutate) {
	unsigned int nr_el = 0;
	for (unsigned int i = 0; i < dimensiune; i++) {
		if (tabela[i] != NULL) {
			Nod* temp = tabela[i];
			while (temp != NULL) {
				if (temp->pachet.weight > greutate) {
					nr_el++;
				}
				temp = temp->next;
			}
		}
	}
	Nod** vector = (Nod**)malloc(nr_el * sizeof(Nod*));
	for (unsigned int i = 0; i < nr_el; i++) {
		vector[i] = NULL;
	}

	for (unsigned int i = 0; i < dimensiune; i++) {
		if (tabela[i] != NULL) {
			Nod* temp = tabela[i];
			while (temp != NULL) {
				if (temp->pachet.weight > greutate) {
					adauga_pachet_HT(vector, temp->pachet, nr_el);
				}
				temp = temp->next;
			}
		}
	}
	traversare_HT(vector, nr_el);
}


int main() {

	FILE* f = fopen("pachete.txt", "r");
	PackageDelivery pachet;
	
	printf("Introduceti numarul de pachete: ");
	unsigned int n;
	scanf("%d", &n);

	Nod** HT = (Nod**)malloc(n * sizeof(Nod*));
	for (unsigned int i = 0; i < n; i++) {
		HT[i] = NULL;
	}
	char buffer[256];
	char* token, separator[] = ",";
	unsigned int i = 0;
	if (f != NULL) {
		while ((fgets(buffer, sizeof(buffer), f) != NULL)&&i<n) {
			token = strtok(buffer, separator);
			pachet.trackingN = atoi(token);
			token = strtok(NULL, separator);
			pachet.recipientName = (char*)malloc(strlen(token) + 1);
			strcpy(pachet.recipientName, token);
			token = strtok(NULL, separator);
			pachet.address = (char*)malloc(strlen(token) + 1);
			strcpy(pachet.address, token);
			token = strtok(NULL, separator);
			pachet.weight = atof(token);
			token = strtok(NULL, separator);
			strcpy(pachet.delivered, token);
			adauga_pachet_HT(HT, pachet, n);
			i++;
		}
		fclose(f);
	}
	printf("Tabela de dispersie a pachetelor:\n");
	traversare_HT(HT, n);
	char client[12] = "Rares Mihai";
	int livrari = nr_livrari(HT, client,n);
	printf("\n");
	printf("Clientul %s are %d livrari inregistrate.\n", client, livrari);
	stergere_comenzi(HT, n);
	printf("\n");
	printf("Tabela de dispersie a pachetelor dupa stergere:\n");
	traversare_HT(HT, n);
	printf("\n");
	printf("Tabela de dispersie noua cu greutatea filtrata:\n");
	float greutate = 10.0;
	vector_greutate(HT, n, greutate);
	

}