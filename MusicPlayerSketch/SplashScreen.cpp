// 
// 
// 

#include "SplashScreen.h"

SplashScreen::SplashScreen(Adafruit_GFX *gfx, TouchScreen *touchScreen)
{
	tft = gfx;
	ts = touchScreen;
}

void SplashScreen::draw()
{
	tft->fillScreen(BLACK);
	drawArduinoLogo(BLUE);
}

void SplashScreen::update()
{
}

void SplashScreen::drawArduinoLogo(unsigned int color)
{
	int outerRadius = 80;
	int innderRadius = outerRadius - 30;
	int overlap = 10;
	int leftCenterX = tft->width() / 2 - (outerRadius - overlap);
	int rightCenterX = tft->width() / 2 + (outerRadius - overlap);
	int centerY = tft->height() / 2 - 30;

	tft->fillCircle(leftCenterX, centerY, outerRadius, color);
	tft->fillCircle(rightCenterX, centerY, outerRadius, color);
	tft->fillCircle(leftCenterX, centerY, innderRadius, BLACK);
	tft->fillCircle(rightCenterX, centerY, innderRadius, BLACK);

	tft->fillRect(leftCenterX - 25, centerY - 7, 50, 14, color);

	tft->fillRect(rightCenterX - 20, centerY - 7, 40, 14, color);
	tft->fillRect(rightCenterX - 7, centerY - 20, 14, 40, color);

	tft->setCursor(5, 185);
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