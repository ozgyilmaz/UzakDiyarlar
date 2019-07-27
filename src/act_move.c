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




const char *	dir_name	[]		=
{
    "north", "east", "south", "west", "up", "down"
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
int	find_door	args( ( CHAR_DATA *ch, char *arg ) );
int 	find_exit	args( ( CHAR_DATA *ch, char *arg ) );
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
    char buf[MAX_STRING_LENGTH];

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
	 send_to_char( "When you attempt to leave the room, you break the webs holding you tight.\n\r", ch );
	 act( "$n struggles against the webs which hold $m in place, and break it.", ch, NULL, NULL, TO_ROOM );
	}
	else 
	{
	 send_to_char( "You attempt to leave the room, but the webs hold you tight.\n\r", ch );
	 act( "$n struggles vainly against the webs which hold $m in place.", ch, NULL, NULL, TO_ROOM );
	 return; 
	}
    }

    if ( door < 0 || door > 5 )
    {
	bug( "Do_move: bad door %d.", door );
	return;
    }

    if (( IS_AFFECTED( ch, AFF_HIDE ) && !IS_AFFECTED(ch, AFF_SNEAK) )  
	|| ( IS_AFFECTED( ch, AFF_FADE ) && !IS_AFFECTED(ch, AFF_SNEAK) )  )
    {
	REMOVE_BIT(ch->affected_by, AFF_HIDE);
	REMOVE_BIT(ch->affected_by, AFF_FADE);
	send_to_char( "You step out of shadows.\n\r", ch );
	act( "$n steps out of shadows.", ch, NULL, NULL, TO_ROOM);
    }
    if ( IS_AFFECTED( ch, AFF_CAMOUFLAGE ) )  
    {
	if ( ch->level < skill_table[gsn_move_camf].skill_level[ch->iclass])
	     {
	      REMOVE_BIT(ch->affected_by, AFF_CAMOUFLAGE);
	      send_to_char( "You step out from your cover.\n\r", ch );
	      act("$n steps out from $m's cover.", ch, NULL, NULL,TO_ROOM);
	     }	    
	else if ( number_percent() < get_skill(ch,gsn_move_camf) )
	           check_improve(ch,gsn_move_camf,TRUE,5);
	else {
	      REMOVE_BIT(ch->affected_by, AFF_CAMOUFLAGE);
	      send_to_char( "You step out from your cover.\n\r", ch );
	      act("$n steps out from $m's cover.", ch, NULL, NULL,TO_ROOM);
	      check_improve(ch,gsn_move_camf,FALSE,5);
	     }	    
    }
    if ( CAN_DETECT(ch, ADET_EARTHFADE) )
    {
	send_to_char("You fade to your neutral form.\n\r", ch);
	act("Earth forms $n in front of you.", ch, NULL, NULL, TO_ROOM);
	affect_strip(ch, gsn_earthfade);
	WAIT_STATE(ch, (PULSE_VIOLENCE / 2) );
	REMOVE_BIT(ch->detection, ADET_EARTHFADE );
    }

    in_room = ch->in_room;
    if ( ( pexit   = in_room->exit[door] ) == NULL
    ||   ( to_room = pexit->u1.to_room   ) == NULL 
    ||	 !can_see_room(ch,pexit->u1.to_room))
    {
	send_to_char( "Alas, you cannot go that way.\n\r", ch );
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
  	  act( "You failed to pass through the $d.", ch, NULL, pexit->keyword, TO_CHAR);
	  act( "$n tries to pass through the $d, but $e fails", ch, NULL, pexit->keyword, TO_ROOM);
	}
	else
	  act( "The $d is closed.", ch, NULL, pexit->keyword, TO_CHAR );
        return;
    }

    if ( IS_AFFECTED(ch, AFF_CHARM)
    &&   ch->master != NULL
    &&   in_room == ch->master->in_room )
    {
	send_to_char( "What?  And leave your beloved master?\n\r", ch );
	return;
    }

/*    if ( !is_room_owner(ch,to_room) && room_is_private( to_room ) )	*/
    if ( room_is_private( to_room ) )
    {
	send_to_char( "That room is private right now.\n\r", ch );
	return;
    }

    if (MOUNTED(ch)) 
    {
        if (MOUNTED(ch)->position < POS_FIGHTING) 
        {
         send_to_char("Your mount must be standing.\n\r", ch);
         return; 
        }
        if (!mount_success(ch, MOUNTED(ch), FALSE)) 
        {
         send_to_char("Your mount subbornly refuses to go that way.\n\r", ch);
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
		      send_to_char( "You aren't allowed in there.\n\r", ch );
		      return;
		    }
                    if (ch->last_fight_time != -1 &&
                      current_time - ch->last_fight_time < FIGHT_DELAY_TIME)
                    {
                      send_to_char(
                           "You feel too bloody to go in there now.\n\r",ch);
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
                    send_to_char( "Your mount can't fly.\n\r", ch );
                    return;
                }
            } 
           else if ( !IS_AFFECTED(ch, AFF_FLYING) && !IS_IMMORTAL(ch))
	    {
	      send_to_char( "You can't fly.\n\r", ch );
	      return;
            } 
	}

	if (( in_room->sector_type == SECT_WATER_NOSWIM
	||    to_room->sector_type == SECT_WATER_NOSWIM )
	&&    (MOUNTED(ch) && !IS_AFFECTED(MOUNTED(ch),AFF_FLYING)) )
	{
	    send_to_char("You can't take your mount there.\n\r",ch);
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
		send_to_char( "You need a boat to go there.\n\r", ch );
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
	    send_to_char( "You are too exhausted.\n\r", ch );
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
	        sprintf(buf,"$n leaves, riding on %s.", 
			MOUNTED(ch)->short_descr );
            else sprintf(buf,"$n leaves.");
            check_improve(ch,gsn_quiet_movement,TRUE,1);
          }
	else 
	  {
           if (MOUNTED(ch))
		sprintf(buf,"$n leaves $T, riding on %s.",
			MOUNTED(ch)->short_descr );
  	   else sprintf(buf,"$n leaves $T." );
	  }
  	  act( buf, ch, NULL, dir_name[door], TO_ROOM );
     }

    if ( IS_AFFECTED(ch, AFF_CAMOUFLAGE) && to_room->sector_type != SECT_FIELD
	 && to_room->sector_type != SECT_FOREST &&
           to_room->sector_type != SECT_MOUNTAIN &&
           to_room->sector_type != SECT_HILLS ) 
     {
      REMOVE_BIT(ch->affected_by, AFF_CAMOUFLAGE);
      send_to_char( "You step out from your cover.\n\r", ch );
      act("$n steps out from $m's cover.", ch, NULL, NULL,TO_ROOM);
     }	    

    if ( (IS_AFFECTED( ch, AFF_HIDE ) )
	 && to_room->sector_type != SECT_FOREST
         && to_room->sector_type != SECT_FIELD )
    {
	REMOVE_BIT(ch->affected_by, AFF_HIDE);
	send_to_char( "You step out of shadows.\n\r", ch );
	act( "$n steps out of shadows.", ch, NULL, NULL, TO_ROOM);
    }

    mount = MOUNTED(ch);
    char_from_room( ch );
    char_to_room( ch, to_room );

    if (ch->in_room != to_room)
    {
	bug("Is char dead!",0);
	return;
    }

    /* room record for tracking */
    if (!IS_NPC(ch) && ch->in_room)
      room_record(ch->name,in_room, door);


    if ( !IS_AFFECTED(ch, AFF_SNEAK)
    &&   ch->invis_level < LEVEL_HERO)
      {
	if (mount)
	     act( "$n has arrived, riding $N.", ch, NULL, mount,TO_ROOM );
	else act( "$n has arrived.", ch, NULL, NULL, TO_ROOM );
      }

    do_look( ch, (char*)"auto" );

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
          room_has_pc = TRUE;
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
        (fch->pIndexData->mprogs->greet_prog) (fch,ch);
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
	    do_stand(fch,(char*)"");

	if ( fch->master == ch && fch->position == POS_STANDING 
	&&   can_see_room(fch,to_room))
	{

	    if (IS_SET(ch->in_room->room_flags,ROOM_LAW)
	    &&  (IS_NPC(fch) && IS_SET(fch->act,ACT_AGGRESSIVE)))
	    {
		act("You can't bring $N into the city.",
		    ch,NULL,fch,TO_CHAR);
		act("You aren't allowed in the city.",
		    fch,NULL,NULL,TO_CHAR);
		continue;
	    }

	    act( "You follow $N.", fch, NULL, ch, TO_CHAR );
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



void do_north( CHAR_DATA *ch, char *argument )
{
    move_char( ch, DIR_NORTH, FALSE );
    return;
}



void do_east( CHAR_DATA *ch, char *argument )
{
    move_char( ch, DIR_EAST, FALSE );
    return;
}



void do_south( CHAR_DATA *ch, char *argument )
{
    move_char( ch, DIR_SOUTH, FALSE );
    return;
}



void do_west( CHAR_DATA *ch, char *argument )
{
    move_char( ch, DIR_WEST, FALSE );
    return;
}



void do_up( CHAR_DATA *ch, char *argument )
{
    move_char( ch, DIR_UP, FALSE );
    return;
}



void do_down( CHAR_DATA *ch, char *argument )
{
    move_char( ch, DIR_DOWN, FALSE );
    return;
}

void do_run( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    int door,count;

    argument = one_argument( argument, arg1 );

    if ( arg1[0] == '\0' || arg2[0] == '\0')
    {
	send_to_char("run <direction> <count>\n\r", ch);
	return;
    }

    if ( ( door = find_exit(ch,arg1) ) < -1) return;
    if ( ( count = atoi( arg2 ) ) < 1 )
    {
	send_to_char("Count should be larger than 0.\n\r", ch);
	return;
    }

    return;
}



int find_exit( CHAR_DATA *ch, char *arg )
{
    int door;

	 if ( !str_cmp( arg, "n" ) || !str_cmp( arg, "north" ) ) door = 0;
    else if ( !str_cmp( arg, "e" ) || !str_cmp( arg, "east"  ) ) door = 1;
    else if ( !str_cmp( arg, "s" ) || !str_cmp( arg, "south" ) ) door = 2;
    else if ( !str_cmp( arg, "w" ) || !str_cmp( arg, "west"  ) ) door = 3;
    else if ( !str_cmp( arg, "u" ) || !str_cmp( arg, "up"    ) ) door = 4;
    else if ( !str_cmp( arg, "d" ) || !str_cmp( arg, "down"  ) ) door = 5;
    else
    {
	act( "I see no exit $T here.", ch, NULL, arg, TO_CHAR );
	return -1;
    }

    return door;
}


int find_door( CHAR_DATA *ch, char *arg )
{
    EXIT_DATA *pexit;
    int door;

	 if ( !str_cmp( arg, "n" ) || !str_cmp( arg, "north" ) ) door = 0;
    else if ( !str_cmp( arg, "e" ) || !str_cmp( arg, "east"  ) ) door = 1;
    else if ( !str_cmp( arg, "s" ) || !str_cmp( arg, "south" ) ) door = 2;
    else if ( !str_cmp( arg, "w" ) || !str_cmp( arg, "west"  ) ) door = 3;
    else if ( !str_cmp( arg, "u" ) || !str_cmp( arg, "up"    ) ) door = 4;
    else if ( !str_cmp( arg, "d" ) || !str_cmp( arg, "down"  ) ) door = 5;
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
	act( "I see no $T here.", ch, NULL, arg, TO_CHAR );
	return -1;
    }

    if ( ( pexit = ch->in_room->exit[door] ) == NULL )
    {
	act( "I see no door $T here.", ch, NULL, arg, TO_CHAR );
	return -1;
    }

    if ( !IS_SET(pexit->exit_info, EX_ISDOOR) )
    {
	send_to_char( "You can't do that.\n\r", ch );
	return -1;
    }

    return door;
}

