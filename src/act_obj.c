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
OD *	get_obj_keeper	args( (CHAR_DATA *ch,CHAR_DATA *keeper,char *argument));
void	hold_a_light	args( (CHAR_DATA *ch,OBJ_DATA *obj, int iWear) );
void	hold_a_shield	args( (CHAR_DATA *ch,OBJ_DATA *obj, int iWear) );
void	hold_a_thing	args( (CHAR_DATA *ch,OBJ_DATA *obj, int iWear) );
void	wear_multi	args( (CHAR_DATA *ch,OBJ_DATA *obj,int iWear,bool fReplace) );
void	wear_a_wield	args( (CHAR_DATA *ch,OBJ_DATA *obj, bool fReplace) );

#undef OD
#undef CD

AFFECT_DATA  *	affect_find(AFFECT_DATA *paf, int sn);
int		find_exit( CHAR_DATA *ch, char *arg );


extern char *  const   dir_name        [];

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
        if (!str_cmp(wch->name,obj->owner))
            owner = wch;

    if (owner == NULL)
	return TRUE;

    if (!str_cmp(ch->name,owner->name))
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
    char buffer[100];

    if ( !CAN_WEAR(obj, ITEM_TAKE) )
    {
	send_to_char( "You can't take that.\n\r", ch );
	return;
    }

    if (obj->pIndexData->limit != -1)
    {
      if ( ( IS_OBJ_STAT(obj, ITEM_ANTI_EVIL)    && IS_EVIL(ch)    )
      ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_GOOD)    && IS_GOOD(ch)    )
      ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_NEUTRAL) && IS_NEUTRAL(ch) ) )
      {
	act( "You are zapped by $p and drop it.", ch, obj, NULL, TO_CHAR );
	act( "$n is zapped by $p and drops it.",  ch, obj, NULL, TO_ROOM );
	return;
      }
    }

    if ( ch->carry_number + get_obj_number( obj ) > can_carry_n( ch ) )
    {
	act( "$d: you can't carry that many items.",
	    ch, NULL, obj->name, TO_CHAR );
	return;
    }


    if ( get_carry_weight(ch) + get_obj_weight( obj ) > can_carry_w( ch ) )
    {
	act( "$d: you can't carry that much weight.",
	    ch, NULL, obj->name, TO_CHAR );
	return;
    }

    if (obj->in_room != NULL)
    {
	for (gch = obj->in_room->people; gch != NULL; gch = gch->next_in_room)
	    if (gch->on == obj)
	    {
		act("$N appears to be using $p.",
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

          act( "You get $p from $P.", ch, obj, container, TO_CHAR );
          if (!IS_AFFECTED(ch,AFF_SNEAK))
            act( "$n gets $p from $P.", ch, obj, container, TO_ROOM );
          obj_from_obj( obj );
          act("$p fades to black, then dissapears!", ch,container,NULL,TO_ROOM);
          act("$p fades to black, then dissapears!", ch,container,NULL,TO_CHAR);
          extract_obj(container);
          obj_to_char(obj, ch);

          for ( d = descriptor_list; d; d = d->next )
          {
           if ( d->connected == CON_PLAYING && d->character != NULL &&
             cabal_table[d->character->cabal].obj_ptr == obj)
           act_color("$CYou feel a shudder in your Cabal Power!$c",
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
	act( "You get $p from $P.", ch, obj, container, TO_CHAR );
        if (!IS_AFFECTED(ch,AFF_SNEAK))
	  act( "$n gets $p from $P.", ch, obj, container, TO_ROOM );
	REMOVE_BIT(obj->extra_flags,ITEM_HAD_TIMER);
	obj_from_obj( obj );
    }
    else
    {
	act( "You get $p.", ch, obj, container, TO_CHAR );
        if (!IS_AFFECTED(ch,AFF_SNEAK))
	  act( "$n gets $p.", ch, obj, container, TO_ROOM );
	obj_from_room( obj );
    }

    if ( obj->item_type == ITEM_MONEY)
    {
	ch->silver += obj->value[0];
	ch->gold += obj->value[1];
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
	    sprintf(buffer,"%d %d",obj->value[0],obj->value[1]);
	    do_split(ch,buffer);	
	  }
        }
 
	extract_obj( obj );
    }
    else
    {
	obj_to_char( obj, ch );
        if (IS_SET(obj->progtypes,OPROG_GET))
          (obj->pIndexData->oprogs->get_prog) (obj,ch);
    }

    return;
}



void do_get( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    OBJ_DATA *container;
    bool found;


    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (!str_cmp(arg2,"from"))
	argument = one_argument(argument,arg2);

    /* Get type. */
    if ( arg1[0] == '\0' )
    {
	send_to_char( "Get what?\n\r", ch );
	return;
    }

    if ( is_number( arg1 ) )
    {
	int amount, weight, gold = 0, silver = 0;

	amount = atoi( arg1 );
	if ( amount <= 0
	|| ( str_cmp( arg2, "coins" ) && str_cmp( arg2, "coin" ) &&
	     str_cmp( arg2, "gold"  ) && str_cmp( arg2, "silver") ) )
	{
	    send_to_char( "Usage: <get> <number> <silver|gold|coin|coins>\n\r", ch );
	    return;
	}

        if ( !str_cmp( arg2, "gold") ) 
	    weight = amount * 2 / 5;
	else	weight = amount / 10;

	if ( get_carry_weight(ch) + weight > can_carry_w( ch ) )
	{
	     act( "You can't carry that much weight.",ch, NULL, NULL, TO_CHAR );
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

            case OBJ_VNUM_GOLD_ONE:
                gold += 1;
                break;

            case OBJ_VNUM_SILVER_SOME:
                silver += obj->value[0];
                break;

            case OBJ_VNUM_GOLD_SOME:
                gold += obj->value[1];
                break;

            case OBJ_VNUM_COINS:
                silver += obj->value[0];
                gold += obj->value[1];
                break;
            }
        }

	if ( (!str_cmp( arg2, "gold") && amount > gold )
	|| (str_cmp( arg2, "gold") && amount > silver ) )
	{
	    send_to_char("There's not that much coins there.\n\r", ch);
	    return;
	}
	    
        if ( !str_cmp( arg2, "gold") ) 
	    { gold = amount; silver = 0; }
	else	{ silver = amount; gold = 0; }

        for ( obj = ch->in_room->contents; obj != NULL; obj = obj_next )
        {
            obj_next = obj->next_content;

            switch ( obj->pIndexData->vnum )
            {
            case OBJ_VNUM_SILVER_ONE:
                if (silver) { silver -= 1; extract_obj(obj); }
                break;

            case OBJ_VNUM_GOLD_ONE:
                if (gold) { gold -= 1; extract_obj( obj ); }
                break;

            case OBJ_VNUM_SILVER_SOME:
		if (silver) {
		    if (silver >= obj->value[0]) 
			{ silver -= obj->value[0]; extract_obj(obj); }
		    else { obj->value[0] -= silver; silver = 0;}
		}
                break;

            case OBJ_VNUM_GOLD_SOME:
		if (gold) {
		    if (gold >= obj->value[1]) 
			{ gold -= obj->value[1]; extract_obj(obj); }
		    else { obj->value[1] -= gold; gold = 0;}
		}
                break;

            case OBJ_VNUM_COINS:
		if (silver) {
		    if (silver >= obj->value[0]) {
			    silver -= obj->value[0]; 
			    gold = obj->value[1];
			    extract_obj(obj); 
			    obj = create_money( gold, 0 );
			    obj_to_room( obj, ch->in_room );
			    gold = 0;
			}
		    else { obj->value[0] -= silver; silver = 0;}
		}
		if (gold) {
		    if (gold >= obj->value[1]) {
			    gold -= obj->value[1]; 
			    silver = obj->value[0];
			    extract_obj(obj); 
			    obj = create_money( 0, silver );
			    obj_to_room( obj, ch->in_room );
			    silver = 0;
			}
		    else { obj->value[1] -= gold; gold = 0;}
		}
                break;
            }
	    if (!silver && !gold) break;
        }

	/* restore the amount */
        if ( !str_cmp( arg2, "gold") ) 
	    { gold = amount; silver = 0; }
	else	{ silver = amount; gold = 0; }

	if ( silver ) ch->silver += amount;
	else ch->gold += amount;

        act( "You get some money from the floor.", ch, NULL, NULL, TO_CHAR );
        if (!IS_AFFECTED(ch,AFF_SNEAK))
          act( "$n gets some money from the floor.", ch, NULL, NULL, TO_ROOM );

        if (IS_SET(ch->act,PLR_AUTOSPLIT))
        { /* AUTOSPLIT code */
          int members = 0;
	  CHAR_DATA *gch;
	  char buffer[MAX_INPUT_LENGTH];

          for (gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
          {
            if (!IS_AFFECTED(gch,AFF_CHARM) && is_same_group( gch, ch ) )
              members++;
          }

          if ( members > 1 && (amount > 1) )
          {
            sprintf(buffer,"%d %d",silver, gold);
            do_split(ch,buffer);        
          }
        }

 
	return;
    }

    if ( arg2[0] == '\0' )
    {
	if ( str_cmp( arg1, "all" ) && str_prefix( "all.", arg1 ) )
	{
	    /* 'get obj' */
	    obj = get_obj_list( ch, arg1, ch->in_room->contents );
	    if ( obj == NULL )
	    {
		act( "I see no $T here.", ch, NULL, arg1, TO_CHAR );
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
		    send_to_char( "I see nothing here.\n\r", ch );
		else
		    act( "I see no $T here.", ch, NULL, &arg1[4], TO_CHAR );
	    }
	}
    }
    else
    {
	/* 'get ... container' */
	if ( !str_cmp( arg2, "all" ) || !str_prefix( "all.", arg2 ) )
	{
	    send_to_char( "You can't do that.\n\r", ch );
	    return;
	}

	if ( ( container = get_obj_here( ch, arg2 ) ) == NULL )
	{
	    act( "I see no $T here.", ch, NULL, arg2, TO_CHAR );
	    return;
	}

	switch ( container->item_type )
	{
	default:
	    send_to_char( "That's not a container.\n\r", ch );
	    return;

	case ITEM_CONTAINER:
	case ITEM_CORPSE_NPC:
	    break;

	case ITEM_CORPSE_PC:
	    {

		if (!can_loot(ch,container))
		{
		    send_to_char( "You can't do that.\n\r", ch );
		    return;
		}
	    }
	}

	if ( IS_SET(container->value[1], CONT_CLOSED) )
	{
	    act( "The $d is closed.", ch, NULL, container->name, TO_CHAR );
	    return;
	}

	if ( str_cmp( arg1, "all" ) && str_prefix( "all.", arg1 ) )
	{
	    /* 'get obj container' */
	    obj = get_obj_list( ch, arg1, container->contains );
	    if ( obj == NULL )
	    {
		act( "I see nothing like that in the $T.",
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
			send_to_char("Don't be so greedy!\n\r",ch);
			return;
		    }
		    get_obj( ch, obj, container );
		}
	    }

	    if ( !found )
	    {
		if ( arg1[3] == '\0' )
		    act( "I see nothing in the $T.",
			ch, NULL, arg2, TO_CHAR );
		else
		    act( "I see nothing like that in the $T.",
			ch, NULL, arg2, TO_CHAR );
	    }
	}
    }

    return;
}



void do_put( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *container;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    OBJ_DATA *objc;
    int pcount;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (!str_cmp(arg2,"in") || !str_cmp(arg2,"on"))
	argument = one_argument(argument,arg2);

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Put what in what?\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "all" ) || !str_prefix( "all.", arg2 ) )
    {
	send_to_char( "You can't do that.\n\r", ch );
	return;
    }

    if ( ( container = get_obj_here( ch, arg2 ) ) == NULL )
    {
	act( "I see no $T here.", ch, NULL, arg2, TO_CHAR );
	return;
    }

    if ( container->item_type != ITEM_CONTAINER )
    {
	send_to_char( "That's not a container.\n\r", ch );
	return;
    }

    if ( IS_SET(container->value[1], CONT_CLOSED) )
    {
	act( "The $d is closed.", ch, NULL, container->name, TO_CHAR );
	return;
    }

    if ( str_cmp( arg1, "all" ) && str_prefix( "all.", arg1 ) )
    {
	/* 'put obj container' */
	if ( ( obj = get_obj_carry( ch, arg1 ) ) == NULL )
	{
	    send_to_char( "You do not have that item.\n\r", ch );
	    return;
	}

	if ( obj == container )
	{
	    send_to_char( "You can't fold it into itself.\n\r", ch );
	    return;
	}

	if ( !can_drop_obj( ch, obj ) )
	{
	    send_to_char( "You can't let go of it.\n\r", ch );
	    return;
	}

    	if (WEIGHT_MULT(obj) != 100)
    	{
           send_to_char("You have a feeling that would be a bad idea.\n\r",ch);
            return;
        }

	if ( obj->pIndexData->limit != -1 
		&& !IS_SET(container->value[1], CONT_ST_LIMITED) )
        {
	  act( "This unworthy container won't hold $p.", ch,obj,NULL,TO_CHAR);
	  return;
	}
/*
	if ( IS_SET(container->value[1],CONT_FOR_ARROW) 
		&& (obj->item_type != ITEM_WEAPON
		|| obj->value[0]  != WEAPON_ARROW ) )
	{
	 act("You can only put arrows in $p.",ch,container,NULL,TO_CHAR);
	 return;
	}
*/
	if (get_obj_weight( obj ) + get_true_weight( container )
	     > (container->value[0] * 10) 
	||  get_obj_weight(obj) > (container->value[3] * 10))
	{
	    send_to_char( "It won't fit.\n\r", ch );
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
	      act("It's not safe to put more potions into $p.",ch,container,NULL, TO_CHAR);
	      return;
	   }
	}

        pcount = 0;
        for(objc=container->contains; objc!=NULL ; objc=objc->next_content)
	     pcount++;
	if ( pcount>container->value[0] )
	   {
	      act("It's not safe to put that much item into $p.",ch,container,NULL, TO_CHAR);
	      return;
	   }

	if (container->pIndexData->vnum == OBJ_VNUM_PIT 
	&&  !CAN_WEAR(container,ITEM_TAKE))
	    if (obj->timer)
		SET_BIT(obj->extra_flags,ITEM_HAD_TIMER);
	    else
	        obj->timer = number_range(100,200);

	obj_from_char( obj );
	obj_to_obj( obj, container );

	if (IS_SET(container->value[1],CONT_PUT_ON))
	{
	    act("$n puts $p on $P.",ch,obj,container, TO_ROOM);
	    act("You put $p on $P.",ch,obj,container, TO_CHAR);
	}
	else
	{
	    act( "$n puts $p in $P.", ch, obj, container, TO_ROOM );
	    act( "You put $p in $P.", ch, obj, container, TO_CHAR );
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
	    	    if (obj->timer)
			SET_BIT(obj->extra_flags,ITEM_HAD_TIMER);
	    	    else
	    	    	obj->timer = number_range(100,200);

		if ( obj->pIndexData->limit != -1 )
		{
		  act( "This unworthy container won't hold $p.", ch,obj,NULL,TO_CHAR);
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
	     	    act("It's not safe to put more potions into $p.",ch,container,NULL, TO_CHAR);
	      	    continue;
	   	  }
		}

		pcount++;
		if ( pcount>container->value[0] )
		   {
		      act("It's not safe to put that much item into $p.",ch,container,NULL, TO_CHAR);
		      return;
		   }
		obj_from_char( obj );
		obj_to_obj( obj, container );

        	if (IS_SET(container->value[1],CONT_PUT_ON))
        	{
            	    act("$n puts $p on $P.",ch,obj,container, TO_ROOM);
            	    act("You put $p on $P.",ch,obj,container, TO_CHAR);
        	}
		else
		{
		    act( "$n puts $p in $P.", ch, obj, container, TO_ROOM );
		    act( "You put $p in $P.", ch, obj, container, TO_CHAR );
		}
	    }
	}
    }

    return;
}



