/*
 * Backgroud.h
 *
 *  Created on: Dec 12, 2014
 *      Author: guillaume
 */

#ifndef BACKGROUD_H_
#define BACKGROUD_H_

#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
using namespace std;


//Class that handle the scrolling background of the game
class Background {

protected:
	RenderWindow &window;		//the application window
	Texture texture;			//contains the texture of the background
	Sprite sprite1;				//the sprite used to display the background
	Sprite sprite2;				//the sprite used to display the background

public:
	Background(RenderWindow &window);		//initialize the sprites and store the window
	void update();				//scroll the background and update the window
};



#endif /* BACKGROUD_H_ */
