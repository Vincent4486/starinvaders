#include "player.h"
#include <stdio.h>
#include <stdlib.h>

Player *createPlayer(SDL_Renderer *renderer, const char *imagePath, int x,
                     int y) {
  Player *player = (Player *)malloc(sizeof(Player));

  SDL_Surface *surface = SDL_LoadBMP(imagePath);
  if (!surface) {
    fprintf(stderr, "Could not load image %s: %s\n", imagePath, SDL_GetError());
    free(player);
    return NULL;
  }

  player->texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);

  if (!player->texture) {
    fprintf(stderr, "Could not create texture: %s\n", SDL_GetError());
    free(player);
    return NULL;
  }

  player->rect.x = x;
  player->rect.y = y;
  player->rect.w = 50;
  player->rect.h = 50;

  return player;
}

void renderPlayer(SDL_Renderer *renderer, Player *player) {
  SDL_RenderCopy(renderer, player->texture, NULL, &player->rect);
}

void renderPlayerFlipped(SDL_Renderer *renderer, Player *player,
                         SDL_RendererFlip flip) {
  SDL_RenderCopyEx(renderer, player->texture, NULL, &player->rect, 0, NULL,
                   flip);
}

void movePlayerLeft(Player *player, int speed) {
  if (player->rect.x - speed >= 0) {
    player->rect.x -= speed;
  }
}

void movePlayerRight(Player *player, int speed, int screenWidth) {
  if (player->rect.x + player->rect.w + speed <= screenWidth) {
    player->rect.x += speed;
  }
}

void destroyPlayer(Player *player) {
  if (player) {
    SDL_DestroyTexture(player->texture);
    free(player);
  }
}
