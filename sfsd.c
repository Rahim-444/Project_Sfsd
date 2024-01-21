#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define FPS 60
// the max number of rows in the width of the screen
#define maxBlockCols 6
// the number of chars in each block must be bigger than 251 + 31 + 31 + 11 + 9
// = 333 else the block woudn't be able to hold a contact
#define blockSegments 334
#define blockSegments 334
typedef struct Block {
  int blockNumber;  // num de block
  int ocupiedSpace; // in chars
  char Contacts[blockSegments];
  struct Block *nextBlock;
} Block;
typedef struct IndexFile {
  char id[10];
  Block *Pblock;
  int offset;
  struct IndexFile *next;
} IndexFile;
typedef struct Contact {
  int x;
  // j'ai ajouter 1 a chaque taille de string pour le caractere de fin de chaine
  int iD;
  char name[31];
  char phoneNumber[11];
  char email[31];
  char *otherInfo;
} Contact;

int compareIds(const void *a, const void *b) {
  return strcmp(((IndexFile *)a)->id, ((IndexFile *)b)->id);
}

Block *rechercheDichotomique(IndexFile *arr, int left, int right,
                             const char *searchId, int *offset) {

  if (right >= left) {
    int middle = left + (right - left) / 2;

    if (strcmp(arr[middle].id, searchId) == 0) {
      *offset = arr[middle].offset;
      printf("le contact est dans le block %p offset %d\n", arr[middle].Pblock,
             *offset);
      return arr[middle].Pblock;
    }

    if (strcmp(arr[middle].id, searchId) > 0) {
      return rechercheDichotomique(arr, left, middle - 1, searchId, offset);
    }

    return rechercheDichotomique(arr, middle + 1, right, searchId, offset);
  }
  printf("le Id: %s n'existe pas!\n", searchId);
  return NULL;
}

void mm(IndexFile arr[], int left, int middle, int right) {
  int i, j, k;
  int n1 = middle - left + 1;
  int n2 = right - middle;

  IndexFile leftArr[n1], rightArr[n2];

  for (i = 0; i < n1; i++)
    leftArr[i] = arr[left + i];
  for (j = 0; j < n2; j++)
    rightArr[j] = arr[middle + 1 + j];

  i = 0;
  j = 0;
  k = left;
  while (i < n1 && j < n2) {
    if (atoi(leftArr[i].id) <= atoi(rightArr[j].id)) {
      arr[k] = leftArr[i];
      i++;
    } else {
      arr[k] = rightArr[j];
      j++;
    }
    k++;
  }

  while (i < n1) {
    arr[k] = leftArr[i];
    i++;
    k++;
  }

  while (j < n2) {
    arr[k] = rightArr[j];
    j++;
    k++;
  }
}

void mSort(IndexFile *arr, int left, int right) {
  if (left < right) {
    int middle = left + (right - left) / 2;

    mSort(arr, left, middle);
    mSort(arr, middle + 1, right);

    mm(arr, left, middle, right);
  }
}

void merge(Contact arr[], int left, int mid, int right) { // not_same
  int i, j, k;
  int n1 = mid - left + 1;
  int n2 = right - mid;

  Contact L[n1], R[n2];

  for (i = 0; i < n1; i++)
    L[i] = arr[left + i];
  for (j = 0; j < n2; j++)
    R[j] = arr[mid + 1 + j];

  i = 0;
  j = 0;
  k = left;
  while (i < n1 && j < n2) {
    if (atoi(L[i].iD) <= atoi(R[j].iD)) {
      arr[k] = L[i];
      i++;
    } else {
      arr[k] = R[j];
      j++;
    }
    k++;
  }
  while (i < n1) {
    arr[k] = L[i];
    i++;
    k++;
  }
  while (j < n2) {
    arr[k] = R[j];
    j++;
    k++;
  }
}

