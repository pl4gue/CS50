#include <stdio.h>
#include <stdlib.h>

int main() {
  int tamanho;

  do {
    printf("Qual o tamanho da pirâmide desejas imprimir?");
    scanf("%d", &tamanho);
  } while (tamanho <= 0 || tamanho > 8);

  for (int i = 1; i <= tamanho; i++) {
    for (int j = 1; j <= tamanho; j++) {
      char caracter = (j > tamanho - i) ? '#' : ' ';

      printf("%c", caracter);
    }

    printf("  ");

    for (int j = tamanho; j > 0; j--) {
      char caracter = (j > tamanho - i) ? '#' : '\n';

      printf("%c", caracter);
      if (caracter == '\n') break;
    }
  }
  printf("\n");
}