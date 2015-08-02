#include "HitCircle.hpp"
#include <iostream>

HitCircle::HitCircle(int number, sf::RenderWindow* window)
	: window(window), number(number)
{
	fadeState = new FadeState;	
	*fadeState = FadeState::FADEDOUT;
	texture = new sf::Texture();
	texture->loadFromFile(std::to_string(number) + ".png");
	texture->setSmooth(true);
	sprite = new sf::Sprite();
	
	textureAC = new sf::Texture();
	textureAC->loadFromFile("approachCircle.png");
	textureAC->setSmooth(true);
	spriteAC = new sf::Sprite();
	spriteAC->setTexture(*textureAC);

	reload();

	textureF = new sf::Texture();
	textureF->loadFromFile("follow.png");
	textureF->setSmooth(true);
	spriteF = new sf::Sprite();
	spriteF->setTexture(*textureF);
	spriteF->setPosition(sprite->getPosition().x + sprite->getGlobalBounds().width / 2.0f,
		sprite->getPosition().y + sprite->getGlobalBounds().height / 2.0f);
	spriteF->setOrigin(spriteF->getOrigin().x, spriteF->getLocalBounds().height / 2);
	spriteF->setRotation(22 + (number-1)*45);
}

void HitCircle::reload()
{
	sprite->setTexture(*texture);
	float scale = 0.25f;
	sprite->setScale(sf::Vector2f(scale, scale));

	float distance = window->getSize().y * 0.24f;
	float degrees = (number - 1) * 45 - 90;
	float radians = degrees * 3.14159265f / 180.0f;
	float offsetY = distance * sin(radians);
	float offsetX = distance * cos(radians);
	sf::Vector2f midpoint = sf::Vector2f(window->getSize().x / 2, window->getSize().y / 2);
	sf::Vector2f position = sf::Vector2f(midpoint.x + offsetX, midpoint.y + offsetY);
	position.x -= sprite->getGlobalBounds().width / 2;
	position.y -= sprite->getGlobalBounds().height / 2;
	sprite->setPosition(position);

	float approachCircleScale = scale * 1.0f;
	spriteAC->setScale(sf::Vector2f(approachCircleScale, approachCircleScale));
	position.x = midpoint.x - spriteAC->getGlobalBounds().width / 2;
	position.y = midpoint.y - spriteAC->getGlobalBounds().height / 2;
	spriteAC->setPosition(position);

	timer.restart();
	alpha = 0;
}

void HitCircle::update()
{
	if (*fadeState == FadeState::FADINGIN)
	{
		if (timer.getElapsedTime().asMilliseconds() > 100)
		{
			*fadeState = FadeState::FADEDIN;
			sprite->setColor(sf::Color(255, 255, 255, 255));
		}
		else
		{
			alpha = (int)(timer.getElapsedTime().asMilliseconds() / 100.0f * 255.0f);
			sprite->setColor(sf::Color(255,
				                       255,
									   255,
									   alpha));
			spriteF->setColor(sf::Color(255, 255, 255, alpha));
		}
	}

	if (*fadeState == FadeState::FADINGIN || *fadeState == FadeState::FADEDIN)
	{
		if (timer.getElapsedTime().asMilliseconds() > 600.0f)
			*fadeState = FadeState::FADINGOUT;

		float scale = (600.0f - timer.getElapsedTime().asMilliseconds()) / 850.0f + 0.25f;
		spriteAC->setScale(sf::Vector2f(scale, scale));
		sf::FloatRect bounds = sprite->getGlobalBounds();
		sf::Vector2f center = sf::Vector2f(bounds.left + bounds.width / 2,
			                               bounds.top + bounds.height / 2);
		spriteAC->setPosition(center.x - spriteAC->getGlobalBounds().width / 2, 
			                  center.y - spriteAC->getGlobalBounds().height / 2);
	}

	if (*fadeState == FadeState::FADINGOUT)
	{
		if (timer.getElapsedTime().asMilliseconds() > 800.0f)
			*fadeState = FadeState::FADEDOUT;

		sf::FloatRect bounds = sprite->getGlobalBounds();
		sf::Vector2f center = sf::Vector2f(bounds.left + bounds.width / 2,
			                               bounds.top + bounds.height / 2);

		float scale = (timer.getElapsedTime().asMilliseconds() - 600.0f) / 4500.0f + 0.25f;
		sprite->setScale(scale, scale);

		sprite->setPosition(center.x - sprite->getGlobalBounds().width / 2,
			                center.y - sprite->getGlobalBounds().height / 2);

		alpha = 255 - ((timer.getElapsedTime().asMilliseconds() - 600.0f) / 200.0f) * 255;
		sprite->setColor(sf::Color(255, 255, 255, alpha));
		spriteF->setColor(sf::Color(255, 255, 255, alpha));
	}
}

void HitCircle::draw()
{
	if (*fadeState != FadeState::FADEDOUT)
	{
		if (number != 8)
			window->draw(*spriteF);
		window->draw(*sprite);
	}

	if (*fadeState == FadeState::FADINGIN || *fadeState == FadeState::FADEDIN)
		window->draw(*spriteAC);
}