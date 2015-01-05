/*
Compiled via command line using:
	g++ BulletHell.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -o BulletHell
	
Future improvements:
	Replace bullet frequency (int) and followPath (function) with index for two vectors (shot pattern and followPath)
		Add shot pattern functions and additional followPath functions
		Shot pattern functions take the ff parameters: xPos, yPos or xVel, yVel
		Shot pattern functions will change the bullets velocity depending to the length of time it's existed

Resources:
	Audio:
		http://downloads.khinsider.com/game-soundtracks/album/mortal-kombat-trilogy-original-game-audio/15-wasteland.mp3
	Background image:
		http://www.xnaresources.com/default.asp?page=Tutorial:StarDefense:3
*/

//Using SDL, SDL_image, SDL_ttf, SDL_mixer, standard IO, strings, and string streams


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>

#define PI 3.14159265

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
int SCOREBOARD_WIDTH = 100;
int PLAYFIELD_WIDTH = SCREEN_WIDTH-SCOREBOARD_WIDTH;

//A circle structure
struct Circle{
	int x, y;
	int r;
};

//Texture wrapper class
class LTexture{
	public:
		//Initialize variables
		LTexture();
		
		//Deallocates memory
		~LTexture();
		
		//Loads image at specified path
		bool loadFromFile(std::string);
		
		#ifdef _SDL_TTF_H
		//Creates image from font string
		bool loadFromRenderedText(std::string, SDL_Color, TTF_Font*);
		#endif
		
		//Deallocates texture
		void free();
		
		//Set color modulation
		void setColor(Uint8, Uint8, Uint8);
		
		//Set blending
		void setBlendMode(SDL_BlendMode);
		
		//Set alpha modulation
		void setAlpha(Uint8 alpha);
		
		//Renders texture at given point
		void render(int, int, SDL_Rect* = NULL, double = 0.0, SDL_Point* = NULL, SDL_RendererFlip = SDL_FLIP_NONE);
		
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

//Timer wrapper class
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

//The bullets
class Bullet{
	public:
		//The dimensions of the bullet
		static const int BULLET_WIDTH = 20;
		static const int BULLET_HEIGHT = 20;
		
		//Maximum axis velocity of the bullet
		static const int MAX_VEL = 5;
		
		//Initializes the variables
		Bullet(int, double, double, double);
		
		//Moves the bullet
		bool move();
		
		//Shows the bullet on the screen
		void render();
		
		Circle& getCollider();
	
	private:
		//The X and Y offsets of the bullet
		double mPosX, mPosY;
		
		//Velocity multiplier
		int mVel;
		
		//The velocity of the bullet
		double mVelX, mVelY;
		
		//The angle of the bullet
		double mAngle;
		
		//The "index" for the bullet speed pattern
		int mPattern;
		
		//Pointer to bullet texture image
		LTexture* mBulletTexture;
		
		//Bullet's collision circle
		Circle mCollider;
		
		//Bullet timer
		LTimer mTimer;
};

//The enemmies
class Enemy{
	public:
		//The dimensions of the enemy
		static const int ENEMY_WIDTH = 20;
		static const int ENEMY_HEIGHT = 20;
		
		//Initializes the variables
		Enemy(int, double, double);
		
		//Shoot the friggin' bullets
		void shoot();
		
		//Move enemy
		bool move();
		
		//Show the enemy on the screen
		void render();
	
	private:
		//The X and Y positions of the enemy
		double mPosX, mPosY;
		
		//The X and Y offsets of the enemy
		double mOffsetX, mOffsetY;
		
		//The angle and rotational speed of the enemy
		double mAngle, mRotation;
		
		//The number of and angle between bullets fired radially
		double mBulletsR, mBulletAngle;
		
		//The number and firing frequency of bullets fired linearly
		int mBulletsL, mBulletFrequency;
		
		//The "index" for the enemy path
		int mPath;
		
		//Enemy texture
		LTexture* mEnemyTexture;
		
		//Timer for the enemy
		LTimer mDurationTimer;
		
		//Timer for shooting
		LTimer mShotTimer;
};

//The player
class Player{
	public:
		static const int PLAYER_WIDTH = 20;
		static const int PLAYER_HEIGHT = 20;
		
		Player();
		
		void move(SDL_Event* e);
		
		void render();
		
		Circle& getCollider();
	
	private:
		double mPosX, mPosY;
		
