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

#include <stdio.h>
#include <wchar.h>
#include <wctype.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#endif

#include "merc.h"
#include "db.h"
#include "lookup.h"
#include "tables.h"


/* values for db2.c */
struct		social_type	social_table		[MAX_SOCIALS];
int		social_count;

/* snarf a socials file */
void load_socials( FILE *fp)
{
    for ( ; ; )
    {
    	struct social_type social;
    	wchar_t *temp;
        /* clear social */
	social.char_no_arg = NULL;
	social.others_no_arg = NULL;
	social.char_found = NULL;
	social.others_found = NULL;
	social.vict_found = NULL;
	social.char_not_found = NULL;
	social.char_auto = NULL;
	social.others_auto = NULL;

    	temp = fread_word(fp);
    	if (!wcscmp(temp,L"#0"))
	    return;  /* done */
#if defined(social_debug)
	else
	    fwprintf( stderr, L"%s\n\r",temp);
#endif

    	wcscpy(social.name,temp);
    	fread_to_eol(fp);

	temp = fread_string_eol(fp);
	if (!wcscmp(temp, L"$"))
	     social.char_no_arg = NULL;
	else if (!wcscmp(temp, L"#"))
	{
	     social_table[social_count] = social;
	     social_count++;
	     continue;
	}
        else
	    social.char_no_arg = temp;

        temp = fread_string_eol(fp);
        if (!wcscmp(temp, L"$"))
             social.others_no_arg = NULL;
        else if (!wcscmp(temp, L"#"))
        {
	     social_table[social_count] = social;
             social_count++;
             continue;
        }
        else
	    social.others_no_arg = temp;

        temp = fread_string_eol(fp);
        if (!wcscmp(temp, L"$"))
             social.char_found = NULL;
        else if (!wcscmp(temp, L"#"))
        {
	     social_table[social_count] = social;
             social_count++;
             continue;
        }
       	else
	    social.char_found = temp;

        temp = fread_string_eol(fp);
        if (!wcscmp(temp, L"$"))
             social.others_found = NULL;
        else if (!wcscmp(temp, L"#"))
        {
	     social_table[social_count] = social;
             social_count++;
             continue;
        }
        else
	    social.others_found = temp;

        temp = fread_string_eol(fp);
        if (!wcscmp(temp, L"$"))
             social.vict_found = NULL;
        else if (!wcscmp(temp, L"#"))
        {
	     social_table[social_count] = social;
             social_count++;
             continue;
        }
        else
	    social.vict_found = temp;

        temp = fread_string_eol(fp);
        if (!wcscmp(temp, L"$"))
             social.char_not_found = NULL;
        else if (!wcscmp(temp, L"#"))
        {
	     social_table[social_count] = social;
             social_count++;
             continue;
        }
        else
	    social.char_not_found = temp;

        temp = fread_string_eol(fp);
        if (!wcscmp(temp, L"$"))
             social.char_auto = NULL;
        else if (!wcscmp(temp, L"#"))
        {
	     social_table[social_count] = social;
             social_count++;
             continue;
        }
        else
	    social.char_auto = temp;

        temp = fread_string_eol(fp);
        if (!wcscmp(temp, L"$"))
             social.others_auto = NULL;
        else if (!wcscmp(temp, L"#"))
        {
             social_table[social_count] = social;
             social_count++;
             continue;
        }
        else
	    social.others_auto = temp;

	social_table[social_count] = social;
    	social_count++;
   }
   return;
}



