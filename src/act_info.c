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
#include <sys/time.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include "merc.h"
#include "magic.h"
#include "recycle.h"
#include "tables.h"
#include "lookup.h"

/* command procedures needed */
DECLARE_DO_FUN(	do_exits	);
DECLARE_DO_FUN( do_look		);
DECLARE_DO_FUN( do_help		);
DECLARE_DO_FUN( do_affects	);
DECLARE_DO_FUN( do_murder	);
DECLARE_DO_FUN( do_say  	);
DECLARE_DO_FUN( do_scan2  	);
DECLARE_DO_FUN( do_score_col  	);
DECLARE_DO_FUN( do_affects_col 	);
DECLARE_DO_FUN( do_who_col	);
DECLARE_DO_FUN( do_autolist_col	);
DECLARE_DO_FUN( do_nscore_col  	);

char *get_stat_alias( CHAR_DATA *ch, int which );

char *	const	where_name	[] =
{
    "<worn on finger>    ",
    "<worn around neck>  ",
    "<worn on torso>     ",
    "<worn on head>      ",
    "<worn on legs>      ",
    "<worn on feet>      ",
    "<worn on hands>     ",
    "<worn on arms>      ",
    "<worn about body>   ",
    "<worn about waist>  ",
    "<worn around wrist> ",
    "<left hand holds>%c  ",
    "<right hand holds>%c ",
    "<both hands hold>   ",
    "<floating nearby>   ",
    "<scratched tattoo>  ",
    "<stuck in>          "
};


/* for do_count */
int max_on = 0;



/*
 * Local functions.
 */
char *	format_obj_to_char	args( ( OBJ_DATA *obj, CHAR_DATA *ch,
				    bool fShort ) );
void	show_list_to_char	args( ( OBJ_DATA *list, CHAR_DATA *ch,
				    bool fShort, bool fShowNothing ) );
void	show_char_to_char_0	args( ( CHAR_DATA *victim, CHAR_DATA *ch ) );
void	show_char_to_char_1	args( ( CHAR_DATA *victim, CHAR_DATA *ch ) );
void	show_char_to_char	args( ( CHAR_DATA *list, CHAR_DATA *ch ) );
bool	check_blind		args( ( CHAR_DATA *ch ) );
bool	show_vwear_to_char	args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );
bool	show_cwear_to_char	args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );


   

bool show_vwear_to_char( CHAR_DATA *ch, OBJ_DATA *obj )
{
 char buf[MAX_STRING_LENGTH];

 if (can_see_obj( ch, obj ) )
   {
       sprintf(buf,where_name[obj->wear_loc], ' ');
       send_to_char( buf, ch );
       send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
       send_to_char( "\n\r", ch );
       return TRUE;
   }
 return FALSE;
}


bool show_cwear_to_char( CHAR_DATA *ch, OBJ_DATA *obj )
{
 char buf[MAX_STRING_LENGTH];

  if ((obj->wear_loc == WEAR_LEFT && LEFT_HANDER(ch))
	|| (obj->wear_loc == WEAR_RIGHT && RIGHT_HANDER(ch)))
       sprintf(buf,where_name[obj->wear_loc], '*');
  else sprintf(buf,where_name[obj->wear_loc], ' ');
  send_to_char(buf, ch );
  if ( can_see_obj( ch, obj ) )
	send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
  else send_to_char( "something.\n\r", ch );
  send_to_char("\n\r", ch);

 return TRUE;
}



char *format_obj_to_char( OBJ_DATA *obj, CHAR_DATA *ch, bool fShort )
{
    static char buf[MAX_STRING_LENGTH];
    static char buf_con[100];

    buf[0] = '\0';
    buf_con[0] = '\0';

    if (obj->pIndexData->vnum > 5)	/* money, gold, etc */
    	sprintf(buf_con," [%s%s%s]",
		IS_SET(ch->act,PLR_COLOR) ? CLR_GREEN : "",
		get_cond_alias(obj),
		IS_SET(ch->act,PLR_COLOR) ? CLR_WHITE_BOLD : "" ); 
    else buf_con[0] = '\0';

    if ((fShort && (obj->short_descr == NULL || obj->short_descr[0] == '\0'))
    ||  (obj->description == NULL || obj->description[0] == '\0'))
	return buf;
   if (IS_SET(ch->act,PLR_COLOR) )
   {
    if ( IS_OBJ_STAT(obj, ITEM_BURIED)     )   
		{
		 strcat( buf, CLR_WHITE );
		 strcat( buf, "(Buried) "     );
		}
    if ( IS_OBJ_STAT(obj, ITEM_INVIS)     )   
		{
		 strcat( buf, CLR_WHITE );
		 strcat( buf, "(Invis) "     );
		}
    if ( CAN_DETECT(ch, DETECT_EVIL)
         && IS_OBJ_STAT(obj, ITEM_EVIL)   )   
		{
		 strcat( buf, CLR_RED );
		 strcat( buf, "(Red Aura) "  );
		}
    if (CAN_DETECT(ch, DETECT_GOOD)
    &&  IS_OBJ_STAT(obj,ITEM_BLESS))	      
		{
		 strcat( buf, CLR_BLUE );
		 strcat(buf,"(Blue Aura) "	);
		}
    if ( CAN_DETECT(ch, DETECT_MAGIC)
         && IS_OBJ_STAT(obj, ITEM_MAGIC)  )   
		{
		 strcat( buf, CLR_YELLOW );
		 strcat( buf, "(Magical) "   );
		}
    if ( IS_OBJ_STAT(obj, ITEM_GLOW)      )   
		{
		 strcat( buf, CLR_CYAN );
		 strcat( buf, "(Glowing) "   );
		}
    if ( IS_OBJ_STAT(obj, ITEM_HUM)       )   
		{
		 strcat( buf, CLR_BROWN );
		 strcat( buf, "(Humming) "   );
		}
    strcat( buf, CLR_WHITE_BOLD );
   }
   else
   {
    if ( IS_OBJ_STAT(obj, ITEM_BURIED)     )  strcat( buf, "(Buried) "     );
    if ( IS_OBJ_STAT(obj, ITEM_INVIS)     )   strcat( buf, "(Invis) "     );
    if ( CAN_DETECT(ch, DETECT_EVIL)
         && IS_OBJ_STAT(obj, ITEM_EVIL)   )   strcat( buf, "(Red Aura) "  );
    if (CAN_DETECT(ch, DETECT_GOOD)
    &&  IS_OBJ_STAT(obj,ITEM_BLESS))	      strcat(buf,"(Blue Aura) "	);
    if ( CAN_DETECT(ch, DETECT_MAGIC)
         && IS_OBJ_STAT(obj, ITEM_MAGIC)  )   strcat( buf, "(Magical) "   );
    if ( IS_OBJ_STAT(obj, ITEM_GLOW)      )   strcat( buf, "(Glowing) "   );
    if ( IS_OBJ_STAT(obj, ITEM_HUM)       )   strcat( buf, "(Humming) "   );
   }
    if ( fShort )
    {
	if ( obj->short_descr != NULL )
	{
	    strcat( buf, obj->short_descr );
            strcat( buf, buf_con );
	}
    }
    else
    {
	if ( obj->description != NULL)  
	{
	  char tmp[70];
	  if ( obj->in_room != NULL )  
	  {
	    if ( IS_WATER( obj->in_room ) )  
	    {
	      strcpy( tmp, obj->short_descr );
              tmp[0] = UPPER(tmp[0]);
              strcat( buf, tmp );
	      switch(dice(1,3)) 
	      {
		case 1: strcat(buf, " is floating gently on the water.");break;
		case 2: strcat(buf, " is making it's way on the water.");break;
		case 3: strcat(buf, " is getting wet by the water.");break;
              }
            }
	    else
		{
		 strcat( buf, obj->description );
		}
          }
	  else
	  {
	    strcat( buf, obj->description );
	  }
        }
    }

    return buf;
}



/*
 * Show a list to a character.
 * Can coalesce duplicated items.
 */
void show_list_to_char( OBJ_DATA *list, CHAR_DATA *ch, bool fShort, bool fShowNothing )
{
    char buf[MAX_STRING_LENGTH];
    BUFFER *output;
    char **prgpstrShow;
    int *prgnShow;
    char *pstrShow;
    OBJ_DATA *obj;
    int nShow;
    int iShow;
    int count;
    bool fCombine;

    if ( ch->desc == NULL )
	return;

    /*
     * Alloc space for output lines.
     */
    output = new_buf();

    count = 0;
    for ( obj = list; obj != NULL; obj = obj->next_content )
	count++;
    prgpstrShow	= (char **)alloc_mem( count * sizeof(char *) );
    prgnShow    = (int *)alloc_mem( count * sizeof(int)    );
    nShow	= 0;

    /*
     * Format the list of objects.
     */
    for ( obj = list; obj != NULL; obj = obj->next_content )
    { 
	if ( obj->wear_loc == WEAR_NONE && can_see_obj( ch, obj )) 
	{
	    pstrShow = format_obj_to_char( obj, ch, fShort );

	    fCombine = FALSE;

	    if ( IS_NPC(ch) || IS_SET(ch->comm, COMM_COMBINE) )
	    {
		/*
		 * Look for duplicates, case sensitive.
		 * Matches tend to be near end so run loop backwords.
		 */
		for ( iShow = nShow - 1; iShow >= 0; iShow-- )
		{
		    if ( !strcmp( prgpstrShow[iShow], pstrShow ) )
		    {
			prgnShow[iShow]++;
			fCombine = TRUE;
			break;
		    }
		}
	    }

	    /*
	     * Couldn't combine, or didn't want to.
	     */
	    if ( !fCombine )
	    {
		prgpstrShow [nShow] = str_dup( pstrShow );
		prgnShow    [nShow] = 1;
		nShow++;
	    }
	}
    }

    /*
     * Output the formatted list.
     */
    for ( iShow = 0; iShow < nShow; iShow++ )
    {
	if (prgpstrShow[iShow][0] == '\0')
	{
	    free_string(prgpstrShow[iShow]);
	    continue;
	}

	if ( IS_NPC(ch) || IS_SET(ch->comm, COMM_COMBINE) )
	{
	    if ( prgnShow[iShow] != 1 )
	    {
		sprintf( buf, "(%2d) ", prgnShow[iShow] );
		add_buf(output,buf);
	    }
	    else
	    {
		add_buf(output,"     ");
	    }
	}
	add_buf(output,prgpstrShow[iShow]);
	add_buf(output,"\n\r");
	free_string( prgpstrShow[iShow] );
    }

    if ( fShowNothing && nShow == 0 )
    {
	if ( IS_NPC(ch) || IS_SET(ch->comm, COMM_COMBINE) )
	    send_to_char( "     ", ch );
	send_to_char( "Nothing.\n\r", ch );
    }
    page_to_char(buf_string(output),ch);

    /*
     * Clean up.
     */
    free_buf(output);
    free_mem( prgpstrShow, count * sizeof(char *) );
    free_mem( prgnShow,    count * sizeof(int)    );

    return;
}



void show_char_to_char_0( CHAR_DATA *victim, CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH],message[MAX_STRING_LENGTH];

    buf[0] = '\0';

   /*
    * Quest staff
    */
   if (!IS_NPC(ch) && IS_NPC(victim) 
	&& ch->pcdata->questmob > 0 
	&& victim->pIndexData->vnum == ch->pcdata->questmob)
        strcat( buf, "[TARGET] ");       
/*
    sprintf(message,"(%s) ",race_table[RACE(victim)].name);
    message[1] = UPPER( message[1]); 
    strcat(buf,message);
*/
    if ( RIDDEN(victim)  ) 			strcat( buf, "(Ridden) "     );
    if ( IS_AFFECTED(victim, AFF_INVISIBLE)   ) strcat( buf, "(Invis) "      );
    if ( IS_AFFECTED(victim,AFF_IMP_INVIS )   ) strcat( buf, "(Improved) "   );
    if ( victim->invis_level >= LEVEL_HERO    ) strcat( buf, "(Wizi) "	     );
    if ( IS_AFFECTED(victim, AFF_HIDE)        ) strcat( buf, "(Hide) "       );
    if ( IS_AFFECTED(victim, AFF_FADE)        ) strcat( buf, "(Fade) "       );
    if ( IS_AFFECTED(victim, AFF_CAMOUFLAGE)  ) strcat( buf, "(Camf) "       );
    if ( CAN_DETECT(victim, ADET_EARTHFADE)   ) strcat( buf, "(Earth) "      );
    if ( IS_AFFECTED(victim, AFF_CHARM)       
	&& victim->master == ch) 		strcat( buf, "(Charmed) "    );
    if ( IS_AFFECTED(victim, AFF_PASS_DOOR)   ) strcat( buf, "(Translucent) ");
    if ( IS_AFFECTED(victim, AFF_FAERIE_FIRE) ) strcat( buf, "(Pink Aura) "  );
    if ( IS_NPC(victim) && IS_SET(victim->act,ACT_UNDEAD) 
    &&   CAN_DETECT(ch, DETECT_UNDEAD)     ) strcat( buf, "(Undead) ");
    if ( IS_EVIL(victim)
    &&   CAN_DETECT(ch, DETECT_EVIL)     ) strcat( buf, "(Red Aura) "   );
    if ( IS_GOOD(victim)
    &&   CAN_DETECT(ch, DETECT_GOOD)     ) strcat( buf, "(Golden Aura) ");
    if ( IS_AFFECTED(victim, AFF_SANCTUARY)   ) strcat( buf, "(White Aura) " );
    if ( !IS_NPC(victim) && IS_SET(victim->act, PLR_WANTED ) )
						strcat( buf, "(CRIMINAL) ");

    if ( victim->position == victim->start_pos && victim->long_descr[0] != '\0' )
    {
	strcat( buf, victim->long_descr );
	send_to_char( buf, ch );
	return;
    }

    if ( IS_SET(ch->act,PLR_HOLYLIGHT) && is_affected(victim,gsn_doppelganger))
      {
        strcat( buf, "{"); strcat(buf, PERS(victim,ch)); strcat(buf, "} ");
      }

    if (is_affected(victim,gsn_doppelganger)  &&
        victim->doppel->long_descr[0] != '\0') {
      strcat( buf, victim->doppel->long_descr);
      send_to_char(buf, ch);
      return;
      }

    if (victim->long_descr[0] != '\0' &&
        !is_affected(victim,gsn_doppelganger)) {
      strcat( buf, victim->long_descr );
      send_to_char(buf, ch);
      return;
    }

    if (is_affected(victim, gsn_doppelganger))
      {
        strcat(buf, PERS(victim->doppel, ch ));
        if (!IS_NPC(victim->doppel) && !IS_SET(ch->comm, COMM_BRIEF))
          strcat(buf, victim->doppel->pcdata->title);
      }
   else  
   {
     strcat( buf, PERS( victim, ch ) );
     if ( !IS_NPC(victim) && !IS_SET(ch->comm, COMM_BRIEF) 
     &&   victim->position == POS_STANDING && ch->on == NULL )
 	 strcat( buf, victim->pcdata->title );
    }

    switch ( victim->position )
    {
    case POS_DEAD:     strcat( buf, " is DEAD!!" );              break;
    case POS_MORTAL:   strcat( buf, " is mortally wounded." );   break;
    case POS_INCAP:    strcat( buf, " is incapacitated." );      break;
    case POS_STUNNED:  strcat( buf, " is lying here stunned." ); break;
    case POS_SLEEPING: 
	if (victim->on != NULL)
	{
	    if (IS_SET(victim->on->value[2],SLEEP_AT))
  	    {
		sprintf(message," is sleeping at %s.",
		    victim->on->short_descr);
		strcat(buf,message);
	    }
	    else if (IS_SET(victim->on->value[2],SLEEP_ON))
	    {
		sprintf(message," is sleeping on %s.",
		    victim->on->short_descr); 
		strcat(buf,message);
	    }
	    else
	    {
		sprintf(message, " is sleeping in %s.",
		    victim->on->short_descr);
		strcat(buf,message);
	    }
	}
	else 
	    strcat(buf," is sleeping here.");
	break;
    case POS_RESTING:  
        if (victim->on != NULL)
	{
            if (IS_SET(victim->on->value[2],REST_AT))
            {
                sprintf(message," is resting at %s.",
                    victim->on->short_descr);
                strcat(buf,message);
            }
            else if (IS_SET(victim->on->value[2],REST_ON))
            {
                sprintf(message," is resting on %s.",
                    victim->on->short_descr);
                strcat(buf,message);
            }
            else 
            {
                sprintf(message, " is resting in %s.",
                    victim->on->short_descr);
                strcat(buf,message);
            }
	}
        else
	    strcat( buf, " is resting here." );       
	break;
    case POS_SITTING:  
        if (victim->on != NULL)
        {
            if (IS_SET(victim->on->value[2],SIT_AT))
            {
                sprintf(message," is sitting at %s.",
                    victim->on->short_descr);
                strcat(buf,message);
            }
            else if (IS_SET(victim->on->value[2],SIT_ON))
            {
                sprintf(message," is sitting on %s.",
                    victim->on->short_descr);
                strcat(buf,message);
            }
            else
            {
                sprintf(message, " is sitting in %s.",
                    victim->on->short_descr);
                strcat(buf,message);
            }
        }
        else
	    strcat(buf, " is sitting here.");
	break;
    case POS_STANDING: 
	if (victim->on != NULL)
	{
	    if (IS_SET(victim->on->value[2],STAND_AT))
	    {
		sprintf(message," is standing at %s.",
		    victim->on->short_descr);
		strcat(buf,message);
	    }
	    else if (IS_SET(victim->on->value[2],STAND_ON))
	    {
		sprintf(message," is standing on %s.",
		   victim->on->short_descr);
		strcat(buf,message);
	    }
	    else
	    {
		sprintf(message," is standing in %s.",
		    victim->on->short_descr);
		strcat(buf,message);
	    }
	}
	else if (MOUNTED(victim))
	{
	  sprintf(message," is here, riding %s.",PERS(MOUNTED(victim),ch));
	  strcat(buf, message);
	}
	else  strcat( buf, " is here." );               
	break;
    case POS_FIGHTING:
	strcat( buf, " is here, fighting " );
	if ( victim->fighting == NULL )
	    strcat( buf, "thin air??" );
	else if ( victim->fighting == ch )
	    strcat( buf, "YOU!" );
	else if ( victim->in_room == victim->fighting->in_room )
	{
	    strcat( buf, PERS( victim->fighting, ch ) );
	    strcat( buf, "." );
	}
	else
	    strcat( buf, "somone who left??" );
	break;
    }

    strcat( buf, "\n\r" );
    buf[0] = UPPER(buf[0]);
    send_to_char( buf, ch );
    return;
}



void show_char_to_char_1( CHAR_DATA *victim, CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    int iWear;
    int percent;
    bool found;
    CHAR_DATA *vict;

    vict = is_affected(victim,gsn_doppelganger) ? victim->doppel : victim;

    if ( can_see( victim, ch ) )
    {
	if (ch == victim)
	    act( "$n looks at $mself.",ch,NULL,NULL,TO_ROOM);
	else
	{
	    act( "$n looks at you.", ch, NULL, victim, TO_VICT    );
	    act( "$n looks at $N.",  ch, NULL, victim, TO_NOTVICT );
	}
    }

    if ( vict->description[0] != '\0' )
    {
	send_to_char( vict->description, ch );
    }
    else
    {
	act( "You see nothing special about $M.", ch, NULL, victim, TO_CHAR );
    }

    if ( MOUNTED(victim) ) 
    {
	sprintf(buf, "%s is riding %s.\n\r", 
            PERS(victim,ch), PERS( MOUNTED(victim),ch) );
        send_to_char( buf,ch);
    }
    if ( RIDDEN(victim) ) 
    {
	sprintf(buf,"%s is being ridden by %s.\n\r", 
            PERS(victim,ch), PERS( RIDDEN(victim),ch) );
        send_to_char( buf,ch);
    }

    if ( victim->max_hit > 0 )
	percent = ( 100 * victim->hit ) / victim->max_hit;
    else
	percent = -1;

    sprintf( buf,"(%s) %s", race_table[RACE(vict)].name, PERS(vict, ch));

    if (percent >= 100) 
	strcat( buf, " is in perfect health.\n\r");
    else if (percent >= 90) 
	strcat( buf, " has a few scratches.\n\r");
    else if (percent >= 75) 
	strcat( buf," has some small but disgusting cuts.\n\r");
    else if (percent >=  50) 
	strcat( buf, " is covered with bleeding wounds.\n\r");
    else if (percent >= 30)
	strcat( buf, " is gushing blood.\n\r");
    else if (percent >= 15)
	strcat ( buf, " is writhing in agony.\n\r");
    else if (percent >= 0 )
	strcat (buf, " is convulsing on the ground.\n\r");
    else
	strcat(buf, " is nearly dead.\n\r");

	/* vampire ... */
    if (percent < 90 && ch->iclass == CLASS_VAMPIRE && ch->level > 10)
	gain_condition(ch,COND_BLOODLUST,-1);

    buf[0] = UPPER(buf[0]);
    send_to_char( buf, ch );

    found = FALSE;
    for ( iWear = 0; iWear < MAX_WEAR; iWear++ )
    {
        if ( iWear==WEAR_FINGER || iWear==WEAR_NECK || iWear==WEAR_WRIST
		|| iWear==WEAR_TATTOO || iWear == WEAR_STUCK_IN)
        {
	  for ( obj=vict->carrying; obj != NULL; obj = obj->next_content )
	   {
	    if ( obj->wear_loc == iWear )
	     {
		if (!found) 
		 {
		  act( "$N is using:", ch, NULL, victim, TO_CHAR);
		  send_to_char( "\n\r", ch );
		  found = TRUE;
                 }
                show_vwear_to_char( ch, obj );
	     }
		  
	   }
	}
	else 
        {
	 if ( ( obj = get_eq_char(vict,iWear)) != NULL )
	  {
		if (!found) 
		 {
		  act( "$N is using:", ch, NULL, victim, TO_CHAR);
		  send_to_char( "\n\r", ch );
		  found = TRUE;
                 }
                show_vwear_to_char( ch, obj );
	  }
        }
    }

    if ( victim != ch
    &&   !IS_NPC(ch)
    &&   number_percent( ) < get_skill(ch,gsn_peek))
    {
	send_to_char( "\n\rYou peek at the inventory:\n\r", ch );
	check_improve(ch,gsn_peek,TRUE,4);
	show_list_to_char( vict->carrying, ch, TRUE, TRUE );
    }

    return;
}



void show_char_to_char( CHAR_DATA *list, CHAR_DATA *ch )
{
    CHAR_DATA *rch;
    int life_count=0;

    for ( rch = list; rch != NULL; rch = rch->next_in_room )
    {
	if ( rch == ch )
	    continue;

	if ( get_trust(ch) < rch->invis_level)
	    continue;

	if ( can_see( ch, rch ) )
	{
	    show_char_to_char_0( rch, ch );
	}
	else if ( room_is_dark( ch )
	&&        IS_AFFECTED(rch, AFF_INFRARED ) )
	{
	    send_to_char( "You see glowing red eyes watching YOU!\n\r", ch );
	    if (!IS_IMMORTAL(rch)) life_count++;
	}
	else if (!IS_IMMORTAL(rch)) life_count++;
    }

    if (life_count && CAN_DETECT(ch,DETECT_LIFE) )
	{
	 char buf[MAX_STRING_LENGTH];
	 sprintf(buf,"You feel %d more life %s in the room.\n\r",
			life_count,
			(life_count == 1) ? "form" : "forms");
	 send_to_char(buf,ch);
	}
    return;
} 



bool check_blind( CHAR_DATA *ch )
{

    if (!IS_NPC(ch) && IS_SET(ch->act,PLR_HOLYLIGHT))
	return TRUE;

    if ( IS_AFFECTED(ch, AFF_BLIND) )
    { 
	send_to_char( "You can't see a thing!\n\r", ch ); 
	return FALSE; 
    }

    return TRUE;
}

void do_clear( CHAR_DATA *ch, char *argument )
{
  if ( !IS_NPC(ch) )
    send_to_char("\033[0;0H\033[2J", ch );
  return;
}

/* changes your scroll */
void do_scroll(CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    char buf[100];
    int lines;

    one_argument(argument,arg);
    
    if (arg[0] == '\0')
    {
	if (ch->lines == 0)
	    send_to_char("You do not page long messages.\n\r",ch);
	else
	{
	    sprintf(buf,"You currently display %d lines per page.\n\r",
		    ch->lines + 2);
	    send_to_char(buf,ch);
	}
	return;
    }

    if (!is_number(arg))
    {
	send_to_char("You must provide a number.\n\r",ch);
	return;
    }

    lines = atoi(arg);

    if (lines == 0)
    {
        send_to_char("Paging disabled.\n\r",ch);
        ch->lines = 0;
        return;
    }

    if (lines < 10 || lines > 100)
    {
	send_to_char("You must provide a reasonable number.\n\r",ch);
	return;
    }

    sprintf(buf,"Scroll set to %d lines.\n\r",lines);
    send_to_char(buf,ch);
    ch->lines = lines - 2;
}

/* RT does socials */
void do_socials(CHAR_DATA *ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];
    int iSocial;
    int col;
     
    col = 0;
   
    for (iSocial = 0; social_table[iSocial].name[0] != '\0'; iSocial++)
    {
	sprintf(buf,"%-12s",social_table[iSocial].name);
	send_to_char(buf,ch);
	if (++col % 6 == 0)
	    send_to_char("\n\r",ch);
    }

    if ( col % 6 != 0)
	send_to_char("\n\r",ch);
    return;
}


 
/* RT Commands to replace news, motd, imotd, etc from ROM */

void do_motd(CHAR_DATA *ch, char *argument)
{
    do_help(ch,"motd");
}

void do_imotd(CHAR_DATA *ch, char *argument)
{  
    do_help(ch,"imotd");
}

