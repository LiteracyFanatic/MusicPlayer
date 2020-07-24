// 
// 
// 

#include "SongListScreen.h"

SongListScreen::SongListScreen(Adafruit_GFX *gfx, TouchScreen *touchScreen, char* t[], byte n)
{
	tft = gfx;
	ts = touchScreen;
	titles = t;
	numberOfItems = n;
	numberOfPages = ceil((float)numberOfItems / itemsPerPage);
}

void SongListScreen::init()
{
	cx = tft->width() / 2;
	cy = tft->height() / 2;

	margin = 10;
	w = tft->width() - 2 * margin;
	h = tft->height() - 2 * margin;
	buttonHeight = (h - margin * 4) / itemsPerPage;
	buttonWidth = w * ((float)2 / 3);
	buttonCenterx = margin + buttonWidth / 2;
	tSide = 50;
	tcx = margin + w * ((float)5 / 6);
	tcy1 = (tft->height() / 2) - 70;
	tcy2 = (tft->height() / 2) + 70;
	cdRadius = 30;
}

void SongListScreen::draw()
{
	variableChangeAction();
	tft->fillScreen(backgroundColor);

	for (byte i = 0; i < itemsPerPage; i++)
	{
		if (itemsPerPage * currentPage + i < numberOfItems)
		{
			listButtons[i].initButton(tft, buttonCenterx, margin + (buttonHeight / 2) + i * (margin + buttonHeight), buttonWidth, buttonHeight, themeColor, themeColor, backgroundColor, titles[i + itemsPerPage * currentPage], 2);
		}
		else
		{
			listButtons[i].initButton(tft, 0, 0, 0, 0, themeColor, themeColor, backgroundColor, NULL, 0);
		}
	}

	for (byte i = 0; i < itemsPerPage; i++)
	{
		if (itemsPerPage * currentPage + i < numberOfItems)
		{
			listButtons[i].drawButton();
		}
	}

	upButton.initButton(tft, tcx, tcy1, tSide, tSide * cos(radians(30)), themeColor, BLUE, themeColor, NULL, 0);
	downButton.initButton(tft, tcx, tcy2, tSide, tSide * cos(radians(30)), themeColor, BLUE, themeColor, NULL, 0);

	if (currentPage < numberOfPages - 1)
	{
		drawDownButton(tcx, tcy2, tSide, themeColor);
	}

	if (currentPage > 0)
	{
		drawUpButton(tcx, tcy1, tSide, themeColor);
	}

	cdButton.initButton(tft, tcx, cy, cdRadius * 2, cdRadius * 2, themeColor, BLUE, themeColor, NULL, 0);
	drawCD(tcx, cy, 30, themeColor, false);
}

void SongListScreen::update()
{
	variableChangeAction();

	TSPoint p = readPoint();

	for (byte i = 0; i < itemsPerPage; i++)
	{
		if (itemsPerPage * currentPage + i < numberOfItems)
		{
			if (listButtons[i].contains(p.x, p.y))
			{
				listButtons[i].press(true);
			}
			else
			{
				listButtons[i].press(false);
			}
		}
	}

	for (byte i = 0; i < itemsPerPage; i++)
	{
		if (itemsPerPage * currentPage + i < numberOfItems)
		{
			if (listButtons[i].justPressed())
			{
				listButtons[i].drawButton(true);
			}

			if (listButtons[i].justReleased())
			{
				listButtons[i].drawButton();
				selectedItem = (currentPage * itemsPerPage) + i;
				listButtonAction();
				return;
			}
		}
	}

	if (currentPage > 0)
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
			if (currentPage > 0)
			{
				currentPage--;
				draw();
			}
		}
	}

	if (currentPage < numberOfPages - 1)
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
			if (currentPage < numberOfPages - 1)
			{
				currentPage++;
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
		drawCD(tcx, cy, cdRadius, themeColor, true);
	}

	if (cdButton.justReleased())
	{
		drawCD(tcx, cy, cdRadius, themeColor, false);
		cdButtonAction();
		return;
	}
}

void SongListScreen::onListButtonPressed(void(*f)())
{
	listButtonAction = f;
}

void SongListScreen::onCDButtonPressed(void(*f)())
{
	cdButtonAction = f;
}

void SongListScreen::linkVariables(void(*f)())
{
	variableChangeAction = f;
}

byte SongListScreen::getSelectedItem()
{
	return selectedItem;
}

byte SongListScreen::getItemsPerPage()
{
	return itemsPerPage;
}

byte SongListScreen::getCurrentPage()
{
	return currentPage;
}

byte SongListScreen::setCurrentPage(byte c)
{
	if (c < numberOfPages)
	{
		currentPage = c;
	}
}

byte SongListScreen::getNumberOfPages()
{
	return numberOfPages;
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