void load_new_mobiles( FILE *fp )
{
    MOB_INDEX_DATA *pMobIndex;

    for ( ; ; )
    {
        sh_int vnum;
        wchar_t letter;
        int iHash;

        letter                          = fread_letter( fp );
        if ( letter != '#' )
        {
            bug( L"Load_new_mobiles: # not found.", 0 );
            exit( 1 );
        }

        vnum                            = fread_number( fp );
        if ( vnum == 0 )
            break;

        fBootDb = FALSE;
        if ( get_mob_index( vnum ) != NULL )
        {
            bug( L"Load_new_mobiles: vnum %d duplicated.", vnum );
            exit( 1 );
        }
        fBootDb = TRUE;

        pMobIndex                       = (MOB_INDEX_DATA *)alloc_perm( sizeof(*pMobIndex) );
        pMobIndex->vnum                 = vnum;
		newmobs++;
        pMobIndex->player_name          = fread_string( fp );
        pMobIndex->short_descr          = fread_string( fp );
        pMobIndex->long_descr           = fread_string( fp );
        pMobIndex->description          = fread_string( fp );
		pMobIndex->race		 			= race_lookup(fread_string( fp ));

        pMobIndex->act                  = fread_flag( fp ) | ACT_IS_NPC | race_table[pMobIndex->race].act;
        pMobIndex->affected_by          = fread_flag( fp ) | race_table[pMobIndex->race].aff;
		pMobIndex->level                = fread_number( fp );

    if (pMobIndex->level <= 0)
      bug( L"Load_new_mobiles: Level %d found.", pMobIndex->level );

		pMobIndex->long_descr[0]        = towupper(pMobIndex->long_descr[0]);
        pMobIndex->description[0]       = towupper(pMobIndex->description[0]);

		pMobIndex->practicer			= 0;
		pMobIndex->detection			= race_table[pMobIndex->race].det;

/* chronos smashed affection of ROM and created detection of ANATOLIA */
	if (IS_AFFECTED(pMobIndex,C))	/* detect evil */
		 SET_BIT(pMobIndex->detection,C);
	if (IS_AFFECTED(pMobIndex,D))	/* detect invis */
		 SET_BIT(pMobIndex->detection,D);
	if (IS_AFFECTED(pMobIndex,E))	/* detect magic */
		 SET_BIT(pMobIndex->detection,E);
	if (IS_AFFECTED(pMobIndex,F))	/* detect hidden */
		 SET_BIT(pMobIndex->detection,F);
	if (IS_AFFECTED(pMobIndex,G))	/* detect good */
		 SET_BIT(pMobIndex->detection,G);
	if (IS_AFFECTED(pMobIndex,Z))	/* dark vision */
		 SET_BIT(pMobIndex->detection,Z);
	if (IS_AFFECTED(pMobIndex,ee))	/* acute vision */
		 SET_BIT(pMobIndex->detection,ee);
	REMOVE_BIT(pMobIndex->affected_by,(C|D|E|F|G|Z|ee));

        pMobIndex->pShop                = NULL;
        pMobIndex->alignment            = 0;

        pMobIndex->hitroll              = 0;

	/* read hit dice */
        pMobIndex->hit[DICE_NUMBER]     = 1;
        /* 'd'          */
        pMobIndex->hit[DICE_TYPE]   	= 1;
        /* '+'          */
        pMobIndex->hit[DICE_BONUS]      = 1;

 	/* read mana dice */
	pMobIndex->mana[DICE_NUMBER]	= 1;
	pMobIndex->mana[DICE_TYPE]		= 1;
	pMobIndex->mana[DICE_BONUS]		= 1;

	/* read damage dice */
	pMobIndex->damage[DICE_NUMBER]	= 1;
	pMobIndex->damage[DICE_TYPE]	= 1;
	pMobIndex->damage[DICE_BONUS]	= 1;
	pMobIndex->dam_type				= 1;

	/* read armor class */
	pMobIndex->ac[AC_PIERCE]	= 1;
	pMobIndex->ac[AC_BASH]		= 1;
	pMobIndex->ac[AC_SLASH]		= 1;
	pMobIndex->ac[AC_EXOTIC]	= 1;

	/* read flags and add in data from the race table */
	pMobIndex->off_flags		= 0;
	pMobIndex->imm_flags		= 0;
	pMobIndex->res_flags		= 0;
	pMobIndex->vuln_flags		= 0;

	/* vital statistics */
	pMobIndex->start_pos		= 0;
	pMobIndex->default_pos		= 0;
	pMobIndex->sex				= 0;

	pMobIndex->form				= 0;
	pMobIndex->parts			= 0;
	/* size */
	pMobIndex->size				= 0;
	pMobIndex->material			= str_dup( L"none");
	pMobIndex->mprogs			= NULL;
	pMobIndex->progtypes		= 0;

        iHash                   = vnum % MAX_KEY_HASH;
        pMobIndex->next         = mob_index_hash[iHash];
        mob_index_hash[iHash]   = pMobIndex;
        top_mob_index++;
        kill_table[URANGE(0, pMobIndex->level, MAX_LEVEL-1)].number++;
    }

    return;
}

/*
 * Snarf an obj section. new style
 */
