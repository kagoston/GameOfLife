// Description: implementation of Game of live, a cellular automaton invented by Cambridge mathematician John Conway. 
//

#include "Globals.h"

#include <iostream>
#include <SDL.h>
#include <sstream>

void demonoid()
{
	(*pActualState)[0][1] = 1;

	(*pActualState)[1][2] = 1;

	(*pActualState)[2][0] = 1;
	(*pActualState)[2][1] = 1;
	(*pActualState)[2][2] = 1;
}

void behive()
{
	(*pActualState)[0][1] = 1;
	(*pActualState)[0][2] = 1;
	(*pActualState)[2][1] = 1;
	(*pActualState)[2][2] = 1;
	(*pActualState)[1][0] = 1;
	(*pActualState)[1][3] = 1;
}

void blinker()
{
	(*pActualState)[2][1] = 1;
	(*pActualState)[2][2] = 1;
	(*pActualState)[2][3] = 1;
}

void penthadecathlon()
{
	int iStartRow = 5;
	int iStartCol = 5;
	(*pActualState)[iStartRow + 1][iStartCol + 1] = 1;
	(*pActualState)[iStartRow + 1][iStartCol + 2] = 1;
	(*pActualState)[iStartRow + 1][iStartCol + 4] = 1;
	(*pActualState)[iStartRow + 1][iStartCol + 5] = 1;
	(*pActualState)[iStartRow + 1][iStartCol + 6] = 1;
	(*pActualState)[iStartRow + 1][iStartCol + 7] = 1;
	(*pActualState)[iStartRow + 1][iStartCol + 9] = 1;
	(*pActualState)[iStartRow + 1][iStartCol + 10] = 1;

	(*pActualState)[iStartRow + 0][iStartCol + 3] = 1;
	(*pActualState)[iStartRow + 2][iStartCol + 3] = 1;
	(*pActualState)[iStartRow + 0][iStartCol + 8] = 1;
	(*pActualState)[iStartRow + 2][iStartCol + 8] = 1;

}

void gliderByTheDozen()
{
	(*pActualState)[1][1] = 1;
	(*pActualState)[1][2] = 1;
	(*pActualState)[1][5] = 1;

	(*pActualState)[2][1] = 1;
	(*pActualState)[2][5] = 1;

	(*pActualState)[3][1] = 1;
	(*pActualState)[3][4] = 1;
	(*pActualState)[3][5] = 1;
}


void drawGrid(SDL_Renderer* _renderer)
{
	// draw horizontal lines
	for (int y = 0; y < WINDOW_HEIGHT; y += GRID_SIZE)
	{
		SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
		SDL_RenderDrawLine(_renderer, 0, y, WINDOW_WIDTH, y);
	}
	// draw vertical lines
	for (int x = 0; x < WINDOW_WIDTH; x += GRID_SIZE)
	{
		SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
		SDL_RenderDrawLine(_renderer, x, 0, x, WINDOW_HEIGHT);
	}

}

void showState(SDL_Renderer *_renderer)
{
	int iX1, iY1;
	for (int i = 0; i < GRID_N_VERTICAL; i++)
	{
		for (int j = 0; j < GRID_N_HORIZONTAL; j++)
		{
			if ((*pActualState)[i][j])
			{
				iX1 = j * GRID_SIZE;
				iY1 = i * GRID_SIZE;

				SDL_Rect rect = { iX1, iY1, GRID_SIZE, GRID_SIZE };

				SDL_SetRenderDrawColor(_renderer, 0, 255, 0, 255);
				SDL_RenderFillRect(_renderer, &rect);
			}
		}
	}
}