void do_drop( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    bool found;


    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Drop what?\n\r", ch );
	return;
    }

    if ( is_number( arg ) )
    {
	/* 'drop NNNN coins' */
	int amount, gold = 0, silver = 0;

	amount   = atoi(arg);
	argument = one_argument( argument, arg );
	if ( amount <= 0
	|| ( str_cmp( arg, "coins" ) && str_cmp( arg, "coin" ) && 
	     str_cmp( arg, "gold"  ) && str_cmp( arg, "silver") ) )
	{
	    send_to_char( "Sorry, you can't do that.\n\r", ch );
	    return;
	}

	if ( !str_cmp( arg, "coins") || !str_cmp(arg,"coin") 
	||   !str_cmp( arg, "silver"))
	{
	    if (ch->silver < amount)
	    {
		send_to_char("You don't have that much silver.\n\r",ch);
		return;
	    }

	    ch->silver -= amount;
	    silver = amount;
	}

	else
	{
	    if (ch->gold < amount)
	    {
		send_to_char("You don't have that much gold.\n\r",ch);
		return;
	    }

	    ch->gold -= amount;
  	    gold = amount;
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

	    case OBJ_VNUM_GOLD_ONE:
		gold += 1;
		extract_obj( obj );
		break;

	    case OBJ_VNUM_SILVER_SOME:
		silver += obj->value[0];
		extract_obj(obj);
		break;

	    case OBJ_VNUM_GOLD_SOME:
		gold += obj->value[1];
		extract_obj( obj );
		break;

	    case OBJ_VNUM_COINS:
		silver += obj->value[0];
		gold += obj->value[1];
		extract_obj(obj);
		break;
	    }
	}

	obj = create_money( gold, silver );
	obj_to_room( obj, ch->in_room );
	if ( !IS_AFFECTED(ch, AFF_SNEAK) )
	  act( "$n drops some coins.", ch, NULL, NULL, TO_ROOM );
	send_to_char( "OK.\n\r", ch );
	if ( IS_WATER( ch->in_room ) )   {
	  extract_obj( obj );
	  if ( !IS_AFFECTED(ch, AFF_SNEAK) )
	    act("The coins sink down, and disapear in the water.", ch, NULL, NULL, TO_ROOM );
	  act("The coins sink down, and disapear in the water.", ch, NULL, NULL, TO_CHAR );
	}
	return;
    }

    if ( str_cmp( arg, "all" ) && str_prefix( "all.", arg ) )
    {
	/* 'drop obj' */
	if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
	{
	    send_to_char( "You do not have that item.\n\r", ch );
	    return;
	}

	if ( !can_drop_obj( ch, obj ) )
	{
	    send_to_char( "You can't let go of it.\n\r", ch );
	    return;
	}

	obj_from_char( obj );
	obj_to_room( obj, ch->in_room );
	if ( !IS_AFFECTED(ch, AFF_SNEAK) )
  	  act( "$n drops $p.", ch, obj, NULL, TO_ROOM );
	act( "You drop $p.", ch, obj, NULL, TO_CHAR );
	if ( obj->pIndexData->vnum == OBJ_VNUM_POTION_VIAL &&
              number_percent( ) < 40 )  
	  if ( !IS_SET(ch->in_room->sector_type, SECT_FOREST) &&
	       !IS_SET(ch->in_room->sector_type, SECT_DESERT) &&
	       !IS_SET(ch->in_room->sector_type, SECT_AIR) &&
	       !IS_WATER(ch->in_room) )  
	  {
	    act( "$p cracks and shaters into tiny pieces.", ch, obj, NULL,TO_ROOM );
	    act( "$p cracks and shaters into tiny pieces.", ch, obj, NULL,TO_CHAR );
	    extract_obj( obj );
	    return;
	  }
        if (IS_SET(obj->progtypes,OPROG_DROP))
          (obj->pIndexData->oprogs->drop_prog) (obj,ch);

	if ( !may_float(obj) && cant_float(obj) && IS_WATER( ch->in_room )) 
	{
	  if ( !IS_AFFECTED(ch, AFF_SNEAK) )
	    act( "$p sinks down the water.", ch, obj, NULL, TO_ROOM);
	  act( "$p sinks down the water.", ch, obj, NULL, TO_CHAR);
	  extract_obj( obj );
	}

	else if (IS_OBJ_STAT(obj,ITEM_MELT_DROP) )
	{
	  if ( !IS_AFFECTED(ch, AFF_SNEAK) )
	    act("$p dissolves into smoke.",ch,obj,NULL,TO_ROOM);
	  act("$p dissolves into smoke.",ch,obj,NULL,TO_CHAR);
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
		  act( "$n drops $p.", ch, obj, NULL, TO_ROOM );
		act( "You drop $p.", ch, obj, NULL, TO_CHAR );
	        if ( obj->pIndexData->vnum == OBJ_VNUM_POTION_VIAL &&
		     number_percent( )  < 70 )
		  if ( !IS_SET(ch->in_room->sector_type, SECT_FOREST) &&
		       !IS_SET(ch->in_room->sector_type, SECT_DESERT) &&
			!IS_SET(ch->in_room->sector_type, SECT_AIR) &&  
		       !IS_WATER(ch->in_room) )  

	  	{
		  if ( !IS_AFFECTED(ch, AFF_SNEAK) )
	            act( "$p cracks and shaters into tiny pieces.", ch, obj, NULL,TO_ROOM );
	          act( "$p cracks and shaters into tiny pieces.", ch, obj, NULL,TO_CHAR );
	          extract_obj( obj );
	          continue;
		}

                if (IS_SET(obj->progtypes,OPROG_DROP))
                  (obj->pIndexData->oprogs->drop_prog) (obj,ch);

		if ( !may_float(obj) && cant_float(obj) && IS_WATER(ch->in_room) )  
		{
		  if ( !IS_AFFECTED(ch, AFF_SNEAK) )
		    act( "$p sinks down the water.", ch, obj, NULL, TO_ROOM);
		  act( "$p sinks down the water.", ch, obj, NULL, TO_CHAR);
		  extract_obj( obj );
		}
        	else if (IS_OBJ_STAT(obj,ITEM_MELT_DROP))
        	{
		  if ( !IS_AFFECTED(ch, AFF_SNEAK) )
             	    act("$p dissolves into smoke.",ch,obj,NULL,TO_ROOM);
            	  act("$p dissolves into smoke.",ch,obj,NULL,TO_CHAR);
            	  extract_obj(obj);
        	}
	    }
	}

	if ( !found )
	{
	    if ( arg[3] == '\0' )
		act( "You are not carrying anything.",
		    ch, NULL, arg, TO_CHAR );
	    else
		act( "You are not carrying any $T.",
		    ch, NULL, &arg[4], TO_CHAR );
	}
    }

    return;
}



void do_drag( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
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
	send_to_char( "Drag what to which direction?\n\r", ch );
	return;
    }

    if ( !str_cmp( arg1, "all" ) || !str_prefix( "all.", arg1 ) )
    {
        send_to_char( "You can't do that.\n\r", ch );
        return;
    }

   obj = get_obj_list( ch, arg1, ch->in_room->contents );
   if ( obj == NULL )
   {
	act( "I see no $T here.", ch, NULL, arg1, TO_CHAR );
        return;
   }

   if ( !CAN_WEAR(obj, ITEM_TAKE) )
   {
        send_to_char( "You can't take that.\n\r", ch );
        return;
   }

   if (obj->pIndexData->limit != -1)
   {
      if ( ( IS_OBJ_STAT(obj, ITEM_ANTI_EVIL)    && IS_EVIL(ch)    )
      ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_GOOD)    && IS_GOOD(ch)    )
      ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_NEUTRAL) && IS_NEUTRAL(ch) ) )
      {
        act( "You are zapped by $p and drop it.", ch, obj, NULL, TO_CHAR );
        act( "$n is zapped by $p and drops it.",  ch, obj, NULL, TO_ROOM );
        return;
      }
   }

   if (obj->in_room != NULL)
   {
       for (gch = obj->in_room->people; gch != NULL; gch = gch->next_in_room)
           if (gch->on == obj)
           {
               act("$N appears to be using $p.", ch,obj,gch,TO_CHAR);
               return;
           }
   }

   if ( (get_carry_weight(ch)+get_obj_weight(obj)) > (2*can_carry_w(ch)) )
   {
       act( "$d: you can't drag that much weight.",
           ch, NULL, obj->name, TO_CHAR );
       return;
   }

   if ( get_eq_char(ch, WEAR_LEFT) != NULL 
	|| get_eq_char(ch, WEAR_RIGHT) != NULL
	|| get_eq_char(ch, WEAR_BOTH) != NULL)
   {
      send_to_char( "You need your both hands free.\n\r",ch);
      return;
   }

   if ( (direction = find_exit( ch, arg2 )) < 0)
	return;

   if ( ( pexit   = ch->in_room->exit[direction] ) == NULL
   ||   pexit->u1.to_room   == NULL 
   ||   !can_see_room(ch,pexit->u1.to_room))
   {
       send_to_char( "Alas, you cannot go that way.\n\r", ch );
       return;
   }

   sprintf(buf,"You grab $p to drag towards %s.", dir_name[direction] );
   act( buf, ch, obj, NULL, TO_CHAR );
   if (!IS_AFFECTED(ch,AFF_SNEAK))
   {
         sprintf(buf,"$n grabs $p to drag towards %s.", dir_name[direction] );
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
       send_to_char( "You cannot drag that way.\n\r", ch );
   else 
   {
	if ( !IS_AFFECTED(ch, AFF_SNEAK) )
		act( "$n drops $p.", ch, obj, NULL, TO_ROOM );
	act( "You drop $p.", ch, obj, NULL, TO_CHAR );
        WAIT_STATE( ch,2 * PULSE_VIOLENCE );
   }

   obj_from_char( obj );
   obj_to_room( obj, ch->in_room );

   return;
}


