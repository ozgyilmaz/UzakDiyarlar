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
#include "merc.h"
#include "recycle.h"

DECLARE_DO_FUN(do_yell		);
DECLARE_DO_FUN(do_sleep		);
DECLARE_DO_FUN(do_sit		);
DECLARE_DO_FUN(do_bash_door	);
DECLARE_DO_FUN(do_throw_spear	);

DECLARE_SPELL_FUN(	spell_poison	);
DECLARE_SPELL_FUN(	spell_blindness	);

void	one_hit		args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ,bool secondary) );
void	set_fighting	args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );

/*
 * Disarm a creature.
 * Caller must check for successful attack.
 */
void disarm( CHAR_DATA *ch, CHAR_DATA *victim ,int disarm_second)
{
    OBJ_DATA *obj;

    if (disarm_second)
	{
         if ((obj=get_wield_char(victim,TRUE)) == NULL)
	  {
	   bug( L"Disarm second with NULL DUAL_WIELD",0);
	   return;
	  }
	}
	else
	{
         if ((obj=get_wield_char(victim,FALSE)) == NULL)
	  {
	   bug( L"Disarm first with NULL WEAR_WIELD",0);
	   return;
          }
	}

    if ( IS_OBJ_STAT(obj,ITEM_NOREMOVE))
    {
      act( L"$S silahı kıpırdamıyor!",ch,NULL,victim,TO_CHAR);
    	act( L"$n seni silahsızlandırmaya çalışıyor, ancak silahın kıpırdamıyor!",
    	    ch,NULL,victim,TO_VICT);
    	act( L"$n $M silahsızlandırmaya çalıştı, ama başaramadı.",ch,NULL,victim,TO_NOTVICT);
	return;
    }

    if ( !IS_NPC(victim) &&
	victim->level > skill_table[gsn_grip].skill_level[victim->iclass])
    {
     int skill = get_skill(victim,gsn_grip);

        skill +=
	 (get_curr_stat(victim,STAT_STR) - get_curr_stat(ch,STAT_STR)) * 5;
	if ( number_percent() < skill )
	{
    act( L"$N kabzayı sıkıyor ve silahsızlandırmanı engelliyor!",ch,NULL,victim,TO_CHAR);
 	 act( L"$n seni silahsızlandırmaya çalıştı, kabzayı sıktığından başaramadı!",
 	    ch,NULL,victim,TO_VICT);
 	 act( L"$n $M silahsızlandırmaya çalıştı, ama başaramadı.",ch,NULL,victim,TO_NOTVICT);
	 check_improve(victim,gsn_grip,TRUE,1);
	 return;
	}
	else 	 check_improve(victim,gsn_grip,FALSE,1);
    }

    act_color( L"$n seni $Csilahsızlandırdı$c!",
	 ch, NULL, victim, TO_VICT ,POS_FIGHTING,CLR_CYAN   );
    act_color( L"$M $Csilahsızlandırdın$c!", ch,NULL, victim, TO_CHAR,
	POS_FIGHTING,CLR_CYAN_BOLD);
    act_color( L"$n $M $Csilahsızlandırdı$c!",ch, NULL, victim,TO_NOTVICT,
	POS_FIGHTING,CLR_CYAN_BOLD);

    obj_from_char( obj );
    if ( IS_OBJ_STAT(obj,ITEM_NODROP) || IS_OBJ_STAT(obj,ITEM_INVENTORY) )
	obj_to_char( obj, victim );
    else
    {
	obj_to_room( obj, victim->in_room );
	if (IS_NPC(victim) && victim->wait == 0 && can_see_obj(victim,obj))
	    get_obj(victim,obj,NULL);
    }
/*
    if ( (obj2 = get_wield_char(victim,TRUE)) != NULL)
	{
act_color( L"$CYou wield your second weapon as your first!.$c", ch, NULL,
	victim,TO_VICT,POS_FIGHTING,CLR_CYAN);
act_color( L"$C$N wields his second weapon as first!$c",  ch, NULL,
	victim,TO_CHAR ,POS_FIGHTING,CLR_CYAN_BOLD);
act_color( L"$C$N wields his second weapon as first!$c",  ch, NULL, victim,
	TO_NOTVICT ,POS_FIGHTING,CLR_CYAN_BOLD);
	unequip_char( victim, obj2);
	equip_char( victim, obj2 , WEAR_WIELD);
	}
*/
    return;
}

void do_berserk( CHAR_DATA *ch, wchar_t *argument)
{
    int chance, hp_percent;

    if ((chance = get_skill(ch,gsn_berserk)) == 0
    ||  (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_BERSERK))
    ||  (!IS_NPC(ch)
    &&   ch->level < skill_table[gsn_berserk].skill_level[ch->iclass]))
    {
      send_to_char( L"Yüzün kızarıyor, ama değişen birşey olmuyor.\n\r",ch);
	return;
    }

    if (IS_AFFECTED(ch,AFF_BERSERK) || is_affected(ch,gsn_berserk)
    ||  is_affected(ch,skill_lookup( L"frenzy")))
    {
      send_to_char( L"Biraz çıldırıyorsun.\n\r",ch);
	return;
    }

    if (IS_AFFECTED(ch,AFF_CALM))
    {
      send_to_char( L"Vahşileşemeyecek kadar sakin hissediyorsun.\n\r",ch);
	return;
    }

    if (ch->mana < 50)
    {
      send_to_char( L"Yeterli enerji toplayamıyorsun.\n\r",ch);
	return;
    }

    /* modifiers */

    /* fighting */
    if (ch->position == POS_FIGHTING)
	chance += 10;

    /* damage -- below 50% of hp helps, above hurts */
    hp_percent = 100 * ch->hit/ch->max_hit;
    chance += 25 - hp_percent/2;

    if (number_percent() < chance)
    {
	AFFECT_DATA af;

	WAIT_STATE(ch,PULSE_VIOLENCE);
	ch->mana -= 50;
	ch->move /= 2;

	/* heal a little damage */
	ch->hit += ch->level * 2;
	ch->hit = UMIN(ch->hit,ch->max_hit);

  send_to_char( L"Nabzın hızlanıyor ve sen öfkeyle dolup taşıyorsun!\n\r",ch);
	act_color( L"$C$n vahşi bir bakış takındı.$c",
		ch,NULL,NULL,TO_ROOM,POS_FIGHTING,CLR_RED);
	check_improve(ch,gsn_berserk,TRUE,2);

	af.where	= TO_AFFECTS;
	af.type		= gsn_berserk;
	af.level	= ch->level;
	af.duration	= number_fuzzy(ch->level / 8);
	af.modifier	= UMAX(1,ch->level/5);
	af.bitvector 	= AFF_BERSERK;

	af.location	= APPLY_HITROLL;
	affect_to_char(ch,&af);

	af.location	= APPLY_DAMROLL;
	affect_to_char(ch,&af);

	af.modifier	= UMAX(10,10 * (ch->level/5));
	af.location	= APPLY_AC;
	affect_to_char(ch,&af);
    }

    else
    {
	WAIT_STATE(ch,2 * PULSE_VIOLENCE);
	ch->mana -= 25;
	ch->move /= 2;

  send_to_char( L"Nabzın hızlanıyor, ama birşey olmuyor.\n\r",ch);
	check_improve(ch,gsn_berserk,FALSE,2);
    }
}

void do_bash( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int chance, wait;
    wchar_t buf[MAX_STRING_LENGTH];
    bool FightingCheck;
    int damage_bash;

    if ( MOUNTED(ch) )
    {
      send_to_char( L"Biniciyken olmaz!\n\r", ch);
        return;
    }

    if (ch->fighting != NULL)
	FightingCheck = TRUE;
    else
	FightingCheck = FALSE;

    argument = one_argument(argument,arg);

    if ( (chance = get_skill(ch,gsn_bash)) == 0
    ||	 (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_BASH))
    ||	 (!IS_NPC(ch)
    &&	  ch->level < skill_table[gsn_bash].skill_level[ch->iclass]))
    {
      send_to_char( L"Omuz? O da ne?\n\r",ch);
	return;
    }

    if (arg[0] != '\0' && !wcscasecmp(arg, L"door"))
	{ do_bash_door(ch,argument); return; }

    if (arg[0] == '\0')
    {
	victim = ch->fighting;
	if (victim == NULL)
	{
    send_to_char( L"Fakat kimseyle dövüşmüyorsun!\n\r",ch);
	    return;
	}
    }

    else if ((victim = get_char_room(ch,arg)) == NULL)
    {
	send_to_char( L"O burada değil.\n\r",ch);
	return;
    }

    if (victim->position < POS_FIGHTING)
    {
      act( L"Önce ayağa kalkmasına izin vermelisin.",ch,NULL,victim,TO_CHAR);
	return;
    }

    if (victim == ch)
    {
      send_to_char( L"Kendini omuzlamaya çalıştın, başaramadın.\n\r",ch);
	return;
    }

    if ( MOUNTED(victim) )
    {
      send_to_char( L"Bir biniciyi omuzlayamazsın!\n\r", ch);
        return;
    }

    if (is_safe(ch,victim))
	return;

    if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)
    {
      act( L"Fakat $N senin arkadaşın!",ch,NULL,victim,TO_CHAR);
	return;
    }

    if (is_affected(victim, gsn_protective_shield))
     {
       act_color( L"$CAttığın omuz $S etrafından kayıyor.$c", ch, NULL, victim,
       	TO_CHAR,POS_FIGHTING,CLR_YELLOW);
       act_color( L"$C$s attığı omuz koruyucu kalkanının etrafından kayıyor.$c", ch, NULL,
       	victim,	TO_VICT,POS_FIGHTING,CLR_YELLOW);
       act_color( L"$C$s attığı omuz $S etrafından kayıyor.$c",ch,NULL,victim,
       	TO_NOTVICT,POS_FIGHTING,CLR_YELLOW);
	return;
     }

    /* modifiers */

    /* size  and weight */
    chance += ch->carry_weight / 25;
    chance -= victim->carry_weight / 20;

    if (ch->size < victim->size)
	chance += (ch->size - victim->size) * 25;
    else
	chance += (ch->size - victim->size) * 10;


    /* stats */
    chance += get_curr_stat(ch,STAT_STR);
    chance -= get_curr_stat(victim,STAT_DEX) * 4/3;

    if (IS_AFFECTED(ch,AFF_FLYING))
	chance -= 10;

    /* speed */
    if (IS_SET(ch->off_flags,OFF_FAST))
	chance += 10;
    if (IS_SET(victim->off_flags,OFF_FAST))
	chance -= 20;

    /* level */
    chance += (ch->level - victim->level) * 2;

    /* now the attack */
    if (number_percent() < chance)
    {

      act( L"$n güçlü bir omuzla ayaklarını yerden kesiyor!",
    		ch,NULL,victim,TO_VICT);
    	act( L"Güçlü bir omuzla $S ayaklarını yerden kesiyorsun!",ch,NULL,victim,TO_CHAR);
    	act( L"$n attığı güçlü omuzla $S ayaklarını yerden kesiyor.",
    		ch,NULL,victim,TO_NOTVICT);
	check_improve(ch,gsn_bash,TRUE,1);

	wait = 3;

	switch(number_range(0,1)) {
	case 0: wait = 1; break;
	case 1: wait = 2; break;
	case 2: wait = 4; break;
	case 3: wait = 3; break;
	}

	WAIT_STATE(victim, wait * PULSE_VIOLENCE);
	WAIT_STATE(ch,skill_table[gsn_bash].beats);
	victim->position = POS_RESTING;
	damage_bash = (ch->damroll / 2) + number_range(4,4 + 4* ch->size + chance/10);
	damage(ch,victim,damage_bash,gsn_bash, DAM_BASH, TRUE);

    }
    else
    {
	damage(ch,victim,0,gsn_bash,DAM_BASH, TRUE);
  act( L"Yüzüstü yere düştün!",
	    ch,NULL,victim,TO_CHAR);
	act( L"$n yüzüstü yere düştü.",
	    ch,NULL,victim,TO_NOTVICT);
	act( L"$s attığı omuzdan kurtuluyor ve yüzüstü yere düşmesini sağlıyorsun.",
	    ch,NULL,victim,TO_VICT);
	check_improve(ch,gsn_bash,FALSE,1);
	ch->position = POS_RESTING;
	WAIT_STATE(ch,skill_table[gsn_bash].beats * 3/2);
    }
    if (!(IS_NPC(victim)) && !(IS_NPC(ch)) && victim->position > POS_STUNNED
		&& !FightingCheck)
      {
	if (!can_see(victim, ch))
	  do_yell(victim, (wchar_t*)"İmdat! Biri bana omuz atıyor!");
	else
	  {
      swprintf( buf, MAX_STRING_LENGTH-1, L"İmdat! %s bana omuz atıyor!",
		(is_affected(ch,gsn_doppelganger) && !IS_IMMORTAL(victim)) ?
		ch->doppel->name : ch->name);
	    do_yell(victim, buf);
	  }
      }
}

void do_dirt( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int chance;
    wchar_t buf[MAX_STRING_LENGTH];
    bool FightingCheck;

    if ( MOUNTED(ch) )
    {
      send_to_char( L"Biniciyken toz atamazsın!\n\r", ch);
        return;
    }

    if (ch->fighting != NULL)
	FightingCheck = TRUE;
    else
	FightingCheck = FALSE;

    one_argument(argument,arg);

    if ( (chance = get_skill(ch,gsn_dirt)) == 0
    ||   (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_KICK_DIRT))
    ||   (!IS_NPC(ch)
    &&    ch->level < skill_table[gsn_dirt].skill_level[ch->iclass]))
    {
      send_to_char( L"Ayakların kirlenir.\n\r",ch);
	return;
    }

    if (arg[0] == '\0')
    {
	victim = ch->fighting;
	if (victim == NULL)
	{
    send_to_char( L"Fakat kimseyle dövüşmüyorsun!\n\r",ch);
	    return;
	}
    }

    else if ((victim = get_char_room(ch,arg)) == NULL)
    {
	send_to_char( L"O burada değil.\n\r",ch);
	return;
    }

    if (IS_AFFECTED(ch,AFF_FLYING) )
	{
    send_to_char( L"Uçarken mi?.\n\r",ch);
	 return;
	}

    if (IS_AFFECTED(victim,AFF_BLIND))
    {
      act( L"$N zaten kör.",ch,NULL,victim,TO_CHAR);
	return;
    }

    if (victim == ch)
    {
      send_to_char( L"Çok komiksin.\n\r",ch);
	return;
    }

    if ( MOUNTED(victim) )
    {
      send_to_char( L"Bir biniciye toz atamazsın!\n\r", ch);
        return;
    }

    if (is_safe(ch,victim))
	return;

    if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)
    {
      act( L"Fakat $N senin arkadaşın!",ch,NULL,victim,TO_CHAR);
	return;
    }

    /* modifiers */

    /* dexterity */
    chance += get_curr_stat(ch,STAT_DEX);
    chance -= 2 * get_curr_stat(victim,STAT_DEX);

    /* speed  */
    if (IS_SET(ch->off_flags,OFF_FAST) || IS_AFFECTED(ch,AFF_HASTE))
	chance += 10;
    if (IS_SET(victim->off_flags,OFF_FAST) || IS_AFFECTED(victim,OFF_FAST))
	chance -= 25;

    /* level */
    chance += (ch->level - victim->level) * 2;

    if (chance % 5 == 0)
	chance += 1;

    /* terrain */

    switch(ch->in_room->sector_type)
    {
	case(SECT_INSIDE):		chance -= 20;	break;
	case(SECT_CITY):		chance -= 10;	break;
	case(SECT_FIELD):		chance +=  5;	break;
	case(SECT_FOREST):				break;
	case(SECT_HILLS):				break;
	case(SECT_MOUNTAIN):		chance -= 10;	break;
	case(SECT_WATER_SWIM):		chance  =  0;	break;
	case(SECT_WATER_NOSWIM):	chance  =  0;	break;
	case(SECT_AIR):			chance  =  0;  	break;
	case(SECT_DESERT):		chance += 10;   break;
    }

    if (chance == 0)
    {
      send_to_char( L"Hiç toz bulamıyorsun.\n\r",ch);
	return;
    }

    /* now the attack */
    if (number_percent() < chance)
    {
	AFFECT_DATA af;
  act( L"$n gözlerine atılan tozla kör oldu!",victim,NULL,NULL,TO_ROOM);
        damage(ch,victim,number_range(2,5),gsn_dirt,DAM_NONE, TRUE);
	send_to_char( L"Hiçbir şey göremiyorsun!\n\r",victim);
	check_improve(ch,gsn_dirt,TRUE,2);
	WAIT_STATE(ch,skill_table[gsn_dirt].beats);

	af.where	= TO_AFFECTS;
	af.type 	= gsn_dirt;
	af.level 	= ch->level;
	af.duration	= 0;
	af.location	= APPLY_HITROLL;
	af.modifier	= -4;
	af.bitvector 	= AFF_BLIND;

	affect_to_char(victim,&af);
    }
    else
    {
	damage(ch,victim,0,gsn_dirt,DAM_NONE, TRUE);
	check_improve(ch,gsn_dirt,FALSE,2);
	WAIT_STATE(ch,skill_table[gsn_dirt].beats);
    }
    if (!(IS_NPC(victim)) && !(IS_NPC(ch)) && victim->position > POS_STUNNED
		&& !FightingCheck)
      {
    	if (!can_see(victim,ch))
	  do_yell(victim, (wchar_t*)"Biri gözlerime toz attı!");
	else
	  {
      swprintf( buf, MAX_STRING_LENGTH-1, L"Geber %s!  Seni hilekar!", (is_affected(ch,
		gsn_doppelganger) && !IS_IMMORTAL(victim)) ? ch->doppel->name
		: ch->name);
	    do_yell(victim, buf);
	  }
      }

}

