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
#include <stdlib.h>
#include <string.h>
#include "merc.h"
#include "recycle.h"

/* command procedures needed */
DECLARE_DO_FUN(do_look		);
DECLARE_DO_FUN(do_recall	);
DECLARE_DO_FUN(do_stand		);
DECLARE_DO_FUN(do_yell		);
void	one_hit		args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ,bool secondary) );
int 	mount_success 	args( ( CHAR_DATA *ch, CHAR_DATA *mount, int canattack) );
int 	find_path	args( ( int in_room_vnum, int out_room_vnum, CHAR_DATA *ch, int depth, int in_zone ) );
int	check_obj_dodge	args( ( CHAR_DATA *ch, CHAR_DATA *victim, OBJ_DATA *obj, int bonus) );
OBJ_DATA *	get_weapon_char	args( ( CHAR_DATA *ch, int wType ) );
bool cabal_area_check   args( (CHAR_DATA *ch) );
bool is_at_cabal_area   args( (CHAR_DATA *ch) );




const wchar_t *	dir_name	[]		=
{
  L"kuzey", L"doğu", L"güney", L"batı", L"yukarı", L"aşağı"
};

const	sh_int	rev_dir		[]		=
{
    2, 3, 0, 1, 5, 4
};

const	sh_int	movement_loss	[SECT_MAX]	=
{
    1, 2, 2, 3, 4, 6, 4, 1, 6, 10, 6
};



/*
 * Local functions.
 */
int	find_door	args( ( CHAR_DATA *ch, wchar_t *arg ) );
int 	find_exit	args( ( CHAR_DATA *ch, wchar_t *arg ) );
bool	has_key		args( ( CHAR_DATA *ch, int key ) );



void move_char( CHAR_DATA *ch, int door, bool follow )
{
    CHAR_DATA *fch;
    CHAR_DATA *fch_next;
    CHAR_DATA *mount;
    ROOM_INDEX_DATA *in_room;
    ROOM_INDEX_DATA *to_room;
    EXIT_DATA *pexit;
    bool room_has_pc;
    OBJ_DATA *obj;
    wchar_t buf[MAX_STRING_LENGTH];

   if (RIDDEN(ch) && !IS_NPC(ch->mount))
   {
    move_char(ch->mount,door,follow);
    return;
   }

   if (CAN_DETECT(ch,ADET_WEB)
	|| (MOUNTED(ch) && CAN_DETECT(ch->mount,ADET_WEB)))
    {
	WAIT_STATE(ch,PULSE_VIOLENCE);
	if ( number_percent() < str_app[IS_NPC(ch) ? 20 : get_curr_stat(ch,STAT_STR)].tohit * 5)
	{
	 affect_strip(ch,gsn_web);
	 REMOVE_BIT(ch->detection,ADET_WEB);
   send_to_char( L"Odayı terketmek istediğinde seni tutan ağları parçalıyorsun.\n\r", ch );
	 act( L"$n kendisini tutan ağlardan kurtulmaya çalışıyor ve onları parçalıyor.", ch, NULL, NULL, TO_ROOM );
	}
	else
	{
    send_to_char( L"Ağlar odadan ayrılmanı engelliyor.\n\r", ch );
 	 act( L"$n kendisini tutan ağlardan kurtulmaya çalışması sonuş vermiyor.", ch, NULL, NULL, TO_ROOM );
	 return;
	}
    }

    if ( door < 0 || door > 5 )
    {
	bug( L"Do_move: bad door %d.", door );
	return;
    }

    if (( IS_AFFECTED( ch, AFF_HIDE ) && !IS_AFFECTED(ch, AFF_SNEAK) )
	|| ( IS_AFFECTED( ch, AFF_FADE ) && !IS_AFFECTED(ch, AFF_SNEAK) )  )
    {
	REMOVE_BIT(ch->affected_by, AFF_HIDE);
	REMOVE_BIT(ch->affected_by, AFF_FADE);
  send_to_char( L"Gölgelerden çıkıyorsun.\n\r", ch );
	act( L"$n gölgelerden çıkıyor.", ch, NULL, NULL, TO_ROOM);
    }
    if ( IS_AFFECTED( ch, AFF_CAMOUFLAGE ) )
    {
	if ( ch->level < skill_table[gsn_move_camf].skill_level[ch->iclass])
	     {
	      REMOVE_BIT(ch->affected_by, AFF_CAMOUFLAGE);
        send_to_char( L"Örtünden sıyrılıyorsun.\n\r", ch );
	      act( L"$n örtüsünden sıyrılıyor.", ch, NULL, NULL,TO_ROOM);
	     }
	else if ( number_percent() < get_skill(ch,gsn_move_camf) )
	           check_improve(ch,gsn_move_camf,TRUE,5);
	else {
	      REMOVE_BIT(ch->affected_by, AFF_CAMOUFLAGE);
        send_to_char( L"Örtünden sıyrılıyorsun.\n\r", ch );
	      act( L"$n örtüsünden sıyrılıyor.", ch, NULL, NULL,TO_ROOM);
	      check_improve(ch,gsn_move_camf,FALSE,5);
	     }
    }
    if ( CAN_DETECT(ch, ADET_EARTHFADE) )
    {
      send_to_char( L"Doğal formuna dönüyorsun.\n\r", ch);
    	act( L"$n şekilleniyor.", ch, NULL, NULL, TO_ROOM);
	affect_strip(ch, gsn_earthfade);
	WAIT_STATE(ch, (PULSE_VIOLENCE / 2) );
	REMOVE_BIT(ch->detection, ADET_EARTHFADE );
    }

    in_room = ch->in_room;
    if ( ( pexit   = in_room->exit[door] ) == NULL
    ||   ( to_room = pexit->u1.to_room   ) == NULL
    ||	 !can_see_room(ch,pexit->u1.to_room))
    {
      send_to_char( L"O yöne gidemezsin.\n\r", ch );
	return;
    }

    if (IS_ROOM_AFFECTED(in_room,AFF_ROOM_RANDOMIZER))
    {
	 int d0;
	 int i;

	 for(i=0; i< 1000; i++)
	 {
	  d0 = number_range(0,5);
          if ( ( pexit   = in_room->exit[d0] ) == NULL
           ||   ( to_room = pexit->u1.to_room   ) == NULL
           ||	 !can_see_room(ch,pexit->u1.to_room))
	   continue;
	  door = d0;
	  break;
	 }
    }

    if (IS_SET(pexit->exit_info, EX_CLOSED)
      && (!IS_AFFECTED(ch, AFF_PASS_DOOR) || IS_SET(pexit->exit_info,EX_NOPASS))
      &&   !IS_TRUSTED(ch,ANGEL) )
    {
	if ( IS_AFFECTED( ch, AFF_PASS_DOOR ) && IS_SET(pexit->exit_info, EX_NOPASS))  {
    act( L"$d içinden geçemedin.", ch, NULL, pexit->keyword, TO_CHAR);
  act( L"$n $d içinden geçmeyi başaramadı.", ch, NULL, pexit->keyword, TO_ROOM);
	}
	else
  act( L"$d kapalı.", ch, NULL, pexit->keyword, TO_CHAR );
        return;
    }

    if ( IS_AFFECTED(ch, AFF_CHARM)
    &&   ch->master != NULL
    &&   in_room == ch->master->in_room )
    {
      send_to_char( L"Pek sevgili efendini terk mi edeceksin?\n\r", ch );
	return;
    }

/*    if ( !is_room_owner(ch,to_room) && room_is_private( to_room ) )	*/
    if ( room_is_private( to_room ) )
    {
      send_to_char( L"Oda şu an özel kullanımda.\n\r", ch );
	return;
    }

    if (IS_SET(to_room->area->area_flag, AREA_CABAL) && !IS_NPC(ch) && IS_SET(ch->act,PLR_GHOST))
    {
      send_to_char( L"Orası bir kabal bölgesi ve hayaletlerin girmesine izin verilmiyor.\n\r", ch );
	return;
    }

    if (IS_SET(to_room->area->area_flag, AREA_CABAL) && !IS_NPC(ch) && (ch->level < 20))
    {
      send_to_char( L"Orası bir kabal bölgesi. 20. seviyeden önce oraya girmesen iyi olur. Tabii senin için...\n\r", ch );
	return;
    }

    if (MOUNTED(ch))
    {
        if (MOUNTED(ch)->position < POS_FIGHTING)
        {
          send_to_char( L"Bineğin ayakta olmalı.\n\r", ch);
         return;
        }
        if (!mount_success(ch, MOUNTED(ch), FALSE))
        {
          send_to_char( L"Bineğin inatla reddediyor.\n\r", ch);
         return;
        }
    }

    if ( !IS_NPC(ch) )
    {
	int iClass, iGuild;
	int move;


	for ( iClass = 0; iClass < MAX_CLASS; iClass++ )
	    for ( iGuild = 0; iGuild < MAX_GUILD; iGuild ++)
	    	if ( to_room->vnum == class_table[iClass].guild[iGuild]
		     && !IS_IMMORTAL(ch) )
	    	{
		    if ( iClass != ch->iclass )  {
          send_to_char( L"İznin yok.\n\r", ch );
		      return;
		    }
                    if (ch->last_fight_time != -1 &&
                      current_time - ch->last_fight_time < FIGHT_DELAY_TIME)
                    {
                      send_to_char( L"Fazlasıyla kan kokuyorsun.\n\r",ch);
                      return;
                    }
	        }

  if ( in_room->sector_type == SECT_AIR
  ||   to_room->sector_type == SECT_AIR )
  {
    if ( MOUNTED(ch) )
    {
      if( !IS_AFFECTED(MOUNTED(ch), AFF_FLYING) )
      {
      send_to_char( L"Bineğin uçamıyor.\n\r", ch );
      return;
      }
    }
    else if ( !IS_AFFECTED(ch, AFF_FLYING) && !IS_IMMORTAL(ch))
    {
      send_to_char( L"Uçamıyorsun.\n\r", ch );
      return;
    }
  }

	if (( in_room->sector_type == SECT_WATER_NOSWIM
	||    to_room->sector_type == SECT_WATER_NOSWIM )
	&&    (MOUNTED(ch) && !IS_AFFECTED(MOUNTED(ch),AFF_FLYING)) )
	{
    send_to_char( L"Bineğini oraya götüremezsin.\n\r",ch);
	    return;
        }

	if (( in_room->sector_type == SECT_WATER_NOSWIM
	||    to_room->sector_type == SECT_WATER_NOSWIM )
  	&&    (!MOUNTED(ch) && !IS_AFFECTED(ch,AFF_FLYING)) )
	{
	    OBJ_DATA *obj;
	    bool found;

	    /*
	     * Look for a boat.
	     */
	    found = FALSE;

	    if (IS_IMMORTAL(ch))
		found = TRUE;

    if (IS_SET(ch->act,PLR_GHOST))
      found = TRUE;

	    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
	    {
		if ( obj->item_type == ITEM_BOAT )
		{
		    found = TRUE;
		    break;
		}
	    }
	    if ( !found )
	    {
        send_to_char( L"Sala ihtiyacın var.\n\r", ch );
		return;
	    }
	}

	move = movement_loss[UMIN(SECT_MAX-1, in_room->sector_type)]
	     + movement_loss[UMIN(SECT_MAX-1, to_room->sector_type)]
	     ;

        move /= 2;  /* i.e. the average */


	/* conditional effects */
	if (IS_AFFECTED(ch,AFF_FLYING) || IS_AFFECTED(ch,AFF_HASTE))
	    move /= 2;

	if (IS_AFFECTED(ch,AFF_SLOW))
	    move *= 2;

	if ( !MOUNTED(ch) && ch->move < move )
	{
    send_to_char( L"Çok yorgunsun.\n\r", ch );
	    return;
	}

	if ( !MOUNTED(ch) &&
	(ch->in_room->sector_type == SECT_DESERT || IS_WATER(ch->in_room)) )
	  WAIT_STATE( ch, 2 );
	else
	  WAIT_STATE( ch, 1 );

	if (!MOUNTED(ch))	ch->move -= move;
    }

    if ( !IS_AFFECTED(ch, AFF_SNEAK) && !IS_AFFECTED(ch, AFF_CAMOUFLAGE)
    &&   ch->invis_level < LEVEL_HERO)
     {
        if (!IS_NPC(ch) && ch->in_room->sector_type != SECT_INSIDE &&
            ch->in_room->sector_type != SECT_CITY &&
            number_percent() < get_skill(ch,gsn_quiet_movement) )
          {
	    if (MOUNTED(ch))
      swprintf( buf, MAX_STRING_LENGTH-1, L"$n %s üzerinde ilerliyor.", MOUNTED(ch)->short_descr );
            else
            swprintf( buf, MAX_STRING_LENGTH-1, L"$n ilerliyor.");
            check_improve(ch,gsn_quiet_movement,TRUE,1);
          }
	else
	  {
           if (MOUNTED(ch))
           swprintf( buf, MAX_STRING_LENGTH-1, L"$n %s üzerinde $T yönünde ilerliyor.",MOUNTED(ch)->short_descr );
  	   else
       swprintf( buf, MAX_STRING_LENGTH-1, L"$n $T yönünde ilerliyor." );
	  }
  	  act( buf, ch, NULL, dir_name[door], TO_ROOM );
     }

    if ( IS_AFFECTED(ch, AFF_CAMOUFLAGE) && to_room->sector_type != SECT_FIELD
	 && to_room->sector_type != SECT_FOREST &&
           to_room->sector_type != SECT_MOUNTAIN &&
           to_room->sector_type != SECT_HILLS )
     {
      REMOVE_BIT(ch->affected_by, AFF_CAMOUFLAGE);
      send_to_char( L"Örtünden sıyrılıyorsun.\n\r", ch );
      act( L"$n örtüsünden sıyrılıyor.", ch, NULL, NULL,TO_ROOM);
     }

    if ( (IS_AFFECTED( ch, AFF_HIDE ) )
	 && to_room->sector_type != SECT_FOREST
         && to_room->sector_type != SECT_FIELD )
    {
	REMOVE_BIT(ch->affected_by, AFF_HIDE);
  send_to_char( L"Gölgelerden çıkıyorsun.\n\r", ch );
	act( L"$n gölgelerden çıkıyor.", ch, NULL, NULL, TO_ROOM);
    }

    mount = MOUNTED(ch);
    char_from_room( ch );
    char_to_room( ch, to_room );

    if (ch->in_room != to_room)
    {
	bug( L"Is char dead!",0);
	return;
    }

    /* room record for tracking */
    if (!IS_NPC(ch) && ch->in_room)
      room_record(ch->name,in_room, door);


    if ( !IS_AFFECTED(ch, AFF_SNEAK)
    &&   ch->invis_level < LEVEL_HERO)
      {
	if (mount)
  act( L"$n $M sürerek geldi.", ch, NULL, mount,TO_ROOM );
else act( L"$n geldi.", ch, NULL, NULL, TO_ROOM );
      }
    do_look( ch, (wchar_t*)"auto" );

    if (mount)
	{
	 char_from_room( mount );
	 char_to_room( mount, to_room);
  	 ch->riding = TRUE;
  	 mount->riding = TRUE;
	}

    if (in_room == to_room) /* no circular follows */
	return;

    for (fch = to_room->people,room_has_pc = FALSE;fch != NULL; fch = fch_next)
      {
        fch_next = fch->next_in_room;
        if (!IS_NPC(fch))
        {
          room_has_pc = TRUE;
          }
      }
    for (fch = to_room->people;fch!=NULL;fch = fch_next) {
      fch_next = fch->next_in_room;

      /* greet progs for items carried by people in room */
      for (obj = fch->carrying;room_has_pc && obj != NULL;
           obj = obj->next_content)
        {
          if (IS_SET(obj->progtypes,OPROG_GREET))
            (obj->pIndexData->oprogs->greet_prog) (obj,ch);
        }
      /* greet programs for npcs  */
      if (room_has_pc && IS_SET(fch->progtypes,MPROG_GREET))
      {
        (fch->pIndexData->mprogs->greet_prog) (fch,ch);
      }
    }

    /* entry programs for items */
    for (obj = ch->carrying;room_has_pc && obj!=NULL;obj=obj->next_content)
      {
        if (IS_SET(obj->progtypes,OPROG_ENTRY))
          (obj->pIndexData->oprogs->entry_prog) (obj);
      }


    for ( fch = in_room->people; fch != NULL; fch = fch_next )
    {
	fch_next = fch->next_in_room;

	if ( fch->master == ch && IS_AFFECTED(fch,AFF_CHARM)
	&&   fch->position < POS_STANDING)
	    do_stand(fch,(wchar_t*)"");

	if ( fch->master == ch && fch->position == POS_STANDING
	&&   can_see_room(fch,to_room))
	{

	    if (IS_SET(ch->in_room->room_flags,ROOM_LAW)
	    &&  (IS_NPC(fch) && IS_SET(fch->act,ACT_AGGRESSIVE)))
	    {
        act( L"$M şehre sokamazsın.",
    		    ch,NULL,fch,TO_CHAR);
    		act( L"Şehre girmek için iznin yok.",
    		    fch,NULL,NULL,TO_CHAR);
		continue;
	    }

      act( L"$M takip ediyorsun.", fch, NULL, ch, TO_CHAR );
	    move_char( fch, door, TRUE );
	}
    }

    for (obj = ch->in_room->contents;room_has_pc && obj != NULL;
         obj = obj->next_content)
      {
        if (IS_SET(obj->progtypes,OPROG_GREET))
          (obj->pIndexData->oprogs->greet_prog) (obj,ch);
      }

    if ( IS_SET(ch->progtypes,MPROG_ENTRY))
      (ch->pIndexData->mprogs->entry_prog) (ch);

    return;
}



