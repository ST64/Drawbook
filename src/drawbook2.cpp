#include "sf_button.hpp"
#include <cmath>
using namespace svg;
double _x1,this_y1,_x2,_y2;
int width_rect=0, height_rect=0, _width_rect=0, _height_rect=0, screenshot=0, color=0, textmode=0, drawing=0;
int f=0;
sf::Font font;
std::string text_string;
const char charslist[]={' ','!','"','#','$','%','&','\'','(',')','*','+',',','-','.'};
const std::string buttonstrs[] = {"New File","Pick Color","Save File","Load File","Help","Text Mode"};
sf::Clock global_timer;
std::string tmp_st;
sf::Color rgb(0,0,0);
void color_wheel(sf::RenderWindow *rwind,sf::RenderWindow *swind, sf::Font *fo){
	rwind->clear(sf::Color::White);
	sf::VertexArray colorwheel(sf::TriangleFan, 256);
	sf::Vector2u rw = rwind->getSize();
	int k = rw.x;
	if (rw.y < k){
		k = rw.y;
	}
	float k2 = (static_cast<float>(k)) * 0.4f;
	rw.x = rw.x >> 1;
	rw.y = rw.y >> 1;
	for (int i=0;i<256;i++){
		float vh = static_cast<float>(i) * 0.023438f;
		float vi = floor(vh);
		float v3 = vh - vi;
		sf::Color c;
		if (vi == 0){
			c.r = 255;
			c.g = 255.0f*v3;
			c.b = 0;
		}else if (vi == 1){
			c.r = 255.0f*(1.0f - v3);
			c.g = 255;
			c.b = 0;
		}else if (vi == 2){
			c.r = 0;
			c.g = 255;
			c.b = 255.0f*v3;
		}else if (vi == 3){
			c.r = 0;
			c.g = 255.0f*(1.0f - v3);
			c.b = 255;
		}else if (vi == 4){
			c.r = 255.0f*v3;
			c.g = 0;
			c.b = 255;
		}else{
			c.r = 255;
			c.g = 0;
			c.b = 255.0f*(1.0f - v3);
		}
		colorwheel[i].position = sf::Vector2f(rw.x + (k2 * cos(i*0.0245437f)),rw.y + (k2*sin(i*0.0245437f)));
		colorwheel[i].color = c;
	}
	rwind->draw(colorwheel);
	rwind->display();
	sf::Event sf_ev;
	sf::RectangleShape s(sf::Vector2f(rw.x >> 1,15));
	s.setPosition(((rw.x >> 1) + (rw.x >> 2)),static_cast<float>(rw.y)*0.15f);
	s.setFillColor(rgb);
	sf::RectangleShape s2(sf::Vector2f(rw.x >> 1,50));
	s2.setPosition(((rw.x >> 1) + (rw.x >> 2)), 0.0f);
	s2.setFillColor(sf::Color::Black);
	sf_Button sf_bt(sf::Vector2f(((rw.x >> 1 ) + (rw.x >> 2)), 0.0f),sf::Vector2f(rw.x >> 1,50),sf::Color(0xff,0x63,0),fo,"Save Color");
	bool buttonflag = true;
	double ang;
	while (buttonflag){
		rwind->clear(sf::Color::White);
		rwind->draw(s);
		rwind->draw(s2);
		rwind->draw(colorwheel);
		sf_bt.draw(rwind);
		while (rwind->pollEvent(sf_ev)){
			if (sf_ev.type == sf::Event::Closed){
				rwind->close();
				swind->close();
				exit(0);
			}else if (sf_ev.type == sf::Event::MouseButtonPressed){
				sf::Vector2i mousev = sf::Mouse::getPosition(*rwind);
				mousev.x = mousev.x - rw.x;
				mousev.y = mousev.y - rw.y;
				if ((mousev.x < static_cast<int>(rw.x >> 1)) && (mousev.x > (-static_cast<int>(rw.x >> 1))) && (mousev.y < (static_cast<float>(rw.y) * -0.75f))){
					buttonflag = false;
					rgb = colorwheel[ang].color;
				}
				ang = atan2(mousev.y, mousev.x);
				if (ang < 0){
					ang = 2*3.14159265358 + ang;
				}
				ang = floor(ang * 40.743665);
				s.setFillColor(colorwheel[ang].color);
			}
		}
		rwind->display();
	}
}
void blink_cursor(double interval, sf::Text *_text){
	if (global_timer.getElapsedTime() >= sf::seconds(interval)){
		tmp_st = std::string(_text->getString());
		if (tmp_st.back() == '_'){
			tmp_st.pop_back();
			_text->setString(tmp_st);
		}else{
			_text->setString(tmp_st + '_');
		}
		global_timer.restart();
	}
}
std::string dialog(std::string dialogtitle,std::string dialogdata){
	sf::RenderWindow dialogWindow(sf::VideoMode(800,100),dialogtitle,sf::Style::Close|sf::Style::Titlebar);
	dialogdata += '\n';
	sf::Text dialogtext(dialogdata,font,25);
	dialogtext.setPosition(0,0);
	dialogWindow.setPosition(sf::Vector2i(200,0));
	sf::Event dialogEvent;
	std::string retstr;
	dialogtext.setFillColor(sf::Color::Black);
	while (dialogWindow.isOpen()){
		blink_cursor(0.5f,&dialogtext);
		dialogWindow.clear(sf::Color::White);
		dialogWindow.draw(dialogtext);
		while (dialogWindow.pollEvent(dialogEvent)){
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
	sf::VideoMode v = sf::VideoMode::getDesktopMode();
	v.width = v.width - 200;
	sf::RenderWindow window(v,"Drawbook",sf::Style::Default);
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
	sf_Button buttons[6];
	sf::Clock timer;
	for (int i=0;i<6;i++){
		buttons[i] = sf_Button(sf::Vector2f(0,i*50),sf::Vector2f(200,50),sf::Color(0xff,0x63,0),&font,buttonstrs[i]);
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
		if (textmode==1){blink_cursor(0.5f,&text);}
		sideWindow.clear(sf::Color::Black);
		for (int i=0;i<6;i++){
			buttons[i].draw(&sideWindow);
		}
		window.clear(sf::Color::White);
		window.draw(text);
		if (drawable_verts.size()>0){
			for (int64_t i=0;i<static_cast<int64_t>(drawable_verts.size());i++){
				if (drawable_verts[i].first.size()>1){
					tmpverts.reserve(drawable_verts[i].first.size());
					for (int32_t j=1;j<static_cast<int32_t>(drawable_verts[i].first.size());j++){
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
					color_wheel(&window,&sideWindow,&font);
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
							if (SingleLine[0]=='\t'){
								SingleLine.erase(0,1);
							}
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
			switch (event.type){
			case sf::Event::Closed:
				window.close();
				sideWindow.close();
				break;
			case sf::Event::MouseButtonPressed:
				windowvect= window.getSize();
				drawing=1 & (static_cast<int>(mousevect.x) > 0) & (static_cast<int>(mousevect.y) > 0) & (mousevect.x < static_cast<int>(windowvect.x)) & (mousevect.y < static_cast<int>(windowvect.y));
				if (drawing == 1){
					drawable_verts[f].first.emplace_back(mousevect);
					drawable_verts[f].first.emplace_back(mousevect);
				}
				drawable_verts[f].second = rgb;
				break;
			case sf::Event::MouseButtonReleased:
				drawing=0;
				drawable_verts[f].first.emplace_back(mousevect);
				drawable_verts[f].first.shrink_to_fit();
				drawable_verts[f].second = rgb;
				drawable_verts.emplace_back();
				f++;
				_x1 = 0.0;
				this_y1 = 0.0;
				break;
			case sf::Event::MouseMoved:
				_x2=_x1;
				_y2=this_y1;
				_x1=event.mouseMove.x;
				this_y1=event.mouseMove.y;
				if (((_x2 != _x1) || (_y2 != this_y1))&&((drawing==1)&&(_x1 > 0)&&(this_y1 > 0)&&(_x1 < windowvect.x)&&(this_y1 < windowvect.y))){
					drawable_verts[f].first.emplace_back(sf::Vector2i(_x1,this_y1));
				}
				drawable_verts[f].second = rgb;
				break;
			case sf::Event::KeyPressed:
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
				break;
			case sf::Event::TextEntered:
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
				break;
			}
		}
		window.display();
		sideWindow.display();
	}
	return 0;
}
