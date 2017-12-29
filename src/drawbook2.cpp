#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
#include <utility>
double _x1,this_y1,_x2,_y2;
int width_rect=0, height_rect=0, _width_rect=0, _height_rect=0, screenshot=0, color=0, textmode=0, gamemode=0, drawing=0;
char savescreen[65];
char bmpname[100];
int f=0;
std::string text_string;
const char charslist[]={' ','!','"','#','$','%','&','\'','(',')','*','+',',','-','.'};
const sf::Color rgb[]={sf::Color::Black,sf::Color::Red,sf::Color::Yellow,sf::Color::Green,sf::Color::Blue};
const std::string buttonstrs[] = {"New File","Next Color","Save File","Load File","Help","Text Mode"};
/* black, red, yellow, green, blue*/
int main(){
	sf::RenderWindow window(sf::VideoMode(800,600),"Drawbook",sf::Style::Default);
	sf::RenderWindow sideWindow(sf::VideoMode(200,300),"Tools",sf::Style::Close|sf::Style::Titlebar);
	sf::Event event = {};
	window.clear(sf::Color::White);
	sideWindow.clear(sf::Color::White);
	std::vector<std::pair<std::vector<sf::Vector2i>,sf::Color>> drawable_verts;
	drawable_verts.emplace_back();
	std::vector<sf::Vertex> tmpverts;
	sf::Vector2i mousevect;
	sf::Vector2u windowvect = window.getSize();
	this_y1=0.0f;
	_x1=0.0f;
	_x2=0.0f;
	_y2=0.0f;
	sf::Texture texture;
	sf::Sprite spritez;
	sf::Image Image_screenshot;
	sf::Font font;
	font.loadFromFile("Data/Drawbook.ttf");
	sf::Text text;
	sf::Text buttontexts[6];
	sf::RectangleShape buttonrects[6];
	for (int i=0;i<6;i++){
		buttontexts[i] = sf::Text(buttonstrs[i],font,25);
		buttontexts[i].setPosition(10,5+(50*i));
		buttontexts[i].setFillColor(sf::Color::Black);
		buttonrects[i].setSize(sf::Vector2f(190,40));
		buttonrects[i].setPosition(5,5+(i*50));
		buttonrects[i].setFillColor(sf::Color(0xff,0x63,0));
	}
	sideWindow.display();
	sideWindow.clear(sf::Color::White);
	sideWindow.display();
	window.setPosition(sf::Vector2i(200,0));
	sideWindow.setPosition(sf::Vector2i(0,0));
	text = sf::Text("Welcome to Drawbook.\nPress the Up or Down button to change line size.\nDraw with the mouse.\nPress ESC to Continue",font,25);
	text.setFillColor(sf::Color::Black);
	window.draw(text);
	window.setFramerateLimit(60);
	texture.create(windowvect.x, windowvect.y);
	window.clear(sf::Color::White);
	texture.update(window);
	sf::Sprite TexSprite;
	TexSprite.setTexture(texture);
	while (window.isOpen()&&sideWindow.isOpen()){
		sideWindow.clear(sf::Color::Black);
		for (int i=0;i<6;i++){
			sideWindow.draw(buttonrects[i]);
			sideWindow.draw(buttontexts[i]);
		}
		window.clear(sf::Color::White);
		window.draw(TexSprite);
		if (gamemode==0){
			window.draw(text);
		}
		while (sideWindow.pollEvent(event)){
			if (event.type == sf::Event::Closed){
				sideWindow.close();
				window.close();
			}
			else if (event.type == sf::Event::MouseButtonPressed){
				mousevect = sf::Mouse::getPosition(sideWindow);
				if (mousevect.y < 50){
					window.clear(sf::Color::White);
					drawable_verts.clear();
					drawable_verts.shrink_to_fit();
					drawable_verts.emplace_back();
					f=0;
					gamemode = 1;
				}else if (mousevect.y < 100){
					color++;
					if (color > 5){color=0;}
				}else if (mousevect.y < 150){
					screenshot++;
					texture.update(window);
					Image_screenshot = texture.copyToImage();
					sprintf(bmpname,"Screenshot%d.png",screenshot);
					Image_screenshot.saveToFile(bmpname);
					memset(bmpname,0,99);
				}else if (mousevect.y < 200){
					sprintf(bmpname,"Screenshot%d.png",screenshot);
					texture.loadFromFile(bmpname);
					memset(bmpname,0,99);
				}else if (mousevect.y < 250){
					gamemode = 0;
				}else{
					textmode = textmode ^ 0x01;
					window.clear(sf::Color::White);
				}
			}
		}
		while (window.pollEvent(event)){
			if (event.type == sf::Event::Closed){
				window.close();
				sideWindow.close();
			}else if (event.type == sf::Event::MouseButtonPressed){
				mousevect = sf::Mouse::getPosition(window);
				windowvect= window.getSize();
				drawing=1 & static_cast<int>(mousevect.x > 0) & static_cast<int>(mousevect.y > 0) & static_cast<int>(mousevect.x < windowvect.x) & static_cast<int>(mousevect.y < windowvect.y);
				if (drawing == 1){
					drawable_verts[f].first.emplace_back(mousevect);
					drawable_verts[f].first.emplace_back(mousevect);
				}
				drawable_verts[f].second = rgb[color];
			}else if (event.type == sf::Event::MouseButtonReleased){
				drawing=0;
				mousevect = sf::Mouse::getPosition(window);
				drawable_verts[f].first.emplace_back(mousevect);
				drawable_verts[f].first.shrink_to_fit();
				drawable_verts[f].second = rgb[color];
				drawable_verts.emplace_back();
				f++;
				_x1 = 0.0;
				this_y1 = 0.0;
			}else if (event.type == sf::Event::MouseMoved){
				_x2=_x1;
				_y2=this_y1;
				_x1=event.mouseMove.x;
				this_y1=event.mouseMove.y;
				if (((_x2 != _x1) || (_y2 != this_y1))&&((drawing==1)&&(_x1 > 0)&&(this_y1 > 0)&&(_x1 < windowvect.x)&&(this_y1 < windowvect.y))){
					drawable_verts[f].first.emplace_back(sf::Vector2i(_x1,this_y1));
				}
				drawable_verts[f].second = rgb[color];
			}else if (event.type==sf::Event::KeyPressed){
				if (event.key.code==sf::Keyboard::Escape){
					window.clear(sf::Color::White);
					drawable_verts.clear();
					drawable_verts.emplace_back();
					f=0;
					gamemode = 1;
				}else if (event.key.code==sf::Keyboard::BackSpace){
					if (textmode==1){
						Image_screenshot.loadFromFile("Data/saves.png");
						texture.loadFromImage(Image_screenshot);
						spritez.setTexture(texture);
						window.draw(spritez);
						text = sf::Text(text_string,font,25);
						text.setFillColor(sf::Color::Black);
						window.draw(text);
					}
				}
			}else if (event.type==sf::Event::TextEntered){
				if (textmode==1){
					if ((event.text.unicode == 10)||(event.text.unicode == 13)){
						char rs = '\n';
						text_string.push_back(rs);
						text=sf::Text(text_string,font,25);
						text.setFillColor(sf::Color::Black);
						window.draw(text);
					}else if ((event.text.unicode < 128)&&(event.text.unicode > 31)){
						auto rs = static_cast<char>(event.text.unicode);
						if (event.text.unicode < 48){
							rs = charslist[event.text.unicode-32];
						}
						text_string.push_back(rs);
						text=sf::Text(text_string,font,25);
						text.setFillColor(sf::Color::Black);
						window.draw(text);
					}
				}
			}
		}
		if (drawable_verts.size()>1){
			for (uint64_t i=1;i<drawable_verts.size();i++){
				if (drawable_verts[i].first.size()>1){
					tmpverts.reserve(drawable_verts[i].first.size());
					for (uint32_t j=1;j<drawable_verts[i].first.size();j++){
						tmpverts.emplace_back(sf::Vertex(sf::Vector2f(drawable_verts[i].first[j].x,drawable_verts[i].first[j].y),drawable_verts[i].second));
					}
					window.draw(tmpverts.data(),tmpverts.size(),sf::LineStrip);
					std::vector<sf::Vertex>().swap(tmpverts);
				}
			}
		}
		window.display();
		sideWindow.display();
	}
	return 0;
}
