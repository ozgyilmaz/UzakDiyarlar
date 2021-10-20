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
#include <ctype.h>
#include <stdio.h>
#include <wchar.h>
#include <wctype.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <malloc.h>
#include "merc.h"
#include "recycle.h"
#include "lookup.h"
#include "tables.h"

#if !defined(macintosh)
extern  int     _filbuf         args( (FILE *) );
#endif

int system(const wchar_t *command);


int rename(const wchar_t *oldfname, const wchar_t *newfname);
int wear_convert( int oldwear );

wchar_t *print_flags(int flag)
{
    int count, pos = 0;
    static wchar_t buf[52];


    for (count = 0; count < 32;  count++)
    {
        if (IS_SET(flag,1<<count))
        {
            if (count < 26)
                buf[pos] = 'A' + count;
            else
                buf[pos] = 'a' + (count - 26);
            pos++;
        }
    }

    if (pos == 0)
    {
        buf[pos] = '0';
        pos++;
    }

    buf[pos] = '\0';

    return buf;
}


/*
 * Array of containers read for proper re-nesting of objects.
 */
#define MAX_NEST	100
static	OBJ_DATA *	rgObjNest	[MAX_NEST];



/*
 * Local functions.
 */
void	fwrite_char	args( ( CHAR_DATA *ch,  FILE *fp ) );
void	fwrite_obj	args( ( CHAR_DATA *ch,  OBJ_DATA  *obj,
			    FILE *fp, int iNest ) );
void	fwrite_pet	args( ( CHAR_DATA *pet, FILE *fp) );
void	fread_char	args( ( CHAR_DATA *ch,  FILE *fp ) );
void    fread_pet	args( ( CHAR_DATA *ch,  FILE *fp ) );
void	fread_obj	args( ( CHAR_DATA *ch,  FILE *fp ) );



/*
 * Save a character and inventory.
 * Would be cool to save NPC's too for quest purposes,
 *   some of the infrastructure is provided.
 */

void save_char_obj( CHAR_DATA *ch )
{
    wchar_t strsave[MAX_INPUT_LENGTH];
    FILE *fp;

    if ( IS_NPC(ch) || (ch->level < 2 && !IS_SET(ch->act, PLR_REMORTED)) )
	return;

    if ( ch->desc != NULL && ch->desc->original != NULL )
	ch = ch->desc->original;

#if defined(unix)
    /* create god log */
    if (IS_IMMORTAL(ch) || ch->level >= LEVEL_IMMORTAL)
    {
	fclose(fpReserve);
	swprintf(strsave, MAX_INPUT_LENGTH-1, L"%s%s",GOD_DIR, capitalize(ch->name));
	if ((fp = fopen((char*)strsave,"w")) == NULL)
	{
	    bug( L"Save_char_obj: fopen",0);
	    perror((char*)strsave);
 	}

	fwprintf(fp, L"Lev %2d Trust %2d  %s%s\n",
	    ch->level, get_trust(ch), ch->name, ch->pcdata->title);
	fclose( fp );
	fpReserve = fopen( NULL_FILE, "r" );
    if ( fpReserve == NULL )
      bug( L"save_char_obj: Can't open null file.", 0 );

    }
#endif

    fclose( fpReserve );
    swprintf( strsave, MAX_INPUT_LENGTH-1, L"%s%s", PLAYER_DIR, capitalize( ch->name ) );
    if ( ( fp = fopen( TEMP_FILE, "w" ) ) == NULL )
    {
	bug( L"Save_char_obj: fopen", 0 );
	perror( (char*)strsave );
    }
    else
    {
	fwrite_char( ch, fp );
	if ( ch->carrying != NULL )
	    fwrite_obj( ch, ch->carrying, fp, 0 );
	/* save the pets */
	if (ch->pet != NULL && ch->pet->in_room == ch->in_room)
	    fwrite_pet(ch->pet,fp);
	fwprintf( fp, L"#END\n" );
    }
    fclose( fp );
    rename(TEMP_FILE,(char*)strsave);
    fpReserve = fopen( NULL_FILE, "r" );
    if ( fpReserve == NULL )
      bug( L"save_char_obj: Can't open null file.", 0 );

    return;
}



/*
 * Write the char.
 */