void do_give( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA  *obj;

    argument = one_argument( argument, arg1 );
    sprintf(buf2,"%s",argument);
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Give what to whom?\n\r", ch );
	return;
    }

    if ( is_number( arg1 ) )
    {
	/* 'give NNNN coins victim' */
	int amount;
	bool silver;
	int weight;

	amount   = atoi(arg1);
	if ( amount <= 0
	|| ( str_cmp( arg2, "coins" ) && str_cmp( arg2, "coin" ) && 
	     str_cmp( arg2, "gold"  ) && str_cmp( arg2, "silver")) )
	{
	    send_to_char( "Sorry, you can't do that.\n\r", ch );
	    return;
	}

	silver = str_cmp(arg2,"gold");

	argument = one_argument( argument, arg2 );
	if ( arg2[0] == '\0' )
	{
	    send_to_char( "Give what to whom?\n\r", ch );
	    return;
	}

	if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
	{
	    send_to_char( "They aren't here.\n\r", ch );
	    return;
	}

	if ( (!silver && ch->gold < amount) || (silver && ch->silver < amount) )
	{
	    send_to_char( "You haven't got that much.\n\r", ch );
	    return;
	}

        if ( !silver )
	    weight = amount * 2 / 5;
	else	weight = amount / 10;

	if ( !IS_NPC(victim) 
		&& get_carry_weight(victim) + weight > can_carry_w(victim) )
	{
	     act( "$N can't carry that much weight.", ch,NULL,victim, TO_CHAR);
	     return;
	}

	if (silver)
	{
	    ch->silver		-= amount;
	    victim->silver 	+= amount;
	}
	else
	{
	    ch->gold		-= amount;
	    victim->gold	+= amount;
	}

	sprintf(buf,"$n gives you %d %s.",amount, silver ? "silver" : "gold");
	act( buf, ch, NULL, victim, TO_VICT    );
	act( "$n gives $N some coins.",  ch, NULL, victim, TO_NOTVICT );
	sprintf(buf,"You give $N %d %s.",amount, silver ? "silver" : "gold");
	act( buf, ch, NULL, victim, TO_CHAR    );
        if (IS_SET(victim->progtypes,MPROG_BRIBE))
          (victim->pIndexData->mprogs->bribe_prog) (victim,ch,amount);

	if (IS_NPC(victim) && IS_SET(victim->act,ACT_IS_CHANGER))
	{
	    int change;

	    change = (silver ? 95 * amount / 100 / 100 
		 	     : 95 * amount);


            if ( silver )
	       weight = change * 2 / 5;
	    else weight = change / 10;

            if ( !silver )
	       weight -= amount * 2 / 5;
	    else weight -= amount / 10;

	    if (!IS_NPC(ch) 
		&& get_carry_weight(ch) + weight > can_carry_w(ch) )
	    {
	        act("You can't carry that much weight.", ch,NULL,NULL,TO_CHAR);
	        return;
	    }

	    if (!silver && change > victim->silver)
	    	victim->silver += change;

	    if (silver && change > victim->gold)
		victim->gold += change;

	    if (change < 1 && can_see(victim,ch))
	    {
		act(
	"$n tells you 'I'm sorry, you did not give me enough to change.'"
		    ,victim,NULL,ch,TO_VICT);
		ch->reply = victim;
		sprintf(buf,"%d %s %s", 
			amount, silver ? "silver" : "gold",ch->name);
		do_give(victim,buf);
	    }
	    else if (can_see(victim,ch))
	    {
		sprintf(buf,"%d %s %s", 
			change, silver ? "gold" : "silver",ch->name);
		do_give(victim,buf);
		if (silver)
		{
		    sprintf(buf,"%d silver %s", 
			(95 * amount / 100 - change * 100),ch->name);
		    do_give(victim,buf);
		}
		act("$n tells you 'Thank you, come again.'",
		    victim,NULL,ch,TO_VICT);
		ch->reply = victim;
	    }
	}
	return;
    }

    if ( ( obj = get_obj_carry( ch, arg1 ) ) == NULL )
    {
	send_to_char( "You do not have that item.\n\r", ch );
	return;
    }

    if ( obj->wear_loc != WEAR_NONE )
    {
	send_to_char( "You must remove it first.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }


    if (  (IS_NPC(victim) && victim->pIndexData->pShop != NULL ) &&
	   !IS_SET(victim->progtypes, MPROG_GIVE) )
    {
	act("$N tells you 'Sorry, you'll have to sell that.'",
	    ch,NULL,victim,TO_CHAR);
	ch->reply = victim;
	return;
    }

    if ( !can_drop_obj( ch, obj ) )
    {
	send_to_char( "You can't let go of it.\n\r", ch );
	return;
    }

    if ( victim->carry_number + get_obj_number( obj ) > can_carry_n( victim ) )
    {
	act( "$N has $S hands full.", ch, NULL, victim, TO_CHAR );
	return;
    }

    if (get_carry_weight(victim) + get_obj_weight(obj) > can_carry_w( victim ) )
    {
	act( "$N can't carry that much weight.", ch, NULL, victim, TO_CHAR );
	return;
    }

    if ( !can_see_obj( victim, obj ) )
    {
	act( "$N can't see it.", ch, NULL, victim, TO_CHAR );
	return;
    }

    if (obj->pIndexData->limit != -1)
    {
      if ( ( IS_OBJ_STAT(obj, ITEM_ANTI_EVIL)    && IS_EVIL(victim)    )
      ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_GOOD)    && IS_GOOD(victim)    )
      ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_NEUTRAL) && IS_NEUTRAL(victim) ) )
      {
	send_to_char( "Your victim's alignment doesn't match with the objects align.", ch );
	return;
      }
    }

    obj_from_char( obj );
    obj_to_char( obj, victim );
    act( "$n gives $p to $N.", ch, obj, victim, TO_NOTVICT );
    act( "$n gives you $p.",   ch, obj, victim, TO_VICT    );
    act( "You give $p to $N.", ch, obj, victim, TO_CHAR    );

    if (IS_SET(obj->progtypes,OPROG_GIVE))
      (obj->pIndexData->oprogs->give_prog) (obj,ch,victim);

    if (IS_SET(obj->progtypes,OPROG_GET))
      (obj->pIndexData->oprogs->get_prog) (obj,victim);

    if (IS_SET(victim->progtypes,MPROG_GIVE))
      (victim->pIndexData->mprogs->give_prog) (victim,ch,obj);

    return;
}


void do_bury( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_INPUT_LENGTH];
    char *bufp;
    OBJ_DATA *obj, *shovel, *stone;
    int move;

    one_argument( argument, arg );

    if ( !IS_IMMORTAL(ch) && ch->iclass != CLASS_CLERIC )
    {    
        send_to_char( "You are not a cleric!\n\r", ch );
        return;
    }
    
    if ( arg[0] == '\0' )
    {    
        send_to_char( "Bury whose corpse?\n\r", ch );
        return;
    }
    
    if ( (shovel = get_weapon_char(ch, WEAPON_MACE)) == NULL
         || !is_name("shovel",shovel->name) )
    {
        send_to_char( "You don't have shovel do dig!\n\r", ch );
        return;
    }

   obj = get_obj_list( ch, arg, ch->in_room->contents );
   if ( obj == NULL )
   {
        act( "I see no $T here.", ch, NULL, arg, TO_CHAR );
        return;
   }

   if (obj->item_type != ITEM_CORPSE_PC && obj->item_type != ITEM_CORPSE_NPC)
    {
      send_to_char("Why do you want to bury that?\n\r",ch);
      return;
    }

    switch( ch->in_room->sector_type )
    {
        case SECT_CITY:
        case SECT_INSIDE:
            send_to_char( "The floor is too hard to dig through.\n\r", ch );
            return;
        case SECT_WATER_SWIM:
        case SECT_WATER_NOSWIM:
            send_to_char( "You cannot bury something here.\n\r", ch );
            return;
        case SECT_AIR:
            send_to_char( "What?  In the air?!\n\r", ch );
            return;
    }

    move = (int) ((obj->weight * 5) / get_curr_stat(ch, STAT_STR) );
    move = URANGE( 2, move, 1000 );
    if ( move > ch->move )
    {
        send_to_char( "You don't have enough energy to bury something of that size.\n\r", ch );
        return;
    }
    ch->move -= move;

    act( "You solemnly bury $p...", ch, obj, NULL, TO_CHAR );
    act( "$n solemnly buries $p...", ch, obj, NULL, TO_ROOM );

    SET_BIT( obj->extra_flags, ITEM_BURIED );
    WAIT_STATE( ch, 4 * PULSE_VIOLENCE );

    obj->timer = -1;

    buf[0] = '\0';
    bufp = obj->short_descr;
    while ( bufp[0] != '\0' )
    {
     bufp = one_argument(bufp, arg);
     if (!(!str_cmp(arg,"The") || !str_cmp(arg,"undead") 
	|| !str_cmp(arg,"body") || !str_cmp(arg,"corpse")
        || !str_cmp(arg,"of") ))
     {
	if (buf[0] == '\0')   strcat(buf,arg);
        else  {
                 strcat(buf," ");
                 strcat(buf,arg);
        }
     }
    }
    sprintf(arg, "%s", buf);

    stone = create_object( get_obj_index(OBJ_VNUM_GRAVE_STONE), ch->level);

    sprintf(buf, stone->description, arg);
    free_string( stone->description );
    stone->description = str_dup( buf ); 

    sprintf(buf, stone->short_descr, arg);
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

void do_dig( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj, *shovel, *corpse;
    int move;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {    
        send_to_char( "Dig which grave?\n\r", ch );
        return;
    }
    
    if ( (shovel = get_weapon_char(ch, WEAPON_MACE)) == NULL
         || !is_name("shovel",shovel->name) )
    {
        send_to_char( "You don't have shovel do dig!\n\r", ch );
        return;
    }

   obj = get_obj_list( ch, arg, ch->in_room->contents );
   if ( obj == NULL )
   {
        act( "I see no $T here.", ch, NULL, arg, TO_CHAR );
        return;
   }

   if (obj->pIndexData->vnum != OBJ_VNUM_GRAVE_STONE)
    {
      send_to_char("I don't think that it is a grave.\n\r",ch);
      return;
    }

    move = (int) ((obj->weight * 5) / get_curr_stat(ch, STAT_STR) );
    move = URANGE( 2, move, 1000 );
    if ( move > ch->move )
    {
        send_to_char( "You don't have enough energy to dig something of that size.\n\r", ch );
        return;
    }
    ch->move -= move;

    act( "You start digging $p...", ch, obj, NULL, TO_CHAR );
    act( "$n starts diggin $p...", ch, obj, NULL, TO_ROOM );

    WAIT_STATE( ch, 4 * PULSE_VIOLENCE );

    if ( (corpse = obj->contains) == NULL)
    {
	act("Digging reveals nothing.\n\r",ch, NULL, NULL, TO_ALL );
        return;
    }

    REMOVE_BIT( corpse->extra_flags, ITEM_BURIED );
    obj_from_obj( corpse );
    obj_to_room( corpse, ch->in_room ); 
    extract_obj( obj );
    corpse->timer   = number_range( 25, 40 );
    act("Digging reveals $p.\n\r",ch, corpse, NULL, TO_ALL );

    return;
}


/* for poisoning weapons and food/drink */
void do_envenom(CHAR_DATA *ch, char *argument)
{
    OBJ_DATA *obj;
    AFFECT_DATA af;
    int percent,skill;

    /* find out what */
    if (argument == '\0')
    {
	send_to_char("Envenom what item?\n\r",ch);
	return;
    }

    obj =  get_obj_list(ch,argument,ch->carrying);

    if (obj== NULL)
    {
	send_to_char("You don't have that item.\n\r",ch);
	return;
    }

    if ((skill = get_skill(ch,gsn_envenom)) < 1)
    {
	send_to_char("Are you crazy? You'd poison yourself!\n\r",ch);
	return;
    }

    if (obj->item_type == ITEM_FOOD || obj->item_type == ITEM_DRINK_CON)
    {
	if (IS_OBJ_STAT(obj,ITEM_BLESS) || IS_OBJ_STAT(obj,ITEM_BURN_PROOF))
	{
	    act("You fail to poison $p.",ch,obj,NULL,TO_CHAR);
	    return;
	}

	if (number_percent() < skill)  /* success! */
	{
	    act("$n treats $p with deadly poison.",ch,obj,NULL,TO_ROOM);
	    act("You treat $p with deadly poison.",ch,obj,NULL,TO_CHAR);
	    if (!obj->value[3])
	    {
		obj->value[3] = 1;
		check_improve(ch,gsn_envenom,TRUE,4);
	    }
	    WAIT_STATE(ch,skill_table[gsn_envenom].beats);
	    return;
	}

	act("You fail to poison $p.",ch,obj,NULL,TO_CHAR);
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
            act("You can't seem to envenom $p.",ch,obj,NULL,TO_CHAR);
            return;
        }

	if (obj->value[3] < 0 
	||  attack_table[obj->value[3]].damage == DAM_BASH)
	{
	    send_to_char("You can only envenom edged weapons.\n\r",ch);
	    return;
	}

        if (IS_WEAPON_STAT(obj,WEAPON_POISON))
        {
            act("$p is already envenomed.",ch,obj,NULL,TO_CHAR);
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
              act("$n coats $p with deadly venom.",ch,obj,NULL,TO_ROOM);
	    act("You coat $p with venom.",ch,obj,NULL,TO_CHAR);
	    check_improve(ch,gsn_envenom,TRUE,3);
	    WAIT_STATE(ch,skill_table[gsn_envenom].beats);
            return;
        }
	else
	{
	    act("You fail to envenom $p.",ch,obj,NULL,TO_CHAR);
	    check_improve(ch,gsn_envenom,FALSE,3);
	    WAIT_STATE(ch,skill_table[gsn_envenom].beats);
	    return;
	}
    }
 
    act("You can't poison $p.",ch,obj,NULL,TO_CHAR);
    return;
}

void do_fill( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *fountain;
    bool found;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Fill what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
    {
	send_to_char( "You do not have that item.\n\r", ch );
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
	send_to_char( "There is no fountain here!\n\r", ch );
	return;
    }

    if ( obj->item_type != ITEM_DRINK_CON )
    {
	send_to_char( "You can't fill that.\n\r", ch );
	return;
    }

    if ( obj->value[1] != 0 && obj->value[2] != fountain->value[2] )
    {
	send_to_char( "There is already another liquid in it.\n\r", ch );
	return;
    }

    if ( obj->value[1] >= obj->value[0] )
    {
	send_to_char( "Your container is full.\n\r", ch );
	return;
    }

    sprintf(buf,"You fill $p with %s from $P.",
	liq_table[fountain->value[2]].liq_name);
    act( buf, ch, obj,fountain, TO_CHAR );
    sprintf(buf,"$n fills $p with %s from $P.",
	liq_table[fountain->value[2]].liq_name);
    act(buf,ch,obj,fountain,TO_ROOM);
    obj->value[2] = fountain->value[2];
    obj->value[1] = obj->value[0];

}

