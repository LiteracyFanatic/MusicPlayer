// DedicationScreen.h

#ifndef _DEDICATIONSCREEN_h
#define _DEDICATIONSCREEN_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Screen.h"
#include "MusicPlayer.h"

class DedicationScreen : public Screen
{
public:
	DedicationScreen(Adafruit_GFX *gfx, TouchScreen *touchScreen);
	void init();
	void draw();
	void update();

private:
};


#endif