void do_north( CHAR_DATA *ch, wchar_t *argument )
{
    move_char( ch, DIR_NORTH, FALSE );
    return;
}



void do_east( CHAR_DATA *ch, wchar_t *argument )
{
    move_char( ch, DIR_EAST, FALSE );
    return;
}



void do_south( CHAR_DATA *ch, wchar_t *argument )
{
    move_char( ch, DIR_SOUTH, FALSE );
    return;
}



void do_west( CHAR_DATA *ch, wchar_t *argument )
{
    move_char( ch, DIR_WEST, FALSE );
    return;
}



void do_up( CHAR_DATA *ch, wchar_t *argument )
{
    move_char( ch, DIR_UP, FALSE );
    return;
}



void do_down( CHAR_DATA *ch, wchar_t *argument )
{
    move_char( ch, DIR_DOWN, FALSE );
    return;
}

void do_run( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg1[MAX_INPUT_LENGTH];
    wchar_t arg2[MAX_INPUT_LENGTH];
    int door,count;

    argument = one_argument( argument, arg1 );

    if ( arg1[0] == '\0' || arg2[0] == '\0')
    {
      send_to_char( L"koş <yön> <miktar>\n\r", ch);
	return;
    }

    if ( ( door = find_exit(ch,arg1) ) < -1) return;
    if ( ( count = wcstol( arg2, 0, 10 ) ) < 1 )
    {
      send_to_char( L"Sayı sıfırdan büyük olmalı.\n\r", ch);
	return;
    }

    return;
}



int find_exit( CHAR_DATA *ch, wchar_t *arg )
{
    int door;

    if ( !wcscasecmp( arg, L"k" ) || !wcscasecmp( arg, L"kuzey" ) ) door = 0;
     else if ( !wcscasecmp( arg, L"d" ) || !wcscasecmp( arg, L"doğu"  ) ) door = 1;
     else if ( !wcscasecmp( arg, L"g" ) || !wcscasecmp( arg, L"güney" ) ) door = 2;
     else if ( !wcscasecmp( arg, L"b" ) || !wcscasecmp( arg, L"batı"  ) ) door = 3;
     else if ( !wcscasecmp( arg, L"y" ) || !wcscasecmp( arg, L"yukarı"    ) ) door = 4;
     else if ( !wcscasecmp( arg, L"a" ) || !wcscasecmp( arg, L"aşağı"  ) ) door = 5;
     else
     {
 	act( L"$T yönünde çıkış yok.", ch, NULL, arg, TO_CHAR );
 	return -1;
     }

    return door;
}


int find_door( CHAR_DATA *ch, wchar_t *arg )
{
    EXIT_DATA *pexit;
    int door;

    if ( !wcscasecmp( arg, L"k" ) || !wcscasecmp( arg, L"kuzey" ) ) door = 0;
     else if ( !wcscasecmp( arg, L"d" ) || !wcscasecmp( arg, L"doğu"  ) ) door = 1;
     else if ( !wcscasecmp( arg, L"g" ) || !wcscasecmp( arg, L"güney" ) ) door = 2;
     else if ( !wcscasecmp( arg, L"b" ) || !wcscasecmp( arg, L"batı"  ) ) door = 3;
     else if ( !wcscasecmp( arg, L"y" ) || !wcscasecmp( arg, L"yukarı"    ) ) door = 4;
     else if ( !wcscasecmp( arg, L"a" ) || !wcscasecmp( arg, L"aşağı"  ) ) door = 5;
    else
    {
	for ( door = 0; door <= 5; door++ )
	{
	    if ( ( pexit = ch->in_room->exit[door] ) != NULL
	    &&   IS_SET(pexit->exit_info, EX_ISDOOR)
	    &&   pexit->keyword != NULL
	    &&   is_name( arg, pexit->keyword ) )
		return door;
	}
  act( L"Hiç $T yok.", ch, NULL, arg, TO_CHAR );
	return -1;
    }

    if ( ( pexit = ch->in_room->exit[door] ) == NULL )
    {
      act( L"$T yönünde kapı görünmüyor.", ch, NULL, arg, TO_CHAR );
	return -1;
    }

    if ( !IS_SET(pexit->exit_info, EX_ISDOOR) )
    {
      send_to_char( L"Bunu yapamazsın.\n\r", ch );
	return -1;
    }

    return door;
}

/* scan.c */

const wchar_t * distance[4]=
{
  L"burada.", L"%s yönünde.", L"%s yönünde ilerde.", L"%s yönünde uzakta."
};

void scan_list           args((ROOM_INDEX_DATA *scan_room, CHAR_DATA *ch,
                               sh_int depth, sh_int door));
void scan_char           args((CHAR_DATA *victim, CHAR_DATA *ch,
                               sh_int depth, sh_int door));
void do_scan2(CHAR_DATA *ch, wchar_t *argument)
{
   //extern   wchar_t *  const   dir_name        [];
   EXIT_DATA *pExit;
   sh_int door;

   act( L"$n etrafa bakıyor.", ch, NULL, NULL, TO_ROOM);
   send_to_char( L"Etrafa bakınca...\n\r", ch);
                scan_list(ch->in_room, ch, 0, -1);
   for (door=0;door<6;door++)
      {
        if ( (pExit = ch->in_room->exit[door]) == NULL
	  || pExit->u1.to_room == NULL
	  || IS_SET(pExit->exit_info,EX_CLOSED) )
	continue;
        scan_list(pExit->u1.to_room, ch, 1, door);
      }
   return;
}

void scan_list(ROOM_INDEX_DATA *scan_room, CHAR_DATA *ch, sh_int depth, sh_int door)
{
   CHAR_DATA *rch;

   if (scan_room == NULL) return;
   for (rch=scan_room->people; rch != NULL; rch=rch->next_in_room)
   {
      if (rch == ch) continue;
      if (!IS_NPC(rch) && rch->invis_level > get_trust(ch)) continue;
      if (can_see(ch, rch)) scan_char(rch, ch, depth, door);
   }
   return;
}

void scan_char(CHAR_DATA *victim, CHAR_DATA *ch, sh_int depth, sh_int door)
{
   //extern wchar_t *const dir_name[];
   //extern wchar_t *const distance[];
   wchar_t buf[MAX_INPUT_LENGTH], buf2[MAX_INPUT_LENGTH];

   buf[0] = '\0';

   wcscat(buf,
 	(is_affected(victim,gsn_doppelganger) && !IS_SET(ch->act,PLR_HOLYLIGHT))
		 ? PERS(victim->doppel, ch) : PERS(victim, ch));
   wcscat(buf, L", ");
   swprintf(buf2, MAX_INPUT_LENGTH-1, distance[depth], dir_name[door]);
   wcscat(buf, buf2);
   wcscat(buf, L"\n\r");

   send_to_char(buf, ch);
   return;
}

void do_open( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int door;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
      send_to_char( L"Neyi açacaksın?\n\r", ch );
	return;
    }



    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    {
 	/* open portal */
	if (obj->item_type == ITEM_PORTAL)
	{
	    if (!IS_SET(obj->value[1], EX_ISDOOR))
	    {
		send_to_char( L"Bunu yapamazsın.\n\r",ch);
		return;
	    }

	    if (!IS_SET(obj->value[1], EX_CLOSED))
	    {
		send_to_char( L"Zaten açık.\n\r",ch);
		return;
	    }

	    if (IS_SET(obj->value[1], EX_LOCKED))
	    {
		send_to_char( L"Kilitli.\n\r",ch);
		return;
	    }

	    REMOVE_BIT(obj->value[1], EX_CLOSED);
	    act( L"$p açıyorsun.",ch,obj,NULL,TO_CHAR);
	    act( L"$n $p açıyor.",ch,obj,NULL,TO_ROOM);
	    return;
 	}

	/* 'open object' */
  if ( obj->item_type != ITEM_CONTAINER)
	    { send_to_char( L"Bu bir taşıyıcı değil.\n\r", ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSED) )
	    { send_to_char( L"Zaten açık.\n\r",ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSEABLE) )
	    { send_to_char( L"Bunu yapamazsın.\n\r",ch ); return; }
	if ( IS_SET(obj->value[1], CONT_LOCKED) )
	    { send_to_char( L"Kilitli.\n\r",ch ); return; }

	REMOVE_BIT(obj->value[1], CONT_CLOSED);
  act( L"$p açıyorsun.",ch,obj,NULL,TO_CHAR);
	act( L"$n $p açıyor.", ch, obj, NULL, TO_ROOM );
	return;
    }
    if ( ( door = find_door( ch, arg ) ) >= 0 )
    {
	/* 'open door' */
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit = ch->in_room->exit[door];
  if ( !IS_SET(pexit->exit_info, EX_CLOSED) )
	    { send_to_char( L"Zaten açık.\n\r",ch ); return; }
	if (  IS_SET(pexit->exit_info, EX_LOCKED) )
	    { send_to_char( L"Kilitli.\n\r", ch ); return; }

	REMOVE_BIT(pexit->exit_info, EX_CLOSED);
  act( L"$n $d açıyor.", ch, NULL, pexit->keyword, TO_ROOM );
	send_to_char( L"Tamam.\n\r", ch );

	/* open the other side */
	if ( ( to_room   = pexit->u1.to_room            ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != NULL
	&&   pexit_rev->u1.to_room == ch->in_room )
	{
	    CHAR_DATA *rch;

	    REMOVE_BIT( pexit_rev->exit_info, EX_CLOSED );
	    for ( rch = to_room->people; rch != NULL; rch = rch->next_in_room )
      act( L"$d açılıyor.", rch, NULL, pexit_rev->keyword, TO_CHAR );
	}
	return;
    }
    return;
}



void do_close( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int door;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
      send_to_char( L"Neyi kapatacaksın?\n\r", ch );
	return;
    }



    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    {
	/* portal stuff */
	if (obj->item_type == ITEM_PORTAL)
	{

	    if (!IS_SET(obj->value[1],EX_ISDOOR)
	    ||   IS_SET(obj->value[1],EX_NOCLOSE))
	    {
        send_to_char( L"Bunu yapamazsın.\n\r",ch);
		return;
	    }

	    if (IS_SET(obj->value[1],EX_CLOSED))
	    {
        send_to_char( L"Zaten kapalı.\n\r",ch);
		return;
	    }

	    SET_BIT(obj->value[1],EX_CLOSED);
      act( L"$p'yi kapatıyorsun.",ch,obj,NULL,TO_CHAR);
	    act( L"$n $p'yi kapatıyor.",ch,obj,NULL,TO_ROOM);
	    return;
	}

	/* 'close object' */
  if ( obj->item_type != ITEM_CONTAINER )
	    { send_to_char( L"Bu bir taşıyıcı değil.\n\r", ch ); return; }
	if ( IS_SET(obj->value[1], CONT_CLOSED) )
	    { send_to_char( L"Zaten kapalı.\n\r",ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSEABLE) )
	    { send_to_char( L"Bunu yapamazsın.\n\r",ch ); return; }

	SET_BIT(obj->value[1], CONT_CLOSED);
  act( L"$p'yi kapatıyorsun.",ch,obj,NULL,TO_CHAR);
	act( L"$n $p'yi kapatıyor.", ch, obj, NULL, TO_ROOM );
	return;
    }

    if ( ( door = find_door( ch, arg ) ) >= 0 )
    {
	/* 'close door' */
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit	= ch->in_room->exit[door];
	if ( IS_SET(pexit->exit_info, EX_CLOSED) )
  { send_to_char( L"Zaten kapalı.\n\r",ch ); return; }

	SET_BIT(pexit->exit_info, EX_CLOSED);
  act( L"$d $n tarafından kapatılıyor.", ch, NULL, pexit->keyword, TO_ROOM );
	send_to_char( L"Tamam.\n\r", ch );

	/* close the other side */
	if ( ( to_room   = pexit->u1.to_room            ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != 0
	&&   pexit_rev->u1.to_room == ch->in_room )
	{
	    CHAR_DATA *rch;

	    SET_BIT( pexit_rev->exit_info, EX_CLOSED );
	    for ( rch = to_room->people; rch != NULL; rch = rch->next_in_room )
      act( L"$d kapanıyor.", rch, NULL, pexit_rev->keyword, TO_CHAR );
	}
	return;
    }

    return;
}


/*
 * Added can_see check. Kio.
 */

bool has_key( CHAR_DATA *ch, int key )
{
    OBJ_DATA *obj;

    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
	if ( obj->pIndexData->vnum == key )
	  if ( can_see_obj( ch, obj ) )
	    return TRUE;
    }

    return FALSE;
}

bool has_key_ground( CHAR_DATA *ch, int key )
{
    OBJ_DATA *obj;

    for ( obj = ch->in_room->contents; obj != NULL; obj = obj->next_content )
    {
	if ( obj->pIndexData->vnum == key )
          if ( can_see_obj( ch, obj ) )
	    return TRUE;
    }

    return FALSE;
}



void do_lock( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int door;
    CHAR_DATA *rch;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
      send_to_char( L"Neyi kilitleyeceksin?\n\r", ch );
	return;
    }


    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    {
	/* portal stuff */
	if (obj->item_type == ITEM_PORTAL)
	{
	    if (!IS_SET(obj->value[1],EX_ISDOOR)
	    ||  IS_SET(obj->value[1],EX_NOCLOSE))
	    {
		send_to_char( L"Bunu yapamazsın.\n\r",ch);
		return;
	    }
	    if (!IS_SET(obj->value[1],EX_CLOSED))
	    {
		send_to_char( L"Kapalı değil.\n\r",ch);
	 	return;
	    }

	    if (obj->value[4] < 0 || IS_SET(obj->value[1],EX_NOLOCK))
	    {
		send_to_char( L"Kilitlenemez.\n\r",ch);
		return;
	    }

	    if (!has_key(ch,obj->value[4]))
	    {
		send_to_char( L"Anahtarın yok.\n\r",ch);
		return;
	    }

	    if (IS_SET(obj->value[1],EX_LOCKED))
	    {
		send_to_char( L"Zaten kilitli.\n\r",ch);
		return;
	    }

	    SET_BIT(obj->value[1],EX_LOCKED);
      act( L"$p eşyasını kilitliyorsun.",ch,obj,NULL,TO_CHAR);
	    act( L"$n $p eşyasını kilitliyor.",ch,obj,NULL,TO_ROOM);
	    return;
	}

	/* 'lock object' */
  if ( obj->item_type != ITEM_CONTAINER )
	    { send_to_char( L"Bu bir taşıyıcı değil.\n\r", ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSED) )
	    { send_to_char( L"Kapalı değil.\n\r", ch ); return; }
	if ( obj->value[2] < 0 )
	    { send_to_char( L"Kilitlenemez.\n\r",ch ); return; }
	if ( !has_key( ch, obj->value[2] ) )
	    { send_to_char( L"Anahtarın yok.\n\r",ch ); return; }
	if ( IS_SET(obj->value[1], CONT_LOCKED) )
	    { send_to_char( L"Zaten kilitli.\n\r",    ch ); return; }

	SET_BIT(obj->value[1], CONT_LOCKED);
  act( L"$p eşyasını kilitliyorsun.",ch,obj,NULL,TO_CHAR);
	act( L"$n $p eşyasını kilitliyor.", ch, obj, NULL, TO_ROOM );
	return;
    }

    if ( ( door = find_door( ch, arg ) ) >= 0 )
    {
	/* 'lock door' */
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit	= ch->in_room->exit[door];
  if ( !IS_SET(pexit->exit_info, EX_CLOSED) )
	    { send_to_char( L"Kapalı değil.\n\r",        ch ); return; }
	if ( pexit->key < 0 )
	    { send_to_char( L"Kilitlenemez.\n\r",     ch ); return; }
	if ( !has_key( ch, pexit->key) &&
             !has_key_ground( ch, pexit->key) )
	    { send_to_char( L"Anahtarın yok.\n\r",       ch ); return; }
	if ( IS_SET(pexit->exit_info, EX_LOCKED) )
	    { send_to_char( L"Zaten kilitli.\n\r",    ch ); return; }

	SET_BIT(pexit->exit_info, EX_LOCKED);
  send_to_char( L"*Klik*\n\r", ch );
	act( L"$n $d'yi kilitliyor.", ch, NULL, pexit->keyword, TO_ROOM );

	/* lock the other side */
	if ( ( to_room   = pexit->u1.to_room            ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != 0
	&&   pexit_rev->u1.to_room == ch->in_room )
	{
	    SET_BIT( pexit_rev->exit_info, EX_LOCKED );
	    for ( rch = to_room->people; rch != NULL; rch = rch->next_in_room )
      act( L"$d'nin kilitleniyor.", rch, NULL, pexit_rev->keyword, TO_CHAR );

	}
        return;
    }
    return;
}



