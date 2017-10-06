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

DECLARE_OPROG_FUN_FIGHT( fight_prog_tattoo_apollon );
DECLARE_OPROG_FUN_FIGHT( fight_prog_tattoo_zeus );
DECLARE_OPROG_FUN_FIGHT( fight_prog_tattoo_siebele );
DECLARE_OPROG_FUN_FIGHT( fight_prog_tattoo_ahrumazda );
DECLARE_OPROG_FUN_FIGHT( fight_prog_tattoo_hephaestus );
DECLARE_OPROG_FUN_FIGHT( fight_prog_tattoo_ehrumen );
DECLARE_OPROG_FUN_FIGHT( fight_prog_tattoo_venus );
DECLARE_OPROG_FUN_FIGHT( fight_prog_tattoo_deimos );
DECLARE_OPROG_FUN_FIGHT( fight_prog_tattoo_odin );
DECLARE_OPROG_FUN_FIGHT( fight_prog_tattoo_phobos );
DECLARE_OPROG_FUN_FIGHT( fight_prog_tattoo_mars );
DECLARE_OPROG_FUN_FIGHT( fight_prog_tattoo_athena );
DECLARE_OPROG_FUN_FIGHT(fight_prog_tattoo_prometheus);
DECLARE_OPROG_FUN_FIGHT(fight_prog_tattoo_goktengri);
DECLARE_OPROG_FUN_FIGHT( fight_prog_tattoo_hera);
DECLARE_OPROG_FUN_FIGHT( fight_prog_tattoo_ares);
DECLARE_OPROG_FUN_FIGHT( fight_prog_tattoo_eros);

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
DECLARE_OPROG_FUN_GET(get_prog_quest_obj	);
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
DECLARE_OPROG_FUN_GET(get_prog_quest_hreward	);

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


