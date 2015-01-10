/*
 * Game.cpp
 *
 *  Created on: Dec 11, 2014
 *      Author: guillaume
 */

#include "Game.h"

Game::Game() : window(VideoMode(900, 500), "ASEC Shooter", Style::Close | Style::Titlebar), background(window),
				player(window), enemiesManager(window){
	window.setFramerateLimit(25);
	gameIsOver = false;
}

void Game::update(){

	//clear the window
	window.clear();

	//check if game is over
	if(player.isDead())
		gameIsOver = true;

	//display score
	if(gameIsOver) {
		displayScore(player.getScore());
	}

	//display the game
	else {
        background.update();
        player.update();
        enemiesManager.update(player);
	}

	//display
	window.display();
}

void Game::run(){

	//Main loop
	while(window.isOpen()) {

        //events
        while(window.pollEvent(event)) {

            //Exit
            if(event.type == Event::Closed)
                window.close();

            //MoveUp
            if(event.type == Event::KeyPressed && event.key.code == Keyboard::Up)
            	player.setIsMovingUp(true);
            if(event.type == Event::KeyReleased && event.key.code == Keyboard::Up)
            	player.setIsMovingUp(false);

            //MoveDown
            if(event.type == Event::KeyPressed && event.key.code == Keyboard::Down)
            	player.setIsMovingDown(true);
            if(event.type == Event::KeyReleased && event.key.code == Keyboard::Down)
            	player.setIsMovingDown(false);

            //MoveLeft
            if(event.type == Event::KeyPressed && event.key.code == Keyboard::Left)
            	player.setIsMovingLeft(true);
            if(event.type == Event::KeyReleased && event.key.code == Keyboard::Left)
            	player.setIsMovingLeft(false);

            //MoveRight
            if(event.type == Event::KeyPressed && event.key.code == Keyboard::Right)
            	player.setIsMovingRight(true);
            if(event.type == Event::KeyReleased && event.key.code == Keyboard::Right)
            	player.setIsMovingRight(false);

            //Restart
            if(gameIsOver && event.type == Event::KeyReleased && event.key.code == Keyboard::Return) {
            	gameIsOver = false;
            	player.restart();
            	enemiesManager.restart();
            }
        }

        //Update
        update();
	}
}

void Game::displayScore(const unsigned int& score) {

	//variables
	Text text;
	Font font;
	string tmpScore= static_cast<ostringstream*>( &(ostringstream() << score) )->str();
	string message = "Score : " + tmpScore + "\nPress Enter to restart.";

	//load font
	if(!font.loadFromFile("resources/Capture_it.ttf")) {
		std::cout << "Capture_it.ttf not found." << std::endl;
	}

	//Set text
	text.setFont(font);
	text.setColor(Color::White);
	text.setString(message);
	text.setCharacterSize(40);
	text.setOrigin(text.getGlobalBounds().width / 2, text.getGlobalBounds().height / 2);
	text.setPosition(window.getSize().x /2, window.getSize().y / 2);

	//draw the text
	window.draw(text);
}





