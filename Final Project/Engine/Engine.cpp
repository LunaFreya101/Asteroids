// Engine.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "GameObjects.h"
#include <string>
#include <vector>

bool fullscreen = false;
int main()
{
	srand(static_cast<unsigned int>(time(0)));	
	GameEngine UnrealEngine6;

	sf::RenderWindow window(sf::VideoMode(1280, 720), "Welcome to SDVA 103!");
	
	sf::Music music;
	if (!music.openFromFile("Audio/EpicMusic.ogg"))
	{
		return 1;
	}

	music.setLoop(true);
	music.play();

	UnrealEngine6.RespawnAsteroids();
	UnrealEngine6.RespawnPlayer();
	UnrealEngine6.RespawnPlayer2();

	sf::Clock clock;
	while (window.isOpen())
	{
		sf::Time dt = clock.restart();		
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Return &&
					event.key.alt)
				{
					window.close();
					fullscreen = !fullscreen;
					window.create(fullscreen ? sf::VideoMode(1920, 1080) : sf::VideoMode(1280, 720), "Welcome to SDVA 103!", fullscreen ? sf::Style::Fullscreen : sf::Style::Titlebar);
				}
			}
			if (event.type == sf::Event::Closed)
				window.close();
		}

	
		window.clear();
		
		UnrealEngine6.Update(dt.asSeconds());
		UnrealEngine6.Draw(&window);		

		window.display();
	}

	return 0;
}
