/*
 * Tools.cpp
 *
 *  Created on: Dec 15, 2014
 *      Author: guillaume
 */

#include "Tools.h"


bool Tools::isTop(Sprite const &sprite) {
	return sprite.getPosition().y <= sprite.getGlobalBounds().height / 2;
}

bool Tools::isLeft(Sprite const &sprite){
	return sprite.getPosition().x <= sprite.getGlobalBounds().width / 2;
}

bool Tools::isBottom(Sprite const &sprite, RenderWindow const &window){
	return sprite.getPosition().y >= window.getSize().y - sprite.getGlobalBounds().height / 2;
}

bool Tools::isRight(Sprite const &sprite, RenderWindow const &window){
	return sprite.getPosition().x >= window.getSize().x - sprite.getGlobalBounds().width / 2;
}

bool Tools::isOutside(Sprite const &sprite, RenderWindow const &window){
	return isTop(sprite) || isLeft(sprite) ||  isBottom(sprite, window) || isRight(sprite, window);
}

float Tools::norm(Vector2f a, Vector2f b) {
	float x = b.x - a.y;
	float y = b.y - a.y;

	return (float) sqrt(x*x + y*y);
}


