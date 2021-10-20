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

void one_hit(CHAR_DATA *ch, CHAR_DATA *victim, int dt, bool second);
bool cabal_area_check   args( (CHAR_DATA *ch) );

DECLARE_OPROG_FUN_WEAR( wear_prog_excalibur 	);
DECLARE_OPROG_FUN_REMOVE( remove_prog_excalibur );
DECLARE_OPROG_FUN_DEATH( death_prog_excalibur 	);
DECLARE_OPROG_FUN_SPEECH( speech_prog_excalibur );
DECLARE_OPROG_FUN_SAC( sac_prog_excalibur 	);

DECLARE_OPROG_FUN_SAC( sac_prog_cabal_item 	);
DECLARE_OPROG_FUN_GET( get_prog_cabal_item 	);

DECLARE_OPROG_FUN_FIGHT( fight_prog_sub_weapon  );
DECLARE_OPROG_FUN_SPEECH( speech_prog_kassandra );

DECLARE_OPROG_FUN_FIGHT( fight_prog_chaos_blade );
DECLARE_OPROG_FUN_DEATH( death_prog_chaos_blade );

DECLARE_OPROG_FUN_FIGHT( fight_prog_tattoo_kame );
DECLARE_OPROG_FUN_FIGHT( fight_prog_tattoo_nir );
DECLARE_OPROG_FUN_FIGHT( fight_prog_tattoo_nyah );
DECLARE_OPROG_FUN_FIGHT( fight_prog_tattoo_sint );

DECLARE_OPROG_FUN_FIGHT( fight_prog_golden_weapon );
DECLARE_OPROG_FUN_DEATH( death_prog_golden_weapon );

DECLARE_OPROG_FUN_GET( get_prog_heart 		);

DECLARE_OPROG_FUN_WEAR( wear_prog_bracer 	);
DECLARE_OPROG_FUN_REMOVE( remove_prog_bracer 	);

DECLARE_OPROG_FUN_WEAR(wear_prog_ranger_staff	);
DECLARE_OPROG_FUN_FIGHT( fight_prog_ranger_staff );
DECLARE_OPROG_FUN_DEATH( death_prog_ranger_staff );

DECLARE_OPROG_FUN_WEAR(wear_prog_coconut	);
DECLARE_OPROG_FUN_ENTRY(entry_prog_coconut	);
DECLARE_OPROG_FUN_GREET(greet_prog_coconut	);
DECLARE_OPROG_FUN_GET(get_prog_coconut		);
DECLARE_OPROG_FUN_REMOVE(remove_prog_coconut	);

DECLARE_OPROG_FUN_FIGHT(fight_prog_firegauntlets);
DECLARE_OPROG_FUN_WEAR(wear_prog_firegauntlets	);
DECLARE_OPROG_FUN_REMOVE(remove_prog_firegauntlets);
/* ibrahim armbands */
DECLARE_OPROG_FUN_FIGHT(fight_prog_armbands	);
DECLARE_OPROG_FUN_WEAR(wear_prog_armbands	);
DECLARE_OPROG_FUN_REMOVE(remove_prog_armbands	);

DECLARE_OPROG_FUN_FIGHT(fight_prog_demonfireshield);
DECLARE_OPROG_FUN_WEAR(wear_prog_demonfireshield);
DECLARE_OPROG_FUN_REMOVE(remove_prog_demonfireshield);

DECLARE_OPROG_FUN_FIGHT(fight_prog_vorbalblade	);
DECLARE_OPROG_FUN_GET(get_prog_spec_weapon	);
DECLARE_OPROG_FUN_FIGHT(fight_prog_shockwave	);
DECLARE_OPROG_FUN_FIGHT(fight_prog_snake 	);
/* new ones by chronos */
DECLARE_OPROG_FUN_WEAR( wear_prog_wind_boots 	);
DECLARE_OPROG_FUN_REMOVE( remove_prog_wind_boots);

DECLARE_OPROG_FUN_WEAR( wear_prog_arm_hercules );
DECLARE_OPROG_FUN_REMOVE( remove_prog_arm_hercules );

DECLARE_OPROG_FUN_WEAR( wear_prog_girdle_giant );
DECLARE_OPROG_FUN_REMOVE( remove_prog_girdle_giant );

DECLARE_OPROG_FUN_WEAR( wear_prog_breastplate_strength );
DECLARE_OPROG_FUN_REMOVE( remove_prog_breastplate_strength );

DECLARE_OPROG_FUN_WEAR( wear_prog_boots_flying );
DECLARE_OPROG_FUN_REMOVE( remove_prog_boots_flying );

DECLARE_OPROG_FUN_FIGHT( fight_prog_rose_shield );
DECLARE_OPROG_FUN_FIGHT( fight_prog_lion_claw );

DECLARE_OPROG_FUN_SPEECH( speech_prog_ring_ra );
DECLARE_OPROG_FUN_WEAR( wear_prog_eyed_sword );
DECLARE_OPROG_FUN_WEAR( wear_prog_katana_sword );

DECLARE_OPROG_FUN_WEAR( wear_prog_snake 	);
DECLARE_OPROG_FUN_REMOVE( remove_prog_snake 	);
DECLARE_OPROG_FUN_GET( get_prog_snake 		);

DECLARE_OPROG_FUN_WEAR( wear_prog_fire_shield 	);
DECLARE_OPROG_FUN_REMOVE( remove_prog_fire_shield );
DECLARE_OPROG_FUN_WEAR( wear_prog_quest_weapon );
DECLARE_OPROG_FUN_GET(get_prog_quest_reward	);

DECLARE_OPROG_FUN_FIGHT(fight_prog_ancient_gloves);
DECLARE_OPROG_FUN_WEAR(wear_prog_ancient_gloves	);
DECLARE_OPROG_FUN_REMOVE(remove_prog_ancient_gloves);

DECLARE_OPROG_FUN_FIGHT(fight_prog_ancient_shield);
DECLARE_OPROG_FUN_WEAR(wear_prog_ancient_shield	);
DECLARE_OPROG_FUN_REMOVE(remove_prog_ancient_shield);

DECLARE_OPROG_FUN_WEAR( wear_prog_neckguard );
DECLARE_OPROG_FUN_REMOVE( remove_prog_neckguard );

DECLARE_OPROG_FUN_WEAR( wear_prog_headguard );
DECLARE_OPROG_FUN_REMOVE( remove_prog_headguard );

DECLARE_OPROG_FUN_WEAR( wear_prog_blackguard );
DECLARE_OPROG_FUN_REMOVE( remove_prog_blackguard );


void    raw_kill        args( ( CHAR_DATA *victim ) );
void    raw_kill_org    args( ( CHAR_DATA *victim, int part ) );


