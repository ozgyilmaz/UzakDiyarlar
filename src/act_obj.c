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
#include <time.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#endif
#include <stdio.h>
#include <wchar.h>
#include <wctype.h>
#include <string.h>
#include <stdlib.h>
#include "merc.h"

/* command procedures needed */
DECLARE_DO_FUN(do_split		);
DECLARE_DO_FUN(do_yell		);
DECLARE_DO_FUN(do_say		);
DECLARE_DO_FUN(do_mount		);



/*
 * Local functions.
 */
#define CD CHAR_DATA
#define OD OBJ_DATA

DECLARE_SPELL_FUN(	spell_enchant_weapon	);

bool	remove_obj	args( (CHAR_DATA *ch, OBJ_DATA *obj, bool fReplace ) );
bool	remove_obj_loc	args( (CHAR_DATA *ch, int iWear, bool fReplace ) );
void	wear_obj	args( (CHAR_DATA *ch, OBJ_DATA *obj, bool fReplace ) );
CD *	find_keeper	args( (CHAR_DATA *ch ) );
int	get_cost	args( (CHAR_DATA *keeper, OBJ_DATA *obj, bool fBuy ) );
void 	obj_to_keeper	args( (OBJ_DATA *obj, CHAR_DATA *ch ) );
OD *	get_obj_keeper	args( (CHAR_DATA *ch,CHAR_DATA *keeper,wchar_t *argument));
void	hold_a_light	args( (CHAR_DATA *ch,OBJ_DATA *obj, int iWear) );
void	hold_a_shield	args( (CHAR_DATA *ch,OBJ_DATA *obj, int iWear) );
void	hold_a_thing	args( (CHAR_DATA *ch,OBJ_DATA *obj, int iWear) );
void	wear_multi	args( (CHAR_DATA *ch,OBJ_DATA *obj,int iWear,bool fReplace) );
void	wear_a_wield	args( (CHAR_DATA *ch,OBJ_DATA *obj, bool fReplace) );

#undef OD
#undef CD

AFFECT_DATA  *	affect_find(AFFECT_DATA *paf, int sn);
int		find_exit( CHAR_DATA *ch, wchar_t *arg );


extern const wchar_t *   dir_name        [];

/* RT part of the corpse looting code */

bool can_loot(CHAR_DATA *ch, OBJ_DATA *obj)
{
    CHAR_DATA *owner, *wch;

    return TRUE;
    if (IS_IMMORTAL(ch))
	return TRUE;

    if (!obj->owner || obj->owner == NULL)
	return TRUE;

    owner = NULL;
    for ( wch = char_list; wch != NULL ; wch = wch->next )
        if (!wcscasecmp(wch->name,obj->owner))
            owner = wch;

    if (owner == NULL)
	return TRUE;

    if (!wcscasecmp(ch->name,owner->name))
	return TRUE;

    if (!IS_NPC(owner) && IS_SET(owner->act,PLR_CANLOOT))
	return TRUE;

    if (is_same_group(ch,owner))
	return TRUE;

    return FALSE;
}


void get_obj( CHAR_DATA *ch, OBJ_DATA *obj, OBJ_DATA *container )
{
    /* variables for AUTOSPLIT */
    CHAR_DATA *gch;
    int members;
    wchar_t buffer[100];
    if ( !CAN_WEAR(obj, ITEM_TAKE) )
    {
      send_to_char( L"Onu alamazsın.\n\r", ch );
	return;
    }
    if (obj->pIndexData->limit != -1)
    {
      if ( ( IS_OBJ_STAT(obj, ITEM_ANTI_EVIL)    && IS_EVIL(ch)    )
      ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_GOOD)    && IS_GOOD(ch)    )
      ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_NEUTRAL) && IS_NEUTRAL(ch) ) )
      {
        act( L"$p tarafından çarpıldın ve onu yere düşürdün.", ch, obj, NULL, TO_CHAR );
  			act( L"$n $p tarafından çarpıldı ve onu yere düşürdü.",  ch, obj, NULL, TO_ROOM );
	return;
      }
      if( !limit_kontrol(ch,obj) )
      {
  			return;
      }
    }
    if ( ch->carry_number + get_obj_number( obj ) > can_carry_n( ch ) )
    {
      act( L"$d: bu kadar çok şey taşıyamazsın.",
	    ch, NULL, obj->name, TO_CHAR );
	return;
    }

    if ( get_carry_weight(ch) + get_obj_weight( obj ) > can_carry_w( ch ) )
    {
      act( L"$d: bu kadar ağırlık taşıyamazsın.",
	    ch, NULL, obj->name, TO_CHAR );
	return;
    }
    if (obj->in_room != NULL)
    {
	for (gch = obj->in_room->people; gch != NULL; gch = gch->next_in_room)
	    if (gch->on == obj)
	    {
        act( L"$N $p kullanmaya başladı.",
		    ch,obj,gch,TO_CHAR);
		return;
	    }
    }

    if ( container != NULL )
    {
      if (container->pIndexData->vnum == OBJ_VNUM_INVADER_SKULL
       || container->pIndexData->vnum == OBJ_VNUM_RULER_STAND
       || container->pIndexData->vnum == OBJ_VNUM_BATTLE_THRONE
       || container->pIndexData->vnum == OBJ_VNUM_CHAOS_ALTAR
       || container->pIndexData->vnum == OBJ_VNUM_SHALAFI_ALTAR
        || container->pIndexData->vnum == OBJ_VNUM_KNIGHT_ALTAR
        || container->pIndexData->vnum == OBJ_VNUM_LIONS_ALTAR
	|| container->pIndexData->vnum == OBJ_VNUM_HUNTER_ALTAR)
        {
	  DESCRIPTOR_DATA *d;
    act( L"$P içinden $p aldın.", ch, obj, container, TO_CHAR );
    if (!IS_AFFECTED(ch,AFF_SNEAK))
      act( L"$n $P içinden $p aldı.", ch, obj, container, TO_ROOM );
    obj_from_obj( obj );
    act( L"$p kararıyor ve yokoluyor!", ch,container,NULL,TO_ROOM);
    act( L"$p kararıyor ve yokoluyor!", ch,container,NULL,TO_CHAR);
          extract_obj(container);
          obj_to_char(obj, ch);

          for ( d = descriptor_list; d; d = d->next )
          {
           if ( d->connected == CON_PLAYING && d->character != NULL &&
             cabal_table[d->character->cabal].obj_ptr == obj)
             act_color( L"$CKabal gücünde titreme hissediyorsun!$c",
                   d->character,NULL,NULL,TO_CHAR,POS_DEAD,CLR_GREEN);
          }
          if (IS_SET(obj->progtypes,OPROG_GET))
            (obj->pIndexData->oprogs->get_prog) (obj,ch);
	  return;
	}

    	if (container->pIndexData->vnum == OBJ_VNUM_PIT
	&&  !CAN_WEAR(container, ITEM_TAKE)
	&&  !IS_OBJ_STAT(obj,ITEM_HAD_TIMER))
	    obj->timer = 0;
      act( L"$P içinden $p alıyorsun.", ch, obj, container, TO_CHAR );
            if (!IS_AFFECTED(ch,AFF_SNEAK))
    	  act( L"$n $P içinden $p alıyor.", ch, obj, container, TO_ROOM );
	REMOVE_BIT(obj->extra_flags,ITEM_HAD_TIMER);
	obj_from_obj( obj );
    }
    else
    {
      act( L"$p alıyorsun.", ch, obj, container, TO_CHAR );
            if (!IS_AFFECTED(ch,AFF_SNEAK))
    	  act( L"$n $p alıyor.", ch, obj, container, TO_ROOM );
	obj_from_room( obj );
    }

    if ( obj->item_type == ITEM_MONEY)
    {
	ch->silver += ( obj->value[0] + ( obj->value[1] / 2 ) ) ;
        if (IS_SET(ch->act,PLR_AUTOSPLIT))
        { /* AUTOSPLIT code */
    	  members = 0;
    	  for (gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    	  {
            if (!IS_AFFECTED(gch,AFF_CHARM) && is_same_group( gch, ch ) )
              members++;
    	  }

	  if ( members > 1 && (obj->value[0] > 1 || obj->value[1]))
	  {
	    swprintf(buffer, 100-1, L"%d", obj->value[0] + ( obj->value[1] / 2 ) );
	    do_split(ch,buffer);
	  }
        }

	extract_obj( obj );
    }
    else
    {
	obj_to_char( obj, ch );
        if (IS_SET(obj->progtypes,OPROG_GET))
        {
          (obj->pIndexData->oprogs->get_prog) (obj,ch);
        }
    }

    return;
}



void do_get( CHAR_DATA *ch, wchar_t *argument )
{

  wchar_t arg1[MAX_INPUT_LENGTH];
  wchar_t arg2[MAX_INPUT_LENGTH];
  OBJ_DATA *obj;
  OBJ_DATA *obj_next;
  OBJ_DATA *container;
  bool found;

  argument = one_argument( argument, arg1 );
  argument = one_argument( argument, arg2 );

  if (!wcscasecmp(arg2,L"içinden"))
  {
    argument = one_argument(argument,arg2);
  }

  /* Get type. */
  if ( arg1[0] == '\0' )
  {
    send_to_char( L"Neyi alacaksın?\n\r", ch );
    return;
  }

  if ( is_number( arg1 ) )
  {
    int amount, weight, silver = 0;

    amount = wcstol( arg1, 0, 10 );

    if ( amount <= 0 || wcscasecmp( arg2, L"akçe" ) )
    {
      send_to_char( L"Kullanım: al <miktar> akçe\n\r", ch );
      return;
    }

    weight = amount / 10;

    if ( get_carry_weight(ch) + weight > can_carry_w( ch ) )
    {
      act( L"Bu kadar ağırlığı taşıyamazsın.",ch, NULL, NULL, TO_CHAR );
      return;
    }


    for ( obj = ch->in_room->contents; obj != NULL; obj = obj_next )
    {
      obj_next = obj->next_content;

      switch ( obj->pIndexData->vnum )
      {
        case OBJ_VNUM_SILVER_ONE:
          silver += 1;
          break;

        case OBJ_VNUM_SILVER_SOME:
          silver += obj->value[0];
          break;
      }
    }

    if ( !wcscasecmp( arg2, L"akçe") && (amount > silver)  )
    {
      send_to_char( L"O kadar akçe yok.\n\r", ch);
      return;
    }

    silver = amount;

    for ( obj = ch->in_room->contents; obj != NULL; obj = obj_next )
    {
      obj_next = obj->next_content;

      switch ( obj->pIndexData->vnum )
      {
        case OBJ_VNUM_SILVER_ONE:
        if (silver)
        {
          silver -= 1;
          extract_obj(obj);
        }
        break;

        case OBJ_VNUM_SILVER_SOME:
        if (silver)
        {
          if (silver >= obj->value[0])
          {
            silver -= obj->value[0];
            extract_obj(obj);
          }
          else
          {
            obj->value[0] -= silver;
            silver = 0;
          }
        }
        break;
      }
      if (!silver)
      {
        break;
      }
    }

    silver = amount;

    if ( silver )
    {
      ch->silver += amount;
    }

    act( L"Yerden bir miktar para aldın.", ch, NULL, NULL, TO_CHAR );
    if (!IS_AFFECTED(ch,AFF_SNEAK))
    {
      act( L"$n yerden bir miktar para aldı.", ch, NULL, NULL, TO_ROOM );
    }

    if (IS_SET(ch->act,PLR_AUTOSPLIT))
    {
      int members = 0;
      CHAR_DATA *gch;
      wchar_t buffer[MAX_INPUT_LENGTH];

      for (gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
      {
        if (!IS_AFFECTED(gch,AFF_CHARM) && is_same_group( gch, ch ) )
        members++;
      }

      if ( members > 1 && (amount > 1) )
      {
        swprintf(buffer, MAX_INPUT_LENGTH-1, L"%d",silver);
        do_split(ch,buffer);
      }
    }

    return;
  }

    if ( arg2[0] == '\0' )
    {
	if ( wcscasecmp( arg1, L"tümü" ) && str_prefix( L"tümü.", arg1 ) )
	{
	    /* 'get obj' */
	    obj = get_obj_list( ch, arg1, ch->in_room->contents );
	    if ( obj == NULL )
	    {
        act( L"Burada $T yok.", ch, NULL, arg1, TO_CHAR );
		return;
	    }
	    get_obj( ch, obj, NULL );
	}
	else
	{
	    /* 'get all' or 'get all.obj' */
	    found = FALSE;
	    for ( obj = ch->in_room->contents; obj != NULL; obj = obj_next )
	    {
		obj_next = obj->next_content;
		if ( ( arg1[3] == '\0' || is_name( &arg1[4], obj->name ) )
		&&   can_see_obj( ch, obj ) )
		{
		    found = TRUE;
		    get_obj( ch, obj, NULL );
		}
	    }

	    if ( !found )
	    {
		if ( arg1[3] == '\0' )
    send_to_char( L"Bir şey görmüyorum.\n\r", ch );
		else
    act( L"Burada $T yok.", ch, NULL, &arg1[4], TO_CHAR );
	    }
	}
    }
    else
    {
	/* 'get ... container' */
	if ( !wcscasecmp( arg2, L"tümü" ) || !str_prefix( L"tümü.", arg2 ) )
	{
    send_to_char( L"Bunu yapamazsın.\n\r", ch );
	    return;
	}

	if ( ( container = get_obj_here( ch, arg2 ) ) == NULL )
	{
    act( L"Burada $T yok.", ch, NULL, arg2, TO_CHAR );
	    return;
	}

	switch ( container->item_type )
	{
	default:
  send_to_char( L"Bu bir taşıyıcı değil.\n\r", ch );
	    return;

	case ITEM_CONTAINER:
	case ITEM_CORPSE_NPC:
	    break;

	case ITEM_CORPSE_PC:
	    {

		if (!can_loot(ch,container))
		{
      send_to_char( L"Bunu yapamazsın.\n\r", ch );
		    return;
		}
	    }
	}

	if ( IS_SET(container->value[1], CONT_CLOSED) )
	{
    act( L"$d kapalı.", ch, NULL, container->name, TO_CHAR );
	    return;
	}

	if ( wcscasecmp( arg1, L"tümü" ) && str_prefix( L"tümü.", arg1 ) )
	{
	    /* 'get obj container' */
	    obj = get_obj_list( ch, arg1, container->contains );
	    if ( obj == NULL )
	    {
        act( L"$T içinde böyle birşey yok.",
		    ch, NULL, arg2, TO_CHAR );
		return;
	    }
	    get_obj( ch, obj, container );
	}
	else
	{
	    /* 'get all container' or 'get all.obj container' */
	    found = FALSE;
	    for ( obj = container->contains; obj != NULL; obj = obj_next )
	    {
		obj_next = obj->next_content;
		if ( ( arg1[3] == '\0' || is_name( &arg1[4], obj->name ) )
		&&   can_see_obj( ch, obj ) )
		{
		    found = TRUE;
		    if (container->pIndexData->vnum == OBJ_VNUM_PIT
		    &&  !IS_IMMORTAL(ch))
		    {
          send_to_char( L"Açgözlülük yapma!\n\r",ch);
			return;
		    }
		    get_obj( ch, obj, container );
		}
	    }

	    if ( !found )
	    {
		if ( arg1[3] == '\0' )
    act( L"$T içinde hiçbir şey yok.",
			ch, NULL, arg2, TO_CHAR );
		else
    act( L"$T içinde böyle birşey yok.",
			ch, NULL, arg2, TO_CHAR );
	    }
	}
    }

    return;
}



void do_put( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg1[MAX_INPUT_LENGTH];
    wchar_t arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *container;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    OBJ_DATA *objc;
    int pcount;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (!wcscasecmp(arg2,L"içine") || !wcscasecmp(arg2,L"üstüne"))
	argument = one_argument(argument,arg2);

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
      send_to_char( L"Neyi neyin içine koyacaksın?\n\r", ch );
	return;
    }

    if ( !wcscasecmp( arg2, L"tümü" ) || !str_prefix( L"tümü.", arg2 ) )
    {
      send_to_char( L"Bunu yapamazsın.\n\r", ch );
	return;
    }

    if ( ( container = get_obj_here( ch, arg2 ) ) == NULL )
    {
      act( L"Burada $T yok.", ch, NULL, arg2, TO_CHAR );
	return;
    }

    if ( container->item_type != ITEM_CONTAINER )
    {
      send_to_char( L"Bu bir taşıyıcı değil.\n\r", ch );
	return;
    }

    if ( IS_SET(container->value[1], CONT_CLOSED) )
    {
      act( L"$d kapalı.", ch, NULL, container->name, TO_CHAR );
	return;
    }

    if ( wcscasecmp( arg1, L"tümü" ) && str_prefix( L"tümü.", arg1 ) )
    {
	/* 'put obj container' */
	if ( ( obj = get_obj_carry( ch, arg1 ) ) == NULL )
	{
    send_to_char( L"Sende öyle birşey yok.\n\r", ch );
	    return;
	}

	if ( obj == container )
	{
    send_to_char( L"Onu kendi içine koyamazsın.\n\r", ch );
	    return;
	}

	if ( !can_drop_obj( ch, obj ) )
	{
    send_to_char( L"Ondan kurtulamıyorsun.\n\r", ch );
	    return;
	}

    	if (WEIGHT_MULT(obj) != 100)
    	{
        send_to_char( L"Bunun kötü bir fikir olduğunu hissediyorsun.\n\r",ch);
            return;
        }

	if ( obj->pIndexData->limit != -1
		&& !IS_SET(container->value[1], CONT_ST_LIMITED) )
        {
          act( L"Bu değersiz taşıyıcı $p alamaz.", ch,obj,NULL,TO_CHAR);
	  return;
	}
/*
	if ( IS_SET(container->value[1],CONT_FOR_ARROW)
		&& (obj->item_type != ITEM_WEAPON
		|| obj->value[0]  != WEAPON_ARROW ) )
	{
	 act( L"You can only put arrows in $p.",ch,container,NULL,TO_CHAR);
	 return;
	}
*/
	if (get_obj_weight( obj ) + get_true_weight( container )
	     > (container->value[0] * 10)
	||  get_obj_weight(obj) > (container->value[3] * 10))
	{
    send_to_char( L"Sığmaz.\n\r", ch );
	    return;
	}

	if ( obj->item_type == ITEM_POTION &&
	     IS_SET(container->wear_flags, ITEM_TAKE) )
	{
	   pcount = 0;
	   for(objc=container->contains; objc!=NULL ; objc=objc->next_content)
	     if (objc->item_type == ITEM_POTION) pcount++;
	   if ( pcount>15 )
	   {
       act( L"$p içine bu kadar iksir koymak güvenli değil.",ch,container,NULL, TO_CHAR);
	      return;
	   }
	}

        pcount = 0;
        for(objc=container->contains; objc!=NULL ; objc=objc->next_content)
	     pcount++;
	if ( pcount>container->value[0] )
	   {
       act( L"$p içine bu kadar şey koymak güvenli değil.",ch,container,NULL, TO_CHAR);
	      return;
	   }

	if (container->pIndexData->vnum == OBJ_VNUM_PIT
	&&  !CAN_WEAR(container,ITEM_TAKE))
	{
	    if (obj->timer)
		SET_BIT(obj->extra_flags,ITEM_HAD_TIMER);
	    else
	        obj->timer = number_range(100,200);
	        }

	obj_from_char( obj );
	obj_to_obj( obj, container );

	if (IS_SET(container->value[1],CONT_PUT_ON))
	{
    act( L"$n $P üzerine $p koyuyor.",ch,obj,container, TO_ROOM);
    act( L"$P üzerine $p koyuyorsun.",ch,obj,container, TO_CHAR);
	}
	else
	{
    act( L"$n $P içine $p koyuyor.", ch, obj, container, TO_ROOM );
    act( L"$P içine $p koyuyorsun.", ch, obj, container, TO_CHAR );
	}
    }
    else
    {
        pcount = 0;
        for(objc=container->contains; objc!=NULL ; objc=objc->next_content)
	     pcount++;

	/* 'put all container' or 'put all.obj container' */
	for ( obj = ch->carrying; obj != NULL; obj = obj_next )
	{
	    obj_next = obj->next_content;

	    if ( ( arg1[3] == '\0' || is_name( &arg1[4], obj->name ) )
	    &&   can_see_obj( ch, obj )
	    &&   WEIGHT_MULT(obj) == 100
	    &&   obj->wear_loc == WEAR_NONE
	    &&   obj != container
	    &&   can_drop_obj( ch, obj )
	    &&   get_obj_weight( obj ) + get_true_weight( container )
		 <= (container->value[0] * 10)
	    &&   get_obj_weight(obj) < (container->value[3] * 10))
	    {
	    	if (container->pIndexData->vnum == OBJ_VNUM_PIT
	    	&&  !CAN_WEAR(obj, ITEM_TAKE) )
	    	{
	    	    if (obj->timer)
			SET_BIT(obj->extra_flags,ITEM_HAD_TIMER);
	    	    else
	    	    	obj->timer = number_range(100,200);
	    	    	}

		if ( obj->pIndexData->limit != -1 )
		{
      act( L"Bu değersiz taşıyıcı $p'yi alamaz.", ch,obj,NULL,TO_CHAR);
		  continue;
		}

		if ( obj->item_type == ITEM_POTION &&
	     	     IS_SET(container->wear_flags, ITEM_TAKE) )
		{
	   	  pcount = 0;
	   	  for(objc=container->contains; objc!=NULL ; objc=objc->next_content)
	     	  if (objc->item_type == ITEM_POTION) pcount++;
	   	  if ( pcount>15 )
	   	  {
          act( L"$p içine bu kadar iksir koymak güvenli değil.",ch,container,NULL, TO_CHAR);
	      	    continue;
	   	  }
		}

		pcount++;
		if ( pcount>container->value[0] )
		   {
         act( L"$p içine bu kadar şey koymak güvenli değil.",ch,container,NULL, TO_CHAR);
		      return;
		   }
		obj_from_char( obj );
		obj_to_obj( obj, container );

        	if (IS_SET(container->value[1],CONT_PUT_ON))
        	{
            act( L"$n $P üzerine $p koyuyor.",ch,obj,container, TO_ROOM);
            act( L"$P üzerine $p koyuyorsun.",ch,obj,container, TO_CHAR);
        	}
		else
		{
      act( L"$n $P içine $p koyuyor.", ch, obj, container, TO_ROOM );
      act( L"$P içine $p koyuyorsun.", ch, obj, container, TO_CHAR );
		}
	    }
	}
    }

    return;
}



