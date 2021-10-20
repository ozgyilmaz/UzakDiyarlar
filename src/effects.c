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
#include "recycle.h"

void acid_effect(void *vo, int level, int dam, int target)
{
    if (target == TARGET_ROOM) /* nail objects on the floor */
     {
	ROOM_INDEX_DATA *room = (ROOM_INDEX_DATA *) vo;
	OBJ_DATA *obj, *obj_next;

	for (obj = room->contents; obj != NULL; obj = obj_next)
	{
	    obj_next = obj->next_content;
	    acid_effect(obj,level,dam,TARGET_OBJ);
	}
	return;
    }

    if (target == TARGET_CHAR)  /* do the effect on a victim */
    {
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	OBJ_DATA *obj, *obj_next;

	/* let's toast some gear */
	for (obj = victim->carrying; obj != NULL; obj = obj_next)
	{
	    obj_next = obj->next_content;
	    acid_effect(obj,level,dam,TARGET_OBJ);
	}
	return;
    }

    if (target == TARGET_OBJ) /* toast an object */
    {
	OBJ_DATA *obj = (OBJ_DATA *) vo;
	OBJ_DATA *t_obj,*n_obj;
	int chance;
	const wchar_t *msg;

	if (IS_OBJ_STAT(obj,ITEM_BURN_PROOF)
	||  IS_OBJ_STAT(obj,ITEM_NOPURGE)
	||  obj->pIndexData->limit != -1
	||  number_range(0,4) == 0)
	    return;

	chance = level / 4 + dam / 10;

	if (chance > 25)
	    chance = (chance - 25) / 2 + 25;
	 if (chance > 50)
	    chance = (chance - 50) / 2 + 50;

	if (IS_OBJ_STAT(obj,ITEM_BLESS))
	    chance -= 5;

	chance -= obj->level * 2;

	switch (obj->item_type)
	{
	    default:
		return;
	    case ITEM_CONTAINER:
	    case ITEM_CORPSE_PC:
	    case ITEM_CORPSE_NPC:
      msg = L"$p duman çıkararak eriyor.";
		break;
	    case ITEM_ARMOR:
      msg = L"$p delik delik oldu.";
		break;
	    case ITEM_CLOTHING:
      msg = L"$p çürüyerek parçalara ayrıldı.";
	 	break;
	    case ITEM_STAFF:
	    case ITEM_WAND:
		chance -= 10;
    msg = L"$p çürüyerek kırıldı.";
		break;
	    case ITEM_SCROLL:
		chance += 10;
    msg = L"$p işe yaramaz hale geldi.";
		break;
	}

	chance = URANGE(5,chance,95);

	if (number_percent() > chance)
	    return;

	if (obj->carried_by != NULL)
	    act(msg,obj->carried_by,obj,NULL,TO_ALL);
	else if (obj->in_room != NULL && obj->in_room->people != NULL)
	    act(msg,obj->in_room->people,obj,NULL,TO_ALL);

	if (obj->item_type == ITEM_ARMOR)  /* etch it */
	{
	    AFFECT_DATA *paf;
	    bool af_found = FALSE;
	    int i;

 	    affect_enchant(obj);

	    for ( paf = obj->affected; paf != NULL; paf = paf->next)
            {
                if ( paf->location == APPLY_AC)
                {
                    af_found = TRUE;
                    paf->type = -1;
                    paf->modifier += 1;
                    paf->level = UMAX(paf->level,level);
		    break;
                }
            }

            if (!af_found)
            /* needs a new affect */
            {
		paf = new_affect();

                paf->type       = -1;
                paf->level      = level;
                paf->duration   = -1;
                paf->location   = APPLY_AC;
                paf->modifier   =  1;
                paf->bitvector  = 0;
                paf->next       = obj->affected;
                obj->affected   = paf;
            }

            if (obj->carried_by != NULL && obj->wear_loc != WEAR_NONE)
                for (i = 0; i < 4; i++)
                    obj->carried_by->armor[i] += 1;
            return;
	}

	/* get rid of the object */
	if (obj->contains)  /* dump contents */
	{
	    for (t_obj = obj->contains; t_obj != NULL; t_obj = n_obj)
	    {
		n_obj = t_obj->next_content;
		obj_from_obj(t_obj);
		if (obj->in_room != NULL)
		    obj_to_room(t_obj,obj->in_room);
		else if (obj->carried_by != NULL)
		    obj_to_room(t_obj,obj->carried_by->in_room);
		else
		{
		    extract_obj(t_obj);
		    continue;
		}

		acid_effect(t_obj,level/2,dam/2,TARGET_OBJ);
	    }
 	}

	extract_obj(obj);
	return;
    }
}


