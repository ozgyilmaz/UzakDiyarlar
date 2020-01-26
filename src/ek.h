/***************************************************************************
*    Uzak Diyarlar, Rom2.4b6 tabanlý Türkçe Mud                            *
*    http://www.uzakdiyarlar.net                                           *
***************************************************************************/
#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "merc.h"

char * son_harf_unlu(CHAR_DATA *ch, int say);

struct pers_suffix_type
{
    char *belirtec;
    char *birisi;
	char *olumsuz;
	char *sesli_eki;
	char *bir;//a,ý
	char *iki;//e,i
	char *uc;//o,u
	char *dort;//ö,ü
};

extern	const	struct	pers_suffix_type	pers_suffix_table[];
