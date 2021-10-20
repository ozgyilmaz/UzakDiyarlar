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
 *     ANATOLIA 2.1 is copyright 1996-1997 Serdar BULUT		           *
 *     ANATOLIA has been brought to you by ANATOLIA consortium		   *
 *	 Serdar BULUT {Chronos}		bulut@rorqual.cc.metu.edu.tr       *
 *	 Ibrahim Canpunar  {Mandrake}	canpunar@rorqual.cc.metu.edu.tr    *
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
#endif
#include <stdio.h>
#include <wchar.h>
#include <wctype.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "magic.h"

void	say_spell	args( ( CHAR_DATA *ch, int sn ) );
void	one_hit(CHAR_DATA *ch, CHAR_DATA *victim, int dt, bool second );
bool    mob_cast_mage   args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool    mob_cast_cleric args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );

/* command procedures needed */
DECLARE_DO_FUN(do_yell		);
DECLARE_DO_FUN(do_open		);
DECLARE_DO_FUN(do_close		);
DECLARE_DO_FUN(do_say		);
DECLARE_DO_FUN(do_backstab	);
DECLARE_DO_FUN(do_flee		);
DECLARE_DO_FUN(do_tell		);
DECLARE_DO_FUN(do_cb		);
DECLARE_DO_FUN(do_track		);
DECLARE_DO_FUN(do_murder	);
DECLARE_DO_FUN(do_kill		);
DECLARE_DO_FUN(do_unlock	);
DECLARE_DO_FUN(do_lock		);
DECLARE_DO_FUN(do_drop		);
DECLARE_DO_FUN(do_get		);
DECLARE_DO_FUN(do_north		);
DECLARE_DO_FUN(do_south		);
DECLARE_DO_FUN(do_east		);
DECLARE_DO_FUN(do_west		);
DECLARE_DO_FUN(do_sacrifice	);
DECLARE_DO_FUN(do_give		);
DECLARE_DO_FUN(do_rescue	);
DECLARE_DO_FUN(do_assassinate	);
DECLARE_DO_FUN(do_spellbane	);
DECLARE_DO_FUN(do_resistance	);
DECLARE_DO_FUN(do_bandage	);

/*
 * The following special functions are available for mobiles.
 */

DECLARE_SPEC_FUN(	spec_breath_any		);
DECLARE_SPEC_FUN(	spec_breath_acid	);
DECLARE_SPEC_FUN(	spec_breath_fire	);
DECLARE_SPEC_FUN(	spec_breath_frost	);
DECLARE_SPEC_FUN(	spec_breath_gas		);
DECLARE_SPEC_FUN(	spec_breath_lightning	);
DECLARE_SPEC_FUN(	spec_cast_adept		);
DECLARE_SPEC_FUN(	spec_cast_cleric	);
DECLARE_SPEC_FUN(	spec_cast_judge		);
DECLARE_SPEC_FUN(	spec_cast_mage		);
DECLARE_SPEC_FUN(	spec_cast_beholder	);
DECLARE_SPEC_FUN(	spec_cast_undead	);
DECLARE_SPEC_FUN(	spec_executioner	);
DECLARE_SPEC_FUN(	spec_fido		);
DECLARE_SPEC_FUN(	spec_guard		);
DECLARE_SPEC_FUN(	spec_janitor		);
DECLARE_SPEC_FUN(	spec_mayor		);
DECLARE_SPEC_FUN(	spec_poison		);
DECLARE_SPEC_FUN(	spec_thief		);
DECLARE_SPEC_FUN(	spec_nasty		);
DECLARE_SPEC_FUN(	spec_patrolman		);
DECLARE_SPEC_FUN(       spec_cast_cabal         );
DECLARE_SPEC_FUN(       spec_special_guard      );
DECLARE_SPEC_FUN(       spec_stalker            );
DECLARE_SPEC_FUN(       spec_questmaster        );
DECLARE_SPEC_FUN(       spec_assassinater       );
DECLARE_SPEC_FUN(       spec_repairman		);
DECLARE_SPEC_FUN(       spec_wishmaster		);
DECLARE_SPEC_FUN(       spec_kameni_dindar		);
DECLARE_SPEC_FUN(       spec_niryani_dindar		);
DECLARE_SPEC_FUN(       spec_nyahi_dindar		);
DECLARE_SPEC_FUN(       spec_sintaryan_dindar		);
DECLARE_SPEC_FUN(	spec_captain		);
DECLARE_SPEC_FUN(       spec_headlamia          );
/* cabal guardians */
DECLARE_SPEC_FUN(	spec_fight_enforcer	);
DECLARE_SPEC_FUN(	spec_fight_invader	);
DECLARE_SPEC_FUN(	spec_fight_ivan		);
DECLARE_SPEC_FUN(	spec_fight_seneschal	);
DECLARE_SPEC_FUN(	spec_fight_powerman	);
DECLARE_SPEC_FUN(	spec_fight_protector	);
DECLARE_SPEC_FUN(	spec_fight_hunter	);
DECLARE_SPEC_FUN(	spec_fight_lionguard	);


/* the function table */
const   struct  spec_type    spec_table[] =
{
    {	L"spec_breath_any",		spec_breath_any		},
    {	L"spec_breath_acid",		spec_breath_acid	},
    {	L"spec_breath_fire",		spec_breath_fire	},
    {	L"spec_breath_frost",		spec_breath_frost	},
    {	L"spec_breath_gas",		spec_breath_gas		},
    {	L"spec_breath_lightning",	spec_breath_lightning	},
    {	L"spec_cast_adept",		spec_cast_adept		},
    {	L"spec_cast_cleric",		spec_cast_cleric	},
    {	L"spec_cast_judge",		spec_cast_judge		},
    {	L"spec_cast_mage",		spec_cast_mage		},
    {	L"spec_cast_beholder",		spec_cast_beholder	},
    {	L"spec_cast_undead",		spec_cast_undead	},
    {	L"spec_executioner",		spec_executioner	},
    {	L"spec_fido",			spec_fido		},
    {	L"spec_guard",			spec_guard		},
    {	L"spec_janitor",			spec_janitor		},
    {	L"spec_mayor",			spec_mayor		},
    {	L"spec_poison",			spec_poison		},
    {	L"spec_thief",			spec_thief		},
    {	L"spec_nasty",			spec_nasty		},
    {	L"spec_patrolman",		spec_patrolman		},
    {	L"spec_cast_cabal",		spec_cast_cabal		},
    {	L"spec_stalker",			spec_stalker		},
    {	L"spec_special_guard",		spec_special_guard	},
    {	L"spec_questmaster",             spec_questmaster        },
    {	L"spec_assassinater",            spec_assassinater	},
    {	L"spec_repairman",		spec_repairman		},
    {	L"spec_wishmaster",		spec_wishmaster		},
    {	L"spec_kameni_dindar",		spec_kameni_dindar		},
    {	L"spec_niryani_dindar",		spec_niryani_dindar		},
    {	L"spec_nyahi_dindar",		spec_nyahi_dindar		},
    {	L"spec_sintaryan_dindar",		spec_sintaryan_dindar		},
    {	L"spec_captain",			spec_captain		},
    {	L"spec_headlamia",		spec_headlamia		},
    {	L"spec_fight_enforcer",		spec_fight_enforcer	},
    {	L"spec_fight_invader",		spec_fight_invader	},
    {	L"spec_fight_ivan",		spec_fight_ivan		},
    {	L"spec_fight_seneschal",		spec_fight_seneschal	},
    {	L"spec_fight_powerman",		spec_fight_powerman	},
    {	L"spec_fight_protector",		spec_fight_protector	},
    {	L"spec_fight_hunter",		spec_fight_hunter	},
    {	L"spec_fight_lionguard",		spec_fight_lionguard	},
    {	NULL,				NULL			}
};

