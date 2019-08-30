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

/**************************************************************************r
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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "magic.h"
#include "recycle.h"
#include "tables.h"

/* command procedures needed */
DECLARE_DO_FUN(do_return	);
DECLARE_DO_FUN(do_wake		);
DECLARE_DO_FUN(do_raffects	);
DECLARE_DO_FUN(do_say		);
DECLARE_DO_FUN(do_track		);



/*
 * Local functions.
 */
void	affect_modify	args( ( CHAR_DATA *ch, AFFECT_DATA *paf, bool fAdd ) );
int	age_to_num	args( ( int age) );
void	raffect_to_char	args( ( ROOM_INDEX_DATA *room, CHAR_DATA *ch) );
void	raffect_back_char	args( ( ROOM_INDEX_DATA *room, CHAR_DATA *ch) );
bool	is_safe_rspell	args( ( int level, CHAR_DATA *victim) );
ROOM_INDEX_DATA *	find_location	args( ( CHAR_DATA *ch, char *arg ) );

/* friend stuff -- for NPC's mostly */
bool is_friend(CHAR_DATA *ch,CHAR_DATA *victim)
{
    if (is_same_group(ch,victim))
	return TRUE;


    if (!IS_NPC(ch))
	return FALSE;

    if (!IS_NPC(victim))
    {
	if (IS_SET(ch->off_flags,ASSIST_PLAYERS))
	    return TRUE;
	else
	    return FALSE;
    }

    if (IS_AFFECTED(ch,AFF_CHARM))
	return FALSE;

    if (IS_SET(ch->off_flags,ASSIST_ALL))
	return TRUE;

    if (ch->group && ch->group == victim->group)
	return TRUE;

    if (IS_SET(ch->off_flags,ASSIST_VNUM)
    &&  ch->pIndexData == victim->pIndexData)
	return TRUE;

    if (IS_SET(ch->off_flags,ASSIST_RACE) && RACE(ch) == RACE(victim) )
	return TRUE;

    if (IS_SET(ch->off_flags,ASSIST_ALIGN)
    &&  !IS_SET(ch->act,ACT_NOALIGN) && !IS_SET(victim->act,ACT_NOALIGN)
    &&  ((IS_GOOD(ch) && IS_GOOD(victim))
    ||	 (IS_EVIL(ch) && IS_EVIL(victim))
    ||   (IS_NEUTRAL(ch) && IS_NEUTRAL(victim))))
	return TRUE;

    return FALSE;
}

/*
 * Room record:
 * For less than 5 people in room create a new record.
 * Else use the oldest one.
 */

void room_record(char *name,ROOM_INDEX_DATA *room,sh_int door)
{
  ROOM_HISTORY_DATA *rh;
  int i=0;

  for (rh = room->history; i < 5 && rh != NULL; i++,rh = rh->next);

  if (i < 5)
    rh = (ROOM_HISTORY_DATA *)alloc_perm(sizeof(ROOM_HISTORY_DATA));
  else
  {
    rh = room->history->next->next->next->next;
    room->history->next->next->next->next = NULL;
    free_string(rh->name);
  }

  rh->next = room->history;
  room->history = rh;
  rh->name = str_dup(name);
  rh->went = door;
}



/* returns number of people on an object */
int count_users(OBJ_DATA *obj)
{
    CHAR_DATA *fch;
    int count = 0;

    if (obj->in_room == NULL)
	return 0;

    for (fch = obj->in_room->people; fch != NULL; fch = fch->next_in_room)
	if (fch->on == obj)
	    count++;

    return count;
}

/* returns material number */
int material_lookup (const char *name)
{
    return 0;
}

/* returns race number */
int race_lookup (const char *name)
{
   int race;

   for ( race = 0; race_table[race].name[0] != NULL; race++)
   {
	if ( (LOWER(name[0]) == LOWER(race_table[race].name[0][0]))
	&&  !str_prefix( name,race_table[race].name[0]))
	    return race;
   }

   return 0;
}

int liq_lookup (const char *name)
{
    int liq;

    for ( liq = 0; liq_table[liq].liq_name != NULL; liq++)
    {
	if (LOWER(name[0]) == LOWER(liq_table[liq].liq_name[0])
	&& !str_prefix(name,liq_table[liq].liq_name))
	    return liq;
    }

    return LIQ_WATER;
}

int weapon_lookup (const char *name)
{
    int type;

    for (type = 0; weapon_table[type].name != NULL; type++)
    {
	if (LOWER(name[0]) == LOWER(weapon_table[type].name[0])
	&&  !str_prefix(name,weapon_table[type].name))
	    return type;
    }

    return -1;
}

bool cabal_ok(CHAR_DATA *ch, sh_int sn)
{
  int i;

  if (IS_NPC(ch) || skill_table[sn].cabal == CABAL_NONE ||
      cabal_table[ch->cabal].obj_ptr == NULL ||
      cabal_table[ch->cabal].obj_ptr->in_room == NULL ||
      cabal_table[ch->cabal].obj_ptr->in_room->vnum ==
        cabal_table[ch->cabal].room_vnum)
    return TRUE;

  for (i=1;i < MAX_CABAL; i++)
    if (cabal_table[ch->cabal].obj_ptr->in_room->vnum ==
          cabal_table[i].room_vnum) {
            send_to_char( "Kabal gücünün varolmadýðýný hissediyorsun.\n\r",ch);
      return FALSE;
    }

  return TRUE;
}

int weapon_type (const char *name)
{
    int type;

    for (type = 0; weapon_table[type].name != NULL; type++)
    {
        if (LOWER(name[0]) == LOWER(weapon_table[type].name[0])
        &&  !str_prefix(name,weapon_table[type].name))
            return weapon_table[type].type;
    }

    return WEAPON_EXOTIC;
}


int item_lookup(const char *name)
{
    int type;

    for (type = 0; item_table[type].name != NULL; type++)
    {
        if (LOWER(name[0]) == LOWER(item_table[type].name[0])
        &&  !str_prefix(name,item_table[type].name))
            return item_table[type].type;
    }

    return -1;
}

const char *item_name(int item_type)
{
    int type;

    for (type = 0; item_table[type].name != NULL; type++)
	if (item_type == item_table[type].type)
	    return item_table[type].name;
    return "none";
}

const char *weapon_name( int weapon_type)
{
    int type;

    for (type = 0; weapon_table[type].name != NULL; type++)
        if (weapon_type == weapon_table[type].type)
            return weapon_table[type].name;
    return "exotic";
}

/*
 * Check the material
 */
bool check_material( OBJ_DATA *obj, char *material )
{
    if ( strstr( obj->material, material ) != NULL )
	return TRUE;
    else
        return FALSE;

}

bool is_metal( OBJ_DATA *obj )
{

  if ( check_material(obj, (char*)"silver") ||
       check_material(obj, (char*)"gold") ||
       check_material(obj, (char*)"iron") ||
       check_material(obj, (char*)"mithril") ||
       check_material(obj, (char*)"adamantite") ||
       check_material(obj, (char*)"steel") ||
       check_material(obj, (char*)"lead") ||
       check_material(obj, (char*)"bronze") ||
       check_material(obj, (char*)"copper") ||
       check_material(obj, (char*)"brass") ||
       check_material(obj, (char*)"platinium") ||
       check_material(obj, (char*)"titanium") ||
       check_material(obj, (char*)"aliminum") )
    return TRUE;

  return FALSE;

}

bool may_float( OBJ_DATA *obj )
{

    if ( check_material( obj, (char*)"wood" )  ||
         check_material( obj, (char*)"ebony" )  ||
         check_material( obj, (char*)"ice" )  ||
         check_material( obj, (char*)"energy" )  ||
         check_material( obj, (char*)"hardwood" )  ||
         check_material( obj, (char*)"softwood" )  ||
         check_material( obj, (char*)"flesh" )  ||
         check_material( obj, (char*)"silk" )  ||
         check_material( obj, (char*)"wool" )  ||
         check_material( obj, (char*)"cloth" )  ||
         check_material( obj, (char*)"fur" )  ||
         check_material( obj, (char*)"water" )  ||
         check_material( obj, (char*)"ice" )  ||
         check_material( obj, (char*)"oak" ) )
       return TRUE;

    if ( obj->item_type == ITEM_BOAT )
	return TRUE;

    return FALSE;
}


bool cant_float( OBJ_DATA *obj )
{
    if ( check_material( obj, (char*)"steel" ) ||
         check_material( obj, (char*)"iron" ) ||
         check_material( obj, (char*)"brass" ) ||
         check_material( obj, (char*)"silver" ) ||
         check_material( obj, (char*)"gold" ) ||
         check_material( obj, (char*)"ivory" ) ||
         check_material( obj, (char*)"copper" ) ||
         check_material( obj, (char*)"diamond" ) ||
         check_material( obj, (char*)"pearl" ) ||
         check_material( obj, (char*)"gem" ) ||
         check_material( obj, (char*)"platinium" ) ||
         check_material( obj, (char*)"ruby" ) ||
         check_material( obj, (char*)"bronze" ) ||
         check_material( obj, (char*)"titanium" ) ||
         check_material( obj, (char*)"mithril" ) ||
         check_material( obj, (char*)"obsidian" ) ||
         check_material( obj, (char*)"lead" ) )
       return TRUE;

    return FALSE;
}

int floating_time( OBJ_DATA *obj )
{
 int  ftime;

 ftime = 0;
 switch( obj->item_type )
 {
    default: break;
    case ITEM_KEY 	: ftime = 1;	break;
    case ITEM_ARMOR 	: ftime = 2;	break;
    case ITEM_TREASURE 	: ftime = 2;	break;
    case ITEM_PILL 	: ftime = 2;	break;
    case ITEM_POTION 	: ftime = 3;	break;
    case ITEM_TRASH 	: ftime = 3;	break;
    case ITEM_FOOD 	: ftime = 4;	break;
    case ITEM_CONTAINER	: ftime = 5;	break;
    case ITEM_CORPSE_NPC: ftime = 10;	break;
    case ITEM_CORPSE_PC	: ftime = 10;	break;
 }
 ftime = number_fuzzy( ftime ) ;

 return ( ftime < 0 ? 0 : ftime);
}

int attack_lookup  (const char *name)
{
    int att;

    for ( att = 0; attack_table[att].name != NULL; att++)
    {
	if (LOWER(name[0]) == LOWER(attack_table[att].name[0])
	&&  !str_prefix(name,attack_table[att].name))
	    return att;
    }

    return 0;
}

/* returns a flag for wiznet */
long wiznet_lookup (const char *name)
{
    int flag;

    for (flag = 0; wiznet_table[flag].name != NULL; flag++)
    {
	if (LOWER(name[0]) == LOWER(wiznet_table[flag].name[0])
	&& !str_prefix(name,wiznet_table[flag].name))
	    return flag;
    }

    return -1;
}

/* returns class number */
int class_lookup (const char *name)
{
   int iclass;

   for ( iclass = 0; iclass < MAX_CLASS; iclass++)
   {
        if (LOWER(name[0]) == LOWER(class_table[iclass].name[0][0])
        &&  !str_prefix( name,class_table[iclass].name[0]))
	{
            return iclass;
	}
   }

   return -1;
}

/* for immunity, vulnerabiltiy, and resistant
   the 'globals' (magic and weapons) may be overriden
   three other cases -- wood, silver, and iron -- are checked in fight.c */

int check_immune(CHAR_DATA *ch, int dam_type)
{
    int immune, def;
    int bit;

    immune = -1;
    def = IS_NORMAL;

    if (dam_type == DAM_NONE)
	return immune;

    if (dam_type <= 3)
    {
	if (IS_SET(ch->imm_flags,IMM_WEAPON))
	    def = IS_IMMUNE;
	else if (IS_SET(ch->res_flags,RES_WEAPON))
	    def = IS_RESISTANT;
	else if (IS_SET(ch->vuln_flags,VULN_WEAPON))
	    def = IS_VULNERABLE;
    }
    else /* magical attack */
    {
	if (IS_SET(ch->imm_flags,IMM_MAGIC))
	    def = IS_IMMUNE;
	else if (IS_SET(ch->res_flags,RES_MAGIC))
	    def = IS_RESISTANT;
	else if (IS_SET(ch->vuln_flags,VULN_MAGIC))
	    def = IS_VULNERABLE;
    }

    /* set bits to check -- VULN etc. must ALL be the same or this will fail */
    switch (dam_type)
    {
	case(DAM_BASH):		bit = IMM_BASH;		break;
	case(DAM_PIERCE):	bit = IMM_PIERCE;	break;
	case(DAM_SLASH):	bit = IMM_SLASH;	break;
	case(DAM_FIRE):		bit = IMM_FIRE;		break;
	case(DAM_COLD):		bit = IMM_COLD;		break;
	case(DAM_LIGHTNING):	bit = IMM_LIGHTNING;	break;
	case(DAM_ACID):		bit = IMM_ACID;		break;
	case(DAM_POISON):	bit = IMM_POISON;	break;
	case(DAM_NEGATIVE):	bit = IMM_NEGATIVE;	break;
	case(DAM_HOLY):		bit = IMM_HOLY;		break;
	case(DAM_ENERGY):	bit = IMM_ENERGY;	break;
	case(DAM_MENTAL):	bit = IMM_MENTAL;	break;
	case(DAM_DISEASE):	bit = IMM_DISEASE;	break;
	case(DAM_DROWNING):	bit = IMM_DROWNING;	break;
	case(DAM_LIGHT):	bit = IMM_LIGHT;	break;
	case(DAM_CHARM):	bit = IMM_CHARM;	break;
	case(DAM_SOUND):	bit = IMM_SOUND;	break;
	default:		return def;
    }

    if (IS_SET(ch->imm_flags,bit))
	immune = IS_IMMUNE;
    else if (IS_SET(ch->res_flags,bit) && immune != IS_IMMUNE)
	immune = IS_RESISTANT;
    else if (IS_SET(ch->vuln_flags,bit))
    {
	if (immune == IS_IMMUNE)
	    immune = IS_RESISTANT;
	else if (immune == IS_RESISTANT)
	    immune = IS_NORMAL;
	else
	    immune = IS_VULNERABLE;
    }

    if (!IS_NPC(ch) && get_curr_stat(ch, STAT_CHA) < 18
		&& dam_type == DAM_CHARM)
	immune = IS_VULNERABLE;

    if (immune == -1)
	return def;
    else
      	return immune;
}

/* checks mob format */
bool is_old_mob(CHAR_DATA *ch)
{
    if (ch->pIndexData == NULL)
	return FALSE;
    else if (ch->pIndexData->new_format)
	return FALSE;
    return TRUE;
}

/* for returning skill information */
int get_skill(CHAR_DATA *ch, int sn)
{
    int skill;

    if (sn == -1) /* shorthand for level based skills */
    {
	skill = ch->level * 5 / 2;
    }

    else if (sn < -1 || sn > MAX_SKILL)
    {
	bug("Bad sn %d in get_skill.",sn);
	skill = 0;
    }

    else if (!IS_NPC(ch))
    {
	if (ch->level < skill_table[sn].skill_level[ch->iclass])
	    skill = 0;
	else
	    skill = ch->pcdata->learned[sn];
    }

    else /* mobiles */
    {

        if (skill_table[sn].spell_fun != spell_null)
	    skill = 40 + 2 * ch->level;

	else if (sn == gsn_sneak || sn == gsn_hide)
	    skill = ch->level + 20;

        else if ((sn == gsn_dodge && IS_SET(ch->off_flags,OFF_DODGE))
 	||       (sn == gsn_parry && IS_SET(ch->off_flags,OFF_PARRY)))
	    skill = ch->level * 2;

 	else if (sn == gsn_shield_block)
	    skill = 10 + 2 * ch->level;

	else if (sn == gsn_second_attack )
	    skill = 30 + ch->level;

	else if (sn == gsn_third_attack
	&& (IS_SET(ch->act,ACT_WARRIOR) || IS_SET(ch->act,ACT_THIEF)))
	    skill = 30 + ch->level / 2;

	else if (sn == gsn_fourth_attack && IS_SET(ch->act,ACT_WARRIOR))
	    skill = 20 + ch->level / 2;

	else if (sn == gsn_second_weapon && IS_SET(ch->act,ACT_WARRIOR))
	    skill = 30 + ch->level / 2;

	else if (sn == gsn_hand_to_hand)
	    skill = 40 + 2 * ch->level;

 	else if (sn == gsn_trip && IS_SET(ch->off_flags,OFF_TRIP))
	    skill = 10 + 3 * ch->level;

 	else if (sn == gsn_bash && IS_SET(ch->off_flags,OFF_BASH))
	    skill = 10 + 3 * ch->level;

	else if (sn == gsn_disarm
	     &&  (IS_SET(ch->off_flags,OFF_DISARM)
	     ||   IS_SET(ch->act,ACT_WARRIOR)
	     ||	  IS_SET(ch->act,ACT_THIEF)))
	    skill = 20 + 3 * ch->level;

	else if (sn == gsn_grip
	     &&  (IS_SET(ch->act,ACT_WARRIOR)
	     ||	  IS_SET(ch->act,ACT_THIEF)))
	    skill = ch->level;

	else if (sn == gsn_berserk && IS_SET(ch->off_flags,OFF_BERSERK))
	    skill = 3 * ch->level;

	else if (sn == gsn_kick)
	    skill = 10 + 3 * ch->level;

	else if (sn == gsn_backstab && IS_SET(ch->act,ACT_THIEF))
	    skill = 20 + 2 * ch->level;

  	else if (sn == gsn_rescue)
	    skill = 40 + ch->level;

	else if (sn == gsn_recall)
	    skill = 40 + ch->level;

	else if (sn == gsn_sword
	||  sn == gsn_dagger
	||  sn == gsn_spear
	||  sn == gsn_mace
	||  sn == gsn_axe
	||  sn == gsn_flail
	||  sn == gsn_whip
	||  sn == gsn_polearm
	||  sn == gsn_bow
	||  sn == gsn_arrow
	||  sn == gsn_lance)
	    skill = 40 + 5 * ch->level / 2;

	else
	   skill = 0;
    }

    if (ch->daze > 0)
    {
	if (skill_table[sn].spell_fun != spell_null)
	    skill /= 2;
	else
	    skill = 2 * skill / 3;
    }

    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK]  > 10 )
	skill = 9 * skill / 10;

    if ( ch->hit < (ch->max_hit * 0.6))
	skill = 9 * skill / 10;

    return URANGE(0,skill,100);
}