void oprog_set(OBJ_INDEX_DATA *objindex,const wchar_t *progtype, const wchar_t *name)
{

   if (!wcscasecmp(progtype, L"wear_prog"))
     {
 /*
  *    if (!wcscasecmp(name, L"wear_prog_"))
  *          objindex->mprogs->wear_prog = wear_prog_;
  *    else if (!wcscasecmp(name, L"wear_prog_")) ...
  */

       if (!wcscasecmp(name, L"wear_prog_excalibur"))
	 objindex->oprogs->wear_prog = wear_prog_excalibur;
       else if (!wcscasecmp(name, L"wear_prog_bracer"))
	 objindex->oprogs->wear_prog = wear_prog_bracer;
       else if (!wcscasecmp(name, L"wear_prog_coconut"))
	 objindex->oprogs->wear_prog = wear_prog_coconut;
       else if (!wcscasecmp(name, L"wear_prog_firegauntlets"))
	 objindex->oprogs->wear_prog = wear_prog_firegauntlets;
      else if (!wcscasecmp(name, L"wear_prog_armbands"))
	 objindex->oprogs->wear_prog = wear_prog_armbands;
      else if (!wcscasecmp(name, L"wear_prog_demonfireshield"))
	 objindex->oprogs->wear_prog = wear_prog_demonfireshield;
       else if (!wcscasecmp(name, L"wear_prog_ranger_staff"))
	 objindex->oprogs->wear_prog = wear_prog_ranger_staff;
       else if (!wcscasecmp(name, L"wear_prog_wind_boots"))
	 objindex->oprogs->wear_prog = wear_prog_wind_boots;
       else if (!wcscasecmp(name, L"wear_prog_boots_flying"))
	 objindex->oprogs->wear_prog = wear_prog_boots_flying;
       else if (!wcscasecmp(name, L"wear_prog_arm_hercules"))
	 objindex->oprogs->wear_prog = wear_prog_arm_hercules;
       else if (!wcscasecmp(name, L"wear_prog_girdle_giant"))
	 objindex->oprogs->wear_prog = wear_prog_girdle_giant;
       else if (!wcscasecmp(name, L"wear_prog_breastplate_strength"))
	 objindex->oprogs->wear_prog = wear_prog_breastplate_strength;

       else if (!wcscasecmp(name, L"wear_prog_katana_sword"))
	 objindex->oprogs->wear_prog = wear_prog_katana_sword;
       else if (!wcscasecmp(name, L"wear_prog_eyed_sword"))
	 objindex->oprogs->wear_prog = wear_prog_eyed_sword;
       else if (!wcscasecmp(name, L"wear_prog_snake"))
	 objindex->oprogs->wear_prog = wear_prog_snake;
       else if (!wcscasecmp(name, L"wear_prog_fire_shield"))
	 objindex->oprogs->wear_prog = wear_prog_fire_shield;
       else if (!wcscasecmp(name, L"wear_prog_quest_weapon"))
	 objindex->oprogs->wear_prog = wear_prog_quest_weapon;
       else if (!wcscasecmp(name, L"wear_prog_ancient_gloves"))
	 objindex->oprogs->wear_prog = wear_prog_ancient_gloves;
       else if (!wcscasecmp(name, L"wear_prog_ancient_shield"))
	 objindex->oprogs->wear_prog = wear_prog_ancient_shield;
       else if (!wcscasecmp(name, L"wear_prog_neckguard"))
	 objindex->oprogs->wear_prog = wear_prog_neckguard;
       else if (!wcscasecmp(name, L"wear_prog_headguard"))
	 objindex->oprogs->wear_prog = wear_prog_headguard;
       else if (!wcscasecmp(name, L"wear_prog_blackguard"))
	 objindex->oprogs->wear_prog = wear_prog_blackguard;
       else
	 {
	   bug( L"Load_oprogs: 'O': Function not found for vnum %d",
	       objindex->vnum);
	   exit(1);
	 }

       SET_BIT(objindex->progtypes, OPROG_WEAR);
       return;
     }

   if (!wcscasecmp(progtype, L"remove_prog"))
     {
       if (!wcscasecmp(name, L"remove_prog_excalibur"))
	 objindex->oprogs->remove_prog = remove_prog_excalibur;
       else if (!wcscasecmp(name, L"remove_prog_bracer"))
	 objindex->oprogs->remove_prog = remove_prog_bracer;
       else if (!wcscasecmp(name, L"remove_prog_coconut"))
	 objindex->oprogs->remove_prog = remove_prog_coconut;
       else if (!wcscasecmp(name, L"remove_prog_firegauntlets"))
	 objindex->oprogs->remove_prog = remove_prog_firegauntlets;
       else if (!wcscasecmp(name, L"remove_prog_armbands"))
	 objindex->oprogs->remove_prog = remove_prog_armbands;
       else if (!wcscasecmp(name, L"remove_prog_demonfireshield"))
	 objindex->oprogs->remove_prog = remove_prog_demonfireshield;
       else if (!wcscasecmp(name, L"remove_prog_wind_boots"))
	 objindex->oprogs->remove_prog = remove_prog_wind_boots;
       else if (!wcscasecmp(name, L"remove_prog_boots_flying"))
	 objindex->oprogs->remove_prog = remove_prog_boots_flying;
       else if (!wcscasecmp(name, L"remove_prog_arm_hercules"))
	 objindex->oprogs->remove_prog = remove_prog_arm_hercules;
       else if (!wcscasecmp(name, L"remove_prog_girdle_giant"))
	 objindex->oprogs->remove_prog = remove_prog_girdle_giant;
       else if (!wcscasecmp(name, L"remove_prog_breastplate_strength"))
	 objindex->oprogs->remove_prog = remove_prog_breastplate_strength;
       else if (!wcscasecmp(name, L"remove_prog_snake"))
	 objindex->oprogs->remove_prog = remove_prog_snake;
       else if (!wcscasecmp(name, L"remove_prog_fire_shield"))
	 objindex->oprogs->remove_prog = remove_prog_fire_shield;
       else if (!wcscasecmp(name, L"remove_prog_ancient_gloves"))
	 objindex->oprogs->remove_prog = remove_prog_ancient_gloves;
       else if (!wcscasecmp(name, L"remove_prog_ancient_shield"))
	 objindex->oprogs->remove_prog = remove_prog_ancient_shield;
       else if (!wcscasecmp(name, L"remove_prog_neckguard"))
	 objindex->oprogs->remove_prog = remove_prog_neckguard;
       else if (!wcscasecmp(name, L"remove_prog_headguard"))
	 objindex->oprogs->remove_prog = remove_prog_headguard;
       else if (!wcscasecmp(name, L"remove_prog_blackguard"))
	 objindex->oprogs->remove_prog = remove_prog_blackguard;
       else
	 {
	   bug( L"Load_oprogs: 'O': Function not found for vnum %d",
	       objindex->vnum);
	   exit(1);
	 }
       SET_BIT(objindex->progtypes, OPROG_REMOVE);
       return;
     }
   if (!wcscasecmp(progtype, L"get_prog"))
     {
       if (!wcscasecmp(name, L"get_prog_cabal_item"))
	 objindex->oprogs->get_prog = get_prog_cabal_item;
       else if (!wcscasecmp(name, L"get_prog_heart"))
	 objindex->oprogs->get_prog = get_prog_heart;
       else if (!wcscasecmp(name, L"get_prog_coconut"))
	 objindex->oprogs->get_prog = get_prog_coconut;
       else if (!wcscasecmp(name, L"get_prog_spec_weapon"))
	 objindex->oprogs->get_prog = get_prog_spec_weapon;
       else if (!wcscasecmp(name, L"get_prog_snake"))
	 objindex->oprogs->get_prog = get_prog_snake;
       else if (!wcscasecmp(name, L"get_prog_quest_reward"))
	 objindex->oprogs->get_prog = get_prog_quest_reward;
       else
	 {
	   bug( L"Load_oprogs: 'O': Function not found for vnum %d",
	       objindex->vnum);
	   exit(1);
	 }
       SET_BIT(objindex->progtypes, OPROG_GET);
       return;
     }
   if (!wcscasecmp(progtype, L"drop_prog"))
     {
/*
       if (!wcscasecmp(name, L"drop_prog_"))
	 objindex->oprogs->drop_prog = drop_prog_;
       else
*/
	 {
	   bug( L"Load_oprogs: 'O': Function not found for vnum %d",
	       objindex->vnum);
	   exit(1);
	 }
       SET_BIT(objindex->progtypes, OPROG_DROP);
       return;
     }

   /*
    * returning TRUE prevents destruction
    */
   if (!wcscasecmp(progtype, L"sac_prog"))
     {
       if (!wcscasecmp(name, L"sac_prog_excalibur"))
	 objindex->oprogs->sac_prog = sac_prog_excalibur;
       else if (!wcscasecmp(name, L"sac_prog_cabal_item"))
	 objindex->oprogs->sac_prog = sac_prog_cabal_item;
       else
	 {
	   bug( L"Load_oprogs: 'O': Function not found for vnum %d",
	       objindex->vnum);
	   exit(1);
	 }
       SET_BIT(objindex->progtypes, OPROG_SAC);
       return;
     }
   if (!wcscasecmp(progtype, L"entry_prog"))
     {
       if (!wcscasecmp(name, L"entry_prog_coconut"))
         objindex->oprogs->entry_prog = entry_prog_coconut;
       else
         {
           bug( L"Load_oprogs: 'O': Function not found for vnum %d",
               objindex->vnum);
           exit(1);
         }
       SET_BIT(objindex->progtypes, OPROG_ENTRY);
       return;
     }
   if (!wcscasecmp(progtype, L"give_prog"))
     {
      /*
       if (!wcscasecmp(name, L"give_prog_"))
         objindex->oprogs->give_prog = give_prog_;
       else
       */
         {
           bug( L"Load_oprogs: 'O': Function not found for vnum %d",
               objindex->vnum);
           exit(1);
         }
       SET_BIT(objindex->progtypes, OPROG_GIVE);
       return;
     }
   if (!wcscasecmp(progtype, L"greet_prog"))
     {
       if (!wcscasecmp(name, L"greet_prog_coconut"))
         objindex->oprogs->greet_prog = greet_prog_coconut;
       else
         {
           bug( L"Load_oprogs: 'O': Function not found for vnum %d",
               objindex->vnum);
           exit(1);
         }
       SET_BIT(objindex->progtypes, OPROG_GREET);
       return;
     }
   if (!wcscasecmp(progtype, L"fight_prog"))
     {
       if (!wcscasecmp(name, L"fight_prog_ranger_staff"))
	 objindex->oprogs->fight_prog = fight_prog_ranger_staff;
       else if (!wcscasecmp(name, L"fight_prog_sub_weapon"))
	 objindex->oprogs->fight_prog = fight_prog_sub_weapon;
       else if (!wcscasecmp(name, L"fight_prog_chaos_blade"))
	 objindex->oprogs->fight_prog = fight_prog_chaos_blade;
       else if (!wcscasecmp(name, L"fight_prog_tattoo_kame"))
	 objindex->oprogs->fight_prog = fight_prog_tattoo_kame;
       else if (!wcscasecmp(name, L"fight_prog_tattoo_nir"))
	 objindex->oprogs->fight_prog = fight_prog_tattoo_nir;
       else if (!wcscasecmp(name, L"fight_prog_tattoo_nyah"))
	 objindex->oprogs->fight_prog = fight_prog_tattoo_nyah;
       else if (!wcscasecmp(name, L"fight_prog_tattoo_sint"))
   objindex->oprogs->fight_prog = fight_prog_tattoo_sint;
       else if (!wcscasecmp(name, L"fight_prog_golden_weapon"))
	 objindex->oprogs->fight_prog = fight_prog_golden_weapon;
       else if (!wcscasecmp(name, L"fight_prog_snake"))
	 objindex->oprogs->fight_prog = fight_prog_snake;
       else if (!wcscasecmp(name, L"fight_prog_shockwave"))
	 objindex->oprogs->fight_prog = fight_prog_shockwave;
       else if (!wcscasecmp(name, L"fight_prog_firegauntlets"))
	 objindex->oprogs->fight_prog = fight_prog_firegauntlets;
       else if (!wcscasecmp(name, L"fight_prog_armbands"))
	 objindex->oprogs->fight_prog = fight_prog_armbands;
       else if (!wcscasecmp(name, L"fight_prog_demonfireshield"))
	 objindex->oprogs->fight_prog = fight_prog_demonfireshield;
       else if (!wcscasecmp(name, L"fight_prog_vorbalblade"))
	 objindex->oprogs->fight_prog = fight_prog_vorbalblade;
       else if (!wcscasecmp(name, L"fight_prog_rose_shield"))
	 objindex->oprogs->fight_prog = fight_prog_rose_shield;
       else if (!wcscasecmp(name, L"fight_prog_lion_claw"))
	 objindex->oprogs->fight_prog = fight_prog_lion_claw;
       else if (!wcscasecmp(name, L"fight_prog_ancient_gloves"))
	 objindex->oprogs->fight_prog = fight_prog_ancient_gloves;
       else if (!wcscasecmp(name, L"fight_prog_ancient_shield"))
	 objindex->oprogs->fight_prog = fight_prog_ancient_shield;
       else
	 {
	   bug( L"Load_oprogs: 'O': Function not found for vnum %d",
	       objindex->vnum);
	   exit(1);
	 }
       SET_BIT(objindex->progtypes, OPROG_FIGHT);
       return;
     }
   if (!wcscasecmp(progtype, L"death_prog")) /* returning TRUE prevents death */
     {
       if (!wcscasecmp(name, L"death_prog_excalibur"))
	 objindex->oprogs->death_prog = death_prog_excalibur;
       else if (!wcscasecmp(name, L"death_prog_ranger_staff"))
	 objindex->oprogs->death_prog = death_prog_ranger_staff;
       else if (!wcscasecmp(name, L"death_prog_chaos_blade"))
	 objindex->oprogs->death_prog = death_prog_chaos_blade;
       else if (!wcscasecmp(name, L"death_prog_golden_weapon"))
	 objindex->oprogs->death_prog = death_prog_golden_weapon;
       else
	 {
	   bug( L"Load_oprogs: 'O': Function not found for vnum %d",
	       objindex->vnum);
	   exit(1);
	 }
       SET_BIT(objindex->progtypes, OPROG_DEATH);
       return;
     }
   if (!wcscasecmp(progtype, L"speech_prog"))
     {
       if (!wcscasecmp(name, L"speech_prog_excalibur"))
	 objindex->oprogs->speech_prog = speech_prog_excalibur;
       else if(!wcscasecmp(name, L"speech_prog_kassandra"))
	 objindex->oprogs->speech_prog = speech_prog_kassandra;
       else if(!wcscasecmp(name, L"speech_prog_ring_ra"))
	 objindex->oprogs->speech_prog = speech_prog_ring_ra;
       else
	 {
	   bug( L"Load_oprogs: 'O': Function not found for vnum %d",
	       objindex->vnum);
	   exit(1);
	 }
       SET_BIT(objindex->progtypes, OPROG_SPEECH);
       return;
     }

   if (!wcscasecmp(progtype, L"area_prog"))
     {
/*
       if (!wcscasecmp(name, L"area_prog_"))
	 objindex->oprogs->area_prog = area_prog_;
       else
*/
	 {
	   bug( L"Load_oprogs: 'O': Function not found for vnum %d",
	       objindex->vnum);
	   exit(1);
	 }
       SET_BIT(objindex->progtypes, OPROG_AREA);
       return;
     }


   bug( L"Load_oprogs: 'O': invalid program type for vnum %d",objindex->vnum);
   exit(1);
 }



