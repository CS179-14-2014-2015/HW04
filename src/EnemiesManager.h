/*
 * EnemiesManager.h
 *
 *  Created on: Dec 14, 2014
 *      Author: guillaume
 */

#ifndef ENEMIESMANAGER_H_
#define ENEMIESMANAGER_H_

#include  <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <list>
#include <stdlib.h>
#include <time.h>

#include "Enemy.h"
#include "Player.h"
#include "Bullet.h"

using namespace sf;

class EnemiesManager{

protected:
	RenderWindow &window;				//the application window

	Texture enemyTexture;				//the enemies texture
	Texture bulletTexture;				//the texture of the bullets
    SoundBuffer buffer;					// Contain the stream of boum.wav
	Sound sound;						//used to play the boum sound

	list<Enemy> enemies;				//the list of enemies
	list<Bullet> bullets;				//contains the bullets of all the enemies

	unsigned int frameCounter;			//frame counter to handle updates

public:
	EnemiesManager(RenderWindow &window);
	void addEnemy();											//create a new enemy
	void clearEnemies();										//clear dead enemies
    void checkPlayerBullets(Player &player); 	//check if enemies are touched by player bullets
    void checkBullet(Player &player);					//check if bullets touched the player
    void checkEnemies(Player &player);					//check if enemies touched the player
    void fire(Vector2f const &player);				 			//Make all the enemies fire toward the player
    void moveEnemies();										//Move all the enemies
    void moveBullets();										//Move the bullets
    void clearBullets();										//clear off screen bullets
	void update(Player &player);								//update the enemies
	void restart();							//restart the enemiesManager
};

#endif /* ENEMIESMANAGER_H_ */
