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
int	find_door	args( ( CHAR_DATA *ch, wchar_t *arg ) );
int	check_exit	args( ( wchar_t *argument ) );

extern wchar_t *target_name;


ROOM_INDEX_DATA * check_place( CHAR_DATA *ch, wchar_t *argument )
{
 EXIT_DATA *pExit;
 ROOM_INDEX_DATA *dest_room;
 int number,door;
 int range = (ch->level / 10) + 1;
 wchar_t arg[MAX_INPUT_LENGTH];

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
    ||   (IS_NPC(victim) && (victim->pIndexData->vnum == ch->pcdata->questmob))
    ||   (IS_NPC(victim) && saves_spell( level, victim,DAM_NONE) ) )
    {
        send_to_char( L"Başaramadın.\n\r", ch );
        return;
    }

    stone = get_hold_char(ch);
    if (!IS_IMMORTAL(ch)
    &&  (stone == NULL || stone->item_type != ITEM_WARP_STONE))
    {
      send_to_char( L"Büyü için gereken bileşene sahip değilsin.\n\r",ch);
	return;
    }

    if (stone != NULL && stone->item_type == ITEM_WARP_STONE)
    {
      act( L"$p içindeki güce yükleniyorsun.",ch,stone,NULL,TO_CHAR);
     	act( L"$p alev alıyor ve yokoluyor!",ch,stone,NULL,TO_CHAR);
     	extract_obj(stone);
    }

    portal = create_object(get_obj_index(OBJ_VNUM_PORTAL),0);
    portal->timer = 2 + level / 25;
    portal->value[3] = victim->in_room->vnum;

    obj_to_room(portal,ch->in_room);

    act( L"$p havaya yükseliyor.",ch,portal,NULL,TO_ROOM);
    act( L"$p önünde havaya yükseliyor.",ch,portal,NULL,TO_CHAR);
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
    ||   (IS_NPC(victim) && (victim->pIndexData->vnum == ch->pcdata->questmob))
    ||   (IS_NPC(victim) && saves_spell( level, victim,DAM_NONE) ) )
    {
        send_to_char( L"Başaramadın.\n\r", ch );
        return;
    }

    stone = get_hold_char(ch);
    if (!IS_IMMORTAL(ch)
    &&  (stone == NULL || stone->item_type != ITEM_WARP_STONE))
    {
      send_to_char( L"Büyü için gereken bileşene sahip değilsin.\n\r",ch);
        return;
    }

    if (stone != NULL && stone->item_type == ITEM_WARP_STONE)
    {
      act( L"$p içindeki güce yükleniyorsun.",ch,stone,NULL,TO_CHAR);
      act( L"$p alev alıyor ve yokoluyor!",ch,stone,NULL,TO_CHAR);
        extract_obj(stone);
    }

    /* portal one */
    portal = create_object(get_obj_index(OBJ_VNUM_PORTAL),0);
    portal->timer = 1 + level / 10;
    portal->value[3] = to_room->vnum;

    obj_to_room(portal,from_room);

    act( L"$p havaya yükseliyor.",ch,portal,NULL,TO_ROOM);
    act( L"$p önünde havaya yükseliyor.",ch,portal,NULL,TO_CHAR);

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
      act( L"$p havaya yükseliyor.",to_room->people,portal,NULL,TO_ROOM);
    	act( L"$p havaya yükseliyor.",to_room->people,portal,NULL,TO_CHAR);
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


    if (saves_spell(level,victim,DAM_MENTAL) || number_range(0,1) == 0)
	{
	 dam = dice( level , 24 ) ;
	 damage(ch, victim , dam , sn, DAM_MENTAL, TRUE);
	 return;
	}

  act_color( L"$S ince ışık ışını seni $C### PARÇALIYOR ###$c!",
  victim, NULL, ch, TO_CHAR, POS_RESTING, CLR_RED );
  act_color( L"$s ince ışık ışını $M $C### PARÇALIYOR ###$c!",
  ch, NULL, victim, TO_NOTVICT, POS_RESTING, CLR_RED );
  act_color( L"İnce ışık ışının $M $C### PARÇALIYOR ###$c!",
  ch, NULL, victim, TO_CHAR, POS_RESTING, CLR_RED );
  send_to_char( L"Ö L D Ü R Ü L D Ü N!\n\r", victim );

  act( L"$N artık yok!\n\r", ch, NULL, victim, TO_CHAR);
  act( L"$N artık yok!\n\r", ch, NULL, victim, TO_ROOM);

  send_to_char( L"Birkaç dakikalığına yenilmez bir hayalete dönüştün.\n\r",victim);
  send_to_char( L"Tabii birşeylere saldırmadığın sürece.\n\r", victim);

  if (!IS_NPC(ch) && IS_QUESTOR(ch) && IS_NPC(victim))
  {
    if (ch->pcdata->questmob == victim->pIndexData->vnum)
    {
      printf_to_char(ch,L"{cGörevin neredeyse tamamlandı!\n\rZamanın bitmeden önce görevciye git!{x\n\r");
      ch->pcdata->questmob = -1;
    }
  }

    /*  disintegrate the objects... */
    tattoo = get_eq_char(victim, WEAR_TATTOO); /* keep tattoos for later */
    if (tattoo != NULL)
      obj_from_char(tattoo);

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

