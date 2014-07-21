/***
 * MBusPanasonic.h - Library to emulate Alpine M-Bus commands
 * 
 * Variation of MBus library for Arduino 
 *      Copyright 2012 Oliver Mueller
 * 
 ***/

#include "MBusPanasonic.h"
#include <stdio.h>

char buffer[30];

MBus::MBus(uint8_t in, uint8_t out){

  _in=in;
  _out=out;
  
  _playerstate = PAUSED;

  pinMode(_in, INPUT);
  pinMode(_out,OUTPUT);
}

void MBus::sendZero(){
  digitalWrite(_out, HIGH);
  delayMicroseconds(600);
  digitalWrite(_out, LOW);
  delayMicroseconds(2400);
}

void MBus::sendOne(){
  digitalWrite(_out, HIGH);
  delayMicroseconds(1800);
  digitalWrite(_out, LOW);
  delayMicroseconds(1200);
}

void MBus::writeHexBitWise(uint8_t message){
  for(int8_t i=3; i>-1; i--){
    uint8_t output=((message & (1<<i) )>>i);
    if(output==1)
      sendOne();
    else
      sendZero();
  }
}

boolean MBus::checkParity(uint64_t *message){
  uint8_t parity=0;
  uint8_t test=0;
  for(uint8_t i=16; i>0; i--){
    test=((*message & ((uint64_t)0xF<<i*4) )>>i*4);
    parity=parity^test;
  }
  parity+=1;

  if(parity==(*message & ((uint64_t)0xF)))
    return true;
  else
    return false;
}

void MBus::send(uint64_t message){
  uint8_t printed=0;
  uint8_t parity=0;
  digitalWrite(11, HIGH);
  for(int8_t i=16; i>=0; i--){
    uint8_t output=((message & ((uint64_t)0xF<<i*4) )>>i*4);
    parity=parity^output;
    if(!output&&!printed){
      //do nothing
    }
    else{
      writeHexBitWise(output);
      printed++;
    }
  }
  parity = (parity+1)&0x0F;
  writeHexBitWise(parity);
  if(Serial){
    Serial.print("\tOUT:\t0x");
    if(printed>7)
      Serial.print((long)(message>>32), HEX);
    Serial.print((long)message, HEX);
    //sprintf(buffer, "\tOUT:\t%8x%8x", (unsigned long)(message>>32), (unsigned long)message);
    //sprintf(buffer, "\tOUT:\t%16x %1x", message, parity);//);
    //Serial.println(buffer);
    //Serial.print((unsigned long long)message, HEX);
    
    Serial.print(" ");
    Serial.println(parity, HEX);
  }
  digitalWrite(11, LOW);
}

boolean MBus::receive(uint64_t *message){
  *message=0;
  if(digitalRead(_in)==LOW){
    unsigned long time=micros();

    boolean gelesen=false; 
    uint8_t counter=0;

    while((micros()-time)<4000){
      if(digitalRead(_in)==HIGH&&!gelesen){
        if((micros()-time)<1400&&(micros()-time)>600){//0 is in between 600 and 1700 microseconds
          *message*=2;
          counter++;
          gelesen=true; 
        }
        else if((micros()-time)>1400){//1 is longer then 1700 microseconds
          *message*=2;
          *message+=1;
          counter++;
          gelesen=true;
        }
      }
      if(gelesen&&digitalRead(_in)==LOW){
        gelesen=false;
        time=micros();
      }  
    }
    if(counter%4||!checkParity(message)||counter==0){
      //message is not ok
      *message=0;
      return false;
    }
    else{
      if(Serial && !(*message==0x28B || *message==0x38C || *message==0x48D || *message==0x58E)){
        Serial.print("\t IN:\t0x");
        if(counter>31)
          Serial.print((long unsigned int)((*message)>>32), HEX);
        Serial.println((long unsigned int)(*message), HEX);
        //sprintf(buffer, "\t IN:\t %16x", (uint64_t)*message);//);
        //Serial.println(buffer);
      }
      (*message)=(*message)>>4;//ingnore parity
      return true;
    }
  }
  return false;

}

/*
 CD-changer emulation from here on
 */
void MBus::sendPlayingTrack(uint8_t disc, uint8_t track, 
                            uint16_t secs, uint8_t playbackStatus){
  uint64_t play = (uint64_t) PLAYBACK_STATUS;//0xE90000000000000ull;
  //Serial.print((long unsigned int)(message >> 32), HEX);
  //Serial.println((long unsigned int) message, HEX);
  
  switch(playbackStatus & PBSTATUS_MASK){
    case(STOPPED):
      play |= (uint64_t)PBSTATUS_STOPPED;
      break;
    case(PAUSED):
      play |= (uint64_t)PBSTATUS_PAUSED;
      break;
    case(PLAYING):
      play |= (uint64_t)PBSTATUS_PLAYING; //0x004000000000001ull;
      break;
    default:
      break;
  }
  
  //if( playbackStatus & SHUFFLE_MASK ) play |= PBSTATUS_SHUFFLE;
  //if( playbackStatus & REPEAT_ONE)    play |= PBSTATUS_REPEAT_ONE;
  //if( playbackStatus & REPEAT_ALL)    play |= PBSTATUS_REPEAT_ALL;
  
  play |= (uint64_t)BCD(track)<<(10*4);
  play |= (uint64_t)BCD(disc)<<(8*4);
  play |= (uint64_t)BCD(secs/60)<<(6*4);
  play |= (uint64_t)BCD(secs%60)<<(4*4);

  send(play);
}

void MBus::sendChangedCD(uint8_t CD,uint8_t Track){
  uint64_t play=0xEB900000001ull;
  play|=(uint64_t)CD<<(7*4);
  play|=(uint64_t)BCD(Track)<<(5*4);
  //play|=(uint64_t)(Track/10)<<(6*4);
  send(play);
}

void MBus::sendCDStatus(uint8_t CD){
  uint64_t play=0xEC001999999Full;
  play|=(uint64_t)CD<<(9*4);
  send(play);
}

//convert a byte with a decimal value less than 100 into two binary-coded hex digits
// 49 (0x31) -> 0x49
uint64_t MBus::BCD(uint8_t val){
  uint64_t bcd = 0;
  bcd |= (val/10)<<4;  //tens
  bcd |= (val%10);     //ones
  return bcd;
}