void cold_effect(void *vo, int level, int dam, int target)
{
    if (target == TARGET_ROOM) /* nail objects on the floor */
    {
        ROOM_INDEX_DATA *room = (ROOM_INDEX_DATA *) vo;
        OBJ_DATA *obj, *obj_next;

        for (obj = room->contents; obj != NULL; obj = obj_next)
        {
            obj_next = obj->next_content;
            cold_effect(obj,level,dam,TARGET_OBJ);
        }
        return;
    }

    if (target == TARGET_CHAR) /* whack a character */
    {
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	OBJ_DATA *obj, *obj_next;

	/* chill touch effect */
	if (!saves_spell(level/4 + dam / 20, victim, DAM_COLD))
	{
	    AFFECT_DATA af;

      act( L"$s morarıyor ve titriyor.",victim,NULL,NULL,TO_ROOM);
act( L"Soğuk kemiklerinin içine işliyor.",victim,NULL,NULL,TO_CHAR);
            af.where     = TO_AFFECTS;
            af.type      = skill_lookup( L"chill touch");
            af.level     = level;
            af.duration  = 6;
            af.location  = APPLY_STR;
            af.modifier  = -1;
            af.bitvector = 0;
            affect_join( victim, &af );
	}

	/* hunger! (warmth sucked out */
	if (!IS_NPC(victim))
	    gain_condition(victim,COND_HUNGER,dam/20);

	/* let's toast some gear */
	for (obj = victim->carrying; obj != NULL; obj = obj_next)
	{
	    obj_next = obj->next_content;
	    cold_effect(obj,level,dam,TARGET_OBJ);
	}
	return;
   }

   if (target == TARGET_OBJ) /* toast an object */
   {
	OBJ_DATA *obj = (OBJ_DATA *) vo;
	int chance;
	const wchar_t *msg;

	if (IS_OBJ_STAT(obj,ITEM_BURN_PROOF)
	||  IS_OBJ_STAT(obj,ITEM_NOPURGE)
	||  obj->pIndexData->limit != -1
	||  number_range(0,4) == 0)
	    return;

	chance = level / 4 + dam / 10;

	if (chance > 25)
	    chance = (chance - 25) / 2 + 25;
	if (chance > 50)
	    chance = (chance - 50) / 2 + 50;

	if (IS_OBJ_STAT(obj,ITEM_BLESS))
	    chance -= 5;

 	chance -= obj->level * 2;

	switch(obj->item_type)
	{
	    default:
		return;
	    case ITEM_POTION:
      msg = L"$p donarak kırılıyor!";
		chance += 25;
		break;
	    case ITEM_DRINK_CON:
      msg = L"$p donarak kırılıyor!";
		chance += 5;
		break;
	}

	chance = URANGE(5,chance,95);

	if (number_percent() > chance)
	    return;

	if (obj->carried_by != NULL)
	    act(msg,obj->carried_by,obj,NULL,TO_ALL);
	else if (obj->in_room != NULL && obj->in_room->people != NULL)
	    act(msg,obj->in_room->people,obj,NULL,TO_ALL);

	extract_obj(obj);
	return;
    }
}