/*
 * Given a name, return the appropriate spec fun.
 */
SPEC_FUN *spec_lookup( wchar_t *name )
{
   int i;

   for ( i = 0; spec_table[i].name != NULL; i++)
   {
        if (towlower(name[0]) == towlower(spec_table[i].name[0])
        &&  !str_prefix( name,spec_table[i].name))
            return spec_table[i].function;
   }

    return 0;
}

wchar_t *spec_name( SPEC_FUN *function)
{
    int i;

    for (i = 0; spec_table[i].function != NULL; i++)
    {
	if (function == spec_table[i].function)
	    return (wchar_t*)spec_table[i].name;
    }

    return NULL;
}


bool spec_patrolman(CHAR_DATA *ch)
{
    CHAR_DATA *vch,*victim = NULL;
/*    OBJ_DATA *obj; */
    const wchar_t *message;
    int count = 0;

    if (!IS_AWAKE(ch) || IS_AFFECTED(ch,AFF_CALM) || ch->in_room == NULL
    ||  IS_AFFECTED(ch,AFF_CHARM) || ch->fighting != NULL)
        return FALSE;

    /* look for a fight in the room */
    for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
    {
	if (vch == ch)
	    continue;

	if (vch->fighting != NULL)  /* break it up! */
	{
	    if (number_range(0,count) == 0)
	        victim = (vch->level > vch->fighting->level)
		    ? vch : vch->fighting;
	    count++;
	}
    }

    if (victim == NULL || (IS_NPC(victim) && victim->spec_fun == ch->spec_fun))
	return FALSE;
/*
    if (((obj = search_obj_char(ch,WEAR_NECK)) != NULL
    &&   obj->pIndexData->vnum == OBJ_VNUM_WHISTLE)
    ||  ((obj = get_eq_char(ch,WEAR_NECK_2)) != NULL
    &&   obj->pIndexData->vnum == OBJ_VNUM_WHISTLE))
    {
	act( L"You blow down hard on $p.",ch,obj,NULL,TO_CHAR);
	act( L"$n blows on $p, ***WHEEEEEEEEEEEET***",ch,obj,NULL,TO_ROOM);

    	for ( vch = char_list; vch != NULL; vch = vch->next )
    	{
            if ( vch->in_room == NULL )
            	continue;

            if (vch->in_room != ch->in_room
	    &&  vch->in_room->area == ch->in_room->area)
            	send_to_char( L"You hear a shrill whistling sound.\n\r", vch );
    	}
    }
*/
    switch (number_range(0,6))
    {
	default:	message = NULL;		break;
  case 0:	message = L"$n 'Hey hey heey, ayrılın!' diye bağırıyor";
		break;
	case 1: message = L"$n 'Sorumlu olan toplum, tek başıma ne yapabilirim ki?' dedi";
		break;
	case 2: message = L"$n 'Bu serseriler hepimizi mahvedecek.' diye mırıldanıyor";
		break;
	case 3: message = L"$n 'Durun! Durun!' diye bağırıyor ve saldırıyor.";
		break;
	case 4: message = L"$n sopasını eline alarak işe girişiyor.";
		break;
	case 5: message = L"$n iç çekerek kavgayı ayırmaya girişiyor.";
		break;
	case 6: message = L"$n 'Sakin olun, sizi fanatikler!' diyor.";
		break;
    }

    if (message != NULL)
	act(message,ch,NULL,NULL,TO_ALL);

    multi_hit(ch,victim,TYPE_UNDEFINED);

    return TRUE;
}

/*
 * Core procedure for dragons.
 */
bool dragon( CHAR_DATA *ch, wchar_t *spell_name )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    int sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

   for ( victim = ch->in_room->people; victim != NULL; victim = v_next)
    {
	v_next = victim->next_in_room;
	if ( ((RIDDEN(ch) && RIDDEN(ch)->fighting == victim)
		|| victim->fighting == ch)
	     && number_range(0,7) == 0 )
	    break;
    }

    if ( victim == NULL )
	return FALSE;

    if ( ( sn = skill_lookup( spell_name ) ) < 0 )
	return FALSE;

    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim, TARGET_CHAR);
    return TRUE;
}



/*
 * Special procedures for mobiles.
 */
bool spec_breath_any( CHAR_DATA *ch )
{
    if ( ch->position != POS_FIGHTING )
	return FALSE;

    switch ( number_range(0,7) )
    {
    case 0: return spec_breath_fire		( ch );
    case 1:
    case 2: return spec_breath_lightning	( ch );
    case 3: return spec_breath_gas		( ch );
    case 4: return spec_breath_acid		( ch );
    case 5:
    case 6:
    case 7: return spec_breath_frost		( ch );
    }

    return FALSE;
}



bool spec_breath_acid( CHAR_DATA *ch )
{
    return dragon( ch, (wchar_t*)"acid breath" );
}



bool spec_breath_fire( CHAR_DATA *ch )
{
    return dragon( ch, (wchar_t*)"fire breath" );
}



bool spec_breath_frost( CHAR_DATA *ch )
{
    return dragon( ch, (wchar_t*)"frost breath" );
}



bool spec_breath_gas( CHAR_DATA *ch )
{
    int sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    if ( ( sn = skill_lookup( L"gas breath" ) ) < 0 )
	return FALSE;
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, NULL,TARGET_CHAR);
    return TRUE;
}



bool spec_breath_lightning( CHAR_DATA *ch )
{
    return dragon( ch, (wchar_t*)"lightning breath" );
}



