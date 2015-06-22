#ifndef HITCIRCLE_HPP
#define HITCIRCLE_HPP

#include "SFML/Graphics.hpp"

enum FadeState
{
	FADINGIN,
	FADEDIN,
	FADINGOUT,
	FADEDOUT
};

class HitCircle
{
public:
	HitCircle(int number, sf::RenderWindow* window);
	sf::Texture* texture;
	sf::Texture* textureAC;
	sf::Texture* textureF;
	sf::Sprite* sprite;
	sf::Sprite* spriteAC;
	sf::Sprite* spriteF;
	FadeState* fadeState;
	sf::RenderWindow* window;
	int alpha = 0;
	int number;
	sf::Clock timer;
	void reload();
	void update();
	void draw();
};

#endif//HITCIRCLE_HPP