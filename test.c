#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Contact {
  bool isDeleted;
  // j'ai ajouter 1 a chaque taille de string pour le caractere de fin de chaine
  char iD[9];
  char name[31];
  char phoneNumber[11];
  char email[31];
  char *otherInfo;
} Contact;

int main() { // not-same
  FILE *file = fopen("Contacts.bin", "rb");
  Contact tempContact;
  int deleted;
  if (file != NULL) {
    int tailleTableau = 0; // Initialiser la taille du tableau
    fscanf(file, "%d,%08s,%30s,%10s,%30s,%250s", &deleted, tempContact.iD,
           tempContact.name, tempContact.phoneNumber, tempContact.email,
           tempContact.otherInfo);
    printf("%8s,%s,%s,%s,%s\n", tempContact.iD, tempContact.name,
           tempContact.phoneNumber, tempContact.email, tempContact.otherInfo);

    // while (fscanf(file, "%d,%8s,%s,%s,%s,%s\n", &deleted, tempContact.iD,
    //               tempContact.name, tempContact.phoneNumber,
    //               tempContact.email, tempContact.otherInfo) == 6) {
    //   (*tailleTableau)++;
    //   if (deleted == 0)
    //     tempContact.isDeleted = false;
    //   else
    //     tempContact.isDeleted = true;
    // }
    // printf("taille de tableau new: %d\n", tailleTableau);
    //
    // rewind(file); // Rembobiner le fichier
    // // Allouer de la mémoire pour le tableau
    // Contact *tableau = (Contact *)malloc((tailleTableau) * sizeof(Contact));
    // for (int i = 0; i < tailleTableau; i++)
    //   tableau[i].otherInfo = malloc(251 * sizeof(char));
    //
    // if (tableau == NULL) {
    //   fprintf(stderr, "Erreur d'allocation de mémoire\n");
    //   exit(EXIT_FAILURE);
    // }
    // // Lire les données du fichier dans le tableau
    // for (int i = 0; i < tailleTableau; i++) {
    //   fscanf(file, "%d,", &deleted);
    //   if (deleted == 0)
    //     tempContact.isDeleted = false;
    //   else
    //     tempContact.isDeleted = true;
    //   if (tempContact.isDeleted == 0) {
    //     fscanf(file, "%8s,%30s,%10s,%30s,%250s\n", tableau[i].iD,
    //            tableau[i].name, tableau[i].phoneNumber, tableau[i].email,
    //            tableau[i].otherInfo);
    //   }
    // }
    fclose(file);
  } else {
    perror("Erreur lors de l'ouverture du fichier");
    exit(EXIT_FAILURE);
  }
}