bool spec_cast_adept( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;

    if ( !IS_AWAKE(ch) )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim != ch && can_see( ch, victim ) && number_range(0,1) == 0
	     && !IS_NPC(victim) && victim->level < 11)
	    break;
    }

    if ( victim == NULL )
	return FALSE;

    switch ( number_range(0,15) )
    {
    case 0:
    act( L"$n sihirli sözcükler söylüyor 'abrazak'.", ch, NULL, NULL, TO_ROOM );
	spell_armor( skill_lookup( L"armor" ), ch->level,ch,victim,TARGET_CHAR);
	return TRUE;

    case 1:
    act( L"$n sihirli sözcükler söylüyor 'fido'.", ch, NULL, NULL, TO_ROOM );
	spell_bless( skill_lookup( L"bless" ), ch->level,ch,victim,TARGET_CHAR);
	return TRUE;

    case 2:
    act( L"$n sihirli sözcükler söylüyor 'judicandus noselacri'.",ch,NULL,NULL,TO_ROOM);
	spell_cure_blindness( skill_lookup( L"cure blindness" ),
	    ch->level, ch, victim,TARGET_CHAR);
	return TRUE;

    case 3:
    act( L"$n sihirli sözcükler söylüyor 'judicandus dies'.", ch,NULL, NULL, TO_ROOM );
	spell_cure_light( skill_lookup( L"cure light" ),
	    ch->level, ch, victim,TARGET_CHAR);
	return TRUE;

    case 4:
    act( L"$n sihirli sözcükler söylüyor 'judicandus sausabru'.",ch,NULL,NULL,TO_ROOM);
	spell_cure_poison( skill_lookup( L"cure poison" ),
	    ch->level, ch, victim,TARGET_CHAR);
	return TRUE;

    case 5:
    act( L"$n sihirli sözcükler söylüyor 'candusima'.", ch, NULL, NULL, TO_ROOM );
	spell_refresh( skill_lookup( L"refresh"),ch->level,ch,victim,TARGET_CHAR);
	return TRUE;

    case 6:
    act( L"$n sihirli sözcükler söylüyor 'judicandus eugzagz'.",ch,NULL,NULL,TO_ROOM);
	spell_cure_disease(skill_lookup( L"cure disease"),
	    ch->level,ch,victim,TARGET_CHAR);
    }

    return FALSE;
}



bool spec_cast_cleric( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_range(0,3) == 0 )
	    break;
    }

    if ( victim == NULL )
	return FALSE;

    mob_cast_cleric(ch, victim);
    return TRUE;
}

bool spec_cast_judge( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    const wchar_t *spell;
    int sn;

    if ( ch->position != POS_FIGHTING )
        return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
        v_next = victim->next_in_room;
        if ( victim->fighting == ch && number_range(0,3) == 0 )
            break;
    }

    if ( victim == NULL )
        return FALSE;

    spell = L"high explosive";
    if ( ( sn = skill_lookup( (wchar_t*)spell ) ) < 0 )
        return FALSE;
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim,TARGET_CHAR);
    return TRUE;
}



bool spec_cast_mage( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_range(0,3) == 0 )
	    break;
    }

    if ( victim == NULL )
	return FALSE;

    mob_cast_mage(ch, victim);
    return TRUE;
}



bool spec_cast_undead( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    const wchar_t *spell;
    int sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_range(0,3) == 0 )
	    break;
    }

    if ( victim == NULL )
	return FALSE;

    for ( ;; )
    {
	int min_level;

	switch ( number_range(0,15) )
	{
	case  0: min_level =  0; spell = L"curse";          break;
	case  1: min_level =  3; spell = L"weaken";         break;
	case  2: min_level =  6; spell = L"chill touch";    break;
	case  3: min_level =  9; spell = L"blindness";      break;
	case  4: min_level = 12; spell = L"poison";         break;
	case  5: min_level = 15; spell = L"energy drain";   break;
	case  6: min_level = 18; spell = L"harm";           break;
	case  7: min_level = 21; spell = L"teleport";       break;
	case  8: min_level = 20; spell = L"plague";	   break;
	default: min_level = 18; spell = L"harm";           break;
	}

	if ( ch->level >= min_level )
	    break;
    }

    if ( ( sn = skill_lookup( (wchar_t*)spell ) ) < 0 )
	return FALSE;
    say_spell(ch,sn);
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim,TARGET_CHAR);
    return TRUE;
}


bool spec_executioner( CHAR_DATA *ch )
{
    wchar_t buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    const wchar_t *crime;

    if ( !IS_AWAKE(ch) || ch->fighting != NULL )
	return FALSE;

    crime = L"";
    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;

	if ( !IS_NPC(victim) && IS_SET(victim->act, PLR_WANTED)
	&&   can_see(ch,victim))
	    { crime = L"SUÇLU"; break; }
    }

    if ( victim == NULL )
	return FALSE;

  swprintf( buf, MAX_STRING_LENGTH-1, L"%s bir %s!  MASUMU KORU!  KAAAANN!!!",victim->name, crime );
    REMOVE_BIT(ch->comm,COMM_NOSHOUT);
    do_yell( ch, buf );
    multi_hit( ch, victim, TYPE_UNDEFINED );
    return TRUE;
}



bool spec_fido( CHAR_DATA *ch )
{
    OBJ_DATA *corpse;
    OBJ_DATA *c_next;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;

    if ( !IS_AWAKE(ch) )
	return FALSE;

    for ( corpse = ch->in_room->contents; corpse != NULL; corpse = c_next )
    {
	c_next = corpse->next_content;
	if ( corpse->item_type != ITEM_CORPSE_NPC )
	    continue;

      act( L"$n cesedi vahşice parçalayıp yiyor.", ch, NULL, NULL, TO_ROOM );
	for ( obj = corpse->contains; obj; obj = obj_next )
	{
	    obj_next = obj->next_content;
	    obj_from_obj( obj );
	    obj_to_room( obj, ch->in_room );
	}
	extract_obj( corpse );
	return TRUE;
    }

    return FALSE;
}


bool spec_janitor( CHAR_DATA *ch )
{
    OBJ_DATA *trash;
    OBJ_DATA *trash_next;

    if ( !IS_AWAKE(ch) )
	return FALSE;

    for ( trash = ch->in_room->contents; trash != NULL; trash = trash_next )
    {
	trash_next = trash->next_content;
	if ( !IS_SET( trash->wear_flags, ITEM_TAKE ) || !can_loot(ch,trash))
	    continue;
	if ( trash->item_type == ITEM_DRINK_CON
	||   trash->item_type == ITEM_TRASH
	||   trash->cost < 10 )
	{
    act( L"$n çöpleri topluyor.", ch, NULL, NULL, TO_ROOM );
	    obj_from_room( trash );
	    obj_to_char( trash, ch );
	    if (IS_SET(trash->progtypes,OPROG_GET))
		(trash->pIndexData->oprogs->get_prog) (trash,ch);
	    return TRUE;
	}
    }

    return FALSE;
}



