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
int skill_lookup( const char *name )
{
    int sn;

    for ( sn = 0; sn < MAX_SKILL; sn++ )
    {
	if ( skill_table[sn].name == NULL )
	    break;
	if ( LOWER(name[0]) == LOWER(skill_table[sn].name[0])
	&&   !str_prefix( name, skill_table[sn].name ) )
	    return sn;
    }

    return -1;
}

int find_spell( CHAR_DATA *ch, const char *name )
{
    /* finds a spell the character can cast if possible */
    int sn, found = -1;

    if (IS_NPC(ch))
	return skill_lookup(name);

    for ( sn = 0; sn < MAX_SKILL; sn++ )
    {
	if (skill_table[sn].name == NULL)
	    break;
	if (LOWER(name[0]) == LOWER(skill_table[sn].name[0])
	&&  !str_prefix(name,skill_table[sn].name))
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
	bug( "Slot_lookup: bad slot %d.", slot );
	abort( );
    }

    return -1;
}



/*
 * Utter mystical words for an sn.
 */
void say_spell( CHAR_DATA *ch, int sn )
{
    char buf  [MAX_STRING_LENGTH];
    char buf2 [MAX_STRING_LENGTH];
    CHAR_DATA *rch;
    char *pName;
    int iSyl;
    int length;
    int skill;

    struct syl_type
    {
	char *	old;
	char *	inew;
    };

    static const struct syl_type syl_table[] =
    {
	{ " ",		" "		},
	{ "ar",		"abra"		},
	{ "au",		"kada"		},
	{ "bless",	"fido"		},
	{ "blind",	"nose"		},
	{ "bur",	"mosa"		},
	{ "cu",		"judi"		},
	{ "de",		"oculo"		},
	{ "en",		"unso"		},
	{ "light",	"dies"		},
	{ "lo",		"hi"		},
	{ "mor",	"zak"		},
	{ "move",	"sido"		},
	{ "ness",	"lacri"		},
	{ "ning",	"illa"		},
	{ "per",	"duda"		},
	{ "ra",		"gru"		},
	{ "fresh",	"ima"		},
	{ "re",		"candus"	},
	{ "son",	"sabru"		},
	{ "tect",	"infra"		},
	{ "tri",	"cula"		},
	{ "ven",	"nofo"		},
	{ "a", "a" }, { "b", "b" }, { "c", "q" }, { "d", "e" },
	{ "e", "z" }, { "f", "y" }, { "g", "o" }, { "h", "p" },
	{ "i", "u" }, { "j", "y" }, { "k", "t" }, { "l", "r" },
	{ "m", "w" }, { "n", "i" }, { "o", "a" }, { "p", "s" },
	{ "q", "d" }, { "r", "f" }, { "s", "g" }, { "t", "h" },
	{ "u", "j" }, { "v", "z" }, { "w", "x" }, { "x", "n" },
	{ "y", "l" }, { "z", "k" },
	{ "", "" }
    };

    buf[0]	= '\0';
    for ( pName = skill_table[sn].name; *pName != '\0'; pName += length )
    {
	for ( iSyl = 0; (length = strlen(syl_table[iSyl].old)) != 0; iSyl++ )
	{
	    if ( !str_prefix( syl_table[iSyl].old, pName ) )
	    {
		strcat( buf, syl_table[iSyl].inew );
		break;
	    }
	}

	if ( length == 0 )
	    length = 1;
    }

    sprintf( buf2, "$n utters the words, '%s'.", buf );
    sprintf( buf,  "$n utters the words, '%s'.", skill_table[sn].name );

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
            		send_to_char( "\n\r", victim );
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
	|| sn == find_spell(ch,"chain lightning") ) return 0;
 else return skill_table[sn].skill_level[ch->iclass] / 10;
}

/*
 * The kludgy global is for spells who want more stuff from command line.
 */
char *target_name;

void do_cast( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
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
	 send_to_char("You reach for the True Source and feel something stopping you.\n\r",ch);
	 return;
	}

     if (is_affected(ch, gsn_garble) || is_affected(ch, gsn_deafen))
     {
        send_to_char("You can't get the right intonations.\n\r", ch);
        return;
     }

    target_name = one_argument( argument, arg1 );
    one_argument( target_name, arg2 );

    if ( arg1[0] == '\0' )
    {
	send_to_char( "Cast which what where?\n\r", ch );
	return;
    }

    if (ch->cabal == CABAL_BATTLE && !IS_IMMORTAL(ch)) 
    {
      send_to_char("You are a BattleRager, not a filthy magician!\n\r",ch);
      return;
    }

    if (  ( sn = find_spell( ch,arg1 ) ) < 0 
          || ch_skill_nok_nomessage(ch,sn) )
    {
	send_to_char( "You don't know any spells of that name.\n\r", ch );
	return;
    }

    if ( ch->iclass == CLASS_VAMPIRE
	&& !IS_VAMPIRE(ch) && skill_table[sn].cabal == CABAL_NONE)
    {
      send_to_char("You must transform to vampire before casting!\n\r",ch);
      return;
    }

    if ( skill_table[sn].spell_fun == spell_null )
    {
        send_to_char("That's not a spell.\n\r",ch);
        return;
    }

    if ( ch->position < skill_table[sn].minimum_position )
    {
	send_to_char( "You can't concentrate enough.\n\r", ch );
	return;
    }

    if (!cabal_ok(ch,sn))
      return;

    if (IS_SET(ch->in_room->room_flags,ROOM_NO_MAGIC))
    {
        send_to_char("Your spell fizzles out and fails.\n\r",ch);
        act("$n's spell fizzles out and fails.", ch, NULL, NULL, TO_ROOM);
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
	bug( "Do_cast: bad target for sn %d.", sn );
	return;

    case TAR_IGNORE:
        if ( is_affected(ch,gsn_spellbane))
          {
	    WAIT_STATE( ch, skill_table[sn].beats );
            act("Your spellbane deflects the spell!",ch,NULL,NULL,TO_CHAR);
            act("$n's spellbane deflects the spell!",ch,NULL,NULL,TO_ROOM);
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
		send_to_char( "Cast the spell on whom?\n\r", ch );
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
	      act("You can't cast this spell to $N at this distance.",
			ch,NULL,victim,TO_CHAR);
	      return;
	     }

	     cast_far = 1;
	    }
	    else if ( ( victim = get_char_room( ch, target_name ) ) == NULL )
	    {
		send_to_char( "They aren't here.\n\r", ch );
		return;
	    }
	}

	if ( !IS_NPC(ch) && is_safe(ch,victim) )
		return;
