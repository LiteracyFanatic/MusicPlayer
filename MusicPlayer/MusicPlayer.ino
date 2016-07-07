#include <Tone.h>
#include <gfxfont.h>
#include "Song.h"
#include "Songs.h"

// IMPORTANT: Adafruit_TFTLCD LIBRARY MUST BE SPECIFICALLY
// CONFIGURED FOR EITHER THE TFT SHIELD OR THE BREAKOUT BOARD.
// SEE RELEVANT COMMENTS IN Adafruit_TFTLCD.h FOR SETUP.
//Technical support:goodtft@163.com

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>

// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

// When using the BREAKOUT BOARD only, use these 8 data lines to the LCD:
// For the Arduino Uno, Duemilanove, Diecimila, etc.:
//   D0 connects to digital pin 8  (Notice these are
//   D1 connects to digital pin 9   NOT in order!)
//   D2 connects to digital pin 2
//   D3 connects to digital pin 3
//   D4 connects to digital pin 4
//   D5 connects to digital pin 5
//   D6 connects to digital pin 6
//   D7 connects to digital pin 7
// For the Arduino Mega, use digital pins 22 through 29
// (on the 2-row header at the end of the board).

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940

#define MINPRESSURE 10
#define MAXPRESSURE 1000

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define GRAY    0xBDF7
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
// If using the shield, all control and data lines are fixed, and
// a simpler declaration can optionally be used:
// Adafruit_TFTLCD tft;

const struct Song* songList[] =
{
	&bachPartita2GigaSong,
	&bachDoubleSong,
	&bachSong,
	&castleSong,
	&marioSong,
	&nyanCatSong,
	&undertaleSong
};

int cx;
int cy;
int buttonSize = 50;

Adafruit_GFX_Button playPauseButton;
Adafruit_GFX_Button nextButton;
Adafruit_GFX_Button previousButton;
Adafruit_GFX_Button listButton;

int nextButtonX;
int nextButtonY;
int nextButtonW;
int previousButtonX;
int previousButtonY;
int previousButtonW;

MusicPlayer mp = MusicPlayer(songList, sizeof(songList) / sizeof(unsigned int));
#define TONE_1_PIN 22
#define TONE_2_PIN 24

char* titles[7] =
{
	"Partita 2 Giga",
	"Bach Double",
	"Random Bach",
	"Mario Castle",
	"Mario Tune",
	"Nyan Cat",
	"Bonetrousle"
};

Adafruit_GFX_Button songButtons[5];
Adafruit_GFX_Button upButton;
Adafruit_GFX_Button downButton;
Adafruit_GFX_Button cdButton;

byte curPage = 0;
byte numberOfPages;

void setup()
{
	Serial.begin(9600);

	tft.reset();

	uint16_t identifier = tft.readID();
	if (identifier == 0x9325 ||
		identifier == 0x9328 ||
		identifier == 0x4535 ||
		identifier == 0x7575 ||
		identifier == 0x9341 ||
		identifier == 0x8357)
	{

	}
	else if (identifier == 0x0101)
	{
		identifier = 0x9341;
	}
	else {
		identifier = 0x9341;
	}

	tft.begin(identifier);

	tft.setRotation(3);

	cx = tft.width() / 2;
	cy = tft.height() / 2;

	tft.fillScreen(BLACK);

	tft.fillScreen(BLACK);

	drawArduinoLogo(BLUE);

	waitForTap(5000);

	numberOfPages = ceil((float)mp.numberOfSongs / 5);

	mp.init(22, 24);
	mp.currentSong(0);

	initPlayerScreen();
}

unsigned long t = 0;

enum Screen
{
	PLAYER,
	SONGLIST
};

enum Screen currentScreen = PLAYER;

void loop()
{
	switch (currentScreen)
	{
	case PLAYER:
		updatePlayerScreen();
		break;
	case SONGLIST:
		updateSongListScreen();
		break;
	default:
		break;
	}

	mp.run();

	if (mp.songDone)
	{
		if (t == 0)
		{
			t = millis() + (unsigned long)1500;
		}
		if (millis() >= t && !mp.paused)
		{
			mp.nextSong();
			mp.songDone = false;
		}
	}
	else
	{
		t = 0;
	}

	delay(10);
}


