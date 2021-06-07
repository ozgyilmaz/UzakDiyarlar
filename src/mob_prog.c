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
#include "interp.h"
#include "recycle.h"

void raw_kill args( (CHAR_DATA *victim) );
DECLARE_DO_FUN(do_yell);
DECLARE_DO_FUN(do_cb);
DECLARE_DO_FUN(do_murder);
DECLARE_DO_FUN(do_help);
DECLARE_DO_FUN(do_murder);
int find_path( int in_room_vnum, int out_room_vnum, CHAR_DATA *ch, int depth, int in_zone );
int lookup_religion_name (const wchar_t *name);
void heal_battle(CHAR_DATA *mob,CHAR_DATA *ch );
void	say_spell	args( ( CHAR_DATA *ch, int sn ) );
void	one_hit		args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ,bool secondary) );

DECLARE_MPROG_FUN_GREET( greet_prog_ruler_pre   );
DECLARE_MPROG_FUN_GREET( greet_prog_ruler 	);
DECLARE_MPROG_FUN_GREET( greet_prog_shalafi 	);
DECLARE_MPROG_FUN_GREET( greet_prog_battle 	);
DECLARE_MPROG_FUN_GREET( greet_prog_chaos 	);
DECLARE_MPROG_FUN_GREET( greet_prog_knight 	);
DECLARE_MPROG_FUN_GREET( greet_prog_invader 	);
DECLARE_MPROG_FUN_GREET( greet_prog_lions 	);
DECLARE_MPROG_FUN_GREET( greet_prog_hunter 	);
DECLARE_MPROG_FUN_SPEECH(speech_prog_hunter_cleric);

DECLARE_MPROG_FUN_BRIBE( bribe_prog_cityguard	);

DECLARE_MPROG_FUN_SPEECH( speech_prog_keeper 	);
DECLARE_MPROG_FUN_GREET( greet_prog_fireflash 	);
DECLARE_MPROG_FUN_GREET( greet_prog_solamnia 	);
DECLARE_MPROG_FUN_GIVE( give_prog_solamnia 	);
DECLARE_MPROG_FUN_GIVE( give_prog_fireflash 	);
DECLARE_MPROG_FUN_GIVE( give_prog_keeper 	);
DECLARE_MPROG_FUN_GIVE( give_prog_dressmaker 	);
DECLARE_MPROG_FUN_GREET( greet_prog_keeper 	);

DECLARE_MPROG_FUN_DEATH( death_prog_stalker 	);

DECLARE_MPROG_FUN_GREET( greet_prog_templeman 	);
DECLARE_MPROG_FUN_SPEECH( speech_prog_templeman );
DECLARE_MPROG_FUN_FIGHT( fight_prog_diana  	);
DECLARE_MPROG_FUN_FIGHT( fight_prog_ofcol_guard );
DECLARE_MPROG_FUN_SPEECH( speech_prog_wiseman	);


DECLARE_MPROG_FUN_GREET( greet_prog_armourer	);
DECLARE_MPROG_FUN_GREET( greet_prog_baker	);
DECLARE_MPROG_FUN_GREET( greet_prog_beggar	);
DECLARE_MPROG_FUN_GREET( greet_prog_drunk	);
DECLARE_MPROG_FUN_GREET( greet_prog_grocer	);

DECLARE_MPROG_FUN_BRIBE( bribe_prog_beggar 	);
DECLARE_MPROG_FUN_BRIBE( bribe_prog_drunk 	);

DECLARE_MPROG_FUN_FIGHT( fight_prog_beggar 	);

DECLARE_MPROG_FUN_DEATH( death_prog_beggar 	);
DECLARE_MPROG_FUN_DEATH( death_prog_vagabond	);

DECLARE_MPROG_FUN_SPEECH( speech_prog_crier	);

DECLARE_MPROG_FUN_AREA( area_prog_drunk		);
DECLARE_MPROG_FUN_AREA( area_prog_janitor	);
DECLARE_MPROG_FUN_AREA( area_prog_vagabond	);
DECLARE_MPROG_FUN_AREA( area_prog_baker		);
DECLARE_MPROG_FUN_AREA( area_prog_grocer	);
DECLARE_MPROG_FUN_FIGHT( fight_prog_golem  	);


#define GIVE_HELP_RELIGION	16
#define RELIG_CHOSEN		17


