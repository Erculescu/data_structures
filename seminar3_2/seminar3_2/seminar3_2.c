#include<stdio.h>

#define DIM_NR_CARD 17
#define DIM_DATA_EX 5
#define NR_ELEM 30

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

	return 0;
}