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
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "magic.h"
#include "recycle.h"

/* command procedures needed */
DECLARE_DO_FUN(do_look		);
DECLARE_DO_FUN(do_yell		);
DECLARE_DO_FUN(do_say		);
DECLARE_DO_FUN(do_murder	);
DECLARE_DO_FUN(do_kill		);
DECLARE_DO_FUN(do_visible	);

DECLARE_SPEC_FUN(	spec_special_guard	);
DECLARE_SPEC_FUN(	spec_stalker		);

/*
 * Local functions.
 */
void	say_spell	args( ( CHAR_DATA *ch, int sn ) );

/* imported functions */
bool    remove_obj      args( ( CHAR_DATA *ch, OBJ_DATA *obj, bool fReplace ) );
void 	wear_obj	args( ( CHAR_DATA *ch, OBJ_DATA *obj, bool fReplace ) );
bool	cabal_area_check     args( (CHAR_DATA *ch) );
bool	is_at_cabal_area     args( (CHAR_DATA *ch) );


/*
 * Lookup a skill by name.
 */
int skill_lookup( const wchar_t *name )
{
    int sn;

    for ( sn = 0; sn < MAX_SKILL; sn++ )
    {
	if ( skill_table[sn].name == NULL )
	    break;
	if ( towlower(name[0]) == towlower(skill_table[sn].name[0][0])
	&&   !str_prefix( name, skill_table[sn].name[0] ) )
	    return sn;
      if ( towlower(name[0]) == towlower(skill_table[sn].name[1][0])
      &&   !str_prefix( name, skill_table[sn].name[1] ) )
          return sn;
    }

    return -1;
}

int find_spell( CHAR_DATA *ch, const wchar_t *name )
{
    /* finds a spell the character can cast if possible */
    int sn, found = -1;

    if (IS_NPC(ch))
	return skill_lookup(name);

    for ( sn = 0; sn < MAX_SKILL; sn++ )
    {
	if (skill_table[sn].name == NULL)
	    break;
	if (towlower(name[0]) == towlower(skill_table[sn].name[0][0])
	&&  !str_prefix(name,skill_table[sn].name[0]))
	{
	    if ( found == -1)
		found = sn;
	    if (ch->level >= skill_table[sn].skill_level[ch->iclass]
	    &&  ch->pcdata->learned[sn] > 0)
		    return sn;
	}
  if (towlower(name[0]) == towlower(skill_table[sn].name[1][0])
	&&  !str_prefix(name,skill_table[sn].name[1]))
	{
	    if ( found == -1)
		found = sn;
	    if (ch->level >= skill_table[sn].skill_level[ch->iclass]
	    &&  ch->pcdata->learned[sn] > 0)
		    return sn;
	}
    }
    return found;
}



/*
 * Lookup a skill by slot number.
 * Used for object loading.
 */
int slot_lookup( int slot )
{
    extern bool fBootDb;
    int sn;

    if ( slot <= 0 )
	return -1;

    for ( sn = 0; sn < MAX_SKILL; sn++ )
    {
	if ( slot == skill_table[sn].slot )
	    return sn;
    }

    if ( fBootDb )
    {
	bug( L"Slot_lookup: bad slot %d.", slot );
	abort( );
    }

    return -1;
}



/*
 * Utter mystical words for an sn.
 */
void say_spell( CHAR_DATA *ch, int sn )
{
    wchar_t buf  [MAX_STRING_LENGTH];
    wchar_t buf2 [MAX_STRING_LENGTH];
    CHAR_DATA *rch;
    wchar_t *pName;
    int iSyl;
    int length;
    int skill;

    struct syl_type
    {
	const wchar_t *	old;
	const wchar_t *	inew;
    };

    static const struct syl_type syl_table[] =
    {
	{ L" ",		L" "		},
	{ L"ar",		L"abra"		},
	{ L"au",		L"kada"		},
	{ L"bless",	L"fido"		},
	{ L"blind",	L"nose"		},
	{ L"bur",	L"mosa"		},
	{ L"cu",		L"judi"		},
	{ L"de",		L"oculo"		},
	{ L"en",		L"unso"		},
	{ L"light",	L"dies"		},
	{ L"lo",		L"hi"		},
	{ L"mor",	L"zak"		},
	{ L"move",	L"sido"		},
	{ L"ness",	L"lacri"		},
	{ L"ning",	L"illa"		},
	{ L"per",	L"duda"		},
	{ L"ra",		L"gru"		},
	{ L"fresh",	L"ima"		},
	{ L"re",		L"candus"	},
	{ L"son",	L"sabru"		},
	{ L"tect",	L"infra"		},
	{ L"tri",	L"cula"		},
	{ L"ven",	L"nofo"		},
	{ L"a", L"a" }, { L"b", L"b" }, { L"c", L"q" }, { L"d", L"e" },
	{ L"e", L"z" }, { L"f", L"y" }, { L"g", L"o" }, { L"h", L"p" },
	{ L"i", L"u" }, { L"j", L"y" }, { L"k", L"t" }, { L"l", L"r" },
	{ L"m", L"w" }, { L"n", L"i" }, { L"o", L"a" }, { L"p", L"s" },
	{ L"q", L"d" }, { L"r", L"f" }, { L"s", L"g" }, { L"t", L"h" },
	{ L"u", L"j" }, { L"v", L"z" }, { L"w", L"x" }, { L"x", L"n" },
	{ L"y", L"l" }, { L"z", L"k" },
	{ L"", L"" }
    };

    buf[0]	= '\0';
    for ( pName = (wchar_t*)skill_table[sn].name; *pName != '\0'; pName += length )
    {
	for ( iSyl = 0; (length = wcslen(syl_table[iSyl].old)) != 0; iSyl++ )
	{
	    if ( !str_prefix( syl_table[iSyl].old, pName ) )
	    {
		wcscat( buf, syl_table[iSyl].inew );
		break;
	    }
	}

	if ( length == 0 )
	    length = 1;
    }

    swprintf( buf2, MAX_STRING_LENGTH-1, L"$n sihirli sözcükler söylüyor '%s'.", buf );
    swprintf( buf, MAX_STRING_LENGTH-1,  L"$n sihirli sözcükler söylüyor '%s'.", skill_table[sn].name[0] );

    for ( rch = ch->in_room->people; rch; rch = rch->next_in_room )
    {
	if ( rch != ch )
	{
         skill = (get_skill(rch,gsn_spell_craft) * 9) / 10;
	 if (skill < number_percent() )
	  {
	    act(buf2 , ch, NULL, rch, TO_VICT );
	    check_improve( rch, gsn_spell_craft, TRUE, 5 );
	  }
	 else
	  {
	    act( buf, ch, NULL, rch, TO_VICT );
	    check_improve( rch, gsn_spell_craft, TRUE, 5 );
	  }
	}
    }

    return;
}



/*
 * Compute a saving throw.
 * Negative apply's make saving throw better.
 */
bool saves_spell( int level, CHAR_DATA *victim, int dam_type )
{
    int save;

    save = 40 + ( victim->level - level) * 4 -
	(victim->saving_throw * 90) / UMAX(45,victim->level);

    if (IS_AFFECTED(victim,AFF_BERSERK))
	save += victim->level / 5;

    switch(check_immune(victim,dam_type))
    {
	case IS_IMMUNE:		return TRUE;
	case IS_RESISTANT:	save += victim->level / 5;	break;
	case IS_VULNERABLE:	save -= victim->level / 5;	break;
    }

    if (!IS_NPC(victim) && class_table[victim->iclass].fMana)
	save = 9 * save / 10;
    save = URANGE( 5, save, 95 );
    return number_percent( ) < save;
}

/* RT configuration smashed */

bool saves_dispel( int dis_level, int spell_level, int duration)
{
    int save;

      /* impossible to dispel permanent effects */
    if (duration == -2 ) return 1;
    if (duration == -1 ) spell_level += 5;

    save = 50 + (spell_level - dis_level) * 5;
    save = URANGE( 5, save, 95 );
    return number_percent( ) < save;
}

/* co-routine for dispel magic and cancellation */

bool check_dispel( int dis_level, CHAR_DATA *victim, int sn)
{
    AFFECT_DATA *af;

    if (is_affected(victim, sn))
    {
	for ( af = victim->affected; af != NULL; af = af->next )
        {
            if ( af->type == sn )
            {
                if (!saves_dispel(dis_level,af->level,af->duration))
                {
                    affect_strip(victim,sn);
        	    if ( skill_table[sn].msg_off )
        	    {
            		send_to_char( skill_table[sn].msg_off, victim );
            		send_to_char( L"\n\r", victim );
        	    }
		    return TRUE;
		}
		else
		    af->level--;
            }
        }
    }
    return FALSE;
}

/* for finding mana costs -- temporary version */
int mana_cost (CHAR_DATA *ch, int min_mana, int level)
{
    if (ch->level + 2 == level)
	return 1000;
    return UMAX(min_mana,(100/(2 + ch->level - level)));
}

/*
 * for casting different rooms
 * returned value is the range
 */
int allowed_other( CHAR_DATA *ch, int sn)
{
 if (skill_table[sn].minimum_position == POS_STANDING
  	|| skill_table[sn].skill_level[ch->iclass] < 26
	|| sn == find_spell(ch,L"chain lightning") ) return 0;
 else return skill_table[sn].skill_level[ch->iclass] / 10;
}

/*
 * The kludgy global is for spells who want more stuff from command line.
 */
wchar_t *target_name;

void do_cast( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg1[MAX_INPUT_LENGTH];
    wchar_t arg2[MAX_INPUT_LENGTH];
    wchar_t buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    void *vo;
    int mana;
    int sn;
    int target;
    int cast_far = 0, door, range;

    /*
     * Switched NPC's can cast spells, but others can't.
     */
    if ( IS_NPC(ch) && ch->desc == NULL)
	return;

     if (is_affected(ch, gsn_shielding) )
	{
    send_to_char( L"Gerçek Kaynağa erişeceğin sırada birşeyin seni engellediğini hissediyorsun.\n\r",ch);
	 return;
	}

     if (is_affected(ch, gsn_garble) || is_affected(ch, gsn_deafen))
     {
       send_to_char( L"Doğru tınıyı yakalayamıyorsun.\n\r", ch);
        return;
     }

    target_name = one_argument( argument, arg1 );
    one_argument( target_name, arg2 );

    if ( arg1[0] == '\0' )
    {
      send_to_char( L"Hangi büyüyü nereye-kime yapacaksın?\n\r", ch );
	return;
    }

    if (ch->cabal == CABAL_BATTLE && !IS_IMMORTAL(ch))
    {
      send_to_char( L"Sen Öfke Kabalı üyesisin, pis bir büyücü değil!\n\r",ch);
      return;
    }

    if (  ( sn = find_spell( ch,arg1 ) ) < 0
          || ch_skill_nok_nomessage(ch,sn) )
    {
      send_to_char( L"Bu isimde büyü bilmiyorsun.\n\r", ch );
	return;
    }

    if ( ch->iclass == CLASS_VAMPIRE
	&& !IS_VAMPIRE(ch) && skill_table[sn].cabal == CABAL_NONE)
    {
      send_to_char( L"Büyüler için önce vampire dönüşmelisin!\n\r",ch);
      return;
    }

    if ( skill_table[sn].spell_fun == spell_null )
    {
      send_to_char( L"Bu bir büyü değil.\n\r",ch);
        return;
    }

    if ( ch->position < skill_table[sn].minimum_position )
    {
      send_to_char( L"Yeterince konsantre olamıyorsun.\n\r", ch );
	return;
    }

    if (!cabal_ok(ch,sn))
      return;

    if (IS_SET(ch->in_room->room_flags,ROOM_NO_MAGIC))
    {
      send_to_char( L"Büyün başarısız oldu.\n\r",ch);
      act( L"$s büyüsü başarısız oldu.", ch, NULL, NULL, TO_ROOM);
        return;
    }

    if (ch->level + 2 == skill_table[sn].skill_level[ch->iclass])
	mana = 50;
    else
	mana = UMAX(
	    skill_table[sn].min_mana,
	    100 / ( 2 + ch->level - skill_table[sn].skill_level[ch->iclass] ) );

    /*
     * Locate targets.
     */
    victim	= NULL;
    obj		= NULL;
    vo		= NULL;
    target	= TARGET_NONE;

    switch ( skill_table[sn].target )
    {
    default:
	bug( L"Do_cast: bad target for sn %d.", sn );
	return;

    case TAR_IGNORE:
        if ( is_affected(ch,gsn_spellbane))
          {
	    WAIT_STATE( ch, skill_table[sn].beats );
      act( L"Büyüyıkımın büyüyü saptırıyor!",ch,NULL,NULL,TO_CHAR);
      act( L"$s büyüyıkımı büyüyü saptırıyor!",ch,NULL,NULL,TO_ROOM);
	    check_improve(ch,gsn_spellbane,TRUE,1);
            damage(ch,ch,3 * ch->level,gsn_spellbane,DAM_NEGATIVE, TRUE);
            return;
          }
 	break;

    case TAR_CHAR_OFFENSIVE:
	if ( arg2[0] == '\0' )
	{
	    if ( ( victim = ch->fighting ) == NULL )
	    {
        send_to_char( L"Büyüyü kime yapacaksın?\n\r", ch );
		return;
	    }
	}
	else
	{
	    if ( (range = allowed_other(ch,sn)) > 0 )
	    {
 	     if ((victim=get_char_spell(ch,target_name,&door,range)) == NULL)
	      return;

	     if (victim->in_room != ch->in_room
		&& ( (IS_NPC(victim) && IS_SET(victim->act,ACT_NOTRACK))
		   || is_at_cabal_area(ch) || is_at_cabal_area(victim) ) )
	     {
         act( L"Bu büyüyü $E bu mesafeden yapamazsın.",
			ch,NULL,victim,TO_CHAR);
	      return;
	     }

	     cast_far = 1;
	    }
	    else if ( ( victim = get_char_room( ch, target_name ) ) == NULL )
	    {
		send_to_char( L"O burada değil.\n\r", ch );
		return;
	    }
	}

	if ( !IS_NPC(ch) && is_safe(ch,victim) )
		return;
/*
	if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim )
	{
	    send_to_char( L"You can't do that on your own follower.\n\r",
		ch );
	    return;
	}
*/
	vo = (void *) victim;
	target = TARGET_CHAR;
        if (!IS_NPC(ch) && victim != ch &&
            ch->fighting != victim && victim->fighting != ch &&
            (IS_SET(victim->affected_by,AFF_CHARM) || !IS_NPC(victim)))
          {
            if (!can_see(victim, ch))
                do_yell(victim, (wchar_t*)"İmdat! Biri bana saldırıyor!");
            else
              {
                swprintf( buf, MAX_STRING_LENGTH-1, L"Geber %s, seni büyücü köpek!",
                    (is_affected(ch,gsn_doppelganger)&&!IS_IMMORTAL(victim))?
                     ch->doppel->name : ch->name);
                 do_yell(victim,buf);
              }
          }
        if ( is_affected(victim,gsn_spellbane) &&
            (number_percent() < 2*get_skill(victim, gsn_spellbane)/3)
		&& sn != slot_lookup(524)  && sn != slot_lookup(204) )
          {
	    WAIT_STATE( ch, skill_table[sn].beats );
            if (ch==victim)
            {
              act( L"Büyüyıkımın büyüyü yansıtıyor!",ch,NULL,NULL,TO_CHAR);
              act( L"$s büyüyıkımı büyüyü saptırıyor!",ch,NULL,NULL,TO_ROOM);
	        check_improve(victim,gsn_spellbane,TRUE,1);
                damage(ch,ch,3 * ch->level,gsn_spellbane,DAM_NEGATIVE,TRUE);
            }
            else
	    {
        act( L"$N büyünü saptırıyor!",ch,NULL,victim,TO_CHAR);
				act( L"$s büyüsünü saptırıyorsun!",ch,NULL,victim,TO_VICT);
				act( L"$N $s büyüsünü saptırıyor!",ch,NULL,victim,TO_NOTVICT);
	      check_improve(victim,gsn_spellbane,TRUE,1);
              damage(victim,ch,3 * victim->level,gsn_spellbane,DAM_NEGATIVE, TRUE);
              multi_hit(victim,ch,TYPE_UNDEFINED);
            }
            return;
          }
        if ( ch != victim && CAN_DETECT(victim, ADET_ABSORB) &&
            (number_percent() < 2*get_skill(victim,gsn_absorb)/3)
		&& sn != slot_lookup(524) && sn != slot_lookup(204))
	{
    act( L"Büyün $S enerji alanını geçemiyor!",ch,NULL,victim,TO_CHAR);
          act( L"$s büyüsünü soğuruyorsun!",ch,NULL,victim,TO_VICT);
          act( L"$N $s büyüsünü soğuruyor!",ch,NULL,victim,TO_NOTVICT);
	    check_improve(victim,gsn_absorb,TRUE,1);
	    victim->mana += mana;
	    return;
	}
	break;

    case TAR_CHAR_DEFENSIVE:
	if ( arg2[0] == '\0' )
	{
	    victim = ch;
	}
	else
	{
	    if ( ( victim = get_char_room( ch, target_name ) ) == NULL )
	    {
		send_to_char( L"O burada değil.\n\r", ch );
		return;
	    }
	}

	vo = (void *) victim;
	target = TARGET_CHAR;
        if ( is_affected(victim,gsn_spellbane) )
          {
	    WAIT_STATE( ch, skill_table[sn].beats );
            if (ch==victim)
              {
                act( L"Büyüyıkımın büyüyü yansıtıyor!",ch,NULL,NULL,TO_CHAR);
                act( L"$s büyüyıkımı büyüyü saptırıyor!",ch,NULL,NULL,TO_ROOM);
	        check_improve(victim,gsn_spellbane,TRUE,1);
                damage(victim,ch,3 * victim->level,gsn_spellbane,DAM_NEGATIVE, TRUE);              }
            else {
              act( L"$N büyünü saptırıyor!",ch,NULL,victim,TO_CHAR);
              act( L"$s büyüsünü saptırıyorsun!",ch,NULL,victim,TO_VICT);
              act( L"$N $s büyüsünü saptırıyor!",ch,NULL,victim,TO_NOTVICT);
	      check_improve(victim,gsn_spellbane,TRUE,1);
              damage(victim,ch,3 * victim->level,gsn_spellbane,DAM_NEGATIVE, TRUE);
            }
            return;
          }
        if ( ch != victim && CAN_DETECT(victim, ADET_ABSORB) &&
            (number_percent() < 2*get_skill(victim,gsn_absorb)/3) )
	{
    act( L"Büyün $S enerji alanını geçemiyor!",ch,NULL,victim,TO_CHAR);
    act( L"$s büyüsünü soğuruyorsun!",ch,NULL,victim,TO_VICT);
    act( L"$N $s büyüsünü soğuruyor!",ch,NULL,victim,TO_NOTVICT);
	    check_improve(victim,gsn_absorb,TRUE,1);
	    victim->mana += mana;
	    return;
	}
	break;

    case TAR_CHAR_SELF:
	if ( arg2[0] != '\0' && !is_name( target_name, ch->name ) )
	{
    send_to_char( L"Bu büyüyü başkasına yapamazsın.\n\r", ch );
	    return;
	}

	vo = (void *) ch;
	target = TARGET_CHAR;

        if ( is_affected(ch,gsn_spellbane) )
          {
	    WAIT_STATE( ch, skill_table[sn].beats );
      act( L"Büyüyıkımın büyüyü saptırıyor!",ch,NULL,NULL,TO_CHAR);
      act( L"$s büyüyıkımı büyüyü saptırıyor!",ch,NULL,NULL,TO_ROOM);
	    check_improve(ch,gsn_spellbane,TRUE,1);
            damage(ch,ch,3 * ch->level,gsn_spellbane,DAM_NEGATIVE, TRUE);
            return;
          }

	break;

    case TAR_OBJ_INV:
	if ( arg2[0] == '\0' )
	{
    send_to_char( L"Büyü kime yapılacak?\n\r", ch );
	    return;
	}

	if ( ( obj = get_obj_carry( ch, target_name ) ) == NULL )
	{
    send_to_char( L"Onu taşımıyorsun.\n\r", ch );
	    return;
	}

	vo = (void *) obj;
	target = TARGET_OBJ;
        if ( is_affected(ch,gsn_spellbane) )
          {
	    WAIT_STATE( ch, skill_table[sn].beats );
      act( L"Büyüyıkımın büyüyü saptırıyor!",ch,NULL,NULL,TO_CHAR);
      act( L"$s büyüyıkımı büyüyü saptırıyor!",ch,NULL,NULL,TO_ROOM);
	    check_improve(ch,gsn_spellbane,TRUE,1);
            damage(ch,ch,3 * ch->level,gsn_spellbane,DAM_NEGATIVE, TRUE);
            return;
          }
	break;

    case TAR_OBJ_CHAR_OFF:
	if (arg2[0] == '\0')
	{
	    if ((victim = ch->fighting) == NULL)
	    {
		send_to_char( L"Büyüyü kime veya neye yapacaksın?\n\r",ch);
		return;
	    }

	    target = TARGET_CHAR;
	}
	else if ((victim = get_char_room(ch,target_name)) != NULL)
	{
	    target = TARGET_CHAR;
	}

	if (target == TARGET_CHAR) /* check the sanity of the attack */
	{
	    if(is_safe_spell(ch,victim,FALSE) && victim != ch)
	    {
        send_to_char( L"Büyün işe yaramadı.\n\r",ch);
		return;
	    }

	    if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim )
	    {
        send_to_char( L"Takipçine bunu yapamazsın.\n\r",ch );

		return;
	    }

	    if ( is_safe(ch, victim) )
		return;

	    vo = (void *) victim;
	}
	else if ((obj = get_obj_here(ch,target_name)) != NULL)
	{
	    vo = (void *) obj;
	    target = TARGET_OBJ;
	}
	else
	{
    send_to_char( L"Öyle bir şey görmüyorsun.\n\r",ch );
	    return;
	}
	break;

    case TAR_OBJ_CHAR_DEF:
	if (arg2[0] == '\0')
	{
	    vo = (void *) ch;
	    target = TARGET_CHAR;
	}
	else if ((victim = get_char_room(ch,target_name)) != NULL)
	{
	    vo = (void *) victim;
	    target = TARGET_CHAR;
	}
	else if ((obj = get_obj_carry(ch,target_name)) != NULL)
	{
	    vo = (void *) obj;
	    target = TARGET_OBJ;
	}
	else
	{
	    send_to_char( L"Öyle bir şey görmüyorsun.\n\r",ch);
	    return;
	}
	break;
    }

    if ( !IS_NPC(ch) && ch->mana < mana )
    {
      send_to_char( L"Yeterli manan yok.\n\r", ch );
	return;
    }

    if ( wcscasecmp( skill_table[sn].name[0], L"ventriloquate" ) )
	say_spell( ch, sn );

    WAIT_STATE( ch, skill_table[sn].beats );

    if ( number_percent( ) > get_skill(ch,sn) )
    {
      send_to_char( L"Konsantrasyonunu kaybettin.\n\r", ch );
	check_improve(ch,sn,FALSE,1);
	ch->mana -= mana / 2;
	if (cast_far) cast_far = 2;
    }
    else
    {
	int slevel;

	if (class_table[ch->iclass].fMana)
		slevel = ch->level - UMAX(0,(ch->level / 20));
	else	slevel = ch->level - UMAX(5,(ch->level / 10));

	if (skill_table[sn].cabal != CABAL_NONE)
		slevel = ch->level;

	if ( ch->level > skill_table[gsn_spell_craft].skill_level[ch->iclass])
	 {
	  if (number_percent() < get_skill(ch,gsn_spell_craft) )
		{
		 slevel = ch->level;
		 check_improve( ch, gsn_spell_craft, TRUE, 1 );
		}
	  check_improve( ch, gsn_spell_craft, FALSE, 1);
	 }

	if (ch->cabal == CABAL_SHALAFI &&
	    ch->level > skill_table[gsn_mastering_spell].skill_level[ch->iclass]
	    && cabal_ok(ch,gsn_mastering_spell) )
	 {
	  if (number_percent() < get_skill(ch,gsn_mastering_spell) )
		{
		 slevel += number_range(1,4);
		 check_improve( ch, gsn_mastering_spell, TRUE, 1 );
		}
	 }

	ch->mana -= mana;
	if (get_curr_stat(ch,STAT_INT) > 21 )
		slevel = UMAX(1,(slevel + (get_curr_stat(ch,STAT_INT) - 21)) );
	else	slevel = UMAX(1, slevel );

	if (IS_NPC(ch))
	    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, vo,target);
	else
	    (*skill_table[sn].spell_fun) (sn, slevel , ch, vo,target);
	check_improve(ch,sn,TRUE,1);
    }

    if (cast_far == 1 && door != -1)
	path_to_track(ch,victim,door);
    else if ((skill_table[sn].target == TAR_CHAR_OFFENSIVE
    ||   (skill_table[sn].target == TAR_OBJ_CHAR_OFF && target == TARGET_CHAR))
    &&   victim != ch
    &&   victim->master != ch)
    {
	CHAR_DATA *vch;
	CHAR_DATA *vch_next;

	for ( vch = ch->in_room->people; vch; vch = vch_next )
	{
	    vch_next = vch->next_in_room;
	    if ( victim == vch && victim->fighting == NULL )
	    {
		if ( victim->position != POS_SLEEPING )
		multi_hit( victim, ch, TYPE_UNDEFINED );

		break;
	    }
	}
    }

    return;
}



/*
 * Cast spells at targets using a magical object.
 */
