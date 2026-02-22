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

/* Description: Disk Image
 *
 * Author: Various
 */

/* Adaptation for SDL and POSIX (l) by beom beotiger, Nov-Dec 2007 */

#include "./stdafx.hpp"
#include "./wwrapper.hpp"
//#pragma  hdrstop

/* DO logical order  0 1 2 3 4 5 6 7 8 9 A B C D E F */
/*    physical order 0 D B 9 7 5 3 1 E C A 8 6 4 2 F */

/* PO logical order  0 E D C B A 9 8 7 6 5 4 3 2 1 F */
/*    physical order 0 2 4 6 8 A C E 1 3 5 7 9 B D F */

struct imageinfo {
    char      filename[MAX_PATH];
    DWORD      format;
    HANDLE     file;
    DWORD      offset;
    BOOL       writeprotected;
    DWORD      headersize;
    std::uint8_t *     header;
    BOOL       validtrack[TRACKS];
};

using imageinfoptr = imageinfo*;

using boottype = BOOL (*)(imageinfoptr);
using detecttype = DWORD (*)(std::uint8_t *,DWORD);
using readtype = void (*)(imageinfoptr,int,int,std::uint8_t *,int *);
using writetype = void (*)(imageinfoptr,int,int,std::uint8_t *,int);

BOOL  AplBoot (imageinfoptr ptr);
DWORD AplDetect (std::uint8_t * imageptr, DWORD imagesize);
DWORD DoDetect (std::uint8_t * imageptr, DWORD imagesize);
void  DoRead (imageinfoptr ptr, int track, int quartertrack, std::uint8_t * trackimagebuffer, int *nibbles);
void  DoWrite (imageinfoptr ptr, int track, int quartertrack, std::uint8_t * trackimage, int nibbles);
DWORD IieDetect (std::uint8_t * imageptr, DWORD imagesize);
void  IieRead (imageinfoptr ptr, int track, int quartertrack, std::uint8_t * trackimagebuffer, int *nibbles);
void  IieWrite (imageinfoptr ptr, int track, int quartertrack, std::uint8_t * trackimage, int nibbles);
DWORD Nib1Detect (std::uint8_t * imageptr, DWORD imagesize);
void  Nib1Read (imageinfoptr ptr, int track, int quartertrack, std::uint8_t * trackimagebuffer, int *nibbles);
void  Nib1Write (imageinfoptr ptr, int track, int quartertrack, std::uint8_t * trackimage, int nibbles);
DWORD Nib2Detect (std::uint8_t * imageptr, DWORD imagesize);
void  Nib2Read (imageinfoptr ptr, int track, int quartertrack, std::uint8_t * trackimagebuffer, int *nibbles);
void  Nib2Write (imageinfoptr ptr, int track, int quartertrack, std::uint8_t * trackimage, int nibbles);
DWORD PoDetect (std::uint8_t * imageptr, DWORD imagesize);
void  PoRead (imageinfoptr ptr, int track, int quartertrack, std::uint8_t * trackimagebuffer, int *nibbles);
void  PoWrite (imageinfoptr ptr, int track, int quartertrack, std::uint8_t * trackimage, int nibbles);
BOOL  PrgBoot (imageinfoptr ptr);
DWORD PrgDetect (std::uint8_t * imageptr, DWORD imagesize);

typedef struct _imagetyperec {
    const char *    createexts;
    const char *    rejectexts;
    detecttype detect;
    boottype   boot;
    readtype   read;
    writetype  write;
} imagetyperec, *imagetypeptr;

static imagetyperec imagetype[IMAGETYPES] = {{TEXT(".prg"),
                                       TEXT(".do;.dsk;.iie;.nib;.po"),
                                       PrgDetect,
                                       PrgBoot,
                                       nullptr,
                                       nullptr},
                                      {TEXT(".do;.dsk"),
                                       TEXT(".nib;.iie;.po;.prg"),
                                       DoDetect,
                                       nullptr,
                                       DoRead,
                                       DoWrite},
                                      {TEXT(".po"),
                                       TEXT(".do;.iie;.nib;.prg"),
                                       PoDetect,
                                       nullptr,
                                       PoRead,
                                       PoWrite},
                                      {TEXT(".apl"),
                                       TEXT(".do;.dsk;.iie;.nib;.po"),
                                       AplDetect,
                                       AplBoot,
                                       nullptr,
                                       nullptr},
                                      {TEXT(".nib"),
                                       TEXT(".do;.iie;.po;.prg"),
                                       Nib1Detect,
                                       nullptr,
                                       Nib1Read,
                                       Nib1Write},
                                      {TEXT(".nb2"),
                                       TEXT(".do;.iie;.po;.prg"),
                                       Nib2Detect,
                                       nullptr,
                                       Nib2Read,
                                       Nib2Write},
                                      {TEXT(".iie"),
                                       TEXT(".do.;.nib;.po;.prg"),
                                       IieDetect,
                                       nullptr,
                                       IieRead,
                                       IieWrite}};