void wear_prog_excalibur(OBJ_DATA *obj, CHAR_DATA *ch)
{
  act( L"$p parlak beyaz renkte parlamaya başladı.",ch,obj,NULL,TO_CHAR);
  act( L"$p parlak beyaz renkte parlamaya başladı.",ch,obj,NULL,TO_ROOM);
  if ( ch->level > 20 && ch->level <= 30)	obj->value[2] = 4;
  else if ( ch->level > 30 && ch->level <= 40)   obj->value[2] = 5;
  else if ( ch->level > 40 && ch->level <= 50)   obj->value[2] = 6;
  else if ( ch->level > 50 && ch->level <= 60)   obj->value[2] = 7;
  else if ( ch->level > 60 && ch->level <= 70)   obj->value[2] = 9;
  else if ( ch->level > 70 && ch->level <= 80)   obj->value[2] = 11;
  else obj->value[2] = 12;
  return;
}

void wear_prog_bracer(OBJ_DATA *obj, CHAR_DATA *ch)
{
  AFFECT_DATA af;

  if (!is_affected(ch, gsn_haste))
    {
      send_to_char( L"Taktığın bileklikler seninle bütünleşiyor.\n\r", ch);
      send_to_char( L"Ellerin ve kolların inanılmaz derecede hafifledi.\n\r", ch);

      af.where = TO_AFFECTS;
      af.type = gsn_haste;
      af.duration = -2;
      af.level = ch->level;
      af.bitvector = AFF_HASTE;
      af.location = APPLY_DEX;
      af.modifier = 1 + (ch->level >= 18) + (ch->level >= 30) + (ch->level >= 45);
      affect_to_char(ch, &af);
    }
}

void remove_prog_bracer(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (is_affected(ch, gsn_haste))
    {
      affect_strip(ch, gsn_haste);
      send_to_char( L"Ellerin ve kolların ağırlaştı.\n\r", ch);
    }
}


void remove_prog_excalibur(OBJ_DATA *obj, CHAR_DATA *ch)
{
  act( L"$p artık parlamıyor.",ch,obj,NULL,TO_CHAR);
  act( L"$p artık parlamıyor.",ch,obj,NULL,TO_ROOM);
}

bool death_prog_excalibur(OBJ_DATA *obj, CHAR_DATA *ch)
{
  act_new( L"$p mavi bir ayla ile parlıyor.",ch,obj,NULL,TO_CHAR,POS_DEAD);
  act( L"$p mavi bir ayla ile parlıyor,",ch,obj,NULL,TO_ROOM);
  ch->hit = ch->max_hit;
  send_to_char( L"Kendini daha iyi hiseediyorsun.",ch);
  act( L"$n daha iyi görünüyor.",ch,NULL,NULL,TO_ROOM);
  return TRUE;
}

void speech_prog_excalibur(OBJ_DATA *obj, CHAR_DATA *ch, wchar_t *speech)
{

  if (!wcscasecmp(speech, L"asit")
      && (ch->fighting) && is_wielded_char(ch,obj)  )
    {
      send_to_char( L"Excalibur'un bıçağından asit fışkırıyor.\n\r",ch);
      act( L"Excalibur'un bıçağından asit fışkırıyor.",ch,NULL,NULL,TO_ROOM);
      obj_cast_spell(gsn_acid_blast,ch->level,ch,ch->fighting,NULL);
      WAIT_STATE(ch, 2 * PULSE_VIOLENCE);
    }
}

bool sac_prog_excalibur(OBJ_DATA *obj, CHAR_DATA *ch)
{
  act( L"Ölümsüzler çileden çıktı!",ch,NULL,NULL,TO_CHAR);
  act( L"Ölümsüzler çileden çıktı!",ch,NULL,NULL,TO_ROOM);
  damage(ch,ch,
	 (ch->hit - 1) > 1000? 1000 : (ch->hit - 1),
	 TYPE_HIT,DAM_HOLY, TRUE);
  ch->silver = 0;
  return TRUE;
}

void fight_prog_ranger_staff(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if ( is_wielded_char(ch,obj) && number_percent() < 10)
    {
      send_to_char( L"Korucu asan mavi renkte parlamaya başlıyor!\n\r",ch);
      act( L"$s korucu asası mavi renkte parlamaya başlıyor!",ch,NULL,NULL,TO_ROOM);

      obj_cast_spell(gsn_cure_critical,ch->level,ch,ch,obj);
    }
}

