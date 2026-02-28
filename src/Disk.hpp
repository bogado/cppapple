#pragma once

#define  DRIVE_1  0
#define  DRIVE_2  1

#define  DRIVES   2
#define  TRACKS   35

extern bool       enhancedisk;

void    DiskInitialize (); // DiskManagerStartup()
void    DiskDestroy (); // no, doesn't "destroy" the disk image.  DiskManagerShutdown()

void    DiskBoot ();
void    DiskEject( const int iDrive );
const char * DiskGetFullName (int);


enum Disk_Status_e
{
	DISK_STATUS_OFF  ,
	DISK_STATUS_READ ,
	DISK_STATUS_WRITE,
	DISK_STATUS_PROT ,
	NUM_DISK_STATUS
};
void    DiskGetLightStatus (int *pDisk1Status_,int *pDisk2Status_);

const char * DiskGetName (int);
int     DiskInsert (int,const char *,bool,bool);
bool    DiskIsSpinning ();
void    DiskNotifyInvalidImage (const char *,int);
void    DiskReset ();
bool    DiskGetProtect( const int iDrive );
void    DiskSetProtect( const int iDrive, const bool bWriteProtect );
void    DiskSelect (int);
void 	Disk_FTP_SelectImage (int);
void    DiskUpdatePosition (std::uint32_t);
bool    DiskDriveSwap();
void    DiskLoadRom(std::uint8_t * pCxRomPeripheral, unsigned uSlot);
std::uint32_t   DiskGetSnapshot(SS_CARD_DISK2* pSS, std::uint32_t dwSlot);
std::uint32_t   DiskSetSnapshot(SS_CARD_DISK2* pSS, std::uint32_t dwSlot);