void oprog_set(OBJ_INDEX_DATA *objindex,const char *progtype, const char *name)
{

   if (!str_cmp(progtype, "wear_prog"))
     {
 /*
  *    if (!str_cmp(name, "wear_prog_"))
  *          objindex->mprogs->wear_prog = wear_prog_;
  *    else if (!str_cmp(name, "wear_prog_")) ...
  */

       if (!str_cmp(name, "wear_prog_excalibur"))
	 objindex->oprogs->wear_prog = wear_prog_excalibur;
       else if (!str_cmp(name, "wear_prog_bracer"))
	 objindex->oprogs->wear_prog = wear_prog_bracer;
       else if (!str_cmp(name, "wear_prog_coconut"))
	 objindex->oprogs->wear_prog = wear_prog_coconut;
       else if (!str_cmp(name, "wear_prog_firegauntlets"))
	 objindex->oprogs->wear_prog = wear_prog_firegauntlets;
      else if (!str_cmp(name, "wear_prog_armbands"))
	 objindex->oprogs->wear_prog = wear_prog_armbands;
      else if (!str_cmp(name, "wear_prog_demonfireshield"))
	 objindex->oprogs->wear_prog = wear_prog_demonfireshield;
       else if (!str_cmp(name, "wear_prog_ranger_staff"))
	 objindex->oprogs->wear_prog = wear_prog_ranger_staff;
       else if (!str_cmp(name, "wear_prog_wind_boots"))
	 objindex->oprogs->wear_prog = wear_prog_wind_boots;
       else if (!str_cmp(name, "wear_prog_boots_flying"))
	 objindex->oprogs->wear_prog = wear_prog_boots_flying;
       else if (!str_cmp(name, "wear_prog_arm_hercules"))
	 objindex->oprogs->wear_prog = wear_prog_arm_hercules;
       else if (!str_cmp(name, "wear_prog_girdle_giant"))
	 objindex->oprogs->wear_prog = wear_prog_girdle_giant;
       else if (!str_cmp(name, "wear_prog_breastplate_strength"))
	 objindex->oprogs->wear_prog = wear_prog_breastplate_strength;
       
       else if (!str_cmp(name, "wear_prog_katana_sword"))
	 objindex->oprogs->wear_prog = wear_prog_katana_sword;
       else if (!str_cmp(name, "wear_prog_eyed_sword"))
	 objindex->oprogs->wear_prog = wear_prog_eyed_sword;
       else if (!str_cmp(name, "wear_prog_snake"))
	 objindex->oprogs->wear_prog = wear_prog_snake;
       else if (!str_cmp(name, "wear_prog_fire_shield"))
	 objindex->oprogs->wear_prog = wear_prog_fire_shield;
       else if (!str_cmp(name, "wear_prog_quest_weapon"))
	 objindex->oprogs->wear_prog = wear_prog_quest_weapon;
       else if (!str_cmp(name, "wear_prog_ancient_gloves"))
	 objindex->oprogs->wear_prog = wear_prog_ancient_gloves;
       else if (!str_cmp(name, "wear_prog_ancient_shield"))
	 objindex->oprogs->wear_prog = wear_prog_ancient_shield;
       else if (!str_cmp(name, "wear_prog_neckguard"))
	 objindex->oprogs->wear_prog = wear_prog_neckguard;
       else if (!str_cmp(name, "wear_prog_headguard"))
	 objindex->oprogs->wear_prog = wear_prog_headguard;
       else if (!str_cmp(name, "wear_prog_blackguard"))
	 objindex->oprogs->wear_prog = wear_prog_blackguard;
       else 
	 {
	   bug("Load_oprogs: 'O': Function not found for vnum %d",
	       objindex->vnum);
	   exit(1);
	 }

       SET_BIT(objindex->progtypes, OPROG_WEAR);
       return;
     }

   if (!str_cmp(progtype, "remove_prog"))
     {
       if (!str_cmp(name,"remove_prog_excalibur"))
	 objindex->oprogs->remove_prog = remove_prog_excalibur;
       else if (!str_cmp(name, "remove_prog_bracer"))
	 objindex->oprogs->remove_prog = remove_prog_bracer;
       else if (!str_cmp(name, "remove_prog_coconut"))
	 objindex->oprogs->remove_prog = remove_prog_coconut;
       else if (!str_cmp(name, "remove_prog_firegauntlets"))
	 objindex->oprogs->remove_prog = remove_prog_firegauntlets;
       else if (!str_cmp(name, "remove_prog_armbands"))
	 objindex->oprogs->remove_prog = remove_prog_armbands;
       else if (!str_cmp(name, "remove_prog_demonfireshield"))
	 objindex->oprogs->remove_prog = remove_prog_demonfireshield;
       else if (!str_cmp(name, "remove_prog_wind_boots"))
	 objindex->oprogs->remove_prog = remove_prog_wind_boots;
       else if (!str_cmp(name, "remove_prog_boots_flying"))
	 objindex->oprogs->remove_prog = remove_prog_boots_flying;
       else if (!str_cmp(name, "remove_prog_arm_hercules"))
	 objindex->oprogs->remove_prog = remove_prog_arm_hercules;
       else if (!str_cmp(name, "remove_prog_girdle_giant"))
	 objindex->oprogs->remove_prog = remove_prog_girdle_giant;
       else if (!str_cmp(name, "remove_prog_breastplate_strength"))
	 objindex->oprogs->remove_prog = remove_prog_breastplate_strength;
       else if (!str_cmp(name, "remove_prog_snake"))
	 objindex->oprogs->remove_prog = remove_prog_snake;
       else if (!str_cmp(name, "remove_prog_fire_shield"))
	 objindex->oprogs->remove_prog = remove_prog_fire_shield;
       else if (!str_cmp(name, "remove_prog_ancient_gloves"))
	 objindex->oprogs->remove_prog = remove_prog_ancient_gloves;
       else if (!str_cmp(name, "remove_prog_ancient_shield"))
	 objindex->oprogs->remove_prog = remove_prog_ancient_shield;
       else if (!str_cmp(name, "remove_prog_neckguard"))
	 objindex->oprogs->remove_prog = remove_prog_neckguard;
       else if (!str_cmp(name, "remove_prog_headguard"))
	 objindex->oprogs->remove_prog = remove_prog_headguard;
       else if (!str_cmp(name, "remove_prog_blackguard"))
	 objindex->oprogs->remove_prog = remove_prog_blackguard;
       else
	 {
	   bug("Load_oprogs: 'O': Function not found for vnum %d",
	       objindex->vnum);
	   exit(1);
	 }
       SET_BIT(objindex->progtypes, OPROG_REMOVE);
       return;
     }
   if (!str_cmp(progtype, "get_prog"))
     {
       if (!str_cmp(name,"get_prog_cabal_item"))
	 objindex->oprogs->get_prog = get_prog_cabal_item;
       else if (!str_cmp(name, "get_prog_heart"))
	 objindex->oprogs->get_prog = get_prog_heart;
       else if (!str_cmp(name, "get_prog_coconut"))
	 objindex->oprogs->get_prog = get_prog_coconut;
       else if (!str_cmp(name, "get_prog_spec_weapon"))
	 objindex->oprogs->get_prog = get_prog_spec_weapon;
       else if (!str_cmp(name, "get_prog_quest_obj"))
	 objindex->oprogs->get_prog = get_prog_quest_obj;
       else if (!str_cmp(name, "get_prog_snake"))
	 objindex->oprogs->get_prog = get_prog_snake;
       else if (!str_cmp(name, "get_prog_quest_reward"))
	 objindex->oprogs->get_prog = get_prog_quest_reward;
       else if (!str_cmp(name, "get_prog_quest_hreward"))
	 objindex->oprogs->get_prog = get_prog_quest_hreward;
       else
	 {
	   bug("Load_oprogs: 'O': Function not found for vnum %d",
	       objindex->vnum);
	   exit(1);
	 }
       SET_BIT(objindex->progtypes, OPROG_GET);
       return;
     }
   if (!str_cmp(progtype, "drop_prog"))
     {
/*
       if (!str_cmp(name, "drop_prog_"))
	 objindex->oprogs->drop_prog = drop_prog_;
       else
*/
	 {
	   bug("Load_oprogs: 'O': Function not found for vnum %d",
	       objindex->vnum);
	   exit(1);
	 }
       SET_BIT(objindex->progtypes, OPROG_DROP);	
       return;
     }

   /* 
    * returning TRUE prevents destruction, gives player gold
    */
   if (!str_cmp(progtype, "sac_prog")) 
     {
       if (!str_cmp(name, "sac_prog_excalibur"))
	 objindex->oprogs->sac_prog = sac_prog_excalibur;
       else if (!str_cmp(name, "sac_prog_cabal_item"))
	 objindex->oprogs->sac_prog = sac_prog_cabal_item;
       else
	 {
	   bug("Load_oprogs: 'O': Function not found for vnum %d",
	       objindex->vnum);
	   exit(1);
	 }
       SET_BIT(objindex->progtypes, OPROG_SAC);
       return;
     }
   if (!str_cmp(progtype, "entry_prog"))
     {
       if (!str_cmp(name, "entry_prog_coconut"))
         objindex->oprogs->entry_prog = entry_prog_coconut;
       else
         {
           bug("Load_oprogs: 'O': Function not found for vnum %d",
               objindex->vnum);
           exit(1);
         }
       SET_BIT(objindex->progtypes, OPROG_ENTRY);
       return;
     }
   if (!str_cmp(progtype, "give_prog"))
     {
      /*
       if (!str_cmp(name, "give_prog_"))
         objindex->oprogs->give_prog = give_prog_;
       else
       */
         {
           bug("Load_oprogs: 'O': Function not found for vnum %d",
               objindex->vnum);
           exit(1);
         }
       SET_BIT(objindex->progtypes, OPROG_GIVE);
       return;
     }
   if (!str_cmp(progtype, "greet_prog"))
     {
       if (!str_cmp(name, "greet_prog_coconut"))
         objindex->oprogs->greet_prog = greet_prog_coconut;
       else
         {
           bug("Load_oprogs: 'O': Function not found for vnum %d",
               objindex->vnum);
           exit(1);
         }
       SET_BIT(objindex->progtypes, OPROG_GREET);
       return;
     }
   if (!str_cmp(progtype, "fight_prog"))
     {
       if (!str_cmp(name, "fight_prog_ranger_staff"))
	 objindex->oprogs->fight_prog = fight_prog_ranger_staff;
       else if (!str_cmp(name, "fight_prog_sub_weapon"))
	 objindex->oprogs->fight_prog = fight_prog_sub_weapon;
       else if (!str_cmp(name, "fight_prog_chaos_blade"))
	 objindex->oprogs->fight_prog = fight_prog_chaos_blade;
       else if (!str_cmp(name, "fight_prog_tattoo_apollon"))
	 objindex->oprogs->fight_prog = fight_prog_tattoo_apollon;
       else if (!str_cmp(name, "fight_prog_tattoo_zeus"))
	 objindex->oprogs->fight_prog = fight_prog_tattoo_zeus;
       else if (!str_cmp(name, "fight_prog_tattoo_siebele"))
	 objindex->oprogs->fight_prog = fight_prog_tattoo_siebele;
       else if (!str_cmp(name, "fight_prog_tattoo_ahrumazda"))
         objindex->oprogs->fight_prog = fight_prog_tattoo_ahrumazda;
       else if (!str_cmp(name, "fight_prog_tattoo_hephaestus"))
	 objindex->oprogs->fight_prog = fight_prog_tattoo_hephaestus;
       else if (!str_cmp(name, "fight_prog_tattoo_ehrumen"))
	 objindex->oprogs->fight_prog = fight_prog_tattoo_ehrumen;
       else if (!str_cmp(name, "fight_prog_tattoo_venus"))
	 objindex->oprogs->fight_prog = fight_prog_tattoo_venus;
       else if (!str_cmp(name, "fight_prog_tattoo_deimos"))
	 objindex->oprogs->fight_prog = fight_prog_tattoo_deimos;
       else if (!str_cmp(name, "fight_prog_tattoo_odin"))
	 objindex->oprogs->fight_prog = fight_prog_tattoo_odin;
       else if (!str_cmp(name, "fight_prog_tattoo_phobos"))
	 objindex->oprogs->fight_prog = fight_prog_tattoo_phobos;
       else if (!str_cmp(name, "fight_prog_tattoo_mars"))
	 objindex->oprogs->fight_prog = fight_prog_tattoo_mars;
       else if (!str_cmp(name, "fight_prog_tattoo_athena"))
	 objindex->oprogs->fight_prog = fight_prog_tattoo_athena; 
       else if (!str_cmp(name, "fight_prog_golden_weapon"))
	 objindex->oprogs->fight_prog = fight_prog_golden_weapon;
       else if (!str_cmp(name, "fight_prog_snake"))
	 objindex->oprogs->fight_prog = fight_prog_snake;
       else if (!str_cmp(name, "fight_prog_tattoo_prometheus"))
	 objindex->oprogs->fight_prog = fight_prog_tattoo_prometheus;
       else if (!str_cmp(name, "fight_prog_tattoo_goktengri"))
	 objindex->oprogs->fight_prog = fight_prog_tattoo_goktengri;
       else if (!str_cmp(name, "fight_prog_shockwave"))
	 objindex->oprogs->fight_prog = fight_prog_shockwave;
       else if (!str_cmp(name, "fight_prog_firegauntlets"))
	 objindex->oprogs->fight_prog = fight_prog_firegauntlets;
       else if (!str_cmp(name, "fight_prog_armbands"))
	 objindex->oprogs->fight_prog = fight_prog_armbands;
       else if (!str_cmp(name, "fight_prog_demonfireshield"))
	 objindex->oprogs->fight_prog = fight_prog_demonfireshield;
       else if (!str_cmp(name, "fight_prog_vorbalblade"))
	 objindex->oprogs->fight_prog = fight_prog_vorbalblade;
       else if (!str_cmp(name, "fight_prog_rose_shield"))
	 objindex->oprogs->fight_prog = fight_prog_rose_shield;
       else if (!str_cmp(name, "fight_prog_lion_claw"))
	 objindex->oprogs->fight_prog = fight_prog_lion_claw;
       else if (!str_cmp(name, "fight_prog_tattoo_hera"))
	 objindex->oprogs->fight_prog = fight_prog_tattoo_hera;
       else if (!str_cmp(name, "fight_prog_tattoo_ares"))
	 objindex->oprogs->fight_prog = fight_prog_tattoo_ares;
       else if (!str_cmp(name, "fight_prog_tattoo_eros"))
	 objindex->oprogs->fight_prog = fight_prog_tattoo_eros;
       else if (!str_cmp(name, "fight_prog_ancient_gloves"))
	 objindex->oprogs->fight_prog = fight_prog_ancient_gloves;
       else if (!str_cmp(name, "fight_prog_ancient_shield"))
	 objindex->oprogs->fight_prog = fight_prog_ancient_shield;
       else
	 {
	   bug("Load_oprogs: 'O': Function not found for vnum %d",
	       objindex->vnum);
	   exit(1);
	 }
       SET_BIT(objindex->progtypes, OPROG_FIGHT);
       return;
     }
   if (!str_cmp(progtype, "death_prog")) /* returning TRUE prevents death */
     {
       if (!str_cmp(name, "death_prog_excalibur"))
	 objindex->oprogs->death_prog = death_prog_excalibur;
       else if (!str_cmp(name, "death_prog_ranger_staff"))
	 objindex->oprogs->death_prog = death_prog_ranger_staff;
       else if (!str_cmp(name, "death_prog_chaos_blade"))
	 objindex->oprogs->death_prog = death_prog_chaos_blade;
       else if (!str_cmp(name, "death_prog_golden_weapon"))
	 objindex->oprogs->death_prog = death_prog_golden_weapon;
       else
	 {
	   bug("Load_oprogs: 'O': Function not found for vnum %d",
	       objindex->vnum);
	   exit(1);
	 }
       SET_BIT(objindex->progtypes, OPROG_DEATH);
       return;
     }
   if (!str_cmp(progtype, "speech_prog"))
     {
       if (!str_cmp(name, "speech_prog_excalibur"))
	 objindex->oprogs->speech_prog = speech_prog_excalibur;
       else if(!str_cmp(name, "speech_prog_kassandra"))
	 objindex->oprogs->speech_prog = speech_prog_kassandra;
       else if(!str_cmp(name, "speech_prog_ring_ra"))
	 objindex->oprogs->speech_prog = speech_prog_ring_ra;
       else
	 {
	   bug("Load_oprogs: 'O': Function not found for vnum %d",
	       objindex->vnum);
	   exit(1);
	 }
       SET_BIT(objindex->progtypes, OPROG_SPEECH);
       return;
     }

   if (!str_cmp(progtype, "area_prog"))
     {
/*
       if (!str_cmp(name, "area_prog_"))
	 objindex->oprogs->area_prog = area_prog_;
       else
*/
	 {
	   bug("Load_oprogs: 'O': Function not found for vnum %d",
	       objindex->vnum);
	   exit(1);
	 }
       SET_BIT(objindex->progtypes, OPROG_AREA);
       return;
     }


   bug( "Load_oprogs: 'O': invalid program type for vnum %d",objindex->vnum);
   exit(1);
 }



