/***************************************************************************
 *                                                                         *
 * Uzak Diyarlar açýk kaynak Türkçe Mud projesidir.                        *
 * Oyun geliþtirmesi Jai ve Maru tarafýndan yönetilmektedir.               *
 * Unutulmamasý gerekenler: Nir, Kame, Nyah, Sint                          *
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
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,	   *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *									   *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael	   *
 *  Chastain, Michael Quan, and Mitchell Tse.				   *
 *									   *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc	   *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.						   *
 *									   *
 *  Much time and thought has gone into this software and you are	   *
 *  benefitting.  We hope that you share your changes too.  What goes	   *
 *  around, comes around.						   *
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
#include <sys/time.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "merc.h"
#include "magic.h"

DECLARE_DO_FUN(	do_say	);

void do_heal(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *mob;
    char arg[MAX_INPUT_LENGTH];
    int cost,sn;
    SPELL_FUN *spell;
    const char *words;

    /* check for healer */
    for ( mob = ch->in_room->people; mob; mob = mob->next_in_room )
    {
        if ( IS_NPC(mob) && IS_SET(mob->act, ACT_IS_HEALER))
	 {
	  if (ch->cabal && (is_name((char*)"cabal",mob->name)||is_name((char*)"kabal",mob->name)))
		{
    		 if (is_name((char*)cabal_table[ch->cabal].short_name,mob->name) )
		 	break;
		 else continue;
		}
          else  break;
	 }
    }

    if ( mob == NULL )
    {
      send_to_char( "Burada yapamazsýn.\n\r", ch );
        return;
    }

    if ( ch->cabal == CABAL_BATTLE )
    {
      send_to_char( "Sen Öfke Kabalý üyesisin, pis bir büyücü deðil!\n\r",ch );
        return;
    }

    one_argument(argument,arg);

    if (arg[0] == '\0')
    {
        /* display price list */
        act("Þifacý þu hizmetleri veriyor.",ch,NULL,mob,TO_CHAR);
      	send_to_char("  hafif       : hafif yara tedavisi   100 akçe\n\r",ch);
      	send_to_char("  ciddi       : ciddi yara tedavisi   150 akçe\n\r",ch);
      	send_to_char( "  kritik      : kritik yara tedavisi  250 akçe\n\r",ch);
      	send_to_char( "  þifa        : þifa büyüsü           500 akçe\n\r",ch);
      	send_to_char( "  körlük      : körlük tedavisi       200 akçe\n\r",ch);
      	send_to_char( "  hastalýk    : hastalýk tedavisi     150 akçe\n\r",ch);
      	send_to_char( "  zehir       : zehir tedavisi        250 akçe\n\r",ch);
      	send_to_char( "  lanet       : lanet kaldýrma        500 akçe\n\r",ch);
      	send_to_char( "  yenileme    : yorgunluk azaltma      50 akçe\n\r",ch);
      	send_to_char( "  mana        : mana yenileme         100 akçe\n\r",ch);
      	send_to_char( "  yüksek þifa : yüksek tedavi        2000 akçe\n\r",ch);
      	send_to_char( "  takat       : ileri mana tedavi    2000 akçe\n\r",ch);
      	send_to_char( " Hizmet almak için: iyileþ <tip>\n\r",ch);
	return;
    }

    if (!str_prefix(arg,"hafif"))
    {
        spell = spell_cure_light;
	sn    = skill_lookup("cure light");
	words = "judicandus dies";
	 cost  = 100;
    }

    else if (!str_prefix(arg,"ciddi"))
    {
	spell = spell_cure_serious;
	sn    = skill_lookup("cure serious");
	words = "judicandus gzfuajg";
	cost  = 150;
    }

    else if (!str_prefix(arg,"kritik"))
    {
	spell = spell_cure_critical;
	sn    = skill_lookup("cure critical");
	words = "judicandus qfuhuqar";
	cost  = 250;
    }

    else if (!str_prefix(arg,"þifa"))
    {
	spell = spell_heal;
	sn = skill_lookup("heal");
	words = "pzar";
	cost  = 500;
    }

    else if (!str_prefix(arg,"körlük"))
    {
	spell = spell_cure_blindness;
	sn    = skill_lookup("cure blindness");
      	words = "judicandus noselacri";
        cost  = 200;
    }

    else if (!str_prefix(arg,"hastalýk"))
    {
	spell = spell_cure_disease;
	sn    = skill_lookup("cure disease");
	words = "judicandus eugzagz";
	cost = 150;
    }

    else if (!str_prefix(arg,"zehir"))
    {
	spell = spell_cure_poison;
	sn    = skill_lookup("cure poison");
	words = "judicandus sausabru";
	cost  = 250;
    }

    else if (!str_prefix(arg,"lanet") )
    {
	spell = spell_remove_curse;
	sn    = skill_lookup("remove curse");
	words = "candussido judifgz";
	cost  = 500;
    }

    else if (!str_prefix(arg,"mana"))
    {
        spell = NULL;
        sn = -3;
        words = "candamira";
        cost = 100;
    }


    else if (!str_prefix(arg,"yenileme") || !str_prefix(arg,"hareket"))
    {
	spell =  spell_refresh;
	sn    = skill_lookup("refresh");
	words = "candusima";
	cost  = 50;
    }

    else if (!str_prefix(arg,"yüksek") )
    {
	spell =  spell_master_heal;
	sn    = skill_lookup("master healing");
	words = "candastra nikazubra";
	cost  = 2000;
    }

    else if (!str_prefix(arg,"takat") )
    {
	spell =  NULL;
	sn    = -2;
	words = "energizer";
	cost  = 2000;
    }

    else
    {
      act("Þifacý bu hizmeti vermiyor. Hizmet listesi için 'iyileþ' yazýn.",
	    ch,NULL,mob,TO_CHAR);
	return;
    }

    if (cost > (ch->gold * 100 + ch->silver))
    {
      act("Yeterli akçen yok.",ch,NULL,mob,TO_CHAR);
	return;
    }

    WAIT_STATE(ch,PULSE_VIOLENCE);

    deduct_cost(ch,cost);
    mob->gold += cost / 100;

    act("$n mýrýldanýyor, '$T'.",mob,NULL,words,TO_ROOM);
    if (sn == -2)
     {
	ch->mana += 300;
	ch->mana = UMIN(ch->mana,ch->max_mana);
  send_to_char("Vücudundan þifalý bir sýcaklýk geçiyor.\n\r",ch);
     }
    if (sn == -3)
    {
	ch->mana += dice(2,8) + mob->level / 3;
	ch->mana = UMIN(ch->mana,ch->max_mana);
  send_to_char("Vücudundan þifalý bir sýcaklýk geçiyor.\n\r",ch);
    }

     if (sn < 0)
	return;

     spell(sn,mob->level,mob,ch,TARGET_CHAR);
}


