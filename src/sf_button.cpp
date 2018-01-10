#include "sf_button.hpp"
sf_Button::sf_Button(){
}
sf_Button::sf_Button(sf::Vector2f pos, sf::Vector2f size, sf::Color c, sf::Font *t, std::string s){
	position = pos;
	b_size = size;
	col = c;
	text_ = sf::Text(s, *t, 25);
	text_.setFillColor(sf::Color::Black);
	text_.setPosition(pos);
}
void sf_Button::setSize(sf::Vector2f size){
	b_size = size;
}
void sf_Button::setColor(sf::Color c){
	col = c;
}
void sf_Button::draw(sf::RenderWindow *wind){
	r.setSize(sf::Vector2f(b_size.x-10,b_size.y-10));
	r.setFillColor(col);
	r.setPosition(position.x+5,position.y+5);
	wind->draw(r);
	wind->draw(text_);
}