void wear_prog_excalibur(OBJ_DATA *obj, CHAR_DATA *ch)
{
  act("$p begins to shine a bright white.",ch,obj,NULL,TO_CHAR);
  act("$p begins to shine a bright white.",ch,obj,NULL,TO_ROOM);
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
      send_to_char("As you slide your arms into these bracers, they mold to your skin.\n\r", ch);
      send_to_char("Your hands and arms feel incredibly light.\n\r", ch);

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
      send_to_char("Your hands and arms feel heavy again.\n\r", ch);
    }
}


void remove_prog_excalibur(OBJ_DATA *obj, CHAR_DATA *ch)
{
  act("$p stops glowing.",ch,obj,NULL,TO_CHAR);
  act("$p stops glowing.",ch,obj,NULL,TO_ROOM);
}

bool death_prog_excalibur(OBJ_DATA *obj, CHAR_DATA *ch)
{
  act_new("$p starts to glow with a blue aura.",ch,obj,NULL,TO_CHAR,POS_DEAD);
  act("$p starts to glow with a blue aura,",ch,obj,NULL,TO_ROOM);
  ch->hit = ch->max_hit;
  send_to_char("You feel much better.",ch);
  act("$n looks much better.",ch,NULL,NULL,TO_ROOM);
  return TRUE; 
}

void speech_prog_excalibur(OBJ_DATA *obj, CHAR_DATA *ch, char *speech)
{

  if (!str_cmp(speech, "sword of acid")      
      && (ch->fighting) && is_wielded_char(ch,obj)  )
    {
      send_to_char("Acid sprays from the blade of Excalibur.\n\r",ch);
      act("Acid sprays from the blade of Excalibur.",ch,NULL,NULL,TO_ROOM);
      obj_cast_spell(gsn_acid_blast,ch->level,ch,ch->fighting,NULL);
      WAIT_STATE(ch, 2 * PULSE_VIOLENCE);
    }
}
  
bool sac_prog_excalibur(OBJ_DATA *obj, CHAR_DATA *ch)
{
  act("The gods are infuriated!",ch,NULL,NULL,TO_CHAR);
  act("The gods are infuriated!",ch,NULL,NULL,TO_ROOM);
  damage(ch,ch,
	 (ch->hit - 1) > 1000? 1000 : (ch->hit - 1),
	 TYPE_HIT,DAM_HOLY, TRUE);
  ch->gold = 0;
  return TRUE; 
}

void fight_prog_ranger_staff(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if ( is_wielded_char(ch,obj) && number_percent() < 10)
    {
      send_to_char("Your ranger's staff glows blue!\n\r",ch);
      act("$n's ranger's staff glows blue!",ch,NULL,NULL,TO_ROOM);
      
      obj_cast_spell(gsn_cure_critical,ch->level,ch,ch,obj);
    }
}

void fight_prog_sub_weapon(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (is_wielded_char(ch,obj) && number_percent() < 30)
    {
      if ( ((float) ch->hit)/((float) ch->max_hit) > 0.9)
	send_to_char("Your weapon whispers, 'You're doing great!'\n\r",ch);
      else if ( ((float) ch->hit)/((float) ch->max_hit) > 0.6)
	send_to_char("Your weapon whispers, 'Keep up the good work!'\n\r",ch);
      else if ( ((float) ch->hit)/((float) ch->max_hit) > 0.4)
	  send_to_char("Your weapon whispers, 'You can do it!'\n\r",ch);
      else send_to_char("Your weapon whispers, 'Run away! Run away!'\n\r",ch);
    }
}

bool death_prog_ranger_staff(OBJ_DATA *obj, CHAR_DATA *ch)
{
  send_to_char("Your ranger's staff disappears.\n\r",ch);
  act("$n's ranger's staff disappears.",ch,NULL,NULL,TO_ROOM);
  extract_obj(obj);
  return FALSE;
}


