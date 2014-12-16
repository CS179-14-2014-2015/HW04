/*
 * Enemy.cpp
 *
 *  Created on: Dec 15, 2014
 *      Author: guillaume
 */

#include "Enemy.h"








Enemy::Enemy(const Texture & texture) : Sprite(texture){

	//init speeds
	xSpeed = - 10;
	ySpeed = 0;
	health = 3;
}

float Enemy::getXspeed() const{
	return xSpeed;
}



float Enemy::getYspeed() const{
	return ySpeed;
}

void Enemy::checkPlayerBullets(list<Bullet> &playerBullets){

	//check for collisions and delete bullets
	for(list<Bullet>::iterator it = playerBullets.begin(); it != playerBullets.end(); it++){

		if(it->getGlobalBounds().intersects(getGlobalBounds())){
			health--;
			it = playerBullets.erase(it);
		}
	}
}


bool Enemy::isDead() const{
	return health == 0 || Tools::isLeft(*this);
}





