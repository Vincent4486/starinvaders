#include "bullet.h"

void updateBullets(Bullet* bullets, int* bulletCount, SDL_Renderer* renderer) {
    int bulletSpeed = 7;
    
    for (int i = 0; i < *bulletCount; i++) {
        if (bullets[i].active) {
            // Move bullet upward
            bullets[i].rect.y -= bulletSpeed;
            
            // Deactivate if bullet goes off screen
            if (bullets[i].rect.y < 0) {
                bullets[i].active = 0;
            } else {
                // Draw bullet
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                SDL_RenderFillRect(renderer, &bullets[i].rect);
            }
        }
    }
}

void fireBullet(Bullet* bullets, int* bulletCount, SDL_Rect* rect) {
    if (*bulletCount < MAX_BULLETS) {
        bullets[*bulletCount].rect.x = rect->x + rect->w / 2 - 2;
        bullets[*bulletCount].rect.y = rect->y - 10;
        bullets[*bulletCount].rect.w = 4;
        bullets[*bulletCount].rect.h = 10;
        bullets[*bulletCount].active = 1;
        (*bulletCount)++;
    }
}
