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

int system(const char *command);


int rename(const char *oldfname, const char *newfname);
int wear_convert( int oldwear );

char *print_flags(int flag)
{
    int count, pos = 0;
    static char buf[52];


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
    char strsave[MAX_INPUT_LENGTH];
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
	sprintf(strsave, "%s%s",GOD_DIR, capitalize(ch->name));
	if ((fp = fopen(strsave,"w")) == NULL)
	{
	    bug("Save_char_obj: fopen",0);
	    perror(strsave);
 	}

	fprintf(fp,"Lev %2d Trust %2d  %s%s\n",
	    ch->level, get_trust(ch), ch->name, ch->pcdata->title);
	fclose( fp );
	fpReserve = fopen( NULL_FILE, "r" );
    if ( fpReserve == NULL )
      bug("save_char_obj: Can't open null file.", 0 );

    }
#endif

    fclose( fpReserve );
    sprintf( strsave, "%s%s", PLAYER_DIR, capitalize( ch->name ) );
    if ( ( fp = fopen( TEMP_FILE, "w" ) ) == NULL )
    {
	bug( "Save_char_obj: fopen", 0 );
	perror( strsave );
    }
    else
    {
	fwrite_char( ch, fp );
	if ( ch->carrying != NULL )
	    fwrite_obj( ch, ch->carrying, fp, 0 );
	/* save the pets */
	if (ch->pet != NULL && ch->pet->in_room == ch->in_room)
	    fwrite_pet(ch->pet,fp);
	fprintf( fp, "#END\n" );
    }
    fclose( fp );
    rename(TEMP_FILE,strsave);
    fpReserve = fopen( NULL_FILE, "r" );
    if ( fpReserve == NULL )
      bug("save_char_obj: Can't open null file.", 0 );

    return;
}



/*
 * Write the char.
 */