void do_unlock( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int door;
    CHAR_DATA *rch;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
      send_to_char( L"Neyin kilidini açacaksın?\n\r", ch );
	return;
    }


    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    {
 	/* portal stuff */
	if (obj->item_type == ITEM_PORTAL)
	{
	    if (IS_SET(obj->value[1],EX_ISDOOR))
	    {
		send_to_char( L"Bunu yapamazsın.\n\r",ch);
		return;
	    }

	    if (!IS_SET(obj->value[1],EX_CLOSED))
	    {
		send_to_char( L"Kapalı değil.\n\r",ch);
		return;
	    }

	    if (obj->value[4] < 0)
	    {
		send_to_char( L"Kilidi açılamaz.\n\r",ch);
		return;
	    }

	    if (!has_key(ch,obj->value[4]))
	    {
		send_to_char( L"Anahtarın yok.\n\r",ch);
		return;
	    }

	    if (!IS_SET(obj->value[1],EX_LOCKED))
	    {
		send_to_char( L"Kilidi açılmış.\n\r",ch);
		return;
	    }

	    REMOVE_BIT(obj->value[1],EX_LOCKED);
      act( L"$p nesnesinin kilidini açıyorsun.",ch,obj,NULL,TO_CHAR);
	    act( L"$n $p nesnesinin kilidini açıyor.",ch,obj,NULL,TO_ROOM);
	    return;
	}

	/* 'unlock object' */
  if ( obj->item_type != ITEM_CONTAINER )
	    { send_to_char( L"Bu bir taşıyıcı değil.\n\r", ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSED) )
	    { send_to_char( L"Kapalı değil.\n\r", ch ); return; }
	if ( obj->value[2] < 0 )
	    { send_to_char( L"Kilidi açılamaz.\n\r",ch ); return; }
	if ( !has_key( ch, obj->value[2] ) )
	    { send_to_char( L"Anahtarın yok.\n\r",ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_LOCKED) )
	    { send_to_char( L"Kilidi açılmış.\n\r",ch ); return; }

	REMOVE_BIT(obj->value[1], CONT_LOCKED);
  act( L"$p nesnesinin kilidini açıyorsun.",ch,obj,NULL,TO_CHAR);
	act( L"$n $p nesnesinin kilidini açıyor.", ch, obj, NULL, TO_ROOM );
	return;
    }

    if ( ( door = find_door( ch, arg ) ) >= 0 )
    {
	/* 'unlock door' */
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit = ch->in_room->exit[door];
	if ( !IS_SET(pexit->exit_info, EX_CLOSED) )
	    { send_to_char( L"Kapalı değil.\n\r",        ch ); return; }
	if ( pexit->key < 0 )
	    { send_to_char( L"Kilidi açılamaz.\n\r",   ch ); return; }
	if ( !has_key( ch, pexit->key) &&
	     !has_key_ground( ch, pexit->key) )
	    { send_to_char( L"Anahtarın yok.\n\r",       ch ); return; }
	if ( !IS_SET(pexit->exit_info, EX_LOCKED) )
	    { send_to_char( L"Kilidi zaten açılmış.\n\r",  ch ); return; }

	REMOVE_BIT(pexit->exit_info, EX_LOCKED);
  send_to_char( L"*Klik*\n\r", ch );
	act( L"$n $d'nin kilidini açıyor.", ch, NULL, pexit->keyword, TO_ROOM );

	/* unlock the other side */
	if ( ( to_room   = pexit->u1.to_room            ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != NULL
	&&   pexit_rev->u1.to_room == ch->in_room )
	{
	    REMOVE_BIT( pexit_rev->exit_info, EX_LOCKED );
	    for ( rch = to_room->people; rch != NULL; rch = rch->next_in_room )
      act( L"$d'nin kilidi açılıyor.", rch, NULL, pexit_rev->keyword, TO_CHAR );
	}
        return;
    }
    return;
}



void do_pick( CHAR_DATA *ch, wchar_t *argument )
{
  wchar_t arg[MAX_INPUT_LENGTH];
  OBJ_DATA *obj, *obj_eldiven, *obj_next;
  int door;
  bool obj_found = FALSE;
  int chance = 0;

  if(IS_NPC(ch))
  {
    return;
  }

  if(ch->iclass != CLASS_THIEF && ch->iclass != CLASS_NINJA && !IS_IMMORTAL(ch))
  {
    send_to_char( L"Kilitlerin ilgi alanına girdiği söylenemez.\n\r", ch);
    return;
  }

  if (MOUNTED(ch))
  {
    send_to_char( L"Biniciyken maymuncuk uygulayamazsın.\n\r", ch);
    return;
  }

  one_argument( argument, arg );

  if ( arg[0] == '\0' )
  {
    send_to_char( L"Neye maymuncuk kullanacaksın?\n\r", ch );
    return;
  }

  for (obj = ch->carrying; obj != NULL; obj= obj_next)
  {
    obj_next = obj->next_content;

    if(obj->item_type == ITEM_MAYMUNCUK)
    {
      obj_found = TRUE;
      extract_obj( obj );
      break;
    }
  }

  if ( obj_found == FALSE )
  {
    send_to_char( L"Maymuncuk setin yok.\n\r", ch );
    return;
  }

  WAIT_STATE( ch, skill_table[gsn_pick_lock].beats );

  chance += int( get_curr_stat( ch , STAT_INT ) / 3 );
  chance += int( get_curr_stat( ch , STAT_WIS ) / 3 );
  chance += int( get_curr_stat( ch , STAT_DEX ) / 3 );
  chance += int( ( get_skill( ch , gsn_pick_lock ) - 75 ) / 2 ) ;

  obj_eldiven = get_eq_char( ch, WEAR_HANDS );

  if( obj_eldiven == NULL )
  {
    chance += 10;
  }
  else if( is_metal( obj_eldiven ) )
  {
    chance -= 20;
  }
  else
  {
    chance -= 5;
  }

  obj = get_obj_here( ch, arg );

  // Kabal esyalarini tutan kasalari koruyalim.
  if ( obj != NULL )
  {
    if( obj->pIndexData->vnum >= 500 && obj->pIndexData->vnum <= 580 )
    {
      if( obj_eldiven == NULL )
        chance = 5;
      else if( is_metal( obj_eldiven ) )
        chance = 1;
      else
        chance = 2;
    }
  }
  // Kabal kasalari duzenlemesi bitti.

  if ( number_percent( ) > chance )
  {
    send_to_char( L"Başaramadın.\n\r",ch);
    check_improve(ch,gsn_pick_lock,FALSE,2);
    return;
  }

  if ( obj != NULL )
  {
    /* portal stuff */
    if (obj->item_type == ITEM_PORTAL)
    {
      if (!IS_SET(obj->value[1],EX_ISDOOR))
      {
        send_to_char( L"Bunu yapamazsın.\n\r",ch);
        return;
      }

      if (!IS_SET(obj->value[1],EX_CLOSED))
      {
        send_to_char( L"Kapalı değil.\n\r",ch );
        return;
      }

      if (obj->value[4] < 0)
      {
        send_to_char( L"Kilidi açılamaz.\n\r",ch);
        return;
      }

      REMOVE_BIT(obj->value[1],EX_LOCKED);
      act( L"$p üzerindeki kilidi maymuncukla açtın.",ch,obj,NULL,TO_CHAR);
      act( L"$n $p üzerindeki kilidi maymuncukla açtı.",ch,obj,NULL,TO_ROOM);
      check_improve(ch,gsn_pick_lock,TRUE,2);
      return;
    }


    /* 'pick object' */
    if ( obj->item_type != ITEM_CONTAINER )
    { send_to_char( L"Bu bir taşıyıcı değil.\n\r", ch ); return; }
    if ( !IS_SET(obj->value[1], CONT_CLOSED) )
    { send_to_char( L"Kapalı değil.\n\r",ch ); return; }
    if ( obj->value[2] < 0 )
    {send_to_char( L"Kilidi açılamaz.\n\r",   ch ); return; }
    if ( !IS_SET(obj->value[1], CONT_LOCKED) )
    { send_to_char( L"Kilidi açılmış.\n\r",  ch ); return; }

    REMOVE_BIT(obj->value[1], CONT_LOCKED);
    act( L"$p üzerindeki kilidi maymuncukla açtın.",ch,obj,NULL,TO_CHAR);
    act( L"$n $p üzerindeki kilidi maymuncukla açtı.",ch,obj,NULL,TO_ROOM);
    check_improve(ch,gsn_pick_lock,TRUE,2);
    return;
  }

  if ( ( door = find_door( ch, arg ) ) >= 0 )
  {
    /* 'pick door' */
    ROOM_INDEX_DATA *to_room;
    EXIT_DATA *pexit;
    EXIT_DATA *pexit_rev;

    pexit = ch->in_room->exit[door];
    if ( !IS_SET(pexit->exit_info, EX_CLOSED) && !IS_IMMORTAL(ch))
    { send_to_char( L"Kapalı değil.\n\r",ch ); return; }
    if ( !IS_SET(pexit->exit_info, EX_LOCKED) )
    { send_to_char( L"Kilidi açılmış.\n\r",  ch ); return; }

    REMOVE_BIT(pexit->exit_info, EX_LOCKED);
    send_to_char( L"*Klik*\n\r", ch );
    act( L"$n $d'nin kilidini maymuncukla açıyor.", ch, NULL, pexit->keyword, TO_ROOM );
    check_improve(ch,gsn_pick_lock,TRUE,2);

    /* pick the other side */
    if ( ( to_room   = pexit->u1.to_room            ) != NULL
    &&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != NULL
    &&   pexit_rev->u1.to_room == ch->in_room )
    {
      REMOVE_BIT( pexit_rev->exit_info, EX_LOCKED );
    }
  }
  return;
}




void do_stand( CHAR_DATA *ch, wchar_t *argument )
{
    OBJ_DATA *obj = NULL;

    if (argument[0] != '\0')
    {
	if (ch->position == POS_FIGHTING)
	{
    send_to_char( L"Belki de önce dövüşmeyi bırakmalısın?\n\r",ch);
	    return;
	}
	obj = get_obj_list(ch,argument,ch->in_room->contents);
	if (obj == NULL)
	{
    send_to_char( L"Öyle birşey görmüyorsun.\n\r",ch );
	    return;
	}
	if (obj->item_type != ITEM_FURNITURE
	||  (!IS_SET(obj->value[2],STAND_AT)
	&&   !IS_SET(obj->value[2],STAND_ON)
	&&   !IS_SET(obj->value[2],STAND_IN)))
	{
    send_to_char( L"Ayağa dikilebilecek bir yer göremiyorsun.\n\r",ch);
	    return;
	}
	if (ch->on != obj && count_users(obj) >= obj->value[0])
	{
    act_new( L"$p üzerinde ayağa dikilebileceğin yer yok.",
  ch,obj,NULL,TO_ROOM,POS_DEAD);
	    return;
	}
    }
    switch ( ch->position )
    {
    case POS_SLEEPING:
	if ( IS_AFFECTED(ch, AFF_SLEEP) )
  { send_to_char( L"Uyanamıyorsun!\n\r", ch ); return; }

	if (obj == NULL)
	{
    send_to_char( L"Uyanıyor ve kalkıyorsun.\n\r", ch );
    act( L"$n uyanıyor ve kalkıyor.", ch, NULL, NULL, TO_ROOM );
	    ch->on = NULL;
	}
	else if (IS_SET(obj->value[2],STAND_AT))
	{
    act_new( L"$p'de uyanıyor ve kalkıyorsun.",ch,obj,NULL,TO_CHAR,POS_DEAD);
    act( L"$n $p'de uyanıyor ve kalkıyor.",ch,obj,NULL,TO_ROOM);
	}
	else if (IS_SET(obj->value[2],STAND_ON))
	{
    act_new( L"$p üzerinde uyanıyor ve kalkıyorsun.",ch,obj,NULL,TO_CHAR,POS_DEAD);
    act( L"$n $p üzerinde uyanıyor ve kalkıyor.",ch,obj,NULL,TO_ROOM);
	}
	else
	{
    act_new( L"$p içinde uyanıyor ve kalkıyorsun.",ch,obj,NULL,TO_CHAR,POS_DEAD);
    act( L"$n $p içinde uyanıyor ve kalkıyor.",ch,obj,NULL,TO_ROOM);
	}

        if (IS_HARA_KIRI(ch))
	{
    send_to_char( L"Kanının vücudunu ısıttığını hissediyorsun.\n\r",ch);
	 REMOVE_BIT(ch->act,PLR_HARA_KIRI);
	}

	ch->position = POS_STANDING;
	do_look(ch,(wchar_t*)"auto");
	break;

    case POS_RESTING: case POS_SITTING:
	if (obj == NULL)
	{
    send_to_char( L"Ayağa kalkıyorsun.\n\r", ch );
    act( L"$n ayağa kalkıyor.", ch, NULL, NULL, TO_ROOM );
	    ch->on = NULL;
	}
	else if (IS_SET(obj->value[2],STAND_AT))
	{
    act( L"$p'de ayağa kalkıyorsun.",ch,obj,NULL,TO_CHAR);
    act( L"$n $p'de ayağa kalkıyor.",ch,obj,NULL,TO_ROOM);
	}
	else if (IS_SET(obj->value[2],STAND_ON))
	{
    act( L"$p üzerinde ayağa kalkıyorsun.",ch,obj,NULL,TO_CHAR);
    act( L"$n $p üzerinde ayağa kalkıyor.",ch,obj,NULL,TO_ROOM);
	}
	else
	{
    act( L"$p içinde ayağa kalkıyorsun.",ch,obj,NULL,TO_CHAR);
    act( L"$n $p içinde ayağa kalkıyor.",ch,obj,NULL,TO_ROOM);
	}
	ch->position = POS_STANDING;
	break;

    case POS_STANDING:
    send_to_char( L"Zaten ayaktasın.\n\r", ch );
	break;

    case POS_FIGHTING:
    send_to_char( L"Zaten dövüşüyorsun!\n\r", ch );
	break;
    }

    return;
}