/* for returning weapon information */
int get_weapon_sn(CHAR_DATA *ch, bool second)
{
    OBJ_DATA *wield;
    int sn;

    wield = get_wield_char( ch, second );

    if (wield == NULL || wield->item_type != ITEM_WEAPON)
        sn = gsn_hand_to_hand;
    else switch (wield->value[0])
    {
        default :               sn = -1;                break;
        case(WEAPON_SWORD):     sn = gsn_sword;         break;
        case(WEAPON_DAGGER):    sn = gsn_dagger;        break;
        case(WEAPON_SPEAR):     sn = gsn_spear;         break;
        case(WEAPON_MACE):      sn = gsn_mace;          break;
        case(WEAPON_AXE):       sn = gsn_axe;           break;
        case(WEAPON_FLAIL):     sn = gsn_flail;         break;
        case(WEAPON_WHIP):      sn = gsn_whip;          break;
        case(WEAPON_POLEARM):   sn = gsn_polearm;       break;
        case(WEAPON_BOW):   	sn = gsn_bow;       	break;
        case(WEAPON_ARROW):   	sn = gsn_arrow;       	break;
        case(WEAPON_LANCE):   	sn = gsn_lance;       	break;
   }
   return sn;
}

int get_weapon_skill(CHAR_DATA *ch, int sn)
{
     int skill;

     /* -1 is exotic */
    if (IS_NPC(ch))
    {
	if (sn == -1)
	    skill = 3 * ch->level;
	else if (sn == gsn_hand_to_hand)
	    skill = 40 + 2 * ch->level;
	else
	    skill = 40 + 5 * ch->level / 2;
    }

    else
    {
	if (sn == -1)
	    skill = UMIN(3 * ch->level,100);
	else
	    skill = ch->pcdata->learned[sn];
    }

    if ( ch->hit < (ch->max_hit * 0.6))
        skill = 9 * skill / 10;

    return URANGE(0,skill,100);
}


/* used to de-screw characters */
void reset_char(CHAR_DATA *ch)
{
     int loc;
     OBJ_DATA *obj, *obj_next;
     AFFECT_DATA *af;

     if (IS_NPC(ch) || ch->in_room == NULL)
	return;

    if (ch->pcdata->true_sex < 0 || ch->pcdata->true_sex > 2)
	ch->pcdata->true_sex = 0;

    ch->sex		= ch->pcdata->true_sex;
    ch->max_hit 	= ch->pcdata->perm_hit;
    ch->max_mana	= ch->pcdata->perm_mana;
    ch->max_move	= ch->pcdata->perm_move;
/*
    ch->hit		= ch->max_hit;
    ch->mana		= ch->max_mana;
    ch->move		= ch->max_move;
*/

/* a little hack */

    ch->extracted = TRUE;
    /* now add back spell effects */
    for (af = ch->affected; af != NULL; af = af->next)
    {
	affect_modify(ch, af, TRUE);
    }

    /* now start adding back the effects */
    for ( obj = ch->carrying; obj != NULL; obj = obj_next)
    {
	obj_next = obj->next_content;
	if ((loc = obj->wear_loc) != WEAR_NONE)
	{
	   obj->wear_loc = WEAR_NONE;
	   equip_char(ch, obj, loc);
	}
    }

    ch->extracted = FALSE;
    /* make sure sex is RIGHT! */
    if (ch->sex < 0 || ch->sex > 2)
	ch->sex = ch->pcdata->true_sex;

}


/*
 * Retrieve a character's trusted level for permission checking.
 */
int get_trust( CHAR_DATA *ch )
{
    if ( ch->desc != NULL && ch->desc->original != NULL )
	ch = ch->desc->original;

    if ( ch->trust != 0 && IS_SET(ch->comm,COMM_TRUE_TRUST))
	return ch->trust;

    if ( IS_NPC(ch) && ch->level >= LEVEL_HERO )
	return LEVEL_HERO - 1;
    else
	return ch->level;
}


/*
 * Retrieve a character's age.
 */
int get_age( CHAR_DATA *ch )
{
    return 17 + ( ch->played + (int) (current_time - ch->logon) ) / 72000;
}

int age_to_num( int age )
{
    return  age * 72000;
}

/* command for retrieving stats */
int get_curr_stat( CHAR_DATA *ch, int stat )
{
    int max;

    if (IS_NPC(ch) || ch->level > LEVEL_IMMORTAL)
	max = 25;

    else
    {
        max = get_max_train(ch,stat);
 	max = UMIN(max,25);
    }

    return URANGE(3,ch->perm_stat[stat] + ch->mod_stat[stat], max);
}


/* command for returning max training score */
int get_max_train( CHAR_DATA *ch, int stat )
{
    int max;

    if (IS_NPC(ch) || ch->level > LEVEL_IMMORTAL)
	return 25;

    max = (20 + pc_race_table[ORG_RACE(ch)].stats[stat] + /* ORG_RACE && RACE serdar*/
		class_table[ch->iclass].stats[stat]);

    return UMIN(max,25);
}

/*
 * command for returning max training score
 * for do_train and stat2train in comm.c
 */
int get_max_train2( CHAR_DATA *ch, int stat )
{
    int max;

    if (IS_NPC(ch) || ch->level > LEVEL_IMMORTAL)
	return 25;

    max = (20 + pc_race_table[ORG_RACE(ch)].stats[stat] +
		class_table[ch->iclass].stats[stat]);

    return UMIN(max,25);
}


/*
 * Retrieve a character's carry capacity.
 */
int can_carry_n( CHAR_DATA *ch )
{
    if ( !IS_NPC(ch) && ch->level >= LEVEL_IMMORTAL )
	return 1000;

    if ( IS_NPC(ch) && IS_SET(ch->act, ACT_PET) )
	return 0;

    return MAX_WEAR + get_curr_stat(ch,STAT_DEX) - 10 + ch->size;
}



/*
 * Retrieve a character's carry capacity.
 */
int can_carry_w( CHAR_DATA *ch )
{
    if ( !IS_NPC(ch) && ch->level >= LEVEL_IMMORTAL )
	return 10000000;

    if ( IS_NPC(ch) && IS_SET(ch->act, ACT_PET) )
	return 0;

    return str_app[get_curr_stat(ch,STAT_STR)].carry * 10 + ch->level * 25;
}



/*
 * See if a string is one of the names of an object.
 */

bool is_name( char *str, char *namelist )
{
    char name[MAX_INPUT_LENGTH], part[MAX_INPUT_LENGTH];
    char *list, *string;


    string = str;
    /* we need ALL parts of string to match part of namelist */
    for ( ; ; )  /* start parsing string */
    {
	str = one_argument(str,part);

	if (part[0] == '\0' )
	    return TRUE;

	/* check to see if this is part of namelist */
	list = namelist;
	for ( ; ; )  /* start parsing namelist */
	{
	    list = one_argument(list,name);
	    if (name[0] == '\0')  /* this name was not found */
		return FALSE;

	    if (!str_prefix(string,name))
		return TRUE; /* full pattern match */

	    if (!str_prefix(part,name))
		break;
	}
    }
}

/* enchanted stuff for eq */
void affect_enchant(OBJ_DATA *obj)
{
    /* okay, move all the old flags into new vectors if we have to */
    if (!obj->enchanted)
    {
        AFFECT_DATA *paf, *af_new;
        obj->enchanted = TRUE;

        for (paf = obj->pIndexData->affected;
             paf != NULL; paf = paf->next)
        {
	    af_new = new_affect();

            af_new->next = obj->affected;
            obj->affected = af_new;

	    af_new->where	= paf->where;
            af_new->type        = UMAX(0,paf->type);
            af_new->level       = paf->level;
            af_new->duration    = paf->duration;
            af_new->location    = paf->location;
            af_new->modifier    = paf->modifier;
            af_new->bitvector   = paf->bitvector;
        }
    }
}


/*
 * Apply or remove an affect to a character.
 */
void affect_modify( CHAR_DATA *ch, AFFECT_DATA *paf, bool fAdd )
{
    OBJ_DATA *hold;
    int mod,i;

    mod = paf->modifier;

    if ( fAdd )
    {
	switch (paf->where)
	{
	case TO_AFFECTS:
	     SET_BIT(ch->affected_by, paf->bitvector);
	     if (IS_SET(paf->bitvector, AFF_FLYING) && !IS_NPC(ch))
		REMOVE_BIT(ch->act,PLR_CHANGED_AFF);
	    break;
	case TO_IMMUNE:
	    SET_BIT(ch->imm_flags,paf->bitvector);
	    break;
	case TO_RESIST:
	    SET_BIT(ch->res_flags,paf->bitvector);
	    break;
	case TO_ACT_FLAG:
	    SET_BIT(ch->act,paf->bitvector);
	    break;
	case TO_VULN:
	    SET_BIT(ch->vuln_flags,paf->bitvector);
	    break;
	case TO_DETECTS:
	    SET_BIT(ch->detection,paf->bitvector);
	    break;
	case TO_RACE:
	    RACE(ch) = paf->modifier < MAX_PC_RACE ? paf->modifier : 1;
	    REMOVE_BIT(ch->affected_by,race_table[ORG_RACE(ch)].det);
	    SET_BIT(ch->affected_by,race_table[RACE(ch)].det);
	    REMOVE_BIT(ch->affected_by,race_table[ORG_RACE(ch)].aff);
	    SET_BIT(ch->affected_by,race_table[RACE(ch)].aff);
	    REMOVE_BIT(ch->imm_flags,race_table[ORG_RACE(ch)].imm);
	    SET_BIT(ch->imm_flags,race_table[RACE(ch)].imm);
	    REMOVE_BIT(ch->res_flags,race_table[ORG_RACE(ch)].res);
	    SET_BIT(ch->res_flags,race_table[RACE(ch)].res);
	    REMOVE_BIT(ch->vuln_flags,race_table[ORG_RACE(ch)].vuln);
	    SET_BIT(ch->vuln_flags,race_table[RACE(ch)].vuln);
	    ch->form	= race_table[RACE(ch)].form;
	    ch->parts	= race_table[RACE(ch)].parts;
	    break;
	}
    }
    else
    {
        switch (paf->where)
        {
        case TO_AFFECTS:
            REMOVE_BIT(ch->affected_by, paf->bitvector);
            break;
        case TO_IMMUNE:
            REMOVE_BIT(ch->imm_flags,paf->bitvector);
            break;
        case TO_RESIST:
            REMOVE_BIT(ch->res_flags,paf->bitvector);
            break;
	case TO_ACT_FLAG:
	    REMOVE_BIT(ch->act,paf->bitvector);
	    break;
        case TO_VULN:
            REMOVE_BIT(ch->vuln_flags,paf->bitvector);
            break;
        case TO_DETECTS:
            REMOVE_BIT(ch->detection,paf->bitvector);
            break;
	case TO_RACE:
	    REMOVE_BIT(ch->affected_by,race_table[RACE(ch)].det);
	    SET_BIT(ch->affected_by,race_table[ORG_RACE(ch)].det);
	    REMOVE_BIT(ch->affected_by,race_table[RACE(ch)].aff);
	    SET_BIT(ch->affected_by,race_table[ORG_RACE(ch)].aff);
	    REMOVE_BIT(ch->imm_flags,race_table[RACE(ch)].imm);
	    SET_BIT(ch->imm_flags,race_table[ORG_RACE(ch)].imm);
	    REMOVE_BIT(ch->res_flags,race_table[RACE(ch)].res);
	    SET_BIT(ch->res_flags,race_table[ORG_RACE(ch)].res);
	    REMOVE_BIT(ch->vuln_flags,race_table[RACE(ch)].vuln);
	    SET_BIT(ch->vuln_flags,race_table[ORG_RACE(ch)].vuln);
	    ch->form	= race_table[ORG_RACE(ch)].form;
	    ch->parts	= race_table[ORG_RACE(ch)].parts;
	    RACE(ch) = ORG_RACE(ch);
	    break;
        }
	mod = 0 - mod;
    }

    switch ( paf->location )
    {
    default:
	bug( "Affect_modify: unknown location %d.", paf->location );
	return;

    case APPLY_NONE:						break;
    case APPLY_STR:           ch->mod_stat[STAT_STR]	+= mod;	break;
    case APPLY_DEX:           ch->mod_stat[STAT_DEX]	+= mod;	break;
    case APPLY_INT:           ch->mod_stat[STAT_INT]	+= mod;	break;
    case APPLY_WIS:           ch->mod_stat[STAT_WIS]	+= mod;	break;
    case APPLY_CON:           ch->mod_stat[STAT_CON]	+= mod;	break;
    case APPLY_CHA:	      ch->mod_stat[STAT_CHA]	+= mod; break;
    case APPLY_CLASS:						break;
    case APPLY_LEVEL:						break;
    case APPLY_AGE:	ch->played += age_to_num( mod );	break;
    case APPLY_HEIGHT:						break;
    case APPLY_WEIGHT:						break;
    case APPLY_MANA:          ch->max_mana		+= mod;	break;
    case APPLY_HIT:           ch->max_hit		+= mod;	break;
    case APPLY_MOVE:          ch->max_move		+= mod;	break;
    case APPLY_GOLD:						break;
    case APPLY_EXP:						break;
    case APPLY_AC:
        for (i = 0; i < 4; i ++)
            ch->armor[i] += mod;
        break;
    case APPLY_HITROLL:       ch->hitroll		+= mod;	break;
    case APPLY_DAMROLL:       ch->damroll		+= mod;	break;
    case APPLY_SIZE:		ch->size		+= mod; break;
    case APPLY_SAVES:   ch->saving_throw		+= mod;	break;
    case APPLY_SAVING_ROD:    ch->saving_throw		+= mod;	break;
    case APPLY_SAVING_PETRI:  ch->saving_throw		+= mod;	break;
    case APPLY_SAVING_BREATH: ch->saving_throw		+= mod;	break;
    case APPLY_SAVING_SPELL:  ch->saving_throw		+= mod;	break;
    case APPLY_SPELL_AFFECT:  					break;
    }

    /*
     * Check for weapon wielding.
     * Guard against recursion (for weapons with affects).
     */
    if ( !IS_NPC(ch) && !ch->extracted)
    {
      static int depth;

      if ( (hold = get_eq_char( ch, WEAR_BOTH ) ) != NULL
      && get_obj_weight(hold) > str_app[get_curr_stat(ch,STAT_STR)].carry )
      {
	if ( depth == 0 )
	{
	    depth++;
      act( "$p objesini býrakýyorsun.", ch, hold, NULL, TO_CHAR );
	    act( "$n $p objesini býrakýyor.", ch, hold, NULL, TO_ROOM );
	    obj_from_char( hold );
	    obj_to_room( hold, ch->in_room );
	    depth--;
	}
      }

      if ( (hold = get_eq_char( ch, WEAR_RIGHT ) ) != NULL
      && get_obj_weight(hold) > str_app[get_curr_stat(ch,STAT_STR)].carry )
      {
	if ( depth == 0 )
	{
	    depth++;
      act("$p objesini býrakýyorsun.", ch, hold, NULL, TO_CHAR );
	    act("$n $p objesini býrakýyor.", ch, hold, NULL, TO_ROOM );
	    obj_from_char( hold );
	    obj_to_room( hold, ch->in_room );
	    depth--;
	}
      }

      if ( (hold = get_eq_char( ch, WEAR_LEFT ) ) != NULL
      && get_obj_weight(hold) > str_app[get_curr_stat(ch,STAT_STR)].carry )
      {
	if ( depth == 0 )
	{
	    depth++;
      act( "$p objesini býrakýyorsun.", ch, hold, NULL, TO_CHAR );
	    act( "$n $p objesini býrakýyor.", ch, hold, NULL, TO_ROOM );
	    obj_from_char( hold );
	    obj_to_room( hold, ch->in_room );
	    depth--;
	}
      }
    }

    return;
}


/* find an effect in an affect list */
AFFECT_DATA  *affect_find(AFFECT_DATA *paf, int sn)
{
    AFFECT_DATA *paf_find;

    for ( paf_find = paf; paf_find != NULL; paf_find = paf_find->next )
    {
        if ( paf_find->type == sn )
	return paf_find;
    }

    return NULL;
}