bool spec_mayor( CHAR_DATA *ch )
{
    static const wchar_t open_path[] = L"W3a3003b000c000d111Oe333333Oe22c222112212111a1S.";

    static const wchar_t close_path[] = L"W3a3003b000c000d111CE333333CE22c222112212111a1S.";

    static const wchar_t *path;
    static int pos;
    static bool move;
    OBJ_DATA *key;

    if ( !move )
    {
	if ( time_info.hour ==  6 )
	{
	    path = open_path;
	    move = TRUE;
	    pos  = 0;
	}

	if ( time_info.hour == 20 )
	{
	    path = close_path;
	    move = TRUE;
	    pos  = 0;
	}
    }

    if ( !move || ch->position < POS_SLEEPING )
	return FALSE;

    switch ( path[pos] )
    {
    case '0':
    case '1':
    case '2':
    case '3':
	move_char( ch, path[pos] - '0', FALSE );
	break;

    case 'W':
	ch->position = POS_STANDING;
  act( L"$n uyanıyor ve gürültülü bir inilti koparıyor.", ch, NULL, NULL, TO_ROOM );
	break;

    case 'S':
	ch->position = POS_SLEEPING;
  act( L"$n yatıp uyuyor.", ch, NULL, NULL, TO_ROOM );
	break;

    case 'a':
    do_say( ch,(wchar_t*) "Merhaba tatlım!" );
	break;

    case 'b':
    do_say(ch,(wchar_t*)"Şu rezilliğe bak! Çöpler konusunda birşey yapmalıyım!");
	break;

    case 'c':
    do_say(ch,(wchar_t*)"Gençlerde saygı denen şey kalmadı.");
	break;

    case 'd':
    do_say(ch,(wchar_t*)"İyi günler, vatandaşlar!");
	break;

    case 'e':
    do_say(ch,(wchar_t*)"Selenge şehir kapılarının açıldığını bildiririm.");
	break;

    case 'E':
    do_say(ch,(wchar_t*)"Selenge şehir kapılarının kapandığını bildiririm.");
	break;

    case 'O':
	do_unlock( ch, (wchar_t*)"kapı" );
	do_open( ch, (wchar_t*)"kapı" );
  interpret( ch, (wchar_t*)"duygu şehir kapısının kilidini açıyor.",FALSE);
	for( key=ch->in_room->contents; key!=NULL; key=key->next_content )
	  if ( key->pIndexData->vnum == 3379 )
	    break;
	if ( key != NULL )
	  SET_BIT( key->wear_flags, ITEM_TAKE );
	do_get( ch, (wchar_t*)"anahtar" );
	break;

    case 'C':
	do_close( ch, (wchar_t*)"kapı" );
	do_lock( ch, (wchar_t*)"kapı" );
	do_drop( ch, (wchar_t*)"anahtar" );
  interpret( ch, (wchar_t*)"duygu şehir kapısını zincirliyor.",FALSE);
	for( key=ch->in_room->contents; key!=NULL; key=key->next_content )
	  if ( key->pIndexData->vnum == 3379 )
	    break;
	if ( key != NULL )
	  REMOVE_BIT( key->wear_flags, ITEM_TAKE );
	break;

    case '.' :
	move = FALSE;
	break;
    }

    pos++;
    return FALSE;
}



bool spec_poison( CHAR_DATA *ch )
{
    CHAR_DATA *victim;

    if ( ch->position != POS_FIGHTING
    || ( victim = ch->fighting ) == NULL
    ||   number_percent( ) > 2 * ch->level )
	return FALSE;

  act( L"$M ısırıyorsun!",  ch, NULL, victim, TO_CHAR    );
  act( L"$n $M ısırıyor!",  ch, NULL, victim, TO_NOTVICT );
  act( L"$n seni ısırıyor!", ch, NULL, victim, TO_VICT    );
    spell_poison( gsn_poison, ch->level, ch, victim,TARGET_CHAR);
    return TRUE;
}



bool spec_thief( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    long silver;

    if ( ch->position != POS_STANDING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;

	if ( IS_NPC(victim)
	||   victim->level >= LEVEL_IMMORTAL
	||   number_range(0,31) != 0
	||   !can_see(ch,victim))
	    continue;

	if ( IS_AWAKE(victim) && number_range( 0, ch->level ) == 0 )
	{
    act( L"$s ellerinin para keseni kurcaladığını farkediyorsun!",
  ch, NULL, victim, TO_VICT );
    act( L"$N $s ellerinin para kesesini kurcaladığını farkediyor!",
		ch, NULL, victim, TO_NOTVICT );
	    return TRUE;
	}
	else
	{
	    silver = victim->silver * UMIN(number_range(1,20),ch->level/2)/100;
	    silver = UMIN(silver,ch->level*ch->level * 25);
	    ch->silver	+= silver;
	    victim->silver -= silver;
	    return TRUE;
	}
    }

    return FALSE;
}


bool spec_cast_cabal( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;

    if ( !IS_AWAKE(ch) )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim != ch && can_see( ch, victim ) && number_range(0,1) == 0)
	  break;
    }

    if ( victim == NULL )
	return FALSE;


    switch ( number_range(0,15) )
    {
    case 0:
    act( L"$n sihirli sözcükler söylüyor 'abracal'.", ch, NULL, NULL, TO_ROOM );
	spell_armor( skill_lookup( L"armor" ), ch->level, ch, victim, TARGET_CHAR );
	return TRUE;

    case 1:
    act( L"$n sihirli sözcükler söylüyor 'balc'.", ch, NULL, NULL, TO_ROOM );
	spell_bless( skill_lookup( L"bless" ), ch->level, ch, victim, TARGET_CHAR );
	return TRUE;

    case 2:
    act( L"$n sihirli sözcükler söylüyor 'judicandus noselacba'.", ch, NULL, NULL, TO_ROOM );
	spell_cure_blindness( skill_lookup( L"cure blindness" ),
	    ch->level, ch, victim, TARGET_CHAR );
	return TRUE;

    case 3:
    act( L"$n sihirli sözcükler söylüyor 'judicandus bacla'.", ch, NULL, NULL,TO_ROOM);
	spell_cure_light( skill_lookup( L"cure light" ),
	    ch->level, ch, victim, TARGET_CHAR );
	return TRUE;

    case 4:
    act( L"$n sihirli sözcükler söylüyor 'judicandus sausabcla'.",
	    ch, NULL, NULL, TO_ROOM );
	spell_cure_poison( skill_lookup( L"cure poison" ),
	    ch->level, ch, victim, TARGET_CHAR );
	return TRUE;

    case 5:
    act( L"$n sihirli sözcükler söylüyor 'candabala'.", ch, NULL, NULL, TO_ROOM );
	spell_refresh( skill_lookup( L"refresh" ), ch->level, ch, victim,
TARGET_CHAR );
	return TRUE;

    }

    return FALSE;
}



