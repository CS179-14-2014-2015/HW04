/*
 * Bullet.h
 *
 *  Created on: Dec 14, 2014
 *      Author: guillaume
 */

#ifndef BULLET_H_
#define BULLET_H_

#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class Bullet : public Sprite {

protected:
	float xSpeed;
	float ySpeed;

public:
	Bullet(const Texture & texture, const float& xSpeed, const float& ySpeed);
    int getXspeed() const;
    int getYspeed() const;

};

#endif /* BULLET_H_ */
