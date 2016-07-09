// SongListScreen.h

#ifndef _SONGLISTSCREEN_h
#define _SONGLISTSCREEN_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "Screen.h"
#include "MusicPlayer.h"

class SongListScreen : public Screen
{
public:
	SongListScreen(Adafruit_GFX *gfx, TouchScreen *touchScreen, char* t[], byte n);
	void init();
	void draw();
	void update();
	void onListButtonPressed(void(*f)());
	void onCDButtonPressed(void(*f)());
	void linkVariables(void(*f)());
	byte getSelectedItem();
	byte getItemsPerPage();
	byte getCurrentPage();
	byte setCurrentPage(byte c);
	byte getNumberOfPages();
	

private:
	void(*listButtonAction)() = []() {};
	void(*cdButtonAction)() = []() {};
	void(*variableChangeAction)() = []() {};
	void drawCD(int cx, int cy, int r, unsigned int color, bool pressed);
	void drawUpButton(int cx, int cy, int ts, unsigned int color);
	void drawDownButton(int cx, int cy, int ts, unsigned int color);
	int cx;
	int cy;
	char** titles;
	const static byte itemsPerPage = 5;
	byte numberOfItems;
	byte selectedItem = 0;
	byte currentPage = 0;
	byte numberOfPages;
	Adafruit_GFX_Button listButtons[itemsPerPage];
	Adafruit_GFX_Button upButton;
	Adafruit_GFX_Button downButton;
	Adafruit_GFX_Button cdButton;
	int margin;
	int w;
	int h;
	int buttonHeight;
	int buttonWidth;
	int buttonCenterx;
	int tSide;
	int tcx;
	int tcy1;
	int tcy2;
	int cdRadius;
};



#endif

