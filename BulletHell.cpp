
/**www.opengameart.org source of spacePlayer**/
//Using SDL, SDL_image, standard IO, and strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include <time.h> 
#include <sstream>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Texture wrapper class
class LTexture
{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( std::string path );

		//Deallocates texture
		void free();

		//Renders texture at given point
		void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

		//Gets image dimensions
		int getWidth();
		int getHeight();

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;

		//Image dimensions
		int mWidth;
		int mHeight;
};

class LTimer
{
    public:
		//Initializes variables
		LTimer();

		//The various clock actions
		void start();
		void stop();
		void pause();
		void unpause();

		//Gets the timer's time
		Uint32 getTicks();

		//Checks the status of the timer
		bool isStarted();
		bool isPaused();

    private:
		//The clock time when the timer started
		Uint32 mStartTicks;

		//The ticks stored when the timer was paused
		Uint32 mPausedTicks;

		//The timer status
		bool mPaused;
		bool mStarted;
};

class Player
{
    public:
		//The dimensions of the Player
		static const int PLAYER_WIDTH = 20;
		static const int PLAYER_HEIGHT = 20;

		//Initializes the variables
		Player();

		//Takes key presses and adjusts the Player's velocity
		void handleEvent( SDL_Event* e );

		//Shows the Player on the screen
		void render();

    private:
		//The X and Y offsets of the Player
		int posX, posY;

		//mouse position
		int mouseX, mouseY;
};
class Enemy
{
    public:
        static const int ENEMY_WIDTH = 20;
		static const int ENEMY_HEIGHT = 20;

		//Initializes the variables
		Enemy();

		//Generate Enemy
		void generateEnemy();
		//Moves the enemy
		void move();
        //Shows the degreesg Enemy on the screen
		void render();

    private:
		int posX, posY, degrees, posXDisplacement, enemyCounter, direction;

};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Scene textures
LTexture gPlayerTexture;
LTexture gEnemyTexture;

//*************Start of LTexture definitions**************
LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile( std::string path )
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

void LTexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

//*************Start of Player definitions**************
Player::Player()
{
    //Initialize the offsets
    posX = 0;
    posY = 0;

    mouseX = 0;
    mouseY = 0;

}

void Player::handleEvent( SDL_Event* e )
{
    //hide sthe cursor
    SDL_ShowCursor(SDL_DISABLE);
    //If mouse is moving
    if(e->type == SDL_MOUSEMOTION )
    {
    //Position of the mouxe
        mouseX=e->motion.x;
        mouseY=e->motion.y;

    //Prevents the player from moving outside the screen 
    //If the Player went too far to the left or right
    if( ( mouseX < 0 ) || ( mouseX + PLAYER_WIDTH > SCREEN_WIDTH ) )
    {
        mouseX = 0;
        mouseX = SCREEN_WIDTH-PLAYER_WIDTH;
    }

    //If the Player went too far up or down
    if( ( mouseY < 0 ) || ( mouseY + PLAYER_HEIGHT > SCREEN_HEIGHT ) )
    {
        mouseY = 0;
        mouseY = SCREEN_HEIGHT-PLAYER_HEIGHT;
    }
    }

}

void Player::render()
{
    //Show the Player and renders the ball directly to the position of the mouse
	gPlayerTexture.render( mouseX, mouseY );
}

//*************Start of Enemy definitions**************
Enemy::Enemy()
{
	//initial position
	srand (time(NULL));
	posY = 0;
	degrees = 0;
	direction = rand() % 3 - 1;
	generateEnemy();
}

void Enemy::generateEnemy()
{
	enemyCounter = rand() % 5 + 4;
}

void Enemy::move()
{
	posY += 1;    
    if(posY > SCREEN_HEIGHT)
    {
    	posY = 0;
    	direction = rand() % 3 - 1;
    	generateEnemy();
    }
}

void Enemy::render()
{
	SDL_RendererFlip flipType = SDL_FLIP_NONE;	
	//Clear screen
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	for(int i = 0; i < enemyCounter; i++){
		degrees += direction * 1;
		gEnemyTexture.render( posXDisplacement, posY, NULL, degrees, NULL, flipType);
		posXDisplacement += 50;
	}	
	posXDisplacement = 0;
}

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
		gWindow = SDL_CreateWindow( "Bullet Hell", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
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

	//Load Player texture
	if( !gPlayerTexture.loadFromFile( "Images/player.png" ) )
	{
		printf( "Failed to load Player texture!\n" );
		success = false;
	}
    if( !gEnemyTexture.loadFromFile( "Images/enemy.png" ) )
	{
		printf( "Failed to load Enemy texture!\n" );
		success = false;
	}
	return success;
}

void close()
{
	//Free loaded images
	gPlayerTexture.free();
	gEnemyTexture.free();

	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

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

			//The Player that will be moving around on the screen
			Player player;
            Enemy enemy;

            //Degree of degrees
          	/*double degrees = 0; 

            int xDisplacement = 0;
			int enemyCounter = rand() % 5 + 1;
			int direction = rand() % 3 - 1;*/

			//Flip type
			//SDL_RendererFlip flipType = SDL_FLIP_NONE;

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User quits by closing window or pressing end
					if( e.type == SDL_QUIT || e.type == SDL_KEYDOWN )
					{
						quit = true;
						if(e.key.keysym.sym == SDLK_END)
                            quit = true;
					}
                    //Handle input for the Player
					player.handleEvent( &e );
				}

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

   				//degrees += direction * 1;
				/*for(int i = 0; i < enemyCounter; i++){
					gEnemyTexture.render( xDisplacement, ( SCREEN_HEIGHT - gEnemyTexture.getHeight() ) / 2, NULL, degrees, NULL, flipType);
					xDisplacement += 100;
				}
				xDisplacement = 0;*/

				//Render objects
				player.render();
				enemy.move();
                enemy.render();
				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();
	return 0;
}
/**Notes**/
/*
for collision detection possible algorithm is
mouseX and mouseY are the coordinates
if(mouseY+imageHeight/2||mouseY-imageHeight/2||mouseX+imageWitdth/2||mouseX-imagewidth/2)
{hit
*/