bool spec_guard( CHAR_DATA *ch )
{
    wchar_t buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    CHAR_DATA *ech;
    const wchar_t *crime;

    if ( !IS_AWAKE(ch) || ch->fighting != NULL )
      return FALSE;

    ech      = NULL;
    crime    = L"";

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
      {
	v_next = victim->next_in_room;

        if (!can_see(ch,victim)) continue;

	if (IS_SET(ch->in_room->area->area_flag,AREA_HOMETOWN)
		&& number_percent() < 2 )
	 {
     do_say( ch, (wchar_t*)"Seni tanıyor muyum?");
 	  if (wcscasecmp(ch->in_room->area->name,
		hometown_table[victim->hometown].name) )
	   {
       do_say( ch,(wchar_t*)"Seni hatırlamıyorum. Uzaklaş!");
	   }
	  else
	   {
       do_say(ch, (wchar_t*)"Tamam, şimdi hatırladım.");
		 interpret( ch, (wchar_t*)"gülümse",FALSE);
	   }
	 }

	if ( !IS_NPC(victim) && IS_SET(victim->act, PLR_WANTED) )
	  { crime = L"SUÇLU"; break; }

	if ( victim->fighting != NULL
	     && victim->fighting != ch
	     && victim->ethos != 1
	     && !IS_GOOD(victim)
	     && !IS_EVIL(victim->fighting) )
	  { ech = victim; victim=NULL; break; }
      }

    if ( victim != NULL )
      {
        swprintf( buf, MAX_STRING_LENGTH-1, L"%s bir %s!  MASUMU KORU!  SALDIR!!!",victim->name, crime );
	do_yell( ch, buf );
	multi_hit( ch, victim, TYPE_UNDEFINED );
	return TRUE;
      }

    if ( ech != NULL )
      {
        act( L"$n bağırıyor 'MASUMU KORU!!  SALDIR!!!'",
			ch, NULL, NULL, TO_ROOM );
	multi_hit( ch, ech, TYPE_UNDEFINED );
	return TRUE;
      }

    return FALSE;
  }




bool spec_special_guard( CHAR_DATA *ch )
{
    wchar_t buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim, *ech;
    CHAR_DATA *v_next;
    const wchar_t *crime;

    if ( !IS_AWAKE(ch) || ch->fighting != NULL )
	return FALSE;

    crime    = L"";
    ech      = NULL;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;

        if (!can_see(ch,victim)) continue;

	if ( !IS_NPC(victim) && IS_SET(victim->act, PLR_WANTED) )
	  { crime = L"SUÇLU"; break; }

	if ( victim->fighting != NULL
	     && victim->fighting != ch
	     && victim->fighting->cabal == CABAL_RULER )
	  { ech = victim; victim=NULL; break; }
    }

    if ( victim != NULL )
    {
      swprintf( buf, MAX_STRING_LENGTH-1, L"%s bir %s!  MASUMU KORU!  SALDIR!!!",victim->name, crime );

	do_yell( ch, buf );
	multi_hit( ch, victim, TYPE_UNDEFINED );
	return TRUE;
    }

    if ( ech != NULL )
    {
      act( L"$n bağırıyor 'MASUMU KORU!  SALDIR!!!'",
			ch, NULL, NULL, TO_ROOM );
	multi_hit( ch, ech, TYPE_UNDEFINED );
	return TRUE;
    }

    return FALSE;
}

bool spec_stalker(CHAR_DATA *ch)
{
  wchar_t buf[MAX_STRING_LENGTH];
  CHAR_DATA *victim;
  CHAR_DATA *wch;
  CHAR_DATA *wch_next;
  int i;

  victim = ch->last_fought;

  if (ch->fighting != NULL)
    return FALSE;

  if (ch->status == 10)
    {
      ch->cabal = CABAL_RULER;
      do_cb(ch, (wchar_t*)"Kurbanımı öldürdüğüme göre geldiğim yere dönebilirim.");
      extract_char(ch, TRUE);
      return TRUE;
    }

  if (victim == NULL)
    {
      ch->cabal = CABAL_RULER;
      do_cb(ch,(wchar_t*) "Kurbanım oyundan ayrıldı. Bu benim gitme vaktimin geldiğinin de göstergesi.");
      extract_char(ch, TRUE);
      return TRUE;
    }

  if (IS_GOOD(victim))
    i = 0;
  if (IS_EVIL(victim))
    i = 2;
  else
    i = 1;

  for (wch = ch->in_room->people; wch != NULL; wch = wch_next)
    {
     wch_next = wch->next_in_room;
     if (victim == wch)
      {
        swprintf( buf, MAX_STRING_LENGTH-1, L"Sen bir suçlusun %s! Ve öleceksin!!!", victim->name);
       do_yell(ch,buf);
       multi_hit(ch,wch,TYPE_UNDEFINED);
       return TRUE;
      }
  }
  do_track(ch, victim->name);

  if (ch->status == 5)
    {
      if (ch->in_room !=
get_room_index(hometown_table[victim->hometown].recall[1]))
	{
      	  char_from_room(ch);
      	  char_to_room(ch,
		   get_room_index(hometown_table[victim->hometown].recall[i]));
      	  do_track(ch, victim->name);
      	  return TRUE;
    	}
       else
    	{
      	  ch->cabal = CABAL_RULER;
          swprintf( buf, MAX_STRING_LENGTH-1, L"Üzgünüm ama %s izini kaybettirdi. Gitmeliyim.",
	      victim->name);
      	  do_cb(ch, buf);
      	  extract_char(ch, TRUE);
      	  return TRUE;
    	}
    }
  return FALSE;
}