void load_objects( FILE *fp )
{
    OBJ_INDEX_DATA *pObjIndex;

    for ( ; ; )
    {
        sh_int vnum;
        wchar_t letter;
        int iHash;

        letter                          = fread_letter( fp );
        if ( letter != '#' )
        {
            bug( L"Load_objects: # not found.", 0 );
            exit( 1 );
        }

        vnum                            = fread_number( fp );
        if ( vnum == 0 )
            break;

        fBootDb = FALSE;
        if ( get_obj_index( vnum ) != NULL )
        {
            bug( L"Load_objects: vnum %d duplicated.", vnum );
            exit( 1 );
        }
        fBootDb = TRUE;

        pObjIndex                       = (OBJ_INDEX_DATA *)alloc_perm( sizeof(*pObjIndex) );
        pObjIndex->vnum                 = vnum;
        pObjIndex->new_format           = TRUE;
	pObjIndex->reset_num		= 0;
	newobjs++;
        pObjIndex->name                 = fread_string( fp );
        pObjIndex->short_descr          = fread_string( fp );
        pObjIndex->description          = fread_string( fp );
        pObjIndex->material		= fread_string( fp );

        pObjIndex->item_type            = item_lookup(fread_word( fp ));
        pObjIndex->extra_flags          = fread_flag( fp );
        pObjIndex->wear_flags           = fread_flag( fp );
	switch(pObjIndex->item_type)
	{
	case ITEM_WEAPON:
	    pObjIndex->value[0]		= weapon_type(fread_word(fp));
	    pObjIndex->value[1]		= fread_number(fp);
	    pObjIndex->value[2]		= fread_number(fp);
	    pObjIndex->value[3]		= attack_lookup(fread_word(fp));
	    pObjIndex->value[4]		= fread_flag(fp);
	    break;
	case ITEM_CONTAINER:
	    pObjIndex->value[0]		= fread_number(fp);
	    pObjIndex->value[1]		= fread_flag(fp);
	    pObjIndex->value[2]		= fread_number(fp);
	    pObjIndex->value[3]		= fread_number(fp);
	    pObjIndex->value[4]		= fread_number(fp);
	    break;
        case ITEM_DRINK_CON:
	case ITEM_FOUNTAIN:
            pObjIndex->value[0]         = fread_number(fp);
            pObjIndex->value[1]         = fread_number(fp);
            pObjIndex->value[2]         = liq_lookup(fread_word(fp));
            pObjIndex->value[3]         = fread_number(fp);
            pObjIndex->value[4]         = fread_number(fp);
            break;
	case ITEM_WAND:
	case ITEM_STAFF:
	    pObjIndex->value[0]		= fread_number(fp);
	    pObjIndex->value[1]		= fread_number(fp);
	    pObjIndex->value[2]		= fread_number(fp);
	    pObjIndex->value[3]		= skill_lookup(fread_word(fp));
	    pObjIndex->value[4]		= fread_number(fp);
	    break;
	case ITEM_POTION:
	case ITEM_PILL:
	case ITEM_SCROLL:
 	    pObjIndex->value[0]		= fread_number(fp);
	    pObjIndex->value[1]		= skill_lookup(fread_word(fp));
	    pObjIndex->value[2]		= skill_lookup(fread_word(fp));
	    pObjIndex->value[3]		= skill_lookup(fread_word(fp));
	    pObjIndex->value[4]		= skill_lookup(fread_word(fp));
	    break;
	default:
            pObjIndex->value[0]             = fread_flag( fp );
            pObjIndex->value[1]             = fread_flag( fp );
            pObjIndex->value[2]             = fread_flag( fp );
            pObjIndex->value[3]             = fread_flag( fp );
	    pObjIndex->value[4]		    = fread_flag( fp );
	    break;
	}
	pObjIndex->level		= fread_number( fp );
        pObjIndex->weight               = fread_number( fp );
        pObjIndex->cost                 = fread_number( fp );
        pObjIndex->progtypes            = 0;
        pObjIndex->oprogs               = NULL;
        pObjIndex->limit                = -1;

        /* condition */
        letter 				= fread_letter( fp );
	switch (letter)
 	{
	    case ('P') :		pObjIndex->condition = 100; break;
	    case ('G') :		pObjIndex->condition =  90; break;
	    case ('A') :		pObjIndex->condition =  75; break;
	    case ('W') :		pObjIndex->condition =  50; break;
	    case ('D') :		pObjIndex->condition =  25; break;
	    case ('B') :		pObjIndex->condition =  10; break;
	    case ('R') :		pObjIndex->condition =   0; break;
	    default:			pObjIndex->condition = 100; break;
	}

        for ( ; ; )
        {
            wchar_t letter;

            letter = fread_letter( fp );

            if ( letter == 'A' )
            {
                AFFECT_DATA *paf;

                paf                     = (AFFECT_DATA *)alloc_perm( sizeof(*paf) );
		paf->where		= TO_OBJECT;
                paf->type               = -1;
                paf->level              = pObjIndex->level;
                paf->duration           = -1;
                paf->location           = fread_number( fp );
                paf->modifier           = fread_number( fp );
                paf->bitvector          = 0;
                paf->next               = pObjIndex->affected;
                pObjIndex->affected     = paf;
                top_affect++;
            }

	    else if (letter == 'F')
            {
                AFFECT_DATA *paf;

                paf                     = (AFFECT_DATA *)alloc_perm( sizeof(*paf) );
		letter 			= fread_letter(fp);
		switch (letter)
	 	{
		case 'A':
                    paf->where          = TO_AFFECTS;
		    break;
		case 'I':
		    paf->where		= TO_IMMUNE;
		    break;
		case 'R':
		    paf->where		= TO_RESIST;
		    break;
		case 'V':
		    paf->where		= TO_VULN;
		    break;
		case 'D':
		    paf->where		= TO_DETECTS;
		    break;
		default:
            	    bug( L"Load_objects: Bad where on flag set.", 0 );
            	   exit( 1 );
		}
                paf->type               = -1;
                paf->level              = pObjIndex->level;
                paf->duration           = -1;
                paf->location           = fread_number(fp);
                paf->modifier           = fread_number(fp);
                paf->bitvector          = fread_flag(fp);
                paf->next               = pObjIndex->affected;
                pObjIndex->affected     = paf;
                top_affect++;
            }

            else if ( letter == 'E' )
            {
                EXTRA_DESCR_DATA *ed;

                ed                      = (EXTRA_DESCR_DATA *)alloc_perm( sizeof(*ed) );
                ed->keyword             = fread_string( fp );
                ed->description         = fread_string( fp );
                ed->next                = pObjIndex->extra_descr;
                pObjIndex->extra_descr  = ed;
                top_ed++;
            }

            else
            {
                ungetwc( letter, fp );
                break;
            }
        }

        iHash                   = vnum % MAX_KEY_HASH;
        pObjIndex->next         = obj_index_hash[iHash];
        obj_index_hash[iHash]   = pObjIndex;
        top_obj_index++;
    }

    return;
}

