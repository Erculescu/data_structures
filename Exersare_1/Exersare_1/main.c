#define _CRT_SECURE_NO_WARNINGS
#include "sortare.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
	int n;
	printf("Introduceti numarul de elemente: ");
	scanf("%d", &n);
	int* v = (int*)malloc(n * sizeof(int));
	for (int i = 0; i < n; i++) {
		printf("v[%d]=", i);
		scanf("%d", &v[i]);
	}
	
	sortare(v, n);
	printf("\nVectorul sortat este: ");

	for (int i = 0; i < n; i++) {
		printf("%d ", v[i]);
	}
	free(v);
	return 0;
}