//#define WIN32_LEAN_AND_MEAN

// Mouse Wheel is not supported on Win95.
// If we didn't care about supporting Win95 (compile/run-time errors)
// we would just define the minmimum windows version to support.
// #define _WIN32_WINDOWS  0x0401
#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL 0x020A
#endif

//#include <crtdbg.h>
//#include <dsound.h>
//#include <dshow.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <tchar.h>
#include <time.h>

#ifndef _WIN32
#include "./wincompat.hpp"
#else
#include <windows.h>
#endif

//#include <winuser.h> // WM_MOUSEWHEEL
//#include <commctrl.h>
//#include <ddraw.h> - no need
//#include <htmlhelp.h> - no need
#include <SDL.h>


#include "./Common.hpp"
#include "./Structs.hpp"

#include "./AppleWin.hpp"
#include "./AY8910.hpp"

#include "./CPU.hpp"

#include "./Debug.hpp" //- may be soon...


#include "./Disk.hpp"
#include "./DiskChoose.hpp"
#include "./DiskImage.hpp"

#include "./Frame.hpp"
#include "./Harddisk.hpp"
#include "./Joystick.hpp"
#include "./Keyboard.hpp"
#include "./Log.hpp"
#include "./Memory.hpp"
#include "./Mockingboard.hpp"
#include "./ParallelPrinter.hpp"
//#include "./PropertySheetPage.hpp"
#include "./Registry.hpp"

#include "./Riff.hpp"
#include "./SaveState.hpp"
#include "./SerialComms.hpp"
#include "./SoundCore.hpp"  //- will do later
#include "./Speaker.hpp"
//#include "Tfe/Tfe.h"

#include "./stretch.hpp"

#include "./Timer.hpp"

#include "./Video.hpp"

//#include "./Util_MemoryTextFile.hpp"

