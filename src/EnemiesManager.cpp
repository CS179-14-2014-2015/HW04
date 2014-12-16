/*
 * EnemiesManager.cpp
 *
 *  Created on: Dec 15, 2014
 *      Author: guillaume
 */

#include "EnemiesManager.h"

EnemiesManager::EnemiesManager(RenderWindow &window) : window(window){

	//load the texture, alert if failed
	if(!texture.loadFromFile("resources/enemy.png")) {
		cout<< "Impossible to load enemy.png" <<endl;
		exit(EXIT_FAILURE);
	}

	//initialize counters
	frameCounter = 0;
	srand (time(NULL));
}



void EnemiesManager::update(Player &player)
{
	frameCounter++;

	//check player bullet collision
	checkPlayerBullets(player.getBullets());

	//clear dead enemies
	clearEnemies();

	//add enemy
	if(frameCounter % 20 == 0) {
		addEnemy();
	}

	//move enemies
	for(list<Enemy>::iterator it = enemies.begin(); it != enemies.end(); it++){

		it->move(it->getXspeed(), it->getYspeed());
		window.draw(*it);
	}
}

void EnemiesManager::addEnemy(){

	//Create sprite, place the enemy, modify origin and add it to the list
	Enemy enemy(texture);
	enemy.setOrigin(enemy.getGlobalBounds().width / 2, enemy.getGlobalBounds().height / 2);
	int y = rand() % (window.getSize().y - (int) enemy.getGlobalBounds().height) + enemy.getGlobalBounds().height / 2;
	enemy.setPosition(window.getSize().x + enemy.getGlobalBounds().width / 2, y);
	enemies.push_back(enemy);

}

void EnemiesManager::clearEnemies()
{
	//delete all the dead enemies
	for(list<Enemy>::iterator it = enemies.begin(); it != enemies.end(); it++){
		if(it->isDead())
			it = enemies.erase(it);
	}
}

void EnemiesManager::checkPlayerBullets(list<Bullet> &playerBullets){

	//test all the enemies
	for(list<Enemy>::iterator it = enemies.begin(); it != enemies.end(); it++) {
		it->checkPlayerBullets(playerBullets);
	}
}



