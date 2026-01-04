#ifndef GAME_H
#define GAME_H

#include <SDL.h>

#define FPS 60
#define FRAME_DELAY (1000 / FPS) // Milliseconds per frame
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

typedef struct {
  char *spaceshipPath;
  char *backgroundPath;
  char *bulletPath;
} AssetPaths;

typedef struct {
    SDL_Texture *bulletTexture;
    SDL_Texture *playerTexture;
    SDL_Texture *backgroundTexture;
} Textures;

extern Textures textures;

#endif /* GAME_H */