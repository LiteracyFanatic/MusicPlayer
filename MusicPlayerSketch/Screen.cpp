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

		Serial.println(p.z);

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
