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
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "merc.h"

/* command procedures needed */
DECLARE_DO_FUN(do_human		);
DECLARE_DO_FUN(do_murder	);
DECLARE_DO_FUN(do_rescue	);
DECLARE_DO_FUN(do_quaff		);
DECLARE_DO_FUN(do_quit		);
DECLARE_DO_FUN(do_quit_count	);
DECLARE_DO_FUN(do_spellbane	);
DECLARE_DO_FUN(do_stand		);
DECLARE_DO_FUN(do_track		);
DECLARE_DO_FUN(do_yell		);
DECLARE_DO_FUN(do_wake		);

DECLARE_SPEC_FUN(       spec_special_guard      );

void	raw_kill		args( ( CHAR_DATA *victim ) );
void	back_home		args( ( CHAR_DATA *ch ) );
int 	check_time_sync		( );

//#include <unistd.h>
#include <signal.h>

/*
 * Local functions.
 */
int	hit_gain	args( ( CHAR_DATA *ch ) );
int	mana_gain	args( ( CHAR_DATA *ch ) );
int	move_gain	args( ( CHAR_DATA *ch ) );
void	mobile_update	args( ( void ) );
void	weather_update	args( ( void ) );
void	char_update	args( ( void ) );
void	obj_update	args( ( void ) );
void	aggr_update	args( ( void ) );
int	potion_cure_level	args( ( OBJ_DATA *potion ) );
int	potion_arm_level	args( ( OBJ_DATA *potion ) );
bool	potion_cure_blind	args( ( OBJ_DATA *potion ) );
bool	potion_cure_poison	args( ( OBJ_DATA *potion ) );
bool	potion_cure_disease	args( ( OBJ_DATA *potion ) );

/* below done by chronos */
void    quest_update    args( ( void ) );
void	auction_update	args( ( void ) );
void	light_update	args( ( void ) );
void	room_update	args( ( void ) );
void	room_affect_update	args( ( void ) );
void	check_reboot	args( ( void ) );
void	track_update	args( ( void ) );

/* used for saving */

int	save_number = 0;
extern int max_on;
extern int max_on_so_far;



/*
 * Advancement stuff.
 */
void advance_level( CHAR_DATA *ch )
{
    wchar_t buf[MAX_STRING_LENGTH];
    int add_hp;
    int add_mana;
    int add_move;
    int add_prac;

    if (IS_NPC(ch) )  {
       bug( L"Advance_level: a mob to advance!", 0);
       return;
    }

    ch->pcdata->last_level =
	( ch->played + (int) (current_time - ch->logon) ) / 3600;

    if ( wcsstr( ch->pcdata->title,
	title_table[ch->iclass][ch->level-1])
	|| CANT_CHANGE_TITLE(ch) )
    {
      swprintf( buf, MAX_STRING_LENGTH-1, L", %s",
	title_table [ch->iclass] [ch->level]  );
      set_title( ch, buf );
    }

    add_hp = (con_app[get_curr_stat(ch,STAT_CON)].hitp + number_range(1,5)) - 3;
    add_hp = (add_hp * class_table[ch->iclass].hp_rate) / 100;
    add_mana 	=
   number_range(get_curr_stat(ch,STAT_INT)/2,(2*get_curr_stat(ch,STAT_INT)
				  + get_curr_stat(ch,STAT_WIS)/5));
    add_mana = (add_mana * class_table[ch->iclass].mana_rate) / 100;

    add_move	= number_range( 1, (get_curr_stat(ch,STAT_CON)
				  + 2 * get_curr_stat(ch,STAT_DEX))/6 );
    add_prac	= wis_app[get_curr_stat(ch,STAT_WIS)].practice;

    add_hp	= UMAX(  3, add_hp   );
    add_mana	= UMAX(  3, add_mana );
    add_move	= UMAX(  6, add_move );

    if (ch->sex == SEX_FEMALE)
	{
	 add_hp   -= 1;
	 add_mana += 2;
	}

    ch->max_hit 	+= add_hp;
    ch->max_mana	+= add_mana;
    ch->max_move	+= add_move;
    ch->practice	+= add_prac;
    ch->train		+= ch->level%5==0?1:0;

    ch->pcdata->perm_hit	+= add_hp;
    ch->pcdata->perm_mana	+= add_mana;
    ch->pcdata->perm_move	+= add_move;


    printf_to_char(ch,L"{GKazancın: %d/%d yp, %d/%d mana, %d/%d hp %d/%d pratik.{x\n\r" ,add_hp,		ch->max_hit,
  	add_mana,	ch->max_mana,add_move,	ch->max_move,add_prac,	ch->practice);
    return;
}



void gain_exp( CHAR_DATA *ch, int gain )
{
    wchar_t buf[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) || ch->level >= LEVEL_HERO )
	return;
    if ( IS_SET(ch->act,PLR_NO_EXP))
	{
    send_to_char( L"Ruhun olmadan tecrübe kazanamazsın.\n\r", ch );
	 return;
	}

/*
    ch->exp = UMAX( exp_per_level(ch,ch->pcdata->points), ch->exp + gain );
    while ( ch->level < LEVEL_HERO && ch->exp >=
	exp_per_level(ch,ch->pcdata->points) * (ch->level+1) )
*/

    ch->exp = UMAX( base_exp(ch,ch->pcdata->points), ch->exp + gain );
    while ( ch->level < LEVEL_HERO &&
           exp_to_level(ch,ch->pcdata->points) <= 0)
    {
      printf_to_char(ch, L"{GSeviyen yükseldi!!{x  " );
	ch->level += 1;

	/* added for samurais by chronos */
	if ((ch->iclass == 9) && (ch->level == 10)) ch->wimpy = 0;

        /* Level counting */
        if (ch->level > 5)
          total_levels++;

        if (ch->level == 90)
          {
            swprintf( log_buf, (2*MAX_INPUT_LENGTH)-1, L"%s seviye 90 oldu.", ch->name);
            log_string(log_buf);
          }

	swprintf( buf, MAX_STRING_LENGTH-1, L"$N seviye %d oldu!",ch->level);
	wiznet(buf,ch,NULL,WIZ_LEVELS,0,0);
	advance_level( ch );
	save_char_obj(ch);
    }

    return;
}



/*
 * Regeneration stuff.
 */
int hit_gain( CHAR_DATA *ch )
{
    int gain;
    int number;

    if (ch->in_room == NULL)
	return 0;

    if ( IS_NPC(ch) )
    {
	gain =  5 + ch->level;
 	if (IS_AFFECTED(ch,AFF_REGENERATION))
	    gain *= 2;

	switch(ch->position)
	{
            default :           gain /= 2;                      break;
            case POS_SLEEPING:  gain = 3 * gain/2;              break;
            case POS_RESTING:                                   break;
            case POS_FIGHTING:  gain /= 3;                      break;
 	}


    }
    else
    {
	gain = UMAX(3, 2 * get_curr_stat(ch,STAT_CON) + (7 * ch->level) / 4);
	gain = (gain * class_table[ch->iclass].hp_rate) / 100;
 	number = number_percent();
	if (number < get_skill(ch,gsn_fast_healing))
	{
	    gain += number * gain / 100;
	    if (ch->hit < ch->max_hit)
		check_improve(ch,gsn_fast_healing,TRUE,8);
	}

	if (number < get_skill(ch,gsn_trance))
	{
	    gain += number * gain / 150;
	    if (ch->mana < ch->max_mana)
	        check_improve(ch,gsn_trance,TRUE,8);
	}
	switch ( ch->position )
	{
	    default:	   	gain /= 4;			break;
	    case POS_SLEEPING: 					break;
	    case POS_RESTING:  	gain /= 2;			break;
	    case POS_FIGHTING: 	gain /= 6;			break;
	}

	if ( ch->pcdata->condition[COND_HUNGER]   < 0 )
	    gain = 0;

	if ( ch->pcdata->condition[COND_THIRST] < 0 )
	    gain = 0;

    }

    gain = gain * ch->in_room->heal_rate / 100;

    if (ch->on != NULL && ch->on->item_type == ITEM_FURNITURE)
	gain = gain * ch->on->value[3] / 100;

    if ( IS_AFFECTED(ch, AFF_POISON) )
	gain /= 4;

    if (IS_AFFECTED(ch, AFF_PLAGUE))
	gain /= 8;

    if (IS_AFFECTED(ch,AFF_HASTE) )
	gain /=2 ;

    if (IS_AFFECTED(ch,AFF_SLOW) )
	gain *=2 ;

    if (get_curr_stat(ch,STAT_CON) > 20 )
	gain = ( gain * 14) / 10;

    if (IS_HARA_KIRI(ch))
	gain *= 3;

    return UMIN(gain, ch->max_hit - ch->hit);
}



int mana_gain( CHAR_DATA *ch )
{
    int gain;
    int number;

    if (ch->in_room == NULL)
	return 0;

    if ( IS_NPC(ch) )
    {
	gain = 5 + ch->level;
	switch (ch->position)
	{
	    default:		gain /= 2;		break;
	    case POS_SLEEPING:	gain = 3 * gain/2;	break;
   	    case POS_RESTING:				break;
	    case POS_FIGHTING:	gain /= 3;		break;
    	}
    }
    else
    {
	gain = get_curr_stat(ch,STAT_WIS)
		      + (2 * get_curr_stat(ch,STAT_INT)) + ch->level;
	gain = (gain * class_table[ch->iclass].mana_rate) / 100;
	number = number_percent();
	if (number < get_skill(ch,gsn_meditation))
	{
	    gain += number * gain / 100;
	    if (ch->mana < ch->max_mana)
	        check_improve(ch,gsn_meditation,TRUE,8);
	}

	if (number < get_skill(ch,gsn_trance))
	{
	    gain += number * gain / 100;
	    if (ch->mana < ch->max_mana)
	        check_improve(ch,gsn_trance,TRUE,8);
	}

	if (!class_table[ch->iclass].fMana)
	    gain /= 2;

	switch ( ch->position )
	{
	    default:		gain /= 4;			break;
	    case POS_SLEEPING: 					break;
	    case POS_RESTING:	gain /= 2;			break;
	    case POS_FIGHTING:	gain /= 6;			break;
	}

	if ( ch->pcdata->condition[COND_HUNGER]   < 0 )
	    gain = 0;

	if ( ch->pcdata->condition[COND_THIRST] < 0 )
	    gain = 0;

    }

    gain = gain * ch->in_room->mana_rate / 100;

    if (ch->on != NULL && ch->on->item_type == ITEM_FURNITURE)
	gain = gain * ch->on->value[4] / 100;

    if ( IS_AFFECTED( ch, AFF_POISON ) )
	gain /= 4;

    if (IS_AFFECTED(ch, AFF_PLAGUE))
        gain /= 8;

    if (IS_AFFECTED(ch,AFF_HASTE) )
        gain /= 2 ;
    if (IS_AFFECTED(ch,AFF_SLOW) )
        gain *= 2 ;
    if (get_curr_stat(ch,STAT_INT) > 20 )
	gain = ( gain * 13) / 10;
    if (get_curr_stat(ch,STAT_WIS) > 20 )
	gain = ( gain * 11) / 10;
    if (IS_HARA_KIRI(ch))
	gain *= 3;

    return UMIN(gain, ch->max_mana - ch->mana);
}