void fight_prog_sub_weapon(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (is_wielded_char(ch,obj) && number_percent() < 30)
    {
      if ( ((float) ch->hit)/((float) ch->max_hit) > 0.9)
      printf_to_char(ch,L"Silahın fısıldıyor '{yİyi iş çıkarıyorsun!{x'.\n\r");
          else if ( ((float) ch->hit)/((float) ch->max_hit) > 0.6)
    	printf_to_char(ch,L"Silahın fısıldıyor '{yBöyle devam et!{x'.\n\r");
          else if ( ((float) ch->hit)/((float) ch->max_hit) > 0.4)
    	  printf_to_char(ch,L"Silahın fısıldıyor '{yBaşarabilirsin!{x'.\n\r");
          else printf_to_char(ch,L"Silahın fısıldıyor '{yKaç! Kaç!{x'.\n\r");
    }
}

bool death_prog_ranger_staff(OBJ_DATA *obj, CHAR_DATA *ch)
{
  send_to_char( L"Korucu asan yokoluyor.\n\r",ch);
  act( L"$s korucu asası yokoluyor.",ch,NULL,NULL,TO_ROOM);
  extract_obj(obj);
  return FALSE;
}


void get_prog_spec_weapon(OBJ_DATA *obj, CHAR_DATA *ch)
{

  if ( wcsstr( obj->extra_descr->description, ch->name ) != NULL )
  {
    if ( IS_AFFECTED( ch, AFF_POISON ) && (dice(1,5)==1) )  {
      send_to_char( L"Silahın mavi renkte parlıyor.", ch );
      act( L"$s silahı mavi renkte parlıyor.", ch, NULL, NULL, TO_ROOM );
      spell_cure_poison( gsn_cure_poison, 30, ch, ch, TARGET_CHAR );
      return;
    }
    if ( IS_AFFECTED( ch, AFF_CURSE ) && (dice(1,5)==1) )  {
      send_to_char( L"Silahın mavi renkte parlıyor.", ch );
      act( L"$s silahı mavi renkte parlıyor.", ch, NULL, NULL, TO_ROOM );
      spell_remove_curse( gsn_remove_curse, 30, ch, ch, TARGET_CHAR );
      return;
    }
    send_to_char( L"Silahın daha güçlü vızıldamaya başladı.\n\r", ch );
    return;
  }
  act( L"Sen $p tarafından çarpıldın ve onu düşürdün.", ch, obj, NULL, TO_CHAR );

  obj_from_char( obj );
  obj_to_room( obj, ch->in_room );

  switch( dice(1, 10) )  {
  case 1:
    spell_curse( gsn_curse, ch->level < 10? 1 : ch->level-9, ch, ch, TARGET_CHAR );
    break;
  case 2:
    spell_poison( gsn_poison, ch->level < 10? 1 : ch->level-9, ch, ch, TARGET_CHAR );
    break;
  }
  return;

}

void get_prog_quest_reward(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if ( wcsstr( obj->short_descr, ch->name ) != NULL )
  {
    act_color( L"$C$p parlamaya başladı.\n\r$c",
		ch,obj,NULL,TO_CHAR,POS_SLEEPING,CLR_BLUE);
    return;
  }

  act( L"Sen $p tarafından çarpıldın ve onu düşürdün.", ch, obj, NULL, TO_CHAR );

  obj_from_char( obj );
  obj_to_room( obj, ch->in_room );
  return;
}

void get_prog_cabal_item(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (IS_NPC(ch))
  {
    act( L"Taşımayı haketmediğin $p elinden düşüyor.",
 ch, obj, NULL, TO_CHAR);
    act( L"$s taşımayı haketmediği $p elinden düşüyor.",
 ch, obj, NULL, TO_ROOM);
      obj_from_char(obj);
      obj_to_room(obj, ch->in_room);
      return;
  }

  if (obj->timer < 1)
  {
    obj->timer = 30;
    act( L"$p saydamlaştı.", ch, obj, NULL, TO_CHAR);
    act( L"$p saydamlaştı.", ch, obj, NULL, TO_ROOM);
  }
}

bool sac_prog_cabal_item(OBJ_DATA *obj, CHAR_DATA *ch)
{
  OBJ_DATA *container;
  wchar_t buf[160];
  int i;

  act( L"Ölümsüzler çileden çıktı!",ch,NULL,NULL,TO_CHAR);
  act( L"Ölümsüzler çileden çıktı!",ch,NULL,NULL,TO_ROOM);
  damage(ch,ch,(int)(ch->hit/10),TYPE_HIT,DAM_HOLY, TRUE);
  ch->silver = 0;

  obj_from_room(obj);
  for(i=0;i<MAX_CABAL;i++)
    if (cabal_table[i].obj_ptr == obj) break;

  if ( i < MAX_CABAL )
  {
    if ( obj->pIndexData->vnum == cabal_table[CABAL_RULER].obj_vnum )
      container = create_object( get_obj_index( OBJ_VNUM_RULER_STAND ),100 );
    else if ( obj->pIndexData->vnum == cabal_table[CABAL_INVADER].obj_vnum)
      container = create_object( get_obj_index( OBJ_VNUM_INVADER_SKULL ),100 );
    else if ( obj->pIndexData->vnum == cabal_table[CABAL_BATTLE].obj_vnum )
      container = create_object( get_obj_index( OBJ_VNUM_BATTLE_THRONE ),100 );
    else if ( obj->pIndexData->vnum == cabal_table[CABAL_KNIGHT].obj_vnum )
      container = create_object( get_obj_index( OBJ_VNUM_KNIGHT_ALTAR ), 100 );
    else if ( obj->pIndexData->vnum == cabal_table[CABAL_CHAOS].obj_vnum )
      container = create_object( get_obj_index( OBJ_VNUM_CHAOS_ALTAR ), 100 );
    else if ( obj->pIndexData->vnum == cabal_table[CABAL_LIONS].obj_vnum )
      container = create_object( get_obj_index( OBJ_VNUM_LIONS_ALTAR ), 100 );
    else if ( obj->pIndexData->vnum == cabal_table[CABAL_HUNTER].obj_vnum )
      container = create_object( get_obj_index( OBJ_VNUM_HUNTER_ALTAR ), 100 );
    else
      container = create_object( get_obj_index( OBJ_VNUM_SHALAFI_ALTAR),100 );

    obj_to_obj( obj, container );
    obj_to_room( container, get_room_index(cabal_table[i].room_vnum) );
    swprintf( buf, MAX_STRING_LENGTH-1, L"%s yavaşça tekrar oluşuyor.\n\r",
	container->short_descr );
    if ( get_room_index(cabal_table[i].room_vnum)->people != NULL )
    {
	act( buf, get_room_index(cabal_table[i].room_vnum)->people,NULL,NULL, TO_CHAR);
	act( buf, get_room_index(cabal_table[i].room_vnum)->people,NULL,NULL, TO_ROOM);
    }
    return TRUE;
  }
  else
  {
    extract_obj(obj);
    bug( L"oprog: Sac_cabal_item: Was not the cabal's item.", 0);
  }

  return FALSE;
}

void speech_prog_kassandra(OBJ_DATA *obj, CHAR_DATA *ch, wchar_t *speech)
{
  if (!wcscasecmp(speech, L"kassandra") && (get_hold_char(ch) == obj)
           && !IS_NPC(ch))
    obj_cast_spell(gsn_kassandra,ch->level,ch,ch,NULL);

  else if (!wcscasecmp(speech, L"sebat") && (get_hold_char(ch) == obj)
           && !IS_NPC(ch))
    obj_cast_spell(gsn_sebat,ch->level,ch,ch,NULL);

  else if (!wcscasecmp(speech, L"matandra") && (get_hold_char(ch) == obj)
	   && (ch->fighting) && !IS_NPC(ch))
    {
      act( L"Bir enerji topu elinden $E fırlıyor!",
	  ch,NULL,ch->fighting,TO_CHAR);
      act( L"Bir enerji yopu $s elinden sana fırlıyor!",
	  ch,NULL,ch->fighting,TO_VICT);
      act( L"Bir enerji topu $s elinden $E fırlıyor!",
	  ch,NULL,ch->fighting,TO_NOTVICT);
      obj_cast_spell(gsn_matandra,ch->level,ch,ch->fighting,NULL);
    }
}

void fight_prog_chaos_blade(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if ( is_wielded_char(ch,obj) )
    switch(number_range(0,127)) {
    case 0:

    act( L"Yatağan titriyor!", ch, NULL, NULL, TO_ROOM);
    send_to_char( L"Yatağanın titriyor!\n\r", ch);
      obj_cast_spell(gsn_mirror,ch->level,ch,ch,obj);
      WAIT_STATE(ch, 2 * PULSE_VIOLENCE);
      break;

    case 1:

    act( L"Yatağan biraz sallanıyor.", ch, NULL, NULL, TO_ROOM);
    send_to_char( L"Yatağanın biraz sallanıyor.\n\r", ch);
      obj_cast_spell(gsn_garble,ch->level,ch,ch->fighting,obj);
      WAIT_STATE(ch, 2 * PULSE_VIOLENCE);
      break;

    case 2:

    act( L"Yatağan şiddetle titriyor!",ch,NULL,NULL,TO_ROOM);
    send_to_char( L"Yatağanın şiddetle titriyor!\n\r",ch);
      obj_cast_spell(gsn_confuse, ch->level,ch,ch->fighting,obj);
      WAIT_STATE(ch, 2 * PULSE_VIOLENCE);
      break;

    }
}

