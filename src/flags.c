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
#include <string.h>
#include <stdlib.h>
#include "merc.h"
#include "tables.h"

int flag_lookup args( ( const wchar_t *name, const struct flag_type *flag_table) );

void do_flag(CHAR_DATA *ch, wchar_t *argument)
{
    wchar_t arg1[MAX_INPUT_LENGTH],arg2[MAX_INPUT_LENGTH],arg3[MAX_INPUT_LENGTH];
    wchar_t word[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    long *flag, old = 0, inew = 0, marked = 0, pos;
    wchar_t type;
    const struct flag_type *flag_table;

    argument = one_argument(argument,arg1);
    argument = one_argument(argument,arg2);
    argument = one_argument(argument,arg3);

    type = argument[0];

    if (type == '=' || type == '-' || type == '+')
        argument = one_argument(argument,word);

    if (arg1[0] == '\0')
    {
	send_to_char( L"Syntax:\n\r",ch);
	send_to_char( L"  flag mob  <name> <field> <flags>\n\r",ch);
	send_to_char( L"  flag char <name> <field> <flags>\n\r",ch);
	send_to_char( L"  flag obj  <name> <field> <flags>\n\r",ch);
	send_to_char( L"  flag room <room> <field> <flags>\n\r",ch);
	send_to_char( L"  mob  flags: act,aff,off,imm,res,vuln,form,part\n\r",ch);
	send_to_char( L"  char flags: plr,comm,aff,imm,res,vuln,\n\r",ch);
	send_to_char( L"  obj  flags: extra,wear,weap,cont,gate,exit\n\r",ch);
	send_to_char( L"  room flags: room\n\r",ch);
	send_to_char( L"  +: add flag, -: remove flag, = set equal to\n\r",ch);
	send_to_char( L"  otherwise flag toggles the flags listed.\n\r",ch);
	return;
    }

    if (arg2[0] == '\0')
    {
	send_to_char( L"What do you wish to set flags on?\n\r",ch);
	return;
    }

    if (arg3[0] == '\0')
    {
	send_to_char( L"You need to specify a flag to set.\n\r",ch);
	return;
    }

    if (argument[0] == '\0')
    {
	send_to_char( L"Which flags do you wish to change?\n\r",ch);
	return;
    }

    if (!str_prefix(arg1, L"mob") || !str_prefix(arg1, L"char"))
    {
	victim = get_char_world(ch,arg2);
	if (victim == NULL)
	{
	    send_to_char( L"You can't find them.\n\r",ch);
	    return;
	}

        /* select a flag to set */
	if (!str_prefix(arg3, L"act"))
	{
	    if (!IS_NPC(victim))
	    {
		send_to_char( L"Use plr for PCs.\n\r",ch);
		return;
	    }

	    flag = &victim->act;
	    flag_table = act_flags;
	}

	else if (!str_prefix(arg3, L"plr"))
	{
	    if (IS_NPC(victim))
	    {
		send_to_char( L"Use act for NPCs.\n\r",ch);
		return;
	    }

	    flag = &victim->act;
	    flag_table = plr_flags;
	}

 	else if (!str_prefix(arg3, L"aff"))
	{
	    flag = &victim->affected_by;
	    flag_table = affect_flags;
	}

  	else if (!str_prefix(arg3, L"immunity"))
	{
	    flag = &victim->imm_flags;
	    flag_table = imm_flags;
	}

	else if (!str_prefix(arg3, L"resist"))
	{
	    flag = &victim->res_flags;
	    flag_table = imm_flags;
	}

	else if (!str_prefix(arg3, L"vuln"))
	{
	    flag = &victim->vuln_flags;
	    flag_table = imm_flags;
	}

	else if (!str_prefix(arg3, L"form"))
	{
	    if (!IS_NPC(victim))
	    {
	 	send_to_char( L"Form can't be set on PCs.\n\r",ch);
		return;
	    }

	    flag = &victim->form;
	    flag_table = form_flags;
	}

	else if (!str_prefix(arg3, L"parts"))
	{
	    if (!IS_NPC(victim))
	    {
		send_to_char( L"Parts can't be set on PCs.\n\r",ch);
		return;
	    }

	    flag = &victim->parts;
	    flag_table = part_flags;
	}

	else if (!str_prefix(arg3, L"comm"))
	{
	    if (IS_NPC(victim))
	    {
		send_to_char( L"Comm can't be set on NPCs.\n\r",ch);
		return;
	    }

	    flag = &victim->comm;
	    flag_table = comm_flags;
	}

	else
	{
	    send_to_char( L"That's not an acceptable flag.\n\r",ch);
	    return;
	}

	old = *flag;
	victim->zone = NULL;

	if (type != '=')
	    inew = old;

        /* mark the words */
        for (; ;)
        {
	    argument = one_argument(argument,word);

	    if (word[0] == '\0')
		break;

	    pos = flag_lookup(word,flag_table);
	    if (pos == 0)
	    {
		send_to_char( L"That flag doesn't exist!\n\r",ch);
		return;
	    }
	    else
		SET_BIT(marked,pos);
	}

	for (pos = 0; flag_table[pos].name != NULL; pos++)
	{
	    if (!flag_table[pos].settable && IS_SET(old,flag_table[pos].bit))
	    {
		SET_BIT(inew,flag_table[pos].bit);
		continue;
	    }

	    if (IS_SET(marked,flag_table[pos].bit))
	    {
		switch(type)
		{
		    case '=':
		    case '+':
			SET_BIT(inew,flag_table[pos].bit);
			break;
		    case '-':
			REMOVE_BIT(inew,flag_table[pos].bit);
			break;
		    default:
			if (IS_SET(inew,flag_table[pos].bit))
			    REMOVE_BIT(inew,flag_table[pos].bit);
			else
			    SET_BIT(inew,flag_table[pos].bit);
		}
	    }
	}
	*flag = inew;
	return;
    }
}
