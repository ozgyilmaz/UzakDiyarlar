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
DECLARE_SPEC_FUN(	spec_troll_member	);
DECLARE_SPEC_FUN(	spec_ogre_member	);
DECLARE_SPEC_FUN(	spec_patrolman		);
DECLARE_SPEC_FUN(       spec_cast_cabal         );
DECLARE_SPEC_FUN(       spec_special_guard      );
DECLARE_SPEC_FUN(       spec_stalker            );
DECLARE_SPEC_FUN(       spec_questmaster        );
DECLARE_SPEC_FUN(       spec_assassinater       );
DECLARE_SPEC_FUN(       spec_repairman		);
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
    {	"spec_breath_any",		spec_breath_any		},
    {	"spec_breath_acid",		spec_breath_acid	},
    {	"spec_breath_fire",		spec_breath_fire	},
    {	"spec_breath_frost",		spec_breath_frost	},
    {	"spec_breath_gas",		spec_breath_gas		},
    {	"spec_breath_lightning",	spec_breath_lightning	},
    {	"spec_cast_adept",		spec_cast_adept		},
    {	"spec_cast_cleric",		spec_cast_cleric	},
    {	"spec_cast_judge",		spec_cast_judge		},
    {	"spec_cast_mage",		spec_cast_mage		},
    {	"spec_cast_beholder",		spec_cast_beholder	},
    {	"spec_cast_undead",		spec_cast_undead	},
    {	"spec_executioner",		spec_executioner	},
    {	"spec_fido",			spec_fido		},
    {	"spec_guard",			spec_guard		},
    {	"spec_janitor",			spec_janitor		},
    {	"spec_mayor",			spec_mayor		},
    {	"spec_poison",			spec_poison		},
    {	"spec_thief",			spec_thief		},
    {	"spec_nasty",			spec_nasty		},
    {	"spec_troll_member",		spec_troll_member	},
    {	"spec_ogre_member",		spec_ogre_member	},
    {	"spec_patrolman",		spec_patrolman		},
    {	"spec_cast_cabal",		spec_cast_cabal		},
    {	"spec_stalker",			spec_stalker		},
    {	"spec_special_guard",		spec_special_guard	},
    {   "spec_questmaster",             spec_questmaster        },
    {   "spec_assassinater",            spec_assassinater	},
    {   "spec_repairman",		spec_repairman		},
    {	"spec_captain",			spec_captain		},
    {   "spec_headlamia",		spec_headlamia		},
    {	"spec_fight_enforcer",		spec_fight_enforcer	},
    {	"spec_fight_invader",		spec_fight_invader	},
    {	"spec_fight_ivan",		spec_fight_ivan		},
    {	"spec_fight_seneschal",		spec_fight_seneschal	},
    {	"spec_fight_powerman",		spec_fight_powerman	},
    {	"spec_fight_protector",		spec_fight_protector	},
    {	"spec_fight_hunter",		spec_fight_hunter	},
    {	"spec_fight_lionguard",		spec_fight_lionguard	},
    {	NULL,				NULL			}
};

/*
 * Given a name, return the appropriate spec fun.
 */
SPEC_FUN *spec_lookup( char *name )
{
   int i;

   for ( i = 0; spec_table[i].name != NULL; i++)
   {
        if (LOWER(name[0]) == LOWER(spec_table[i].name[0])
        &&  !str_prefix( name,spec_table[i].name))
            return spec_table[i].function;
   }

    return 0;
}

char *spec_name( SPEC_FUN *function)
{
    int i;

    for (i = 0; spec_table[i].function != NULL; i++)
    {
	if (function == spec_table[i].function)
	    return (char*)spec_table[i].name;
    }

    return NULL;
}