void do_rules(CHAR_DATA *ch, char *argument)
{
    do_help(ch,"rules");
}

void do_story(CHAR_DATA *ch, char *argument)
{
    do_help(ch,"story");
}

void do_wizlist(CHAR_DATA *ch, char *argument)
{
    do_help(ch,"wizlist");
}

/* RT this following section holds all the auto commands from ROM, as well as
   replacements for config */

void do_autolist(CHAR_DATA *ch, char *argument)
{
    /* lists most player flags */
    if (IS_NPC(ch))
      return;

    if (IS_SET(ch->act,PLR_COLOR))
	{
	 do_autolist_col(ch,argument);
	 return;
	}

    send_to_char("   action     status\n\r",ch);
    send_to_char("---------------------\n\r",ch);
 
    send_to_char("color          ",ch);
    if (IS_SET(ch->act,PLR_COLOR))
	send_to_char("ON\n\r",ch);
    else
	send_to_char("OFF\n\r",ch);

    send_to_char("autoassist     ",ch);
    if (IS_SET(ch->act,PLR_AUTOASSIST))
        send_to_char("ON\n\r",ch);
    else
        send_to_char("OFF\n\r",ch); 

    send_to_char("autoexit       ",ch);
    if (IS_SET(ch->act,PLR_AUTOEXIT))
        send_to_char("ON\n\r",ch);
    else
        send_to_char("OFF\n\r",ch);

    send_to_char("autogold       ",ch);
    if (IS_SET(ch->act,PLR_AUTOGOLD))
        send_to_char("ON\n\r",ch);
    else
        send_to_char("OFF\n\r",ch);

    send_to_char("autoloot       ",ch);
    if (IS_SET(ch->act,PLR_AUTOLOOT))
        send_to_char("ON\n\r",ch);
    else
        send_to_char("OFF\n\r",ch);

    send_to_char("autosac        ",ch);
    if (IS_SET(ch->act,PLR_AUTOSAC))
        send_to_char("ON\n\r",ch);
    else
        send_to_char("OFF\n\r",ch);

    send_to_char("autosplit      ",ch);
    if (IS_SET(ch->act,PLR_AUTOSPLIT))
        send_to_char("ON\n\r",ch);
    else
        send_to_char("OFF\n\r",ch);

    send_to_char("compact mode   ",ch);
    if (IS_SET(ch->comm,COMM_COMPACT))
        send_to_char("ON\n\r",ch);
    else
        send_to_char("OFF\n\r",ch);

    send_to_char("prompt         ",ch);
    if (IS_SET(ch->comm,COMM_PROMPT))
	send_to_char("ON\n\r",ch);
    else
	send_to_char("OFF\n\r",ch);

    send_to_char("combine items  ",ch);
    if (IS_SET(ch->comm,COMM_COMBINE))
	send_to_char("ON\n\r",ch);
    else
	send_to_char("OFF\n\r",ch);


    if (IS_SET(ch->act,PLR_NOSUMMON))
	send_to_char("You can only be summoned players within your PK range.\n\r",ch);
    else
	send_to_char("You can be summoned by anyone.\n\r",ch);
   
    if (IS_SET(ch->act,PLR_NOFOLLOW))
	send_to_char("You do not welcome followers.\n\r",ch);
    else
	send_to_char("You accept followers.\n\r",ch);

    if (IS_SET(ch->act,PLR_NOCANCEL))
	send_to_char("You do not welcome others' cancellation spells.\n\r",ch);
    else
	send_to_char("You accept others' cancellation spells.\n\r",ch);
}

void do_autolist_col(CHAR_DATA *ch, char *argument)
{
    /* lists most player flags */
    if (IS_NPC(ch))
      return;

    send_to_char("  [1;33maction           status\n\r[0;37m",ch);
    send_to_char("[1;36m-------------------------\n\r[0;37m",ch);
 
    send_to_char("[1;34m|[0;37m [0;36mcolor          [0;37m",ch);
    if (IS_SET(ch->act,PLR_COLOR))
	send_to_char("[1;34m|  [1;32mON  [1;34m|\n\r[0;37m",ch);
    else
	send_to_char("[1;34m|  [1;31mOFF [1;34m|\n\r[0;37m",ch);

    send_to_char("[1;34m|[0;37m [0;36mautoassist     ",ch);
    if (IS_SET(ch->act,PLR_AUTOASSIST))
        send_to_char("[1;34m|  [1;32mON  [1;34m|\n\r[0;37m",ch);
    else
        send_to_char("[1;34m|  [1;31mOFF [1;34m|\n\r[0;37m",ch); 

    send_to_char("[1;34m|[0;37m [0;36mautoexit       ",ch);
    if (IS_SET(ch->act,PLR_AUTOEXIT))
        send_to_char("[1;34m|  [1;32mON  [1;34m|\n\r[0;37m",ch);
    else
        send_to_char("[1;34m|  [1;31mOFF [1;34m|\n\r[0;37m",ch);

    send_to_char("[1;34m|[0;37m [0;36mautogold       ",ch);
    if (IS_SET(ch->act,PLR_AUTOGOLD))
        send_to_char("[1;34m|  [1;32mON  [1;34m|\n\r[0;37m",ch);
    else
        send_to_char("[1;34m|  [1;31mOFF [1;34m|\n\r[0;37m",ch);

    send_to_char("[1;34m|[0;37m [0;36mautoloot       ",ch);
    if (IS_SET(ch->act,PLR_AUTOLOOT))
        send_to_char("[1;34m|  [1;32mON  [1;34m|\n\r[0;37m",ch);
    else
        send_to_char("[1;34m|  [1;31mOFF [1;34m|\n\r[0;37m",ch);

    send_to_char("[1;34m|[0;37m [0;36mautosac        ",ch);
    if (IS_SET(ch->act,PLR_AUTOSAC))
        send_to_char("[1;34m|  [1;32mON  [1;34m|\n\r[0;37m",ch);
    else
        send_to_char("[1;34m|  [1;31mOFF [1;34m|\n\r[0;37m",ch);

    send_to_char("[1;34m|[0;37m [0;36mautosplit      ",ch);
    if (IS_SET(ch->act,PLR_AUTOSPLIT))
        send_to_char("[1;34m|  [1;32mON  [1;34m|\n\r[0;37m",ch);
    else
        send_to_char("[1;34m|  [1;31mOFF [1;34m|\n\r[0;37m",ch);

    send_to_char("[1;34m|[0;37m [0;36mcompact mode   ",ch);
    if (IS_SET(ch->comm,COMM_COMPACT))
        send_to_char("[1;34m|  [1;32mON  [1;34m|\n\r[0;37m",ch);
    else
        send_to_char("[1;34m|  [1;31mOFF [1;34m|\n\r[0;37m",ch);

    send_to_char("[1;34m|[0;37m [0;36mprompt         ",ch);
    if (IS_SET(ch->comm,COMM_PROMPT))
	send_to_char("[1;34m|  [1;32mON  [1;34m|\n\r[0;37m",ch);
    else
	send_to_char("[1;34m|  [1;31mOFF [1;34m|\n\r[0;37m",ch);

    send_to_char("[1;34m|[0;37m [0;36mcombine items  ",ch);
    if (IS_SET(ch->comm,COMM_COMBINE))
	send_to_char("[1;34m|  [1;32mON  [1;34m|\n\r[0;37m",ch);
    else
	send_to_char("[1;34m|  [1;31mOFF [1;34m|\n\r[0;37m",ch);
        send_to_char("[1;36m-------------------------\n\r[0;37m",ch);
 

    if (IS_SET(ch->act,PLR_NOSUMMON))
	send_to_char("You can only be summoned players within your PK range.\n\r",ch);
    else
	send_to_char("You can be summoned by anyone.\n\r",ch);
   
    if (IS_SET(ch->act,PLR_NOFOLLOW))
	send_to_char("You do not welcome followers.\n\r",ch);
    else
	send_to_char("You accept followers.\n\r",ch);

    if (IS_SET(ch->act,PLR_NOCANCEL))
	send_to_char("You do not welcome others' cancellation spells.\n\r",ch);
    else
	send_to_char("You accept others' cancellation spells.\n\r",ch);
}

void do_autoassist(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;
    
    if (IS_SET(ch->act,PLR_AUTOASSIST))
    {
      send_to_char("Autoassist removed.\n\r",ch);
      REMOVE_BIT(ch->act,PLR_AUTOASSIST);
    }
    else
    {
      send_to_char("You will now assist when needed.\n\r",ch);
      SET_BIT(ch->act,PLR_AUTOASSIST);
    }
}

void do_autoexit(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;
 
    if (IS_SET(ch->act,PLR_AUTOEXIT))
    {
      send_to_char("Exits will no longer be displayed.\n\r",ch);
      REMOVE_BIT(ch->act,PLR_AUTOEXIT);
    }
    else
    {
      send_to_char("Exits will now be displayed.\n\r",ch);
      SET_BIT(ch->act,PLR_AUTOEXIT);
    }
}

void do_autogold(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;
 
    if (IS_SET(ch->act,PLR_AUTOGOLD))
    {
      send_to_char("Autogold removed.\n\r",ch);
      REMOVE_BIT(ch->act,PLR_AUTOGOLD);
    }
    else
    {
      send_to_char("Automatic gold looting set.\n\r",ch);
      SET_BIT(ch->act,PLR_AUTOGOLD);
    }
}

void do_autoloot(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;
 
    if (IS_SET(ch->act,PLR_AUTOLOOT))
    {
      send_to_char("Autolooting removed.\n\r",ch);
      REMOVE_BIT(ch->act,PLR_AUTOLOOT);
    }
    else
    {
      send_to_char("Automatic corpse looting set.\n\r",ch);
      SET_BIT(ch->act,PLR_AUTOLOOT);
    }
}

void do_autosac(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;
 
    if (IS_SET(ch->act,PLR_AUTOSAC))
    {
      send_to_char("Autosacrificing removed.\n\r",ch);
      REMOVE_BIT(ch->act,PLR_AUTOSAC);
    }
    else
    {
      send_to_char("Automatic corpse sacrificing set.\n\r",ch);
      SET_BIT(ch->act,PLR_AUTOSAC);
    }
}

void do_autosplit(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;
 
    if (IS_SET(ch->act,PLR_AUTOSPLIT))
    {
      send_to_char("Autosplitting removed.\n\r",ch);
      REMOVE_BIT(ch->act,PLR_AUTOSPLIT);
    }
    else
    {
      send_to_char("Automatic gold splitting set.\n\r",ch);
      SET_BIT(ch->act,PLR_AUTOSPLIT);
    }
}

void do_color(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;
 
    if (IS_SET(ch->act,PLR_COLOR))
    {
      send_to_char("Your color is now OFF.\n\r",ch);
      REMOVE_BIT(ch->act,PLR_COLOR);
    }
    else
    {
      send_to_char("Your color is now ON.\n\r",ch);
      SET_BIT(ch->act,PLR_COLOR);
    }
}

void do_brief(CHAR_DATA *ch, char *argument)
{
    if (IS_SET(ch->comm,COMM_BRIEF))
    {
      send_to_char("Full descriptions activated.\n\r",ch);
      REMOVE_BIT(ch->comm,COMM_BRIEF);
    }
    else
    {
      send_to_char("Short descriptions activated.\n\r",ch);
      SET_BIT(ch->comm,COMM_BRIEF);
    }
}

void do_compact(CHAR_DATA *ch, char *argument)
{
    if (IS_SET(ch->comm,COMM_COMPACT))
    {
      send_to_char("Compact mode removed.\n\r",ch);
      REMOVE_BIT(ch->comm,COMM_COMPACT);
    }
    else
    {
      send_to_char("Compact mode set.\n\r",ch);
      SET_BIT(ch->comm,COMM_COMPACT);
    }
}

void do_show(CHAR_DATA *ch, char *argument)
{
    if (IS_SET(ch->comm,COMM_SHOW_AFFECTS))
    {
      send_to_char("Affects will no longer be shown in score.\n\r",ch);
      REMOVE_BIT(ch->comm,COMM_SHOW_AFFECTS);
    }
    else
    {
      send_to_char("Affects will now be shown in score.\n\r",ch);
      SET_BIT(ch->comm,COMM_SHOW_AFFECTS);
    }
}

void do_prompt(CHAR_DATA *ch, char *argument)
{
   char buf[MAX_STRING_LENGTH];
 
   if ( argument[0] == '\0' )
   {
	if (IS_SET(ch->comm,COMM_PROMPT))
   	{
      	    send_to_char("You will no longer see prompts.\n\r",ch);
      	    REMOVE_BIT(ch->comm,COMM_PROMPT);
    	}
    	else
    	{
      	    send_to_char("You will now see prompts.\n\r",ch);
      	    SET_BIT(ch->comm,COMM_PROMPT);
    	}
       return;
   }

   if( !strcmp( argument, "all" ) )
      strcpy( buf, "<%n: %hhp %mm %vmv Opp:<%o>> ");
   else
   {
      if ( strlen(argument) > 50 )
         argument[50] = '\0';
      strcpy( buf, argument );
      smash_tilde( buf );
      if (str_suffix("%c",buf))
	strcat(buf," ");
	
   }
 
   free_string( ch->prompt );
   ch->prompt = str_dup( buf );
   sprintf(buf,"Prompt set to %s\n\r",ch->prompt );
   send_to_char(buf,ch);
   return;
}

void do_combine(CHAR_DATA *ch, char *argument)
{
    if (IS_SET(ch->comm,COMM_COMBINE))
    {
      send_to_char("Long inventory selected.\n\r",ch);
      REMOVE_BIT(ch->comm,COMM_COMBINE);
    }
    else
    {
      send_to_char("Combined inventory selected.\n\r",ch);
      SET_BIT(ch->comm,COMM_COMBINE);
    }
}

void do_noloot(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;
 
    if (IS_SET(ch->act,PLR_CANLOOT))
    {
      send_to_char("Your corpse is now safe from thieves.\n\r",ch);
      REMOVE_BIT(ch->act,PLR_CANLOOT);
    }
    else
    {
      send_to_char("Your corpse may now be looted.\n\r",ch);
      SET_BIT(ch->act,PLR_CANLOOT);
    }
}

void do_nofollow(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;
    if ( IS_AFFECTED( ch, AFF_CHARM ) )  {
	send_to_char( "You don't want to leave your beloved master.\n\r",ch);
	return;
    } 

    if (IS_SET(ch->act,PLR_NOFOLLOW))
    {
      send_to_char("You now accept followers.\n\r",ch);
      REMOVE_BIT(ch->act,PLR_NOFOLLOW);
    }
    else
    {
      send_to_char("You no longer accept followers.\n\r",ch);
      SET_BIT(ch->act,PLR_NOFOLLOW);
      die_follower( ch );
    }
}

void do_nosummon(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
    {
      if (IS_SET(ch->imm_flags,IMM_SUMMON))
      {
	send_to_char("You are no longer immune to summoning.\n\r",ch);
	REMOVE_BIT(ch->imm_flags,IMM_SUMMON);
      }
      else
      {
	send_to_char("You are now immune to summoning.\n\r",ch);
	SET_BIT(ch->imm_flags,IMM_SUMMON);
      }
    }
    else
    {
      if (IS_SET(ch->act,PLR_NOSUMMON))
      {
        send_to_char("You may now be summoned by anyone.\n\r",ch);
        REMOVE_BIT(ch->act,PLR_NOSUMMON);
      }
      else
      {
        send_to_char("You may only be summoned by players within your PK range.\n\r",ch);
        SET_BIT(ch->act,PLR_NOSUMMON);
      }
    }
}

void do_look( CHAR_DATA *ch, char *argument )
{
    char buf  [MAX_STRING_LENGTH];
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    EXIT_DATA *pexit;
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    char *pdesc;
    int door;
    int number,count;

    if ( ch->desc == NULL )
	return;

    if ( ch->position < POS_SLEEPING )
    {
	send_to_char( "You can't see anything but stars!\n\r", ch );
	return;
    }

    if ( ch->position == POS_SLEEPING )
    {
	send_to_char( "You can't see anything, you're sleeping!\n\r", ch );
	return;
    }

    if ( !check_blind( ch ) )
	return;

    if ( !IS_NPC(ch)
    &&   !IS_SET(ch->act, PLR_HOLYLIGHT)
    &&   room_is_dark( ch ) )
    {
	send_to_char( "It is pitch black ... \n\r", ch );
	show_char_to_char( ch->in_room->people, ch );
	return;
    }

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    number = number_argument(arg1,arg3);
    count = 0;

    if ( arg1[0] == '\0' || !str_cmp( arg1, "auto" ) )
    {
	/* 'look' or 'look auto' */
	send_to_char( ch->in_room->name, ch );

	if (IS_IMMORTAL(ch) && (IS_NPC(ch) || IS_SET(ch->act,PLR_HOLYLIGHT)))
	{
	    sprintf(buf," [Room %d]",ch->in_room->vnum);
	    send_to_char(buf,ch);
	}

	send_to_char( "\n\r", ch );

	if ( arg1[0] == '\0'
	|| ( !IS_NPC(ch) && !IS_SET(ch->comm, COMM_BRIEF) ) )
	{
	    send_to_char( "  ",ch);
	    send_to_char( ch->in_room->description, ch );
	}

        if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_AUTOEXIT) )
	{
	    send_to_char("\n\r",ch);
            do_exits( ch, "auto" );
	}

	show_list_to_char( ch->in_room->contents, ch, FALSE, FALSE );
	show_char_to_char( ch->in_room->people,   ch );
	return;
    }

    if ( !str_cmp( arg1, "i" ) || !str_cmp(arg1, "in")  || !str_cmp(arg1,"on"))
    {
	/* 'look in' */
	if ( arg2[0] == '\0' )
	{
	    send_to_char( "Look in what?\n\r", ch );
	    return;
	}

	if ( ( obj = get_obj_here( ch, arg2 ) ) == NULL )
	{
	    send_to_char( "You do not see that here.\n\r", ch );
	    return;
	}

	switch ( obj->item_type )
	{
	default:
	    send_to_char( "That is not a container.\n\r", ch );
	    break;

	case ITEM_DRINK_CON:
	    if ( obj->value[1] <= 0 )
	    {
		send_to_char( "It is empty.\n\r", ch );
		break;
	    }

	    sprintf( buf, "It's %sfilled with  a %s liquid.\n\r",
		obj->value[1] <     obj->value[0] / 4
		    ? "less than half-" :
		obj->value[1] < 3 * obj->value[0] / 4
		    ? "about half-"     : "more than half-",
		liq_table[obj->value[2]].liq_color
		);

	    send_to_char( buf, ch );
	    break;

	case ITEM_CONTAINER:
	case ITEM_CORPSE_NPC:
	case ITEM_CORPSE_PC:
	    if ( IS_SET(obj->value[1], CONT_CLOSED) )
	    {
		send_to_char( "It is closed.\n\r", ch );
		break;
	    }

	    act( "$p holds:", ch, obj, NULL, TO_CHAR );
	    show_list_to_char( obj->contains, ch, TRUE, TRUE );
	    break;
	}
	return;
    }

    if ( ( victim = get_char_room( ch, arg1 ) ) != NULL )
    {
	show_char_to_char_1( victim, ch );

        /* Love potion */

        if (is_affected(ch, gsn_love_potion) && (victim != ch))
          {
            AFFECT_DATA af;

            affect_strip(ch, gsn_love_potion);

            if (ch->master)
              stop_follower(ch);
            add_follower(ch, victim);
            ch->leader = victim;

	    af.where = TO_AFFECTS;
            af.type = gsn_charm_person;
            af.level = ch->level;
            af.duration =  number_fuzzy(victim->level / 4);
            af.bitvector = AFF_CHARM;
            af.modifier = 0;
            af.location = 0;
            affect_to_char(ch, &af);

            act("Isn't $n just so nice?", victim, NULL, ch, TO_VICT);
            act("$N looks at you with adoring eyes.",victim,NULL,ch,TO_CHAR);
            act("$N looks at $n with adoring eyes.",victim,NULL,ch,TO_NOTVICT);
          }

	return;
    }

    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
	if ( can_see_obj( ch, obj ) )
	{  /* player can see object */
	    pdesc = get_extra_descr( arg3, obj->extra_descr );
	    if ( pdesc != NULL )
	    	if (++count == number)
	    	{
		    send_to_char( pdesc, ch );
		    return;
	    	}
	    	else continue;

 	    pdesc = get_extra_descr( arg3, obj->pIndexData->extra_descr );
 	    if ( pdesc != NULL )
 	    	if (++count == number)
 	    	{	
		    send_to_char( pdesc, ch );
		    return;
	     	}
		else continue;

	    if ( is_name( arg3, obj->name ) )
	    	if (++count == number)
	    	{
		    send_to_char( "You see nothing special about it.\n\r", ch);
		    return;
		}
	  }
    }

    for ( obj = ch->in_room->contents; obj != NULL; obj = obj->next_content )
    {
	if ( can_see_obj( ch, obj ) )
	{
	    pdesc = get_extra_descr( arg3, obj->extra_descr );
	    if ( pdesc != NULL )
	    	if (++count == number)
	    	{
		    send_to_char( pdesc, ch );
		    return;
	    	}

	    pdesc = get_extra_descr( arg3, obj->pIndexData->extra_descr );
	    if ( pdesc != NULL )
	    	if (++count == number)
	    	{
		    send_to_char( pdesc, ch );
		    return;
	    	}
	}

	if ( is_name( arg3, obj->name ) )
	    if (++count == number)
	    {
	    	send_to_char( obj->description, ch );
	    	send_to_char("\n\r",ch);
	    	return;
	    }
    }

    pdesc = get_extra_descr(arg3,ch->in_room->extra_descr);
    if (pdesc != NULL)
    {
	if (++count == number)
	{
	    send_to_char(pdesc,ch);
	    return;
	}
    }
    
    if (count > 0 && count != number)
    {
    	if (count == 1)
    	    sprintf(buf,"You only see one %s here.\n\r",arg3);
    	else
    	    sprintf(buf,"You only see %d of those here.\n\r",count);
    	
    	send_to_char(buf,ch);
    	return;
    }

         if ( !str_cmp( arg1, "n" ) || !str_cmp( arg1, "north" ) ) door = 0;
    else if ( !str_cmp( arg1, "e" ) || !str_cmp( arg1, "east"  ) ) door = 1;
    else if ( !str_cmp( arg1, "s" ) || !str_cmp( arg1, "south" ) ) door = 2;
    else if ( !str_cmp( arg1, "w" ) || !str_cmp( arg1, "west"  ) ) door = 3;
    else if ( !str_cmp( arg1, "u" ) || !str_cmp( arg1, "up"    ) ) door = 4;
    else if ( !str_cmp( arg1, "d" ) || !str_cmp( arg1, "down"  ) ) door = 5;
    else
    {
	send_to_char( "You do not see that here.\n\r", ch );
	return;
    }

    /* 'look direction' */
    if ( ( pexit = ch->in_room->exit[door] ) == NULL )
    {
	send_to_char( "Nothing special there.\n\r", ch );
	return;
    }

    if ( pexit->description != NULL && pexit->description[0] != '\0' )
	send_to_char( pexit->description, ch );
    else
	send_to_char( "Nothing special there.\n\r", ch );

    if ( pexit->keyword    != NULL
    &&   pexit->keyword[0] != '\0'
    &&   pexit->keyword[0] != ' ' )
    {
	if ( IS_SET(pexit->exit_info, EX_CLOSED) )
	{
	    act( "The $d is closed.", ch, NULL, pexit->keyword, TO_CHAR );
	}
	else if ( IS_SET(pexit->exit_info, EX_ISDOOR) )
	{
	    act( "The $d is open.",   ch, NULL, pexit->keyword, TO_CHAR );
	}
    }

    return;
}

/* RT added back for the hell of it */
void do_read (CHAR_DATA *ch, char *argument )
{
    do_look(ch,argument);
}

void do_examine( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Examine what?\n\r", ch );
	return;
    }

    do_look( ch, arg );

    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    {
	switch ( obj->item_type )
	{
	default:
	    break;

	case ITEM_MONEY:
	    if (obj->value[0] == 0)
	    {
	        if (obj->value[1] == 0)
		    sprintf(buf,"Odd...there's no coins in the pile.\n\r");
		else if (obj->value[1] == 1)
		    sprintf(buf,"Wow. One gold coin.\n\r");
		else
		    sprintf(buf,"There are %d gold coins in the pile.\n\r",
			obj->value[1]);
	    }
	    else if (obj->value[1] == 0)
	    {
		if (obj->value[0] == 1)
		    sprintf(buf,"Wow. One silver coin.\n\r");
		else
		    sprintf(buf,"There are %d silver coins in the pile.\n\r",
			obj->value[0]);
	    }
	    else
		sprintf(buf,
		    "There are %d gold and %d silver coins in the pile.\n\r",
		    obj->value[1],obj->value[0]);
	    send_to_char(buf,ch);
	    break;

	case ITEM_DRINK_CON:
	case ITEM_CONTAINER:
	case ITEM_CORPSE_NPC:
	case ITEM_CORPSE_PC:
	    sprintf(buf,"in %s",argument);
	    do_look( ch, buf );
	}
    }

    return;
}



/*
 * Thanks to Zrin for auto-exit part.
 */