void drawCD(int cx, int cy, int r, unsigned int color, bool pressed)
{
	if (pressed)
	{
		tft.fillCircle(cx, cy, r, GRAY);
		tft.drawCircle(cx, cy, r, color);
		tft.fillCircle(cx, cy, r / 2, color);
		tft.fillCircle(cx, cy, r / 5, BLACK);
	}
	else
	{
		tft.fillCircle(cx, cy, r, color);
		tft.drawCircle(cx, cy, r, GRAY);
		tft.fillCircle(cx, cy, r / 2, GRAY);
		tft.fillCircle(cx, cy, r / 5, BLACK);
	}
}

void initSongListScreen(byte page)
{
	tft.fillScreen(BLACK);
	int margin = 10;

	int w = tft.width() - 2 * margin;
	int h = tft.height() - 2 * margin;

	int buttonHeight = (h - margin * 4) / 5;
	int buttonWidth = w * ((float)2 / 3);
	int buttonCenterx = margin + buttonWidth / 2;

	int tSide = 50;
	int tcx = margin + w * ((float)5 / 6);
	int tcy1 = (tft.height() / 2) - 70;
	int tcy2 = (tft.height() / 2) + 70;

	for (byte i = 0; i < 5; i++)
	{
		songButtons[i].initButton(&tft, buttonCenterx, margin + (buttonHeight / 2) + i * (margin + buttonHeight), buttonWidth, buttonHeight, WHITE, WHITE, BLACK, titles[i + 5 * page], 2);
	}

	for (byte i = 0; i < 5; i++)
	{
		if (5 * page + i < mp.numberOfSongs)
		{
			songButtons[i].drawButton();
		}
	}

	upButton.initButton(&tft, tcx, tcy1, tSide, tSide * cos(radians(30)), WHITE, BLUE, WHITE, NULL, 0);
	downButton.initButton(&tft, tcx, tcy2, tSide, tSide * cos(radians(30)), WHITE, BLUE, WHITE, NULL, 0);

	if (page < numberOfPages - 1)
	{
		drawDownButton(tcx, tcy2, tSide, WHITE);
	}

	if (page > 0)
	{
		drawUpButton(tcx, tcy1, tSide, WHITE);
	}

	cdButton.initButton(&tft, tcx, cy, 60, 60, WHITE, BLUE, WHITE, NULL, 0);
	drawCD(tcx, cy, 30, WHITE, false);

}

void updateSongListScreen()
{
	int margin = 10;

	int w = tft.width() - 2 * margin;
	int h = tft.height() - 2 * margin;

	int buttonHeight = (h - margin * 4) / 5;
	int buttonWidth = w * ((float)2 / 3);
	int buttonCenterx = margin + buttonWidth / 2;

	int tSide = 50;
	int tcx = margin + w * ((float)5 / 6);
	int tcy1 = (tft.height() / 2) - 70;
	int tcy2 = (tft.height() / 2) + 70;

	digitalWrite(13, HIGH);
	TSPoint p = ts.getPoint();
	digitalWrite(13, LOW);

	// if sharing pins, you'll need to fix the directions of the touchscreen pins
	//pinMode(XP, OUTPUT);
	pinMode(XM, OUTPUT);
	pinMode(YP, OUTPUT);
	//pinMode(YM, OUTPUT);


	if (p.z > MINPRESSURE && p.z < MAXPRESSURE)
	{
		int tempPY = p.y;
		p.y = (map(p.x, TS_MINX, TS_MAXX, 0, tft.height()));
		p.x = (map(tempPY, TS_MINY, TS_MAXY, 0, tft.width()));
	}
	else
	{
		p.x = 0;
		p.y = 0;
	}

	for (byte i = 0; i < 5; i++)
	{
		if (5 * curPage + i < mp.numberOfSongs)
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

			mp.currentSong((curPage * 5) + i);

			mp.play();
			initPlayerScreen();
			currentScreen = PLAYER;
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
				initSongListScreen(--curPage);
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
				initSongListScreen(++curPage);
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
		drawCD(tcx, cy, 30, WHITE, true);
	}

	if (cdButton.justReleased())
	{
		drawCD(tcx, cy, 30, WHITE, false);
		initPlayerScreen();
		currentScreen = PLAYER;
		return;
	}
}

void displaySongs()
{

}

void drawUpButton(int cx, int cy, int ts, unsigned int color)
{
	int th = ts * cos(radians(30));
	tft.fillTriangle(cx - ts / 2, cy + th / 2, cx + ts / 2, cy + th / 2, cx, cy - th / 2, color);
}

void drawDownButton(int cx, int cy, int ts, unsigned int color)
{
	int th = ts * cos(radians(30));
	tft.fillTriangle(cx - ts / 2, cy - th / 2, cx + ts / 2, cy - th / 2, cx, cy + th / 2, color);
}


