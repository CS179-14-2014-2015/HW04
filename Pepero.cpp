#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>


const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_FPS = 30;
const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;
int countedFrames = 0;

class Player
{
    public:
		static const int pRadius = 10;

		Player();
		void handleEvent( SDL_Event& e );

		void render();

		~Player();

    private:

		int mPosX, mPosY;
		bool start = false;
		uint8_t pRed = 0xFF;

};

class Bullet{
    public:
  static const int bRadius = 25;

  Bullet();
  void move(int x, int y);
  void render();
  ~Bullet();

    private:
  int bPosX, bPosY;
  int bInitX, bInitY;
};

class BulletGroup{
    public:

  BulletGroup();
  void moveStraight();
  void moveCircular();
  void render();
  ~BulletGroup();

    private:
  std::vector<Bullet> bGroup;
};

bool init();

bool loadMedia();

void close();

SDL_Window* gWindow = nullptr;

SDL_Renderer* gRenderer = nullptr;

Player::Player()
{
    mPosX = SCREEN_WIDTH/2;
    mPosY = SCREEN_HEIGHT/2;
}


void Player::handleEvent( SDL_Event& e )
{
  if (!start){
   if (e.type = SDL_MOUSEBUTTONDOWN){
      if( e.button.button == SDL_BUTTON_RIGHT ){
        start = true;
      }
   }
  }
  if (start){
    SDL_ShowCursor(0);
    if (e.type == SDL_MOUSEMOTION)
        {
            mPosX = e.motion.x;
            mPosY = e.motion.y;
        }
  }
}

void Player::render()
{
	filledCircleRGBA(gRenderer, mPosX, mPosY, pRadius, pRed,0xFF,0xFF,0xFF);
}

Player::~Player(){

}

Bullet::Bullet(){
 bPosX = rand() % SCREEN_WIDTH;
 bPosY = rand() % SCREEN_HEIGHT;
}

void Bullet::render(){
 filledCircleRGBA(gRenderer, bPosX, bPosY, bRadius, 0xFF,0x0,0x0,0xFF);
}

void Bullet::move(int x, int y){
 bPosX += x;
 bPosY += y;
 if (bPosX > SCREEN_WIDTH || bPosY > SCREEN_HEIGHT){
   bPosX = rand() % SCREEN_WIDTH;
   bPosY = rand() % SCREEN_HEIGHT;
  }
}

Bullet::~Bullet(){}


BulletGroup::BulletGroup(){
 for(int i = 0; i < 10; i++)
  bGroup.push_back(Bullet());
}

void BulletGroup::moveStraight(){
 for(auto &x:bGroup){
   static int bVelX = rand() % 5 - 1;
   static int bVelY = rand() % 5 - 1;
   x.move(bVelX, bVelY);
  }
}

void BulletGroup::moveCircular(){
 for(auto &x:bGroup){
   static int bVelX = rand() % 3 - 1;
   static int bVelY = rand() % 3 - 1;
   x.move(bVelX, bVelY);
  }
}

void BulletGroup::render(){
 for(auto &x:bGroup)
  x.render();
}

BulletGroup::~BulletGroup(){}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load dot texture

	return success;
}

void close()
{
	//Free loaded images


	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = nullptr;
	gRenderer = nullptr;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

int main( int argc, char* args[] )
{
	//Start up SDL and create window

	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//Elements declaration
			Player player;
   BulletGroup lol, meh;
			//While application is running
			uint initTimer = SDL_GetTicks();
			while( !quit )
			{

    uint capTimer = SDL_GetTicks();
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}

					//Handle input for the dot
					player.handleEvent( e );
				}
    lol.moveStraight();
    meh.moveCircular();
				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0x0, 0x0, 0x0, 0xFF );
				SDL_RenderClear( gRenderer );

				//Render objects
				player.render();
				lol.render();
				meh.render();
				//Update screen
				SDL_RenderPresent( gRenderer );
					++countedFrames;

				//If frame finished early
    if( capTimer < SCREEN_TICK_PER_FRAME )
				{
					//Wait remaining time
					SDL_Delay( SCREEN_TICK_PER_FRAME - capTimer );
				}
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