void mprog_set(MOB_INDEX_DATA *mobindex,const wchar_t *progtype, const wchar_t *name)
{
   if (!wcscasecmp(progtype, L"bribe_prog"))
     {
 /*
  *    if (!wcscasecmp(name, L"bribe_prog"))
  *          mobindex->mprogs->bribe_prog = bribe_prog_;
  *    else if (!wcscasecmp(name, L"bribe_prog")) ...
  */

       if (!wcscasecmp(name, L"bribe_prog_cityguard"))
	 mobindex->mprogs->bribe_prog = bribe_prog_cityguard;
       else if (!wcscasecmp(name, L"bribe_prog_beggar"))
	 mobindex->mprogs->bribe_prog = bribe_prog_beggar;
       else if (!wcscasecmp(name, L"bribe_prog_drunk"))
	 mobindex->mprogs->bribe_prog = bribe_prog_drunk;

       else
	 {
	   bug( L"Load_mprogs: 'M': Function not found for vnum %d",
	       mobindex->vnum);
	   exit(1);
	 }

       SET_BIT(mobindex->progtypes, MPROG_BRIBE);
       return;
     }

   if (!wcscasecmp(progtype, L"entry_prog"))
     {
/*
       if (!wcscasecmp(name, L"entry_prog_"))
	 mobindex->mprogs->entry_prog = entry_prog_;
       else
*/
	 {
	   bug( L"Load_mprogs: 'M': Function not found for vnum %d",
	       mobindex->vnum);
	   exit(1);
	 }
       SET_BIT(mobindex->progtypes, MPROG_ENTRY);
       return;
     }
   if (!wcscasecmp(progtype, L"greet_prog"))
     {
       if (!wcscasecmp(name, L"greet_prog_shalafi"))
	 mobindex->mprogs->greet_prog = greet_prog_shalafi;
       else if (!wcscasecmp(name, L"greet_prog_knight"))
	 mobindex->mprogs->greet_prog = greet_prog_knight;
       else if (!wcscasecmp(name, L"greet_prog_lions"))
	 mobindex->mprogs->greet_prog = greet_prog_lions;
       else if (!wcscasecmp(name, L"greet_prog_hunter"))
	 mobindex->mprogs->greet_prog = greet_prog_hunter;
       else if (!wcscasecmp(name, L"greet_prog_invader"))
	 mobindex->mprogs->greet_prog = greet_prog_invader;
       else if (!wcscasecmp(name, L"greet_prog_ruler"))
	 mobindex->mprogs->greet_prog = greet_prog_ruler;
       else if (!wcscasecmp(name, L"greet_prog_ruler_pre"))
	 mobindex->mprogs->greet_prog = greet_prog_ruler_pre;
       else if (!wcscasecmp(name, L"greet_prog_battle"))
	 mobindex->mprogs->greet_prog = greet_prog_battle;
       else if (!wcscasecmp(name, L"greet_prog_chaos"))
	 mobindex->mprogs->greet_prog = greet_prog_chaos;
       else if (!wcscasecmp(name, L"greet_prog_fireflash"))
	 mobindex->mprogs->greet_prog = greet_prog_fireflash;
       else if (!wcscasecmp(name, L"greet_prog_solamnia"))
	 mobindex->mprogs->greet_prog = greet_prog_solamnia;
       else if (!wcscasecmp(name, L"greet_prog_keeper"))
	 mobindex->mprogs->greet_prog = greet_prog_keeper;
       else if (!wcscasecmp(name, L"greet_prog_templeman"))
	 mobindex->mprogs->greet_prog = greet_prog_templeman;
       else if (!wcscasecmp(name, L"greet_prog_armourer"))
	 mobindex->mprogs->greet_prog = greet_prog_armourer;
       else if (!wcscasecmp(name, L"greet_prog_baker"))
	 mobindex->mprogs->greet_prog = greet_prog_baker;
       else if (!wcscasecmp(name, L"greet_prog_beggar"))
	 mobindex->mprogs->greet_prog = greet_prog_beggar;
       else if (!wcscasecmp(name, L"greet_prog_drunk"))
	 mobindex->mprogs->greet_prog = greet_prog_drunk;
       else if (!wcscasecmp(name, L"greet_prog_grocer"))
	 mobindex->mprogs->greet_prog = greet_prog_grocer;
       else
	 {
	   bug( L"Load_mprogs: 'M': Function not found for vnum %d",
	       mobindex->vnum);
	   exit(1);
	 }

       SET_BIT(mobindex->progtypes, MPROG_GREET);
       return;
     }

   if (!wcscasecmp(progtype, L"fight_prog"))
     {
       if (!wcscasecmp(name, L"fight_prog_diana"))
	 mobindex->mprogs->fight_prog = fight_prog_diana;
       else if (!wcscasecmp(name, L"fight_prog_ofcol_guard"))
	 mobindex->mprogs->fight_prog = fight_prog_ofcol_guard;
       else if (!wcscasecmp(name, L"fight_prog_beggar"))
	 mobindex->mprogs->fight_prog = fight_prog_beggar;
       else if (!wcscasecmp(name, L"fight_prog_golem"))
	 mobindex->mprogs->fight_prog = fight_prog_golem;
       else
	 {
	   bug( L"Load_mprogs: 'M': Function not found for vnum %d",
	       mobindex->vnum);
	   exit(1);
	 }
       SET_BIT(mobindex->progtypes, MPROG_FIGHT);
       return;
     }
   if (!wcscasecmp(progtype, L"death_prog")) /* returning TRUE prevents death */
     {
       if (!wcscasecmp(name, L"death_prog_stalker"))
	 mobindex->mprogs->death_prog = death_prog_stalker;
       else if (!wcscasecmp(name, L"death_prog_beggar"))
	 mobindex->mprogs->death_prog = death_prog_beggar;
       else if (!wcscasecmp(name, L"death_prog_vagabond"))
	 mobindex->mprogs->death_prog = death_prog_vagabond;
       else
	 {
	   bug( L"Load_mprogs: 'M': Function not found for vnum %d",
	       mobindex->vnum);
	   exit(1);
	 }
       SET_BIT(mobindex->progtypes, MPROG_DEATH);
       return;
     }
   if (!wcscasecmp(progtype, L"area_prog"))
     {
       if (!wcscasecmp(name, L"area_prog_drunk"))
	 mobindex->mprogs->area_prog = area_prog_drunk;
       else if (!wcscasecmp(name, L"area_prog_janitor"))
	 mobindex->mprogs->area_prog = area_prog_janitor;
       else if (!wcscasecmp(name, L"area_prog_vagabond"))
	 mobindex->mprogs->area_prog = area_prog_vagabond;
       else if (!wcscasecmp(name, L"area_prog_baker"))
	 mobindex->mprogs->area_prog = area_prog_baker;
       else if (!wcscasecmp(name, L"area_prog_grocer"))
	 mobindex->mprogs->area_prog = area_prog_grocer;
       else
	 {
	   bug( L"Load_mprogs: 'M': Function not found for vnum %d",
	       mobindex->vnum);
	   exit(1);
	 }
       SET_BIT(mobindex->progtypes, MPROG_AREA);
       return;
     }
   if (!wcscasecmp(progtype, L"speech_prog"))
     {
       if (!wcscasecmp(name, L"speech_prog_keeper"))
	 mobindex->mprogs->speech_prog = speech_prog_keeper;
       else if(!wcscasecmp(name, L"speech_prog_templeman"))
	 mobindex->mprogs->speech_prog = speech_prog_templeman;
       else if(!wcscasecmp(name, L"speech_prog_wiseman"))
	 mobindex->mprogs->speech_prog = speech_prog_wiseman;
       else if(!wcscasecmp(name, L"speech_prog_crier"))
	 mobindex->mprogs->speech_prog = speech_prog_crier;
       else if(!wcscasecmp(name, L"speech_prog_hunter_cleric"))
	 mobindex->mprogs->speech_prog = speech_prog_hunter_cleric;
       else
	 {
	   bug( L"Load_mprogs: 'M': Function not found for vnum %d",
	       mobindex->vnum);
	   exit(1);
	 }
       SET_BIT(mobindex->progtypes, MPROG_SPEECH);
       return;
     }
   if (!wcscasecmp(progtype, L"give_prog"))
     {
       if (!wcscasecmp(name, L"give_prog_fireflash"))
	 mobindex->mprogs->give_prog = give_prog_fireflash;
       else if (!wcscasecmp(name, L"give_prog_solamnia"))
	 mobindex->mprogs->give_prog = give_prog_solamnia;
       else if (!wcscasecmp(name, L"give_prog_keeper"))
 	 mobindex->mprogs->give_prog = give_prog_keeper;
       else if (!wcscasecmp(name, L"give_prog_dressmaker"))
	 mobindex->mprogs->give_prog = give_prog_dressmaker;
       else
	 {
	   bug( L"Load_mprogs: 'M': Function not found for vnum %d",
	       mobindex->vnum);
	   exit(1);
	 }

       SET_BIT(mobindex->progtypes, MPROG_GIVE);
       return;
     }
   bug( L"Load_mprogs: 'M': invalid program type for vnum %d",mobindex->vnum);
   exit(1);
 }

