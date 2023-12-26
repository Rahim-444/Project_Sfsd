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
  struct Contact *nextContact;
} Contact;

typedef struct Block {
  int blockNumber;
  int ocupiedSpace; // in segments
  struct Block *nextBlock;
  Contact *Contact;
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
Block *addBlock(FileInfo *fileinfo) {
  FileInfo *tmp = fileinfo;
  if (fileinfo->firstBlock == NULL) {
    Block *block = malloc(sizeof(Block));
    fileinfo->totalSize++;
    block->blockNumber = fileinfo->totalSize;
    block->ocupiedSpace = 0;
    block->Contact = NULL;
    block->nextBlock = NULL;
    return block;
  }
  while (fileinfo->firstBlock->nextBlock != NULL) {
    fileinfo->firstBlock = fileinfo->firstBlock->nextBlock;
  }
  Block *block = malloc(sizeof(Block));
  fileinfo->totalSize++;
  block->blockNumber = fileinfo->totalSize;
  block->Contact = NULL;
  block->ocupiedSpace = 0;
  block->nextBlock = NULL;
  return block;
}

void fillFile(FileInfo *fileinfo, FILE *file) {
  Block *block = fileinfo->firstBlock;
  for (int i = 0; i < fileinfo->contactSize; i++) {
    long iD = ((i + 1) * 10000019) % 100000000;
    char ID[9];
    sprintf(ID, "%08ld", iD);
    Contact *contact = createContact(ID);
    // the size of the contatcs in chars
    int contactSize = 1 + strlen(contact->iD) + strlen(contact->name) +
                      strlen(contact->phoneNumber) + strlen(contact->email) +
                      strlen(contact->otherInfo);
    if (block->Contact == NULL) {
      block->Contact = contact;
    } else {
      if (block->ocupiedSpace + contactSize > blockSegments) {
        block->nextBlock = addBlock(fileinfo);
        block = block->nextBlock;
        block->Contact = contact;
      } else {
        Block *tmp = block;
        while (tmp->nextBlock != NULL) {
          tmp = tmp->nextBlock;
        }
        tmp->Contact = contact;
      }
    }
    block->ocupiedSpace += contactSize;
    if (contact->isDeleted == false)
      fprintf(file, "%d %s %s %s %s %s\n", 0, contact->iD, contact->name,
              contact->phoneNumber, contact->email, contact->otherInfo);
    else
      fprintf(file, "%d %s %s %s %s\n", 1, contact->iD, contact->name,
              contact->phoneNumber, contact->email);
    free(contact);
    free(contact->otherInfo);
  }
  rewind(file);
}

int main(int argc, char *argv[]) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    printf("error initializing SDL: %s\n", SDL_GetError());
    return 1;
  }
  SDL_Window *win = SDL_CreateWindow("Project sfsd", SDL_WINDOWPOS_CENTERED,
                                     SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH,
                                     WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
  SDL_Renderer *ren = SDL_CreateRenderer(
      win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  FILE *file = fopen("Contacts.bin", "w+");
  FileInfo *fileinfo = malloc(sizeof(FileInfo));
  fileinfo->contactSize = 5;
  fileinfo->firstBlock = NULL;
  fileinfo->totalSize = 0;
  fileinfo->firstBlock = addBlock(fileinfo);
  bool running = true;
  SDL_Event event;
  // FIX:account for every division it could be a floating number
  int Blockrows = ((fileinfo->contactSize / blockSegments) / maxBlockCols);
  float extraBlock = fileinfo->contactSize % blockSegments;
  if (extraBlock != 0) {
    Blockrows++;
  }
  for (int i = 0; i < (Blockrows * maxBlockCols) - 1; i++) {
    addBlock(fileinfo);
  }
  fillFile(fileinfo, file);
  int BlockWidth = WINDOW_WIDTH / maxBlockCols;
  int BlockHeight = WINDOW_HEIGHT / Blockrows;
  SDL_RenderClear(ren);
  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = false;
      }
    }
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_Rect rect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    SDL_RenderFillRect(ren, &rect);
    for (int i = 0; i <= maxBlockCols; i++) {
      for (int j = 0; j <= Blockrows; j++) {
        SDL_Rect rect = {i * BlockWidth, j * BlockHeight, BlockWidth - 5,
                         BlockHeight - 10};
        // draw the boarder of the rectangle
        int grayScale = 255;

        SDL_SetRenderDrawColor(ren, grayScale, grayScale, grayScale, 255);
        SDL_RenderFillRect(ren, &rect);
      }
    }
    // for (int i = 0; i <= maxBlockCols; i++) {
    //   for (int j = 0; j <= Blockrows; j++) {
    //     for (int k = 0; k < fileinfo.contactSize; k++) {
    //       // divide each block into blockSegments number of lignes
    //       int fragmentHeight = (BlockHeight - 5) / blockSegments;
    //       SDL_Rect rect = {i * BlockWidth, j * BlockHeight + k *
    //       fragmentHeight, fragmentHeight,
    //                        BlockWidth - 5, fragmentHeight};
    //       SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    //       SDL_RenderDrawRect(ren, &rect);
    //     }
    //   }
    // }
    // write the last loop again using while because we have a dynamic list of
    // contacts
    // FIX: this is stright up wrong
    Block *block = fileinfo->firstBlock;
    int i = 0;
    int j = 0;
    while (block->nextBlock != NULL) {
      Contact *contact = block->Contact;
      while (contact->nextContact != NULL) {
        // divide each block into blockSegments number of lignes
        int fragmentHeight = (BlockHeight - 5) / blockSegments;
        SDL_Rect rect = {i * BlockWidth, j * BlockHeight + i * fragmentHeight,
                         BlockWidth - 5, fragmentHeight};
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderDrawRect(ren, &rect);
        contact = contact->nextContact;
        j++;
      }
      block = block->nextBlock;
      i++;
    }
    SDL_RenderPresent(ren);
    SDL_Delay(1000 / FPS);
  }
  SDL_DestroyWindow(win);
  SDL_DestroyRenderer(ren);
  SDL_Quit();
  fclose(file);
  return 0;
}
