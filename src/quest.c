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

/****************************************************************************
*  Automated Quest code written by Vassago of MOONGATE, moongate.ams.com    *
*  4000. Copyright (c) 1996 Ryan Addams, All Rights Reserved. Use of this   *
*  code is allowed provided you add a credit line to the effect of:         *
*  "Quest Code (c) 1996 Ryan Addams" to your logon screen with the rest     *
*  of the standard diku/rom credits. If you use this or a modified version  *
*  of this code, let me know via email: moongate@moongate.ams.com. Further  *
*  updates will be posted to the rom mailing list. If you'd like to get     *
*  the latest version of quest.c, please send a request to the above add-   *
*  ress. Quest Code v2.01. Please do not remove this notice from this file. *
****************************************************************************/

/****************************************************************************
 *  Revised and adopted to Anatolia by chronos.                             *
 ****************************************************************************/

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
*       ROM 2.4 is copyright 1993-1995 Russ Taylor                         *
*       ROM has been brought to you by the ROM consortium                  *
*           Russ Taylor (rtaylor@pacinfo.com)                              *
*           Gabrielle Taylor (gtaylor@pacinfo.com)                         *
*           Brian Moore (rom@rom.efn.org)                                  *
*       By using this code, you have agreed to follow the terms of the     *
*       ROM license, in the file Rom24/doc/rom.license                     *
***************************************************************************/

#include <sys/types.h>
#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "recycle.h"

DECLARE_SPELL_FUN(	spell_identify	);

void do_tell_quest( CHAR_DATA *ch, CHAR_DATA *victim, wchar_t *argument);
CHAR_DATA *get_quest_world( CHAR_DATA *ch, MOB_INDEX_DATA *victim );
extern	MOB_INDEX_DATA	*mob_index_hash	[MAX_KEY_HASH];

/* Local functions */

void generate_quest	args(( CHAR_DATA *ch, CHAR_DATA *questman ));
void quest_update	args(( void ));
bool chance		args(( int num ));

/* CHANCE function. I use this everywhere in my code, very handy :> */

bool chance(int num)
{
    if (number_range(1,100) <= num) return TRUE;
    else return FALSE;
}

/* The main quest function */

