#include "SplashScreen.h"
#include "MusicPlayer.h"
#include "SongListScreen.h"
#include "PlayerScreen.h"
#include "Screen.h"
#include <Tone.h>
#include <gfxfont.h>
#include "MusicPlayer.h"
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

MusicPlayer mp = MusicPlayer(songList, sizeof(songList) / sizeof(songList[0]));
#define TONE_1_PIN 22
#define TONE_2_PIN 24

char* titles[7] =
{
	"Bach Partita 2",
	"Bach Double",
	"Bach Presto",
	"Mario Castle",
	"Mario Tune",
	"Nyan Cat",
	"Bonetrousle"
};

enum Screens
{
	PLAYER,
	SONGLIST
} currentScreen;

SplashScreen splashScreen = SplashScreen(&tft, &ts);

PlayerScreen playerScreen = PlayerScreen(&tft, &ts, titles, sizeof(titles) / sizeof(titles[0]));

SongListScreen songListScreen = SongListScreen(&tft, &ts, titles, sizeof(titles) / sizeof(titles[0]));

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

	playerScreen.init();
	playerScreen.onNextButtonPressed([]() {
		mp.nextSong();
		mp.play();
	});
	playerScreen.onPreviousPressed([]() {
		mp.previousSong();
		mp.play();
	});
	playerScreen.onPlayButtonPressed([]() {
		mp.play();
	});
	playerScreen.onPauseButtonPressed([]() {
		mp.pause();
	});
	playerScreen.onListButtonPressed([]() {
		mp.pause();
		songListScreen.curPage = mp.currentSong() / 5;
		songListScreen.draw();
		currentScreen = SONGLIST;
	});
	playerScreen.linkVariables([]() {
		playerScreen.paused = mp.paused;
		playerScreen.curSong = mp.currentSong();
		playerScreen.prog = mp.percentComplete();
	});

	songListScreen.init();
	songListScreen.onSongButtonPressed([]() {
		mp.currentSong((songListScreen.curPage * 5) + songListScreen.pressedButton);
		mp.play();
		playerScreen.draw();
		currentScreen = PLAYER;
	});
	songListScreen.onCDButtonPressed([]() {
		playerScreen.draw();
		currentScreen = PLAYER;
	});
	songListScreen.linkVariables([]() {

	});

	mp.init(22, 24);

	splashScreen.draw();

	splashScreen.waitForTap(5000);

	playerScreen.draw();
}

unsigned long t = 0;

void loop()
{
	switch (currentScreen)
	{
	case PLAYER:
		playerScreen.update();
		break;
	case SONGLIST:
		songListScreen.update();
		break;
	}

	mp.run();

	if (mp.songDone)
	{
		if (t == 0)
		{
			t = millis() + 1500;
		}
		if (millis() >= t && !mp.paused)
		{
			mp.nextSong();
		}
	}
	else
	{
		t = 0;
	}

	delay(10);
}