bool spec_nasty( CHAR_DATA *ch )
{
    CHAR_DATA *victim, *v_next;
    long silver;

    if (!IS_AWAKE(ch)) {
       return FALSE;
    }

    if (ch->position != POS_FIGHTING) {
       for ( victim = ch->in_room->people; victim != NULL; victim = v_next)
       {
          v_next = victim->next_in_room;
          if (!IS_NPC(victim)
             && (victim->level > ch->level)
             && (victim->level < ch->level + 10))
          {
	     do_backstab(ch,victim->name);
             if (ch->position != POS_FIGHTING)
                 do_murder(ch,victim->name);
             /* should steal some coins right away? :) */
             return TRUE;
          }
       }
       return FALSE;    /*  No one to attack */
    }

    /* okay, we must be fighting.... steal some coins and flee */
    if ( (victim = ch->fighting) == NULL)
        return FALSE;   /* let's be paranoid.... */

    switch ( number_range(0,3) )
    {
      case 0:  act( L"$n para keseni yırtıp atarak paranın saçılmasına neden oluyor!",
                   ch, NULL, victim, TO_VICT);
               act( L"$S para kesesini yırtıyor ve akçelerini topluyorsun.",
                   ch, NULL, victim, TO_CHAR);
               act( L"$S para kesesi yırtılıyor!",
                     ch, NULL, victim, TO_NOTVICT);
                 silver = victim->silver / 2;  /* steal half of his silver */
                 victim->silver -= silver;
                 ch->silver     += silver;
                 return TRUE;

        case 1:  do_flee( ch, (wchar_t*)"");
                 return TRUE;

        default: return FALSE;
    }
}

bool spec_questmaster( CHAR_DATA *ch )
{
    if ( !IS_AWAKE(ch) )
        return FALSE;
    if (number_range(0,100) == 0) {
      do_say(ch, (wchar_t*)"Bir görev istemez misin?");
        return TRUE;
    }
    return FALSE;
}

bool spec_assassinater( CHAR_DATA *ch )
{
    wchar_t buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
         int rnd_say;

    if ( ch->fighting != NULL )
                return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
               /* this should kill mobs as well as players */
	v_next = ch->next_in_room;
        if ((victim->iclass != CLASS_THIEF) && (victim->iclass != CLASS_NINJA))
 		/* thieves & ninjas*/
        break;
    }

    if ( victim == NULL || victim == ch || IS_IMMORTAL(victim) )
        return FALSE;
    if ( victim->level > ch->level + 7 || IS_NPC(victim))
        return FALSE;
   if (victim->hit < victim->max_hit)
	return FALSE;

   rnd_say = number_range (1, 40);

   switch (rnd_say)
	{
    case  5:
           swprintf( buf, MAX_STRING_LENGTH-1, L"Ölüm tek sondur...");
     break;
  case  6:
           swprintf( buf, MAX_STRING_LENGTH-1, L"Ölme zamanı....");
     break;
  case  7:
           swprintf( buf, MAX_STRING_LENGTH-1, L"Ölme zamanı....");
     break;
  case  8:
           swprintf( buf, MAX_STRING_LENGTH-1, L"Kaderin seni bekliyor....");
     break;
  case  9:
     swprintf( buf, MAX_STRING_LENGTH-1, L"Ölümsüzlere bir kurban.... ");
     break;
  case 10:
           swprintf( buf, MAX_STRING_LENGTH-1, L"Hiç şeytanla raks ettin mi....");
	    break;
	 default:
	   return FALSE;
	}
    do_say( ch, buf );
    multi_hit( ch, victim ,gsn_assassinate );
    return TRUE;
}


bool spec_repairman( CHAR_DATA *ch )
{
    if ( !IS_AWAKE(ch) )
        return FALSE;
    if (number_range(0,100) == 0)
    {
      do_say(ch, (wchar_t*)"Şimdi diğer ekipmanları tamir etme zamanı.");
        return TRUE;
    }
    return FALSE;
}

bool spec_captain( CHAR_DATA *ch )
{

    static const wchar_t open_path[] = L"Wn0onc0oe1f2212211s2tw3xw3xd3322a22b22yO00d00a0011e1fe1fn0o3300300w3xs2ts2tS.";

    static const wchar_t close_path[] = L"Wn0on0oe1f2212211s2twc3xw3x3322d22a22EC0a00d0b0011e1fe1fn0o3300300w3xs2ts2tS.";

    static const wchar_t *path;
    static int pos;
    static bool move;

    if ( !move )
    {
	if ( time_info.hour ==  6 )
	{
	    path = open_path;
	    move = TRUE;
	    pos  = 0;
	}

	if ( time_info.hour == 20 )
	{
	    path = close_path;
	    move = TRUE;
	    pos  = 0;
	}
    }

    if ( ch->fighting != NULL )
	return spec_cast_cleric( ch );

    if ( !move || ch->position < POS_SLEEPING )
	return FALSE;

    switch ( path[pos] )
    {
    case '0':
    case '1':
    case '2':
    case '3':
	move_char( ch, path[pos] - '0' ,FALSE);
	break;

    case 'W':
	ch->position = POS_STANDING;
  act_color( L"$C$n uyanıyor ve esniyor.$c", ch, NULL, NULL, TO_ROOM,
POS_RESTING,CLR_WHITE );
	break;

    case 'S':
	ch->position = POS_SLEEPING;
  act_color( L"$C$n yatıp uyumaya başlıyor.$c", ch, NULL, NULL, TO_ROOM,
POS_RESTING,CLR_WHITE );
	break;

    case 'a':
    act_color( L"$C$n 'Selamlar! İyi avlar dilerim!' dedi$c", ch, NULL,
        NULL,TO_ROOM,POS_RESTING, CLR_YELLOW );
	break;

    case 'b':
    act_color( L"$C$n 'Caddeleri temiz tutalım ki Solace temiz olsun,' dedi.$c",ch,NULL, NULL, TO_ROOM, POS_RESTING,CLR_YELLOW );
	break;

    case 'c':
    act_color( L"$C$n 'Bu kapılara bir çare bulmalıyım,' dedi.$c",ch,
    NULL, NULL, TO_ROOM, POS_RESTING,CLR_YELLOW );
    act_color( L"$C$n 'Buradan hiç ayrılmayacağım,' dedi.$c", ch, NULL, NULL,
        TO_ROOM,POS_RESTING,CLR_YELLOW );
	break;

    case 'd':
    act_color( L"$C$n says 'Selamlar Solace vatadaşları!' dedi.$c", ch, NULL, NULL,
        TO_ROOM, POS_RESTING,CLR_YELLOW );
	break;

    case 'y':
    act_color( L"$C$n 'Solace şehrinin açıldığını bildiririm!' dedi.$c", ch,
NULL, NULL, TO_ROOM, POS_RESTING,CLR_YELLOW );
	break;

    case 'E':
    act_color( L"$C$n 'Solace şehrinin kapandığını deklare ederim!' dedi.$c", ch,
NULL, NULL, TO_ROOM, POS_RESTING,CLR_YELLOW );
	break;

    case 'O':
	do_unlock( ch, (wchar_t*)"kapı" );
	do_open( ch, (wchar_t*)"kapı" );
	break;

    case 'C':
	do_close( ch, (wchar_t*)"kapı" );
	do_lock( ch, (wchar_t*)"kapı" );
	break;

    case 'n':
	do_open( ch, (wchar_t*)"kuzey" );
	break;

    case 'o':
        do_close( ch, (wchar_t*)"güney" );
        break;

    case 's':
	do_open( ch, (wchar_t*)"güney" );
	break;

    case 't':
        do_close( ch, (wchar_t*)"kuzey" );
        break;

    case 'e':
	do_open( ch, (wchar_t*)"doğu" );
	break;

    case 'f':
        do_close( ch, (wchar_t*)"batı" );
        break;

    case 'w':
	do_open( ch, (wchar_t*)"batı" );
	break;

    case 'x':
        do_close( ch, (wchar_t*)"doğu" );
        break;

    case '.' :
	move = FALSE;
	break;
    }

    pos++;
    return FALSE;
}