void fire_effect(void *vo, int level, int dam, int target)
{

    if (target == TARGET_ROOM)  /* nail objects on the floor */
    {
	ROOM_INDEX_DATA *room = (ROOM_INDEX_DATA *) vo;
	OBJ_DATA *obj, *obj_next;
	for (obj = room->contents; obj != NULL; obj = obj_next)
	{
	    obj_next = obj->next_content;
	    fire_effect(obj,level,dam,TARGET_OBJ);
	}
	return;
    }

    if (target == TARGET_CHAR)   /* do the effect on a victim */
    {
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	OBJ_DATA *obj, *obj_next;

	/* chance of blindness */
	if (!IS_AFFECTED(victim,AFF_BLIND)
	&&  !saves_spell(level / 4 + dam / 20, victim,DAM_FIRE))
	{
            AFFECT_DATA af;
            act( L"$n dumandan kör oldu!",victim,NULL,NULL,TO_ROOM);
            act( L"Dumandan gözlerin taşarıyor...göremiyorsun!",
		victim,NULL,NULL,TO_CHAR);

            af.where        = TO_AFFECTS;
            af.type         = skill_lookup( L"fire breath");
            af.level        = level;
            af.duration     = number_range(0,level/10);
            af.location     = APPLY_HITROLL;
            af.modifier     = -4;
            af.bitvector    = AFF_BLIND;

            affect_to_char(victim,&af);
	}

	/* getting thirsty */
	if (!IS_NPC(victim))
	    gain_condition(victim,COND_THIRST,dam/20);

	/* let's toast some gear! */
	for (obj = victim->carrying; obj != NULL; obj = obj_next)
	{
	    obj_next = obj->next_content;

	    fire_effect(obj,level,dam,TARGET_OBJ);
        }
	return;
    }

    if (target == TARGET_OBJ)  /* toast an object */
    {
	OBJ_DATA *obj = (OBJ_DATA *) vo;
	OBJ_DATA *t_obj,*n_obj;
	int chance;
	const wchar_t *msg;

    	if (IS_OBJ_STAT(obj,ITEM_BURN_PROOF)
        ||  IS_OBJ_STAT(obj,ITEM_NOPURGE)
	||  obj->pIndexData->limit != -1
	||  number_range(0,4) == 0)
            return;

        chance = level / 4 + dam / 10;

        if (chance > 25)
            chance = (chance - 25) / 2 + 25;
        if (chance > 50)
            chance = (chance - 50) / 2 + 50;

        if (IS_OBJ_STAT(obj,ITEM_BLESS))
            chance -= 5;
        chance -= obj->level * 2;

        if  ( check_material( obj, (wchar_t*)"ice" ) )  {
          chance += 30;
          msg = L"$p eriyerek buharlaşıyor!";
        }
        else
        switch ( obj->item_type )
        {
        default:
	    return;
        case ITEM_CONTAINER:
        msg = L"$p tutuşarak yanıyor!";
              break;
          case ITEM_POTION:
              chance += 25;
  			msg = L"$p fokurdayarak kaynıyor!";
              break;
          case ITEM_SCROLL:
              chance += 50;
  			msg = L"$p çatırdayarak yanıyor!";
              break;
          case ITEM_STAFF:
              chance += 10;
              msg = L"$p duman çıkararak kömürleşiyor!";
              break;
          case ITEM_WAND:
              msg = L"$p kıvılcımlar çıkararak çatırdıyor!";
              break;
          case ITEM_FOOD:
              msg = L"$p kavrularak kararıyor!";
              break;
          case ITEM_PILL:
              msg = L"$p eriyerek damlamaya başlıyor!";
              break;
        }

        chance = URANGE(5,chance,95);

        if (number_percent() > chance)
            return;

	if (obj->carried_by != NULL)
            act( msg, obj->carried_by, obj, NULL, TO_ALL );
	else if (obj->in_room != NULL && obj->in_room->people != NULL)
	    act(msg,obj->in_room->people,obj,NULL,TO_ALL);

        if (obj->contains)
        {
            /* dump the contents */

            for (t_obj = obj->contains; t_obj != NULL; t_obj = n_obj)
            {
                n_obj = t_obj->next_content;
                obj_from_obj(t_obj);
		if (obj->in_room != NULL)
                    obj_to_room(t_obj,obj->in_room);
		else if (obj->carried_by != NULL)
		    obj_to_room(t_obj,obj->carried_by->in_room);
		else
		{
		    extract_obj(t_obj);
		    continue;
		}
		fire_effect(t_obj,level/2,dam/2,TARGET_OBJ);
            }
        }

        extract_obj( obj );
	return;
    }
}