void do_trip( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int chance;
    wchar_t buf[MAX_STRING_LENGTH];
    bool FightingCheck;

    if ( MOUNTED(ch) )
    {
      send_to_char( L"Biniciyken çelme takamazsın!\n\r", ch);
        return;
    }

    if (ch->fighting != NULL)
	FightingCheck = TRUE;
    else
	FightingCheck = FALSE;

    one_argument(argument,arg);

    if ( (chance = get_skill(ch,gsn_trip)) == 0
    ||   (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_TRIP) &&
	ch->master != NULL)
    ||   (!IS_NPC(ch)
	  && ch->level < skill_table[gsn_trip].skill_level[ch->iclass]))
    {
      send_to_char( L"Çelme?  O da ne?\n\r",ch);
	return;
    }


    if (arg[0] == '\0')
    {
	victim = ch->fighting;
	if (victim == NULL)
	{
	    send_to_char( L"Fakat kimseyle dövüşmüyorsun!\n\r",ch);
	    return;
 	}
    }

    else if ((victim = get_char_room(ch,arg)) == NULL)
    {
	send_to_char( L"O burada değil.\n\r",ch);
	return;
    }

    if ( MOUNTED(victim) )
    {
      send_to_char( L"Bir biniciye çelme takamazsın!\n\r", ch);
        return;
    }

    if (is_safe(ch,victim))
	return;

    if (IS_AFFECTED(victim,AFF_FLYING))
    {
      act( L"$S ayakları yerde değil.",ch,NULL,victim,TO_CHAR);
	return;
    }

    if (victim->position < POS_FIGHTING)
    {
      act( L"$N zaten yere düşmüş.",ch,NULL,victim,TO_CHAR);
	return;
    }

    if (victim == ch)
    {
      send_to_char( L"Yüzüstü yere düşüyorsun!\n\r",ch);
    	WAIT_STATE(ch,2 * skill_table[gsn_trip].beats);
    	act( L"$n kendi ayağına çelme takıyor!",ch,NULL,NULL,TO_ROOM);
	return;
    }

    if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)
    {
      act( L"$N senin pek sevgili efendin.",ch,NULL,victim,TO_CHAR);
	return;
    }

    /* modifiers */

    /* size */
    if (ch->size < victim->size)
        chance += (ch->size - victim->size) * 10;  /* bigger = harder to trip */

    /* dex */
    chance += get_curr_stat(ch,STAT_DEX);
    chance -= get_curr_stat(victim,STAT_DEX) * 3 / 2;

    if (IS_AFFECTED(ch,AFF_FLYING) )
	chance -= 10;

    /* speed */
    if (IS_SET(ch->off_flags,OFF_FAST) || IS_AFFECTED(ch,AFF_HASTE))
	chance += 10;
    if (IS_SET(victim->off_flags,OFF_FAST) || IS_AFFECTED(victim,AFF_HASTE))
	chance -= 20;

    /* level */
    chance += (ch->level - victim->level) * 2;


    /* now the attack */
    if (number_percent() < chance)
    {
      act( L"$s taktığı çelme ile yere düşüyorsun!",ch,NULL,victim,TO_VICT);
    	act( L"Taktığın çelme ile $M düşürüyorsun!",ch,NULL,victim,TO_CHAR);
    	act( L"$n taktığı çelme ile $M düşürüyor.",ch,NULL,victim,TO_NOTVICT);
	check_improve(ch,gsn_trip,TRUE,1);

	WAIT_STATE(victim,2 * PULSE_VIOLENCE);
        WAIT_STATE(ch,skill_table[gsn_trip].beats);
	victim->position = POS_RESTING;
	damage(ch,victim,number_range(2, 2 +  2 * victim->size),gsn_trip,
	    DAM_BASH, TRUE);
    }
    else
    {
	damage(ch,victim,0,gsn_trip,DAM_BASH, TRUE);
	WAIT_STATE(ch,skill_table[gsn_trip].beats*2/3);
	check_improve(ch,gsn_trip,FALSE,1);
    }
    if (!(IS_NPC(victim)) && !(IS_NPC(ch)) && victim->position > POS_STUNNED
		&& !FightingCheck)
      {
	if (!can_see(victim, ch))
	  do_yell(victim, (wchar_t*)"İmdat! Biri bana çelme taktı!");
	else
	  {
      swprintf( buf, MAX_STRING_LENGTH-1, L"İmdat! %s bana çelme taktı!",
		(is_affected(ch,gsn_doppelganger) && !IS_IMMORTAL(victim)) ?
		ch->doppel->name : ch->name);
	    do_yell(victim, buf);
	  }
      }
}



void do_backstab( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    wchar_t buf[MAX_STRING_LENGTH];

    one_argument( argument, arg );

    if ( MOUNTED(ch) )
    {
      send_to_char( L"Biniciyken ardılan deneyemezsin!\n\r", ch);
        return;
    }

    if ( !IS_NPC(ch)
    &&   ch->level < skill_table[gsn_backstab].skill_level[ch->iclass] )
      {
        send_to_char( L"Ardılan yapmayı bilmiyorsun.\n\r",ch);
	return;
      }

    if ( arg[0] == '\0' )
    {
      send_to_char( L"Kime ardılan yapacaksın?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
      send_to_char( L"Burada değil.\n\r", ch);
	return;
    }
    if ( (IS_NPC(ch)) && (!(IS_NPC(victim))) )
	return;


    if ( victim == ch )
    {
      send_to_char( L"Kendine süzülmeyi becerebilir misin?\n\r", ch );
	return;
    }

    if ( is_safe( ch, victim ) )
      return;

    if ( ( obj = get_wield_char( ch,FALSE ) ) == NULL
	|| attack_table[obj->value[3]].damage != DAM_PIERCE)
    {
      send_to_char( L"Ardılan için delici bir silah kullanmalısın.\n\r",ch);
	return;
    }

    if ( victim->fighting != NULL )
    {
      send_to_char( L"Dövüşen birine ardılan atamazsın.\n\r", ch );
	return;
    }


    WAIT_STATE( ch, skill_table[gsn_backstab].beats );

    if ( victim->hit < (0.7 * victim->max_hit) &&
	 (IS_AWAKE(victim) ) )
    {
      act( L"$N yaralı ve kuşkulu ... gizlice yaklaşamazsın.",
	    ch, NULL, victim, TO_CHAR );
	return;
    }

    if ( current_time-victim->last_fight_time<300 && IS_AWAKE(victim) )
    {
      act( L"$N yaralı ve kuşkulu ... gizlice yaklaşamazsın.",
	    ch, NULL, victim, TO_CHAR );
	return;
    }

    if ( !IS_AWAKE(victim)
    ||   IS_NPC(ch)
    ||   number_percent( ) < get_skill(ch,gsn_backstab) )
    {
	check_improve(ch,gsn_backstab,TRUE,1);
	if (!IS_NPC(ch) &&
	    number_percent( ) < (get_skill(ch,gsn_dual_backstab)/10)*8)
	  {
	    check_improve(ch,gsn_dual_backstab,TRUE,1);
	    one_hit(ch,victim,gsn_backstab,FALSE);
	    one_hit(ch,victim,gsn_dual_backstab,FALSE);
	  }
	else
	  {
	    check_improve(ch,gsn_dual_backstab,FALSE,1);
	    multi_hit( ch, victim, gsn_backstab );
	  }
    }
    else
    {
	check_improve(ch,gsn_backstab,FALSE,1);
	damage( ch, victim, 0, gsn_backstab,DAM_NONE, TRUE );
    }
    /* Player shouts if he doesn't die */
    if (!(IS_NPC(victim)) && !(IS_NPC(ch))
	&& victim->position == POS_FIGHTING )
      {
	if (!can_see(victim, ch))
	  do_yell(victim, (wchar_t*)"İmdat! Biri beni ardılanla vurdu!");
	else
	  {
      swprintf( buf, MAX_STRING_LENGTH-1, L"Geber %s, seni kahrolası madrabaz!",
		(is_affected(ch,gsn_doppelganger) && !IS_IMMORTAL(victim)) ?
		ch->doppel->name : ch->name );
	    do_yell( victim, buf );
          }
      }
    return;
}

void do_cleave( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    wchar_t buf[MAX_STRING_LENGTH];

    if ( MOUNTED(ch) )
    {
      send_to_char( L"Biniciyken yarma deneyemezsin!\n\r", ch);
        return;
    }

    one_argument( argument, arg );

	if (ch->master != NULL && IS_NPC(ch))
	return;

    if ( !IS_NPC(ch)
    &&   ch->level < skill_table[gsn_cleave].skill_level[ch->iclass] )
      {
        send_to_char( L"Yarmayı bilmiyorsun.\n\r",ch);
	return;
      }

    if ( arg[0] == '\0' )
    {
      send_to_char( L"Kimi yaracaksın?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( L"O burada değil.\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
      send_to_char( L"Kendine süzülmeyi becerebilir misin?\n\r", ch );
	return;
    }

    if ( is_safe( ch, victim ) )
      return;

    if ( ( obj = get_wield_char( ch,FALSE ) ) == NULL)
    {
      send_to_char( L"Yarma için bir silah kuşanmalısın.\n\r", ch );
	return;
    }


    if ( victim->fighting != NULL )
    {
      send_to_char( L"Dövüşen birine yarma atamazsın.\n\r", ch );
	return;
    }

    if ( (victim->hit < (0.9 * victim->max_hit)) &&
	 (IS_AWAKE(victim)) )
    {
      act( L"$N yaralı ve kuşkulu ... gizlice yaklaşamazsın.",
	    ch, NULL, victim, TO_CHAR );
	return;
    }

    WAIT_STATE( ch, skill_table[gsn_cleave].beats );
    if ( !IS_AWAKE(victim)
    ||   IS_NPC(ch)
    ||   number_percent( ) < get_skill(ch,gsn_cleave) )
      {
	check_improve(ch,gsn_cleave,TRUE,1);
	multi_hit(ch,victim,gsn_cleave);
      }
    else
      {
	check_improve(ch,gsn_cleave,FALSE,1);
	damage( ch, victim, 0, gsn_cleave,DAM_NONE, TRUE );
      }
    /* Player shouts if he doesn't die */
    if (!(IS_NPC(victim)) && !(IS_NPC(ch)) && victim->position == POS_FIGHTING)
      {
	if (!can_see(victim, ch))
	  do_yell(victim, (wchar_t*)"İmdat! Biri bana saldırıyor!");
	else
	  {
	    swprintf( buf, MAX_STRING_LENGTH-1, L"Geber %s, seni kasap kılıklı aptal!",
		(is_affected(ch,gsn_doppelganger) && !IS_IMMORTAL(victim)) ?
		ch->doppel->name : ch->name );
	    do_yell( victim, buf );
	  }
      }
    return;
}

void do_ambush( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    wchar_t buf[MAX_STRING_LENGTH];

    if ( MOUNTED(ch) )
    {
      send_to_char( L"Biniciyken pusu atamazsın!\n\r", ch);
        return;
    }

    one_argument( argument, arg );

    if ( IS_NPC(ch) ||
         ch->level < skill_table[gsn_ambush].skill_level[ch->iclass] )
      {
        send_to_char( L"Pusu atmayı bilmiyorsun.\n\r",ch);
	return;
      }

    if ( arg[0] == '\0' )
    {
      send_to_char( L"Kime pusu atacaksın?\n\r", ch );
	return;
    }


    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( L"O burada değil.\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
      send_to_char( L"Kendini pusuya düşürebilir misin?\n\r", ch );
	return;
    }


    if (!IS_AFFECTED(ch,AFF_CAMOUFLAGE) || can_see(victim,ch))
      {
        send_to_char( L"Fakat seni görebiliyorlar.\n\r",ch);
	return;
      }

    if ( is_safe( ch, victim ) )
      return;

    WAIT_STATE( ch, skill_table[gsn_ambush].beats );
    if ( !IS_AWAKE(victim)
    ||   IS_NPC(ch)
    ||   number_percent( ) < get_skill(ch,gsn_ambush) )
      {
	check_improve(ch,gsn_ambush,TRUE,1);
	multi_hit(ch,victim,gsn_ambush);
      }
    else
      {
	check_improve(ch,gsn_ambush,FALSE,1);
	damage( ch, victim, 0, gsn_ambush,DAM_NONE, TRUE );
      }

    /* Player shouts if he doesn't die */
    if (!(IS_NPC(victim)) && !(IS_NPC(ch))
	&& victim->position == POS_FIGHTING )
      {
	if (!can_see(victim, ch))
	  do_yell(victim, (wchar_t*)"İmdat! Pusuya düştüm!");
	else
	  {
      swprintf( buf, MAX_STRING_LENGTH-1, L"İmdat! %s tarafından pusuya düşürüldüm!",
		    (is_affected(ch,gsn_doppelganger)&& !IS_IMMORTAL(victim)) ?
		    ch->doppel->name : ch->name );
	    do_yell( victim, buf );
	  }
      }
    return;
}



void do_rescue( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *fch;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
      send_to_char( L"Kimi kurtaracaksın?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( L"O burada değil.\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
      send_to_char( L"Kaçmaya ne dersin?\n\r", ch );
	return;
    }

    if ( !IS_NPC(ch) && IS_NPC(victim) )
    {
      send_to_char( L"Yardımına ihtiyacı yok!\n\r", ch );
	return;
    }

    if ( ch->fighting == victim )
    {
      send_to_char( L"Artık çok geç.\n\r", ch );
	return;
    }

    if ( ( fch = victim->fighting ) == NULL )
    {
      send_to_char( L"O kişi şu an dövüşmüyor.\n\r", ch );
	return;
    }
    if (IS_NPC(ch) && ch->master != NULL && IS_NPC(victim))
      return;

    if (is_safe(ch, fch))
	return;

    if (ch->master != NULL)
      {
	if (is_safe(ch->master, fch)) return;
      }

    WAIT_STATE( ch, skill_table[gsn_rescue].beats );
    if ( ( !IS_NPC(ch)
	 && number_percent( ) > get_skill(ch,gsn_rescue) )
	 || ( victim->level > (ch->level + 30) ) )
    {
      send_to_char( L"Kurtarmayı başaramadın.\n\r", ch );
	check_improve(ch,gsn_rescue,FALSE,1);
	return;
    }

    act( L"$M kurtarıyorsun!",  ch, NULL, victim, TO_CHAR    );
    act( L"$n seni kurtarıyor!", ch, NULL, victim, TO_VICT    );
    act( L"$n $M kurtarıyor!",  ch, NULL, victim, TO_NOTVICT );
    check_improve(ch,gsn_rescue,TRUE,1);

    stop_fighting( fch, FALSE );
    stop_fighting( victim, FALSE );

    set_fighting( ch, fch );
    set_fighting( fch, ch );
    return;
}



void do_kick( CHAR_DATA *ch, wchar_t *argument )
{
    CHAR_DATA *victim;
    int kick_dam;
    int chance;

    if ( MOUNTED(ch) )
    {
      send_to_char( L"Biniciyken tekme atamazsın!\n\r", ch);
        return;
    }

    if ( !IS_NPC(ch)
    &&   ch->level < skill_table[gsn_kick].skill_level[ch->iclass] )
    {
	send_to_char(L"Savaş sanatlarını savaşçılara bırakmaya ne dersin?\n\r", ch );
	return;
    }

    if (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_KICK))
	return;

    if ( ( victim = ch->fighting ) == NULL )
    {
	send_to_char( L"Kimseyle dövüşmüyorsun.\n\r", ch );
	return;
    }
    chance = number_percent( );
    if ( IS_AFFECTED(ch,AFF_FLYING) ) chance = (int)((float)chance*1.1);
    WAIT_STATE( ch, skill_table[gsn_kick].beats );
    if ( IS_NPC(ch) || chance < get_skill(ch,gsn_kick) )
    {
	kick_dam = number_range(1,ch->level);
	if ((ch->iclass == 9) && ( get_eq_char(ch,WEAR_FEET) == NULL) )
		kick_dam *= 2;
	kick_dam += ch->damroll / 2;
	damage( ch,victim,kick_dam,gsn_kick,DAM_BASH, TRUE );
	check_improve(ch,gsn_kick,TRUE,1);
    }
    else
    {
	damage( ch, victim, 0, gsn_kick,DAM_BASH, TRUE );
	check_improve(ch,gsn_kick,FALSE,1);
    }

    return;
}

void do_circle( CHAR_DATA *ch, wchar_t *argument )
{
    CHAR_DATA *victim;
    CHAR_DATA *person;
    bool second;

    if ( MOUNTED(ch) )
    {
      send_to_char( L"Biniciyken çevrel vuruş atamazsın!\n\r", ch);
        return;
    }

    if ( IS_NPC(ch)
    ||   ch->level < skill_table[gsn_circle].skill_level[ch->iclass] )
    {
      send_to_char( L"Çevrel vuruş atmayı bilmiyorsun.\n\r", ch );
	return;
    }

    if ( ( victim = ch->fighting ) == NULL )
    {
	send_to_char( L"Kimseyle dövüşmüyorsun.\n\r", ch );
	return;
    }

    second = FALSE;
    if ((get_wield_char(ch,FALSE) == NULL) ||
	attack_table[get_wield_char(ch,FALSE)->value[3]].damage != DAM_PIERCE)
    {
     if ((get_wield_char(ch,TRUE) == NULL) ||
	attack_table[get_wield_char(ch,TRUE)->value[3]].damage != DAM_PIERCE)
     {
       send_to_char( L"Çevrel vuruş için delici bir silah kuşanmalısın.\n\r",ch);
       return;
     }
     second = TRUE;
    }

    if (is_safe(ch,victim)) return;

    WAIT_STATE( ch, skill_table[gsn_circle].beats );

    for (person = ch->in_room->people;person != NULL;
	 person = person->next_in_room)
      {
	if (person->fighting == ch)
	  {
      send_to_char( L"Kendini savunurken çevrel vuruş atamazsın.\n\r",ch);
	    return;
	  }
      }

    if ( IS_NPC(ch) || number_percent( ) < get_skill(ch,gsn_circle) )
    {
	one_hit(ch, victim, gsn_circle, second);
	check_improve(ch,gsn_circle,TRUE,1);
    }
    else
    {
	damage( ch, victim, 0, gsn_circle,TYPE_UNDEFINED, TRUE);
	check_improve(ch,gsn_circle,FALSE,1);
    }

    return;
}




void do_disarm( CHAR_DATA *ch, wchar_t *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    int chance,hth,ch_weapon,vict_weapon,ch_vict_weapon,disarm_second=0;
    wchar_t arg[MAX_INPUT_LENGTH];
/*
    if ( MOUNTED(ch) )
    {
        send_to_char( L"You can't disarm while riding!\n\r", ch);
        return;
    }
*/
    hth = 0;

	if (ch->master != NULL && IS_NPC(ch))
	return;

    if ((chance = get_skill(ch,gsn_disarm)) == 0)
    {
      send_to_char( L"Silahsızlandırmayı bilmiyorsun.\n\r", ch );
	return;
    }

    if ( get_wield_char( ch,FALSE ) == NULL
    &&   ((hth = get_skill(ch,gsn_hand_to_hand)) == 0
    ||    (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_DISARM))))
    {
      send_to_char( L"Silahsızlandırabilmek için bir silah giymelisin.\n\r", ch );
	return;
    }

    if ( ( victim = ch->fighting ) == NULL )
    {
	send_to_char( L"Kimseyle dövüşmüyorsun.\n\r", ch );
	return;
    }

    argument = one_argument(argument,arg);
    if (!IS_NPC(ch) && arg[0] != '\0')
    {
    	 if (is_name(arg,(wchar_t*)"second") ) disarm_second = 1;
	 else disarm_second = 0;
    }

    if ( ( obj = get_wield_char( victim, disarm_second ) ) == NULL )
    {
      send_to_char( L"Rakibin silah kullanmıyor.\n\r", ch );
	return;
    }


    /* find weapon skills */
    ch_weapon = get_weapon_skill(ch,get_weapon_sn(ch,FALSE));

    vict_weapon = get_weapon_skill(victim,get_weapon_sn(victim,disarm_second));
    ch_vict_weapon = get_weapon_skill(ch,get_weapon_sn(victim,disarm_second));

    /* modifiers */

    /* skill */
    if ( get_wield_char(ch,FALSE) == NULL)
	chance = chance * hth/150;
    else
	chance = chance * ch_weapon/100;

    chance += (ch_vict_weapon/2 - vict_weapon) / 2;

    /* dex vs. strength */
    chance += get_curr_stat(ch,STAT_DEX);
    chance -= 2 * get_curr_stat(victim,STAT_STR);

    /* level */
    chance += (ch->level - victim->level) * 2;

    /* and now the attack */
    if (number_percent() < chance)
    {
    	WAIT_STATE( ch, skill_table[gsn_disarm].beats );
	disarm( ch, victim ,disarm_second);
	check_improve(ch,gsn_disarm,TRUE,1);
    }
    else
    {
	WAIT_STATE(ch,skill_table[gsn_disarm].beats);
  act( L"$M silahsızlandırmayı başaramadın.",ch,NULL,victim,TO_CHAR);
	act( L"$n seni silahsızlandırmayı başaramadı.",ch,NULL,victim,TO_VICT);
	act( L"$n $M silahsızlandırmayı başaramadı.",ch,NULL,victim,TO_NOTVICT);
	check_improve(ch,gsn_disarm,FALSE,1);
    }
    return;
}



