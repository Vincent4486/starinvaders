#include "player.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>

int playerSpeed = 5;

void handlePlayer(SDL_Renderer *renderer, Player *player, int playerSpeed, const Uint8 *keys) {
  if (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A]) {
    movePlayerLeft(player, playerSpeed);
  }
  if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D]) {
    movePlayerRight(player, playerSpeed, SCREEN_WIDTH);
  }

  // Render player flipped vertically
  renderPlayerFlipped(renderer, player, SDL_FLIP_VERTICAL);
}

Player *createPlayer(int x, int y) {
  Player *player = (Player *)malloc(sizeof(Player));

  player->rect.x = x;
  player->rect.y = y;
  player->rect.w = 50;
  player->rect.h = 50;

  return player;
}

void renderPlayer(SDL_Renderer *renderer, Player *player) {
  SDL_RenderCopy(renderer, textures.playerTexture, NULL, &player->rect);
}

void renderPlayerFlipped(SDL_Renderer *renderer, Player *player,
                         SDL_RendererFlip flip) {
  SDL_RenderCopyEx(renderer, textures.playerTexture, NULL, &player->rect, 0, NULL,
                   flip);
}

void movePlayerLeft(Player *player, int playerSpeed) {
  if (player->rect.x - playerSpeed >= 0) {
    player->rect.x -= playerSpeed;
  }
}

void movePlayerRight(Player *player, int playerSpeed, int screenWidth) {
  if (player->rect.x + player->rect.w + playerSpeed <= screenWidth) {
    player->rect.x += playerSpeed;
  }
}

void destroyPlayer(Player *player) {
  if (player) {
    free(player);
  }
}