void do_exits( CHAR_DATA *ch, char *argument )
{
    extern char * const dir_name[];
    char buf[MAX_STRING_LENGTH];
    EXIT_DATA *pexit;
    bool found;
    bool fAuto;
    int door;

    fAuto  = !str_cmp( argument, "auto" );

    if ( !check_blind( ch ) )
	return;

    if (fAuto)
	{
	 if (IS_SET(ch->act,PLR_COLOR) )
	 {
	  sprintf(buf,CLR_BLUE_BOLD);
	  strcat(buf,"[Exits:");
	 }
	 else sprintf(buf,"[Exits:");
	}
    else if (IS_IMMORTAL(ch))
	sprintf(buf,"Obvious exits from room %d:\n\r",ch->in_room->vnum);
    else
	sprintf(buf,"Obvious exits:\n\r");

    found = FALSE;
    for ( door = 0; door <= 5; door++ )
    {
	if ( ( pexit = ch->in_room->exit[door] ) != NULL
	&&   pexit->u1.to_room != NULL
	&&   can_see_room(ch,pexit->u1.to_room) 
	&&   !IS_SET(pexit->exit_info, EX_CLOSED) )
	{
	    found = TRUE;
	    if ( fAuto )
	    {
		strcat( buf, " " );
		strcat( buf, dir_name[door] );
	    }
	    else
	    {
		sprintf( buf + strlen(buf), "%-5s - %s",
		    capitalize( dir_name[door] ),
		    room_dark( pexit->u1.to_room )
			?  "Too dark to tell"
			: pexit->u1.to_room->name
		    );
		if (IS_IMMORTAL(ch))
		    sprintf(buf + strlen(buf), 
			" (room %d)\n\r",pexit->u1.to_room->vnum);
		else
		    sprintf(buf + strlen(buf), "\n\r");
	    }
	}

	if ( number_percent() < get_skill(ch,gsn_perception)  
	&&   ( pexit = ch->in_room->exit[door] ) != NULL
	&&   pexit->u1.to_room != NULL
	&&   can_see_room(ch,pexit->u1.to_room) 
	&&   IS_SET(pexit->exit_info, EX_CLOSED) )
	{
	    check_improve(ch,gsn_perception,TRUE,5);
	    found = TRUE;
	    if ( fAuto )
	    {
		strcat( buf, " " );
		strcat( buf, dir_name[door] );
		strcat( buf, "*" );
	    }
	    else
	    {
		sprintf( buf + strlen(buf), "%-5s * (%s)",
		    capitalize( dir_name[door] ),
		    pexit->keyword   );
		if (IS_IMMORTAL(ch))
		    sprintf(buf + strlen(buf), 
			" (room %d)\n\r",pexit->u1.to_room->vnum);
		else
		    sprintf(buf + strlen(buf), "\n\r");
	    }
	}

    }

    if ( !found )
	strcat( buf, fAuto ? " none" : "None.\n\r" );

    if ( fAuto )
	{
	 if ( IS_SET(ch->act,PLR_COLOR) )
	 {
	  strcat( buf, "]\n\r" );	  
	  strcat( buf, CLR_WHITE_BOLD);
	 }
	 else strcat( buf, "]\n\r" );
	}

    send_to_char( buf, ch );
    return;
}

void do_worth( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int total_played;

    if (IS_NPC(ch))
    {
	sprintf(buf,"You have %ld gold and %ld silver.\n\r",
	    ch->gold,ch->silver);
	send_to_char(buf,ch);
	return;
    }

    sprintf(buf, 
    "You have %ld gold, %ld silver, and %d experience (%d exp to level).\n\r",
	ch->gold, ch->silver,ch->exp, exp_to_level( ch, ch->pcdata->points ) );
 
    send_to_char(buf,ch);
    sprintf(buf,"You have killed %3d %s and %3d %s.\n\r",
		ch->pcdata->has_killed,
		IS_GOOD(ch) ? "non-goods" :
				IS_EVIL(ch) ? "non-evils" : "non-neutrals",
		ch->pcdata->anti_killed,
		IS_GOOD(ch) ? "goods" :	IS_EVIL(ch) ? "evils" : "neutrals");
    send_to_char(buf, ch);

    total_played = get_total_played(ch);
    sprintf(buf, 
	"Within last 14 days, you have played %d hours and %d minutes.\n\r",
	(int) (total_played / 60), (total_played % 60) );
    send_to_char(buf, ch);
    if (IS_IMMORTAL(ch))
    {
	int l;

	for( l=0; l<MAX_TIME_LOG; l++)
	{
	   sprintf(buf, "Day: %3d Playing Time: %3d\n", 
		ch->pcdata->log_date[l], ch->pcdata->log_time[l]);
	   send_to_char(buf, ch);
	}
    }

    return;
}

void do_score( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char buf1[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    AFFECT_DATA *paf;
    int i;

    if (IS_SET(ch->act,PLR_COLOR) )
	{
	 do_score_col(ch,argument);
	 return;
	}

    sprintf( buf,
	"You are %s%s, level %d, %d years old (%d hours).\n\r",
	ch->name,
	IS_NPC(ch) ? "" : ch->pcdata->title,
	ch->level, get_age(ch),
        ( ch->played + (int) (current_time - ch->logon) ) / 3600);
    send_to_char( buf, ch );

    if ( get_trust( ch ) != ch->level )
    {
	sprintf( buf, "You are trusted at level %d.\n\r",
	    get_trust( ch ) );
	send_to_char( buf, ch );
    }

    sprintf(buf, "Race: %s  Sex: %s  Class: %s  Hometown: %s\n\r",
	race_table[ORG_RACE(ch)].name,
	ch->sex == 0 ? "sexless" : ch->sex == 1 ? "male" : "female",
 	IS_NPC(ch) ? "mobile" : class_table[ch->iclass].name,
	IS_NPC(ch) ? "Midgaard" : hometown_table[ch->hometown].name);
    send_to_char(buf,ch);
	
      sprintf( buf,
	"You have %d/%d hit, %d/%d mana, %d/%d movement.\n\r",
	ch->hit,  ch->max_hit,
	ch->mana, ch->max_mana,
	ch->move, ch->max_move);
  
    send_to_char( buf, ch );

    sprintf( buf,
	"You have %d practices and %d training sessions.\n\r",
	ch->practice, ch->train);
    send_to_char( buf, ch );

    sprintf( buf,
	"You are carrying %d/%d items with weight %ld/%d pounds.\n\r",
	ch->carry_number, can_carry_n(ch),
	get_carry_weight(ch), can_carry_w(ch) );
    send_to_char( buf, ch );

    if ( ch->level > 20 || IS_NPC(ch) )  {
      sprintf( buf,
"Str: %d(%d)  Int: %d(%d)  Wis: %d(%d)  Dex: %d(%d)  Con: %d(%d) Cha: %d(%d)\n\r",
	ch->perm_stat[STAT_STR],
	get_curr_stat(ch,STAT_STR),
	ch->perm_stat[STAT_INT],
	get_curr_stat(ch,STAT_INT),
	ch->perm_stat[STAT_WIS],
	get_curr_stat(ch,STAT_WIS),
	ch->perm_stat[STAT_DEX],
	get_curr_stat(ch,STAT_DEX),
	ch->perm_stat[STAT_CON],
	get_curr_stat(ch,STAT_CON),
	ch->perm_stat[STAT_CHA],
	get_curr_stat(ch,STAT_CHA) );
      send_to_char( buf, ch );
    }
    else  {
	sprintf( buf,
 "Str: %-9s Wis: %-9s Con: %-9s\n\rInt: %-9s Dex: %-9s Cha: %-11s\n\r",
        get_stat_alias( ch, STAT_STR ),
        get_stat_alias( ch, STAT_WIS ),
        get_stat_alias( ch, STAT_CON ),
        get_stat_alias( ch, STAT_INT ),
        get_stat_alias( ch, STAT_DEX ),
        get_stat_alias( ch, STAT_CHA ) );

      send_to_char( buf, ch );
    }

    sprintf( buf, "You have scored %d exp, and have %s%s%s.\n\r",
       ch->exp, ch->gold+ch->silver==0?"no money":ch->gold!=0?"%ld gold ":"",
                ch->silver!=0?"%ld silver ":"", 
       ch->gold+ch->silver!=0?ch->gold+ch->silver==1?"coin":"coins":"" );
    if ( ch->gold != 0 )
      sprintf( buf2, buf, ch->gold, ch->silver );
    else 
      sprintf( buf2, buf, ch->silver );

    send_to_char( buf2, ch );

    /* KIO shows exp to level */
    if (!IS_NPC(ch) && ch->level < LEVEL_HERO)
    {
      sprintf (buf, 
	"You need %d exp to level.\n\r",
        exp_to_level(ch,ch->pcdata->points));
      send_to_char( buf, ch );
     }

/*** Added By KIO  ****/    
    if (!IS_NPC(ch))
    {
      sprintf (buf, 
	"Quest Points: %d.	Next Quest Time: %d.\n\r",
         ch->pcdata->questpoints,ch->pcdata->nextquest);
      send_to_char( buf, ch );
    }
/*** Added By KIO  ****/    
            
	if (ch->iclass != 9)
	{
         sprintf( buf, "Wimpy set to %d hit points.  ", ch->wimpy );
	 send_to_char( buf, ch );
	}
	else 
	{
	 sprintf(buf,"Total %d deaths up to now .",ch->pcdata->death);
	 send_to_char(buf, ch);
	}
    if (ch->guarding != NULL)
      {
	sprintf(buf1, "You are guarding: %s  ", ch->guarding->name);
	send_to_char( buf1, ch);
      }

    if (ch->guarded_by != NULL)
      {
	sprintf(buf2, "You are guarded by: %s", ch->guarded_by->name);
	send_to_char(buf2, ch);
      }

    send_to_char("\n\r", ch);

    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK]   > 10 )
	send_to_char( "You are drunk.\n\r",   ch );
    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_THIRST] <=  0 )
	send_to_char( "You are thirsty.\n\r", ch );
/*    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_FULL]   ==  0 ) */
    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_HUNGER]   <=  0 )
	send_to_char( "You are hungry.\n\r",  ch );
    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_BLOODLUST]   <=  0 )
	send_to_char( "You are hungry for blood.\n\r",  ch );
    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_DESIRE]   <=  0 )
	send_to_char( "You are desiring your home.\n\r",  ch );

    switch ( ch->position )
    {
    case POS_DEAD:     
	send_to_char( "You are DEAD!!\n\r",		ch );
	break;
    case POS_MORTAL:
	send_to_char( "You are mortally wounded.\n\r",	ch );
	break;
    case POS_INCAP:
	send_to_char( "You are incapacitated.\n\r",	ch );
	break;
    case POS_STUNNED:
	send_to_char( "You are stunned.\n\r",		ch );
	break;
    case POS_SLEEPING:
	send_to_char( "You are sleeping.",		ch );
    if ( ch->last_fight_time != -1 && !IS_IMMORTAL(ch) &&
        (current_time - ch->last_fight_time)<FIGHT_DELAY_TIME) 
        send_to_char("Your adrenalin is gushing!\n\r",ch);
	else send_to_char("\n\r",ch);
	break;
    case POS_RESTING:
	send_to_char( "You are resting.",		ch );
    if ( ch->last_fight_time != -1 && !IS_IMMORTAL(ch) &&
        (current_time - ch->last_fight_time)<FIGHT_DELAY_TIME) 
        send_to_char("Your adrenalin is gushing!\n\r",ch);
	break;
    case POS_STANDING:
	send_to_char( "You are standing.",		ch );
    if ( ch->last_fight_time != -1 && !IS_IMMORTAL(ch) &&
        (current_time - ch->last_fight_time)<FIGHT_DELAY_TIME) 
        send_to_char("Your adrenalin is gushing!\n\r",ch);
	else send_to_char("\n\r",ch);
	break;
    case POS_FIGHTING:
	send_to_char( "You are fighting.",		ch );
    if ( ch->last_fight_time != -1 && !IS_IMMORTAL(ch) &&
        (current_time - ch->last_fight_time)<FIGHT_DELAY_TIME) 
        send_to_char("Your adrenalin is gushing!\n\r",ch);
	else send_to_char("\n\r",ch);
	break;
    }


    /* print AC values */
    if (ch->level >= 25)
    {	
	sprintf( buf,"Armor: pierce: %d  bash: %d  slash: %d  magic: %d\n\r",
		 GET_AC(ch,AC_PIERCE),
		 GET_AC(ch,AC_BASH),
		 GET_AC(ch,AC_SLASH),
		 GET_AC(ch,AC_EXOTIC));
    	send_to_char(buf,ch);
    }

    for (i = 0; i < 4; i++)
    {
	char * temp;

	switch(i)
	{
	    case(AC_PIERCE):	temp = "piercing";	break;
	    case(AC_BASH):	temp = "bashing";	break;
	    case(AC_SLASH):	temp = "slashing";	break;
	    case(AC_EXOTIC):	temp = "magic";		break;
	    default:		temp = "error";		break;
	}
	
	send_to_char("You are ", ch);

	if      (GET_AC(ch,i) >=  101 ) 
	    sprintf(buf,"hopelessly vulnerable to %s.\n\r",temp);
	else if (GET_AC(ch,i) >= 80) 
	    sprintf(buf,"defenseless against %s.\n\r",temp);
	else if (GET_AC(ch,i) >= 60)
	    sprintf(buf,"barely protected from %s.\n\r",temp);
	else if (GET_AC(ch,i) >= 40)
	    sprintf(buf,"slightly armored against %s.\n\r",temp);
	else if (GET_AC(ch,i) >= 20)
	    sprintf(buf,"somewhat armored against %s.\n\r",temp);
	else if (GET_AC(ch,i) >= 0)
	    sprintf(buf,"armored against %s.\n\r",temp);
	else if (GET_AC(ch,i) >= -20)
	    sprintf(buf,"well-armored against %s.\n\r",temp);
	else if (GET_AC(ch,i) >= -40)
	    sprintf(buf,"very well-armored against %s.\n\r",temp);
	else if (GET_AC(ch,i) >= -60)
	    sprintf(buf,"heavily armored against %s.\n\r",temp);
	else if (GET_AC(ch,i) >= -80)
	    sprintf(buf,"superbly armored against %s.\n\r",temp);
	else if (GET_AC(ch,i) >= -100)
	    sprintf(buf,"almost invulnerable to %s.\n\r",temp);
	else
	    sprintf(buf,"divinely armored against %s.\n\r",temp);

	send_to_char(buf,ch);
    }


    /* RT wizinvis and holy light */
    if ( IS_IMMORTAL(ch))
    {
      send_to_char("Holy Light: ",ch);
      if (IS_SET(ch->act,PLR_HOLYLIGHT))
        send_to_char("on",ch);
      else
        send_to_char("off",ch);

      if (ch->invis_level)
      {
        sprintf( buf, "  Invisible: level %d",ch->invis_level);
        send_to_char(buf,ch);
      }

      if (ch->incog_level)
      {
        sprintf( buf, "  Incognito: level %d",ch->invis_level);
        send_to_char(buf,ch);
      }
      send_to_char("\n\r",ch);

    }
    if ( ch->level >= 20 )  
    {
	sprintf( buf, "Hitroll: %d  Damroll: %d.\n\r",
	    GET_HITROLL(ch), GET_DAMROLL(ch) );
	send_to_char( buf, ch );
    }
    
    send_to_char( "You are ", ch );
    if (IS_GOOD(ch)) send_to_char("good.  ", ch);
    else if (IS_EVIL(ch)) send_to_char ("evil.  ", ch);
    else send_to_char ("neutral.  ", ch);

    if (ch->ethos == 1) send_to_char("You have a lawful ethos.\n\r", ch);
    else if (ch->ethos == 2) send_to_char("You have a neutral ethos.\n\r", ch);
    else if (ch->ethos == 3) send_to_char("You have a chaotic ethos.\n\r", ch);
    else if (!IS_NPC(ch))
    send_to_char("You have no ethos, report it to the gods!\n\r", ch);
    if (IS_NPC(ch)) ch->religion = 0;
    if ((ch->religion <= RELIGION_NONE) || (ch->religion > MAX_RELIGION) )
	send_to_char("You don't believe any religion.\n\r",ch);
    else 
	{
	 sprintf(buf,"Your religion is the way of %s.\n\r",
			religion_table[ch->religion].leader);
	 send_to_char(buf,ch);
	}	
    if ( ch->affected != NULL && IS_SET(ch->comm, COMM_SHOW_AFFECTS) )
    {
	send_to_char( "You are affected by:\n\r", ch );
	for ( paf = ch->affected; paf != NULL; paf = paf->next )
	{
	    sprintf( buf, "Spell: '%s'", skill_table[paf->type].name );
	    send_to_char( buf, ch );

	    if ( ch->level >= 20 )
	    {
		if ( paf->duration != -1 && paf->duration != -2)
		  sprintf( buf,
		      " modifies %s by %d for %d hours",
		      affect_loc_name( paf->location ),
		      paf->modifier,
		      paf->duration );
		else 
		  sprintf( buf,
		      " modifies %s by %d permenantly",
		      affect_loc_name( paf->location ),
		      paf->modifier );
		send_to_char( buf, ch );
	    }

	    send_to_char( ".\n\r", ch );
	}
    }

    return;
}


void do_affects(CHAR_DATA *ch, char *argument )
{
    AFFECT_DATA *paf, *paf_last = NULL;
    char buf[MAX_STRING_LENGTH];

    if (IS_SET(ch->act,PLR_COLOR) )
	{
	 do_affects_col(ch,argument);
	 return;
	}
    
    if ( ch->affected != NULL )
    {
	send_to_char( "You are affected by the following spells:\n\r", ch );
	for ( paf = ch->affected; paf != NULL; paf = paf->next )
	{
	    if (paf_last != NULL && paf->type == paf_last->type)
		if (ch->level >= 20 )
		    sprintf( buf, "                      ");
		else
		    continue;
	    else
	    	sprintf( buf, "Spell: %-15s", skill_table[paf->type].name );

	    send_to_char( buf, ch );

	    if ( ch->level >= 20 )
	    {
		sprintf( buf,
		    ": modifies %s by %d ",
		    affect_loc_name( paf->location ),
		    paf->modifier);
		send_to_char( buf, ch );
		if ( paf->duration == -1 || paf->duration == -2 )
		    sprintf( buf, "permanently" );
		else
		    sprintf( buf, "for %d hours", paf->duration );
		send_to_char( buf, ch );
	    }
	    send_to_char( "\n\r", ch );
	    paf_last = paf;
	}
    }
    else 
	send_to_char("You are not affected by any spells.\n\r",ch);

    return;
}




char *	const	day_name	[] =
{
    "the Moon", "the Bull", "Deception", "Thunder", "Freedom",
    "the Great Gods", "the Sun"
};

char *	const	month_name	[] =
{
    "Winter", "the Winter Wolf", "the Frost Giant", "the Old Forces",
    "the Grand Struggle", "the Spring", "Nature", "Futility", "the Dragon",
    "the Sun", "the Heat", "the Battle", "the Dark Shades", "the Shadows",
    "the Long Shadows", "the Ancient Darkness", "the Great Evil"
};


#define COLOR_DAWN "\033[1;34;44m"
#define COLOR_MORNING "\033[1;37;44m"
#define COLOR_DAY "\033[1;33;44m"
#define COLOR_EVENING "\033[1;31;44m"
#define COLOR_NIGHT "\033[0;30;44m"

void do_time( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char *suf;
    int day;

    day     = time_info.day + 1;

         if ( day > 4 && day <  20 ) suf = "th";
    else if ( day % 10 ==  1       ) suf = "st";
    else if ( day % 10 ==  2       ) suf = "nd";
    else if ( day % 10 ==  3       ) suf = "rd";
    else                             suf = "th";

    sprintf( buf,
	"It is %d o'clock %s, Day of %s, %d%s the Month of %s.\n\r",
	(time_info.hour % 12 == 0) ? 12 : time_info.hour %12,
	time_info.hour >= 12 ? "pm" : "am",
	day_name[day % 7],
	day, suf,
	month_name[time_info.month]);

    send_to_char(buf,ch);

    if ( !IS_SET(ch->in_room->room_flags,ROOM_INDOORS) ||
         IS_IMMORTAL(ch) )  
    {
      sprintf( buf, "It's$C %s. $c",
         (time_info.hour>=5 && time_info.hour<9)? "dawn":
         (time_info.hour>=9 && time_info.hour<12)? "morning":
         (time_info.hour>=12 && time_info.hour<18)? "mid-day":
         (time_info.hour>=18 && time_info.hour<21)? "evening":
         "night" );

      act_color( buf, ch, NULL, NULL, TO_CHAR, POS_RESTING, 
         (time_info.hour>=5 && time_info.hour<9)? COLOR_DAWN:
         (time_info.hour>=9 && time_info.hour<12)? COLOR_MORNING:
         (time_info.hour>=12 && time_info.hour<18)? COLOR_DAY:
         (time_info.hour>=18 && time_info.hour<21)? COLOR_EVENING:
         COLOR_NIGHT, CLR_NORMAL );
    }

    if ( !IS_IMMORTAL( ch ) ) 
      return;

    sprintf(buf2, "%s", (char *) ctime( &boot_time ));
    sprintf(buf,"ANATOLIA started up at %s\n\rThe system time is %s.\n\r",
	buf2, (char *) ctime( &current_time ) );

    send_to_char( buf, ch );
    return;
}



void do_weather( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    static char * const sky_look[4] =
    {
	"cloudless",
	"cloudy",
	"rainy",
	"lit by flashes of lightning"
    };

    if ( !IS_OUTSIDE(ch) )
    {
	send_to_char( "You can't see the weather indoors.\n\r", ch );
	return;
    }

    sprintf( buf, "The sky is %s and %s.\n\r",
	sky_look[weather_info.sky],
	weather_info.change >= 0
	? "a warm southerly breeze blows"
	: "a cold northern gust blows"
	);
    send_to_char( buf, ch );
    return;
}



void do_help( CHAR_DATA *ch, char *argument )
{
    HELP_DATA *pHelp;
    char argall[MAX_INPUT_LENGTH],argone[MAX_INPUT_LENGTH];

    if ( argument[0] == '\0' )
	argument = "summary";

    /* this parts handles help a b so that it returns help 'a b' */
    argall[0] = '\0';
    while (argument[0] != '\0' )
    {
	argument = one_argument(argument,argone);
	if (argall[0] != '\0')
	    strcat(argall," ");
	strcat(argall,argone);
    }

    for ( pHelp = help_first; pHelp != NULL; pHelp = pHelp->next )
    {
	if ( pHelp->level > get_trust( ch ) )
	    continue;

	if ( is_name( argall, pHelp->keyword ) )
	{
	    if ( pHelp->level >= 0 && str_cmp( argall, "imotd" ) )
	    {
		send_to_char( pHelp->keyword, ch );
		send_to_char( "\n\r", ch );
	    }

	    /*
	     * Strip leading '.' to allow initial blanks.
	     */
	    if ( pHelp->text[0] == '.' )
		page_to_char( pHelp->text+1, ch );
	    else
		page_to_char( pHelp->text  , ch );
	    return;
	}
    }

    send_to_char( "No help on that word.\n\r", ch );
    return;
}


