/*
 * Enemy.h
 *
 *  Created on: Dec 14, 2014
 *      Author: guillaume
 */

#ifndef ENEMY_H_
#define ENEMY_H_

#include "SFML/Graphics.hpp"
#include <list>

#include "Tools.h"
#include "Bullet.h"

using namespace sf;
using namespace std;

class Enemy : public Sprite {

protected:
	unsigned int health;		//the health of the enemy
	float xSpeed;				//the x speed of the enemy
	float ySpeed;				//the y speed of the enemy

public:
	Enemy(const Texture &texture);
	float getXspeed() const;
	float getYspeed() const;
	void checkPlayerBullets(list<Bullet> &playerBullets);		//handle collision with player bullets
	bool isDead() const;											//say if the enemy is dead(exit or no health)
	Bullet fire(Vector2f const & player, Texture const &texture);	//fire toward the player
};

#endif /* ENEMY_H_ */