void obj_cast_spell( int sn, int level, CHAR_DATA *ch, CHAR_DATA *victim, OBJ_DATA *obj )
{
    void *vo;
    int target = TARGET_NONE;

    if ( sn <= 0 )
	return;

    if ( sn >= MAX_SKILL || skill_table[sn].spell_fun == 0 )
    {
	bug( L"Obj_cast_spell: bad sn %d.", sn );
	return;
    }

    if ( (IS_NPC(ch) && ch->position == POS_DEAD) ||
        (!IS_NPC(ch) && (current_time - ch->last_death_time) < 10))
    {
	bug( L"Obj_cast_spell: Ch is dead! But it is Tamam.", sn);
	return;
    }

    if (victim != NULL)
    {
      if ( (IS_NPC(victim) && victim->position == POS_DEAD) ||
        (!IS_NPC(victim) && (current_time - victim->last_death_time) < 10))
      {
	bug( L"Obj_cast_spell: Victim is dead! But it is Tamam.. ", sn);
	return;
      }
    }

    switch ( skill_table[sn].target )
    {
    default:
	bug( L"Obj_cast_spell: bad target for sn %d.", sn );
	return;

    case TAR_IGNORE:
	vo = NULL;
	break;

    case TAR_CHAR_OFFENSIVE:
	if ( victim == NULL )
	    victim = ch->fighting;
	if ( victim == NULL )
	{
	    send_to_char( L"Bunu yapamazsın.\n\r", ch );
	    return;
	}
	if (is_safe(ch,victim) && ch != victim)
	{
    send_to_char( L"Birşeyler yanlış...\n\r",ch);
	    return;
	}
	vo = (void *) victim;
	target = TARGET_CHAR;
        if ( is_affected(victim,gsn_spellbane) && (/*IS_NPC(victim) ||*/
                number_percent() < 2*get_skill(victim, gsn_spellbane)/3) )
        {
            if (ch==victim)
              {
                act( L"Büyüyıkımın büyüyü yansıtıyor!",ch,NULL,NULL,TO_CHAR);
                act( L"$s büyüyıkımı büyüyü saptırıyor!",ch,NULL,NULL,TO_ROOM);
	        check_improve(victim,gsn_spellbane,TRUE,1);
                damage(victim,ch,10 * level,gsn_spellbane,DAM_NEGATIVE, TRUE);
              }
            else {
              act( L"$N büyünü saptırıyor!",ch,NULL,victim,TO_CHAR);
              act( L"$s büyüsünü saptırıyorsun!",ch,NULL,victim,TO_VICT);
              act( L"$N $s büyüsünü saptırıyor!",ch,NULL,victim,TO_NOTVICT);
	      check_improve(victim,gsn_spellbane,TRUE,1);
              damage(victim,ch,10 * victim->level,gsn_spellbane,DAM_NEGATIVE, TRUE);
            }
            return;
        }

        if ( ch != victim && CAN_DETECT(victim, ADET_ABSORB) &&
            (number_percent() < 2*get_skill(victim,gsn_absorb)/3)
		&& sn != slot_lookup(524) && sn != slot_lookup(204))
	{
    act( L"Büyün $S enerji alanını geçemiyor!",ch,NULL,victim,TO_CHAR);
    act( L"$s büyüsünü soğuruyorsun!",ch,NULL,victim,TO_VICT);
    act( L"$N $s büyüsünü soğuruyor!",ch,NULL,victim,TO_NOTVICT);
	    check_improve(victim,gsn_absorb,TRUE,1);
	    victim->mana += skill_table[sn].min_mana;
	    return;
	}
	break;

    case TAR_CHAR_DEFENSIVE:
    case TAR_CHAR_SELF:
	if ( victim == NULL )
	    victim = ch;
	vo = (void *) victim;
	target = TARGET_CHAR;
        if ( is_affected(victim,gsn_spellbane) )
          {
            if (ch==victim)
              {
                act( L"Büyüyıkımın büyüyü yansıtıyor!",ch,NULL,NULL,TO_CHAR);
                act( L"$s büyüyıkımı büyüyü saptırıyor!",ch,NULL,NULL,TO_ROOM);
	        check_improve(victim,gsn_spellbane,TRUE,1);
                damage(victim,ch,10 * victim->level,gsn_spellbane,DAM_NEGATIVE, TRUE);
              }
            else {
              act( L"$N büyünü saptırıyor!",ch,NULL,victim,TO_CHAR);
              act( L"$s büyüsünü saptırıyorsun!",ch,NULL,victim,TO_VICT);
              act( L"$N $s büyüsünü saptırıyor!",ch,NULL,victim,TO_NOTVICT);
	      check_improve(victim,gsn_spellbane,TRUE,1);
              damage(victim,ch,10 * victim->level,gsn_spellbane,DAM_NEGATIVE, TRUE);
            }
            return;
          }
        break;

    case TAR_OBJ_INV:
	if ( obj == NULL )
	{
	    send_to_char( L"Bunu yapamazsın.\n\r", ch );
	    return;
	}
	vo = (void *) obj;
	target = TARGET_OBJ;
        if ( is_affected(ch,gsn_spellbane) )
          {
            act( L"Büyüyıkımın büyüyü saptırıyor!",ch,NULL,NULL,TO_CHAR);
            act( L"$s büyüyıkımı büyüyü saptırıyor!",ch,NULL,NULL,TO_ROOM);
	    check_improve(ch,gsn_spellbane,TRUE,1);
            damage(ch,ch,3 * ch->level,gsn_spellbane,DAM_NEGATIVE, TRUE);
            return;
          }

	break;

    case TAR_OBJ_CHAR_OFF:
	if ( victim == NULL && obj == NULL)
	{
	    if (ch->fighting != NULL)
		victim = ch->fighting;
	    else
	    {
		send_to_char( L"Bunu yapamazsın.\n\r",ch);
		return;
	    }
	}

	    if (victim != NULL)
	    {
		if (is_safe_spell(ch,victim,FALSE) && ch != victim)
		{
      send_to_char( L"Bir şeyler yanlış...\n\r",ch);
		    return;
		}

		vo = (void *) victim;
		target = TARGET_CHAR;
	    }
	    else
	    {
		vo = (void *) obj;
		target = TARGET_OBJ;
	    }
	break;


    case TAR_OBJ_CHAR_DEF:
	if (victim == NULL && obj == NULL)
	{
	    vo = (void *) ch;
	    target = TARGET_CHAR;
	}
	else if (victim != NULL)
	{
	    vo = (void *) victim;
	    target = TARGET_CHAR;
	}
	else
	{
	    vo = (void *) obj;
	    target = TARGET_OBJ;
	}

	break;
    }

    target_name = (wchar_t*)"";
    (*skill_table[sn].spell_fun) ( sn, level, ch, vo,target);



    if ( (skill_table[sn].target == TAR_CHAR_OFFENSIVE
    ||   (skill_table[sn].target == TAR_OBJ_CHAR_OFF && target == TARGET_CHAR))
    &&   victim != ch
    &&   victim->master != ch )
    {
	CHAR_DATA *vch;
	CHAR_DATA *vch_next;

	for ( vch = ch->in_room->people; vch; vch = vch_next )
	{
	    vch_next = vch->next_in_room;
	    if ( victim == vch && victim->fighting == NULL )
	    {
		multi_hit( victim, ch, TYPE_UNDEFINED );

		break;
	    }
	}
    }

    return;
}



/*
 * Spell functions.
 */
void spell_acid_blast( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = dice( level, 18 );
    if ( saves_spell( level, victim, DAM_ACID ) )
	dam /= 2;
    damage( ch, victim, dam, sn,DAM_ACID,TRUE);
    return;
}



void spell_armor( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
    {
	if (victim == ch)
  send_to_char( L"Zaten zırhlısın.\n\r",ch);
	else
  act( L"$N zaten zırhlı.",ch,NULL,victim,TO_CHAR);
	return;
    }
    af.where	 = TO_AFFECTS;
    af.type      = sn;
    af.level	 = level;
    af.duration  = 7 + level / 6;
    af.modifier  = -1 * UMAX(20,10 + level / 4); /* af.modifier  = -20;*/
    af.location  = APPLY_AC;
    af.bitvector = 0;
    affect_to_char( victim, &af );
    send_to_char( L"Birinin seni koruduğunu hissediyorsun.\n\r", victim );
    if ( ch != victim )
    act( L"$N büyünle korunuyor.",ch,NULL,victim,TO_CHAR);
    return;
}



void spell_bless( int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    AFFECT_DATA af;

    /* deal with the object case first */
    if (target == TARGET_OBJ)
    {
	obj = (OBJ_DATA *) vo;
	if (IS_OBJ_STAT(obj,ITEM_BLESS))
	{
    act( L"$p zaten kutsanmış.",ch,obj,NULL,TO_CHAR);
	    return;
	}

	if (IS_OBJ_STAT(obj,ITEM_EVIL))
	{
	    AFFECT_DATA *paf;

	    paf = affect_find(obj->affected,gsn_curse);
	    if (!saves_dispel(level,paf != NULL ? paf->level : obj->level,0))
	    {
		if (paf != NULL)
		    affect_remove_obj(obj,paf);
        act( L"$p soluk mavi renkte parlıyor.",ch,obj,NULL,TO_ALL);
		REMOVE_BIT(obj->extra_flags,ITEM_EVIL);
		return;
	    }
	    else
	    {
        act( L"$p içindeki şer üstesinden gelemeyeceğin kadar güçlü.",
		    ch,obj,NULL,TO_CHAR);
		return;
	    }
	}

	af.where	= TO_OBJECT;
	af.type		= sn;
	af.level	= level;
	af.duration	= (6 + level / 2);
	af.location	= APPLY_SAVES;
	af.modifier	= -1;
	af.bitvector	= ITEM_BLESS;
	affect_to_obj(obj,&af);

  act( L"$p kutsal aurayla parlıyor.",ch,obj,NULL,TO_ALL);
	return;
    }

    /* character target */
    victim = (CHAR_DATA *) vo;


    if ( victim->position == POS_FIGHTING || is_affected( victim, sn ) )
    {
	if (victim == ch)
  send_to_char( L"Zaten kutsanmışsın.\n\r",ch);
else
  act( L"$N zaten kutsal desteğe sahip.",ch,NULL,victim,TO_CHAR);
	return;
    }

    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level	 = level;
    af.duration  = (6 + level / 2);
    af.location  = APPLY_HITROLL;
    af.modifier  = level / 8;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    af.location  = APPLY_SAVING_SPELL;
    af.modifier  = 0 - level / 8;
    affect_to_char( victim, &af );
    send_to_char( L"Kutsanmış hissediyorsun.\n\r", victim );
    if ( ch != victim )
	act( L"$E tanrının desteğini sunuyorsun.",ch,NULL,victim,TO_CHAR);
    return;
}



void spell_blindness( int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_BLIND) ||
        saves_spell(level,victim,DAM_OTHER))  {
      send_to_char( L"Başaramadın.\n\r", ch );
	return;
    }

    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.location  = APPLY_HITROLL;
    af.modifier  = -4;
    af.duration  = 3+level / 15;
    af.bitvector = AFF_BLIND;
    affect_to_char( victim, &af );
    send_to_char( L"Kör oldun!\n\r", victim );
    act( L"$n kör oldu.",victim,NULL,NULL,TO_ROOM);
    return;
}



void spell_burning_hands(int sn,int level, CHAR_DATA *ch, void *vo, int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = dice(level , 2) + 7;
    if ( saves_spell( level, victim,DAM_FIRE) )
	dam /= 2;
    damage( ch, victim, dam, sn, DAM_FIRE,TRUE);
    return;
}



void spell_call_lightning( int sn, int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    int dam;

    if ( !IS_OUTSIDE(ch) )
    {
      send_to_char( L"Kapıların dışında olmalısın.\n\r", ch );
	return;
    }

    if ( weather_info.sky < SKY_RAINING )
    {
      send_to_char( L"Kötü havaya ihtiyacın var.\n\r", ch );
	return;
    }

    dam = dice(level, 14);

    send_to_char( L"Tanrıların şimşeği düşmanlarını vuruyor!\n\r", ch );
    act( L"$n düşmanlarını vurması için şimşek çağırıyor!",
	ch, NULL, NULL, TO_ROOM );

    for ( vch = char_list; vch != NULL; vch = vch_next )
    {
	vch_next	= vch->next;
	if ( vch->in_room == NULL )
	    continue;
	if (vch == ch)
	    continue;
	if ( vch->in_room == ch->in_room )
	{
	    if ( vch != ch && !is_same_group(ch, vch) )
	      if (is_safe(ch, vch))
		continue;

	    if (CAN_DETECT(vch, ADET_GROUNDING))
	    {
        send_to_char( L"Elektrik düşmanlarında yitip gidiyor.\n\r",vch);
    		act( L"Bir yıldırım $S düşmanlarında yitip gidiyor.\n\r",
			ch, NULL, vch, TO_ROOM);
		continue;
	    }

	    if (saves_spell(level, vch, DAM_LIGHTNING))
		dam /= 2;
	    damage( ch, vch, dam, sn, DAM_LIGHTNING,TRUE);
	    continue;
	}

	if ( vch->in_room->area == ch->in_room->area
	&&   IS_OUTSIDE(vch)
	&&   IS_AWAKE(vch) )
  send_to_char( L"Gökyüzünde şimşekler çakıyor.\n\r", vch );
    }

    return;
}

/* RT calm spell stops all fighting in the room */

void spell_calm( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *vch;
    int mlevel = 0;
    int count = 0;
    int high_level = 0;
    int chance;
    AFFECT_DATA af;

    /* get sum of all mobile levels in the room */
    for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
    {
	if (vch->position == POS_FIGHTING)
	{
	    count++;
	    if (IS_NPC(vch))
	      mlevel += vch->level;
	    else
	      mlevel += vch->level/2;
	    high_level = UMAX(high_level,vch->level);
	}
    }

    /* compute chance of stopping combat */
    chance = 4 * level - high_level + 2 * count;

    if (IS_IMMORTAL(ch)) /* always works */
      mlevel = 0;

    if (number_range(0, chance) >= mlevel)  /* hard to stop large fights */
    {
	for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
   	{
	    if (IS_NPC(vch) && (IS_SET(vch->imm_flags,IMM_MAGIC) ||
				IS_SET(vch->act,ACT_UNDEAD)))
	      return;

	    if (IS_AFFECTED(vch,AFF_CALM) || IS_AFFECTED(vch,AFF_BERSERK)
	    ||  is_affected(vch,skill_lookup( L"frenzy")))
	      return;

        send_to_char( L"Üzerinden bir sakinlik dalgası geçiyor.\n\r",vch);

	    if (vch->fighting || vch->position == POS_FIGHTING)
	      stop_fighting(vch,FALSE);


	    af.where = TO_AFFECTS;
	    af.type = sn;
	    af.level = level;
	    af.duration = level/4;
	    af.location = APPLY_HITROLL;
	    if (!IS_NPC(vch))
	      af.modifier = -5;
	    else
	      af.modifier = -2;
	    af.bitvector = AFF_CALM;
	    affect_to_char(vch,&af);

	    af.location = APPLY_DAMROLL;
	    affect_to_char(vch,&af);
	}
    }
}

