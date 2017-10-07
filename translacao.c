#ifdef __ANDROID__
	#include <SDL.h>
	#include <SDL_image.h>
	#include <SDL_ttf.h>
#else
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
	#include <SDL2/SDL_ttf.h>
#endif

int translacao(SDL_Point *point, int tx, int ty);
int escala(SDL_Point *point, int ref, int Sx, int Sy);

int main(void) {
	int i;
	SDL_Point point[3] = {
		{4,4}, //A
		{8,6}, //B
		{6,8}  //C
	};
	
	escala(point, 0, 2, 2);
	
	for (i = 0; i < 3; i++) {
		printf("[%d,%d]\n", point[i].x, point[i].y);
	}	
	
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
	int tx, ty, i;
	
	tx = 0 - point[ref].x;
	ty = 0 - point[ref].y;
	
	translacao(point, tx, ty);
	
	printf("tx %d, ty = %d\n", tx, ty);
	
	for (i = 0; i < 3; i++) {
		printf("[%d,%d]\n", point[i].x, point[i].y);
	}
	
	printf("\n");
		
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