void fwrite_char( CHAR_DATA *ch, FILE *fp )
{
    AFFECT_DATA *paf;
    int sn, pos, l;

    fprintf( fp, "#%s\n", IS_NPC(ch) ? "MOB" : "PLAYER"	);

    fprintf( fp, "Name %s~\n",	ch->name		);
    fprintf( fp, "Id   %ld\n", ch->id			);
    fprintf( fp, "LogO %ld\n",	current_time		);
    fprintf( fp, "Vers %d\n",   7			);
    fprintf( fp, "Etho %d\n",   ch->ethos		);
    fprintf( fp, "Home %d\n",   ch->hometown		);
    fprintf( fp, "Cab  %d\n",   ch->cabal		);
    fprintf( fp, "Dead %d\n",   ch->pcdata->death	);
    fprintf( fp, "Ques %s\n", 	print_flags(ch->quest)	);

    if (ch->short_descr[0] != '\0')
      	fprintf( fp, "ShD  %s~\n",	ch->short_descr	);
    if( ch->long_descr[0] != '\0')
	fprintf( fp, "LnD  %s~\n",	ch->long_descr	);
    if (ch->description[0] != '\0')
    	fprintf( fp, "Desc %s~\n",	ch->description	);
    if (ch->prompt != NULL || !str_cmp(ch->prompt,"<%hhp %mm %vmv> "))
        fprintf( fp, "Prom %s~\n",      ch->prompt  	);
    fprintf( fp, "Race %s~\n", pc_race_table[ORG_RACE(ch)].name[0] );
    fprintf( fp, "Sex  %d\n",	ch->sex			);
    fprintf( fp, "Cla  %d\n",	ch->iclass		);
    fprintf( fp, "Levl %d\n",	ch->level		);
    if (ch->trust != 0)
	fprintf( fp, "Tru  %d\n",	ch->trust	);
    fprintf( fp, "Plyd %d\n",
	ch->pcdata->played + (int) (current_time - ch->logon)	);
    fprintf( fp, "Not  %ld %ld %ld %ld %ld\n",
	ch->pcdata->last_note,ch->pcdata->last_idea,ch->pcdata->last_penalty,
	ch->pcdata->last_news,ch->pcdata->last_changes	);
    fprintf( fp, "Scro %d\n", 	ch->lines		);
    fprintf( fp, "Room %d\n",
        (  ch->in_room == get_room_index( ROOM_VNUM_LIMBO )
        && ch->was_in_room != NULL )
            ? ch->was_in_room->vnum
            : ch->in_room == NULL ? 3001 : ch->in_room->vnum );

    fprintf( fp, "HMV  %d %d %d\n", ch->hit, ch->mana, ch->move );

    if (ch->gold > 0)
      fprintf( fp, "Gold %ld\n",	ch->gold		);
    else
      fprintf( fp, "Gold %d\n", 0			);
    if (ch->silver > 0)
	fprintf( fp, "Silv %ld\n",ch->silver		);
    else
	fprintf( fp, "Silv %d\n",0			);
    if (ch->pcdata->bank_s > 0)
    	fprintf( fp, "Banks %ld\n", ch->pcdata->bank_s );
    else
    	fprintf( fp, "Banks %ld\n", ch->pcdata->bank_s );
    if (ch->pcdata->bank_g > 0)
    	fprintf( fp, "Bankg %ld\n", ch->pcdata->bank_g );
    else
    	fprintf( fp, "Bankg %ld\n", ch->pcdata->bank_g );
    fprintf( fp, "Exp  %d\n",	ch->exp			);
    if (ch->act != 0)
	fprintf( fp, "Act  %s\n",   print_flags(ch->act));
/*
    if (ch->affected_by != 0)
	{
	 if (IS_NPC(ch))
	 fprintf( fp, "AfBy %s\n", print_flags(ch->affected_by) );
	 else
	 fprintf( fp, "AfBy %s\n",
		print_flags((ch->affected_by & (~AFF_CHARM))) );
	}
    if (ch->detection != 0)
	fprintf( fp, "Detect %s\n",   print_flags(ch->detection));
*/
    fprintf( fp, "Comm %s\n",       print_flags(ch->comm));
    if (ch->wiznet)
    	fprintf( fp, "Wizn %s\n",   print_flags(ch->wiznet));
    if (ch->invis_level)
	fprintf( fp, "Invi %d\n", 	ch->invis_level	);
    if (ch->incog_level)
	fprintf(fp,"Inco %d\n",ch->incog_level);
    fprintf( fp, "Pos  %d\n",
	ch->position == POS_FIGHTING ? POS_STANDING : ch->position );
    if (ch->practice != 0)
    	fprintf( fp, "Prac %d\n",	ch->practice	);
    if (ch->train != 0)
	fprintf( fp, "Trai %d\n",	ch->train	);
    fprintf( fp, "Alig  %d\n",	ch->alignment		);
/*
    if (ch->saving_throw != 0)
	fprintf( fp, "Save  %d\n",	ch->saving_throw);
    if (ch->hitroll != 0)
	fprintf( fp, "Hit   %d\n",	ch->hitroll	);
    if (ch->damroll != 0)
	fprintf( fp, "Dam   %d\n",	ch->damroll	);
    fprintf( fp, "ACs %d %d %d %d\n",
	ch->armor[0],ch->armor[1],ch->armor[2],ch->armor[3]);
*/
    if (ch->wimpy !=0 )
	fprintf( fp, "Wimp  %d\n",	ch->wimpy	);

    fprintf( fp, "Attr %d %d %d %d %d %d\n",
	ch->perm_stat[STAT_STR],
	ch->perm_stat[STAT_INT],
	ch->perm_stat[STAT_WIS],
	ch->perm_stat[STAT_DEX],
	ch->perm_stat[STAT_CON],
	ch->perm_stat[STAT_CHA] );

/*
    fprintf (fp, "AMod %d %d %d %d %d %d\n",
	ch->mod_stat[STAT_STR],
	ch->mod_stat[STAT_INT],
	ch->mod_stat[STAT_WIS],
	ch->mod_stat[STAT_DEX],
	ch->mod_stat[STAT_CON],
	ch->mod_stat[STAT_CHA] );
*/

	fprintf( fp, "Pass %s~\n",	ch->pcdata->pwd		);
	if (ch->pcdata->bamfin[0] != '\0')
	    fprintf( fp, "Bin  %s~\n",	ch->pcdata->bamfin);
	if (ch->pcdata->bamfout[0] != '\0')
		fprintf( fp, "Bout %s~\n",	ch->pcdata->bamfout);
	fprintf( fp, "Titl %s~\n",	ch->pcdata->title	);
    	fprintf( fp, "Pnts %d\n",   	ch->pcdata->points      );
	fprintf( fp, "TSex %d\n",	ch->pcdata->true_sex	);
	fprintf( fp, "LLev %d\n",	ch->pcdata->last_level	);
	fprintf( fp, "HMVP %d %d %d\n", ch->pcdata->perm_hit,
						   ch->pcdata->perm_mana,
						   ch->pcdata->perm_move);
	fprintf( fp, "CndC  %d %d %d %d %d %d\n",
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

	    fprintf(fp,"Alias %s %s~\n",ch->pcdata->alias[pos],
		    ch->pcdata->alias_sub[pos]);
	}

	for ( sn = 0; sn < MAX_SKILL; sn++ )
	{
	    if ( skill_table[sn].name != NULL && ch->pcdata->learned[sn] > 0 )
	    {
		fprintf( fp, "Sk %d '%s'\n",
		    ch->pcdata->learned[sn], skill_table[sn].name[0] );
	    }
	}

  for ( sn = 0; sn < MAX_RACE; sn++ )
  {
    if(race_table[sn].name[0]==NULL)
      break;
    if(!str_cmp(race_table[sn].name[0],"unique"))
      continue;
      if ( ch->pcdata->familya[sn] > 0 )
      {
    fprintf( fp, "Fm %d '%s'\n",
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

	fprintf( fp, "Affc '%s' %3d %3d %3d %3d %3d %10d\n",
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
        fprintf( fp, "QuestPnts %d\n", ch->pcdata->questpoints	);
    if (ch->pcdata->nextquest !=0)
        fprintf( fp ,"QuestNext %d\n", ch->pcdata->nextquest	);
    if (IS_QUESTOR(ch))
	{
        fprintf( fp ,"QuestCnt %d\n", ch->pcdata->countdown	);
        fprintf( fp ,"QuestMob %d\n", ch->pcdata->questmob	);
        fprintf( fp ,"QuestObj %d\n", ch->pcdata->questobj	);
        fprintf( fp ,"QuestGiv %d\n", ch->pcdata->questgiver	);
	}

    fprintf( fp ,"Relig %d\n", 		ch->religion		);
    fprintf( fp ,"Haskilled %d\n",	ch->pcdata->has_killed	);
    fprintf( fp ,"Antkilled %d\n",	ch->pcdata->anti_killed	);

    /* character log info */
    fprintf( fp, "PlayLog 1\n");	/* 1 stands for version */
    for( l = 0; l < MAX_TIME_LOG; l++)
    {
	if (ch->pcdata->log_date[l] != 0 && ch->pcdata->log_time[l] != 0)
	   fprintf(fp, "%d %d\n",
		ch->pcdata->log_date[l], ch->pcdata->log_time[l]);
    }
    if (ch->logon != current_time)
    {
	int add_day = parse_date(current_time);
	int add_time = parse_time_spec(current_time - ch->logon);
	fprintf(fp, "%d %d\n", add_day, add_time);
    }
    fprintf(fp, "-1\n");

    fprintf( fp, "End\n\n" );
    return;
}

/* write a pet */
void fwrite_pet( CHAR_DATA *pet, FILE *fp)
{
    AFFECT_DATA *paf;

    fprintf(fp,"#PET\n");

    fprintf(fp,"Vnum %d\n",pet->pIndexData->vnum);

    fprintf(fp,"Name %s~\n", pet->name);
    fprintf(fp,"LogO %ld\n", current_time);
    fprintf(fp,"Cab  %d\n", pet->cabal );
    if (pet->short_descr != pet->pIndexData->short_descr)
    	fprintf(fp,"ShD  %s~\n", pet->short_descr);
    if (pet->long_descr != pet->pIndexData->long_descr)
    	fprintf(fp,"LnD  %s~\n", pet->long_descr);
    if (pet->description != pet->pIndexData->description)
    	fprintf(fp,"Desc %s~\n", pet->description);
    if (RACE(pet) != pet->pIndexData->race) /* serdar ORG_RACE */
    	fprintf(fp,"Race %s~\n", race_table[ORG_RACE(pet)].name[0]);
    fprintf(fp,"Sex  %d\n", pet->sex);
    if (pet->level != pet->pIndexData->level)
    	fprintf(fp,"Levl %d\n", pet->level);
    fprintf(fp, "HMV  %d %d %d %d %d %d\n",
    	pet->hit, pet->max_hit, pet->mana, pet->max_mana, pet->move, pet->max_move);
    if (pet->gold > 0)
    	fprintf(fp,"Gold %ld\n",pet->gold);
    if (pet->silver > 0)
	fprintf(fp,"Silv %ld\n",pet->silver);
    if (pet->exp > 0)
    	fprintf(fp, "Exp  %d\n", pet->exp);
    if (pet->act != pet->pIndexData->act)
    	fprintf(fp, "Act  %s\n", print_flags(pet->act));
    if (pet->affected_by != pet->pIndexData->affected_by)
    	fprintf(fp, "AfBy %s\n", print_flags(pet->affected_by));
    if (pet->detection != pet->pIndexData->detection)
    	fprintf(fp, "Detect %s\n", print_flags(pet->detection));
    if (pet->comm != 0)
    	fprintf(fp, "Comm %s\n", print_flags(pet->comm));
    fprintf(fp,"Pos  %d\n", pet->position = POS_FIGHTING ? POS_STANDING : pet->position);
    if (pet->saving_throw != 0)
    	fprintf(fp, "Save %d\n", pet->saving_throw);
    if (pet->alignment != pet->pIndexData->alignment)
    	fprintf(fp, "Alig %d\n", pet->alignment);
    if (pet->hitroll != pet->pIndexData->hitroll)
    	fprintf(fp, "Hit  %d\n", pet->hitroll);
    if (pet->damroll != pet->pIndexData->damage[DICE_BONUS])
    	fprintf(fp, "Dam  %d\n", pet->damroll);
    fprintf(fp, "ACs  %d %d %d %d\n",
    	pet->armor[0],pet->armor[1],pet->armor[2],pet->armor[3]);
    fprintf(fp, "Attr %d %d %d %d %d %d\n",
    	pet->perm_stat[STAT_STR], pet->perm_stat[STAT_INT],
    	pet->perm_stat[STAT_WIS], pet->perm_stat[STAT_DEX],
    	pet->perm_stat[STAT_CON], pet->perm_stat[STAT_CHA]);
    fprintf(fp, "AMod %d %d %d %d %d %d\n",
    	pet->mod_stat[STAT_STR], pet->mod_stat[STAT_INT],
    	pet->mod_stat[STAT_WIS], pet->mod_stat[STAT_DEX],
    	pet->mod_stat[STAT_CON], pet->mod_stat[STAT_CHA]);

    for ( paf = pet->affected; paf != NULL; paf = paf->next )
    {
    	if (paf->type < 0 || paf->type >= MAX_SKILL ||
          paf->type == gsn_doppelganger)
    	    continue;

    	fprintf(fp, "Affc '%s' %3d %3d %3d %3d %3d %10d\n",
    	    skill_table[paf->type].name[0],
    	    paf->where, paf->level, paf->duration, paf->modifier,paf->location,
    	    paf->bitvector);
    }

    fprintf(fp,"End\n");
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

    if ( obj->pIndexData->vnum == QUEST_ITEM1
	|| obj->pIndexData->vnum == QUEST_ITEM2
	|| obj->pIndexData->vnum == QUEST_ITEM3
	|| obj->pIndexData->vnum == OBJ_VNUM_EYED_SWORD)
      if (strstr(obj->short_descr,ch->name) == NULL)
	{
    act("$p yokoluyor!",ch,obj,NULL,TO_CHAR);
	 extract_obj(obj);
	 return;
	}

    fprintf( fp, "#O\n" );
    fprintf( fp, "Vnum %d\n",   	obj->pIndexData->vnum		);
    fprintf( fp, "Cond %d\n",		obj->condition			);

    if (!obj->pIndexData->new_format)
	fprintf( fp, "Oldstyle\n");
    if (obj->enchanted)
	fprintf( fp,"Enchanted\n");
    fprintf( fp, "Nest %d\n",	iNest	  	     );

    /* these data are only used if they do not match the defaults */

    if ( obj->name != obj->pIndexData->name)
    	fprintf( fp, "Name %s~\n",	obj->name		     );
    if ( obj->short_descr != obj->pIndexData->short_descr)
        fprintf( fp, "ShD  %s~\n",	obj->short_descr	     );
    if ( obj->description != obj->pIndexData->description)
        fprintf( fp, "Desc %s~\n",	obj->description	     );
    if ( obj->extra_flags != obj->pIndexData->extra_flags)
        fprintf( fp, "ExtF %d\n",	obj->extra_flags	     );
    if ( obj->wear_flags != obj->pIndexData->wear_flags)
        fprintf( fp, "WeaF %d\n",	obj->wear_flags		     );
    if ( obj->item_type != obj->pIndexData->item_type)
        fprintf( fp, "Ityp %d\n",	obj->item_type		     );
    if ( obj->weight != obj->pIndexData->weight)
        fprintf( fp, "Wt   %d\n",	obj->weight		     );

    /* variable data */

    fprintf( fp, "WLoc %d\n",   obj->wear_loc                );
    if (obj->level != obj->pIndexData->level)
        fprintf( fp, "Lev  %d\n",	obj->level		     );
    if (obj->timer != 0)
        fprintf( fp, "Time %d\n",	obj->timer	     );
    fprintf( fp, "Cost %d\n",	obj->cost		     );
    if (obj->value[0] != obj->pIndexData->value[0]
    ||  obj->value[1] != obj->pIndexData->value[1]
    ||  obj->value[2] != obj->pIndexData->value[2]
    ||  obj->value[3] != obj->pIndexData->value[3]
    ||  obj->value[4] != obj->pIndexData->value[4])
    	fprintf( fp, "Val  %d %d %d %d %d\n",
	    obj->value[0], obj->value[1], obj->value[2], obj->value[3],
	    obj->value[4]	     );

    switch ( obj->item_type )
    {
    case ITEM_POTION:
    case ITEM_SCROLL:
	if ( obj->value[1] > 0 )
	{
	    fprintf( fp, "Spell 1 '%s'\n",
		skill_table[obj->value[1]].name[0] );
	}

	if ( obj->value[2] > 0 )
	{
	    fprintf( fp, "Spell 2 '%s'\n",
		skill_table[obj->value[2]].name[0] );
	}

	if ( obj->value[3] > 0 )
	{
	    fprintf( fp, "Spell 3 '%s'\n",
		skill_table[obj->value[3]].name[0] );
	}

	break;

    case ITEM_PILL:
    case ITEM_STAFF:
    case ITEM_WAND:
	if ( obj->value[3] > 0 )
	{
	    fprintf( fp, "Spell 3 '%s'\n",
		skill_table[obj->value[3]].name[0] );
	}

	break;
    }

    for ( paf = obj->affected; paf != NULL; paf = paf->next )
    {
	if (paf->type < 0 || paf->type >= MAX_SKILL)
	    continue;
        fprintf( fp, "Affc '%s' %3d %3d %3d %3d %3d %10d\n",
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
	fprintf( fp, "ExDe %s~ %s~\n",
	    ed->keyword, ed->description );
    }

    fprintf( fp, "End\n\n" );

    if ( obj->contains != NULL )
	fwrite_obj( ch, obj->contains, fp, iNest + 1 );

    return;
}



/*
 * Load a char and inventory into a new ch structure.
 */
bool load_char_obj( DESCRIPTOR_DATA *d, char *name )
{
    char strsave[MAX_INPUT_LENGTH];
    char buf[100];
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
    ch->race				= race_lookup("human");
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
    ch->prompt 				= str_dup("<%n: %hhp %mm %vmv Opp:<%o>> ");
    ch->pcdata->confirm_delete		= FALSE;
    ch->pcdata->confirm_remort		= FALSE;
    ch->pcdata->pwd			= str_dup( "" );
    ch->pcdata->bamfin			= str_dup( "" );
    ch->pcdata->bamfout			= str_dup( "" );
    ch->pcdata->title			= str_dup( "" );

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
    ch->pcdata->questobj = 0;
    ch->pcdata->questmob = 0;
    ch->religion = RELIGION_NONE;
    ch->pcdata->has_killed = 0;
    ch->pcdata->anti_killed = 0;
    ch->timer	= 0;
    ch->hunting = NULL;
    ch->endur   = 0;
    ch->riding	= FALSE;
    ch->mount	= NULL;
    ch->in_mind	= NULL;

    found = FALSE;
    fclose( fpReserve );

    #if defined(unix)
    /* decompress if .gz file exists */
    sprintf( strsave, "%s%s%s", PLAYER_DIR, capitalize(name),".gz");
    if ( ( fp = fopen( strsave, "r" ) ) != NULL )
    {
	fclose(fp);
	sprintf(buf,"gzip -dfq %s",strsave);
	system(buf);
    }
    #endif

    sprintf( strsave, "%s%s", PLAYER_DIR, capitalize( name ) );
    if ( ( fp = fopen( strsave, "r" ) ) != NULL )
    {
	int iNest;

	for ( iNest = 0; iNest < MAX_NEST; iNest++ )
	    rgObjNest[iNest] = NULL;

	found = TRUE;
	for ( ; ; )
	{
	    char letter;
	    char *word;

	    letter = fread_letter( fp );
	    if ( letter == '*' )
	    {
		fread_to_eol( fp );
		continue;
	    }

	    if ( letter != '#' )
	    {
		bug( "Load_char_obj: # not found.", 0 );
		break;
	    }

	    word = fread_word( fp );
	    if      ( !str_cmp( word, "PLAYER" ) ) fread_char ( ch, fp );
	    else if ( !str_cmp( word, "OBJECT" ) ) fread_obj  ( ch, fp );
	    else if ( !str_cmp( word, "O"      ) ) fread_obj  ( ch, fp );
	    else if ( !str_cmp( word, "PET"    ) ) fread_pet  ( ch, fp );
	    else if ( !str_cmp( word, "END"    ) ) break;
	    else
	    {
		bug( "Load_char_obj: bad section.", 0 );
		break;
	    }
	}
	fclose( fp );
    }

    fpReserve = fopen( NULL_FILE, "r" );
    if ( fpReserve == NULL )
      bug("load_char: Can't open null file.", 0 );


    /* initialize race */
    if (found)
    {
	if (ORG_RACE(ch) == 0)
	    ORG_RACE(ch) = race_lookup("human");
	if (RACE(ch) == 0)
	    RACE(ch) = race_lookup("human");

	ch->size = pc_race_table[ORG_RACE(ch)].size;
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

	/* fix levels */
	if (ch->version < 3 && (ch->level > 35 || ch->trust > 35))
	{
	    switch (ch->level)
	    {
		case(40) : ch->level = MAX_LEVEL; break;  /* imp -> imp */
		case(39) : ch->level = MAX_LEVEL - 2; break;/* god -> supreme */
		case(38) : ch->level = MAX_LEVEL - 4; break;/* deity -> god */
		case(37) : ch->level = MAX_LEVEL - 7; break;/* angel->demigod */
	    }

            switch (ch->trust)
            {
		case(40) : ch->trust = MAX_LEVEL; break;/* imp -> imp */
		case(39) : ch->trust = MAX_LEVEL - 2; break;/* god -> supreme */
		case(38) : ch->trust = MAX_LEVEL - 4; break;/* deity -> god */
		case(37) : ch->trust = MAX_LEVEL - 7; break;/* angel->demigod */
		case(36) : ch->trust = LEVEL_HERO; break;/* hero -> hero */
            }
	}

	/* ream gold */
	if (ch->version < 4)
	{
	    ch->gold   /= 100;
	}

	/* anatolia 3.0 class adjustment to be compatible with old versions */
	if (ch->version == 6)
	{
    sprintf(log_buf, "%s bir Uzak Diyarlar oyuncusu deðil.", ch->name);
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
  if ( !str_cmp( word, literal ) )	\
  {					\
  field  = value;			\
  fMatch = TRUE;			\
  break;				\
}

void fread_char( CHAR_DATA *ch, FILE *fp )
{
    char buf[MAX_STRING_LENGTH];
    const char *word="End";
    bool fMatch;
    bool fPlayLog = FALSE;
    int count = 0;
    int lastlogoff = current_time;
    long dev_null;


    sprintf(buf,"Karakter yukleme: %s.",ch->name);
    log_string(buf);
    ch->pcdata->bank_s = 0;
    ch->pcdata->bank_g = 0;

    for ( ; ; )
    {
	word   = (char *)(feof( fp ) ? "End" : fread_word( fp ));
	fMatch = FALSE;

	switch ( UPPER(word[0]) )
	{
	case '*':
	    fMatch = TRUE;
	    fread_to_eol( fp );
	    break;

	case 'A':
	    KEY( "Act",		ch->act,		fread_flag( fp ) );
	    KEY( "AffectedBy",	dev_null,		fread_flag( fp ) );
	    KEY( "AfBy",	dev_null,		fread_flag( fp ) );
	    KEY( "Alignment",	ch->alignment,		fread_number( fp ) );
	    KEY( "Alig",	ch->alignment,		fread_number( fp ) );
	    KEY( "AntKilled",	ch->pcdata->anti_killed,fread_number( fp ) );

	    if (!str_cmp( word, "Alia"))
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

            if (!str_cmp( word, "Alias"))
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

	    if (!str_cmp( word, "AC") || !str_cmp(word,"Armor"))
	    {
		fread_to_eol(fp);
		fMatch = TRUE;
		break;
	    }

	    if (!str_cmp(word,"ACs"))
	    {
		int i;

		for (i = 0; i < 4; i++)
		    ch->armor[i] = fread_number(fp);
		fMatch = TRUE;
		break;
	    }

	    if (!str_cmp(word, "AffD"))
	    {
		AFFECT_DATA *paf;
		int sn;

		paf = new_affect();

		sn = skill_lookup(fread_word(fp));
		if (sn < 0)
		    bug("Fread_char: unknown skill.",0);
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

            if (!str_cmp(word, "Affc"))
            {
                AFFECT_DATA *paf;
                int sn;

                paf = new_affect();

                sn = skill_lookup(fread_word(fp));
                if (sn < 0)
                    bug("Fread_char: unknown skill.",0);
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

	    if ( !str_cmp( word, "AttrMod"  ) || !str_cmp(word,"AMod"))
	    {
		int stat;
		for (stat = 0; stat < MAX_STATS; stat ++)
		   dev_null = fread_number(fp);
		fMatch = TRUE;
		break;
	    }

	    if ( !str_cmp( word, "AttrPerm" ) || !str_cmp(word,"Attr"))
	    {
		int stat;

		for (stat = 0; stat < MAX_STATS; stat++)
		    ch->perm_stat[stat] = fread_number(fp);
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'B':
	    KEY( "Bamfin",	ch->pcdata->bamfin,	fread_string( fp ) );
            KEY( "Banks",        ch->pcdata->bank_s,       fread_number( fp ) );
            KEY( "Bankg",        ch->pcdata->bank_g,       fread_number( fp ) );
	    KEY( "Bamfout",	ch->pcdata->bamfout,	fread_string( fp ) );
	    KEY( "Bin",		ch->pcdata->bamfin,	fread_string( fp ) );
	    KEY( "Bout",	ch->pcdata->bamfout,	fread_string( fp ) );
	    break;

	case 'C':
	    KEY( "Class",	ch->iclass,		fread_number( fp ) );
	    KEY( "Cla",		ch->iclass,		fread_number( fp ) );
	    KEY( "Cab",		ch->cabal,		fread_number( fp ) );

	    if ( !str_cmp( word, "Condition" ) || !str_cmp(word,"Cond"))
	    {
		ch->pcdata->condition[0] = fread_number( fp );
		ch->pcdata->condition[1] = fread_number( fp );
		ch->pcdata->condition[2] = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
            if (!str_cmp(word,"CndC"))
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
            if (!str_cmp(word,"Cnd"))
            {
                ch->pcdata->condition[0] = fread_number( fp );
                ch->pcdata->condition[1] = fread_number( fp );
                ch->pcdata->condition[2] = fread_number( fp );
		ch->pcdata->condition[3] = fread_number( fp );
                fMatch = TRUE;
                break;
            }
	    KEY("Comm",		ch->comm,		fread_flag( fp ) );

	    break;

	case 'D':
	    KEY( "Damroll",	dev_null,		fread_number( fp ) );
	    KEY( "Dam",		dev_null,		fread_number( fp ) );
	    KEY( "Description",	ch->description,	fread_string( fp ) );
	    KEY( "Desc",	ch->description,	fread_string( fp ) );
	    KEY( "Dead",	ch->pcdata->death,	fread_number( fp ) );
    	    KEY( "Detect",	dev_null,		fread_flag(fp)     );
	    break;

	case 'E':
	    if ( !str_cmp( word, "End" ) )
	    {
    		/* adjust hp mana move up  -- here for speed's sake
		int percent;

    		percent = (current_time - lastlogoff) * 25 / ( 2 * 60 * 60);

		percent = UMIN(percent,100);

    		if (percent > 0 && !IS_AFFECTED(ch,AFF_POISON)
    		&&  !IS_AFFECTED(ch,AFF_PLAGUE))
    		{
        	    ch->hit	+= (ch->max_hit - ch->hit) * percent / 100;
        	    ch->mana    += (ch->max_mana - ch->mana) * percent / 100;
        	    ch->move    += (ch->max_move - ch->move)* percent / 100;
    		}
		*/
   		ch->played = ch->pcdata->played;

		/* if this is an old player, give some played time */
		if (!fPlayLog)
		    SET_BIT(ch->pcdata->time_flag, TLP_NOLOG);
		return;
	    }
	    KEY( "Exp",		ch->exp,		fread_number( fp ) );
	    KEY( "Etho",	ch->ethos,		fread_number( fp ) );
	    break;

      case 'F':
    		if (!str_cmp(word,"Fm"))
    	    	{
    		int sn;
    		int value;
    		char *temp;

    		value = fread_number( fp );
    		temp = fread_word( fp ) ;
    		sn = race_lookup(temp);
    		if ( sn < 0 )
    		{
    		    fprintf(stderr,"%s",temp);
    		    bug( "Fread_char: unknown race[for familya]. ", 0 );
    		}
    		else
    		    ch->pcdata->familya[sn] = value;
    		fMatch = TRUE;
    	    	}
    	break;

	case 'G':
	    KEY( "Gold",	ch->gold,		fread_number( fp ) );
            if ( !str_cmp( word, "Group" )  || !str_cmp(word,"Gr"))
            {
                fread_word( fp ) ;
                fMatch = TRUE;
            }
	    break;

	case 'H':
	    KEY( "Hitroll",	dev_null,		fread_number( fp ) );
	    KEY( "Hit",		dev_null,		fread_number( fp ) );
	    KEY( "Home",	ch->hometown,		fread_number( fp ) );
	    KEY( "Haskilled",	ch->pcdata->has_killed, fread_number( fp ) );

	    if ( !str_cmp( word, "HpManaMove" ) || !str_cmp(word,"HMV"))
	    {
		ch->hit		= fread_number( fp );
		ch->mana	= fread_number( fp );
		ch->move	= fread_number( fp );
		fMatch = TRUE;
		break;
	    }

            if ( !str_cmp( word, "HpManaMovePerm" ) || !str_cmp(word,"HMVP"))
            {
                ch->pcdata->perm_hit	= fread_number( fp );
                ch->pcdata->perm_mana   = fread_number( fp );
                ch->pcdata->perm_move   = fread_number( fp );
                fMatch = TRUE;
                break;
            }

	    break;

	case 'I':
	    KEY( "Id",		ch->id,			fread_number( fp ) );
	    KEY( "InvisLevel",	ch->invis_level,	fread_number( fp ) );
	    KEY( "Inco",	ch->incog_level,	fread_number( fp ) );
	    KEY( "Invi",	ch->invis_level,	fread_number( fp ) );
	    break;

	case 'L':
	    KEY( "LastLevel",	ch->pcdata->last_level, fread_number( fp ) );
	    KEY( "LLev",	ch->pcdata->last_level, fread_number( fp ) );
	    KEY( "Level",	ch->level,		fread_number( fp ) );
	    KEY( "Lev",		ch->level,		fread_number( fp ) );
	    KEY( "Levl",	ch->level,		fread_number( fp ) );
	    KEY( "LogO",	lastlogoff,		fread_number( fp ) );
	    KEY( "LongDescr",	ch->long_descr,		fread_string( fp ) );
	    KEY( "LnD",		ch->long_descr,		fread_string( fp ) );
	    break;

	case 'N':
	    KEY( "Name",	ch->name,		fread_string( fp ) );
	    KEY( "Note",	ch->pcdata->last_note,	fread_number( fp ) );
	    if (!str_cmp(word,"Not"))
	    {
		ch->pcdata->last_note			= fread_number(fp);
		ch->pcdata->last_idea			= fread_number(fp);
		ch->pcdata->last_penalty		= fread_number(fp);
		ch->pcdata->last_news			= fread_number(fp);
		ch->pcdata->last_changes		= fread_number(fp);
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'P':
	    KEY( "Password",	ch->pcdata->pwd,	fread_string( fp ) );
	    KEY( "Pass",	ch->pcdata->pwd,	fread_string( fp ) );
	    KEY( "Played",	ch->pcdata->played,	fread_number( fp ) );
	    KEY( "Plyd",	ch->pcdata->played,	fread_number( fp ) );
	    KEY( "Points",	ch->pcdata->points,	fread_number( fp ) );
	    KEY( "Pnts",	ch->pcdata->points,	fread_number( fp ) );
	    KEY( "Position",	ch->position,		fread_number( fp ) );
	    KEY( "Pos",		ch->position,		fread_number( fp ) );
	    KEY( "Practice",	ch->practice,		fread_number( fp ) );
	    KEY( "Prac",	ch->practice,		fread_number( fp ) );
            KEY( "Prompt",      ch->prompt,             fread_string( fp ) );
 	    KEY( "Prom",	ch->prompt,		fread_string( fp ) );
	    if (!str_cmp(word,"PlayLog"))
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
	    KEY( "QuestCnt",	ch->pcdata->countdown,	fread_number( fp) );
	    KEY( "QuestMob",	ch->pcdata->questmob,	fread_number( fp) );
	    KEY( "QuestObj",	ch->pcdata->questobj,	fread_number( fp) );
	    KEY( "QuestGiv",	ch->pcdata->questgiver,	fread_number( fp) );
	    KEY( "QuestPnts",   ch->pcdata->questpoints,fread_number( fp) );
            KEY( "QuestNext",   ch->pcdata->nextquest,	fread_number( fp) );
       	    KEY( "Ques",	ch->quest, 		fread_flag( fp)	  );
	    break;

	case 'R':
	    KEY( "Relig",       ch->religion,		fread_number( fp ) );

/*	    KEY( "Race",        ch->race,  race_lookup(fread_string( fp )) ); */
	    if ( !str_cmp( word, "Race" ) )
	    {
		RACE(ch) = race_lookup(fread_string(fp));
		ORG_RACE(ch) = RACE(ch);
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "Room" ) )
	    {
		ch->in_room = get_room_index( fread_number( fp ) );
		if ( ch->in_room == NULL )
		    ch->in_room = get_room_index( ROOM_VNUM_LIMBO );
		fMatch = TRUE;
		break;
	    }

	    break;

	case 'S':
	    KEY( "SavingThrow",	dev_null,		fread_number( fp ) );
	    KEY( "Save",	dev_null,		fread_number( fp ) );
	    KEY( "Scro",	ch->lines,		fread_number( fp ) );
	    KEY( "Sex",		ch->sex,		fread_number( fp ) );
	    KEY( "ShortDescr",	ch->short_descr,	fread_string( fp ) );
	    KEY( "ShD",		ch->short_descr,	fread_string( fp ) );
            KEY( "Silv",        ch->silver,             fread_number( fp ) );


	    if ( !str_cmp( word, "Skill" ) || !str_cmp(word,"Sk"))
	    {
		int sn;
		int value;
		char *temp;

		value = fread_number( fp );
		temp = fread_word( fp ) ;
		sn = skill_lookup(temp);
		/* sn    = skill_lookup( fread_word( fp ) ); */
		if ( sn < 0 )
		{
		    fprintf(stderr,"%s",temp);
		    bug( "Fread_char: unknown skill. ", 0 );
		}
		else
		    ch->pcdata->learned[sn] = value;
		fMatch = TRUE;
	    }

	    break;

	case 'T':
            KEY( "TrueSex",     ch->pcdata->true_sex,  	fread_number( fp ) );
	    KEY( "TSex",	ch->pcdata->true_sex,   fread_number( fp ) );
	    KEY( "Trai",	ch->train,		fread_number( fp ) );
	    KEY( "Trust",	ch->trust,		fread_number( fp ) );
	    KEY( "Tru",		ch->trust,		fread_number( fp ) );

	    if ( !str_cmp( word, "Title" )  || !str_cmp( word, "Titl"))
	    {
		ch->pcdata->title = fread_string( fp );
    		if (ch->pcdata->title[0] != '.' && ch->pcdata->title[0] != ','
		&&  ch->pcdata->title[0] != '!' && ch->pcdata->title[0] != '?')
		{
		    sprintf( buf, " %s", ch->pcdata->title );
		    free_string( ch->pcdata->title );
		    ch->pcdata->title = str_dup( buf );
		}
		fMatch = TRUE;
		break;
	    }

	    break;

	case 'V':
	    KEY( "Version",     ch->version,		fread_number ( fp ) );
	    KEY( "Vers",	ch->version,		fread_number ( fp ) );
	    if ( !str_cmp( word, "Vnum" ) )
	    {
		ch->pIndexData = get_mob_index( fread_number( fp ) );
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'W':
	    KEY( "Wimpy",	ch->wimpy,		fread_number( fp ) );
	    KEY( "Wimp",	ch->wimpy,		fread_number( fp ) );
	    KEY( "Wizn",	ch->wiznet,		fread_flag( fp ) );
	    break;
	}

	if ( !fMatch )
	{
	    bug( "Fread_char: no match.", 0 );
	    fread_to_eol( fp );
	}
    }
 return;
}

/* load a pet from the forgotten reaches */
void fread_pet( CHAR_DATA *ch, FILE *fp )
{
    char *word;
    CHAR_DATA *pet;
    bool fMatch;
    int lastlogoff = current_time;
    int percent;

    /* first entry had BETTER be the vnum or we barf */
    word = (char *)(feof(fp) ? "END" : fread_word(fp));
    if (!str_cmp(word,"Vnum"))
    {
    	int vnum;

    	vnum = fread_number(fp);
    	if (get_mob_index(vnum) == NULL)
	{
    	    bug("Fread_pet: bad vnum %d.",vnum);
	    pet = create_mobile(get_mob_index(MOB_VNUM_FIDO));
	}
    	else
    	    pet = create_mobile(get_mob_index(vnum));
    }
    else
    {
        bug("Fread_pet: no vnum in file.",0);
        pet = create_mobile(get_mob_index(MOB_VNUM_FIDO));
    }

    for ( ; ; )
    {
    	word 	= (char *)(feof(fp) ? "END" : fread_word(fp));
    	fMatch = FALSE;

    	switch (UPPER(word[0]))
    	{
    	case '*':
    	    fMatch = TRUE;
    	    fread_to_eol(fp);
    	    break;

    	case 'A':
    	    KEY( "Act",		pet->act,		fread_flag(fp));
    	    KEY( "AfBy",	pet->affected_by,	fread_flag(fp));
    	    KEY( "Alig",	pet->alignment,		fread_number(fp));

    	    if (!str_cmp(word,"ACs"))
    	    {
    	    	int i;

    	    	for (i = 0; i < 4; i++)
    	    	    pet->armor[i] = fread_number(fp);
    	    	fMatch = TRUE;
    	    	break;
    	    }

    	    if (!str_cmp(word,"AffD"))
    	    {
    	    	AFFECT_DATA *paf;
    	    	int sn;

    	    	paf = new_affect();

    	    	sn = skill_lookup(fread_word(fp));
    	     	if (sn < 0)
    	     	    bug("Fread_char: unknown skill.",0);
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

            if (!str_cmp(word,"Affc"))
            {
                AFFECT_DATA *paf;
                int sn;

                paf = new_affect();

                sn = skill_lookup(fread_word(fp));
                if (sn < 0)
                    bug("Fread_char: unknown skill.",0);
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

    	    if (!str_cmp(word,"AMod"))
    	    {
    	     	int stat;

    	     	for (stat = 0; stat < MAX_STATS; stat++)
    	     	    pet->mod_stat[stat] = fread_number(fp);
    	     	fMatch = TRUE;
    	     	break;
    	    }

    	    if (!str_cmp(word,"Attr"))
    	    {
    	         int stat;

    	         for (stat = 0; stat < MAX_STATS; stat++)
    	             pet->perm_stat[stat] = fread_number(fp);
    	         fMatch = TRUE;
    	         break;
    	    }
    	    break;

    	 case 'C':
             KEY( "Cab",	pet->cabal,             fread_number(fp) );
    	     KEY( "Comm",	pet->comm,		fread_flag(fp));
    	     break;

    	 case 'D':
    	     KEY( "Dam",	pet->damroll,		fread_number(fp));
    	     KEY( "Desc",	pet->description,	fread_string(fp));
	     KEY( "Detect",	pet->detection,		fread_flag(fp));
    	     break;

    	 case 'E':
    	     if (!str_cmp(word,"End"))
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
    	     KEY( "Exp",	pet->exp,		fread_number(fp));
    	     break;

    	 case 'G':
    	     KEY( "Gold",	pet->gold,		fread_number(fp));
    	     break;

    	 case 'H':
    	     KEY( "Hit",	pet->hitroll,		fread_number(fp));

    	     if (!str_cmp(word,"HMV"))
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
    	     KEY( "Levl",	pet->level,		fread_number(fp));
    	     KEY( "LnD",	pet->long_descr,	fread_string(fp));
	     KEY( "LogO",	lastlogoff,		fread_number(fp));
    	     break;

    	case 'N':
    	     KEY( "Name",	pet->name,		fread_string(fp));
    	     break;

    	case 'P':
    	     KEY( "Pos",	pet->position,		fread_number(fp));
    	     break;

	case 'R':
	    if ( !str_cmp( word, "Race" ) )
	    {
		RACE(pet) = race_lookup(fread_string(fp));
		ORG_RACE(pet) = RACE(pet);
		fMatch = TRUE;
		break;
	    }
    	    break;

    	case 'S' :
    	    KEY( "Save",	pet->saving_throw,	fread_number(fp));
    	    KEY( "Sex",		pet->sex,		fread_number(fp));
    	    KEY( "ShD",		pet->short_descr,	fread_string(fp));
            KEY( "Silv",        pet->silver,            fread_number( fp ) );
    	    break;

    	if ( !fMatch )
    	{
    	    bug("Fread_pet: no match.",0);
    	    fread_to_eol(fp);
    	}

    	}
    }
}



void fread_obj( CHAR_DATA *ch, FILE *fp )
{
    OBJ_DATA *obj;
    char *word;
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

    word   = (char *)(feof( fp ) ? "End" : fread_word( fp ));
    if (!str_cmp(word,"Vnum" ))
    {
        int vnum;
	first = FALSE;  /* fp will be in right place */

        vnum = fread_number( fp );
        if (  get_obj_index( vnum )  == NULL )
	{
            bug( "Fread_obj: bad vnum %d.", vnum );
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
    	obj->name		= str_dup( "" );
    	obj->short_descr	= str_dup( "" );
    	obj->description	= str_dup( "" );
    }

    fNest		= FALSE;
    fVnum		= TRUE;
    iNest		= 0;

    for ( ; ; )
    {
	if (first)
	    first = FALSE;
	else
	    word   = (char *)(feof( fp ) ? "End" : fread_word( fp ));
	fMatch = FALSE;

	switch ( UPPER(word[0]) )
	{
	case '*':
	    fMatch = TRUE;
	    fread_to_eol( fp );
	    break;

	case 'A':
	    if (!str_cmp(word,"AffD"))
	    {
		AFFECT_DATA *paf;
		int sn;

		paf = new_affect();

		sn = skill_lookup(fread_word(fp));
		if (sn < 0)
		    bug("Fread_obj: unknown skill.",0);
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
            if (!str_cmp(word,"Affc"))
            {
                AFFECT_DATA *paf;
                int sn;

                paf = new_affect();

                sn = skill_lookup(fread_word(fp));
                if (sn < 0)
                    bug("Fread_obj: unknown skill.",0);
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
	    if ( !str_cmp( word, "Cond"))
	    {
		obj->condition = fread_number(fp);
		if (obj->condition < 1) obj->condition = 100;
	 	fMatch 	= TRUE;
		break;
	    }
	    KEY( "Cost",	obj->cost,		fread_number( fp ) );
	    break;

	case 'D':
	    KEY( "Description",	obj->description,	fread_string( fp ) );
	    KEY( "Desc",	obj->description,	fread_string( fp ) );
	    break;

	case 'E':

	    if ( !str_cmp( word, "Enchanted"))
	    {
		obj->enchanted = TRUE;
	 	fMatch 	= TRUE;
		break;
	    }

	    KEY( "ExtraFlags",	obj->extra_flags,	fread_number( fp ) );
	    KEY( "ExtF",	obj->extra_flags,	fread_number( fp ) );

	    if ( !str_cmp( word, "ExtraDescr" ) || !str_cmp(word,"ExDe"))
	    {
		EXTRA_DESCR_DATA *ed;

		ed = new_extra_descr();

		ed->keyword		= fread_string( fp );
		ed->description		= fread_string( fp );
		ed->next		= obj->extra_descr;
		obj->extra_descr	= ed;
		fMatch = TRUE;
	    }

	    if ( !str_cmp( word, "End" ) )
	    {
		if ( !fNest || !fVnum || obj->pIndexData == NULL)
		{
		    bug( "Fread_obj: incomplete object.", 0 );
		    free_obj(obj);
		    return;
		}
		else if (obj->pIndexData->limit != -1
			&& get_total_played(ch) < MIN_TIME_LIMIT )
		{
		    sprintf(log_buf, "Ignoring limited %d for %s.",
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
	    KEY( "ItemType",	obj->item_type,		fread_number( fp ) );
	    KEY( "Ityp",	obj->item_type,		fread_number( fp ) );
	    break;

	case 'L':
	    KEY( "Level",	obj->level,		fread_number( fp ) );
	    KEY( "Lev",		obj->level,		fread_number( fp ) );
	    break;

	case 'N':
	    KEY( "Name",	obj->name,		fread_string( fp ) );

	    if ( !str_cmp( word, "Nest" ) )
	    {
		iNest = fread_number( fp );
		if ( iNest < 0 || iNest >= MAX_NEST )
		{
		    bug( "Fread_obj: bad nest %d.", iNest );
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
	    if ( !str_cmp( word,"Oldstyle" ) )
	    {
		if (obj->pIndexData != NULL && obj->pIndexData->new_format)
		    make_new = TRUE;
		fMatch = TRUE;
	    }
	    break;


	case 'Q':
	    KEY( "Quality",	obj->condition,		fread_number( fp ) );
	    break;

	case 'S':
	    KEY( "ShortDescr",	obj->short_descr,	fread_string( fp ) );
	    KEY( "ShD",		obj->short_descr,	fread_string( fp ) );

	    if ( !str_cmp( word, "Spell" ) )
	    {
		int iValue;
		int sn;

		iValue = fread_number( fp );
		sn     = skill_lookup( fread_word( fp ) );
		if ( iValue < 0 || iValue > 3 )
		{
		    bug( "Fread_obj: bad iValue %d.", iValue );
		}
		else if ( sn < 0 )
		{
		    bug( "Fread_obj: unknown skill.", 0 );
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
	    KEY( "Timer",	obj->timer,		fread_number( fp ) );
	    KEY( "Time",	obj->timer,		fread_number( fp ) );
	    break;

	case 'V':
	    if ( !str_cmp( word, "Values" ) || !str_cmp(word,"Vals"))
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

	    if ( !str_cmp( word, "Val" ) )
	    {
		obj->value[0] 	= fread_number( fp );
	 	obj->value[1]	= fread_number( fp );
	 	obj->value[2] 	= fread_number( fp );
		obj->value[3]	= fread_number( fp );
		obj->value[4]	= fread_number( fp );
		fMatch = TRUE;
		break;
	    }

	    if ( !str_cmp( word, "Vnum" ) )
	    {
		int vnum;

		vnum = fread_number( fp );
		if ( ( obj->pIndexData = get_obj_index( vnum ) ) == NULL )
		    bug( "Fread_obj: bad vnum %d.", vnum );
		else
		    fVnum = TRUE;
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'W':
	    KEY( "WearFlags",	obj->wear_flags,	fread_number( fp ) );
	    KEY( "WeaF",	obj->wear_flags,	fread_number( fp ) );
	    KEY( "Weight",	obj->weight,		fread_number( fp ) );
	    KEY( "WLoc",	obj->wear_loc,		fread_number( fp ) );
	    KEY( "Wt",		obj->weight,		fread_number( fp ) );
	    if ( !str_cmp( word, "Wear" ) )
	    {
		obj->wear_loc 	= wear_convert(fread_number( fp ));
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "Wearloc" ) )
	    {
		obj->wear_loc 	= wear_convert(fread_number( fp ));
		fMatch = TRUE;
		break;
	    }
	    break;

	}

	if ( !fMatch )
	{
	    bug( "Fread_obj: no match.", 0 );
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