bool spec_headlamia( CHAR_DATA *ch )
{
    static const wchar_t path[] = L"T111111100003332222232211.";
    static int pos=0;
    static bool move;
    static int count=0;
    CHAR_DATA *vch, *vch_next;

    if ( !move )
    {
      if (count++ == 10000) move=1;
    }

    if (  ch->position < POS_SLEEPING || ch->fighting )
	return FALSE;

    for(vch=ch->in_room->people;vch;vch=vch->next_in_room)
	{
	 if (!IS_NPC(vch) && vch->pIndexData->vnum == 3143)
	  {
	   do_kill(ch,vch->name);
	   break;
	  }
	}

    if (!move) return FALSE;

    switch ( path[pos] )
    {
    case '0':
    case '1':
    case '2':
    case '3':
	move_char( ch, path[pos] - '0', FALSE );
	pos++;
	break;

    case 'T':
	pos++;
	for(vch=char_list;vch;vch=vch_next)
	 {
	   vch_next = vch->next;
	   if (!IS_NPC(vch)) continue;
	   if (vch->pIndexData->vnum == 5201)
	    {
	     if (!vch->fighting && !vch->last_fought)
		{
		 char_from_room( vch );
		 char_to_room( vch , ch->in_room );
		 vch->master = ch;
		 vch->leader = ch;
		}
	    }
	 }
	break;

    case '.' :
	move = FALSE;
	count = 0;
	pos = 0;
	break;
    }

    return FALSE;
}


bool spec_cast_beholder( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    const wchar_t *spell;
    int sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_range(0,1) == 0 )
	    break;
    }


    if ( victim == NULL )
	return FALSE;

        switch ( dice(1,16) )
	{
	case  0: spell = L"fear";    break;
	case  1: spell = L"fear";    break;
	case  2: spell = L"slow";    break;
	case  3: spell = L"cause serious";       break;
	case  4: spell = L"cause critical";      break;
	case  5: spell = L"harm";   break;
	case  6: spell = L"harm";   break;
	case  7: spell = L"dispel magic";   break;
	case  8: spell = L"dispel magic";   break;
	default: spell = L"";     break;
	}

    if ( ( sn = skill_lookup( (wchar_t*)spell ) ) < 0 )
	return FALSE;
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim,TARGET_CHAR);
    return TRUE;
}



bool spec_fight_enforcer( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    const wchar_t *spell;
    int sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_range(0,1) == 0 )
	    break;
    }


    if ( victim == NULL )
	return FALSE;

        switch ( dice(1,16) )
	{
	case  0:
	case  1: spell = L"dispel magic";    break;
	case  2:
	case  3: spell = L"acid arrow";   break;
	case  4:
	case  5: spell = L"caustic font";     break;
	case  6:
	case  7:
	case  8:
	case  9:
	case 10: spell = L"acid blast";       break;
	default: spell = L"";     break;
	}

    if ( ( sn = skill_lookup( (wchar_t*)spell ) ) < 0 )
	return FALSE;
    say_spell(ch,sn);
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim,TARGET_CHAR);
    return TRUE;
}


bool spec_fight_invader( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    const wchar_t *spell;
    int sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_range(0,1) == 0 )
	    break;
    }


    if ( victim == NULL )
	return FALSE;

        switch ( dice(1,16) )
	{
	case  0:
	case  1: spell = L"blindness";      break;
	case  2:
	case  3: spell = L"dispel magic";    break;
	case  4:
	case  5: spell = L"weaken";         break;
	case  6:
	case  7: spell = L"energy drain"; break;
	case  8:
	case  9: spell = L"plague";   break;
	case 10:
	case 11: spell = L"acid arrow";   break;
	case 12:
	case 13:
	case 14: spell = L"acid blast";       break;
	case 15:
		if ( ch->hit < (ch->max_hit / 3))
		  spell = L"shadow cloak";
		else spell = L"";
		break;
	default: spell = L"";     break;
	}

    if ( ( sn = skill_lookup( (wchar_t*)spell ) ) < 0 )
	return FALSE;
    say_spell(ch,sn);
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim,TARGET_CHAR);
    return TRUE;
}


bool spec_fight_ivan( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    const wchar_t *spell;
    int sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_range(0,1) == 0 )
	    break;
    }


    if ( victim == NULL )
	return FALSE;

        switch ( dice(1,16) )
	{
	case  0:
	case  1: spell = L"dispel magic";    break;
	case  2:
	case  3: spell = L"acid arrow";   break;
	case  4:
	case  5: spell = L"caustic font";     break;
	case  6:
	case  7:
	case  8: spell = L"acid blast";       break;
	case  9: spell = L"disgrace";	break;
	case 10:
		if ( ch->hit < (ch->max_hit / 3))
		  spell = L"garble";
		else spell = L"";
		break;
	default: spell = L"";     break;
	}

    if ( ( sn = skill_lookup( (wchar_t*)spell ) ) < 0 )
	return FALSE;
    say_spell(ch,sn);
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim,TARGET_CHAR);
    return TRUE;
}


bool spec_fight_seneschal( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    const wchar_t *spell;
    int sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_range(0,1) == 0 )
	    break;
    }


    if ( victim == NULL )
	return FALSE;

        switch ( dice(1,16) )
	{
	case  0: spell = L"blindness";      break;
	case  1: spell = L"dispel magic";    break;
	case  2: spell = L"weaken";         break;
	case  3: spell = L"blindness";      break;
	case  4: spell = L"acid arrow";   break;
	case  5: spell = L"caustic font";     break;
	case  6: spell = L"energy drain";   break;
	case  7:
	case  8:
	case  9: spell = L"acid blast";       break;
	case 10: spell = L"plague";	   break;
	case 11: spell = L"acid blast";         break;
	case 12:
	case 13: spell = L"lightning breath";  break;
	case 14:
	case 15: spell = L"mental knife";	break;
	default: spell = L"";     break;
	}

    if ( ( sn = skill_lookup( (wchar_t*)spell ) ) < 0 )
	return FALSE;
    say_spell(ch,sn);
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim,TARGET_CHAR);
    return TRUE;
}