void do_rest( CHAR_DATA *ch, wchar_t *argument )
{
    OBJ_DATA *obj = NULL;

    if (ch->position == POS_FIGHTING)
    {
      send_to_char( L"Zaten dövüşüyorsun!\n\r",ch);
	return;
    }

    if (MOUNTED(ch))
    {
      send_to_char( L"Biniciyken dinlenemezsin.\n\r", ch);
        return;
    }
    if (RIDDEN(ch))
    {
      send_to_char( L"Binekken dinlenemezsin.\n\r", ch);
        return;
    }


    if ( IS_AFFECTED(ch, AFF_SLEEP) )
    { send_to_char( L"Zaten uyuyorsun.\n\r", ch ); return; }

    /* okay, now that we know we can rest, find an object to rest on */
    if (argument[0] != '\0')
    {
	obj = get_obj_list(ch,argument,ch->in_room->contents);
	if (obj == NULL)
	{
    send_to_char( L"Öyle birşey görmüyorsun.\n\r",ch );
	    return;
	}
    }
    else obj = ch->on;

    if (obj != NULL)
    {
        if (!IS_SET(obj->item_type,ITEM_FURNITURE)
    	||  (!IS_SET(obj->value[2],REST_ON)
    	&&   !IS_SET(obj->value[2],REST_IN)
    	&&   !IS_SET(obj->value[2],REST_AT)))
    	{
        send_to_char( L"Onun üzerinde dinlenemezsin.\n\r",ch);
	    return;
    	}

        if (obj != NULL && ch->on != obj && count_users(obj) >= obj->value[0])
        {
          act_new( L"$p üzerinde yer yok.",ch,obj,NULL,TO_CHAR,POS_DEAD);
	    return;
    	}

	ch->on = obj;
    }

    switch ( ch->position )
    {
    case POS_SLEEPING:
	if (obj == NULL)
	{
    send_to_char( L"Uyanıyor ve dinlenmeye başlıyorsun.\n\r", ch );
    act ( L"$n uyanıyor ve dinlenmeye başlıyor.",ch,NULL,NULL,TO_ROOM);
	}
	else if (IS_SET(obj->value[2],REST_AT))
	{
    act_new( L"Uyanıyor ve $p'de sinlenmeye başlıyorsun.",
      ch,obj,NULL,TO_CHAR,POS_SLEEPING);
    act( L"$n uyanıyor ve $p'de dinlenmeye başlıyor.",ch,obj,NULL,TO_ROOM);
	}
        else if (IS_SET(obj->value[2],REST_ON))
        {
          act_new( L"Uyanıyor ve $p üzerinde dinlenmeye başlıyorsun.",
                  ch,obj,NULL,TO_CHAR,POS_SLEEPING);
          act( L"$n uyanıyor ve $p üzerinde dinlenmeye başlıyor.",ch,obj,NULL,TO_ROOM);
        }
        else
        {
          act_new( L"Uyanıyor ve $p içinde dinlenmeye başlıyorsun.",
                  ch,obj,NULL,TO_CHAR,POS_SLEEPING);
          act( L"$n uyanıyor ve $p içinde dinlenmeye başlıyor.",ch,obj,NULL,TO_ROOM);
        }
	ch->position = POS_RESTING;
	break;

    case POS_RESTING:
    send_to_char( L"Zaten dinleniyorsun.\n\r", ch );
	break;

    case POS_STANDING:
    if (obj == NULL)
  	{
  	    send_to_char( L"Dinlenmeye başlıyorsun.\n\r", ch );
  	    act( L"$n oturuyor ve dinlenmeye başlıyor.", ch, NULL, NULL, TO_ROOM );
  	}
          else if (IS_SET(obj->value[2],REST_AT))
          {
  	    act( L"$p'de oturuyor ve dinlenmeye başlıyorsun.",ch,obj,NULL,TO_CHAR);
  	    act( L"$n $p'de oturuyor ve dinlenmeye başlıyor.",ch,obj,NULL,TO_ROOM);
          }
          else if (IS_SET(obj->value[2],REST_ON))
          {
  	    act( L"$p üzerine oturuyor ve dinlenmeye başlıyorsun.",ch,obj,NULL,TO_CHAR);
  	    act( L"$n $p üzerine oturuyor ve dinlenmeye başlıyor.",ch,obj,NULL,TO_ROOM);
          }
          else
          {
  	    act( L"$p içinde dinlenmeye başlıyorsun.",ch,obj,NULL,TO_CHAR);
  	    act( L"$n $p içinde dinlenmeye başlıyor.",ch,obj,NULL,TO_ROOM);
          }
	ch->position = POS_RESTING;
	break;

    case POS_SITTING:
    if (obj == NULL)
  	{
  	    send_to_char( L"Dinlenmeye başlıyorsun.\n\r",ch);
  	    act( L"$n rests.",ch,NULL,NULL,TO_ROOM);
  	}
          else if (IS_SET(obj->value[2],REST_AT))
          {
  	    act( L"$p'de dinlenmeye başlıyorsun.",ch,obj,NULL,TO_CHAR);
  	    act( L"$n $p'de dinlenmeye başlıyor.",ch,obj,NULL,TO_ROOM);
          }
          else if (IS_SET(obj->value[2],REST_ON))
          {
  	    act( L"$p üzerinde dinlenmeye başlıyorsun.",ch,obj,NULL,TO_CHAR);
  	    act( L"$n $p üzerinde dinlenmeye başlıyor.",ch,obj,NULL,TO_ROOM);
          }
          else
          {
  	    act( L"$p içinde dinlenmeye başlıyorsun.",ch,obj,NULL,TO_CHAR);
  	    act( L"$n $p içinde dinlenmeye başlıyor.",ch,obj,NULL,TO_ROOM);
  	}
	ch->position = POS_RESTING;

        if (IS_HARA_KIRI(ch))
	{
    send_to_char( L"Kanının vücudunu ısıttığını hissediyorsun.\n\r",ch);
	 REMOVE_BIT(ch->act,PLR_HARA_KIRI);
	}

	break;
    }


    return;
}


void do_sit (CHAR_DATA *ch, wchar_t *argument )
{
    OBJ_DATA *obj = NULL;

    if (ch->position == POS_FIGHTING)
    {
	send_to_char( L"Belki de önce dövüşmeyi bırakmalısın?\n\r",ch);
	return;
    }
    if (MOUNTED(ch))
    {
        send_to_char( L"Biniciyken oturamazsın.\n\r", ch);
        return;
    }
    if (RIDDEN(ch))
    {
        send_to_char( L"Binekken oturamazsın.\n\r", ch);
        return;
    }

    if ( IS_AFFECTED(ch, AFF_SLEEP) )
    { send_to_char( L"Zaten uyuyorsun.\n\r", ch ); return; }

    /* okay, now that we know we can sit, find an object to sit on */
    if (argument[0] != '\0')
    {
	obj = get_obj_list(ch,argument,ch->in_room->contents);
	if (obj == NULL)
	{
    if ( IS_AFFECTED(ch, AFF_SLEEP) )
    { send_to_char( L"Zaten uyuyorsun.\n\r", ch ); return; }
	    send_to_char( L"Öyle birşey görmüyorsun.\n\r",ch );
	    return;
	}
    }
    else obj = ch->on;

    if (obj != NULL)
    {
	if (!IS_SET(obj->item_type,ITEM_FURNITURE)
	||  (!IS_SET(obj->value[2],SIT_ON)
	&&   !IS_SET(obj->value[2],SIT_IN)
	&&   !IS_SET(obj->value[2],SIT_AT)))
	{
	    send_to_char( L"Onun üzerine oturamazsın.\n\r",ch);
	    return;
	}

	if (obj != NULL && ch->on != obj && count_users(obj) >= obj->value[0])
	{
	    act_new( L"$p üzerinde yer yok.",ch,obj,NULL,TO_CHAR,POS_DEAD);
	    return;
	}

	ch->on = obj;
    }
    switch (ch->position)
    {
	case POS_SLEEPING:
            if (obj == NULL)
            {
            	send_to_char( L"Uyanıyor ve oturuyorsun.\n\r", ch );
            	act( L"$n uyanıyor ve oturuyor.", ch, NULL, NULL, TO_ROOM );
            }
            else if (IS_SET(obj->value[2],SIT_AT))
            {
            	act_new( L"Uyanıyor ve $p'de oturuyorsun.",ch,obj,NULL,TO_CHAR,POS_DEAD);
            	act( L"$n uyanıyor ve $p'de oturuyor.",ch,obj,NULL,TO_ROOM);
            }
            else if (IS_SET(obj->value[2],SIT_ON))
            {
            	act_new( L"Uyanıyor ve $p üstüne oturuyorsun.",ch,obj,NULL,TO_CHAR,POS_DEAD);
            	act( L"$n uyanıyor ve $p üzerine oturuyor.",ch,obj,NULL,TO_ROOM);
            }
            else
            {
            	act_new( L"Uyanıyor ve $p içine oturuyorsun.",ch,obj,NULL,TO_CHAR,POS_DEAD);
            	act( L"$n uyanıyor ve $p içine oturuyor.",ch,obj,NULL,TO_ROOM);
            }

	    ch->position = POS_SITTING;
	    break;
	case POS_RESTING:
	    if (obj == NULL)
		send_to_char( L"Dinlenmeyi bıraktın.\n\r",ch);
	    else if (IS_SET(obj->value[2],SIT_AT))
	    {
		act( L"$p'de oturuyorsun.",ch,obj,NULL,TO_CHAR);
		act( L"$n $p'de oturuyor.",ch,obj,NULL,TO_ROOM);
	    }

	    else if (IS_SET(obj->value[2],SIT_ON))
	    {
		act( L"$p üzerine oturuyorsun.",ch,obj,NULL,TO_CHAR);
		act( L"$n $p üzerine oturuyor.",ch,obj,NULL,TO_ROOM);
	    }
	    ch->position = POS_SITTING;
	    break;
	case POS_SITTING:
	    send_to_char( L"Zaten oturuyorsun.\n\r",ch);
	    break;
	case POS_STANDING:
	    if (obj == NULL)
    	    {
		send_to_char( L"Oturuyorsun.\n\r",ch);
    	        act( L"$n yere oturuyor.",ch,NULL,NULL,TO_ROOM);
	    }
	    else if (IS_SET(obj->value[2],SIT_AT))
	    {
		act( L"$p'de oturuyorsun.",ch,obj,NULL,TO_CHAR);
		act( L"$n $p'de oturuyor.",ch,obj,NULL,TO_ROOM);
	    }
	    else if (IS_SET(obj->value[2],SIT_ON))
	    {
		act( L"$p üzerine oturuyorsun.",ch,obj,NULL,TO_CHAR);
		act( L"$n $p üzerine oturuyor.",ch,obj,NULL,TO_ROOM);
	    }
	    else
	    {
		act( L"$p içine oturuyorsun.",ch,obj,NULL,TO_CHAR);
		act( L"$n $p içine oturuyor.",ch,obj,NULL,TO_ROOM);
	    }
    	    ch->position = POS_SITTING;
    	    break;
    }
    if (IS_HARA_KIRI(ch))
	{
	 send_to_char( L"Kanının vücudunu ısıttığını hissediyorsun.\n\r",ch);
	 REMOVE_BIT(ch->act,PLR_HARA_KIRI);
	}
    return;
}


void do_sleep( CHAR_DATA *ch, wchar_t *argument )
{
    OBJ_DATA *obj = NULL;

    if (MOUNTED(ch))
    {
        send_to_char( L"Biniciyken uyuyamazsın.\n\r", ch);
        return;
    }
    if (RIDDEN(ch))
    {
        send_to_char( L"Binekken uyuyamazsın.\n\r", ch);
        return;
    }

    switch ( ch->position )
    {
    case POS_SLEEPING:
	send_to_char( L"Zaten uyuyorsun.\n\r", ch );
	break;

    case POS_RESTING:
    case POS_SITTING:
    case POS_STANDING:
	if (argument[0] == '\0' && ch->on == NULL)
	{
	    send_to_char( L"Uyumaya başlıyorsun.\n\r", ch );
	    act( L"$n uyumaya başlıyor.", ch, NULL, NULL, TO_ROOM );
	    ch->position = POS_SLEEPING;
	}
	else  /* find an object and sleep on it */
	{
	    if (argument[0] == '\0')
		obj = ch->on;
	    else
	    	obj = get_obj_list( ch, argument,  ch->in_room->contents );

	    if (obj == NULL)
	    {
		send_to_char( L"Öyle birşey görmüyorsun.\n\r",ch );
		return;
	    }
	    if (obj->item_type != ITEM_FURNITURE
	    ||  (!IS_SET(obj->value[2],SLEEP_ON)
	    &&   !IS_SET(obj->value[2],SLEEP_IN)
	    &&	 !IS_SET(obj->value[2],SLEEP_AT)))
	    {
		send_to_char( L"Onun üzerinde uyuyamazsın!\n\r",ch);
		return;
	    }

	    if (ch->on != obj && count_users(obj) >= obj->value[0])
	    {
		act_new( L"$p üzerinde yer yok.",
		    ch,obj,NULL,TO_CHAR,POS_DEAD);
		return;
	    }

	    ch->on = obj;
	    if (IS_SET(obj->value[2],SLEEP_AT))
	    {
		act( L"You go to sleep at $p.",ch,obj,NULL,TO_CHAR);
		act( L"$n goes to sleep at $p.",ch,obj,NULL,TO_ROOM);
	    }
	    else if (IS_SET(obj->value[2],SLEEP_ON))
	    {
	        act( L"$p üzerinde uyumaya başlıyorsun.",ch,obj,NULL,TO_CHAR);
	        act( L"$n $p üzerinde uyumaya başlıyor.",ch,obj,NULL,TO_ROOM);
	    }
	    else
	    {
		act( L"$p içinde uyumaya başlıyorsun.",ch,obj,NULL,TO_CHAR);
		act( L"$n $p içinde uyumaya başlıyor.",ch,obj,NULL,TO_ROOM);
	    }
	    ch->position = POS_SLEEPING;
	}
	break;

    case POS_FIGHTING:
	send_to_char( L"Zaten dövüşüyorsun!\n\r",ch );
	break;
    }

    return;
}



void do_wake( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
	{ do_stand( ch, argument ); return; }

    if ( !IS_AWAKE(ch) )
	{ send_to_char( L"Uyanık değilken mi!\n\r",       ch ); return; }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
	{ send_to_char( L"Burada değil.\n\r",              ch ); return; }

    if ( IS_AWAKE(victim) )
	{ act( L"$N zaten uyanık.", ch, NULL, victim, TO_CHAR ); return; }

    if ( IS_AFFECTED(victim, AFF_SLEEP) )
	{ act( L"Onu uyandıramazsın!",   ch, NULL, victim, TO_CHAR );  return; }

    act_new( L"$n seni uyandırıyor.", ch, NULL, victim, TO_VICT,POS_SLEEPING );
    do_stand(victim,(wchar_t*)"");
    return;
}



void do_sneak( CHAR_DATA *ch, wchar_t *argument )
{
    AFFECT_DATA af;

    if (MOUNTED(ch))
    {
      send_to_char( L"Biniciyken süzülemezsin.\n\r", ch);
        return;
    }

    send_to_char( L"Süzülmeye çalışıyorsun.\n\r", ch );
    affect_strip( ch, gsn_sneak );

    if (IS_AFFECTED(ch,AFF_SNEAK))
	return;

    if ( number_percent( ) < get_skill(ch,gsn_sneak))
    {
	check_improve(ch,gsn_sneak,TRUE,3);
	af.where     = TO_AFFECTS;
	af.type      = gsn_sneak;
	af.level     = ch->level;
	af.duration  = ch->level;
	af.location  = APPLY_NONE;
	af.modifier  = 0;
	af.bitvector = AFF_SNEAK;
	affect_to_char( ch, &af );
    }
    else
	check_improve(ch,gsn_sneak,FALSE,3);

    return;
}



void do_hide( CHAR_DATA *ch, wchar_t *argument )
{
int forest;

    if (MOUNTED(ch))
    {
      send_to_char( L"Biniciyken saklanamazsın.\n\r", ch);
        return;
    }

    if (RIDDEN(ch))
    {
      send_to_char( L"Binekken saklanamazsın.\n\r", ch);
        return;
    }

    if ( IS_AFFECTED( ch, AFF_FAERIE_FIRE )  )  {
      send_to_char( L"Parlarken saklanamazsın.\n\r", ch );
      return;
    }
    forest = 0;
    forest += ch->in_room->sector_type == SECT_FOREST?60:0;
    forest += ch->in_room->sector_type == SECT_FIELD?60:0;

    send_to_char( L"Saklanmaya çalışıyorsun.\n\r", ch );

    if ( number_percent( ) < get_skill(ch,gsn_hide)-forest)
    {
	SET_BIT(ch->affected_by, AFF_HIDE);
	check_improve(ch,gsn_hide,TRUE,3);
    }
    else  {
      if ( IS_AFFECTED(ch, AFF_HIDE) )
	REMOVE_BIT(ch->affected_by, AFF_HIDE);
	check_improve(ch,gsn_hide,FALSE,3);
    }

    return;
}

void do_camouflage( CHAR_DATA *ch, wchar_t *argument )
{

    if (MOUNTED(ch))
    {
      send_to_char( L"Biniciyken kamufle olamazsın.\n\r", ch);
        return;
    }
    if (RIDDEN(ch))
    {
      send_to_char( L"Binekken kamufle olamazsın.\n\r", ch);
        return;
    }

    if ( IS_NPC(ch) ||
         ch->level < skill_table[gsn_camouflage].skill_level[ch->iclass] )
      {
        send_to_char( L"Nasıl kamufle olunacağını bilmiyorsun.\n\r",ch);
	return;
      }

    if ( IS_AFFECTED( ch, AFF_FAERIE_FIRE ) )  {
      send_to_char( L"Parlarken kamufle olamazsın.\n\r", ch );
      return;
    }

    if (ch->in_room->sector_type != SECT_FOREST &&
	ch->in_room->sector_type != SECT_HILLS  &&
	ch->in_room->sector_type != SECT_MOUNTAIN)
      {
        send_to_char( L"Doğal bir örtü görmüyorsun.\n\r",ch);
      	act( L"$n küçük bir yaprağın altına gizlenmeye çalışıyor.",ch,NULL,NULL,TO_ROOM);
	return;
      }
      send_to_char( L"Kamufle olmaya çalışıyorsun.\n\r", ch );
    WAIT_STATE( ch, skill_table[gsn_camouflage].beats );

    if ( IS_AFFECTED(ch, AFF_CAMOUFLAGE) )
	REMOVE_BIT(ch->affected_by, AFF_CAMOUFLAGE);


    if ( IS_NPC(ch) ||
	number_percent( ) < get_skill(ch,gsn_camouflage) )
    {
	SET_BIT(ch->affected_by, AFF_CAMOUFLAGE);
	check_improve(ch,gsn_camouflage,TRUE,1);
    }
    else
	check_improve(ch,gsn_camouflage,FALSE,1);

    return;
}

