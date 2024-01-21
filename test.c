#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_OTHER_INFO_SIZE 250

typedef struct Contact {
  int x;
  bool isDeleted;
  char iD[9];
  char name[31];
  char phoneNumber[11];
  char email[31];
  char otherInfo[MAX_OTHER_INFO_SIZE];
  struct Contact *next;
} Contact;

void merge(Contact arr[], int left, int mid, int right) { // not_same
  int i, j, k;
  int n1 = mid - left + 1;
  int n2 = right - mid;

  // Create temporary arrays
  Contact L[n1], R[n2];

  // Copy data to temporary arrays L[] and R[]
  for (i = 0; i < n1; i++)
    L[i] = arr[left + i];
  for (j = 0; j < n2; j++)
    R[j] = arr[mid + 1 + j];

  // Merge the temporary arrays back into arr[left..right]
  i = 0;
  j = 0;
  k = left;
  while (i < n1 && j < n2) {
    // Compare based on the iD field, adjust as needed
    if (L[i].iD <= R[j].iD) {
      arr[k] = L[i];
      i++;
    } else {
      arr[k] = R[j];
      j++;
    }
    k++;
  }

  // Copy the remaining elements of L[], if there are any
  while (i < n1) {
    arr[k] = L[i];
    i++;
    k++;
  }

  // Copy the remaining elements of R[], if there are any
  while (j < n2) {
    arr[k] = R[j];
    j++;
    k++;
  }
}

void mergeSort(Contact *arr, int left, int right) { // not_same
  if (left < right) {
    // Same as (left+right)/2, but avoids overflow for large left and right
    int mid = left + (right - left) / 2;

    // Sort first and second halves
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);

    // Merge the sorted halves
    merge(arr, left, mid, right);
  }
}

void afficherTableau(Contact *tableau, int tailleTableau) { // not_same
  printf("\nTableau :\n");
  for (int i = 0; i < tailleTableau; i++) {
    printf("ID: %08s, Name: %s, Phone: %s, Email: %s, Otherinfos: %s.\n",
           tableau[i].iD, tableau[i].name, tableau[i].phoneNumber,
           tableau[i].email, tableau[i].otherInfo);
  }
}

Contact *BinFile_to_tab(int *tailleTableau) { // not-same

  FILE *file = fopen("Contacts.bin", "rb");
  Contact tempContact;

  if (file != NULL) {
    //*tailleTableau = 0; // Initialiser la taille du tableau

    while (fscanf(file, "%1d,%08s,%30s,%10s,%30s,%250s", &tempContact.x,
                  tempContact.iD, tempContact.name, tempContact.phoneNumber,
                  tempContact.email, tempContact.otherInfo) == 6) {
      (*tailleTableau)++;
    }
    printf("taille tab : %d\n", *tailleTableau);

    rewind(file); // Rembobiner le fichier
    // Allouer de la mémoire pour le tableau
    Contact *tableau = (Contact *)malloc((*tailleTableau) * sizeof(Contact));

    if (tableau == NULL) {
      fprintf(stderr, "Erreur d'allocation de mémoire\n");
      exit(EXIT_FAILURE);
    }
    // Lire les données du fichier dans le tableau
    for (int i = 0; i < *tailleTableau; i++) {

      fscanf(file, "%1d,%08s,%30s,%10s,%30s,%250s", &tableau[i].x,
             tableau[i].iD, tableau[i].name, tableau[i].phoneNumber,
             tableau[i].email, tableau[i].otherInfo);
      printf("%8s\n", tableau[i].iD);
    }
    return tableau;
    perror("Erreur lors de l'ouverture du fichier");
    exit(EXIT_FAILURE);
  }
}

void Tab_To_FileBin(Contact *arr, int sizetab) {
  FILE *fichier = fopen("Contacts_sorted.bin", "wb");

  if (fichier != NULL) {
    // char tmp[12];
    for (int i = 0; i <= sizetab - 1; i++) {
      // sprintf(tmp,"%d",arr[i].iD);
      fprintf(fichier, "%1d,%08s,%s,%s,%s,%s$", arr[i].x, arr[i].iD,
              arr[i].name, arr[i].phoneNumber, arr[i].email, arr[i].otherInfo);
    }
    fclose(fichier);
  } else {
    perror("Erreur lors de l'ouverture du fichier");
  }
}

void Supp_logique(Contact *arr, int sizetab) {

  char id[10];
  printf("\ndonner le ID de contacts que vous voulez supprimer :");
  scanf("%s", id);
  for (int i = 0; i < sizetab; i++) {
    if (!strcmp(id, arr[i].iD)) {
      arr[i].x = 1;
      printf("Le contact avec iD: %8s a ete supprime avec succes", id);
      Tab_To_FileBin(arr, sizetab);
      return;
    }
  }
  printf("le contact Le contact avec iD: %8s n'existe pas !", id);
}

void libererTableau(Contact *tableau) { free(tableau); }

int main() {

  int sizetab = 0;

  Contact *ptab = BinFile_to_tab(&sizetab);

  mergeSort(ptab, 0, sizetab - 1);

  Tab_To_FileBin(ptab, sizetab);

  afficherTableau(ptab, sizetab);

  Supp_logique(ptab, sizetab);

  libererTableau(ptab);

  return 0;
}
