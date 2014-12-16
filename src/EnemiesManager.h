/*
 * EnemiesManager.h
 *
 *  Created on: Dec 14, 2014
 *      Author: guillaume
 */

#ifndef ENEMIESMANAGER_H_
#define ENEMIESMANAGER_H_

#include  <SFML/Graphics.hpp>
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
	Texture texture;					//the enemies texture
	list<Enemy> enemies;				//the list of enemies
	unsigned int frameCounter;			//frame counter to handle updates

public:
	EnemiesManager(RenderWindow &window);
	void addEnemy();											//create a new enemy
	void clearEnemies();										//clear dead enemies
    void checkPlayerBullets(list<Bullet> &playerBullets); 	//check if enemies are touched by player bullets
	void update(Player &player);								//update the enemies

};

#endif /* ENEMIESMANAGER_H_ */
