/*
AppleWin : An Apple //e emulator for Windows

Copyright (C) 1994-1996, Michael O'Brien
Copyright (C) 1999-2001, Oliver Schmidt
Copyright (C) 2002-2005, Tom Charlesworth
Copyright (C) 2006-2007, Tom Charlesworth, Michael Pohoreski

AppleWin is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

AppleWin is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with AppleWin; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/* Description: RIFF funcs	--- my GOD, what is RIFF?
 *
 * Author: Various
 */

/* Adaptation for SDL and POSIX (l) by beom beotiger, Nov-Dec 2007 */

#include "./wincompat.hpp"
#include "./Riff.hpp"
#include "./wwrapper.hpp"

static HANDLE g_hRiffFile = INVALID_HANDLE_VALUE;
static std::uint32_t dwTotalOffset;
static std::uint32_t dwDataOffset;
static std::uint32_t g_dwTotalNumberOfBytesWritten = 0;
static unsigned int g_NumChannels = 2;

int RiffInitWriteFile(char* pszFile, unsigned int sample_rate, unsigned int NumChannels)
{
//	g_hRiffFile = CreateFile(pszFile, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	g_hRiffFile = fopen(pszFile, "w");

	if(g_hRiffFile == INVALID_HANDLE_VALUE)
		return 1;

	g_NumChannels = NumChannels;

	//

	std::uint32_t temp32;
	std::uint16_t temp16;

	std::uint32_t dwNumberOfBytesWritten;

	WriteFile(g_hRiffFile, "RIFF", 4, &dwNumberOfBytesWritten, nullptr);

	temp32 = 0;				// total size
	dwTotalOffset = SetFilePointer(g_hRiffFile, 0, nullptr, FILE_CURRENT);
	WriteFile(g_hRiffFile, &temp32, 4, &dwNumberOfBytesWritten, nullptr);

	WriteFile(g_hRiffFile, "WAVE", 4, &dwNumberOfBytesWritten, nullptr);

	//

	WriteFile(g_hRiffFile, "fmt ", 4, &dwNumberOfBytesWritten, nullptr);

	temp32 = 16;			// format length
	WriteFile(g_hRiffFile, &temp32, 4, &dwNumberOfBytesWritten, nullptr);

	temp16 = 1;				// PCM format
	WriteFile(g_hRiffFile, &temp16, 2, &dwNumberOfBytesWritten, nullptr);

	temp16 = NumChannels;		// channels
	WriteFile(g_hRiffFile, &temp16, 2, &dwNumberOfBytesWritten, nullptr);

	temp32 = sample_rate;	// sample rate
	WriteFile(g_hRiffFile, &temp32, 4, &dwNumberOfBytesWritten, nullptr);

	temp32 = sample_rate * 2 * NumChannels;	// bytes/second
	WriteFile(g_hRiffFile, &temp32, 4, &dwNumberOfBytesWritten, nullptr);

	temp16 = 2 * NumChannels;	// block align
	WriteFile(g_hRiffFile, &temp16, 2, &dwNumberOfBytesWritten, nullptr);

	temp16 = 16;			// bits/sample
	WriteFile(g_hRiffFile, &temp16, 2, &dwNumberOfBytesWritten, nullptr);

	//

	WriteFile(g_hRiffFile, "data", 4, &dwNumberOfBytesWritten, nullptr);

	temp32 = 0;				// data length
	dwDataOffset = SetFilePointer(g_hRiffFile, 0, nullptr, FILE_CURRENT);
	WriteFile(g_hRiffFile, &temp32, 4, &dwNumberOfBytesWritten, nullptr);

	return 0;
}

int RiffFinishWriteFile()
{
	if(g_hRiffFile == INVALID_HANDLE_VALUE)
		return 1;

	//

	std::uint32_t temp32;

	std::uint32_t dwNumberOfBytesWritten;

	temp32 = g_dwTotalNumberOfBytesWritten - (dwTotalOffset + 4);
	SetFilePointer(g_hRiffFile, dwTotalOffset, nullptr, FILE_BEGIN);
	WriteFile(g_hRiffFile, &temp32, 4, &dwNumberOfBytesWritten, nullptr);

	temp32 = g_dwTotalNumberOfBytesWritten - (dwDataOffset + 4);
	SetFilePointer(g_hRiffFile, dwDataOffset, nullptr, FILE_BEGIN);
	WriteFile(g_hRiffFile, &temp32, 4, &dwNumberOfBytesWritten, nullptr);

	return CloseHandle(g_hRiffFile);
}

int RiffPutSamples(short* buf, unsigned int uSamples)
{
	if(g_hRiffFile == INVALID_HANDLE_VALUE)
		return 1;

	//

	std::uint32_t dwNumberOfBytesWritten;

	bool bRes = WriteFile(
		g_hRiffFile,
		buf,
		uSamples * sizeof(short) * g_NumChannels,
		&dwNumberOfBytesWritten,
		nullptr);

	g_dwTotalNumberOfBytesWritten += dwNumberOfBytesWritten;

	return 0;
}
