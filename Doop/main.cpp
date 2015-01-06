#include <iostream>
#include <SDL.h>
#include <gfx/SDL2_gfxPrimitives.h>
#include <vector>
#include <cmath>
//http://wiki.libsdl.org/APIByCategory

using namespace std;

//If damaged by asteroids, there is an invuln period of 2 seconds

//NOT SUPPOSED TO CHANGE, GAME GOES BONKERS
const int FRAMERATE = 60;
const double FRAME_TIME = 1000/FRAMERATE;
const float PI = 3.14159265;
const int TIME_IN_SECONDS = 60;

double dt = 0.1;
int windowWidth = 720;
int windowHeight = 720;
int stormIntensity = 1;
int bulletSpeed = 5;
int bulletSize = 100;
int shipSize = 50;
int maxBullets = 5*stormIntensity;


//it will take windowHeight/bulletSpeed frames for the bullet to traverse the screen
//in a second, a bullet travels bulletspeed*FRAMERATE pixels
//we need the last bullet for each wave to be bulletspeed*FRAMERATE*7 pixels above the top border of the screen

struct bullet {
	int ox,x,y;
};

vector<bullet> bullets (maxBullets);

float fRand()
{
	return (float)rand()/RAND_MAX;
}

void generateBulletStorm()
{
	int lastBulletTravelDist = bulletSpeed*FRAMERATE*7;
	for (int i = 0; i < maxBullets; i++)
	{
		//cout << fRand() << " ";
		bullets[i].ox = fRand() * (windowWidth-bulletSize);
		bullets[i].y = -i*(lastBulletTravelDist/maxBullets) - (bulletSpeed*FRAMERATE/2);
		//cout << bullets[i].x << " " << bullets[i].y << endl;
	}
}

void advanceBullets(float t)
{
	for (int i = 0; i < maxBullets; i+=2)
	{
		bullets[i].x = bullets[i].ox + (30*cos(t));
		bullets[i].y += bulletSpeed;
	}
	for (int i = 1; i < maxBullets; i+=2)
	{
		bullets[i].x = bullets[i].ox - (30*cos(t));
		bullets[i].y += bulletSpeed;
	}
}

void drawBullets(SDL_Renderer *ren, SDL_Texture *tex)
{
	for (int i = 0; i < maxBullets; i+=2)
	{
		SDL_Rect bulletSpace = {bullets[i].x, bullets[i].y, bulletSize, bulletSize}; //positioning of thingy
		SDL_RenderCopy(ren, tex, NULL, &bulletSpace); //texturization of thingy
	}
	for (int i = 1; i < maxBullets; i+=2)
	{
		SDL_Rect bulletSpace = {bullets[i].x, bullets[i].y, bulletSize, bulletSize}; //positioning of thingy
		SDL_RenderCopy(ren, tex, NULL, &bulletSpace); //texturization of thingy
	}
}

//for circles
bool checkForCollision(SDL_Rect ship)
{
	for (int i = 0; i < maxBullets; i++)
	{
		if (	sqrtf((((ship.x+(shipSize/2))-(bullets[i].x+(bulletSize/2)))*((ship.x+(shipSize/2))-(bullets[i].x+(bulletSize/2)))) +
				     (((ship.y+(shipSize/2))-(bullets[i].y+(bulletSize/2)))*((ship.y+(shipSize/2))-(bullets[i].y+(bulletSize/2))))) 
				< sqrtf((bulletSize/2+shipSize/2)*(bulletSize/2+shipSize/2)) )
		{
			return true;
		}
	}
	return false;
}

//for rectangles
bool checkCollision (SDL_Rect playerCursor, std::vector<SDL_Rect> bulletWave)
{
	bool isHit = false;
	
	for(int i = 0; i < bulletWave.size(); i++)
	{
		//check intersection along x axis
		if(playerCursor.x+playerCursor.w > bulletWave[i].x && playerCursor.x < bulletWave[i].x + bulletWave[i].w)
		{
			//check intersection along y axis
			if(playerCursor.y+playerCursor.h > bulletWave[i].y && playerCursor.y < bulletWave[i].y + bulletWave[i].h)
			{
				isHit = true;
			}
		}
	}
	return isHit;
}

