#pragma once

extern bool       g_bMBTimerIrqActive;
extern UINT32	g_uTimer1IrqCount;	// DEBUG

void	MB_Initialize();
void	MB_Reinitialize();
void	MB_Destroy();
void    MB_Reset();
void    MB_Mute();
void    MB_Demute();
void    MB_StartOfCpuExecute();
void    MB_EndOfVideoFrame();
void    MB_CheckIRQ();
void    MB_UpdateCycles(unsigned long uExecutedCycles);
eSOUNDCARDTYPE MB_GetSoundcardType();
void    MB_SetSoundcardType(eSOUNDCARDTYPE NewSoundcardType);
double  MB_GetFramePeriod();
bool    MB_IsActive();
std::uint32_t   MB_GetVolume();
void    MB_SetVolume(std::uint32_t dwVolume, std::uint32_t dwVolumeMax);
std::uint32_t   MB_GetSnapshot(SS_CARD_MOCKINGBOARD* pSS, std::uint32_t dwSlot);
std::uint32_t   MB_SetSnapshot(SS_CARD_MOCKINGBOARD* pSS, std::uint32_t dwSlot);

extern short 	* pDSMockBuf;	// Mockingboard data buffer (in size of g_dwDSMockBufferSize samples?)
extern UINT	nDSMockWCur;	// write cursor
extern UINT	nDSMockRCur;	// read cursor
//extern std::uint32_t	g_dwDSMockBufferSize;	// size of low-level buffer