/* whois command */
void do_whois (CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    char output[MAX_STRING_LENGTH];
    char buf[MAX_STRING_LENGTH];
    char titlebuf[MAX_STRING_LENGTH];
    char level_buf[MAX_STRING_LENGTH];
    char classbuf[MAX_STRING_LENGTH];
    char pk_buf[100];
    char act_buf[100];
    DESCRIPTOR_DATA *d;
    bool found = FALSE;
    char cabalbuf[MAX_STRING_LENGTH];

    one_argument(argument,arg);
  
    if (arg[0] == '\0')
    {
	send_to_char("You must provide a name.\n\r",ch);
	return;
    }

    output[0] = '\0';

    for (d = descriptor_list; d != NULL; d = d->next)
    {
	CHAR_DATA *wch;
	char const *iclass;

 	if (d->connected != CON_PLAYING || !can_see(ch,d->character))
	    continue;
	
	if ( d->connected != CON_PLAYING || 
(IS_VAMPIRE( d->character ) && !IS_IMMORTAL(ch) && (ch != d->character) ) )
	    continue;

	wch = ( d->original != NULL ) ? d->original : d->character;

 	if (!can_see(ch,wch))
	    continue;

	if (!str_prefix(arg,wch->name))
	{
	    found = TRUE;
	    
	    /* work out the printing */
		
	    iclass = class_table[wch->iclass].who_name;
	    switch(wch->level)
	    {
		case MAX_LEVEL - 0 : iclass = "IMP"; 	break;
		case MAX_LEVEL - 1 : iclass = "CRE";	break;
		case MAX_LEVEL - 2 : iclass = "SUP";	break;
		case MAX_LEVEL - 3 : iclass = "DEI";	break;
		case MAX_LEVEL - 4 : iclass = "GOD";	break;
		case MAX_LEVEL - 5 : iclass = "IMM";	break;
		case MAX_LEVEL - 6 : iclass = "DEM";	break;
		case MAX_LEVEL - 7 : iclass = "ANG";	break;
		case MAX_LEVEL - 8 : iclass = "AVA";	break;
	    }

	    /* for cabals 
	    if ((wch->cabal && (ch->cabal == wch->cabal || 
			       IS_TRUSTED(ch,LEVEL_IMMORTAL))) ||
                               wch->level >= LEVEL_HERO)
	      sprintf(cabalbuf, "[%s] ",cabal_table[wch->cabal].short_name);
	    else cabalbuf[0] = '\0';
		*/
	if (( wch->cabal && ch->cabal == wch->cabal) || IS_IMMORTAL(ch) 
		|| ( IS_SET(wch->act , PLR_CANINDUCT) && wch->cabal == 1) 
		|| wch->cabal == CABAL_HUNTER 
		|| (wch->cabal == CABAL_RULER
		   && is_equiped_n_char(wch, OBJ_VNUM_RULER_BADGE, WEAR_NECK)) )
	{
	  if (IS_SET(ch->act,PLR_COLOR) )
	  sprintf(cabalbuf, "[%s%s%s] ",
		CLR_CYAN,
		cabal_table[wch->cabal].short_name,
		CLR_WHITE_BOLD
		);
	  else 
	  sprintf(cabalbuf, "[%s] ",cabal_table[wch->cabal].short_name);
	}
	else cabalbuf[0] = '\0';
	if (wch->cabal == 0) cabalbuf[0] = '\0';

	pk_buf[0] = '\0';
	if (!((ch==wch && ch->level<PK_MIN_LEVEL) || is_safe_nomessage(ch,wch)))
	 {
	  sprintf(pk_buf,"%s(PK) %s",
		IS_SET(ch->act,PLR_COLOR) ? CLR_RED : "",
	        IS_SET(ch->act,PLR_COLOR) ? CLR_WHITE_BOLD : "");
	 }
	act_buf[0] = '\0';
	sprintf(act_buf,"%s%s%s",
		IS_SET(ch->act,PLR_COLOR) ? CLR_WHITE : "",
		    IS_SET(wch->act, PLR_WANTED) ? "(WANTED) " : "",
	        IS_SET(ch->act,PLR_COLOR) ? CLR_WHITE_BOLD : "");

	if (IS_NPC(wch))
		sprintf(titlebuf,"Believer of Chronos.");
	else {
		char tempbuf[MAX_INPUT_LENGTH];
		sprintf(tempbuf,"%s", wch->pcdata->title );
		if (strlen(tempbuf) > 45 )
		 {
			tempbuf[44] = '\0';
			free_string(wch->pcdata->title);
			wch->pcdata->title = str_dup( tempbuf );
			bug("Title length more than 45",0);
		 }
		sprintf(titlebuf,"%s%s%s",
		IS_SET(ch->act,PLR_COLOR) ? CLR_WHITE : "",
		tempbuf,
	        IS_SET(ch->act,PLR_COLOR) ? CLR_WHITE_BOLD : "");
	     }
	/*
	 * Format it up.
	 */

	sprintf( level_buf, "%s%2d%s",
		IS_SET(ch->act,PLR_COLOR) ? CLR_CYAN : "",
		wch->level,
	        IS_SET(ch->act,PLR_COLOR) ? CLR_WHITE_BOLD : "");
	sprintf(classbuf,"%s%s%s",
		IS_SET(ch->act,PLR_COLOR) ? CLR_YELLOW : "",
		iclass,
	        IS_SET(ch->act,PLR_COLOR) ? CLR_WHITE_BOLD : "");
	    /* a little formatting */

	    if (IS_TRUSTED(ch,LEVEL_IMMORTAL) || ch==wch || 
                      wch->level >= LEVEL_HERO)

	      sprintf(buf, "[%2d %s %s] %s%s%s%s%s\n\r",
		      wch->level,
		      RACE(wch) < MAX_PC_RACE ? 
		        pc_race_table[RACE(wch)].who_name: "     ",
		      classbuf,
		      pk_buf,
		      cabalbuf,    
		      act_buf,
		      wch->name, 
		      titlebuf);
	    

		else
	  sprintf( buf, "[%s %s    ] %s%s%s%s%s\n\r",
		(get_curr_stat(wch, STAT_CHA) < 18 ) ? level_buf : "  ",
 		      RACE(wch) < MAX_PC_RACE ? 
 		        pc_race_table[RACE(wch)].who_name: "     ",
 		      ( (ch==wch && ch->level < PK_MIN_LEVEL) || 
 		        is_safe_nomessage(ch,wch) ) ? 
 		       "" : "(PK) ",
 		      cabalbuf,    
 		      IS_SET(wch->act,PLR_WANTED) ? "(WANTED) " : "",
 		      wch->name, 
		      titlebuf);

	    strcat(output,buf);
	}
    }

    if (!found)
    {
	send_to_char("No one of that name is playing.\n\r",ch);
	return;
    }

    page_to_char(output,ch);
}


/*
 * New 'who' command originally by Alander of Rivers of Mud.
 */
void do_who( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char cabalbuf[MAX_STRING_LENGTH];
    char titlebuf[MAX_STRING_LENGTH];
    char output[4 * MAX_STRING_LENGTH];
    char act_buf[100];
    char level_buf[100];
    DESCRIPTOR_DATA *d;
    int iClass;
    int iRace;
    int iLevelLower;
    int iLevelUpper;
    int nNumber;
    int nMatch;
    int vnum;
    int count;
    bool rgfClass[MAX_CLASS];
    bool rgfRace[MAX_PC_RACE];
    bool fClassRestrict;
    bool fRaceRestrict;
    bool fImmortalOnly;
    bool fPKRestrict;
    bool fRulerRestrict;
    bool fChaosRestrict;
    bool fShalafiRestrict;
    bool fInvaderRestrict;
    bool fBattleRestrict;
    bool fKnightRestrict;
    bool fLionsRestrict;
    bool fTattoo;

    /*
     * Set default arguments.
     */
    iLevelLower    = 0;
    iLevelUpper    = MAX_LEVEL;
    fClassRestrict = FALSE;
    fRaceRestrict = FALSE;
    fPKRestrict = FALSE;
    fImmortalOnly  = FALSE;
    fBattleRestrict = FALSE;
    fChaosRestrict = FALSE;
    fRulerRestrict = FALSE;
    fInvaderRestrict = FALSE;
    fShalafiRestrict = FALSE;
    fKnightRestrict = FALSE;
    fLionsRestrict = FALSE;
    vnum = 0;
    fTattoo = FALSE;
    for ( iClass = 0; iClass < MAX_CLASS; iClass++ )
	rgfClass[iClass] = FALSE;
    for ( iRace = 0; iRace < MAX_PC_RACE; iRace++ )
	rgfRace[iRace] = FALSE;


    if (IS_SET(ch->act,PLR_COLOR)) 
	{
	 do_who_col(ch,argument);
	 return;
	}
    /*
     * Parse arguments.
     */
    nNumber = 0;
    for ( ;; )
    {
	char arg[MAX_STRING_LENGTH];

	argument = one_argument( argument, arg );
	if ( arg[0] == '\0' )
	    break;

	if (!str_cmp(arg,"pk"))
	  {
	    fPKRestrict = TRUE;
	    break;
	  }

	if (!str_cmp(arg,"ruler"))
	  {
	    if (ch->cabal != CABAL_RULER && !IS_IMMORTAL(ch))
	      {
		send_to_char("You are not in that cabal!\n\r",ch);
		return;
	      }
	    else
	      {
		fRulerRestrict = TRUE;
		break;
	      }
	  }
	if (!str_cmp(arg,"shalafi"))
	  {
	    if (ch->cabal != CABAL_SHALAFI && !IS_IMMORTAL(ch))
	      {
		send_to_char("You are not in that cabal!\n\r",ch);
		return;
	      }
	    else
	      {
		fShalafiRestrict = TRUE;
		break;
	      }
	  }
	if (!str_cmp(arg,"battle"))
	  {
	    if (ch->cabal != CABAL_BATTLE && !IS_IMMORTAL(ch))
	      {
		send_to_char("You are not in that cabal!\n\r",ch);
		return;
	      }
	    else
	      {
		fBattleRestrict = TRUE;
		break;
	      }
	  }
	if (!str_cmp(arg,"invader"))
	  {
	    if (ch->cabal != CABAL_INVADER && !IS_IMMORTAL(ch))
	      {
		send_to_char("You are not in that cabal!\n\r",ch);
		return;
	      }
	    else
	      {
		fInvaderRestrict = TRUE;
		break;
	      }
	  }
	if (!str_cmp(arg,"chaos"))
	  {
	    if (ch->cabal != CABAL_CHAOS && !IS_IMMORTAL(ch))
	      {
		send_to_char("You are not in that cabal!\n\r",ch);
		return;
	      }
	    else
	      {
		fChaosRestrict = TRUE;
		break;
	      }
	  }
	if (!str_cmp(arg,"knight"))
	  {
	    if (ch->cabal != CABAL_KNIGHT && !IS_IMMORTAL(ch))
	      {
		send_to_char("You are not in that cabal!\n\r",ch);
		return;
	      }
	    else
	      {
		fKnightRestrict = TRUE;
		break;
	      }
	  }
	if (!str_cmp(arg,"lions"))
	  {
	    if (ch->cabal != CABAL_LIONS && !IS_IMMORTAL(ch))
	      {
		send_to_char("You are not in that cabal!\n\r",ch);
		return;
	      }
	    else
	      {
		fLionsRestrict = TRUE;
		break;
	      }
	  }
	if (!str_cmp(arg,"tattoo"))
	  {
	    if (get_eq_char(ch,WEAR_TATTOO) == NULL)
	      {
		send_to_char("You haven't got a tattoo yetl!\n\r",ch);
		return;
	      }
	    else
	      {
		fTattoo = TRUE;
		vnum = get_eq_char(ch,WEAR_TATTOO)->pIndexData->vnum;
		break;
	      }
	  }
	if (!str_cmp(arg, "tattoo"))
	  {
	    send_to_char("This function of who is disabled for now.\n\r", ch);
	    return; 
	  }


	if ( is_number( arg ) && IS_IMMORTAL(ch))
	{
	    switch ( ++nNumber )
	    {
	    case 1: iLevelLower = atoi( arg ); break;
	    case 2: iLevelUpper = atoi( arg ); break; 
	    default:
		send_to_char( "This function of who is for immortals.\n\r",ch);
		return;
	    }
	}
	else
	{

	    /*
	     * Look for classes to turn on.
	     */
	    if ( arg[0] == 'i' )
	    {
		fImmortalOnly = TRUE;
	    }
	    else
	    {
	      iClass = class_lookup(arg);
	      if (iClass == -1 || !IS_IMMORTAL(ch))
	      	{
	          iRace = race_lookup(arg);

	          if (iRace == 0 || iRace >= MAX_PC_RACE)
		    {
		      send_to_char("That's not a valid race.\n\r",ch);
		      return;
		    }
	          else
		    {
			fRaceRestrict = TRUE;
			rgfRace[iRace] = TRUE;
		    }
		}
	      else
		{
		  fClassRestrict = TRUE;
		  rgfClass[iClass] = TRUE;
		}
	    }
	  }
      }

    /*
     * Now show matching chars.
     */
    nMatch = 0;
    buf[0] = '\0';
    output[0] = '\0';
    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	CHAR_DATA *wch;
	char const *iclass;

	/*
	 * Check for match against restrictions.
	 * Don't use trust as that exposes trusted mortals.
	 */
	if ( d->connected != CON_PLAYING || !can_see( ch, d->character ) )
	    continue;

	if ( d->connected != CON_PLAYING || 
(IS_VAMPIRE( d->character ) && !IS_IMMORTAL(ch) && (ch != d->character) ) )
	    continue;

	wch   = ( d->original != NULL ) ? d->original : d->character;
	if (!can_see(ch, wch)) /* can't see switched wizi imms */
	  continue;

	if ( wch->level < iLevelLower
	||   wch->level > iLevelUpper
	|| ( fImmortalOnly  && wch->level < LEVEL_HERO )
	|| ( fClassRestrict && !rgfClass[wch->iclass])
	|| ( fRaceRestrict && !rgfRace[RACE(wch)])
        || ( fPKRestrict && is_safe_nomessage(ch,wch) )
	|| ( fTattoo &&(vnum == get_eq_char(wch,WEAR_TATTOO)->pIndexData->vnum))
	    || (fRulerRestrict && wch->cabal != CABAL_RULER )
	    || (fChaosRestrict && wch->cabal != CABAL_CHAOS)
	    || (fBattleRestrict && wch->cabal != CABAL_BATTLE)
	    || (fInvaderRestrict && wch->cabal != CABAL_INVADER)
	    || (fShalafiRestrict && wch->cabal != CABAL_SHALAFI)
	    || (fKnightRestrict && wch->cabal != CABAL_KNIGHT)
	    || (fLionsRestrict && wch->cabal != CABAL_LIONS))
	    continue;

	nMatch++;

	/*
	 * Figure out what to print for class.
	 */
	iclass = class_table[wch->iclass].who_name;
	switch ( wch->level )
	{
	default: break;
            {
                case MAX_LEVEL - 0 : iclass = "IMP";     break;
                case MAX_LEVEL - 1 : iclass = "CRE";     break;
                case MAX_LEVEL - 2 : iclass = "SUP";     break;
                case MAX_LEVEL - 3 : iclass = "DEI";     break;
                case MAX_LEVEL - 4 : iclass = "GOD";     break;
                case MAX_LEVEL - 5 : iclass = "IMM";     break;
                case MAX_LEVEL - 6 : iclass = "DEM";     break;
                case MAX_LEVEL - 7 : iclass = "ANG";     break;
                case MAX_LEVEL - 8 : iclass = "AVA";     break;
            }
	}

	/* for cabals 
	if ((wch->cabal && (wch->cabal == ch->cabal || 
			   IS_TRUSTED(ch,LEVEL_IMMORTAL))) ||
                           wch->level >= LEVEL_HERO)
	*/
	if ( (wch->cabal && ch->cabal ==  wch->cabal) || IS_IMMORTAL(ch) 
		|| ( IS_SET(wch->act , PLR_CANINDUCT) && wch->cabal == 1) 
		|| wch->cabal == CABAL_HUNTER 
		|| (wch->cabal == CABAL_RULER
		   && is_equiped_n_char(wch, OBJ_VNUM_RULER_BADGE, WEAR_NECK)) )
	  sprintf(cabalbuf, "[%s] ",cabal_table[wch->cabal].short_name);
	else cabalbuf[0] = '\0';
	if (wch->cabal == 0) cabalbuf[0] = '\0';

	act_buf[0] = '\0';
	sprintf(act_buf,"%s",
		    IS_SET(wch->act, PLR_WANTED) ? "(WANTED) " : "");

	sprintf( level_buf, "%2d",wch->level);
	if (IS_NPC(wch))
		sprintf(titlebuf,"Believer of Chronos.");
	else {
		sprintf(titlebuf,"%s", wch->pcdata->title );
		if (strlen(titlebuf) > 45 )
		 {
			free_string(wch->pcdata->title);
			titlebuf[44] = '\0';
			wch->pcdata->title = str_dup( titlebuf );
			bug("Title length more than 45",0);
		 }
	     }
	/*
	 * Format it up.
	 */

	if (IS_TRUSTED(ch,LEVEL_IMMORTAL) || ch==wch ||
                   wch->level >= LEVEL_HERO || (get_curr_stat(wch, STAT_CHA) < 18 ) )

	  sprintf( buf, "[%2d %s %s] %s%s%s%s%s\n\r",
	    wch->level,
	    RACE(wch) < MAX_PC_RACE ? pc_race_table[RACE(wch)].who_name 
				    : "     ",
	    iclass,
	    (!is_safe_nomessage(ch,wch) && ch->level > PK_MIN_LEVEL) ?
		"(PK) " : "",
	    cabalbuf,
	    act_buf,
	    wch->name,
	    titlebuf);

	else
	  sprintf( buf, "[%s %s    ] %s%s%s%s%s\n\r",
		(get_curr_stat(wch, STAT_CHA) < 18 ) ? level_buf : "  ",
	    RACE(wch) < MAX_PC_RACE ? pc_race_table[RACE(wch)].who_name 
				    : "     ",
	    (!is_safe_nomessage(ch,wch) && ch->level > PK_MIN_LEVEL) ?
		"(PK) " : "",
	    cabalbuf,
	    act_buf,
	    wch->name,
	    titlebuf); 

	strcat(output,buf);
    }

    count = 0;
    for ( d = descriptor_list; d != NULL; d = d->next )
        if ( d->connected == CON_PLAYING )    count++;

    max_on = UMAX(count,max_on);
    sprintf( buf2, "\n\rPlayers found: %d. Most so far today: %d.\n\r",
		nMatch,max_on );
    strcat(output,buf2);
    page_to_char( output, ch );
    return;
}


void do_count ( CHAR_DATA *ch, char *argument )
{
    int count;
    DESCRIPTOR_DATA *d;
    char buf[MAX_STRING_LENGTH];

    count = 0;

    for ( d = descriptor_list; d != NULL; d = d->next )
        if ( d->connected == CON_PLAYING && can_see( ch, d->character ) )
	    count++;

    max_on = UMAX(count,max_on);

    if (max_on == count)
        sprintf(buf,"There are %d characters on, the most so far today.\n\r",
	    count);
    else
	sprintf(buf,"There are %d characters on, the most on today was %d.\n\r",
	    count,max_on);

    send_to_char(buf,ch);
}

void do_inventory( CHAR_DATA *ch, char *argument )
{
    send_to_char( "You are carrying:\n\r", ch );
    show_list_to_char( ch->carrying, ch, TRUE, TRUE );
    return;
}



void do_equipment( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    int iWear;
    bool found;

    send_to_char( "You are using:\n\r", ch );
    found = FALSE;
    for ( iWear = 0; iWear < MAX_WEAR; iWear++ )
    {
        if ( iWear==WEAR_FINGER || iWear==WEAR_NECK || iWear==WEAR_WRIST
		|| iWear==WEAR_TATTOO || iWear == WEAR_STUCK_IN)
        {
	  for ( obj=ch->carrying; obj != NULL; obj = obj->next_content )
	   {
	    if ( obj->wear_loc == iWear
                 && show_cwear_to_char( ch, obj ) )
	      found = TRUE;
	   }
	}
	else 
        {
	 if ( ( obj = get_eq_char(ch,iWear)) != NULL
              && show_cwear_to_char( ch, obj ) )
	  found = TRUE;
        }
    }

    if ( !found )
	send_to_char( "Nothing.\n\r", ch );

    return;
}



void do_compare( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *obj1;
    OBJ_DATA *obj2;
    int value1;
    int value2;
    char *msg;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    if ( arg1[0] == '\0' )
    {
	send_to_char( "Compare what to what?\n\r", ch );
	return;
    }

    if ( ( obj1 = get_obj_carry( ch, arg1 ) ) == NULL )
    {
	send_to_char( "You do not have that item.\n\r", ch );
	return;
    }

    if (arg2[0] == '\0')
    {
	for (obj2 = ch->carrying; obj2 != NULL; obj2 = obj2->next_content)
	{
	    if (obj2->wear_loc != WEAR_NONE
	    &&  can_see_obj(ch,obj2)
	    &&  obj1->item_type == obj2->item_type
	    &&  (obj1->wear_flags & obj2->wear_flags & ~ITEM_TAKE) != 0 )
		break;
	}

	if (obj2 == NULL)
	{
	    send_to_char("You aren't wearing anything comparable.\n\r",ch);
	    return;
	}
    } 

    else if ( (obj2 = get_obj_carry(ch,arg2) ) == NULL )
    {
	send_to_char("You do not have that item.\n\r",ch);
	return;
    }

    msg		= NULL;
    value1	= 0;
    value2	= 0;

    if ( obj1 == obj2 )
    {
	msg = "You compare $p to itself.  It looks about the same.";
    }
    else if ( obj1->item_type != obj2->item_type )
    {
	msg = "You can't compare $p and $P.";
    }
    else
    {
	switch ( obj1->item_type )
	{
	default:
	    msg = "You can't compare $p and $P.";
	    break;

	case ITEM_ARMOR:
	    value1 = obj1->value[0] + obj1->value[1] + obj1->value[2];
	    value2 = obj2->value[0] + obj2->value[1] + obj2->value[2];
	    break;

	case ITEM_WEAPON:
	    if (obj1->pIndexData->new_format)
		value1 = (1 + obj1->value[2]) * obj1->value[1];
	    else
	    	value1 = obj1->value[1] + obj1->value[2];

	    if (obj2->pIndexData->new_format)
		value2 = (1 + obj2->value[2]) * obj2->value[1];
	    else
	    	value2 = obj2->value[1] + obj2->value[2];
	    break;
	}
    }

    if ( msg == NULL )
    {
	     if ( value1 == value2 ) msg = "$p and $P look about the same.";
	else if ( value1  > value2 ) msg = "$p looks better than $P.";
	else                         msg = "$p looks worse than $P.";
    }

    act( msg, ch, obj1, obj2, TO_CHAR );
    return;
}



void do_credits( CHAR_DATA *ch, char *argument )
{
    do_help( ch, "diku" );
    return;
}

void do_where( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char pkbuf[100];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    DESCRIPTOR_DATA *d;
    bool found;
    bool fPKonly = FALSE;

    one_argument( argument, arg );

    if (!check_blind(ch))
      return;

    if (room_is_dark( ch ) && !IS_SET(ch->act, PLR_HOLYLIGHT)) 
    {
      send_to_char("It's too dark to see.\n\r",ch);
      return;
    }

    if (!str_cmp(arg, "protector"))
    {
      if (IS_SET(ch->in_room->area->area_flag, AREA_PROTECTED))
	send_to_char("This area is protected by Rulers!\n\r",ch);
      else
	send_to_char("This area is not protected.\n\r",ch);
      return;
    }

    if (!str_cmp(arg,"pk"))
      fPKonly = TRUE;

    sprintf(pkbuf,"%s%s%s",
	IS_SET(ch->act, PLR_COLOR) ? CLR_RED : "",
	"(PK) ",
	IS_SET(ch->act, PLR_COLOR) ? CLR_WHITE_BOLD : "");

    if ( arg[0] == '\0' || fPKonly)
    {
	send_to_char( "Players near you:\n\r", ch );
	found = FALSE;
	for ( d = descriptor_list; d; d = d->next )
	{
	    if ( d->connected == CON_PLAYING
	    && ( victim = d->character ) != NULL
	    &&   !IS_NPC(victim)
	    && !(fPKonly && is_safe_nomessage(ch,victim))	
	    &&   victim->in_room != NULL
	    &&   victim->in_room->area == ch->in_room->area
	    &&   can_see( ch, victim ) )

	    {
		found = TRUE;
		sprintf( buf, "%s%-28s %s\n\r",
		 (is_safe_nomessage(ch,
(is_affected(victim,gsn_doppelganger) && victim->doppel) ? 
	victim->doppel : victim) || IS_NPC(victim)) ?
			"  " :  pkbuf,
	         (is_affected(victim,gsn_doppelganger) 
		  && !IS_SET(ch->act,PLR_HOLYLIGHT)) ?
		    victim->doppel->name : victim->name,
		 victim->in_room->name );
		send_to_char( buf, ch );
	    }
	}
	if ( !found )
	    send_to_char( "None\n\r", ch );
    }
    else
    {
	found = FALSE;
	for ( victim = char_list; victim != NULL; victim = victim->next )
	{
	    if ( victim->in_room != NULL
	    &&   victim->in_room->area == ch->in_room->area
	    &&   !IS_AFFECTED(victim, AFF_HIDE)
	    &&   !IS_AFFECTED(victim, AFF_FADE)
	    &&   !IS_AFFECTED(victim, AFF_SNEAK)
	    &&   can_see( ch, victim )
	    &&   is_name( arg, victim->name ) )
	    {
		found = TRUE;
		sprintf( buf, "%-28s %s\n\r",
		    PERS(victim, ch), victim->in_room->name );
		send_to_char( buf, ch );
		break;
	    }
	}
	if ( !found )
	    act( "You didn't find any $T.", ch, NULL, arg, TO_CHAR );
    }

    return;
}




void do_consider( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    char *msg;
    char *align;
    int diff;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Consider killing whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They're not here.\n\r", ch );
	return;
    }

    if (is_safe(ch,victim))
    {
	send_to_char("Don't even think about it.\n\r",ch);
	return;
    }

    diff = victim->level - ch->level;

         if ( diff <= -10 ) msg = "You can kill $N naked and weaponless.";
    else if ( diff <=  -5 ) msg = "$N is no match for you.";
    else if ( diff <=  -2 ) msg = "$N looks like an easy kill.";
    else if ( diff <=   1 ) msg = "The perfect match!";
    else if ( diff <=   4 ) msg = "$N says 'Do you feel lucky, punk?'.";
    else if ( diff <=   9 ) msg = "$N laughs at you mercilessly.";
    else                    msg = "Death will thank you for your gift.";

    if (IS_EVIL(ch) && IS_EVIL(victim))
      align = "$N grins evilly with you.";
    else if (IS_GOOD(victim) && IS_GOOD(ch))
      align = "$N greets you warmly.";
    else if (IS_GOOD(victim) && IS_EVIL(ch))
      align = "$N smiles at you, hoping you will turn from your evil path.";
    else if (IS_EVIL(victim) && IS_GOOD(ch))
      align = "$N grins evilly at you.";
    else if (IS_NEUTRAL(ch) && IS_EVIL(victim))
      align = "$N grins evilly.";
    else if (IS_NEUTRAL(ch) && IS_GOOD(victim))
      align = "$N smiles happily.";
    else if (IS_NEUTRAL(ch) && IS_NEUTRAL(victim))
      align = "$N looks just as disinterested as you.";
    else
      align = "$N looks very disinterested.";

    act( msg, ch, NULL, victim, TO_CHAR );
    act( align, ch, NULL, victim, TO_CHAR);
    return;
}


void set_title( CHAR_DATA *ch, char *title )
{
    char buf[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) )
    {
	bug( "Set_title: NPC.", 0 );
	return;
    }

    if ( title[0] != '.' && title[0] != ',' && title[0] != '!' && title[0] != '?' )
    {
	buf[0] = ' ';
	strcpy( buf+1, title );
    }
    else
    {
	strcpy( buf, title );
    }

    free_string( ch->pcdata->title );
    ch->pcdata->title = str_dup( buf );
    return;
}


void do_title( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
        return;

    if ( CANT_CHANGE_TITLE(ch) )
	{
         send_to_char( "You can't change your title.\n\r", ch );
         return;
	}

    if ( argument[0] == '\0' )
    {
        send_to_char( "Change your title to what?\n\r", ch );
        return;
    }
          
    if ( strlen(argument) > 45 )
        argument[45] = '\0';

    smash_tilde( argument );
    set_title( ch, argument );
    send_to_char( "Ok.\n\r", ch );
}
       