bool death_prog_chaos_blade(OBJ_DATA *obj, CHAR_DATA *ch)
{
  send_to_char( L"Yatağanın yokoluyor.\n\r",ch);
  act( L"$s yatağanı yokoluyor.",ch,NULL,NULL,TO_ROOM);
  extract_obj(obj);
  return FALSE;
}

void fight_prog_tattoo_kame(OBJ_DATA *obj, CHAR_DATA *ch)
{
  int sn;

  if (get_eq_char(ch, WEAR_TATTOO) == obj)
    switch(number_range(0,63)) {
    case 0:
    case 1:
    act_color( L"$COmzundaki dövme mavi renkte parlıyor.$c",
		   ch,NULL,NULL,TO_CHAR,POS_DEAD,CLR_CYAN);
      obj_cast_spell(gsn_cure_serious, ch->level, ch, ch, obj);
      break;
    case 2:
    act_color( L"$COmzundaki dövme kızıl renkte parlıyor.$c",
		   ch,NULL,NULL,TO_CHAR,POS_DEAD,CLR_RED);
       do_yell( ch, (wchar_t*)"Kutsal güçle dans etme....");
      sn = skill_lookup( L"holy word");
      spell_holy_word(sn,ch->level,ch,NULL,TARGET_CHAR);
      break;
    }
}


void fight_prog_tattoo_nir(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (get_eq_char(ch, WEAR_TATTOO) == obj)
    switch(number_range(0,63)) {
    case 0:
    case 1:
    case 2:
      act_color( L"$COmzundaki dövme mavi renkte parlıyor.$c",
		   ch,NULL,NULL,TO_CHAR,POS_DEAD,CLR_CYAN);
      obj_cast_spell(gsn_cure_critical, ch->level, ch, ch, obj);
      break;
    case 3:
      act_color( L"$COmzundaki dövme mavi renkte parlıyor.$c",
		   ch,NULL,NULL,TO_CHAR,POS_DEAD,CLR_CYAN);
      if (IS_AFFECTED(ch,AFF_PLAGUE))
	spell_cure_disease(25,100,ch,ch,TARGET_CHAR);
      if (IS_AFFECTED(ch,AFF_POISON))
        spell_cure_poison(27,100,ch,ch,TARGET_CHAR);
      break;
    }
}

void fight_prog_tattoo_nyah(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (get_eq_char(ch, WEAR_TATTOO) == obj)
    switch(number_range(0,63)) {
    case 0:
      act_color( L"$COmzundaki dövme mavi renkte parlıyor.$c",
		   ch,NULL,NULL,TO_CHAR,POS_DEAD,CLR_CYAN);
      obj_cast_spell(gsn_cure_serious, ch->level, ch,ch, obj);
      break;
    case 1:
      act_color( L"$COmzundaki dövme kızıl renkte parlıyor.$c",
		   ch,NULL,NULL,TO_CHAR,POS_DEAD,CLR_RED);
      spell_bluefire(gsn_dispel_good, ch->level, ch, ch->fighting, TARGET_CHAR);
      break;
    }
}

void fight_prog_tattoo_sint(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (get_eq_char(ch, WEAR_TATTOO) == obj)
    switch(number_range(0,63)) {
    case 0:
      act_color( L"$COmzundaki dövme mavi renkte parlıyor.$c",
		   ch,NULL,NULL,TO_CHAR,POS_DEAD,CLR_BLUE);
      obj_cast_spell(gsn_cure_serious, ch->level, ch, ch, obj);
      break;
    case 1:
      act_color( L"$COmzundaki dövme kızıl renkte parlıyor.$c",
		   ch,NULL,NULL,TO_CHAR,POS_DEAD,CLR_RED);
      obj_cast_spell(gsn_demonfire, ch->level, ch, ch->fighting, obj);
      break;
    }
}

bool death_prog_golden_weapon(OBJ_DATA *obj, CHAR_DATA *ch)
{
  send_to_char( L"Altın silahın yokoluyor.\n\r",ch);
  act( L"$s altın silahı yok oluyor.",ch,NULL,NULL,TO_ROOM);
  extract_obj(obj);
  ch->hit = 1;
  while ( ch->affected )
    affect_remove( ch, ch->affected );
  ch->last_fight_time = -1;
  ch->last_death_time = current_time;
  if (cabal_area_check(ch))
  {
    act( L"$n yokoluyor.",ch,NULL,NULL,TO_ROOM);
    char_from_room(ch);
    char_to_room(ch, get_room_index( cabal_table[CABAL_KNIGHT].room_vnum ));
    act( L"$n odada beliriyor.",ch,NULL,NULL,TO_ROOM);
  }
  return TRUE;
}

void fight_prog_golden_weapon(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if ( is_wielded_char(ch,obj) )
    {
      if (number_percent() < 4)
	{
	  act( L"$p parlak mavi renkte parlıyor!\n\r",ch, obj, NULL, TO_CHAR);
	  act( L"$s $p parlak mavi renkte parlıyor!",ch,obj,NULL,TO_ROOM);

	  obj_cast_spell(gsn_cure_critical,ch->level,ch,ch,obj);
	  return;
	}
      else if (number_percent() > 92)
	{
	  act( L"$p parlak mavi renkte parlıyor!\n\r",ch, obj, NULL, TO_CHAR);
	  act( L"$s $p parlak mavi renkte parlıyor!",ch,obj,NULL,TO_ROOM);

	  obj_cast_spell(gsn_cure_serious,ch->level,ch,ch,obj);
	  return;
	}
    }
}
void get_prog_heart(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (obj->timer == 0)
    obj->timer = 24;
}

void fight_prog_snake(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if ( is_wielded_char(ch,obj) )
    {
      switch(number_range(0,127)) {
      case 0:
      act( L"Kırbacındaki yılanlardan biri $M ısırıyor!", ch, NULL,
  		ch->fighting, TO_CHAR);
  	act( L"$s kırbacındaki yılanlardan biri seni ısırıyor!", ch, NULL,
  		ch->fighting, TO_VICT);
  	act( L"$s kırbacındaki yılanlardan biri $E saldırıyor!", ch, NULL,
		ch->fighting, TO_NOTVICT);
	obj_cast_spell(gsn_poison, ch->level, ch, ch->fighting, obj);
	break;
      case 1:
      act( L"Kırbacındaki yılanlardan biri $M ısırıyor!", ch, NULL,
    		ch->fighting, TO_CHAR);
    	act( L"$s kırbacındaki yılanlardan biri seni ısırıyor!", ch, NULL,
    		ch->fighting, TO_VICT);
    	act( L"$s kırbacındaki yılanlardan biri $E saldırıyor!", ch, NULL,
		ch->fighting, TO_NOTVICT);
	obj_cast_spell(gsn_weaken, ch->level, ch, ch->fighting, obj);
	break;
      }
    }
}


void fight_prog_shockwave(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if ( is_wielded_char(ch,obj) )
    switch(number_range(0,31)) {
    case 0:
    act( L"Silahından fırlayan bir yıldırım $M vuruyor!", ch,
NULL, ch->fighting, TO_CHAR);
    act( L"$s silahından fırlayan bir yıldırım sana atlıyor!",
ch, NULL, ch->fighting, TO_VICT);
    act( L"$s silahından fırlayan bir yıldırım $E atlıyor!",
	ch, NULL, ch->fighting, TO_NOTVICT);
      obj_cast_spell(gsn_lightning_bolt, ch->level, ch, ch->fighting, NULL);
      break;
    }
}

void wear_prog_ranger_staff(OBJ_DATA *obj, CHAR_DATA *ch)
{

  if ( ch->iclass != CLASS_RANGER )
  {
    send_to_char( L"Onu nasıl kullanacağını bilmiyorsun.\n\r", ch );
    unequip_char( ch, obj );
    send_to_char( L"Korucu asası elinden kayıp düşüyor.\n\r", ch );
     obj_from_char( obj );
     obj_to_room( obj, ch->in_room );
     return;
  }

}

void wear_prog_coconut(OBJ_DATA *obj, CHAR_DATA *ch)
{
  act( L"Hindistancevizi kabuklarını birbirine vurmaya başlıyorsun.",ch,NULL,NULL,TO_CHAR);
  act( L"Dörtnala gelen atların sesini duyunca bineğine atlıyorsun.",
	ch, NULL, NULL, TO_CHAR);
  act( L"$n görünmez bir atı sürüyormuş rolü yapıyor.",
	ch,NULL,NULL,TO_ROOM);
}

