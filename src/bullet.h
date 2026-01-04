#ifndef BULLET_H
#define BULLET_H

#include <SDL.h>

#define MAX_BULLETS 5

typedef struct {
  SDL_Rect rect;
  int active;
} Bullet;

/* Initialize an existing Bullet struct */
void initBullet(Bullet *bullet, int x, int y);

void updateBullets(Bullet *bullets, int *bulletCount, SDL_Renderer *renderer);
void fireBullet(Bullet *bullets, int *bulletCount, SDL_Rect *rect);
void cleanupBullets(Bullet *bullets, int *bulletCount);

#endif