void mergeSort(Contact *arr, int left, int right) { // not_same
  if (left < right) {
    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
  }
}
Contact *BinFile_to_tab(int *tailleTableau) { // not-same

  FILE *file = fopen("Contacts.bin", "rb");
  Contact tempContact;

  if (file != NULL) {
    *tailleTableau = 0; // Initialiser la taille du tableau

    while (fscanf(file, "%1d,%08d,%30s,%10s,%30s,%250s", &tempContact.x,
                  &tempContact.iD, tempContact.name, tempContact.phoneNumber,
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

      fscanf(file, "%1d,%08d,%30s,%10s,%30s,%250s", &tableau[i].x,
             &tableau[i].iD, tableau[i].name, tableau[i].phoneNumber,
             tableau[i].email, tableau[i].otherInfo);
    }
    return tableau;
    perror("Erreur lors de l'ouverture du fichier");
    exit(EXIT_FAILURE);
  }
}

void Tab_To_FileBin(Contact *arr, int sizetab) {
  FILE *fichier = fopen("Contacts_sorted.bin", "wb");

  if (fichier != NULL) {
    char tmp[12];
    for (int i = 0; i <= sizetab - 1; i++) {
      sprintf(tmp, "%d", arr[i].iD);
      fprintf(fichier, "%1d,%08s,%s,%s,%s,%s$", arr[i].x, tmp, arr[i].name,
              arr[i].phoneNumber, arr[i].email, arr[i].otherInfo);
    }
    fclose(fichier);
  } else {
    perror("Erreur lors de l'ouverture du fichier");
  }
}

void Supp_logique(Contact *arr, int sizetab) {

  int id;
  printf("\ndonner le ID de contacts que vous voulez supprimer :");
  scanf("%d", &id);
  for (int i = 0; i < sizetab; i++) {
    if (arr[i].iD == id) {
      arr[i].x = 1;
      printf("Le contact avec iD: %d a ete supprime avec succes", id);
      Tab_To_FileBin(arr, sizetab);
      return;
    }
  }
  printf("le contact Le contact avec iD: %d n'existe pas !", id);
}

// Contact *BinFile_to_tab(FILE *file, int *tailleTableau) { // not-same
//
//   Contact tempContact;
//   int deleted;
//   rewind(file);
//   if (file != NULL) {
//     *tailleTableau = 0; // Initialiser la taille du tableau
//     rewind(file);
//     while (fscanf(file, "%d,%8s,%30s,%10s,%30s,%250s\n", &deleted,
//                   tempContact.iD, tempContact.name, tempContact.phoneNumber,
//                   tempContact.email, tempContact.otherInfo) == 6) {
//       (*tailleTableau)++;
//       if (deleted == 0)
//         tempContact.isDeleted = false;
//       else
//         tempContact.isDeleted = true;
//     }
//     printf("taille de tableau new: %d\n", *tailleTableau);
//
//     rewind(file); // Rembobiner le fichier
//     // Allouer de la mémoire pour le tableau
//     Contact *tableau = (Contact *)malloc((*tailleTableau) * sizeof(Contact));
//     for (int i = 0; i < *tailleTableau; i++)
//       tableau[i].otherInfo = malloc(251 * sizeof(char));
//
//     if (tableau == NULL) {
//       fprintf(stderr, "Erreur d'allocation de mémoire\n");
//       exit(EXIT_FAILURE);
//     }
//     // Lire les données du fichier dans le tableau
//     for (int i = 0; i < *tailleTableau; i++) {
//       fscanf(file, "%d,", &deleted);
//       if (deleted == 0)
//         tempContact.isDeleted = false;
//       else
//         tempContact.isDeleted = true;
//       if (tempContact.isDeleted == 0) {
//         fscanf(file, "%8s,%30s,%10s,%30s,%250s\n", tableau[i].iD,
//                tableau[i].name, tableau[i].phoneNumber, tableau[i].email,
//                tableau[i].otherInfo);
//       }
//     }
//     return tableau;
//   } else {
//     perror("Erreur lors de l'ouverture du fichier");
//     exit(EXIT_FAILURE);
//   }
// }
//
// void Tab_To_FileBin(Contact *arr, int sizetab, FILE *fichier) {
//
//   if (fichier != NULL) {
//     for (int i = 0; i < sizetab; i++) {
//       fprintf(fichier, "%8s,%s,%s,%s,%s\n", arr[i].iD, arr[i].name,
//               arr[i].phoneNumber, arr[i].email, arr[i].otherInfo);
//     }
//   } else {
//     printf("Erreur lors de l'ouverture du fichier");
//   }
// }

void libererTableau(IndexFile *tableau) { free(tableau); }

IndexFile *IndexFile_to_tableau(int *tailleTableau) {
  FILE *file = fopen("Contact_index.bin", "r");
  IndexFile tmp;

  if (file != NULL) {
    *tailleTableau = 0;

    while (fscanf(file, "%8s,%p,%d\n", tmp.id, &tmp.Pblock, &tmp.offset) == 3) {
      (*tailleTableau)++;
    }

    rewind(file);

    IndexFile *tableau =
        (IndexFile *)malloc(*tailleTableau * sizeof(IndexFile));
    if (tableau == NULL) {
      fprintf(stderr, "Erreur d'allocation de mémoire\n");
      exit(EXIT_FAILURE);
    }

    for (int i = 0; i < *tailleTableau; i++) {
      fscanf(file, "%8s,%p,%d\n", tableau[i].id, &tableau[i].Pblock,
             &tableau[i].offset);
    }

    fclose(file);
    return tableau;
  } else {
    perror("Erreur lors de l'ouverture du fichier");
    exit(EXIT_FAILURE);
  }
}

typedef struct {
  int Red;
  int Green;
  int Blue;
} RGB;
typedef struct {
  char ID[9];
  Block *block;
  int offset;
} contactArray;

typedef struct {
  Block *firstBlock;
  int totalSize;   // number of blocks
  int contactSize; // number of contacts
  int addedConctacts;
  int deletedContacts;
} FileInfo; // this is my entete struct can't bother naming it in french

Contact *createContact(char ID[9]) {
  char letters[] = "abcdefghijklmnopqrstuvwxyz";
  long *iD = malloc(sizeof(long));
  *iD = atoi(ID);
  srand(time(NULL) + *iD);
  int obsLength = (rand() % 250) + 1;
  Contact *contact = (Contact *)malloc(sizeof(Contact));
  contact->otherInfo = malloc((obsLength + 1) * sizeof(char));
  char *name = malloc(30 * sizeof(char));
  char *phoneNumber = malloc(10 * sizeof(char));
  char *email = malloc(30 * sizeof(char));
  char *otherInfo = malloc(250 * sizeof(char));
  for (int i = 0; i < obsLength; i++) {
    otherInfo[i] = letters[rand() % 26];
  }
  strcpy(contact->otherInfo, otherInfo);
  for (int i = 0; i < 30; i++) {
    name[i] = letters[rand() % 26];
  }
  strcpy(contact->name, name);

  phoneNumber[0] = '0';
  for (int i = 1; i < 10; i++) {
    phoneNumber[i] = (rand() % 10) + '0';
  }
  strcpy(contact->phoneNumber, phoneNumber);

  for (int i = 0; i < 30; i++) {
    email[i] = letters[rand() % 26];
  }
  strcpy(contact->email, email);

  contact->isDeleted = false;
  strcpy(contact->iD, ID);
  free(name);
  free(phoneNumber);
  free(otherInfo);
  free(email);

  return contact;
}

void addBlock(FileInfo *fileinfo) {
  Block *block = malloc(sizeof(Block));
  fileinfo->totalSize++;
  block->blockNumber = fileinfo->totalSize;
  block->ocupiedSpace = 0;
  block->nextBlock = NULL;
  if (fileinfo->firstBlock == NULL) {
    fileinfo->firstBlock = block;
  } else {
    Block *tmp = fileinfo->firstBlock;
    while (tmp->nextBlock != NULL) {
      tmp = tmp->nextBlock;
    }
    tmp->nextBlock = block;
  }
}
void insertContactinBlock(FileInfo *fileinfo, Contact *contact) {
  Block *block = fileinfo->firstBlock;
  while (block->nextBlock != NULL) {
    block = block->nextBlock;
  }
  int contactSize = snprintf(NULL, 0, "%d,%s,%s,%s,%s,%s$", contact->isDeleted,
                             contact->iD, contact->name, contact->phoneNumber,
                             contact->email, contact->otherInfo);
  char *contactString = malloc(contactSize + 1);
  if (contact->isDeleted) {
    sprintf(contactString, "%d,%s,%s,%s,%s,%s$", 1, contact->iD, contact->name,
            contact->phoneNumber, contact->email, contact->otherInfo);
  } else {
    sprintf(contactString, "%d,%s,%s,%s,%s,%s$", 0, contact->iD, contact->name,
            contact->phoneNumber, contact->email, contact->otherInfo);
  }
  int len = strlen(contactString);
  // insertion block
  if (block->ocupiedSpace + len > blockSegments) { // chevauchement
    int cpt = 0;
    for (int j = block->ocupiedSpace; j < blockSegments; j++) {
      block->Contacts[j] = contactString[j - block->ocupiedSpace];
      cpt++;
    }
    block->ocupiedSpace = blockSegments;
    addBlock(fileinfo);
    block = block->nextBlock;
    for (int j = 0; j < len - cpt; j++) {
      block->Contacts[j] = contactString[j + cpt];
    }
    block->ocupiedSpace = len - cpt;
  } else { // non chevauchemen
    for (int j = block->ocupiedSpace; j < block->ocupiedSpace + len; j++) {
      block->Contacts[j] = contactString[j - block->ocupiedSpace];
    }
    block->ocupiedSpace += len;
  }
  free(contactString);
}

void insertContactinFile(FILE *file, Contact *contact) {

  int contactSize = snprintf(NULL, 0, "%d,%s,%s,%s,%s,%s$", contact->isDeleted,
                             contact->iD, contact->name, contact->phoneNumber,
                             contact->email, contact->otherInfo);
  char *contactString = malloc(contactSize + 1);
  if (contact->isDeleted) {
    sprintf(contactString, "%d,%s,%s,%s,%s,%s$", 1, contact->iD, contact->name,
            contact->phoneNumber, contact->email, contact->otherInfo);
  } else {
    sprintf(contactString, "%d,%s,%s,%s,%s,%s$", 0, contact->iD, contact->name,
            contact->phoneNumber, contact->email, contact->otherInfo);
  }
  if (contact->isDeleted == false)
    fprintf(file, "%d,%s,%s,%s,%s,%s\n", 0, contact->iD, contact->name,
            contact->phoneNumber, contact->email, contact->otherInfo);
  else
    fprintf(file, "%d,%s,%s,%s,%s,%s\n", 1, contact->iD, contact->name,
            contact->phoneNumber, contact->email, contact->otherInfo);
  free(contactString);
}

void fillFile(FileInfo *fileinfo, FILE *file) {
  addBlock(fileinfo);
  for (int i = 0; i < fileinfo->contactSize; i++) {
    long iD = ((i + 1) * 10000019) % 100000000;
    char ID[9];
    sprintf(ID, "%08ld", iD);
    Contact *contact = createContact(ID);
    insertContactinBlock(fileinfo, contact);
    insertContactinFile(file, contact);
    free(contact->otherInfo);
    free(contact);
  }
}
void CreateIndexFileNonDense(FileInfo *fileinfo, FILE *file) {
  Block *block = fileinfo->firstBlock;
  char *id = malloc(9 * sizeof(char));
  for (int i = 2; i < 10; i++) {
    id[i - 2] = fileinfo->firstBlock->Contacts[i];
  }
  fprintf(file, "%s,%p,%d\n", id, block, 0);
  block = block->nextBlock;
  while (block != NULL) {
    bool move = true;
    for (int j = 0; j < block->ocupiedSpace; j++) {
      if (block->Contacts[j] == '$') {
        int temp = j;
        if ((block->nextBlock == NULL) && (j + 1 >= block->ocupiedSpace))
          break;
        if (j + 10 < blockSegments) {
          while (block->Contacts[j] != ',')
            j++;
          for (int k = j + 1; k < j + 9; k++) {
            id[k - j - 1] = block->Contacts[k];
          }
          id[8] = '\0';
        } else {
          while ((block->Contacts[j] != ',') && (j < blockSegments))
            j++;
          if (j == blockSegments) {
            block = block->nextBlock;
            move = false;
            while (block->Contacts[j] != ',')
              j++;
            for (int k = j + 1; k < j + 9; k++) {
              id[k - j - 1] = block->Contacts[k];
            }
            id[8] = '\0';
          } else {
            int cpt = 0;
            for (int k = j + 1; k < blockSegments; k++) {
              id[k - j - 1] = block->Contacts[k];
              cpt++;
            }
            block = block->nextBlock;
            move = false;
            for (int k = 0; k < 10 - cpt; k++) {
              id[k + cpt] = block->Contacts[k];
            }
            id[8] = '\0';
          }
        }
        fprintf(file, "%s,%p,%d\n", id, block, temp);
        break;
      }
    }
    if (move)
      block = block->nextBlock;
  }
  free(id);
}

void CreateIndexFile(FileInfo *fileinfo, FILE *file) {
  Block *block = fileinfo->firstBlock;
  char *id = malloc(9 * sizeof(char));
  for (int i = 2; i < 10; i++) {
    id[i - 2] = fileinfo->firstBlock->Contacts[i];
  }
  fprintf(file, "%s,%p,%d\n", id, block, 0);
  while (block != NULL) {
    bool move = true;
    for (int j = 0; j < block->ocupiedSpace; j++) {
      if (block->Contacts[j] == '$') {
        int temp = j;
        if ((block->nextBlock == NULL) && (j + 1 >= block->ocupiedSpace))
          break;
        if (j + 10 < blockSegments) {
          while (block->Contacts[j] != ',')
            j++;
          for (int k = j + 1; k < j + 9; k++) {
            id[k - j - 1] = block->Contacts[k];
          }
          id[8] = '\0';
        } else {
          while ((block->Contacts[j] != ',') && (j < blockSegments))
            j++;
          if (j == blockSegments) {
            block = block->nextBlock;
            move = false;
            while (block->Contacts[j] != ',')
              j++;
            for (int k = j + 1; k < j + 9; k++) {
              id[k - j - 1] = block->Contacts[k];
            }
            id[8] = '\0';
          } else {
            int cpt = 0;
            for (int k = j + 1; k < blockSegments; k++) {
              id[k - j - 1] = block->Contacts[k];
              cpt++;
            }
            block = block->nextBlock;
            move = false;
            for (int k = 0; k < 10 - cpt; k++) {
              id[k + cpt] = block->Contacts[k];
            }
            id[8] = '\0';
          }
        }
        fprintf(file, "%s,%p,%d\n", id, block, temp);
      }
    }
    if (move)
      block = block->nextBlock;
  }
  free(id);
}
Contact *createContactInput(FileInfo *fileinfo) {
  char *name = malloc(30 * sizeof(char));
  char *phoneNumber = malloc(10 * sizeof(char));
  char *email = malloc(30 * sizeof(char));
  char *otherInfo = malloc(250 * sizeof(char));
  printf("Enter the name of the contact : ");
  scanf("%s", name);
  printf("\n");
  printf("Enter the phone number of the contact : ");
  scanf("%s", phoneNumber);
  printf("\n");
  printf("Enter the email of the contact : ");
  scanf("%s", email);
  printf("\n");
  printf("Enter the other info of the contact : ");
  scanf("%s", otherInfo);
  printf("\n");
  long iD = ((fileinfo->contactSize + 1) * 10000019) % 100000000;
  char *ID = malloc(9 * sizeof(char));
  sprintf(ID, "%08ld", iD);
  Contact *contact = (Contact *)malloc(sizeof(Contact));
  contact->otherInfo = malloc((strlen(otherInfo) + 1) * sizeof(char));
  strcpy(contact->otherInfo, otherInfo);
  strcpy(contact->name, name);
  strcpy(contact->phoneNumber, phoneNumber);
  strcpy(contact->email, email);
  contact->isDeleted = false;
  strcpy(contact->iD, ID);
  free(name);
  free(phoneNumber);
  free(otherInfo);
  free(email);
  free(ID);
  return contact;
}
int deleteContact(FileInfo *fileinfo) {
  char ID[9];
  char tmp[9];
  Block *tmp2;
  int temp3;

  FILE *index = fopen("Contact_index.bin", "rw");
  if (index == NULL) {
    fprintf(stderr, "Error opening file.\n");
    return 1;
  }

  contactArray *contacts = malloc(fileinfo->contactSize * sizeof(contactArray));
  if (contacts == NULL) {
    fprintf(stderr, "Memory allocation error.\n");
    fclose(index);
    return 1;
  }

  int ContactLen = 0;
  while (fscanf(index, "%8s,%p,%d\n", tmp, &tmp2, &temp3) == 3 &&
         ContactLen < fileinfo->contactSize) {
    strcpy(contacts[ContactLen].ID, tmp);
    printf("%s\n", contacts[ContactLen].ID);
    contacts[ContactLen].block = tmp2;
    contacts[ContactLen].offset = temp3;
    ContactLen++;
  }

  printf("Enter the ID of the contact you want to delete: ");
  scanf("%8s", ID);

  printf("\n");

  int contactFound = 0;
  for (int j = 0; j < ContactLen; j++) {
    if (strcmp(ID, contacts[j].ID) == 0) {
      printf("Contact found\n");
      contactFound = 1;
      contacts[j].block->Contacts[contacts[j].offset + 1] = '1';
    }
  }

  if (!contactFound) {
    printf("Contact not found\n");
  }
  // rewrite the index file
  CreateIndexFile(fileinfo, index);
  // TODO: rewrite the contact file with the changes
  // from block to file
  free(contacts);
  fclose(index);

  return 0;
}
void search(char *id) {
  int sizetab = 0;
  IndexFile *ptab = IndexFile_to_tableau(&sizetab);
  mSort(ptab, 0, sizetab - 1);
  int offset = 0;
  Block *tmp = rechercheDichotomique(ptab, 0, sizetab - 1, id, &offset);
  libererTableau(ptab);
}
void CreatedSortedContacts(FILE *file, FILE *indexFile, FILE *sortedFile) {
  // FIX:error here i don't know why
  int sizetab = 0;
  Contact *Ctab = BinFile_to_tab(file, &sizetab);
  mergeSort(Ctab, 0, sizetab - 1);
  Tab_To_FileBin(Ctab, sizetab, sortedFile);
  free(Ctab);
}

int main(int argc, char *argv[]) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    printf("error initializing SDL: %s\n", SDL_GetError());
    return 1;
  }
  SDL_Window *win = SDL_CreateWindow("Project test", SDL_WINDOWPOS_CENTERED,
                                     SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH,
                                     WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
  SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
  FILE *ContactFile = fopen("Contacts.bin", "w+");
  FileInfo *fileinfo = malloc(sizeof(FileInfo));
  fileinfo->contactSize = 10;
  fileinfo->firstBlock = NULL;
  fileinfo->totalSize = 0;
  fillFile(fileinfo, ContactFile);
  FILE *indexFile = fopen("Contact_index.bin", "w+");
  CreateIndexFile(fileinfo, indexFile);
  FILE *indexFile2 = fopen("./Contacts_index_nonDense.bin", "w+");
  CreateIndexFileNonDense(fileinfo, indexFile2);
  fclose(indexFile);
  FILE *sortedFile = fopen("Contacts_sorted.bin", "w+");
  CreatedSortedContacts(ContactFile, indexFile, sortedFile);
  SDL_RenderClear(ren);
  bool running = true;
  SDL_Event event;
  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = false;
      }
      if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_ESCAPE) {
          running = false;
        }
        if (event.key.keysym.sym == SDLK_SPACE) {
          Contact *contact = createContactInput(fileinfo);
          insertContactinBlock(fileinfo, contact);
          insertContactinFile(ContactFile, contact);
          fileinfo->contactSize++;
        }
        if (event.key.keysym.sym == SDLK_BACKSPACE) {
          deleteContact(fileinfo);
        }
        if (event.key.keysym.sym == SDLK_r) {
          char id[9];
          printf("Enter the ID of the contact you want to search: ");
          scanf("%8s", id);
          printf("\n");
          search(id);
        }
      }
    }
    int Blockrows = (fileinfo->totalSize / maxBlockCols);
    float extraBlock = fileinfo->totalSize % maxBlockCols;
    if (extraBlock != 0) {
      Blockrows++;
    }
    int BlockWidth = WINDOW_WIDTH / maxBlockCols;
    int BlockHeight = WINDOW_HEIGHT / Blockrows;
    int color = 0;
    SDL_SetRenderDrawColor(ren, color, color, color, 255);
    SDL_Rect rect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    SDL_RenderFillRect(ren, &rect);
    int k = 0;
    int i = 0;
    int j = 0;
    while (k < fileinfo->totalSize) {
      SDL_Rect rect = {i * BlockWidth, j * BlockHeight, BlockWidth - 5,
                       BlockHeight - 10};
      int grayScale = 255;
      SDL_SetRenderDrawColor(ren, grayScale, grayScale, grayScale, 255);
      SDL_RenderFillRect(ren, &rect);
      i++;
      k++;
      if (i == maxBlockCols) {
        i = 0;
        j++;
      }
    }
    k = 0;
    i = 0;
    j = 0;
    float SegementHeight = (BlockHeight - 10) / (blockSegments * 1.0);
    RGB *rgb = malloc(sizeof(RGB));
    Block *block = fileinfo->firstBlock;
    srand(1);
    rgb->Red = rand() % 255;
    rgb->Green = rand() % 255;
    rgb->Blue = rand() % 255;
    int cpt = 0;
    while (k < fileinfo->totalSize) {
      for (int h = 0; h < blockSegments; h++) {
        if (block->Contacts[h] == '$') {
          if (block->Contacts[h + 1] == '1') {
            rgb->Red = 0;
            rgb->Green = 0;
            rgb->Blue = 0;
          } else {
            rgb->Red = rand() % 255;
            rgb->Green = rand() % 255;
            rgb->Blue = rand() % 255;
            cpt++;
          }
          if (cpt == fileinfo->contactSize) {
            SDL_Rect rect = {i * BlockWidth,
                             h * SegementHeight + j * BlockHeight,
                             BlockWidth - 5, 1200};
            SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
            SDL_RenderFillRect(ren, &rect);
            break;
          }
        }
        SDL_Rect rect = {i * BlockWidth, h * SegementHeight + j * BlockHeight,
                         BlockWidth - 5, SegementHeight};
        SDL_SetRenderDrawColor(ren, rgb->Red, rgb->Green, rgb->Blue, 255);
        SDL_RenderFillRect(ren, &rect);
        SDL_RenderDrawRect(ren, &rect);
      }
      block = block->nextBlock;
      i++;
      k++;
      if (i == maxBlockCols) {
        i = 0;
        j++;
      }
    }
    free(rgb);
    SDL_RenderPresent(ren);
    SDL_RenderClear(ren);
    SDL_Delay(1000 / FPS);
  }
  SDL_DestroyWindow(win);
  SDL_DestroyRenderer(ren);
  SDL_Quit();
  free(fileinfo);
  fclose(ContactFile);
  return 0;
}
