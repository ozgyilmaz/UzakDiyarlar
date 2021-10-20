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
#include <time.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#endif
#include <stdio.h>
#include <wchar.h>
#include <wctype.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "merc.h"
#include "recycle.h"

int unlink(const wchar_t *pathname);

BAN_DATA *ban_list;

void save_bans(void)
{
    BAN_DATA *pban;
    FILE *fp;
    bool found = FALSE;
    wchar_t buf[160];

    fclose( fpReserve );
    if ( ( fp = fopen( BAN_FILE, "w" ) ) == NULL )
    {
        perror( BAN_FILE );
    }

    for (pban = ban_list; pban != NULL; pban = pban->next)
    {
	if (IS_SET(pban->ban_flags,BAN_PERMANENT))
	{
	    found = TRUE;
	    swprintf( buf, MAX_STRING_LENGTH-1, L"%-20s %-2d %s\n\r", pban->name, pban->level,print_flags(pban->ban_flags) );
	    dump_to_scr( buf );
	    fwprintf(fp, L"%-20s %-2d %s\n",pban->name,pban->level,
		print_flags(pban->ban_flags));
	}
     }

     fclose(fp);
     fpReserve = fopen( NULL_FILE, "r" );
     if (!found)
	unlink(BAN_FILE);

     if ( fpReserve == NULL )
	bug( L"ban_save: can't open null file.", 0 );
}

void load_bans(void)
{
    FILE *fp;
    BAN_DATA *ban_last;

    if ( ( fp = fopen( BAN_FILE, "r" ) ) == NULL )
        return;

    ban_last = NULL;
    for ( ; ; )
    {
        BAN_DATA *pban;
        if ( feof(fp) )
        {
            fclose( fp );
            return;
        }

        pban = new_ban();

        pban->name = str_dup(fread_word(fp));
	pban->level = fread_number(fp);
	pban->ban_flags = fread_flag(fp);
	fread_to_eol(fp);

        if (ban_list == NULL)
	    ban_list = pban;
	else
	    ban_last->next = pban;
	ban_last = pban;
    }
}

bool check_ban(wchar_t *site,int type)
{
    BAN_DATA *pban;
    wchar_t host[MAX_STRING_LENGTH];

    wcscpy(host,capitalize(site));
    host[0] = towlower(host[0]);

    for ( pban = ban_list; pban != NULL; pban = pban->next )
    {
	if(!IS_SET(pban->ban_flags,type))
	    continue;

	if (IS_SET(pban->ban_flags,BAN_PREFIX)
	&&  IS_SET(pban->ban_flags,BAN_SUFFIX)
	&&  wcsstr(pban->name,host) != NULL)
	    return TRUE;

	if (IS_SET(pban->ban_flags,BAN_PREFIX)
	&&  !str_suffix(pban->name,host))
	    return TRUE;

	if (IS_SET(pban->ban_flags,BAN_SUFFIX)
	&&  !str_prefix(pban->name,host))
	    return TRUE;
    }

    return FALSE;
}


