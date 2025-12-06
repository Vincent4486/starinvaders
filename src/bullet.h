#ifndef BULLET_H
#define BULLET_H

#include <SDL2/SDL.h>

#define MAX_BULLETS 100

typedef struct {
    SDL_Rect rect;
    int active;
} Bullet;

void updateBullets(Bullet* bullets, int* bulletCount, SDL_Renderer* renderer);
void fireBullet(Bullet* bullets, int* bulletCount, SDL_Rect* rect);

#endif
