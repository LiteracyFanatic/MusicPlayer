// 
// 
// 

#include "PlayerScreen.h"

PlayerScreen::PlayerScreen(Adafruit_GFX *gfx, TouchScreen *touchScreen, char* t[], byte n)
{
	tft = gfx;
	ts = touchScreen;
	titles = t;
	numberOfPages = n;
}

void PlayerScreen::init()
{
	cx = tft->width() / 2;
	cy = tft->height() / 2;

	playPauseButtonSize = 50;
	playPauseButtonX = cx;
	npButtonH = 50;
	npButtonW = npButtonH * 1.8;
	nextButtonX = tft->width() - (playPauseButtonSize + 10);
	previousButtonX = playPauseButtonSize + 10;
	controlsY = cy - 20;
	progressBarX0 = 10;
	progressBarX1 = tft->width() - 10;
	progressBarY = 155;
	progressBarH = 10;
	listButtonX = cx;
	listButtonY = tft->height() - 30;
	listButtonW = 60;
	listButtonH = 40;
}

void PlayerScreen::draw()
{
	variableChangeAction();
	tft->fillScreen(backgroundColor);

	if (paused)
	{
		drawPlayButton(cx, controlsY, playPauseButtonSize, playPauseButtonSize, themeColor);
	}
	else
	{
		drawPauseButton(cx, controlsY, playPauseButtonSize, playPauseButtonSize, themeColor);
	}

	playPauseButton.initButton(tft, playPauseButtonX, controlsY, playPauseButtonSize, playPauseButtonSize, themeColor, BLUE, themeColor, NULL, 3);



	nextButton.initButton(tft, nextButtonX, controlsY, playPauseButtonSize * 1.8, playPauseButtonSize, themeColor, BLUE, themeColor, NULL, 3);

	previousButton.initButton(tft, previousButtonX, controlsY, playPauseButtonSize * 1.8, playPauseButtonSize, themeColor, BLUE, themeColor, NULL, 3);

	drawNextButton(nextButtonX, controlsY, playPauseButtonSize * 1.8, playPauseButtonSize, themeColor);
	drawPreviousButton(previousButtonX, controlsY, playPauseButtonSize * 1.8, playPauseButtonSize, themeColor);

	displayTitle();

	drawProgressBar(progressBarX0, progressBarX1, progressBarY, progressBarH, prog, themeColor, true);

	listButton.initButton(tft, listButtonX, listButtonY, listButtonW, listButtonH, themeColor, backgroundColor, themeColor, NULL, 0);
	drawListButton(listButtonX, listButtonY, listButtonW, listButtonH, themeColor);
}

void PlayerScreen::update()
{
	variableChangeAction();

	TSPoint p = readPoint();

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
		drawNextButton(nextButtonX, controlsY, playPauseButtonSize * 1.8, playPauseButtonSize, GRAY);
	}

	if (nextButton.justReleased())
	{
		drawNextButton(nextButtonX, controlsY, playPauseButtonSize * 1.8, playPauseButtonSize, themeColor);
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
		drawPreviousButton(previousButtonX, controlsY, playPauseButtonSize * 1.8, playPauseButtonSize, GRAY);
	}

	if (previousButton.justReleased())
	{
		drawPreviousButton(previousButtonX, controlsY, playPauseButtonSize * 1.8, playPauseButtonSize, themeColor);
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
			drawPlayButton(playPauseButtonX, controlsY, playPauseButtonSize, playPauseButtonSize, GRAY);
		}
		else
		{
			drawPauseButton(playPauseButtonX, controlsY, playPauseButtonSize, playPauseButtonSize, GRAY);
		}
	}

	if (playPauseButton.justReleased())
	{
		if (paused)
		{
			playButtonAction();
			drawPlayButton(playPauseButtonX, controlsY, playPauseButtonSize, playPauseButtonSize, backgroundColor);
			drawPauseButton(playPauseButtonX, controlsY, playPauseButtonSize, playPauseButtonSize, themeColor);
		}
		else
		{
			pauseButtonAction();
			drawPauseButton(playPauseButtonX, controlsY, playPauseButtonSize, playPauseButtonSize, backgroundColor);
			drawPlayButton(playPauseButtonX, controlsY, playPauseButtonSize, playPauseButtonSize, themeColor);
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
		drawListButton(listButtonX, listButtonY, listButtonW, listButtonH, GRAY);
	}

	if (listButton.justReleased())
	{
		drawListButton(listButtonX, listButtonY, listButtonW, listButtonH, themeColor);
		listButtonAction();
		return;
	}

	if (currentPage != prevSong)
	{
		displayTitle();
		prevSong = currentPage;
	}

	if (lastProg > prog)
	{
		lastProg = 0;
		drawProgressBar(progressBarX0, progressBarX1, progressBarY, progressBarH, prog, themeColor, true);
	}
	else
	{
		lastProg = prog;
		drawProgressBar(progressBarX0, progressBarX1, progressBarY, progressBarH, prog, themeColor, false);
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

byte PlayerScreen::getCurrentPage()
{
	return currentPage;
}

void PlayerScreen::setCurrentPage(byte cs)
{
	if (cs < numberOfPages)
	{
		currentPage = cs;
	}
}

void PlayerScreen::drawPlayButton(int cx, int cy, int w, int h, unsigned int color)
{
	tft->fillTriangle(cx - w / 2, cy - h / 2, cx + w / 2, cy, cx - w / 2, cy + h / 2, color);
}

void PlayerScreen::drawPauseButton(int cx, int cy, int w, int h, unsigned int color)
{
	tft->fillRect(cx - playPauseButtonSize / 2, cy - playPauseButtonSize / 2, 15, playPauseButtonSize, color);
	tft->fillRect(cx + (playPauseButtonSize / 2) - 15, cy - playPauseButtonSize / 2, 15, playPauseButtonSize, color);
}

void PlayerScreen::drawNextButton(int cx, int cy, int w, int h, unsigned int color)
{
	int x0 = cx - w / 2;
	int y0 = cy - h / 2;
	int x1 = x0 + w;
	int y1 = y0 + h;
	int overLap = w / 10;

	tft->fillTriangle(x0, y0, cx + overLap, cy, x0, y1, color);
	tft->fillTriangle(cx - overLap, y0, x1 - 1, cy, cx - overLap, y1, color);
	tft->fillRect(x1 - 10, y0, 10, h, color);
}

void PlayerScreen::drawPreviousButton(int cx, int cy, int w, int h, unsigned int color)
{
	int x0 = cx - w / 2;
	int y0 = cy - h / 2;
	int x1 = x0 + w;
	int y1 = y0 + h;
	int overLap = w / 10;

	tft->fillRect(x0, y0, 10, h, color);
	tft->fillTriangle(x0 + 1, cy, cx + overLap, y0, cx + overLap, y1, color);
	tft->fillTriangle(cx - overLap, cy, x1, y0, x1, y1, color);
}

void PlayerScreen::displayTitle()
{
	tft->fillRect(0, 0, tft->width(), 40, backgroundColor);
	tft->setCursor(10, 10);
	tft->setTextSize(3);
	tft->setTextColor(themeColor);
	tft->print(titles[currentPage]);
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

float PlayerScreen::mapFloat(double x, double in_min, double in_max, double out_min, double out_max)
{
	return (int)((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);
}