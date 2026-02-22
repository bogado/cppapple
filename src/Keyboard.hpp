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
DWORD   KeybGetNumQueries ();
void    KeybQueueKeypress (int,BOOL);
void    KeybToggleCapsLock ();
DWORD   KeybGetSnapshot(SS_IO_Keyboard* pSS);
DWORD   KeybSetSnapshot(SS_IO_Keyboard* pSS);

std::uint8_t KeybReadData (WORD pc, WORD addr, std::uint8_t bWrite, std::uint8_t d, ULONG nCyclesLeft);
std::uint8_t KeybReadFlag (WORD pc, WORD addr, std::uint8_t bWrite, std::uint8_t d, ULONG nCyclesLeft);