void entry_prog_coconut(OBJ_DATA *obj)
{
  if (obj->carried_by != NULL)
    if (get_hold_char(obj->carried_by) == obj)
    act( L"$n iki hindistancevizini birbirine vurup görünmez bineğiyle dörtnala atılıyor.",
	obj->carried_by, NULL, NULL, TO_ROOM);
}

void greet_prog_coconut(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (obj->carried_by != NULL)
    {
      if (get_hold_char(obj->carried_by) == obj &&
		obj->carried_by != ch)
    act( L"Dörtnala giden atların sesini duyuyorsun.", ch, NULL, NULL, TO_CHAR);
    }
  else
  send_to_char( L"$p dörtnala giden atların zayıf sesiyle birini çağırıyor.\n\r",ch);

}

void get_prog_coconut(OBJ_DATA *obj, CHAR_DATA *ch)
{
  send_to_char(L"Hindistancevizini kulağına tutunca dörtnala giden atların\n\rzayıf kükreyişini duyuyorsun.\n\r", ch);
  act( L"$n bir hindistancevizini kulağına tutuyor.", ch, NULL, NULL, TO_ROOM);
}

void remove_prog_coconut(OBJ_DATA *obj, CHAR_DATA *ch)
{
  send_to_char( L"Atların sesi yokoluyor.\n\r", ch);
  act( L"$n bir attan iniyormuş numarası yapıyor.", ch, NULL, NULL, TO_ROOM);
}

void fight_prog_firegauntlets( OBJ_DATA *obj, CHAR_DATA *ch )
{
int dam;

  if (get_wield_char(ch,FALSE) != NULL )
	return;

  if ( get_eq_char( ch, WEAR_HANDS ) != obj )
	return;

  if ( number_percent() < 50 )  {
	dam = dice( ch->level, 8) + number_percent() / 2;
  act( L"Eldivenlerin $S yüzünü yakıyor!", ch, NULL, ch->fighting, TO_CHAR);
	act( L"$s eldivenleri $S yüzünü yakıyor!", ch, NULL, ch->fighting, TO_NOTVICT);
	act( L"$S eldivenleri yüzünü yakıyor!", ch->fighting, NULL, ch, TO_CHAR);
	damage( ch, ch->fighting, dam/2, gsn_burning_hands, DAM_FIRE, TRUE);
	if ( ch == NULL || ch->fighting == NULL )
	  return;
	fire_effect( ch->fighting, obj->level/2, dam/2, TARGET_CHAR );
  }
  return;
}

void wear_prog_firegauntlets(OBJ_DATA *obj, CHAR_DATA *ch)
{
  send_to_char( L"Eldivenler ellerini ısıtıyor.\n\r", ch );
	return;
}

void remove_prog_firegauntlets(OBJ_DATA *obj, CHAR_DATA *ch)
{
  send_to_char( L"Ellerin soğuyor.\n\r", ch );
	return;
}

void fight_prog_armbands( OBJ_DATA *obj, CHAR_DATA *ch )
{
int dam;
  if ( get_eq_char( ch, WEAR_ARMS ) != obj )
	return;

  if ( IS_NPC(ch ) )
	return;

  if ( number_percent() < 20 )  {
	dam = number_percent()/2 + 30 + 5*ch->level;
  act( L"Kolbantların $S yüzünü yakıyor!", ch, NULL, ch->fighting, TO_CHAR);
	act( L"$s kolbantları $S yüzünü yakıyor!", ch, NULL, ch->fighting, TO_NOTVICT);
	act( L"$S kolbantları senin yüzünü yakıyor!", ch->fighting, NULL, ch, TO_CHAR);
	damage( ch, ch->fighting, dam, gsn_burning_hands, DAM_FIRE, TRUE);
	if ( ch == NULL || ch->fighting == NULL )
	  return;
	fire_effect( ch->fighting, obj->level/2, dam, TARGET_CHAR );
  }
  return;
}

void wear_prog_armbands(OBJ_DATA *obj, CHAR_DATA *ch)
{
  send_to_char( L"Kolbantların kollarını ısıtıyor.\n\r", ch );
	return;
}

void remove_prog_armbands(OBJ_DATA *obj, CHAR_DATA *ch)
{
  send_to_char( L"Kolların soğuyor.\n\r", ch );
	return;
}

void fight_prog_demonfireshield( OBJ_DATA *obj, CHAR_DATA *ch )
{
int dam;

  if ( get_shield_char( ch ) != obj )
	return;

  if ( number_percent() < 15 )  {
	dam = number_percent()/2 + 5 * ch->level;
  act( L"Kalkanının önünde gizemli bir delik açılıyor!", ch, NULL, ch->fighting, TO_CHAR);
	act( L"Kalkanın $S yüzünü yakıyor!", ch, NULL, ch->fighting, TO_CHAR);
	act( L"$s kalkanı $S yüzünü yakıyor!", ch, NULL, ch->fighting, TO_NOTVICT);
	act( L"$S kalkanı yüzünü yakıyor!", ch->fighting, NULL, ch, TO_CHAR);
	fire_effect( ch->fighting, obj->level,dam, TARGET_CHAR );
	damage( ch, ch->fighting, dam, gsn_demonfire, DAM_FIRE, TRUE);
  }
  return;
}

void wear_prog_demonfireshield(OBJ_DATA *obj, CHAR_DATA *ch)
{
  send_to_char( L"Kalkanın ellerini ısıtıyor.\n\r", ch );
	return;
}

void remove_prog_demonfireshield(OBJ_DATA *obj, CHAR_DATA *ch)
{
  send_to_char( L"Ellerin soğuyor.\n\r", ch );
	return;
}
void fight_prog_vorbalblade( OBJ_DATA *obj, CHAR_DATA *ch )
{
CHAR_DATA *victim;

  if ( IS_NPC(ch) )
	return;

  if ( !is_wielded_char( ch, obj ) )
    return;

  victim = ch->fighting;

  if (!IS_EVIL(victim)) return;

  if ( number_percent() < 10 )  {
    send_to_char(L"Silahın aniden rakibinin boynuna atılıyor!\n\r", ch);
    if ( number_percent() < 20 )  {
	act( L"Silahın havada bir yay çizerek $S kafasını koparıyor!",
	     ch, NULL, victim, TO_CHAR);
	act( L"$S silahı havada ıslık çalarak senin kafanı koparıyor!",
	     ch, NULL, victim, TO_NOTVICT);
	act( L"$s silahı havada ıslık çalarak $S kafasını koparıyor!",
	     ch, NULL, victim, TO_ROOM);
	act( L"$n ÖLDÜ!!", victim, NULL, NULL, TO_ROOM );
	act( L"$n ÖLDÜ!!", victim, NULL, NULL, TO_CHAR );
	raw_kill_org( victim, 3 );
	send_to_char( L"Ö L D Ü R Ü L D Ü N!!\n\r", victim );
	return;
    }
  }
  return;
}

void wear_prog_wind_boots(OBJ_DATA *obj, CHAR_DATA *ch)
{
  AFFECT_DATA af;

  if (!is_affected(ch, gsn_fly))
    {
      send_to_char( L"Rüzgar çizmelerini ayağına giydiğin anda uçmaya başlıyorsun.\n\r", ch);
      send_to_char( L"Uçmaya başladın.\n\r", ch);

      af.where = TO_AFFECTS;
      af.type = gsn_fly;
      af.duration = -2;
      af.level = ch->level;
      af.bitvector = AFF_FLYING;
      af.location = 0;
      af.modifier = 0;
      affect_to_char(ch, &af);
    }
}

void remove_prog_wind_boots(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (is_affected(ch, gsn_fly))
    {
      affect_strip(ch, gsn_fly);
      send_to_char( L"Yere düşüyorsun.\n\r", ch);
      send_to_char( L"Ahhh!.\n\r", ch);
    }
}
void wear_prog_boots_flying(OBJ_DATA *obj, CHAR_DATA *ch)
{
  AFFECT_DATA af;

  if (!is_affected(ch, gsn_fly))
    {
      send_to_char( L"Uçuş botlarını ayaklarına giydiğin gibi havaya yükseliyorsun.\n\r", ch);
      send_to_char( L"Uçmaya başladın.\n\r", ch);

      af.where = TO_AFFECTS;
      af.type = gsn_fly;
      af.duration = -2;
      af.level = ch->level;
      af.bitvector = AFF_FLYING;
      af.location = 0;
      af.modifier = 0;
      affect_to_char(ch, &af);
    }
}
void remove_prog_boots_flying(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (is_affected(ch, gsn_fly))
    {
      affect_strip(ch, gsn_fly);
      send_to_char( L"Yere düştün.\n\r", ch);
      send_to_char( L"Uçmak yerine yürümeye başladın!.\n\r", ch);
    }
}


