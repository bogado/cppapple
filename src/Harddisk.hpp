#pragma once

extern bool g_bHD_Enabled;

bool    HD_CardIsEnabled();
void    HD_SetEnabled(bool bEnabled);
const char * HD_GetFullName (int drive);
VOID    HD_Load_Rom(std::uint8_t * pCxRomPeripheral, UINT uSlot);
VOID    HD_Cleanup();
BOOL    HD_InsertDisk2(int nDrive, const char * pszFilename);
BOOL    HD_InsertDisk(int nDrive, const char * imagefilename);

void    HD_Select(int nDrive);
void 	HD_FTP_Select(int nDrive);

int	HD_GetStatus(void);
void	HD_ResetStatus(void);
