#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>

typedef struct {
  SDL_Rect rect;
} Player;

extern int playerSpeed;

void handlePlayer(SDL_Renderer *renderer, Player *player, int playerSpeed, const Uint8 *keys);

Player *createPlayer(int x, int y);
void renderPlayer(SDL_Renderer *renderer, Player *player);
void renderPlayerFlipped(SDL_Renderer *renderer, Player *player,
                         SDL_RendererFlip flip);
void movePlayerLeft(Player *player, int playerSpeed);
void movePlayerRight(Player *player, int playerSpeed, int screenWidth);
void destroyPlayer(Player *player);

#endif