void get_prog_spec_weapon(OBJ_DATA *obj, CHAR_DATA *ch) 
{

  if ( strstr( obj->extra_descr->description, ch->name ) != NULL )  
  {
    if ( IS_AFFECTED( ch, AFF_POISON ) && (dice(1,5)==1) )  {
      send_to_char( "Your weapon glows blue.", ch );
      act( "$n's weapon glows blue.", ch, NULL, NULL, TO_ROOM );
      spell_cure_poison( gsn_cure_poison, 30, ch, ch, TARGET_CHAR );
      return;
    }
    if ( IS_AFFECTED( ch, AFF_CURSE ) && (dice(1,5)==1) )  {
      send_to_char( "Your weapon glows blue.", ch );
      act( "$n's weapon glows blue.", ch, NULL, NULL, TO_ROOM );
      spell_remove_curse( gsn_remove_curse, 30, ch, ch, TARGET_CHAR );
      return;
    }
    send_to_char( "Your weapon's humming gets lauder.\n\r", ch );
    return;
  }
  act( "You are zapped by $p and drop it.", ch, obj, NULL, TO_CHAR );

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

void get_prog_quest_hreward(OBJ_DATA *obj, CHAR_DATA *ch) 
{

  if ( strstr( obj->extra_descr->description, ch->name ) != NULL )  
  {
    act_color("$CYour $p starts glowing.\n\r$c",
		ch,obj,NULL,TO_CHAR,POS_SLEEPING,CLR_BLUE);
    return;
  }

  act( "You are zapped by $p and drop it.", ch, obj, NULL, TO_CHAR );

  obj_from_char( obj );
  obj_to_room( obj, ch->in_room );

  return;
}
void get_prog_quest_obj(OBJ_DATA *obj, CHAR_DATA *ch) 
{

  if ( strstr( obj->extra_descr->description, ch->name ) != NULL )  
  {
    if ( IS_AFFECTED( ch, AFF_POISON ) && (dice(1,5)==1) )  {
      send_to_char( "Your weapon glows blue.", ch );
      act( "$n's weapon glows blue.", ch, NULL, NULL, TO_ROOM );
      spell_cure_poison( gsn_cure_poison, 30, ch, ch, TARGET_CHAR );
      return;
    }
    if ( IS_AFFECTED( ch, AFF_CURSE ) && (dice(1,5)==1) )  {
      send_to_char( "Your weapon glows blue.", ch );
      act( "$n's weapon glows blue.", ch, NULL, NULL, TO_ROOM );
      spell_remove_curse( gsn_remove_curse, 30, ch, ch, TARGET_CHAR );
      return;
    }
    send_to_char( "Quest staff waits patiently to return.\n\r", ch );
    return;
  }
  act( "You are zapped by $p and drop it.", ch, obj, NULL, TO_CHAR );

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

void get_prog_cabal_item(OBJ_DATA *obj, CHAR_DATA *ch) 
{
  if (IS_NPC(ch))
  {
      act("You are not worthy to have $p and drop it.",
	 ch, obj, NULL, TO_CHAR);
      act("$n is not worthy to have $p and drops it.",
	 ch, obj, NULL, TO_ROOM);
      obj_from_char(obj);
      obj_to_room(obj, ch->in_room);
      return;
  }

  if (obj->timer < 1) 
  {
    obj->timer = 30;
    act("$p becomes transparent.", ch, obj, NULL, TO_CHAR);
    act("$p becomes transparent.", ch, obj, NULL, TO_ROOM);
  }
}

bool sac_prog_cabal_item(OBJ_DATA *obj, CHAR_DATA *ch)
{
  OBJ_DATA *container;
  char buf[160];
  int i;

  act("The gods are infuriated!",ch,NULL,NULL,TO_CHAR);
  act("The gods are infuriated!",ch,NULL,NULL,TO_ROOM);
  damage(ch,ch,(int)(ch->hit/10),TYPE_HIT,DAM_HOLY, TRUE); 
  ch->gold = 0;

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
    sprintf( buf, "You see %s forming again slowly.\n\r", 
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
    bug( "oprog: Sac_cabal_item: Was not the cabal's item.", 0);
  }

  return FALSE; 
} 

void speech_prog_kassandra(OBJ_DATA *obj, CHAR_DATA *ch, char *speech)
{
  if (!str_cmp(speech, "kassandra") && (get_hold_char(ch) == obj)
           && !IS_NPC(ch))
    obj_cast_spell(gsn_kassandra,ch->level,ch,ch,NULL);

  else if (!str_cmp(speech, "sebat") && (get_hold_char(ch) == obj)
           && !IS_NPC(ch))
    obj_cast_spell(gsn_sebat,ch->level,ch,ch,NULL);

  else if (!str_cmp(speech, "matandra") && (get_hold_char(ch) == obj)
	   && (ch->fighting) && !IS_NPC(ch))
    {
      act("A blast of energy bursts from your hand toward $N!",
	  ch,NULL,ch->fighting,TO_CHAR);
      act("A blast of energy bursts from $n's hand toward you!",
	  ch,NULL,ch->fighting,TO_VICT);
      act("A blast of energy bursts from $n's hand toward $N!",
	  ch,NULL,ch->fighting,TO_NOTVICT);
      obj_cast_spell(gsn_matandra,ch->level,ch,ch->fighting,NULL);
    }
}
      
void fight_prog_chaos_blade(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if ( is_wielded_char(ch,obj) )
    switch(number_bits(7)) {
    case 0:
      
      act("The chaotic blade trembles violently!", ch, NULL, NULL, TO_ROOM);
      send_to_char("Your chaotic blade trembles violently!\n\r", ch);
      obj_cast_spell(gsn_mirror,ch->level,ch,ch,obj); 
      WAIT_STATE(ch, 2 * PULSE_VIOLENCE);
      break;

    case 1:

      act("The chaotic blade shakes a bit.", ch, NULL, NULL, TO_ROOM);
      send_to_char("Your chaotic blade shakes a bit.\n\r", ch);
      obj_cast_spell(gsn_garble,ch->level,ch,ch->fighting,obj);
      WAIT_STATE(ch, 2 * PULSE_VIOLENCE);
      break;

    case 2:

      act("The chaotic blade shivers uncontrollably!",ch,NULL,NULL,TO_ROOM);
      send_to_char("Your chaotic blade shivers uncontrollably!\n\r",ch);
      obj_cast_spell(gsn_confuse, ch->level,ch,ch->fighting,obj);
      WAIT_STATE(ch, 2 * PULSE_VIOLENCE);
      break;

    }
}

bool death_prog_chaos_blade(OBJ_DATA *obj, CHAR_DATA *ch)
{
  send_to_char("Your chaotic blade disappears.\n\r",ch);
  act("$n's chaotic blade disappears.",ch,NULL,NULL,TO_ROOM);
  extract_obj(obj);
  return FALSE;
}

void fight_prog_tattoo_apollon(OBJ_DATA *obj, CHAR_DATA *ch)
{
  int sn;

  if (get_eq_char(ch, WEAR_TATTOO) == obj)
    switch(number_bits(6)) {
    case 0:
    case 1:
      act_color("$CThe tattoo on your shoulder glows blue.$c",
		   ch,NULL,NULL,TO_CHAR,POS_DEAD,CLR_CYAN);
      obj_cast_spell(gsn_cure_serious, ch->level, ch, ch, obj);
      break;
    case 2:
      act_color("$CThe tattoo on your shoulder glows red.$c",
		   ch,NULL,NULL,TO_CHAR,POS_DEAD,CLR_RED);
      do_yell( ch, "Ever dance with good....");
      sn = skill_lookup("holy word");
      spell_holy_word(sn,ch->level,ch,NULL,TARGET_CHAR);
      break;
    }
}


void fight_prog_tattoo_zeus(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (get_eq_char(ch, WEAR_TATTOO) == obj)
    switch(number_bits(6)) {
    case 0:
    case 1:
    case 2:
      act_color("$CThe tattoo on your shoulder glows blue.$c",
		   ch,NULL,NULL,TO_CHAR,POS_DEAD,CLR_CYAN);
      obj_cast_spell(gsn_cure_critical, ch->level, ch, ch, obj);
      break;
    case 3:
      act_color("$CThe tattoo on your shoulder glows blue.$c",
		   ch,NULL,NULL,TO_CHAR,POS_DEAD,CLR_CYAN);
      if (IS_AFFECTED(ch,AFF_PLAGUE))	
	spell_cure_disease(25,100,ch,ch,TARGET_CHAR);
      if (IS_AFFECTED(ch,AFF_POISON))	
        spell_cure_poison(27,100,ch,ch,TARGET_CHAR);
      break;
    }
}

void fight_prog_tattoo_siebele(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (get_eq_char(ch, WEAR_TATTOO) == obj)
    switch(number_bits(6)) {
    case 0:
      act_color("$CThe tattoo on your shoulder glows blue.$c",
		   ch,NULL,NULL,TO_CHAR,POS_DEAD,CLR_CYAN);
      obj_cast_spell(gsn_cure_serious, ch->level, ch,ch, obj);
      break;
    case 1:
      act_color("$CThe tattoo on your shoulder glows red.$c",
		   ch,NULL,NULL,TO_CHAR,POS_DEAD,CLR_RED);
      spell_bluefire(gsn_dispel_good, ch->level, ch, ch->fighting, TARGET_CHAR);
      break;
    }
}

void fight_prog_tattoo_ahrumazda(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (get_eq_char(ch, WEAR_TATTOO) == obj)
    switch(number_bits(6)) {
    case 0:
      act_color("$CThe tattoo on your shoulder glows blue.$c",
		   ch,NULL,NULL,TO_CHAR,POS_DEAD,CLR_BLUE);
      obj_cast_spell(gsn_cure_serious, ch->level, ch, ch, obj); 
      break;
    case 1:
      act_color("$CThe tattoo on your shoulder glows red.$c",
		   ch,NULL,NULL,TO_CHAR,POS_DEAD,CLR_RED);
      obj_cast_spell(gsn_demonfire, ch->level, ch, ch->fighting, obj);
      break;
    }
}

 void fight_prog_tattoo_hephaestus(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (get_eq_char(ch, WEAR_TATTOO) == obj)
    switch(number_bits(6)) {
    case 0:
    case 1:
      act_color("$CThe tattoo on your shoulder glows blue.$c",
		   ch,NULL,NULL,TO_CHAR,POS_DEAD,CLR_CYAN);
      obj_cast_spell(gsn_cure_serious, ch->level, ch, ch, obj);
      break;
    case 2:
      act_color("$CThe tattoo on your shoulder glows red.$c",
		   ch,NULL,NULL,TO_CHAR,POS_DEAD,CLR_RED);
      do_yell(ch,"And justice for all!....");
      spell_scream(gsn_scream,ch->level,ch,ch->fighting,TARGET_CHAR);
      break;
    }
}

void fight_prog_tattoo_ehrumen(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (get_eq_char(ch, WEAR_TATTOO) == obj)
    switch(number_bits(6)) {
    case 0:
      act_color("$CThe tattoo on your shoulder glows red.$c",
		   ch,NULL,NULL,TO_CHAR,POS_DEAD,CLR_RED);
      obj_cast_spell(gsn_cure_light, ch->level, ch, ch->fighting, obj);
      break;
    case 1:
      act_color("$CThe tattoo on your shoulder glows blue.$c",
		   ch,NULL,NULL,TO_CHAR,POS_DEAD,CLR_CYAN);
      obj_cast_spell(gsn_cure_serious, ch->level, ch, ch, obj);
      break;
    case 2:
      act_color("$CThe tattoo on your shoulder glows red.$c",
		   ch,NULL,NULL,TO_CHAR,POS_DEAD,CLR_RED);
      spell_dispel_evil(gsn_dispel_evil, ch->level, ch, ch->fighting,TARGET_CHAR);
      break;
    }
}

void fight_prog_tattoo_venus(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (get_eq_char(ch, WEAR_TATTOO) == obj)
    switch(number_bits(7)) {
    case 0:
    case 1:
    case 2:
      act_color("$CThe tattoo on your shoulder glows blue.$c",
		   ch,NULL,NULL,TO_CHAR,POS_DEAD,CLR_CYAN);
      obj_cast_spell(gsn_cure_light, ch->level, ch, ch, obj);
      break;
    case 3:
      act_color("$CThe tattoo on your shoulder glows red.$c",
		   ch,NULL,NULL,TO_CHAR,POS_DEAD,CLR_RED);
      obj_cast_spell(gsn_plague, ch->level, ch, ch->fighting, obj);
      break;
    case 4:
      act_color("$CThe tattoo on your shoulder glows blue.$c",
		   ch,NULL,NULL,TO_CHAR,POS_DEAD,CLR_CYAN);
      obj_cast_spell(gsn_bless, ch->level, ch, ch, obj);
      break;
    }
}

void fight_prog_tattoo_ares(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (get_eq_char(ch, WEAR_TATTOO) == obj)
    switch(number_bits(5)) {
    case 0:
      act_color("$CThe tattoo on your shoulder glows blue.$c",
		   ch,NULL,NULL,TO_CHAR,POS_DEAD,CLR_CYAN);
      obj_cast_spell(gsn_dragon_strength, ch->level, ch, ch, obj);
      break;
    case 1:
      act_color("$CThe tattoo on your shoulder glows RED.$c",
		   ch,NULL,NULL,TO_CHAR,POS_DEAD,CLR_RED);
      obj_cast_spell(gsn_dragon_breath, ch->level, ch, ch->fighting, obj);
      break;
    }
}


void fight_prog_tattoo_odin(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (get_eq_char(ch, WEAR_TATTOO) == obj)
    switch(number_bits(5)) {
    case 0:
      act_color("$CThe tattoo on your shoulder glows blue.$c",
		   ch,NULL,NULL,TO_CHAR,POS_DEAD,CLR_CYAN);
      obj_cast_spell(gsn_cure_critical, ch->level, ch, ch, obj);
      break;
    case 1:
      act_color("$CThe tattoo on your shoulder glows red.$c",
		   ch,NULL,NULL,TO_CHAR,POS_DEAD,CLR_RED);
      obj_cast_spell(gsn_faerie_fire, ch->level, ch, ch->fighting, obj);
      break;
    }
}

void fight_prog_tattoo_phobos(OBJ_DATA *obj, CHAR_DATA *ch)
{
  int sn;

  if (get_eq_char(ch, WEAR_TATTOO) == obj)
    switch(number_bits(6)) {
    case 0:
      act_color("$CThe tattoo on your shoulder glows blue.$c",
		   ch,NULL,NULL,TO_CHAR,POS_DEAD,CLR_CYAN);
      obj_cast_spell(gsn_cure_serious, ch->level, ch, ch, obj);
      break;
    case 1:
      if ((sn = skill_lookup("colour spray")) < 0) break;
      act_color("$CThe tattoo on your shoulder glows red.$c",
		   ch,NULL,NULL,TO_CHAR,POS_DEAD,CLR_RED);
      obj_cast_spell(sn, ch->level, ch, ch->fighting, obj);
      break;
    }
}

void fight_prog_tattoo_mars(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (get_eq_char(ch, WEAR_TATTOO) == obj)
    switch(number_bits(7)) {
    case 0:
      obj_cast_spell(gsn_blindness, ch->level, ch, ch->fighting, obj);
      send_to_char("You send out a cloud of confusion!\n\r", ch);
      break;
    case 1:
      obj_cast_spell(gsn_poison, ch->level, ch, ch->fighting, obj);
      send_to_char("Some of your insanity rubs off on your opponent.\n\r", ch);
      break;
    case 2:
      obj_cast_spell(gsn_haste, ch->level, ch, ch, obj);
      send_to_char("You suddenly feel more hyperactive!\n\r", ch);
      break;
    case 3:
      obj_cast_spell(gsn_shield, ch->level, ch, ch, obj);
      send_to_char("You feel even more paranoid!\n\r", ch);
      break;
    }
}

void fight_prog_tattoo_athena(OBJ_DATA *obj, CHAR_DATA *ch)
{
  AFFECT_DATA af;

  if(get_eq_char(ch, WEAR_TATTOO) == obj)
    {
    if (number_percent() < 50)
      {
	switch(number_bits(4)) {
	case 0:
	  if (IS_AFFECTED(ch,AFF_BERSERK) || is_affected(ch,gsn_berserk)
	      ||  is_affected(ch,skill_lookup("frenzy")))
	    {
	      send_to_char("You get a little madder.\n\r",ch);
	      return;
	    }

	  af.where = TO_AFFECTS;
	  af.type = gsn_berserk;
	  af.level = ch->level;
	  af.duration = ch->level / 3;
	  af.modifier = ch->level / 5;
	  af.bitvector = AFF_BERSERK;

	  af.location = APPLY_HITROLL;
	  affect_to_char(ch, &af);

	  af.location = APPLY_DAMROLL;
	  affect_to_char(ch, &af);

	  af.modifier = 10 * (ch->level / 10);
	  af.location = APPLY_AC;
	  affect_to_char(ch, &af);
	  
	  ch->hit += ch->level * 2;
	  ch->hit = UMIN(ch->hit,ch->max_hit);
	  
	  send_to_char("Your pulse races as you are consumned by rage!\n\r",
		       ch);
	  act("$n gets a wild look in $s eyes.",ch,NULL,NULL,TO_ROOM);

	  break;
	}
      }
    else
      {
	switch(number_bits(4)) {
	case 0:
	  do_yell(ch, "Cry Havoc and Let Loose the Dogs of War!");
	  break;
	case 1:
	  do_yell(ch, "No Mercy!");
	  break;
	case 2:
	  do_yell(ch, "Los Valdar Cuebiyari!");
	  break;
	case 3:
	  do_yell(ch, "Carai an Caldazar! Carai an Ellisande! Al Ellisande!");
	  break;
	case 4:
	  do_yell(ch, "Siempre Vive el Riesgo!");
	  break;
	}
      }
    }
}


void fight_prog_tattoo_hera( OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (get_eq_char(ch, WEAR_TATTOO) == obj)
    switch(number_bits(5)) {
    case 0:
      act_color("$CThe tattoo on your shoulder glows red.$c",
		   ch,NULL,NULL,TO_CHAR,POS_DEAD,CLR_RED);
      obj_cast_spell(gsn_plague, ch->level, ch, ch->fighting, obj);
      break;
    case 1:
      act_color("$CThe tattoo on your shoulder glows red.$c",
		   ch,NULL,NULL,TO_CHAR,POS_DEAD,CLR_RED);
      obj_cast_spell(gsn_poison, ch->level, ch, ch->fighting, obj);
    case 2:
      act_color("$CThe tattoo on your shoulder glows red.$c",
		   ch,NULL,NULL,TO_CHAR,POS_DEAD,CLR_RED);
      obj_cast_spell(gsn_weaken, ch->level, ch, ch->fighting, obj);
    case 3:
      act_color("$CThe tattoo on your shoulder glows red.$c",
		   ch,NULL,NULL,TO_CHAR,POS_DEAD,CLR_RED);
      obj_cast_spell(gsn_slow, ch->level, ch, ch->fighting, obj);
      break;
    }
}


void fight_prog_tattoo_deimos(OBJ_DATA *obj, CHAR_DATA *ch)
{
  int sn;
  if (get_eq_char(ch, WEAR_TATTOO) == obj)
    switch(number_bits(6)) {
    case 0:
    case 1:
      act_color("$CThe tattoo on your shoulder glows blue.$c",
		   ch,NULL,NULL,TO_CHAR,POS_DEAD,CLR_CYAN);
      obj_cast_spell(gsn_cure_serious, ch->level, ch, ch, obj);
      break;
    case 2:
      act_color("$CThe tattoo on your shoulder glows red.$c",
		   ch,NULL,NULL,TO_CHAR,POS_DEAD,CLR_RED);
      sn = skill_lookup("web");
      spell_web(sn, ch->level, ch, ch->fighting,TARGET_CHAR);
      break;
    }
}


void fight_prog_tattoo_eros(OBJ_DATA *obj, CHAR_DATA *ch)
{
  int sn;
  if (get_eq_char(ch, WEAR_TATTOO) == obj)
    switch(number_bits(5)) {
    case 0:
    case 1:
      if ((sn = skill_lookup("heal")) < 0) break;
      act_color("$CThe tattoo on your shoulder glows blue.$c",
		   ch,NULL,NULL,TO_CHAR,POS_DEAD,CLR_CYAN);
      obj_cast_spell(sn, ch->level, ch, ch, obj);
      break;
    case 2:
      if ((sn = skill_lookup("mass healing")) < 0) break;
      act_color("$CThe tattoo on your shoulder glows blue.$c",
		   ch,NULL,NULL,TO_CHAR,POS_DEAD,CLR_CYAN);
      obj_cast_spell(sn, ch->level, ch, ch, obj);
      break;
    }
}


bool death_prog_golden_weapon(OBJ_DATA *obj, CHAR_DATA *ch)
{
  send_to_char("Your golden weapon disappears.\n\r",ch);
  act("$n's golden weapon disappears.",ch,NULL,NULL,TO_ROOM);
  extract_obj(obj);
  ch->hit = 1;
  while ( ch->affected )
    affect_remove( ch, ch->affected );
  ch->last_fight_time = -1;
  ch->last_death_time = current_time;
  if (cabal_area_check(ch))
  {
    act("$n disappears.",ch,NULL,NULL,TO_ROOM);
    char_from_room(ch);
    char_to_room(ch, get_room_index( cabal_table[CABAL_KNIGHT].room_vnum )); 
    act("$n appears in the room.",ch,NULL,NULL,TO_ROOM);
  }
  return TRUE; 
}

void fight_prog_golden_weapon(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if ( is_wielded_char(ch,obj) )
    {
      if (number_percent() < 4)
	{
	  act("Your $p glows bright blue!\n\r",ch, obj, NULL, TO_CHAR);
	  act("$n's $p glows bright blue!",ch,obj,NULL,TO_ROOM);
	  
	  obj_cast_spell(gsn_cure_critical,ch->level,ch,ch,obj);
	  return;
	}
      else if (number_percent() > 92)
	{
	  act("Your $p glows bright blue!\n\r",ch, obj, NULL, TO_CHAR);
	  act("$n's $p glows bright blue!",ch,obj,NULL,TO_ROOM);
	  
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
      switch(number_bits(7)) {
      case 0:
  	act("One of the snake heads on your whip bites $N!", ch, NULL,
		ch->fighting, TO_CHAR);
	act("A snake from $n's whip strikes out and bites you!", ch, NULL,
		ch->fighting, TO_VICT);
	act("One of the snakes from $n's whip strikes at $N!", ch, NULL, 
		ch->fighting, TO_NOTVICT);
	obj_cast_spell(gsn_poison, ch->level, ch, ch->fighting, obj);
	break;
      case 1:
	act("One of the snake heads on your whip bites $N!", ch, NULL,
		ch->fighting, TO_CHAR);
	act("A snake from $n's whip strikes out and bites you!", ch, NULL,
		ch->fighting, TO_VICT);
	act("One of the snakes from $n's whip strikes at $N!", ch, NULL,
		ch->fighting, TO_NOTVICT);
	obj_cast_spell(gsn_weaken, ch->level, ch, ch->fighting, obj);
	break;
      }
    }
}

void fight_prog_tattoo_prometheus(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (get_eq_char(ch, WEAR_TATTOO) == obj)
    switch(number_bits(5)) {
    case 0:
      act_color("$CThe tattoo on your shoulder glows blue.$c",
		   ch,NULL,NULL,TO_CHAR,POS_DEAD,CLR_CYAN);
      obj_cast_spell(gsn_cure_critical, ch->level, ch, ch, obj);
      break;
    case 1:
    case 2:
      act_color("$CThe tattoo on your shoulder glows red.$c",
		   ch,NULL,NULL,TO_CHAR,POS_DEAD,CLR_RED);
      if (IS_EVIL(ch->fighting))
      spell_dispel_evil(gsn_dispel_evil,(int)(1.2* (float)ch->level), ch, ch->fighting,TARGET_CHAR);
      else if (IS_GOOD(ch->fighting))
      spell_dispel_good(gsn_dispel_good,(int)(1.2* (float)ch->level), ch, ch->fighting,TARGET_CHAR);
      else 
      spell_lightning_bolt(64, (int)(1.2* (float)ch->level), ch, ch->fighting, TARGET_CHAR);
      break;
    }
}


void fight_prog_shockwave(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if ( is_wielded_char(ch,obj) )
    switch(number_bits(5)) {
    case 0:
      act("A bolt of lightning arcs out from your bolt, hitting $N!", ch, 
	NULL, ch->fighting, TO_CHAR);
      act("A bolt of lightning crackles along $n's bolt and arcs towards you!",
	ch, NULL, ch->fighting, TO_VICT);
      act("A bolt of lightning shoots out from $n's bolt, arcing towards $N!",
	ch, NULL, ch->fighting, TO_NOTVICT);
      obj_cast_spell(gsn_lightning_bolt, ch->level, ch, ch->fighting, NULL);
      break;
    }
}

void wear_prog_ranger_staff(OBJ_DATA *obj, CHAR_DATA *ch)
{

  if ( ch->iclass != CLASS_RANGER )
  {
     send_to_char( "You don't know to use this thing.\n\r", ch );
     unequip_char( ch, obj );
     send_to_char( "Ranger staff slides off from your hand.\n\r", ch );
     obj_from_char( obj );
     obj_to_room( obj, ch->in_room );
     return;
  }

}

void wear_prog_coconut(OBJ_DATA *obj, CHAR_DATA *ch)
{
  act("You start to bang the coconut shells together.",ch,NULL,NULL,TO_CHAR);
  act("You hear a sound like horses galloping and you mount your steed.", 
	ch, NULL, NULL, TO_CHAR);
  act("$n pretends to mount an invisible horse.",
	ch,NULL,NULL,TO_ROOM); 
}

void entry_prog_coconut(OBJ_DATA *obj)
{
  if (obj->carried_by != NULL)
    if (get_hold_char(obj->carried_by) == obj)
  act("$n gallops in on his invisible steed, banging two coconuts together.",
	obj->carried_by, NULL, NULL, TO_ROOM);
}  

void greet_prog_coconut(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (obj->carried_by != NULL)
    {
      if (get_hold_char(obj->carried_by) == obj && 
		obj->carried_by != ch)
	act("You hear the sound of galloping horses.", ch, NULL, NULL, TO_CHAR);
    }
  else
    send_to_char("$p beckons with the faint sound of galloping horses.\n\r",
	ch);
}

void get_prog_coconut(OBJ_DATA *obj, CHAR_DATA *ch)
{
  send_to_char("You hold the coconut up to your ear and suddenly you hear the faint\n\rroar of galloping horses.\n\r", ch);
  act("$n holds a coconut up to $s ear.", ch, NULL, NULL, TO_ROOM);
}

void remove_prog_coconut(OBJ_DATA *obj, CHAR_DATA *ch)
{
  send_to_char("The sounds of horses fade away.\n\r", ch);
  act("$n pretends to dismount a horse.", ch, NULL, NULL, TO_ROOM);
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
	act( "Your gauntlets burns $N's face!", ch, NULL, ch->fighting, TO_CHAR);
	act( "$n's gauntlets burns $N's face!", ch, NULL, ch->fighting, TO_NOTVICT);
	act( "$N's gauntlets burns your face!", ch->fighting, NULL, ch, TO_CHAR);
	damage( ch, ch->fighting, dam/2, gsn_burning_hands, DAM_FIRE, TRUE);
	if ( ch == NULL || ch->fighting == NULL )
	  return;
	fire_effect( ch->fighting, obj->level/2, dam/2, TARGET_CHAR );
  }
  return;
}

void wear_prog_firegauntlets(OBJ_DATA *obj, CHAR_DATA *ch)
{
	send_to_char( "Your hands warm up by the gauntlets.\n\r", ch );
	return;
}

void remove_prog_firegauntlets(OBJ_DATA *obj, CHAR_DATA *ch)
{
	send_to_char( "Your hands cool down.\n\r", ch );
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
	act( "Your armbands burns $N's face!", ch, NULL, ch->fighting, TO_CHAR);
	act( "$n's armbands burns $N's face!", ch, NULL, ch->fighting, TO_NOTVICT);
	act( "$N's armbands burns your face!", ch->fighting, NULL, ch, TO_CHAR);
	damage( ch, ch->fighting, dam, gsn_burning_hands, DAM_FIRE, TRUE);
	if ( ch == NULL || ch->fighting == NULL )
	  return;
	fire_effect( ch->fighting, obj->level/2, dam, TARGET_CHAR );
  }
  return;
}

void wear_prog_armbands(OBJ_DATA *obj, CHAR_DATA *ch)
{
	send_to_char( "Your arms warm up by the armbands of the volcanoes.\n\r", ch );
	return;
}

void remove_prog_armbands(OBJ_DATA *obj, CHAR_DATA *ch)
{
	send_to_char( "Your arms cool down again.\n\r", ch );
	return;
}

void fight_prog_demonfireshield( OBJ_DATA *obj, CHAR_DATA *ch )
{
int dam;

  if ( get_shield_char( ch ) != obj )
	return;

  if ( number_percent() < 15 )  {
	dam = number_percent()/2 + 5 * ch->level;
	act( "A magical hole appears in your shield !", ch, NULL, ch->fighting, TO_CHAR);
	act( "Your shield burns $N's face!", ch, NULL, ch->fighting, TO_CHAR);
	act( "$n's shield burns $N's face!", ch, NULL, ch->fighting, TO_NOTVICT);
	act( "$N's shield burns your face!", ch->fighting, NULL, ch, TO_CHAR);
	fire_effect( ch->fighting, obj->level,dam, TARGET_CHAR );
	damage( ch, ch->fighting, dam, gsn_demonfire, DAM_FIRE, TRUE);
  }
  return;
}

void wear_prog_demonfireshield(OBJ_DATA *obj, CHAR_DATA *ch)
{
	send_to_char( "Your hands warm up by the fire shield.\n\r", ch );
	return;
}

void remove_prog_demonfireshield(OBJ_DATA *obj, CHAR_DATA *ch)
{
	send_to_char( "Your hands cool down.\n\r", ch );
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
    send_to_char( 
	"Your weapon swings at your victim's neck without your control!\n\r", ch);
    if ( number_percent() < 20 )  {
	act( "It makes an huge arc in the air, chopping $N's head OFF!",
	     ch, NULL, victim, TO_CHAR);
	act( "$N's weapon whistles in the air, chopping your head OFF!",
	     ch, NULL, victim, TO_NOTVICT);
	act( "$n's weapon whistles in the air, chopping $N's head OFF!",
	     ch, NULL, victim, TO_ROOM);
	act( "$n is DEAD!!", victim, NULL, NULL, TO_ROOM );
	act( "$n is DEAD!!", victim, NULL, NULL, TO_CHAR );
	raw_kill_org( victim, 3 );
	send_to_char( "You have been KILLED!!\n\r", victim );
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
      send_to_char("As you wear wind boots on your feet, they hold you up.\n\r", ch);
      send_to_char("You start to fly.\n\r", ch);

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
      send_to_char("You fall down to the ground.\n\r", ch);
      send_to_char("Ouch!.\n\r", ch);
    }
}
void wear_prog_boots_flying(OBJ_DATA *obj, CHAR_DATA *ch)
{
  AFFECT_DATA af;

  if (!is_affected(ch, gsn_fly))
    {
      send_to_char("As you wear boots of flying on your feet, they hold you up.\n\r", ch);
      send_to_char("You start to fly.\n\r", ch);

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
      send_to_char("You fall down to the ground.\n\r", ch);
      send_to_char("You start to walk again instead of flying!.\n\r", ch);
    }
}


void wear_prog_arm_hercules(OBJ_DATA *obj, CHAR_DATA *ch)
{
  AFFECT_DATA af;

  if (!is_affected(ch, gsn_giant_strength))
    {
      send_to_char("As you wear your arms these plates, You feel your self getting stronger.\n\r", ch);
      send_to_char("Your muscles seems incredibly huge.\n\r", ch);

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
      send_to_char("Your muscles regain its original value.\n\r", ch);
    }
}

void wear_prog_girdle_giant(OBJ_DATA *obj, CHAR_DATA *ch)
{
  AFFECT_DATA af;

  if (!is_affected(ch, gsn_giant_strength))
    {
      send_to_char("As you wear this girdle, You feel your self getting stronger.\n\r", ch);
      send_to_char("Your muscles seems incredibly huge.\n\r", ch);

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
      send_to_char("Your muscles regain its original value.\n\r", ch);
    }
}
void wear_prog_breastplate_strength(OBJ_DATA *obj, CHAR_DATA *ch)
{
  AFFECT_DATA af;

  if (!is_affected(ch, gsn_giant_strength))
    {
      send_to_char("As you wear breastplate of strength, You feel yourself getting stronger.\n\r", ch);
      send_to_char("Your muscles seems incredibly huge.\n\r", ch);

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
      send_to_char("Your muscles regain its original value.\n\r", ch);
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

  send_to_char("The leaves of your shield grows suddenly.\n\r",ch);
  send_to_char("The leaves of shield surrounds you!.\n\r",ch->fighting);
  act("$n's shield of rose grows suddenly.",ch,NULL,NULL,TO_ROOM);
  obj_cast_spell(gsn_slow,ch->level,ch,ch->fighting,NULL);
  return;
}

void fight_prog_lion_claw(OBJ_DATA *obj, CHAR_DATA *ch)
{
 if ( number_percent() < 90 )  return;

 if ( is_wielded_char(ch,obj) )
 {
  send_to_char("The nails of your claw appears form its fingers.\n\r",ch);
  act_color("the nails of $n's claw appears for an instant.",
		ch,NULL,NULL,TO_ROOM,POS_DEAD,CLR_WHITE);
  one_hit(ch,ch->fighting,TYPE_HIT,FALSE);
  one_hit(ch,ch->fighting,TYPE_HIT,FALSE);
  one_hit(ch,ch->fighting,TYPE_HIT,FALSE);
  one_hit(ch,ch->fighting,TYPE_HIT,FALSE);
  send_to_char("The nails of your claw disappears.\n\r",ch);
  act_color("the nails of $n's claw disappears suddenly.",
	ch,NULL,NULL,TO_ROOM,POS_DEAD,CLR_WHITE);
  return;
}
 return;
}


void speech_prog_ring_ra(OBJ_DATA *obj, CHAR_DATA *ch, char *speech)
{

  if (!str_cmp(speech, "punish")   
      && (ch->fighting) && is_equiped_char(ch,obj,WEAR_FINGER) )
    {
      send_to_char("An electrical arc sprays from the ring.\n\r",ch);
      act("An electrical arc sprays from the ring.",ch,NULL,NULL,TO_ROOM);
      obj_cast_spell(gsn_lightning_breath,ch->level,ch,ch->fighting,NULL);
      WAIT_STATE(ch, 2 * PULSE_VIOLENCE);
    }
 return;
}

void wear_prog_eyed_sword(OBJ_DATA *obj, CHAR_DATA *ch)
{
  act("$p's eye opens.",ch,obj,NULL,TO_CHAR);
  act("$p's eye opens.",ch,obj,NULL,TO_ROOM);
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
	&& strstr( obj->extra_descr->description, ch->name ) != NULL)
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
   send_to_char("You feel your katana like a part of you!\n\r",ch);
  }
  return;
}

