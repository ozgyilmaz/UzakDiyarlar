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
#include <wchar.h>
#include <wctype.h>
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
ROOM_INDEX_DATA *	find_location	args( ( CHAR_DATA *ch, wchar_t *arg ) );

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
int material_lookup (const wchar_t *name)
{
    return 0;
}

/* returns race number */
int race_lookup (const wchar_t *name)
{
   int race;
   wchar_t buf[MAX_STRING_LENGTH];

   if( !wcscmp( name, L"duergar" ) || !wcscmp( name, L"yeg" ) || !wcscmp( name, L"githyanki" ) || !wcscmp( name, L"arial" ) )
	   wcscpy( (wchar_t*)name, L"gamayun");
   if( !wcscmp( name, L"half-elf" ) || !wcscmp( name, L"dark-elf" ) || !wcscmp( name, L"rockseer" ) )
	   wcscpy( (wchar_t*)name, L"çora");
   if( !wcscmp( name, L"felar" ) || !wcscmp( name, L"börü" ) || !wcscmp( name, L"naga" ) || !wcscmp( name, L"ciren" ) )
	   wcscpy( (wchar_t*)name, L"pardus");
   if( !wcscmp( name, L"troll" ) || !wcscmp( name, L"trol" ) )
	   wcscpy( (wchar_t*)name, L"asura");

   for ( race = 0; race_table[race].name[1] != NULL; race++)
   {
     if (((towlower(name[0]) == towlower(race_table[race].name[0][0])) &&  !str_prefix( name,race_table[race].name[0])) ||
        ((towlower(name[0]) == towlower(race_table[race].name[1][0])) &&  !str_prefix( name,race_table[race].name[1])) )
	    return race;
   }

   swprintf( buf, MAX_STRING_LENGTH-1, L"Race_lookup: race not found %s.", name);
   bug(buf, 0);
   return 0;
}

int liq_lookup (const wchar_t *name)
{
    int liq;

    for ( liq = 0; liq_table[liq].liq_name != NULL; liq++)
    {
	if (towlower(name[0]) == towlower(liq_table[liq].liq_name[0])
	&& !str_prefix(name,liq_table[liq].liq_name))
	    return liq;
    }

    return LIQ_WATER;
}

int weapon_lookup (const wchar_t *name)
{
    int type;

    for (type = 0; weapon_table[type].name != NULL; type++)
    {
	if (towlower(name[0]) == towlower(weapon_table[type].name[0])
	&&  !str_prefix(name,weapon_table[type].name))
	    return type;
    }

    return -1;
}

/*
 * Belirtilen büyü/yetenek için ch'nin cabal durumu OK midir?
 */
bool cabal_ok(CHAR_DATA *ch, sh_int sn)
{
  int i;

  // ch bir NPC
  if (IS_NPC(ch))
    return TRUE;

  // belirtilen yetenek/büyü kabal bağımlı değil.
  if (skill_table[sn].cabal == CABAL_NONE)
    return TRUE;

  // kabal eşyasının tablodaki pointer'ı henüz boş.
  // kabal eşyası extract edilmiş olabilir.
  if (cabal_table[ch->cabal].obj_ptr == NULL)
    return TRUE;

  // Kabal eşyası bir odaya bırakılmamış.
  // Alan kişi envanterinde taşıyor olabilir.
  if (cabal_table[ch->cabal].obj_ptr->in_room == NULL )
    return TRUE;

  // Kabal eşyası olması gereken odada duruyor.
  if (cabal_table[ch->cabal].obj_ptr->in_room->vnum == cabal_table[ch->cabal].room_vnum)
    return TRUE;

  for (i=1;i < MAX_CABAL; i++)
  {
    // Kabal eşyası başka bir kabalın hedef odasına koyulmuş.
    // Artık eşyası alınan kabalın güçleri iptal edilebilir.
    if (cabal_table[ch->cabal].obj_ptr->in_room->vnum == cabal_table[i].room_vnum)
    {
      send_to_char( L"Kabal gücünün varolmadığını hissediyorsun.\n\r",ch);
      return FALSE;
    }
  }

  return TRUE;
}

int weapon_type (const wchar_t *name)
{
    int type;

    for (type = 0; weapon_table[type].name != NULL; type++)
    {
        if (towlower(name[0]) == towlower(weapon_table[type].name[0])
        &&  !str_prefix(name,weapon_table[type].name))
            return weapon_table[type].type;
    }

    return WEAPON_EXOTIC;
}


int item_lookup(const wchar_t *name)
{
    int type;

    for (type = 0; item_table[type].name != NULL; type++)
    {
        if (towlower(name[0]) == towlower(item_table[type].name[0])
        &&  !str_prefix(name,item_table[type].name))
            return item_table[type].type;
    }

    return -1;
}

const wchar_t *item_name(int item_type)
{
    int type;

    for (type = 0; item_table[type].name != NULL; type++)
	if (item_type == item_table[type].type)
	    return item_table[type].name;
    return L"none";
}

const wchar_t *weapon_name( int weapon_type)
{
    int type;

    for (type = 0; weapon_table[type].name != NULL; type++)
        if (weapon_type == weapon_table[type].type)
            return weapon_table[type].name;
    return L"exotic";
}

void familya_check_improve(CHAR_DATA* ch,CHAR_DATA* victim)
{
  int chance;

  if (IS_NPC(ch))
  {
    return;
  }
  if(ch->pcdata->familya[victim->race] >= 100)
  {
    return;
  }
  chance = 2 * int_app[get_curr_stat(ch,STAT_INT)].learn;

  if(number_range(1,10000) > chance)
  {
    return;
  }
  ch->pcdata->familya[victim->race]++;
  printf_to_char(ch,L"{g%s ırkına ilişkin irfanın artıyor!{x\n\r",race_table[victim->race].name[1]);
  return;
}

/*
 * Check the material
 */
bool check_material( OBJ_DATA *obj, wchar_t *material )
{
    if ( wcsstr( obj->material, material ) != NULL )
	return TRUE;
    else
        return FALSE;

}

bool is_metal( OBJ_DATA *obj )
{

  if ( check_material(obj, (wchar_t*)"silver") ||
       check_material(obj, (wchar_t*)"gold") ||
       check_material(obj, (wchar_t*)"iron") ||
       check_material(obj, (wchar_t*)"mithril") ||
       check_material(obj, (wchar_t*)"adamantite") ||
       check_material(obj, (wchar_t*)"steel") ||
       check_material(obj, (wchar_t*)"lead") ||
       check_material(obj, (wchar_t*)"bronze") ||
       check_material(obj, (wchar_t*)"copper") ||
       check_material(obj, (wchar_t*)"brass") ||
       check_material(obj, (wchar_t*)"platinium") ||
       check_material(obj, (wchar_t*)"titanium") ||
       check_material(obj, (wchar_t*)"aliminum") )
    return TRUE;

  return FALSE;

}

