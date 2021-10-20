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
 
/***************************************************************************
 *     ANATOLIA 2.1 is copyright 1996-1997 Serdar BULUT, Ibrahim CANPUNAR  *
 *     ANATOLIA has been brought to you by ANATOLIA consortium		   *
 *	 Serdar BULUT {Chronos}		bulut@rorqual.cc.metu.edu.tr       *
 *	 Ibrahim Canpunar  {Asena}	canpunar@rorqual.cc.metu.edu.tr    *
 *	 Murat BICER  {KIO}		mbicer@rorqual.cc.metu.edu.tr	   *
 *	 D.Baris ACAR {Powerman}	dbacar@rorqual.cc.metu.edu.tr	   *
 *     By using this code, you have agreed to follow the terms of the      *
 *     ANATOLIA license, in the file Anatolia/anatolia.licence             *
 ***************************************************************************/

/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

/***************************************************************************
*	ROM 2.4 is copyright 1993-1995 Russ Taylor			   *
*	ROM has been brought to you by the ROM consortium		   *
*	    Russ Taylor (rtaylor@pacinfo.com)				   *
*	    Gabrielle Taylor (gtaylor@pacinfo.com)			   *
*	    Brian Moore (rom@rom.efn.org)				   *
*	By using this code, you have agreed to follow the terms of the	   *
*	ROM license, in the file Rom24/doc/rom.license			   *
***************************************************************************/

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <wchar.h>
#include <wctype.h>
#include <time.h>
#include "merc.h"
#include "tables.h"

int flag_lookup (const wchar_t *name, const struct flag_type *flag_table)
{
    int flag;

    for (flag = 0; flag_table[flag].name != NULL; flag++)
    {
	if (towlower(name[0]) == towlower(flag_table[flag].name[0])
	&&  !str_prefix(name,flag_table[flag].name))
	    return flag_table[flag].bit;
    }

    return 0;
}


int position_lookup (const wchar_t *name)
{
   int pos;

   for (pos = 0; position_table[pos].name != NULL; pos++)
   {
	if (towlower(name[0]) == towlower(position_table[pos].name[0])
	&&  !str_prefix(name,position_table[pos].name))
	    return pos;
   }

   return -1;
}

int sex_lookup (const wchar_t *name)
{
   int sex;

   for (sex = 0; sex_table[sex].name != NULL; sex++)
   {
	if (towlower(name[0]) == towlower(sex_table[sex].name[0])
	&&  !str_prefix(name,sex_table[sex].name))
	    return sex;
   }

   return -1;
}

int size_lookup (const wchar_t *name)
{
   int size;

   for ( size = 0; size_table[size].name != NULL; size++)
   {
        if (towlower(name[0]) == towlower(size_table[size].name[0])
        &&  !str_prefix( name,size_table[size].name))
            return size;
   }

   return 0;
}

int lang_lookup (const wchar_t *name)
{
   int lang;

   if (towlower(name[0]) == 'a' && (!str_prefix(name,L"analisan")) )
    return MAX_LANGUAGE;

   for (lang = 0; lang < MAX_LANGUAGE; lang++)
   {
	if (towlower(name[0]) == towlower(language_table[lang].name[0])
	&&  !str_prefix(name,language_table[lang].name))
	    return lang;
   }

   return -1;
}
