#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <vector>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_FPS = 30;
const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;
int countedFrames = 0;


SDL_Window* gWindow = nullptr;
SDL_Renderer* renderer = nullptr;
TTF_Font *font = nullptr;

class Texture
{
    public:
        Texture();
        bool loadFromFile(std::string path);
        bool loadFromText(std::string textureText, SDL_Color textColor);
        void free();
        void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );
        int getHeight();
        int getWidth();
        ~Texture();

    private:
        SDL_Texture* texture;
        int width, height;
};

Texture::Texture()
{
    texture = nullptr;
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
    SDL_Texture* newTexture = nullptr;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());

    if(loadedSurface == nullptr)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }
    else
    {
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0, 0));
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface );

        if(newTexture == nullptr)
        {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }
        else
        {
            width = loadedSurface->w;
            height = loadedSurface->h;
        }

        SDL_FreeSurface(loadedSurface);
    }

    texture = newTexture;
    return texture != nullptr;
}

bool Texture::loadFromText(std::string textureText, SDL_Color textColor)
{
    free();

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText.c_str(), textColor);

    if(textSurface == nullptr)
    {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    }
    else
    {
        texture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if(texture == nullptr)
        {
            printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        }
        else
        {
            width = textSurface->w;
            height = textSurface->h;
        }

        SDL_FreeSurface(textSurface);
    }

    return texture != nullptr;
}

void Texture::free()
{
    if (texture != nullptr)
    {
        SDL_DestroyTexture(texture);
        texture = nullptr;
        width = 0;
        height = 0;
    }
}

void Texture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
    SDL_Rect renderQuad = {x, y, width, height };

    if(clip != nullptr)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    SDL_RenderCopyEx(renderer, texture, clip, &renderQuad, angle, center, flip);
}

int Texture::getHeight()
{
    return height;
}

int Texture::getWidth()
{
    return width;
}

bool init();
bool loadStart();
bool loadScore(int hits);
void close();
double distanceSquared(int x1, int y1, int x2, int y2);

Texture start;
Texture score;
Texture background;

class Bullet
{
    public:
        static const int bRadius = 5;

        Bullet();
        void move(int x, int y);
        void render();
        int getX();
        int getY();
        void Destroy();
        ~Bullet();

        int bPosX, bPosY;
        int bInitX, bInitY;
};

Bullet::Bullet()
{
    bPosX = rand() % SCREEN_WIDTH;
    bPosY = rand() % SCREEN_HEIGHT;
}

Bullet::~Bullet()
{
}

void Bullet::move(int x, int y)
{
    bPosX += x;
    bPosY += y;

    if (bPosX > SCREEN_WIDTH || bPosY > SCREEN_HEIGHT)
    {
        bPosX = rand() % SCREEN_WIDTH;
        bPosY = rand() % SCREEN_HEIGHT;
    }
}

void Bullet::render()
{
    filledCircleRGBA(renderer, bPosX, bPosY, bRadius, 0xFF,0x0,0x0,0xFF);
}

int Bullet::getX()
{
    return bPosX;
}

int Bullet::getY()
{
    return bPosY;
}

void Bullet::Destroy()
{
 delete this;
}

class BulletGroup
{
    public:
        std::vector<Bullet> bGroup;

        BulletGroup();
        void moveCircular();
        void moveStraight();
        void render();
        ~BulletGroup();
};

BulletGroup::BulletGroup()
{
    for(int i = 0; i < 10; i++)
        bGroup.push_back(Bullet());
}

BulletGroup::~BulletGroup()
{
}

void BulletGroup::moveCircular()
{
    for(auto &x:bGroup)
    {
        static int bVelX = rand() % 3 - 1;
        static int bVelY = rand() % 3 - 1;
        x.move(bVelX, bVelY);
    }
}

void BulletGroup::moveStraight()
{

    for(auto &x:bGroup)
    {
        static int bVelX = rand() % 5 - 1;
        static int bVelY = rand() % 5 - 1;
        x.move(bVelX, bVelY);
    }
}

void BulletGroup::render()
{
    for(auto &x:bGroup)
        x.render();
}

class Player
{
    public:
        static const int pRadius = 10;
        bool start = false;

        Player();
        bool collisionDetection(Player* a, BulletGroup* b);
        void handleEvent(SDL_Event &e);
        void render();
        int getHits();
        bool hasStarted();
        ~Player();

    private:
        int mPosX, mPosY;
        int hits;
};

Player::Player()
{
    mPosX = SCREEN_WIDTH/2;
    mPosY = SCREEN_HEIGHT/2;
    hits = 0;
}

Player::~Player()
{
}

bool Player::collisionDetection(Player* a, BulletGroup* b)
{
    int counter = 0;
    for(Bullet x : b->bGroup)
    {
        int totalRadiusSquared = a->pRadius + x.bRadius;
        totalRadiusSquared *= totalRadiusSquared;

        if(distanceSquared(a->mPosX, a->mPosY, x.getX(), x.getY()) < totalRadiusSquared)
        {
            hits += 1;
            b->bGroup[counter] = Bullet();
            return true;
        }
        ++counter;
    }

    return false;
}