void drawPlayButton(int cx, int cy, unsigned int color)
{
	tft.fillTriangle(cx - buttonSize / 2, cy - buttonSize / 2, cx + buttonSize / 2, cy, cx - buttonSize / 2, cy + buttonSize / 2, color);
}

void drawPauseButton(int cx, int cy, unsigned int color)
{
	tft.fillRect(cx - buttonSize / 2, cy - buttonSize / 2, 15, buttonSize, color);
	tft.fillRect(cx + (buttonSize / 2) - 15, cy - buttonSize / 2, 15, buttonSize, color);
}

int drawNextButton(int x, int y, int h, unsigned int color)
{
	int w = (h * 2) - 18;
	tft.fillTriangle(x, y, x + h, y + h / 2, x, y + h, color);
	x += h - 20;
	tft.fillTriangle(x, y, x + h, y + h / 2, x, y + h, color);
	x += h - 5;
	tft.fillRect(x, y, 7, h, color);
	return w;
}

int drawPreviousButton(int x, int y, int h, unsigned int color)
{
	int w = (h * 2) - 18;
	tft.fillRect(x, y, 7, h, color);
	x += 2;
	tft.fillTriangle(x, y + h / 2, x + h, y, x + h, y + h, color);
	x += h - 20;
	tft.fillTriangle(x, y + h / 2, x + h, y, x + h, y + h, color);
	return w;
}

void drawArduinoLogo(unsigned int color)
{
	int outerRadius = 80;
	int innderRadius = outerRadius - 30;
	int overlap = 10;
	int leftCenterX = cx - (outerRadius - overlap);
	int rightCenterX = cx + (outerRadius - overlap);
	int centerY = cy - 30;

	tft.fillCircle(leftCenterX, centerY, outerRadius, color);
	tft.fillCircle(rightCenterX, centerY, outerRadius, color);
	tft.fillCircle(leftCenterX, centerY, innderRadius, BLACK);
	tft.fillCircle(rightCenterX, centerY, innderRadius, BLACK);

	tft.fillRect(leftCenterX - 25, centerY - 7, 50, 14, color);

	tft.fillRect(rightCenterX - 20, centerY - 7, 40, 14, color);
	tft.fillRect(rightCenterX - 7, centerY - 20, 14, 40, color);

	tft.setCursor(5, 185);
	tft.setTextSize(6);
	tft.setTextWrap(false);
	tft.setTextColor(color);

	char text[] = "Noteduino";

	delay(400);

	for (byte i = 0; i < sizeof(text) / sizeof(char) - 1; i++)
	{
		delay(50);
		tft.print(text[i]);
	}
}

void drawProgressBar(int x1, int x2, int y, int h, float p, unsigned int color, bool refresh)
{
	if (refresh)
	{
		tft.fillRoundRect(x1, y - h / 2, x2 - x1, h, h / 2, GRAY);
	}
	int progressWidth = (int)mapFloat(p, 0, 1, x1, x2);
	progressWidth = constrain(progressWidth, 0, x2 - x1);
	tft.fillRoundRect(x1 + 2, y - (h / 2) + 2, progressWidth - 4, h - 4, (h - 4) / 2, color);
}

void displayTitle()
{
	tft.fillRect(0, 0, tft.width(), 40, BLACK);
	tft.setCursor(10, 10);
	tft.setTextSize(3);

	tft.setTextColor(WHITE);
	tft.print(titles[mp.currentSong()]);
	/*for (byte i = 0; i < 9; i++)
	{
		tft.print(mp.title()[i]);
	}

	Serial.println(mp.title());*/

}

byte prevSong = 0;

float prog = 0;


void initPlayerScreen()
{
	tft.fillScreen(BLACK);

	if (mp.paused)
	{
		drawPlayButton(cx, cy - 20, WHITE);
	}
	else
	{
		drawPauseButton(cx, cy - 20, WHITE);
	}

	playPauseButton.initButton(&tft, cx, cy - 20, buttonSize, buttonSize, WHITE, BLUE, WHITE, NULL, 3);

	nextButtonX = tft.width() - (buttonSize * 2) - 2;
	nextButtonY = cy - (buttonSize / 2) - 20;
	previousButtonX = 20;
	previousButtonY = cy - (buttonSize / 2) - 20;

	nextButtonW = drawNextButton(nextButtonX, nextButtonY, buttonSize, WHITE);
	previousButtonW = drawPreviousButton(previousButtonX, previousButtonY, buttonSize, WHITE);

	nextButton.initButton(&tft, nextButtonX + nextButtonW / 2, nextButtonY + buttonSize / 2, nextButtonW, buttonSize, WHITE, BLUE, WHITE, NULL, 3);

	previousButton.initButton(&tft, previousButtonX + previousButtonW / 2, previousButtonY + buttonSize / 2, previousButtonW, buttonSize, WHITE, BLUE, WHITE, NULL, 3);

	displayTitle();
	drawProgressBar(10, tft.width() - 10, 155, 10, prog, WHITE, true);

	listButton.initButton(&tft, cx, tft.height() - 30, 60, 40, WHITE, BLACK, WHITE, NULL, 0);
	drawListButton(cx, tft.height() - 40, 60, 40, WHITE);
}