/* fix object affects when removing one */
void affect_check(CHAR_DATA *ch,int where,int vector)
{
    AFFECT_DATA *paf;
    OBJ_DATA *obj;

    if (where == TO_OBJECT || where == TO_WEAPON || vector == 0)
	return;

    for (paf = ch->affected; paf != NULL; paf = paf->next)
	if (paf->where == where && paf->bitvector == vector)
	{
	    switch (where)
	    {
	        case TO_AFFECTS:
		    SET_BIT(ch->affected_by,vector);
		    break;
	        case TO_IMMUNE:
		    SET_BIT(ch->imm_flags,vector);
		    break;
	        case TO_RESIST:
		    SET_BIT(ch->res_flags,vector);
		    break;
		case TO_ACT_FLAG:
		    SET_BIT(ch->act,paf->bitvector);
		    break;
	        case TO_VULN:
		    SET_BIT(ch->vuln_flags,vector);
		    break;
	        case TO_DETECTS:
		    SET_BIT(ch->detection,vector);
		    break;
		case TO_RACE:
		    if (RACE(ch) == ORG_RACE(ch))
		    {
		     RACE(ch) = paf->modifier<MAX_PC_RACE ? paf->modifier:1;
		     REMOVE_BIT(ch->affected_by,race_table[ORG_RACE(ch)].det);
		     SET_BIT(ch->affected_by,race_table[RACE(ch)].det);
	     	     REMOVE_BIT(ch->affected_by,race_table[ORG_RACE(ch)].aff);
		     SET_BIT(ch->affected_by,race_table[RACE(ch)].aff);
		     REMOVE_BIT(ch->imm_flags,race_table[ORG_RACE(ch)].imm);
		     SET_BIT(ch->imm_flags,race_table[RACE(ch)].imm);
		     REMOVE_BIT(ch->res_flags,race_table[ORG_RACE(ch)].res);
		     SET_BIT(ch->res_flags,race_table[RACE(ch)].res);
		     REMOVE_BIT(ch->vuln_flags,race_table[ORG_RACE(ch)].vuln);
		     SET_BIT(ch->vuln_flags,race_table[RACE(ch)].vuln);
		     ch->form	= race_table[RACE(ch)].form;
		     ch->parts	= race_table[RACE(ch)].parts;
		    }
		    break;
	    }
	    return;
	}

    for (obj = ch->carrying; obj != NULL; obj = obj->next_content)
    {
	if (obj->wear_loc == -1 || obj->wear_loc == WEAR_STUCK_IN)
	    continue;

            for (paf = obj->affected; paf != NULL; paf = paf->next)
            if (paf->where == where && paf->bitvector == vector)
            {
                switch (where)
                {
                    case TO_AFFECTS:
                        SET_BIT(ch->affected_by,vector);
                        break;
                    case TO_IMMUNE:
                        SET_BIT(ch->imm_flags,vector);
                        break;
		    case TO_ACT_FLAG:
		        SET_BIT(ch->act,paf->bitvector);
		        break;
                    case TO_RESIST:
                        SET_BIT(ch->res_flags,vector);
                        break;
                    case TO_VULN:
                        SET_BIT(ch->vuln_flags,vector);
                  	break;
                    case TO_DETECTS:
                        SET_BIT(ch->detection,vector);
                  	break;
		    case TO_RACE:
		    	if (RACE(ch) == ORG_RACE(ch))
		    {
		     RACE(ch) = paf->modifier<MAX_PC_RACE ? paf->modifier:1;
		     REMOVE_BIT(ch->affected_by,race_table[ORG_RACE(ch)].det);
		     SET_BIT(ch->affected_by,race_table[RACE(ch)].det);
	     	     REMOVE_BIT(ch->affected_by,race_table[ORG_RACE(ch)].aff);
		     SET_BIT(ch->affected_by,race_table[RACE(ch)].aff);
		     REMOVE_BIT(ch->imm_flags,race_table[ORG_RACE(ch)].imm);
		     SET_BIT(ch->imm_flags,race_table[RACE(ch)].imm);
		     REMOVE_BIT(ch->res_flags,race_table[ORG_RACE(ch)].res);
		     SET_BIT(ch->res_flags,race_table[RACE(ch)].res);
		     REMOVE_BIT(ch->vuln_flags,race_table[ORG_RACE(ch)].vuln);
		     SET_BIT(ch->vuln_flags,race_table[RACE(ch)].vuln);
		     ch->form	= race_table[RACE(ch)].form;
		     ch->parts	= race_table[RACE(ch)].parts;
		    }
		        break;
                }
                return;
            }

        if (obj->enchanted)
	    continue;

        for (paf = obj->pIndexData->affected; paf != NULL; paf = paf->next)
            if (paf->where == where && paf->bitvector == vector)
            {
                switch (where)
                {
                    case TO_AFFECTS:
                        SET_BIT(ch->affected_by,vector);
	                break;
                    case TO_IMMUNE:
                        SET_BIT(ch->imm_flags,vector);
                        break;
		   case TO_ACT_FLAG:
	                SET_BIT(ch->act,paf->bitvector);
		        break;
                    case TO_RESIST:
                        SET_BIT(ch->res_flags,vector);
                        break;
                    case TO_VULN:
                        SET_BIT(ch->vuln_flags,vector);
                        break;
                    case TO_DETECTS:
                        SET_BIT(ch->detection,vector);
                        break;
		   case TO_RACE:
		    	if (RACE(ch) == ORG_RACE(ch))
		    {
		     RACE(ch) = paf->modifier<MAX_PC_RACE ? paf->modifier:1;
		     REMOVE_BIT(ch->affected_by,race_table[ORG_RACE(ch)].det);
		     SET_BIT(ch->affected_by,race_table[RACE(ch)].det);
	     	     REMOVE_BIT(ch->affected_by,race_table[ORG_RACE(ch)].aff);
		     SET_BIT(ch->affected_by,race_table[RACE(ch)].aff);
		     REMOVE_BIT(ch->imm_flags,race_table[ORG_RACE(ch)].imm);
		     SET_BIT(ch->imm_flags,race_table[RACE(ch)].imm);
		     REMOVE_BIT(ch->res_flags,race_table[ORG_RACE(ch)].res);
		     SET_BIT(ch->res_flags,race_table[RACE(ch)].res);
		     REMOVE_BIT(ch->vuln_flags,race_table[ORG_RACE(ch)].vuln);
		     SET_BIT(ch->vuln_flags,race_table[RACE(ch)].vuln);
		     ch->form	= race_table[RACE(ch)].form;
		     ch->parts	= race_table[RACE(ch)].parts;
		    }
		        break;
                }
                return;
            }
    }
}

/*
 * Give an affect to a char.
 */
void affect_to_char( CHAR_DATA *ch, AFFECT_DATA *paf )
{
    AFFECT_DATA *paf_new;

    paf_new = new_affect();

    *paf_new		= *paf;
    paf_new->next	= ch->affected;
    ch->affected	= paf_new;

    affect_modify( ch, paf_new, TRUE );
    return;
}

/* give an affect to an object */
void affect_to_obj(OBJ_DATA *obj, AFFECT_DATA *paf)
{
    AFFECT_DATA *paf_new;

    paf_new = new_affect();

    *paf_new		= *paf;
    paf_new->next	= obj->affected;
    obj->affected	= paf_new;

    /* apply any affect vectors to the object's extra_flags */
    if (paf->bitvector)
        switch (paf->where)
        {
        case TO_OBJECT:
    	    SET_BIT(obj->extra_flags,paf->bitvector);
	    break;
        case TO_WEAPON:
	    if (obj->item_type == ITEM_WEAPON)
	        SET_BIT(obj->value[4],paf->bitvector);
	    break;
        }


    return;
}



/*
 * Remove an affect from a char.
 */
void affect_remove( CHAR_DATA *ch, AFFECT_DATA *paf )
{
    int where;
    int vector;

    if ( ch->affected == NULL )
    {
	bug( "Affect_remove: no affect.", 0 );
	return;
    }

    affect_modify( ch, paf, FALSE );
    where = paf->where;
    vector = paf->bitvector;

    if ( paf == ch->affected )
    {
	ch->affected	= paf->next;
    }
    else
    {
	AFFECT_DATA *prev;

	for ( prev = ch->affected; prev != NULL; prev = prev->next )
	{
	    if ( prev->next == paf )
	    {
		prev->next = paf->next;
		break;
	    }
	}

	if ( prev == NULL )
	{
	    bug( "Affect_remove: cannot find paf.", 0 );
	    return;
	}
    }

    free_affect(paf);

    affect_check(ch,where,vector);
    return;
}

void affect_remove_obj( OBJ_DATA *obj, AFFECT_DATA *paf)
{
    int where, vector;
    if ( obj->affected == NULL )
    {
        bug( "Affect_remove_object: no affect.", 0 );
        return;
    }

    if (obj->carried_by != NULL && obj->wear_loc != -1)
	affect_modify( obj->carried_by, paf, FALSE );

    where = paf->where;
    vector = paf->bitvector;

    /* remove flags from the object if needed */
    if (paf->bitvector)
	switch( paf->where)
        {
        case TO_OBJECT:
            REMOVE_BIT(obj->extra_flags,paf->bitvector);
            break;
        case TO_WEAPON:
            if (obj->item_type == ITEM_WEAPON)
                REMOVE_BIT(obj->value[4],paf->bitvector);
            break;
        }

    if ( paf == obj->affected )
    {
        obj->affected    = paf->next;
    }
    else
    {
        AFFECT_DATA *prev;

        for ( prev = obj->affected; prev != NULL; prev = prev->next )
        {
            if ( prev->next == paf )
            {
                prev->next = paf->next;
                break;
            }
        }

        if ( prev == NULL )
        {
            bug( "Affect_remove_object: cannot find paf.", 0 );
            return;
        }
    }

    free_affect(paf);

    if (obj->carried_by != NULL && obj->wear_loc != -1)
	affect_check(obj->carried_by,where,vector);
    return;
}



/*
 * Strip all affects of a given sn.
 */
void affect_strip( CHAR_DATA *ch, int sn )
{
    AFFECT_DATA *paf;
    AFFECT_DATA *paf_next;

    for ( paf = ch->affected; paf != NULL; paf = paf_next )
    {
	paf_next = paf->next;
	if ( paf->type == sn )
	    affect_remove( ch, paf );
    }

    return;
}



/*
 * Return true if a char is affected by a spell.
 */
bool is_affected( CHAR_DATA *ch, int sn )
{
    AFFECT_DATA *paf;

    for ( paf = ch->affected; paf != NULL; paf = paf->next )
    {
	if ( paf->type == sn )
	    return TRUE;
    }

    return FALSE;
}



/*
 * Add or enhance an affect.
 */
void affect_join( CHAR_DATA *ch, AFFECT_DATA *paf )
{
    AFFECT_DATA *paf_old;

    for ( paf_old = ch->affected; paf_old != NULL; paf_old = paf_old->next )
    {
	if ( paf_old->type == paf->type )
	{
	    paf->level = (paf->level += paf_old->level) / 2;
	    paf->duration += paf_old->duration;
	    paf->modifier += paf_old->modifier;
	    affect_remove( ch, paf_old );
	    break;
	}
    }

    affect_to_char( ch, paf );
    return;
}



/*
 * Move a char out of a room.
 */
void char_from_room( CHAR_DATA *ch )
{
    OBJ_DATA *obj;
    ROOM_INDEX_DATA *prev_room = ch->in_room;

    if ( ch->in_room == NULL )
    {
	bug( "Char_from_room: NULL.", 0 );
	return;
    }

    if ( !IS_NPC(ch) )
	--ch->in_room->area->nplayer;

    if ( ( obj = get_light_char( ch ) ) != NULL
/*
    &&   obj->item_type == ITEM_LIGHT
    &&   obj->value[2] != 0
*/
    &&   ch->in_room->light > 0 )
	--ch->in_room->light;

    if ( ch == ch->in_room->people )
    {
	ch->in_room->people = ch->next_in_room;
    }
    else
    {
	CHAR_DATA *prev;

	for ( prev = ch->in_room->people; prev; prev = prev->next_in_room )
	{
	    if ( prev->next_in_room == ch )
	    {
		prev->next_in_room = ch->next_in_room;
		break;
	    }
	}

	if ( prev == NULL )
	    bug( "Char_from_room: ch not found.", 0 );
    }

    ch->in_room      = NULL;
    ch->next_in_room = NULL;
    ch->on 	     = NULL;  /* sanity check! */

    if (MOUNTED(ch))
    {
     ch->mount->riding	= FALSE;
     ch->riding		= FALSE;
    }

    if (RIDDEN(ch))
    {
     ch->mount->riding	= FALSE;
     ch->riding		= FALSE;
    }

    if ( prev_room && prev_room->affected_by )
	  raffect_back_char( prev_room, ch);

    return;
}



/*
 * Move a char into a room.
 */
void char_to_room( CHAR_DATA *ch, ROOM_INDEX_DATA *pRoomIndex )
{
    OBJ_DATA *obj;

    if ( pRoomIndex == NULL )
    {
	ROOM_INDEX_DATA *room;

	bug( "Char_to_room: NULL.", 0 );

	if ((room = get_room_index(ROOM_VNUM_TEMPLE)) != NULL)
	    char_to_room(ch,room);

	return;
    }

    ch->in_room		= pRoomIndex;
    ch->next_in_room	= pRoomIndex->people;
    pRoomIndex->people	= ch;

    if ( !IS_NPC(ch) )
    {
	if (ch->in_room->area->empty)
	{
	    ch->in_room->area->empty = FALSE;
	    ch->in_room->area->age = 0;
	}
	++ch->in_room->area->nplayer;
    }

    if ( ( obj = get_light_char( ch ) ) != NULL )
/*
    &&   obj->item_type == ITEM_LIGHT
    &&   obj->value[2] != 0 )
*/
	++ch->in_room->light;

    while (IS_AFFECTED(ch,AFF_PLAGUE))
    {
        AFFECT_DATA *af, plague;
        CHAR_DATA *vch;

        for ( af = ch->affected; af != NULL; af = af->next )
        {
            if (af->type == gsn_plague)
                break;
        }

        if (af == NULL)
        {
            REMOVE_BIT(ch->affected_by,AFF_PLAGUE);
            break;
        }

        if (af->level == 1)
            break;

	plague.where		= TO_AFFECTS;
        plague.type 		= gsn_plague;
        plague.level 		= af->level - 1;
        plague.duration 	= number_range(1,2 * plague.level);
        plague.location		= APPLY_STR;
        plague.modifier 	= -5;
        plague.bitvector 	= AFF_PLAGUE;

        for ( vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
        {
            if (!saves_spell(plague.level - 2,vch,DAM_DISEASE)
	    &&  !IS_IMMORTAL(vch) &&
            	!IS_AFFECTED(vch,AFF_PLAGUE) && number_bits(6) == 0)
            {
              send_to_char("Ateþinin yükseldiðini hissediyorsun.\n\r",vch);
            	act("$n çok hasta görünüyor.",vch,NULL,NULL,TO_ROOM);
            	affect_join(vch,&plague);
            }
        }
	break;
    }

    if ( ch->in_room->affected_by )
	{
	 if (IS_IMMORTAL(ch))
		do_raffects(ch,(char*)"");
	 else raffect_to_char( ch->in_room, ch);
	}

    return;
}



/*
 * Give an obj to a char.
 */
void obj_to_char( OBJ_DATA *obj, CHAR_DATA *ch )
{
    obj->next_content	 = ch->carrying;
    ch->carrying	 = obj;
    obj->carried_by	 = ch;
    obj->in_room	 = NULL;
    obj->in_obj		 = NULL;
    ch->carry_number	+= get_obj_number( obj );
    ch->carry_weight	+= get_obj_weight( obj );
}



/*
 * Take an obj from its character.
 */
void obj_from_char( OBJ_DATA *obj )
{
    CHAR_DATA *ch;

    if ( ( ch = obj->carried_by ) == NULL )
    {
	bug( "Obj_from_char: null ch.", 0 );
	return;
    }

    if ( obj->wear_loc != WEAR_NONE )
	unequip_char( ch, obj );

    if ( ch->carrying == obj )
    {
	ch->carrying = obj->next_content;
    }
    else
    {
	OBJ_DATA *prev;

	for ( prev = ch->carrying; prev != NULL; prev = prev->next_content )
	{
	    if ( prev->next_content == obj )
	    {
		prev->next_content = obj->next_content;
		break;
	    }
	}

	if ( prev == NULL )
	    bug( "Obj_from_char: obj not in list.", 0 );
    }

    obj->carried_by	 = NULL;
    obj->next_content	 = NULL;
    ch->carry_number	-= get_obj_number( obj );
    ch->carry_weight	-= get_obj_weight( obj );
    return;
}



/*
 * Find the ac value of an obj, including position effect.
 */
int apply_ac( OBJ_DATA *obj, int iWear, int type )
{
    if ( obj->item_type != ITEM_ARMOR )
	return 0;

    switch ( iWear )
    {
    case WEAR_BODY:	return 3 * obj->value[type];
    case WEAR_HEAD:	return 2 * obj->value[type];
    case WEAR_LEGS:	return 2 * obj->value[type];
    case WEAR_FEET:	return     obj->value[type];
    case WEAR_HANDS:	return     obj->value[type];
    case WEAR_ARMS:	return     obj->value[type];
    case WEAR_FINGER:   return     obj->value[type];
    case WEAR_NECK:	return     obj->value[type];
    case WEAR_ABOUT:	return 2 * obj->value[type];
    case WEAR_WAIST:	return     obj->value[type];
    case WEAR_WRIST:	return     obj->value[type];
    case WEAR_LEFT:	return     obj->value[type];
    case WEAR_RIGHT:	return     obj->value[type];
    case WEAR_BOTH:	return     obj->value[type];
    }

    return 0;
}



/*
 * Find a piece of eq on a character.
 */
OBJ_DATA *get_eq_char( CHAR_DATA *ch, int iWear )
{
    OBJ_DATA *obj;

    if (ch == NULL)
	return NULL;

    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
	if ( obj->wear_loc == iWear )
	    return obj;
    }

    return NULL;
}



/*
 * Equip a char with an obj.
 */