void do_drop( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    bool found;


    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
      send_to_char( L"Neyi bırakacaksın?\n\r", ch );
	return;
    }

    if ( is_number( arg ) )
    {
	/* 'drop NNNN coins' */
	int amount, silver = 0;

	amount   = wcstol(arg, 0, 10 );
	argument = one_argument( argument, arg );
  if ( amount <= 0 || ( wcscasecmp( arg, L"akçe"  ) ) )
	{
    send_to_char( L"Bunu yapamazsın.\n\r", ch );
	    return;
	}

  if ( !wcscasecmp(arg,L"akçe"))
	{
	    if (ch->silver < amount)
	    {
        send_to_char( L"Bu kadar akçen yok.\n\r",ch);
		return;
	    }

	    ch->silver -= amount;
	    silver = amount;
	}

	for ( obj = ch->in_room->contents; obj != NULL; obj = obj_next )
	{
	    obj_next = obj->next_content;

	    switch ( obj->pIndexData->vnum )
	    {
	    case OBJ_VNUM_SILVER_ONE:
		silver += 1;
		extract_obj(obj);
		break;

	    case OBJ_VNUM_SILVER_SOME:
		silver += obj->value[0];
		extract_obj(obj);
		break;
	    }
	}

	obj = create_money( silver );
	obj_to_room( obj, ch->in_room );
	if ( !IS_AFFECTED(ch, AFF_SNEAK) )
  act( L"$n bir miktar sikke bırakıyor.", ch, NULL, NULL, TO_ROOM );
  send_to_char( L"Tamam.\n\r", ch );
	if ( IS_WATER( ch->in_room ) )   {
	  extract_obj( obj );
	  if ( !IS_AFFECTED(ch, AFF_SNEAK) )
    act( L"Sikkeler suyun içinde kayboluyor.", ch, NULL, NULL, TO_ROOM );
  act( L"Sikkeler suyun içinde kayboluyor.", ch, NULL, NULL, TO_CHAR );
	}
	return;
    }

    if ( wcscasecmp( arg, L"tümü" ) && str_prefix( L"tümü.", arg ) )
    {
	/* 'drop obj' */
	if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
	{
    send_to_char( L"Sende öyle birşey yok.\n\r", ch );
	    return;
	}

	if ( !can_drop_obj( ch, obj ) )
	{
    send_to_char( L"Ondan kurtulamıyorsun.\n\r", ch );
	    return;
	}

	obj_from_char( obj );
	obj_to_room( obj, ch->in_room );
	if ( !IS_AFFECTED(ch, AFF_SNEAK) )
  act( L"$n $p bırakıyor.", ch, obj, NULL, TO_ROOM );
act( L"$p bırakıyorsun.", ch, obj, NULL, TO_CHAR );
	if ( obj->pIndexData->vnum == OBJ_VNUM_POTION_VIAL &&
              number_percent( ) < 40 )
	  if ( !IS_SET(ch->in_room->sector_type, SECT_FOREST) &&
	       !IS_SET(ch->in_room->sector_type, SECT_DESERT) &&
	       !IS_SET(ch->in_room->sector_type, SECT_AIR) &&
	       !IS_WATER(ch->in_room) )
	  {
      act( L"$p küçük parçalara bölünüyor.", ch, obj, NULL,TO_ROOM );
	    act( L"$p küçük parçalara bölünüyor.", ch, obj, NULL,TO_CHAR );
	    extract_obj( obj );
	    return;
	  }
        if (IS_SET(obj->progtypes,OPROG_DROP))
          (obj->pIndexData->oprogs->drop_prog) (obj,ch);

	if ( !may_float(obj) && cant_float(obj) && IS_WATER( ch->in_room ))
	{
	  if ( !IS_AFFECTED(ch, AFF_SNEAK) )
    act( L"$p suyun içinde kayboluyor.", ch, obj, NULL, TO_ROOM);
  act( L"$p suyun içinde kayboluyor.", ch, obj, NULL, TO_CHAR);
	  extract_obj( obj );
	}

	else if (IS_OBJ_STAT(obj,ITEM_MELT_DROP) )
	{
	  if ( !IS_AFFECTED(ch, AFF_SNEAK) )
    act( L"$p dumana dönüşüyor.",ch,obj,NULL,TO_ROOM);
  act( L"$p dumana dönüşüyor.",ch,obj,NULL,TO_CHAR);
	  extract_obj(obj);
	}
    }
    else
    {
	/* 'drop all' or 'drop all.obj' */
	found = FALSE;
	for ( obj = ch->carrying; obj != NULL; obj = obj_next )
	{
	    obj_next = obj->next_content;

	    if ( ( arg[3] == '\0' || is_name( &arg[4], obj->name ) )
	    &&   can_see_obj( ch, obj )
	    &&   obj->wear_loc == WEAR_NONE
	    &&   can_drop_obj( ch, obj ) )
	    {
		found = TRUE;
		obj_from_char( obj );
		obj_to_room( obj, ch->in_room );
	  	if ( !IS_AFFECTED(ch, AFF_SNEAK) )
      act( L"$n $p bırakıyor.", ch, obj, NULL, TO_ROOM );
		act( L"$p bırakıyorsun.", ch, obj, NULL, TO_CHAR );
	        if ( obj->pIndexData->vnum == OBJ_VNUM_POTION_VIAL &&
		     number_percent( )  < 70 )
		  if ( !IS_SET(ch->in_room->sector_type, SECT_FOREST) &&
		       !IS_SET(ch->in_room->sector_type, SECT_DESERT) &&
			!IS_SET(ch->in_room->sector_type, SECT_AIR) &&
		       !IS_WATER(ch->in_room) )

	  	{
		  if ( !IS_AFFECTED(ch, AFF_SNEAK) )
      act( L"$p küçük parçalara bölünüyor.", ch, obj, NULL,TO_ROOM );
    act( L"$p küçük parçalara bölünüyor.", ch, obj, NULL,TO_CHAR );
	          extract_obj( obj );
	          continue;
		}

                if (IS_SET(obj->progtypes,OPROG_DROP))
                  (obj->pIndexData->oprogs->drop_prog) (obj,ch);

		if ( !may_float(obj) && cant_float(obj) && IS_WATER(ch->in_room) )
		{
		  if ( !IS_AFFECTED(ch, AFF_SNEAK) )
      act( L"$p suyun içinde kayboluyor.", ch, obj, NULL, TO_ROOM);
    act( L"$p suyun içinde kayboluyor.", ch, obj, NULL, TO_CHAR);
		  extract_obj( obj );
		}
        	else if (IS_OBJ_STAT(obj,ITEM_MELT_DROP))
        	{
		  if ( !IS_AFFECTED(ch, AFF_SNEAK) )
      act( L"$p dumana dönüşüyor.",ch,obj,NULL,TO_ROOM);
    act( L"$p dumana dönüşüyor.",ch,obj,NULL,TO_CHAR);
            	  extract_obj(obj);
        	}
	    }
	}

	if ( !found )
	{
	    if ( arg[3] == '\0' )
      act( L"Hiçbir şey taşımıyorsun.",
		    ch, NULL, arg, TO_CHAR );
	    else
      act( L"$T taşımıyorsun.",
		    ch, NULL, &arg[4], TO_CHAR );
	}
    }

    return;
}



void do_drag( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg1[MAX_INPUT_LENGTH];
    wchar_t arg2[MAX_INPUT_LENGTH];
    wchar_t buf[MAX_STRING_LENGTH];
    CHAR_DATA *gch;
    OBJ_DATA *obj;
    EXIT_DATA *pexit;
    ROOM_INDEX_DATA *was_in_room;
    int direction;


    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    /* Get type. */
    if ( arg1[0] == '\0' || arg2[0] == '\0')
    {
      send_to_char( L"Neyi hangi yönde sürükleyeceksin?\n\r", ch );
	return;
    }

    if ( !wcscasecmp( arg1, L"tümü" ) || !str_prefix( L"tümü.", arg1 ) )
    {
      send_to_char( L"Bunu yapamazsın.\n\r", ch );
        return;
    }

   obj = get_obj_list( ch, arg1, ch->in_room->contents );
   if ( obj == NULL )
   {
     act( L"Burada $T yok.", ch, NULL, arg1, TO_CHAR );
        return;
   }

   if ( !CAN_WEAR(obj, ITEM_TAKE) )
   {
     send_to_char( L"Onu alamazsın.\n\r", ch );
        return;
   }

   if (obj->pIndexData->limit != -1)
   {
      if ( ( IS_OBJ_STAT(obj, ITEM_ANTI_EVIL)    && IS_EVIL(ch)    )
      ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_GOOD)    && IS_GOOD(ch)    )
      ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_NEUTRAL) && IS_NEUTRAL(ch) ) )
      {
        act( L"$p tarafından çarpıldın ve onu düşürdün.", ch, obj, NULL, TO_CHAR );
        act( L"$n $p tarafından çarpıldı ve onu düşürdü.",  ch, obj, NULL, TO_ROOM );
        return;
      }
      if( !limit_kontrol(ch,obj) )
      {
  			return;
      }
   }

   if (obj->in_room != NULL)
   {
       for (gch = obj->in_room->people; gch != NULL; gch = gch->next_in_room)
           if (gch->on == obj)
           {
             act( L"$N $p kullanmaya başladı.", ch,obj,gch,TO_CHAR);
               return;
           }
   }

   if ( (get_carry_weight(ch)+get_obj_weight(obj)) > (2*can_carry_w(ch)) )
   {
     act( L"$d: bu kadar ağırlığı sürükleyemezsin.",
           ch, NULL, obj->name, TO_CHAR );
       return;
   }

   if ( get_eq_char(ch, WEAR_LEFT) != NULL
	|| get_eq_char(ch, WEAR_RIGHT) != NULL
	|| get_eq_char(ch, WEAR_BOTH) != NULL)
   {
     send_to_char( L"İki elin de boş olmalı.\n\r",ch);
      return;
   }

   if ( (direction = find_exit( ch, arg2 )) < 0)
	return;

   if ( ( pexit   = ch->in_room->exit[direction] ) == NULL
   ||   pexit->u1.to_room   == NULL
   ||   !can_see_room(ch,pexit->u1.to_room))
   {
     send_to_char( L"O yöne gidemezsin.\n\r", ch );
       return;
   }

   swprintf( buf, MAX_STRING_LENGTH-1, L"$p'yi %s yönünde sürüklemek için kavradın.", dir_name[direction] );
   act( buf, ch, obj, NULL, TO_CHAR );
   if (!IS_AFFECTED(ch,AFF_SNEAK))
   {
     swprintf( buf, MAX_STRING_LENGTH-1, L"$n $p'yi %s yönünde sürüklemek için kavradı.", dir_name[direction] );
         act( buf, ch, obj, NULL, TO_ROOM );
   }

   obj_from_room( obj );
   obj_to_char( obj, ch );

   if (IS_SET(obj->progtypes,OPROG_GET))
          (obj->pIndexData->oprogs->get_prog) (obj,ch);

   if (obj->carried_by != ch) return;

   was_in_room = ch->in_room;
   move_char(ch, direction, FALSE);

   if ( was_in_room == ch->in_room )
   send_to_char( L"O yöne sürükleyemezsin.\n\r", ch );
   else
   {
	if ( !IS_AFFECTED(ch, AFF_SNEAK) )
  act( L"$n $p bırakıyor.", ch, obj, NULL, TO_ROOM );
act( L"$p bırakıyorsun.", ch, obj, NULL, TO_CHAR );
        WAIT_STATE( ch,2 * PULSE_VIOLENCE );
   }

   obj_from_char( obj );
   obj_to_room( obj, ch->in_room );

   return;
}


void do_give( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg1 [MAX_INPUT_LENGTH];
    wchar_t arg2 [MAX_INPUT_LENGTH];
    wchar_t buf[MAX_STRING_LENGTH];
    wchar_t buf2[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA  *obj;

    argument = one_argument( argument, arg1 );
    swprintf(buf2,MAX_STRING_LENGTH-1, L"%s",argument);
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
      send_to_char( L"Neyi kime vereceksin?\n\r", ch );
	return;
    }

    if ( is_number( arg1 ) )
    {
	/* 'give NNNN coins victim' */
	int amount;
	int weight;

	amount   = wcstol(arg1, 0, 10 );
	if ( amount <= 0 || ( wcscasecmp( arg2, L"akçe")) )
	{
    send_to_char( L"Bunu yapamazsın.\n\r", ch );
	    return;
	}

	argument = one_argument( argument, arg2 );
	if ( arg2[0] == '\0' )
	{
    send_to_char( L"Neyi kime vereceksin?\n\r", ch );
	    return;
	}

	if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
	{
    send_to_char( L"Burada değil.\n\r", ch);
	    return;
	}

	if ( ch->silver < amount )
	{
    send_to_char( L"Bu kadarına sahip değilsin.\n\r", ch );
	    return;
	}

  weight = amount / 10;

	if ( !IS_NPC(victim)
		&& get_carry_weight(victim) + weight > can_carry_w(victim) )
	{
    act( L"$N bu kadar ağırlık taşıyamaz.", ch,NULL,victim, TO_CHAR);
	     return;
	}

  ch->silver		-= amount;
  victim->silver 	+= amount;

  swprintf( buf, MAX_STRING_LENGTH-1, L"$n sana %d akçe veriyor.",amount);
	act( buf, ch, NULL, victim, TO_VICT    );
	act( L"$n $E bir miktar akçe veriyor.",  ch, NULL, victim, TO_NOTVICT );
	swprintf( buf, MAX_STRING_LENGTH-1, L"$E %d akçe veriyorsun.",amount);
	act( buf, ch, NULL, victim, TO_CHAR    );
        if (IS_SET(victim->progtypes,MPROG_BRIBE))
          (victim->pIndexData->mprogs->bribe_prog) (victim,ch,amount);

	return;
    }

    if ( ( obj = get_obj_carry( ch, arg1 ) ) == NULL )
    {
      send_to_char( L"Sende öyle birşey yok.\n\r", ch );
	return;
    }

    if ( obj->wear_loc != WEAR_NONE )
    {
      send_to_char( L"Önce onu çıkarmalısın.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
    {
      send_to_char( L"Burada değil.\n\r", ch);
	return;
    }


    if (  (IS_NPC(victim) && victim->pIndexData->pShop != NULL ) &&
	   !IS_SET(victim->progtypes, MPROG_GIVE) )
    {
      act( L"$N anlatıyor 'Üzgünüm, onu satmalısın.'",
	    ch,NULL,victim,TO_CHAR);
	ch->reply = victim;
	return;
    }

    if ( !can_drop_obj( ch, obj ) )
    {
      send_to_char( L"Ondan kurtulamıyorsun.\n\r", ch );
	return;
    }

    if ( victim->carry_number + get_obj_number( obj ) > can_carry_n( victim ) )
    {
      act( L"$S elleri dolu.", ch, NULL, victim, TO_CHAR );
	return;
    }

    if (get_carry_weight(victim) + get_obj_weight(obj) > can_carry_w( victim ) )
    {
      act( L"$N bu ağırlığı taşıyamaz.", ch, NULL, victim, TO_CHAR );
	return;
    }

    if ( !can_see_obj( victim, obj ) )
    {
      act( L"$N onu göremiyor.", ch, NULL, victim, TO_CHAR );
	return;
    }

    if (obj->pIndexData->limit != -1)
    {
      if ( ( IS_OBJ_STAT(obj, ITEM_ANTI_EVIL)    && IS_EVIL(victim)    )
      ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_GOOD)    && IS_GOOD(victim)    )
      ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_NEUTRAL) && IS_NEUTRAL(victim) ) )
      {
        send_to_char( L"Kurbanının yönelimi eşyanınkiyle uyuşmuyor.", ch );
	return;
      }
      if( !limit_kontrol(victim,obj) )
      {
  			return;
      }
    }

    obj_from_char( obj );
    obj_to_char( obj, victim );
    act( L"$n $E $p veriyor.", ch, obj, victim, TO_NOTVICT );
    act( L"$n sana $p veriyor.",   ch, obj, victim, TO_VICT    );
    act( L"$E $p veriyorsun.", ch, obj, victim, TO_CHAR    );

    if (IS_SET(obj->progtypes,OPROG_GIVE))
      (obj->pIndexData->oprogs->give_prog) (obj,ch,victim);

    if (IS_SET(obj->progtypes,OPROG_GET))
      (obj->pIndexData->oprogs->get_prog) (obj,victim);

    if (IS_SET(victim->progtypes,MPROG_GIVE))
      (victim->pIndexData->mprogs->give_prog) (victim,ch,obj);

    return;
}


