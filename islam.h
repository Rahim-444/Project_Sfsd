#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <string.h>


#define blockSegments 334
typedef struct Block {
  int blockNumber;  // num de block
  int ocupiedSpace; // in chars
  char Contacts[blockSegments];
  struct Block *nextBlock;
} Block;
typedef struct IndexFile {
    char id[10];
    Block* Pblock;
    struct IndexFile* next;
} IndexFile;
typedef struct Contact {
  bool isDeleted;
  // j'ai ajouter 1 a chaque taille de string pour le caractere de fin de chaine
  char iD[9];
  char name[31];
  char phoneNumber[11];
  char email[31];
  char *otherInfo;
} Contact;


int compareIds(const void *a, const void *b) {
    return strcmp(((IndexFile *)a)->id, ((IndexFile *)b)->id);
}

Block * rechercheDichotomique(IndexFile* arr, int left, int right, const char *searchId) {
	
    if (right >= left) {
        int middle = left + (right - left) / 2;

        // Si l'élément est présent au milieu
        if (strcmp(arr[middle].id, searchId) == 0) {
		printf("%d, %s, %p.\n",middle,arr[middle].id,arr[middle].Pblock);
        return arr[middle].Pblock ;
        }

        // Si l'élément est plus petit que celui au milieu, recherchez à gauche
        if (strcmp(arr[middle].id, searchId) > 0) {
            return rechercheDichotomique(arr, left, middle - 1, searchId);
        }

        // Sinon, recherchez à droite
        return rechercheDichotomique(arr, middle + 1, right, searchId);
    }
    printf("le Id: %s n'existe pas!\n",searchId);
    return NULL;
    // Si l'élément n'est pas présent dans le tableau

}

void mm(IndexFile arr[], int left, int middle, int right) {
    int i, j, k;
    int n1 = middle - left + 1;
    int n2 = right - middle;

    // Créer des tableaux temporaires
    IndexFile leftArr[n1], rightArr[n2];

    // Copier les données dans les tableaux temporaires leftArr[] et rightArr[]
    for (i = 0; i < n1; i++)
        leftArr[i] = arr[left + i];
    for (j = 0; j < n2; j++)
        rightArr[j] = arr[middle + 1 + j];

    // Fusionner les tableaux temporaires dans arr[left..right]
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = left; // Initial index of merged subarray
    while (i < n1 && j < n2) {
        if (strcmp(leftArr[i].id, rightArr[j].id) <= 0) {
            arr[k] = leftArr[i];
            i++;
        } else {
            arr[k] = rightArr[j];
            j++;
        }
        k++;
    }

    // Copier les éléments restants de leftArr[], s'il y en a
    while (i < n1) {
        arr[k] = leftArr[i];
        i++;
        k++;
    }

    // Copier les éléments restants de rightArr[], s'il y en a
    while (j < n2) {
        arr[k] = rightArr[j];
        j++;
        k++;
    }
}

void mSort(IndexFile* arr, int left, int right) {
    if (left < right) {
        // Same as (left+right)/2, but avoids overflow for large left and right
        int middle = left + (right - left) / 2;

        // Trier la première et la deuxième moitié
        mSort(arr, left, middle);
        mSort(arr, middle + 1, right);

        // Fusionner les deux moitiés triées
        mm(arr, left, middle, right);
    }
}

void merge(Contact arr[], int left, int mid, int right) {//not_same
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

void mergeSort(Contact* arr, int left, int right) {//not_same
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



Contact* BinFile_to_tab(int* tailleTableau) {//not-same
	
    FILE *file = fopen("Contacts.bin", "rb");
    Contact tempContact;
    int deleted;
    if (file != NULL) {
        *tailleTableau = 0; // Initialiser la taille du tableau
        while (fscanf(file, "%d,%8s,%30s,%10s,%30s,%250s", &deleted,
              tempContact.iD, tempContact.name,
              tempContact.phoneNumber, tempContact.email,
              tempContact.otherInfo) == 6) {
   		 (*tailleTableau)++;
            if(deleted==0)
                tempContact.isDeleted=false;
            else
                tempContact.isDeleted=true;
        }
        
        rewind(file); // Rembobiner le fichier
        // Allouer de la mémoire pour le tableau
        Contact* tableau = (Contact*)malloc((*tailleTableau) * sizeof(Contact));

        if (tableau == NULL) {
            fprintf(stderr, "Erreur d'allocation de mémoire\n");
            exit(EXIT_FAILURE);
        }
        // Lire les données du fichier dans le tableau
        for (int i = 0; i < *tailleTableau; i++) {
           fscanf(file, "%d,", &deleted);
            if(deleted==0)
                tempContact.isDeleted=false;
            else
                tempContact.isDeleted=true; 
			if (tempContact.isDeleted == 0) {
    			fscanf(file, "%8s,%30s,%10s,%30s,%250s",
          		 tableau[i].iD, tableau[i].name,
         		  tableau[i].phoneNumber, tableau[i].email,
           		tableau[i].otherInfo);
			}
		}

        fclose(file); // Fermer le fichier
        return tableau;
    } else {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }
}

void Tab_To_FileBin(Contact* arr,int sizetab) {
    FILE* fichier = fopen("Contacts_sorted.bin", "wb");
    
    if (fichier != NULL) {
		char tmp[12];
       for(int i=0;i<sizetab;i++){
            fprintf(fichier, "%8s,%s,%s,%s,%s$",
                arr[i].iD,arr[i].name,arr[i].phoneNumber, arr[i].email, arr[i].otherInfo);
        }
        fclose(fichier); 
    } else {
        perror("Erreur lors de l'ouverture du fichier");
    }
}

void libererTableau(IndexFile* tableau) {
    free(tableau);
}

IndexFile* IndexFile_to_tableau(int* tailleTableau) {
    FILE *file = fopen("Contact_index.bin", "r");
    IndexFile tmp;

    if (file != NULL) {
        *tailleTableau = 0; // Initialiser la taille du tableau

        // Compter le nombre d'éléments dans le fichier
        while (fscanf(file, "%8s,%p\n", tmp.id, &tmp.Pblock) == 2) {
            (*tailleTableau)++;
        }

        rewind(file); // Rembobiner le fichier

        // Allouer de la mémoire pour le tableau
        IndexFile* tableau = (IndexFile*)malloc(*tailleTableau * sizeof(IndexFile));
        if (tableau == NULL) {
            fprintf(stderr, "Erreur d'allocation de mémoire\n");
            exit(EXIT_FAILURE);
        }

        // Lire les données du fichier dans le tableau
        for (int i = 0; i < *tailleTableau; i++) {
            fscanf(file, "%8s,%p\n", tableau[i].id, &tableau[i].Pblock);
        }

        fclose(file); // Fermer le fichier
        return tableau;
    } else {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }
}


void islam() {
    int sizetab = 0;
    IndexFile* ptab = IndexFile_to_tableau(&sizetab);
    mSort(ptab, 0,sizetab-1);
    Block *tmp = rechercheDichotomique(ptab, 0, sizetab - 1,"20000608");
    printf("%p\n",tmp);
     sizetab = 0;
    Contact* Ctab = BinFile_to_tab(&sizetab);
    
    mergeSort(Ctab,0,sizetab-1);
    
    Tab_To_FileBin(Ctab,sizetab);
    
    free(Ctab);
    libererTableau(ptab);
}