void equip_char( CHAR_DATA *ch, OBJ_DATA *obj, int iWear )
{
    AFFECT_DATA *paf;
    int i;

    if (iWear == WEAR_STUCK_IN)
    {
	obj->wear_loc = iWear;
	return;
    }

    if ( count_worn( ch, iWear ) >= max_can_wear(ch, iWear) )
    {
	bug( "Equip_char: already equipped (%d).", iWear );
	return;
    }

    if ( ( IS_OBJ_STAT(obj, ITEM_ANTI_EVIL)    && IS_EVIL(ch)    )
    ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_GOOD)    && IS_GOOD(ch)    )
    ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_NEUTRAL) && IS_NEUTRAL(ch) ) )
    {
	/*
	 * Thanks to Morgenes for the bug fix here!
	 */
   act("$p seni çarparak elinden düþüyor.", ch, obj, NULL, TO_CHAR );
 	act("$p $m çarparak elinden düþüyor.",  ch, obj, NULL, TO_ROOM );
	obj_from_char( obj );
	obj_to_room( obj, ch->in_room );
	return;
    }


    for (i = 0; i < 4; i++)
    	ch->armor[i]      	-= apply_ac( obj, iWear,i );

    if ( get_light_char(ch) == NULL && ch->in_room != NULL
	&& ( (obj->item_type == ITEM_LIGHT && obj->value[2] != 0)
            || (iWear == WEAR_HEAD && IS_OBJ_STAT(obj, ITEM_GLOW)) ) )
	++ch->in_room->light;

    obj->wear_loc	 = iWear;

    if (!obj->enchanted)
	for ( paf = obj->pIndexData->affected; paf != NULL; paf = paf->next )
	    if ( paf->location != APPLY_SPELL_AFFECT )
	        affect_modify( ch, paf, TRUE );
    for ( paf = obj->affected; paf != NULL; paf = paf->next )
	if ( paf->location == APPLY_SPELL_AFFECT )
    	    affect_to_char ( ch, paf );
	else
	    affect_modify( ch, paf, TRUE );

    if (IS_SET(obj->progtypes,OPROG_WEAR))
      (obj->pIndexData->oprogs->wear_prog) (obj,ch);

    return;
}



/*
 * Unequip a char with an obj.
 */
void unequip_char( CHAR_DATA *ch, OBJ_DATA *obj )
{
    AFFECT_DATA *paf = NULL;
    AFFECT_DATA *lpaf = NULL;
    AFFECT_DATA *lpaf_next = NULL;
    int i, old_wear;

    if ( obj->wear_loc == WEAR_NONE )
    {
	bug( "Unequip_char: already unequipped.", 0 );
	return;
    }

    if ( obj->wear_loc == WEAR_STUCK_IN )
    {
	obj->wear_loc = WEAR_NONE;
	return;
    }

    for (i = 0; i < 4; i++)
    	ch->armor[i]	+= apply_ac( obj, obj->wear_loc,i );
    old_wear 		 = obj->wear_loc;
    obj->wear_loc	 = -1;

    if (!obj->enchanted)
	for ( paf = obj->pIndexData->affected; paf != NULL; paf = paf->next )
	    if ( paf->location == APPLY_SPELL_AFFECT )
	    {
	        for ( lpaf = ch->affected; lpaf != NULL; lpaf = lpaf_next )
	        {
		    lpaf_next = lpaf->next;
		    if ((lpaf->type == paf->type) &&
		        (lpaf->level == paf->level) &&
		        (lpaf->location == APPLY_SPELL_AFFECT))
		    {
		        affect_remove( ch, lpaf );
			lpaf_next = NULL;
		    }
	        }
	    }
	    else
	    {
	        affect_modify( ch, paf, FALSE );
		affect_check(ch,paf->where,paf->bitvector);
	    }

    for ( paf = obj->affected; paf != NULL; paf = paf->next )
	if ( paf->location == APPLY_SPELL_AFFECT )
	{
	    bug ( "Norm-Apply: %d", 0 );
	    for ( lpaf = ch->affected; lpaf != NULL; lpaf = lpaf_next )
	    {
		lpaf_next = lpaf->next;
		if ((lpaf->type == paf->type) &&
		    (lpaf->level == paf->level) &&
		    (lpaf->location == APPLY_SPELL_AFFECT))
		{
		    bug ( "location = %d", lpaf->location );
		    bug ( "type = %d", lpaf->type );
		    affect_remove( ch, lpaf );
		    lpaf_next = NULL;
		}
	    }
	}
	else
	{
	    affect_modify( ch, paf, FALSE );
	    affect_check(ch,paf->where,paf->bitvector);
	}

    if ( get_light_char(ch) == NULL && ch->in_room != NULL
	&& ( (obj->item_type == ITEM_LIGHT && obj->value[2] != 0)
            || (old_wear == WEAR_HEAD && IS_OBJ_STAT(obj, ITEM_GLOW)) )
	&&   ch->in_room->light > 0 )
	--ch->in_room->light;

    if (IS_SET(obj->progtypes,OPROG_REMOVE))
      (obj->pIndexData->oprogs->remove_prog) (obj,ch);

    return;
}



/*
 * Count occurrences of an obj in a list.
 */
int count_obj_list( OBJ_INDEX_DATA *pObjIndex, OBJ_DATA *list )
{
    OBJ_DATA *obj;
    int nMatch;

    nMatch = 0;
    for ( obj = list; obj != NULL; obj = obj->next_content )
    {
	if ( obj->pIndexData == pObjIndex )
	    nMatch++;
    }

    return nMatch;
}



/*
 * Move an obj out of a room.
 */
void obj_from_room( OBJ_DATA *obj )
{
    ROOM_INDEX_DATA *in_room;
    CHAR_DATA *ch;

    if ( ( in_room = obj->in_room ) == NULL )
    {
	bug( "obj_from_room: NULL.", 0 );
	return;
    }

    for (ch = in_room->people; ch != NULL; ch = ch->next_in_room)
	if (ch->on == obj)
	    ch->on = NULL;

    if ( obj == in_room->contents )
    {
	in_room->contents = obj->next_content;
    }
    else
    {
	OBJ_DATA *prev;

	for ( prev = in_room->contents; prev; prev = prev->next_content )
	{
	    if ( prev->next_content == obj )
	    {
		prev->next_content = obj->next_content;
		break;
	    }
	}

	if ( prev == NULL )
	{
	    bug( "Obj_from_room: obj not found.", 0 );
	    return;
	}
    }

    obj->in_room      = NULL;
    obj->next_content = NULL;
    return;
}



/*
 * Move an obj into a room.
 */
void obj_to_room( OBJ_DATA *obj, ROOM_INDEX_DATA *pRoomIndex )
{
    int i;

    obj->next_content		= pRoomIndex->contents;
    pRoomIndex->contents	= obj;
    obj->in_room		= pRoomIndex;
    obj->carried_by		= NULL;
    obj->in_obj			= NULL;

    if ( IS_WATER(pRoomIndex) )
    {
	if ( may_float( obj ) )
	  obj->water_float = -1;
	else
	  obj->water_float = floating_time( obj );
	  }

    if (obj->pIndexData->vnum < 600)
    {
      for (i=1;i < MAX_CABAL; i++)
	if (cabal_table[i].obj_vnum == obj->pIndexData->vnum)
	   break;

      if (i < MAX_CABAL)
      {
        for (i=1;i < MAX_CABAL; i++)
	  if (cabal_table[i].room_vnum == pRoomIndex->vnum)
	    break;

        if (i < MAX_CABAL)
        {
	  obj->timer = -1;
	  if (pRoomIndex->people)
	  {
      act("$p saydamlýðýný yitirerek katýlaþýyor.",
		pRoomIndex->people, obj, NULL, TO_CHAR);
	    act("$p saydamlýðýný yitirerek katýlaþýyor.",
		pRoomIndex->people, obj, NULL, TO_ROOM);
	  }
        }
      }
    }
    return;
}



/*
 * Move an object into an object.
 */
void obj_to_obj( OBJ_DATA *obj, OBJ_DATA *obj_to )
{

    obj->next_content		= obj_to->contains;
    obj_to->contains		= obj;
    obj->in_obj			= obj_to;
    obj->in_room		= NULL;
    obj->carried_by		= NULL;
    if (obj_to->pIndexData->vnum == OBJ_VNUM_PIT)
        obj->cost = 0;

    for ( ; obj_to != NULL; obj_to = obj_to->in_obj )
    {
	if ( obj_to->carried_by != NULL )
	{
/*	    obj_to->carried_by->carry_number += get_obj_number( obj ); */
	    obj_to->carried_by->carry_weight += get_obj_weight( obj )
		* WEIGHT_MULT(obj_to) / 100;
	}
    }

    return;
}



/*
 * Move an object out of an object.
 */
void obj_from_obj( OBJ_DATA *obj )
{
    OBJ_DATA *obj_from;

    if ( ( obj_from = obj->in_obj ) == NULL )
    {
	bug( "Obj_from_obj: null obj_from.", 0 );
	return;
    }

    if ( obj == obj_from->contains )
    {
	obj_from->contains = obj->next_content;
    }
    else
    {
	OBJ_DATA *prev;

	for ( prev = obj_from->contains; prev; prev = prev->next_content )
	{
	    if ( prev->next_content == obj )
	    {
		prev->next_content = obj->next_content;
		break;
	    }
	}

	if ( prev == NULL )
	{
	    bug( "Obj_from_obj: obj not found.", 0 );
	    return;
	}
    }

    obj->next_content = NULL;
    obj->in_obj       = NULL;

    for ( ; obj_from != NULL; obj_from = obj_from->in_obj )
    {
	if ( obj_from->carried_by != NULL )
	{
/*	    obj_from->carried_by->carry_number -= get_obj_number( obj ); */
	    obj_from->carried_by->carry_weight -= get_obj_weight( obj )
		* WEIGHT_MULT(obj_from) / 100;
	}
    }

    return;
}

/*
 * Extract an object consider limit
 */
void extract_obj( OBJ_DATA *obj )
{
  extract_obj_1(obj,TRUE);
}

/*
 * Extract an object consider limit
 */
void extract_obj_nocount( OBJ_DATA *obj )
{
  extract_obj_1(obj,FALSE);
}

/*
 * Extract an obj from the world.
 */
void extract_obj_1( OBJ_DATA *obj, bool count )
{
    OBJ_DATA *obj_content;
    OBJ_DATA *obj_next;
    int i;
    char buf[MAX_STRING_LENGTH];

    if (obj->extracted)  /* if the object has already been extracted once */
      {
        sprintf(buf, "Warning! Extraction of %s, vnum %d.", obj->name,
                obj->pIndexData->vnum);
        bug(buf, 0);
        return; /* if it's already been extracted, something bad is going on */
      }
    else
      obj->extracted = TRUE;  /* if it hasn't been extracted yet, now
                               * it's being extracted. */

    if ( obj->in_room != NULL )
	obj_from_room( obj );
    else if ( obj->carried_by != NULL )
	obj_from_char( obj );
    else if ( obj->in_obj != NULL )
	obj_from_obj( obj );

    for (i=1;i < MAX_CABAL;i++)
      if (obj->pIndexData->vnum == cabal_table[i].obj_vnum
	&& cabal_table[i].obj_ptr == obj )
      {
        obj->pIndexData->count--;
        cabal_table[i].obj_ptr = NULL;
      }

    for ( obj_content = obj->contains; obj_content; obj_content = obj_next )
    {
	obj_next = obj_content->next_content;
	extract_obj_1( obj_content, count );
    }

    if (obj->pIndexData->vnum == OBJ_VNUM_MAGIC_JAR )
	{
	 CHAR_DATA *wch;

	 for ( wch = char_list; wch != NULL ; wch = wch->next )
	  {
	   if (IS_NPC(wch)) continue;
	   if (is_name(obj->name,wch->name) )
		{
		 REMOVE_BIT(wch->act,PLR_NO_EXP);
     send_to_char("Ruhunu yakalýyorsun...\n\r",wch);
		 break;
		}
	  }
	}
    if ( object_list == obj )
    {
	object_list = obj->next;
    }
    else
    {
	OBJ_DATA *prev;

	for ( prev = object_list; prev != NULL; prev = prev->next )
	{
	    if ( prev->next == obj )
	    {
		prev->next = obj->next;
		break;
	    }
	}

	if ( prev == NULL )
	{
	    bug( "Extract_obj: obj %d not found.", obj->pIndexData->vnum );
	    return;
	}
    }
    if (count)
      --obj->pIndexData->count;
    free_obj(obj);
    return;
}

void extract_char( CHAR_DATA *ch, bool fPull )
{
  extract_char_org( ch, fPull, TRUE );
  return;
}

void extract_char_nocount( CHAR_DATA *ch, bool fPull )
{
  extract_char_org( ch, fPull, FALSE );
  return;
}


/*
 * Extract a char from the world.
 */
void extract_char_org( CHAR_DATA *ch, bool fPull, bool Count )
{
    CHAR_DATA *wch;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    int i;
    char buf[MAX_STRING_LENGTH];

    if (fPull) /* only for total extractions should it check */
    {
    if (ch->extracted)  /* if the char has already been extracted once */
      {
        sprintf(buf, "Warning! Extraction of %s.", ch->name);
        bug(buf, 0);
        return; /* if it's already been extracted, something bad is going on */
      }
    else
      ch->extracted = TRUE;  /* if it hasn't been extracted yet, now
                               * it's being extracted. */
    }


    if ( ch->in_room == NULL )
    {
	bug( "Extract_char: NULL.", 0 );
	return;
    }

    nuke_pets(ch);
    ch->pet = NULL; /* just in case */

    if ( fPull )

	die_follower( ch );

    stop_fighting( ch, TRUE );

    char_from_room( ch );
    char_to_room( ch, get_room_index( ROOM_VNUM_LIMBO ) );

    for ( obj = ch->carrying; obj != NULL; obj = obj_next )
    {
	obj_next = obj->next_content;
	if ( Count )
  	  extract_obj( obj );
	else
	  extract_obj_nocount( obj );
    }

    char_from_room( ch );

    if ( !fPull )
    {
        if (IS_GOOD(ch))
          i = 0;
        if (IS_EVIL(ch))
          i = 2;
        else
          i = 1;
        char_to_room(ch, get_room_index(hometown_table[ch->hometown].altar[i]));
	return;
    }

    if ( IS_NPC(ch) )
	--ch->pIndexData->count;

    if ( ch->desc != NULL && ch->desc->original != NULL )
    {
	do_return( ch, (char*)"" );
	ch->desc = NULL;
    }

    for ( wch = char_list; wch != NULL; wch = wch->next )
    {
	if ( wch->reply == ch )
	    wch->reply = NULL;
    }

    if ( ch == char_list )
    {
       char_list = ch->next;
    }
    else
    {
	CHAR_DATA *prev;

	for ( prev = char_list; prev != NULL; prev = prev->next )
	{
	    if ( prev->next == ch )
	    {
		prev->next = ch->next;
		break;
	    }
	}

	if ( prev == NULL )
	{
	    bug( "Extract_char: char not found.", 0 );
	    return;
	}
    }

    if ( ch->desc != NULL )
	ch->desc->character = NULL;
    free_char( ch );
    return;
}



/*
 * Find a char in the room.
 */
CHAR_DATA *get_char_room( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *rch;
    int number;
    int count;
    int ugly;

    number = number_argument( argument, arg );
    count  = 0;
    ugly   = 0;
    if ( !str_cmp( arg, "self" ) )
	return ch;
    if ( !str_cmp( arg, "ugly" ) )
	ugly = 1;

    for ( rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room )
    {
	if ( !can_see( ch, rch ) )
	    continue;

	if ( ugly && (count + 1) == number && IS_VAMPIRE(rch) )
	   return rch;

        if ( (is_affected(rch,gsn_doppelganger)
              && !IS_SET(ch->act,PLR_HOLYLIGHT))?
            !is_name(arg,rch->doppel->name):!is_name(arg,rch->name) )
          continue;

	if ( ++count == number )
	    return rch;
    }

    return NULL;
}



/*
 * Find a char in the room.
 * Chronos uses in act_move.c
 */
CHAR_DATA *get_char_room2( CHAR_DATA *ch, ROOM_INDEX_DATA *room, char *argument, int *number )
{
    CHAR_DATA *rch;
    int count;
    int ugly;

    if (room == NULL ) return NULL;
    count  = 0;
    ugly   = 0;

    if ( !str_cmp( argument, "ugly" ) )
	ugly = 1;

    for ( rch = room->people; rch != NULL; rch = rch->next_in_room )
    {
	if ( !can_see( ch, rch ) )
	    continue;

	if ( ugly && (count + 1) == *number && IS_VAMPIRE(rch) )
	   return rch;

        if ( (is_affected(rch,gsn_doppelganger)
              && !IS_SET(ch->act,PLR_HOLYLIGHT))?
	!is_name(argument,rch->doppel->name):!is_name(argument,rch->name) )
          continue;

	if ( ++count == *number )
	    return rch;
    }

    *number -= count;
    return NULL;
}


/*
 * Find a char in the world.
 */
CHAR_DATA *get_char_world( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *wch;
    int number;
    int count;

    if ( ( wch = get_char_room( ch, argument ) ) != NULL )
	return wch;

    number = number_argument( argument, arg );
    count  = 0;
    for ( wch = char_list; wch != NULL ; wch = wch->next )
    {
	if ( wch->in_room == NULL || !can_see( ch, wch )
	||   !is_name( arg, wch->name ) )
	    continue;

	if ( ++count == number )
	    return wch;
    }

    return NULL;
}



/*
 * Find some object with a given index data.
 * Used by area-reset 'P' command.
 */
OBJ_DATA *get_obj_type( OBJ_INDEX_DATA *pObjIndex )
{
    OBJ_DATA *obj;

    for ( obj = object_list; obj != NULL; obj = obj->next )
    {
	if ( obj->pIndexData == pObjIndex )
	    return obj;
    }

    return NULL;
}


/*
 * Find an obj in a list.
 */
OBJ_DATA *get_obj_list( CHAR_DATA *ch, char *argument, OBJ_DATA *list )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int number;
    int count;

    number = number_argument( argument, arg );
    count  = 0;
    for ( obj = list; obj != NULL; obj = obj->next_content )
    {
	if ( can_see_obj( ch, obj ) && is_name( arg, obj->name ) )
	{
	    if ( ++count == number )
		return obj;
	}
    }

    return NULL;
}



/*
 * Find an obj in player's inventory.
 */