void poison_effect(void *vo,int level, int dam, int target)
{
    if (target == TARGET_ROOM)  /* nail objects on the floor */
    {
        ROOM_INDEX_DATA *room = (ROOM_INDEX_DATA *) vo;
        OBJ_DATA *obj, *obj_next;

        for (obj = room->contents; obj != NULL; obj = obj_next)
        {
            obj_next = obj->next_content;
            poison_effect(obj,level,dam,TARGET_OBJ);
        }
        return;
    }

    if (target == TARGET_CHAR)   /* do the effect on a victim */
    {
        CHAR_DATA *victim = (CHAR_DATA *) vo;
        OBJ_DATA *obj, *obj_next;

	/* chance of poisoning */
        if (!saves_spell(level / 4 + dam / 20,victim,DAM_POISON))
        {
	    AFFECT_DATA af;

      send_to_char( L"Zehrin damarlarında dolaştığını hissediyorsun.\n\r",victim);
      act( L"$n çok hasta görünüyor.",victim,NULL,NULL,TO_ROOM);

            af.where     = TO_AFFECTS;
            af.type      = gsn_poison;
            af.level     = level;
            af.duration  = level / 2;
            af.location  = APPLY_STR;
            af.modifier  = -1;
            af.bitvector = AFF_POISON;
            affect_join( victim, &af );
        }

	/* equipment */
	for (obj = victim->carrying; obj != NULL; obj = obj_next)
	{
	    obj_next = obj->next_content;
	    poison_effect(obj,level,dam,TARGET_OBJ);
	}
	return;
    }

    if (target == TARGET_OBJ)  /* do some poisoning */
    {
	OBJ_DATA *obj = (OBJ_DATA *) vo;
	int chance;


	if (IS_OBJ_STAT(obj,ITEM_BURN_PROOF)
  	||  IS_OBJ_STAT(obj,ITEM_BLESS)
	||  obj->pIndexData->limit != -1
	||  number_range(0,4) == 0)
	    return;

	chance = level / 4 + dam / 10;
	if (chance > 25)
	    chance = (chance - 25) / 2 + 25;
	if (chance > 50)
	    chance = (chance - 50) / 2 + 50;

	chance -= obj->level * 2;

	switch (obj->item_type)
	{
	    default:
		return;
	    case ITEM_FOOD:
		break;
	    case ITEM_DRINK_CON:
		if (obj->value[0] == obj->value[1])
		    return;
		break;
	}

	chance = URANGE(5,chance,95);

	if (number_percent() > chance)
	    return;

	obj->value[3] = 1;
	return;
    }
}