bool spec_troll_member( CHAR_DATA *ch)
{
    CHAR_DATA *vch, *victim = NULL;
    int count = 0;
    const char *message;

    if (!IS_AWAKE(ch) || IS_AFFECTED(ch,AFF_CALM) || ch->in_room == NULL
    ||  IS_AFFECTED(ch,AFF_CHARM) || ch->fighting != NULL)
	return FALSE;

    /* find an ogre to beat up */
    for (vch = ch->in_room->people;  vch != NULL;  vch = vch->next_in_room)
    {
	if (!IS_NPC(vch) || ch == vch)
	    continue;

	if (vch->pIndexData->vnum == MOB_VNUM_PATROLMAN)
	    return FALSE;

	if (vch->pIndexData->group == GROUP_VNUM_OGRES
	&&  ch->level > vch->level - 2 && !is_safe(ch,vch))
	{
	    if (number_range(0,count) == 0)
		victim = vch;

	    count++;
	}
    }

    if (victim == NULL)
	return FALSE;

    /* say something, then raise hell */
    switch (number_range(0,6))
    {
	default:  message = NULL; 	break;
  case 0:	message = "$n 'Seni bekliyordum, serseri!' diye baðýrýyor";
		break;
	case 1: message = "$n ýrkçý bir öfkeyle $E saldýrýyor.";
		break;
	case 2: message = "$n 'Senin gibi bir Umacý pisliðinin buralarda iþi ne?' diyor";
		break;
	case 3: message = "$n parmaklarýný kütleterek soruyor, 'Kendini þanslý hissediyor musun?'";
		break;
	case 4: message = "$n 'Bu sefer etrafta seni kurtaracak muhafýz da yok!' diyor.";
		break;
	case 5: message = "$n 'Kardeþinin yanýna gitme vaktin geldi, ahmak,' diyor";
		break;
	case 6: message = "$n 'Haydi baþlayalým,' diyor.";
		break;
    }

    if (message != NULL)
    	act(message,ch,NULL,victim,TO_ALL);
    multi_hit( ch, victim, TYPE_UNDEFINED );
    return TRUE;
}

bool spec_ogre_member( CHAR_DATA *ch)
{
    CHAR_DATA *vch, *victim = NULL;
    int count = 0;
    const char *message;

    if (!IS_AWAKE(ch) || IS_AFFECTED(ch,AFF_CALM) || ch->in_room == NULL
    ||  IS_AFFECTED(ch,AFF_CHARM) || ch->fighting != NULL)
        return FALSE;

    /* find an troll to beat up */
    for (vch = ch->in_room->people;  vch != NULL;  vch = vch->next_in_room)
    {
        if (!IS_NPC(vch) || ch == vch)
            continue;

        if (vch->pIndexData->vnum == MOB_VNUM_PATROLMAN)
            return FALSE;

        if (vch->pIndexData->group == GROUP_VNUM_TROLLS
        &&  ch->level > vch->level - 2 && !is_safe(ch,vch))
        {
            if (number_range(0,count) == 0)
                victim = vch;

            count++;
        }
    }

    if (victim == NULL)
        return FALSE;

    /* say something, then raise hell */
    switch (number_range(0,6))
    {
	default: message = NULL;	break;
  case 0: message = "$n 'Seni bekliyordum, serseri!' diye baðýrýyor";
          break;
  case 1: message = "$n ýrkçý bir öfkeyle $E saldýrýyor.";
          break;
  case 2: message = "$n 'Senin gibi bir Arba pisliðinin buralarda iþi ne?' diyor";
          break;
  case 3: message = "$n parmaklarýný kütleterek soruyor, 'Kendini þanslý hissediyor musun?'";
          break;
  case 4: message = "$n 'Bu sefer etrafta seni kurtaracak muhafýz da yok!' diyor.";
          break;
  case 5: message = "$n 'Kardeþinin yanýna gitme vaktin geldi, ahmak,' diyor";
          break;
  case 6: message = "$n 'Haydi baþlayalým,' diyor.";
                break;
    }

    if (message != NULL)
    	act(message,ch,NULL,victim,TO_ALL);
    multi_hit( ch, victim, TYPE_UNDEFINED );
    return TRUE;
}

bool spec_patrolman(CHAR_DATA *ch)
{
    CHAR_DATA *vch,*victim = NULL;
/*    OBJ_DATA *obj; */
    const char *message;
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
	act("You blow down hard on $p.",ch,obj,NULL,TO_CHAR);
	act("$n blows on $p, ***WHEEEEEEEEEEEET***",ch,obj,NULL,TO_ROOM);

    	for ( vch = char_list; vch != NULL; vch = vch->next )
    	{
            if ( vch->in_room == NULL )
            	continue;

            if (vch->in_room != ch->in_room
	    &&  vch->in_room->area == ch->in_room->area)
            	send_to_char( "You hear a shrill whistling sound.\n\r", vch );
    	}
    }