void do_bury( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH];
    wchar_t buf[MAX_INPUT_LENGTH];
    wchar_t *bufp;
    OBJ_DATA *obj, *shovel, *stone;
    int move;

    one_argument( argument, arg );

    if ( !IS_IMMORTAL(ch) && ch->iclass != CLASS_CLERIC )
    {
      send_to_char( L"Bir ermiş değilsin!\n\r", ch );
        return;
    }

    if ( arg[0] == '\0' )
    {
      send_to_char( L"Kimin cesedini gömeceksin?\n\r", ch );
        return;
    }

    if ( (shovel = get_weapon_char(ch, WEAPON_MACE)) == NULL
         || !is_name((wchar_t*)"kürek",shovel->name) )
    {
      send_to_char( L"Kazmak için küreğin yok!\n\r", ch );
        return;
    }

   obj = get_obj_list( ch, arg, ch->in_room->contents );
   if ( obj == NULL )
   {
     act( L"Burada $T yok.", ch, NULL, arg, TO_CHAR );
        return;
   }

   if (obj->item_type != ITEM_CORPSE_PC && obj->item_type != ITEM_CORPSE_NPC)
    {
      send_to_char( L"Neden onu gömmek istiyorsun?\n\r",ch);
      return;
    }

    switch( ch->in_room->sector_type )
    {
        case SECT_CITY:
        case SECT_INSIDE:
        send_to_char( L"Zemin kazılamayacak kadar sert.\n\r", ch );
            return;
        case SECT_WATER_SWIM:
        case SECT_WATER_NOSWIM:
        send_to_char( L"Buraya birşey gömemezsin.\n\r", ch );
            return;
        case SECT_AIR:
        send_to_char( L"Ne? Havaya mı?!\n\r", ch );
            return;
    }

    move = (int) ((obj->weight * 5) / get_curr_stat(ch, STAT_STR) );
    move = URANGE( 2, move, 1000 );
    if ( move > ch->move )
    {
      send_to_char( L"BU büyüklükte birşeyi gömmek için yeterli enerjin yok.\n\r", ch );
        return;
    }
    ch->move -= move;

    act( L"$p'yi ayinle gömüyorsun...", ch, obj, NULL, TO_CHAR );
    act( L"$n $p'yi ayinle gömüyor...", ch, obj, NULL, TO_ROOM );

    SET_BIT( obj->extra_flags, ITEM_BURIED );
    WAIT_STATE( ch, 4 * PULSE_VIOLENCE );

    obj->timer = -1;

    buf[0] = '\0';
    bufp = obj->short_descr;
    while ( bufp[0] != '\0' )
    {
     bufp = one_argument(bufp, arg);
     if (!( !wcscasecmp(arg,L"hortlak") || !wcscasecmp(arg,L"gövde") || !wcscasecmp(arg,L"beden") || !wcscasecmp(arg,L"ceset") ))
     {
	if (buf[0] == '\0')   wcscat(buf,arg);
        else  {
                 wcscat(buf,L" ");
                 wcscat(buf,arg);
        }
     }
    }
    swprintf(arg, MAX_INPUT_LENGTH-1, L"%s", buf);

    stone = create_object( get_obj_index(OBJ_VNUM_GRAVE_STONE), ch->level);

    swprintf( buf, MAX_STRING_LENGTH-1, stone->description, arg);
    free_string( stone->description );
    stone->description = str_dup( buf );

    swprintf( buf, MAX_STRING_LENGTH-1, stone->short_descr, arg);
    free_string( stone->short_descr );
    stone->short_descr = str_dup( buf );

    obj_to_room(stone, ch->in_room );

    /*
     * a little trick here... :)
     * although grave stone is not a container....
     * protects corpse from area affect attacks.
     * but what about earthquake
     */
    obj_from_room( obj );
    obj_to_obj( obj, stone );

    return;
}

void do_dig( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj, *shovel, *corpse;
    int move;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
      send_to_char( L"Hangi mezarı kazacaksın?\n\r", ch );
        return;
    }

    if ( (shovel = get_weapon_char(ch, WEAPON_MACE)) == NULL
         || !is_name((wchar_t*)"kürek",shovel->name) )
    {
      send_to_char( L"Kazmak için küreğin yok!\n\r", ch );
        return;
    }

   obj = get_obj_list( ch, arg, ch->in_room->contents );
   if ( obj == NULL )
   {
     act( L"Burada $T yok.", ch, NULL, arg, TO_CHAR );
        return;
   }

   if (obj->pIndexData->vnum != OBJ_VNUM_GRAVE_STONE)
    {
      send_to_char( L"O bir mezar değil.\n\r",ch);
      return;
    }

    move = (int) ((obj->weight * 5) / get_curr_stat(ch, STAT_STR) );
    move = URANGE( 2, move, 1000 );
    if ( move > ch->move )
    {
      send_to_char( L"Bu büyüklükte birşeyi kazmak için yeterli enerjin yok.\n\r", ch );
        return;
    }
    ch->move -= move;

    act( L"$p'yi kazmaya başlıyorsun...", ch, obj, NULL, TO_CHAR );
    act( L"$n $p'yi kazmaya başlıyor...", ch, obj, NULL, TO_ROOM );

    WAIT_STATE( ch, 4 * PULSE_VIOLENCE );

    if ( (corpse = obj->contains) == NULL)
    {
      act( L"Kazı sonucu ortaya bir şey çıkmıyor.\n\r",ch, NULL, NULL, TO_ALL );
        return;
    }

    REMOVE_BIT( corpse->extra_flags, ITEM_BURIED );
    obj_from_obj( corpse );
    obj_to_room( corpse, ch->in_room );
    extract_obj( obj );
    corpse->timer   = number_range( 25, 40 );
    act( L"Kazı $p'yi ortaya çıkarıyor.\n\r",ch, corpse, NULL, TO_ALL );

    return;
}


/* for poisoning weapons and food/drink */
void do_envenom(CHAR_DATA *ch, wchar_t *argument)
{
    OBJ_DATA *obj;
    AFFECT_DATA af;
    int percent,skill;

    /* find out what */
    if (argument == NULL)
    {
      send_to_char( L"Neyi zehirleyeceksin?\n\r",ch);
	return;
    }

    obj =  get_obj_list(ch,argument,ch->carrying);

    if (obj== NULL)
    {
      send_to_char( L"Sende ondan yok.\n\r",ch);
	return;
    }

    if ((skill = get_skill(ch,gsn_envenom)) < 1)
    {
      send_to_char( L"Deli misin? Kendini zehirledin!\n\r",ch);
	return;
    }

    if (obj->item_type == ITEM_FOOD || obj->item_type == ITEM_DRINK_CON)
    {
	if (IS_OBJ_STAT(obj,ITEM_BLESS) || IS_OBJ_STAT(obj,ITEM_BURN_PROOF))
	{
    act( L"$p'yi zehirleyemedin.",ch,obj,NULL,TO_CHAR);
	    return;
	}

	if (number_percent() < skill)  /* success! */
	{
    act( L"$n $p objesini ölümcül zehirle kaplıyor.",ch,obj,NULL,TO_ROOM);
    act( L"$p objesini ölümcül zehirle kaplıyorsun.",ch,obj,NULL,TO_CHAR);
	    if (!obj->value[3])
	    {
		obj->value[3] = 1;
		check_improve(ch,gsn_envenom,TRUE,4);
	    }
	    WAIT_STATE(ch,skill_table[gsn_envenom].beats);
	    return;
	}

  act( L"$p'yi zehirleyemedin.",ch,obj,NULL,TO_CHAR);
	if (!obj->value[3])
	    check_improve(ch,gsn_envenom,FALSE,4);
	WAIT_STATE(ch,skill_table[gsn_envenom].beats);
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
        ||  IS_OBJ_STAT(obj,ITEM_BLESS) || IS_OBJ_STAT(obj,ITEM_BURN_PROOF))
        {
          act( L"$p objesini zehirleyemezsin.",ch,obj,NULL,TO_CHAR);
            return;
        }

	if (obj->value[3] < 0
	||  attack_table[obj->value[3]].damage == DAM_BASH)
	{
    send_to_char( L"Yalnız kabzalı silahları zehirleyebilirsin.\n\r",ch);
	    return;
	}

        if (IS_WEAPON_STAT(obj,WEAPON_POISON))
        {
          act( L"$p zaten zehirlenmiş.",ch,obj,NULL,TO_CHAR);
            return;
        }

	percent = number_percent();
	if (percent < skill)
	{

            af.where     = TO_WEAPON;
            af.type      = gsn_poison;
            af.level     = ch->level * percent / 100;
            af.duration  = ch->level * percent / 100;
            af.location  = 0;
            af.modifier  = 0;
            af.bitvector = WEAPON_POISON;
            affect_to_obj(obj,&af);

	    if ( !IS_AFFECTED( ch, AFF_SNEAK ) )
      act( L"$n $p objesini ölümcül zehirle kaplıyor.",ch,obj,NULL,TO_ROOM);
act( L"$p objesini zehirle kaplıyorsun.",ch,obj,NULL,TO_CHAR);
	    check_improve(ch,gsn_envenom,TRUE,3);
	    WAIT_STATE(ch,skill_table[gsn_envenom].beats);
            return;
        }
	else
	{
    act( L"$p objesini zehirlemeyi başaramadın.",ch,obj,NULL,TO_CHAR);
	    check_improve(ch,gsn_envenom,FALSE,3);
	    WAIT_STATE(ch,skill_table[gsn_envenom].beats);
	    return;
	}
    }

    act( L"$p'yi zehirleyemezsin.",ch,obj,NULL,TO_CHAR);
    return;
}

void do_fill( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH];
    wchar_t buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *fountain;
    bool found;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( L"Neyi dolduracaksın?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
    {
	send_to_char( L"Sende öyle birşey yok.\n\r", ch );
	return;
    }

    found = FALSE;
    for ( fountain = ch->in_room->contents; fountain != NULL;
	fountain = fountain->next_content )
    {
	if ( fountain->item_type == ITEM_FOUNTAIN )
	{
	    found = TRUE;
	    break;
	}
    }

    if ( !found )
    {
	send_to_char( L"Burada çeşme yok!\n\r", ch );
	return;
    }

    if ( obj->item_type != ITEM_DRINK_CON )
    {
	send_to_char( L"Onu dolduramazsın.\n\r", ch );
	return;
    }

    if ( obj->value[1] != 0 && obj->value[2] != fountain->value[2] )
    {
	send_to_char( L"İçinde başka bir sıvı var.\n\r", ch );
	return;
    }

    if ( obj->value[1] >= obj->value[0] )
    {
	send_to_char( L"Taşıyıcın dolu.\n\r", ch );
	return;
    }

    swprintf( buf, MAX_STRING_LENGTH-1, L"$P içindeki %s ile $p'yi dolduruyorsun.",
	liq_table[fountain->value[2]].liq_name_tr);
    act( buf, ch, obj,fountain, TO_CHAR );
    swprintf( buf, MAX_STRING_LENGTH-1, L"$n $P içindeki %s ile $p'yi dolduruyor.",
	liq_table[fountain->value[2]].liq_name_tr);
    act(buf,ch,obj,fountain,TO_ROOM);
    obj->value[2] = fountain->value[2];
    obj->value[1] = obj->value[0];

}

void do_pour(CHAR_DATA *ch, wchar_t *argument)
{
    wchar_t arg[MAX_STRING_LENGTH],buf[MAX_STRING_LENGTH];
    OBJ_DATA *out, *in;
    CHAR_DATA *vch = NULL;
    int amount;

    argument = one_argument(argument,arg);

    if (arg[0] == '\0' || argument[0] == '\0')
    {
	send_to_char( L"Neyin içine dökeceksin?\n\r",ch);
	return;
    }


    if ((out = get_obj_carry(ch,arg)) == NULL)
    {
	send_to_char( L"Sende ondan yok.\n\r",ch);
	return;
    }

    if (out->item_type != ITEM_DRINK_CON)
    {
	send_to_char( L"O içecek taşıyıcısı değil.\n\r",ch);
	return;
    }

    if (!wcscasecmp(argument,L"dışarı"))
    {
	if (out->value[1] == 0)
	{
	    send_to_char( L"Zaten boş.\n\r",ch);
	    return;
	}

	out->value[1] = 0;
	out->value[3] = 0;
        if ( !IS_WATER( ch->in_room ) )
			{
			swprintf( buf, MAX_STRING_LENGTH-1, L"$p'yi ters çevirip içindeki %s'yi yere boşaltıyorsun.",liq_table[out->value[2]].liq_name_tr);
			act(buf,ch,out,NULL,TO_CHAR);

			swprintf( buf, MAX_STRING_LENGTH-1, L"$n $p'yi ters çevirip içindeki %s'yi yere boşaltıyor.",liq_table[out->value[2]].liq_name_tr);
			act(buf,ch,out,NULL,TO_ROOM);
	}
	else  {
	  swprintf( buf, MAX_STRING_LENGTH-1, L"$p'yi ters çevirip %s'yi suya boşaltıyorsun.",liq_table[out->value[2]].liq_name_tr);
	  act(buf,ch,out,NULL,TO_CHAR);

	  swprintf( buf, MAX_STRING_LENGTH-1, L"$n $p'yi ters çevirip içindeki %s'yi yere boşaltıyor.",liq_table[out->value[2]].liq_name_tr);
	  act(buf,ch,out,NULL,TO_ROOM);
	}
	return;
    }

    if ((in = get_obj_here(ch,argument)) == NULL)
    {
	vch = get_char_room(ch,argument);

	if (vch == NULL)
	{
	    send_to_char( L"Neyin içine Pour edeceksin?\n\r",ch);
	    return;
	}

	in = get_hold_char(vch);

	if (in == NULL)
	{
	    send_to_char( L"Hiçbir şey tutmuyorsun.",ch);
 	    return;
	}
    }

    if (in->item_type != ITEM_DRINK_CON)
    {
	send_to_char( L"Yalnızca içecek taşıyıcılarının içine pour yapabilirsin.\n\r",ch);
	return;
    }

    if (in == out)
    {
	send_to_char( L"Fizik kurallarını değiştiremezsin!\n\r",ch);
	return;
    }

    if (in->value[1] != 0 && in->value[2] != out->value[2])
    {
	send_to_char( L"Aynı sıvıyı taşımıyorlar.\n\r",ch);
	return;
    }

    if (out->value[1] == 0)
    {
	act( L"$p içinde dökülebilecek birşey yok.",ch,out,NULL,TO_CHAR);
	return;
    }

    if (in->value[1] >= in->value[0])
    {
	act( L"$p ağzına kadar dolu.",ch,in,NULL,TO_CHAR);
	return;
    }

    amount = UMIN(out->value[1],in->value[0] - in->value[1]);

    in->value[1] += amount;
    out->value[1] -= amount;
    in->value[2] = out->value[2];

    if (vch == NULL)
    {
    	swprintf( buf, MAX_STRING_LENGTH-1, L"%s'i $p'den $P'ye döküyorsun.",liq_table[out->value[2]].liq_name_tr);
    	act(buf,ch,out,in,TO_CHAR);
    	swprintf( buf, MAX_STRING_LENGTH-1, L"$n %s'i $p'den $P'ye döküyor.",liq_table[out->value[2]].liq_name_tr);
    	act(buf,ch,out,in,TO_ROOM);
    }
    else
    {
        swprintf( buf, MAX_STRING_LENGTH-1, L"$N için %s döküyorsun.",liq_table[out->value[2]].liq_name_tr);
        act(buf,ch,NULL,vch,TO_CHAR);
		swprintf( buf, MAX_STRING_LENGTH-1, L"$n senin için biraz %s döküyor.",liq_table[out->value[2]].liq_name_tr);
		act(buf,ch,NULL,vch,TO_VICT);
        swprintf( buf, MAX_STRING_LENGTH-1, L"$n $N için biraz %s döküyor.",liq_table[out->value[2]].liq_name_tr);
        act(buf,ch,NULL,vch,TO_NOTVICT);
    }

}

void do_drink( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int amount;
    int liquid;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	for ( obj = ch->in_room->contents; obj; obj = obj->next_content )
	{
	    if ( obj->item_type == ITEM_FOUNTAIN )
		break;
	}

	if ( obj == NULL )
	{
	    send_to_char( L"Ne içeceksin?\n\r", ch );
	    return;
	}
    }
    else
    {
	if ( ( obj = get_obj_here( ch, arg ) ) == NULL )
	{
	    send_to_char( L"Onu bulamıyorsun.\n\r", ch );
	    return;
	}
    }

    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK] > 10 && number_range(0,7) < 1)
    {
	send_to_char( L"Ağzını bulamıyorsun.  *Hıck*\n\r", ch );
	return;
    }

    switch ( obj->item_type )
    {
    default:
	send_to_char( L"Ondan içemezsin.\n\r", ch );
	return;

    case ITEM_FOUNTAIN:
        if ( ( liquid = obj->value[2] )  < 0 )
        {
            bug( L"Do_drink: bad liquid number %d.", liquid );
            liquid = obj->value[2] = 0;
        }
	amount = liq_table[liquid].liq_affect[4] * 3;
	break;

    case ITEM_DRINK_CON:
	if ( obj->value[1] <= 0 )
	{
	    send_to_char( L"Çoktan boşalmış.\n\r", ch );
	    return;
	}

	if ( ( liquid = obj->value[2] )  < 0 )
	{
	    bug( L"Do_drink: bad liquid number %d.", liquid );
	    liquid = obj->value[2] = 0;
	}

        amount = liq_table[liquid].liq_affect[4];
        amount = UMIN(amount, obj->value[1]);
	break;
     }
    if (!IS_NPC(ch) && !IS_IMMORTAL(ch)
    &&  ch->pcdata->condition[COND_FULL] > 80)
    {
	send_to_char( L"Daha fazla içemeyecek kadar dolusun.\n\r",ch);
	return;
    }

    act( L"$n $p içinden $T içiyor.",
	ch, obj, liq_table[liquid].liq_name_tr, TO_ROOM );
    act( L"$p içinden $T içiyorsun.",
	ch, obj, liq_table[liquid].liq_name_tr, TO_CHAR );

    if ( ch->fighting != NULL )
      WAIT_STATE( ch, 3 * PULSE_VIOLENCE );

    gain_condition( ch, COND_DRUNK,
	amount * liq_table[liquid].liq_affect[COND_DRUNK] / 36 );
    gain_condition( ch, COND_FULL,
	amount * liq_table[liquid].liq_affect[COND_FULL] / 2 );
    gain_condition( ch, COND_THIRST,
	amount * liq_table[liquid].liq_affect[COND_THIRST] / 5 );
    gain_condition(ch, COND_HUNGER,
	amount * liq_table[liquid].liq_affect[COND_HUNGER] / 1 );

    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK]  > 10 )
	{
		send_to_char( L"Sarhoş olduğunu hissediyorsun.\n\r", ch );
	}
    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_FULL]   > 60 )
	send_to_char( L"Dolusun.\n\r", ch );
    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_THIRST] > 60 )
	{
		send_to_char( L"Susuzluğunu giderdin.\n\r", ch );
	}

    if ( obj->value[3] != 0 )
    {
	/* The drink was poisoned ! */
	AFFECT_DATA af;

	act( L"$n boğazına kaçan sıvı yüzünden öğürüyor.", ch, NULL, NULL, TO_ROOM );
	send_to_char( L"Boğazına kaçan sıvı yüzünden öğürüyorsun.\n\r", ch );
	af.where     = TO_AFFECTS;
	af.type      = gsn_poison;
	af.level     = number_fuzzy(amount);
	af.duration  = 3 * amount;
	af.location  = APPLY_NONE;
	af.modifier  = 0;
	af.bitvector = AFF_POISON;
	affect_join( ch, &af );
    }

    if (obj->value[0] > 0)
        obj->value[1] -= amount;
    return;
}



