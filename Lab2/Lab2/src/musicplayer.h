#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <inttypes.h>

typedef enum MODE {
    HOLD,
    BEAT
} MODE;

typedef struct Note {
    uint16_t freq;
    uint8_t duration; // Whole, half, quarter, eighth, 16th
    uint8_t leds; // Could compactify defintion by combining with duration, but no need as of yet
} Note;

typedef struct Song {
    char* name;
    const Note* notes;
    uint16_t length;
    uint8_t bpm;
} Song;

void handleMusicButtons(uint8_t buttons);
void setPlayerMode(MODE mode);
void setSong(const Song* song);
void restartSong();
Song* currentlyPlaying(void);
void stopSong(void);
void resumeSong(void);
void pauseSong(void);
void initMusicPlayer();
uint16_t getScore();

#endif