void do_description( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    if ( argument[0] != '\0' )
    {
	buf[0] = '\0';
	smash_tilde( argument );

    	if (argument[0] == '-')
    	{
            int len;
            bool found = FALSE;
 
            if (ch->description == NULL || ch->description[0] == '\0')
            {
                send_to_char("No lines left to remove.\n\r",ch);
                return;
            }
	
  	    strcpy(buf,ch->description);
 
            for (len = strlen(buf); len > 0; len--)
            {
                if (buf[len] == '\r')
                {
                    if (!found)  /* back it up */
                    {
                        if (len > 0)
                            len--;
                        found = TRUE;
                    }
                    else /* found the second one */
                    {
                        buf[len + 1] = '\0';
			free_string(ch->description);
			ch->description = str_dup(buf);
			send_to_char( "Your description is:\n\r", ch );
			send_to_char( ch->description ? ch->description : 
			    "(None).\n\r", ch );
                        return;
                    }
                }
            }
            buf[0] = '\0';
	    free_string(ch->description);
	    ch->description = str_dup(buf);
	    send_to_char("Description cleared.\n\r",ch);
	    return;
        }
	if ( argument[0] == '+' )
	{
	    if ( ch->description != NULL )
		strcat( buf, ch->description );
	    argument++;
	    while ( isspace(*argument) )
		argument++;
	}

	if ( strlen(buf) + strlen(argument) >= MAX_STRING_LENGTH - 2 )
	{
	    send_to_char( "Description too long.\n\r", ch );
	    return;
	}

	strcat( buf, argument );
	strcat( buf, "\n\r" );
	free_string( ch->description );
	ch->description = str_dup( buf );
    }

    send_to_char( "Your description is:\n\r", ch );
    send_to_char( ch->description ? ch->description : "(None).\n\r", ch );
    return;
}



void do_report( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_INPUT_LENGTH];

    sprintf( buf,
	"I have %d/%d hp %d/%d mana %d/%d mv",
	ch->hit,  ch->max_hit,
	ch->mana, ch->max_mana,
	ch->move, ch->max_move );
    do_say( ch, buf );

    return;
}



void do_practice( CHAR_DATA *ch, char *argument )
{
    char buf2[10*MAX_STRING_LENGTH]; 
    char buf[MAX_STRING_LENGTH];
    int sn;

    if ( IS_NPC(ch) )
	return;

    if ( argument[0] == '\0' )
    {
	int col;

	col    = 0;
        strcpy( buf2, "" );
	for ( sn = 0; sn < MAX_SKILL; sn++ )
	{
	    if ( skill_table[sn].name == NULL )
		break;
	    if ( ch->level < skill_table[sn].skill_level[ch->iclass] ||
		!RACE_OK(ch,sn) ||
(skill_table[sn].cabal != ch->cabal && skill_table[sn].cabal != CABAL_NONE) 
	      )
		continue;

	    sprintf( buf, "%-18s %3d%%  ",
		skill_table[sn].name, ch->pcdata->learned[sn] );
	    strcat( buf2, buf );
	    if ( ++col % 3 == 0 )
		strcat( buf2, "\n\r" );
	}

	if ( col % 3 != 0 )
	    strcat( buf2, "\n\r" );

	sprintf( buf, "You have %d practice sessions left.\n\r",
	    ch->practice );  
	strcat( buf2, buf );
	
	if ( IS_IMMORTAL( ch ) )
	  page_to_char( buf2, ch );
	else
	  send_to_char( buf2, ch );  
    }
    else
    {
	CHAR_DATA *mob;
	int adept;

	if ( !IS_AWAKE(ch) )
	{
	    send_to_char( "In your dreams, or what?\n\r", ch );
	    return;
	}

	for ( mob = ch->in_room->people; mob != NULL; mob = mob->next_in_room )
	{
	    if ( IS_NPC(mob) && IS_SET(mob->act, ACT_PRACTICE) )
		break;
	}

	if ( mob == NULL )
	{
	    send_to_char( "You can't do that here.\n\r", ch );
	    return;
	}

	if ( ch->practice <= 0 )
	{
	    send_to_char( "You have no practice sessions left.\n\r", ch );
	    return;
	}

	if ( ( sn = find_spell( ch,argument ) ) < 0
	|| ( !IS_NPC(ch)
	&&   (ch->level < skill_table[sn].skill_level[ch->iclass] 
 	|| !RACE_OK(ch,sn) ||
(skill_table[sn].cabal != ch->cabal && skill_table[sn].cabal != CABAL_NONE) )))
	{
	    send_to_char( "You can't practice that.\n\r", ch );
	    return;
	}

	if (!str_cmp("vampire",skill_table[sn].name) )
	{
	 send_to_char( "You can't practice that, only available at questor.\n\r",ch);
	 return;
	}

	adept = IS_NPC(ch) ? 100 : class_table[ch->iclass].skill_adept;

	if ( ch->pcdata->learned[sn] >= adept )
	{
	    sprintf( buf, "You are already learned at %s.\n\r",
		skill_table[sn].name );
	    send_to_char( buf, ch );
	}
	else
	{
	    if (!ch->pcdata->learned[sn]) ch->pcdata->learned[sn] = 1;
	    ch->practice--;
	    ch->pcdata->learned[sn] += 
		int_app[get_curr_stat(ch,STAT_INT)].learn / 
	        UMAX(skill_table[sn].rating[ch->iclass],1);
	    if ( ch->pcdata->learned[sn] < adept )
	    {
		act( "You practice $T.",
		    ch, NULL, skill_table[sn].name, TO_CHAR );
		act( "$n practices $T.",
		    ch, NULL, skill_table[sn].name, TO_ROOM );
	    }
	    else
	    {
		ch->pcdata->learned[sn] = adept;
		act( "You are now learned at $T.",
		    ch, NULL, skill_table[sn].name, TO_CHAR );
		act( "$n is now learned at $T.",
		    ch, NULL, skill_table[sn].name, TO_ROOM );
	    }
	}
    }
    return;
}



/*
 * 'Wimpy' originally by Dionysos.
 */
void do_wimpy( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    int wimpy;

    one_argument( argument, arg );

    if ((ch->iclass == 9) && (ch->level >=10))
	{
	 sprintf(buf,"You don't deal with wimpies, or such feary things.\n\r");
	 send_to_char(buf,ch);
	 if (ch->wimpy != 0) ch->wimpy = 0;
	 return;
	}

    if ( arg[0] == '\0' )
	wimpy = ch->max_hit / 5;
    else  wimpy = atoi( arg );
    
    if ( wimpy < 0 )
    {
	send_to_char( "Your courage exceeds your wisdom.\n\r", ch );
	return;
    }

    if ( wimpy > ch->max_hit/2 )
    {
	send_to_char( "Such cowardice ill becomes you.\n\r", ch );
	return;
    }

    ch->wimpy	= wimpy;

    sprintf( buf, "Wimpy set to %d hit points.\n\r", wimpy );
    send_to_char( buf, ch );
    return;
}



void do_password( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char *pArg;
    char *pwdnew;
    char *p;
    char cEnd;

    if ( IS_NPC(ch) )
	return;

    /*
     * Can't use one_argument here because it smashes case.
     * So we just steal all its code.  Bleagh.
     */
    pArg = arg1;
    while ( isspace(*argument) )
	argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
	cEnd = *argument++;

    while ( *argument != '\0' )
    {
	if ( *argument == cEnd )
	{
	    argument++;
	    break;
	}
	*pArg++ = *argument++;
    }
    *pArg = '\0';

    pArg = arg2;
    while ( isspace(*argument) )
	argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
	cEnd = *argument++;

    while ( *argument != '\0' )
    {
	if ( *argument == cEnd )
	{
	    argument++;
	    break;
	}
	*pArg++ = *argument++;
    }
    *pArg = '\0';

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Syntax: password <old> <new>.\n\r", ch );
	return;
    }

    if ( strcmp( crypt( arg1, ch->pcdata->pwd ), ch->pcdata->pwd ) )
    {
	WAIT_STATE( ch, 40 );
	send_to_char( "Wrong password.  Wait 10 seconds.\n\r", ch );
	return;
    }

    if ( strlen(arg2) < 5 )
    {
	send_to_char(
	    "New password must be at least five characters long.\n\r", ch );
	return;
    }

    /*
     * No tilde allowed because of player file format.
     */
    pwdnew = crypt( arg2, ch->name );
    for ( p = pwdnew; *p != '\0'; p++ )
    {
	if ( *p == '~' )
	{
	    send_to_char(
		"New password not acceptable, try again.\n\r", ch );
	    return;
	}
    }

    free_string( ch->pcdata->pwd );
    ch->pcdata->pwd = str_dup( pwdnew );
    save_char_obj( ch );
    send_to_char( "Ok.\n\r", ch );
    return;
}

/* RT configure command */

void do_scan(CHAR_DATA *ch, char *argument)
{
  char dir[MAX_INPUT_LENGTH];
  char *dir2;
  ROOM_INDEX_DATA *in_room;
  ROOM_INDEX_DATA *to_room;
  EXIT_DATA *exit;	/* pExit */
  int door;
  int range;
  char buf[MAX_STRING_LENGTH];
  int i;
  CHAR_DATA *person;
  int numpeople;

  one_argument(argument,dir);

 if (dir[0] == '\0')

   {
	do_scan2(ch,"");
	return;
   }

  switch (dir[0])
    {
    case 'N':    case 'n':      door = 0;      dir2 = "north";	break;
    case 'E':    case 'e':      door = 1;      dir2 = "east";	break;
    case 'S':    case 's':      door = 2;      dir2 = "south";	break;
    case 'W':    case 'w':      door = 3;      dir2 = "west";	break;
    case 'U':    case 'u':      door = 4;      dir2 = "up";	break;
    case 'D':    case 'd':      door = 5;      dir2 = "down";	break;
    default:
      send_to_char("That's not a direction.\n\r",ch);
      return;
    }
  
  sprintf(buf, "You scan %s.\n\r",dir2);
  send_to_char(buf,ch);
  sprintf(buf, "$n scans %s.",dir2);
  act(buf,ch,NULL,NULL,TO_ROOM);

  if (!check_blind(ch))
    return;

  range = 1 + (ch->level)/10;
  
  in_room = ch->in_room;
  
  for (i=1; i <= range; i++)
    {
      if ( (exit = in_room->exit[door]) == NULL
	  || (to_room = exit->u1.to_room) == NULL
	  || IS_SET(exit->exit_info,EX_CLOSED) )
	return;

      for (numpeople = 0,person = to_room->people; person != NULL;
	   person = person->next_in_room)
	if (can_see(ch,person)) numpeople++;
	      
      if (numpeople)
	{
	  sprintf(buf, "***** Range %d *****\n\r",i);
	  send_to_char(buf,ch);
	  show_char_to_char(to_room->people, ch);
	  send_to_char("\n\r", ch);
	}
      in_room = to_room;
    }
}

void do_request( CHAR_DATA *ch, char *argument)
{
  char arg1 [MAX_INPUT_LENGTH];
  char arg2 [MAX_INPUT_LENGTH];
  CHAR_DATA *victim;
  OBJ_DATA  *obj;
  AFFECT_DATA af;

  if ( is_affected(ch, gsn_reserved))
	{
	 send_to_char("Wait for a while to request again.\n\r",ch);
	 return;
	}

  
  argument = one_argument( argument, arg1 );
  argument = one_argument( argument, arg2 );
 
  if (IS_NPC(ch))
	return;
 
  if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
      send_to_char( "Request what from whom?\n\r", ch );
      return;
    }
  
  if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
    {
      send_to_char( "They aren't here.\n\r", ch );
      return;
    }
  
  if (!IS_NPC(victim))
    {
      send_to_char("Why don't you just ask the player?\n\r", ch);
      return;
    }

  if (!IS_GOOD(ch))
    {
      do_say(victim, "I will not give anything to someone so impure.");
      return;
    }

  if (ch->move < (50 + ch->level))
    {
      do_say(victim, "You look rather tired, why don't you rest a bit first?");
      return;
    }

  WAIT_STATE(ch, PULSE_VIOLENCE);
  ch->move -= 10;
  ch->move = UMAX(ch->move, 0);

  if (victim->level >= ch->level + 10 || victim->level >= ch->level * 2)
    {
      do_say(victim, "In good time, my child");
      return;
    }

  if (( ( obj = get_obj_carry(victim , arg1 ) ) == NULL 
      && (obj = get_obj_wear(victim, arg1)) == NULL)
      || IS_SET(obj->extra_flags, ITEM_INVENTORY))
    {
      do_say(victim, "Sorry, I don't have that.");
      return;
    }
  
  if (!IS_GOOD(victim))
    {
      do_say(victim, "I'm not about to give you anything!");
      do_murder(victim, ch->name);
      return;
    }
  
  if ( obj->wear_loc != WEAR_NONE )
    unequip_char(victim, obj);
  
  if ( !can_drop_obj( ch, obj ) )
    {
      do_say(victim, "Sorry, I can't let go of it.  It's cursed.");
      return;
    }
  
  if ( ch->carry_number + get_obj_number( obj ) > can_carry_n( ch ) )
    {
      send_to_char( "Your hands are full.\n\r", ch);
      return;
    }
  
  if ( ch->carry_weight + get_obj_weight( obj ) > can_carry_w( ch ) )
    {
      send_to_char( "You can't carry that much weight.\n\r", ch);
      return;
    }
  
  if ( !can_see_obj( ch, obj ) )
    {
      act( "You don't see that.", ch, NULL, victim, TO_CHAR );
      return;
    }

  obj_from_char( obj );
  obj_to_char( obj, ch );
  act( "$n requests $p from $N.", ch, obj, victim, TO_NOTVICT );
  act( "You request $p from $N.",   ch, obj, victim, TO_CHAR    );
  act( "$n requests $p from you.", ch, obj, victim, TO_VICT    );



  if (IS_SET(obj->progtypes,OPROG_GIVE))
    (obj->pIndexData->oprogs->give_prog) (obj,ch,victim);

  if (IS_SET(victim->progtypes,MPROG_GIVE))
    (victim->pIndexData->mprogs->give_prog) (victim,ch,obj);



  ch->move -= (50 + ch->level);
  ch->move = UMAX(ch->move, 0);
  ch->hit -= 3 * (ch->level / 2);
  ch->hit = UMAX(ch->hit, 0);

  act("You feel grateful for the trust of $N.", ch, NULL, victim,TO_CHAR);  
  send_to_char("and for the goodness you have seen in the world.\n\r",ch);
  
  af.type = gsn_reserved;
  af.where = TO_AFFECTS;
  af.level = ch->level;
  af.duration = ch->level / 10;
  af.location = APPLY_NONE;
  af.modifier = 0;
  af.bitvector = 0;
  affect_to_char ( ch,&af );

  return;
}
  

void do_hometown(CHAR_DATA *ch, char *argument)
{
  int amount;
  char buf[MAX_INPUT_LENGTH];

  if (IS_NPC(ch))
    {
      send_to_char("You can't change your hometown!\n\r", ch);
      return;
    }

  if (ORG_RACE(ch) == 11 || ORG_RACE(ch) == 12
	|| ORG_RACE(ch) == 13 || ORG_RACE(ch) == 14 )
    {
      send_to_char("Your hometown is permenantly Titan Valley!\n\r", ch);
      return;
    }

  if (ch->iclass == 10 || ch->iclass == 11)
    {
      send_to_char("Your hometown is permenantly Old Midgaard!\n\r", ch);
      return;
    }

  if (!IS_SET(ch->in_room->room_flags, ROOM_REGISTRY))
    {
  send_to_char("You have to be in the Registry to change your hometown.\n\r",
      ch);
      return;
    }
  
  amount = (ch->level * ch->level * 250) + 1000;

  if (argument[0] == '\0')
    {
      sprintf(buf, "It will cost you %d gold.\n\r", amount);
      send_to_char(buf, ch);
      return;
    }

  if (ch->gold < amount)
    {
      send_to_char("You don't have enough money to change hometowns!\n\r",ch);
      return;
    }
  if (!str_prefix(argument, "midgaard"))
    {
      if (ch->hometown == 0)
        {
	  send_to_char("But you already live in Midgaard!\n\r", ch);
	  return;
	}
      ch->gold -= amount;
      send_to_char("Your hometown is changed to Midgaard.\n\r", ch);
      ch->hometown = 0;
      return;
    }

  else if (!str_prefix(argument, "newthalos"))
    {
      if (ch->hometown == 1)
        {
	  send_to_char("But you already live in New Thalos!\n\r", ch);
	  return;
	}
      ch->gold -= amount;
      send_to_char("Your hometown is changed to New Thalos.\n\r", ch);
      ch->hometown = 1;
      return;
    }

  else if (!str_prefix(argument, "titans"))
    {
      if (ch->hometown == 2)
        {
	  send_to_char("But you already live in Titan!\n\r", ch);
	  return;
	}
      ch->gold -= amount;
      send_to_char("Your hometown is changed to Titan.\n\r", ch);
      ch->hometown = 2;
      return;
    }

  else if (!str_prefix(argument, "ofcol"))
    {
      if (!IS_NEUTRAL(ch))
	{
	  send_to_char("Only neutral people can live in Ofcol.\n\r", ch);
	  return;
 	}
      if (ch->hometown == 3)
        {
	  send_to_char("But you already live in Ofcol!\n\r", ch);
	  return;
	}
      ch->gold -= amount;
      send_to_char("Your hometown is changed to Ofcol.\n\r", ch);
      ch->hometown = 3;
      return;
    }

  else if (!str_prefix(argument, "oldmidgaard"))
    {
      if (ch->iclass == CLASS_VAMPIRE || ch->iclass == CLASS_NECROMANCER)
	{
	  send_to_char("Only vampires and necromancers live there.\n\r", ch);
	  return;
 	}
      if (ch->hometown == 4)
        {
	  send_to_char("But you already live in Old Midgaard!\n\r", ch);
	  return;
	}
      ch->gold -= amount;
      send_to_char("Your hometown is changed to Old Midgaard.\n\r", ch);
      ch->hometown = 4;
      return;
    }

  else
    {
      send_to_char("That is not a valid choice.\n\r", ch);
send_to_char("Choose from Midgaard, Newthalos, Titans, Ofcol and Old Midgaard.\n\r",
      		ch);
      return;
    }
}


void do_detect_hidden( CHAR_DATA *ch, char *argument)
{
    AFFECT_DATA af;


  if (IS_NPC(ch) ||
      ch->level < skill_table[gsn_detect_hidden].skill_level[ch->iclass] )
    {
      send_to_char( "Huh?\n\r", ch );
      return;
    }

    if ( CAN_DETECT(ch, DETECT_HIDDEN) )
    {
	send_to_char("You are already as alert as you can be. \n\r",ch);
	return;
    }
    if ( number_percent( ) > get_skill( ch, gsn_detect_hidden ) )  {
      send_to_char( 
         "You peer intently at the shadows but they are unrevealing.\n\r", 
         ch );
      return;
    }
    af.where     = TO_DETECTS;
    af.type      = gsn_detect_hidden;
    af.level     = ch->level;
    af.duration  = ch->level;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = DETECT_HIDDEN;
    affect_to_char( ch, &af );
    send_to_char( "Your awareness improves.\n\r", ch );
    return;
}


void do_bear_call( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *gch;
  CHAR_DATA *bear;
  CHAR_DATA *bear2;
  AFFECT_DATA af;
  int i;
  
  if (IS_NPC(ch) ||
      ch->level < skill_table[gsn_bear_call].skill_level[ch->iclass] )
    {
      send_to_char( "Huh?\n\r", ch );
      return;
    }

  send_to_char("You call for bears help you.\n\r",ch);
  act("$n shouts a bear call.",ch,NULL,NULL,TO_ROOM);

  if (is_affected(ch, gsn_bear_call))
    {
      send_to_char("You cannot summon the strength to handle more bears right now.\n\r", ch);
      return;
    }
  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  gch->pIndexData->vnum == MOB_VNUM_BEAR)
	{
	  send_to_char("What's wrong with the bear you've got?",ch);
	  return;
	}
    }

  if ( ch->in_room != NULL && IS_SET(ch->in_room->room_flags, ROOM_NO_MOB) )
  {
     send_to_char( "No bears listen you.\n\r", ch );
     return;
  }

  if ( number_percent( ) > get_skill( ch, gsn_bear_call) )
  {
	send_to_char( "No bears listen you.\n\r", ch );
	check_improve(ch,gsn_bear_call,TRUE,1);
	return;
  }
  
  if ( IS_SET(ch->in_room->room_flags, ROOM_SAFE)      ||
       IS_SET(ch->in_room->room_flags, ROOM_PRIVATE)   ||
       IS_SET(ch->in_room->room_flags, ROOM_SOLITARY)  ||
       (ch->in_room->exit[0] == NULL &&
          ch->in_room->exit[1] == NULL &&
          ch->in_room->exit[2] == NULL &&
          ch->in_room->exit[3] == NULL &&
          ch->in_room->exit[4] == NULL &&
          ch->in_room->exit[5] == NULL) ||

         ( ch->in_room->sector_type != SECT_FIELD &&
           ch->in_room->sector_type != SECT_FOREST &&
           ch->in_room->sector_type != SECT_MOUNTAIN &&
           ch->in_room->sector_type != SECT_HILLS ) )
  {
    send_to_char( "No bears come to your rescue.\n\r", ch );
    return;
  }

  if ( ch->mana < 125 )
  {
     send_to_char( "You don't have enough mana to shout a bear call.\n\r", ch );
     return;
  }
  ch->mana -= 125;

  check_improve(ch,gsn_bear_call,TRUE,1);
  bear = create_mobile( get_mob_index(MOB_VNUM_BEAR) );

  for (i=0;i < MAX_STATS; i++)
    {
      bear->perm_stat[i] = UMIN(25,2 * ch->perm_stat[i]);
    }

  bear->max_hit = IS_NPC(ch)? ch->max_hit : ch->pcdata->perm_hit;
  bear->hit = bear->max_hit;
  bear->max_mana = IS_NPC(ch)? ch->max_mana : ch->pcdata->perm_mana;
  bear->mana = bear->max_mana;
  bear->alignment = ch->alignment;
  bear->level = UMIN(100,1 * ch->level-2);
  for (i=0; i < 3; i++)
    bear->armor[i] = interpolate(bear->level,100,-100);
  bear->armor[3] = interpolate(bear->level,100,0);
  bear->sex = ch->sex;
  bear->gold = 0;
    
  bear2 = create_mobile(bear->pIndexData);
  clone_mobile(bear,bear2);
  
  SET_BIT(bear->affected_by, AFF_CHARM);
  SET_BIT(bear2->affected_by, AFF_CHARM);
  bear->master = bear2->master = ch;
  bear->leader = bear2->leader = ch;

  char_to_room(bear,ch->in_room);
  char_to_room(bear2,ch->in_room);
  send_to_char("Two bears come to your rescue!\n\r",ch);
  act("Two bears come to $n's rescue!",ch,NULL,NULL,TO_ROOM);

  af.where		= TO_AFFECTS;
  af.type               = gsn_bear_call;
  af.level              = ch->level; 
  af.duration           = 24;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);  

}


void do_identify( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    CHAR_DATA *rch;

    if ( ( obj = get_obj_carry( ch, argument ) ) == NULL )
    {
       send_to_char( "You are not carrying that.\n\r", ch );
       return;
    }

    for ( rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room )
       if (IS_NPC(rch) && rch->pIndexData->vnum == MOB_VNUM_SAGE)
          break;

    if (!rch)
    {
       send_to_char("No one here seems to know much about that.\n\r", ch);
       return;
    }

    if (IS_IMMORTAL(ch))
       act( "$n looks at you!\n\r", rch, obj, ch, TO_VICT );
    else if (ch->gold < 1)
       {
       act( "$n resumes to identify by looking at $p.",
             rch, obj, 0, TO_ROOM );
	send_to_char(" You need at least 1 gold.\n\r", ch);
       return;
       }
    else
       {
       ch->gold -= 1;
       send_to_char("Your purse feels lighter.\n\r", ch);
       }

    act( "$n gives a wise look at $p.", rch, obj, 0, TO_ROOM );
    spell_identify( 0, 0, ch, obj ,0);
}


