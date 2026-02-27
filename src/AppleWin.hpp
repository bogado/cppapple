#pragma once

#include <filesystem>
#include <array>

static constexpr auto FILE_SEPARATOR= std::filesystem::path::preferred_separator;
static constexpr auto FTP_SEPARATOR = '/';

// let it be our second version!
#define LINAPPLE_VERSION	2

#include <curl/curl.h>

extern FILE * spMono,*spStereo;

extern char VERSIONSTRING[];	// Contructed in WinMain()

extern char     *g_pAppTitle;

extern eApple2Type	g_Apple2Type;

extern bool       behind;
extern std::uint32_t      cumulativecycles;
extern std::uint32_t      cyclenum;
extern std::uint32_t      emulmsec;
extern bool       g_bFullSpeed;

// Win32
//extern HINSTANCE  g_hInstance;


extern AppMode_e g_nAppMode;

extern unsigned g_ScreenWidth;
extern unsigned g_ScreenHeight;

extern std::uint32_t      needsprecision;
//extern char      g_sProgramDir[MAX_PATH];
extern char      g_sCurrentDir[MAX_PATH];
extern char      g_sHDDDir[MAX_PATH];
extern char      g_sSaveStateDir[MAX_PATH];
extern char      g_sParallelPrinterFile[MAX_PATH];
// FTP vars
extern char     g_sFTPLocalDir[MAX_PATH]; // FTP Local Dir, see linapple.conf for details
extern char     g_sFTPServer[MAX_PATH]; // full path to default FTP server
extern char     g_sFTPServerHDD[MAX_PATH]; // full path to default FTP server

//extern char     g_sFTPUser[256]; // user name
//extern char     g_sFTPPass[256]; // password
extern char     g_sFTPUserPass[512]; // full login line

extern CURL *	 g_curl;


extern bool       g_bResetTiming;
extern bool       restart;

extern std::uint32_t      g_dwSpeed;
extern double     g_fCurrentCLK6502;

extern int        g_nCpuCyclesFeedback;
extern std::uint32_t      g_dwCyclesThisFrame;

extern FILE*      g_fh;				// Filehandle for log file
extern bool       g_bDisableDirectSound;	// Cmd line switch: don't init DS (so no MB support)

extern unsigned		g_Slot4;	// Mockingboard or Mouse in slot4

void    SetCurrentCLK6502();
