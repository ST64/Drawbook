#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
// if mousevert != vertices.at()
using namespace std;
double _x1,this_y1,_x2,_y2;
int width_rect=0, height_rect=0, _width_rect=0, _height_rect=0, screenshot=0, color=0, textmode=0, gamemode=0, drawing=0;
//enum mode_flags { GAMEMODE=1, DRAWING=2, TEXTMODE=4};
// Flags for mode of operation
char savescreen[65];
char bmpname[100];
int f=0;
string text_string;
const char charslist[]={' ','!','"','#','$','%','&','\'','(',')','*','+',',','-','.'};
sf::Image Image_screenshot;
sf::Texture texture;
sf::Sprite spritez;
sf::Font font;
sf::Text text;
const char * buttonfiles[9] = {"Data/new.png","Data/up.png","Data/down.png","Data/Color.png","Data/Save.png","Data/Load.png","Data/text.png","Data/text2.png","\0"};
const int x_rects[9]={0,0,100,0,0,0,0,0,0};
const int y_rects[9]={0,50,50,150,200,250,300,350,350};
const int w_rects[9]={200,75,75,200,200,200,200,200,0};
const int h_rects[9]={50,100,100,50,50,50,50,50,0};
const sf::Color rgb[]={sf::Color::Black,sf::Color::Red,sf::Color::Yellow,sf::Color::Green,sf::Color::Blue};
/* black, red, yellow, green, blue*/
int main(){
	sf::RenderWindow window(sf::VideoMode(800,600),"Drawbook",sf::Style::Default);
	sf::RenderWindow sideWindow(sf::VideoMode(200,400),"Tools",sf::Style::Close|sf::Style::Titlebar);
	sf::Event event;
	window.clear(sf::Color::White);
	sideWindow.clear(sf::Color::White);
	vector<sf::VertexArray> drawable_verts;
	drawable_verts.push_back(sf::VertexArray());
	sf::Vector2i mousevect;
	sf::Vector2u windowvect;
	this_y1=0.0f;
	_x1=0.0f;
	_x2=0.0f;
	_y2=0.0f;
	sf::Texture buttonsurfs;
	sf::Sprite sprites;
	sideWindow.display();
	sideWindow.clear(sf::Color::White);
	buttonsurfs.loadFromFile("Data/sidewindow.png");
	sprites.setTexture(buttonsurfs);
	sprites.setTextureRect(sf::IntRect(0,0,200,400));
	sprites.setPosition(0,0);
	sideWindow.draw(sprites);
	sideWindow.display();
	window.setPosition(sf::Vector2i(200,0));
	sideWindow.setPosition(sf::Vector2i(0,0));
	font.loadFromFile("Data/Drawbook.ttf");
	text = sf::Text("Welcome to Drawbook.\nPress the Up or Down button to change line size.\nDraw with the mouse.\nPress ESC to Continue",font,25);
	text.setFillColor(sf::Color::Black);
	window.draw(text);
	window.setFramerateLimit(60);
	while (window.isOpen()&&sideWindow.isOpen()){
		sideWindow.clear(sf::Color::White);
		window.clear(sf::Color::White);
		sideWindow.draw(sprites);
		if (gamemode==1){
			spritez.setTexture(texture);
			window.draw(spritez);
		}else if (gamemode==0){
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
					drawable_verts.push_back(sf::VertexArray());
					f=0;
					gamemode = 1;
				}else if ((mousevect.y > 50)&&(mousevect.y < 150)&&(mousevect.x < 100)){
					screenshot++;
				}else if ((mousevect.y > 50)&&(mousevect.y < 150)&&(mousevect.x > 100)){
					screenshot--;
				}else if ((mousevect.y > 150)&&(mousevect.y < 200)){
					color++;
					if (color > 5){color=0;}
				}else if ((mousevect.y > 200)&&(mousevect.y < 250)){
					screenshot++;
					sf::Vector2u windowSize = window.getSize();
					texture.create(windowSize.x, windowSize.y);
					texture.update(window);
					Image_screenshot = texture.copyToImage();
					sprintf(savescreen,"Screenshot%d",screenshot);
					strcat(bmpname,savescreen);
					strcat(bmpname,".png");
					Image_screenshot.saveToFile(bmpname);
					memset(bmpname,0,99);
				}else if ((mousevect.y > 250)&&(mousevect.y < 300)){
					sprintf(savescreen,"Screenshot%d",screenshot);
					strcat(bmpname,savescreen);
					strcat(bmpname,".png");
					Image_screenshot.loadFromFile(bmpname);
					texture.loadFromImage(Image_screenshot);
					spritez.setTexture(texture);
					window.draw(spritez);
					memset(bmpname,0,99);
				}else if ((mousevect.y > 300)&&(mousevect.y < 350)){
					text = sf::Text("Welcome to Drawbook.\nPress the Up or Down button to change line size.\nDraw with the mouse and press ESC to reset\nMore Features COMING SOON...",font,25);
					text.setFillColor(sf::Color::Black);
					window.draw(text);
				}else if ((mousevect.y > 350)){
					if (textmode==0){
						textmode=1;
						sf::Vector2u windowSize = window.getSize();
						texture.create(windowSize.x, windowSize.y);
						texture.update(window);
						Image_screenshot = texture.copyToImage();
						Image_screenshot.saveToFile("Data/saves.png");
					}else{
						textmode=0;
						sf::Vector2u windowSize = window.getSize();
						texture.create(windowSize.x, windowSize.y);
						texture.update(window);
						Image_screenshot = texture.copyToImage();
						Image_screenshot.saveToFile("Data/saves.png");
					}
				}
			}
		}
		while (window.pollEvent(event)){
			if (event.type == sf::Event::Closed){
				window.close();
				sideWindow.close();
			}else if (event.type == sf::Event::MouseButtonPressed){
				drawing=1;
				mousevect = sf::Mouse::getPosition(window);
				windowvect= window.getSize();
				if ((mousevect.x < 0)||(mousevect.y < 0)){
					drawing=0;
				}else if ((mousevect.x > windowvect.x)||(mousevect.y > windowvect.y)){
					drawing=0;
				}else{
					drawable_verts[f].append(sf::Vertex(sf::Vector2f(mousevect),rgb[color]));
					drawable_verts[f].append(sf::Vertex(sf::Vector2f(mousevect),rgb[color]));
				}
			}else if (event.type == sf::Event::MouseButtonReleased){
				drawing=0;
				drawable_verts.push_back(sf::VertexArray(sf::LineStrip));
				f++;
				_x1 = 0.0;
				this_y1 = 0.0;
			}else if (event.type == sf::Event::MouseMoved){
				_x2=_x1;
				_y2=this_y1;
				_x1=event.mouseMove.x;
				this_y1=event.mouseMove.y;
				if (drawing==1){
					if ((_x2 != _x1) || (_y2 != this_y1)){
						drawable_verts[f].append(sf::Vertex(sf::Vector2f(_x1,this_y1),rgb[color]));
					}
				}
			}else if (event.type==sf::Event::KeyPressed){
				if (event.key.code==sf::Keyboard::Escape){
					window.clear(sf::Color::White);
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
						char rs = static_cast<char>(event.text.unicode);
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
			for (int i=1;i<drawable_verts.size();i++){
				if (drawable_verts[i].getVertexCount()>1){
					window.draw(drawable_verts[i]);
				}
			}
		}
		window.display();
		sideWindow.display();
	}
	return 0;
}
