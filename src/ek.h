/***************************************************************************
 *                                                                         *
 * Uzak Diyarlar açık kaynak Türkçe Mud projesidir.                        *
 * Oyun geliştirmesi Jai ve Maru tarafından yönetilmektedir.               *
 * Unutulmaması gerekenler: Nir, Kame, Nyah, Sint                          *
 *                                                                         *
 * Github  : https://github.com/yelbuke/UzakDiyarlar                       *
 * Web     : http://www.uzakdiyarlar.net                                   *
 * Discord : https://discord.gg/kXyZzv                                     *
 *                                                                         *
 ***************************************************************************/
 
#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <time.h>
#include "merc.h"

wchar_t * son_harf_unlu(CHAR_DATA *ch, int say);

struct pers_suffix_type
{
    wchar_t *belirtec;
    wchar_t *birisi;
	wchar_t *olumsuz;
	wchar_t *sesli_eki;
	wchar_t *bir;//a,ı
	wchar_t *iki;//e,i
	wchar_t *uc;//o,u
	wchar_t *dort;//ö,ü
};

extern	const	struct	pers_suffix_type	pers_suffix_table[];
