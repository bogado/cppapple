#pragma once

extern bool   g_bShiftKey;
extern bool   g_bCtrlKey;
extern bool   g_bAltKey;
//extern bool   g_bCapsLock;

void    ClipboardInitiatePaste();

void    KeybReset();
bool    KeybGetAltStatus();
bool    KeybGetCapsStatus();
bool    KeybGetCtrlStatus();
bool    KeybGetShiftStatus();
void    KeybUpdateCtrlShiftStatus();
std::uint8_t    KeybGetKeycode ();
std::uint32_t   KeybGetNumQueries ();
void    KeybQueueKeypress (int,bool);
void    KeybToggleCapsLock ();
std::uint32_t   KeybGetSnapshot(SS_IO_Keyboard* pSS);
std::uint32_t   KeybSetSnapshot(SS_IO_Keyboard* pSS);

std::uint8_t KeybReadData (std::uint16_t pc, std::uint16_t addr, std::uint8_t bWrite, std::uint8_t d, unsigned long nCyclesLeft);
std::uint8_t KeybReadFlag (std::uint16_t pc, std::uint16_t addr, std::uint8_t bWrite, std::uint8_t d, unsigned long nCyclesLeft);