void do_score_col( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char buf1[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    AFFECT_DATA *paf;
    int i;


    sprintf( buf,
	"You are %s%s%s%s, level %s%d%s, %s%d%s years old (%d hours).\n\r",
	CLR_YELLOW,ch->name,CLR_WHITE_BOLD,
	IS_NPC(ch) ? "" : ch->pcdata->title,	
	CLR_YELLOW,ch->level,CLR_WHITE_BOLD,
	CLR_WHITE,get_age(ch),CLR_WHITE_BOLD,
        ( ch->played + (int) (current_time - ch->logon) ) / 3600);
    send_to_char( buf, ch );

    if ( get_trust( ch ) != ch->level )
    {
	sprintf( buf, "You are trusted at level %d.\n\r",
	    get_trust( ch ) );
	send_to_char( buf, ch );
    }

  sprintf(buf, "Race: %s%s%s  Sex: %s%s%s  Class: %s%s%s  Hometown: %s%s%s\n\r",
	CLR_BLUE_BOLD,race_table[ORG_RACE(ch)].name,CLR_WHITE_BOLD,
	CLR_BLUE_BOLD,
	ch->sex == 0 ? "sexless" : ch->sex == 1 ? "male" : "female",
	CLR_WHITE_BOLD,CLR_BLUE_BOLD,
 	IS_NPC(ch) ? "mobile" : class_table[ch->iclass].name,
	CLR_WHITE_BOLD,CLR_BLUE_BOLD,
	IS_NPC(ch) ? "Midgaard" : hometown_table[ch->hometown].name,
	CLR_WHITE_BOLD);
    send_to_char(buf,ch);
	
      sprintf( buf,
	"You have %d/%s%d%s hit, %d/%s%d%s mana, %d/%s%d%s movement.\n\r",
	ch->hit,  CLR_WHITE,ch->max_hit,CLR_WHITE_BOLD,
	ch->mana, CLR_WHITE,ch->max_mana,CLR_WHITE_BOLD,
	ch->move, CLR_WHITE,ch->max_move,CLR_WHITE_BOLD);
  
    send_to_char( buf, ch );

    sprintf( buf,
	"You have %s%d%s practices and %s%d%s training sessions.\n\r",
	CLR_BLUE_BOLD,ch->practice,CLR_WHITE_BOLD,
	CLR_BLUE_BOLD,ch->train,CLR_WHITE_BOLD);
    send_to_char( buf, ch );

    sprintf( buf,
"You are carrying %s%d%s/%s%d%s items with weight %s%ld%s/%s%d%s pounds.\n\r",
	CLR_CYAN,ch->carry_number,CLR_WHITE_BOLD,
	CLR_BLUE_BOLD,can_carry_n(ch),CLR_WHITE_BOLD,
	CLR_CYAN,get_carry_weight(ch),CLR_WHITE_BOLD,
	CLR_BLUE_BOLD,can_carry_w(ch),CLR_WHITE_BOLD );
    send_to_char( buf, ch );

    if ( ch->level > 20 || IS_NPC(ch) )  {
      sprintf( buf,
"Str: %s%d%s(%s%d%s)  Int: %s%d%s(%s%d%s)  Wis: %s%d%s(%s%d%s)  Dex: %s%d%s(%s%d%s)  Con: %s%d%s(%s%d%s) Cha: %s%d%s(%s%d%s)\n\r",
	CLR_YELLOW,ch->perm_stat[STAT_STR],CLR_WHITE_BOLD,
	CLR_BLUE,get_curr_stat(ch,STAT_STR),CLR_WHITE_BOLD,
	CLR_YELLOW,ch->perm_stat[STAT_INT],CLR_WHITE_BOLD,
	CLR_BLUE,get_curr_stat(ch,STAT_INT),CLR_WHITE_BOLD,
	CLR_YELLOW,ch->perm_stat[STAT_WIS],CLR_WHITE_BOLD,
	CLR_BLUE,get_curr_stat(ch,STAT_WIS),CLR_WHITE_BOLD,
	CLR_YELLOW,ch->perm_stat[STAT_DEX],CLR_WHITE_BOLD,
	CLR_BLUE,get_curr_stat(ch,STAT_DEX),CLR_WHITE_BOLD,
	CLR_YELLOW,ch->perm_stat[STAT_CON],CLR_WHITE_BOLD,
	CLR_BLUE,get_curr_stat(ch,STAT_CON),CLR_WHITE_BOLD,
	CLR_YELLOW,ch->perm_stat[STAT_CHA],CLR_WHITE_BOLD,
	CLR_BLUE,get_curr_stat(ch,STAT_CHA),CLR_WHITE_BOLD );
      send_to_char( buf, ch );
    }
    else  {
	sprintf( buf,
 "Str: %s%-9s%s Wis: %s%-9s%s Con: %s%-9s%s\n\rInt: %s%-9s%s Dex: %s%-9s%s Cha: %s%-11s%s\n\r",
	CLR_YELLOW,get_stat_alias( ch, STAT_STR ),CLR_WHITE_BOLD,
	CLR_YELLOW,get_stat_alias( ch, STAT_WIS ),CLR_WHITE_BOLD,
	CLR_YELLOW,get_stat_alias( ch, STAT_CON ),CLR_WHITE_BOLD,
	CLR_YELLOW,get_stat_alias( ch, STAT_INT ),CLR_WHITE_BOLD,
	CLR_YELLOW,get_stat_alias( ch, STAT_DEX ),CLR_WHITE_BOLD,
	CLR_YELLOW,get_stat_alias( ch, STAT_CHA ),CLR_WHITE_BOLD );

      send_to_char( buf, ch );
    }

    sprintf( buf, "You have scored %s%d%s exp, and have %s%s%s%s%s.\n\r",
	CLR_MAGENTA,ch->exp,CLR_WHITE_BOLD,
	CLR_BLUE_BOLD,
	ch->gold+ch->silver==0?"no money":ch->gold!=0?"%ld gold ":"",
        ch->silver!=0?"%ld silver ":"", 
        ch->gold+ch->silver!=0?ch->gold+ch->silver==1?"coin":"coins":"" ,
	CLR_WHITE_BOLD );
    if ( ch->gold != 0 )
      sprintf( buf2, buf, ch->gold, ch->silver );
    else 
      sprintf( buf2, buf, ch->silver );

    send_to_char( buf2, ch );

    /* KIO shows exp to level */
    if (!IS_NPC(ch) && ch->level < LEVEL_HERO)
    {
      sprintf (buf, 
	"You need %s%d%s exp to level.\n\r",
        CLR_CYAN,exp_to_level(ch,ch->pcdata->points),CLR_WHITE_BOLD);
      send_to_char( buf, ch );
     }

    if (!(IS_NPC(ch)))
    {
      sprintf (buf, 
	"Quest Points: %s%d%s.	Next Quest Time: %s%d%s.\n\r",
         CLR_BLUE_BOLD,ch->pcdata->questpoints,CLR_WHITE_BOLD,
	 CLR_BLUE_BOLD,ch->pcdata->nextquest,CLR_WHITE_BOLD);
      send_to_char( buf, ch );
    }
            
	if (ch->iclass != 9)
	{
         sprintf( buf, "Wimpy set to %s%d%s hit points.  ", 
		CLR_RED,ch->wimpy,CLR_WHITE_BOLD );
	 send_to_char( buf, ch );
	}
	else 
	{
	 sprintf(buf,"Total %s%d%s deaths up to now.",
		CLR_RED,ch->pcdata->death,CLR_WHITE_BOLD);
	 send_to_char(buf, ch);
	}
    if (ch->guarding != NULL)
      {
	sprintf(buf1, "You are guarding: %s%s%s  ", 
		CLR_GREEN,ch->guarding->name,CLR_WHITE_BOLD);
	send_to_char( buf1, ch);
      }

    if (ch->guarded_by != NULL)
      {
	sprintf(buf2, "You are guarded by: %s%s%s",
		CLR_GREEN, ch->guarded_by->name,CLR_WHITE_BOLD);
	send_to_char(buf2, ch);
      }

    send_to_char("\n\r", ch);
    send_to_char(CLR_WHITE,ch);
    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK]   > 10 )
	send_to_char( "You are drunk.\n\r",   ch );
    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_THIRST] <=  0 )
	send_to_char( "You are thirsty.\n\r", ch );
/*    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_FULL]   ==  0 ) */
    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_HUNGER]   <=  0 )
	send_to_char( "You are hungry.\n\r",  ch );
    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_BLOODLUST]   <=  0 )
	send_to_char( "You are hungry for blood.\n\r",  ch );
    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_DESIRE]   <=  0 )
	send_to_char( "You are desiring your home.\n\r",  ch );
    send_to_char(CLR_WHITE_BOLD,ch);
	
    switch ( ch->position )
    {
    case POS_DEAD:     
	send_to_char(CLR_RED,ch);
	send_to_char( "You are DEAD!!\n\r",		ch );
	break;
    case POS_MORTAL:
	send_to_char(CLR_RED,ch);
	send_to_char( "You are mortally wounded.\n\r",	ch );
	break;
    case POS_INCAP:
	send_to_char(CLR_RED,ch);
	send_to_char( "You are incapacitated.\n\r",	ch );
	break;
    case POS_STUNNED:
	send_to_char(CLR_RED,ch);
	send_to_char( "You are stunned.\n\r",		ch );
	break;
    case POS_SLEEPING:
	send_to_char(CLR_BROWN,ch);
	send_to_char( "You are sleeping.",		ch );
    if ( ch->last_fight_time != -1 && !IS_IMMORTAL(ch) &&
        (current_time - ch->last_fight_time)<FIGHT_DELAY_TIME) 
        send_to_char("Your adrenalin is gushing!\n\r",ch);
	else send_to_char("\n\r",ch);
	break;
    case POS_RESTING:
	send_to_char(CLR_BLUE,ch);
	send_to_char( "You are resting.",		ch );
    if ( ch->last_fight_time != -1 && !IS_IMMORTAL(ch) &&
        (current_time - ch->last_fight_time)<FIGHT_DELAY_TIME) 
        send_to_char("Your adrenalin is gushing!\n\r",ch);
	else send_to_char("\n\r",ch);
	break;
    case POS_STANDING:
	send_to_char(CLR_CYAN,ch);
	send_to_char( "You are standing.",		ch );
    if ( ch->last_fight_time != -1 && !IS_IMMORTAL(ch) &&
        (current_time - ch->last_fight_time)<FIGHT_DELAY_TIME) 
        send_to_char("Your adrenalin is gushing!\n\r",ch);
	else send_to_char("\n\r",ch);
	break;
    case POS_FIGHTING:
	send_to_char(CLR_RED,ch);
	send_to_char( "You are fighting.",		ch );
    if ( ch->last_fight_time != -1 && !IS_IMMORTAL(ch) &&
        (current_time - ch->last_fight_time)<FIGHT_DELAY_TIME) 
        send_to_char("Your adrenalin is gushing!\n\r",ch);
	else send_to_char("\n\r",ch);
	break;
    }
    send_to_char(CLR_WHITE_BOLD,ch);


    /* print AC values */
    if (ch->level >= 25)
    {	
	sprintf( buf,"Armor: pierce: %s%d%s  bash: %s%d%s  slash: %s%d%s  magic: %s%d%s\n\r",
	 CLR_BLUE_BOLD,GET_AC(ch,AC_PIERCE),CLR_WHITE_BOLD,
	 CLR_BLUE_BOLD,GET_AC(ch,AC_BASH),CLR_WHITE_BOLD,
	 CLR_BLUE_BOLD,GET_AC(ch,AC_SLASH),CLR_WHITE_BOLD,
	 CLR_BLUE_BOLD,GET_AC(ch,AC_EXOTIC),CLR_WHITE_BOLD);
    	send_to_char(buf,ch);
    }

    for (i = 0; i < 4; i++)
    {
	char  temp[100];

	switch(i)
	{
	    case(AC_PIERCE):
		sprintf(temp,"%spiercing%s",CLR_RED,CLR_WHITE_BOLD);
		break;
	    case(AC_BASH):
		sprintf(temp,"%sbashing%s",CLR_RED,CLR_WHITE_BOLD);
		break;
	    case(AC_SLASH):
		sprintf(temp,"%sslashing%s",CLR_RED,CLR_WHITE_BOLD);
		break;
	    case(AC_EXOTIC):
		sprintf(temp,"%smagic%s",CLR_RED,CLR_WHITE_BOLD);
		break;
	    default:
		sprintf(temp,"%serror%s",CLR_RED,CLR_WHITE_BOLD);
		break;
	}
	
	send_to_char("You are ", ch);

	if      (GET_AC(ch,i) >=  101 ) 
	    sprintf(buf,"hopelessly vulnerable to %s.\n\r",temp);
	else if (GET_AC(ch,i) >= 80) 
	    sprintf(buf,"defenseless against %s.\n\r",temp);
	else if (GET_AC(ch,i) >= 60)
	    sprintf(buf,"barely protected from %s.\n\r",temp);
	else if (GET_AC(ch,i) >= 40)
	    sprintf(buf,"slightly armored against %s.\n\r",temp);
	else if (GET_AC(ch,i) >= 20)
	    sprintf(buf,"somewhat armored against %s.\n\r",temp);
	else if (GET_AC(ch,i) >= 0)
	    sprintf(buf,"armored against %s.\n\r",temp);
	else if (GET_AC(ch,i) >= -20)
	    sprintf(buf,"well-armored against %s.\n\r",temp);
	else if (GET_AC(ch,i) >= -40)
	    sprintf(buf,"very well-armored against %s.\n\r",temp);
	else if (GET_AC(ch,i) >= -60)
	    sprintf(buf,"heavily armored against %s.\n\r",temp);
	else if (GET_AC(ch,i) >= -80)
	    sprintf(buf,"superbly armored against %s.\n\r",temp);
	else if (GET_AC(ch,i) >= -100)
	    sprintf(buf,"almost invulnerable to %s.\n\r",temp);
	else
	    sprintf(buf,"divinely armored against %s.\n\r",temp);

	send_to_char(buf,ch);
    }


    /* RT wizinvis and holy light */
    if ( IS_IMMORTAL(ch))
    {
      send_to_char(CLR_GREEN,ch);	
      send_to_char("Holy Light: ",ch);
      if (IS_SET(ch->act,PLR_HOLYLIGHT))
        send_to_char("on",ch);
      else
        send_to_char("off",ch);
      send_to_char(CLR_WHITE_BOLD,ch);

      if (ch->invis_level)
      {
        sprintf( buf, "  Invisible: level %d",ch->invis_level);
        send_to_char(buf,ch);
      }

      if (ch->incog_level)
      {
        sprintf( buf, "  Incognito: level %d",ch->invis_level);
        send_to_char(buf,ch);
      }
      send_to_char("\n\r",ch);

    }
    if ( ch->level >= 20 )  
    {
	sprintf( buf, "Hitroll: %s%d%s  Damroll: %s%d%s.\n\r",
	    CLR_YELLOW,GET_HITROLL(ch),CLR_WHITE_BOLD, 
	    CLR_YELLOW,GET_DAMROLL(ch),CLR_WHITE_BOLD );
	send_to_char( buf, ch );
    }
    
    send_to_char( "You are ", ch );
    if (IS_GOOD(ch)) { send_to_char(CLR_WHITE,ch);send_to_char("good.  ", ch);}
    else if (IS_EVIL(ch)) {send_to_char(CLR_RED,ch);send_to_char("evil.  ",ch);}
    else {send_to_char(CLR_CYAN,ch);send_to_char ("neutral.  ", ch);}
    send_to_char(CLR_WHITE_BOLD,ch);

    if (ch->ethos == 1) send_to_char("You have a lawful ethos.\n\r", ch);
    else if (ch->ethos == 2) send_to_char("You have a neutral ethos.\n\r", ch);
    else if (ch->ethos == 3) send_to_char("You have a chaotic ethos.\n\r", ch);
    else if (!IS_NPC(ch))
    send_to_char("You have no ethos, report it to the gods!\n\r", ch);
    if (IS_NPC(ch)) ch->religion = 0;
    if ((ch->religion <= RELIGION_NONE) || (ch->religion > MAX_RELIGION) )
	send_to_char("You don't believe any religion.\n\r",ch);
    else 
	{
	 sprintf(buf,"Your religion is the way of %s%s%s.\n\r",
	CLR_BLUE_BOLD,religion_table[ch->religion].leader,CLR_WHITE_BOLD);
	 send_to_char(buf,ch);
	}	
    if ( ch->affected != NULL && IS_SET(ch->comm, COMM_SHOW_AFFECTS) )
    {
	send_to_char( "You are affected by:\n\r", ch );
	for ( paf = ch->affected; paf != NULL; paf = paf->next )
	{
	    sprintf( buf, "%sSpell%s: '%s%s%s'", 
		CLR_RED,CLR_WHITE_BOLD,CLR_YELLOW,
		skill_table[paf->type].name,CLR_WHITE_BOLD );
	    send_to_char( buf, ch );

	    if ( ch->level >= 20 )
	    {
		if ( paf->duration != -1 && paf->duration != -2)
		  sprintf( buf,
		      " modifies %s%s%s by %s%d%s for %s%d%s hours",
		CLR_MAGENTA,affect_loc_name( paf->location ),CLR_WHITE_BOLD,
		CLR_MAGENTA,paf->modifier,CLR_WHITE_BOLD,
		CLR_MAGENTA,paf->duration,CLR_WHITE_BOLD );
		else 
		  sprintf( buf,
		      " modifies %s%s%s by %s%d%s %spermenantly%s",
		CLR_MAGENTA,affect_loc_name( paf->location ),CLR_WHITE_BOLD,
		CLR_MAGENTA,paf->modifier,CLR_WHITE_BOLD,
		CLR_CYAN,CLR_WHITE_BOLD );
		send_to_char( buf, ch );
	    }

	    send_to_char( ".\n\r", ch );
	}
    }

    return;
}


void do_affects_col(CHAR_DATA *ch, char *argument )
{
    AFFECT_DATA *paf, *paf_last = NULL;
    char buf[MAX_STRING_LENGTH];
    
    if ( ch->affected != NULL )
    {
	send_to_char( "You are affected by the following spells:\n\r", ch );
	for ( paf = ch->affected; paf != NULL; paf = paf->next )
	{
	    if (paf_last != NULL && paf->type == paf_last->type)
		if (ch->level >= 20 )
		    sprintf( buf, "                      ");
		else
		    continue;
	    else
	    	sprintf( buf, "%sSpell%s: %s%-15s%s", 
		CLR_RED,CLR_WHITE_BOLD,CLR_YELLOW,
		skill_table[paf->type].name ,CLR_WHITE_BOLD);

	    send_to_char( buf, ch );

	    if ( ch->level >= 20 )
	    {
		sprintf( buf,
		    ": modifies %s%s%s by %s%d%s ",
		    CLR_MAGENTA,affect_loc_name( paf->location ),CLR_WHITE_BOLD,
		    CLR_MAGENTA,paf->modifier,CLR_WHITE_BOLD);
		send_to_char( buf, ch );
		if ( paf->duration == -1 || paf->duration == -2)
		    sprintf( buf, "%spermanently%s" ,CLR_CYAN,CLR_WHITE_BOLD);
		else
		    sprintf( buf, "for %s%d%s hours", 
			CLR_MAGENTA,paf->duration ,CLR_WHITE_BOLD);
		send_to_char( buf, ch );
	    }
	    send_to_char( "\n\r", ch );
	    paf_last = paf;
	}
    }
    else 
	send_to_char("You are not affected by any spells.\n\r",ch);

    return;
}


void do_lion_call( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *gch;
  CHAR_DATA *bear;
  CHAR_DATA *bear2;
  AFFECT_DATA af;
  int i;
  
  if (IS_NPC(ch) ||
      ch->level < skill_table[gsn_lion_call].skill_level[ch->iclass] )
    {
      send_to_char( "Huh?\n\r", ch );
      return;
    }

  send_to_char("You call for lions help you.\n\r",ch);
  act("$n shouts a lion call.",ch,NULL,NULL,TO_ROOM);

  if (is_affected(ch, gsn_lion_call))
    {
      send_to_char("You cannot summon the strength to handle more lions right now.\n\r", ch);
      return;
    }
  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  gch->pIndexData->vnum == MOB_VNUM_LION)
	{
	  send_to_char("What's wrong with the lion you've got?",ch);
	  return;
	}
    }

  if ( ch->in_room != NULL && IS_SET(ch->in_room->room_flags, ROOM_NO_MOB) )
  {
     send_to_char( "No lions can listen you.\n\r", ch );
     return;
  }

  if ( number_percent( ) > get_skill( ch, gsn_lion_call) )
  {
	send_to_char( "No lions listen you.\n\r", ch );
	return;
  }
  
  if ( IS_SET(ch->in_room->room_flags, ROOM_SAFE)      ||
       IS_SET(ch->in_room->room_flags, ROOM_PRIVATE)   ||
       IS_SET(ch->in_room->room_flags, ROOM_SOLITARY)  ||
       (ch->in_room->exit[0] == NULL &&
          ch->in_room->exit[1] == NULL &&
          ch->in_room->exit[2] == NULL &&
          ch->in_room->exit[3] == NULL &&
          ch->in_room->exit[4] == NULL &&
          ch->in_room->exit[5] == NULL) ||
         ( ch->in_room->sector_type != SECT_FIELD &&
           ch->in_room->sector_type != SECT_FOREST &&
           ch->in_room->sector_type != SECT_MOUNTAIN &&
           ch->in_room->sector_type != SECT_HILLS ) )
  {
    send_to_char( "No lions come to your rescue.\n\r", ch );
    return;
  }

  if ( ch->mana < 125 )
  {
     send_to_char( "You don't have enough mana to shout a lion call.\n\r", ch );
     return;
  }
  ch->mana -= 125;

  bear = create_mobile( get_mob_index(MOB_VNUM_LION) );

  for (i=0;i < MAX_STATS; i++)
    {
      bear->perm_stat[i] = UMIN(25,2 * ch->perm_stat[i]);
    }

  bear->max_hit = IS_NPC(ch)? ch->max_hit : ch->pcdata->perm_hit;
  bear->hit = bear->max_hit;
  bear->max_mana = IS_NPC(ch)? ch->max_mana : ch->pcdata->perm_mana;
  bear->mana = bear->max_mana;
  bear->alignment = ch->alignment;
  bear->level = UMIN(100,1 * ch->level-2);
  for (i=0; i < 3; i++)
    bear->armor[i] = interpolate(bear->level,100,-100);
  bear->armor[3] = interpolate(bear->level,100,0);
  bear->sex = ch->sex;
  bear->gold = 0;
    
  bear2 = create_mobile(bear->pIndexData);
  clone_mobile(bear,bear2);
  
  SET_BIT(bear->affected_by, AFF_CHARM);
  SET_BIT(bear2->affected_by, AFF_CHARM);
  bear->master = bear2->master = ch;
  bear->leader = bear2->leader = ch;

  char_to_room(bear,ch->in_room);
  char_to_room(bear2,ch->in_room);
  send_to_char("Two lions come to your rescue!\n\r",ch);
  act("Two bears come to $n's rescue!",ch,NULL,NULL,TO_ROOM);

  af.where		= TO_AFFECTS;
  af.type               = gsn_lion_call;
  af.level              = ch->level; 
  af.duration           = 24;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);  

}

/* object condition aliases */
char *get_cond_alias( OBJ_DATA *obj)
{
 char *stat;
 int istat;

 istat = obj->condition;

 if      ( istat >  99 ) stat = "excellent";
 else if ( istat >= 80 ) stat = "good";
 else if ( istat >= 60 ) stat = "fine";
 else if ( istat >= 40 ) stat = "average";
 else if ( istat >= 20 ) stat = "poor";
 else                    stat = "fragile";

 return stat;
}

/* room affects */
void do_raffects(CHAR_DATA *ch, char *argument )
{
    AFFECT_DATA *paf, *paf_last = NULL;
    char buf[MAX_STRING_LENGTH];

    if (ch->in_room->affected != NULL )
    {
	send_to_char( "The room is affected by the following spells:\n\r", ch );
	for ( paf = ch->in_room->affected; paf != NULL; paf = paf->next )
	{
	    if (paf_last != NULL && paf->type == paf_last->type)
		if (ch->level >= 20 )
		    sprintf( buf, "                      ");
		else
		    continue;
	    else
	    	sprintf( buf, "Spell: %-15s", skill_table[paf->type].name );

	    send_to_char( buf, ch );

	    if ( ch->level >= 20 )
	    {
		sprintf( buf,
		    ": modifies %s by %d ",
		    raffect_loc_name( paf->location ),
		    paf->modifier);
		send_to_char( buf, ch );
		if ( paf->duration == -1 || paf->duration == -2 )
		    sprintf( buf, "permanently" );
		else
		    sprintf( buf, "for %d hours", paf->duration );
		send_to_char( buf, ch );
	    }
	    send_to_char( "\n\r", ch );
	    paf_last = paf;
	}
    }
    else 
	send_to_char("The room is not affected by any spells.\n\r",ch);

    return;
}

/* new practice */
void do_pracnew( CHAR_DATA *ch, char *argument )
{
    char buf2[10*MAX_STRING_LENGTH];
    char buf[MAX_STRING_LENGTH];
    int sn;

    if ( IS_NPC(ch) )
	return;

    if ( argument[0] == '\0' )
    {
	int col;

	col    = 0;
        strcpy( buf2, "" );
	for ( sn = 0; sn < MAX_SKILL; sn++ )
	{
	    if ( skill_table[sn].name == NULL )
		break;
	    if ( ch->level < skill_table[sn].skill_level[ch->iclass] ||
	!RACE_OK(ch,sn) ||
(skill_table[sn].cabal != ch->cabal && skill_table[sn].cabal != CABAL_NONE) 
	      )
		continue;

	    sprintf( buf, "%-18s %3d%%  ",
		skill_table[sn].name, ch->pcdata->learned[sn] );
	    strcat( buf2, buf );
	    if ( ++col % 3 == 0 )
		strcat( buf2, "\n\r" );
	}

	if ( col % 3 != 0 )
	    strcat( buf2, "\n\r" );

	sprintf( buf, "You have %d practice sessions left.\n\r",
	    ch->practice );  
	strcat( buf2, buf );
	
        page_to_char( buf2, ch );
    }
    else
    {
	CHAR_DATA *mob;
	int adept;

	if ( !IS_AWAKE(ch) )
	{
	    send_to_char( "In your dreams, or what?\n\r", ch );
	    return;
	}

	if ( ch->practice <= 0 )
	{
	    send_to_char( "You have no practice sessions left.\n\r", ch );
	    return;
	}

	if ( ( sn = find_spell( ch,argument ) ) < 0
	|| ( !IS_NPC(ch)
	&&   (ch->level < skill_table[sn].skill_level[ch->iclass] 
 	|| !RACE_OK(ch,sn) ||
(skill_table[sn].cabal != ch->cabal && skill_table[sn].cabal != CABAL_NONE) )))
	{
	    send_to_char( "You can't practice that.\n\r", ch );
	    return;
	}

	if (!str_cmp("vampire",skill_table[sn].name) )
	{
	 send_to_char( "You can't practice that, only available at questor.\n\r",ch);
	 return;
	}

	for ( mob = ch->in_room->people; mob != NULL; mob = mob->next_in_room )
	{
	    if ( IS_NPC(mob) && IS_SET(mob->act, ACT_PRACTICE) )
	      {
	        if (skill_table[sn].cabal == CABAL_NONE)
		 {
		  if ( ( mob->pIndexData->practicer == 0 &&
			( skill_table[sn].group == GROUP_NONE 
			  || skill_table[sn].group == GROUP_CREATION
			  || skill_table[sn].group == GROUP_HARMFUL
			  || skill_table[sn].group == GROUP_PROTECTIVE
			  || skill_table[sn].group == GROUP_DETECTION
			  || skill_table[sn].group == GROUP_WEATHER ))
		      || (mob->pIndexData->practicer & 
			 (1 << prac_table[skill_table[sn].group].number) ) )
		  break;
		 }
		else
		 {
		  if (ch->cabal == mob->cabal)	break;
	     	 }
	     }
	}

	if ( mob == NULL )
	{
	    send_to_char( "You can't do that here. USE glist ,slook for more info.\n\r", ch );
	    return;
	}

	adept = IS_NPC(ch) ? 100 : class_table[ch->iclass].skill_adept;

	if ( ch->pcdata->learned[sn] >= adept )
	{
	    sprintf( buf, "You are already learned at %s.\n\r",
		skill_table[sn].name );
	    send_to_char( buf, ch );
	}
	else
	{
	    if (!ch->pcdata->learned[sn]) ch->pcdata->learned[sn] = 1;
	    ch->practice--;
	    ch->pcdata->learned[sn] += 
		int_app[get_curr_stat(ch,STAT_INT)].learn / 
	        UMAX(skill_table[sn].rating[ch->iclass],1);
	    if ( ch->pcdata->learned[sn] < adept )
	    {
		act( "You practice $T.",
		    ch, NULL, skill_table[sn].name, TO_CHAR );
		act( "$n practices $T.",
		    ch, NULL, skill_table[sn].name, TO_ROOM );
	    }
	    else
	    {
		ch->pcdata->learned[sn] = adept;
		act( "You are now learned at $T.",
		    ch, NULL, skill_table[sn].name, TO_CHAR );
		act( "$n is now learned at $T.",
		    ch, NULL, skill_table[sn].name, TO_ROOM );
	    }
	}
    }
    return;
}