void fwrite_char( CHAR_DATA *ch, FILE *fp )
{
    AFFECT_DATA *paf;
    int sn, pos, l;

    fwprintf( fp, L"#%s\n", IS_NPC(ch) ? "MOB" : "PLAYER"	);

    fwprintf( fp, L"Name %s~\n",	ch->name		);
    fwprintf( fp, L"Id   %ld\n", ch->id			);
    fwprintf( fp, L"Birth  %ld\n", ch->pcdata->birth_time	);
    fwprintf( fp, L"LogO %ld\n",	current_time		);
    fwprintf( fp, L"Vers %d\n",   7			);
    fwprintf( fp, L"Etho %d\n",   ch->ethos		);
    fwprintf( fp, L"Home %d\n",   0		);
    fwprintf( fp, L"Cab  %d\n",   ch->cabal		);
    fwprintf( fp, L"Dead %d\n",   ch->pcdata->death	);
    fwprintf( fp, L"GhostCounter %d\n",   ch->pcdata->ghost_mode_counter	);
    fwprintf( fp, L"Ques %s\n", 	print_flags(ch->quest)	);

    if (ch->short_descr[0] != '\0')
      	fwprintf( fp, L"ShD  %s~\n",	ch->short_descr	);
    if( ch->long_descr[0] != '\0')
	fwprintf( fp, L"LnD  %s~\n",	ch->long_descr	);
    if (ch->description[0] != '\0')
    	fwprintf( fp, L"Desc %s~\n",	ch->description	);
    if (ch->prompt != NULL || !wcscasecmp(ch->prompt,L"<%hhp %mm %vmv> "))
        fwprintf( fp, L"Prom %s~\n",      ch->prompt  	);
    fwprintf( fp, L"Race %s~\n", race_table[ORG_RACE(ch)].name[0] );
    fwprintf( fp, L"Sex  %d\n",	ch->sex			);
    fwprintf( fp, L"Cla  %d\n",	ch->iclass		);
    fwprintf( fp, L"Levl %d\n",	ch->level		);
    if (ch->trust != 0)
	fwprintf( fp, L"Tru  %d\n",	ch->trust	);
    fwprintf( fp, L"Plyd %d\n",
	ch->pcdata->played + (int) (current_time - ch->logon)	);
    fwprintf( fp, L"Scro %d\n", 	ch->lines		);
    fwprintf( fp, L"Room %d\n",
        (  ch->in_room == get_room_index( ROOM_VNUM_LIMBO )
        && ch->was_in_room != NULL )
            ? ch->was_in_room->vnum
            : ch->in_room == NULL ? 3001 : ch->in_room->vnum );

    fwprintf( fp, L"HMV  %d %d %d\n", ch->hit, ch->mana, ch->move );

	fwprintf( fp, L"Silv %ld\n",ch->silver		);
  fwprintf( fp, L"Banks %ld\n", ch->pcdata->bank_s );
	fwprintf( fp, L"Dilek %s\n", 	print_flags(ch->pcdata->dilek)	);
    fwprintf( fp, L"Exp  %d\n",	ch->exp			);
    if (ch->act != 0)
	fwprintf( fp, L"Act  %s\n",   print_flags(ch->act));
/*
    if (ch->affected_by != 0)
	{
	 if (IS_NPC(ch))
	 fwprintf( fp, L"AfBy %s\n", print_flags(ch->affected_by) );
	 else
	 fwprintf( fp, L"AfBy %s\n",
		print_flags((ch->affected_by & (~AFF_CHARM))) );
	}
    if (ch->detection != 0)
	fwprintf( fp, L"Detect %s\n",   print_flags(ch->detection));
*/
    fwprintf( fp, L"Comm %s\n",       print_flags(ch->comm));
    if (ch->wiznet)
    	fwprintf( fp, L"Wizn %s\n",   print_flags(ch->wiznet));
    if (ch->invis_level)
	fwprintf( fp, L"Invi %d\n", 	ch->invis_level	);
    if (ch->incog_level)
	fwprintf(fp, L"Inco %d\n",ch->incog_level);
    fwprintf( fp, L"Pos  %d\n",
	ch->position == POS_FIGHTING ? POS_STANDING : ch->position );
    if (ch->practice != 0)
    	fwprintf( fp, L"Prac %d\n",	ch->practice	);
    if (ch->train != 0)
	fwprintf( fp, L"Trai %d\n",	ch->train	);
    fwprintf( fp, L"Alig  %d\n",	ch->alignment		);
/*
    if (ch->saving_throw != 0)
	fwprintf( fp, L"Save  %d\n",	ch->saving_throw);
    if (ch->hitroll != 0)
	fwprintf( fp, L"Hit   %d\n",	ch->hitroll	);
    if (ch->damroll != 0)
	fwprintf( fp, L"Dam   %d\n",	ch->damroll	);
    fwprintf( fp, L"ACs %d %d %d %d\n",
	ch->armor[0],ch->armor[1],ch->armor[2],ch->armor[3]);
*/
    if (ch->wimpy !=0 )
	fwprintf( fp, L"Wimp  %d\n",	ch->wimpy	);

    fwprintf( fp, L"Attr %d %d %d %d %d %d\n",
	ch->perm_stat[STAT_STR],
	ch->perm_stat[STAT_INT],
	ch->perm_stat[STAT_WIS],
	ch->perm_stat[STAT_DEX],
	ch->perm_stat[STAT_CON],
	ch->perm_stat[STAT_CHA] );

	fwprintf( fp, L"Pass %s~\n",	ch->pcdata->pwd		);
	if (ch->pcdata->bamfin[0] != '\0')
	    fwprintf( fp, L"Bin  %s~\n",	ch->pcdata->bamfin);
	if (ch->pcdata->bamfout[0] != '\0')
		fwprintf( fp, L"Bout %s~\n",	ch->pcdata->bamfout);
	fwprintf( fp, L"Titl %s~\n",	ch->pcdata->title	);
    	fwprintf( fp, L"Pnts %d\n",   	ch->pcdata->points      );
	fwprintf( fp, L"TSex %d\n",	ch->pcdata->true_sex	);
	fwprintf( fp, L"LLev %d\n",	ch->pcdata->last_level	);
	fwprintf( fp, L"HMVP %d %d %d\n", ch->pcdata->perm_hit,
						   ch->pcdata->perm_mana,
						   ch->pcdata->perm_move);
	fwprintf( fp, L"CndC  %d %d %d %d %d %d\n",
	    ch->pcdata->condition[0],
	    ch->pcdata->condition[1],
	    ch->pcdata->condition[2],
	    ch->pcdata->condition[3],
	    ch->pcdata->condition[4],
	    ch->pcdata->condition[5] );

	/* write alias */
        for (pos = 0; pos < MAX_ALIAS; pos++)
	{
	    if (ch->pcdata->alias[pos] == NULL
	    ||  ch->pcdata->alias_sub[pos] == NULL)
		break;

	    fwprintf(fp, L"Alias %s %s~\n",ch->pcdata->alias[pos],
		    ch->pcdata->alias_sub[pos]);
	}

	for ( sn = 0; sn < MAX_SKILL; sn++ )
	{
	    if ( skill_table[sn].name != NULL && ch->pcdata->learned[sn] > 0 )
	    {
		fwprintf( fp, L"Sk %d '%s'\n",
		    ch->pcdata->learned[sn], skill_table[sn].name[0] );
	    }
	}

  for ( sn = 0; sn < MAX_RACE; sn++ )
  {
    if(race_table[sn].name[0]==NULL)
      break;
    if(!wcscasecmp(race_table[sn].name[0],L"unique"))
      continue;
      if ( ch->pcdata->familya[sn] > 0 )
      {
    fwprintf( fp, L"Fm %d '%s'\n",
        ch->pcdata->familya[sn], race_table[sn].name[0] );
      }
  }

    for ( paf = ch->affected; paf != NULL; paf = paf->next )
    {
	if (paf->type < 0 || paf->type>= MAX_SKILL ||
            paf->type == gsn_doppelganger )
	    continue;

	if (!IS_NPC(ch)	&&
		(paf->bitvector == AFF_CHARM || paf->duration < -1) )
	    continue;

	fwprintf( fp, L"Affc '%s' %3d %3d %3d %3d %3d %10d\n",
	    skill_table[paf->type].name[0],
	    paf->where,
	    paf->level,
	    paf->duration,
	    paf->modifier,
	    paf->location,
	    paf->bitvector
	    );
    }

/* quest done by chronos */

    if (ch->pcdata->questpoints !=0)
        fwprintf( fp, L"QuestPnts %d\n", ch->pcdata->questpoints	);
    if (ch->pcdata->nextquest !=0)
        fwprintf( fp , L"QuestNext %d\n", ch->pcdata->nextquest	);
    if (IS_QUESTOR(ch))
	{
        fwprintf( fp , L"QuestCnt %d\n", ch->pcdata->countdown	);
        fwprintf( fp , L"QuestMob %d\n", ch->pcdata->questmob	);
        fwprintf( fp , L"QuestRoom %d\n", ch->pcdata->questroom	);
        fwprintf( fp , L"QuestGiv %d\n", ch->pcdata->questgiver	);
	}

    fwprintf( fp , L"Relig %d\n", 		ch->religion		);
    fwprintf( fp , L"Haskilled %d\n",	ch->pcdata->has_killed	);
    fwprintf( fp , L"Antkilled %d\n",	ch->pcdata->anti_killed	);
    if (ch->pcdata->rk_puani !=0)
        fwprintf( fp, L"RKPuani %ld\n", ch->pcdata->rk_puani	);
    if (ch->pcdata->din_puani !=0)
        fwprintf( fp, L"DinPuani %ld\n", ch->pcdata->din_puani	);
    if (ch->pcdata->yardim_puani !=0)
        fwprintf( fp, L"YardimPuani %ld\n", ch->pcdata->yardim_puani	);
    /* character log info */
    fwprintf( fp, L"PlayLog 1\n");	/* 1 stands for version */
    for( l = 0; l < MAX_TIME_LOG; l++)
    {
	if (ch->pcdata->log_date[l] != 0 && ch->pcdata->log_time[l] != 0)
	   fwprintf(fp, L"%d %d\n",
		ch->pcdata->log_date[l], ch->pcdata->log_time[l]);
    }
    if (ch->logon != current_time)
    {
	int add_day = parse_date(current_time);
	int add_time = parse_time_spec(current_time - ch->logon);
	fwprintf(fp, L"%d %d\n", add_day, add_time);
    }
    fwprintf(fp, L"-1\n");

    fwprintf( fp, L"End\n\n" );
    return;
}

/* write a pet */
void fwrite_pet( CHAR_DATA *pet, FILE *fp)
{
    AFFECT_DATA *paf;

    fwprintf(fp, L"#PET\n");

    fwprintf(fp, L"Vnum %d\n",pet->pIndexData->vnum);

    fwprintf(fp, L"Name %s~\n", pet->name);
    fwprintf(fp, L"LogO %ld\n", current_time);
    fwprintf(fp, L"Cab  %d\n", pet->cabal );
    if (pet->short_descr != pet->pIndexData->short_descr)
    	fwprintf(fp, L"ShD  %s~\n", pet->short_descr);
    if (pet->long_descr != pet->pIndexData->long_descr)
    	fwprintf(fp, L"LnD  %s~\n", pet->long_descr);
    if (pet->description != pet->pIndexData->description)
    	fwprintf(fp, L"Desc %s~\n", pet->description);
    if (RACE(pet) != pet->pIndexData->race) /* serdar ORG_RACE */
    	fwprintf(fp, L"Race %s~\n", race_table[ORG_RACE(pet)].name[0]);
    fwprintf(fp, L"Sex  %d\n", pet->sex);
    if (pet->level != pet->pIndexData->level)
    	fwprintf(fp, L"Levl %d\n", pet->level);
    fwprintf(fp, L"HMV  %d %d %d %d %d %d\n",
    	pet->hit, pet->max_hit, pet->mana, pet->max_mana, pet->move, pet->max_move);
    if (pet->silver > 0)
	fwprintf(fp, L"Silv %ld\n",pet->silver);
    if (pet->exp > 0)
    	fwprintf(fp, L"Exp  %d\n", pet->exp);
    if (pet->act != pet->pIndexData->act)
    	fwprintf(fp, L"Act  %s\n", print_flags(pet->act));
    if (pet->affected_by != pet->pIndexData->affected_by)
    	fwprintf(fp, L"AfBy %s\n", print_flags(pet->affected_by));
    if (pet->detection != pet->pIndexData->detection)
    	fwprintf(fp, L"Detect %s\n", print_flags(pet->detection));
    if (pet->comm != 0)
    	fwprintf(fp, L"Comm %s\n", print_flags(pet->comm));
    fwprintf(fp, L"Pos  %d\n", pet->position = POS_FIGHTING ? POS_STANDING : pet->position);
    if (pet->saving_throw != 0)
    	fwprintf(fp, L"Save %d\n", pet->saving_throw);
    if (pet->alignment != pet->pIndexData->alignment)
    	fwprintf(fp, L"Alig %d\n", pet->alignment);
    if (pet->hitroll != pet->pIndexData->hitroll)
    	fwprintf(fp, L"Hit  %d\n", pet->hitroll);
    if (pet->damroll != pet->pIndexData->damage[DICE_BONUS])
    	fwprintf(fp, L"Dam  %d\n", pet->damroll);
    fwprintf(fp, L"ACs  %d %d %d %d\n",
    	pet->armor[0],pet->armor[1],pet->armor[2],pet->armor[3]);
    fwprintf(fp, L"Attr %d %d %d %d %d %d\n",
    	pet->perm_stat[STAT_STR], pet->perm_stat[STAT_INT],
    	pet->perm_stat[STAT_WIS], pet->perm_stat[STAT_DEX],
    	pet->perm_stat[STAT_CON], pet->perm_stat[STAT_CHA]);
    fwprintf(fp, L"AMod %d %d %d %d %d %d\n",
    	pet->mod_stat[STAT_STR], pet->mod_stat[STAT_INT],
    	pet->mod_stat[STAT_WIS], pet->mod_stat[STAT_DEX],
    	pet->mod_stat[STAT_CON], pet->mod_stat[STAT_CHA]);

    for ( paf = pet->affected; paf != NULL; paf = paf->next )
    {
    	if (paf->type < 0 || paf->type >= MAX_SKILL ||
          paf->type == gsn_doppelganger)
    	    continue;

    	fwprintf(fp, L"Affc '%s' %3d %3d %3d %3d %3d %10d\n",
    	    skill_table[paf->type].name[0],
    	    paf->where, paf->level, paf->duration, paf->modifier,paf->location,
    	    paf->bitvector);
    }

    fwprintf(fp, L"End\n");
    return;
}

