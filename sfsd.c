#include "islam.h"
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
typedef struct {
  int Red;
  int Green;
  int Blue;
} RGB;

typedef struct Contact {
  bool isDeleted;
  // j'ai ajouter 1 a chaque taille de string pour le caractere de fin de chaine
  char iD[9];
  char name[31];
  char phoneNumber[11];
  char email[31];
  char *otherInfo;
} Contact;

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
  int len = strlen(contactString);
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
  Block *block = fileinfo->firstBlock;
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

void CreateIndexFile(FileInfo *fileinfo, FILE *file) {
  Block *block = fileinfo->firstBlock;
  int i = 0;
  char *id = malloc(9 * sizeof(char));
  for (int i = 2; i < 10; i++) {
    id[i - 2] = fileinfo->firstBlock->Contacts[i];
  }
  fprintf(file, "%s,%p\n", id, block);
  while (block != NULL) {
    bool move = true;
    for (int j = 0; j < block->ocupiedSpace; j++) {
      if (block->Contacts[j] == '$') {
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
        fprintf(file, "%s,%p\n", id, block);
      }
    }
    if (move)
      block = block->nextBlock;
  }
  free(id);
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
  fileinfo->contactSize = 10;
  fileinfo->firstBlock = NULL;
  fileinfo->totalSize = 0;
  fillFile(fileinfo, file);
  FILE *indexFile = fopen("Contact_index.bin", "w+");
  CreateIndexFile(fileinfo, indexFile);
  fclose(indexFile);
  islam();
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
          Contact *contact = createContact("92049238");
          insertContactinBlock(fileinfo, contact);
          insertContactinFile(file, contact);
          fileinfo->contactSize++;
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
    int grayScale = 0;
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
  fclose(file);
  return 0;
}
