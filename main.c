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

int translacao(SDL_Point *point, int tx, int ty);
int escala(SDL_Point *point, int ref, int Sx, int Sy);
int espelhamento(SDL_Point *point, int ref);
int cisalhamento(SDL_Point *point, int ref, int Cx, int Cy);
int rotacao(SDL_Point *point, int ref, double alfa);

int main(void) {
	int i;
	SDL_Point point[3] = {
		{5,4}, //A
		{1,4}, //B
		{5,0}  //C
	};
	
	rotacao(point, 0, 30);
	
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
