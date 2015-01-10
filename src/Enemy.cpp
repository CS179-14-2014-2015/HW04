/*
 * Enemy.cpp
 *
 *  Created on: Dec 15, 2014
 *      Author: guillaume
 */

#include "Enemy.h"

Enemy::Enemy(const Texture & texture) : Sprite(texture){

	//init speeds
	xSpeed = - 5;
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

Bullet Enemy::fire(const Vector2f& player, const Texture& texture) {

	//create the bullet and place it
	Bullet bullet(texture, -5, 0, 10);
	float xB = getPosition().x - getGlobalBounds().width / 2 - 5;
	float yB = getPosition().y;
	bullet.setPosition(xB, yB);

	//calculate the vector toward the player
	float norm = Tools::norm(player, bullet.getPosition());
	int speed = 5;
	float xSpeed = (player.x - xB) * speed / norm;
	float ySpeed = (player.y - yB) * speed / norm;
	bullet.setXspeed(xSpeed);
	bullet.setYspeed(ySpeed);

	return bullet;
}