int move_gain( CHAR_DATA *ch )
{
    int gain;

    if (ch->in_room == NULL)
	return 0;

    if ( IS_NPC(ch) )
    {
	gain = ch->level;
    }
    else
    {
	gain = UMAX( 15, 2 * ch->level );

	switch ( ch->position )
	{
	case POS_SLEEPING: gain += 2 * (get_curr_stat(ch,STAT_DEX));	break;
	case POS_RESTING:  gain += get_curr_stat(ch,STAT_DEX);		break;
	}

	if ( ch->pcdata->condition[COND_HUNGER]   < 0 )
	    gain = 3;

	if ( ch->pcdata->condition[COND_THIRST] < 0 )
	    gain = 3;
    }

    gain = gain * ch->in_room->heal_rate/100;

    if (ch->on != NULL && ch->on->item_type == ITEM_FURNITURE)
	gain = gain * ch->on->value[3] / 100;

    if ( IS_AFFECTED(ch, AFF_POISON) )
	gain /= 4;

    if (IS_AFFECTED(ch, AFF_PLAGUE))
        gain /= 8;

    if (IS_AFFECTED(ch,AFF_HASTE) || IS_AFFECTED(ch,AFF_SLOW))
        gain /=2 ;
    if (get_curr_stat(ch,STAT_DEX) > 20)
	gain *= (14 /10);
    if (IS_HARA_KIRI(ch))
	gain *= 3;

    return UMIN(gain, ch->max_move - ch->move);
}



void gain_condition( CHAR_DATA *ch, int iCond, int value )
{
    int condition;
    int damage_hunger;
    int fdone;
    CHAR_DATA *vch,*vch_next;

    if ( value == 0 || IS_NPC(ch) || ch->level >= LEVEL_IMMORTAL)
	return;

    condition				= ch->pcdata->condition[iCond];

    ch->pcdata->condition[iCond]	= URANGE( -6, condition + value, 96 );

    if ( iCond == COND_FULL && (ch->pcdata->condition[COND_FULL] < 0) )
       ch->pcdata->condition[COND_FULL] = 0;

    if ( (iCond == COND_DRUNK) && (condition < 1) )
      ch->pcdata->condition[COND_DRUNK] = 0;

    if ( ch->pcdata->condition[iCond] < 1 && ch->pcdata->condition[iCond] > -6 )
    {
	switch ( iCond )
	{
	case COND_HUNGER:
  send_to_char( L"Karnın aç.\n\r",  ch );
	    break;

	case COND_THIRST:
  send_to_char( L"Susadın.\n\r", ch );
	    break;

	case COND_DRUNK:
	    if ( condition != 0 )
      send_to_char( L"Sarhoşsun.\n\r", ch );
	    break;

	case COND_BLOODLUST:
	    if ( condition != 0 )
      send_to_char( L"Kana susadın.\n\r", ch );
	    break;

	case COND_DESIRE:
	    if ( condition != 0 )
      send_to_char( L"Yurdunu özledin.\n\r", ch );
	    break;
	}
    }

    if ( ch->pcdata->condition[iCond] == -6 && ch->level >= KIDEMLI_OYUNCU_SEVIYESI )
    {
	switch ( iCond )
	{
	case COND_HUNGER:
  send_to_char( L"Açlıktan ölüyorsun!\n\r",  ch );
  act( L"$n açlıktan ölüyor!",  ch, NULL, NULL, TO_ROOM );
	    damage_hunger = ch->max_hit * number_range(2, 4) / 100;
	    if (!damage_hunger) damage_hunger = 1;
	    damage( ch, ch, damage_hunger, TYPE_HUNGER, DAM_HUNGER, TRUE );
	    if ( ch->position == POS_SLEEPING )
		return;
	    break;

	case COND_THIRST:
  send_to_char( L"Susuzluktan ölüyorsun!\n\r", ch );
  act( L"$n susuzluktan ölüyor!", ch, NULL, NULL, TO_ROOM );
	    damage_hunger = ch->max_hit * number_range(2, 4) / 100;
	    if (!damage_hunger) damage_hunger = 1;
	    damage( ch, ch, damage_hunger, TYPE_HUNGER, DAM_THIRST, TRUE );
	    if ( ch->position == POS_SLEEPING )
		return;
	    break;

	case COND_BLOODLUST:
	    fdone = 0;
      send_to_char( L"Kana olan açlığın seni öldürüyor!\n\r",ch );
	    act( L"Kana olan açlığı $m öldürüyor!", ch,NULL,NULL,TO_ROOM );
	    if (ch->in_room && ch->in_room->people && !ch->fighting)
	    {
		if (!IS_AWAKE(ch)) do_stand(ch,(wchar_t*)"");
                for ( vch = ch->in_room->people;
               		vch != NULL && ch->fighting == NULL; vch = vch_next)
                {
                  vch_next = vch->next_in_room;
                  if ( ch != vch && can_see(ch,vch) &&
                        !is_safe_nomessage(ch,vch) )
                   {
                     do_yell(ch,(wchar_t*)"KAN! Kana ihtiyacım var!");
                    do_murder(ch,vch->name);
		    fdone = 1;
		    break;
                   }
                }
	    }
	    if (fdone) break;
	    damage_hunger = ch->max_hit * number_range(2, 4) / 100;
	    if (!damage_hunger) damage_hunger = 1;
	    damage( ch, ch, damage_hunger, TYPE_HUNGER, DAM_THIRST, TRUE );
	    if ( ch->position == POS_SLEEPING )
		return;
	    break;

	case COND_DESIRE:
  send_to_char( L"Eve dönmek istiyorsun!\n\r", ch );
  act( L"$n memleket özlemiyle yanıp tutuşuyor!", ch, NULL, NULL, TO_ROOM );
	    if (ch->position >= POS_STANDING)
		move_char(ch,number_door(),FALSE);
	    break;
	}
    }



    return;
}



/*
 * Mob autonomous action.
 * This function takes 25% to 35% of ALL Merc cpu time.
 * -- Furey
 */
