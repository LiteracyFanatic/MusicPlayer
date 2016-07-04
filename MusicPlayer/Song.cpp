#include <Tone.h>
#include "Song.h"

#define dv(v) Serial.print(#v); Serial.print(" = "); Serial.println(v);


MusicPlayer::MusicPlayer(const struct Song* *sl, byte n)
{
	songList = sl;
	numberOfSongs = n;
	curSong = 0;
	t1 = 0;
	t2 = 0;
	ti = NULL;
	paused = 0;
	note1Index = 0;
	note2Index = 0;
	songDone = false;
	paused = true;
	pausedTime = 0;
	dt = 0;
	calculateDuration(songList[curSong]);
}

void MusicPlayer::start(byte t1, byte t2)
{
	tone1.begin(t1);
	tone2.begin(t2);
}

void MusicPlayer::currentSong(byte cs)
{
	curSong = cs;
	t1 = 0;
	t2 = 0;
	ti = NULL;
	note1Index = 0;
	note2Index = 0;
	songDone = false;
	pausedTime = 0;
	calculateDuration(songList[curSong]);
}

byte MusicPlayer::currentSong()
{
	return curSong;
}

void MusicPlayer::nextSong()
{
	if (curSong < numberOfSongs - 1)
	{
		currentSong(currentSong() + 1);
	}
	else
	{
		currentSong(0);
	}
}

void MusicPlayer::previousSong()
{
	if (curSong == 0)
	{
		currentSong(numberOfSongs - 1);
	}
	else
	{
		currentSong(currentSong() - 1);
	}
}

void MusicPlayer::run()
{
	playSong(songList[curSong]);
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

const char* MusicPlayer::title()
{
	int len = pgm_read_word(&(songList[curSong])->titleLength);
	//Serial.println(len);
	char buffer[len];

	for (int i = 0; i < len; i++)
	{
		strcpy_P(buffer, (char*)pgm_read_word(&(songList[curSong])->title));
	}

	//Serial.println(buffer);

	return buffer;
}

float MusicPlayer::percentComplete()
{
	return (float) calculateElapsedTime() / currentSongDuration;
}

void MusicPlayer::calculateDuration(const Song * song)
{
	int noteLen1 = pgm_read_word(&(song->notes1Length));
	int noteLen2 = pgm_read_word(&(song->notes2Length));

	unsigned long d1 = 0;
	unsigned long d2 = 0;

	for (int i = 0; i < noteLen1; i++)
	{
		d1 += pgm_read_word(pgm_read_word(&(song->times1)) + i * sizeof(unsigned int));
	}

	for (int j = 0; j < noteLen1; j++)
	{
		d2 += pgm_read_word(pgm_read_word(&(song->times2)) + j * sizeof(unsigned int));
	}

	currentSongDuration = max(d1, d2);
}

unsigned long MusicPlayer::calculateElapsedTime()
{
	const Song * song = songList[curSong];

	unsigned long d1 = 0;
	unsigned long d2 = 0;

	for (int i = 0; i < note1Index; i++)
	{
		d1 += pgm_read_word(pgm_read_word(&(song->times1)) + i * sizeof(unsigned int));
	}

	for (int j = 0; j < note2Index; j++)
	{
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

	if (ti == NULL)
	{
		ti = millis();
	}

	if (paused)
	{
		if (ti != NULL)
		{
			pausedTime = dt ? (millis() - dt) : (millis() - ti);
		}
		return;
	}

	t1 += pausedTime;
	t2 += pausedTime;
	pausedTime = 0;


	bool part1Exists = pgm_read_word(&(song->notes1)); //&& pgm_read_word(&(song->times1));
	bool part2Exists = pgm_read_word(&(song->notes2)); //&& pgm_read_word(&(song->times2));

	unsigned int curNote1 = 0;
	unsigned int curTime1 = 0;
	unsigned int curNote2 = 0;
	unsigned int curTime2 = 0;

	int noteLen1 = pgm_read_word(&(song->notes1Length));
	int noteLen2 = pgm_read_word(&(song->notes2Length));

	//Serial.print("noteLen1 = "); Serial.println(noteLen1);
	//Serial.print("noteLen2 = "); Serial.println(noteLen2);

	//dv(curTime1);

	dt = millis();

	if (part1Exists && (dt >= t1 + ti))
	{
		tone1.stop();
		delay(5);

		curNote1 = pgm_read_word(pgm_read_word(&(song->notes1)) + note1Index * sizeof(unsigned int));
		curTime1 = pgm_read_word(pgm_read_word(&(song->times1)) + note1Index * sizeof(unsigned int));

		note1Index++;
		//Serial.print("curNote1 = "); Serial.println(curNote1);
		//Serial.print("curTime1 = "); Serial.println(curTime1);
		t1 += (long)curTime1;
		tone1.play(curNote1, curTime1);
	}

	if (part2Exists && (dt >= t2 + ti))
	{
		tone2.stop();
		delay(5);

		curNote2 = pgm_read_word(pgm_read_word(&(song->notes2)) + note2Index * sizeof(unsigned int));
		curTime2 = pgm_read_word(pgm_read_word(&(song->times2)) + note2Index * sizeof(unsigned int));

		note2Index++;
		//Serial.print("curNote2 = "); Serial.println(curNote2);
		//Serial.print("curTime2 = "); Serial.println(curTime2);
		t2 += (long)curTime2;
		tone2.play(curNote2, curTime2);
	}

	if (note1Index > noteLen1)
	{
		tone1.stop();
	}
	if (note2Index > noteLen2)
	{
		tone2.stop();
	}
	if ((!part1Exists || note1Index > noteLen1) && (!part2Exists || note2Index > noteLen2))
	{
		songDone = true;
		note1Index = -1;
		note2Index = -1;
		ti = NULL;
		t1 = 0;
		t2 = 0;
		return;
	}

}


