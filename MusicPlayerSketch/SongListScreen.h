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
	byte curPage = 0;
	byte numberOfPages;
	byte numberOfSongs;
	byte pressedButton = -1;
	void onSongButtonPressed(void(*f)());
	void onCDButtonPressed(void(*f)());
	void linkVariables(void(*f)());
	unsigned int themeColor = WHITE;
	unsigned int backgroundColor = BLACK;


private:
	void(*songButtonAction)();
	void(*cdButtonAction)();
	void(*variableChangeAction)();

	void drawCD(int cx, int cy, int r, unsigned int color, bool pressed);
	void drawUpButton(int cx, int cy, int ts, unsigned int color);
	void drawDownButton(int cx, int cy, int ts, unsigned int color);

	int cx;
	int cy;

	Adafruit_GFX_Button songButtons[5];
	Adafruit_GFX_Button upButton;
	Adafruit_GFX_Button downButton;
	Adafruit_GFX_Button cdButton;

	MusicPlayer *mp;

	char **titles;
};



#endif