*/
    switch (number_range(0,6))
    {
	default:	message = NULL;		break;
  case 0:	message = "$n 'Hey hey heey, ayrýlýn!' diye baðýrýyor";
		break;
	case 1: message =
		"$n 'Sorumlu olan toplum, tek baþýma ne yapabilirim ki?' dedi";
		break;
	case 2: message =
		"$n 'Bu serseriler hepimizi mahvedecek.' diye mýrýldanýyor";
		break;
	case 3: message = "$n 'Durun! Durun!' diye baðýrýyor ve saldýrýyor.";
		break;
	case 4: message = "$n sopasýný eline alarak iþe giriþiyor.";
		break;
	case 5: message =
		"$n iç çekerek kavgayý ayýrmaya giriþiyor.";
		break;
	case 6: message = "$n 'Sakin olun, sizi fanatikler!' diyor.";
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
bool dragon( CHAR_DATA *ch, char *spell_name )
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
	     && number_bits( 3 ) == 0 )
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

    switch ( number_bits( 3 ) )
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
    return dragon( ch, (char*)"acid breath" );
}



bool spec_breath_fire( CHAR_DATA *ch )
{
    return dragon( ch, (char*)"fire breath" );
}



bool spec_breath_frost( CHAR_DATA *ch )
{
    return dragon( ch, (char*)"frost breath" );
}



bool spec_breath_gas( CHAR_DATA *ch )
{
    int sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    if ( ( sn = skill_lookup( "gas breath" ) ) < 0 )
	return FALSE;
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, NULL,TARGET_CHAR);
    return TRUE;
}



bool spec_breath_lightning( CHAR_DATA *ch )
{
    return dragon( ch, (char*)"lightning breath" );
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
	if ( victim != ch && can_see( ch, victim ) && number_bits( 1 ) == 0
	     && !IS_NPC(victim) && victim->level < 11)
	    break;
    }

    if ( victim == NULL )
	return FALSE;

    switch ( number_bits( 4 ) )
    {
    case 0:
    act( "$n sihirli sözcükler söylüyor 'abrazak'.", ch, NULL, NULL, TO_ROOM );
	spell_armor( skill_lookup( "armor" ), ch->level,ch,victim,TARGET_CHAR);
	return TRUE;

    case 1:
    act( "$n sihirli sözcükler söylüyor 'fido'.", ch, NULL, NULL, TO_ROOM );
	spell_bless( skill_lookup( "bless" ), ch->level,ch,victim,TARGET_CHAR);
	return TRUE;

    case 2:
    act("$n sihirli sözcükler söylüyor 'judicandus noselacri'.",ch,NULL,NULL,TO_ROOM);
	spell_cure_blindness( skill_lookup( "cure blindness" ),
	    ch->level, ch, victim,TARGET_CHAR);
	return TRUE;

    case 3:
    act("$n sihirli sözcükler söylüyor 'judicandus dies'.", ch,NULL, NULL, TO_ROOM );
	spell_cure_light( skill_lookup( "cure light" ),
	    ch->level, ch, victim,TARGET_CHAR);
	return TRUE;

    case 4:
    act( "$n sihirli sözcükler söylüyor 'judicandus sausabru'.",ch,NULL,NULL,TO_ROOM);
	spell_cure_poison( skill_lookup( "cure poison" ),
	    ch->level, ch, victim,TARGET_CHAR);
	return TRUE;

    case 5:
    act("$n sihirli sözcükler söylüyor 'candusima'.", ch, NULL, NULL, TO_ROOM );
	spell_refresh( skill_lookup("refresh"),ch->level,ch,victim,TARGET_CHAR);
	return TRUE;

    case 6:
    act("$n sihirli sözcükler söylüyor 'judicandus eugzagz'.",ch,NULL,NULL,TO_ROOM);
	spell_cure_disease(skill_lookup("cure disease"),
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
	if ( victim->fighting == ch && number_bits( 2 ) == 0 )
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
    const char *spell;
    int sn;

    if ( ch->position != POS_FIGHTING )
        return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
        v_next = victim->next_in_room;
        if ( victim->fighting == ch && number_bits( 2 ) == 0 )
            break;
    }

    if ( victim == NULL )
        return FALSE;

    spell = "high explosive";
    if ( ( sn = skill_lookup( (char*)spell ) ) < 0 )
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
	if ( victim->fighting == ch && number_bits( 2 ) == 0 )
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
    const char *spell;
    int sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_bits( 2 ) == 0 )
	    break;
    }

    if ( victim == NULL )
	return FALSE;

    for ( ;; )
    {
	int min_level;

	switch ( number_bits( 4 ) )
	{
	case  0: min_level =  0; spell = "curse";          break;
	case  1: min_level =  3; spell = "weaken";         break;
	case  2: min_level =  6; spell = "chill touch";    break;
	case  3: min_level =  9; spell = "blindness";      break;
	case  4: min_level = 12; spell = "poison";         break;
	case  5: min_level = 15; spell = "energy drain";   break;
	case  6: min_level = 18; spell = "harm";           break;
	case  7: min_level = 21; spell = "teleport";       break;
	case  8: min_level = 20; spell = "plague";	   break;
	default: min_level = 18; spell = "harm";           break;
	}

	if ( ch->level >= min_level )
	    break;
    }

    if ( ( sn = skill_lookup( (char*)spell ) ) < 0 )
	return FALSE;
    say_spell(ch,sn);
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim,TARGET_CHAR);
    return TRUE;
}


