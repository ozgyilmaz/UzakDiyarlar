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

DECLARE_DO_FUN(do_scan2);
/* command procedures needed */
DECLARE_DO_FUN(do_look		);
DECLARE_DO_FUN(do_yell		);
DECLARE_DO_FUN(do_say		);
DECLARE_DO_FUN(do_murder	);
DECLARE_DO_FUN(do_kill		);

void	raw_kill(CHAR_DATA *victim);
int	find_door	args( ( CHAR_DATA *ch, char *arg ) );
int	check_exit	args( ( char *argument ) );

extern char *target_name;


ROOM_INDEX_DATA * check_place( CHAR_DATA *ch, char *argument ) 
{
 EXIT_DATA *pExit;
 ROOM_INDEX_DATA *dest_room;
 int number,door;
 int range = (ch->level / 10) + 1;
 char arg[MAX_INPUT_LENGTH];

 number = number_argument(argument,arg);
 if ( (door = check_exit( arg )) == -1) return NULL;

 dest_room = ch->in_room;
 while (number > 0)
 {
  number--;
  if ( --range < 1 ) return NULL;
  if ( (pExit = dest_room->exit[door]) == NULL
      || (dest_room = pExit->u1.to_room) == NULL
      || IS_SET(pExit->exit_info,EX_CLOSED) )
   break;
  if ( number < 1)    return dest_room;
 }
 return NULL;
}
	
void spell_portal( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim;
    OBJ_DATA *portal, *stone;

        if ( ( victim = get_char_world( ch, target_name ) ) == NULL
    ||   victim == ch
    ||   victim->in_room == NULL
    ||   !can_see_room(ch,victim->in_room)
    ||   IS_SET(victim->in_room->room_flags, ROOM_SAFE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_PRIVATE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_SOLITARY)
    ||   IS_SET(victim->in_room->room_flags, ROOM_NOSUMMON)
    ||   IS_SET(ch->in_room->room_flags, ROOM_NOSUMMON)
    ||   IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL)
    ||   victim->level >= level + 3
    ||   (!IS_NPC(victim) && victim->level >= LEVEL_HERO)  /* NOT trust */
    ||   (IS_NPC(victim) && is_safe_nomessage(ch, victim) && IS_SET(victim->imm_flags,IMM_SUMMON))
    ||   (IS_NPC(victim) && saves_spell( level, victim,DAM_NONE) ) )
    {
        send_to_char( "You failed.\n\r", ch );
        return;
    }   

    stone = get_hold_char(ch);
    if (!IS_IMMORTAL(ch) 
    &&  (stone == NULL || stone->item_type != ITEM_WARP_STONE))
    {
	send_to_char("You lack the proper component for this spell.\n\r",ch);
	return;
    }

    if (stone != NULL && stone->item_type == ITEM_WARP_STONE)
    {
     	act("You draw upon the power of $p.",ch,stone,NULL,TO_CHAR);
     	act("It flares brightly and vanishes!",ch,stone,NULL,TO_CHAR);
     	extract_obj(stone);
    }

    portal = create_object(get_obj_index(OBJ_VNUM_PORTAL),0);
    portal->timer = 2 + level / 25; 
    portal->value[3] = victim->in_room->vnum;

    obj_to_room(portal,ch->in_room);

    act("$p rises up from the ground.",ch,portal,NULL,TO_ROOM);
    act("$p rises up before you.",ch,portal,NULL,TO_CHAR);
}

void spell_nexus( int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    CHAR_DATA *victim;
    OBJ_DATA *portal, *stone;
    ROOM_INDEX_DATA *to_room, *from_room;

    from_room = ch->in_room;
 
        if ( ( victim = get_char_world( ch, target_name ) ) == NULL
    ||   victim == ch
    ||   (to_room = victim->in_room) == NULL
    ||   !can_see_room(ch,to_room) || !can_see_room(ch,from_room)
    ||   IS_SET(to_room->room_flags, ROOM_SAFE)
    ||	 IS_SET(from_room->room_flags,ROOM_SAFE)
    ||   IS_SET(to_room->room_flags, ROOM_PRIVATE)
    ||   IS_SET(to_room->room_flags, ROOM_SOLITARY)
    ||   IS_SET(to_room->room_flags, ROOM_NOSUMMON)
    ||   victim->level >= level + 3
    ||   (!IS_NPC(victim) && victim->level >= LEVEL_HERO)  /* NOT trust */
    ||   (IS_NPC(victim) && is_safe_nomessage(ch, victim) && IS_SET(victim->imm_flags,IMM_SUMMON))
    ||   (IS_NPC(victim) && saves_spell( level, victim,DAM_NONE) ) )
    {
        send_to_char( "You failed.\n\r", ch );
        return;
    }   
 
    stone = get_hold_char(ch);
    if (!IS_IMMORTAL(ch)
    &&  (stone == NULL || stone->item_type != ITEM_WARP_STONE))
    {
        send_to_char("You lack the proper component for this spell.\n\r",ch);
        return;
    }
 
    if (stone != NULL && stone->item_type == ITEM_WARP_STONE)
    {
        act("You draw upon the power of $p.",ch,stone,NULL,TO_CHAR);
        act("It flares brightly and vanishes!",ch,stone,NULL,TO_CHAR);
        extract_obj(stone);
    }

    /* portal one */ 
    portal = create_object(get_obj_index(OBJ_VNUM_PORTAL),0);
    portal->timer = 1 + level / 10;
    portal->value[3] = to_room->vnum;
 
    obj_to_room(portal,from_room);
 
    act("$p rises up from the ground.",ch,portal,NULL,TO_ROOM);
    act("$p rises up before you.",ch,portal,NULL,TO_CHAR);

    /* no second portal if rooms are the same */
    if (to_room == from_room)
	return;

    /* portal two */
    portal = create_object(get_obj_index(OBJ_VNUM_PORTAL),0);
    portal->timer = 1 + level/10;
    portal->value[3] = from_room->vnum;

    obj_to_room(portal,to_room);

    if (to_room->people != NULL)
    {
	act("$p rises up from the ground.",to_room->people,portal,NULL,TO_ROOM);
	act("$p rises up from the ground.",to_room->people,portal,NULL,TO_CHAR);
    }
}


void spell_disintegrate( int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  CHAR_DATA *tmp_ch;
  OBJ_DATA *obj;
  OBJ_DATA *obj_next;
  int i,dam=0;
  OBJ_DATA *tattoo; 
  

    if (saves_spell(level,victim,DAM_MENTAL) || number_bits(1) == 0) 
	{
	 dam = dice( level , 24 ) ;
	 damage(ch, victim , dam , sn, DAM_MENTAL, TRUE);
	 return;
	}

    act_color( "$N's thin light ray $C###DISINTEGRATES###$c you!", 
		victim, NULL, ch, TO_CHAR, POS_RESTING, CLR_RED );
    act_color( "$n's thin light ray $C###DISINTEGRATES###$c $N!", 
		ch, NULL, victim, TO_NOTVICT, POS_RESTING, CLR_RED );
    act_color( "Your thin light ray $C###DISINTEGRATES###$c $N!", 
		ch, NULL, victim, TO_CHAR, POS_RESTING, CLR_RED );
    send_to_char( "You have been KILLED!\n\r", victim );

    act("$N does not exist anymore!\n\r", ch, NULL, victim, TO_CHAR);
    act("$N does not exist anymore!\n\r", ch, NULL, victim, TO_ROOM);

    send_to_char("You turn into an invincible ghost for a few minutes.\n\r",
                 victim);
    send_to_char("As long as you don't attack anything.\n\r", victim);

    /*  disintegrate the objects... */
    tattoo = get_eq_char(victim, WEAR_TATTOO); /* keep tattoos for later */
    if (tattoo != NULL)
      obj_from_char(tattoo);

    victim->gold = 0;
    victim->silver = 0;

    for ( obj = victim->carrying; obj != NULL; obj = obj_next )
    {
        obj_next = obj->next_content;
	extract_obj( obj );
    }

    if ( IS_NPC(victim) )
    {
      victim->pIndexData->killed++;
      kill_table[URANGE(0, victim->level, MAX_LEVEL-1)].killed++;
      extract_char( victim, TRUE );
      return;
    }
    
    extract_char( victim, FALSE );

    while ( victim->affected )
      affect_remove( victim, victim->affected );
    victim->affected_by   = 0;
    for (i = 0; i < 4; i++)
      victim->armor[i]= 100;
    victim->position      = POS_RESTING;
    victim->hit           = 1;
    victim->mana  	  = 1;

    REMOVE_BIT(victim->act, PLR_WANTED);
    REMOVE_BIT(victim->act, PLR_BOUGHT_PET);

    victim->pcdata->condition[COND_THIRST] = 40;
    victim->pcdata->condition[COND_HUNGER] = 40;
    victim->pcdata->condition[COND_FULL] = 40;
    victim->pcdata->condition[COND_BLOODLUST] = 40;
    victim->pcdata->condition[COND_DESIRE] = 40;

    victim->last_death_time = current_time;

    if (tattoo != NULL)
    {
      obj_to_char(tattoo, victim);
      equip_char(victim, tattoo, WEAR_TATTOO);
    }

    for (tmp_ch = char_list; tmp_ch != NULL; tmp_ch = tmp_ch->next)
      if (tmp_ch->last_fought == victim)
        tmp_ch->last_fought = NULL;

    return;
}

void spell_poison_smoke( int sn, int level, CHAR_DATA *ch, void *vo, int target) {

  CHAR_DATA *tmp_vict;
  char buf[MAX_STRING_LENGTH];

  send_to_char("A cloud of poison smoke fills the room.\n\r",ch);
  act("A cloud of poison smoke fills the room.",ch,NULL,NULL,TO_ROOM);

  for (tmp_vict=ch->in_room->people;tmp_vict!=NULL;
       tmp_vict=tmp_vict->next_in_room)
    if (!is_safe_spell(ch,tmp_vict,TRUE))
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
		    (is_affected(ch,gsn_doppelganger)&&!IS_IMMORTAL(tmp_vict))?
		     ch->doppel->name : ch->name);
	         do_yell(tmp_vict,buf);
	      }
	  }
	  
	spell_poison(gsn_poison,ch->level,ch,tmp_vict, TARGET_CHAR);
/*        poison_effect(ch->in_room,level,level,TARGET_CHAR);  */
	if (tmp_vict != ch)
	  multi_hit(tmp_vict,ch,TYPE_UNDEFINED);
	
      }
  
}

void spell_blindness_dust( int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
  CHAR_DATA *tmp_vict;
  char buf[MAX_STRING_LENGTH];

  send_to_char("A cloud of dust fills in the room.\n\r",ch);
  act("A cloud of dust fills the room.",ch,NULL,NULL,TO_ROOM);


  for (tmp_vict=ch->in_room->people;tmp_vict!=NULL;
       tmp_vict=tmp_vict->next_in_room)
    if (!is_safe_spell(ch,tmp_vict,TRUE))
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

void spell_bark_skin( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( ch, sn ) )
    {
	if (victim == ch)
	  send_to_char("Your skin is already covered in bark.\n\r",ch); 
	else
	  act("$N is already as hard as can be.",ch,NULL,victim,TO_CHAR);
	return;
    }
    af.where	 = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = level;
    af.location  = APPLY_AC;
    af.modifier  = -((int)((float)level * 1.5));
    af.bitvector = 0;
    affect_to_char( victim, &af );
    act( "$n's skin becomes covered in bark.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "Your skin becomes covered in bark.\n\r", victim );
    return;
}

void spell_bear_call( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *gch;
  CHAR_DATA *bear;
  CHAR_DATA *bear2;
  AFFECT_DATA af;
  int i;
  
  send_to_char("You call for bears help you.\n\r",ch);
  act("$n shouts a bear call.",ch,NULL,NULL,TO_ROOM);

  if (is_affected(ch, sn))
    {
      send_to_char("You cannot summon the strength to handle more bears right now.\n\r", ch);
      return;
    }
  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  gch->pIndexData->vnum == MOB_VNUM_BEAR)
	{
	  send_to_char("What's wrong with the bear you've got?",ch);
	  return;
	}
    }

  if (count_charmed(ch)) return;

  bear = create_mobile( get_mob_index(MOB_VNUM_BEAR) );

  for (i=0;i < MAX_STATS; i++)
    {
      bear->perm_stat[i] = UMIN(25,2 * ch->perm_stat[i]);
    }

  bear->max_hit = IS_NPC(ch)? ch->max_hit : ch->pcdata->perm_hit;
  bear->hit = bear->max_hit;
  bear->max_mana = IS_NPC(ch)? ch->max_mana : ch->pcdata->perm_mana;
  bear->mana = bear->max_mana;
  bear->alignment = ch->alignment;
  bear->level = UMIN(70,1 * ch->level);
  for (i=0; i < 3; i++)
    bear->armor[i] = interpolate(bear->level,100,-100);
  bear->armor[3] = interpolate(bear->level,100,0);
  bear->sex = ch->sex;
  bear->gold = 0;
    
  bear2 = create_mobile(bear->pIndexData);
  clone_mobile(bear,bear2);
  
  SET_BIT(bear->affected_by, AFF_CHARM);
  SET_BIT(bear2->affected_by, AFF_CHARM);
  bear->master = bear2->master = ch;
  bear->leader = bear2->leader = ch;

  char_to_room(bear,ch->in_room);
  char_to_room(bear2,ch->in_room);
  send_to_char("Two bears come to your rescue!\n\r",ch);
  act("Two bears come to $n's rescue!",ch,NULL,NULL,TO_ROOM);

  af.where		= TO_AFFECTS;
  af.type               = sn;
  af.level              = level; 
  af.duration           = 24;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);  

}

void spell_ranger_staff( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  OBJ_DATA *staff;
  AFFECT_DATA tohit;
  AFFECT_DATA todam;

  staff = create_object( get_obj_index(OBJ_VNUM_RANGER_STAFF),level);
  send_to_char("You create a ranger's staff!\n\r",ch);
  act("$n creates a ranger's staff!",ch,NULL,NULL,TO_ROOM);

  if (ch->level  < 50 )
  	staff->value[2] = (ch->level / 10);
  else staff->value[2] = (ch->level / 6 ) - 3;
  staff->level = ch->level;

  tohit.where		   = TO_OBJECT;
  tohit.type               = sn;
  tohit.level              = ch->level; 
  tohit.duration           = -1;
  tohit.location           = APPLY_HITROLL;
  tohit.modifier           = 2 + level/5;
  tohit.bitvector          = 0;
  affect_to_obj(staff,&tohit);

  todam.where		   = TO_OBJECT;
  todam.type               = sn;
  todam.level              = ch->level; 
  todam.duration           = -1;
  todam.location           = APPLY_DAMROLL;
  todam.modifier           = 2 + level/5;
  todam.bitvector          = 0;
  affect_to_obj(staff,&todam);


  staff->timer = level;
  
  obj_to_char(staff,ch);
}

void spell_vanish( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  ROOM_INDEX_DATA *pRoomIndex;
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  int i;

  if ( victim->in_room == NULL
      ||   IS_SET(victim->in_room->room_flags, ROOM_NO_RECALL))
  {
      send_to_char( "You failed.\n\r", ch );
      return;
  }

  for (i=0 ; i < 65535; i++) 
    {
      pRoomIndex = get_room_index( number_range( 0, 65535 ) );
      if ( pRoomIndex != NULL )
	if ( can_see_room(victim,pRoomIndex) && !room_is_private(pRoomIndex)
	    && victim->in_room->area == pRoomIndex->area)
	    
	  break;
    }
 
  if ( pRoomIndex == NULL )
  {
      send_to_char( "You failed.\n\r", ch );
      return;
  }

  
  act( "$n throws down a small globe.", ch, NULL, NULL, TO_ROOM );

  if (!IS_NPC(ch) && ch->fighting != NULL && number_bits(1) == 1) {
    send_to_char("You failed.\n\r",ch);
    return;
  }

  act( "$n is gone!",victim,NULL,NULL,TO_ROOM);

  char_from_room( victim );
  char_to_room( victim, pRoomIndex );
  act( "$n appears from nowhere.", victim, NULL, NULL, TO_ROOM );
  do_look( victim, "auto" );
  stop_fighting(victim,TRUE);
  return;
}

void spell_transform( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  AFFECT_DATA af;

  if (is_affected(ch,sn) || ch->hit > ch->max_hit)
    {
      send_to_char("You are already overflowing with health.\n\r",ch);
      return;
    }

  ch->hit += UMIN(30000 - ch->max_hit, ch->max_hit);

  af.where		= TO_AFFECTS;
  af.type               = sn;
  af.level              = level; 
  af.duration           = 24;
  af.location           = APPLY_HIT;
  af.modifier           = UMIN(30000 - ch->max_hit, ch->max_hit);
  af.bitvector          = 0;
  affect_to_char(ch,&af);


  send_to_char("Your mind clouds as your health increases.\n\r",ch);
}

void spell_mana_transfer( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;

  if (victim == ch)
    {
      send_to_char("You would implode if you tried to transfer mana to yourself.\n\r",ch);
      return;
    }

  if (ch->cabal != victim->cabal)
    {
      send_to_char("You may only cast this spell on fellow cabal members.\n\r",ch);
      return;
    }

  if (ch->hit < 50)
    damage(ch,ch,50,sn,DAM_NONE, TRUE);
  else {
    damage(ch,ch,50,sn,DAM_NONE, TRUE);
    victim->mana = UMIN(victim->max_mana, victim->mana + number_range(20,120));
  }
}
    
void spell_mental_knife( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  AFFECT_DATA af;
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  int dam;

  if (ch->level < 40)
	dam = dice(level,8);
  else if (ch->level < 65)
	dam = dice(level,11);
  else dam = dice(level,14);

  if (saves_spell(level,victim, DAM_MENTAL))
	      dam /= 2;
  damage(ch,victim,dam,sn,DAM_MENTAL, TRUE);

  if (!is_affected(victim,sn) && !saves_spell(level, victim, DAM_MENTAL))
    {
      af.where		    = TO_AFFECTS;
      af.type               = sn;
      af.level              = level; 
      af.duration           = level;
      af.location           = APPLY_INT;
      af.modifier           = -7;
      af.bitvector          = 0;
      affect_to_char(victim,&af);

      af.location = APPLY_WIS;
      affect_to_char(victim,&af);
      act("Your mental knife sears $N's mind!",ch,NULL,victim,TO_CHAR);
      act("$n's mental knife sears your mind!",ch,NULL,victim,TO_VICT);
      act("$n's mental knife sears $N's mind!",ch,NULL,victim,TO_NOTVICT);
    }
}

void spell_demon_summon( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  CHAR_DATA *gch;
  CHAR_DATA *demon;
  AFFECT_DATA af;
  int i;

  if (is_affected(ch,sn))
    {
      send_to_char("You lack the power to summon another demon right now.\n\r",
		   ch);
      return;
    }

  send_to_char("You attempt to summon a demon.\n\r",ch);
  act("$n attempts to summon a demon.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  gch->pIndexData->vnum == MOB_VNUM_DEMON)
	{
	  send_to_char("Two demons are more than you can control!\n\r",ch);
	  return;
	}
    }

  if (count_charmed(ch)) return;

  demon = create_mobile( get_mob_index(MOB_VNUM_DEMON) );

  for (i=0;i < MAX_STATS; i++)
    {
      demon->perm_stat[i] = ch->perm_stat[i];
    }

  demon->max_hit = IS_NPC(ch)? URANGE(ch->max_hit,1 * ch->max_hit,30000)
		: URANGE(ch->pcdata->perm_hit,ch->hit,30000);
  demon->hit = demon->max_hit;
  demon->max_mana = IS_NPC(ch)? ch->max_mana : ch->pcdata->perm_mana;
  demon->mana = demon->max_mana;
  demon->level = ch->level;
  for (i=0; i < 3; i++)
    demon->armor[i] = interpolate(demon->level,100,-100);
  demon->armor[3] = interpolate(demon->level,100,0);
  demon->gold = 0;
  demon->timer = 0;
  demon->damage[DICE_NUMBER] = number_range(level/15, level/12);   
  demon->damage[DICE_TYPE] = number_range(level/3, level/2);
  demon->damage[DICE_BONUS] = number_range(level/10, level/8);

  char_to_room(demon,ch->in_room);
  send_to_char("A demon arrives from the underworld!\n\r",ch);
  act("A demon arrives from the underworld!",ch,NULL,NULL,TO_ROOM);

  af.where		= TO_AFFECTS;
  af.type               = sn;
  af.level              = level; 
  af.duration           = 24;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);  

  if (number_percent() < 40)
    {
      if ( can_see( demon, ch ) )
        do_say(demon, "You dare disturb me?!");
      else
        do_say(demon, "Who dares disturb me?!");
      do_murder(demon, ch->name);
    }
  else {
    SET_BIT(demon->affected_by, AFF_CHARM);
    demon->master = demon->leader = ch;
  }

}

void spell_scourge( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  CHAR_DATA *tmp_vict;
  CHAR_DATA *tmp_next;
  char buf[MAX_STRING_LENGTH];
  int dam;

  if (ch->level < 40)
	dam = dice(level,6);
  else if (ch->level < 65)
	dam = dice(level,9);
  else dam = dice(level,12);

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
		    (is_affected(ch,gsn_doppelganger)&&!IS_IMMORTAL(tmp_vict))?
		     ch->doppel->name : ch->name);
		  do_yell(tmp_vict,buf);
		}
	    }
	    
	  if (!is_affected(tmp_vict,sn)) {
	    

	    if (number_percent() < level)
	      spell_poison(gsn_poison, level, ch, tmp_vict, TARGET_CHAR);

	    if (number_percent() < level)
	      spell_blindness(gsn_blindness,level,ch,tmp_vict, TARGET_CHAR);

	    if (number_percent() < level)
	      spell_weaken(gsn_weaken, level, ch, tmp_vict, TARGET_CHAR);

            if (saves_spell(level,tmp_vict, DAM_FIRE))
	      dam /= 2;
	    damage( ch, tmp_vict, dam, sn, DAM_FIRE, TRUE );
	  }

	}
    }
}

void spell_doppelganger( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  AFFECT_DATA af;

  if ( (ch == victim) || 
      (is_affected(ch, sn) && (ch->doppel == victim)) )
    {
      act("You already look like $M.",ch,NULL,victim,TO_CHAR);
      return;
    }

  if (IS_NPC(victim))
    {
     act("$N is too different from yourself to mimic.",ch,NULL,victim,TO_CHAR);
     return;
   }

  if (IS_IMMORTAL(victim))
    {
      send_to_char("Yeah, sure. And I'm the Pope.\n\r",ch);
      return;
    }

  if (saves_spell(level,victim, DAM_CHARM))
   {
    send_to_char("You failed.\n\r",ch);
    return;
   }

  act("You change form to look like $N.",ch,NULL,victim,TO_CHAR);
  act("$n changes form to look like YOU!",ch,NULL,victim,TO_VICT);
  act("$n changes form to look like $N!",ch,NULL,victim,TO_NOTVICT);

  af.where 		= TO_AFFECTS;
  af.type               = sn;
  af.level              = level; 
  af.duration           = (2 * level)/3;
  af.location           = APPLY_NONE;
  af.modifier           = 0;
  af.bitvector          = 0;

  affect_to_char(ch,&af);  
  ch->doppel = victim;

}
  
