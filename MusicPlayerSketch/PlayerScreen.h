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
	PlayerScreen(Adafruit_GFX *gfx, TouchScreen *touchScreen, char* t[], byte n);
	void init();
	void draw();
	void update();
	void onNextButtonPressed(void (*f)());
	void onPreviousPressed(void(*f)());
	void onPlayButtonPressed(void(*f)());
	void onPauseButtonPressed(void(*f)());
	void onListButtonPressed(void(*f)());
	void linkVariables(void(*f)());
	byte curSong = 0;
	byte numberOfSongs;
	bool paused = true;
	float prog;
	unsigned int themeColor = WHITE;
	unsigned int backgroundColor = BLACK;

private:
	//Adafruit_GFX *tft;
	//TouchScreen *ts;
	MusicPlayer *mp;

	void(*nextButtonAction)();
	void(*previousButtonAction)();
	void(*playButtonAction)();
	void(*pauseButtonAction)();
	void(*listButtonAction)();
	void(*variableChangeAction)();

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
	float lastProg = 0;

	char **titles;
};

#endif

