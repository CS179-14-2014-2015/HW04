/*
 * Game.cpp
 *
 *  Created on: Dec 11, 2014
 *      Author: guillaume
 */

#include "Game.h"

Game::Game() : window(VideoMode(900, 500), "ASEC Shooter", Style::Close | Style::Titlebar),background(window), player(window){
	window.setVerticalSyncEnabled(true);
}

void Game::update(){

	//Check enough time elapsed to have 25fps
	if(clock.getElapsedTime().asMilliseconds() < 40)
		return;
	clock.restart();

	//clear the window
	window.clear();

	//update the drawables
	background.update();
	player.update();

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

        }


        //Update
        update();
	}
}




