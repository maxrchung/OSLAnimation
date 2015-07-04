#include <SFML/Graphics.hpp>
#include <iostream>
#include "HitCircle.hpp"
#include <vector>
#include <math.h>

enum PatternState
{
	LOADING,
	COMPLETE
};

int main()
{
	sf::Vector2i screen = sf::Vector2i(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height + 2);
	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(screen.x, screen.y), "OSL", sf::Style::None);
	window->setFramerateLimit(60);

	window->setMouseCursorVisible(false);

	sf::Texture backgroundTexture;
	backgroundTexture.loadFromFile("orangeBackground.jpg", sf::IntRect(0, 0, window->getSize().x, window->getSize().y));
	backgroundTexture.setSmooth(true);
	sf::Sprite background;
	background.setTexture(backgroundTexture);

	std::vector<HitCircle*> hitCircles;

	for (int i = 0; i < 8; i++)
	{
		HitCircle* hitCircle = new HitCircle(i + 1, window);
		hitCircles.push_back(hitCircle);
	}

	PatternState patternState = PatternState::LOADING;
	sf::Clock spawnTimer;
	int toSpawn = 1;

	sf::Clock frame;

    while (window->isOpen())
    {
        sf::Event event;
		while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();
        }

		if (patternState == PatternState::LOADING)
		{
			if (spawnTimer.getElapsedTime().asMilliseconds() > 300)
			{
				hitCircles[toSpawn - 1]->reload();
				*hitCircles[toSpawn-1]->fadeState = FadeState::FADINGIN;

				spawnTimer.restart();
				toSpawn++;
				if (toSpawn > 8)
				{
					toSpawn = 1;
					patternState = PatternState::COMPLETE;
				}
			}
		}
		else if (patternState == PatternState::COMPLETE)
		{
			if (spawnTimer.getElapsedTime().asMilliseconds() > 1600)
			{
				spawnTimer.restart();
				patternState = PatternState::LOADING;
			}
		}

		for (auto hitCircle : hitCircles)
			hitCircle->update();

		window->clear();
        window->draw(background);
		for (auto hitCircle : hitCircles)
		{
			hitCircle->draw();
		}
        window->display();
    }

    return 0;
}