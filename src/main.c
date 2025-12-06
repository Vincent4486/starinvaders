#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "bullet.h"
#include "player.h"

#define FPS 60
#define FRAME_DELAY (1000 / FPS)  // Milliseconds per frame
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

void handlePlayer(SDL_Renderer* renderer, Player* player, int speed) {
    // Get current keyboard state
    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A]) {
        movePlayerLeft(player, speed);
    }
    if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D]) {
        movePlayerRight(player, speed, SCREEN_WIDTH);
    }

    // Render player flipped vertically
    renderPlayerFlipped(renderer, player, SDL_FLIP_VERTICAL);
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Could not initialize SDL: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Window* window = SDL_CreateWindow("Star Invaders",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT,
                                          SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr, "Could not create window: %s\n", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Could not create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    // Rectangle position and size
    SDL_Rect rect = {400, 500, 50, 50};
    int speed = 5;

    // Bullets array
    Bullet bullets[MAX_BULLETS];
    int bulletCount = 0;
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].active = 0;
    }

    // Create player
    Player* player = createPlayer(renderer, "../res/spaceship.bmp", 400, 500);
    if (!player) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }

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
        const Uint8* keys = SDL_GetKeyboardState(NULL);
        if (keys[SDL_SCANCODE_SPACE] && !spacePressedLastFrame) {
            fireBullet(bullets, &bulletCount, &player->rect);
        }
        spacePressedLastFrame = keys[SDL_SCANCODE_SPACE];

        // Clear screen with black
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Update and draw bullets
        updateBullets(bullets, &bulletCount, renderer);

        // Handle player movement and rendering
        handlePlayer(renderer, player, speed);

        // Present the rendered frame
        SDL_RenderPresent(renderer);

        // Cap framerate at 60 FPS
        frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < FRAME_DELAY) {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }

    destroyPlayer(player);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}