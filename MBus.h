/***
 * MBus.cpp - Library to emulate Alpine M-Bus commands
 * 
 * Copyright 2012 Oliver Mueller
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ***/
#ifndef MBus_h
#define MBus_h

#include "Arduino.h"
#include "./defines.h"

#define Shutdown 0x11142
#define OFF 0x11142
#define Changing 0x9B910100001

#define MBUS_WAIT                 0xEF00000UL

#define CHANGER_STATE             0xEF00000UL
#define CHGRST_ERR_HITEMP           0x30000UL
#define CHGRST_MALF_CHANGER          0x1000
#define CHGRST_MALF_PLAYER           0x2000

// masks to take apart the playback mode masks
#define MBUS_PBMODE_MASK 0xFFFFFFFFFFFF0000ull
#define MBUS_PBMODE               0x6140000
#define MBUS_PBMODE_REPEAT_MSK       0xF000
#define MBUS_PBMODE_REPEAT_ALL       0x8000
#define MBUS_PBMODE_REPEAT_ONE       0x4000
#define MBUS_PBMODE_SCAN              0x800
#define MBUS_PBMODE_RANDOM            0x200

// playback status structure, representing the different modes available.
// these are assembled by sendPlayingTrack().
#define PLAYBACK_STATUS    0xE90000000000000ull
#define PBSTATUS_REWIND    0x007000000000001ull
#define PBSTATUS_FFWD      0x006000000000001ull
#define PBSTATUS_SPINUP    0x005000000000001ull
#define PBSTATUS_PLAYING   0x004000000000001ull
#define PBSTATUS_PAUSED    0x003000000000002ull
#define PBSTATUS_STOPPED   0x002000000000009ull
#define PBSTATUS_REPEAT_ONE           0x4000
#define PBSTATUS_REPEAT_ALL           0x8000
#define PBSTATUS_SCAN                 0x0800
#define PBSTATUS_SHUFFLE              0x0400

// masks to disassemble the disc/track selection command
#define MBUS_SELCMD_MASK 0xFFFFFFFFFFFF0000ull
#define MBUS_SELCMD               0x6130000
#define MBUS_SELCMD_DISCMASK         0xF000
#define MBUS_SELCMD_TRACKMASK         0xFF0
#define MBUS_SELCMD_PAUSEMASK           0x1

// mask and various bits to pick apart the playback control command
#define MBUS_CMD_MASK    0xFFFFFFFFFFFFF000ull
#define MBUS_CMD                   0x611000ul
#define MBUS_CMD_PAUSE                0x402
#define MBUS_CMD_PLAY                 0x012
#define MBUS_CMD_FFWD                 0x052
#define MBUS_CMD_RWD                  0x092


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

