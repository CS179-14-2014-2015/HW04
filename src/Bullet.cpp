/*
 * Bullet.cpp
 *
 *  Created on: Dec 15, 2014
 *      Author: guillaume
 */

#include "Bullet.h"

Bullet::Bullet(const Texture & texture, const float& xSpeed, const float & ySpeed) : Sprite(texture) ,xSpeed(xSpeed), ySpeed(ySpeed){}

int Bullet::getXspeed() const{
	return xSpeed;
}

int Bullet::getYspeed() const{
	return ySpeed;
}