		Circle mCollider;
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Checks collision between two circles
bool checkCollision(Circle&, Circle&);

//Functions for enemy path and bullet firing frequency
void followPath(int, double*, double*, double*, double);
bool shootBullet(int, int*, int*, double);

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//The font
TTF_Font* gFontBasic = NULL;
TTF_Font* gFontMain =  NULL;

//The sound effects
Mix_Chunk *gLow = NULL;
Mix_Music *gBGMusic = NULL;

//Main texture
LTexture gMainTexture;

//Playfield textures
LTexture gBulletTexture;
LTexture gEnemyTexture;
LTexture gPlayerTexture;
LTexture gBGTexture;

//Scoreboard textures
LTexture gScoreTexture, gScoreTextTexture;
LTexture gTimeTexture, gTimeTextTexture;

//Logo texture
LTexture gLogoTexture;
Circle gLogoCollider;
double gLogoAngle = 45;

//Things shown on scoreboard
LTimer gDurationTimer;
int gScore = 0;

//Vectors for bullets and enemies
std::vector<Bullet> gBullets;
std::vector<Enemy> gEnemies;

//Used for checking if the game is over
bool gGameOver = false;

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
			bool start = false;
			
			//Event handler
			SDL_Event e;

			//The background scrolling offset
			int scrollingOffset = 0;
			
			Player player;
			
			SDL_Color textColor = {0xD0, 0xD0, 0xD0, 0xFF};
			
			std::stringstream scoreText;
			std::stringstream timeText;

			//Spawn rate timer
			LTimer spawnTimer;
			

			int spawnRate = 1500;
					
			//While application is running
			while(!quit){									
				//Handle events on queue
				while(SDL_PollEvent(&e) != 0){
					//User requests quit
					if(e.type == SDL_QUIT){
						quit = true;
					}
					else if(e.type == SDL_KEYDOWN){
						if(e.key.keysym.sym == SDLK_RETURN){
							start = true;
							gDurationTimer.start();
							spawnTimer.start();
							Mix_PlayChannel(-1, gLow, 0);
						}
					}
					//Handle input from the player
					player.move(&e);
				}

				if(!start){
					gMainTexture.render((SCREEN_WIDTH-gMainTexture.getWidth())/2, (SCREEN_HEIGHT-gMainTexture.getHeight())/2);
				}
				else{
					//Keep running the program for 60 seconds
					if(gDurationTimer.getTicks()/1000 >= 60){
						gDurationTimer.pause();
						gGameOver = true;
					}
					
					//Screen animation at end and restart of game + Background music
					if(gGameOver && gBullets.empty()){
						if(PLAYFIELD_WIDTH != 0){
							PLAYFIELD_WIDTH -= 5;
							SCOREBOARD_WIDTH += 5;
							Mix_HaltMusic();
						}
					}else{
						if(SCOREBOARD_WIDTH != 100){
							SCOREBOARD_WIDTH -= 5;
							PLAYFIELD_WIDTH += 5;
							gDurationTimer.start();
						}
						if(Mix_PlayingMusic() == 0){
							Mix_PlayMusic(gBGMusic, -1);
						}
						else{
							Mix_ResumeMusic();
						}
					}
				
					
					SDL_Rect playfield = {0, 0, PLAYFIELD_WIDTH, SCREEN_HEIGHT};
					SDL_Rect scoreboard = {PLAYFIELD_WIDTH, 0, SCOREBOARD_WIDTH, SCREEN_HEIGHT};
					
					//Clear screen
					SDL_RenderClear(gRenderer);
					
					//Set the playfield viewport
					SDL_RenderSetViewport(gRenderer, &playfield);

					//Render the playfield
					SDL_SetRenderDrawColor(gRenderer, 0xB4, 0xB4, 0xB4, 0xFF);
					SDL_RenderFillRect(gRenderer, &playfield);

					//Scroll background
					--scrollingOffset;
					if(scrollingOffset < -gBGTexture.getHeight()){
						scrollingOffset = 0;
					}

					//Render background
					gBGTexture.render(0, scrollingOffset);
					gBGTexture.render(0, scrollingOffset + gBGTexture.getHeight());		
					
					//Spawn enemy every "spawnRate" frames
					if(spawnTimer.getTicks() >= spawnRate && !gGameOver && SCOREBOARD_WIDTH == 100){
						static double xOffset = SCREEN_WIDTH/4;
						
						//Determines the position of enemy spawn
						Enemy enemy(gDurationTimer.getTicks()/10000, xOffset+PLAYFIELD_WIDTH/2, -Enemy::ENEMY_HEIGHT);
						gEnemies.push_back(enemy);
						
						//Change the xOffset
						xOffset *= -1;
						
						//Restarts timer
						spawnTimer.start();
					}

					for(int i = 0; i < gEnemies.size(); ++i){
						//Move/rotate enemies
						if(!gEnemies[i].move() || gGameOver){
							gEnemies.erase(gEnemies.begin()+i);
						}else{
							//Shoot bullet;
							gEnemies[i].shoot();
						}
					}
					
					player.render();
					
					for(int i = 0; i < gBullets.size(); ++i){
						//Move the bullet
						if(!gBullets[i].move() || checkCollision(player.getCollider(), gBullets[i].getCollider())){
							gBullets.erase(gBullets.begin()+i);
						}else{
							//Render bullets
							gBullets[i].render();
						}
					}
					
					for(int i = 0; i < gEnemies.size(); ++i){
						//Render enemies
						gEnemies[i].render();
					}
					
					SDL_RenderSetViewport(gRenderer, &scoreboard);
					SDL_SetRenderDrawColor(gRenderer, 0xC8, 0x22, 0x22, 0xFF);
					
					scoreText.str("");
					scoreText << gScore;
					
					//Render text
					if(!gScoreTexture.loadFromRenderedText(scoreText.str().c_str(), textColor, gFontBasic)){
						printf("Unable to render score texture!\n");
					}
					
					//Render textures
					gScoreTextTexture.render((SCOREBOARD_WIDTH-gScoreTextTexture.getWidth())/2, SCREEN_HEIGHT-2*gScoreTextTexture.getHeight());
					gScoreTexture.render((SCOREBOARD_WIDTH-gScoreTexture.getWidth())/2, SCREEN_HEIGHT-gScoreTexture.getHeight());
					
					timeText.str("");
					timeText << gDurationTimer.getTicks()/1000;
					
					//Render text
					if(!gTimeTexture.loadFromRenderedText(timeText.str().c_str(), textColor, gFontBasic)){
						printf("Unable to render time texture!\n");
					}
					
					//Render textures
					gTimeTextTexture.render((SCOREBOARD_WIDTH-gTimeTextTexture.getWidth())/2, 0);
					gTimeTexture.render((SCOREBOARD_WIDTH-gTimeTexture.getWidth())/2, gTimeTexture.getHeight());
					
					//Render logo
					gLogoTexture.render((SCOREBOARD_WIDTH-gLogoTexture.getWidth())/2, (SCREEN_HEIGHT-gLogoTexture.getHeight())/2, NULL, gLogoAngle);
					
					gLogoCollider.x = (SCOREBOARD_WIDTH)/2+PLAYFIELD_WIDTH;
					gLogoCollider.y = (SCREEN_HEIGHT)/2;
					gLogoCollider.r = gLogoTexture.getWidth()/2;
				}
				//Update screen
				SDL_RenderPresent(gRenderer);
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
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0xFF, 0xFF, 0xFF));
		
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
bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor, TTF_Font* font){
	//Get rid of preexisting texture
	free();
	
	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText.c_str(), textColor);
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
			time = SDL_GetTicks()-mStartTicks;
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

