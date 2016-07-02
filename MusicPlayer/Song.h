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
};

class MusicPlayer
{
public:
	MusicPlayer(const struct Song* *sl, byte n);
	void start(byte t1, byte t2);
	byte numberOfSongs;
	const struct Song* *songList;
	byte curSong;
	void playNextSong();
	void playPreviousSong();
	void playSong(const struct Song* song);
	void playCurrentSong();
	Tone tone1;
	Tone tone2;


private:

};



#endif

