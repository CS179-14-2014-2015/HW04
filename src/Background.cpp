/*
 * Background.cpp
 *
 *  Created on: Dec 12, 2014
 *      Author: guillaume
 */

#include "Background.h"

Background::Background(RenderWindow& window) : window(window){

	//load the texture, alert if failed
	if(!texture.loadFromFile("resources/background.jpg")) {
		cout<< "Impossible to load background.jpg" <<endl;
		exit(EXIT_FAILURE);
	}

	//load the texture in the sprites
	sprite1.setTexture(texture);
	sprite1.setPosition(0 , 0);
	sprite2.setTexture(texture);
	sprite2.setPosition(window.getSize().x, 0);
}

void Background::update() {

	//move the background
	const int speed = 5;
	sprite1.move(-speed, 0);
	sprite2.move(-speed, 0);

	//replace the background when it repeats
	if(sprite2.getPosition().x <= 0) {
		sprite1.setPosition(0,0);
		sprite2.setPosition(window.getSize().x, 0);
	}

	//update the window
	window.draw(sprite1);
	window.draw(sprite2);
}



