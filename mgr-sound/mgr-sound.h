#ifndef MGR_SOUND_H
#define MGR_SOUND_H

void initializeScreenBuffer();
void updateScreenPosition(const FMOD_VECTOR& worldPosition);

const int SCREEN_WIDTH = NUM_COLUMNS;
const int SCREEN_HEIGHT = 16;

int currentScreenPosition = -1;
char screenBuffer[(SCREEN_WIDTH + 1) * SCREEN_HEIGHT + 1] = { 0 };


#endif /*MGR_SOUND_H*/