void do_nerve(CHAR_DATA *ch, wchar_t *argument)
{
  CHAR_DATA *victim;
  wchar_t arg[MAX_INPUT_LENGTH];
  wchar_t buf[MAX_STRING_LENGTH];

    if ( MOUNTED(ch) )
    {
      send_to_char( L"Biniciyken olmaz!\n\r", ch);
        return;
    }

  one_argument(argument,arg);

  if (ch->level < skill_table[gsn_nerve].skill_level[ch->iclass] )
    {
      send_to_char( L"Hı?\n\r", ch );
      return;
    }
  if (ch->fighting == NULL)
    {
      send_to_char( L"Kimseyle dövüşmüyorsun.\n\r",ch);
      return;
    }

  victim = ch->fighting;

  if (is_safe(ch,victim)) return;

  if (is_affected(ch,gsn_nerve))
    {
      send_to_char( L"Onu daha fazla zayıflatamazsın.\n\r",ch);
      return;
    }
  WAIT_STATE( ch, skill_table[gsn_nerve].beats );

  if (IS_NPC(ch) ||
      number_percent() < (get_skill(ch,gsn_nerve) + ch->level
                         + get_curr_stat(ch,STAT_DEX))/2)
    {
      AFFECT_DATA af;
      af.where  = TO_AFFECTS;
      af.type 	= gsn_nerve;
      af.level 	= ch->level;
      af.duration = ch->level * PULSE_VIOLENCE/PULSE_TICK;
      af.location = APPLY_STR;
      af.modifier = -3;
      af.bitvector = 0;

      affect_to_char(victim,&af);
      act( L"$M sinir baskısıyla zayıflatıyorsun.",ch,NULL,victim,TO_CHAR);
      act( L"$n seni sinir baskısıyla zayıflatıyor.",ch,NULL,victim,TO_VICT);
      act( L"$n $M sinir baskısıyla zayıflatıyor.",ch,NULL,victim,TO_NOTVICT);
      check_improve(ch,gsn_nerve,TRUE,1);
    }
  else
    {
      send_to_char( L"Yanlış noktaya baskı yaptın.\n\r",ch);
      act( L"$n seni zayıflatmayı denedi ama başaramadı.",
	  ch,NULL,victim,TO_VICT);
      act( L"$n $M zayıflatmaya çalıştı ama başaramadı.",
	  ch,NULL,victim,TO_NOTVICT);
      check_improve(ch,gsn_nerve,FALSE,1);
    }

    multi_hit(victim,ch,TYPE_UNDEFINED);

    if (!(IS_NPC(victim)) && !(IS_NPC(ch))
	&& victim->position != POS_FIGHTING )
      {
	if (!can_see(victim, ch))
	  do_yell(victim, (wchar_t*)"İmdat! Biri bana saldırıyor!");
	else
	{
    swprintf( buf, MAX_STRING_LENGTH-1, L"İmdat! %s bana saldırdı!",
		  (is_affected(ch,gsn_doppelganger) && !IS_IMMORTAL(victim)) ?
		  ch->doppel->name : ch->name );
	  do_yell( victim, buf );
	}
      }
  return;
}

void do_endure(CHAR_DATA *ch, wchar_t *arg)
{
  AFFECT_DATA af;

  if (IS_NPC(ch))
    {
      send_to_char( L"Hiç dayanımın yok.\n\r",ch);
      return;
    }

  if ( ch->level < skill_table[gsn_endure].skill_level[ch->iclass] ||
       ch->pcdata->learned[gsn_endure] <= 1 )
    {
      send_to_char( L"Konsantre olamadın.\n\r",ch);
      return;
    }

  if (is_affected(ch,gsn_endure))
    {
      send_to_char( L"Daha fazla bir dayanım mı?.\n\r",ch);
      return;
    }


  WAIT_STATE( ch, skill_table[gsn_endure].beats );

  af.where 	= TO_AFFECTS;
  af.type 	= gsn_endure;
  af.level 	= ch->level;
  af.duration = ch->level / 4;
  af.location = APPLY_SAVING_SPELL;
  af.modifier = -1 * (get_skill(ch,gsn_endure) / 10);
  af.bitvector = 0;

  affect_to_char(ch,&af);

  send_to_char( L"Kendini büyü karşılaşmalarına hazırlıyorsun.\n\r",ch);
  act( L"$n bir an için konsantre oluyor...",
      ch,NULL,NULL,TO_ROOM);
  check_improve(ch,gsn_endure,TRUE,1);
}

void do_tame(CHAR_DATA *ch, wchar_t *argument)
{
  CHAR_DATA *victim;
  wchar_t arg[MAX_INPUT_LENGTH];

  one_argument(argument,arg);

  if (arg[0] == '\0')
    {
      send_to_char( L"Sen uysallaştırılamazsın.\n\r",ch);
      act( L"$n kendini uysallaştırmayı denedi! Ne kadar garip biri...",ch,NULL,NULL,TO_ROOM);
      return;
    }

  if ( (victim = get_char_room(ch,arg)) == NULL)
    {
      send_to_char( L"Burada değil.\n\r",ch);
      return;
    }

  if (IS_NPC(ch))
    {
      send_to_char( L"Önce kendini uysallaştırsan????",ch);
      return;
    }

  if (!IS_NPC(victim))
    {
      act( L"$N uysallaştırılamaz.",ch,NULL,victim,TO_CHAR);
      return;
    }

  if (!IS_SET(victim->act,ACT_AGGRESSIVE))
    {
      act( L"$N genelde saldırgan değildir.",ch,NULL,victim,TO_CHAR);
      return;
    }

  WAIT_STATE( ch, skill_table[gsn_tame].beats );

  if (number_percent() < get_skill(ch,gsn_tame) + 15
                         + 4*(ch->level - victim->level))
    {
      REMOVE_BIT(victim->act,ACT_AGGRESSIVE);
      SET_BIT(victim->affected_by,AFF_CALM);
      send_to_char( L"Uysallaşıyorsun.\n\r",victim);
      act( L"$M uysallaştırıyorsun.",ch,NULL,victim,TO_CHAR);
      act( L"$n $M uysallaştırıyor.",ch,NULL,victim,TO_NOTVICT);
      stop_fighting(victim,TRUE);
      check_improve(ch,gsn_tame,TRUE,1);
    }
  else
    {
      send_to_char( L"Başaramadın.\n\r",ch);
      act( L"$n $M uysallaştırmayı denedi ama başaramadı.",ch,NULL,victim,TO_NOTVICT);
      act( L"$n seni uysallaştırmayı denedi ama başaramadı.",ch,NULL,victim,TO_VICT);
      check_improve(ch,gsn_tame,FALSE,1);
    }
}