/*
 * Contributed by Alander
 */
 void do_visible( CHAR_DATA *ch, wchar_t *argument )
 {
     if (IS_SET(ch->affected_by, AFF_HIDE))
       {
 	send_to_char( L"Gölgelerden çıkıyorsun.\n\r", ch );
 	REMOVE_BIT(ch->affected_by, AFF_HIDE);
 	act( L"$n gölgelerden çıkıyor.", ch, NULL, NULL, TO_ROOM);
       }
     if (IS_SET(ch->affected_by, AFF_FADE))
       {
 	send_to_char( L"Gölgelerden çıkıyorsun.\n\r", ch );
 	REMOVE_BIT(ch->affected_by, AFF_FADE);
 	act( L"$n gölgelerden çıkıyor.", ch, NULL, NULL, TO_ROOM);
       }
     if (IS_SET(ch->affected_by, AFF_CAMOUFLAGE))
       {
 	send_to_char( L"Örtünden sıyrılıyorsun.\n\r",ch);
 	REMOVE_BIT(ch->affected_by,AFF_CAMOUFLAGE);
 	act( L"$n örtüsünden sıyrılıyor.",ch,NULL,NULL,TO_ROOM);
       }
     if (IS_SET(ch->affected_by, AFF_INVISIBLE))
       {
 	send_to_char( L"Görünür oldun!\n\r", ch);
 	affect_strip(ch, gsn_invis);
 	affect_strip(ch, gsn_mass_invis);
 	REMOVE_BIT(ch->affected_by, AFF_INVISIBLE);
 	act( L"$n görünmeye başladı.", ch, NULL, NULL, TO_ROOM);
       }
     if (IS_SET(ch->affected_by, AFF_IMP_INVIS))
       {
 	send_to_char( L"Görünür oldun!\n\r", ch);
 	affect_strip(ch, gsn_imp_invis);
 	REMOVE_BIT(ch->affected_by, AFF_IMP_INVIS);
 	act( L"$n görünmeye başladı.", ch, NULL, NULL, TO_ROOM);
       }
     if (IS_SET(ch->affected_by, AFF_SNEAK)
 	&& !IS_NPC(ch) && !IS_SET(race_table[RACE(ch)].aff,AFF_SNEAK) )
       {
 	send_to_char( L"Adımların gürültü çıkarmaya başladı.\n\r", ch);
 	affect_strip(ch, gsn_sneak);
 	REMOVE_BIT(ch->affected_by, AFF_SNEAK);
       }

     affect_strip ( ch, gsn_mass_invis			);

     if ( CAN_DETECT(ch, ADET_EARTHFADE) )
     {
 	send_to_char( L"Doğal formuna dönüyorsun.\n\r", ch);
 	act( L"$n şekilleniyor.", ch, NULL, NULL, TO_ROOM);
 	affect_strip(ch, gsn_earthfade);
 	WAIT_STATE(ch, (PULSE_VIOLENCE / 2) );
 	REMOVE_BIT(ch->detection, ADET_EARTHFADE );
     }
     return;
 }

void do_recall( CHAR_DATA *ch, wchar_t *argument )
{
  wchar_t buf[MAX_STRING_LENGTH];
  CHAR_DATA *victim;
  ROOM_INDEX_DATA *location;
  int point;
  int lose,skill;

  if (IS_NPC(ch) && !IS_SET(ch->act,ACT_PET))
  {
    send_to_char( L"Yalnız oyuncular anımsama kullanabilir.\n\r",ch);
    return;
  }

  if (ch->level >= KIDEMLI_OYUNCU_SEVIYESI && !IS_IMMORTAL(ch) )
  {
    send_to_char( L"Seviyesi 16'dan düşük olanlar anımsama kullanabilir.\n\r",ch);
    return;
  }

  if (IS_GOOD(ch))
    point = hometown_table[ch->hometown].recall[0];
  else if (IS_EVIL(ch))
    point = hometown_table[ch->hometown].recall[2];
  else
    point = hometown_table[ch->hometown].recall[1];

  if (ch->desc == NULL && !IS_NPC(ch))
  {
    point =	hometown_table[number_range(0, 4)].recall[number_range(0,2)];
  }

  /* mud okulunda animsa yapan okulun girisine gitsin */
  if( ( ch->in_room->vnum >= 3700 ) && ( ch->in_room->vnum <= 3799 ) && ch->level < 6 )
  {
    point = ROOM_VNUM_SCHOOL;
  }
  else if ( ch->level < KIDEMLI_OYUNCU_SEVIYESI )
  {
    point = ROOM_VNUM_TEMPLE;
  }

  act( L"$n anımsama duası ediyor!", ch, 0, 0, TO_ROOM );

  if ( ( location = get_room_index(point ) )== NULL )
  {
    send_to_char( L"Kayboldun!\n\r", ch );
    return;
  }

  if ( ch->in_room == location )
  {
    return;
  }

    if ( ( IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL) || IS_AFFECTED(ch, AFF_CURSE) ||
        IS_RAFFECTED(ch->in_room, AFF_ROOM_CURSE) ) && ch->level >= KIDEMLI_OYUNCU_SEVIYESI )
    {
      send_to_char( L"Tanrılar seni terketti.\n\r", ch );
      return;
    }

    if ( ( victim = ch->fighting ) != NULL )
    {
      send_to_char( L"Hala dövüşüyorsun!\n\r", ch);

	if (IS_NPC(ch))
	    skill = 40 + ch->level;
	else
	    skill = get_skill(ch,gsn_recall);

	if ( number_percent() < 80 * skill / 100 )
	{
	    check_improve(ch,gsn_recall,FALSE,6);
	    WAIT_STATE( ch, 4 );
      swprintf( buf, MAX_STRING_LENGTH-1, L"Başaramadın.\n\r");
	    send_to_char( buf, ch );
	    return;
	}

	lose = 25;
	gain_exp( ch, 0 - lose );
	check_improve(ch,gsn_recall,TRUE,4);
  swprintf( buf, MAX_STRING_LENGTH-1, L"Dövüşürken anımsama kullandın!  %d TP kaybettin.\n\r", lose );
	send_to_char( buf, ch );
	stop_fighting( ch, TRUE );
    }

    ch->move /= 2;
    act( L"$n yokoluyor.", ch, NULL, NULL, TO_ROOM );
    char_from_room( ch );
    char_to_room( ch, location );
    act( L"$n ortaya çıkıyor.", ch, NULL, NULL, TO_ROOM );
    do_look( ch, (wchar_t*)"auto" );

    if (ch->pet != NULL)
      {
 	char_from_room( ch->pet );
	char_to_room( ch->pet, location );
	do_look(ch->pet, (wchar_t*)"auto" );
      }

    return;
}


void do_train( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t buf[MAX_STRING_LENGTH];
    CHAR_DATA *mob;
    sh_int stat = - 1;
    wchar_t *pOutput = NULL;
    int cost;

    if ( IS_NPC(ch) )
	return;

    /*
     * Check for trainer.
     */
    for ( mob = ch->in_room->people; mob; mob = mob->next_in_room )
    {
	if ( IS_NPC(mob) && ( IS_SET(mob->act, ACT_PRACTICE)
	|| IS_SET(mob->act,ACT_TRAIN) || IS_SET(mob->act,ACT_GAIN) ) )
	    break;
    }

    if ( mob == NULL )
    {
      send_to_char( L"Burada olmaz.\n\r", ch );
	return;
    }

    if ( argument[0] == '\0' )
    {
      swprintf( buf, MAX_STRING_LENGTH-1, L"%d eğitim seansın var.\n\r", ch->train );
	send_to_char( buf, ch );
	argument = (wchar_t*)"foo";
    }

    cost = 1;

    if ( !wcscasecmp( argument, L"güç" ) )
    {
	if ( class_table[ch->iclass].attr_prime == STAT_STR )
	    cost    = 1;
	stat        = STAT_STR;
	pOutput     = (wchar_t*)"Gücün";
    }

    else if ( !wcscasecmp( argument, L"zek" ) || !wcscasecmp( argument, L"zeka" ) )
    {
	if ( class_table[ch->iclass].attr_prime == STAT_INT )
	    cost    = 1;
	stat	    = STAT_INT;
	pOutput     = (wchar_t*)"Zekan";
    }

    else if ( !wcscasecmp( argument, L"bil" ) || !wcscasecmp( argument, L"bilgelik" ) )
    {
	if ( class_table[ch->iclass].attr_prime == STAT_WIS )
	    cost    = 1;
	stat	    = STAT_WIS;
	pOutput     = (wchar_t*)"Bilgeliğin";
    }

    else if ( !wcscasecmp( argument, L"çev" ) || !wcscasecmp( argument, L"çeviklik" ) )
    {
	if ( class_table[ch->iclass].attr_prime == STAT_DEX )
	    cost    = 1;
	stat  	    = STAT_DEX;
	pOutput     = (wchar_t*)"Çevikliğin";
    }

    else if ( !wcscasecmp( argument, L"bün" ) || !wcscasecmp( argument, L"bünye" ) )
    {
	if ( class_table[ch->iclass].attr_prime == STAT_CON )
	    cost    = 1;
	stat	    = STAT_CON;
	pOutput     = (wchar_t*)"Bünyen";
    }

    else if ( !wcscasecmp( argument, L"kar") || !wcscasecmp( argument, L"karizma") )
    {
	if ( class_table[ch->iclass].attr_prime == STAT_CHA )
	    cost    = 1;
	stat	    = STAT_CHA;
	pOutput     = (wchar_t*)"Karizman";
/*
	swprintf( buf, MAX_STRING_LENGTH-1,
 "You can't train charisma. That is about your behaviour.\n\r");
	send_to_char( buf, ch );
	return;
*/
    }

    else if ( !wcscasecmp(argument, L"yp" ) )
	cost = 1;

    else if ( !wcscasecmp(argument, L"mana" ) )
	cost = 1;

    else
    {
      wcscpy( buf, L"Eğitebileceklerin:" );
    	if ( ch->perm_stat[STAT_STR] < get_max_train(ch,STAT_STR))
    	    wcscat( buf, L" güç" );
    	if ( ch->perm_stat[STAT_INT] < get_max_train(ch,STAT_INT))
    	    wcscat( buf, L" zek" );
    	if ( ch->perm_stat[STAT_WIS] < get_max_train(ch,STAT_WIS))
    	    wcscat( buf, L" bil" );
    	if ( ch->perm_stat[STAT_DEX] < get_max_train(ch,STAT_DEX))
    	    wcscat( buf, L" çev" );
    	if ( ch->perm_stat[STAT_CON] < get_max_train(ch,STAT_CON))
    	    wcscat( buf, L" bün" );
    	if ( ch->perm_stat[STAT_CHA] < get_max_train(ch,STAT_CHA))
    	    wcscat(buf, L" kar" );
    	wcscat( buf, L" yp mana");

	if ( buf[MAX_INPUT_LENGTH-1] != ':' )
	{
	    wcscat( buf, L".\n\r" );
	    send_to_char( buf, ch );
	}
	else
	{
	    /*
	     * This message dedicated to Jordan ... you big stud!
	     */
       act( L"Eğitebileceğin birşeyin kalmadı",
 		ch, NULL,NULL,TO_CHAR );
	}

	return;
    }

    if (!wcscasecmp(L"yp",argument))
    {
    	if ( cost > ch->train )
    	{
        send_to_char( L"Yeterli eğitim seansın kalmadı.\n\r", ch );
            return;
        }

	ch->train -= cost;
        ch->pcdata->perm_hit += 10;
        ch->max_hit += 10;
        ch->hit +=10;
        act( L"Yaşam puanın artıyor!",ch,NULL,NULL,TO_CHAR);
        act( L"$s yaşam puanı artıyor!",ch,NULL,NULL,TO_ROOM);
        return;
    }

    if (!wcscasecmp(L"mana",argument))
    {
        if ( cost > ch->train )
        {
          send_to_char( L"Yeterli eğitim seansın kalmadı.\n\r", ch );
            return;
        }

	ch->train -= cost;
        ch->pcdata->perm_mana += 10;
        ch->max_mana += 10;
        ch->mana += 10;
        act( L"Manan artıyor!",ch,NULL,NULL,TO_CHAR);
        act( L"$s manası artıyor!",ch,NULL,NULL,TO_ROOM);
        return;
    }

    if ( ch->perm_stat[stat]  >= get_max_train(ch,stat) )
    {
      act( L"$T zaten maksimum.", ch, NULL, pOutput, TO_CHAR );
	return;
    }

    if ( cost > ch->train )
    {
      send_to_char( L"Yeterli eğitim seansın kalmadı.\n\r", ch );
	return;
    }

    ch->train		-= cost;

    ch->perm_stat[stat]		+= 1;
    act( L"$T artıyor!", ch, NULL, pOutput, TO_CHAR );
    act( L"$n $T eğitimi yapıyor!", ch, NULL, pOutput, TO_ROOM );
    return;
}



void do_track(CHAR_DATA *ch, wchar_t *argument)
{
  ROOM_HISTORY_DATA *rh;
  EXIT_DATA *pexit;
  wchar_t buf[MAX_STRING_LENGTH];
  static const wchar_t *door[] = { L"kuzeye",L"doğuya",L"güneye",L"batıya",L"yukarı",L"aşağı", L"o yöne" };
  int d;

  if (!IS_NPC(ch) && get_skill(ch,gsn_track) < 2) {
    send_to_char( L"Burada eğitim yapıldığına dair iz yok.\n\r",ch);
    return;
  }

  WAIT_STATE(ch,skill_table[gsn_track].beats);
  act( L"$n iz bulmak için yeri kontrol ediyor.",ch,NULL,NULL,TO_ROOM);

  if (IS_NPC(ch) || number_percent() < get_skill(ch,gsn_track) )
    {
      if (IS_NPC(ch))
	{
         ch->status = 0;
	 if (ch->last_fought != NULL && !IS_SET(ch->act,ACT_NOTRACK))
		add_mind(ch,ch->last_fought->name);
        }
      for (rh = ch->in_room->history;rh != NULL;rh = rh->next)
        if (is_name(argument,rh->name))
         {
          check_improve(ch,gsn_track,TRUE,1);
	  if ((d = rh->went) == -1) continue;
    swprintf( buf, MAX_STRING_LENGTH-1, L"%s kişisine ait izler %s uzanıyor.\n\r",capitalize(rh->name),door[d]);
        send_to_char(buf, ch);
	  if ( ( pexit = ch->in_room->exit[d] ) != NULL
	    &&   IS_SET(pexit->exit_info, EX_ISDOOR)
	    &&   pexit->keyword != NULL )
	    {
	     swprintf( buf, MAX_STRING_LENGTH-1, L"%s",door[d]);
	     do_open(ch,buf);
	    }
          move_char(ch,rh->went,FALSE);
          return;
      }
    }
    send_to_char( L"Hiç iz bulamıyorsun.\n\r",ch);
  if (IS_NPC(ch))
    ch->status = 5;	/* for stalker */
  check_improve(ch,gsn_track,FALSE,1);
}


void do_vampire( CHAR_DATA *ch, wchar_t *argument )
{
    AFFECT_DATA af;
    int level,duration;

    if ( is_affected( ch, gsn_vampire ) )
    {
      send_to_char( L"Daha fazla vampirleşemezsin!\n\r",ch);
     return;
    }

    if ( !IS_NPC(ch)
    &&   ch->level < skill_table[gsn_vampire].skill_level[ch->iclass] )
    {
	send_to_char( L"Daha çirkin görünmeye çalışıyorsun.\n\r", ch );
	return;
    }

    if ( get_skill(ch,gsn_vampire) < 50 )
	{
    send_to_char( L"Git ve görevciden yardım al.\n\r",ch);
	 return;
	}
    if ( is_affected(ch,gsn_vampire) )
	{
    send_to_char( L"Daha fazla vampir olmak istiyorsam git birilerini öldür.\n\r",ch);
	 return;
	}


    if ( weather_info.sunlight == SUN_LIGHT
	||   weather_info.sunlight == SUN_RISE )
	{
    send_to_char( L"Vampire dönüşmek için akşamı beklemelisin.\n\r",ch);
	}

    level = ch->level;
    duration = level / 10 ;
    duration += 5;

/* haste */
    af.where     = TO_AFFECTS;
    af.type      = gsn_vampire;
    af.level     = level;
    af.duration  = duration;
    af.location  = APPLY_DEX;
    af.modifier  = 1 + (level /20);
    af.bitvector = AFF_HASTE;
    affect_to_char( ch, &af );

/* giant strength + infrared */
    af.where     = TO_AFFECTS;
    af.type      = gsn_vampire;
    af.level     = level;
    af.duration  = duration;
    af.location  = APPLY_STR;
    af.modifier  = 1 + (level / 20);
    af.bitvector = AFF_INFRARED;
    affect_to_char( ch, &af );

/* size */
    af.where     = TO_AFFECTS;
    af.type      = gsn_vampire;
    af.level     = level;
    af.duration  = duration;
    af.location  = APPLY_SIZE;
    af.modifier  = 1 + (level / 50 );
    af.bitvector = AFF_SNEAK;
    affect_to_char( ch, &af );

/* damroll */
    af.where     = TO_AFFECTS;
    af.type      = gsn_vampire;
    af.level     = level;
    af.duration  = duration;
    af.location  = APPLY_DAMROLL;
    af.modifier  = ch->damroll;
    af.bitvector = AFF_BERSERK;
    affect_to_char( ch, &af );

/* negative immunity */
    af.where = TO_IMMUNE;
    af.type = gsn_vampire;
    af.duration = duration;
    af.level = level;
    af.location = APPLY_NONE;
    af.modifier = 0;
    af.bitvector = IMM_NEGATIVE;
    affect_to_char(ch, &af);

/* vampire flag */
    af.where     = TO_ACT_FLAG;
    af.type      = gsn_vampire;
    af.level     = level;
    af.duration  = duration;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = PLR_VAMPIRE;
    affect_to_char( ch, &af );

    send_to_char( L"İrileştiğini hissediyorsun.\n\r", ch );
    act( L"$m tanıyamıyorsun.",ch,NULL,NULL,TO_ROOM);
   return;
}