OBJ_DATA *get_obj_carry( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int number;
    int count;

    number = number_argument( argument, arg );
    count  = 0;
    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
	if ( obj->wear_loc == WEAR_NONE
	&&   (can_see_obj( ch, obj ) )
	&&   is_name( arg, obj->name ) )
	{
	    if ( ++count == number )
		return obj;
	}
    }

    return NULL;
}



/*
 * Find an obj in player's equipment.
 */
OBJ_DATA *get_obj_wear( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int number;
    int count;

    number = number_argument( argument, arg );
    count  = 0;
    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
	if ( obj->wear_loc != WEAR_NONE
	&&   can_see_obj( ch, obj )
	&&   is_name( arg, obj->name ) )
	{
	    if ( ++count == number )
		return obj;
	}
    }

    return NULL;
}



/*
 * Find an obj in the room or in inventory.
 */
OBJ_DATA *get_obj_here( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;

    obj = get_obj_list( ch, argument, ch->in_room->contents );
    if ( obj != NULL )
	return obj;

    if ( ( obj = get_obj_carry( ch, argument ) ) != NULL )
	return obj;

    if ( ( obj = get_obj_wear( ch, argument ) ) != NULL )
	return obj;

    return NULL;
}



/*
 * Find an obj in the world.
 */
OBJ_DATA *get_obj_world( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int number;
    int count;

    if ( ( obj = get_obj_here( ch, argument ) ) != NULL )
	return obj;

    number = number_argument( argument, arg );
    count  = 0;
    for ( obj = object_list; obj != NULL; obj = obj->next )
    {
	if ( can_see_obj( ch, obj ) && is_name( arg, obj->name ) )
	{
	    if ( ++count == number )
		return obj;
	}

    }

    return NULL;
}

/* deduct cost from a character */

void deduct_cost(CHAR_DATA *ch, int cost)
{
    int silver = 0, gold = 0;

    silver = UMIN(ch->silver,cost);

    if (silver < cost)
    {
	gold = ((cost - silver + 99) / 100);
	silver = cost - 100 * gold;
    }

    ch->gold -= gold;
    ch->silver -= silver;

    if (ch->gold < 0)
    {
	bug("deduct costs: gold %d < 0",ch->gold);
	ch->gold = 0;
    }
    if (ch->silver < 0)
    {
	bug("deduct costs: silver %d < 0",ch->silver);
	ch->silver = 0;
    }
}
/*
 * Create a 'money' obj.
 */
OBJ_DATA *create_money( int gold, int silver )
{
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;

    if ( gold < 0 || silver < 0 || (gold == 0 && silver == 0) )
    {
	bug( "Create_money: zero or negative money.",UMIN(gold,silver));
	gold = UMAX(1,gold);
	silver = UMAX(1,silver);
    }

    if (gold == 0 && silver == 1)
    {
	obj = create_object( get_obj_index( OBJ_VNUM_SILVER_ONE ), 0 );
    }
    else if (gold == 1 && silver == 0)
    {
	obj = create_object( get_obj_index( OBJ_VNUM_GOLD_ONE), 0 );
    }
    else if (silver == 0)
    {
        obj = create_object( get_obj_index( OBJ_VNUM_GOLD_SOME ), 0 );
        sprintf( buf, obj->short_descr, gold );
        free_string( obj->short_descr );
        obj->short_descr        = str_dup( buf );
        obj->value[1]           = gold;
        obj->cost               = gold;
	obj->weight		= gold/5;
    }
    else if (gold == 0)
    {
        obj = create_object( get_obj_index( OBJ_VNUM_SILVER_SOME ), 0 );
        sprintf( buf, obj->short_descr, silver );
        free_string( obj->short_descr );
        obj->short_descr        = str_dup( buf );
        obj->value[0]           = silver;
        obj->cost               = silver;
	obj->weight		= silver/20;
    }

    else
    {
	obj = create_object( get_obj_index( OBJ_VNUM_COINS ), 0 );
	sprintf( buf, obj->short_descr, silver, gold );
	free_string( obj->short_descr );
	obj->short_descr	= str_dup( buf );
	obj->value[0]		= silver;
	obj->value[1]		= gold;
	obj->cost		= 100 * gold + silver;
	obj->weight		= gold / 5 + silver / 20;
    }

    return obj;
}


/*
 * Return # of objects which an object counts as.
 * Thanks to Tony Chamberlain for the correct recursive code here.
 */
int get_obj_number( OBJ_DATA *obj )
{
    int number;
/*
    if (obj->item_type == ITEM_CONTAINER || obj->item_type == ITEM_MONEY
    ||  obj->item_type == ITEM_GEM || obj->item_type == ITEM_JEWELRY)
        number = 0;
*/
    if ( obj->item_type == ITEM_MONEY )
	number = 0;
    else
        number = 1;

/*
    for ( obj = obj->contains; obj != NULL; obj = obj->next_content )
        number += get_obj_number( obj );
*/
    return number;
}

int get_obj_realnumber( OBJ_DATA *obj )
{
    int number = 1;

    for ( obj = obj->contains; obj != NULL; obj = obj->next_content )
        number += get_obj_number( obj );

    return number;
}

/*
 * Return weight of an object, including weight of contents.
 */
int get_obj_weight( OBJ_DATA *obj )
{
    int weight;
    OBJ_DATA *tobj;

    weight = obj->weight;
    for ( tobj = obj->contains; tobj != NULL; tobj = tobj->next_content )
	weight += get_obj_weight( tobj ) * WEIGHT_MULT(obj) / 100;

    return weight;
}

int get_true_weight(OBJ_DATA *obj)
{
    int weight;

    weight = obj->weight;
    for ( obj = obj->contains; obj != NULL; obj = obj->next_content )
        weight += get_obj_weight( obj );

    return weight;
}

/*
 * True if room is dark.
 */
bool room_is_dark( CHAR_DATA *ch )
{
    ROOM_INDEX_DATA * pRoomIndex = ch->in_room;

    if ( IS_VAMPIRE (ch) )
	{
         return FALSE;
	}

    if ( pRoomIndex->light > 0 )
	return FALSE;

    if ( IS_SET(pRoomIndex->room_flags, ROOM_DARK) )
	return TRUE;

    if ( pRoomIndex->sector_type == SECT_INSIDE
    ||   pRoomIndex->sector_type == SECT_CITY )
	return FALSE;

    if ( weather_info.sunlight == SUN_SET
    	   || weather_info.sunlight == SUN_DARK )
	return TRUE;

    return FALSE;
}

bool room_dark( ROOM_INDEX_DATA *pRoomIndex )
{
    if ( pRoomIndex->light > 0 )
	return FALSE;

    if ( IS_SET(pRoomIndex->room_flags, ROOM_DARK) )
	return TRUE;

    if ( pRoomIndex->sector_type == SECT_INSIDE
    ||   pRoomIndex->sector_type == SECT_CITY )
	return FALSE;

    if ( weather_info.sunlight == SUN_SET
    	   || weather_info.sunlight == SUN_DARK )
	return TRUE;

    return FALSE;
}


bool is_room_owner(CHAR_DATA *ch, ROOM_INDEX_DATA *room)
{
    if (room->owner == NULL || room->owner[0] == '\0')
	return FALSE;

    return is_name(ch->name,room->owner);
}

/*
 * True if room is private.
 */
bool room_is_private( ROOM_INDEX_DATA *pRoomIndex )
{
    CHAR_DATA *rch;
    int count;

/*
    if (pRoomIndex->owner != NULL && pRoomIndex->owner[0] != '\0')
	return TRUE;
*/
    count = 0;
    for ( rch = pRoomIndex->people; rch != NULL; rch = rch->next_in_room )
	count++;

    if ( IS_SET(pRoomIndex->room_flags, ROOM_PRIVATE)  && count >= 2 )
	return TRUE;

    if ( IS_SET(pRoomIndex->room_flags, ROOM_SOLITARY) && count >= 1 )
	return TRUE;

    if ( IS_SET(pRoomIndex->room_flags, ROOM_IMP_ONLY) )
	return TRUE;

    return FALSE;
}

/* visibility on a room -- for entering and exits */
bool can_see_room( CHAR_DATA *ch, ROOM_INDEX_DATA *pRoomIndex )
{
    if (IS_SET(pRoomIndex->room_flags, ROOM_IMP_ONLY)
    &&  get_trust(ch) < MAX_LEVEL)
	return FALSE;

    if (IS_SET(pRoomIndex->room_flags, ROOM_GODS_ONLY)
    &&  !IS_IMMORTAL(ch))
	return FALSE;

    if (IS_SET(pRoomIndex->room_flags, ROOM_HEROES_ONLY)
    &&  !IS_IMMORTAL(ch))
	return FALSE;

    if (IS_SET(pRoomIndex->room_flags,ROOM_NEWBIES_ONLY)
    &&  ch->level > 5 && !IS_IMMORTAL(ch))
	return FALSE;

    return TRUE;
}



/*
 * True if char can see victim.
 */
bool can_see( CHAR_DATA *ch, CHAR_DATA *victim )
{
/* RT changed so that WIZ_INVIS has levels */
    if ( ch == victim )
	return TRUE;

    if ( ch == NULL || victim == NULL )
	dump_to_scr( (char*)">>>>>>>> CAN_ SEE ERROR <<<<<<<<<<<\n\r" );

    if ( get_trust(ch) < victim->invis_level)
	return FALSE;


    if (get_trust(ch) < victim->incog_level && ch->in_room != victim->in_room)
	return FALSE;

    if ( (!IS_NPC(ch) && IS_SET(ch->act, PLR_HOLYLIGHT))
    ||   (IS_NPC(ch) && IS_IMMORTAL(ch)))
	return TRUE;

    if ( IS_AFFECTED(ch, AFF_BLIND) )
	return FALSE;

    if ( ch->in_room == NULL )
        return FALSE;

    if ( room_is_dark( ch ) && !IS_AFFECTED(ch, AFF_INFRARED) )
	return FALSE;

    if ( IS_AFFECTED(victim, AFF_INVISIBLE)
    &&   !CAN_DETECT(ch, DETECT_INVIS) )
	return FALSE;

    if ( IS_AFFECTED(victim, AFF_IMP_INVIS)
    &&   !CAN_DETECT(ch, DETECT_IMP_INVIS) )
	return FALSE;

/* sneaking

    if ( IS_AFFECTED(victim, AFF_SNEAK)
    &&   !CAN_DETECT(ch,DETECT_HIDDEN)
    &&   victim->fighting == NULL)
    {
	int chance;
	chance = get_skill(victim,gsn_sneak);
	chance += get_curr_stat(ch,STAT_DEX) * 3/2;
 	chance -= get_curr_stat(ch,STAT_INT) * 2;
	chance += ch->level - victim->level * 3/2;

	if (number_percent() < chance)
	    return FALSE;
    }
*/

    if (IS_AFFECTED(victim,AFF_CAMOUFLAGE) &&
        !CAN_DETECT(ch,ACUTE_VISION))
      return FALSE;

    if ( IS_AFFECTED(victim, AFF_HIDE)
    &&   !CAN_DETECT(ch, DETECT_HIDDEN)
    &&   victim->fighting == NULL)
	return FALSE;

    if ( IS_AFFECTED(victim, AFF_FADE)
    &&   !CAN_DETECT(ch, DETECT_FADE)
    &&   victim->fighting == NULL)
	return FALSE;

    if ( CAN_DETECT(victim, ADET_EARTHFADE))
	return FALSE;
    return TRUE;
}



/*
 * True if char can see obj.
 */
bool can_see_obj( CHAR_DATA *ch, OBJ_DATA *obj )
{
    if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_HOLYLIGHT) )
	return TRUE;

    if ( IS_SET(obj->extra_flags,ITEM_VIS_DEATH))
	return FALSE;

    if ( IS_AFFECTED( ch, AFF_BLIND ) && obj->item_type != ITEM_POTION)
	return FALSE;

    if ( obj->item_type == ITEM_LIGHT && obj->value[2] != 0 )
	return TRUE;

    if ( IS_SET(obj->extra_flags, ITEM_INVIS)
    &&   !CAN_DETECT(ch, DETECT_INVIS) )
        return FALSE;

    if ( IS_SET(obj->extra_flags, ITEM_BURIED) && !IS_IMMORTAL(ch) )
        return FALSE;

    if ( IS_OBJ_STAT(obj,ITEM_GLOW))
	return TRUE;

    if ( room_is_dark( ch ) && !IS_AFFECTED(ch, AFF_INFRARED) )
	return FALSE;

    if ( obj->item_type == ITEM_TATTOO )
	return TRUE;

    return TRUE;
}



/*
 * True if char can drop obj.
 */
bool can_drop_obj( CHAR_DATA *ch, OBJ_DATA *obj )
{
    if ( !IS_SET(obj->extra_flags, ITEM_NODROP) )
	return TRUE;

    if ( !IS_NPC(ch) && ch->level >= LEVEL_IMMORTAL )
	return TRUE;

    return FALSE;
}



/*
 * Return ascii name of an item type.
 */
char *item_type_name( OBJ_DATA *obj )
{
    switch ( obj->item_type )
    {
    case ITEM_LIGHT:		return (char*)"light";
    case ITEM_SCROLL:		return (char*)"scroll";
    case ITEM_WAND:		return (char*)"wand";
    case ITEM_STAFF:		return (char*)"staff";
    case ITEM_WEAPON:		return (char*)"weapon";
    case ITEM_TREASURE:		return (char*)"treasure";
    case ITEM_ARMOR:		return (char*)"armor";
    case ITEM_CLOTHING:		return (char*)"clothing";
    case ITEM_POTION:		return (char*)"potion";
    case ITEM_FURNITURE:	return (char*)"furniture";
    case ITEM_TRASH:		return (char*)"trash";
    case ITEM_CONTAINER:	return (char*)"container";
    case ITEM_DRINK_CON:	return (char*)"drink container";
    case ITEM_KEY:		return (char*)"key";
    case ITEM_FOOD:		return (char*)"food";
    case ITEM_MONEY:		return (char*)"money";
    case ITEM_BOAT:		return (char*)"boat";
    case ITEM_CORPSE_NPC:	return (char*)"npc corpse";
    case ITEM_CORPSE_PC:	return (char*)"pc corpse";
    case ITEM_FOUNTAIN:		return (char*)"fountain";
    case ITEM_PILL:		return (char*)"pill";
    case ITEM_MAP:		return (char*)"map";
    case ITEM_PORTAL:		return (char*)"portal";
    case ITEM_WARP_STONE:	return (char*)"warp stone";
    case ITEM_GEM:		return (char*)"gem";
    case ITEM_JEWELRY:		return (char*)"jewelry";
    case ITEM_JUKEBOX:		return (char*)"juke box";
    case ITEM_TATTOO:		return (char*)"tattoo";
    }

    bug( "Item_type_name: unknown type %d.", obj->item_type );
    return (char*)"(unknown)";
}



/*
 * Return ascii name of an affect location.
 */
char *affect_loc_name( int location )
{
    switch ( location )
    {
    case APPLY_NONE:		return (char*)"none";
    case APPLY_STR:		return (char*)"strength";
    case APPLY_DEX:		return (char*)"dexterity";
    case APPLY_INT:		return (char*)"intelligence";
    case APPLY_WIS:		return (char*)"wisdom";
    case APPLY_CON:		return (char*)"constitution";
    case APPLY_CHA:		return (char*)"charisma";
    case APPLY_CLASS:		return (char*)"class";
    case APPLY_LEVEL:		return (char*)"level";
    case APPLY_AGE:		return (char*)"age";
    case APPLY_MANA:		return (char*)"mana";
    case APPLY_HIT:		return (char*)"hp";
    case APPLY_MOVE:		return (char*)"moves";
    case APPLY_GOLD:		return (char*)"gold";
    case APPLY_EXP:		return (char*)"experience";
    case APPLY_AC:		return (char*)"armor class";
    case APPLY_HITROLL:		return (char*)"hit roll";
    case APPLY_DAMROLL:		return (char*)"damage roll";
    case APPLY_SIZE:		return (char*)"size";
    case APPLY_SAVES:		return (char*)"saves";
    case APPLY_SAVING_ROD:	return (char*)"save vs rod";
    case APPLY_SAVING_PETRI:	return (char*)"save vs petrification";
    case APPLY_SAVING_BREATH:	return (char*)"save vs breath";
    case APPLY_SAVING_SPELL:	return (char*)"save vs spell";
    case APPLY_SPELL_AFFECT:	return (char*)"none";
    }

    bug( "Affect_location_name: unknown location %d.", location );
    return (char*)"(unknown)";
}



/*
 * Return ascii name of an affect bit vector.
 */
char *affect_bit_name( int vector )
{
    static char buf[512];

    buf[0] = '\0';
    if ( vector & AFF_BLIND         ) strcat( buf, " blind"         );
    if ( vector & AFF_INVISIBLE     ) strcat( buf, " invisible"     );
    if ( vector & AFF_IMP_INVIS     ) strcat( buf, " imp_invis"     );
    if ( vector & AFF_FADE	    ) strcat( buf, " fade"     	    );
    if ( vector & AFF_SCREAM	    ) strcat( buf, " scream"        );
    if ( vector & AFF_BLOODTHIRST   ) strcat( buf, " bloodthirst"   );
    if ( vector & AFF_STUN	    ) strcat( buf, " stun"   );
    if ( vector & AFF_SANCTUARY     ) strcat( buf, " sanctuary"     );
    if ( vector & AFF_FAERIE_FIRE   ) strcat( buf, " faerie_fire"   );
    if ( vector & AFF_INFRARED      ) strcat( buf, " infrared"      );
    if ( vector & AFF_CURSE         ) strcat( buf, " curse"         );
    if ( vector & AFF_POISON        ) strcat( buf, " poison"        );
    if ( vector & AFF_PROTECT_EVIL  ) strcat( buf, " prot_evil"     );
    if ( vector & AFF_PROTECT_GOOD  ) strcat( buf, " prot_good"     );
    if ( vector & AFF_SLEEP         ) strcat( buf, " sleep"         );
    if ( vector & AFF_SNEAK         ) strcat( buf, " sneak"         );
    if ( vector & AFF_HIDE          ) strcat( buf, " hide"          );
    if ( vector & AFF_CHARM         ) strcat( buf, " charm"         );
    if ( vector & AFF_FLYING        ) strcat( buf, " flying"        );
    if ( vector & AFF_PASS_DOOR     ) strcat( buf, " pass_door"     );
    if ( vector & AFF_BERSERK	    ) strcat( buf, " berserk"	    );
    if ( vector & AFF_CALM	    ) strcat( buf, " calm"	    );
    if ( vector & AFF_HASTE	    ) strcat( buf, " haste"	    );
    if ( vector & AFF_SLOW          ) strcat( buf, " slow"          );
    if ( vector & AFF_WEAKEN        ) strcat( buf, " weaken"        );
    if ( vector & AFF_PLAGUE	    ) strcat( buf, " plague" 	    );
    if ( vector & AFF_REGENERATION  ) strcat( buf, " regeneration"  );
    if ( vector & AFF_CAMOUFLAGE    ) strcat( buf, " camouflage"    );
    if ( vector & AFF_SWIM          ) strcat( buf, " swim"          );
    return (char *)(( buf[0] != '\0' ) ? buf+1 : "none");
}


