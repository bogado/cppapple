#pragma once

enum JOYNUM {JN_JOYSTICK0=0, JN_JOYSTICK1};

extern std::uint32_t      joytype[2];

extern std::uint32_t    joy1index;
extern std::uint32_t    joy2index;
extern std::uint32_t    joy1button1;
extern std::uint32_t    joy1button2;
extern std::uint32_t    joy2button1;
extern std::uint32_t    joy1axis0;
extern std::uint32_t    joy1axis1;
extern std::uint32_t    joy2axis0;
extern std::uint32_t    joy2axis1;
extern std::uint32_t    joyexitenable;
extern std::uint32_t    joyexitbutton0;
extern std::uint32_t    joyexitbutton1;
extern bool           joyquitevent;
void CheckJoyExit(); 



void    JoyInitialize ();
void	JoyShutDown();

BOOL    JoyProcessKey (int,BOOL,BOOL,BOOL);
void    JoyReset ();
void    JoySetButton (eBUTTON,eBUTTONSTATE);
BOOL    JoySetEmulationType (/*HWND,*/std::uint32_t,int);
void    JoySetPosition (int,int,int,int);
void    JoyUpdatePosition ();
BOOL    JoyUsingMouse ();
void    JoySetTrim(short nValue, bool bAxisX);
short   JoyGetTrim(bool bAxisX);
std::uint32_t   JoyGetSnapshot(SS_IO_Joystick* pSS);
std::uint32_t   JoySetSnapshot(SS_IO_Joystick* pSS);

std::uint8_t JoyReadButton (std::uint16_t pc, std::uint16_t addr, std::uint8_t bWrite, std::uint8_t d, ULONG nCyclesLeft);
std::uint8_t JoyReadPosition (std::uint16_t pc, std::uint16_t addr, std::uint8_t bWrite, std::uint8_t d, ULONG nCyclesLeft);
std::uint8_t JoyResetPosition (std::uint16_t pc, std::uint16_t addr, std::uint8_t bWrite, std::uint8_t d, ULONG nCyclesLeft);
