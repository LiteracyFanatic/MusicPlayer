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
	byte getCurrentPage();
	void setCurrentPage(byte cs);
	bool paused = true;
	float prog;

private:
	void(*nextButtonAction)() = []() {};
	void(*previousButtonAction)() = []() {};
	void(*playButtonAction)() = []() {};
	void(*pauseButtonAction)() = []() {};
	void(*listButtonAction)() = []() {};
	void(*variableChangeAction)() = []() {};
	void drawListButton(int cx, int cy, int w, int h, unsigned int color);
	void drawProgressBar(int x1, int x2, int y, int h, float p, unsigned int color, bool refresh);
	void displayTitle();
	void drawPlayButton(int cx, int cy, int w, int h, unsigned int color);
	void drawPauseButton(int cx, int cy, int w, int h, unsigned int color);
	void drawNextButton(int x, int y, int w, int h, unsigned int color);
	void drawPreviousButton(int x, int y, int w, int h, unsigned int color);
	float mapFloat(double x, double in_min, double in_max, double out_min, double out_max);
	byte currentPage = 0;
	byte numberOfPages;
	byte prevSong = 0;
	float lastProg = 0;
	char **titles;
	Adafruit_GFX_Button playPauseButton;
	Adafruit_GFX_Button nextButton;
	Adafruit_GFX_Button previousButton;
	Adafruit_GFX_Button listButton;
	int cx;
	int cy;
	int playPauseButtonX;
	int playPauseButtonSize;
	int nextButtonX;
	int nextButtonY;
	int previousButtonX;
	int previousButtonY;
	int npButtonW;
	int npButtonH;
	int controlsY;
	int progressBarX0;
	int progressBarX1;
	int progressBarY;
	int progressBarH;
	int listButtonX;
	int listButtonY;
	int listButtonW;
	int listButtonH;
};

#endif