void spell_cancellation( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    bool found = FALSE;

    level += 2;

    if ((!IS_NPC(ch) && IS_NPC(victim) &&
	 !(IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim) ) ||
	(IS_NPC(ch) && !IS_NPC(victim)) )
    {
      send_to_char( L"Başarısız oldun, BÜYÜ DEFET'i dene.\n\r",ch);
	return;
    }

    if ( !is_same_group(ch, victim) && ch != victim
	 && (IS_NPC(victim) || IS_SET(victim->act, PLR_NOCANCEL)) )
    {
      act( L"Bu büyüyü $Y kullanamazsın.", ch, NULL, victim, TO_CHAR);
	return;
    }

    /* unlike dispel magic, the victim gets NO save */

    /* begin running through the spells */

    if (check_dispel(level,victim,skill_lookup( L"armor")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup( L"enhanced armor")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup( L"bless")))
        found = TRUE;

    if (check_dispel(level,victim,skill_lookup( L"blindness")))
    {
        found = TRUE;
        act( L"$n artık kör değil.",victim,NULL,NULL,TO_ROOM);
    }

    if (check_dispel(level,victim,skill_lookup( L"calm")))
    {
	found = TRUE;
  act( L"$n artık barışçıl görünmüyor...",victim,NULL,NULL,TO_ROOM);
    }

    if (check_dispel(level,victim,skill_lookup( L"change sex")))
    {
        found = TRUE;
        act( L"$n artık kendisi gibi görünüyor.",victim,NULL,NULL,TO_ROOM);
    }

    if (check_dispel(level,victim,skill_lookup( L"charm person")))
    {
	found = TRUE;
  act( L"$n özgür iradesini kazanıyor.",victim,NULL,NULL,TO_ROOM);
    }

    if (check_dispel(level,victim,skill_lookup( L"chill touch")))
    {
	found = TRUE;
  act( L"$n ısınmış görünüyor.",victim,NULL,NULL,TO_ROOM);
    }

    if (check_dispel(level,victim,skill_lookup( L"curse")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup( L"detect evil")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup( L"detect good")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup( L"detect hidden")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup( L"detect invis")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup( L"detect hidden")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup( L"detect magic")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup( L"faerie fire")))
    {
      act( L"$s aurası yokoluyor.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup( L"fly")))
    {
      act( L"$n yere düşüyor!",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup( L"frenzy")))
    {
      act( L"$n artık vahşi görünmüyor.",victim,NULL,NULL,TO_ROOM);;
	found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup( L"giant strength")))
    {
      act( L"$n gücünü yitirmiş görünüyor.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup( L"haste")))
    {
      act( L"$n hızını yitirmiş görünüyor.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup( L"infravision")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup( L"invis")))
    {
      act( L"$n varlığa dönüyor.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup( L"mass invis")))
    {
      act( L"$n varlığa dönüyor.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup( L"pass door")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup( L"protection evil")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup( L"protection good")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup( L"sanctuary")))
    {
      act( L"$s çevresindeki beyaz aura yokoluyor.",
	    victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup( L"shield")))
    {
      act( L"$m koruyan kalkan yokoluyor.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup( L"sleep")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup( L"slow")))
    {
      act( L"$n eski hızını kazanmış görünüyor.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup( L"stone skin")))
    {
      act( L"$s derisi eski haline dönüyor.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup( L"weaken")))
    {
      act( L"$n güçlenmiş görünüyor.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup( L"shielding")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup( L"web")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup( L"fear")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup( L"protection heat")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup( L"protection cold")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup( L"magic resistance")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup( L"hallucination")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup( L"terangreal")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup( L"power word stun")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup( L"corruption")))
    {
      act( L"$n daha sağlıklı görünüyor.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup( L"web")))
    {
      act( L"$s çevresindeki ağlar eriyor.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }

    if (found)
        send_to_char( L"Tamam.\n\r",ch);
    else
        send_to_char( L"Büyü işe yaramadı.\n\r",ch);
}

void spell_cause_light( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    damage( ch, (CHAR_DATA *) vo, dice(1, 8) + level / 3, sn,DAM_HARM,TRUE);
    return;
}



void spell_cause_critical(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    damage( ch, (CHAR_DATA *) vo, dice(3, 8) + level - 6, sn,DAM_HARM,TRUE);
    return;
}



void spell_cause_serious(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    damage( ch, (CHAR_DATA *) vo, dice(2, 8) + level / 2, sn,DAM_HARM,TRUE);
    return;
}

void spell_chain_lightning(int sn,int level,CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    CHAR_DATA *tmp_vict,*last_vict,*next_vict;
    wchar_t buf[MAX_STRING_LENGTH];
    bool found;
    int dam;

    /* first strike */

    act( L"Bir yıldırım $s elinden $E atlıyor.",
	ch,NULL,victim,TO_ROOM);
    act( L"Bir yıldırım elinden $E atlıyor.",
	ch,NULL,victim,TO_CHAR);
    act( L"Bir yıldırım $s elinden sana atlıyor!",
	ch,NULL,victim,TO_VICT);

    dam = dice(level,6);

    if (CAN_DETECT(victim, ADET_GROUNDING))
    {
      send_to_char( L"Elektrik düşmanlarında sönüyor.\n\r",victim);
    	act( L"Bir yıldırım $S düşmanlarında sönüyor.\n\r",
		ch, NULL, victim, TO_ROOM);
    }
    else
    {
      if (saves_spell(level,victim,DAM_LIGHTNING))
	dam /= 3;
      damage(ch,victim,dam,sn,DAM_LIGHTNING,TRUE);
    }

    if (!IS_NPC(ch) && victim != ch &&
        ch->fighting != victim && victim->fighting != ch &&
        (IS_SET(victim->affected_by,AFF_CHARM) || !IS_NPC(victim)))
      {
        if (!can_see(victim, ch))
          do_yell(victim, (wchar_t*)"İmdat! Biri bana saldırıyor!");
        else
          {
            swprintf( buf, MAX_STRING_LENGTH-1, L"Geber %s, seni büyücü köpek!",
                    (is_affected(ch,gsn_doppelganger)&&!IS_IMMORTAL(victim))?
                    ch->doppel->name : ch->name);
            do_yell(victim,buf);
          }
      }


    last_vict = victim;
    level -= 4;   /* decrement damage */

    /* new targets */
    while (level > 0)
    {
	found = FALSE;
	for (tmp_vict = ch->in_room->people;
	     tmp_vict != NULL;
	     tmp_vict = next_vict)
	{
	  next_vict = tmp_vict->next_in_room;
	  if (!is_safe_spell(ch,tmp_vict,TRUE) && tmp_vict != last_vict)
	  {
	    found = TRUE;
	    last_vict = tmp_vict;
	    if (is_safe(ch, tmp_vict) )  {
        act( L"Yıldırım $s vücudunun etrafından geçiyor.", ch, NULL, NULL, TO_ROOM );
	      act( L"Yıldırım vücudunun etrafından dolanıyor.", ch, NULL, NULL, TO_CHAR );
	    }
	    else  {
        act( L"Yıldırım $e atlıyor!",tmp_vict,NULL,NULL,TO_ROOM);
	      act( L"Yıldırım seni vuruyor!",tmp_vict,NULL,NULL,TO_CHAR);
	      dam = dice(level,6);

            if (!IS_NPC(ch) && tmp_vict != ch &&
                ch->fighting != tmp_vict && tmp_vict->fighting != ch &&
                (IS_SET(tmp_vict->affected_by,AFF_CHARM) || !IS_NPC(tmp_vict)))
            {
               if (!can_see(tmp_vict, ch))
                    do_yell(tmp_vict, (wchar_t*)"İmdat! Biri bana saldırıyor!");
               else
               {
                 swprintf( buf, MAX_STRING_LENGTH-1, L"Geber %s, seni büyücü köpek!",
		(is_affected(ch,gsn_doppelganger)&&!IS_IMMORTAL(tmp_vict))?
                     ch->doppel->name : ch->name);
                 do_yell(tmp_vict,buf);
               }
             }

	     if (CAN_DETECT(tmp_vict, ADET_GROUNDING))
	     {
         send_to_char( L"Elektrik düşmanlarında sönüyor.\n\r",tmp_vict);
     		act( L"Bir yıldırım $S düşmanlarında sönüyor.\n\r",
			ch, NULL, tmp_vict, TO_ROOM);
	     }
	     else
	     {
	      if (saves_spell(level,tmp_vict,DAM_LIGHTNING))
		dam /= 3;
	      damage(ch,tmp_vict,dam,sn,DAM_LIGHTNING,TRUE);
             }
	     level -= 4;  /* decrement damage */
	    }
	  }
	}   /* end target searching loop */

	if (!found) /* no target found, hit the caster */
	{
	  if (ch == NULL )
	    return;

	  if (last_vict == ch) /* no double hits */
	  {
      act( L"Yıldırım sönmüş görünüyor.",ch,NULL,NULL,TO_ROOM);
	    act( L"Yıldırım vücudundan toprağa akıyor.",
		ch,NULL,NULL,TO_CHAR);
	    return;
	  }

	  last_vict = ch;
    act( L"Yıldırım $e sıçrıyor...ahaa!",ch,NULL,NULL,TO_ROOM);
	  send_to_char( L"Kendi yıldırımınla çarpıldın!\n\r",ch);
	  dam = dice(level,6);

	  if (CAN_DETECT(ch, ADET_GROUNDING))
	  {
      send_to_char( L"Elektrik düşmanlarında sönüyor.\n\r",ch);
  		act( L"Bir yıldırım $S düşmanlarında sönüyor.\n\r",
			ch, NULL, ch, TO_ROOM);
	  }
	  else
	  {
	     if (saves_spell(level,ch,DAM_LIGHTNING))
	   	dam /= 3;
	     damage(ch,ch,dam,sn,DAM_LIGHTNING,TRUE);
	  }
	  level -= 4;  /* decrement damage */
	  if (ch == NULL)
	    return;
	}
    /* now go back and find more targets */
    }
}


void spell_healing_light(int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    AFFECT_DATA af,af2;

    if ( is_affected_room( ch->in_room, sn ))
    {
      send_to_char( L"Işık bu odayı zaten iyileştirmiş.\n\r",ch);
	return;
    }

    af.where     = TO_ROOM_CONST;
    af.type      = sn;
    af.level     = level;
    af.duration  = level / 25;
    af.location  = APPLY_ROOM_HEAL;
    af.modifier  = level;
    af.bitvector = 0;
    affect_to_room( ch->in_room, &af );

    af2.where     = TO_AFFECTS;
    af2.type      = sn;
    af2.level	 = level;
    af2.duration  = level / 10;
    af2.modifier  = 0;
    af2.location  = APPLY_NONE;
    af2.bitvector = 0;
    affect_to_char( ch, &af2 );
    send_to_char( L"Oda şifalı ışıkla dolmaya başlıyor.\n\r", ch );
    act( L"Oda $s şifalı ışığıyla dolmaya başlıyor.",ch,NULL,NULL,TO_ROOM);
    return;
}



void spell_charm_person( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    wchar_t buf[MAX_INPUT_LENGTH];
    AFFECT_DATA af;

    if (is_safe(ch,victim)) return;

    if (count_charmed(ch)) return;

    if ( victim == ch )
    {
      send_to_char( L"Artık kendini daha çok seviyorsun!\n\r", ch );
	return;
    }

    if ( IS_AFFECTED(victim, AFF_CHARM)
    ||   IS_AFFECTED(ch, AFF_CHARM)
    || ( ch->sex != SEX_FEMALE &&  level < victim->level )
    || ( ch->sex == SEX_FEMALE &&  level < ( victim->level - 2 ) )
    ||   IS_SET(victim->imm_flags,IMM_CHARM)
    ||   saves_spell( level, victim,DAM_CHARM)
    ||   (IS_NPC(victim) && victim->pIndexData->pShop != NULL) )
	return;

    if ( victim->master )
	stop_follower( victim );
    add_follower( victim, ch );
    victim->leader = ch;
    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level	 = level;
    af.duration  = number_fuzzy( level / 5 );
    af.location  = 0;
    af.modifier  = 0;
    af.bitvector = AFF_CHARM;
    affect_to_char( victim, &af );
    act( L"$n sence de şirin değil mi?", ch, NULL, victim, TO_VICT );
    if ( ch != victim )
	act( L"$N büyülenmiş gözlerle sana bakıyor.",ch,NULL,victim,TO_CHAR);

    if (IS_NPC(victim) 	&& !IS_NPC(ch) )
    {
     if (number_percent() < (4 + (victim->level - ch->level)) * 10 )
     	add_mind(victim,ch->name);
     else if (victim->in_mind == NULL)
	{
	 swprintf( buf, MAX_STRING_LENGTH-1, L"%d",victim->in_room->vnum);
	 victim->in_mind = str_dup( buf );
	}
    }

    return;
}



void spell_chill_touch( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
    int dam;

    dam = number_range(1,level);
    if ( !saves_spell( level, victim,DAM_COLD ) )
    {
      act( L"$n morarıyor ve titriyor.",victim,NULL,NULL,TO_ROOM);
	af.where     = TO_AFFECTS;
	af.type      = sn;
	af.level     = level;
	af.duration  = 6;
	af.location  = APPLY_STR;
	af.modifier  = -1;
	af.bitvector = 0;
	affect_join( victim, &af );
    }
    else
    {
	dam /= 2;
    }

    damage( ch, victim, dam, sn, DAM_COLD,TRUE );
    return;
}



void spell_colour_spray( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = dice(level,3) + 13;
    if ( saves_spell( level, victim,DAM_LIGHT) )
	dam /= 2;
    else
	spell_blindness(skill_lookup( L"blindness"),
	    level/2,ch,(void *) victim,TARGET_CHAR);

    damage( ch, victim, dam, sn, DAM_LIGHT,TRUE );
    return;
}



void spell_continual_light(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    OBJ_DATA *light;

    if (target_name[0] != '\0')  /* do a glow on some object */
    {
	light = get_obj_carry(ch,target_name);

	if (light == NULL)
	{
	    send_to_char( L"Öyle bir şey görmüyorsun.\n\r",ch);
	    return;
	}

	if (IS_OBJ_STAT(light,ITEM_GLOW))
	{
    act( L"$p zaten parlıyor.",ch,light,NULL,TO_CHAR);
	    return;
	}

	SET_BIT(light->extra_flags,ITEM_GLOW);
  act( L"$p beyaz bir ışıkla parlıyor.",ch,light,NULL,TO_ALL);
	return;
    }

    light = create_object( get_obj_index( OBJ_VNUM_LIGHT_BALL ), 0 );
    obj_to_room( light, ch->in_room );
    act( L"$n parmaklarını oynatıyor ve $p ortaya çıkıyor.",   ch, light, NULL, TO_ROOM );
    act( L"Parmaklarını oynatıyorsun ve $p ortaya çıkıyor.", ch, light, NULL, TO_CHAR );
    return;
}



void spell_control_weather(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    if ( !wcscasecmp( target_name, L"iyi" ) )
	weather_info.change += dice( level / 3, 4 );
    else if ( !wcscasecmp( target_name, L"kötü" ) )
	weather_info.change -= dice( level / 3, 4 );
    else  {
      send_to_char ( L"İYİ mi olsun, KÖTÜ mü?\n\r", ch );
	return;
    }

    send_to_char( L"Tamam.\n\r", ch );
    return;
}



void spell_create_food( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    OBJ_DATA *mushroom;

    mushroom = create_object( get_obj_index( OBJ_VNUM_MUSHROOM ), 0 );
    mushroom->value[0] = level / 2;
    mushroom->value[1] = level;
    obj_to_room( mushroom, ch->in_room );
    act( L"$p birden ortaya çıkıyor.", ch, mushroom, NULL, TO_ROOM );
    act( L"$p birden ortaya çıkıyor.", ch, mushroom, NULL, TO_CHAR );
    return;
}

void spell_create_rose( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    OBJ_DATA *rose;
    rose = create_object(get_obj_index(OBJ_VNUM_ROSE), 0);
    act( L"$n güzel bir gül yaratıyor.",ch,rose,NULL,TO_ROOM);
    send_to_char( L"Güzel bir gül yaratıyorsun.\n\r",ch);
    obj_to_char(rose,ch);
    return;
}

void spell_create_spring(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    OBJ_DATA *spring;

    spring = create_object( get_obj_index( OBJ_VNUM_SPRING ), 0 );
    spring->timer = level;
    obj_to_room( spring, ch->in_room );
    act( L"$p yerden fışkırıyor.", ch, spring, NULL, TO_ROOM );
    act( L"$p yerden fışkırıyor.", ch, spring, NULL, TO_CHAR );
    return;
}



void spell_create_water( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    int water;

    if ( obj->item_type != ITEM_DRINK_CON )
    {
      send_to_char( L"Suyu tutamazsın.\n\r", ch );
	return;
    }

    if ( obj->value[2] != LIQ_WATER && obj->value[1] != 0 )
    {
      send_to_char( L"O başka bir sıvı ihtiva ediyor.\n\r", ch );
	return;
    }

    water = UMIN(
		level * (weather_info.sky >= SKY_RAINING ? 4 : 2),
		obj->value[0] - obj->value[1]
		);

    if ( water > 0 )
    {
	obj->value[2] = LIQ_WATER;
	obj->value[1] += water;
	if ( !is_name( (wchar_t*)"water", obj->name ) || !is_name( (wchar_t*)"su", obj->name ) )
	{
	    wchar_t buf[MAX_STRING_LENGTH];

	    swprintf( buf, MAX_STRING_LENGTH-1, L"%s water", obj->name );
	    free_string( obj->name );
	    obj->name = str_dup( buf );
	}
  act( L"$p doldu.", ch, obj, NULL, TO_CHAR );
    }

    return;
}



void spell_cure_blindness(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;

    if ( !is_affected( victim, gsn_blindness ) )
    {
	if (victim == ch)
  send_to_char( L"Kör değilsin.\n\r",ch);
else
  act( L"$N kör görünmüyor.",ch,NULL,victim,TO_CHAR);
	return;
    }

    if (check_dispel(level,victim,gsn_blindness))
    {
      send_to_char( L"Yeniden görmeye başlıyorsun!\n\r", victim );
    	act( L"$n artık kör değil.",victim,NULL,NULL,TO_ROOM);
    }
    else
    send_to_char( L"Büyü işe yaramadı.\n\r",ch);
}



void spell_cure_critical( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int heal;

    heal = dice(3, 8) + level / 2 ;
    victim->hit = UMIN( victim->hit + heal, victim->max_hit );
    update_pos( victim );
    send_to_char( L"Kendini iyi hissediyorsun!\n\r", victim );
    if ( ch != victim )
	send_to_char( L"Tamam.\n\r", ch );
    return;
}

/* RT added to cure plague */
void spell_cure_disease( int sn, int level, CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;

    if ( !is_affected( victim, gsn_plague ) )
    {
	if (victim == ch)
  send_to_char( L"Hasta değilsin.\n\r",ch);
else
  act( L"$N hasta görünmüyor.",ch,NULL,victim,TO_CHAR);
	return;
    }

    if (check_dispel(level,victim,gsn_plague))
    {
      send_to_char( L"Yaraların yokoluyor.\n\r",victim);
    	act( L"Yaraları yokolan $n rahatlamış görünüyor.",victim,NULL,NULL,TO_ROOM);
    }
    else
    send_to_char( L"Büyü işe yaramadı.\n\r",ch);
}



void spell_cure_light( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int heal;

    heal = dice(1, 8) + level / 4 + 5;
    victim->hit = UMIN( victim->hit + heal, victim->max_hit );
    update_pos( victim );
    send_to_char( L"Kendini iyi hissediyorsun!\n\r", victim );
    if ( ch != victim )
	send_to_char( L"Tamam.\n\r", ch );
    return;
}



void spell_cure_poison( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;

    if ( !is_affected( victim, gsn_poison ) )
    {
	if (victim == ch)
  send_to_char( L"Zehirlenmedin.\n\r",ch);
else
  act( L"$N zehirlenmiş görünmüyor.",ch,NULL,victim,TO_CHAR);
	return;
    }

    if (check_dispel(level,victim,gsn_poison))
    {
      send_to_char( L"Vücudundan bir sıcaklık geçiyor.\n\r",victim);
    	act( L"$n daha iyi görünüyor.",victim,NULL,NULL,TO_ROOM);
    }
    else
	send_to_char( L"Büyü işe yaramadı.\n\r",ch);
}

void spell_cure_serious( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int heal;

    heal = dice(2, 8) + level / 3 + 10 ;
    victim->hit = UMIN( victim->hit + heal, victim->max_hit );
    update_pos( victim );
    send_to_char( L"Kendini iyi hissediyorsun!\n\r", victim );
    if ( ch != victim )
	send_to_char( L"Tamam.\n\r", ch );
    return;
}



void spell_curse( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    AFFECT_DATA af;

    /* deal with the object case first */
    if (target == TARGET_OBJ)
    {
	obj = (OBJ_DATA *) vo;

	if (obj->wear_loc != WEAR_NONE)
	{
    act( L"Önce $p'yi çıkarmalısın.",ch,obj,NULL,TO_CHAR);
	    return;
	}

	if (IS_OBJ_STAT(obj,ITEM_EVIL))
	{
    act( L"$p zaten kemle dolmuş.",ch,obj,NULL,TO_CHAR);
	    return;
	}

	if (IS_OBJ_STAT(obj,ITEM_BLESS))
	{
	    AFFECT_DATA *paf;

	    paf = affect_find(obj->affected,skill_lookup( L"bless"));
	    if (!saves_dispel(level,paf != NULL ? paf->level : obj->level,0))
	    {
		if (paf != NULL)
		    affect_remove_obj(obj,paf);
        act( L"$p kızıl bir ayla ile parlıyor.",ch,obj,NULL,TO_ALL);
		REMOVE_BIT(obj->extra_flags,ITEM_BLESS);
		return;
	    }
	    else
	    {
        act( L"$p çevresindeki kutsal ayla çok güçlü görünüyor.",
		    ch,obj,NULL,TO_CHAR);
		return;
	    }
	}

	af.where        = TO_OBJECT;
	af.type         = sn;
	af.level        = level;
	af.duration     = (8 + level / 5);
	af.location     = APPLY_SAVES;
	af.modifier     = +1;
	af.bitvector    = ITEM_EVIL;
	affect_to_obj(obj,&af);

  act( L"$p şer yanlısı bir ayla ile parlıyor.",ch,obj,NULL,TO_ALL);
	return;
    }

    /* character curses */
    victim = (CHAR_DATA *) vo;

    if (IS_AFFECTED(victim,AFF_CURSE) || saves_spell(level,victim,DAM_NEGATIVE))
	return;
    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = (8 + level / 10);
    af.location  = APPLY_HITROLL;
    af.modifier  = -1 * (level / 8);
    af.bitvector = AFF_CURSE;
    affect_to_char( victim, &af );

    af.location  = APPLY_SAVING_SPELL;
    af.modifier  = level / 8;
    affect_to_char( victim, &af );

    send_to_char( L"Kirlendiğini hissediyorsun.\n\r", victim );
    if ( ch != victim )
	act( L"$N rahatsız görünüyor.",ch,NULL,victim,TO_CHAR);
    return;
}


/* RT replacement demonfire spell */

void spell_demonfire(int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    if ( !IS_NPC(ch) && !IS_EVIL(ch) )
    {
	victim = ch;
  send_to_char( L"İblisler sana dönüyor!\n\r",ch);
    }

    if (victim != ch)
    {
      act( L"$n cehennem iblislerini $S üzerine salıyor!",
    	    ch,NULL,victim,TO_ROOM);
    	act( L"$n cehennem iblislerini senin üzerine salıyor!",
    	    ch,NULL,victim,TO_VICT);
    	send_to_char( L"Cehennem iblislerine sesleniyorsun!\n\r",ch);
    }
    dam = dice( level, 10 );
    if ( saves_spell( level, victim,DAM_NEGATIVE) )
	dam /= 2;
    damage( ch, victim, dam, sn, DAM_NEGATIVE ,TRUE);
    spell_curse(gsn_curse, 3 * level / 4, ch, (void *) victim,TARGET_CHAR);
}

/* added by chronos */
void spell_bluefire(int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    if ( !IS_NPC(ch) && !IS_NEUTRAL(ch) )
    {
	victim = ch;
  send_to_char( L"Mavi alevin sana dönüyor!\n\r",ch);
    }

    if (victim != ch)
    {
      act( L"$n dünyanın mavi alevini $S üzerine salıyor!",
    	    ch,NULL,victim,TO_ROOM);
    	act( L"$n dünyanın yansızlarını üzerine salıyor!",
    	    ch,NULL,victim,TO_VICT);
    	send_to_char( L"Dünyanın yansızlarına sesleniyorsun!\n\r",ch);
    }

    dam = dice( level, 10 );
    if ( saves_spell( level, victim,DAM_FIRE) )
	dam /= 2;
    damage( ch, victim, dam, sn, DAM_FIRE ,TRUE);
}


void spell_detect_evil( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( CAN_DETECT(victim, DETECT_EVIL) )
    {
	if (victim == ch)
  send_to_char( L"Zaten kem'i saptıyorsun.\n\r",ch);
else
  act( L"$N zaten kem'i saptıyor.",ch,NULL,victim,TO_CHAR);
	return;
    }
    af.where     = TO_DETECTS;
    af.type      = sn;
    af.level	 = level;
    af.duration  = (5 + level / 3);
    af.modifier  = 0;
    af.location  = APPLY_NONE;
    af.bitvector = DETECT_EVIL;
    affect_to_char( victim, &af );
    send_to_char( L"Gözlerin yanıyor.\n\r", victim );
    if ( ch != victim )
	send_to_char( L"Tamam.\n\r", ch );
    return;
}


void spell_detect_good( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( CAN_DETECT(victim, DETECT_GOOD) )
    {
	if (victim == ch)
  send_to_char( L"Zaten iyiyi saptıyorsun.\n\r",ch);
else
  act( L"$N zaten iyiyi saptıyor.",ch,NULL,victim,TO_CHAR);
	return;
    }
    af.where     = TO_DETECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = (5 + level / 3);
    af.modifier  = 0;
    af.location  = APPLY_NONE;
    af.bitvector = DETECT_GOOD;
    affect_to_char( victim, &af );
    send_to_char( L"Gözlerin yanıyor.\n\r", victim );
    if ( ch != victim )
	send_to_char( L"Tamam.\n\r", ch );
    return;
}



void spell_detect_hidden(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( CAN_DETECT(victim, DETECT_HIDDEN) )
    {
	if (victim == ch)
  send_to_char( L"Olabildiğince tetiktesin. \n\r",ch);
else
  act( L"$N zaten saklı yaşam formlarını saptayabiliyor.",ch,NULL,victim,TO_CHAR);
	return;
    }
    af.where     = TO_DETECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = (5 + level / 3);
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = DETECT_HIDDEN;
    affect_to_char( victim, &af );
    send_to_char( L"Dikkatin artıyor.\n\r", victim );
    if ( ch != victim )
	send_to_char( L"Tamam.\n\r", ch );
    return;
}



void spell_detect_invis( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( CAN_DETECT(victim, DETECT_INVIS) )
    {
	if (victim == ch)
  send_to_char( L"Zaten görünmezi görüyorsun.\n\r",ch);
else
  act( L"$N zaten görünmezi görüyor.",ch,NULL,victim,TO_CHAR);
	return;
    }

    af.where     = TO_DETECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = (5 + level / 3);
    af.modifier  = 0;
    af.location  = APPLY_NONE;
    af.bitvector = DETECT_INVIS;
    affect_to_char( victim, &af );
    send_to_char( L"Gözlerin yanıyor.\n\r", victim );
    if ( ch != victim )
	send_to_char( L"Tamam.\n\r", ch );
    return;
}



void spell_detect_magic( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( CAN_DETECT(victim, DETECT_MAGIC) )
    {
	if (victim == ch)
  send_to_char( L"Zaten büyülü auraları saptayaibliyorsun.\n\r",ch);
else
  act( L"$N zaten büyülüleri saptayabiliyor.",ch,NULL,victim,TO_CHAR);
	return;
    }

    af.where     = TO_DETECTS;
    af.type      = sn;
    af.level	 = level;
    af.duration  = (5 + level / 3);
    af.modifier  = 0;
    af.location  = APPLY_NONE;
    af.bitvector = DETECT_MAGIC;
    affect_to_char( victim, &af );
    send_to_char( L"Gözlerin yanıyor.\n\r", victim );
    if ( ch != victim )
	send_to_char( L"Tamam.\n\r", ch );
    return;
}



void spell_detect_poison( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;

    if ( obj->item_type == ITEM_DRINK_CON || obj->item_type == ITEM_FOOD )
    {
	if ( obj->value[3] != 0 )
  send_to_char( L"Zehrin kokusunu alıyorsun.\n\r", ch );
else
  send_to_char( L"Lezzetli görünüyor.\n\r", ch );
    }
    else
    {
      send_to_char( L"Zehirlenmiş görünmüyor.\n\r", ch );
    }

    return;
}



void spell_dispel_evil( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    if ( !IS_NPC(ch) && IS_EVIL(ch) )
	victim = ch;

    if ( IS_GOOD(victim) )
    {
      act( L"Tanrılar $M koruyor.", ch, NULL, victim, TO_ROOM );
	return;
    }

    if ( IS_NEUTRAL(victim) )
    {
      act( L"$N etkilenmiş görünmüyor.", ch, NULL, victim, TO_CHAR );
	return;
    }

    if (victim->hit > (ch->level * 4))
      dam = dice( level, 4 );
    else
      dam = UMAX(victim->hit, dice(level,4));
    if ( saves_spell( level, victim,DAM_HOLY) )
	dam /= 2;
    damage( ch, victim, dam, sn, DAM_HOLY ,TRUE);
    return;
}


void spell_dispel_good( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    if ( !IS_NPC(ch) && IS_GOOD(ch) )
	victim = ch;

    if ( IS_EVIL(victim) )
    {
      act( L"$N kem ile korunuyor.", ch, NULL, victim, TO_ROOM );
	return;
    }

    if ( IS_NEUTRAL(victim) )
    {
      act( L"$N etkilenmiş görünmüyor.", ch, NULL, victim, TO_CHAR );
	return;
    }

    if (victim->hit > (ch->level * 4))
      dam = dice( level, 4 );
    else
      dam = UMAX(victim->hit, dice(level,4));
    if ( saves_spell( level, victim,DAM_NEGATIVE) )
	dam /= 2;
    damage( ch, victim, dam, sn, DAM_NEGATIVE ,TRUE);
    return;
}


/* modified for enhanced use */

void spell_dispel_magic( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    bool found = FALSE;

    if (saves_spell(level, victim,DAM_OTHER))
    {
      send_to_char( L"Kısa süre ürpertici güçle doluyorsun.\n\r",victim);
    	send_to_char( L"Başaramadın.\n\r",ch);
	return;
    }

    /* begin running through the spells */

    if (check_dispel(level,victim,skill_lookup( L"armor")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup( L"enhanced armor")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup( L"bless")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup( L"blindness")))
    {
	found = TRUE;
  act( L"$n artık kör değil.",victim,NULL,NULL,TO_ROOM);
    }

    if (check_dispel(level,victim,skill_lookup( L"calm")))
    {
	found = TRUE;
  act( L"$n eskisi kadar barışçıl görünmüyor...",victim,NULL,NULL,TO_ROOM);
    }

    if (check_dispel(level,victim,skill_lookup( L"change sex")))
    {
	found = TRUE;
  act( L"$n kendine gelmeye başlıyor.",victim,NULL,NULL,TO_ROOM);
    }

    if (check_dispel(level,victim,skill_lookup( L"charm person")))
    {
        found = TRUE;
        act( L"$n özgür iradesini kazanıyor.",victim,NULL,NULL,TO_ROOM);
    }

    if (check_dispel(level,victim,skill_lookup( L"chill touch")))
    {
        found = TRUE;
        act( L"$n ısınmış görünüyor.",victim,NULL,NULL,TO_ROOM);
    }

    if (check_dispel(level,victim,skill_lookup( L"curse")))
        found = TRUE;

    if (check_dispel(level,victim,skill_lookup( L"detect evil")))
        found = TRUE;

    if (check_dispel(level,victim,skill_lookup( L"detect good")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup( L"detect hidden")))
        found = TRUE;

    if (check_dispel(level,victim,skill_lookup( L"detect invis")))
        found = TRUE;

        found = TRUE;

    if (check_dispel(level,victim,skill_lookup( L"detect hidden")))
        found = TRUE;

    if (check_dispel(level,victim,skill_lookup( L"detect magic")))
        found = TRUE;

    if (check_dispel(level,victim,skill_lookup( L"faerie fire")))
    {
      act( L"$s aurası yokoluyor.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup( L"fly")))
    {
      act( L"$n yere düşüyor!",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup( L"frenzy")))
    {
      act( L"$n eskisi kadar vahşi görünmüyor.",victim,NULL,NULL,TO_ROOM);;
        found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup( L"giant strength")))
    {
      act( L"$n eskisi kadar güçlü görünmüyor.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup( L"haste")))
    {
      act( L"$n eskisi kadar çabuk hareket etmiyor.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup( L"infravision")))
        found = TRUE;

    if (check_dispel(level,victim,skill_lookup( L"invis")))
    {
      act( L"$n varlığa dönüyor.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup( L"mass invis")))
    {
      act( L"$n varlığa dönüyor.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup( L"pass door")))
        found = TRUE;


    if (check_dispel(level,victim,skill_lookup( L"protection evil")))
        found = TRUE;

    if (check_dispel(level,victim,skill_lookup( L"protection good")))
        found = TRUE;

    if (check_dispel(level,victim,skill_lookup( L"sanctuary")))
    {
      act( L"$s çevresindeki beyaz aura yokoluyor.",
            victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }

    if (IS_AFFECTED(victim,AFF_SANCTUARY)
	&& !saves_dispel(level, victim->level,-1)
	&& !is_affected(victim,skill_lookup( L"sanctuary"))
	&& !(victim->spec_fun == spec_special_guard
		|| victim->spec_fun == spec_stalker) )
    {
	REMOVE_BIT(victim->affected_by,AFF_SANCTUARY);
  act( L"$s çevresindeki beyaz aura yokoluyor.",
            victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup( L"shield")))
    {
      act( L"$m koruyan kalkan yokoluyor.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup( L"sleep")))
        found = TRUE;

    if (check_dispel(level,victim,skill_lookup( L"slow")))
    {
      act( L"$n eskisi gibi yavaş hareket etmiyor.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup( L"stone skin")))
    {
      act( L"$s derisi eski haline dönüyor.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup( L"weaken")))
    {
      act( L"$n güçlenmiş görünüyor.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup( L"shielding")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup( L"web")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup( L"fear")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup( L"protection heat")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup( L"protection cold")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup( L"magic resistance")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup( L"hallucination")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup( L"terangreal")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup( L"power word stun")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup( L"corruption")))
    {
      act( L"$n daha sağlıklı görünüyor.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup( L"web")))
    {
      act( L"$s çevresindeki ağlar eriyor.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }

    if (found)
        send_to_char( L"Tamam.\n\r",ch);
    else
        send_to_char( L"Büyü işe yaramadı.\n\r",ch);
	return;
}

void spell_earthquake( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    OBJ_DATA *obj, *obj_next, *corpse;

    send_to_char( L"Dünya ayaklarının altında titriyor!\n\r", ch );
    act( L"$n dünyayı titretiyor.", ch, NULL, NULL, TO_ROOM );

    for ( vch = char_list; vch != NULL; vch = vch_next )
    {
	vch_next	= vch->next;
	if ( vch->in_room == NULL )
	    continue;
	if ( vch->in_room == ch->in_room )
	{
	    if ( vch != ch && !is_safe_spell(ch,vch,TRUE) && !is_same_group(ch, vch))
	      if ( is_safe(ch, vch) )
		continue;
	    if ( ch == vch )
		continue;
		if (IS_AFFECTED(vch,AFF_FLYING))
		    damage(ch,vch,0,sn,DAM_BASH,TRUE);
		else
		    damage( ch,vch,level + dice(2, 8), sn, DAM_BASH,TRUE);
	    continue;
	}

	if ( vch->in_room->area == ch->in_room->area )
  send_to_char( L"Yeryüzü titriyor.\n\r", vch );
    }

    for (obj = ch->in_room->contents; obj != NULL; obj = obj_next)
    {
	obj_next = obj->next_content;
	if ( obj->pIndexData->vnum == OBJ_VNUM_GRAVE_STONE
		&& (corpse = obj->contains) != NULL
		&& number_percent() < get_skill(ch,sn) )
	{
		obj_from_obj( corpse );
		REMOVE_BIT( corpse->extra_flags, ITEM_BURIED );
		obj_to_room( corpse, ch->in_room );
		extract_obj( obj );
		corpse->timer   = number_range( 25, 40 );
    act( L"Deprem nedeniyle $p açığa çıkıyor.\n\r",ch, corpse, NULL, TO_ALL );
	}
    }

    return;
}

void spell_enchant_armor( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    AFFECT_DATA *paf;
    int result, fail;
    int ac_bonus, added;
    bool ac_found = FALSE;

    if (obj->item_type != ITEM_ARMOR)
    {
      send_to_char( L"O bir zırh değil.\n\r",ch);
	return;
    }

    if (obj->wear_loc != -1)
    {
      send_to_char( L"Yükseltilecek zırh envanterinde taşınmalı.\n\r",ch);
	return;
    }

    /* this means they have no bonus */
    ac_bonus = 0;
    fail = 25;	/* base 25% chance of failure */

    /* find the bonuses */

    if (!obj->enchanted)
	for ( paf = obj->pIndexData->affected; paf != NULL; paf = paf->next )
	{
	    if ( paf->location == APPLY_AC )
	    {
	    	ac_bonus = paf->modifier;
		ac_found = TRUE;
	    	fail += 5 * (ac_bonus * ac_bonus);
 	    }

	    else  /* things get a little harder */
	    	fail += 20;
    	}

    for ( paf = obj->affected; paf != NULL; paf = paf->next )
    {
	if ( paf->location == APPLY_AC )
  	{
	    ac_bonus = paf->modifier;
	    ac_found = TRUE;
	    fail += 5 * (ac_bonus * ac_bonus);
	}

	else /* things get a little harder */
	    fail += 20;
    }

    /* apply other modifiers */
    fail -= level;

    if (IS_OBJ_STAT(obj,ITEM_BLESS))
	fail -= 15;
    if (IS_OBJ_STAT(obj,ITEM_GLOW))
	fail -= 5;

    fail = URANGE(5,fail,85);

    result = number_percent();

    /* the moment of truth */
    if (result < (fail / 5))  /* item destroyed */
    {
      act( L"$p kör edici bir ışıkla alev alarak buharlaşıyor!",ch,obj,NULL,TO_CHAR);
    	act( L"$p kör edici bir ışıkla alev alarak buharlaşıyor!",ch,obj,NULL,TO_ROOM);
	extract_obj(obj);
	return;
    }

    if (result < (fail / 3)) /* item disenchanted */
    {
	AFFECT_DATA *paf_next;

  act( L"$p parlak bir ışık vererek yokoluyor.",ch,obj,NULL,TO_CHAR);
	act( L"$p parlak bir ışık vererek yokoluyor.",ch,obj,NULL,TO_ROOM);
	obj->enchanted = TRUE;

	/* remove all affects */
	for (paf = obj->affected; paf != NULL; paf = paf_next)
	{
	    paf_next = paf->next;
	    free_affect(paf);
	}
	obj->affected = NULL;

	/* clear all flags */
	obj->extra_flags = 0;
	return;
    }

    if ( result <= fail )  /* failed, no bad result */
    {
      send_to_char( L"Değişen bir şey yok.\n\r",ch);
	return;
    }

    /* okay, move all the old flags into new vectors if we have to */
    if (!obj->enchanted)
    {
	AFFECT_DATA *af_new;
	obj->enchanted = TRUE;

	for (paf = obj->pIndexData->affected; paf != NULL; paf = paf->next)
	{
	    af_new = new_affect();

	    af_new->next = obj->affected;
	    obj->affected = af_new;

	    af_new->where	= paf->where;
	    af_new->type 	= UMAX(0,paf->type);
	    af_new->level	= paf->level;
	    af_new->duration	= paf->duration;
	    af_new->location	= paf->location;
	    af_new->modifier	= paf->modifier;
	    af_new->bitvector	= paf->bitvector;
	}
    }

    if (result <= (90 - level/5))  /* success! */
    {
      act( L"$p altın renginde parlıyor.",ch,obj,NULL,TO_CHAR);
    	act( L"$p altın renginde parlıyor.",ch,obj,NULL,TO_ROOM);
	SET_BIT(obj->extra_flags, ITEM_MAGIC);
	added = -1;
    }

    else  /* exceptional enchant */
    {
      act( L"$p parlak altın renginde parlıyor!",ch,obj,NULL,TO_CHAR);
    	act( L"$p parlak altın renginde parlıyor!",ch,obj,NULL,TO_ROOM);
	SET_BIT(obj->extra_flags,ITEM_MAGIC);
	SET_BIT(obj->extra_flags,ITEM_GLOW);
	added = -2;
    }

    /* now add the enchantments */

    if (obj->level < LEVEL_HERO)
	obj->level = UMIN(LEVEL_HERO - 1,obj->level + 1);

    if (ac_found)
    {
	for ( paf = obj->affected; paf != NULL; paf = paf->next)
	{
	    if ( paf->location == APPLY_AC)
	    {
		paf->type = sn;
		paf->modifier += added;
		paf->level = UMAX(paf->level,level);
	    }
	}
    }
    else /* add a new affect */
    {
 	paf = new_affect();

	paf->where	= TO_OBJECT;
	paf->type	= sn;
	paf->level	= level;
	paf->duration	= -1;
	paf->location	= APPLY_AC;
	paf->modifier	=  added;
	paf->bitvector  = 0;
    	paf->next	= obj->affected;
    	obj->affected	= paf;
    }

}




void spell_enchant_weapon(int sn,int level,CHAR_DATA *ch, void *vo,int target)
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    AFFECT_DATA *paf;
    int result, fail;
    int hit_bonus, dam_bonus, added;
    bool hit_found = FALSE, dam_found = FALSE;

    if (obj->item_type != ITEM_WEAPON)
    {
      send_to_char( L"O bir silah değil.\n\r",ch);
	return;
    }

    if (obj->wear_loc != -1)
    {
      send_to_char( L"Yükseltilecek silah envanterinde taşınmalı.\n\r",ch);
	return;
    }

    /* this means they have no bonus */
    hit_bonus = 0;
    dam_bonus = 0;
    fail = 25;	/* base 25% chance of failure */

    /* find the bonuses */

    if (!obj->enchanted)
    	for ( paf = obj->pIndexData->affected; paf != NULL; paf = paf->next )
    	{
            if ( paf->location == APPLY_HITROLL )
            {
	    	hit_bonus = paf->modifier;
		hit_found = TRUE;
	    	fail += 2 * (hit_bonus * hit_bonus);
 	    }

	    else if (paf->location == APPLY_DAMROLL )
	    {
	    	dam_bonus = paf->modifier;
		dam_found = TRUE;
	    	fail += 2 * (dam_bonus * dam_bonus);
	    }

	    else  /* things get a little harder */
	    	fail += 25;
    	}

    for ( paf = obj->affected; paf != NULL; paf = paf->next )
    {
	if ( paf->location == APPLY_HITROLL )
  	{
	    hit_bonus = paf->modifier;
	    hit_found = TRUE;
	    fail += 2 * (hit_bonus * hit_bonus);
	}

	else if (paf->location == APPLY_DAMROLL )
  	{
	    dam_bonus = paf->modifier;
	    dam_found = TRUE;
	    fail += 2 * (dam_bonus * dam_bonus);
	}

	else /* things get a little harder */
	    fail += 25;
    }

    /* apply other modifiers */
    fail -= 3 * level/2;

    if (IS_OBJ_STAT(obj,ITEM_BLESS))
	fail -= 15;
    if (IS_OBJ_STAT(obj,ITEM_GLOW))
	fail -= 5;

    fail = URANGE(5,fail,95);

    result = number_percent();

    /* the moment of truth */
    if (result < (fail / 5))  /* item destroyed */
    {
      act( L"$p şiddetle titreyerek patlıyor!",ch,obj,NULL,TO_CHAR);
    	act( L"$p şiddetle titreyerek patlıyor!",ch,obj,NULL,TO_ROOM);
	extract_obj(obj);
	return;
    }

    if (result < (fail / 2)) /* item disenchanted */
    {
	AFFECT_DATA *paf_next;

  act( L"$p parlak bir ışık vererek yokoluyor.",ch,obj,NULL,TO_CHAR);
	act( L"$p parlak bir ışık vererek yokoluyor.",ch,obj,NULL,TO_ROOM);
	obj->enchanted = TRUE;

	/* remove all affects */
	for (paf = obj->affected; paf != NULL; paf = paf_next)
	{
	    paf_next = paf->next;
	    free_affect(paf);
	}
	obj->affected = NULL;

	/* clear all flags */
	obj->extra_flags = 0;
	return;
    }

    if ( result <= fail )  /* failed, no bad result */
    {
      send_to_char( L"Değişen bir şey yok.\n\r",ch);
	return;
    }

    /* okay, move all the old flags into new vectors if we have to */
    if (!obj->enchanted)
    {
	AFFECT_DATA *af_new;
	obj->enchanted = TRUE;

	for (paf = obj->pIndexData->affected; paf != NULL; paf = paf->next)
	{
	    af_new = new_affect();

	    af_new->next = obj->affected;
	    obj->affected = af_new;

	    af_new->where	= paf->where;
	    af_new->type 	= UMAX(0,paf->type);
	    af_new->level	= paf->level;
	    af_new->duration	= paf->duration;
	    af_new->location	= paf->location;
	    af_new->modifier	= paf->modifier;
	    af_new->bitvector	= paf->bitvector;
	}
    }

    if (result <= (100 - level/5))  /* success! */
    {
      act( L"$p mavi renkte parlıyor.",ch,obj,NULL,TO_CHAR);
    	act( L"$p mavi renkte parlıyor.",ch,obj,NULL,TO_ROOM);
	SET_BIT(obj->extra_flags, ITEM_MAGIC);
	added = 1;
    }

    else  /* exceptional enchant */
    {
      act( L"$p parlak mavi renkte parlıyor!",ch,obj,NULL,TO_CHAR);
    	act( L"$p parlak mavi renkte parlıyor!",ch,obj,NULL,TO_ROOM);
	SET_BIT(obj->extra_flags,ITEM_MAGIC);
	SET_BIT(obj->extra_flags,ITEM_GLOW);
	added = 2;
    }

    /* now add the enchantments */

    if (obj->level < LEVEL_HERO - 1)
	obj->level = UMIN(LEVEL_HERO - 1,obj->level + 1);

    if (dam_found)
    {
	for ( paf = obj->affected; paf != NULL; paf = paf->next)
	{
	    if ( paf->location == APPLY_DAMROLL)
	    {
		paf->type = sn;
		paf->modifier += added;
		paf->level = UMAX(paf->level,level);
		if (paf->modifier > 4)
		    SET_BIT(obj->extra_flags,ITEM_HUM);
	    }
	}
    }
    else /* add a new affect */
    {
	paf = new_affect();

	paf->where	= TO_OBJECT;
	paf->type	= sn;
	paf->level	= level;
	paf->duration	= -1;
	paf->location	= APPLY_DAMROLL;
	paf->modifier	=  added;
	paf->bitvector  = 0;
    	paf->next	= obj->affected;
    	obj->affected	= paf;
    }

    if (hit_found)
    {
        for ( paf = obj->affected; paf != NULL; paf = paf->next)
	{
            if ( paf->location == APPLY_HITROLL)
            {
		paf->type = sn;
                paf->modifier += added;
                paf->level = UMAX(paf->level,level);
                if (paf->modifier > 4)
                    SET_BIT(obj->extra_flags,ITEM_HUM);
            }
	}
    }
    else /* add a new affect */
    {
        paf = new_affect();

        paf->type       = sn;
        paf->level      = level;
        paf->duration   = -1;
        paf->location   = APPLY_HITROLL;
        paf->modifier   =  added;
        paf->bitvector  = 0;
        paf->next       = obj->affected;
        obj->affected   = paf;
    }

}



/*
 * Drain XP, MANA, HP.
 * Caster gains HP.
 */
void spell_energy_drain( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    if ( saves_spell( level, victim,DAM_NEGATIVE) )
    {
      send_to_char( L"Kısa bir soğukluk hissediyorsun.\n\r",victim);
	return;
    }


    if ( victim->level <= 2 )
    {
	dam		 = ch->hit + 1;
    }
    else
    {
	gain_exp( victim, 0 - number_range( level/5, 3 * level / 5 ) );
	victim->mana	/= 2;
	victim->move	/= 2;
	dam		 = dice(1, level);
	ch->hit		+= dam;
    }

    send_to_char( L"Yaşamının aktığını hissediyorsun!\n\r",victim);
    send_to_char( L"Vaay....ne telaş ama!\n\r",ch);
    damage( ch, victim, dam, sn, DAM_NEGATIVE ,TRUE);

    return;
}

void spell_hellfire( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  int dam;

  dam = dice(level, 7);

  damage(ch,victim,dam,sn,DAM_FIRE, TRUE);

}

void spell_iceball( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *tmp_vict;
    CHAR_DATA *tmp_next;
    wchar_t buf[MAX_STRING_LENGTH];
    int dam;
    int movedam;

    dam = dice( level , 12);
    movedam     = number_range( ch->level, 2 * ch->level );

  for (tmp_vict = ch->in_room->people;tmp_vict != NULL;
       tmp_vict = tmp_next)
    {
      tmp_next = tmp_vict->next_in_room;

      if ( !is_safe_spell(ch,tmp_vict,TRUE))
        {
          if (!IS_NPC(ch) && tmp_vict != ch &&
              ch->fighting != tmp_vict && tmp_vict->fighting != ch &&
              (IS_SET(tmp_vict->affected_by,AFF_CHARM) || !IS_NPC(tmp_vict)))
            {
            if (!can_see(tmp_vict, ch))
                do_yell(tmp_vict, (wchar_t*)"İmdat! Biri bana saldırıyor!");
            else
              {
                swprintf( buf, MAX_STRING_LENGTH-1, L"Geber %s, seni büyücü köpek!",
	(is_affected(ch,gsn_doppelganger)&&!IS_IMMORTAL(tmp_vict))? ch->doppel->name : ch->name);
                 do_yell(tmp_vict,buf);
              }
          }

          if (saves_spell(level,tmp_vict, DAM_COLD))
            dam /= 2;
          damage( ch, tmp_vict, dam, sn, DAM_COLD, TRUE );
          tmp_vict->move -= UMIN(tmp_vict->move,movedam);

        }
    }
}

void spell_fireball( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *tmp_vict;
    CHAR_DATA *tmp_next;
    wchar_t buf[MAX_STRING_LENGTH];
    int dam;
    int movedam;

    dam = dice( level , 12);
    movedam     = number_range( ch->level, 2 * ch->level );

  for (tmp_vict = ch->in_room->people;tmp_vict != NULL;
       tmp_vict = tmp_next)
    {
      tmp_next = tmp_vict->next_in_room;

      if ( !is_safe_spell(ch,tmp_vict,TRUE))
        {
          if (!IS_NPC(ch) && tmp_vict != ch &&
              ch->fighting != tmp_vict && tmp_vict->fighting != ch &&
              (IS_SET(tmp_vict->affected_by,AFF_CHARM) || !IS_NPC(tmp_vict)))
            {
            if (!can_see(tmp_vict, ch))
                do_yell(tmp_vict, (wchar_t*)"İmdat! Biri bana saldırıyor!");
            else
              {
                swprintf( buf, MAX_STRING_LENGTH-1, L"Geber %s, seni büyücü köpek!",
	(is_affected(ch,gsn_doppelganger)&&!IS_IMMORTAL(tmp_vict))? ch->doppel->name : ch->name);
                 do_yell(tmp_vict,buf);
              }
          }

          if (saves_spell(level,tmp_vict, DAM_FIRE))
            dam /= 2;
          damage( ch, tmp_vict, dam, sn, DAM_FIRE, TRUE );
          tmp_vict->move -= UMIN(tmp_vict->move,movedam);

        }
    }
}


void spell_fireproof(int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    AFFECT_DATA af;

    if (IS_OBJ_STAT(obj,ITEM_BURN_PROOF))
    {
      act( L"$p ateşe karşı zaten dayanıklı.",ch,obj,NULL,TO_CHAR);
	return;
    }

    af.where     = TO_OBJECT;
    af.type      = sn;
    af.level     = level;
    af.duration  = number_fuzzy(level / 4);
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = ITEM_BURN_PROOF;

    affect_to_obj(obj,&af);

    act( L"$p'yi ateşe karşı koruyorsun.",ch,obj,NULL,TO_CHAR);
    act( L"$p koruyucu bir aurayla çevrelendi.",ch,obj,NULL,TO_ROOM);
}



void spell_flamestrike( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = dice(level, 10);
    if ( saves_spell( level, victim,DAM_FIRE) )
	dam /= 2;
    damage( ch, victim, dam, sn, DAM_FIRE ,TRUE);
    return;
}



void spell_faerie_fire( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_FAERIE_FIRE) )
	return;
    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level	 = level;
    af.duration  = 10 + level / 5;
    af.location  = APPLY_AC;
    af.modifier  = 2 * level;
    af.bitvector = AFF_FAERIE_FIRE;
    affect_to_char( victim, &af );
    send_to_char( L"Pembe bir ayla ile çevrelendin.\n\r", victim );
    act( L"$n pembe bir ayla ile çevrelendi.", victim, NULL, NULL, TO_ROOM );
    return;
}



void spell_faerie_fog( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *ich;

    act( L"$n mor bir duman oluşturuyor.", ch, NULL, NULL, TO_ROOM );
    send_to_char( L"Mor bir duman oluşturuyorsun.\n\r", ch );

    for ( ich = ch->in_room->people; ich != NULL; ich = ich->next_in_room )
    {
	if (ich->invis_level > 0)
	    continue;

	if ( ich == ch || saves_spell( level, ich,DAM_OTHER) )
	    continue;

	affect_strip ( ich, gsn_invis			);
	affect_strip ( ich, gsn_mass_invis		);
	affect_strip ( ich, gsn_imp_invis		);
	REMOVE_BIT   ( ich->affected_by, AFF_HIDE	);
	REMOVE_BIT   ( ich->affected_by, AFF_FADE	);
	REMOVE_BIT   ( ich->affected_by, AFF_INVISIBLE	);
	REMOVE_BIT   ( ich->affected_by, AFF_IMP_INVIS	);

        /* An cora sneaks eternally */
        if ( IS_NPC(ich) || !IS_SET(race_table[RACE(ich)].aff,AFF_SNEAK) )
          {
            affect_strip ( ich, gsn_sneak                       );
            REMOVE_BIT   ( ich->affected_by, AFF_SNEAK  );
          }

          act( L"$n ortaya çıktı!", ich, NULL, NULL, TO_ROOM );
        	send_to_char( L"Ortaya çıktın!\n\r", ich );
    }

    return;
}

void spell_floating_disc( int sn, int level,CHAR_DATA *ch,void *vo,int target )
{
    OBJ_DATA *disc, *floating;

    floating = get_eq_char(ch,WEAR_FLOAT);
    if (floating != NULL && IS_OBJ_STAT(floating,ITEM_NOREMOVE))
    {
      act( L"$p eşyasını çıkaramazsın.",ch,floating,NULL,TO_CHAR);
	return;
    }

    disc = create_object(get_obj_index(OBJ_VNUM_DISC), 0);
    disc->value[0]	= ch->level * 10; /* 10 pounds per level capacity */
    disc->value[3]	= ch->level * 5; /* 5 pounds per level max per item */
    disc->timer		= ch->level * 2 - number_range(0,level / 2);

    act( L"$n asılı siyah bir disk yarattı.",ch,NULL,NULL,TO_ROOM);
    send_to_char( L"Asılı bir disc yarattın.\n\r",ch);
    obj_to_char(disc,ch);
    wear_obj(ch,disc,TRUE);
    return;
}


void spell_fly( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_FLYING) )
    {
	if (victim == ch)
  send_to_char( L"Zaten uçuyorsun.\n\r",ch);
	else
  act( L"$S uçmak için yardımına ihtiyacı yok.",ch,NULL,victim,TO_CHAR);
	return;
    }
    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level	 = level;
    af.duration  = level + 3;
    af.location  = 0;
    af.modifier  = 0;
    af.bitvector = AFF_FLYING;
    affect_to_char( victim, &af );
    send_to_char( L"Ayakların yerden kesiliyor.\n\r", victim );
    act( L"$s ayakları yerden kesiliyor.", victim, NULL, NULL, TO_ROOM );
    return;
}

/* RT clerical berserking spell */

void spell_frenzy(int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if (is_affected(victim,sn) || IS_AFFECTED(victim,AFF_BERSERK))
    {
	if (victim == ch)
  send_to_char( L"Zaten cinnet getiriyorsun.\n\r",ch);
else
  act( L"$N zaten cinnet getiriyor.",ch,NULL,victim,TO_CHAR);
	return;
    }

    if (is_affected(victim,skill_lookup( L"calm")))
    {
	if (victim == ch)
  send_to_char( L"Neden bir an için sakinleşmiyorsun?\n\r",ch);
else
  act( L"$N dövüşmeyi arzulamaya devam ediyor.",
	      ch,NULL,victim,TO_CHAR);
	return;
    }

    if ((IS_GOOD(ch) && !IS_GOOD(victim)) ||
	(IS_NEUTRAL(ch) && !IS_NEUTRAL(victim)) ||
	(IS_EVIL(ch) && !IS_EVIL(victim))
       )
    {
      act( L"Tanrın $M seviyormuş gibi görünmüyor.",ch,NULL,victim,TO_CHAR);
	return;
    }

    af.where     = TO_AFFECTS;
    af.type 	 = sn;
    af.level	 = level;
    af.duration	 = level / 3;
    af.modifier  = level / 6;
    af.bitvector = 0;

    af.location  = APPLY_HITROLL;
    affect_to_char(victim,&af);

    af.location  = APPLY_DAMROLL;
    affect_to_char(victim,&af);

    af.modifier  = 10 * (level / 12);
    af.location  = APPLY_AC;
    affect_to_char(victim,&af);

    send_to_char( L"Kutsal gazapla doldun!\n\r",victim);
    act( L"$n gözlerine vahşi bir bakış alıyor!",victim,NULL,NULL,TO_ROOM);
}

void spell_gate( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim;
    bool gate_pet;


    if ( ( victim = get_char_world( ch, target_name ) ) == NULL
    ||   victim == ch
    ||   victim->in_room == NULL
    ||   !can_see_room(ch,victim->in_room)
    ||   IS_SET(victim->in_room->room_flags, ROOM_SAFE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_PRIVATE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_SOLITARY)
    ||   IS_SET(ch->in_room->room_flags, ROOM_NOSUMMON)
    ||   IS_SET(victim->in_room->room_flags, ROOM_NOSUMMON)
    ||   victim->level >= level + 3
    ||   saves_spell(level,victim,DAM_OTHER)
/*    ||   (!IS_NPC(victim) && victim->level >= LEVEL_HERO)  * NOT trust */
    ||   (IS_NPC(victim) && is_safe_nomessage(ch, victim) && IS_SET(victim->imm_flags,IMM_SUMMON))
    ||   (!IS_NPC(victim) && is_safe_nomessage(ch, victim) && IS_SET(victim->act,PLR_NOSUMMON))
    ||   (!IS_NPC(victim) && ch->in_room->area != victim->in_room->area )
    ||   (IS_NPC(victim) && (victim->pIndexData->vnum == ch->pcdata->questmob))
    ||   (IS_NPC(victim) && saves_spell( level, victim,DAM_OTHER) ) )
    {
	send_to_char( L"Başaramadın.\n\r", ch );
	return;
    }
    if (ch->pet != NULL && ch->in_room == ch->pet->in_room)
	gate_pet = TRUE;
    else
	gate_pet = FALSE;

    act( L"$n bir geçide girerek kayboluyor.",ch,NULL,NULL,TO_ROOM);
    send_to_char( L"Bir geçide girerek kayboluyorsun.\n\r",ch);
    char_from_room(ch);
    char_to_room(ch,victim->in_room);

    act( L"$n bir geçitten çıkıyor.",ch,NULL,NULL,TO_ROOM);
    do_look(ch,(wchar_t*)"auto");

    if (gate_pet)
    {
	act( L"$n bir geçide girerek kayboluyor.",ch->pet,NULL,NULL,TO_ROOM);
	send_to_char( L"Bir geçide girerek kayboluyorsun.\n\r",ch->pet);
	char_from_room(ch->pet);
	char_to_room(ch->pet,victim->in_room);
	act( L"$n bir geçitten çıkıyor.",ch->pet,NULL,NULL,TO_ROOM);
	do_look(ch->pet,(wchar_t*)"auto");
    }
}



void spell_giant_strength(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
    {
	if (victim == ch)
  send_to_char( L"Zaten olabildiğince güçlüsün!\n\r",ch);
else
  act( L"$N daha fazla güçlenemiyor.",ch,NULL,victim,TO_CHAR);
	return;
    }

    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level	 = level;
    af.duration  = (10 + level / 3);
    af.location  = APPLY_STR;
    af.modifier  = UMAX(2,level / 10);
    af.bitvector = 0;
    affect_to_char( victim, &af );
    send_to_char( L"Kasların yüksek güçle şişiyor!\n\r", victim );
    act( L"$s kasları yüksek güçle şişiyor.",victim,NULL,NULL,TO_ROOM);
    return;
}



void spell_harm( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = UMAX(  20, victim->hit - dice(1,4) );
    if ( saves_spell( level, victim,DAM_HARM) )
	dam = UMIN( 50, dam / 2 );
    dam = UMIN( 100, dam );
    damage( ch, victim, dam, sn, DAM_HARM ,TRUE);
    return;
}

/* RT haste spell */

void spell_haste( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) || IS_AFFECTED(victim,AFF_HASTE)
    ||   IS_SET(victim->off_flags,OFF_FAST))
    {
	if (victim == ch)
  send_to_char( L"Daha hızlı hareket edemiyorsun!\n\r",ch);
else
  act( L"$N olabildiğince hızlı hareket ediyor.",
	      ch,NULL,victim,TO_CHAR);
	return;
    }

    if (IS_AFFECTED(victim,AFF_SLOW))
    {
	if (!check_dispel(level,victim,skill_lookup( L"slow")))
	{
	    if (victim != ch)
      send_to_char( L"Büyü işe yaramadı.\n\r",ch);
  	    send_to_char( L"Bir an hızlandığını hissediyorsun.\n\r",victim);
	    return;
	}
  act( L"$n daha acelesiz hareket etmeye başladı.",victim,NULL,NULL,TO_ROOM);
	return;
    }

    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    if (victim == ch)
      af.duration  = level/2;
    else
      af.duration  = level/4;
    af.location  = APPLY_DEX;
    af.modifier  = UMAX(2,level / 12 );
    af.bitvector = AFF_HASTE;
    affect_to_char( victim, &af );
    send_to_char( L"Daha hızlı hareket etmeye başlıyorsun.\n\r", victim );
    act( L"$n daha hızlı hareket etmeye başladı.",victim,NULL,NULL,TO_ROOM);
    if ( ch != victim )
	send_to_char( L"Tamam.\n\r", ch );
    return;
}



void spell_heal( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    victim->hit = UMIN( victim->hit + 100 + level / 10, victim->max_hit );
    update_pos( victim );
    send_to_char( L"Sıcak bir duygu vücudunu sarıyor.\n\r", victim );
    if ( ch != victim )
	send_to_char( L"Tamam.\n\r", ch );
    return;
}

void spell_heat_metal( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    OBJ_DATA *obj_lose, *obj_next;
    int dam = 0;
    bool fail = TRUE;

   if (!saves_spell(level + 2,victim,DAM_FIRE)
   &&  !IS_SET(victim->imm_flags,IMM_FIRE))
   {
	for ( obj_lose = victim->carrying;
	      obj_lose != NULL;
	      obj_lose = obj_next)
	{
	    obj_next = obj_lose->next_content;
	    if ( number_range(1,2 * level) > obj_lose->level
	    &&   !saves_spell(level,victim,DAM_FIRE)
	    &&   is_metal(obj_lose)
	    &&   !IS_OBJ_STAT(obj_lose,ITEM_BURN_PROOF))
	    {
		switch ( obj_lose->item_type )
		{
		case ITEM_ARMOR:
		if (obj_lose->wear_loc != -1) /* remove the item */
		{
		    if (can_drop_obj(victim,obj_lose)
		    &&  (obj_lose->weight / 10) <
			number_range(1,2 * get_curr_stat(victim,STAT_DEX))
		    &&  remove_obj( victim, obj_lose, TRUE ))
		    {
          act( L"$n bağırarak $p'yi yere atıyor!",
    			    victim,obj_lose,NULL,TO_ROOM);
    			act( L"Seni yakmadan önce $p'yi çıkarıp atıyorsun.",
			    victim,obj_lose,NULL,TO_CHAR);
			dam += (number_range(1,obj_lose->level) / 3);
			obj_from_char(obj_lose);
			obj_to_room(obj_lose, victim->in_room);
			fail = FALSE;
		    }
		    else /* stuck on the body! ouch! */
		    {
          act( L"$p derini yakıyor!",
			    victim,obj_lose,NULL,TO_CHAR);
			dam += (number_range(1,obj_lose->level));
			fail = FALSE;
		    }

		}
		else /* drop it if we can */
		{
		    if (can_drop_obj(victim,obj_lose))
		    {
          act( L"$n bağırarak $p'yi yere atıyor!",
    			    victim,obj_lose,NULL,TO_ROOM);
    			act( L"Seni yakmadan önce $p'yi çıkarıp atıyorsun.",
			    victim,obj_lose,NULL,TO_CHAR);
			dam += (number_range(1,obj_lose->level) / 6);
			obj_from_char(obj_lose);
			obj_to_room(obj_lose, victim->in_room);
			fail = FALSE;
		    }
		    else /* cannot drop */
		    {
          act( L"$p derini yakıyor!",
			    victim,obj_lose,NULL,TO_CHAR);
			dam += (number_range(1,obj_lose->level) / 2);
			fail = FALSE;
		    }
		}
		break;
		case ITEM_WEAPON:
		if (obj_lose->wear_loc != -1) /* try to drop it */
		{
		    if (IS_WEAPON_STAT(obj_lose,WEAPON_FLAMING))
			continue;

		    if (can_drop_obj(victim,obj_lose)
		    &&  remove_obj(victim,obj_lose,TRUE))
		    {
          act( L"$n $p tarafından yakılınca onu yere attı.",
    			    victim,obj_lose,NULL,TO_ROOM);
    			send_to_char(L"Kor halindeki silahını yere atıyorsun!\n\r",
			    victim);
			dam += 1;
			obj_from_char(obj_lose);
			obj_to_room(obj_lose,victim->in_room);
			fail = FALSE;
		    }
		    else /* YOWCH! */
		    {
          send_to_char( L"Silahın etini yakıyor!\n\r",victim);

			dam += number_range(1,obj_lose->level);
			fail = FALSE;
		    }
		}
		else /* drop it if we can */
		{
		    if (can_drop_obj(victim,obj_lose))
		    {
          act( L"$n kor halindeki $p'yi yere atıyor!",
    			    victim,obj_lose,NULL,TO_ROOM);
    			act( L"Seni yakmadan önce $p'yi çıkarıp atıyorsun.",
			    victim,obj_lose,NULL,TO_CHAR);
			dam += (number_range(1,obj_lose->level) / 6);
			obj_from_char(obj_lose);
			obj_to_room(obj_lose, victim->in_room);
			fail = FALSE;
		    }
		    else /* cannot drop */
		    {
          act( L"$p derini yakıyor!",
			    victim,obj_lose,NULL,TO_CHAR);
			dam += (number_range(1,obj_lose->level) / 2);
			fail = FALSE;
		    }
		}
		break;
		}
	    }
	}
    }
    if (fail)
    {
      send_to_char( L"Büyünün etkisi olmadı.\n\r", ch);
    	send_to_char( L"Bir an ısındığını hissediyorsun.\n\r",victim);
    }
    else /* damage! */
    {
	if (saves_spell(level,victim,DAM_FIRE))
	    dam = 2 * dam / 3;
	damage(ch,victim,dam,sn,DAM_FIRE,TRUE);
    }
}

/* RT really nasty high-level attack spell */
void spell_holy_word(int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    wchar_t buf[MAX_STRING_LENGTH];
    int dam;
    int bless_num, curse_num, frenzy_num;

    bless_num = skill_lookup( L"bless");
    curse_num = skill_lookup( L"curse");
    frenzy_num = skill_lookup( L"frenzy");

    act( L"$n kutsal sözler mırıldanıyor!",ch,NULL,NULL,TO_ROOM);
   send_to_char( L"Kutsal sözler mırıldanıyorsun.\n\r",ch);

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
	vch_next = vch->next_in_room;

	if ((IS_GOOD(ch) && IS_GOOD(vch)) ||
	    (IS_EVIL(ch) && IS_EVIL(vch)) ||
	    (IS_NEUTRAL(ch) && IS_NEUTRAL(vch)) )
	{
    send_to_char( L"Kendini daha güçlü hissediyorsun.\n\r",vch);
	  spell_frenzy(frenzy_num,level,ch,(void *) vch,TARGET_CHAR);
	  spell_bless(bless_num,level,ch,(void *) vch,TARGET_CHAR);
	}

	else if ((IS_GOOD(ch) && IS_EVIL(vch)) ||
		 (IS_EVIL(ch) && IS_GOOD(vch)) )
	{
          if (!is_safe_spell(ch,vch,TRUE))
          {
            if (!IS_NPC(ch) && vch != ch &&
                ch->fighting != vch && vch->fighting != ch &&
                (IS_SET(vch->affected_by,AFF_CHARM) || !IS_NPC(vch)))
              {
            if (!can_see(vch, ch))
                do_yell(vch, (wchar_t*)"İmdat! Biri bana saldırıyor!");
            else
              {
                 swprintf( buf, MAX_STRING_LENGTH-1, L"Geber %s, seni büyücü köpek!",
                    (is_affected(ch,gsn_doppelganger)&&!IS_IMMORTAL(vch))?
                     ch->doppel->name : ch->name);
                 do_yell(vch,buf);
              }
          }

            spell_curse(curse_num,level,ch,(void *) vch, TARGET_CHAR);
            send_to_char( L"Dövülmüş durumdasın!\n\r",vch);
            dam = dice(level,6);
            damage(ch,vch,dam,sn,DAM_ENERGY, TRUE);
          }
        }

	else if (IS_NEUTRAL(ch))
	{
          if (!is_safe_spell(ch,vch,TRUE))
          {
            if (!IS_NPC(ch) && vch != ch &&
                ch->fighting != vch && vch->fighting != ch &&
                (IS_SET(vch->affected_by,AFF_CHARM) || !IS_NPC(vch)))
              {
            if (!can_see(vch, ch))
                do_yell(vch, (wchar_t*)"İmdat! Biri bana saldırıyor!");
            else
              {
                 swprintf( buf, MAX_STRING_LENGTH-1, L"Geber %s, seni büyücü köpek!",
                    (is_affected(ch,gsn_doppelganger)&&!IS_IMMORTAL(vch))?
                     ch->doppel->name : ch->name);
                 do_yell(vch,buf);
              }
          }

            spell_curse(curse_num,level/2,ch,(void *) vch, TARGET_CHAR);
            send_to_char( L"Dövülmüş durumdasın!\n\r",vch);
            dam = dice(level,4);
            damage(ch,vch,dam,sn,DAM_ENERGY, TRUE);
	  }
	}
    }

    send_to_char( L"Gücünün akıp gittiğini hissediyorsun.\n\r",ch);
    gain_exp( ch, -1 * number_range(1,10) * 5);
    ch->move /= (4/3);
    ch->hit /= (4/3);
}

void spell_identify( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    wchar_t buf[MAX_STRING_LENGTH];
    AFFECT_DATA *paf;

    swprintf( buf, MAX_STRING_LENGTH-1,L"Obje '%s', tip %s, ekstra özellik %s.\n\rAğırlık %d, değer %d, seviye %d.\n\r",

	obj->name,
	item_type_name( obj ),
	extra_bit_name( obj->extra_flags ),
	obj->weight / 10,
	obj->cost,
	obj->level
	);
    send_to_char( buf, ch );

	    if ( obj->pIndexData->limit != -1 )
		{
		 swprintf( buf, MAX_STRING_LENGTH-1,L"Bu ekipmanın limiti: %d \n\r",
				obj->pIndexData->limit);
	 send_to_char(buf,ch);
		}

    switch ( obj->item_type )
    {
    case ITEM_SCROLL:
    case ITEM_POTION:
    case ITEM_PILL:
    swprintf( buf, MAX_STRING_LENGTH-1, L"Seviye %d büyüleri:", obj->value[0] );
	send_to_char( buf, ch );

	if ( obj->value[1] >= 0 && obj->value[1] < MAX_SKILL )
	{
	    send_to_char( L" '", ch );
	    send_to_char( skill_table[obj->value[1]].name[1], ch );
	    send_to_char( L"'", ch );
	}

	if ( obj->value[2] >= 0 && obj->value[2] < MAX_SKILL )
	{
	    send_to_char( L" '", ch );
	    send_to_char( skill_table[obj->value[2]].name[1], ch );
	    send_to_char( L"'", ch );
	}

	if ( obj->value[3] >= 0 && obj->value[3] < MAX_SKILL )
	{
	    send_to_char( L" '", ch );
	    send_to_char( skill_table[obj->value[3]].name[1], ch );
	    send_to_char( L"'", ch );
	}

	if (obj->value[4] >= 0 && obj->value[4] < MAX_SKILL)
	{
	    send_to_char( L" '",ch);
	    send_to_char(skill_table[obj->value[4]].name[1],ch);
	    send_to_char( L"'",ch);
	}

	send_to_char( L".\n\r", ch );
	break;

    case ITEM_WAND:
    case ITEM_STAFF:
    swprintf( buf, MAX_STRING_LENGTH-1, L"%d seviyesinde %d şarjı var",
	    obj->value[0] , obj->value[2] );
	send_to_char( buf, ch );

	if ( obj->value[3] >= 0 && obj->value[3] < MAX_SKILL )
	{
	    send_to_char( L" '", ch );
	    send_to_char( skill_table[obj->value[3]].name[1], ch );
	    send_to_char( L"'", ch );
	}

	send_to_char( L".\n\r", ch );
	break;

    case ITEM_DRINK_CON:
    swprintf( buf, MAX_STRING_LENGTH-1, L"İçinde %s renginde %s var.\n\r",
	    liq_table[obj->value[2]].liq_color,
            liq_table[obj->value[2]].liq_name);
        send_to_char(buf,ch);
        break;

    case ITEM_CONTAINER:
    swprintf( buf, MAX_STRING_LENGTH-1, L"Kapasite: %d#  Maks. Ağırlık: %d#  Özellikler: %s\n\r",
	    obj->value[0], obj->value[3], cont_bit_name(obj->value[1]));
	send_to_char(buf,ch);
	if (obj->value[4] != 100)
	{
    swprintf( buf, MAX_STRING_LENGTH-1, L"Ağırlık çarpanı: %d%%\n\r",obj->value[4]);

	    send_to_char(buf,ch);
	}
	break;

    case ITEM_WEAPON:
    send_to_char( L"Silah türü ",ch);
	switch (obj->value[0])
	{
    case(WEAPON_EXOTIC) : send_to_char( L"egzotik.\n\r",ch);	break;
    case(WEAPON_SWORD)  : send_to_char( L"kılıç.\n\r",ch);	break;
    case(WEAPON_DAGGER) : send_to_char( L"hançer.\n\r",ch);	break;
    case(WEAPON_SPEAR)	: send_to_char( L"mızrak/asa.\n\r",ch);	break;
    case(WEAPON_MACE) 	: send_to_char( L"topuz/çomak.\n\r",ch);	break;
    case(WEAPON_AXE)	: send_to_char( L"balta.\n\r",ch);		break;
    case(WEAPON_FLAIL)	: send_to_char( L"döven.\n\r",ch);	break;
    case(WEAPON_WHIP)	: send_to_char( L"kırbaç.\n\r",ch);		break;
    case(WEAPON_POLEARM): send_to_char( L"teber.\n\r",ch);	break;
    case(WEAPON_BOW)	: send_to_char( L"yay.\n\r",ch);		break;
    case(WEAPON_ARROW)	: send_to_char( L"ok.\n\r",ch);	break;
    case(WEAPON_LANCE)	: send_to_char( L"kargı.\n\r",ch);	break;
    default		: send_to_char( L"bilinmiyor.\n\r",ch);	break;
 	}
	if (obj->pIndexData->new_format)
  swprintf( buf, MAX_STRING_LENGTH-1, L"Zarar %dd%d (ortalama %d).\n\r",
		obj->value[1],obj->value[2],
		(1 + obj->value[2]) * obj->value[1] / 2);
	else
  swprintf( buf, MAX_STRING_LENGTH-1, L"Zarar %d - %d (ortalama %d).\n\r",
	    	obj->value[1], obj->value[2],
	    	( obj->value[1] + obj->value[2] ) / 2 );
	send_to_char( buf, ch );
        if (obj->value[4])  /* weapon flags */
        {
          swprintf( buf, MAX_STRING_LENGTH-1, L"Silah özellikleri: %s\n\r",weapon_bit_name(obj->value[4]));
            send_to_char(buf,ch);
	}
	break;

    case ITEM_ARMOR:
	swprintf( buf, MAX_STRING_LENGTH-1,L"Zırh sınıfı: delici %d, omuz %d, kesiş %d ve büyü %d.\n\r",
	    obj->value[0], obj->value[1], obj->value[2], obj->value[3] );
	send_to_char( buf, ch );
	break;
    }

    if (!obj->enchanted)
    for ( paf = obj->pIndexData->affected; paf != NULL; paf = paf->next )
    {
	if ( paf->location != APPLY_NONE && paf->modifier != 0 )
	{
    swprintf( buf, MAX_STRING_LENGTH-1, L"%s etkisi %d birim.\n\r",
		affect_loc_name( paf->location ), paf->modifier );
	    send_to_char(buf,ch);
            if (paf->bitvector)
	    {
                switch(paf->where)
                {
                    case TO_AFFECTS:
                        swprintf( buf, MAX_STRING_LENGTH-1, L"%s etkisi ekler.\n",
                            affect_bit_name(paf->bitvector));
                        break;
                    case TO_OBJECT:
                        swprintf( buf, MAX_STRING_LENGTH-1, L"%s obje özelliği ekler.\n",
                            extra_bit_name(paf->bitvector));
                        break;
                    case TO_IMMUNE:
                        swprintf( buf, MAX_STRING_LENGTH-1, L"%s bağışıklığı ekler.\n",
                            imm_bit_name(paf->bitvector));
                        break;
					case TO_RESIST:
                        swprintf( buf, MAX_STRING_LENGTH-1, L"%s direnci ekler.\n\r",
                            imm_bit_name(paf->bitvector));
                        break;
                    case TO_VULN:
						swprintf( buf, MAX_STRING_LENGTH-1, L"%s dayanıksızlığı ekler.\n\r",
                            imm_bit_name(paf->bitvector));
                        break;
                    case TO_DETECTS:
						swprintf( buf, MAX_STRING_LENGTH-1, L"%s saptaması ekler.\n\r",
                            detect_bit_name(paf->bitvector));
                        break;
                    default:
                        swprintf( buf, MAX_STRING_LENGTH-1, L"Bilinmeyen bit %d: %d\n\r",
                            paf->where,paf->bitvector);
                        break;
                }
	        send_to_char( buf, ch );
	    }
	}
    }

    for ( paf = obj->affected; paf != NULL; paf = paf->next )
    {
	if ( paf->location != APPLY_NONE && paf->modifier != 0 )
	{
    swprintf( buf, MAX_STRING_LENGTH-1, L"%s etkisi %d birim",
	    	affect_loc_name( paf->location ), paf->modifier );
	    send_to_char( buf, ch );
	    if ( paf->duration > -1)
      swprintf( buf, MAX_STRING_LENGTH-1, L", %d saat.\n\r",paf->duration);
            else
                swprintf( buf, MAX_STRING_LENGTH-1, L".\n\r");
	    send_to_char(buf,ch);

            if (paf->bitvector)
            {
                switch(paf->where)
                {
                    case TO_AFFECTS:
                    swprintf( buf, MAX_STRING_LENGTH-1, L"%s etkisi ekler.\n",
                            affect_bit_name(paf->bitvector));
                        break;
                    case TO_OBJECT:
                    swprintf( buf, MAX_STRING_LENGTH-1, L"%s obje etkisi ekler.\n",
			    extra_bit_name(paf->bitvector));
                        break;
		    case TO_WEAPON:
			swprintf( buf, MAX_STRING_LENGTH-1, L"Adds %s weapon flags.\n",
			    weapon_bit_name(paf->bitvector));
			break;
      case TO_IMMUNE:
          swprintf( buf, MAX_STRING_LENGTH-1, L"%s bağışıklığı ekler.\n",
              imm_bit_name(paf->bitvector));
          break;
      case TO_RESIST:
          swprintf( buf, MAX_STRING_LENGTH-1, L"%s direnci ekler.\n\r",
              imm_bit_name(paf->bitvector));
          break;
      case TO_VULN:
          swprintf( buf, MAX_STRING_LENGTH-1, L"%s dayanıksızlığı ekler.\n\r",
              imm_bit_name(paf->bitvector));
          break;
      case TO_DETECTS:
          swprintf( buf, MAX_STRING_LENGTH-1, L"%s saptaması ekler.\n\r",
              detect_bit_name(paf->bitvector));
          break;
      default:
          swprintf( buf, MAX_STRING_LENGTH-1, L"Bilinmeyen bit %d: %d\n\r",
			    paf->where,paf->bitvector);
                        break;
                }
                send_to_char(buf,ch);
            }
	}
    }

    return;
}



void spell_infravision( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_INFRARED) )
    {
	if (victim == ch)
  send_to_char( L"Zaten ısı görüşün var.\n\r",ch);
else
  act( L"$N zaten ısı görüşü var.\n\r",ch,NULL,victim,TO_CHAR);
return;
  }
  act( L"$s gözleri kızıl renkte parlıyor.\n\r", ch, NULL, NULL, TO_ROOM );

    af.where	 = TO_AFFECTS;
    af.type      = sn;
    af.level	 = level;
    af.duration  = 2 * level;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_INFRARED;
    affect_to_char( victim, &af );
    send_to_char( L"Gözlerin kızıl renkte parlıyor.\n\r", victim );
    return;
}



void spell_invis( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    AFFECT_DATA af;

    /* object invisibility */
    if (target == TARGET_OBJ)
    {
	obj = (OBJ_DATA *) vo;

	if (IS_OBJ_STAT(obj,ITEM_INVIS))
	{
    act( L"$p zaten görünmez.",ch,obj,NULL,TO_CHAR);
	    return;
	}

	af.where	= TO_OBJECT;
	af.type		= sn;
	af.level	= level;
	af.duration	= level / 4 + 12;
	af.location	= APPLY_NONE;
	af.modifier	= 0;
	af.bitvector	= ITEM_INVIS;
	affect_to_obj(obj,&af);

  act( L"$p görüş dışına çıkıyor.",ch,obj,NULL,TO_ALL);
	return;
    }

    /* character invisibility */
    victim = (CHAR_DATA *) vo;

    if ( IS_AFFECTED(victim, AFF_INVISIBLE) )
	return;

  act( L"$n görünmez oluyor.", victim, NULL, NULL, TO_ROOM );

    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = (level / 8 + 10);
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_INVISIBLE;
    affect_to_char( victim, &af );
    send_to_char( L"Görünmez oluyorsun.\n\r", victim );
    return;
}



void spell_know_alignment(int sn,int level,CHAR_DATA *ch,void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    const wchar_t *msg;

	 if ( IS_GOOD(victim) ) msg = L"$N saf ve iyi bir aurası var.";
    else if ( IS_NEUTRAL(victim) ) msg = L"$N yansız görünüyor.";
    else msg = L"$N kem bir auraya sahip.";

    act( msg, ch, NULL, victim, TO_CHAR );

    if (!IS_NPC(victim))
    {
     if (victim->ethos == 1)		msg = L"$N kanunları savunuyor.";
     else if (victim->ethos == 2) 	msg = L"$N toplum değerlerine karşı hissiz.";
     else if (victim->ethos == 3) 	msg = L"$N kaotik görünüyor.";
     else msg = L"$N etik olarak neye bağlı olacağını bilemiyor.";
     act( msg, ch, NULL, victim, TO_CHAR);
    }
    return;
}



void spell_lightning_bolt(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    if (CAN_DETECT(victim, ADET_GROUNDING))
    {
      send_to_char( L"Elektrik düşmanlarında sönüp gidiyor.\n\r",victim);
    	act( L"Bir yıldırım $S düşmanlarında sönüp gidiyor.\n\r",
		ch, NULL, victim, TO_ROOM);
	return;
    }
    dam = dice(level,4) + 12;
    if ( saves_spell( level, victim,DAM_LIGHTNING) )
	dam /= 2;
    damage( ch, victim, dam, sn, DAM_LIGHTNING ,TRUE);
    return;
}



void spell_locate_object( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    wchar_t buf[MAX_INPUT_LENGTH];
    BUFFER *buffer;
    OBJ_DATA *obj;
    OBJ_DATA *in_obj;
    bool found;
    int number = 0, max_found;

    found = FALSE;
    number = 0;
    max_found = IS_IMMORTAL(ch) ? 200 : 2 * level;

    buffer = new_buf();

    for ( obj = object_list; obj != NULL; obj = obj->next )
    {
	if ( !can_see_obj( ch, obj ) || !is_name( target_name, obj->name )
	||   IS_OBJ_STAT(obj,ITEM_NOLOCATE) || number_percent() > 2 * level
	||   ch->level < obj->level)
	    continue;

	found = TRUE;
	number++;

	for ( in_obj = obj; in_obj->in_obj != NULL; in_obj = in_obj->in_obj )
	    ;

	if ( in_obj->carried_by != NULL && can_see(ch,in_obj->carried_by))
	{
    swprintf( buf, MAX_STRING_LENGTH-1, L"bir tanesini taşıyan: %s\n\r",
		PERS(in_obj->carried_by, ch) );
	}
	else
	{
	    if (IS_IMMORTAL(ch) && in_obj->in_room != NULL)
      swprintf( buf, MAX_STRING_LENGTH-1, L"bir tanesinin yeri: %s [Oda %d]\n\r",
		    in_obj->in_room->name, in_obj->in_room->vnum);
	    else
      swprintf( buf, MAX_STRING_LENGTH-1, L"bir tanesinin yeri: %s\n\r",
		    in_obj->in_room == NULL
			? L"bir yer" : in_obj->in_room->name );
	}

	buf[0] = towupper(buf[0]);
	add_buf(buffer,buf);

	if (number >= max_found)
	    break;
    }

    if ( !found )
    send_to_char( L"Ne bu dünyada, ne de diğerinde böyle bir şey yok.\n\r", ch );
    else
	page_to_char(buf_string(buffer),ch);

    free_buf(buffer);

    return;
}



void spell_magic_missile( int sn, int level, CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;

    static const sh_int dam_each[] =
    {
	 0,
	 3,  3,  4,  4,  5,	 6,  6,  6,  6,  6,
	 7,  7,  7,  7,  7,	 8,  8,  8,  8,  8,
	 9,  9,  9,  9,  9,	10, 10, 10, 10, 10,
	11, 11, 11, 11, 11,	12, 12, 12, 12, 12,
	13, 13, 13, 13, 13,	14, 14, 14, 14, 14
    };

    int dam;

    if ( is_affected( ch, 67 ) )  {
	if ( ch->level > 4 )  {
    send_to_char( L"Büyülü fişeklerin düşmanına varmadan yitiyor.\n\r", ch );
          act( L"Kalkanın $S büyülü fişeklerini karşılıyor.", victim, NULL, ch, TO_CHAR);
        }
	else  {
    send_to_char( L"Büyülü fişeğin düşmanına varmadan yitiyor.\n\r", ch );
          act( L"Kalkanın $S büyülü fişeğini karşılıyor.", victim, NULL, ch, TO_CHAR);
	}
	return;
    }


    level	= UMIN(level, (int)(sizeof(dam_each)/sizeof(dam_each[0])) - 1);
    level	= UMAX(0, level);
	if (ch->level > 50)
    dam		= level / 4;
	else
    dam		= number_range( dam_each[level] / 2, dam_each[level] * 2 );

    if ( saves_spell( level, victim,DAM_ENERGY) )
	dam /= 2;
    damage( ch, victim, dam, sn, DAM_ENERGY ,TRUE);
    if ( ch->level > 4 )  {
      dam = number_range( dam_each[level] / 2, dam_each[level] * 2 );
      if ( saves_spell( level, victim,DAM_ENERGY) )
  	  dam /= 2;
      damage( ch, victim, dam, sn, DAM_ENERGY ,TRUE);
    }
    if ( ch->level > 8 )  {
      dam = number_range( dam_each[level] / 2, dam_each[level] * 2 );
      if ( saves_spell( level, victim,DAM_ENERGY) )
  	  dam /= 2;
      damage( ch, victim, dam, sn, DAM_ENERGY ,TRUE);
    }
    if ( ch->level > 12 )  {
      dam = number_range( dam_each[level] / 2, dam_each[level] * 2 );
      if ( saves_spell( level, victim,DAM_ENERGY) )
  	  dam /= 2;
      damage( ch, victim, dam, sn, DAM_ENERGY ,TRUE);
    }
    if ( ch->level > 16 )  {
      dam = number_range( dam_each[level] / 2, dam_each[level] * 2 );
      if ( saves_spell( level, victim,DAM_ENERGY) )
  	  dam /= 2;
      damage( ch, victim, dam, sn, DAM_ENERGY ,TRUE);
    }

    return;
}

void spell_mass_healing(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    CHAR_DATA *gch;
    int heal_num, refresh_num;

    heal_num = skill_lookup( L"heal");
    refresh_num = skill_lookup( L"refresh");

    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
	if ((IS_NPC(ch) && IS_NPC(gch)) ||
	    (!IS_NPC(ch) && !IS_NPC(gch)))
	{
	    spell_heal(heal_num,level,ch,(void *) gch,TARGET_CHAR);
	    spell_refresh(refresh_num,level,ch,(void *) gch,TARGET_CHAR);
	}
    }
}


void spell_mass_invis( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    AFFECT_DATA af;
    CHAR_DATA *gch;

    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
	if ( !is_same_group( gch, ch ) || IS_AFFECTED(gch, AFF_INVISIBLE) )
	    continue;
      act( L"$n yavaşça kayboluyor.", gch, NULL, NULL, TO_ROOM );
    	send_to_char( L"Yavaşça kayboluyorsun.\n\r", gch );

	af.where     = TO_AFFECTS;
	af.type      = sn;
	af.level     = level/2;
	af.duration  = 24;
	af.location  = APPLY_NONE;
	af.modifier  = 0;
	af.bitvector = AFF_INVISIBLE;
	affect_to_char( gch, &af );
    }
    send_to_char( L"Tamam.\n\r", ch );

    return;
}



void spell_null( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  send_to_char( L"O bir büyü değil!\n\r", ch );
    return;
}



void spell_pass_door( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_PASS_DOOR) )
    {
	if (victim == ch)
  send_to_char( L"Zaten şeffafsın.\n\r",ch);
else
  act( L"$N zaten şeffaf.",ch,NULL,victim,TO_CHAR);
	return;
    }

    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = number_fuzzy( level / 4 );
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_PASS_DOOR;
    affect_to_char( victim, &af );
    act( L"$n saydamlaşıyor.", victim, NULL, NULL, TO_ROOM );
    send_to_char( L"Saydamlaşıyorsun.\n\r", victim );
    return;
}

/* RT plague spell, very nasty */

void spell_plague( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if (saves_spell(level,victim,DAM_DISEASE) ||
	(IS_NPC(victim) && IS_SET(victim->act,ACT_UNDEAD)))
    {
	if (ch == victim)
  send_to_char( L"Bir an için kendini hasta hissediyorsun, ama sonra geçiyor.\n\r",ch);
else
  act( L"$N etkilenmiş görünmüyor.",ch,NULL,victim,TO_CHAR);
	return;
    }

    af.where     = TO_AFFECTS;
    af.type 	 = sn;
    af.level	 = level * 3/4;
    af.duration  = (10 + level / 10);
    af.location  = APPLY_STR;
    af.modifier  = -1 * UMAX(1,3 + level / 15);
    af.bitvector = AFF_PLAGUE;
    affect_join(victim,&af);

    send_to_char(L"Veba derini yaktıkça acı içinde çığlık atıyorsun.\n\r",victim);
  act( L"Veba derisini yaktıkça $n acı içinde çığlık atıyor.",
	victim,NULL,NULL,TO_ROOM);
}

void spell_poison( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    AFFECT_DATA af;


    if (target == TARGET_OBJ)
    {
	obj = (OBJ_DATA *) vo;

	if (obj->item_type == ITEM_FOOD || obj->item_type == ITEM_DRINK_CON)
	{
	    if (IS_OBJ_STAT(obj,ITEM_BLESS) || IS_OBJ_STAT(obj,ITEM_BURN_PROOF))
	    {
        act( L"$p büyünle bozulmuyor.",ch,obj,NULL,TO_CHAR);
		return;
	    }
	    obj->value[3] = 1;
      act( L"$p zehirli buharla aşılandı.",ch,obj,NULL,TO_ALL);
	    return;
	}

	if (obj->item_type == ITEM_WEAPON)
	{
	    if (IS_WEAPON_STAT(obj,WEAPON_FLAMING)
	    ||  IS_WEAPON_STAT(obj,WEAPON_FROST)
	    ||  IS_WEAPON_STAT(obj,WEAPON_VAMPIRIC)
	    ||  IS_WEAPON_STAT(obj,WEAPON_SHARP)
	    ||  IS_WEAPON_STAT(obj,WEAPON_VORPAL)
	    ||  IS_WEAPON_STAT(obj,WEAPON_SHOCKING)
	    ||  IS_WEAPON_STAT(obj,WEAPON_HOLY)
	    ||  IS_OBJ_STAT(obj,ITEM_BLESS) || IS_OBJ_STAT(obj,ITEM_BURN_PROOF))
	    {
        act( L"$p'yi zehirleyemezsin.",ch,obj,NULL,TO_CHAR);
		return;
	    }

	    if (IS_WEAPON_STAT(obj,WEAPON_POISON))
	    {
        act( L"$p zaten zehirlenmiş.",ch,obj,NULL,TO_CHAR);
		return;
	    }

	    af.where	 = TO_WEAPON;
	    af.type	 = sn;
	    af.level	 = level / 2;
	    af.duration	 = level/8;
	    af.location	 = 0;
	    af.modifier	 = 0;
	    af.bitvector = WEAPON_POISON;
	    affect_to_obj(obj,&af);

      act( L"$p ölümcül zehirle kaplandı.",ch,obj,NULL,TO_ALL);
	    return;
	}

  act( L"$p'yi zehirleyemezsin.",ch,obj,NULL,TO_CHAR);
	return;
    }

    victim = (CHAR_DATA *) vo;

    if ( saves_spell( level, victim,DAM_POISON) )
    {
      act( L"$s benzi bir an için yeşile dönüyor.",victim,NULL,NULL,TO_ROOM);
    	send_to_char( L"Bir an için hastalandığını hissediyorsun, fakat sonra geçiyor.\n\r",victim);
	return;
    }

    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = (10 + level / 10);
    af.location  = APPLY_STR;
    af.modifier  = -2;
    af.bitvector = AFF_POISON;
    affect_join( victim, &af );
    send_to_char( L"Kendini hasta hissediyorsun.\n\r", victim );
    act( L"$n çok hasta görünüyor.",victim,NULL,NULL,TO_ROOM);
    return;
}



void spell_protection_evil(int sn,int level,CHAR_DATA *ch,void *vo, int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_PROTECT_EVIL)
    ||   IS_AFFECTED(victim, AFF_PROTECT_GOOD))
    {
	if (victim == ch)
  send_to_char( L"Zaten korunuyorsun.\n\r",ch);
else
  act( L"$N zaten korunuyor.",ch,NULL,victim,TO_CHAR);
	return;
    }

    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = (10 + level / 5);
    af.location  = APPLY_SAVING_SPELL;
    af.modifier  = -1;
    af.bitvector = AFF_PROTECT_EVIL;
    affect_to_char( victim, &af );
    send_to_char( L"Kendini kutsal ve saf hissediyorsun.\n\r", victim );
    if ( ch != victim )
	act( L"$N kemden korunuyor.",ch,NULL,victim,TO_CHAR);
    return;
}

void spell_protection_good(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_PROTECT_GOOD)
    ||   IS_AFFECTED(victim, AFF_PROTECT_EVIL))
    {
	if (victim == ch)
  send_to_char( L"Zaten korunuyorsun.\n\r",ch);
else
  act( L"$N zaten korunuyor.",ch,NULL,victim,TO_CHAR);
	return;
    }

    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = (10 + level / 5);
    af.location  = APPLY_SAVING_SPELL;
    af.modifier  = -1;
    af.bitvector = AFF_PROTECT_GOOD;
    affect_to_char( victim, &af );
    send_to_char( L"Karanlığa yaklaşıyorsun.\n\r", victim );
    if ( ch != victim )
	act( L"$N iyiden korunuyor.",ch,NULL,victim,TO_CHAR);
    return;
}


void spell_ray_of_truth (int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam, align;

    if (IS_EVIL(ch) )
    {
	victim = ch;
  send_to_char( L"Enerji içinde patlıyor!\n\r",ch);
    }

    if (victim != ch)
    {
      act( L"$n elini kaldırdığında kör edici bir ışık ışını ileri fırlıyor!",ch,NULL,NULL,TO_ROOM);
  		send_to_char( L"Elini kaldırdığında kör edici bir ışık ışını ileri fırlıyor!\n\r",ch);
    }

    if (IS_GOOD(victim))
    {
      act( L"$n ışıktan zarar görmüş görünmüyor.",victim,NULL,victim,TO_ROOM);
  		send_to_char( L"Işık seni etkileyemeyecek kadar güçsüz.\n\r",victim);
	return;
    }

    dam = dice( level, 10 );
    if ( saves_spell( level, victim,DAM_HOLY) )
	dam /= 2;

    align = victim->alignment;
    align -= 350;

    if (align < -1000)
	align = -1000 + (align + 1000) / 3;

    dam = (dam * align * align) / 1000000;

    damage( ch, victim, dam, sn, DAM_HOLY ,TRUE);
    spell_blindness(gsn_blindness,
	3 * level / 4, ch, (void *) victim,TARGET_CHAR);
}


void spell_recharge( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    int chance, percent;

    if (obj->item_type != ITEM_WAND && obj->item_type != ITEM_STAFF)
    {
      send_to_char( L"Şarjı yok.\n\r",ch);
	return;
    }

    if (obj->value[3] >= 3 * level / 2)
    {
      send_to_char( L"Bu iş yeteneklerini aşar.\n\r",ch);
	return;
    }

    if (obj->value[1] == 0)
    {
      send_to_char( L"Bir kez şarj edilmiş zaten.\n\r",ch);
	return;
    }

    chance = 40 + 2 * level;

    chance -= obj->value[3]; /* harder to do high-level spells */
    chance -= (obj->value[1] - obj->value[2]) *
	      (obj->value[1] - obj->value[2]);

    chance = UMAX(level/2,chance);

    percent = number_percent();

    if (percent < chance / 2)
    {
      act( L"$p hafifçe parlıyor.",ch,obj,NULL,TO_CHAR);
    	act( L"$p hafifçe parlıyor.",ch,obj,NULL,TO_ROOM);
	obj->value[2] = UMAX(obj->value[1],obj->value[2]);
	obj->value[1] = 0;
	return;
    }

    else if (percent <= chance)
    {
	int chargeback,chargemax;

  act( L"$p hafifçe parlıyor.",ch,obj,NULL,TO_CHAR);
	act( L"$p hafifçe parlıyor.",ch,obj,NULL,TO_CHAR);

	chargemax = obj->value[1] - obj->value[2];

	if (chargemax > 0)
	    chargeback = UMAX(1,chargemax * percent / 100);
	else
	    chargeback = 0;

	obj->value[2] += chargeback;
	obj->value[1] = 0;
	return;
    }

    else if (percent <= UMIN(95, 3 * chance / 2))
    {
      send_to_char( L"Değişen bir şey yok.\n\r",ch);
	if (obj->value[1] > 1)
	    obj->value[1]--;
	return;
    }

    else /* whoops! */
    {
      act( L"$p ışıldayarak patlıyor!",ch,obj,NULL,TO_CHAR);
    	act( L"$p ışıldayarak patlıyor!",ch,obj,NULL,TO_ROOM);
	extract_obj(obj);
    }
}

void spell_refresh( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    victim->move = UMIN( victim->move + level, victim->max_move );
    if (victim->max_move == victim->move)
    send_to_char( L"Tamamen tazelendin!\n\r",victim);
    else
    send_to_char( L"Yorgunluğunun azaldığını hissediyorsun.\n\r", victim );
    if ( ch != victim )
	send_to_char( L"Tamam.\n\r", ch );
    return;
}

void spell_remove_curse( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    bool found = FALSE;

    /* do object cases first */
    if (target == TARGET_OBJ)
    {
	obj = (OBJ_DATA *) vo;

	if (IS_OBJ_STAT(obj,ITEM_NODROP) || IS_OBJ_STAT(obj,ITEM_NOREMOVE))
	{
	    if (!IS_OBJ_STAT(obj,ITEM_NOUNCURSE)
	    &&  !saves_dispel(level + 2,obj->level,0))
	    {
		REMOVE_BIT(obj->extra_flags,ITEM_NODROP);
		REMOVE_BIT(obj->extra_flags,ITEM_NOREMOVE);
    act( L"$p mavi renkte parlıyor.",ch,obj,NULL,TO_ALL);
		return;
	    }

      act( L"$p üzerindeki lanet seni aşar.",ch,obj,NULL,TO_CHAR);
	    return;
	}
	else  {
    send_to_char( L"Değişen bir şey yok...\n\r", ch );
	  return;
	}
    }

    /* characters */
    victim = (CHAR_DATA *) vo;

    if (check_dispel(level,victim,gsn_curse))
    {
      send_to_char( L"Kendini daha iyi hissediyorsun.\n\r",victim);
    	act( L"$n rahatlamış görünüyor.",victim,NULL,NULL,TO_ROOM);
    }

   for (obj = victim->carrying; (obj != NULL && !found); obj = obj->next_content)
   {
	if ((IS_OBJ_STAT(obj,ITEM_NODROP) || IS_OBJ_STAT(obj,ITEM_NOREMOVE))
	&&  !IS_OBJ_STAT(obj,ITEM_NOUNCURSE))
	{   /* attempt to remove curse */
	    if (!saves_dispel(level,obj->level,0))
	    {
		found = TRUE;
		REMOVE_BIT(obj->extra_flags,ITEM_NODROP);
		REMOVE_BIT(obj->extra_flags,ITEM_NOREMOVE);
    act( L"Sana ait $p mavi renkte parlıyor.",victim,obj,NULL,TO_CHAR);
		act( L"$s $p mavi renkte parlıyor.",victim,obj,NULL,TO_ROOM);
	    }
	 }
    }
}

void spell_sanctuary( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_SANCTUARY) )
    {
	if (victim == ch)
  send_to_char( L"Zaten takdislisin.\n\r",ch);
else
  act( L"$N zaten takdisli.",ch,NULL,victim,TO_CHAR);
	return;
    }

    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = level / 6;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_SANCTUARY;
    affect_to_char( victim, &af );
    act( L"$n beyaz bir ayla ile çevreleniyor.", victim, NULL, NULL, TO_ROOM );
    send_to_char( L"Beyaz bir ayla ile çevreleniyorsun.\n\r", victim );
    return;
}



void spell_shield( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
    {
	if (victim == ch)
  send_to_char( L"Zaten kalkanlısın.\n\r",ch);
else
  act( L"$N zaten bir kalkanla korunuyor.",ch,NULL,victim,TO_CHAR);
	return;
    }

    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = (8 + level / 3);
    af.location  = APPLY_AC;
    af.modifier  = -1 * UMAX(20,10 + level / 3); /* af.modifier  = -20;*/
    af.bitvector = 0;
    affect_to_char( victim, &af );
    act( L"$n bir güç kalkanıyla çevreleniyor.", victim, NULL, NULL, TO_ROOM );
    send_to_char( L"Bir güç kalkanıyla çevreleniyorsun.\n\r", victim );
    return;
}



void spell_shocking_grasp(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;

    static const int dam_each[] =
    {
	 6,
	 8,  10,  12,  14,  16,	 18, 20, 25, 29, 33,
	36, 39, 39, 39, 40,	40, 41, 41, 42, 42,
	43, 43, 44, 44, 45,	45, 46, 46, 47, 47,
	48, 48, 49, 49, 50,	50, 51, 51, 52, 52,
	53, 53, 54, 54, 55,	55, 56, 56, 57, 57
    };

    int dam;

    level	= UMIN(level, (int)(sizeof(dam_each)/sizeof(dam_each[0])) - 1);
    level	= UMAX(0, level);
	if (ch->level > 50)
    dam 	= level / 2 ;
	else
    dam		= number_range( dam_each[level] / 2, dam_each[level] * 2 );
    if ( saves_spell( level, victim,DAM_LIGHTNING) )
	dam /= 2;
    damage( ch, victim, dam, sn, DAM_LIGHTNING ,TRUE);
    return;
}



void spell_sleep( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_SLEEP)
    ||   (IS_NPC(victim) && IS_SET(victim->act,ACT_UNDEAD))
    ||   level < victim->level
    ||   saves_spell( level-4, victim,DAM_CHARM) )
	return;

    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = 1 + level/10;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_SLEEP;
    affect_join( victim, &af );

    if ( IS_AWAKE(victim) )
    {
      send_to_char( L"Uykunun geldiğini hissediyorsun ..... zzzzzz.\n\r", victim );
    	act( L"$n uyumaya başlıyor.", victim, NULL, NULL, TO_ROOM );
	victim->position = POS_SLEEPING;
    }
    return;
}

void spell_slow( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) || IS_AFFECTED(victim,AFF_SLOW))
    {
	if (victim == ch)
  send_to_char( L"Daha yavaş hareket edemezsin!\n\r",ch);
	else
  act( L"$N daha yavaş hareket edemez.",
	      ch,NULL,victim,TO_CHAR);
	return;
    }

    if (saves_spell(level,victim,DAM_OTHER)
    ||  IS_SET(victim->imm_flags,IMM_MAGIC))
    {
	if (victim != ch)
  send_to_char( L"Değişen birşey yok.\n\r",ch);
send_to_char( L"Uyuşuk hissediyorsun.\n\r",victim);
	return;
    }

    if (IS_AFFECTED(victim,AFF_HASTE))
    {
	if (!check_dispel(level,victim,skill_lookup( L"haste")))
	{
	    if (victim != ch)
		send_to_char( L"Büyü işe yaramadı.\n\r",ch);
    send_to_char( L"Bir an yavaşladığını hissediyorsun.\n\r",victim);
	    return;
	}

  act( L"$n daha yavaş hareket ediyor.",victim,NULL,NULL,TO_ROOM);
	return;
    }


    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = (4 + level / 12);
    af.location  = APPLY_DEX;
    af.modifier  = - UMAX(2,level / 12);
    af.bitvector = AFF_SLOW;
    affect_to_char( victim, &af );
    send_to_char( L"Yavaş l a d ı ğ ı n ı   hissediyorsun...\n\r", victim );
    act( L"$n yavaş hareket etmeye başlıyor.",victim,NULL,NULL,TO_ROOM);
    return;
}




void spell_stone_skin( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( ch, sn ) )
    {
      if (victim == ch)
    	  send_to_char( L"Derin zaten kaya kadar sert.\n\r",ch);
    	else
    	  act( L"$N zaten olabildiğince sert.",ch,NULL,victim,TO_CHAR);
    	return;
    }

    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = (10 + level / 5);
    af.location  = APPLY_AC;
    af.modifier  = -1 * UMAX(40,20 + level / 2);  /*af.modifier=-40;*/
    af.bitvector = 0;
    affect_to_char( victim, &af );
    act( L"$s derisi taşa dönüyor.", victim, NULL, NULL, TO_ROOM );
    send_to_char( L"Derin taşa dönüyor.\n\r", victim );
    return;
}



void spell_summon( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim;
    wchar_t buf[MAX_INPUT_LENGTH];

    if ( ( victim = get_char_world( ch, target_name ) ) == NULL
    ||   victim == ch
    ||   victim->in_room == NULL
    ||   IS_SET(ch->in_room->room_flags, ROOM_SAFE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_SAFE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_PRIVATE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_SOLITARY)
    ||   IS_SET(ch->in_room->room_flags, ROOM_NOSUMMON)
    ||   IS_SET(victim->in_room->room_flags, ROOM_NOSUMMON)
    ||   (IS_NPC(victim) && IS_SET(victim->act,ACT_AGGRESSIVE))
    ||   victim->level >= level + 3
    ||   (!IS_NPC(victim) && victim->level >= LEVEL_IMMORTAL)
    ||   victim->fighting != NULL
    ||   (IS_NPC(victim) && IS_SET(victim->imm_flags,IMM_SUMMON))
    ||	 (IS_NPC(victim) && victim->pIndexData->pShop != NULL)
    ||   (!IS_NPC(victim) && is_safe_nomessage(ch,victim) && IS_SET(victim->act,PLR_NOSUMMON))
    ||   (saves_spell( level, victim,DAM_OTHER))
    ||   (ch->in_room->area != victim->in_room->area && !IS_NPC(victim))
    ||   (IS_NPC(victim) && (victim->pIndexData->vnum == ch->pcdata->questmob))
    ||   (victim->in_room->exit[0] == NULL &&
          victim->in_room->exit[1] == NULL &&
          victim->in_room->exit[2] == NULL &&
          victim->in_room->exit[3] == NULL &&
          victim->in_room->exit[4] == NULL && victim->in_room->exit[5] == NULL) )
    {
	send_to_char( L"Başaramadın.\n\r", ch );
	return;
    }

    if (IS_NPC(victim) && victim->in_mind == NULL)
	{
	 swprintf( buf, MAX_STRING_LENGTH-1, L"%d",victim->in_room->vnum);
	 victim->in_mind = str_dup( buf );
	}

  act( L"$n aniden yokoluyor.", victim, NULL, NULL, TO_ROOM );
  char_from_room( victim );
  char_to_room( victim, ch->in_room );
  act( L"$n aniden beliriyor.", victim, NULL, NULL, TO_ROOM );
  act( L"$n seni çağırdı!", ch, NULL, victim,   TO_VICT );
    do_look( victim, (wchar_t*)"auto" );
    return;
}



void spell_teleport( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    ROOM_INDEX_DATA *pRoomIndex;

    if (!IS_NPC(ch)) victim = ch;

    if ( victim->in_room == NULL
    ||   IS_SET(victim->in_room->room_flags, ROOM_NO_RECALL)
    || ( victim != ch && IS_SET(victim->imm_flags,IMM_SUMMON))
    || ( !IS_NPC(ch) && victim->fighting != NULL )
    || ( victim != ch
    && ( saves_spell( level - 5, victim,DAM_OTHER))))
    {
	send_to_char( L"Başaramadın.\n\r", ch );
	return;
    }

    pRoomIndex = get_random_room(victim);

    if (victim != ch)
    send_to_char( L"Nakledildin!\n\r",victim);

      act( L"$n yokoluyor!", victim, NULL, NULL, TO_ROOM );
      char_from_room( victim );
      char_to_room( victim, pRoomIndex );
      act( L"$n yavaşça kayboluyor.", victim, NULL, NULL, TO_ROOM );
    do_look( victim, (wchar_t*)"auto" );
    return;
}



void spell_ventriloquate( int sn, int level, CHAR_DATA *ch,void *vo,int target)
{
    wchar_t buf1[MAX_STRING_LENGTH];
    wchar_t buf2[MAX_STRING_LENGTH];
    wchar_t speaker[MAX_INPUT_LENGTH];
    CHAR_DATA *vch;

    target_name = one_argument( target_name, speaker );

    swprintf( buf1, MAX_STRING_LENGTH-1, L"%s '%s' dedi.\n\r",              speaker, target_name );
    swprintf( buf2, MAX_STRING_LENGTH-1, L"Biri %s'e zorla '%s' dedirtiyor.\n\r", speaker, target_name );
    buf1[0] = towupper(buf1[0]);

    for ( vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room )
    {
	if ( !is_name( speaker, vch->name ) )
	    send_to_char( saves_spell(level,vch,DAM_OTHER) ? buf2 : buf1, vch );
    }

    return;
}



void spell_weaken( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) || saves_spell( level, victim,DAM_OTHER) )
	return;

    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = (4 + level / 12);
    af.location  = APPLY_STR;
    af.modifier  = -1 * (2 + level / 12);
    af.bitvector = AFF_WEAKEN;
    affect_to_char( victim, &af );
    send_to_char( L"Gücünün akıp gittiğini hissediyorsun.\n\r", victim );
    act( L"$n yorgun ve zayıf görünüyor.",victim,NULL,NULL,TO_ROOM);
    return;
}



/* RT recall spell is back */

void spell_word_of_recall( int sn, int level, CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    ROOM_INDEX_DATA *location;
    int to_room_vnum;

    if ((ch->iclass == 9) && (ch->fighting) && (victim == NULL))
	{
    send_to_char( L"Onurun anımsama kullanmana izin vermiyor!.\n\r",ch);
	 return;
	}

    if (victim != NULL)
    {
        if  ((victim->fighting) && (victim->iclass == 9))
	{
    send_to_char( L"Bu büyüyü onuruyla dövüşen bir samuraya yapamazsın!.\n\r",ch);
	 return;
	}
    }

    if (IS_NPC(victim))
      return;
   to_room_vnum =
hometown_table[victim->hometown].recall[IS_GOOD(victim)?0:IS_NEUTRAL(victim)?1:IS_EVIL(victim)?2:1];

    if ((location = get_room_index( to_room_vnum )) == NULL)
    {
      send_to_char( L"Tamamen kayboldun.\n\r",victim);
	return;
    }
/*
    if (victim->desc != NULL &&
	(current_time - victim->last_fight_time) < FIGHT_DELAY_TIME)
      {
	send_to_char( L"You are too pumped to pray now.\n\r",victim);
	return;
      }
*/
    if (IS_SET(victim->in_room->room_flags,ROOM_NO_RECALL) ||
	IS_AFFECTED(victim,AFF_CURSE) ||
	IS_RAFFECTED(victim->in_room,AFF_ROOM_CURSE))
    {
	send_to_char( L"Büyü işe yaramadı.\n\r",victim);
	return;
    }

    if (victim->fighting != NULL)
    {
      if (victim == ch)  gain_exp(victim,0 -(victim->level + 25) );
	stop_fighting(victim,TRUE);
    }

    ch->move /= 2;
    act( L"$n yokoluyor.",victim,NULL,NULL,TO_ROOM);
    char_from_room(victim);
    char_to_room(victim,location);
    do_visible(ch,(wchar_t*)"");
    act( L"$n beliriyor.",victim,NULL,NULL,TO_ROOM);
    do_look(victim,(wchar_t*)"auto");

    if (victim->pet != NULL)
      {
 	char_from_room( victim->pet );
	char_to_room( victim->pet, location );
	do_look(victim->pet, (wchar_t*)"auto" );
      }

}

/*
 * Draconian spells.
 */
void spell_acid_breath( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam,hp_dam,dice_dam,hpch;

    act( L"$n $E asit tükürüyor.",ch,NULL,victim,TO_NOTVICT);
    act( L"$n sana doğru yakıcı asit tükürüyor.",ch,NULL,victim,TO_VICT);
    act( L"$N asit tükürüyorsun.",ch,NULL,victim,TO_CHAR);

    hpch = UMAX(12,ch->hit);
    hp_dam = number_range(hpch/11 + 1, hpch/6);
    dice_dam = dice(level,16);

    dam = UMAX(hp_dam + dice_dam/10,dice_dam + hp_dam/10);

    if (saves_spell(level,victim,DAM_ACID))
    {
	acid_effect(victim,level/2,dam/4,TARGET_CHAR);
	damage(ch,victim,dam/2,sn,DAM_ACID,TRUE);
    }
    else
    {
	acid_effect(victim,level,dam,TARGET_CHAR);
	damage(ch,victim,dam,sn,DAM_ACID,TRUE);
    }
}



void spell_fire_breath( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    CHAR_DATA *vch, *vch_next;
    int dam,hp_dam,dice_dam;
    int hpch;

    act( L"$n ateşten bir nefes gönderiyor.",ch,NULL,victim,TO_NOTVICT);
    act( L"$n üzerine ateşten bir nefes gönderiyor!",ch,NULL,victim,TO_VICT);
    act( L"Ateşten bir nefes gönderiyorsun.",ch,NULL,NULL,TO_CHAR);

    hpch = UMAX( 10, ch->hit );
    hp_dam  = number_range( hpch/9+1, hpch/5 );
    dice_dam = dice(level,20);

    dam = UMAX(hp_dam + dice_dam /10, dice_dam + hp_dam / 10);
    fire_effect(victim->in_room,level,dam/2,TARGET_ROOM);

    for (vch = victim->in_room->people; vch != NULL; vch = vch_next)
    {
	vch_next = vch->next_in_room;

	if (is_safe_spell(ch,vch,TRUE)
	||  ( IS_NPC(vch) && IS_NPC(ch)
	&&  (ch->fighting != vch /*|| vch->fighting != ch */)))
	    continue;
	if ( is_safe(ch, vch) )
          continue;

	if (vch == victim) /* full damage */
	{
	    if (saves_spell(level,vch,DAM_FIRE))
	    {
		fire_effect(vch,level/2,dam/4,TARGET_CHAR);
		damage(ch,vch,dam/2,sn,DAM_FIRE,TRUE);
	    }
	    else
	    {
		fire_effect(vch,level,dam,TARGET_CHAR);
		damage(ch,vch,dam,sn,DAM_FIRE,TRUE);
	    }
	}
	else /* partial damage */
	{
	    if (saves_spell(level - 2,vch,DAM_FIRE))
	    {
		fire_effect(vch,level/4,dam/8,TARGET_CHAR);
		damage(ch,vch,dam/4,sn,DAM_FIRE,TRUE);
	    }
	    else
	    {
		fire_effect(vch,level/2,dam/4,TARGET_CHAR);
		damage(ch,vch,dam/2,sn,DAM_FIRE,TRUE);
	    }
	}
    }
}

void spell_frost_breath( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    CHAR_DATA *vch, *vch_next;
    int dam,hp_dam,dice_dam, hpch;

    act( L"$n buzdan bir nefes gönderiyor!",ch,NULL,victim,TO_NOTVICT);
    act( L"$n üzerine buzdan bir nefes gönderiyor!",
	ch,NULL,victim,TO_VICT);
    act( L"Buzdan bir nefes gönderiyorsun.",ch,NULL,NULL,TO_CHAR);

    hpch = UMAX(12,ch->hit);
    hp_dam = number_range(hpch/11 + 1, hpch/6);
    dice_dam = dice(level,16);

    dam = UMAX(hp_dam + dice_dam/10,dice_dam + hp_dam/10);
    cold_effect(victim->in_room,level,dam/2,TARGET_ROOM);

    for (vch = victim->in_room->people; vch != NULL; vch = vch_next)
    {
	vch_next = vch->next_in_room;

	if (is_safe_spell(ch,vch,TRUE)
	||  (IS_NPC(vch) && IS_NPC(ch)
	&&   (ch->fighting != vch /*|| vch->fighting != ch*/)))
	    continue;
	if ( is_safe(ch, vch) )
          continue;


	if (vch == victim) /* full damage */
	{
	    if (saves_spell(level,vch,DAM_COLD))
	    {
		cold_effect(vch,level/2,dam/4,TARGET_CHAR);
		damage(ch,vch,dam/2,sn,DAM_COLD,TRUE);
	    }
	    else
	    {
		cold_effect(vch,level,dam,TARGET_CHAR);
		damage(ch,vch,dam,sn,DAM_COLD,TRUE);
	    }
	}
	else
	{
	    if (saves_spell(level - 2,vch,DAM_COLD))
	    {
		cold_effect(vch,level/4,dam/8,TARGET_CHAR);
		damage(ch,vch,dam/4,sn,DAM_COLD,TRUE);
	    }
	    else
	    {
		cold_effect(vch,level/2,dam/4,TARGET_CHAR);
		damage(ch,vch,dam/2,sn,DAM_COLD,TRUE);
	    }
	}
    }
}


void spell_gas_breath( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    wchar_t buf[MAX_STRING_LENGTH];
    int dam,hp_dam,dice_dam,hpch;

    act( L"$n zehirli bir nefes gönderiyor!",ch,NULL,NULL,TO_ROOM);
    act( L"Zehirli bir nefes gönderiyorsun.",ch,NULL,NULL,TO_CHAR);

    hpch = UMAX(16,ch->hit);
    hp_dam = number_range(hpch/15+1,8);
    dice_dam = dice(level,12);

    dam = UMAX(hp_dam + dice_dam/10,dice_dam + hp_dam/10);
    poison_effect(ch->in_room,level,dam,TARGET_ROOM);

    for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
    {
	vch_next = vch->next_in_room;

	if (is_safe_spell(ch,vch,TRUE)
	||  (IS_NPC(ch) && IS_NPC(vch)
	&&   (ch->fighting == vch || vch->fighting == ch)))
	    continue;
	if ( is_safe(ch, vch) )
          continue;
          if (!IS_NPC(ch) && vch != ch &&
              ch->fighting != vch && vch->fighting != ch &&
              (IS_SET(vch->affected_by,AFF_CHARM) || !IS_NPC(vch)))
            {
            if (!can_see(vch, ch))
                do_yell(vch, (wchar_t*)"İmdat! Biri bana saldırıyor!");
            else
              {
                 swprintf( buf, MAX_STRING_LENGTH-1, L"Geber %s, seni büyücü köpek!",
                    (is_affected(ch,gsn_doppelganger)&&!IS_IMMORTAL(vch))?
                     ch->doppel->name : ch->name);
                 do_yell(vch,buf);
              }
          }

	if (saves_spell(level,vch,DAM_POISON))
	{
	    poison_effect(vch,level/2,dam/4,TARGET_CHAR);
	    damage(ch,vch,dam/2,sn,DAM_POISON,TRUE);
	}
	else
	{
	    poison_effect(vch,level,dam,TARGET_CHAR);
	    damage(ch,vch,dam,sn,DAM_POISON,TRUE);
	}
    }
}

void spell_lightning_breath(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam,hp_dam,dice_dam,hpch;

    act( L"$n $E yıldırım yüklü bir nefes gönderiyor.",ch,NULL,victim,TO_NOTVICT);
    act( L"$n üzerine yıldırım yüklü bir nefes gönderiyor!",ch,NULL,victim,TO_VICT);
    act( L"$E yıldırım yüklü bir nefes gönderiyorsun.",ch,NULL,victim,TO_CHAR);

    hpch = UMAX(10,ch->hit);
    hp_dam = number_range(hpch/9+1,hpch/5);
    dice_dam = dice(level,20);

    dam = UMAX(hp_dam + dice_dam/10,dice_dam + hp_dam/10);

    if (CAN_DETECT(victim, ADET_GROUNDING))
    {
      send_to_char( L"Elektiriğin gücü düşmanında yitiyor.\n\r",victim);
    	act( L"Bir yıldırım topu $S düşmanında yitiyor.\n\r",
		ch, NULL, victim, TO_ROOM);
	return;
    }

    if (saves_spell(level,victim,DAM_LIGHTNING))
    {
	shock_effect(victim,level/2,dam/4,TARGET_CHAR);
	damage(ch,victim,dam/2,sn,DAM_LIGHTNING,TRUE);
    }
    else
    {
	shock_effect(victim,level,dam,TARGET_CHAR);
	damage(ch,victim,dam,sn,DAM_LIGHTNING,TRUE);
    }
}

/*
 * Spells for mega1.are from Glop/Erkenbrand.
 */
void spell_general_purpose(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = number_range( 25, 100 );
    if ( saves_spell( level, victim, DAM_PIERCE) )
        dam /= 2;
    damage( ch, victim, dam, sn, DAM_PIERCE ,TRUE);
    return;
}

void spell_high_explosive(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = number_range( 30, 120 );
    if ( saves_spell( level, victim, DAM_PIERCE) )
        dam /= 2;
    damage( ch, victim, dam, sn, DAM_PIERCE ,TRUE);
    return;
}



void spell_find_object( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    wchar_t buf[MAX_INPUT_LENGTH];
    BUFFER *buffer;
    OBJ_DATA *obj;
    OBJ_DATA *in_obj;
    bool found;
    int number = 0, max_found;

    found = FALSE;
    number = 0;
    max_found = IS_IMMORTAL(ch) ? 200 : 2 * level;

    buffer = new_buf();

    for ( obj = object_list; obj != NULL; obj = obj->next )
    {
	if ( !can_see_obj( ch, obj ) || !is_name( target_name, obj->name )
		|| number_percent() > 2 * level
		||   ch->level < obj->level)
	    continue;

	found = TRUE;
	number++;

	for ( in_obj = obj; in_obj->in_obj != NULL; in_obj = in_obj->in_obj )
	    ;

	if ( in_obj->carried_by != NULL && can_see(ch,in_obj->carried_by))
	{
    swprintf( buf, MAX_STRING_LENGTH-1, L"bit tanesi %s tarafından taşınıyor\n\r",
		PERS(in_obj->carried_by, ch) );
	}
	else
	{
	    if (IS_IMMORTAL(ch) && in_obj->in_room != NULL)
      swprintf( buf, MAX_STRING_LENGTH-1, L"bir tanesi %s odasında [Oda %d]\n\r",
		    in_obj->in_room->name, in_obj->in_room->vnum);
	    else
      swprintf( buf, MAX_STRING_LENGTH-1, L"bir tanesi %s odasında\n\r",
		    in_obj->in_room == NULL
			? L"[bilinmeyen]" : in_obj->in_room->name );
	}

	buf[0] = towupper(buf[0]);
	add_buf(buffer,buf);

	if (number >= max_found)
	    break;
    }

    if ( !found )
    send_to_char( L"Ne bu dünyada, ne de diğerinde böyle bir şey yok.\n\r", ch );
    else
	page_to_char(buf_string(buffer),ch);

    free_buf(buffer);

    return;
}

void spell_lightning_shield(int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    AFFECT_DATA af,af2;

    if ( is_affected_room( ch->in_room, sn ))
    {
      send_to_char( L"Bu oda zaten kalkanlanmış.\n\r",ch);
	return;
    }

    if ( is_affected(ch,sn))
    {
      send_to_char( L"Bu büyü yakın zamanda kullanıldı.\n\r",ch);
	return;
    }

    af.where     = TO_ROOM_AFFECTS;
    af.type      = sn;
    af.level     = ch->level;
    af.duration  = level / 40;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_ROOM_L_SHIELD;
    affect_to_room( ch->in_room, &af );

    af2.where     = TO_AFFECTS;
    af2.type      = sn;
    af2.level	 = ch->level;
    af2.duration  = level / 10;
    af2.modifier  = 0;
    af2.location  = APPLY_NONE;
    af2.bitvector = 0;
    affect_to_char( ch, &af2 );

    ch->in_room->owner = str_dup( ch->name );
    send_to_char( L"Oda şimşeklerle dolmaya başlıyor.\n\r", ch );
    act( L"Oda $s şimşekleriyle dolmaya başlıyor.",ch,NULL,NULL,TO_ROOM);
    return;
}

void spell_shocking_trap(int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    AFFECT_DATA af,af2;

    if ( is_affected_room( ch->in_room, sn ))
    {
      send_to_char( L"Bu odaya yerleştirilmiş şok dalgaları zaten var.\n\r",ch);
	return;
    }

    if ( is_affected(ch,sn))
    {
      send_to_char( L"Bu büyü yakın zamanda kullanıldı.\n\r",ch);
	return;
    }

    af.where     = TO_ROOM_AFFECTS;
    af.type      = sn;
    af.level     = ch->level;
    af.duration  = level / 40;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_ROOM_SHOCKING;
    affect_to_room( ch->in_room, &af );

    af2.where     = TO_AFFECTS;
    af2.type      = sn;
    af2.level	 = level;
    af2.duration  = ch->level / 10;
    af2.modifier  = 0;
    af2.location  = APPLY_NONE;
    af2.bitvector = 0;
    affect_to_char( ch, &af2 );
    send_to_char( L"Oda şok dalgalarıyla dolmaya başlıyor.\n\r", ch );
    act( L"Oda $s şok dalgalarıyla dolmaya başlıyor.",ch,NULL,NULL,TO_ROOM);
    return;
}

void spell_acid_arrow( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = dice( level, 12 );
    if ( saves_spell( level, victim, DAM_ACID ) )
	dam /= 2;
    damage( ch, victim, dam, sn,DAM_ACID,TRUE);
    return;
}


/* energy spells */
void spell_etheral_fist( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = dice( level, 12 );
    if ( saves_spell( level, victim, DAM_ENERGY ) )
	dam /= 2;
  act( L"Başka bir dünyaya ait kara bir yumruk $E atılıyor ve onu hareketsiz bırakıyor!"
  ,ch,NULL,victim,TO_NOTVICT);
    damage( ch, victim, dam, sn,DAM_ENERGY,TRUE);
    return;
}

void spell_spectral_furor( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = dice( level, 8 );
    if ( saves_spell( level, victim, DAM_ENERGY ) )
	dam /= 2;
  act( L"Evrenin özü $N için endişe duyuyor!",
		ch,NULL,victim,TO_NOTVICT);
    damage( ch, victim, dam, sn,DAM_ENERGY,TRUE);
    return;
}

void spell_disruption( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = dice( level, 9 );
    if ( saves_spell( level, victim, DAM_ENERGY ) )
	dam /= 2;
  act( L"Tuhaf bir enerji $M kuşatırken varlığı belirsizleşiyor.",
		ch,NULL,victim,TO_NOTVICT);
    damage( ch, victim, dam, sn,DAM_ENERGY,TRUE);
    return;
}


void spell_sonic_resonance( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = dice( level, 7 );
    if ( saves_spell( level, victim, DAM_ENERGY ) )
	dam /= 2;
  act( L"Bir hareket enerjisi silindiri $N çevrelerken çınlamasına neden oluyor.",
		ch,NULL,victim,TO_NOTVICT);
    damage( ch, victim, dam, sn,DAM_ENERGY,TRUE);
    WAIT_STATE( victim, skill_table[sn].beats );
    return;
}
/* mental */
void spell_mind_wrack( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = dice( level, 7 );
    if ( saves_spell( level, victim, DAM_MENTAL ) )
	dam /= 2;
  act( L"$n bakışlarıyla $M uyuşuklaştırıyor.",
		ch,NULL,victim,TO_NOTVICT);
    damage( ch, victim, dam, sn,DAM_MENTAL,TRUE);
    return;
}

void spell_mind_wrench( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = dice( level, 9 );
    if ( saves_spell( level, victim, DAM_MENTAL ) )
	dam /= 2;
  act( L"$n bakışlarıyla $M hiperaktifleştiriyor.",
		ch,NULL,victim,TO_NOTVICT);
    damage( ch, victim, dam, sn,DAM_MENTAL,TRUE);
    return;
}
/* acid */
void spell_sulfurus_spray( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = dice( level, 7 );
    if ( saves_spell( level, victim, DAM_ACID ) )
	dam /= 2;
  act( L"Yakıcı kokusuyla bir sülfür yağmuru $S üzerine yağıyor.",
		ch,NULL,victim,TO_NOTVICT);
    damage( ch, victim, dam, sn,DAM_ACID,TRUE);
    return;
}

void spell_caustic_font( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = dice( level, 9 );
    if ( saves_spell( level, victim, DAM_ACID ) )
	dam /= 2;
  act( L"Kaynaktan gelen yakıcı sıvı $S altında birikerek dokusunu eritiyor! ",
		ch,NULL,victim,TO_NOTVICT);
    damage( ch, victim, dam, sn,DAM_ACID,TRUE);
    return;
}

void spell_acetum_primus( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = dice( level, 8 );
    if ( saves_spell( level, victim, DAM_ACID ) )
	dam /= 2;
  act( L"$S üzerine örtülen asit pelerini dokunduğu her bölgeyi yakıyor. ",
		ch,NULL,victim,TO_NOTVICT);
    damage( ch, victim, dam, sn,DAM_ACID,TRUE);
    return;
}


/*  Electrical  */

void spell_galvanic_whip( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = dice( level, 7 );
    if ( saves_spell( level, victim, DAM_LIGHTNING ) )
	dam /= 2;
  act( L"$n atom parçacıklarından oluşturduğu kırbaçla $E vuruyor.",
		ch,NULL,victim,TO_NOTVICT);
    damage( ch, victim, dam, sn,DAM_LIGHTNING,TRUE);
    return;
}


void spell_magnetic_trust( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = dice( level, 8 );
    if ( saves_spell( level, victim, DAM_LIGHTNING ) )
	dam /= 2;
  act( L"Bilinmeyen bir enerji ile saçların dikiliyor!",
		ch,NULL,victim,TO_NOTVICT);
    damage( ch, victim, dam, sn,DAM_LIGHTNING,TRUE);
    return;
}

void spell_quantum_spike( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = dice( level, 9 );
    if ( saves_spell( level, victim, DAM_LIGHTNING ) )
	dam /= 2;
  act( L"$N küçük ayrık parçalara ayrılıyor ve sonra tekrar birleşiyor.",
		ch,NULL,victim,TO_NOTVICT);
    damage( ch, victim, dam, sn,DAM_LIGHTNING,TRUE);
    return;
}

/* negative */
void spell_hand_of_undead( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    if ( saves_spell( level, victim,DAM_NEGATIVE) )
    {
      send_to_char( L"Bir anlık soğukluk hissediyorsun.\n\r",victim);
	return;
    }

    if ( (IS_NPC(victim) && IS_SET(victim->act,ACT_UNDEAD))
	|| IS_VAMPIRE(victim) )
	{
    send_to_char( L"Kurbanın hortlak elinden etkilenmiyor.\n\r",ch);
	 return;
	}
    if ( victim->level <= 2 )
    {
	dam		 = ch->hit + 1;
    }
    else
    {
     dam = dice( level, 10 );
     victim->mana	/= 2;
     victim->move	/= 2;
     ch->hit		+= dam / 2;
    }

    send_to_char( L"Yaşamının aktığını hissediyorsun!\n\r",victim);
    act( L"$N akıl almaz bir hortlak eli tarafından yakalandı!",
		ch,NULL,victim,TO_NOTVICT);
    damage( ch, victim, dam, sn,DAM_NEGATIVE,TRUE);
    return;
}


/* travel via astral plains */
void spell_astral_walk( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim;
    bool gate_pet;
    wchar_t buf[512];


    if ( ( victim = get_char_world( ch, target_name ) ) == NULL
    ||   victim == ch
    ||   victim->in_room == NULL
    ||   !can_see_room(ch,victim->in_room)
    ||   IS_SET(victim->in_room->room_flags, ROOM_SAFE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_PRIVATE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_SOLITARY)
    ||   IS_SET(ch->in_room->room_flags, ROOM_NOSUMMON)
    ||   IS_SET(victim->in_room->room_flags, ROOM_NOSUMMON)
    ||   victim->level >= level + 3
/*    ||   (!IS_NPC(victim) && victim->level >= LEVEL_HERO)  * NOT trust */
    ||   saves_spell(level,victim,DAM_OTHER)
    ||   (IS_NPC(victim) && is_safe_nomessage(ch, victim) && IS_SET(victim->imm_flags,IMM_SUMMON))
    ||   (!IS_NPC(victim) && is_safe_nomessage(ch, victim) && IS_SET(victim->act,PLR_NOSUMMON))
    ||   (!IS_NPC(victim) && ch->in_room->area != victim->in_room->area )
    ||   (IS_NPC(victim) && (victim->pIndexData->vnum == ch->pcdata->questmob))
    ||   (IS_NPC(victim) && saves_spell( level, victim,DAM_OTHER) ) )
    {
	send_to_char( L"Başaramadın.\n\r", ch );
	return;
    }
    if (ch->pet != NULL && ch->in_room == ch->pet->in_room)
	gate_pet = TRUE;
    else
	gate_pet = FALSE;


  act( L"$n bir ışık parlamasıyla yokoluyor!",ch,NULL,NULL,TO_ROOM);
  swprintf( buf, MAX_STRING_LENGTH-1, L"Bir yıldız yolculuğuyla %s'e gidiyorsun.\n\r",victim->name);
    send_to_char(buf,ch);
    char_from_room(ch);
    char_to_room(ch,victim->in_room);

    act( L"$n bir ışık parlamasıyla beliriyor!",ch,NULL,NULL,TO_ROOM);
    do_look(ch,(wchar_t*)"auto");

    if (gate_pet)
    {
      act( L"$n bir ışık parlamasıyla yokoluyor!",ch->pet,NULL,NULL,TO_ROOM);
	send_to_char(buf,ch->pet);
	char_from_room(ch->pet);
	char_to_room(ch->pet,victim->in_room);
  act( L"$n bir ışık parlamasıyla beliriyor!",ch->pet,NULL,NULL,TO_ROOM);
	do_look(ch->pet,(wchar_t*)"auto");
    }
}


/* vampire version astral walk */
void spell_mist_walk( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim;


    if ( ( victim = get_char_world( ch, target_name ) ) == NULL
    ||   victim == ch
/*    ||   !IS_VAMPIRE(ch) Karakam da bu buyuye sahip */
    ||   victim->in_room == NULL
    ||   !can_see_room(ch,victim->in_room)
    ||   IS_SET(victim->in_room->room_flags, ROOM_SAFE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_PRIVATE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_SOLITARY)
    ||   IS_SET(ch->in_room->room_flags, ROOM_NOSUMMON)
    ||   IS_SET(victim->in_room->room_flags, ROOM_NOSUMMON)
    ||   victim->level >= level + 3
/*    ||   (!IS_NPC(victim) && victim->level >= LEVEL_HERO)  * NOT trust */
    ||   saves_spell(level,victim,DAM_OTHER)
    ||   (IS_NPC(victim) && is_safe_nomessage(ch, victim) && IS_SET(victim->imm_flags,IMM_SUMMON))
    ||   (!IS_NPC(victim) && is_safe_nomessage(ch, victim) && IS_SET(victim->act,PLR_NOSUMMON))
    ||   (IS_NPC(victim) && (victim->pIndexData->vnum == ch->pcdata->questmob))
    ||   (!IS_NPC(victim) && ch->in_room->area != victim->in_room->area )
    ||   (IS_NPC(victim) && saves_spell( level, victim,DAM_OTHER) ) )
    {
	send_to_char( L"Başaramadın.\n\r", ch );
	return;
    }

    act( L"$n gizemli bir buluta girerek yokoluyor!",ch,NULL,NULL,TO_ROOM);
    send_to_char( L"Gizemli bir buluta girerek hedefine akıyorsun!.\n\r",ch);

    char_from_room(ch);
    char_to_room(ch,victim->in_room);

    act( L"Parlayan sis bulutu sizi içine çekiyor, sonra $n'i açığa çıkarmak için geri çekiliyor!",ch,NULL,NULL,TO_ROOM);
    do_look(ch,(wchar_t*)"auto");

}

/*  Cleric version of astra_walk  */
void spell_solar_flight( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim;


    if  (time_info.hour > 18 || time_info.hour < 8)
	{
    send_to_char( L"Güneş uçuşu için gün ışığına ihtiyacın var.\n\r",ch);
	 return;
	}

    if ( ( victim = get_char_world( ch, target_name ) ) == NULL
    ||   victim == ch
    ||   victim->in_room == NULL
    ||   !can_see_room(ch,victim->in_room)
    ||   IS_SET(victim->in_room->room_flags, ROOM_SAFE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_PRIVATE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_SOLITARY)
    ||   IS_SET(ch->in_room->room_flags, ROOM_NOSUMMON)
    ||   IS_SET(victim->in_room->room_flags, ROOM_NOSUMMON)
    ||   victim->level >= level + 1
/*    ||   (!IS_NPC(victim) && victim->level >= LEVEL_HERO)  * NOT trust */
    ||   saves_spell(level,victim,DAM_OTHER)
    ||   (IS_NPC(victim) && is_safe_nomessage(ch, victim) && IS_SET(victim->imm_flags,IMM_SUMMON))
    ||   (!IS_NPC(victim) && is_safe_nomessage(ch, victim) && IS_SET(victim->act,PLR_NOSUMMON))
    ||   (IS_NPC(victim) && (victim->pIndexData->vnum == ch->pcdata->questmob))
    ||   (!IS_NPC(victim) && ch->in_room->area != victim->in_room->area )
    ||   (IS_NPC(victim) && saves_spell( level, victim,DAM_OTHER) ) )
    {
	send_to_char( L"Başaramadın.\n\r", ch );
	return;
    }

    act( L"$n kör edici bir ışık parlamasıyla yokoluyor!",ch,NULL,NULL,TO_ROOM);
    send_to_char( L"Kör edici bir ışık parlamasına karışıyorsun!.\n\r",ch);

    char_from_room(ch);
    char_to_room(ch,victim->in_room);

    act( L"$n kör edici bir ışık parlamasıyla beliriyor!",ch,NULL,NULL,TO_ROOM);
    do_look(ch,(wchar_t*)"auto");

}



/* travel via astral plains */
void spell_helical_flow( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim;


    if ( ( victim = get_char_world( ch, target_name ) ) == NULL
    ||   victim == ch
    ||   victim->in_room == NULL
    ||   !can_see_room(ch,victim->in_room)
    ||   IS_SET(victim->in_room->room_flags, ROOM_SAFE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_PRIVATE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_SOLITARY)
    ||   IS_SET(ch->in_room->room_flags, ROOM_NOSUMMON)
    ||   IS_SET(victim->in_room->room_flags, ROOM_NOSUMMON)
    ||   victim->level >= level + 3
/*    ||   (!IS_NPC(victim) && victim->level >= LEVEL_HERO)  * NOT trust */
    ||   saves_spell(level,victim,DAM_OTHER)
    ||   (IS_NPC(victim) && is_safe_nomessage(ch, victim) && IS_SET(victim->imm_flags,IMM_SUMMON))
    ||   (!IS_NPC(victim) && is_safe_nomessage(ch, victim) && IS_SET(victim->act,PLR_NOSUMMON))
    ||   (IS_NPC(victim) && (victim->pIndexData->vnum == ch->pcdata->questmob))
    ||   (!IS_NPC(victim) && ch->in_room->area != victim->in_room->area )
    ||   (IS_NPC(victim) && saves_spell( level, victim,DAM_OTHER) ) )
    {
	send_to_char( L"Başaramadın.\n\r", ch );
	return;
    }

    act( L"$n yükselen bir renk sarmalına dolanarak havaya karışıyor.",ch,NULL,NULL,TO_ROOM);
    send_to_char( L"Yükselen bir renk sarmalına dolanarak havaya karışıyor.\n\r",ch);

    char_from_room(ch);
    char_to_room(ch,victim->in_room);

    act( L"Bir renk bobini yukarıdan aşağıya iniyor, dağıldığı gibi $n ortaya çıkıyor.",ch,NULL,NULL,TO_ROOM);
    do_look(ch,(wchar_t*)"auto");

}



void spell_corruption( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if (IS_AFFECTED(victim,AFF_CORRUPTION))
	{
    act( L"$N zaten çürüyor.\n\r",ch,NULL,victim,TO_CHAR);
	 return;
	}

    if (saves_spell(level,victim,DAM_NEGATIVE) ||
	(IS_NPC(victim) && IS_SET(victim->act,ACT_UNDEAD)))
    {
	if (ch == victim)
  send_to_char( L"Bir an kendini hasta hissediyorsun, fakat sonra geçiyor.\n\r",ch);
else
  act( L"$N etkilenmiş görünmüyor.",ch,NULL,victim,TO_CHAR);
	return;
    }

    af.where     = TO_AFFECTS;
    af.type 	 = sn;
    af.level	 = level * 3/4;
    af.duration  = (10 + level / 5);
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_CORRUPTION;
    affect_join(victim,&af);

    send_to_char(L"Çürümeye başlayınca acı içinde çığlık atıyorsun.\n\r",victim);
  act( L"$n çürümeye başlayınca acı içinde çığlık atıyor.",
	victim,NULL,NULL,TO_ROOM);
}


void spell_hurricane(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    wchar_t buf[MAX_STRING_LENGTH];
    int dam,hp_dam,dice_dam,hpch;

    act( L"$n yıldırım tanrılarından yardım istiyor.",ch,NULL,NULL,TO_NOTVICT);
    act( L"Yıldırım tanrılarından yardım istiyorsun.",ch,NULL,NULL,TO_CHAR);

    hpch = UMAX(16,ch->hit);
    hp_dam = number_range(hpch/15+1,8);
    dice_dam = dice(level,12);

    dam = UMAX(hp_dam + dice_dam/10,dice_dam + hp_dam/10);

    for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
    {
	vch_next = vch->next_in_room;

	if (is_safe_spell(ch,vch,TRUE)
	||  (IS_NPC(ch) && IS_NPC(vch)
	&&   (ch->fighting == vch || vch->fighting == ch)))
	    continue;
	if ( is_safe(ch, vch) )
          continue;
          if (!IS_NPC(ch) && vch != ch &&
              ch->fighting != vch && vch->fighting != ch &&
              (IS_SET(vch->affected_by,AFF_CHARM) || !IS_NPC(vch)))
            {
            if (!can_see(vch, ch))
                do_yell(vch, (wchar_t*)"İmdat! Biri bana saldırıyor!");
            else
              {
                 swprintf( buf, MAX_STRING_LENGTH-1, L"Geber %s, seni büyücü köpek!",
                    (is_affected(ch,gsn_doppelganger)&&!IS_IMMORTAL(vch))?
                     ch->doppel->name : ch->name);
                 do_yell(vch,buf);
              }
          }

	if (!IS_AFFECTED(vch,AFF_FLYING)) dam /= 2;

	if (vch->size == SIZE_TINY)  dam = (int)((float)dam * 1.5);
	else if (vch->size == SIZE_SMALL)  dam = (int)((float)dam * 1.3);
	else if (vch->size == SIZE_MEDIUM)  dam *= 1;
	else if (vch->size == SIZE_LARGE)  dam = (int)((float)dam * 0.9);
	else if (vch->size == SIZE_HUGE)  dam = (int)((float)dam * 0.7);
	else dam = (int)((float)dam * 0.5);

	if (saves_spell(level,vch,DAM_OTHER))
	    damage(ch,vch,dam/2,sn,DAM_OTHER,TRUE);
	else
	    damage(ch,vch,dam,sn,DAM_OTHER,TRUE);
    }
}


void spell_detect_undead( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( CAN_DETECT(victim, DETECT_UNDEAD) )
    {
	if (victim == ch)
  send_to_char( L"Zaten hortlakları saptıyorsun.\n\r",ch);
else
  act( L"$N zaten hortlakları saptıyor.",ch,NULL,victim,TO_CHAR);
	return;
    }

    af.where     = TO_DETECTS;
    af.type      = sn;
    af.level	 = level;
    af.duration  = (5 + level / 3);
    af.modifier  = 0;
    af.location  = APPLY_NONE;
    af.bitvector = DETECT_UNDEAD;
    affect_to_char( victim, &af );
    send_to_char( L"Gözlerin yanıyor.\n\r", victim );
    if ( ch != victim )
	send_to_char( L"Tamam.\n\r", ch );
    return;
}



void spell_take_revenge( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
 OBJ_DATA *obj;
 OBJ_DATA *in_obj;
 ROOM_INDEX_DATA *room = NULL;
 bool found = FALSE;

  if (IS_NPC(ch)
	|| ch->last_death_time == -1
	|| current_time - ch->last_death_time > 600)
   {
     send_to_char( L"İntikam için çok geç.\n\r",ch);
    return;
   }

 for ( obj = object_list; obj != NULL; obj = obj->next )
    {
	if ( obj->pIndexData->vnum != OBJ_VNUM_CORPSE_PC
		|| !is_name(ch->name,obj->short_descr) )
	    continue;

        found = TRUE;
	for ( in_obj = obj; in_obj->in_obj != NULL; in_obj = in_obj->in_obj )
	    ;

	if ( in_obj->carried_by != NULL )
		room = in_obj->carried_by->in_room;
	else    room = in_obj->in_room;
        break;
    }

    if ( !found || room == NULL)
    send_to_char( L"Maalesef cesedin yokolmuş.\n\r", ch );
    else
      {
	ROOM_INDEX_DATA *prev_room;

	prev_room = ch->in_room;
  act( L"$n nakledilmek için dua ediyor.", ch, NULL, NULL, TO_ROOM);
	char_from_room( ch );
	char_to_room(ch, room);
	if (cabal_area_check( ch )
		|| IS_ROOM_AFFECTED(ch->in_room,AFF_ROOM_PREVENT) )
	{
    send_to_char( L"Cesedine ulaşmayı başaramadın.\n\r", ch);
	  char_from_room( ch );
	  char_to_room(ch, prev_room);
	}
	else do_look(ch,(wchar_t*)"auto");
      }
    return;
}



void spell_firestream( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    act( L"$n kavurucu alevler fırlatıyor.",ch,NULL,victim,TO_NOTVICT);
    act( L"$n üzerine kavurucu alevler fırlatıyor!",ch,NULL,victim,TO_VICT);
    act( L"$S üzerine kavurucu alevler fırlatıyorsun.",ch,NULL,victim,TO_CHAR);

    dam = dice(level, 8);

    if (saves_spell(level,victim,DAM_FIRE))
    {
	fire_effect(victim,level/2,dam/4,TARGET_CHAR);
	damage(ch,victim,dam/2,sn,DAM_FIRE,TRUE);
    }
    else
    {
	fire_effect(victim,level,dam,TARGET_CHAR);
	damage(ch,victim,dam,sn,DAM_FIRE,TRUE);
    }
    return;
}

void spell_summon_earth_elm( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
  CHAR_DATA *gch;
  CHAR_DATA *elm;
  AFFECT_DATA af;
  int i=0;

  if (is_affected(ch,sn))
    {
      send_to_char( L"Başka bir uşak yaratacak güce sahip değilsin.\n\r",ch);
      return;
    }

    send_to_char( L"Bir toprak uşağı yaratmayı denedin.\n\r",ch);
    act( L"$n bir toprak uşağı yaratmayı denedi.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
          ( gch->pIndexData->vnum == MOB_VNUM_ELM_EARTH ) )
        {
          i++;
          if (i > 2)
           {
             send_to_char( L"Daha fazla toprak uşağını kontrol edemezsin!\n\r",ch);
            return;
           }
        }
    }

  if (count_charmed(ch)) return;

  elm = create_mobile( get_mob_index(MOB_VNUM_ELM_EARTH), NULL );


  for (i = 0; i < MAX_STATS; i ++)
       elm->perm_stat[i] = UMIN(25,15 + ch->level/10);

  elm->perm_stat[STAT_STR] += 3;
  elm->perm_stat[STAT_INT] -= 1;
  elm->perm_stat[STAT_CON] += 2;

  elm->max_hit = IS_NPC(ch)? URANGE(ch->max_hit,1 * ch->max_hit,30000)
                : UMIN( (2 * ch->pcdata->perm_hit) + 400,30000);
  elm->hit = elm->max_hit;
  elm->max_mana = IS_NPC(ch)? ch->max_mana : ch->pcdata->perm_mana;
  elm->mana = elm->max_mana;
  elm->level = ch->level;
  for (i=0; i < 3; i++)
    elm->armor[i] = interpolate(elm->level,100,-100);
  elm->armor[3] = interpolate(elm->level,100,0);
  elm->silver = 0;
  elm->timer = 0;
  elm->damage[DICE_NUMBER] = 3;
  elm->damage[DICE_TYPE] = 10;
  elm->damage[DICE_BONUS] = ch->level / 2;

  char_to_room(elm,ch->in_room);
  send_to_char( L"Bir toprak uşağı yarattın!\n\r",ch);
  act( L"$n bir toprak uşağı yarattı!",ch,NULL,NULL,TO_ROOM);

  af.where              = TO_AFFECTS;
  af.type               = sn;
  af.level              = level;
  af.duration           = 24;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);

  SET_BIT(elm->affected_by, AFF_CHARM);
  elm->master = elm->leader = ch;

}

void spell_frostbolt( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = dice( level, 10 );
    if ( saves_spell( level, victim, DAM_COLD ) )
	dam /= 2;
    damage( ch, victim, dam, sn,DAM_COLD,TRUE);
    return;
}

void spell_summon_air_elm( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
  CHAR_DATA *gch;
  CHAR_DATA *elm;
  AFFECT_DATA af;
  int i=0;

  if (is_affected(ch,sn))
    {
      send_to_char( L"Başka bir uşak yaratacak güce sahip değilsin.\n\r",ch);
      return;
    }

    send_to_char( L"Bir hava uşağı yaratmayı denedin.\n\r",ch);
    act( L"$n bir hava uşağı yaratmayı denedi.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
          ( gch->pIndexData->vnum == MOB_VNUM_ELM_AIR ) )
        {
          i++;
          if (i > 2)
           {
             send_to_char( L"Daha fazla hava uşağını kontrol edemezsin!\n\r",ch);
            return;
           }
        }
    }

  if (count_charmed(ch)) return;

  elm = create_mobile( get_mob_index(MOB_VNUM_ELM_AIR), NULL );


  for (i = 0; i < MAX_STATS; i ++)
       elm->perm_stat[i] = UMIN(25,15 + ch->level/10);

  elm->perm_stat[STAT_STR] += 3;
  elm->perm_stat[STAT_INT] -= 1;
  elm->perm_stat[STAT_CON] += 2;

  elm->max_hit = IS_NPC(ch)? URANGE(ch->max_hit,1 * ch->max_hit,30000)
                : UMIN( (4 * ch->pcdata->perm_hit) + 1000,30000);
  elm->hit = elm->max_hit;
  elm->max_mana = IS_NPC(ch)? ch->max_mana : ch->pcdata->perm_mana;
  elm->mana = elm->max_mana;
  elm->level = ch->level;
  for (i=0; i < 3; i++)
    elm->armor[i] = interpolate(elm->level,100,-100);
  elm->armor[3] = interpolate(elm->level,100,0);
  elm->silver = 0;
  elm->timer = 0;
  elm->damage[DICE_NUMBER] = 7;
  elm->damage[DICE_TYPE] = 4;
  elm->damage[DICE_BONUS] = ch->level / 2;

  char_to_room(elm,ch->in_room);
  send_to_char( L"Bir hava uşağı yaratmayı denedin!\n\r",ch);
  act( L"$n bir hava uşağı yaratmayı denedi!",ch,NULL,NULL,TO_ROOM);

  af.where              = TO_AFFECTS;
  af.type               = sn;
  af.level              = level;
  af.duration           = 24;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);

  SET_BIT(elm->affected_by, AFF_CHARM);
  elm->master = elm->leader = ch;

	return;
}

void spell_summon_water_elm( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
  CHAR_DATA *gch;
  CHAR_DATA *elm;
  AFFECT_DATA af;
  int i=0;

  if (is_affected(ch,sn))
    {
      send_to_char( L"Başka bir uşak yaratacak güce sahip değilsin.\n\r",ch);
      return;
    }

    send_to_char( L"Bir su uşağı yaratmayı denedin.\n\r",ch);
    act( L"$n bir su uşağı yaratmayı denedi.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
          ( gch->pIndexData->vnum == MOB_VNUM_ELM_WATER ) )
        {
          i++;
          if (i > 2)
           {
             send_to_char( L"Daha fazla su uşağını kontrol edemezsin!\n\r",ch);
            return;
           }
        }
    }

  if (count_charmed(ch)) return;

  elm = create_mobile( get_mob_index(MOB_VNUM_ELM_WATER), NULL );


  for (i = 0; i < MAX_STATS; i ++)
       elm->perm_stat[i] = UMIN(25,15 + ch->level/10);

  elm->perm_stat[STAT_STR] += 3;
  elm->perm_stat[STAT_INT] -= 1;
  elm->perm_stat[STAT_CON] += 2;

  elm->max_hit = IS_NPC(ch)? URANGE(ch->max_hit,1 * ch->max_hit,30000)
                : UMIN( (5 * ch->pcdata->perm_hit) + 2000,30000);
  elm->hit = elm->max_hit;
  elm->max_mana = IS_NPC(ch)? ch->max_mana : ch->pcdata->perm_mana;
  elm->mana = elm->max_mana;
  elm->level = ch->level;
  for (i=0; i < 3; i++)
    elm->armor[i] = interpolate(elm->level,100,-100);
  elm->armor[3] = interpolate(elm->level,100,0);
  elm->silver = 0;
  elm->timer = 0;
  elm->damage[DICE_NUMBER] = 8;
  elm->damage[DICE_TYPE] = 4;
  elm->damage[DICE_BONUS] = ch->level / 2;

  char_to_room(elm,ch->in_room);
  send_to_char( L"Bir su uşağı yarattın!\n\r",ch);
  act( L"$n bir su uşağı yarattı!",ch,NULL,NULL,TO_ROOM);

  af.where              = TO_AFFECTS;
  af.type               = sn;
  af.level              = level;
  af.duration           = 24;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);

  SET_BIT(elm->affected_by, AFF_CHARM);
  elm->master = elm->leader = ch;

	return;
}

void spell_summon_fire_elm( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
  CHAR_DATA *gch;
  CHAR_DATA *elm;
  AFFECT_DATA af;
  int i=0;

  if (is_affected(ch,sn))
    {
      send_to_char( L"Başka bir uşak yaratacak güce sahip değilsin.\n\r",ch);
      return;
    }

    send_to_char( L"Bir ateş uşağı yaratmayı denedin.\n\r",ch);
    act( L"$n bir ateş uşağı yaratmayı denedi.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
          ( gch->pIndexData->vnum == MOB_VNUM_ELM_FIRE ) )
        {
          i++;
          if (i > 2)
           {
             send_to_char( L"Daha fazla ateş uşağını kontrol edemezsin!\n\r",ch);
            return;
           }
        }
    }

  if (count_charmed(ch)) return;

  elm = create_mobile( get_mob_index(MOB_VNUM_ELM_FIRE), NULL );


  for (i = 0; i < MAX_STATS; i ++)
       elm->perm_stat[i] = UMIN(25,15 + ch->level/10);

  elm->perm_stat[STAT_STR] += 3;
  elm->perm_stat[STAT_INT] -= 1;
  elm->perm_stat[STAT_CON] += 2;

  elm->max_hit = IS_NPC(ch)? URANGE(ch->max_hit,1 * ch->max_hit,30000)
                : UMIN( (10 * ch->pcdata->perm_hit) + 1000,30000);
  elm->hit = elm->max_hit;
  elm->max_mana = IS_NPC(ch)? ch->max_mana : ch->pcdata->perm_mana;
  elm->mana = elm->max_mana;
  elm->level = ch->level;
  for (i=0; i < 3; i++)
    elm->armor[i] = interpolate(elm->level,100,-100);
  elm->armor[3] = interpolate(elm->level,100,0);
  elm->silver = 0;
  elm->timer = 0;
  elm->damage[DICE_NUMBER] = 11;
  elm->damage[DICE_TYPE] = 5;
  elm->damage[DICE_BONUS] = ch->level / 2 + 10;

  char_to_room(elm,ch->in_room);
  send_to_char( L"Bir ateş uşağı yarattın!\n\r",ch);
  act( L"$n bir ateş uşağı yarattı!",ch,NULL,NULL,TO_ROOM);

  af.where              = TO_AFFECTS;
  af.type               = sn;
  af.level              = level;
  af.duration           = 24;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);

  SET_BIT(elm->affected_by, AFF_CHARM);
  elm->master = elm->leader = ch;

	return;
}

void spell_summon_light_elm( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
  CHAR_DATA *gch;
  CHAR_DATA *elm;
  AFFECT_DATA af;
  int i=0;

  if (is_affected(ch,sn))
    {
      send_to_char( L"Başka bir uşak yaratacak güce sahip değilsin.\n\r",ch);
      return;
    }

    send_to_char( L"Bir enerji uşağı yaratmayı denedin.\n\r",ch);
    act( L"$n bir enerji uşağı yaratmayı denedi.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
          ( gch->pIndexData->vnum == MOB_VNUM_ELM_LIGHT ) )
        {
          i++;
          if (i > 2)
           {
             send_to_char( L"Daha fazla enerji uşağını kontrol edemezsin!\n\r",ch);
            return;
           }
        }
    }

  if (count_charmed(ch)) return;

  elm = create_mobile( get_mob_index(MOB_VNUM_ELM_LIGHT), NULL );


  for (i = 0; i < MAX_STATS; i ++)
       elm->perm_stat[i] = UMIN(25,15 + ch->level/10);

  elm->perm_stat[STAT_STR] += 3;
  elm->perm_stat[STAT_INT] -= 1;
  elm->perm_stat[STAT_CON] += 2;

  elm->max_hit = IS_NPC(ch)? URANGE(ch->max_hit,1 * ch->max_hit,30000)
                : UMIN( (10 * ch->pcdata->perm_hit) + 4000,30000);
  elm->hit = elm->max_hit;
  elm->max_mana = IS_NPC(ch)? ch->max_mana : ch->pcdata->perm_mana;
  elm->mana = elm->max_mana;
  elm->level = ch->level;
  for (i=0; i < 3; i++)
    elm->armor[i] = interpolate(elm->level,100,-100);
  elm->armor[3] = interpolate(elm->level,100,0);
  elm->silver = 0;
  elm->timer = 0;
  elm->damage[DICE_NUMBER] = 13;
  elm->damage[DICE_TYPE] = 9;
  elm->damage[DICE_BONUS] = ch->level / 2 + 10;

  char_to_room(elm,ch->in_room);
  send_to_char( L"Bir enerji uşağı yarattın!\n\r",ch);
  act( L"$n bir enerji uşağı yarattı!",ch,NULL,NULL,TO_ROOM);

  af.where              = TO_AFFECTS;
  af.type               = sn;
  af.level              = level;
  af.duration           = 24;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);

  SET_BIT(elm->affected_by, AFF_CHARM);
  elm->master = elm->leader = ch;

	return;
}


void spell_fire_and_ice( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *tmp_vict;
    CHAR_DATA *tmp_next;
    wchar_t buf[MAX_STRING_LENGTH];
    int dam,dam_sn;
    int movedam;

    dam = dice( level , 8);
    movedam = number_range( ch->level, 2 * ch->level );

  for (tmp_vict = ch->in_room->people;tmp_vict != NULL;
       tmp_vict = tmp_next)
    {
      tmp_next = tmp_vict->next_in_room;

      if ( !is_safe_spell(ch,tmp_vict,TRUE))
        {
          if (!IS_NPC(ch) && tmp_vict != ch &&
              ch->fighting != tmp_vict && tmp_vict->fighting != ch &&
              (IS_SET(tmp_vict->affected_by,AFF_CHARM) || !IS_NPC(tmp_vict)))
            {
            if (!can_see(tmp_vict, ch))
                do_yell(tmp_vict, (wchar_t*)"İmdat! Biri bana saldırıyor!");
            else
              {
                 swprintf( buf, MAX_STRING_LENGTH-1, L"Geber %s, seni büyücü köpek!",
	(is_affected(ch,gsn_doppelganger)&&!IS_IMMORTAL(tmp_vict))? ch->doppel->name : ch->name);
                 do_yell(tmp_vict,buf);
              }
          }

          if (saves_spell(level,tmp_vict, DAM_FIRE))
            dam /= 2;
	  if ((dam_sn = skill_lookup( L"fireball")) < 0 )
	    dam_sn = sn;
          damage( ch, tmp_vict, dam, dam_sn, DAM_FIRE, TRUE );
          tmp_vict->move -= UMIN(tmp_vict->move,movedam);

          if ( (IS_NPC(tmp_vict) && tmp_vict->position == POS_DEAD) ||
             (!IS_NPC(tmp_vict) && (current_time - tmp_vict->last_death_time) < 10))
	  {
          if (saves_spell(level,tmp_vict, DAM_COLD))
            dam /= 2;
	  if ((dam_sn = skill_lookup( L"iceball")) < 0 )
	    dam_sn = sn;
          damage( ch, tmp_vict, dam, dam_sn, DAM_COLD, TRUE );
          }

        }
    }
	return;
}

void spell_grounding( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
    {
	if (victim == ch)
  send_to_char( L"Zaten topraklanmışsın.\n\r",ch);
else
  act( L"$N zaten topraklanmış.",ch,NULL,victim,TO_CHAR);
	return;
    }
    af.where	 = TO_DETECTS;
    af.type      = sn;
    af.level	 = level;
    af.duration  = 5 + level / 8;
    af.modifier  = 0;
    af.location  = APPLY_NONE;
    af.bitvector = ADET_GROUNDING;
    affect_to_char( victim, &af );
    send_to_char( L"Vücut elektiriğin topraklanıyor.\n\r", victim );
    if ( ch != victim )
	act( L"$N büyünle topraklanıyor.",ch,NULL,victim,TO_CHAR);
	return;
}

void spell_tsunami( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    if ( (ch->in_room->sector_type != SECT_WATER_SWIM)
        && (ch->in_room->sector_type != SECT_WATER_NOSWIM))
        {
          send_to_char( L"Bir su kaynağına erişemiyorsun.\n\r",ch);
         ch->wait = 0;
         return;
        }

        act( L"Büyük bir su kütlesi havalanarak yumruğa dönüşüyor ve $e vuruyor.",victim,NULL,NULL,TO_ROOM);
        act( L"Büyük bir su kütlesi havalanarak yumruğa dönüşüyor ve sana vuruyor.",victim,NULL,NULL,TO_CHAR);
    dam = dice( level , 16 );
    damage(ch,victim,dam,sn,DAM_DROWNING,TRUE);
}

void spell_disenchant_armor( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    AFFECT_DATA *paf;
    int result, fail;

    if (obj->wear_loc != -1)
    {
      send_to_char( L"Yükseltilecek eşya envanterde taşınıyor olmalı.\n\r",ch);
	return;
    }

    /* find the bonuses */
    fail = 75;
    fail -= (level - obj->level) * 5;
    if (IS_SET(obj->extra_flags, ITEM_MAGIC))
	fail += 25;

    fail = URANGE(5,fail,95);

    result = number_percent();

    /* the moment of truth */
    if (result < (fail / 5))  /* item destroyed */
    {
      act( L"$p kör edici alevlerle buharlaşıyor!",ch,obj,NULL,TO_CHAR);
    	act( L"$p kör edici alevlerle buharlaşıyor!",ch,obj,NULL,TO_ROOM);
	extract_obj(obj);
	return;
    }

    if (result > (fail / 2)) /* item disenchanted */
    {
	AFFECT_DATA *paf_next;

  act( L"$p ışıldayarak yokoluyor.",ch,obj,NULL,TO_CHAR);
	act( L"$p ışıldayarak yokoluyor.",ch,obj,NULL,TO_ROOM);
	obj->enchanted = TRUE;

	/* remove all affects */
	for (paf = obj->affected; paf != NULL; paf = paf_next)
	{
	    paf_next = paf->next;
	    free_affect(paf);
	}
	obj->affected = NULL;

	obj->enchanted = FALSE;

	/* clear some flags */
	REMOVE_BIT(obj->extra_flags, ITEM_GLOW);
	REMOVE_BIT(obj->extra_flags, ITEM_HUM);
	REMOVE_BIT(obj->extra_flags, ITEM_MAGIC);
	REMOVE_BIT(obj->extra_flags, ITEM_INVIS);
	REMOVE_BIT(obj->extra_flags, ITEM_NODROP);
	REMOVE_BIT(obj->extra_flags, ITEM_NOREMOVE);

	return;
    }

    send_to_char( L"Hiçbir şey değişmiyor.\n\r",ch);
    return;
}

void spell_disenchant_weapon( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    AFFECT_DATA *paf;
    int result, fail;

    if (obj->item_type != ITEM_WEAPON)
    {
      send_to_char( L"O bir silah değil.\n\r",ch);
	return;
    }

    if (obj->wear_loc != -1)
    {
      send_to_char( L"Yükseltilecek eşya envanterde taşınıyor olmalı.\n\r",ch);
	return;
    }

    /* find the bonuses */
    fail = 75;
    fail -= (level - obj->level) * 5;
    if (IS_SET(obj->extra_flags, ITEM_MAGIC))
	fail += 25;

    fail = URANGE(5,fail,95);

    result = number_percent();

    /* the moment of truth */
    if (result < (fail / 5))  /* item destroyed */
    {
      act( L"$p şiddetle parlayarak patlıyor!",ch,obj,NULL,TO_CHAR);
    	act( L"$p şiddetle parlayarak patlıyor!",ch,obj,NULL,TO_ROOM);
	extract_obj(obj);
	return;
    }

    if (result > (fail / 2)) /* item disenchanted */
    {
	AFFECT_DATA *paf_next;

  act( L"$p ışıldayarak yokoluyor.",ch,obj,NULL,TO_CHAR);
	act( L"$p ışıldayarak yokoluyor.",ch,obj,NULL,TO_ROOM);
	obj->enchanted = TRUE;

	/* remove all affects */
	for (paf = obj->affected; paf != NULL; paf = paf_next)
	{
	    paf_next = paf->next;
	    free_affect(paf);
	}
	obj->affected = NULL;

	obj->enchanted = FALSE;

	/* clear some flags */
	REMOVE_BIT(obj->extra_flags, ITEM_GLOW);
	REMOVE_BIT(obj->extra_flags, ITEM_HUM);
	REMOVE_BIT(obj->extra_flags, ITEM_MAGIC);
	REMOVE_BIT(obj->extra_flags, ITEM_INVIS);
	REMOVE_BIT(obj->extra_flags, ITEM_NODROP);
	REMOVE_BIT(obj->extra_flags, ITEM_NOREMOVE);

	return;
    }

    send_to_char( L"Hiçbir şey değişmiyor.\n\r",ch);
    return;
}

void spell_absorb( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    AFFECT_DATA af;

    if ( is_affected( ch, sn ) )
    {
      send_to_char( L"Zaten büyüyü soğuruyorsun.\n\r",ch);
	return;
    }
    af.where	 = TO_DETECTS;
    af.type      = gsn_absorb;
    af.level	 = level;
    af.duration  = 3 + level / 10;
    af.modifier  = 0;
    af.location  = APPLY_NONE;
    af.bitvector = ADET_ABSORB;
    affect_to_char( ch, &af );
    send_to_char( L"Vücudun bir enerji alanıyla çevreleniyor.\n\r", ch );
    return;
}

void spell_transfer_object( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
	send_to_char( L"Not implemented!\n\r", ch);
	return;
}

void spell_animate_object( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
  CHAR_DATA *mob;
  OBJ_DATA *obj = (OBJ_DATA *) vo;
  AFFECT_DATA af;
  wchar_t buf[MAX_STRING_LENGTH];
  int i;

  if (!(obj->item_type == ITEM_WEAPON ||
         obj->item_type == ITEM_ARMOR))
  {
    send_to_char( L"Yalnız zırhları ve silahları canlandırabilirsin.\n\r",ch);
        return;
  }

  if (is_affected(ch, sn))
  {
    send_to_char( L"Daha fazla hortlak bedeni kontrol edecek gücü bulamazsın.\n\r", ch);
      return;
  }

  if ( obj->level > level )
  {
    act( L"$p canlandıramayacağın kadar güçlü.", ch, obj, NULL, TO_CHAR);
      return;
  }

  if (count_charmed(ch)) return;

  if (obj->item_type == ITEM_ARMOR
	&& !(CAN_WEAR(obj, ITEM_WEAR_BODY)
	|| CAN_WEAR(obj, ITEM_WEAR_HANDS)
	|| CAN_WEAR(obj, ITEM_WEAR_SHIELD)) )
  {
    send_to_char( L"Başka tip eşyaları canlandırabilirsin.\n\r", ch);
      return;
  }

  if (number_percent() > get_skill(ch, sn))
  {
    act( L"$p şiddetle patlıyor!", ch, obj, NULL, TO_CHAR);
    act( L"$p şiddetle patlıyor!", ch, obj, NULL, TO_ROOM);
      extract_obj( obj );
      return;
  }

  if (obj->item_type == ITEM_WEAPON)
	mob = create_mobile(get_mob_index(MOB_VNUM_WEAPON), NULL);
  else	mob = create_mobile(get_mob_index(MOB_VNUM_ARMOR), NULL);

  swprintf( buf, MAX_STRING_LENGTH-1, L"canlandırılmış %s", obj->name);
  free_string( mob->name );
  mob->name = str_dup( buf );

  swprintf( buf, MAX_STRING_LENGTH-1, mob->short_descr, obj->short_descr );
  free_string( mob->short_descr );
  mob->short_descr = str_dup( buf );

  swprintf( buf, MAX_STRING_LENGTH-1, L"%s burada, dik dik sana bakıyor!\n\r", capitalize(obj->short_descr) );
  free_string( mob->long_descr );
  mob->long_descr = str_dup( buf );

  char_to_room( mob, ch->in_room);
  mob->level = obj->level;

  for(i=0; i < MAX_STATS; i++)
	mob->perm_stat[i] = UMIN(25, ch->perm_stat[i]);
  for(i=0; i<3; i++)
	mob->armor[i] = interpolate( mob->level, 100, -100);
  mob->armor[3] = interpolate(mob->level, 100, 0);

  if (obj->item_type == ITEM_WEAPON)
  {
     mob->hit = IS_NPC(ch) ? 100 :
		UMIN( (25 * mob->level) + 1000, 30000);
     mob->max_hit = mob->hit;
     mob->mana = ch->level * 40;
     mob->max_mana = mob->mana;
     mob->move = ch->level * 40;
     mob->max_move = mob->move;
     mob->timer = 0;
     mob->damage[DICE_NUMBER] = obj->value[1];
     mob->damage[DICE_TYPE] = obj->value[2];
     mob->damage[DICE_BONUS] = number_range(level/10, level/8);
  }

  if (obj->item_type == ITEM_ARMOR)
  {
     mob->hit = IS_NPC(ch) ? 100:
		UMIN( (100 * mob->level) + 2000, 30000);
     mob->max_hit = mob->hit;
     mob->mana = ch->level * 40;
     mob->max_mana = mob->mana;
     mob->move = ch->level * 40;
     mob->max_move = mob->move;
     mob->timer = 0;
     mob->damage[DICE_NUMBER] = number_range(level/15, level/12);
     mob->damage[DICE_TYPE] = number_range(level/3, level/2);
     mob->damage[DICE_BONUS] = number_range(level/10, level/8);
  }
  mob->sex = ch->sex;
  mob->silver = 0;
  mob->master = mob->leader = ch;
  SET_BIT(mob->affected_by, AFF_CHARM);

  af.where	= TO_AFFECTS;
  af.type 	= sn;
  af.level	= ch->level;
  af.duration	= 1 + (obj->level / 30);
  af.modifier	= 0;
  af.location	= APPLY_NONE;
  af.bitvector	= 0;
  affect_to_char(ch, &af);

  act( L"Gücünle $p'ye yaşam veriyorsun!\n\r", ch, obj, NULL, TO_CHAR);
  act( L"$n gücüyle $p'ye yaşam veriyor!\n\r", ch, obj, NULL, TO_ROOM);

  extract_obj( obj );
	return;
}

void spell_windwall( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    wchar_t buf[MAX_STRING_LENGTH];
    int dam,hp_dam,dice_dam,hpch;

    act( L"$n herkese saldıran bir rüzgar duvarı yaratıyor.",ch,NULL,NULL,TO_ROOM);
    act( L"Bir rüzgar duvarı oluşturuyorsun.",ch,NULL,NULL,TO_CHAR);

    hpch = UMAX(16,ch->hit);
    hp_dam = number_range(hpch/15+1,8);
    dice_dam = dice(level,12);

    dam = UMAX(hp_dam + dice_dam/10,dice_dam + hp_dam/10);

    for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
    {
	vch_next = vch->next_in_room;

	if (is_safe_spell(ch,vch,TRUE)
	||  (IS_NPC(ch) && IS_NPC(vch)
	&&   (ch->fighting == vch || vch->fighting == ch)))
	    continue;
	if ( is_safe(ch, vch) )
          continue;
          if (!IS_NPC(ch) && vch != ch &&
              ch->fighting != vch && vch->fighting != ch &&
              (IS_SET(vch->affected_by,AFF_CHARM) || !IS_NPC(vch)))
            {
            if (!can_see(vch, ch))
                do_yell(vch, (wchar_t*)"İmdat! Biri bana saldırıyor!");
            else
              {
                 swprintf( buf, MAX_STRING_LENGTH-1, L"Geber %s, seni büyücü köpek!",
                    (is_affected(ch,gsn_doppelganger)&&!IS_IMMORTAL(vch))?
                     ch->doppel->name : ch->name);
                 do_yell(vch,buf);
              }
          }

	if (!IS_AFFECTED(vch,AFF_FLYING)) dam /= 2;

	if (vch->size == SIZE_TINY)  dam = (int)((float)dam * 1.5);
	else if (vch->size == SIZE_SMALL)  dam = (int)((float)dam * 1.3);
	else if (vch->size == SIZE_MEDIUM)  dam *= 1;
	else if (vch->size == SIZE_LARGE)  dam = (int)((float)dam * 0.9);
	else if (vch->size == SIZE_HUGE)  dam = (int)((float)dam * 0.7);
	else dam = (int)((float)dam * 0.5);

	if (saves_spell(level,vch,DAM_OTHER))
	    damage(ch,vch,dam/2,sn,DAM_OTHER,TRUE);
	else
	    damage(ch,vch,dam,sn,DAM_OTHER,TRUE);
    }
	return;
}

void spell_earthfade( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    AFFECT_DATA af;

    if ( CAN_DETECT(ch, ADET_EARTHFADE) )
	return;

    if (ch->in_room->sector_type == SECT_AIR
	|| ch->in_room->sector_type == SECT_WATER_SWIM
	|| ch->in_room->sector_type == SECT_WATER_NOSWIM )
    {
      send_to_char( L"Toprağa erişemiyorsun.\n\r", ch);
	return;
    }

    if (MOUNTED(ch))
    {
      send_to_char( L"Biniciyken olmaz.\n\r", ch);
        return;
    }

    act( L"$n arzla bir oluyor.", ch, NULL, NULL, TO_ROOM );

    af.where     = TO_DETECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = (level / 8 + 10);
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = ADET_EARTHFADE;
    affect_to_char( ch, &af );
    send_to_char( L"Arzla bir oluyorsun.\n\r", ch );
    return;
}

void spell_earthmaw( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    wchar_t buf[MAX_STRING_LENGTH];
    int dam;

    swprintf( buf, MAX_STRING_LENGTH-1, L"%s'in altındaki toprağı titretiyorsun.\n\r",victim->name);
    send_to_char(buf, ch);
    act( L"$n altındaki toprağı titretiyor!.", ch, NULL, victim, TO_VICT );
    if (IS_AFFECTED(victim,AFF_FLYING))
        dam = 0;
    else dam = dice( level , 16 );
    damage(ch,victim,dam,sn,DAM_BASH,TRUE);
    return;
}

void spell_drain( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    AFFECT_DATA *paf;
    int drain;

    if (!IS_SET(obj->extra_flags,ITEM_MAGIC))
    {
      send_to_char( L"O büyülü bir eşya değil.\n\r",ch);
        return;
    }

    switch (obj->item_type)
    {
	default:
	 drain = 1;
	break;

	case ITEM_ARMOR:	drain = obj->value[3];	break;
	case ITEM_TREASURE:	drain = 4;		break;
	case ITEM_POTION:	drain = 8;		break;
	case ITEM_SCROLL:
	case ITEM_STAFF:
	case ITEM_WAND:		drain = 12;		break;
	case ITEM_WEAPON:
	 drain = obj->value[1] + obj->value[2] / 2;
	break;

	case ITEM_LIGHT:
	 if (obj->value[2] == -1)     drain = 10;
	 else drain = 4;
	break;
    }

    for (paf = obj->affected; paf != NULL; paf = paf->next)
	drain += 5;

    drain *= dice(2,5);
    drain += obj->level / 2;

    if (number_percent() > get_skill(ch, sn))
    {
      act( L"$p buharlaşıyor!",ch,obj,NULL,TO_ROOM);
      act( L"$p buharlaşıyor fakat enerjiyi aktaramıyorsun.",ch,obj,NULL,TO_CHAR);
    }
    else
    {
      act( L"$n enerjisini emdiğinden $p buharlaşıyor!",ch,obj,NULL,TO_ROOM);
    	act( L"Enerjisini emdiğinden $p buharlaşıyor!",ch,obj,NULL,TO_CHAR);
	ch->mana = UMIN(ch->mana + drain, ch->max_mana);
    }
    extract_obj(obj);
    return;

}

void spell_soften( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected(victim, sn) )
	return;

    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level	 = level;
    af.duration  = 5 + level / 10;
    af.location  = APPLY_AC;
    af.modifier  = 4 * level;
    af.bitvector = AFF_FAERIE_FIRE;
    affect_to_char( victim, &af );

    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = (10 + level / 5);
    af.location  = APPLY_SAVING_SPELL;
    af.modifier  = -1;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    send_to_char( L"Derin buruşmaya başladı.\n\r", victim );
    act( L"$s derisi buruşmaya başladı.", victim, NULL, NULL, TO_ROOM );
    return;
}


void spell_fumble( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    OBJ_DATA *obj;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
    {
	if (victim == ch)
  send_to_char( L"Daha beceriksiz olamazsın!\n\r",ch);
	else
  act( L"$N bundan daha beceriksiz olamaz.",
	      ch,NULL,victim,TO_CHAR);
	return;
    }

    if (saves_spell(level,victim,DAM_OTHER)
    ||  IS_SET(victim->imm_flags,IMM_MAGIC))
    {
	if (victim != ch)
  send_to_char( L"Hiçbir şey olmuyor.\n\r",ch);
send_to_char( L"Bir an için uyuşukluk hissediyorsun.\n\r",victim);
	return;
    }

    if (IS_AFFECTED(victim,AFF_HASTE))
    {
	if (!check_dispel(level,victim,skill_lookup( L"haste")))
	{
	    if (victim != ch)
      send_to_char( L"Büyü işe yaramadı.\n\r",ch);
  	    send_to_char( L"Bir an için yavaşladığını hissediyorsun.\n\r",victim);
	    return;
	}

  act( L"$n daha acelesiz hareket etmeye başladı.",victim,NULL,NULL,TO_ROOM);
	return;
    }


    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = (4 + level / 12);
    af.location  = APPLY_DEX;
    af.modifier  = - UMAX(2,level / 6);
    af.bitvector = 0;
    affect_to_char( victim, &af );

    if ((obj = get_weapon_char(victim, TRUE)) != NULL)
    {
	if (can_drop_obj(victim,obj)
	    &&  remove_obj(victim,obj,TRUE))
	{
    act( L"$n $p'yi daha fazla taşıyamayarak düşürüyor.", victim,obj,NULL,TO_ROOM);
	  send_to_char( L"Çifte silahını taşıyamayarak düşürüyorsun!\n\r", victim);
	  obj_from_char(obj);
	  obj_to_room(obj,victim->in_room);
	}
    }
    if ((obj = get_weapon_char(victim, FALSE)) != NULL)
    {
	if (can_drop_obj(victim,obj)
	    &&  remove_obj(victim,obj,TRUE))
	{
    act( L"$n $p'yi daha fazla taşıyamayarak düşürüyor.", victim,obj,NULL,TO_ROOM);
	  send_to_char( L"Silahını daha fazla taşıyamayarak düşürüyorsun!\n\r", victim);
	  obj_from_char(obj);
	  obj_to_room(obj,victim->in_room);
	}
    }

    WAIT_STATE(victim, PULSE_VIOLENCE);
    send_to_char( L"Kendini çok beceriksiz hissediyorsun...\n\r", victim );
    act( L"$n beceriksizce birşeyler yapmaya çalışıyor.",victim,NULL,NULL,TO_ROOM);
    return;
}