bool spec_executioner( CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    const char *crime;

    if ( !IS_AWAKE(ch) || ch->fighting != NULL )
	return FALSE;

    crime = "";
    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;

	if ( !IS_NPC(victim) && IS_SET(victim->act, PLR_WANTED)
	&&   can_see(ch,victim))
	    { crime = "SUÇLU"; break; }
    }

    if ( victim == NULL )
	return FALSE;

  sprintf( buf, "%s bir %s!  MASUMU KORU!  KAAAANN!!!",victim->name, crime );
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

      act( "$n cesedi vahþice parçalayýp yiyor.", ch, NULL, NULL, TO_ROOM );
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
    act( "$n çöpleri topluyor.", ch, NULL, NULL, TO_ROOM );
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
    static const char open_path[] =
	"W3a3003b000c000d111Oe333333Oe22c222112212111a1S.";

    static const char close_path[] =
	"W3a3003b000c000d111CE333333CE22c222112212111a1S.";

    static const char *path;
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
  act("$n uyanýyor ve gürültülü bir inilti koparýyor.", ch, NULL, NULL, TO_ROOM );
	break;

    case 'S':
	ch->position = POS_SLEEPING;
  act("$n yatýp uyuyor.", ch, NULL, NULL, TO_ROOM );
	break;

    case 'a':
    do_say( ch,(char*) "Merhaba tatlým!" );
	break;

    case 'b':
    do_say(ch,(char*)"Þu rezilliðe bak! Çöpler konusunda birþey yapmalýyým!");
	break;

    case 'c':
    do_say(ch,(char*)"Gençlerde saygý denen þey kalmadý.");
	break;

    case 'd':
    do_say(ch,(char*)"Ýyi günler, vatandaþlar!");
	break;

    case 'e':
    do_say(ch,(char*)"Selenge þehir kapýlarýnýn açýldýðýný bildiririm.");
	break;

    case 'E':
    do_say(ch,(char*)"Selenge þehir kapýlarýnýn kapandýðýný bildiririm.");
	break;

    case 'O':
	do_unlock( ch, (char*)"kapý" );
	do_open( ch, (char*)"kapý" );
  interpret( ch, (char*)"duygu þehir kapýsýnýn kilidini açýyor.",FALSE);
	for( key=ch->in_room->contents; key!=NULL; key=key->next_content )
	  if ( key->pIndexData->vnum == 3379 )
	    break;
	if ( key != NULL )
	  SET_BIT( key->wear_flags, ITEM_TAKE );
	do_get( ch, (char*)"anahtar" );
	break;

    case 'C':
	do_close( ch, (char*)"kapý" );
	do_lock( ch, (char*)"kapý" );
	do_drop( ch, (char*)"anahtar" );
  interpret( ch, (char*)"duygu þehir kapýsýný zincirliyor.",FALSE);
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

  act( "$M ýsýrýyorsun!",  ch, NULL, victim, TO_CHAR    );
  act( "$n $M ýsýrýyor!",  ch, NULL, victim, TO_NOTVICT );
  act( "$n seni ýsýrýyor!", ch, NULL, victim, TO_VICT    );
    spell_poison( gsn_poison, ch->level, ch, victim,TARGET_CHAR);
    return TRUE;
}



