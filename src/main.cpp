
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
bool hit = false;
int time = 0;
int ptime = 0;

SDL_Window *win = NULL;
SDL_Renderer *ren = NULL;

using namespace std;

struct particle{
	int x, y;
	int vx, vy;
	int rad;
	Uint32 color;
	char t;
	int dt;
	particle(){};
	particle(char tt, int tx, int ty, int tvx, int tvy, int trad, Uint32 tcolor): t(tt), x(tx), y(ty), vx(tvx), vy(tvy), rad(trad), color(tcolor) {}

};

//create player here
particle p1;
vector<particle> px;
vector<vector<particle>> sLine;

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


void generatePrime(){
		ptime = time;
}


void generateHBeam(int rad){
	px.push_back(*new particle('a', 0, 0, 2, 0, rad, 0xFF000000));
	px.back().dt = ptime;
	px.push_back(*new particle('a', 0, HEIGHT/4, 2, 0, rad, 0xFF000000));
	px.back().dt = ptime;
	px.push_back(*new particle('a', 0, HEIGHT*1/2, 2, 0, rad, 0xFF000000));
	px.back().dt = ptime;
	px.push_back(*new particle('a', 0, HEIGHT*3/4, 2, 0, rad, 0xFF000000));
	px.back().dt = ptime;
	px.push_back(*new particle('a', 0, HEIGHT, 2, 0, rad, 0xFF000000));
	px.back().dt = ptime;
}

void generateSBeam(int rad){
	px.push_back(*new particle('b', 0, 0, 0, 2, rad, 0xFF000000));
	px.back().dt = ptime;
	px.push_back(*new particle('b', WIDTH/4, 0, 0, 2, rad, 0xFF000000));
	px.back().dt = ptime;
	px.push_back(*new particle('b', WIDTH*1/2, 0, 0, 2, rad, 0xFF000000));
	px.back().dt = ptime;
	px.push_back(*new particle('b', WIDTH*3/4, 0, 0, 2, rad, 0xFF000000));
	px.back().dt = ptime;
	px.push_back(*new particle('b', WIDTH, 0, 0, 2, rad, 0xFF000000));
	px.back().dt = ptime;
}


void generateXBeam(int rad){
	px.push_back(*new particle('x',0, 0, 2, 2, rad, 0xFF000000));
	px.back().dt = ptime;
	px.push_back(*new particle('y',WIDTH,0, -2, 2, rad, 0xFF000000));
	px.back().dt = ptime;
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
	//cout << "Size Now is" << px.size() << " \n";
	if(gen == '0'){
		if(time < 10){
			if(time % 10 == 0){
				generatePrime();
			}
		}else if(time < 1100){
			if(time % 10 == 0){
				generateXBeam(5);
			}
		}
		else if(time < 1200){}
		else if(time < 1210){
			if(time % 10 == 0){
				generatePrime();
			}
		}else if(time < 2300){
			if(time % 10 == 0){
				generateHBeam(5);
			}
		}
		else if( time < 2400){}
		else if( time < 2410) {
			if(time % 10 == 0){
				generatePrime();
			}
		}
		else if( time < 3500){
			if ( time % 10 == 0){
				generateSBeam(5);
			}
		}else if( time < 3600){}
		else{
			running = false;
		}
	}
	
	for(int i = 0; i < px.size(); i++){
		if (px[i].t == 'a'){
			px[i].vy = sin(time/20.0*1.0)*2.0+sin(px[i].dt++/20.0*M_PI)*5.0;
		}else if(px[i].t == 'b'){
			px[i].vx = sin(time/20.0*1.0)+sin(px[i].dt++/20.0*M_PI)*5.0;
		}
		else if(px[i].t == 'x'){
			px[i].vx += sin(time/20.0*1.0)*rand()/RAND_MAX+sin(px[i].dt++/20.0*M_PI)*1.0;
			//px[i].vy *= sin(time/20.0*M_PI)*5.0;
		}
		else if(px[i].t == 'y'){
			px[i].vx -= sin(time/20.0*1.0)*rand()/RAND_MAX+sin(px[i].dt++/20.0*M_PI)*1.0;
			//px[i].vy *= sin(px[i].dt++/20.0*M_PI)*5.0;
		}
	}

	for(int i = 0; i < px.size(); i++){//collision detection
		px[i].x += px[i].vx;
		px[i].y += px[i].vy;
		if(collide(p1,px[i])){
			score++;
			cout << "Times Hit: " << score << endl; 
			hit = true;
			px.erase(px.begin()+i);
		}else if(px[i].y > HEIGHT + px[i].rad){
			px.erase(px.begin()+i);
		}
		/*else if(px[i].x < -px[i].rad){
			px.erase(px.begin()+i);
		}*/
		else if(px[i].x > WIDTH + px[i].rad && (px[i].t == 'a' )){
			px.erase(px.begin()+i);
		}
	}

	
}

void render(){
	if(hit) SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
	else SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
	SDL_RenderClear(ren);
	if(hit) filledCircleColor(ren,p1.x,p1.y,p1.rad, 0xFFFFFFFF);
	else filledCircleColor(ren,p1.x,p1.y,p1.rad,p1.color);
	for(int i = 0; i < px.size(); i++){
		//cout << i << " particle @ " << px[i].x << " " << px[i].y << endl;
		if(hit) filledCircleColor(ren, px[i].x, px[i].y, px[i].rad, 0xFFFFFFFF);
		else filledCircleColor(ren, px[i].x, px[i].y, px[i].rad, px[i].color);
	}
	if(hit) hit = false;
	SDL_RenderPresent(ren);
	// #raffehpls
	//cout << "SCORE: " << score << endl;
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
		input(ev);
		//cout << "input lag: " << SDL_GetTicks()<< endl;
		physics();
		//cout << "phy lag: " << SDL_GetTicks()<< endl;
		render();
		//cout << "render lag: " << SDL_GetTicks()<< endl;
		
		if(1000/FPS > (SDL_GetTicks() - start)){
			SDL_Delay(1000/FPS - (SDL_GetTicks() - start));
		}
		
		time++;
	}
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
}
