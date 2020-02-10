#include <stdio.h>
#include <inttypes.h>

void printbits(uint8_t s) {
  for (int i = (8 - 1); i >= 0; i--) {
    printf("%d", ((1ULL << i) & s) ? 1 : 0);
  }
}

int main(int argc, char *argv[]){
  uint8_t a = 73;
  uint8_t b = 99;
  uint8_t c = (a ^ b);

  printf("a = "); printbits(a); 
  printf("\nb = "); printbits(b); 
  printf("\nc = "); printbits(c); 
  printf("\n");

  printf("\n\n");

  printf(" a = "); printbits( a); 
  printf("\n~a = "); printbits(~a); 
  printf("\n");

  printf("\n\n");

  printf(" 1       = "); printbits( 1      );
  printf("\n(1 << 3) = "); printbits((1 << 3)); 
  printf("\n"); 

  printf("\n\n");

  printf(" 99       = "); printbits(            99                   ); 
  printf("\n(99 << 3) = "); printbits((((uint8_t) 99) << ((uint8_t) 3))); 
  printf("\n"); 

  printf("\n\n");

  printf("              99 = "); printbits(              99); 
  printf("\n(1ULL << 5)      = "); printbits((1ULL << 5)     ); 
  printf("\n(1ULL << 5) & 99 = "); printbits((1ULL << 5) & 99); 
  printf("\n"); 

  return 0;
}