bool spec_thief( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    long gold,silver;

    if ( ch->position != POS_STANDING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;

	if ( IS_NPC(victim)
	||   victim->level >= LEVEL_IMMORTAL
	||   number_bits( 5 ) != 0
	||   !can_see(ch,victim))
	    continue;

	if ( IS_AWAKE(victim) && number_range( 0, ch->level ) == 0 )
	{
    act( "$s ellerinin para keseni kurcaladýðýný farkediyorsun!",
  ch, NULL, victim, TO_VICT );
    act( "$N $s ellerinin para kesesini kurcaladýðýný farkediyor!",
		ch, NULL, victim, TO_NOTVICT );
	    return TRUE;
	}
	else
	{
	    gold = victim->gold * UMIN(number_range(1,20),ch->level / 2) / 100;
	    gold = UMIN(gold, ch->level * ch->level * 10 );
	    ch->gold     += gold;
	    victim->gold -= gold;
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
	if ( victim != ch && can_see( ch, victim ) && number_bits( 1 ) == 0)
	  break;
    }

    if ( victim == NULL )
	return FALSE;


    switch ( number_bits( 4 ) )
    {
    case 0:
    act( "$n sihirli sözcükler söylüyor 'abracal'.", ch, NULL, NULL, TO_ROOM );
	spell_armor( skill_lookup( "armor" ), ch->level, ch, victim, TARGET_CHAR );
	return TRUE;

    case 1:
    act(  "$n sihirli sözcükler söylüyor 'balc'.", ch, NULL, NULL, TO_ROOM );
	spell_bless( skill_lookup( "bless" ), ch->level, ch, victim, TARGET_CHAR );
	return TRUE;

    case 2:
    act("$n sihirli sözcükler söylüyor 'judicandus noselacba'.", ch, NULL, NULL, TO_ROOM );
	spell_cure_blindness( skill_lookup( "cure blindness" ),
	    ch->level, ch, victim, TARGET_CHAR );
	return TRUE;

    case 3:
    act( "$n sihirli sözcükler söylüyor 'judicandus bacla'.", ch, NULL, NULL,TO_ROOM);
	spell_cure_light( skill_lookup( "cure light" ),
	    ch->level, ch, victim, TARGET_CHAR );
	return TRUE;

    case 4:
    act( "$n sihirli sözcükler söylüyor 'judicandus sausabcla'.",
	    ch, NULL, NULL, TO_ROOM );
	spell_cure_poison( skill_lookup( "cure poison" ),
	    ch->level, ch, victim, TARGET_CHAR );
	return TRUE;

    case 5:
    act( "$n sihirli sözcükler söylüyor 'candabala'.", ch, NULL, NULL, TO_ROOM );
	spell_refresh( skill_lookup( "refresh" ), ch->level, ch, victim,
TARGET_CHAR );
	return TRUE;

    }

    return FALSE;
}



bool spec_guard( CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    CHAR_DATA *ech;
    const char *crime;

    if ( !IS_AWAKE(ch) || ch->fighting != NULL )
      return FALSE;

    ech      = NULL;
    crime    = "";

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
      {
	v_next = victim->next_in_room;

        if (!can_see(ch,victim)) continue;

	if (IS_SET(ch->in_room->area->area_flag,AREA_HOMETOWN)
		&& number_percent() < 2 )
	 {
     do_say( ch, (char*)"Seni tanýyor muyum?");
 	  if (str_cmp(ch->in_room->area->name,
		hometown_table[victim->hometown].name) )
	   {
       do_say( ch,(char*)"Seni hatýrlamýyorum. Uzaklaþ!");
	   }
	  else
	   {
       do_say(ch, (char*)"Tamam, þimdi hatýrladým.");
		 interpret( ch, (char*)"gülümse",FALSE);
	   }
	 }

	if ( !IS_NPC(victim) && IS_SET(victim->act, PLR_WANTED) )
	  { crime = "SUÇLU"; break; }

	if ( victim->fighting != NULL
	     && victim->fighting != ch
	     && victim->ethos != 1
	     && !IS_GOOD(victim)
	     && !IS_EVIL(victim->fighting) )
	  { ech = victim; victim=NULL; break; }
      }

    if ( victim != NULL )
      {
        sprintf( buf, "%s bir %s!  MASUMU KORU!  SALDIR!!!",victim->name, crime );
	do_yell( ch, buf );
	multi_hit( ch, victim, TYPE_UNDEFINED );
	return TRUE;
      }

    if ( ech != NULL )
      {
        act( "$n baðýrýyor 'MASUMU KORU!!  SALDIR!!!'",
			ch, NULL, NULL, TO_ROOM );
	multi_hit( ch, ech, TYPE_UNDEFINED );
	return TRUE;
      }

    return FALSE;
  }




bool spec_special_guard( CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim, *ech;
    CHAR_DATA *v_next;
    const char *crime;

    if ( !IS_AWAKE(ch) || ch->fighting != NULL )
	return FALSE;

    crime    = "";
    ech      = NULL;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;

        if (!can_see(ch,victim)) continue;

	if ( !IS_NPC(victim) && IS_SET(victim->act, PLR_WANTED) )
	  { crime = "SUÇLU"; break; }

	if ( victim->fighting != NULL
	     && victim->fighting != ch
	     && victim->fighting->cabal == CABAL_RULER )
	  { ech = victim; victim=NULL; break; }
    }

    if ( victim != NULL )
    {
      sprintf( buf, "%s bir %s!  MASUMU KORU!  SALDIR!!!",victim->name, crime );

	do_yell( ch, buf );
	multi_hit( ch, victim, TYPE_UNDEFINED );
	return TRUE;
    }

    if ( ech != NULL )
    {
      act( "$n baðýrýyor 'MASUMU KORU!  SALDIR!!!'",
			ch, NULL, NULL, TO_ROOM );
	multi_hit( ch, ech, TYPE_UNDEFINED );
	return TRUE;
    }

    return FALSE;
}