void shock_effect(void *vo,int level, int dam, int target)
{
    if (target == TARGET_ROOM)
    {
	ROOM_INDEX_DATA *room = (ROOM_INDEX_DATA *) vo;
	OBJ_DATA *obj, *obj_next;

	for (obj = room->contents; obj != NULL; obj = obj_next)
	{
	    obj_next = obj->next_content;
	    shock_effect(obj,level,dam,TARGET_OBJ);
	}
	return;
    }

    if (target == TARGET_CHAR)
    {
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	OBJ_DATA *obj, *obj_next;

	/* daze and confused? */
	if (!saves_spell(level/4 + dam/20,victim,DAM_LIGHTNING))
	{
    send_to_char( L"Kasların yanıt vermiyor.\n\r",victim);
	    DAZE_STATE(victim,UMAX(12,level/4 + dam/20));
	}

	/* toast some gear */
	for (obj = victim->carrying; obj != NULL; obj = obj_next)
	{
	    obj_next = obj->next_content;
	    shock_effect(obj,level,dam,TARGET_OBJ);
	}
	return;
    }

    if (target == TARGET_OBJ)
    {
	OBJ_DATA *obj = (OBJ_DATA *) vo;
	int chance;
	const wchar_t *msg;

	if (IS_OBJ_STAT(obj,ITEM_BURN_PROOF)
	||  IS_OBJ_STAT(obj,ITEM_NOPURGE)
	||  obj->pIndexData->limit != -1
	||  number_range(0,4) == 0)
	    return;

	chance = level / 4 + dam / 10;

	if (chance > 25)
	    chance = (chance - 25) / 2 + 25;
	if (chance > 50)
	    chance = (chance - 50) /2 + 50;

	if (IS_OBJ_STAT(obj,ITEM_BLESS))
	    chance -= 5;

 	chance -= obj->level * 2;

	switch(obj->item_type)
	{
	    default:
		return;
	   case ITEM_WAND:
	   case ITEM_STAFF:
		chance += 10;
    msg = L"$p aşırı yüklenerek patlıyor!";
		break;
	   case ITEM_JEWELRY:
		chance -= 10;
    msg = L"$p çatlayarak değersizleşiyor.";
	}

	chance = URANGE(5,chance,95);

	if (number_percent() > chance)
	    return;

	if (obj->carried_by != NULL)
	    act(msg,obj->carried_by,obj,NULL,TO_ALL);
	else if (obj->in_room != NULL && obj->in_room->people != NULL)
	    act(msg,obj->in_room->people,obj,NULL,TO_ALL);

	extract_obj(obj);
	return;
    }
}