/*
 * New 'who_col' command by chronos
 */
void do_who_col( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char cabalbuf[MAX_STRING_LENGTH];
    char titlebuf[MAX_STRING_LENGTH];
    char classbuf[100];
    char output[4 * MAX_STRING_LENGTH];
    char pk_buf[100];
    char act_buf[100];
    char level_buf[100];
    DESCRIPTOR_DATA *d;
    int iClass;
    int iRace;
    int iLevelLower;
    int iLevelUpper;
    int nNumber;
    int nMatch;
    int vnum;
    int count;
    bool rgfClass[MAX_CLASS];
    bool rgfRace[MAX_PC_RACE];
    bool fClassRestrict;
    bool fRaceRestrict;
    bool fImmortalOnly;
    bool fPKRestrict;
    bool fRulerRestrict;
    bool fChaosRestrict;
    bool fShalafiRestrict;
    bool fInvaderRestrict;
    bool fBattleRestrict;
    bool fKnightRestrict;
    bool fLionsRestrict;
    bool fTattoo;

    /*
     * Set default arguments.
     */
    iLevelLower    = 0;
    iLevelUpper    = MAX_LEVEL;
    fClassRestrict = FALSE;
    fRaceRestrict = FALSE;
    fPKRestrict = FALSE;
    fImmortalOnly  = FALSE;
    fBattleRestrict = FALSE;
    fChaosRestrict = FALSE;
    fRulerRestrict = FALSE;
    fInvaderRestrict = FALSE;
    fShalafiRestrict = FALSE;
    fKnightRestrict = FALSE;
    fLionsRestrict = FALSE;
    vnum = 0;
    fTattoo = FALSE;
    for ( iClass = 0; iClass < MAX_CLASS; iClass++ )
	rgfClass[iClass] = FALSE;
    for ( iRace = 0; iRace < MAX_PC_RACE; iRace++ )
	rgfRace[iRace] = FALSE;


    /*
     * Parse arguments.
     */
    nNumber = 0;
    for ( ;; )
    {
	char arg[MAX_STRING_LENGTH];

	argument = one_argument( argument, arg );
	if ( arg[0] == '\0' )
	    break;

	if (!str_cmp(arg,"pk"))
	  {
	    fPKRestrict = TRUE;
	    break;
	  }

	if (!str_cmp(arg,"ruler"))
	  {
	    if (ch->cabal != CABAL_RULER && !IS_IMMORTAL(ch))
	      {
		send_to_char("You are not in that cabal!\n\r",ch);
		return;
	      }
	    else
	      {
		fRulerRestrict = TRUE;
		break;
	      }
	  }
	if (!str_cmp(arg,"shalafi"))
	  {
	    if (ch->cabal != CABAL_SHALAFI && !IS_IMMORTAL(ch))
	      {
		send_to_char("You are not in that cabal!\n\r",ch);
		return;
	      }
	    else
	      {
		fShalafiRestrict = TRUE;
		break;
	      }
	  }
	if (!str_cmp(arg,"battle"))
	  {
	    if (ch->cabal != CABAL_BATTLE && !IS_IMMORTAL(ch))
	      {
		send_to_char("You are not in that cabal!\n\r",ch);
		return;
	      }
	    else
	      {
		fBattleRestrict = TRUE;
		break;
	      }
	  }
	if (!str_cmp(arg,"invader"))
	  {
	    if (ch->cabal != CABAL_INVADER && !IS_IMMORTAL(ch))
	      {
		send_to_char("You are not in that cabal!\n\r",ch);
		return;
	      }
	    else
	      {
		fInvaderRestrict = TRUE;
		break;
	      }
	  }
	if (!str_cmp(arg,"chaos"))
	  {
	    if (ch->cabal != CABAL_CHAOS && !IS_IMMORTAL(ch))
	      {
		send_to_char("You are not in that cabal!\n\r",ch);
		return;
	      }
	    else
	      {
		fChaosRestrict = TRUE;
		break;
	      }
	  }
	if (!str_cmp(arg,"knight"))
	  {
	    if (ch->cabal != CABAL_KNIGHT && !IS_IMMORTAL(ch))
	      {
		send_to_char("You are not in that cabal!\n\r",ch);
		return;
	      }
	    else
	      {
		fKnightRestrict = TRUE;
		break;
	      }
	  }
	if (!str_cmp(arg,"lions"))
	  {
	    if (ch->cabal != CABAL_LIONS && !IS_IMMORTAL(ch))
	      {
		send_to_char("You are not in that cabal!\n\r",ch);
		return;
	      }
	    else
	      {
		fLionsRestrict = TRUE;
		break;
	      }
	  }
	if (!str_cmp(arg,"tattoo"))
	  {
	    if (get_eq_char(ch,WEAR_TATTOO) == NULL)
	      {
		send_to_char("You haven't got a tattoo yetl!\n\r",ch);
		return;
	      }
	    else
	      {
		fTattoo = TRUE;
		vnum = get_eq_char(ch,WEAR_TATTOO)->pIndexData->vnum;
		break;
	      }
	  }


	if ( is_number( arg ) && IS_IMMORTAL(ch))
	{
	    switch ( ++nNumber )
	    {
	    case 1: iLevelLower = atoi( arg ); break;
	    case 2: iLevelUpper = atoi( arg ); break; 
	    default:
		send_to_char( "This function of who is for immortals.\n\r",ch);
		return;
	    }
	}
	else
	{

	    /*
	     * Look for classes to turn on.
	     */
	    if ( arg[0] == 'i' )
	    {
		fImmortalOnly = TRUE;
	    }
	    else
	    {
	      iClass = class_lookup(arg);
	      if (iClass == -1 || !IS_IMMORTAL(ch))
	      	{
	          iRace = race_lookup(arg);

	          if (iRace == 0 || iRace >= MAX_PC_RACE)
		    {
		      send_to_char("That's not a valid race.\n\r",ch);
		      return;
		    }
	          else
		    {
			fRaceRestrict = TRUE;
			rgfRace[iRace] = TRUE;
		    }
		}
	      else
		{
		  fClassRestrict = TRUE;
		  rgfClass[iClass] = TRUE;
		}
	    }
	  }
      }

    /*
     * Now show matching chars.
     */
    nMatch = 0;
    buf[0] = '\0';
    output[0] = '\0';
    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	CHAR_DATA *wch;
	char const *iclass;

	/*
	 * Check for match against restrictions.
	 * Don't use trust as that exposes trusted mortals.
	 */
	if ( d->connected != CON_PLAYING || !can_see( ch, d->character ) )
	    continue;

	if ( d->connected != CON_PLAYING || 
(IS_VAMPIRE( d->character ) && !IS_IMMORTAL(ch) && (ch != d->character) ) )
	    continue;

	wch   = ( d->original != NULL ) ? d->original : d->character;
	if (!can_see(ch, wch)) /* can't see switched wizi imms */
	  continue;

	if ( wch->level < iLevelLower
	||   wch->level > iLevelUpper
	|| ( fImmortalOnly  && wch->level < LEVEL_HERO )
	|| ( fClassRestrict && !rgfClass[wch->iclass])
	|| ( fRaceRestrict && !rgfRace[RACE(wch)])
        || ( fPKRestrict && is_safe_nomessage(ch,wch) )
	|| ( fTattoo &&(vnum == get_eq_char(wch,WEAR_TATTOO)->pIndexData->vnum))
	    || (fRulerRestrict && wch->cabal != CABAL_RULER )
	    || (fChaosRestrict && wch->cabal != CABAL_CHAOS)
	    || (fBattleRestrict && wch->cabal != CABAL_BATTLE)
	    || (fInvaderRestrict && wch->cabal != CABAL_INVADER)
	    || (fShalafiRestrict && wch->cabal != CABAL_SHALAFI)
	    || (fKnightRestrict && wch->cabal != CABAL_KNIGHT)
	    || (fLionsRestrict && wch->cabal != CABAL_LIONS))
	    continue;

	nMatch++;

	/*
	 * Figure out what to print for class.
	 */
	iclass = class_table[wch->iclass].who_name;
	switch ( wch->level )
	{
	default: break;
            {
                case MAX_LEVEL - 0 : iclass = "IMP";     break;
                case MAX_LEVEL - 1 : iclass = "CRE";     break;
                case MAX_LEVEL - 2 : iclass = "SUP";     break;
                case MAX_LEVEL - 3 : iclass = "DEI";     break;
                case MAX_LEVEL - 4 : iclass = "GOD";     break;
                case MAX_LEVEL - 5 : iclass = "IMM";     break;
                case MAX_LEVEL - 6 : iclass = "DEM";     break;
                case MAX_LEVEL - 7 : iclass = "ANG";     break;
                case MAX_LEVEL - 8 : iclass = "AVA";     break;
            }
	}

	/* for cabals 
	if ((wch->cabal && (wch->cabal == ch->cabal || 
			   IS_TRUSTED(ch,LEVEL_IMMORTAL))) ||
                           wch->level >= LEVEL_HERO)
	*/
	if ( (wch->cabal && ch->cabal ==  wch->cabal) || IS_IMMORTAL(ch) 
		|| ( IS_SET(wch->act , PLR_CANINDUCT) && wch->cabal == 1) 
		|| wch->cabal == CABAL_HUNTER
		|| (wch->cabal == CABAL_RULER
		   && is_equiped_n_char(wch, OBJ_VNUM_RULER_BADGE, WEAR_NECK)) )
	{
	  sprintf(cabalbuf, "[%s%s%s] ",
		CLR_CYAN,
		cabal_table[wch->cabal].short_name,
		CLR_WHITE_BOLD	);
	}
	else cabalbuf[0] = '\0';
	if (wch->cabal == 0) cabalbuf[0] = '\0';

	pk_buf[0] = '\0';
	if (!((ch==wch && ch->level<PK_MIN_LEVEL) || is_safe_nomessage(ch,wch)))
	  sprintf(pk_buf,"%s(PK) %s", CLR_RED , CLR_WHITE_BOLD );

	act_buf[0] = '\0';
	sprintf(act_buf,"%s%s%s", CLR_WHITE ,
		    IS_SET(wch->act, PLR_WANTED) ? "(WANTED) " : "",
	        CLR_WHITE_BOLD );

	if (IS_NPC(wch))
		sprintf(titlebuf,"Believer of Chronos.");
	else {
		sprintf(titlebuf,"%s", wch->pcdata->title );
		if (strlen(titlebuf) > 45 )
		 {
			free_string(wch->pcdata->title);
			titlebuf[44] = '\0';
			wch->pcdata->title = str_dup( titlebuf );
			bug("Title length more than 45",0);
		 }
	     }
	/*
	 * Format it up.
	 */

	sprintf( level_buf, "%s%2d%s", CLR_CYAN,wch->level, CLR_WHITE_BOLD);
	sprintf(classbuf,"%s%s%s",CLR_YELLOW,iclass, CLR_WHITE_BOLD);

	if (IS_TRUSTED(ch,LEVEL_IMMORTAL) || ch==wch ||
                   wch->level >= LEVEL_HERO)

	  sprintf( buf, "[%2d %s %s] %s%s%s%s%s\n\r",
	    wch->level,
	    RACE(wch) < MAX_PC_RACE ? pc_race_table[RACE(wch)].who_name 
				    : "     ",
	    classbuf,
	    pk_buf,
	    cabalbuf,
	    act_buf,
	    wch->name,
	    titlebuf);

	else
/*	  sprintf( buf, "[%s %s %s] %s%s%s%s%s\n\r",	*/
	  sprintf( buf, "[%s %s    ] %s%s%s%s%s\n\r",
		(get_curr_stat(wch, STAT_CHA) < 18 ) ? level_buf : "  ",
	    RACE(wch) < MAX_PC_RACE ? pc_race_table[RACE(wch)].who_name 
				    : "     ",
/*	    classbuf, 	*/
	    pk_buf,
	    cabalbuf,
	    act_buf,
	    wch->name,
	    titlebuf); 

	strcat(output,buf);
    }

    count = 0;
    for ( d = descriptor_list; d != NULL; d = d->next )
        if ( d->connected == CON_PLAYING )    count++;

    max_on = UMAX(count,max_on);
    sprintf( buf2, "\n\rPlayers found: %d. Most so far today: %d.\n\r",
		nMatch,max_on );
    strcat(output,buf2);
    page_to_char( output, ch );
    return;
}


void do_camp( CHAR_DATA *ch, char *argument )
{
  AFFECT_DATA af,af2;
  
  if (IS_NPC(ch) ||
      ch->level < skill_table[gsn_camp].skill_level[ch->iclass] )
    {
      send_to_char( "Huh?\n\r", ch );
      return;
    }

  if (is_affected(ch, gsn_camp))
    {
      send_to_char("You don't have enough power to handle more camp areas.\n\r", ch);
      return;
    }


  if ( number_percent( ) > get_skill( ch, gsn_camp) )
  {
	send_to_char( "You failed to make your camp.\n\r", ch );
	check_improve(ch,gsn_camp,TRUE,4);
	return;
  }
  
  if ( IS_SET(ch->in_room->room_flags, ROOM_SAFE)      ||
       IS_SET(ch->in_room->room_flags, ROOM_PRIVATE)   ||
       IS_SET(ch->in_room->room_flags, ROOM_SOLITARY)  ||
         ( ch->in_room->sector_type != SECT_FIELD &&
           ch->in_room->sector_type != SECT_FOREST &&
           ch->in_room->sector_type != SECT_MOUNTAIN &&
           ch->in_room->sector_type != SECT_HILLS ) )
  {
    send_to_char( "There are not enough leaves to camp here.\n\r", ch );
    return;
  }

  if ( ch->mana < 150 )
  {
     send_to_char( "You don't have enough mana to make a camp.\n\r", ch );
     return;
  }
 
  check_improve(ch,gsn_camp,TRUE,4);
  ch->mana -= 150;

  WAIT_STATE(ch,skill_table[gsn_camp].beats);

  send_to_char("You succeeded to make your camp.\n\r",ch);
  act("$n succeeded to make $s camp.",ch,NULL,NULL,TO_ROOM);

  af.where		= TO_AFFECTS;
  af.type               = gsn_camp;
  af.level              = ch->level; 
  af.duration           = 12;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);  

  af2.where		= TO_ROOM_CONST;
  af2.type               = gsn_camp;
  af2.level              = ch->level; 
  af2.duration           = ch->level / 20;
  af2.bitvector          = 0;
  af2.modifier           = 2 * ch->level;
  af2.location           = APPLY_ROOM_HEAL;
  affect_to_room(ch->in_room, &af2);  

  af2.modifier           = ch->level;
  af2.location           = APPLY_ROOM_MANA;
  affect_to_room(ch->in_room, &af2);  

}


void do_demand( CHAR_DATA *ch, char *argument)
{
  char arg1 [MAX_INPUT_LENGTH];
  char arg2 [MAX_INPUT_LENGTH];
  CHAR_DATA *victim;
  OBJ_DATA  *obj;
  int chance;

  argument = one_argument( argument, arg1 );
  argument = one_argument( argument, arg2 );
 
  if (IS_NPC(ch))
	return;
 
  if (ch->iclass != CLASS_ANTI_PALADIN)
    {
      send_to_char("You can't do that.\n\r", ch);
      return;
    }

  if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
      send_to_char( "Demand what from whom?\n\r", ch );
      return;
    }
  
  if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
    {
      send_to_char( "They aren't here.\n\r", ch );
      return;
    }
  
  if (!IS_NPC(victim))
    {
      send_to_char("Why don't you just want that directly from the player?\n\r", ch);
      return;
    }

  WAIT_STATE(ch, PULSE_VIOLENCE);

  chance = IS_EVIL(victim) ? 10 : IS_GOOD(victim) ? -5 : 0;
  chance += (get_curr_stat(ch,STAT_CHA) - 15) * 10;
  chance += ch->level - victim->level;

  if (victim->level >= ch->level + 10 || victim->level >= ch->level * 2)
	chance = 0;

  if ( number_percent() > chance )
	{
	 do_say(victim,"I'm not about to give you anything!");
         do_murder(victim, ch->name);
	 return;
	}

  if (( ( obj = get_obj_carry(victim , arg1 ) ) == NULL 
      && (obj = get_obj_wear(victim, arg1)) == NULL)
      || IS_SET(obj->extra_flags, ITEM_INVENTORY))
    {
      do_say(victim, "Sorry, I don't have that.");
      return;
    }
  
  
  if ( obj->wear_loc != WEAR_NONE )
    unequip_char(victim, obj);
  
  if ( !can_drop_obj( ch, obj ) )
    {
      do_say(victim, 
	"It's cursed so, I can't let go of it. Forgive me, my master");
      return;
    }
  
  if ( ch->carry_number + get_obj_number( obj ) > can_carry_n( ch ) )
    {
      send_to_char( "Your hands are full.\n\r", ch);
      return;
    }
  
  if ( ch->carry_weight + get_obj_weight( obj ) > can_carry_w( ch ) )
    {
      send_to_char( "You can't carry that much weight.\n\r", ch);
      return;
    }
  
  if ( !can_see_obj( ch, obj ) )
    {
      act( "You don't see that.", ch, NULL, victim, TO_CHAR );
      return;
    }

  obj_from_char( obj );
  obj_to_char( obj, ch );
  act( "$n demands $p from $N.", ch, obj, victim, TO_NOTVICT );
  act( "You demand $p from $N.",   ch, obj, victim, TO_CHAR    );
  act( "$n demands $p from you.", ch, obj, victim, TO_VICT    );



  if (IS_SET(obj->progtypes,OPROG_GIVE))
    (obj->pIndexData->oprogs->give_prog) (obj,ch,victim);

  if (IS_SET(victim->progtypes,MPROG_GIVE))
    (victim->pIndexData->mprogs->give_prog) (victim,ch,obj);

  send_to_char("Your power makes all around the world shivering.\n\r",ch);
  
  return;
}
  


void do_control( CHAR_DATA *ch, char *argument)
{
  char arg[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;
  int chance;

  argument = one_argument( argument, arg );
 
  if (IS_NPC(ch) ||
      ch->level < skill_table[gsn_control_animal].skill_level[ch->iclass] )
    {
      send_to_char( "Huh?\n\r", ch );
      return;
    }
 
  if ( arg[0] == '\0' )
    {
      send_to_char( "Charm what?\n\r", ch );
      return;
    }
  
  if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
      send_to_char( "They aren't here.\n\r", ch );
      return;
    }
  
  if (race_table[ORG_RACE(victim)].pc_race)
    {
      send_to_char("You should try this on monsters?\n\r", ch);
      return;
    }

  if (is_safe(ch,victim)) return;

  if (count_charmed(ch))  return;

  WAIT_STATE(ch, PULSE_VIOLENCE);

  chance = get_skill(ch,gsn_control_animal);

  chance += (get_curr_stat(ch,STAT_CHA) - 20) * 5;
  chance += (ch->level - victim->level) * 3;
  chance +=
   (get_curr_stat(ch,STAT_INT) - get_curr_stat(victim,STAT_INT)) * 5;

 if ( IS_AFFECTED(victim, AFF_CHARM)
    ||   IS_AFFECTED(ch, AFF_CHARM)
    ||	 number_percent() > chance
    ||   ch->level < ( victim->level + 2 ) 
    ||   IS_SET(victim->imm_flags,IMM_CHARM)
    ||   (IS_NPC(victim) && victim->pIndexData->pShop != NULL) )
	{
	 check_improve(ch,gsn_control_animal,FALSE,2);
	 do_say(victim,"I'm not about to follow you!");
         do_murder(victim, ch->name);
	 return;
	}

  check_improve(ch,gsn_control_animal,TRUE,2);

  if ( victim->master )
	stop_follower( victim );
  SET_BIT(victim->affected_by,AFF_CHARM);
  victim->master = victim->leader = ch;

  act( "Isn't $n just so nice?", ch, NULL, victim, TO_VICT );
  if ( ch != victim )
	act("$N looks at you with adoring eyes.",ch,NULL,victim,TO_CHAR);

  return;
}
  



void do_nscore( CHAR_DATA *ch, char *argument )
{
   char buf[MAX_STRING_LENGTH]; 
   char buf2[MAX_INPUT_LENGTH]; 
   char titlebuf[MAX_INPUT_LENGTH]; 
   int ekle=0;
    
    if (IS_SET(ch->act,PLR_COLOR) )
	{
	 do_nscore_col(ch,argument);
	 return;
	}

    sprintf( buf, "\n\r/~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~/~~\\\n\r");
    send_to_char( buf, ch);
    sprintf(titlebuf,"%s",
		IS_NPC(ch) ? "Believer of Chronos." : ch->pcdata->title);
    titlebuf[32] = '\0';
    sprintf( buf, 
"     |   %-12s%-33s %3d years old   |____|\n\r",
		ch->name,titlebuf,get_age(ch));
    send_to_char( buf, ch);
    sprintf( buf, "     |+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+|\n\r");
    send_to_char( buf, ch);
    sprintf( buf, 
"     | Level:  %3d          |  Str:  %2d(%2d)  | Religion  :  %-10s |\n\r",
		ch->level,ch->perm_stat[STAT_STR],get_curr_stat(ch,STAT_STR),
		religion_table[ch->religion].leader);
    send_to_char( buf, ch); 
    sprintf( buf, 
"     | Race :  %-11s  |  Int:  %2d(%2d)  | Practice  :   %3d       |\n\r",
		race_table[ORG_RACE(ch)].name,
		ch->perm_stat[STAT_INT], get_curr_stat(ch,STAT_INT),
		ch->practice);
    send_to_char(buf, ch);

    sprintf( buf, 
"     | Sex  :  %-11s  |  Wis:  %2d(%2d)  | Train     :   %3d       |\n\r",
	ch->sex == 0 ? "sexless" : ch->sex == 1 ? "male" : "female",
	ch->perm_stat[STAT_WIS], get_curr_stat(ch,STAT_WIS),ch->train);
    send_to_char(buf, ch);

    sprintf( buf, 
"     | Class:  %-12s |  Dex:  %2d(%2d)  | Quest Pnts:  %4d       |\n\r",
		IS_NPC(ch) ? "mobile" : class_table[ch->iclass].name,
		ch->perm_stat[STAT_DEX], get_curr_stat(ch,STAT_DEX),
		IS_NPC(ch) ? 0 : ch->pcdata->questpoints );
    send_to_char(buf, ch);

    sprintf( buf, 
"     | Home :  %-12s |  Con:  %2d(%2d)  | Quest Time:   %3d       |\n\r",
		IS_NPC(ch) ? "Midgaard" : hometown_table[ch->hometown].name,
		ch->perm_stat[STAT_CON], get_curr_stat(ch,STAT_CON),
		IS_NPC(ch) ? 0 : ch->pcdata->nextquest);
    send_to_char(buf, ch);
    sprintf( buf, 
"     | Ethos:  %-11s  |  Cha:  %2d(%2d)  | %s     :   %3d       |\n\r",
		IS_NPC(ch) ? "mobile" : ch->ethos == 1 ? "lawful" : 
	ch->ethos == 2 ? "neutral" : ch->ethos == 3 ? "chaotic" : "none",
		ch->perm_stat[STAT_CHA], get_curr_stat(ch,STAT_CHA),
		ch->iclass == 9 ? "Death" : "Wimpy" ,
		ch->iclass == 9 ? ch->pcdata->death : ch->wimpy);
    send_to_char(buf, ch);
    switch ( ch->position )
    {
    case POS_DEAD:     
	sprintf(buf2,"You are DEAD!!");
	break;
    case POS_MORTAL:
	sprintf(buf2,"You're fatally wounded.");
	break;
    case POS_INCAP:
	sprintf(buf2,"You are incapacitated.");
	break;
    case POS_STUNNED:
	sprintf(buf2,"You are stunned.");
	break;
    case POS_SLEEPING:
	sprintf(buf2,"You are sleeping.");
	break;
    case POS_RESTING:
	sprintf(buf2,"You are resting.");
	break;
    case POS_STANDING:
	sprintf(buf2,"You are standing.");
	break;
    case POS_FIGHTING:
	sprintf(buf2,"You are fighting.");
	break;
    }

    sprintf( buf, 
"     | Align:  %-11s  |                | %-23s |\n\r",		
		IS_GOOD(ch) ? "good" : IS_EVIL(ch) ? "evil" : "neutral",
		buf2);
    send_to_char(buf, ch);

    sprintf( buf, 
"     |+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+|\n\r");
    send_to_char(buf, ch);

    if (ch->guarding != NULL)
	{
	 ekle = 1;
    sprintf( buf, 
"     | You are guarding: %-10s                                    |\n\r",
	ch->guarding->name);
    send_to_char(buf,ch);
	}

    if (ch->guarded_by != NULL)
	{
	 ekle = 1;
    sprintf( buf, 
"     | You are guarded by: %-10s                                  |\n\r",
	ch->guarded_by->name);
    send_to_char(buf,ch);
	}

    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK]   > 10 )
	{
	 ekle = 1;
    sprintf( buf, 
"     | You are drunk.                                                  |\n\r");
    send_to_char(buf,ch);
	}

    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_THIRST] <=  0 )
	{
	 ekle = 1;
    sprintf( buf, 
"     | You are thirsty.                                                |\n\r");
    send_to_char(buf,ch);
	}
