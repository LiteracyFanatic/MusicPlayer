// Song.h
#include <Tone.h>

#ifndef _SONG_h
#define _SONG_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

struct Song
{
	const unsigned int *notes1;
	const unsigned int *times1;
	const unsigned int *notes2;
	const unsigned int *times2;
	const unsigned int songLength;
	const char* title;
	const unsigned int titleLength;
};

class MusicPlayer
{
public:
	MusicPlayer(const struct Song* *sl, byte n);
	void start(byte t1, byte t2);
	byte numberOfSongs;
	const struct Song* *songList;
	void currentSong(byte cs);
	byte currentSong();
	void nextSong();
	void previousSong();
	void playSong(const struct Song* song);
	void playCurrentSong();
	bool songDone;
	bool paused;
	void play();
	void pause();
	const char* title();

private:
	Tone tone1;
	Tone tone2;
	byte curSong;
	unsigned long ti;
	unsigned long t1;
	unsigned long t2;
	unsigned long dt;
	unsigned long pausedTime;
	int i;
	int j;
};



#endif