Bullet::Bullet(int pattern, double sPosX, double sPosY, double sAngle){
	//Initializes the offsets
	mPosX = sPosX;
	mPosY = sPosY;
	
	//Initialize the velocity multiplier
	mVel = MAX_VEL;
	
	//Initializes the velocity
	mVelX = cos((sAngle+90)*PI/180)*mVel;
	mVelY = sin((sAngle+90)*PI/180)*mVel;
	
	//Initialize the angle
	mAngle = sAngle;
	
	//Initialize the bullet speed pattern
	mPattern = pattern;
	
	//Initializes the bullet pointer
	mBulletTexture = &gBulletTexture;
	
	//Initialize the bullet timer
	mTimer.start();
	
	//Initialize the collider
	mCollider.x = mPosX;
	mCollider.y = mPosY;
	mCollider.r = BULLET_WIDTH/2;
}

bool Bullet::move(){
	//Move the bullet
	mPosX += mVelX;
	mPosY += mVelY;
	
	//Shoot "fireworks"
	if(mPattern == 5 && mTimer.getTicks() >= 500){
		Enemy enemy(6, mPosX, mPosY);
		enemy.shoot();
		mPosY = SCREEN_HEIGHT+1;
	}
	
	//Change the collider position
	mCollider.x = mPosX;
	mCollider.y = mPosY;
	
	//If the bullet went too far to the left, right, up, or down
	if((mPosY+BULLET_HEIGHT < 0) || (mPosY > SCREEN_HEIGHT) || (mPosX+BULLET_WIDTH < 0) || (mPosX > PLAYFIELD_WIDTH)){
		//Return false if bullet is outside screen
		return false;
	}
	
	//Return true if bullet is inside screen
	return true;
}

