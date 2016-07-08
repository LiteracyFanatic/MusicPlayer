// 
// 
// 

#include "PlayerScreen.h"


PlayerScreen::PlayerScreen(Adafruit_GFX *gfx, TouchScreen *touchScreen, char* t[], byte n)
{
	tft = gfx;
	ts = touchScreen;
	titles = t;
	numberOfSongs = n;
}

void PlayerScreen::init()
{
	cx = tft->width() / 2;
	cy = tft->height() / 2;

	nextButtonX = tft->width() - (buttonSize * 2) - 2;
	nextButtonY = cy - (buttonSize / 2) - 20;
	previousButtonX = 20;
	previousButtonY = cy - (buttonSize / 2) - 20;

	//BLETCHEROUS HACK
	nextButtonW = buttonSize * 2 - 18;
	previousButtonW = buttonSize * 2 - 18;
	//BLETCHEROUS HACK
}

void PlayerScreen::draw()
{
	variableChangeAction();
	tft->fillScreen(backgroundColor);

	if (paused)
	{
		drawPlayButton(cx, cy - 20, themeColor);
	}
	else
	{
		drawPauseButton(cx, cy - 20, themeColor);
	}

	playPauseButton.initButton(tft, cx, cy - 20, buttonSize, buttonSize, themeColor, BLUE, themeColor, NULL, 3);

	drawNextButton(nextButtonX, nextButtonY, buttonSize, themeColor);
	drawPreviousButton(previousButtonX, previousButtonY, buttonSize, themeColor);

	nextButton.initButton(tft, nextButtonX + nextButtonW / 2, nextButtonY + buttonSize / 2, nextButtonW, buttonSize, themeColor, BLUE, themeColor, NULL, 3);

	previousButton.initButton(tft, previousButtonX + previousButtonW / 2, previousButtonY + buttonSize / 2, previousButtonW, buttonSize, themeColor, BLUE, themeColor, NULL, 3);

	displayTitle();
	drawProgressBar(10, tft->width() - 10, 155, 10, prog, themeColor, true);

	listButton.initButton(tft, cx, tft->height() - 30, 60, 40, themeColor, backgroundColor, themeColor, NULL, 0);
	drawListButton(cx, tft->height() - 40, 60, 40, themeColor);
}

void PlayerScreen::update()
{
	variableChangeAction();

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

	if (nextButton.contains(p.x, p.y))
	{
		nextButton.press(true);
	}
	else
	{
		nextButton.press(false);
	}

	if (nextButton.justPressed())
	{
		drawNextButton(nextButtonX, nextButtonY, buttonSize, GRAY);
	}

	if (nextButton.justReleased())
	{
		drawNextButton(nextButtonX, nextButtonY, buttonSize, themeColor);
		if (paused)
		{
			drawPlayButton(cx, cy - 20, backgroundColor);
		}
		drawPauseButton(cx, cy - 20, themeColor);
		nextButtonAction();
	}

	if (previousButton.contains(p.x, p.y))
	{
		previousButton.press(true);
	}
	else
	{
		previousButton.press(false);
	}

	if (previousButton.justPressed())
	{
		drawPreviousButton(previousButtonX, previousButtonY, buttonSize, GRAY);
	}

	if (previousButton.justReleased())
	{
		drawPreviousButton(previousButtonX, previousButtonY, buttonSize, themeColor);
		if (paused)
		{
			drawPlayButton(cx, cy - 20, backgroundColor);
		}
		drawPauseButton(cx, cy - 20, themeColor);
		previousButtonAction();
	}

	if (playPauseButton.contains(p.x, p.y))
	{
		playPauseButton.press(true);
	}
	else
	{
		playPauseButton.press(false);
	}

	if (playPauseButton.justPressed())
	{
		if (paused)
		{
			drawPlayButton(cx, cy - 20, GRAY);
		}
		else
		{
			drawPauseButton(cx, cy - 20, GRAY);
		}
	}

	if (playPauseButton.justReleased())
	{
		if (paused)
		{
			playButtonAction();
			drawPlayButton(cx, cy - 20, backgroundColor);
			drawPauseButton(cx, cy - 20, themeColor);
		}
		else
		{
			pauseButtonAction();
			drawPauseButton(cx, cy - 20, backgroundColor);
			drawPlayButton(cx, cy - 20, themeColor);
		}
	}

	if (listButton.contains(p.x, p.y))
	{
		listButton.press(true);
	}
	else
	{
		listButton.press(false);
	}

	if (listButton.justPressed())
	{
		drawListButton(cx, tft->height() - 40, 60, 40, GRAY);
	}

	if (listButton.justReleased())
	{
		drawListButton(cx, tft->height() - 40, 60, 40, themeColor);
		listButtonAction();
		return;
	}

	if (curSong != prevSong)
	{
		displayTitle();
		prevSong = curSong;
	}

	if (lastProg > prog)
	{
		lastProg = 0;
		drawProgressBar(10, tft->width() - 10, 155, 10, prog, themeColor, true);
	}
	else
	{
		lastProg = prog;
		drawProgressBar(10, tft->width() - 10, 155, 10, prog, themeColor, false);
	}

}