void do_quest(CHAR_DATA *ch, wchar_t *argument)
{
    CHAR_DATA *questman;
    OBJ_DATA *obj=NULL, *obj_next;
    MOB_INDEX_DATA *questinfo;
    ROOM_INDEX_DATA *pRoomIndex;
    wchar_t buf [MAX_STRING_LENGTH];
    wchar_t arg1 [MAX_INPUT_LENGTH];
    wchar_t arg2 [MAX_INPUT_LENGTH];
    int sn,trouble_vnum=0;

    argument = one_argument(argument, arg1);
    argument = one_argument(argument, arg2);

    if (IS_NPC(ch) )  return;

  if ( IS_SET(ch->act,PLR_NO_DESCRIPTION) )
  {
    printf_to_char(ch,L"En az 350 karakterlik tanımın olmadan görev komutlarını kullanamazsın.");
    return;
  }

  if (!wcscmp(arg1, L"bilgi"))
  {
    if (IS_SET(ch->act, PLR_QUESTOR))
    {
      if (ch->pcdata->questmob == -1 )
      {
        printf_to_char(ch,L"{cGörevin neredeyse tamamlandı!\n\rZamanın bitmeden önce görevciye git!{x\n\r");
      }
      else if (ch->pcdata->questmob > 0)
      {
        questinfo = get_mob_index(ch->pcdata->questmob);
        if (questinfo != NULL)
        {
          pRoomIndex = get_room_index(ch->pcdata->questroom);
          printf_to_char(ch,L"Görevin, {c%s{w bölgesinde {c%s{w isimli odadaki {c%s{w adlı korkunç yaratığı öldürmek!\n\r",
            pRoomIndex->area->name,pRoomIndex->name,questinfo->short_descr);
        }
        else
        {
          send_to_char( L"Henüz bir görevin yok.\n\r",ch);
        }
        return;
      }
    }
    else
    {
      send_to_char( L"Henüz bir görevin yok.\n\r",ch);
    }
    return;
  }
    if (!wcscmp(arg1, L"puan"))
    {
      swprintf( buf, MAX_STRING_LENGTH-1, L"%d görev puanın var.\n\r",ch->pcdata->questpoints);
	send_to_char(buf, ch);
	return;
    }
    else if (!wcscmp(arg1, L"zaman"))
    {
	if (!IS_SET(ch->act, PLR_QUESTOR))
	{
    send_to_char( L"Henüz bir görevin yok.\n\r",ch);
	    if (ch->pcdata->nextquest > 1)
	    {
        swprintf( buf, MAX_STRING_LENGTH-1, L"Yeni bir görev isteyebilmen için %d dakika kaldı.\n\r",ch->pcdata->nextquest);
		send_to_char(buf, ch);
	    }
	    else if (ch->pcdata->nextquest == 1)
	    {
        swprintf( buf, MAX_STRING_LENGTH-1, L"Yeni bir görev isteyebilmen için bir dakikadan az zaman kaldı.\n\r");
		send_to_char(buf, ch);
	    }
	}
        else if (ch->pcdata->countdown > 0)
        {
          swprintf( buf, MAX_STRING_LENGTH-1, L"Görevi bitirmek için kalan zaman: %d dakika.\n\r",ch->pcdata->countdown);
	    send_to_char(buf, ch);
	}
	return;
    }

/* Checks for a character in the room with spec_questmaster set. This special
   procedure must be defined in special.c. You could instead use an
   ACT_QUESTMASTER flag instead of a special procedure. */

    for ( questman = ch->in_room->people; questman != NULL; questman = questman->next_in_room )
    {
	if (!IS_NPC(questman)) continue;
        if (questman->spec_fun == spec_lookup( (wchar_t*)"spec_questmaster" )) break;
    }

    if (questman == NULL || questman->spec_fun != spec_lookup( (wchar_t*)"spec_questmaster" ))
    {
      send_to_char( L"Odada görevci göremiyorum.\n\r", ch);
        return;
    }

    if ( questman->fighting != NULL)
    {
      send_to_char( L"Dövüş bitene kadar bekle.\n\r",ch);
        return;
    }

    ch->pcdata->questgiver = questman->pIndexData->vnum;

/* And, of course, you will need to change the following lines for YOUR
   quest item information. Quest items on Moongate are unbalanced, very
   very nice items, and no one has one yet, because it takes awhile to
   build up quest points :> Make the item worth their while. */

    if (!wcscmp(arg1, L"liste"))
    {
      act( L"$n $Z görev ekipmanlarının listesini istiyor.", ch, NULL, questman, TO_ROOM);
act ( L"$Z görev ekipmanlarının listesini istiyorsun.",ch, NULL, questman, TO_CHAR);

printf_to_char(ch,L"{Csırtçantası{x.........({yçanta{x   )......{R5000 gp{x\n\r");
printf_to_char(ch,L"{Ckemer{x...............({ykemer{x   )......{R1000 gp{x\n\r");
printf_to_char(ch,L"{Cparlak silah{x........({yparlak{x  )......{R1000 gp{x\n\r");
printf_to_char(ch,L"{Cmat silah{x...........({ymat{x     )......{R1000 gp{x\n\r");
printf_to_char(ch,L"{C100 pratik{x..........({ypratik{x  )......{R1000 gp{x\n\r");
printf_to_char(ch,L"{Cişlemeli yüzük{x......({yişlemeli{x).......{R750 gp{x\n\r");
printf_to_char(ch,L"{Cdesenli yüzük{x.......({ydesenli{x ).......{R750 gp{x\n\r");
printf_to_char(ch,L"{Coymalı yüzük{x........({yoymalı{x  ).......{R750 gp{x\n\r");
printf_to_char(ch,L"{Ckakmalı yüzük{x.......({ykakmalı{x ).......{R750 gp{x\n\r");
printf_to_char(ch,L"{Ctesti{x...............({ytesti{x   ).......{R500 gp{x\n\r");
printf_to_char(ch,L"{C1 bünye puanı{x.......({ybünye{x   ).......{R250 gp{x\n\r");
if ( ch->iclass == CLASS_SAMURAI )
{
printf_to_char(ch,L"{Ckatana{x..............({ykatana{x  ).......{R100 gp{x\n\r");
printf_to_char(ch,L"{Cikinci katana{x.......({ykeskin{x  ).......{R100 gp{x\n\r");
printf_to_char(ch,L"{Cölüm azaltma{x........({yölüm{x    )........{R50 gp{x\n\r");
}
if ( ch->iclass == CLASS_VAMPIRE )
{
printf_to_char(ch,L"{Cvampir yeteneği{x.....({yvampir{x  )........{R50 gp{x\n\r");
}

printf_to_char(ch,L"Bir eşya satın almak için {Rgörev satınal <eşya_adı>{x yaz.\n\r");
	return;
    }
    else if (!wcscmp(arg1, L"özellik"))
  	{
  		if (arg2[0] == '\0')
  		{
  			printf_to_char(ch,L"Hangi görev ekipmanının özelliklerini öğrenmek istiyorsun?\n\rKullanım: {Rgörev özellik <ekipman>{x\n\r");
  			return;
  		}
  		else if (is_name(arg2, (wchar_t*)"çanta"))
  		{
  			obj = create_object(get_obj_index(QUEST_ITEM4),ch->level);
  			spell_identify( 0, 0, ch, obj ,0);
  			extract_obj(obj);
  			return;
  		}
  		else if (is_name(arg2, (wchar_t*)"kemer"))
  		{
  			obj = create_object(get_obj_index(QUEST_ITEM1),ch->level);
  			spell_identify( 0, 0, ch, obj ,0);
  			extract_obj(obj);
  			return;
  		}
  		else if (is_name(arg2, (wchar_t*)"parlak"))
  		{
  			obj = create_object(get_obj_index(QUEST_ITEM_SILAH1),ch->level);
  			spell_identify( 0, 0, ch, obj ,0);
  			extract_obj(obj);
  			return;
  		}
  		else if (is_name(arg2, (wchar_t*)"mat"))
  		{
  			obj = create_object(get_obj_index(QUEST_ITEM_SILAH2),ch->level);
  			spell_identify( 0, 0, ch, obj ,0);
  			extract_obj(obj);
  			return;
  		}
  		else if (is_name(arg2, (wchar_t*)"işlemeli"))
  		{
  			obj = create_object(get_obj_index(QUEST_ITEM_YUZUK1),ch->level);
  			spell_identify( 0, 0, ch, obj ,0);
  			extract_obj(obj);
  			return;
  		}
  		else if (is_name(arg2, (wchar_t*)"desenli"))
  		{
  			obj = create_object(get_obj_index(QUEST_ITEM_YUZUK2),ch->level);
  			spell_identify( 0, 0, ch, obj ,0);
  			extract_obj(obj);
  			return;
  		}
  		else if (is_name(arg2, (wchar_t*)"oymalı"))
  		{
  			obj = create_object(get_obj_index(QUEST_ITEM_YUZUK3),ch->level);
  			spell_identify( 0, 0, ch, obj ,0);
  			extract_obj(obj);
  			return;
  		}
      else if (is_name(arg2, (wchar_t*)"kakmalı"))
  		{
  			obj = create_object(get_obj_index(QUEST_ITEM_YUZUK4),ch->level);
  			spell_identify( 0, 0, ch, obj ,0);
  			extract_obj(obj);
  			return;
  		}
      else if (is_name(arg2, (wchar_t*)"testi"))
  		{
  			obj = create_object(get_obj_index(QUEST_ITEM5),ch->level);
  			spell_identify( 0, 0, ch, obj ,0);
  			extract_obj(obj);
  			return;
  		}
  		else
  		{
  			printf_to_char(ch,L"Hangi görev ekipmanının özelliklerini öğrenmek istiyorsun? {Rgörev özellik <ekipman>{x\n\r");
  			return;
  		}
  	}

    else if (!wcscmp(arg1, L"satınal"))
    {
	if (arg2[0] == '\0')
	{
    send_to_char( L"Bir ödülü satın almak için 'görev satınal <görev_eşyası>' yazın.\n\r",ch);
	    return;
	}

	else if (is_name(arg2, (wchar_t*)"çanta"))
	{
      if (IS_SET(ch->quest,QUEST_BACKPACK))
      {
        do_tell_quest(ch,questman,(wchar_t*)"Bu eşyayı zaten almışsın. Kaybettiysen sigortadan faydalan.");
        return;
      }

	    if (ch->pcdata->questpoints >= 5000)
	    {
		ch->pcdata->questpoints -= 5000;
	        obj = create_object(get_obj_index(QUEST_ITEM4),ch->level);
	         SET_BIT(ch->quest,QUEST_BACKPACK);
	    }
	    else
	    {
        swprintf( buf, MAX_STRING_LENGTH-1, L"Üzgünüm %s, bunun için yeterli görev puanın yok.",ch->name);
		      do_tell_quest(ch,questman,buf);
		return;
	    }
	}

	else if (is_name(arg2, (wchar_t*)"testi"))
	{
    if (IS_SET(ch->quest,QUEST_DECANTER))
    {
      do_tell_quest(ch,questman,(wchar_t*)"Bu eşyayı zaten almışsın. Kaybettiysen sigortadan faydalan.");
      return;
    }

	    if (ch->pcdata->questpoints >= 500)
	    {
		ch->pcdata->questpoints -= 500;
	        obj = create_object(get_obj_index(QUEST_ITEM5),ch->level);
	 	SET_BIT(ch->quest,QUEST_DECANTER);
	    }
	    else
	    {
        swprintf( buf, MAX_STRING_LENGTH-1, L"Üzgünüm %s, bunun için yeterli görev puanın yok.",ch->name);
		do_tell_quest(ch,questman,buf);
		return;
	    }
	}

	else if (is_name(arg2, (wchar_t*)"kemer"))
	{
    if (IS_SET(ch->quest,QUEST_GIRTH))
    {
      do_tell_quest(ch,questman,(wchar_t*)"Bu eşyayı zaten almışsın. Kaybettiysen sigortadan faydalan.");
      return;
    }
	    if (ch->pcdata->questpoints >= 1000)
	    {
		ch->pcdata->questpoints -= 1000;
	        obj = create_object(get_obj_index(QUEST_ITEM1),ch->level);
	 	SET_BIT(ch->quest,QUEST_GIRTH);
	    }
	    else
	    {
        swprintf( buf, MAX_STRING_LENGTH-1, L"Üzgünüm %s, bunun için yeterli görev puanın yok.",ch->name);
		do_tell_quest(ch,questman,buf);
		return;
	    }
	}
	else if (is_name(arg2, (wchar_t*)"işlemeli"))
	{
    if (IS_SET(ch->quest,QUEST_YUZUK1))
    {
      do_tell_quest(ch,questman,(wchar_t*)"Bu eşyayı zaten almışsın. Kaybettiysen sigortadan faydalan.");
      return;
    }
	    if (ch->pcdata->questpoints >= 750)
	    {
		ch->pcdata->questpoints -= 750;
	        obj = create_object(get_obj_index(QUEST_ITEM_YUZUK1),ch->level);
          SET_BIT(ch->quest,QUEST_YUZUK1);
	    }
	    else
	    {
        swprintf( buf, MAX_STRING_LENGTH-1, L"Üzgünüm %s, bunun için yeterli görev puanın yok.",ch->name);
		do_tell_quest(ch,questman,buf);
		return;
	    }
	}
  else if (is_name(arg2, (wchar_t*)"desenli"))
  {
    if (IS_SET(ch->quest,QUEST_YUZUK2))
    {
      do_tell_quest(ch,questman,(wchar_t*)"Bu eşyayı zaten almışsın. Kaybettiysen sigortadan faydalan.");
      return;
    }
      if (ch->pcdata->questpoints >= 750)
      {
    ch->pcdata->questpoints -= 750;
          obj = create_object(get_obj_index(QUEST_ITEM_YUZUK2),ch->level);
          SET_BIT(ch->quest,QUEST_YUZUK2);
      }
      else
      {
        swprintf( buf, MAX_STRING_LENGTH-1, L"Üzgünüm %s, bunun için yeterli görev puanın yok.",ch->name);
    do_tell_quest(ch,questman,buf);
    return;
      }
  }
  else if (is_name(arg2, (wchar_t*)"oymalı"))
  {
    if (IS_SET(ch->quest,QUEST_YUZUK3))
    {
      do_tell_quest(ch,questman,(wchar_t*)"Bu eşyayı zaten almışsın. Kaybettiysen sigortadan faydalan.");
      return;
    }
      if (ch->pcdata->questpoints >= 750)
      {
    ch->pcdata->questpoints -= 750;
          obj = create_object(get_obj_index(QUEST_ITEM_YUZUK3),ch->level);
          SET_BIT(ch->quest,QUEST_YUZUK3);
      }
      else
      {
        swprintf( buf, MAX_STRING_LENGTH-1, L"Üzgünüm %s, bunun için yeterli görev puanın yok.",ch->name);
    do_tell_quest(ch,questman,buf);
    return;
      }
  }
  else if (is_name(arg2, (wchar_t*)"kakmalı"))
  {
    if (IS_SET(ch->quest,QUEST_YUZUK4))
    {
      do_tell_quest(ch,questman,(wchar_t*)"Bu eşyayı zaten almışsın. Kaybettiysen sigortadan faydalan.");
      return;
    }
      if (ch->pcdata->questpoints >= 750)
      {
    ch->pcdata->questpoints -= 750;
          obj = create_object(get_obj_index(QUEST_ITEM_YUZUK4),ch->level);
          SET_BIT(ch->quest,QUEST_YUZUK4);
      }
      else
      {
        swprintf( buf, MAX_STRING_LENGTH-1, L"Üzgünüm %s, bunun için yeterli görev puanın yok.",ch->name);
    do_tell_quest(ch,questman,buf);
    return;
      }
  }
  else if (is_name(arg2, (wchar_t*)"parlak"))
  {
    if (IS_SET(ch->quest,QUEST_SILAH1))
    {
      do_tell_quest(ch,questman,(wchar_t*)"Bu eşyayı zaten almışsın. Kaybettiysen sigortadan faydalan.");
      return;
    }
      if (ch->pcdata->questpoints >= 1000)
      {
    ch->pcdata->questpoints -= 1000;
          obj = create_object(get_obj_index(QUEST_ITEM_SILAH1),ch->level);
          SET_BIT(ch->quest,QUEST_SILAH1);
      }
      else
      {
        swprintf( buf, MAX_STRING_LENGTH-1, L"Üzgünüm %s, bunun için yeterli görev puanın yok.",ch->name);
    do_tell_quest(ch,questman,buf);
    return;
      }
  }
  else if (is_name(arg2, (wchar_t*)"mat"))
  {
    if (IS_SET(ch->quest,QUEST_SILAH2))
    {
      do_tell_quest(ch,questman,(wchar_t*)"Bu eşyayı zaten almışsın. Kaybettiysen sigortadan faydalan.");
      return;
    }
      if (ch->pcdata->questpoints >= 1000)
      {
    ch->pcdata->questpoints -= 1000;
          obj = create_object(get_obj_index(QUEST_ITEM_SILAH2),ch->level);
          SET_BIT(ch->quest,QUEST_SILAH2);
      }
      else
      {
        swprintf( buf, MAX_STRING_LENGTH-1, L"Üzgünüm %s, bunun için yeterli görev puanın yok.",ch->name);
    do_tell_quest(ch,questman,buf);
    return;
      }
  }
	else if (is_name(arg2, (wchar_t*)"pratik"))
	{
	    if (IS_SET(ch->quest, QUEST_PRACTICE))
	    {
        swprintf( buf, MAX_STRING_LENGTH-1, L"Üzgünüm %s, bu ödülü daha önce almıştın!",ch->name);
		do_tell_quest(ch,questman,buf);
		return;
	    }

	    if (ch->pcdata->questpoints >= 1000)
	    {
		ch->pcdata->questpoints -= 1000;
	        ch->practice += 100;
          act( L"$N $e 100 pratik seansı veriyor.", ch, NULL, questman, TO_ROOM );
          act( L"$N sana 100 pratik seansı veriyor.",   ch, NULL, questman, TO_CHAR );
		SET_BIT(ch->quest, QUEST_PRACTICE);
	        return;
	    }
	    else
	    {
        swprintf( buf, MAX_STRING_LENGTH-1, L"Üzgünüm %s, bunun için yeterli görev puanın yok.",ch->name);
		do_tell_quest(ch,questman,buf);
		return;
	    }
	}
	else if (is_name(arg2, (wchar_t*)"vampir"))
	{
	  if (ch->iclass != CLASS_VAMPIRE)
	  {
      swprintf( buf, MAX_STRING_LENGTH-1, L"Sen bu yeteneği kazanamazsın %s.",ch->name);
	    do_tell_quest(ch,questman,buf);
	    return;
	  }
	  if (ch->pcdata->questpoints >= 50)
	    {
		ch->pcdata->questpoints -= 50;
		sn = skill_lookup( L"vampire");
		ch->pcdata->learned[sn] = 100;
    act( L"$N hortlaklığın sırrını $e veriyor.", ch, NULL, questman, TO_ROOM );
  act( L"$N sana hortlaklığın sırrını veriyor.",   ch, NULL, questman, TO_CHAR );
act_color( L"$CGökyüzünde şimşekler çakıyor.$c",   ch, NULL,
			questman, TO_ALL,POS_SLEEPING,CLR_BLUE );
	        return;
	    }
	    else
	    {
        swprintf( buf, MAX_STRING_LENGTH-1, L"Üzgünüm %s, bunun için yeterli görev puanın yok.",ch->name);
		do_tell_quest(ch,questman,buf);
		return;
	    }
	}
	else if (is_name(arg2, (wchar_t*)"bün bünye"))
	{
	    if (ch->perm_stat[STAT_CON] >= get_max_train(ch, STAT_CON))
	    {
        swprintf( buf, MAX_STRING_LENGTH-1, L"Üzgünüm %s, bünye niteliğin yeterince güçlü.",ch->name);
		do_tell_quest(ch,questman,buf);
		return;
	    }

	    if (ch->pcdata->questpoints >= 250)
	    {
		ch->pcdata->questpoints -= 250;
		ch->perm_stat[STAT_CON] += 1;
	    }
	    else
	    {
        swprintf( buf, MAX_STRING_LENGTH-1, L"Üzgünüm %s, bunun için yeterli görev puanın yok.",ch->name);
		do_tell_quest(ch,questman,buf);
		return;
	    }
	}
	else if (is_name(arg2, (wchar_t*)"samuray ölüm"))
	{
	    if (ch->iclass != CLASS_SAMURAI)
	    {
        swprintf( buf, MAX_STRING_LENGTH-1, L"Üzgünüm %s, fakat sen bir samuray değilsin.",ch->name);
		do_tell_quest(ch,questman,buf);
		return;
	    }

	    if (ch->pcdata->death < 1)
	    {
        swprintf( buf, MAX_STRING_LENGTH-1, L"Üzgünüm %s, henüz ölmemişsin.",ch->name);
		do_tell_quest(ch,questman,buf);
		return;
	    }

	    if (ch->pcdata->questpoints >= 50)
	    {
		ch->pcdata->questpoints -= 50;
		ch->pcdata->death -= 1;
	    }
	    else
	    {
        swprintf( buf, MAX_STRING_LENGTH-1, L"Üzgünüm %s, bunun için yeterli görev puanın yok.",ch->name);
		do_tell_quest(ch,questman,buf);
		return;
	    }
	}
	else if (is_name(arg2, (wchar_t*)"katana"))
	{
	  AFFECT_DATA af;
	  OBJ_DATA *katana;
	    if (ch->iclass != 9)
	    {
        swprintf( buf, MAX_STRING_LENGTH-1, L"Üzgünüm %s, fakat sen bir samuray değilsin.",ch->name);
		do_tell_quest(ch,questman,buf);
		return;
	    }

	    if ( (katana = get_obj_list(ch,(wchar_t*)"katana",ch->carrying)) == NULL)
	    {
        swprintf( buf, MAX_STRING_LENGTH-1, L"Üzgünüm %s, fakat katanan yanında değil.",ch->name);
		 do_tell_quest(ch,questman,buf);
	         return;
	    }

	    if (IS_WEAPON_STAT(katana,WEAPON_KATANA))
	    {
        swprintf( buf, MAX_STRING_LENGTH-1, L"Üzgünüm %s, fakat katanan ilk görevi geçmiş zaten.",ch->name);
		do_tell_quest(ch,questman,buf);
		return;
	    }

	    if (ch->pcdata->questpoints >= 100)
	    {
		ch->pcdata->questpoints -= 100;
		af.where	= TO_WEAPON;
		af.type		= gsn_reserved;
		af.level	= 100;
		af.duration	= -1;
		af.modifier	= 0;
		af.bitvector	= WEAPON_KATANA;
		af.location	= APPLY_NONE;
		affect_to_obj(katana,&af);
    swprintf( buf, MAX_STRING_LENGTH-1, L"Katanandaki gücün giderek artacağını hissediyorsun.");
		do_tell_quest(ch,questman,buf);
	    }
	    else
	    {
        swprintf( buf, MAX_STRING_LENGTH-1, L"Üzgünüm %s, bunun için yeterli görev puanın yok.",ch->name);
		do_tell_quest(ch,questman,buf);
		return;
	    }
	}
	else if (is_name(arg2, (wchar_t*)"keskin ikinci"))
	{
	  AFFECT_DATA af;
	  OBJ_DATA *katana;

	    if (ch->iclass != 9)
	    {
        swprintf( buf, MAX_STRING_LENGTH-1, L"Üzgünüm %s, fakat sen bir samuray değilsin.",ch->name);
		do_tell_quest(ch,questman,buf);
		return;
	    }

	    if ( (katana = get_obj_list(ch,(wchar_t*)"katana",ch->carrying)) == NULL)
	    {
        swprintf( buf, MAX_STRING_LENGTH-1, L"Üzgünüm %s, fakat katanan yanında değil.",ch->name);
		 do_tell_quest(ch,questman,buf);
	         return;
	    }

	    if (!IS_WEAPON_STAT(katana,WEAPON_KATANA))
	    {
        swprintf( buf, MAX_STRING_LENGTH-1, L"Üzgünüm %s, fakat katanan ilk görevi henüz geçmemiş.",ch->name);
		do_tell_quest(ch,questman,buf);
		return;
	    }

	    if (ch->pcdata->questpoints >= 100)
	    {
		ch->pcdata->questpoints -= 100;
		af.where	= TO_WEAPON;
		af.type		= gsn_reserved;
		af.level	= 100;
		af.duration	= -1;
		af.modifier	= 0;
		af.bitvector	= WEAPON_SHARP;
		af.location	= APPLY_NONE;
		affect_to_obj(katana,&af);
    swprintf( buf, MAX_STRING_LENGTH-1, L"Şu andan sonra katanan en keskin kılıçlardan daha keskin olacak.");
		do_tell_quest(ch,questman,buf);
	    }
	    else
	    {
        swprintf( buf, MAX_STRING_LENGTH-1, L"Üzgünüm %s, bunun için yeterli görev puanın yok.",ch->name);
		do_tell_quest(ch,questman,buf);
		return;
	    }
	}
	else
	{
    swprintf( buf, MAX_STRING_LENGTH-1, L"Ondan bende yok, %s.",ch->name);
	    do_tell_quest(ch,questman,buf);
	}
	if (obj != NULL)
	{
   if ( gorev_ekipmani_mi( obj ) )
	 {
	  swprintf( buf, MAX_STRING_LENGTH-1, obj->short_descr,
		IS_GOOD(ch) ? "parlak" :
		IS_NEUTRAL(ch) ? "mavi-yeşil" : "kızıl", ch->name);
	  free_string( obj->short_descr );
	  obj->short_descr = str_dup( buf );
	 }
   act( L"$N $e $p veriyor.", ch, obj, questman, TO_ROOM );
   act( L"$N sana $p veriyor.",   ch, obj, questman, TO_CHAR );
	 obj_to_char(obj, ch);
	}
	return;
    }
    else if (!wcscmp(arg1, L"iste"))
    {
      act( L"$n $Z görev istiyor.", ch, NULL, questman, TO_ROOM);
act ( L"$Z görev istiyorsun.",ch, NULL, questman, TO_CHAR);

	if (IS_SET(ch->act, PLR_QUESTOR))
	{
    swprintf( buf, MAX_STRING_LENGTH-1, L"Zaten görevdesin. İnanmıyorsan \"görev bilgi\" yaz!");
	    do_tell_quest(ch,questman,buf);
	    return;
	}
	if (ch->pcdata->nextquest > 0)
	{
    swprintf( buf, MAX_STRING_LENGTH-1, L"Çok cesursun %s, fakat izin ver başkaları da nasiplensin.",ch->name);
    do_tell_quest(ch,questman,buf);
    swprintf( buf, MAX_STRING_LENGTH-1, L"Daha sonra tekrar uğra.");
	    do_tell_quest(ch,questman,buf);
	    return;
	}

  swprintf( buf, MAX_STRING_LENGTH-1, L"Teşekkür ederim, cesur %s!",ch->name);
	do_tell_quest(ch,questman,buf);

	generate_quest(ch, questman);

        if (ch->pcdata->questmob > 0 )
	{
            ch->pcdata->countdown = number_range(15,30);
	    SET_BIT(ch->act, PLR_QUESTOR);
      swprintf( buf, MAX_STRING_LENGTH-1, L"Bu görevi tamamlamak için %d dakikan var.",ch->pcdata->countdown);
	    do_tell_quest(ch,questman,buf);
      swprintf( buf, MAX_STRING_LENGTH-1, L"Tanrılar seninle olsun!");
	    do_tell_quest(ch,questman,buf);
	}
	return;
    }
    else if (!wcscmp(arg1, L"iptal"))

    {

      if (!IS_SET(ch->act, PLR_QUESTOR))

      {

        swprintf( buf, MAX_STRING_LENGTH-1, L"Görevde değilsin.");
        do_tell_quest(ch, questman, buf);

        return;

      }

      REMOVE_BIT(ch->act, PLR_QUESTOR);
      ch->pcdata->questgiver = 0;
      ch->pcdata->countdown = 0;
      ch->pcdata->nextquest = 5;
      ch->pcdata->questmob = 0;
      ch->pcdata->questroom = 0;



      swprintf( buf, MAX_STRING_LENGTH-1, L"Görevini iptal ettim.");

      do_tell_quest(ch, questman, buf);

      return;

    }

	else if (!wcscmp(arg1, L"bitti"))
    {
      act( L"$n $E görevi bitirdiğini haber veriyor.", ch, NULL, questman, TO_ROOM);
act ( L"$E görevi bitirdiğini haber veriyorsun.",ch, NULL, questman, TO_CHAR);
	if (ch->pcdata->questgiver != questman->pIndexData->vnum)
	{
    swprintf( buf, MAX_STRING_LENGTH-1, L"Seni bir göreve gönderdiğimi hatırlamıyorum!");
	    do_tell_quest(ch,questman,buf);
	    return;
	}

	if (IS_SET(ch->act, PLR_QUESTOR))
	{
	    if (ch->pcdata->questmob == -1 && ch->pcdata->countdown > 0)
	    {
		int reward=0, pointreward=0, pracreward=0,level;

		level = ch->level;
		reward = 10 + dice( level, 20);
		pointreward = number_range(20,40);

		if(IS_SET(ch->pcdata->dilek,DILEK_FLAG_GOREV))
		{
				printf_to_char( ch , L"{CGörev dileğin sayesinde kazandığın GP artıyor.{x\n\r" );
				pointreward *= 2;
		}

    if( ikikat_gp > 0 )
		{
				printf_to_char( ch , L"{Cİki kat GP kazanma etkinliği nedeniyle kazandığın GP artıyor.{x\n\r" );
				pointreward *= 2;
		}

    swprintf( buf, MAX_STRING_LENGTH-1, L"Tebrikler!");
    do_tell_quest(ch,questman,buf);
    swprintf( buf, MAX_STRING_LENGTH-1, L"Karşılığında sana %d GP ve %d akçe veriyorum.",pointreward,reward);
		do_tell_quest(ch,questman,buf);
		if (chance(2))
		{
		    pracreward = number_range(1,6);
        swprintf( buf, MAX_STRING_LENGTH-1, L"%d pratik seansı kazandın!\n\r",pracreward);
		    send_to_char(buf, ch);
		    ch->practice += pracreward;
		}
    if (number_range(1,8)==1)
		{
      pracreward = number_range(1,7);
      printf_to_char(ch,L"%d RK puanı kazandın.\n\r",pracreward);
		  ch->pcdata->rk_puani += pracreward;
		}

	        REMOVE_BIT(ch->act, PLR_QUESTOR);
	        ch->pcdata->questgiver = 0;
	        ch->pcdata->countdown = 0;
	        ch->pcdata->questmob = 0;
	        ch->pcdata->nextquest = number_range(1,4);
		ch->silver += reward;
		ch->pcdata->questpoints += pointreward;

	        return;
	    }
	    else if ( ch->pcdata->questmob > 0 && ch->pcdata->countdown > 0 )
	    {
        swprintf( buf, MAX_STRING_LENGTH-1, L"Henüz görevi bitirmedin. Fakat hala zamanın var!");
		do_tell_quest(ch,questman,buf);
		return;
	    }
	}
	if (ch->pcdata->nextquest > 0)
  swprintf( buf, MAX_STRING_LENGTH-1, L"Maalesef görevi zamanında tamamlayamadın!");
else swprintf( buf, MAX_STRING_LENGTH-1, L"Önce bir görev İSTEmelisin, %s.",ch->name);
	do_tell_quest(ch,questman,buf);
	return;
    }
    else if (!wcscmp(arg1, L"sigorta"))
    {
	if (arg2[0] == '\0')
	{
    send_to_char( L"Görev objesinin sigortasından faydalanmak için 'görev sigorta <obje>' yazın.\n\r",ch);
	    return;
	}

	if (is_name(arg2, (wchar_t*)"kemer"))
	{
	  if (IS_SET(ch->quest,QUEST_GIRTH))
	    {
		trouble_vnum = QUEST_ITEM1;
	    }
	}
	else if (is_name(arg2, (wchar_t*)"çanta"))
	{
	  if (IS_SET(ch->quest,QUEST_BACKPACK))
	    {
		trouble_vnum = QUEST_ITEM4;
	    }
	}
	else if (is_name(arg2, (wchar_t*)"testi"))
	{
	  if (IS_SET(ch->quest,QUEST_DECANTER))
	    {
		trouble_vnum = QUEST_ITEM5;
	    }
	}
	else if (is_name(arg2, (wchar_t*)"parlak"))
	{
	  if (IS_SET(ch->quest,QUEST_SILAH1))
	    {
		trouble_vnum = QUEST_ITEM_SILAH1;
	    }
	}
  else if (is_name(arg2, (wchar_t*)"mat"))
	{
	  if (IS_SET(ch->quest,QUEST_SILAH2))
	    {
		trouble_vnum = QUEST_ITEM_SILAH2;
	    }
	}
  else if (is_name(arg2, (wchar_t*)"işlemeli"))
	{
	  if (IS_SET(ch->quest,QUEST_YUZUK1))
	    {
		trouble_vnum = QUEST_ITEM_YUZUK1;
	    }
	}
  else if (is_name(arg2, (wchar_t*)"desenli"))
	{
	  if (IS_SET(ch->quest,QUEST_YUZUK2))
	    {
		trouble_vnum = QUEST_ITEM_YUZUK2;
	    }
	}
  else if (is_name(arg2, (wchar_t*)"oymalı"))
	{
	  if (IS_SET(ch->quest,QUEST_YUZUK3))
	    {
		trouble_vnum = QUEST_ITEM_YUZUK3;
	    }
	}
  else if (is_name(arg2, (wchar_t*)"kakmalı"))
	{
	  if (IS_SET(ch->quest,QUEST_YUZUK4))
	    {
		trouble_vnum = QUEST_ITEM_YUZUK4;
	    }
	}
	if (!trouble_vnum)
	{
    swprintf( buf, MAX_STRING_LENGTH-1, L"Üzgünüm %s, fakat bahsettiğin ödülü henüz almamışsın.\n\r",ch->name);
	    do_tell_quest(ch,questman,buf);
	    return;
	}

        for(obj = object_list; obj != NULL; obj = obj_next)
	{
	 obj_next = obj->next;
	 if ( obj->pIndexData->vnum == trouble_vnum &&
		wcsstr( obj->short_descr, ch->name))
	 {
	  extract_obj( obj );
	  break;
	 }
	}
        obj = create_object(get_obj_index(trouble_vnum),ch->level);

	 if ( gorev_ekipmani_mi ( obj ) )
	 {
	  swprintf( buf, MAX_STRING_LENGTH-1, obj->short_descr,
      IS_GOOD(ch) ? "parlak" :
  		IS_NEUTRAL(ch) ? "mavi-yeşil" : "kızıl", ch->name);
	  free_string( obj->short_descr );
	  obj->short_descr = str_dup( buf );
	 }
   act( L"$N $p objesini $e veriyor.", ch, obj, questman, TO_ROOM );
   act( L"$N $p objesini sana veriyor.",   ch, obj, questman, TO_CHAR );
	obj_to_char(obj, ch);
     return;
    }

    else if (!wcscmp(arg1, L"iade"))
  	{
  		if (arg2[0] == '\0')
  		{
  			printf_to_char(ch,L"Hangi görev ekipmanını iade etmek istiyorsun?\n\rKullanım: {Rgörev iade <ekipman>{x\n\r");
  			return;
  		}
  		else if (is_name(arg2, (wchar_t*)"kemer"))
  		{
  			if( !IS_SET(ch->quest,QUEST_GIRTH) )
  			{
  				printf_to_char(ch,L"Bu eşyayı zaten almamışsın.\n\r");
  				return;
  			}
  			for(obj = object_list; obj != NULL; obj = obj_next)
  			{
  				obj_next = obj->next;
  				if ( (obj->pIndexData->vnum == QUEST_ITEM1) && wcsstr( obj->short_descr, ch->name))
  				{
  					extract_obj( obj );
  					break;
  				}
  			}
  			REMOVE_BIT( ch->quest , QUEST_GIRTH );
  			ch->pcdata->questpoints += 900;
  			printf_to_char(ch,L"İade işlemi tamamlandı. 900 GP hesabına geçti.\n\r");
  			return;
  		}
  		else if (is_name(arg2, (wchar_t*)"çanta"))
  		{
  			if( !IS_SET(ch->quest,QUEST_BACKPACK) )
  			{
  				printf_to_char(ch,L"Bu eşyayı zaten almamışsın.\n\r");
  				return;
  			}
  			for(obj = object_list; obj != NULL; obj = obj_next)
  			{
  				obj_next = obj->next;
  				if ( (obj->pIndexData->vnum == QUEST_ITEM4) && wcsstr( obj->short_descr, ch->name))
  				{
  					extract_obj( obj );
  					break;
  				}
  			}
  			REMOVE_BIT( ch->quest , QUEST_BACKPACK );
  			ch->pcdata->questpoints += 4500;
  			printf_to_char(ch,L"İade işlemi tamamlandı. 4500 GP hesabına geçti.\n\r");
  			return;
  		}
  		else if (is_name(arg2, (wchar_t*)"testi"))
  		{
  			if( !IS_SET(ch->quest,QUEST_DECANTER) )
  			{
  				printf_to_char(ch,L"Bu eşyayı zaten almamışsın.\n\r");
  				return;
  			}
  			for(obj = object_list; obj != NULL; obj = obj_next)
  			{
  				obj_next = obj->next;
  				if ( (obj->pIndexData->vnum == QUEST_ITEM5) && wcsstr( obj->short_descr, ch->name))
  				{
  					extract_obj( obj );
  					break;
  				}
  			}
  			REMOVE_BIT( ch->quest , QUEST_DECANTER );
  			ch->pcdata->questpoints += 450;
  			printf_to_char(ch,L"İade işlemi tamamlandı. 450 GP hesabına geçti.\n\r");
  			return;
  		}
  		else if (is_name(arg2, (wchar_t*)"parlak"))
  		{
  			if( !IS_SET(ch->quest,QUEST_SILAH1) )
  			{
  				printf_to_char(ch,L"Bu eşyayı zaten almamışsın.\n\r");
  				return;
  			}
  			for(obj = object_list; obj != NULL; obj = obj_next)
  			{
  				obj_next = obj->next;
  				if ( (obj->pIndexData->vnum == QUEST_ITEM_SILAH1) && wcsstr( obj->short_descr, ch->name))
  				{
  					extract_obj( obj );
  					break;
  				}
  			}
  			REMOVE_BIT( ch->quest , QUEST_SILAH1 );
  			ch->pcdata->questpoints += 900;
  			printf_to_char(ch,L"İade işlemi tamamlandı. 900 GP hesabına geçti.\n\r");
  			return;
  		}
      else if (is_name(arg2, (wchar_t*)"mat"))
  		{
  			if( !IS_SET(ch->quest,QUEST_SILAH2) )
  			{
  				printf_to_char(ch,L"Bu eşyayı zaten almamışsın.\n\r");
  				return;
  			}
  			for(obj = object_list; obj != NULL; obj = obj_next)
  			{
  				obj_next = obj->next;
  				if ( (obj->pIndexData->vnum == QUEST_ITEM_SILAH2) && wcsstr( obj->short_descr, ch->name))
  				{
  					extract_obj( obj );
  					break;
  				}
  			}
  			REMOVE_BIT( ch->quest , QUEST_SILAH2 );
  			ch->pcdata->questpoints += 900;
  			printf_to_char(ch,L"İade işlemi tamamlandı. 900 GP hesabına geçti.\n\r");
  			return;
  		}
  		else if (is_name(arg2, (wchar_t*)"işlemeli"))
  		{
  			if( !IS_SET(ch->quest,QUEST_YUZUK1) )
  			{
  				printf_to_char(ch,L"Bu eşyayı zaten almamışsın.\n\r");
  				return;
  			}
  			for(obj = object_list; obj != NULL; obj = obj_next)
  			{
  				obj_next = obj->next;
  				if ( (obj->pIndexData->vnum == QUEST_ITEM_YUZUK1) && wcsstr( obj->short_descr, ch->name))
  				{
  					extract_obj( obj );
  					break;
  				}
  			}
  			REMOVE_BIT( ch->quest , QUEST_YUZUK1 );
  			ch->pcdata->questpoints += 675;
  			printf_to_char(ch,L"İade işlemi tamamlandı. 675 GP hesabına geçti.\n\r");
  			return;
  		}
      else if (is_name(arg2, (wchar_t*)"desenli"))
  		{
  			if( !IS_SET(ch->quest,QUEST_YUZUK2) )
  			{
  				printf_to_char(ch,L"Bu eşyayı zaten almamışsın.\n\r");
  				return;
  			}
  			for(obj = object_list; obj != NULL; obj = obj_next)
  			{
  				obj_next = obj->next;
  				if ( (obj->pIndexData->vnum == QUEST_ITEM_YUZUK2) && wcsstr( obj->short_descr, ch->name))
  				{
  					extract_obj( obj );
  					break;
  				}
  			}
  			REMOVE_BIT( ch->quest , QUEST_YUZUK2 );
  			ch->pcdata->questpoints += 675;
  			printf_to_char(ch,L"İade işlemi tamamlandı. 675 GP hesabına geçti.\n\r");
  			return;
  		}
      else if (is_name(arg2, (wchar_t*)"oymalı"))
  		{
  			if( !IS_SET(ch->quest,QUEST_YUZUK3) )
  			{
  				printf_to_char(ch,L"Bu eşyayı zaten almamışsın.\n\r");
  				return;
  			}
  			for(obj = object_list; obj != NULL; obj = obj_next)
  			{
  				obj_next = obj->next;
  				if ( (obj->pIndexData->vnum == QUEST_ITEM_YUZUK3) && wcsstr( obj->short_descr, ch->name))
  				{
  					extract_obj( obj );
  					break;
  				}
  			}
  			REMOVE_BIT( ch->quest , QUEST_YUZUK3 );
  			ch->pcdata->questpoints += 675;
  			printf_to_char(ch,L"İade işlemi tamamlandı. 675 GP hesabına geçti.\n\r");
  			return;
  		}
      else if (is_name(arg2, (wchar_t*)"kakmalı"))
  		{
  			if( !IS_SET(ch->quest,QUEST_YUZUK4) )
  			{
  				printf_to_char(ch,L"Bu eşyayı zaten almamışsın.\n\r");
  				return;
  			}
  			for(obj = object_list; obj != NULL; obj = obj_next)
  			{
  				obj_next = obj->next;
  				if ( (obj->pIndexData->vnum == QUEST_ITEM_YUZUK4) && wcsstr( obj->short_descr, ch->name))
  				{
  					extract_obj( obj );
  					break;
  				}
  			}
  			REMOVE_BIT( ch->quest , QUEST_YUZUK4 );
  			ch->pcdata->questpoints += 675;
  			printf_to_char(ch,L"İade işlemi tamamlandı. 675 GP hesabına geçti.\n\r");
  			return;
  		}


  		else
  		{
  			printf_to_char(ch,L"Hangi görev ekipmanını iade etmek istiyorsun?\n\rKullanım: {Rgörev iade <ekipman>{x\n\r");
  			return;
  		}
  	}

    send_to_char( L"GÖREV KOMUTLARI: puan bilgi zaman iste bitti iptal liste özellk satınal sigorta iade.\n\r",ch);
    send_to_char( L"Daha fazla bilgi için: yardım görev.\n\r",ch);
    return;
}