/*
 * Return ascii name of an affect bit vector.
 */
char *detect_bit_name( int vector )
{
    static char buf[512];

    buf[0] = '\0';
    if ( vector & DETECT_IMP_INVIS   ) strcat( buf, " detect_imp_inv"   );
    if ( vector & DETECT_EVIL   ) strcat( buf, " detect_evil"   );
    if ( vector & DETECT_GOOD   ) strcat( buf, " detect_good"   );
    if ( vector & DETECT_INVIS  ) strcat( buf, " detect_invis"  );
    if ( vector & DETECT_MAGIC  ) strcat( buf, " detect_magic"  );
    if ( vector & DETECT_HIDDEN ) strcat( buf, " detect_hidden" );
    if ( vector & DARK_VISION   ) strcat( buf, " dark_vision"   );
    if ( vector & ACUTE_VISION  ) strcat( buf, " acute_vision"   );
    if ( vector & ADET_FEAR  	) strcat( buf, " fear"   );
    if ( vector & ADET_FORM_TREE  ) strcat( buf, " form_tree"   );
    if ( vector & ADET_FORM_GRASS ) strcat( buf, " form_grass"   );
    if ( vector & ADET_WEB	) strcat( buf, " web"   );
    if ( vector & DETECT_LIFE 	) strcat( buf, " life"   );
    if ( vector & DETECT_SNEAK 	) strcat( buf, " detect_sneak"   );
    return (char *)(( buf[0] != '\0' ) ? buf+1 : "none");
}



/*
 * Return ascii name of extra flags vector.
 */
char *extra_bit_name( int extra_flags )
{
    static char buf[512];

    buf[0] = '\0';
    if ( extra_flags & ITEM_GLOW         ) strcat( buf, " glow"         );
    if ( extra_flags & ITEM_HUM          ) strcat( buf, " hum"          );
    if ( extra_flags & ITEM_DARK         ) strcat( buf, " dark"         );
    if ( extra_flags & ITEM_LOCK         ) strcat( buf, " lock"         );
    if ( extra_flags & ITEM_EVIL         ) strcat( buf, " evil"         );
    if ( extra_flags & ITEM_INVIS        ) strcat( buf, " invis"        );
    if ( extra_flags & ITEM_MAGIC        ) strcat( buf, " magic"        );
    if ( extra_flags & ITEM_NODROP       ) strcat( buf, " nodrop"       );
    if ( extra_flags & ITEM_BLESS        ) strcat( buf, " bless"        );
    if ( extra_flags & ITEM_ANTI_GOOD    ) strcat( buf, " anti-good"    );
    if ( extra_flags & ITEM_ANTI_EVIL    ) strcat( buf, " anti-evil"    );
    if ( extra_flags & ITEM_ANTI_NEUTRAL ) strcat( buf, " anti-neutral" );
    if ( extra_flags & ITEM_NOREMOVE     ) strcat( buf, " noremove"     );
    if ( extra_flags & ITEM_INVENTORY    ) strcat( buf, " inventory"    );
    if ( extra_flags & ITEM_NOPURGE	 ) strcat( buf, " nopurge"	);
    if ( extra_flags & ITEM_VIS_DEATH	 ) strcat( buf, " vis_death"	);
    if ( extra_flags & ITEM_ROT_DEATH	 ) strcat( buf, " rot_death"	);
    if ( extra_flags & ITEM_NOLOCATE	 ) strcat( buf, " no_locate"	);
    if ( extra_flags & ITEM_SELL_EXTRACT ) strcat( buf, " sell_extract" );
    if ( extra_flags & ITEM_BURN_PROOF	 ) strcat( buf, " burn_proof"	);
    if ( extra_flags & ITEM_NOUNCURSE	 ) strcat( buf, " no_uncurse"	);
    if ( extra_flags & ITEM_BURIED	 ) strcat( buf, " buried"	);
    return (char *)(( buf[0] != '\0' ) ? buf+1 : "none");
}

/* return ascii name of an act vector */
char *act_bit_name( int act_flags )
{
    static char buf[512];

    buf[0] = '\0';

    if (IS_SET(act_flags,ACT_IS_NPC))
    {
 	strcat(buf," npc");
    	if (act_flags & ACT_SENTINEL 	) strcat(buf, " sentinel");
    	if (act_flags & ACT_SCAVENGER	) strcat(buf, " scavenger");
	if (act_flags & ACT_AGGRESSIVE	) strcat(buf, " aggressive");
	if (act_flags & ACT_STAY_AREA	) strcat(buf, " stay_area");
	if (act_flags & ACT_WIMPY	) strcat(buf, " wimpy");
	if (act_flags & ACT_PET		) strcat(buf, " pet");
	if (act_flags & ACT_TRAIN	) strcat(buf, " train");
	if (act_flags & ACT_PRACTICE	) strcat(buf, " practice");
	if (act_flags & ACT_UNDEAD	) strcat(buf, " undead");
	if (act_flags & ACT_HUNTER	) strcat(buf, " hunter");
	if (act_flags & ACT_CLERIC	) strcat(buf, " cleric");
	if (act_flags & ACT_MAGE	) strcat(buf, " mage");
	if (act_flags & ACT_THIEF	) strcat(buf, " thief");
	if (act_flags & ACT_WARRIOR	) strcat(buf, " warrior");
	if (act_flags & ACT_NOALIGN	) strcat(buf, " no_align");
	if (act_flags & ACT_NOPURGE	) strcat(buf, " no_purge");
	if (act_flags & ACT_IS_HEALER	) strcat(buf, " healer");
	if (act_flags & ACT_IS_CHANGER  ) strcat(buf, " changer");
	if (act_flags & ACT_GAIN	) strcat(buf, " skill_train");
	if (act_flags & ACT_UPDATE_ALWAYS) strcat(buf," update_always");
    }
    else
    {
	strcat(buf," player");
	if (act_flags & PLR_AUTOASSIST	) strcat(buf, " autoassist");
	if (act_flags & PLR_AUTOEXIT	) strcat(buf, " autoexit");
	if (act_flags & PLR_AUTOLOOT	) strcat(buf, " autoloot");
	if (act_flags & PLR_AUTOSAC	) strcat(buf, " autosac");
	if (act_flags & PLR_AUTOGOLD	) strcat(buf, " autogold");
	if (act_flags & PLR_AUTOSPLIT	) strcat(buf, " autosplit");
	if (act_flags & PLR_WANTED	) strcat(buf, " wanted");
	if (act_flags & PLR_NO_TITLE	) strcat(buf, " no_title");
	if (act_flags & PLR_NO_EXP	) strcat(buf, " no_exp");
	if (act_flags & PLR_HOLYLIGHT	) strcat(buf, " holy_light");
	if (act_flags & PLR_NOCANCEL	) strcat(buf, " no_cancel");
	if (act_flags & PLR_CANLOOT	) strcat(buf, " loot_corpse");
	if (act_flags & PLR_NOSUMMON	) strcat(buf, " no_summon");
	if (act_flags & PLR_NOFOLLOW	) strcat(buf, " no_follow");
	if (act_flags & PLR_CANINDUCT	) strcat(buf, " Cabal_LEADER");
	if (act_flags & PLR_GHOST	) strcat(buf, " ghost");
	if (act_flags & PLR_PERMIT	) strcat(buf, " permit");
	if (act_flags & PLR_REMORTED	) strcat(buf, " remorted");
	if (act_flags & PLR_LOG		) strcat(buf, " log");
	if (act_flags & PLR_FREEZE	) strcat(buf, " frozen");
	if (act_flags & PLR_LEFTHAND	) strcat(buf, " lefthand");
	if (act_flags & PLR_CANREMORT	) strcat(buf, " canremort");
	if (act_flags & PLR_QUESTOR	) strcat(buf, " questor");
	if (act_flags & PLR_VAMPIRE	) strcat(buf, " VAMPIRE");
	if (act_flags & PLR_HARA_KIRI	) strcat(buf, " harakiri");
	if (act_flags & PLR_BLINK_ON	) strcat(buf, " blink_on");
    }
    return (char *)(( buf[0] != '\0' ) ? buf+1 : "none");
}

char *comm_bit_name(int comm_flags)
{
    static char buf[512];

    buf[0] = '\0';

    if (comm_flags & COMM_QUIET		) strcat(buf, " quiet");
    if (comm_flags & COMM_DEAF		) strcat(buf, " deaf");
    if (comm_flags & COMM_NOWIZ		) strcat(buf, " no_wiz");
    if (comm_flags & COMM_NOAUCTION	) strcat(buf, " no_auction");
    if (comm_flags & COMM_NOGOSSIP	) strcat(buf, " no_gossip");
    if (comm_flags & COMM_NOQUESTION	) strcat(buf, " no_question");
    if (comm_flags & COMM_NOMUSIC	) strcat(buf, " no_music");
    if (comm_flags & COMM_NOQUOTE	) strcat(buf, " no_quote");
    if (comm_flags & COMM_COMPACT	) strcat(buf, " compact");
    if (comm_flags & COMM_BRIEF		) strcat(buf, " brief");
    if (comm_flags & COMM_PROMPT	) strcat(buf, " prompt");
    if (comm_flags & COMM_COMBINE	) strcat(buf, " combine");
    if (comm_flags & COMM_NOEMOTE	) strcat(buf, " no_emote");
    if (comm_flags & COMM_NOSHOUT	) strcat(buf, " no_shout");
    if (comm_flags & COMM_NOTELL	) strcat(buf, " no_tell");
    if (comm_flags & COMM_NOCHANNELS	) strcat(buf, " no_channels");


    return (char *)(( buf[0] != '\0' ) ? buf+1 : "none");
}

char *imm_bit_name(int imm_flags)
{
    static char buf[512];

    buf[0] = '\0';

    if (imm_flags & IMM_SUMMON		) strcat(buf, " summon");
    if (imm_flags & IMM_CHARM		) strcat(buf, " charm");
    if (imm_flags & IMM_MAGIC		) strcat(buf, " magic");
    if (imm_flags & IMM_WEAPON		) strcat(buf, " weapon");
    if (imm_flags & IMM_BASH		) strcat(buf, " blunt");
    if (imm_flags & IMM_PIERCE		) strcat(buf, " piercing");
    if (imm_flags & IMM_SLASH		) strcat(buf, " slashing");
    if (imm_flags & IMM_FIRE		) strcat(buf, " fire");
    if (imm_flags & IMM_COLD		) strcat(buf, " cold");
    if (imm_flags & IMM_LIGHTNING	) strcat(buf, " lightning");
    if (imm_flags & IMM_ACID		) strcat(buf, " acid");
    if (imm_flags & IMM_POISON		) strcat(buf, " poison");
    if (imm_flags & IMM_NEGATIVE	) strcat(buf, " negative");
    if (imm_flags & IMM_HOLY		) strcat(buf, " holy");
    if (imm_flags & IMM_ENERGY		) strcat(buf, " energy");
    if (imm_flags & IMM_MENTAL		) strcat(buf, " mental");
    if (imm_flags & IMM_DISEASE	) strcat(buf, " disease");
    if (imm_flags & IMM_DROWNING	) strcat(buf, " drowning");
    if (imm_flags & IMM_LIGHT		) strcat(buf, " light");
    if (imm_flags & VULN_IRON		) strcat(buf, " iron");
    if (imm_flags & VULN_WOOD		) strcat(buf, " wood");
    if (imm_flags & VULN_SILVER	) strcat(buf, " silver");

    return (char *)(( buf[0] != '\0' ) ? buf+1 : "none");
}

char *wear_bit_name(int wear_flags)
{
    static char buf[512];

    buf [0] = '\0';
    if (wear_flags & ITEM_TAKE		) strcat(buf, " take");
    if (wear_flags & ITEM_WEAR_FINGER	) strcat(buf, " finger");
    if (wear_flags & ITEM_WEAR_NECK	) strcat(buf, " neck");
    if (wear_flags & ITEM_WEAR_BODY	) strcat(buf, " torso");
    if (wear_flags & ITEM_WEAR_HEAD	) strcat(buf, " head");
    if (wear_flags & ITEM_WEAR_LEGS	) strcat(buf, " legs");
    if (wear_flags & ITEM_WEAR_FEET	) strcat(buf, " feet");
    if (wear_flags & ITEM_WEAR_HANDS	) strcat(buf, " hands");
    if (wear_flags & ITEM_WEAR_ARMS	) strcat(buf, " arms");
    if (wear_flags & ITEM_WEAR_SHIELD	) strcat(buf, " shield");
    if (wear_flags & ITEM_WEAR_ABOUT	) strcat(buf, " body");
    if (wear_flags & ITEM_WEAR_WAIST	) strcat(buf, " waist");
    if (wear_flags & ITEM_WEAR_WRIST	) strcat(buf, " wrist");
    if (wear_flags & ITEM_WIELD		) strcat(buf, " wield");
    if (wear_flags & ITEM_HOLD		) strcat(buf, " hold");
    if (wear_flags & ITEM_WEAR_FLOAT	) strcat(buf, " float");
    if (wear_flags & ITEM_WEAR_TATTOO	) strcat(buf, " tattoo");

    return (char *)(( buf[0] != '\0' ) ? buf+1 : "none");
}

char *form_bit_name(int form_flags)
{
    static char buf[512];

    buf[0] = '\0';
    if (form_flags & FORM_POISON	) strcat(buf, " poison");
    else if (form_flags & FORM_EDIBLE	) strcat(buf, " edible");
    if (form_flags & FORM_MAGICAL	) strcat(buf, " magical");
    if (form_flags & FORM_INSTANT_DECAY	) strcat(buf, " instant_rot");
    if (form_flags & FORM_OTHER		) strcat(buf, " other");
    if (form_flags & FORM_ANIMAL	) strcat(buf, " animal");
    if (form_flags & FORM_SENTIENT	) strcat(buf, " sentient");
    if (form_flags & FORM_UNDEAD	) strcat(buf, " undead");
    if (form_flags & FORM_CONSTRUCT	) strcat(buf, " construct");
    if (form_flags & FORM_MIST		) strcat(buf, " mist");
    if (form_flags & FORM_INTANGIBLE	) strcat(buf, " intangible");
    if (form_flags & FORM_BIPED		) strcat(buf, " biped");
    if (form_flags & FORM_CENTAUR	) strcat(buf, " centaur");
    if (form_flags & FORM_INSECT	) strcat(buf, " insect");
    if (form_flags & FORM_SPIDER	) strcat(buf, " spider");
    if (form_flags & FORM_CRUSTACEAN	) strcat(buf, " crustacean");
    if (form_flags & FORM_WORM		) strcat(buf, " worm");
    if (form_flags & FORM_BLOB		) strcat(buf, " blob");
    if (form_flags & FORM_MAMMAL	) strcat(buf, " mammal");
    if (form_flags & FORM_BIRD		) strcat(buf, " bird");
    if (form_flags & FORM_REPTILE	) strcat(buf, " reptile");
    if (form_flags & FORM_SNAKE		) strcat(buf, " snake");
    if (form_flags & FORM_DRAGON	) strcat(buf, " dragon");
    if (form_flags & FORM_AMPHIBIAN	) strcat(buf, " amphibian");
    if (form_flags & FORM_FISH		) strcat(buf, " fish");
    if (form_flags & FORM_COLD_BLOOD 	) strcat(buf, " cold_blooded");

    return (char *)(( buf[0] != '\0' ) ? buf+1 : "none");
}

char *part_bit_name(int part_flags)
{
    static char buf[512];

    buf[0] = '\0';
    if (part_flags & PART_HEAD		) strcat(buf, " head");
    if (part_flags & PART_ARMS		) strcat(buf, " arms");
    if (part_flags & PART_LEGS		) strcat(buf, " legs");
    if (part_flags & PART_HEART		) strcat(buf, " heart");
    if (part_flags & PART_BRAINS	) strcat(buf, " brains");
    if (part_flags & PART_GUTS		) strcat(buf, " guts");
    if (part_flags & PART_HANDS		) strcat(buf, " hands");
    if (part_flags & PART_FEET		) strcat(buf, " feet");
    if (part_flags & PART_FINGERS	) strcat(buf, " fingers");
    if (part_flags & PART_EAR		) strcat(buf, " ears");
    if (part_flags & PART_EYE		) strcat(buf, " eyes");
    if (part_flags & PART_LONG_TONGUE	) strcat(buf, " long_tongue");
    if (part_flags & PART_EYESTALKS	) strcat(buf, " eyestalks");
    if (part_flags & PART_TENTACLES	) strcat(buf, " tentacles");
    if (part_flags & PART_FINS		) strcat(buf, " fins");
    if (part_flags & PART_WINGS		) strcat(buf, " wings");
    if (part_flags & PART_TAIL		) strcat(buf, " tail");
    if (part_flags & PART_CLAWS		) strcat(buf, " claws");
    if (part_flags & PART_FANGS		) strcat(buf, " fangs");
    if (part_flags & PART_HORNS		) strcat(buf, " horns");
    if (part_flags & PART_SCALES	) strcat(buf, " scales");

    return (char *)(( buf[0] != '\0' ) ? buf+1 : "none");
}

