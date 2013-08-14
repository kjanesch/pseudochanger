#ifndef _DEFINES_H
#define _DEFINES_H


#define STOPPED 0x00
#define PLAYING 0x01
#define PAUSED  0x02
#define SHUFFLE_SONGS 0x10
#define SHUFFLE_ALBUMS 0x20
#define REPEAT_ONE 0x40
#define REPEAT_ALL 0x80

#define PBSTATUS_MASK 0x0F
#define SHUFFLE_MASK 0x30
#define REPEAT_MASK  0xC0

#define mBusIn  PIN_D1
#define mBusOut PIN_D0

//pin definitions
static const byte SENSE_IPOD  = PIN_F4;
static const byte LED_AUX     = PIN_C6;
static const byte LED_IPOD    = PIN_C7;
static const byte RELAY_AUX   = PIN_B0;
static const byte RELAY_IPOD  = PIN_B1;
static const byte TOGGLE_AUX  = PIN_D5;
static const byte TOGGLE_IPOD = PIN_D4;


#endif