void do_vbite( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    wchar_t buf[MAX_STRING_LENGTH];

    one_argument( argument, arg );

    if ( !IS_NPC(ch)
    &&   ch->level < skill_table[gsn_vampiric_bite].skill_level[ch->iclass] )
      {
        send_to_char( L"Canlıları nasıl ısıracağını bilmiyorsun.\n\r",ch);
	return;
      }

    if (!IS_VAMPIRE(ch))
	{
    send_to_char( L"Isırmadan önce vampire dönüşmelisin.\n\r",ch);
	 return;
	}

    if ( arg[0] == '\0' )
    {
      send_to_char( L"Kimi ısıracaksın?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
      send_to_char( L"Burada değil.\n\r", ch);
	return;
    }

    if ( victim->position != POS_SLEEPING )
    {
      send_to_char( L"Uyuyor olmalı.\n\r", ch );
	return;
    }

    if ( (IS_NPC(ch)) && (!(IS_NPC(victim))) )
	return;


    if ( victim == ch )
    {
      send_to_char( L"Kendi boynunu nasıl ısıracaksın?\n\r", ch );
	return;
    }

    if ( is_safe( ch, victim ) )
      return;

    if ( victim->fighting != NULL )
    {
      send_to_char( L"Dövüşen birini ısıramazsın.\n\r", ch );
	return;
    }


    WAIT_STATE( ch, skill_table[gsn_vampiric_bite].beats );

    if ( victim->hit < (0.8 * victim->max_hit) &&
	 (IS_AWAKE(victim) ) )
    {
      act( L"$N yaralı ve kuşkulu... onu ısıramazsın.",
	    ch, NULL, victim, TO_CHAR );
	return;
    }

    if ( current_time-victim->last_fight_time<300 && IS_AWAKE(victim) )
    {
      act( L"$N yaralı ve kuşkulu... onu ısıramazsın.",
	    ch, NULL, victim, TO_CHAR );
	return;
    }

    if ( !IS_AWAKE(victim)
    && (  IS_NPC(ch)
    ||   number_percent( ) <
	( (get_skill(ch,gsn_vampiric_bite) * 0.7 ) +
	( 2 * (ch->level - victim->level ))  ) ) )
    {
	check_improve(ch,gsn_vampiric_bite,TRUE,1);
	one_hit( ch, victim, gsn_vampiric_bite,FALSE );
    }
    else
    {
	check_improve(ch,gsn_vampiric_bite,FALSE,1);
	damage( ch, victim, 0, gsn_vampiric_bite,DAM_NONE, TRUE );
    }
    /* Player shouts if he doesn't die */
    if (!(IS_NPC(victim)) && !(IS_NPC(ch))
	&& victim->position == POS_FIGHTING )
      {
        swprintf( buf, MAX_STRING_LENGTH-1, L"İmdat, bir vampir beni ısırmaya çalıştı!");
	    do_yell( victim, buf );
      }
    return;
}

void do_bash_door( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH];
    CHAR_DATA *gch;
    int chance=0;
    int damage_bash,door;

    one_argument(argument,arg);

    if ( get_skill(ch,gsn_bash_door) == 0
    ||	 (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_BASH))
    ||	 (!IS_NPC(ch)
    &&	  ch->level < skill_table[gsn_bash_door].skill_level[ch->iclass]))
    {
      send_to_char( L"Kapı kırmak? O da ne?\n\r",ch);
	return;
    }

    if (MOUNTED(ch))
    {
      send_to_char( L"Biniciyken nasıl kapı kıracaksın.\n\r", ch);
        return;
    }
    if (RIDDEN(ch))
    {
      send_to_char( L"Binekken nasıl kapı kıracaksın.\n\r", ch);
        return;
    }

    if (arg[0] == '\0')
    {
      send_to_char( L"Hangi yönü ya da kapıyı kıracaksın.\n\r",ch);
    return;
    }

    if (ch->fighting)
    {
      send_to_char( L"Dövüş bitene kadar bekle.\n\r",ch);
	return;
    }

    /* look for guards */
    for ( gch = ch->in_room->people; gch; gch = gch->next_in_room )
    {
	if ( IS_NPC(gch) && IS_AWAKE(gch) && ch->level + 5 < gch->level )
	{
    act( L"$N kapıya çok yakın duruyor.",
		ch, NULL, gch, TO_CHAR );
	    return;
	}
    }

    if ( ( door = find_door( ch, arg ) ) >= 0 )
    {
	/* 'bash door' */
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit = ch->in_room->exit[door];
  if ( !IS_SET(pexit->exit_info, EX_CLOSED) )
	    { send_to_char( L"Zaten açık.\n\r",ch ); return; }
	if ( !IS_SET(pexit->exit_info, EX_LOCKED) )
	    { send_to_char( L"Açmayı denesen?\n\r",ch ); return; }
	if ( IS_SET(pexit->exit_info, EX_NOPASS) )
	    { send_to_char( L"Mistik bir kalkan çıkışı koruyor.\n\r",ch );
	      return; }

    /* modifiers */

    /* size  and weight */
    chance += get_carry_weight(ch) / 100;

    chance += (ch->size - 2) * 20;

    /* stats */
    chance += get_curr_stat(ch,STAT_STR);

    if (IS_AFFECTED(ch,AFF_FLYING))
	chance -= 10;

    chance += ch->level / 15;

    chance += (get_skill(ch,gsn_bash_door) - 90);

    act( L"$d'ye yükleniyor ve kırmaya çalışıyorsun!",
		ch,NULL,pexit->keyword,TO_CHAR);
    act( L"$n $d'ye yükleniyor ve kırmaya çalışıyor!",
		ch,NULL,pexit->keyword,TO_ROOM);

    if (room_dark(ch->in_room))
		chance /= 2;

    /* now the attack */
    if (number_percent() < chance )
    {

	check_improve(ch,gsn_bash_door,TRUE,1);

	REMOVE_BIT(pexit->exit_info, EX_LOCKED);
	REMOVE_BIT(pexit->exit_info, EX_CLOSED);
  act( L"$n $d'ye yüklenerek kilidi kırıyor.", ch, NULL,
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


	if ( number_percent() < chance )
	{
		move_char(ch, door, FALSE);
		ch->position = POS_RESTING;
	}
	WAIT_STATE(ch,skill_table[gsn_bash_door].beats);

    }
    else
    {
      act( L"Yüzüstü yere kapaklandın!",
    	    ch,NULL,NULL,TO_CHAR);
    	act( L"$n yüzüstü yere kapaklandı.",
    	    ch,NULL,NULL,TO_ROOM);
	check_improve(ch,gsn_bash_door,FALSE,1);
	ch->position = POS_RESTING;
	WAIT_STATE(ch,skill_table[gsn_bash].beats * 3/2);
	damage_bash = ch->damroll + number_range(4,4 + 4* ch->size + chance/5);
	damage(ch,ch,damage_bash,gsn_bash, DAM_BASH, TRUE);
    }
    return;
    }
  return;
}

void do_blink( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH];
    wchar_t buf[MAX_STRING_LENGTH];

    argument = one_argument(argument , arg);

    if ( !IS_NPC(ch)
    &&   ch->level < skill_table[gsn_blink].skill_level[ch->iclass] )
    {
      send_to_char( L"Hı?.\n\r", ch);
	return;
    }

    if (arg[0] == '\0' )
    {
      swprintf( buf, MAX_STRING_LENGTH-1, L"Zayi durumu : %s.\n\r",
   	IS_BLINK_ON(ch) ? "AÇIK" : "KAPALI");
     send_to_char(buf,ch);
     return;
    }

    if ( !wcscasecmp(arg,L"açık") || !wcscasecmp(arg,L"aç") )
	{
	 SET_BIT(ch->act,PLR_BLINK_ON);
   swprintf( buf, MAX_STRING_LENGTH-1, L"Zayi artık AÇIK.\n\r");
	 send_to_char(buf,ch);
	 return;
	}

  if ( !wcscasecmp(arg,L"kapalı") || !wcscasecmp(arg,L"kapa") )
	{
	 REMOVE_BIT(ch->act,PLR_BLINK_ON);
   swprintf( buf, MAX_STRING_LENGTH-1, L"Zayi artık KAPALI.\n\r");
	 send_to_char(buf,ch);
	 return;
	}

  swprintf( buf, MAX_STRING_LENGTH-1, L"%s da nedir? Bir durum mu?\n\r",arg);
   send_to_char(buf,ch);
   return;
}

void do_vanish( CHAR_DATA *ch, wchar_t *argument )
{
  ROOM_INDEX_DATA *pRoomIndex;
  int i;

  if (!IS_NPC(ch)
      && ch->level < skill_table[gsn_vanish].skill_level[ch->iclass] )
	{
    send_to_char( L"Hı?\n\r",ch);
	 return;
	}


  if (ch->mana < 25)
	{
    send_to_char( L"Yeterince manan yok.\n\r" , ch);
	 return;
	}

  ch->mana -= 25;

  WAIT_STATE( ch, skill_table[gsn_vanish].beats );

  if (number_percent() > get_skill(ch,gsn_vanish) )
    {
      send_to_char( L"Başaramadın.\n\r",ch );
      check_improve(ch,gsn_vanish,FALSE,1);
      return;
    }

  if ( ch->in_room == NULL
      || IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL)
      || cabal_area_check(ch) )
  {
    send_to_char( L"Başaramadın.\n\r",ch );
      return;
  }

  for ( i=0; i < 65535; i++)
    {
      if ( (pRoomIndex = get_room_index(number_range(0, 65535))) == NULL)
	continue;
      if ( can_see_room(ch,pRoomIndex)
		&& !room_is_private(pRoomIndex)
		&& ch->in_room->area == pRoomIndex->area)
	  break;
    }

  if (pRoomIndex == NULL)
  {
    send_to_char( L"Başaramadın.\n\r",ch );
      return;
  }

  act( L"$n küçük bir küreyi yere fırlatıyor.", ch, NULL, NULL, TO_ROOM );

  check_improve(ch,gsn_vanish,TRUE,1);

  if (!IS_NPC(ch) && ch->fighting != NULL && number_range(0,1) == 1)
  {
    send_to_char( L"Başaramadın.\n\r",ch);
    return;
  }

  act( L"$n gitti!",ch,NULL,NULL,TO_ROOM);

  char_from_room( ch );
  char_to_room( ch, pRoomIndex );
  act( L"$n ortaya çıkıyor.", ch, NULL, NULL, TO_ROOM );
  do_look( ch, (wchar_t*)"auto" );
  stop_fighting(ch,TRUE);
  return;
}

void do_detect_sneak( CHAR_DATA *ch, wchar_t *argument)
{
    AFFECT_DATA af;

    if ( is_affected(ch, gsn_detect_sneak) )
    {
      send_to_char( L"Zaten süzülenleri saptayabiliyorsun.\n\r",ch);
    }
    af.where	 = TO_DETECTS;
    af.type      = gsn_detect_sneak;
    af.level     = ch->level;
    af.duration  = ch->level / 10;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = DETECT_SNEAK;
    affect_to_char( ch, &af );
    send_to_char( L"Artık süzülenleri saptıyorsun.\n\r", ch );
    return;
}


void do_fade( CHAR_DATA *ch, wchar_t *argument )
{
    if (ch_skill_nok(ch,gsn_fade)) return;

    if (MOUNTED(ch))
    {
      send_to_char( L"Biniciyken solamazsın.\n\r", ch);
        return;
    }
    if (RIDDEN(ch))
    {
      send_to_char( L"Binekken solamazsın.\n\r", ch);
        return;
    }

    if (!cabal_ok(ch,gsn_fade)) return;
    send_to_char( L"Solmaya çalışıyorsun.\n\r", ch );

    SET_BIT(ch->affected_by, AFF_FADE);
    check_improve(ch,gsn_fade,TRUE,3);

    return;
}

void do_vtouch(CHAR_DATA *ch, wchar_t *argument)
{
    CHAR_DATA *victim;
    AFFECT_DATA af;

    if ( IS_NPC(ch) ||
	 ch->level < skill_table[gsn_vampiric_touch].skill_level[ch->iclass] )
    {
      send_to_char( L"Kurutucu dokunuşlar hakkında bilgin yok.\n\r",ch);
	return;
    }

    if (!IS_VAMPIRE(ch))
	{
    send_to_char( L"Bırak olsun.\n\r",ch);
	return;
	}


    if ( (victim = get_char_room(ch,argument)) == NULL)
      {
        send_to_char( L"Onu göremiyorsun.\n\r",ch);
	return;
      }

    if (ch==victim)
      {
        send_to_char( L"Bu kadar aptal olamazsın.\n\r",ch);
	return;
      }

    if ( IS_AFFECTED( ch, AFF_CHARM ) && victim == ch->leader)  {
      send_to_char( L"Efendini kurutmak istemiyorsun.\n\r",ch);
	return;
    }

    if ( IS_AFFECTED( victim, AFF_CHARM ) )  {
      send_to_char( L"Kurbanın zaten uyuyor.\n\r",ch);
	return;
    }

    if (is_affected(victim,gsn_blackguard) )
    {
      act( L"$N yeterince yaklaşmana izin vermiyor.", ch, NULL, victim, TO_CHAR );
        return;
    }

    if (is_affected(victim,gsn_vampiric_touch))
      return;

    if (is_safe(ch,victim))
      return;

    victim->last_fight_time = current_time;
    ch->last_fight_time = current_time;

    WAIT_STATE(ch,skill_table[gsn_vampiric_touch].beats);

    if (IS_NPC(ch) ||
	number_percent() < 0.85 * get_skill(ch,gsn_vampiric_touch))
      {
        act( L"Boyna yaptığın dokunuşla $M kabuslara sürüklüyorsun.",
      	    ch,NULL,victim,TO_CHAR);
      	act( L"$s boynuna yaptığı dokunuş seni kabuslara sürüklüyor.",
      	    ch,NULL,victim,TO_VICT);
      	act( L"$n $S boynuna yaptığı dokunuşla onu kabuslara sürüklüyor.",
      	    ch,NULL,victim,TO_NOTVICT);
	check_improve(ch,gsn_vampiric_touch,TRUE,1);

	af.type = gsn_vampiric_touch;
        af.where = TO_AFFECTS;
	af.level = ch->level;
	af.duration = ch->level / 50 + 1;
	af.location = APPLY_NONE;
	af.modifier = 0;
	af.bitvector = AFF_SLEEP;
	affect_join ( victim,&af );

	if (IS_AWAKE(victim))
	  victim->position = POS_SLEEPING;
      }
    else
      {
	wchar_t buf[MAX_STRING_LENGTH];

	damage(ch,victim,0,gsn_vampiric_touch,DAM_NONE, TRUE);
	check_improve(ch,gsn_vampiric_touch,FALSE,1);
        if (!can_see(victim, ch))
        do_yell(victim,  (wchar_t*)"İmdat! Biri beni boğazladı!");
        else
        {
          swprintf( buf, MAX_STRING_LENGTH-1, L"İmdat! %s bana saldırdı!",
                    (is_affected(ch,gsn_doppelganger)&& !IS_IMMORTAL(victim))?
                    ch->doppel->name : ch->name );
            if (!IS_NPC(victim)) do_yell(victim,buf);
        }
        af.type = gsn_blackguard;
        af.where = TO_AFFECTS;
        af.level = victim->level;
        af.duration = 2 + victim->level / 50;
        af.modifier = 0;
        af.bitvector = 0;
        af.location  = APPLY_NONE;
        affect_join(victim, &af);
      }
}

