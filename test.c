#include <stdio.h>
#include <stdlib.h>

typedef struct test {
  char *s;
} test;

int main(void) {
  test *t = (test *)malloc(sizeof(test));
  t->s = (char *)malloc(sizeof(char));
  char kamal[10] = "kamal";
  for (int i = 0; i < 500; i++) {
    t->s[i] = kamal[i];
  }
  printf("%s\n", t->s);
  return 0;
}
