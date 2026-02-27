#pragma once
//
// For audio use only none or SDL_SOUND subsystem
#define  SOUND_NONE    0
//#define  SOUND_DIRECT  1
//#define  SOUND_SMART   2
#define  SOUND_WAVE    1

extern std::uint32_t      soundtype;
extern double     g_fClksPerSpkrSample;

// needed for DSPlaySnd callback function
extern short 	* pDSSpkrBuf;	// speaker data buffer (in size of g_dwDSSpkrBufferSize samples?
extern std::uint32_t g_dwDSSpkrBufferSize;	// size of Speakers audio buffer
extern unsigned	nDSSpkrWCur;	// write cursor
extern unsigned	nDSSpkrRCur;	// read cursor


void    SpkrDestroy ();
void    SpkrInitialize ();
void    SpkrReinitialize ();
void    SpkrReset();
//bool    SpkrSetEmulationType (/*HWND,*/std::uint32_t); -2012aD
void    SpkrUpdate (std::uint32_t);
//void    SpkrUpdate_Timer();
std::uint32_t   SpkrGetVolume();
void    SpkrSetVolume(std::uint32_t dwVolume, std::uint32_t dwVolumeMax);
void    Spkr_Mute();
void    Spkr_Demute();
bool    Spkr_IsActive();
bool    Spkr_DSInit();
void    Spkr_DSUninit();
std::uint32_t   SpkrGetSnapshot(SS_IO_Speaker* pSS);
std::uint32_t   SpkrSetSnapshot(SS_IO_Speaker* pSS);

std::uint8_t SpkrToggle (std::uint16_t pc, std::uint16_t addr, std::uint8_t bWrite, std::uint8_t d, unsigned long nCyclesLeft);
