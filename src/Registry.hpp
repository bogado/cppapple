#pragma once

// configuration file
#define REGISTRY	"/opt/retropie/configs/apple2/linapple.conf"
extern FILE * registry;	// our opened file

bool    RegLoadString (const char *,const char *,bool,char**,std::uint32_t);
bool    RegLoadValue (const char *,const char *,bool,std::uint32_t *);
bool	RegLoadBool(const char *,const char *,bool,bool *);

void    RegSaveString (const char *,const char *,bool,const char *);
void    RegSaveValue (const char *,const char *,bool,std::uint32_t);
void    RegSaveBool (const char *,const char *,bool,bool);

char	*php_trim(char *c, int len);	// trimming string like PHP function trim does!

