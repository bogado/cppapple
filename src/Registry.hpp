#pragma once

// configuration file
#define REGISTRY	"/opt/retropie/configs/apple2/linapple.conf"
extern FILE * registry;	// our opened file

BOOL    RegLoadString (const char *,const char *,BOOL,char**,DWORD);
BOOL    RegLoadValue (const char *,const char *,BOOL,DWORD *);
BOOL	RegLoadBool(const char *,const char *,BOOL,BOOL *);

void    RegSaveString (const char *,const char *,BOOL,const char *);
void    RegSaveValue (const char *,const char *,BOOL,DWORD);
void    RegSaveBool (const char *,const char *,BOOL,BOOL);

char	*php_trim(char *c, int len);	// trimming string like PHP function trim does!