/*
 * Write an object and its contents.
 */
void fwrite_obj( CHAR_DATA *ch, OBJ_DATA *obj, FILE *fp, int iNest )
{
    EXTRA_DESCR_DATA *ed;
    AFFECT_DATA *paf;
    int i;

    /*
     * Slick recursion to write lists backwards,
     *   so loading them will load in forwards order.
     */
    if ( obj->next_content != NULL )
	fwrite_obj( ch, obj->next_content, fp, iNest );

    for (i=1;i < MAX_CABAL;i++)
      if (obj->pIndexData->vnum == cabal_table[i].obj_vnum)
        return;



    /*
     * Castrate storage characters.
     */
    if ( ((ch->level < 10) && (obj->pIndexData->limit != -1 ))
    || (obj->item_type == ITEM_KEY && obj->value[0] == 0)
    || (obj->item_type == ITEM_MAP && !obj->value[0])
    || ((ch->level < obj->level -3) && (obj->item_type != ITEM_CONTAINER))
    || ((ch->level > obj->level + 35) && (obj->pIndexData->limit > 1 ))  )
    {
	extract_obj( obj );
	return;
    }

    if ( gorev_ekipmani_mi( obj ) && wcsstr(obj->short_descr,ch->name) == NULL )
  	{
      act( L"$p yokoluyor!",ch,obj,NULL,TO_CHAR);
  	 extract_obj(obj);
  	 return;
  	}

    fwprintf( fp, L"#O\n" );
    fwprintf( fp, L"Vnum %d\n",   	obj->pIndexData->vnum		);
    fwprintf( fp, L"Cond %d\n",		obj->condition			);

    if (!obj->pIndexData->new_format)
	fwprintf( fp, L"Oldstyle\n");
    if (obj->enchanted)
	fwprintf( fp, L"Enchanted\n");
    fwprintf( fp, L"Nest %d\n",	iNest	  	     );

    /* these data are only used if they do not match the defaults */

    if ( obj->name != obj->pIndexData->name)
    	fwprintf( fp, L"Name %s~\n",	obj->name		     );
    if ( obj->short_descr != obj->pIndexData->short_descr)
        fwprintf( fp, L"ShD  %s~\n",	obj->short_descr	     );
    if ( obj->description != obj->pIndexData->description)
        fwprintf( fp, L"Desc %s~\n",	obj->description	     );
    if ( obj->extra_flags != obj->pIndexData->extra_flags)
        fwprintf( fp, L"ExtF %d\n",	obj->extra_flags	     );
    if ( obj->wear_flags != obj->pIndexData->wear_flags)
        fwprintf( fp, L"WeaF %d\n",	obj->wear_flags		     );
    if ( obj->item_type != obj->pIndexData->item_type)
        fwprintf( fp, L"Ityp %d\n",	obj->item_type		     );
    if ( obj->weight != obj->pIndexData->weight)
        fwprintf( fp, L"Wt   %d\n",	obj->weight		     );

    /* variable data */

    fwprintf( fp, L"WLoc %d\n",   obj->wear_loc                );
    if (obj->level != obj->pIndexData->level)
        fwprintf( fp, L"Lev  %d\n",	obj->level		     );
    if (obj->timer != 0)
        fwprintf( fp, L"Time %d\n",	obj->timer	     );
    fwprintf( fp, L"Cost %d\n",	obj->cost		     );
    if (obj->value[0] != obj->pIndexData->value[0]
    ||  obj->value[1] != obj->pIndexData->value[1]
    ||  obj->value[2] != obj->pIndexData->value[2]
    ||  obj->value[3] != obj->pIndexData->value[3]
    ||  obj->value[4] != obj->pIndexData->value[4])
    	fwprintf( fp, L"Val  %d %d %d %d %d\n",
	    obj->value[0], obj->value[1], obj->value[2], obj->value[3],
	    obj->value[4]	     );

    switch ( obj->item_type )
    {
    case ITEM_POTION:
    case ITEM_SCROLL:
	if ( obj->value[1] > 0 )
	{
	    fwprintf( fp, L"Spell 1 '%s'\n",
		skill_table[obj->value[1]].name[0] );
	}

	if ( obj->value[2] > 0 )
	{
	    fwprintf( fp, L"Spell 2 '%s'\n",
		skill_table[obj->value[2]].name[0] );
	}

	if ( obj->value[3] > 0 )
	{
	    fwprintf( fp, L"Spell 3 '%s'\n",
		skill_table[obj->value[3]].name[0] );
	}

	break;

    case ITEM_PILL:
    case ITEM_STAFF:
    case ITEM_WAND:
	if ( obj->value[3] > 0 )
	{
	    fwprintf( fp, L"Spell 3 '%s'\n",
		skill_table[obj->value[3]].name[0] );
	}

	break;
    }

    for ( paf = obj->affected; paf != NULL; paf = paf->next )
    {
	if (paf->type < 0 || paf->type >= MAX_SKILL)
	    continue;
        fwprintf( fp, L"Affc '%s' %3d %3d %3d %3d %3d %10d\n",
            skill_table[paf->type].name[0],
            paf->where,
            paf->level,
            paf->duration,
            paf->modifier,
            paf->location,
            paf->bitvector
            );
    }

    for ( ed = obj->extra_descr; ed != NULL; ed = ed->next )
    {
	fwprintf( fp, L"ExDe %s~ %s~\n",
	    ed->keyword, ed->description );
    }

    fwprintf( fp, L"End\n\n" );

    if ( obj->contains != NULL )
	fwrite_obj( ch, obj->contains, fp, iNest + 1 );

    return;
}



/*
 * Load a char and inventory into a new ch structure.
 */
