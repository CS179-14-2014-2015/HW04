/*
 * Player.cpp
 *
 *  Created on: Dec 13, 2014
 *      Author: guillaume
 */

#include "Player.h"

Player::Player(RenderWindow &window) : window(window) {

	//load the texture, alert if failed
	if(!playerTexture.loadFromFile("resources/player.jpg")){
		cout<< "Impossible to load player.jpg" <<endl;
		exit(EXIT_FAILURE);
	}
	if(!bulletTexture.loadFromFile("resources/playerBullet.png")){
		cout<< "Impossible to load playerBullet.png" <<endl;
		exit(EXIT_FAILURE);
	}
	setTexture(playerTexture);

	//initialize the player
	setOrigin(getGlobalBounds().width / 2, getGlobalBounds().height / 2);
	restart();
}

void Player::update() {

	//blink animation
	int a = getColor().a;
	int fadeRate = 30;
	if(isFadingIn){
		a -= fadeRate;
		if(a <= 0) {
			a = 0;
			isFadingIn = false;
			isFadingOut = true;
		}
		setColor(Color(225, 225, 225, a));
	}
	if(isFadingOut) {
		a += fadeRate;
		if(a >= 225){
			a = 225;
			isFadingOut = false;
		}
			setColor(Color(255, 255, 255, a + fadeRate));
	}

	//clear obsolete bullets
	clearBullets();

	//move bullets
	moveBullets();

	//fire
	frameCounter++;
	if(frameCounter % 3 == 0)
		fire();

	//HANDLE PLAYER MOVE
	const int speed = 18;
	//move the player up
	if(isMovingUp)
		move(0, - speed);
	if(Tools::isTop(*this))
		setPosition(getPosition().x, getGlobalBounds().height / 2);

	//Move the player down
	if(isMovingDown)
		move(0, speed);
	if(Tools::isBottom(*this, window))
		setPosition(getPosition().x,  window.getSize().y - getGlobalBounds().height / 2);

	//Move the player left
	if(isMovingLeft)
		move(-speed, 0);
	if(Tools::isLeft(*this))
		setPosition(getGlobalBounds().width / 2, getPosition().y);

	//Move the player right
	if(isMovingRight)
		move(speed, 0);
	if(Tools::isRight(*this, window))
		setPosition(window.getSize().x - getGlobalBounds().width / 2, getPosition().y);

	//draw the player
	window.draw(*this);
}


void Player::setIsMovingDown(bool isMovingDown) {
	this->isMovingDown = isMovingDown;
}

void Player::setIsMovingLeft(bool isMovingLeft) {
	this->isMovingLeft = isMovingLeft;
}


void Player::fire() {

	//create the bullet and add it to the list
	Bullet playerBullet(bulletTexture, 18, 0);
	playerBullet.setOrigin(playerBullet.getGlobalBounds().width / 2, playerBullet.getGlobalBounds().height / 2);
	playerBullet.setPosition(getPosition().x + getGlobalBounds().width / 2 + 5, getPosition().y);
	bullets.push_back(playerBullet);
}

void Player::getHit() {

	//blink animation
	if(isFadingIn == false && isFadingOut == false)
		isFadingIn = true;

	//decrease the shield
	shield--;
}

bool Player::isDead() {
	return shield < 1;
}

void Player::increaseScore(unsigned short point) {
	score += point;
}

list<Bullet>& Player::getBullets() {
	return bullets;
}

void Player::setIsMovingRight(bool isMovingRight) {
	this->isMovingRight = isMovingRight;
}

void Player::setIsMovingUp(bool isMovingUp) {
	this->isMovingUp = isMovingUp;
}

void Player::clearBullets(){

	//clean player bullets
	for(list<Bullet>::iterator it = bullets.begin(); it != bullets.end(); it++){
		if(Tools::isOutside(*it, window))
			it = bullets.erase(it);
	}
}


void Player::moveBullets() {

	//Move the player's bullets and draw them
	for(list<Bullet>::iterator it = bullets.begin(); it != bullets.end(); it++){
		it->move(it->getXspeed(), it->getYspeed());
		window.draw(*it);
	}
}

void Player::restart() {
	setPosition(getGlobalBounds().width / 2, window.getSize().y / 2);
	isMovingUp = false;
	isMovingDown = false;
	isMovingLeft = false;
	isMovingRight = false;
	isFadingIn = false;
	isFadingOut = false;
	shield = 10;
	score = 0;
	bullets.clear();
	setColor(Color(255, 255, 255, 255));
}

unsigned int Player::getScore() {
	return score;
}