void sand_effect(void *vo, int level, int dam, int target)
{
    if (target == TARGET_ROOM) /* nail objects on the floor */
     {
	ROOM_INDEX_DATA *room = (ROOM_INDEX_DATA *) vo;
	OBJ_DATA *obj, *obj_next;

	for (obj = room->contents; obj != NULL; obj = obj_next)
	{
	    obj_next = obj->next_content;
	    sand_effect(obj,level,dam,TARGET_OBJ);
	}
	return;
    }

    if (target == TARGET_CHAR)  /* do the effect on a victim */
    {
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	OBJ_DATA *obj, *obj_next;

	if (!IS_AFFECTED(victim,AFF_BLIND)
	&&  !saves_spell(level / 4 + dam / 20, victim,DAM_COLD))
	{
            AFFECT_DATA af;
            act( L"$n uçuşan kumlarla körleşti!",victim,NULL,NULL,TO_ROOM);
            act( L"Kum gözlerini yaşartıyor...göremiyorsun!",
		victim,NULL,NULL,TO_CHAR);

            af.where        = TO_AFFECTS;
            af.type         = skill_lookup( L"sand storm");
            af.level        = level;
            af.duration     = number_range(0,level/10);
            af.location     = APPLY_HITROLL;
            af.modifier     = -4;
            af.bitvector    = AFF_BLIND;

            affect_to_char(victim,&af);
	}

	/* let's toast some gear */
	for (obj = victim->carrying; obj != NULL; obj = obj_next)
	{
	    obj_next = obj->next_content;
	    sand_effect(obj,level,dam,TARGET_OBJ);
	}
	return;
    }

    if (target == TARGET_OBJ) /* toast an object */
    {
	OBJ_DATA *obj = (OBJ_DATA *) vo;
	OBJ_DATA *t_obj,*n_obj;
	int chance;
	const wchar_t *msg;

	if (IS_OBJ_STAT(obj,ITEM_BURN_PROOF)
	||  IS_OBJ_STAT(obj,ITEM_NOPURGE)
	||  obj->pIndexData->limit != -1
	||  number_range(0,4) == 0)
	    return;

	chance = level / 4 + dam / 10;

	if (chance > 25)
	    chance = (chance - 25) / 2 + 25;
	 if (chance > 50)
	    chance = (chance - 50) / 2 + 50;

	if (IS_OBJ_STAT(obj,ITEM_BLESS))
	    chance -= 5;

	chance -= obj->level * 2;

	switch (obj->item_type)
	{
	    default:
		return;
	    case ITEM_CONTAINER:
	    case ITEM_CORPSE_PC:
	    case ITEM_CORPSE_NPC:
		chance += 50;
    msg = L"$p kumla dolarak buharlaşıyor.";
		break;
	    case ITEM_ARMOR:
		chance -=10;
		msg = L"$p kumla yontuluyor.";
		break;
	    case ITEM_CLOTHING:
		msg = L"$p kumla aşınıyor.";
	 	break;
	    case ITEM_WAND:
		chance = 50;
		msg = L"$p çarpan kum taneleri ile şeklini kaybediyor.";
		break;
	    case ITEM_SCROLL:
		chance += 20;
		msg = L"$p kumla kaplanıyor.";
		break;
	    case ITEM_POTION:
		chance +=10;
		msg = L"$p çarpan kum taneleri ile parçalara ayrılıyor.";
		break;
	}

	chance = URANGE(5,chance,95);

	if (number_percent() > chance)
	    return;

	if (obj->carried_by != NULL)
	    act(msg,obj->carried_by,obj,NULL,TO_ALL);
	else if (obj->in_room != NULL && obj->in_room->people != NULL)
	    act(msg,obj->in_room->people,obj,NULL,TO_ALL);

	if (obj->item_type == ITEM_ARMOR)  /* etch it */
	{
	    AFFECT_DATA *paf;
	    bool af_found = FALSE;
	    int i;

 	    affect_enchant(obj);

	    for ( paf = obj->affected; paf != NULL; paf = paf->next)
            {
                if ( paf->location == APPLY_AC)
                {
                    af_found = TRUE;
                    paf->type = -1;
                    paf->modifier += 1;
                    paf->level = UMAX(paf->level,level);
		    break;
                }
            }

            if (!af_found)
            /* needs a new affect */
            {
		paf = new_affect();

                paf->type       = -1;
                paf->level      = level;
                paf->duration   = level;
                paf->location   = APPLY_AC;
                paf->modifier   =  1;
                paf->bitvector  = 0;
                paf->next       = obj->affected;
                obj->affected   = paf;
            }

            if (obj->carried_by != NULL && obj->wear_loc != WEAR_NONE)
                for (i = 0; i < 4; i++)
                    obj->carried_by->armor[i] += 1;
            return;
	}

	/* get rid of the object */
	if (obj->contains)  /* dump contents */
	{
	    for (t_obj = obj->contains; t_obj != NULL; t_obj = n_obj)
	    {
		n_obj = t_obj->next_content;
		obj_from_obj(t_obj);
		if (obj->in_room != NULL)
		    obj_to_room(t_obj,obj->in_room);
		else if (obj->carried_by != NULL)
		    obj_to_room(t_obj,obj->carried_by->in_room);
		else
		{
		    extract_obj(t_obj);
		    continue;
		}

		sand_effect(t_obj,level/2,dam/2,TARGET_OBJ);
	    }
 	}

	extract_obj(obj);
	return;
    }
}