CHAR_DATA * find_a_quest_mob( CHAR_DATA *ch )
{
  CHAR_DATA *victim;
  CHAR_DATA *victim_next = NULL;
  QUEST_INDEX_DATA *quest_mob_list;
  QUEST_INDEX_DATA *pQuestMob;
  QUEST_INDEX_DATA *pQuestMob_next=NULL;
  int mob_count, selected_mob, level_diff;

  mob_count = 0;
  quest_mob_list = NULL;
  pQuestMob = NULL;
  selected_mob = 0;


  for ( victim = char_list; victim != NULL; victim = victim_next )
  {
    victim_next = victim->next;
    if (!IS_NPC(victim))
    {
      continue;
    }
    level_diff = victim->level - ch->level;
    if (level_diff > 5 || level_diff < -5)
    {
      continue;
    }
    if ( IS_SET(victim->act,ACT_TRAIN) || IS_SET(victim->act,ACT_PRACTICE)
      || IS_SET(victim->act,ACT_IS_HEALER) || IS_SET(victim->act,ACT_NOTRACK) )
    {
        continue;
    }

    if( victim->pIndexData->vnum == MOB_VNUM_SHADOW || victim->pIndexData->vnum == MOB_VNUM_SPECIAL_GUARD
    || victim->pIndexData->vnum == MOB_VNUM_BEAR || victim->pIndexData->vnum == MOB_VNUM_DEMON
    || victim->pIndexData->vnum == MOB_VNUM_NIGHTWALKER || victim->pIndexData->vnum == MOB_VNUM_STALKER
    || victim->pIndexData->vnum == MOB_VNUM_SQUIRE || victim->pIndexData->vnum == MOB_VNUM_MIRROR_IMAGE
    || victim->pIndexData->vnum == MOB_VNUM_UNDEAD || victim->pIndexData->vnum == MOB_VNUM_LION
    || victim->pIndexData->vnum == MOB_VNUM_WOLF || victim->pIndexData->vnum == MOB_VNUM_LESSER_GOLEM
    || victim->pIndexData->vnum == MOB_VNUM_STONE_GOLEM || victim->pIndexData->vnum == MOB_VNUM_IRON_GOLEM
    || victim->pIndexData->vnum == MOB_VNUM_ADAMANTITE_GOLEM || victim->pIndexData->vnum == MOB_VNUM_HUNTER
    || victim->pIndexData->vnum == MOB_VNUM_SUM_SHADOW || victim->pIndexData->vnum == MOB_VNUM_DOG
    || victim->pIndexData->vnum == MOB_VNUM_ELM_EARTH || victim->pIndexData->vnum == MOB_VNUM_ELM_AIR
    || victim->pIndexData->vnum == MOB_VNUM_ELM_FIRE || victim->pIndexData->vnum == MOB_VNUM_ELM_WATER
    || victim->pIndexData->vnum == MOB_VNUM_ELM_LIGHT || victim->pIndexData->vnum == MOB_VNUM_WEAPON
    || victim->pIndexData->vnum == MOB_VNUM_ARMOR )
    {
        continue;
    }

    /* kabal moblarini eleyelim */
    if( victim->pIndexData->vnum >= 500 && victim->pIndexData->vnum <= 580 )
    {
        continue;
    }

    if ( (victim->in_room == NULL) || (room_has_exit( victim->in_room ) == FALSE ) )
    {
      continue;
    }

    if ( IS_GOOD(victim) && IS_GOOD(ch) )
    {
      continue;
    }

    mob_count++;
    pQuestMob = (QUEST_INDEX_DATA *)alloc_mem(sizeof(*pQuestMob));
    pQuestMob->mob = victim;
    pQuestMob->next = quest_mob_list;
    quest_mob_list = pQuestMob;

  }
  if (mob_count == 0)
  {
    return NULL;
  }
  selected_mob = number_range(1,mob_count);

  mob_count = 0;
  for ( pQuestMob = quest_mob_list; pQuestMob != NULL; pQuestMob = pQuestMob_next )
  {
    pQuestMob_next = pQuestMob->next;

    mob_count++;
    if (mob_count == selected_mob)
    {
      victim = pQuestMob->mob;
      break;
    }
  }
  return victim;
}