void spell_manacles( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;

  if (!IS_SET(victim->act, PLR_WANTED))
    {
      act("But $N is not wanted.",ch,NULL,victim,TO_CHAR);
      return;
    }

  if (!is_affected(victim, sn) && !saves_spell(ch->level, victim,DAM_CHARM))
    {
      AFFECT_DATA af;

      af.where		    = TO_AFFECTS;
      af.type               = sn;
      af.level              = level; 
      af.duration           = 5 + level/5;
      af.bitvector          = 0;

      af.modifier           = 0 - (get_curr_stat(victim, STAT_DEX)-4);
      af.location           = APPLY_DEX;
      affect_to_char(victim, &af);

      af.modifier           = -5;
      af.location           = APPLY_HITROLL;
      affect_to_char(victim, &af);

	
      af.modifier           = -10;
      af.location           = APPLY_DAMROLL;
      affect_to_char(victim, &af);

      spell_charm_person(gsn_charm_person, level, ch, vo, TARGET_CHAR);
    }
}

void spell_shield_ruler( int sn, int level, CHAR_DATA *ch, void *vo,int target) 
{
  int shield_vnum;
  OBJ_DATA *shield;
  AFFECT_DATA af;

  if (level >= 71)
    shield_vnum = OBJ_VNUM_RULER_SHIELD4;
  else if (level >= 51)
    shield_vnum = OBJ_VNUM_RULER_SHIELD3;
  else if (level >= 31)
    shield_vnum = OBJ_VNUM_RULER_SHIELD2;
  else shield_vnum = OBJ_VNUM_RULER_SHIELD1;

  shield = create_object( get_obj_index(shield_vnum), level );
  shield->timer = level;
  shield->level = ch->level;
  shield->cost  = 0;
  obj_to_char(shield, ch);
  
  af.where        = TO_OBJECT;
  af.type         = sn;
  af.level        = level;
  af.duration     = -1;
  af.modifier     = level / 8;
  af.bitvector    = 0;

  af.location     = APPLY_HITROLL;
  affect_to_obj(shield,&af);

  af.location     = APPLY_DAMROLL;
  affect_to_obj(shield,&af);

  
  af.where        = TO_OBJECT;
  af.type         = sn;
  af.level        = level;
  af.duration     = -1;
  af.modifier     = -level/2;
  af.bitvector    = 0;
  af.location     = APPLY_AC;
  affect_to_obj(shield,&af);

  af.where        = TO_OBJECT;
  af.type         = sn;
  af.level        = level;
  af.duration     = -1;
  af.modifier     = -level/9;
  af.bitvector    = 0;
  af.location     = APPLY_SAVING_SPELL;
  affect_to_obj(shield,&af);

  af.where        = TO_OBJECT;
  af.type         = sn;
  af.level        = level;
  af.duration     = -1;
  af.modifier     = UMAX(1,level /  30);
  af.bitvector    = 0;
  af.location     = APPLY_CHA;
  affect_to_obj(shield,&af);

  act("You create $p!",ch,shield,NULL,TO_CHAR);
  act("$n creates $p!",ch,shield,NULL,TO_ROOM);
}
  
void spell_guard_call( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *gch;
  CHAR_DATA *guard;
  CHAR_DATA *guard2;
  CHAR_DATA *guard3;
  AFFECT_DATA af;
  char buf[] = "Guards! Guards!";
  int i;

  if (is_affected(ch,sn))
    {
      send_to_char("You lack the power to call another three guards now.\n\r",ch);
      return;
    }

  do_yell(ch, buf);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch
	  && gch->pIndexData->vnum == MOB_VNUM_SPECIAL_GUARD)
	{
	  do_say(gch, "What? I'm not good enough?");
	  return;
	}
    }

  if (count_charmed(ch)) return;

  guard = create_mobile( get_mob_index(MOB_VNUM_SPECIAL_GUARD) );

  for (i=0;i < MAX_STATS; i++)
    guard->perm_stat[i] = ch->perm_stat[i];

  guard->max_hit = 2*ch->max_hit;
  guard->hit = guard->max_hit;
  guard->max_mana = ch->max_mana;
  guard->mana = guard->max_mana;
  guard->alignment = ch->alignment;
  guard->level = ch->level;
  for (i=0; i < 3; i++)
    guard->armor[i] = interpolate(guard->level,100,-200);
  guard->armor[3] = interpolate(guard->level,100,-100);
  guard->sex = ch->sex;
  guard->gold = 0;
  guard->timer = 0;

  guard->damage[DICE_NUMBER] = number_range(level/16, level/12);   
  guard->damage[DICE_TYPE] = number_range(level/3, level/2);
  guard->damage[DICE_BONUS] = number_range(level/9, level/6);

  guard->detection = (A|C|D|E|F|G|H|ee);
  SET_BIT(guard->affected_by, AFF_CHARM);
  SET_BIT(guard->affected_by, AFF_SANCTUARY);

  guard2 = create_mobile(guard->pIndexData);
  clone_mobile(guard,guard2);
  
  guard3 = create_mobile(guard->pIndexData);
  clone_mobile(guard,guard3);

  guard->master = guard2->master = guard3->master = ch;
  guard->leader = guard2->leader = guard3->leader = ch;

  char_to_room(guard,ch->in_room);
  char_to_room(guard2,ch->in_room);
  char_to_room(guard3,ch->in_room);
  send_to_char("Three guards come to your rescue!\n\r",ch);
  act("Three guards come to $n's rescue!",ch,NULL,NULL,TO_ROOM);

  af.where		= TO_AFFECTS;
  af.type               = sn;
  af.level              = level; 
  af.duration           = 6;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);  

}

void spell_nightwalker( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  CHAR_DATA *gch;
  CHAR_DATA *walker;
  AFFECT_DATA af;
  char buf[100];
  int i;

  if (is_affected(ch,sn))
    {
      send_to_char("You feel too weak to summon a Nightwalker now.\n\r",
		   ch);
      return;
    }

  send_to_char("You attempt to summon a Nightwalker.\n\r",ch);
  act("$n attempts to summon a Nightwalker.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  gch->pIndexData->vnum == MOB_VNUM_NIGHTWALKER)
	{
	  send_to_char("Two Nightwalkers are more than you can control!\n\r",ch);
	  return;
	}
    }

  if (count_charmed(ch)) return;

  walker = create_mobile( get_mob_index(MOB_VNUM_NIGHTWALKER) );

  for (i=0;i < MAX_STATS; i++)
    {
      walker->perm_stat[i] = ch->perm_stat[i];
    }

  walker->max_hit = IS_NPC(ch)? URANGE(ch->max_hit,1 * ch->max_hit,30000)
                : URANGE(ch->pcdata->perm_hit,1*ch->pcdata->perm_hit,30000);
  walker->hit = walker->max_hit;
  walker->max_mana = ch->max_mana;
  walker->mana = walker->max_mana;
  walker->level = ch->level;
  for (i=0; i < 3; i++)
    walker->armor[i] = interpolate(walker->level,100,-100);
  walker->armor[3] = interpolate(walker->level,100,0);
  walker->gold = 0;
  walker->timer = 0;
  walker->damage[DICE_NUMBER] = number_range(level/15, level/10);   
  walker->damage[DICE_TYPE]   = number_range(level/3, level/2);
  walker->damage[DICE_BONUS]  = 0;
   
  char_to_room(walker,ch->in_room);
  send_to_char("A Nightwalker rises from the shadows!\n\r",ch);
  act("A Nightwalker rises from the shadows!",ch,NULL,NULL,TO_ROOM);
  sprintf(buf, "A Nightwalker kneels before you.");
  send_to_char( buf, ch );
  sprintf(buf, "A Nightwalker kneels before %s!", ch->name );
  act( buf, ch, NULL, NULL, TO_ROOM );

  af.where		= TO_AFFECTS;
  af.type               = sn;
  af.level              = level; 
  af.duration           = 24;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);  

  SET_BIT(walker->affected_by, AFF_CHARM);
  walker->master = walker->leader = ch;

}
  
void spell_eyes( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim;
    ROOM_INDEX_DATA *ori_room;

    if ( (victim = get_char_world(ch, target_name)) == NULL)
      {
	send_to_char("Your spy network reveals no such player.\n\r",ch);
	return;
      }

    if ((victim->level > ch->level + 7) 
	|| saves_spell((ch->level + 9), victim, DAM_NONE))
      {
	send_to_char("Your spy network cannot find that player.\n\r",ch);
	return;
      }

    if (ch==victim)
      do_look( ch, "auto" );
    else {
      ori_room = ch->in_room;
      char_from_room( ch );
      char_to_room( ch, victim->in_room );
      do_look( ch, "auto" );
      char_from_room( ch );
      char_to_room( ch, ori_room );
    }
}

void spell_shadow_cloak( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if (ch->cabal != victim->cabal)
      {
	send_to_char("You may only use this spell on fellow cabal members.\n\r",ch);
	return;
      }

    if ( is_affected( victim, sn ) )
    {
	if (victim == ch)
	  send_to_char("You are already protected by a shadow cloak.\n\r",ch);
	else
	  act("$N is already protected by a shadow cloak.",ch,NULL,victim,TO_CHAR);
	return;
    }

    af.where	 = TO_AFFECTS;
    af.type      = sn;
    af.level	 = level;
    af.duration  = 24;
    af.modifier  = -level;
    af.location  = APPLY_AC;
    af.bitvector = 0;
    affect_to_char( victim, &af );
    send_to_char( "You feel the shadows protect you.\n\r", victim );
    if ( ch != victim )
	act("A cloak of shadows protect $N.",ch,NULL,victim,TO_CHAR);
    return;
}
  
void spell_nightfall( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *vch;
  OBJ_DATA  *light;
  AFFECT_DATA af;
  
  if ( is_affected( ch, sn ) )
  {
    send_to_char( "You can't find the power to control lights.\n\r", ch );
    return;
  }

  for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
    for (light = vch->carrying; light != NULL; light = light->next_content)
      {
	if (light->item_type == ITEM_LIGHT && light->value[2] != 0
		&& !is_same_group(ch, vch)) {  
	  if ( /*light->value[2] != -1 ||*/ saves_spell(level, vch, DAM_ENERGY))
	  {
	    act("$p flickers and goes out!",ch,light,NULL,TO_CHAR);
	    act("$p flickers and goes out!",ch,light,NULL,TO_ROOM);
	    light->value[2] = 0; 
	    if (get_light_char(ch) == NULL) ch->in_room->light--;
	  }
/*	  else {
	    act("$p momentarily dims.",ch,light,NULL,TO_CHAR);
	    act("$p momentarily dims.",ch,light,NULL,TO_ROOM);
	  } */
	}
      }  

  for (light = ch->in_room->contents;light != NULL; light=light->next_content)
    if (light->item_type == ITEM_LIGHT && light->value[2] != 0) {  
      act("$p flickers and goes out!",ch,light,NULL,TO_CHAR);
      act("$p flickers and goes out!",ch,light,NULL,TO_ROOM);
      light->value[2] = 0; 
    }

    af.where	 = TO_AFFECTS;
    af.type      = sn;
    af.level	 = level;
    af.duration  = 2;
    af.modifier  = 0;
    af.location  = APPLY_NONE;
    af.bitvector = 0;
    affect_to_char( ch, &af );
}
	      
void spell_mirror( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  AFFECT_DATA af;
  int mirrors,new_mirrors;
  CHAR_DATA *gch;
  CHAR_DATA *tmp_vict;
  char long_buf[MAX_STRING_LENGTH];
  char short_buf[20];
  int order;

  if (IS_NPC(victim)) {
    send_to_char("Only players can be mirrored.\n\r",ch);
    return;
  }

  for (mirrors = 0, gch = char_list; gch != NULL; gch = gch->next)
    if (IS_NPC(gch) && is_affected(gch,gsn_mirror)
	&& is_affected(gch,gsn_doppelganger) && gch->doppel == victim)
      mirrors++;

if (mirrors >= level/5) {
  if (ch==victim) 
    send_to_char("You cannot be further mirrored.\n\r",ch);
  else
    act("$N cannot be further mirrored.",ch,NULL,victim,TO_CHAR);
  return;
}

  af.where     = TO_AFFECTS;
  af.level     = level;
  af.modifier  = 0;
  af.location  = 0;
  af.bitvector = 0;

  for (tmp_vict = victim; is_affected(tmp_vict,gsn_doppelganger);
       tmp_vict = tmp_vict->doppel);

  sprintf(long_buf, "%s%s is here.\n\r", tmp_vict->name, tmp_vict->pcdata->title);
  sprintf(short_buf, tmp_vict->name);

  order = number_range(0,level/5 - mirrors);

  for (new_mirrors=0; mirrors + new_mirrors < level/5;new_mirrors++) 
  {
    gch = create_mobile( get_mob_index(MOB_VNUM_MIRROR_IMAGE) );
    free_string(gch->name);
    free_string(gch->short_descr);
    free_string(gch->long_descr);
    free_string(gch->description);
    gch->name = str_dup(tmp_vict->name);
    gch->short_descr = str_dup(short_buf);
    gch->long_descr = str_dup(long_buf);
    gch->description = (tmp_vict->description == NULL) ? 
                       NULL : str_dup(tmp_vict->description);
    gch->sex = tmp_vict->sex;
    
    af.type = gsn_doppelganger;
    af.duration = level;
    affect_to_char(gch,&af);
    af.type = gsn_mirror;
    af.duration = -1;
    affect_to_char(gch,&af);

    gch->max_hit = gch->hit = 1;
    gch->level = 1;
    gch->doppel = victim;
    gch->master = victim;
    char_to_room(gch,victim->in_room);

    if (number_percent() < 20)
    {
        ROOM_INDEX_DATA *ori_room;

	ori_room = victim->in_room;
	char_from_room(victim);
	char_to_room(victim, ori_room);
    }

    if (new_mirrors == order) 
    {
      char_from_room(victim);
      char_to_room(victim,gch->in_room);
    }
      

    if (ch==victim) {
      send_to_char("A mirror image of yourself appears beside you!\n\r",ch);
      act("A mirror image of $n appears beside $M!",ch,NULL,victim,TO_ROOM);
    }
    else {
      act("A mirror of $N appears beside $M!",ch,NULL,victim,TO_CHAR);
      act("A mirror of $N appears beside $M!",ch,NULL,victim,TO_NOTVICT);
      send_to_char("A mirror image of yourself appears beside you!\n\r",
		   victim);
    }
    
  }
}    
 
void spell_garble( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  AFFECT_DATA af;

  if (ch == victim) 
  {
    send_to_char("Garble whose speech?\n\r",ch);
    return;
  }

  if (is_affected(victim,sn)) 
  {
    act("$N's speech is already garbled.",ch,NULL,victim,TO_CHAR);
    return;
  }

  if (is_safe_nomessage(ch,victim)) {
      send_to_char("You cannot garble that person.\n\r",ch);
      return;
  } 

  if ((victim->level > ch->level + 7) 
        || saves_spell((ch->level + 9), victim, DAM_MENTAL))
    return;

  af.where	= TO_AFFECTS;
  af.type      = sn;
  af.level     = level;
  af.duration  = 10;
  af.modifier  = 0;
  af.location  = 0;
  af.bitvector = 0;
  affect_to_char(victim,&af);

  act("You have garbled $N's speech!",ch,NULL,victim,TO_CHAR);
  send_to_char("You feel your tongue contort.\n\r",victim);
}

void spell_confuse( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  AFFECT_DATA af;
  CHAR_DATA *rch;
  int count=0;

  if (is_affected(victim,gsn_confuse)) {
    act("$N is already thoroughly confused.",ch,NULL,victim,TO_CHAR);
    return;
  }

  if (saves_spell(level,victim, DAM_MENTAL))
    return;

  af.where		= TO_AFFECTS;
  af.type      = sn;
  af.level     = level;
  af.duration  = 10;
  af.modifier  = 0;
  af.location  = 0;
  af.bitvector = 0;
  affect_to_char(victim,&af);

    for ( rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room )
    {
     if ( rch == ch 
          && !can_see( ch, rch ) 
	  && get_trust(ch) < rch->invis_level)
	  count++;
	    continue;
    }

    for ( rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room )
    {
     if ( rch != ch 
          && can_see( ch, rch ) 
	  && get_trust(ch) >= rch->invis_level
	  && number_range(1,count) == 1 )
	 break;
    }

    if (rch)   do_murder(victim,rch->name); 
    do_murder(victim, ch->name);
}

void spell_terangreal( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
  
    if (IS_NPC(victim))
	return;

    af.where		= TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = 10;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_SLEEP;
    affect_join( victim, &af );

    if ( IS_AWAKE(victim) )
    {
	send_to_char( "You are overcome by a sudden surge of fatigue.\n\r",
		     victim );
	act( "$n falls into a deep sleep.", victim, NULL, NULL, TO_ROOM );
	victim->position = POS_SLEEPING;
    }

    return;
}
    
