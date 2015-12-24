#ifndef __GROVE_SPEAKER_G_H__
#define __GROVE_SPEAKER_G_H__
#include <Arduino.h>
// I am not sure yet
#define NOTE_1  523
#define NOTE_2  587
#define NOTE_3  659
#define NOTE_4  698
#define NOTE_5  784
#define NOTE_6  880
#define NOTE_7  988
#define NOTE_8  1047
#define NO_NOTE 0 


class grove_speaker_g{
    
public:

void SpeakerWrite(int PinName,int Note)
{
    if(Note == NO_NOTE) {
        noTone(PinName);
    } else {
        tone(PinName,Note,0);
    }
}
    
};


#endif