/* scan.c */

const char * distance[4]=
{
"right here.", "nearby to the %s.", "not far %s.", "off in the distance %s."
};

void scan_list           args((ROOM_INDEX_DATA *scan_room, CHAR_DATA *ch,
                               sh_int depth, sh_int door));
void scan_char           args((CHAR_DATA *victim, CHAR_DATA *ch,
                               sh_int depth, sh_int door));
void do_scan2(CHAR_DATA *ch, char *argument)
{
   //extern   char *  const   dir_name        [];
   EXIT_DATA *pExit;
   sh_int door;

   act("$n looks all around.", ch, NULL, NULL, TO_ROOM);
   send_to_char("Looking around you see:\n\r", ch);
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
   //extern char *const dir_name[];
   //extern char *const distance[];
   char buf[MAX_INPUT_LENGTH], buf2[MAX_INPUT_LENGTH];

   buf[0] = '\0';

   strcat(buf, 
 	(is_affected(victim,gsn_doppelganger) && !IS_SET(ch->act,PLR_HOLYLIGHT))
		 ? PERS(victim->doppel, ch) : PERS(victim, ch));
   strcat(buf, ", ");
   sprintf(buf2, distance[depth], dir_name[door]);
   strcat(buf, buf2);
   strcat(buf, "\n\r");
 
   send_to_char(buf, ch);
   return;
}

void do_open( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int door;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Open what?\n\r", ch );
	return;
    }



    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    {
 	/* open portal */
	if (obj->item_type == ITEM_PORTAL)
	{
	    if (!IS_SET(obj->value[1], EX_ISDOOR))
	    {
		send_to_char("You can't do that.\n\r",ch);
		return;
	    }

	    if (!IS_SET(obj->value[1], EX_CLOSED))
	    {
		send_to_char("It's already open.\n\r",ch);
		return;
	    }

	    if (IS_SET(obj->value[1], EX_LOCKED))
	    {
		send_to_char("It's locked.\n\r",ch);
		return;
	    }

	    REMOVE_BIT(obj->value[1], EX_CLOSED);
	    act("You open $p.",ch,obj,NULL,TO_CHAR);
	    act("$n opens $p.",ch,obj,NULL,TO_ROOM);
	    return;
 	}

	/* 'open object' */
	if ( obj->item_type != ITEM_CONTAINER)
	    { send_to_char( "That's not a container.\n\r", ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSED) )
	    { send_to_char( "It's already open.\n\r",      ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSEABLE) )
	    { send_to_char( "You can't do that.\n\r",      ch ); return; }
	if ( IS_SET(obj->value[1], CONT_LOCKED) )
	    { send_to_char( "It's locked.\n\r",            ch ); return; }

	REMOVE_BIT(obj->value[1], CONT_CLOSED);
	act("You open $p.",ch,obj,NULL,TO_CHAR);
	act( "$n opens $p.", ch, obj, NULL, TO_ROOM );
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
	    { send_to_char( "It's already open.\n\r",      ch ); return; }
	if (  IS_SET(pexit->exit_info, EX_LOCKED) )
	    { send_to_char( "It's locked.\n\r",            ch ); return; }

	REMOVE_BIT(pexit->exit_info, EX_CLOSED);
	act( "$n opens the $d.", ch, NULL, pexit->keyword, TO_ROOM );
	send_to_char( "Ok.\n\r", ch );

	/* open the other side */
	if ( ( to_room   = pexit->u1.to_room            ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != NULL
	&&   pexit_rev->u1.to_room == ch->in_room )
	{
	    CHAR_DATA *rch;

	    REMOVE_BIT( pexit_rev->exit_info, EX_CLOSED );
	    for ( rch = to_room->people; rch != NULL; rch = rch->next_in_room )
		act( "The $d opens.", rch, NULL, pexit_rev->keyword, TO_CHAR );
	}
	return;
    }
    return;
}



