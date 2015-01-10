/*
 * Bullet.cpp
 *
 *  Created on: Dec 15, 2014
 *      Author: guillaume
 */

#include "Bullet.h"

Bullet::Bullet(const Texture & texture, const float& xSpeed, const float & ySpeed, const float &spin) :
Sprite(texture) ,xSpeed(xSpeed), ySpeed(ySpeed), spin(spin){
	setOrigin(getGlobalBounds().width / 2, getGlobalBounds().height / 2);
}

float Bullet::getXspeed() const{
	return xSpeed;
}

float Bullet::getYspeed() const{
	return ySpeed;
}

float Bullet::getSpin() const {
	return spin;
}

void Bullet::setXspeed(float speed) {
	this->xSpeed = speed;
}

void Bullet::setYspeed(float speed) {
	this->ySpeed = speed;
}



