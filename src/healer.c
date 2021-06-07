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
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include "merc.h"
#include "magic.h"

DECLARE_DO_FUN(	do_say	);

void do_heal(CHAR_DATA *ch, wchar_t *argument)
{
    CHAR_DATA *mob;
    wchar_t arg[MAX_INPUT_LENGTH];
    int cost,sn;
    SPELL_FUN *spell;
    const wchar_t *words;

    /* check for healer */
    for ( mob = ch->in_room->people; mob; mob = mob->next_in_room )
    {
        if ( IS_NPC(mob) && IS_SET(mob->act, ACT_IS_HEALER))
	 {
	  if (ch->cabal && (is_name((wchar_t*)"cabal",mob->name)||is_name((wchar_t*)"kabal",mob->name)))
		{
    		 if (is_name((wchar_t*)cabal_table[ch->cabal].short_name,mob->name) )
		 	break;
		 else continue;
		}
          else  break;
	 }
    }

    if ( mob == NULL )
    {
      send_to_char( L"Burada yapamazsın.\n\r", ch );
        return;
    }

    if ( ch->cabal == CABAL_BATTLE )
    {
      send_to_char( L"Sen Öfke Kabalı üyesisin, pis bir büyücü değil!\n\r",ch );
        return;
    }

    one_argument(argument,arg);

    if (arg[0] == '\0')
    {
        /* display price list */
        act( L"Şifacı şu hizmetleri veriyor.",ch,NULL,mob,TO_CHAR);
      	send_to_char( L"  hafif       : hafif yara tedavisi   100 akçe\n\r",ch);
      	send_to_char( L"  ciddi       : ciddi yara tedavisi   150 akçe\n\r",ch);
      	send_to_char( L"  kritik      : kritik yara tedavisi  250 akçe\n\r",ch);
      	send_to_char( L"  şifa        : şifa büyüsü           500 akçe\n\r",ch);
      	send_to_char( L"  körlük      : körlük tedavisi       200 akçe\n\r",ch);
      	send_to_char( L"  hastalık    : hastalık tedavisi     150 akçe\n\r",ch);
      	send_to_char( L"  zehir       : zehir tedavisi        250 akçe\n\r",ch);
      	send_to_char( L"  lanet       : lanet kaldırma        500 akçe\n\r",ch);
      	send_to_char( L"  yenileme    : yorgunluk azaltma      50 akçe\n\r",ch);
      	send_to_char( L"  mana        : mana yenileme         100 akçe\n\r",ch);
      	send_to_char( L"  yüksek şifa : yüksek tedavi        2000 akçe\n\r",ch);
      	send_to_char( L"  takat       : ileri mana tedavi    2000 akçe\n\r",ch);
      	send_to_char( L" Hizmet almak için: iyileş <tip>\n\r",ch);
	return;
    }

    if (!str_prefix(arg, L"hafif"))
    {
        spell = spell_cure_light;
	sn    = skill_lookup( L"cure light");
	words = L"judicandus dies";
	 cost  = 100;
    }

    else if (!str_prefix(arg, L"ciddi"))
    {
	spell = spell_cure_serious;
	sn    = skill_lookup( L"cure serious");
	words = L"judicandus gzfuajg";
	cost  = 150;
    }

    else if (!str_prefix(arg, L"kritik"))
    {
	spell = spell_cure_critical;
	sn    = skill_lookup( L"cure critical");
	words = L"judicandus qfuhuqar";
	cost  = 250;
    }

    else if (!str_prefix(arg, L"şifa"))
    {
	spell = spell_heal;
	sn = skill_lookup( L"heal");
	words = L"pzar";
	cost  = 500;
    }

    else if (!str_prefix(arg, L"körlük"))
    {
	spell = spell_cure_blindness;
	sn    = skill_lookup( L"cure blindness");
      	words = L"judicandus noselacri";
        cost  = 200;
    }

    else if (!str_prefix(arg, L"hastalık"))
    {
	spell = spell_cure_disease;
	sn    = skill_lookup( L"cure disease");
	words = L"judicandus eugzagz";
	cost = 150;
    }

    else if (!str_prefix(arg, L"zehir"))
    {
	spell = spell_cure_poison;
	sn    = skill_lookup( L"cure poison");
	words = L"judicandus sausabru";
	cost  = 250;
    }

    else if (!str_prefix(arg, L"lanet") )
    {
	spell = spell_remove_curse;
	sn    = skill_lookup( L"remove curse");
	words = L"candussido judifgz";
	cost  = 500;
    }

    else if (!str_prefix(arg, L"mana"))
    {
        spell = NULL;
        sn = -3;
        words = L"candamira";
        cost = 100;
    }


    else if (!str_prefix(arg, L"yenileme") || !str_prefix(arg, L"hareket"))
    {
	spell =  spell_refresh;
	sn    = skill_lookup( L"refresh");
	words = L"candusima";
	cost  = 50;
    }

    else if (!str_prefix(arg, L"yüksek") )
    {
	spell =  spell_master_heal;
	sn    = skill_lookup( L"master healing");
	words = L"candastra nikazubra";
	cost  = 2000;
    }

    else if (!str_prefix(arg, L"takat") )
    {
	spell =  NULL;
	sn    = -2;
	words = L"energizer";
	cost  = 2000;
    }

    else
    {
      act( L"Şifacı bu hizmeti vermiyor. Hizmet listesi için 'iyileş' yazın.",
	    ch,NULL,mob,TO_CHAR);
	return;
    }

    if ( cost > ch->silver )
    {
      act( L"Yeterli akçen yok.",ch,NULL,mob,TO_CHAR);
	return;
    }

    WAIT_STATE(ch,PULSE_VIOLENCE);

    deduct_cost(ch,cost);
    mob->silver += cost;

    act( L"$n mırıldanıyor, '$T'.",mob,NULL,words,TO_ROOM);
    if (sn == -2)
     {
	ch->mana += 300;
	ch->mana = UMIN(ch->mana,ch->max_mana);
  send_to_char( L"Vücudundan şifalı bir sıcaklık geçiyor.\n\r",ch);
     }
    if (sn == -3)
    {
	ch->mana += dice(2,8) + mob->level / 3;
	ch->mana = UMIN(ch->mana,ch->max_mana);
  send_to_char( L"Vücudundan şifalı bir sıcaklık geçiyor.\n\r",ch);
    }

     if (sn < 0)
	return;

     spell(sn,mob->level,mob,ch,TARGET_CHAR);
}