static std::uint8_t diskbyte[0x40] = {0x96,0x97,0x9A,0x9B,0x9D,0x9E,0x9F,0xA6,
                       0xA7,0xAB,0xAC,0xAD,0xAE,0xAF,0xB2,0xB3,
                       0xB4,0xB5,0xB6,0xB7,0xB9,0xBA,0xBB,0xBC,
                       0xBD,0xBE,0xBF,0xCB,0xCD,0xCE,0xCF,0xD3,
                       0xD6,0xD7,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,
                       0xDF,0xE5,0xE6,0xE7,0xE9,0xEA,0xEB,0xEC,
                       0xED,0xEE,0xEF,0xF2,0xF3,0xF4,0xF5,0xF6,
                       0xF7,0xF9,0xFA,0xFB,0xFC,0xFD,0xFE,0xFF};

static std::uint8_t sectornumber[3][0x10] = {{0x00,0x08,0x01,0x09,0x02,0x0A,0x03,0x0B,
                               0x04,0x0C,0x05,0x0D,0x06,0x0E,0x07,0x0F},
                              {0x00,0x07,0x0E,0x06,0x0D,0x05,0x0C,0x04,
                               0x0B,0x03,0x0A,0x02,0x09,0x01,0x08,0x0F},
                              {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                               0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};

static std::uint8_t * workbuffer = nullptr;

/****************************************************************************
*
*  NIBBLIZATION FUNCTIONS
*
***/

//===========================================================================
std::uint8_t * Code62 (int sector) {

  // CONVERT THE 256 8-BIT std::uint8_tS INTO 342 6-BIT std::uint8_tS, WHICH WE STORE
  // STARTING AT 4K INTO THE WORK BUFFER.
  {
    std::uint8_t * sectorbase = workbuffer+(sector << 8);
    std::uint8_t * resultptr  = workbuffer+0x1000;
    std::uint8_t   offset     = 0xAC;
    while (offset != 0x02) {
      std::uint8_t value = 0;
#define ADDVALUE(a) value = (value << 2) |        \
                            (((a) & 0x01) << 1) | \
                            (((a) & 0x02) >> 1)
      ADDVALUE(*(sectorbase+offset));  offset -= 0x56;
      ADDVALUE(*(sectorbase+offset));  offset -= 0x56;
      ADDVALUE(*(sectorbase+offset));  offset -= 0x53;
#undef ADDVALUE
      *(resultptr++) = value << 2;
    }
    *(resultptr-2) &= 0x3F;
    *(resultptr-1) &= 0x3F;
    int loop = 0;
    while (loop < 0x100)
      *(resultptr++) = *(sectorbase+(loop++));
  }

  // EXCLUSIVE-OR THE ENTIRE DATA BLOCK WITH ITSELF OFFSET BY ONE std::uint8_t,
  // CREATING A 343RD std::uint8_t WHICH IS USED AS A CHECKSUM.  STORE THE NEW
  // BLOCK OF 343 std::uint8_tS STARTING AT 5K INTO THE WORK BUFFER.
  {
    std::uint8_t   savedval  = 0;
    std::uint8_t * sourceptr = workbuffer+0x1000;
    std::uint8_t * resultptr = workbuffer+0x1400;
    int    loop      = 342;
    while (loop--) {
      *(resultptr++) = savedval ^ *sourceptr;
      savedval = *(sourceptr++);
    }
    *resultptr = savedval;
  }

  // USING A LOOKUP TABLE, CONVERT THE 6-BIT std::uint8_tS INTO DISK std::uint8_tS.  A VALID
  // DISK std::uint8_t IS A std::uint8_t THAT HAS THE HIGH BIT SET, AT LEAST TWO ADJACENT
  // BITS SET (EXCLUDING THE HIGH BIT), AND AT MOST ONE PAIR OF CONSECUTIVE
  // ZERO BITS.  THE CONVERTED BLOCK OF 343 std::uint8_tS IS STORED STARTING AT 4K
  // INTO THE WORK BUFFER.
  {
    std::uint8_t * sourceptr = workbuffer+0x1400;
    std::uint8_t * resultptr = workbuffer+0x1000;
    int    loop      = 343;
    while (loop--)
      *(resultptr++) = diskbyte[(*(sourceptr++)) >> 2];
  }

  return workbuffer+0x1000;
}

//===========================================================================
void Decode62 (std::uint8_t * imageptr) {

  // IF WE HAVEN'T ALREADY DONE SO, GENERATE A TABLE FOR CONVERTING
  // DISK std::uint8_tS BACK INTO 6-BIT std::uint8_tS
  static BOOL tablegenerated = 0;
  static std::uint8_t sixbitbyte[0x80];
  if (!tablegenerated) {
    ZeroMemory(sixbitbyte,0x80);
    int loop = 0;
    while (loop < 0x40) {
      sixbitbyte[diskbyte[loop]-0x80] = loop << 2;
      loop++;
    }
    tablegenerated = 1;
  }

  // USING OUR TABLE, CONVERT THE DISK std::uint8_tS BACK INTO 6-BIT std::uint8_tS
  {
    std::uint8_t * sourceptr = workbuffer+0x1000;
    std::uint8_t * resultptr = workbuffer+0x1400;
    int    loop      = 343;
    while (loop--)
      *(resultptr++) = sixbitbyte[*(sourceptr++) & 0x7F];
  }

  // EXCLUSIVE-OR THE ENTIRE DATA BLOCK WITH ITSELF OFFSET BY ONE std::uint8_t
  // TO UNDO THE EFFECTS OF THE CHECKSUMMING PROCESS
  {
    std::uint8_t   savedval  = 0;
    std::uint8_t * sourceptr = workbuffer+0x1400;
    std::uint8_t * resultptr = workbuffer+0x1000;
    int    loop      = 342;
    while (loop--) {
      *resultptr = savedval ^ *(sourceptr++);
      savedval = *(resultptr++);
    }
  }

  // CONVERT THE 342 6-BIT std::uint8_tS INTO 256 8-BIT std::uint8_tS
  {
    std::uint8_t * lowbitsptr = workbuffer+0x1000;
    std::uint8_t * sectorbase = workbuffer+0x1056;
    std::uint8_t   offset     = 0xAC;
    while (offset != 0x02) {
      if (offset >= 0xAC)
        *(imageptr+offset) = (*(sectorbase+offset) & 0xFC)
                               | (((*lowbitsptr) & 0x80) >> 7)
                               | (((*lowbitsptr) & 0x40) >> 5);
      offset -= 0x56;
      *(imageptr+offset) = (*(sectorbase+offset) & 0xFC)
                             | (((*lowbitsptr) & 0x20) >> 5)
                             | (((*lowbitsptr) & 0x10) >> 3);
      offset -= 0x56;
      *(imageptr+offset) = (*(sectorbase+offset) & 0xFC)
                             | (((*lowbitsptr) & 0x08) >> 3)
                             | (((*lowbitsptr) & 0x04) >> 1);
      offset -= 0x53;
      lowbitsptr++;
    }
  }

}

//===========================================================================
void DenibblizeTrack (std::uint8_t * trackimage, BOOL dosorder, int nibbles) {
  ZeroMemory(workbuffer,0x1000);

  // SEARCH THROUGH THE TRACK IMAGE FOR EACH SECTOR.  FOR EVERY SECTOR
  // WE FIND, COPY THE NIBBLIZED DATA FOR THAT SECTOR INTO THE WORK
  // BUFFER AT OFFSET 4K.  THEN CALL DECODE62() TO DENIBBLIZE THE DATA
  // IN THE BUFFER AND WRITE IT INTO THE FIRST PART OF THE WORK BUFFER
  // OFFSET BY THE SECTOR NUMBER.
  {
    int offset    = 0;
    int partsleft = 33;
    int sector    = 0;
    while (partsleft--) {
      std::uint8_t byteval[3] = {0,0,0};
      int  bytenum    = 0;
      int  loop       = nibbles;
      while ((loop--) && (bytenum < 3)) {
        if (bytenum)
          byteval[bytenum++] = *(trackimage+offset++);
        else if (*(trackimage+offset++) == 0xD5)
          bytenum = 1;
        if (offset >= nibbles)
          offset = 0;
      }
      if ((bytenum == 3) && (byteval[1] = 0xAA)) {
        int loop       = 0;
        int tempoffset = offset;
        while (loop < 384) {
          *(workbuffer+0x1000+loop++) = *(trackimage+tempoffset++);
          if (tempoffset >= nibbles)
            tempoffset = 0;
        }
        if (byteval[2] == 0x96)
          sector = ((*(workbuffer+0x1004) & 0x55) << 1)
                     | (*(workbuffer+0x1005) & 0x55);
        else if (byteval[2] == 0xAD) {
          Decode62(workbuffer+(sectornumber[dosorder][sector] << 8));
          sector = 0;
        }
      }
    }
  }
}

//===========================================================================
DWORD NibblizeTrack (std::uint8_t * trackimagebuffer, BOOL dosorder, int track) {
  ZeroMemory(workbuffer+4096,4096);
  std::uint8_t * imageptr = trackimagebuffer;
  std::uint8_t   sector   = 0;

  // WRITE GAP ONE, WHICH CONTAINS 48 SELF-SYNC std::uint8_tS
  int loop;
  for (loop = 0; loop < 48; loop++)
    *(imageptr++) = 0xFF;

  while (sector < 16) {

    // WRITE THE ADDRESS FIELD, WHICH CONTAINS:
    //   - PROLOGUE (D5AA96)
    //   - VOLUME NUMBER ("4 AND 4" ENCODED)
    //   - TRACK NUMBER ("4 AND 4" ENCODED)
    //   - SECTOR NUMBER ("4 AND 4" ENCODED)
    //   - CHECKSUM ("4 AND 4" ENCODED)
    //   - EPILOGUE (DEAAEB)
    *(imageptr++) = 0xD5;
    *(imageptr++) = 0xAA;
    *(imageptr++) = 0x96;
#define VOLUME 0xFE
#define CODE44A(a) ((((a) >> 1) & 0x55) | 0xAA)
#define CODE44B(a) (((a) & 0x55) | 0xAA)
    *(imageptr++) = CODE44A(VOLUME);
    *(imageptr++) = CODE44B(VOLUME);
    *(imageptr++) = CODE44A((std::uint8_t)track);
    *(imageptr++) = CODE44B((std::uint8_t)track);
    *(imageptr++) = CODE44A(sector);
    *(imageptr++) = CODE44B(sector);
    *(imageptr++) = CODE44A(VOLUME ^ ((std::uint8_t)track) ^ sector);
    *(imageptr++) = CODE44B(VOLUME ^ ((std::uint8_t)track) ^ sector);
#undef CODE44A
#undef CODE44B
    *(imageptr++) = 0xDE;
    *(imageptr++) = 0xAA;
    *(imageptr++) = 0xEB;

    // WRITE GAP TWO, WHICH CONTAINS SIX SELF-SYNC std::uint8_tS
    for (loop = 0; loop < 6; loop++)
      *(imageptr++) = 0xFF;

    // WRITE THE DATA FIELD, WHICH CONTAINS:
    //   - PROLOGUE (D5AAAD)
    //   - 343 6-BIT std::uint8_tS OF NIBBLIZED DATA, INCLUDING A 6-BIT CHECKSUM
    //   - EPILOGUE (DEAAEB)
    *(imageptr++) = 0xD5;
    *(imageptr++) = 0xAA;
    *(imageptr++) = 0xAD;
    CopyMemory(imageptr,Code62(sectornumber[dosorder][sector]),343);
    imageptr += 343;
    *(imageptr++) = 0xDE;
    *(imageptr++) = 0xAA;
    *(imageptr++) = 0xEB;

    // WRITE GAP THREE, WHICH CONTAINS 27 SELF-SYNC std::uint8_tS
    for (loop = 0; loop < 27; loop++)
      *(imageptr++) = 0xFF;

    sector++;
  }
  return imageptr-trackimagebuffer;
}

//===========================================================================
void SkewTrack (int track, int nibbles, std::uint8_t * trackimagebuffer) {
  int skewbytes = (track*768) % nibbles;
  CopyMemory(workbuffer,trackimagebuffer,nibbles);
  CopyMemory(trackimagebuffer,workbuffer+skewbytes,nibbles-skewbytes);
  CopyMemory(trackimagebuffer+nibbles-skewbytes,workbuffer,skewbytes);
}

/****************************************************************************
*
*  RAW PROGRAM IMAGE (APL) FORMAT IMPLEMENTATION
*
***/

//===========================================================================
BOOL AplBoot (imageinfoptr ptr) {
  SetFilePointer(ptr->file,0,nullptr,FILE_BEGIN);
  WORD address = 0;
  WORD length  = 0;
  DWORD bytesread;
  ReadFile(ptr->file,&address,sizeof(WORD),&bytesread,nullptr);
  ReadFile(ptr->file,&length ,sizeof(WORD),&bytesread,nullptr);
  if ((((WORD)(address+length)) <= address) ||
      (address >= 0xC000) ||
      (address+length-1 >= 0xC000))
    return 0;
  ReadFile(ptr->file,mem+address,length,&bytesread,nullptr);
  int loop = 192;
  while (loop--)
    *(memdirty+loop) = 0xFF;
  regs.pc = address;
  return 1;
}

//===========================================================================
DWORD AplDetect (std::uint8_t * imageptr, DWORD imagesize) {
  DWORD length = *(LPWORD)(imageptr+2);
  return (((length+4) == imagesize) ||
          ((length+4+((256-((length+4) & 255)) & 255)) == imagesize));
}

/****************************************************************************
*
*  DOS ORDER (DO) FORMAT IMPLEMENTATION
*
***/

//===========================================================================
DWORD DoDetect (std::uint8_t * imageptr, DWORD imagesize) {
  if (((imagesize < 143105) || (imagesize > 143364)) &&
      (imagesize != 143403) && (imagesize != 143488))
    return 0;

  // CHECK FOR A DOS ORDER IMAGE OF A DOS DISKETTE
  {
    int  loop     = 0;
    BOOL mismatch = 0;
    while ((loop++ < 15) && !mismatch)
      if (*(imageptr+0x11002+(loop << 8)) != loop-1)
        mismatch = 1;
    if (!mismatch)
      return 2;
  }

  // CHECK FOR A DOS ORDER IMAGE OF A PRODOS DISKETTE
  {
    int  loop     = 1;
    BOOL mismatch = 0;
    while ((loop++ < 5) && !mismatch)
      if ((*(LPWORD)(imageptr+(loop << 9)+0x100) != ((loop == 5) ? 0 : 6-loop)) ||
          (*(LPWORD)(imageptr+(loop << 9)+0x102) != ((loop == 2) ? 0 : 8-loop)))
        mismatch = 1;
    if (!mismatch)
      return 2;
  }

  return 1;
}

//===========================================================================
void DoRead (imageinfoptr ptr, int track, int quartertrack, std::uint8_t * trackimagebuffer, int *nibbles) {
  SetFilePointer(ptr->file,ptr->offset+(track << 12),nullptr,FILE_BEGIN);
  ZeroMemory(workbuffer,4096);
  DWORD bytesread;
  ReadFile(ptr->file,workbuffer,4096,&bytesread,nullptr);
  *nibbles = NibblizeTrack(trackimagebuffer,1,track);
  if (!enhancedisk)
    SkewTrack(track,*nibbles,trackimagebuffer);
}

//===========================================================================
void DoWrite (imageinfoptr ptr, int track, int quartertrack, std::uint8_t * trackimage, int nibbles) {
  ZeroMemory(workbuffer,4096);
  DenibblizeTrack(trackimage,1,nibbles);
  SetFilePointer(ptr->file,ptr->offset+(track << 12),nullptr,FILE_BEGIN);
  DWORD byteswritten;
  WriteFile(ptr->file,workbuffer,4096,&byteswritten,nullptr);
}

/****************************************************************************
*
*  SIMSYSTEM IIE (IIE) FORMAT IMPLEMENTATION
*
***/

//===========================================================================
void IieConvertSectorOrder (std::uint8_t * sourceorder) {
  int loop = 16;
  while (loop--) {
    std::uint8_t found = 0xFF;
    int  loop2 = 16;
    while (loop2-- && (found == 0xFF))
      if (*(sourceorder+loop2) == loop)
        found = loop2;
    if (found == 0xFF)
      found = 0;
    sectornumber[2][loop] = found;
  }
}

//===========================================================================
DWORD IieDetect (std::uint8_t * imageptr, DWORD imagesize) {
  if (strncmp((const char *)imageptr,"SIMSYSTEM_IIE",13) ||
      (*(imageptr+13) > 3))
    return 0;
  return 2;
}

//===========================================================================
void IieRead (imageinfoptr ptr, int track, int quartertrack, std::uint8_t * trackimagebuffer, int *nibbles) {

  // IF WE HAVEN'T ALREADY DONE SO, READ THE IMAGE FILE HEADER
  if (!ptr->header) {
	  ptr->header = (std::uint8_t *)VirtualAlloc(nullptr, 88, 0x1000, 0);
    if (!ptr->header) {
      *nibbles = 0;
      return;
    }
    ZeroMemory(ptr->header,88);
    DWORD bytesread;
    SetFilePointer(ptr->file,0,nullptr,FILE_BEGIN);
    ReadFile(ptr->file,ptr->header,88,&bytesread,nullptr);
  }

  // IF THIS IMAGE CONTAINS USER DATA, READ THE TRACK AND NIBBLIZE IT
  if (*(ptr->header+13) <= 2) {
    IieConvertSectorOrder(ptr->header+14);
    SetFilePointer(ptr->file,(track << 12)+30,nullptr,FILE_BEGIN);
    ZeroMemory(workbuffer,4096);
    DWORD bytesread;
    ReadFile(ptr->file,workbuffer,4096,&bytesread,nullptr);
    *nibbles = NibblizeTrack(trackimagebuffer,2,track);
  }

  // OTHERWISE, IF THIS IMAGE CONTAINS NIBBLE INFORMATION, READ IT
  // DIRECTLY INTO THE TRACK BUFFER
  else {
    *nibbles = *(LPWORD)(ptr->header+(track << 1)+14);
    DWORD offset = 88;
    while (track--)
      offset += *(LPWORD)(ptr->header+(track << 1)+14);
    SetFilePointer(ptr->file,offset,nullptr,FILE_BEGIN);
    ZeroMemory(trackimagebuffer,*nibbles);
    DWORD bytesread;
    ReadFile(ptr->file,trackimagebuffer,*nibbles,&bytesread,nullptr);
  }

}

//===========================================================================
void IieWrite (imageinfoptr ptr, int track, int quartertrack, std::uint8_t * trackimage, int nibbles) {
  // note: unimplemented
}

/****************************************************************************
*
*  NIBBLIZED 6656-NIBBLE (NIB) FORMAT IMPLEMENTATION
*
***/

//===========================================================================
DWORD Nib1Detect (std::uint8_t * imageptr, DWORD imagesize) {
  return (imagesize == 232960) ? 2 : 0;
}

//===========================================================================
void Nib1Read (imageinfoptr ptr, int track, int quartertrack, std::uint8_t * trackimagebuffer, int *nibbles) {
  SetFilePointer(ptr->file,ptr->offset+track*NIBBLES,nullptr,FILE_BEGIN);
  ReadFile(ptr->file,trackimagebuffer,NIBBLES,(DWORD *)nibbles,nullptr);
}

//===========================================================================
void Nib1Write (imageinfoptr ptr, int track, int quartertrack, std::uint8_t * trackimage, int nibbles) {
  SetFilePointer(ptr->file,ptr->offset+track*NIBBLES,nullptr,FILE_BEGIN);
  DWORD byteswritten;
  WriteFile(ptr->file,trackimage,nibbles,&byteswritten,nullptr);
}

/****************************************************************************
*
*  NIBBLIZED 6384-NIBBLE (NB2) FORMAT IMPLEMENTATION
*
***/

//===========================================================================
DWORD Nib2Detect (std::uint8_t * imageptr, DWORD imagesize) {
  return (imagesize == 223440) ? 2 : 0;
}

//===========================================================================
void Nib2Read (imageinfoptr ptr, int track, int quartertrack, std::uint8_t * trackimagebuffer, int *nibbles) {
  SetFilePointer(ptr->file,ptr->offset+track*6384,nullptr,FILE_BEGIN);
  ReadFile(ptr->file,trackimagebuffer,6384,(DWORD *)nibbles,nullptr);
}

//===========================================================================
void Nib2Write (imageinfoptr ptr, int track, int quartertrack, std::uint8_t * trackimage, int nibbles) {
  SetFilePointer(ptr->file,ptr->offset+track*6384,nullptr,FILE_BEGIN);
  DWORD byteswritten;
  WriteFile(ptr->file,trackimage,nibbles,&byteswritten,nullptr);
}

/****************************************************************************
*
*  PRODOS ORDER (PO) FORMAT IMPLEMENTATION
*
***/

//===========================================================================
DWORD PoDetect (std::uint8_t * imageptr, DWORD imagesize) {
  if (((imagesize < 143105) || (imagesize > 143364)) &&
      (imagesize != 143488))
    return 0;

  // CHECK FOR A PRODOS ORDER IMAGE OF A DOS DISKETTE
  {
    int  loop     = 4;
    BOOL mismatch = 0;
    while ((loop++ < 13) && !mismatch)
      if (*(imageptr+0x11002+(loop << 8)) != 14-loop)
        mismatch = 1;
    if (!mismatch)
      return 2;
  }

  // CHECK FOR A PRODOS ORDER IMAGE OF A PRODOS DISKETTE
  {
    int  loop     = 1;
    BOOL mismatch = 0;
    while ((loop++ < 5) && !mismatch)
      if ((*(LPWORD)(imageptr+(loop << 9)  ) != ((loop == 2) ? 0 : loop-1)) ||
          (*(LPWORD)(imageptr+(loop << 9)+2) != ((loop == 5) ? 0 : loop+1)))
        mismatch = 1;
    if (!mismatch)
      return 2;
  }

  return 1;
}

//===========================================================================
void PoRead (imageinfoptr ptr, int track, int quartertrack, std::uint8_t * trackimagebuffer, int *nibbles) {
  SetFilePointer(ptr->file,ptr->offset+(track << 12),nullptr,FILE_BEGIN);
  ZeroMemory(workbuffer,4096);
  DWORD bytesread;
  ReadFile(ptr->file,workbuffer,4096,&bytesread,nullptr);
  *nibbles = NibblizeTrack(trackimagebuffer,0,track);
  if (!enhancedisk)
    SkewTrack(track,*nibbles,trackimagebuffer);
}

//===========================================================================
void PoWrite (imageinfoptr ptr, int track, int quartertrack, std::uint8_t * trackimage, int nibbles) {
  ZeroMemory(workbuffer,4096);
  DenibblizeTrack(trackimage,0,nibbles);
  SetFilePointer(ptr->file,ptr->offset+(track << 12),nullptr,FILE_BEGIN);
  DWORD byteswritten;
  WriteFile(ptr->file,workbuffer,4096,&byteswritten,nullptr);
}

/****************************************************************************
*
*  PRODOS PROGRAM IMAGE (PRG) FORMAT IMPLEMENTATION
*
***/

//===========================================================================
BOOL PrgBoot (imageinfoptr ptr) {
  SetFilePointer(ptr->file,5,nullptr,FILE_BEGIN);
  WORD address = 0;
  WORD length  = 0;
  DWORD bytesread;
  ReadFile(ptr->file,&address,sizeof(WORD),&bytesread,nullptr);
  ReadFile(ptr->file,&length ,sizeof(WORD),&bytesread,nullptr);
  length <<= 1;
  if ((((WORD)(address+length)) <= address) ||
      (address >= 0xC000) ||
      (address+length-1 >= 0xC000))
    return 0;
  SetFilePointer(ptr->file,128,nullptr,FILE_BEGIN);
  ReadFile(ptr->file,mem+address,length,&bytesread,nullptr);
  int loop = 192;
  while (loop--)
    *(memdirty+loop) = 0xFF;
  regs.pc = address;
  return 1;
}

//===========================================================================
DWORD PrgDetect (std::uint8_t * imageptr, DWORD imagesize) {
  return (*(LPDWORD)imageptr == 0x214C470A) ? 2 : 0;
}

//
// ----- ALL GLOBALLY ACCESSIBLE FUNCTIONS ARE BELOW THIS LINE -----
//

//===========================================================================
BOOL ImageBoot (HIMAGE imagehandle) {
  imageinfoptr ptr = (imageinfoptr)imagehandle;
  BOOL result = 0;
  if (imagetype[ptr->format].boot)
    result = imagetype[ptr->format].boot(ptr);
  if (result)
    ptr->writeprotected = 1;
  return result;
}

//===========================================================================
void ImageClose (HIMAGE imagehandle) {
  imageinfoptr ptr = (imageinfoptr)imagehandle;
  if (ptr->file != INVALID_HANDLE_VALUE)
    CloseHandle(ptr->file);
  for (int track = 0; track < TRACKS; track++)
    if (!ptr->validtrack[track]) {
      DeleteFile(ptr->filename);
      break;
    }
  if (ptr->header)
    VirtualFree(ptr->header,0,/*MEM_RELEASE*/0);
  VirtualFree(ptr,0,/*MEM_RELEASE*/0);
}

//===========================================================================
void ImageDestroy () {
  VirtualFree(workbuffer,0,/*MEM_RELEASE*/0);
  workbuffer = nullptr;
}

//===========================================================================
void ImageInitialize () {
  workbuffer = (std::uint8_t *)VirtualAlloc(nullptr,0x2000,0x1000,/*PAGE_READWRITE*/0);
}

//===========================================================================
int ImageOpen (const char *  imagefilename,
               HIMAGE  *hDiskImage_,
               BOOL    *pWriteProtected_,
               BOOL     bCreateIfNecessary)
{
	if (! (imagefilename && hDiskImage_ && pWriteProtected_ && workbuffer))
		return IMAGE_ERROR_BAD_POINTER; // HACK: MAGIC # -1

	// TRY TO OPEN THE IMAGE FILE
	HANDLE file = INVALID_HANDLE_VALUE;

	if (! *pWriteProtected_)
// 		file = CreateFile(imagefilename,
//                       GENERIC_READ | GENERIC_WRITE,
//                       FILE_SHARE_READ | FILE_SHARE_WRITE,
//                       (LPSECURITY_ATTRIBUTES)nullptr,
//                       OPEN_EXISTING,
//                       FILE_ATTRIBUTE_NORMAL,
//                       nullptr);
		file = fopen(imagefilename, "r+b"); // open file in r/w mode
	// File may have read-only attribute set, so try to open as read-only.
	if (file == INVALID_HANDLE_VALUE)
	{
// 		file = CreateFile(
// 			imagefilename,
// 			GENERIC_READ,
// 			FILE_SHARE_READ,
// 			(LPSECURITY_ATTRIBUTES)nullptr,
// 			OPEN_EXISTING,
// 			FILE_ATTRIBUTE_NORMAL,
// 			nullptr );
		file = fopen(imagefilename, "rb"); // open file just for reading

		if (file != INVALID_HANDLE_VALUE)
			*pWriteProtected_ = 1;
	}

	if ((file == INVALID_HANDLE_VALUE) && bCreateIfNecessary)
// 		file = CreateFile(
// 			imagefilename,
// 			GENERIC_READ | GENERIC_WRITE,
// 			FILE_SHARE_READ | FILE_SHARE_WRITE,
// 			(LPSECURITY_ATTRIBUTES)nullptr,
// 			CREATE_NEW,
// 			FILE_ATTRIBUTE_NORMAL,
// 			nullptr );
		file = fopen(imagefilename, "a+b"); // create file

	// IF WE AREN'T ABLE TO OPEN THE FILE, RETURN
	if (file == INVALID_HANDLE_VALUE)
		return IMAGE_ERROR_UNABLE_TO_OPEN; // HACK: MAGIC # 1

	// DETERMINE THE FILE'S EXTENSION AND CONVERT IT TO LOWERCASE
	const char * imagefileext = imagefilename;
	if (_tcsrchr(imagefileext,FILE_SEPARATOR))
	imagefileext = _tcsrchr(imagefileext,FILE_SEPARATOR)+1;
	if (_tcsrchr(imagefileext,TEXT('.')))
	imagefileext = _tcsrchr(imagefileext,TEXT('.'));

#define _MAX_EXT	5
	char ext[_MAX_EXT];
	_tcsncpy(ext,imagefileext,_MAX_EXT);
	CharLowerBuff(ext,_tcslen(ext));

	DWORD  size     = GetFileSize(file,nullptr);
	std::uint8_t * view     = nullptr;
	std::uint8_t * pImage = nullptr;

	const DWORD UNKNOWN_FORMAT = 0xFFFFFFFF;
	DWORD  format   = UNKNOWN_FORMAT;

	if (size > 0)
	{
		// MAP THE FILE INTO MEMORY FOR USE BY THE DETECTION FUNCTIONS
/*		HANDLE mapping = CreateFileMapping(
			file,
			(LPSECURITY_ATTRIBUTES)nullptr,
			PAGE_READONLY,
			0,0,nullptr );

		view     = (std::uint8_t *)MapViewOfFile(mapping,FILE_MAP_READ,0,0,0);*/
		view = (std::uint8_t *)malloc(size);
		fread(view, 1, size, (FILE*)file);
		fseek((FILE*)file, 0, FILE_BEGIN); // I just got accustomed to mrsftish FILE_BEGIN, FILE_END, etc. Hmm. ^_^
		pImage = view;

		if (pImage)
		{
			// DETERMINE WHETHER THE FILE HAS A 128-BYTE MACBINARY HEADER
			if ((size > 128) &&
				(!*pImage) &&
				(*(pImage+1) < 120) &&
				(!*(pImage+*(pImage+1)+2)) &&
				(*(pImage+0x7A) == 0x81) &&
				(*(pImage+0x7B) == 0x81))
			{
				pImage += 128;
				size     -= 128;
			}

			// CALL THE DETECTION FUNCTIONS IN ORDER, LOOKING FOR A MATCH
			DWORD possibleformat = UNKNOWN_FORMAT; // 0xFFFFFFFF;
			int   loop           = 0;
			while ((loop < IMAGETYPES) && (format == UNKNOWN_FORMAT)) // 0xFFFFFFFF)) {
			{
				if (*ext && _tcsstr(imagetype[loop].rejectexts,ext))
					++loop;
				else
				{
					DWORD result = imagetype[loop].detect(pImage,size);
					if (result == 2)
						format = loop;
					else if ((result == 1) && (possibleformat == UNKNOWN_FORMAT)) // 0xFFFFFFFF))
						possibleformat = loop++;
					else
						++loop;
				}
			}

			if (format == UNKNOWN_FORMAT) // 0xFFFFFFFF)
			format = possibleformat;

			// CLOSE THE MEMORY MAPPING
		//	UnmapViewOfFile(view);
			free(view); // free memory block, allocated before
		}
	    //CloseHandle(mapping);
	}
	else
	{
		// WE CREATE ONLY DOS ORDER (DO) OR 6656-NIBBLE (NIB) FORMAT FILES
		for (int loop = 1; loop <= 4; loop += 3)
		{
			if (*ext && _tcsstr(imagetype[loop].createexts,ext))
			{
				format = loop;
				break;
			}

		}
	}

	// IF THE FILE DOES MATCH A KNOWN FORMAT...
	if (format != UNKNOWN_FORMAT)
	{
		// CREATE A RECORD FOR THE FILE, AND RETURN AN IMAGE HANDLE
		*hDiskImage_ = (HIMAGE)VirtualAlloc(nullptr,sizeof(imageinfo),0x1000,0);
		if (*hDiskImage_)
		{
			ZeroMemory(*hDiskImage_,sizeof(imageinfo));
			//   do this in DiskInsert vv
			_tcsncpy(((imageinfoptr)*hDiskImage_)->filename,imagefilename,MAX_PATH);
			((imageinfoptr)*hDiskImage_)->format         = format;
			((imageinfoptr)*hDiskImage_)->file           = file;
			((imageinfoptr)*hDiskImage_)->offset         = pImage-view;
			((imageinfoptr)*hDiskImage_)->writeprotected = *pWriteProtected_;

			for (int track = 0; track < TRACKS; track++)
		        ((imageinfoptr)*hDiskImage_)->validtrack[track] = (size > 0);

			return IMAGE_ERROR_NONE; // HACK: MAGIC # 0
		}
	}

	CloseHandle(file);
	if (!(size > 0))
		DeleteFile(imagefilename);

	return IMAGE_ERROR_BAD_SIZE; // HACK: MAGIC # 2
}

//===========================================================================
void ImageReadTrack (HIMAGE  imagehandle,
                     int     track,
                     int     quartertrack,
                     std::uint8_t *  trackimagebuffer,
                     int    *nibbles) {
  imageinfoptr ptr = (imageinfoptr)imagehandle;
  if (imagetype[ptr->format].read && ptr->validtrack[track])
    imagetype[ptr->format].read(ptr,track,quartertrack,trackimagebuffer,nibbles);
  else
    for (*nibbles = 0; *nibbles < NIBBLES; (*nibbles)++)
      trackimagebuffer[*nibbles] = (std::uint8_t)(rand() & 0xFF);
}

//===========================================================================
void ImageWriteTrack (HIMAGE imagehandle,
                      int    track,
                      int    quartertrack,
                      std::uint8_t * trackimage,
                      int    nibbles) {
  imageinfoptr ptr = (imageinfoptr)imagehandle;
  if (imagetype[ptr->format].write && !ptr->writeprotected) {
    imagetype[ptr->format].write(ptr,track,quartertrack,trackimage,nibbles);
    ptr->validtrack[track] = 1;
  }
}