void do_pour(CHAR_DATA *ch, char *argument)
{
    char arg[MAX_STRING_LENGTH],buf[MAX_STRING_LENGTH];
    OBJ_DATA *out, *in;
    CHAR_DATA *vch = NULL;
    int amount;

    argument = one_argument(argument,arg);
    
    if (arg[0] == '\0' || argument[0] == '\0')
    {
	send_to_char("Pour what into what?\n\r",ch);
	return;
    }
    

    if ((out = get_obj_carry(ch,arg)) == NULL)
    {
	send_to_char("You don't have that item.\n\r",ch);
	return;
    }

    if (out->item_type != ITEM_DRINK_CON)
    {
	send_to_char("That's not a drink container.\n\r",ch);
	return;
    }

    if (!str_cmp(argument,"out"))
    {
	if (out->value[1] == 0)
	{
	    send_to_char("It's already empty.\n\r",ch);
	    return;
	}

	out->value[1] = 0;
	out->value[3] = 0;
        if ( !IS_WATER( ch->in_room ) )  {
	  sprintf(buf,"You invert $p, spilling %s all over the ground.",
		liq_table[out->value[2]].liq_name);
	  act(buf,ch,out,NULL,TO_CHAR);
	
	  sprintf(buf,"$n inverts $p, spilling %s all over the ground.",
		liq_table[out->value[2]].liq_name);
	  act(buf,ch,out,NULL,TO_ROOM);
	}
	else  {
	  sprintf(buf,"You invert $p, spilling %s in to the water.",
		liq_table[out->value[2]].liq_name);
	  act(buf,ch,out,NULL,TO_CHAR);
	
	  sprintf(buf,"$n inverts $p, spilling %s in to the water.",
		liq_table[out->value[2]].liq_name);
	  act(buf,ch,out,NULL,TO_ROOM);
	}
	return;
    }

    if ((in = get_obj_here(ch,argument)) == NULL)
    {
	vch = get_char_room(ch,argument);

	if (vch == NULL)
	{
	    send_to_char("Pour into what?\n\r",ch);
	    return;
	}

	in = get_hold_char(vch);

	if (in == NULL)
	{
	    send_to_char("They aren't holding anything.",ch);
 	    return;
	}
    }

    if (in->item_type != ITEM_DRINK_CON)
    {
	send_to_char("You can only pour into other drink containers.\n\r",ch);
	return;
    }
    
    if (in == out)
    {
	send_to_char("You cannot change the laws of physics!\n\r",ch);
	return;
    }

    if (in->value[1] != 0 && in->value[2] != out->value[2])
    {
	send_to_char("They don't hold the same liquid.\n\r",ch);
	return;
    }

    if (out->value[1] == 0)
    {
	act("There's nothing in $p to pour.",ch,out,NULL,TO_CHAR);
	return;
    }

    if (in->value[1] >= in->value[0])
    {
	act("$p is already filled to the top.",ch,in,NULL,TO_CHAR);
	return;
    }

    amount = UMIN(out->value[1],in->value[0] - in->value[1]);

    in->value[1] += amount;
    out->value[1] -= amount;
    in->value[2] = out->value[2];
    
    if (vch == NULL)
    {
    	sprintf(buf,"You pour %s from $p into $P.",
	    liq_table[out->value[2]].liq_name);
    	act(buf,ch,out,in,TO_CHAR);
    	sprintf(buf,"$n pours %s from $p into $P.",
	    liq_table[out->value[2]].liq_name);
    	act(buf,ch,out,in,TO_ROOM);
    }
    else
    {
        sprintf(buf,"You pour some %s for $N.",
            liq_table[out->value[2]].liq_name);
        act(buf,ch,NULL,vch,TO_CHAR);
	sprintf(buf,"$n pours you some %s.",
	    liq_table[out->value[2]].liq_name);
	act(buf,ch,NULL,vch,TO_VICT);
        sprintf(buf,"$n pours some %s for $N.",
            liq_table[out->value[2]].liq_name);
        act(buf,ch,NULL,vch,TO_NOTVICT);
    }

}

void do_drink( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
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
	    send_to_char( "Drink what?\n\r", ch );
	    return;
	}
    }
    else
    {
	if ( ( obj = get_obj_here( ch, arg ) ) == NULL )
	{
	    send_to_char( "You can't find it.\n\r", ch );
	    return;
	}
    }

    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK] > 10 )
    {
	send_to_char( "You fail to reach your mouth.  *Hic*\n\r", ch );
	return;
    }

    switch ( obj->item_type )
    {
    default:
	send_to_char( "You can't drink from that.\n\r", ch );
	return;

    case ITEM_FOUNTAIN:
        if ( ( liquid = obj->value[2] )  < 0 )
        {
            bug( "Do_drink: bad liquid number %d.", liquid );
            liquid = obj->value[2] = 0;
        }
	amount = liq_table[liquid].liq_affect[4] * 3;
	break;

    case ITEM_DRINK_CON:
	if ( obj->value[1] <= 0 )
	{
	    send_to_char( "It is already empty.\n\r", ch );
	    return;
	}

	if ( ( liquid = obj->value[2] )  < 0 )
	{
	    bug( "Do_drink: bad liquid number %d.", liquid );
	    liquid = obj->value[2] = 0;
	}

        amount = liq_table[liquid].liq_affect[4];
        amount = UMIN(amount, obj->value[1]);
	break;
     }
    if (!IS_NPC(ch) && !IS_IMMORTAL(ch) 
    &&  ch->pcdata->condition[COND_FULL] > 80)
    {
	send_to_char("You're too full to drink more.\n\r",ch);
	return;
    }

    act( "$n drinks $T from $p.",
	ch, obj, liq_table[liquid].liq_name, TO_ROOM );
    act( "You drink $T from $p.",
	ch, obj, liq_table[liquid].liq_name, TO_CHAR );

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
	send_to_char( "You feel drunk.\n\r", ch );
    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_FULL]   > 60 )
	send_to_char( "You are full.\n\r", ch );
    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_THIRST] > 60 )
	send_to_char( "Your thirst is quenched.\n\r", ch );
	
    if ( obj->value[3] != 0 )
    {
	/* The drink was poisoned ! */
	AFFECT_DATA af;

	act( "$n chokes and gags.", ch, NULL, NULL, TO_ROOM );
	send_to_char( "You choke and gag.\n\r", ch );
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



void do_eat( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;


    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Eat what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
    {
	send_to_char( "You do not have that item.\n\r", ch );
	return;
    }

    if ( !IS_IMMORTAL(ch) )
    {
	if ( obj->item_type != ITEM_FOOD && obj->item_type != ITEM_PILL )
	{
	    send_to_char( "That's not edible.\n\r", ch );
	    return;
	}

	if ( !IS_NPC(ch) && ch->pcdata->condition[COND_FULL] > 80 )
	{   
	    send_to_char( "You are too full to eat more.\n\r", ch );
	    return;
	}
    }

    act( "$n eats $p.",  ch, obj, NULL, TO_ROOM );
    act( "You eat $p.", ch, obj, NULL, TO_CHAR );
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
		send_to_char( "You are no longer hungry.\n\r", ch );
	    else if ( ch->pcdata->condition[COND_FULL] > 60 )
		send_to_char( "You are full.\n\r", ch );
	}

	if ( obj->value[3] != 0 )
	{
	    /* The food was poisoned! */
	    AFFECT_DATA af;

	    act( "$n chokes and gags.", ch, 0, 0, TO_ROOM );
	    send_to_char( "You choke and gag.\n\r", ch );

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
	act( "You can't remove $p.", ch, obj, NULL, TO_CHAR );
	return FALSE;
    }

    if (( obj->item_type == ITEM_TATTOO ) && ( !IS_IMMORTAL(ch) ) )
    {
	act( "You must scratch it to remove $p.", ch, obj, NULL, TO_CHAR );
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
	act( "You remove $p, in pain.", ch, obj, NULL, TO_CHAR );
	act( "$n remove $p, in pain.", ch, obj, NULL, TO_ROOM );
	WAIT_STATE(ch,4);
	return TRUE;
    }

    unequip_char( ch, obj );
    act( "$n stops using $p.", ch, obj, NULL, TO_ROOM );
    act( "You stop using $p.", ch, obj, NULL, TO_CHAR );

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
	act( "You can't remove $p.", ch, obj, NULL, TO_CHAR );
	return FALSE;
    }

    if (( obj->item_type == ITEM_TATTOO ) && ( !IS_IMMORTAL(ch) ) )
    {
	act( "You must scratch it to remove $p.", ch, obj, NULL, TO_CHAR );
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
	act( "You remove $p, in pain.", ch, obj, NULL, TO_CHAR );
	act( "$n remove $p, in pain.", ch, obj, NULL, TO_ROOM );
	WAIT_STATE(ch,4);
	return TRUE;
    }

    unequip_char( ch, obj );
    act( "$n stops using $p.", ch, obj, NULL, TO_ROOM );
    act( "You stop using $p.", ch, obj, NULL, TO_CHAR );

    return TRUE;
}



/*
 * Wear one object.
 * Optional replacement of existing objects.
 * Big repetitive code, ick.
 */