void fight_prog_tattoo_goktengri(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (get_eq_char(ch, WEAR_TATTOO) == obj)
    switch(number_bits(4)) {
    case 0:
    case 1:
      act_color("$CThe tattoo on your shoulder glows white.$c",
		   ch,NULL,NULL,TO_CHAR,POS_DEAD,CLR_WHITE);
      do_say(ch,"My honour is my life.");
      one_hit(ch,ch->fighting,TYPE_UNDEFINED,FALSE);
      break;
    }
}


void wear_prog_snake(OBJ_DATA *obj, CHAR_DATA *ch)
{
  act_color("$CSnakes of whip starts to breath a poisonous air.$c",
		ch,obj,NULL,TO_CHAR,POS_DEAD,CLR_GREEN);
  act_color("$CSnakes of whip starts to breath a poisonous air.$c",
		ch,obj,NULL,TO_ROOM,POS_DEAD,CLR_GREEN);
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
  act_color("$CSnakes of whip slowly melds to non-living skin.$c",
		ch,obj,NULL,TO_CHAR,POS_DEAD,CLR_RED);
  act_color("$CSnakes of whip slowy melds to non-living skin.$c",
		ch,obj,NULL,TO_ROOM,POS_DEAD,CLR_RED);
}

void get_prog_snake(OBJ_DATA *obj, CHAR_DATA *ch) 
{
  act("You feel as if snakes of whip moved.",ch,obj,NULL,TO_CHAR);
}

