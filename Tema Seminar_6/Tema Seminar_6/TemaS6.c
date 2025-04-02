#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#define DIM_NR_CARD 16
#define DIM_DATA_EXPIRARE 5
#define MARIME_STACK 1000

struct CardBancar
{
	char* titular; // 4 bytes
	char nr_card[DIM_NR_CARD + 1]; // +1 pt byte terminator de string; 20 bytes (17 + 3 padding)
	float sold; // 4 bytes
	char* emitent; // 4 bytes
	char data_expirare[DIM_DATA_EXPIRARE + 1]; // LL/AA; 8 bytes (6 + 2 padding)
};

typedef struct CardBancar CardBancar;

struct Nod
{
	CardBancar cb;
	struct Nod* next;
};

typedef struct Nod Nod;

//inceput definire stiva + operatii de pop si push

struct StackFrame {
	Nod* prim;
	Nod* ultim;
};

typedef struct StackFrame StackFrame;
struct Stack {
	StackFrame stack[MARIME_STACK];
	int varf;
};
typedef struct Stack Stack;

void push(Stack* s, Nod* prim, Nod* ultim) {
	if (s->varf < MARIME_STACK-1) {
		s->stack[++(s->varf)].prim = prim;
		s->stack[s->varf].ultim = ultim;
	}
}
StackFrame pop(Stack* s) {
	StackFrame rez = { NULL,NULL };
	if(s->varf>=0)
	{
		rez = s->stack[s->varf--];
	}
	return rez;
}
//definire functii de sortare + partitionare a listei
Nod* partitionare(Nod* prim, Nod* ultim) {
	float pivot = ultim->cb.sold;
	Nod* i = prim;
	Nod* j = prim;
	while (j != ultim&&j!=NULL) {
		if (j->cb.sold < pivot) {
			
			CardBancar temp = i->cb;
			i->cb = j->cb;
			j->cb = temp;
			i = i->next;

		}
		j = j->next;
	}
	if (i != NULL) {
		CardBancar temp = i->cb;
		i->cb = ultim->cb;
		ultim->cb = temp;
	}
	return i;
}

void quickSort(Nod* prim) {
	if (!prim || !prim->next) {
		return;
	}
	Stack stack;
	stack.varf = -1;
	Nod* ultim = prim;
	while (ultim->next != NULL) {
		ultim = ultim->next;
	}
	push(&stack, prim, ultim);
	while (stack.varf >= 0) {
		StackFrame frame = pop(&stack);
		Nod* inceput = frame.prim;
		ultim = frame.ultim;
		if (inceput != ultim && inceput!=NULL && ultim!=NULL) {
			Nod* pivot = partitionare(inceput, ultim);
			
			if (pivot != inceput) {
				push(&stack, inceput, pivot);
			}
			if (pivot->next != ultim) {
				push(&stack, pivot->next, ultim);
			}
		}

	}
}

Nod* inserare_nod_sfarsit(Nod* lista, CardBancar data)
{
	// cautare sfarsit de lista
	// alocare nod
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	// populre nod cu data
	//nou->cb = data;//partajare temporara memorie heap pt titular si emitent
	//nou->next = NULL;//nu exista succesor nou, va deveni ultimul nod din lista
	nou->cb.titular = (char*)malloc(strlen(data.titular) + 1);
	strcpy(nou->cb.titular, data.titular);
	nou->cb.emitent = (char*)malloc(strlen(data.emitent) + 1);
	strcpy(nou->cb.emitent, data.emitent);
	strcpy(nou->cb.nr_card, data.nr_card);
	strcpy(nou->cb.data_expirare, data.data_expirare);
	nou->cb.sold = data.sold;
	nou->next = NULL;


	if (lista == NULL) {
		//nu exista niciun nod in lista simpla
		return nou;//sau lista=nou; astfel incat lista se returneaza la finalul functiei
	}
	
		//exista cel putin un nod
		//cautare sfarsit de lista
		Nod* temp = lista;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		//temp este plasat pe ultimul nod din lista simpla
		temp->next = nou;//legarea lui nou la sfarsitul listei simple


	
	// return lista actualizata daca este cazul
	return lista;
}
void traversare_listasimpla(Nod* lista) {
	Nod* temp = lista;
	while (temp) {
		printf("%s %s %.2f \n", temp->cb.nr_card, temp->cb.titular,temp->cb.sold);
		temp = temp->next; //actualizez temp pe nodul succesor
	}
}

Nod* stergere_inceput(Nod* lista) {
	if (lista) {
		//exista cel putin un nod in lista simpla
		Nod* temp = lista;
		lista = lista->next;//actualizare inceput de lista pe nodul 2(succesor)
		free(temp->cb.titular);//dezalocare extensie in heap pentru CardBancar
		free(temp->cb.emitent);//dezalocare extensie in heap pentru CardBancar
		free(temp);//dezalocare nod

	}
	return lista;
}

int main()
{
	FILE* f = NULL;
	f = fopen("CarduriBancare.txt", "r");

	CardBancar card;
	Nod* prim = NULL; // variabila pointer acces la lista simpla

	char temp_buff[100]; // buffer temporar pentru stocare linii preluate din fisier text
	fgets(temp_buff, sizeof(temp_buff), f);

	while (!feof(f))
	{
		strtok(temp_buff, "\n");
		card.titular = malloc(strlen(temp_buff) + 1);
		strcpy(card.titular, temp_buff);

		fgets(temp_buff, sizeof(temp_buff), f);
		strtok(temp_buff, "\n");
		strcpy(card.nr_card, temp_buff);

		fgets(temp_buff, sizeof(temp_buff), f);
		strtok(temp_buff, "\n");
		card.sold = (float)atof(temp_buff); // conversie ASCII-to-Binary pentru float

		fgets(temp_buff, sizeof(temp_buff), f);
		strtok(temp_buff, "\n");
		card.emitent = malloc(strlen(temp_buff) + 1);
		strcpy(card.emitent, temp_buff);

		fgets(temp_buff, sizeof(temp_buff), f);
		strtok(temp_buff, "\n");
		strcpy(card.data_expirare, temp_buff);

		// inserare date card in lista simpla
		prim = inserare_nod_sfarsit(prim, card);
		free(card.titular);
		free(card.emitent);

		// incercare preluare titular card urmator
		fgets(temp_buff, sizeof(temp_buff), f);
	}
	fclose(f);
	printf("Lista simpla dupa creare: \n");
	traversare_listasimpla(prim);
	// stergere nod la inceput
	prim = stergere_inceput(prim);
	printf("\nLista simpla dupa stergere nod la inceput: \n");
	traversare_listasimpla(prim);
	//implementare quicksort
	quickSort(prim);
	printf("\nLista simpla dupa sortare: \n");
	traversare_listasimpla(prim);


	//dezalocare lista simpla
	while (prim != NULL) {
		//exista cel putin un nod in lista simpla care poate fi sters din lista
		prim = stergere_inceput(prim);

	}
	/*printf("\nLista simpla dupa dezalocare: \n");
	traversare_listasimpla(prim);*/
	//algoritm quicksort
	
	// la interior (criteriu definit)

	return 0;
}