void wear_obj( CHAR_DATA *ch, OBJ_DATA *obj, bool fReplace )
{
    char buf[MAX_STRING_LENGTH];
    int wear_level;

    wear_level = ch->level;

    if ( (class_table[ch->iclass].fMana && obj->item_type == ITEM_ARMOR)
	|| (!class_table[ch->iclass].fMana && obj->item_type == ITEM_WEAPON))
		wear_level += 3;	     

    if ( wear_level < obj->level )
    {
	sprintf( buf, "You must be level %d to use this object.\n\r",
	    obj->level );
	send_to_char( buf, ch );
	act( "$n tries to use $p, but is too inexperienced.",
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
        else send_to_char("You can't hold a light right now.\n\r",ch);
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
	act( "$n wears $p on $s torso.",   ch, obj, NULL, TO_ROOM );
	act( "You wear $p on your torso.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_BODY );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_HEAD ) )
    {
	if ( !remove_obj_loc( ch, WEAR_HEAD, fReplace ) )
	    return;
	act( "$n wears $p on $s head.",   ch, obj, NULL, TO_ROOM );
	act( "You wear $p on your head.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_HEAD );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_LEGS ) )
    {
	if ( !remove_obj_loc( ch, WEAR_LEGS, fReplace ) )
	    return;
	act( "$n wears $p on $s legs.",   ch, obj, NULL, TO_ROOM );
	act( "You wear $p on your legs.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_LEGS );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_FEET ) )
    {
	if ( !remove_obj_loc( ch, WEAR_FEET, fReplace ) )
	    return;
	act( "$n wears $p on $s feet.",   ch, obj, NULL, TO_ROOM );
	act( "You wear $p on your feet.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_FEET );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_HANDS ) )
    {
	if ( !remove_obj_loc( ch, WEAR_HANDS, fReplace ) )
	    return;
	act( "$n wears $p on $s hands.",   ch, obj, NULL, TO_ROOM );
	act( "You wear $p on your hands.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_HANDS );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_ARMS ) )
    {
	if ( !remove_obj_loc( ch, WEAR_ARMS, fReplace ) )
	    return;
	act( "$n wears $p on $s arms.",   ch, obj, NULL, TO_ROOM );
	act( "You wear $p on your arms.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_ARMS );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_ABOUT ) )
    {
	if ( !remove_obj_loc( ch, WEAR_ABOUT, fReplace ) )
	    return;
	act( "$n wears $p about $s torso.",   ch, obj, NULL, TO_ROOM );
	act( "You wear $p about your torso.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_ABOUT );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_WAIST ) )
    {
	if ( !remove_obj_loc( ch, WEAR_WAIST, fReplace ) )
	    return;
	act( "$n wears $p about $s waist.",   ch, obj, NULL, TO_ROOM );
	act( "You wear $p about your waist.", ch, obj, NULL, TO_CHAR );
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
        else send_to_char("You can't hold a shield right now.\n\r",ch);
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
        else send_to_char("You can't hold a thing right now.\n\r",ch);
	return;
    }


    if ( CAN_WEAR(obj,ITEM_WEAR_FLOAT) )
    {
	if (!remove_obj_loc(ch,WEAR_FLOAT, fReplace) )
	    return;
	act("$n releases $p to float next to $m.",ch,obj,NULL,TO_ROOM);
	act("You release $p and it floats next to you.",ch,obj,NULL,TO_CHAR);
	equip_char(ch,obj,WEAR_FLOAT);
	return;
    }

    if ( CAN_WEAR(obj,ITEM_WEAR_TATTOO)  && IS_IMMORTAL (ch) )
    {
	wear_multi(ch, obj, WEAR_TATTOO, fReplace);
	return;
    }

    if ( fReplace )
	send_to_char( "You can't wear, wield, or hold that.\n\r", ch );

    return;
}



void do_wear( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Wear, wield, or hold what?\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "all" ) )
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
	    send_to_char( "You do not have that item.\n\r", ch );
	    return;
	}

	wear_obj( ch, obj, TRUE );
    }

    return;
}



void do_remove( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Remove what?\n\r", ch );
	return;
    }


    if ( !str_cmp( arg, "all" ) )
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
	send_to_char( "You do not have that item.\n\r", ch );
	return;
    }

    remove_obj( ch, obj, TRUE );
    return;
}



void do_sacrifice( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *obj_content;
    OBJ_DATA *obj_next;
    OBJ_DATA *two_objs[2];
    int silver;
    int iScatter;
    bool fScatter;

    
    /* variables for AUTOSPLIT */
    CHAR_DATA *gch;
    int members;
    char buffer[100];


    one_argument( argument, arg );

    if ( arg[0] == '\0' || !str_cmp( arg, ch->name ) )
    {
	act( "$n offers $mself to gods, who graciously declines.",
	    ch, NULL, NULL, TO_ROOM );
	send_to_char(
	    "Gods appreciates your offer and may accept it later.\n\r", ch );
	return;
    }

    obj = get_obj_list( ch, arg, ch->in_room->contents );
    if ( obj == NULL )
    {
	send_to_char( "You can't find it.\n\r", ch );
	return;
    }

    if ( obj->item_type == ITEM_CORPSE_PC && ch->level < MAX_LEVEL )
    {
     send_to_char("Gods wouldn't like that.\n\r",ch);
     return;
    }


    if ( !CAN_WEAR(obj, ITEM_TAKE) || CAN_WEAR(obj, ITEM_NO_SAC))
    {
	act( "$p is not an acceptable sacrifice.", ch, obj, 0, TO_CHAR );
	return;
    }

    silver = UMAX(1,number_fuzzy(obj->level));

    if (obj->item_type != ITEM_CORPSE_NPC && obj->item_type != ITEM_CORPSE_PC)
    	silver = UMIN(silver,obj->cost);

    if (silver == 1)
        send_to_char(
	    "Gods give you one silver coin for your sacrifice.\n\r", ch );
    else
    {
	sprintf(buf,"Gods give you %d silver coins for your sacrifice.\n\r",
		silver);
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
	    sprintf(buffer,"%d",silver);
	    do_split(ch,buffer);	
	}
    }

    act( "$n sacrifices $p to gods.", ch, obj, NULL, TO_ROOM );

    if (IS_SET(obj->progtypes,OPROG_SAC))
      if ( (obj->pIndexData->oprogs->sac_prog) (obj,ch) )
        return;

    wiznet("$N sends up $p as a burnt offering.",
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
	act( "Your sacrifice reveals $p.", ch, two_objs[0], NULL, TO_CHAR);
	act( "$p is revealed by $n's sacrifice.", ch, two_objs[0], NULL, TO_ROOM);
      }
      if ( iScatter == 2 )  {
	act( "Your sacrifice reveals $p and $P.", ch, two_objs[0], two_objs[1], TO_CHAR);
	act( "$p and $P are revealed by $n's sacrifice.", ch, two_objs[0], two_objs[1], TO_ROOM);
      }
      sprintf( buf, "As you sacrifice the corpse, " );
      sprintf( buf2, "As $n sacrifices the corpse, ");
      if ( iScatter < 3 )
		   fScatter = FALSE; 	
	else if ( iScatter < 5 )  {
	           strcat( buf, "few things " );
		   strcat( buf2, "few things " );
 	}
	else if ( iScatter < 9 )  {
		   strcat( buf, "a bunch of objects " );
                   strcat( buf2, "a bunch of objects " );
        }
	else if ( iScatter < 15 )  {
		    strcat( buf, "many things " );
                    strcat( buf2, "many things " );
        }
	else  {
		   strcat( buf, "a lot of objects " );
                   strcat( buf2, "a lot of objects " );
        }
      strcat( buf, "on it, " );
      strcat( buf2, "on it, " );

      switch( ch->in_room->sector_type )  {
	case SECT_FIELD: strcat( buf, "scatter on the dirt." );
                         strcat( buf2, "scatter on the dirt." );
                         break;
	case SECT_FOREST: strcat( buf, "scatter on the dirt." );
                          strcat( buf2, "scatter on the dirt." );
                          break;
	case SECT_WATER_SWIM: strcat( buf, "scatter over the water." );
                          strcat( buf2, "scatter over the water." );
                          break;
	case SECT_WATER_NOSWIM: strcat( buf, "scatter over the water." );
                          strcat( buf2, "scatter over the water." );
                          break;
	default: strcat( buf, "scatter around." );
                      strcat( buf2, "scatter around." );
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



void do_quaff( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if (ch->cabal == CABAL_BATTLE && !IS_IMMORTAL(ch)) 
    {
      send_to_char("You are a BattleRager, not a filthy magician!\n\r",ch);
      return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Quaff what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
    {
	send_to_char( "You do not have that potion.\n\r", ch );
	return;
    }

    if ( obj->item_type != ITEM_POTION )
    {
	send_to_char( "You can quaff only potions.\n\r", ch );
	return;
    }

    if (ch->level < obj->level)
    {
	send_to_char("This liquid is too powerful for you to drink.\n\r",ch);
	return;
    }


    act( "$n quaffs $p.", ch, obj, NULL, TO_ROOM );
    act( "You quaff $p.", ch, obj, NULL ,TO_CHAR );

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

    if (IS_NPC(ch))	do_drop(ch, "vial");

    return;
}



void do_recite( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *scroll;
    OBJ_DATA *obj;

    if ( ch->cabal == CABAL_BATTLE )
    {
	send_to_char( 
	"RECITE?!  You are a battle rager, not a filthy magician!\n\r", ch );
	return;
    }

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( ( scroll = get_obj_carry( ch, arg1 ) ) == NULL )
    {
	send_to_char( "You do not have that scroll.\n\r", ch );
	return;
    }

    if ( scroll->item_type != ITEM_SCROLL )
    {
	send_to_char( "You can recite only scrolls.\n\r", ch );
	return;
    }


    if ( ch->level < scroll->level)
    {
	send_to_char(
		"This scroll is too complex for you to comprehend.\n\r",ch);
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
	    send_to_char( "You can't find it.\n\r", ch );
	    return;
	}
    }

    act( "$n recites $p.", ch, scroll, NULL, TO_ROOM );
    act( "You recite $p.", ch, scroll, NULL, TO_CHAR );

    if (number_percent() >= get_skill(ch,gsn_scrolls) * 4/5 )
    {
	send_to_char("You mispronounce a syllable.\n\r",ch);
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



void do_brandish( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    OBJ_DATA *staff;
    int sn;

    if ( ch->cabal == CABAL_BATTLE )
    {
	send_to_char( "You are not a filthy magician!\n\r", ch );
	return;
    }

    if ( ( staff = get_hold_char( ch ) ) == NULL )
    {
	send_to_char( "You hold nothing in your hand.\n\r", ch );
	return;
    }

    if ( staff->item_type != ITEM_STAFF )
    {
	send_to_char( "You can brandish only with a staff.\n\r", ch );
	return;
    }

    if ( ( sn = staff->value[3] ) < 0
    ||   sn >= MAX_SKILL
    ||   skill_table[sn].spell_fun == 0 )
    {
	bug( "Do_brandish: bad sn %d.", sn );
	return;
    }

    WAIT_STATE( ch, 2 * PULSE_VIOLENCE );

    if ( staff->value[2] > 0 )
    {
	act( "$n brandishes $p.", ch, staff, NULL, TO_ROOM );
	act( "You brandish $p.",  ch, staff, NULL, TO_CHAR );
	if ( ch->level+3 < staff->level
	||   number_percent() >= 10 + get_skill(ch,gsn_staves) * 4/5)
 	{
	    act ("You fail to invoke $p.",ch,staff,NULL,TO_CHAR);
	    act ("...and nothing happens.",ch,NULL,NULL,TO_ROOM);
	    check_improve(ch,gsn_staves,FALSE,2);
	}
	
	else for ( vch = ch->in_room->people; vch; vch = vch_next )
	{
	    vch_next	= vch->next_in_room;

	    switch ( skill_table[sn].target )
	    {
	    default:
		bug( "Do_brandish: bad target for sn %d.", sn );
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
	act( "$n's $p blazes bright and is gone.", ch, staff, NULL, TO_ROOM );
	act( "Your $p blazes bright and is gone.", ch, staff, NULL, TO_CHAR );
	extract_obj( staff );
    }

    return;
}



void do_zap( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *wand;
    OBJ_DATA *obj;

    if ( ch->cabal == CABAL_BATTLE )
    {
	send_to_char( "You'd destroy the magic, not use it!\n\r", ch );
	return;
    }

    one_argument( argument, arg );
    if ( arg[0] == '\0' && ch->fighting == NULL )
    {
	send_to_char( "Zap whom or what?\n\r", ch );
	return;
    }

    if ( ( wand = get_hold_char( ch ) ) == NULL )
    {
	send_to_char( "You hold nothing in your hand.\n\r", ch );
	return;
    }

    if ( wand->item_type != ITEM_WAND )
    {
	send_to_char( "You can zap only with a wand.\n\r", ch );
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
	    send_to_char( "Zap whom or what?\n\r", ch );
	    return;
	}
    }
    else
    {
	if ( ( victim = get_char_room ( ch, arg ) ) == NULL
	&&   ( obj    = get_obj_here  ( ch, arg ) ) == NULL )
	{
	    send_to_char( "You can't find it.\n\r", ch );
	    return;
	}
    }

    WAIT_STATE( ch, 2 * PULSE_VIOLENCE );

    if ( wand->value[2] > 0 )
    {
	if ( victim != NULL )
	{
	    act( "$n zaps $N with $p.", ch, wand, victim, TO_ROOM );
	    act( "You zap $N with $p.", ch, wand, victim, TO_CHAR );
	}
	else
	{
	    act( "$n zaps $P with $p.", ch, wand, obj, TO_ROOM );
	    act( "You zap $P with $p.", ch, wand, obj, TO_CHAR );
	}

 	if (ch->level+5 < wand->level 
	||  number_percent() >= 20 + get_skill(ch,gsn_wands) * 4/5) 
	{
	    act( "Your efforts with $p produce only smoke and sparks.",
		 ch,wand,NULL,TO_CHAR);
	    act( "$n's efforts with $p produce only smoke and sparks.",
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
	act( "$n's $p explodes into fragments.", ch, wand, NULL, TO_ROOM );
	act( "Your $p explodes into fragments.", ch, wand, NULL, TO_CHAR );
	extract_obj( wand );
    }

    return;
}



void do_steal( CHAR_DATA *ch, char *argument )
{
    char buf  [MAX_STRING_LENGTH];
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *tmp_ch;
    OBJ_DATA *obj;
    OBJ_DATA *obj_inve;
    int percent, number;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Steal what from whom?\n\r", ch );
	return;
    }

    if ( IS_NPC(ch) && IS_SET(ch->affected_by, AFF_CHARM) 
	&& (ch->master != NULL))
      {
	send_to_char( "You are to dazed to steal anything.\n\r", ch);
	return;
      }

    if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
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
	send_to_char("You'd better not -- you might get hit.\n\r",ch);
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
    if ( str_cmp( arg1, "coin"  )
    &&   str_cmp( arg1, "coins" )
    &&   str_cmp( arg1, "silver")
    &&   str_cmp( arg1, "gold"  ) )
    {
	if ( ( obj = get_obj_carry( victim, arg1 ) ) == NULL )
	{
	 send_to_char( "You can't find it.\n\r", ch );
	 return;
	}

    }

    if (obj != NULL && obj->pIndexData->limit != -1)
    {
      if ( ( IS_OBJ_STAT(obj, ITEM_ANTI_EVIL)    && IS_EVIL(ch)    )
      ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_GOOD)    && IS_GOOD(ch)    )
      ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_NEUTRAL) && IS_NEUTRAL(ch) ) )
      {
	act( "You are zapped by $p.", ch, obj, NULL, TO_CHAR );
	act( "$n is zapped by $p.",  ch, obj, NULL, TO_ROOM );
	percent = 0;
      }

      if ( obj->pIndexData->limit < obj->pIndexData->count )
      {
	act( "Gods doesn't allow $p to be stolen.", ch, obj, NULL, TO_CHAR );
	act( "Gods doesn't approve $n's behaviour.",  ch, obj, NULL, TO_ROOM );
	percent = 0;
      }
    }

    number = ( obj != NULL ) ? get_obj_number(obj) : 0;

    if ( ch->carry_number + number > can_carry_n( ch ) )
    {
	send_to_char( "You can't carry that much item.\n\r", ch );
	return;
    }
 
    if ( victim->position == POS_FIGHTING
	||  number_percent() > percent  )
    {
	/*
	 * Failure.
	 */

	send_to_char( "Oops.\n\r", ch );
        if ( !IS_AFFECTED( victim, AFF_SLEEP ) ) {
          victim->position= victim->position==POS_SLEEPING? POS_STANDING:
					victim->position;
	  act( "$n tried to steal from you.\n\r", ch, NULL, victim,TO_VICT  );
        }
	act( "$n tried to steal from $N.\n\r",  ch, NULL, victim,TO_NOTVICT);

	switch(number_range(0,3))
	{
	case 0 :
	   sprintf( buf, "%s is a lousy thief!", tmp_ch->name );
	   break;
        case 1 :
	   sprintf( buf, "%s couldn't rob %s way out of a paper bag!",
		    tmp_ch->name,(tmp_ch->sex == 2) ? "her" : "his");
	   break;
	case 2 :
	    sprintf( buf,"%s tried to rob me!",tmp_ch->name );
	    break;
	case 3 :
	    sprintf(buf,"Keep your hands out of there, %s!",tmp_ch->name);
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

    if ( !str_cmp( arg1, "coin"  )
    ||   !str_cmp( arg1, "coins" )
    ||   !str_cmp( arg1, "silver")
    ||   !str_cmp( arg1, "gold"  ) )
    {
	int amount_s = 0;
	int amount_g = 0;
        if ( !str_cmp( arg1, "silver" ) || 
             !str_cmp(arg1, "coin" ) ||
             !str_cmp(arg1, "coins" ) )
	  amount_s = victim->silver * number_range(1, 20) / 100;
        else if ( !str_cmp( arg1, "gold" ) )
	  amount_g = victim->gold * number_range(1, 7) / 100;

	if ( amount_s <= 0 && amount_g <= 0 )
	{
	    send_to_char( "You couldn't get any coins.\n\r", ch );
	    return;
	}

	ch->gold     += amount_g;
	victim->gold -= amount_g;
	ch->silver     += amount_s;
	victim->silver -= amount_s;
	sprintf( buf, "Bingo!  You got %d %s coins.\n\r", 
           amount_s!=0?amount_s:amount_g,
           amount_s!=0?"silver":"gold" );

	send_to_char( buf, ch );
	check_improve(ch,gsn_steal,TRUE,2);
	return;
    }

    if ( !can_drop_obj( ch, obj )
   /* ||   IS_SET(obj->extra_flags, ITEM_INVENTORY)*/
   /* ||  obj->level > ch->level */)
    {
	send_to_char( "You can't pry it away.\n\r", ch );
	return;
    }

    if ( ch->carry_number + get_obj_number( obj ) > can_carry_n( ch ) )
    {
	send_to_char( "You have your hands full.\n\r", ch );
	return;
    }

    if ( ch->carry_weight + get_obj_weight( obj ) > can_carry_w( ch ) )
    {
	send_to_char( "You can't carry that much weight.\n\r", ch );
	return;
    }

    if ( !IS_SET( obj->extra_flags, ITEM_INVENTORY ) )  {
      obj_from_char( obj );
      obj_to_char( obj, ch );
      send_to_char( "You got it!\n\r", ch );
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
      send_to_char( "You got one of them!\n\r", ch );
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
    char buf[MAX_STRING_LENGTH];
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
	send_to_char( "You can't do that here.\n\r", ch );
	return NULL;
    }

    if ( IS_SET(keeper->in_room->area->area_flag,AREA_HOMETOWN) 
	 && !IS_NPC(ch) && IS_SET(ch->act,PLR_WANTED) )
    {
	do_say( keeper, "Criminals are not welcome!" );
	sprintf( buf, "%s the CRIMINAL is over here!\n\r", ch->name );
	do_yell( keeper, buf );
	return NULL;
    }

    /*
     * Shop hours.
     */
    if ( time_info.hour < pShop->open_hour )
    {
	do_say( keeper, "Sorry, I am closed. Come back later." );
	return NULL;
    }
    
    if ( time_info.hour > pShop->close_hour )
    {
	do_say( keeper, "Sorry, I am closed. Come back tomorrow." );
	return NULL;
    }

    /*
     * Invisible or hidden people.
     */
    if ( !can_see( keeper, ch ) && !IS_IMMORTAL( ch ) )
    {
	do_say( keeper, "I don't trade with folks I can't see." );
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
	&&  !str_cmp(obj->short_descr,t_obj->short_descr))
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
OBJ_DATA *get_obj_keeper( CHAR_DATA *ch, CHAR_DATA *keeper, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
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
	    && !str_cmp(obj->short_descr,obj->next_content->short_descr))
		obj = obj->next_content;
        }
    }
 
    return NULL;
}

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
		&&   !str_cmp(obj->short_descr,obj2->short_descr) )
		  return 0;
/*
	 	    if (IS_OBJ_STAT(obj2,ITEM_INVENTORY))
			cost /= 2;
		    else
                    	cost = cost * 3 / 4;
*/
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



void do_buy( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int cost,roll;

    if ( argument[0] == '\0' )
    {
	send_to_char( "Buy what?\n\r", ch );
	return;
    }

    if ( IS_SET(ch->in_room->room_flags, ROOM_PET_SHOP) )
    {
	char arg[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];
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
	    bug( "Do_buy: bad pet shop at vnum %d.", ch->in_room->vnum );
	    send_to_char( "Sorry, you can't buy that here.\n\r", ch );
	    return;
	}

	in_room     = ch->in_room;
	ch->in_room = pRoomIndexNext;
	pet         = get_char_room( ch, arg );
	ch->in_room = in_room;

	if ( pet == NULL || !IS_SET(pet->act, ACT_PET) || !IS_NPC(pet) )
	{
	    send_to_char( "Sorry, you can't buy that here.\n\r", ch );
	    return;
	}

	if (IS_SET(pet->act,ACT_RIDEABLE) 
		&& ch->cabal == CABAL_KNIGHT
		&& !MOUNTED(ch) )
	{
 	 cost = 10 * pet->level * pet->level;

	 if ( (ch->silver + 100 * ch->gold) < cost )
	 {
	    send_to_char( "You can't afford it.\n\r", ch );
	    return;
	 }

	 if ( ch->level < pet->level + 5)
	 {
	    send_to_char(
		"You're not powerful enough to master this pet.\n\r", ch );
	    return;
	 }

	 deduct_cost(ch,cost);
	 pet = create_mobile( pet->pIndexData );
	 pet->comm = COMM_NOTELL|COMM_NOSHOUT|COMM_NOCHANNELS;

	 char_to_room( pet, ch->in_room );
	 do_mount(ch, pet->name);
	 send_to_char( "Enjoy your mount.\n\r", ch );
	 act( "$n bought $N as a mount.", ch, NULL, pet, TO_ROOM );
	 return;
	}

	if ( ch->pet != NULL )
	{
	    send_to_char("You already own a pet.\n\r",ch);
	    return;
	}

 	cost = 10 * pet->level * pet->level;

	if ( (ch->silver + 100 * ch->gold) < cost )
	{
	    send_to_char( "You can't afford it.\n\r", ch );
	    return;
	}

	if ( ch->level < pet->level )
	{
	    send_to_char(
		"You're not powerful enough to master this pet.\n\r", ch );
	    return;
	}

	/* haggle */
	roll = number_percent();
	if (roll < get_skill(ch,gsn_haggle))
	{
	    cost -= cost / 2 * roll / 100;
	    sprintf(buf,"You haggle the price down to %d coins.\n\r",cost);
	    send_to_char(buf,ch);
	    check_improve(ch,gsn_haggle,TRUE,4);
	
	}

	deduct_cost(ch,cost);
	pet			= create_mobile( pet->pIndexData );
	SET_BIT(pet->act, ACT_PET);
	SET_BIT(pet->affected_by, AFF_CHARM);
	pet->comm = COMM_NOTELL|COMM_NOSHOUT|COMM_NOCHANNELS;

	argument = one_argument( argument, arg );
	if ( arg[0] != '\0' )
	{
	    sprintf( buf, "%s %s", pet->name, arg );
	    free_string( pet->name );
	    pet->name = str_dup( buf );
	}

	sprintf( buf, "%sA neck tag says 'I belong to %s'.\n\r",
	    pet->description, ch->name );
	free_string( pet->description );
	pet->description = str_dup( buf );

	char_to_room( pet, ch->in_room );
	add_follower( pet, ch );
	pet->leader = ch;
	ch->pet = pet;
	send_to_char( "Enjoy your pet.\n\r", ch );
	act( "$n bought $N as a pet.", ch, NULL, pet, TO_ROOM );
	return;
    }
    else
    {
	CHAR_DATA *keeper;
	OBJ_DATA *obj,*t_obj;
	char arg[MAX_INPUT_LENGTH];
	int number, count = 1;

	if ( ( keeper = find_keeper( ch ) ) == NULL )
	    return;

	number = mult_argument(argument,arg);
	if ( number < -1 || number > 100)
	{
	    act( "$n tells you 'Get real!", keeper, NULL, ch, TO_VICT );
	    ch->reply = keeper;
	    return;
	}

	obj  = get_obj_keeper( ch,keeper, arg );
	cost = get_cost( keeper, obj, TRUE );

	if ( cost <= 0 || !can_see_obj( ch, obj ) )
	{
	    act( "$n tells you 'I don't sell that -- try 'list''.",
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
	    	&&  !str_cmp(t_obj->short_descr,obj->short_descr))
		    count++;
	    	else
		    break;
	    }

	    if (count < number)
	    {
	    	act("$n tells you 'I don't have that many in stock.'",
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
	    	act("$n tells you 'Gods will not approve me to sell that.'",
		    keeper,NULL,ch,TO_VICT);
	    	ch->reply = keeper;
	    	return;
	    }
	    if (count < number)
	    {
	    	act("$n tells you 'I don't have that many in stock.'",
		    keeper,NULL,ch,TO_VICT);
	    	ch->reply = keeper;
	    	return;
	    }
	}

	if ( (ch->silver + ch->gold * 100) < cost * number )
	{
	    if (number > 1)
		act("$n tells you 'You can't afford to buy that many.",
		    keeper,obj,ch,TO_VICT);
	    else
	    	act( "$n tells you 'You can't afford to buy $p'.",
		    keeper, obj, ch, TO_VICT );
	    ch->reply = keeper;
	    return;
	}
	
	if ( obj->level > ch->level )
	{
	    act( "$n tells you 'You can't use $p yet'.",
		keeper, obj, ch, TO_VICT );
	    ch->reply = keeper;
	    return;
	}

	if (ch->carry_number +  number * get_obj_number(obj) > can_carry_n(ch))
	{
	    send_to_char( "You can't carry that many items.\n\r", ch );
	    return;
	}

	if ( ch->carry_weight + number * get_obj_weight(obj) > can_carry_w(ch))
	{
	    send_to_char( "You can't carry that much weight.\n\r", ch );
	    return;
	}

	/* haggle */
	roll = number_percent();
	if (!IS_OBJ_STAT(obj,ITEM_SELL_EXTRACT) 
	&& roll < get_skill(ch,gsn_haggle))
	{
	    cost -= obj->cost / 2 * roll / 100;
	    act("You haggle with $N.",ch,NULL,keeper,TO_CHAR);
	    check_improve(ch,gsn_haggle,TRUE,4);
	}

	if (number > 1)
	{
	    sprintf(buf,"$n buys $p[%d].",number);
	    act(buf,ch,obj,NULL,TO_ROOM);
	    sprintf(buf,"You buy $p[%d] for %d silver.",number,cost * number);
	    act(buf,ch,obj,NULL,TO_CHAR);
	}
	else
	{
	    act( "$n buys $p.", ch, obj, NULL, TO_ROOM );
	    sprintf(buf,"You buy $p for %d silver.",cost);
	    act( buf, ch, obj, NULL, TO_CHAR );
	}
	deduct_cost(ch,cost * number);
	keeper->gold += cost * number/100;
	keeper->silver += cost * number - (cost * number/100) * 100;

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



void do_list( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

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
	    bug( "Do_list: bad pet shop at vnum %d.", ch->in_room->vnum );
	    send_to_char( "You can't do that here.\n\r", ch );
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
		    send_to_char( "Pets for sale:\n\r", ch );
		}
		sprintf( buf, "[%2d] %8d - %s\n\r",
		    pet->level,
		    10 * pet->level * pet->level,
		    pet->short_descr );
		send_to_char( buf, ch );
	    }
	}
	if ( !found )
	    send_to_char( "Sorry, we're out of pets right now.\n\r", ch );
	return;
    }
    else
    {
	CHAR_DATA *keeper;
	OBJ_DATA *obj;
	int cost,count;
	bool found;
	char arg[MAX_INPUT_LENGTH];

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
		    send_to_char( "[Lv Price Qty] Item\n\r", ch );
		}

		if (IS_OBJ_STAT(obj,ITEM_INVENTORY))
		    sprintf(buf,"[%2d %5d -- ] %s%s\n\r",
			obj->level,cost,obj->short_descr,
			(obj->pIndexData->limit != -1) ?  (obj->pIndexData->count > obj->pIndexData->limit) ? " (NOT AVAILABLE NOW)" : " (AVAILABLE)" : "" );
		else
		{
		    count = 1;

		    while (obj->next_content != NULL 
		    && obj->pIndexData == obj->next_content->pIndexData
		    && !str_cmp(obj->short_descr,
			        obj->next_content->short_descr))
		    {
			obj = obj->next_content;
			count++;
		    }
		    sprintf(buf,"[%2d %5d %2d ] %s\n\r",
			obj->level,cost,count,obj->short_descr);
		}
		send_to_char( buf, ch );
	    }
	}

	if ( !found )
	    send_to_char( "You can't buy anything here.\n\r", ch );
	return;
    }
}



void do_sell( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *keeper;
    OBJ_DATA *obj;
    int cost,roll;
    int gold, silver;
    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Sell what?\n\r", ch );
	return;
    }

    if ( ( keeper = find_keeper( ch ) ) == NULL )
	return;

    if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
    {
	act( "$n tells you 'You don't have that item'.",
	    keeper, NULL, ch, TO_VICT );
	ch->reply = keeper;
	return;
    }

    if ( !can_drop_obj( ch, obj ) )
    {
	send_to_char( "You can't let go of it.\n\r", ch );
	return;
    }

    if (!can_see_obj(keeper,obj))
    {
	act("$n doesn't see what you are offering.",keeper,NULL,ch,TO_VICT);
	return;
    }

    if ( ( cost = get_cost( keeper, obj, FALSE ) ) <= 0 )
    {
	act( "$n looks uninterested in $p.", keeper, obj, ch, TO_VICT );
	return;
    }
    if ( cost > (keeper->silver + 100 * keeper->gold) )
    {
	act("$n tells you 'I'm afraid I don't have enough wealth to buy $p.",
	    keeper,obj,ch,TO_VICT);
	return;
    }

    act( "$n sells $p.", ch, obj, NULL, TO_ROOM );
    /* haggle */
    roll = number_percent();
    if (!IS_OBJ_STAT(obj,ITEM_SELL_EXTRACT) && roll < get_skill(ch,gsn_haggle))
    {
        roll = get_skill(ch, gsn_haggle) + number_range(1, 20) - 10;
        send_to_char("You haggle with the shopkeeper.\n\r",ch);
        cost += obj->cost / 2 * roll / 100;
        cost = UMIN(cost,95 * get_cost(keeper,obj,TRUE) / 100);
	cost = UMIN(cost,(keeper->silver + 100 * keeper->gold));
        check_improve(ch,gsn_haggle,TRUE,4);
    }

    silver = cost - (cost/100) * 100;
    gold   = cost/100;
 
    sprintf( buf2, "You sell $p for %s %s%spiece%s.",
      silver!=0?"%d silver":"",                         /* silvers  */
      (silver!=0 && gold != 0)?"and ":"",		/*   and    */
      gold!=0?"%d gold ":"",				/*  golds   */
      silver+gold>1?"s":"" );				/* piece(s) */
    sprintf( buf, buf2, silver, gold );
           
    act( buf, ch, obj, NULL, TO_CHAR );
    ch->gold     += gold;
    ch->silver 	 += silver;
    deduct_cost(keeper,cost);
    if ( keeper->gold < 0 )
	keeper->gold = 0;
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



void do_value( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *keeper;
    OBJ_DATA *obj;
    int cost;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Value what?\n\r", ch );
	return;
    }

    if ( ( keeper = find_keeper( ch ) ) == NULL )
	return;

    if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
    {
	act( "$n tells you 'You don't have that item'.",
	    keeper, NULL, ch, TO_VICT );
	ch->reply = keeper;
	return;
    }

    if (!can_see_obj(keeper,obj))
    {
        act("$n doesn't see what you are offering.",keeper,NULL,ch,TO_VICT);
        return;
    }

    if ( !can_drop_obj( ch, obj ) )
    {
	send_to_char( "You can't let go of it.\n\r", ch );
	return;
    }

    if ( ( cost = get_cost( keeper, obj, FALSE ) ) <= 0 )
    {
	act( "$n looks uninterested in $p.", keeper, obj, ch, TO_VICT );
	return;
    }

    sprintf( buf, 
	"$n tells you 'I'll give you %d silver and %d gold coins for $p'.", 
	cost - (cost/100) * 100, cost/100 );
    act( buf, keeper, obj, ch, TO_VICT );
    ch->reply = keeper;

    return;
}

void do_wanted( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
 
    if (ch_skill_nok(ch,gsn_wanted))
      return;

    if (!cabal_ok(ch,gsn_wanted))
      return;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
      {
        send_to_char( "Usage: wanted <player> <Y|N>\n\r", ch );
        return;
      }

    victim = get_char_world(ch, arg1);

    if ( (victim == NULL) || 
	!(can_see(ch, victim)) )
      {
        send_to_char( "They aren't here.\n\r", ch );
        return;
      }
    
    if (victim->level >= LEVEL_IMMORTAL && (ch->level < victim->level)) 
      {
	act( "You do not have the power to arrest $N.", ch, NULL, victim, 
	    TO_CHAR);
	return;
      }
    
    if (victim == ch)
      {
	send_to_char( "You cannot do that to yourself.\n\r", ch);
	return;
      }

    switch(arg2[0]) 
      {
      case 'Y':
      case 'y':
	if (IS_SET(victim->act ,PLR_WANTED))
	  act( "$n is already wanted.", ch, NULL, NULL, TO_CHAR);
	else 
	  {
	    SET_BIT(victim->act, PLR_WANTED);
	    act( "$n is now WANTED!!!",victim, NULL, ch, TO_NOTVICT);
	    send_to_char( "You are now WANTED!!!\n\r", victim);
	    send_to_char( "Ok.\n\r", ch);
	  }
	break;
      case 'N':
      case 'n':
	if (!IS_SET(victim->act,PLR_WANTED))
	  act( "$N is not wanted.", ch, NULL, victim, TO_CHAR);
	else
	  {
	    REMOVE_BIT(victim->act, PLR_WANTED);
	    act( "$n is no longer wanted.",victim, NULL, ch, TO_NOTVICT);
	    send_to_char( "You are no longer wanted.\n\r", victim);
	    send_to_char( "Ok.\n\r", ch);
	  }
	break;
      default:
        send_to_char( "Usage: wanted <player> <Y|N>\n\r", ch );
	break;
      }
  }

void do_herbs(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
  
  one_argument(argument,arg);
  
  if (IS_NPC(ch))
    return;

  if (is_affected(ch,gsn_herbs))
    {
      send_to_char("You can't find any more herbs.\n\r",ch);
      return;
    }

  if (arg[0] == '\0')
    victim = ch;
  else if ( (victim = get_char_room(ch,arg)) == NULL)
    {
      send_to_char("They're not here.\n\r",ch);
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

      send_to_char("You gather some beneficial herbs.\n\r",ch);
      act("$n gathers some herbs.",ch,NULL,NULL,TO_ROOM);
      
      if (ch != victim)
	{
	  act("$n gives you some herbs to eat.",ch,NULL,victim,TO_VICT);
	  act("You give the herbs to $N.",ch,NULL,victim,TO_CHAR);
	  act("$n gives the herbs to $N.",ch,NULL,victim,TO_NOTVICT);
	}
	      
      if (victim->hit < victim->max_hit)
	{
	  send_to_char("You feel better.\n\r",victim);
	  act("$n looks better.",victim,NULL,NULL,TO_ROOM);
	}
      victim->hit = UMIN(victim->max_hit,victim->hit + 5 * ch->level);
      check_improve(ch,gsn_herbs,TRUE,1);
      if (is_affected(victim, gsn_plague))
	{
	  if (check_dispel(ch->level,victim,gsn_plague))
	    { 
	      send_to_char("Your sores vanish.\n\r",victim);
	      act("$n looks relieved as $s sores vanish.",victim,NULL,NULL,TO_ROOM);
	    }
	}
    }
  else
    {
      send_to_char("You search for herbs but find none here.\n\r",ch);
      act("$n looks around for herbs.",ch,NULL,NULL,TO_ROOM);
      check_improve(ch,gsn_herbs,FALSE,1);
    }
}

void do_lore( CHAR_DATA *ch, char *argument )
{
  char arg1[MAX_INPUT_LENGTH];
  OBJ_DATA *obj;
  char buf[MAX_STRING_LENGTH];
  AFFECT_DATA *paf;
  int chance;
  int value0, value1, value2, value3;

  argument = one_argument( argument, arg1 );
  
  if ( ( obj = get_obj_carry( ch, arg1 ) ) == NULL )
    {
      send_to_char( "You do not have that object.\n\r", ch );
      return;
    }
  
  if (ch->mana < 30)
    {
      send_to_char("You don't have enough mana.\n\r", ch);
      return;
    }
  
  if ( IS_NPC(ch) || ch->level < skill_table[gsn_lore].skill_level[ch->iclass]
      || get_skill(ch,gsn_lore) < 10)
    {
      send_to_char("The meaning of this object escapes you for the moment.\n\r", ch);
      return;
    }

  /* a random lore */  
  chance = number_percent();
  
  if (get_skill(ch,gsn_lore) < 20)
    {
      sprintf( buf, "Object '%s'.\n\r", obj->name);
      send_to_char( buf, ch );
      ch->mana -= 30;
      check_improve(ch,gsn_lore,TRUE,8);
      return;
    }
  
  else if (get_skill(ch,gsn_lore) < 40)
    {    
      sprintf( buf,
	  "Object '%s'.  Weight is %d, value is %d.\n\r",
	      obj->name,
	      chance < 60 ? obj->weight : number_range(1, 2 * obj->weight),
	      chance < 60 ? number_range(1, 2 * obj->cost) : obj->cost
	      );
      send_to_char( buf, ch );
      if ( str_cmp( obj->material, "oldstyle" ) )  {
        sprintf( buf, "Material is %s.\n\r", obj->material );
        send_to_char( buf, ch );
      }
      ch->mana -= 30;
      check_improve(ch,gsn_lore,TRUE,7);
      return;
    }
  
  else if (get_skill(ch,gsn_lore) < 60)
    {
      sprintf( buf,
	      "Object '%s' has weight %d.\n\rValue is %d, level is %d.\n\rMaterial is %s.\n\r",
	      obj->name,
	      obj->weight,
	      chance < 60 ? number_range(1, 2 * obj->cost) : obj->cost,
	      chance < 60 ? obj->level : number_range(1, 2 * obj->level),
	  str_cmp(obj->material,"oldstyle")?obj->material:"unknown"
	      );
      send_to_char( buf, ch );
      ch->mana -= 30;
      check_improve(ch,gsn_lore,TRUE,6);
      return;
    }
  
  else if (get_skill(ch,gsn_lore) < 80)
    {
      sprintf( buf,
	      "Object '%s' is type %s, extra flags %s.\n\rWeight is %d, value is %d, level is %d.\n\rMaterial is %s.\n\r",
	      obj->name,
	      item_type_name( obj ),
	      extra_bit_name( obj->extra_flags ),
	      obj->weight,
	      chance < 60 ? number_range(1, 2 * obj->cost) : obj->cost,
	      chance < 60 ? obj->level : number_range(1, 2 * obj->level),
	  str_cmp(obj->material,"oldstyle")?obj->material:"unknown"
	      );
      send_to_char( buf, ch );
      ch->mana -= 30;
      check_improve(ch,gsn_lore,TRUE,5);
      return;
    }

  else if (get_skill(ch,gsn_lore) < 85)
    {
      sprintf( buf,
	      "Object '%s' is type %s, extra flags %s.\n\rWeight is %d, value is %d, level is %d.\n\rMaterial is %s.\n\r",
	      obj->name,
	      item_type_name( obj ),
	      extra_bit_name( obj->extra_flags ),
	      obj->weight,
	      obj->cost,
	      obj->level,
	  str_cmp(obj->material,"oldstyle")?obj->material:"unknown"
	      );
      send_to_char( buf, ch );
    }
  else
    {
      sprintf( buf,
	      "Object '%s' is type %s, extra flags %s.\n\rWeight is %d, value is %d, level is %d.\n\rMaterial is %s.\n\r",
	      obj->name,
	      item_type_name( obj ),
	      extra_bit_name( obj->extra_flags ),
	      obj->weight,
	      obj->cost,
	      obj->level,
	  str_cmp(obj->material,"oldstyle")?obj->material:"unknown"
	      );
      send_to_char( buf, ch );
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
      
      sprintf( buf, "Level %d spells of:", obj->value[0] );
      send_to_char( buf, ch );
      
      if ( value1 >= 0 && value1 < MAX_SKILL )
	{
	  send_to_char( " '", ch );
	  send_to_char( skill_table[value1].name, ch );
	  send_to_char( "'", ch );
	}
      
      if ( value2 >= 0 && value2 < MAX_SKILL )
	{
	  send_to_char( " '", ch );
	  send_to_char( skill_table[value2].name, ch );
	  send_to_char( "'", ch );
	}
      
      if ( value3 >= 0 && value3 < MAX_SKILL )
	{
	  send_to_char( " '", ch );
	  send_to_char( skill_table[value3].name, ch );
	  send_to_char( "'", ch );
	}
      
      send_to_char( ".\n\r", ch );
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
      
      sprintf( buf, "Has %d(%d) charges of level %d",
	      value1, value2, value0 );
      send_to_char( buf, ch );
      
      if ( value3 >= 0 && value3 < MAX_SKILL )
	  {
	    send_to_char( " '", ch );
	    send_to_char( skill_table[value3].name, ch );
	    send_to_char( "'", ch );
	  }
      
      send_to_char( ".\n\r", ch );
      break;
      
    case ITEM_WEAPON:
      send_to_char("Weapon type is ",ch);
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
	case(WEAPON_EXOTIC) : send_to_char("exotic.\n\r",ch);	break;
	case(WEAPON_SWORD)  : send_to_char("sword.\n\r",ch);	break;
	case(WEAPON_DAGGER) : send_to_char("dagger.\n\r",ch);	break;
	case(WEAPON_SPEAR)	: send_to_char("spear/staff.\n\r",ch);	break;
	case(WEAPON_MACE) 	: send_to_char("mace/club.\n\r",ch);	break;
	case(WEAPON_AXE)	: send_to_char("axe.\n\r",ch);		break;
	case(WEAPON_FLAIL)	: send_to_char("flail.\n\r",ch);	break;
	case(WEAPON_WHIP)	: send_to_char("whip.\n\r",ch);		break;
	case(WEAPON_POLEARM): send_to_char("polearm.\n\r",ch);	break;
	case(WEAPON_BOW)	: send_to_char("bow.\n\r",ch);	break;
	case(WEAPON_ARROW)	: send_to_char("arrow.\n\r",ch);	break;
	case(WEAPON_LANCE)	: send_to_char("lance.\n\r",ch);	break;
	default		: send_to_char("unknown.\n\r",ch);	break;
 	}
      if (obj->pIndexData->new_format)
	sprintf(buf,"Damage is %dd%d (average %d).\n\r",
		value1,value2,
		(1 + value2) * value1 / 2);
      else
	sprintf( buf, "Damage is %d to %d (average %d).\n\r",
	    	value1, value2,
	    	( value1 + value2 ) / 2 );
      send_to_char( buf, ch );
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

      sprintf( buf, 
	      "Armor class is %d pierce, %d bash, %d slash, and %d vs. magic.\n\r", 
	      value0, value1, value2, value3 );
      send_to_char( buf, ch );
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
	    sprintf( buf, "Affects %s by %d.\n\r",
		    affect_loc_name( paf->location ), paf->modifier );
	    send_to_char( buf, ch );
	  }
      }
  
  for ( paf = obj->affected; paf != NULL; paf = paf->next )
    {
      if ( paf->location != APPLY_NONE && paf->modifier != 0 )
	{
	  sprintf( buf, "Affects %s by %d.\n\r",
		  affect_loc_name( paf->location ), paf->modifier );
	  send_to_char( buf, ch );
	}
    }
  check_improve(ch,gsn_lore,TRUE,5);
  return;
}