void spell_kassandra( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{

    AFFECT_DATA af;

    if ( is_affected( ch, sn ) )
      {
	send_to_char
	  ("The kassandra has been used for this purpose too recently.\n\r",
	   ch);
	return;
      }
    af.where		= TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = 5;
    af.location  = 0;
    af.modifier  = 0;
    af.bitvector = 0;
    affect_to_char( ch, &af );
    ch->hit = UMIN( ch->hit + 150, ch->max_hit );
    update_pos( ch );
    send_to_char( "A warm feeling fills your body.\n\r", ch );
    act("$n looks better.", ch, NULL, NULL, TO_ROOM);
    return;
}  
  
  
void spell_sebat( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  AFFECT_DATA af;

  if ( is_affected( ch, sn ) )
    {
      send_to_char("The kassandra has been used for that too recently.\n\r"
		   ,ch); 
      return;
    }
  af.where		= TO_AFFECTS;
  af.type      = sn;
  af.level     = level;
  af.duration  = level;
  af.location  = APPLY_AC;
  af.modifier  = -30;
  af.bitvector = 0;
  affect_to_char( ch, &af );
  act( "$n is surrounded by a mystical shield.",ch, NULL,NULL,TO_ROOM );
  send_to_char( "You are surrounded by a mystical shield.\n\r", ch );
  return;
}
    

void spell_matandra( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  int dam;
  AFFECT_DATA af;
  
  if ( is_affected( ch, sn ) )
    {
      send_to_char
	("The kassandra has been used for this purpose too recently.\n\r",
	 ch);
      return;
    }
  af.where		= TO_AFFECTS;
  af.type      = sn;
  af.level     = level;
  af.duration  = 5;
  af.location  = 0;
  af.modifier  = 0;
  af.bitvector = 0;
  affect_to_char( ch, &af );
  dam = dice(level, 7);
  
  damage(ch,victim,dam,sn,DAM_HOLY, TRUE);
  
}  
  
void spell_amnesia( int sn, int level, CHAR_DATA *ch, void *vo, int target )  
{
  int i;
  CHAR_DATA *victim = (CHAR_DATA *) vo;

  if (IS_NPC(victim))
    return;

  for (i = 0; i < MAX_SKILL; i++)
    victim->pcdata->learned[i] /= 2;

  act("You feel your memories slip away.",victim,NULL,NULL,TO_CHAR);
  act("$n gets a blank look on $s face.",victim,NULL,NULL,TO_ROOM);
}
    

void spell_chaos_blade( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  OBJ_DATA *blade;
  AFFECT_DATA af;
  
  blade = create_object( get_obj_index(OBJ_VNUM_CHAOS_BLADE),level);
  send_to_char("You create a blade of chaos!\n\r",ch);
  act("$n creates a blade of chaos!",ch,NULL,NULL,TO_ROOM);

  blade->timer = level * 2;
  blade->level = ch->level;

  if (  ch->level <= 10)                        blade->value[2] = 2;
  else if ( ch->level > 10 && ch->level <= 20)   blade->value[2] = 3;
  else if ( ch->level > 20 && ch->level <= 30)   blade->value[2] = 4;
  else if ( ch->level > 30 && ch->level <= 40)   blade->value[2] = 5;
  else if ( ch->level > 40 && ch->level <= 50)   blade->value[2] = 6;
  else if ( ch->level > 50 && ch->level <= 60)   blade->value[2] = 7;
  else if ( ch->level > 60 && ch->level <= 70)   blade->value[2] = 9;
  else if ( ch->level > 70 && ch->level <= 80)   blade->value[2] = 11;
  else blade->value[2] = 12;

  af.where        = TO_OBJECT;
  af.type         = sn;
  af.level        = level;
  af.duration     = -1;
  af.modifier     = level / 6;
  af.bitvector    = 0;

  af.location     = APPLY_HITROLL;
  affect_to_obj(blade,&af);

  af.location     = APPLY_DAMROLL;
  affect_to_obj(blade,&af);

  obj_to_char(blade,ch);
}    

void spell_tattoo( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  OBJ_DATA *tattoo;
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  int i;

  if (IS_NPC(victim))
    {
      act("$N is too dumb to worship you!", ch, NULL, victim, TO_CHAR);
      return;
    }

  for (i = 0; i < MAX_RELIGION; i++)
    {
      if (!str_cmp(ch->name, religion_table[i].leader))
	{
  	  tattoo = get_eq_char(victim, WEAR_TATTOO);
   	  if (tattoo != NULL)
    	    {
     	      act("$N is already tattooed!  You'll have to remove it first.",
		ch, NULL, victim, TO_CHAR);
      	      act("$n tried to give you another tattoo but failed.",
		ch, NULL, victim, TO_VICT);
              act("$n tried to give $N another tattoo but failed.",
		ch, NULL, victim, TO_NOTVICT);
      	      return;
    	    }
  	  else
    	    { 
      	      tattoo = create_object(get_obj_index(religion_table[i].vnum),60);
       	      act("You tattoo $N with $p!",ch, tattoo, victim, TO_CHAR);
      	      act("$n tattoos $N with $p!",ch,tattoo,victim,TO_NOTVICT);
      	      act("$n tattoos you with $p!",ch,tattoo,victim,TO_VICT);

      	      obj_to_char(tattoo,victim);
      	      equip_char(victim, tattoo, WEAR_TATTOO);
	      return;
	    }
	}    
    }
  send_to_char("You don't have a religious tattoo.\n\r", ch);
  return;    
}

void spell_remove_tattoo( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  OBJ_DATA *tattoo;
  CHAR_DATA *victim = (CHAR_DATA *) vo;
 
  tattoo = get_eq_char(victim, WEAR_TATTOO);
  if (tattoo != NULL)
    {
      extract_obj(tattoo);
      act("Through a painful process, your tattoo has been destroyed by $n.",
	ch, NULL, victim, TO_VICT);
      act("You remove the tattoo from $N.", ch, NULL, victim, TO_CHAR);
      act("$N's tattoo is destroyed by $n.", ch, NULL, victim, TO_NOTVICT);
    }
  else
    act("$N doesn't have any tattoos.", ch, NULL, victim, TO_CHAR);
}


void spell_wrath( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  int dam;
  AFFECT_DATA af;
  
  if ( !IS_NPC(ch) && IS_EVIL(ch) )
    victim = ch;
  
  if ( IS_GOOD(victim) )
    {
      act( "The gods protect $N.", ch, NULL, victim, TO_ROOM );
      return;
    }
  
  if ( IS_NEUTRAL(victim) )
    {
      act( "$N does not seem to be affected.", ch, NULL, victim, TO_CHAR );
      return;
    }

  dam = dice(level,12);

  if ( saves_spell( level, victim, DAM_HOLY ) )
    dam /= 2;
  damage( ch, victim, dam, sn, DAM_HOLY, TRUE );
  
  if (IS_AFFECTED(victim, AFF_CURSE) || saves_spell( level, victim, DAM_HOLY ))
    return;

  af.where     = TO_AFFECTS;
  af.type      = sn;
  af.level     = level;
  af.duration  = 2*level;
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

void spell_old_randomizer(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
  ROOM_INDEX_DATA *pRoomIndex;
  EXIT_DATA *pexit;
  int d0;
  int d1;
  AFFECT_DATA af;
  char log_buf[MAX_INPUT_LENGTH];

  if ( is_affected( ch, sn ) )
    {
      send_to_char
	("Your power of randomness has been exhausted for now.\n\r",
	 ch);
      return;
    }
  if (IS_SET(ch->in_room->room_flags, ROOM_LAW))
    {
      send_to_char(
	    "This room is far too orderly for your powers to work on it.\n\r",
		   ch);
      return;
    }

  af.where		= TO_AFFECTS;
  af.type      = sn;
  af.level     = UMIN(level + 15, MAX_LEVEL);
  af.location  = 0;
  af.modifier  = 0;
  af.bitvector = 0;

  pRoomIndex = get_room_index(ch->in_room->vnum);

  if (number_bits(1) == 0)
    {
      send_to_char("Despite your efforts, the universe resisted chaos.\n\r",
		   ch);
      if (ch->trust >= 56)
        af.duration  = 1;
      else
	af.duration = level;
      affect_to_char(ch, &af);
      return;
    }
  for (d0 = 0; d0 < 5; d0++)
    {
      d1 = number_range(d0, 5);
      pexit = pRoomIndex->exit[d0];
      pRoomIndex->exit[d0] = pRoomIndex->exit[d1];
      pRoomIndex->exit[d1] = pexit;

    }
  if (ch->trust >= 56)
    af.duration = 1;
  else
    af.duration = 2*level;
  affect_to_char(ch, &af);
  send_to_char("The room was successfully randomized!\n\r", ch);
  send_to_char("You feel very drained from the effort.\n\r", ch);
  ch->hit -= UMIN(200, ch->hit/2);

  sprintf(log_buf, "%s used randomizer in room %d", ch->name, ch->in_room->vnum);
  log_string(log_buf);

}

void spell_stalker( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *victim;
  CHAR_DATA *stalker;
  AFFECT_DATA af;
  int i;
  char log_buf[MAX_INPUT_LENGTH];

  if ( ( victim = get_char_world( ch, target_name ) ) == NULL
      ||   victim == ch || victim->in_room == NULL
      || IS_NPC(victim) || !IS_SET(victim->act, PLR_WANTED))
    {
      send_to_char("You failed.\n\r", ch);
      return;
    }

  if (is_affected(ch,sn))
    {
      send_to_char("This power is used too recently.\n\r", ch);
      return;
    }

  if (!is_safe_nomessage(ch,victim) && !IS_SET(ch->act,PLR_CANINDUCT) )
    {
      send_to_char("You better use special guards for this purpose.\n\r", ch);
      return;
    }

  send_to_char("You attempt to summon a stalker.\n\r",ch);
  act("$n attempts to summon a stalker.",ch,NULL,NULL,TO_ROOM);

  stalker = create_mobile( get_mob_index(MOB_VNUM_STALKER) );

  af.where		= TO_AFFECTS;
  af.type               = sn;
  af.level              = level; 
  af.duration           = 6;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);  

  for (i=0;i < MAX_STATS; i++)
    {
      stalker->perm_stat[i] = victim->perm_stat[i];
    }

  stalker->max_hit = UMIN(30000,2 * victim->max_hit);
  stalker->hit = stalker->max_hit;
  stalker->max_mana = victim->max_mana;
  stalker->mana = stalker->max_mana;
  stalker->level = victim->level;

  stalker->damage[DICE_NUMBER] = 
		number_range(victim->level/8, victim->level/6);   
  stalker->damage[DICE_TYPE] = 
		number_range(victim->level/6, victim->level/5);
  stalker->damage[DICE_BONUS] = 
		number_range(victim->level/10, victim->level/8);

  stalker->hitroll = victim->level;
  stalker->damroll = victim->level;
  stalker->size = victim->size;
  for (i=0; i < 3; i++)
    stalker->armor[i] = interpolate(stalker->level,100,-100);
  stalker->armor[3] = interpolate(stalker->level,100,0);
  stalker->gold = 0;
  stalker->invis_level = LEVEL_IMMORTAL;
  stalker->detection = (A|B|C|D|E|F|G|H|ee);
  stalker->affected_by = (H|J|N|O|U|V|aa|cc);
  
  char_to_room(stalker,victim->in_room);
  stalker->last_fought = victim;
  send_to_char("An invisible stalker arrives to stalk you!\n\r",victim);
  act("An invisible stalker arrives to stalk $n!",victim,NULL,NULL,TO_ROOM);
  send_to_char("An invisible stalker has been sent.\n\r", ch);

  sprintf(log_buf, "%s used stalker on %s", ch->name, victim->name);
  log_string(log_buf);
}

  
void spell_tesseract( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *victim;
  CHAR_DATA *wch;
  CHAR_DATA *wch_next;
  bool gate_pet;

  if( (victim = get_char_world(ch,target_name))==NULL
	|| victim == ch
	|| victim->in_room == NULL
	|| ch->in_room == NULL) {
	send_to_char("You failed.\n\r",ch);
	return;
	}

  
  if ( !can_see_room(ch,victim->in_room) 
      ||   (is_safe(ch,victim) && IS_SET(victim->act,PLR_NOSUMMON))
      ||   room_is_private(victim->in_room)
      ||   IS_SET(victim->in_room->room_flags, ROOM_NO_RECALL)
      ||   IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL)
      ||   IS_SET(victim->in_room->room_flags, ROOM_NOSUMMON)
      ||   IS_SET(ch->in_room->room_flags, ROOM_NOSUMMON)
      ||   (!IS_NPC(victim) && victim->level >= LEVEL_HERO)  /* NOT trust */ 
      ||   (IS_NPC(victim) && IS_SET(victim->imm_flags,IMM_SUMMON))
      ||   (!IS_NPC(victim) && IS_SET(victim->act,PLR_NOSUMMON) 
	    && is_safe_nomessage(ch,victim))
      ||   (saves_spell( level, victim, DAM_NONE ) ) )
    {
      send_to_char( "You failed.\n\r", ch );
      return;
    }
  
  if (ch->pet != NULL && ch->in_room == ch->pet->in_room)
    gate_pet = TRUE;
  else
    gate_pet = FALSE;

  for (wch = ch->in_room->people; wch != NULL; wch = wch_next)
    {
      wch_next = wch->next_in_room;
      if (is_same_group(wch, ch) && wch != ch)
	{
	  act("$n utters some strange words and, with a sickening lurch, you feel time",ch, NULL, wch, TO_VICT);
	  act("and space shift around you.", ch, NULL, wch, TO_VICT);
          if(victim->in_room == NULL) {
		bug("Tesseract: victim room has become NULL!!!",0);
                return;
          }
	  char_from_room(wch);
	  char_to_room(wch,victim->in_room);
	  act("$n arrives suddenly.",wch,NULL,NULL,TO_ROOM);
	  if (wch->in_room == NULL)
	     bug("Tesseract: other char sent to NULL room",0); 
	  else
	     do_look(wch,"auto");
	} 
    }
 
  act("With a sudden flash of light, $n and $s friends disappear!",ch,NULL,NULL,TO_ROOM);
  send_to_char("As you utter the words, time and space seem to blur.  You feel as though\n\rspace and time are shifting all around you while you remain motionless.\n\r",ch);
  char_from_room(ch);
  char_to_room(ch,victim->in_room);

  act("$n arrives suddenly.",ch,NULL,NULL,TO_ROOM);
  if (ch->in_room == NULL)
     bug("Tesseract: char sent to NULL room",0); 
  else
     do_look(ch,"auto");
  
  if (gate_pet)
    {
      send_to_char("You feel time and space shift around you.\n\r",ch->pet);
      char_from_room(ch->pet);
      char_to_room(ch->pet,victim->in_room);
      act("$n arrives suddenly.",ch->pet,NULL,NULL,TO_ROOM);
      if (ch->pet->in_room == NULL)
         bug("Tesseract: pet sent to NULL room",0); 
      else
         do_look(ch->pet,"auto");
    }
}

void spell_brew( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    OBJ_DATA *potion;
    OBJ_DATA *vial;
    int spell;

    if (obj->item_type != ITEM_TRASH && obj->item_type != ITEM_TREASURE
	&& obj->item_type != ITEM_KEY)
      {
	send_to_char("That can't be transformed into a potion.\n\r",ch);
	return;
      }
    
    if (obj->wear_loc != -1)
      {
	send_to_char("The item must be carried to be brewed.\n\r",ch);
	return;
      }

    for( vial=ch->carrying; vial != NULL; vial=vial->next_content )
      if ( vial->pIndexData->vnum == OBJ_VNUM_POTION_VIAL )
        break;
    if (  vial == NULL )  {
	send_to_char( "You don't have any vials to brew the potion into.\n\r"
			, ch );
	return;
    }
 

    if (number_percent() < 50)
      { 
	send_to_char("You failed and destroyed it.\n\r", ch);
	extract_obj(obj);
	return;
      }
	
    if (obj->item_type == ITEM_TRASH)
      potion = create_object( get_obj_index(OBJ_VNUM_POTION_SILVER), level);
    else if (obj->item_type == ITEM_TREASURE)
      potion = create_object( get_obj_index(OBJ_VNUM_POTION_GOLDEN), level);
    else
      potion = create_object( get_obj_index(OBJ_VNUM_POTION_SWIRLING), level);

    spell = 0;

    potion->value[0] = level;

    if (obj->item_type == ITEM_TRASH)
      {
	if (number_percent() < 20)
	  spell = skill_lookup("fireball");
	else if (number_percent() < 40)
	  spell = skill_lookup("cure poison");
	else if (number_percent() < 60)
	  spell = skill_lookup("cure blind");
	else if (number_percent() < 80)
	  spell = skill_lookup("cure disease");
	else
	  spell = skill_lookup("word of recall");
      }
    else if (obj->item_type == ITEM_TREASURE)
      {
	switch(number_bits(3)) {
	case 0:
	  spell = skill_lookup("cure critical");
	  break;
	case 1:
	  spell = skill_lookup("haste");
	  break;
	case 2:
	  spell = skill_lookup("frenzy");
	  break;
	case 3:
	  spell = skill_lookup("create spring");
	  break;
	case 4:
	  spell = skill_lookup("holy word");
	  break;
	case 5:
	  spell = skill_lookup("invis");
	  break;
	case 6:
	  spell = skill_lookup("cure light");
	  break;
	case 7:
	  spell = skill_lookup("cure serious");
	  break;
	  
	}
      }
    else
      {
	if (number_percent() < 20)
	  spell = skill_lookup("detect magic");
	else if (number_percent() < 40)
	  spell = skill_lookup("detect invis");
	else if (number_percent() < 65)
	  spell = skill_lookup("pass door");
	else
	  spell = skill_lookup("acute vision");
      }

    potion->value[1] = spell;
    extract_obj(obj);
    act("You brew $p from your resources!", ch, potion, NULL, TO_CHAR);
    act("$n brews $p from $s resources!", ch, potion, NULL, TO_ROOM);

    obj_to_char(potion, ch);
    extract_obj( vial );

}


void spell_shadowlife( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  CHAR_DATA *shadow;
  AFFECT_DATA af;
  int i;
  char buf[MAX_STRING_LENGTH];
  char *name;

  if (IS_NPC(victim))
    {
      send_to_char("Now, why would you want to do that?!?\n\r", ch);
      return;
    }

  if (is_affected(ch,sn))
    {
      send_to_char("You don't have the strength to raise a Shadow now.\n\r",
		   ch);
      return;
    }

  act("You give life to $N's shadow!",ch, NULL, victim, TO_CHAR);
  act("$n gives life to $N's shadow!",ch,NULL,victim,TO_NOTVICT);
  act("$n gives life to your shadow!", ch, NULL, victim, TO_VICT);
  
  shadow = create_mobile( get_mob_index(MOB_VNUM_SHADOW) );
  
  for (i=0;i < MAX_STATS; i++)
    {
      shadow->perm_stat[i] = ch->perm_stat[i];
    }
  
  shadow->max_hit = (3 * ch->max_hit) / 4;
  shadow->hit = shadow->max_hit;
  shadow->max_mana = (3 * ch->max_mana) / 4;
  shadow->mana = shadow->max_mana;
  shadow->alignment = ch->alignment;
  shadow->level = ch->level;
  for (i=0; i < 3; i++)
    shadow->armor[i] = interpolate(shadow->level,100,-100);
  shadow->armor[3] = interpolate(shadow->level,100,0);
  shadow->sex = victim->sex;
  shadow->gold = 0;

  name		= IS_NPC(victim) ? victim->short_descr : victim->name;
  sprintf( buf, shadow->short_descr, name );
  free_string( shadow->short_descr );
  shadow->short_descr = str_dup( buf );

  sprintf( buf, shadow->long_descr, name );
  free_string( shadow->long_descr );
  shadow->long_descr = str_dup( buf );
  
  sprintf( buf, shadow->description, name );
  free_string( shadow->description );
  shadow->description = str_dup( buf );
  
  char_to_room(shadow,ch->in_room);

  do_murder(shadow, victim->name);

  af.where		= TO_AFFECTS;
  af.type               = sn;
  af.level              = level; 
  af.duration           = 24;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);  

}  

void spell_ruler_badge( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  OBJ_DATA *badge;
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  OBJ_DATA *obj_next;
  AFFECT_DATA af;

  if (count_worn(ch, WEAR_NECK) >= max_can_wear(ch, WEAR_NECK) )
  {
    send_to_char( "But you are wearing something else.\n\r", ch );
    return;
  }

  for ( badge = ch->carrying; badge != NULL;
       badge = obj_next )
    {
      obj_next = badge->next_content;
      if (badge->pIndexData->vnum == OBJ_VNUM_DEPUTY_BADGE 
	  || badge->pIndexData->vnum == OBJ_VNUM_RULER_BADGE)
	{
	  act("Your $p vanishes.",ch, badge, NULL, TO_CHAR);
	  obj_from_char(badge);
	  extract_obj(badge);
	  continue;
	}
    }


  badge = create_object( get_obj_index(OBJ_VNUM_RULER_BADGE),level);

  af.where        = TO_OBJECT;
  af.type         = sn;
  af.level        = level;
  af.duration     = -1;
  af.modifier     = 100 + level / 2;
  af.bitvector    = 0;

  af.location     = APPLY_HIT;
  affect_to_obj(badge,&af);

  af.location     = APPLY_MANA;
  affect_to_obj(badge,&af);

  af.where        = TO_OBJECT;
  af.type         = sn;
  af.level        = level;
  af.duration     = -1;
  af.modifier     = level / 8;
  af.bitvector    = 0;

  af.location     = APPLY_HITROLL;
  affect_to_obj(badge,&af);

  af.location     = APPLY_DAMROLL;
  affect_to_obj(badge,&af);


  badge->timer = 200;
  act("You wear the ruler badge!",ch, NULL, NULL, TO_CHAR);
  act("$n wears the $s ruler badge!", ch, NULL, NULL, TO_ROOM);

  obj_to_char(badge,victim);
  equip_char( ch, badge, WEAR_NECK );
  ch->hit = UMIN( (ch->hit + 100 + level/2), ch->max_hit);
  ch->mana = UMIN( (ch->mana + 100 + level/2), ch->max_mana);

}    

void spell_remove_badge( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  OBJ_DATA *badge;
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  OBJ_DATA *obj_next;

  badge = 0;

  for ( badge = victim->carrying; badge != NULL;
       badge = obj_next )
    {
      obj_next = badge->next_content;
      if (badge->pIndexData->vnum == OBJ_VNUM_DEPUTY_BADGE 
	  || badge->pIndexData->vnum == OBJ_VNUM_RULER_BADGE)
	{
	  act("Your $p vanishes.",ch, badge, NULL, TO_CHAR);
	  act("$n's $p vanishes.", ch, badge, NULL, TO_ROOM);
	  
	  obj_from_char(badge);
	  extract_obj(badge);
	  continue;
	}
    }
  return;
}

void spell_dragon_strength(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
  AFFECT_DATA af;

  if (is_affected(ch, sn))
    {
      send_to_char("You are already full of the strength of the dragon.\n\r",
		ch);
      return;
    }

  af.where		= TO_AFFECTS;
  af.type = sn;
  af.level = level;
  af.duration = level / 3;
  af.bitvector = 0;

  af.modifier = 2;
  af.location = APPLY_HITROLL;
  affect_to_char(ch, &af);

  af.modifier = 2;
  af.location = APPLY_DAMROLL;
  affect_to_char(ch, &af);

  af.modifier = 10;
  af.location = APPLY_AC;
  affect_to_char(ch, &af);

  af.modifier = 2;
  af.location = APPLY_STR;
  affect_to_char(ch, &af);

  af.modifier = -2;
  af.location = APPLY_DEX;
  affect_to_char(ch, &af);

  send_to_char("The strength of the dragon enters you.\n\r", ch);
  act("$n looks a bit meaner now.", ch, NULL, NULL, TO_ROOM);
}

void spell_dragon_breath(int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  int dam;

  dam = dice(level , 6);
  if (!is_safe_spell(ch, victim, TRUE))
    {
      if (saves_spell(level, victim, DAM_FIRE))
	dam /= 2;
      damage(ch, victim, dam, sn, DAM_FIRE, TRUE);
    }
}

void spell_golden_aura( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  AFFECT_DATA af;
  CHAR_DATA *vch = (CHAR_DATA *)vo;

  for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
    {
      if (!is_same_group(vch, ch)) 
	continue;

      if ( is_affected( vch, sn ) || is_affected(vch, gsn_bless) || 
	  IS_AFFECTED(vch, AFF_PROTECT_EVIL))
	{
	  if (vch == ch)
	    send_to_char("You are already protected by a golden aura.\n\r",ch);
	  else
	    act("$N is already protected by a golden aura.",ch,NULL,vch,TO_CHAR);
	  continue;
	}
	
      af.where		= TO_AFFECTS;
      af.type      = sn;
      af.level	 = level;
      af.duration  = 6 + level;
      af.modifier  = 0;
      af.location  = APPLY_NONE;
      af.bitvector = AFF_PROTECT_EVIL;
      affect_to_char( vch, &af );
      
      af.modifier = level/8;
      af.location = APPLY_HITROLL;
      af.bitvector = 0;
      affect_to_char(vch, &af);
      
      af.modifier = 0 - level/8;
      af.location = APPLY_SAVING_SPELL;
      affect_to_char(vch, &af);

      send_to_char( "You feel a golden aura around you.\n\r", vch );
      if ( ch != vch )
	act("A golden aura surrounds $N.",ch,NULL,vch,TO_CHAR);

    }
}

void spell_dragonplate( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  int plate_vnum;
  OBJ_DATA *plate;
  AFFECT_DATA af;
  
  plate_vnum = OBJ_VNUM_PLATE;

  plate = create_object( get_obj_index(plate_vnum), level + 5);
  plate->timer = 2 * level;
  plate->cost  = 0;
  plate->level  = ch->level;

  af.where        = TO_OBJECT;
  af.type         = sn;
  af.level        = level;
  af.duration     = -1;
  af.modifier     = level / 8;
  af.bitvector    = 0;

  af.location     = APPLY_HITROLL;
  affect_to_obj(plate,&af);

  af.location     = APPLY_DAMROLL;
  affect_to_obj(plate,&af);

  obj_to_char(plate, ch);
  
  act("You create $p!",ch,plate,NULL,TO_CHAR);
  act("$n creates $p!",ch,plate,NULL,TO_ROOM);
}

void spell_squire( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  CHAR_DATA *gch;
  CHAR_DATA *squire;
  AFFECT_DATA af;
  int i;
  char buf[MAX_STRING_LENGTH];

  if (is_affected(ch,sn))
    {
      send_to_char("You cannot command another squire right now.\n\r",
		   ch);
      return;
    }

  send_to_char("You attempt to summon a squire.\n\r",ch);
  act("$n attempts to summon a squire.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  gch->pIndexData->vnum == MOB_VNUM_SQUIRE)
	{
	  send_to_char("Two squires are more than you need!\n\r",ch);
	  return;
	}
    }

  if (count_charmed(ch)) return;

  squire = create_mobile( get_mob_index(MOB_VNUM_SQUIRE) );

  for (i=0;i < MAX_STATS; i++)
    {
      squire->perm_stat[i] = ch->perm_stat[i];
    }

  squire->max_hit = ch->max_hit;
  squire->hit = squire->max_hit;
  squire->max_mana = ch->max_mana;
  squire->mana = squire->max_mana;
  squire->level = ch->level;
  for (i=0; i < 3; i++)
    squire->armor[i] = interpolate(squire->level,100,-100);
  squire->armor[3] = interpolate(squire->level,100,0);
  squire->gold = 0;

  sprintf( buf, squire->short_descr, ch->name );
  free_string( squire->short_descr );
  squire->short_descr = str_dup( buf );

  sprintf( buf, squire->long_descr, ch->name );
  free_string( squire->long_descr );
  squire->long_descr = str_dup( buf );
  
  sprintf( buf, squire->description, ch->name );
  free_string( squire->description );
  squire->description = str_dup( buf ); 
  
  squire->damage[DICE_NUMBER] = number_range(level/15, level/12);   
  squire->damage[DICE_TYPE] = number_range(level/3, level/2);
  squire->damage[DICE_BONUS] = number_range(level/8, level/6);

  char_to_room(squire,ch->in_room);
  send_to_char("A squire arrives from nowhere!\n\r",ch);
  act("A squire arrives from nowhere!",ch,NULL,NULL,TO_ROOM);

  af.where		= TO_AFFECTS;
  af.type               = sn;
  af.level              = level; 
  af.duration           = 24;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);  

  SET_BIT(squire->affected_by, AFF_CHARM);
  squire->master = squire->leader = ch;
  
}