/*
	if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim )
	{
	    send_to_char( "You can't do that on your own follower.\n\r",
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
                do_yell(victim, "Help someone is attacking me!");
            else
              {
                 sprintf(buf,"Die, %s, you sorcerous dog!",
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
                act("Your spellbane deflects the spell!",ch,NULL,NULL,TO_CHAR);
                act("$n's spellbane deflects the spell!",ch,NULL,NULL,TO_ROOM);
	        check_improve(victim,gsn_spellbane,TRUE,1);
                damage(ch,ch,3 * ch->level,gsn_spellbane,DAM_NEGATIVE,TRUE);
            }
            else 
	    {
              act("$N deflects your spell!",ch,NULL,victim,TO_CHAR);
              act("You deflect $n's spell!",ch,NULL,victim,TO_VICT);
              act("$N deflects $n's spell!",ch,NULL,victim,TO_NOTVICT);
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
            act("Your spell fails to pass $N's energy field!",ch,NULL,victim,TO_CHAR);
            act("You absorb $n's spell!",ch,NULL,victim,TO_VICT);
            act("$N absorbs $n's spell!",ch,NULL,victim,TO_NOTVICT);
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
		send_to_char( "They aren't here.\n\r", ch );
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
                act("Your spellbane deflects the spell!",ch,NULL,NULL,TO_CHAR);
                act("$n's spellbane deflects the spell!",ch,NULL,NULL,TO_ROOM);
	        check_improve(victim,gsn_spellbane,TRUE,1);
                damage(victim,ch,3 * victim->level,gsn_spellbane,DAM_NEGATIVE, TRUE);              }
            else {
              act("$N deflects your spell!",ch,NULL,victim,TO_CHAR);
              act("You deflect $n's spell!",ch,NULL,victim,TO_VICT);
              act("$N deflects $n's spell!",ch,NULL,victim,TO_NOTVICT);
	      check_improve(victim,gsn_spellbane,TRUE,1);
              damage(victim,ch,3 * victim->level,gsn_spellbane,DAM_NEGATIVE, TRUE);
            }
            return;
          }
        if ( ch != victim && CAN_DETECT(victim, ADET_ABSORB) &&
            (number_percent() < 2*get_skill(victim,gsn_absorb)/3) )
	{
            act("Your spell fails to pass $N's energy field!",ch,NULL,victim,TO_CHAR);
            act("You absorb $n's spell!",ch,NULL,victim,TO_VICT);
            act("$N absorbs $n's spell!",ch,NULL,victim,TO_NOTVICT);
	    check_improve(victim,gsn_absorb,TRUE,1);
	    victim->mana += mana;
	    return;
	}
	break;

    case TAR_CHAR_SELF:
	if ( arg2[0] != '\0' && !is_name( target_name, ch->name ) )
	{
	    send_to_char( "You cannot cast this spell on another.\n\r", ch );
	    return;
	}

	vo = (void *) ch;
	target = TARGET_CHAR;

        if ( is_affected(ch,gsn_spellbane) )
          {
	    WAIT_STATE( ch, skill_table[sn].beats );
            act("Your spellbane deflects the spell!",ch,NULL,NULL,TO_CHAR);
            act("$n's spellbane deflects the spell!",ch,NULL,NULL,TO_ROOM);
	    check_improve(ch,gsn_spellbane,TRUE,1);
            damage(ch,ch,3 * ch->level,gsn_spellbane,DAM_NEGATIVE, TRUE);
            return;
          }

	break;

    case TAR_OBJ_INV:
	if ( arg2[0] == '\0' )
	{
	    send_to_char( "What should the spell be cast upon?\n\r", ch );
	    return;
	}

	if ( ( obj = get_obj_carry( ch, target_name ) ) == NULL )
	{
	    send_to_char( "You are not carrying that.\n\r", ch );
	    return;
	}

	vo = (void *) obj;
	target = TARGET_OBJ;
        if ( is_affected(ch,gsn_spellbane) )
          {
	    WAIT_STATE( ch, skill_table[sn].beats );
            act("Your spellbane deflects the spell!",ch,NULL,NULL,TO_CHAR);
            act("$n's spellbane deflects the spell!",ch,NULL,NULL,TO_ROOM);
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
		send_to_char("Cast the spell on whom or what?\n\r",ch);
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
		send_to_char("Your spell didn't work.\n\r",ch);
		return;
	    }

	    if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim )
	    {
		send_to_char( "You can't do that on your own follower.\n\r",
		    ch );
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
	    send_to_char("You don't see that here.\n\r",ch);
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
	    send_to_char("You don't see that here.\n\r",ch);
	    return;
	}
	break;
    }

    if ( !IS_NPC(ch) && ch->mana < mana )
    {
	send_to_char( "You don't have enough mana.\n\r", ch );
	return;
    }

    if ( str_cmp( skill_table[sn].name, "ventriloquate" ) )
	say_spell( ch, sn );

    WAIT_STATE( ch, skill_table[sn].beats );

    if ( number_percent( ) > get_skill(ch,sn) )
    {
	send_to_char( "You lost your concentration.\n\r", ch );
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
	bug( "Obj_cast_spell: bad sn %d.", sn );
	return;
    }

    if ( (IS_NPC(ch) && ch->position == POS_DEAD) ||
        (!IS_NPC(ch) && (current_time - ch->last_death_time) < 10))
    {
	bug( "Obj_cast_spell: Ch is dead! But it is ok.", sn);
	return;
    }

    if (victim != NULL)
    {
      if ( (IS_NPC(victim) && victim->position == POS_DEAD) ||
        (!IS_NPC(victim) && (current_time - victim->last_death_time) < 10))
      {
	bug( "Obj_cast_spell: Victim is dead! But it is ok.. ", sn);
	return;
      }
    }

    switch ( skill_table[sn].target )
    {
    default:
	bug( "Obj_cast_spell: bad target for sn %d.", sn );
	return;

    case TAR_IGNORE:
	vo = NULL;
	break;

    case TAR_CHAR_OFFENSIVE:
	if ( victim == NULL )
	    victim = ch->fighting;
	if ( victim == NULL )
	{
	    send_to_char( "You can't do that.\n\r", ch );
	    return;
	}
	if (is_safe(ch,victim) && ch != victim)
	{
	    send_to_char("Something isn't right...\n\r",ch);
	    return;
	}
	vo = (void *) victim;
	target = TARGET_CHAR;
        if ( is_affected(victim,gsn_spellbane) && (/*IS_NPC(victim) ||*/
                number_percent() < 2*get_skill(victim, gsn_spellbane)/3) )
        {
            if (ch==victim)
              {
                act("Your spellbane deflects the spell!",ch,NULL,NULL,TO_CHAR);
                act("$n's spellbane deflects the spell!",ch,NULL,NULL,TO_ROOM);
	        check_improve(victim,gsn_spellbane,TRUE,1);
                damage(victim,ch,10 * level,gsn_spellbane,DAM_NEGATIVE, TRUE);
              }
            else {
              act("$N deflects your spell!",ch,NULL,victim,TO_CHAR);
              act("You deflect $n's spell!",ch,NULL,victim,TO_VICT);
              act("$N deflects $n's spell!",ch,NULL,victim,TO_NOTVICT);
	      check_improve(victim,gsn_spellbane,TRUE,1);
              damage(victim,ch,10 * victim->level,gsn_spellbane,DAM_NEGATIVE, TRUE);
            }
            return;
        }

        if ( ch != victim && CAN_DETECT(victim, ADET_ABSORB) &&
            (number_percent() < 2*get_skill(victim,gsn_absorb)/3) 
		&& sn != slot_lookup(524) && sn != slot_lookup(204))  
	{
            act("Your spell fails to pass $N's energy field!",ch,NULL,victim,TO_CHAR);
            act("You absorb $n's spell!",ch,NULL,victim,TO_VICT);
            act("$N absorbs $n's spell!",ch,NULL,victim,TO_NOTVICT);
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
                act("Your spellbane deflects the spell!",ch,NULL,NULL,TO_CHAR);
                act("$n's spellbane deflects the spell!",ch,NULL,NULL,TO_ROOM);
	        check_improve(victim,gsn_spellbane,TRUE,1);
                damage(victim,ch,10 * victim->level,gsn_spellbane,DAM_NEGATIVE, TRUE);
              }
            else {
              act("$N deflects your spell!",ch,NULL,victim,TO_CHAR);
              act("You deflect $n's spell!",ch,NULL,victim,TO_VICT);
              act("$N deflects $n's spell!",ch,NULL,victim,TO_NOTVICT);
	      check_improve(victim,gsn_spellbane,TRUE,1);
              damage(victim,ch,10 * victim->level,gsn_spellbane,DAM_NEGATIVE, TRUE);
            }
            return;
          }
        break;

    case TAR_OBJ_INV:
	if ( obj == NULL )
	{
	    send_to_char( "You can't do that.\n\r", ch );
	    return;
	}
	vo = (void *) obj;
	target = TARGET_OBJ;
        if ( is_affected(ch,gsn_spellbane) )
          {
            act("Your spellbane deflects the spell!",ch,NULL,NULL,TO_CHAR);
            act("$n's spellbane deflects the spell!",ch,NULL,NULL,TO_ROOM);
	    check_improve(ch,gsn_spellbane,TRUE,1);
            damage(ch,ch,3 * ch->level,gsn_spellbane,DAM_NEGATIVE, TRUE);
            return;
          }

	break;

    case TAR_OBJ_CHAR_OFF:
	if ( victim == NULL && obj == NULL)
	    if (ch->fighting != NULL)
		victim = ch->fighting;
	    else
	    {
		send_to_char("You can't do that.\n\r",ch);
		return;
	    }

	    if (victim != NULL)
	    {
		if (is_safe_spell(ch,victim,FALSE) && ch != victim)
		{
		    send_to_char("Somehting isn't right...\n\r",ch);
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

    target_name = "";
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
	  send_to_char("You are already armored.\n\r",ch);
	else
	  act("$N is already armored.",ch,NULL,victim,TO_CHAR);
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
    send_to_char( "You feel someone protecting you.\n\r", victim );
    if ( ch != victim )
	act("$N is protected by your magic.",ch,NULL,victim,TO_CHAR);
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
	    act("$p is already blessed.",ch,obj,NULL,TO_CHAR);
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
		act("$p glows a pale blue.",ch,obj,NULL,TO_ALL);
		REMOVE_BIT(obj->extra_flags,ITEM_EVIL);
		return;
	    }
	    else
	    {
		act("The evil of $p is too powerful for you to overcome.",
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

	act("$p glows with a holy aura.",ch,obj,NULL,TO_ALL);
	return;
    }

    /* character target */
    victim = (CHAR_DATA *) vo;


    if ( victim->position == POS_FIGHTING || is_affected( victim, sn ) )
    {
	if (victim == ch)
	  send_to_char("You are already blessed.\n\r",ch);
	else
	  act("$N already has divine favor.",ch,NULL,victim,TO_CHAR);
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
    send_to_char( "You feel righteous.\n\r", victim );
    if ( ch != victim )
	act("You grant $N the favor of your god.",ch,NULL,victim,TO_CHAR);
    return;
}



void spell_blindness( int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_BLIND) || 
        saves_spell(level,victim,DAM_OTHER))  {
      send_to_char( "You failed.\n\r", ch );
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
    send_to_char( "You are blinded!\n\r", victim );
    act("$n appears to be blinded.",victim,NULL,NULL,TO_ROOM);
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
	send_to_char( "You must be out of doors.\n\r", ch );
	return;
    }

    if ( weather_info.sky < SKY_RAINING )
    {
	send_to_char( "You need bad weather.\n\r", ch );
	return;
    }

    dam = dice(level, 14);

    send_to_char( "Gods' lightning strikes your foes!\n\r", ch );
    act( "$n calls lightning to strike $s foes!",
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
		send_to_char("The electricity fizzles at your foes.\n\r",vch);
		act("A lightning bolt fizzles at $N's foes.\n\r",
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
	    send_to_char( "Lightning flashes in the sky.\n\r", vch );
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
	    ||  is_affected(vch,skill_lookup("frenzy")))
	      return;

	    send_to_char("A wave of calm passes over you.\n\r",vch);

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
	send_to_char("You failed, try dispel magic.\n\r",ch);
	return;
    }

    if ( !is_same_group(ch, victim) && ch != victim
	 && (IS_NPC(victim) || IS_SET(victim->act, PLR_NOCANCEL)) )
    {
	act("You cannot cast this spell to $N.", ch, NULL, victim, TO_CHAR);
	return;
    }

    /* unlike dispel magic, the victim gets NO save */

    /* begin running through the spells */

    if (check_dispel(level,victim,skill_lookup("armor")))
	found = TRUE;
 
    if (check_dispel(level,victim,skill_lookup("enhanced armor")))
	found = TRUE;
 
    if (check_dispel(level,victim,skill_lookup("bless")))
        found = TRUE;
 
    if (check_dispel(level,victim,skill_lookup("blindness")))
    {
        found = TRUE;
        act("$n is no longer blinded.",victim,NULL,NULL,TO_ROOM);
    }

    if (check_dispel(level,victim,skill_lookup("calm")))
    {
	found = TRUE;
	act("$n no longer looks so peaceful...",victim,NULL,NULL,TO_ROOM);
    }
 
    if (check_dispel(level,victim,skill_lookup("change sex")))
    {
        found = TRUE;
	act("$n looks more like $mself again.",victim,NULL,NULL,TO_ROOM);
    }

    if (check_dispel(level,victim,skill_lookup("charm person")))
    {
	found = TRUE;
	act("$n regains $s free will.",victim,NULL,NULL,TO_ROOM);
    }

    if (check_dispel(level,victim,skill_lookup("chill touch")))
    {
	found = TRUE;
	act("$n looks warmer.",victim,NULL,NULL,TO_ROOM);
    }

    if (check_dispel(level,victim,skill_lookup("curse")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup("detect evil")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup("detect good")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup("detect hidden")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup("detect invis")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup("detect hidden")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup("detect magic")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup("faerie fire")))
    {
	act("$n's outline fades.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup("fly")))
    {
	act("$n falls to the ground!",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup("frenzy")))
    {
	act("$n no longer looks so wild.",victim,NULL,NULL,TO_ROOM);;
	found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup("giant strength")))
    {
	act("$n no longer looks so mighty.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup("haste")))
    {
	act("$n is no longer moving so quickly.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup("infravision")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup("invis")))
    {
	act("$n fades into existance.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup("mass invis")))
    {
	act("$n fades into existance.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup("pass door")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup("protection evil")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup("protection good")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup("sanctuary")))
    {
	act("The white aura around $n's body vanishes.",
	    victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup("shield")))
    {
	act("The shield protecting $n vanishes.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup("sleep")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup("slow")))
    {
	act("$n is no longer moving so slowly.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup("stone skin")))
    {
	act("$n's skin regains its normal texture.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup("weaken")))
    {
	act("$n looks stronger.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
 
    if (check_dispel(level,victim,skill_lookup("shielding")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup("web")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup("fear")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup("protection heat")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup("protection cold")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup("magic resistance")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup("hallucination")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup("terangreal")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup("power word stun")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup("corruption")))
    {
	act("$n looks healthier.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup("web")))
    {
	act("The webs around $n dissolve.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }

    if (found)
        send_to_char("Ok.\n\r",ch);
    else
        send_to_char("Spell failed.\n\r",ch);
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
    char buf[MAX_STRING_LENGTH];
    bool found;
    int dam;

    /* first strike */

    act("A lightning bolt leaps from $n's hand and arcs to $N.",
	ch,NULL,victim,TO_ROOM);
    act("A lightning bolt leaps from your hand and arcs to $N.",
	ch,NULL,victim,TO_CHAR);
    act("A lightning bolt leaps from $n's hand and hits you!",
	ch,NULL,victim,TO_VICT);

    dam = dice(level,6);

    if (CAN_DETECT(victim, ADET_GROUNDING))
    {
	send_to_char("The electricity fizzles at your foes.\n\r",victim);
	act("A lightning bolt fizzles at $N's foes.\n\r",
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
          do_yell(victim, "Help someone is attacking me!");
        else
          {
            sprintf(buf,"Die, %s, you sorcerous dog!",
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
	      act( "The bolt passes around $n's body.", ch, NULL, NULL, TO_ROOM );
	      act( "The bolt passes around your body.", ch, NULL, NULL, TO_CHAR );
	    }
	    else  {
	      act("The bolt arcs to $n!",tmp_vict,NULL,NULL,TO_ROOM);
	      act("The bolt hits you!",tmp_vict,NULL,NULL,TO_CHAR);
	      dam = dice(level,6);

            if (!IS_NPC(ch) && tmp_vict != ch &&
                ch->fighting != tmp_vict && tmp_vict->fighting != ch &&
                (IS_SET(tmp_vict->affected_by,AFF_CHARM) || !IS_NPC(tmp_vict)))
            {
               if (!can_see(tmp_vict, ch))
                    do_yell(tmp_vict, "Help someone is attacking me!");
               else
               {
                 sprintf(buf,"Die, %s, you sorcerous dog!",                    
		(is_affected(ch,gsn_doppelganger)&&!IS_IMMORTAL(tmp_vict))?
                     ch->doppel->name : ch->name);
                 do_yell(tmp_vict,buf);
               }
             }

	     if (CAN_DETECT(tmp_vict, ADET_GROUNDING))
	     {
		send_to_char("The electricity fizzles at your foes.\n\r",tmp_vict);
		act("A lightning bolt fizzles at $N's foes.\n\r",
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
	    act("The bolt seems to have fizzled out.",ch,NULL,NULL,TO_ROOM);
	    act("The bolt grounds out through your body.",
		ch,NULL,NULL,TO_CHAR);
	    return;
	  }

	  last_vict = ch;
	  act("The bolt arcs to $n...whoops!",ch,NULL,NULL,TO_ROOM);
	  send_to_char("You are struck by your own lightning!\n\r",ch);
	  dam = dice(level,6);

	  if (CAN_DETECT(ch, ADET_GROUNDING))
	  {
		send_to_char("The electricity fizzles at your foes.\n\r",ch);
		act("A lightning bolt fizzles at $N's foes.\n\r",
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
	send_to_char("This room has already been healed by light.\n\r",ch);
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
    send_to_char( "The room starts to be filled with healing light.\n\r", ch );
    act("The room starts to be filled with $n's healing light.",ch,NULL,NULL,TO_ROOM);
    return;
}



void spell_charm_person( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    char buf[MAX_INPUT_LENGTH];
    AFFECT_DATA af;

    if (is_safe(ch,victim)) return;

    if (count_charmed(ch)) return;

    if ( victim == ch )
    {
	send_to_char( "You like yourself even better!\n\r", ch );
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
    act( "Isn't $n just so nice?", ch, NULL, victim, TO_VICT );
    if ( ch != victim )
	act("$N looks at you with adoring eyes.",ch,NULL,victim,TO_CHAR);

    if (IS_NPC(victim) 	&& !IS_NPC(ch) )
    {
     if (number_percent() < (4 + (victim->level - ch->level)) * 10 )
     	add_mind(victim,ch->name);
     else if (victim->in_mind == NULL)
	{
	 sprintf(buf,"%d",victim->in_room->vnum);
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
	act("$n turns blue and shivers.",victim,NULL,NULL,TO_ROOM);
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
	spell_blindness(skill_lookup("blindness"),
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
	    send_to_char("You don't see that here.\n\r",ch);
	    return;
	}

	if (IS_OBJ_STAT(light,ITEM_GLOW))
	{
	    act("$p is already glowing.",ch,light,NULL,TO_CHAR);
	    return;
	}

	SET_BIT(light->extra_flags,ITEM_GLOW);
	act("$p glows with a white light.",ch,light,NULL,TO_ALL);
	return;
    }

    light = create_object( get_obj_index( OBJ_VNUM_LIGHT_BALL ), 0 );
    obj_to_room( light, ch->in_room );
    act( "$n twiddles $s thumbs and $p appears.",   ch, light, NULL, TO_ROOM );
    act( "You twiddle your thumbs and $p appears.", ch, light, NULL, TO_CHAR );
    return;
}



void spell_control_weather(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    if ( !str_cmp( target_name, "better" ) )
	weather_info.change += dice( level / 3, 4 );
    else if ( !str_cmp( target_name, "worse" ) )
	weather_info.change -= dice( level / 3, 4 );
    else  {
	send_to_char ("Do you want it to get better or worse?\n\r", ch );
	return;
    }

    send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_create_food( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    OBJ_DATA *mushroom;

    mushroom = create_object( get_obj_index( OBJ_VNUM_MUSHROOM ), 0 );
    mushroom->value[0] = level / 2;
    mushroom->value[1] = level;
    obj_to_room( mushroom, ch->in_room );
    act( "$p suddenly appears.", ch, mushroom, NULL, TO_ROOM );
    act( "$p suddenly appears.", ch, mushroom, NULL, TO_CHAR );
    return;
}

void spell_create_rose( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    OBJ_DATA *rose;
    rose = create_object(get_obj_index(OBJ_VNUM_ROSE), 0);
    act("$n has created a beautiful red rose.",ch,rose,NULL,TO_ROOM);
    send_to_char("You create a beautiful red rose.\n\r",ch);
    obj_to_char(rose,ch);
    return;
}

void spell_create_spring(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    OBJ_DATA *spring;

    spring = create_object( get_obj_index( OBJ_VNUM_SPRING ), 0 );
    spring->timer = level;
    obj_to_room( spring, ch->in_room );
    act( "$p flows from the ground.", ch, spring, NULL, TO_ROOM );
    act( "$p flows from the ground.", ch, spring, NULL, TO_CHAR );
    return;
}



void spell_create_water( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    int water;

    if ( obj->item_type != ITEM_DRINK_CON )
    {
	send_to_char( "It is unable to hold water.\n\r", ch );
	return;
    }

    if ( obj->value[2] != LIQ_WATER && obj->value[1] != 0 )
    {
	send_to_char( "It contains some other liquid.\n\r", ch );
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
	if ( !is_name( "water", obj->name ) )
	{
	    char buf[MAX_STRING_LENGTH];

	    sprintf( buf, "%s water", obj->name );
	    free_string( obj->name );
	    obj->name = str_dup( buf );
	}
	act( "$p is filled.", ch, obj, NULL, TO_CHAR );
    }

    return;
}



void spell_cure_blindness(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;

    if ( !is_affected( victim, gsn_blindness ) )
    {
	if (victim == ch)
	  send_to_char("You aren't blind.\n\r",ch);
	else
	  act("$N doesn't appear to be blinded.",ch,NULL,victim,TO_CHAR);
	return;
    }

    if (check_dispel(level,victim,gsn_blindness))
    {
	send_to_char( "Your vision returns!\n\r", victim );
	act("$n is no longer blinded.",victim,NULL,NULL,TO_ROOM);
    }
    else
	send_to_char("Spell failed.\n\r",ch);
}



void spell_cure_critical( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int heal;

    heal = dice(3, 8) + level / 2 ;
    victim->hit = UMIN( victim->hit + heal, victim->max_hit );
    update_pos( victim );
    send_to_char( "You feel better!\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}

/* RT added to cure plague */
void spell_cure_disease( int sn, int level, CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;

    if ( !is_affected( victim, gsn_plague ) )
    {
	if (victim == ch)
	  send_to_char("You aren't ill.\n\r",ch);
	else
	  act("$N doesn't appear to be diseased.",ch,NULL,victim,TO_CHAR);
	return;
    }

    if (check_dispel(level,victim,gsn_plague))
    {
	send_to_char("Your sores vanish.\n\r",victim);
	act("$n looks relieved as $s sores vanish.",victim,NULL,NULL,TO_ROOM);
    }
    else
	send_to_char("Spell failed.\n\r",ch);
}



void spell_cure_light( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int heal;

    heal = dice(1, 8) + level / 4 + 5;
    victim->hit = UMIN( victim->hit + heal, victim->max_hit );
    update_pos( victim );
    send_to_char( "You feel better!\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_cure_poison( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;

    if ( !is_affected( victim, gsn_poison ) )
    {
	if (victim == ch)
	  send_to_char("You aren't poisoned.\n\r",ch);
	else
	  act("$N doesn't appear to be poisoned.",ch,NULL,victim,TO_CHAR);
	return;
    }

    if (check_dispel(level,victim,gsn_poison))
    {
	send_to_char("A warm feeling runs through your body.\n\r",victim);
	act("$n looks much better.",victim,NULL,NULL,TO_ROOM);
    }
    else
	send_to_char("Spell failed.\n\r",ch);
}

void spell_cure_serious( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int heal;

    heal = dice(2, 8) + level / 3 + 10 ;
    victim->hit = UMIN( victim->hit + heal, victim->max_hit );
    update_pos( victim );
    send_to_char( "You feel better!\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
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
	    act("You must remove $p first.",ch,obj,NULL,TO_CHAR);
	    return;
	}

	if (IS_OBJ_STAT(obj,ITEM_EVIL))
	{
	    act("$p is already filled with evil.",ch,obj,NULL,TO_CHAR);
	    return;
	}

	if (IS_OBJ_STAT(obj,ITEM_BLESS))
	{
	    AFFECT_DATA *paf;

	    paf = affect_find(obj->affected,skill_lookup("bless"));
	    if (!saves_dispel(level,paf != NULL ? paf->level : obj->level,0))
	    {
		if (paf != NULL)
		    affect_remove_obj(obj,paf);
		act("$p glows with a red aura.",ch,obj,NULL,TO_ALL);
		REMOVE_BIT(obj->extra_flags,ITEM_BLESS);
		return;
	    }
	    else
	    {
		act("The holy aura of $p is too powerful for you to overcome.",
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

	act("$p glows with a malevolent aura.",ch,obj,NULL,TO_ALL);
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

    send_to_char( "You feel unclean.\n\r", victim );
    if ( ch != victim )
	act("$N looks very uncomfortable.",ch,NULL,victim,TO_CHAR);
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
	send_to_char("The demons turn upon you!\n\r",ch);
    }

    if (victim != ch)
    {
	act("$n calls forth the demons of Hell upon $N!",
	    ch,NULL,victim,TO_ROOM);
	act("$n has assailed you with the demons of Hell!",
	    ch,NULL,victim,TO_VICT);
	send_to_char("You conjure forth the demons of hell!\n\r",ch);
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
	send_to_char("Your blue fire turn upon you!\n\r",ch);
    }

    if (victim != ch)
    {
	act("$n calls forth the blue fire of earth $N!",
	    ch,NULL,victim,TO_ROOM);
	act("$n has assailed you with the neutrals of earth!",
	    ch,NULL,victim,TO_VICT);
	send_to_char("You conjure forth the blue fire!\n\r",ch);
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
	  send_to_char("You can already sense evil.\n\r",ch);
	else
	  act("$N can already detect evil.",ch,NULL,victim,TO_CHAR);
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
    send_to_char( "Your eyes tingle.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}


void spell_detect_good( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( CAN_DETECT(victim, DETECT_GOOD) )
    {
	if (victim == ch)
	  send_to_char("You can already sense good.\n\r",ch);
	else
	  act("$N can already detect good.",ch,NULL,victim,TO_CHAR);
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
    send_to_char( "Your eyes tingle.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_detect_hidden(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( CAN_DETECT(victim, DETECT_HIDDEN) )
    {
	if (victim == ch)
	  send_to_char("You are already as alert as you can be. \n\r",ch);
	else
	  act("$N can already sense hidden lifeforms.",ch,NULL,victim,TO_CHAR);
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
    send_to_char( "Your awareness improves.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_detect_invis( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( CAN_DETECT(victim, DETECT_INVIS) )
    {
	if (victim == ch)
	  send_to_char("You can already see invisible.\n\r",ch);
	else
	  act("$N can already see invisible things.",ch,NULL,victim,TO_CHAR);
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
    send_to_char( "Your eyes tingle.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_detect_magic( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( CAN_DETECT(victim, DETECT_MAGIC) )
    {
	if (victim == ch)
	  send_to_char("You can already sense magical auras.\n\r",ch);
	else
	  act("$N can already detect magic.",ch,NULL,victim,TO_CHAR);
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
    send_to_char( "Your eyes tingle.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_detect_poison( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;

    if ( obj->item_type == ITEM_DRINK_CON || obj->item_type == ITEM_FOOD )
    {
	if ( obj->value[3] != 0 )
	    send_to_char( "You smell poisonous fumes.\n\r", ch );
	else
	    send_to_char( "It looks delicious.\n\r", ch );
    }
    else
    {
	send_to_char( "It doesn't look poisoned.\n\r", ch );
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
	act( "Gods protects $N.", ch, NULL, victim, TO_ROOM );
	return;
    }

    if ( IS_NEUTRAL(victim) )
    {
	act( "$N does not seem to be affected.", ch, NULL, victim, TO_CHAR );
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
	act( "$N is protected by $S evil.", ch, NULL, victim, TO_ROOM );
	return;
    }

    if ( IS_NEUTRAL(victim) )
    {
	act( "$N does not seem to be affected.", ch, NULL, victim, TO_CHAR );
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
	send_to_char( "You feel a brief tingling sensation.\n\r",victim);
	send_to_char( "You failed.\n\r", ch);
	return;
    }

    /* begin running through the spells */

    if (check_dispel(level,victim,skill_lookup("armor")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup("enhanced armor")))
	found = TRUE;
 
    if (check_dispel(level,victim,skill_lookup("bless")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup("blindness")))
    {
	found = TRUE;
	act("$n is no longer blinded.",victim,NULL,NULL,TO_ROOM);
    }

    if (check_dispel(level,victim,skill_lookup("calm")))
    {
	found = TRUE;
	act("$n no longer looks so peaceful...",victim,NULL,NULL,TO_ROOM);
    }

    if (check_dispel(level,victim,skill_lookup("change sex")))
    {
	found = TRUE;
	act("$n looks more like $mself again.",victim,NULL,NULL,TO_ROOM);
    }

    if (check_dispel(level,victim,skill_lookup("charm person")))
    {
        found = TRUE;
        act("$n regains $s free will.",victim,NULL,NULL,TO_ROOM);
    }
 
    if (check_dispel(level,victim,skill_lookup("chill touch")))
    {
        found = TRUE;
        act("$n looks warmer.",victim,NULL,NULL,TO_ROOM);
    }

    if (check_dispel(level,victim,skill_lookup("curse")))
        found = TRUE;
 
    if (check_dispel(level,victim,skill_lookup("detect evil")))
        found = TRUE;

    if (check_dispel(level,victim,skill_lookup("detect good")))
	found = TRUE;
 
    if (check_dispel(level,victim,skill_lookup("detect hidden")))
        found = TRUE;
 
    if (check_dispel(level,victim,skill_lookup("detect invis")))
        found = TRUE;
 
        found = TRUE;
 
    if (check_dispel(level,victim,skill_lookup("detect hidden")))
        found = TRUE;

    if (check_dispel(level,victim,skill_lookup("detect magic")))
        found = TRUE;
 
    if (check_dispel(level,victim,skill_lookup("faerie fire")))
    {
        act("$n's outline fades.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
 
    if (check_dispel(level,victim,skill_lookup("fly")))
    {
        act("$n falls to the ground!",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
 
    if (check_dispel(level,victim,skill_lookup("frenzy")))
    {
        act("$n no longer looks so wild.",victim,NULL,NULL,TO_ROOM);;
        found = TRUE;
    }
 
    if (check_dispel(level,victim,skill_lookup("giant strength")))
    {
        act("$n no longer looks so mighty.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
 
    if (check_dispel(level,victim,skill_lookup("haste")))
    {
        act("$n is no longer moving so quickly.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
 
    if (check_dispel(level,victim,skill_lookup("infravision")))
        found = TRUE;
 
    if (check_dispel(level,victim,skill_lookup("invis")))
    {
        act("$n fades into existance.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }
 
    if (check_dispel(level,victim,skill_lookup("mass invis")))
    {
        act("$n fades into existance.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup("pass door")))
        found = TRUE;
 

    if (check_dispel(level,victim,skill_lookup("protection evil")))
        found = TRUE;

    if (check_dispel(level,victim,skill_lookup("protection good")))
        found = TRUE;
 
    if (check_dispel(level,victim,skill_lookup("sanctuary")))
    {
        act("The white aura around $n's body vanishes.",
            victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }

    if (IS_AFFECTED(victim,AFF_SANCTUARY) 
	&& !saves_dispel(level, victim->level,-1)
	&& !is_affected(victim,skill_lookup("sanctuary"))
	&& !(victim->spec_fun == spec_special_guard
		|| victim->spec_fun == spec_stalker) )
    {
	REMOVE_BIT(victim->affected_by,AFF_SANCTUARY);
        act("The white aura around $n's body vanishes.",
            victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
 
    if (check_dispel(level,victim,skill_lookup("shield")))
    {
        act("The shield protecting $n vanishes.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
 
    if (check_dispel(level,victim,skill_lookup("sleep")))
        found = TRUE;

    if (check_dispel(level,victim,skill_lookup("slow")))
    {
        act("$n is no longer moving so slowly.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }
 
    if (check_dispel(level,victim,skill_lookup("stone skin")))
    {
        act("$n's skin regains its normal texture.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
 
    if (check_dispel(level,victim,skill_lookup("weaken")))
    {
        act("$n looks stronger.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }
 
    if (check_dispel(level,victim,skill_lookup("shielding")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup("web")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup("fear")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup("protection heat")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup("protection cold")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup("magic resistance")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup("hallucination")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup("terangreal")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup("power word stun")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup("corruption")))
    {
	act("$n looks healthier.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup("web")))
    {
	act("The webs around $n dissolve.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }

    if (found)
        send_to_char("Ok.\n\r",ch);
    else
        send_to_char("Spell failed.\n\r",ch);
	return;
}

void spell_earthquake( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    OBJ_DATA *obj, *obj_next, *corpse;

    send_to_char( "The earth trembles beneath your feet!\n\r", ch );
    act( "$n makes the earth tremble and shiver.", ch, NULL, NULL, TO_ROOM );

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
	    send_to_char( "The earth trembles and shivers.\n\r", vch );
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
		act("The earthquake reveals $p.\n\r",ch, corpse, NULL, TO_ALL );
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
	send_to_char("That isn't an armor.\n\r",ch);
	return;
    }

    if (obj->wear_loc != -1)
    {
	send_to_char("The item must be carried to be enchanted.\n\r",ch);
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
	act("$p flares blindingly... and evaporates!",ch,obj,NULL,TO_CHAR);
	act("$p flares blindingly... and evaporates!",ch,obj,NULL,TO_ROOM);
	extract_obj(obj);
	return;
    }

    if (result < (fail / 3)) /* item disenchanted */
    {
	AFFECT_DATA *paf_next;

	act("$p glows brightly, then fades...oops.",ch,obj,NULL,TO_CHAR);
	act("$p glows brightly, then fades.",ch,obj,NULL,TO_ROOM);
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
	send_to_char("Nothing seemed to happen.\n\r",ch);
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
	act("$p shimmers with a gold aura.",ch,obj,NULL,TO_CHAR);
	act("$p shimmers with a gold aura.",ch,obj,NULL,TO_ROOM);
	SET_BIT(obj->extra_flags, ITEM_MAGIC);
	added = -1;
    }
    
    else  /* exceptional enchant */
    {
	act("$p glows a brillant gold!",ch,obj,NULL,TO_CHAR);
	act("$p glows a brillant gold!",ch,obj,NULL,TO_ROOM);
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
	send_to_char("That isn't a weapon.\n\r",ch);
	return;
    }

    if (obj->wear_loc != -1)
    {
	send_to_char("The item must be carried to be enchanted.\n\r",ch);
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
	act("$p shivers violently and explodes!",ch,obj,NULL,TO_CHAR);
	act("$p shivers violently and explodeds!",ch,obj,NULL,TO_ROOM);
	extract_obj(obj);
	return;
    }

    if (result < (fail / 2)) /* item disenchanted */
    {
	AFFECT_DATA *paf_next;

	act("$p glows brightly, then fades...oops.",ch,obj,NULL,TO_CHAR);
	act("$p glows brightly, then fades.",ch,obj,NULL,TO_ROOM);
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
	send_to_char("Nothing seemed to happen.\n\r",ch);
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
	act("$p glows blue.",ch,obj,NULL,TO_CHAR);
	act("$p glows blue.",ch,obj,NULL,TO_ROOM);
	SET_BIT(obj->extra_flags, ITEM_MAGIC);
	added = 1;
    }
    
    else  /* exceptional enchant */
    {
	act("$p glows a brillant blue!",ch,obj,NULL,TO_CHAR);
	act("$p glows a brillant blue!",ch,obj,NULL,TO_ROOM);
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
	send_to_char("You feel a momentary chill.\n\r",victim);
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

    send_to_char("You feel your life slipping away!\n\r",victim);
    send_to_char("Wow....what a rush!\n\r",ch);
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
    char buf[MAX_STRING_LENGTH];
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
                do_yell(tmp_vict, "Help someone is attacking me!");
            else
              {
                 sprintf(buf,"Die, %s, you sorcerous dog!",
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
    char buf[MAX_STRING_LENGTH];
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
                do_yell(tmp_vict, "Help someone is attacking me!");
            else
              {
                 sprintf(buf,"Die, %s, you sorcerous dog!",
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
	act("$p is already protected from burning.",ch,obj,NULL,TO_CHAR);
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

    act("You protect $p from fire.",ch,obj,NULL,TO_CHAR);
    act("$p is surrounded by a protective aura.",ch,obj,NULL,TO_ROOM);
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
    send_to_char( "You are surrounded by a pink outline.\n\r", victim );
    act( "$n is surrounded by a pink outline.", victim, NULL, NULL, TO_ROOM );
    return;
}



void spell_faerie_fog( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *ich;

    act( "$n conjures a cloud of purple smoke.", ch, NULL, NULL, TO_ROOM );
    send_to_char( "You conjure a cloud of purple smoke.\n\r", ch );

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

        /* An elf sneaks eternally */
        if ( IS_NPC(ich) || !IS_SET(race_table[RACE(ich)].aff,AFF_SNEAK) )
          {
            affect_strip ( ich, gsn_sneak                       );
            REMOVE_BIT   ( ich->affected_by, AFF_SNEAK  );
          }

	act( "$n is revealed!", ich, NULL, NULL, TO_ROOM );
	send_to_char( "You are revealed!\n\r", ich );
    }

    return;
}

void spell_floating_disc( int sn, int level,CHAR_DATA *ch,void *vo,int target )
{
    OBJ_DATA *disc, *floating;

    floating = get_eq_char(ch,WEAR_FLOAT);
    if (floating != NULL && IS_OBJ_STAT(floating,ITEM_NOREMOVE))
    {
	act("You can't remove $p.",ch,floating,NULL,TO_CHAR);
	return;
    }

    disc = create_object(get_obj_index(OBJ_VNUM_DISC), 0);
    disc->value[0]	= ch->level * 10; /* 10 pounds per level capacity */
    disc->value[3]	= ch->level * 5; /* 5 pounds per level max per item */
    disc->timer		= ch->level * 2 - number_range(0,level / 2); 

    act("$n has created a floating black disc.",ch,NULL,NULL,TO_ROOM);
    send_to_char("You create a floating disc.\n\r",ch);
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
	  send_to_char("You are already airborne.\n\r",ch);
	else
	  act("$N doesn't need your help to fly.",ch,NULL,victim,TO_CHAR);
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
    send_to_char( "Your feet rise off the ground.\n\r", victim );
    act( "$n's feet rise off the ground.", victim, NULL, NULL, TO_ROOM );
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
	  send_to_char("You are already in a frenzy.\n\r",ch);
	else
	  act("$N is already in a frenzy.",ch,NULL,victim,TO_CHAR);
	return;
    }

    if (is_affected(victim,skill_lookup("calm")))
    {
	if (victim == ch)
	  send_to_char("Why don't you just relax for a while?\n\r",ch);
	else
	  act("$N doesn't look like $e wants to fight anymore.",
	      ch,NULL,victim,TO_CHAR);
	return;
    }

    if ((IS_GOOD(ch) && !IS_GOOD(victim)) ||
	(IS_NEUTRAL(ch) && !IS_NEUTRAL(victim)) ||
	(IS_EVIL(ch) && !IS_EVIL(victim))
       )
    {
	act("Your god doesn't seem to like $N",ch,NULL,victim,TO_CHAR);
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

    send_to_char("You are filled with holy wrath!\n\r",victim);
    act("$n gets a wild look in $s eyes!",victim,NULL,NULL,TO_ROOM);
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
    ||   (IS_NPC(victim) && saves_spell( level, victim,DAM_OTHER) ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }
    if (ch->pet != NULL && ch->in_room == ch->pet->in_room)
	gate_pet = TRUE;
    else
	gate_pet = FALSE;

    act("$n steps through a gate and vanishes.",ch,NULL,NULL,TO_ROOM);
    send_to_char("You step through a gate and vanish.\n\r",ch);
    char_from_room(ch);
    char_to_room(ch,victim->in_room);

    act("$n has arrived through a gate.",ch,NULL,NULL,TO_ROOM);
    do_look(ch,"auto");

    if (gate_pet)
    {
	act("$n steps through a gate and vanishes.",ch->pet,NULL,NULL,TO_ROOM);
	send_to_char("You step through a gate and vanish.\n\r",ch->pet);
	char_from_room(ch->pet);
	char_to_room(ch->pet,victim->in_room);
	act("$n has arrived through a gate.",ch->pet,NULL,NULL,TO_ROOM);
	do_look(ch->pet,"auto");
    }
}



void spell_giant_strength(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
    {
	if (victim == ch)
	  send_to_char("You are already as strong as you can get!\n\r",ch);
	else
	  act("$N can't get any stronger.",ch,NULL,victim,TO_CHAR);
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
    send_to_char( "Your muscles surge with heightened power!\n\r", victim );
    act("$n's muscles surge with heightened power.",victim,NULL,NULL,TO_ROOM);
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
	  send_to_char("You can't move any faster!\n\r",ch);
	else
	  act("$N is already moving as fast as $E can.",
	      ch,NULL,victim,TO_CHAR);
	return;
    }

    if (IS_AFFECTED(victim,AFF_SLOW))
    {
	if (!check_dispel(level,victim,skill_lookup("slow")))
	{
	    if (victim != ch)
		send_to_char("Spell failed.\n\r",ch);
	    send_to_char("You feel momentarily faster.\n\r",victim);
	    return;
	}
	act("$n is moving less slowly.",victim,NULL,NULL,TO_ROOM);
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
    send_to_char( "You feel yourself moving more quickly.\n\r", victim );
    act("$n is moving more quickly.",victim,NULL,NULL,TO_ROOM);
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_heal( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    victim->hit = UMIN( victim->hit + 100 + level / 10, victim->max_hit );
    update_pos( victim );
    send_to_char( "A warm feeling fills your body.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
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
			act("$n yelps and throws $p to the ground!",
			    victim,obj_lose,NULL,TO_ROOM);
			act("You remove and drop $p before it burns you.",
			    victim,obj_lose,NULL,TO_CHAR);
			dam += (number_range(1,obj_lose->level) / 3);
			obj_from_char(obj_lose);
			obj_to_room(obj_lose, victim->in_room);
			fail = FALSE;
		    }
		    else /* stuck on the body! ouch! */
		    {
			act("Your skin is seared by $p!",
			    victim,obj_lose,NULL,TO_CHAR);
			dam += (number_range(1,obj_lose->level));
			fail = FALSE;
		    }

		}
		else /* drop it if we can */
		{
		    if (can_drop_obj(victim,obj_lose))
		    {
			act("$n yelps and throws $p to the ground!",
			    victim,obj_lose,NULL,TO_ROOM);
			act("You and drop $p before it burns you.",
			    victim,obj_lose,NULL,TO_CHAR);
			dam += (number_range(1,obj_lose->level) / 6);
			obj_from_char(obj_lose);
			obj_to_room(obj_lose, victim->in_room);
			fail = FALSE;
		    }
		    else /* cannot drop */
		    {
			act("Your skin is seared by $p!",
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
			act("$n is burned by $p, and throws it to the ground.",
			    victim,obj_lose,NULL,TO_ROOM);
			send_to_char(
			    "You throw your red-hot weapon to the ground!\n\r",
			    victim);
			dam += 1;
			obj_from_char(obj_lose);
			obj_to_room(obj_lose,victim->in_room);
			fail = FALSE;
		    }
		    else /* YOWCH! */
		    {
			send_to_char("Your weapon sears your flesh!\n\r",
			    victim);
			dam += number_range(1,obj_lose->level);
			fail = FALSE;
		    }
		}
		else /* drop it if we can */
		{
		    if (can_drop_obj(victim,obj_lose))
		    {
			act("$n throws a burning hot $p to the ground!",
			    victim,obj_lose,NULL,TO_ROOM);
			act("You and drop $p before it burns you.",
			    victim,obj_lose,NULL,TO_CHAR);
			dam += (number_range(1,obj_lose->level) / 6);
			obj_from_char(obj_lose);
			obj_to_room(obj_lose, victim->in_room);
			fail = FALSE;
		    }
		    else /* cannot drop */
		    {
			act("Your skin is seared by $p!",
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
	send_to_char("Your spell had no effect.\n\r", ch);
	send_to_char("You feel momentarily warmer.\n\r",victim);
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
    char buf[MAX_STRING_LENGTH];
    int dam;
    int bless_num, curse_num, frenzy_num;

    bless_num = skill_lookup("bless");
    curse_num = skill_lookup("curse");
    frenzy_num = skill_lookup("frenzy");

    act("$n utters a word of divine power!",ch,NULL,NULL,TO_ROOM);
    send_to_char("You utter a word of divine power.\n\r",ch);

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
	vch_next = vch->next_in_room;

	if ((IS_GOOD(ch) && IS_GOOD(vch)) ||
	    (IS_EVIL(ch) && IS_EVIL(vch)) ||
	    (IS_NEUTRAL(ch) && IS_NEUTRAL(vch)) )
	{
	  send_to_char("You feel full more powerful.\n\r",vch);
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
                do_yell(vch, "Help someone is attacking me!");
            else
              {
                 sprintf(buf,"Die, %s, you sorcerous dog!",
                    (is_affected(ch,gsn_doppelganger)&&!IS_IMMORTAL(vch))?
                     ch->doppel->name : ch->name);
                 do_yell(vch,buf);
              }
          }

            spell_curse(curse_num,level,ch,(void *) vch, TARGET_CHAR);
            send_to_char("You are struck down!\n\r",vch);
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
                do_yell(vch, "Help someone is attacking me!");
            else
              {
                 sprintf(buf,"Die, %s, you sorcerous dog!",
                    (is_affected(ch,gsn_doppelganger)&&!IS_IMMORTAL(vch))?
                     ch->doppel->name : ch->name);
                 do_yell(vch,buf);
              }
          }

            spell_curse(curse_num,level/2,ch,(void *) vch, TARGET_CHAR);
            send_to_char("You are struck down!\n\r",vch);
            dam = dice(level,4);
            damage(ch,vch,dam,sn,DAM_ENERGY, TRUE);
	  }
	}
    }

    send_to_char("You feel drained.\n\r",ch);
    gain_exp( ch, -1 * number_range(1,10) * 5);
    ch->move /= (4/3);
    ch->hit /= (4/3);
}

void spell_identify( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    char buf[MAX_STRING_LENGTH];
    AFFECT_DATA *paf;

    sprintf( buf,
	"Object '%s' is type %s, extra flags %s.\n\rWeight is %d, value is %d, level is %d.\n\r",

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
		 sprintf(buf,
	"This equipment has been LIMITED by number %d \n\r", 
				obj->pIndexData->limit);
	 send_to_char(buf,ch);
		}

    switch ( obj->item_type )
    {
    case ITEM_SCROLL:
    case ITEM_POTION:
    case ITEM_PILL:
	sprintf( buf, "Level %d spells of:", obj->value[0] );
	send_to_char( buf, ch );

	if ( obj->value[1] >= 0 && obj->value[1] < MAX_SKILL )
	{
	    send_to_char( " '", ch );
	    send_to_char( skill_table[obj->value[1]].name, ch );
	    send_to_char( "'", ch );
	}

	if ( obj->value[2] >= 0 && obj->value[2] < MAX_SKILL )
	{
	    send_to_char( " '", ch );
	    send_to_char( skill_table[obj->value[2]].name, ch );
	    send_to_char( "'", ch );
	}

	if ( obj->value[3] >= 0 && obj->value[3] < MAX_SKILL )
	{
	    send_to_char( " '", ch );
	    send_to_char( skill_table[obj->value[3]].name, ch );
	    send_to_char( "'", ch );
	}

	if (obj->value[4] >= 0 && obj->value[4] < MAX_SKILL)
	{
	    send_to_char(" '",ch);
	    send_to_char(skill_table[obj->value[4]].name,ch);
	    send_to_char("'",ch);
	}

	send_to_char( ".\n\r", ch );
	break;

    case ITEM_WAND: 
    case ITEM_STAFF: 
	sprintf( buf, "Has %d charges of level %d",
	    obj->value[2], obj->value[0] );
	send_to_char( buf, ch );
      
	if ( obj->value[3] >= 0 && obj->value[3] < MAX_SKILL )
	{
	    send_to_char( " '", ch );
	    send_to_char( skill_table[obj->value[3]].name, ch );
	    send_to_char( "'", ch );
	}

	send_to_char( ".\n\r", ch );
	break;

    case ITEM_DRINK_CON:
        sprintf(buf,"It holds %s-colored %s.\n\r",
	    liq_table[obj->value[2]].liq_color,
            liq_table[obj->value[2]].liq_name);
        send_to_char(buf,ch);
        break;

    case ITEM_CONTAINER:
	sprintf(buf,"Capacity: %d#  Maximum weight: %d#  flags: %s\n\r",
	    obj->value[0], obj->value[3], cont_bit_name(obj->value[1]));
	send_to_char(buf,ch);
	if (obj->value[4] != 100)
	{
	    sprintf(buf,"Weight multiplier: %d%%\n\r",
		obj->value[4]);
	    send_to_char(buf,ch);
	}
	break;
		
    case ITEM_WEAPON:
 	send_to_char("Weapon type is ",ch);
	switch (obj->value[0])
	{
	    case(WEAPON_EXOTIC) : send_to_char("exotic.\n\r",ch);	break;
	    case(WEAPON_SWORD)  : send_to_char("sword.\n\r",ch);	break;	
	    case(WEAPON_DAGGER) : send_to_char("dagger.\n\r",ch);	break;
	    case(WEAPON_SPEAR)	: send_to_char("spear/staff.\n\r",ch);	break;
	    case(WEAPON_MACE) 	: send_to_char("mace/club.\n\r",ch);	break;
	    case(WEAPON_AXE)	: send_to_char("axe.\n\r",ch);		break;
	    case(WEAPON_FLAIL)	: send_to_char("flail.\n\r",ch);	break;
	    case(WEAPON_WHIP)	: send_to_char("whip.\n\r",ch);		break;
	    case(WEAPON_POLEARM): send_to_char("polearm.\n\r",ch);	break;
	    case(WEAPON_BOW)	: send_to_char("bow.\n\r",ch);		break;
	    case(WEAPON_ARROW)	: send_to_char("arrow.\n\r",ch);	break;
	    case(WEAPON_LANCE)	: send_to_char("lance.\n\r",ch);	break;
	    default		: send_to_char("unknown.\n\r",ch);	break;
 	}
	if (obj->pIndexData->new_format)
	    sprintf(buf,"Damage is %dd%d (average %d).\n\r",
		obj->value[1],obj->value[2],
		(1 + obj->value[2]) * obj->value[1] / 2);
	else
	    sprintf( buf, "Damage is %d to %d (average %d).\n\r",
	    	obj->value[1], obj->value[2],
	    	( obj->value[1] + obj->value[2] ) / 2 );
	send_to_char( buf, ch );
        if (obj->value[4])  /* weapon flags */
        {
            sprintf(buf,"Weapons flags: %s\n\r",weapon_bit_name(obj->value[4]));
            send_to_char(buf,ch);
	}
	break;

    case ITEM_ARMOR:
	sprintf( buf, 
	"Armor class is %d pierce, %d bash, %d slash, and %d vs. magic.\n\r", 
	    obj->value[0], obj->value[1], obj->value[2], obj->value[3] );
	send_to_char( buf, ch );
	break;
    }

    if (!obj->enchanted)
    for ( paf = obj->pIndexData->affected; paf != NULL; paf = paf->next )
    {
	if ( paf->location != APPLY_NONE && paf->modifier != 0 )
	{
	    sprintf( buf, "Affects %s by %d.\n\r",
		affect_loc_name( paf->location ), paf->modifier );
	    send_to_char(buf,ch);
            if (paf->bitvector)
	    {
                switch(paf->where)
                {
                    case TO_AFFECTS:
                        sprintf(buf,"Adds %s affect.\n",
                            affect_bit_name(paf->bitvector));
                        break;
                    case TO_OBJECT:
                        sprintf(buf,"Adds %s object flag.\n",
                            extra_bit_name(paf->bitvector));
                        break;
                    case TO_IMMUNE:
                        sprintf(buf,"Adds immunity to %s.\n",
                            imm_bit_name(paf->bitvector));
                        break;
		    case TO_RESIST:
                        sprintf(buf,"Adds resistance to %s.\n\r",
                            imm_bit_name(paf->bitvector));
                        break;
                    case TO_VULN:
			sprintf(buf,"Adds vulnerability to %s.\n\r",
                            imm_bit_name(paf->bitvector));
                        break;
                    case TO_DETECTS:
			sprintf(buf,"Adds %s detection.\n\r",
                            detect_bit_name(paf->bitvector));
                        break;
                    default:
                        sprintf(buf,"Unknown bit %d: %d\n\r",
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
	    sprintf( buf, "Affects %s by %d",
	    	affect_loc_name( paf->location ), paf->modifier );
	    send_to_char( buf, ch );
	    if ( paf->duration > -1)
                sprintf(buf,", %d hours.\n\r",paf->duration);
            else
                sprintf(buf,".\n\r");
	    send_to_char(buf,ch);

            if (paf->bitvector)
            {
                switch(paf->where)
                {
                    case TO_AFFECTS:
                        sprintf(buf,"Adds %s affect.\n",
                            affect_bit_name(paf->bitvector));
                        break;
                    case TO_OBJECT:
                        sprintf(buf,"Adds %s object flag.\n",
			    extra_bit_name(paf->bitvector));
                        break;
		    case TO_WEAPON:
			sprintf(buf,"Adds %s weapon flags.\n",
			    weapon_bit_name(paf->bitvector));
			break;
                    case TO_IMMUNE:
                        sprintf(buf,"Adds immunity to %s.\n",
                            imm_bit_name(paf->bitvector));
                        break;
                    case TO_RESIST:
                        sprintf(buf,"Adds resistance to %s.\n\r",
                            imm_bit_name(paf->bitvector));
                        break;
                    case TO_VULN:
                        sprintf(buf,"Adds vulnerability to %s.\n\r",
                            imm_bit_name(paf->bitvector));
                        break;
                    case TO_DETECTS:
                        sprintf(buf,"Adds %s detection.\n\r",
                            detect_bit_name(paf->bitvector));
                        break;
                    default:
                        sprintf(buf,"Unknown bit %d: %d\n\r",
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
	  send_to_char("You can already see in the dark.\n\r",ch);
	else
	  act("$N already has infravision.\n\r",ch,NULL,victim,TO_CHAR);
	return;
    }
    act( "$n's eyes glow red.\n\r", ch, NULL, NULL, TO_ROOM );

    af.where	 = TO_AFFECTS;
    af.type      = sn;
    af.level	 = level;
    af.duration  = 2 * level;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_INFRARED;
    affect_to_char( victim, &af );
    send_to_char( "Your eyes glow red.\n\r", victim );
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
	    act("$p is already invisible.",ch,obj,NULL,TO_CHAR);
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

	act("$p fades out of sight.",ch,obj,NULL,TO_ALL);
	return;
    }

    /* character invisibility */
    victim = (CHAR_DATA *) vo;

    if ( IS_AFFECTED(victim, AFF_INVISIBLE) )
	return;

    act( "$n fades out of existence.", victim, NULL, NULL, TO_ROOM );

    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = (level / 8 + 10);
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_INVISIBLE;
    affect_to_char( victim, &af );
    send_to_char( "You fade out of existence.\n\r", victim );
    return;
}



void spell_know_alignment(int sn,int level,CHAR_DATA *ch,void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    char *msg;

	 if ( IS_GOOD(victim) ) msg = "$N has a pure and good aura.";
    else if ( IS_NEUTRAL(victim) ) msg = "$N act as no align.";
    else msg = "$N is the embodiment of pure evil!.";

    act( msg, ch, NULL, victim, TO_CHAR );

    if (!IS_NPC(victim)) 
    {
     if (victim->ethos == 1)		msg = "$N upholds the laws.";
     else if (victim->ethos == 2) 	msg = "$N seems ambivalent to society.";
     else if (victim->ethos == 3) 	msg = "$N seems very chaotic.";
     else msg = "$N doesn't know where they stand on the laws.";
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
	send_to_char("The electricity fizzles at your foes.\n\r",victim);
	act("A lightning bolt fizzles at $N's foes.\n\r",
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
    char buf[MAX_INPUT_LENGTH];
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
	    sprintf( buf, "one is carried by %s\n\r",
		PERS(in_obj->carried_by, ch) );
	}
	else
	{
	    if (IS_IMMORTAL(ch) && in_obj->in_room != NULL)
		sprintf( buf, "one is in %s [Room %d]\n\r",
		    in_obj->in_room->name, in_obj->in_room->vnum);
	    else
		sprintf( buf, "one is in %s\n\r",
		    in_obj->in_room == NULL
			? "somewhere" : in_obj->in_room->name );
	}

	buf[0] = UPPER(buf[0]);
	add_buf(buffer,buf);

	if (number >= max_found)
	    break;
    }

    if ( !found )
	send_to_char( "Nothing like that in heaven or earth.\n\r", ch );
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
	  send_to_char( "Your magic missiles fizzle out near your victim.\n\r", ch );
          act( "Your shield blocks $N's magic missiles.", victim, NULL, ch, TO_CHAR);
        }
	else  {
	  send_to_char( "Your magic missile fizzle out near your victim.\n\r", ch );
          act( "Your shield blocks $N's magic missile.", victim, NULL, ch, TO_CHAR);
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

    heal_num = skill_lookup("heal");
    refresh_num = skill_lookup("refresh");

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
	act( "$n slowly fades out of existence.", gch, NULL, NULL, TO_ROOM );
	send_to_char( "You slowly fade out of existence.\n\r", gch );

	af.where     = TO_AFFECTS;
	af.type      = sn;
	af.level     = level/2;
	af.duration  = 24;
	af.location  = APPLY_NONE;
	af.modifier  = 0;
	af.bitvector = AFF_INVISIBLE;
	affect_to_char( gch, &af );
    }
    send_to_char( "Ok.\n\r", ch );

    return;
}



void spell_null( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    send_to_char( "That's not a spell!\n\r", ch );
    return;
}



void spell_pass_door( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_PASS_DOOR) )
    {
	if (victim == ch)
	  send_to_char("You are already out of phase.\n\r",ch);
	else
	  act("$N is already shifted out of phase.",ch,NULL,victim,TO_CHAR);
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
    act( "$n turns translucent.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "You turn translucent.\n\r", victim );
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
	  send_to_char("You feel momentarily ill, but it passes.\n\r",ch);
	else
	  act("$N seems to be unaffected.",ch,NULL,victim,TO_CHAR);
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

    send_to_char
      ("You scream in agony as plague sores erupt from your skin.\n\r",victim);
    act("$n screams in agony as plague sores erupt from $s skin.",
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
		act("Your spell fails to corrupt $p.",ch,obj,NULL,TO_CHAR);
		return;
	    }
	    obj->value[3] = 1;
	    act("$p is infused with poisonous vapors.",ch,obj,NULL,TO_ALL);
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
		act("You can't seem to envenom $p.",ch,obj,NULL,TO_CHAR);
		return;
	    }

	    if (IS_WEAPON_STAT(obj,WEAPON_POISON))
	    {
		act("$p is already envenomed.",ch,obj,NULL,TO_CHAR);
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

	    act("$p is coated with deadly venom.",ch,obj,NULL,TO_ALL);
	    return;
	}

	act("You can't poison $p.",ch,obj,NULL,TO_CHAR);
	return;
    }

    victim = (CHAR_DATA *) vo;

    if ( saves_spell( level, victim,DAM_POISON) )
    {
	act("$n turns slightly green, but it passes.",victim,NULL,NULL,TO_ROOM);
	send_to_char("You feel momentarily ill, but it passes.\n\r",victim);
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
    send_to_char( "You feel very sick.\n\r", victim );
    act("$n looks very ill.",victim,NULL,NULL,TO_ROOM);
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
	  send_to_char("You are already protected.\n\r",ch);
	else
	  act("$N is already protected.",ch,NULL,victim,TO_CHAR);
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
    send_to_char( "You feel holy and pure.\n\r", victim );
    if ( ch != victim )
	act("$N is protected from evil.",ch,NULL,victim,TO_CHAR);
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
	  send_to_char("You are already protected.\n\r",ch);
	else
	  act("$N is already protected.",ch,NULL,victim,TO_CHAR);
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
    send_to_char( "You feel aligned with darkness.\n\r", victim );
    if ( ch != victim )
	act("$N is protected from good.",ch,NULL,victim,TO_CHAR);
    return;
}


void spell_ray_of_truth (int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam, align;

    if (IS_EVIL(ch) )
    {
	victim = ch;
	send_to_char("The energy explodes inside you!\n\r",ch);
    }

    if (victim != ch)
    {
	act("$n raises $s hand, and a blinding ray of light shoots forth!",
	    ch,NULL,NULL,TO_ROOM);
	send_to_char(
	   "You raise your hand and a blinding ray of light shoots forth!\n\r",
	   ch);
    }

    if (IS_GOOD(victim))
    {
	act("$n seems unharmed by the light.",victim,NULL,victim,TO_ROOM);
	send_to_char("The light seems powerless to affect you.\n\r",victim);
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
	send_to_char("That item does not carry charges.\n\r",ch);
	return;
    }

    if (obj->value[3] >= 3 * level / 2)
    {
	send_to_char("Your skills are not great enough for that.\n\r",ch);
	return;
    }

    if (obj->value[1] == 0)
    {
	send_to_char("That item has already been recharged once.\n\r",ch);
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
	act("$p glows softly.",ch,obj,NULL,TO_CHAR);
	act("$p glows softly.",ch,obj,NULL,TO_ROOM);
	obj->value[2] = UMAX(obj->value[1],obj->value[2]);
	obj->value[1] = 0;
	return;
    }

    else if (percent <= chance)
    {
	int chargeback,chargemax;

	act("$p glows softly.",ch,obj,NULL,TO_CHAR);
	act("$p glows softly.",ch,obj,NULL,TO_CHAR);

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
	send_to_char("Nothing seems to happen.\n\r",ch);
	if (obj->value[1] > 1)
	    obj->value[1]--;
	return;
    }

    else /* whoops! */
    {
	act("$p glows brightly and explodes!",ch,obj,NULL,TO_CHAR);
	act("$p glows brightly and explodes!",ch,obj,NULL,TO_ROOM);
	extract_obj(obj);
    }
}

void spell_refresh( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    victim->move = UMIN( victim->move + level, victim->max_move );
    if (victim->max_move == victim->move)
	send_to_char("You feel fully refreshed!\n\r",victim);
    else
	send_to_char( "You feel less tired.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
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
		act("$p glows blue.",ch,obj,NULL,TO_ALL);
		return;
	    }

	    act("The curse on $p is beyond your power.",ch,obj,NULL,TO_CHAR);
	    return;
	}
	else  {
	  send_to_char( "Nothing happens...\n\r", ch );
	  return;
	}
    }

    /* characters */
    victim = (CHAR_DATA *) vo;

    if (check_dispel(level,victim,gsn_curse))
    {
	send_to_char("You feel better.\n\r",victim);
	act("$n looks more relaxed.",victim,NULL,NULL,TO_ROOM);
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
		act("Your $p glows blue.",victim,obj,NULL,TO_CHAR);
		act("$n's $p glows blue.",victim,obj,NULL,TO_ROOM);
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
	  send_to_char("You are already in sanctuary.\n\r",ch);
	else
	  act("$N is already in sanctuary.",ch,NULL,victim,TO_CHAR);
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
    act( "$n is surrounded by a white aura.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "You are surrounded by a white aura.\n\r", victim );
    return;
}



void spell_shield( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
    {
	if (victim == ch)
	  send_to_char("You are already shielded from harm.\n\r",ch);
	else
	  act("$N is already protected by a shield.",ch,NULL,victim,TO_CHAR);
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
    act( "$n is surrounded by a force shield.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "You are surrounded by a force shield.\n\r", victim );
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
	send_to_char( "You feel very sleepy ..... zzzzzz.\n\r", victim );
	act( "$n goes to sleep.", victim, NULL, NULL, TO_ROOM );
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
	  send_to_char("You can't move any slower!\n\r",ch);
	else
	  act("$N can't get any slower than that.",
	      ch,NULL,victim,TO_CHAR);
	return;
    }

    if (saves_spell(level,victim,DAM_OTHER)
    ||  IS_SET(victim->imm_flags,IMM_MAGIC))
    {
	if (victim != ch)
	    send_to_char("Nothing seemed to happen.\n\r",ch);
	send_to_char("You feel momentarily lethargic.\n\r",victim);
	return;
    }

    if (IS_AFFECTED(victim,AFF_HASTE))
    {
	if (!check_dispel(level,victim,skill_lookup("haste")))
	{
	    if (victim != ch)
		send_to_char("Spell failed.\n\r",ch);
	    send_to_char("You feel momentarily slower.\n\r",victim);
	    return;
	}

	act("$n is moving less quickly.",victim,NULL,NULL,TO_ROOM);
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
    send_to_char( "You feel yourself slowing d o w n...\n\r", victim );
    act("$n starts to move in slow motion.",victim,NULL,NULL,TO_ROOM);
    return;
}




void spell_stone_skin( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( ch, sn ) )
    {
	if (victim == ch)
	  send_to_char("Your skin is already as hard as a rock.\n\r",ch);
	else
	  act("$N is already as hard as can be.",ch,NULL,victim,TO_CHAR);
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
    act( "$n's skin turns to stone.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "Your skin turns to stone.\n\r", victim );
    return;
}



void spell_summon( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim;
    char buf[MAX_INPUT_LENGTH];

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
    ||   (victim->in_room->exit[0] == NULL &&
          victim->in_room->exit[1] == NULL &&
          victim->in_room->exit[2] == NULL &&
          victim->in_room->exit[3] == NULL &&
          victim->in_room->exit[4] == NULL && victim->in_room->exit[5] == NULL) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if (IS_NPC(victim) && victim->in_mind == NULL)
	{
	 sprintf(buf,"%d",victim->in_room->vnum);
	 victim->in_mind = str_dup( buf );
	}

    act( "$n disappears suddenly.", victim, NULL, NULL, TO_ROOM );
    char_from_room( victim );
    char_to_room( victim, ch->in_room );
    act( "$n arrives suddenly.", victim, NULL, NULL, TO_ROOM );
    act( "$n has summoned you!", ch, NULL, victim,   TO_VICT );
    do_look( victim, "auto" );
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
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    pRoomIndex = get_random_room(victim);

    if (victim != ch)
	send_to_char("You have been teleported!\n\r",victim);

    act( "$n vanishes!", victim, NULL, NULL, TO_ROOM );
    char_from_room( victim );
    char_to_room( victim, pRoomIndex );
    act( "$n slowly fades into existence.", victim, NULL, NULL, TO_ROOM );
    do_look( victim, "auto" );
    return;
}



void spell_ventriloquate( int sn, int level, CHAR_DATA *ch,void *vo,int target)
{
    char buf1[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char speaker[MAX_INPUT_LENGTH];
    CHAR_DATA *vch;

    target_name = one_argument( target_name, speaker );

    sprintf( buf1, "%s says '%s'.\n\r",              speaker, target_name );
    sprintf( buf2, "Someone makes %s say '%s'.\n\r", speaker, target_name );
    buf1[0] = UPPER(buf1[0]);

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
    send_to_char( "You feel your strength slip away.\n\r", victim );
    act("$n looks tired and weak.",victim,NULL,NULL,TO_ROOM);
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
	 send_to_char("Your honour doesn't let you recall!.\n\r",ch);
	 return;
	}

    if (victim != NULL)
    {
        if  ((victim->fighting) && (victim->iclass == 9))
	{
	 send_to_char("You can't cast this spell to a honourable fighting Samurai!.\n\r",ch);
	 return;
	}
    }

    if (IS_NPC(victim))
      return;
   to_room_vnum = 
hometown_table[victim->hometown].recall[IS_GOOD(victim)?0:IS_NEUTRAL(victim)?1:IS_EVIL(victim)?2:1];

    if ((location = get_room_index( to_room_vnum )) == NULL)
    {
	send_to_char("You are completely lost.\n\r",victim);
	return;
    }
/*
    if (victim->desc != NULL && 
	(current_time - victim->last_fight_time) < FIGHT_DELAY_TIME)
      {
	send_to_char("You are too pumped to pray now.\n\r",victim);
	return;
      }
*/
    if (IS_SET(victim->in_room->room_flags,ROOM_NO_RECALL) ||
	IS_AFFECTED(victim,AFF_CURSE) ||
	IS_RAFFECTED(victim->in_room,AFF_ROOM_CURSE))
    {
	send_to_char("Spell failed.\n\r",victim);
	return;
    }

    if (victim->fighting != NULL)
    {
      if (victim == ch)  gain_exp(victim,0 -(victim->level + 25) );
	stop_fighting(victim,TRUE);
    }

    ch->move /= 2;
    act("$n disappears.",victim,NULL,NULL,TO_ROOM);
    char_from_room(victim);
    char_to_room(victim,location);
    do_visible(ch,"");
    act("$n appears in the room.",victim,NULL,NULL,TO_ROOM);
    do_look(victim,"auto");

    if (victim->pet != NULL)
      {
 	char_from_room( victim->pet );
	char_to_room( victim->pet, location );
	do_look(victim->pet, "auto" );
      }

}

/*
 * Draconian spells.
 */
void spell_acid_breath( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam,hp_dam,dice_dam,hpch;

    act("$n spits acid at $N.",ch,NULL,victim,TO_NOTVICT);
    act("$n spits a stream of corrosive acid at you.",ch,NULL,victim,TO_VICT);
    act("You spit acid at $N.",ch,NULL,victim,TO_CHAR);

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

    act("$n breathes forth a cone of fire.",ch,NULL,victim,TO_NOTVICT);
    act("$n breathes a cone of hot fire over you!",ch,NULL,victim,TO_VICT);
    act("You breath forth a cone of fire.",ch,NULL,NULL,TO_CHAR);

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

    act("$n breathes out a freezing cone of frost!",ch,NULL,victim,TO_NOTVICT);
    act("$n breathes a freezing cone of frost over you!",
	ch,NULL,victim,TO_VICT);
    act("You breath out a cone of frost.",ch,NULL,NULL,TO_CHAR);

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
    char buf[MAX_STRING_LENGTH];
    int dam,hp_dam,dice_dam,hpch;

    act("$n breathes out a cloud of poisonous gas!",ch,NULL,NULL,TO_ROOM);
    act("You breath out a cloud of poisonous gas.",ch,NULL,NULL,TO_CHAR);

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
                do_yell(vch, "Help someone is attacking me!");
            else
              {
                 sprintf(buf,"Die, %s, you sorcerous dog!",
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

    act("$n breathes a bolt of lightning at $N.",ch,NULL,victim,TO_NOTVICT);
    act("$n breathes a bolt of lightning at you!",ch,NULL,victim,TO_VICT);
    act("You breathe a bolt of lightning at $N.",ch,NULL,victim,TO_CHAR);

    hpch = UMAX(10,ch->hit);
    hp_dam = number_range(hpch/9+1,hpch/5);
    dice_dam = dice(level,20);

    dam = UMAX(hp_dam + dice_dam/10,dice_dam + hp_dam/10);

    if (CAN_DETECT(victim, ADET_GROUNDING))
    {
	send_to_char("The electricity fizzles at your foes.\n\r",victim);
	act("A lightning bolt fizzles at $N's foes.\n\r",
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
    char buf[MAX_INPUT_LENGTH];
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
	    sprintf( buf, "one is carried by %s\n\r",
		PERS(in_obj->carried_by, ch) );
	}
	else
	{
	    if (IS_IMMORTAL(ch) && in_obj->in_room != NULL)
		sprintf( buf, "one is in %s [Room %d]\n\r",
		    in_obj->in_room->name, in_obj->in_room->vnum);
	    else
		sprintf( buf, "one is in %s\n\r",
		    in_obj->in_room == NULL
			? "somewhere" : in_obj->in_room->name );
	}

	buf[0] = UPPER(buf[0]);
	add_buf(buffer,buf);

	if (number >= max_found)
	    break;
    }

    if ( !found )
	send_to_char( "Nothing like that in heaven or earth.\n\r", ch );
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
	send_to_char("This room has already shielded.\n\r",ch);
	return;
    }

    if ( is_affected(ch,sn))
    {
	send_to_char("This spell is used too recently.\n\r",ch);
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
    send_to_char( "The room starts to be filled with lightnings.\n\r", ch );
    act("The room starts to be filled with $n's lightnings.",ch,NULL,NULL,TO_ROOM);
    return;
}

void spell_shocking_trap(int sn, int level, CHAR_DATA *ch, void *vo,int target ) 
{
    AFFECT_DATA af,af2;

    if ( is_affected_room( ch->in_room, sn ))
    {
	send_to_char("This room has already trapped with shocks waves.\n\r",ch);
	return;
    }

    if ( is_affected(ch,sn))
    {
	send_to_char("This spell is used too recently.\n\r",ch);
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
    send_to_char( "The room starts to be filled with shock waves.\n\r", ch );
    act("The room starts to be filled with $n's shock waves.",ch,NULL,NULL,TO_ROOM);
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
    act("A fist of black, otherworldly ether rams into $N, leaving $M looking stunned!"
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
    act("The fabric of the cosmos strains in fury about $N!",
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
    act("A weird energy encompasses $N, causing you to question $S continued existence.",
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
    act("A cylinder of kinetic energy enshrouds $N causing $S to resonate.",
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
    act("$n stares intently at $N, causing $N to seem very lethargic.",
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
    act("$n stares intently at $N, causing $N to seem very hyperactive.",
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
    act("A stinking spray of sulfurous liquid rains down on $N." ,
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
    act("A fountain of caustic liquid forms below $N.  The smell of $S degenerating tissues is revolting! ",
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
    act("A cloak of primal acid enshrouds $N, sparks form as it consumes all it touches. ",
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
    act("$n conjures a whip of ionized particles, which lashes ferociously at $N.",
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
    act("An unseen energy moves nearby, causing your hair to stand on end!",
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
    act("$N seems to dissolve into tiny unconnected particles, then is painfully reassembled.",
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
	send_to_char("You feel a momentary chill.\n\r",victim);
	return;
    }

    if ( (IS_NPC(victim) && IS_SET(victim->act,ACT_UNDEAD)) 
	|| IS_VAMPIRE(victim) )
	{
	 send_to_char("Your victim is unaffected by hand of undead.\n\r",ch);
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

    send_to_char("You feel your life slipping away!\n\r",victim);
    act("$N is grasped by an incomprehensible hand of undead!",
		ch,NULL,victim,TO_NOTVICT);
    damage( ch, victim, dam, sn,DAM_NEGATIVE,TRUE);
    return;
}


/* travel via astral plains */
void spell_astral_walk( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim;
    bool gate_pet;
    char buf[512];


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
    ||   (IS_NPC(victim) && saves_spell( level, victim,DAM_OTHER) ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }
    if (ch->pet != NULL && ch->in_room == ch->pet->in_room)
	gate_pet = TRUE;
    else
	gate_pet = FALSE;


    act("$n disappears in a flash of light!",ch,NULL,NULL,TO_ROOM);
    sprintf(buf,"You travel via astral planes and go to %s.\n\r",victim->name);
    send_to_char(buf,ch);
    char_from_room(ch);
    char_to_room(ch,victim->in_room);

    act("$n appears in a flash of light!",ch,NULL,NULL,TO_ROOM);
    do_look(ch,"auto");

    if (gate_pet)
    {
	act("$n disappears in a flash of light!",ch->pet,NULL,NULL,TO_ROOM);
	send_to_char(buf,ch->pet);
	char_from_room(ch->pet);
	char_to_room(ch->pet,victim->in_room);
	act("$n appears in a flash of light!",ch->pet,NULL,NULL,TO_ROOM);
	do_look(ch->pet,"auto");
    }
}


/* vampire version astral walk */
void spell_mist_walk( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim;


    if ( ( victim = get_char_world( ch, target_name ) ) == NULL
    ||   victim == ch
    ||   !IS_VAMPIRE(ch)
    ||   victim->in_room == NULL
    ||   !can_see_room(ch,victim->in_room)
    ||   IS_SET(victim->in_room->room_flags, ROOM_SAFE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_PRIVATE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_SOLITARY)
    ||   IS_SET(ch->in_room->room_flags, ROOM_NOSUMMON)
    ||   IS_SET(victim->in_room->room_flags, ROOM_NOSUMMON)
    ||   victim->level >= level - 5
/*    ||   (!IS_NPC(victim) && victim->level >= LEVEL_HERO)  * NOT trust */
    ||   saves_spell(level,victim,DAM_OTHER)
    ||   (IS_NPC(victim) && is_safe_nomessage(ch, victim) && IS_SET(victim->imm_flags,IMM_SUMMON))
    ||   (!IS_NPC(victim) && is_safe_nomessage(ch, victim) && IS_SET(victim->act,PLR_NOSUMMON))
    ||   (!IS_NPC(victim) && ch->in_room->area != victim->in_room->area )
    ||   (IS_NPC(victim) && saves_spell( level, victim,DAM_OTHER) ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    act("$n dissolves into a cloud of glowing mist, then vanishes!",ch,NULL,NULL,TO_ROOM);
    send_to_char("You dissolve into a cloud of glowing mist, then flow to your target.\n\r",ch);

    char_from_room(ch);
    char_to_room(ch,victim->in_room);

    act("A cloud of glowing mist engulfs you, then withdraws to unveil $n!",ch,NULL,NULL,TO_ROOM);
    do_look(ch,"auto");

}

/*  Cleric version of astra_walk  */
void spell_solar_flight( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim;


    if  (time_info.hour > 18 || time_info.hour < 8)
	{
	 send_to_char("You need sunlight for solar flight.\n\r",ch);
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
    ||   (!IS_NPC(victim) && ch->in_room->area != victim->in_room->area )
    ||   (IS_NPC(victim) && saves_spell( level, victim,DAM_OTHER) ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    act("$n disappears in a blinding flash of light!",ch,NULL,NULL,TO_ROOM);
    send_to_char("You dissolve in a blinding flash of light!.\n\r",ch);

    char_from_room(ch);
    char_to_room(ch,victim->in_room);

    act("$n appears in a blinding flash of light!",ch,NULL,NULL,TO_ROOM);
    do_look(ch,"auto");

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
    ||   (!IS_NPC(victim) && ch->in_room->area != victim->in_room->area )
    ||   (IS_NPC(victim) && saves_spell( level, victim,DAM_OTHER) ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    act("$n coils into an ascending column of colour, vanishing into thin air.",ch,NULL,NULL,TO_ROOM);
    send_to_char("You coils into an ascending column of colour, and vanishing into thin air.\n\r",ch);

    char_from_room(ch);
    char_to_room(ch,victim->in_room);

    act("A coil of colours descends from above, revealing $n as it dissipates.",ch,NULL,NULL,TO_ROOM);
    do_look(ch,"auto");

}



void spell_corruption( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if (IS_AFFECTED(victim,AFF_CORRUPTION))
	{
	 act("$N is already corrupting.\n\r",ch,NULL,victim,TO_CHAR);
	 return;
	}

    if (saves_spell(level,victim,DAM_NEGATIVE) ||
	(IS_NPC(victim) && IS_SET(victim->act,ACT_UNDEAD)))
    {
	if (ch == victim)
	  send_to_char("You feel momentarily ill, but it passes.\n\r",ch);
	else
	  act("$N seems to be unaffected.",ch,NULL,victim,TO_CHAR);
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

    send_to_char
      ("You scream in agony as you start to decay into dust.\n\r",victim);
    act("$n screams in agony as $n start to decay into dust.",
	victim,NULL,NULL,TO_ROOM);
}


void spell_hurricane(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    char buf[MAX_STRING_LENGTH];
    int dam,hp_dam,dice_dam,hpch;

    act("$n prays the gods of the storm for help.",ch,NULL,NULL,TO_NOTVICT);
    act("You pray the gods of the storm to help you.",ch,NULL,NULL,TO_CHAR);

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
                do_yell(vch, "Help someone is attacking me!");
            else
              {
                 sprintf(buf,"Die, %s, you sorcerous dog!",
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
	  send_to_char("You can already sense undead.\n\r",ch);
	else
	  act("$N can already detect undead.",ch,NULL,victim,TO_CHAR);
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
    send_to_char( "Your eyes tingle.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
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
    send_to_char("It is too late to take revenge.\n\r",ch);
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
	send_to_char( "Unluckily your corpse is devoured.\n\r", ch );
    else
      {
	ROOM_INDEX_DATA *prev_room;

	prev_room = ch->in_room;
        act("$n prays for transportation.", ch, NULL, NULL, TO_ROOM);
	char_from_room( ch );
	char_to_room(ch, room);
	if (cabal_area_check( ch )
		|| IS_ROOM_AFFECTED(ch->in_room,AFF_ROOM_PREVENT) )
	{
	  send_to_char(" You failed to go to your corpse.\n\r", ch);
	  char_from_room( ch );
	  char_to_room(ch, prev_room);
	}
	else do_look(ch,"auto");
      }
    return;
}



void spell_firestream( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    act("$n throws a stream of searing flames.",ch,NULL,victim,TO_NOTVICT);
    act("$n throws a stream of hot flames over you!",ch,NULL,victim,TO_VICT);
    act("You throw a stream of searing flames to $N.",ch,NULL,victim,TO_CHAR);

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
      send_to_char("You lack the power to create another elemental right now.\n\r",
                   ch);
      return;
    }

  send_to_char("You attempt to create an earth elemental.\n\r",ch);
  act("$n attempts to create an earth elemental.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
          ( gch->pIndexData->vnum == MOB_VNUM_ELM_EARTH ) )
        {
          i++;
          if (i > 2)
           {
            send_to_char("More earth elementals are more than you can control!\n\r",ch);
            return;
           }
        }
    }

  if (count_charmed(ch)) return;

  elm = create_mobile( get_mob_index(MOB_VNUM_ELM_EARTH) );


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
  elm->gold = 0;
  elm->timer = 0;
  elm->damage[DICE_NUMBER] = 3;   
  elm->damage[DICE_TYPE] = 10;
  elm->damage[DICE_BONUS] = ch->level / 2;

  char_to_room(elm,ch->in_room);
  send_to_char("You created an earth elemental!\n\r",ch);
  act("$n creates an earth elemental!",ch,NULL,NULL,TO_ROOM);

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
      send_to_char("You lack the power to create another elemental right now.\n\r",
                   ch);
      return;
    }

  send_to_char("You attempt to create an air elemental.\n\r",ch);
  act("$n attempts to create an air elemental.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
          ( gch->pIndexData->vnum == MOB_VNUM_ELM_AIR ) )
        {
          i++;
          if (i > 2)
           {
            send_to_char("More air elementals are more than you can control!\n\r",ch);
            return;
           }
        }
    }

  if (count_charmed(ch)) return;

  elm = create_mobile( get_mob_index(MOB_VNUM_ELM_AIR) );


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
  elm->gold = 0;
  elm->timer = 0;
  elm->damage[DICE_NUMBER] = 7;   
  elm->damage[DICE_TYPE] = 4;
  elm->damage[DICE_BONUS] = ch->level / 2;

  char_to_room(elm,ch->in_room);
  send_to_char("You created an air elemental!\n\r",ch);
  act("$n creates an air elemental!",ch,NULL,NULL,TO_ROOM);

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
      send_to_char("You lack the power to create another elemental right now.\n\r",
                   ch);
      return;
    }

  send_to_char("You attempt to create a water elemental.\n\r",ch);
  act("$n attempts to create a water elemental.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
          ( gch->pIndexData->vnum == MOB_VNUM_ELM_WATER ) )
        {
          i++;
          if (i > 2)
           {
            send_to_char("More water elementals are more than you can control!\n\r",ch);
            return;
           }
        }
    }

  if (count_charmed(ch)) return;

  elm = create_mobile( get_mob_index(MOB_VNUM_ELM_WATER) );


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
  elm->gold = 0;
  elm->timer = 0;
  elm->damage[DICE_NUMBER] = 8;   
  elm->damage[DICE_TYPE] = 4;
  elm->damage[DICE_BONUS] = ch->level / 2;

  char_to_room(elm,ch->in_room);
  send_to_char("You created a water elemental!\n\r",ch);
  act("$n creates a water elemental!",ch,NULL,NULL,TO_ROOM);

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
      send_to_char("You lack the power to create another elemental right now.\n\r",
                   ch);
      return;
    }

  send_to_char("You attempt to create a fire elemental.\n\r",ch);
  act("$n attempts to create a fire elemental.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
          ( gch->pIndexData->vnum == MOB_VNUM_ELM_FIRE ) )
        {
          i++;
          if (i > 2)
           {
            send_to_char("More fire elementals are more than you can control!\n\r",ch);
            return;
           }
        }
    }

  if (count_charmed(ch)) return;

  elm = create_mobile( get_mob_index(MOB_VNUM_ELM_FIRE) );


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
  elm->gold = 0;
  elm->timer = 0;
  elm->damage[DICE_NUMBER] = 11;   
  elm->damage[DICE_TYPE] = 5;
  elm->damage[DICE_BONUS] = ch->level / 2 + 10;

  char_to_room(elm,ch->in_room);
  send_to_char("You created a fire elemental!\n\r",ch);
  act("$n creates a fire elemental!",ch,NULL,NULL,TO_ROOM);

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
      send_to_char("You lack the power to create another elemental right now.\n\r",
                   ch);
      return;
    }

  send_to_char("You attempt to create a lightning elemental.\n\r",ch);
  act("$n attempts to create a lightning elemental.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
          ( gch->pIndexData->vnum == MOB_VNUM_ELM_LIGHT ) )
        {
          i++;
          if (i > 2)
           {
            send_to_char("More lightning elementals are more than you can control!\n\r",ch);
            return;
           }
        }
    }

  if (count_charmed(ch)) return;

  elm = create_mobile( get_mob_index(MOB_VNUM_ELM_LIGHT) );


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
  elm->gold = 0;
  elm->timer = 0;
  elm->damage[DICE_NUMBER] = 13;   
  elm->damage[DICE_TYPE] = 9;
  elm->damage[DICE_BONUS] = ch->level / 2 + 10;

  char_to_room(elm,ch->in_room);
  send_to_char("You created a lightning elemental!\n\r",ch);
  act("$n creates a lightning elemental!",ch,NULL,NULL,TO_ROOM);

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
    char buf[MAX_STRING_LENGTH];
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
                do_yell(tmp_vict, "Help someone is attacking me!");
            else
              {
                 sprintf(buf,"Die, %s, you sorcerous dog!",
	(is_affected(ch,gsn_doppelganger)&&!IS_IMMORTAL(tmp_vict))? ch->doppel->name : ch->name);
                 do_yell(tmp_vict,buf);
              }
          }

          if (saves_spell(level,tmp_vict, DAM_FIRE))
            dam /= 2;
	  if ((dam_sn = skill_lookup("fireball")) < 0 ) 
	    dam_sn = sn;
          damage( ch, tmp_vict, dam, dam_sn, DAM_FIRE, TRUE );
          tmp_vict->move -= UMIN(tmp_vict->move,movedam);

          if ( (IS_NPC(tmp_vict) && tmp_vict->position == POS_DEAD) ||
             (!IS_NPC(tmp_vict) && (current_time - tmp_vict->last_death_time) < 10))
	  {
          if (saves_spell(level,tmp_vict, DAM_COLD))
            dam /= 2;
	  if ((dam_sn = skill_lookup("iceball")) < 0 ) 
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
	  send_to_char("You are already at ground potential.\n\r",ch);
	else
	  act("$N is already at ground potential.",ch,NULL,victim,TO_CHAR);
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
    send_to_char( "Your body is electrically grounded.\n\r", victim );
    if ( ch != victim )
	act("$N is grounded by your magic.",ch,NULL,victim,TO_CHAR);
	return;
}

void spell_tsunami( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    if ( (ch->in_room->sector_type != SECT_WATER_SWIM)
        && (ch->in_room->sector_type != SECT_WATER_NOSWIM))
        {
         send_to_char("You can't reach a water source to create a tsunami.\n\r",ch);
         ch->wait = 0;
         return;
        }

    act("An existing parcel of water rises up and forms a fist and pummels $n.",
                victim,NULL,NULL,TO_ROOM);
    act("An existing parcel of water rises up and forms a fist and pummels you.",
                victim,NULL,NULL,TO_CHAR);
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
	send_to_char("The item must be carried to be enchanted.\n\r",ch);
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
	act("$p flares blindingly... and evaporates!",ch,obj,NULL,TO_CHAR);
	act("$p flares blindingly... and evaporates!",ch,obj,NULL,TO_ROOM);
	extract_obj(obj);
	return;
    }

    if (result > (fail / 2)) /* item disenchanted */
    {
	AFFECT_DATA *paf_next;

	act("$p glows brightly, then fades.",ch,obj,NULL,TO_CHAR);
	act("$p glows brightly, then fades.",ch,obj,NULL,TO_ROOM);
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

    send_to_char("Nothing seemed to happen.\n\r",ch);
    return;
}

void spell_disenchant_weapon( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    AFFECT_DATA *paf;
    int result, fail;

    if (obj->item_type != ITEM_WEAPON)
    {
	send_to_char("That isn't a weapon.\n\r",ch);
	return;
    }

    if (obj->wear_loc != -1)
    {
	send_to_char("The item must be carried to be enchanted.\n\r",ch);
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
	act("$p shivers violently and explodes!",ch,obj,NULL,TO_CHAR);
	act("$p shivers violently and explodeds!",ch,obj,NULL,TO_ROOM);
	extract_obj(obj);
	return;
    }

    if (result > (fail / 2)) /* item disenchanted */
    {
	AFFECT_DATA *paf_next;

	act("$p glows brightly, then fades.",ch,obj,NULL,TO_CHAR);
	act("$p glows brightly, then fades.",ch,obj,NULL,TO_ROOM);
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

    send_to_char("Nothing seemed to happen.\n\r",ch);
    return;
}

void spell_absorb( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    AFFECT_DATA af;

    if ( is_affected( ch, sn ) )
    {
	send_to_char("You are already absorbing magic surrounding you.\n\r",ch);
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
    send_to_char( "Your body is surrounded by an energy field.\n\r", ch );
    return;
}

void spell_transfer_object( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
	send_to_char("Not implemented!\n\r", ch);
	return;
}

void spell_animate_object( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
  CHAR_DATA *mob;
  OBJ_DATA *obj = (OBJ_DATA *) vo;
  AFFECT_DATA af;
  char buf[MAX_STRING_LENGTH];
  int i;

  if (!(obj->item_type == ITEM_WEAPON ||  
         obj->item_type == ITEM_ARMOR))
  {
        send_to_char("You can animate only armors and weapons.\n\r",ch);
        return;
  }

  if (is_affected(ch, sn))
  {
      send_to_char("You cannot summon the strength to handle more undead bodies.\n\r", ch);
      return;
  }

  if ( obj->level > level )
  {
      act("$p is too powerful for you to animate it.", ch, obj, NULL, TO_CHAR);
      return;
  }

  if (count_charmed(ch)) return;

  if (obj->item_type == ITEM_ARMOR
	&& !(CAN_WEAR(obj, ITEM_WEAR_BODY)
	|| CAN_WEAR(obj, ITEM_WEAR_HANDS)
	|| CAN_WEAR(obj, ITEM_WEAR_SHIELD)) )
  {
      send_to_char("You can only animate that type of armor.\n\r", ch);
      return;
  }

  if (number_percent() > get_skill(ch, sn))
  {
      act("$p violately explodes!", ch, obj, NULL, TO_CHAR);
      act("$p violately explodes!", ch, obj, NULL, TO_ROOM);
      extract_obj( obj );
      return;
  }

  if (obj->item_type == ITEM_WEAPON)
	mob = create_mobile(get_mob_index(MOB_VNUM_WEAPON));
  else	mob = create_mobile(get_mob_index(MOB_VNUM_ARMOR));

  sprintf(buf, "animate %s", obj->name);
  free_string( mob->name );
  mob->name = str_dup( buf );

  sprintf(buf, mob->short_descr, obj->short_descr );
  free_string( mob->short_descr );
  mob->short_descr = str_dup( buf );

  sprintf(buf, "%s is here, staring at you!.\n\r", capitalize(obj->short_descr) );
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
  mob->gold = 0;
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
 
  act("You give life to $p with your power!\n\r", ch, obj, NULL, TO_CHAR);
  act("$n gives life to $p with $s power!\n\r", ch, obj, NULL, TO_ROOM);
  
  extract_obj( obj );
	return;
}

void spell_windwall( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    char buf[MAX_STRING_LENGTH];
    int dam,hp_dam,dice_dam,hpch;

    act("$n raise a wall of wind striking everyone.",ch,NULL,NULL,TO_ROOM);
    act("You raise a wall of wind.",ch,NULL,NULL,TO_CHAR);

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
                do_yell(vch, "Help someone is attacking me!");
            else
              {
                 sprintf(buf,"Die, %s, you sorcerous dog!",
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
	send_to_char("You cannot reach the earth to fade.\n\r", ch);
	return;
    }

    if (MOUNTED(ch)) 
    {
        send_to_char("You can't fade to earth while mounted.\n\r", ch);
        return;
    }

    act( "$n fades into earth.", ch, NULL, NULL, TO_ROOM );

    af.where     = TO_DETECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = (level / 8 + 10);
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = ADET_EARTHFADE;
    affect_to_char( ch, &af );
    send_to_char( "You fade into earth.\n\r", ch );
    return;
}

void spell_earthmaw( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    char buf[MAX_STRING_LENGTH]; 
    int dam;

    sprintf(buf,"You tremble the earth underneath the %s.\n\r",victim->name);
    send_to_char( buf, ch );
    act( "$n trembles the earth underneath you!.", ch, NULL, victim, TO_VICT );
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
        send_to_char("That item is not magical.\n\r",ch);
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
        act("$p evaporates!",ch,obj,NULL,TO_ROOM);
        act("$p evaporates, but you fail to channel the energy.",ch,obj,NULL,TO_CHAR);
    }
    else
    {
	act("$p evaporates as $n drains its energy!",ch,obj,NULL,TO_ROOM);
	act("$p evaporates as you drain its energy!",ch,obj,NULL,TO_CHAR);
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

    send_to_char( "Your skin starts to wrinkle.\n\r", victim );
    act( "$n skin starts to wrinkle.", victim, NULL, NULL, TO_ROOM );
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
	  send_to_char("You can't be more fumble!\n\r",ch);
	else
	  act("$N can't get any more fumble than that.",
	      ch,NULL,victim,TO_CHAR);
	return;
    }

    if (saves_spell(level,victim,DAM_OTHER)
    ||  IS_SET(victim->imm_flags,IMM_MAGIC))
    {
	if (victim != ch)
	    send_to_char("Nothing seemed to happen.\n\r",ch);
	send_to_char("You feel momentarily lethargic.\n\r",victim);
	return;
    }

    if (IS_AFFECTED(victim,AFF_HASTE))
    {
	if (!check_dispel(level,victim,skill_lookup("haste")))
	{
	    if (victim != ch)
		send_to_char("Spell failed.\n\r",ch);
	    send_to_char("You feel momentarily slower.\n\r",victim);
	    return;
	}

	act("$n is moving less quickly.",victim,NULL,NULL,TO_ROOM);
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
	  act("$n cannot carry $p anymore and drops it.", victim,obj,NULL,TO_ROOM);
	  send_to_char("You cannot carry your dual weapon anymore and drop it!\n\r", victim);
	  obj_from_char(obj);
	  obj_to_room(obj,victim->in_room);
	}
    }
    if ((obj = get_weapon_char(victim, FALSE)) != NULL)
    {
	if (can_drop_obj(victim,obj)
	    &&  remove_obj(victim,obj,TRUE))
	{
	  act("$n cannot carry $p anymore and drops it.", victim,obj,NULL,TO_ROOM);
	  send_to_char("You cannot carry your weapon anymore and drop it!\n\r", victim);
	  obj_from_char(obj);
	  obj_to_room(obj,victim->in_room);
	}
    }

    WAIT_STATE(victim, PULSE_VIOLENCE);
    send_to_char( "You feel yourself very  f u m b l e...\n\r", victim );
    act("$n starts to move in a fumble way.",victim,NULL,NULL,TO_ROOM);
    return;
}

