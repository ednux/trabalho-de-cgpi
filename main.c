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
#define FONT "FreeSans.ttf"

typedef enum {intro, show, translation, scale, rotate, mirror, shearing} GameState;
typedef enum {pA, pB, pC} PState;
typedef enum {pX, pY, pRef} PStateP;

SDL_Rect ScreenRect = {0, 0, 800, 480};

int translacao(SDL_Point *point, int tx, int ty);
int escala(SDL_Point *point, int ref, double Sx, double Sy);
int espelhamento(SDL_Point *point, int ref);
int cisalhamento(SDL_Point *point, int ref, int Cx, int Cy);
int rotacao(SDL_Point *point, int ref, double alfa);

int iniciar(SDL_Window **window, SDL_Renderer **renderer);
int DrawLines(SDL_Renderer **renderer, SDL_Point *point);
int renderFont(SDL_Texture **texture, SDL_Renderer **renderer, TTF_Font *font, char *text, SDL_Rect *pos);
int mapear(SDL_Point *point);
void removerCaractere(char *text);

int main(int argc, char **argv) {
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	SDL_Texture *texture = NULL;
	SDL_Texture *texture2 = NULL;
	TTF_Font *font = NULL;
	SDL_Event e;
	SDL_Rect pos = {0,0,0,0};
	SDL_Rect pos2 = {0,0,0,0};
	/*
	SDL_Point point[3] = {
		{0,0}, //A
		{0,0}, //B
		{0,0}  //C
	};
	*/
	SDL_Point point[3] = {
		{300,340},
		{500,340},
		{400,140}
	};


	unsigned int lastTime = 0, currentTime;
	int quit = 0;
	int temValor = 0;
	int tx, ty, ref;
	double sX, sY, alfa, tmp = 0;
	char text[20] = "Ponto Ax: ";
	char ctext[20] = "";

	GameState gstate = 1;
	PState pstate = pA;
	PStateP pstateP = pX;

	if (!iniciar(&window,&renderer)) {
		return 1;
	}

	if (TTF_Init() == -1) {
		SDL_Log("TTF_Init: %s\n", TTF_GetError());
		return 1;
	}

	font = TTF_OpenFont(FONT, 28);

	if (font == NULL)
		return 1;

	SDL_StartTextInput();

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
					if (gstate != show) {
						if (e.key.keysym.sym == SDLK_RETURN) {
							temValor = 1;
							break;
						}
						if (e.key.keysym.sym == SDLK_BACKSPACE && strlen(ctext) > 0 ) {
							removerCaractere(ctext);
							break;
						}
					}
					else {
						switch (e.key.keysym.sym) {
							case SDLK_1:
								gstate = translation;
								strcpy(text, "Tx: ");
								strcpy(ctext, "");
								temValor = 0;
								pstateP = pX;
								tmp = 0;
							break;
							case SDLK_2:
								gstate = scale;
								strcpy(text, "Ref: ");
								strcpy(ctext, "");
								temValor = 0;
								pstateP = pRef;
								tmp = 0;
							break;
							case SDLK_3:
								gstate = rotate;
								strcpy(text, "Ref: ");
								strcpy(ctext, "");
								temValor = 0;
								pstateP = pRef;
								tmp = 0;
							break;
							case SDLK_4:
								gstate = mirror;
								strcpy(text, "Ref: ");
								strcpy(ctext, "");
								temValor = 0;
								pstateP = pRef;
								tmp = 0;
							break;
							case SDLK_5:
								gstate = shearing;
							break;
							default:
							break;
						}
					}
					break;
				case SDL_TEXTINPUT:
					if (strlen(ctext) < 20)
						strcat(ctext, e.text.text);
					break;
			}
		}


		if (currentTime > lastTime + 60) {
			lastTime = currentTime;
			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(renderer);

			switch (gstate) {
				case 0:
					switch (pstate) {
						case pA:
							if (pstateP == 0) {
								if (temValor) {
									temValor = 0;
									tmp = atoi(ctext);
									point[0].x = tmp;
									pstateP = pY;
									strcpy(text, "Ponto Ay: ");
									strcpy(ctext, "");
									tmp = 0;
								}
							}
							else {
								if (pstateP == 1) {
									if (temValor) {
										temValor = 0;
										tmp = atoi(ctext);
										point[0].y = tmp;
										pstateP = pX;
										pstate = pB;
										strcpy(text, "Ponto Bx: ");
										strcpy(ctext, "");
										tmp = 0;
									}
								}
							}
						break;
						case pB:
							if (pstateP == 0) {
								if (temValor) {
									temValor = 0;
									tmp = atoi(ctext);
									point[1].x = tmp;
									pstateP = pY;
									strcpy(text, "Ponto By: ");
									strcpy(ctext, "");
									tmp = 0;
								}
							}
							else {
								if (pstateP == 1) {
									if (temValor) {
										temValor = 0;
										tmp = atoi(ctext);
										point[1].y = tmp;
										pstateP = pX;
										pstate = pC;
										strcpy(text, "Ponto Cx: ");
										strcpy(ctext, "");
										tmp = 0;
									}
								}
							}
						break;
						case pC:
							if (pstateP == 0) {
								if (temValor) {
									temValor = 0;
									tmp = atoi(ctext);
									point[2].x = tmp;
									pstateP = pY;
									strcpy(text, "Ponto Cy: ");
									strcpy(ctext, "");
									tmp = 0;
								}
							}
							else {
								if (pstateP == 1) {
									if (temValor) {
										temValor = 0;
										tmp = atoi(ctext);
										point[2].y = tmp;
										pstateP = pX;
										pstate = pA;
										strcpy(text, "");
										strcpy(ctext, "");
										gstate = show;
										tmp = 0;
										//SDL_StopTextInput();
									}
								}
							}
						break;
						default:
						break;
					}
					pos2.x = (ScreenRect.w - pos2.w) / 2;
					pos2.y = (ScreenRect.h - pos2.h) / 2;

					renderFont(&texture2, &renderer, font, text, &pos2);

					pos.x = pos2.x + pos2.w + 5;
					pos.y = pos2.y;

					renderFont(&texture, &renderer, font, ctext, &pos);

					SDL_SetRenderDrawColor(renderer,255,255,255,255);
					if (strlen(ctext) > 0 )
						SDL_RenderCopy(renderer,texture, NULL, &pos);
					SDL_RenderCopy(renderer,texture2, NULL, &pos2);
				break;
				case show:
					SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, 0xFF);
					DrawLines(&renderer, point);
				break;
				case translation:
					if (pstateP == 0) {
						if (temValor) {
							temValor = 0;
							tmp = atoi(ctext);
							tx = tmp;
							pstateP = pY;
							strcpy(text, "Ty: ");
							strcpy(ctext, "");
							tmp = 0;
						}
					}
					else {
						if (pstateP == 1) {
							if (temValor) {
								temValor = 0;
								tmp = atoi(ctext);
								ty = tmp;
								pstateP = pX;
								strcpy(text, "");
								strcpy(ctext, "");
								translacao(point, tx, ty);
								gstate = show;
								tmp = 0;
							}
						}
					}
					pos2.x = (ScreenRect.w - pos2.w) / 2;
					pos2.y = (ScreenRect.h - pos2.h) / 2;

					renderFont(&texture2, &renderer, font, text, &pos2);

					pos.x = pos2.x + pos2.w + 5;
					pos.y = pos2.y;

					renderFont(&texture, &renderer, font, ctext, &pos);

					SDL_SetRenderDrawColor(renderer,255,255,255,255);
					if (strlen(ctext) > 0 )
						SDL_RenderCopy(renderer,texture, NULL, &pos);
					SDL_RenderCopy(renderer,texture2, NULL, &pos2);
				break;
				case scale:
					if (pstateP == pRef) {
						if (temValor) {
							temValor = 0;
							tmp = atoi(ctext);
							ref = tmp;
							pstateP = pX;
							strcpy(text, "Sx: ");
							strcpy(ctext, "");
							tmp = 0;
						}
					}
					else {
						if (pstateP == pX) {
							if (temValor) {
								temValor = 0;
								tmp = atof(ctext);
								sX = tmp;
								pstateP = pY;
								strcpy(text, "Sy: ");
								strcpy(ctext, "");
								tmp = 0;
							}
						}
						else {
							if (pstateP == pY) {
								if (temValor) {
									temValor = 0;
									tmp = atof(ctext);
									sY = tmp;
									pstateP = pX;
									strcpy(text, "");
									strcpy(ctext, "");
									escala(point, ref, sX, sY);
									gstate = show;
									tmp = 0;
									break;
								}
							}
						}
					}
					pos2.x = (ScreenRect.w - pos2.w) / 2;
					pos2.y = (ScreenRect.h - pos2.h) / 2;

					renderFont(&texture2, &renderer, font, text, &pos2);

					pos.x = pos2.x + pos2.w + 5;
					pos.y = pos2.y;

					renderFont(&texture, &renderer, font, ctext, &pos);

					SDL_SetRenderDrawColor(renderer,255,255,255,255);
					if (strlen(ctext) > 0 )
						SDL_RenderCopy(renderer,texture, NULL, &pos);
					SDL_RenderCopy(renderer,texture2, NULL, &pos2);
					break;
				case rotate:
					if (pstateP == pRef) {
						if (temValor) {
							temValor = 0;
							tmp = atoi(ctext);
							ref = tmp;
							pstateP = pX;
							strcpy(text, "Angulo: ");
							strcpy(ctext, "");
							tmp = 0;
						}
					}
					else {
						if (pstateP == pX) {
							if (temValor) {
								temValor = 0;
								tmp = atof(ctext);
								alfa = tmp;
								pstateP = pX;
								strcpy(text, "");
								strcpy(ctext, "");
								rotacao(point,ref,alfa);
								gstate = show;
								tmp = 0;
								break;
							}
						}
					}
					pos2.x = (ScreenRect.w - pos2.w) / 2;
					pos2.y = (ScreenRect.h - pos2.h) / 2;

					renderFont(&texture2, &renderer, font, text, &pos2);

					pos.x = pos2.x + pos2.w + 5;
					pos.y = pos2.y;

					renderFont(&texture, &renderer, font, ctext, &pos);

					SDL_SetRenderDrawColor(renderer,255,255,255,255);
					if (strlen(ctext) > 0 )
						SDL_RenderCopy(renderer,texture, NULL, &pos);
					SDL_RenderCopy(renderer,texture2, NULL, &pos2);
					break;
				case mirror:
					if (pstateP == pRef) {
						if (temValor) {
							temValor = 0;
							tmp = atoi(ctext);
							ref = tmp;
							pstateP = pX;
							strcpy(text, "");
							strcpy(ctext, "");
							espelhamento(point, ref);
							gstate = show;
							tmp = 0;
							break;
						}
					}
					pos2.x = (ScreenRect.w - pos2.w) / 2;
					pos2.y = (ScreenRect.h - pos2.h) / 2;
					renderFont(&texture2, &renderer, font, text, &pos2);
					pos.x = pos2.x + pos2.w + 5;
					pos.y = pos2.y;
					renderFont(&texture, &renderer, font, ctext, &pos);
					SDL_SetRenderDrawColor(renderer,255,255,255,255);
					if (strlen(ctext) > 0 )
						SDL_RenderCopy(renderer,texture, NULL, &pos);
					SDL_RenderCopy(renderer,texture2, NULL, &pos2);
					break;
				case shearing:
					cisalhamento(point, 1, 0, 1);
					gstate = show;
					break;
				default:
				break;
			}

			SDL_RenderPresent(renderer);
		}
	}

	TTF_CloseFont(font);
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
int escala(SDL_Point *point, int ref, double Sx, double Sy)
{
	int tx, ty;

	tx = 0 - point[ref].x;
	ty = 0 - point[ref].y;

	translacao(point, tx, ty);

	point[0].x = round(point[0].x * Sx);
	point[0].y = round(point[0].y * Sy);

	point[1].x = round(point[1].x * Sx);
	point[1].y = round(point[1].y * Sy);

	point[2].x = round(point[2].x * Sx);
	point[2].y = round(point[2].y * Sy);

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
int renderFont(SDL_Texture **texture, SDL_Renderer **renderer, TTF_Font *font, char *text, SDL_Rect *pos)
{
	SDL_Surface *sText = NULL;
	SDL_Color textColor = {0,0,0,255};

	sText = TTF_RenderUTF8_Blended(font, text, textColor);

	if (sText == NULL )
		return 1;

	pos->w = sText->w;
	pos->h = sText->h;

	*texture = SDL_CreateTextureFromSurface(*renderer,sText);
	SDL_FreeSurface(sText);

	return 1;
}
int mapear(SDL_Point *point){
    int screen_maxw = 400;
    int screen_maxh = 240;
    int universe_maxw = 800;
    int universe_maxh = 480;

    point[0].x = (point[0].x * screen_maxw) / universe_maxw;
    point[0].y = (point[0].y * screen_maxh) / universe_maxh;

    point[1].x = (point[1].x * screen_maxw) / universe_maxw;
    point[1].y = (point[1].y * screen_maxh) / universe_maxh;

    point[2].x = (point[2].x * screen_maxw) / universe_maxw;
    point[2].y = (point[2].y * screen_maxh) / universe_maxh;

    return 1;
}
void removerCaractere(char *text)
{
	char *c = text;
	while (*c != '\0')
		++c;
	*(c - 1) = '\0';
}
