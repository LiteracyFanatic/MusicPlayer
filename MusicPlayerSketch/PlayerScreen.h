// PlayerScreen.h

#ifndef _PLAYERSCREEN_h
#define _PLAYERSCREEN_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Screen.h"
#include "MusicPlayer.h"

class PlayerScreen: public Screen
{ 
public:
	PlayerScreen(Adafruit_GFX *gfx, TouchScreen *touchScreen, MusicPlayer *musicPlayer);
	void init();
	void draw();
	void update();

private:
	//Adafruit_GFX *tft;
	//TouchScreen *ts;
	MusicPlayer *mp;

	void drawListButton(int cx, int cy, int w, int h, unsigned int color);
	void drawProgressBar(int x1, int x2, int y, int h, float p, unsigned int color, bool refresh);
	void displayTitle();
	void drawPlayButton(int cx, int cy, unsigned int color);
	void drawPauseButton(int cx, int cy, unsigned int color);
	int drawNextButton(int x, int y, int h, unsigned int color);
	int drawPreviousButton(int x, int y, int h, unsigned int color);
	float mapFloat(double x, double in_min, double in_max, double out_min, double out_max);

	int cx;
	int cy;
	int buttonSize = 50;

	Adafruit_GFX_Button playPauseButton;
	Adafruit_GFX_Button nextButton;
	Adafruit_GFX_Button previousButton;
	Adafruit_GFX_Button listButton;

	int nextButtonX;
	int nextButtonY;
	int nextButtonW;
	int previousButtonX;
	int previousButtonY;
	int previousButtonW;

	byte prevSong = 0;

	float prog;

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