void do_butcher(CHAR_DATA *ch, char *argument)
{
  OBJ_DATA *obj;
  char buf[MAX_STRING_LENGTH];
  char arg[MAX_STRING_LENGTH];
  OBJ_DATA *tmp_obj;
  OBJ_DATA *tmp_next;

  if (IS_NPC(ch))
    return;

  one_argument(argument,arg);
  if ( arg[0]=='\0' )
  {
    send_to_char( "Butcher what?\n\r", ch );
    return;
  }
  if ( (obj = get_obj_here(ch,arg)) == NULL )
    {
      send_to_char("You do not see that here.\n\r",ch);
      return;
    }

  if (obj->item_type != ITEM_CORPSE_PC && obj->item_type != ITEM_CORPSE_NPC)
    {
      send_to_char("You can't butcher that.\n\r",ch);
      return;
    }

  if (obj->carried_by != NULL)
    {
      send_to_char("Put it down first.\n\r",ch);
      return;
    }

  if ( !IS_NPC(ch) && get_skill(ch,gsn_butcher) < 1)
    {
      send_to_char("You don't have the precision instruments for that.", ch);
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
        
      numsteaks = number_bits(2) + 1; 
      
      if (numsteaks > 1)
	{
	  sprintf(buf, "$n butchers $p and creates %i steaks.",numsteaks);
	  act(buf,ch,obj,NULL,TO_ROOM);

	  sprintf(buf, "You butcher $p and create %i steaks.",numsteaks);
	  act(buf,ch,obj,NULL,TO_CHAR);
	}

      else 
	{
	  act("$n butchers $p and creates a steak."
	      ,ch,obj,NULL,TO_ROOM);

	  act("You butcher $p and create a steak."
	      ,ch,obj,NULL,TO_CHAR);
	}
      check_improve(ch,gsn_butcher,TRUE,1);

      for (i=0; i < numsteaks; i++)
	{
	  steak = create_object(get_obj_index(OBJ_VNUM_STEAK),0);
	  sprintf( buf, steak->short_descr, obj->short_descr);
	  free_string( steak->short_descr );
	  steak->short_descr = str_dup( buf );

	  sprintf( buf, steak->description, obj->short_descr );
	  free_string( steak->description );
	  steak->description = str_dup( buf );

	  obj_to_room(steak,ch->in_room);
	}
    }	
  else 
    {
      act("You fail and destroy $p.",ch,obj,NULL,TO_CHAR);
      act("$n fails to butcher $p and destroys it.",
	  ch,obj,NULL,TO_ROOM);

      check_improve(ch,gsn_butcher,FALSE,1);
    }
  extract_obj(obj);
}