void Player::handleEvent(SDL_Event &e)
{


    if (start)
    {
        SDL_ShowCursor(0);

        if (e.type == SDL_MOUSEMOTION)
        {
            mPosX = e.motion.x;
            mPosY = e.motion.y;

            for (int i = (pRadius); i >= 0; i--)
            {
                if (mPosX <= i)
                {
                    mPosX += (pRadius-i);
                }

                if (mPosX >= (SCREEN_WIDTH - i))
                {
                    mPosX -= (pRadius-i);
                }

                if (mPosY <= i)
                {
                    mPosY += (pRadius-i);
                }

                if (mPosY >= (SCREEN_HEIGHT - i))
                {
                    mPosY -= (pRadius-i);
                }
            }
        }
    }
}

void Player::render()
{
    filledCircleRGBA(renderer, mPosX, mPosY, pRadius, 255, 255, 255, 255);
}

int Player::getHits()
{
return hits;
}

bool Player::hasStarted()
{
 return start;
}

class Start
{
    public:
        Start();
        void click(SDL_Event &e, Player &p);
        void render();
        int getX();
        int getY();
        ~Start();

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

void Start::click(SDL_Event &e, Player &p)
{
    if (e.type == SDL_MOUSEBUTTONDOWN)
    {
        if (e.button.button == SDL_BUTTON_LEFT)
        {
            mouseX = e.button.x;
            mouseY = e.button.y;

            if ((mouseX > getX()) && (mouseX < (getX() + start.getWidth())) && (mouseY > getY()) && (mouseY < (getY() + start.getHeight())))
            {
                start.free();
                p.start = true;

            }
        }
    }
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
    bool success = true;

    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            printf("Warning: Linear texture filtering not enabled!");
        }

        gWindow = SDL_CreateWindow("Pepero", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

        if(gWindow == NULL)
        {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            renderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

            if(renderer == NULL)
            {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                int imgFlags = IMG_INIT_PNG;

                if(!(IMG_Init(imgFlags) &imgFlags))
                {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }

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
    bool success = true;

    if (!start.loadFromFile("start.bmp"))
    {
        printf( "Failed to load start texture!\n" );
        success = false;
    }

    return success;
}

bool loadBackground()
{
    bool success = true;

    if (!background.loadFromFile("background.bmp"))
    {
        printf( "Failed to load background texture!\n" );
        success = false;
    }

    return success;
}

bool loadScore(int hits)
{
    bool success = true;
    std::string text = "Hits " + std::to_string(hits);
    font = TTF_OpenFont("scoreboard.ttf", 32);

    if(font == NULL)
    {
        printf("Failed to load scoreboard font! SDL_ttf Error: %s\n", TTF_GetError());
        success = false;
    }
    else
    {
        SDL_Color textColor = {0, 0, 255};

        if(!score.loadFromText(text, textColor))
        {
            printf("Failed to render text texture!\n");
            success = false;
        }
    }

    return success;
}

double distanceSquared( int x1, int y1, int x2, int y2 )
{
    int deltaX = x2 - x1;
    int deltaY = y2 - y1;
    return deltaX*deltaX + deltaY*deltaY;
}

void close()
{
    score.free();
    TTF_CloseFont(font);
    font = NULL;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    renderer = NULL;

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* args[])
{
    if(!init())
    {
        printf("Failed to initialize!\n");
    }
    else
    {
        if(!loadStart())
        {
            printf("Failed to load start!\n");
        }
        else
        {
            bool quit = false;
            SDL_Event e;
            Start s;
            Player player;
            BulletGroup lol, meh;

            int scrollingOffset = 0;

            uint initTimer = SDL_GetTicks();

            while(!quit)
            {
                loadBackground();

                uint capTimer = SDL_GetTicks();
                uint gameTimer = SDL_GetTicks() - initTimer;
                printf("%u", gameTimer);
                printf("\n");
                  if (player.hasStarted() && ((player.collisionDetection(&player, &lol) == true || player.collisionDetection(&player, &meh)== true)))
                  {
                      loadScore(player.getHits());
                  }

                  while(SDL_PollEvent(&e) != 0)
                  {
                      if(e.type == SDL_QUIT)
                      {
                          quit = true;
                      }

                      s.click(e, player);
                      player.handleEvent(e);
                  }
                  if (gameTimer < 60000) // in milliseconds :)
                  {
                    lol.moveStraight();
                    meh.moveCircular();

                    ++scrollingOffset;
                    if(scrollingOffset > background.getHeight())
                    {
                     scrollingOffset = 0;
                    }

                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderClear(renderer);

                    background.render(0, scrollingOffset);
                    background.render(0, scrollingOffset - background.getHeight());

                    player.render();
                    s.render();
                    score.render(SCREEN_WIDTH - score.getWidth(),0);
                    lol.render();
                    meh.render();

                    SDL_RenderPresent(renderer);
                    ++countedFrames;

                }
                else {
                // Game Over code here
                SDL_RenderClear(renderer);
                SDL_RenderPresent(renderer);
                }
                if( capTimer < SCREEN_TICK_PER_FRAME )
                {
                    //Wait remaining time
                    SDL_Delay( SCREEN_TICK_PER_FRAME - capTimer );
                }


            }
        }
    }

    close();
    return 0;
}