void bribe_prog_cityguard(CHAR_DATA *mob, CHAR_DATA *ch, int amount)
{
  if (amount < 100)
    {
      do_say(mob, (wchar_t*)"Cimri!!!");
      do_murder(mob, ch->name);
      return;
    }
  else if (amount >= 5000)
    {
      interpret(mob, (wchar_t*)"gülümse", FALSE);
      do_sleep(mob, (wchar_t*)"");
      return;
    }
  else
    {
      do_say(mob,(wchar_t*)"Bana rüşvet mi öneriyorsun? Pekala, ama bu sana pahalıya patlar!");
      return;
    }
}

void greet_prog_shalafi(CHAR_DATA *mob,CHAR_DATA *ch)
{
  if ( IS_NPC(ch) )
    return;

  mob->cabal = CABAL_SHALAFI;
  SET_BIT(mob->off_flags,OFF_AREA_ATTACK);

  if (ch->cabal == CABAL_SHALAFI) {
    do_say(mob,(wchar_t*)"Selamlar arif olan.");
    return;
  }
  if (ch->last_death_time != -1 && current_time - ch->last_death_time < 600)
    {
      do_say(mob,(wchar_t*)"Hayaletler buraya giremez.");
      do_slay(mob, ch->name);
      return;
    }

  if (IS_IMMORTAL(ch))	return;

  do_cb(mob, (wchar_t*)"Dikkat!!! Davetsiz Misafir!!!");
  do_say(mob,(wchar_t*)"Kabalımı rahatsız etmemeliydin!");
}

void greet_prog_invader(CHAR_DATA *mob,CHAR_DATA *ch)
{
  if ( IS_NPC(ch) )
    return;

  mob->cabal = CABAL_INVADER;
  SET_BIT(mob->off_flags,OFF_AREA_ATTACK);

  if (ch->cabal == CABAL_INVADER) {
    do_say(mob, (wchar_t*)"Selamlar karanlığın ta kendisi!");
    return;
  }
  if (ch->last_death_time != -1 && current_time - ch->last_death_time < 600)
    {
      do_say(mob, (wchar_t*)"Hayaletler buraya giremez.");
      do_slay(mob, ch->name);
      return;
    }

  if (IS_IMMORTAL(ch))	return;

  do_cb(mob, (wchar_t*)"Dikkat!!! Davetsiz Misafir!!!");
  if (!IS_NPC(ch))  do_say(mob,(wchar_t*)"Kabalımı rahatsız etmemeliydin!");
}

void greet_prog_ruler_pre(CHAR_DATA *mob,CHAR_DATA *ch)
{
  wchar_t buf[100];

  if ( IS_NPC(ch) )
    return;

  if (ch->cabal == CABAL_RULER) {
    swprintf( buf, MAX_STRING_LENGTH-1, L"eğil %s",ch->name);
    interpret(mob,buf, FALSE);
    return;
  }

  do_say( mob, (wchar_t*)"Daha fazla ilerlemesen iyi olur." );
  do_say( mob, (wchar_t*)"Bu bölge özel kullanıma aittir." );
  return;
}

void greet_prog_ruler(CHAR_DATA *mob,CHAR_DATA *ch)
{
  wchar_t buf[100];

  if ( IS_NPC(ch) )
    return;

  mob->cabal = CABAL_RULER;
  SET_BIT(mob->off_flags,OFF_AREA_ATTACK);

  if (ch->cabal == CABAL_RULER) {
    swprintf( buf, MAX_STRING_LENGTH-1, L"eğil %s",ch->name);
    interpret(mob,buf, FALSE);
    return;
  }
  if (ch->last_death_time != -1 && current_time - ch->last_death_time < 600)
    {
      do_say(mob,(wchar_t*)"Hayaletler buraya giremez.");
      do_slay(mob, ch->name);
      return;
    }

  if (IS_IMMORTAL(ch))	return;

  do_cb(mob,(wchar_t*) "Dikkat!!! Davetsiz Misafir!!!");
  do_say(mob,(wchar_t*) "Kabalımı rahatsız etmemeliydin!");
}

void greet_prog_chaos(CHAR_DATA *mob,CHAR_DATA *ch)
{
  if ( IS_NPC(ch) )
    return;

  mob->cabal = CABAL_CHAOS;
  SET_BIT(mob->off_flags,OFF_AREA_ATTACK);

  if (ch->cabal == CABAL_CHAOS) {
    do_say(mob, (wchar_t*)"Selamlar kaosun efendisi!");
    return;
  }
  if (ch->last_death_time != -1 && current_time - ch->last_death_time < 600)
    {
      do_say(mob,(wchar_t*)"Hayaletler buraya giremez.");
      do_slay(mob, ch->name);
      return;
    }

  if (IS_IMMORTAL(ch))	return;

  do_cb(mob, (wchar_t*)"Dikkat!!! Davetsiz Misafir!!!");
  do_say(mob,(wchar_t*) "Kabalımı rahatsız etmemeliydin!");
}

void greet_prog_battle(CHAR_DATA *mob, CHAR_DATA *ch)
{
  if ( IS_NPC(ch))
    return;

  mob->cabal = CABAL_BATTLE;
  SET_BIT(mob->off_flags,OFF_AREA_ATTACK);

  if (ch->cabal == CABAL_BATTLE) {
    do_say(mob, (wchar_t*)"Selamlar yüce savaşçı!");
    return;
  }
  if (ch->last_death_time != -1 && current_time - ch->last_death_time < 600)
    {
      do_say(mob,(wchar_t*)"Hayaletler buraya giremez.");
      do_slay(mob, ch->name);
      return;
    }

  if (IS_IMMORTAL(ch))	return;

  do_cb(mob, (wchar_t*)"Dikkat!!! Davetsiz Misafir!!!");
  do_say(mob, (wchar_t*)"Kabalımı rahatsız etmemeliydin!");
}


