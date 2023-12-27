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
// the number of chars in each block
#define blockSegments 120
typedef struct Contact {
  bool isDeleted;
  // j'ai ajouter 1 a chaque taille de string pour le caractere de fin de chaine
  char iD[9];
  char name[31];
  char phoneNumber[11];
  char email[31];
  char *otherInfo;
} Contact;

typedef struct Block {
  int blockNumber;  // num de block
  int ocupiedSpace; // in chars
  struct Block *nextBlock;
  char Contacts[blockSegments];
} Block;

typedef struct {
  Block *firstBlock;
  int totalSize;   // number of blocks
  int contactSize; // number of contacts
  int addedConctacts;
  int deletedContacts;
} FileInfo; // this is my entete struct can't bother naming it in french

Contact *createContact(char ID[9]) {
  char letters[] = "abcdefghijklmnopqrstuvwxyz";
  int *iD = malloc(sizeof(int));
  *iD = atoi(ID);
  srand((unsigned int)time(NULL) + *iD);

  int obsLength = (rand() % 200) + 1;
  Contact *contact = malloc(sizeof(Contact));
  contact->otherInfo = malloc(obsLength * sizeof(char));
  char *name = malloc(30 * sizeof(char));
  char *phoneNumber = malloc(10 * sizeof(char));
  char *email = malloc(30 * sizeof(char));

  for (int i = 0; i < obsLength; i++) {
    contact->otherInfo[i] = letters[rand() % 26];
  }

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

void fillFile(FileInfo *fileinfo, FILE *file) {
  addBlock(fileinfo);
  Block *block = fileinfo->firstBlock;
  for (int i = 0; i < fileinfo->contactSize; i++) {
    long iD = ((i + 1) * 10000019) % 100000000;
    char ID[9];
    sprintf(ID, "%08ld", iD);
    Contact *contact = createContact(ID);
    // the size of the contatcs in chars
    int contactSize = 1 + strlen(contact->iD) + strlen(contact->name) +
                      strlen(contact->phoneNumber) + strlen(contact->email) +
                      strlen(contact->otherInfo) + 5 +
                      10; // for testing purposes
    char *contactString = malloc(contactSize * sizeof(char));
    sprintf(contactString, "%d,%s,%s,%s,%s,%s\n", contact->isDeleted,
            contact->iD, contact->name, contact->phoneNumber, contact->email,
            contact->otherInfo);
    // // parcourir jusqu'a la fin de bloc
    // while (block->nextBlock != NULL) {
    //   block = block->nextBlock;
    // }
    if (block->ocupiedSpace + contactSize > blockSegments) { // chevauchement
      int cpt = 0;
      for (int j = block->ocupiedSpace + 1; j <= blockSegments; j++) {
        block->Contacts[j] = contactString[j - (block->ocupiedSpace + 1)];
        cpt++;
      }
      addBlock(fileinfo);
      block = block->nextBlock;
      for (int j = 0; j < contactSize - cpt; j++) {
        block->Contacts[j] = contactString[j + cpt];
      }
    } else { // non chevauchemen
      for (int j = block->ocupiedSpace + 1;
           j < block->ocupiedSpace + 1 + contactSize; j++) {
        block->Contacts[j] = contactString[j - block->ocupiedSpace - 1];
      }
    }
    block->ocupiedSpace += contactSize;
    if (contact->isDeleted == false)
      fprintf(file, "%d,%s,%s,%s,%s,%s\n", 0, contact->iD, contact->name,
              contact->phoneNumber, contact->email, contact->otherInfo);
    else
      fprintf(file, "%d,%s,%s,%s,%s\n", 1, contact->iD, contact->name,
              contact->phoneNumber, contact->email);
    free(contact->otherInfo);
    free(contact);
    free(contactString);
  }
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
  FILE *file = fopen("Contacts.bin", "w+");
  FileInfo *fileinfo = malloc(sizeof(FileInfo));
  fileinfo->contactSize = 30;
  fileinfo->firstBlock = NULL;
  fileinfo->totalSize = 0;
  fillFile(fileinfo, file);
  int Blockrows = ((fileinfo->contactSize / blockSegments) / maxBlockCols);
  float extraBlock = fileinfo->contactSize % blockSegments;
  if (extraBlock != 0) {
    Blockrows++;
  }
  int BlockWidth = WINDOW_WIDTH / maxBlockCols;
  int BlockHeight = WINDOW_HEIGHT / Blockrows;
  SDL_RenderClear(ren);
  bool running = true;
  SDL_Event event;
  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = false;
      }
    }
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_Rect rect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    SDL_RenderFillRect(ren, &rect);
    // int i = 0;
    // int j = 0;
    // while (i < fileinfo->totalSize) {
    //   int BlockWidth = WINDOW_WIDTH / maxBlockCols;
    //   int BlockHeight = WINDOW_HEIGHT / fileinfo->totalSize;
    //   SDL_Rect rect = {i * BlockWidth, j * BlockHeight, BlockWidth - 5,
    //                    BlockHeight - 10};
    //   // draw the boarder of the rectangle
    //   int grayScale = 255;
    //
    //   SDL_SetRenderDrawColor(ren, grayScale, grayScale, grayScale, 255);
    //   SDL_RenderFillRect(ren, &rect);
    //   i++;
    //   if (i == maxBlockCols) {
    //     i = 0;
    //     j++;
    //   }
    // }
    //
    // Block *block = fileinfo->firstBlock;
    // i = 0;
    // j = 0;
    // while (block != NULL) {
    //   int BlockWidth = WINDOW_WIDTH / maxBlockCols;
    //   int BlockHeight = WINDOW_HEIGHT / fileinfo->totalSize;
    //   for (int k = 0; k < block->ocupiedSpace; k++) {
    //     // divide each block into blockSegments number of lignes
    //     int fragmentHeight = (BlockHeight - 5) / blockSegments;
    //     SDL_Rect rect = {i * BlockWidth, j * BlockHeight + k *
    //     fragmentHeight,
    //                      fragmentHeight, BlockWidth - 5};
    //     SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    //     SDL_RenderDrawRect(ren, &rect);
    //   }
    //   block = block->nextBlock;
    //   i++;
    //   if (i == maxBlockCols) {
    //     i = 0;
    //     j++;
    //   }
    // }
    SDL_RenderPresent(ren);
  }
  SDL_DestroyWindow(win);
  SDL_DestroyRenderer(ren);
  SDL_Quit();
  free(fileinfo);
  fclose(file);
  return 0;
}