void spell_dragonsword( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  int sword_vnum;
  OBJ_DATA *sword;
  char arg[MAX_INPUT_LENGTH];
  AFFECT_DATA af;
  
  target_name = one_argument(target_name, arg);
  sword_vnum = 0;

  if (!str_cmp(arg, "sword"))
    sword_vnum = OBJ_VNUM_DRAGONSWORD;
  else if (!str_cmp(arg, "mace"))
    sword_vnum = OBJ_VNUM_DRAGONMACE;
  else if (!str_cmp(arg, "dagger"))
    sword_vnum = OBJ_VNUM_DRAGONDAGGER;
  else if (!str_cmp(arg, "lance"))
    sword_vnum = OBJ_VNUM_DRAGONLANCE;
  else
    {
      send_to_char("You can't make a DragonSword like that!", ch);
      return;
    }

  sword = create_object( get_obj_index(sword_vnum), level);
  sword->timer = level * 2;
  sword->cost  = 0;
  if (ch->level  < 50 )
  	sword->value[2] = (ch->level / 10);
  else sword->value[2] = (ch->level / 6 ) - 3;
  sword->level = ch->level;

  af.where        = TO_OBJECT;
  af.type         = sn;
  af.level        = level;
  af.duration     = -1;
  af.modifier     = level / 5;
  af.bitvector    = 0;

  af.location     = APPLY_HITROLL;
  affect_to_obj(sword,&af);

  af.location     = APPLY_DAMROLL;
  affect_to_obj(sword,&af);

  if (IS_GOOD(ch))
	 SET_BIT(sword->extra_flags,(ITEM_ANTI_NEUTRAL | ITEM_ANTI_EVIL));
  else if (IS_NEUTRAL(ch))
	 SET_BIT(sword->extra_flags,(ITEM_ANTI_GOOD | ITEM_ANTI_EVIL));
  else if (IS_EVIL(ch))
	 SET_BIT(sword->extra_flags,(ITEM_ANTI_NEUTRAL | ITEM_ANTI_GOOD));	
  obj_to_char(sword, ch);
  
  act("You create $p!",ch,sword,NULL,TO_CHAR);
  act("$n creates $p!",ch,sword,NULL,TO_ROOM);
}

void spell_entangle( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  int dam;
  
  if (ch->in_room->sector_type == SECT_INSIDE || 
      ch->in_room->sector_type == SECT_CITY || 
      ch->in_room->sector_type == SECT_DESERT || 
      ch->in_room->sector_type == SECT_AIR)
    {
      send_to_char("No plants can grow here.\n\r", ch);
      return;
    }
      
  dam = number_range(level, 4 * level);
  if ( saves_spell( level, victim, DAM_PIERCE ) )
    dam /= 2;
  
  damage(ch,victim,ch->level,gsn_entangle,DAM_PIERCE, TRUE);
  
  act("The thorny plants spring up around $n, entangling $s legs!", victim, 
      NULL, NULL, TO_ROOM);
  act("The thorny plants spring up around you, entangling your legs!", victim,
      NULL, NULL, TO_CHAR);

  victim->move -= dice(level, 6);
  victim->move = UMAX(0, victim->move);

  if (!is_affected(victim,gsn_entangle))
    {
      AFFECT_DATA todex;
      
      todex.type = gsn_entangle;
      todex.level = level;
      todex.duration = level / 10;
      todex.location = APPLY_DEX;
      todex.modifier = - (level / 10);
      todex.bitvector = 0;
      affect_to_char( victim, &todex);
      
    }
  return;
}

void spell_holy_armor( int sn, int level, CHAR_DATA *ch, void *vo, int target ) 
{
  AFFECT_DATA af;
   
  if ( is_affected( ch, sn ) )
    {
      send_to_char("You are already protected from harm.",ch);
      return;
    }

  af.where	= TO_AFFECTS;
  af.type      = sn;
  af.level     = level;
  af.duration  = level;
  af.location  = APPLY_AC;
  af.modifier  = - UMAX(10, 10 * (level / 5) );
  af.bitvector = 0;
  affect_to_char( ch, &af );
  act( "$n is protected from harm.",ch,NULL,NULL,TO_ROOM );
  send_to_char( "Your are protected from harm.\n\r", ch );

}  

void spell_love_potion( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  AFFECT_DATA af;

  af.where		= TO_AFFECTS;
  af.type               = sn;
  af.level              = level; 
  af.duration           = 50;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);  

  send_to_char("You feel like looking at people.\n\r", ch);
}

void spell_protective_shield( int sn, int level, CHAR_DATA *ch, void *vo, int target ) {
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  AFFECT_DATA af;
   
  if ( is_affected( victim, sn ) )
    {
      if (victim == ch)
       	send_to_char("You are already surrounded by a protective shield.\n\r",
		     ch); 
      else
       	act("$N is already surrounded by a protective shield.",ch,NULL,
	    victim,TO_CHAR);
      return;
    }
  af.where	= TO_AFFECTS;
  af.type      = sn;
  af.level     = level;
  af.duration  = number_fuzzy( level / 30 ) + 3;
  af.location  = APPLY_AC;
  af.modifier  = 20;
  af.bitvector = 0;
  affect_to_char( victim, &af );
  act( "$n is surrounded by a protective shield.",victim,NULL,NULL,TO_ROOM );
  send_to_char( "You are surrounded by a protective shield.\n\r", victim );
  return;
}
	
void spell_deafen( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  AFFECT_DATA af;

  if (ch == victim) {
    send_to_char("Deafen who?\n\r",ch);
    return;
  }

  if (is_affected(victim,sn)) {
    act("$N is already deaf.",ch,NULL,victim,TO_CHAR);
    return;
  }

  if (is_safe_nomessage(ch,victim)) {
      send_to_char("You cannot deafen that person.\n\r",ch);
      return;
  }

  if (saves_spell(level,victim, DAM_NONE))
    return;

  af.where		= TO_AFFECTS;
  af.type      = sn;
  af.level     = level;
  af.duration  = 10;
  af.modifier  = 0;
  af.location  = 0;
  af.bitvector = 0;
  affect_to_char(victim,&af);

  act("You have deafened $N!",ch,NULL,victim,TO_CHAR);
  send_to_char("A loud ringing fills your ears...you can't hear anything!\n\r",
	       victim);
}

void spell_disperse( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *vch;
  CHAR_DATA *vch_next;
  ROOM_INDEX_DATA *pRoomIndex;
  AFFECT_DATA af;

  if ( is_affected( ch, sn ) )
    {
      send_to_char("You aren't up to dispersing this crowd.\n\r",ch);
      return;
    }

  for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
      vch_next = vch->next_in_room;

      if ( vch->in_room != NULL
      &&   !IS_SET(vch->in_room->room_flags, ROOM_NO_RECALL)
      &&   !IS_IMMORTAL(vch)
      && (( IS_NPC(vch) && !IS_SET(vch->act, ACT_AGGRESSIVE) ) ||
/*      (!IS_NPC(vch) && vch->level > PK_MIN_LEVEL && (vch->level < level || */
      (!IS_NPC(vch) && vch->level > PK_MIN_LEVEL && (
	!is_safe_nomessage(ch, vch)))) && vch != ch
      && !IS_SET(vch->imm_flags, IMM_SUMMON))
	{
	  for ( ; ; )
  	    {
	      pRoomIndex = get_room_index( number_range( 0, 65535 ) );
	      if ( pRoomIndex != NULL )
	        if ( can_see_room(ch,pRoomIndex)
	    	&&   !room_is_private(pRoomIndex) && 
		  !IS_SET(pRoomIndex->room_flags, ROOM_NO_RECALL))
	      break;
	    }

	  send_to_char("The world spins around you!\n\r",vch);
	  act( "$n vanishes!", vch, NULL, NULL, TO_ROOM );
	  char_from_room( vch );
	  char_to_room( vch, pRoomIndex );
	  act( "$n slowly fades into existence.", vch, NULL, NULL, TO_ROOM );
	  do_look( vch, "auto" );
	}
    }
  af.where		= TO_AFFECTS;
  af.type      = sn;
  af.level	 = level;
  af.duration  = 10;
  af.modifier  = 0;
  af.location  = APPLY_NONE;
  af.bitvector = 0;
  affect_to_char( ch, &af );

}


void spell_honor_shield( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  AFFECT_DATA af;

  if (is_affected(victim, sn))
    {
      if (victim == ch)
        send_to_char("But you're already protected by your honor.\n\r", ch);
      else
	send_to_char("They're already protected by their honor.\n\r", ch);
      return;
    }

  af.where		= TO_AFFECTS;
  af.type      = sn;
  af.level	 = level;
  af.duration  = 24;
  af.modifier  = -30;
  af.location  = APPLY_AC;
  af.bitvector = 0;
  affect_to_char( victim, &af );

  spell_remove_curse(skill_lookup("remove curse"), level, ch, victim, TARGET_CHAR);
  spell_bless(skill_lookup("bless"), level, ch, victim, TARGET_CHAR);

  send_to_char("Your honor protects you.\n\r", victim);
  act("$n's Honor protects $m.", victim, NULL, NULL, TO_ROOM);
}
  
void spell_acute_vision( int sn, int level, CHAR_DATA *ch, void *vo, int target )
 {
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( CAN_DETECT(victim, ACUTE_VISION) )
    {
        if (victim == ch)
          send_to_char("Your vision is already acute. \n\r",ch);
        else
          act("$N already sees acutely.",ch,NULL,victim,TO_CHAR);
        return;
    }
    af.where		= TO_DETECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = level;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = ACUTE_VISION;
    affect_to_char( victim, &af );
    send_to_char( "Your vision sharpens.\n\r", victim );
    if ( ch != victim )
        send_to_char( "Ok.\n\r", ch );
    return;
}

void spell_dragons_breath(int sn,int level,CHAR_DATA *ch, void *vo,int target ) 
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    CHAR_DATA *vch, *vch_next;
    int dam,hp_dam,dice_dam;
    int hpch;
    char buf[MAX_STRING_LENGTH];

    act("You call the dragon lord to help you.",ch,NULL,NULL,TO_CHAR);
    act("$n start to breath like a dragon.",ch,NULL,victim,TO_NOTVICT);
    act("$n breath disturbs you!",ch,NULL,victim,TO_VICT);
    act("You breath the breath of lord of Dragons.",ch,NULL,NULL,TO_CHAR);

    hpch = UMAX( 10, ch->hit );
    hp_dam  = number_range( hpch/9+1, hpch/5 );
    dice_dam = dice(level,20);

    dam = UMAX(hp_dam + dice_dam / 5, dice_dam + hp_dam / 5);
    
switch( dice(1,5) )
 {
	 case 1:
    fire_effect(victim->in_room,level,dam/2,TARGET_ROOM);

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
    break;
    case 2:
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
    break;
    case 3:
    cold_effect(victim->in_room,level,dam/2,TARGET_ROOM); 

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
    break;
    case 4:
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
    break;
    case 5:
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
    break;
  }
}

void spell_sand_storm( int sn, int level, CHAR_DATA *ch, void *vo,int target )
 {
    CHAR_DATA *vch, *vch_next;
    int dam,hp_dam,dice_dam;
    int hpch;

    if ( (ch->in_room->sector_type == SECT_AIR)
	|| (ch->in_room->sector_type == SECT_WATER_SWIM)
	|| (ch->in_room->sector_type == SECT_WATER_NOSWIM) )
	{
	 send_to_char("You don't find any sand here to make storm.\n\r",ch);
	 ch->wait = 0;
	 return;
	}

    act("$n creates a storm with sands on the floor.",ch,NULL,NULL,TO_ROOM);
    act("You create the ..sand.. storm.",ch,NULL,NULL,TO_CHAR);

    hpch = UMAX( 10, ch->hit );
    hp_dam  = number_range( hpch/9+1, hpch/5 );
    dice_dam = dice(level,20);

    dam = UMAX(hp_dam + dice_dam /10, dice_dam + hp_dam / 10);
    sand_effect(ch->in_room,level,dam/2,TARGET_ROOM);

    for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
    {
	vch_next = vch->next_in_room;

	if (is_safe_spell(ch,vch,TRUE)
	||  (IS_NPC(vch) && IS_NPC(ch)
	&&   (ch->fighting != vch /*|| vch->fighting != ch*/)))
	    continue;
	if ( is_safe(ch, vch) )
          continue;

	    if (saves_spell(level,vch,DAM_COLD))
	    {
		sand_effect(vch,level/2,dam/4,TARGET_CHAR);
		damage(ch,vch,dam/2,sn,DAM_COLD,TRUE);
	    }
	    else
	    {
		sand_effect(vch,level,dam,TARGET_CHAR);
		damage(ch,vch,dam,sn,DAM_COLD,TRUE);
	    }
    }
}

void spell_scream( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *vch, *vch_next;
    int dam=0,hp_dam,dice_dam;
    int hpch;

    act("$n screames with a disturbing NOISE!.",ch,NULL,NULL,TO_ROOM);
    act("You scream with a powerful sound.",ch,NULL,NULL,TO_CHAR);

    hpch = UMAX( 10, ch->hit );
    hp_dam  = number_range( hpch/9+1, hpch/5 );
    dice_dam = dice(level,20);
    dam = UMAX(hp_dam + dice_dam /10 , dice_dam + hp_dam /10);

    scream_effect(ch->in_room,level,dam/2,TARGET_ROOM);

    for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
    {
	vch_next = vch->next_in_room;

	if (is_safe_spell(ch,vch,TRUE))
	    continue;
	if ( is_safe(ch, vch) )
          continue;

	    if (saves_spell(level,vch,DAM_ENERGY))
	    {
		WAIT_STATE(vch, PULSE_VIOLENCE );
		scream_effect(vch,level/2,dam/4,TARGET_CHAR);
/*		damage(ch,vch,dam/2,sn,DAM_ENERGY,TRUE);
	     if (vch->fighting)  stop_fighting( vch , TRUE ); */
	    }
	    else
	    {
		WAIT_STATE(vch, ( skill_table[sn].beats + PULSE_VIOLENCE ));
		scream_effect(vch,level,dam,TARGET_CHAR);
/*		damage(ch,vch,dam,sn,DAM_ENERGY,TRUE); */
	     if (vch->fighting)  stop_fighting( vch , TRUE );
	    }
    }
}

void spell_attract_other( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;

    if  ( ch->sex == victim->sex ) 
    {
	send_to_char( "You'd better try your chance on other sex!\n\r", ch );
	return;
    }
    spell_charm_person(sn,level,ch,vo,target);
    return;
}



void spell_vampire( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    AFFECT_DATA af;
 
    if ( is_affected( ch, sn ) )
    {
     send_to_char("You can't be much more vampire!\n\r",ch);
     return;
    }
/* haste */
    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = level/2;
    af.location  = APPLY_DEX;
    af.modifier  = 1 + (level >= 18) + (level >= 25) + (level >= 32);
    af.bitvector = AFF_HASTE;
    affect_to_char( ch, &af );

/* giant strength */
    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = level/2;
    af.location  = APPLY_STR;
    af.modifier  = 1 + (level >= 18) + (level >= 25) + (level >= 32);
    af.bitvector = 0;
    affect_to_char( ch, &af );

/* cusse */
    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = level/2;
    af.location  = APPLY_SIZE;
    af.modifier  = 1 + (level >= 25) + (level >= 50) + (level >= 75);
    af.bitvector = AFF_SNEAK;
    affect_to_char( ch, &af );

/* damroll */
    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = level/2;
    af.location  = APPLY_DAMROLL;
    af.modifier  = ch->damroll;
    af.bitvector = AFF_BERSERK;
    affect_to_char( ch, &af );

/* vampire flag */
    af.where     = TO_ACT_FLAG;
    af.type      = sn;
    af.level     = level;
    af.duration  = level/2;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = PLR_VAMPIRE;
    affect_to_char( ch, &af );

   send_to_char( "You feel yourself getting greater and greater.\n\r", ch );
   act("You cannot recognize $n anymore.",ch,NULL,NULL,TO_ROOM);
   return;
}




void spell_animate_dead(int sn,int level, CHAR_DATA *ch, void *vo,int target )
 {
    CHAR_DATA *victim;
    CHAR_DATA *undead;
    OBJ_DATA *obj,*obj2,*next;
    AFFECT_DATA af;
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char buf3[MAX_STRING_LENGTH];
    char *argument,*arg;
    int i;

    /* deal with the object case first */
    if (target == TARGET_OBJ)
    {
	obj = (OBJ_DATA *) vo;

      if (!(obj->item_type == ITEM_CORPSE_NPC ||  
          obj->item_type == ITEM_CORPSE_PC))
    {
	send_to_char("You can animate only corpses!\n\r",ch);
	return;
    }
/*
     if (obj->item_type == ITEM_CORPSE_PC)     
	{
	send_to_char("The magic fails abruptly!\n\r",ch);
	return;
	}
*/
  if (is_affected(ch, sn))
    {
      send_to_char("You cannot summon the strength to handle more undead bodies.\n\r", ch);
      return;
    }

  if (count_charmed(ch)) return;

  if ( ch->in_room != NULL && IS_SET(ch->in_room->room_flags, ROOM_NO_MOB) )
  {
     send_to_char( "You can't animate deads here.\n\r", ch );
     return;
  }

  if ( IS_SET(ch->in_room->room_flags, ROOM_SAFE)      ||
       IS_SET(ch->in_room->room_flags, ROOM_PRIVATE)   ||
       IS_SET(ch->in_room->room_flags, ROOM_SOLITARY)  )
  {
    send_to_char( "You can't animate here.\n\r", ch );
    return;
  }

  undead = create_mobile( get_mob_index(MOB_VNUM_UNDEAD) );
  char_to_room(undead,ch->in_room);
  for (i=0;i < MAX_STATS; i++)
    {
      undead->perm_stat[i] = UMIN(25,2 * ch->perm_stat[i]);
    }

  undead->max_hit = IS_NPC(ch)? ch->max_hit : ch->pcdata->perm_hit;
  undead->hit = undead->max_hit;
  undead->max_mana = IS_NPC(ch)? ch->max_mana : ch->pcdata->perm_mana;
  undead->mana = undead->max_mana;
  undead->alignment = ch->alignment;
  undead->level = UMIN(100,(ch->level-2));

  for (i=0; i < 3; i++)
    undead->armor[i] = interpolate(undead->level,100,-100);
  undead->armor[3] = interpolate(undead->level,50,-200);
  undead->damage[DICE_NUMBER] = number_range(level/20,level/15);
  undead->damage[DICE_TYPE] = number_range(level/6, level/3);
  undead->damage[DICE_BONUS] = number_range(level/12, level/10);
  undead->sex = ch->sex;
  undead->gold = 0;
  
  SET_BIT(undead->act, ACT_UNDEAD);
  SET_BIT(undead->affected_by, AFF_CHARM);
  undead->master = ch;
  undead->leader = ch;

  sprintf(buf, "%s body undead", obj->name);
  undead->name = str_dup(buf);
  sprintf(buf2,"%s",obj->short_descr);
  argument = (char*)alloc_perm ( MAX_STRING_LENGTH );
  arg = (char*)alloc_perm ( MAX_STRING_LENGTH );
  argument = buf2;
  buf3[0] = '\0';
  while (argument[0] != '\0' )
	{
  argument = one_argument(argument, arg);
  if (!(!str_cmp(arg,"The") || !str_cmp(arg,"undead") || !str_cmp(arg,"body") ||
	!str_cmp(arg,"corpse") || !str_cmp(arg,"of") ))
	 {
	  if (buf3[0] == '\0')   strcat(buf3,arg);
	  else  {
		 strcat(buf3," ");
		 strcat(buf3,arg);
		}
	 }
	}
  sprintf(buf, "The undead body of %s", buf3);
  undead->short_descr = str_dup(buf);
  sprintf(buf, "The undead body of %s slowly staggers around.\n\r", buf3);
  undead->long_descr = str_dup(buf);

  for(obj2 = obj->contains;obj2;obj2=next)
    {
	next = obj2->next_content;
	obj_from_obj(obj2);
	obj_to_char(obj2, undead);
    }
  interpret( undead,"wear all", TRUE);

  af.where	 = TO_AFFECTS;
  af.type      = sn;
  af.level	 = ch->level;
  af.duration  = (ch->level / 10);
  af.modifier  = 0;
  af.bitvector = 0;
  af.location  = APPLY_NONE;
  affect_to_char( ch, &af );

  send_to_char("With mystic power, you animate it!\n\r",ch);
  sprintf(buf,"With mystic power, %s animates %s!",ch->name,obj->name); 
  act(buf,ch,NULL,NULL,TO_ROOM);
  sprintf(buf,"%s looks at you and plans to make you pay for distrurbing its rest!",obj->short_descr); 
  act(buf,ch,NULL,NULL,TO_CHAR);
  extract_obj (obj);
	return;
    }

    victim = (CHAR_DATA *) vo;

    if ( victim == ch )
    {
	send_to_char( "But you aren't dead!!\n\r", ch );
	return;
    }

    send_to_char( "But it ain't dead!!\n\r", ch );
    return;
}



void spell_enhanced_armor( int sn, int level, CHAR_DATA *ch, void *vo, int target ) 
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
    {
	if (victim == ch)
	  send_to_char("You are already enhancedly armored.\n\r",ch);
	else
	  act("$N is already enhancedly armored.",ch,NULL,victim,TO_CHAR);
	return;
    }
    af.where	 = TO_AFFECTS;
    af.type      = sn;
    af.level	 = level;
    af.duration  = 24;
    af.modifier  = -60;
    af.location  = APPLY_AC;
    af.bitvector = 0;
    affect_to_char( victim, &af );
    send_to_char( "You feel protected for all attacks.\n\r", victim );
    if ( ch != victim )
	act("$N is protected by your magic.",ch,NULL,victim,TO_CHAR);
    return;
}



void spell_meld_into_stone( int sn, int level, CHAR_DATA *ch, void *vo, int target ) 
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  AFFECT_DATA af;
   
  if ( is_affected( victim, sn ) )
    {
      if (victim == ch)
       	send_to_char("Your skin is already covered with stone.\n\r",
		     ch); 
      else
       	act("$N's skin is already covered with stone.",ch,NULL,
	    victim,TO_CHAR);
      return;
    }
  af.where	= TO_AFFECTS;
  af.type      = sn;
  af.level     = level;
  af.duration  = level / 10;
  af.location  = APPLY_AC;
  af.modifier  = -100;
  af.bitvector = 0;
  affect_to_char( victim, &af );
  act( "$n's skin melds into stone.",victim,NULL,NULL,TO_ROOM );
  send_to_char( "Your skin melds into stone.\n\r", victim );
  return;
}
	
