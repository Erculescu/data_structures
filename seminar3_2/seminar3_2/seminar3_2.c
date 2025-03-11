#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>

#define DIM_NR_CARD 17
#define DIM_DATA_EX 5
#define NR_ELEM 3

struct CardBancar {
	char* titular;//4 bytes
	char nr_card[DIM_NR_CARD];//17 bytes + 3 bytes padding
	float sold;//4 bytes
	char data_expirare[DIM_DATA_EX+1]; //formatul este LL/AA 8bytes
	char* emitent;//4 bytes


};

typedef struct CardBancar CardBancar;

int main() {
	
	CardBancar v_card[NR_ELEM];// sau struct CardBancar v_card[NR_ELEM];
	CardBancar* p_card = NULL;
	printf("Dimensiunea vectorului v_card este %d\n", sizeof(v_card));
	printf("Dimensiune structura CardBancar este %d\n", sizeof(CardBancar));
	printf("Dimensiune pointer la structura CardBancar este %d\n", sizeof(p_card));
	// ! ! TEMA: EXEMPLU DE POPULARE A VECTORULUI V_CARD CU DATE DINTR-UN FISIER TEXT SAU BINAR ! !
	/*FILE* fisier;
	fisier = fopen("carduri.bin", "rb");
	if (fisier == NULL) {
		printf("Eroare la deschiderea fisierului\n");
		return 1;
	}
	int lungime;
	for (int i = 0; i < NR_ELEM; i++) {
		fread(&v_card[i].titular, sizeof(char*), 1, fisier);
		fread(&lungime, sizeof(int), 1, fisier);
		v_card[i].titular = (char*)malloc(lungime);
		printf("Lungimea citita este %d\n", lungime);
		if (v_card[i].titular == NULL) {
			printf("eroare la alocarea memoriei\n");
			fclose(fisier);
			return 2;
		}
		fread(v_card[i].titular, sizeof(char), lungime, fisier);
		v_card[i].titular[lungime] = '\0';
		fread(&v_card[i].nr_card, sizeof(char), DIM_NR_CARD, fisier);
		fread(&v_card[i].sold, sizeof(float), 1, fisier);
		fread(&v_card[i].data_expirare, sizeof(char), DIM_DATA_EX, fisier);
		fread(&lungime, sizeof(int), 1, fisier);
		v_card[i].emitent = (char*)malloc(lungime);
		if (v_card[i].emitent == NULL) {
			printf("eroare la alocarea memoriei\n");
			fclose(fisier);
			return 3;
		}
		fread(v_card[i].emitent, sizeof(char), lungime, fisier);
		v_card[i].emitent[lungime] = '\0';
		fread(&v_card[i].emitent, sizeof(char*), 1, fisier);
	}
	fclose(fisier);
	for (int i = 0; i < NR_ELEM; i++) {
		printf("Titular: %s\n", v_card[i].titular);
		printf("Numar card: %s\n", v_card[i].nr_card);
		printf("Sold: %5.2f\n", v_card[i].sold);
		printf("Data expirare: %s\n", v_card[i].data_expirare);
		printf("Emitent: %s\n", v_card[i].emitent);
	}
	for (int i = 0; i < NR_ELEM; i++) {
		free(v_card[i].titular);
		free(v_card[i].emitent);
	}*/

	//de verificat cu debuggerul cum se citesc elementele din fisier printr-o functie care populeaza fisierul binar.
	FILE* f = NULL;
	f = fopen("carduriBancare.txt", "r");
	char buffer_temp[100];//buffer temporar pentru stocare linii preluate din fisier text
	fgets(buffer_temp,sizeof(buffer_temp), f);
	unsigned char contor = 0;//nr efectiv de elemente din vectorul v_card
	while (!feof(f)&&contor<NR_ELEM) {//exista card bancar si vectorul are spatiu pt a stoca elementul
		strtok(buffer_temp, "\n");
		v_card[contor].titular = (char*)malloc(strlen(buffer_temp) + 1);//alocare mem heap titular element contor
		strcpy(v_card[contor].titular, buffer_temp);//copiere din buffer in mem heap

		fgets(buffer_temp, sizeof(buffer_temp), f);
		strtok(buffer_temp, "\n");
		strcpy(v_card[contor].nr_card, buffer_temp);
		
		fgets(buffer_temp, sizeof(buffer_temp), f);
		strtok(buffer_temp, "\n");
		v_card[contor].sold = (float)atof(buffer_temp);
		
		fgets(buffer_temp, sizeof(buffer_temp), f);
		strtok(buffer_temp, "\n");
		strcpy(v_card[contor].data_expirare, buffer_temp);
		
		fgets(buffer_temp, sizeof(buffer_temp), f);
		strtok(buffer_temp, "\n");
		v_card[contor].emitent = (char*)malloc(strlen(buffer_temp) + 1);
		strcpy(v_card[contor].emitent, buffer_temp);
		contor++;
		
		fgets(buffer_temp, sizeof(buffer_temp), f);
	}
	p_card = v_card;
	for (unsigned char i = 0; i < contor; i++) {
		printf("Titular: %s\n", v_card[i].titular);
		printf("Numar card: %s\n", v_card[i].nr_card);
		printf("Sold: %5.2f\n", v_card[i].sold);
		printf("Data expirare: %s\n", v_card[i].data_expirare);
		printf("Emitent: %s\n", v_card[i].emitent);
		printf("\n");
	}
	printf("\n______________________________\n");
	printf("\nCarduri bancare stocate in heap\n");
	for (unsigned char i = 0; i < contor; i++) {
		printf("Titular: %s\n", p_card[i].titular);
		printf("Numar card: %s\n", p_card[i].nr_card);
		printf("Sold: %5.2f\n", p_card[i].sold);
		printf("Data expirare: %s\n", p_card[i].data_expirare);
		printf("Emitent: %s\n", p_card[i].emitent);
		printf("\n");
	}
	//p_card alocat in memorie heap
	p_card = malloc(contor * sizeof(CardBancar));
	for (unsigned char i = 0; i < contor; i++) {
		p_card[i] = v_card[i];//temporar asumata partajarea de zone heap pt titular si emitent
		//alocare si populare zone separate de heap pt titular si emitent
		p_card[i].titular = (char*)malloc(strlen(v_card[i].titular) + 1);
		strcpy(p_card[i].titular, v_card[i].titular);
		p_card[i].emitent = (char*)malloc(strlen(v_card[i].emitent) + 1);
		strcpy(p_card[i].emitent, v_card[i].emitent);
	}

	printf("\n______________________________\n");
	for (unsigned char i = 0; i < contor; i++) {
		printf("Titular: %s\n", p_card[i].titular);
		printf("Numar card: %s\n", p_card[i].nr_card);
		printf("Sold: %5.2f\n", p_card[i].sold);
		printf("Data expirare: %s\n", p_card[i].data_expirare);
		printf("Emitent: %s\n", p_card[i].emitent);
		printf("\n");
	}

	//dezalocare vectori si structuri alocate dinamic
	for (unsigned char i = 0; i < contor; i++) {
		free(v_card[i].titular);
		free(v_card[i].emitent);
	}

	//PAS 1: dezalocarea zonelor cele mai indepartate
	for (unsigned char i = 0; i < contor; i++) {
		free(p_card[i].titular);
		free(p_card[i].emitent);
	}
	//PAS 2: dezalocarea vectorului de structuri
	free(p_card);


	fclose(f);

	return 0;
}