void mobile_update( void )
{
    CHAR_DATA *ch;
    CHAR_DATA *ch_next;
    EXIT_DATA *pexit;
    int door;
    OBJ_DATA *obj;


    /* Examine all mobs. */
    for ( ch = char_list; ch != NULL; ch = ch_next )
    {
	ch_next = ch->next;

	if ( IS_AFFECTED(ch, AFF_REGENERATION ) && ch->in_room != NULL )
	{
	 ch->hit = UMIN( ch->hit + ch->level / 10, ch->max_hit );
	 if ( RACE(ch) == 18 /* asura */ )
	   ch->hit = UMIN( ch->hit + ch->level / 10, ch->max_hit );
	 if (ch->cabal == CABAL_BATTLE && is_affected(ch, gsn_bandage))
	   ch->hit = UMIN( ch->hit + ch->level / 10, ch->max_hit );
	 if (ch->hit != ch->max_hit) send_to_char( L"",ch);
	}

	if ( IS_AFFECTED(ch, AFF_CORRUPTION ) && ch->in_room != NULL )
	{
	 ch->hit -=  ch->level / 10;
	 if (ch->hit < 1)
         {
	    int sn = skill_lookup( L"corruption");

	    if (sn < 0) sn = gsn_witch_curse;
	    ch->hit = 1;
	    damage(ch,ch,16,sn,DAM_NONE,FALSE);
	    continue;
	 }
	 else send_to_char( L"",ch);
	}

	if ( CAN_DETECT(ch, ADET_SUFFOCATE ) && ch->in_room != NULL )
	{
	 ch->hit -=  ch->level / 5;
	 if (ch->hit < 1)
         {
	    int sn = skill_lookup( L"suffocate");

	    if (sn < 0) sn = gsn_witch_curse;
	    ch->hit = 1;
	    damage(ch,ch,16,sn,DAM_NONE,FALSE);
	    continue;
	 }
	 else {
		if (number_percent() < 30) {
      send_to_char( L"Nefes alamıyorsun!",ch);
		}
	 }
	}

	if ( ch->spec_fun == spec_special_guard )
	{
	    if ( (*ch->spec_fun) ( ch ) )
		continue;
	}

	if ( !IS_NPC(ch) || ch->in_room == NULL || IS_AFFECTED(ch,AFF_CHARM) )
	    continue;

	if (IS_SET(ch->act,ACT_HUNTER) && ch->hunting && ch->fighting == NULL)
	    hunt_victim(ch);

	if (ch->in_room->area->empty && !IS_SET(ch->act,ACT_UPDATE_ALWAYS))
	    continue;

	/* Examine call for special procedure */
	if ( ch->spec_fun != 0 )
	{
	    if ( (*ch->spec_fun) ( ch ) )
		continue;
	}

	if (ch->pIndexData->pShop != NULL) /* give him some silver */
  {
    ch->silver = number_range(ch->level,ch->level*5);
  }


	/*
	 *  Potion using and stuff for intelligent mobs
	 */

	if ( ch->position == POS_STANDING ||
	     ch->position == POS_RESTING ||
	     ch->position == POS_FIGHTING )
	{
	   if ( get_curr_stat(ch, STAT_INT ) > 15 &&
		(ch->hit<ch->max_hit*0.9 ||
		 IS_AFFECTED(ch,AFF_BLIND) ||
		 IS_AFFECTED(ch,AFF_POISON) ||
		 IS_AFFECTED(ch,AFF_PLAGUE) || ch->fighting!=NULL) )
	   {
	      for(obj=ch->carrying;obj!=NULL;obj=obj->next_content)
		 if ( obj->item_type == ITEM_POTION )
		 {
		   if ( ch->hit < ch->max_hit*0.9 )  /* hp curies */
		   {
		   int cl;
		     cl=potion_cure_level( obj );
		     if (cl > 0)
		     {
			if ( ch->hit<ch->max_hit*0.5 && cl > 3 )  {
			  do_quaff( ch, obj->name );
			  continue;
			}
			else if ( ch->hit<ch->max_hit*0.7 )  {
			  do_quaff( ch, obj->name );
			  continue;
			}
		     }
		   }
	     	   if ( IS_AFFECTED(ch,AFF_POISON) && potion_cure_poison(obj) )
		   {
		     do_quaff( ch, obj->name );
		     continue;
		   }
	     	   if ( IS_AFFECTED(ch,AFF_PLAGUE) && potion_cure_disease(obj) )
		   {
		     do_quaff( ch, obj->name );
		     continue;
		   }
	     	   if ( IS_AFFECTED(ch,AFF_BLIND) && potion_cure_blind(obj) )
		   {
		     do_quaff( ch, obj->name );
		     continue;
		   }
	     	   if ( ch->fighting != NULL )
		   {
		   int al;
		     al = potion_arm_level( obj );
		     if ( ch->level-ch->fighting->level < 7 && al>3)
		     {
		     	do_quaff( ch, obj->name );
			continue;
		     }
		     if ( ch->level-ch->fighting->level < 8 && al>2 )
		     {
		     	do_quaff( ch, obj->name );
			continue;
		     }
		     if ( ch->level-ch->fighting->level < 9 && al>1 )
		     {
		     	do_quaff( ch, obj->name );
			continue;
		     }
		     if ( ch->level-ch->fighting->level < 10 && al>0 )
		     {
		     	do_quaff( ch, obj->name );
			continue;
		     }
		     else
		        continue;
		   }
		 }
	   }
	}


	/* That's all for sleeping / busy monster, and empty zones */
	if ( ch->position != POS_STANDING )
	    continue;

        if ( IS_SET(ch->progtypes,MPROG_AREA) &&
            (ch->in_room->area->nplayer > 0))
          (ch->pIndexData->mprogs->area_prog) (ch);


        if (ch->position < POS_STANDING)
          continue;

	/* Scavenge */
	if ( IS_SET(ch->act, ACT_SCAVENGER)
	&&   ch->in_room->contents != NULL
	&&   number_range(0,63) == 0 )
	{
	    OBJ_DATA *obj;
	    OBJ_DATA *obj_best;
	    int max;

	    max         = 1;
	    obj_best    = 0;
	    for ( obj = ch->in_room->contents; obj; obj = obj->next_content )
	    {
		if ( CAN_WEAR(obj, ITEM_TAKE) && can_loot(ch, obj)
		     && obj->cost > max  && obj->cost > 0 )
		{
		    obj_best    = obj;
		    max         = obj->cost;
		}
	    }

	    if ( obj_best )
	    {
		obj_from_room( obj_best );
		obj_to_char( obj_best, ch );
    act( L"$n $p alıyor.", ch, obj_best, NULL, TO_ROOM );
		if (IS_SET(obj_best->progtypes,OPROG_GET))
		   (obj_best->pIndexData->oprogs->get_prog) (obj_best,ch);
	    }
	}

	/* Wander */
	if ( !IS_SET(ch->act, ACT_SENTINEL)
	&& number_range(0,7) == 0
	&& ( door = number_range(0,31) ) <= 5
	&& !RIDDEN(ch)
	&& ( pexit = ch->in_room->exit[door] ) != NULL
	&&   pexit->u1.to_room != NULL
	&&   !IS_SET(pexit->exit_info, EX_CLOSED)
	&&   !IS_SET(pexit->u1.to_room->room_flags, ROOM_NO_MOB)
	&& ( !IS_SET(ch->act, ACT_STAY_AREA)
	||   pexit->u1.to_room->area == ch->in_room->area )
	&& ( !IS_SET(ch->act, ACT_OUTDOORS)
	||   !IS_SET(pexit->u1.to_room->room_flags,ROOM_INDOORS))
	&& ( !IS_SET(ch->act, ACT_INDOORS)
	||   IS_SET(pexit->u1.to_room->room_flags,ROOM_INDOORS)))
	{
	    move_char( ch, door, FALSE );
	}
    }

    return;
}

int potion_cure_level( OBJ_DATA *potion )
{
int cl;
int i;
  cl = 0;
  for (i=1;i<5;i++)
  {
    if ( skill_lookup( L"cure critical") == potion->value[i] )
      cl += 3;
    if ( skill_lookup( L"cure light") == potion->value[i])
      cl += 1;
    if ( skill_lookup( L"cure serious") == potion->value[i] )
      cl += 2;
    if ( skill_lookup( L"heal") == potion->value[i])
      cl += 4;
  }
  return(cl);
}
int potion_arm_level( OBJ_DATA *potion )
{
int al;
int i;
  al = 0;
  for (i=1;i<5;i++)
  {
    if ( skill_lookup( L"armor") == potion->value[i] )
      al += 1;
    if ( skill_lookup( L"shield") == potion->value[i])
      al += 1;
    if ( skill_lookup( L"stone skin") == potion->value[i] )
      al += 2;
    if ( skill_lookup( L"sanctuary") == potion->value[i])
      al += 4;
    if ( skill_lookup( L"protection") == potion->value[i])
      al += 3;
  }
  return(al);
}

bool potion_cure_blind( OBJ_DATA *potion )
{
int i;
  for (i=0;i<5;i++)
  {
    if (skill_lookup( L"cure blindness") == potion->value[i])
      return(TRUE);
  }
  return(FALSE);
}
bool potion_cure_poison( OBJ_DATA *potion )
{
int i;
  for (i=0;i<5;i++)
  {
    if (skill_lookup( L"cure poison") == potion->value[i] )
      return(TRUE);
  }
  return(FALSE);
}
bool potion_cure_disease( OBJ_DATA *potion )
{
int i;
  for (i=0;i<5;i++)
  {
    if (skill_lookup( L"cure disease") == potion->value[i] )
      return(TRUE);
  }
  return(FALSE);
}

void game_time_update( void )
{
/*
milat: sunucu alindiktan sonra oyunun ilk acildigi gun
22 agustos 2019 00:00:01
*/
	fwprintf( stderr, L"game_time_update :: 1\n" );
	long milat 			= 1566421201;
	long bir_oyun_saati	= 5 * 60; /* 300 gerçek saniye , 5 gerçek dakika */
	long bir_oyun_gunu	= 24 * bir_oyun_saati; /* 120 gerçek dakika , 2 gerçek saat*/
	long bir_oyun_ayi	= 10 * bir_oyun_gunu; /* 20 gerçek saat */
	long bir_oyun_yili	= 12 * bir_oyun_ayi; /* 240 gerçek saat, 10 gerçek gün */

	long x , eski_gun;
	wchar_t buf[ MAX_STRING_LENGTH ];

	eski_gun = time_info.day;
	fwprintf( stderr, L"game_time_update :: 2\n" );
	x = (long)(current_time - milat);
	time_info.year = ceil( (float)x / (float)bir_oyun_yili );

	x = x % bir_oyun_yili;
	time_info.month = ceil( (float)x / (float)bir_oyun_ayi );

	x = x % bir_oyun_ayi;
	time_info.day = ceil( (float)x / (float)bir_oyun_gunu );

	x = x % bir_oyun_gunu;
	time_info.hour = x / bir_oyun_saati;
	fwprintf( stderr, L"game_time_update :: 3\n" );
	if( eski_gun != time_info.day )
	{
		fwprintf( stderr, L"game_time_update :: 4-%d\n",MAX_STRING_LENGTH );
		swprintf( buf, MAX_STRING_LENGTH-1, L"Oyun tarihi: %ld/%ld/%ld Saat: %ld",time_info.day,time_info.month,time_info.year,time_info.hour);
		fwprintf( stderr, L"game_time_update :: 5\n" );
		log_string( buf );
		fwprintf( stderr, L"game_time_update :: 6\n" );
	}
	fwprintf( stderr, L"game_time_update :: 7\n" );
}

void game_time_to_string( time_t gameTime , wchar_t *buf )
{
	/* varsayılan değerler */
	long milat 			= 1566421201;
	long bir_oyun_saati	= 5 * 60; /* 300 gerçek saniye , 5 gerçek dakika */
	long bir_oyun_gunu	= 24 * bir_oyun_saati; /* 120 gerçek dakika , 2 gerçek saat*/
	long bir_oyun_ayi	= 10 * bir_oyun_gunu; /* 20 gerçek saat */
	long bir_oyun_yili	= 12 * bir_oyun_ayi; /* 240 gerçek saat, 10 gerçek gün */

	long yil , ay , gun , x;

	x = (long)( gameTime - milat );
	yil = ceil( (float)x / (float)bir_oyun_yili );

	x = x % bir_oyun_yili;
	ay = ceil( (float)x / (float)bir_oyun_ayi );

	x = x % bir_oyun_ayi;
	gun = ceil( (float)x / (float)bir_oyun_gunu );

	swprintf( buf, MAX_STRING_LENGTH-1, L"%ld/%ld/%ld",gun,ay,yil);
}

int game_time_to_year( time_t gameTime )
{
	/* varsayılan değerler */
	long milat 			= 1566421201;
	long bir_oyun_saati	= 5 * 60; /* 300 gerçek saniye , 5 gerçek dakika */
	long bir_oyun_gunu	= 24 * bir_oyun_saati; /* 120 gerçek dakika , 2 gerçek saat*/
	long bir_oyun_ayi	= 10 * bir_oyun_gunu; /* 20 gerçek saat */
	long bir_oyun_yili	= 12 * bir_oyun_ayi; /* 240 gerçek saat, 10 gerçek gün */

	long yil , x;

	x = (long)( gameTime - milat );
	yil = ceil( (float)x / (float)bir_oyun_yili );
	return yil;
}

int get_age( CHAR_DATA *ch )
{
	int birth_year = game_time_to_year(ch->pcdata->birth_time);
	int current_year = game_time_to_year( current_time );
    return (current_year - birth_year);
}