void spell_web( int sn, int level, CHAR_DATA *ch, void *vo , int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

     if (saves_spell (level, victim,DAM_OTHER) ) return; 

    if ( is_affected( victim, sn ) )
    {
	if (victim == ch)
	  send_to_char("You are already webbed.\n\r",ch);
	else
	  act("$N is already webbed.",ch,NULL,victim,TO_CHAR);
	return;
    }

    af.type      = sn;
    af.level	 = level;
    af.duration  = 1;
    af.location  = APPLY_HITROLL;
    af.modifier  = -1 * ( level / 6); 
    af.where	 = TO_DETECTS;
    af.bitvector = ADET_WEB;
    affect_to_char( victim, &af );

    af.location  = APPLY_DEX;
    af.modifier  = -2;
    affect_to_char( victim, &af );

    af.location  = APPLY_DAMROLL;
    af.modifier  = -1 * ( level / 6);  
    affect_to_char( victim, &af );
    send_to_char( "You are emeshed in thick webs!\n\r", victim );
    if ( ch != victim )
	act("You emesh $N in a bundle of webs!",ch,NULL,victim,TO_CHAR);
    return;
}


void spell_group_defense( int sn, int level, CHAR_DATA *ch, void *vo ,int target) 
{
	CHAR_DATA *gch;
	AFFECT_DATA af;
	int shield_sn, armor_sn;

	shield_sn = skill_lookup("shield");
	armor_sn = skill_lookup("armor");

	for( gch=ch->in_room->people; gch != NULL; gch=gch->next_in_room)
	{
		if( !is_same_group( gch, ch))
			continue;
		if( is_affected( gch, armor_sn ) )
		{
	    	    if( gch == ch)
			send_to_char("You are already armored.\n\r",ch);
		    else
			act( "$N is already armored.", ch, NULL, gch, TO_CHAR);
		  continue;
		}

		af.type      = armor_sn;
		af.level     = level;
		af.duration  = level;
		af.location  = APPLY_AC;
		af.modifier  = -20;
		af.bitvector = 0;
		affect_to_char( gch, &af );

		send_to_char( "You feel someone protecting you.\n\r",gch);
		if( ch != gch )
			act("$N is protected by your magic.",
				ch, NULL, gch, TO_CHAR );
		
		if( !is_same_group( gch, ch ) )
			continue;
		if( is_affected( gch, shield_sn ) )
		{
		  if( gch == ch) 	
			send_to_char("You are already shielded.\n\r",ch);
		  else 
		        act( "$N is already shielded.", ch, NULL, gch, TO_CHAR);
		  continue;
		}

		af.type      = shield_sn;
		af.level     = level;
		af.duration  = level;
		af.location  = APPLY_AC;
		af.modifier   = -20;
		af.bitvector = 0;
		affect_to_char( gch, &af );

		send_to_char( "You are surrounded by a force shield.\n\r",gch);
		if( ch != gch )
			act("$N is surrounded by a force shield.",
				ch, NULL, gch, TO_CHAR );

	}
	return;
}


void spell_inspire( int sn, int level, CHAR_DATA *ch, void *vo,int target) 
{
	CHAR_DATA *gch;
	AFFECT_DATA af;
	int bless_sn;

	bless_sn = skill_lookup("bless");

	for( gch=ch->in_room->people; gch != NULL; gch=gch->next_in_room )
	{
		if( !is_same_group( gch, ch) )
			continue;
		if ( is_affected(gch, bless_sn ) )
		{
		  if(gch == ch)
		     send_to_char("You are already inspired.\n\r",ch);
		  else
		     act("$N is already inspired.",
			ch, NULL, gch, TO_CHAR);
		  continue;
		}
		af.type      = bless_sn;
		af.level     = level;
		af.duration  = 6 + level;
		af.location  = APPLY_HITROLL;
		af.modifier  = level/12;
		af.bitvector = 0;
		affect_to_char( gch, &af );

		af.location  = APPLY_SAVING_SPELL;
		af.modifier  = 0 - level/12;
		affect_to_char( gch, &af );

		send_to_char( "You feel inspired!\n\r", gch );
		if( ch != gch )
			act( "You inspire $N with the Creator's power!",
				ch, NULL, gch, TO_CHAR );

	}
	return;
}


void spell_mass_sanctuary( int sn, int level, CHAR_DATA *ch, void *vo,int target) 
{
	CHAR_DATA *gch;
	AFFECT_DATA af;
	int sanc_sn;

	sanc_sn = skill_lookup("sanctuary");

	for( gch=ch->in_room->people; gch != NULL; gch=gch->next_in_room)
	{
		if( !is_same_group( gch, ch ) )
			continue;
		if( IS_AFFECTED(gch, AFF_SANCTUARY ) )
		{
		  if(gch == ch)
			send_to_char("You are already in sanctuary.\n\r", ch);
		  else 
		    act( "$N is already in sanctuary.", ch, NULL, gch, TO_CHAR);
		  continue;
		}

		af.type      = sanc_sn;
		af.level     = level;
		af.duration  = number_fuzzy( level/6 );
		af.location  = APPLY_NONE;
		af.modifier  = 0;
		af.bitvector = AFF_SANCTUARY;
		af.where     = TO_AFFECTS;
		affect_to_char( gch, &af );

		send_to_char( "You are surrounded by a white aura.\n\r",gch );
		if( ch != gch )
			act( "$N is surrounded by a white aura.",
				ch, NULL, gch, TO_CHAR );
	}
	return;
}

void spell_mend( int sn, int level, CHAR_DATA *ch, void *vo , int target)
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    int result,skill;

    if ( obj->condition > 99 )
    {
    send_to_char("That item is not in need of mending.\n\r",ch);
    return;
    }

    if (obj->wear_loc != -1)
    {
	send_to_char("The item must be carried to be mended.\n\r",ch);
	return;
    }

    skill = get_skill(ch,gsn_mend) / 2;
    result = number_percent ( ) + skill; 

    if (IS_OBJ_STAT(obj,ITEM_GLOW))
	  result -= 5;
    if (IS_OBJ_STAT(obj,ITEM_MAGIC))
	  result += 5;

    if (result >= 50)
    {
	act("$p glows brightly, and is whole again.  Good Job!",ch,obj,NULL,TO_CHAR);
	act("$p glows brightly, and is whole again.",ch,obj,NULL,TO_ROOM);
	obj->condition += result;
	obj->condition = UMIN( obj->condition , 100 );
	return;
    }

    else if ( result >=10)
    {
	send_to_char("Nothing seemed to happen.\n\r",ch);
	return;
    }

    else
    {
	act("$p flares blindingly... and evaporates!",ch,obj,NULL,TO_CHAR);
	act("$p flares blindingly... and evaporates!",ch,obj,NULL,TO_ROOM);
	extract_obj(obj);
	return;
    }
}

void spell_shielding( int sn, int level, CHAR_DATA *ch, void *vo ,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( saves_spell( level, victim ,DAM_NONE) )
	{
	act("$N shivers slightly, but it passes quickly.",
		ch, NULL, victim, TO_CHAR );
	send_to_char("You shiver slightly, but it passes quickly.\n\r",victim);
	return;
	}

    if (is_affected(victim, sn) )
	{
		af.type    = sn;
		af.level   = level;
		af.duration = level / 20;
		af.location = APPLY_NONE;
		af.modifier = 0;
		af.bitvector = 0;
		affect_to_char(victim, &af );
	  act("You wrap $N in more flows of Spirit.",
		ch, NULL, victim, TO_CHAR );
	  send_to_char("You feel the shielding get stronger.\n\r",victim);
	  return;
	}

    af.type	= sn;
    af.level    = level;
    af.duration = level / 15;
    af.location = APPLY_NONE;
    af.modifier	= 0;
    af.bitvector = 0;
    affect_join( victim, &af );

    send_to_char( "You feel as if you have lost touch with something.\n\r",
		victim );
    act("You shield $N from the True Source.", ch, NULL, victim, TO_CHAR);
    return;
}


void spell_link ( int sn, int level, CHAR_DATA *ch, void *vo , int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int random, tmpmana;

    random = number_percent ( );
	tmpmana = ch->mana;
	ch->mana = 0;
    ch->endur /= 2;
	tmpmana = (int)(.5 * (float)tmpmana);
    tmpmana = ( (tmpmana + random)/2 );
    victim->mana = victim->mana + tmpmana;    
}

void spell_power_kill ( int sn, int level, CHAR_DATA *ch, void *vo , int target)
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  int dam;

   
    act_color( "A stream of darkness from your finger surrounds $N.", 
		ch, NULL, victim, TO_CHAR, POS_RESTING, CLR_RED );
    act_color( "A stream of darkness from $n's finger surrounds $N.", 
		ch, NULL, victim, TO_NOTVICT, POS_RESTING, CLR_RED );
    act_color( "A stream of darkness from $N's finger surrounds you.", 
		victim, NULL, ch, TO_CHAR, POS_RESTING, CLR_RED );

    if (saves_spell(level,victim,DAM_MENTAL) || number_bits(1) == 0) 
	{
	 dam = dice( level , 24 ) ;
	 damage(ch, victim , dam , sn, DAM_MENTAL, TRUE);
	 return;
	}

    send_to_char( "You have been KILLED!\n\r", victim );

    act("$N has been killed!\n\r", ch, NULL, victim, TO_CHAR);
    act("$N has been killed!\n\r", ch, NULL, victim, TO_ROOM);

    raw_kill(victim);
    return;
}

void spell_eyed_sword ( int sn, int level, CHAR_DATA *ch, void *vo , int target) 
{
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *eyed;
    int i;
/*
    if (IS_SET(ch->quest,QUEST_EYE)
	{
	 send_to_char("You created your sword ,before.\n\r",ch);
	 return;
	}
    SET_BIT(ch->quest,QUEST_EYE);
*/
	if (IS_GOOD(ch))
		i=0;
	else if (IS_EVIL(ch))
		i=2;
	else i = 1;
	
	eyed	= create_object(get_obj_index(OBJ_VNUM_EYED_SWORD), 0);
	eyed->owner = str_dup(ch->name);
	eyed->from = str_dup(ch->name);
	eyed->altar = hometown_table[ch->hometown].altar[i];
	eyed->pit = hometown_table[ch->hometown].pit[i];
	eyed->level = ch->level;

	sprintf( buf, eyed->short_descr, ch->name );
	free_string( eyed->short_descr );
	eyed->short_descr = str_dup( buf );

	sprintf( buf, eyed->description, ch->name	 );
	free_string( eyed->description );
	eyed->description = str_dup( buf );

        sprintf( buf, eyed->pIndexData->extra_descr->description, ch->name );
        eyed->extra_descr = new_extra_descr();
        eyed->extra_descr->keyword =
		str_dup(eyed->pIndexData->extra_descr->keyword);
        eyed->extra_descr->description = str_dup( buf );
	eyed->extra_descr->next = NULL;

  	eyed->value[2] = (ch->level / 10) + 3;  
  	eyed->level = ch->level;
	eyed->cost = 0;
	obj_to_char( eyed, ch);
	send_to_char("You create YOUR sword with your name.\n\r",ch);
	send_to_char("Don't forget that you won't be able to create this weapon anymore.\n\r",ch);
 return;
}

void spell_lion_help ( int sn, int level, CHAR_DATA *ch, void *vo , int target) 
{
  CHAR_DATA *lion;
  CHAR_DATA *victim;
  AFFECT_DATA af;
  char arg[MAX_INPUT_LENGTH];
  int i;
  
  target_name = one_argument(target_name, arg);
  if (arg[0] == '\0')
	{
         send_to_char("Whom do you want to have killed.\n\r",ch);
	 return;
	}

  if ( (victim = get_char_area(ch,arg)) == NULL)
	{
	 send_to_char("Noone around with that name.\n\r",ch);
	 return;
	}
  if (is_safe_nomessage(ch,victim))
	{
	 send_to_char("God protects your victim.\n\r",ch);
	 return;
	}	

  send_to_char("You call for a hunter lion.\n\r",ch);
  act("$n shouts a hunter lion.",ch,NULL,NULL,TO_ROOM);

  if (is_affected(ch,sn))
    {
      send_to_char("You cannot summon the strength to handle more lion right now.\n\r", ch);
      return;
    }

  if ( ch->in_room != NULL && IS_SET(ch->in_room->room_flags, ROOM_NO_MOB) )
  {
     send_to_char( "No lions can listen you.\n\r", ch );
     return;
  }

  if ( IS_SET(ch->in_room->room_flags, ROOM_SAFE)      ||
       IS_SET(ch->in_room->room_flags, ROOM_PRIVATE)   ||
       IS_SET(ch->in_room->room_flags, ROOM_SOLITARY)  ||
       (ch->in_room->exit[0] == NULL &&
          ch->in_room->exit[1] == NULL &&
          ch->in_room->exit[2] == NULL &&
          ch->in_room->exit[3] == NULL &&
          ch->in_room->exit[4] == NULL &&
          ch->in_room->exit[5] == NULL) ||
         ( ch->in_room->sector_type != SECT_FIELD &&
           ch->in_room->sector_type != SECT_FOREST &&
           ch->in_room->sector_type != SECT_MOUNTAIN &&
           ch->in_room->sector_type != SECT_HILLS ) )
  {
    send_to_char( "No hunter lion can come to you.\n\r", ch );
    return;
  }

  lion = create_mobile( get_mob_index(MOB_VNUM_HUNTER) );

  for (i=0;i < MAX_STATS; i++)
    {
      lion->perm_stat[i] = UMIN(25,2 * ch->perm_stat[i]);
    }

  lion->max_hit =  UMIN(30000,(int)((float)ch->max_hit * 1.2));
  lion->hit = lion->max_hit;
  lion->max_mana = ch->max_mana;
  lion->mana = lion->max_mana;
  lion->alignment = ch->alignment;
  lion->level = UMIN(100,ch->level);
  for (i=0; i < 3; i++)
    lion->armor[i] = interpolate(lion->level,100,-100);
  lion->armor[3] = interpolate(lion->level,100,0);
  lion->sex = ch->sex;
  lion->gold = 0;
  lion->damage[DICE_NUMBER] = number_range(level/15, level/10);   
  lion->damage[DICE_TYPE] = number_range(level/3, level/2);
  lion->damage[DICE_BONUS] = number_range(level/8, level/6);
  lion->detection = (A|B|C|D|E|F|G|H|ee);
    
/*   SET_BIT(lion->affected_by, AFF_CHARM); 
  lion->master = lion->leader = ch; */

  char_to_room(lion,ch->in_room);

  send_to_char("A hunter lion comes to kill your victim!\n\r",ch);
  act("A hunter lion comes to kill $n's victim!",ch,NULL,NULL,TO_ROOM);

  af.where		= TO_AFFECTS;
  af.type               = sn;
  af.level              = ch->level; 
  af.duration           = 24;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);  
  SET_BIT(lion->act,ACT_HUNTER);
  lion->hunting = victim;
  hunt_victim(lion);

}


void spell_magic_jar ( int sn, int level, CHAR_DATA *ch, void *vo , int target) 
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    OBJ_DATA *vial;
    OBJ_DATA *fire;
    char buf[MAX_STRING_LENGTH];
    int i;

    if (victim == ch)
	{
	send_to_char("You like yourself even better.\n\r",ch);
	return;
	}

    if (IS_NPC(victim))
	{
	send_to_char("Your victim is a npc. Not necessary!.\n\r",ch);
	return;
	}

    if (saves_spell(level ,victim,DAM_MENTAL)) 
       {
        send_to_char("You failed.\n\r",ch);
        return;
       }

    for( vial=ch->carrying; vial != NULL; vial=vial->next_content )
      if ( vial->pIndexData->vnum == OBJ_VNUM_POTION_VIAL )
        break;

    if (  vial == NULL )  {
     send_to_char( "You don't have any vials to put your victim's spirit.\n\r"
			, ch );
	return;
    }
    extract_obj(vial);
    if (IS_GOOD(ch))
		i=0;
    else if (IS_EVIL(ch))
		i=2;
    else i = 1;
	
	fire	= create_object(get_obj_index(OBJ_VNUM_MAGIC_JAR), 0);
	fire->owner = str_dup(ch->name);
	fire->from = str_dup(ch->name);
	fire->altar = hometown_table[ch->hometown].altar[i];
	fire->pit = hometown_table[ch->hometown].pit[i];
	fire->level = ch->level;

	sprintf( buf, fire->name, victim->name);
	free_string( fire->name );
	fire->name = str_dup( buf );

	sprintf( buf, fire->short_descr, victim->name);
	free_string( fire->short_descr );
	fire->short_descr = str_dup( buf );

	sprintf( buf, fire->description, victim->name );
	free_string( fire->description );
	fire->description = str_dup( buf );

        sprintf( buf,fire->pIndexData->extra_descr->description, victim->name );
        fire->extra_descr = new_extra_descr();
	fire->extra_descr->keyword =
		str_dup( fire->pIndexData->extra_descr->keyword );
        fire->extra_descr->description = str_dup( buf );
	fire->extra_descr->next = NULL;

  	fire->level = ch->level;
	fire->timer = ch->level ;
	fire->cost = 0;
	obj_to_char( fire , ch );    
 SET_BIT(victim->act,PLR_NO_EXP);
 sprintf(buf,"You catch %s's spirit in to your vial.\n\r",victim->name);
 send_to_char( buf , ch);
 return;
}

void turn_spell ( int sn, int level, CHAR_DATA *ch, void *vo , int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam, align;

    if (victim != ch)
    {
	act("$n raises $s hand, and a blinding ray of light shoots forth!",
	    ch,NULL,NULL,TO_ROOM);
	send_to_char(
	   "You raise your hand and a blinding ray of light shoots forth!\n\r",
	   ch);
    }

    if (IS_GOOD(victim) || IS_NEUTRAL(victim))
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

    /* cabal guardians */
    if (IS_NPC(victim) && victim->cabal != CABAL_NONE)
	return;

    if ( (IS_NPC(victim) && victim->position != POS_DEAD) ||
	(!IS_NPC(victim) && (current_time - victim->last_death_time) > 10))
    {
     ROOM_INDEX_DATA *was_in;
     ROOM_INDEX_DATA *now_in;
     int door;

     was_in = victim->in_room;
     for ( door = 0; door < 6; door++ )
     {
	EXIT_DATA *pexit;

	if ( ( pexit = was_in->exit[door] ) == 0
	||   pexit->u1.to_room == NULL
	||   IS_SET(pexit->exit_info, EX_CLOSED)
	|| ( IS_NPC(ch)
	&&   IS_SET(pexit->u1.to_room->room_flags, ROOM_NO_MOB) ) )
	    continue;

	move_char( victim, door, FALSE );
	if ( ( now_in = victim->in_room ) == was_in )
	    continue;

	victim->in_room = was_in;
	act( "$n has fled!", victim, NULL, NULL, TO_ROOM );
	victim->in_room = now_in;

	if (IS_NPC(victim))  victim->last_fought = NULL;  

	stop_fighting( victim, TRUE );
	return;
     }
    }
    return;
}

void spell_turn ( int sn, int level, CHAR_DATA *ch, void *vo , int target)
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;

    AFFECT_DATA af;

    if ( is_affected( ch, sn ) )
    {
	send_to_char("This power is used too recently.",ch);
	return;
    }
    af.where	 = TO_AFFECTS;
    af.type      = sn;
    af.level	 = level;
    af.duration  = 5;
    af.modifier  = 0;
    af.location  = 0;
    af.bitvector = 0;
    affect_to_char( ch, &af );

    if (IS_EVIL(ch) )
    {
	send_to_char("The energy explodes inside you!\n\r",ch);
	turn_spell(sn,ch->level,ch,ch,target);
	return;
    }

    for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
    {
	vch_next = vch->next_in_room;

	if (is_safe_spell(ch,vch,TRUE))
	    continue;
	if ( is_safe(ch, vch) )
          continue;
	turn_spell(sn,ch->level,ch,vch,target);
    }
}


void spell_fear ( int sn, int level, CHAR_DATA *ch, void *vo , int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ((victim->iclass == 9) && (victim->level >=10) )
	{
	 send_to_char( "Your victim is beyond this power.\n\r", ch );
	 return; 
	}

    if (is_affected(victim,gsn_fear ) || saves_spell( level, victim,DAM_OTHER) )
	return;

    af.where     = TO_DETECTS;
    af.type      = gsn_fear;
    af.level     = level;
    af.duration  = level / 10;
    af.location  = 0;
    af.modifier  = 0;
    af.bitvector = ADET_FEAR;
    affect_to_char( victim, &af );
    send_to_char( "You are afraid as much as a rabbit.\n\r", victim );
    act("$n looks with afraid eyes.",victim,NULL,NULL,TO_ROOM);
    return;
}

void spell_protection_heat ( int sn, int level, CHAR_DATA *ch, void *vo , int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected(victim, gsn_protection_heat) )
    {
	if (victim == ch)
	  send_to_char("You are already protected from heat.\n\r",ch);
	else
	  act("$N is already protected from heat.",ch,NULL,victim,TO_CHAR);
	return;
    }

    if ( is_affected(victim, gsn_protection_cold) )
    {
	if (victim == ch)
	  send_to_char("You are already protected from cold.\n\r",ch);
	else
	  act("$N is already protected from cold.",ch,NULL,victim,TO_CHAR);
	return;
    }

    if ( is_affected(victim, gsn_fire_shield) )
    {
	if (victim == ch)
	  send_to_char("You are already using fire shield.\n\r",ch);
	else
	  act("$N is already using fire shield.",ch,NULL,victim,TO_CHAR);
	return;
    }

    af.where     = TO_AFFECTS;
    af.type      = gsn_protection_heat;
    af.level     = level;
    af.duration  = 24;
    af.location  = APPLY_SAVING_SPELL;
    af.modifier  = -1;
    af.bitvector = 0;
    affect_to_char( victim, &af );
    send_to_char( "You feel strengthed against heat.\n\r", victim );
    if ( ch != victim )
	act("$N is protected against heat.",ch,NULL,victim,TO_CHAR);
    return;
}

void spell_protection_cold ( int sn, int level, CHAR_DATA *ch, void *vo , int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected(victim, gsn_protection_cold) )
    {
	if (victim == ch)
	  send_to_char("You are already protected from cold.\n\r",ch);
	else
	  act("$N is already protected from cold.",ch,NULL,victim,TO_CHAR);
	return;
    }

    if ( is_affected(victim, gsn_protection_heat) )
    {
	if (victim == ch)
	  send_to_char("You are already protected from heat.\n\r",ch);
	else
	  act("$N is already protected from heat.",ch,NULL,victim,TO_CHAR);
	return;
    }

    if ( is_affected(victim, gsn_fire_shield) )
    {
	if (victim == ch)
	  send_to_char("You are already using fire shield.\n\r",ch);
	else
	  act("$N is already using fire shield.",ch,NULL,victim,TO_CHAR);
	return;
    }
    af.where     = TO_AFFECTS;
    af.type      = gsn_protection_cold;
    af.level     = level;
    af.duration  = 24;
    af.location  = APPLY_SAVING_SPELL;
    af.modifier  = -1;
    af.bitvector = 0;
    affect_to_char( victim, &af );
    send_to_char( "You feel strengthed against cold.\n\r", victim );
    if ( ch != victim )
	act("$N is protected against cold.",ch,NULL,victim,TO_CHAR);
    return;
}