void wear_prog_fire_shield(OBJ_DATA *obj, CHAR_DATA *ch)
{
  AFFECT_DATA af;

  if ( strstr( obj->extra_descr->description, "cold" ) != NULL )  
   {
    if (!is_affected(ch, gsn_fire_shield))
    {
      send_to_char("As you wear shield, you become resistive to cold.\n\r", ch);

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
      send_to_char("As you wear shield, you become resistive to fire.\n\r", ch);

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
      if ( strstr( obj->extra_descr->description, "cold" ) != NULL )  
       send_to_char("You have become normal to cold attacks.\n\r", ch);
      else  send_to_char("You have become normal to fire attacks.\n\r", ch);
    }
}


void wear_prog_quest_weapon(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if ( strstr( obj->short_descr, ch->name ) != NULL )  
  {
    send_ch_color("$CYour weapon starts glowing.$c",ch,POS_SLEEPING,CLR_BLUE);
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

  act( "You are zapped by $p and drop it.", ch, obj, NULL, TO_CHAR );

  obj_from_char( obj );
  obj_to_room( obj, ch->in_room );
  return;
}


void get_prog_quest_reward(OBJ_DATA *obj, CHAR_DATA *ch) 
{
  if ( strstr( obj->short_descr, ch->name ) != NULL )  
  {
    act_color("$CYour $p starts glowing.\n\r$c",
		ch,obj,NULL,TO_CHAR,POS_SLEEPING,CLR_BLUE);
    return;
  }

  act( "You are zapped by $p and drop it.", ch, obj, NULL, TO_CHAR );

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
   act( "As you touch $N, the flame within your hands blows UP on $N!", 
	ch, NULL, ch->fighting,TO_CHAR);
   act( "As $n touches $N, the flame within $s hands blows UP on $N!",
	ch, NULL, ch->fighting,TO_NOTVICT);
   act( "As $N touches you, the flame within $S hands blows UP on YOU!"
	, ch->fighting, NULL,ch,TO_CHAR);
   fire_effect( ch->fighting, obj->level,dam, TARGET_CHAR );
   damage( ch, ch->fighting, dam, gsn_burning_hands, DAM_FIRE, TRUE);
 }
  return;
}


void remove_prog_ancient_gloves(OBJ_DATA *obj, CHAR_DATA *ch)
{
        send_to_char("The flame within your hands disappears.\n\r", ch );
        return;
}

void wear_prog_ancient_gloves(OBJ_DATA *obj, CHAR_DATA *ch)
{
        send_to_char( "A flame starts to burn within your hands!\n\r", ch );
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
   act("Your shield SHIMMERS brightly!", ch, NULL,ch->fighting,TO_CHAR);
   act("$n's shield SHIMMERS brightly!", ch, NULL,ch->fighting,TO_VICT);
   act("$n's shield SHIMMERS brightly!", ch, NULL,ch->fighting,TO_NOTVICT);
   fire_effect( ch->fighting, obj->level/2, dam, TARGET_CHAR );
   damage( ch, ch->fighting, dam, gsn_fire_breath, DAM_FIRE, TRUE);
 }
 else if ( chance < 10 ) 
 {
   act("Your shield shines with a bright red aura!", 
	ch, NULL,ch->fighting,TO_CHAR);
   act("$n's shield shine with a bright red aura!",
	 ch, NULL,ch->fighting,TO_VICT);
   act("$n's shield shines with a bright red aura!", 
	ch, NULL,ch->fighting,TO_NOTVICT);
   obj_cast_spell(gsn_blindness, ch->level+ 5, ch, ch->fighting,obj);
   obj_cast_spell(gsn_slow, ch->level + 5, ch, ch->fighting, obj);
 }
 return;
}

void remove_prog_ancient_shield(OBJ_DATA *obj, CHAR_DATA *ch)
{
  act_color("$CYour shield returns to its original form.$c",
                ch,obj,NULL,TO_CHAR,POS_DEAD,CLR_RED);
  act_color("$CYour shield returns to its original form.$c",
                ch,obj,NULL,TO_ROOM,POS_DEAD,CLR_RED);
  return;

}

void wear_prog_ancient_shield(OBJ_DATA *obj, CHAR_DATA *ch)
{
  act_color(
 "$CYour shield changes its shape and surrounds itself with dragon skin.\n\r"
 "A dragon head gets born on the upper surface of the shield and opens its mouth!$c",
                ch,obj,NULL,TO_CHAR,POS_DEAD,CLR_RED);
  act_color(
 "$C$n's shield changes its shape and surrounds itself with dragon skin.\n\r"
 "A dragon head gets born on the upper surface of the shield and opens its mouth!$c",
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
