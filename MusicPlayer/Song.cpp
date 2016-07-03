#include <Tone.h>
#include "Song.h"



MusicPlayer::MusicPlayer(const struct Song* *sl, byte n)
{
	songList = sl;
	numberOfSongs = n;
	curSong = 0;
	t1 = 0;
	t2 = 0;
	ti = NULL;
	paused = 0;
	i = -1;
	j = -1;
	songDone = false;
	paused = true;
	pausedTime = 0;
	dt = 0;
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
	i = -1;
	j = -1;
	songDone = false;
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

void MusicPlayer::playCurrentSong()
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


void  MusicPlayer::playSong(const struct Song* song)
{
	if (ti == NULL)
	{
		ti = millis();
	}

	if (paused)
	{
		if (ti != NULL)
		{
			//Serial.print("dt = "); Serial.println(dt);
			pausedTime = dt ? (millis() - dt) : (millis() - ti);
			//Serial.print("pausedTime = "); Serial.println(pausedTime);
		}
		return;
	}

	//Serial.print("pausedTime = "); Serial.println(pausedTime);
	//ti += pausedTime;
	t1 += pausedTime;
	t2 += pausedTime;
	pausedTime = 0;


	bool part1Exists = pgm_read_word(&(song->notes1)) && pgm_read_word(&(song->times1));
	bool part2Exists = pgm_read_word(&(song->notes2)) && pgm_read_word(&(song->times2));
	//Serial.print("Part 1 Exists = "); Serial.println(part1Exists);
	//Serial.print("Part 2 Exists = "); Serial.println(part2Exists);

	//int i = -1;
	//unsigned long t1 = 0;
	//int j = -1;
	//unsigned long t2 = 0;

	unsigned int curNote1 = 0;
	unsigned int curTime1 = 0;
	unsigned int curNote2 = 0;
	unsigned int curTime2 = 0;

	int noteLen = pgm_read_word(&(song->songLength));
	//Serial.print("number of notes = "); Serial.println(noteLen);

	//unsigned long ti = millis();

	//Serial.print("ti = "); Serial.println(ti);


	//Serial.print("ti = "); Serial.println(ti);

	//Serial.print("i = "); Serial.println(i);
	//Serial.print("j = "); Serial.println(j);
	//Serial.print("noteLen = "); Serial.println(noteLen);

	if (i >= noteLen - 1 && j >= noteLen - 1)
	{
		songDone = true;
		tone1.stop();
		tone2.stop();
		i = -1;
		j = -1;
		ti = NULL;
		t1 = 0;
		t2 = 0;
		return;
	}


	dt = millis();

	//Serial.print("dt = "); Serial.println(dt);
	//Serial.print("t1 = "); Serial.println(t1);
	//Serial.print("ti = "); Serial.println(ti);

	if (part1Exists && (dt >= t1 + ti))
	{
		Serial.println("in part 1");
		tone1.stop();
		delay(5);
		i++;

		curNote1 = pgm_read_word(pgm_read_word(&(song->notes1)) + i * sizeof(unsigned int));
		curTime1 = pgm_read_word(pgm_read_word(&(song->times1)) + i * sizeof(unsigned int));

		//Serial.print("curNote1 = "); Serial.println(curNote1);
		//Serial.print("curTime1 = "); Serial.println(curTime1);
		t1 += (long) curTime1;
		tone1.play(curNote1, curTime1);
	}

	if (part2Exists && (dt >= t2 + ti))
	{
		tone2.stop();
		delay(5);
		j++;

		curNote2 = pgm_read_word(pgm_read_word(&(song->notes2)) + j * sizeof(unsigned int));
		curTime2 = pgm_read_word(pgm_read_word(&(song->times2)) + j * sizeof(unsigned int));

		//Serial.print("curNote2 = "); Serial.println(curNote2);
		//Serial.print("curTime2 = "); Serial.println(curTime2);
		t2 += (long) curTime2;
		tone2.play(curNote2, curTime2);
	}

}


