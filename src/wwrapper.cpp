/*
 * Wrappers for some common Microsoft file and memory functions used in AppleWin
 *	by beom beotiger, Nov 2007AD
*/

#include "./wwrapper.hpp"

std::uint32_t SetFilePointer(HANDLE hFile,
       long lDistanceToMove,
       long * lpDistanceToMoveHigh,
       std::uint32_t dwMoveMethod)	{
	       /* ummm,fseek in Russian */
	       fseek((FILE*)hFile, lDistanceToMove, dwMoveMethod);
	       return ftell((FILE*)hFile);
}

BOOL ReadFile(HANDLE hFile, void * lpBuffer, std::uint32_t nNumberOfBytesToRead,
       		std::uint32_t * lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped)	{

	/* read something from file */
	std::uint32_t bytesread = fread(lpBuffer, 1, nNumberOfBytesToRead, (FILE*)hFile);
	*lpNumberOfBytesRead = bytesread;
	return (nNumberOfBytesToRead == bytesread);
}

BOOL WriteFile(HANDLE hFile, const void * lpBuffer, std::uint32_t nNumberOfBytesToWrite,
		std::uint32_t * lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped) {
	/* write something to file */
	std::uint32_t byteswritten = fwrite(lpBuffer, 1, nNumberOfBytesToWrite, (FILE*)hFile);
	*lpNumberOfBytesWritten = byteswritten;
	return (nNumberOfBytesToWrite == byteswritten);
}

/* close handle whatever it has been .... hmmmmm. I just love Microsoft! */
BOOL CloseHandle(HANDLE hObject) {
	return(!fclose((FILE*) hObject));
}

BOOL DeleteFile(const char * lpFileName) {
	if(remove(lpFileName) == 0) return true;
	else return false;
}

std::uint32_t GetFileSize(HANDLE hFile, std::uint32_t * lpFileSizeHigh) {
	/* what is the size of the specified file??? Hmmm, really I donna. ^_^ */
	long lcurset = ftell((FILE*)hFile); // remember current file position

	fseek((FILE*)hFile, 0, FILE_END);	// go to the end of file
	std::uint32_t lfilesize = ftell((FILE*)hFile); // that is the real size of file, isn't it??
	fseek((FILE*)hFile, lcurset, FILE_BEGIN); // let the file position be the same as before
	return lfilesize;
}

void * VirtualAlloc(void * lpAddress, size_t dwSize,
      std::uint32_t flAllocationType, std::uint32_t flProtect) {
	/* just malloc and alles? 0_0 */
	void* mymemory;
	mymemory = malloc(dwSize);
	if(flAllocationType & 0x1000) ZeroMemory(mymemory, dwSize); // original VirtualAlloc does this (if..)
	return mymemory;
}

BOOL VirtualFree(void * lpAddress, size_t dwSize,
			std::uint32_t dwFreeType) {
	free(lpAddress);
	return true;
}

// make all chars in buffer lowercase
std::uint32_t CharLowerBuff(char * lpsz, std::uint32_t cchLength)
{
//		char *s;
	if (lpsz)
		for (lpsz; *lpsz; lpsz++)
			*lpsz = tolower(*lpsz);
	return 1;

}