void do_fly( CHAR_DATA *ch, wchar_t *argument )
{
  wchar_t arg[MAX_INPUT_LENGTH];

    if (IS_NPC(ch)) return;

    argument = one_argument(argument,arg);
    if ( !wcscmp(arg, L"yukarı") )
    {
     if (IS_AFFECTED(ch,AFF_FLYING) )
	{
    send_to_char( L"Zaten uçuyorsun.\n\r",ch);
	 return;
	}
     if (is_affected(ch,gsn_fly)
	 || (race_table[RACE(ch)].aff & AFF_FLYING)
	 || affect_check_obj(ch,AFF_FLYING) )
	{
   	 SET_BIT(ch->affected_by,AFF_FLYING);
	 REMOVE_BIT(ch->act,PLR_CHANGED_AFF);
   send_to_char( L"Uçmaya başladın.\n\r",ch);
	}
   else
	{
    send_to_char( L"Uçmak için bir iksir ya da bir çift kanat bul.\n\r",ch);
	}
    }
    else if ( !wcscmp(arg, L"aşağı") )
    {
   if (IS_AFFECTED(ch,AFF_FLYING) )
	{
   	 REMOVE_BIT(ch->affected_by,AFF_FLYING);
	 SET_BIT(ch->act,PLR_CHANGED_AFF);
   send_to_char( L"Yavaşça konuyorsun.\n\r",ch);
	}
   else
	{
    send_to_char( L"Zaten yerdesin.\n\r",ch);
	 return;
	}
    }
   else
    {
      send_to_char( L"UÇ komutunu AŞAĞI veya YUKARI parametreleriyle kullanın.\n\r",ch);
      return;
    }

    WAIT_STATE( ch, skill_table[gsn_fly].beats );

   return;
}


void do_push( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t buf  [MAX_STRING_LENGTH];
    wchar_t arg1 [MAX_INPUT_LENGTH];
    wchar_t arg2 [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int percent;
    int door;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0')
    {
      send_to_char( L"Kimi hangi yöne iteceksin?\n\r", ch );
	return;
    }

    if (MOUNTED(ch))
    {
      send_to_char( L"Biniciyken itemezsin.\n\r", ch);
        return;
    }
    if (RIDDEN(ch))
    {
      send_to_char( L"Binekken itemezsin.\n\r", ch);
        return;
    }

    if ( IS_NPC(ch) && IS_SET(ch->affected_by, AFF_CHARM)
	&& (ch->master != NULL))
      {
        send_to_char( L"Birini itemeyecek kadar afallamışsın.\n\r", ch);
	return;
      }

    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
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
      send_to_char( L"Anlamsız.\n\r", ch );
	return;
    }

    if (is_safe(ch,victim))
	return;

    if (victim->position == POS_FIGHTING)
    {
      send_to_char( L"Dövüş bitene kadar bekle.\n\r",ch);
	return;
    }

if ( ( door = find_exit( ch, arg2 ) ) >= 0 )
 {
	/* 'push' */
    EXIT_DATA *pexit;

    if ( (pexit = ch->in_room->exit[door]) != NULL )
    {
     if ( IS_SET(pexit->exit_info, EX_ISDOOR) )
	{
          if ( IS_SET(pexit->exit_info, EX_CLOSED) )
          send_to_char( L"Yön kapalı.\n\r",      ch );
              else if ( IS_SET(pexit->exit_info, EX_LOCKED) )
    	     send_to_char( L"Yön kilitli.\n\r",     ch );
     	  return;
	}
    }

    if (CAN_DETECT(ch,ADET_WEB))
    {
      send_to_char( L"Ağlarla tutulmuşken ne yapmak istiyorsun??\n\r", ch );
    	act( L"$M ağlarla tutulmuşken $n aptalca bir hareketle onu itmeye çalışıyor.", ch, NULL, victim, TO_ROOM );
	return;
    }

    if (CAN_DETECT(victim,ADET_WEB))
    {
      act( L"$M itmeye çalışıyorsun fakat ağlar onu yerinde tutuyor.",
    	  ch, NULL, victim, TO_CHAR );
    	act( L"$n $M itmeye çalışıyor fakat ağlar $M yerinde tutuyor.",
    	  ch, NULL, victim, TO_ROOM );
	return;
    }

    ch->last_death_time = -1;

    WAIT_STATE( ch, skill_table[gsn_push].beats );
    percent  = number_percent( ) + ( IS_AWAKE(victim) ? 10 : -50 );
    percent += can_see( victim, ch ) ? -10 : 0;

    if ( /* ch->level + 5 < victim->level || */
         victim->position == POS_FIGHTING
    || ( !IS_NPC(ch) && percent > get_skill(ch,gsn_push) ) )
    {
	/*
	 * Failure.
	 */

	send_to_char( L"Oops.\n\r", ch );
        if ( !IS_AFFECTED( victim, AFF_SLEEP ) ) {
          victim->position= victim->position==POS_SLEEPING? POS_STANDING:
					victim->position;
          act( L"$n seni itmeyi denedi.\n\r", ch, NULL, victim,TO_VICT  );
        }
        act( L"$n $M itmeyi denedi.\n\r",  ch, NULL, victim,TO_NOTVICT);

        swprintf( buf, MAX_STRING_LENGTH-1, L"Ellerini benden uzak tut %s!",ch->name);

	if ( IS_AWAKE( victim ) )
	  	do_yell( victim, buf );
	if ( !IS_NPC(ch) )
	{
	    if ( IS_NPC(victim) )
	    {
	        check_improve(ch,gsn_push,FALSE,2);
		multi_hit( victim, ch, TYPE_UNDEFINED );
	    }
	}

	return;
    }


    swprintf( buf, MAX_STRING_LENGTH-1, L"$C$M %s yönüne ittin.$c",dir_name[door]);
      act_color(buf,ch,NULL,victim,TO_CHAR,POS_SLEEPING,CLR_YELLOW);
  	swprintf( buf, MAX_STRING_LENGTH-1, L"$C$n seni %s yönüne itti.$c", dir_name[door]);
      act_color(buf,ch,NULL,victim,TO_VICT,POS_SLEEPING,CLR_YELLOW);
  	swprintf( buf, MAX_STRING_LENGTH-1, L"$C$n $M %s yönüne itti.$c", dir_name[door]);
      act_color(buf,ch,NULL,victim,TO_NOTVICT,POS_SLEEPING,CLR_YELLOW);
      move_char( victim , door , FALSE );

    check_improve(ch,gsn_push,TRUE,1);
 }
   return;
}


void do_crecall( CHAR_DATA *ch, wchar_t *argument )
{
    CHAR_DATA *victim;
    ROOM_INDEX_DATA *location;
    int point = ROOM_VNUM_BATTLE;
    AFFECT_DATA af;

    if (ch_skill_nok(ch,gsn_cabal_recall))
	return;

    if (!cabal_ok(ch,gsn_cabal_recall))
	return;

    if ( is_affected(ch, gsn_cabal_recall) )
    {
      send_to_char( L"Şimdi olmaz.\n\r",ch);
    }

    if (ch->desc == NULL && !IS_NPC(ch))
      {
	point =	ROOM_VNUM_BATTLE;
      }

      act( L"$n anımsama için Öfke Kabalı lorduna dua ediyor!", ch, 0,0, TO_ROOM );

    if ( ( location = get_room_index(point ) )== NULL )
    {
      send_to_char( L"Tamamen kayboldun.\n\r", ch );
	return;
    }

    if ( ch->in_room == location )
	return;

    if ( IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL)
    ||   IS_AFFECTED(ch, AFF_CURSE)
    ||   IS_RAFFECTED(ch->in_room, AFF_ROOM_CURSE) )
    {
      send_to_char( L"Tanrılar seni terketti.\n\r", ch );
	return;
    }

    if ( ( victim = ch->fighting ) != NULL )
    {
      send_to_char( L"Hala dövüşüyorsun!.\n\r", ch);
	return;
    }

    if ( ch->mana < ( ch->max_mana * 0.3 ) )
    {
      send_to_char( L"Dua için yeterli enerjin yok.\n\r",ch);
	return;
    }

    ch->move /= 2;
    ch->mana /= 10;
    act( L"$n yokoluyor.", ch, NULL, NULL, TO_ROOM );
    char_from_room( ch );
    char_to_room( ch, location );
    act( L"$n beliriyor.", ch, NULL, NULL, TO_ROOM );
    do_look( ch, (wchar_t*)"auto" );

    if (ch->pet != NULL)
      {
 	char_from_room( ch->pet );
	char_to_room( ch->pet, location );
	do_look(ch->pet, (wchar_t*)"auto" );
      }

    af.type      = gsn_cabal_recall;
    af.level     = ch->level;
    af.duration  = ch->level / 6 + 15;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = 0;
    affect_to_char( ch, &af );

    return;
}

void do_escape( CHAR_DATA *ch, wchar_t *argument )
{
    ROOM_INDEX_DATA *was_in;
    ROOM_INDEX_DATA *now_in;
    CHAR_DATA *victim;
    wchar_t arg[MAX_INPUT_LENGTH];
    int door;


    if ( ( victim = ch->fighting ) == NULL )
    {
        if ( ch->position == POS_FIGHTING )
		{
            ch->position = POS_STANDING;
		}
        send_to_char( L"Kimseyle dövüşmüyorsun.\n\r", ch );
	return;
    }

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0')
    {
      send_to_char( L"Hangi yöne sıvışacaksın?\n\r", ch );
	return;
    }

    if (MOUNTED(ch))
    {
      send_to_char( L"Biniciyken sıvışamazsın.\n\r", ch);
        return;
    }
    if (RIDDEN(ch))
    {
      send_to_char( L"Binekken sıvışamazsın.\n\r", ch);
        return;
    }

    if ( !IS_NPC(ch) && ch->level <
			skill_table[gsn_escape].skill_level[ch->iclass] )
    {
      send_to_char( L"Kaçmayı dene, senin için daha uygun olabilir.\n\r", ch );
	return;
    }

    was_in = ch->in_room;
    if ( ( door = find_exit( ch, arg ) ) >= 0 )
     {
	EXIT_DATA *pexit;

	if ( ( pexit = was_in->exit[door] ) == 0
	||   pexit->u1.to_room == NULL
	|| ( IS_SET(pexit->exit_info, EX_CLOSED)
      && (!IS_AFFECTED(ch, AFF_PASS_DOOR) || IS_SET(pexit->exit_info,EX_NOPASS))
      &&   !IS_TRUSTED(ch,ANGEL) )
	|| ( IS_SET(pexit->exit_info , EX_NOFLEE) )
	|| ( IS_NPC(ch)
	&&   IS_SET(pexit->u1.to_room->room_flags, ROOM_NO_MOB) ) )
    	{
        send_to_char( L"Birşey o yöne sıvışmanı engelliyor.\n\r",ch);
	 return;
    	}

	if ( number_percent() > get_skill(ch,gsn_escape) )
    	{
        send_to_char( L"PANİK! Sıvışamadın!\n\r", ch );
	 check_improve(ch,gsn_escape,FALSE,1);
	 return;
    	}

	check_improve(ch,gsn_escape,TRUE,1);
	move_char( ch, door, FALSE );
	if ( ( now_in = ch->in_room ) == was_in )
	{
    send_to_char( L"O yöne ulaşamadın. Başka bir tanesini dene.\n\r",ch);
	 return;
	}

	ch->in_room = was_in;
  act( L"$n sıvıştı!", ch, NULL, NULL, TO_ROOM );
	ch->in_room = now_in;

	if ( !IS_NPC(ch) )
	{
    send_to_char( L"Dövüşten sıvıştın!  10 TP kaybettin.\n\r", ch );
	    gain_exp( ch, -10 );
	}
	else
	  ch->last_fought = NULL;  /* Once fled, the mob will not go after */

	stop_fighting( ch, TRUE );
	return;
     }
     else send_to_char( L"Yanlış yön seçtin.\n\r",ch);
    return;
}

void do_layhands(CHAR_DATA *ch, wchar_t *argument)
{
    CHAR_DATA *victim;
    AFFECT_DATA af;

    if ( IS_NPC(ch) ||
	 ch->level < skill_table[gsn_lay_hands].skill_level[ch->iclass] )
    {
      send_to_char( L"Dokunuşla iyileştirme yeteneğinden mahrumsun.\n\r",ch);
	return;
    }

    if ( (victim = get_char_room(ch,argument)) == NULL)
      {
        send_to_char( L"Onu göremiyorsun.\n\r",ch);
	return;
      }

    if ( is_affected(ch, gsn_lay_hands))
	{
    send_to_char( L"Yeterince konsantre olamadın.\n\r",ch);
	 return;
	}
    WAIT_STATE(ch,skill_table[gsn_lay_hands].beats);

    af.type = gsn_lay_hands;
    af.where = TO_AFFECTS;
    af.level = ch->level;
    af.duration = 2;
    af.location = APPLY_NONE;
    af.modifier = 0;
    af.bitvector = 0;
    affect_to_char ( ch, &af );

    victim->hit = UMIN( victim->hit + ch->level * 2, victim->max_hit );
    update_pos( victim );
    send_to_char( L"Sıcak bir his vücudunu sarıyor.\n\r", victim );
    if ( ch != victim )
    send_to_char( L"Tamam.\n\r", ch );
    check_improve(ch,gsn_lay_hands,TRUE,1);

}

int mount_success ( CHAR_DATA *ch, CHAR_DATA *mount, int canattack)
{
  int percent;
  int success;

  percent = number_percent() + (ch->level < mount->level ?
            (mount->level - ch->level) * 3 :
            (mount->level - ch->level) * 2);

  if (!ch->fighting)
    percent -= 25;

  if (!IS_NPC(ch) && IS_DRUNK(ch)) {
    percent += get_skill(ch,gsn_riding) / 2;
    send_to_char( L"Alkolün etkisinde olduğundan binicilik zor olabilir...\n\r", ch);
  }

  success = percent - get_skill(ch,gsn_riding);
  if( success <= 0 ) { /* Success */
    check_improve(ch, gsn_riding, TRUE, 1);
    return(1);
  } else {
    check_improve(ch, gsn_riding, FALSE, 1);
    if ( success >= 10 && MOUNTED(ch) == mount) {
      act( L"Kontrolünü yitirerek $S üzerinden düştün.", ch, NULL, mount, TO_CHAR);
      act( L"$n kontolünü yitirerek $S üzerinden düştü.", ch, NULL, mount, TO_ROOM);
      act( L"$n kontrolünü yitirerek üzerinden düştü.", ch, NULL, mount, TO_VICT);

      ch->riding = FALSE;
      mount->riding = FALSE;
      if (ch->position > POS_STUNNED)
		ch->position=POS_SITTING;

    /*  if (ch->hit > 2) { */
        ch->hit -= 5;
        update_pos(ch);

    }
    if ( success >= 40 && canattack) {
      act( L"$N davranışlarından hoşlanmadı.", ch, NULL, mount, TO_CHAR);
      act( L"$N $s davranışlarından hoşlanmadı.", ch, NULL, mount, TO_ROOM);
      act( L"$s davranışlarından hoşlanmıyorsun.", ch, NULL, mount, TO_VICT);

      act( L"$N hırlayarak sana saldırıyor!", ch, NULL, mount, TO_CHAR);
      act( L"$N hırlayarak $e saldırıyor!", ch, NULL, mount, TO_ROOM);
      act( L"Hırlayarak $e saldırıyorsun!", ch, NULL, mount, TO_VICT);

      damage(mount, ch, number_range(1, mount->level), gsn_kick,DAM_BASH,TRUE );

/*      multi_hit( mount, ch, TYPE_UNDEFINED ); */
    }
  }
  return(0);
}

/*
 * It is not finished yet to implement all.
 */
