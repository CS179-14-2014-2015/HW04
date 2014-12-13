/*
 * Player.h
 *
 *  Created on: Dec 13, 2014
 *      Author: guillaume
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include <iostream>
#include <list>
/*
#include "PlayerBullet.h"
*/

#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class Player {
protected:
	unsigned int score;				//contains the score of the player, increase when touched
	const int speed;			//the speed of the player
	Texture texture;					//contains the texture of the player
	Sprite sprite;						//the sprite of the player
	RenderWindow &window;				//the application window
	bool isMovingUp;					//used to know how to move the player, switch on user input
	bool isMovingDown;					//used to know how to move the player, switch on user input
	bool isMovingLeft;					//used to know how to move the player, switch on user input
	bool isMovingRight;					//used to know how to move the player, switch on user input
/*
	list<PlayerBullet> playerBullets	//the player's bullets
	list<EnemyBullet> enemyBullets	//the bullets that can hit the player
*/

public:
	Player(RenderWindow &window);				//initialize the player
	void update();								//update the player
	bool isTop();								//test if the player hit the top of the window
	bool isBottom();							//test if the player hit the bottom of the window
	bool isLeft();								//test if the player hit the left of the window
	bool isRight();							//test if the player hit the right of the window
	void fire();								//fire a bullet, update playerBullets
	void blink();								//blink animation when the player is touched
	void setIsMovingUp(bool isMovingUp);
	void setIsMovingDown(bool isMovingDown);
	void setIsMovingLeft(bool isMovingLeft);
	void setIsMovingRight(bool isMovingRight);

	/*
	bool isTouched(enemyBullets);		//check if the player is touched, update the score and blink
	 */
}
;

#endif /* PLAYER_H_ */
