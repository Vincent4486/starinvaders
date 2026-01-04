#include "bullet.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initBullet(Bullet *bullet, int x, int y) {
    bullet->rect.x = x;
    bullet->rect.y = y;
    bullet->rect.w = 50;  // Bullet width
    bullet->rect.h = 50; // Bullet height
    bullet->active = 0;
}

void updateBullets(Bullet *bullets, int *bulletCount, SDL_Renderer* renderer) {
    int bulletSpeed = 7;
    
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            // Move bullet upward
            bullets[i].rect.y -= bulletSpeed;
            
            // Deactivate if bullet goes off screen
            if (bullets[i].rect.y < 0) {
                bullets[i].active = 0;
                (*bulletCount)--;
            } else {
                // Draw bullet using global textures
                SDL_RenderCopy(renderer, textures.bulletTexture, NULL, &bullets[i].rect);
            }
        }
    }
}

void fireBullet(Bullet *bullets, int *bulletCount, SDL_Rect* rect) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) {
            bullets[i].rect.x = rect->x;
            bullets[i].rect.y = rect->y;
            bullets[i].active = 1;
            (*bulletCount)++;
            break;
        }
    }
}

void cleanupBullets(Bullet *bullets, int *bulletCount) {
    // Deactivate all bullets and reset count
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].active = 0;
    }
    *bulletCount = 0;
}