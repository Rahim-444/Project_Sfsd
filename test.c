#include <stdio.h>

int binarySearch(int T[], int value, int n) {
  int low = 0;
  int high = n - 1;
  int m = (low + high) / 2;
  while (low <= high) {
    if (T[m] == value)
      return m;
    if (T[m] > value)
      high = m - 1;
    else
      low = m + 1;
    m = (low + high) / 2;
  }
  return -1;
}

int main() {
  int T[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  int n = sizeof(T) / sizeof(T[0]);
  int value;
  printf("please enter a value : ");
  scanf("%d", &value);
  int index = binarySearch(T, value, n);
  printf("%d\n", index);
}
