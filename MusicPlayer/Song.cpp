#include <Tone.h>
#include "Song.h"



MusicPlayer::MusicPlayer(const struct Song* *sl, byte n)
{
	songList = sl;
	numberOfSongs = n;
	curSong = 0;
}

void MusicPlayer::start(byte t1, byte t2)
{
	tone1.begin(24);
	tone2.begin(26);
}

void MusicPlayer::playNextSong()
{
	if (curSong < numberOfSongs - 1)
	{
		curSong++;
	}
	else
	{
		curSong = 0;
	}
	playCurrentSong();
}

void MusicPlayer::playPreviousSong()
{
	if (curSong == 0)
	{
		curSong = numberOfSongs - 1;
	}
	else
	{
		curSong--;
	}
	playCurrentSong();
}

void MusicPlayer::playCurrentSong()
{
	playSong(songList[curSong]);
}


void  MusicPlayer::playSong(const struct Song* song)
{
	bool part1Exists = pgm_read_word(&(song->notes1)) && pgm_read_word(&(song->times1));
	bool part2Exists = pgm_read_word(&(song->notes2)) && pgm_read_word(&(song->times2));
	//Serial.print("Part 1 Exists = "); Serial.println(part1Exists);
	//Serial.print("Part 2 Exists = "); Serial.println(part2Exists);

	int i = -1;
	unsigned long t1 = 0;
	int j = -1;
	unsigned long t2 = 0;

	unsigned int curNote1 = 0;
	unsigned int curTime1 = 0;
	unsigned int curNote2 = 0;
	unsigned int curTime2 = 0;

	int noteLen = pgm_read_word(&(song->songLength));
	Serial.println(noteLen);
	unsigned long dt;
	unsigned long ti = millis();

	while (i < noteLen && j < noteLen)
	{
		dt = millis();

		if (part1Exists && (dt >= t1 + ti))
		{
			tone1.stop();
			delay(5);
			i++;

			curNote1 = pgm_read_word(pgm_read_word(&(song->notes1)) + i * sizeof(unsigned int));
			curTime1 = pgm_read_word(pgm_read_word(&(song->times1)) + i * sizeof(unsigned int));

			//Serial.print("curNote1 = "); Serial.println(curNote1);
			//Serial.print("curTime1 = "); Serial.println(curTime1);
			t1 += curTime1;
			tone1.play(curNote1, curTime1);
		}

		//dt = millis();

		if (part2Exists && (dt >= t2 + ti))
		{
			tone2.stop();
			delay(5);
			j++;

			curNote2 = pgm_read_word(pgm_read_word(&(song->notes2)) + j * sizeof(unsigned int));
			curTime2 = pgm_read_word(pgm_read_word(&(song->times2)) + j * sizeof(unsigned int));

			Serial.print("curNote2 = "); Serial.println(curNote2);
			Serial.print("curTime2 = "); Serial.println(curTime2);
			t2 += curTime2;
			tone2.play(curNote2, curTime2);
		}

	}
	//Serial.println("Done");
	tone1.stop();
	tone2.stop();
}


