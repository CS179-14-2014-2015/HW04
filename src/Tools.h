/*
 * Tools.h
 *
 *  Created on: Dec 15, 2014
 *      Author: guillaume
 */

#ifndef TOOLS_H_
#define TOOLS_H_

#include <SFML/Graphics.hpp>
#include <cmath>

using namespace sf;

class Tools {

public:
	static bool isTop(Sprite const &sprite);			//tell if a sprite touch the top of the screen
	static bool isBottom(Sprite const &sprite, RenderWindow const &window);		//tell if a sprite touch the bottom of the screen
	static bool isLeft(Sprite const &sprite);		//tell if a sprite touch the left of the screen
	static bool isRight(Sprite const &sprite, RenderWindow const &window);		//tell if a sprite touch the right of the screen
	static bool isOutside(Sprite const &sprite, RenderWindow const &window);		//tell if a sprite touch a border of the screen
	static float norm(Vector2f a, Vector2f b);		//give the norm of a vector

};

#endif /* TOOLS_H_ */
