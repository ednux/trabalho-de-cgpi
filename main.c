#ifdef __ANDROID__
	#include <SDL.h>
	#include <SDL_image.h>
	#include <SDL_ttf.h>
#else
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
	#include <SDL2/SDL_ttf.h>
#endif

#include <math.h>

#define PI 3.14159265

SDL_Rect ScreenRect = {0, 0, 800, 480};

int translacao(SDL_Point *point, int tx, int ty);
int escala(SDL_Point *point, int ref, int Sx, int Sy);
int espelhamento(SDL_Point *point, int ref);
int cisalhamento(SDL_Point *point, int ref, int Cx, int Cy);
int rotacao(SDL_Point *point, int ref, double alfa);

int iniciar(SDL_Window **window, SDL_Renderer **renderer);
int DrawLines(SDL_Renderer **renderer, SDL_Point *point);

int main(int argc, char **argv) {
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	SDL_Event e;
	SDL_Point point[3] = {
		{300,340},
		{500,340},
		{400,140}
	};
	
	unsigned int lastTime = 0, currentTime;
	int quit = 0;
	
	if (!iniciar(&window,&renderer)) {
		return 1;
	}
	
	if (TTF_Init() == -1) {
		SDL_Log("TTF_Init: %s\n", TTF_GetError());
		return 1;
	}

	while (!quit) {
		currentTime =  SDL_GetTicks();
		while(SDL_PollEvent(&e)) {
			switch(e.type) {
				case SDL_QUIT:
					quit = 1;
					break;
				case SDL_KEYDOWN:
					if (e.key.keysym.sym == SDLK_ESCAPE) {
						quit = 1;
						break;
					}
					break;
			}
		}
	
		if (currentTime > lastTime + 60) {
			lastTime = currentTime;
			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(renderer);
			SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, 0xFF);
			DrawLines(&renderer, point);
			SDL_RenderPresent(renderer);
		}
	}
	
	TTF_Quit();	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	
	return 0;
}
int translacao(SDL_Point *point, int tx, int ty)
{
	point[0].x = point[0].x + tx;
	point[0].y = point[0].y + ty;
	
	point[1].x = point[1].x + tx;
	point[1].y = point[1].y + ty;
	
	point[2].x = point[2].x + tx;
	point[2].y = point[2].y + ty;
	
	return 1;
}
int escala(SDL_Point *point, int ref, int Sx, int Sy)
{
	int tx, ty;
	
	tx = 0 - point[ref].x;
	ty = 0 - point[ref].y;
	
	translacao(point, tx, ty);
		
	point[0].x = point[0].x * Sx;
	point[0].y = point[0].y * Sy;
	
	point[1].x = point[1].x * Sx;
	point[1].y = point[1].y * Sy;
	
	point[2].x = point[2].x * Sx;
	point[2].y = point[2].y * Sy;
	
	tx = tx * (-1);
	ty = ty * (-1);
	
	translacao(point, tx, ty);
	
	return 1;
}
int espelhamento(SDL_Point *point, int ref)
{
	switch (ref) {
		//Somente em x
		case 0:
			point[0].x = point[0].x * (-1);
			point[1].x = point[1].x * (-1);
			point[2].x = point[2].x * (-1);
			break;
		//Somente em y
		case 1:
			point[0].y = point[0].y * (-1);
			point[1].y = point[1].y * (-1);
			point[2].y = point[2].y * (-1);
			break;
		//Em x e y
		case 2:
			point[0].x = point[0].x * (-1);
			point[0].y = point[0].y * (-1);
			
			point[1].x = point[1].x * (-1);
			point[1].y = point[1].y * (-1);
			
			point[2].x = point[2].x * (-1);
			point[2].y = point[2].y * (-1);
			break;
		default:
			return 0;
	}
	
	return 1;
}
int cisalhamento(SDL_Point *point, int ref, int Cx, int Cy)
{
	switch (ref) {
		//Somente em x
		case 0:
			point[0].x = point[0].x + (Cx * point[0].y);
			point[1].x = point[1].x + (Cx * point[1].y);
			point[2].x = point[2].x + (Cx * point[2].y);
			break;
		//Somente em y
		case 1:
			point[0].y = point[0].y + (Cy * point[0].x);
			point[1].y = point[1].y + (Cy * point[1].x);
			point[2].y = point[2].y + (Cy * point[2].x);
			break;
		//Em x e y
		case 2:
			point[0].x = point[0].x + (Cx * point[0].y);
			point[0].y = point[0].y + (Cy * point[0].x);
			
			point[1].x = point[1].x + (Cx * point[1].y);
			point[1].y = point[1].y + (Cy * point[1].x);
			
			point[2].x = point[2].x + (Cx * point[2].y);
			point[2].y = point[2].y + (Cy * point[2].x);
			break;
		default:
			return 0;
	}
	
	return 1;
}
int rotacao(SDL_Point *point, int ref, double alfa)
{
	int tx, ty;
	double val = PI / 180.0;
	double ax, ay,
				 bx, by,
				 cx, cy;
	
	tx = 0 - point[ref].x;
	ty = 0 - point[ref].y;
	
	translacao(point, tx, ty);
	
	ax = (point[0].x * cos(alfa * val)) + (sin(alfa * val) * point[0].y) * -1;
	ay = (point[0].x * sin(alfa * val)) + (cos(alfa * val) * point[0].y);
	
	point[0].x = round(ax);
	point[0].y = round(ay);
	
	bx = (point[1].x * cos(alfa * val)) + (sin(alfa * val) * point[1].y) * -1;
	by = (point[1].x * sin(alfa * val)) + (cos(alfa * val) * point[1].y);
	
	point[1].x = round(bx);
	point[1].y = round(by);
	
	cx = (point[2].x * cos(alfa * val)) + ((sin(alfa * val) * point[2].y) * -1);
	cy = (point[2].x * sin(alfa * val)) + (cos(alfa * val) * point[2].y);
	
	point[2].x = round(cx);
	point[2].y = round(cy);
	
	tx = tx * (-1);
	ty = ty * (-1);
	
	translacao(point, tx, ty);
	
	return 1;
}
int iniciar(SDL_Window **window, SDL_Renderer **renderer)
{
	int status = 1;
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		status = 0;
	}
	else {
		#ifdef __ANDROID__
			SDL_DisplayMode displayMode;
			if (SDL_GetCurrentDisplayMode(0, &displayMode) == 0) {
				ScreenRect.w = displayMode.w;
				ScreenRect.h = displayMode.h;
			}
		#endif
		
		// Create an application window with the following settings:
		*window = SDL_CreateWindow(
			"Transformações Computação Gráfica",										// window title
			SDL_WINDOWPOS_UNDEFINED,		// initial x position
			SDL_WINDOWPOS_UNDEFINED,		// initial y position
			ScreenRect.w,								// width, in pixels
			ScreenRect.h,								// height, in pixels
			SDL_WINDOW_SHOWN						// flags - see below
		);
	
		if (*window == NULL) {
			SDL_Log("Window could not be created! SDL Error: %s\n", SDL_GetError());
			status = 0;
		}
		else {
			*renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (*renderer == NULL) {
				SDL_Log( "Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				status = 0;
			}
			else { 	
				SDL_RenderSetLogicalSize(*renderer, 800, 480);
				SDL_SetRenderDrawColor(*renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			}
		}
	}
	return status;
}
int DrawLines(SDL_Renderer **renderer, SDL_Point *point)
{
	SDL_RenderDrawLine(*renderer, point[0].x, point[0].y, point[1].x, point[1].y);
	SDL_RenderDrawLine(*renderer, point[1].x, point[1].y, point[2].x, point[2].y);
	SDL_RenderDrawLine(*renderer, point[2].x, point[2].y, point[0].x, point[0].y);
	return 0;
}