void generate_quest(CHAR_DATA *ch, CHAR_DATA *questman)
{
    wchar_t buf [MAX_STRING_LENGTH];
    char_data *victim;

    victim = find_a_quest_mob(ch);

    if (victim == NULL)
    {
      swprintf( buf, MAX_STRING_LENGTH-1, L"Üzgünüm ama şu an sana verebileceğim bir görev yok.");
      do_tell_quest(ch,questman,buf);
      swprintf( buf, MAX_STRING_LENGTH-1, L"Daha sonra tekrar dene.");
      do_tell_quest(ch,questman,buf);
      ch->pcdata->nextquest = 5;
      return;
    }

    if (IS_GOOD(ch))
    {
      swprintf( buf, MAX_STRING_LENGTH-1, L"Diyarın azılı asilerinden %s,	zindandan kaçtı!",victim->short_descr);
      do_tell_quest(ch,questman,buf);
      swprintf( buf, MAX_STRING_LENGTH-1, L"Kaçışından bu yana tahminimizce %d sivili katletti!", number_range(2,20));
      do_tell_quest(ch,questman,buf);
      do_tell_quest(ch,questman,(wchar_t*)"Bunun cezası ölümdür!");
    }
    else
    {
      swprintf( buf, MAX_STRING_LENGTH-1, L"Şahsi düşmanım %s, kraliyet tacına karşı tehdit oluşturuyor.",victim->short_descr);
      do_tell_quest(ch,questman,buf);
      swprintf( buf, MAX_STRING_LENGTH-1, L"Bu tehdit yokedilmeli!");
      do_tell_quest(ch,questman,buf);
    }

    if (victim->in_room->name != NULL)
    {
      swprintf( buf, MAX_STRING_LENGTH-1, L"%s şu sıralar %s bölgesindedir!",victim->short_descr,victim->in_room->area->name);
      do_tell_quest(ch,questman,buf);

      /* I changed my area names so that they have just the name of the area
      and none of the level stuff. You may want to comment these next two
      lines. - Vassago */

      swprintf( buf, MAX_STRING_LENGTH-1, L"Yeri %s civarında.",victim->in_room->name);
      do_tell_quest(ch,questman,buf);
    }

    ch->pcdata->questmob = victim->pIndexData->vnum;
    ch->pcdata->questroom = victim->in_room->vnum;
    return;
}