void do_assassinate( CHAR_DATA *ch, wchar_t *argument )
{
  wchar_t arg[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;
  wchar_t buf[MAX_STRING_LENGTH];
  int chance = 0;
  OBJ_DATA *obj;

  if ( MOUNTED(ch) )
  {
    send_to_char( L"Biniciyken suikast deneyemezsin!\n\r", ch);
    return;
  }

  one_argument( argument, arg );

  if (ch->master != NULL && IS_NPC(ch))
  {
    return;
  }

  if ( !IS_NPC(ch) && ch->level < skill_table[gsn_assassinate].skill_level[ch->iclass] )
  {
    send_to_char( L"Suikast yapmayı bilmiyorsun.\n\r",ch);
    return;
  }

  if ( IS_AFFECTED( ch, AFF_CHARM ) )
  {
    send_to_char( L"Pek sevgili efendini öldürmek istemiyorsun.\n\r",ch);
    return;
  }

  if ( arg[0] == '\0' )
  {
    send_to_char( L"Kime suikast yapacaksın?\n\r", ch );
    return;
  }

  if ( ( victim = get_char_room( ch, arg ) ) == NULL )
  {
    send_to_char( L"O burada değil.\n\r", ch );
    return;
  }

  if ( victim == ch )
  {
    send_to_char( L"İntihar etmek istemiyorsun.\n\r", ch );
    return;
  }

  if ( is_safe( ch, victim ) )
    return;

  if ( IS_IMMORTAL( victim ) )
  {
    send_to_char( L"Korkudan ellerini kıpırdatamıyorsun.\n\r", ch );
    return;
  }

  if ( victim->fighting != NULL )
  {
    send_to_char( L"Dövüşen birine suikast deneyemezsin.\n\r", ch );
    return;
  }

  if ( (get_wield_char( ch,FALSE ) != NULL) || (get_hold_char( ch ) != NULL) )
  {
    send_to_char( L"Suikast yapmak için iki elin de boşta olmalı.\n\r", ch );
    return;
  }

  if ( (victim->hit < victim->max_hit) && (can_see(victim, ch)) && (IS_AWAKE(victim) ) )
  {
    act( L"$N yaralı ve tedirgin... gizlice sokulamazsın.",ch, NULL, victim, TO_CHAR );
    return;
  }

  WAIT_STATE( ch, skill_table[gsn_assassinate].beats );

  chance += int( get_curr_stat( ch , STAT_INT ) / 4 );
  chance += int( get_curr_stat( ch , STAT_WIS ) / 4 );
  chance += int( get_curr_stat( ch , STAT_DEX ) / 4 );
  chance += int( ( get_skill( ch , gsn_assassinate ) - 75 ) / 2 ) ;

  obj = get_eq_char( ch, WEAR_HANDS );

  if( obj == NULL )
  {
    chance += 10;
  }
  else if( is_metal( obj ) )
  {
    chance -= 20;
  }
  else
  {
    chance -= 5;
  }

  if ( IS_NPC(ch) ||
	!IS_AWAKE(victim)
	||   number_percent( ) < chance)
      multi_hit(ch,victim,gsn_assassinate);
    else
      {
	check_improve(ch,gsn_assassinate,FALSE,1);
	damage( ch, victim, 0, gsn_assassinate,DAM_NONE, TRUE );
      }
    /* Player shouts if he doesn't die */
    if (!(IS_NPC(victim)) && !(IS_NPC(ch))
	&& victim->position == POS_FIGHTING)
      {
	if (!can_see(victim, ch))
	  do_yell(victim, (wchar_t*)"İmdat! Biri bana suikast denedi!");
	else
	  {
      swprintf( buf, MAX_STRING_LENGTH-1, L"İmdat! %s bana suikast düzenlemeye çalıştı!",
		    (is_affected(ch,gsn_doppelganger) && !IS_IMMORTAL(victim)) ?
		    ch->doppel->name : ch->name );
	    do_yell( victim, buf );
	  }
      }
    return;
  }


void do_caltraps(CHAR_DATA *ch, wchar_t *argument)
{
  CHAR_DATA *victim = ch->fighting;

  if (IS_NPC(ch) || ch->pcdata->learned[gsn_caltraps] < 1)
    {
      send_to_char( L"Çivi mi? İnşaat malzemesine mi ihtiyacın var?\n\r",ch);
      return;
    }

  if (victim == NULL)
    {
      send_to_char( L"Dövüşüyor olmalısın.\n\r",ch);
      return;
    }

  if (is_safe(ch,victim))
    return;

    act( L"$S bacaklarına bir avuç çivi fırlatıyorsun.",
        ch,NULL,victim,TO_CHAR);
    act( L"$n bacaklarına bir avuç çivi fırlatıyor!",
      ch,NULL,victim,TO_VICT);

  WAIT_STATE(ch,skill_table[gsn_caltraps].beats);

  if (!IS_NPC(ch) && number_percent() >= get_skill(ch,gsn_caltraps))
    {
      damage(ch,victim,0,gsn_caltraps,DAM_PIERCE, TRUE);
      check_improve(ch,gsn_caltraps,FALSE,1);
      return;
    }

  damage(ch,victim,ch->level,gsn_caltraps,DAM_PIERCE, TRUE);

  if (!is_affected(victim,gsn_caltraps))
    {
      AFFECT_DATA tohit,todam,todex;

      tohit.where     = TO_AFFECTS;
      tohit.type      = gsn_caltraps;
      tohit.level     = ch->level;
      tohit.duration  = -1;
      tohit.location  = APPLY_HITROLL;
      tohit.modifier  = -5;
      tohit.bitvector = 0;
      affect_to_char( victim, &tohit );

      todam.where = TO_AFFECTS;
      todam.type = gsn_caltraps;
      todam.level = ch->level;
      todam.duration = -1;
      todam.location = APPLY_DAMROLL;
      todam.modifier = -5;
      todam.bitvector = 0;
      affect_to_char( victim, &todam);

      todex.type = gsn_caltraps;
      todex.level = ch->level;
      todex.duration = -1;
      todex.location = APPLY_DEX;
      todex.modifier = -5;
      todex.bitvector = 0;
      affect_to_char( victim, &todex);

      act( L"$N topallamaya başlıyor.",ch,NULL,victim,TO_CHAR);
      act( L"Topallamaya başlıyorsun.",ch,NULL,victim,TO_VICT);
      check_improve(ch,gsn_caltraps,TRUE,1);
    }
}


void do_throw( CHAR_DATA *ch, wchar_t *argument )
{
    CHAR_DATA *victim;
    wchar_t arg[MAX_INPUT_LENGTH];
    int chance;

    if ( MOUNTED(ch) )
    {
      send_to_char( L"Biniciyken olmaz!\n\r", ch);
        return;
    }

    argument = one_argument(argument,arg);

    if (!wcscasecmp(arg, L"spear") || !wcscasecmp(arg, L"mızrak"))
    {
     do_throw_spear(ch,argument);
     return;
    }

    if ( IS_NPC(ch) ||
         ch->level < skill_table[gsn_throw].skill_level[ch->iclass] )
    {
	send_to_char(L"Birşey fırlatmayı beceremezsin.\n\r", ch );
	return;
    }

    if (IS_AFFECTED(ch,AFF_FLYING))
	{
    send_to_char( L"Ayakların yere basmalı.\n\r",ch);
	 return;
	}

    if ( ( victim = ch->fighting ) == NULL )
    {
	send_to_char( L"Kimseyle dövüşmüyorsun.\n\r", ch );
	return;
    }

    if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)
    {
      act( L"Fakat $N senin arkadaşın!",ch,NULL,victim,TO_CHAR);
	return;
    }

    if (is_safe(ch,victim))
      return;

    WAIT_STATE( ch, skill_table[gsn_throw].beats );

    if (is_affected(victim, gsn_protective_shield))
     {
       act_color( L"$COnu kolundan yakalayamıyorsun.$c",ch,NULL,victim, TO_CHAR,
       	POS_FIGHTING,CLR_YELLOW);
       act_color( L"$C$n seni kolundan yakalayamadı.$c", ch, NULL, victim, TO_VICT,
       	POS_FIGHTING,CLR_YELLOW);
       act_color( L"$C$n $M fırlatamadı.$c",ch,NULL,victim,TO_NOTVICT,
       	POS_FIGHTING,CLR_YELLOW);
	return;
     }

    chance = get_skill(ch,gsn_throw);

    if (ch->size < victim->size)
	chance += (ch->size - victim->size) * 10;
    else
	chance += (ch->size - victim->size) * 25;


    /* stats */
    chance += get_curr_stat(ch,STAT_STR);
    chance -= get_curr_stat(victim,STAT_DEX) * 4/3;

    if (IS_AFFECTED(victim,AFF_FLYING) ) chance += 10;

    /* speed */
    if (IS_SET(ch->off_flags,OFF_FAST))
	chance += 10;
    if (IS_SET(victim->off_flags,OFF_FAST))
	chance -= 20;

    /* level */
    chance += (ch->level - victim->level) * 2;

    if ( IS_NPC(ch) || number_percent() < chance )
    {
      act( L"$M sersemletici bir güçle fırlatıyorsun.",
	  ch,NULL,victim,TO_CHAR);
      act( L"$n seni sersemletici bir güçle fırlatıyor.",
	  ch,NULL,victim,TO_VICT);
      act( L"$n $M sersemletici bir güçle fırlatıyor.",
	  ch,NULL,victim,TO_NOTVICT);
      WAIT_STATE(victim,2 * PULSE_VIOLENCE);

      damage( ch, victim,ch->level + get_curr_stat(ch,STAT_STR),
	     gsn_throw,DAM_BASH, TRUE );
      check_improve(ch,gsn_throw,TRUE,1);
    }
    else
    {
      act( L"Onu yakalayamadın.", ch, NULL, NULL, TO_CHAR);
    	act( L"$N seni fırlatmayı denedi, ama beceremedi.", victim, NULL, ch,TO_CHAR);
    	act( L"$n $S kolundan yakalamaya çalışıyor.", ch, NULL, victim, TO_NOTVICT);
	check_improve(ch,gsn_throw,FALSE,1);
    }

    return;
}

void do_strangle(CHAR_DATA *ch, wchar_t *argument)
{
    CHAR_DATA *victim;
    AFFECT_DATA af;
    int chance;

    if ( MOUNTED(ch) )
    {
      send_to_char( L"Biniciyken olmaz!\n\r", ch);
        return;
    }

    if ( IS_NPC(ch) ||
	 ch->level < skill_table[gsn_strangle].skill_level[ch->iclass] )
    {
      send_to_char( L"Bayıltma yeteneğin yok.\n\r",ch);
	return;
    }


    if ( (victim = get_char_room(ch,argument)) == NULL)
      {
        send_to_char( L"Onu göremiyorsun.\n\r",ch);
	return;
      }

    if (ch==victim)
      {
        send_to_char( L"Bu kadar aptal olamazsın.\n\r",ch);
	return;
      }

    if ( IS_AFFECTED( ch, AFF_CHARM ) && victim == ch->leader)  {
      send_to_char( L"Pek sevgili efendinin boynunu sıkmak istemiyorsun.\n\r",ch);
	return;
    }

    if (IS_AFFECTED(victim,AFF_SLEEP))  {
      act( L"$E zaten uyuyor.",ch,NULL,victim,TO_CHAR);
      return;
    }

    if (is_affected(victim,gsn_neckguard) )
    {
      act( L"$N boynunu koruyor.", ch, NULL, victim, TO_CHAR );
	return;
    }

    if (is_safe(ch,victim))
      return;

    victim->last_fight_time = current_time;
    ch->last_fight_time = current_time;

    WAIT_STATE(ch,skill_table[gsn_strangle].beats);

    if (IS_NPC(ch)) chance = UMIN( 35, ch->level /2 );
    else chance = (int)(0.6 * (float)get_skill(ch,gsn_strangle));

    if ( IS_NPC(victim) && victim->pIndexData->pShop != NULL )
	   chance -= 40;

    if ( number_percent() < chance )
      {
        act( L"$M boynundan yakalayarak uyutuyorsun.",
      	    ch,NULL,victim,TO_CHAR);
      	act( L"$n seni boynundan yakalayarak uyutuyor.",
      	    ch,NULL,victim,TO_VICT);
      	act( L"$n $M boynundan yakalayarak uyutuyor.",
	    ch,NULL,victim,TO_NOTVICT);
	check_improve(ch,gsn_strangle,TRUE,1);

	af.type = gsn_strangle;
        af.where = TO_AFFECTS;
	af.level = ch->level;
	af.duration =  ch->level / 50 + 1;
	af.location = APPLY_NONE;
	af.modifier = 0;
	af.bitvector = AFF_SLEEP;
	affect_join ( victim,&af );

	if (IS_AWAKE(victim))
	  victim->position = POS_SLEEPING;
      }
    else
      {
	wchar_t buf[MAX_STRING_LENGTH];

	damage(ch,victim,0,gsn_strangle,DAM_NONE, TRUE);
	check_improve(ch,gsn_strangle,FALSE,1);
	if (!can_see(victim, ch))
	  do_yell(victim, (wchar_t*)"İmdat! Biri beni boğuyor!");
	else
	{
    swprintf( buf, MAX_STRING_LENGTH-1, L"İmdat! %s beni boğazlıyor!",
		    (is_affected(ch,gsn_doppelganger)&& !IS_IMMORTAL(victim))?
		    ch->doppel->name : ch->name );
	    if (!IS_NPC(victim)) do_yell(victim,buf);
	}
        af.type = gsn_neckguard;
        af.where = TO_AFFECTS;
        af.level = victim->level;
        af.duration = 2 + victim->level / 50;
        af.modifier = 0;
        af.bitvector = 0;
        af.location  = APPLY_NONE;
        affect_join(victim, &af);
      }
}

void do_blackjack(CHAR_DATA *ch, wchar_t *argument)
{
    CHAR_DATA *victim;
    AFFECT_DATA af;
    int chance;

    if ( MOUNTED(ch) )
    {
      send_to_char( L"Biniciyken coplayamazsın!\n\r", ch);
        return;
    }

    if ( IS_NPC(ch) ||
	 ch->level < skill_table[gsn_blackjack].skill_level[ch->iclass] )

    {
      send_to_char( L"Hı?\n\r",ch);
	return;
    }

    if ( (victim = get_char_room(ch,argument)) == NULL)
      {
        send_to_char( L"Onu göremiyorsun.\n\r",ch);
	return;
      }

    if (ch==victim)
      {
        send_to_char( L"Kendini coplamak mı???\n\r",ch);
	return;
      }


    if ( IS_AFFECTED( ch, AFF_CHARM ) && victim == ch->leader)  {
      send_to_char( L"Pek sevgili efendini coplamak istemiyorsun.\n\r",ch);
	return;
    }

    if (IS_AFFECTED(victim,AFF_SLEEP))  {
      act( L"$E zaten uyuyor.",ch,NULL,victim,TO_CHAR);
      return;
    }

    if (is_affected(victim,gsn_headguard) )
    {
      act( L"$N boynunu koruyor!.", ch, NULL, victim, TO_CHAR );
	return;
    }

    if (is_safe(ch,victim))
      return;
    victim->last_fight_time = current_time;
    ch->last_fight_time = current_time;

    WAIT_STATE(ch,skill_table[gsn_blackjack].beats);

    chance = (int)(0.5 * (float)get_skill(ch,gsn_blackjack));
    chance += URANGE( 0, (get_curr_stat(ch,STAT_DEX)-20)*2, 10);
    chance += can_see(victim, ch) ? 0 : 5;
    if ( IS_NPC(victim) )
	if ( victim->pIndexData->pShop != NULL )
	   chance -= 40;

    if (IS_NPC(ch) ||
	number_percent() < chance)
      {
        act( L"$S kafasını copluyorsun.",
      	    ch,NULL,victim,TO_CHAR);
      	act( L"Kafatasına hücum eden bir acı hissediyorsun!",
      	    ch,NULL,victim,TO_VICT);
      	act( L"$n $S kafasına bir copla vuruyor!",
	    ch,NULL,victim,TO_NOTVICT);
	check_improve(ch,gsn_blackjack,TRUE,1);

	af.type = gsn_blackjack;
        af.where = TO_AFFECTS;
	af.level = ch->level;
	af.duration =  ch->level / 50 + 1;
	af.location = APPLY_NONE;
	af.modifier = 0;
	af.bitvector = AFF_SLEEP;
	affect_join ( victim,&af );

	if (IS_AWAKE(victim))
	  victim->position = POS_SLEEPING;
      }
    else
      {
	wchar_t buf[MAX_STRING_LENGTH];

	damage(ch,victim,ch->level / 2,gsn_blackjack,DAM_NONE, TRUE);
	check_improve(ch,gsn_blackjack,FALSE,1);
	if (!IS_NPC(victim))
	{
	if (!can_see(victim, ch))
	  do_yell(victim, (wchar_t*)"İmdat! Biri beni copluyor!");
	else
	{
    swprintf( buf, MAX_STRING_LENGTH-1, L"İmdat! %s beni copluyor!",
		    (is_affected(ch,gsn_doppelganger)&& !IS_IMMORTAL(victim))?
		    ch->doppel->name : ch->name );
	    if (!IS_NPC(victim)) do_yell(victim,buf);
	}
	}
        af.type = gsn_headguard;
        af.where = TO_AFFECTS;
        af.level = victim->level;
        af.duration = 2 + victim->level / 50;
        af.modifier = 0;
        af.bitvector = 0;
        af.location  = APPLY_NONE;
        affect_join(victim, &af);
      }
}



void do_bloodthirst( CHAR_DATA *ch, wchar_t *argument)
{
    int chance, hp_percent;

    if (ch_skill_nok_nomessage(ch,gsn_bloodthirst))
    {
      send_to_char( L"O kadar susamadın.\n\r",ch);
	return;
    }

    if (!cabal_ok(ch,gsn_bloodthirst))
      return;

    chance = get_skill(ch,gsn_bloodthirst);

    if (IS_AFFECTED(ch,AFF_BLOODTHIRST))
    {
      send_to_char( L"Kana olan susuzluğun devam ediyor.\n\r",ch);
	return;
    }

    if (IS_AFFECTED(ch,AFF_CALM))
    {
      send_to_char( L"Kendini kan içemeyecek kadar yumuşak hissediyorsun.\n\r",ch);
	return;
    }

    if (ch->fighting == NULL)
      {
        send_to_char( L"Dövüşüyor olmalısın.\n\r",ch);
	return;
      }

    /* modifiers */

    hp_percent = 100 * ch->hit/ch->max_hit;
    chance += 25 - hp_percent/2;

    if (number_percent() < chance)
    {
	AFFECT_DATA af;

	WAIT_STATE(ch, PULSE_VIOLENCE);


  send_to_char( L"Kana susuyorsun!\n\r",ch);
	act( L"$n kana susamış gözlerle bakmaya başladı.",ch,NULL,NULL,TO_ROOM);
	check_improve(ch,gsn_bloodthirst,TRUE,2);

        af.where	= TO_AFFECTS;
	af.type		= gsn_bloodthirst;
	af.level	= ch->level;
	af.duration	= 2 + ch->level / 18;
	af.modifier	= 5 + ch->level / 4;
	af.bitvector 	= AFF_BLOODTHIRST;

	af.location	= APPLY_HITROLL;
	affect_to_char(ch,&af);

	af.location	= APPLY_DAMROLL;
	affect_to_char(ch,&af);

	af.modifier	= - UMIN(ch->level - 5,35);
	af.location	= APPLY_AC;
	affect_to_char(ch,&af);
    }

    else
    {
	WAIT_STATE(ch,3 * PULSE_VIOLENCE);

  send_to_char( L"Bir an için kana susadığını hissediyorsun fakat sonra geçiyor.\n\r",ch);
	check_improve(ch,gsn_bloodthirst,FALSE,2);
    }
}


void do_spellbane(CHAR_DATA *ch, wchar_t *argument)
{
  AFFECT_DATA af;

  if (ch_skill_nok(ch,gsn_spellbane))
      return;

  if (!cabal_ok(ch,gsn_spellbane))
    return;

  if (is_affected(ch,gsn_spellbane))
  {
    send_to_char( L"Büyüleri zaten saptırıyorsun.\n\r",ch);
      return;
  }

  WAIT_STATE( ch, skill_table[gsn_spellbane].beats );

  af.where 	= TO_DETECTS;
  af.type 	= gsn_spellbane;
  af.level 	= ch->level;
  af.duration   = ch->level / 3;
  af.location 	= APPLY_SAVING_SPELL;
  af.modifier 	= -ch->level/4;
  af.bitvector  = ADET_SPELLBANE;

  affect_to_char(ch,&af);

  act( L"Seni saran büyüden nefret ediyorsun.",ch,NULL,NULL,TO_CHAR);
  act( L"$n havayı büyü nefretiyle dolduruyor.",ch,NULL,NULL,TO_ROOM);
  check_improve(ch,gsn_spellbane,TRUE,1);

}

