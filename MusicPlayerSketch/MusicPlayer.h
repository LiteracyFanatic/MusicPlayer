// MusicPlayer.h
#include <Tone.h>

#ifndef _MUSICPLAYER_h
#define _MUSICPLAYER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

struct Song
{
	const unsigned int* notes1;
	const unsigned int* times1;
	const unsigned int* notes2;
	const unsigned int* times2;
	const unsigned int notes1Length;
	const unsigned int notes2Length;
};

class MusicPlayer
{
public:
	MusicPlayer(const struct Song* *sl, byte n);
	void attachPins(byte t1, byte t2);
	void play();
	void pause();
	void nextSong();
	void previousSong();
	void process();
	byte getCurrentSong();
	void setCurrentSong(byte cs);
	unsigned long getCurrentSongDurration();
	bool donePlaying();
	bool isPaused();
	float percentComplete();
	unsigned long calculateElapsedTime();
	void playSong(const struct Song* song);

private:
	const struct Song* *songList;
	Tone tone1;
	Tone tone2;
	byte numberOfSongs;
	byte currentSong = 0;
	bool buzzer1Done = false;
	bool buzzer2Done = false;
	bool songDone = false;
	bool paused = true;
	int note1Index = 0;
	int note2Index = 0;
	unsigned long ti = NULL;
	unsigned long t1 = 0;
	unsigned long t2 = 0;
	unsigned long dt = 0;
	unsigned long pausedTime = 0;
	unsigned long currentSongDuration;
	void calculateDuration(const struct Song* song);
};

#endif
