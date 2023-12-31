#include <stdio.h>
#include <stdlib.h>

typedef struct Block {
  int blockNumber;  // num de block
  int ocupiedSpace; // in chars
  char Contacts[120];
  struct Block *nextBlock;
} Block;
int main(void) {
  FILE *file = fopen("Contact_index.bin", "r");
  char *buffer = malloc(100 * sizeof(char));
  char *block = malloc(sizeof(char));
  fscanf(file, "%s,%s\n", buffer, block);
  printf("%s\n", buffer);
  printf("%s\n", block);
  return 0;
}
