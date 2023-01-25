#include "pitches.h"
#include <Arduino.h>

void buzzerOn() 
    {
    // notes in the melody:
    int melody[] = {NOTE_CS5, NOTE_DS5, NOTE_FS5};
    // 500 milisecond
    int duration = 500;
    // We only use melody[1] here to not have too long of a delay
    // pin8 output the voice, every scale is 0.5 sencond
    tone(14, melody[1], duration);
    }