void Bullet::render(){
	//Show the bullet
	mBulletTexture->render(mPosX, mPosY, NULL, mAngle);
}

Circle& Bullet::getCollider(){
	return mCollider;
}

Enemy::Enemy(int path, double sOffsetX, double sOffsetY){
	//Initialize the offsets
	mOffsetX = mPosX = sOffsetX;
	mOffsetY = mPosY = sOffsetY;
	
	//Initialize the angle and rotational speed of the enemy
	mAngle = 0;
	mRotation = 0;
	
	//Initialize the number of and angle between of bullets
	switch(path){
		case 0: case 1: case 2: case 6:
			mBulletsR = 10;
			break;
		case 3: case 5:
			mBulletsR = 1;
			break;
		case 4:
			mBulletsR = 4;
			break;
	}
	
	mBulletAngle = 360/mBulletsR;

	mBulletsL = 0;
	mBulletFrequency = 0;
	
	//Initialize the enemy path
	mPath = path;
	
	//Initialize enemy texture
	mEnemyTexture = &gEnemyTexture;
	
	//Initialize the timers;
	mDurationTimer.start();
	mShotTimer.start();
}

bool Enemy::move(){
	//Follow the path
	followPath(mPath, &mPosX, &mPosY, &mRotation, mDurationTimer.getTicks());
	
	//Add the offsets
	mPosX += mOffsetX;
	mPosY += mOffsetY;
	mAngle += mRotation;
	
	
	//Check if enemy is outside the screen
	if(mPosY > SCREEN_HEIGHT){
		//Return false if bullet is outside screen
		return false;
	}
	
	//Return true if bullet is inside screen
	return true;
}

void Enemy::shoot(){
	//Shoot every mBulletFrequency mSeconds
	if(shootBullet(mPath, &mBulletsL, &mBulletFrequency, mShotTimer.getTicks())){
		//Add bullet/s into "barrel" i.e. vector of bullets to be shot
		for(double i = mAngle; i < mAngle+360; i += mBulletAngle){
			Bullet bullet(mPath, mPosX, mPosY, i);
			gBullets.push_back(bullet);
		}
		++mBulletsL;
		mShotTimer.start();
	}
}

void Enemy::render(){
	//Show the bullet
	mEnemyTexture->render(mPosX, mPosY, NULL, mAngle);
}

Player::Player(){
	mPosX = (PLAYFIELD_WIDTH-PLAYER_WIDTH)/2;
	mPosY = (SCREEN_HEIGHT-PLAYER_HEIGHT)/2;
	
	//Initialize the collider
	mCollider.x = mPosX;
	mCollider.y = mPosY;
	mCollider.r = PLAYER_WIDTH/2;
}

void Player::move(SDL_Event* e){
	//If the mouse is moving
	SDL_ShowCursor(SDL_ENABLE);
	gLogoAngle = 45;
	
	//If the mouse is outside the screen
	if(e->motion.x+PLAYER_WIDTH < PLAYFIELD_WIDTH && e->motion.x > 0){
		mPosX = e->motion.x;
		
		//Hide the cursor from the screen
		SDL_ShowCursor(SDL_DISABLE);
	}
	
	if(e->motion.y+PLAYER_HEIGHT < SCREEN_HEIGHT && e->motion.y > 0){
		mPosY = e->motion.y;
	}
	
	mCollider.x = mPosX;
	mCollider.y = mPosY;
	
	if(gGameOver){
		//Create the collision circle for the restart button
		Circle mouse;
		mouse.x = e->motion.x;
		mouse.y = e->motion.y;
		mouse.r = 0;
		
		//Restart when logo is pressed
		if(checkCollision(mouse, gLogoCollider)){
			gLogoAngle = -45;
			if(e->button.button == SDL_BUTTON_LEFT && e->button.state == SDL_RELEASED){
				Mix_PlayChannel(-1, gLow, 0);
				gDurationTimer.stop();
				gScore = 0;
				gGameOver = false;
			}
		}
	}
}

void Player::render(){
	gPlayerTexture.render(mPosX, mPosY);
}