void ban_site(CHAR_DATA *ch, wchar_t *argument, bool fPerm)
{
    wchar_t buf[MAX_STRING_LENGTH],buf2[MAX_STRING_LENGTH];
    wchar_t arg1[MAX_INPUT_LENGTH], arg2[MAX_INPUT_LENGTH];
    wchar_t *name;
    BUFFER *buffer;
    BAN_DATA *pban, *prev;
    bool prefix = FALSE,suffix = FALSE;
    int type;

    argument = one_argument(argument,arg1);
    argument = one_argument(argument,arg2);

    if ( arg1[0] == '\0' )
    {
	if (ban_list == NULL)
	{
	    send_to_char( L"Şu an yasaklı site yok.\n\r",ch);
	    return;
  	}
	buffer = new_buf();

        add_buf(buffer,(wchar_t*)"Banned sites  level  type     status\n\r");
        for (pban = ban_list;pban != NULL;pban = pban->next)
        {
	    swprintf(buf2, MAX_STRING_LENGTH-1, L"%s%s%s",
		IS_SET(pban->ban_flags,BAN_PREFIX) ? "*" : "",
		pban->name,
		IS_SET(pban->ban_flags,BAN_SUFFIX) ? "*" : "");
	    swprintf( buf, MAX_STRING_LENGTH-1, L"%-12s    %-3d  %-7s  %s\n\r",
		buf2, pban->level,
		IS_SET(pban->ban_flags,BAN_NEWBIES) ? "newbies" :
		IS_SET(pban->ban_flags,BAN_PLAYER)  ? "player" :
		IS_SET(pban->ban_flags,BAN_PERMIT)  ? "permit"  :
		IS_SET(pban->ban_flags,BAN_ALL)     ? "all"	: "",
	    	IS_SET(pban->ban_flags,BAN_PERMANENT) ? "perm" : "temp");
	    add_buf(buffer,buf);
        }

        page_to_char( buf_string(buffer), ch );
	free_buf(buffer);
        return;
    }

    /* find out what type of ban */
    if (arg2[0] == '\0' || !str_prefix(arg2, L"all"))
	type = BAN_ALL;
    else if (!str_prefix(arg2, L"newbies"))
	type = BAN_NEWBIES;
    else if (!str_prefix(arg2, L"player"))
	type = BAN_PLAYER;
    else if (!str_prefix(arg2, L"permit"))
	type = BAN_PERMIT;
    else
    {
	send_to_char( L"Acceptable ban types are all, newbies, player, and permit.\n\r",
	    ch);
	return;
    }

    name = arg1;

    if (name[0] == '*')
    {
	prefix = TRUE;
	name++;
    }

    if (name[wcslen(name) - 1] == '*')
    {
	suffix = TRUE;
	name[wcslen(name) - 1] = '\0';
    }

    if (wcslen(name) == 0)
    {
	send_to_char( L"Bir şeyi yasaklaman gerekiyor.\n\r",ch);
	return;
    }

    prev = NULL;
    for ( pban = ban_list; pban != NULL; prev = pban, pban = pban->next )
    {
        if (!wcscasecmp(name,pban->name))
        {
	    if (pban->level > get_trust(ch))
	    {
            	send_to_char( L"Bu yasak daha yüksek seviyeli biri tarafından koyulmuş.\n\r", ch );
            	return;
	    }
	    else
	    {
		if (prev == NULL)
		    ban_list = pban->next;
		else
		    prev->next = pban->next;
		free_ban(pban);
	    }
        }
    }

    pban = new_ban();
    pban->name = str_dup(name);
    pban->level = get_trust(ch);

    /* set ban type */
    pban->ban_flags = type;

    if (prefix)
	SET_BIT(pban->ban_flags,BAN_PREFIX);
    if (suffix)
	SET_BIT(pban->ban_flags,BAN_SUFFIX);
    if (fPerm)
	SET_BIT(pban->ban_flags,BAN_PERMANENT);

    pban->next  = ban_list;
    ban_list    = pban;
    save_bans();
    swprintf( buf, MAX_STRING_LENGTH-1, L"%s has been banned.\n\r",pban->name);
    send_to_char( buf, ch );
    return;
}

void do_ban(CHAR_DATA *ch, wchar_t *argument)
{
    ban_site(ch,argument,FALSE);
}

void do_permban(CHAR_DATA *ch, wchar_t *argument)
{
    ban_site(ch,argument,TRUE);
}

void do_allow( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH];
    wchar_t buf[MAX_STRING_LENGTH];
    BAN_DATA *prev;
    BAN_DATA *curr;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
        send_to_char( L"Remove which site from the ban list?\n\r", ch );
        return;
    }

    prev = NULL;
    for ( curr = ban_list; curr != NULL; prev = curr, curr = curr->next )
    {
        if ( !wcscasecmp( arg, curr->name ) )
        {
	    if (curr->level > get_trust(ch))
	    {
		send_to_char( L"You are not powerful enough to lift that ban.\n\r",ch);
		return;
	    }
            if ( prev == NULL )
                ban_list   = ban_list->next;
            else
                prev->next = curr->next;

            free_ban(curr);
	    swprintf( buf, MAX_STRING_LENGTH-1, L"Ban on %s lifted.\n\r",arg);
            send_to_char( buf, ch );
	    save_bans();
            return;
        }
    }

    send_to_char( L"Bu site yasaklı değil.\n\r", ch );
    return;
}