void give_prog_keeper(CHAR_DATA *mob, CHAR_DATA *ch, OBJ_DATA *obj)
{
  OBJ_DATA *rug;
  wchar_t buf[100];

  if (obj->pIndexData->vnum == 90)
    {
      do_say(mob,(wchar_t*) "İşte beklediğim giysi!");
      act( L"$n giysiyi masanın altına sokuşturuyor.",mob,NULL,NULL,TO_ROOM);
      obj_from_char(obj);
      extract_obj(obj);
      if ((rug = get_obj_carry(ch, (wchar_t*)"rug")) != NULL)
	{
    do_say(mob, (wchar_t*)"Sanırım FireFlash'i ziyaret etmek isteyeceksin.");
	  do_say(mob, (wchar_t*)"Dikkatli ol, canı biraz sıkkın.");
	  do_unlock (mob, (wchar_t*)"door");
	  do_open (mob, (wchar_t*)"door");
	}
      else
	{
    do_say(mob,(wchar_t*)"Sanırım FireFlash ile işin yok.");
    do_say(mob,(wchar_t*)"Sanırım şimdi buradan ayrılacak ve eksiklerini tamamlayacaksın.");
	}
    }
    else
    {
     swprintf( buf, MAX_STRING_LENGTH-1, L"%s %s",obj->name,ch->name);
     do_give(mob,buf);
     do_say(mob,(wchar_t*)"Neden buna ihtiyacım olsun?");
    }
 return;
}


void speech_prog_keeper(CHAR_DATA *mob, CHAR_DATA *ch, wchar_t *speech)
{
  OBJ_DATA *obj;

  if (!wcscasecmp(speech, L"keeper") && !IS_NPC(ch) )
    {
      obj = create_object(get_obj_index(90), 0);
      free_string(obj->name);
      obj->name = str_dup( L"keeper dress");
      act( L"$n ipekten beyaz bir giysi hazırlıyor.", mob, NULL,
	NULL, TO_ROOM);
      act( L"Keeper için beyaz bir giysi hazırlıyorsun.", mob, NULL, NULL, TO_CHAR);
      do_say(mob,(wchar_t*)"İşte Keeper için bir giysi.");
      obj_to_char(obj, ch);
    }
}

void greet_prog_fireflash(CHAR_DATA *mob, CHAR_DATA *ch)
{
  wchar_t buf[100];
  OBJ_DATA *obj;

  if (!can_see(mob,ch) || IS_NPC(ch) || IS_IMMORTAL(ch))
    return;

  if ((obj = get_obj_carry(ch,(wchar_t*)"rug")) == NULL)
    {
      do_say(mob,(wchar_t*)"Bu değersiz halıyı yakınlarımda görmek istemiyorum.");
    do_say(mob, (wchar_t*)"Neden onu Tear'dan aptal rahibeye vermiyorsun?");
      do_unlock(mob, (wchar_t*)"box");
      do_open(mob, (wchar_t*)"box");
      do_get(mob, (wchar_t*)"papers box");
      do_say(mob,(wchar_t*) "Bu kağıtların sana yardımı olabilir.");
      act( L"$n sana dudak büküyor.", mob, NULL, ch, TO_VICT);
      act( L"$E dudak büküyorsun.", mob, NULL, ch, TO_CHAR);
      act( L"$n $E dudak büküyor.", mob, NULL, ch, TO_NOTVICT);
      swprintf( buf, MAX_STRING_LENGTH-1, L"papers %s", ch->name);
      do_give(mob, buf);
      do_close(mob, (wchar_t*)"box");
      do_lock(mob, (wchar_t*)"box");
    }
}

void give_prog_fireflash(CHAR_DATA *mob, CHAR_DATA *ch, OBJ_DATA *obj)
{
  wchar_t buf[100];

  if (!can_see(mob,ch))
  do_say(mob,(wchar_t*)"Orada biri mi var?");
else if (IS_NPC(ch))
 do_say(mob,(wchar_t*) "Birşeyler getiren bir hayvan... Ne kadar garip!");

  else if (obj->pIndexData->vnum != 91)
    {
      do_say(mob,(wchar_t*)"Ne kadar ilginç!... Nedir bu?");
      interpret(mob, (wchar_t*)"kıkırda", FALSE);
      swprintf( buf, MAX_STRING_LENGTH-1, L"%s %s",obj->name,ch->name);
      do_give(mob,buf);
    }
  else
    {
      do_say(mob, (wchar_t*)"Ne kadar şahane bir halı! Acaba nereye koysam...");
      act( L"$n odayı dolanarak kendi kendine mırıldanıyor.", mob,
	NULL, NULL, TO_ROOM);
      act( L"$n ellerini cebine sokuyor.",mob,NULL,NULL,TO_ROOM);
      do_load(mob, (wchar_t*)"obj 2438");
      do_say(mob,(wchar_t*)"Nedir bu? Bir anahtar? Hmm, geri alabilirsin.");
      swprintf( buf, MAX_STRING_LENGTH-1, L"xxx %s",ch->name);
      do_give(mob, buf);
      act( L"$n dalgınlıkla halıyı sandalyenin altına itiyor.",mob,NULL,NULL,TO_ROOM);
      obj_from_char(obj);
      extract_obj(obj);
    }
   return;
}

void greet_prog_solamnia(CHAR_DATA *mob, CHAR_DATA *ch)
{

  OBJ_DATA *obj;
  wchar_t arg[100];

  if (!can_see(mob,ch) || IS_NPC(ch) || IS_IMMORTAL(ch))
    return;

  swprintf(arg,100-1, L"xxx");

  if ((obj = get_obj_carry(ch, arg)) != NULL)
    {
      do_say(mob,(wchar_t*) "Sanırım benim için birşey getirdin.");
      interpret(mob,(wchar_t*) "gülümse", FALSE);
    }
}

void give_prog_solamnia(CHAR_DATA *mob, CHAR_DATA *ch, OBJ_DATA *obj)

{
  wchar_t buf[100];
  OBJ_DATA *kassandra;

  if (obj->pIndexData->vnum == 2438 )
    {
      do_say(mob,(wchar_t*) "İşte ödülün!");
      kassandra = create_object(get_obj_index(89), 0);
      kassandra->timer = 500;
      obj_to_char(kassandra, mob);
      swprintf( buf, MAX_STRING_LENGTH-1, L"kasandra %s",ch->name);
      do_give(mob, buf);
      do_say(mob,(wchar_t*)"Bu taşın özel güçleri vardır, dikkatli kullan.");
      obj_from_char(obj);
      extract_obj(obj);
    }
}

bool death_prog_stalker(CHAR_DATA *mob)
{
  wchar_t buf[100];

  mob->cabal = CABAL_RULER;
  swprintf( buf, MAX_STRING_LENGTH-1, L"%syi öldürmeyi beceremedim, son nefesimi vermeliyimg.",
	  mob->last_fought->name);
  do_cb(mob, buf);
  return FALSE;
}