void do_balance(CHAR_DATA *ch, char *argument)
{
  char buf[160];
  char buf2[100];
  long bank_g;
  long bank_s;

  if (IS_NPC(ch))
    {
      send_to_char("You don't have a bank account.\n\r",ch);
      return;
    }
  
  if (!IS_SET(ch->in_room->room_flags, ROOM_BANK))
    {
      send_to_char("You are not in a bank.\n\r",ch);
      return;
    }


  if ( ch->pcdata->bank_s + ch->pcdata->bank_g == 0 )  {
    send_to_char( "You don't have any money in the bank.\n\r", ch );
    return;
  }

  bank_g = ch->pcdata->bank_g;
  bank_s = ch->pcdata->bank_s;
  sprintf( buf, "You have %s%s%s coin%s in the bank.\n\r",
    bank_g!=0?"%ld gold":"",
    (bank_g!=0)&&(bank_s!=0)?" and ":"",
    bank_s!=0?"%ld silver":"",
    bank_s+bank_g>1?"s":"" );
  if (bank_g == 0)
    sprintf( buf2, buf, bank_s );
  else
    sprintf( buf2, buf, bank_g, bank_s );

  send_to_char(buf2, ch);
}

void do_withdraw(CHAR_DATA *ch, char *argument)
{
  long  amount_s;
  long  amount_g;
  char arg[MAX_INPUT_LENGTH];
  char buf[100];
  int weight;

  if (IS_NPC(ch))
    {
      send_to_char("You don't have a bank account.\n\r",ch);
      return;
    }
  
  if (!IS_SET(ch->in_room->room_flags, ROOM_BANK))
    {
      send_to_char("The mosquito by your feet will not give you any money.\n\r",ch);
      return;
    }  

  argument = one_argument( argument, arg );
  if ( arg[0] == '\0' )  {
    send_to_char( "Withdraw how much?\n\r", ch );
    return;
  }
    
  amount_s = labs (atol(arg));
  if ( !str_cmp( argument, "silver") || argument[0] == '\0' )
    amount_g = 0;
  else if ( !str_cmp( argument, "gold" ) )  {
    amount_g = amount_s;
    amount_s = 0;
  }
  else {
    send_to_char( "You can withdraw gold and silver coins only.", ch );
    return;
  }

  if ( amount_g > ch->pcdata->bank_g)
  {
      send_to_char("Sorry, we don't give loans.\n\r",ch);
      return;
  }

  if ( amount_s > ch->pcdata->bank_s)
  {
      send_to_char("Sorry, we don't give loans.\n\r",ch);
      return;
  }

  weight = amount_g * 2 / 5;
  weight += amount_s / 10;

  if ( get_carry_weight(ch) + weight > can_carry_w(ch) )
  {
     act( "You can't carry that much weight.", ch,NULL,NULL, TO_CHAR);
     return;
  }

  ch->pcdata->bank_g -= amount_g;
  ch->pcdata->bank_s -= amount_s;
  ch->gold += (long)(0.98 * amount_g);
  ch->silver += (long)(0.90 * amount_s);
  if (amount_s > 0  && amount_s < 10 )  {
    if ( amount_s == 1 )
      sprintf(buf, "One coin?? You cheapskate!\n\r");
    else
      sprintf(buf, "%ld coins?? You cheapskate!\n\r", amount_s);
  }
  else 
    sprintf(buf, 
	    "Here are your %ld %s coins, minus a %ld coin withdrawal fee.\n\r",
	    amount_s!=0?amount_s:amount_g, 
	    amount_s!=0?"silver":"gold",
	    amount_s!=0?(long) UMAX(1, (0.10 * amount_s)):
                        (long) UMAX(1, (0.02 * amount_g)) );
  send_to_char(buf, ch);
  act("$n steps up to the teller window.",ch,NULL,NULL,TO_ROOM);
}

