/***
 * MBusPanasonic.cpp - Library to emulate Alpine M-Bus commands
 * 
 * Variation of MBus library for Arduino 
 *      Copyright 2012 Oliver Mueller
 * 
 ***/
#ifndef _MBUSPANASONIC_H
#define _MBUSPANASONIC_H

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

#define CMD_OFF 0x11142
#define Changing 0x9B910100001

#define MBUS_WAIT                 0xEF00000ULL

#define CHANGER_STATE             0xEF00000ULL
#define CHGRST_ERR_HITEMP           0x30000ULL
#define CHGRST_MALF_CHANGER          0x1000ULL
#define CHGRST_MALF_PLAYER           0x2000ULL

// masks to take apart the playback mode masks
#define MBUS_PBMODE_MASK  0xFFFFFFFFFFFF0000ULL
#define MBUS_PBMODE                0x6140000ULL
#define MBUS_PBMODE_REPEAT_MSK        0xF000ULL
#define MBUS_PBMODE_REPEAT_ALL        0x8000ULL
#define MBUS_PBMODE_REPEAT_ONE        0x4000ULL
#define MBUS_PBMODE_SCAN               0x800ULL
#define MBUS_PBMODE_RANDOM             0x200ULL

// playback status structure, representing the different modes available.
// these are assembled by sendPlayingTrack().
#define PLAYBACK_STMASK   0xFFF0000000000000ULL
#define PLAYBACK_STATUS    0xE90000000000000ULL
#define PBSTATUS_REWIND    0x007000000000001ULL
#define PBSTATUS_FFWD      0x006000000000001ULL
#define PBSTATUS_SPINUP    0x005000000000001ULL
#define PBSTATUS_PLAYING   0x004000000000001ULL
#define PBSTATUS_PAUSED    0x003000000000002ULL
#define PBSTATUS_STOPPED   0x002000000000009ULL
#define PBSTATUS_REPEAT_ONE           0x4000ULL 
#define PBSTATUS_REPEAT_ALL           0x8000ULL 
#define PBSTATUS_SCAN                 0x0800ULL 
#define PBSTATUS_SHUFFLE              0x0400ULL 

// masks to disassemble the disc/track selection command
#define MBUS_SELCMD_MASK  0xFFFFFFFFFFFF0000ULL 
#define MBUS_SELCMD                0x6130000ULL 
#define MBUS_SELCMD_DISCMASK          0xF000ULL 
#define MBUS_SELCMD_TRACKMASK          0xFF0ULL 
#define MBUS_SELCMD_PAUSEMASK            0x1ULL 

// mask and various bits to pick apart the playback control command
#define MBUS_CMD_MASK     0xFFFFFFFFFFFFF000ULL 
#define MBUS_CMD                    0x611000ULL 
#define MBUS_CMD_PAUSE                 0x402ULL 
#define MBUS_CMD_PLAY                  0x012ULL 
#define MBUS_CMD_FFWD                  0x052ULL 
#define MBUS_CMD_RWD                   0x092ULL 

class MBus{
public:
  MBus(uint8_t in, uint8_t out);
  void send(uint64_t message);
  boolean receive(uint64_t *message);
  void sendPlayingTrack(uint8_t disc, uint8_t track, uint16_t secs, uint8_t playbackStatus);
  void sendChangedCD(uint8_t CD,uint8_t Track);
  void sendCDStatus(uint8_t CD);
  uint8_t _playerstate;
  
private:  

  uint8_t _in;
  uint8_t _out;
  void sendZero();
  void sendOne();
  void writeHexBitWise(uint8_t message);
  boolean checkParity(uint64_t *message);
  uint64_t BCD(uint8_t val);
};

#endif	

