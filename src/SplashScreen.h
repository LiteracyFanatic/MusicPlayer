// SplashScreen.h

#ifndef _SPLASHSCREEN_h
#define _SPLASHSCREEN_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "Screen.h"
#include "MusicPlayer.h"

class SplashScreen: public Screen
{
public:
	SplashScreen(Adafruit_GFX *gfx, TouchScreen *touchScreen);
	void init();
	void draw();
	void update();

private:
	void drawArduinoLogo(unsigned int color);
};

#endif