void wear_prog_arm_hercules(OBJ_DATA *obj, CHAR_DATA *ch)
{
  AFFECT_DATA af;

  if (!is_affected(ch, gsn_giant_strength))
    {
      send_to_char( L"Kollukları giydiğin anda güçlendiğini hissediyorsun.\n\r", ch);
      send_to_char( L"Kaslarının irileştiğini hissediyorsun.\n\r", ch);

      af.where = TO_AFFECTS;
      af.type = gsn_giant_strength;
      af.duration = -2;
      af.level = ch->level;
      af.bitvector = 0;
      af.location = APPLY_STR;
      af.modifier = 1 + (ch->level >= 18) + (ch->level >= 30) + (ch->level >= 45);
      affect_to_char(ch, &af);
    }
}

void remove_prog_arm_hercules(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (is_affected(ch, gsn_giant_strength))
    {
      affect_strip(ch, gsn_giant_strength);
      send_to_char( L"Kasların doğal haline döndü.\n\r", ch);
    }
}

void wear_prog_girdle_giant(OBJ_DATA *obj, CHAR_DATA *ch)
{
  AFFECT_DATA af;

  if (!is_affected(ch, gsn_giant_strength))
    {
      send_to_char( L"Kemeri giydiğin anda güçlendiğini hissediyorsun.\n\r", ch);
      send_to_char( L"Kaslarının irileştiğini hissediyorsun.\n\r", ch);

      af.where = TO_AFFECTS;
      af.type = gsn_giant_strength;
      af.duration = -2;
      af.level = ch->level;
      af.bitvector = 0;
      af.location = APPLY_STR;
      af.modifier = 1 + (ch->level >= 18) + (ch->level >= 30) + (ch->level >= 45);
      affect_to_char(ch, &af);
    }
}

void remove_prog_girdle_giant(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (is_affected(ch, gsn_giant_strength))
    {
      affect_strip(ch, gsn_giant_strength);
      send_to_char( L"Kasların doğal haline döndü.\n\r", ch);
    }
}
void wear_prog_breastplate_strength(OBJ_DATA *obj, CHAR_DATA *ch)
{
  AFFECT_DATA af;

  if (!is_affected(ch, gsn_giant_strength))
    {
      send_to_char( L"Göğüslüğü giydiğin anda güçlendiğini hissediyorsun.\n\r", ch);
      send_to_char( L"Kaslarının irileştiğini hissediyorsun.\n\r", ch);

      af.where = TO_AFFECTS;
      af.type = gsn_giant_strength;
      af.duration = -2;
      af.level = ch->level;
      af.bitvector = 0;
      af.location = APPLY_STR;
      af.modifier = 1 + (ch->level >= 18) + (ch->level >= 30) + (ch->level >= 45);
      affect_to_char(ch, &af);
    }
}

void remove_prog_breastplate_strength(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (is_affected(ch, gsn_giant_strength))
    {
      affect_strip(ch, gsn_giant_strength);
      send_to_char( L"Kasların doğal haline döndü.\n\r", ch);
    }
}


void fight_prog_rose_shield(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (!( (ch->in_room->sector_type != SECT_FIELD) ||
       (ch->in_room->sector_type != SECT_FOREST) ||
       (ch->in_room->sector_type != SECT_MOUNTAIN) ||
       (ch->in_room->sector_type != SECT_HILLS) ) )
  return;

  if (get_shield_char(ch) != obj )
  return;

  if ( number_percent() < 90 )  return;

  send_to_char( L"Kalkanındaki yapraklar irileşmeye başlıyor.\n\r",ch);
  send_to_char( L"Kalkanın yaprakları seni sarıyor!.\n\r",ch->fighting);
  act( L"$s gül kalkanı irileşiyor.",ch,NULL,NULL,TO_ROOM);
  obj_cast_spell(gsn_slow,ch->level,ch,ch->fighting,NULL);
  return;
}

void fight_prog_lion_claw(OBJ_DATA *obj, CHAR_DATA *ch)
{
 if ( number_percent() < 90 )  return;

 if ( is_wielded_char(ch,obj) )
 {
   send_to_char( L"Parmakların ucundan pençenin tırnakları beliriyor.\n\r",ch);
   act_color( L"$s pençesinden bir an tırnaklar çıkıyor.",
		ch,NULL,NULL,TO_ROOM,POS_DEAD,CLR_WHITE);
  one_hit(ch,ch->fighting,TYPE_HIT,FALSE);
  one_hit(ch,ch->fighting,TYPE_HIT,FALSE);
  one_hit(ch,ch->fighting,TYPE_HIT,FALSE);
  one_hit(ch,ch->fighting,TYPE_HIT,FALSE);
  send_to_char( L"Pençenin tırnakları yokoluyor.\n\r",ch);
  act_color( L"$s pençesinin tırnakları yokoluyor.",
	ch,NULL,NULL,TO_ROOM,POS_DEAD,CLR_WHITE);
  return;
}
 return;
}


void speech_prog_ring_ra(OBJ_DATA *obj, CHAR_DATA *ch, wchar_t *speech)
{

  if (!wcscasecmp(speech, L"punish")
      && (ch->fighting) && is_equiped_char(ch,obj,WEAR_FINGER) )
    {
      send_to_char( L"Bir elektrik arkı yüzükten fırlıyor.\n\r",ch);
      act( L"Bir elektrik arkı yüzükten fırlıyor.",ch,NULL,NULL,TO_ROOM);
      obj_cast_spell(gsn_lightning_breath,ch->level,ch,ch->fighting,NULL);
      WAIT_STATE(ch, 2 * PULSE_VIOLENCE);
    }
 return;
}

void wear_prog_eyed_sword(OBJ_DATA *obj, CHAR_DATA *ch)
{
  act( L"$p objesindeki göz açılıyor.",ch,obj,NULL,TO_CHAR);
  act( L"$p objesindeki göz açılıyor.",ch,obj,NULL,TO_ROOM);
  if (  ch->level <= 10)			obj->value[2] = 2;
  else if ( ch->level > 10 && ch->level <= 20)   obj->value[2] = 3;
  else if ( ch->level > 20 && ch->level <= 30)   obj->value[2] = 4;
  else if ( ch->level > 30 && ch->level <= 40)   obj->value[2] = 5;
  else if ( ch->level > 40 && ch->level <= 50)   obj->value[2] = 6;
  else if ( ch->level > 50 && ch->level <= 60)   obj->value[2] = 7;
  else if ( ch->level > 60 && ch->level <= 70)   obj->value[2] = 9;
  else if ( ch->level > 70 && ch->level <= 80)   obj->value[2] = 11;
  else obj->value[2] = 12;
  obj->level = ch->level;
  return;
}

void wear_prog_katana_sword(OBJ_DATA *obj, CHAR_DATA *ch)
{
  obj->value[2] = 2;
  if ( obj->item_type == ITEM_WEAPON
	&& IS_WEAPON_STAT(obj,WEAPON_KATANA)
	&& wcsstr( obj->extra_descr->description, ch->name ) != NULL)
  {
   if (  ch->level <= 10)			obj->value[2] = 2;
   else if ( ch->level > 10 && ch->level <= 20)   obj->value[2] = 3;
   else if ( ch->level > 20 && ch->level <= 30)   obj->value[2] = 4;
   else if ( ch->level > 30 && ch->level <= 40)   obj->value[2] = 5;
   else if ( ch->level > 40 && ch->level <= 50)   obj->value[2] = 6;
   else if ( ch->level > 50 && ch->level <= 60)   obj->value[2] = 7;
   else if ( ch->level > 60 && ch->level <= 70)   obj->value[2] = 9;
   else if ( ch->level > 70 && ch->level <= 80)   obj->value[2] = 11;
   else obj->value[2] = 12;
   obj->level = ch->level;
   send_to_char( L"Katananın senin bir parçan olduğunu hissediyorsun!\n\r",ch);
  }
  return;
}


void wear_prog_snake(OBJ_DATA *obj, CHAR_DATA *ch)
{
  act_color( L"$CKırbacın yılanları zehirli bir gaz salmaya başlıyor.$c",ch,obj,NULL,TO_CHAR,POS_DEAD,CLR_GREEN);
  act_color( L"$CKırbacın yılanları zehirli bir gaz salmaya başlıyor.$c",ch,obj,NULL,TO_ROOM,POS_DEAD,CLR_GREEN);
  if ( ch->level > 20 && ch->level <= 30)       obj->value[2] = 4;
  else if ( ch->level > 30 && ch->level <= 40)   obj->value[2] = 5;
  else if ( ch->level > 40 && ch->level <= 50)   obj->value[2] = 6;
  else if ( ch->level > 50 && ch->level <= 60)   obj->value[2] = 7;
  else if ( ch->level > 60 && ch->level <= 70)   obj->value[2] = 9;
  else if ( ch->level > 70 && ch->level <= 80)   obj->value[2] = 11;
  else obj->value[2] = 12;

  return;
}


void remove_prog_snake(OBJ_DATA *obj, CHAR_DATA *ch)
{
  act_color( L"$CKırbacın yılanları kabartmalara dönüşüyor.$c",ch,obj,NULL,TO_CHAR,POS_DEAD,CLR_RED);
  act_color( L"$CKırbacın yılanları kabartmalara dönüşüyor.$c",ch,obj,NULL,TO_ROOM,POS_DEAD,CLR_RED);
}