/* Called from update_handler() by pulse_area */

void quest_update(void)
{
    CHAR_DATA *ch, *ch_next;

    for ( ch = char_list; ch != NULL; ch = ch_next )
    {
        ch_next = ch->next;

	if (IS_NPC(ch)) continue;

	if (ch->pcdata->nextquest > 0)
	{
	    ch->pcdata->nextquest--;

	    if (ch->pcdata->nextquest == 0)
	    {
        send_to_char( L"Yeniden görev isteyebilirsin.\n\r",ch);
	        continue;
	    }
	}
        else if (IS_SET(ch->act,PLR_QUESTOR))
        {
	    if (--ch->pcdata->countdown <= 0)
	    {

	        ch->pcdata->nextquest = 0;
          send_to_char( L"Görev süren doldu!\n\rYeni bir görev isteyebilirsin.\n\r", ch);
	        REMOVE_BIT(ch->act, PLR_QUESTOR);

                ch->pcdata->questgiver = 0;
                ch->pcdata->countdown = 0;
                ch->pcdata->questmob = 0;
                ch->pcdata->questroom = 0;
	    }
	    if (ch->pcdata->countdown > 0 && ch->pcdata->countdown < 6)
	    {
        send_to_char( L"Acele et, görev süren dolmak üzere!\n\r",ch);
	        continue;
	    }
        }
    }
    return;
}