bool spec_stalker(CHAR_DATA *ch)
{
  char buf[MAX_STRING_LENGTH];
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
      do_cb(ch, (char*)"Kurbanýmý öldürdüðüme göre geldiðim yere dönebilirim.");
      extract_char(ch, TRUE);
      return TRUE;
    }

  if (victim == NULL)
    {
      ch->cabal = CABAL_RULER;
      do_cb(ch,(char*) "Kurbaným oyundan ayrýldý. Bu benim gitme vaktimin geldiðinin de göstergesi.");
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
        sprintf(buf,"Sen bir suçlusun %s! Ve öleceksin!!!", victim->name);
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
          sprintf(buf, "Üzgünüm ama %s izini kaybettirdi. Gitmeliyim.",
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
    long gold;

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

    switch ( number_bits(2) )
    {
      case 0:  act("$n para keseni yýrtýp atarak paranýn saçýlmasýna neden oluyor!",
                   ch, NULL, victim, TO_VICT);
               act("$S para kesesini yýrtýyor ve altýnlarýný topluyorsun.",
                   ch, NULL, victim, TO_CHAR);
               act("$S para kesesi yýrtýlýyor!",
                     ch, NULL, victim, TO_NOTVICT);
                 gold = victim->gold / 10;  /* steal 10% of his gold */
                 victim->gold -= gold;
                 ch->gold     += gold;
                 return TRUE;

        case 1:  do_flee( ch, (char*)"");
                 return TRUE;

        default: return FALSE;
    }
}

bool spec_questmaster( CHAR_DATA *ch )
{
    if ( !IS_AWAKE(ch) )
        return FALSE;
    if (number_range(0,100) == 0) {
      do_say(ch, (char*)"Bir görev istemez misin?");
        return TRUE;
    }
    return FALSE;
}

bool spec_assassinater( CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
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
           sprintf( buf, "Ölüm tek sondur...");
     break;
  case  6:
           sprintf( buf, "Ölme zamaný....");
     break;
  case  7:
           sprintf( buf, "Ölme zamaný....");
     break;
  case  8:
           sprintf( buf, "Kaderin seni bekliyor....");
     break;
  case  9:
     sprintf( buf, "Ölümsüzlere bir kurban.... ");
     break;
  case 10:
           sprintf( buf, "Hiç þeytanla raks ettin mi....");
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
      do_say(ch, (char*)"Þimdi diðer ekipmanlarý tamir etme zamaný.");
        return TRUE;
    }
    return FALSE;
}