void drawListButton(int cx, int cy, int w, int h, unsigned int color)
{
	int margin = 5;
	int rh = (h - margin * 2) / 3;
	for (byte i = 0; i < 3; i++)
	{
		tft.fillRoundRect(cx - w / 2, (cy - h / 2) + i * (margin + rh), w, rh, 2, color);
	}
}

void updatePlayerScreen()
{
	digitalWrite(13, HIGH);
	TSPoint p = ts.getPoint();
	digitalWrite(13, LOW);

	// if sharing pins, you'll need to fix the directions of the touchscreen pins
	//pinMode(XP, OUTPUT);
	pinMode(XM, OUTPUT);
	pinMode(YP, OUTPUT);
	//pinMode(YM, OUTPUT);


	if (p.z > MINPRESSURE && p.z < MAXPRESSURE)
	{
		int tempPY = p.y;
		p.y = (map(p.x, TS_MINX, TS_MAXX, 0, tft.height()));
		p.x = (map(tempPY, TS_MINY, TS_MAXY, 0, tft.width()));
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
		if (mp.paused)
		{
			drawPlayButton(cx, cy - 20, BLACK);
		}
		drawPauseButton(cx, cy - 20, WHITE);
		mp.nextSong();
		mp.play();
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
		if (mp.paused)
		{
			drawPlayButton(cx, cy - 20, BLACK);
		}
		drawPauseButton(cx, cy - 20, WHITE);
		mp.previousSong();
		mp.play();
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
		if (mp.paused)
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
		if (mp.paused)
		{
			mp.play();
			drawPlayButton(cx, cy - 20, BLACK);
			drawPauseButton(cx, cy - 20, WHITE);
		}
		else
		{
			mp.pause();
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
		drawListButton(cx, tft.height() - 40, 60, 40, GRAY);
	}

	if (listButton.justReleased())
	{
		mp.pause();
		curPage = mp.currentSong() / 5;
		initSongListScreen(curPage);
		//mp.play();
		currentScreen = SONGLIST;
		return;
	}

	if (mp.currentSong() != prevSong)
	{
		displayTitle();
		prevSong = mp.currentSong();
	}

	if (prog > mp.percentComplete())
	{
		prog = 0;
		drawProgressBar(10, tft.width() - 10, 155, 10, prog, WHITE, true);
	}
	else
	{
		prog = mp.percentComplete();
		drawProgressBar(10, tft.width() - 10, 155, 10, prog, WHITE, false);
	}
}


void waitForTap()
{
	bool done = false;
	while (!done)
	{
		digitalWrite(13, HIGH);
		TSPoint p = ts.getPoint();
		digitalWrite(13, LOW);

		// if sharing pins, you'll need to fix the directions of the touchscreen pins
		//pinMode(XP, OUTPUT);
		pinMode(XM, OUTPUT);
		pinMode(YP, OUTPUT);
		//pinMode(YM, OUTPUT);

		if (p.z > MINPRESSURE && p.z < MAXPRESSURE)
		{
			done = true;
		}
	}
}

void waitForTap(unsigned long waitTime)
{
	bool done = false;
	unsigned long ti = millis();
	while (!done)
	{
		digitalWrite(13, HIGH);
		TSPoint p = ts.getPoint();
		digitalWrite(13, LOW);

		// if sharing pins, you'll need to fix the directions of the touchscreen pins
		//pinMode(XP, OUTPUT);
		pinMode(XM, OUTPUT);
		pinMode(YP, OUTPUT);
		//pinMode(YM, OUTPUT);

		if (p.z > MINPRESSURE && p.z < MAXPRESSURE)
		{
			done = true;
		}

		if (waitTime != NULL)
		{
			if (millis() >= ti + waitTime)
			{
				done = true;
			}
		}
	}
}


float mapFloat(double x, double in_min, double in_max, double out_min, double out_max)
{
	return (int)((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);
}


