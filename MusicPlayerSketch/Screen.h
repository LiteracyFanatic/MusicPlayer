// Screen.h

#ifndef _SCREEN_h
#define _SCREEN_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>

#define	BLACK   0x0000
#define GRAY    0xBDF7
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940

#define MINPRESSURE 10
#define MAXPRESSURE 1000

class Screen
{
public:
	virtual void draw() = 0;
	virtual void update() = 0;
	void waitForTap(unsigned long waitTime = NULL);

protected:
	Adafruit_GFX *tft;
	TouchScreen *ts;
	
};

#endif