void greet_prog_knight(CHAR_DATA *mob, CHAR_DATA *ch)
{
  if ( IS_NPC(ch))
    return;

  mob->cabal = CABAL_KNIGHT;
  SET_BIT(mob->off_flags,OFF_AREA_ATTACK);

  if (ch->cabal == CABAL_KNIGHT) {
    do_say(mob,(wchar_t*)"Merhaba onurlu olan!");
    return;
  }
  if (ch->last_death_time != -1 && current_time - ch->last_death_time < 600)
    {
      do_say(mob, (wchar_t*)"Hayaletler buraya giremez.");
      do_slay(mob, ch->name);
      return;
    }

  if (IS_IMMORTAL(ch))	return;

  do_cb(mob, (wchar_t*)"Dikkat!!! Davetsiz Misafir!!!");
  do_say(mob, (wchar_t*)"Kabalımı rahatsız etmemeliydin!");
}

void give_prog_dressmaker(CHAR_DATA *mob, CHAR_DATA *ch, OBJ_DATA *obj)
{

  if (IS_NPC(ch))
    return;

  if (!can_see(mob, ch))
    {
      do_say(mob,(wchar_t*)"Bu nereden geldi?");
      return;
    }

  if (obj->pIndexData->vnum != 2436)
    {
      do_say(mob,(wchar_t*)"Bu işime yaramaz. Bana ipek lazım.");
      do_drop(mob, obj->name);
      return;
    }

  else
    {
      do_say(mob,(wchar_t*)"Bu elbiseyi kime yapıyorum?");
      obj_from_char(obj);
      extract_obj(obj);
    }
}

void greet_prog_keeper(CHAR_DATA *mob, CHAR_DATA *ch)
{
  if (IS_NPC(ch))
    return;

  if (!can_see(mob, ch))
    return;

    do_say(mob,(wchar_t*)"Burada ne işin var? O şey sipariş ettiğim giysi mi?");
}

void speech_prog_templeman(CHAR_DATA *mob, CHAR_DATA *ch, wchar_t *speech)
{
wchar_t buf[160];
int chosen = 0;

if ( !wcscasecmp( speech, L"din" )|| !wcscasecmp(speech, L"dinler") )
	mob->status = GIVE_HELP_RELIGION;
    else if (( chosen = lookup_religion_name( speech)) != 0 )
	mob->status = RELIG_CHOSEN;
    else  return;

   if ( mob->status == RELIG_CHOSEN )
   {
    if (( ch->religion > 0) && (ch->religion < MAX_RELIGION) )
	{
    swprintf( buf, MAX_STRING_LENGTH-1, L"Zaten %s yolundasın.",
		religion_table[ch->religion].name);
	 do_say(mob,buf);
	 return;
	}

    ch->religion = chosen;
    swprintf( buf, MAX_STRING_LENGTH-1, L"Bundan böyle sonsuza kadar %s yolundasın.",
		religion_table[ch->religion].name);
    do_say(mob,buf);
    return;
   }
   do_say(mob,(wchar_t*)"Hmmm... Eveeett.. Din.. Bununla gerçekten ilgileniyor musun?");
   do_say(mob,(wchar_t*)"Önce yardım almayı dene. 'yardım din' yaz.");
   do_say(mob,(wchar_t*)"Bildiğin gibi bu diyarda üç din vardır.");
   do_say(mob,(wchar_t*)"Aktular için Nama, Yansızlar için Bolloh ve Şer için Kamos.");
   do_say(mob,(wchar_t*)"Bana seçmek istediğin dinin efendisini söylemelisin.");
   do_say(mob,(wchar_t*)"Unutma ki dinini bir kez seçersin.");
   do_say(mob,(wchar_t*)"Dinini değiştirmek istersen bunun için kimi görevleri bitirmen gerekir.");
   return;
}

void greet_prog_templeman(CHAR_DATA *mob, CHAR_DATA *ch)
{

  wchar_t arg[100];

  if (!can_see(mob,ch) || IS_NPC(ch) || IS_IMMORTAL(ch))
    return;

    swprintf(arg,100-1, L"gülümse %s",ch->name);
  interpret(mob, arg, FALSE);
}


int lookup_religion_name (const wchar_t *name)
{
   int value;

   for ( value = 0; value < MAX_RELIGION ; value++)
   {
	if (towlower(name[0]) == towlower(religion_table[value].name[0])
	&&  !str_prefix( name,religion_table[value].name))
	    return value;
   }

   return 0;
}

void greet_prog_lions(CHAR_DATA *mob, CHAR_DATA *ch)
{
  if ( IS_NPC(ch))
    return;

  mob->cabal = CABAL_LIONS;
  SET_BIT(mob->off_flags,OFF_AREA_ATTACK);

  if (ch->cabal == CABAL_LIONS )
  {
    do_say(mob, (wchar_t*)"Hoşgeldiniz aslan efendiler.");
    return;
  }
  if (ch->last_death_time != -1 && current_time - ch->last_death_time < 600)
    {
      do_say(mob,(wchar_t*)"Hayaletler buraya giremez.");
      do_slay(mob, ch->name);
      return;
    }

  if (IS_IMMORTAL(ch))	return;

  do_cb(mob, (wchar_t*)"Dikkat!!! Davetsiz Misafir!!!");
  do_say(mob, (wchar_t*)"Kabalımı rahatsız etmemeliydin!");
}

void greet_prog_hunter_old(CHAR_DATA *mob, CHAR_DATA *ch)
{
  if ( IS_NPC(ch))
    return;

  mob->cabal = CABAL_HUNTER;
  SET_BIT(mob->off_flags,OFF_AREA_ATTACK);

  if (ch->cabal == CABAL_HUNTER )
  {
    do_say(mob, (wchar_t*)"Merhaba sevgili avcı.");
    return;
  }
  if (ch->last_death_time != -1 && current_time - ch->last_death_time < 600)
    {
      do_say(mob,(wchar_t*)"Hayaletler buraya giremez.");
      do_slay(mob, ch->name);
      return;
    }

  if (IS_IMMORTAL(ch))	return;

  do_cb(mob, (wchar_t*)"Dikkat!!! Davetsiz Misafir!!!");
  do_say(mob, (wchar_t*)"Kabalımı rahatsız etmemeliydin!");
}