void get_prog_snake(OBJ_DATA *obj, CHAR_DATA *ch)
{
  act( L"Kalkanın yılanlarının uykuya daldığını hissediyorsun.",ch,obj,NULL,TO_CHAR);
}

void wear_prog_fire_shield(OBJ_DATA *obj, CHAR_DATA *ch)
{
  AFFECT_DATA af;

  if ( wcsstr( obj->extra_descr->description, L"cold" ) != NULL )
   {
    if (!is_affected(ch, gsn_fire_shield))
    {
      send_to_char( L"Kalkanı kullanmaya başladığında soğuğa karşı dayanıklılaşıyorsun.\n\r", ch);

      af.where = TO_RESIST;
      af.type = gsn_fire_shield;
      af.duration = -2;
      af.level = ch->level;
      af.bitvector = RES_COLD;
      af.location = 0;
      af.modifier = 0;
      affect_to_char(ch, &af);
    }
   }
  else
   {
    if (!is_affected(ch, gsn_fire_shield))
    {
      send_to_char( L"Kalkanı kullanmaya başladığında sıcağa karşı dayanıklılaşıyorsun.\n\r", ch);

      af.where = TO_RESIST;
      af.type = gsn_fire_shield;
      af.duration = -2;
      af.level = ch->level;
      af.bitvector = RES_FIRE;
      af.location = 0;
      af.modifier = 0;
      affect_to_char(ch, &af);
    }
   }
}

void remove_prog_fire_shield(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (is_affected(ch, gsn_fire_shield))
    {
      affect_strip(ch, gsn_fire_shield);
      if ( wcsstr( obj->extra_descr->description, L"cold" ) != NULL )
      send_to_char( L"Soğuk saldırılarına karşı korunman azalıyor.\n\r", ch);
     else  send_to_char( L"Ateş saldırılarına karşı korunman azalıyor.\n\r", ch);
    }
}


void wear_prog_quest_weapon(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if ( wcsstr( obj->short_descr, ch->name ) != NULL )
  {
    send_ch_color(L"$CSilahın parlamaya başlıyor.$c",ch,POS_SLEEPING,CLR_BLUE);
    if ( ch->level > 20 && ch->level <= 30)	   obj->value[2] = 4;
    else if ( ch->level > 30 && ch->level <= 40)   obj->value[2] = 5;
    else if ( ch->level > 40 && ch->level <= 50)   obj->value[2] = 6;
    else if ( ch->level > 50 && ch->level <= 60)   obj->value[2] = 7;
    else if ( ch->level > 60 && ch->level <= 70)   obj->value[2] = 9;
    else if ( ch->level > 70 && ch->level <= 80)   obj->value[2] = 11;
    else obj->value[2] = 12;
    obj->level = ch->level;
    return;
  }

  act( L"Sen $p tarafından çarpıldın ve onu düşürdün.", ch, obj, NULL, TO_CHAR );

  obj_from_char( obj );
  obj_to_room( obj, ch->in_room );
  return;
}

void fight_prog_ancient_gloves( OBJ_DATA *obj, CHAR_DATA *ch )
{
 int dam;

 if ( get_eq_char(ch, WEAR_HANDS) != obj
	|| get_wield_char(ch, FALSE) != NULL)
        return;

 if (number_percent() < 20)
 {
   dam = number_percent() + dice(ch->level, 14);
   act( L"$E dokunmanla elindeki alevin ona vurması bir oluyor!",
	ch, NULL, ch->fighting,TO_CHAR);
   act( L"$s $E dokunmasıyla $s elindeki alevin $M vurması bir oluyor!",
	ch, NULL, ch->fighting,TO_NOTVICT);
   act( L"$S sana dokunmasıyla elindeki alevin seni vurması bir oluyor!"
	, ch->fighting, NULL,ch,TO_CHAR);
   fire_effect( ch->fighting, obj->level,dam, TARGET_CHAR );
   damage( ch, ch->fighting, dam, gsn_burning_hands, DAM_FIRE, TRUE);
 }
  return;
}


void remove_prog_ancient_gloves(OBJ_DATA *obj, CHAR_DATA *ch)
{
  send_to_char( L"Ellerindeki alev yokoluyor.\n\r", ch );
        return;
}

void wear_prog_ancient_gloves(OBJ_DATA *obj, CHAR_DATA *ch)
{
  send_to_char( L"Ellerinde bir alev dans etmeye başlıyor!\n\r", ch );
        return;
}

void fight_prog_ancient_shield(OBJ_DATA *obj, CHAR_DATA *ch)
{
 int chance;
 int dam;

 if ( get_shield_char( ch ) != obj )
        return;

 if ( (chance = number_percent()) < 5)
 {
   dam = dice(ch->level, 20);
   act( L"Kalkanın ışık saçmaya başladı!", ch, NULL,ch->fighting,TO_CHAR);
   act( L"$s kalkanı ışık saçmaya başladı!", ch, NULL,ch->fighting,TO_VICT);
   act( L"$s kalkanı ışık saçmaya başladı!", ch, NULL,ch->fighting,TO_NOTVICT);
   fire_effect( ch->fighting, obj->level/2, dam, TARGET_CHAR );
   damage( ch, ch->fighting, dam, gsn_fire_breath, DAM_FIRE, TRUE);
 }
 else if ( chance < 10 )
 {
   act( L"Kalkanın kızıl bir aurayla parlamaya başladı!",
	ch, NULL,ch->fighting,TO_CHAR);
   act( L"$s kalkanı kızıl bir aurayla parlamaya başladı!",
	 ch, NULL,ch->fighting,TO_VICT);
   act( L"$s kalkanı kızıl bir aurayla parlamaya başladı!",
	ch, NULL,ch->fighting,TO_NOTVICT);
   obj_cast_spell(gsn_blindness, ch->level+ 5, ch, ch->fighting,obj);
   obj_cast_spell(gsn_slow, ch->level + 5, ch, ch->fighting, obj);
 }
 return;
}

void remove_prog_ancient_shield(OBJ_DATA *obj, CHAR_DATA *ch)
{
  act_color(  L"$CKalkanın eski haline dönüyor.$c",ch,obj,NULL,TO_CHAR,POS_DEAD,CLR_RED);
  act_color(  L"$C$s kalkanı eski haline dönüyor.$c",ch,obj,NULL,TO_ROOM,POS_DEAD,CLR_RED);
  return;

}

void wear_prog_ancient_shield(OBJ_DATA *obj, CHAR_DATA *ch)
{
  act_color(
 L"$CKalkanın şekil değiştirerek bir ejderhaya benzemeye başlıyor.\n\r"
 L"Kalkanın üstünden yükselen bir ejderha başı ağzını açıyor!$c",
                ch,obj,NULL,TO_CHAR,POS_DEAD,CLR_RED);
  act_color(
 L"$C$s kalkanı şekil değiştirerek bir ejderhaya benzemeye başlıyor.\n\r"
 L"Kalkanın üstünden yükselen bir ejderha başı ağzını açıyor!$c",
                ch,obj,NULL,TO_ROOM,POS_DEAD,CLR_RED);
  return;
}


void wear_prog_neckguard(OBJ_DATA *obj, CHAR_DATA *ch)
{
  AFFECT_DATA af;

  if (!is_affected(ch, gsn_neckguard))
    {
      af.where = TO_AFFECTS;
      af.type = gsn_neckguard;
      af.duration = -2;
      af.level = ch->level;
      af.bitvector = 0;
      af.location = APPLY_NONE;
      af.modifier = 0;
      affect_to_char(ch, &af);
    }
}

void remove_prog_neckguard(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (is_affected(ch, gsn_neckguard))
      affect_strip(ch, gsn_neckguard);
}

void wear_prog_headguard(OBJ_DATA *obj, CHAR_DATA *ch)
{
  AFFECT_DATA af;

  if (!is_affected(ch, gsn_headguard))
    {
      af.where = TO_AFFECTS;
      af.type = gsn_headguard;
      af.duration = -2;
      af.level = ch->level;
      af.bitvector = 0;
      af.location = APPLY_NONE;
      af.modifier = 0;
      affect_to_char(ch, &af);
    }
}

void remove_prog_headguard(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (is_affected(ch, gsn_headguard))
      affect_strip(ch, gsn_headguard);
}

void wear_prog_blackguard(OBJ_DATA *obj, CHAR_DATA *ch)
{
  AFFECT_DATA af;

  if (!is_affected(ch, gsn_blackguard))
    {
      af.where = TO_AFFECTS;
      af.type = gsn_blackguard;
      af.duration = -2;
      af.level = ch->level;
      af.bitvector = 0;
      af.location = APPLY_NONE;
      af.modifier = 0;
      affect_to_char(ch, &af);
    }
}

void remove_prog_blackguard(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (is_affected(ch, gsn_blackguard))
      affect_strip(ch, gsn_blackguard);
}
