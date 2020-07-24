#include <Tone.h>
#include "MusicPlayer.h"

MusicPlayer::MusicPlayer(const struct Song* *sl, byte n)
{
	songList = sl;
	numberOfSongs = n;
	calculateDuration(songList[currentSong]);
}

void MusicPlayer::attachPins(byte t1, byte t2)
{
	tone1.begin(t1);
	tone2.begin(t2);
}

void MusicPlayer::play()
{
	paused = false;
}

void MusicPlayer::pause()
{
	tone1.stop();
	tone2.stop();
	paused = true;
}

void MusicPlayer::nextSong()
{
	if (currentSong < numberOfSongs - 1)
	{
		setCurrentSong(currentSong + 1);
	}
	else
	{
		setCurrentSong(0);
	}
}

void MusicPlayer::previousSong()
{
	if (currentSong == 0)
	{
		setCurrentSong(numberOfSongs - 1);
	}
	else
	{
		setCurrentSong(currentSong - 1);
	}
}

void MusicPlayer::process()
{
	playSong(songList[currentSong]);
}

byte MusicPlayer::getCurrentSong()
{
	return currentSong;
}

void MusicPlayer::setCurrentSong(byte cs)
{
	if (cs < numberOfSongs)
	{
		currentSong = cs;
		t1 = 0;
		t2 = 0;
		ti = NULL;
		pausedTime = 0;
		note1Index = 0;
		note2Index = 0;
		buzzer1Done = false;
		buzzer2Done = false;
		songDone = false;
		calculateDuration(songList[currentSong]);
	}
}

unsigned long MusicPlayer::getCurrentSongDurration()
{
	return currentSongDuration;
}

bool MusicPlayer::donePlaying()
{
	return songDone;
}

bool MusicPlayer::isPaused()
{
	return paused;
}

float MusicPlayer::percentComplete()
{
	return (float)calculateElapsedTime() / currentSongDuration;
}

void MusicPlayer::calculateDuration(const Song* song)
{
	int noteLen1 = pgm_read_word(&(song->notes1Length));
	int noteLen2 = pgm_read_word(&(song->notes2Length));

	unsigned long d1 = 0;
	unsigned long d2 = 0;

	for (int i = 0; i < noteLen1; i++)
	{
		d1 += pgm_read_word(pgm_read_word(&(song->times1)) + i * sizeof(unsigned int));
	}

	for (int j = 0; j < noteLen2; j++)
	{
		d2 += pgm_read_word(pgm_read_word(&(song->times2)) + j * sizeof(unsigned int));
	}

	currentSongDuration = max(d1, d2);
}

unsigned long MusicPlayer::calculateElapsedTime()
{
	const Song* song = songList[currentSong];

	int noteLen1 = pgm_read_word(&(song->notes1Length));
	int noteLen2 = pgm_read_word(&(song->notes2Length));

	unsigned long d1 = 0;
	unsigned long d2 = 0;

	for (int i = 0; i < note1Index; i++)
	{
		if (i > noteLen1 - 1)
		{
			break;
		}
		d1 += pgm_read_word(pgm_read_word(&(song->times1)) + i * sizeof(unsigned int));
	}

	for (int j = 0; j < note2Index; j++)
	{
		if (j > noteLen2 - 1)
		{
			break;
		}
		d2 += pgm_read_word(pgm_read_word(&(song->times2)) + j * sizeof(unsigned int));
	}

	return max(d1, d2);
}

void MusicPlayer::playSong(const struct Song* song)
{
	if (songDone)
	{
		return;
	}

	if (paused)
	{
		if (ti != NULL)
		{
			pausedTime = dt ? (millis() - dt) : (millis() - ti);
		}
		return;
	}

	if (ti == NULL)
	{
		ti = millis();
	}

	t1 += pausedTime;
	t2 += pausedTime;
	pausedTime = 0;

	bool part1Exists = pgm_read_word(&(song->notes1));
	bool part2Exists = pgm_read_word(&(song->notes2));

	int noteLen1 = pgm_read_word(&(song->notes1Length));
	int noteLen2 = pgm_read_word(&(song->notes2Length));

	dt = millis();

	if (part1Exists && (dt >= t1 + ti))
	{
		if (note1Index > noteLen1 - 1)
		{
			tone1.stop();
			buzzer1Done = true;
		}
		else
		{
			tone1.stop();
			delay(5);

			int curNote1 = pgm_read_word(pgm_read_word(&(song->notes1)) + note1Index * sizeof(unsigned int));
			int curTime1 = pgm_read_word(pgm_read_word(&(song->times1)) + note1Index * sizeof(unsigned int));

			note1Index++;

			t1 += curTime1;
			tone1.play(curNote1, curTime1);
		}
	}

	if (part2Exists && (dt >= t2 + ti))
	{
		if (note2Index > noteLen2 - 1)
		{
			tone2.stop();
			buzzer2Done = true;
		}
		else
		{
			tone2.stop();
			delay(5);

			int curNote2 = pgm_read_word(pgm_read_word(&(song->notes2)) + note2Index * sizeof(unsigned int));
			int curTime2 = pgm_read_word(pgm_read_word(&(song->times2)) + note2Index * sizeof(unsigned int));

			note2Index++;

			t2 += curTime2;
			tone2.play(curNote2, curTime2);
		}
	}

	if ((!part1Exists || buzzer1Done) && (!part2Exists || buzzer2Done))
	{
		songDone = true;
		return;
	}
}