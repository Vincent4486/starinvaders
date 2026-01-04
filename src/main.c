#include "bullet.h"
#include "game.h"
#include "player.h"
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Textures textures = {0}; // global textures storage (initialized to NULL pointers)

int loadTextures(SDL_Renderer *renderer, AssetPaths *paths) {
  SDL_Surface *bulletSurface = SDL_LoadBMP(paths->bulletPath);
  if (!bulletSurface) {
    fprintf(stderr, "Could not load bullet image: %s\n", SDL_GetError());
    return 0;
  }

  textures.bulletTexture = SDL_CreateTextureFromSurface(renderer, bulletSurface);
  SDL_FreeSurface(bulletSurface);
  if (!textures.bulletTexture) {
    fprintf(stderr, "Could not create bullet texture: %s\n", SDL_GetError());
    return 0;
  }

  SDL_Surface *playerSurface = SDL_LoadBMP(paths->spaceshipPath);
  if (!playerSurface) {
    fprintf(stderr, "Could not load player image: %s\n", SDL_GetError());
    SDL_DestroyTexture(textures.bulletTexture);
    textures.bulletTexture = NULL;
    return 0;
  }

  textures.playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);
  SDL_FreeSurface(playerSurface);
  if (!textures.playerTexture) {
    fprintf(stderr, "Could not create player texture: %s\n", SDL_GetError());
    SDL_DestroyTexture(textures.bulletTexture);
    textures.bulletTexture = NULL;
    return 0;
  }

  /* Load background (non-fatal) */
  SDL_Surface *bgSurface = SDL_LoadBMP(paths->backgroundPath);
  if (!bgSurface) {
    fprintf(stderr, "Could not load background %s: %s\n", paths->backgroundPath, SDL_GetError());
    textures.backgroundTexture = NULL;
  } else {
    textures.backgroundTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
    SDL_FreeSurface(bgSurface);
    if (!textures.backgroundTexture) {
      fprintf(stderr, "Could not create background texture: %s\n", SDL_GetError());
      textures.backgroundTexture = NULL;
    }
  }

  return 1;
}

/*
 * Parse command-line arguments. Returns:
 *  0 -> continue normally
 *  1 -> printed help/version, should exit success
 * -1 -> error in args, should exit failure
 */
static int handleArguments(int argc, char *argv[], char **resLocation) {
  for (int args = 1; args < argc; args++) {
    if (strcmp(argv[args], "--help") == 0 || strcmp(argv[args], "-h") == 0) {
      printf("Star Invaders Help:\n");
      printf("Use arrow keys or A/D to move the spaceship left and right.\n");
      printf("Press SPACE to fire bullets.\n");
      printf("Press ESC or close the window to exit the game.\n");
      printf("Use --res-location or -rl to specify the resource location.\n");
      return 1;
    } else if (strcmp(argv[args], "--version") == 0 || strcmp(argv[args], "-v") == 0) {
      printf("Star Invaders Version 1.0.0\n");
      return 1;
    } else if (strcmp(argv[args], "--res-location") == 0 || strcmp(argv[args], "-rl") == 0) {
      if (args + 1 >= argc) {
        fprintf(stderr, "Missing value for %s\n", argv[args]);
        printf("Use --help or -h for usage information.\n");
        return -1;
      }
      *resLocation = argv[++args];
    } else {
      printf("Unknown argument: %s\n", argv[args]);
      printf("Use --help or -h for usage information.\n");
      return -1;
    }
  }
  return 0;
}

AssetPaths *getResources(const char *resLocation) {
  AssetPaths *paths = (AssetPaths *)malloc(sizeof(AssetPaths));
  if (!paths) {
    fprintf(stderr, "Out of memory\n");
    return NULL;
  }

  size_t pathLen;
  const char *shipFile = "spaceship.bmp";
  const char *bgFile = "background.bmp";
  const char *bulletFile = "bullet.bmp";

  pathLen = strlen(resLocation) + strlen(shipFile) + 2;
  paths->spaceshipPath = (char *)malloc(pathLen);
  if (!paths->spaceshipPath) {
    free(paths);
    fprintf(stderr, "Out of memory\n");
    return NULL;
  }
  if (resLocation[strlen(resLocation) - 1] == '/') {
    snprintf(paths->spaceshipPath, pathLen, "%s%s", resLocation, shipFile);
  } else {
    snprintf(paths->spaceshipPath, pathLen, "%s/%s", resLocation, shipFile);
  }

  pathLen = strlen(resLocation) + strlen(bgFile) + 2;
  paths->backgroundPath = (char *)malloc(pathLen);
  if (!paths->backgroundPath) {
    free(paths->spaceshipPath);
    free(paths);
    fprintf(stderr, "Out of memory\n");
    return NULL;
  }
  if (resLocation[strlen(resLocation) - 1] == '/') {
    snprintf(paths->backgroundPath, pathLen, "%s%s", resLocation, bgFile);
  } else {
    snprintf(paths->backgroundPath, pathLen, "%s/%s", resLocation, bgFile);
  }

  pathLen = strlen(resLocation) + strlen(bulletFile) + 2;
  paths->bulletPath = (char *)malloc(pathLen);
  if (!paths->bulletPath) {
    free(paths->backgroundPath);
    free(paths->spaceshipPath);
    free(paths);
    fprintf(stderr, "Out of memory\n");
    return NULL;
  }
  if (resLocation[strlen(resLocation) - 1] == '/') {
    snprintf(paths->bulletPath, pathLen, "%s%s", resLocation, bulletFile);
  } else {
    snprintf(paths->bulletPath, pathLen, "%s/%s", resLocation, bulletFile);
  }

  return paths;
}

