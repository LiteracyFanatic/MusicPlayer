// 
// 
// 

#include "PlayerScreen.h"


PlayerScreen::PlayerScreen(Adafruit_GFX *gfx, TouchScreen *touchScreen, MusicPlayer *musicPlayer)
{
	tft = gfx;
	ts = touchScreen;
	mp = musicPlayer;
}

void PlayerScreen::init()
{
	cx = tft->width() / 2;
	cy = tft->height() / 2;

	nextButtonX = tft->width() - (buttonSize * 2) - 2;
	nextButtonY = cy - (buttonSize / 2) - 20;
	previousButtonX = 20;
	previousButtonY = cy - (buttonSize / 2) - 20;

	nextButtonW = drawNextButton(nextButtonX, nextButtonY, buttonSize, WHITE);
	previousButtonW = drawPreviousButton(previousButtonX, previousButtonY, buttonSize, WHITE);
}

void PlayerScreen::draw()
{
	tft->fillScreen(BLACK);

	if (mp->paused)
	{
		drawPlayButton(cx, cy - 20, WHITE);
	}
	else
	{
		drawPauseButton(cx, cy - 20, WHITE);
	}

	playPauseButton.initButton(tft, cx, cy - 20, buttonSize, buttonSize, WHITE, BLUE, WHITE, NULL, 3);

	nextButtonX = tft->width() - (buttonSize * 2) - 2;
	nextButtonY = cy - (buttonSize / 2) - 20;
	previousButtonX = 20;
	previousButtonY = cy - (buttonSize / 2) - 20;

	nextButtonW = drawNextButton(nextButtonX, nextButtonY, buttonSize, WHITE);
	previousButtonW = drawPreviousButton(previousButtonX, previousButtonY, buttonSize, WHITE);

	nextButton.initButton(tft, nextButtonX + nextButtonW / 2, nextButtonY + buttonSize / 2, nextButtonW, buttonSize, WHITE, BLUE, WHITE, NULL, 3);

	previousButton.initButton(tft, previousButtonX + previousButtonW / 2, previousButtonY + buttonSize / 2, previousButtonW, buttonSize, WHITE, BLUE, WHITE, NULL, 3);

	displayTitle();
	drawProgressBar(10, tft->width() - 10, 155, 10, prog, WHITE, true);

	listButton.initButton(tft, cx, tft->height() - 30, 60, 40, WHITE, BLACK, WHITE, NULL, 0);
	drawListButton(cx, tft->height() - 40, 60, 40, WHITE);
}

void PlayerScreen::update()
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
		drawNextButton(nextButtonX, nextButtonY, buttonSize, WHITE);
		if (mp->paused)
		{
			drawPlayButton(cx, cy - 20, BLACK);
		}
		drawPauseButton(cx, cy - 20, WHITE);
		mp->nextSong();
		mp->play();
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
		drawPreviousButton(previousButtonX, previousButtonY, buttonSize, WHITE);
		if (mp->paused)
		{
			drawPlayButton(cx, cy - 20, BLACK);
		}
		drawPauseButton(cx, cy - 20, WHITE);
		mp->previousSong();
		mp->play();
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
		if (mp->paused)
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
		if (mp->paused)
		{
			mp->play();
			drawPlayButton(cx, cy - 20, BLACK);
			drawPauseButton(cx, cy - 20, WHITE);
		}
		else
		{
			mp->pause();
			drawPauseButton(cx, cy - 20, BLACK);
			drawPlayButton(cx, cy - 20, WHITE);
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
		mp->pause();
		//curPage = mp->currentSong() / 5;
		//initSongListScreen(curPage);
		//mp->play();
		//currentScreen = SONGLIST;
		return;
	}

	if (mp->currentSong() != prevSong)
	{
		displayTitle();
		prevSong = mp->currentSong();
	}

	if (prog > mp->percentComplete())
	{
		prog = 0;
		drawProgressBar(10, tft->width() - 10, 155, 10, prog, WHITE, true);
	}
	else
	{
		prog = mp->percentComplete();
		drawProgressBar(10, tft->width() - 10, 155, 10, prog, WHITE, false);
	}
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
	tft->fillRect(0, 0, tft->width(), 40, BLACK);
	tft->setCursor(10, 10);
	tft->setTextSize(3);

	tft->setTextColor(WHITE);
	tft->print(titles[mp->currentSong()]);
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