bool load_char_obj( DESCRIPTOR_DATA *d, wchar_t *name )
{
    wchar_t strsave[MAX_INPUT_LENGTH];
    wchar_t buf[100];
    CHAR_DATA *ch;
    FILE *fp;
    bool found;
    int stat;

    ch = new_char();
    ch->pcdata = new_pcdata();

    d->character			= ch;
    ch->desc				= d;
    ch->name				= str_dup( name );
    ch->id				= get_pc_id();
    ch->race				= race_lookup(L"human");
    ch->pcdata->race			= ch->race;
    ch->cabal                           = 0;
    ch->hometown                        = 0;
    ch->ethos                           = 0;
    ch->affected_by                     = 0;
    ch->detection			= 0;
    ch->act				= (PLR_NOSUMMON | PLR_NOCANCEL);
    ch->comm				= COMM_COMBINE
					| COMM_PROMPT;

    ch->pcdata->perm_hit		= 20;
    ch->pcdata->perm_mana		= 100;
    ch->pcdata->perm_move		= 100;

    ch->invis_level                     = 0;
    ch->practice                        = 0;
    ch->train                           = 0;
    ch->hitroll                         = 0;
    ch->damroll                         = 0;
    ch->trust                           = 0;
    ch->wimpy                           = 0;
    ch->saving_throw                    = 0;
    ch->progtypes                       = 0;
    ch->extracted                       = FALSE;
    ch->pcdata->points                  = 0;
    ch->prompt 				= str_dup( L"<%n: %hhp %mm %vmv Opp:<%o>> ");
    ch->pcdata->confirm_delete		= FALSE;
    ch->pcdata->confirm_remort		= FALSE;
    ch->pcdata->pwd			= str_dup( L"" );
    ch->pcdata->bamfin			= str_dup( L"" );
    ch->pcdata->bamfout			= str_dup( L"" );
    ch->pcdata->title			= str_dup( L"" );

    ch->pcdata->time_flag		= 0;

    for (stat =0; stat < MAX_STATS; stat++)
	ch->perm_stat[stat]		= 13;

    ch->pcdata->condition[COND_THIRST]	= 48;
    ch->pcdata->condition[COND_FULL]	= 48;
    ch->pcdata->condition[COND_HUNGER]	= 48;
    ch->pcdata->condition[COND_BLOODLUST]	= 48;
    ch->pcdata->condition[COND_DESIRE]	= 48;

    for (stat =0; stat < MAX_RACE; stat++)
      ch->pcdata->familya[stat]	= 0;

    ch->pcdata->nextquest = 0;
    ch->pcdata->questpoints = 0;
    ch->pcdata->questgiver = 0;
    ch->pcdata->countdown = 0;
    ch->pcdata->questmob = 0;
    ch->pcdata->birth_time = current_time - 14688000;// 17 oyun yılını çıkarıyoruz ki doğum yılı doğru olsun
    ch->religion = RELIGION_NONE;
    ch->pcdata->has_killed = 0;
    ch->pcdata->anti_killed = 0;
    ch->timer	= 0;
    ch->hunting = NULL;
    ch->endur   = 0;
    ch->riding	= FALSE;
    ch->mount	= NULL;
    ch->in_mind	= NULL;
    ch->pcdata->rk_puani = 0;
    ch->pcdata->din_puani = 0;
    ch->pcdata->yardim_puani = 0;

    found = FALSE;
    fclose( fpReserve );

    #if defined(unix)
    /* decompress if .gz file exists */
    swprintf( strsave, MAX_INPUT_LENGTH-1, L"%s%s%s", PLAYER_DIR, capitalize(name),".gz");
    if ( ( fp = fopen( (char*)strsave, "r" ) ) != NULL )
    {
	fclose(fp);
	swprintf( buf, MAX_STRING_LENGTH-1, L"gzip -dfq %s",strsave);
	system((char*)buf);
    }
    #endif

    swprintf( strsave, MAX_INPUT_LENGTH-1, L"%s%s", PLAYER_DIR, capitalize( name ) );
    if ( ( fp = fopen( (char*)strsave, "r" ) ) != NULL )
    {
	int iNest;

	for ( iNest = 0; iNest < MAX_NEST; iNest++ )
	    rgObjNest[iNest] = NULL;

	found = TRUE;
	for ( ; ; )
	{
	    wchar_t letter;
	    wchar_t *word;

	    letter = fread_letter( fp );
	    if ( letter == '*' )
	    {
		fread_to_eol( fp );
		continue;
	    }

	    if ( letter != '#' )
	    {
		bug( L"Load_char_obj: # not found.", 0 );
		break;
	    }

	    word = fread_word( fp );
	    if      ( !wcscasecmp( word, L"PLAYER" ) ) fread_char ( ch, fp );
	    else if ( !wcscasecmp( word, L"OBJECT" ) ) fread_obj  ( ch, fp );
	    else if ( !wcscasecmp( word, L"O"      ) ) fread_obj  ( ch, fp );
	    else if ( !wcscasecmp( word, L"PET"    ) ) fread_pet  ( ch, fp );
	    else if ( !wcscasecmp( word, L"END"    ) ) break;
	    else
	    {
		bug( L"Load_char_obj: bad section.", 0 );
		break;
	    }
	}
	fclose( fp );
    }

    fpReserve = fopen( NULL_FILE, "r" );
    if ( fpReserve == NULL )
      bug( L"load_char: Can't open null file.", 0 );


    /* initialize race */
    if (found)
    {
	if (ORG_RACE(ch) == 0)
	    ORG_RACE(ch) = race_lookup(L"human");
	if (RACE(ch) == 0)
	    RACE(ch) = race_lookup(L"human");

	ch->size = race_table[ORG_RACE(ch)].size;
	ch->dam_type = 17; /*punch */

	ch->detection   = ch->detection|race_table[RACE(ch)].det;
	ch->affected_by = ch->affected_by|race_table[RACE(ch)].aff;
	ch->imm_flags	= ch->imm_flags | race_table[RACE(ch)].imm;
	ch->res_flags	= ch->res_flags | race_table[RACE(ch)].res;
	ch->vuln_flags	= ch->vuln_flags | race_table[RACE(ch)].vuln;
	ch->form	= race_table[RACE(ch)].form;
	ch->parts	= race_table[RACE(ch)].parts;

	if (ch->pcdata->condition[COND_BLOODLUST] <48
		&& ch->iclass != CLASS_VAMPIRE)
	    ch->pcdata->condition[COND_BLOODLUST] = 48;

	/*
	 * Add the bonus time now, because we don't allow old players
	 * to be loaded with limited equipments!
	 */
	if (IS_SET(ch->pcdata->time_flag, TLP_NOLOG))
	{
	    int l, today, day;

	    today = parse_date( current_time );
	    for (l =0; l < MAX_TIME_LOG; l++)
	    {
		day = ((365 + today - l) % 365);
		ch->pcdata->log_date[l]	= day ? day : 365;
		ch->pcdata->log_time[l]	= 60;
	    }
	    REMOVE_BIT(ch->pcdata->time_flag, TLP_NOLOG);
	}

    }

    return found;
}



/*
 * Read in a char.
 */

#if defined(KEY)
#undef KEY
#endif

#define KEY( literal, field, value )					\
  if ( !wcscasecmp( word, literal ) )	\
  {					\
  field  = value;			\
  fMatch = TRUE;			\
  break;				\
}