void PlayerScreen::onNextButtonPressed(void(*f)())
{
	nextButtonAction = f;
}

void PlayerScreen::onPreviousPressed(void(*f)())
{
	previousButtonAction = f;
}

void PlayerScreen::onPlayButtonPressed(void(*f)())
{
	playButtonAction = f;
}

void PlayerScreen::onPauseButtonPressed(void(*f)())
{
	pauseButtonAction = f;
}

void PlayerScreen::onListButtonPressed(void(*f)())
{
	listButtonAction = f;
}

void PlayerScreen::linkVariables(void(*f)())
{
	variableChangeAction = f;
}

void PlayerScreen::drawListButton(int cx, int cy, int w, int h, unsigned int color)
{
	int margin = 5;
	int rh = (h - margin * 2) / 3;
	for (byte i = 0; i < 3; i++)
	{
		tft->fillRoundRect(cx - w / 2, (cy - h / 2) + i * (margin + rh), w, rh, 2, color);
	}
}

void PlayerScreen::drawProgressBar(int x1, int x2, int y, int h, float p, unsigned int color, bool refresh)
{
	if (refresh)
	{
		tft->fillRoundRect(x1, y - h / 2, x2 - x1, h, h / 2, GRAY);
	}
	int progressWidth = (int)mapFloat(p, 0, 1, x1, x2);
	progressWidth = constrain(progressWidth, 0, x2 - x1);
	tft->fillRoundRect(x1 + 2, y - (h / 2) + 2, progressWidth - 4, h - 4, (h - 4) / 2, color);
}

void PlayerScreen::displayTitle()
{
	tft->fillRect(0, 0, tft->width(), 40, backgroundColor);
	tft->setCursor(10, 10);
	tft->setTextSize(3);

	tft->setTextColor(themeColor);
	tft->print(titles[curSong]);
}

void PlayerScreen::drawPlayButton(int cx, int cy, unsigned int color)
{
	tft->fillTriangle(cx - buttonSize / 2, cy - buttonSize / 2, cx + buttonSize / 2, cy, cx - buttonSize / 2, cy + buttonSize / 2, color);
}

void PlayerScreen::drawPauseButton(int cx, int cy, unsigned int color)
{
	tft->fillRect(cx - buttonSize / 2, cy - buttonSize / 2, 15, buttonSize, color);
	tft->fillRect(cx + (buttonSize / 2) - 15, cy - buttonSize / 2, 15, buttonSize, color);
}

int PlayerScreen::drawNextButton(int x, int y, int h, unsigned int color)
{
	int w = (h * 2) - 18;
	tft->fillTriangle(x, y, x + h, y + h / 2, x, y + h, color);
	x += h - 20;
	tft->fillTriangle(x, y, x + h, y + h / 2, x, y + h, color);
	x += h - 5;
	tft->fillRect(x, y, 7, h, color);
	return w;
}

int PlayerScreen::drawPreviousButton(int x, int y, int h, unsigned int color)
{
	int w = (h * 2) - 18;
	tft->fillRect(x, y, 7, h, color);
	x += 2;
	tft->fillTriangle(x, y + h / 2, x + h, y, x + h, y + h, color);
	x += h - 20;
	tft->fillTriangle(x, y + h / 2, x + h, y, x + h, y + h, color);
	return w;
}

float PlayerScreen::mapFloat(double x, double in_min, double in_max, double out_min, double out_max)
{
	return (int)((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);
}