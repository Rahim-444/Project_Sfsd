#include <SDL2/SDL.h>
#include <SDL2/SDL_filesystem.h>
#include <stdbool.h>
#include <stdio.h>

// you can change these values and the changes will happen in the app
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define maxBlockRows 2
#define blockSegments 8

typedef struct {
  char filename[20];
  int totalSize;
  int numSegments;
} FileInfo;

typedef struct {
} enregistement;

typedef struct {
  int segmentNumber;
  int blockSize;
  enregistement *enregistements;
} BlockSegment;

int main(int argc, char *argv[]) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    printf("error initializing SDL: %s\n", SDL_GetError());
    return 1;
  }
  SDL_Window *win = SDL_CreateWindow("Project sfsd", SDL_WINDOWPOS_CENTERED,
                                     SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH,
                                     WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
  SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
  FileInfo fileinfo;
  fileinfo.numSegments = 50;
  // FILE *file = fopen("file.txt", "r");
  // char *line = malloc(100 * sizeof(char));
  // while (fgets(line, 100, file) != NULL) {
  //   fileinfo.numSegments++;
  // }
  // rewind(file);
  bool running = true;
  SDL_Event event;
  int BlockCol = ((fileinfo.numSegments / blockSegments) / maxBlockRows);
  float extraBlock = fileinfo.numSegments % blockSegments;
  if (extraBlock != 0) {
    BlockCol++;
  }
  if (fileinfo.numSegments % blockSegments == 0) {
    BlockCol++;
  }
  int BlockWidth = WINDOW_WIDTH / BlockCol;
  int BlockHeight = WINDOW_HEIGHT / maxBlockRows;
  SDL_RenderClear(ren);
  // rednering the blocks outside the loop because they are static
  // WARNING: ida bdlt haja fi l bocks mtnsach tdkhol had l code ll loop
  for (int i = 0; i <= maxBlockRows; i++) {
    for (int j = 0; j <= BlockCol; j++) {
      SDL_Rect rect = {i * BlockWidth, j * BlockHeight, BlockWidth - 5,
                       BlockHeight - 10};
      // draw the boarder of the rectangle
      int grayScale = 50;

      SDL_SetRenderDrawColor(ren, grayScale, grayScale, grayScale, 255);
      SDL_RenderFillRect(ren, &rect);
      SDL_RenderPresent(ren);
    }
  }
  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = false;
      }
    }
    for (int i = 0; i <= maxBlockRows; i++) {
      for (int j = 0; j <= BlockCol; j++) {
        for (int k = 0; k < blockSegments; k++) {
          // divide each block into blockSegments number of lignes
          int fragmentHeight = (BlockHeight - 10) / blockSegments;
          SDL_Rect rect = {i * BlockWidth, j * BlockHeight + k * fragmentHeight,
                           BlockWidth - 5, (BlockHeight / blockSegments) - 2};
          SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
          SDL_RenderDrawRect(ren, &rect);
          SDL_RenderPresent(ren);
        }
      }
    }
  }
  SDL_DestroyWindow(win);
  SDL_DestroyRenderer(ren);
  SDL_Quit();
  // fclose(file);
  return 0;
}
