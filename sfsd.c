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
// the number of lignes in each block
#define blockSegments 8
#define numberOfContacts 30
typedef struct {
  bool isDeleted;
  // j'ai ajouter 1 a chaque taille de string pour le caractere de fin de chaine
  char iD[9];
  char name[31];
  char phoneNumber[11];
  char email[31];
  char *otherInfo;
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

void fillFile(FILE *file) {
  for (int i = 0; i < numberOfContacts; i++) {
    long iD = ((i + 1) * 10000019) % 100000000;
    char ID[9];
    sprintf(ID, "%08ld", iD);
    Contact *contact = createContact(ID);
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
// create a new block
Block *addBlock(FileInfo *fileinfo) {
  FileInfo *tmp = fileinfo;
  while (fileinfo->firstBlock->nextBlock != NULL) {
    fileinfo->firstBlock = fileinfo->firstBlock->nextBlock;
  }
  Block *block = malloc(sizeof(Block));
  fileinfo->totalSize++;
  block->blockNumber = fileinfo->totalSize;
  block->ocupiedSpace = 0;
  block->nextBlock = NULL;
  block->Contact = malloc(blockSegments * sizeof(Contact));
  return block;
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
  fillFile(file);
  FileInfo fileinfo;
  fileinfo.contactSize = 41;
  fileinfo.totalSize = 0;
  bool running = true;
  SDL_Event event;
  // FIX:account for every division it could be a floating number
  int Blockrows = ((fileinfo.contactSize / blockSegments) / maxBlockCols);
  float extraBlock = fileinfo.contactSize % blockSegments;
  if (extraBlock != 0) {
    Blockrows++;
  }
  // if (fileinfo.totalSize % blockSegments == 0) {
  //   BlockCol++;
  // }
  int BlockWidth = WINDOW_WIDTH / maxBlockCols;
  int BlockHeight = WINDOW_HEIGHT / Blockrows;
  SDL_RenderClear(ren);
  // WARNING: ida bdlt haja fi l bocks mtnsach tdkhol had l code ll loop
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
    for (int i = 0; i <= maxBlockCols; i++) {
      for (int j = 0; j <= Blockrows; j++) {
        for (int k = 0; k < blockSegments; k++) {
          // divide each block into blockSegments number of lignes
          int fragmentHeight = (BlockHeight - 5) / blockSegments;
          SDL_Rect rect = {i * BlockWidth, j * BlockHeight + k * fragmentHeight,
                           BlockWidth - 5, fragmentHeight};
          SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
          SDL_RenderDrawRect(ren, &rect);
        }
      }
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
