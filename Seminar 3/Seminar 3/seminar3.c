#include <stdio.h>
#include<malloc.h>
//lucru cu debugger-ul
//Continuare a seminar 2
#define NR_ELEMENTE 10
#define NR_LINII 10
#define NR_COLOANE 9

int main() {
	char x = 65;
	char y = -2;
	float z = 65.64f;

	char* p_char = NULL;//daca nu e null, adresa va fi cccc
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

	p_char = (char*)malloc((NR_ELEMENTE + 1) * sizeof(char));

	for (unsigned char i = 0; i < NR_ELEMENTE; i++) {
		p_char[i] = x + i;
	}
	p_char[NR_ELEMENTE] = 0;//scriere terminator de sir
	for (unsigned char i = 0; i < NR_ELEMENTE; i++) {
		printf("%c-->%d", p_char[i], p_char[i]);
		printf("\n");
	}
	printf("\n");
	printf("%s In memorie heap\n", p_char);
	free(p_char);
	p_char = NULL;
	//printf("%c\n", p_char[0]);// eroare la runtime, p_char e NULL
	char v[NR_ELEMENTE+1];
	p_char = v;//adresa lui p_char va fi adresa primului element din v
	for (unsigned char i = 0; i < NR_ELEMENTE; i++) {
		v[i] = x + i;
	}
	v[NR_ELEMENTE] = 0;
	printf("%s In memorie stack\n", v);

	char** M = NULL;
	//alocare nivel 1 al matricei (nr linii)
	M = (char**)malloc(NR_LINII * sizeof(char*));
	//alocare vector de pointeri la linii
	for (unsigned char i = 0; i < NR_LINII; i++) {
		//alocare nivel 2 al matricei (nr coloane)
		M[i] = (char*)malloc(NR_COLOANE * sizeof(char));//alocare memorie heap pentru linia i
														//adresa de inceput a liniei i este stocata in M[i]
														//se stocheaza lexicografic ,pt invers lexicografic, mai intai alocam coloanele
	}
	//initializare si utilizare (prelucrare) elemente matrice


	//dezalocare (se realizeaza in mod invers fata de alocare: nivel 2->nivel 1);
	for (unsigned char i = 0; i < NR_LINII; i++)
	{//nivel 2
		free(M[i]);//dezalocare vector de elemente char aferent liniei i
	}
	//nivel 1
	free(M);//dezalocare vector de adrese de inceput de linii 

	return 0;
}