Circle& Player::getCollider(){
	return mCollider;
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
		gWindow = SDL_CreateWindow("Bullet Hell", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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
				//Initialize SDL_ttf
				if(TTF_Init() == -1){
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}
				//Initialize SDL_mixer
				if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0)				{
					printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
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
	
	//Load Main texture
	if(!gMainTexture.loadFromFile("Resources/main.bmp")){
		printf("Failed to load Main texture!\n");
		success = false;
	}

	//Load bullet texture
	if(!gBulletTexture.loadFromFile("Resources/bullet.bmp")){
		printf("Failed to load bullet texture!\n");
		success = false;
	}
	
	//Load enemy texture
	if(!gEnemyTexture.loadFromFile("Resources/enemy.bmp")){
		printf("Failed to load enemy texture!\n");
		success = false;
	}
	
	//Load player texture
	if(!gPlayerTexture.loadFromFile("Resources/player.bmp")){
		printf("Failed to load player texture!\n");
		success = false;
	}
	
	//Load logo texture
	if(!gLogoTexture.loadFromFile("Resources/logo.bmp")){
		printf("Failed to load logo texture!\n");
		success = false;
	}

	//Load background texture
	if(!gBGTexture.loadFromFile("Resources/bg.png")){
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	//Load BG sound 
	gBGMusic = Mix_LoadMUS("Resources/wasteland.mp3");
	if(gBGMusic == NULL){
		printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
	//Load button sound effect
	gLow = Mix_LoadWAV("Resources/low.wav");
	if(gLow == NULL){
		printf( "Failed to load low sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
	
	//Open the font
	gFontBasic = TTF_OpenFont("Resources/consola.ttf", 28);
	gFontMain = TTF_OpenFont("Resources/ostrich.ttf", 50);
	if(gFontBasic == NULL){
		printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	if(gFontMain = NULL){
		printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	else{
		//Set text color
		SDL_Color textColor = {0xD0, 0xD0, 0xD0, 0xFF};
		//Load 'time' texture
		if(!gTimeTextTexture.loadFromRenderedText("TIME", textColor, gFontBasic)){
			printf("Unable to render 'time' texture!\n");
			success = false;
		}
		
		//Load 'score' texture
		if(!gScoreTextTexture.loadFromRenderedText("HITS", textColor, gFontBasic)){
			printf("Unable to render 'time' texture!\n");
			success = false;
		}
	}
	
	return success;
}

void close(){
	//Free loaded images
	gMainTexture.free();
	gBulletTexture.free();
	gEnemyTexture.free();
	gPlayerTexture.free();
	gTimeTexture.free();
	gScoreTexture.free();
	gTimeTextTexture.free();
	gScoreTextTexture.free();
	gLogoTexture.free();
	
	//Free global fonts
	TTF_CloseFont(gFontBasic);
	gFontBasic = NULL;
	TTF_CloseFont(gFontMain);
	gFontMain = NULL;
	
	//Free sound effects
	Mix_FreeChunk(gLow);
	gLow = NULL;

	//Free music
	Mix_FreeMusic(gBGMusic);
	gBGMusic = NULL;

	//Destroy window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;
	
	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	Mix_Quit();
	SDL_Quit();
}

bool checkCollision(Circle& c1, Circle& c2){
	if(sqrt(pow(c1.x-c2.x, 2)+pow(c1.y-c2.y, 2)) < c1.r+c2.r){
		if(!gGameOver){
			++gScore;
		}
		return true;
	}
	return false;
}

//Moves in a straight line without rotating, then rotates after one second
void followPath(int i, double* xPos, double* yPos, double* rot, double t){
	*rot = 5;
	switch(i){
		case 0:
			//Moves in a straight line
			*yPos = t/10;
			*xPos = 0;
			break;
		case 1:
			//Moves in a catastrophic sine wave
			*yPos = t/10;
			*xPos = (t/100)*sin(t/100);
			break;
		case 2:
			//Moves in a damped sine wave
			*yPos = t/10;
			*xPos = 5000*sin(t/100)/(t/10);
			break;
		case 3:
			//Moves in a cycloid
			*yPos = t/10-30.0*sin(t/100);
			*xPos = 1-30.0*cos(t/100);
			*rot = 10;
			break;
		case 4:
			//Moves in an exponential curve
			*yPos = t;
			*xPos = exp(t/100);
			break;
		case 5:
			//Also moves in a straight line but rotates faster
			*yPos = t/10;
			*xPos = 0;
			*rot = SCREEN_HEIGHT/360;
			break;
	}
}

bool shootBullet(int i, int* n, int* f, double t){
	switch(i){
		case 0:
			*f = 1000;
			if(*n%2 > 0){*f = 100;}	
			break;
		case 1: case 2: case 5:
			*f = 500;
			break;
		case 3: case 4:
			*f = 50;
			break;
	}
	if(t >= *f){
		return true;
	}else{
		return false;
	}
}