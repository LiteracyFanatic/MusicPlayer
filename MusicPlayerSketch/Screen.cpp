// 
// 
// 

#include "Screen.h"
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>

void Screen::waitForTap(unsigned long waitTime)
{
	unsigned long ti = millis();
	while (true)
	{
		digitalWrite(13, HIGH);
		TSPoint p = ts->getPoint();
		digitalWrite(13, LOW);

		// if sharing pins, you'll need to fix the directions of the touchscreen pins
		//pinMode(XP, OUTPUT);
		pinMode(XM, OUTPUT);
		pinMode(YP, OUTPUT);
		//pinMode(YM, OUTPUT);

		if (p.z > MINPRESSURE && p.z < MAXPRESSURE)
		{
			return;
		}

		if (waitTime != NULL)
		{
			if (millis() >= ti + waitTime)
			{
				return;
			}
		}
	}
}

unsigned int Screen::getThemeColor()
{
	return themeColor;
}

void Screen::setThemeColor(unsigned int c)
{
	themeColor = c;
}

unsigned int Screen::getBackgroundColor()
{
	return backgroundColor;
}

void Screen::setBackgroundColor(unsigned int c)
{
	backgroundColor = c;
}

TSPoint Screen::readPoint()
{
	digitalWrite(13, HIGH);
	TSPoint p = ts->getPoint();
	digitalWrite(13, LOW);

	// if sharing pins, you'll need to fix the directions of the touchscreen pins
	//pinMode(XP, OUTPUT);
	pinMode(XM, OUTPUT);
	pinMode(YP, OUTPUT);
	//pinMode(YM, OUTPUT);

	if (p.z > MINPRESSURE && p.z < MAXPRESSURE)
	{
		int tempPY = p.y;
		p.y = (map(p.x, TS_MINX, TS_MAXX, 0, tft->height()));
		p.x = (map(tempPY, TS_MINY, TS_MAXY, 0, tft->width()));
	}
	return p;
}