void do_resistance(CHAR_DATA *ch, wchar_t *argument)
{
  if (ch_skill_nok(ch,gsn_resistance) )
      return;

  if (!cabal_ok(ch,gsn_resistance))
    return;

  if (is_affected(ch,gsn_resistance))
    {
      send_to_char( L"Olabildiğince dirençlisin.\n\r",ch);
      return;
    }

  if (ch->mana < 50)
    {
      send_to_char( L"Enerji toplayamıyorsun.\n\r",ch);
      return;
    }

  WAIT_STATE( ch, skill_table[gsn_resistance].beats );

  if (IS_NPC(ch) || number_percent() < get_skill(ch,gsn_resistance))
    {
      AFFECT_DATA af;

      af.where	= TO_DETECTS;
      af.type 	= gsn_resistance;
      af.level 	= ch->level;
      af.duration = ch->level / 6;
      af.location = APPLY_NONE;
      af.modifier = 0;
      af.bitvector = ADET_PROTECTOR;

      affect_to_char(ch,&af);
      ch->mana -= 50;

      act( L"Kendini dayanıklı hissediyorsun!",ch,NULL,NULL,TO_CHAR);
      act( L"$n dayanıklılaşmış görünüyor.",ch,NULL,NULL,TO_ROOM);
      check_improve(ch,gsn_resistance,TRUE,1);
    }
  else
    {
      ch->mana -= 25;

      send_to_char( L"Kaslarını geriyorsun ama dayanıklılaşmış görünmüyorsun.\n\r",ch);
       act( L"$n dayanıklı görünmek için kaslarını geriyor.",
	  ch,NULL,NULL,TO_ROOM);
      check_improve(ch,gsn_resistance,FALSE,1);
    }

}

void do_trophy(CHAR_DATA *ch, wchar_t *argument)
{
  int trophy_vnum;
  OBJ_DATA *trophy;
  AFFECT_DATA af;
  OBJ_DATA *part;
  wchar_t arg[MAX_INPUT_LENGTH];
  int level;
  one_argument( argument, arg );

  if (ch_skill_nok(ch,gsn_trophy))
	return;

  if (!cabal_ok(ch, gsn_trophy))
    return;

  if (is_affected(ch, gsn_trophy))
    {
      send_to_char( L"Zaten bir andacın var!\n\r", ch);
      return;
    }

  if (ch->mana < 30)
    {
      send_to_char( L"Andaç için konsantre olamayacak kadar zayıf hissediyorsun.\n\r", ch);
      return;
    }

  if ( arg[0] == '\0' )
    {
      send_to_char( L"Neyin andacı?\n\r", ch );
      return;
    }

  if ( ( part = get_obj_carry( ch, arg ) ) == NULL )
    {
      send_to_char( L"O parçaya sahip değilsin.\n\r", ch );
      return;
    }

    if ( number_percent( ) < (get_skill( ch, gsn_trophy )/3)*2 )
    {
      send_to_char( L"Başaramadın ve onu parçaladın.\n\r", ch);
      extract_obj(part);
      return;
    }

  WAIT_STATE(ch, skill_table[gsn_trophy].beats);

  if (part->pIndexData->vnum == OBJ_VNUM_SLICED_ARM)
    trophy_vnum = OBJ_VNUM_BATTLE_PONCHO;
  else if (part->pIndexData->vnum == OBJ_VNUM_SLICED_LEG)
    trophy_vnum = OBJ_VNUM_BATTLE_PONCHO;
  else if (part->pIndexData->vnum == OBJ_VNUM_SEVERED_HEAD)
    trophy_vnum = OBJ_VNUM_BATTLE_PONCHO;
  else if (part->pIndexData->vnum == OBJ_VNUM_TORN_HEART)
    trophy_vnum = OBJ_VNUM_BATTLE_PONCHO;
  else if (part->pIndexData->vnum == OBJ_VNUM_GUTS)
    trophy_vnum = OBJ_VNUM_BATTLE_PONCHO;
  else if (part->pIndexData->vnum == OBJ_VNUM_BRAINS)
    {
      send_to_char( L"Neden onu yemeyi düşünmüyorsun?\n\r", ch);
      return;
    }
  else
    {
      send_to_char( L"Ondan bir andaç çıkaramazsın!\n\r", ch);
      return;
    }

  if (part->from[0] == '\0')
    {
      send_to_char( L"Geçersiz vücut parçası.\n\r", ch);
      return;
    }

  if (!IS_NPC(ch) && number_percent() < ch->pcdata->learned[gsn_trophy])
    {
      af.where  = TO_AFFECTS;
      af.type	= gsn_trophy;
      af.level	= ch->level;
      af.duration	= ch->level/2;
      af.modifier	= 0;
      af.bitvector 	= 0;

      af.location	= 0;
      affect_to_char(ch,&af);

      if ( trophy_vnum != 0 )
	{
	  wchar_t buf[MAX_STRING_LENGTH];
	  level = UMIN(part->level + 5, MAX_LEVEL);

	  trophy = create_object( get_obj_index( trophy_vnum ), level );
	  trophy->timer = ch->level * 2;

	  swprintf( buf, MAX_STRING_LENGTH-1, trophy->short_descr, part->from );
	  free_string( trophy->short_descr );
	  trophy->short_descr = str_dup( buf );

	  swprintf( buf, MAX_STRING_LENGTH-1, trophy->description, part->from );
	  free_string( trophy->description );
	  trophy->description = str_dup( buf );
	  trophy->cost  = 0;
	  trophy->level = ch->level;
	  ch->mana -= 30;
	  af.where	= TO_OBJECT;
	  af.type 	= gsn_trophy;
	  af.level	= level;
	  af.duration	= -1;
	  af.location	= APPLY_DAMROLL;
	  af.modifier  = ch->level / 5;
	  af.bitvector	= 0;
	  affect_to_obj( trophy, &af );

	  af.location	= APPLY_HITROLL;
	  af.modifier  = ch->level / 5;
	  af.bitvector	= 0;
	  affect_to_obj( trophy, &af );

	  af.location	= APPLY_INT;
	  af.modifier	= level>20?-2:-1;
	  affect_to_obj( trophy, &af );

	  af.location	= APPLY_STR;
	  af.modifier	= level>20?2:1;
	  affect_to_obj( trophy, &af );

	  trophy->value[0] = ch->level;
	  trophy->value[1] = ch->level;
	  trophy->value[2] = ch->level;
	  trophy->value[3] = ch->level;


	  obj_to_char(trophy, ch);
          check_improve(ch, gsn_trophy, TRUE, 1);

          act( L"$p ile bir başlık yaptın!",ch,part,NULL,TO_CHAR);
      	  act( L"$n $p ile bir başlık yaptı!",ch,part,NULL,TO_ROOM);

	  extract_obj(part);
	  return;
	}
    }
    else
    {
      send_to_char( L"Onu parçaladın.\n\r", ch);
      extract_obj(part);
      ch->mana -= 15;
      check_improve(ch, gsn_trophy, FALSE, 1);
    }
}


void do_truesight(CHAR_DATA *ch, wchar_t *argument)
{
  if (ch_skill_nok(ch,gsn_truesight))
    return;

  if (!cabal_ok(ch,gsn_truesight))
    return;

  if (is_affected(ch,gsn_truesight))
    {
      send_to_char( L"Gözlerin zaten keskin.\n\r",ch);
      return;
    }

  if (ch->mana < 50)
    {
      send_to_char( L"Yeterince odaklanamadın.\n\r",ch);
      return;
    }

  WAIT_STATE( ch, skill_table[gsn_truesight].beats );

  if (!IS_NPC(ch) && number_percent() < ch->pcdata->learned[gsn_truesight])
    {
      AFFECT_DATA af;

      af.where  = TO_DETECTS;
      af.type 	= gsn_truesight;
      af.level 	= ch->level;
      af.duration = ch->level/2 + 5;
      af.location = APPLY_NONE;
      af.modifier = 0;
      af.bitvector = DETECT_HIDDEN;
      affect_to_char(ch, &af);

      af.bitvector = DETECT_INVIS;
      affect_to_char(ch, &af);

      af.bitvector = DETECT_IMP_INVIS;
      affect_to_char(ch,&af);

      af.bitvector = ACUTE_VISION;
      affect_to_char(ch,&af);

      af.bitvector = DETECT_MAGIC;
      affect_to_char(ch,&af);

      ch->mana -= 50;

      act( L"Keskin gözlerle etrafı süzüyorsun!",ch,NULL,NULL,TO_CHAR);
      act( L"$n aydınlanmış görünüyor.",ch,NULL,NULL,TO_ROOM);
      check_improve(ch,gsn_truesight,TRUE,1);
    }
  else
    {
      ch->mana -= 25;

      send_to_char( L"Keskin gözlerle etrafa bakıyorsun, fakat yeni birşey görmüyorsun.\n\r",ch);
       act( L"$n keskin gözlerle etrafa bakıyor fakat aydınlanmış görünmüyor.",
	  ch,NULL,NULL,TO_ROOM);
      check_improve(ch,gsn_truesight,FALSE,1);
    }

}

void do_warcry(CHAR_DATA *ch, wchar_t *argument)
{
  AFFECT_DATA af;

  if (IS_NPC(ch) ||
      ch->level < skill_table[gsn_warcry].skill_level[ch->iclass] )
    {
      send_to_char( L"Hı?\n\r", ch );
      return;
    }

  if (is_affected(ch, gsn_warcry))
    {
      send_to_char( L"Zaten daha önce çok etkili bir nara atmışsın?!\n\r",ch);
      return;
    }

  if (ch->mana < 30)
    {
      send_to_char( L"Yeterince konsantre olamıyorsun.\n\r",ch);
      return;
    }

  WAIT_STATE( ch, skill_table[gsn_warcry].beats );

  if (number_percent() > ch->pcdata->learned[gsn_warcry])
    {
      send_to_char( L"Nazikçe hırlıyorsun.\n\r", ch);
      act( L"$n yumuşak hırıltılar çıkarıyor.", ch, NULL, NULL, TO_ROOM);
      return;
    }

  ch->mana -= 30;

  af.where	= TO_AFFECTS;
  af.type      = gsn_warcry;
  af.level	 = ch->level;
  af.duration  = 6+ch->level;
  af.location  = APPLY_HITROLL;
  af.modifier  = ch->level / 8;
  af.bitvector = 0;
  affect_to_char( ch, &af );

  af.location  = APPLY_SAVING_SPELL;
  af.modifier  = 0 - ch->level / 8;
  affect_to_char( ch, &af );
  send_to_char( L"Nara atınca kutsandığını hissediyorsun.\n\r", ch );
  return;
}

void do_guard(CHAR_DATA *ch, wchar_t *argument)
{
  wchar_t arg[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;
  one_argument(argument, arg);

  if (ch_skill_nok(ch,gsn_guard))
      return;

  if (!cabal_ok(ch,gsn_guard))
    return;

  if ( arg[0] == '\0' )
    {
      send_to_char( L"Kimi savunacaksın?\n\r", ch);
      return;
    }

  if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
      send_to_char( L"O burada değil.\n\r", ch );
      return;
    }

  if (IS_NPC(victim))
    {
      act( L"$S yardımına ihtiyacı yok!", ch, NULL, victim, TO_CHAR);
      return;
    }

    if (!wcscasecmp(arg, L"beni") || !wcscasecmp(arg, L"ben") || victim == ch)
    {
	if (ch->guarding == NULL)
	  {
      send_to_char( L"Kendini mi?\n\r", ch);
	    return;
	  }
	else
	{
    act( L"$M savunmayı bıraktın.", ch, NULL, ch->guarding, TO_CHAR);
    act( L"$n seni savunmayı bıraktı.", ch, NULL, ch->guarding, TO_VICT);
    act( L"$n $M savunmayı bıraktı.", ch, NULL, ch->guarding, TO_NOTVICT);
      ch->guarding->guarded_by = NULL;
      ch->guarding             = NULL;
      return;
        }
    }

  if (ch->guarding == victim)
    {
      act( L"Zaten $M savunuyorsun!", ch, NULL, victim, TO_CHAR);
      return;
    }

  if (ch->guarding != NULL)
    {
      send_to_char( L"Ama sen bir başkasını savunuyorsun!\n\r", ch);
      return;
    }

  if (victim->guarded_by != NULL)
    {
      act( L"$N bir başkasınca zaten savunuluyor.",ch,NULL,victim,TO_CHAR);
      return;
    }

  if (victim->guarding == ch)
    {
      act( L"Fakat $N seni koruyor!", ch, NULL, victim, TO_CHAR);
      return;
    }

  if (!is_same_group(victim, ch))
    {
      act( L"Fakat $N ile aynı grupta değilsin.",ch,NULL,victim,TO_CHAR);
      return;
    }

  if (IS_AFFECTED(ch,AFF_CHARM))
    {
      act( L"$M savunarak efendinin canını sıkmak istemezsin!",
	  ch,NULL,victim,TO_VICT);
      return;
    }

 if (victim->fighting != NULL)
   {
     send_to_char( L"Neden onlara dövüşmeyi kesmelerini söylemiyorsun?\n\r", ch);
     return;
   }

  if (ch->fighting != NULL)
    {
      send_to_char(L"Önce kendi dövüşünü bitir....\n\r",
     ch);
      return;
    }

    act( L"$M savunmaya başladın.", ch, NULL, victim, TO_CHAR);
    act( L"$n seni savunmaya başladı.", ch, NULL, victim, TO_VICT);
    act( L"$n $M savunmaya başladı.", ch, NULL, victim, TO_NOTVICT);

  ch->guarding = victim;
  victim->guarded_by = ch;

}

CHAR_DATA *check_guard(CHAR_DATA *ch, CHAR_DATA *mob)
{
  int chance;

  if (ch->guarded_by == NULL ||
      get_char_room(ch,ch->guarded_by->name) == NULL)
      return ch;
  else
    {
      chance = (get_skill(ch->guarded_by,gsn_guard) -
		(int)(1.5 * (float)(ch->level - mob->level)));
      if (number_percent() < UMIN(100,chance))
	{
    act( L"$n kendini $S önüne atıyor!",ch->guarded_by,NULL,ch,TO_NOTVICT);
	  act( L"$n kendini senin önüne atıyor!",ch->guarded_by,NULL,ch,TO_VICT);
	  act( L"Kendini $S önüne atıyorsunN!",ch->guarded_by,NULL,ch,TO_CHAR);
	  check_improve(ch->guarded_by,gsn_guard,TRUE,3);
	  return ch->guarded_by;
	}
      else
	{
	  check_improve(ch->guarded_by, gsn_guard, FALSE, 3);
	  return ch;
	}

    }
}


void do_explode(CHAR_DATA *ch, wchar_t *argument)
{
  CHAR_DATA *victim = ch->fighting;
  CHAR_DATA *vch, *vch_next;
  int dam=0,hp_dam,dice_dam,mana;
  int hpch,level= ch->level;
  wchar_t arg[MAX_INPUT_LENGTH];

  if (IS_NPC(ch)) return;

  if (ch_skill_nok_nomessage(ch, gsn_explode))
  {
    send_to_char( L"Ateş? O da ne?\n\r",ch);
      return;
  }

  if (victim == NULL)
  {
     one_argument(argument, arg);
     if ( arg[0] == '\0' )
     {
       send_to_char( L"Patlayıcı maddelerle oynamaya başladın.\n\r",ch);
	return;
     }
     if ((victim = get_char_room(ch,arg)) == NULL)
     {
	send_to_char( L"O burada değil.\n\r",ch);
	return;
     }
  }

  mana= skill_table[gsn_explode].min_mana;

  if (ch->mana < mana )
	{
    send_to_char( L"Ateşlemek için yeterli enerjiyi bulamıyorsun.\n\r",ch);
	 return;
	}
  ch->mana -= mana;

  act( L"$n birşey yakıyor.",ch,NULL,victim,TO_NOTVICT);
  act( L"$n senin üzerine ateşe verilmiş patlayıcı madde atıyor!",ch,NULL,victim,TO_VICT);
  act( L"Hepsini yak!.",ch,NULL,NULL,TO_CHAR);

  WAIT_STATE(ch,skill_table[gsn_explode].beats);

  if (!IS_NPC(ch) && number_percent() >= ch->pcdata->learned[gsn_explode])
    {
      damage(ch,victim,0,gsn_explode,DAM_FIRE, TRUE);
      check_improve(ch,gsn_explode,FALSE,1);
      return;
    }

    hpch = UMAX( 10, ch->hit );
    hp_dam  = number_range( hpch/9+1, hpch/5 );
    dice_dam = dice(level,20);

  if (!(is_safe(ch,victim)))
   {
    dam = UMAX(hp_dam + dice_dam /10, dice_dam + hp_dam / 10);
    fire_effect(victim->in_room,level,dam/2,TARGET_ROOM);
  }
    for (vch = victim->in_room->people; vch != NULL; vch = vch_next)
    {
	vch_next = vch->next_in_room;

	if (is_safe_spell(ch,vch,TRUE)
	||  (IS_NPC(vch) && IS_NPC(ch)
	&&   (ch->fighting != vch || vch->fighting != ch)))
	    continue;
	if ( is_safe(ch, vch) )
          continue;

	if (vch == victim) /* full damage */
	{
		fire_effect(vch,level,dam,TARGET_CHAR);
		damage(ch,vch,dam,gsn_explode,DAM_FIRE,TRUE);
	}
	else /* partial damage */
	{
		fire_effect(vch,level/2,dam/4,TARGET_CHAR);
		damage(ch,vch,dam/2,gsn_explode,DAM_FIRE,TRUE);
	}
    }
  if (!IS_NPC(ch) && number_percent() >= ch->pcdata->learned[gsn_explode])
	{
	 fire_effect(ch,level/4,dam/10,TARGET_CHAR);
	 damage(ch,ch,(ch->hit / 10),gsn_explode,DAM_FIRE,TRUE);
	}
}



