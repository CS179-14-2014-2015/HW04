/*
 * EnemiesManager.cpp
 *
 *  Created on: Dec 15, 2014
 *      Author: guillaume
 */

#include "EnemiesManager.h"

EnemiesManager::EnemiesManager(RenderWindow &window) : window(window){

	//Initialize the sound
	buffer.loadFromFile("resources/boum.wav");
	sound.setBuffer(buffer);

	//load the textures, alert if failed
	if(!enemyTexture.loadFromFile("resources/enemy.png")) {
		cout<< "Impossible to load enemy.png" <<endl;
		exit(EXIT_FAILURE);
	}
	if(!bulletTexture.loadFromFile("resources/enemyBullet.png")) {
		cout<< "Impossible to load enemyBullet.png" <<endl;
		exit(EXIT_FAILURE);
	}

	//initialize counters
	frameCounter = 0;
	srand (time(NULL));
}


void EnemiesManager::fire(Vector2f const &player) {

	//make all the enemies fire and add it to the list
	for(list<Enemy>::iterator it = enemies.begin(); it != enemies.end(); it++)
		bullets.push_back(it->fire(player, bulletTexture));
}

void EnemiesManager::moveBullets() {

	//move all the bullets and draw them

	for(list<Bullet>::iterator it = bullets.begin(); it != bullets.end(); it++) {
		it->move(it->getXspeed(), it->getYspeed());
		it->rotate(it->getSpin());
		window.draw(*it);
	}
}

void EnemiesManager::clearBullets() {

	for(list<Bullet>::iterator it = bullets.begin(); it != bullets.end(); it++){
		if(Tools::isLeft(*it))
			it = bullets.erase(it);
	}
}

void EnemiesManager::moveEnemies() {

	//move all the enemies
	for(list<Enemy>::iterator it = enemies.begin(); it != enemies.end(); it++){
		it->move(it->getXspeed(), it->getYspeed());
		window.draw(*it);
	}
}

void EnemiesManager::checkBullet(Player& player) {

	//check the enemies bullets collisions with the player
	for(list<Bullet>::iterator it = bullets.begin(); it != bullets.end(); it++) {
		if(it->getGlobalBounds().intersects(player.getGlobalBounds())) {
			it = bullets.erase(it);
			player.getHit();
			if(player.isDead())
				sound.play();
		}
	}
}

void EnemiesManager::checkEnemies(Player& player) {

	//check the enemies collision with the player
	for(list<Enemy>::iterator it = enemies.begin(); it != enemies.end(); it++) {
		if(it->getGlobalBounds().intersects(player.getGlobalBounds())) {
			player.getHit();
			it = enemies.erase(it);
		}
	}
}

void EnemiesManager::update(Player &player){
	frameCounter++;

	//check  collisions
	checkPlayerBullets(player);
	checkBullet(player);
	checkEnemies(player);

	//clear dead enemies
	clearEnemies();

	//add enemy
	if(frameCounter % 20 == 0) {
		addEnemy();
	}

	//move enemies
	moveEnemies();

	//make enemy fire
	clearBullets();
	if(frameCounter % 50 == 0)
		fire(player.getPosition());
	moveBullets();
}

void EnemiesManager::addEnemy(){

	//Create sprite, place the enemy, modify origin and add it to the list
	Enemy enemy(enemyTexture);
	enemy.setOrigin(enemy.getGlobalBounds().width / 2, enemy.getGlobalBounds().height / 2);
	int y = rand() % (window.getSize().y - (int) enemy.getGlobalBounds().height) + enemy.getGlobalBounds().height / 2;
	enemy.setPosition(window.getSize().x + enemy.getGlobalBounds().width / 2, y);
	enemies.push_back(enemy);

}

void EnemiesManager::clearEnemies(){

	//delete all the dead enemies
	for(list<Enemy>::iterator it = enemies.begin(); it != enemies.end(); it++){
		if(it->isDead()) {
			it = enemies.erase(it);
			sound.play();
		}
	}
}

void EnemiesManager::checkPlayerBullets(Player &player){

	//test all the enemies
	for(list<Enemy>::iterator it = enemies.begin(); it != enemies.end(); it++) {
		it->checkPlayerBullets(player.getBullets());

		//handle enemies killed
		if(it->isDead()) {
			player.increaseScore(1);
			it = enemies.erase(it);
			sound.play();
		}
	}
}

void EnemiesManager::restart() {
	enemies.clear();
	bullets.clear();
}