void spell_fire_shield ( int sn, int level, CHAR_DATA *ch, void *vo , int target) 
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *fire;
    int i;

    target_name = one_argument(target_name, arg);
    if (!(!str_cmp(arg,"cold") || !str_cmp(arg,"fire")))
	{
	send_to_char("You must specify the type.\n\r",ch);
	return;
	}
    if (IS_GOOD(ch))
		i=0;
    else if (IS_EVIL(ch))
		i=2;
    else i = 1;
	
	fire	= create_object(get_obj_index(OBJ_VNUM_FIRE_SHIELD), 0);
	fire->owner = str_dup(ch->name);
	fire->from = str_dup(ch->name);
	fire->altar = hometown_table[ch->hometown].altar[i];
	fire->pit = hometown_table[ch->hometown].pit[i];
	fire->level = ch->level;

	sprintf( buf, fire->short_descr, arg);
	free_string( fire->short_descr );
	fire->short_descr = str_dup( buf );

	sprintf( buf, fire->description, arg	 );
	free_string( fire->description );
	fire->description = str_dup( buf );

        sprintf( buf, fire->pIndexData->extra_descr->description, arg );
        fire->extra_descr = new_extra_descr();
	fire->extra_descr->keyword =
		str_dup( fire->pIndexData->extra_descr->keyword );
        fire->extra_descr->description = str_dup( buf );
	fire->extra_descr->next = NULL;

  	fire->level = ch->level;
	fire->cost = 0;
	fire->timer = 5 * ch->level ;
  if (IS_GOOD(ch))
	 SET_BIT(fire->extra_flags,(ITEM_ANTI_NEUTRAL | ITEM_ANTI_EVIL));
  else if (IS_NEUTRAL(ch))
	 SET_BIT(fire->extra_flags,(ITEM_ANTI_GOOD | ITEM_ANTI_EVIL));
  else if (IS_EVIL(ch))
	 SET_BIT(fire->extra_flags,(ITEM_ANTI_NEUTRAL | ITEM_ANTI_GOOD));	
	obj_to_char( fire, ch);
 send_to_char("You create the fire shield.\n\r",ch);
 return;
}

void spell_witch_curse ( int sn, int level, CHAR_DATA *ch, void *vo , int target) 
{
  AFFECT_DATA af;
  CHAR_DATA *victim = (CHAR_DATA *) vo;

  if (is_affected(victim,gsn_witch_curse))
    {
      send_to_char("It has already underflowing with health.\n\r",ch);
      return;
    }

  if (saves_spell((level+5),victim,DAM_MENTAL) || number_bits(1) == 0) 
  {
      send_to_char("You failed!\n\r",ch);
      return;
  }

  ch->hit -=(2 * level);
  ch->hit = UMAX(ch->hit, 1);

  af.where		= TO_AFFECTS;
  af.type               = gsn_witch_curse;
  af.level              = level; 
  af.duration           = 24;
  af.location           = APPLY_HIT;
  af.modifier           = - level;
  af.bitvector          = 0;
  affect_to_char(victim,&af);


  send_to_char("Now he got the path to death.\n\r",ch);
}


void spell_knock ( int sn, int level, CHAR_DATA *ch, void *vo , int target)
{
    char arg[MAX_INPUT_LENGTH];
    int chance=0;
    int door;
    const	sh_int	rev_dir		[]		=
	{
	    2, 3, 0, 1, 5, 4
	};

    target_name = one_argument(target_name,arg);
 
    if (arg[0] == '\0')
    {
    send_to_char("Knock which door or direction.\n\r",ch);
    return;
    }

    if (ch->fighting)
    {	
	send_to_char("Wait until the fight finishes.\n\r",ch);
	return;
    }

    if ( ( door = find_door( ch, arg ) ) >= 0 )
    {
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit = ch->in_room->exit[door];
	if ( !IS_SET(pexit->exit_info, EX_CLOSED) )
	    { send_to_char( "It's already open.\n\r",      ch ); return; }
	if ( !IS_SET(pexit->exit_info, EX_LOCKED) )
	    { send_to_char( "Just try to open it.\n\r",     ch ); return; }
	if ( IS_SET(pexit->exit_info, EX_NOPASS) )
	    { send_to_char( "A mystical shield protects the exit.\n\r",ch ); 
	      return; }
    chance = ch->level / 5 + get_curr_stat(ch,STAT_INT) + get_skill(ch,sn) / 5;

    act("You knock $d, and try to open $d!",
		ch,NULL,pexit->keyword,TO_CHAR);
    act("You knock $d, and try to open $d!",
		ch,NULL,pexit->keyword,TO_ROOM);

    if (room_dark(ch->in_room))
		chance /= 2;

    /* now the attack */
    if (number_percent() < chance )
     {
	REMOVE_BIT(pexit->exit_info, EX_LOCKED);
	REMOVE_BIT(pexit->exit_info, EX_CLOSED);
	act( "$n knocks the the $d and opens the lock.", ch, NULL, 
		pexit->keyword, TO_ROOM );
	send_to_char( "You successed to open the door.\n\r", ch );

	/* open the other side */
	if ( ( to_room   = pexit->u1.to_room            ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != NULL
	&&   pexit_rev->u1.to_room == ch->in_room )
	{
	    CHAR_DATA *rch;

	    REMOVE_BIT( pexit_rev->exit_info, EX_CLOSED );
	    REMOVE_BIT( pexit_rev->exit_info, EX_LOCKED );
	    for ( rch = to_room->people; rch != NULL; rch = rch->next_in_room )
		act( "The $d opens.", rch, NULL, pexit_rev->keyword, TO_CHAR );
	}
     }
    else
     {
	act("You couldn't knock the $d!",
	    ch,NULL,pexit->keyword,TO_CHAR);
	act("$n failed to knock $d.",
	    ch,NULL,pexit->keyword,TO_ROOM);
     }
    return;
    }

  send_to_char("You can't see that here.\n\r",ch);
  return;
}


void spell_magic_resistance ( int sn, int level, CHAR_DATA *ch, void *vo , int target)
{
  AFFECT_DATA af;

    if (!is_affected(ch, sn))
    {
      send_to_char("You are now resistive to magic.\n\r", ch);

      af.where = TO_RESIST;
      af.type = sn;
      af.duration = level / 10;
      af.level = ch->level;
      af.bitvector = RES_MAGIC;
      af.location = 0;
      af.modifier = 0;
      affect_to_char(ch, &af);
    }
  else 
      send_to_char("You are already resistive to magic.\n\r", ch);
 return;
}

void spell_hallucination ( int sn, int level, CHAR_DATA *ch, void *vo , int target) 
{
 send_to_char("That spell is under construction.\n\r",ch);
 return;
}

void spell_wolf( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  CHAR_DATA *gch;
  CHAR_DATA *demon;
  AFFECT_DATA af;
  int i;

  if (is_affected(ch,sn))
    {
    send_to_char("You lack the power to summon another wolf right now.\n\r",ch);
      return;
    }

  send_to_char("You attempt to summon a wolf.\n\r",ch);
  act("$n attempts to summon a wolf.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  gch->pIndexData->vnum == MOB_VNUM_WOLF)
	{
	  send_to_char("Two wolfs are more than you can control!\n\r",ch);
	  return;
	}
    }

  if (count_charmed(ch)) return;

  demon = create_mobile( get_mob_index(MOB_VNUM_WOLF) );

  for (i=0;i < MAX_STATS; i++)
    {
      demon->perm_stat[i] = ch->perm_stat[i];
    }

  demon->max_hit = IS_NPC(ch)? URANGE(ch->max_hit,1 * ch->max_hit,30000)
		: URANGE(ch->pcdata->perm_hit,ch->hit,30000);
  demon->hit = demon->max_hit;
  demon->max_mana = IS_NPC(ch)? ch->max_mana : ch->pcdata->perm_mana;
  demon->mana = demon->max_mana;
  demon->level = ch->level;
  for (i=0; i < 3; i++)
    demon->armor[i] = interpolate(demon->level,100,-100);
  demon->armor[3] = interpolate(demon->level,100,0);
  demon->gold = 0;
  demon->timer = 0;
  demon->damage[DICE_NUMBER] = number_range(level/15, level/10);   
  demon->damage[DICE_TYPE] = number_range(level/3, level/2);
  demon->damage[DICE_BONUS] = number_range(level/6, level/5);

  char_to_room(demon,ch->in_room);
  send_to_char("The wolf arrives and bows before you!\n\r",ch);
  act("A wolf arrives from somewhere and bows!",ch,NULL,NULL,TO_ROOM);

  af.where		= TO_AFFECTS;
  af.type               = sn;
  af.level              = level; 
  af.duration           = 24;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);  

  SET_BIT(demon->affected_by, AFF_CHARM);
  demon->master = demon->leader = ch;
}

void spell_vam_blast( int sn, int level, CHAR_DATA *ch, void *vo, int target ) 
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = dice( level, 12);
    if ( saves_spell( level, victim, DAM_ACID ) )
	dam /= 2;
    damage( ch, victim, dam, sn,DAM_ACID,TRUE);
    return;
}

void spell_dragon_skin( int sn, int level, CHAR_DATA *ch, void *vo, int target ) 
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  AFFECT_DATA af;
   
  if ( is_affected( victim, sn ) )
    {
      if (victim == ch)
       	send_to_char("Your skin is already hard as rock.\n\r",
		     ch); 
      else
       	act("$N's skin is already hard as rock.",ch,NULL,
	    victim,TO_CHAR);
      return;
    }
  af.where	= TO_AFFECTS;
  af.type      = sn;
  af.level     = level;
  af.duration  = level;
  af.location  = APPLY_AC;
  af.modifier  = - (2 * level);
  af.bitvector = 0;
  affect_to_char( victim, &af );
  act( "$n's skin is now hard as rock.",victim,NULL,NULL,TO_ROOM );
  send_to_char( "Your skin is now hard as rock.\n\r", victim );
  return;
}
	

void spell_mind_light(int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    AFFECT_DATA af,af2;

    if ( is_affected_room( ch->in_room, sn ))
    {
	send_to_char("This room has already had booster of mana.\n\r",ch);
	return;
    }

    af.where     = TO_ROOM_CONST;
    af.type      = sn;
    af.level     = level;
    af.duration  = level / 30;
    af.location  = APPLY_ROOM_MANA;
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
    send_to_char( "The room starts to be filled with mind light.\n\r", ch );
    act("The room starts to be filled with $n's mind light.",ch,NULL,NULL,TO_ROOM);
    return;
}

void spell_insanity ( int sn, int level, CHAR_DATA *ch, void *vo , int target) 
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if (IS_NPC(ch))
	{
	 send_to_char("This spell can cast on PC's only.\n\r",ch);
	 return;
	}

    if (IS_AFFECTED(victim,AFF_BLOODTHIRST ) || saves_spell( level, victim,DAM_OTHER) )
	return;

    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = level / 10;
    af.location  = 0;
    af.modifier  = 0;
    af.bitvector = AFF_BLOODTHIRST;
    affect_to_char( victim, &af );
    send_to_char( "You are as aggressive as a battlerager.\n\r", victim );
    act("$n looks with red eyes.",victim,NULL,NULL,TO_ROOM);
    return;
}


void spell_power_stun ( int sn, int level, CHAR_DATA *ch, void *vo , int target) 
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;


    if (is_affected(victim,sn ) || saves_spell( level, victim,DAM_OTHER) )
	return;

    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = level / 90;
    af.location  = APPLY_DEX;
    af.modifier  = - 3;
    af.bitvector = AFF_STUN;
    affect_to_char( victim, &af );
    send_to_char( "You are stunned.\n\r", victim );
    act_color("$n is stunned.",victim,NULL,NULL,TO_ROOM,POS_SLEEPING,CLR_RED);
    return;
}



void spell_improved_invis( int sn, int level, CHAR_DATA *ch, void *vo,int target ) 
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_IMP_INVIS) )
	return;

    act( "$n fades out of existence.", victim, NULL, NULL, TO_ROOM );

    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = level / 10 ;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_IMP_INVIS;
    affect_to_char( victim, &af );
    send_to_char( "You fade out of existence.\n\r", victim );
    return;
}



void spell_improved_detection( int sn, int level, CHAR_DATA *ch, void *vo,int target) 
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( CAN_DETECT(victim, DETECT_IMP_INVIS) )
    {
	if (victim == ch)
	  send_to_char("You can already see improved invisible.\n\r",ch);
	else
	  act("$N can already see improved invisible mobiles.",ch,NULL,victim,TO_CHAR);
	return;
    }

    af.where     = TO_DETECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = level / 3;
    af.modifier  = 0;
    af.location  = APPLY_NONE;
    af.bitvector = DETECT_IMP_INVIS;
    affect_to_char( victim, &af );
    send_to_char( "Your eyes tingle.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}

void spell_severity_force( int sn, int level, CHAR_DATA *ch, void *vo,int target ) 
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    char buf[MAX_STRING_LENGTH]; 
    int dam;

    sprintf(buf,"You cracked the ground towards the %s.\n\r",victim->name);
    send_to_char( buf, ch );
    act( "$n cracked the ground towards you!.", ch, NULL, victim, TO_VICT );
    if (IS_AFFECTED(victim,AFF_FLYING))
	dam = 0;
    else dam = dice( level , 12 );
    damage(ch,victim,dam,sn,DAM_BASH,TRUE);
    return;
}

void spell_randomizer(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    AFFECT_DATA af,af2;

    if ( is_affected( ch, sn ) )
    {
      send_to_char
	("Your power of randomness has been exhausted for now.\n\r",
	 ch);
      return;
    }

  if (IS_SET(ch->in_room->room_flags, ROOM_LAW))
    {
      send_to_char(
	    "This room is far too orderly for your powers to work on it.\n\r",
		   ch);
      return;
    }
    if ( is_affected_room( ch->in_room, sn ))
    {
	send_to_char("This room has already been randomized.\n\r",ch);
	return;
    }

  if (number_bits(1) == 0)
    {
      send_to_char("Despite your efforts, the universe resisted chaos.\n\r",ch);
      af2.where     = TO_AFFECTS;
      af2.type      = sn;
      af2.level	    = ch->level;
      af2.duration  = level / 10;
      af2.modifier  = 0;
      af2.location  = APPLY_NONE;
      af2.bitvector = 0;
      affect_to_char( ch, &af2 );
      return;
    }

    af.where     = TO_ROOM_AFFECTS;
    af.type      = sn;
    af.level     = ch->level;
    af.duration  = level / 15;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_ROOM_RANDOMIZER;
    affect_to_room( ch->in_room, &af );

    af2.where     = TO_AFFECTS;
    af2.type      = sn;
    af2.level	  = ch->level;
    af2.duration  = level / 5;
    af2.modifier  = 0;
    af2.location  = APPLY_NONE;
    af2.bitvector = 0;
    affect_to_char( ch, &af2 );
    send_to_char("The room was successfully randomized!\n\r", ch);
    send_to_char("You feel very drained from the effort.\n\r", ch);
    ch->hit -= UMIN(200, ch->hit/2);
    act("The room starts to randomize exits.",ch,NULL,NULL,TO_ROOM);
    return;
}

void spell_bless_weapon(int sn,int level,CHAR_DATA *ch, void *vo,int target)
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    AFFECT_DATA af;

    if (obj->item_type != ITEM_WEAPON)
    {
	send_to_char("That isn't a weapon.\n\r",ch);
	return;
    }

    if (obj->wear_loc != -1)
    {
	send_to_char("The item must be carried to be blessed.\n\r",ch);
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
	    ||  IS_OBJ_STAT(obj,ITEM_BLESS) 
	    || IS_OBJ_STAT(obj,ITEM_BURN_PROOF) )
     {
	act("You can't seem to bless $p.",ch,obj,NULL,TO_CHAR);
	return;
     }
    }
    if (IS_WEAPON_STAT(obj,WEAPON_HOLY))
    {
	act("$p is already blessed for holy attacks.",ch,obj,NULL,TO_CHAR);
	return;
    }

    af.where	 = TO_WEAPON;
    af.type	 = sn;
    af.level	 = level / 2;
    af.duration	 = level/8;
    af.location	 = 0;
    af.modifier	 = 0;
    af.bitvector = WEAPON_HOLY;
    affect_to_obj(obj,&af);

    act("$p is prepared for holy attacks.",ch,obj,NULL,TO_ALL);
    return;

}

void spell_resilience( int sn, int level, CHAR_DATA *ch, void *vo , int target)
{
  AFFECT_DATA af;

    if (!is_affected(ch, sn))
    {
      send_to_char("You are now resistive to draining attacks.\n\r", ch);

      af.where = TO_RESIST;
      af.type = sn;
      af.duration = level / 10;
      af.level = ch->level;
      af.bitvector = RES_ENERGY;
      af.location = 0;
      af.modifier = 0;
      affect_to_char(ch, &af);
    }
  else 
      send_to_char("You are already resistive to draining attacks.\n\r",ch);
 return;
}