void greet_prog_hunter(CHAR_DATA *mob, CHAR_DATA *ch)
{
  if ( IS_NPC(ch))
    return;

  mob->cabal = CABAL_HUNTER;
  SET_BIT(mob->off_flags,OFF_AREA_ATTACK);

  if (ch->cabal == CABAL_HUNTER )
  {
    wchar_t buf[MAX_STRING_LENGTH];
    OBJ_DATA *eyed;
    int i;

    do_say(mob,(wchar_t*)"Merhaba sevgili avcı.");
        if (IS_SET(ch->quest,QUEST_EYE)) return;

        SET_BIT(ch->quest,QUEST_EYE);

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

        swprintf( buf, MAX_STRING_LENGTH-1, eyed->pIndexData->extra_descr->description, ch->name );
        eyed->extra_descr = new_extra_descr();
        eyed->extra_descr->keyword =
		str_dup( eyed->pIndexData->extra_descr->keyword );
        eyed->extra_descr->description = str_dup( buf );
        eyed->extra_descr->next = NULL;

  	eyed->value[2] = (ch->level / 10) + 3;
  	eyed->level = ch->level;
	eyed->cost = 0;
	obj_to_char( eyed, mob);
  interpret( mob, (wchar_t*)"emote bir Avcı Kılıcı yaratıyor.", FALSE);
	do_say( mob ,(wchar_t*)"Sana avcının kılıcını veriyorum.");
	swprintf( buf, MAX_STRING_LENGTH-1, L"give eyed %s" , ch->name);
	interpret( mob , buf , FALSE);
	do_say( mob , (wchar_t*)"Şunu unutma ki, eğer onu kaybedersen kabal şifacısından yeni bir tane isteyebilirsin.");
	do_say( mob ,(wchar_t*)"Ona sadece 'felaket' de.");
    return;
  }
  if (ch->last_death_time != -1 && current_time - ch->last_death_time < 600)
    {
      do_say(mob, (wchar_t*)"Hayaletler buraya giremez.");
      do_slay(mob, ch->name);
      return;
    }

  if (IS_IMMORTAL(ch))	return;

  do_cb(mob, (wchar_t*)"Dikkat!!! Davetsiz Misafir!!!");
  do_say(mob,(wchar_t*)"Kabalımı rahatsız etmemeliydin!");
}


void fight_prog_diana( CHAR_DATA *mob, CHAR_DATA *ch )
{
   CHAR_DATA *ach, *ach_next;
   int door;

   if ( !mob->in_room || number_percent() < 25 ) return;
   if (mob->in_room->area != mob->zone) return;

   do_yell(mob,(wchar_t*)"Yardım edin nöbetçiler.");
   for( ach = char_list; ach != NULL; ach = ach_next )
   {
     ach_next = ach->next;
     if ( !ach->in_room
	 || ach->in_room->area != ch->in_room->area
	 || !IS_NPC(ach) )
		continue;
     if ( ach->pIndexData->vnum == 600
	|| ach->pIndexData->vnum == 603 )
	{
	 if (ach->fighting || ach->last_fought) continue;
	 if (mob->in_room == ach->in_room)
	  {
	   int i;

     act_color( L"$C$n tanrıları yardımına çağırıyor.$c",
  ach,NULL,NULL,TO_ROOM,POS_SLEEPING,CLR_BLUE);
     act_color( L"$CTanrılar $m Diana'ya yardım etmesi için geliştiriyor.$c",
		ach,NULL,NULL,TO_ROOM,POS_SLEEPING,CLR_GREEN);
  	   ach->max_hit = 6000;
  	   ach->hit = 6000;
  	   ach->level = 60;
  	   ach->timer = 0;
  	   ach->damage[DICE_NUMBER] = number_range(3,5);
  	   ach->damage[DICE_TYPE] = number_range(12,22);
  	   ach->damage[DICE_BONUS] = number_range(6, 8);
	   for(i=0;i<MAX_STATS;i++)
		ach->perm_stat[i] = 23;
    do_say(ach,(wchar_t*)"İşte geldim Diana.");
	   do_murder(ach,ch->name);
	   continue;
	  }
         door = find_path( ach->in_room->vnum, mob->in_room->vnum,
			ach,-40,1);
	 if (door == -1) bug( L"Couldn't find a path with -40",0);
	 else {
		if (number_percent() < 25)
    do_yell(ach,(wchar_t*)" Dayan Diana! Geliyorum!");
   else
    do_say(ach,(wchar_t*)"Gidip Dianaya yardım etmeliyim.");
		move_char(ach,door,FALSE);
	      }
	}
   }
}

void fight_prog_ofcol_guard( CHAR_DATA *mob, CHAR_DATA *ch )
{
   wchar_t buf[MAX_STRING_LENGTH];
   CHAR_DATA *ach, *ach_next;
   int door;

   if (number_percent() < 25) return;
   swprintf( buf, MAX_STRING_LENGTH-1, L"Yardım edin nöbetçiler! %s benimle dövüşüyor!",ch->name);
   do_yell(mob,buf);
   for( ach = char_list; ach != NULL; ach = ach_next )
   {
     ach_next = ach->next;
     if ( ach->in_room->area != ch->in_room->area || !IS_NPC(ach) ) continue;
     if ( ach->pIndexData->vnum == 600 )
	{
	 if (ach->fighting) continue;
	 if (mob->in_room == ach->in_room)
	  {
      swprintf( buf, MAX_STRING_LENGTH-1, L"Şimdi %s, muhafızlara saldırmanın cezasını çekeceksin.",ch->name);
	   do_say(ach,buf);
	   do_murder(ach,ch->name);
	   continue;
	  }
         door = find_path( ach->in_room->vnum, mob->in_room->vnum,
			ach,-40,1);
	 if (door == -1) bug( L"Couldn't find a path with -40",0);
	 else {
		if (number_percent() < 25)
    do_yell(ach,(wchar_t*)" Dayan nöbetçi! Geliyorum!");
   else
    do_say(ach,(wchar_t*)"Gidip muhafıza yardım etmeliyim.");
		move_char(ach,door,FALSE);
	      }
	}
   }
}

void speech_prog_wiseman(CHAR_DATA *mob, CHAR_DATA *ch, wchar_t *speech)
{
 wchar_t arg[512];

 one_argument(speech,arg);
 if (arg[0] == '\0')	return;
 if (!wcscasecmp(speech,L"bilge iyileştir şifa"))
	heal_battle(mob,ch);
 else return;
}

