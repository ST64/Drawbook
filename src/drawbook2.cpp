#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <string>
using namespace std;
int drawing=0;
double _x1,this_y1,_x2,_y2;
int width_rect=0;
int height_rect=0;
int _width_rect=0;
int _height_rect=0;
int screenshot=0;
int color=0;
int textmode=0;
int gamemode=0;
char savescreen[65];
char bmpname[100];
string text_string;
int r[]={0,255,255,0,0,128, 255}; /* black, red, yellow, green 3, deepskyblue, gray, white*/
int g[]={0,0,255,205,199,128,255};
int b[]={0,0,0,0,255,128,255};
char charslist[]={' ','!','"','#','$','%','&','\'','(',')','*','+',',','-','.'};
sf::Image Image_screenshot;
sf::Texture texture;
sf::Sprite spritez;
sf::Font font;
sf::Text text;
int main(){
  sf::RenderWindow window(sf::VideoMode(800,600),"Drawbook",sf::Style::Default);
  sf::RenderWindow sideWindow(sf::VideoMode(200,400),"Tools",sf::Style::Close||sf::Style::Titlebar);
  sf::Event event;
  window.clear(sf::Color::White);
  sideWindow.clear(sf::Color::White);
  sf::Vector2i mousevect;
  sf::Vector2u windowvect;
  this_y1=0.0f;
  _x1=0.0f;
  _x2=0.0f;
  _y2=0.0f;
  sf::Texture buttonsurfs[9];
  sf::Texture uptexture;
  sf::Texture downtexture;
//  sf::Texture temptexture;
  buttonsurfs[0].loadFromFile("Data\\new.png");
//  buttonsurfs[0]=temptexture;
  buttonsurfs[1].loadFromFile("Data\\up.png");
//  buttonsurfs[1]=temptexture;
  buttonsurfs[2].loadFromFile("Data\\down.png");
//  buttonsurfs[2]=temptexture;
  buttonsurfs[3].loadFromFile("Data\\Color.png");
//  buttonsurfs[3]=temptexture;
  buttonsurfs[4].loadFromFile("Data\\Save.png");
//  buttonsurfs[4]=temptexture;
  buttonsurfs[5].loadFromFile("Data\\Load.png");
//  buttonsurfs[5]=temptexture;
  buttonsurfs[6].loadFromFile("Data\\text.png");
//  buttonsurfs[6]=temptexture;
  buttonsurfs[7].loadFromFile("Data\\text2.png");
//  buttonsurfs[7]=temptexture;
  sf::Sprite sprites[9];
  sf::Sprite tempsprite;
  int x_rects[9]={0,0,100,-100,0,0,0,0,0};
  int y_rects[9]={0,50,0,100,50,50,50,50,0};
  int w_rects[9]={200,75,75,200,200,200,200,200,0};
  int h_rects[9]={50,100,100,50,50,50,50,50,0};
  int __x=0;
  int __y=0;
  int __w=0;
  int __h=0;
  sideWindow.display();
  sideWindow.clear(sf::Color::White);
  tempsprite.move(0,-350);
  for (int i=0;i<8;i++){
	__x=x_rects[i];
    __y=y_rects[i];
    __w=w_rects[i];
    __h=h_rects[i];
    tempsprite.setTexture(buttonsurfs[i]);
    tempsprite.setTextureRect(sf::IntRect(0,0,__w,__h));
    tempsprite.move(__x,__y);
    sprites[i]=tempsprite;
    sideWindow.draw(sprites[i]);
  }
  sideWindow.display();
  sideWindow.clear(sf::Color::White);
  for (int i=0;i<8;i++){
	__x=x_rects[i];
    __y=y_rects[i];
    __w=w_rects[i];
    __h=h_rects[i];
    tempsprite.setTexture(buttonsurfs[i]);
    tempsprite.setTextureRect(sf::IntRect(0,0,__w,__h));
    tempsprite.move(__x,__y);
    sprites[i]=tempsprite;
    sideWindow.draw(sprites[i]);
  }
  sideWindow.display();
  window.setPosition(sf::Vector2i(200,0));
  sideWindow.setPosition(sf::Vector2i(0,0));
  font.loadFromFile("Data\\Drawbook.ttf");
  text.setFont(font);
  text.setString("Welcome to Drawbook.\nPress the Up or Down button to change line size.\nDraw with the mouse.\nPress ESC to Continue");
  text.setCharacterSize(25);
  text.setColor(sf::Color::Black);
  window.draw(text);
  window.setFramerateLimit(60);
  while (window.isOpen()&&sideWindow.isOpen()){
	window.clear(sf::Color::White);
	if (gamemode==1){
		spritez.setTexture(texture);
		window.draw(spritez);
	}
	if (gamemode==0){
		window.draw(text);
	}
    while (sideWindow.pollEvent(event)){
      if (event.type == sf::Event::Closed){
        sideWindow.close();
        window.close();
      }
      if (event.type == sf::Event::MouseButtonPressed){
        mousevect = sf::Mouse::getPosition(sideWindow);
        if (mousevect.y < 50){
          window.clear(sf::Color::White);
        }
        if ((mousevect.y > 50)&&(mousevect.y < 150)&&(mousevect.x < 100)){
          screenshot++;
          cout << screenshot << endl;
        }
        if ((mousevect.y > 50)&&(mousevect.y < 150)&&(mousevect.x > 100)){
          screenshot--;
          cout << screenshot << endl;
        }
        if ((mousevect.y > 150)&&(mousevect.y < 200)){
          color++;
          if (color > 8){color=0;}
        }
        if ((mousevect.y > 200)&&(mousevect.y < 250)){
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
        }
        if ((mousevect.y > 250)&&(mousevect.y < 300)){
          sprintf(savescreen,"Screenshot%d",screenshot);
          strcat(bmpname,savescreen);
          strcat(bmpname,".png");
          Image_screenshot.loadFromFile(bmpname);
          texture.loadFromImage(Image_screenshot);
          spritez.setTexture(texture);
          window.draw(spritez);
          memset(bmpname,0,99);
        }
        if ((mousevect.y > 300)&&(mousevect.y < 350)){
          text.setString("Welcome to Drawbook.\nPress the Up or Down button to change line size.\nDraw with the mouse and press ESC to reset\nMore Features COMING SOON...");
          text.setCharacterSize(25);
          text.setColor(sf::Color::Black);
          window.draw(text);
        }
        if ((mousevect.y > 350)){
          if (textmode==0){
            textmode=1;
            sf::Vector2u windowSize = window.getSize();
            texture.create(windowSize.x, windowSize.y);
            texture.update(window);
            Image_screenshot = texture.copyToImage();
            Image_screenshot.saveToFile("Data\\saves.png");
          }else{
            textmode=0;
            sf::Vector2u windowSize = window.getSize();
            texture.create(windowSize.x, windowSize.y);
            texture.update(window);
            Image_screenshot = texture.copyToImage();
            Image_screenshot.saveToFile("Data\\saves.png");
          }
        }
      }
    }
    while (window.pollEvent(event)){
      if (event.type == sf::Event::Closed){
        window.close();
        sideWindow.close();
      }
//      if (event.type == sf::Event::KeyPressed){
//        w
//      }
      if (event.type == sf::Event::MouseButtonPressed){
        drawing=1;
        mousevect = sf::Mouse::getPosition(window);
        windowvect= window.getSize();
        if ((mousevect.x < 0)||(mousevect.y < 0)){
          drawing=0;
        }else if ((mousevect.x > windowvect.x)||(mousevect.y > windowvect.y)){
          drawing=0;
        }
      }
      if (event.type == sf::Event::MouseButtonReleased){
        drawing=0;
      }
      if (event.type == sf::Event::MouseMoved){
        _x2=_x1;
        _y2=this_y1;
        _x1=event.mouseMove.x;
        this_y1=event.mouseMove.y;
        if (drawing==1){
          _width_rect = _x2-_x1+1;
          _height_rect= _y2-this_y1+1;
          sf::Vertex line[] =
          {
            sf::Vertex(sf::Vector2f(_x1,this_y1),sf::Color(r[color],g[color],b[color])),
            sf::Vertex(sf::Vector2f(_x2,_y2),sf::Color(r[color],g[color],b[color]))
          };
          window.draw(line,2,sf::Lines);
        }
      }
      if (event.type==sf::Event::KeyPressed){
        if (event.key.code==sf::Keyboard::Escape){
          window.clear(sf::Color::White);
		  gamemode = 1;
        }
        if (event.key.code==sf::Keyboard::BackSpace){
          if (textmode==1){
            text_string.pop_back();
            Image_screenshot.loadFromFile("Data\\saves.png");
            texture.loadFromImage(Image_screenshot);
            spritez.setTexture(texture);
            window.draw(spritez);
            text.setString(text_string);
            text.setCharacterSize(25);
            text.setColor(sf::Color::Black);
            window.draw(text);
          }
        }
      }
      if (event.type==sf::Event::TextEntered){
        if (textmode==1){
          if ((event.text.unicode == 10)||(event.text.unicode == 13)){
            char rs = '\n';
            text_string.push_back(rs);
            text.setString(text_string);
            text.setCharacterSize(25);
            text.setColor(sf::Color::Black);
            window.draw(text);
        }
        if ((event.text.unicode < 128)&&(event.text.unicode > 31)){
          char rs = static_cast<char>(event.text.unicode);
          if (event.text.unicode < 48){
            rs = charslist[event.text.unicode-32];
          }
          text_string.push_back(rs);
          text.setString(text_string);
          text.setCharacterSize(25);
          text.setColor(sf::Color::Black);
          window.draw(text);
        }
        }
      }
    }
	sf::Vector2u windowSize = window.getSize();
	texture.create(windowSize.x, windowSize.y);
    texture.update(window);
    window.display();
  }
}
