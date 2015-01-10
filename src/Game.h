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
#include <string>
#include <iostream>
#include <sstream>

#include "Background.h"
#include "Player.h"
#include "EnemiesManager.h"

using namespace sf;
using namespace std;

class Game {

protected:
	RenderWindow window;				//the game window
	Event event;						//for user inputs
	Player player; 						//the player
	Clock clock;						//the clock of the game
	EnemiesManager enemiesManager;		//handle all the enemies of the game
	Background background;

	bool gameIsOver;

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
	void displayScore(unsigned int const &score);	//display the score and wait for restart
};

#endif /* GAME_H_ */