void do_mount( CHAR_DATA *ch, wchar_t *argument )
{
  wchar_t arg[MAX_INPUT_LENGTH];
  wchar_t buf[MAX_STRING_LENGTH];
  struct char_data *mount;

  argument = one_argument(argument, arg);

  if (IS_NPC(ch)) return;

  if (arg[0] == '\0' && ch->mount && ch->mount->in_room == ch->in_room)
  {
    mount = ch->mount;
  }
  else if (arg[0] == '\0')
  {
    send_to_char( L"Neye bineceksin?\n\r", ch);
    return;
  }

 if (!(mount = get_char_room(ch, arg)))
 {
   send_to_char( L"Öyle birşey görmüyorsun.\n\r",ch );
    return;
  }

  if (ch_skill_nok_nomessage(ch,gsn_riding))
  {
    send_to_char( L"Binek sürmeyi bilmiyorsun!\n\r", ch);
    return;
  }

  if ( !IS_NPC(mount)
	|| !IS_SET(mount->act,ACT_RIDEABLE)
        || IS_SET(mount->act,ACT_NOTRACK) )
  {
    send_to_char( L"Onu süremezsin.\n\r",ch);
    return;
  }

  if (mount->level - 5 > ch->level) {
    send_to_char( L"O süremeyeceğin kadar güçlü.", ch);
    return;
  }

  if( (mount->mount) && (!mount->riding) && (mount->mount != ch)) {
    swprintf( buf, MAX_STRING_LENGTH-1, L"%s %s'e bağlı, sana değil.\n\r", mount->short_descr,mount->mount->name);
    send_to_char(buf,ch);
    return;
  }

  if (mount->position < POS_STANDING) {
    send_to_char( L"Bineğin ayakta olmalı.\n\r", ch);
    return;
  }

  if (RIDDEN(mount)) {
    send_to_char( L"Onun bir binicisi var zaten.\n\r", ch);
    return;
  } else if (MOUNTED(ch)) {
    send_to_char( L"Zaten sürüyorsun.\n\r", ch);
    return;
  }

  if( !mount_success(ch, mount, TRUE) ) {
    send_to_char( L"Ona binmeyi beceremedin.\n\r", ch);
    return;
  }

  act( L"$S sırtına sıçrıyorsun.", ch, NULL, mount, TO_CHAR);
  act( L"$n $S sırtına sıçrıyor.", ch, NULL, mount, TO_NOTVICT);
  act( L"$n sırtına sıçrıyor!", ch, NULL, mount, TO_VICT);

  ch->mount = mount;
  ch->riding = TRUE;
  mount->mount = ch;
  mount->riding = TRUE;

  /* No sneaky people on mounts */
  affect_strip(ch, gsn_sneak);
  REMOVE_BIT(ch->affected_by, AFF_SNEAK);
  affect_strip(ch, gsn_hide);
  REMOVE_BIT(ch->affected_by, AFF_HIDE);
  affect_strip(ch, gsn_fade);
  REMOVE_BIT(ch->affected_by, AFF_FADE);
  affect_strip(ch, gsn_imp_invis);
  REMOVE_BIT(ch->affected_by, AFF_IMP_INVIS);
}

void do_dismount( CHAR_DATA *ch, wchar_t *argument )
{
  struct char_data *mount;

  if(MOUNTED(ch)) {
    mount = MOUNTED(ch);

    act( L"$Z iniyorsun.", ch, NULL, mount, TO_CHAR);
    act( L"$n $Z iniyor.", ch, NULL, mount, TO_NOTVICT);
    act( L"$n sırtından iniyor.", ch, NULL, mount, TO_VICT);

    ch->riding = FALSE;
    mount->riding = FALSE;
  }
  else
  {
    send_to_char( L"Sürülmüyorsun.\n\r", ch);
    return;
  }
}

int send_arrow( CHAR_DATA *ch, CHAR_DATA *victim,OBJ_DATA *arrow , int door, int chance ,int bonus)
{
	EXIT_DATA *pExit;
	ROOM_INDEX_DATA *dest_room;
	wchar_t buf[512];
        AFFECT_DATA *paf;
        int damroll=0,hitroll=0,sn;
	AFFECT_DATA af;
	int i;

	if (arrow->value[0] == WEAPON_SPEAR)  sn = gsn_spear;
	else sn = gsn_arrow;

        for ( paf = arrow->affected; paf != NULL; paf = paf->next )
         {
 	   if ( paf->location == APPLY_DAMROLL )
	      damroll += paf->modifier;
	   if ( paf->location == APPLY_HITROLL )
	      hitroll += paf->modifier;
         }

	dest_room = ch->in_room;
	chance += (hitroll + str_app[get_curr_stat(ch,STAT_STR)].tohit
			+ (get_curr_stat(ch,STAT_DEX) - 18)) * 2;
	damroll *= 10;
	for(i=0; i< 1000; i++)
	{
	 chance -= 10;
	 if ( victim->in_room == dest_room )
	 {
	  if (number_percent() < chance)
	  {
	   if ( check_obj_dodge(ch,victim,arrow,chance))
		return 0;
    act( L"$p seni vuruyor!", victim, arrow, NULL, TO_CHAR );
    act( L"Attığın $p $E vuruyor!", ch, arrow, victim, TO_CHAR );
    if (ch->in_room == victim->in_room)
       act( L"$s $p $M vuruyor!", ch, arrow, victim, TO_NOTVICT );
	   else
	   {
       act( L"$s $p $M vuruyor!", ch, arrow, victim, TO_ROOM );
	     act( L"$p $m vuruyor!", victim, arrow, NULL, TO_ROOM );
	   }
	   if (is_safe(ch,victim) ||
		(IS_NPC(victim) && IS_SET(victim->act,ACT_NOTRACK)) )
    	    {
            act( L"$p $e hasar vermeden yere düşüyor...",victim,arrow,NULL,TO_ALL);
        	 	act( L"$p $e hasar vermeden yere düşüyor...",ch,arrow,NULL,TO_CHAR);
    	 	obj_to_room(arrow,victim->in_room);
    	    }
	   else
	    {
	        int dam;

		dam = dice(arrow->value[1],arrow->value[2]);
		dam = number_range(dam, (3*dam));
		dam += damroll + bonus +
			(10 * str_app[get_curr_stat(ch,STAT_STR)].todam);

        	if (IS_WEAPON_STAT(arrow,WEAPON_POISON))
        	{
            	 int level;
            	 AFFECT_DATA *poison, af;

            	 if ((poison = affect_find(arrow->affected,gsn_poison)) == NULL)
                	level = arrow->level;
            	 else
                	level = poison->level;
            	 if (!saves_spell(level,victim,DAM_POISON))
            	 {
                 send_to_char( L"Damarlarında dolaşan zehri hissediyorsun.",victim);
                  act( L"$n $p üzerindeki zehirle zehirlendi.",
			victim,arrow,NULL,TO_ROOM);

                  af.where     = TO_AFFECTS;
                  af.type      = gsn_poison;
                  af.level     = level * 3/4;
                  af.duration  = level / 2;
                  af.location  = APPLY_STR;
                  af.modifier  = -1;
                  af.bitvector = AFF_POISON;
                  affect_join( victim, &af );
            	 }

        	}
        	if (IS_WEAPON_STAT(arrow,WEAPON_FLAMING))
        	{
            act( L"$p $m yakıyor.",victim,arrow,NULL,TO_ROOM);
            act( L"$p etini yakıyor.",victim,arrow,NULL,TO_CHAR);
            	 fire_effect( (void *) victim,arrow->level,dam,TARGET_CHAR);
	        }
        	if (IS_WEAPON_STAT(arrow,WEAPON_FROST))
	        {
            act( L"$p $m donduruyor.",victim,arrow,NULL,TO_ROOM);
            act( L"$p soğuk bir dokunuşla seni ince buzla kapladı.",
                victim,arrow,NULL,TO_CHAR);
	            cold_effect(victim,arrow->level,dam,TARGET_CHAR);
	        }
	        if (IS_WEAPON_STAT(arrow,WEAPON_SHOCKING))
	        {
            act( L"$p şimşeğiyle $m çarpıyor.",victim,arrow,NULL,TO_ROOM);
            act( L"$p tarafından şok edildin.",victim,arrow,NULL,TO_CHAR);
	            shock_effect(victim,arrow->level,dam,TARGET_CHAR);
	        }

		if ( dam > victim->max_hit / 10
			&& number_percent() < 50 )
		{
		  af.where     = TO_AFFECTS;
		  af.type      = sn;
		  af.level     = ch->level;
		  af.duration  = -1;
		  af.location  = APPLY_HITROLL;
		  af.modifier  = - (dam / 20);
		  if (IS_NPC(victim)) af.bitvector = 0;
			else af.bitvector = AFF_CORRUPTION;

		  affect_join( victim, &af );

		  obj_to_char(arrow,victim);
		  equip_char(victim,arrow,WEAR_STUCK_IN);
		}
	        else obj_to_room(arrow,victim->in_room);

		damage( ch, victim,dam,sn,DAM_PIERCE,TRUE );
		path_to_track(ch,victim,door);

	    }
	    return 1;
	  }
	  else {
    	 	  obj_to_room(arrow,victim->in_room);
          act( L"$p ayaklarının önünde yere saplanıyor!",victim,arrow,NULL, TO_ALL );
	          return 0;
	        }
	 }
 	 pExit = dest_room->exit[ door ];
	 if ( !pExit ) break;
	 else
	 {
	  dest_room = pExit->u1.to_room;
	  if ( dest_room->people )
		{
      swprintf( buf, MAX_STRING_LENGTH-1, L"$p %s yönünden odaya giriyor!",dir_name[rev_dir[door]]);
		 act(buf, dest_room->people, arrow, NULL, TO_ALL );
		}
	 }
	}
 return 0;
}


void do_shoot( CHAR_DATA *ch, wchar_t *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA *wield;
    OBJ_DATA *arrow;
    wchar_t arg1[512],arg2[512],buf[512];
    int chance,direction;
    int range = (ch->level / 10) + 1;

   if (IS_NPC(ch)) return; /* Mobs can't use bows */

   if (ch_skill_nok_nomessage(ch,gsn_bow))
	{
    send_to_char( L"Vurmayı bilmiyorsun.\n\r",ch);
	  return;
	}

   argument=one_argument( argument, arg1 );
   one_argument( argument, arg2 );

   if ( arg1[0] == '\0' || arg2[0] == '\0')
    {
      send_to_char( L"Hangi yönde kimi vuracaksın?\n\r", ch );
	return;
    }

    if (ch->fighting)
    {
      send_to_char( L"Konsantre olamıyorsun.\n\r",ch);
	return;
    }

   direction = find_exit( ch, arg1 );

   if (direction<0 || direction > 5)
	 return;

    if ( ( victim = find_char( ch, arg2, direction, range) ) == NULL )
    {
      send_to_char( L"Onu göremiyorsun.\n\r", ch);
	return;
    }

    if (!IS_NPC(victim) && victim->desc == NULL)
    {
	send_to_char( L"Bunu yapamazsın.\n\r", ch);
	return;
    }

    if ( victim == ch )
    {
      send_to_char( L"Çok anlamsız.\n\r", ch);
	return;
    }

    if (is_at_cabal_area(ch) || is_at_cabal_area(victim) )
    {
      send_to_char( L"Kabal bölgeleri yakınında olmaz.\n\r", ch );
	return;
    }

    if (is_safe(ch,victim))
    {
      swprintf( buf, MAX_STRING_LENGTH-1, L"%s tanrılar tarafından korunuyor.\n\r",victim->name);
	send_to_char(buf,ch);
	return;
    }

   if ( (wield = get_weapon_char(ch,WEAPON_BOW)) == NULL)
    	{
        send_to_char( L"Bir yaya ihtiyacın var!\n\r",ch);
	 return;
	}

   if ( (arrow= get_weapon_char(ch,WEAPON_ARROW)) == NULL)
    	{
        send_to_char( L"Bir oka ihtiyacın var!\n\r",ch);
	 return;
	}

    WAIT_STATE( ch, skill_table[gsn_bow].beats );

    chance = (get_skill(ch,gsn_bow) - 50) * 2;
    if (ch->position == POS_SLEEPING)
	chance += 40;
    if (ch->position == POS_RESTING)
	chance += 10;
    if (victim->position == POS_FIGHTING)
	chance -= 40;
    chance += GET_HITROLL(ch);

    swprintf( buf, MAX_STRING_LENGTH-1, L"%s'e $p atıyorsun.", dir_name[ direction ] );
      act( buf, ch, arrow, NULL, TO_CHAR );
  	swprintf( buf, MAX_STRING_LENGTH-1, L"$n %s'e $p atıyor.", dir_name[ direction ] );
      act( buf, ch, arrow, NULL, TO_ROOM );

    obj_from_char(arrow);
    send_arrow(ch,victim,arrow,direction,chance,dice(wield->value[1],wield->value[2]) );
    check_improve(ch,gsn_bow,TRUE,1);
}


wchar_t *find_way(CHAR_DATA *ch,ROOM_INDEX_DATA *rstart, ROOM_INDEX_DATA *rend)
{
 int direction;
 static wchar_t buf[1024];
 EXIT_DATA *pExit;
 wchar_t buf2[2];
 int i;

 swprintf( buf, MAX_STRING_LENGTH-1, L"Find: ");
 for(i=0; i<65535; i++)
 {
 if ( (rend == rstart))
	 return buf;
  if ( (direction = find_path(rstart->vnum,rend->vnum,ch,-40000,0)) == -1)
	{
	 wcscat(buf,L" BUGGY");
	 return buf;
	}
  if (direction < 0 || direction > 5)
	{
	 wcscat(buf,L" VERY BUGGY");
	 return buf;
	}
   buf2[0] = dir_name[direction][0];
   buf2[1] = '\0';
   wcscat(buf,buf2);
   /* find target room */
   pExit = rstart->exit[ direction ];
   if ( !pExit )
	{
	 wcscat(buf,L" VERY VERY BUGGY");
	 return buf;
	}
   else rstart = pExit->u1.to_room;
 }
 return buf;
}

void do_human( CHAR_DATA *ch, wchar_t *argument )
{
    if (ch->iclass != CLASS_VAMPIRE)
    {
      send_to_char( L"Hı?\n\r",ch);
     return;
    }

    if ( !IS_VAMPIRE(ch) )
    {
      send_to_char( L"Zaten insansın.\n\r",ch);
     return;
    }

   affect_strip(ch, gsn_vampire);
   REMOVE_BIT(ch->act,PLR_VAMPIRE);
   send_to_char( L"Gerçek ölçülerine dönüyorsun.\n\r", ch );
   return;
}


void do_throw_spear( CHAR_DATA *ch, wchar_t *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA *spear;
    wchar_t arg1[512],arg2[512],buf[512];
    int chance,direction;
    int range = (ch->level / 10) + 1;

   if (IS_NPC(ch)) return; /* Mobs can't shoot spears */

   if (ch_skill_nok_nomessage(ch,gsn_spear))
	{
    send_to_char( L"Mızrak fırlatmayı bilmiyorsun.\n\r",ch);
	  return;
	}

   argument=one_argument( argument, arg1 );
   one_argument( argument, arg2 );

   if ( arg1[0] == '\0' || arg2[0] == '\0')
    {
      send_to_char( L"Mızrağı hangi yöne ve kime fırlatacaksın?\n\r", ch );
	return;
    }

    if (ch->fighting)
    {
      send_to_char( L"Konsantre olamıyorsun.\n\r",ch);
	return;
    }

   direction = find_exit( ch, arg1 );

   if (direction<0 || direction > 5)
	{
    send_to_char( L"Hangi yöne ve kime fırlatacaksın?\n\r",ch);
	 return;
	}

    if ( ( victim = find_char( ch, arg2, direction, range) ) == NULL )
    {
      send_to_char( L"Onu göremiyorsun.\n\r", ch);
	return;
    }

    if (!IS_NPC(victim) && victim->desc == NULL)
    {
	send_to_char( L"Bunu yapamazsın.\n\r", ch);
	return;
    }

    if ( victim == ch )
    {
      send_to_char( L"Çok anlamsız.\n\r", ch);
	return;
    }

    if (is_at_cabal_area(ch) || is_at_cabal_area(victim) )
    {
      send_to_char( L"Kabal bölgeleri yakınında olmaz.\n\r", ch );
	return;
    }

    if (is_safe(ch,victim))
    {
      swprintf( buf, MAX_STRING_LENGTH-1, L"%s tanrılar tarafından korunuyor.\n\r",victim->name);
	send_to_char(buf,ch);
	return;
    }

   if ( (spear = get_weapon_char(ch,WEAPON_SPEAR)) == NULL)
    	{
        send_to_char( L"Fırlatmak için bir mızrağa ihtiyacın var!\n\r",ch);
	 return;
	}


    WAIT_STATE( ch, skill_table[gsn_spear].beats );

    chance = (get_skill(ch,gsn_spear) - 50) * 2;
    if (ch->position == POS_SLEEPING)
	chance += 40;
    if (ch->position == POS_RESTING)
	chance += 10;
    if (victim->position == POS_FIGHTING)
	chance -= 40;
    chance += GET_HITROLL(ch);

    swprintf( buf, MAX_STRING_LENGTH-1, L"%s'e $p fırlatıyorsun.", dir_name[ direction ] );
      act( buf, ch, spear, NULL, TO_CHAR );
      swprintf( buf, MAX_STRING_LENGTH-1, L"$n %s'e $p fırlatıyor.", dir_name[ direction ] );
      act( buf, ch, spear, NULL, TO_ROOM );

    obj_from_char(spear);
    send_arrow(ch,victim,spear,direction,chance,dice(spear->value[1],spear->value[2]) );
    check_improve(ch,gsn_spear,TRUE,1);
}

OBJ_DATA *get_weapon_char( CHAR_DATA *ch, int wType )
{
    OBJ_DATA *obj;

    if (ch == NULL)
	return NULL;

    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
	if ( (obj->wear_loc == WEAR_LEFT || obj->wear_loc == WEAR_RIGHT ||
	   obj->wear_loc == WEAR_BOTH) )
	{
	 if (obj->value[0] == wType) return obj;
	 else continue;
	}
    }

    return NULL;
}