void do_tell_quest( CHAR_DATA *ch, CHAR_DATA *victim, wchar_t *argument )
{

	    act_color( L"$N: $C$t$c",ch,argument,victim,TO_CHAR,POS_DEAD, CLR_MAGENTA_BOLD );

		return;
}

CHAR_DATA *get_quest_world( CHAR_DATA *ch, MOB_INDEX_DATA *victim)
{
    CHAR_DATA *wch;

    for ( wch = char_list; wch != NULL ; wch = wch->next )
    {
        if ( wch->in_room == NULL
		|| wch->pIndexData != victim)
            continue;

	return wch;
    }

    return NULL;
}

bool gorev_ekipmani_mi( OBJ_DATA *obj )
{
  if ( obj->pIndexData->vnum == QUEST_ITEM1 || obj->pIndexData->vnum == QUEST_ITEM_YUZUK1
       || obj->pIndexData->vnum == QUEST_ITEM_YUZUK2 || obj->pIndexData->vnum == QUEST_ITEM_YUZUK3
       || obj->pIndexData->vnum == QUEST_ITEM_YUZUK4 || obj->pIndexData->vnum == QUEST_ITEM_SILAH1
       || obj->pIndexData->vnum == QUEST_ITEM_SILAH2 || obj->pIndexData->vnum == QUEST_ITEM4
       || obj->pIndexData->vnum == QUEST_ITEM5
     )
  {
    return TRUE;
  }

  return FALSE;
}