/*
 * Snarf a mprog section
 */

void load_omprogs( FILE *fp )
{
  wchar_t progtype[MAX_INPUT_LENGTH];
  wchar_t progname[MAX_INPUT_LENGTH];

    for ( ; ; )
    {
	MOB_INDEX_DATA *pMobIndex;
	OBJ_INDEX_DATA *pObjIndex;
	wchar_t letter;


	switch ( letter = fread_letter( fp ) )
	{
	default:
	    bug( L"Load_omprogs: letter '%c' not *IMS.", letter );
	    exit( 1 );

	case 'S':
	    return;

	case '*':
	    break;

        case 'O':
	    pObjIndex = get_obj_index ( fread_number ( fp ) );
	    if (pObjIndex->oprogs == NULL)
	      pObjIndex->oprogs = (OPROG_DATA*)alloc_perm(sizeof(OPROG_DATA));

	    wcscpy(progtype, fread_word(fp));
	    wcscpy(progname, fread_word(fp));
	    oprog_set( pObjIndex, progtype, progname);
	    break;

	case 'M':
	    pMobIndex		= get_mob_index	( fread_number ( fp ) );
	    if (pMobIndex->mprogs == NULL)
	      pMobIndex->mprogs = (MPROG_DATA*)alloc_perm(sizeof(MPROG_DATA));

	    wcscpy(progtype,fread_word(fp));
	    wcscpy(progname,fread_word(fp));
	    mprog_set( pMobIndex,progtype,progname);
	    break;
	}

	fread_to_eol( fp );
    }
}
