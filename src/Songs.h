// Songs.h

#ifndef _SONGS_h
#define _SONGS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

extern const unsigned int prestoEModerato1Notes[] PROGMEM;
extern const unsigned int prestoEModerato1Times[];
extern const unsigned int prestoEModerato2Notes[] PROGMEM;
extern const unsigned int prestoEModerato2Times[] PROGMEM;
extern const struct Song prestoEModeratoSong PROGMEM;

extern const unsigned int larghetto1Notes[] PROGMEM;
extern const unsigned int larghetto1Times[];
extern const unsigned int larghetto2Notes[] PROGMEM;
extern const unsigned int larghetto2Times[] PROGMEM;
extern const struct Song larghettoSong PROGMEM;

extern const unsigned int zelda1Notes[] PROGMEM;
extern const unsigned int zelda1Times[];
extern const unsigned int zelda2Notes[] PROGMEM;
extern const unsigned int zelda2Times[] PROGMEM;
extern const struct Song zeldaSong PROGMEM;

extern const unsigned int songOfStorms1Notes[] PROGMEM;
extern const unsigned int songOfStorms1Times[];
extern const unsigned int songOfStorms2Notes[] PROGMEM;
extern const unsigned int songOfStorms2Times[] PROGMEM;
extern const struct Song songOfStormsSong PROGMEM;

extern const unsigned int tetris1Notes[] PROGMEM;
extern const unsigned int tetris1Times[] PROGMEM;
extern const unsigned int tetris2Notes[] PROGMEM;
extern const unsigned int tetris2Times[] PROGMEM;
extern const struct Song tetrisSong PROGMEM;

extern const unsigned int castle1Notes[] PROGMEM;
extern const unsigned int castle1Times[] PROGMEM;
extern const unsigned int castle2Notes[] PROGMEM;
extern const unsigned int castle2Times[] PROGMEM;
extern const struct Song castleSong PROGMEM;

extern const unsigned int mario1Notes[] PROGMEM;
extern const unsigned int mario1Times[] PROGMEM;
extern const unsigned int mario2Notes[] PROGMEM;
extern const unsigned int mario2Times[] PROGMEM;
extern const struct Song marioSong PROGMEM;

extern const unsigned int bachDouble1Notes[] PROGMEM;
extern const unsigned int bachDouble1Times[] PROGMEM;
extern const unsigned int bachDouble2Notes[] PROGMEM;
extern const unsigned int bachDouble2Times[] PROGMEM;
extern const struct Song bachDoubleSong PROGMEM;

extern const unsigned int bachNotes[] PROGMEM;
extern const unsigned int bachTimes[] PROGMEM;
extern const unsigned int bachNotesLength PROGMEM;
extern const struct Song bachSong PROGMEM;

extern const unsigned int undertale1Notes[] PROGMEM;
extern const unsigned int undertale1Times[] PROGMEM;
extern const unsigned int undertale2Notes[] PROGMEM;
extern const unsigned int undertale2Times[] PROGMEM;
extern const struct Song undertaleSong PROGMEM;

extern const unsigned int nyanCatNotes[] PROGMEM;
extern const unsigned int nyanCatTimes[] PROGMEM;
extern const unsigned int nyanCatNotesLength PROGMEM;
extern const struct Song nyanCatSong PROGMEM;

extern const unsigned int bachPartita2GigaNotes[] PROGMEM;
extern const unsigned int bachPartita2GigaTimes[] PROGMEM;
extern const unsigned int bachPartita2GigaLength PROGMEM;
extern const struct Song bachPartita2GigaSong PROGMEM;

#endif

