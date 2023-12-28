#include <stdio.h>
#include <stdlib.h>

int main(void) {
  char *s = malloc(4 * sizeof(char));
  printf("%c\n", s[6]);
  return 0;
}
