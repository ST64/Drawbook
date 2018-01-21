#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Color_Chooser.H>
#include <FL/Fl_Native_File_Chooser.H>
#include <utility>
#include <vector>
#include <tuple>
#include <cstdint>
#include <iostream>
#include "simple_svg_1.0.0.hpp"
using sPoint = std::pair<int,int>;
using GLColor = std::tuple<double,double,double>;
std::vector<std::pair<GLColor,std::vector<sPoint>>> drawable_verts;
int f=0;
class DrawbookGLWin : public Fl_Gl_Window {
	void FixViewport(int W, int H){
		glLoadIdentity();
		glViewport(0,0,W,H);
		glOrtho(-W,W,-H,H,-1,1);
	}
	void draw(){
		if (!valid()){
			valid(1);
			FixViewport(w(),h());
		}
		glClearColor(1.0f,1.0f,1.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		for (int64_t i=0;i<drawable_verts.size();i++){
			glColor3f(std::get<0>(drawable_verts[i].first),std::get<1>(drawable_verts[i].first),std::get<2>(drawable_verts[i].first));
			glBegin(GL_LINE_STRIP);
			for (int64_t j=0;j<drawable_verts[i].second.size();j++){
				glVertex2f(static_cast<double>(drawable_verts[i].second[j].first),static_cast<double>(drawable_verts[i].second[j].second));
			}
			glEnd();
		}
	}
	int handle(int e){
		switch (e){
			case FL_PUSH:
				drawable_verts[f].second.emplace_back(sPoint(Fl::event_x()*2-w(),h() - Fl::event_y()*2));
				return 1;
				break;
			case FL_DRAG:
				drawable_verts[f].second.emplace_back(sPoint(Fl::event_x()*2-w(),h() - Fl::event_y()*2));
				break;
			case FL_RELEASE:
				drawable_verts[f].second.emplace_back(sPoint(Fl::event_x()*2-w(),h() - Fl::event_y()*2));
				f++;
				drawable_verts.emplace_back();
				drawable_verts[f].first = drawable_verts[f-1].first;
				break;
			default:
				break;
		}
		return Fl_Gl_Window::handle(e);
	}
	void resize(int X, int Y, int W, int H){
		Fl_Gl_Window::resize(X,Y,W,H);
		FixViewport(W,H);
		redraw();
	}
	static void Timer_CB(void *userdata){
		DrawbookGLWin *pb = (DrawbookGLWin *)userdata;
		pb->redraw();
		Fl::repeat_timeout(1.0/60.0,Timer_CB,userdata);
	}
public:
	int frame=0;
	DrawbookGLWin(int X, int Y, int W, int H, const char *L=0) : Fl_Gl_Window(X,Y,W,H,L){
		frame = 0;
		Fl::add_timeout(1.0/60.0,Timer_CB,(void *)this);
		end();
	}
};


class DrawbookAppWindow : public Fl_Window {
	DrawbookGLWin *mygl;
	Fl_Menu_Bar *menu;
	static void Menu_CB(Fl_Widget *w, void *data){
		DrawbookAppWindow *o = (DrawbookAppWindow *)data;
		o->Menu_CB2();
	}
	void Menu_CB2(){
		char picked[80];
		menu->item_pathname(picked,sizeof(picked)-1);
		double r, g, b;
		if ( strcmp(picked,"Color/Pick Color") == 0 ){
			fl_color_chooser("Pick Color",r,g,b,-1);
			std::get<0>(drawable_verts[f].first) = r;
			std::get<1>(drawable_verts[f].first) = g;
			std::get<2>(drawable_verts[f].first) = b;
		}else if ( strcmp(picked,"File/New") == 0 ){
			drawable_verts.clear();
			drawable_verts.shrink_to_fit();
			drawable_verts.emplace_back();
			f=0;
		}else if ( strcmp(picked,"File/Quit") == 0){
			exit(0);
		}else if ( strcmp(picked,"File/Save") == 0 ){
			Fl_Native_File_Chooser chooser;
			chooser.title("Save File");
			chooser.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
			chooser.filter("*.svg");
			chooser.directory(".");
			switch (chooser.show()){
			case -1: break;
			case 1: break;
			default:
				svg::Dimensions dimensions(mygl->w(),mygl->h());
				svg::Document doc(chooser.filename(),svg::Layout(dimensions,svg::Layout::TopLeft));
				for (int64_t i=0;i<static_cast<int64_t>(drawable_verts.size());i++){
					svg::Polyline polyline_a(svg::Stroke(1,svg::Color(std::get<0>(drawable_verts[i].first)*256.0,std::get<1>(drawable_verts[i].first)*256.0,std::get<2>(drawable_verts[i].first)*256.0)));
					for (int32_t j=0;j<static_cast<int32_t>(drawable_verts[i].second.size());j++){
							polyline_a << svg::Point(drawable_verts[i].second[j].first,drawable_verts[i].second[j].second);
					}
					if (drawable_verts[i].second.size() != 0){
						doc << polyline_a;
					}
				}
				doc.save();
				break;
			}

		}else if ( strcmp(picked,"File/Load") == 0 ){
			Fl_Native_File_Chooser chooser;
			chooser.title("Load File");
			chooser.type(Fl_Native_File_Chooser::BROWSE_FILE);
			chooser.filter("*.svg");
			chooser.directory(".");
			switch (chooser.show()){
			case -1: break;
			case 1: break;
			default:
				drawable_verts.clear();
				drawable_verts.shrink_to_fit();
				drawable_verts.emplace_back();
				f=0;
				std::ifstream ifs(chooser.filename());
				while (ifs.good()){
					std::string SingleLine;
					getline(ifs,SingleLine,'\n');
					if (SingleLine[2]=='p'){
						SingleLine.erase(0,SingleLine.find("points=\""));
						std::stringstream SL(SingleLine);
						while (SL.good()){
							std::string SinglePoint;
							SL >> SinglePoint;
							sPoint tmp;
							sscanf(SinglePoint.c_str(),"%i,%i ",&tmp.first,&tmp.second);
							if ((tmp.first != 0)||(tmp.second != 0)){
								drawable_verts[f].second.emplace_back(tmp);
							}
						}
						SingleLine.erase(0,SingleLine.find("stroke=\"rgb("));
						int r=0,g=0,b=0;
						sscanf(SingleLine.c_str(),"%i,%i,%i",&r,&g,&b);
						std::get<0>(drawable_verts[f].first) = static_cast<double>(r) * 0.00390625;
						std::get<1>(drawable_verts[f].first) = static_cast<double>(g) * 0.00390625;
						std::get<2>(drawable_verts[f].first) = static_cast<double>(b) * 0.00390625;
						f++;
						drawable_verts.emplace_back();
					}
				}
				break;
			}
		}
	}
public:
	DrawbookAppWindow(int W, int H, const char *L=0) : Fl_Window(W,H,L){
		mygl = new DrawbookGLWin(25,25,w()-35,h()-35);
		menu = new Fl_Menu_Bar(0,0,800,25);
		menu->add("File/New",0,Menu_CB,(void *)this);
		menu->add("File/Save",0,Menu_CB,(void *)this);
		menu->add("File/Load",0,Menu_CB,(void *)this);
		menu->add("File/Quit",0,Menu_CB,(void *)this);
		menu->add("Color/Pick Color",0,Menu_CB,(void*)this);
		end();
	}
};
int main(){
	drawable_verts.emplace_back();
	DrawbookAppWindow win(800,600,"Drawbook Test");
	win.resizable(win);
	win.show();
	return (Fl::run());
}
