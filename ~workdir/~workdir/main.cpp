
//http://wiki.libsdl.org/APIByCategory

#include <iostream>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#undef main

int main() {

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	SDL_Window *win = SDL_CreateWindow("Run this", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
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

	bool running = true;

	while (running) {
		SDL_Event ev;
		while(SDL_PollEvent(&ev) != 0) {
			if(ev.type == SDL_QUIT) running = false;
		}
		SDL_RenderClear(ren);

		roundedBoxColor(ren, 40, 40, 240, 240, 10, 0xFABCA0FF);
		
		SDL_RenderPresent(ren);
		SDL_Delay(2000);
	}

	//SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
}
