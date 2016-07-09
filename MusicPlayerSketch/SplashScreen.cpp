// 
// 
// 

#include "SplashScreen.h"

SplashScreen::SplashScreen(Adafruit_GFX *gfx, TouchScreen *touchScreen)
{
	tft = gfx;
	ts = touchScreen;
}

void SplashScreen::init()
{
	cx = tft->width() / 2;
	cy = tft->height() / 2;
}

void SplashScreen::draw()
{
	tft->fillScreen(backgroundColor);
	drawArduinoLogo(themeColor);
}

void SplashScreen::update()
{
}

void SplashScreen::drawArduinoLogo(unsigned int color)
{
	int outerRadius = 80;
	int innderRadius = outerRadius - 30;
	int overlap = 10;
	int leftCenterX = cx - (outerRadius - overlap);
	int rightCenterX = cx + (outerRadius - overlap);
	int centerY = cy - 30;

	tft->fillCircle(leftCenterX, centerY, outerRadius, color);
	tft->fillCircle(rightCenterX, centerY, outerRadius, color);
	tft->fillCircle(leftCenterX, centerY, innderRadius, backgroundColor);
	tft->fillCircle(rightCenterX, centerY, innderRadius, backgroundColor);

	tft->fillRect(leftCenterX - 25, centerY - 7, 50, 14, color);

	tft->fillRect(rightCenterX - 20, centerY - 7, 40, 14, color);
	tft->fillRect(rightCenterX - 7, centerY - 20, 14, 40, color);

	tft->setCursor(5, tft->height() - 55);
	tft->setTextSize(6);
	tft->setTextWrap(false);
	tft->setTextColor(color);

	char text[] = "Noteduino";

	delay(400);

	for (byte i = 0; i < sizeof(text) / sizeof(char) - 1; i++)
	{
		delay(50);
		tft->print(text[i]);
	}
}