#ifndef DRAWBOOK_HPP
#define DRAWBOOK_HPP
#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
#include <sstream>
#include <utility>
#include "simple_svg_1.0.0.hpp"
class sf_Button{
	public:
		sf_Button();
		sf_Button(sf::Vector2f pos,sf::Vector2f size,sf::Color col,sf::Font *t,std::string s );
		void setPosition(sf::Vector2f pos);
		void setSize(sf::Vector2f size);
		void setColor(sf::Color col);
		void draw(sf::RenderWindow *wind);
	protected:
		sf::Vector2f position;
		sf::Vector2f b_size;
		sf::Color col;
		sf::Text text_;
		sf::RectangleShape r;
};
#endif