// update the game state using the rules
int updateState(float fDeltaTime)
{
	if (fDeltaTime <= g_fSpeed) { return 0; }
	
	int iAliveNeighbours = 0;

	for (int i = 0; i < GRID_N_VERTICAL; i++)
	{
		for (int j = 0; j < GRID_N_HORIZONTAL; j++)
		{
			// count the neighbours alive
			for (int iRow = i - 1; iRow <= i + 1; iRow++)
			{
				if (iRow < 0 || iRow > GRID_N_VERTICAL) { continue; }

				for (int iCol = j - 1; iCol <= j + 1; iCol++)
				{
					if (iCol < 0 || iCol > GRID_N_HORIZONTAL) { continue; }
					iAliveNeighbours += (*pActualState)[iRow][iCol];
				}
			}
			// increment with the cell itself
			iAliveNeighbours -= (*pActualState)[i][j];

			// define the state of the cell depending on the number of neighbours
			if ((*pActualState)[i][j])
			{
				// any live cell with fewer than two live neighbours dies (underpopulation)
				if (iAliveNeighbours < 2)
				{
					(*pNextState)[i][j] = 0;
				}
				// Any live cell with two or three live neighbours lives on to the next generation.
				if (iAliveNeighbours  < 4 && iAliveNeighbours > 1)
				{
					(*pNextState)[i][j] = 1;
				}
				// Any live cell with more than three live neighbours dies (overpopulation)
				if (iAliveNeighbours > 3)
				{
					(*pNextState)[i][j] = 0;
				}
			}
			else
			{
				// Any dead cell with exactly three live neighbours becomes a live cell (reproduction)
				if (iAliveNeighbours == 3)
				{
					(*pNextState)[i][j] = 1;
				}
				else
				{
					(*pNextState)[i][j] = 0;
				}
			}
			iAliveNeighbours = 0;
		}
	}
	gameState* pTemp = pActualState;
	pActualState = pNextState;
	pNextState = pTemp;
	return 1;
}


int initWindow()
{
	// if (SDL_Init(SDL_INIT_EVERYTHING) != 0)

	if (SDL_Init(SDL_INIT_VIDEO) != 0)		
	{
		std::cout << "SDL initializing error" << std::endl;
		return 0;
	}

	// create window
	g_pWindow = SDL_CreateWindow(
		"My SDL Empty Window",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		SDL_WINDOW_BORDERLESS
	);

	if (!g_pWindow)
	{
		std::cout << "window initializing error" << std::endl;
		return 0;
	}

	//create renderer
	g_pRenderer = SDL_CreateRenderer(g_pWindow, -1, 0);
	if (!g_pRenderer)
	{
		std::cout << "renderer initializing error" << std::endl;
		return 0;
	}

	return 1;
}


void processInput()
{
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type)
	{
		case SDL_QUIT:
		{
			g_iIsGameRunning = 0;
			break;
		}
		case SDL_KEYDOWN:
		{
			if (event.key.keysym.sym == SDLK_ESCAPE) { g_iIsGameRunning = 0; }
			break;
		}
		case SDL_MOUSEMOTION:
			int mouseX = event.motion.x;
			int mouseY = event.motion.y;

			std::stringstream ss;
			ss << "X: " << mouseX << " Y: " << mouseY;

			SDL_SetWindowTitle(g_pWindow, ss.str().c_str());
			break;

	}
}

void setup()
{
	pActualState = &aActualState;
	pNextState = &aNextState;
	// put the initial pattern into the grid

	behive();

}



void update()
{
	// the time since the last update
	float fDeltaTime = (SDL_GetTicks() - g_iLastUpdate) / 1000.0f;

	if (updateState(fDeltaTime))
	{
		g_iLastUpdate = SDL_GetTicks();

	}


}

void render()
{
	// clear window
	SDL_SetRenderDrawColor(g_pRenderer, 115, 115, 115, 255);
	SDL_RenderClear(g_pRenderer);

	// rendering
	showState(g_pRenderer);
	drawGrid(g_pRenderer);


	// render window
	SDL_RenderPresent(g_pRenderer);

}

void finish()
{
	SDL_DestroyRenderer(g_pRenderer);
	SDL_DestroyWindow(g_pWindow);
	SDL_Quit();
}


int main(int, char**)
{
	g_iIsGameRunning = initWindow();

	setup();

	while (g_iIsGameRunning)
	{
		processInput();
		update();
		render();
	}

	finish();

	return 0;
}