bool spec_fight_powerman( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    ch->cabal = CABAL_BATTLE;

    if ( !is_affected(ch, gsn_spellbane))
	do_spellbane(ch, (wchar_t*)"");

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_range(0,1) == 0 )
	    break;
    }


    if ( victim == NULL )
	return FALSE;

    if ( number_percent() < 33)
    {
      act( L"Ölümcül üçlü saldırıyı yapıyorsun!",ch,NULL,NULL,TO_CHAR);
    	act( L"$n ölümcül üçlü saldırıyı yapıyor!",ch,NULL,NULL,TO_ROOM);
	one_hit( ch, victim, TYPE_UNDEFINED, FALSE );
	one_hit( ch, victim, TYPE_UNDEFINED, FALSE );
	one_hit( ch, victim, TYPE_UNDEFINED, FALSE );
    }

    if ( !is_affected(ch, gsn_resistance))
	do_resistance(ch, (wchar_t*)"");

    if ( ch->hit < (ch->max_hit /3) && !IS_AFFECTED(ch, AFF_REGENERATION))
	do_bandage(ch, (wchar_t*)"");

    return TRUE;
}


bool spec_fight_protector( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    const wchar_t *spell;
    int sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_range(0,1) == 0 )
	    break;
    }


    if ( victim == NULL )
	return FALSE;

        switch ( dice(1,16) )
	{
	case  0:
	case  1: spell = L"dispel magic";    break;
	case  2:
	case  3: spell = L"acid arrow";   break;
	case  4:
	case  5: spell = L"caustic font";     break;
	case  6:
	case  7:
	case  8:
	case  9:
	case 10: spell = L"acid blast";       break;
	default: spell = L"";     break;
	}

    if ( ( sn = skill_lookup( (wchar_t*)spell ) ) < 0 )
	return FALSE;
    say_spell(ch,sn);
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim,TARGET_CHAR);
    return TRUE;
}


bool spec_fight_lionguard( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    const wchar_t *spell;
    int sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_range(0,1) == 0 )
	    break;
    }


    if ( victim == NULL )
	return FALSE;


    if ( number_percent() < 33)
    {
	int damage_claw;

	damage_claw = dice(ch->level, 24) + ch->damroll;
	damage(ch, victim, damage_claw, gsn_claw, DAM_BASH, TRUE);
	return TRUE;
    }

    switch ( dice(1,16) )
    {
	case  0:
	case  1: spell = L"dispel magic";    break;
	case  2:
	case  3: spell = L"acid blast";   break;
	case  4:
	case  5: spell = L"caustic font";     break;
	case  6:
	case  7:
	case  8: spell = L"acid arrow";       break;
	default: spell = L"";     break;
     }

    if ( ( sn = skill_lookup( (wchar_t*)spell ) ) < 0 )
	return FALSE;
    say_spell(ch,sn);
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim,TARGET_CHAR);
    return TRUE;
}


bool spec_fight_hunter( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    const wchar_t *spell;
    int sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_range(0,1) == 0 )
	    break;
    }

        switch ( dice(1,16) )
	{
	case  0:
	case  1: spell = L"dispel magic";    break;
	case  2:
	case  3: spell = L"acid arrow";   break;
	case  4:
	case  5: spell = L"caustic font";     break;
	case  6:
	case  7:
	case  8:
	case  9: spell = L"acid blast";       break;
	default: spell = L"";     break;
	}

    if ( victim == NULL )
	return FALSE;

    if ( ( sn = skill_lookup( (wchar_t*)spell ) ) < 0 )
	return FALSE;
    say_spell(ch,sn);
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim,TARGET_CHAR);
    return TRUE;
}

bool spec_wishmaster( CHAR_DATA *ch )
{
	int i;
    if ( !IS_AWAKE(ch) )
        return FALSE;
	i=number_range(0,150);
	switch(i)
	{
		case 0:
			do_say(ch, (wchar_t*)"Bir dilek tutmak istemez misin? Öyleyse dilek listeme bakmalısın.");
			return TRUE;
		case 1:
			do_say(ch, (wchar_t*)"Uygun bir ücrete harika bir dilek dilemek istemez misin? Listeme bakmalısın.");
			return TRUE;
	}
    return FALSE;
}

bool spec_kameni_dindar( CHAR_DATA *ch )
{
    if ( !IS_AWAKE(ch) )
        return FALSE;
    if (number_range(0,100) == 0) {
      do_say(ch, (wchar_t*)"Merhaba tanrının merhametine muhtaç kişi.");
      do_say(ch, (wchar_t*)"Kame'nin el yazmalarını arıyorum.");
      do_say(ch, (wchar_t*)"Yeraltı'nda o el yazmalarından bahsedildiğini duydum.");
      do_say(ch, (wchar_t*)"Ama bulmak nasip olmadı.");
        return TRUE;
    }
    return FALSE;
}

bool spec_niryani_dindar( CHAR_DATA *ch )
{
    if ( !IS_AWAKE(ch) )
        return FALSE;
    if (number_range(0,100) == 0) {
      do_say(ch, (wchar_t*)"Merhaba tanrının merhametine muhtaç kişi.");
      do_say(ch, (wchar_t*)"Nir'in el yazmalarını arıyorum.");
      do_say(ch, (wchar_t*)"Eski Thalos'ta o el yazmalarından bahsedildiğini duydum.");
      do_say(ch, (wchar_t*)"Ama bulmak nasip olmadı.");
        return TRUE;
    }
    return FALSE;
}

bool spec_nyahi_dindar( CHAR_DATA *ch )
{
    if ( !IS_AWAKE(ch) )
        return FALSE;
    if (number_range(0,100) == 0) {
      do_say(ch, (wchar_t*)"Merhaba tanrının merhametine muhtaç kişi.");
      do_say(ch, (wchar_t*)"Nyah'ın el yazmalarını arıyorum.");
      do_say(ch, (wchar_t*)"Bölümüş Ruhlar'da o el yazmalarından bahsedildiğini duydum.");
      do_say(ch, (wchar_t*)"Ama bulmak nasip olmadı.");
        return TRUE;
    }
    return FALSE;
}

bool spec_sintaryan_dindar( CHAR_DATA *ch )
{
    if ( !IS_AWAKE(ch) )
        return FALSE;
    if (number_range(0,100) == 0) {
      do_say(ch, (wchar_t*)"Merhaba tanrının merhametine muhtaç kişi.");
      do_say(ch, (wchar_t*)"Sint'in el yazmalarını arıyorum.");
      do_say(ch, (wchar_t*)"Haon Dor'da o el yazmalarından bahsedildiğini duydum.");
      do_say(ch, (wchar_t*)"Ama bulmak nasip olmadı.");
        return TRUE;
    }
    return FALSE;
}