void do_deposit(CHAR_DATA *ch, char *argument)
{
  long amount_s;
  long amount_g;
  char buf[100];
  char arg[200];

  if (IS_NPC(ch))
    {
      send_to_char("You don't have a bank account.\n\r",ch);
      return;
    }
  
  if (!IS_SET(ch->in_room->room_flags, ROOM_BANK))
    {
      send_to_char("The ant by your feet can't carry your gold.\n\r",ch);
      return;
    }  

  argument = one_argument( argument, arg );
  if ( arg[0] == '\0' )  {
    send_to_char( "Deposit how much?\n\r", ch );
    return;
  }
  amount_s = labs (atol(arg));
  if ( !str_cmp( argument, "silver" ) || argument[0] == '\0' )
    amount_g = 0;
  else if ( !str_cmp( argument, "gold" ) )  {
    amount_g = amount_s;
    amount_s = 0;
  }
  else {
    send_to_char( "You can deposit gold and silver coins only.", ch );
    return;
  }
  
  if (amount_g > ch->gold)
    {
      send_to_char("That's more than you've got.\n\r",ch);
      return;
    }
  if (amount_s > ch->silver)
    {
      send_to_char("That's more than you've got.\n\r",ch);
      return;
    }

  if ( (amount_g + ch->pcdata->bank_g) > 400000 )
    {
      send_to_char("Bank cannot accept more than 400,000 gold.\n\r",ch);
      return;
    }
	
  ch->pcdata->bank_s += amount_s;
  ch->pcdata->bank_g += amount_g;
  ch->gold -= amount_g;
  ch->silver -= amount_s;

  if (amount_s == 1)
    sprintf(buf, "Oh boy! One gold coin!\n\r");
  else sprintf(buf, "%ld %s coins deposited. Come again soon!\n\r",
                amount_s!=0?amount_s:amount_g,
                amount_s!=0?"silver":"gold" );

  send_to_char(buf, ch);
  act("$n steps up to the teller window.",ch,NULL,NULL,TO_ROOM);
}



void do_enchant(CHAR_DATA *ch, char *argument)
{
    OBJ_DATA *obj;
    char buf[MAX_STRING_LENGTH]; 
    int wear_level;

    if ( !IS_NPC(ch)
    &&   ch->level < skill_table[gsn_enchant_sword].skill_level[ch->iclass] )
    {
	send_to_char("Huh?.\n\r", ch );
	return;
    }

    if (argument[0] == '\0') /* empty */
    {
        send_to_char ("Wear which weapon to enchant?\n\r",ch);
        return;
    }

    obj = get_obj_carry (ch, argument); 

    if (obj == NULL)
    {
        send_to_char ("You don't have that item.\n\r",ch);
        return;
    }


    wear_level = ch->level;

    if ( (class_table[ch->iclass].fMana && obj->item_type == ITEM_ARMOR)
	|| (!class_table[ch->iclass].fMana && obj->item_type == ITEM_WEAPON))
		wear_level += 3;	     

    if ( wear_level < obj->level )
    {
  sprintf( buf, "You must be level %d to be able to enchant this object.\n\r",
            obj->level );
        send_to_char( buf, ch );
        act( "$n tries to enchant $p, but is too inexperienced.",
            ch, obj, NULL, TO_ROOM );
        return;
    }

   if (ch->mana < 100 )
	{
	 send_to_char("You don't have enough mana.\n\r",ch);
	 return;
	}

   if ( number_percent() > get_skill(ch,gsn_enchant_sword) )
	{
	 send_to_char("You lost your concentration.\n\r",ch);
        act( "$n tries to enchant $p, but he forgets how for a moment.",
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
    act( "$n lights $p and holds it.", ch, obj, NULL, TO_ROOM );
    act( "You light $p and hold it.",  ch, obj, NULL, TO_CHAR );
    equip_char( ch, obj, iWear );
}

void hold_a_shield(CHAR_DATA *ch,OBJ_DATA *obj, int iWear)
{
	act( "$n wears $p as a shield.", ch, obj, NULL, TO_ROOM );
	act( "You wear $p as a shield.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, iWear );
}

void hold_a_thing(CHAR_DATA *ch,OBJ_DATA *obj, int iWear)
{
	act( "$n holds $p in $s hand.",   ch, obj, NULL, TO_ROOM );
	act( "You hold $p in your hand.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, iWear );
}

/* wear object as a secondary weapon */
void hold_a_wield(CHAR_DATA *ch, OBJ_DATA *obj, int iWear)
{
    int sn,skill;

    if (obj == NULL)
    {
        bug("Hold_a_wield: Obj NULL",0);
        return;
    }

    if (obj->item_type != ITEM_WEAPON)
    {
	hold_a_thing(ch, obj, iWear);
	return;
    }

    act( "$n wields $p.", ch, obj, NULL, TO_ROOM );
    act( "You wield $p.", ch, obj, NULL, TO_CHAR );
    equip_char ( ch, obj, iWear);
    if (get_wield_char(ch,TRUE) == obj) sn = get_weapon_sn(ch,TRUE);
    else sn = get_weapon_sn(ch,FALSE);

    if  (sn)
      {
        skill = get_weapon_skill(ch,sn);
 
        if (skill >= 100)
            act("$p feels like a part of you!",ch,obj,NULL,TO_CHAR);
        else if (skill > 85)
            act("You feel quite confident with $p.",ch,obj,NULL,TO_CHAR);
        else if (skill > 70)
            act("You are skilled with $p.",ch,obj,NULL,TO_CHAR);
        else if (skill > 50)
            act("Your skill with $p is adequate.",ch,obj,NULL,TO_CHAR);
        else if (skill > 25)
            act("$p feels a little clumsy in your hands.",ch,obj,NULL,TO_CHAR);
        else if (skill > 1)
            act("You fumble and almost drop $p.",ch,obj,NULL,TO_CHAR);
        else
            act("You don't even know which end is up on $p.",
                ch,obj,NULL,TO_CHAR);
      }

    return;
}


void wear_a_wield(CHAR_DATA *ch,OBJ_DATA *obj, bool fReplace)
{
  if ( !IS_NPC(ch) 
	&& get_obj_weight(obj) > str_app[get_curr_stat(ch,STAT_STR)].carry )
  {
    send_to_char( "It is too heavy for you to wield.\n\r", ch );
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
     if ( get_eq_char(ch,WEAR_RIGHT) )
	if (!remove_obj_loc(ch,WEAR_RIGHT,fReplace)) return;
     if ( get_eq_char(ch,WEAR_LEFT) )
	if (!remove_obj_loc(ch,WEAR_LEFT,fReplace)) return;
     hold_a_wield(ch, obj, WEAR_BOTH);
    }
   }
  else 
  {
    if (get_eq_char(ch,WEAR_BOTH) != NULL )
    {
     if (!remove_obj_loc( ch, WEAR_BOTH, fReplace ) )
	return;
     hold_a_wield(ch, obj, WEAR_RIGHT);
    }
    else if (get_eq_char(ch,WEAR_RIGHT) == NULL )
		hold_a_wield(ch, obj, WEAR_RIGHT);
    else if (get_eq_char(ch,WEAR_LEFT) == NULL )
		hold_a_wield(ch, obj, WEAR_LEFT);
    else if ( remove_obj_loc( ch, WEAR_RIGHT, fReplace ) )
		hold_a_wield(ch, obj, WEAR_RIGHT);
    else if ( remove_obj_loc( ch, WEAR_LEFT, fReplace ) )
		hold_a_wield(ch, obj, WEAR_LEFT);
    else send_to_char("You found your hands full.\n\r",ch);
  }
}



void wear_multi(CHAR_DATA *ch,OBJ_DATA *obj,int iWear,bool fReplace)
{
 if (count_worn(ch, iWear) < max_can_wear(ch, iWear))
  {
   switch( iWear )
   {
    case WEAR_FINGER:
	act( "$n wears $p on one of $s finger.", ch, obj, NULL, TO_ROOM );
	act( "You wear $p on one of your finger.", ch, obj, NULL, TO_CHAR );
	break;
    case WEAR_NECK:
	act( "$n wears $p around $s neck.",   ch, obj, NULL, TO_ROOM );
	act( "You wear $p around your neck.", ch, obj, NULL, TO_CHAR );
	break;
    case WEAR_WRIST:
	act("$n wears $p around one of $s wrist.", ch, obj, NULL, TO_ROOM );
	act("You wear $p around one of your wrist.",ch, obj, NULL, TO_CHAR);
	break;
    case WEAR_TATTOO:
	act("$n now uses $p as tattoo of $s religion.",ch,obj,NULL,TO_ROOM);
	act("You now use $p as the tattoo of your religion.",ch,obj,NULL,TO_CHAR);
	break;
    default:
	act("$n wears $p around somewhere.", ch, obj, NULL, TO_ROOM );
	act("You wear $p around somewhere.",ch, obj, NULL, TO_CHAR);
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
          act( "$n stops using $p.", ch, w, NULL, TO_ROOM );
          act( "You stop using $p.", ch, w, NULL, TO_CHAR );
	  wear_multi(ch, obj, iWear, fReplace );
	  not_worn = 0;
          break;
	}
    }

    if ( not_worn )
          act( "You couldn't remove anything to replace with $p.", 
		ch, obj, NULL, TO_CHAR );
  }  

}