void do_target (CHAR_DATA *ch, wchar_t *argument)
{
    CHAR_DATA *victim;


    if ( !IS_NPC(ch)
    &&   ch->level < skill_table[gsn_target].skill_level[ch->iclass] )
    {
	send_to_char(L"Bir grupla dövüşürken hedefini nasıl değiştireceğini bilmiyorsun.\n\r",ch );
	return;
    }

    if (ch->fighting == NULL)
    {
      send_to_char ( L"Henüz dövüşmüyorsun.\n\r",ch);
        return;
    }

    if (argument[0] == '\0')
    {
      send_to_char ( L"Yeni hedefin kim olacak?\n\r",ch);
        return;
    }

    if (( victim = get_char_room (ch, argument)) == NULL )
    {
      send_to_char ( L"Onu görmüyorsun.\n\r",ch);
        return;
    }


    /* check victim is fighting with him */

    if ( victim->fighting != ch)
    {
      send_to_char ( L"O seninle dövüşmüyor ki.\n\r",ch);
        return;
    }


  WAIT_STATE(ch,skill_table[gsn_target].beats);

  if (!IS_NPC(ch) && number_percent() <
	(get_skill(ch,gsn_target) / 2) )
    {
      check_improve(ch,gsn_target,FALSE,1);

    ch->fighting = victim;

    act ( L"$s hedefi artık $N!",ch,NULL,victim,TO_NOTVICT);
    act ( L"Hedefin artık $N!",ch,NULL,victim,TO_CHAR);
    act ( L"$s hedefi artık SENSİN!",ch,NULL,victim,TO_VICT);
      return;
    }

    send_to_char( L"Denedin ama başaramadın. Onurun aşkına tekrar dene!.\n\r",ch);
      check_improve(ch,gsn_target,FALSE,1);

    return;
}


void do_tiger( CHAR_DATA *ch, wchar_t *argument)
{
    int chance, hp_percent;

    if ((chance = get_skill(ch,gsn_tiger_power)) == 0
    ||  (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_BERSERK))
    ||  (!IS_NPC(ch)
    &&   ch->level < skill_table[gsn_tiger_power].skill_level[ch->iclass]))
    {
	send_to_char( L"Hı?.\n\r",ch);
	return;
    }
    act( L"$n 10 kaplan gücünü çağırıyor!.",ch,NULL,NULL,TO_ROOM);

    if (IS_AFFECTED(ch,AFF_BERSERK) || is_affected(ch,gsn_berserk) ||
    is_affected(ch,gsn_tiger_power) || is_affected(ch,skill_lookup( L"frenzy")))
    {
      send_to_char( L"Azıcık delirdin.\n\r",ch);
	return;
    }

    if (IS_AFFECTED(ch,AFF_CALM))
    {
      send_to_char( L"10 kaplanı çağıramayacak kadar sakinsin.\n\r",ch);
	return;
    }
    if (ch->in_room->sector_type != SECT_FIELD &&
           ch->in_room->sector_type != SECT_FOREST &&
           ch->in_room->sector_type != SECT_MOUNTAIN &&
           ch->in_room->sector_type != SECT_HILLS )
  {
    send_to_char( L"Kaplanlar çağrını duyamazlar.\n\r", ch );
    return;
  }


    if (ch->mana < 50)
    {
      send_to_char( L"Yeterli enerjiyi toplayamadın.\n\r",ch);
	return;
    }

    /* modifiers */

    /* fighting */
    if (ch->position == POS_FIGHTING)
	chance += 10;

    hp_percent = 100 * ch->hit/ch->max_hit;
    chance += 25 - hp_percent/2;

    if (number_percent() < chance)
    {
	AFFECT_DATA af;

	WAIT_STATE(ch,PULSE_VIOLENCE);
	ch->mana -= 50;
	ch->move /= 2;

	/* heal a little damage */
	ch->hit += ch->level * 2;
	ch->hit = UMIN(ch->hit,ch->max_hit);

  send_to_char( L"Çağrını duyan kaplanlar geldi!\n\r",ch);
	act( L"$s çağrısını duyan 10 kaplan geldi.",ch,NULL,NULL,TO_ROOM);
	check_improve(ch,gsn_tiger_power,TRUE,2);

	af.where	= TO_AFFECTS;
	af.type		= gsn_tiger_power;
	af.level	= ch->level;
	af.duration	= number_fuzzy(ch->level / 8);
	af.modifier	= UMAX(1,ch->level/5);
	af.bitvector 	= AFF_BERSERK;

	af.location	= APPLY_HITROLL;
	affect_to_char(ch,&af);

	af.location	= APPLY_DAMROLL;
	affect_to_char(ch,&af);

	af.modifier	= UMAX(10,10 * (ch->level/5));
	af.location	= APPLY_AC;
	affect_to_char(ch,&af);
    }

    else
    {
	WAIT_STATE(ch,2 * PULSE_VIOLENCE);
	ch->mana -= 25;
	ch->move /= 2;

  send_to_char( L"Bir an güçlendiğini hissettin ama değişen birşey olmadı.\n\r",ch);
	check_improve(ch,gsn_tiger_power,FALSE,2);
    }
}

void do_hara( CHAR_DATA *ch, wchar_t *argument)
{
    int chance;
    AFFECT_DATA  af;

    if ( MOUNTED(ch) )
    {
      send_to_char( L"Biniciyken harakiri yapamazsın!\n\r", ch);
        return;
    }

    if ( (chance = get_skill(ch,gsn_hara_kiri)) == 0
    ||   ch->level < skill_table[gsn_hara_kiri].skill_level[ch->iclass])
    {
      send_to_char( L"Kendini öldürmeyi deniyorsun ama acıya dayanamayınca vazgeçiyorsun.\n\r",ch);
	return;
    }

    if (is_affected(ch,gsn_hara_kiri))
    {
      send_to_char( L"Eğer kendini öldürmek istiyorsan bir ejderha inine gitmeni öneririm.\n\r",ch);
	return;
    }

    /* fighting */
    if (ch->position == POS_FIGHTING)
    {
      send_to_char( L"Dövüş bitince dene.\n\r",ch);
	return;
    }

    if (is_affected(ch,gsn_hara_kiri) )
	{
    send_to_char( L"Bir deneme daha seni öldürecek.\n\r",ch);
	 return;
	}
    if (number_percent() < chance)
    {
	AFFECT_DATA af;

	WAIT_STATE(ch,PULSE_VIOLENCE);

        ch->hit = 1;
        ch->mana = 1;
	ch->move = 1;

	if (ch->pcdata->condition[COND_HUNGER] < 40)
		ch->pcdata->condition[COND_HUNGER] = 40;
	if (ch->pcdata->condition[COND_THIRST] < 40)
		ch->pcdata->condition[COND_THIRST] = 40;

    send_to_char( L"Parmağını kestin ve şimdi kanının akıp bitmesini bekliyorsun.\n\r",ch);
  act_color( L"$C$n vücudunu kesti ve şimdi bir ölü figürü canlandırıyor.$c",ch,NULL,NULL,TO_ROOM,
	POS_FIGHTING,CLR_RED);
	check_improve(ch,gsn_hara_kiri,TRUE,2);
	do_sleep( ch, (wchar_t*)"");
	SET_BIT(ch->act,PLR_HARA_KIRI);

               af.where     = TO_AFFECTS;
               af.type      = gsn_hara_kiri;
               af.level     = ch->level;
               af.duration  = 10;
               af.location  = APPLY_NONE;
               af.modifier  = 0;
               af.bitvector = 0;
               affect_to_char( ch, &af );
    }

    else
    {
	WAIT_STATE(ch,2 * PULSE_VIOLENCE);

               af.where     = TO_AFFECTS;
               af.type      = gsn_hara_kiri;
               af.level     = ch->level;
               af.duration  = 0;
               af.location  = APPLY_NONE;
               af.modifier  = 0;
               af.bitvector = 0;
               affect_to_char( ch, &af );

               send_to_char( L"Parmağını kesemedin. Bu düşündüğün kadar kolay olmaz.\n\r",ch);
	check_improve(ch,gsn_hara_kiri,FALSE,2);
    }
}

/*
 * ground strike
*/
int ground_strike( CHAR_DATA *ch, CHAR_DATA *victim,   int dam )
{
  int diceroll;
  AFFECT_DATA baf;

  if ( IS_NPC(ch) || get_skill( ch, gsn_ground_strike) < 2 )
	return dam;

  if ( ch->in_room->sector_type != SECT_HILLS
		&& ch->in_room->sector_type != SECT_MOUNTAIN
		&& ch->in_room->sector_type != SECT_FOREST
		&& ch->in_room->sector_type != SECT_FIELD )
      return dam;

  diceroll = number_range( 0, 100 );
  if( victim -> level > ch -> level )
      diceroll += ( victim->level - ch->level ) * 2;
  if( victim -> level < ch -> level )
      diceroll -= ( ch->level - victim->level );

  if ( diceroll <= (get_skill( ch, gsn_ground_strike )/3) )
  {
      check_improve( ch, gsn_ground_strike, TRUE, 2 );
      dam += dam * diceroll/200;
  }

  if ( diceroll <= (get_skill(ch, gsn_ground_strike)/15) )
  {
      diceroll = number_percent( );
      if( diceroll < 75 )
      {
        act_color( L"$CZeminin hareket etmeye başladığını hissediyorsun!$c",
	     ch, NULL, victim, TO_VICT,POS_RESTING,CLR_RED);
	act_color( L"$C$S ayaklarının altındaki zemini kontrolün altına alıyorsun!$c",
	     ch, NULL, victim, TO_CHAR,POS_RESTING,CLR_RED );

        check_improve( ch, gsn_ground_strike, TRUE, 3 );
	WAIT_STATE( victim, 2 * PULSE_VIOLENCE );
	dam += (dam * number_range( 2, 5 )) / 5;
	return dam;
      }
      else if( diceroll > 75 && diceroll < 95 )
      {
        act_color( L"$C$s saldırısıyla kör oldun!$c", ch, NULL, victim,
			TO_VICT ,POS_RESTING,CLR_BROWN);
        act_color( L"$CSaldırınla $M kör ettin!$c", ch, NULL, victim,
			TO_CHAR,POS_RESTING,CLR_BROWN );

	check_improve( ch, gsn_ground_strike, TRUE, 4 );
	if (!IS_AFFECTED(victim,AFF_BLIND))
	{
          baf.type = gsn_dirt;
	  baf.level = ch->level;
	  baf.location = APPLY_HITROLL;
	  baf.modifier = -4;
	  baf.duration = number_range(1,5);
	  baf.bitvector = AFF_BLIND;
	  affect_to_char( victim, &baf );
        }
	dam += dam * number_range( 1, 2 );
	return dam;
      }
      else if( diceroll > 95 )
      {
        act_color( L"$C$n kalbini söktü! AahH!!$c",
             ch, NULL, victim, TO_VICT ,POS_RESTING,CLR_RED);
        act_color( L"$C$S kalbini söktün!  Eminim acımıştır!$c",
             ch, NULL, victim, TO_CHAR ,POS_RESTING,CLR_RED);

	check_improve( ch, gsn_ground_strike, TRUE, 5 );
	dam += dam * number_range( 2, 5 );
	return dam;
      }
  }

  return dam;
}

/*
 * critical strike
*/
int critical_strike( CHAR_DATA *ch, CHAR_DATA *victim,   int dam )
{
  int diceroll;
  AFFECT_DATA baf;

  if ( IS_NPC(ch) || get_skill( ch, gsn_critical ) < 2 )
	return dam;

  if ( get_wield_char(ch,FALSE) != NULL
	&& get_wield_char(ch,TRUE) != NULL
	&& number_percent() > ( (ch->hit * 100) / ch->max_hit ) )
      return dam;

  diceroll = number_range( 0, 100 );
  if( victim -> level > ch -> level )
      diceroll += ( victim->level - ch->level ) * 2;
  if( victim -> level < ch -> level )
      diceroll -= ( ch->level - victim->level );

  if ( diceroll <= (get_skill( ch, gsn_critical )/2) )
  {
      check_improve( ch, gsn_critical, TRUE, 2 );
      dam += dam * diceroll/200;
  }

  if ( diceroll <= (get_skill(ch, gsn_critical)/13) )
  {
      diceroll = number_percent( );
      if( diceroll < 75 )
      {
        act_color( L"$C$n seni tuhaf bir judo hamlesyle yere çaldı!$c",
	     ch, NULL, victim, TO_VICT,POS_RESTING,CLR_RED);
	act_color( L"$C$M tuhaf bir judo hamlesiyle yere çaldın!$c",
	     ch, NULL, victim, TO_CHAR,POS_RESTING,CLR_RED );

        check_improve( ch, gsn_critical, TRUE, 3 );
	WAIT_STATE( victim, 2 * PULSE_VIOLENCE );
	dam += (dam * number_range( 2, 5 )) / 5;
	return dam;
      }
      else if( diceroll > 75 && diceroll < 95 )
      {
        act_color( L"$C$s saldırısıyla kör oldun!$c", ch, NULL, victim,
			TO_VICT ,POS_RESTING,CLR_BROWN);
        act_color( L"$CSaldırınla $M kör ettin!$c", ch, NULL, victim,
			TO_CHAR,POS_RESTING,CLR_BROWN );

	check_improve( ch, gsn_critical, TRUE, 4 );
	if (!IS_AFFECTED(victim,AFF_BLIND))
	{
          baf.type = gsn_dirt;
	  baf.level = ch->level;
	  baf.location = APPLY_HITROLL;
	  baf.modifier = -4;
	  baf.duration = number_range(1,5);
	  baf.bitvector = AFF_BLIND;
	  affect_to_char( victim, &baf );
        }
	dam += dam * number_range( 1, 2 );
	return dam;
      }
      else if( diceroll > 95 )
      {
        act_color( L"$C$n kalbini söktü! AahHH!!$c",
             ch, NULL, victim, TO_VICT ,POS_RESTING,CLR_RED);
        act_color( L"$C$S kalbini söktün! Eminim acımıştır!$c",
             ch, NULL, victim, TO_CHAR ,POS_RESTING,CLR_RED);

	check_improve( ch, gsn_critical, TRUE, 5 );
	dam += dam * number_range( 2, 5 );
	return dam;
      }
  }

  return dam;
}

void do_shield( CHAR_DATA *ch, wchar_t *argument )
{
    CHAR_DATA *victim;
    int chance,ch_weapon,vict_shield;
    OBJ_DATA *shield,*axe;

    if ( IS_NPC(ch))
	return;

    if ( ( victim = ch->fighting ) == NULL )
    {
      send_to_char( L"Kimseyle dövüşmüyorsun.\n\r", ch );
	return;
    }

    if ((axe = get_wield_char(ch,FALSE)) == NULL)
    {
      send_to_char( L"Bir silah kuşanıyor olmalısın.\n\r",ch);
	return;
    }

    if ((chance = get_skill(ch,gsn_shield_cleave)) == 0)
    {
      send_to_char( L"Rakibinin kalkanını nasıl yaracağını bilmiyorsun.\n\r",ch);
	return;
    }

    if ( ( shield = get_shield_char( victim )) == NULL )
    {
      send_to_char( L"Rakibin kalkan kullanmıyor.\n\r", ch );
	return;
    }

    if ( check_material(shield,(wchar_t*)"platinum") || shield->pIndexData->limit != -1)
	return;

    if (axe->value[0] == WEAPON_AXE )
	chance = (int)((float)chance*1.2);
    else if (axe->value[0] != WEAPON_SWORD)
	{
    send_to_char( L"Silahın bir kılıç veya bir balta olmalı.\n\r",ch);
	 return;
	}

    /* find weapon skills */
    ch_weapon = get_weapon_skill(ch,get_weapon_sn(ch,FALSE));
    vict_shield = get_skill(ch,gsn_shield_block);
    /* modifiers */

    /* skill */
   chance = chance * ch_weapon / 200;
   chance = chance * 100 / vict_shield;

    /* dex vs. strength */
    chance += get_curr_stat(ch,STAT_DEX);
    chance -= 2 * get_curr_stat(victim,STAT_STR);

    /* level */
/*    chance += (ch->level - victim->level) * 2; */
    chance += ch->level - victim->level;
    chance += axe->level - shield->level;

    /* and now the attack */
    SET_BIT(ch->affected_by,AFF_WEAK_STUN);
    if (number_percent() < chance)
    {
    	WAIT_STATE( ch, skill_table[gsn_shield_cleave].beats );
      act( L"$S kalkanını ikiye böldün.",ch,NULL,victim,TO_CHAR);
    	act( L"$n senin kalkanını ikiye böldü.",ch,NULL,victim,TO_VICT);
    	act( L"$n $S kalkanını ikiye böldü.",ch,NULL,victim,TO_NOTVICT);
	check_improve(ch,gsn_shield_cleave,TRUE,1);
	extract_obj( get_shield_char(victim) );
    }
    else
    {
	WAIT_STATE(ch,skill_table[gsn_shield_cleave].beats);
  act( L"$S kalkanını yarmayı başaramadın.",ch,NULL,victim,TO_CHAR);
	act( L"$n senin kalkanını yarmaya çalıştı ama beceremedi.",ch,NULL,victim,TO_VICT);
	act( L"$n $S kalkanını yarmaya çalıştı ama beceremedi.",ch,NULL,victim,TO_NOTVICT);
	check_improve(ch,gsn_shield_cleave,FALSE,1);
    }
    return;
}