char *weapon_bit_name(int weapon_flags)
{
    static char buf[512];

    buf[0] = '\0';
    if (weapon_flags & WEAPON_FLAMING	) strcat(buf, " flaming");
    if (weapon_flags & WEAPON_FROST	) strcat(buf, " frost");
    if (weapon_flags & WEAPON_VAMPIRIC	) strcat(buf, " vampiric");
    if (weapon_flags & WEAPON_SHARP	) strcat(buf, " sharp");
    if (weapon_flags & WEAPON_VORPAL	) strcat(buf, " vorpal");
    if (weapon_flags & WEAPON_TWO_HANDS ) strcat(buf, " two-handed");
    if (weapon_flags & WEAPON_SHOCKING 	) strcat(buf, " shocking");
    if (weapon_flags & WEAPON_POISON	) strcat(buf, " poison");
    if (weapon_flags & WEAPON_HOLY	) strcat(buf, " holy");

    return (char *)(( buf[0] != '\0' ) ? buf+1 : "none");
}

char *cont_bit_name( int cont_flags)
{
    static char buf[512];

    buf[0] = '\0';

    if (cont_flags & CONT_CLOSEABLE	) strcat(buf, " closable");
    if (cont_flags & CONT_PICKPROOF	) strcat(buf, " pickproof");
    if (cont_flags & CONT_CLOSED	) strcat(buf, " closed");
    if (cont_flags & CONT_LOCKED	) strcat(buf, " locked");

    return (char *)(( buf[0] != '\0' ) ? buf+1 : "none");
}


char *off_bit_name(int off_flags)
{
    static char buf[512];

    buf[0] = '\0';

    if (off_flags & OFF_AREA_ATTACK	) strcat(buf, " area attack");
    if (off_flags & OFF_BACKSTAB	) strcat(buf, " backstab");
    if (off_flags & OFF_BASH		) strcat(buf, " bash");
    if (off_flags & OFF_BERSERK		) strcat(buf, " berserk");
    if (off_flags & OFF_DISARM		) strcat(buf, " disarm");
    if (off_flags & OFF_DODGE		) strcat(buf, " dodge");
    if (off_flags & OFF_FADE		) strcat(buf, " fade");
    if (off_flags & OFF_FAST		) strcat(buf, " fast");
    if (off_flags & OFF_KICK		) strcat(buf, " kick");
    if (off_flags & OFF_KICK_DIRT	) strcat(buf, " kick_dirt");
    if (off_flags & OFF_PARRY		) strcat(buf, " parry");
    if (off_flags & OFF_RESCUE		) strcat(buf, " rescue");
    if (off_flags & OFF_TAIL		) strcat(buf, " tail");
    if (off_flags & OFF_TRIP		) strcat(buf, " trip");
    if (off_flags & OFF_CRUSH		) strcat(buf, " crush");
    if (off_flags & ASSIST_ALL		) strcat(buf, " assist_all");
    if (off_flags & ASSIST_ALIGN	) strcat(buf, " assist_align");
    if (off_flags & ASSIST_RACE		) strcat(buf, " assist_race");
    if (off_flags & ASSIST_PLAYERS	) strcat(buf, " assist_players");
    if (off_flags & ASSIST_GUARD	) strcat(buf, " assist_guard");
    if (off_flags & ASSIST_VNUM		) strcat(buf, " assist_vnum");

    return (char *)(( buf[0] != '\0' ) ? buf+1 : "none");
}

int cabal_lookup (const char *argument)
{
   int cabal;

   for ( cabal = 0; cabal < MAX_CABAL; cabal++)
   {
        if (LOWER(argument[0]) == LOWER(cabal_table[cabal].short_name[0])
        &&  !str_prefix( argument,cabal_table[cabal].short_name))
            return cabal;
   }

   return -1;
}


bool isn_dark_safe( CHAR_DATA *ch)
{
    CHAR_DATA *rch;
    OBJ_DATA *light;
    int light_exist;

    if (!IS_VAMPIRE(ch))  return 0;

    if ( IS_SET(ch->in_room->room_flags, ROOM_DARK) )
	return 0;

    if ( weather_info.sunlight == SUN_LIGHT
    ||   weather_info.sunlight == SUN_RISE )
	return 2;

    light_exist = 0;
    for ( rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room )
    {
	if ( (light = get_light_char(rch)) != NULL
	      && light->item_type == ITEM_LIGHT
	      && IS_OBJ_STAT(light,ITEM_MAGIC) )
	 {
	  light_exist = 1;
	  break;
	 }
    }

    return light_exist;
}


int ch_skill_nok_nomessage( CHAR_DATA *ch , int skill )
{
 if ( IS_NPC(ch) && RACE_OK(ch,skill) && CABAL_OK(ch,skill))
	return 0;
 if ( !IS_NPC(ch) && CLEVEL_OK(ch,skill) && RACE_OK(ch,skill) &&
	CABAL_OK(ch,skill) && ALIGN_OK(ch,skill)) return 0;
 return 1;
}

int ch_skill_nok( CHAR_DATA *ch, int skill )
{
   if (ch_skill_nok_nomessage(ch,skill))
   {
     send_to_char("Hý?\n\r",ch);
	 return 1;
   }
   return 0;
}

/* room affects by chronos */
void	affect_modify_room	args( ( ROOM_INDEX_DATA *room, AFFECT_DATA *paf, bool fAdd ) );

/*
 * Apply or remove an affect to a room.
 */
void affect_modify_room( ROOM_INDEX_DATA *room, AFFECT_DATA *paf, bool fAdd )
{
    int mod;

    mod = paf->modifier;

    if ( fAdd )
    {
	switch (paf->where)
	{
	case TO_ROOM_AFFECTS:
	      SET_BIT(room->affected_by, paf->bitvector);
	    break;
	case TO_ROOM_FLAGS:
	      SET_BIT(room->room_flags, paf->bitvector);
	    break;
	case TO_ROOM_CONST:
	    break;
	}
    }
    else
    {
        switch (paf->where)
        {
        case TO_ROOM_AFFECTS:
              REMOVE_BIT(room->affected_by, paf->bitvector);
            break;
	case TO_ROOM_FLAGS:
	      REMOVE_BIT(room->room_flags, paf->bitvector);
	    break;
        case TO_ROOM_CONST:
            break;
        }
	mod = 0 - mod;
    }

    switch ( paf->location )
    {
    default:
	bug( "Affect_modify_room: unknown location %d.", paf->location );
	return;

    case APPLY_ROOM_NONE:					break;
    case APPLY_ROOM_HEAL:	room->heal_rate += mod;		break;
    case APPLY_ROOM_MANA:	room->mana_rate += mod;		break;
    }

    return;
}

/*
 * Give an affect to a room.
 */
void affect_to_room( ROOM_INDEX_DATA *room, AFFECT_DATA *paf )
{
    AFFECT_DATA *paf_new;
    ROOM_INDEX_DATA *pRoomIndex;

    if ( ! room->affected )
    {
     if ( top_affected_room )
     {
      for ( pRoomIndex  = top_affected_room;
	  pRoomIndex->aff_next != NULL;
	  pRoomIndex  = pRoomIndex->aff_next )
			continue;
      pRoomIndex->aff_next = room;
     }
     else top_affected_room = room;
     room->aff_next = NULL;
    }

    paf_new = new_affect();

    *paf_new		= *paf;
    paf_new->next	= room->affected;
    room->affected	= paf_new;

    affect_modify_room( room , paf_new, TRUE );
    return;
}

void affect_check_room(ROOM_INDEX_DATA *room,int where,int vector)
{
    AFFECT_DATA *paf;

    if (vector == 0)
	return;

    for (paf = room->affected; paf != NULL; paf = paf->next)
	if (paf->where == where && paf->bitvector == vector)
	{
	    switch (where)
	    {
	        case TO_ROOM_AFFECTS:
		      SET_BIT(room->affected_by,vector);
		    break;
		case TO_ROOM_FLAGS:
	      	      SET_BIT(room->room_flags, vector);
	    	    break;
	        case TO_ROOM_CONST:
		    break;
	    }
	    return;
	}
}

/*
 * Remove an affect from a room.
 */
void affect_remove_room( ROOM_INDEX_DATA *room, AFFECT_DATA *paf )
{
    int where;
    int vector;


    if ( room->affected == NULL )
    {
	bug( "Affect_remove_room: no affect.", 0 );
	return;
    }

    affect_modify_room( room, paf, FALSE );
    where = paf->where;
    vector = paf->bitvector;

    if ( paf == room->affected )
    {
	room->affected	= paf->next;
    }
    else
    {
	AFFECT_DATA *prev;

	for ( prev = room->affected; prev != NULL; prev = prev->next )
	{
	    if ( prev->next == paf )
	    {
		prev->next = paf->next;
		break;
	    }
	}

	if ( prev == NULL )
	{
	    bug( "Affect_remove_room: cannot find paf.", 0 );
	    return;
	}
    }

    if ( !room->affected )
    {
     ROOM_INDEX_DATA *prev;

     if (top_affected_room  == room)
	{
	 top_affected_room = room->aff_next;
	}
     else
        {
         for(prev = top_affected_room; prev->aff_next; prev = prev->aff_next )
	  {
	    if ( prev->aff_next == room )
	    {
		prev->aff_next = room->aff_next;
		break;
	    }
	  }
	 if ( prev == NULL )
	  {
	    bug( "Affect_remove_room: cannot find room.", 0 );
	    return;
	  }
        }
      room->aff_next = NULL;

     }

    free_affect(paf);

    affect_check_room(room,where,vector);
    return;
}

/*
 * Strip all affects of a given sn.
 */
void affect_strip_room( ROOM_INDEX_DATA *room, int sn )
{
    AFFECT_DATA *paf;
    AFFECT_DATA *paf_next;

    for ( paf = room->affected; paf != NULL; paf = paf_next )
    {
	paf_next = paf->next;
	if ( paf->type == sn )
	    affect_remove_room( room, paf );
    }

    return;
}



/*
 * Return true if a room is affected by a spell.
 */
bool is_affected_room( ROOM_INDEX_DATA *room, int sn )
{
    AFFECT_DATA *paf;

    for ( paf = room->affected; paf != NULL; paf = paf->next )
    {
	if ( paf->type == sn )
	    return TRUE;
    }

    return FALSE;
}



/*
 * Add or enhance an affect.
 */
void affect_join_room( ROOM_INDEX_DATA *room, AFFECT_DATA *paf )
{
    AFFECT_DATA *paf_old;

    for ( paf_old = room->affected; paf_old != NULL; paf_old = paf_old->next )
    {
	if ( paf_old->type == paf->type )
	{
	    paf->level = (paf->level += paf_old->level) / 2;
	    paf->duration += paf_old->duration;
	    paf->modifier += paf_old->modifier;
	    affect_remove_room( room, paf_old );
	    break;
	}
    }

    affect_to_room( room, paf );
    return;
}

/*
 * Return ascii name of an raffect location.
 */
char *raffect_loc_name( int location )
{
    switch ( location )
    {
    case APPLY_ROOM_NONE:	return (char*)"none";
    case APPLY_ROOM_HEAL:	return (char*)"heal rate";
    case APPLY_ROOM_MANA:	return (char*)"mana rate";
    }

    bug( "Affect_location_name: unknown location %d.", location );
    return (char*)"(unknown)";
}


/*
 * Return ascii name of an affect bit vector.
 */
char *raffect_bit_name( int vector )
{
    static char buf[512];

    buf[0] = '\0';
    if ( vector & AFF_ROOM_SHOCKING  ) strcat( buf, " shocking"        );
    if ( vector & AFF_ROOM_L_SHIELD  ) strcat( buf, " lightning_shield");
    if ( vector & AFF_ROOM_THIEF_TRAP) strcat( buf, " thief_trap"      );
    if ( vector & AFF_ROOM_CURSE     ) strcat( buf, " curse"           );
    if ( vector & AFF_ROOM_POISON    ) strcat( buf, " poison"          );
    if ( vector & AFF_ROOM_PLAGUE    ) strcat( buf, " plague"          );
    if ( vector & AFF_ROOM_SLEEP     ) strcat( buf, " sleep"           );
    if ( vector & AFF_ROOM_SLOW      ) strcat( buf, " slow"            );
    return (char *)(( buf[0] != '\0' ) ? buf+1 : "none");
}


bool is_safe_rspell_nom(int level, CHAR_DATA *victim )
{
  /* ghosts are safe */
  if ( !IS_NPC(victim) && IS_SET(victim->act, PLR_GHOST))
    return TRUE;

  /* link dead players who do not have rushing adrenalin are safe */
  if (!IS_NPC(victim) && ((victim->last_fight_time == -1) ||
	((current_time - victim->last_fight_time) > FIGHT_DELAY_TIME)) &&
	victim->desc == NULL)
    return TRUE;

  if  ( victim->level < 5  && !IS_NPC(victim) )
    return TRUE;

  if ( !IS_NPC(victim) &&
      (victim->last_death_time != -1 && current_time - 	victim->last_death_time < 600))
    return TRUE;


  if ( !IS_NPC(victim) &&
      ((level >= victim->level + 5) || (victim->level >= level + 5)))
    return TRUE;

  return FALSE;
}


bool is_safe_rspell(int level, CHAR_DATA *victim)
{
  if (is_safe_rspell_nom(level,victim))
    {
      act("Tanrýlar $m koruyor.",victim,NULL,NULL,TO_CHAR);
      act("Tanrýlar $m odanýn büyüsünden koruyor.",victim,NULL,NULL,TO_ROOM);
      return TRUE;
    }
  else return FALSE;
}


void raffect_to_char( ROOM_INDEX_DATA *room, CHAR_DATA *ch)
 {
  AFFECT_DATA *paf;

  if ( IS_ROOM_AFFECTED( room, AFF_ROOM_L_SHIELD) )
  {
     int sn;
     CHAR_DATA *vch;

     if ( (sn = skill_lookup("lightning shield")) == -1 )
	{ bug("Bad sn for lightning shield",0); return; }

     for (vch=room->people;vch;vch=vch->next_in_room)
	{
	 if (is_room_owner(vch,room)) break;
	}

    if ( !vch )
	{
	 bug("Owner of lightning shield left the room.",0);
	 free_string(room->owner);
	 room->owner = str_dup("");
	 affect_strip_room( room,sn);
	}
     else
     {
       send_to_char("Odadaki koruma kalkaný seni engelliyor.\n\r",ch);
       act("$N odaya girdi.",vch,NULL,ch,TO_CHAR);
      do_wake(vch,(char*)"");

      if ( (paf = affect_find(room->affected,sn)) == NULL )
	 { bug("Bad paf for lightning shield",0); return; }

      if (!is_safe_rspell(paf->level,ch))
	{

         if (CAN_DETECT(ch, ADET_GROUNDING))
         {
           send_to_char("Enerji düþmanlarýnda yitip gidiyor.\n\r",ch);
           act("Bir yýldýrým $S düþmanlarýnda yitip gidiyor.\n\r",ch, NULL, ch, TO_ROOM);
         }
	 else
	 {
	   damage( vch,ch,dice(paf->level,4)+12,sn,DAM_LIGHTNING, TRUE);
	   free_string(room->owner);
	   room->owner = str_dup("");
	   affect_remove_room( room , paf);
	 }
	}
     }
   }

  if ( IS_ROOM_AFFECTED( room, AFF_ROOM_SHOCKING) )
  {
     int sn;

     if ( (sn = skill_lookup("shocking trap")) == -1 )
	{ bug("Bad sn for shocking shield",0); return; }

  send_to_char("Odadaki þok dalgalarý seni þokluyor.\n\r",ch);

     if ( (paf = affect_find(room->affected,sn)) == NULL )
	 { bug("Bad paf for shocking shield",0); return; }

     if (!is_safe_rspell(paf->level,ch))
	{
	 if ( check_immune(ch, DAM_LIGHTNING) != IS_IMMUNE )
	 damage( ch,ch,dice(paf->level,4)+12,TYPE_HUNGER,DAM_TRAP_ROOM, TRUE);
	 affect_remove_room( room , paf);
	}
   }

  if ( IS_ROOM_AFFECTED( room, AFF_ROOM_THIEF_TRAP) )
  {
    send_to_char("Birisi tarafýndan kurulmuþ tuzak seni engelliyor.\n\r",ch);

     if ( (paf = affect_find(room->affected,gsn_settraps)) == NULL )
	 { bug("Bad paf for settraps",0); return; }

     if (!is_safe_rspell(paf->level,ch))
	{
	 if ( check_immune(ch, DAM_PIERCE) != IS_IMMUNE )
	 damage( ch,ch,dice(paf->level,5)+12,TYPE_HUNGER,DAM_TRAP_ROOM, TRUE);
	 affect_remove_room( room , paf);
	}
   }

  if ( IS_ROOM_AFFECTED( room, AFF_ROOM_SLOW)
	|| IS_ROOM_AFFECTED( room, AFF_ROOM_SLEEP) )
  send_ch_color("$CHavayý dolduran gizemi hissediyorsun.$c\n\r",ch,POS_SLEEPING,CLR_YELLOW);

  return;
 }

