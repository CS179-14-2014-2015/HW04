/*This source code copyrighted by Lazy Foo' Productions (2004-2014)
and may not be redistributed without written permission.*/

//things I removed functions: setBlendMode, setAlpha, setColor
//the timer class

/**www.opengameart.org source of spaceship**/
//Using SDL, SDL_image, standard IO, and strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <cmath>

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

//The ship that will move around on the screen
class ship
{
    public:
		//The dimensions of the ship
		static const int ship_WIDTH = 20;
		static const int ship_HEIGHT = 20;

		//Initializes the variables
		ship();

		//Takes key presses and adjusts the ship's velocity
		void handleEvent( SDL_Event* e );

		//Shows the ship on the screen
		void render();

    private:
		//The X and Y offsets of the ship
		int mPosX, mPosY;

		//mouse position
		int mouseX, mouseY;
};
class comet
{
    public:
        static const int COMET_WIDTH = 20;
		static const int COMET_HEIGHT = 20;

		//Initializes the variables
		comet();

        //Shows the comet on the screen
		void render();

    private:
		//comet position
		int cometX, cometY;
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
LTexture gshipTexture;
LTexture cometTexture;

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


ship::ship()
{
    //Initialize the offsets
    mPosX = 0;
    mPosY = 0;

    mouseX = 0;
    mouseY = 0;

}

comet::comet()
{
	//initial position
	cometX = 0;
	cometY = 0;
}

void ship::handleEvent( SDL_Event* e )
{
    //hide sthe cursor
    SDL_ShowCursor(SDL_DISABLE);
    //If mouse is moving
    if(e->type == SDL_MOUSEMOTION )
    {
     /*****gives the position of the mouse*****/
        mouseX=e->motion.x;
        mouseY=e->motion.y;

     /*****prevents the ball from going out of the screen*****/
    //If the ship went too far to the left or right
    if( ( mouseX < 0 ) || ( mouseX + ship_WIDTH > SCREEN_WIDTH ) )
    {
        mouseX = 0;
        mouseX = SCREEN_WIDTH-ship_WIDTH;
    }

    //If the ship went too far up or down
    if( ( mouseY < 0 ) || ( mouseY + ship_HEIGHT > SCREEN_HEIGHT ) )
    {
        mouseY = 0;
        mouseY = SCREEN_HEIGHT-ship_HEIGHT;
    }
    }

}


void ship::render()
{
    //Show the ship and renders the ball directly to the position of the mouse
	gshipTexture.render( mouseX, mouseY );
}

void comet::render()
{
    //renders the comet to the screen
	cometX += 1;
	cometY += 1;
	if( ( cometX < 0 ) || ( cometX + COMET_WIDTH > SCREEN_WIDTH ) )
    {
        cometX += 10;
        cometX -= 90;
    }

    //If the ship went too far up or down
    if( ( cometY < 0 ) || ( cometY + COMET_HEIGHT > SCREEN_HEIGHT ) )
    {
        cometY += 11;
        cometY -= 90;
    }

	cometTexture.render(cometX, cometY );
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

	//Load ship texture
	if( !gshipTexture.loadFromFile( "Images/index.bmp" ) )
	{
		printf( "Failed to load ship texture!\n" );
		success = false;
	}
    if( !cometTexture.loadFromFile( "Images/dot.bmp" ) )
	{
		printf( "Failed to load ship texture!\n" );
		success = false;
	}
	return success;
}

void close()
{
	//Free loaded images
	gshipTexture.free();

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

			//The ship that will be moving around on the screen
			ship ship;
            comet comet;
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
                    //Handle input for the ship
					ship.handleEvent( &e );
				}

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				//Render objects
				ship.render();
                comet.render();
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
