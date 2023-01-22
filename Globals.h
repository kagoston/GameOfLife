#pragma once

#ifndef GLOBALS_H
#define GLOBALS_H

#include <SDL.h>

// constants
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define GRID_SIZE 20
#define GRID_N_VERTICAL (int) (WINDOW_HEIGHT / GRID_SIZE)
#define GRID_N_HORIZONTAL (int) (WINDOW_WIDTH / GRID_SIZE)

// globals
SDL_Window* g_pWindow = nullptr;
SDL_Renderer* g_pRenderer = nullptr;
int g_iIsGameRunning = 0;
Uint32 g_iLastUpdate = 0;
float g_fSpeed = 0.5;

typedef int gameState[GRID_N_VERTICAL][GRID_N_HORIZONTAL];

gameState* pActualState = nullptr;
gameState* pNextState = nullptr;

int aActualState[GRID_N_VERTICAL][GRID_N_HORIZONTAL] = {};
int aNextState[GRID_N_VERTICAL][GRID_N_HORIZONTAL] = {};

#endif // GLOBALS_H
