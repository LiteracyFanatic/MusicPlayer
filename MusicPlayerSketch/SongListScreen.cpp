// 
// 
// 

#include "SongListScreen.h"

SongListScreen::SongListScreen(Adafruit_GFX *gfx, TouchScreen *touchScreen, char* t[], byte n)
{
	tft = gfx;
	ts = touchScreen;
	titles = t;
	numberOfSongs = n;
	numberOfPages = ceil((float)numberOfSongs / 5);
}

void SongListScreen::init()
{
	cx = tft->width() / 2;
	cy = tft->height() / 2;
}

void SongListScreen::drawCD(int cx, int cy, int r, unsigned int color, bool pressed)
{
	if (pressed)
	{
		tft->fillCircle(cx, cy, r, GRAY);
		tft->drawCircle(cx, cy, r, color);
		tft->fillCircle(cx, cy, r / 2, color);
		tft->fillCircle(cx, cy, r / 5, backgroundColor);
	}
	else
	{
		tft->fillCircle(cx, cy, r, color);
		tft->drawCircle(cx, cy, r, GRAY);
		tft->fillCircle(cx, cy, r / 2, GRAY);
		tft->fillCircle(cx, cy, r / 5, backgroundColor);
	}
}

void SongListScreen::draw()
{
	variableChangeAction();
	tft->fillScreen(backgroundColor);
	int margin = 10;

	int w = tft->width() - 2 * margin;
	int h = tft->height() - 2 * margin;

	int buttonHeight = (h - margin * 4) / 5;
	int buttonWidth = w * ((float)2 / 3);
	int buttonCenterx = margin + buttonWidth / 2;

	int tSide = 50;
	int tcx = margin + w * ((float)5 / 6);
	int tcy1 = (tft->height() / 2) - 70;
	int tcy2 = (tft->height() / 2) + 70;

	for (byte i = 0; i < 5; i++)
	{
		songButtons[i].initButton(tft, buttonCenterx, margin + (buttonHeight / 2) + i * (margin + buttonHeight), buttonWidth, buttonHeight, themeColor, themeColor, backgroundColor, titles[i + 5 * curPage], 2);
	}

	for (byte i = 0; i < 5; i++)
	{
		if (5 * curPage + i < numberOfSongs)
		{
			songButtons[i].drawButton();
		}
	}

	upButton.initButton(tft, tcx, tcy1, tSide, tSide * cos(radians(30)), themeColor, BLUE, themeColor, NULL, 0);
	downButton.initButton(tft, tcx, tcy2, tSide, tSide * cos(radians(30)), themeColor, BLUE, themeColor, NULL, 0);

	if (curPage < numberOfPages - 1)
	{
		drawDownButton(tcx, tcy2, tSide, themeColor);
	}

	if (curPage > 0)
	{
		drawUpButton(tcx, tcy1, tSide, themeColor);
	}

	cdButton.initButton(tft, tcx, cy, 60, 60, themeColor, BLUE, themeColor, NULL, 0);
	drawCD(tcx, cy, 30, themeColor, false);

}

void SongListScreen::update()
{
	variableChangeAction();

	int margin = 10;

	int w = tft->width() - 2 * margin;
	int h = tft->height() - 2 * margin;

	int buttonHeight = (h - margin * 4) / 5;
	int buttonWidth = w * ((float)2 / 3);
	int buttonCenterx = margin + buttonWidth / 2;

	int tSide = 50;
	int tcx = margin + w * ((float)5 / 6);
	int tcy1 = (tft->height() / 2) - 70;
	int tcy2 = (tft->height() / 2) + 70;

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
	else
	{
		p.x = 0;
		p.y = 0;
	}

	for (byte i = 0; i < 5; i++)
	{
		if (5 * curPage + i < numberOfSongs)
		{
			if (songButtons[i].contains(p.x, p.y))
			{
				songButtons[i].press(true);
			}
			else
			{
				songButtons[i].press(false);
			}
		}
	}

	for (byte i = 0; i < 5; i++)
	{
		if (songButtons[i].justPressed())
		{
			songButtons[i].drawButton(true);
		}

		if (songButtons[i].justReleased())
		{
			songButtons[i].drawButton();

			pressedButton = i;

			songButtonAction();
			return;
		}
	}

	if (curPage > 0)
	{
		if (upButton.contains(p.x, p.y))
		{

			upButton.press(true);
		}
		else
		{
			upButton.press(false);
		}

		if (upButton.justPressed())
		{
			drawUpButton(tcx, tcy1, tSide, GRAY);
		}

		if (upButton.justReleased())
		{
			if (curPage > 0)
			{
				curPage--;
				draw();
			}
		}
	}

	if (curPage < numberOfPages - 1)
	{
		if (downButton.contains(p.x, p.y))
		{
			downButton.press(true);
		}
		else
		{
			downButton.press(false);
		}

		if (downButton.justPressed())
		{
			drawDownButton(tcx, tcy2, tSide, GRAY);
		}

		if (downButton.justReleased())
		{
			if (curPage < numberOfPages - 1)
			{
				curPage++;
				draw();
			}
		}
	}

	if (cdButton.contains(p.x, p.y))
	{
		cdButton.press(true);
	}
	else
	{
		cdButton.press(false);
	}

	if (cdButton.justPressed())
	{
		drawCD(tcx, cy, 30, themeColor, true);
	}

	if (cdButton.justReleased())
	{
		drawCD(tcx, cy, 30, themeColor, false);
		cdButtonAction();
		return;
	}
}

void SongListScreen::onSongButtonPressed(void(*f)())
{
	songButtonAction = f;
}

void SongListScreen::onCDButtonPressed(void(*f)())
{
	cdButtonAction = f;
}

void SongListScreen::linkVariables(void(*f)())
{
	variableChangeAction = f;
}

void SongListScreen::drawUpButton(int cx, int cy, int ts, unsigned int color)
{
	int th = ts * cos(radians(30));
	tft->fillTriangle(cx - ts / 2, cy + th / 2, cx + ts / 2, cy + th / 2, cx, cy - th / 2, color);
}

void SongListScreen::drawDownButton(int cx, int cy, int ts, unsigned int color)
{
	int th = ts * cos(radians(30));
	tft->fillTriangle(cx - ts / 2, cy - th / 2, cx + ts / 2, cy - th / 2, cx, cy + th / 2, color);
}