void do_weapon( CHAR_DATA *ch, wchar_t *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA *wield,*axe;
    int chance,ch_weapon,vict_weapon;

    if ( IS_NPC(ch))
	return;

    if ( ( victim = ch->fighting ) == NULL )
    {
      send_to_char( L"Kimseyle dövüşmüyorsun.\n\r", ch );
	return;
    }

    if ( (axe = get_wield_char(ch,FALSE)) == NULL)
    {
      send_to_char( L"Bir silah kuşanmış olmalısın.\n\r",ch);
	return;
    }

    if ((chance = get_skill(ch,gsn_weapon_cleave)) == 0)
    {
      send_to_char( L"Rakibin silahını nasıl yarabileceğini bilmiyorsun.\n\r",ch);
	return;
    }

    if ( (wield = get_wield_char( victim,FALSE )) == NULL )
    {
      send_to_char( L"Rakibin silah kullanmıyor.\n\r", ch );
	return;
    }

    if ( check_material(wield,(wchar_t*)"platinum") || wield->pIndexData->limit != -1 )
	return;


    if (axe->value[0] == WEAPON_AXE )
	chance = (int)((float)chance*1.2);
    else if (axe->value[0] != WEAPON_SWORD)
	{
    send_to_char( L"Silahın bir kılıç veya bir balta olmalı.\n\r",ch);
	 return;
	}

    /* find weapon skills */
    ch_weapon = get_weapon_skill(ch,get_weapon_sn(ch,FALSE));
    vict_weapon = get_weapon_skill(victim,get_weapon_sn(victim,FALSE));
    /* modifiers */

    /* skill */
    chance = chance * ch_weapon / 200;
    chance = chance * 100 / vict_weapon;

    /* dex vs. strength */
    chance += get_curr_stat(ch,STAT_DEX) + get_curr_stat(ch,STAT_STR);
    chance -= get_curr_stat(victim,STAT_STR) +
			2 * get_curr_stat(victim,STAT_DEX);

    chance += ch->level - victim->level;
    chance += axe->level - wield->level;

    /* and now the attack */
    SET_BIT(ch->affected_by,AFF_WEAK_STUN);
    if (number_percent() < chance)
    {
    	WAIT_STATE( ch, skill_table[gsn_weapon_cleave].beats );
      act( L"$S silahını ikiye böldün.",ch,NULL,victim,TO_CHAR);
    	act( L"$n senin silahını ikiye böldü.",ch,NULL,victim,TO_VICT);
    	act( L"$n $S silahını ikiye böldü.",ch,NULL,victim,TO_NOTVICT);
	check_improve(ch,gsn_weapon_cleave,TRUE,1);
	extract_obj( get_wield_char(victim,FALSE) );
    }
    else
    {
	WAIT_STATE(ch,skill_table[gsn_weapon_cleave].beats);
  act( L"$S silahını yarmayı başaramadın.",ch,NULL,victim,TO_CHAR);
	act( L"$n senin silahını yarmayı denedi ama başaramadı.",ch,NULL,victim,TO_VICT);
	act( L"$n $S silahını yarmaya çalıştı ama başaramadı.",ch,NULL,victim,TO_NOTVICT);
	check_improve(ch,gsn_weapon_cleave,FALSE,1);
    }
    return;
}


void do_tail( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int chance, wait;
    wchar_t buf[MAX_STRING_LENGTH];
    bool FightingCheck;
    int damage_tail;

    if ( MOUNTED(ch) )
    {
      send_to_char( L"Biniciyken kuyruk kullanamazsın!\n\r", ch);
        return;
    }

    if (ch->fighting != NULL)
	FightingCheck = TRUE;
    else
	FightingCheck = FALSE;

    one_argument(argument,arg);

    if ( (chance = get_skill(ch,gsn_tail)) == 0
    ||	 (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_TAIL))
    ||	 (!IS_NPC(ch)
    &&    RACE(ch) != skill_table[gsn_tail].race
    &&	  ch->level < skill_table[gsn_tail].skill_level[ch->iclass]))
    {
	send_to_char( L"Hı?\n\r",ch);
	return;
    }

    if (arg[0] == '\0')
    {
	victim = ch->fighting;
	if (victim == NULL)
	{
	    send_to_char( L"Fakat kimseyle dövüşmüyorsun!\n\r",ch);
	    return;
	}
    }

    else if ((victim = get_char_room(ch,arg)) == NULL)
    {
	send_to_char( L"O burada değil.\n\r",ch);
	return;
    }
/*
    if (victim->position < POS_FIGHTING)
    {
	act( L"You'll have to let $M get back up first.",ch,NULL,victim,TO_CHAR);
	return;
    }
*/
    if (victim == ch)
    {
      send_to_char( L"Kendi kuyruğunla kendine vurmaya çalıştın ama beceremedin.\n\r",ch);
	return;
    }

    if (is_safe(ch,victim))
	return;

    if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)
    {
      act( L"Fakat $N senin arkadaşın!",ch,NULL,victim,TO_CHAR);
	return;
    }

    if (is_affected(victim, gsn_protective_shield))
     {
       act_color( L"$CKuyruğun $S çevresinde kaydı.$c", ch, NULL, victim,
       	TO_CHAR,POS_FIGHTING,CLR_YELLOW);
       act_color( L"$C$s kuyruğu koruyucu kalkanının çevresinde kaydı.$c", ch, NULL,
       	victim,	TO_VICT,POS_FIGHTING,CLR_YELLOW);
       act_color( L"$C$s kuyruğu $S çevresinde kaydı.$c",ch,NULL,victim,
	TO_NOTVICT,POS_FIGHTING,CLR_YELLOW);
	return;
     }

    /* modifiers */

    /* size  and weight */
    chance -= ch->carry_weight / 20;
    chance += victim->carry_weight / 25;

    if (ch->size < victim->size)
	chance += (ch->size - victim->size) * 25;
    else
	chance += (ch->size - victim->size) * 10;


    /* stats */
    chance += get_curr_stat(ch,STAT_STR) +  get_curr_stat(ch,STAT_DEX);
    chance -= get_curr_stat(victim,STAT_DEX) * 2;

    if (IS_AFFECTED(ch,AFF_FLYING))
	chance -= 10;

    /* speed */
    if (IS_SET(ch->off_flags,OFF_FAST))
	chance += 20;
    if (IS_SET(victim->off_flags,OFF_FAST))
	chance -= 30;

    /* level */
    chance += (ch->level - victim->level) * 2;

    /* now the attack */
    if (number_percent() < (chance / 4) )
    {

      act( L"$n güçlü kuyruğuyla seni yere çaldı!",
    		ch,NULL,victim,TO_VICT);
    	act( L"Güçlü kuyruğunla $M yere çaldın!",
    		ch,NULL,victim,TO_CHAR);
    	act( L"$n güçlü kuyruğuyla $M yere çaldı.",
		ch,NULL,victim,TO_NOTVICT);
	check_improve(ch,gsn_tail,TRUE,1);

	wait = 3;

	switch(number_range(0,1)) {
	case 0: wait = 1; break;
	case 1: wait = 2; break;
	case 2: wait = 4; break;
	case 3: wait = 3; break;
	}

	WAIT_STATE(victim, wait * PULSE_VIOLENCE);
	WAIT_STATE(ch,skill_table[gsn_tail].beats);
	victim->position = POS_RESTING;
	damage_tail = ch->damroll +
			( 2 * number_range(4,4 + 10* ch->size + chance/10) );
	damage(ch,victim,damage_tail,gsn_tail, DAM_BASH, TRUE);

    }
    else
    {
	damage(ch,victim,0,gsn_tail,DAM_BASH, TRUE);
  act( L"Pozisyonunu kaybederek yere düşüyorsun!",
	    ch,NULL,victim,TO_CHAR);
	act( L"$n pozisyonunu kaybederek yere düşüyor!.",
	    ch,NULL,victim,TO_NOTVICT);
	act( L"$s kuyruk hamlesini atlatarak yere düşmesini sağladın.",
	    ch,NULL,victim,TO_VICT);
	check_improve(ch,gsn_tail,FALSE,1);
	ch->position = POS_RESTING;
	WAIT_STATE(ch,skill_table[gsn_tail].beats * 3/2);
    }
    if (!(IS_NPC(victim)) && !(IS_NPC(ch)) && victim->position > POS_STUNNED
		&& !FightingCheck)
      {
	if (!can_see(victim, ch))
	  do_yell(victim, (wchar_t*)"İmdat! Biri bana vurdu!");
	else
	  {
      swprintf( buf, MAX_STRING_LENGTH-1, L"İmdat! %s bana kuyruğuyla vurmaya çalıştı!",
		(is_affected(ch,gsn_doppelganger) && !IS_IMMORTAL(victim)) ?
		ch->doppel->name : ch->name);
	    do_yell(victim, buf);
	  }
      }
}

void do_concentrate( CHAR_DATA *ch, wchar_t *argument)
{
    int chance;

    if ( MOUNTED(ch) )
    {
      send_to_char( L"Biniciyken konsantre olamazsın!\n\r", ch);
        return;
    }

    if ((chance = get_skill(ch,gsn_concentrate)) == 0
    ||  (!IS_NPC(ch)
    &&   ch->level < skill_table[gsn_concentrate].skill_level[ch->iclass]))
    {
      send_to_char( L"Olanlar üzerine konsantre olmaya çalışıyorsun.\n\r",ch);
	return;
    }

    if (is_affected(ch,gsn_concentrate))
    {
      send_to_char( L"Zaten dövüşe yoğunlaşmış durumdasın.\n\r",ch);
	return;
    }

    if (ch->mana < 50)
    {
      send_to_char( L"Yeterli enerjiyi toplayamıyorsun.\n\r",ch);
	return;
    }

    /* fighting */
    if (ch->fighting)
    {
      send_to_char( L"Devam eden dövüşe mi?\n\r",ch);
	return;
    }


    if (number_percent() < chance)
    {
	AFFECT_DATA af;

	WAIT_STATE(ch,2 * PULSE_VIOLENCE);
	ch->mana -= 50;
	ch->move /= 2;

	do_sit(ch,(wchar_t*)"");
  send_to_char( L"Oturarak rahatlıyor, bir sonraki dövüşe konsantre oluyorsun.!\n\r",ch);
	act_color( L"$C$n bir sonraki dövüşe konsantre oluyor.$c",ch,NULL,NULL,TO_ROOM,
	POS_FIGHTING,CLR_RED);
	check_improve(ch,gsn_concentrate,TRUE,2);

	af.where	= TO_AFFECTS;
	af.type		= gsn_concentrate;
	af.level	= ch->level;
	af.duration	= number_fuzzy(ch->level / 8);
	af.modifier	= UMAX(1,ch->level/8);
	af.bitvector 	= 0;

	af.location	= APPLY_HITROLL;
	affect_to_char(ch,&af);

	af.location	= APPLY_DAMROLL;
	affect_to_char(ch,&af);

	af.modifier	= UMAX(1,ch->level/10);
	af.location	= APPLY_AC;
	affect_to_char(ch,&af);
    }

    else
    {
      send_to_char( L"Sonraki dövüşe konsantre olmaya çalışıyor ama başaramıyorsun.\n\r",ch);
	check_improve(ch,gsn_concentrate,FALSE,2);
    }
}


void do_bandage( CHAR_DATA *ch, wchar_t *argument)
{
    int heal;

    if (ch_skill_nok(ch,gsn_bandage) )
      return;

    if (!cabal_ok(ch,gsn_bandage))
      return;

    if (is_affected(ch, gsn_bandage))
    {
      send_to_char( L"Zaten sargı kullanıyorsun.\n\r",ch);
	return;
    }


    if (IS_NPC(ch) || number_percent() < get_skill(ch,gsn_bandage) )
    {
	AFFECT_DATA af;

	WAIT_STATE(ch, PULSE_VIOLENCE);


  send_to_char( L"Omzuna sargı yerleştiriyorsun!\n\r",ch);
	act( L"$n omzuna bir sargı yerleştiriyor.",ch,NULL,NULL,TO_ROOM);
	check_improve(ch,gsn_bandage,TRUE,2);

        heal = dice(4, 8) + ch->level / 2;
        ch->hit = UMIN( ch->hit + heal, ch->max_hit );
        update_pos( ch );
        send_to_char( L"Kendini daha iyi hissediyorsun!\n\r", ch);

	af.where	= TO_AFFECTS;
	af.type		= gsn_bandage;
	af.level	= ch->level;
	af.duration	= ch->level / 10;
	af.modifier	= UMIN(15,ch->level/2);
	af.bitvector 	= AFF_REGENERATION;
	af.location	= 0;
	affect_to_char(ch,&af);

    }

    else
    {
     WAIT_STATE(ch, PULSE_VIOLENCE);

     send_to_char( L"Omzuna sargı yerleştirmeyi beceremedin.\n\r", ch);
     check_improve(ch,gsn_bandage,FALSE,2);
    }
}


void do_katana(CHAR_DATA *ch, wchar_t *argument)
{
  OBJ_DATA *katana;
  AFFECT_DATA af;
  OBJ_DATA *part;
  wchar_t arg[MAX_INPUT_LENGTH];
  wchar_t buf[MAX_STRING_LENGTH];

  one_argument( argument, arg );

  if (IS_NPC(ch) || ch->level < skill_table[gsn_katana].skill_level[ch->iclass] )
    {
      send_to_char( L"Hı?\n\r", ch );
      return;
    }

  if (is_affected(ch, gsn_katana))
    {
      send_to_char( L"Zaten bir katanan var!\n\r", ch);
      return;
    }

  if (ch->mana < 300)
    {
      send_to_char( L"Kendini bir katanaya konsantre olamayacak kadar güçsüz hissediyorsun.\n\r", ch);
      return;
    }

  if ( arg[0] == '\0' )
    {
      send_to_char( L"Ne ile katana yapacaksın?\n\r", ch );
      return;
    }

  if ( ( part = get_obj_carry( ch, arg ) ) == NULL )
    {
      send_to_char( L"Demire ihtiyacın var.\n\r", ch );
      return;
    }

  if (part->pIndexData->vnum != OBJ_VNUM_CHUNK_IRON)
    {
      send_to_char( L"Doğru hammaddeye sahip değilsin.\n\r", ch );
      return;
    }

    if ( number_percent( ) < (get_skill( ch, gsn_katana )/3)*2 )
    {
      send_to_char( L"Başaramadın ve onu yokettin.\n\r", ch);
      extract_obj(part);
      return;
    }

    WAIT_STATE(ch, skill_table[gsn_katana].beats);

   if (!IS_NPC(ch) && number_percent() < get_skill(ch,gsn_katana))
    {
      af.where  = TO_AFFECTS;
      af.type	= gsn_katana;
      af.level	= ch->level;
      af.duration	= ch->level;
      af.modifier	= 0;
      af.bitvector 	= 0;
      af.location	= 0;
      affect_to_char(ch,&af);

      katana = create_object( get_obj_index( OBJ_VNUM_KATANA_SWORD),ch->level );
      katana->cost  = 0;
      katana->level = ch->level;
      ch->mana -= 300;

      af.where	= TO_OBJECT;
      af.type 	= gsn_katana;
      af.level	= ch->level;
      af.duration	= -1;
      af.location	= APPLY_DAMROLL;
      af.modifier	= ch->level / 10;
      af.bitvector	= 0;
      affect_to_obj( katana, &af );

      af.location	= APPLY_HITROLL;
      affect_to_obj( katana, &af );

      katana->value[2] = ch->level / 10;

      swprintf( buf, MAX_STRING_LENGTH-1,katana->pIndexData->extra_descr->description,ch->name );
      katana->extra_descr = new_extra_descr();
      katana->extra_descr->keyword =
		str_dup( katana->pIndexData->extra_descr->keyword );
      katana->extra_descr->description = str_dup( buf );
      katana->extra_descr->next = NULL;

      obj_to_char(katana, ch);
      check_improve(ch, gsn_katana, TRUE, 1);

      act( L"$p ile bir katana yaptın!",ch,part,NULL,TO_CHAR);
      act( L"$n $p ile bir katana yaptı!",ch,part,NULL,TO_ROOM);

      extract_obj(part);
      return;
    }
    else
    {
      send_to_char( L"Onu yokettin.\n\r", ch);
      extract_obj(part);
      ch->mana -= 150;
      check_improve(ch, gsn_katana, FALSE, 1);
    }
}