void do_eat( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;


    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( L"Ne yiyeceksin?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
    {
	send_to_char( L"Sende öyle birşey yok.\n\r", ch );
	return;
    }

    if ( !IS_IMMORTAL(ch) )
    {
	if ( obj->item_type != ITEM_FOOD && obj->item_type != ITEM_PILL )
	{
	    send_to_char( L"Yenilebilir değil.\n\r", ch );
	    return;
	}

	if ( !IS_NPC(ch) && ch->pcdata->condition[COND_FULL] > 80 )
	{
	    send_to_char( L"Miden daha fazlasını kaldırmaz.\n\r", ch );
	    return;
	}
    }

    act( L"$n $p yiyor.",  ch, obj, NULL, TO_ROOM );
    act( L"$p yiyorsun.", ch, obj, NULL, TO_CHAR );
    if ( ch->fighting != NULL )
      WAIT_STATE( ch, 3 * PULSE_VIOLENCE );

    switch ( obj->item_type )
    {

    case ITEM_FOOD:
	if ( !IS_NPC(ch) )
	{
	    int condition;

	    condition = ch->pcdata->condition[COND_HUNGER];
	    gain_condition( ch, COND_FULL, obj->value[0]*2 );
	    gain_condition( ch, COND_HUNGER, obj->value[1]*2 );
	    if ( condition == 0 && ch->pcdata->condition[COND_HUNGER] > 0 )
		send_to_char( L"Artık aç değilsin.\n\r", ch );
	    else if ( ch->pcdata->condition[COND_FULL] > 60 )
		send_to_char( L"Doydun.\n\r", ch );
	}

	if ( obj->value[3] != 0 )
	{
	    /* The food was poisoned! */
	    AFFECT_DATA af;

	    act( L"$n boğazına kaçan lokmayla öğürüyor.", ch, 0, 0, TO_ROOM );
	    send_to_char( L"Boğazına kaçan lokmayla öğürüyorsun.\n\r", ch );

	    af.where	 = TO_AFFECTS;
	    af.type      = gsn_poison;
	    af.level 	 = number_fuzzy(obj->value[0]);
	    af.duration  = 2 * obj->value[0];
	    af.location  = APPLY_NONE;
	    af.modifier  = 0;
	    af.bitvector = AFF_POISON;
	    affect_join( ch, &af );
	}
	break;

    case ITEM_PILL:
	obj_cast_spell( obj->value[1], obj->value[0], ch, ch, NULL );
	obj_cast_spell( obj->value[2], obj->value[0], ch, ch, NULL );
	obj_cast_spell( obj->value[3], obj->value[0], ch, ch, NULL );
	break;
    }

    extract_obj( obj );
    return;
}



/*
 * Remove an object. Only for non-multi-wear locations
 */
bool remove_obj_loc( CHAR_DATA *ch, int iWear, bool fReplace )
{
    OBJ_DATA *obj;

    if ( ( obj = get_eq_char( ch, iWear ) ) == NULL )
	return TRUE;

    if ( !fReplace )
	return FALSE;

    if ( IS_SET(obj->extra_flags, ITEM_NOREMOVE) )
    {
      act( L"$p çıkmıyor.", ch, obj, NULL, TO_CHAR );
	return FALSE;
    }

    if (( obj->item_type == ITEM_TATTOO ) && ( !IS_IMMORTAL(ch) ) )
    {
      act( L"$p ancak yırtılarak çıkarılabilir.", ch, obj, NULL, TO_CHAR );
	return FALSE;
    }

    if ( iWear == WEAR_STUCK_IN )
    {
        unequip_char( ch, obj );

	if ( get_eq_char(ch,WEAR_STUCK_IN) == NULL)
	{
	  if  (is_affected(ch,gsn_arrow)) affect_strip(ch,gsn_arrow);
	  if  (is_affected(ch,gsn_spear)) affect_strip(ch,gsn_spear);
	}
  act( L"$p'yi acı içinde çıkarıyorsun.", ch, obj, NULL, TO_CHAR );
	act( L"$n $p'yi acı içinde çıkarıyor.", ch, obj, NULL, TO_ROOM );
	WAIT_STATE(ch,4);
	return TRUE;
    }

    unequip_char( ch, obj );
    act( L"$n $p kullanmayı bırakıyor.", ch, obj, NULL, TO_ROOM );
    act( L"$p kullanmayı bırakıyorsun.", ch, obj, NULL, TO_CHAR );

    return TRUE;
}

/*
 * Remove an object.
 */
bool remove_obj( CHAR_DATA *ch, OBJ_DATA *obj, bool fReplace )
{
    if ( obj == NULL )
	return TRUE;

    if ( !fReplace )
	return FALSE;

    if ( IS_SET(obj->extra_flags, ITEM_NOREMOVE) )
    {
      act( L"$p çıkarılamaz.", ch, obj, NULL, TO_CHAR );
	return FALSE;
    }

    if (( obj->item_type == ITEM_TATTOO ) && ( !IS_IMMORTAL(ch) ) )
    {
      act( L"$p ancak yırtılarak çıkarılabilir.", ch, obj, NULL, TO_CHAR );
	return FALSE;
    }

    if ( obj->wear_loc == WEAR_STUCK_IN )
    {
        unequip_char( ch, obj );

	if ( get_eq_char(ch,WEAR_STUCK_IN) == NULL)
	{
	  if  (is_affected(ch,gsn_arrow)) affect_strip(ch,gsn_arrow);
	  if  (is_affected(ch,gsn_spear)) affect_strip(ch,gsn_spear);
	}
  act( L"$p'yi acı içinde çıkarıyorsun.", ch, obj, NULL, TO_CHAR );
	act( L"$n $p'yi acı içinde çıkarıyor.", ch, obj, NULL, TO_ROOM );
	WAIT_STATE(ch,4);
	return TRUE;
    }

    unequip_char( ch, obj );
    act( L"$n $p kullanmayı bırakıyor.", ch, obj, NULL, TO_ROOM );
    act( L"$p kullanmayı bırakıyorsun.", ch, obj, NULL, TO_CHAR );

    return TRUE;
}



/*
 * Wear one object.
 * Optional replacement of existing objects.
 * Big repetitive code, ick.
 */
void wear_obj( CHAR_DATA *ch, OBJ_DATA *obj, bool fReplace )
{
    wchar_t buf[MAX_STRING_LENGTH];
    int wear_level;

    wear_level = ch->level;

    if ( (class_table[ch->iclass].fMana && obj->item_type == ITEM_ARMOR)
	|| (!class_table[ch->iclass].fMana && obj->item_type == ITEM_WEAPON))
		wear_level += 3;

    if ( wear_level < obj->level )
    {
      swprintf( buf, MAX_STRING_LENGTH-1, L"Bunu kullanabilmek için seviyen en az %d olmalı.\n\r",
	    obj->level );
	send_to_char( buf, ch );
  act( L"$n $p'yi kullanmayı denedi, ama çok deneyimsiz.",
	    ch, obj, NULL, TO_ROOM );
	return;
    }

    if ( obj->item_type == ITEM_LIGHT )
    {
	if (get_eq_char(ch,WEAR_BOTH) != NULL )
	 {
	  if (!remove_obj_loc( ch, WEAR_BOTH, fReplace ) )
		return;
	  hold_a_light(ch, obj, WEAR_LEFT);
	 }
	else if (get_eq_char(ch,WEAR_LEFT) == NULL )
		hold_a_light(ch, obj, WEAR_LEFT);
	else if (get_eq_char(ch,WEAR_RIGHT) == NULL )
		hold_a_light(ch, obj, WEAR_RIGHT);
	else if ( remove_obj_loc( ch, WEAR_LEFT, fReplace ) )
		hold_a_light(ch, obj, WEAR_LEFT);
	else if ( remove_obj_loc( ch, WEAR_RIGHT, fReplace ) )
		hold_a_light(ch, obj, WEAR_RIGHT);
    else send_to_char( L"Şu an bir ışık tutamazsın.\n\r",ch);
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_FINGER ) )
    {
	wear_multi(ch, obj, WEAR_FINGER, fReplace);
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_NECK ) )
    {
	wear_multi(ch, obj, WEAR_NECK, fReplace);
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_BODY ) )
    {
	if ( !remove_obj_loc( ch, WEAR_BODY, fReplace ) )
	    return;
      act( L"$n gövdesine $p giyiyor.",   ch, obj, NULL, TO_ROOM );
    	act( L"Gövdene $p giyiyorsun.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_BODY );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_HEAD ) )
    {
	if ( !remove_obj_loc( ch, WEAR_HEAD, fReplace ) )
	    return;
      act( L"$n kafasına $p takıyor.",   ch, obj, NULL, TO_ROOM );
    	act( L"Kafana $p takıyorsun.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_HEAD );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_LEGS ) )
    {
	if ( !remove_obj_loc( ch, WEAR_LEGS, fReplace ) )
	    return;
      act( L"$n bacaklarına $p giyiyor.",   ch, obj, NULL, TO_ROOM );
    	act( L"Bacaklarına $p giyiyorsun.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_LEGS );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_FEET ) )
    {
	if ( !remove_obj_loc( ch, WEAR_FEET, fReplace ) )
	    return;
      act( L"$n ayaklarına $p giyiyor.",   ch, obj, NULL, TO_ROOM );
    	act( L"Ayaklarına $p giyiyorsun.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_FEET );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_HANDS ) )
    {
	if ( !remove_obj_loc( ch, WEAR_HANDS, fReplace ) )
	    return;
      act( L"$n ellerine $p giyiyor.",   ch, obj, NULL, TO_ROOM );
    	act( L"Ellerine $p giyiyorsun.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_HANDS );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_ARMS ) )
    {
	if ( !remove_obj_loc( ch, WEAR_ARMS, fReplace ) )
	    return;
      act( L"$n kollarına $p takıyor.",   ch, obj, NULL, TO_ROOM );
    	act( L"Kollarına $p takıyorsun.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_ARMS );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_ABOUT ) )
    {
	if ( !remove_obj_loc( ch, WEAR_ABOUT, fReplace ) )
	    return;
      act( L"$n gövdesine $p giyiyor.",   ch, obj, NULL, TO_ROOM );
    	act( L"Gövdene $p giyiyorsun.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_ABOUT );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_WAIST ) )
    {
	if ( !remove_obj_loc( ch, WEAR_WAIST, fReplace ) )
	    return;
      act( L"$n beline $p takıyor.",   ch, obj, NULL, TO_ROOM );
    	act( L"Beline $p takıyorsun.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_WAIST );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_WRIST ) )
    {
	wear_multi(ch, obj, WEAR_WRIST, fReplace);
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_SHIELD ) )
    {
	if (get_eq_char(ch,WEAR_BOTH) != NULL )
	 {
	  if (!remove_obj_loc( ch, WEAR_BOTH, fReplace ) )
		return;
	  hold_a_shield(ch, obj, WEAR_LEFT);
	 }
	else if (get_eq_char(ch,WEAR_LEFT) == NULL )
		hold_a_shield(ch, obj, WEAR_LEFT);
	else if (get_eq_char(ch,WEAR_RIGHT) == NULL )
		hold_a_shield(ch, obj, WEAR_RIGHT);
	else if ( remove_obj_loc( ch, WEAR_LEFT, fReplace ) )
		hold_a_shield(ch, obj, WEAR_LEFT);
	else if ( remove_obj_loc( ch, WEAR_RIGHT, fReplace ) )
		hold_a_shield(ch, obj, WEAR_RIGHT);
    else send_to_char( L"Şu an bir kalkan tutamazsın.\n\r",ch);
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WIELD ) )
    {
	wear_a_wield(ch, obj, fReplace);
	return;
    }

    if ( CAN_WEAR( obj, ITEM_HOLD ) )
    {
	if (get_eq_char(ch,WEAR_BOTH) != NULL )
	 {
	  if (!remove_obj_loc( ch, WEAR_BOTH, fReplace ) )
		return;
	  hold_a_thing(ch, obj, WEAR_LEFT);
	 }
	else if (get_eq_char(ch,WEAR_LEFT) == NULL )
		hold_a_thing(ch, obj, WEAR_LEFT);
	else if (get_eq_char(ch,WEAR_RIGHT) == NULL )
		hold_a_thing(ch, obj, WEAR_RIGHT);
	else if ( remove_obj_loc( ch, WEAR_LEFT, fReplace ) )
		hold_a_thing(ch, obj, WEAR_LEFT);
	else if ( remove_obj_loc( ch, WEAR_RIGHT, fReplace ) )
		hold_a_thing(ch, obj, WEAR_RIGHT);
    else send_to_char( L"Şu an birşey tutamazsın.\n\r",ch);
	return;
    }


    if ( CAN_WEAR(obj,ITEM_WEAR_FLOAT) )
    {
	if (!remove_obj_loc(ch,WEAR_FLOAT, fReplace) )
	    return;
      act( L"$n $p'yi yanında süzülmesi için bırakıyor.",ch,obj,NULL,TO_ROOM);
    	act( L"$p'yi yanında süzülmesi için bırakıyorsun.",ch,obj,NULL,TO_CHAR);
	equip_char(ch,obj,WEAR_FLOAT);
	return;
    }

    if ( CAN_WEAR(obj,ITEM_WEAR_TATTOO)  && IS_IMMORTAL (ch) )
    {
	wear_multi(ch, obj, WEAR_TATTOO, fReplace);
	return;
    }

    if ( fReplace )
    send_to_char( L"Onu giyemez veya tutamazsın.\n\r", ch );

    return;
}



void do_wear( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
      send_to_char( L"Neyi giyecek ya da tutacaksın?\n\r", ch );
	return;
    }

    if ( !wcscasecmp( arg, L"tümü" ) )
    {
	OBJ_DATA *obj_next;

	for ( obj = ch->carrying; obj != NULL; obj = obj_next )
	{
	    obj_next = obj->next_content;
	    if ( obj->wear_loc == WEAR_NONE && can_see_obj( ch, obj ) )
		wear_obj( ch, obj, FALSE );
	}
	return;
    }
    else
    {
	if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
	{
    send_to_char( L"Sende öyle birşey yok.\n\r", ch );
	    return;
	}

	wear_obj( ch, obj, TRUE );
    }

    return;
}



void do_remove( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
      send_to_char( L"Neyi çıkaracaksın?\n\r", ch );
	return;
    }


    if ( !wcscasecmp( arg, L"tümü" ) )
    {
        OBJ_DATA *obj_next;

        for ( obj = ch->carrying; obj != NULL; obj = obj_next )
        {
            obj_next = obj->next_content;
            if ( obj->wear_loc != WEAR_NONE && can_see_obj( ch, obj ) )
                remove_obj( ch, obj, TRUE );
        }
        return;
    }

    if ( ( obj = get_obj_wear( ch, arg ) ) == NULL )
    {
      send_to_char( L"Sende öyle birşey yok.\n\r", ch );
	return;
    }

    remove_obj( ch, obj, TRUE );
    return;
}



void do_sacrifice( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH];
    wchar_t buf[MAX_STRING_LENGTH];
    wchar_t buf2[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *obj_content;
    OBJ_DATA *obj_next;
    OBJ_DATA *two_objs[2];
    int silver;
    int iScatter;
    bool fScatter;
    int count;
    long toplam_silver;


    /* variables for AUTOSPLIT */
    CHAR_DATA *gch;
    int members;
    wchar_t buffer[100];


    one_argument( argument, arg );

    if ( arg[0] == '\0' || !wcscasecmp( arg, ch->name ) )
    {
      act( L"$n kendisini tanrılara sunuyor.",
	    ch, NULL, NULL, TO_ROOM );
	send_to_char( L"Tanrılar teklifini takdir ediyor...\n\r", ch );
	return;
    }

    /* kurban tümü */
    count=0;
    toplam_silver=0;
    if ( !wcscasecmp( arg, L"tümü" ) )
      {
      for ( obj = ch->in_room->contents; obj != NULL; obj = obj_next )
      {
        obj_next = obj->next_content;
        if ( can_see_obj( ch, obj ) && !IS_OBJ_STAT(obj,ITEM_NOPURGE) && obj->item_type != ITEM_CORPSE_PC  && CAN_WEAR(obj, ITEM_TAKE) && !CAN_WEAR(obj, ITEM_NO_SAC) )
        {
          silver = UMAX(1,number_fuzzy(obj->level));
          if (obj->item_type != ITEM_CORPSE_NPC && obj->item_type != ITEM_CORPSE_PC)
              silver = UMIN(silver,obj->cost);
          count++;//kurban edilen eşya sayısı
          toplam_silver+=silver;//kurban etme işleminden toplanan akçe sayısı
          extract_obj( obj );
        }
      }
    if (count>0)//eger birşeyler kurban edilebilmişse
    {
      swprintf( buf, MAX_STRING_LENGTH-1, L"Tanrılar %d kurbanın için %ld akçe veriyor.\n\r",count,toplam_silver);
      send_to_char(buf,ch);
      ch->silver += toplam_silver;
    }
    return;
      }
    /* kurban tümü bitti */

    obj = get_obj_list( ch, arg, ch->in_room->contents );
    if ( obj == NULL )
    {
      send_to_char( L"Onu bulamıyorsun.\n\r", ch );
	return;
    }

    if ( obj->item_type == ITEM_CORPSE_PC && ch->level < MAX_LEVEL )
    {
      send_to_char( L"Tanrılar bundan hoşlanmaz.\n\r",ch);
     return;
    }


    if ( !CAN_WEAR(obj, ITEM_TAKE) || CAN_WEAR(obj, ITEM_NO_SAC))
    {
      act( L"$p kabul edilebilir bir kurban değil.", ch, obj, 0, TO_CHAR );
	return;
    }

    silver = UMAX(1,number_fuzzy(obj->level));

    if (obj->item_type != ITEM_CORPSE_NPC && obj->item_type != ITEM_CORPSE_PC)
    	silver = UMIN(silver,obj->cost);

    if (silver == 1)
        send_to_char( L"Tanrılar kurbanın için 1 akçe veriyor.\n\r", ch );
    else
    {
      swprintf( buf, MAX_STRING_LENGTH-1, L"Tanrılar kurbanın için %d akçe veriyor.\n\r",silver);
	send_to_char(buf,ch);
    }

    ch->silver += silver;

    if (IS_SET(ch->act,PLR_AUTOSPLIT) )
    { /* AUTOSPLIT code */
    	members = 0;
	for (gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    	{
    	    if ( is_same_group( gch, ch ) )
            members++;
    	}

	if ( members > 1 && silver > 1)
	{
	    swprintf(buffer, 100-1, L"%d",silver);
	    do_split(ch,buffer);
	}
    }

    act( L"$n tanrılara $p kurban ediyor.", ch, obj, NULL, TO_ROOM );

    if (IS_SET(obj->progtypes,OPROG_SAC))
      if ( (obj->pIndexData->oprogs->sac_prog) (obj,ch) )
        return;

    wiznet( L"$N sends up $p as a burnt offering.",
	   ch,obj,WIZ_SACCING,0,0);
    fScatter = TRUE;
    if ( (obj->item_type == ITEM_CORPSE_NPC ) ||
	  (obj->item_type == ITEM_CORPSE_PC  ) )
    {
      iScatter = 0;
      for ( obj_content = obj->contains; obj_content; obj_content = obj_next )
      {
  	obj_next = obj_content->next_content;
	two_objs[iScatter<1?0:1] = obj_content;
	obj_from_obj( obj_content );
	obj_to_room( obj_content, ch->in_room );
	iScatter++;
      }
      if ( iScatter == 1 )  {
        act( L"Kurban etmenin ardından $p ortaya çıkıyor.", ch, two_objs[0], NULL, TO_CHAR);
      	act( L"$s kurbanının ardından $p ortaya çıkıyor.", ch, two_objs[0], NULL, TO_ROOM);
      }
      if ( iScatter == 2 )  {
        act( L"Kurban etmenin ardından $p ve $P ortaya çıkıyor.", ch, two_objs[0], two_objs[1], TO_CHAR);
      	act( L"$s kurbanının ardından $p ve $P ortaya çıkıyor.", ch, two_objs[0], two_objs[1], TO_ROOM);
      }
      swprintf( buf, MAX_STRING_LENGTH-1, L"Cesedi kurban ettiğinde " );
      swprintf( buf2, MAX_STRING_LENGTH-1, L"$s cesedi kurban etmesiyle " );
      if ( iScatter < 3 )
		   fScatter = FALSE;
	else if ( iScatter < 5 )  {
    wcscat( buf, L"üzerindeki birkaç şey " );
wcscat( buf2, L"üzerindeki birkaç şey " );
 	}
	else if ( iScatter < 9 )  {
    wcscat( buf, L"üzerindeki bir miktar eşya " );
                wcscat( buf2, L"üzerindeki bir miktar eşya " );
        }
	else if ( iScatter < 15 )  {
    wcscat( buf, L"üzerindeki bir sürü şey " );
                wcscat( buf2, L"üzerindeki bir sürü şey " );
        }
	else  {
    wcscat( buf, L"üzerindeki bir sürü şey " );
                wcscat( buf2, L"üzerindeki bir sürü şey " );
        }

      switch( ch->in_room->sector_type )  {
        case SECT_FIELD: wcscat( buf, L"yere saçılıyor." );
                               wcscat( buf2, L"yere saçılıyor." );
                               break;
      	case SECT_FOREST: wcscat( buf, L"yere saçılıyor." );
                                wcscat( buf2, L"yere saçılıyor." );
                                break;
      	case SECT_WATER_SWIM: wcscat( buf, L"suya saçılıyor." );
                                wcscat( buf2, L"suya saçılıyor." );
                                break;
      	case SECT_WATER_NOSWIM: wcscat( buf, L"suya saçılıyor." );
                                wcscat( buf2, L"suya saçılıyor." );
                                break;
      	default: wcscat( buf, L"etrafa saçılıyor." );
                            wcscat( buf2, L"etrafa saçılıyor." );
                            break;
      }
      if ( fScatter )  {
	act( buf, ch, NULL, NULL, TO_CHAR );
	act( buf2, ch, NULL, NULL, TO_ROOM );
      }

    }

    extract_obj( obj );
    return;
}



void do_quaff( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if (ch->cabal == CABAL_BATTLE && !IS_IMMORTAL(ch))
    {
      send_to_char( L"Sen Öfke Kabalı üyesisin, pis bir büyücü değil!\n\r",ch);
      return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( L"Neyi yudumlayacaksın?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
    {
	send_to_char( L"O iksirden sende yok.\n\r", ch );
	return;
    }

    if ( obj->item_type != ITEM_POTION )
    {
	send_to_char( L"Yalnız iksirleri yudumlayabilirsin.\n\r", ch );
	return;
    }

    if (ch->level < obj->level)
    {
	send_to_char( L"O iksir kaldıramayacağın kadar güçlü.\n\r",ch);
	return;
    }


    act( L"$n $p yudumluyor.", ch, obj, NULL, TO_ROOM );
    act( L"$p yudumluyorsun.", ch, obj, NULL ,TO_CHAR );

    obj_cast_spell( obj->value[1], obj->value[0], ch, ch, NULL );
    obj_cast_spell( obj->value[2], obj->value[0], ch, ch, NULL );
    obj_cast_spell( obj->value[3], obj->value[0], ch, ch, NULL );

    if (( ch->last_fight_time != -1 &&
        (current_time - ch->last_fight_time)<FIGHT_DELAY_TIME) ||
	(ch->fighting != NULL) )
      {
        WAIT_STATE( ch,2 * PULSE_VIOLENCE );
      }

    extract_obj( obj );
    obj_to_char( create_object(get_obj_index(OBJ_VNUM_POTION_VIAL),0),ch);

    if (IS_NPC(ch))	do_drop(ch, (wchar_t*)"vial");

    return;
}



void do_recite( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg1[MAX_INPUT_LENGTH];
    wchar_t arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *scroll;
    OBJ_DATA *obj;

    if ( ch->cabal == CABAL_BATTLE )
    {
	send_to_char( L"Parşömen okumak?!  Sen Öfke Kabalı üyesisin, pis bir büyücü değil!\n\r", ch );
	return;
    }

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( ( scroll = get_obj_carry( ch, arg1 ) ) == NULL )
    {
	send_to_char( L"O parşömen sende yok.\n\r", ch );
	return;
    }

    if ( scroll->item_type != ITEM_SCROLL )
    {
	send_to_char( L"Yalnız parşömenler recite edilebilir.\n\r", ch );
	return;
    }


    if ( ch->level < scroll->level)
    {
	send_to_char( L"Parşömen kavrayamayacağın kadar karmaşık.\n\r",ch);
	return;
    }

    obj = NULL;
    if ( arg2[0] == '\0' )
    {
	victim = ch;
    }
    else
    {
	if ( ( victim = get_char_room ( ch, arg2 ) ) == NULL
	&&   ( obj    = get_obj_here  ( ch, arg2 ) ) == NULL )
	{
	    send_to_char( L"Onu bulamıyorsun.\n\r", ch );
	    return;
	}
    }

    act( L"$n $p okuyor.", ch, scroll, NULL, TO_ROOM );
    act( L"$p okuyorsun.", ch, scroll, NULL, TO_CHAR );

    if (number_percent() >= get_skill(ch,gsn_scrolls) * 4/5 )
    {
	send_to_char( L"Bir heceyi yanlış telaffuz ettin.\n\r",ch);
	check_improve(ch,gsn_scrolls,FALSE,2);
    }

    else
    {
    	obj_cast_spell( scroll->value[1], scroll->value[0], ch, victim, obj );
    	obj_cast_spell( scroll->value[2], scroll->value[0], ch, victim, obj );
    	obj_cast_spell( scroll->value[3], scroll->value[0], ch, victim, obj );
	check_improve(ch,gsn_scrolls,TRUE,2);

        if (( ch->last_fight_time != -1 &&
          (current_time - ch->last_fight_time)<FIGHT_DELAY_TIME) ||
          (ch->fighting != NULL) )
	      {
	        WAIT_STATE( ch, 2 * PULSE_VIOLENCE );
	      }
    }

    extract_obj( scroll );
    return;
}



void do_brandish( CHAR_DATA *ch, wchar_t *argument )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    OBJ_DATA *staff;
    int sn;

    if ( ch->cabal == CABAL_BATTLE )
    {
	send_to_char( L"Pis bir büyücü değilsin!\n\r", ch );
	return;
    }

    if ( ( staff = get_hold_char( ch ) ) == NULL )
    {
	send_to_char( L"Elinde tuttuğun birşey yok.\n\r", ch );
	return;
    }

    if ( staff->item_type != ITEM_STAFF )
    {
	send_to_char( L"Yalnız asalar savrulabilir.\n\r", ch );
	return;
    }

    if ( ( sn = staff->value[3] ) < 0
    ||   sn >= MAX_SKILL
    ||   skill_table[sn].spell_fun == 0 )
    {
	bug( L"Do_brandish: bad sn %d.", sn );
	return;
    }

    WAIT_STATE( ch, 2 * PULSE_VIOLENCE );

    if ( staff->value[2] > 0 )
    {
	act( L"$n $p savuruyor.", ch, staff, NULL, TO_ROOM );
	act( L"$p savuruyorsun.",  ch, staff, NULL, TO_CHAR );
	if ( ch->level+3 < staff->level
	||   number_percent() >= 10 + get_skill(ch,gsn_staves) * 4/5)
 	{
	    act ( L"$p'yi uyandırmayı başaramadın.",ch,staff,NULL,TO_CHAR);
	    act ( L"...ve hiçbir şey olmuyor.",ch,NULL,NULL,TO_ROOM);
	    check_improve(ch,gsn_staves,FALSE,2);
	}

	else for ( vch = ch->in_room->people; vch; vch = vch_next )
	{
	    vch_next	= vch->next_in_room;

	    switch ( skill_table[sn].target )
	    {
	    default:
		bug( L"Do_brandish: bad target for sn %d.", sn );
		return;

	    case TAR_IGNORE:
		if ( vch != ch )
		    continue;
		break;

	    case TAR_CHAR_OFFENSIVE:
		if ( IS_NPC(ch) ? IS_NPC(vch) : !IS_NPC(vch) )
		    continue;
		break;

	    case TAR_CHAR_DEFENSIVE:
		if ( IS_NPC(ch) ? !IS_NPC(vch) : IS_NPC(vch) )
		    continue;
		break;

	    case TAR_CHAR_SELF:
		if ( vch != ch )
		    continue;
		break;
	    }

	    obj_cast_spell( staff->value[3], staff->value[0], ch, vch, NULL );
	    check_improve(ch,gsn_staves,TRUE,2);
	}
    }

    if ( --staff->value[2] <= 0 )
    {
	act( L"$n's $p parlayarak yokoluyor.", ch, staff, NULL, TO_ROOM );
	act( L"$p parlayarak yokoluyor.", ch, staff, NULL, TO_CHAR );
	extract_obj( staff );
    }

    return;
}



void do_zap( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *wand;
    OBJ_DATA *obj;

    if ( ch->cabal == CABAL_BATTLE )
    {
	send_to_char( L"Büyüyü kullanmak yerine yoketmelisin!\n\r", ch );
	return;
    }

    one_argument( argument, arg );
    if ( arg[0] == '\0' && ch->fighting == NULL )
    {
	send_to_char( L"Kimi veya neyi çarpacaksın?\n\r", ch );
	return;
    }

    if ( ( wand = get_hold_char( ch ) ) == NULL )
    {
	send_to_char( L"Elinde birşey tutmuyorsun.\n\r", ch );
	return;
    }

    if ( wand->item_type != ITEM_WAND )
    {
	send_to_char( L"Yalnız değneklerle çarpabilirsin.\n\r", ch );
	return;
    }

    obj = NULL;
    if ( arg[0] == '\0' )
    {
	if ( ch->fighting != NULL )
	{
	    victim = ch->fighting;
	}
	else
	{
	    send_to_char( L"Kimi veya neyi çarpacaksın?\n\r", ch );
	    return;
	}
    }
    else
    {
	if ( ( victim = get_char_room ( ch, arg ) ) == NULL
	&&   ( obj    = get_obj_here  ( ch, arg ) ) == NULL )
	{
	    send_to_char( L"Onu bulamıyorsun.\n\r", ch );
	    return;
	}
    }

    WAIT_STATE( ch, 2 * PULSE_VIOLENCE );

    if ( wand->value[2] > 0 )
    {
	if ( victim != NULL )
	{
	    act( L"$n $p ile $M çarpıyor.", ch, wand, victim, TO_ROOM );
	    act( L"$p ile $M çarpıyorsun.", ch, wand, victim, TO_CHAR );
	}
	else
	{
	    act( L"$n $p ile $P'yi çarpıyor.", ch, wand, obj, TO_ROOM );
	    act( L"$p ile $P'yi çarpıyorsun.", ch, wand, obj, TO_CHAR );
	}

 	if (ch->level+5 < wand->level
	||  number_percent() >= 20 + get_skill(ch,gsn_wands) * 4/5)
	{
	    act( L"$p ile uğraşının sonunda yalnız duman ve kıvılcımlar çıkıyor.",
		 ch,wand,NULL,TO_CHAR);
	    act( L"$s $p ile uğraşının sonunda yalnız duman ve kıvılcımlar çıkıyor.",
		 ch,wand,NULL,TO_ROOM);
	    check_improve(ch,gsn_wands,FALSE,2);
	}
	else
	{
	    obj_cast_spell( wand->value[3], wand->value[0], ch, victim, obj );
	    check_improve(ch,gsn_wands,TRUE,2);
	}
    }

    if ( --wand->value[2] <= 0 )
    {
	act( L"$s $p'si patlayarak parçalara ayrılıyor.", ch, wand, NULL, TO_ROOM );
	act( L"$p patlayarak parçalara ayrılıyor.", ch, wand, NULL, TO_CHAR );
	extract_obj( wand );
    }

    return;
}



void do_steal( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t buf  [MAX_STRING_LENGTH];
    wchar_t arg1 [MAX_INPUT_LENGTH];
    wchar_t arg2 [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *tmp_ch;
    OBJ_DATA *obj;
    OBJ_DATA *obj_inve;
    int percent, number;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
      send_to_char( L"Kimden ne araklayacaksın?\n\r", ch );
	return;
    }

    if ( IS_NPC(ch) && IS_SET(ch->affected_by, AFF_CHARM)
	&& (ch->master != NULL))
      {
        send_to_char( L"Bir şey araklayamayacak kadar sersemlemişsin.\n\r", ch);
	return;
      }

    if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
    {
      send_to_char( L"Burada değil.\n\r", ch);
	return;
    }

    if (!IS_NPC(victim) && victim->desc == NULL)
    {
      send_to_char( L"Bunu yapamazsın.\n\r", ch);
	return;
    }

    if ( victim == ch )
    {
	send_to_char( L"Bu biraz anlamsız.\n\r", ch );
	return;
    }

    if (is_safe(ch,victim))
	return;

    if (victim->position == POS_FIGHTING)
    {
      send_to_char( L"Denemesen daha iyi, arada darbe alabilirsin.\n\r",ch);
	return;
    }

    ch->last_death_time = -1;

    tmp_ch = (is_affected(ch,gsn_doppelganger) && !IS_IMMORTAL(victim)) ?
     ch->doppel : ch;

    WAIT_STATE( ch, skill_table[gsn_steal].beats );
    percent  = get_skill(ch, gsn_steal) + ( IS_AWAKE(victim) ? -10 : 30 );
    percent -= can_see( victim, ch ) ? 10 : 0;
    percent -= (IS_NPC(victim) && victim->pIndexData->pShop != NULL) ? 25 : 0;
    percent -= (victim->level > ch->level) ?
			((victim->level - ch->level) * 2) : 0;

    obj = NULL;
    if ( wcscasecmp( arg1, L"akçe") )
    {
	if ( ( obj = get_obj_carry( victim, arg1 ) ) == NULL )
	{
    send_to_char( L"Onu bulamıyorsun.\n\r", ch );
	 return;
	}

    }

    if (obj != NULL && obj->pIndexData->limit != -1)
    {
      if ( ( IS_OBJ_STAT(obj, ITEM_ANTI_EVIL)    && IS_EVIL(ch)    )
      ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_GOOD)    && IS_GOOD(ch)    )
      ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_NEUTRAL) && IS_NEUTRAL(ch) ) )
      {
        act( L"$p tarafından çarpıldın.", ch, obj, NULL, TO_CHAR );
      	act( L"$n $p tarafından çarpıldı.",  ch, obj, NULL, TO_ROOM );
	percent = 0;
      }

      if ( obj->pIndexData->limit < obj->pIndexData->count )
      {
        act( L"Tanrılar $p'nin araklanmasına izin vermez.", ch, obj, NULL, TO_CHAR );
      	act( L"Tanrılar $s davranışını onaylamıyor.",  ch, obj, NULL, TO_ROOM );
	percent = 0;
      }

      if( !limit_kontrol(ch,obj) )
      {
  			return;
      }
    }

    number = ( obj != NULL ) ? get_obj_number(obj) : 0;

    if ( ch->carry_number + number > can_carry_n( ch ) )
    {
      send_to_char( L"Bu kadar çok şey taşıyamazsın.\n\r", ch );
	return;
    }

    if ( victim->position == POS_FIGHTING
	||  number_percent() > percent  )
    {
	/*
	 * Failure.
	 */

	send_to_char( L"Oops.\n\r", ch );
        if ( !IS_AFFECTED( victim, AFF_SLEEP ) ) {
          victim->position= victim->position==POS_SLEEPING? POS_STANDING:
					victim->position;
          act( L"$n senden birşeyler araklamaya çalıştı.\n\r", ch, NULL, victim,TO_VICT  );
        }
        act( L"$n $Z birşey araklamaya çalıştı.\n\r",  ch, NULL, victim,TO_NOTVICT);

	switch(number_range(0,3))
	{
    case 0 :
		swprintf( buf, MAX_STRING_LENGTH-1, L"Rezil bir hırsızsın %s!", tmp_ch->name );
		break;
        case 1 :
		swprintf( buf, MAX_STRING_LENGTH-1, L"%s beni soyamadı!",tmp_ch->name);
		break;
		case 2 :
	    swprintf( buf, MAX_STRING_LENGTH-1, L"%s beni soymaya çalıştı!",tmp_ch->name );
	    break;
		case 3 :
	    swprintf( buf, MAX_STRING_LENGTH-1, L"Ellerini oradan çek %s!",tmp_ch->name);
	    break;
        }
	if ( IS_AWAKE( victim ) )
	  do_yell( victim, buf );
	if ( !IS_NPC(ch) )
	{
	    if ( IS_NPC(victim) )
	    {
	        check_improve(ch,gsn_steal,FALSE,2);
		multi_hit( victim, ch, TYPE_UNDEFINED );
	    }
	}

	return;
    }

  if ( !wcscasecmp( arg1, L"akçe"  ))
  {
    int amount_s = 0;
    amount_s = victim->silver * number_range(1, 20) / 100;

    if ( amount_s <= 0 )
    {
    send_to_char( L"Hiç akçe koparamadın.\n\r", ch );
    return;
    }

    ch->silver     += amount_s;
    victim->silver -= amount_s;
    swprintf( buf, MAX_STRING_LENGTH-1, L"Bingo!  %d akçe arakladın.\n\r",amount_s);

    send_to_char( buf, ch );
    check_improve(ch,gsn_steal,TRUE,2);
    return;
  }

    if ( !can_drop_obj( ch, obj )
   /* ||   IS_SET(obj->extra_flags, ITEM_INVENTORY)*/
   /* ||  obj->level > ch->level */)
    {
      send_to_char( L"Onu alamıyorsun.\n\r", ch );
	return;
    }

    if ( ch->carry_number + get_obj_number( obj ) > can_carry_n( ch ) )
    {
      send_to_char( L"Ellerin dolu.\n\r", ch );
	return;
    }

    if ( ch->carry_weight + get_obj_weight( obj ) > can_carry_w( ch ) )
    {
      send_to_char( L"O kadar ağırlık taşıyamazsın.\n\r", ch );
	return;
    }

    if ( !IS_SET( obj->extra_flags, ITEM_INVENTORY ) )  {
      obj_from_char( obj );
      obj_to_char( obj, ch );
      send_to_char( L"Onu aldın!\n\r", ch );
      check_improve(ch,gsn_steal,TRUE,2);
      if (IS_SET(obj->progtypes,OPROG_GET))
          (obj->pIndexData->oprogs->get_prog) (obj,ch);
    }
    else  {
      obj_inve = NULL;
      obj_inve = create_object( obj->pIndexData, 0 );
      clone_object( obj, obj_inve );
      REMOVE_BIT( obj_inve->extra_flags, ITEM_INVENTORY );
      obj_to_char( obj_inve, ch );
      send_to_char( L"Bir tanesini aldın!\n\r", ch );
      check_improve(ch,gsn_steal,TRUE,1);
      if (IS_SET(obj_inve->progtypes,OPROG_GET))
          (obj_inve->pIndexData->oprogs->get_prog) (obj_inve,ch);
    }

    return;
}

/*
 * Shopping commands.
 */
CHAR_DATA *find_keeper( CHAR_DATA *ch )
{
    wchar_t buf[MAX_STRING_LENGTH];
    CHAR_DATA *keeper;
    SHOP_DATA *pShop;

    pShop = NULL;
    for ( keeper = ch->in_room->people; keeper; keeper = keeper->next_in_room )
    {
	if ( IS_NPC(keeper) && (pShop = keeper->pIndexData->pShop) != NULL )
	    break;
    }

    if ( pShop == NULL )
    {
      send_to_char( L"Burada yapamazsın.\n\r", ch);
	return NULL;
    }

    if ( IS_SET(keeper->in_room->area->area_flag,AREA_HOMETOWN)
	 && !IS_NPC(ch) && IS_SET(ch->act,PLR_WANTED) )
    {
	do_say( keeper, (wchar_t*)"Şuçluları hoş karşılamayız!" );
  swprintf( buf, MAX_STRING_LENGTH-1, L"%s bir SUÇLU! Gördüm Onu!\n\r", ch->name );
	do_yell( keeper, buf );
	return NULL;
    }

    /*
     * Shop hours.
     */
    if ( time_info.hour < pShop->open_hour )
    {
	do_say( keeper, (wchar_t*)"Üzgünüm kapattım. Daha sonra gel." );
	return NULL;
    }

    if ( time_info.hour > pShop->close_hour )
    {
	do_say( keeper, (wchar_t*)"Üzgünüm kapattım. Yarın gel." );
	return NULL;
    }

    /*
     * Invisible or hidden people.
     */
    if ( !can_see( keeper, ch ) && !IS_IMMORTAL( ch ) )
    {
	do_say( keeper, (wchar_t*)"Göremediğim tiplerle ticaret yapmam." );
	return NULL;
    }

    return keeper;
}

/* insert an object at the right spot for the keeper */
void obj_to_keeper( OBJ_DATA *obj, CHAR_DATA *ch )
{
    OBJ_DATA *t_obj, *t_obj_next;

    /* see if any duplicates are found */
    for (t_obj = ch->carrying; t_obj != NULL; t_obj = t_obj_next)
    {
	t_obj_next = t_obj->next_content;

	if (obj->pIndexData == t_obj->pIndexData
	&&  !wcscasecmp(obj->short_descr,t_obj->short_descr))
	{
	    if (IS_OBJ_STAT(t_obj,ITEM_INVENTORY))
	    {
		extract_obj(obj);
		return;
	    }
	    obj->cost = t_obj->cost; /* keep it standard */
	    break;
	}
    }

    if (t_obj == NULL)
    {
	obj->next_content = ch->carrying;
	ch->carrying = obj;
    }
    else
    {
	obj->next_content = t_obj->next_content;
	t_obj->next_content = obj;
    }

    obj->carried_by      = ch;
    obj->in_room         = NULL;
    obj->in_obj          = NULL;
    ch->carry_number    += get_obj_number( obj );
    ch->carry_weight    += get_obj_weight( obj );
}

/* get an object from a shopkeeper's list */
OBJ_DATA *get_obj_keeper( CHAR_DATA *ch, CHAR_DATA *keeper, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int number;
    int count;

    number = number_argument( argument, arg );
    count  = 0;
    for ( obj = keeper->carrying; obj != NULL; obj = obj->next_content )
    {
        if (obj->wear_loc == WEAR_NONE
        &&  can_see_obj( keeper, obj )
	&&  can_see_obj(ch,obj)
        &&  is_name( arg, obj->name ) )
        {
            if ( ++count == number )
                return obj;

	    /* skip other objects of the same name */
	    while (obj->next_content != NULL
	    && obj->pIndexData == obj->next_content->pIndexData
	    && !wcscasecmp(obj->short_descr,obj->next_content->short_descr))
		obj = obj->next_content;
        }
    }

    return NULL;
}

/*
* fBuy TRUE ise satin alma islemi. FALSE ise satma islemi.
*/
int get_cost( CHAR_DATA *keeper, OBJ_DATA *obj, bool fBuy )
{
    SHOP_DATA *pShop;
    int cost;

    if ( obj == NULL || ( pShop = keeper->pIndexData->pShop ) == NULL )
	return 0;

    if (IS_OBJ_STAT(obj,ITEM_NOSELL))
	return 0;

    if ( fBuy )
    {
	cost = obj->cost * pShop->profit_buy  / 100;
    }
    else
    {
	OBJ_DATA *obj2;
	int itype;

	cost = 0;
	for ( itype = 0; itype < MAX_TRADE; itype++ )
	{
	    if ( obj->item_type == pShop->buy_type[itype] )
	    {
		cost = obj->cost * pShop->profit_sell / 100;
		break;
	    }
	}

	if (!IS_OBJ_STAT(obj,ITEM_SELL_EXTRACT))
	    for ( obj2 = keeper->carrying; obj2; obj2 = obj2->next_content )
	    {
	    	if ( obj->pIndexData == obj2->pIndexData
		&&   !wcscasecmp(obj->short_descr,obj2->short_descr) )
		  return 0;

	    }
    }

    if ( obj->item_type == ITEM_STAFF || obj->item_type == ITEM_WAND )
    {
	if (obj->value[1] == 0)
	    cost /= 4;
	else
	    cost = cost * obj->value[2] / obj->value[1];
    }

    return cost;
}



void do_buy( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t buf[MAX_STRING_LENGTH];
    int cost,roll;

    if ( argument[0] == '\0' )
    {
      send_to_char( L"Ne satın alacaksın?\n\r", ch );
	return;
    }

    if ( IS_SET(ch->in_room->room_flags, ROOM_PET_SHOP) )
    {
	wchar_t arg[MAX_INPUT_LENGTH];
	wchar_t buf[MAX_STRING_LENGTH];
	CHAR_DATA *pet;
	ROOM_INDEX_DATA *pRoomIndexNext;
	ROOM_INDEX_DATA *in_room;

	/* added by kio */
	smash_tilde(argument);

	if ( IS_NPC(ch) )
	    return;

	argument = one_argument(argument,arg);

	/* hack to make new thalos pets work */

	if (ch->in_room->vnum == 9621)
	    pRoomIndexNext = get_room_index(9706);
	else
	    pRoomIndexNext = get_room_index( ch->in_room->vnum + 1 );
	if ( pRoomIndexNext == NULL )
	{
	    bug( L"Do_buy: bad pet shop at vnum %d.", ch->in_room->vnum );
      send_to_char( L"Üzgünüm, buradan satın alamazsın.\n\r", ch );
	    return;
	}

	in_room     = ch->in_room;
	ch->in_room = pRoomIndexNext;
	pet         = get_char_room( ch, arg );
	ch->in_room = in_room;

	if ( pet == NULL || !IS_SET(pet->act, ACT_PET) || !IS_NPC(pet) )
	{
    send_to_char( L"Üzgünüm, buradan satın alamazsın.\n\r", ch );
	    return;
	}

	if (IS_SET(pet->act,ACT_RIDEABLE)
		&& ch->cabal == CABAL_KNIGHT
		&& !MOUNTED(ch) )
	{
 	 cost = 10 * pet->level * pet->level;

	 if ( ch->silver < cost )
	 {
     send_to_char( L"Onu satın almaya gücün yetmez.\n\r", ch );
	    return;
	 }

	 if ( ch->level < pet->level + 5)
	 {
	    send_to_char( L"O hayvana hükmedecek güçte değilsin.\n\r", ch );
	    return;
	 }

	 deduct_cost(ch,cost);
	 pet = create_mobile( pet->pIndexData, NULL );
	 pet->comm = COMM_NOTELL|COMM_NOSHOUT|COMM_NOCHANNELS;

	 char_to_room( pet, ch->in_room );
	 do_mount(ch, pet->name);
   send_to_char( L"Bineğin hayırlı olsun.\n\r", ch );
	 act( L"$n binek olarak $N satın aldı.", ch, NULL, pet, TO_ROOM );
	 return;
	}

	if ( ch->pet != NULL )
	{
    send_to_char( L"Zaten bir hayvanın var.\n\r",ch);
	    return;
	}

 	cost = 10 * pet->level * pet->level;

	if ( ch->silver < cost )
	{
    send_to_char( L"Onu satın almaya gücün yetmez.\n\r", ch );
	    return;
	}

	if ( ch->level < pet->level )
	{
	    send_to_char( L"O hayvana hükmedecek güçte değilsin.\n\r", ch );
	    return;
	}

	/* haggle */
	roll = number_percent();
	if (roll < get_skill(ch,gsn_haggle))
	{
	    cost -= cost / 2 * roll / 100;
      swprintf( buf, MAX_STRING_LENGTH-1, L"pazarlık ederek fiyatı %d sikkeye çekiyorsun.\n\r",cost);
	    send_to_char(buf,ch);
	    check_improve(ch,gsn_haggle,TRUE,4);

	}

	deduct_cost(ch,cost);
	pet			= create_mobile( pet->pIndexData , NULL);
	SET_BIT(pet->act, ACT_PET);
	SET_BIT(pet->affected_by, AFF_CHARM);
	pet->comm = COMM_NOTELL|COMM_NOSHOUT|COMM_NOCHANNELS;

	argument = one_argument( argument, arg );
	if ( arg[0] != '\0' )
	{
	    swprintf( buf, MAX_STRING_LENGTH-1, L"%s %s", pet->name, arg );
	    free_string( pet->name );
	    pet->name = str_dup( buf );
	}

  swprintf( buf, MAX_STRING_LENGTH-1, L"%sin tasması diyor ki 'Ben %s'e aitim'.\n\r",pet->description, ch->name );
	free_string( pet->description );
	pet->description = str_dup( buf );

	char_to_room( pet, ch->in_room );
	add_follower( pet, ch );
	pet->leader = ch;
	ch->pet = pet;
  send_to_char( L"Hayvanın hayırlı olsun.\n\r", ch );
	act( L"$n $N satın aldı.", ch, NULL, pet, TO_ROOM );
	return;
    }
    else
    {
	CHAR_DATA *keeper;
	OBJ_DATA *obj,*t_obj;
	wchar_t arg[MAX_INPUT_LENGTH];
	int number, count = 1;

	if ( ( keeper = find_keeper( ch ) ) == NULL )
	    return;

	number = mult_argument(argument,arg);
	if ( number < -1 || number > 100)
	{
    act( L"$n sana anlatıyor 'Gerçekçi Ol!", keeper, NULL, ch, TO_VICT );
	    ch->reply = keeper;
	    return;
	}

	obj  = get_obj_keeper( ch,keeper, arg );
	cost = get_cost( keeper, obj, TRUE );

	if ( cost <= 0 || !can_see_obj( ch, obj ) )
	{
    act( L"$n 'Ondan satmıyorum, 'liste'yi dene' dedi.",
  keeper, NULL, ch, TO_VICT );
	    ch->reply = keeper;
	    return;
	}

	if (!IS_OBJ_STAT(obj,ITEM_INVENTORY))
	{
	    for (t_obj = obj->next_content;
	     	 count < number && t_obj != NULL;
	     	 t_obj = t_obj->next_content)
	    {
	    	if (t_obj->pIndexData == obj->pIndexData
	    	&&  !wcscasecmp(t_obj->short_descr,obj->short_descr))
		    count++;
	    	else
		    break;
	    }

	    if (count < number)
	    {
        act( L"$n 'Stoğumda o kadar yok' dedi.",
		    keeper,NULL,ch,TO_VICT);
	    	ch->reply = keeper;
	    	return;
	    }
	}
	else if (obj->pIndexData->limit != -1)
	{
	    count = 1 + obj->pIndexData->limit - obj->pIndexData->count;
	    if (count < 1)
	    {
        act( L"$n anlatıyor 'Tanrılar onu satmamı onaylamaz.'",
		    keeper,NULL,ch,TO_VICT);
	    	ch->reply = keeper;
	    	return;
	    }
	    if (count < number)
	    {
        act( L"$n 'Stoğumda o kadar yok' dedi.",
		    keeper,NULL,ch,TO_VICT);
	    	ch->reply = keeper;
	    	return;
	    }
      if( !limit_kontrol(ch,obj) )
      {
  			return;
      }
	}

  if ( (ch->silver) < cost * number )
	{
	    if (number > 1)
		act( L"$n anlatıyor 'Bu kadar çok alacak paran yok.",keeper,obj,ch,TO_VICT);
	    else
	    	act( L"$n anlatıyor '$p satın alacak paran yok'.",keeper, obj, ch, TO_VICT );
	    ch->reply = keeper;
	    return;
	}

	if ( obj->level > ch->level )
	{
    act( L"$n anlatıyor 'Henüz $p kullanamazsın'.",
		keeper, obj, ch, TO_VICT );
	    ch->reply = keeper;
	    return;
	}

	if (ch->carry_number +  number * get_obj_number(obj) > can_carry_n(ch))
	{
    send_to_char( L"Bu kadar çok şeyi taşıyamazsın.\n\r", ch );
	    return;
	}

	if ( ch->carry_weight + number * get_obj_weight(obj) > can_carry_w(ch))
	{
    send_to_char( L"Bu kadar ağırlığı taşıyamazsın.\n\r", ch );
	    return;
	}

	/* haggle */
	roll = number_percent();
	if (!IS_OBJ_STAT(obj,ITEM_SELL_EXTRACT)
	&& roll < get_skill(ch,gsn_haggle))
	{
	    cost -= obj->cost / 2 * roll / 100;
      act( L"$N ile pazarlık ediyorsun.",ch,NULL,keeper,TO_CHAR);
	    check_improve(ch,gsn_haggle,TRUE,4);
	}

	if (number > 1)
	{
    swprintf( buf, MAX_STRING_LENGTH-1, L"$n $p[%d] satın alıyor.",number);
    act(buf,ch,obj,NULL,TO_ROOM);
    swprintf( buf, MAX_STRING_LENGTH-1, L"%d akçeye $p[%d] satın alıyorsun.",cost * number,number);
    act(buf,ch,obj,NULL,TO_CHAR);
	}
	else
	{
    act( L"$n $p satın alıyor.", ch, obj, NULL, TO_ROOM );
    swprintf( buf, MAX_STRING_LENGTH-1, L"%d akçeye $p satın alıyorsun.",cost);
	    act( buf, ch, obj, NULL, TO_CHAR );
	}
	deduct_cost(ch,cost * number);
	keeper->silver += cost * number;

	for (count = 0; count < number; count++)
	{
	    if ( IS_SET( obj->extra_flags, ITEM_INVENTORY ) )
	    	t_obj = create_object( obj->pIndexData, obj->level );
	    else
	    {
		t_obj = obj;
		obj = obj->next_content;
	    	obj_from_char( t_obj );
	    }

	    if (t_obj->timer > 0 && !IS_OBJ_STAT(t_obj,ITEM_HAD_TIMER))
	    	t_obj->timer = 0;
	    REMOVE_BIT(t_obj->extra_flags,ITEM_HAD_TIMER);
	    obj_to_char( t_obj, ch );
	    if (cost < t_obj->cost)
	    	t_obj->cost = cost;
	}
    }
}



void do_list( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t buf[MAX_STRING_LENGTH];

    if ( IS_SET(ch->in_room->room_flags, ROOM_PET_SHOP) )
    {
	ROOM_INDEX_DATA *pRoomIndexNext;
	CHAR_DATA *pet;
	bool found;

        /* hack to make new thalos pets work */

        if (ch->in_room->vnum == 9621)
            pRoomIndexNext = get_room_index(9706);
        else
            pRoomIndexNext = get_room_index( ch->in_room->vnum + 1 );

	if ( pRoomIndexNext == NULL )
	{
	    bug( L"Do_list: bad pet shop at vnum %d.", ch->in_room->vnum );
	    send_to_char( L"Burada yapamazsın.\n\r", ch );
	    return;
	}

	found = FALSE;
	for ( pet = pRoomIndexNext->people; pet; pet = pet->next_in_room )
	{
	    if (!IS_NPC(pet)) continue;		/* :) */
	    if ( IS_SET(pet->act, ACT_PET) )
	    {
		if ( !found )
		{
		    found = TRUE;
        send_to_char( L"Satılık hayvanlar:\n\r", ch );
		}
		swprintf( buf, MAX_STRING_LENGTH-1, L"[%2d] %8d - %s\n\r",
		    pet->level,
		    10 * pet->level * pet->level,
		    pet->short_descr );
		send_to_char( buf, ch );
	    }
	}
	if ( !found )
  send_to_char( L"Üzgünüm, elimizde hayvan kalmadı.\n\r", ch );
	return;
    }
    else
    {
	CHAR_DATA *keeper;
	OBJ_DATA *obj;
	int cost,count;
	bool found;
	wchar_t arg[MAX_INPUT_LENGTH];

	if ( ( keeper = find_keeper( ch ) ) == NULL )
	    return;
        one_argument(argument,arg);

	found = FALSE;
	for ( obj = keeper->carrying; obj; obj = obj->next_content )
	{
	    if ( obj->wear_loc == WEAR_NONE
	    &&   can_see_obj( ch, obj )
	    &&   ( cost = get_cost( keeper, obj, TRUE ) ) > 0
	    &&   ( arg[0] == '\0'
 	       ||  is_name(arg,obj->name) ))
	    {
		if ( !found )
		{
		    found = TRUE;
        send_to_char( L"[Sv Fiyat Adt] Eşya\n\r", ch );
		}

		if (IS_OBJ_STAT(obj,ITEM_INVENTORY))
		    swprintf( buf, MAX_STRING_LENGTH-1, L"[%2d %5d -- ] %s%s\n\r",
			obj->level,cost,obj->short_descr,
			(obj->pIndexData->limit != -1) ?  (obj->pIndexData->count > obj->pIndexData->limit) ? " (NOT AVAILABLE NOW)" : " (AVAILABLE)" : "" );
		else
		{
		    count = 1;

		    while (obj->next_content != NULL
		    && obj->pIndexData == obj->next_content->pIndexData
		    && !wcscasecmp(obj->short_descr,
			        obj->next_content->short_descr))
		    {
			obj = obj->next_content;
			count++;
		    }
		    swprintf( buf, MAX_STRING_LENGTH-1, L"[%2d %5d %2d ] %s\n\r",
			obj->level,cost,count,obj->short_descr);
		}
		send_to_char( buf, ch );
	    }
	}

	if ( !found )
  send_to_char( L"Buradan birşey satın alamazsın.\n\r", ch );
	return;
    }
}



void do_sell( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t buf[MAX_STRING_LENGTH];
    wchar_t arg[MAX_INPUT_LENGTH];
    CHAR_DATA *keeper;
    OBJ_DATA *obj;
    int cost,roll;
    int silver;
    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
      send_to_char( L"Ne satacaksın?\n\r", ch );
	return;
    }

    if ( ( keeper = find_keeper( ch ) ) == NULL )
	return;

    if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
    {
      act( L"$n anlatıyor 'Sende ondan yok.'",
	    keeper, NULL, ch, TO_VICT );
	ch->reply = keeper;
	return;
    }

    if ( !can_drop_obj( ch, obj ) )
    {
      send_to_char( L"Ondan kurtulamıyorsun.\n\r", ch );
	return;
    }

    if (!can_see_obj(keeper,obj))
    {
      act( L"$n teklif ettiğin şeyi göremiyor.",keeper,NULL,ch,TO_VICT);
	return;
    }

    if ( ( cost = get_cost( keeper, obj, FALSE ) ) <= 0 )
    {
      act( L"$n $p ile ilgilenmiyor.", keeper, obj, ch, TO_VICT );
	return;
    }
    if ( cost > keeper->silver )
    {
      act( L"$n anlatıyor Üzgünüm '$p'nin ederini ödeyemem.",keeper,obj,ch,TO_VICT);
	return;
    }

    act( L"$n $p satıyor.", ch, obj, NULL, TO_ROOM );
    /* haggle */
    roll = number_percent();
    if (!IS_OBJ_STAT(obj,ITEM_SELL_EXTRACT) && roll < get_skill(ch,gsn_haggle))
    {
        roll = get_skill(ch, gsn_haggle) + number_range(1, 20) - 10;
        send_to_char( L"Esnafla pazarlık ediyorsun.\n\r",ch);
        cost += obj->cost / 2 * roll / 100;
        cost = UMIN(cost,95 * get_cost(keeper,obj,TRUE) / 100);
	cost = UMIN(cost,keeper->silver );
        check_improve(ch,gsn_haggle,TRUE,4);
    }

    silver = cost ;

    swprintf( buf, MAX_STRING_LENGTH-1, L"$p eşyasını %d akçeye satıyorsun.",silver);

    act( buf, ch, obj, NULL, TO_CHAR );
    ch->silver 	 += silver;
    deduct_cost(keeper,cost);
    if ( keeper->silver< 0)
	keeper->silver = 0;

    if ( obj->item_type == ITEM_TRASH || IS_OBJ_STAT(obj,ITEM_SELL_EXTRACT))
    {
	extract_obj( obj );
    }
    else
    {
	obj_from_char( obj );
	if (obj->timer)
	    SET_BIT(obj->extra_flags,ITEM_HAD_TIMER);
	else
	    obj->timer = number_range(50,100);
	obj_to_keeper( obj, keeper );
    }

    return;
}



void do_value( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t buf[MAX_STRING_LENGTH];
    wchar_t arg[MAX_INPUT_LENGTH];
    CHAR_DATA *keeper;
    OBJ_DATA *obj;
    int cost;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( L"Neyin değerini öğreneceksin?\n\r", ch );
	return;
    }

    if ( ( keeper = find_keeper( ch ) ) == NULL )
	return;

    if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
    {
	act( L"$n anlatıyor 'Sende ondan yok.'",
	    keeper, NULL, ch, TO_VICT );
	ch->reply = keeper;
	return;
    }

    if (!can_see_obj(keeper,obj))
    {
        act( L"$n teklif ettiğin şeyi göremiyor.",keeper,NULL,ch,TO_VICT);
        return;
    }

    if ( !can_drop_obj( ch, obj ) )
    {
	send_to_char( L"Ondan kurtulamıyorsun.\n\r", ch );
	return;
    }

    if ( ( cost = get_cost( keeper, obj, FALSE ) ) <= 0 )
    {
	act( L"$n $p ile ilgilenmiyor.", keeper, obj, ch, TO_VICT );
	return;
    }

    swprintf( buf, MAX_STRING_LENGTH-1, L"$n sana anlatıyor '$p için sana %d akçe veririm'.",cost);
    act( buf, keeper, obj, ch, TO_VICT );
    ch->reply = keeper;

    return;
}

void do_wanted( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg1[MAX_INPUT_LENGTH];
    wchar_t arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    if (ch_skill_nok(ch,gsn_wanted))
      return;

    if (!cabal_ok(ch,gsn_wanted))
      return;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
      {
        send_to_char( L"Kullanım: aranıyor <karakter> <E|H>\n\r", ch );
        return;
      }

    victim = get_char_world(ch, arg1);

    if ( (victim == NULL) ||
	!(can_see(ch, victim)) )
      {
        send_to_char( L"Burada değil.\n\r", ch);
        return;
      }

    if (victim->level >= LEVEL_IMMORTAL && (ch->level < victim->level))
      {
	act( L"$M tutulayacak gücün yok.", ch, NULL, victim,
	    TO_CHAR);
	return;
      }

    if (victim == ch)
      {
	send_to_char( L"Bunu kendine yapamazsın.\n\r", ch);
	return;
      }

    switch(arg2[0])
      {
      case 'E':
      case 'e':
	if (IS_SET(victim->act ,PLR_WANTED))
	  act( L"$n zaten aranıyor.", ch, NULL, NULL, TO_CHAR);
	else
	  {
	    SET_BIT(victim->act, PLR_WANTED);
	    act( L"$n artık ARANIYOR!!!",victim, NULL, ch, TO_NOTVICT);
	    send_to_char( L"Artık ARANIYORsun!!!\n\r", victim);
	    send_to_char( L"Tamam.\n\r", ch);
	  }
	break;
      case 'H':
      case 'h':
	if (!IS_SET(victim->act,PLR_WANTED))
	  act( L"$N aranmıyor.", ch, NULL, victim, TO_CHAR);
	else
	  {
	    REMOVE_BIT(victim->act, PLR_WANTED);
	    act( L"$n artık aranmıyor.",victim, NULL, ch, TO_NOTVICT);
	    send_to_char( L"Artık aranmıyorsun.\n\r", victim);
	    send_to_char( L"Tamam.\n\r", ch);
	  }
	break;
      default:
        send_to_char( L"Kullanım: aranıyor <karakter> <E|H>\n\r", ch );
	break;
      }
  }

void do_herbs(CHAR_DATA *ch, wchar_t *argument)
{
  CHAR_DATA *victim;
  wchar_t arg[MAX_INPUT_LENGTH];

  one_argument(argument,arg);

  if (IS_NPC(ch))
    return;

  if (is_affected(ch,gsn_herbs))
    {
      send_to_char( L"Başka ot bulamıyorsun.\n\r",ch);
      return;
    }

  if (arg[0] == '\0')
    victim = ch;
  else if ( (victim = get_char_room(ch,arg)) == NULL)
    {
      send_to_char( L"Burada değil.\n\r",ch);
      return;
    }
  WAIT_STATE( ch, skill_table[gsn_herbs].beats );

  if (ch->in_room->sector_type != SECT_INSIDE &&
      ch->in_room->sector_type != SECT_CITY &&
      (IS_NPC(ch) || number_percent() < get_skill(ch,gsn_herbs)))
    {
      AFFECT_DATA af;
      af.where  = TO_AFFECTS;
      af.type 	= gsn_herbs;
      af.level 	= ch->level;
      af.duration = 5;
      af.location = APPLY_NONE;
      af.modifier = 0;
      af.bitvector = 0;

      affect_to_char(ch,&af);

      send_to_char( L"Şifalı otlar topluyorsun.\n\r",ch);
      act( L"$n şifalı otlar topluyor.",ch,NULL,NULL,TO_ROOM);

      if (ch != victim)
	{
	  act( L"$n sana yemen için ot veriyor.",ch,NULL,victim,TO_VICT);
	  act( L"$E yemesi için ot veriyorsun.",ch,NULL,victim,TO_CHAR);
	  act( L"$n $E yemesi için ot veriyor.",ch,NULL,victim,TO_NOTVICT);
	}

      if (victim->hit < victim->max_hit)
	{
	  send_to_char( L"Daha iyi hissediyorsun.\n\r",victim);
	  act( L"$n daha iyi görünüyor.",victim,NULL,NULL,TO_ROOM);
	}
      victim->hit = UMIN(victim->max_hit,victim->hit + 5 * ch->level);
      check_improve(ch,gsn_herbs,TRUE,1);
      if (is_affected(victim, gsn_plague))
	{
	  if (check_dispel(ch->level,victim,gsn_plague))
	    {
	      send_to_char( L"Yaraların kayboluyor.\n\r",victim);
	      act( L"$n yaralarından kurtulmuş görünüyor.",victim,NULL,NULL,TO_ROOM);
	    }
	}
    }
  else
    {
      send_to_char( L"Şifalı otlar bulmaya çalışıyorsun, ama bulamıyorsun.\n\r",ch);
      act( L"$n şifalı ot arıyor.",ch,NULL,NULL,TO_ROOM);
      check_improve(ch,gsn_herbs,FALSE,1);
    }
}

void do_lore( CHAR_DATA *ch, wchar_t *argument )
{
  wchar_t arg1[MAX_INPUT_LENGTH];
  OBJ_DATA *obj;
  wchar_t buf[MAX_STRING_LENGTH];
  AFFECT_DATA *paf;
  int chance;
  int value0, value1, value2, value3;

  argument = one_argument( argument, arg1 );

  if ( ( obj = get_obj_carry( ch, arg1 ) ) == NULL )
    {
      send_to_char( L"Bu eşya sende yok.\n\r", ch );
      return;
    }

  if (ch->mana < 30)
    {
      send_to_char( L"Yeterli manan yok.\n\r", ch);
      return;
    }

  if ( IS_NPC(ch) || ch->level < skill_table[gsn_lore].skill_level[ch->iclass]
      || get_skill(ch,gsn_lore) < 10)
    {
      send_to_char( L"Eşyanın bilgisi bir an için senden uzaklaşıyor.\n\r", ch);
      return;
    }

  /* a random lore */
  chance = number_percent();

  if (get_skill(ch,gsn_lore) < 20)
    {
      swprintf( buf, MAX_STRING_LENGTH-1, L"Eşya '%s'.\n\r", obj->name);
      send_to_char(buf, ch);
      ch->mana -= 30;
      check_improve(ch,gsn_lore,TRUE,8);
      return;
    }

  else if (get_skill(ch,gsn_lore) < 40)
    {
      swprintf( buf, MAX_STRING_LENGTH-1, L"Eşya '%s'.  Ağırlığı %d, değeri %d.\n\r",
	      obj->name,
	      chance < 60 ? obj->weight : number_range(1, 2 * obj->weight),
	      chance < 60 ? number_range(1, 2 * obj->cost) : obj->cost
	      );
      send_to_char(buf, ch);
      if ( wcscasecmp( obj->material, L"oldstyle" ) )  {
        swprintf( buf, MAX_STRING_LENGTH-1, L"Materyali %s.\n\r", obj->material );
        send_to_char(buf, ch);
      }
      ch->mana -= 30;
      check_improve(ch,gsn_lore,TRUE,7);
      return;
    }

  else if (get_skill(ch,gsn_lore) < 60)
    {
      swprintf( buf, MAX_STRING_LENGTH-1, L"Obje '%s', ağırlığı %d.\n\rDeğeri %d, seviyesi %d.\n\rMateryali %s.\n\r",
	      obj->name,
	      obj->weight,
	      chance < 60 ? number_range(1, 2 * obj->cost) : obj->cost,
	      chance < 60 ? obj->level : number_range(1, 2 * obj->level),
	  wcscasecmp(obj->material,L"oldstyle")?obj->material:L"bilinmiyor"
	      );
      send_to_char(buf, ch);
      ch->mana -= 30;
      check_improve(ch,gsn_lore,TRUE,6);
      return;
    }

  else if (get_skill(ch,gsn_lore) < 80)
    {
      swprintf( buf, MAX_STRING_LENGTH-1, L"Obje '%s', tipi %s, ekstra özellikleri %s.\n\rAğırlığı %d, değeri %d, seviyesi %d.\n\rMateryali %s.\n\r",
	      obj->name,
	      item_type_name( obj ),
	      extra_bit_name( obj->extra_flags ),
	      obj->weight,
	      chance < 60 ? number_range(1, 2 * obj->cost) : obj->cost,
	      chance < 60 ? obj->level : number_range(1, 2 * obj->level),
	  wcscasecmp(obj->material,L"oldstyle")?obj->material:L"bilinmiyor"
	      );
      send_to_char(buf, ch);
      ch->mana -= 30;
      check_improve(ch,gsn_lore,TRUE,5);
      return;
    }

  else if (get_skill(ch,gsn_lore) < 85)
    {
      swprintf( buf, MAX_STRING_LENGTH-1, L"Obje '%s', tipi %s, ekstra özellikleri %s.\n\rAğırlığı %d, değeri %d, seviyesi %d.\n\rMateryali %s.\n\r",
	      obj->name,
	      item_type_name( obj ),
	      extra_bit_name( obj->extra_flags ),
	      obj->weight,
	      obj->cost,
	      obj->level,
	  wcscasecmp(obj->material,L"oldstyle")?obj->material:L"bilinmiyor"
	      );
      send_to_char(buf, ch);
    }
  else
    {
      swprintf( buf, MAX_STRING_LENGTH-1, L"Obje '%s', tipi %s, ekstra özellikleri %s.\n\rAğırlığı %d, değeri %d, seviyesi %d.\n\rMateryali %s.\n\r",
	      obj->name,
	      item_type_name( obj ),
	      extra_bit_name( obj->extra_flags ),
	      obj->weight,
	      obj->cost,
	      obj->level,
	  wcscasecmp(obj->material,L"oldstyle")?obj->material:L"bilinmiyor"
	      );
      send_to_char(buf, ch);
    }

  ch->mana -= 30;

  value0 = obj->value[0];
  value1 = obj->value[1];
  value2 = obj->value[2];
  value3 = obj->value[3];

  switch ( obj->item_type )
    {
    case ITEM_SCROLL:
    case ITEM_POTION:
    case ITEM_PILL:
      if (get_skill(ch,gsn_lore) < 85)
	{
	  value0 = number_range(1, 60);
	  if (chance > 40) {
	    value1 = number_range(1, (MAX_SKILL - 1));
	    if (chance > 60) {
	      value2 = number_range(1, (MAX_SKILL - 1));
	      if (chance > 80)
		value3 = number_range(1, (MAX_SKILL - 1));
	    }
	  }
	}
      else
	{
	  if (chance > 60) {
	    value1 = number_range(1, (MAX_SKILL - 1));
	    if (chance > 80) {
	      value2 = number_range(1, (MAX_SKILL - 1));
	      if (chance > 95)
		value3 = number_range(1, (MAX_SKILL - 1));
	    }
	  }
	}

      swprintf( buf, MAX_STRING_LENGTH-1, L"Seviye %d büyüleri:", obj->value[0] );
      send_to_char(buf, ch);

      if ( value1 >= 0 && value1 < MAX_SKILL )
	{
	  send_to_char( L" '", ch );
	  send_to_char( skill_table[value1].name[1], ch );
	  send_to_char( L"'", ch );
	}

      if ( value2 >= 0 && value2 < MAX_SKILL )
	{
	  send_to_char( L" '", ch );
	  send_to_char( skill_table[value2].name[1], ch );
	  send_to_char( L"'", ch );
	}

      if ( value3 >= 0 && value3 < MAX_SKILL )
	{
	  send_to_char( L" '", ch );
	  send_to_char( skill_table[value3].name[1], ch );
	  send_to_char( L"'", ch );
	}

      send_to_char( L".\n\r", ch );
      break;

    case ITEM_WAND:
    case ITEM_STAFF:
      if (get_skill(ch,gsn_lore) < 85)
	{
	  value0 = number_range(1, 60);
	  if (chance > 40) {
	    value3 = number_range(1, (MAX_SKILL - 1));
	    if (chance > 60) {
	      value2 = number_range(0, 2 * obj->value[2]);
	      if (chance > 80)
		value1 = number_range(0, value2);
	    }
	  }
	}
      else
	{
	  if (chance > 60) {
	    value3 = number_range(1, (MAX_SKILL - 1));
	    if (chance > 80) {
	      value2 = number_range(0, 2 * obj->value[2]);
	      if (chance > 95)
		value1 = number_range(0, value2);
	    }
	  }
	}

      swprintf( buf, MAX_STRING_LENGTH-1, L"%d seviyesinde %d(%d) şarjı var",value0, value1, value2 );
      send_to_char(buf, ch);

      if ( value3 >= 0 && value3 < MAX_SKILL )
	  {
	    send_to_char( L" '", ch );
	    send_to_char( skill_table[value3].name[1], ch );
	    send_to_char( L"'", ch );
	  }

      send_to_char( L".\n\r", ch );
      break;

    case ITEM_WEAPON:
      send_to_char( L"Silah tipi ",ch);
      if (get_skill(ch,gsn_lore) < 85)
	{
	  value0 = number_range(0, 8);
	  if (chance > 33) {
	    value1 = number_range(1, 2 * obj->value[1]);
	    if (chance > 66)
	      value2 = number_range(1, 2 * obj->value[2]);
	  }
	}
      else
	{
	  if (chance > 50) {
	    value1 = number_range(1, 2 * obj->value[1]);
	    if (chance > 75)
	      value2 = number_range(1, 2 * obj->value[2]);
	  }
	}

      switch (value0)
	{
	case(WEAPON_EXOTIC) : send_to_char( L"egzotik.\n\r",ch);	break;
	case(WEAPON_SWORD)  : send_to_char( L"kılıç.\n\r",ch);	break;
	case(WEAPON_DAGGER) : send_to_char( L"hançer.\n\r",ch);	break;
	case(WEAPON_SPEAR)	: send_to_char( L"spear/staff.\n\r",ch);	break;
	case(WEAPON_MACE) 	: send_to_char( L"mace/club.\n\r",ch);	break;
	case(WEAPON_AXE)	: send_to_char( L"balta.\n\r",ch);		break;
	case(WEAPON_FLAIL)	: send_to_char( L"flail.\n\r",ch);	break;
	case(WEAPON_WHIP)	: send_to_char( L"kırbaç.\n\r",ch);		break;
	case(WEAPON_POLEARM): send_to_char( L"polearm.\n\r",ch);	break;
	case(WEAPON_BOW)	: send_to_char( L"yay.\n\r",ch);	break;
	case(WEAPON_ARROW)	: send_to_char( L"ok.\n\r",ch);	break;
	case(WEAPON_LANCE)	: send_to_char( L"lance.\n\r",ch);	break;
	default		: send_to_char( L"bilinmiyor.\n\r",ch);	break;
 	}
      if (obj->pIndexData->new_format)
		{
	swprintf( buf, MAX_STRING_LENGTH-1, L"Zarar %dd%d (ortalama %d).\n\r",
		value1,value2,
		(1 + value2) * value1 / 2);
		}
      else
		{
	swprintf( buf, MAX_STRING_LENGTH-1, L"Zarar %d - %d (ortalama %d).\n\r",
	    	value1, value2,
	    	( value1 + value2 ) / 2 );
		}
      send_to_char(buf, ch);
      break;

    case ITEM_ARMOR:
      if (get_skill(ch,gsn_lore) < 85)
	{
	  if (chance > 25) {
	    value2 = number_range(0, 2 * obj->value[2]);
	      if (chance > 45) {
		value0 = number_range(0, 2 * obj->value[0]);
		  if (chance > 65) {
		    value3 = number_range(0, 2 * obj->value[3]);
		      if (chance > 85)
			value1 = number_range(0, 2 * obj->value[1]);
		  }
	      }
	  }
	}
      else
	{
	  if (chance > 45) {
	    value2 = number_range(0, 2 * obj->value[2]);
	      if (chance > 65) {
		value0 = number_range(0, 2 * obj->value[0]);
		  if (chance > 85) {
		    value3 = number_range(0, 2 * obj->value[3]);
		      if (chance > 95)
			value1 = number_range(0, 2 * obj->value[1]);
		  }
	      }
	  }
	}

      swprintf( buf, MAX_STRING_LENGTH-1, L"Zırh sınıfı: delici %d, omuz %d, kesme %d ve büyü  türleri %d.\n\r",
	      value0, value1, value2, value3 );
      send_to_char(buf, ch);
      break;
    }

  if (get_skill(ch,gsn_lore) < 87)
    check_improve(ch,gsn_lore,TRUE,5);
    return;

  if (!obj->enchanted)
    for ( paf = obj->pIndexData->affected; paf != NULL; paf = paf->next )
      {
	if ( paf->location != APPLY_NONE && paf->modifier != 0 )
	  {
	    swprintf( buf, MAX_STRING_LENGTH-1, L"%s özelliğini %d etkiler.\n\r",
		    affect_loc_name( paf->location ), paf->modifier );
	    send_to_char(buf, ch);
	  }
      }

  for ( paf = obj->affected; paf != NULL; paf = paf->next )
    {
      if ( paf->location != APPLY_NONE && paf->modifier != 0 )
	{
	  swprintf( buf, MAX_STRING_LENGTH-1, L"%s özelliğini %d etkiler.\n\r",
		  affect_loc_name( paf->location ), paf->modifier );
	  send_to_char(buf, ch);
	}
    }
  check_improve(ch,gsn_lore,TRUE,5);
  return;
}



void do_butcher(CHAR_DATA *ch, wchar_t *argument)
{
  OBJ_DATA *obj;
  wchar_t buf[MAX_STRING_LENGTH];
  wchar_t arg[MAX_STRING_LENGTH];
  OBJ_DATA *tmp_obj;
  OBJ_DATA *tmp_next;

  if (IS_NPC(ch))
    return;

  one_argument(argument,arg);
  if ( arg[0]=='\0' )
  {
    send_to_char( L"Neyi doğrayacaksın?\n\r", ch );
    return;
  }
  if ( (obj = get_obj_here(ch,arg)) == NULL )
    {
      send_to_char( L"Onu göremiyorsun.\n\r",ch);
      return;
    }

  if (obj->item_type != ITEM_CORPSE_PC && obj->item_type != ITEM_CORPSE_NPC)
    {
      send_to_char( L"Onu doğrayamazsın.\n\r",ch);
      return;
    }

  if (obj->carried_by != NULL)
    {
      send_to_char( L"Önce onu yere bırak.\n\r",ch);
      return;
    }

  if ( !IS_NPC(ch) && get_skill(ch,gsn_butcher) < 1)
    {
      send_to_char( L"Bunun için gerekli araç gerecin yok.", ch);
      return;
    }

  obj_from_room(obj);

  for (tmp_obj = obj->contains;tmp_obj != NULL;
       tmp_obj = tmp_next)
    {
      tmp_next = tmp_obj->next_content;
      obj_from_obj(tmp_obj);
      obj_to_room(tmp_obj,ch->in_room);
    }


  if (IS_NPC(ch) || number_percent() < get_skill(ch,gsn_butcher))
    {
      int numsteaks;
      int i;
      OBJ_DATA *steak;

      numsteaks = number_range(0,3) + 1;

      if (numsteaks > 1)
	{
	  swprintf( buf, MAX_STRING_LENGTH-1, L"$n $p'yi doğruyor ve ondan %i biftek çıkartıyor.",numsteaks);
	  act(buf,ch,obj,NULL,TO_ROOM);

	  swprintf( buf, MAX_STRING_LENGTH-1, L"$p'yi doğruyor ve ondan %i biftek çıkartıyorsun.",numsteaks);
	  act(buf,ch,obj,NULL,TO_CHAR);
	}

      else
	{
	  act( L"$n $p'yi doğruyor ve ondan bir dilim biftek çıkartıyor."
	      ,ch,obj,NULL,TO_ROOM);

	  act( L"$p'yi doğruyor ve ondan bir dilim biftek çıkartıyorsun."
	      ,ch,obj,NULL,TO_CHAR);
	}
      check_improve(ch,gsn_butcher,TRUE,1);

      for (i=0; i < numsteaks; i++)
	{
	  steak = create_object(get_obj_index(OBJ_VNUM_STEAK),0);
	  swprintf( buf, MAX_STRING_LENGTH-1, steak->short_descr, obj->short_descr);
	  free_string( steak->short_descr );
	  steak->short_descr = str_dup( buf );

	  swprintf( buf, MAX_STRING_LENGTH-1, steak->description, obj->short_descr );
	  free_string( steak->description );
	  steak->description = str_dup( buf );

	  obj_to_room(steak,ch->in_room);
	}
    }
  else
    {
      act( L"Başaramadın ve $p'yi yokettin.",ch,obj,NULL,TO_CHAR);
      act( L"$n $p'den biftek çıkarmayı beceremedi.",
	  ch,obj,NULL,TO_ROOM);

      check_improve(ch,gsn_butcher,FALSE,1);
    }
  extract_obj(obj);
}


void do_balance(CHAR_DATA *ch, wchar_t *argument)
{

  if (IS_NPC(ch))
    {
      send_to_char( L"Banka hesabın yok.\n\r",ch);
      return;
    }

  if (!IS_SET(ch->in_room->room_flags, ROOM_BANK))
    {
      send_to_char( L"Bankada değilsin.\n\r",ch);
      return;
    }


  if ( ch->pcdata->bank_s == 0 )  {
    send_to_char( L"Bankada paran yok.\n\r", ch );
    return;
  }

  printf_to_char(ch,L"Bankada %ld akçen var.\n\r", ch->pcdata->bank_s);

}

void do_withdraw(CHAR_DATA *ch, wchar_t *argument)
{
  long  amount_s;
  wchar_t arg[MAX_INPUT_LENGTH];
  int weight;

  if (IS_NPC(ch))
    {
      send_to_char( L"Banka hesabın yok.\n\r",ch);
      return;
    }

  if (!IS_SET(ch->in_room->room_flags, ROOM_BANK))
    {
      send_to_char( L"Bankada değilsin.\n\r",ch);
      return;
    }

  argument = one_argument( argument, arg );
  if ( arg[0] == '\0' )  {
    send_to_char( L"Ne kadar para çekeceksin?\n\r", ch );
    return;
  }

  amount_s = labs (wcstol(arg,0,10));

  if ( amount_s > ch->pcdata->bank_s)
  {
    send_to_char( L"Üzgünüm, borç vermiyoruz.\n\r",ch);
      return;
  }

  weight = amount_s / 10;

  if ( get_carry_weight(ch) + weight > can_carry_w(ch) )
  {
    act( L"Bu ağırlığı taşıyamazsın.", ch,NULL,NULL, TO_CHAR);
     return;
  }

  ch->pcdata->bank_s -= amount_s;
  ch->silver += (long)(0.90 * amount_s);

  printf_to_char(ch,L"İşte %ld akçe, hesap işlemi olarak %ld sikkeni alıyorum.\n\r",amount_s,UMAX(1, (0.10 * amount_s)) );
  act( L"$n vezneye yaklaşıyor.",ch,NULL,NULL,TO_ROOM);
}

void do_deposit(CHAR_DATA *ch, wchar_t *argument)
{
  long amount_s;
  wchar_t buf[100];
  wchar_t arg[200];

  if (IS_NPC(ch))
    {
      send_to_char( L"Banka hesabın yok.\n\r",ch);
      return;
    }

  if (!IS_SET(ch->in_room->room_flags, ROOM_BANK))
    {
      send_to_char( L"Bankada değilsin.\n\r",ch);
      return;
    }

  argument = one_argument( argument, arg );
  if ( arg[0] == '\0' )  {
    send_to_char( L"Ne kadar yatıracaksın?\n\r", ch );
    return;
  }
  amount_s = labs (wcstol(arg,0,10));

  if (amount_s > ch->silver)
    {
      send_to_char( L"Bu sahip olduğundan fazla.\n\r",ch);
      return;
    }

  if ( (amount_s + ch->pcdata->bank_s) > 400000 )
    {
      send_to_char( L"Bankamız 400,000 akçeden fazlasını kabul etmez.\n\r",ch);
      return;
    }

  ch->pcdata->bank_s += amount_s;
  ch->silver -= amount_s;

  if (amount_s == 1)
	{
    swprintf( buf, MAX_STRING_LENGTH-1, L"Şuna bak! Bir sikkeymiş!\n\r");
	}
  else
	{
	  swprintf( buf, MAX_STRING_LENGTH-1, L"%ld akçe hesabına geçti. Yine beklerim!\n\r",amount_s);
	}

  send_to_char(buf, ch);
  act( L"$n vezneye yaklaşıyor.",ch,NULL,NULL,TO_ROOM);
}



void do_enchant(CHAR_DATA *ch, wchar_t *argument)
{
    OBJ_DATA *obj;
    wchar_t buf[MAX_STRING_LENGTH];
    int wear_level;

    if ( !IS_NPC(ch)
    &&   ch->level < skill_table[gsn_enchant_sword].skill_level[ch->iclass] )
    {
	send_to_char( L"Hı?.\n\r", ch);
	return;
    }

    if (argument[0] == '\0') /* empty */
    {
        send_to_char ( L"Hangi silahı?\n\r",ch);
        return;
    }

    obj = get_obj_carry (ch, argument);

    if (obj == NULL)
    {
        send_to_char ( L"Ondan sende yok.\n\r",ch);
        return;
    }


    wear_level = ch->level;

    if ( (class_table[ch->iclass].fMana && obj->item_type == ITEM_ARMOR)
	|| (!class_table[ch->iclass].fMana && obj->item_type == ITEM_WEAPON))
		wear_level += 3;

    if ( wear_level < obj->level )
    {
  swprintf( buf, MAX_STRING_LENGTH-1, L"Onu yükseltmek için %d. seviye olmalısın.\n\r",
            obj->level );
        send_to_char(buf, ch);
        act( L"$n $p eşyasını yükseltmeye çalışıyor, ama yeterince tecrübeli değil.",
            ch, obj, NULL, TO_ROOM );
        return;
    }

   if (ch->mana < 100 )
	{
	 send_to_char( L"Yeterli manan yok.\n\r",ch);
	 return;
	}

   if ( number_percent() > get_skill(ch,gsn_enchant_sword) )
	{
	 send_to_char( L"Konsantrasyonunu yitirdin.\n\r",ch);
        act( L"$n $p eşyasını yükseltmek istiyor, ama bir an için nasıl yapılacağını unutuyor.",
            ch, obj, NULL, TO_ROOM );
	WAIT_STATE(ch,skill_table[gsn_enchant_sword].beats);
	check_improve(ch,gsn_enchant_sword,FALSE,6);
	ch->mana -= 50;
	 return;
	}
    ch->mana -= 100;
    spell_enchant_weapon(24,ch->level,ch,obj,TARGET_OBJ);
    check_improve(ch,gsn_enchant_sword,TRUE,2);
    WAIT_STATE(ch,skill_table[gsn_enchant_sword].beats);
    return;
}




void hold_a_light(CHAR_DATA *ch,OBJ_DATA *obj, int iWear)
{
    act( L"$n $p yakıyor ve tutmaya başlıyor.", ch, obj, NULL, TO_ROOM );
    act( L"$p yakıyor ve tutmaya başlıyorsun.",  ch, obj, NULL, TO_CHAR );
    equip_char( ch, obj, iWear );
}

void hold_a_shield(CHAR_DATA *ch,OBJ_DATA *obj, int iWear)
{
	act( L"$n kalkan olarak $p kullanıyor.", ch, obj, NULL, TO_ROOM );
	act( L"Kalkan olarak $p kullanıyorsun.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, iWear );
}

void hold_a_thing(CHAR_DATA *ch,OBJ_DATA *obj, int iWear)
{
	act( L"$n elinde $p tutuyor.",   ch, obj, NULL, TO_ROOM );
	act( L"Elinde $p tutuyorsun.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, iWear );
}

/* wear object as a secondary weapon */
void hold_a_wield(CHAR_DATA *ch, OBJ_DATA *obj, int iWear)
{
    int sn,skill;

    if (obj == NULL)
    {
        bug( L"Hold_a_wield: Obj NULL",0);
        return;
    }

    if (obj->item_type != ITEM_WEAPON)
    {
	hold_a_thing(ch, obj, iWear);
	return;
    }

    act( L"$n $p kuşanıyor.", ch, obj, NULL, TO_ROOM );
    act( L"$p kuşanıyorsun.", ch, obj, NULL, TO_CHAR );
    equip_char ( ch, obj, iWear);
    if (get_wield_char(ch,TRUE) == obj) sn = get_weapon_sn(ch,TRUE);
    else sn = get_weapon_sn(ch,FALSE);

    if  (sn)
      {
        skill = get_weapon_skill(ch,sn);

        if (skill >= 100)
            act( L"$p senin bir parçan gibi!",ch,obj,NULL,TO_CHAR);
        else if (skill > 85)
            act( L"$p ile kendinden emin gibisin.",ch,obj,NULL,TO_CHAR);
        else if (skill > 70)
            act( L"$p üzerine maharetlisin.",ch,obj,NULL,TO_CHAR);
        else if (skill > 50)
            act( L"$p kullanma becerin yeterli sayılır.",ch,obj,NULL,TO_CHAR);
        else if (skill > 25)
            act( L"$p ellerinde biçimsiz durdu.",ch,obj,NULL,TO_CHAR);
        else if (skill > 1)
            act( L"$p kullanırken ellerin birbirine dolaşacak.",ch,obj,NULL,TO_CHAR);
        else
            act( L"$p'nin ne tarafı tutulur bilmiyorsun.",
                ch,obj,NULL,TO_CHAR);
      }

    return;
}


void wear_a_wield(CHAR_DATA *ch,OBJ_DATA *obj, bool fReplace)
{
	int EL_BIR,EL_IKI;

		if(LEFT_HANDER(ch))
		{
			EL_BIR=WEAR_LEFT;
			EL_IKI=WEAR_RIGHT;
		}
		else
		{
			EL_BIR=WEAR_RIGHT;
			EL_IKI=WEAR_LEFT;
		}

  if ( !IS_NPC(ch)
	&& get_obj_weight(obj) > str_app[get_curr_stat(ch,STAT_STR)].carry )
  {
    send_to_char( L"Kuşanamayacağın kadar ağır.\n\r", ch );
    return;
  }

  if (IS_WEAPON_STAT(obj,WEAPON_TWO_HANDS) &&
         (!IS_NPC(ch) && ch->size < SIZE_LARGE) )
   {
    	if (get_eq_char(ch,WEAR_BOTH) != NULL )
    	{
     		if (!remove_obj_loc(ch,WEAR_BOTH,fReplace)) return;
     		hold_a_wield(ch, obj, WEAR_BOTH);
    	}
    	else
    	{
     		if ( get_eq_char(ch,EL_BIR) )
			if (!remove_obj_loc(ch,EL_BIR,fReplace)) return;
     		if ( get_eq_char(ch,EL_IKI) )
			if (!remove_obj_loc(ch,EL_IKI,fReplace)) return;
     		hold_a_wield(ch, obj, WEAR_BOTH);
    	}
   }
  else
  {
    	if (get_eq_char(ch,WEAR_BOTH) != NULL )
    	{
     		if (!remove_obj_loc( ch, WEAR_BOTH, fReplace ) )
			return;
		hold_a_wield(ch, obj, EL_BIR);
    	}
	else if (get_eq_char(ch,EL_BIR) == NULL )
		hold_a_wield(ch, obj, EL_BIR);
	else if (get_eq_char(ch,EL_IKI) == NULL )
		hold_a_wield(ch, obj, EL_IKI);
	else if ( remove_obj_loc( ch, EL_BIR, fReplace ) )
		hold_a_wield(ch, obj, EL_BIR);
	else if ( remove_obj_loc( ch, EL_IKI, fReplace ) )
		hold_a_wield(ch, obj, EL_IKI);
	else
		send_to_char( L"Ellerin dolu.\n\r",ch);
  }
}



void wear_multi(CHAR_DATA *ch,OBJ_DATA *obj,int iWear,bool fReplace)
{
 if (count_worn(ch, iWear) < max_can_wear(ch, iWear))
  {
   switch( iWear )
   {
    case WEAR_FINGER:
	act( L"$n parmaklarından birine $p takıyor.", ch, obj, NULL, TO_ROOM );
	act( L"Parmaklarından birine $p takıyorsun.", ch, obj, NULL, TO_CHAR );
	break;
    case WEAR_NECK:
	act( L"$n boynuna $p takıyor.",   ch, obj, NULL, TO_ROOM );
	act( L"Boynuna $p takıyorsun.", ch, obj, NULL, TO_CHAR );
	break;
    case WEAR_WRIST:
	act( L"$n bileğine $p takıyor.", ch, obj, NULL, TO_ROOM );
	act( L"Bileğine $p takıyorsun.",ch, obj, NULL, TO_CHAR);
	break;
    case WEAR_TATTOO:
	act( L"$n dini gereği $p dövmesini kazıtıyor.",ch,obj,NULL,TO_ROOM);
	act( L"Dinin gereği $p dövmesi kazıtıyorsun.",ch,obj,NULL,TO_CHAR);
	break;
    default:
	act( L"$n bir yerine $p giyiyor.", ch, obj, NULL, TO_ROOM );
	act( L"Bir yerine $p giyiyorsun.",ch, obj, NULL, TO_CHAR);
	break;
   }
   equip_char(ch, obj, iWear);
  }
  else if (fReplace)
  {
    OBJ_DATA *w;
    int not_worn = 1;

    for ( w = ch->carrying; w != NULL; w = w->next_content )
    {
	if ( w->wear_loc == iWear
		&& !IS_SET(w->extra_flags, ITEM_NOREMOVE)
		&& (w->item_type != ITEM_TATTOO || IS_IMMORTAL(ch) ) )
	{
          unequip_char( ch, w );
          act( L"$n $p kullanmayı bıraktı.", ch, w, NULL, TO_ROOM );
          act( L"$p kullanmayı bıraktın.", ch, w, NULL, TO_CHAR );
	  wear_multi(ch, obj, iWear, fReplace );
	  not_worn = 0;
          break;
	}
    }

    if ( not_worn )
          act( L"$p ile değiştirilebilecek birşey giymiyorsun.",
		ch, obj, NULL, TO_CHAR );
  }

}

bool limit_kontrol (CHAR_DATA *ch, OBJ_DATA *obj)
{
	OBJ_DATA *b_obj,*c_obj;
    int limit_ekipman_sayisi, limit_iksir_sayisi;

	limit_ekipman_sayisi=0;
  limit_iksir_sayisi = 0;

  if(IS_NPC(ch))
  {
    return TRUE;
  }

	for ( b_obj = ch->carrying; b_obj != NULL; b_obj = b_obj->next_content)
	{
		if (b_obj->item_type==ITEM_CONTAINER)
		{
      c_obj = NULL;

			for ( c_obj = b_obj->contains; c_obj != NULL; c_obj = c_obj->next_content )
			{
				if ( c_obj->pIndexData->limit != -1)
				{
          if( c_obj->item_type == ITEM_SCROLL || c_obj->item_type == ITEM_PILL || c_obj->item_type == ITEM_POTION )
          {
            limit_iksir_sayisi++;
            if( limit_iksir_sayisi > MAKSIMUM_LIMIT_IKSIR_HAP_PARSOMEN)
  					{
  						extract_obj( c_obj );
  						limit_iksir_sayisi--;
  					}
          }
          else
          {
  					limit_ekipman_sayisi++;
  					if( !(ch->cabal) && limit_ekipman_sayisi > MAKSIMUM_LIMIT_EKIPMAN)
  					{
  						extract_obj( c_obj );
  						limit_ekipman_sayisi--;
  					}
            else if( (ch->cabal) && limit_ekipman_sayisi > MAKSIMUM_LIMIT_EKIPMAN_KABAL)
  					{
  						extract_obj( c_obj );
  						limit_ekipman_sayisi--;
  					}
          }
				}
			}
		}
		if ( b_obj->pIndexData->limit != -1)
		{
      if( b_obj->item_type == ITEM_SCROLL || b_obj->item_type == ITEM_PILL || b_obj->item_type == ITEM_POTION )
      {
        limit_iksir_sayisi++;
        if( limit_iksir_sayisi > MAKSIMUM_LIMIT_IKSIR_HAP_PARSOMEN)
        {
          extract_obj( b_obj );
          limit_iksir_sayisi--;
        }
      }
      else
      {
  			limit_ekipman_sayisi++;
  			if( !(ch->cabal) && limit_ekipman_sayisi > MAKSIMUM_LIMIT_EKIPMAN)
  			{
  				extract_obj( b_obj );
  				limit_ekipman_sayisi--;
  			}
        else if( (ch->cabal) && limit_ekipman_sayisi > MAKSIMUM_LIMIT_EKIPMAN_KABAL)
        {
          extract_obj( b_obj );
          limit_ekipman_sayisi--;
        }
      }
		}
	}

  if (obj->pIndexData->limit != -1)
  {
    if ((obj->item_type == ITEM_SCROLL) || (obj->item_type == ITEM_PILL) || (obj->item_type == ITEM_POTION))
    {
      if (limit_iksir_sayisi==MAKSIMUM_LIMIT_IKSIR_HAP_PARSOMEN)
      {
        printf_to_char(ch,L"Limit hap/parşömen/iksir kontenjanın dolu!\n\r");
        return FALSE;
      }
    }
    else
    {
      if (ch->cabal)
      {
        if (limit_ekipman_sayisi==MAKSIMUM_LIMIT_EKIPMAN_KABAL)
        {
          printf_to_char(ch,L"Limit ekipman kontenjanın dolu!\n\r");
          return FALSE;
        }
      }
      else
      {
        if (limit_ekipman_sayisi==MAKSIMUM_LIMIT_EKIPMAN)
        {
          printf_to_char(ch,L"Limit ekipman kontenjanın dolu!\n\r");
          return FALSE;
        }
      }
    }
  }

	return TRUE;
}
