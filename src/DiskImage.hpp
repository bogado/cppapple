#pragma once

#include <cinttypes>

#define  TRACKS      35
#define  IMAGETYPES  7
#define  NIBBLES     6656

bool    ImageBoot (HIMAGE);
void    ImageClose (HIMAGE);
void    ImageDestroy ();
void    ImageInitialize ();

enum ImageError_e
{
	IMAGE_ERROR_BAD_POINTER    =-1,
	IMAGE_ERROR_NONE           = 0,
	IMAGE_ERROR_UNABLE_TO_OPEN = 1,
	IMAGE_ERROR_BAD_SIZE       = 2
};

//int     ImageOpen (const char *,HIMAGE *,bool *,bool);
int ImageOpen (const char * imagefilename, HIMAGE *hDiskImage_, bool *pWriteProtected_, bool bCreateIfNecessary );

void    ImageReadTrack (HIMAGE,int,int,std::uint8_t *,int *);
void    ImageWriteTrack (HIMAGE,int,int,std::uint8_t *,int);