void fread_char( CHAR_DATA *ch, FILE *fp )
{
    wchar_t buf[MAX_STRING_LENGTH];
    const wchar_t *word=L"End";
    bool fMatch;
    bool fPlayLog = FALSE;
    int count = 0;
    long tmp_silver;


    swprintf( buf, MAX_STRING_LENGTH-1, L"Karakter yukleme: %s.",ch->name);
    log_string(buf);
    ch->pcdata->bank_s = 0;
    tmp_silver = 0;
    ch->pcdata->ghost_mode_counter = 0;

    for ( ; ; )
    {
	word   = (wchar_t *)(feof( fp ) ? L"End" : fread_word( fp ));
	fMatch = FALSE;

	switch ( towupper(word[0]) )
	{
	case '*':
	    fMatch = TRUE;
	    fread_to_eol( fp );
	    break;

	case 'A':
	    KEY( L"Act",		ch->act,		fread_flag( fp ) );
      if (!wcscasecmp( word, L"AffectedBy") || !wcscasecmp( word, L"AfBy"))
      {
        fread_flag( fp );
        fMatch = TRUE;
        break;
      }
	    KEY( L"Alignment",	ch->alignment,		fread_number( fp ) );
	    KEY( L"Alig",	ch->alignment,		fread_number( fp ) );
	    KEY( L"AntKilled",	ch->pcdata->anti_killed,fread_number( fp ) );

	    if (!wcscasecmp( word, L"Alia"))
	    {
		if (count >= MAX_ALIAS)
		{
		    fread_to_eol(fp);
		    fMatch = TRUE;
		    break;
		}

		ch->pcdata->alias[count] 	= str_dup(fread_word(fp));
		ch->pcdata->alias_sub[count]	= str_dup(fread_word(fp));
		count++;
		fMatch = TRUE;
		break;
	    }

            if (!wcscasecmp( word, L"Alias"))
            {
                if (count >= MAX_ALIAS)
                {
                    fread_to_eol(fp);
                    fMatch = TRUE;
                    break;
                }

                ch->pcdata->alias[count]        = str_dup(fread_word(fp));
                ch->pcdata->alias_sub[count]    = fread_string(fp);
                count++;
                fMatch = TRUE;
                break;
            }

	    if (!wcscasecmp( word, L"AC") || !wcscasecmp(word, L"Armor"))
	    {
		fread_to_eol(fp);
		fMatch = TRUE;
		break;
	    }

	    if (!wcscasecmp(word, L"ACs"))
	    {
		int i;

		for (i = 0; i < 4; i++)
		    ch->armor[i] = fread_number(fp);
		fMatch = TRUE;
		break;
	    }

	    if (!wcscasecmp(word, L"AffD"))
	    {
		AFFECT_DATA *paf;
		int sn;

		paf = new_affect();

		sn = skill_lookup(fread_word(fp));
		if (sn < 0)
		    bug( L"Fread_char: unknown skill.",0);
		else
		    paf->type = sn;

		paf->level	= fread_number( fp );
		paf->duration	= fread_number( fp );
		paf->modifier	= fread_number( fp );
		paf->location	= fread_number( fp );
		paf->bitvector	= fread_number( fp );
		paf->next	= ch->affected;
		ch->affected	= paf;
		fMatch = TRUE;
		break;
	    }

            if (!wcscasecmp(word, L"Affc"))
            {
                AFFECT_DATA *paf;
                int sn;

                paf = new_affect();

                sn = skill_lookup(fread_word(fp));
                if (sn < 0)
                    bug( L"Fread_char: unknown skill.",0);
                else
                    paf->type = sn;

                paf->where  = fread_number(fp);
                paf->level      = fread_number( fp );
                paf->duration   = fread_number( fp );
                paf->modifier   = fread_number( fp );
                paf->location   = fread_number( fp );
                paf->bitvector  = fread_number( fp );
                paf->next       = ch->affected;
                ch->affected    = paf;
                fMatch = TRUE;
                break;
            }

	    if ( !wcscasecmp( word, L"AttrMod"  ) || !wcscasecmp(word, L"AMod"))
	    {
		int stat;
		for (stat = 0; stat < MAX_STATS; stat ++)
		   fread_number(fp);
		fMatch = TRUE;
		break;
	    }

	    if ( !wcscasecmp( word, L"AttrPerm" ) || !wcscasecmp(word, L"Attr"))
	    {
		int stat;

		for (stat = 0; stat < MAX_STATS; stat++)
		    ch->perm_stat[stat] = fread_number(fp);
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'B':
	    KEY( L"Bamfin",	ch->pcdata->bamfin,	fread_string( fp ) );
      if ( !wcscasecmp( word, L"Bankg" ) )
      {
        tmp_silver = fread_number( fp );
        ch->pcdata->bank_s += tmp_silver;
        fMatch = TRUE;
        break;
      }
      if ( !wcscasecmp( word, L"Banks" ) )
      {
        tmp_silver = fread_number( fp );
        ch->pcdata->bank_s += tmp_silver;
        fMatch = TRUE;
        break;
      }
	    KEY( L"Bamfout",	ch->pcdata->bamfout,	fread_string( fp ) );
	    KEY( L"Bin",		ch->pcdata->bamfin,	fread_string( fp ) );
      KEY( L"Birth",	ch->pcdata->birth_time,	fread_number( fp ) );
	    KEY( L"Bout",	ch->pcdata->bamfout,	fread_string( fp ) );
	    break;

	case 'C':
	    KEY( L"Class",	ch->iclass,		fread_number( fp ) );
	    KEY( L"Cla",		ch->iclass,		fread_number( fp ) );
	    KEY( L"Cab",		ch->cabal,		fread_number( fp ) );

	    if ( !wcscasecmp( word, L"Condition" ) || !wcscasecmp(word, L"Cond"))
	    {
		ch->pcdata->condition[0] = fread_number( fp );
		ch->pcdata->condition[1] = fread_number( fp );
		ch->pcdata->condition[2] = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
            if (!wcscasecmp(word, L"CndC"))
            {
                ch->pcdata->condition[0] = fread_number( fp );
                ch->pcdata->condition[1] = fread_number( fp );
                ch->pcdata->condition[2] = fread_number( fp );
		ch->pcdata->condition[3] = fread_number( fp );
		ch->pcdata->condition[4] = fread_number( fp );
		ch->pcdata->condition[5] = fread_number( fp );
                fMatch = TRUE;
                break;
            }
            if (!wcscasecmp(word, L"Cnd"))
            {
                ch->pcdata->condition[0] = fread_number( fp );
                ch->pcdata->condition[1] = fread_number( fp );
                ch->pcdata->condition[2] = fread_number( fp );
		ch->pcdata->condition[3] = fread_number( fp );
                fMatch = TRUE;
                break;
            }
	    KEY( L"Comm",		ch->comm,		fread_flag( fp ) );

	    break;

	case 'D':
      if (!wcscasecmp(word, L"Damroll") || !wcscasecmp(word, L"Dam"))
      {
        fread_number( fp );
        fMatch = TRUE;
        break;
      }
	    KEY( L"Description",	ch->description,	fread_string( fp ) );
	    KEY( L"Desc",	ch->description,	fread_string( fp ) );
	    KEY( L"Dead",	ch->pcdata->death,	fread_number( fp ) );
      if (!wcscasecmp(word, L"Detect"))
      {
        fread_flag(fp);
        fMatch = TRUE;
        break;
      }
		KEY( L"Dilek",	ch->pcdata->dilek, 		fread_flag( fp)	  );
    KEY( L"DinPuani",   ch->pcdata->din_puani,fread_number( fp) );
	    break;

	case 'E':
	    if ( !wcscasecmp( word, L"End" ) )
	    {
   		ch->played = ch->pcdata->played;

		/* if this is an old player, give some played time */
		if (!fPlayLog)
		    SET_BIT(ch->pcdata->time_flag, TLP_NOLOG);
		return;
	    }
	    KEY( L"Exp",		ch->exp,		fread_number( fp ) );
	    KEY( L"Etho",	ch->ethos,		fread_number( fp ) );
	    break;

      case 'F':
    		if (!wcscasecmp(word, L"Fm"))
    	    	{
    		int sn;
    		int value;
    		wchar_t *temp;

    		value = fread_number( fp );
    		temp = fread_word( fp ) ;
    		sn = race_lookup(temp);
    		if ( sn < 0 )
    		{
    		    fwprintf( stderr, L"%s",temp);
    		    bug( L"Fread_char: unknown race[for familya]. ", 0 );
    		}
    		else
    		    ch->pcdata->familya[sn] = value;
    		fMatch = TRUE;
    	    	}
    	break;

	case 'G':
      KEY( L"GhostCounter",	ch->pcdata->ghost_mode_counter,		fread_number( fp ) );
      if ( !wcscasecmp( word, L"Gold" ) )
      {
        tmp_silver = fread_number( fp );
        ch->silver += tmp_silver;
        fMatch = TRUE;
        break;
      }
      if ( !wcscasecmp( word, L"Group" )  || !wcscasecmp(word, L"Gr"))
      {
          fread_word( fp ) ;
          fMatch = TRUE;
      }
	    break;

	case 'H':
      if (!wcscasecmp(word, L"Hitroll") || !wcscasecmp(word, L"Hit"))
      {
        fread_number(fp);
        fMatch = TRUE;
        break;
      }
	    KEY( L"Home",	ch->hometown,		fread_number( fp ) );
	    KEY( L"Haskilled",	ch->pcdata->has_killed, fread_number( fp ) );

	    if ( !wcscasecmp( word, L"HpManaMove" ) || !wcscasecmp(word, L"HMV"))
	    {
		ch->hit		= fread_number( fp );
		ch->mana	= fread_number( fp );
		ch->move	= fread_number( fp );
		fMatch = TRUE;
		break;
	    }

            if ( !wcscasecmp( word, L"HpManaMovePerm" ) || !wcscasecmp(word, L"HMVP"))
            {
                ch->pcdata->perm_hit	= fread_number( fp );
                ch->pcdata->perm_mana   = fread_number( fp );
                ch->pcdata->perm_move   = fread_number( fp );
                fMatch = TRUE;
                break;
            }

	    break;

	case 'I':
	    KEY( L"Id",		ch->id,			fread_number( fp ) );
	    KEY( L"InvisLevel",	ch->invis_level,	fread_number( fp ) );
	    KEY( L"Inco",	ch->incog_level,	fread_number( fp ) );
	    KEY( L"Invi",	ch->invis_level,	fread_number( fp ) );
	    break;

	case 'L':
	    KEY( L"LastLevel",	ch->pcdata->last_level, fread_number( fp ) );
	    KEY( L"LLev",	ch->pcdata->last_level, fread_number( fp ) );
	    KEY( L"Level",	ch->level,		fread_number( fp ) );
	    KEY( L"Lev",		ch->level,		fread_number( fp ) );
	    KEY( L"Levl",	ch->level,		fread_number( fp ) );
      if ( !wcscasecmp( word, L"LogO" ) )
      {
        fread_number( fp );
        fMatch = TRUE;
        break;
      }
	    KEY( L"LongDescr",	ch->long_descr,		fread_string( fp ) );
	    KEY( L"LnD",		ch->long_descr,		fread_string( fp ) );
	    break;

	case 'N':
	    KEY( L"Name",	ch->name,		fread_string( fp ) );
	    break;

	case 'P':
	    KEY( L"Password",	ch->pcdata->pwd,	fread_string( fp ) );
	    KEY( L"Pass",	ch->pcdata->pwd,	fread_string( fp ) );
	    KEY( L"Played",	ch->pcdata->played,	fread_number( fp ) );
	    KEY( L"Plyd",	ch->pcdata->played,	fread_number( fp ) );
	    KEY( L"Points",	ch->pcdata->points,	fread_number( fp ) );
	    KEY( L"Pnts",	ch->pcdata->points,	fread_number( fp ) );
	    KEY( L"Position",	ch->position,		fread_number( fp ) );
	    KEY( L"Pos",		ch->position,		fread_number( fp ) );
	    KEY( L"Practice",	ch->practice,		fread_number( fp ) );
	    KEY( L"Prac",	ch->practice,		fread_number( fp ) );
            KEY( L"Prompt",      ch->prompt,             fread_string( fp ) );
 	    KEY( L"Prom",	ch->prompt,		fread_string( fp ) );
	    if (!wcscasecmp(word, L"PlayLog"))
	    {
		int l, d, t, today;

		today = parse_date( boot_time );
		for (l =0; l < MAX_TIME_LOG; l++)
		{
		  d = ((365 + today - l) % 365);
		  ch->pcdata->log_date[l]	= d ? d : 365;
		  ch->pcdata->log_time[l]	= 0;
		}
		fread_number(fp);	/* read the version */
		while (1)
		{
		   if ( (d = fread_number(fp)) < 0 ) break;
		   t = fread_number(fp);
		   for( l=0; l<MAX_TIME_LOG; l++)
		   {
		     if ( d == ch->pcdata->log_date[l] )
		     {
			ch->pcdata->log_time[l]	+= t;
			break;
		     }
		   }
		}
		fPlayLog = TRUE;
		fMatch = TRUE;
		break;
	    }
	    break;
	    break;
	case 'Q':
	    KEY( L"QuestCnt",	ch->pcdata->countdown,	fread_number( fp) );
	    KEY( L"QuestMob",	ch->pcdata->questmob,	fread_number( fp) );
	    KEY( L"QuestGiv",	ch->pcdata->questgiver,	fread_number( fp) );
      KEY( L"QuestRoom",	ch->pcdata->questroom,	fread_number( fp) );
	    KEY( L"QuestPnts",   ch->pcdata->questpoints,fread_number( fp) );
            KEY( L"QuestNext",   ch->pcdata->nextquest,	fread_number( fp) );
       	    KEY( L"Ques",	ch->quest, 		fread_flag( fp)	  );
	    break;

	case 'R':
	    KEY( L"Relig",       ch->religion,		fread_number( fp ) );
      KEY( L"RKPuani",   ch->pcdata->rk_puani,fread_number( fp) );

/*	    KEY( L"Race",        ch->race,  race_lookup(fread_string( fp )) ); */
	    if ( !wcscasecmp( word, L"Race" ) )
	    {
		RACE(ch) = race_lookup(fread_string(fp));
		ORG_RACE(ch) = RACE(ch);
		fMatch = TRUE;
		break;
	    }
	    if ( !wcscasecmp( word, L"Room" ) )
	    {
		ch->in_room = get_room_index( fread_number( fp ) );
		if ( ch->in_room == NULL )
		    ch->in_room = get_room_index( ROOM_VNUM_LIMBO );
		fMatch = TRUE;
		break;
	    }

	    break;

	case 'S':
      if (!wcscasecmp(word, L"SavingThrow") || !wcscasecmp(word, L"Save"))
      {
        fread_number(fp);
        fMatch = TRUE;
        break;
      }
	    KEY( L"Scro",	ch->lines,		fread_number( fp ) );
	    KEY( L"Sex",		ch->sex,		fread_number( fp ) );
	    KEY( L"ShortDescr",	ch->short_descr,	fread_string( fp ) );
	    KEY( L"ShD",		ch->short_descr,	fread_string( fp ) );
      if ( !wcscasecmp( word, L"Silv" ) )
      {
        tmp_silver = fread_number( fp );
        ch->silver += tmp_silver;
        fMatch = TRUE;
        break;
      }


	    if ( !wcscasecmp( word, L"Skill" ) || !wcscasecmp(word, L"Sk"))
	    {
		int sn;
		int value;
		wchar_t *temp;

		value = fread_number( fp );
		temp = fread_word( fp ) ;
		sn = skill_lookup(temp);
		/* sn    = skill_lookup( fread_word( fp ) ); */
		if ( sn < 0 )
		{
		    fwprintf( stderr, L"%s",temp);
		    bug( L"Fread_char: unknown skill. ", 0 );
		}
		else
		    ch->pcdata->learned[sn] = value;
		fMatch = TRUE;
	    }

	    break;

	case 'T':
            KEY( L"TrueSex",     ch->pcdata->true_sex,  	fread_number( fp ) );
	    KEY( L"TSex",	ch->pcdata->true_sex,   fread_number( fp ) );
	    KEY( L"Trai",	ch->train,		fread_number( fp ) );
	    KEY( L"Trust",	ch->trust,		fread_number( fp ) );
	    KEY( L"Tru",		ch->trust,		fread_number( fp ) );

	    if ( !wcscasecmp( word, L"Title" )  || !wcscasecmp( word, L"Titl"))
	    {
		ch->pcdata->title = fread_string( fp );
    		if (ch->pcdata->title[0] != '.' && ch->pcdata->title[0] != ','
		&&  ch->pcdata->title[0] != '!' && ch->pcdata->title[0] != '?')
		{
		    swprintf( buf, MAX_STRING_LENGTH-1, L" %s", ch->pcdata->title );
		    free_string( ch->pcdata->title );
		    ch->pcdata->title = str_dup( buf );
		}
		fMatch = TRUE;
		break;
	    }

	    break;

	case 'V':
      if (!wcscasecmp(word, L"Version") || !wcscasecmp(word, L"Vers"))
      {
        fread_number(fp);
        fMatch = TRUE;
        break;
      }
	    if ( !wcscasecmp( word, L"Vnum" ) )
	    {
		ch->pIndexData = get_mob_index( fread_number( fp ) );
		fMatch = TRUE;
		break;
	    }
	    break;
  case 'Y':
	    KEY( L"YardimPuani",   ch->pcdata->yardim_puani,fread_number( fp) );
	    break;

	case 'W':
	    KEY( L"Wimpy",	ch->wimpy,		fread_number( fp ) );
	    KEY( L"Wimp",	ch->wimpy,		fread_number( fp ) );
	    KEY( L"Wizn",	ch->wiznet,		fread_flag( fp ) );
	    break;
	}

	if ( !fMatch )
	{
	    bug( L"Fread_char: no match.", 0 );
	    fread_to_eol( fp );
	}
    }
 return;
}

/* load a pet from the forgotten reaches */
void fread_pet( CHAR_DATA *ch, FILE *fp )
{
    wchar_t *word;
    CHAR_DATA *pet;
    bool fMatch;
    int lastlogoff = current_time;
    int percent;

    /* first entry had BETTER be the vnum or we barf */
    word = (wchar_t *)(feof(fp) ? L"END" : fread_word(fp));
    if (!wcscasecmp(word, L"Vnum"))
    {
    	int vnum;

    	vnum = fread_number(fp);
    	if (get_mob_index(vnum) == NULL)
	{
    	    bug( L"Fread_pet: bad vnum %d.",vnum);
	    pet = create_mobile(get_mob_index(MOB_VNUM_FIDO), NULL);
	}
    	else
    	    pet = create_mobile(get_mob_index(vnum), NULL);
    }
    else
    {
        bug( L"Fread_pet: no vnum in file.",0);
        pet = create_mobile(get_mob_index(MOB_VNUM_FIDO), NULL);
    }

    for ( ; ; )
    {
    	word 	= (wchar_t *)(feof(fp) ? L"END" : fread_word(fp));
    	fMatch = FALSE;

    	switch (towupper(word[0]))
    	{
    	case '*':
    	    fMatch = TRUE;
    	    fread_to_eol(fp);
    	    break;

    	case 'A':
    	    KEY( L"Act",		pet->act,		fread_flag(fp));
    	    KEY( L"AfBy",	pet->affected_by,	fread_flag(fp));
    	    KEY( L"Alig",	pet->alignment,		fread_number(fp));

    	    if (!wcscasecmp(word, L"ACs"))
    	    {
    	    	int i;

    	    	for (i = 0; i < 4; i++)
    	    	    pet->armor[i] = fread_number(fp);
    	    	fMatch = TRUE;
    	    	break;
    	    }

    	    if (!wcscasecmp(word, L"AffD"))
    	    {
    	    	AFFECT_DATA *paf;
    	    	int sn;

    	    	paf = new_affect();

    	    	sn = skill_lookup(fread_word(fp));
    	     	if (sn < 0)
    	     	    bug( L"Fread_char: unknown skill.",0);
    	     	else
    	     	   paf->type = sn;

    	     	paf->level	= fread_number(fp);
    	     	paf->duration	= fread_number(fp);
    	     	paf->modifier	= fread_number(fp);
    	     	paf->location	= fread_number(fp);
    	     	paf->bitvector	= fread_number(fp);
    	     	paf->next	= pet->affected;
    	     	pet->affected	= paf;
    	     	fMatch		= TRUE;
    	     	break;
    	    }

            if (!wcscasecmp(word, L"Affc"))
            {
                AFFECT_DATA *paf;
                int sn;

                paf = new_affect();

                sn = skill_lookup(fread_word(fp));
                if (sn < 0)
                    bug( L"Fread_char: unknown skill.",0);
                else
                   paf->type = sn;

		paf->where	= fread_number(fp);
                paf->level      = fread_number(fp);
                paf->duration   = fread_number(fp);
                paf->modifier   = fread_number(fp);
                paf->location   = fread_number(fp);
                paf->bitvector  = fread_number(fp);
                paf->next       = pet->affected;
                pet->affected   = paf;
                fMatch          = TRUE;
                break;
            }

    	    if (!wcscasecmp(word, L"AMod"))
    	    {
    	     	int stat;

    	     	for (stat = 0; stat < MAX_STATS; stat++)
    	     	    pet->mod_stat[stat] = fread_number(fp);
    	     	fMatch = TRUE;
    	     	break;
    	    }

    	    if (!wcscasecmp(word, L"Attr"))
    	    {
    	         int stat;

    	         for (stat = 0; stat < MAX_STATS; stat++)
    	             pet->perm_stat[stat] = fread_number(fp);
    	         fMatch = TRUE;
    	         break;
    	    }
    	    break;

    	 case 'C':
             KEY( L"Cab",	pet->cabal,             fread_number(fp) );
    	     KEY( L"Comm",	pet->comm,		fread_flag(fp));
    	     break;

    	 case 'D':
    	     KEY( L"Dam",	pet->damroll,		fread_number(fp));
    	     KEY( L"Desc",	pet->description,	fread_string(fp));
	     KEY( L"Detect",	pet->detection,		fread_flag(fp));
    	     break;

    	 case 'E':
    	     if (!wcscasecmp(word, L"End"))
	     {
		pet->leader = ch;
		pet->master = ch;
		ch->pet = pet;
    		/* adjust hp mana move up  -- here for speed's sake */
    		percent = (current_time - lastlogoff) * 25 / ( 2 * 60 * 60);

    		if (percent > 0 && !IS_AFFECTED(ch,AFF_POISON)
    		&&  !IS_AFFECTED(ch,AFF_PLAGUE))
    		{
		    percent = UMIN(percent,100);
    		    pet->hit	+= (pet->max_hit - pet->hit) * percent / 100;
        	    pet->mana   += (pet->max_mana - pet->mana) * percent / 100;
        	    pet->move   += (pet->max_move - pet->move)* percent / 100;
    		}
		pet->in_room = get_room_index(ROOM_VNUM_LIMBO);
    	     	return;
	     }
    	     KEY( L"Exp",	pet->exp,		fread_number(fp));
    	     break;

    	 case 'H':
    	     KEY( L"Hit",	pet->hitroll,		fread_number(fp));

    	     if (!wcscasecmp(word, L"HMV"))
    	     {
    	     	pet->hit	= fread_number(fp);
    	     	pet->max_hit	= fread_number(fp);
    	     	pet->mana	= fread_number(fp);
    	     	pet->max_mana	= fread_number(fp);
    	     	pet->move	= fread_number(fp);
    	     	pet->max_move	= fread_number(fp);
    	     	fMatch = TRUE;
    	     	break;
    	     }
    	     break;

     	case 'L':
    	     KEY( L"Levl",	pet->level,		fread_number(fp));
    	     KEY( L"LnD",	pet->long_descr,	fread_string(fp));
	     KEY( L"LogO",	lastlogoff,		fread_number(fp));
    	     break;

    	case 'N':
    	     KEY( L"Name",	pet->name,		fread_string(fp));
    	     break;

    	case 'P':
    	     KEY( L"Pos",	pet->position,		fread_number(fp));
    	     break;

	case 'R':
	    if ( !wcscasecmp( word, L"Race" ) )
	    {
		RACE(pet) = race_lookup(fread_string(fp));
		ORG_RACE(pet) = RACE(pet);
		fMatch = TRUE;
		break;
	    }
    	    break;

    	case 'S' :
    	    KEY( L"Save",	pet->saving_throw,	fread_number(fp));
    	    KEY( L"Sex",		pet->sex,		fread_number(fp));
    	    KEY( L"ShD",		pet->short_descr,	fread_string(fp));
            KEY( L"Silv",        pet->silver,            fread_number( fp ) );
    	    break;

    	if ( !fMatch )
    	{
    	    bug( L"Fread_pet: no match.",0);
    	    fread_to_eol(fp);
    	}

    	}
    }
}



void fread_obj( CHAR_DATA *ch, FILE *fp )
{
    OBJ_DATA *obj;
    wchar_t *word;
    int iNest;
    bool fMatch;
    bool fNest;
    bool fVnum;
    bool first;
    bool new_format;  /* to prevent errors */
    bool make_new;    /* update object */

    fVnum = FALSE;
    obj = NULL;
    first = TRUE;  /* used to counter fp offset */
    new_format = FALSE;
    make_new = FALSE;

    word   = (wchar_t *)(feof( fp ) ? L"End" : fread_word( fp ));
    if (!wcscasecmp(word, L"Vnum" ))
    {
        int vnum;
	first = FALSE;  /* fp will be in right place */

        vnum = fread_number( fp );
        if (  get_obj_index( vnum )  == NULL )
	{
            bug( L"Fread_obj: bad vnum %d.", vnum );
	}
        else
	{
	    obj = create_object_nocount(get_obj_index(vnum),-1);
	    new_format = TRUE;
	}

    }

    if (obj == NULL)  /* either not found or old style */
    {
    	obj = new_obj();
    	obj->name		= str_dup( L"" );
    	obj->short_descr	= str_dup( L"" );
    	obj->description	= str_dup( L"" );
    }

    fNest		= FALSE;
    fVnum		= TRUE;
    iNest		= 0;

    for ( ; ; )
    {
	if (first)
	    first = FALSE;
	else
	    word   = (wchar_t *)(feof( fp ) ? L"End" : fread_word( fp ));
	fMatch = FALSE;

	switch ( towupper(word[0]) )
	{
	case '*':
	    fMatch = TRUE;
	    fread_to_eol( fp );
	    break;

	case 'A':
	    if (!wcscasecmp(word, L"AffD"))
	    {
		AFFECT_DATA *paf;
		int sn;

		paf = new_affect();

		sn = skill_lookup(fread_word(fp));
		if (sn < 0)
		    bug( L"Fread_obj: unknown skill.",0);
		else
		    paf->type = sn;

		paf->level	= fread_number( fp );
		paf->duration	= fread_number( fp );
		paf->modifier	= fread_number( fp );
		paf->location	= fread_number( fp );
		paf->bitvector	= fread_number( fp );
		paf->next	= obj->affected;
		obj->affected	= paf;
		fMatch		= TRUE;
		break;
	    }
            if (!wcscasecmp(word, L"Affc"))
            {
                AFFECT_DATA *paf;
                int sn;

                paf = new_affect();

                sn = skill_lookup(fread_word(fp));
                if (sn < 0)
                    bug( L"Fread_obj: unknown skill.",0);
                else
                    paf->type = sn;

		paf->where	= fread_number( fp );
                paf->level      = fread_number( fp );
                paf->duration   = fread_number( fp );
                paf->modifier   = fread_number( fp );
                paf->location   = fread_number( fp );
                paf->bitvector  = fread_number( fp );
                paf->next       = obj->affected;
                obj->affected   = paf;
                fMatch          = TRUE;
                break;
            }
	    break;

	case 'C':
	    if ( !wcscasecmp( word, L"Cond"))
	    {
		obj->condition = fread_number(fp);
		if (obj->condition < 1) obj->condition = 100;
	 	fMatch 	= TRUE;
		break;
	    }
	    KEY( L"Cost",	obj->cost,		fread_number( fp ) );
	    break;

	case 'D':
	    KEY( L"Description",	obj->description,	fread_string( fp ) );
	    KEY( L"Desc",	obj->description,	fread_string( fp ) );
	    break;

	case 'E':

	    if ( !wcscasecmp( word, L"Enchanted"))
	    {
		obj->enchanted = TRUE;
	 	fMatch 	= TRUE;
		break;
	    }

	    KEY( L"ExtraFlags",	obj->extra_flags,	fread_number( fp ) );
	    KEY( L"ExtF",	obj->extra_flags,	fread_number( fp ) );

	    if ( !wcscasecmp( word, L"ExtraDescr" ) || !wcscasecmp(word, L"ExDe"))
	    {
		EXTRA_DESCR_DATA *ed;

		ed = new_extra_descr();

		ed->keyword		= fread_string( fp );
		ed->description		= fread_string( fp );
		ed->next		= obj->extra_descr;
		obj->extra_descr	= ed;
		fMatch = TRUE;
	    }

	    if ( !wcscasecmp( word, L"End" ) )
	    {
		if ( !fNest || !fVnum || obj->pIndexData == NULL)
		{
		    bug( L"Fread_obj: incomplete object.", 0 );
		    free_obj(obj);
		    return;
		}
		else if (obj->pIndexData->limit != -1
			&& get_total_played(ch) < MIN_TIME_LIMIT )
		{
		    swprintf( log_buf, (2*MAX_INPUT_LENGTH)-1, L"Ignoring limited %d for %s.",
			obj->pIndexData->vnum, ch->name );
		    log_string( log_buf );
		    extract_obj_nocount(obj);
		    rgObjNest[iNest] = NULL;
		    return;
		}
		{
		    if (!new_format)
		    {
		    	obj->next	= object_list;
		    	object_list	= obj;
		    	obj->pIndexData->count++;
		    }

		    if (!obj->pIndexData->new_format
		    && obj->item_type == ITEM_ARMOR
		    &&  obj->value[1] == 0)
		    {
			obj->value[1] = obj->value[0];
			obj->value[2] = obj->value[0];
		    }
		    if (make_new)
		    {
			int wear;

			wear = obj->wear_loc;
			extract_obj(obj);

			obj = create_object(obj->pIndexData,0);
			obj->wear_loc = wear;
		    }
		    if ( iNest == 0 || rgObjNest[iNest-1] == NULL )
			obj_to_char( obj, ch );
		    else
			obj_to_obj( obj, rgObjNest[iNest-1] );
		    return;
		}
	    }
	    break;

	case 'I':
	    KEY( L"ItemType",	obj->item_type,		fread_number( fp ) );
	    KEY( L"Ityp",	obj->item_type,		fread_number( fp ) );
	    break;

	case 'L':
	    KEY( L"Level",	obj->level,		fread_number( fp ) );
	    KEY( L"Lev",		obj->level,		fread_number( fp ) );
	    break;

	case 'N':
	    KEY( L"Name",	obj->name,		fread_string( fp ) );

	    if ( !wcscasecmp( word, L"Nest" ) )
	    {
		iNest = fread_number( fp );
		if ( iNest < 0 || iNest >= MAX_NEST )
		{
		    bug( L"Fread_obj: bad nest %d.", iNest );
		}
		else
		{
		    rgObjNest[iNest] = obj;
		    fNest = TRUE;
		}
		fMatch = TRUE;
	    }
	    break;

   	case 'O':
	    if ( !wcscasecmp( word, L"Oldstyle" ) )
	    {
		if (obj->pIndexData != NULL && obj->pIndexData->new_format)
		    make_new = TRUE;
		fMatch = TRUE;
	    }
	    break;


	case 'Q':
	    KEY( L"Quality",	obj->condition,		fread_number( fp ) );
	    break;

	case 'S':
	    KEY( L"ShortDescr",	obj->short_descr,	fread_string( fp ) );
	    KEY( L"ShD",		obj->short_descr,	fread_string( fp ) );

	    if ( !wcscasecmp( word, L"Spell" ) )
	    {
		int iValue;
		int sn;

		iValue = fread_number( fp );
		sn     = skill_lookup( fread_word( fp ) );
		if ( iValue < 0 || iValue > 3 )
		{
		    bug( L"Fread_obj: bad iValue %d.", iValue );
		}
		else if ( sn < 0 )
		{
		    bug( L"Fread_obj: unknown skill.", 0 );
		}
		else
		{
		    obj->value[iValue] = sn;
		}
		fMatch = TRUE;
		break;
	    }

	    break;

	case 'T':
	    KEY( L"Timer",	obj->timer,		fread_number( fp ) );
	    KEY( L"Time",	obj->timer,		fread_number( fp ) );
	    break;

	case 'V':
	    if ( !wcscasecmp( word, L"Values" ) || !wcscasecmp(word, L"Vals"))
	    {
		obj->value[0]	= fread_number( fp );
		obj->value[1]	= fread_number( fp );
		obj->value[2]	= fread_number( fp );
		obj->value[3]	= fread_number( fp );
		if (obj->item_type == ITEM_WEAPON && obj->value[0] == 0)
		   obj->value[0] = obj->pIndexData->value[0];
		fMatch		= TRUE;
		break;
	    }

	    if ( !wcscasecmp( word, L"Val" ) )
	    {
		obj->value[0] 	= fread_number( fp );
	 	obj->value[1]	= fread_number( fp );
	 	obj->value[2] 	= fread_number( fp );
		obj->value[3]	= fread_number( fp );
		obj->value[4]	= fread_number( fp );
		fMatch = TRUE;
		break;
	    }

	    if ( !wcscasecmp( word, L"Vnum" ) )
	    {
		int vnum;

		vnum = fread_number( fp );
		if ( ( obj->pIndexData = get_obj_index( vnum ) ) == NULL )
		    bug( L"Fread_obj: bad vnum %d.", vnum );
		else
		    fVnum = TRUE;
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'W':
	    KEY( L"WearFlags",	obj->wear_flags,	fread_number( fp ) );
	    KEY( L"WeaF",	obj->wear_flags,	fread_number( fp ) );
	    KEY( L"Weight",	obj->weight,		fread_number( fp ) );
	    KEY( L"WLoc",	obj->wear_loc,		fread_number( fp ) );
	    KEY( L"Wt",		obj->weight,		fread_number( fp ) );
	    if ( !wcscasecmp( word, L"Wear" ) )
	    {
		obj->wear_loc 	= wear_convert(fread_number( fp ));
		fMatch = TRUE;
		break;
	    }
	    if ( !wcscasecmp( word, L"Wearloc" ) )
	    {
		obj->wear_loc 	= wear_convert(fread_number( fp ));
		fMatch = TRUE;
		break;
	    }
	    break;

	}

	if ( !fMatch )
	{
	    bug( L"Fread_obj: no match.", 0 );
	    fread_to_eol( fp );
	}
    }
}


int wear_convert( int oldwear )
{
 int iWear;

 switch( oldwear )
     {
	case OWEAR_FINGER_L:
	case OWEAR_FINGER_R:	iWear = WEAR_FINGER;	break;
	case OWEAR_NECK_1:
	case OWEAR_NECK_2:	iWear = WEAR_NECK;	break;
	case OWEAR_BODY:	iWear = WEAR_BODY;	break;
	case OWEAR_HEAD:	iWear = WEAR_HEAD;	break;
	case OWEAR_LEGS:	iWear = WEAR_LEGS;	break;
	case OWEAR_FEET:	iWear = WEAR_FEET;	break;
	case OWEAR_HANDS:	iWear = WEAR_HANDS;	break;
	case OWEAR_ARMS:	iWear = WEAR_ARMS;	break;
	case OWEAR_SHIELD:	iWear = WEAR_LEFT;	break;
	case OWEAR_ABOUT:	iWear = WEAR_ABOUT;	break;
	case OWEAR_WAIST:	iWear = WEAR_WAIST;	break;
	case OWEAR_WRIST_L:
	case OWEAR_WRIST_R:	iWear = WEAR_WRIST;	break;
	case OWEAR_WIELD:	iWear = WEAR_RIGHT;	break;
	case OWEAR_FLOAT:	iWear = WEAR_FLOAT;	break;
	case OWEAR_TATTOO:	iWear = WEAR_TATTOO;	break;
	case OWEAR_STUCK_IN:	iWear = WEAR_STUCK_IN;	break;
	case OWEAR_LIGHT:
	case OWEAR_HOLD:
	case OWEAR_SECOND_WIELD:
	default:		iWear = WEAR_NONE;	break;
      }
 return iWear;
}