void greet_prog_armourer(CHAR_DATA *mob, CHAR_DATA *ch)
{
  wchar_t buf[MAX_STRING_LENGTH];

  if (!can_see(mob,ch) || IS_NPC(ch) || IS_IMMORTAL(ch))
    return;
  interpret(mob,(wchar_t*)"gülümse", FALSE);
  swprintf( buf, MAX_STRING_LENGTH-1, L"Zırhçıya hoşgeldin %s,",
   wcscasecmp(mob->in_room->area->name,hometown_table[ch->hometown].name) ?
   L"yolcu" : ch->name );
  do_say(mob,buf);
  do_say(mob,(wchar_t*)"Sana nasıl yardımcı olabilirim?");
  do_say(mob,(wchar_t*)"Dükkanımda gördüğün zırhların tümü çok kalitelidir.");
  interpret(mob,(wchar_t*)"emote gururla geriniyor.", FALSE);
}

void greet_prog_baker(CHAR_DATA *mob, CHAR_DATA *ch)
{
  wchar_t buf[MAX_STRING_LENGTH];

  if (!can_see(mob,ch) || IS_NPC(ch) || IS_IMMORTAL(ch))
    return;
  interpret(mob,(wchar_t*)"gülümse", FALSE);
  swprintf( buf, MAX_STRING_LENGTH-1, L"Fırına hoşgeldin %s.",
   wcscasecmp(mob->in_room->area->name,hometown_table[ch->hometown].name) ?
   L"yolcu" : ch->name );
  do_say(mob,buf);
}

void greet_prog_beggar(CHAR_DATA *mob, CHAR_DATA *ch)
{
  wchar_t buf[MAX_STRING_LENGTH];

  if (!can_see(mob,ch) || IS_NPC(ch) || IS_IMMORTAL(ch))
    return;
    swprintf( buf, MAX_STRING_LENGTH-1, L"Allah rızası için %s,",
   wcscasecmp(mob->in_room->area->name,hometown_table[ch->hometown].name) ?
   L"yolcu" : ch->name );
  do_say(mob,buf);
  do_say(mob,(wchar_t*)"Birkaç akçe ver...");
}

void greet_prog_drunk(CHAR_DATA *mob, CHAR_DATA *ch)
{
  if (!can_see(mob,ch) || IS_NPC(ch) || IS_IMMORTAL(ch))
    return;
  if (number_percent() < 5)
   {
     do_yell(mob,(wchar_t*)"Canavar! Bir canavar buldum! Saldırın!");
    do_murder(mob,ch->name);
   }
}

void greet_prog_grocer(CHAR_DATA *mob, CHAR_DATA *ch)
{
  wchar_t buf[MAX_STRING_LENGTH];

  if (!can_see(mob,ch) || IS_NPC(ch) || IS_IMMORTAL(ch))
    return;
    swprintf( buf, MAX_STRING_LENGTH-1, L"Dükkanıma hoşgeldin %s.",
   wcscasecmp(mob->in_room->area->name,hometown_table[ch->hometown].name) ?
   L"yolcu" : ch->name );
  do_say(mob,buf);
}


void bribe_prog_beggar(CHAR_DATA *mob, CHAR_DATA *ch, int amount)
{
  wchar_t buf[MAX_STRING_LENGTH];

  if (amount < 10)
    {
     swprintf( buf, MAX_STRING_LENGTH-1, L"teşekkür %s",
      wcscasecmp(mob->in_room->area->name,hometown_table[ch->hometown].name) ?
      L"yolcu" : ch->name );
     interpret(mob,buf, FALSE);
     return;
    }
  else if (amount < 100)
    {
      do_say(mob,(wchar_t*)"Ohaaa! Çok teşekkür ederim.");
     return;
    }
  else if (amount < 500)
    {
      do_say(mob,(wchar_t*)"Tanrıma şükürler olsun! Teşekkürler! Teşekkürler!");
      swprintf( buf, MAX_STRING_LENGTH-1, L"öp %s",ch->name);
     interpret(mob,buf, FALSE);
     return;
    }
  else
    {
     swprintf( buf, MAX_STRING_LENGTH-1, L"dans %s",ch->name);
     interpret(mob,buf, FALSE);
     swprintf( buf, MAX_STRING_LENGTH-1, L"öp %s",ch->name);
     interpret(mob,buf, FALSE);
     return;
    }
}


void bribe_prog_drunk(CHAR_DATA *mob, CHAR_DATA *ch, int amount)
{
  do_say(mob,(wchar_t*) "Ahh! Daha fazla ruh! Güzel Ruhlar!");
  interpret(mob,(wchar_t*)"şarkı", FALSE);
   return;
}



void fight_prog_beggar(CHAR_DATA *mob, CHAR_DATA *ch)
{
  if (mob->hit < (mob->max_hit * 0.45) && mob->hit > (mob->max_hit * 0.55))
  do_say(mob,(wchar_t*)"İşte ölüyorum...");
  return;
}


bool death_prog_beggar(CHAR_DATA *mob)
{
  if (number_percent() < 50 )
  do_say(mob,(wchar_t*)"Her nereyse gittiğim yer, biliyorum ki buradan iyidir...");
  else do_say(mob,(wchar_t*)"Günahlarımı affet tanrım...");
  return FALSE;
}

bool death_prog_vagabond(CHAR_DATA *mob)
{
  interpret(mob,(wchar_t*)"emote kafasını arkaya atıp deli gibi kahkaha atıyor!", FALSE);
  return FALSE;
}


void speech_prog_crier(CHAR_DATA *mob, CHAR_DATA *ch, wchar_t *speech)
{
 wchar_t arg[512];

 speech = one_argument(speech,arg);
 if (is_name(arg,(wchar_t*)"ne"))
	do_say(mob,(wchar_t*)"Sevgilim beni terketti.");
 return;
}


void area_prog_drunk(CHAR_DATA *mob)
{
  if (number_percent() < 5)
  interpret(mob, (wchar_t*)"dans", FALSE);
else if (number_percent() < 10)
  interpret(mob,(wchar_t*) "şarkı", FALSE);
  return;
}

void area_prog_janitor(CHAR_DATA *mob)
{
  if (number_percent() < 20)
   {
    interpret(mob,(wchar_t*)"grumble", FALSE);
    do_say(mob,(wchar_t*)"Çöpler");
    if (number_percent() < 20 )
     {
       do_say(mob,(wchar_t*)"Hergün yaptığım tek iş başkalarının pisliklerini temizlemek.");
      if (number_percent() < 20 )
      do_say(mob,(wchar_t*)"Yeterince kazanamıyorum.");
      else if (number_percent() < 20)
	   {
       do_say(mob,(wchar_t*)"Gün başlıyor, gün bitiyor. Hiç aralıksız çöp topluyorum.");
	    if ( number_percent() < 10 )
      do_yell(mob,(wchar_t*)"Tatil istiyorum!");
	   }
     }
   }
  return;
}

