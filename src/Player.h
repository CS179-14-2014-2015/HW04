/*
 * Player.h
 *
 *  Created on: Dec 13, 2014
 *      Author: guillaume
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include <iostream>
#include <SFML/Graphics.hpp>
#include <list>

#include "Tools.h"
#include "Bullet.h"

using namespace sf;
using namespace std;

class Player : public Sprite {

protected:
	unsigned int score;				//contains the score of the player, increase when touched
	Texture playerTexture;				//contains the texture of the player
	Texture bulletTexture;				//contains the texture of the bullets
	RenderWindow &window;				//the application window
	bool isMovingUp;					//used to know how to move the player, switch on user input
	bool isMovingDown;					//used to know how to move the player, switch on user input
	bool isMovingLeft;					//used to know how to move the player, switch on user input
	bool isMovingRight;					//used to know how to move the player, switch on user input
	bool isFadingIn;					//used to know if the player is fading in
	bool isFadingOut;					//used to know if the player is fading out
	unsigned int frameCounter;			//count the number of frame elapsed
	list<Bullet> bullets;				//contains the bullets of the player

public:
	Player(RenderWindow &window); 							//initialize the player
	void update();											//update the player
	void blink();											//start a blink animation when the player is hit
	void setIsMovingUp(bool isMovingUp);
	void setIsMovingDown(bool isMovingDown);
	void setIsMovingLeft(bool isMovingLeft);
	void setIsMovingRight(bool isMovingRight);
	void fire();											//create a new bullet
	list<Bullet>& getBullets();
	void clearBullets();									//clear obsolete bullets
	void moveBullets();									//move the bullets
};

#endif /* PLAYER_H_ */
