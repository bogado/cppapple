#include "./wincompat.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FILE_BEGIN 	SEEK_SET
#define FILE_CURRENT	SEEK_CUR
#define FILE_END	SEEK_END
static constexpr auto INVALID_HANDLE_VALUE = nullptr;

#define MEM_COMMIT	0x1000
#define PAGE_READWRITE	0
#define MEM_RELEASE	0


std::uint32_t SetFilePointer(HANDLE hFile,
       long lDistanceToMove,
       long * lpDistanceToMoveHigh,
       std::uint32_t dwMoveMethod);

bool ReadFile(HANDLE hFile, void * lpBuffer, std::uint32_t nNumberOfBytesToRead,
		     std::uint32_t * lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);

bool WriteFile(HANDLE hFile, const void * lpBuffer, std::uint32_t nNumberOfBytesToWrite,
		    std::uint32_t * lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped);

 /* close handle whatever it has been .... hmmmmm. I just love Microsoft! */
bool CloseHandle(HANDLE hObject);

bool DeleteFile(const char * lpFileName);

std::uint32_t GetFileSize(HANDLE hFile, std::uint32_t * lpFileSizeHigh);

void * VirtualAlloc(void * lpAddress, size_t dwSize,
		std::uint32_t flAllocationType, std::uint32_t flProtect);

bool VirtualFree(void * lpAddress, size_t dwSize, std::uint32_t dwFreeType);


static inline bool IsCharLower(char ch) {
	return isascii(ch) && islower(ch);
}

static inline bool IsCharUpper(char ch) {
	return isascii(ch) && isupper(ch);
}

std::uint32_t CharLowerBuff(char * lpsz, std::uint32_t cchLength);

