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
using namespace svg;
double _x1,this_y1,_x2,_y2;
int width_rect=0, height_rect=0, _width_rect=0, _height_rect=0, screenshot=0, color=0, textmode=0, drawing=0;
char bmpname[16];
int f=0;
sf::Font font;
std::string text_string;
const char charslist[]={' ','!','"','#','$','%','&','\'','(',')','*','+',',','-','.'};
const sf::Color rgb[]={sf::Color::Black,sf::Color::Red,sf::Color::Yellow,sf::Color::Green,sf::Color::Blue};
const std::string buttonstrs[] = {"New File","Next Color","Save File","Load File","Help","Text Mode"};
/* black, red, yellow, green, blue*/
std::string trim(std::string const& str){
    if(str.empty())
        return str;

    std::size_t firstScan = str.find_first_not_of('\t');
    std::size_t first     = firstScan == std::string::npos ? str.length() : firstScan;
    std::size_t last      = str.find_last_not_of(' ');
    return str.substr(first, last-first+1);
}
std::string dialog(std::string dialogtitle,std::string dialogdata){
	sf::RenderWindow dialogWindow(sf::VideoMode(800,100),dialogtitle,sf::Style::Close|sf::Style::Titlebar);
	dialogdata += '\n';
	sf::Text dialogtext(dialogdata,font,25);
	dialogtext.setPosition(0,0);
	dialogWindow.setPosition(sf::Vector2i(200,0));
	sf::Event dialogEvent;
	std::string retstr;
	sf::Clock dialogTimer;
	dialogtext.setFillColor(sf::Color::Black);
	while (dialogWindow.isOpen()){
		dialogtext.setString(dialogdata + retstr);
		if (dialogTimer.getElapsedTime() >= sf::seconds(0.5f)){
			if (std::string(dialogtext.getString()).back() == '_'){
				dialogtext.setString(dialogdata+retstr);
			}else{
				dialogtext.setString(dialogdata+retstr+'_');
			}
			dialogTimer.restart();
		}
		dialogWindow.clear(sf::Color::White);
		dialogWindow.draw(dialogtext);
		while (dialogWindow.pollEvent(dialogEvent)){
			dialogWindow.draw(dialogtext);
			if (dialogEvent.type==sf::Event::Closed){
				dialogWindow.close();
				return retstr;
			}else if (dialogEvent.type==sf::Event::TextEntered){
				if (dialogEvent.text.unicode < 128){
					retstr += static_cast<char>(dialogEvent.text.unicode);
					dialogtext.setString(dialogdata+retstr);
				}
			}else if (dialogEvent.type==sf::Event::KeyPressed){
				if (dialogEvent.key.code == sf::Keyboard::Return){
					dialogWindow.close();
					return retstr;
				}
			}
		}
		dialogWindow.display();
	}
	return retstr;
}
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
	font.loadFromFile("Data/Drawbook.ttf");
	sf::Text text;
	sf::Text buttontexts[6];
	sf::RectangleShape buttonrects[6];
	sf::Clock timer;
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
	window.clear(sf::Color::White);
	while (window.isOpen()&&sideWindow.isOpen()){
		if ((timer.getElapsedTime() >= sf::seconds(0.5f))&&(textmode==1)){
			if (std::string(text.getString()).back()=='_'){
				text.setString(text_string);
			}else{
				text.setString(text_string+'_');
			}
			timer.restart();
		}
		sideWindow.clear(sf::Color::Black);
		for (int i=0;i<6;i++){
			sideWindow.draw(buttonrects[i]);
			sideWindow.draw(buttontexts[i]);
		}
		window.clear(sf::Color::White);
		window.draw(text);
		if (drawable_verts.size()>0){
			for (uint64_t i=0;i<drawable_verts.size();i++){
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
					text.setString("");
					f=0;
				}else if (mousevect.y < 100){
					color++;
					if (color > 5){color=0;}
				}else if (mousevect.y < 150){
					screenshot++;
					windowvect=window.getSize();
					Dimensions dimensions(windowvect.x,windowvect.y);
					std::string svgname = dialog("Save File","Choose the filename:");
					Document doc(svgname.c_str(),Layout(dimensions,Layout::TopLeft));
					int64_t o = static_cast<int64_t>(drawable_verts.size());
					for (int64_t i=0;i<o;i++){
						int32_t k = static_cast<int32_t>(drawable_verts[i].first.size());
						Polyline polyline_a(Stroke(1,Color(drawable_verts[i].second.r,drawable_verts[i].second.g,drawable_verts[i].second.b)));
						for (int32_t j=0;j<k;j++){
							polyline_a << Point(drawable_verts[i].first[j].x,drawable_verts[i].first[j].y);
						}
						if (drawable_verts[i].first.size() != 0){
							doc << polyline_a;
						}
					}
					std::stringstream iss(text_string);
					int32_t ind = 25;
					while (iss.good()){
						std::string SingleLine;
						getline(iss,SingleLine,'\n');
						doc << Text(Point(0,ind),SingleLine,Color::Black,Font(25,"Verdana"));
						ind += 40;
					}
					doc.save();
					memset(bmpname,0,15);
				}else if (mousevect.y < 200){
					std::string svgname = dialog("Load File","Load what file?");
					std::ifstream ifs(svgname.c_str());
					text_string = "";
					drawable_verts.clear();
					drawable_verts.shrink_to_fit();
					drawable_verts.emplace_back();
					f=0;
					while (ifs.good()){
						std::string SingleLine;
						getline(ifs,SingleLine,'\n');
						if (SingleLine[2] == 't'){
							SingleLine = trim(SingleLine);
							SingleLine.erase(SingleLine.length()-7);
							SingleLine.erase(0,SingleLine.rfind('>')+1);
							text_string = text_string + SingleLine + "\n";
							text.setString(text_string);
							text.setPosition(0,0);
						}else if (SingleLine[2] == 'p'){
							SingleLine.erase(0,SingleLine.find("points=\""));
							std::stringstream SL(SingleLine);
							while (SL.good()){
								std::string SinglePoint;
								SL >> SinglePoint;
								sf::Vector2i tmp;
								sscanf(SinglePoint.c_str(),"%i,%i ",&tmp.x,&tmp.y);
								if ((tmp.x!=0)||(tmp.y!=0)){
									drawable_verts[f].first.emplace_back(tmp);
								}
							}
							SingleLine.erase(0,SingleLine.find("stroke=\"rgb("));
							int r=0,g=0,b=0;
							sscanf(SingleLine.c_str(),"%i,%i,%i",&r,&g,&b);
							drawable_verts[f].second = sf::Color(r,g,b);
							f++;
							drawable_verts.emplace_back();
						}
					}
					memset(bmpname,0,15);
				}else if (mousevect.y < 250){
					text.setString("Welcome to Drawbook.\nPress the Up or Down button to change line size.\nDraw with the mouse.\nPress ESC to Continue. \nIf you appear to have lost text,\npress the text mode button again.");
				}else{
					textmode = textmode ^ 0x01;
					if (textmode == 0){
						text.setString("");
					}
					window.clear(sf::Color::White);
				}
			}
		}
		while (window.pollEvent(event)){
			mousevect = sf::Mouse::getPosition(window);
			if (event.type == sf::Event::Closed){
				window.close();
				sideWindow.close();
			}else if (event.type == sf::Event::MouseButtonPressed){
				windowvect= window.getSize();
				drawing=1 & (static_cast<int>(mousevect.x) > 0) & (static_cast<int>(mousevect.y) > 0) & (mousevect.x < static_cast<int>(windowvect.x)) & (mousevect.y < static_cast<int>(windowvect.y));
				if (drawing == 1){
					drawable_verts[f].first.emplace_back(mousevect);
					drawable_verts[f].first.emplace_back(mousevect);
				}
				drawable_verts[f].second = rgb[color];
			}else if (event.type == sf::Event::MouseButtonReleased){
				drawing=0;
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
					text_string = "";
					text.setString(text_string);
					f=0;
				}else if (event.key.code==sf::Keyboard::BackSpace){
					if (textmode==1){
						window.clear(sf::Color::White);
						text_string.pop_back();
						text.setString(text_string);
						window.draw(text);
					}
				}
			}else if (event.type==sf::Event::TextEntered){
				if (textmode==1){
					if ((event.text.unicode == 10)||(event.text.unicode == 13)){
						char rs = '\n';
						if (text_string.back()=='_'){
							text_string.pop_back();
						}
						text_string.push_back(rs);
						text.setString(text_string);
					}else if ((event.text.unicode < 128)&&(event.text.unicode > 31)){
						char rs = static_cast<char>(event.text.unicode);
						if (event.text.unicode < 48){
							rs = charslist[event.text.unicode-32];
						}
						text_string.push_back(rs);
						text.setString(text_string);
					}
				}
			}
		}
		window.display();
		sideWindow.display();
	}
	return 0;
}