void spell_arz_yutagi( int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  CHAR_DATA *tmp_ch;
  OBJ_DATA *obj;
  OBJ_DATA *obj_next;
  int i,dam=0;
  OBJ_DATA *tattoo;


    if (saves_spell(level,victim,DAM_MENTAL) || number_range(0,1) == 0)
	{
	 dam = dice( level , 24 ) ;
	 damage(ch, victim , dam , sn, DAM_MENTAL, TRUE);
	 return;
	}

  act_color( L"$S büyüsü seni $C### YERİN İÇİNE ÇEKİYOR ###$c!",
  victim, NULL, ch, TO_CHAR, POS_RESTING, CLR_RED );
  act_color( L"$s büyüsü $M $C### YERİN İÇİNE ÇEKİYOR ###$c!",
  ch, NULL, victim, TO_NOTVICT, POS_RESTING, CLR_RED );
  act_color( L"Büyün $M $C### YERİN İÇİNE ÇEKİYOR ###$c!",
  ch, NULL, victim, TO_CHAR, POS_RESTING, CLR_RED );
  send_to_char( L"Y U T U L D U N!\n\r", victim );

  act( L"$N arz tarafından yutuldu!\n\r", ch, NULL, victim, TO_CHAR);
  act( L"$N arz tarafından yutuldu!\n\r", ch, NULL, victim, TO_ROOM);

  send_to_char( L"Birkaç dakikalığına yenilmez bir hayalete dönüştün.\n\r",victim);
  send_to_char( L"Tabii birşeylere saldırmadığın sürece.\n\r", victim);

  if (!IS_NPC(ch) && IS_QUESTOR(ch) && IS_NPC(victim))
  {
    if (ch->pcdata->questmob == victim->pIndexData->vnum)
    {
      printf_to_char(ch,L"{cGörevin neredeyse tamamlandı!\n\rZamanın bitmeden önce görevciye git!{x\n\r");
      ch->pcdata->questmob = -1;
    }
  }

    /*  disintegrate the objects... */
    tattoo = get_eq_char(victim, WEAR_TATTOO); /* keep tattoos for later */
    if (tattoo != NULL)
      obj_from_char(tattoo);

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
  wchar_t buf[MAX_STRING_LENGTH];

  send_to_char( L"Zehirli duman bulutu odayı dolduruyor.\n\r",ch);
  act( L"Zehirli duman bulutu odayı dolduruyor.",ch,NULL,NULL,TO_ROOM);

  for (tmp_vict=ch->in_room->people;tmp_vict!=NULL;
       tmp_vict=tmp_vict->next_in_room)
    if (!is_safe_spell(ch,tmp_vict,TRUE))
      {
	if (!IS_NPC(ch) && tmp_vict != ch &&
	    ch->fighting != tmp_vict && tmp_vict->fighting != ch &&
	    (IS_SET(tmp_vict->affected_by,AFF_CHARM) || !IS_NPC(tmp_vict)))
	  {
	    if (!can_see(tmp_vict, ch))
		do_yell(tmp_vict, (wchar_t*)"İmdat biri bana saldırıyor!");
	    else
	      {
	         swprintf( buf, MAX_STRING_LENGTH-1, L"Geber %s, seni büyücü köpek!",
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
  wchar_t buf[MAX_STRING_LENGTH];

  send_to_char( L"Bir toz bulutu odayı dolduruyor.\n\r",ch);
  act( L"Bir toz bulutu odayı dolduruyor.",ch,NULL,NULL,TO_ROOM);


  for (tmp_vict=ch->in_room->people;tmp_vict!=NULL;
       tmp_vict=tmp_vict->next_in_room)
    if (!is_safe_spell(ch,tmp_vict,TRUE))
      {
	if (!IS_NPC(ch) && tmp_vict != ch &&
	    ch->fighting != tmp_vict && tmp_vict->fighting != ch &&
	    (IS_SET(tmp_vict->affected_by,AFF_CHARM) || !IS_NPC(tmp_vict)))
	  {
	    if (!can_see(tmp_vict, ch))
		do_yell(tmp_vict, (wchar_t*)"İmdat biri bana saldırıyor!");
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

void spell_bark_skin( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( ch, sn ) )
    {
	if (victim == ch)
  send_to_char( L"Derin zaten kabuklarla kaplanmış.\n\r",ch);
else
  act( L"$N olabildiğince sert zaten.",ch,NULL,victim,TO_CHAR);
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
    act( L"$s derisi ağaç kabuklarıyla kaplanıyor.", victim, NULL, NULL, TO_ROOM );
    send_to_char( L"Derin ağaç kabuklarıyla kaplanıyor.\n\r", victim );
    return;
}

void spell_bear_call( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *gch;
  CHAR_DATA *bear;
  CHAR_DATA *bear2;
  AFFECT_DATA af;
  int i;

  send_to_char( L"Ayıları yardımına çağırıyorsun.\n\r",ch);
  act( L"$n ayıları çağırıyor.",ch,NULL,NULL,TO_ROOM);

  if (is_affected(ch, sn))
    {
      send_to_char( L"Onları kontrol edecek gücü nasıl çağıracaksın?\n\r", ch);
      return;
    }
  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  gch->pIndexData->vnum == MOB_VNUM_BEAR)
	{
    send_to_char( L"Sahip olduğun ayıyla bir sorunun mu var?",ch);
	  return;
	}
    }

  if (count_charmed(ch)) return;

  bear = create_mobile( get_mob_index(MOB_VNUM_BEAR), NULL );

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
  bear->silver = 0;

  bear2 = create_mobile(bear->pIndexData, NULL);
  clone_mobile(bear,bear2);

  SET_BIT(bear->affected_by, AFF_CHARM);
  SET_BIT(bear2->affected_by, AFF_CHARM);
  bear->master = bear2->master = ch;
  bear->leader = bear2->leader = ch;

  char_to_room(bear,ch->in_room);
  char_to_room(bear2,ch->in_room);
  send_to_char( L"İki ayı yardımına geliyor!\n\r",ch);
  act( L"$s yardımına iki ayı geliyor!",ch,NULL,NULL,TO_ROOM);

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
  send_to_char( L"Bir korucu asası yaratıyorsun!\n\r",ch);
  act( L"$n bir korucu asası yaratıyor!",ch,NULL,NULL,TO_ROOM);

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
      send_to_char( L"Başaramadın.\n\r", ch );
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
      send_to_char( L"Başaramadın.\n\r", ch );
      return;
  }


  act( L"$n küçük bir küreyi yere fırlatıyor.", ch, NULL, NULL, TO_ROOM );

  if (!IS_NPC(ch) && ch->fighting != NULL && number_range(0,1) == 1) {
    send_to_char( L"Başaramadın.\n\r",ch);
    return;
  }

  act( L"$n gitti!",victim,NULL,NULL,TO_ROOM);

  char_from_room( victim );
  char_to_room( victim, pRoomIndex );
  act( L"$n ortaya çıkıyor.", victim, NULL, NULL, TO_ROOM );
  do_look( victim, (wchar_t*)"auto" );
  stop_fighting(victim,TRUE);
  return;
}

void spell_transform( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  AFFECT_DATA af;

  if (is_affected(ch,sn) || ch->hit > ch->max_hit)
    {
      send_to_char( L"Zaten heryerinden sağlık fışkırıyor.\n\r",ch);
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


  send_to_char( L"Sağlığın arttıkça aklın karışıyor.\n\r",ch);
}

void spell_mana_transfer( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;

  if (victim == ch)
    {
      send_to_char( L"Kendi kendine mana transferi yaparsan patlarsın.\n\r",ch);
      return;
    }

  if (ch->cabal != victim->cabal)
    {
      send_to_char( L"Bu büyüyü yalnız kendi kabal arkadaşlarında kullanabilirsin.\n\r",ch);
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
      act( L"Zihin bıçağın $S aklını kavuruyor!",ch,NULL,victim,TO_CHAR);
      act( L"$s zihin bıçağı senin aklını kavuruyor!",ch,NULL,victim,TO_VICT);
      act( L"$s zihin bıçağı $S aklını kavuruyor!",ch,NULL,victim,TO_NOTVICT);
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
      send_to_char( L"Başka bir iblis çağırmaya gücün kalmadı.\n\r",ch);

      return;
    }

    send_to_char( L"Bir iblis çağırmayı deniyorsun.\n\r",ch);
    act( L"$n bir iblis çağırmayı deniyor.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  gch->pIndexData->vnum == MOB_VNUM_DEMON)
	{
    send_to_char( L"İki iblisi kontrol edemezsin!\n\r",ch);
	  return;
	}
    }

  if (count_charmed(ch)) return;

  demon = create_mobile( get_mob_index(MOB_VNUM_DEMON) , NULL);

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
  demon->silver = 0;
  demon->timer = 0;
  demon->damage[DICE_NUMBER] = number_range(level/15, level/12);
  demon->damage[DICE_TYPE] = number_range(level/3, level/2);
  demon->damage[DICE_BONUS] = number_range(level/10, level/8);

  char_to_room(demon,ch->in_room);
  send_to_char( L"Yerin çok çok altından bir iblis geliyor!\n\r",ch);
  act( L"Yerin çok çok altından bir iblis geliyor!",ch,NULL,NULL,TO_ROOM);

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
      do_say(demon, (wchar_t*)"Beni rahatsız etmeye nasıl cüret edersin?!!!");
    else
      do_say(demon, (wchar_t*)"Beni rahatsız etmeye cüret eden kim?!!!");
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
  wchar_t buf[MAX_STRING_LENGTH];
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
		do_yell(tmp_vict, (wchar_t*)"İmdat biri bana saldırıyor!");
	      else
		{
		  swprintf( buf, MAX_STRING_LENGTH-1, L"Geber %s, seni büyücü köpek!",
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
      act( L"Zaten onun gibi görünüyorsun.",ch,NULL,victim,TO_CHAR);
      return;
    }

  if (IS_NPC(victim))
    {
      act( L"$N taklit edemeyeceğin kadar farklı.",ch,NULL,victim,TO_CHAR);
     return;
   }

  if (IS_IMMORTAL(victim))
    {
      send_to_char( L"Akıllı ol.\n\r",ch);
      return;
    }

  if (saves_spell(level,victim, DAM_CHARM))
   {
    send_to_char( L"Başaramadın.\n\r",ch);
    return;
   }

   act( L"$E benzemek için formunu değiştiriyorsun.",ch,NULL,victim,TO_CHAR);
   act( L"$n SANA benzemek için formunu değiştiriyor!",ch,NULL,victim,TO_VICT);
   act( L"$n $E benzemek için formunu değiştiriyor!",ch,NULL,victim,TO_NOTVICT);

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
      act( L"Fakat $N aranmıyor.",ch,NULL,victim,TO_CHAR);
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

  act( L"$p yaratıyorsun!",ch,shield,NULL,TO_CHAR);
  act( L"$n $p yaratıyor!",ch,shield,NULL,TO_ROOM);
}

void spell_guard_call( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *gch;
  CHAR_DATA *guard;
  CHAR_DATA *guard2;
  CHAR_DATA *guard3;
  AFFECT_DATA af;
  wchar_t buf[] = L"Muhafızlar!";
  int i;

  if (is_affected(ch,sn))
    {
      send_to_char( L"Üç muhafız daha çağırmaya gücün kalmadı.\n\r",ch);
      return;
    }

  do_yell(ch, buf);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch
	  && gch->pIndexData->vnum == MOB_VNUM_SPECIAL_GUARD)
	{
	  do_say(gch, (wchar_t*)"Nasıl yani? Ben yeterince iyi değil miyim?");
	  return;
	}
    }

  if (count_charmed(ch)) return;

  guard = create_mobile( get_mob_index(MOB_VNUM_SPECIAL_GUARD), NULL );

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
  guard->silver = 0;
  guard->timer = 0;

  guard->damage[DICE_NUMBER] = number_range(level/16, level/12);
  guard->damage[DICE_TYPE] = number_range(level/3, level/2);
  guard->damage[DICE_BONUS] = number_range(level/9, level/6);

  guard->detection = (A|C|D|E|F|G|H|ee);
  SET_BIT(guard->affected_by, AFF_CHARM);
  SET_BIT(guard->affected_by, AFF_SANCTUARY);

  guard2 = create_mobile(guard->pIndexData, NULL);
  clone_mobile(guard,guard2);

  guard3 = create_mobile(guard->pIndexData, NULL);
  clone_mobile(guard,guard3);

  guard->master = guard2->master = guard3->master = ch;
  guard->leader = guard2->leader = guard3->leader = ch;

  char_to_room(guard,ch->in_room);
  char_to_room(guard2,ch->in_room);
  char_to_room(guard3,ch->in_room);
  send_to_char( L"Yardımına üç muhafız geliyor!\n\r",ch);
  act( L"$s yardımına üç muhafız geliyor!",ch,NULL,NULL,TO_ROOM);

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
  wchar_t buf[100];
  int i;

  if (is_affected(ch,sn))
    {
      send_to_char( L"Bir solucan çağıracak gücü bulamıyorsun.\n\r",ch);

      return;
    }

    send_to_char( L"Bir solucan çağırmayı denedin.\n\r",ch);
    act( L"$n bir solucan çağırmayı denedi.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  gch->pIndexData->vnum == MOB_VNUM_NIGHTWALKER)
	{
    send_to_char( L"İki solucanı kontrol edemezsin!\n\r",ch);
	  return;
	}
    }

  if (count_charmed(ch)) return;

  walker = create_mobile( get_mob_index(MOB_VNUM_NIGHTWALKER) , NULL);

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
  walker->silver = 0;
  walker->timer = 0;
  walker->damage[DICE_NUMBER] = number_range(level/15, level/10);
  walker->damage[DICE_TYPE]   = number_range(level/3, level/2);
  walker->damage[DICE_BONUS]  = 0;

  char_to_room(walker,ch->in_room);
  send_to_char( L"Gölgelerin içinden bir solucan yükseliyor!\n\r",ch);
  act( L"Gölgelerin içinden bir solucan yükseliyor!",ch,NULL,NULL,TO_ROOM);
  swprintf( buf, MAX_STRING_LENGTH-1, L"Bir solucan önünde eğiliyor.");
  send_to_char(buf, ch);
  swprintf( buf, MAX_STRING_LENGTH-1, L"Bir solucan %s önünde eğiliyor!", ch->name );
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
        send_to_char( L"Casus ağın öyle birine rastlamıyor.\n\r",ch);
	return;
      }

    if ((victim->level > ch->level + 7)
	|| saves_spell((ch->level + 9), victim, DAM_NONE))
      {
        send_to_char( L"Casus ağın öyle birini bulamıyor.\n\r",ch);
	return;
      }

    if (ch==victim)
      do_look( ch, (wchar_t*)"auto" );
    else {
      ori_room = ch->in_room;
      char_from_room( ch );
      char_to_room( ch, victim->in_room );
      do_look( ch, (wchar_t*)"auto" );
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
        send_to_char( L"Bu büyüyü ancak kabal dostların üzerinde kullanabilirsin.\n\r",ch);
	return;
      }

    if ( is_affected( victim, sn ) )
    {
	if (victim == ch)
  send_to_char( L"Zaten bir gölge peleriniyle korunuyorsun.\n\r",ch);
else
  act( L"$N zaten bir gölge peleriniyle korunuyor.",ch,NULL,victim,TO_CHAR);
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
    send_to_char( L"Gölgelerin seni koruduğunu hissediyorsun.\n\r", victim );
    if ( ch != victim )
	act( L"Bir gölge pelerini $M koruyor.",ch,NULL,victim,TO_CHAR);
    return;
}

void spell_nightfall( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *vch;
  OBJ_DATA  *light;
  AFFECT_DATA af;

  if ( is_affected( ch, sn ) )
  {
    send_to_char( L"Işıkları kontrol edebilecek gücün yok.\n\r", ch );
    return;
  }

  for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
    for (light = vch->carrying; light != NULL; light = light->next_content)
      {
	if (light->item_type == ITEM_LIGHT && light->value[2] != 0
		&& !is_same_group(ch, vch)) {
	  if ( /*light->value[2] != -1 ||*/ saves_spell(level, vch, DAM_ENERGY))
	  {
      act( L"$p titreyerek sönüyor!",ch,light,NULL,TO_CHAR);
	    act( L"$p titreyerek sönüyor!",ch,light,NULL,TO_ROOM);
	    light->value[2] = 0;
	    if (get_light_char(ch) == NULL) ch->in_room->light--;
	  }
/*	  else {
	    act( L"$p momentarily dims.",ch,light,NULL,TO_CHAR);
	    act( L"$p momentarily dims.",ch,light,NULL,TO_ROOM);
	  } */
	}
      }

  for (light = ch->in_room->contents;light != NULL; light=light->next_content)
    if (light->item_type == ITEM_LIGHT && light->value[2] != 0) {
      act( L"$p titreyerek sönüyor!",ch,light,NULL,TO_CHAR);
      act( L"$p titreyerek sönüyor!",ch,light,NULL,TO_ROOM);
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
  wchar_t long_buf[MAX_STRING_LENGTH];
  wchar_t short_buf[20];
  int order;

  if (IS_NPC(victim)) {
    send_to_char( L"Yalnız oyuncular aynalanabilir.\n\r",ch);
    return;
  }

  for (mirrors = 0, gch = char_list; gch != NULL; gch = gch->next)
    if (IS_NPC(gch) && is_affected(gch,gsn_mirror)
	&& is_affected(gch,gsn_doppelganger) && gch->doppel == victim)
      mirrors++;

if (mirrors >= level/5) {
  if (ch==victim)
  send_to_char( L"Daha fazla aynalanamazsın.\n\r",ch);
else
  act( L"$N daha fazla aynalanamaz.",ch,NULL,victim,TO_CHAR);
  return;
}

  af.where     = TO_AFFECTS;
  af.level     = level;
  af.modifier  = 0;
  af.location  = 0;
  af.bitvector = 0;

  for (tmp_vict = victim; is_affected(tmp_vict,gsn_doppelganger);
       tmp_vict = tmp_vict->doppel);

       swprintf(long_buf, MAX_STRING_LENGTH-1, L"%s%s burada.\n\r", tmp_vict->name, tmp_vict->pcdata->title);
  swprintf(short_buf, 20-1, tmp_vict->name);

  order = number_range(0,level/5 - mirrors);

  for (new_mirrors=0; mirrors + new_mirrors < level/5;new_mirrors++)
  {
    gch = create_mobile( get_mob_index(MOB_VNUM_MIRROR_IMAGE), NULL );
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
      send_to_char( L"Sana ait bir yalancı görüntü önünde beliriyor!\n\r",ch);
      act( L"$s önünde kendisine ait bir yalancı görüntü beliriyor!",ch,NULL,victim,TO_ROOM);
    }
    else {
      act( L"$S önünde kendisine ait bir yalancı görüntü beliriyor!",ch,NULL,victim,TO_CHAR);
      act( L"$S önünde kendisine ait bir yalancı görüntü beliriyor!",ch,NULL,victim,TO_NOTVICT);
      send_to_char( L"Sana ait bir yalancı görüntü önünde beliriyor!\n\r",victim);
    }

  }
}

void spell_garble( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  AFFECT_DATA af;

  if (ch == victim)
  {
    send_to_char( L"Kimin konuşmasını sürçeceksin?\n\r",ch);
    return;
  }

  if (is_affected(victim,sn))
  {
    act( L"$S zaten sürç-i lisan ediyor.",ch,NULL,victim,TO_CHAR);
    return;
  }

  if (is_safe_nomessage(ch,victim)) {
    send_to_char( L"Onun konuşmasını sürçemezsin.\n\r",ch);
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

  act( L"$S konuşmasını sürçüyorsun!",ch,NULL,victim,TO_CHAR);
  send_to_char( L"Sürç-i lisan ediyorsun.\n\r",victim);
}

void spell_confuse( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  AFFECT_DATA af;
  CHAR_DATA *rch;
  int count=0;

  if (is_affected(victim,gsn_confuse)) {
    act( L"$N zaten şaşırmış.",ch,NULL,victim,TO_CHAR);
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
      send_to_char( L"Bir uyuşukluk dalgası tarafından ele geçiriliyorsun.\n\r",victim );
    	act( L"$n derin bir uykuya daldı.", victim, NULL, NULL, TO_ROOM );
    	victim->position = POS_SLEEPING;
    }

    return;
}

void spell_kassandra( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{

    AFFECT_DATA af;

    if ( is_affected( ch, sn ) )
      {
	send_to_char(L"Kassandra bu iş için yeni kullanıldı.\n\r",ch);
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
    send_to_char( L"Sıcak bir dalga vücudunu sarıyor.\n\r", ch );
    act( L"$n daha iyi görünüyor.", ch, NULL, NULL, TO_ROOM);
    return;
}


void spell_sebat( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  AFFECT_DATA af;

  if ( is_affected( ch, sn ) )
    {
      send_to_char( L"Kasandra bu iş için yeni kullanıldı.\n\r",ch);

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
  act( L"$n mistik bir kalkanla çevrelendi.",ch, NULL,NULL,TO_ROOM );
  send_to_char( L"Mistik bir kalkanla çevrelendin.\n\r", ch );
  return;
}


void spell_matandra( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  int dam;
  AFFECT_DATA af;

  if ( is_affected( ch, sn ) )
    {
      send_to_char(L"Kasandra bu iş için yeni kullanıldı.\n\r",ch);
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

    act( L"Anılarının akıp gittiğini hissediyorsun.",victim,NULL,NULL,TO_CHAR);
    act( L"$n boş gözlerle etrafa bakıyor.",victim,NULL,NULL,TO_ROOM);
}


void spell_chaos_blade( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  OBJ_DATA *blade;
  AFFECT_DATA af;

  blade = create_object( get_obj_index(OBJ_VNUM_CHAOS_BLADE),level);
  send_to_char( L"Bir yatağan yaratıyorsun!\n\r",ch);
  act( L"$n bir yatağan yaratıyor!",ch,NULL,NULL,TO_ROOM);

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
      act( L"$N sana tapamayacak kadar aptal!", ch, NULL, victim, TO_CHAR);
      return;
    }

  for (i = 0; i < MAX_RELIGION; i++)
    {
      if (!wcscasecmp(ch->name, religion_table[i].name))
	{
  	  tattoo = get_eq_char(victim, WEAR_TATTOO);
   	  if (tattoo != NULL)
    	    {
            act( L"$N zaten dövmelenmiş!  Önce diğerini çıkarmalısın.",
		ch, NULL, victim, TO_CHAR);
      	      act( L"$n sana ikinci bir dövme vermeye çalıştı, ama başaramadı.",
		ch, NULL, victim, TO_VICT);
              act( L"$n $E ikinci bir dövme vermeye çalıştı, ama başaramadı.",
		ch, NULL, victim, TO_NOTVICT);
      	      return;
    	    }
  	  else
    	    {
      	      tattoo = create_object(get_obj_index(religion_table[i].vnum),60);
              act( L"$E $p dövmesi kazıyorsun!",ch, tattoo, victim, TO_CHAR);
      	      act( L"$n $E $p dövmesi kazıyor!",ch,tattoo,victim,TO_NOTVICT);
      	      act( L"$n sana $p dövmesi kazıyor!",ch,tattoo,victim,TO_VICT);

      	      obj_to_char(tattoo,victim);
      	      equip_char(victim, tattoo, WEAR_TATTOO);
	      return;
	    }
	}
    }
    send_to_char( L"Dini bir dövmen yok.\n\r", ch);
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
      act( L"Acılı bir işlemle $n dövmeni kaldırıyor.",
	ch, NULL, victim, TO_VICT);
      act( L"$S dövmesini kaldırıyorsun.", ch, NULL, victim, TO_CHAR);
      act( L"$n $S dövmesini kaldırıyor.", ch, NULL, victim, TO_NOTVICT);
    }
  else
  act( L"$S hiç dövmesi yok.", ch, NULL, victim, TO_CHAR);
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
      act( L"Tanrılar $M koruyor.", ch, NULL, victim, TO_ROOM );
      return;
    }

  if ( IS_NEUTRAL(victim) )
    {
      act( L"$N etkilenmiş görünmüyor.", ch, NULL, victim, TO_CHAR );
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

  send_to_char( L"Kirlenmiş hissediyorsun.\n\r", victim );
  if ( ch != victim )
    act( L"$N rahatsız görünüyor.",ch,NULL,victim,TO_CHAR);
   return;
}

void spell_old_randomizer(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
  ROOM_INDEX_DATA *pRoomIndex;
  EXIT_DATA *pexit;
  int d0;
  int d1;
  AFFECT_DATA af;
  wchar_t log_buf[MAX_INPUT_LENGTH];

  if ( is_affected( ch, sn ) )
    {
      send_to_char(L"Bu iş için tükenmiş durumdası.\n\r",ch);
      return;
    }
  if (IS_SET(ch->in_room->room_flags, ROOM_LAW))
    {
      send_to_char(L"Bu odayı karıştırmak gücünün ötesinde.\n\r", ch);
      return;
    }

  af.where		= TO_AFFECTS;
  af.type      = sn;
  af.level     = UMIN(level + 15, MAX_LEVEL);
  af.location  = 0;
  af.modifier  = 0;
  af.bitvector = 0;

  pRoomIndex = get_room_index(ch->in_room->vnum);

  if (number_range(0,1) == 0)
    {
      send_to_char( L"Tüm çabalarına rağmen evren kaosa direniyor.\n\r",ch);
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
  send_to_char( L"Oda rasgele dağılımla etkilendi!\n\r", ch);
  send_to_char( L"Çabanın ardından tükendiğini hissediyorsun.\n\r", ch);
  ch->hit -= UMIN(200, ch->hit/2);

  swprintf( log_buf, (2*MAX_INPUT_LENGTH)-1, L"%s used randomizer in room %d", ch->name, ch->in_room->vnum);
  log_string(log_buf);

}

void spell_stalker( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *victim;
  CHAR_DATA *stalker;
  AFFECT_DATA af;
  int i;
  wchar_t log_buf[MAX_INPUT_LENGTH];

  if ( ( victim = get_char_world( ch, target_name ) ) == NULL
      ||   victim == ch || victim->in_room == NULL
      || IS_NPC(victim) || !IS_SET(victim->act, PLR_WANTED))
    {
      send_to_char( L"Başaramadın.\n\r", ch);
      return;
    }

  if (is_affected(ch,sn))
    {
      send_to_char( L"Bu güç yakın zamanda kullanıldı.\n\r", ch);
      return;
    }

  if (!is_safe_nomessage(ch,victim) && !IS_SET(ch->act,PLR_CANINDUCT) )
    {
      send_to_char( L"Bu iş için özel muhafızları kullansan daha iyi sonuç verir.\n\r", ch);
      return;
    }

    send_to_char( L"Bir avcı çağırmaya çalışıyorsun.\n\r",ch);
    act( L"$n bir avcı çağırmaya çalışıyor.",ch,NULL,NULL,TO_ROOM);

  stalker = create_mobile( get_mob_index(MOB_VNUM_STALKER), NULL );

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
  stalker->silver = 0;
  stalker->invis_level = LEVEL_IMMORTAL;
  stalker->detection = (A|B|C|D|E|F|G|H|ee);
  stalker->affected_by = (H|J|N|O|U|V|aa|cc);

  char_to_room(stalker,victim->in_room);
  stalker->last_fought = victim;
  send_to_char( L"Görünmez bir avcı seni avlamaya geldi!\n\r",victim);
  act( L"Görünmez bir avcı $m avlamaya geldi!",victim,NULL,NULL,TO_ROOM);
  send_to_char( L"Görünmez bir avcı gönderildi.\n\r", ch);

  swprintf( log_buf, (2*MAX_INPUT_LENGTH)-1, L"%s used stalker on %s", ch->name, victim->name);
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
	send_to_char( L"Başaramadın.\n\r",ch);
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
      ||   (!IS_NPC(victim) && IS_SET(victim->act,PLR_NOSUMMON) && is_safe_nomessage(ch,victim))
      ||   (IS_NPC(victim) && (victim->pIndexData->vnum == ch->pcdata->questmob))
      ||   (saves_spell( level, victim, DAM_NONE ) ) )
    {
      send_to_char( L"Başaramadın.\n\r", ch );
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
    act( L"$n garip sözcükler fısıldarken çevrende aksayan zamanı hissediyorsun.",ch, NULL, wch, TO_VICT);
          if(victim->in_room == NULL) {
		bug( L"Tesseract: victim room has become NULL!!!",0);
                return;
          }
	  char_from_room(wch);
	  char_to_room(wch,victim->in_room);
    act( L"$n aniden geliyor.",wch,NULL,NULL,TO_ROOM);
	  if (wch->in_room == NULL)
	     bug( L"Tesseract: other char sent to NULL room",0);
	  else
	     do_look(wch,(wchar_t*)"auto");
	}
    }

    act( L"Bir ışık patlamasıyla $n ve arkadaşları beliriyor!",ch,NULL,NULL,TO_ROOM);
    send_to_char( L"Tuhaf sözcükleri fısıldadığın anda zaman ve mekan bulanıklaşarak duruluyor.\n\r",ch);
  char_from_room(ch);
  char_to_room(ch,victim->in_room);

  act( L"$n aniden geliyor.",ch,NULL,NULL,TO_ROOM);
  if (ch->in_room == NULL)
     bug( L"Tesseract: char sent to NULL room",0);
  else
     do_look(ch,(wchar_t*)"auto");

  if (gate_pet)
    {
      send_to_char( L"Zaman ve mekanın kaydığını hissediyorsun.\n\r",ch->pet);
      char_from_room(ch->pet);
      char_to_room(ch->pet,victim->in_room);
      act( L"$n aniden geliyor.",ch->pet,NULL,NULL,TO_ROOM);
      if (ch->pet->in_room == NULL)
         bug( L"Tesseract: pet sent to NULL room",0);
      else
         do_look(ch->pet,(wchar_t*)"auto");
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
        send_to_char( L"O bir iksire dönüştürülemez.\n\r",ch);
	return;
      }

    if (obj->wear_loc != -1)
      {
        send_to_char( L"Mayalanacak şey taşınmalı.\n\r",ch);
	return;
      }

    for( vial=ch->carrying; vial != NULL; vial=vial->next_content )
      if ( vial->pIndexData->vnum == OBJ_VNUM_POTION_VIAL )
        break;
    if (  vial == NULL )  {
      send_to_char( L"İksiri içine mayalayabileceğin bir iksir şişen yok.\n\r", ch );
	return;
    }


    if (number_percent() < 50)
      {
        send_to_char( L"Başaramadın ve onu yokettin.\n\r", ch);
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
	  spell = skill_lookup( L"fireball");
	else if (number_percent() < 40)
	  spell = skill_lookup( L"cure poison");
	else if (number_percent() < 60)
	  spell = skill_lookup( L"cure blind");
	else if (number_percent() < 80)
	  spell = skill_lookup( L"cure disease");
	else
	  spell = skill_lookup( L"word of recall");
      }
    else if (obj->item_type == ITEM_TREASURE)
      {
	switch(number_range(0,7)) {
	case 0:
	  spell = skill_lookup( L"cure critical");
	  break;
	case 1:
	  spell = skill_lookup( L"haste");
	  break;
	case 2:
	  spell = skill_lookup( L"frenzy");
	  break;
	case 3:
	  spell = skill_lookup( L"create spring");
	  break;
	case 4:
	  spell = skill_lookup( L"holy word");
	  break;
	case 5:
	  spell = skill_lookup( L"invis");
	  break;
	case 6:
	  spell = skill_lookup( L"cure light");
	  break;
	case 7:
	  spell = skill_lookup( L"cure serious");
	  break;

	}
      }
    else
      {
	if (number_percent() < 20)
	  spell = skill_lookup( L"detect magic");
	else if (number_percent() < 40)
	  spell = skill_lookup( L"detect invis");
	else if (number_percent() < 65)
	  spell = skill_lookup( L"pass door");
	else
	  spell = skill_lookup( L"acute vision");
      }

    potion->value[1] = spell;
    extract_obj(obj);
    act( L"Elindekilerle $p mayalıyorsun!", ch, potion, NULL, TO_CHAR);
    act( L"$n elindekilerle $p mayalıyor!", ch, potion, NULL, TO_ROOM);

    obj_to_char(potion, ch);
    extract_obj( vial );

}


void spell_shadowlife( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  CHAR_DATA *shadow;
  AFFECT_DATA af;
  int i;
  wchar_t buf[MAX_STRING_LENGTH];
  wchar_t *name;

  if (IS_NPC(victim))
    {
      send_to_char( L"Bunu neden yapmak istiyorsun?!?\n\r", ch);
      return;
    }

  if (is_affected(ch,sn))
    {
      send_to_char( L"Gölgeye can verecek gücün yok.\n\r",ch);
      return;
    }

    act( L"$S gölgesine yaşam veriyorsun!",ch, NULL, victim, TO_CHAR);
    act( L"$n $S gölgesine yaşam veriyor!",ch,NULL,victim,TO_NOTVICT);
    act( L"$n gölgene yaşam veriyor!", ch, NULL, victim, TO_VICT);

  shadow = create_mobile( get_mob_index(MOB_VNUM_SHADOW), NULL );

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
  shadow->silver = 0;

  name		= IS_NPC(victim) ? victim->short_descr : victim->name;
  swprintf( buf, MAX_STRING_LENGTH-1, shadow->short_descr, name );
  free_string( shadow->short_descr );
  shadow->short_descr = str_dup( buf );

  swprintf( buf, MAX_STRING_LENGTH-1, shadow->long_descr, name );
  free_string( shadow->long_descr );
  shadow->long_descr = str_dup( buf );

  swprintf( buf, MAX_STRING_LENGTH-1, shadow->description, name );
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
    send_to_char( L"Fakat sen başka birşey giyiyorsun.\n\r", ch );
    return;
  }

  for ( badge = ch->carrying; badge != NULL;
       badge = obj_next )
    {
      obj_next = badge->next_content;
      if (badge->pIndexData->vnum == OBJ_VNUM_DEPUTY_BADGE
	  || badge->pIndexData->vnum == OBJ_VNUM_RULER_BADGE)
	{
    act( L"Sendeki $p yokoluyor.",ch, badge, NULL, TO_CHAR);
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
  act( L"Hüküm nişanını giyiyorsun!",ch, NULL, NULL, TO_CHAR);
  act( L"$n hüküm nişanını giyiyor!", ch, NULL, NULL, TO_ROOM);

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
    act( L"$p yokoluyor.",ch, badge, NULL, TO_CHAR);
	  act( L"$s $p yokoluyor.", ch, badge, NULL, TO_ROOM);

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
      send_to_char( L"Zaten ejderhanın gücüyle dolusun.\n\r",ch);
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

  send_to_char( L"Ejderhanın gücü ruhuna katılıyor.\n\r", ch);
  act( L"$n biraz daha acımasız görünüyor.", ch, NULL, NULL, TO_ROOM);
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

      if ( is_affected( vch, sn ) || IS_AFFECTED(vch, AFF_PROTECT_EVIL))
	{
	  if (vch == ch)
    send_to_char( L"Zaten altın bir aurayla korunuyorsun.\n\r",ch);
  else
    act( L"$N zaten altın bir aurayla korunuyor.",ch,NULL,vch,TO_CHAR);
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

      send_to_char( L"Etrafını saran altın aylayı hissediyorsun.\n\r", vch );
      if ( ch != vch )
	act( L"Altın bir ayla $M çevreliyor.",ch,NULL,vch,TO_CHAR);

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

  act( L"$p yarattın!",ch,plate,NULL,TO_CHAR);
  act( L"$n $p yarattı!",ch,plate,NULL,TO_ROOM);
}

void spell_squire( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *gch;
  CHAR_DATA *squire;
  AFFECT_DATA af;
  int i;
  wchar_t buf[MAX_STRING_LENGTH];

  if (is_affected(ch,sn))
    {
      send_to_char( L"Şu an başka bir ağayı yönetemezsin.\n\r",ch);
      return;
    }

    send_to_char( L"Bir ağa çağırmayı denedin.\n\r",ch);
    act( L"$n bir ağa çağırmayı denedi.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  gch->pIndexData->vnum == MOB_VNUM_SQUIRE)
	{
    send_to_char( L"İki ağa ihtiyaç duyduğundan daha fazla değil mi?!\n\r",ch);
	  return;
	}
    }

  if (count_charmed(ch)) return;

  squire = create_mobile( get_mob_index(MOB_VNUM_SQUIRE), NULL );

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
  squire->silver = 0;

  swprintf( buf, MAX_STRING_LENGTH-1, squire->short_descr, ch->name );
  free_string( squire->short_descr );
  squire->short_descr = str_dup( buf );

  swprintf( buf, MAX_STRING_LENGTH-1, squire->long_descr, ch->name );
  free_string( squire->long_descr );
  squire->long_descr = str_dup( buf );

  swprintf( buf, MAX_STRING_LENGTH-1, squire->description, ch->name );
  free_string( squire->description );
  squire->description = str_dup( buf );

  squire->damage[DICE_NUMBER] = number_range(level/15, level/12);
  squire->damage[DICE_TYPE] = number_range(level/3, level/2);
  squire->damage[DICE_BONUS] = number_range(level/8, level/6);

  char_to_room(squire,ch->in_room);
  send_to_char( L"Bir ağa hiçlikten geliyor!\n\r",ch);
  act( L"Bir ağa hiçlikten geliyor!",ch,NULL,NULL,TO_ROOM);

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
  wchar_t arg[MAX_INPUT_LENGTH];
  AFFECT_DATA af;

  target_name = one_argument(target_name, arg);
  sword_vnum = 0;

  if (!wcscasecmp(arg, L"kılıç"))
    sword_vnum = OBJ_VNUM_DRAGONSWORD;
  else if (!wcscasecmp(arg, L"topuz"))
    sword_vnum = OBJ_VNUM_DRAGONMACE;
  else if (!wcscasecmp(arg, L"hançer"))
    sword_vnum = OBJ_VNUM_DRAGONDAGGER;
  else if (!wcscasecmp(arg, L"kargı"))
    sword_vnum = OBJ_VNUM_DRAGONLANCE;
  else
    {
      send_to_char( L"Ne tür bir silah yapmak istiyorsun? Kılıç, topuz, kama, kargı?", ch);
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

  act( L"$p yarattın!",ch,sword,NULL,TO_CHAR);
  act( L"$n $p yarattı!",ch,sword,NULL,TO_ROOM);
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
      send_to_char( L"Burada bitki büyüyemez.\n\r", ch);
      return;
    }

  dam = number_range(level, 4 * level);
  if ( saves_spell( level, victim, DAM_PIERCE ) )
    dam /= 2;

  damage(ch,victim,ch->level,gsn_entangle,DAM_PIERCE, TRUE);

  act( L"Yerden fırlayan sarmaşıklar $s bacaklarına dolanıyor!", victim,
      NULL, NULL, TO_ROOM);
  act( L"Yerden fırlayan sarmaşıklar ayaklarına dolanıyor!", victim,
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
      send_to_char( L"Zaten zarara karşı korunuyorsun.",ch);
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
  act( L"$n zarardan korunmaya başladı.",ch,NULL,NULL,TO_ROOM );
  send_to_char( L"Zarara karşı korunmaya başladın.\n\r", ch );

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

  send_to_char( L"Çevredekileri izlemeyi seviyorsun.\n\r", ch);
}

void spell_protective_shield( int sn, int level, CHAR_DATA *ch, void *vo, int target ) {
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  AFFECT_DATA af;

  if ( is_affected( victim, sn ) )
    {
      if (victim == ch)
      send_to_char( L"Zaten koruyucu bir kalkanla çevrilisin.\n\r",ch);
    else
      act( L"$N zaten koruyucu bir kalkanla çevrili.",ch,NULL,
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
  act( L"$n koruyucu bir kalkanla çevrelendi.",victim,NULL,NULL,TO_ROOM );
  send_to_char( L"Koruyucu bir kalkanla çevrelendin.\n\r", victim );
  return;
}

void spell_deafen( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  AFFECT_DATA af;

  if (ch == victim) {
    send_to_char( L"Kimi sağır edeceksin?\n\r",ch);
    return;
  }

  if (is_affected(victim,sn)) {
    act( L"$N zaten sağır.",ch,NULL,victim,TO_CHAR);
    return;
  }

  if (is_safe_nomessage(ch,victim)) {
    send_to_char( L"Onu sağır edemezsin.\n\r",ch);
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

  act( L"$M sağır ettin!",ch,NULL,victim,TO_CHAR);
  send_to_char( L"Gürültülü bir çınlama kulaklarını dolduruyor...artık duyamıyorsun!\n\r",victim);
}

void spell_disperse( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *vch;
  CHAR_DATA *vch_next;
  ROOM_INDEX_DATA *pRoomIndex;
  AFFECT_DATA af;

  if ( is_affected( ch, sn ) )
    {
      send_to_char( L"Yeterli dağıtma gücün yok.\n\r",ch);
      return;
    }

  for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
      vch_next = vch->next_in_room;

      if ( vch->in_room != NULL
      &&   !IS_SET(vch->in_room->room_flags, ROOM_NO_RECALL)
      &&   !IS_IMMORTAL(vch)
      && (( IS_NPC(vch) && !IS_SET(vch->act, ACT_AGGRESSIVE) ) ||
/*      (!IS_NPC(vch) && vch->level >= KIDEMLI_OYUNCU_SEVIYESI && (vch->level < level || */
      (!IS_NPC(vch) && vch->level >= KIDEMLI_OYUNCU_SEVIYESI && (
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

      send_to_char( L"Dünya çevrende dönüyor!\n\r",vch);
  	  act( L"$n yokoluyor!", vch, NULL, NULL, TO_ROOM );
  	  char_from_room( vch );
  	  char_to_room( vch, pRoomIndex );
  	  act( L"$n yavaşça beliriyor.", vch, NULL, NULL, TO_ROOM );
	  do_look( vch, (wchar_t*)"auto" );
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
      send_to_char( L"Onurun zaten seni koruyor.\n\r", ch);
    else
send_to_char( L"Onlar zaten onurlarıyla korunuyorlar.\n\r", ch);
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

  spell_remove_curse(skill_lookup( L"remove curse"), level, ch, victim, TARGET_CHAR);
  spell_bless(skill_lookup( L"bless"), level, ch, victim, TARGET_CHAR);

  send_to_char( L"Onurun seni koruyor.\n\r", victim);
  act( L"$s onuru kendisini koruyor.", victim, NULL, NULL, TO_ROOM);
}

void spell_acute_vision( int sn, int level, CHAR_DATA *ch, void *vo, int target )
 {
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( CAN_DETECT(victim, ACUTE_VISION) )
    {
        if (victim == ch)
        send_to_char( L"Görüşün yeterince güçlü. \n\r",ch);
      else
        act( L"$S görüşü zaten güçlü.",ch,NULL,victim,TO_CHAR);
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
    send_to_char( L"Görüşün keskinleşiyor.\n\r", victim );
    if ( ch != victim )
        send_to_char( L"Tamam.\n\r", ch );
    return;
}

void spell_dragons_breath(int sn,int level,CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    CHAR_DATA *vch, *vch_next;
    int dam,hp_dam,dice_dam;
    int hpch;
    wchar_t buf[MAX_STRING_LENGTH];

    act( L"Ejderha lordunu yardıma çağırıyorsun.",ch,NULL,NULL,TO_CHAR);
    act( L"$n bir ejderha gibi solumaya başlıyor.",ch,NULL,victim,TO_NOTVICT);
    act( L"$s solumasından rahatsız oluyorsun!",ch,NULL,victim,TO_VICT);
    act( L"Ejderha lordunun nefesini çekiyorsun.",ch,NULL,NULL,TO_CHAR);

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
                do_yell(vch, (wchar_t*)"İmdat biri bana saldırıyor!");
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
    send_to_char( L"Hiç kum bulamıyorsun.\n\r",ch);
	 ch->wait = 0;
	 return;
	}

  act( L"$n yerdeki kumla fırtına yaratıyor.",ch,NULL,NULL,TO_ROOM);
  act( L"Şey yaratıyorsun...kum fırtınası.",ch,NULL,NULL,TO_CHAR);

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

    act( L"$n rahatsız edici bir sesle ÇIĞLIK atıyor!.",ch,NULL,NULL,TO_ROOM);
    act( L"Güçlü bir sesle çığlık atıyorsun.",ch,NULL,NULL,TO_CHAR);

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
      send_to_char( L"Belki de şansını karşı cinste denemelisin!\n\r", ch );
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
      send_to_char( L"Daha fazla vampirleşemezsin!\n\r",ch);
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

    send_to_char( L"Gitgide irileştiğini hissediyorsun.\n\r", ch );
    act( L"$m tanıyamıyorsun.",ch,NULL,NULL,TO_ROOM);
   return;
}




void spell_animate_dead(int sn,int level, CHAR_DATA *ch, void *vo,int target )
 {
    CHAR_DATA *victim;
    CHAR_DATA *undead;
    OBJ_DATA *obj,*obj2,*next;
    AFFECT_DATA af;
    wchar_t buf[MAX_STRING_LENGTH];
    wchar_t buf2[MAX_STRING_LENGTH];
    wchar_t buf3[MAX_STRING_LENGTH];
    wchar_t *argument,*arg;
    int i;

    /* deal with the object case first */
    if (target == TARGET_OBJ)
    {
	obj = (OBJ_DATA *) vo;

      if (!(obj->item_type == ITEM_CORPSE_NPC ||
          obj->item_type == ITEM_CORPSE_PC))
    {
      send_to_char( L"Yalnız cesetleri canlandırabilirsin!\n\r",ch);
	return;
    }
/*
     if (obj->item_type == ITEM_CORPSE_PC)
	{
	send_to_char( L"The magic fails abruptly!\n\r",ch);
	return;
	}
*/
  if (is_affected(ch, sn))
    {
      send_to_char( L"Daha fazla hortlak bedenini kontrol edecek gücü bulamazsın.\n\r", ch);
      return;
    }

  if (count_charmed(ch)) return;

  if ( ch->in_room != NULL && IS_SET(ch->in_room->room_flags, ROOM_NO_MOB) )
  {
    send_to_char( L"Ölüleri burada canlandıramazsın.\n\r", ch );
     return;
  }

  if ( IS_SET(ch->in_room->room_flags, ROOM_SAFE)      ||
       IS_SET(ch->in_room->room_flags, ROOM_PRIVATE)   ||
       IS_SET(ch->in_room->room_flags, ROOM_SOLITARY)  )
  {
    send_to_char( L"Burada canlandıramazsın.\n\r", ch );
    return;
  }

  undead = create_mobile( get_mob_index(MOB_VNUM_UNDEAD), NULL );
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
  undead->silver = 0;

  SET_BIT(undead->act, ACT_UNDEAD);
  SET_BIT(undead->affected_by, AFF_CHARM);
  undead->master = ch;
  undead->leader = ch;

  swprintf( buf, MAX_STRING_LENGTH-1, L"%s beden hortlak", obj->name);
  undead->name = str_dup(buf);
  swprintf(buf2, MAX_STRING_LENGTH-1, L"%s",obj->short_descr);
  argument = (wchar_t*)alloc_perm ( MAX_STRING_LENGTH );
  arg = (wchar_t*)alloc_perm ( MAX_STRING_LENGTH );
  argument = buf2;
  buf3[0] = '\0';
  while (argument[0] != '\0' )
	{
  argument = one_argument(argument, arg);
  if (!(!wcscasecmp(arg, L"hortlak") || !wcscasecmp(arg, L"beden") ||
	!wcscasecmp(arg, L"ceset") ))
	 {
	  if (buf3[0] == '\0')   wcscat(buf3,arg);
	  else  {
		 wcscat(buf3,L" ");
		 wcscat(buf3,arg);
		}
	 }
	}
  swprintf( buf, MAX_STRING_LENGTH-1, L"%s'in hortlak bedeni", buf3);
  undead->short_descr = str_dup(buf);
  swprintf( buf, MAX_STRING_LENGTH-1, L"%s'in hortlak bedeni yürürken sendeliyor.\n\r", buf3);
  undead->long_descr = str_dup(buf);

  for(obj2 = obj->contains;obj2;obj2=next)
    {
	next = obj2->next_content;
	obj_from_obj(obj2);
	obj_to_char(obj2, undead);
    }
    interpret( undead,(wchar_t*)"giy tümü", TRUE);

  af.where	 = TO_AFFECTS;
  af.type      = sn;
  af.level	 = ch->level;
  af.duration  = (ch->level / 10);
  af.modifier  = 0;
  af.bitvector = 0;
  af.location  = APPLY_NONE;
  affect_to_char( ch, &af );

  send_to_char( L"Mistik bir güçle onu canlandırıyorsun!\n\r",ch);
  swprintf( buf, MAX_STRING_LENGTH-1, L"Mistik bir güçle %s %s'i canlandırıyor!",ch->name,obj->name);
  act(buf,ch,NULL,NULL,TO_ROOM);
  swprintf( buf, MAX_STRING_LENGTH-1, L"%s sana bakarken onu rahatsız etmenin bedelini ödetmeyi planlıyor!",obj->short_descr);
  act(buf,ch,NULL,NULL,TO_CHAR);
  extract_obj (obj);
	return;
    }

    victim = (CHAR_DATA *) vo;

    if ( victim == ch )
    {
      send_to_char( L"Fakat sen ölü değilsin!!\n\r", ch );
    	return;
        }

        send_to_char( L"Fakat o ölü değil!!\n\r", ch );
    return;
}



void spell_enhanced_armor( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
    {
	if (victim == ch)
  send_to_char( L"Zaten gelişmiş zırhla kaplısın.\n\r",ch);
else
  act( L"$N zaten gelişmiş zırhla kaplı.",ch,NULL,victim,TO_CHAR);
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
    send_to_char( L"Her tür saldırıya karşı daha iyi korunuyorsun.\n\r", victim );
    if ( ch != victim )
	act( L"$N senin büyünle korunuyor.",ch,NULL,victim,TO_CHAR);
    return;
}



void spell_meld_into_stone( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  AFFECT_DATA af;

  if ( is_affected( victim, sn ) )
    {
      if (victim == ch)
      send_to_char( L"Zaten olabildiğince taşa dönüşmüşsün.\n\r",ch);
    else
      act( L"$S zaten olabildiğince taşa dönüşmüş.",ch,NULL,
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
  act( L"$s taşa dönüşüyor.",victim,NULL,NULL,TO_ROOM );
  send_to_char( L"Taşa dönüşüyorsun.\n\r", victim );
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
  send_to_char( L"Zaten ağlısın.\n\r",ch);
else
  act( L"$N zaten ağlı.",ch,NULL,victim,TO_CHAR);
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
    send_to_char( L"Kalın ağlarla tutulmuşsun!\n\r", victim );
    if ( ch != victim )
	act( L"$M kalın ağlarla tutuyorsun!",ch,NULL,victim,TO_CHAR);
    return;
}


void spell_group_defense( int sn, int level, CHAR_DATA *ch, void *vo ,int target)
{
	CHAR_DATA *gch;
	AFFECT_DATA af;
	int shield_sn, armor_sn;

	shield_sn = skill_lookup( L"shield");
	armor_sn = skill_lookup( L"armor");

	for( gch=ch->in_room->people; gch != NULL; gch=gch->next_in_room)
	{
		if( !is_same_group( gch, ch))
			continue;
		if( is_affected( gch, armor_sn ) )
		{
	    	    if( gch == ch)
            send_to_char( L"Zaten zırhlısın.\n\r",ch);
      		    else
      			act( L"$N zaten zırhlı.", ch, NULL, gch, TO_CHAR);
		  continue;
		}

		af.type      = armor_sn;
		af.level     = level;
		af.duration  = level;
		af.location  = APPLY_AC;
		af.modifier  = -20;
		af.bitvector = 0;
		affect_to_char( gch, &af );

    send_to_char( L"Birinin seni koruduğunu hissediyorsun.\n\r",gch);
		if( ch != gch )
    act( L"$N senin büyünle korunuyor.",
				ch, NULL, gch, TO_CHAR );

		if( !is_same_group( gch, ch ) )
			continue;
		if( is_affected( gch, shield_sn ) )
		{
		  if( gch == ch)
      send_to_char( L"Zaten kalkanlısın.\n\r",ch);
		  else
		        act( L"$N zaten kalkanlı.", ch, NULL, gch, TO_CHAR);
		  continue;
		}

		af.type      = shield_sn;
		af.level     = level;
		af.duration  = level;
		af.location  = APPLY_AC;
		af.modifier   = -20;
		af.bitvector = 0;
		affect_to_char( gch, &af );

    send_to_char( L"Bir güç kalkanıyla çevrelendin.\n\r",gch);
		if( ch != gch )
			act( L"$N bir güç kalkanıyla çevrelendi.",
				ch, NULL, gch, TO_CHAR );

	}
	return;
}


void spell_inspire( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
	CHAR_DATA *gch;
	AFFECT_DATA af;
	int bless_sn;

	bless_sn = skill_lookup( L"bless");

	for( gch=ch->in_room->people; gch != NULL; gch=gch->next_in_room )
	{
		if( !is_same_group( gch, ch) )
			continue;
		if ( is_affected(gch, bless_sn ) )
		{
		  if(gch == ch)
      send_to_char( L"Zaten ilhamlısın.\n\r",ch);
   else
      act( L"$N zaten ilhamlı.",ch, NULL, gch, TO_CHAR);
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

    send_to_char( L"Ruhuna işleyen ilhamı hissediyorsun!\n\r", gch );
		if( ch != gch )
			act( L"$M yaratıcının gücüyle uyandırıyorsun!",
				ch, NULL, gch, TO_CHAR );

	}
	return;
}


void spell_mass_sanctuary( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
	CHAR_DATA *gch;
	AFFECT_DATA af;
	int sanc_sn;

	sanc_sn = skill_lookup( L"sanctuary");

	for( gch=ch->in_room->people; gch != NULL; gch=gch->next_in_room)
	{
		if( !is_same_group( gch, ch ) )
			continue;
		if( IS_AFFECTED(gch, AFF_SANCTUARY ) )
		{
		  if(gch == ch)
      send_to_char( L"Zaten takdislisin.\n\r", ch);
		  else
		    act( L"$N zaten takdisli.", ch, NULL, gch, TO_CHAR);
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

    send_to_char( L"Beyaz bir aurayla çevrelendin.\n\r",gch );
		if( ch != gch )
			act( L"$N beyaz bir aurayla çevrelendi.",
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
      send_to_char( L"Onu onarmaya gerek yok.\n\r",ch);
    return;
    }

    if (obj->wear_loc != -1)
    {
      send_to_char( L"Onarılacak şey taşınıyor olmalı.\n\r",ch);
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
      act( L"$p parlamaya başlıyor.  İyi işti!",ch,obj,NULL,TO_CHAR);
    	act( L"$p parlamaya başlıyor.",ch,obj,NULL,TO_ROOM);
	obj->condition += result;
	obj->condition = UMIN( obj->condition , 100 );
	return;
    }

    else if ( result >=10)
    {
      send_to_char( L"Değişen bir şey olmadı.\n\r",ch);
	return;
    }

    else
    {
      act( L"$p alev alarak buharlaşıyor!",ch,obj,NULL,TO_CHAR);
    	act( L"$p alev alarak buharlaşıyor!",ch,obj,NULL,TO_ROOM);
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
    act( L"$N bir an için titriyor.",
  		ch, NULL, victim, TO_CHAR );
  	send_to_char( L"Bir an için titriyorsun.\n\r",victim);
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
    act( L"$M ruh akıntılarıyla örtüyorsun.",
		ch, NULL, victim, TO_CHAR );
	  send_to_char( L"Kalkanlamanın arttığını hissediyorsun.\n\r",victim);
	  return;
	}

    af.type	= sn;
    af.level    = level;
    af.duration = level / 15;
    af.location = APPLY_NONE;
    af.modifier	= 0;
    af.bitvector = 0;
    affect_join( victim, &af );

    send_to_char( L"Birşeylerle bağlantını yitirdiğini hissediyorsun.\n\r",victim );
    act( L"$M Gerçek Kaynak'tan ayırıyorsun.", ch, NULL, victim, TO_CHAR);
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


  act_color( L"Parmağından çıkan karanlık $M sarıyor.",
  ch, NULL, victim, TO_CHAR, POS_RESTING, CLR_RED );
  act_color( L"$s parmağından çıkan karanlık $M sarıyor.",
  ch, NULL, victim, TO_NOTVICT, POS_RESTING, CLR_RED );
  act_color( L"$S parmağından çıkan karanlık seni sarıyor.",
		victim, NULL, ch, TO_CHAR, POS_RESTING, CLR_RED );

    if (saves_spell(level,victim,DAM_MENTAL) || number_range(0,1) == 0)
	{
	 dam = dice( level , 24 ) ;
	 damage(ch, victim , dam , sn, DAM_MENTAL, TRUE);
	 return;
	}

  send_to_char( L"Ö L D Ü R Ü L D Ü N!\n\r", victim );

  act( L"$N öldürüldü!\n\r", ch, NULL, victim, TO_CHAR);
  act( L"$N öldürüldü!\n\r", ch, NULL, victim, TO_ROOM);

    raw_kill(victim);
    return;
}

void spell_eyed_sword ( int sn, int level, CHAR_DATA *ch, void *vo , int target)
{
    wchar_t buf[MAX_STRING_LENGTH];
    OBJ_DATA *eyed;
    int i;
/*
    if (IS_SET(ch->quest,QUEST_EYE)
	{
	 send_to_char( L"You created your sword ,before.\n\r",ch);
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

	swprintf( buf, MAX_STRING_LENGTH-1, eyed->short_descr, ch->name );
	free_string( eyed->short_descr );
	eyed->short_descr = str_dup( buf );

	swprintf( buf, MAX_STRING_LENGTH-1, eyed->description, ch->name	 );
	free_string( eyed->description );
	eyed->description = str_dup( buf );

        swprintf( buf, MAX_STRING_LENGTH-1, eyed->pIndexData->extra_descr->description, ch->name );
        eyed->extra_descr = new_extra_descr();
        eyed->extra_descr->keyword =
		str_dup(eyed->pIndexData->extra_descr->keyword);
        eyed->extra_descr->description = str_dup( buf );
	eyed->extra_descr->next = NULL;

  	eyed->value[2] = (ch->level / 10) + 3;
  	eyed->level = ch->level;
	eyed->cost = 0;
	obj_to_char( eyed, ch);
  send_to_char( L"Kendi isminle anılacak KILICINI yarattın.\n\r",ch);
	send_to_char( L"Bu silahı bir tane daha yaratamayacağını bil.\n\r",ch);
 return;
}

void spell_lion_help ( int sn, int level, CHAR_DATA *ch, void *vo , int target)
{
  CHAR_DATA *lion;
  CHAR_DATA *victim;
  AFFECT_DATA af;
  wchar_t arg[MAX_INPUT_LENGTH];
  int i;

  target_name = one_argument(target_name, arg);
  if (arg[0] == '\0')
	{
    send_to_char( L"Kimin öldürülmesini istiyorsun?\n\r",ch);
	 return;
	}

  if ( (victim = get_char_area(ch,arg)) == NULL)
	{
    send_to_char( L"Etrafta bu isimde biri yok.\n\r",ch);
	 return;
	}
  if (is_safe_nomessage(ch,victim))
	{
    send_to_char( L"Tanrılar kurbanını koruyor.\n\r",ch);
	 return;
	}

  send_to_char( L"Bir avcı aslan çağırıyorsun.\n\r",ch);
  act( L"$n bir avcı aslan çağırıyor.",ch,NULL,NULL,TO_ROOM);

  if (is_affected(ch,sn))
    {
      send_to_char( L"Daha fazla aslanı kontrol edecek gücün yok.\n\r", ch);
      return;
    }

  if ( ch->in_room != NULL && IS_SET(ch->in_room->room_flags, ROOM_NO_MOB) )
  {
    send_to_char( L"Hiçbir aslan seni dinlemiyor.\n\r", ch );
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
    send_to_char( L"Hiçbir avcı aslan sana gelemez.\n\r", ch );
    return;
  }

  lion = create_mobile( get_mob_index(MOB_VNUM_HUNTER), NULL );

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
  lion->silver = 0;
  lion->damage[DICE_NUMBER] = number_range(level/15, level/10);
  lion->damage[DICE_TYPE] = number_range(level/3, level/2);
  lion->damage[DICE_BONUS] = number_range(level/8, level/6);
  lion->detection = (A|B|C|D|E|F|G|H|ee);

/*   SET_BIT(lion->affected_by, AFF_CHARM);
  lion->master = lion->leader = ch; */

  char_to_room(lion,ch->in_room);

  send_to_char( L"Bir avcı aslan kurbanını öldürmeye geldi!\n\r",ch);
  act( L"Bir avcı aslan $s kurbanını öldürmeye geldi!",ch,NULL,NULL,TO_ROOM);

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
    wchar_t buf[MAX_STRING_LENGTH];
    int i;

    if (victim == ch)
	{
    send_to_char( L"Kendini daha fazla seviyorsun.\n\r",ch);
	return;
	}

    if (IS_NPC(victim))
	{
	send_to_char( L"Kurbanın bir oyuncu! Buna gerek yok.\n\r",ch);
	return;
	}

    if (saves_spell(level ,victim,DAM_MENTAL))
       {
        send_to_char( L"Başaramadın.\n\r",ch);
        return;
       }

    for( vial=ch->carrying; vial != NULL; vial=vial->next_content )
      if ( vial->pIndexData->vnum == OBJ_VNUM_POTION_VIAL )
        break;

    if (  vial == NULL )  {
      send_to_char( L"İçine kurbanının ruhunu koyabileceğin bir şişen yok.\n\r", ch );

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

	swprintf( buf, MAX_STRING_LENGTH-1, fire->name, victim->name);
	free_string( fire->name );
	fire->name = str_dup( buf );

	swprintf( buf, MAX_STRING_LENGTH-1, fire->short_descr, victim->name);
	free_string( fire->short_descr );
	fire->short_descr = str_dup( buf );

	swprintf( buf, MAX_STRING_LENGTH-1, fire->description, victim->name );
	free_string( fire->description );
	fire->description = str_dup( buf );

        swprintf( buf, MAX_STRING_LENGTH-1,fire->pIndexData->extra_descr->description, victim->name );
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
 swprintf( buf, MAX_STRING_LENGTH-1, L"%s'in ruhunu yakalayıp şişenin içine koyuyorsun.\n\r",victim->name);
 send_to_char( buf , ch);
 return;
}

void turn_spell ( int sn, int level, CHAR_DATA *ch, void *vo , int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam, align;

    if (victim != ch)
    {
      act( L"$n raises $s hand, and a blinding ray of light shoots forth!",
    	    ch,NULL,NULL,TO_ROOM);
    	send_to_char(L"You raise your hand and a blinding ray of light shoots forth!\n\r",ch);
    }

    if (IS_GOOD(victim) || IS_NEUTRAL(victim))
    {
      act( L"$n seems unharmed by the light.",victim,NULL,victim,TO_ROOM);
    	send_to_char( L"The light seems powerless to affect you.\n\r",victim);
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
  act( L"$n kaçtı!", victim, NULL, NULL, TO_ROOM );
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
      send_to_char( L"Bu güç yakın zamanda kullanıldı.",ch);
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
      send_to_char( L"Enerji içinde patlıyor!\n\r",ch);
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
    send_to_char( L"Kurbanın bu gücün ötesinde.\n\r", ch );
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
    send_to_char( L"Bir tavşan kadar korkuyorsun.\n\r", victim );
    act( L"$n korkan gözlerle bakıyor.",victim,NULL,NULL,TO_ROOM);
    return;
}

void spell_protection_heat ( int sn, int level, CHAR_DATA *ch, void *vo , int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected(victim, gsn_protection_heat) )
    {
	if (victim == ch)
  send_to_char( L"Zaten sıcaktan korunuyorsun.\n\r",ch);
else
  act( L"$N zaten sıcaktan korunuyor.",ch,NULL,victim,TO_CHAR);
	return;
    }

    if ( is_affected(victim, gsn_protection_cold) )
    {
	if (victim == ch)
  send_to_char( L"Zaten soğuktan korunuyorsun.\n\r",ch);
else
  act( L"$N zaten soğuktan korunuyor.",ch,NULL,victim,TO_CHAR);
	return;
    }

    if ( is_affected(victim, gsn_fire_shield) )
    {
	if (victim == ch)
  send_to_char( L"Zaten ateş kalkanı kullanıyorsun.\n\r",ch);
else
  act( L"$N zaten ateş kalkanı kullanıyor.",ch,NULL,victim,TO_CHAR);
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
    send_to_char( L"Sıcağa karşı güçlendiğini hissediyorsun.\n\r", victim );
    if ( ch != victim )
	act( L"$N sıcağa karşı korunmaya başladı.",ch,NULL,victim,TO_CHAR);
    return;
}

void spell_protection_cold ( int sn, int level, CHAR_DATA *ch, void *vo , int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected(victim, gsn_protection_cold) )
    {
	if (victim == ch)
  send_to_char( L"Zaten soğuktan korunuyorsun.\n\r",ch);
else
  act( L"$N zaten soğuktan korunuyor.",ch,NULL,victim,TO_CHAR);
	return;
    }

    if ( is_affected(victim, gsn_protection_heat) )
    {
	if (victim == ch)
  send_to_char( L"Zaten sıcaktan korunuyorsun.\n\r",ch);
else
  act( L"$N zaten sıcaktan korunuyor.",ch,NULL,victim,TO_CHAR);
	return;
    }

    if ( is_affected(victim, gsn_fire_shield) )
    {
	if (victim == ch)
  send_to_char( L"Zaten ateş kalkanı kullanıyorsun.\n\r",ch);
else
  act( L"$N zaten ateş kalkanı kullanıyor.",ch,NULL,victim,TO_CHAR);
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
    send_to_char( L"Soğuğa karşı güçlendiğini hissediyorsun.\n\r", victim );
    if ( ch != victim )
	act( L"$N soğuğa karşı korunmaya başladı.",ch,NULL,victim,TO_CHAR);
    return;
}

void spell_fire_shield ( int sn, int level, CHAR_DATA *ch, void *vo , int target)
{
    wchar_t buf[MAX_STRING_LENGTH];
    wchar_t arg[MAX_INPUT_LENGTH];
    OBJ_DATA *fire;
    int i;

    target_name = one_argument(target_name, arg);
    if (!(!wcscasecmp(arg, L"buz") || !wcscasecmp(arg, L"ateş")))
	{
	send_to_char( L"Kalkanın türünü belirtmelisin.\n\r",ch);
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

	swprintf( buf, MAX_STRING_LENGTH-1, fire->short_descr, arg);
	free_string( fire->short_descr );
	fire->short_descr = str_dup( buf );

	swprintf( buf, MAX_STRING_LENGTH-1, fire->description, arg	 );
	free_string( fire->description );
	fire->description = str_dup( buf );

        swprintf( buf, MAX_STRING_LENGTH-1, fire->pIndexData->extra_descr->description, arg );
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
  send_to_char( L"Ateş kalkanı yaratıyorsun.\n\r",ch);
 return;
}

void spell_witch_curse ( int sn, int level, CHAR_DATA *ch, void *vo , int target)
{
  AFFECT_DATA af;
  CHAR_DATA *victim = (CHAR_DATA *) vo;

  if (is_affected(victim,gsn_witch_curse))
    {
      send_to_char( L"Zaten yaşamı akıp gidiyor.\n\r",ch);
      return;
    }

  if (saves_spell((level+5),victim,DAM_MENTAL) || number_range(0,1) == 0)
  {
      send_to_char( L"Başaramadın!\n\r",ch);
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


  send_to_char( L"Şimdi ölüm yoluna girdi.\n\r",ch);
}


void spell_knock ( int sn, int level, CHAR_DATA *ch, void *vo , int target)
{
    wchar_t arg[MAX_INPUT_LENGTH];
    int chance=0;
    int door;
    const	sh_int	rev_dir		[]		=
	{
	    2, 3, 0, 1, 5, 4
	};

    target_name = one_argument(target_name,arg);

    if (arg[0] == '\0')
    {
      send_to_char( L"Hangi yönü ya da kapıyı çalacaksın.\n\r",ch);
    return;
    }

    if (ch->fighting)
    {
      send_to_char( L"Dövüş bitene kadar bekle.\n\r",ch);
	return;
    }

    if ( ( door = find_door( ch, arg ) ) >= 0 )
    {
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit = ch->in_room->exit[door];
	if ( !IS_SET(pexit->exit_info, EX_CLOSED) )
  { send_to_char( L"Zaten açık.\n\r",      ch ); return; }
if ( !IS_SET(pexit->exit_info, EX_LOCKED) )
  { send_to_char( L"Sadece açmayı dene.\n\r",     ch ); return; }
if ( IS_SET(pexit->exit_info, EX_NOPASS) )
  { send_to_char( L"Mistik bir güç çıkışı koruyor.\n\r",ch );
	      return; }
    chance = ch->level / 5 + get_curr_stat(ch,STAT_INT) + get_skill(ch,sn) / 5;

    act( L"$d'yi çalıyor ve açmaya çalışıyorsun!",
		ch,NULL,pexit->keyword,TO_CHAR);
    act( L"$n $d'yi çalıyor ve açmaya çalışıyor!",
		ch,NULL,pexit->keyword,TO_ROOM);

    if (room_dark(ch->in_room))
		chance /= 2;

    /* now the attack */
    if (number_percent() < chance )
     {
	REMOVE_BIT(pexit->exit_info, EX_LOCKED);
	REMOVE_BIT(pexit->exit_info, EX_CLOSED);
  act( L"$n $d'yi çalıyor ve kapı açılıyor.", ch, NULL,
		pexit->keyword, TO_ROOM );
	send_to_char( L"Kapıyı açmayı başardın.\n\r", ch );

	/* open the other side */
	if ( ( to_room   = pexit->u1.to_room            ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != NULL
	&&   pexit_rev->u1.to_room == ch->in_room )
	{
	    CHAR_DATA *rch;

	    REMOVE_BIT( pexit_rev->exit_info, EX_CLOSED );
	    REMOVE_BIT( pexit_rev->exit_info, EX_LOCKED );
	    for ( rch = to_room->people; rch != NULL; rch = rch->next_in_room )
      act( L"$d açılıyor.", rch, NULL, pexit_rev->keyword, TO_CHAR );
	}
     }
    else
     {
       act( L"$d'yi açamadın!",
     	    ch,NULL,pexit->keyword,TO_CHAR);
     	act( L"$n $d'yi açamadı.",
	    ch,NULL,pexit->keyword,TO_ROOM);
     }
    return;
    }

    send_to_char( L"Onu göremiyorsun.\n\r",ch);
  return;
}


void spell_magic_resistance ( int sn, int level, CHAR_DATA *ch, void *vo , int target)
{
  AFFECT_DATA af;

    if (!is_affected(ch, sn))
    {
      send_to_char( L"Artık büyüye dirençlisin.\n\r", ch);

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
  send_to_char( L"Büyüye zaten dirençlisin.\n\r", ch);
 return;
}

void spell_hallucination ( int sn, int level, CHAR_DATA *ch, void *vo , int target)
{
  send_to_char( L"That spell is under construction.\n\r",ch);
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
      send_to_char( L"Başka bir kurt çağıracak gücün yok.\n\r",ch);
      return;
    }

    send_to_char( L"Bir kurt çağırmayı deniyorsun.\n\r",ch);
    act( L"$n bir kurt çağırmayı deniyor.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  gch->pIndexData->vnum == MOB_VNUM_WOLF)
	{
    send_to_char( L"İki kurt kontrol edebileceğinden fazla!\n\r",ch);
	  return;
	}
    }

  if (count_charmed(ch)) return;

  demon = create_mobile( get_mob_index(MOB_VNUM_WOLF), NULL );

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
  demon->silver = 0;
  demon->timer = 0;
  demon->damage[DICE_NUMBER] = number_range(level/15, level/10);
  demon->damage[DICE_TYPE] = number_range(level/3, level/2);
  demon->damage[DICE_BONUS] = number_range(level/6, level/5);

  char_to_room(demon,ch->in_room);
  send_to_char( L"Kurt geliyor ve önünde eğiliyor!\n\r",ch);
  act( L"Biryerlerden bir kurt gelerek selam veriyor!",ch,NULL,NULL,TO_ROOM);

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
      send_to_char( L"Derin bir kaya akdar sert zaten.\n\r",ch);

      else
      act( L"$S derisi bir kaya kadar sert zaten.",ch,NULL,
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
  act( L"$s derisi artık bir kaya kadar sert.",victim,NULL,NULL,TO_ROOM );
  send_to_char( L"Derin artık bir kaya kadar sert.\n\r", victim );
  return;
}


void spell_mind_light(int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    AFFECT_DATA af,af2;

    if ( is_affected_room( ch->in_room, sn ))
    {
      send_to_char( L"Bu odanın mana hızlandırıcısı var zaten.\n\r",ch);
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
    send_to_char( L"Oda zihin ışığıyla dolmaya başlıyor.\n\r", ch );
    act( L"Oda $s zihin ışığıyla dolmaya başlıyor.",ch,NULL,NULL,TO_ROOM);
    return;
}

void spell_insanity ( int sn, int level, CHAR_DATA *ch, void *vo , int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if (IS_NPC(ch))
	{
    send_to_char( L"Bu büyü oyunculara yapılabilir.\n\r",ch);
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
    send_to_char( L"Bir Öfke Kabalı üyesi kadar saldırgansın.\n\r", victim );
    act( L"$n kızıl gözlerle bakıyor.",victim,NULL,NULL,TO_ROOM);
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
    send_to_char( L"Sersemledin.\n\r", victim );
    act_color( L"$n sersemledi.",victim,NULL,NULL,TO_ROOM,POS_SLEEPING,CLR_RED);
    return;
}



void spell_improved_invis( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_IMP_INVIS) )
	return;

  act( L"$n ortadan kayboluyor.", victim, NULL, NULL, TO_ROOM );

    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = level / 10 ;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_IMP_INVIS;
    affect_to_char( victim, &af );
    send_to_char( L"Görünmez oluyorsun.\n\r", victim );
    return;
}



void spell_improved_detection( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( CAN_DETECT(victim, DETECT_IMP_INVIS) )
    {
	if (victim == ch)
  send_to_char( L"Gelişmiş görünmezliği zaten saptayabiliyorsun.\n\r",ch);
else
  act( L"$N zaten gelişmiş görünmezliği saptayabiliyor.",ch,NULL,victim,TO_CHAR);
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
    send_to_char( L"Gözlerin çok yanıyor.\n\r", victim );
    if ( ch != victim )
	send_to_char( L"Tamam.\n\r", ch );
    return;
}

void spell_severity_force( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    wchar_t buf[MAX_STRING_LENGTH];
    int dam;

    swprintf( buf, MAX_STRING_LENGTH-1, L"Arzı %s'e doğru kırıyorsun.\n\r",victim->name);
    send_to_char(buf, ch);
    act( L"$n arzı sana doğru kırıyor!", ch, NULL, victim, TO_VICT );
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
      send_to_char(L"Rasgele gücün tükenmiş durumda.\n\r",ch);
      return;
    }

  if (IS_SET(ch->in_room->room_flags, ROOM_LAW))
    {
      send_to_char(L"Bu oda seninkinin ötesinde bir güçle korunuyor.\n\r",ch);
      return;
    }
    if ( is_affected_room( ch->in_room, sn ))
    {
      send_to_char( L"Bu odaya zaten rasgele uygulanmış.\n\r",ch);
	return;
    }

  if (number_range(0,1) == 0)
    {
      send_to_char( L"Evren karmaşaya direniyor.\n\r",ch);
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
    send_to_char( L"Oda rasgele ile şekillendirildi!\n\r", ch);
    send_to_char( L"Kendini tükenmiş hissediyorsun.\n\r", ch);
    ch->hit -= UMIN(200, ch->hit/2);
    act( L"Odanın çıkışları karışıyor.",ch,NULL,NULL,TO_ROOM);
    return;
}

void spell_bless_weapon(int sn,int level,CHAR_DATA *ch, void *vo,int target)
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    AFFECT_DATA af;

    if (obj->item_type != ITEM_WEAPON)
    {
      send_to_char( L"O bir silah değil.\n\r",ch);
	return;
    }

    if (obj->wear_loc != -1)
    {
      send_to_char( L"Taşıdığın şeyi kutsayabilirsin.\n\r",ch);
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
       act( L"$p kutsanmamış görünüyor.",ch,obj,NULL,TO_CHAR);
	return;
     }
    }
    if (IS_WEAPON_STAT(obj,WEAPON_HOLY))
    {
      act( L"$p zaten kutsanmış.",ch,obj,NULL,TO_CHAR);
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

    act( L"$p kutsal saldırılara hazırlanıyor.",ch,obj,NULL,TO_ALL);
    return;

}

void spell_resilience( int sn, int level, CHAR_DATA *ch, void *vo , int target)
{
  AFFECT_DATA af;

    if (!is_affected(ch, sn))
    {
      send_to_char( L"Emici saldırılara karşı esnekleştin.\n\r", ch);

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
  send_to_char( L"Emici saldırılara karşı ZATEN esneksin.\n\r",ch);
 return;
}

void spell_super_heal( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int bonus = 170 + level + dice(1,20);

    victim->hit = UMIN( victim->hit + bonus, victim->max_hit );
    update_pos( victim );
    send_to_char( L"Sıcak bir duygu vücudunu sarıyor.\n\r", victim );
    if ( ch != victim )
	send_to_char( L"Tamam.\n\r", ch );
    return;
}

void spell_master_heal( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int bonus = 300 + level + dice(1,40);

    victim->hit = UMIN( victim->hit + bonus, victim->max_hit );
    update_pos( victim );
    send_to_char( L"Sıcak bir duygu vücudunu sarıyor.\n\r", victim );
    if ( ch != victim )
	send_to_char( L"Tamam.\n\r", ch );
    return;
}

void spell_group_healing(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    CHAR_DATA *gch;
    int heal_num, refresh_num;

    heal_num = skill_lookup( L"master healing");
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


void spell_restoring_light( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int nsn, mana_add;

    if (IS_AFFECTED(victim,AFF_BLIND))
	{
	 nsn = skill_lookup( L"cure blindness");
	 spell_cure_blindness(nsn,level,ch,(void *)victim,TARGET_CHAR);
	}
    if (IS_AFFECTED(victim,AFF_CURSE))
	{
	 nsn = skill_lookup( L"remove curse");
	 spell_remove_curse(nsn,level,ch,(void *)victim,TARGET_CHAR);
	}
    if (IS_AFFECTED(victim,AFF_POISON))
	{
	 spell_cure_poison(gsn_cure_poison,level,ch,(void *)victim,TARGET_CHAR);
	}
    if (IS_AFFECTED(victim,AFF_PLAGUE))
	{
	 nsn = skill_lookup( L"cure disease");
	 spell_cure_disease(nsn,level,ch,(void *)victim,TARGET_CHAR);
	}

    if (victim->hit != victim->max_hit)
	{
    	 mana_add = UMIN( (victim->max_hit - victim->hit), ch->mana );
    	 victim->hit = UMIN( victim->hit + mana_add, victim->max_hit );
    	 ch->mana -= mana_add;
	}
    update_pos( victim );
    send_to_char( L"Sıcak bir duygu vücudunu sarıyor.\n\r", victim );
    if ( ch != victim )
	send_to_char( L"Tamam.\n\r", ch );
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
      send_to_char( L"Başka bir golem yaratacak gücün yok.\n\r",ch);

      return;
    }

    send_to_char( L"Küçük bir golem yaratmayı deniyorsun.\n\r",ch);
    act( L"$n küçük bir golem yaratmayı deniyor.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  ( gch->pIndexData->vnum == MOB_VNUM_LESSER_GOLEM ) )
	{
	  i++;
	  if (i > 2)
           {
             send_to_char( L"Daha fazla golemi kontrol altından tutamazsın!\n\r",ch);
	    return;
	   }
	}
    }

  if (count_charmed(ch)) return;

  golem = create_mobile( get_mob_index(MOB_VNUM_LESSER_GOLEM), NULL );


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
  golem->silver = 0;
  golem->timer = 0;
  golem->damage[DICE_NUMBER] = 3;
  golem->damage[DICE_TYPE] = 10;
  golem->damage[DICE_BONUS] = ch->level / 2;

  char_to_room(golem,ch->in_room);
  send_to_char( L"Küçük bir golem yaratıyorsun!\n\r",ch);
  act( L"$n küçük bir golem yaratıyor!",ch,NULL,NULL,TO_ROOM);

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
      send_to_char( L"Başka bir golem yaratacak gücün yok.\n\r",ch);
      return;
    }

    send_to_char( L"Bir taş golem yaratmayı deniyorsun.\n\r",ch);
    act( L"$n bir taş golem yaratmayı deniyor.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  ( gch->pIndexData->vnum == MOB_VNUM_STONE_GOLEM ) )
	{
	  i++;
	  if (i > 2)
           {
             send_to_char( L"Daha fazla golemi kontrol altından tutamazsın!\n\r",ch);
	    return;
	   }
	}
    }

  if (count_charmed(ch)) return;

  golem = create_mobile( get_mob_index(MOB_VNUM_STONE_GOLEM), NULL );


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
  golem->silver = 0;
  golem->timer = 0;
  golem->damage[DICE_NUMBER] = 8;
  golem->damage[DICE_TYPE] = 4;
  golem->damage[DICE_BONUS] = ch->level / 2;

  char_to_room(golem,ch->in_room);
  send_to_char( L"Bir taş golem yaratıyorsun!\n\r",ch);
  act( L"$n bir taş golem yaratıyor!",ch,NULL,NULL,TO_ROOM);

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
      send_to_char( L"Başka bir golem yaratacak gücün yok.\n\r",ch);
      return;
    }

    send_to_char( L"Bir demir golem yaratmayı deniyorsun.\n\r",ch);
    act( L"$n bir demir golem yaratmayı deniyor.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  ( gch->pIndexData->vnum == MOB_VNUM_IRON_GOLEM ) )
	{
    send_to_char( L"Daha fazla golemi kontrol altından tutamazsın!\n\r",ch);
	    return;
	}
    }

  if (count_charmed(ch)) return;

  golem = create_mobile( get_mob_index(MOB_VNUM_IRON_GOLEM), NULL );


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
  golem->silver = 0;
  golem->timer = 0;
  golem->damage[DICE_NUMBER] = 11;
  golem->damage[DICE_TYPE] = 5;
  golem->damage[DICE_BONUS] = ch->level / 2 + 10;

  char_to_room(golem,ch->in_room);
  send_to_char( L"Bir demir golem yaratıyorsun!\n\r",ch);
  act( L"$n bir demir golem yaratıyor!",ch,NULL,NULL,TO_ROOM);

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
      send_to_char( L"Başka bir golem yaratacak gücün yok.\n\r",ch);
      return;
    }

    send_to_char( L"Bir serttaşı golem yaratmayı deniyorsun.\n\r",ch);
    act( L"$n bir serttaşı golem yaratmayı deniyor.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  ( gch->pIndexData->vnum == MOB_VNUM_ADAMANTITE_GOLEM ) )
	{
    send_to_char( L"Daha fazla golemi kontrol altından tutamazsın!\n\r",ch);
	    return;
	}
    }

  if (count_charmed(ch)) return;

  golem = create_mobile( get_mob_index(MOB_VNUM_ADAMANTITE_GOLEM) , NULL);


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
  golem->silver = 0;
  golem->timer = 0;
  golem->damage[DICE_NUMBER] = 13;
  golem->damage[DICE_TYPE] = 9;
  golem->damage[DICE_BONUS] = ch->level / 2 + 10;

  char_to_room(golem,ch->in_room);
  send_to_char( L"Bir serttaşı golem yarattın!\n\r",ch);
  act( L"$n bir serttaşı golem yarattı!",ch,NULL,NULL,TO_ROOM);

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
  if (number_range(0,1) == 0)
    {
      send_to_char( L"Başaramadın.\n\r",ch);
      return;
    }

  if (IS_RAFFECTED(ch->in_room,AFF_ROOM_CURSE))
	{
	 affect_strip_room(ch->in_room,gsn_cursed_lands);
   send_to_char( L"Bölgedeki lanet yokoluyor.\n\r",ch);
	 act( L"Bölgedeki lanet yokoluyor.\n\r",ch,NULL,NULL,TO_ROOM);
	}
  if (IS_RAFFECTED(ch->in_room,AFF_ROOM_POISON))
	{
	 affect_strip_room(ch->in_room,gsn_deadly_venom);
   send_to_char( L"Bölge daha sağlıklı görünüyor.\n\r",ch);
	 act( L"Bölge daha sağlıklı görünüyor.\n\r",ch,NULL,NULL,TO_ROOM);
	}
  if (IS_RAFFECTED(ch->in_room,AFF_ROOM_SLEEP))
	{
    send_to_char( L"Bölge gizemli uykusundan uyanıyor.\n\r",ch);
 	 act( L"Bölge gizemli uykusundan uyanıyor.\n\r",ch,NULL,NULL,TO_ROOM);
	 affect_strip_room(ch->in_room,gsn_mysterious_dream);
	}
  if (IS_RAFFECTED(ch->in_room,AFF_ROOM_PLAGUE))
	{
    send_to_char( L"Bölgenin hastalığı yokoldu.\n\r",ch);
 	 act( L"Bölgenin hastalığı yokoldu.\n\r",ch,NULL,NULL,TO_ROOM);
	 affect_strip_room(ch->in_room,gsn_black_death);
	}
  if (IS_RAFFECTED(ch->in_room,AFF_ROOM_SLOW))
	{
    send_to_char( L"Uyuşukluk sisi yokoluyor.\n\r",ch);
  	 act( L"Uyuşukluk sisi yokoluyor.\n\r",ch,NULL,NULL,TO_ROOM);
	 affect_strip_room(ch->in_room,gsn_lethargic_mist);
	}
    return;
}


void spell_deadly_venom(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    AFFECT_DATA af;

  if (IS_SET(ch->in_room->room_flags, ROOM_LAW))
    {
      send_to_char( L"Oda tanrılarca korunuyor.\n\r",  ch);
      return;
    }
    if ( is_affected_room( ch->in_room, sn ))
    {
      send_to_char( L"Oda zaten ölüm zehriyle dolu.\n\r",ch);
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

    send_to_char( L"Oda zehirle dolmaya başlıyor.\n\r",ch);
    act( L"Oda zehirle dolmaya başlıyor.\n\r",ch,NULL,NULL,TO_ROOM);

}

void spell_cursed_lands(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    AFFECT_DATA af;

  if (IS_SET(ch->in_room->room_flags, ROOM_LAW))
    {
      send_to_char( L"Oda tanrılarca korunuyor.\n\r",ch);
      return;
    }
    if ( is_affected_room( ch->in_room, sn ))
    {
      send_to_char( L"Oda zaten lanetli.\n\r",ch);
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

    send_to_char( L"Tanrılar odayı başıboş bıraktılar.\n\r",ch);
    act( L"Tanrılar odayı başıboş bıraktılar.\n\r",ch,NULL,NULL,TO_ROOM);

}

void spell_lethargic_mist(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
   AFFECT_DATA af;

  if (IS_SET(ch->in_room->room_flags, ROOM_LAW))
    {
      send_to_char( L"Oda tanrılarca korunuyor.\n\r",  ch);
      return;
    }
    if ( is_affected_room( ch->in_room, sn ))
    {
      send_to_char( L"Oda zaten uyuşukluk sisiyle dolu.\n\r",ch);
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

    send_to_char( L"Odanın havası seni yavaşlatıyor.\n\r",ch);
    act( L"Odanın havası seni yavaşlatıyor.\n\r",ch,NULL,NULL,TO_ROOM);

}

void spell_black_death(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
  AFFECT_DATA af;

  if (IS_SET(ch->in_room->room_flags, ROOM_LAW))
    {
      send_to_char( L"Oda tanrılarca korunuyor.\n\r",ch);
      return;
    }
    if ( is_affected_room( ch->in_room, sn ))
    {
      send_to_char( L"Bu oda zaten hastalıkla dolu.\n\r",ch);
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

    send_to_char( L"Oda hastalıkla dolmaya başlıyor.\n\r",ch);
     act( L"Oda hastalıkla dolmaya başlıyor.\n\r",ch,NULL,NULL,TO_ROOM);

}

void spell_mysterious_dream(int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
  AFFECT_DATA af;

  if (IS_SET(ch->in_room->room_flags, ROOM_LAW))
    {
      send_to_char( L"Bu oda tanrılarca korunuyor.\n\r",  ch);
      return;
    }
    if ( is_affected_room( ch->in_room, sn ))
    {
      send_to_char( L"Bu oda zaten uyku gazının etkisinde.\n\r",ch);
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

    send_to_char( L"Uyumak için güzel bir yer.\n\r",ch);
    act( L"Uyumak için ne kadar da güzel bir yer.\n\r",ch,NULL,NULL,TO_ROOM);

}

void spell_polymorph( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    AFFECT_DATA af;
    int race;

    if ( is_affected( ch, sn ) )
    {
      send_to_char( L"Zaten şeklin değişmiş.\n\r",ch);
     return;
    }

    if (target_name == NULL)
    {
      send_to_char( L"Kullanım: büyü 'binbir şekil' <ırk>.\n\r",ch);
     return;
    }

    race = race_lookup( target_name );

    if (race == 0 || !race_table[race].pc_race)
    {
      send_to_char( L"O dönüşebileceğin birşey değil.\n\r",ch);
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

    act( L"$n kendini $t yapıyor.",
		ch, race_table[race].name[1], NULL,TO_ROOM );
    act( L"Kendini $t yapıyorsun.\n\r",
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
    send_to_char( L"Burada değil.\n\r",ch);
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
      send_to_char( L"Yakındaki bir ağaca dönüşmeye başlıyorsun!\n\r",ch);
      act( L"$n yakındaki bir ağaca dönüşmeye başlıyor!", ch, NULL, NULL,TO_ROOM );
      af.bitvector = ADET_FORM_TREE;
    }
    else
    {
      send_to_char( L"Bir tutam çimene dönüşmeye başlıyorsun!\n\r",ch);
      act( L"$n bir tutam çimene dönüşmeye başlıyor!", ch, NULL, NULL, TO_ROOM );
      af.bitvector = ADET_FORM_GRASS;
    }
    affect_to_char( ch, &af );
    return;
}


void spell_blade_barrier(int sn,int level,CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    wchar_t buf[MAX_STRING_LENGTH];
    int dam;

    act( L"$s çevresinde bıçaklar oluşuyor ve $E çarpıyor.",
	ch,NULL,victim,TO_ROOM);
    act( L"Çevrende bıçaklar oluşuyor ve $E çarpıyor.",
	ch,NULL,victim,TO_CHAR);
    act( L"$s çevresinde bıçaklar oluşuyor ve sana çarpıyor!",
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
          do_yell(victim, (wchar_t*)"İmdat biri bana saldırıyor!");
        else
          {
            swprintf( buf, MAX_STRING_LENGTH-1, L"Geber %s, seni büyücü köpek!",
                    (is_affected(ch,gsn_doppelganger)&&!IS_IMMORTAL(victim))?
                    ch->doppel->name : ch->name);
            do_yell(victim,buf);
          }
      }

      act( L"Bıçak duvarı $e çarpıyor!",victim,NULL,NULL,TO_ROOM);
    dam = dice(level,5);
    if (saves_spell(level,victim,DAM_PIERCE))
	dam /= 2;
    damage(ch,victim,dam,sn,DAM_PIERCE,TRUE);
    act( L"Bıçak duvarı sana çarpıyor!",victim,NULL,NULL,TO_CHAR);

    if ( number_percent() < 50 ) return;

    act( L"Bıçak duvarı $e çarpıyor!",victim,NULL,NULL,TO_ROOM);
    dam = dice(level,4);
    if (saves_spell(level,victim,DAM_PIERCE))
	dam /= 2;
    damage(ch,victim,dam,sn,DAM_PIERCE,TRUE);
    act( L"Bıçak duvarı sana çarpıyor!",victim,NULL,NULL,TO_CHAR);

    if ( number_percent() < 50 ) return;

    act( L"Bıçak duvarı $e çarpıyor!",victim,NULL,NULL,TO_ROOM);
    dam = dice(level,2);
    if (saves_spell(level,victim,DAM_PIERCE))
	dam /= 2;
    damage(ch,victim,dam,sn,DAM_PIERCE,TRUE);
    act( L"Bıçak duvarı sana çarpıyor!",victim,NULL,NULL,TO_CHAR);

}


void spell_protection_negative ( int sn, int level, CHAR_DATA *ch, void *vo , int target)
{
  AFFECT_DATA af;

    if (!is_affected(ch, sn))
    {
      send_to_char( L"Negatif saldırılara bağışıklı olduğunu hissediyorsun.\n\r", ch);

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
  send_to_char( L"Zaten negatif saldırılara bağışıklısın.\n\r", ch);
 return;
}


void spell_ruler_aura( int sn, int level, CHAR_DATA *ch, void *vo , int target)
{
  AFFECT_DATA af;

    if (!is_affected(ch, sn))
    {
      send_to_char( L"Kendine ve ideallerine olan inancın güçleniyor.\n\r", ch);

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
  send_to_char( L"Kendinden eminsin zaten.\n\r", ch);
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
    send_to_char( L"Bölge zaten şerle dolu.\n\r",ch);
   return;
  }

 if ( is_affected( ch, sn ) )
    {
      send_to_char( L"Yeterince gücün yok.\n\r",ch);
      return;
    }

  if (IS_SET(ch->in_room->room_flags, ROOM_LAW)
	|| IS_SET(ch->in_room->area->area_flag,AREA_HOMETOWN) )
    {
      send_to_char( L"Odadaki kutsal aura seni engelliyor.\n\r",ch);
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
     act( L"Oda şer ruhların bilinciyle dolmaya başlıyor.",room->people,NULL,NULL,TO_ALL);
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

      act( L"$N kendinden emin değil!",ch,NULL,victim,TO_ALL);
    }
  else send_to_char( L"Başaramadın.\n\r",ch);
}


void spell_control_undead( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;

    if  ( !IS_NPC(victim) || !IS_SET(victim->act,ACT_UNDEAD) )
    {
      act( L"$N bir hortlağa benzemiyor.",ch,NULL,victim,TO_CHAR);
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
        send_to_char( L"Bu güç yakın zamanda kullanıldı.\n\r",ch);
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
    send_to_char( L"Sıcak bir his vücudunu sarıyor.\n\r", victim );
    act( L"$n daha iyi görünüyor.", victim, NULL, NULL, TO_ROOM);
    if (ch != victim) send_to_char( L"Tamam.\n\r",ch);
    return;
}



void spell_aid( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( ch, sn ) )
      {
        send_to_char( L"Bu güç yakın zamanda kullanıldı.\n\r",ch);
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
    send_to_char( L"Sıcak bir his vücudunu sarıyor.\n\r", victim );
    act( L"$n daha iyi görünüyor.", victim, NULL, NULL, TO_ROOM);
    if (ch != victim) send_to_char( L"Tamam.\n\r",ch);
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
      send_to_char( L"Başka bir gölge çağırmak için yeterli gücün yok.\n\r",ch);
      return;
    }

    send_to_char( L"Bir gölge çağırmayı deniyorsun.\n\r",ch);
    act( L"$n bir gölge çağırmayı deniyor.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  gch->pIndexData->vnum == MOB_VNUM_SUM_SHADOW)
	{
    send_to_char( L"İki gölge kontrol edebileceğinden fazla!\n\r",ch);
	  return;
	}
    }

  if (count_charmed(ch)) return;

  shadow = create_mobile( get_mob_index(MOB_VNUM_SUM_SHADOW), NULL );

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
  shadow->silver = 0;
  shadow->timer = 0;
  shadow->damage[DICE_NUMBER] = number_range(level/15, level/10);
  shadow->damage[DICE_TYPE] = number_range(level/3, level/2);
  shadow->damage[DICE_BONUS] = number_range(level/8, level/6);

  char_to_room(shadow,ch->in_room);
  act( L"Bir gölge oluşuyor!",ch,NULL,NULL,TO_ALL);

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
        send_to_char( L"O uzaklığı göremezsin.\n\r",ch);
	return;
      }

    if (ch->in_room == room)
      do_look( ch, (wchar_t*)"auto" );
    else
    {
      mount = MOUNTED(ch) ? 1 : 0;
      oldr = ch->in_room;
      char_from_room( ch );
      char_to_room( ch, room );
      do_look( ch, (wchar_t*)"auto" );
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
      send_to_char( L"Daha cesur hissediyorsun.\n\r",victim);
    	act( L"$n daha bilinçli görünüyor.",victim,NULL,NULL,TO_ROOM);
    }
    else send_to_char( L"Başaramadın.\n\r",ch);
}

void spell_desert_fist( int sn, int level, CHAR_DATA *ch, void *vo,int target )
 {
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    if ( (ch->in_room->sector_type != SECT_HILLS)
	&& (ch->in_room->sector_type != SECT_MOUNTAIN)
	&& (ch->in_room->sector_type != SECT_DESERT) )
	{
    send_to_char( L"Yumruk yaratmak için gerekli kumu bulamıyorsun.\n\r",ch);
	 ch->wait = 0;
	 return;
	}

  act( L"Büyük bir kum kütlesi yükselerek bir yumruk oluşturuyor, ve $e vuruyor.",
  victim,NULL,NULL,TO_ROOM);
  act( L"Büyük bir kum kütlesi yükselerek bir yumruk oluşturuyor, ve sana vuruyor.",
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
        send_to_char( L"Zaten kutsal bir aylan var.\n\r",ch);
       else
         act( L"$S zaten kutsal bir aylası var.",ch,NULL,victim,TO_CHAR);
        return;
    }

    if ( !IS_GOOD(victim) )
    {
      send_to_char( L"O kutsal auraya layık değil!", ch);
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

    send_to_char( L"Kadim güçlerin seni koruduğunu hissediyorsun.\n\r", victim );
    if ( ch != victim )
        act( L"$N kadim kutsal güçlerce korunmaya başlıyor.",ch,NULL,victim,TO_CHAR);
    return;
}

void spell_holy_fury(int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if (is_affected(victim,sn) || IS_AFFECTED(victim,AFF_BERSERK))
    {
        if (victim == ch)
        send_to_char( L"Zaten hiddetlisin.\n\r",ch);
      else
        act( L"$N zaten hiddetli.",ch,NULL,victim,TO_CHAR);
        return;
    }

    if (is_affected(victim,skill_lookup( L"calm")))
    {
        if (victim == ch)
        send_to_char( L"Neden bir an için rahatlamıyorsun?\n\r",ch);
      else
        act( L"$N kavgaya meraklıymış gibi durmuyor.",
              ch,NULL,victim,TO_CHAR);
        return;
    }

    if ( !IS_GOOD(victim) )
    {
      act( L"$N kutsal hiddette layık değil.",ch,NULL,victim,TO_CHAR);
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

    send_to_char( L"Kutsal hiddetle doluyorsun!\n\r",victim);
    act( L"$s gözlerine hiddet doluyor!",victim,NULL,NULL,TO_ROOM);
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
    send_to_char( L"Su kaynağına erişemiyorsun.\n\r",ch);
	 ch->wait = 0;
	 return;
	}

  act( L"$s çevresindeki su molekülleri birleşerek bir yumruk oluşturuyor.",
  ch,NULL,NULL,TO_ROOM);
  act( L"Çevrendeki su molekülleri birleşerek bir yumruk oluşturuyor.",
		ch,NULL,NULL,TO_CHAR);
    dam = dice( level , 14 );
    damage(ch,victim,dam,sn,DAM_BASH,TRUE);
}

void spell_wolf_spirit( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    AFFECT_DATA af;

    if ( is_affected( ch, sn ) )
    {
      send_to_char( L"Damarlarındaki kan zaten olabildiğince hızlı akıyor!\n\r",ch);
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


    send_to_char( L"Damarlarındaki kan hızlı akmaya başlıyor.\n\r", ch );
    act( L"$s gözleri KIZILLAŞIYOR!",ch,NULL,NULL,TO_ROOM);
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
	send_to_char( L"Başaramadın!\n\r", ch);
	return;
    }
  }

  if (!IS_NPC(victim) && IS_SET(victim->act, PLR_WANTED))
	dam = dice(level, 20);
  else	dam = dice(level, 14);

  if (saves_spell(level,victim, DAM_MENTAL))
	      dam /= 2;

  do_yell(ch, (wchar_t*)"Adalet Kılıcı!");
  act( L"Adalet kılıcı ortaya çıkarak $E vuruyor!",ch,NULL,victim,TO_ALL);

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
      send_to_char( L"Başka bir köpeği çağıracak gücün yok.\n\r",ch);
      return;
    }

    send_to_char( L"Bir köpek çağırmayı deniyorsun.\n\r",ch);
    act( L"$n bir köpek çağırmayı deniyor.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  gch->pIndexData->vnum == MOB_VNUM_DOG)
	{
    send_to_char( L"İki köpek kontrol edebileceğinden fazla!\n\r",ch);
	  return;
	}
    }

  if (count_charmed(ch)) return;

  dog = create_mobile( get_mob_index(MOB_VNUM_DOG) , NULL);

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
  dog->silver = 0;
  dog->timer = 0;
  dog->damage[DICE_NUMBER] = number_range(level/15, level/12);
  dog->damage[DICE_TYPE] = number_range(level/3, level/2);
  dog->damage[DICE_BONUS] = number_range(level/10, level/8);

  dog2 = create_mobile(dog->pIndexData, NULL);
  clone_mobile(dog,dog2);

  SET_BIT(dog->affected_by, AFF_CHARM);
  SET_BIT(dog2->affected_by, AFF_CHARM);
  dog->master = dog2->master = ch;
  dog->leader = dog2->leader = ch;

  char_to_room(dog,ch->in_room);
  char_to_room(dog2,ch->in_room);
  send_to_char( L"İki köpek geliyor ve önünde eğiliyor!\n\r",ch);
  act( L"İki köpek geliyor ve selam veriyor!",ch,NULL,NULL,TO_ROOM);

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
        send_to_char( L"Kaplan gözlerin öyle birini görmüyor.\n\r",ch);
	return;
      }

    if ( IS_NPC(victim) || victim->cabal != CABAL_HUNTER)
    {
      send_to_char( L"Kaplan gözleri yalnız avcıları görür!\n\r",ch);
	return;
    }

    if ((victim->level > ch->level + 7)
	|| saves_spell((ch->level + 9), victim, DAM_NONE))
      {
        send_to_char( L"Kaplan gözlerin onu görmüyor.\n\r",ch);
	return;
      }

    if (ch==victim)
      do_look( ch, (wchar_t*)"auto" );
    else {
      ori_room = ch->in_room;
      char_from_room( ch );
      char_to_room( ch, victim->in_room );
      do_look( ch, (wchar_t*)"auto" );
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

  act( L"$p yaratıyorsun!",ch,shield,NULL,TO_CHAR);
  act( L"$n $p yaratıyor!",ch,shield,NULL,TO_ROOM);
}

void spell_evolve_lion( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  AFFECT_DATA af;

  if (is_affected(ch,sn) || ch->hit > ch->max_hit)
    {
      send_to_char( L"Olabildiğince aslansın zaten.\n\r",ch);
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

  send_to_char( L"Kendini biraz sakar, fakat daha güçlü hissediyorsun.\n\r",ch);
  act( L"$s derisi grileşiyor!",ch,NULL,NULL,TO_ROOM);
}

void spell_prevent(int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    AFFECT_DATA af,af2;

    if ( is_affected_room( ch->in_room, sn ))
    {
      send_to_char( L"Bu odada zaten intikam alınamıyor!\n\r",ch);
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
    send_to_char( L"Bu odada intikam alınamayacak!\n\r", ch );
    act( L"Oda $s Avcıları engelleyici gücüyle dolmaya başlıyor",ch,NULL,NULL,TO_ROOM);
    return;
}


void spell_enlarge( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
    {
	if (victim == ch)
  send_to_char( L"Daha fazla genişleyemezsin!\n\r",ch);
else
  act( L"$N olabildiğince genişlemiş zaten.",ch,NULL,victim,TO_CHAR);
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

    send_to_char( L"Gittikçe irileştiğini hissediyorsun.\n\r", victim );
    act( L"$s vücudu genişlemeye başladı.",victim,NULL,NULL,TO_ROOM);
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
	spell_blindness(skill_lookup( L"blindness"),
		 (level-10),ch,(void *) victim,TARGET_CHAR);

	spell_slow(skill_lookup( L"slow"),
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
          act( L"$N zaten nefes alamıyor.\n\r",ch,NULL,victim,TO_CHAR);
         return;
        }

    if (saves_spell(level,victim,DAM_NEGATIVE) ||
        (IS_NPC(victim) && IS_SET(victim->act,ACT_UNDEAD)))
    {
        if (ch == victim)
        send_to_char( L"Bir an için kendini hasta hissediyorsun, ama sonra geçiyor.\n\r",ch);
      else
        act( L"$N etkilenmiş görünmüyor.",ch,NULL,victim,TO_CHAR);
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

    send_to_char ( L"Nefes alamıyorsun.\n\r",victim);
    act( L"$n nefes almaya çalışıyor, ama başaramıyor.", victim,NULL,NULL,TO_ROOM);

  return;
}

void spell_mummify( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim;
    CHAR_DATA *undead;
    OBJ_DATA *obj,*obj2,*next;
    AFFECT_DATA af;
    wchar_t buf[MAX_STRING_LENGTH];
    wchar_t buf2[MAX_STRING_LENGTH];
    wchar_t buf3[MAX_STRING_LENGTH];
    wchar_t *argument,*arg;
    int i;

    /* deal with the object case first */
    if (target == TARGET_OBJ)
    {
	obj = (OBJ_DATA *) vo;

    if (!(obj->item_type == ITEM_CORPSE_NPC ||
          obj->item_type == ITEM_CORPSE_PC))
    {
      send_to_char( L"Yalnız cesetleri canlandırabilirsin!\n\r",ch);
	return;
    }

    if (obj->level > level + 10)
    {
      send_to_char( L"Ölü beden mumlayamayacağın kadar güçlü!\n\r",ch);
	return;
    }

    if (is_affected(ch, sn))
    {
      send_to_char( L"Daha fazla cesedi mumyalayacak gücü bulamıyorsun.\n\r", ch);
      return;
    }

  if (count_charmed(ch)) return;

  if ( ch->in_room != NULL && IS_SET(ch->in_room->room_flags, ROOM_NO_MOB) )
  {
    send_to_char( L"Ölüleri burada mumyalayamazsın.\n\r", ch );
     return;
  }

  if ( IS_SET(ch->in_room->room_flags, ROOM_SAFE)      ||
       IS_SET(ch->in_room->room_flags, ROOM_PRIVATE)   ||
       IS_SET(ch->in_room->room_flags, ROOM_SOLITARY)  )
  {
    send_to_char( L"Burada mumyalayamazsın.\n\r", ch );
    return;
  }

  undead = create_mobile( get_mob_index(MOB_VNUM_UNDEAD), NULL );
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
  undead->silver = 0;

  SET_BIT(undead->act, ACT_UNDEAD);
  SET_BIT(undead->affected_by, AFF_CHARM);
  undead->master = ch;
  undead->leader = ch;

  swprintf( buf, MAX_STRING_LENGTH-1, L"%s beden hortlak", obj->name);
  undead->name = str_dup(buf);
  swprintf(buf2, MAX_STRING_LENGTH-1, L"%s",obj->short_descr);
  argument = (wchar_t *)alloc_perm ( MAX_STRING_LENGTH );
  arg = (wchar_t *)alloc_perm ( MAX_STRING_LENGTH );
  argument = buf2;
  buf3[0] = '\0';
  while (argument[0] != '\0' )
	{
  argument = one_argument(argument, arg);
  if (!(!wcscasecmp(arg, L"hortlak") || !wcscasecmp(arg, L"beden") ||
	!wcscasecmp(arg, L"ceset") ))
	 {
	  if (buf3[0] == '\0')   wcscat(buf3,arg);
	  else  {
		 wcscat(buf3,L" ");
		 wcscat(buf3,arg);
		}
	 }
	}
  swprintf( buf, MAX_STRING_LENGTH-1, L"%s'in mumyalanmış bedeni", buf3);
  undead->short_descr = str_dup(buf);
  swprintf( buf, MAX_STRING_LENGTH-1, L"%s'in mumyalanmış bedeni aksayarak dolaşıyor.\n\r", buf3);
  undead->long_descr = str_dup(buf);

  for(obj2 = obj->contains;obj2;obj2=next)
    {
	next = obj2->next_content;
	obj_from_obj(obj2);
	obj_to_char(obj2, undead);
    }
    interpret( undead,(wchar_t*)"giy tümü", TRUE);

  af.where	 = TO_AFFECTS;
  af.type      = sn;
  af.level	 = ch->level;
  af.duration  = (ch->level / 10);
  af.modifier  = 0;
  af.bitvector = 0;
  af.location  = APPLY_NONE;
  affect_to_char( ch, &af );

  send_to_char( L"Mistik güçlerin yardımıyla ona yaşam veriyorsun!\n\r",ch);
  swprintf( buf, MAX_STRING_LENGTH-1, L"Mistik güçlerin yardımıyla %s %s'i mumyalayarak ona yaşam veriyor!",
		ch->name,obj->name);
  act(buf,ch,NULL,NULL,TO_ROOM);
  swprintf( buf, MAX_STRING_LENGTH-1, L"%s sana bakarak onu rahatsız etmenin bedelini ödetmenin planlarını yapıyor!",
	obj->short_descr);
  act(buf,ch,NULL,NULL,TO_CHAR);
  extract_obj (obj);
	return;
    }

  victim = (CHAR_DATA *) vo;

  if ( victim == ch )
  {
    send_to_char( L"Fakat sen ölü değilsin!!\n\r", ch );
  	return;
    }

    send_to_char( L"O ölü değil!!\n\r", ch );
  return;
}

void spell_soul_bind( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;

  if (ch->pet != NULL)
  {
    send_to_char( L"Zaten ruhun birine bağlı.\n\r", ch);
	return;
  }

  if (!IS_NPC(victim)
	|| !(IS_AFFECTED(victim, AFF_CHARM) && victim->master == ch))
  {
    send_to_char( L"O ruhu kendine bağlayamazsın.\n\r", ch);
	return;
  }

  victim->leader = ch;
  ch->pet = victim;

  act( L"$S ruhunu kendine bağlıyorsun.", ch, NULL, victim, TO_CHAR);
  act( L"$n $S ruhunu kendine bağlıyor.", ch, NULL, victim, TO_ROOM);

  return;
}

void spell_forcecage( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( CAN_DETECT(victim, ADET_PROTECTOR) )
    {
      if (victim == ch)
      send_to_char( L"Zaten çevrende bir güç kafesi var.\n\r",ch);
    else
      act( L"$S çevresinde bir güç kafesi zaten var.",ch,NULL,victim,TO_CHAR);
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
    act( L"$n çevresinde bir kafes oluşturmak için unutulmuş güçleri çağırıyor.", victim, NULL, NULL, TO_ROOM );
    send_to_char( L"Çevrende bir kafes oluşturmak için unutulmuş güçleri çağırıyorsun.\n\r", victim );
    return;
}

void spell_iron_body( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( CAN_DETECT(victim, ADET_PROTECTOR) )
    {
      if (victim == ch)
      send_to_char( L"Derin zaten demir gibi sert.\n\r",ch);
    else
      act( L"$S derisi zaten demir gibi sert.",ch,NULL,victim,TO_CHAR);
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
    act( L"$s derisi demir gibi sertleşti.", victim, NULL, NULL, TO_ROOM );
    send_to_char( L"Derin demir gibi sertleşti.\n\r", victim );
    return;
}

void spell_elemental_sphere( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( CAN_DETECT(victim, ADET_PROTECTOR) )
    {
      if (victim == ch)
      send_to_char( L"Seni koruyan bir tanesi zaten var.\n\r",ch);
    else
      act( L"$M koruyan bir tanesi zaten var.",ch,NULL,victim,TO_CHAR);
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
    act( L"$n doğayla ilgili tüm güçleri kendisine bir küre yapmak için kullanıyor.", victim, NULL, NULL, TO_ROOM );
    send_to_char( L"Doğayla ilgili tüm güçleri kendine bir küre yapmak için kullanıyorsun.\n\r", victim );
    return;
}


void spell_aura_of_chaos( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if (ch->cabal != victim->cabal)
      {
        send_to_char( L"Bu büyüyü yalnız kabal dostların üzerinde kullanabilirsin.\n\r",ch);
	return;
      }

    if ( is_affected( victim, sn ) )
    {
	if (victim == ch)
  send_to_char( L"Zaten kaos tanrılarınca korunuyorsun.\n\r",ch);
else
  act( L"$N zaten kaos tanrılarınca korunuyor.",ch,NULL,victim,TO_CHAR);
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
    send_to_char( L"Kaos tanrılarınca korunduğunu hissediyorsun.\n\r", victim );
    if ( ch != victim )
    act( L"$S çevresinde bir aura oluşuyor.",ch,NULL,victim,TO_CHAR);
    return;
}