void do_close( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int door;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Close what?\n\r", ch );
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
		send_to_char("You can't do that.\n\r",ch);
		return;
	    }

	    if (IS_SET(obj->value[1],EX_CLOSED))
	    {
		send_to_char("It's already closed.\n\r",ch);
		return;
	    }

	    SET_BIT(obj->value[1],EX_CLOSED);
	    act("You close $p.",ch,obj,NULL,TO_CHAR);
	    act("$n closes $p.",ch,obj,NULL,TO_ROOM);
	    return;
	}

	/* 'close object' */
	if ( obj->item_type != ITEM_CONTAINER )
	    { send_to_char( "That's not a container.\n\r", ch ); return; }
	if ( IS_SET(obj->value[1], CONT_CLOSED) )
	    { send_to_char( "It's already closed.\n\r",    ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSEABLE) )
	    { send_to_char( "You can't do that.\n\r",      ch ); return; }

	SET_BIT(obj->value[1], CONT_CLOSED);
	act("You close $p.",ch,obj,NULL,TO_CHAR);
	act( "$n closes $p.", ch, obj, NULL, TO_ROOM );
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
	    { send_to_char( "It's already closed.\n\r",    ch ); return; }

	SET_BIT(pexit->exit_info, EX_CLOSED);
	act( "$n closes the $d.", ch, NULL, pexit->keyword, TO_ROOM );
	send_to_char( "Ok.\n\r", ch );

	/* close the other side */
	if ( ( to_room   = pexit->u1.to_room            ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != 0
	&&   pexit_rev->u1.to_room == ch->in_room )
	{
	    CHAR_DATA *rch;

	    SET_BIT( pexit_rev->exit_info, EX_CLOSED );
	    for ( rch = to_room->people; rch != NULL; rch = rch->next_in_room )
		act( "The $d closes.", rch, NULL, pexit_rev->keyword, TO_CHAR );
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



void do_lock( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int door;
    CHAR_DATA *rch;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Lock what?\n\r", ch );
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
		send_to_char("You can't do that.\n\r",ch);
		return;
	    }
	    if (!IS_SET(obj->value[1],EX_CLOSED))
	    {
		send_to_char("It's not closed.\n\r",ch);
	 	return;
	    }

	    if (obj->value[4] < 0 || IS_SET(obj->value[1],EX_NOLOCK))
	    {
		send_to_char("It can't be locked.\n\r",ch);
		return;
	    }

	    if (!has_key(ch,obj->value[4]))
	    {
		send_to_char("You lack the key.\n\r",ch);
		return;
	    }

	    if (IS_SET(obj->value[1],EX_LOCKED))
	    {
		send_to_char("It's already locked.\n\r",ch);
		return;
	    }

	    SET_BIT(obj->value[1],EX_LOCKED);
	    act("You lock $p.",ch,obj,NULL,TO_CHAR);
	    act("$n locks $p.",ch,obj,NULL,TO_ROOM);
	    return;
	}

	/* 'lock object' */
	if ( obj->item_type != ITEM_CONTAINER )
	    { send_to_char( "That's not a container.\n\r", ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSED) )
	    { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( obj->value[2] < 0 )
	    { send_to_char( "It can't be locked.\n\r",     ch ); return; }
	if ( !has_key( ch, obj->value[2] ) )
	    { send_to_char( "You lack the key.\n\r",       ch ); return; }
	if ( IS_SET(obj->value[1], CONT_LOCKED) )
	    { send_to_char( "It's already locked.\n\r",    ch ); return; }

	SET_BIT(obj->value[1], CONT_LOCKED);
	act("You lock $p.",ch,obj,NULL,TO_CHAR);
	act( "$n locks $p.", ch, obj, NULL, TO_ROOM );
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
	    { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( pexit->key < 0 )
	    { send_to_char( "It can't be locked.\n\r",     ch ); return; }
	if ( !has_key( ch, pexit->key) && 
             !has_key_ground( ch, pexit->key) )
	    { send_to_char( "You lack the key.\n\r",       ch ); return; }
	if ( IS_SET(pexit->exit_info, EX_LOCKED) )
	    { send_to_char( "It's already locked.\n\r",    ch ); return; }

	SET_BIT(pexit->exit_info, EX_LOCKED);
	send_to_char( "*Click*\n\r", ch );
	act( "$n locks the $d.", ch, NULL, pexit->keyword, TO_ROOM );

	/* lock the other side */
	if ( ( to_room   = pexit->u1.to_room            ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != 0
	&&   pexit_rev->u1.to_room == ch->in_room )
	{
	    SET_BIT( pexit_rev->exit_info, EX_LOCKED );
	    for ( rch = to_room->people; rch != NULL; rch = rch->next_in_room )
		act( "The $d clicks.", rch, NULL, pexit_rev->keyword, TO_CHAR );

	}
        return;
    }
    return;
}



void do_unlock( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int door;
    CHAR_DATA *rch;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Unlock what?\n\r", ch );
	return;
    }


    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    {
 	/* portal stuff */
	if (obj->item_type == ITEM_PORTAL)
	{
	    if (IS_SET(obj->value[1],EX_ISDOOR))
	    {
		send_to_char("You can't do that.\n\r",ch);
		return;
	    }

	    if (!IS_SET(obj->value[1],EX_CLOSED))
	    {
		send_to_char("It's not closed.\n\r",ch);
		return;
	    }

	    if (obj->value[4] < 0)
	    {
		send_to_char("It can't be unlocked.\n\r",ch);
		return;
	    }

	    if (!has_key(ch,obj->value[4]))
	    {
		send_to_char("You lack the key.\n\r",ch);
		return;
	    }

	    if (!IS_SET(obj->value[1],EX_LOCKED))
	    {
		send_to_char("It's already unlocked.\n\r",ch);
		return;
	    }

	    REMOVE_BIT(obj->value[1],EX_LOCKED);
	    act("You unlock $p.",ch,obj,NULL,TO_CHAR);
	    act("$n unlocks $p.",ch,obj,NULL,TO_ROOM);
	    return;
	}

	/* 'unlock object' */
	if ( obj->item_type != ITEM_CONTAINER )
	    { send_to_char( "That's not a container.\n\r", ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSED) )
	    { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( obj->value[2] < 0 )
	    { send_to_char( "It can't be unlocked.\n\r",   ch ); return; }
	if ( !has_key( ch, obj->value[2] ) )
	    { send_to_char( "You lack the key.\n\r",       ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_LOCKED) )
	    { send_to_char( "It's already unlocked.\n\r",  ch ); return; }

	REMOVE_BIT(obj->value[1], CONT_LOCKED);
	act("You unlock $p.",ch,obj,NULL,TO_CHAR);
	act( "$n unlocks $p.", ch, obj, NULL, TO_ROOM );
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
	    { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( pexit->key < 0 )
	    { send_to_char( "It can't be unlocked.\n\r",   ch ); return; }
	if ( !has_key( ch, pexit->key) &&
	     !has_key_ground( ch, pexit->key) )
	    { send_to_char( "You lack the key.\n\r",       ch ); return; }
	if ( !IS_SET(pexit->exit_info, EX_LOCKED) )
	    { send_to_char( "It's already unlocked.\n\r",  ch ); return; }

	REMOVE_BIT(pexit->exit_info, EX_LOCKED);
	send_to_char( "*Click*\n\r", ch );
	act( "$n unlocks the $d.", ch, NULL, pexit->keyword, TO_ROOM );

	/* unlock the other side */
	if ( ( to_room   = pexit->u1.to_room            ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != NULL
	&&   pexit_rev->u1.to_room == ch->in_room )
	{
	    REMOVE_BIT( pexit_rev->exit_info, EX_LOCKED );
	    for ( rch = to_room->people; rch != NULL; rch = rch->next_in_room )
		act( "The $d clicks.", rch, NULL, pexit_rev->keyword, TO_CHAR );
	}
        return;
    }
    return;
}



void do_pick( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *gch;
    OBJ_DATA *obj;
    int door;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Pick what?\n\r", ch );
	return;
    }

    if (MOUNTED(ch)) 
    {
        send_to_char("You can't pick while mounted.\n\r", ch);
        return;
    }

    WAIT_STATE( ch, skill_table[gsn_pick_lock].beats );

    /* look for guards */
    for ( gch = ch->in_room->people; gch; gch = gch->next_in_room )
    {
	if ( IS_NPC(gch) && IS_AWAKE(gch) && ch->level + 5 < gch->level )
	{
	    act( "$N is standing too close to the lock.",
		ch, NULL, gch, TO_CHAR );
	    return;
	}
    }

    if ( !IS_NPC(ch) && number_percent( ) > get_skill(ch,gsn_pick_lock))
    {
	send_to_char( "You failed.\n\r", ch);
	check_improve(ch,gsn_pick_lock,FALSE,2);
	return;
    }

    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    {
	/* portal stuff */
	if (obj->item_type == ITEM_PORTAL)
	{
	    if (!IS_SET(obj->value[1],EX_ISDOOR))
	    {	
		send_to_char("You can't do that.\n\r",ch);
		return;
	    }

	    if (!IS_SET(obj->value[1],EX_CLOSED))
	    {
		send_to_char("It's not closed.\n\r",ch);
		return;
	    }

	    if (obj->value[4] < 0)
	    {
		send_to_char("It can't be unlocked.\n\r",ch);
		return;
	    }

	    if (IS_SET(obj->value[1],EX_PICKPROOF))
	    {
		send_to_char("You failed.\n\r",ch);
		return;
	    }

	    REMOVE_BIT(obj->value[1],EX_LOCKED);
	    act("You pick the lock on $p.",ch,obj,NULL,TO_CHAR);
	    act("$n picks the lock on $p.",ch,obj,NULL,TO_ROOM);
	    check_improve(ch,gsn_pick_lock,TRUE,2);
	    return;
	}

	
	/* 'pick object' */
	if ( obj->item_type != ITEM_CONTAINER )
	    { send_to_char( "That's not a container.\n\r", ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSED) )
	    { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( obj->value[2] < 0 )
	    { send_to_char( "It can't be unlocked.\n\r",   ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_LOCKED) )
	    { send_to_char( "It's already unlocked.\n\r",  ch ); return; }
	if ( IS_SET(obj->value[1], CONT_PICKPROOF) )
	    { send_to_char( "You failed.\n\r",             ch ); return; }

	REMOVE_BIT(obj->value[1], CONT_LOCKED);
        act("You pick the lock on $p.",ch,obj,NULL,TO_CHAR);
        act("$n picks the lock on $p.",ch,obj,NULL,TO_ROOM);
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
	    { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( pexit->key < 0 && !IS_IMMORTAL(ch))
	    { send_to_char( "It can't be picked.\n\r",     ch ); return; }
	if ( !IS_SET(pexit->exit_info, EX_LOCKED) )
	    { send_to_char( "It's already unlocked.\n\r",  ch ); return; }
	if ( IS_SET(pexit->exit_info, EX_PICKPROOF) && !IS_IMMORTAL(ch))
	    { send_to_char( "You failed.\n\r",             ch ); return; }

	REMOVE_BIT(pexit->exit_info, EX_LOCKED);
	send_to_char( "*Click*\n\r", ch );
	act( "$n picks the $d.", ch, NULL, pexit->keyword, TO_ROOM );
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




void do_stand( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj = NULL;

    if (argument[0] != '\0')
    {
	if (ch->position == POS_FIGHTING)
	{
	    send_to_char("Maybe you should finish fighting first?\n\r",ch);
	    return;
	}
	obj = get_obj_list(ch,argument,ch->in_room->contents);
	if (obj == NULL)
	{
	    send_to_char("You don't see that here.\n\r",ch);
	    return;
	}
	if (obj->item_type != ITEM_FURNITURE
	||  (!IS_SET(obj->value[2],STAND_AT)
	&&   !IS_SET(obj->value[2],STAND_ON)
	&&   !IS_SET(obj->value[2],STAND_IN)))
	{
	    send_to_char("You can't seem to find a place to stand.\n\r",ch);
	    return;
	}
	if (ch->on != obj && count_users(obj) >= obj->value[0])
	{
	    act_new("There's no room to stand on $p.",
		ch,obj,NULL,TO_ROOM,POS_DEAD);
	    return;
	}
    }
    switch ( ch->position )
    {
    case POS_SLEEPING:
	if ( IS_AFFECTED(ch, AFF_SLEEP) )
	    { send_to_char( "You can't wake up!\n\r", ch ); return; }
	
	if (obj == NULL)
	{
	    send_to_char( "You wake and stand up.\n\r", ch );
	    act( "$n wakes and stands up.", ch, NULL, NULL, TO_ROOM );
	    ch->on = NULL;
	}
	else if (IS_SET(obj->value[2],STAND_AT))
	{
	   act_new("You wake and stand at $p.",ch,obj,NULL,TO_CHAR,POS_DEAD);
	   act("$n wakes and stands at $p.",ch,obj,NULL,TO_ROOM);
	}
	else if (IS_SET(obj->value[2],STAND_ON))
	{
	    act_new("You wake and stand on $p.",ch,obj,NULL,TO_CHAR,POS_DEAD);
	    act("$n wakes and stands on $p.",ch,obj,NULL,TO_ROOM);
	}
	else 
	{
	    act_new("You wake and stand in $p.",ch,obj,NULL,TO_CHAR,POS_DEAD);
	    act("$n wakes and stands in $p.",ch,obj,NULL,TO_ROOM);
	}

        if (IS_HARA_KIRI(ch)) 
	{
	 send_to_char("You feel your blood heats your body.\n\r",ch);
	 REMOVE_BIT(ch->act,PLR_HARA_KIRI);
	}

	ch->position = POS_STANDING;
	do_look(ch,(char*)"auto");
	break;

    case POS_RESTING: case POS_SITTING:
	if (obj == NULL)
	{
	    send_to_char( "You stand up.\n\r", ch );
	    act( "$n stands up.", ch, NULL, NULL, TO_ROOM );
	    ch->on = NULL;
	}
	else if (IS_SET(obj->value[2],STAND_AT))
	{
	    act("You stand at $p.",ch,obj,NULL,TO_CHAR);
	    act("$n stands at $p.",ch,obj,NULL,TO_ROOM);
	}
	else if (IS_SET(obj->value[2],STAND_ON))
	{
	    act("You stand on $p.",ch,obj,NULL,TO_CHAR);
	    act("$n stands on $p.",ch,obj,NULL,TO_ROOM);
	}
	else
	{
	    act("You stand in $p.",ch,obj,NULL,TO_CHAR);
	    act("$n stands on $p.",ch,obj,NULL,TO_ROOM);
	}
	ch->position = POS_STANDING;
	break;

    case POS_STANDING:
	send_to_char( "You are already standing.\n\r", ch );
	break;

    case POS_FIGHTING:
	send_to_char( "You are already fighting!\n\r", ch );
	break;
    }

    return;
}



void do_rest( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj = NULL;

    if (ch->position == POS_FIGHTING)
    {
	send_to_char("You are already fighting!\n\r",ch);
	return;
    }

    if (MOUNTED(ch)) 
    {
        send_to_char("You can't rest while mounted.\n\r", ch);
        return;
    }
    if (RIDDEN(ch)) 
    {
        send_to_char("You can't rest while being ridden.\n\r", ch);
        return;
    }


    if ( IS_AFFECTED(ch, AFF_SLEEP) )
    { send_to_char( "You are already sleeping.\n\r", ch ); return; }

    /* okay, now that we know we can rest, find an object to rest on */
    if (argument[0] != '\0')
    {
	obj = get_obj_list(ch,argument,ch->in_room->contents);
	if (obj == NULL)
	{
	    send_to_char("You don't see that here.\n\r",ch);
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
	    send_to_char("You can't rest on that.\n\r",ch);
	    return;
    	}

        if (obj != NULL && ch->on != obj && count_users(obj) >= obj->value[0])
        {
	    act_new("There's no more room on $p.",ch,obj,NULL,TO_CHAR,POS_DEAD);
	    return;
    	}
	
	ch->on = obj;
    }

    switch ( ch->position )
    {
    case POS_SLEEPING:
	if (obj == NULL)
	{
	    send_to_char( "You wake up and start resting.\n\r", ch );
	    act ("$n wakes up and starts resting.",ch,NULL,NULL,TO_ROOM);
	}
	else if (IS_SET(obj->value[2],REST_AT))
	{
	    act_new("You wake up and rest at $p.",
		    ch,obj,NULL,TO_CHAR,POS_SLEEPING);
	    act("$n wakes up and rests at $p.",ch,obj,NULL,TO_ROOM);
	}
        else if (IS_SET(obj->value[2],REST_ON))
        {
            act_new("You wake up and rest on $p.",
                    ch,obj,NULL,TO_CHAR,POS_SLEEPING);
            act("$n wakes up and rests on $p.",ch,obj,NULL,TO_ROOM);
        }
        else
        {
            act_new("You wake up and rest in $p.",
                    ch,obj,NULL,TO_CHAR,POS_SLEEPING);
            act("$n wakes up and rests in $p.",ch,obj,NULL,TO_ROOM);
        }
	ch->position = POS_RESTING;
	break;

    case POS_RESTING:
	send_to_char( "You are already resting.\n\r", ch );
	break;

    case POS_STANDING:
	if (obj == NULL)
	{
	    send_to_char( "You rest.\n\r", ch );
	    act( "$n sits down and rests.", ch, NULL, NULL, TO_ROOM );
	}
        else if (IS_SET(obj->value[2],REST_AT))
        {
	    act("You sit down at $p and rest.",ch,obj,NULL,TO_CHAR);
	    act("$n sits down at $p and rests.",ch,obj,NULL,TO_ROOM);
        }
        else if (IS_SET(obj->value[2],REST_ON))
        {
	    act("You sit on $p and rest.",ch,obj,NULL,TO_CHAR);
	    act("$n sits on $p and rests.",ch,obj,NULL,TO_ROOM);
        }
        else
        {
	    act("You rest in $p.",ch,obj,NULL,TO_CHAR);
	    act("$n rests in $p.",ch,obj,NULL,TO_ROOM);
        }
	ch->position = POS_RESTING;
	break;

    case POS_SITTING:
	if (obj == NULL)
	{
	    send_to_char("You rest.\n\r",ch);
	    act("$n rests.",ch,NULL,NULL,TO_ROOM);
	}
        else if (IS_SET(obj->value[2],REST_AT))
        {
	    act("You rest at $p.",ch,obj,NULL,TO_CHAR);
	    act("$n rests at $p.",ch,obj,NULL,TO_ROOM);
        }
        else if (IS_SET(obj->value[2],REST_ON))
        {
	    act("You rest on $p.",ch,obj,NULL,TO_CHAR);
	    act("$n rests on $p.",ch,obj,NULL,TO_ROOM);
        }
        else
        {
	    act("You rest in $p.",ch,obj,NULL,TO_CHAR);
	    act("$n rests in $p.",ch,obj,NULL,TO_ROOM);
	}
	ch->position = POS_RESTING;

        if (IS_HARA_KIRI(ch)) 
	{
	 send_to_char("You feel your blood heats your body.\n\r",ch);
	 REMOVE_BIT(ch->act,PLR_HARA_KIRI);
	}

	break;
    }


    return;
}


void do_sit (CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj = NULL;

    if (ch->position == POS_FIGHTING)
    {
	send_to_char("Maybe you should finish this fight first?\n\r",ch);
	return;
    }
    if (MOUNTED(ch)) 
    {
        send_to_char("You can't sit while mounted.\n\r", ch);
        return;
    }
    if (RIDDEN(ch)) 
    {
        send_to_char("You can't sit while being ridden.\n\r", ch);
        return;
    }

    if ( IS_AFFECTED(ch, AFF_SLEEP) )
    { send_to_char( "You are already sleeping.\n\r", ch ); return; }

    /* okay, now that we know we can sit, find an object to sit on */
    if (argument[0] != '\0')
    {
	obj = get_obj_list(ch,argument,ch->in_room->contents);
	if (obj == NULL)
	{
    if ( IS_AFFECTED(ch, AFF_SLEEP) )
    { send_to_char( "You are already sleeping.\n\r", ch ); return; }
	    send_to_char("You don't see that here.\n\r",ch);
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
	    send_to_char("You can't sit on that.\n\r",ch);
	    return;
	}

	if (obj != NULL && ch->on != obj && count_users(obj) >= obj->value[0])
	{
	    act_new("There's no more room on $p.",ch,obj,NULL,TO_CHAR,POS_DEAD);
	    return;
	}

	ch->on = obj;
    }
    switch (ch->position)
    {
	case POS_SLEEPING:
            if (obj == NULL)
            {
            	send_to_char( "You wake and sit up.\n\r", ch );
            	act( "$n wakes and sits up.", ch, NULL, NULL, TO_ROOM );
            }
            else if (IS_SET(obj->value[2],SIT_AT))
            {
            	act_new("You wake and sit at $p.",ch,obj,NULL,TO_CHAR,POS_DEAD);
            	act("$n wakes and sits at $p.",ch,obj,NULL,TO_ROOM);
            }
            else if (IS_SET(obj->value[2],SIT_ON))
            {
            	act_new("You wake and sit on $p.",ch,obj,NULL,TO_CHAR,POS_DEAD);
            	act("$n wakes and sits at $p.",ch,obj,NULL,TO_ROOM);
            }
            else
            {
            	act_new("You wake and sit in $p.",ch,obj,NULL,TO_CHAR,POS_DEAD);
            	act("$n wakes and sits in $p.",ch,obj,NULL,TO_ROOM);
            }

	    ch->position = POS_SITTING;
	    break;
	case POS_RESTING:
	    if (obj == NULL)
		send_to_char("You stop resting.\n\r",ch);
	    else if (IS_SET(obj->value[2],SIT_AT))
	    {
		act("You sit at $p.",ch,obj,NULL,TO_CHAR);
		act("$n sits at $p.",ch,obj,NULL,TO_ROOM);
	    }

	    else if (IS_SET(obj->value[2],SIT_ON))
	    {
		act("You sit on $p.",ch,obj,NULL,TO_CHAR);
		act("$n sits on $p.",ch,obj,NULL,TO_ROOM);
	    }
	    ch->position = POS_SITTING;
	    break;
	case POS_SITTING:
	    send_to_char("You are already sitting down.\n\r",ch);
	    break;
	case POS_STANDING:
	    if (obj == NULL)
    	    {
		send_to_char("You sit down.\n\r",ch);
    	        act("$n sits down on the ground.",ch,NULL,NULL,TO_ROOM);
	    }
	    else if (IS_SET(obj->value[2],SIT_AT))
	    {
		act("You sit down at $p.",ch,obj,NULL,TO_CHAR);
		act("$n sits down at $p.",ch,obj,NULL,TO_ROOM);
	    }
	    else if (IS_SET(obj->value[2],SIT_ON))
	    {
		act("You sit on $p.",ch,obj,NULL,TO_CHAR);
		act("$n sits on $p.",ch,obj,NULL,TO_ROOM);
	    }
	    else
	    {
		act("You sit down in $p.",ch,obj,NULL,TO_CHAR);
		act("$n sits down in $p.",ch,obj,NULL,TO_ROOM);
	    }
    	    ch->position = POS_SITTING;
    	    break;
    }
    if (IS_HARA_KIRI(ch)) 
	{
	 send_to_char("You feel your blood heats your body.\n\r",ch);
	 REMOVE_BIT(ch->act,PLR_HARA_KIRI);
	}
    return;
}


void do_sleep( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj = NULL;

    if (MOUNTED(ch)) 
    {
        send_to_char("You can't sleep while mounted.\n\r", ch);
        return;
    }
    if (RIDDEN(ch)) 
    {
        send_to_char("You can't sleep while being ridden.\n\r", ch);
        return;
    }

    switch ( ch->position )
    {
    case POS_SLEEPING:
	send_to_char( "You are already sleeping.\n\r", ch );
	break;

    case POS_RESTING:
    case POS_SITTING:
    case POS_STANDING: 
	if (argument[0] == '\0' && ch->on == NULL)
	{
	    send_to_char( "You go to sleep.\n\r", ch );
	    act( "$n goes to sleep.", ch, NULL, NULL, TO_ROOM );
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
		send_to_char("You don't see that here.\n\r",ch);
		return;
	    }
	    if (obj->item_type != ITEM_FURNITURE
	    ||  (!IS_SET(obj->value[2],SLEEP_ON) 
	    &&   !IS_SET(obj->value[2],SLEEP_IN)
	    &&	 !IS_SET(obj->value[2],SLEEP_AT)))
	    {
		send_to_char("You can't sleep on that!\n\r",ch);
		return;
	    }

	    if (ch->on != obj && count_users(obj) >= obj->value[0])
	    {
		act_new("There is no room on $p for you.",
		    ch,obj,NULL,TO_CHAR,POS_DEAD);
		return;
	    }

	    ch->on = obj;
	    if (IS_SET(obj->value[2],SLEEP_AT))
	    {
		act("You go to sleep at $p.",ch,obj,NULL,TO_CHAR);
		act("$n goes to sleep at $p.",ch,obj,NULL,TO_ROOM);
	    }
	    else if (IS_SET(obj->value[2],SLEEP_ON))
	    {
	        act("You go to sleep on $p.",ch,obj,NULL,TO_CHAR);
	        act("$n goes to sleep on $p.",ch,obj,NULL,TO_ROOM);
	    }
	    else
	    {
		act("You go to sleep in $p.",ch,obj,NULL,TO_CHAR);
		act("$n goes to sleep in $p.",ch,obj,NULL,TO_ROOM);
	    }
	    ch->position = POS_SLEEPING;
	}
	break;

    case POS_FIGHTING:
	send_to_char( "You are already fighting!\n\r", ch );
	break;
    }

    return;
}



void do_wake( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
	{ do_stand( ch, argument ); return; }

    if ( !IS_AWAKE(ch) )
	{ send_to_char( "You are asleep yourself!\n\r",       ch ); return; }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
	{ send_to_char( "They aren't here.\n\r",              ch ); return; }

    if ( IS_AWAKE(victim) )
	{ act( "$N is already awake.", ch, NULL, victim, TO_CHAR ); return; }

    if ( IS_AFFECTED(victim, AFF_SLEEP) )
	{ act( "You can't wake $M!",   ch, NULL, victim, TO_CHAR );  return; }

    act_new( "$n wakes you.", ch, NULL, victim, TO_VICT,POS_SLEEPING );
    do_stand(victim,(char*)"");
    return;
}



void do_sneak( CHAR_DATA *ch, char *argument )
{
    AFFECT_DATA af;

    if (MOUNTED(ch)) 
    {
        send_to_char("You can't sneak while mounted.\n\r", ch);
        return;
    }

    send_to_char( "You attempt to move silently.\n\r", ch );
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



void do_hide( CHAR_DATA *ch, char *argument )
{
int forest;

    if (MOUNTED(ch)) 
    {
        send_to_char("You can't hide while mounted.\n\r", ch);
        return;
    }

    if (RIDDEN(ch)) 
    {
        send_to_char("You can't hide while being ridden.\n\r", ch);
        return;
    }

    if ( IS_AFFECTED( ch, AFF_FAERIE_FIRE )  )  {
      send_to_char( "You can not hide while glowing.\n\r", ch );
      return;
    }
    forest = 0;
    forest += ch->in_room->sector_type == SECT_FOREST?60:0;
    forest += ch->in_room->sector_type == SECT_FIELD?60:0;

    send_to_char( "You attempt to hide.\n\r", ch );

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

void do_camouflage( CHAR_DATA *ch, char *argument )
{

    if (MOUNTED(ch)) 
    {
        send_to_char("You can't camouflage while mounted.\n\r", ch);
        return;
    }
    if (RIDDEN(ch)) 
    {
        send_to_char("You can't camouflage while being ridden.\n\r", ch);
        return;
    }

    if ( IS_NPC(ch) || 
         ch->level < skill_table[gsn_camouflage].skill_level[ch->iclass] )
      {
	send_to_char("You don't know how to camouflage yourself.\n\r",ch);
	return;
      }

    if ( IS_AFFECTED( ch, AFF_FAERIE_FIRE ) )  {
      send_to_char( "You can't camouflage yourself while glowing.\n\r", ch );
      return;
    }

    if (ch->in_room->sector_type != SECT_FOREST &&
	ch->in_room->sector_type != SECT_HILLS  &&
	ch->in_room->sector_type != SECT_MOUNTAIN)
      {
	send_to_char("There is no cover here.\n\r",ch);
	act("$n tries to camouflage $mself against the lone leaf on the ground.",ch,NULL,NULL,TO_ROOM);
	return;
      }
    send_to_char( "You attempt to camouflage yourself.\n\r", ch );
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
void do_visible( CHAR_DATA *ch, char *argument )
{
    if (IS_SET(ch->affected_by, AFF_HIDE))
      {
	send_to_char("You step out of the shadows.\n\r",ch);
	REMOVE_BIT(ch->affected_by, AFF_HIDE);
	act("$n steps out of the shadows.", ch, NULL, NULL, TO_ROOM);
      }
    if (IS_SET(ch->affected_by, AFF_FADE))
      {
	send_to_char("You step out of the shadows.\n\r",ch);
	REMOVE_BIT(ch->affected_by, AFF_FADE);
	act("$n steps out of the shadows.", ch, NULL, NULL, TO_ROOM);
      }
    if (IS_SET(ch->affected_by, AFF_CAMOUFLAGE))
      {
	send_to_char("You step out from your cover.\n\r",ch);
	REMOVE_BIT(ch->affected_by,AFF_CAMOUFLAGE);
	act("$n steps out from $s cover.",ch,NULL,NULL,TO_ROOM);
      }
    if (IS_SET(ch->affected_by, AFF_INVISIBLE))
      {
	send_to_char("You fade into existence.\n\r", ch);
	affect_strip(ch, gsn_invis);
	affect_strip(ch, gsn_mass_invis);
	REMOVE_BIT(ch->affected_by, AFF_INVISIBLE);
	act("$n fades into existence.", ch, NULL, NULL, TO_ROOM);
      }
    if (IS_SET(ch->affected_by, AFF_IMP_INVIS))
      {
	send_to_char("You fade into existence.\n\r", ch);
	affect_strip(ch, gsn_imp_invis);
	REMOVE_BIT(ch->affected_by, AFF_IMP_INVIS);
	act("$n fades into existence.", ch, NULL, NULL, TO_ROOM);
      }
    if (IS_SET(ch->affected_by, AFF_SNEAK) 
	&& !IS_NPC(ch) && !IS_SET(race_table[RACE(ch)].aff,AFF_SNEAK) )
      {
	send_to_char("You trample around loudly again.\n\r", ch);
	affect_strip(ch, gsn_sneak);
	REMOVE_BIT(ch->affected_by, AFF_SNEAK);
      }

    affect_strip ( ch, gsn_mass_invis			);

    if ( CAN_DETECT(ch, ADET_EARTHFADE) )
    {
	send_to_char("You fade to your neutral form.\n\r", ch);
	act("Earth forms $n in front of you.", ch, NULL, NULL, TO_ROOM);
	affect_strip(ch, gsn_earthfade);
	WAIT_STATE(ch, (PULSE_VIOLENCE / 2) );
	REMOVE_BIT(ch->detection, ADET_EARTHFADE );
    }
    return;
}

void do_recall( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    ROOM_INDEX_DATA *location;
    int point;
    int lose,skill;
 
    if (IS_GOOD(ch))      
      point = hometown_table[ch->hometown].recall[0];
    else if (IS_EVIL(ch))
      point = hometown_table[ch->hometown].recall[2];
    else
      point = hometown_table[ch->hometown].recall[1];

    if (IS_NPC(ch) && !IS_SET(ch->act,ACT_PET))
    {
	send_to_char("Only players can recall.\n\r",ch);
	return;
    }

    if (ch->level >= 11 && !IS_IMMORTAL(ch) )
      {
	sprintf(buf, "Recall is for only levels below 10.\n\r");
	send_to_char(buf,ch);
	return;
      }
/*
    if (ch->desc != NULL && current_time - ch->last_fight_time
	< FIGHT_DELAY_TIME)
      {
	send_to_char("You are too pumped to pray now.\n\r",ch);
	return;
      }
*/
    if (ch->desc == NULL && !IS_NPC(ch))
      {
	point =	hometown_table[number_range(0, 4)].recall[number_range(0,2)];
      }

    act( "$n prays for transportation!", ch, 0, 0, TO_ROOM );
    
    if ( ( location = get_room_index(point ) )== NULL )
    {
	send_to_char( "You are completely lost.\n\r", ch );
	return;
    }

    if ( ch->in_room == location )
	return;

    if ( IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL)
    ||   IS_AFFECTED(ch, AFF_CURSE) 
    ||   IS_RAFFECTED(ch->in_room, AFF_ROOM_CURSE) )
    {
	send_to_char( "The gods have forsaken you.\n\r", ch );
	return;
    }

    if ( ( victim = ch->fighting ) != NULL )
    {
	send_to_char( "You are still fighting!\n\r", ch );

	if (IS_NPC(ch))
	    skill = 40 + ch->level;
	else
	    skill = get_skill(ch,gsn_recall);

	if ( number_percent() < 80 * skill / 100 )
	{
	    check_improve(ch,gsn_recall,FALSE,6);
	    WAIT_STATE( ch, 4 );
	    sprintf( buf, "You failed!.\n\r");
	    send_to_char( buf, ch );
	    return;
	}

	lose = 25;
	gain_exp( ch, 0 - lose );
	check_improve(ch,gsn_recall,TRUE,4);
	sprintf( buf, "You recall from combat!  You lose %d exps.\n\r", lose );
	send_to_char( buf, ch );
	stop_fighting( ch, TRUE ); 
    }

    ch->move /= 2;
    act( "$n disappears.", ch, NULL, NULL, TO_ROOM );
    char_from_room( ch );
    char_to_room( ch, location );
    act( "$n appears in the room.", ch, NULL, NULL, TO_ROOM );
    do_look( ch, (char*)"auto" );
    
    if (ch->pet != NULL)
      {
 	char_from_room( ch->pet );
	char_to_room( ch->pet, location );
	do_look(ch->pet, (char*)"auto" );
      }

    return;
}


void do_train( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *mob;
    sh_int stat = - 1;
    char *pOutput = NULL;
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
	send_to_char( "You can't do that here.\n\r", ch );
	return;
    }

    if ( argument[0] == '\0' )
    {
	sprintf( buf, "You have %d training sessions.\n\r", ch->train );
	send_to_char( buf, ch );
	argument = (char*)"foo";
    }

    cost = 1;

    if ( !str_cmp( argument, "str" ) )
    {
	if ( class_table[ch->iclass].attr_prime == STAT_STR )
	    cost    = 1;
	stat        = STAT_STR;
	pOutput     = (char*)"strength";
    }

    else if ( !str_cmp( argument, "int" ) )
    {
	if ( class_table[ch->iclass].attr_prime == STAT_INT )
	    cost    = 1;
	stat	    = STAT_INT;
	pOutput     = (char*)"intelligence";
    }

    else if ( !str_cmp( argument, "wis" ) )
    {
	if ( class_table[ch->iclass].attr_prime == STAT_WIS )
	    cost    = 1;
	stat	    = STAT_WIS;
	pOutput     = (char*)"wisdom";
    }

    else if ( !str_cmp( argument, "dex" ) )
    {
	if ( class_table[ch->iclass].attr_prime == STAT_DEX )
	    cost    = 1;
	stat  	    = STAT_DEX;
	pOutput     = (char*)"dexterity";
    }

    else if ( !str_cmp( argument, "con" ) )
    {
	if ( class_table[ch->iclass].attr_prime == STAT_CON )
	    cost    = 1;
	stat	    = STAT_CON;
	pOutput     = (char*)"constitution";
    }

    else if ( !str_cmp( argument, "cha") )
    {
	if ( class_table[ch->iclass].attr_prime == STAT_CHA )
	    cost    = 1;
	stat	    = STAT_CHA;
	pOutput     = (char*)"charisma";
/*
	sprintf( buf,
 "You can't train charisma. That is about your behaviour.\n\r");
	send_to_char( buf, ch );
	return;
*/
    }

    else if ( !str_cmp(argument, "hp" ) )
	cost = 1;

    else if ( !str_cmp(argument, "mana" ) )
	cost = 1;

    else
    {
	strcpy( buf, "You can train:" );
	if ( ch->perm_stat[STAT_STR] < get_max_train2(ch,STAT_STR)) 
	    strcat( buf, " str" );
	if ( ch->perm_stat[STAT_INT] < get_max_train2(ch,STAT_INT))  
	    strcat( buf, " int" );
	if ( ch->perm_stat[STAT_WIS] < get_max_train2(ch,STAT_WIS)) 
	    strcat( buf, " wis" );
	if ( ch->perm_stat[STAT_DEX] < get_max_train2(ch,STAT_DEX))  
	    strcat( buf, " dex" );
	if ( ch->perm_stat[STAT_CON] < get_max_train2(ch,STAT_CON))  
	    strcat( buf, " con" );
	if ( ch->perm_stat[STAT_CHA] < get_max_train2(ch,STAT_CHA))  
	    strcat( buf, " cha" );
	strcat( buf, " hp mana");

	if ( buf[strlen(buf)-1] != ':' )
	{
	    strcat( buf, ".\n\r" );
	    send_to_char( buf, ch );
	}
	else
	{
	    /*
	     * This message dedicated to Jordan ... you big stud!
	     */
	    act( "You have nothing left to train, you $T!",
		ch, NULL,
		ch->sex == SEX_MALE   ? "big stud" :
		ch->sex == SEX_FEMALE ? "hot babe" :
					"wild thing",
		TO_CHAR );
	}

	return;
    }

    if (!str_cmp("hp",argument))
    {
    	if ( cost > ch->train )
    	{
       	    send_to_char( "You don't have enough training sessions.\n\r", ch );
            return;
        }
 
	ch->train -= cost;
        ch->pcdata->perm_hit += 10;
        ch->max_hit += 10;
        ch->hit +=10;
        act( "Your durability increases!",ch,NULL,NULL,TO_CHAR);
        act( "$n's durability increases!",ch,NULL,NULL,TO_ROOM);
        return;
    }
 
    if (!str_cmp("mana",argument))
    {
        if ( cost > ch->train )
        {
            send_to_char( "You don't have enough training sessions.\n\r", ch );
            return;
        }

	ch->train -= cost;
        ch->pcdata->perm_mana += 10;
        ch->max_mana += 10;
        ch->mana += 10;
        act( "Your power increases!",ch,NULL,NULL,TO_CHAR);
        act( "$n's power increases!",ch,NULL,NULL,TO_ROOM);
        return;
    }

    if ( ch->perm_stat[stat]  >= get_max_train2(ch,stat) )
    {
	act( "Your $T is already at maximum.", ch, NULL, pOutput, TO_CHAR );
	return;
    }

    if ( cost > ch->train )
    {
	send_to_char( "You don't have enough training sessions.\n\r", ch );
	return;
    }

    ch->train		-= cost;
  
    ch->perm_stat[stat]		+= 1;
    act( "Your $T increases!", ch, NULL, pOutput, TO_CHAR );
    act( "$n's $T increases!", ch, NULL, pOutput, TO_ROOM );
    return;
}



void do_track(CHAR_DATA *ch, char *argument)
{
  ROOM_HISTORY_DATA *rh;
  EXIT_DATA *pexit;
  char buf[MAX_STRING_LENGTH];
  static const char *door[] = { "north","east","south","west","up","down",
                            "that way" };
  int d;

  if (!IS_NPC(ch) && get_skill(ch,gsn_track) < 2) {
    send_to_char("There are no train tracks here.\n\r",ch);
    return;
  }

  WAIT_STATE(ch,skill_table[gsn_track].beats);
  act("$n checks the ground for tracks.",ch,NULL,NULL,TO_ROOM);

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
          sprintf(buf,"%s's tracks lead %s.\n\r",capitalize(rh->name),door[d]);
          send_to_char(buf, ch);
	  if ( ( pexit = ch->in_room->exit[d] ) != NULL
	    &&   IS_SET(pexit->exit_info, EX_ISDOOR)
	    &&   pexit->keyword != NULL )
	    {
	     sprintf(buf,"%s",door[d]);
	     do_open(ch,buf);
	    }
          move_char(ch,rh->went,FALSE);
          return;
      }
    }
  send_to_char("You don't see any tracks.\n\r",ch);
  if (IS_NPC(ch))
    ch->status = 5;	/* for stalker */
  check_improve(ch,gsn_track,FALSE,1);
}


void do_vampire( CHAR_DATA *ch, char *argument )
{
    AFFECT_DATA af;
    int level,duration;
 
    if ( is_affected( ch, gsn_vampire ) )
    {
     send_to_char("You can't be much more vampire!\n\r",ch);
     return;
    }

    if ( !IS_NPC(ch)
    &&   ch->level < skill_table[gsn_vampire].skill_level[ch->iclass] )
    {
	send_to_char(
	    "You try to show yourself even more uggly.\n\r", ch );
	return;
    }

    if ( get_skill(ch,gsn_vampire) < 50 )
	{
	 send_to_char("Go and ask the questor to help you.\n\r",ch);
	 return;
	}
    if ( is_affected(ch,gsn_vampire) )
	{
	 send_to_char("If you wan't to be more vampire go and kill a player.\n\r",ch);
	 return;
	}


    if ( weather_info.sunlight == SUN_LIGHT 
	||   weather_info.sunlight == SUN_RISE ) 
	{
	 send_to_char(
"You should have waited the evening or night to tranform to a vampire.\n\r",ch);
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

   send_to_char( "You feel yourself getting greater and greater.\n\r", ch );
   act("You cannot recognize $n anymore.",ch,NULL,NULL,TO_ROOM);
   return;
}

void do_vbite( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];

    one_argument( argument, arg );

    if ( !IS_NPC(ch)
    &&   ch->level < skill_table[gsn_vampiric_bite].skill_level[ch->iclass] )
      {
	send_to_char("You don't know how to bite creatures.\n\r",ch);
	return;
      }

    if (!IS_VAMPIRE(ch))
	{
	 send_to_char("You must transform vampire before biting.\n\r",ch);
	 return;
	}

    if ( arg[0] == '\0' )
    {
	send_to_char( "Bite whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim->position != POS_SLEEPING )
    {
	send_to_char( "They must be sleeping.\n\r", ch );
	return;
    }

    if ( (IS_NPC(ch)) && (!(IS_NPC(victim))) )
	return;
	

    if ( victim == ch )
    {
	send_to_char( "How can you sneak up on yourself?\n\r", ch );
	return;
    }

    if ( is_safe( ch, victim ) )
      return;

    if ( victim->fighting != NULL )
    {
	send_to_char( "You can't bite a fighting person.\n\r", ch );
	return;
    }


    WAIT_STATE( ch, skill_table[gsn_vampiric_bite].beats );

    if ( victim->hit < (0.8 * victim->max_hit) &&
	 (IS_AWAKE(victim) ) )
    {
	act( "$N is hurt and suspicious ... doesn't worth up.",
	    ch, NULL, victim, TO_CHAR );
	return;
    }

    if ( current_time-victim->last_fight_time<300 && IS_AWAKE(victim) )
    {
	act( "$N is suspicious ... it doesn't worth to do.",
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
  	    sprintf( buf, "Help, an ugly creature tried to bite me!");
	    do_yell( victim, buf );
      }
    return;
}

void do_bash_door( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *gch;
    int chance=0;
    int damage_bash,door;

    one_argument(argument,arg);
 
    if ( get_skill(ch,gsn_bash_door) == 0
    ||	 (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_BASH))
    ||	 (!IS_NPC(ch)
    &&	  ch->level < skill_table[gsn_bash_door].skill_level[ch->iclass]))
    {	
	send_to_char("Bashing? What's that?\n\r",ch);
	return;
    }
 
    if (MOUNTED(ch)) 
    {
        send_to_char("You can't bash doors while mounted.\n\r", ch);
        return;
    }
    if (RIDDEN(ch)) 
    {
        send_to_char("You can't bash doors while being ridden.\n\r", ch);
        return;
    }

    if (arg[0] == '\0')
    {
    send_to_char("Bash which door or direction.\n\r",ch);
    return;
    }

    if (ch->fighting)
    {	
	send_to_char("Wait until the fight finishes.\n\r",ch);
	return;
    }

    /* look for guards */
    for ( gch = ch->in_room->people; gch; gch = gch->next_in_room )
    {
	if ( IS_NPC(gch) && IS_AWAKE(gch) && ch->level + 5 < gch->level )
	{
	    act( "$N is standing too close to the door.",
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
	    { send_to_char( "It's already open.\n\r",      ch ); return; }
	if ( !IS_SET(pexit->exit_info, EX_LOCKED) )
	    { send_to_char( "Just try to open it.\n\r",     ch ); return; }
	if ( IS_SET(pexit->exit_info, EX_NOPASS) )
	    { send_to_char( "A mystical shield protects the exit.\n\r",ch ); 
	      return; }

    /* modifiers */

    /* size  and weight */
    chance += get_carry_weight(ch) / 100;

    chance += (ch->size - 2) * 20;

    /* stats */
    chance += get_curr_stat(ch,STAT_STR);

    if (IS_AFFECTED(ch,AFF_FLYING))
	chance -= 10;

    /* level 
    chance += ch->level / 10;
    */

    chance += (get_skill(ch,gsn_bash_door) - 90);

    act("You slam into $d, and try to break $d!",
		ch,NULL,pexit->keyword,TO_CHAR);
    act("You slam into $d, and try to break $d!",
		ch,NULL,pexit->keyword,TO_ROOM);

    if (room_dark(ch->in_room))
		chance /= 2;

    /* now the attack */
    if (number_percent() < chance )
    {
    
	check_improve(ch,gsn_bash_door,TRUE,1);

	REMOVE_BIT(pexit->exit_info, EX_LOCKED);
	REMOVE_BIT(pexit->exit_info, EX_CLOSED);
	act( "$n bashes the the $d and breaks the lock.", ch, NULL, 
		pexit->keyword, TO_ROOM );
	send_to_char( "You successed to open the door.\n\r", ch );

	/* open the other side */
	if ( ( to_room   = pexit->u1.to_room            ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != NULL
	&&   pexit_rev->u1.to_room == ch->in_room )
	{
	    CHAR_DATA *rch;

	    REMOVE_BIT( pexit_rev->exit_info, EX_CLOSED );
	    REMOVE_BIT( pexit_rev->exit_info, EX_LOCKED );
	    for ( rch = to_room->people; rch != NULL; rch = rch->next_in_room )
		act( "The $d opens.", rch, NULL, pexit_rev->keyword, TO_CHAR );
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
	act("You fall flat on your face!",
	    ch,NULL,NULL,TO_CHAR);
	act("$n falls flat on $s face.",
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

void do_blink( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    argument = one_argument(argument , arg);

    if ( !IS_NPC(ch)
    &&   ch->level < skill_table[gsn_blink].skill_level[ch->iclass] )
    {
	send_to_char("Huh?.\n\r", ch );
	return;
    }

    if (arg[0] == '\0' )
    {
     sprintf(buf,"Your current blink status : %s.\n\r",
	IS_BLINK_ON(ch) ? "ON" : "OFF");
     send_to_char(buf,ch);
     return;
    }

    if ( !str_cmp(arg,"ON") )
	{
	 SET_BIT(ch->act,PLR_BLINK_ON);
	 sprintf(buf,"Now ,your current blink status is ON.\n\r");
	 send_to_char(buf,ch);
	 return;
	}

    if ( !str_cmp(arg,"OFF") )
	{
	 REMOVE_BIT(ch->act,PLR_BLINK_ON);
	 sprintf(buf,"Now ,your current blink status is OFF.\n\r");
	 send_to_char(buf,ch);
	 return;
	}

   sprintf(buf,"What is that?.Is %s a status?\n\r",arg);
   send_to_char(buf,ch);
   return;
}

void do_vanish( CHAR_DATA *ch, char *argument )
{
  ROOM_INDEX_DATA *pRoomIndex;
  int i;

  if (!IS_NPC(ch)
      && ch->level < skill_table[gsn_vanish].skill_level[ch->iclass] )
	{
	 send_to_char("Huh?\n\r",ch);
	 return;
	}


  if (ch->mana < 25)
	{
	 send_to_char("You don't have enough power.\n\r" , ch);
	 return;
	}

  ch->mana -= 25;

  WAIT_STATE( ch, skill_table[gsn_vanish].beats );

  if (number_percent() > get_skill(ch,gsn_vanish) )
    {
      send_to_char( "You failed.\n\r", ch );
      check_improve(ch,gsn_vanish,FALSE,1);
      return;
    }
  
  if ( ch->in_room == NULL
      || IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL)
      || cabal_area_check(ch) )
  {
      send_to_char( "You failed.\n\r", ch );
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
      send_to_char( "You failed.\n\r", ch );
      return;
  }

  act( "$n throws down a small globe.", ch, NULL, NULL, TO_ROOM );

  check_improve(ch,gsn_vanish,TRUE,1);

  if (!IS_NPC(ch) && ch->fighting != NULL && number_bits(1) == 1) 
  {
    send_to_char("You failed.\n\r",ch);
    return;
  }

  act( "$n is gone!",ch,NULL,NULL,TO_ROOM);

  char_from_room( ch );
  char_to_room( ch, pRoomIndex );
  act( "$n appears from nowhere.", ch, NULL, NULL, TO_ROOM );
  do_look( ch, (char*)"auto" );
  stop_fighting(ch,TRUE);
  return;
}

void do_detect_sneak( CHAR_DATA *ch, char *argument) 
{
    AFFECT_DATA af;

    if ( is_affected(ch, gsn_detect_sneak) )
    {
	send_to_char("You can already detect sneaking.\n\r",ch);
    }
    af.where	 = TO_DETECTS;
    af.type      = gsn_detect_sneak;
    af.level     = ch->level;
    af.duration  = ch->level / 10;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = DETECT_SNEAK;
    affect_to_char( ch, &af );
    send_to_char( "You can detect the sneaking.\n\r", ch );
    return;
}


void do_fade( CHAR_DATA *ch, char *argument )
{
    if (ch_skill_nok(ch,gsn_fade)) return;

    if (MOUNTED(ch)) 
    {
        send_to_char("You can't fade while mounted.\n\r", ch);
        return;
    }
    if (RIDDEN(ch)) 
    {
        send_to_char("You can't fade while being ridden.\n\r", ch);
        return;
    }

    if (!cabal_ok(ch,gsn_fade)) return;
    send_to_char( "You attempt to fade.\n\r", ch );

    SET_BIT(ch->affected_by, AFF_FADE);
    check_improve(ch,gsn_fade,TRUE,3);

    return;
}

void do_vtouch(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    AFFECT_DATA af;

    if ( IS_NPC(ch) ||
	 ch->level < skill_table[gsn_vampiric_touch].skill_level[ch->iclass] )
    {
	send_to_char("You lack the skill to draining touch.\n\r",ch);
	return;
    }

    if (!IS_VAMPIRE(ch))
	{
	send_to_char("Let it be.\n\r",ch);
	return;
	}

    
    if ( (victim = get_char_room(ch,argument)) == NULL)
      {
	send_to_char("You do not see that person here.\n\r",ch);
	return;
      }

    if (ch==victim)
      {
	send_to_char("Even you are not that stupid.\n\r",ch);
	return;
      }

    if ( IS_AFFECTED( ch, AFF_CHARM ) && victim == ch->leader)  {
	send_to_char( "You don't want to drain your master.\n\r",ch);
	return;
    } 

    if ( IS_AFFECTED( victim, AFF_CHARM ) )  {
	send_to_char( "Your victim is already sleeping.\n\r",ch);
	return;
    } 

    if (is_affected(victim,gsn_blackguard) )
    {
        act( "$N's doesn't let you to go that much close.", ch, NULL, victim, TO_CHAR );
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
	act("You deadly touch  $N's neck and put $M to nightmares.",
	    ch,NULL,victim,TO_CHAR);
	act("$n's deadly touch your neck and puts you to nightmares.",
	    ch,NULL,victim,TO_VICT);
	act("$n's deadly touch $N's neck and puts $M to nightmares.",
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
	char buf[MAX_STRING_LENGTH];

	damage(ch,victim,0,gsn_vampiric_touch,DAM_NONE, TRUE);
	check_improve(ch,gsn_vampiric_touch,FALSE,1);
        if (!can_see(victim, ch))
          do_yell(victim, (char*)"Help! I'm being strangled by someone!");
        else
        {
            sprintf(buf, "Help! I'm being attacked by %s!",
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

void do_fly( CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];

    if (IS_NPC(ch)) return;

    argument = one_argument(argument,arg);
    if ( !str_cmp(arg,"up") )
    {
     if (IS_AFFECTED(ch,AFF_FLYING) )
	{		       
         send_to_char("You are already flying.\n\r",ch); 
	 return;
	}
     if (is_affected(ch,gsn_fly) 
	 || (race_table[RACE(ch)].aff & AFF_FLYING) 
	 || affect_check_obj(ch,AFF_FLYING) )
	{
   	 SET_BIT(ch->affected_by,AFF_FLYING);
	 REMOVE_BIT(ch->act,PLR_CHANGED_AFF);
   	 send_to_char("You start to fly.\n\r",ch);
	}
   else 
	{		       
         send_to_char("To fly , find a potion or wings.\n\r",ch); 
	}
    }
    else if ( !str_cmp(arg,"down") )
    {
   if (IS_AFFECTED(ch,AFF_FLYING) )
	{
   	 REMOVE_BIT(ch->affected_by,AFF_FLYING);
	 SET_BIT(ch->act,PLR_CHANGED_AFF);
   	 send_to_char("You slowly touch the ground.\n\r",ch);
	}
   else 
	{		       
         send_to_char("You are already on the ground.\n\r",ch); 
	 return;
	}
    }
   else 
    {
      send_to_char("Type fly with 'up' or 'down'.\n\r",ch);
      return;
    }

    WAIT_STATE( ch, skill_table[gsn_fly].beats );   

   return;
}
       

void do_push( CHAR_DATA *ch, char *argument )
{
    char buf  [MAX_STRING_LENGTH];
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int percent;
    int door;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0')
    {
	send_to_char( "Push whom to what diretion?\n\r", ch );
	return;
    }

    if (MOUNTED(ch)) 
    {
        send_to_char("You can't push while mounted.\n\r", ch);
        return;
    }
    if (RIDDEN(ch)) 
    {
        send_to_char("You can't push while being ridden.\n\r", ch);
        return;
    }

    if ( IS_NPC(ch) && IS_SET(ch->affected_by, AFF_CHARM) 
	&& (ch->master != NULL))
      {
	send_to_char( "You are to dazed to push anyone.\n\r", ch);
	return;
      }

    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if (!IS_NPC(victim) && victim->desc == NULL)
    {
	send_to_char("You can't do that.\n\r", ch);
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "That's pointless.\n\r", ch );
	return;
    }

    if (is_safe(ch,victim))
	return;

    if (victim->position == POS_FIGHTING)
    {
	send_to_char("Wait till the end of fight.\n\r",ch);
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
	    send_to_char( "Direction is closed.\n\r",      ch ); 
          else if ( IS_SET(pexit->exit_info, EX_LOCKED) )
	     send_to_char( "Direction is locked.\n\r",     ch ); 
     	  return;
	}
    }

    if (CAN_DETECT(ch,ADET_WEB))
    {
	send_to_char( "You're webbed, and want to do WHAT?!?\n\r", ch );
	act( "$n stupidly tries to push $N while webbed.", ch, NULL, victim, TO_ROOM );
	return; 
    }

    if (CAN_DETECT(victim,ADET_WEB))
    {
	act( "You attempt to push $N, but the webs hold $m in place.",
	  ch, NULL, victim, TO_CHAR );
	act( "$n attempts to push $n, but fails as the webs hold $n in place.",
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

	send_to_char( "Oops.\n\r", ch );
        if ( !IS_AFFECTED( victim, AFF_SLEEP ) ) {
          victim->position= victim->position==POS_SLEEPING? POS_STANDING:
					victim->position;
	  act( "$n tried to push you.\n\r", ch, NULL, victim,TO_VICT  );
        }
	act( "$n tried to push $N.\n\r",  ch, NULL, victim,TO_NOTVICT);

        sprintf(buf,"Keep your hands out of me, %s!",ch->name);

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


    sprintf(buf,"$CYou push $N to %s.$c",dir_name[door]);
    act_color(buf,ch,NULL,victim,TO_CHAR,POS_SLEEPING,CLR_YELLOW);
    sprintf(buf,"$C$n pushes you to %s.$c", dir_name[door]);
    act_color(buf,ch,NULL,victim,TO_VICT,POS_SLEEPING,CLR_YELLOW);
    sprintf(buf,"$C$n pushes $N to %s.$c", dir_name[door]);
    act_color(buf,ch,NULL,victim,TO_NOTVICT,POS_SLEEPING,CLR_YELLOW);
    move_char( victim , door , FALSE );

    check_improve(ch,gsn_push,TRUE,1);
 }
   return;
}


void do_crecall( CHAR_DATA *ch, char *argument )
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
	send_to_char("You can't pray now.\n\r",ch);
    }
/*
    if (ch->desc != NULL && current_time - ch->last_fight_time
	< FIGHT_DELAY_TIME) 
      {
	send_to_char("You are too pumped to pray now.\n\r",ch);
	return;
      }
*/
    if (ch->desc == NULL && !IS_NPC(ch))
      {
	point =	ROOM_VNUM_BATTLE;
      }

    act( "$n prays upper lord of Battleragers for transportation!", ch, 0,0, TO_ROOM );
    
    if ( ( location = get_room_index(point ) )== NULL )
    {
	send_to_char( "You are completely lost.\n\r", ch );
	return;
    }

    if ( ch->in_room == location )
	return;

    if ( IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL)
    ||   IS_AFFECTED(ch, AFF_CURSE) 
    ||   IS_RAFFECTED(ch->in_room, AFF_ROOM_CURSE) )
    {
	send_to_char( "The gods have forsaken you.\n\r", ch );
	return;
    }

    if ( ( victim = ch->fighting ) != NULL )
    {
	send_to_char("You are still fighting!.\n\r",ch);
	return;
    }

    if ( ch->mana < ( ch->max_mana * 0.3 ) )
    {
	send_to_char("You don't have enough power to pray now.\n\r",ch);
	return;
    }

    ch->move /= 2;
    ch->mana /= 10;
    act( "$n disappears.", ch, NULL, NULL, TO_ROOM );
    char_from_room( ch );
    char_to_room( ch, location );
    act( "$n appears in the room.", ch, NULL, NULL, TO_ROOM );
    do_look( ch, (char*)"auto" );
    
    if (ch->pet != NULL)
      {
 	char_from_room( ch->pet );
	char_to_room( ch->pet, location );
	do_look(ch->pet, (char*)"auto" );
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

void do_escape( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *was_in;
    ROOM_INDEX_DATA *now_in;
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    int door;


    if ( ( victim = ch->fighting ) == NULL )
    {
        if ( ch->position == POS_FIGHTING )
            ch->position = POS_STANDING;
	send_to_char( "You aren't fighting anyone.\n\r", ch );
	return;
    }

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0')
    {
	send_to_char( "Escape to what diretion?\n\r", ch );
	return;
    }

    if (MOUNTED(ch)) 
    {
        send_to_char("You can't escape while mounted.\n\r", ch);
        return;
    }
    if (RIDDEN(ch)) 
    {
        send_to_char("You can't escape while being ridden.\n\r", ch);
        return;
    }

    if ( !IS_NPC(ch) && ch->level <
			skill_table[gsn_escape].skill_level[ch->iclass] ) 
    {
	send_to_char( "Try flee. It may fit better to you.\n\r", ch );
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
	 send_to_char("Something prevents you to escape that direction.\n\r",ch);
	 return;
    	}

	if ( number_percent() > get_skill(ch,gsn_escape) )
    	{
         send_to_char( "PANIC! You couldn't escape!\n\r", ch );
	 check_improve(ch,gsn_escape,FALSE,1);	
	 return;
    	}

	check_improve(ch,gsn_escape,TRUE,1);	
	move_char( ch, door, FALSE );
	if ( ( now_in = ch->in_room ) == was_in )
	{
	 send_to_char("You couldn't reach that direction, try another.\n\r",ch);
	 return;
	}

	ch->in_room = was_in;
	act( "$n has escaped!", ch, NULL, NULL, TO_ROOM );
	ch->in_room = now_in;

	if ( !IS_NPC(ch) )
	{
	    send_to_char( "You escaped from combat!  You lose 10 exps.\n\r", ch );
	    gain_exp( ch, -10 );
	}
	else
	  ch->last_fought = NULL;  /* Once fled, the mob will not go after */

	stop_fighting( ch, TRUE );
	return;
     }
    else send_to_char("You chose the wrong direction.\n\r",ch);
    return;
}

void do_layhands(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    AFFECT_DATA af;

    if ( IS_NPC(ch) ||
	 ch->level < skill_table[gsn_lay_hands].skill_level[ch->iclass] )
    {
	send_to_char("You lack the skill to heal others with touching.\n\r",ch);
	return;
    }

    if ( (victim = get_char_room(ch,argument)) == NULL)
      {
	send_to_char("You do not see that person here.\n\r",ch);
	return;
      }

    if ( is_affected(ch, gsn_lay_hands))
	{
	 send_to_char("You can't concentrate enough.\n\r",ch);
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
    send_to_char( "A warm feeling fills your body.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
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
    send_to_char("Due to your being under the influence, riding seems a bit harder...\n\r", ch);
  }

  success = percent - get_skill(ch,gsn_riding);
  if( success <= 0 ) { /* Success */
    check_improve(ch, gsn_riding, TRUE, 1);
    return(1);
  } else {
    check_improve(ch, gsn_riding, FALSE, 1);
    if ( success >= 10 && MOUNTED(ch) == mount) {
      act("You lose control and fall off of $N.", ch, NULL, mount, TO_CHAR);
      act("$n loses control and falls off of $N.", ch, NULL, mount, TO_ROOM);
      act("$n loses control and falls off of you.", ch, NULL, mount, TO_VICT);

      ch->riding = FALSE;
      mount->riding = FALSE;
      if (ch->position > POS_STUNNED) 
		ch->position=POS_SITTING;
    
    /*  if (ch->hit > 2) { */
        ch->hit -= 5;
        update_pos(ch);
      
    }
    if ( success >= 40 && canattack) {
      act("$N doesn't like the way you've been treating $M.", ch, NULL, mount, TO_CHAR);
      act("$N doesn't like the way $n has been treating $M.", ch, NULL, mount, TO_ROOM);
      act("You don't like the way $n has been treating you.", ch, NULL, mount, TO_VICT);

      act("$N snarls and attacks you!", ch, NULL, mount, TO_CHAR);
      act("$N snarls and attacks $n!", ch, NULL, mount, TO_ROOM);
      act("You snarl and attack $n!", ch, NULL, mount, TO_VICT);  

      damage(mount, ch, number_range(1, mount->level), gsn_kick,DAM_BASH,TRUE );

/*      multi_hit( mount, ch, TYPE_UNDEFINED ); */
    }
  }
  return(0);
}

/*
 * It is not finished yet to implement all.
 */
void do_mount( CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];
  char buf[MAX_STRING_LENGTH];
  struct char_data *mount;

  argument = one_argument(argument, arg);

  if (IS_NPC(ch)) return;

  if (arg[0] == '\0' && ch->mount && ch->mount->in_room == ch->in_room) 
  {
    mount = ch->mount;
  } 
  else if (arg[0] == '\0')
  {
    send_to_char("Mount what?\n\r", ch);
    return;
  }

 if (!(mount = get_char_room(ch, arg))) 
 {
    send_to_char("You don't see that here.\n\r", ch);
    return;
  }
 
  if (ch_skill_nok_nomessage(ch,gsn_riding)) 
  {
    send_to_char("You don't know how to ride!\n\r", ch);
    return;
  } 

  if ( !IS_NPC(mount) 
	|| !IS_SET(mount->act,ACT_RIDEABLE)
        || IS_SET(mount->act,ACT_NOTRACK) ) 
  { 
    send_to_char("You can't ride that.\n\r",ch); 
    return;
  }
  
  if (mount->level - 5 > ch->level) {
    send_to_char("That beast is too powerful for you to ride.", ch);
    return;
  }    

  if( (mount->mount) && (!mount->riding) && (mount->mount != ch)) {
    sprintf(buf, "%s belongs to %s, not you.\n\r", mount->short_descr,
                   mount->mount->name);
    send_to_char(buf,ch);
    return;
  } 

  if (mount->position < POS_STANDING) {
    send_to_char("Your mount must be standing.\n\r", ch);
    return;
  }

  if (RIDDEN(mount)) {
    send_to_char("This beast is already ridden.\n\r", ch);
    return;
  } else if (MOUNTED(ch)) {
    send_to_char("You are already riding.\n\r", ch);
    return;
  }

  if( !mount_success(ch, mount, TRUE) ) {
    send_to_char("You fail to mount the beast.\n\r", ch);  
    return; 
  }

  act("You hop on $N's back.", ch, NULL, mount, TO_CHAR);
  act("$n hops on $N's back.", ch, NULL, mount, TO_NOTVICT);
  act("$n hops on your back!", ch, NULL, mount, TO_VICT);
 
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

void do_dismount( CHAR_DATA *ch, char *argument )
{
  struct char_data *mount;

  if(MOUNTED(ch)) {
    mount = MOUNTED(ch);

    act("You dismount from $N.", ch, NULL, mount, TO_CHAR);
    act("$n dismounts from $N.", ch, NULL, mount, TO_NOTVICT);
    act("$n dismounts from you.", ch, NULL, mount, TO_VICT);

    ch->riding = FALSE;
    mount->riding = FALSE;
  } 
  else 
  {
    send_to_char("You aren't mounted.\n\r", ch);
    return;
  }
} 

int send_arrow( CHAR_DATA *ch, CHAR_DATA *victim,OBJ_DATA *arrow , int door, int chance ,int bonus) 
{
	EXIT_DATA *pExit;
	ROOM_INDEX_DATA *dest_room;
	char buf[512];
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
	   act("$p strikes you!", victim, arrow, NULL, TO_CHAR );
	   act("Your $p strikes $N!", ch, arrow, victim, TO_CHAR );
	   if (ch->in_room == victim->in_room)
	      act("$n's $p strikes $N!", ch, arrow, victim, TO_NOTVICT );
	   else 
	   {
	     act("$n's $p strikes $N!", ch, arrow, victim, TO_ROOM );
	     act("$p strikes $n!", victim, arrow, NULL, TO_ROOM );
	   }
	   if (is_safe(ch,victim) || 
		(IS_NPC(victim) && IS_SET(victim->act,ACT_NOTRACK)) )
    	    {
    	 	act("$p falls from $n doing no visible damage...",victim,arrow,NULL,TO_ALL);
    	 	act("$p falls from $n doing no visible damage...",ch,arrow,NULL,TO_CHAR);
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
                  send_to_char("You feel poison coursing through your veins.",
                    victim);
                  act("$n is poisoned by the venom on $p.",
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
            	 act("$n is burned by $p.",victim,arrow,NULL,TO_ROOM);
            	 act("$p sears your flesh.",victim,arrow,NULL,TO_CHAR);
            	 fire_effect( (void *) victim,arrow->level,dam,TARGET_CHAR);
	        }
        	if (IS_WEAPON_STAT(arrow,WEAPON_FROST))
	        {
	            act("$p freezes $n.",victim,arrow,NULL,TO_ROOM);
	            act("The cold touch of $p surrounds you with ice.",
	                victim,arrow,NULL,TO_CHAR);
	            cold_effect(victim,arrow->level,dam,TARGET_CHAR);
	        }
	        if (IS_WEAPON_STAT(arrow,WEAPON_SHOCKING))
	        {
	            act("$n is struck by lightning from $p.",victim,arrow,NULL,TO_ROOM);
	            act("You are shocked by $p.",victim,arrow,NULL,TO_CHAR);
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
	          act("$p sticks in the ground at your feet!",victim,arrow,NULL, TO_ALL );
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
		 sprintf(buf,"$p sails into the room from the %s!",dir_name[rev_dir[door]]);
		 act(buf, dest_room->people, arrow, NULL, TO_ALL );
		}
	 }
	}
 return 0;
}
	

void do_shoot( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA *wield;
    OBJ_DATA *arrow; 
    char arg1[512],arg2[512],buf[512];
    int chance,direction;
    int range = (ch->level / 10) + 1;
    
   if (IS_NPC(ch)) return; /* Mobs can't use bows */

   if (ch_skill_nok_nomessage(ch,gsn_bow))
	{
	  send_to_char("You don't know how to shoot.\n\r",ch);
	  return;
	}

   argument=one_argument( argument, arg1 );
   one_argument( argument, arg2 );

   if ( arg1[0] == '\0' || arg2[0] == '\0')
    {
	send_to_char( "Shoot what diretion and whom?\n\r", ch );
	return;
    }

    if (ch->fighting)
    {
	send_to_char("You cannot concentrate on shooting arrows.\n\r",ch);
	return;
    }

   direction = find_exit( ch, arg1 );

   if (direction<0 || direction > 5) 
	 return;
	
    if ( ( victim = find_char( ch, arg2, direction, range) ) == NULL )
    {
	send_to_char("You can't see that one.\n\r", ch);
	return;
    }

    if (!IS_NPC(victim) && victim->desc == NULL)
    {
	send_to_char("You can't do that.\n\r", ch);
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "That's pointless.\n\r", ch );
	return;
    }

    if (is_at_cabal_area(ch) || is_at_cabal_area(victim) )
    {
	send_to_char( "It is not allowed near cabal areas.\n\r", ch );
	return;
    }

    if (is_safe(ch,victim))
    {
	sprintf(buf,"Gods protect %s.\n\r",victim->name);
	send_to_char(buf,ch);
	return;
    }

   if ( (wield = get_weapon_char(ch,WEAPON_BOW)) == NULL)
    	{
	 send_to_char("You need a bow to shoot!\n\r",ch);
	 return;    	
	}

   if ( (arrow= get_weapon_char(ch,WEAPON_ARROW)) == NULL)
    	{
	 send_to_char("You need an arrow holding for your ammunition!\n\r",ch);
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

    sprintf( buf, "You shoot $p to %s.", dir_name[ direction ] );
    act( buf, ch, arrow, NULL, TO_CHAR );
    sprintf( buf, "$n shoots $p to %s.", dir_name[ direction ] );
    act( buf, ch, arrow, NULL, TO_ROOM );

    obj_from_char(arrow);
    send_arrow(ch,victim,arrow,direction,chance,dice(wield->value[1],wield->value[2]) );
    check_improve(ch,gsn_bow,TRUE,1);
}


char *find_way(CHAR_DATA *ch,ROOM_INDEX_DATA *rstart, ROOM_INDEX_DATA *rend) 
{
 int direction;
 static char buf[1024];
 EXIT_DATA *pExit;
 char buf2[2];
 int i;

 sprintf(buf,"Find: ");
 for(i=0; i<65535; i++)
 {
 if ( (rend == rstart))
	 return buf;
  if ( (direction = find_path(rstart->vnum,rend->vnum,ch,-40000,0)) == -1)
	{
	 strcat(buf," BUGGY");
	 return buf;
	}
  if (direction < 0 || direction > 5)
	{
	 strcat(buf," VERY BUGGY");
	 return buf;
	}
   buf2[0] = dir_name[direction][0];
   buf2[1] = '\0';
   strcat(buf,buf2);
   /* find target room */
   pExit = rstart->exit[ direction ];
   if ( !pExit )  
	{
	 strcat(buf," VERY VERY BUGGY");
	 return buf;
	}
   else rstart = pExit->u1.to_room;
 }
 return buf;
}	

void do_human( CHAR_DATA *ch, char *argument )
{
    if (ch->iclass != CLASS_VAMPIRE)
    {
     send_to_char("Huh?\n\r",ch);
     return;
    }
     
    if ( !IS_VAMPIRE(ch) )
    {
     send_to_char("You are already a human.\n\r",ch);
     return;
    }

   affect_strip(ch, gsn_vampire);
   REMOVE_BIT(ch->act,PLR_VAMPIRE);
   send_to_char( "You return to your original size.\n\r", ch );
   return;
}


void do_throw_spear( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA *spear;
    char arg1[512],arg2[512],buf[512];
    int chance,direction;
    int range = (ch->level / 10) + 1;
    
   if (IS_NPC(ch)) return; /* Mobs can't shoot spears */

   if (ch_skill_nok_nomessage(ch,gsn_spear))
	{
	  send_to_char("You don't know how to throw a spear.\n\r",ch);
	  return;
	}

   argument=one_argument( argument, arg1 );
   one_argument( argument, arg2 );

   if ( arg1[0] == '\0' || arg2[0] == '\0')
    {
	send_to_char( "Throw spear what diretion and whom?\n\r", ch );
	return;
    }

    if (ch->fighting)
    {
	send_to_char("You cannot concentrate on throwing spear.\n\r",ch);
	return;
    }

   direction = find_exit( ch, arg1 );

   if (direction<0 || direction > 5) 
	{
	 send_to_char("Throw which direction and whom?\n\r",ch);
	 return;
	}
	
    if ( ( victim = find_char( ch, arg2, direction, range) ) == NULL )
    {
	send_to_char( "You can't see that one.\n\r", ch );
	return;
    }

    if (!IS_NPC(victim) && victim->desc == NULL)
    {
	send_to_char("You can't do that.\n\r", ch);
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "That's pointless.\n\r", ch );
	return;
    }

    if (is_at_cabal_area(ch) || is_at_cabal_area(victim) )
    {
	send_to_char( "It is not allowed near cabal areas.\n\r", ch );
	return;
    }

    if (is_safe(ch,victim))
    {
	sprintf(buf,"Gods protect %s.\n\r",victim->name);
	send_to_char(buf,ch);
	return;
    }

   if ( (spear = get_weapon_char(ch,WEAPON_SPEAR)) == NULL)
    	{
	 send_to_char("You need a spear to throw!\n\r",ch);
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

    sprintf( buf, "You throw $p to %s.", dir_name[ direction ] );
    act( buf, ch, spear, NULL, TO_CHAR );
    sprintf( buf, "$n throws $p to %s.", dir_name[ direction ] );
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