int age_to_num( int age )
{
    return  age * 72000;
}

/*
 * Update the weather.
 */
 void weather_update( void )
 {
     wchar_t buf[MAX_STRING_LENGTH];
     DESCRIPTOR_DATA *d;
     int diff;



     buf[0] = '\0';

     switch ( time_info.hour )
     {
     case  5:
 	weather_info.sunlight = SUN_LIGHT;
 	wcscat(buf, L"Gün başladı.\n\r" );
 	break;

     case  6:
 	weather_info.sunlight = SUN_RISE;
 	wcscat(buf, L"Güneş doğudan yükseliyor.\n\r" );
 	break;

     case 19:
 	weather_info.sunlight = SUN_SET;
 	wcscat(buf, L"Güneş batıda yavaşça kayboluyor.\n\r" );
 	break;

     case 20:
 	weather_info.sunlight = SUN_DARK;
 	wcscat(buf, L"Gece başladı.\n\r" );
 	break;
     }

     /*
      * Weather change.
      */
     if ( time_info.month >= 9 && time_info.month <= 12 )
 	diff = weather_info.mmhg >  985 ? -2 : 2;
     else
 	diff = weather_info.mmhg > 1015 ? -2 : 2;

     weather_info.change   += diff * dice(1, 4) + dice(2, 6) - dice(2, 6);
     weather_info.change    = UMAX(weather_info.change, -12);
     weather_info.change    = UMIN(weather_info.change,  12);

     weather_info.mmhg += weather_info.change;
     weather_info.mmhg  = UMAX(weather_info.mmhg,  960);
     weather_info.mmhg  = UMIN(weather_info.mmhg, 1040);

     switch ( weather_info.sky )
     {
     default:
 	bug( L"Weather_update: bad sky %d.", weather_info.sky );
 	weather_info.sky = SKY_CLOUDLESS;
 	break;

     case SKY_CLOUDLESS:
 	if ( weather_info.mmhg <  990
 	|| ( weather_info.mmhg < 1010 && number_range(0,3) == 0 ) )
 	{
 	    wcscat(buf, L"Gökyüzü bulutlanıyor.\n\r" );
 	    weather_info.sky = SKY_CLOUDY;
 	}
 	break;

     case SKY_CLOUDY:
 	if ( weather_info.mmhg <  970
 	|| ( weather_info.mmhg <  990 && number_range(0,3) == 0 ) )
 	{
 	    wcscat(buf, L"Yağmur başladı.\n\r" );
 	    weather_info.sky = SKY_RAINING;
 	}

 	if ( weather_info.mmhg > 1030 && number_range(0,3) == 0 )
 	{
 	    wcscat(buf, L"Bulutlar dağılıyor.\n\r" );
 	    weather_info.sky = SKY_CLOUDLESS;
 	}
 	break;

     case SKY_RAINING:
 	if ( weather_info.mmhg <  970 && number_range(0,3) == 0 )
 	{
 	    wcscat(buf, L"Gökyüzünde şimşekler çakıyor.\n\r" );
 	    weather_info.sky = SKY_LIGHTNING;
 	}

 	if ( weather_info.mmhg > 1030
 	|| ( weather_info.mmhg > 1010 && number_range(0,3) == 0 ) )
 	{
 	    wcscat(buf, L"Yağmur dindi.\n\r" );
 	    weather_info.sky = SKY_CLOUDY;
 	}
 	break;

     case SKY_LIGHTNING:
 	if ( weather_info.mmhg > 1010
 	|| ( weather_info.mmhg >  990 && number_range(0,3) == 0 ) )
 	{
 	    wcscat(buf, L"Şimşekler durdu.\n\r" );
 	    weather_info.sky = SKY_RAINING;
 	    break;
 	}
 	break;
     }

     if ( buf[0] != '\0' )
     {
 	for ( d = descriptor_list; d != NULL; d = d->next )
 	{
 	    if ( d->connected == CON_PLAYING
 	    &&   IS_OUTSIDE(d->character)
 	    &&   IS_AWAKE(d->character) )
 		send_to_char( buf, d->character );
 	}
     }

     return;
 }


void ikikat_update( void )
{
  if (ikikat_tp > 0 )
  {
    ikikat_tp--;
  }
  if( ikikat_gp > 0 )
  {
    ikikat_gp--;
  }
}


