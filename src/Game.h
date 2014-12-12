/*
 * Game.h
 *
 *  Created on: Dec 11, 2014
 *      Author: guillaume
 */

#ifndef GAME_H_
#define GAME_H_

#include <SFML/Graphics.hpp>
#include <vector>
/*
#include "Player.h"
#include "PlayerBullet.h"
#include "EnemyBullet.h"
#include "Boss.h"
*/
#include "Background.h"

using namespace sf;

class Game {

protected:
	RenderWindow window;				//the game window
	Event event;						//for user inputs
	/*
	Player player; 						//the player
	vector<PlayerBullet> playerBullets;	//all the player bullet at a given time public:
	Boss boss;							//Bruce springsteen
	vector<Enemy> enemies;				//all the enemies at a given time
	vector<EnemyBullet>	enemiesBullets;	//all the bullets that can hit the player
	*/
	Background background;

public:
	Game();						//initiate the member fields and set up the application
	void test();				//run a serial of tests
	void testPlayer();			//test the player : movements, collisions, attack
	void testPlayerBullets();	//test the player's bullets : movements, collision, destruction after exit
	void testEnemyBullets();	//test the enemy's bullets : damage the player
	void testEnemyShips(); 	//test the behavior of enemy ship : movements, attack, damage given and taken,
								//collisions, destruction when exit the game
	void testBoss();			//test the behavior of the boss : bullet
	void run();					//run the game
	void update();				//update the elements on the screen
};

#endif /* GAME_H_ */
