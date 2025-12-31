#include "bullet.h"
#include "player.h"
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FPS 60
#define FRAME_DELAY (1000 / FPS) // Milliseconds per frame
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

void handlePlayer(SDL_Renderer *renderer, Player *player, int speed, const Uint8 *keys) {
  if (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A]) {
    movePlayerLeft(player, speed);
  }
  if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D]) {
    movePlayerRight(player, speed, SCREEN_WIDTH);
  }

  // Render player flipped vertically
  renderPlayerFlipped(renderer, player, SDL_FLIP_VERTICAL);
}

int main(int argc, char *argv[]) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    fprintf(stderr, "Could not initialize SDL: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  char *resLocation = "./res/";

  for (int args = 1; args < argc; args++) {
    if (strcmp(argv[args], "--help") == 0 ||
        strcmp(argv[args], "-h") == 0) {
      printf("Star Invaders Help:\n");
      printf("Use arrow keys or A/D to move the spaceship left and right.\n");
      printf("Press SPACE to fire bullets.\n");
      printf("Press ESC or close the window to exit the game.\n");
      printf("Use --res-location or -rl to specify the resource location.\n");
      SDL_Quit();
      return EXIT_SUCCESS;
    } else if (strcmp(argv[args], "--version") == 0 ||
               strcmp(argv[args], "-v") == 0) {
      printf("Star Invaders Version 1.0.0\n");
      SDL_Quit();
      return EXIT_SUCCESS;
    } else if (strcmp(argv[args], "--res-location") == 0 ||
               strcmp(argv[args], "-rl") == 0) {
      resLocation = argv[args + 1];
      args++;
    } else {
      printf("Unknown argument: %s\n", argv[args]);
      printf("Use --help or -h for usage information.\n");
      SDL_Quit();
      return EXIT_FAILURE;
    }
  }
  printf("Using resource location: %s\n", resLocation);

  // Build full paths for assets
  size_t pathLen;
  char *spaceshipPath;
  char *backgroundPath;
  const char *shipFile = "spaceship.bmp";
  const char *bgFile = "background.bmp";

  pathLen = strlen(resLocation) + strlen(shipFile) + 2;
  spaceshipPath = (char *)malloc(pathLen);
  if (!spaceshipPath) {
    fprintf(stderr, "Out of memory\n");
    SDL_Quit();
    return EXIT_FAILURE;
  }
  if (resLocation[strlen(resLocation) - 1] == '/') {
    snprintf(spaceshipPath, pathLen, "%s%s", resLocation, shipFile);
  } else {
    snprintf(spaceshipPath, pathLen, "%s/%s", resLocation, shipFile);
  }

  pathLen = strlen(resLocation) + strlen(bgFile) + 2;
  backgroundPath = (char *)malloc(pathLen);
  if (!backgroundPath) {
    free(spaceshipPath);
    fprintf(stderr, "Out of memory\n");
    SDL_Quit();
    return EXIT_FAILURE;
  }
  if (resLocation[strlen(resLocation) - 1] == '/') {
    snprintf(backgroundPath, pathLen, "%s%s", resLocation, bgFile);
  } else {
    snprintf(backgroundPath, pathLen, "%s/%s", resLocation, bgFile);
  }

  SDL_Window *window = SDL_CreateWindow("Star Invaders", SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
                                        SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (!window) {
    fprintf(stderr, "Could not create window: %s\n", SDL_GetError());
    SDL_Quit();
    return EXIT_FAILURE;
  }

  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer) {
    fprintf(stderr, "Could not create renderer: %s\n", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_FAILURE;
  }

  int speed = 5;

  // Bullets array
  Bullet bullets[MAX_BULLETS];
  int bulletCount = 0;
  for (int i = 0; i < MAX_BULLETS; i++) {
    bullets[i].active = 0;
  }

  // Create player
  Player *player = createPlayer(renderer, spaceshipPath, 400, 500);
  if (!player) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    free(spaceshipPath);
    free(backgroundPath);
    SDL_Quit();
    return EXIT_FAILURE;
  }

  // Load background
  SDL_Texture *background = NULL;
  SDL_Surface *bgSurface = SDL_LoadBMP(backgroundPath);
  if (!bgSurface) {
    fprintf(stderr, "Could not load background %s: %s\n", backgroundPath, SDL_GetError());
  } else {
    background = SDL_CreateTextureFromSurface(renderer, bgSurface);
    SDL_FreeSurface(bgSurface);
    if (!background) {
      fprintf(stderr, "Could not create background texture: %s\n", SDL_GetError());
    }
  }

  // Main loop flag
  int running = 1;
  SDL_Event event;
  Uint32 frameStart;
  int frameTime;
  int spacePressedLastFrame = 0;

  // Main loop
  while (running) {
    frameStart = SDL_GetTicks();

    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = 0;
      }
    }

    // Get current keyboard state for space key
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_SPACE] && !spacePressedLastFrame) {
      fireBullet(bullets, &bulletCount, &player->rect);
    }
    spacePressedLastFrame = keys[SDL_SCANCODE_SPACE];

    // Clear screen with black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    // Draw background if available
    if (background) {
      SDL_RenderCopy(renderer, background, NULL, NULL);
    }

    // Update and draw bullets
    updateBullets(bullets, &bulletCount, renderer);
    cleanupUnrenderedBullets(bullets, &bulletCount);

    // Handle player movement and rendering
    handlePlayer(renderer, player, speed, keys);

    // Present the rendered frame
    SDL_RenderPresent(renderer);

    // Cap framerate at 60 FPS
    frameTime = SDL_GetTicks() - frameStart;
    if (frameTime < FRAME_DELAY) {
      SDL_Delay(FRAME_DELAY - frameTime);
    }
  }

  destroyPlayer(player);
  if (background) {
    SDL_DestroyTexture(background);
  }
  free(spaceshipPath);
  free(backgroundPath);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return EXIT_SUCCESS;
}