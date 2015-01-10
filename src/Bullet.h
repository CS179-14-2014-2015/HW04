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
	float xSpeed;	//the x speed of the bullet
	float ySpeed;	//the y speed of the bullet
	float spin;		//the spin of the bullet

public:
	Bullet(const Texture & texture, const float& xSpeed, const float& ySpeed, const float &spin = 0);

    float getXspeed() const;
    float getYspeed() const;
    float getSpin() const;

	void setXspeed(float speed);
	void setYspeed(float speed);
};

#endif /* BULLET_H_ */
