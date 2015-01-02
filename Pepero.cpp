#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window* gWindow = NULL;

SDL_Renderer* renderer = NULL;

TTF_Font *font = NULL;

class Texture
{
    public:
        Texture();

        ~Texture();

        bool loadFromFile(std::string path);

        bool loadFromText(std::string textureText, SDL_Color textColor);

        void free();

        void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

        int getWidth();

        int getHeight();

    private:
        SDL_Texture* texture;
        int width, height;
};

Texture::Texture()
{
    texture = NULL;
    width = 0;
    height = 0;
}

Texture::~Texture()
{
    free();
}

bool Texture::loadFromFile(std::string path)
{
    free();

    SDL_Texture* newTexture = NULL;

    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if(loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0, 0));

		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface );
		if(newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			//Get image dimensions
			width = loadedSurface->w;
			height = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	texture = newTexture;
	return texture != NULL;
}

bool Texture::loadFromText(std::string textureText, SDL_Color textColor)
{
    free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText.c_str(), textColor);
	if(textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		//Create texture from surface pixels
        texture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if(texture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			width = textSurface->w;
			height = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}

	//Return success
	return texture != NULL;
}

void Texture::free()
{
    if (texture != NULL)
    {
        SDL_DestroyTexture(texture);
        texture = NULL;
        width = 0;
        height = 0;
    }
}

void Texture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
    //Set rendering space and render to screen
	SDL_Rect renderQuad = {x, y, width, height };

	//Set clip rendering dimensions
	if(clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(renderer, texture, clip, &renderQuad, angle, center, flip);
}

int Texture::getWidth()
{
    return width;
}

int Texture::getHeight()
{
    return height;
}

bool init();

bool loadStart();

bool loadScore();

void close();

Texture start;
Texture score;

class Player
{
    public:

		static const int RADIUS = 10;

		Player()
		{
		}

		void handleEvent(SDL_Event &e);

		void render();

    private:

		int mPosX, mPosY;
};

void Player::handleEvent(SDL_Event &e)
{
    SDL_ShowCursor(0);
    if (e.type == SDL_MOUSEMOTION)
    {
        mPosX = e.motion.x;
        mPosY = e.motion.y;
    }
}

void Player::render()
{
	filledCircleRGBA(renderer, mPosX, mPosY, RADIUS, 255, 255, 255, 255);
}

class Start
{
    public:
        Start();

        ~Start();

        bool click(SDL_Event &e);

        void render();

        int getX();

        int getY();

    private:
        int x, y;
        int mouseX, mouseY;
};

Start::Start()
{
    x = (SCREEN_WIDTH - start.getWidth())/2;
    y = (SCREEN_HEIGHT - start.getHeight())/2;
    mouseX = 0;
    mouseY = 0;
}

Start::~Start()
{
}

bool Start::click(SDL_Event &e)
{
    bool clicked = false;
    if (e.type == SDL_MOUSEBUTTONUP)
    {
        if (e.button.button == SDL_BUTTON_LEFT)
        {
            mouseX = e.button.x;
            mouseY = e.button.y;

            if ((mouseX > getX()) && (mouseX < (getX() + start.getWidth())) && (mouseY > getY()) && (mouseY < (getY() + start.getHeight())))
            {
                start.free();
                clicked = true;
            }
        }
    }
    return clicked;
}

void Start::render()
{
    start.render(x,y);
}

int Start::getX()
{
    return x;
}

int Start::getY()
{
    return y;
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("Pepero", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if(gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			renderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if(renderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if(!(IMG_Init(imgFlags) &imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}

				//Initialize SDL_ttf
				if(TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}
			}
		}
	}
	return success;
}

bool loadStart()
{
	//Loading success flag
	bool success = true;

    if (!start.loadFromFile("start.bmp"))
    {
        printf( "Failed to load start texture!\n" );
		success = false;
    }

	return success;
}

bool loadScore()
{
    bool success = true;

    font = TTF_OpenFont("scoreboard.ttf", 32);
    if(font == NULL)
	{
		printf("Failed to load scoreboard font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	else
	{
		//Render text
		SDL_Color textColor = {255, 255, 255};
		if(!score.loadFromText("900", textColor))
		{
			printf("Failed to render text texture!\n");
			success = false;
		}
	}
	return success;
}

void close()
{
    score.free();
    TTF_CloseFont(font);
    font = NULL;
	//Destroy window
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	renderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* args[])
{
	//Start up SDL and create window
	if(!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Load start button
		if(!loadStart())
		{
			printf("Failed to load start!\n");
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

            Start s;

            Player player;
			//While application is running
			while(!quit)
			{
				//Handle events on queue
				while(SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if(e.type == SDL_QUIT)
					{
						quit = true;
					}

                    s.click(e);
				}

				//Clear screen
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
				SDL_RenderClear(renderer);

				//Render objects
                s.render();
                if (s.click(e) == true)
                {
                    player.render();
                }

				//Update screen
				SDL_RenderPresent(renderer);
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
