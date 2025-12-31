#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>

typedef struct {
  SDL_Texture *texture;
  SDL_Rect rect;
} Player;

Player *createPlayer(SDL_Renderer *renderer, const char *imagePath, int x,
                     int y);
void renderPlayer(SDL_Renderer *renderer, Player *player);
void renderPlayerFlipped(SDL_Renderer *renderer, Player *player,
                         SDL_RendererFlip flip);
void movePlayerLeft(Player *player, int speed);
void movePlayerRight(Player *player, int speed, int screenWidth);
void destroyPlayer(Player *player);

#endif