void heal_battle(CHAR_DATA *mob, CHAR_DATA *ch )
{
    int sn;

    if (is_name((wchar_t*)cabal_table[ch->cabal].short_name,mob->name) )
	return;

    if (IS_NPC(ch) || ch->cabal != CABAL_BATTLE)
       {
         do_say(mob,(wchar_t*)"Sana yardım etmem.");
	return;
       }

    if (!IS_AFFECTED(ch,AFF_BLIND) && !IS_AFFECTED(ch,AFF_PLAGUE)
	 && !IS_AFFECTED(ch,AFF_POISON) && !IS_AFFECTED(ch,AFF_CURSE) )
       {
	do_say(mob,(wchar_t*)"Benim yardımıma ihtiyacın yok evladım. Fakat...");
	sn = skill_lookup((wchar_t*)"remove curse");
	spell_remove_curse(sn,mob->level,mob,ch,TARGET_CHAR);
	return;
       }

       act( L"$n sana yemen için otlar veriyor.",mob,NULL,ch,TO_VICT);
       act( L"Otları yiyorsun.",mob,NULL,ch,TO_VICT);
       act( L"Otları $E veriyorsun.",mob,NULL,ch,TO_CHAR);
       act( L"$N verdiğin otları yiyor.",mob,NULL,ch,TO_CHAR);
       act( L"$n $E ot veriyor.",mob,NULL,ch,TO_NOTVICT);
       act( L"$n $S verdiği otları yiyor.",mob,NULL,ch,TO_NOTVICT);

    WAIT_STATE(ch,PULSE_VIOLENCE);

    if (IS_AFFECTED(ch,AFF_BLIND))
      {
       sn = skill_lookup( L"cure blindness");
       spell_cure_blindness(sn,mob->level,mob,ch,TARGET_CHAR);
      }

    if (IS_AFFECTED(ch,AFF_PLAGUE))
      {
       sn = skill_lookup( L"cure disease");
       spell_cure_disease(sn,mob->level,mob,ch,TARGET_CHAR);
      }
    if (IS_AFFECTED(ch,AFF_POISON))
      {
       sn = skill_lookup( L"cure poison");
       spell_cure_poison(sn,mob->level,mob,ch,TARGET_CHAR);
      }
    if (IS_AFFECTED(ch,AFF_CURSE))
      {
       sn = skill_lookup( L"remove curse");
       spell_remove_curse(sn,mob->level,mob,ch,TARGET_CHAR);
      }
    return;
}
