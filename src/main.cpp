
#include <iostream>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <sstream>
//#include <cmath>
#include <vector>

const int WIDTH = 480;
const int HEIGHT = 640;
const int FPS = 60;

bool running = false;
char gen = '0';
int score = 0;
int time = 0;

SDL_Window *win = NULL;
SDL_Renderer *ren = NULL;

using namespace std;

struct particle{
	int x, y;
	int vx, vy;
	int rad;
	Uint32 color;
	char t;
	particle(){};
	particle(char tt, int tx, int ty, int tvx, int tvy, int trad, Uint32 tcolor): t(tt), x(tx), y(ty), vx(tvx), vy(tvy), rad(trad), color(tcolor) {}
};

/*
struct player{
	int x, y;
	int rad;
	Uint32 color;
};
*/
//create player here
particle p1;
vector<particle> px;

bool collide(particle& bull, particle& play){
	double radSum = bull.rad + play.rad + 0.0;
	double xVal = (bull.x - play.x) * (bull.x - play.x) * 1.0;
	double yVal = (bull.y - play.y) * (bull.y - play.y) * 1.0;
	double dist = sqrt(xVal + yVal);
	if(dist > radSum)
		return false;
	else
		return true;
}
//stage generation #raffehpls
void generateLBeam(int rad){
	px.push_back(*new particle('s', WIDTH/4, 0, 0, 1+2*rand()/RAND_MAX, rad, 0xFF000000));
	px.push_back(*new particle('s', WIDTH*3/4, 0, 0, 1+2*rand()/RAND_MAX, rad, 0xFF000000));
}
//stage generation #raffehpls
void generateHBeam(int rad){
	px.push_back(*new particle('0', 0, HEIGHT/4, 4, 2, rad, 0xFF000000));
	px.push_back(*new particle('s', WIDTH, HEIGHT*3/4, -4, (sin(time*1.0)+1)*5, rad, 0xFF000000));
}

bool init(){
	bool run = true;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		run = false;
	}
	win = SDL_CreateWindow("Bullets all over", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	if (win == nullptr){
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		run = false;
	}
	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == nullptr){
		SDL_DestroyWindow(win);
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		run = false;
	}
	SDL_ShowCursor(SDL_DISABLE);

	return run;
}

void setup(){
	p1.color =  0xFFACABAA;
	p1.x = WIDTH/2;
	p1.y = HEIGHT/2;
	p1.rad = 5;
}

void input(SDL_Event& ev){
	SDL_GetMouseState(&p1.x,&p1.y);
	//std::cout << p1.x << " " << p1.y << " \n";
	while(SDL_PollEvent(&ev) != 0) {
		if(ev.type == SDL_QUIT) running = false;
		else if(ev.type == SDL_KEYDOWN){
			switch(ev.key.keysym.sym){
				case SDLK_1:
				gen = 'b';
				break;
				case SDLK_2:
				gen = '0';
				break;
			}
		}
	}
}

void physics(){
	if(gen == '0'){

		//stage generation #raffehpls
		if(time < 200){
			if(time % 60 == 0){
				generateLBeam(5);
				//gen = '0';
			}
		}else if(time < 1000){
			if(time % 30 == 0){
				generateHBeam(5);
				//gen = '0';
			}
		}
		
	}

	//sine wave ish #raffehpls
	for(int i = 0; i < px.size(); i++){
		if(px[i].t == 's'){
			px[i].vx = sin(time/20.0*M_PI)*5.0;
			//cout << sin(time/120.0*M_PI) <<" HEYY " << px[i].vx << endl;
		}		
	}

	for(int i = 0; i < px.size(); i++){
		px[i].x += px[i].vx;
		px[i].y += px[i].vy;

		if(collide(p1,px[i])){
			score++;
			px.erase(px.begin()+i);
		}else if(px[i].y > HEIGHT + px[i].rad){
			px.erase(px.begin()+i);
		}else if(px[i].x < -px[i].rad){
			px.erase(px.begin()+i);
		}else if(px[i].x > WIDTH + px[i].rad){
			px.erase(px.begin()+i);
		}
	}

	
}

void render(){
	SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
	SDL_RenderClear(ren);
	filledCircleColor(ren,p1.x,p1.y,p1.rad,p1.color);
	for(int i = 0; i < px.size(); i++){
		//cout << i << " particle @ " << px[i].x << " " << px[i].y << endl;
		filledCircleColor(ren, px[i].x, px[i].y, px[i].rad, px[i].color);
	}
	SDL_RenderPresent(ren);
	// #raffehpls
	cout << "SCORE: " << score << endl;
}

int main( int argc, char* args[] ){
	if(!init()) return 1;

	running = true;
	Uint32 start;
	SDL_Event ev;

	setup();

	while (running) {
		// time here
		start = SDL_GetTicks();
		//cout << start << endl;
		input(ev);
		physics();
		render();

		//time here
		if(1000/FPS > (SDL_GetTicks() - start)){
			SDL_Delay(1000/FPS - (SDL_GetTicks() - start));
		}
		time++;
	}
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
}
