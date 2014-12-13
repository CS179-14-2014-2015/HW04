/*
 * Player.cpp
 *
 *  Created on: Dec 13, 2014
 *      Author: guillaume
 */

#include "Player.h"

Player::Player(RenderWindow &window) : window(window), speed(15) {

	//load the texture, alert if failed
	if(!texture.loadFromFile("resources/player.jpg"))
		cout<< "Impossible to load player.jpg" <<endl;
	sprite.setTexture(texture);

	//center origin, position and  initialize flags
	sprite.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2);
	sprite.setPosition(sprite.getGlobalBounds().width / 2, window.getSize().y / 2);
	isMovingUp = false;
	isMovingDown = false;
	isMovingLeft = false;
	isMovingRight = false;
}

void Player::setIsMovingDown(bool isMovingDown) {
	this->isMovingDown = isMovingDown;
}

void Player::setIsMovingLeft(bool isMovingLeft) {
	this->isMovingLeft = isMovingLeft;
}

bool Player::isTop() {
	return sprite.getPosition().y <= sprite.getGlobalBounds().height / 2;
}

bool Player::isBottom() {
	return sprite.getPosition().y >= window. getSize().y - sprite.getGlobalBounds().height / 2 ;
}

bool Player::isLeft() {
	return sprite.getPosition().x <= sprite.getGlobalBounds().width / 2;
}

bool Player::isRight() {
	return sprite.getPosition().x >= window.getSize().x - sprite.getGlobalBounds().width / 2;
}

void Player::setIsMovingRight(bool isMovingRight) {
	this->isMovingRight = isMovingRight;
}

void Player::setIsMovingUp(bool isMovingUp) {
	this->isMovingUp = isMovingUp;
}


void Player::update() {

	//move the player up
	if(isMovingUp && !isTop())
		sprite.move(0, - speed);

	//Move the player down
	if(isMovingDown && !isBottom())
		sprite.move(0, speed);

	//Move the player left
	if(isMovingLeft && !isLeft())
		sprite.move(-speed, 0);

	//Move the player right
	if(isMovingRight && !isRight())
		sprite.move(speed, 0);

	window.draw(sprite);
}
