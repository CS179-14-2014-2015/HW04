//g++ main.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -o main

//Using SDL, SDL_image, SDL_ttf, standard IO, strings, and string streams
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <cmath>

#define PI 3.14159265

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Texture wrapper class
class LTexture{
	public:
		//Initialize variables
		LTexture();
		
		//Deallocates memory
		~LTexture();
		
		//Loads image at specified path
		bool loadFromFile(std::string path);
		
		#ifdef _SDL_TTF_H
		//Creates image from font string
		bool loadFromRenderedText(std::string textureText, SDL_Color textColor);
		#endif
		
		//Deallocates texture
		void free();
		
		//Set color modulation
		void setColor(Uint8 red, Uint8 green, Uint8 blue);
		
		//Set blending
		void setBlendMode(SDL_BlendMode blending);
		
		//Set alpha modulation
		void setAlpha(Uint8 alpha);
		
		//Renders texture at given point
		void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
		
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

class LTimer{
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

//The bullet that will move around on the screen
class Bullet{
	public:
		//The dimensions of the bullet
		static const int BULLET_WIDTH = 20;
		static const int BULLET_HEIGHT = 20;
		
		//Maximum axis velocity of the bullet
		static const int BULLET_VEL = 10;
		
		//Initializes the variables
		Bullet(int sPosX, int sPosY, double sVelX, double sVelY);
		
		//Moves the bullet
		bool move();
		
		//Shows the bullet on the screen
		void render();
	
	private:
		//The X and Y offsets of the bullet
		int mPosX, mPosY;
		
		//The velocity of the bullet
		double mVelX, mVelY;
		
		//Pointer to bullet texture image
		LTexture* mBulletTexture;
};

class Enemy{
	public:
		//The dimensions of the enemy
		static const int ENEMY_WIDTH = 20;
		static const int ENEMY_HEIGHT = 20;
		
		//Maximum axis velocity of the bullet
		static const int ENEMY_VEL = 10;
		
		//Maximum angular velocity of the enemy
		static const double ENEMY_ROT = 30;
		
		//Initializes the variables
		Enemy();
		
		//Shoot the friggin' bullets
		void shoot();
		
		//Move enemy
		void move(SDL_Event& e);
		
		//Show the enemy on the screen
		void render();
	
	private:
		//The X and Y offsets of the enemy
		int mPosX, mPosY;
		
		//The velocity of the enemy
		int mVelX, mVelY;
		
		//The angle of the enemy
		double mAngle;
		
		//The rotation of the enemy
		double mRotation;
		
		//The number of bullets fired
		double mBullets;
		
		//The angle between bullets
		double mBulletAngle;
		
		//Origin of the shots
		SDL_Point mCenter;
		
		//Enemy texture
		LTexture* mEnemyTexture;
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
LTexture gDotTexture;

//Vectors for bullets and enemies
std::vector<Bullet> gBullets;
std::vector<Enemy> gEnemies;

int main(int argc, char *args[]){	
	//Start up SDL and create window
	if(!init()){
		printf("Failed to initialize!\n");
	}else{
		//Load media
		if(!loadMedia()){
			printf("Failed to load media!\n");
		}else{
			//Main loop flag
			bool quit = false;
			
			int frames = 0;
			int frequency = 5;
			
			//Event handler
			SDL_Event e;
			
			//Enemy
			Enemy x;
			gEnemies.push_back(x);
			
			//While application is running
			while(!quit){
				//Handle events on queue
				while(SDL_PollEvent(&e) != 0){
					//User requests quit
					if(e.type == SDL_QUIT){
						quit = true;
					}
					
					for(int i = 0; i < gEnemies.size(); ++i){
						//Handle events
						gEnemies[i].move(e);
					}
				}
				
				//Clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);
				
				for(int i = 0; i < gEnemies.size(); ++i){
					if(frames%frequency == 0){
						//Shoot bullet;
						gEnemies[i].shoot();
					}
					//Render enemies
					gEnemies[i].render();
				}
				
				for(int i = 0; i < gBullets.size(); ++i){
					//Move the bullet
					if(!gBullets[i].move()){
						gBullets.erase(gBullets.begin()+i);
					}else{
						//Render bullets
						gBullets[i].render();
					}
				}
				
				//Update screen
				SDL_RenderPresent(gRenderer);
				
				++frames;
			}
		}
	}
	
	//Free resources and close SDL
	close();
	
	return 0;
}

LTexture::LTexture(){
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture(){
	//Deallocate
	free();
}

bool LTexture::loadFromFile(std::string path){
	//Get rid of preexisting texture
	free();
	
	//The final texture
	SDL_Texture* newTexture = NULL;
	
	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	
	if(loadedSurface == NULL){
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}else{
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0x00, 0xFF, 0xFF));
		
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		
		if(newTexture == NULL){
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}else{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}
		
		//Get rid of loaded surface
		SDL_FreeSurface(loadedSurface);
	}
	
	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

#ifdef _SDL_TTF_H
bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor){
	//Get rid of preexisting texture
	free();
	
	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
	if(textSurface == NULL){
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}else{
		//Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if(mTexture == NULL){
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}else{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}
		
		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}
	
