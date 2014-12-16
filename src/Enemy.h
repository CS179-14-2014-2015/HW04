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
	unsigned int health;
	float xSpeed;
	float ySpeed;

public:
	Enemy(const Texture &texture);
	float getXspeed() const;
	float getYspeed() const;
	void checkPlayerBullets(list<Bullet> &playerBullets) ;
	bool isDead() const;
};

#endif /* ENEMY_H_ */
