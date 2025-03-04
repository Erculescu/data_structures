#include <stdio.h>
#include<malloc.h>
//lucru cu debugger-ul

#define NR_ELEMENTE 10

int main() {
	char x = 65;
	char y = -2;
	float z = 65.64f;

	char* p_char= NULL;//daca nu e null, adresa va fi cccc
	printf("Adresa stack pointer lui p_char: %p\n", &p_char);
	printf("Continutul pointer-ului lui p_char: %p\n", p_char);
	/*printf("Continut zona pointata de p_char: %d\n", *p_char); !! Eroare la runtime, adresa nu exisa/este incorecta !!*/

	p_char = &x;

	printf("Noua adresa stack pointer lui p_char: %p\n", &p_char);
	printf("Noul continut al pointer-ului lui p_char: %p\n", p_char);
	printf("Continutul corect zona pointata de p_char: %d\n", *p_char);//p_char[0] in loc de *p_char

	//&y = &x; adresa nu poate fi suprascrisa
	p_char = (char*)malloc(sizeof(char));//alocare memorie heap

	printf("Noua adresa stack pointer lui p_char: %p\n", &p_char);
	printf("Noul continut al pointer-ului lui p_char: %p\n", p_char);
	printf("Continutul corect zona pointata de p_char: %d\n", *p_char);
	
	p_char[0] = x;//p_char[0]<=>*p_char=x
	printf("Noua adresa stack pointer lui p_char: %p\n", &p_char);
	printf("Noul continut al pointer-ului lui p_char: %p\n", p_char);
	printf("Continutul corect zona pointata de p_char: %d\n", *p_char);

	free(p_char);//dezalocare memorie heap
	//dangling pointer!! fara NULL

	printf("Noua adresa stack pointer lui p_char: %p\n", &p_char);
	printf("Noul continut al pointer-ului lui p_char: %p\n", p_char);
	printf("Continutul corect zona pointata de p_char: %d\n", *p_char);	
	p_char = NULL;

	p_char = (char*)malloc(NR_ELEMENTE * sizeof(char));

	for (unsigned char i = 0; i < NR_ELEMENTE; i++) {
		p_char[i] = x + i;
	}
	for (unsigned char i = 0; i < NR_ELEMENTE; i++) {
		printf("%c-->%d", p_char[i],p_char[i]);
		printf("\n");
	}
	printf("\n");
	printf("%s\n", p_char);

	return 0;
}