void heal_battle(CHAR_DATA *mob, CHAR_DATA *ch )
{
    int sn;

    if (is_name((char*)cabal_table[ch->cabal].short_name,mob->name) )
	return;

    if (IS_NPC(ch) || ch->cabal != CABAL_BATTLE)
       {
         do_say(mob,(char*)"Sana yardým etmem.");
	return;
       }

    if (!IS_AFFECTED(ch,AFF_BLIND) && !IS_AFFECTED(ch,AFF_PLAGUE)
	 && !IS_AFFECTED(ch,AFF_POISON) && !IS_AFFECTED(ch,AFF_CURSE) )
       {
	do_say(mob,(char*)"Benim yardýmýma ihtiyacýn yok evladým. Fakat...");
	sn = skill_lookup((char*)"remove curse");
	spell_remove_curse(sn,mob->level,mob,ch,TARGET_CHAR);
	return;
       }

       act("$n sana yemen için otlar veriyor.",mob,NULL,ch,TO_VICT);
       act("Otlarý yiyorsun.",mob,NULL,ch,TO_VICT);
       act("Otlarý $E veriyorsun.",mob,NULL,ch,TO_CHAR);
       act("$N verdiðin otlarý yiyor.",mob,NULL,ch,TO_CHAR);
       act("$n $E ot veriyor.",mob,NULL,ch,TO_NOTVICT);
       act("$n $S verdiði otlarý yiyor.",mob,NULL,ch,TO_NOTVICT);

    WAIT_STATE(ch,PULSE_VIOLENCE);

    if (IS_AFFECTED(ch,AFF_BLIND))
      {
       sn = skill_lookup("cure blindness");
       spell_cure_blindness(sn,mob->level,mob,ch,TARGET_CHAR);
      }

    if (IS_AFFECTED(ch,AFF_PLAGUE))
      {
       sn = skill_lookup("cure disease");
       spell_cure_disease(sn,mob->level,mob,ch,TARGET_CHAR);
      }
    if (IS_AFFECTED(ch,AFF_POISON))
      {
       sn = skill_lookup("cure poison");
       spell_cure_poison(sn,mob->level,mob,ch,TARGET_CHAR);
      }
    if (IS_AFFECTED(ch,AFF_CURSE))
      {
       sn = skill_lookup("remove curse");
       spell_remove_curse(sn,mob->level,mob,ch,TARGET_CHAR);
      }
    return;
}