int main(int argc, char *argv[]) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    fprintf(stderr, "Could not initialize SDL: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  char *resLocation = "./res/";
  int argResult = handleArguments(argc, argv, &resLocation);
  if (argResult == 1) {
    SDL_Quit();
    return EXIT_SUCCESS;
  } else if (argResult == -1) {
    SDL_Quit();
    return EXIT_FAILURE;
  }

  AssetPaths *paths = getResources(resLocation);
  if (!paths) {
    fprintf(stderr, "Could not load resources\n");
    SDL_Quit();
    return EXIT_FAILURE;
  }

  SDL_Window *window = SDL_CreateWindow("Star Invaders", SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
                                        SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (!window) {
    fprintf(stderr, "Could not create window: %s\n", SDL_GetError());
    free(paths->backgroundPath);
    free(paths->bulletPath);
    free(paths->spaceshipPath);
    free(paths);
    SDL_Quit();
    return EXIT_FAILURE;
  }

  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer) {
    fprintf(stderr, "Could not create renderer: %s\n", SDL_GetError());
    SDL_DestroyWindow(window);
    free(paths->backgroundPath);
    free(paths->bulletPath);
    free(paths->spaceshipPath);
    free(paths);
    SDL_Quit();
    return EXIT_FAILURE;
  }

  if (!loadTextures(renderer, paths)) {
    fprintf(stderr, "Could not create game textures: %s\n", SDL_GetError());
    free(paths->backgroundPath);
    free(paths->bulletPath);
    free(paths->spaceshipPath);
    free(paths);
    SDL_Quit();
    return EXIT_FAILURE;
  }

  // Bullets array
  int bulletCount = 0;
  Bullet bullets[MAX_BULLETS];
  for (int i = 0; i < MAX_BULLETS; i++) {
    initBullet(&bullets[i], 0, 0);
  }

  // Create player
  Player *player = createPlayer(400, 500);
  if (!player) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    free(paths->spaceshipPath);
    free(paths->backgroundPath);
    free(paths);
    if (textures.bulletTexture) SDL_DestroyTexture(textures.bulletTexture);
    if (textures.playerTexture) SDL_DestroyTexture(textures.playerTexture);
    SDL_Quit();
    return EXIT_FAILURE;
  }

  /* background is loaded into textures.backgroundTexture by loadTextures */

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
    if (textures.backgroundTexture) {
      SDL_RenderCopy(renderer, textures.backgroundTexture, NULL, NULL);
    }

    // Update and draw bullets
    updateBullets(bullets, &bulletCount, renderer);
    // cleanupUnrenderedBullets(bullets, &bulletCount); // removed

    // Handle player movement and rendering
    handlePlayer(renderer, player, playerSpeed, keys);

    // Present the rendered frame
    SDL_RenderPresent(renderer);

    // Cap framerate at 60 FPS
    frameTime = SDL_GetTicks() - frameStart;
    if (frameTime < FRAME_DELAY) {
      SDL_Delay(FRAME_DELAY - frameTime);
    }
  }

  destroyPlayer(player);

  // Cleanup bullets (deactivate all)
  cleanupBullets(bullets, &bulletCount);

  if (textures.bulletTexture) SDL_DestroyTexture(textures.bulletTexture);
  if (textures.playerTexture) SDL_DestroyTexture(textures.playerTexture);
  if (textures.backgroundTexture) SDL_DestroyTexture(textures.backgroundTexture);

  free(paths->spaceshipPath);
  free(paths->backgroundPath);
  free(paths);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return EXIT_SUCCESS;
}