	//Return success
	return mTexture != NULL;
}
#endif

void LTexture::free(){
	//Free texture if it exists
	if(mTexture != NULL){
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue){
	//Modulate texture
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending){
	//Set blending function
	SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha){
	//Modulate texture alpha
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip){
	//Set rendering space and render to screen
	SDL_Rect renderQuad = {x, y, mWidth, mHeight};
	
	//Set clip rendering dimensions
	if(clip != NULL){
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	
	//Render to screen
	SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}	

int LTexture::getWidth(){
	return mWidth;
}

int LTexture::getHeight(){
	return mHeight;
}

LTimer::LTimer(){
	//Initialize the variables
	mStartTicks = 0;
	mPausedTicks = 0;
	
	mPaused = false;
	mStarted = false;
}

void LTimer::start(){
	//Start the timer
	mStarted = true;
	
	//Unpause the timer
	mPaused = false;
	
	//Get the current clock time
	mStartTicks = SDL_GetTicks();
	mPausedTicks = 0;
}

void LTimer::stop(){
	//Stop the timer
	mStarted = false;
	
	//Unpause the timer
	mPaused = false;
	
	//Clear tick variables
	mStartTicks = 0;
	mPausedTicks = 0;
}

void LTimer::pause(){
	//If the timer is running and isn't already paused
	if(mStarted && !mPaused){
		//Pause the timer
		mPaused = true;
		
		//Calculate the paused ticks
		mPausedTicks = SDL_GetTicks()-mStartTicks;
		mStartTicks = 0;
	}
}

void LTimer::unpause(){
	//If the timer is running and paused
	if(mStarted && mPaused){
		//Unpause the timer
		mPaused = false;
		
		//Reset the startng ticks
		mStartTicks = SDL_GetTicks()-mPausedTicks;
		
		//Reset the paused ticks
		mPausedTicks = 0;
	}
}

Uint32 LTimer::getTicks(){
	//The actual timer time
	Uint32 time = 0;
	
	//If the timer is running
	if(mStarted){
		//If the timer is paused
		if(mPaused){
			//Return the number of ticks when the timer is paused
			time = mPausedTicks;
		}else{
			//Return the current time minus the start time
			time = SDL_GetTicks() - mStartTicks;
		}
	}
	
	return time;
}

bool LTimer::isStarted(){
	//Timer is running and paused or unpaused
	return mStarted;
}

bool LTimer::isPaused(){
	//Timer is running and paused
	return mPaused && mStarted;
}

Bullet::Bullet(int sPosX, int sPosY, double sVelX, double sVelY){
	//Initializes the offsets
	mPosX = sPosX;
	mPosY = sPosY;
	
	//Initializes the velocity
	mVelX = sVelX*BULLET_VEL;
	mVelY = sVelY*BULLET_VEL;
	
	//Initializes the bullet pointer
	mBulletTexture = &gDotTexture;
}

bool Bullet::move(){
	//Move the bullet left or right
	mPosX += mVelX;
	
	//Move the bullet up or down
	mPosY += mVelY;
	
	//If the bullet went too far to the left, right, up, or down
	if((mPosY+BULLET_HEIGHT < 0) || (mPosY > SCREEN_HEIGHT) || (mPosX+BULLET_WIDTH < 0) || (mPosX > SCREEN_WIDTH)){
		//Return false if bullet is outside screen
		return false;
	}
	
	//Return true if bullet is inside screen
	return true;
}

void Bullet::render(){
	//Show the bullet
	mBulletTexture->render(mPosX, mPosY);
}

Enemy::Enemy(){
	//Initialize the position of the enemy
	mPosX = (SCREEN_WIDTH-ENEMY_WIDTH)/2;
	mPosY = (SCREEN_HEIGHT-ENEMY_HEIGHT)/2;
	
	mVelX = 0;
	mVelY = 0;
	
	//Initialize the angle of the enemy
	mAngle = PI/2;
	
	//Initialize the rotation of the enemy
	mRotation = 0;
	
	//Initialize the number of bullets fired at a time
	mBullets = 9;
	
	//Initialize bullet angle
	mBulletAngle = 2*PI/mBullets;
	
	//Initialize enemy texture
	mEnemyTexture = &gDotTexture;
	
	//Initialize the center
	mCenter.x = mPosX+ENEMY_WIDTH/2;
	mCenter.y = mPosY+ENEMY_HEIGHT/2;
}

void Enemy::move(SDL_Event& e){
	//If a key was pressed
	if(e.type == SDL_KEYDOWN && e.key.repeat == 0){
		switch(e.key.keysym.sym){
			//Adjust the velocity
			case SDLK_UP: mVelY -= ENEMY_VEL; break;
			case SDLK_DOWN: mVelY += ENEMY_VEL; break;
			case SDLK_LEFT: mVelX -= ENEMY_VEL; break;
			case SDLK_RIGHT: mVelX += ENEMY_VEL; break;
			//Press space to rotate
			case SDLK_SPACE: mRotation += ENEMY_ROT; break;
		}
	}
	//If a key was released
	else if(e.type == SDL_KEYUP && e.key.repeat == 0){
		//Reset values
		switch(e.key.keysym.sym){
			case SDLK_UP: mVelY += ENEMY_VEL; break;
			case SDLK_DOWN: mVelY -= ENEMY_VEL; break;
			case SDLK_LEFT: mVelX += ENEMY_VEL; break;
			case SDLK_RIGHT: mVelX -= ENEMY_VEL; break;
			case SDLK_SPACE: mRotation -= ENEMY_ROT; break;
		}
	}
}

void Enemy::shoot(){
	//Add bullet/s into "barrel" i.e. vector of bullets to be shot
	for(double i = mAngle; i < mAngle+2*PI; i += mBulletAngle){
		Bullet bullet(mPosX, mPosY, cos(i), sin(i));
		gBullets.push_back(bullet);
	}
}

void Enemy::render(){
	mPosX += mVelX;
	mPosY += mVelY;
	mAngle += mRotation;
	
	//Show the bullet
	mEnemyTexture->render(mPosX, mPosY);
}

bool init(){
	//Initialization flag
	bool success = true;
	
	//Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}else{
		//Set texture filtering to linear
		if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")){
			printf("Warning: Linear texture filtering not enabled!");
		}
		
		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if(gWindow == NULL){
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}else{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			
			if(gRenderer == NULL){
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}else{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				
				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if(!(IMG_Init(imgFlags) & imgFlags)){
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}
	
	return success;
}

bool loadMedia(){
	//Loading success flag
	bool success = true;
	
	//Load bullet texture
	if(!gDotTexture.loadFromFile("dot.bmp")){
		printf("Failed to load bullet texture!\n");
		success = false;
	}
	
	return success;
}

void close(){
	//Free loaded images
	gDotTexture.free();
	
	//Destroy window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;
	
	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}