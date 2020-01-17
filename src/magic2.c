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
        send_to_char( "Ba�aramad�n.\n\r", ch );
        return;
    }

    stone = get_hold_char(ch);
    if (!IS_IMMORTAL(ch)
    &&  (stone == NULL || stone->item_type != ITEM_WARP_STONE))
    {
      send_to_char("B�y� i�in gereken bile�ene sahip de�ilsin.\n\r",ch);
	return;
    }

    if (stone != NULL && stone->item_type == ITEM_WARP_STONE)
    {
      act("$p i�indeki g�ce y�kleniyorsun.",ch,stone,NULL,TO_CHAR);
     	act("$p alev al�yor ve yokoluyor!",ch,stone,NULL,TO_CHAR);
     	extract_obj(stone);
    }

    portal = create_object(get_obj_index(OBJ_VNUM_PORTAL),0);
    portal->timer = 2 + level / 25;
    portal->value[3] = victim->in_room->vnum;

    obj_to_room(portal,ch->in_room);

    act("$p havaya y�kseliyor.",ch,portal,NULL,TO_ROOM);
    act("$p �n�nde havaya y�kseliyor.",ch,portal,NULL,TO_CHAR);
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
        send_to_char( "Ba�aramad�n.\n\r", ch );
        return;
    }

    stone = get_hold_char(ch);
    if (!IS_IMMORTAL(ch)
    &&  (stone == NULL || stone->item_type != ITEM_WARP_STONE))
    {
      send_to_char("B�y� i�in gereken bile�ene sahip de�ilsin.\n\r",ch);
        return;
    }

    if (stone != NULL && stone->item_type == ITEM_WARP_STONE)
    {
      act("$p i�indeki g�ce y�kleniyorsun.",ch,stone,NULL,TO_CHAR);
      act("$p alev al�yor ve yokoluyor!",ch,stone,NULL,TO_CHAR);
        extract_obj(stone);
    }

    /* portal one */
    portal = create_object(get_obj_index(OBJ_VNUM_PORTAL),0);
    portal->timer = 1 + level / 10;
    portal->value[3] = to_room->vnum;

    obj_to_room(portal,from_room);

    act("$p havaya y�kseliyor.",ch,portal,NULL,TO_ROOM);
    act("$p �n�nde havaya y�kseliyor.",ch,portal,NULL,TO_CHAR);

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
      act("$p havaya y�kseliyor.",to_room->people,portal,NULL,TO_ROOM);
    	act("$p havaya y�kseliyor.",to_room->people,portal,NULL,TO_CHAR);
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

  act_color("$S ince ���k ���n� seni $C### PAR�ALIYOR ###$c!",
  victim, NULL, ch, TO_CHAR, POS_RESTING, CLR_RED );
  act_color( "$s ince ���k ���n� $M $C### PAR�ALIYOR ###$c!",
  ch, NULL, victim, TO_NOTVICT, POS_RESTING, CLR_RED );
  act_color( "�nce ���k ���n�n $M $C### PAR�ALIYOR ###$c!",
  ch, NULL, victim, TO_CHAR, POS_RESTING, CLR_RED );
  send_to_char( "� L D � R � L D � N!\n\r", victim );

  act("$N art�k yok!\n\r", ch, NULL, victim, TO_CHAR);
  act("$N art�k yok!\n\r", ch, NULL, victim, TO_ROOM);

  send_to_char("Birka� dakikal���na yenilmez bir hayalete d�n��t�n.\n\r",victim);
  send_to_char("Tabii bir�eylere sald�rmad���n s�rece.\n\r", victim);

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

  send_to_char("Zehirli duman bulutu oday� dolduruyor.\n\r",ch);
  act("Zehirli duman bulutu oday� dolduruyor.",ch,NULL,NULL,TO_ROOM);

  for (tmp_vict=ch->in_room->people;tmp_vict!=NULL;
       tmp_vict=tmp_vict->next_in_room)
    if (!is_safe_spell(ch,tmp_vict,TRUE))
      {
	if (!IS_NPC(ch) && tmp_vict != ch &&
	    ch->fighting != tmp_vict && tmp_vict->fighting != ch &&
	    (IS_SET(tmp_vict->affected_by,AFF_CHARM) || !IS_NPC(tmp_vict)))
	  {
	    if (!can_see(tmp_vict, ch))
		do_yell(tmp_vict, (char*)"�mdat biri bana sald�r�yor!");
	    else
	      {
	         sprintf(buf,"Geber %s, seni b�y�c� k�pek!",
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

  send_to_char("Bir toz bulutu oday� dolduruyor.\n\r",ch);
  act("Bir toz bulutu oday� dolduruyor.",ch,NULL,NULL,TO_ROOM);


  for (tmp_vict=ch->in_room->people;tmp_vict!=NULL;
       tmp_vict=tmp_vict->next_in_room)
    if (!is_safe_spell(ch,tmp_vict,TRUE))
      {
	if (!IS_NPC(ch) && tmp_vict != ch &&
	    ch->fighting != tmp_vict && tmp_vict->fighting != ch &&
	    (IS_SET(tmp_vict->affected_by,AFF_CHARM) || !IS_NPC(tmp_vict)))
	  {
	    if (!can_see(tmp_vict, ch))
		do_yell(tmp_vict, (char*)"�mdat biri bana sald�r�yor!");
	    else
	      {
	         sprintf(buf,"Geber %s, seni b�y�c� k�pek!",
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
  send_to_char("Derin zaten kabuklarla kaplanm��.\n\r",ch);
else
  act("$N olabildi�ince sert zaten.",ch,NULL,victim,TO_CHAR);
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
    act( "$s derisi a�a� kabuklar�yla kaplan�yor.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "Derin a�a� kabuklar�yla kaplan�yor.\n\r", victim );
    return;
}

void spell_bear_call( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *gch;
  CHAR_DATA *bear;
  CHAR_DATA *bear2;
  AFFECT_DATA af;
  int i;

  send_to_char("Ay�lar� yard�m�na �a��r�yorsun.\n\r",ch);
  act("$n ay�lar� �a��r�yor.",ch,NULL,NULL,TO_ROOM);

  if (is_affected(ch, sn))
    {
      send_to_char( "Onlar� kontrol edecek g�c� nas�l �a��racaks�n?\n\r", ch);
      return;
    }
  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  gch->pIndexData->vnum == MOB_VNUM_BEAR)
	{
    send_to_char( "Sahip oldu�un ay�yla bir sorunun mu var?",ch);
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
  send_to_char("�ki ay� yard�m�na geliyor!\n\r",ch);
  act("$s yard�m�na iki ay� geliyor!",ch,NULL,NULL,TO_ROOM);

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
  send_to_char("Bir korucu asas� yarat�yorsun!\n\r",ch);
  act("$n bir korucu asas� yarat�yor!",ch,NULL,NULL,TO_ROOM);

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
      send_to_char( "Ba�aramad�n.\n\r", ch );
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
      send_to_char( "Ba�aramad�n.\n\r", ch );
      return;
  }


  act("$n k���k bir k�reyi yere f�rlat�yor.", ch, NULL, NULL, TO_ROOM );

  if (!IS_NPC(ch) && ch->fighting != NULL && number_bits(1) == 1) {
    send_to_char("Ba�aramad�n.\n\r",ch);
    return;
  }

  act( "$n gitti!",victim,NULL,NULL,TO_ROOM);

  char_from_room( victim );
  char_to_room( victim, pRoomIndex );
  act( "$n ortaya ��k�yor.", victim, NULL, NULL, TO_ROOM );
  do_look( victim, (char*)"auto" );
  stop_fighting(victim,TRUE);
  return;
}

void spell_transform( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  AFFECT_DATA af;

  if (is_affected(ch,sn) || ch->hit > ch->max_hit)
    {
      send_to_char("Zaten heryerinden sa�l�k f��k�r�yor.\n\r",ch);
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


  send_to_char( "Sa�l���n artt�k�a akl�n kar���yor.\n\r",ch);
}

void spell_mana_transfer( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;

  if (victim == ch)
    {
      send_to_char("Kendi kendine mana transferi yaparsan patlars�n.\n\r",ch);
      return;
    }

  if (ch->cabal != victim->cabal)
    {
      send_to_char("Bu b�y�y� yaln�z kendi kabal arkada�lar�nda kullanabilirsin.\n\r",ch);
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
      act( "Zihin b��a��n $S akl�n� kavuruyor!",ch,NULL,victim,TO_CHAR);
      act("$s zihin b��a�� senin akl�n� kavuruyor!",ch,NULL,victim,TO_VICT);
      act("$s zihin b��a�� $S akl�n� kavuruyor!",ch,NULL,victim,TO_NOTVICT);
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
      send_to_char("Ba�ka bir iblis �a��rmaya g�c�n kalmad�.\n\r",ch);

      return;
    }

    send_to_char("Bir iblis �a��rmay� deniyorsun.\n\r",ch);
    act("$n bir iblis �a��rmay� deniyor.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  gch->pIndexData->vnum == MOB_VNUM_DEMON)
	{
    send_to_char( "�ki iblisi kontrol edemezsin!\n\r",ch);
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
  send_to_char("Yerin �ok �ok alt�ndan bir iblis geliyor!\n\r",ch);
  act("Yerin �ok �ok alt�ndan bir iblis geliyor!",ch,NULL,NULL,TO_ROOM);

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
      do_say(demon, (char*)"Beni rahats�z etmeye nas�l c�ret edersin?!!!");
    else
      do_say(demon, (char*)"Beni rahats�z etmeye c�ret eden kim?!!!");
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
		do_yell(tmp_vict, (char*)"�mdat biri bana sald�r�yor!");
	      else
		{
		  sprintf(buf,"Geber %s, seni b�y�c� k�pek!",
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
      act("Zaten onun gibi g�r�n�yorsun.",ch,NULL,victim,TO_CHAR);
      return;
    }

  if (IS_NPC(victim))
    {
      act("$N taklit edemeyece�in kadar farkl�.",ch,NULL,victim,TO_CHAR);
     return;
   }

  if (IS_IMMORTAL(victim))
    {
      send_to_char("Ak�ll� ol.\n\r",ch);
      return;
    }

  if (saves_spell(level,victim, DAM_CHARM))
   {
    send_to_char("Ba�aramad�n.\n\r",ch);
    return;
   }

   act("$E benzemek i�in formunu de�i�tiriyorsun.",ch,NULL,victim,TO_CHAR);
   act("$n SANA benzemek i�in formunu de�i�tiriyor!",ch,NULL,victim,TO_VICT);
   act("$n $E benzemek i�in formunu de�i�tiriyor!",ch,NULL,victim,TO_NOTVICT);

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
      act("Fakat $N aranm�yor.",ch,NULL,victim,TO_CHAR);
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

  act("$p yarat�yorsun!",ch,shield,NULL,TO_CHAR);
  act("$n $p yarat�yor!",ch,shield,NULL,TO_ROOM);
}

void spell_guard_call( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *gch;
  CHAR_DATA *guard;
  CHAR_DATA *guard2;
  CHAR_DATA *guard3;
  AFFECT_DATA af;
  char buf[] = "Muhaf�zlar!";
  int i;

  if (is_affected(ch,sn))
    {
      send_to_char( "�� muhaf�z daha �a��rmaya g�c�n kalmad�.\n\r",ch);
      return;
    }

  do_yell(ch, buf);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch
	  && gch->pIndexData->vnum == MOB_VNUM_SPECIAL_GUARD)
	{
	  do_say(gch, (char*)"Nas�l yani? Ben yeterince iyi de�il miyim?");
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
  send_to_char( "Yard�m�na �� muhaf�z geliyor!\n\r",ch);
  act("$s yard�m�na �� muhaf�z geliyor!",ch,NULL,NULL,TO_ROOM);

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
      send_to_char("Bir solucan �a��racak g�c� bulam�yorsun.\n\r",ch);

      return;
    }

    send_to_char("Bir solucan �a��rmay� denedin.\n\r",ch);
    act("$n bir solucan �a��rmay� denedi.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  gch->pIndexData->vnum == MOB_VNUM_NIGHTWALKER)
	{
    send_to_char( "�ki solucan� kontrol edemezsin!\n\r",ch);
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
  send_to_char("G�lgelerin i�inden bir solucan y�kseliyor!\n\r",ch);
  act("G�lgelerin i�inden bir solucan y�kseliyor!",ch,NULL,NULL,TO_ROOM);
  sprintf(buf, "Bir solucan �n�nde e�iliyor.");
  send_to_char(buf, ch);
  sprintf(buf, "Bir solucan %s �n�nde e�iliyor!", ch->name );
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
        send_to_char("Casus a��n �yle birine rastlam�yor.\n\r",ch);
	return;
      }

    if ((victim->level > ch->level + 7)
	|| saves_spell((ch->level + 9), victim, DAM_NONE))
      {
        send_to_char("Casus a��n �yle birini bulam�yor.\n\r",ch);
	return;
      }

    if (ch==victim)
      do_look( ch, (char*)"auto" );
    else {
      ori_room = ch->in_room;
      char_from_room( ch );
      char_to_room( ch, victim->in_room );
      do_look( ch, (char*)"auto" );
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
        send_to_char("Bu b�y�y� ancak kabal dostlar�n �zerinde kullanabilirsin.\n\r",ch);
	return;
      }

    if ( is_affected( victim, sn ) )
    {
	if (victim == ch)
  send_to_char("Zaten bir g�lge peleriniyle korunuyorsun.\n\r",ch);
else
  act("$N zaten bir g�lge peleriniyle korunuyor.",ch,NULL,victim,TO_CHAR);
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
    send_to_char("G�lgelerin seni korudu�unu hissediyorsun.\n\r", victim );
    if ( ch != victim )
	act("Bir g�lge pelerini $M koruyor.",ch,NULL,victim,TO_CHAR);
    return;
}

void spell_nightfall( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *vch;
  OBJ_DATA  *light;
  AFFECT_DATA af;

  if ( is_affected( ch, sn ) )
  {
    send_to_char( "I��klar� kontrol edebilecek g�c�n yok.\n\r", ch );
    return;
  }

  for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
    for (light = vch->carrying; light != NULL; light = light->next_content)
      {
	if (light->item_type == ITEM_LIGHT && light->value[2] != 0
		&& !is_same_group(ch, vch)) {
	  if ( /*light->value[2] != -1 ||*/ saves_spell(level, vch, DAM_ENERGY))
	  {
      act("$p titreyerek s�n�yor!",ch,light,NULL,TO_CHAR);
	    act("$p titreyerek s�n�yor!",ch,light,NULL,TO_ROOM);
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
      act("$p titreyerek s�n�yor!",ch,light,NULL,TO_CHAR);
      act("$p titreyerek s�n�yor!",ch,light,NULL,TO_ROOM);
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
    send_to_char("Yaln�z oyuncular aynalanabilir.\n\r",ch);
    return;
  }

  for (mirrors = 0, gch = char_list; gch != NULL; gch = gch->next)
    if (IS_NPC(gch) && is_affected(gch,gsn_mirror)
	&& is_affected(gch,gsn_doppelganger) && gch->doppel == victim)
      mirrors++;

if (mirrors >= level/5) {
  if (ch==victim)
  send_to_char("Daha fazla aynalanamazs�n.\n\r",ch);
else
  act("$N daha fazla aynalanamaz.",ch,NULL,victim,TO_CHAR);
  return;
}

  af.where     = TO_AFFECTS;
  af.level     = level;
  af.modifier  = 0;
  af.location  = 0;
  af.bitvector = 0;

  for (tmp_vict = victim; is_affected(tmp_vict,gsn_doppelganger);
       tmp_vict = tmp_vict->doppel);

       sprintf(long_buf, "%s%s burada.\n\r", tmp_vict->name, tmp_vict->pcdata->title);
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
      send_to_char("Sana ait bir yalanc� g�r�nt� �n�nde beliriyor!\n\r",ch);
      act("$s �n�nde kendisine ait bir yalanc� g�r�nt� beliriyor!",ch,NULL,victim,TO_ROOM);
    }
    else {
      act("$S �n�nde kendisine ait bir yalanc� g�r�nt� beliriyor!",ch,NULL,victim,TO_CHAR);
      act("$S �n�nde kendisine ait bir yalanc� g�r�nt� beliriyor!",ch,NULL,victim,TO_NOTVICT);
      send_to_char("Sana ait bir yalanc� g�r�nt� �n�nde beliriyor!\n\r",victim);
    }

  }
}

void spell_garble( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  AFFECT_DATA af;

  if (ch == victim)
  {
    send_to_char("Kimin konu�mas�n� s�r�eceksin?\n\r",ch);
    return;
  }

  if (is_affected(victim,sn))
  {
    act("$S zaten s�r�-i lisan ediyor.",ch,NULL,victim,TO_CHAR);
    return;
  }

  if (is_safe_nomessage(ch,victim)) {
    send_to_char("Onun konu�mas�n� s�r�emezsin.\n\r",ch);
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

  act("$S konu�mas�n� s�r��yorsun!",ch,NULL,victim,TO_CHAR);
  send_to_char("S�r�-i lisan ediyorsun.\n\r",victim);
}

void spell_confuse( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  AFFECT_DATA af;
  CHAR_DATA *rch;
  int count=0;

  if (is_affected(victim,gsn_confuse)) {
    act("$N zaten �a��rm��.",ch,NULL,victim,TO_CHAR);
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
      send_to_char( "Bir uyu�ukluk dalgas� taraf�ndan ele ge�iriliyorsun.\n\r",victim );
    	act( "$n derin bir uykuya dald�.", victim, NULL, NULL, TO_ROOM );
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
  ("Kassandra bu i� i�in yeni kullan�ld�.\n\r",ch);
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
    send_to_char( "S�cak bir dalga v�cudunu sar�yor.\n\r", ch );
    act("$n daha iyi g�r�n�yor.", ch, NULL, NULL, TO_ROOM);
    return;
}


void spell_sebat( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  AFFECT_DATA af;

  if ( is_affected( ch, sn ) )
    {
      send_to_char("Kasandra bu i� i�in yeni kullan�ld�.\n\r",ch);

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
  act("$n mistik bir kalkanla �evrelendi.",ch, NULL,NULL,TO_ROOM );
  send_to_char( "Mistik bir kalkanla �evrelendin.\n\r", ch );
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
      ("Kasandra bu i� i�in yeni kullan�ld�.\n\r",ch);
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

    act("An�lar�n�n ak�p gitti�ini hissediyorsun.",victim,NULL,NULL,TO_CHAR);
    act("$n bo� g�zlerle etrafa bak�yor.",victim,NULL,NULL,TO_ROOM);
}


void spell_chaos_blade( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  OBJ_DATA *blade;
  AFFECT_DATA af;

  blade = create_object( get_obj_index(OBJ_VNUM_CHAOS_BLADE),level);
  send_to_char("Bir yata�an yarat�yorsun!\n\r",ch);
  act("$n bir yata�an yarat�yor!",ch,NULL,NULL,TO_ROOM);

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
      act("$N sana tapamayacak kadar aptal!", ch, NULL, victim, TO_CHAR);
      return;
    }

  for (i = 0; i < MAX_RELIGION; i++)
    {
      if (!str_cmp(ch->name, religion_table[i].leader))
	{
  	  tattoo = get_eq_char(victim, WEAR_TATTOO);
   	  if (tattoo != NULL)
    	    {
            act("$N zaten d�vmelenmi�!  �nce di�erini ��karmal�s�n.",
		ch, NULL, victim, TO_CHAR);
      	      act("$n sana ikinci bir d�vme vermeye �al��t�, ama ba�aramad�.",
		ch, NULL, victim, TO_VICT);
              act("$n $E ikinci bir d�vme vermeye �al��t�, ama ba�aramad�.",
		ch, NULL, victim, TO_NOTVICT);
      	      return;
    	    }
  	  else
    	    {
      	      tattoo = create_object(get_obj_index(religion_table[i].vnum),60);
              act("$E $p d�vmesi kaz�yorsun!",ch, tattoo, victim, TO_CHAR);
      	      act("$n $E $p d�vmesi kaz�yor!",ch,tattoo,victim,TO_NOTVICT);
      	      act("$n sana $p d�vmesi kaz�yor!",ch,tattoo,victim,TO_VICT);

      	      obj_to_char(tattoo,victim);
      	      equip_char(victim, tattoo, WEAR_TATTOO);
	      return;
	    }
	}
    }
    send_to_char("Dini bir d�vmen yok.\n\r", ch);
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
      act( "Ac�l� bir i�lemle $n d�vmeni kald�r�yor.",
	ch, NULL, victim, TO_VICT);
      act("$S d�vmesini kald�r�yorsun.", ch, NULL, victim, TO_CHAR);
      act("$n $S d�vmesini kald�r�yor.", ch, NULL, victim, TO_NOTVICT);
    }
  else
  act("$S hi� d�vmesi yok.", ch, NULL, victim, TO_CHAR);
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
      act( "Tanr�lar $M koruyor.", ch, NULL, victim, TO_ROOM );
      return;
    }

  if ( IS_NEUTRAL(victim) )
    {
      act("$N etkilenmi� g�r�nm�yor.", ch, NULL, victim, TO_CHAR );
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

  send_to_char( "Kirlenmi� hissediyorsun.\n\r", victim );
  if ( ch != victim )
    act("$N rahats�z g�r�n�yor.",ch,NULL,victim,TO_CHAR);
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
      ("Bu i� i�in t�kenmi� durumdas�.\n\r",ch);
      return;
    }
  if (IS_SET(ch->in_room->room_flags, ROOM_LAW))
    {
      send_to_char(
        "Bu oday� kar��t�rmak g�c�n�n �tesinde.\n\r", ch);
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
      send_to_char("T�m �abalar�na ra�men evren kaosa direniyor.\n\r",ch);
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
  send_to_char("Oda rasgele da��l�mla etkilendi!\n\r", ch);
  send_to_char( "�aban�n ard�ndan t�kendi�ini hissediyorsun.\n\r", ch);
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
      send_to_char("Ba�aramad�n.\n\r", ch);
      return;
    }

  if (is_affected(ch,sn))
    {
      send_to_char("Bu g�� yak�n zamanda kullan�ld�.\n\r", ch);
      return;
    }

  if (!is_safe_nomessage(ch,victim) && !IS_SET(ch->act,PLR_CANINDUCT) )
    {
      send_to_char("Bu i� i�in �zel muhaf�zlar� kullansan daha iyi sonu� verir.\n\r", ch);
      return;
    }

    send_to_char("Bir avc� �a��rmaya �al���yorsun.\n\r",ch);
    act("$n bir avc� �a��rmaya �al���yor.",ch,NULL,NULL,TO_ROOM);

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
  send_to_char("G�r�nmez bir avc� seni avlamaya geldi!\n\r",victim);
  act("G�r�nmez bir avc� $m avlamaya geldi!",victim,NULL,NULL,TO_ROOM);
  send_to_char("G�r�nmez bir avc� g�nderildi.\n\r", ch);

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
	send_to_char("Ba�aramad�n.\n\r",ch);
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
      send_to_char( "Ba�aramad�n.\n\r", ch );
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
    act( "$n garip s�zc�kler f�s�ldarken �evrende aksayan zaman� hissediyorsun.",ch, NULL, wch, TO_VICT);
          if(victim->in_room == NULL) {
		bug("Tesseract: victim room has become NULL!!!",0);
                return;
          }
	  char_from_room(wch);
	  char_to_room(wch,victim->in_room);
    act("$n aniden geliyor.",wch,NULL,NULL,TO_ROOM);
	  if (wch->in_room == NULL)
	     bug("Tesseract: other char sent to NULL room",0);
	  else
	     do_look(wch,(char*)"auto");
	}
    }

    act( "Bir ���k patlamas�yla $n ve arkada�lar� beliriyor!",ch,NULL,NULL,TO_ROOM);
    send_to_char("Tuhaf s�zc�kleri f�s�ldad���n anda zaman ve mekan bulan�kla�arak duruluyor.\n\r",ch);
  char_from_room(ch);
  char_to_room(ch,victim->in_room);

  act("$n aniden geliyor.",ch,NULL,NULL,TO_ROOM);
  if (ch->in_room == NULL)
     bug("Tesseract: char sent to NULL room",0);
  else
     do_look(ch,(char*)"auto");

  if (gate_pet)
    {
      send_to_char( "Zaman ve mekan�n kayd���n� hissediyorsun.\n\r",ch->pet);
      char_from_room(ch->pet);
      char_to_room(ch->pet,victim->in_room);
      act("$n aniden geliyor.",ch->pet,NULL,NULL,TO_ROOM);
      if (ch->pet->in_room == NULL)
         bug("Tesseract: pet sent to NULL room",0);
      else
         do_look(ch->pet,(char*)"auto");
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
        send_to_char("O bir iksire d�n��t�r�lemez.\n\r",ch);
	return;
      }

    if (obj->wear_loc != -1)
      {
        send_to_char("Mayalanacak �ey ta��nmal�.\n\r",ch);
	return;
      }

    for( vial=ch->carrying; vial != NULL; vial=vial->next_content )
      if ( vial->pIndexData->vnum == OBJ_VNUM_POTION_VIAL )
        break;
    if (  vial == NULL )  {
      send_to_char( "�ksiri i�ine mayalayabilece�in bir iksir �i�en yok.\n\r", ch );
	return;
    }


    if (number_percent() < 50)
      {
        send_to_char("Ba�aramad�n ve onu yokettin.\n\r", ch);
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
    act("Elindekilerle $p mayal�yorsun!", ch, potion, NULL, TO_CHAR);
    act("$n elindekilerle $p mayal�yor!", ch, potion, NULL, TO_ROOM);

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
      send_to_char( "Bunu neden yapmak istiyorsun?!?\n\r", ch);
      return;
    }

  if (is_affected(ch,sn))
    {
      send_to_char( "G�lgeye can verecek g�c�n yok.\n\r",ch);
      return;
    }

    act("$S g�lgesine ya�am veriyorsun!",ch, NULL, victim, TO_CHAR);
    act("$n $S g�lgesine ya�am veriyor!",ch,NULL,victim,TO_NOTVICT);
    act("$n g�lgene ya�am veriyor!", ch, NULL, victim, TO_VICT);

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
    send_to_char("Fakat sen ba�ka bir�ey giyiyorsun.\n\r", ch );
    return;
  }

  for ( badge = ch->carrying; badge != NULL;
       badge = obj_next )
    {
      obj_next = badge->next_content;
      if (badge->pIndexData->vnum == OBJ_VNUM_DEPUTY_BADGE
	  || badge->pIndexData->vnum == OBJ_VNUM_RULER_BADGE)
	{
    act("Sendeki $p yokoluyor.",ch, badge, NULL, TO_CHAR);
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
  act("H�k�m ni�an�n� giyiyorsun!",ch, NULL, NULL, TO_CHAR);
  act("$n h�k�m ni�an�n� giyiyor!", ch, NULL, NULL, TO_ROOM);

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
    act("$p yokoluyor.",ch, badge, NULL, TO_CHAR);
	  act("$s $p yokoluyor.", ch, badge, NULL, TO_ROOM);

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
      send_to_char("Zaten ejderhan�n g�c�yle dolusun.\n\r",ch);
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

  send_to_char( "Ejderhan�n g�c� ruhuna kat�l�yor.\n\r", ch);
  act("$n biraz daha ac�mas�z g�r�n�yor.", ch, NULL, NULL, TO_ROOM);
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
    send_to_char("Zaten alt�n bir aurayla korunuyorsun.\n\r",ch);
  else
    act( "$N zaten alt�n bir aurayla korunuyor.",ch,NULL,vch,TO_CHAR);
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

      send_to_char( "Etraf�n� saran alt�n aylay� hissediyorsun.\n\r", vch );
      if ( ch != vch )
	act("Alt�n bir ayla $M �evreliyor.",ch,NULL,vch,TO_CHAR);

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

  act("$p yaratt�n!",ch,plate,NULL,TO_CHAR);
  act("$n $p yaratt�!",ch,plate,NULL,TO_ROOM);
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
      send_to_char( "�u an ba�ka bir a�ay� y�netemezsin.\n\r",ch);
      return;
    }

    send_to_char("Bir a�a �a��rmay� denedin.\n\r",ch);
    act("$n bir a�a �a��rmay� denedi.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  gch->pIndexData->vnum == MOB_VNUM_SQUIRE)
	{
    send_to_char("�ki a�a ihtiya� duydu�undan daha fazla de�il mi?!\n\r",ch);
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
  send_to_char("Bir a�a hi�likten geliyor!\n\r",ch);
  act("Bir a�a hi�likten geliyor!",ch,NULL,NULL,TO_ROOM);

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

  if (!str_cmp(arg, "k�l��"))
    sword_vnum = OBJ_VNUM_DRAGONSWORD;
  else if (!str_cmp(arg, "topuz"))
    sword_vnum = OBJ_VNUM_DRAGONMACE;
  else if (!str_cmp(arg, "han�er"))
    sword_vnum = OBJ_VNUM_DRAGONDAGGER;
  else if (!str_cmp(arg, "karg�"))
    sword_vnum = OBJ_VNUM_DRAGONLANCE;
  else
    {
      send_to_char( "Ne t�r bir silah yapmak istiyorsun? K�l��, topuz, kama, karg�?", ch);
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

  act("$p yaratt�n!",ch,sword,NULL,TO_CHAR);
  act("$n $p yaratt�!",ch,sword,NULL,TO_ROOM);
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
      send_to_char("Burada bitki b�y�yemez.\n\r", ch);
      return;
    }

  dam = number_range(level, 4 * level);
  if ( saves_spell( level, victim, DAM_PIERCE ) )
    dam /= 2;

  damage(ch,victim,ch->level,gsn_entangle,DAM_PIERCE, TRUE);

  act("Yerden f�rlayan sarma��klar $s bacaklar�na dolan�yor!", victim,
      NULL, NULL, TO_ROOM);
  act("Yerden f�rlayan sarma��klar ayaklar�na dolan�yor!", victim,
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
      send_to_char("Zaten zarara kar�� korunuyorsun.",ch);
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
  act("$n zarardan korunmaya ba�lad�.",ch,NULL,NULL,TO_ROOM );
  send_to_char("Zarara kar�� korunmaya ba�lad�n.\n\r", ch );

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

  send_to_char("�evredekileri izlemeyi seviyorsun.\n\r", ch);
}

void spell_protective_shield( int sn, int level, CHAR_DATA *ch, void *vo, int target ) {
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  AFFECT_DATA af;

  if ( is_affected( victim, sn ) )
    {
      if (victim == ch)
      send_to_char( "Zaten koruyucu bir kalkanla �evrilisin.\n\r",ch);
    else
      act("$N zaten koruyucu bir kalkanla �evrili.",ch,NULL,
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
  act( "$n koruyucu bir kalkanla �evrelendi.",victim,NULL,NULL,TO_ROOM );
  send_to_char("Koruyucu bir kalkanla �evrelendin.\n\r", victim );
  return;
}

void spell_deafen( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  AFFECT_DATA af;

  if (ch == victim) {
    send_to_char("Kimi sa��r edeceksin?\n\r",ch);
    return;
  }

  if (is_affected(victim,sn)) {
    act("$N zaten sa��r.",ch,NULL,victim,TO_CHAR);
    return;
  }

  if (is_safe_nomessage(ch,victim)) {
    send_to_char("Onu sa��r edemezsin.\n\r",ch);
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

  act("$M sa��r ettin!",ch,NULL,victim,TO_CHAR);
  send_to_char( "G�r�lt�l� bir ��nlama kulaklar�n� dolduruyor...art�k duyam�yorsun!\n\r",victim);
}

void spell_disperse( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *vch;
  CHAR_DATA *vch_next;
  ROOM_INDEX_DATA *pRoomIndex;
  AFFECT_DATA af;

  if ( is_affected( ch, sn ) )
    {
      send_to_char("Yeterli da��tma g�c�n yok.\n\r",ch);
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

      send_to_char("D�nya �evrende d�n�yor!\n\r",vch);
  	  act( "$n yokoluyor!", vch, NULL, NULL, TO_ROOM );
  	  char_from_room( vch );
  	  char_to_room( vch, pRoomIndex );
  	  act( "$n yava��a beliriyor.", vch, NULL, NULL, TO_ROOM );
	  do_look( vch, (char*)"auto" );
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
      send_to_char("Onurun zaten seni koruyor.\n\r", ch);
    else
send_to_char(  "Onlar zaten onurlar�yla korunuyorlar.\n\r", ch);
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

  send_to_char("Onurun seni koruyor.\n\r", victim);
  act("$s onuru kendisini koruyor.", victim, NULL, NULL, TO_ROOM);
}

void spell_acute_vision( int sn, int level, CHAR_DATA *ch, void *vo, int target )
 {
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( CAN_DETECT(victim, ACUTE_VISION) )
    {
        if (victim == ch)
        send_to_char("G�r���n yeterince g��l�. \n\r",ch);
      else
        act("$S g�r��� zaten g��l�.",ch,NULL,victim,TO_CHAR);
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
    send_to_char("G�r���n keskinle�iyor.\n\r", victim );
    if ( ch != victim )
        send_to_char( "Tamam.\n\r", ch );
    return;
}

void spell_dragons_breath(int sn,int level,CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    CHAR_DATA *vch, *vch_next;
    int dam,hp_dam,dice_dam;
    int hpch;
    char buf[MAX_STRING_LENGTH];

    act("Ejderha lordunu yard�ma �a��r�yorsun.",ch,NULL,NULL,TO_CHAR);
    act("$n bir ejderha gibi solumaya ba�l�yor.",ch,NULL,victim,TO_NOTVICT);
    act("$s solumas�ndan rahats�z oluyorsun!",ch,NULL,victim,TO_VICT);
    act("Ejderha lordunun nefesini �ekiyorsun.",ch,NULL,NULL,TO_CHAR);

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
                do_yell(vch, (char*)"�mdat biri bana sald�r�yor!");
            else
              {
                 sprintf(buf,"Geber %s, seni b�y�c� k�pek!",
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
    send_to_char( "Hi� kum bulam�yorsun.\n\r",ch);
	 ch->wait = 0;
	 return;
	}

  act("$n yerdeki kumla f�rt�na yarat�yor.",ch,NULL,NULL,TO_ROOM);
  act("�ey yarat�yorsun...kum f�rt�nas�.",ch,NULL,NULL,TO_CHAR);

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

    act("$n rahats�z edici bir sesle �I�LIK at�yor!.",ch,NULL,NULL,TO_ROOM);
    act("G��l� bir sesle ���l�k at�yorsun.",ch,NULL,NULL,TO_CHAR);

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
      send_to_char("Belki de �ans�n� kar�� cinste denemelisin!\n\r", ch );
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
      send_to_char("Daha fazla vampirle�emezsin!\n\r",ch);
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

    send_to_char( "Gitgide irile�ti�ini hissediyorsun.\n\r", ch );
    act("$m tan�yam�yorsun.",ch,NULL,NULL,TO_ROOM);
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
      send_to_char("Yaln�z cesetleri canland�rabilirsin!\n\r",ch);
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
      send_to_char("Daha fazla hortlak bedenini kontrol edecek g�c� bulamazs�n.\n\r", ch);
      return;
    }

  if (count_charmed(ch)) return;

  if ( ch->in_room != NULL && IS_SET(ch->in_room->room_flags, ROOM_NO_MOB) )
  {
    send_to_char("�l�leri burada canland�ramazs�n.\n\r", ch );
     return;
  }

  if ( IS_SET(ch->in_room->room_flags, ROOM_SAFE)      ||
       IS_SET(ch->in_room->room_flags, ROOM_PRIVATE)   ||
       IS_SET(ch->in_room->room_flags, ROOM_SOLITARY)  )
  {
    send_to_char( "Burada canland�ramazs�n.\n\r", ch );
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

  sprintf(buf, "%s beden hortlak", obj->name);
  undead->name = str_dup(buf);
  sprintf(buf2,"%s",obj->short_descr);
  argument = (char*)alloc_perm ( MAX_STRING_LENGTH );
  arg = (char*)alloc_perm ( MAX_STRING_LENGTH );
  argument = buf2;
  buf3[0] = '\0';
  while (argument[0] != '\0' )
	{
  argument = one_argument(argument, arg);
  if (!(!str_cmp(arg,"hortlak") || !str_cmp(arg,"beden") ||
	!str_cmp(arg,"ceset") ))
	 {
	  if (buf3[0] == '\0')   strcat(buf3,arg);
	  else  {
		 strcat(buf3," ");
		 strcat(buf3,arg);
		}
	 }
	}
  sprintf(buf, "%s'in hortlak bedeni", buf3);
  undead->short_descr = str_dup(buf);
  sprintf(buf, "%s'in hortlak bedeni y�r�rken sendeliyor.\n\r", buf3);
  undead->long_descr = str_dup(buf);

  for(obj2 = obj->contains;obj2;obj2=next)
    {
	next = obj2->next_content;
	obj_from_obj(obj2);
	obj_to_char(obj2, undead);
    }
    interpret( undead,(char*)"giy t�m�", TRUE);

  af.where	 = TO_AFFECTS;
  af.type      = sn;
  af.level	 = ch->level;
  af.duration  = (ch->level / 10);
  af.modifier  = 0;
  af.bitvector = 0;
  af.location  = APPLY_NONE;
  affect_to_char( ch, &af );

  send_to_char("Mistik bir g��le onu canland�r�yorsun!\n\r",ch);
  sprintf(buf,"Mistik bir g��le %s %s'i canland�r�yor!",ch->name,obj->name);
  act(buf,ch,NULL,NULL,TO_ROOM);
  sprintf(buf,"%s sana bakarken onu rahats�z etmenin bedelini �detmeyi planl�yor!",obj->short_descr);
  act(buf,ch,NULL,NULL,TO_CHAR);
  extract_obj (obj);
	return;
    }

    victim = (CHAR_DATA *) vo;

    if ( victim == ch )
    {
      send_to_char( "Fakat sen �l� de�ilsin!!\n\r", ch );
    	return;
        }

        send_to_char("Fakat o �l� de�il!!\n\r", ch );
    return;
}



void spell_enhanced_armor( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
    {
	if (victim == ch)
  send_to_char("Zaten geli�mi� z�rhla kapl�s�n.\n\r",ch);
else
  act("$N zaten geli�mi� z�rhla kapl�.",ch,NULL,victim,TO_CHAR);
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
    send_to_char("Her t�r sald�r�ya kar�� daha iyi korunuyorsun.\n\r", victim );
    if ( ch != victim )
	act("$N senin b�y�nle korunuyor.",ch,NULL,victim,TO_CHAR);
    return;
}



void spell_meld_into_stone( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  AFFECT_DATA af;

  if ( is_affected( victim, sn ) )
    {
      if (victim == ch)
      send_to_char("Derin zaten ta�la kapl�.\n\r",ch);
    else
      act("$S derisi zaten ta�la kapl�.",ch,NULL,
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
  act( "$s derisi ta�a d�n���yor.",victim,NULL,NULL,TO_ROOM );
  send_to_char( "Derin ta�a d�n���yor.\n\r", victim );
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
  send_to_char("Zaten a�l�s�n.\n\r",ch);
else
  act("$N zaten a�l�.",ch,NULL,victim,TO_CHAR);
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
    send_to_char( "Kal�n a�larla tutulmu�sun!\n\r", victim );
    if ( ch != victim )
	act("$M kal�n a�larla tutuyorsun!",ch,NULL,victim,TO_CHAR);
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
            send_to_char("Zaten z�rhl�s�n.\n\r",ch);
      		    else
      			act( "$N zaten z�rhl�.", ch, NULL, gch, TO_CHAR);
		  continue;
		}

		af.type      = armor_sn;
		af.level     = level;
		af.duration  = level;
		af.location  = APPLY_AC;
		af.modifier  = -20;
		af.bitvector = 0;
		affect_to_char( gch, &af );

    send_to_char( "Birinin seni korudu�unu hissediyorsun.\n\r",gch);
		if( ch != gch )
    act("$N senin b�y�nle korunuyor.",
				ch, NULL, gch, TO_CHAR );

		if( !is_same_group( gch, ch ) )
			continue;
		if( is_affected( gch, shield_sn ) )
		{
		  if( gch == ch)
      send_to_char("Zaten kalkanl�s�n.\n\r",ch);
		  else
		        act("$N zaten kalkanl�.", ch, NULL, gch, TO_CHAR);
		  continue;
		}

		af.type      = shield_sn;
		af.level     = level;
		af.duration  = level;
		af.location  = APPLY_AC;
		af.modifier   = -20;
		af.bitvector = 0;
		affect_to_char( gch, &af );

    send_to_char("Bir g�� kalkan�yla �evrelendin.\n\r",gch);
		if( ch != gch )
			act("$N bir g�� kalkan�yla �evrelendi.",
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
      send_to_char("Zaten ilhaml�s�n.\n\r",ch);
   else
      act("$N zaten ilhaml�.",ch, NULL, gch, TO_CHAR);
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

    send_to_char( "Ruhuna i�leyen ilham� hissediyorsun!\n\r", gch );
		if( ch != gch )
			act( "$M yarat�c�n�n g�c�yle uyand�r�yorsun!",
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
      send_to_char("Zaten takdislisin.\n\r", ch);
		  else
		    act( "$N zaten takdisli.", ch, NULL, gch, TO_CHAR);
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

    send_to_char( "Beyaz bir aurayla �evrelendin.\n\r",gch );
		if( ch != gch )
			act("$N beyaz bir aurayla �evrelendi.",
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
      send_to_char("Onu onarmaya gerek yok.\n\r",ch);
    return;
    }

    if (obj->wear_loc != -1)
    {
      send_to_char("Onar�lacak �ey ta��n�yor olmal�.\n\r",ch);
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
      act("$p parlamaya ba�l�yor.  �yi i�ti!",ch,obj,NULL,TO_CHAR);
    	act("$p parlamaya ba�l�yor.",ch,obj,NULL,TO_ROOM);
	obj->condition += result;
	obj->condition = UMIN( obj->condition , 100 );
	return;
    }

    else if ( result >=10)
    {
      send_to_char("De�i�en bir �ey olmad�.\n\r",ch);
	return;
    }

    else
    {
      act("$p alev alarak buharla��yor!",ch,obj,NULL,TO_CHAR);
    	act("$p alev alarak buharla��yor!",ch,obj,NULL,TO_ROOM);
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
    act("$N bir an i�in titriyor.",
  		ch, NULL, victim, TO_CHAR );
  	send_to_char("Bir an i�in titriyorsun.\n\r",victim);
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
    act("$M ruh ak�nt�lar�yla �rt�yorsun.",
		ch, NULL, victim, TO_CHAR );
	  send_to_char("Kalkanlaman�n artt���n� hissediyorsun.\n\r",victim);
	  return;
	}

    af.type	= sn;
    af.level    = level;
    af.duration = level / 15;
    af.location = APPLY_NONE;
    af.modifier	= 0;
    af.bitvector = 0;
    affect_join( victim, &af );

    send_to_char( "Bir�eylerle ba�lant�n� yitirdi�ini hissediyorsun.\n\r",victim );
    act("$M Ger�ek Kaynak'tan ay�r�yorsun.", ch, NULL, victim, TO_CHAR);
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


  act_color( "Parma��ndan ��kan karanl�k $M sar�yor.",
  ch, NULL, victim, TO_CHAR, POS_RESTING, CLR_RED );
  act_color( "$s parma��ndan ��kan karanl�k $M sar�yor.",
  ch, NULL, victim, TO_NOTVICT, POS_RESTING, CLR_RED );
  act_color( "$S parma��ndan ��kan karanl�k seni sar�yor.",
		victim, NULL, ch, TO_CHAR, POS_RESTING, CLR_RED );

    if (saves_spell(level,victim,DAM_MENTAL) || number_bits(1) == 0)
	{
	 dam = dice( level , 24 ) ;
	 damage(ch, victim , dam , sn, DAM_MENTAL, TRUE);
	 return;
	}

  send_to_char( "� L D � R � L D � N!\n\r", victim );

  act("$N �ld�r�ld�!\n\r", ch, NULL, victim, TO_CHAR);
  act("$N �ld�r�ld�!\n\r", ch, NULL, victim, TO_ROOM);

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
  send_to_char("Kendi isminle an�lacak KILICINI yaratt�n.\n\r",ch);
	send_to_char("Bu silah� bir tane daha yaratamayaca��n� bil.\n\r",ch);
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
    send_to_char("Kimin �ld�r�lmesini istiyorsun?\n\r",ch);
	 return;
	}

  if ( (victim = get_char_area(ch,arg)) == NULL)
	{
    send_to_char("Etrafta bu isimde biri yok.\n\r",ch);
	 return;
	}
  if (is_safe_nomessage(ch,victim))
	{
    send_to_char("Tanr�lar kurban�n� koruyor.\n\r",ch);
	 return;
	}

  send_to_char("Bir avc� aslan �a��r�yorsun.\n\r",ch);
  act("$n bir avc� aslan �a��r�yor.",ch,NULL,NULL,TO_ROOM);

  if (is_affected(ch,sn))
    {
      send_to_char( "Daha fazla aslan� kontrol edecek g�c�n yok.\n\r", ch);
      return;
    }

  if ( ch->in_room != NULL && IS_SET(ch->in_room->room_flags, ROOM_NO_MOB) )
  {
    send_to_char( "Hi�bir aslan seni dinlemiyor.\n\r", ch );
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
    send_to_char("Hi�bir avc� aslan sana gelemez.\n\r", ch );
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

  send_to_char("Bir avc� aslan kurban�n� �ld�rmeye geldi!\n\r",ch);
  act("Bir avc� aslan $s kurban�n� �ld�rmeye geldi!",ch,NULL,NULL,TO_ROOM);

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
    send_to_char("Kendini daha fazla seviyorsun.\n\r",ch);
	return;
	}

    if (IS_NPC(victim))
	{
	send_to_char("Kurban�n bir oyuncu! Buna gerek yok.\n\r",ch);
	return;
	}

    if (saves_spell(level ,victim,DAM_MENTAL))
       {
        send_to_char("Ba�aramad�n.\n\r",ch);
        return;
       }

    for( vial=ch->carrying; vial != NULL; vial=vial->next_content )
      if ( vial->pIndexData->vnum == OBJ_VNUM_POTION_VIAL )
        break;

    if (  vial == NULL )  {
      send_to_char( "��ine kurban�n�n ruhunu koyabilece�in bir �i�en yok.\n\r", ch );

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
 sprintf(buf,"%s'in ruhunu yakalay�p �i�enin i�ine koyuyorsun.\n\r",victim->name);
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
    	   "You raise your hand and a blinding ray of light shoots forth!\n\r",ch);
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
  act( "$n ka�t�!", victim, NULL, NULL, TO_ROOM );
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
      send_to_char("Bu g�� yak�n zamanda kullan�ld�.",ch);
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
      send_to_char("Enerji i�inde patl�yor!\n\r",ch);
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
    send_to_char( "Kurban�n bu g�c�n �tesinde.\n\r", ch );
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
    send_to_char( "Bir tav�an kadar korkuyorsun.\n\r", victim );
    act("$n korkan g�zlerle bak�yor.",victim,NULL,NULL,TO_ROOM);
    return;
}

void spell_protection_heat ( int sn, int level, CHAR_DATA *ch, void *vo , int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected(victim, gsn_protection_heat) )
    {
	if (victim == ch)
  send_to_char("Zaten s�caktan korunuyorsun.\n\r",ch);
else
  act("$N zaten s�caktan korunuyor.",ch,NULL,victim,TO_CHAR);
	return;
    }

    if ( is_affected(victim, gsn_protection_cold) )
    {
	if (victim == ch)
  send_to_char("Zaten so�uktan korunuyorsun.\n\r",ch);
else
  act("$N zaten so�uktan korunuyor.",ch,NULL,victim,TO_CHAR);
	return;
    }

    if ( is_affected(victim, gsn_fire_shield) )
    {
	if (victim == ch)
  send_to_char("Zaten ate� kalkan� kullan�yorsun.\n\r",ch);
else
  act("$N zaten ate� kalkan� kullan�yor.",ch,NULL,victim,TO_CHAR);
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
    send_to_char( "S�ca�a kar�� g��lendi�ini hissediyorsun.\n\r", victim );
    if ( ch != victim )
	act("$N s�ca�a kar�� korunmaya ba�lad�.",ch,NULL,victim,TO_CHAR);
    return;
}

void spell_protection_cold ( int sn, int level, CHAR_DATA *ch, void *vo , int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected(victim, gsn_protection_cold) )
    {
	if (victim == ch)
  send_to_char("Zaten so�uktan korunuyorsun.\n\r",ch);
else
  act("$N zaten so�uktan korunuyor.",ch,NULL,victim,TO_CHAR);
	return;
    }

    if ( is_affected(victim, gsn_protection_heat) )
    {
	if (victim == ch)
  send_to_char("Zaten s�caktan korunuyorsun.\n\r",ch);
else
  act("$N zaten s�caktan korunuyor.",ch,NULL,victim,TO_CHAR);
	return;
    }

    if ( is_affected(victim, gsn_fire_shield) )
    {
	if (victim == ch)
  send_to_char("Zaten ate� kalkan� kullan�yorsun.\n\r",ch);
else
  act("$N zaten ate� kalkan� kullan�yor.",ch,NULL,victim,TO_CHAR);
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
    send_to_char( "So�u�a kar�� g��lendi�ini hissediyorsun.\n\r", victim );
    if ( ch != victim )
	act("$N so�u�a kar�� korunmaya ba�lad�.",ch,NULL,victim,TO_CHAR);
    return;
}

void spell_fire_shield ( int sn, int level, CHAR_DATA *ch, void *vo , int target)
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *fire;
    int i;

    target_name = one_argument(target_name, arg);
    if (!(!str_cmp(arg,"buz") || !str_cmp(arg,"ate�")))
	{
	send_to_char("Kalkan�n t�r�n� belirtmelisin.\n\r",ch);
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
  send_to_char("Ate� kalkan� yarat�yorsun.\n\r",ch);
 return;
}

void spell_witch_curse ( int sn, int level, CHAR_DATA *ch, void *vo , int target)
{
  AFFECT_DATA af;
  CHAR_DATA *victim = (CHAR_DATA *) vo;

  if (is_affected(victim,gsn_witch_curse))
    {
      send_to_char("Zaten ya�am� ak�p gidiyor.\n\r",ch);
      return;
    }

  if (saves_spell((level+5),victim,DAM_MENTAL) || number_bits(1) == 0)
  {
      send_to_char("Ba�aramad�n!\n\r",ch);
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


  send_to_char("�imdi �l�m yoluna girdi.\n\r",ch);
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
      send_to_char("Hangi y�n� ya da kap�y� �alacaks�n.\n\r",ch);
    return;
    }

    if (ch->fighting)
    {
      send_to_char("D�v�� bitene kadar bekle.\n\r",ch);
	return;
    }

    if ( ( door = find_door( ch, arg ) ) >= 0 )
    {
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit = ch->in_room->exit[door];
	if ( !IS_SET(pexit->exit_info, EX_CLOSED) )
  { send_to_char("Zaten a��k.\n\r",      ch ); return; }
if ( !IS_SET(pexit->exit_info, EX_LOCKED) )
  { send_to_char("Sadece a�may� dene.\n\r",     ch ); return; }
if ( IS_SET(pexit->exit_info, EX_NOPASS) )
  { send_to_char( "Mistik bir g�� ��k��� koruyor.\n\r",ch );
	      return; }
    chance = ch->level / 5 + get_curr_stat(ch,STAT_INT) + get_skill(ch,sn) / 5;

    act("$d'yi �al�yor ve a�maya �al���yorsun!",
		ch,NULL,pexit->keyword,TO_CHAR);
    act("$n $d'yi �al�yor ve a�maya �al���yor!",
		ch,NULL,pexit->keyword,TO_ROOM);

    if (room_dark(ch->in_room))
		chance /= 2;

    /* now the attack */
    if (number_percent() < chance )
     {
	REMOVE_BIT(pexit->exit_info, EX_LOCKED);
	REMOVE_BIT(pexit->exit_info, EX_CLOSED);
  act("$n $d'yi �al�yor ve kap� a��l�yor.", ch, NULL,
		pexit->keyword, TO_ROOM );
	send_to_char( "Kap�y� a�may� ba�ard�n.\n\r", ch );

	/* open the other side */
	if ( ( to_room   = pexit->u1.to_room            ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != NULL
	&&   pexit_rev->u1.to_room == ch->in_room )
	{
	    CHAR_DATA *rch;

	    REMOVE_BIT( pexit_rev->exit_info, EX_CLOSED );
	    REMOVE_BIT( pexit_rev->exit_info, EX_LOCKED );
	    for ( rch = to_room->people; rch != NULL; rch = rch->next_in_room )
      act( "$d a��l�yor.", rch, NULL, pexit_rev->keyword, TO_CHAR );
	}
     }
    else
     {
       act("$d'yi a�amad�n!",
     	    ch,NULL,pexit->keyword,TO_CHAR);
     	act("$n $d'yi a�amad�.",
	    ch,NULL,pexit->keyword,TO_ROOM);
     }
    return;
    }

    send_to_char("Onu g�remiyorsun.\n\r",ch);
  return;
}


void spell_magic_resistance ( int sn, int level, CHAR_DATA *ch, void *vo , int target)
{
  AFFECT_DATA af;

    if (!is_affected(ch, sn))
    {
      send_to_char("Art�k b�y�ye diren�lisin.\n\r", ch);

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
  send_to_char("B�y�ye zaten diren�lisin.\n\r", ch);
 return;
}

void spell_hallucination ( int sn, int level, CHAR_DATA *ch, void *vo , int target)
{
  send_to_char( "That spell is under construction.\n\r",ch);
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
      send_to_char("Ba�ka bir kurt �a��racak g�c�n yok.\n\r",ch);
      return;
    }

    send_to_char("Bir kurt �a��rmay� deniyorsun.\n\r",ch);
    act("$n bir kurt �a��rmay� deniyor.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  gch->pIndexData->vnum == MOB_VNUM_WOLF)
	{
    send_to_char("�ki kurt kontrol edebilece�inden fazla!\n\r",ch);
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
  send_to_char("Kurt geliyor ve �n�nde e�iliyor!\n\r",ch);
  act("Biryerlerden bir kurt gelerek selam veriyor!",ch,NULL,NULL,TO_ROOM);

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
      send_to_char("Derin bir kaya akdar sert zaten.\n\r",ch);

      else
      act("$S derisi bir kaya kadar sert zaten.",ch,NULL,
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
  act( "$s derisi art�k bir kaya kadar sert.",victim,NULL,NULL,TO_ROOM );
  send_to_char("Derin art�k bir kaya kadar sert.\n\r", victim );
  return;
}


void spell_mind_light(int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    AFFECT_DATA af,af2;

    if ( is_affected_room( ch->in_room, sn ))
    {
      send_to_char("Bu odan�n mana h�zland�r�c�s� var zaten.\n\r",ch);
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
    send_to_char( "Oda zihin �����yla dolmaya ba�l�yor.\n\r", ch );
    act("Oda $s zihin �����yla dolmaya ba�l�yor.",ch,NULL,NULL,TO_ROOM);
    return;
}

void spell_insanity ( int sn, int level, CHAR_DATA *ch, void *vo , int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if (IS_NPC(ch))
	{
    send_to_char("Bu b�y� oyunculara yap�labilir.\n\r",ch);
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
    send_to_char( "Bir battlerager kadar sald�rgans�n.\n\r", victim );
    act("$n k�z�l g�zlerle bak�yor.",victim,NULL,NULL,TO_ROOM);
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
    send_to_char( "Sersemledin.\n\r", victim );
    act_color("$n sersemledi.",victim,NULL,NULL,TO_ROOM,POS_SLEEPING,CLR_RED);
    return;
}



void spell_improved_invis( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_IMP_INVIS) )
	return;

  act( "$n ortadan kayboluyor.", victim, NULL, NULL, TO_ROOM );

    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = level / 10 ;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_IMP_INVIS;
    affect_to_char( victim, &af );
    send_to_char( "G�r�nmez oluyorsun.\n\r", victim );
    return;
}



void spell_improved_detection( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( CAN_DETECT(victim, DETECT_IMP_INVIS) )
    {
	if (victim == ch)
  send_to_char("Geli�mi� g�r�nmezli�i zaten saptayabiliyorsun.\n\r",ch);
else
  act("$N zaten geli�mi� g�r�nmezli�i saptayabiliyor.",ch,NULL,victim,TO_CHAR);
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
    send_to_char( "G�zlerin �ok yan�yor.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Tamam.\n\r", ch );
    return;
}

void spell_severity_force( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    char buf[MAX_STRING_LENGTH];
    int dam;

    sprintf(buf,"Arz� %s'e do�ru k�r�yorsun.\n\r",victim->name);
    send_to_char(buf, ch);
    act("$n arz� sana do�ru k�r�yor!", ch, NULL, victim, TO_VICT );
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
      ("Rasgele g�c�n t�kenmi� durumda.\n\r",ch);
      return;
    }

  if (IS_SET(ch->in_room->room_flags, ROOM_LAW))
    {
      send_to_char(
	      "Bu oda seninkinin �tesinde bir g��le korunuyor.\n\r",ch);
      return;
    }
    if ( is_affected_room( ch->in_room, sn ))
    {
      send_to_char("Bu odaya zaten rasgele uygulanm��.\n\r",ch);
	return;
    }

  if (number_bits(1) == 0)
    {
      send_to_char("Evren karma�aya direniyor.\n\r",ch);
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
    send_to_char("Oda rasgele ile �ekillendirildi!\n\r", ch);
    send_to_char("Kendini t�kenmi� hissediyorsun.\n\r", ch);
    ch->hit -= UMIN(200, ch->hit/2);
    act("Odan�n ��k��lar� kar���yor.",ch,NULL,NULL,TO_ROOM);
    return;
}

void spell_bless_weapon(int sn,int level,CHAR_DATA *ch, void *vo,int target)
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    AFFECT_DATA af;

    if (obj->item_type != ITEM_WEAPON)
    {
      send_to_char("O bir silah de�il.\n\r",ch);
	return;
    }

    if (obj->wear_loc != -1)
    {
      send_to_char("Ta��d���n �eyi kutsayabilirsin.\n\r",ch);
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
       act("$p kutsanmam�� g�r�n�yor.",ch,obj,NULL,TO_CHAR);
	return;
     }
    }
    if (IS_WEAPON_STAT(obj,WEAPON_HOLY))
    {
      act("$p zaten kutsanm��.",ch,obj,NULL,TO_CHAR);
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

    act("$p kutsal sald�r�lara haz�rlan�yor.",ch,obj,NULL,TO_ALL);
    return;

}

void spell_resilience( int sn, int level, CHAR_DATA *ch, void *vo , int target)
{
  AFFECT_DATA af;

    if (!is_affected(ch, sn))
    {
      send_to_char("Emici sald�r�lara kar�� esnekle�tin.\n\r", ch);

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
  send_to_char("Emici sald�r�lara kar�� ZATEN esneksin.\n\r",ch);
 return;
}

void spell_super_heal( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int bonus = 170 + level + dice(1,20);

    victim->hit = UMIN( victim->hit + bonus, victim->max_hit );
    update_pos( victim );
    send_to_char("S�cak bir duygu v�cudunu sar�yor.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Tamam.\n\r", ch );
    return;
}

void spell_master_heal( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int bonus = 300 + level + dice(1,40);

    victim->hit = UMIN( victim->hit + bonus, victim->max_hit );
    update_pos( victim );
    send_to_char( "S�cak bir duygu v�cudunu sar�yor.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Tamam.\n\r", ch );
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
    send_to_char( "S�cak bir duygu v�cudunu sar�yor.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Tamam.\n\r", ch );
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
      send_to_char("Ba�ka bir golem yaratacak g�c�n yok.\n\r",ch);

      return;
    }

    send_to_char("K���k bir golem yaratmay� deniyorsun.\n\r",ch);
    act( "$n k���k bir golem yaratmay� deniyor.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  ( gch->pIndexData->vnum == MOB_VNUM_LESSER_GOLEM ) )
	{
	  i++;
	  if (i > 2)
           {
             send_to_char("Daha fazla golemi kontrol alt�ndan tutamazs�n!\n\r",ch);
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
  send_to_char("K���k bir golem yarat�yorsun!\n\r",ch);
  act("$n k���k bir golem yarat�yor!",ch,NULL,NULL,TO_ROOM);

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
      send_to_char(   "Ba�ka bir golem yaratacak g�c�n yok.\n\r",ch);
      return;
    }

    send_to_char("Bir ta� golem yaratmay� deniyorsun.\n\r",ch);
    act("$n bir ta� golem yaratmay� deniyor.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  ( gch->pIndexData->vnum == MOB_VNUM_STONE_GOLEM ) )
	{
	  i++;
	  if (i > 2)
           {
             send_to_char(	"Daha fazla golemi kontrol alt�ndan tutamazs�n!\n\r",ch);
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
  send_to_char("Bir ta� golem yarat�yorsun!\n\r",ch);
  act("$n bir ta� golem yarat�yor!",ch,NULL,NULL,TO_ROOM);

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
      send_to_char( "Ba�ka bir golem yaratacak g�c�n yok.\n\r",ch);
      return;
    }

    send_to_char( "Bir demir golem yaratmay� deniyorsun.\n\r",ch);
    act("$n bir demir golem yaratmay� deniyor.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  ( gch->pIndexData->vnum == MOB_VNUM_IRON_GOLEM ) )
	{
    send_to_char("Daha fazla golemi kontrol alt�ndan tutamazs�n!\n\r",ch);
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
  send_to_char("Bir demir golem yarat�yorsun!\n\r",ch);
  act("$n bir demir golem yarat�yor!",ch,NULL,NULL,TO_ROOM);

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
      send_to_char( "Ba�ka bir golem yaratacak g�c�n yok.\n\r",ch);
      return;
    }

    send_to_char("Bir sertta�� golem yaratmay� deniyorsun.\n\r",ch);
    act("$n bir sertta�� golem yaratmay� deniyor.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  ( gch->pIndexData->vnum == MOB_VNUM_ADAMANTITE_GOLEM ) )
	{
    send_to_char("Daha fazla golemi kontrol alt�ndan tutamazs�n!\n\r",ch);
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
  send_to_char( "Bir sertta�� golem yaratt�n!\n\r",ch);
  act("$n bir sertta�� golem yaratt�!",ch,NULL,NULL,TO_ROOM);

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
      send_to_char("Ba�aramad�n.\n\r",ch);
      return;
    }

  if (IS_RAFFECTED(ch->in_room,AFF_ROOM_CURSE))
	{
	 affect_strip_room(ch->in_room,gsn_cursed_lands);
   send_to_char( "B�lgedeki lanet yokoluyor.\n\r",ch);
	 act("B�lgedeki lanet yokoluyor.\n\r",ch,NULL,NULL,TO_ROOM);
	}
  if (IS_RAFFECTED(ch->in_room,AFF_ROOM_POISON))
	{
	 affect_strip_room(ch->in_room,gsn_deadly_venom);
   send_to_char("B�lge daha sa�l�kl� g�r�n�yor.\n\r",ch);
	 act("B�lge daha sa�l�kl� g�r�n�yor.\n\r",ch,NULL,NULL,TO_ROOM);
	}
  if (IS_RAFFECTED(ch->in_room,AFF_ROOM_SLEEP))
	{
    send_to_char("B�lge gizemli uykusundan uyan�yor.\n\r",ch);
 	 act( "B�lge gizemli uykusundan uyan�yor.\n\r",ch,NULL,NULL,TO_ROOM);
	 affect_strip_room(ch->in_room,gsn_mysterious_dream);
	}
  if (IS_RAFFECTED(ch->in_room,AFF_ROOM_PLAGUE))
	{
    send_to_char("B�lgenin hastal��� yokoldu.\n\r",ch);
 	 act( "B�lgenin hastal��� yokoldu.\n\r",ch,NULL,NULL,TO_ROOM);
	 affect_strip_room(ch->in_room,gsn_black_death);
	}
  if (IS_RAFFECTED(ch->in_room,AFF_ROOM_SLOW))
	{
    send_to_char( "Uyu�ukluk sisi yokoluyor.\n\r",ch);
  	 act("Uyu�ukluk sisi yokoluyor.\n\r",ch,NULL,NULL,TO_ROOM);
	 affect_strip_room(ch->in_room,gsn_lethargic_mist);
	}
    return;
}


void spell_deadly_venom(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    AFFECT_DATA af;

  if (IS_SET(ch->in_room->room_flags, ROOM_LAW))
    {
      send_to_char( "Oda tanr�larca korunuyor.\n\r",  ch);
      return;
    }
    if ( is_affected_room( ch->in_room, sn ))
    {
      send_to_char( "Oda zaten �l�m zehriyle dolu.\n\r",ch);
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

    send_to_char("Oda zehirle dolmaya ba�l�yor.\n\r",ch);
    act("Oda zehirle dolmaya ba�l�yor.\n\r",ch,NULL,NULL,TO_ROOM);

}

void spell_cursed_lands(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    AFFECT_DATA af;

  if (IS_SET(ch->in_room->room_flags, ROOM_LAW))
    {
      send_to_char( "Oda tanr�larca korunuyor.\n\r",ch);
      return;
    }
    if ( is_affected_room( ch->in_room, sn ))
    {
      send_to_char("Oda zaten lanetli.\n\r",ch);
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

    send_to_char("Tanr�lar oday� ba��bo� b�rakt�lar.\n\r",ch);
    act("Tanr�lar oday� ba��bo� b�rakt�lar.\n\r",ch,NULL,NULL,TO_ROOM);

}

void spell_lethargic_mist(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
   AFFECT_DATA af;

  if (IS_SET(ch->in_room->room_flags, ROOM_LAW))
    {
      send_to_char("Oda tanr�larca korunuyor.\n\r",  ch);
      return;
    }
    if ( is_affected_room( ch->in_room, sn ))
    {
      send_to_char( "Oda zaten uyu�ukluk sisiyle dolu.\n\r",ch);
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

    send_to_char("Odan�n havas� seni yava�lat�yor.\n\r",ch);
    act("Odan�n havas� seni yava�lat�yor.\n\r",ch,NULL,NULL,TO_ROOM);

}

void spell_black_death(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
  AFFECT_DATA af;

  if (IS_SET(ch->in_room->room_flags, ROOM_LAW))
    {
      send_to_char("Oda tanr�larca korunuyor.\n\r",ch);
      return;
    }
    if ( is_affected_room( ch->in_room, sn ))
    {
      send_to_char( "Bu oda zaten hastal�kla dolu.\n\r",ch);
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

    send_to_char("Oda hastal�kla dolmaya ba�l�yor.\n\r",ch);
     act("Oda hastal�kla dolmaya ba�l�yor.\n\r",ch,NULL,NULL,TO_ROOM);

}

void spell_mysterious_dream(int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
  AFFECT_DATA af;

  if (IS_SET(ch->in_room->room_flags, ROOM_LAW))
    {
      send_to_char("Bu oda tanr�larca korunuyor.\n\r",  ch);
      return;
    }
    if ( is_affected_room( ch->in_room, sn ))
    {
      send_to_char("Bu oda zaten uyku gaz�n�n etkisinde.\n\r",ch);
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

    send_to_char("Uyumak i�in g�zel bir yer.\n\r",ch);
    act("Uyumak i�in ne kadar da g�zel bir yer.\n\r",ch,NULL,NULL,TO_ROOM);

}

void spell_polymorph( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    AFFECT_DATA af;
    int race;

    if ( is_affected( ch, sn ) )
    {
      send_to_char( "Zaten �eklin de�i�mi�.\n\r",ch);
     return;
    }

    if (target_name == NULL)
    {
      send_to_char( "Kullan�m: b�y� 'binbir �ekil' <�rk>.\n\r",ch);
     return;
    }

    race = race_lookup( target_name );

    if (race == 0 || !race_table[race].pc_race)
    {
      send_to_char("O d�n��ebilece�in bir�ey de�il.\n\r",ch);
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

    act("$n kendini $t yap�yor.",
		ch, race_table[race].name[1], NULL,TO_ROOM );
    act( "Kendini $t yap�yorsun.\n\r",
		ch, race_table[race].name[1], NULL,TO_CHAR );
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
    send_to_char("Burada de�il.\n\r",ch);
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
      send_to_char( "Yak�ndaki bir a�aca d�n��meye ba�l�yorsun!\n\r",ch);
      act("$n yak�ndaki bir a�aca d�n��meye ba�l�yor!", ch, NULL, NULL,TO_ROOM );
      af.bitvector = ADET_FORM_TREE;
    }
    else
    {
      send_to_char( "Bir tutam �imene d�n��meye ba�l�yorsun!\n\r",ch);
      act("$n bir tutam �imene d�n��meye ba�l�yor!", ch, NULL, NULL, TO_ROOM );
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

    act("$s �evresinde b��aklar olu�uyor ve $E �arp�yor.",
	ch,NULL,victim,TO_ROOM);
    act("�evrende b��aklar olu�uyor ve $E �arp�yor.",
	ch,NULL,victim,TO_CHAR);
    act("$s �evresinde b��aklar olu�uyor ve sana �arp�yor!",
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
          do_yell(victim, (char*)"�mdat biri bana sald�r�yor!");
        else
          {
            sprintf(buf,"Geber %s, seni b�y�c� k�pek!",
                    (is_affected(ch,gsn_doppelganger)&&!IS_IMMORTAL(victim))?
                    ch->doppel->name : ch->name);
            do_yell(victim,buf);
          }
      }

      act("B��ak duvar� $e �arp�yor!",victim,NULL,NULL,TO_ROOM);
    dam = dice(level,5);
    if (saves_spell(level,victim,DAM_PIERCE))
	dam /= 2;
    damage(ch,victim,dam,sn,DAM_PIERCE,TRUE);
    act("B��ak duvar� sana �arp�yor!",victim,NULL,NULL,TO_CHAR);

    if ( number_percent() < 50 ) return;

    act("B��ak duvar� $e �arp�yor!",victim,NULL,NULL,TO_ROOM);
    dam = dice(level,4);
    if (saves_spell(level,victim,DAM_PIERCE))
	dam /= 2;
    damage(ch,victim,dam,sn,DAM_PIERCE,TRUE);
    act("B��ak duvar� sana �arp�yor!",victim,NULL,NULL,TO_CHAR);

    if ( number_percent() < 50 ) return;

    act("B��ak duvar� $e �arp�yor!",victim,NULL,NULL,TO_ROOM);
    dam = dice(level,2);
    if (saves_spell(level,victim,DAM_PIERCE))
	dam /= 2;
    damage(ch,victim,dam,sn,DAM_PIERCE,TRUE);
    act("B��ak duvar� sana �arp�yor!",victim,NULL,NULL,TO_CHAR);

}


void spell_protection_negative ( int sn, int level, CHAR_DATA *ch, void *vo , int target)
{
  AFFECT_DATA af;

    if (!is_affected(ch, sn))
    {
      send_to_char( "Negatif sald�r�lara ba����kl� oldu�unu hissediyorsun.\n\r", ch);

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
  send_to_char( "Zaten negatif sald�r�lara ba����kl�s�n.\n\r", ch);
 return;
}


void spell_ruler_aura( int sn, int level, CHAR_DATA *ch, void *vo , int target)
{
  AFFECT_DATA af;

    if (!is_affected(ch, sn))
    {
      send_to_char( "Kendine ve ideallerine olan inanc�n g��leniyor.\n\r", ch);

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
  send_to_char( "Kendinden eminsin zaten.\n\r", ch);
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
    send_to_char( "B�lge zaten �erle dolu.\n\r",ch);
   return;
  }

 if ( is_affected( ch, sn ) )
    {
      send_to_char(  "Yeterince g�c�n yok.\n\r",ch);
      return;
    }

  if (IS_SET(ch->in_room->room_flags, ROOM_LAW)
	|| IS_SET(ch->in_room->area->area_flag,AREA_HOMETOWN) )
    {
      send_to_char(  "Odadaki kutsal aura seni engelliyor.\n\r",ch);
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
     act( "Oda �er ruhlar�n bilinciyle dolmaya ba�l�yor.",room->people,NULL,NULL,TO_ALL);
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

      act("$N kendinden emin de�il!",ch,NULL,victim,TO_ALL);
    }
  else send_to_char("Ba�aramad�n.\n\r",ch);
}


void spell_control_undead( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;

    if  ( !IS_NPC(victim) || !IS_SET(victim->act,ACT_UNDEAD) )
    {
      act("$N bir hortla�a benzemiyor.",ch,NULL,victim,TO_CHAR);
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
        send_to_char("Bu g�� yak�n zamanda kullan�ld�.\n\r",ch);
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
    send_to_char("S�cak bir his v�cudunu sar�yor.\n\r", victim );
    act("$n daha iyi g�r�n�yor.", victim, NULL, NULL, TO_ROOM);
    if (ch != victim) send_to_char("Tamam.\n\r",ch);
    return;
}



void spell_aid( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( ch, sn ) )
      {
        send_to_char("Bu g�� yak�n zamanda kullan�ld�.\n\r",ch);
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
    send_to_char( "S�cak bir his v�cudunu sar�yor.\n\r", victim );
    act("$n daha iyi g�r�n�yor.", victim, NULL, NULL, TO_ROOM);
    if (ch != victim) send_to_char("Tamam.\n\r",ch);
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
      send_to_char(  "Ba�ka bir g�lge �a��rmak i�in yeterli g�c�n yok.\n\r",ch);
      return;
    }

    send_to_char("Bir g�lge �a��rmay� deniyorsun.\n\r",ch);
    act("$n bir g�lge �a��rmay� deniyor.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  gch->pIndexData->vnum == MOB_VNUM_SUM_SHADOW)
	{
    send_to_char("�ki g�lge kontrol edebilece�inden fazla!\n\r",ch);
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
  act("Bir g�lge olu�uyor!",ch,NULL,NULL,TO_ALL);

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
        send_to_char("O uzakl��� g�remezsin.\n\r",ch);
	return;
      }

    if (ch->in_room == room)
      do_look( ch, (char*)"auto" );
    else
    {
      mount = MOUNTED(ch) ? 1 : 0;
      oldr = ch->in_room;
      char_from_room( ch );
      char_to_room( ch, room );
      do_look( ch, (char*)"auto" );
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
      send_to_char("Daha cesur hissediyorsun.\n\r",victim);
    	act("$n daha bilin�li g�r�n�yor.",victim,NULL,NULL,TO_ROOM);
    }
    else send_to_char("Ba�aramad�n.\n\r",ch);
}

void spell_desert_fist( int sn, int level, CHAR_DATA *ch, void *vo,int target )
 {
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    if ( (ch->in_room->sector_type != SECT_HILLS)
	&& (ch->in_room->sector_type != SECT_MOUNTAIN)
	&& (ch->in_room->sector_type != SECT_DESERT) )
	{
    send_to_char("Yumruk yaratmak i�in gerekli kumu bulam�yorsun.\n\r",ch);
	 ch->wait = 0;
	 return;
	}

  act("B�y�k bir kum k�tlesi y�kselerek bir yumruk olu�turuyor, ve $e vuruyor.",
  victim,NULL,NULL,TO_ROOM);
  act("B�y�k bir kum k�tlesi y�kselerek bir yumruk olu�turuyor, ve sana vuruyor.",
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
        send_to_char("Zaten kutsal bir aylan var.\n\r",ch);
       else
         act("$S zaten kutsal bir aylas� var.",ch,NULL,victim,TO_CHAR);
        return;
    }

    if ( !IS_GOOD(victim) )
    {
      send_to_char("O kutsal auraya lay�k de�il!", ch);
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

    send_to_char( "Kadim g��lerin seni korudu�unu hissediyorsun.\n\r", victim );
    if ( ch != victim )
        act("$N kadim kutsal g��lerce korunmaya ba�l�yor.",ch,NULL,victim,TO_CHAR);
    return;
}

void spell_holy_fury(int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if (is_affected(victim,sn) || IS_AFFECTED(victim,AFF_BERSERK))
    {
        if (victim == ch)
        send_to_char("Zaten hiddetlisin.\n\r",ch);
      else
        act("$N zaten hiddetli.",ch,NULL,victim,TO_CHAR);
        return;
    }

    if (is_affected(victim,skill_lookup("calm")))
    {
        if (victim == ch)
        send_to_char("Neden bir an i�in rahatlam�yorsun?\n\r",ch);
      else
        act("$N kavgaya merakl�ym�� gibi durmuyor.",
              ch,NULL,victim,TO_CHAR);
        return;
    }

    if ( !IS_GOOD(victim) )
    {
      act("$N kutsal hiddette lay�k de�il.",ch,NULL,victim,TO_CHAR);
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

    send_to_char("Kutsal hiddetle doluyorsun!\n\r",victim);
    act("$s g�zlerine hiddet doluyor!",victim,NULL,NULL,TO_ROOM);
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
    send_to_char( "Su kayna��na eri�emiyorsun.\n\r",ch);
	 ch->wait = 0;
	 return;
	}

  act("$s �evresindeki su molek�lleri birle�erek bir yumruk olu�turuyor.",
  ch,NULL,NULL,TO_ROOM);
  act("�evrendeki su molek�lleri birle�erek bir yumruk olu�turuyor.",
		ch,NULL,NULL,TO_CHAR);
    dam = dice( level , 14 );
    damage(ch,victim,dam,sn,DAM_BASH,TRUE);
}

void spell_wolf_spirit( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    AFFECT_DATA af;

    if ( is_affected( ch, sn ) )
    {
      send_to_char("Damarlar�ndaki kan zaten olabildi�ince h�zl� ak�yor!\n\r",ch);
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


    send_to_char( "Damarlar�ndaki kan h�zl� akmaya ba�l�yor.\n\r", ch );
    act("$s g�zleri KIZILLA�IYOR!",ch,NULL,NULL,TO_ROOM);
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
	send_to_char("Ba�aramad�n!\n\r", ch);
	return;
    }
  }

  if (!IS_NPC(victim) && IS_SET(victim->act, PLR_WANTED))
	dam = dice(level, 20);
  else	dam = dice(level, 14);

  if (saves_spell(level,victim, DAM_MENTAL))
	      dam /= 2;

  do_yell(ch, (char*)"Adalet K�l�c�!");
  act("Adalet k�l�c� ortaya ��karak $E vuruyor!",ch,NULL,victim,TO_ALL);

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
      send_to_char("Ba�ka bir k�pe�i �a��racak g�c�n yok.\n\r",ch);
      return;
    }

    send_to_char("Bir k�pek �a��rmay� deniyorsun.\n\r",ch);
    act("$n bir k�pek �a��rmay� deniyor.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  gch->pIndexData->vnum == MOB_VNUM_DOG)
	{
    send_to_char( "�ki k�pek kontrol edebilece�inden fazla!\n\r",ch);
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
  send_to_char(  "�ki k�pek geliyor ve �n�nde e�iliyor!\n\r",ch);
  act("�ki k�pek geliyor ve selam veriyor!",ch,NULL,NULL,TO_ROOM);

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
        send_to_char(	"Kaplan g�zlerin �yle birini g�rm�yor.\n\r",ch);
	return;
      }

    if ( IS_NPC(victim) || victim->cabal != CABAL_HUNTER)
    {
      send_to_char("Kaplan g�zleri yaln�z avc�lar� g�r�r!\n\r",ch);
	return;
    }

    if ((victim->level > ch->level + 7)
	|| saves_spell((ch->level + 9), victim, DAM_NONE))
      {
        send_to_char("Kaplan g�zlerin onu g�rm�yor.\n\r",ch);
	return;
      }

    if (ch==victim)
      do_look( ch, (char*)"auto" );
    else {
      ori_room = ch->in_room;
      char_from_room( ch );
      char_to_room( ch, victim->in_room );
      do_look( ch, (char*)"auto" );
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

  act("$p yarat�yorsun!",ch,shield,NULL,TO_CHAR);
  act("$n $p yarat�yor!",ch,shield,NULL,TO_ROOM);
}

void spell_evolve_lion( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  AFFECT_DATA af;

  if (is_affected(ch,sn) || ch->hit > ch->max_hit)
    {
      send_to_char( "Olabildi�ince aslans�n zaten.\n\r",ch);
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

  send_to_char("Kendini biraz sakar, fakat daha g��l� hissediyorsun.\n\r",ch);
  act("$s derisi grile�iyor!",ch,NULL,NULL,TO_ROOM);
}

void spell_prevent(int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    AFFECT_DATA af,af2;

    if ( is_affected_room( ch->in_room, sn ))
    {
      send_to_char("Bu odada zaten intikam al�nam�yor!\n\r",ch);
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
    send_to_char("Bu odada intikam al�namayacak!\n\r", ch );
    act("Oda $s Avc�lar� engelleyici g�c�yle dolmaya ba�l�yor",ch,NULL,NULL,TO_ROOM);
    return;
}


void spell_enlarge( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
    {
	if (victim == ch)
  send_to_char("Daha fazla geni�leyemezsin!\n\r",ch);
else
  act("$N olabildi�ince geni�lemi� zaten.",ch,NULL,victim,TO_CHAR);
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

    send_to_char("Gittik�e irile�ti�ini hissediyorsun.\n\r", victim );
    act("$s v�cudu geni�lemeye ba�lad�.",victim,NULL,NULL,TO_ROOM);
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
          act("$N zaten nefes alam�yor.\n\r",ch,NULL,victim,TO_CHAR);
         return;
        }

    if (saves_spell(level,victim,DAM_NEGATIVE) ||
        (IS_NPC(victim) && IS_SET(victim->act,ACT_UNDEAD)))
    {
        if (ch == victim)
        send_to_char("Bir an i�in kendini hasta hissediyorsun, ama sonra ge�iyor.\n\r",ch);
      else
        act("$N etkilenmi� g�r�nm�yor.",ch,NULL,victim,TO_CHAR);
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

    send_to_char ("Nefes alam�yorsun.\n\r",victim);
    act("$n nefes almaya �al���yor, ama ba�aram�yor.", victim,NULL,NULL,TO_ROOM);

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
      send_to_char("Yaln�z cesetleri canland�rabilirsin!\n\r",ch);
	return;
    }

    if (obj->level > level + 10)
    {
      send_to_char("�l� beden mumlayamayaca��n kadar g��l�!\n\r",ch);
	return;
    }

    if (is_affected(ch, sn))
    {
      send_to_char( "Daha fazla cesedi mumyalayacak g�c� bulam�yorsun.\n\r", ch);
      return;
    }

  if (count_charmed(ch)) return;

  if ( ch->in_room != NULL && IS_SET(ch->in_room->room_flags, ROOM_NO_MOB) )
  {
    send_to_char("�l�leri burada mumyalayamazs�n.\n\r", ch );
     return;
  }

  if ( IS_SET(ch->in_room->room_flags, ROOM_SAFE)      ||
       IS_SET(ch->in_room->room_flags, ROOM_PRIVATE)   ||
       IS_SET(ch->in_room->room_flags, ROOM_SOLITARY)  )
  {
    send_to_char( "Burada mumyalayamazs�n.\n\r", ch );
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

  sprintf(buf, "%s beden hortlak", obj->name);
  undead->name = str_dup(buf);
  sprintf(buf2,"%s",obj->short_descr);
  argument = (char *)alloc_perm ( MAX_STRING_LENGTH );
  arg = (char *)alloc_perm ( MAX_STRING_LENGTH );
  argument = buf2;
  buf3[0] = '\0';
  while (argument[0] != '\0' )
	{
  argument = one_argument(argument, arg);
  if (!(!str_cmp(arg,"hortlak") || !str_cmp(arg,"beden") ||
	!str_cmp(arg,"ceset") ))
	 {
	  if (buf3[0] == '\0')   strcat(buf3,arg);
	  else  {
		 strcat(buf3," ");
		 strcat(buf3,arg);
		}
	 }
	}
  sprintf(buf, "%s'in mumyalanm�� bedeni", buf3);
  undead->short_descr = str_dup(buf);
  sprintf(buf, "%s'in mumyalanm�� bedeni aksayarak dola��yor.\n\r", buf3);
  undead->long_descr = str_dup(buf);

  for(obj2 = obj->contains;obj2;obj2=next)
    {
	next = obj2->next_content;
	obj_from_obj(obj2);
	obj_to_char(obj2, undead);
    }
    interpret( undead,(char*)"giy t�m�", TRUE);

  af.where	 = TO_AFFECTS;
  af.type      = sn;
  af.level	 = ch->level;
  af.duration  = (ch->level / 10);
  af.modifier  = 0;
  af.bitvector = 0;
  af.location  = APPLY_NONE;
  affect_to_char( ch, &af );

  send_to_char("Mistik g��lerin yard�m�yla ona ya�am veriyorsun!\n\r",ch);
  sprintf(buf,"Mistik g��lerin yard�m�yla %s %s'i mumyalayarak ona ya�am veriyor!",
		ch->name,obj->name);
  act(buf,ch,NULL,NULL,TO_ROOM);
  sprintf(buf,"%s sana bakarak onu rahats�z etmenin bedelini �detmenin planlar�n� yap�yor!",
	obj->short_descr);
  act(buf,ch,NULL,NULL,TO_CHAR);
  extract_obj (obj);
	return;
    }

  victim = (CHAR_DATA *) vo;

  if ( victim == ch )
  {
    send_to_char( "Fakat sen �l� de�ilsin!!\n\r", ch );
  	return;
    }

    send_to_char("O �l� de�il!!\n\r", ch );
  return;
}

void spell_soul_bind( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;

  if (ch->pet != NULL)
  {
    send_to_char("Zaten ruhun birine ba�l�.\n\r", ch);
	return;
  }

  if (!IS_NPC(victim)
	|| !(IS_AFFECTED(victim, AFF_CHARM) && victim->master == ch))
  {
    send_to_char("O ruhu kendine ba�layamazs�n.\n\r", ch);
	return;
  }

  victim->leader = ch;
  ch->pet = victim;

  act("$S ruhunu kendine ba�l�yorsun.", ch, NULL, victim, TO_CHAR);
  act("$n $S ruhunu kendine ba�l�yor.", ch, NULL, victim, TO_ROOM);

  return;
}

void spell_forcecage( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( CAN_DETECT(victim, ADET_PROTECTOR) )
    {
      if (victim == ch)
      send_to_char("Zaten �evrende bir g�� kafesi var.\n\r",ch);
    else
      act("$S �evresinde bir g�� kafesi zaten var.",ch,NULL,victim,TO_CHAR);
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
    act( "$n �evresinde bir kafes olu�turmak i�in unutulmu� g��leri �a��r�yor.", victim, NULL, NULL, TO_ROOM );
    send_to_char("�evrende bir kafes olu�turmak i�in unutulmu� g��leri �a��r�yorsun.\n\r", victim );
    return;
}

void spell_iron_body( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( CAN_DETECT(victim, ADET_PROTECTOR) )
    {
      if (victim == ch)
      send_to_char( "Derin zaten demir gibi sert.\n\r",ch);
    else
      act("$S derisi zaten demir gibi sert.",ch,NULL,victim,TO_CHAR);
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
    act("$s derisi demir gibi sertle�ti.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "Derin demir gibi sertle�ti.\n\r", victim );
    return;
}

void spell_elemental_sphere( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( CAN_DETECT(victim, ADET_PROTECTOR) )
    {
      if (victim == ch)
      send_to_char( "Seni koruyan bir tanesi zaten var.\n\r",ch);
    else
      act("$M koruyan bir tanesi zaten var.",ch,NULL,victim,TO_CHAR);
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
    act("$n do�ayla ilgili t�m g��leri kendisine bir k�re yapmak i�in kullan�yor.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "Do�ayla ilgili t�m g��leri kendine bir k�re yapmak i�in kullan�yorsun.\n\r", victim );
    return;
}


void spell_aura_of_chaos( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if (ch->cabal != victim->cabal)
      {
        send_to_char("Bu b�y�y� yaln�z kabal dostlar�n �zerinde kullanabilirsin.\n\r",ch);
	return;
      }

    if ( is_affected( victim, sn ) )
    {
	if (victim == ch)
  send_to_char("Zaten kaos tanr�lar�nca korunuyorsun.\n\r",ch);
else
  act("$N zaten kaos tanr�lar�nca korunuyor.",ch,NULL,victim,TO_CHAR);
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
    send_to_char(  "Kaos tanr�lar�nca korundu�unu hissediyorsun.\n\r", victim );
    if ( ch != victim )
    act("$S �evresinde bir aura olu�uyor.",ch,NULL,victim,TO_CHAR);
    return;
}
