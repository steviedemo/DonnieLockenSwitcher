#include <Arduino.h>
#include <stdint.h>
#include <IlluminatedPushButton.h>
#include "definitions.h"
#include <Footswitch.h>
#include <EEPROMWearLevel.h>
#define IDX_COUNT 1
#define CHAN_IDX  0
uint32_t last_fs_read(0);
uint8_t curr_loop(0);
Footswitch fs1(FS_1), fs2(FS_2), fs3(FS_3);
void setInput(uint8_t);
void setLoop1();
void setLoop2();
void setLoop3();
void setLoop(uint8_t);

void setup(){

    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
    pinMode(FS_1, INPUT);
    pinMode(FS_2, INPUT);
    pinMode(FS_3, INPUT);
    pinMode(CTL_A, OUTPUT);
    pinMode(CTL_B, OUTPUT);

    fs1.setActiveLow();
    fs2.setActiveLow();
    fs3.setActiveLow();
    fs1.attachPressCallback(setLoop1);
    fs2.attachPressCallback(setLoop2);
    fs3.attachPressCallback(setLoop3);

    EEPROMwl.begin(IDX_COUNT, 500);
    uint8_t last_channel = EEPROMwl.read(CHAN_IDX);
    if (last_channel < 3){
	curr_loop = last_channel;
    } else {
	curr_loop = 0;
	EEPROMwl.write(CHAN_IDX, 0);
    }
    setLoop(curr_loop);
}

void loop(){
    uint32_t curr = millis();
    if (curr - last_fs_update > 5){
	fs1.check();
	fs2.check();
	fs3.check();
	last_fs_update = curr;
    }
}

void setLoop1(){
    if (curr_loop != 0){
	setLoop(0);
    }
}

void setLoop2(){
    if (curr_loop != 1){
	setLoop(1);
    }
}
void setLoop3(){
    if (curr_loop != 2){
	setLoop(2);
    }
}

void setLoop(uint8_t channel){
    curr_loop = constrain(channel, 0, 2);    
    setInput(curr_loop);
    setLED(curr_loop);
}

void setLED(uint8_t led){
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    if (led == 0){
	digitalWrite(LED1, HIGH);
    } else if (led == 1){
	digitalWrite(LED2, HIGH);
    } else if (led == 2){
	digitalWrite(LED3, HIGH);
    }
}
void setInput(uint8_t x){
    uint8_t channel = 255 - constrain(x, 0, 2);
    digitalWrite(CTL_A, (channel >> 1) & 1);
    digitalWrite(CTL_B, channel & 1);
}