void scream_effect(void *vo, int level, int dam, int target)
{

    if (target == TARGET_ROOM)  /* nail objects on the floor */
    {
	ROOM_INDEX_DATA *room = (ROOM_INDEX_DATA *) vo;
	OBJ_DATA *obj, *obj_next;
	for (obj = room->contents; obj != NULL; obj = obj_next)
	{
	    obj_next = obj->next_content;
	    scream_effect(obj,level,dam,TARGET_OBJ);
	}
	return;
    }

    if (target == TARGET_CHAR)   /* do the effect on a victim */
    {
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	OBJ_DATA *obj, *obj_next;

	if  (!saves_spell(level / 4 + dam / 20, victim,DAM_SOUND))
	{
            AFFECT_DATA af;
            act( L"$n duyamıyor!",victim,NULL,NULL,TO_ROOM);
            act( L"Duyamıyorsun!",victim,NULL,NULL,TO_CHAR);

            af.where        = TO_AFFECTS;
            af.type         = gsn_scream;
            af.level        = level;
            af.duration     = 0;
            af.location     = APPLY_NONE;
            af.modifier     = 0;
            af.bitvector    = AFF_SCREAM;

            affect_to_char(victim,&af);
	}

	/* daze and confused? */
	if (!saves_spell(level/4 + dam/20,victim,DAM_SOUND))
	{
    send_to_char( L"Duyamıyorsun!.\n\r",victim);
	    DAZE_STATE(victim,UMAX(12,level/4 + dam/20));
	}


	/* getting thirsty */
	if (!IS_NPC(victim))
	    gain_condition(victim,COND_THIRST,dam/20);

	/* let's toast some gear! */
	for (obj = victim->carrying; obj != NULL; obj = obj_next)
	{
	    obj_next = obj->next_content;

	    scream_effect(obj,level,dam,TARGET_OBJ);
        }
	return;
    }

    if (target == TARGET_OBJ)  /* toast an object */
    {
	OBJ_DATA *obj = (OBJ_DATA *) vo;
	OBJ_DATA *t_obj,*n_obj;
	int chance;
	const wchar_t *msg;

    	if (IS_OBJ_STAT(obj,ITEM_BURN_PROOF)
        ||  IS_OBJ_STAT(obj,ITEM_NOPURGE)
	||  number_range(0,4) == 0)
            return;

        chance = level / 4 + dam / 10;

        if (chance > 25)
            chance = (chance - 25) / 2 + 25;
        if (chance > 50)
            chance = (chance - 50) / 2 + 50;

        if (IS_OBJ_STAT(obj,ITEM_BLESS))
            chance -= 5;
        chance -= obj->level * 2;

        if  ( check_material( obj, (wchar_t*)"ice" ) )
	{
          chance += 30;
          msg = L"$p parçalanarak buharlaşıyor!";
        }
        else
	if ( check_material ( obj, (wchar_t*)"glass" ) )
	{
	 chance += 30;
   msg = L"$p küçük parçalara ayrılıyor.";
	}
	else
        switch ( obj->item_type )
        {
        default:
	    return;
        case ITEM_POTION:
            chance += 25;
            msg = L"$p şişesi kırılarak içindekini yere döküyor!";
            break;
        case ITEM_SCROLL:
            chance += 50;
            msg = L"$p küçük parçalara ayrılıyor!";
            break;
        case ITEM_DRINK_CON:
        msg = L"$p kırılıyor ve içindeki bozuluyor!";
	    chance += 5;
	    break;
        case ITEM_PILL:
        msg = L"$p parçalara ayrılıyor!";
            break;
        }

        chance = URANGE(5,chance,95);

        if (number_percent() > chance)
            return;

	if (obj->carried_by != NULL)
            act( msg, obj->carried_by, obj, NULL, TO_ALL );
	else if (obj->in_room != NULL && obj->in_room->people != NULL)
	    act(msg,obj->in_room->people,obj,NULL,TO_ALL);

        if (obj->contains)
        {
            /* dump the contents */

            for (t_obj = obj->contains; t_obj != NULL; t_obj = n_obj)
            {
                n_obj = t_obj->next_content;
                obj_from_obj(t_obj);
		if (obj->in_room != NULL)
                    obj_to_room(t_obj,obj->in_room);
		else if (obj->carried_by != NULL)
		    obj_to_room(t_obj,obj->carried_by->in_room);
		else
		{
		    extract_obj(t_obj);
		    continue;
		}
		scream_effect(t_obj,level/2,dam/2,TARGET_OBJ);
            }
        }

        extract_obj( obj );
	return;
    }
}