void raffect_back_char( ROOM_INDEX_DATA *room, CHAR_DATA *ch)
 {
  if ( IS_ROOM_AFFECTED( room, AFF_ROOM_L_SHIELD) )
  {
   int sn;

    if ( (sn = skill_lookup("lightning shield")) == -1 )
	{ bug("Bad sn for lightning shield",0); return; }
    if (is_room_owner(ch,room))
	{
	 free_string(room->owner);
	 room->owner = str_dup("");
	 affect_strip_room( room,sn);
	}
   }
  return;
 }


/*
 * Return ascii name of an affect bit vector.
 */
char *flag_room_name( int vector )
{
    static char buf[512];

    buf[0] = '\0';
    if ( vector & ROOM_DARK         ) strcat( buf, " dark"         );
    if ( vector & ROOM_NO_MOB        ) strcat( buf, " nomob"        );
    if ( vector & ROOM_INDOORS      ) strcat( buf, " indoors"      );
    if ( vector & ROOM_PRIVATE      ) strcat( buf, " private"      );
    if ( vector & ROOM_SAFE         ) strcat( buf, " safe"         );
    if ( vector & ROOM_SOLITARY     ) strcat( buf, " solitary"     );
    if ( vector & ROOM_PET_SHOP     ) strcat( buf, " petshop"      );
    if ( vector & ROOM_NO_RECALL    ) strcat( buf, " norecall"     );
    if ( vector & ROOM_IMP_ONLY     ) strcat( buf, " imp_only"     );
    if ( vector & ROOM_GODS_ONLY    ) strcat( buf, " god_only"     );
    if ( vector & ROOM_HEROES_ONLY  ) strcat( buf, " heroes"       );
    if ( vector & ROOM_NEWBIES_ONLY ) strcat( buf, " newbies"      );
    if ( vector & ROOM_LAW          ) strcat( buf, " law"          );
    if ( vector & ROOM_NOWHERE      ) strcat( buf, " nowhere"      );
    if ( vector & ROOM_BANK         ) strcat( buf, " bank"         );
    if ( vector & ROOM_NO_MAGIC     ) strcat( buf, " nomagic"      );
    if ( vector & ROOM_NOSUMMON     ) strcat( buf, " nosummon"     );
    if ( vector & ROOM_REGISTRY     ) strcat( buf, " registry"     );
    return (char *)(( buf[0] != '\0' ) ? buf+1 : "none");
}


int affect_check_obj(CHAR_DATA *ch,int vector)
{
    AFFECT_DATA *paf;
    OBJ_DATA *obj;

    if (vector == 0) 	return 0;

    for (obj = ch->carrying; obj != NULL; obj = obj->next_content)
    {
	if (obj->wear_loc == -1 || obj->wear_loc == WEAR_STUCK_IN)
	    continue;

        for (paf = obj->affected; paf != NULL; paf = paf->next)
	    {
            if ( paf->bitvector == vector)
                return 1;
            }

        for (paf = obj->pIndexData->affected; paf != NULL; paf = paf->next)
	    {
            if ( paf->bitvector == vector)
		return 1;
            }
    }
  return 0;
}

int count_charmed( CHAR_DATA *ch )
{
  CHAR_DATA *gch;
  int count = 0;

  for (gch = char_list; gch != NULL; gch = gch->next)
  {
      if ( IS_AFFECTED(gch,AFF_CHARM)
		&& gch->master == ch
		&& ch->pet != gch )
	  count++;
  }

  if (count >= MAX_CHARM(ch) )
  {
    send_to_char("Emrindekilerden fazlasýný kontrol etmeye gücün yetmez!\n\r",ch);
    return count;
  }

  return 0;
}

void add_mind(CHAR_DATA *ch, char *str)
{
 char buf[MAX_STRING_LENGTH];

 if (!IS_NPC(ch) || ch->in_room == NULL) return;

 if (ch->in_mind == NULL)
  {
   sprintf(buf,"%d",ch->in_room->vnum);
   ch->in_mind = str_dup(buf);
  }
 if (!is_name(str,ch->in_mind))
  {
    sprintf(buf,"%s %s",ch->in_mind,str);
    free_string(ch->in_mind); ch->in_mind = str_dup(buf);
  }

}
void remove_mind(CHAR_DATA *ch, char *str)
{
 char buf[MAX_STRING_LENGTH];
 char buff[MAX_STRING_LENGTH];
 char arg[MAX_INPUT_LENGTH];
 char *mind = ch->in_mind;

 if (!IS_NPC(ch) || ch->in_room == NULL
	|| mind == NULL || !is_name(str,mind) ) return;

 buf[0] = '\0';
 do
  {
   mind = one_argument(mind,arg);
   if (!is_name(str,arg))
   {
    if (buf[0] == '\0') strcpy(buff,arg);
    else sprintf(buff,"%s %s",buf,arg);
    strcpy(buf,buff);
   }
  }
 while ( mind[0] != '\0' );

 do_say(ch,(char*)"Sonunda intikamýmý aldým!");
 free_string(ch->in_mind);
 ch->in_mind = str_dup( buf );
 if (is_number(buf)) back_home(ch);
}

int opposite_door(int door)
{
  int opdoor;

  switch ( door )
   {
    case 0: opdoor=2;	break;
    case 1: opdoor=3;	break;
    case 2: opdoor=0;	break;
    case 3: opdoor=1;	break;
    case 4: opdoor=5;	break;
    case 5: opdoor=4;	break;
    default: opdoor=-1; break;
  }
  return opdoor;
}
void back_home( CHAR_DATA *ch )
{
 ROOM_INDEX_DATA *location;
 char arg[MAX_INPUT_LENGTH];

 if (!IS_NPC(ch) || ch->in_mind == NULL) return;

 one_argument(ch->in_mind,arg);
 if ( ( location = find_location( ch, arg ) ) == NULL )
  {
   bug("Mob cannot return to reset place",0);
   return;
  }

 if ( ch->fighting == NULL && location != ch->in_room )
  {
    act("$n nakil için dua ediyor.",ch,NULL,NULL,TO_ROOM);
   char_from_room( ch );
   char_to_room( ch, location );
   act("$n odada beliriyor.",ch,NULL,NULL,TO_ROOM);
   if (is_number(ch->in_mind))
   {
	free_string(ch->in_mind);
	ch->in_mind = NULL;
   }
 }
}

CHAR_DATA * find_char( CHAR_DATA *ch, char *argument,int door, int range )
{
 EXIT_DATA *pExit,*bExit;
 ROOM_INDEX_DATA *dest_room,*back_room;
 CHAR_DATA *target;
 int number = 0,opdoor;
 char arg[MAX_INPUT_LENGTH];

 number = number_argument(argument,arg);
 dest_room = ch->in_room;
 if ( (target = get_char_room2(ch,dest_room,arg,&number)) != NULL)
	return target;

 if ( (opdoor = opposite_door( door )) == -1)
  {
   bug("In find_char wrong door: %d",door);
   send_to_char("Onu orada görmüyorsun.\n\r",ch);
   return NULL;
 }
 while (range > 0)
 {
  range--;
  /* find target room */
  back_room = dest_room;
  if ( (pExit = dest_room->exit[door]) == NULL
      || (dest_room = pExit->u1.to_room) == NULL
      || IS_SET(pExit->exit_info,EX_CLOSED) )
   break;
  if ( (bExit = dest_room->exit[opdoor]) == NULL
      || bExit->u1.to_room != back_room)
   {
     send_to_char("Seçtiðin yol gücünün yoldan geçmesine izin vermiyor.\n\r",ch);
    return NULL;
   }
  if ((target = get_char_room2(ch,dest_room,arg,&number)) != NULL )
	return target;
 }

 send_to_char("Onu orada görmüyorsun.\n\r",ch);
 return NULL;
}

int check_exit( char *arg )
{
    int door = -1;

    if ( !str_cmp( arg, "k" ) || !str_cmp( arg, "kuzey" ) ) door = 0;
     else if ( !str_cmp( arg, "d" ) || !str_cmp( arg, "doðu"  ) ) door = 1;
     else if ( !str_cmp( arg, "g" ) || !str_cmp( arg, "güney" ) ) door = 2;
     else if ( !str_cmp( arg, "b" ) || !str_cmp( arg, "batý"  ) ) door = 3;
     else if ( !str_cmp( arg, "y" ) || !str_cmp( arg, "yukarý"    ) ) door = 4;
     else if ( !str_cmp( arg, "a" ) || !str_cmp( arg, "aþaðý"  ) ) door = 5;

    return door;
}

/*
 * Find a char for spell usage.
 */
CHAR_DATA *get_char_spell( CHAR_DATA *ch, char *argument, int *door, int range )
{
 char buf[MAX_INPUT_LENGTH];
 int i;

 for(i=0;argument[i] != '\0' && argument[i] != '.';i++)
   buf[i] = argument[i];
 buf[i] = '\0';

 if (i == 0 || (*door = check_exit(buf)) == -1)
	return get_char_room(ch,argument);

 return find_char(ch,(argument+i+1),*door,range);
}

void path_to_track( CHAR_DATA *ch, CHAR_DATA *victim, int door)
{
  ROOM_INDEX_DATA *temp;
  EXIT_DATA *pExit;
  int opdoor;
  int range = 0;
  int i;

  ch->last_fight_time = current_time;
  if (!IS_NPC(victim)) victim->last_fight_time = current_time;

  if (IS_NPC(victim) && victim->position != POS_DEAD)
   {
    victim->last_fought = ch;

    if ( (opdoor = opposite_door( door )) == -1)
	{
	 bug("In path_to_track wrong door: %d",door);
	 return;
	}
    temp = ch->in_room;
    for(i=0; i< 1000; i++)
     {
      range++;
      if ( victim->in_room == temp ) break;
      if ((pExit = temp->exit[ door ]) == NULL
	  || (temp = pExit->u1.to_room) == NULL)
       {
	bug("In path_to_track: couldn't calculate range %d",range);
	return;
       }
      if ( range > 100)
       {
	bug("In path_to_track: range exceeded 100",0);
	return;
       }
     }

    temp = victim->in_room;
    while (--range > 0)
       {
        room_record(ch->name,temp, opdoor);
        if ((pExit = temp->exit[opdoor]) == NULL
	    || (temp = pExit->u1.to_room) == NULL )
	{
	 sprintf(log_buf,"Path to track: Range: %d Room: %d opdoor:%d",
		range,temp->vnum,opdoor);
	 bug(log_buf,0);
	 return;
	}
       }
    do_track(victim,(char*)"");
  }
 return;
}



/* new staff */

OBJ_DATA *get_wield_char( CHAR_DATA *ch, bool second )
{
    OBJ_DATA *obj;

    if (ch == NULL)
	return NULL;

    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
	if ( obj->item_type == ITEM_WEAPON )
	{
	 if (second)
	 {
	   if ((obj->wear_loc == WEAR_RIGHT && LEFT_HANDER(ch))
		|| (obj->wear_loc == WEAR_LEFT && RIGHT_HANDER(ch)))
	   return obj;
	 }
	 else
	 {
	   if ((obj->wear_loc == WEAR_RIGHT && RIGHT_HANDER(ch))
		|| (obj->wear_loc == WEAR_LEFT && LEFT_HANDER(ch))
		|| obj->wear_loc == WEAR_BOTH)
	   return obj;
	 }
	}
    }

    return NULL;
}


OBJ_DATA *get_shield_char( CHAR_DATA *ch )
{
    OBJ_DATA *obj;

    if (ch == NULL)
	return NULL;

    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
	if ( (obj->wear_loc == WEAR_LEFT || obj->wear_loc == WEAR_RIGHT ||
	     obj->wear_loc == WEAR_BOTH) && CAN_WEAR(obj,ITEM_WEAR_SHIELD))
	   return obj;
    }

    return NULL;
}


OBJ_DATA *get_hold_char( CHAR_DATA *ch )
{
    OBJ_DATA *obj;

    if (ch == NULL)
	return NULL;

    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
	if ( (obj->wear_loc == WEAR_LEFT || obj->wear_loc == WEAR_RIGHT ||
	     obj->wear_loc == WEAR_BOTH) && CAN_WEAR(obj,ITEM_HOLD))
	   return obj;
    }

    return NULL;
}


OBJ_DATA *get_light_char( CHAR_DATA *ch )
{
    OBJ_DATA *obj;

    if (ch == NULL)
	return NULL;

    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
	if ( (obj->item_type == ITEM_LIGHT
	     && obj->value[2] != 0
	     && ( obj->wear_loc == WEAR_LEFT
		|| obj->wear_loc == WEAR_RIGHT
		|| obj->wear_loc == WEAR_BOTH ))
	  || ( obj->wear_loc == WEAR_HEAD
	     && IS_OBJ_STAT(obj, ITEM_GLOW)) )

	   return obj;
    }

    return NULL;
}


bool is_wielded_char( CHAR_DATA *ch, OBJ_DATA *obj )
{
    OBJ_DATA *w;

    if (ch == NULL)
	return FALSE;

    for ( w = ch->carrying; w != NULL; w = w->next_content )
    {
	if ( (w->wear_loc == WEAR_LEFT || w->wear_loc == WEAR_RIGHT ||
	      w->wear_loc == WEAR_BOTH) && CAN_WEAR(w,ITEM_WIELD)
	      && w == obj)
	   return TRUE;
    }

    return FALSE;
}


bool is_equiped_n_char( CHAR_DATA *ch, int vnum, int iWear )
{
    OBJ_DATA *e;

    if (ch == NULL)
	return FALSE;

    for ( e = ch->carrying; e != NULL; e = e->next_content )
    {
	if ( e->wear_loc == iWear && e->pIndexData->vnum == vnum )
	   return TRUE;
    }

    return FALSE;
}


bool is_equiped_char( CHAR_DATA *ch, OBJ_DATA *obj, int iWear )
{
    OBJ_DATA *e;

    if (ch == NULL)
	return FALSE;

    for ( e = ch->carrying; e != NULL; e = e->next_content )
    {
	if ( e->wear_loc == iWear && e == obj )
	   return TRUE;
    }

    return FALSE;
}



int count_worn( CHAR_DATA *ch, int iWear )
{
    OBJ_DATA *obj;
    int count;

    if (ch == NULL)
	return FALSE;

    count = 0;
    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
	if ( obj->wear_loc == iWear)	count++;

    return count;
}



int max_can_wear( CHAR_DATA *ch, int i )
{
    if (IS_NPC(ch) || !IS_SET(ch->act, PLR_REMORTED))
	return ( i == WEAR_FINGER ? MAX_FINGER : \
                                  i == WEAR_STUCK_IN ? MAX_STUCK_IN : \
                                  i == WEAR_WRIST ? MAX_WRIST : \
                                  i == WEAR_TATTOO ? MAX_TATTOO : \
                                  i == WEAR_NECK ? MAX_NECK : 1 );
    else
	return ( i == WEAR_FINGER ? (MAX_FINGER + 2) : \
                                  i == WEAR_STUCK_IN ? MAX_STUCK_IN : \
                                  i == WEAR_WRIST ? MAX_WRIST : \
                                  i == WEAR_TATTOO ? MAX_TATTOO : \
                                  i == WEAR_NECK ? MAX_NECK : 1 );
}

int get_total_played( CHAR_DATA *ch )
{
    int l;
    int sum=0;

    if (IS_NPC(ch)) return 0;

    for( l=0; l<MAX_TIME_LOG; l++)
	sum += ch->pcdata->log_time[l];

    return (sum + parse_time_spec(current_time - ch->logon));
}

int check_time_sync( )
{
    char *strtime;
    char mytime[MAX_INPUT_LENGTH];
    char tword[MAX_INPUT_LENGTH];
    int thour, tmin;

    strtime                    = ctime( &current_time );
    strtime[strlen(strtime)-1] = '\0';

    strtime = one_argument( strtime, tword);
    strtime = one_argument( strtime, tword);
    strtime = one_argument( strtime, tword);
    strtime = one_argument( strtime, mytime);

    tword[0] = mytime[0];
    tword[1] = mytime[1];
    tword[2] = '\0';
    thour = atoi( tword );

    tword[0] = mytime[3];
    tword[1] = mytime[4];
    tword[2] = '\0';
    tmin = atoi( tword );

    if (thour == 23 && tmin > 54)
	return (59 - tmin);
    return 0;
}

int parse_date( time_t t)
{
    char *strtime;
    char day[MAX_INPUT_LENGTH];
    char mon[MAX_INPUT_LENGTH];
    int d;

    strtime                    = ctime( &t );
    strtime[strlen(strtime)-1] = '\0';

    strtime = one_argument( strtime, mon);	/* temporary */
    strtime = one_argument( strtime, mon);
    strtime = one_argument( strtime, day);
    d = atoi( day );

    if (!str_cmp(mon, "Jan"))	;
    else if (!str_cmp(mon, "Feb"))
    {
	d += 31;
        if ( d > 59 ) d = 59;	/* discard 29th of February */
    }
    else if (!str_cmp(mon, "Mar"))	d += 59;
    else if (!str_cmp(mon, "Apr"))	d += 90;
    else if (!str_cmp(mon, "May"))	d += 120;
    else if (!str_cmp(mon, "Jun"))	d += 151;
    else if (!str_cmp(mon, "Jul"))	d += 181;
    else if (!str_cmp(mon, "Aug"))	d += 212;
    else if (!str_cmp(mon, "Sep"))	d += 242;
    else if (!str_cmp(mon, "Oct"))	d += 273;
    else if (!str_cmp(mon, "Nov"))	d += 303;
    else if (!str_cmp(mon, "Dec"))	d += 334;
    else return -1;

    return d;
}

int parse_time_spec( time_t t )
{
    int h, m, s;

    s = t % 60;
    m = t % 3600;
    h = ((t % 86400) - m) / 3600;
    m = (m - s) / 60;

    return ((h*60)+m);
}

int parse_time( time_t t )
{
    int h, m, s;

    s = t % 60;
    m = t % 3600;
    h = ((t % 86400) - m) / 3600;
    h = ( h + 16) % 24;
    m = (m - s) / 60;

    return ((h*60)+m);
}