/*    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_FULL]   ==  0 ) */
    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_HUNGER]   <=  0 )
	{
	 ekle = 1;
    sprintf( buf, 
"     | You are hungry.                                                 |\n\r");
    send_to_char(buf,ch);
	}

    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_BLOODLUST]   <=  0 )
	{
	 ekle = 1;
    sprintf( buf, 
"     | You are hungry for blood.                                       |\n\r");
    send_to_char(buf,ch);
	}

    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_DESIRE]   <=  0 )
	{
	 ekle = 1;
    sprintf( buf, 
"     | You are desiring your home.                                     |\n\r");
    send_to_char(buf,ch);
	}

    if ( ch->last_fight_time != -1 && !IS_IMMORTAL(ch) &&
        (current_time - ch->last_fight_time)<FIGHT_DELAY_TIME) 
	{
	 ekle = 1;
    sprintf( buf, 
"     | Your adrenalin is gushing!                                      |\n\r");
    send_to_char(buf,ch);
	}

    if (ekle)
	{
    sprintf( buf, 
"     |+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+|\n\r");
    send_to_char(buf, ch);
	}

    sprintf( buf, 
"     | Items Carried :     %3d/%-4d        Armor vs magic  : %4d      |\n\r", 
		ch->carry_number, can_carry_n(ch),
		GET_AC(ch,AC_EXOTIC));
    send_to_char(buf, ch);

    sprintf( buf, 
"     | Weight Carried:  %6ld/%-8d    Armor vs bash   : %4d      |\n\r",
	get_carry_weight(ch), can_carry_w(ch),GET_AC(ch,AC_BASH));
    send_to_char(buf, ch);

    sprintf( buf, 
"     | Gold          :   %-10ld        Armor vs pierce : %4d      |\n\r",
		 ch->gold,GET_AC(ch,AC_PIERCE));
    send_to_char(buf, ch);

    sprintf( buf, 
"     | Silver        :   %-10ld        Armor vs slash  : %4d      |\n\r",
		 ch->silver,GET_AC(ch,AC_SLASH));
    send_to_char(buf, ch);

    sprintf( buf, 
"     | Current exp   :   %-6d            Saves vs Spell  : %4d      |\n\r",
		ch->exp,ch->saving_throw);
    send_to_char(buf, ch);

    sprintf( buf, 
"     | Exp to level  :   %-6d                                        |\n\r",
		IS_NPC(ch) ? 0 : exp_to_level(ch,ch->pcdata->points));
    send_to_char(buf, ch);

    sprintf( buf, 
"     |                                     HitP: %5d / %5d         |\n\r",
	ch->hit, ch->max_hit);
    send_to_char(buf, ch);
    sprintf( buf, 
"     | Hitroll       :   %-3d               Mana: %5d / %5d         |\n\r",
	GET_HITROLL(ch),ch->mana, ch->max_mana);
    send_to_char(buf, ch);
    sprintf( buf, 
"     | Damroll       :   %-3d               Move: %5d / %5d         |\n\r",
	GET_DAMROLL(ch), ch->move, ch->max_move);
    send_to_char(buf, ch);
    sprintf( buf, "  /~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~/   |\n\r");
    send_to_char(buf, ch);
    sprintf( buf, "  \\________________________________________________________________\\__/\n\r");
    send_to_char(buf, ch);
    if ( ch->affected != NULL && IS_SET(ch->comm, COMM_SHOW_AFFECTS) )
	do_affects(ch, "");
   return;
}



void do_nscore_col( CHAR_DATA *ch, char *argument )
{
   char buf[MAX_STRING_LENGTH]; 
   char buf2[MAX_INPUT_LENGTH]; 
   char titlebuf[MAX_INPUT_LENGTH]; 
   int ekle=0;
    
    sprintf( buf, "%s\n\r/~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~/~~\\\n\r"
	,CLR_GREEN_BOLD);
    send_to_char( buf, ch);
    sprintf(titlebuf,"%s",
		IS_NPC(ch) ? "Believer of Chronos." : ch->pcdata->title);
    titlebuf[32] = '\0';
    sprintf( buf, 
"     %s|   %s%-12s%s%-33s %s%3d%s years old   %s|%s____|%s\n\r",
		CLR_GREEN_BOLD,CLR_RED_BOLD,
		ch->name,CLR_WHITE_BOLD,
		titlebuf,CLR_BROWN,get_age(ch),CLR_WHITE_BOLD,
		CLR_GREEN_BOLD,CLR_GREEN,CLR_GREEN_BOLD);
    send_to_char( buf, ch);
    sprintf( buf, 
"     |%s+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+%s|\n\r",
	CLR_CYAN_BOLD,CLR_GREEN_BOLD);
    send_to_char( buf, ch);
    sprintf( buf, 
"     | %sLevel:%s  %3d          %s|  %sStr:%s  %2d(%2d)  %s| %sReligion  :%s  %-10s %s|\n\r",
		CLR_RED_BOLD,CLR_WHITE_BOLD,
		ch->level,CLR_CYAN_BOLD,CLR_RED_BOLD,CLR_WHITE_BOLD,
		ch->perm_stat[STAT_STR],get_curr_stat(ch,STAT_STR),
		CLR_CYAN_BOLD,CLR_RED_BOLD,CLR_WHITE_BOLD,
		religion_table[ch->religion].leader,CLR_GREEN_BOLD);
    send_to_char( buf, ch); 
    sprintf( buf, 
"     | %sRace :%s  %-11s  %s|  %sInt:%s  %2d(%2d)  %s| %sPractice  :%s   %3d       %s|\n\r",
		CLR_RED_BOLD,CLR_WHITE_BOLD,
		race_table[ORG_RACE(ch)].name,CLR_CYAN_BOLD,
		CLR_RED_BOLD,CLR_WHITE_BOLD,
		ch->perm_stat[STAT_INT], get_curr_stat(ch,STAT_INT),
		CLR_CYAN_BOLD,CLR_RED_BOLD,CLR_WHITE_BOLD,ch->practice,
		CLR_GREEN_BOLD);
    send_to_char(buf, ch);

    sprintf( buf, 
"     | %sSex  :%s  %-11s  %s|  %sWis:%s  %2d(%2d)  %s| %sTrain     :%s   %3d       %s|\n\r",
	CLR_RED_BOLD,CLR_WHITE_BOLD,
	ch->sex == 0 ? "sexless" : ch->sex == 1 ? "male" : "female",
	CLR_CYAN_BOLD,CLR_RED_BOLD,CLR_WHITE_BOLD,
	ch->perm_stat[STAT_WIS], get_curr_stat(ch,STAT_WIS),
	CLR_CYAN_BOLD,CLR_RED_BOLD,CLR_WHITE_BOLD,ch->train,CLR_GREEN_BOLD);
    send_to_char(buf, ch);

    sprintf( buf, 
"     | %sClass:%s  %-12s %s|  %sDex:%s  %2d(%2d)  %s| %sQuest Pnts:%s  %4d       %s|\n\r",
		CLR_RED_BOLD,CLR_WHITE_BOLD,
		IS_NPC(ch) ? "mobile" : class_table[ch->iclass].name,
		CLR_CYAN_BOLD,CLR_RED_BOLD,CLR_WHITE_BOLD,
		ch->perm_stat[STAT_DEX], get_curr_stat(ch,STAT_DEX),
		CLR_CYAN_BOLD,CLR_RED_BOLD,CLR_WHITE_BOLD,
		IS_NPC(ch) ? 0 : ch->pcdata->questpoints,CLR_GREEN_BOLD );
    send_to_char(buf, ch);

    sprintf( buf, 
"     | %sHome :%s  %-12s %s|  %sCon:%s  %2d(%2d)  %s| %sQuest Time:%s   %3d       %s|\n\r",
		CLR_RED_BOLD,CLR_WHITE_BOLD,
		IS_NPC(ch) ? "Midgaard" : hometown_table[ch->hometown].name,
		CLR_CYAN_BOLD,CLR_RED_BOLD,CLR_WHITE_BOLD,
		ch->perm_stat[STAT_CON], get_curr_stat(ch,STAT_CON),
		CLR_CYAN_BOLD,CLR_RED_BOLD,CLR_WHITE_BOLD,
		IS_NPC(ch) ? 0 : ch->pcdata->nextquest,CLR_GREEN_BOLD);
    send_to_char(buf, ch);
    sprintf( buf, 
"     | %sEthos:%s  %-11s  %s|  %sCha:%s  %2d(%2d)  %s| %s%s     :%s   %3d       %s|\n\r",
		CLR_RED_BOLD,CLR_WHITE_BOLD,
		IS_NPC(ch) ? "mobile" : ch->ethos == 1 ? "lawful" : 
	ch->ethos == 2 ? "neutral" : ch->ethos == 3 ? "chaotic" : "none",
		CLR_CYAN_BOLD,CLR_RED_BOLD,CLR_WHITE_BOLD,
		ch->perm_stat[STAT_CHA], get_curr_stat(ch,STAT_CHA),
		CLR_CYAN_BOLD,CLR_RED_BOLD,
		ch->iclass == 9 ? "Death" : "Wimpy" ,CLR_WHITE_BOLD,
		ch->iclass == 9 ? ch->pcdata->death : ch->wimpy,CLR_GREEN_BOLD);
    send_to_char(buf, ch);
    switch ( ch->position )
    {
    case POS_DEAD:     
	sprintf(buf2,"You are DEAD!!");
	break;
    case POS_MORTAL:
	sprintf(buf2,"You're fatally wounded.");
	break;
    case POS_INCAP:
	sprintf(buf2,"You are incapacitated.");
	break;
    case POS_STUNNED:
	sprintf(buf2,"You are stunned.");
	break;
    case POS_SLEEPING:
	sprintf(buf2,"You are sleeping.");
	break;
    case POS_RESTING:
	sprintf(buf2,"You are resting.");
	break;
    case POS_STANDING:
	sprintf(buf2,"You are standing.");
	break;
    case POS_FIGHTING:
	sprintf(buf2,"You are fighting.");
	break;
    }

    sprintf( buf, 
"     | %sAlign:%s  %-11s  %s|                | %s%-23s %s|\n\r",		
		CLR_RED_BOLD,CLR_WHITE_BOLD,
		IS_GOOD(ch) ? "good" : IS_EVIL(ch) ? "evil" : "neutral",
		CLR_CYAN_BOLD,CLR_BROWN,buf2,CLR_GREEN_BOLD);
    send_to_char(buf, ch);

    sprintf( buf, 
"     |%s+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+%s|\n\r",
	CLR_CYAN_BOLD,CLR_GREEN_BOLD);
    send_to_char(buf, ch);

    if (ch->guarding != NULL)
	{
	 ekle = 1;
    sprintf( buf, 
"     | %sYou are guarding:%s %-10s                                    %s|\n\r",
	CLR_WHITE,CLR_WHITE_BOLD,ch->guarding->name,CLR_GREEN_BOLD);
    send_to_char(buf,ch);
	}

    if (ch->guarded_by != NULL)
	{
	 ekle = 1;
    sprintf( buf, 
"     | %sYou are guarded by:%s %-10s                                  %s|\n\r",
	CLR_WHITE,CLR_WHITE_BOLD,ch->guarded_by->name,CLR_GREEN_BOLD);
    send_to_char(buf,ch);
	}

    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK]   > 10 )
	{
	 ekle = 1;
    sprintf( buf, 
"     | %sYou are drunk.                                                  %s|\n\r",
	CLR_WHITE,CLR_GREEN_BOLD);
    send_to_char(buf,ch);
	}

    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_THIRST] <=  0 )
	{
	 ekle = 1;
    sprintf( buf, 
"     | %sYou are thirsty.                                                %s|\n\r",
	CLR_WHITE,CLR_GREEN_BOLD);
    send_to_char(buf,ch);
	}
/*    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_FULL]   ==  0 ) */
    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_HUNGER]   <=  0 )
	{
	 ekle = 1;
    sprintf( buf, 
"     | %sYou are hungry.                                                 %s|\n\r",
	CLR_WHITE,CLR_GREEN_BOLD);
    send_to_char(buf,ch);
	}

    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_BLOODLUST]   <=  0 )
	{
	 ekle = 1;
    sprintf( buf, 
"     | %sYou are hungry for blood.                                       %s|\n\r",
	CLR_WHITE,CLR_GREEN_BOLD);
    send_to_char(buf,ch);
	}

    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_DESIRE]   <=  0 )
	{
	 ekle = 1;
    sprintf( buf, 
"     | %sYou are desiring your home.                                     %s|\n\r",
	CLR_WHITE,CLR_GREEN_BOLD);
    send_to_char(buf,ch);
	}

    if ( ch->last_fight_time != -1 && !IS_IMMORTAL(ch) &&
        (current_time - ch->last_fight_time)<FIGHT_DELAY_TIME) 
	{
	 ekle = 1;
    sprintf( buf, 
"     | %sYour adrenalin is gushing!                                      %s|\n\r",
	CLR_YELLOW,CLR_GREEN_BOLD);
    send_to_char(buf,ch);
	}

    if (ekle)
	{
    sprintf( buf, 
"     |%s+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+%s|\n\r",
	CLR_CYAN_BOLD,CLR_GREEN_BOLD);
    send_to_char(buf, ch);
	}

    sprintf( buf, 
"     | %sItems Carried :%s     %3d/%-4d        %sArmor vs magic  :%s %4d      %s|\n\r", 
		CLR_RED_BOLD,CLR_WHITE_BOLD,
		ch->carry_number, can_carry_n(ch),
		CLR_RED_BOLD,CLR_WHITE_BOLD,
		GET_AC(ch,AC_EXOTIC),CLR_GREEN_BOLD);
    send_to_char(buf, ch);

    sprintf( buf, 
"     | %sWeight Carried:%s  %6ld/%-8d    %sArmor vs bash   :%s %4d      %s|\n\r",
	CLR_RED_BOLD,CLR_WHITE_BOLD,
	get_carry_weight(ch), can_carry_w(ch),
	CLR_RED_BOLD,CLR_WHITE_BOLD,GET_AC(ch,AC_BASH),CLR_GREEN_BOLD);
    send_to_char(buf, ch);

    sprintf( buf, 
"     | %sGold          :%s   %-10ld        %sArmor vs pierce :%s %4d      %s|\n\r",
		CLR_RED_BOLD,CLR_WHITE_BOLD,
		ch->gold,CLR_RED_BOLD,CLR_WHITE_BOLD,
		GET_AC(ch,AC_PIERCE),CLR_GREEN_BOLD);
    send_to_char(buf, ch);

    sprintf( buf, 
"     | %sSilver        :%s   %-10ld        %sArmor vs slash  :%s %4d      %s|\n\r",
		CLR_RED_BOLD,CLR_WHITE_BOLD, 
		ch->silver,CLR_RED_BOLD,CLR_WHITE_BOLD,
		GET_AC(ch,AC_SLASH),CLR_GREEN_BOLD);
    send_to_char(buf, ch);

    sprintf( buf, 
"     | %sCurrent exp   :%s   %-6d            %sSaves vs Spell  :%s %4d      %s|\n\r",
		CLR_RED_BOLD,CLR_WHITE_BOLD,
		ch->exp,CLR_RED_BOLD,CLR_WHITE_BOLD,
		ch->saving_throw,CLR_GREEN_BOLD);
    send_to_char(buf, ch);

    sprintf( buf, 
"     | %sExp to level  :%s   %-6d                                        %s|\n\r",
		CLR_RED_BOLD,CLR_WHITE_BOLD,
		IS_NPC(ch) ? 0 : exp_to_level(ch,ch->pcdata->points),
		CLR_GREEN_BOLD);
    send_to_char(buf, ch);

    sprintf( buf, 
"     |                                     %sHitP:%s %5d / %5d         %s|\n\r",
	CLR_RED_BOLD,CLR_WHITE_BOLD,
	ch->hit, ch->max_hit,CLR_GREEN_BOLD);
    send_to_char(buf, ch);
    sprintf( buf, 
"     | %sHitroll       :%s   %-3d               %sMana:%s %5d / %5d         %s|\n\r",
	CLR_RED_BOLD,CLR_WHITE_BOLD,
	GET_HITROLL(ch),CLR_RED_BOLD,CLR_WHITE_BOLD,
	ch->mana, ch->max_mana,CLR_GREEN_BOLD);
    send_to_char(buf, ch);
    sprintf( buf, 
"     | %sDamroll       :%s   %-3d               %sMove:%s %5d / %5d         %s|\n\r",
	CLR_RED_BOLD,CLR_WHITE_BOLD,
	GET_DAMROLL(ch), CLR_RED_BOLD,CLR_WHITE_BOLD,
	ch->move, ch->max_move,CLR_GREEN_BOLD);
    send_to_char(buf, ch);
    sprintf( buf, "  /~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~/   |\n\r");
    send_to_char(buf, ch);
    sprintf( buf, "  \\________________________________________________________________\\__/%s\n\r",CLR_WHITE_BOLD);
    send_to_char(buf, ch);
    if ( ch->affected != NULL && IS_SET(ch->comm, COMM_SHOW_AFFECTS) )
	do_affects_col(ch, "");
   return;
}




void do_make_arrow( CHAR_DATA *ch, char *argument )
{
  OBJ_DATA *arrow;
  AFFECT_DATA tohit,todam,saf;
  int count,color,mana,wait;
  char buf[MAX_STRING_LENGTH];
  char arg[MAX_INPUT_LENGTH];
  char *str;
  
  if (IS_NPC(ch)) return;
  if (ch_skill_nok_nomessage(ch,gsn_make_arrow))
    {
      send_to_char("You don't know how to make arrows.\n\r", ch );
      return;
    }

  if ( ch->in_room->sector_type != SECT_FIELD &&
       ch->in_room->sector_type != SECT_FOREST &&
       ch->in_room->sector_type != SECT_HILLS ) 
  {
    send_to_char( "You couldn't find enough wood.\n\r", ch );
    return;
  }

  mana = skill_table[gsn_make_arrow].min_mana;
  wait = skill_table[gsn_make_arrow].beats;

  argument = one_argument(argument, arg);
  if (arg[0] == '\0') color = 0;
  else if (!str_prefix(arg,"green")) color = gsn_green_arrow;
  else if (!str_prefix(arg,"red")) color = gsn_red_arrow;
  else if (!str_prefix(arg,"white")) color = gsn_white_arrow;
  else if (!str_prefix(arg,"blue")) color = gsn_blue_arrow;
  else 
  {
     send_to_char( "You don't know how to make that kind of arrow.\n\r", ch );
     return;
  }

  if (color != 0 ) 
  { 
   mana += skill_table[color].min_mana;
   wait += skill_table[color].beats;
  }

  if ( ch->mana < mana )
  {
     send_to_char( "You don't have enough energy to make that kind of arrows.\n\r", ch );
     return;
  }
  ch->mana -= mana;
  WAIT_STATE(ch,wait);

  send_to_char("You start to make arrows!\n\r",ch);
  act("$n starts to make arrows!",ch,NULL,NULL,TO_ROOM);
  for(count=0; count < (ch->level/5); count++)
  {
   if ( number_percent( ) > get_skill( ch, gsn_make_arrow) )
   {
	send_to_char( "You failed to make the arrow, and broke it.\n\r", ch );
	check_improve(ch,gsn_make_arrow,FALSE,3);
	continue;
   }
   send_to_char( "You successfully make an arrow.\n\r",ch );
   check_improve(ch,gsn_make_arrow,TRUE,3);

   arrow = create_object(get_obj_index(OBJ_VNUM_RANGER_ARROW),ch->level);
   arrow->level = ch->level;
   arrow->value[1] = ch->level / 10;
   arrow->value[2] = ch->level / 10;

   tohit.where		    = TO_OBJECT;
   tohit.type               = gsn_make_arrow;
   tohit.level              = ch->level; 
   tohit.duration           = -1;
   tohit.location           = APPLY_HITROLL;
   tohit.modifier           = ch->level / 10;
   tohit.bitvector          = 0;
   affect_to_obj(arrow,&tohit);

   todam.where		   = TO_OBJECT;
   todam.type               = gsn_make_arrow;
   todam.level              = ch->level; 
   todam.duration           = -1;
   todam.location           = APPLY_DAMROLL;
   todam.modifier           = ch->level / 10;
   todam.bitvector          = 0;
   affect_to_obj(arrow,&todam);

   if ( color != 0 )
   {
    saf.where		   = TO_WEAPON;
    saf.type               = color;
    saf.level              = ch->level; 
    saf.duration           = -1;
    saf.location           = 0;
    saf.modifier           = 0;

    if (color == gsn_green_arrow)
	{
	 saf.bitvector	= WEAPON_POISON;
	 str = "green";
	}
     else if (color == gsn_red_arrow)
	{
	 saf.bitvector	= WEAPON_FLAMING;
	 str = "red";
	}
     else if (color == gsn_white_arrow)
	{
	 saf.bitvector	= WEAPON_FROST;
	 str = "white";
	}
     else
	{
	 saf.bitvector	= WEAPON_SHOCKING;
	 str = "blue";
	}
   }
   else str = "wooden";

   sprintf( buf, arrow->name, str );
   free_string( arrow->name );
   arrow->name = str_dup( buf );
	  
   sprintf( buf, arrow->short_descr, str );
   free_string( arrow->short_descr );
   arrow->short_descr = str_dup( buf );
	  
   sprintf( buf, arrow->description, str );
   free_string( arrow->description );
   arrow->description = str_dup( buf );    

   if (color !=0 ) affect_to_obj(arrow,&saf);
   obj_to_char(arrow,ch);
   arrow = NULL;
  }
  
}



void do_make_bow( CHAR_DATA *ch, char *argument )
{
  OBJ_DATA *bow;
  AFFECT_DATA tohit,todam;
  int mana,wait;
  
  if (IS_NPC(ch)) return;

  if (ch_skill_nok_nomessage(ch,gsn_make_bow))
    {
      send_to_char("You don't know how to make bows.\n\r", ch );
      return;
    }

  if ( ch->in_room->sector_type != SECT_FIELD &&
       ch->in_room->sector_type != SECT_FOREST &&
       ch->in_room->sector_type != SECT_HILLS ) 
  {
    send_to_char( "You couldn't find enough wood.\n\r", ch );
    return;
  }

  mana = skill_table[gsn_make_bow].min_mana;
  wait = skill_table[gsn_make_bow].beats;

  if ( ch->mana < mana )
  {
     send_to_char( "You don't have enough energy to make a bow.\n\r", ch);
     return;
  }
  ch->mana -= mana;
  WAIT_STATE(ch,wait);

  if ( number_percent( ) > get_skill( ch, gsn_make_bow) )
   {
	send_to_char( "You failed to make the bow, and broke it.\n\r", ch );
	check_improve(ch,gsn_make_bow,FALSE,1);
	return;
   }
  send_to_char( "You successfully make bow.\n\r",ch );
  check_improve(ch,gsn_make_bow,TRUE,1);

  bow = create_object(get_obj_index(OBJ_VNUM_RANGER_BOW),ch->level);
  bow->level = ch->level;
  bow->value[1] = 3 + ch->level / 12;
  bow->value[2] = 4 + ch->level / 12;

  tohit.where		    = TO_OBJECT;
  tohit.type               = gsn_make_arrow;
  tohit.level              = ch->level; 
  tohit.duration           = -1;
  tohit.location           = APPLY_HITROLL;
  tohit.modifier           = ch->level / 10;
  tohit.bitvector          = 0;
  affect_to_obj(bow,&tohit);

  todam.where		   = TO_OBJECT;
  todam.type               = gsn_make_arrow;
  todam.level              = ch->level; 
  todam.duration           = -1;
  todam.location           = APPLY_DAMROLL;
  todam.modifier           = ch->level / 10;
  todam.bitvector          = 0;
  affect_to_obj(bow,&todam);

  obj_to_char(bow,ch);
}


void do_make( CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument(argument,arg);
  if (arg[0] == '\0')
  {
   send_to_char("You can make either bow or arrow.\n\r",ch);
   return;
  }

  if (!str_prefix(arg,"arrow")) do_make_arrow(ch,argument);
  else if (!str_prefix(arg,"bow")) do_make_bow(ch,argument);
  else do_make(ch,"");
}


void do_nocancel(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;

    if (IS_SET(ch->act,PLR_NOCANCEL))
    {
      send_to_char("You now accept others' cancellation spells.\n\r",ch);
      REMOVE_BIT(ch->act,PLR_NOCANCEL);
    }
    else
    {
      send_to_char("You no longer accept others' cancellation spells.\n\r",ch);
      SET_BIT(ch->act,PLR_NOCANCEL);
    }
}