void do_eniyi(CHAR_DATA *ch,wchar_t *argument)
{
	CHAR_DATA *questman;
	wchar_t buf[MAX_STRING_LENGTH];
	int bolge=0,seviye=0,zz1zz=0,zz2zz=0,zz3zz=0,zararzari=0,ac1ac=0,ac2ac=0,ac3ac=0,actoplam=0;
	wchar_t arg1 [MAX_INPUT_LENGTH];
	OBJ_DATA *obj,*zz1,*zz2,*zz3,*ac1,*ac2,*ac3;
	AFFECT_DATA *paf;

	for ( questman = ch->in_room->people; questman != NULL; questman = questman->next_in_room )
    {
	if (!IS_NPC(questman)) continue;
        if (questman->spec_fun == spec_lookup( (wchar_t*)"spec_questmaster" )) break;
    }

    if (questman == NULL || questman->spec_fun != spec_lookup( (wchar_t*)"spec_questmaster" ))
    {
        send_to_char( L"Odada bu işleri yapan bir görevci göremiyorum.\n\r", ch);
        return;
    }

if ( argument[0] == '\0' )
	{
		send_to_char( L"Hangi bölgene giyeceğin ekipmanlar hakkında bilgi istiyorsun?\n\r",ch);
		return;
	}
	argument = one_argument(argument, arg1);

	seviye = ch->level;
	bolge=-1;
	if(!wcscmp(arg1, L"parmak"))
		bolge=ITEM_WEAR_FINGER;
	else if(!wcscmp(arg1, L"boyun"))
		bolge=ITEM_WEAR_NECK;
	else if(!wcscmp(arg1, L"gövde"))
		bolge=ITEM_WEAR_BODY;
	else if(!wcscmp(arg1, L"kafa"))
		bolge=ITEM_WEAR_HEAD;
	else if(!wcscmp(arg1, L"bacaklar"))
		bolge=ITEM_WEAR_LEGS;
	else if(!wcscmp(arg1, L"ayaklar"))
		bolge=ITEM_WEAR_FEET;
	else if(!wcscmp(arg1, L"eller"))
		bolge=ITEM_WEAR_HANDS;
	else if(!wcscmp(arg1, L"kollar"))
		bolge=ITEM_WEAR_ARMS;
	else if(!wcscmp(arg1, L"vücut"))
		bolge=ITEM_WEAR_ABOUT;
	else if(!wcscmp(arg1, L"bel"))
		bolge=ITEM_WEAR_WAIST;
	else if(!wcscmp(arg1, L"bilek"))
		bolge=ITEM_WEAR_WRIST;
	else if(!wcscmp(arg1, L"süzülen"))
		bolge=ITEM_WEAR_FLOAT;
	else
	{
		send_to_char( L"Vücudunda böyle bir bölge göremiyorum!\n\r",ch);
		return;
	}
	if(ch->silver<((ch->level/2)+1))
	{
		send_to_char( L"Yeterli akçen yok, bilgi veremem.\n\r",ch);
		return;
	}

	zz1=NULL;
	zz2=NULL;
	zz3=NULL;
	ac1=NULL;
	ac2=NULL;
	ac3=NULL;

	act( L"$n $Z ekipman bilgisi istiyor.", ch, NULL, questman, TO_ROOM);

	for( obj=object_list; obj!=NULL; obj = obj->next )
	{
		zararzari=0;
		actoplam=0;
		if ( CAN_WEAR(obj,ITEM_TAKE) &&
			 CAN_WEAR( obj, bolge ) &&
			 obj->level <= seviye)
		{
			if (!obj->enchanted)
			{
				for ( paf = obj->pIndexData->affected; paf != NULL; paf = paf->next )
				{
					if ( paf->location != APPLY_NONE && paf->modifier != 0 )
					{
						if( (paf->location==APPLY_DAMROLL) || (paf->location==APPLY_HITROLL) )
						{
							zararzari += paf->modifier;
						}
					}
				}//for
				if( (zararzari>=zz1zz) )
				{
					zz3=zz2;
					zz2=zz1;
					zz1=obj;
					zz1zz=zararzari;
				}
				else if( (zararzari>=zz2zz) )
				{
					zz3=zz2;
					zz2=obj;
					zz2zz=zararzari;
				}
				else if( (zararzari>=zz3zz) )
				{
					zz3=obj;
					zz3zz=zararzari;
				}

			}//if (!obj->enchanted)
			if(obj->item_type==ITEM_ARMOR)
			{
				actoplam = obj->value[0] + obj->value[1] + obj->value[2] + obj->value[3];
				if( (actoplam>=ac1ac))
				{
					ac3=ac2;
					ac2=ac1;
					ac1=obj;
					ac1ac=actoplam;
				}
				else if ( (actoplam>=ac2ac) )
				{
					ac3=ac2;
					ac2=obj;
					ac2ac=actoplam;
				}
				else if( (actoplam>=ac3ac))
				{
					ac3=obj;
					ac3ac=actoplam;
				}
			}
		}//if can_wear
	}//for
	if(zz1== NULL && ac1==NULL)
	{
		do_tell_quest(ch,questman,(wchar_t*)"Şu an birşey hatırlayamıyorum. Sanırım yaşlanıyorum.");
		do_tell_quest(ch,questman,(wchar_t*)"Daha sonra tekrar uğra lütfen.");
		return;
	}
	ch->silver -= (ch->level/2)+1;
	do_tell_quest(ch,questman,(wchar_t*)"Bir düşüneyim... Evet sanırım birşeyler hatırladım.");
	do_tell_quest(ch,questman,(wchar_t*)"Bazı ekipmanlar hatırlıyorum, senin giyebileceğin seviyede ekipmanlar.");

	if(zz1 != NULL)
	{
		do_tell_quest(ch,questman,(wchar_t*)"Vuruşlarının gücünü ve isabetini artıracak ekipmanlar. Mesela...");
		swprintf( buf, MAX_STRING_LENGTH-1, L"%s",zz1->short_descr);
		do_tell_quest(ch,questman,(wchar_t*)buf);
	}
	if(zz2 != NULL)
	{
		swprintf( buf, MAX_STRING_LENGTH-1, L"%s",zz2->short_descr);
		do_tell_quest(ch,questman,(wchar_t*)buf);
	}
	if(zz3 != NULL)
	{
		swprintf( buf, MAX_STRING_LENGTH-1, L"%s",zz3->short_descr);
		do_tell_quest(ch,questman,(wchar_t*)buf);
	}
	if(ac1 != NULL)
	{
		do_tell_quest(ch,questman,(wchar_t*)"Bir de seni koruyacak ekipmanlar var aklıma gelen. Mesela...");
		swprintf( buf, MAX_STRING_LENGTH-1, L"%s",ac1->short_descr);
		do_tell_quest(ch,questman,(wchar_t*)buf);
	}
	if(ac2 != NULL)
	{
		swprintf( buf, MAX_STRING_LENGTH-1, L"%s",ac2->short_descr);
		do_tell_quest(ch,questman,(wchar_t*)buf);
	}
	if(ac3 != NULL)
	{
		swprintf( buf, MAX_STRING_LENGTH-1, L"%s",ac3->short_descr);
		do_tell_quest(ch,questman,(wchar_t*)buf);
	}
}