bool spec_captain( CHAR_DATA *ch )
{

    static const char open_path[] =
"Wn0onc0oe1f2212211s2tw3xw3xd3322a22b22yO00d00a0011e1fe1fn0o3300300w3xs2ts2tS.";

    static const char close_path[] =
"Wn0on0oe1f2212211s2twc3xw3x3322d22a22EC0a00d0b0011e1fe1fn0o3300300w3xs2ts2tS.";

    static const char *path;
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
  act_color( "$C$n uyanýyor ve esniyor.$c", ch, NULL, NULL, TO_ROOM,
POS_RESTING,CLR_WHITE );
	break;

    case 'S':
	ch->position = POS_SLEEPING;
  act_color( "$C$n yatýp uyumaya baþlýyor.$c", ch, NULL, NULL, TO_ROOM,
POS_RESTING,CLR_WHITE );
	break;

    case 'a':
    act_color( "$C$n 'Selamlar! Ýyi avlar dilerim!' dedi$c", ch, NULL,
        NULL,TO_ROOM,POS_RESTING, CLR_YELLOW );
	break;

    case 'b':
    act_color("$C$n 'Caddeleri temiz tutalým ki Solace temiz olsun,' dedi.$c",ch,NULL, NULL, TO_ROOM, POS_RESTING,CLR_YELLOW );
	break;

    case 'c':
    act_color( "$C$n 'Bu kapýlara bir çare bulmalýyým,' dedi.$c",ch,
    NULL, NULL, TO_ROOM, POS_RESTING,CLR_YELLOW );
    act_color("$C$n 'Buradan hiç ayrýlmayacaðým,' dedi.$c", ch, NULL, NULL,
        TO_ROOM,POS_RESTING,CLR_YELLOW );
	break;

    case 'd':
    act_color("$C$n says 'Selamlar Solace vatadaþlarý!' dedi.$c", ch, NULL, NULL,
        TO_ROOM, POS_RESTING,CLR_YELLOW );
	break;

    case 'y':
    act_color( "$C$n 'Solace þehrinin açýldýðýný bildiririm!' dedi.$c", ch,
NULL, NULL, TO_ROOM, POS_RESTING,CLR_YELLOW );
	break;

    case 'E':
    act_color( "$C$n 'Solace þehrinin kapandýðýný deklare ederim!' dedi.$c", ch,
NULL, NULL, TO_ROOM, POS_RESTING,CLR_YELLOW );
	break;

    case 'O':
	do_unlock( ch, (char*)"kapý" );
	do_open( ch, (char*)"kapý" );
	break;

    case 'C':
	do_close( ch, (char*)"kapý" );
	do_lock( ch, (char*)"kapý" );
	break;

    case 'n':
	do_open( ch, (char*)"kuzey" );
	break;

    case 'o':
        do_close( ch, (char*)"güney" );
        break;

    case 's':
	do_open( ch, (char*)"güney" );
	break;

    case 't':
        do_close( ch, (char*)"kuzey" );
        break;

    case 'e':
	do_open( ch, (char*)"doðu" );
	break;

    case 'f':
        do_close( ch, (char*)"batý" );
        break;

    case 'w':
	do_open( ch, (char*)"batý" );
	break;

    case 'x':
        do_close( ch, (char*)"doðu" );
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
    static const char path[] = "T111111100003332222232211.";
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
    const char *spell;
    int sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_bits( 1 ) == 0 )
	    break;
    }


    if ( victim == NULL )
	return FALSE;

        switch ( dice(1,16) )
	{
	case  0: spell = "fear";    break;
	case  1: spell = "fear";    break;
	case  2: spell = "slow";    break;
	case  3: spell = "cause serious";       break;
	case  4: spell = "cause critical";      break;
	case  5: spell = "harm";   break;
	case  6: spell = "harm";   break;
	case  7: spell = "dispel magic";   break;
	case  8: spell = "dispel magic";   break;
	default: spell = "";     break;
	}

    if ( ( sn = skill_lookup( (char*)spell ) ) < 0 )
	return FALSE;
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim,TARGET_CHAR);
    return TRUE;
}



bool spec_fight_enforcer( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    const char *spell;
    int sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_bits( 1 ) == 0 )
	    break;
    }


    if ( victim == NULL )
	return FALSE;

        switch ( dice(1,16) )
	{
	case  0:
	case  1: spell = "dispel magic";    break;
	case  2:
	case  3: spell = "acid arrow";   break;
	case  4:
	case  5: spell = "caustic font";     break;
	case  6:
	case  7:
	case  8:
	case  9:
	case 10: spell = "acid blast";       break;
	default: spell = "";     break;
	}

    if ( ( sn = skill_lookup( (char*)spell ) ) < 0 )
	return FALSE;
    say_spell(ch,sn);
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim,TARGET_CHAR);
    return TRUE;
}


bool spec_fight_invader( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    const char *spell;
    int sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_bits( 1 ) == 0 )
	    break;
    }


    if ( victim == NULL )
	return FALSE;

        switch ( dice(1,16) )
	{
	case  0:
	case  1: spell = "blindness";      break;
	case  2:
	case  3: spell = "dispel magic";    break;
	case  4:
	case  5: spell = "weaken";         break;
	case  6:
	case  7: spell = "energy drain"; break;
	case  8:
	case  9: spell = "plague";   break;
	case 10:
	case 11: spell = "acid arrow";   break;
	case 12:
	case 13:
	case 14: spell = "acid blast";       break;
	case 15:
		if ( ch->hit < (ch->max_hit / 3))
		  spell = "shadow cloak";
		else spell = "";
		break;
	default: spell = "";     break;
	}

    if ( ( sn = skill_lookup( (char*)spell ) ) < 0 )
	return FALSE;
    say_spell(ch,sn);
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim,TARGET_CHAR);
    return TRUE;
}


