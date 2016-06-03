// Song.h

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

#endif