bool may_float( OBJ_DATA *obj )
{

    if ( check_material( obj, (wchar_t*)"wood" )  ||
         check_material( obj, (wchar_t*)"ebony" )  ||
         check_material( obj, (wchar_t*)"ice" )  ||
         check_material( obj, (wchar_t*)"energy" )  ||
         check_material( obj, (wchar_t*)"hardwood" )  ||
         check_material( obj, (wchar_t*)"softwood" )  ||
         check_material( obj, (wchar_t*)"flesh" )  ||
         check_material( obj, (wchar_t*)"silk" )  ||
         check_material( obj, (wchar_t*)"wool" )  ||
         check_material( obj, (wchar_t*)"cloth" )  ||
         check_material( obj, (wchar_t*)"fur" )  ||
         check_material( obj, (wchar_t*)"water" )  ||
         check_material( obj, (wchar_t*)"ice" )  ||
         check_material( obj, (wchar_t*)"oak" ) )
       return TRUE;

    if ( obj->item_type == ITEM_BOAT )
	return TRUE;

    return FALSE;
}


bool cant_float( OBJ_DATA *obj )
{
    if ( check_material( obj, (wchar_t*)"steel" ) ||
         check_material( obj, (wchar_t*)"iron" ) ||
         check_material( obj, (wchar_t*)"brass" ) ||
         check_material( obj, (wchar_t*)"silver" ) ||
         check_material( obj, (wchar_t*)"gold" ) ||
         check_material( obj, (wchar_t*)"ivory" ) ||
         check_material( obj, (wchar_t*)"copper" ) ||
         check_material( obj, (wchar_t*)"diamond" ) ||
         check_material( obj, (wchar_t*)"pearl" ) ||
         check_material( obj, (wchar_t*)"gem" ) ||
         check_material( obj, (wchar_t*)"platinium" ) ||
         check_material( obj, (wchar_t*)"ruby" ) ||
         check_material( obj, (wchar_t*)"bronze" ) ||
         check_material( obj, (wchar_t*)"titanium" ) ||
         check_material( obj, (wchar_t*)"mithril" ) ||
         check_material( obj, (wchar_t*)"obsidian" ) ||
         check_material( obj, (wchar_t*)"lead" ) )
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
    case ITEM_MAYMUNCUK 	: ftime = 1;	break;
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

int attack_lookup  (const wchar_t *name)
{
    int att;

    for ( att = 0; attack_table[att].name != NULL; att++)
    {
	if (towlower(name[0]) == towlower(attack_table[att].name[0])
	&&  !str_prefix(name,attack_table[att].name))
	    return att;
    }

    return 0;
}

/* returns a flag for wiznet */
long wiznet_lookup (const wchar_t *name)
{
    int flag;

    for (flag = 0; wiznet_table[flag].name != NULL; flag++)
    {
	if (towlower(name[0]) == towlower(wiznet_table[flag].name[0])
	&& !str_prefix(name,wiznet_table[flag].name))
	    return flag;
    }

    return -1;
}

/* returns class number */
int class_lookup (const wchar_t *name)
{
   int iclass;

   for ( iclass = 0; iclass < MAX_CLASS; iclass++)
   {
        if ( (towlower(name[0]) == towlower(class_table[iclass].name[0][0]) &&  !str_prefix( name,class_table[iclass].name[0])) ||
              (towlower(name[0]) == towlower(class_table[iclass].name[1][0]) &&  !str_prefix( name,class_table[iclass].name[1])) )
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
	bug( L"Bad sn %d in get_skill.",sn);
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

  if (IS_SET(ch->act,PLR_GHOST))
  {
    ch->pcdata->ghost_mode_counter = 0;
    REMOVE_BIT(ch->act,PLR_GHOST);
    printf_to_char(ch,L"Ete kemiğe büründüğünü hissediyorsun. Arkanı kollamaya başlasan iyi olur!\n\r");
    act ( L"$n ete kemiğe bürünüyor!",ch,NULL,NULL,TO_ROOM);
    while ( ch->affected )
      affect_remove( ch, ch->affected );
    ch->affected_by	= 0;
    ch->detection	= 0;
  }

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

/* command for retrieving stats */
int get_curr_stat( CHAR_DATA *ch, int stat )
{
  int max;

  if (IS_NPC(ch) || ch->level > LEVEL_IMMORTAL)
  {
    max = 25;
  }
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

    max = ( race_table[ORG_RACE(ch)].stats[stat] + class_table[ch->iclass].stats[stat]);

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

bool is_name( wchar_t *str, wchar_t *namelist )
{
    wchar_t name[MAX_INPUT_LENGTH], part[MAX_INPUT_LENGTH];
    wchar_t *list, *string;


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
	bug( L"Affect_modify: unknown location %d.", paf->location );
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
      act( L"$p objesini bırakıyorsun.", ch, hold, NULL, TO_CHAR );
	    act( L"$n $p objesini bırakıyor.", ch, hold, NULL, TO_ROOM );
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
      act( L"$p objesini bırakıyorsun.", ch, hold, NULL, TO_CHAR );
	    act( L"$n $p objesini bırakıyor.", ch, hold, NULL, TO_ROOM );
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
      act( L"$p objesini bırakıyorsun.", ch, hold, NULL, TO_CHAR );
	    act( L"$n $p objesini bırakıyor.", ch, hold, NULL, TO_ROOM );
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
	bug( L"Affect_remove: no affect.", 0 );
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
	    bug( L"Affect_remove: cannot find paf.", 0 );
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
        bug( L"Affect_remove_object: no affect.", 0 );
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
            bug( L"Affect_remove_object: cannot find paf.", 0 );
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
    int level;

    for ( paf_old = ch->affected; paf_old != NULL; paf_old = paf_old->next )
    {
	if ( paf_old->type == paf->type )
	{
      level = (paf->level + paf_old->level) / 2;
	    paf->level = (sh_int) level;
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
	bug( L"Obj_from_char: null ch.", 0 );
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
	    bug( L"Obj_from_char: obj not in list.", 0 );
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
	bug( L"Equip_char: already equipped (%d).", iWear );
	return;
    }

    if ( ( IS_OBJ_STAT(obj, ITEM_ANTI_EVIL)    && IS_EVIL(ch)    )
    ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_GOOD)    && IS_GOOD(ch)    )
    ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_NEUTRAL) && IS_NEUTRAL(ch) ) )
    {
	/*
	 * Thanks to Morgenes for the bug fix here!
	 */
   act( L"$p seni çarparak elinden düşüyor.", ch, obj, NULL, TO_CHAR );
 	act( L"$p $m çarparak elinden düşüyor.",  ch, obj, NULL, TO_ROOM );
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
	bug( L"Unequip_char: already unequipped.", 0 );
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
	    bug ( L"Norm-Apply: %d", 0 );
	    for ( lpaf = ch->affected; lpaf != NULL; lpaf = lpaf_next )
	    {
		lpaf_next = lpaf->next;
		if ((lpaf->type == paf->type) &&
		    (lpaf->level == paf->level) &&
		    (lpaf->location == APPLY_SPELL_AFFECT))
		{
		    bug ( L"location = %d", lpaf->location );
		    bug ( L"type = %d", lpaf->type );
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
	bug( L"Obj_from_obj: null obj_from.", 0 );
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
	    bug( L"Obj_from_obj: obj not found.", 0 );
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
    wchar_t buf[MAX_STRING_LENGTH];

    if (obj->extracted)  /* if the object has already been extracted once */
      {
        swprintf( buf, MAX_STRING_LENGTH-1, L"Warning! Extraction of %s, vnum %d.", obj->name,
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
     send_to_char( L"Ruhunu yakalıyorsun...\n\r",wch);
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
	    bug( L"Extract_obj: obj %d not found.", obj->pIndexData->vnum );
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
    wchar_t buf[MAX_STRING_LENGTH];

    if (fPull) /* only for total extractions should it check */
    {
    if (ch->extracted)  /* if the char has already been extracted once */
      {
        swprintf( buf, MAX_STRING_LENGTH-1, L"Warning! Extraction of %s.", ch->name);
        bug(buf, 0);
        return; /* if it's already been extracted, something bad is going on */
      }
    else
      ch->extracted = TRUE;  /* if it hasn't been extracted yet, now
                               * it's being extracted. */
    }


    if ( ch->in_room == NULL )
    {
	bug( L"Extract_char: NULL.", 0 );
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
	do_return( ch, (wchar_t*)"" );
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
	    bug( L"Extract_char: char not found.", 0 );
	    return;
	}
    }

    if ( ch->desc != NULL )
	ch->desc->character = NULL;
    free_char( ch );
    return;
}


/*
 * Find a char in the world.
 */
CHAR_DATA *get_char_world( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH];
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
OBJ_DATA *get_obj_list( CHAR_DATA *ch, wchar_t *argument, OBJ_DATA *list )
{
    wchar_t arg[MAX_INPUT_LENGTH];
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
OBJ_DATA *get_obj_carry( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH];
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
OBJ_DATA *get_obj_wear( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH];
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
OBJ_DATA *get_obj_here( CHAR_DATA *ch, wchar_t *argument )
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
OBJ_DATA *get_obj_world( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH];
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

    ch->silver -= cost;

    if (ch->silver < 0)
    {
	bug( L"deduct costs: silver %d < 0",ch->silver);
	ch->silver = 0;
    }
}
/*
 * Create a 'money' obj.
 */
OBJ_DATA *create_money( int silver )
{
    wchar_t buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;

    if ( silver < 0 || silver == 0 )
    {
	bug( L"Create_money: zero or negative money.",silver);
	silver = UMAX(1,silver);
    }

    if (silver == 1)
    {
	obj = create_object( get_obj_index( OBJ_VNUM_SILVER_ONE ), 0 );
    }
    else
    {
        obj = create_object( get_obj_index( OBJ_VNUM_SILVER_SOME ), 0 );
        swprintf( buf, MAX_STRING_LENGTH-1, obj->short_descr, silver );
        free_string( obj->short_descr );
        obj->short_descr        = str_dup( buf );
        obj->value[0]           = silver;
        obj->cost               = silver;
	obj->weight		= silver/20;
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
 * True if char can see victim.
 */
bool can_see( CHAR_DATA *ch, CHAR_DATA *victim )
{
/* RT changed so that WIZ_INVIS has levels */
    if ( ch == victim )
	return TRUE;

    if ( ch == NULL || victim == NULL )
	dump_to_scr( (wchar_t*)">>>>>>>> CAN_ SEE ERROR <<<<<<<<<<<\n\r" );

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
wchar_t *item_type_name( OBJ_DATA *obj )
{
    switch ( obj->item_type )
    {
    case ITEM_LIGHT:		return (wchar_t*)"light";
    case ITEM_SCROLL:		return (wchar_t*)"scroll";
    case ITEM_WAND:		return (wchar_t*)"wand";
    case ITEM_STAFF:		return (wchar_t*)"staff";
    case ITEM_WEAPON:		return (wchar_t*)"weapon";
    case ITEM_TREASURE:		return (wchar_t*)"treasure";
    case ITEM_ARMOR:		return (wchar_t*)"armor";
    case ITEM_CLOTHING:		return (wchar_t*)"clothing";
    case ITEM_POTION:		return (wchar_t*)"potion";
    case ITEM_FURNITURE:	return (wchar_t*)"furniture";
    case ITEM_TRASH:		return (wchar_t*)"trash";
    case ITEM_CONTAINER:	return (wchar_t*)"container";
    case ITEM_DRINK_CON:	return (wchar_t*)"drink container";
    case ITEM_KEY:		return (wchar_t*)"key";
    case ITEM_MAYMUNCUK:		return (wchar_t*)"maymuncuk";
    case ITEM_FOOD:		return (wchar_t*)"food";
    case ITEM_MONEY:		return (wchar_t*)"money";
    case ITEM_BOAT:		return (wchar_t*)"boat";
    case ITEM_CORPSE_NPC:	return (wchar_t*)"npc corpse";
    case ITEM_CORPSE_PC:	return (wchar_t*)"pc corpse";
    case ITEM_FOUNTAIN:		return (wchar_t*)"fountain";
    case ITEM_PILL:		return (wchar_t*)"pill";
    case ITEM_MAP:		return (wchar_t*)"map";
    case ITEM_PORTAL:		return (wchar_t*)"portal";
    case ITEM_WARP_STONE:	return (wchar_t*)"warp stone";
    case ITEM_GEM:		return (wchar_t*)"gem";
    case ITEM_JEWELRY:		return (wchar_t*)"jewelry";
    case ITEM_JUKEBOX:		return (wchar_t*)"juke box";
    case ITEM_TATTOO:		return (wchar_t*)"tattoo";
    }

    bug( L"Item_type_name: unknown type %d.", obj->item_type );
    return (wchar_t*)"(unknown)";
}



/*
 * Return ascii name of an affect location.
 */
wchar_t *affect_loc_name( int location )
{
    switch ( location )
    {
    case APPLY_NONE:		return (wchar_t*)"none";
    case APPLY_STR:		return (wchar_t*)"strength";
    case APPLY_DEX:		return (wchar_t*)"dexterity";
    case APPLY_INT:		return (wchar_t*)"intelligence";
    case APPLY_WIS:		return (wchar_t*)"wisdom";
    case APPLY_CON:		return (wchar_t*)"constitution";
    case APPLY_CHA:		return (wchar_t*)"charisma";
    case APPLY_CLASS:		return (wchar_t*)"class";
    case APPLY_LEVEL:		return (wchar_t*)"level";
    case APPLY_AGE:		return (wchar_t*)"age";
    case APPLY_MANA:		return (wchar_t*)"mana";
    case APPLY_HIT:		return (wchar_t*)"hp";
    case APPLY_MOVE:		return (wchar_t*)"moves";
    case APPLY_GOLD:		return (wchar_t*)"gold";
    case APPLY_EXP:		return (wchar_t*)"experience";
    case APPLY_AC:		return (wchar_t*)"armor class";
    case APPLY_HITROLL:		return (wchar_t*)"hit roll";
    case APPLY_DAMROLL:		return (wchar_t*)"damage roll";
    case APPLY_SIZE:		return (wchar_t*)"size";
    case APPLY_SAVES:		return (wchar_t*)"saves";
    case APPLY_SAVING_ROD:	return (wchar_t*)"save vs rod";
    case APPLY_SAVING_PETRI:	return (wchar_t*)"save vs petrification";
    case APPLY_SAVING_BREATH:	return (wchar_t*)"save vs breath";
    case APPLY_SAVING_SPELL:	return (wchar_t*)"save vs spell";
    case APPLY_SPELL_AFFECT:	return (wchar_t*)"none";
    }

    bug( L"Affect_location_name: unknown location %d.", location );
    return (wchar_t*)"(unknown)";
}



/*
 * Return ascii name of an affect bit vector.
 */
wchar_t *affect_bit_name( int vector )
{
    static wchar_t buf[512];

    buf[0] = '\0';
    if ( vector & AFF_BLIND         ) wcscat( buf, L" blind"         );
    if ( vector & AFF_INVISIBLE     ) wcscat( buf, L" invisible"     );
    if ( vector & AFF_IMP_INVIS     ) wcscat( buf, L" imp_invis"     );
    if ( vector & AFF_FADE	    ) wcscat( buf, L" fade"     	    );
    if ( vector & AFF_SCREAM	    ) wcscat( buf, L" scream"        );
    if ( vector & AFF_BLOODTHIRST   ) wcscat( buf, L" bloodthirst"   );
    if ( vector & AFF_STUN	    ) wcscat( buf, L" stun"   );
    if ( vector & AFF_SANCTUARY     ) wcscat( buf, L" sanctuary"     );
    if ( vector & AFF_FAERIE_FIRE   ) wcscat( buf, L" faerie_fire"   );
    if ( vector & AFF_INFRARED      ) wcscat( buf, L" infrared"      );
    if ( vector & AFF_CURSE         ) wcscat( buf, L" curse"         );
    if ( vector & AFF_POISON        ) wcscat( buf, L" poison"        );
    if ( vector & AFF_PROTECT_EVIL  ) wcscat( buf, L" prot_evil"     );
    if ( vector & AFF_PROTECT_GOOD  ) wcscat( buf, L" prot_good"     );
    if ( vector & AFF_SLEEP         ) wcscat( buf, L" sleep"         );
    if ( vector & AFF_SNEAK         ) wcscat( buf, L" sneak"         );
    if ( vector & AFF_HIDE          ) wcscat( buf, L" hide"          );
    if ( vector & AFF_CHARM         ) wcscat( buf, L" charm"         );
    if ( vector & AFF_FLYING        ) wcscat( buf, L" flying"        );
    if ( vector & AFF_PASS_DOOR     ) wcscat( buf, L" pass_door"     );
    if ( vector & AFF_BERSERK	    ) wcscat( buf, L" berserk"	    );
    if ( vector & AFF_CALM	    ) wcscat( buf, L" calm"	    );
    if ( vector & AFF_HASTE	    ) wcscat( buf, L" haste"	    );
    if ( vector & AFF_SLOW          ) wcscat( buf, L" slow"          );
    if ( vector & AFF_WEAKEN        ) wcscat( buf, L" weaken"        );
    if ( vector & AFF_PLAGUE	    ) wcscat( buf, L" plague" 	    );
    if ( vector & AFF_REGENERATION  ) wcscat( buf, L" regeneration"  );
    if ( vector & AFF_CAMOUFLAGE    ) wcscat( buf, L" camouflage"    );
    if ( vector & AFF_SWIM          ) wcscat( buf, L" swim"          );
    return (wchar_t *)(( buf[0] != '\0' ) ? buf+1 : L"none");
}


/*
 * Return ascii name of an affect bit vector.
 */
wchar_t *detect_bit_name( int vector )
{
    static wchar_t buf[512];

    buf[0] = '\0';
    if ( vector & DETECT_IMP_INVIS   ) wcscat( buf, L" detect_imp_inv"   );
    if ( vector & DETECT_EVIL   ) wcscat( buf, L" detect_evil"   );
    if ( vector & DETECT_GOOD   ) wcscat( buf, L" detect_good"   );
    if ( vector & DETECT_INVIS  ) wcscat( buf, L" detect_invis"  );
    if ( vector & DETECT_MAGIC  ) wcscat( buf, L" detect_magic"  );
    if ( vector & DETECT_HIDDEN ) wcscat( buf, L" detect_hidden" );
    if ( vector & DARK_VISION   ) wcscat( buf, L" dark_vision"   );
    if ( vector & ACUTE_VISION  ) wcscat( buf, L" acute_vision"   );
    if ( vector & ADET_FEAR  	) wcscat( buf, L" fear"   );
    if ( vector & ADET_FORM_TREE  ) wcscat( buf, L" form_tree"   );
    if ( vector & ADET_FORM_GRASS ) wcscat( buf, L" form_grass"   );
    if ( vector & ADET_WEB	) wcscat( buf, L" web"   );
    if ( vector & DETECT_LIFE 	) wcscat( buf, L" life"   );
    if ( vector & DETECT_SNEAK 	) wcscat( buf, L" detect_sneak"   );
    return (wchar_t *)(( buf[0] != '\0' ) ? buf+1 : L"none");
}



/*
 * Return ascii name of extra flags vector.
 */
wchar_t *extra_bit_name( int extra_flags )
{
    static wchar_t buf[512];

    buf[0] = '\0';
    if ( extra_flags & ITEM_GLOW         ) wcscat( buf, L" glow"         );
    if ( extra_flags & ITEM_HUM          ) wcscat( buf, L" hum"          );
    if ( extra_flags & ITEM_DARK         ) wcscat( buf, L" dark"         );
    if ( extra_flags & ITEM_LOCK         ) wcscat( buf, L" lock"         );
    if ( extra_flags & ITEM_EVIL         ) wcscat( buf, L" evil"         );
    if ( extra_flags & ITEM_INVIS        ) wcscat( buf, L" invis"        );
    if ( extra_flags & ITEM_MAGIC        ) wcscat( buf, L" magic"        );
    if ( extra_flags & ITEM_NODROP       ) wcscat( buf, L" nodrop"       );
    if ( extra_flags & ITEM_BLESS        ) wcscat( buf, L" bless"        );
    if ( extra_flags & ITEM_ANTI_GOOD    ) wcscat( buf, L" anti-good"    );
    if ( extra_flags & ITEM_ANTI_EVIL    ) wcscat( buf, L" anti-evil"    );
    if ( extra_flags & ITEM_ANTI_NEUTRAL ) wcscat( buf, L" anti-neutral" );
    if ( extra_flags & ITEM_NOREMOVE     ) wcscat( buf, L" noremove"     );
    if ( extra_flags & ITEM_INVENTORY    ) wcscat( buf, L" inventory"    );
    if ( extra_flags & ITEM_NOPURGE	 ) wcscat( buf, L" nopurge"	);
    if ( extra_flags & ITEM_VIS_DEATH	 ) wcscat( buf, L" vis_death"	);
    if ( extra_flags & ITEM_ROT_DEATH	 ) wcscat( buf, L" rot_death"	);
    if ( extra_flags & ITEM_NOLOCATE	 ) wcscat( buf, L" no_locate"	);
    if ( extra_flags & ITEM_SELL_EXTRACT ) wcscat( buf, L" sell_extract" );
    if ( extra_flags & ITEM_BURN_PROOF	 ) wcscat( buf, L" burn_proof"	);
    if ( extra_flags & ITEM_NOUNCURSE	 ) wcscat( buf, L" no_uncurse"	);
    if ( extra_flags & ITEM_BURIED	 ) wcscat( buf, L" buried"	);
    return (wchar_t *)(( buf[0] != '\0' ) ? buf+1 : L"none");
}

/* return ascii name of an act vector */
wchar_t *act_bit_name( int act_flags )
{
    static wchar_t buf[512];

    buf[0] = '\0';

    if (IS_SET(act_flags,ACT_IS_NPC))
    {
 	wcscat(buf,L" npc");
    	if (act_flags & ACT_SENTINEL 	) wcscat(buf, L" sentinel");
    	if (act_flags & ACT_SCAVENGER	) wcscat(buf, L" scavenger");
	if (act_flags & ACT_AGGRESSIVE	) wcscat(buf, L" aggressive");
	if (act_flags & ACT_STAY_AREA	) wcscat(buf, L" stay_area");
	if (act_flags & ACT_WIMPY	) wcscat(buf, L" wimpy");
	if (act_flags & ACT_PET		) wcscat(buf, L" pet");
	if (act_flags & ACT_TRAIN	) wcscat(buf, L" train");
	if (act_flags & ACT_PRACTICE	) wcscat(buf, L" practice");
	if (act_flags & ACT_UNDEAD	) wcscat(buf, L" undead");
	if (act_flags & ACT_HUNTER	) wcscat(buf, L" hunter");
	if (act_flags & ACT_CLERIC	) wcscat(buf, L" cleric");
	if (act_flags & ACT_MAGE	) wcscat(buf, L" mage");
	if (act_flags & ACT_THIEF	) wcscat(buf, L" thief");
	if (act_flags & ACT_WARRIOR	) wcscat(buf, L" warrior");
	if (act_flags & ACT_NOALIGN	) wcscat(buf, L" no_align");
	if (act_flags & ACT_NOPURGE	) wcscat(buf, L" no_purge");
	if (act_flags & ACT_IS_HEALER	) wcscat(buf, L" healer");
	if (act_flags & ACT_GAIN	) wcscat(buf, L" skill_train");
	if (act_flags & ACT_UPDATE_ALWAYS) wcscat(buf,L" update_always");
    }
    else
    {
	wcscat(buf,L" player");
	if (act_flags & PLR_AUTOASSIST	) wcscat(buf, L" autoassist");
	if (act_flags & PLR_AUTOEXIT	) wcscat(buf, L" autoexit");
	if (act_flags & PLR_AUTOLOOT	) wcscat(buf, L" autoloot");
	if (act_flags & PLR_AUTOSAC	) wcscat(buf, L" autosac");
	if (act_flags & PLR_AUTOAKCE	) wcscat(buf, L" autoakce");
	if (act_flags & PLR_AUTOSPLIT	) wcscat(buf, L" autosplit");
	if (act_flags & PLR_WANTED	) wcscat(buf, L" wanted");
	if (act_flags & PLR_NO_TITLE	) wcscat(buf, L" no_title");
	if (act_flags & PLR_NO_EXP	) wcscat(buf, L" no_exp");
	if (act_flags & PLR_HOLYLIGHT	) wcscat(buf, L" holy_light");
	if (act_flags & PLR_NOCANCEL	) wcscat(buf, L" no_cancel");
	if (act_flags & PLR_CANLOOT	) wcscat(buf, L" loot_corpse");
	if (act_flags & PLR_NOSUMMON	) wcscat(buf, L" no_summon");
	if (act_flags & PLR_NOFOLLOW	) wcscat(buf, L" no_follow");
	if (act_flags & PLR_CANINDUCT	) wcscat(buf, L" Cabal_LEADER");
	if (act_flags & PLR_GHOST	) wcscat(buf, L" ghost");
	if (act_flags & PLR_PERMIT	) wcscat(buf, L" permit");
	if (act_flags & PLR_REMORTED	) wcscat(buf, L" remorted");
	if (act_flags & PLR_LOG		) wcscat(buf, L" log");
	if (act_flags & PLR_FREEZE	) wcscat(buf, L" frozen");
	if (act_flags & PLR_LEFTHAND	) wcscat(buf, L" lefthand");
	if (act_flags & PLR_CANREMORT	) wcscat(buf, L" canremort");
	if (act_flags & PLR_QUESTOR	) wcscat(buf, L" questor");
	if (act_flags & PLR_VAMPIRE	) wcscat(buf, L" VAMPIRE");
	if (act_flags & PLR_HARA_KIRI	) wcscat(buf, L" harakiri");
	if (act_flags & PLR_BLINK_ON	) wcscat(buf, L" blink_on");
    }
    return (wchar_t *)(( buf[0] != '\0' ) ? buf+1 : L"none");
}

wchar_t *comm_bit_name(int comm_flags)
{
    static wchar_t buf[512];

    buf[0] = '\0';

    if (comm_flags & COMM_QUIET		) wcscat(buf, L" quiet");
    if (comm_flags & COMM_DEAF		) wcscat(buf, L" deaf");
    if (comm_flags & COMM_NOWIZ		) wcscat(buf, L" no_wiz");
    if (comm_flags & COMM_NOAUCTION	) wcscat(buf, L" no_auction");
    if (comm_flags & COMM_NOGOSSIP	) wcscat(buf, L" no_gossip");
    if (comm_flags & COMM_NOQUESTION	) wcscat(buf, L" no_question");
    if (comm_flags & COMM_NOMUSIC	) wcscat(buf, L" no_music");
    if (comm_flags & COMM_NOQUOTE	) wcscat(buf, L" no_quote");
    if (comm_flags & COMM_COMPACT	) wcscat(buf, L" compact");
    if (comm_flags & COMM_BRIEF		) wcscat(buf, L" brief");
    if (comm_flags & COMM_PROMPT	) wcscat(buf, L" prompt");
    if (comm_flags & COMM_COMBINE	) wcscat(buf, L" combine");
    if (comm_flags & COMM_NOEMOTE	) wcscat(buf, L" no_emote");
    if (comm_flags & COMM_NOSHOUT	) wcscat(buf, L" no_shout");
    if (comm_flags & COMM_NOTELL	) wcscat(buf, L" no_tell");
    if (comm_flags & COMM_NOCHANNELS	) wcscat(buf, L" no_channels");


    return (wchar_t *)(( buf[0] != '\0' ) ? buf+1 : L"none");
}

wchar_t *imm_bit_name(int imm_flags)
{
    static wchar_t buf[512];

    buf[0] = '\0';

    if (imm_flags & IMM_SUMMON		) wcscat(buf, L" summon");
    if (imm_flags & IMM_CHARM		) wcscat(buf, L" charm");
    if (imm_flags & IMM_MAGIC		) wcscat(buf, L" magic");
    if (imm_flags & IMM_WEAPON		) wcscat(buf, L" weapon");
    if (imm_flags & IMM_BASH		) wcscat(buf, L" blunt");
    if (imm_flags & IMM_PIERCE		) wcscat(buf, L" piercing");
    if (imm_flags & IMM_SLASH		) wcscat(buf, L" slashing");
    if (imm_flags & IMM_FIRE		) wcscat(buf, L" fire");
    if (imm_flags & IMM_COLD		) wcscat(buf, L" cold");
    if (imm_flags & IMM_LIGHTNING	) wcscat(buf, L" lightning");
    if (imm_flags & IMM_ACID		) wcscat(buf, L" acid");
    if (imm_flags & IMM_POISON		) wcscat(buf, L" poison");
    if (imm_flags & IMM_NEGATIVE	) wcscat(buf, L" negative");
    if (imm_flags & IMM_HOLY		) wcscat(buf, L" holy");
    if (imm_flags & IMM_ENERGY		) wcscat(buf, L" energy");
    if (imm_flags & IMM_MENTAL		) wcscat(buf, L" mental");
    if (imm_flags & IMM_DISEASE	) wcscat(buf, L" disease");
    if (imm_flags & IMM_DROWNING	) wcscat(buf, L" drowning");
    if (imm_flags & IMM_LIGHT		) wcscat(buf, L" light");
    if (imm_flags & VULN_IRON		) wcscat(buf, L" iron");
    if (imm_flags & VULN_WOOD		) wcscat(buf, L" wood");
    if (imm_flags & VULN_SILVER	) wcscat(buf, L" silver");

    return (wchar_t *)(( buf[0] != '\0' ) ? buf+1 : L"none");
}

wchar_t *wear_bit_name(int wear_flags)
{
    static wchar_t buf[512];

    buf [0] = '\0';
    if (wear_flags & ITEM_TAKE		) wcscat(buf, L" take");
    if (wear_flags & ITEM_WEAR_FINGER	) wcscat(buf, L" finger");
    if (wear_flags & ITEM_WEAR_NECK	) wcscat(buf, L" neck");
    if (wear_flags & ITEM_WEAR_BODY	) wcscat(buf, L" torso");
    if (wear_flags & ITEM_WEAR_HEAD	) wcscat(buf, L" head");
    if (wear_flags & ITEM_WEAR_LEGS	) wcscat(buf, L" legs");
    if (wear_flags & ITEM_WEAR_FEET	) wcscat(buf, L" feet");
    if (wear_flags & ITEM_WEAR_HANDS	) wcscat(buf, L" hands");
    if (wear_flags & ITEM_WEAR_ARMS	) wcscat(buf, L" arms");
    if (wear_flags & ITEM_WEAR_SHIELD	) wcscat(buf, L" shield");
    if (wear_flags & ITEM_WEAR_ABOUT	) wcscat(buf, L" body");
    if (wear_flags & ITEM_WEAR_WAIST	) wcscat(buf, L" waist");
    if (wear_flags & ITEM_WEAR_WRIST	) wcscat(buf, L" wrist");
    if (wear_flags & ITEM_WIELD		) wcscat(buf, L" wield");
    if (wear_flags & ITEM_HOLD		) wcscat(buf, L" hold");
    if (wear_flags & ITEM_WEAR_FLOAT	) wcscat(buf, L" float");
    if (wear_flags & ITEM_WEAR_TATTOO	) wcscat(buf, L" tattoo");

    return (wchar_t *)(( buf[0] != '\0' ) ? buf+1 : L"none");
}

wchar_t *form_bit_name(int form_flags)
{
    static wchar_t buf[512];

    buf[0] = '\0';
    if (form_flags & FORM_POISON	) wcscat(buf, L" poison");
    else if (form_flags & FORM_EDIBLE	) wcscat(buf, L" edible");
    if (form_flags & FORM_MAGICAL	) wcscat(buf, L" magical");
    if (form_flags & FORM_INSTANT_DECAY	) wcscat(buf, L" instant_rot");
    if (form_flags & FORM_OTHER		) wcscat(buf, L" other");
    if (form_flags & FORM_ANIMAL	) wcscat(buf, L" animal");
    if (form_flags & FORM_SENTIENT	) wcscat(buf, L" sentient");
    if (form_flags & FORM_UNDEAD	) wcscat(buf, L" undead");
    if (form_flags & FORM_CONSTRUCT	) wcscat(buf, L" construct");
    if (form_flags & FORM_MIST		) wcscat(buf, L" mist");
    if (form_flags & FORM_INTANGIBLE	) wcscat(buf, L" intangible");
    if (form_flags & FORM_BIPED		) wcscat(buf, L" biped");
    if (form_flags & FORM_CIREN	    ) wcscat(buf, L" ciren");
    if (form_flags & FORM_INSECT	) wcscat(buf, L" insect");
    if (form_flags & FORM_SPIDER	) wcscat(buf, L" spider");
    if (form_flags & FORM_CRUSTACEAN	) wcscat(buf, L" crustacean");
    if (form_flags & FORM_WORM		) wcscat(buf, L" worm");
    if (form_flags & FORM_BLOB		) wcscat(buf, L" blob");
    if (form_flags & FORM_MAMMAL	) wcscat(buf, L" mammal");
    if (form_flags & FORM_BIRD		) wcscat(buf, L" bird");
    if (form_flags & FORM_REPTILE	) wcscat(buf, L" reptile");
    if (form_flags & FORM_SNAKE		) wcscat(buf, L" snake");
    if (form_flags & FORM_DRAGON	) wcscat(buf, L" dragon");
    if (form_flags & FORM_AMPHIBIAN	) wcscat(buf, L" amphibian");
    if (form_flags & FORM_FISH		) wcscat(buf, L" fish");
    if (form_flags & FORM_COLD_BLOOD 	) wcscat(buf, L" cold_blooded");

    return (wchar_t *)(( buf[0] != '\0' ) ? buf+1 : L"none");
}

wchar_t *part_bit_name(int part_flags)
{
    static wchar_t buf[512];

    buf[0] = '\0';
    if (part_flags & PART_HEAD		) wcscat(buf, L" head");
    if (part_flags & PART_ARMS		) wcscat(buf, L" arms");
    if (part_flags & PART_LEGS		) wcscat(buf, L" legs");
    if (part_flags & PART_HEART		) wcscat(buf, L" heart");
    if (part_flags & PART_BRAINS	) wcscat(buf, L" brains");
    if (part_flags & PART_GUTS		) wcscat(buf, L" guts");
    if (part_flags & PART_HANDS		) wcscat(buf, L" hands");
    if (part_flags & PART_FEET		) wcscat(buf, L" feet");
    if (part_flags & PART_FINGERS	) wcscat(buf, L" fingers");
    if (part_flags & PART_EAR		) wcscat(buf, L" ears");
    if (part_flags & PART_EYE		) wcscat(buf, L" eyes");
    if (part_flags & PART_LONG_TONGUE	) wcscat(buf, L" long_tongue");
    if (part_flags & PART_EYESTALKS	) wcscat(buf, L" eyestalks");
    if (part_flags & PART_TENTACLES	) wcscat(buf, L" tentacles");
    if (part_flags & PART_FINS		) wcscat(buf, L" fins");
    if (part_flags & PART_WINGS		) wcscat(buf, L" wings");
    if (part_flags & PART_TAIL		) wcscat(buf, L" tail");
    if (part_flags & PART_CLAWS		) wcscat(buf, L" claws");
    if (part_flags & PART_FANGS		) wcscat(buf, L" fangs");
    if (part_flags & PART_HORNS		) wcscat(buf, L" horns");
    if (part_flags & PART_SCALES	) wcscat(buf, L" scales");

    return (wchar_t *)(( buf[0] != '\0' ) ? buf+1 : L"none");
}

wchar_t *weapon_bit_name(int weapon_flags)
{
    static wchar_t buf[512];

    buf[0] = '\0';
    if (weapon_flags & WEAPON_FLAMING	) wcscat(buf, L" flaming");
    if (weapon_flags & WEAPON_FROST	) wcscat(buf, L" frost");
    if (weapon_flags & WEAPON_VAMPIRIC	) wcscat(buf, L" vampiric");
    if (weapon_flags & WEAPON_SHARP	) wcscat(buf, L" sharp");
    if (weapon_flags & WEAPON_VORPAL	) wcscat(buf, L" vorpal");
    if (weapon_flags & WEAPON_TWO_HANDS ) wcscat(buf, L" two-handed");
    if (weapon_flags & WEAPON_SHOCKING 	) wcscat(buf, L" shocking");
    if (weapon_flags & WEAPON_POISON	) wcscat(buf, L" poison");
    if (weapon_flags & WEAPON_HOLY	) wcscat(buf, L" holy");

    return (wchar_t *)(( buf[0] != '\0' ) ? buf+1 : L"none");
}

wchar_t *cont_bit_name( int cont_flags)
{
    static wchar_t buf[512];

    buf[0] = '\0';

    if (cont_flags & CONT_CLOSEABLE	) wcscat(buf, L" closable");
    if (cont_flags & CONT_PICKPROOF	) wcscat(buf, L" pickproof");
    if (cont_flags & CONT_CLOSED	) wcscat(buf, L" closed");
    if (cont_flags & CONT_LOCKED	) wcscat(buf, L" locked");

    return (wchar_t *)(( buf[0] != '\0' ) ? buf+1 : L"none");
}


wchar_t *off_bit_name(int off_flags)
{
    static wchar_t buf[512];

    buf[0] = '\0';

    if (off_flags & OFF_AREA_ATTACK	) wcscat(buf, L" area attack");
    if (off_flags & OFF_BACKSTAB	) wcscat(buf, L" backstab");
    if (off_flags & OFF_BASH		) wcscat(buf, L" bash");
    if (off_flags & OFF_BERSERK		) wcscat(buf, L" berserk");
    if (off_flags & OFF_DISARM		) wcscat(buf, L" disarm");
    if (off_flags & OFF_DODGE		) wcscat(buf, L" dodge");
    if (off_flags & OFF_FADE		) wcscat(buf, L" fade");
    if (off_flags & OFF_FAST		) wcscat(buf, L" fast");
    if (off_flags & OFF_KICK		) wcscat(buf, L" kick");
    if (off_flags & OFF_KICK_DIRT	) wcscat(buf, L" kick_dirt");
    if (off_flags & OFF_PARRY		) wcscat(buf, L" parry");
    if (off_flags & OFF_RESCUE		) wcscat(buf, L" rescue");
    if (off_flags & OFF_TAIL		) wcscat(buf, L" tail");
    if (off_flags & OFF_TRIP		) wcscat(buf, L" trip");
    if (off_flags & OFF_CRUSH		) wcscat(buf, L" crush");
    if (off_flags & ASSIST_ALL		) wcscat(buf, L" assist_all");
    if (off_flags & ASSIST_ALIGN	) wcscat(buf, L" assist_align");
    if (off_flags & ASSIST_RACE		) wcscat(buf, L" assist_race");
    if (off_flags & ASSIST_PLAYERS	) wcscat(buf, L" assist_players");
    if (off_flags & ASSIST_GUARD	) wcscat(buf, L" assist_guard");
    if (off_flags & ASSIST_VNUM		) wcscat(buf, L" assist_vnum");

    return (wchar_t *)(( buf[0] != '\0' ) ? buf+1 : L"none");
}

int cabal_lookup (const wchar_t *argument)
{
   int cabal;

   for ( cabal = 0; cabal < MAX_CABAL; cabal++)
   {
        if (towlower(argument[0]) == towlower(cabal_table[cabal].short_name[0])
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
     send_to_char( L"Hı?\n\r",ch);
	 return 1;
   }
   return 0;
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
    send_to_char( L"Emrindekilerden fazlasını kontrol etmeye gücün yetmez!\n\r",ch);
    return count;
  }

  return 0;
}

void add_mind(CHAR_DATA *ch, wchar_t *str)
{
 wchar_t buf[MAX_STRING_LENGTH];

 if (!IS_NPC(ch) || ch->in_room == NULL) return;

 if (ch->in_mind == NULL)
  {
   swprintf( buf, MAX_STRING_LENGTH-1, L"%d",ch->in_room->vnum);
   ch->in_mind = str_dup(buf);
  }
 if (!is_name(str,ch->in_mind))
  {
    swprintf( buf, MAX_STRING_LENGTH-1, L"%s %s",ch->in_mind,str);
    free_string(ch->in_mind); ch->in_mind = str_dup(buf);
  }

}
void remove_mind(CHAR_DATA *ch, wchar_t *str)
{
 wchar_t buf[MAX_STRING_LENGTH];
 wchar_t buff[MAX_STRING_LENGTH];
 wchar_t arg[MAX_INPUT_LENGTH];
 wchar_t *mind = ch->in_mind;

 if (!IS_NPC(ch) || ch->in_room == NULL
	|| mind == NULL || !is_name(str,mind) ) return;

 buf[0] = '\0';
 do
  {
   mind = one_argument(mind,arg);
   if (!is_name(str,arg))
   {
    if (buf[0] == '\0') wcscpy(buff,arg);
    else swprintf(buff,wcslen(buff)-1, L"%s %s",buf,arg);
    wcscpy(buf,buff);
   }
  }
 while ( mind[0] != '\0' );

 do_say(ch,(wchar_t*)"Sonunda intikamımı aldım!");
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
 wchar_t arg[MAX_INPUT_LENGTH];

 if (!IS_NPC(ch) || ch->in_mind == NULL) return;

 one_argument(ch->in_mind,arg);
 if ( ( location = find_location( ch, arg ) ) == NULL )
  {
   bug( L"Mob cannot return to reset place",0);
   return;
  }

 if ( ch->fighting == NULL && location != ch->in_room )
  {
    act( L"$n nakil için dua ediyor.",ch,NULL,NULL,TO_ROOM);
   char_from_room( ch );
   char_to_room( ch, location );
   act( L"$n odada beliriyor.",ch,NULL,NULL,TO_ROOM);
   if (is_number(ch->in_mind))
   {
	free_string(ch->in_mind);
	ch->in_mind = NULL;
   }
 }
}

CHAR_DATA * find_char( CHAR_DATA *ch, wchar_t *argument,int door, int range )
{
 EXIT_DATA *pExit,*bExit;
 ROOM_INDEX_DATA *dest_room,*back_room;
 CHAR_DATA *target;
 int number = 0,opdoor;
 wchar_t arg[MAX_INPUT_LENGTH];

 number = number_argument(argument,arg);
 dest_room = ch->in_room;
 if ( (target = get_char_room2(ch,dest_room,arg,&number)) != NULL)
	return target;

 if ( (opdoor = opposite_door( door )) == -1)
  {
   bug( L"In find_char wrong door: %d",door);
   send_to_char( L"Onu orada görmüyorsun.\n\r",ch);
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
     send_to_char( L"Seçtiğin yol gücünün yoldan geçmesine izin vermiyor.\n\r",ch);
    return NULL;
   }
  if ((target = get_char_room2(ch,dest_room,arg,&number)) != NULL )
	return target;
 }

 send_to_char( L"Onu orada görmüyorsun.\n\r",ch);
 return NULL;
}

int check_exit( wchar_t *arg )
{
    int door = -1;

    if ( !wcscasecmp( arg, L"k" ) || !wcscasecmp( arg, L"kuzey" ) ) door = 0;
     else if ( !wcscasecmp( arg, L"d" ) || !wcscasecmp( arg, L"doğu"  ) ) door = 1;
     else if ( !wcscasecmp( arg, L"g" ) || !wcscasecmp( arg, L"güney" ) ) door = 2;
     else if ( !wcscasecmp( arg, L"b" ) || !wcscasecmp( arg, L"batı"  ) ) door = 3;
     else if ( !wcscasecmp( arg, L"y" ) || !wcscasecmp( arg, L"yukarı"    ) ) door = 4;
     else if ( !wcscasecmp( arg, L"a" ) || !wcscasecmp( arg, L"aşağı"  ) ) door = 5;

    return door;
}

/*
 * Find a char for spell usage.
 */
CHAR_DATA *get_char_spell( CHAR_DATA *ch, wchar_t *argument, int *door, int range )
{
 wchar_t buf[MAX_INPUT_LENGTH];
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
	 bug( L"In path_to_track wrong door: %d",door);
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
	bug( L"In path_to_track: couldn't calculate range %d",range);
	return;
       }
      if ( range > 100)
       {
	bug( L"In path_to_track: range exceeded 100",0);
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
	 swprintf(log_buf, (2*MAX_INPUT_LENGTH)-1, L"Path to track: Range: %d Room: %d opdoor:%d",
		range,temp->vnum,opdoor);
	 bug(log_buf,0);
	 return;
	}
       }
    do_track(victim,(wchar_t*)"");
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
    wchar_t *strtime;
    wchar_t mytime[MAX_INPUT_LENGTH];
    wchar_t tword[MAX_INPUT_LENGTH];
    int thour, tmin;

    strtime                    = (wchar_t*)ctime( &current_time );
    strtime[wcslen(strtime)-1] = '\0';

    strtime = one_argument( strtime, tword);
    strtime = one_argument( strtime, tword);
    strtime = one_argument( strtime, tword);
    strtime = one_argument( strtime, mytime);

    tword[0] = mytime[0];
    tword[1] = mytime[1];
    tword[2] = '\0';
    thour = wcstol( tword, 0, 10 );

    tword[0] = mytime[3];
    tword[1] = mytime[4];
    tword[2] = '\0';
    tmin = wcstol( tword, 0, 10 );

    if (thour == 23 && tmin > 54)
	return (59 - tmin);
    return 0;
}

int parse_date( time_t t)
{
    wchar_t *strtime;
    wchar_t day[MAX_INPUT_LENGTH];
    wchar_t mon[MAX_INPUT_LENGTH];
    int d;

    strtime                    = (wchar_t*)ctime( &t );
    strtime[wcslen(strtime)-1] = '\0';

    strtime = one_argument( strtime, mon);	/* temporary */
    strtime = one_argument( strtime, mon);
    strtime = one_argument( strtime, day);
    d = wcstol( day, 0, 10 );

    if (!wcscasecmp(mon, L"Jan"))	;
    else if (!wcscasecmp(mon, L"Feb"))
    {
	d += 31;
        if ( d > 59 ) d = 59;	/* discard 29th of February */
    }
    else if (!wcscasecmp(mon, L"Mar"))	d += 59;
    else if (!wcscasecmp(mon, L"Apr"))	d += 90;
    else if (!wcscasecmp(mon, L"May"))	d += 120;
    else if (!wcscasecmp(mon, L"Jun"))	d += 151;
    else if (!wcscasecmp(mon, L"Jul"))	d += 181;
    else if (!wcscasecmp(mon, L"Aug"))	d += 212;
    else if (!wcscasecmp(mon, L"Sep"))	d += 242;
    else if (!wcscasecmp(mon, L"Oct"))	d += 273;
    else if (!wcscasecmp(mon, L"Nov"))	d += 303;
    else if (!wcscasecmp(mon, L"Dec"))	d += 334;
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

bool room_has_exit( ROOM_INDEX_DATA *room )
{
  int i;

  if ( room == NULL )
    return FALSE;

  for(i=0;i<6;i++)
  {
    if (room->exit[i] != NULL)
      return TRUE;
  }

  return FALSE;
}