void area_prog_vagabond(CHAR_DATA *mob)
{
  if (number_percent() < 10)
  do_say(mob,(wchar_t*)"Kan! Zulüm!");
  return;
}

void area_prog_baker(CHAR_DATA *mob)
{
  if (number_percent() < 5)
  do_say(mob,(wchar_t*)"Kurabiyelerimin tadına bakmak ister misin?");
  return;
}

void area_prog_grocer(CHAR_DATA *mob)
{
  if (number_percent() < 5)
  do_say(mob, (wchar_t*)"Şu fenerin işçiliğine bakar mısın?");
  return;
}


void speech_prog_hunter_cleric(CHAR_DATA *mob, CHAR_DATA *ch, wchar_t *speech)
{
 wchar_t buf[MAX_STRING_LENGTH];
 OBJ_DATA *obj,*in_obj;
 int matched,i;

    if (wcscasecmp(speech,L"felaket"))	return;

    if (ch->cabal != CABAL_HUNTER)
    {
      do_say(mob,(wchar_t*)"Ikınırsan belki...");
     return;
    }

    if (!IS_SET(ch->quest,QUEST_EYE))
    {
      do_say(mob,(wchar_t*)"Ne kastediyorsun?");
     return;
    }

    matched = 0;
    for ( obj = object_list; obj != NULL; obj = obj->next )
    {
	if (obj->pIndexData->vnum != OBJ_VNUM_EYED_SWORD ||
		wcsstr(obj->short_descr,ch->name) == NULL)
	continue;

	matched = 1;
	for ( in_obj = obj; in_obj->in_obj != NULL; in_obj = in_obj->in_obj )
	    ;

	if ( in_obj->carried_by != NULL )
	{
	    if (in_obj->carried_by == ch)
	    {
        do_say(mob,(wchar_t*)"Benle dalga mı geçiyorsun? İşte kılıcın elinde...");
	     do_smite(mob,ch->name);
	     return;
	    }

      swprintf( buf, MAX_STRING_LENGTH-1, L"Kılıcını %s taşıyor!",
		PERS(in_obj->carried_by, ch) );
	    do_say(mob, buf);
	    if ( in_obj->carried_by->in_room )
	    {
        swprintf( buf, MAX_STRING_LENGTH-1, L"%s %s bölgesinde, %s civarlarında dolaşıyor!",
		PERS(in_obj->carried_by, ch),
		in_obj->carried_by->in_room->area->name,
		in_obj->carried_by->in_room->name );
	     do_say(mob, buf);
	     return;
	    }
	    else
	    {
	     extract_obj( obj );
       do_say( mob, (wchar_t*)"Sana yenisini vereceğim.");
	    }
	}
	else
	{
	    if (in_obj->in_room != NULL)
	    {
        swprintf( buf, MAX_STRING_LENGTH-1, L"Kılıcın %s bölgesinde, %s civarlarında!",
			in_obj->in_room->area->name, in_obj->in_room->name );
		do_say(mob,buf);
		return;
	    }
	    else
	    {
	     extract_obj( obj );
       do_say( mob,(wchar_t*)"Sana yenisini vereceğim.");
	    }
	}
     break;
    }

    if (!matched) do_say(mob,(wchar_t*)"Kılıcın kayıp!");

    if (IS_GOOD(ch)) 	i=0;
    else if (IS_EVIL(ch)) i=2;
    else i = 1;

    obj = create_object(get_obj_index(OBJ_VNUM_EYED_SWORD), 0);
    obj->owner = str_dup(ch->name);
    obj->from = str_dup(ch->name);
    obj->altar = hometown_table[ch->hometown].altar[i];
    obj->pit = hometown_table[ch->hometown].pit[i];
    obj->level = ch->level;

    swprintf( buf, MAX_STRING_LENGTH-1, obj->short_descr, ch->name );
    free_string( obj->short_descr );
    obj->short_descr = str_dup( buf );

    swprintf( buf, MAX_STRING_LENGTH-1, obj->pIndexData->extra_descr->description, ch->name );
    obj->extra_descr = new_extra_descr();
    obj->extra_descr->keyword =
		str_dup( obj->pIndexData->extra_descr->keyword );
    obj->extra_descr->description = str_dup( buf );
    obj->extra_descr->next = NULL;

    obj->value[2] = (ch->level / 10) + 3;
    obj->level = ch->level;
    obj->cost = 0;
    interpret( mob, (wchar_t*)"emote bir Avcı Kılıcı yaratıyor.", FALSE);
    do_say( mob ,(wchar_t*) "Sana başka bir Avcı Kılıcı veriyorum.");
    act( L"$N $e $p veriyor.", ch, obj, mob, TO_ROOM );
    act( L"$N sana $p veriyor.",   ch, obj, mob, TO_CHAR );
    obj_to_char(obj, ch);
    do_say( mob , (wchar_t*)"Tekrar kaybetme!");
}



void fight_prog_golem( CHAR_DATA *mob, CHAR_DATA *ch)
{
    CHAR_DATA *master;
    CHAR_DATA *m_next;
    wchar_t buf[MAX_INPUT_LENGTH];
    const wchar_t *spell;
    int sn;

    for ( master = mob->in_room->people; master != NULL; master = m_next )
    {
	m_next = master->next_in_room;
	if ( !IS_NPC(master) && mob->master == master &&
	     master->iclass == CLASS_NECROMANCER)
	    break;
    }

    if ( master == NULL )
	return;

    if ( !master->fighting )
	return;

    if ( master->fighting->fighting == master)
     {
      swprintf( buf, MAX_STRING_LENGTH-1, L"%s",master->name);
      do_rescue(mob,buf);
     }

    switch ( number_range(0,15) )
     {
	case  0: spell = L"curse";          break;
	case  1: spell = L"weaken";         break;
	case  2: spell = L"chill touch";    break;
	case  3: spell = L"blindness";      break;
	case  4: spell = L"poison";         break;
	case  5: spell = L"energy drain";   break;
	case  6: spell = L"harm";           break;
	case  7: spell = L"teleport";       break;
	case  8: spell = L"plague";	   break;
	default: spell = NULL;           break;
     }

    if ( spell == NULL || ( sn=skill_lookup(spell) ) < 0 )
	return;

    m_next = (mob->fighting) ? mob->fighting : master->fighting;
    if (m_next)
     {
      say_spell(mob,sn);
      (*skill_table[sn].spell_fun) (sn,mob->level,mob,m_next,TARGET_CHAR);
     }
    return;
}
