// 
// 
// 

#include "DedicationScreen.h"

DedicationScreen::DedicationScreen(Adafruit_GFX * gfx, TouchScreen * touchScreen)
{
	tft = gfx;
	ts = touchScreen;
}

void DedicationScreen::init()
{
	cx = tft->width() / 2;
	cy = tft->height() / 2;
}

void DedicationScreen::draw()
{
	tft->fillScreen(backgroundColor);
	tft->setTextSize(2);
	tft->setTextWrap(false);
	tft->setTextColor(themeColor);

	int margin = 10;
	int r = 30;
	tft->drawRoundRect(margin, margin, tft->width() - 2 * margin, tft->height() - 2 * margin, r, r);

	tft->setCursor(30, 75);
	tft->print("A project dedicated to");

	tft->setCursor(22, 95);
	tft->print("Julia Lake, whose music");
	
	tft->setCursor(50, 115);
	tft->print("and friendship have");

	tft->setCursor(50, 135);
	tft->print("brought so much joy");

	tft->setCursor(85, 155);
	tft->print("into my life.");
}

void DedicationScreen::update()
{
}
