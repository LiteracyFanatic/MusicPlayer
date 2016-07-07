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

class SongListScreen: public Screen
{
public:
	SongListScreen(Adafruit_GFX *gfx, TouchScreen *touchScreen, MusicPlayer *musicPlayer);
	void init();
	void draw();
	void update();
	void drawCD(int cx, int cy, int r, unsigned int color, bool pressed);
	void drawUpButton(int cx, int cy, int ts, unsigned int color);
	void drawDownButton(int cx, int cy, int ts, unsigned int color);

private:
	//Adafruit_GFX *tft;
	//TouchScreen *ts;

	int cx;
	int cy;

	Adafruit_GFX_Button songButtons[5];
	Adafruit_GFX_Button upButton;
	Adafruit_GFX_Button downButton;
	Adafruit_GFX_Button cdButton;

	byte curPage = 0;
	byte numberOfPages;

	MusicPlayer *mp;

	char* titles[7] =
	{
		"Bach Partita 2",
		"Bach Double",
		"Bach Presto",
		"Mario Castle",
		"Mario Tune",
		"Nyan Cat",
		"Bonetrousle"
	};
};



#endif