bool spec_fight_ivan( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    const char *spell;
    int sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_bits( 1 ) == 0 )
	    break;
    }


    if ( victim == NULL )
	return FALSE;

        switch ( dice(1,16) )
	{
	case  0:
	case  1: spell = "dispel magic";    break;
	case  2:
	case  3: spell = "acid arrow";   break;
	case  4:
	case  5: spell = "caustic font";     break;
	case  6:
	case  7:
	case  8: spell = "acid blast";       break;
	case  9: spell = "disgrace";	break;
	case 10:
		if ( ch->hit < (ch->max_hit / 3))
		  spell = "garble";
		else spell = "";
		break;
	default: spell = "";     break;
	}

    if ( ( sn = skill_lookup( (char*)spell ) ) < 0 )
	return FALSE;
    say_spell(ch,sn);
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim,TARGET_CHAR);
    return TRUE;
}


bool spec_fight_seneschal( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    const char *spell;
    int sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_bits( 1 ) == 0 )
	    break;
    }


    if ( victim == NULL )
	return FALSE;

        switch ( dice(1,16) )
	{
	case  0: spell = "blindness";      break;
	case  1: spell = "dispel magic";    break;
	case  2: spell = "weaken";         break;
	case  3: spell = "blindness";      break;
	case  4: spell = "acid arrow";   break;
	case  5: spell = "caustic font";     break;
	case  6: spell = "energy drain";   break;
	case  7:
	case  8:
	case  9: spell = "acid blast";       break;
	case 10: spell = "plague";	   break;
	case 11: spell = "acid blast";         break;
	case 12:
	case 13: spell = "lightning breath";  break;
	case 14:
	case 15: spell = "mental knife";	break;
	default: spell = "";     break;
	}

    if ( ( sn = skill_lookup( (char*)spell ) ) < 0 )
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
	do_spellbane(ch, (char*)"");

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_bits( 1 ) == 0 )
	    break;
    }


    if ( victim == NULL )
	return FALSE;

    if ( number_percent() < 33)
    {
      act("Ölümcül üçlü saldýrýyý yapýyorsun!",ch,NULL,NULL,TO_CHAR);
    	act("$n ölümcül üçlü saldýrýyý yapýyor!",ch,NULL,NULL,TO_ROOM);
	one_hit( ch, victim, TYPE_UNDEFINED, FALSE );
	one_hit( ch, victim, TYPE_UNDEFINED, FALSE );
	one_hit( ch, victim, TYPE_UNDEFINED, FALSE );
    }

    if ( !is_affected(ch, gsn_resistance))
	do_resistance(ch, (char*)"");

    if ( ch->hit < (ch->max_hit /3) && !IS_AFFECTED(ch, AFF_REGENERATION))
	do_bandage(ch, (char*)"");

    return TRUE;
}


bool spec_fight_protector( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    const char *spell;
    int sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_bits( 1 ) == 0 )
	    break;
    }


    if ( victim == NULL )
	return FALSE;

        switch ( dice(1,16) )
	{
	case  0:
	case  1: spell = "dispel magic";    break;
	case  2:
	case  3: spell = "acid arrow";   break;
	case  4:
	case  5: spell = "caustic font";     break;
	case  6:
	case  7:
	case  8:
	case  9:
	case 10: spell = "acid blast";       break;
	default: spell = "";     break;
	}

    if ( ( sn = skill_lookup( (char*)spell ) ) < 0 )
	return FALSE;
    say_spell(ch,sn);
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim,TARGET_CHAR);
    return TRUE;
}


bool spec_fight_lionguard( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    const char *spell;
    int sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_bits( 1 ) == 0 )
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
	case  1: spell = "dispel magic";    break;
	case  2:
	case  3: spell = "acid blast";   break;
	case  4:
	case  5: spell = "caustic font";     break;
	case  6:
	case  7:
	case  8: spell = "acid arrow";       break;
	default: spell = "";     break;
     }

    if ( ( sn = skill_lookup( (char*)spell ) ) < 0 )
	return FALSE;
    say_spell(ch,sn);
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim,TARGET_CHAR);
    return TRUE;
}


bool spec_fight_hunter( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    const char *spell;
    int sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_bits( 1 ) == 0 )
	    break;
    }

        switch ( dice(1,16) )
	{
	case  0:
	case  1: spell = "dispel magic";    break;
	case  2:
	case  3: spell = "acid arrow";   break;
	case  4:
	case  5: spell = "caustic font";     break;
	case  6:
	case  7:
	case  8:
	case  9: spell = "acid blast";       break;
	default: spell = "";     break;
	}

    if ( victim == NULL )
	return FALSE;

    if ( ( sn = skill_lookup( (char*)spell ) ) < 0 )
	return FALSE;
    say_spell(ch,sn);
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim,TARGET_CHAR);
    return TRUE;
}
