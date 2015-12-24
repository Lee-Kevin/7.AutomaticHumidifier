

#include "Speaker.h"

 grove_speaker_g speaker;


void setup() 
{

}

void loop() 
{
  speaker.SpeakerWrite(8,NOTE_1);
  delay(1000);
  speaker.SpeakerWrite(8,NOTE_2);
  delay(1000);
  speaker.SpeakerWrite(8,NOTE_3);
  delay(1000);
  speaker.SpeakerWrite(8,NOTE_4);
  delay(1000);
  speaker.SpeakerWrite(8,NOTE_5);
  delay(1000);
  speaker.SpeakerWrite(8,NOTE_6);
  delay(1000);
  speaker.SpeakerWrite(8,NOTE_7);
  delay(1000);
  speaker.SpeakerWrite(8,NOTE_8);
  delay(1000);
  speaker.SpeakerWrite(8,NO_NOTE);
  delay(1000);  
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