void spell_super_heal( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int bonus = 170 + level + dice(1,20);

    victim->hit = UMIN( victim->hit + bonus, victim->max_hit );
    update_pos( victim );
    send_to_char( "A warm feeling fills your body.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}

void spell_master_heal( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int bonus = 300 + level + dice(1,40);

    victim->hit = UMIN( victim->hit + bonus, victim->max_hit );
    update_pos( victim );
    send_to_char( "A warm feeling fills your body.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}

void spell_group_healing(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    CHAR_DATA *gch;
    int heal_num, refresh_num;

    heal_num = skill_lookup("master healing");
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


void spell_restoring_light( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int nsn, mana_add;

    if (IS_AFFECTED(victim,AFF_BLIND))
	{
	 nsn = skill_lookup("cure blindness");
	 spell_cure_blindness(nsn,level,ch,(void *)victim,TARGET_CHAR);
	}
    if (IS_AFFECTED(victim,AFF_CURSE))
	{
	 nsn = skill_lookup("remove curse");
	 spell_remove_curse(nsn,level,ch,(void *)victim,TARGET_CHAR);
	}
    if (IS_AFFECTED(victim,AFF_POISON))
	{
	 spell_cure_poison(gsn_cure_poison,level,ch,(void *)victim,TARGET_CHAR);
	}
    if (IS_AFFECTED(victim,AFF_PLAGUE))
	{
	 nsn = skill_lookup("cure disease");
	 spell_cure_disease(nsn,level,ch,(void *)victim,TARGET_CHAR);
	}

    if (victim->hit != victim->max_hit)
	{
    	 mana_add = UMIN( (victim->max_hit - victim->hit), ch->mana );
    	 victim->hit = UMIN( victim->hit + mana_add, victim->max_hit );
    	 ch->mana -= mana_add;
	}
    update_pos( victim );
    send_to_char( "A warm feeling fills your body.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}


void spell_lesser_golem( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  CHAR_DATA *gch;
  CHAR_DATA *golem;
  AFFECT_DATA af;
  int i=0;

  if (is_affected(ch,sn))
    {
      send_to_char("You lack the power to create another golem right now.\n\r",
		   ch);
      return;
    }

  send_to_char("You attempt to create a lesser golem.\n\r",ch);
  act("$n attempts to create a lesser golem.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  ( gch->pIndexData->vnum == MOB_VNUM_LESSER_GOLEM ) )
	{
	  i++;
	  if (i > 2)
           {
	    send_to_char("More golems are more than you can control!\n\r",ch);
	    return;
	   }
	}
    }

  if (count_charmed(ch)) return;

  golem = create_mobile( get_mob_index(MOB_VNUM_LESSER_GOLEM) );


  for (i = 0; i < MAX_STATS; i ++)
       golem->perm_stat[i] = UMIN(25,15 + ch->level/10);
            
  golem->perm_stat[STAT_STR] += 3;
  golem->perm_stat[STAT_INT] -= 1;
  golem->perm_stat[STAT_CON] += 2;

  golem->max_hit = IS_NPC(ch)? URANGE(ch->max_hit,1 * ch->max_hit,30000)
		: UMIN( (2 * ch->pcdata->perm_hit) + 400,30000);
  golem->hit = golem->max_hit;
  golem->max_mana = IS_NPC(ch)? ch->max_mana : ch->pcdata->perm_mana;
  golem->mana = golem->max_mana;
  golem->level = ch->level;
  for (i=0; i < 3; i++)
    golem->armor[i] = interpolate(golem->level,100,-100);
  golem->armor[3] = interpolate(golem->level,100,0);
  golem->gold = 0;
  golem->timer = 0;
  golem->damage[DICE_NUMBER] = 3;   
  golem->damage[DICE_TYPE] = 10;
  golem->damage[DICE_BONUS] = ch->level / 2;

  char_to_room(golem,ch->in_room);
  send_to_char("You created a lesser golem!\n\r",ch);
  act("$n creates a lesser golem!",ch,NULL,NULL,TO_ROOM);

  af.where		= TO_AFFECTS;
  af.type               = sn;
  af.level              = level; 
  af.duration           = 24;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);  

  SET_BIT(golem->affected_by, AFF_CHARM);
  golem->master = golem->leader = ch;

}


void spell_stone_golem( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  CHAR_DATA *gch;
  CHAR_DATA *golem;
  AFFECT_DATA af;
  int i=0;

  if (is_affected(ch,sn)) 
    {
      send_to_char("You lack the power to create another golem right now.\n\r",
		   ch);
      return;
    }

  send_to_char("You attempt to create a stone golem.\n\r",ch);
  act("$n attempts to create a stone golem.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  ( gch->pIndexData->vnum == MOB_VNUM_STONE_GOLEM ) )
	{
	  i++;
	  if (i > 2)
           {
	    send_to_char("More golems are more than you can control!\n\r",ch);
	    return;
	   }
	}
    }

  if (count_charmed(ch)) return;

  golem = create_mobile( get_mob_index(MOB_VNUM_STONE_GOLEM) );


  for (i = 0; i < MAX_STATS; i ++)
       golem->perm_stat[i] = UMIN(25,15 + ch->level/10);
            
  golem->perm_stat[STAT_STR] += 3;
  golem->perm_stat[STAT_INT] -= 1;
  golem->perm_stat[STAT_CON] += 2;

  golem->max_hit = IS_NPC(ch)? URANGE(ch->max_hit,1 * ch->max_hit,30000)
		: UMIN( (5 * ch->pcdata->perm_hit) + 2000, 30000);
  golem->hit = golem->max_hit;
  golem->max_mana = IS_NPC(ch)? ch->max_mana : ch->pcdata->perm_mana;
  golem->mana = golem->max_mana;
  golem->level = ch->level;
  for (i=0; i < 3; i++)
    golem->armor[i] = interpolate(golem->level,100,-100);
  golem->armor[3] = interpolate(golem->level,100,0);
  golem->gold = 0;
  golem->timer = 0;
  golem->damage[DICE_NUMBER] = 8;   
  golem->damage[DICE_TYPE] = 4;
  golem->damage[DICE_BONUS] = ch->level / 2;

  char_to_room(golem,ch->in_room);
  send_to_char("You created a stone golem!\n\r",ch);
  act("$n creates a stone golem!",ch,NULL,NULL,TO_ROOM);

  af.where		= TO_AFFECTS;
  af.type               = sn;
  af.level              = level; 
  af.duration           = 24;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);  

  SET_BIT(golem->affected_by, AFF_CHARM);
  golem->master = golem->leader = ch;

}


void spell_iron_golem( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  CHAR_DATA *gch;
  CHAR_DATA *golem;
  AFFECT_DATA af;
  int i = 0;

  if (is_affected(ch,sn)) 
    {
      send_to_char("You lack the power to create another golem right now.\n\r",
		   ch);
      return;
    }

  send_to_char("You attempt to create an iron golem.\n\r",ch);
  act("$n attempts to create an iron golem.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  ( gch->pIndexData->vnum == MOB_VNUM_IRON_GOLEM ) )
	{
	    send_to_char("More golems are more than you can control!\n\r",ch);
	    return;
	}
    }

  if (count_charmed(ch)) return;

  golem = create_mobile( get_mob_index(MOB_VNUM_IRON_GOLEM) );


  for (i = 0; i < MAX_STATS; i ++)
       golem->perm_stat[i] = UMIN(25,15 + ch->level/10);
            
  golem->perm_stat[STAT_STR] += 3;
  golem->perm_stat[STAT_INT] -= 1;
  golem->perm_stat[STAT_CON] += 2;

  golem->max_hit = IS_NPC(ch)? URANGE(ch->max_hit,1 * ch->max_hit,30000)
		: UMIN( (10 * ch->pcdata->perm_hit) + 1000, 30000);
  golem->hit = golem->max_hit;
  golem->max_mana = IS_NPC(ch)? ch->max_mana : ch->pcdata->perm_mana;
  golem->mana = golem->max_mana;
  golem->level = ch->level;
  for (i=0; i < 3; i++)
    golem->armor[i] = interpolate(golem->level,100,-100);
  golem->armor[3] = interpolate(golem->level,100,0);
  golem->gold = 0;
  golem->timer = 0;
  golem->damage[DICE_NUMBER] = 11;   
  golem->damage[DICE_TYPE] = 5;
  golem->damage[DICE_BONUS] = ch->level / 2 + 10;

  char_to_room(golem,ch->in_room);
  send_to_char("You created an iron golem!\n\r",ch);
  act("$n creates an iron golem!",ch,NULL,NULL,TO_ROOM);

  af.where		= TO_AFFECTS;
  af.type               = sn;
  af.level              = level; 
  af.duration           = 24;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);  

  SET_BIT(golem->affected_by, AFF_CHARM);
  golem->master = golem->leader = ch;

}


void spell_adamantite_golem( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  CHAR_DATA *gch;
  CHAR_DATA *golem;
  AFFECT_DATA af;
  int i = 0;

  if (is_affected(ch,sn))
    {
      send_to_char("You lack the power to create another golem right now.\n\r",
		   ch);
      return;
    }

  send_to_char("You attempt to create an Adamantite golem.\n\r",ch);
  act("$n attempts to create an Adamantite golem.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  ( gch->pIndexData->vnum == MOB_VNUM_ADAMANTITE_GOLEM ) )
	{
	    send_to_char("More golems are more than you can control!\n\r",ch);
	    return;
	}
    }

  if (count_charmed(ch)) return;

  golem = create_mobile( get_mob_index(MOB_VNUM_ADAMANTITE_GOLEM) );


  for (i = 0; i < MAX_STATS; i ++)
       golem->perm_stat[i] = UMIN(25,15 + ch->level/10);
            
  golem->perm_stat[STAT_STR] += 3;
  golem->perm_stat[STAT_INT] -= 1;
  golem->perm_stat[STAT_CON] += 2;

  golem->max_hit = IS_NPC(ch)? URANGE(ch->max_hit,1 * ch->max_hit,30000)
		: UMIN( (10 * ch->pcdata->perm_hit) + 4000, 30000);
  golem->hit = golem->max_hit;
  golem->max_mana = IS_NPC(ch)? ch->max_mana : ch->pcdata->perm_mana;
  golem->mana = golem->max_mana;
  golem->level = ch->level;
  for (i=0; i < 3; i++)
    golem->armor[i] = interpolate(golem->level,100,-100);
  golem->armor[3] = interpolate(golem->level,100,0);
  golem->gold = 0;
  golem->timer = 0;
  golem->damage[DICE_NUMBER] = 13;   
  golem->damage[DICE_TYPE] = 9;
  golem->damage[DICE_BONUS] = ch->level / 2 + 10;

  char_to_room(golem,ch->in_room);
  send_to_char("You created an Adamantite golem!\n\r",ch);
  act("$n creates an Adamantite golem!",ch,NULL,NULL,TO_ROOM);

  af.where		= TO_AFFECTS;
  af.type               = sn;
  af.level              = level; 
  af.duration           = 24;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);  

  SET_BIT(golem->affected_by, AFF_CHARM);
  golem->master = golem->leader = ch;

}



void spell_sanctify_lands(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
  if (number_bits(1) == 0)
    {
      send_to_char("You failed.\n\r",ch);
      return;
    }

  if (IS_RAFFECTED(ch->in_room,AFF_ROOM_CURSE))
	{
	 affect_strip_room(ch->in_room,gsn_cursed_lands);
	 send_to_char("The curse of the land wears off.\n\r",ch);
	 act("The curse of the land wears off.\n\r",ch,NULL,NULL,TO_ROOM);
	}
  if (IS_RAFFECTED(ch->in_room,AFF_ROOM_POISON))
	{
	 affect_strip_room(ch->in_room,gsn_deadly_venom);
	 send_to_char("The land seems more healthy.\n\r",ch);
	 act("The land seems more healthy.\n\r",ch,NULL,NULL,TO_ROOM);
	}
  if (IS_RAFFECTED(ch->in_room,AFF_ROOM_SLEEP))
	{
	 send_to_char("The land wake up from mysterious dream.\n\r",ch);
	 act("The land wake up from mysterious dream.\n\r",ch,NULL,NULL,TO_ROOM);
	 affect_strip_room(ch->in_room,gsn_mysterious_dream);
	}
  if (IS_RAFFECTED(ch->in_room,AFF_ROOM_PLAGUE))
	{
	 send_to_char("The disease of the land has been treated.\n\r",ch);
	 act("The disease of the land has been treated.\n\r",ch,NULL,NULL,TO_ROOM);
	 affect_strip_room(ch->in_room,gsn_black_death);
	}
  if (IS_RAFFECTED(ch->in_room,AFF_ROOM_SLOW))
	{
	 send_to_char("The lethargic mist dissolves.\n\r",ch);
	 act("The lethargic mist dissolves.\n\r",ch,NULL,NULL,TO_ROOM);
	 affect_strip_room(ch->in_room,gsn_lethargic_mist);
	}
    return;
}


void spell_deadly_venom(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    AFFECT_DATA af;

  if (IS_SET(ch->in_room->room_flags, ROOM_LAW))
    {
      send_to_char("This room is protected by gods.\n\r",  ch);
      return;
    }
    if ( is_affected_room( ch->in_room, sn ))
    {
     send_to_char("This room has already been effected by deadly venom.\n\r",ch);
     return;
    }

    af.where     = TO_ROOM_AFFECTS;
    af.type      = sn;
    af.level     = ch->level;
    af.duration  = level / 15;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_ROOM_POISON;
    affect_to_room( ch->in_room, &af );

    send_to_char("The room starts to be filled by poison.\n\r",ch);   
    act("The room starts to be filled by poison.\n\r",ch,NULL,NULL,TO_ROOM);

}

void spell_cursed_lands(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    AFFECT_DATA af;

  if (IS_SET(ch->in_room->room_flags, ROOM_LAW))
    {
      send_to_char("This room is protected by gods.\n\r",ch);
      return;
    }
    if ( is_affected_room( ch->in_room, sn ))
    {
     send_to_char("This room has already been cursed.\n\r",ch);
     return;
    }

    af.where     = TO_ROOM_AFFECTS;
    af.type      = sn;
    af.level     = ch->level;
    af.duration  = level / 15;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_ROOM_CURSE;
    affect_to_room( ch->in_room, &af );

    send_to_char("The gods has forsaken the room.\n\r",ch);   
    act("The gos has forsaken the room.\n\r",ch,NULL,NULL,TO_ROOM);

}

void spell_lethargic_mist(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
   AFFECT_DATA af;

  if (IS_SET(ch->in_room->room_flags, ROOM_LAW))
    {
      send_to_char("This room is protected by gods.\n\r",  ch);
      return;
    }
    if ( is_affected_room( ch->in_room, sn ))
    {
     send_to_char("This room has already been full of lethargic mist.\n\r",ch);
     return;
    }

    af.where     = TO_ROOM_AFFECTS;
    af.type      = sn;
    af.level     = ch->level;
    af.duration  = level / 15;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_ROOM_SLOW;
    affect_to_room( ch->in_room, &af );

    send_to_char("The air in the room makes you slowing down.\n\r",ch);   
    act("The air in the room makes you slowing down.\n\r",ch,NULL,NULL,TO_ROOM);

}

void spell_black_death(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
  AFFECT_DATA af;

  if (IS_SET(ch->in_room->room_flags, ROOM_LAW))
    {
      send_to_char("This room is protected by gods.\n\r",ch);
      return;
    }
    if ( is_affected_room( ch->in_room, sn ))
    {
     send_to_char("This room has already been diseased.\n\r",ch);
     return;
    }

    af.where     = TO_ROOM_AFFECTS;
    af.type      = sn;
    af.level     = ch->level;
    af.duration  = level / 15;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_ROOM_PLAGUE;
    affect_to_room( ch->in_room, &af );

    send_to_char("The room starts to be filled by disease.\n\r",ch);   
    act("The room starts to be filled by disease.\n\r",ch,NULL,NULL,TO_ROOM);

}

void spell_mysterious_dream(int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
  AFFECT_DATA af;

  if (IS_SET(ch->in_room->room_flags, ROOM_LAW))
    {
      send_to_char("This room is protected by gods.\n\r",  ch);
      return;
    }
    if ( is_affected_room( ch->in_room, sn ))
    {
     send_to_char("This room has already been affected by sleep gas.\n\r",ch);
     return;
    }

    af.where     = TO_ROOM_AFFECTS;
    af.type      = sn;
    af.level     = ch->level;
    af.duration  = level / 15;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_ROOM_SLEEP;
    affect_to_room( ch->in_room, &af );

    send_to_char("The room starts to be seen good place to sleep.\n\r",ch);   
    act("The room starts to be seen good place to you.\n\r",ch,NULL,NULL,TO_ROOM);

}

void spell_polymorph( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    AFFECT_DATA af;
    int race;

    if ( is_affected( ch, sn ) )
    {
     send_to_char("You are already polymorphed.\n\r",ch); 
     return;
    }

    if (target_name == NULL)
    {
     send_to_char("Usage: cast 'polymorph' <pcracename>.\n\r",ch); 
     return;
    }

    race = race_lookup( target_name );

    if (race == 0 || !race_table[race].pc_race)
    {
     send_to_char("That is not a valid race to polymorph.\n\r",ch); 
     return;
    }

    af.where	 = TO_RACE;
    af.type      = sn;
    af.level     = level;
    af.duration  = level/10;
    af.location  = APPLY_NONE;
    af.modifier  = race;
    af.bitvector = 0;
    affect_to_char( ch, &af );

    act("$n polymorphes $mself to $t.", 
		ch, race_table[race].name, NULL,TO_ROOM );
    act( "You polymorph yourself to $t.\n\r", 
		ch, race_table[race].name, NULL,TO_CHAR );
    return;
}


void spell_plant_form( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    AFFECT_DATA af;

    if ( !ch->in_room
    ||   IS_SET(ch->in_room->room_flags, ROOM_PRIVATE)
    ||   IS_SET(ch->in_room->room_flags, ROOM_SOLITARY)
    ||   (ch->in_room->sector_type != SECT_FOREST
    	  &&   ch->in_room->sector_type != SECT_FIELD)
    ||   IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL))
	{
	 send_to_char("Not here.\n\r",ch);
	 return;
	}

    af.where     = TO_DETECTS;
    af.type      = gsn_fear;
    af.level     = level;
    af.duration  = level / 10;
    af.location  = 0;
    af.modifier  = 0;

    if ( ch->in_room->sector_type == SECT_FOREST )
    {
      send_to_char("You starts to be seen a nearby tree!\n\r",ch);
      act("$n starts to be seen a nearby tree!", ch, NULL, NULL,TO_ROOM );
      af.bitvector = ADET_FORM_TREE;
    }
    else
    {
      send_to_char("You starts to be seen some grass!\n\r",ch);
      act("$n starts to be seen some grass!", ch, NULL, NULL, TO_ROOM );
      af.bitvector = ADET_FORM_GRASS;
    }
    affect_to_char( ch, &af );
    return;
}


void spell_blade_barrier(int sn,int level,CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    char buf[MAX_STRING_LENGTH];
    int dam;

    act("Many sharp blades appear around $n and crash $N.",
	ch,NULL,victim,TO_ROOM);
    act("Many sharp blades appear around you and crash $N.",
	ch,NULL,victim,TO_CHAR);
    act("Many sharp blades appear around $n and crash you!",
	ch,NULL,victim,TO_VICT);

    dam = dice(level,7);
    if (saves_spell(level,victim,DAM_PIERCE))
	dam /= 2;
    damage(ch,victim,dam,sn,DAM_PIERCE,TRUE);

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

    act("The blade barriers crash $n!",victim,NULL,NULL,TO_ROOM);
    dam = dice(level,5);
    if (saves_spell(level,victim,DAM_PIERCE))
	dam /= 2;
    damage(ch,victim,dam,sn,DAM_PIERCE,TRUE);
    act("The blade barriers crash you!",victim,NULL,NULL,TO_CHAR);

    if ( number_percent() < 50 ) return;

    act("The blade barriers crash $n!",victim,NULL,NULL,TO_ROOM);
    dam = dice(level,4);
    if (saves_spell(level,victim,DAM_PIERCE))
	dam /= 2;
    damage(ch,victim,dam,sn,DAM_PIERCE,TRUE);
    act("The blade barriers crash you!",victim,NULL,NULL,TO_CHAR);

    if ( number_percent() < 50 ) return;

    act("The blade barriers crash $n!",victim,NULL,NULL,TO_ROOM);
    dam = dice(level,2);
    if (saves_spell(level,victim,DAM_PIERCE))
	dam /= 2;
    damage(ch,victim,dam,sn,DAM_PIERCE,TRUE);
    act("The blade barriers crash you!",victim,NULL,NULL,TO_CHAR);

}


void spell_protection_negative ( int sn, int level, CHAR_DATA *ch, void *vo , int target)
{
  AFFECT_DATA af;

    if (!is_affected(ch, sn))
    {
      send_to_char("You are now immune to negative attacks.\n\r", ch);

      af.where = TO_IMMUNE;
      af.type = sn;
      af.duration = level / 4;
      af.level = ch->level;
      af.bitvector = IMM_NEGATIVE;
      af.location = 0;
      af.modifier = 0;
      affect_to_char(ch, &af);
    }
  else 
      send_to_char("You are already immune to negative attacks.\n\r", ch);
 return;
}


void spell_ruler_aura( int sn, int level, CHAR_DATA *ch, void *vo , int target)
{
  AFFECT_DATA af;

    if (!is_affected(ch, sn))
    {
      send_to_char("You now feel more self confident in rulership.\n\r", ch);

      af.where = TO_IMMUNE;
      af.type = sn;
      af.duration = level / 4;
      af.level = ch->level;
      af.bitvector = IMM_CHARM;
      af.location = 0;
      af.modifier = 0;
      affect_to_char(ch, &af);
    }
  else 
      send_to_char("You are as much self confident as you can.\n\r", ch);
 return;
}


void spell_evil_spirit( int sn, int level, CHAR_DATA *ch, void *vo , int target)
{
 AREA_DATA *pArea = ch->in_room->area;
 ROOM_INDEX_DATA *room;
 AFFECT_DATA af,af2;
 int i;

 if (IS_RAFFECTED(ch->in_room, AFF_ROOM_ESPIRIT)
	|| is_affected_room(ch->in_room,sn) )
  {
   send_to_char("The zone is already full of evil spirit.\n\r",ch);
   return;
  }

 if ( is_affected( ch, sn ) )
    {
      send_to_char("Your power of evil spirit is less for you, now.\n\r",ch);
      return;
    }

  if (IS_SET(ch->in_room->room_flags, ROOM_LAW)
	|| IS_SET(ch->in_room->area->area_flag,AREA_HOMETOWN) )
    {
      send_to_char("Holy aura in this room prevents your powers to work on it.\n\r",ch);
      return;
    }

    af2.where     = TO_AFFECTS;
    af2.type      = sn;
    af2.level	  = ch->level;
    af2.duration  = level / 5;
    af2.modifier  = 0;
    af2.location  = APPLY_NONE;
    af2.bitvector = 0;
    affect_to_char( ch, &af2 );

    af.where     = TO_ROOM_AFFECTS;
    af.type      = sn;
    af.level     = ch->level;
    af.duration  = level / 25;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_ROOM_ESPIRIT;

    for (i=pArea->min_vnum; i<pArea->max_vnum; i++)  
    {
     if ((room = get_room_index(i)) == NULL) continue;
     affect_to_room( room, &af );
     if (room->people) 
	act("The zone is starts to be filled with evil spirit.",room->people,NULL,NULL,TO_ALL);
    }

}


void spell_disgrace( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  AFFECT_DATA af;
  CHAR_DATA *victim = (CHAR_DATA *) vo;

  if (!is_affected(victim,sn) && !saves_spell(level, victim, DAM_MENTAL))
    {
      af.where		    = TO_AFFECTS;
      af.type               = sn;
      af.level              = level; 
      af.duration           = level;
      af.location           = APPLY_CHA;
      af.modifier           = - (5 + level / 5);
      af.bitvector          = 0;
      affect_to_char(victim,&af);

      act("$N feels $M less confident!",ch,NULL,victim,TO_ALL);
    }
  else send_to_char("You failed.\n\r",ch);
}


void spell_control_undead( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;

    if  ( !IS_NPC(victim) || !IS_SET(victim->act,ACT_UNDEAD) ) 
    {
	act("$N doesn't seem to be an undead.",ch,NULL,victim,TO_CHAR);
	return;
    }
    spell_charm_person(sn,level,ch,vo,target);
    return;
}



void spell_assist( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( ch, sn ) )
      {
	send_to_char("This power is used too recently.\n\r",ch);
	return;
      }

    af.where	 = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = 1+ level / 50;
    af.location  = 0;
    af.modifier  = 0;
    af.bitvector = 0;
    affect_to_char( ch, &af );

    victim->hit += 100 + level * 5;
    update_pos( victim );
    send_to_char( "A warm feeling fills your body.\n\r", victim );
    act("$n looks better.", victim, NULL, NULL, TO_ROOM);
    if (ch != victim) send_to_char("Ok.\n\r",ch);
    return;
}  
  
  

void spell_aid( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( ch, sn ) )
      {
	send_to_char("This power is used too recently.\n\r",ch);
	return;
      }

    af.where	 = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = level / 50;
    af.location  = 0;
    af.modifier  = 0;
    af.bitvector = 0;
    affect_to_char( ch, &af );

    victim->hit += level * 5;
    update_pos( victim );
    send_to_char( "A warm feeling fills your body.\n\r", victim );
    act("$n looks better.", victim, NULL, NULL, TO_ROOM);
    if (ch != victim) send_to_char("Ok.\n\r",ch);
    return;
}  
  


void spell_summon_shadow( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  CHAR_DATA *gch;
  CHAR_DATA *shadow;
  AFFECT_DATA af;
  int i;

  if (is_affected(ch,sn))
    {
      send_to_char("You lack the power to summon another shadow right now.\n\r",
		   ch);
      return;
    }

  send_to_char("You attempt to summon a shadow.\n\r",ch);
  act("$n attempts to summon a shadow.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  gch->pIndexData->vnum == MOB_VNUM_SUM_SHADOW)
	{
	  send_to_char("Two shadows are more than you can control!\n\r",ch);
	  return;
	}
    }

  if (count_charmed(ch)) return;

  shadow = create_mobile( get_mob_index(MOB_VNUM_SUM_SHADOW) );

  for (i=0;i < MAX_STATS; i++)
    {
      shadow->perm_stat[i] = ch->perm_stat[i];
    }

  shadow->max_hit = IS_NPC(ch)? URANGE(ch->max_hit,1 * ch->max_hit,30000)
		: URANGE(ch->pcdata->perm_hit,ch->hit,30000);
  shadow->hit = shadow->max_hit;
  shadow->max_mana = IS_NPC(ch)? ch->max_mana : ch->pcdata->perm_mana;
  shadow->mana = shadow->max_mana;
  shadow->level = ch->level;
  for (i=0; i < 3; i++)
    shadow->armor[i] = interpolate(shadow->level,100,-100);
  shadow->armor[3] = interpolate(shadow->level,100,0);
  shadow->gold = 0;
  shadow->timer = 0;
  shadow->damage[DICE_NUMBER] = number_range(level/15, level/10);   
  shadow->damage[DICE_TYPE] = number_range(level/3, level/2);
  shadow->damage[DICE_BONUS] = number_range(level/8, level/6);

  char_to_room(shadow,ch->in_room);
  act("A shadow conjures!",ch,NULL,NULL,TO_ALL);

  af.where		= TO_AFFECTS;
  af.type               = sn;
  af.level              = level; 
  af.duration           = 24;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);  

  SET_BIT(shadow->affected_by, AFF_CHARM);
  shadow->master = shadow->leader = ch;

}


void spell_farsight( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    ROOM_INDEX_DATA *room,*oldr;
    int mount;

    if ( (room = check_place(ch,target_name)) == NULL)
      {
	send_to_char("You cannot see that much far.\n\r",ch);
	return;
      }

    if (ch->in_room == room)
      do_look( ch, "auto" );
    else 
    {
      mount = MOUNTED(ch) ? 1 : 0;
      oldr = ch->in_room;
      char_from_room( ch );
      char_to_room( ch, room );
      do_look( ch, "auto" );
      char_from_room( ch );
      char_to_room( ch, oldr );
      if (mount)
	{
	 ch->riding = TRUE;
	 MOUNTED(ch)->riding = TRUE;
	}
    }
}


void spell_remove_fear( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;

    if (check_dispel(level,victim,gsn_fear))
    {
	send_to_char("You feel more brave.\n\r",victim);
	act("$n looks more conscious.",victim,NULL,NULL,TO_ROOM);
    }
    else send_to_char("You failed.\n\r",ch);
}