int main(int argc, char* argv[]) {

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	SDL_Window *win = SDL_CreateWindow("Asteroid Hell", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
	if (win == nullptr){
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == nullptr){
		SDL_DestroyWindow(win);
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	/*SDL_Texture *tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_UNKNOWN, SDL_TEXTUREACCESS_STATIC, 720, 720);
	if (tex == nullptr){
		std::cout << "SDL_CreateTexture Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}*/


	//TEXTURE CREATION HOLLAAAA
	SDL_Surface *ship = SDL_LoadBMP("Ship.bmp");
	SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, ship);
	SDL_FreeSurface(ship);

	SDL_Surface *damShip = SDL_LoadBMP("damShip.bmp");
	SDL_Texture *damTex = SDL_CreateTextureFromSurface(ren, damShip);
	SDL_FreeSurface(damShip);

	SDL_Surface *rocks = SDL_LoadBMP("ROCKS.bmp");
	SDL_Texture *ast = SDL_CreateTextureFromSurface(ren, rocks);
	SDL_FreeSurface(rocks);

	SDL_Surface *winScreen = SDL_LoadBMP("winscreen.bmp");
	SDL_Texture *winScreenTex = SDL_CreateTextureFromSurface(ren, winScreen);
	SDL_FreeSurface(winScreen);

	bool running = true;

	int mx = 0, my = 0;
	int frameNo = 0;

	//Uint32 color1 = 0xFF777744, color2 = 0xFF334455;
	
	SDL_ShowCursor(0);
	
	double t = 0;
	int score = 0;
	bool damaged = false;
	int invuln = FRAMERATE * 2;

	generateBulletStorm();
	cout << "Wave: " << stormIntensity << endl;

	while (running) {
		int frameTimeStart = SDL_GetTicks();
		t += dt;
		SDL_Event ev;
		while(SDL_PollEvent(&ev) != 0) {
			if(ev.type == SDL_QUIT) running = false;
		}
		SDL_RenderClear(ren);

		//READ INPUT
		SDL_GetMouseState(&mx, &my);

		//SIMULATE		
		frameNo++;
		if (frameNo % (FRAMERATE*10) == 0)
		{
			stormIntensity++;
			maxBullets = 5*stormIntensity;
			bullets.resize(maxBullets);
			//cout << "maxBullets: " << maxBullets << endl;
			generateBulletStorm();
			if(stormIntensity <= 6)
			{
				cout << "Wave: " << stormIntensity << endl;
			}
		}
		advanceBullets(t);
		

		//DRAW
		if (frameNo > FRAMERATE*TIME_IN_SECONDS)
		{
			cout << endl << "FINAL SCORE: " << score;
			SDL_RenderCopy(ren, winScreenTex, NULL, NULL);
			SDL_Rect cursor = {mx-(shipSize/2),my-(shipSize/2),shipSize,shipSize}; //positioning of thingy
			SDL_RenderCopy(ren, tex, NULL, &cursor); //texturization of thingy
		}
		else
		{
			drawBullets(ren, ast);

			if (damaged == true && invuln != 0)
			{
			
				SDL_Rect cursor = {mx-(shipSize/2),my-(shipSize/2),shipSize,shipSize}; //positioning of thingy
				SDL_RenderCopy(ren, damTex, NULL, &cursor);
				invuln--;
				if (invuln == 0)
				{
					invuln = FRAMERATE;
					damaged = false;
				}
			}
			else
			{
				SDL_Rect cursor = {mx-(shipSize/2),my-(shipSize/2),shipSize,shipSize}; //positioning of thingy
				SDL_RenderCopy(ren, tex, NULL, &cursor); //texturization of thingy

				for (int i = 0; i < maxBullets; i++)
				{
					if ( checkForCollision(cursor) && damaged == false)//SDL_HasIntersection(&cursor, &bullets[i]) && damaged == false)
					{
						score++;
						cout << "Score: " << score << endl;
						damaged = true;
					}
				}
			}
		}

		SDL_RenderPresent(ren);
		
		//DELAY
		if ((FRAME_TIME - (SDL_GetTicks() - frameTimeStart)) > 0)
		{
			SDL_Delay(FRAME_TIME - (SDL_GetTicks() - frameTimeStart));
		}
	}

	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
}
