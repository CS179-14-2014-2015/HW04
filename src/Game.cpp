/*
 * Game.cpp
 *
 *  Created on: Dec 11, 2014
 *      Author: guillaume
 */

#include "Game.h"

Game::Game() : window(VideoMode(900, 500), "ASEC Shooter", Style::Close | Style::Titlebar), background(window) {
	window.setVerticalSyncEnabled(true);
}

void Game::update(){
	//clear the window
	window.clear();

	//update the drawables
	background.update();

	//display
	window.display();
}

void Game::run(){

	//Main loop
	while(window.isOpen()) {

        //Wait for events
        while(window.pollEvent(event)) {

            //Exit
            if(event.type == Event::Closed)
                window.close();
        }

        //Event handler

        //Update
        update();
	}
}