void spell_desert_fist( int sn, int level, CHAR_DATA *ch, void *vo,int target )
 {
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    if ( (ch->in_room->sector_type != SECT_HILLS)
	&& (ch->in_room->sector_type != SECT_MOUNTAIN)
	&& (ch->in_room->sector_type != SECT_DESERT) )
	{
	 send_to_char("You don't find any sand here to create a fist.\n\r",ch);
	 ch->wait = 0;
	 return;
	}

    act("An existing parcel of sand rises up and forms a fist and pummels $n.",
		victim,NULL,NULL,TO_ROOM);
    act("An existing parcel of sand rises up and forms a fist and pummels you.",
		victim,NULL,NULL,TO_CHAR);
    dam = dice( level , 16 );
    damage(ch,victim,dam,sn,DAM_OTHER,TRUE);
    sand_effect(victim,level,dam,TARGET_CHAR);
}

void spell_holy_aura( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
    {
        if (victim == ch)
          send_to_char("You have already got a holy aura.\n\r",ch);
        else
          act("$N has already got a holy aura.",ch,NULL,victim,TO_CHAR);
        return;
    }

    if ( !IS_GOOD(victim) )
    {
	send_to_char("It doesn't worth to protect to with holy aura!", ch);
	return;
    }

    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = 7 + level / 6;
    af.modifier  = - (20 + level / 4); 
    af.location  = APPLY_AC;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    af.where = TO_RESIST;
    af.location = 0;
    af.modifier = 0;
    af.bitvector = RES_NEGATIVE;
    affect_to_char(ch, &af);

    send_to_char( "You feel ancient holy power protecting you.\n\r", victim );
    if ( ch != victim )
        act("$N is protected by ancient holy power.",ch,NULL,victim,TO_CHAR);
    return;
}

void spell_holy_fury(int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if (is_affected(victim,sn) || IS_AFFECTED(victim,AFF_BERSERK))
    {
        if (victim == ch)
          send_to_char("You are already in a holy fury.\n\r",ch);
        else
          act("$N is already in a holy fury.",ch,NULL,victim,TO_CHAR);
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

    if ( !IS_GOOD(victim) )
    {
        act("$N doesn't worth to influence with holy fury.",ch,NULL,victim,TO_CHAR);
        return;
    }

    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = level / 3;
    af.modifier  = level / 6;
    af.bitvector = 0;

    af.location  = APPLY_HITROLL;
    affect_to_char(victim,&af);

    af.location  = APPLY_DAMROLL;
    affect_to_char(victim,&af);

    af.modifier  = 10 * (level / 12);
    af.location  = APPLY_AC;
    affect_to_char(victim,&af);

    send_to_char("You are filled with holy fury!\n\r",victim);
    act("$n gets a wild look in $s eyes!",victim,NULL,NULL,TO_ROOM);
}

void spell_light_arrow( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = dice( level, 12 );
    if ( saves_spell( level, victim, DAM_HOLY ) )
        dam /= 2;
    damage( ch, victim, dam, sn,DAM_HOLY,TRUE);
    return;
}


void spell_hydroblast( int sn, int level, CHAR_DATA *ch, void *vo,int target )
 {
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    if ( (ch->in_room->sector_type != SECT_WATER_SWIM)
	&& (ch->in_room->sector_type != SECT_WATER_NOSWIM)
	&& (weather_info.sky != SKY_RAINING || !IS_OUTSIDE(ch)) )
	{
	 send_to_char("You couldn't reach any water molecule here.\n\r",ch);
	 ch->wait = 0;
	 return;
	}

    act("The water molecules around $n comes together and forms a fist.",
		ch,NULL,NULL,TO_ROOM);
    act("The water molecules around you comes together and forms a fist.",
		ch,NULL,NULL,TO_CHAR);
    dam = dice( level , 14 );
    damage(ch,victim,dam,sn,DAM_BASH,TRUE);
}

void spell_wolf_spirit( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    AFFECT_DATA af;
 
    if ( is_affected( ch, sn ) )
    {
     send_to_char("The blood in your vains flowing as fast as it can!\n\r",ch);
     return;
    }

/* haste */
    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = 3 + level / 30;
    af.location  = APPLY_DEX;
    af.modifier  = 1 + (level > 40) + (level > 60);
    af.bitvector = AFF_HASTE;
    affect_to_char( ch, &af );

/* damroll */
    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = 3 + level / 30;
    af.location  = APPLY_DAMROLL;
    af.modifier  = level / 2;
    af.bitvector = AFF_BERSERK;
    affect_to_char( ch, &af );

/* infravision */
    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = 3 + level / 30;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_INFRARED;
    affect_to_char( ch, &af );


   send_to_char( "The blood in your vains start to flow faster.\n\r", ch );
   act("The eyes of $n turn to RED!",ch,NULL,NULL,TO_ROOM);
   return;
}


void spell_sword_of_justice( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  int dam;

  if ( (IS_GOOD(ch) && IS_GOOD(victim))
	|| (IS_EVIL(ch) && IS_EVIL(victim))
	|| IS_NEUTRAL(ch) )
  {
    if (IS_NPC(victim) || !IS_SET(victim->act, PLR_WANTED))
    {
	send_to_char("You failed!\n\r", ch);
	return;
    }
  }

  if (!IS_NPC(victim) && IS_SET(victim->act, PLR_WANTED))
	dam = dice(level, 20);
  else	dam = dice(level, 14);

  if (saves_spell(level,victim, DAM_MENTAL))
	      dam /= 2;

  do_yell(ch, "The Sword of Justice!");
  act("The sword of justice appears and strikes $N!",ch,NULL,victim,TO_ALL);

  damage(ch,victim,dam,sn,DAM_MENTAL, TRUE);
}

void spell_guard_dogs( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  CHAR_DATA *gch;
  CHAR_DATA *dog;
  CHAR_DATA *dog2;
  AFFECT_DATA af;
  int i;

  if (is_affected(ch,sn))
    {
    send_to_char("You lack the power to summon another wolf right now.\n\r",ch);
      return;
    }

  send_to_char("You attempt to summon a dog.\n\r",ch);
  act("$n attempts to summon a dog.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  gch->pIndexData->vnum == MOB_VNUM_DOG)
	{
	  send_to_char("Two dogs are more than you can control!\n\r",ch);
	  return;
	}
    }

  if (count_charmed(ch)) return;

  dog = create_mobile( get_mob_index(MOB_VNUM_DOG) );

  for (i=0;i < MAX_STATS; i++)
    {
      dog->perm_stat[i] = ch->perm_stat[i];
    }

  dog->max_hit = IS_NPC(ch)? URANGE(ch->max_hit,1 * ch->max_hit,30000)
		: URANGE(ch->pcdata->perm_hit,ch->hit,30000);
  dog->hit = dog->max_hit;
  dog->max_mana = IS_NPC(ch)? ch->max_mana : ch->pcdata->perm_mana;
  dog->mana = dog->max_mana;
  dog->level = ch->level;
  for (i=0; i < 3; i++)
    dog->armor[i] = interpolate(dog->level,100,-100);
  dog->armor[3] = interpolate(dog->level,100,0);
  dog->gold = 0;
  dog->timer = 0;
  dog->damage[DICE_NUMBER] = number_range(level/15, level/12);   
  dog->damage[DICE_TYPE] = number_range(level/3, level/2);
  dog->damage[DICE_BONUS] = number_range(level/10, level/8);

  dog2 = create_mobile(dog->pIndexData);
  clone_mobile(dog,dog2);
  
  SET_BIT(dog->affected_by, AFF_CHARM);
  SET_BIT(dog2->affected_by, AFF_CHARM);
  dog->master = dog2->master = ch;
  dog->leader = dog2->leader = ch;

  char_to_room(dog,ch->in_room);
  char_to_room(dog2,ch->in_room);
  send_to_char("Two dogs arrive and bows before you!\n\r",ch);
  act("Two dogs arrive from somewhere and bows!",ch,NULL,NULL,TO_ROOM);

  af.where		= TO_AFFECTS;
  af.type               = sn;
  af.level              = level; 
  af.duration           = 24;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);  

}

void spell_eyes_of_tiger( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim;
    ROOM_INDEX_DATA *ori_room;

    if ( (victim = get_char_world(ch, target_name)) == NULL)
      {
	send_to_char("Your tiger eyes cannot see such a player.\n\r",ch);
	return;
      }

    if ( IS_NPC(victim) || victim->cabal != CABAL_HUNTER)
    {
	send_to_char("Your tiger eyes sees only hunters!\n\r",ch);
	return;
    }

    if ((victim->level > ch->level + 7) 
	|| saves_spell((ch->level + 9), victim, DAM_NONE))
      {
	send_to_char("Your tiger eyes cannot see that player.\n\r",ch);
	return;
      }

    if (ch==victim)
      do_look( ch, "auto" );
    else {
      ori_room = ch->in_room;
      char_from_room( ch );
      char_to_room( ch, victim->in_room );
      do_look( ch, "auto" );
      char_from_room( ch );
      char_to_room( ch, ori_room );
    }
}

void spell_lion_shield( int sn, int level, CHAR_DATA *ch, void *vo,int target) 
{
  OBJ_DATA *shield;
  AFFECT_DATA af;

  shield = create_object( get_obj_index(OBJ_VNUM_LION_SHIELD), level );
  shield->timer = level;
  shield->level = ch->level;
  shield->cost  = 0;
  obj_to_char(shield, ch);
  
  af.where        = TO_OBJECT;
  af.type         = sn;
  af.level        = level;
  af.duration     = -1;
  af.modifier     = level / 8;
  af.bitvector    = 0;

  af.location     = APPLY_HITROLL;
  affect_to_obj(shield,&af);

  af.location     = APPLY_DAMROLL;
  affect_to_obj(shield,&af);

  
  af.where        = TO_OBJECT;
  af.type         = sn;
  af.level        = level;
  af.duration     = -1;
  af.modifier     = -(level * 2) / 3;
  af.bitvector    = 0;
  af.location     = APPLY_AC;
  affect_to_obj(shield,&af);

  af.where        = TO_OBJECT;
  af.type         = sn;
  af.level        = level;
  af.duration     = -1;
  af.modifier     = UMAX(1,level /  30);
  af.bitvector    = 0;
  af.location     = APPLY_CHA;
  affect_to_obj(shield,&af);

  af.where        = TO_OBJECT;
  af.type         = sn;
  af.level        = level;
  af.duration     = -1;
  af.modifier     = -level/9;
  af.bitvector    = 0;
  af.location     = APPLY_SAVING_SPELL;
  affect_to_obj(shield,&af);

  act("You create $p!",ch,shield,NULL,TO_CHAR);
  act("$n creates $p!",ch,shield,NULL,TO_ROOM);
}
  
void spell_evolve_lion( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  AFFECT_DATA af;

  if (is_affected(ch,sn) || ch->hit > ch->max_hit)
    {
      send_to_char("You are already as lion as you can get.\n\r",ch);
      return;
    }

  ch->hit += ch->pcdata->perm_hit;

  af.where     = TO_AFFECTS;
  af.type      = sn;
  af.level     = level; 
  af.duration  = 3 + level / 30;
  af.location  = APPLY_HIT;
  af.modifier  = ch->pcdata->perm_hit;
  af.bitvector = 0;
  affect_to_char(ch,&af);

  af.where     = TO_AFFECTS;
  af.type      = sn;
  af.level     = level;
  af.duration  = 3 + level / 30;
  af.location  = APPLY_DEX;
  af.modifier  = -(1 + level/30);
  af.bitvector = AFF_SLOW;
  affect_to_char( ch, &af );

  af.where     = TO_AFFECTS;
  af.type      = sn;
  af.level     = level;
  af.duration  = 3 + level / 30;
  af.location  = APPLY_DAMROLL;
  af.modifier  = level / 2;
  af.bitvector = AFF_BERSERK;
  affect_to_char( ch, &af );

  af.where     = TO_DETECTS;
  af.type      = sn;
  af.level     = level;
  af.duration  = 3 + level / 30;
  af.location  = APPLY_NONE;
  af.modifier  = 0;
  af.bitvector = ADET_LION;
  affect_to_char( ch, &af );

  send_to_char("You feel yourself more clumsy, but more strong.\n\r",ch);
  act("The skin of $n turns to grey!",ch,NULL,NULL,TO_ROOM);
}

void spell_prevent(int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    AFFECT_DATA af,af2;

    if ( is_affected_room( ch->in_room, sn ))
    {
	send_to_char("This room has already prevented from revenges!\n\r",ch);
	return;
    }

    af.where     = TO_ROOM_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = level / 30;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_ROOM_PREVENT;
    affect_to_room( ch->in_room, &af );

    af2.where     = TO_AFFECTS;
    af2.type      = sn;
    af2.level	  = level;
    af2.duration  = level / 10;
    af2.modifier  = 0;
    af2.location  = APPLY_NONE;
    af2.bitvector = 0;
    affect_to_char( ch, &af2 );
    send_to_char( "The room is now protected from hunter revenges!\n\r", ch );
    act("The room starts to be filled with $n's prevention from Hunters",ch,NULL,NULL,TO_ROOM);
    return;
}


void spell_enlarge( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
 
    if ( is_affected( victim, sn ) )
    {
	if (victim == ch)
	  send_to_char("You can't enlarge more!\n\r",ch);
	else
	  act("$N is already as large as $N can get.",ch,NULL,victim,TO_CHAR);
	return;
    }

    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = level/2;
    af.location  = APPLY_SIZE;
    af.modifier  = 1 + (level >= 35) + (level >= 65);
    af.modifier  = UMIN((SIZE_GARGANTUAN - victim->size), af.modifier);
    af.bitvector = 0;
    affect_to_char( victim, &af );

    send_to_char( "You feel yourself getting larger and larger.\n\r", victim );
    act("$n's body starts to enlarge.",victim,NULL,NULL,TO_ROOM);
   return;
}

void spell_chromatic_orb( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = dice(level,14);
    if ( saves_spell( level, victim,DAM_LIGHT) )
        dam /= 2;

    if (number_percent() < (0.7 * get_skill(ch,sn)) )
    {
	spell_blindness(skill_lookup("blindness"),
		 (level-10),ch,(void *) victim,TARGET_CHAR);

	spell_slow(skill_lookup("slow"),
		 (level-10),ch,(void *) victim,TARGET_CHAR);
    }

    damage( ch, victim, dam, sn, DAM_LIGHT,TRUE );

  return;
}

void spell_suffocate( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if (CAN_DETECT(victim,ADET_SUFFOCATE))
        {
         act("$N already cannot breathe.\n\r",ch,NULL,victim,TO_CHAR);
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

    af.where     = TO_DETECTS;
    af.type      = sn;
    af.level     = level * 3/4;
    af.duration  = 3 + level / 30;
    af.location  = APPLY_NONE;
    af.modifier  = 0; 
    af.bitvector = ADET_SUFFOCATE;
    affect_join(victim,&af);

    send_to_char ("You cannot breathe.\n\r",victim);
    act("$n tries to breathe, but cannot.", victim,NULL,NULL,TO_ROOM);

  return;
}

void spell_mummify( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
    CHAR_DATA *victim;
    CHAR_DATA *undead;
    OBJ_DATA *obj,*obj2,*next;
    AFFECT_DATA af;
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char buf3[MAX_STRING_LENGTH];
    char *argument,*arg;
    int i;

    /* deal with the object case first */
    if (target == TARGET_OBJ)
    {
	obj = (OBJ_DATA *) vo;

    if (!(obj->item_type == ITEM_CORPSE_NPC ||  
          obj->item_type == ITEM_CORPSE_PC))
    {
	send_to_char("You can animate only corpses!\n\r",ch);
	return;
    }

    if (obj->level > level + 10)     
    {
	send_to_char("The dead body is too strong for you to mummify!\n\r",ch);
	return;
    }

    if (is_affected(ch, sn))
    {
      send_to_char("You cannot summon the strength to handle more undead bodies.\n\r", ch);
      return;
    }

  if (count_charmed(ch)) return;

  if ( ch->in_room != NULL && IS_SET(ch->in_room->room_flags, ROOM_NO_MOB) )
  {
     send_to_char( "You can't mummify deads here.\n\r", ch );
     return;
  }

  if ( IS_SET(ch->in_room->room_flags, ROOM_SAFE)      ||
       IS_SET(ch->in_room->room_flags, ROOM_PRIVATE)   ||
       IS_SET(ch->in_room->room_flags, ROOM_SOLITARY)  )
  {
    send_to_char( "You can't mummify here.\n\r", ch );
    return;
  }

  undead = create_mobile( get_mob_index(MOB_VNUM_UNDEAD) );
  char_to_room(undead,ch->in_room);
  for (i=0;i < MAX_STATS; i++)
    {
      undead->perm_stat[i] = UMIN(25,2 * ch->perm_stat[i]);
    }

  undead->level = obj->level;
  undead->max_hit = (undead->level < 30) ? (undead->level * 30) :
			(undead->level < 60) ? (undead->level * 60) :
				(undead->level * 90);
  undead->hit = undead->max_hit;
  undead->max_mana = IS_NPC(ch)? ch->max_mana : ch->pcdata->perm_mana;
  undead->mana = undead->max_mana;
  undead->alignment = ch->alignment;

  for (i=0; i < 3; i++)
    undead->armor[i] = interpolate(undead->level,100,-100);
  undead->armor[3] = interpolate(undead->level,50,-200);
  undead->damage[DICE_NUMBER] = number_range(undead->level/20,undead->level/15);
  undead->damage[DICE_TYPE] = number_range(undead->level/6, undead->level/3);
  undead->damage[DICE_BONUS] = number_range(undead->level/12, undead->level/10);
  undead->sex = ch->sex;
  undead->gold = 0;
  
  SET_BIT(undead->act, ACT_UNDEAD);
  SET_BIT(undead->affected_by, AFF_CHARM);
  undead->master = ch;
  undead->leader = ch;

  sprintf(buf, "%s body undead", obj->name);
  undead->name = str_dup(buf);
  sprintf(buf2,"%s",obj->short_descr);
  argument = (char *)alloc_perm ( MAX_STRING_LENGTH );
  arg = (char *)alloc_perm ( MAX_STRING_LENGTH );
  argument = buf2;
  buf3[0] = '\0';
  while (argument[0] != '\0' )
	{
  argument = one_argument(argument, arg);
  if (!(!str_cmp(arg,"The") || !str_cmp(arg,"undead") || !str_cmp(arg,"body") ||
	!str_cmp(arg,"corpse") || !str_cmp(arg,"of") ))
	 {
	  if (buf3[0] == '\0')   strcat(buf3,arg);
	  else  {
		 strcat(buf3," ");
		 strcat(buf3,arg);
		}
	 }
	}
  sprintf(buf, "The mummified corpse of %s", buf3);
  undead->short_descr = str_dup(buf);
  sprintf(buf, "The mummifed corpse of %s slowly staggers around.\n\r", buf3);
  undead->long_descr = str_dup(buf);

  for(obj2 = obj->contains;obj2;obj2=next)
    {
	next = obj2->next_content;
	obj_from_obj(obj2);
	obj_to_char(obj2, undead);
    }
  interpret( undead,"wear all", TRUE);

  af.where	 = TO_AFFECTS;
  af.type      = sn;
  af.level	 = ch->level;
  af.duration  = (ch->level / 10);
  af.modifier  = 0;
  af.bitvector = 0;
  af.location  = APPLY_NONE;
  affect_to_char( ch, &af );

  send_to_char("With mystic power, you mummify and give life to it!\n\r",ch);
  sprintf(buf,"With mystic power, %s mummifies %s and give life to it!",
		ch->name,obj->name); 
  act(buf,ch,NULL,NULL,TO_ROOM);
  sprintf(buf,
	"%s looks at you and plans to make you pay for distrurbing its rest!",
	obj->short_descr); 
  act(buf,ch,NULL,NULL,TO_CHAR);
  extract_obj (obj);
	return;
    }

  victim = (CHAR_DATA *) vo;

  if ( victim == ch )
  {
	send_to_char( "But you aren't dead!!\n\r", ch );
	return;
  }

  send_to_char( "But it ain't dead!!\n\r", ch );
  return;
}

void spell_soul_bind( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;

  if (ch->pet != NULL)
  {
	send_to_char("Your soul is already binded to someone else.\n\r", ch);
	return;
  }

  if (!IS_NPC(victim) 
	|| !(IS_AFFECTED(victim, AFF_CHARM) && victim->master == ch))
  {
	send_to_char("You cannot bind that soul to you.\n\r", ch);
	return;
  }

  victim->leader = ch;
  ch->pet = victim;

  act("You bind $N's soul to yourself.", ch, NULL, victim, TO_CHAR);
  act("$n binds $N's soul to $mself.", ch, NULL, victim, TO_ROOM);

  return;
}

void spell_forcecage( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( CAN_DETECT(victim, ADET_PROTECTOR) )
    {
      if (victim == ch)
        send_to_char("You have already your forcecage around you.\n\r",ch);
      else
        act("$N has already a forcecage around $mself.",ch,NULL,victim,TO_CHAR);
        return;
    }

    af.where     = TO_DETECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = level / 6;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = ADET_PROTECTOR;
    affect_to_char( victim, &af );
    act( "$n calls in arcane powers to build a cage of power around $mself.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "You call in arcane powers to build a cage of power around you.\n\r", victim );
    return;
}

void spell_iron_body( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( CAN_DETECT(victim, ADET_PROTECTOR) )
    {
      if (victim == ch)
        send_to_char("Your skin is already as hard as iron.\n\r",ch);
      else
        act("$N's skin is already as hard as iron.",ch,NULL,victim,TO_CHAR);
        return;
    }

    af.where     = TO_DETECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = level / 6;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = ADET_PROTECTOR;
    affect_to_char( victim, &af );
    act( "$n skin is now as hard as iron.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "Your skin is now as hard as iron.\n\r", victim );
    return;
}

void spell_elemental_sphere( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( CAN_DETECT(victim, ADET_PROTECTOR) )
    {
      if (victim == ch)
        send_to_char("An elemental sphere is already protecting you.\n\r",ch);
      else
        act("An elemental sphere is already protecting $N.",ch,NULL,victim,TO_CHAR);
        return;
    }

    af.where     = TO_DETECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = level / 6;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = ADET_PROTECTOR;
    affect_to_char( victim, &af );
    act( "$n uses all elemental powers to build a sphere around $mself.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "You use all elemental powers to build a sphere around you.\n\r", victim );
    return;
}


void spell_aura_of_chaos( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if (ch->cabal != victim->cabal)
      {
	send_to_char("You may only use this spell on fellow cabal members.\n\r",ch);
	return;
      }

    if ( is_affected( victim, sn ) )
    {
	if (victim == ch)
	  send_to_char("You are already protected by gods of chaos.\n\r",ch);
	else
	  act("$N is already protected by aura of chaos.",ch,NULL,victim,TO_CHAR);
	return;
    }

    af.where	 = TO_DETECTS;
    af.type      = sn;
    af.level	 = level;
    af.duration  = 24;
    af.modifier  = 0;
    af.location  = APPLY_NONE;
    af.bitvector = ADET_AURA_CHAOS;
    affect_to_char( victim, &af );
    send_to_char( "You feel the gods of chaos protect you.\n\r", victim );
    if ( ch != victim )
	act("An aura appears around $N.",ch,NULL,victim,TO_CHAR);
    return;
}