void do_crush( CHAR_DATA *ch, wchar_t *argument )
{
    CHAR_DATA *victim;
    int chance=100, wait=0;
    int damage_crush;

    if ( MOUNTED(ch) )
        return;

    if (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_CRUSH))
	return;


    if ( (victim = ch->fighting)== NULL)
	    return;


    if (victim->position < POS_FIGHTING)
	return;

    if (is_safe(ch,victim))
	return;

    if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)
	return;

    if (is_affected(victim, gsn_protective_shield))
     {
       act_color( L"$CEzici saldırın $S çevresinde kayıyor.$c", ch, NULL, victim,
       	TO_CHAR,POS_FIGHTING,CLR_YELLOW);
       act_color( L"$C$s ezici saldırısı koruyucu kalkanında kayıyor.$c", ch, NULL,
       	victim,	TO_VICT,POS_FIGHTING,CLR_YELLOW);
       act_color( L"$C$s ezici saldırısı $S çevresinde kayıyor.$c",ch,NULL,victim,
	TO_NOTVICT,POS_FIGHTING,CLR_YELLOW);
	return;
     }

    /* modifiers */

    /* size  and weight */
    chance += ch->carry_weight / 25;
    chance -= victim->carry_weight / 20;

    if (ch->size < victim->size)
	chance += (ch->size - victim->size) * 25;
    else
	chance += (ch->size - victim->size) * 10;


    /* stats */
    chance += get_curr_stat(ch,STAT_STR);
    chance -= get_curr_stat(victim,STAT_DEX) * 4/3;

    if (IS_AFFECTED(ch,AFF_FLYING))
	chance -= 10;

    /* speed */
    if (IS_SET(ch->off_flags,OFF_FAST))
	chance += 10;
    if (IS_SET(victim->off_flags,OFF_FAST))
	chance -= 20;

    /* level */
    chance += (ch->level - victim->level) * 2;

    /* now the attack */
    if (number_percent() < chance)
    {

      act( L"$n seni büyük bir güçle eziyor!",
    		ch,NULL,victim,TO_VICT);
    	act( L"$M eziyorsun!",ch,NULL,victim,TO_CHAR);
    	act( L"$n $M büyük bir güçle eziyor.",
		ch,NULL,victim,TO_NOTVICT);

	wait = 3;

	switch(number_range(0,1)) {
	case 0: wait = 1; break;
	case 1: wait = 2; break;
	case 2: wait = 4; break;
	case 3: wait = 3; break;
	}

	WAIT_STATE(victim, wait * PULSE_VIOLENCE);
	WAIT_STATE(ch,skill_table[gsn_crush].beats);
	victim->position = POS_RESTING;
	damage_crush = ch->damroll + number_range(4,4 + 4* ch->size + chance/2);
	if (ch->level < 5) damage_crush = UMIN(ch->level, damage_crush);
	damage(ch,victim,damage_crush,gsn_crush, DAM_BASH, TRUE);

    }
    else
    {
	damage(ch,victim,0,gsn_crush,DAM_BASH, TRUE);
  act( L"Yüzüstü yere çakıldın!",
	    ch,NULL,victim,TO_CHAR);
	act( L"$n yüzüstü yere çakıldı.",
	    ch,NULL,victim,TO_NOTVICT);
	act( L"$s ezici saldırısından kurtulup yüzüstü yere çakılmasını sağladın.",
	    ch,NULL,victim,TO_VICT);
	ch->position = POS_RESTING;
	WAIT_STATE(ch,skill_table[gsn_crush].beats * 3/2);
    }

}


void do_sense(CHAR_DATA *ch, wchar_t *argument)
{
  if (IS_NPC(ch) ||
        ch->level < skill_table[gsn_sense_life].skill_level[ch->iclass] )
    {
      send_to_char( L"Hı?\n\r", ch );
      return;
    }

  if (is_affected(ch,gsn_sense_life))
    {
      send_to_char( L"Zaten yaşam formlarını sezebiliyorsun.\n\r",ch);
      return;
    }

  if (ch->mana < 20)
    {
      send_to_char( L"Yeterince konsantre olamadın.\n\r",ch);
      return;
    }

  WAIT_STATE( ch, skill_table[gsn_sense_life].beats );

  if (!IS_NPC(ch) && number_percent() < ch->pcdata->learned[gsn_sense_life])
    {
      AFFECT_DATA af;

      af.where  = TO_DETECTS;
      af.type 	= gsn_sense_life;
      af.level 	= ch->level;
      af.duration = ch->level;
      af.location = APPLY_NONE;
      af.modifier = 0;
      af.bitvector = DETECT_LIFE;
      affect_to_char(ch, &af);

      ch->mana -= 20;

      act( L"Odadaki yaşam formlarını sezmeye başladın!",ch,NULL,NULL,TO_CHAR);
      act( L"$n daha duyarlı görünüyor.",ch,NULL,NULL,TO_ROOM);
      check_improve(ch,gsn_sense_life,TRUE,1);
    }
  else
    {
      ch->mana -= 10;

      send_to_char( L"Başaramadın.\n\r" ,ch);
      check_improve(ch,gsn_sense_life,FALSE,1);
    }

}


void do_poison_smoke( CHAR_DATA *ch, wchar_t *argument)
{
  CHAR_DATA *tmp_vict;
  wchar_t buf[MAX_STRING_LENGTH];

  if (IS_NPC(ch)) return;
  if (ch_skill_nok(ch,gsn_poison_smoke)) return;

  if (ch->mana < skill_table[gsn_poison_smoke].min_mana)
    {
      send_to_char( L"Yeterli enerjiyi toplayamadın.\n\r",ch);
	return;
    }
  ch->mana -= skill_table[gsn_poison_smoke].min_mana;
  WAIT_STATE(ch,skill_table[gsn_poison_smoke].beats);

  if (number_percent() > get_skill(ch,gsn_poison_smoke) )
   {
	send_to_char( L"Başaramadın.\n\r",ch);
	check_improve(ch,gsn_poison_smoke,FALSE,1);
	return;
   }

   send_to_char( L"Bir zehir bulutu odayı dolduruyor.\n\r",ch);
   act( L"Bir zehir bulutu odayı dolduruyor.",ch,NULL,NULL,TO_ROOM);

  check_improve(ch,gsn_poison_smoke,TRUE,1);

  for (tmp_vict=ch->in_room->people;tmp_vict!=NULL;
       tmp_vict=tmp_vict->next_in_room)
    if (!is_safe_spell(ch,tmp_vict,TRUE))
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
		    (is_affected(ch,gsn_doppelganger)&&!IS_IMMORTAL(tmp_vict))?
		     ch->doppel->name : ch->name);
	         do_yell(tmp_vict,buf);
	      }
	  }

	spell_poison(gsn_poison,ch->level,ch,tmp_vict, TARGET_CHAR);
	if (tmp_vict != ch)
	  multi_hit(tmp_vict,ch,TYPE_UNDEFINED);

      }

}

void do_blindness_dust( CHAR_DATA *ch, wchar_t *argument)
{
  CHAR_DATA *tmp_vict;
  wchar_t buf[MAX_STRING_LENGTH];

  if (IS_NPC(ch)) return;
  if (ch_skill_nok(ch,gsn_blindness_dust)) return;
  if (ch->mana < skill_table[gsn_blindness_dust].min_mana)
    {
      send_to_char( L"Yeterli enerjiyi toplayamadın.\n\r",ch);
	return;
    }

  ch->mana -= skill_table[gsn_blindness_dust].min_mana;
  WAIT_STATE(ch,skill_table[gsn_blindness_dust].beats);

  if (number_percent() > get_skill(ch,gsn_blindness_dust) )
    {
	send_to_char( L"Başaramadın.\n\r",ch);
	check_improve(ch,gsn_blindness_dust,FALSE,1);
	return;
    }

    send_to_char( L"Bir toz bulutu odayı dolduruyor.\n\r",ch);
    act( L"Bir toz bulutu odayı dolduruyor.",ch,NULL,NULL,TO_ROOM);

  check_improve(ch,gsn_blindness_dust,TRUE,1);

  for (tmp_vict=ch->in_room->people;tmp_vict!=NULL;
       tmp_vict=tmp_vict->next_in_room)
    if (!is_safe_spell(ch,tmp_vict,TRUE))
      {
	if (!IS_NPC(ch) && tmp_vict != ch &&
	    ch->fighting != tmp_vict && tmp_vict->fighting != ch &&
	    (IS_SET(tmp_vict->affected_by,AFF_CHARM) || !IS_NPC(tmp_vict)))
	  {
	    if (!can_see(tmp_vict, ch))
      do_yell(tmp_vict,(wchar_t*)"İmdat! Biri bana saldırıyor!");
	    else
	      {
          swprintf( buf, MAX_STRING_LENGTH-1, L"Geber %s, seni büyücü köpek!",
		    (is_affected(ch,gsn_doppelganger)&&!IS_IMMORTAL(tmp_vict))?
		     ch->doppel->name : ch->name);
	         do_yell(tmp_vict,buf);
	      }
	  }

	spell_blindness(gsn_blindness,ch->level,ch,tmp_vict, TARGET_CHAR);
	if (tmp_vict != ch)
	  multi_hit(tmp_vict,ch,TYPE_UNDEFINED);
      }

}


void do_lash( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int chance;
    wchar_t buf[MAX_STRING_LENGTH];
    bool FightingCheck;
    int damage_lash;

    if (ch->fighting != NULL)
	FightingCheck = TRUE;
    else
	FightingCheck = FALSE;

    argument = one_argument(argument,arg);

    chance = get_skill(ch, gsn_lash);

    if (ch_skill_nok(ch, gsn_lash)) return;

    if (arg[0] == '\0')
    {
	victim = ch->fighting;
	if (victim == NULL)
	{
	    send_to_char( L"Fakat kimseyle dövüşmüyorsun!\n\r",ch);
	    return;
	}
    }

    else if ((victim = get_char_room(ch,arg)) == NULL)
    {
	send_to_char( L"O burada değil.\n\r",ch);
	return;
    }

    if ( get_weapon_char(ch, WEAPON_WHIP) == NULL)
    {
      send_to_char( L"Bir kamçıya ihtiyacın var.\n\r",ch);
	return;
    }

    if (victim == ch)
    {
      send_to_char( L"Kendi vücuduna zarar verecek kadar aptal mısın?\n\r",ch);
	return;
    }

    if (is_safe(ch,victim))
	return;

    if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)
    {
      act( L"Fakat $N senin arkadaşın!",ch,NULL,victim,TO_CHAR);
	return;
    }

    /* modifiers */

    /* stats */
    chance += get_curr_stat(ch,STAT_STR) / 2;
    chance -= get_curr_stat(victim,STAT_DEX) * 2;

    if (IS_AFFECTED(ch,AFF_FLYING))
	chance += 20;

    /* speed */
    if (IS_AFFECTED(ch,AFF_HASTE))
	chance += 20;
    if (IS_AFFECTED(victim,AFF_HASTE))
	chance -= 20;

    if (IS_AFFECTED(ch,AFF_SLOW))
	chance -= 40;
    if (IS_AFFECTED(victim,AFF_SLOW))
	chance += 20;

    if (MOUNTED(ch))
	chance -= 20;
    if (MOUNTED(victim))
	chance += 40;

    /* level */
    chance += (ch->level - victim->level) * 2;

    /* now the attack */
    if (number_percent() < chance)
    {
	check_improve(ch,gsn_lash,TRUE,1);

	WAIT_STATE(ch, PULSE_VIOLENCE);
	WAIT_STATE(victim,skill_table[gsn_lash].beats);
	damage_lash = number_range(4, 4 + chance/10);
	damage(ch,victim,damage_lash,gsn_lash, DAM_BASH, TRUE);
    }
    else
    {
	damage(ch,victim,0,gsn_lash,DAM_BASH, TRUE);
  act( L"$M kamçılamayı başaramadın!", ch,NULL,victim,TO_CHAR);
	act( L"$n $M kamçılamayı denedi ama başaramadı.", ch,NULL,victim,TO_NOTVICT);
	act( L"$s kamçısından kurtuluyorsun!", ch,NULL,victim,TO_VICT);
	check_improve(ch,gsn_lash,FALSE,1);
	WAIT_STATE(ch, PULSE_VIOLENCE);
    }

    if (!(IS_NPC(victim)) && !(IS_NPC(ch)) && victim->position > POS_STUNNED
		&& !FightingCheck)
      {
	if (!can_see(victim, ch))
	  do_yell(victim, (wchar_t*)"İmdat! Biri beni kamçılıyor!");
	else
	  {
      swprintf( buf, MAX_STRING_LENGTH-1, L"İmdat! %s beni kamçılıyor!",
		(is_affected(ch,gsn_doppelganger) && !IS_IMMORTAL(victim)) ?
		ch->doppel->name : ch->name);
	    do_yell(victim, buf);
	  }
      }
}


void do_claw( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int chance;
    int damage_claw;

  if (ch_skill_nok(ch,gsn_claw) )
      return;

  if (!cabal_ok(ch,gsn_claw))
    return;

    argument = one_argument(argument,arg);

    if (arg[0] == '\0')
    {
	victim = ch->fighting;
	if (victim == NULL)
	{
	    send_to_char( L"Fakat kimseyle dövüşmüyorsun!\n\r",ch);
	    return;
	}
    }

    else if ((victim = get_char_room(ch,arg)) == NULL)
    {
	send_to_char( L"O burada değil.\n\r",ch);
	return;
    }

    if (victim == ch)
    {
      send_to_char( L"Kendini pençelemek istemezsin.\n\r",ch);
	return;
    }

    if (is_safe(ch,victim))
	return;

    if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)
    {
      act( L"Fakat $N senin arkadaşın!",ch,NULL,victim,TO_CHAR);
	return;
    }

    if (ch->mana < 50)
    {
      send_to_char( L"Yeterli enerjiyi toplayamıyorsun.\n\r",ch);
	return;
    }


    /* size  and weight */
    chance = get_skill(ch, gsn_claw);

    if (CAN_DETECT(ch, ADET_LION))
	chance +=25;

    /* stats */
    chance += get_curr_stat(ch,STAT_STR) + get_curr_stat(ch, STAT_DEX);
    chance -= get_curr_stat(victim,STAT_DEX) * 2;

    if (IS_AFFECTED(ch,AFF_FLYING))
	chance -= 10;

    /* speed */
    if (IS_AFFECTED(ch,AFF_HASTE))
	chance += 10;
    if (IS_SET(victim->off_flags,OFF_FAST)
	 || IS_AFFECTED(victim, AFF_HASTE))
	chance -= 20;

    /* level */
    chance += (ch->level - victim->level) * 2;

    /* now the attack */
    if (number_percent() < chance)
    {
        ch->mana -= 50;
	check_improve(ch,gsn_claw,TRUE,1);
	victim->position = POS_RESTING;

        damage_claw = ch->size * 10;
	if (CAN_DETECT(ch, ADET_LION))
	{
		WAIT_STATE(ch, skill_table[gsn_claw].beats / 2);
		damage_claw += dice(ch->level, 12) + ch->damroll;
	}
	else
	{
		WAIT_STATE(ch, skill_table[gsn_claw].beats);
		damage_claw += dice(ch->level, 24) + ch->damroll;
	}

	damage(ch,victim,damage_claw,gsn_claw, DAM_BASH, TRUE);
    }
    else
    {
        ch->mana -= 25;
	damage(ch,victim,0,gsn_claw,DAM_BASH, TRUE);
	check_improve(ch,gsn_claw,FALSE,1);
	ch->position = POS_RESTING;
	WAIT_STATE(ch,skill_table[gsn_claw].beats /2);
    }
}