/*
 * Update all chars, including mobs.
*/
void char_update( void )
{
    CHAR_DATA *ch;
    CHAR_DATA *ch_next;
    CHAR_DATA *ch_quit;

    static time_t last_save_time = -1;

    ch_quit = NULL;


    /* update save counter */
    save_number++;

    if (save_number > 29)
	save_number = 0;

    for ( ch = char_list; ch != NULL; ch = ch_next )
    {
	AFFECT_DATA *paf;
	AFFECT_DATA *paf_next;

	ch_next = ch->next;

  if (!IS_NPC(ch) && (IS_SET(ch->act,PLR_GHOST)))
  {
    if(ch->pcdata->ghost_mode_counter > 0)
    {
      ch->pcdata->ghost_mode_counter--;
    }
    if(ch->pcdata->ghost_mode_counter <= 0)
    {
      REMOVE_BIT(ch->act,PLR_GHOST);
      ch->pcdata->ghost_mode_counter = 0;
      printf_to_char(ch,L"Ete kemiğe büründüğünü hissediyorsun. Arkanı kollamaya başlasan iyi olur!\n\r");
      act ( L"$n ete kemiğe bürünüyor!",ch,NULL,NULL,TO_ROOM);
      while ( ch->affected )
        affect_remove( ch, ch->affected );
      ch->affected_by	= 0;
      ch->detection	= 0;
    }

  }

  if (!IS_NPC(ch) && (wcslen(ch->description)<350) && (ch->level >= KIDEMLI_OYUNCU_SEVIYESI))
  {
    SET_BIT(ch->act, PLR_NO_DESCRIPTION);
  }
  if (!IS_NPC(ch) && (wcslen(ch->description)>=350) && (IS_SET(ch->act, PLR_NO_DESCRIPTION)))
    REMOVE_BIT(ch->act, PLR_NO_DESCRIPTION);

	/* reset hunters path find */
	if (!IS_NPC(ch) && ch->cabal == CABAL_HUNTER)
	{
	 if (number_percent() < get_skill(ch,gsn_path_find) )
	  {
	   ch->endur += (get_skill(ch,gsn_path_find) / 2);
	   check_improve(ch,gsn_path_find,TRUE,8);
	   }
	 else
	   {
	    check_improve(ch,gsn_path_find,FALSE,16);
	   }
	}

	if (!IS_NPC(ch) && ch->cabal == CABAL_BATTLE)
	{
	 if (!is_affected(ch,gsn_spellbane)) do_spellbane(ch,(wchar_t*)"");
	}

        /* Remove caltraps effect after fight off */
        if (is_affected(ch,gsn_caltraps) && !ch->fighting)
          affect_strip(ch,gsn_caltraps);

        /* Remove vampire effect when morning. */
        if (IS_VAMPIRE(ch) &&
	    (weather_info.sunlight == SUN_LIGHT ||
		weather_info.sunlight == SUN_RISE) )
          do_human(ch,(wchar_t*)"");

        /* Reset sneak for vampire */
        if ( !(ch->fighting) && !IS_AFFECTED(ch,AFF_SNEAK) &&
            IS_VAMPIRE(ch) && !MOUNTED(ch))
	{
    send_to_char( L"Süzülmeye başladın.\n\r",ch);
	  SET_BIT(ch->affected_by ,AFF_SNEAK);
	}

        if ( !(ch->fighting) && !IS_AFFECTED(ch,AFF_SNEAK) &&
            (race_table[RACE(ch)].aff & AFF_SNEAK) && !MOUNTED(ch) )
            send_to_char( L"Süzülmeye başladın.\n\r",ch);

        if ( !(ch->fighting) && !IS_AFFECTED(ch,AFF_HIDE) &&
            (race_table[RACE(ch)].aff & AFF_HIDE) && !MOUNTED(ch) )
            send_to_char( L"Gölgelerin içine giriyorsun.\n\r",ch);

        SET_BIT(ch->affected_by, race_table[RACE(ch)].aff );

	if (!IS_NPC(ch) && IS_SET(ch->act,PLR_CHANGED_AFF))
		REMOVE_BIT(ch->affected_by,AFF_FLYING);

	if (MOUNTED(ch)) REMOVE_BIT(ch->affected_by,(C|D|P|Q));

        if ( ch->timer > 20 && !IS_NPC(ch) )
            ch_quit = ch;

	if ( ch->position >= POS_STUNNED )
	{
            /* check to see if we need to go home */
            if (IS_NPC(ch) && ch->zone != NULL && ch->zone != ch->in_room->area
            && ch->desc == NULL &&  ch->fighting == NULL && ch->progtypes==0
	    && !IS_AFFECTED(ch,AFF_CHARM) && ch->last_fought == NULL
	    && !RIDDEN(ch) && number_percent() < 15)
            {
		if (ch->in_mind != NULL
		    && ch->pIndexData->vnum > 100)
			back_home( ch );
		else
		{
      act( L"$n evine gitti.",ch,NULL,NULL,TO_ROOM);
            	 extract_char(ch,TRUE);
		}
            	continue;
            }

	    if ( ch->hit  < ch->max_hit )
		ch->hit  += hit_gain(ch);
	    else
		ch->hit = ch->max_hit;

	    if ( ch->mana < ch->max_mana )
		ch->mana += mana_gain(ch);
	    else
		ch->mana = ch->max_mana;

	    if ( ch->move < ch->max_move )
		ch->move += move_gain(ch);
	    else
		ch->move = ch->max_move;
	}

	if ( ch->position == POS_STUNNED )
	    update_pos( ch );

	if ( !IS_NPC(ch) && ch->level < LEVEL_IMMORTAL )
	{
	    OBJ_DATA *obj;

	    if ( ( obj = get_light_char( ch ) ) != NULL
	    &&   obj->item_type == ITEM_LIGHT
	    &&   obj->value[2] > 0 )
	    {
		if ( --obj->value[2] == 0 && ch->in_room != NULL )
		{
		    unequip_char(ch, obj);
		    if (get_light_char(ch) == NULL) --ch->in_room->light;
        act( L"$p sönüyor.", ch, obj, NULL, TO_ROOM );
		    act( L"$p titreyerek sönüyor.", ch, obj, NULL, TO_CHAR );
		    extract_obj( obj );
		}
	 	else if ( obj->value[2] <= 5 && ch->in_room != NULL)
    act( L"$p titriyor.",ch,obj,NULL,TO_CHAR);
	    }

	    if (IS_IMMORTAL(ch))
		ch->timer = 0;

	    if ( ++ch->timer >= 12 )
	    {
		if ( ch->was_in_room == NULL && ch->in_room != NULL )
		{
		    ch->was_in_room = ch->in_room;
		    if ( ch->fighting != NULL )
			stop_fighting( ch, TRUE );
      act( L"$n hiçliğe sürükleniyor.",
			ch, NULL, NULL, TO_ROOM );
      send_to_char( L"Hiçliğe sürükleniyorsun.\n\r", ch );
		    if (ch->level > 1  )
		        save_char_obj( ch );
		    if ( ch->level < 10 )  {
  		      char_from_room( ch );
		      char_to_room( ch, get_room_index( ROOM_VNUM_LIMBO ) );
		    }
		}
	    }

	    gain_condition( ch, COND_DRUNK,  -1 );
	    if (ch->iclass == CLASS_VAMPIRE && ch->level > 10)
		gain_condition( ch,COND_BLOODLUST, -1);
	    gain_condition( ch, COND_FULL, ch->size > SIZE_MEDIUM ? -4 : -2 );
	    if ( ch->in_room->sector_type == SECT_DESERT )
	      gain_condition( ch, COND_THIRST, -3 );
	    else
	      gain_condition( ch, COND_THIRST, -1 );
	    gain_condition( ch, COND_HUNGER, ch->size > SIZE_MEDIUM ? -2 : -1);
	}

	for ( paf = ch->affected; paf != NULL; paf = paf_next )
	{
	    paf_next	= paf->next;
	    if ( paf->duration > 0 )
	    {
		paf->duration--;

		if (number_range(0,4) == 0 && paf->level > 0)
		  paf->level--;
		/* spell strength fades with time */
            }
	    else if ( paf->duration < 0 )
		;
	    else
	    {
		if ( paf_next == NULL
		||   paf_next->type != paf->type
		||   paf_next->duration > 0 )
		{
		    if ( paf->type > 0 && skill_table[paf->type].msg_off )
		    {
			send_to_char( skill_table[paf->type].msg_off, ch );
			send_to_char( L"\n\r", ch );
		    }
		}

		if ( paf->type == gsn_strangle )
		{
			AFFECT_DATA neck_af;

			REMOVE_BIT(ch->affected_by, AFF_SLEEP);
			do_wake(ch, (wchar_t*)"");
			neck_af.type = gsn_neckguard;
			neck_af.where = TO_AFFECTS;
			neck_af.level = ch->level;
			neck_af.duration = 2 + ch->level / 50;
			neck_af.modifier = 0;
			neck_af.bitvector = 0;
			neck_af.location  = APPLY_NONE;
			affect_join(ch, &neck_af);
		}
		else if ( paf->type == gsn_blackjack )
		{
			AFFECT_DATA head_af;

			REMOVE_BIT(ch->affected_by, AFF_SLEEP);
			do_wake(ch,(wchar_t*) "");
			head_af.type = gsn_headguard;
			head_af.where = TO_AFFECTS;
			head_af.level = ch->level;
			head_af.duration = 2 + ch->level / 50;
			head_af.modifier = 0;
			head_af.bitvector = 0;
			head_af.location  = APPLY_NONE;
			affect_join(ch, &head_af);
		}
		else if ( paf->type == gsn_vampiric_touch )
		{
			AFFECT_DATA b_af;

			REMOVE_BIT(ch->affected_by, AFF_SLEEP);
			do_wake(ch, (wchar_t*)"");
			b_af.type = gsn_blackguard;
			b_af.where = TO_AFFECTS;
			b_af.level = ch->level;
			b_af.duration = 2 + ch->level / 50;
			b_af.modifier = 0;
			b_af.bitvector = 0;
			b_af.location  = APPLY_NONE;
			affect_join(ch, &b_af);
		}

		affect_remove( ch, paf );
	    }
	}

	/*
	 * Careful with the damages here,
	 *   MUST NOT refer to ch after damage taken,
	 *   as it may be lethal damage (on NPC).
	 */

	if (is_affected(ch, gsn_witch_curse))
	{
	  AFFECT_DATA *af,witch;

	  if (ch->in_room == NULL)
		continue;

    act( L"$s üzerindeki cadı laneti ona yaşamının emildiğini hissettiriyor.\n\r",ch,NULL,NULL,TO_ROOM);
	  send_to_char( L"Cadı laneti yaşamının emildiğini hissettiriyor.\n\r",ch);

	  for( af = ch->affected; af!= NULL; af = af->next)
	  {
		if (af->type == gsn_witch_curse)
			break;
	  }

	  if (af == NULL)
		 continue;

	  if (af->level == 1)
		 continue;

	  if (af->modifier > -16001)
	  {
	    witch.where = af->where;
	    witch.type  = af->type;
	    witch.level = af->level;
	    witch.duration = af->duration;
	    witch.location = af->location;
	    witch.modifier = af->modifier * 2;
	    witch.bitvector = 0;

	    affect_remove(ch, af);
	    affect_to_char( ch ,&witch);
	    ch->hit = UMIN(ch->hit,ch->max_hit);
	    if (ch->hit < 1)
	    {
		affect_strip(ch,gsn_witch_curse);
		ch->hit = 1;
		damage(ch,ch,16,gsn_witch_curse,DAM_NONE,FALSE);
		continue;
	    }
          }
          else
          {
	    affect_strip(ch,gsn_witch_curse);
	    ch->hit = 1;
	    damage(ch,ch,16,gsn_witch_curse,DAM_NONE,FALSE);
	    continue;
          }
	}

        if (IS_AFFECTED(ch, AFF_PLAGUE) && ch != NULL)
        {
            AFFECT_DATA *af, plague;
            CHAR_DATA *vch;
            int dam;

	    if (ch->in_room == NULL)
		 continue;

     act( L"$n veba derisini yaktıkça acı içinde kıvranıyor.",ch,NULL,NULL,TO_ROOM);
     send_to_char( L"Veba derini yaktıkça acı içinde kıvranıyorsun.\n\r",ch);
            for ( af = ch->affected; af != NULL; af = af->next )
            {
            	if (af->type == gsn_plague)
                    break;
            }

            if (af == NULL)
            {
            	REMOVE_BIT(ch->affected_by,AFF_PLAGUE);
                continue;
            }

            if (af->level == 1)
		 continue;

	    plague.where		= TO_AFFECTS;
            plague.type 		= gsn_plague;
            plague.level 		= af->level - 1;
            plague.duration 	= number_range(1,2 * plague.level);
            plague.location		= APPLY_STR;
            plague.modifier 	= -5;
            plague.bitvector 	= AFF_PLAGUE;

            for ( vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
            {
                if (!saves_spell(plague.level + 2,vch,DAM_DISEASE)
		&&  !IS_IMMORTAL(vch)
            	&&  !IS_AFFECTED(vch,AFF_PLAGUE) && number_range(0,3) == 0)
            	{
                send_to_char( L"Ateşinin yükseldiğini hissediyorsun.\n\r",vch);
                act( L"$n hastalıktan titriyor.",vch,NULL,NULL,TO_ROOM);
            	    affect_join(vch,&plague);
            	}
            }

	    dam = UMIN(ch->level,af->level/5+1);
	    ch->mana -= dam;
	    ch->move -= dam;
            if (number_percent() < 70 )
  	      damage( ch, ch, dam + UMAX(ch->max_hit/20, 50), gsn_plague,DAM_DISEASE,TRUE);
	    else damage( ch, ch, dam, gsn_plague,DAM_DISEASE,FALSE);
        }
	else if ( IS_AFFECTED(ch, AFF_POISON) && ch != NULL
	     &&   !IS_AFFECTED(ch,AFF_SLOW))

	{
	    AFFECT_DATA *poison;

	    poison = affect_find(ch->affected,gsn_poison);

	    if (poison != NULL)
	    {
        act( L"$n acı çekiyor ve titriyor.", ch, NULL, NULL, TO_ROOM );
        send_to_char( L"Acı çekiyor ve titriyorsun.\n\r", ch );
	        damage(ch,ch,poison->level/10 + 1,gsn_poison,
		    DAM_POISON,TRUE);
	    }
	}

	else if ( ch->position == POS_INCAP && number_range(0,1) == 0)
	{
	    damage( ch, ch, 1, TYPE_UNDEFINED, DAM_NONE,FALSE);
	}
	else if ( ch->position == POS_MORTAL )
	{
	    damage( ch, ch, 1, TYPE_UNDEFINED, DAM_NONE,FALSE);
	}
    }

    /*
     * Autosave and autoquit.
     * Check that these chars still exist.
     */

    if (last_save_time == -1 || current_time - last_save_time > 300)
    {
      last_save_time = current_time;
      for (ch = char_list; ch != NULL; ch = ch_next)
          {
            ch_next = ch->next;
	    if ( !IS_NPC(ch) )
              save_char_obj(ch);
            if ( ch == ch_quit || ch->timer > 20 )
              do_quit(ch, (wchar_t*)"");
          }
    }

    return;
}



void water_float_update( void )
{
OBJ_DATA *obj_next;
OBJ_DATA *obj;
CHAR_DATA *ch;
bool fChar;

    for ( obj = object_list; obj != NULL; obj = obj_next )
    {
 	obj_next = obj->next;

	if ( obj == NULL ) {dump_to_scr((wchar_t*)"NULL OBJ encounter!\n\r");break;}

	if ( obj->in_room == NULL )
	  continue;


	if ( IS_WATER( obj->in_room ) ) {

	  fChar = FALSE;
	  ch = obj->in_room->people;
	  if (ch != NULL)
	    fChar = TRUE;
	  if (obj->water_float != -1)
	    obj->water_float--;

	  if (obj->water_float < 0) obj->water_float = -1;

	  if (obj->item_type == ITEM_DRINK_CON)  {
	     obj->value[1] = URANGE( 1, obj->value[1]+8 , obj->value[0] );
	     if ( fChar )  {
         act( L"$p suda baloncuklar oluşturuyor.", ch, obj, NULL, TO_CHAR);
	       act( L"$p suda baloncuklar oluşturuyor.", ch, obj, NULL, TO_ROOM);
	     }
	     obj->water_float = obj->value[0]-obj->value[1];
	     obj->value[2] = 0;
	  }
  	  if ( obj->water_float == 0 )  {
	    if ( ((obj->item_type == ITEM_CORPSE_NPC) ||
		  (obj->item_type == ITEM_CORPSE_PC)  ||
		  (obj->item_type == ITEM_CONTAINER)) &&
		  fChar )  {
        act( L"$p ardında baloncuklar bırakarak batıyor.", ch, obj, NULL, TO_CHAR );
        act( L"$p ardında baloncuklar bırakarak batıyor.", ch, obj, NULL, TO_ROOM );
	    }
  	    else if ( fChar ) {
          act( L"$p suda batıyor.", ch, obj, NULL, TO_CHAR );
  	      act( L"$p suda batıyor.", ch, obj, NULL, TO_ROOM );
	    }
	    extract_obj( obj );
	    continue;
	  }
       }
    }

	return;

}

/*
 * Update all objs.
 * This function is performance sensitive.
 */
void obj_update( void )
{
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    OBJ_DATA *t_obj, *pit, *next_obj;

    AFFECT_DATA *paf, *paf_next;
    static int pit_count = 1;

    for ( obj = object_list; obj != NULL; obj = obj_next )
    {
	CHAR_DATA *rch;
	const wchar_t *message;

	obj_next = obj->next;

	/* go through affects and decrement */
        for ( paf = obj->affected; paf != NULL; paf = paf_next )
        {
            paf_next    = paf->next;
            if ( paf->duration > 0 )
            {
                paf->duration--;
                if (number_range(0,4) == 0 && paf->level > 0)
                  paf->level--;  /* spell strength fades with time */
            }
            else if ( paf->duration < 0 )
                ;
            else
            {
                if ( paf_next == NULL
                ||   paf_next->type != paf->type
                ||   paf_next->duration > 0 )
                {
                    if ( paf->type > 0 && skill_table[paf->type].msg_obj )
                    {
			if (obj->carried_by != NULL)
			{
			    rch = obj->carried_by;
			    act(skill_table[paf->type].msg_obj,
				rch,obj,NULL,TO_CHAR);
			}
			if (obj->in_room != NULL
			&& obj->in_room->people != NULL)
			{
			    rch = obj->in_room->people;
			    act(skill_table[paf->type].msg_obj,
				rch,obj,NULL,TO_ALL);
			}
                    }
                }

                affect_remove_obj( obj, paf );
            }
        }


        for(t_obj = obj; t_obj->in_obj; t_obj = t_obj->in_obj);

        if (IS_SET(obj->progtypes,OPROG_AREA))
           if ( ( t_obj->in_room != NULL &&
                (t_obj->in_room->area->nplayer > 0))
               ||
                (t_obj->carried_by &&
                 t_obj->carried_by->in_room &&
                 t_obj->carried_by->in_room->area->nplayer > 0) )
              (obj->pIndexData->oprogs->area_prog) (obj);

        if ( check_material( obj, (wchar_t*)"ice" ) )
          {
	   if ( obj->carried_by != NULL )
             {
	      if ( obj->carried_by->in_room->sector_type == SECT_DESERT )
	      if ( number_percent() < 40 )
              {
                act( L"$p aşırı sıcaktan eriyor.", obj->carried_by, obj, NULL, TO_CHAR );
		extract_obj( obj );
		continue;
	      }
	  }
	  else if ( obj->in_room != NULL )
	    if ( obj->in_room->sector_type == SECT_DESERT )
	      if ( number_percent() < 50 )  {
		if ( obj->in_room->people != NULL )
                {
                  act( L"$p aşırı sıcaktan eriyor.", obj->in_room->people, obj, NULL, TO_ROOM );
            		  act( L"$p aşırı sıcaktan eriyor.", obj->in_room->people, obj, NULL, TO_CHAR );
	        }
		extract_obj( obj );
		continue;
	      }
	}

        if ( !check_material( obj, (wchar_t*)"glass" ) && obj->item_type==ITEM_POTION)  {
	  if ( obj->carried_by != NULL )  {
	    if ( obj->carried_by->in_room->sector_type == SECT_DESERT &&
		 !IS_NPC(obj->carried_by) )
	      if ( number_percent() < 20 )  {
          act( L"$p buharlaşıyor.", obj->carried_by, obj, NULL, TO_CHAR );
		extract_obj( obj );
		continue;
	      }
	  }
	  else if ( obj->in_room != NULL )
	    if ( obj->in_room->sector_type == SECT_DESERT )
	      if ( number_percent() < 30 )
               {
		if ( obj->in_room->people != NULL )
                {
                  act( L"$p aşırı sıcaktan buharlaşıyor.", obj->in_room->people, obj, NULL, TO_ROOM );
            		  act( L"$p aşırı sıcaktan buharlaşıyor.", obj->in_room->people, obj, NULL, TO_CHAR );
	        }
	        extract_obj( obj );
	        continue;
	       }
	}

	if ( obj->condition > -1 && (obj->timer <= 0 || --obj->timer > 0) )
	    continue;

	switch ( obj->item_type )
	{
    default:              message = L"$p ufalanıyor.";break;
  	case ITEM_FOUNTAIN:   message = L"$p kuruyor.";break;
  	case ITEM_CORPSE_NPC: message = L"$p çürüyor.";break;
  	case ITEM_CORPSE_PC:  message = L"$p çürüyor.";break;
  	case ITEM_FOOD:       message = L"$p bozuluyor.";break;
  	case ITEM_POTION:     message = L"$p beklemekten buharlaşıyor.";break;
  	case ITEM_PORTAL:     message = L"$p kayboluyor.";break;
	case ITEM_CONTAINER:
	    if (CAN_WEAR(obj,ITEM_WEAR_FLOAT))
		if (obj->contains)
    message = L"$p içindekileri etrafa saçarak yokoluyor.";
		else
    message = L"$p titriyor ve yokoluyor.";
	    else
      message = L"$p ufalanıyor.";
	    break;
	}

	if ( obj->carried_by != NULL )
	{
	    if (IS_NPC(obj->carried_by)
	    &&  obj->carried_by->pIndexData->pShop != NULL)
		obj->carried_by->silver += obj->cost/5;
	    else
	    {
	    	act( message, obj->carried_by, obj, NULL, TO_CHAR );
		if ( obj->wear_loc == WEAR_FLOAT)
		    act(message,obj->carried_by,obj,NULL,TO_ROOM);
	    }
	}
	else if ( obj->in_room != NULL
	&&      ( rch = obj->in_room->people ) != NULL )
	{
	    if (! (obj->in_obj && obj->in_obj->pIndexData->vnum == OBJ_VNUM_PIT
	           && !CAN_WEAR(obj->in_obj,ITEM_TAKE)))
	    {
	    	act( message, rch, obj, NULL, TO_ROOM );
	    	act( message, rch, obj, NULL, TO_CHAR );
	    }
	}

        pit_count = (pit_count+1) % 120; /* more or less an hour */
        if (obj->pIndexData->vnum == OBJ_VNUM_PIT &&
            pit_count == 121) {
          for (t_obj = obj->contains; t_obj != NULL; t_obj = next_obj) {
            next_obj = t_obj->next_content;
            obj_from_obj(t_obj);
            extract_obj(t_obj);
          }
        }


        if ((obj->item_type == ITEM_CORPSE_PC || obj->wear_loc == WEAR_FLOAT)
	&&  obj->contains)
	{   /* save the contents */
     	    OBJ_DATA *t_obj, *next_obj;

	    for (t_obj = obj->contains; t_obj != NULL; t_obj = next_obj)
	    {
		next_obj = t_obj->next_content;
		obj_from_obj(t_obj);

		if (obj->in_obj) /* in another object */
		    obj_to_obj(t_obj,obj->in_obj);

		else if (obj->carried_by)  /* carried */
		    if (obj->wear_loc == WEAR_FLOAT)
			if (obj->carried_by->in_room == NULL)
			    extract_obj(t_obj);
			else
			    obj_to_room(t_obj,obj->carried_by->in_room);
		    else
		    	obj_to_char(t_obj,obj->carried_by);

		else if (obj->in_room == NULL)  /* destroy it */
		    extract_obj(t_obj);

                else { /* to the pit */
                  for (pit = get_room_index(obj->altar)->contents;
                       pit != NULL && pit->pIndexData->vnum != obj->pit;
                       pit = pit->next);

                  if (pit == NULL)
                    obj_to_room(t_obj,obj->in_room);
                  else obj_to_obj(t_obj,pit);
                }
	    }
	}

	extract_obj( obj );
    }

    return;
}



/*
 * Aggress.
 *
 * for each mortal PC
 *     for each mob in room
 *         aggress on some random PC
 *
 * This function takes 25% to 35% of ALL Merc cpu time.
 * Unfortunately, checking on each PC move is too tricky,
 *   because we don't the mob to just attack the first PC
 *   who leads the party into the room.
 *
 * -- Furey
 */
void aggr_update( void )
{
    CHAR_DATA *wch;
    CHAR_DATA *wch_next;
    CHAR_DATA *ch;
    CHAR_DATA *ch_next;
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    CHAR_DATA *victim;
    wchar_t buf[MAX_STRING_LENGTH];

    for ( wch = char_list; wch != NULL; wch = wch_next )
    {
	if (!IS_VALID(wch))
	{
	  bug( L"Aggr_update: Invalid char.",0);
	  break;
	}

	wch_next = wch->next;

	if (IS_AFFECTED(wch,AFF_BLOODTHIRST) &&
          IS_AWAKE(wch) && wch->fighting == NULL)
        {
          for ( vch = wch->in_room->people;
               vch != NULL && wch->fighting == NULL; vch = vch_next)
            {
              vch_next = vch->next_in_room;
              if ( wch != vch && can_see(wch,vch) &&
                  !is_safe_nomessage(wch,vch) )
                {
                  act_color( L"$CDAHA FAZLA KAN! DAHA FAZLA KAAN!!!$c",
                            wch,NULL,NULL,TO_CHAR,POS_RESTING,CLR_RED);
                  do_murder(wch,vch->name);
                }
            }
        }

	if ( wch->cabal != CABAL_NONE && IS_NPC(wch))
	{

	    for ( ch = wch->in_room->people; ch != NULL; ch = ch_next )
	    {

		ch_next	= ch->next_in_room;
		if ( !IS_NPC(ch)
		&& !IS_IMMORTAL(ch)
		&& ch->cabal != wch->cabal
		&& ch->fighting == NULL )

		    multi_hit(wch, ch, TYPE_UNDEFINED);

	    }
	    continue;
	}

	if ( IS_NPC(wch)
	||   wch->level >= LEVEL_IMMORTAL
	||   wch->in_room == NULL
	||   wch->in_room->area->empty)
	    continue;

	for ( ch = wch->in_room->people; ch != NULL; ch = ch_next )
	{
	    int count;

	    ch_next	= ch->next_in_room;

	    if ( !IS_NPC(ch)
	    ||   (!IS_SET(ch->act, ACT_AGGRESSIVE) && (ch->last_fought == NULL))
	    ||   IS_SET(ch->in_room->room_flags,ROOM_SAFE)
	    ||   IS_AFFECTED(ch,AFF_CALM)
	    ||   ch->fighting != NULL
	    ||	 RIDDEN(ch)
	    ||   IS_AFFECTED(ch, AFF_CHARM)
	    ||   IS_AFFECTED(ch, AFF_SCREAM)
	    ||   !IS_AWAKE(ch)
	    ||   ( IS_SET(ch->act, ACT_WIMPY) && IS_AWAKE(wch) )
	    ||   !can_see( ch, wch )
	    ||   number_range(0,1) == 0
            ||   is_safe_nomessage(ch,wch))

		continue;

          /* Mad mob attacks! */
          if ( ch->last_fought == wch )
	   {
       swprintf( buf, MAX_STRING_LENGTH-1, L"%s! Öleceksin!",
                    (is_affected(wch,gsn_doppelganger) &&
                     !IS_SET(ch->act,PLR_HOLYLIGHT))?
                    PERS(wch->doppel,ch) : PERS(wch,ch));
            do_yell(ch,buf);
            wch = check_guard(wch, ch);

            multi_hit(ch,wch,TYPE_UNDEFINED);
            continue;
          }

          if (ch->last_fought != NULL)
            continue;

	    /*
	     * Ok we have a 'wch' player character and a 'ch' npc aggressor.
	     * Now make the aggressor fight a RANDOM pc victim in the room,
	     *   giving each 'vch' an equal chance of selection.
	     */
	    count	= 0;
	    victim	= NULL;
	    for ( vch = wch->in_room->people; vch != NULL; vch = vch_next )
	    {
		vch_next = vch->next_in_room;

		if ( !IS_NPC(vch)
		&&   vch->level < LEVEL_IMMORTAL
		&&   ch->level >= vch->level - 5
		&&   ( !IS_SET(ch->act, ACT_WIMPY) || !IS_AWAKE(vch) )
		&&   can_see( ch, vch )
		&&   vch->iclass != CLASS_VAMPIRE /* do not attack vampires */
		&&   !(IS_GOOD(ch) && IS_GOOD(vch)) ) /* good vs good :( */
		{
		    if ( number_range( 0, count ) == 0 )
			victim = vch;
		    count++;
		}
	    }

	    if ( victim == NULL )
		continue;

	    if ( !is_safe_nomessage( ch, victim ) )
	    {
              victim = check_guard(victim, ch);
	      if (IS_SET(ch->off_flags, OFF_BACKSTAB)
		  && get_wield_char(ch, FALSE) )
		multi_hit( ch, victim, gsn_backstab );
	      else multi_hit( ch, victim, TYPE_UNDEFINED );
	    }
	}
    }

    return;
}



/*
 * Handle all kinds of updates.
 * Called once per pulse from game loop.
 * Random times to defeat tick-timing clients and players.
 */

void update_handler( void )
{
    static  int     pulse_area;
    static  int     pulse_mobile;
    static  int     pulse_violence;
    static  int     pulse_point;
    static  int	    pulse_music;
    static  int	    pulse_water_float;
    static  int	    pulse_raffect;
    static  int	    pulse_track;

    game_time_update();

    if ( --pulse_area     <= 0 )
    {
	wiznet( L"BOLGE & ODA YENILEME!",NULL,NULL,WIZ_TICKS,0,0);
	pulse_area	= PULSE_AREA;
	area_update	( );
	room_update	( );
    }

    if ( --pulse_music	  <= 0 )
    {
	pulse_music	= PULSE_MUSIC;
/*	song_update(); */
    }

    if ( --pulse_mobile   <= 0 )
    {
	pulse_mobile	= PULSE_MOBILE;
	mobile_update	( );
	light_update	( );
    }

    if ( --pulse_violence <= 0 )
    {
	pulse_violence	= PULSE_VIOLENCE;
	violence_update	( );
    }

    if ( --pulse_water_float <= 0 )
    {
	pulse_water_float = PULSE_WATER_FLOAT;
	water_float_update( );
    }

    if ( --pulse_raffect <= 0 )
    {
	pulse_raffect = PULSE_RAFFECT;
	room_affect_update( );
    }

    if ( --pulse_track <= 0 )
    {
	pulse_track = PULSE_TRACK;
	track_update( );
    }

    if ( --pulse_point    <= 0 )
    {
	wiznet( L"KARAKTER YENILEME!",NULL,NULL,WIZ_TICKS,0,0);
	pulse_point     = PULSE_TICK;
	weather_update	( );
	char_update	( );
	quest_update    ( );
  ikikat_update    ( );
	obj_update	( );
	check_reboot	( );

	/* room counting */
	{
	 CHAR_DATA *ch;

	 for (ch = char_list; ch != NULL; ch = ch->next)
            if (!IS_NPC(ch) && ch->in_room != NULL)
                    ch->in_room->area->count =
                      UMIN(ch->in_room->area->count+1,5000000);
	}

    }

    aggr_update( );
    auction_update( );

    tail_chain( );

    return;
}

void light_update( void )
{
    CHAR_DATA *ch;
    int dam_light;
    DESCRIPTOR_DATA *d, *d_next;


    for ( d = descriptor_list; d != NULL; d = d_next )
	{
	 d_next = d->next;
	 if ( d->connected != CON_PLAYING )
		continue;

	ch = (d->original != NULL) ? d->original : d->character;

	if (IS_IMMORTAL(ch) ) continue;

        if ( ch->iclass != CLASS_VAMPIRE)   continue;

	/* also checks vampireness */
	if ( (dam_light = isn_dark_safe(ch)) == 0 )
		continue;

	if (dam_light != 2 && number_percent() < get_skill(ch,gsn_light_res))
	{
	    check_improve(ch,gsn_light_res,TRUE,32);
	    continue;
	}

	if (dam_light == 1)
  send_to_char( L"Odadaki ışıktan rahatsız oluyorsun.\n\r",ch);
else send_to_char( L"Güneş ışığından rahatsız oluyorsun.\n\r",ch);

	dam_light = ( ch->max_hit * 4 )/ 100;
	if (!dam_light) dam_light = 1;
	damage(ch, ch, dam_light, TYPE_HUNGER, DAM_LIGHT_V , TRUE);

	if ( ch->position == POS_STUNNED )
	    update_pos( ch );

	if (number_percent() < 10)  gain_condition( ch, COND_DRUNK,  -1 );
	}


    return;
}


void room_update( void )
{
    ROOM_INDEX_DATA *room;
    ROOM_INDEX_DATA *room_next;

    for ( room = top_affected_room; room ; room = room_next )
    {
	AFFECT_DATA *paf;
	AFFECT_DATA *paf_next;

	room_next = room->aff_next;

	for ( paf = room->affected; paf != NULL; paf = paf_next )
	{
	    paf_next	= paf->next;
	    if ( paf->duration > 0 )
	    {
		paf->duration--;
/*
		if (number_range(0,4) == 0 && paf->level > 0)
		  paf->level--;
	 spell strength shouldn't fade with time
	 because checks safe_rpsell with af->level */
            }
	    else if ( paf->duration < 0 )
		;
	    else
	    {
		if ( paf_next == NULL
		||   paf_next->type != paf->type
		||   paf_next->duration > 0 )
		{
/*
		    if ( paf->type > 0 && skill_table[paf->type].msg_off )
		    {
			act( skill_table[paf->type].msg_off, ch );
			send_to_char( L"\n\r", ch );
		    }
*/
		}

		affect_remove_room( room, paf );
	    }
	}

    }
    return;
}

void room_affect_update( void )
{
    ROOM_INDEX_DATA *room;
    ROOM_INDEX_DATA *room_next;

    for ( room = top_affected_room; room ; room = room_next )
    {
	room_next = room->aff_next;

        while (IS_ROOM_AFFECTED(room, AFF_ROOM_PLAGUE) && room->people != NULL)
        {
            AFFECT_DATA *af, plague;
            CHAR_DATA *vch;

            for ( af = room->affected; af != NULL; af = af->next )
            {
            	if (af->type == gsn_black_death)
                    break;
            }

            if (af == NULL)
            {
            	REMOVE_BIT(room->affected_by,AFF_ROOM_PLAGUE);
            	break;
            }

            if (af->level == 1)
            	af->level = 2;

	    plague.where		= TO_AFFECTS;
            plague.type 		= gsn_plague;
            plague.level 		= af->level - 1;
            plague.duration 		= number_range(1,((plague.level/2)+1));
            plague.location		= APPLY_NONE;
            plague.modifier 		= -5;
            plague.bitvector 		= AFF_PLAGUE;

            for ( vch = room->people; vch != NULL; vch = vch->next_in_room)
            {
                if (!saves_spell(plague.level ,vch,DAM_DISEASE)
		&&  !IS_IMMORTAL(vch)
		&&  !is_safe_rspell(af->level,vch)
            	&&  !IS_AFFECTED(vch,AFF_PLAGUE) && number_range(0,7) == 0)
            	{
                send_to_char( L"Ateşinin çıktığını hissediyorsun.\n\r",vch);
                act( L"$n hasta görünüyor.",vch,NULL,NULL,TO_ROOM);
            	    affect_join(vch,&plague);
            	}
            }
	 break;
        }

        while (IS_ROOM_AFFECTED(room, AFF_ROOM_POISON) && room->people != NULL)
        {
            AFFECT_DATA *af, paf;
            CHAR_DATA *vch;

            for ( af = room->affected; af != NULL; af = af->next )
            {
            	if (af->type == gsn_deadly_venom)
                    break;
            }

            if (af == NULL)
            {
            	REMOVE_BIT(room->affected_by,AFF_ROOM_POISON);
            	break;
            }

            if (af->level == 1)
            	af->level = 2;

	    paf.where		= TO_AFFECTS;
            paf.type 		= gsn_poison;
            paf.level 		= af->level - 1;
            paf.duration 	= number_range(1,((paf.level/5)+1));
            paf.location	= APPLY_NONE;
            paf.modifier 	= -5;
            paf.bitvector 	= AFF_POISON;

            for ( vch = room->people; vch != NULL; vch = vch->next_in_room)
            {
                if (!saves_spell(paf.level ,vch,DAM_POISON)
		&&  !IS_IMMORTAL(vch)
		&&  !is_safe_rspell(af->level,vch)
            	&&  !IS_AFFECTED(vch,AFF_POISON) && number_range(0,7) == 0)
            	{
                send_to_char( L"Kendini hasta hissediyorsun.\n\r",vch);
                act( L"$n çok hasta görünüyor.",vch,NULL,NULL,TO_ROOM);
            	    affect_join(vch,&paf);
            	}
            }
	 break;
        }

        while (IS_ROOM_AFFECTED(room, AFF_ROOM_SLOW) && room->people != NULL)
        {
            AFFECT_DATA *af, paf;
            CHAR_DATA *vch;

            for ( af = room->affected; af != NULL; af = af->next )
            {
            	if (af->type == gsn_lethargic_mist)
                    break;
            }

            if (af == NULL)
            {
            	REMOVE_BIT(room->affected_by,AFF_ROOM_SLOW);
            	break;
            }

            if (af->level == 1)
            	af->level = 2;

	    paf.where		= TO_AFFECTS;
            paf.type 		= gsn_slow;
            paf.level 		= af->level - 1;
            paf.duration 	= number_range(1,((paf.level/5)+1));
            paf.location	= APPLY_NONE;
            paf.modifier 	= -5;
            paf.bitvector 	= AFF_SLOW;

            for ( vch = room->people; vch != NULL; vch = vch->next_in_room)
            {
                if (!saves_spell(paf.level ,vch,DAM_OTHER)
		&&  !IS_IMMORTAL(vch)
		&&  !is_safe_rspell(af->level,vch)
            	&&  !IS_AFFECTED(vch,AFF_SLOW) && number_range(0,7) == 0)
            	{
                send_to_char( L"Acelesiz hareket etmeye başlıyorsun.\n\r",vch);
                act( L"$s hareketlerindeki telaş kayboluyor.",vch,NULL,NULL,TO_ROOM);
            	    affect_join(vch,&paf);
            	}
            }
	 break;
        }

        while (IS_ROOM_AFFECTED(room, AFF_ROOM_SLEEP) && room->people != NULL)
        {
            AFFECT_DATA *af, paf;
            CHAR_DATA *vch;

            for ( af = room->affected; af != NULL; af = af->next )
            {
            	if (af->type == gsn_mysterious_dream)
                    break;
            }

            if (af == NULL)
            {
            	REMOVE_BIT(room->affected_by,AFF_ROOM_SLEEP);
            	break;
            }

            if (af->level == 1)
            	af->level = 2;

	    paf.where		= TO_AFFECTS;
            paf.type 		= gsn_sleep;
            paf.level 		= af->level - 1;
            paf.duration 	= number_range(1,((paf.level/10)+1));
            paf.location	= APPLY_NONE;
            paf.modifier 	= -5;
            paf.bitvector 	= AFF_SLEEP;

            for ( vch = room->people; vch != NULL; vch = vch->next_in_room)
            {
                if (!saves_spell(paf.level - 4,vch,DAM_CHARM)
		&&  !IS_IMMORTAL(vch)
		&&  !is_safe_rspell(af->level,vch)
		&&  !(IS_NPC(vch) && IS_SET(vch->act,ACT_UNDEAD) )
            	&&  !IS_AFFECTED(vch,AFF_SLEEP) && number_range(0,7) == 0)
            	{
		  if (IS_AWAKE(vch))
		   {
         send_to_char( L"Uykun geliyor.......zzzzzz.\n\r",vch);
         act( L"$n uykuya dalıyor.",vch,NULL,NULL,TO_ROOM);
		    vch->position = POS_SLEEPING;
		   }
          	  affect_join(vch,&paf);
            	}
            }
	 break;
        }


        while (IS_ROOM_AFFECTED(room, AFF_ROOM_ESPIRIT) && room->people != NULL)
        {
            AFFECT_DATA *af, paf;
            CHAR_DATA *vch;

            for ( af = room->affected; af != NULL; af = af->next )
            {
            	if (af->type == gsn_evil_spirit)
                    break;
            }

            if (af == NULL)
            {
            	REMOVE_BIT(room->affected_by,AFF_ROOM_ESPIRIT);
            	break;
            }

            if (af->level == 1)
            	af->level = 2;

	    paf.where		= TO_AFFECTS;
            paf.type 		= gsn_evil_spirit;
            paf.level 		= af->level;
            paf.duration 	= number_range(1,(paf.level/30));
            paf.location	= APPLY_NONE;
            paf.modifier 	= 0;
            paf.bitvector 	= 0;

            for ( vch = room->people; vch != NULL; vch = vch->next_in_room)
            {
                if (!saves_spell(paf.level + 2,vch,DAM_MENTAL)
		&&  !IS_IMMORTAL(vch)
		&&  !is_safe_rspell(af->level,vch)
            	&&  !is_affected(vch,gsn_evil_spirit) && number_range(0,7) == 0)
            	{
                send_to_char( L"Kendini bu kadar kötü hissetmemiştin.\n\r",vch);
                act( L"$s üstüne şerrin gölgesi düşüyor.",vch,NULL,NULL,TO_ROOM);
            	    affect_join(vch,&paf);
            	}
            }
	 break;
        }

/* new ones here
        while (IS_ROOM_AFFECTED(room, AFF_ROOM_) && room->people != NULL)
        {
            AFFECT_DATA *af, paf;
            CHAR_DATA *vch;

            for ( af = room->affected; af != NULL; af = af->next )
            {
            	if (af->type == gsn_)
                    break;
            }

            if (af == NULL)
            {
            	REMOVE_BIT(room->affected_by,AFF_ROOM_);
            	break;
            }

            if (af->level == 1)
            	af->level = 2;

	    paf.where		= TO_AFFECTS;
            paf.type 		= gsn_;
            paf.level 		= af->level - 1;
            paf.duration 	= number_range(1,((paf.level/5)+1));
            paf.location	= APPLY_NONE;
            paf.modifier 	= -5;
            paf.bitvector 	= AFF_;

            for ( vch = room->people; vch != NULL; vch = vch->next_in_room)
            {
                if (!saves_spell(paf.level + 2,vch,DAM_)
		&&  !IS_IMMORTAL(vch)
		&&  !is_safe_rspell(af->level,vch)
            	&&  !IS_AFFECTED(vch,AFF_) && number_range(0,7) == 0)
            	{
            	    send_to_char( L"You feel hot and feverish.\n\r",vch);
            	    act( L"$n shivers and looks very ill.",vch,NULL,NULL,TO_ROOM);
            	    affect_join(vch,&paf);
            	}
            }
	 break;
        }
*/
    }
    return;
}



void check_reboot( void )
{
 wchar_t buf[MAX_STRING_LENGTH];
 DESCRIPTOR_DATA *d;
 int t;

 if ((t = check_time_sync()) != 0)
 {
  time_sync = 1;
  reboot_counter = UMAX(t,1);
 }

 if (parse_date(current_time) != parse_date(boot_time))
	reboot_counter = 0;

 switch(reboot_counter)
 {
  case -1:
     break;
  case 0:
     reboot_uzakdiyarlar(TRUE);
     return;
  case 1:
  case 2:
  case 3:
  case 4:
  case 5:
  case 10:
  case 15:
  swprintf( buf, MAX_STRING_LENGTH-1, L"\007***** %i DAKİKA SONRA UD YENİDEN BAŞLATILACAK *****\007\n\r",reboot_counter);
    for (d = descriptor_list; d != NULL; d = d->next)
	  write_to_buffer(d,buf,0);
  default:
    reboot_counter--;
    break;
 }
}

void track_update( void )
{
    CHAR_DATA *ch, *ch_next;
    CHAR_DATA *vch,*vch_next;
    wchar_t buf[MAX_STRING_LENGTH];

    for ( ch = char_list; ch != NULL; ch = ch_next )
    {
	ch_next = ch->next;

	if (IS_NPC(ch) && !IS_AFFECTED(ch,AFF_CALM)
	    && !IS_AFFECTED(ch,AFF_CHARM)
            && ch->fighting == NULL
	    && ch->in_room != NULL
            && IS_AWAKE(ch)
            && !IS_SET(ch->act,ACT_NOTRACK)
	    && !RIDDEN(ch)
	    && !IS_AFFECTED(ch,AFF_SCREAM) )
        {
         if ( ch->last_fought != NULL
	      && ch->in_room != ch->last_fought->in_room )
          do_track(ch,ch->last_fought->name);
	 else if (ch->in_mind != NULL)
	 {
    	  for (vch = ch->in_room->people; vch!=NULL; vch = vch_next)
	   {
      	     vch_next = vch->next_in_room;

	     if (ch == vch) continue;
	     if ( !IS_IMMORTAL(vch) && can_see(ch,vch) &&
		!is_safe_nomessage(ch,vch)  && is_name(vch->name,ch->in_mind) )
	     {
         swprintf( buf, MAX_STRING_LENGTH-1, L"İşte yine karşılaştık %s!",vch->name);
	      do_yell(ch,buf);
	      do_murder(ch,vch->name);
	      break; /* one fight at a time */
	     }
	   }
	 }
        }
   }
}

void cevrimici_oyuncu_sayisi( void )
{
	DESCRIPTOR_DATA *d;
	int count;
	count=0;
	for ( d = descriptor_list; d != NULL; d = d->next )
	{
        	if (d->connected == CON_PLAYING )
			count++;
	}
  max_on = UMAX(count,max_on);
	max_on_so_far  = UMAX(count,max_on_so_far);
	return;
}

