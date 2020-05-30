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
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
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
DECLARE_DO_FUN( do_murder	);
DECLARE_DO_FUN( do_say  	);
DECLARE_DO_FUN( do_scan2  	);
DECLARE_DO_FUN( do_affects_col 	);
DECLARE_DO_FUN( do_who_col	);
DECLARE_DO_FUN( do_autolist_col	);

char *get_stat_alias( CHAR_DATA *ch, int which );

const char *	where_name	[] =
{
  "< parmak   >  ",
  "< boyun    >  ",
  "< gövde    >  ",
  "< kafa     >  ",
  "< bacaklar >  ",
  "< ayaklar  >  ",
  "< eller    >  ",
  "< kollar   >  ",
  "< vücut    >  ",
  "< bel      >  ",
  "< bilek    >  ",
  "< sol el   >%c ",
  "< sað el   >%c ",
  "< çift el  >  ",
  "< süzülen  >  ",
  "< dövme    >  ",
  "< saplanmýþ>  "
};


/* for do_count */
int max_on = 0;
int max_on_so_far;



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
  else
  send_to_char( "bir þey.\n\r", ch );
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
		CLR_GREEN,
		get_cond_alias(obj),
		CLR_WHITE_BOLD);
    else buf_con[0] = '\0';

    if ((fShort && (obj->short_descr == NULL || obj->short_descr[0] == '\0'))
    ||  (obj->description == NULL || obj->description[0] == '\0'))
	return buf;

    if ( IS_OBJ_STAT(obj, ITEM_BURIED)     )
		{
		 strcat( buf, CLR_WHITE );
     strcat( buf, "[gömülü] "     );//(Gömülü)
		}
    if ( IS_OBJ_STAT(obj, ITEM_INVIS)     )
		{
		 strcat( buf, CLR_WHITE );
     strcat( buf, "[görünmez] "     );//(Görünmez)
		}
    if ( CAN_DETECT(ch, DETECT_EVIL)
         && IS_OBJ_STAT(obj, ITEM_EVIL)   )
		{
		 strcat( buf, CLR_RED );
     strcat( buf, "[kýzýl aura] "  );
		}
    if (CAN_DETECT(ch, DETECT_GOOD)
    &&  IS_OBJ_STAT(obj,ITEM_BLESS))
		{
		 strcat( buf, CLR_BLUE );
     strcat( buf, "[mavi aura] "  );
		}
    if ( CAN_DETECT(ch, DETECT_MAGIC)
         && IS_OBJ_STAT(obj, ITEM_MAGIC)  )
		{
		 strcat( buf, CLR_YELLOW );
     strcat( buf, "[büyülü] "   );//(Büyülü)
		}
    if ( IS_OBJ_STAT(obj, ITEM_GLOW)      )
		{
		 strcat( buf, CLR_CYAN );
     strcat( buf, "[parlayan] "   );//(Parlayan)
		}
    if ( IS_OBJ_STAT(obj, ITEM_HUM)       )
		{
		 strcat( buf, CLR_BROWN );
     strcat( buf, "[výzýldayan] "   );//(Výzýldayan)
		}
    strcat( buf, CLR_WHITE_BOLD );


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
          case 1: strcat(buf, " suyun üstünde yüzüyor.");break;
      		case 2: strcat(buf, " suda ilerliyor.");break;
      		case 3: strcat(buf, " suda ýslanýyor.");break;
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
		add_buf(output,(char*)"     ");
	    }
	}
	add_buf(output,prgpstrShow[iShow]);
	add_buf(output,(char*)"\n\r");
	free_string( prgpstrShow[iShow] );
    }

    if ( fShowNothing && nShow == 0 )
    {
	if ( IS_NPC(ch) || IS_SET(ch->comm, COMM_COMBINE) )
	    send_to_char( "     ", ch );
      send_to_char( "Hiçbir þey.\n\r", ch );
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
  {
		strcat( buf, "{R[hedef]{x ");
}
/*
    sprintf(message,"(%s) ",race_table[RACE(victim)].name);
    message[1] = UPPER( message[1]);
    strcat(buf,message);
*/
    if ( RIDDEN(victim)  ) 			strcat( buf, "[binek] "     );
    if ( IS_AFFECTED(victim, AFF_INVISIBLE)   ) strcat( buf, "[görünmez] "      );
    if ( IS_AFFECTED(victim,AFF_IMP_INVIS )   ) strcat( buf, "[geliþmiþ] "   );
    if ( victim->invis_level >= LEVEL_HERO    ) strcat( buf, "[WiZ] "	     );
    if ( IS_AFFECTED(victim, AFF_HIDE)        ) strcat( buf, "[saklý] "       );
    if ( IS_AFFECTED(victim, AFF_FADE)        ) strcat( buf, "[solmuþ] "       );
    if ( IS_AFFECTED(victim, AFF_CAMOUFLAGE)  ) strcat( buf, "[kamufle] "       );
    if ( CAN_DETECT(victim, ADET_EARTHFADE)   ) strcat( buf, "[arz] "      );
    if ( IS_AFFECTED(victim, AFF_CHARM)
	&& victim->master == ch) 		strcat( buf, "[teshir] "    );
    if ( IS_AFFECTED(victim, AFF_PASS_DOOR)   ) strcat( buf, "{W[yarýsaydam]{x ");
    if ( IS_AFFECTED(victim, AFF_FAERIE_FIRE) ) strcat( buf, "{M[pembe aura]{x "  );
    if ( IS_NPC(victim) && IS_SET(victim->act,ACT_UNDEAD)
    &&   CAN_DETECT(ch, DETECT_UNDEAD)     ) strcat( buf, "{W[hortlak]{x ");
    if ( IS_EVIL(victim)
    &&   CAN_DETECT(ch, DETECT_EVIL)     ) strcat( buf, "{R[kýzýl aura]{x "   );
    if ( IS_GOOD(victim)
    &&   CAN_DETECT(ch, DETECT_GOOD)     ) strcat( buf, "{Y[altýn aura]{x ");
    if ( IS_AFFECTED(victim, AFF_SANCTUARY)   ) strcat( buf, "{W[beyaz aura]{x " );
    if ( !IS_NPC(victim) && IS_SET(victim->act, PLR_WANTED ) )
						strcat( buf, "[SUÇLU] ");

    if ( victim->position == victim->start_pos && victim->long_descr[0] != '\0' )
    {
	strcat( buf, victim->long_descr );
	printf_to_char( ch, "%s", buf );
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
      case POS_DEAD:
  		strcat( buf, " ÖLDÜ!!" );
  		break;
      case POS_MORTAL:
  		strcat( buf, " öldürücü yaralar almýþ." );
  		break;
      case POS_INCAP:
  		strcat( buf, " aciz durumda." );
  		break;
      case POS_STUNNED:
  		strcat( buf, " sersemlemiþ yatýyor." );
  		break;
    case POS_SLEEPING:
	if (victim->on != NULL)
	{
	    if (IS_SET(victim->on->value[2],SLEEP_AT))
  	    {
          sprintf(message," %s'de uyuyor.",victim->on->short_descr);
		strcat(buf,message);
	    }
	    else if (IS_SET(victim->on->value[2],SLEEP_ON))
	    {
        sprintf(message," %s üzerinde uyuyor.",victim->on->short_descr);
		strcat(buf,message);
	    }
	    else
	    {
        sprintf(message, " %s içinde uyuyor.",victim->on->short_descr);
		strcat(buf,message);
	    }
	}
	else
  strcat(buf," burada uyuyor.");
	break;
    case POS_RESTING:
        if (victim->on != NULL)
	{
            if (IS_SET(victim->on->value[2],REST_AT))
            {
              sprintf(message," %s'de dinleniyor.",victim->on->short_descr);
                strcat(buf,message);
            }
            else if (IS_SET(victim->on->value[2],REST_ON))
            {
              sprintf(message," %s üzerinde dinleniyor.", victim->on->short_descr);
                strcat(buf,message);
            }
            else
            {
              sprintf(message, " %s içinde dinleniyor.",victim->on->short_descr);
                strcat(buf,message);
            }
	}
        else
        strcat( buf, " burada dinleniyor." );
	break;
    case POS_SITTING:
        if (victim->on != NULL)
        {
            if (IS_SET(victim->on->value[2],SIT_AT))
            {
              sprintf(message," %s'de oturuyor.",victim->on->short_descr);
                strcat(buf,message);
            }
            else if (IS_SET(victim->on->value[2],SIT_ON))
            {
              sprintf(message," %s üzerinde oturuyor.",victim->on->short_descr);
                strcat(buf,message);
            }
            else
            {
              sprintf(message, " %s içinde oturuyor.",victim->on->short_descr);
                strcat(buf,message);
            }
        }
        else
        strcat(buf, " burada oturuyor.");
	break;
    case POS_STANDING:
	if (victim->on != NULL)
	{
	    if (IS_SET(victim->on->value[2],STAND_AT))
	    {
        sprintf(message," %s'de duruyor.",victim->on->short_descr);
		strcat(buf,message);
	    }
	    else if (IS_SET(victim->on->value[2],STAND_ON))
	    {
        sprintf(message," %s üzerinde duruyor.",victim->on->short_descr);
		strcat(buf,message);
	    }
	    else
	    {
        sprintf(message," %s içinde duruyor.",victim->on->short_descr);
		strcat(buf,message);
	    }
	}
	else if (MOUNTED(victim))
	{
    sprintf(message," burada, %s'i sürüyor.",PERS(MOUNTED(victim),ch));
	  strcat(buf, message);
	}
  else  strcat( buf, " burada." );
	break;
    case POS_FIGHTING:
    strcat( buf, " burada, " );
	if ( victim->fighting == NULL )
  strcat( buf, "havayla dövüþüyor??" );
	else if ( victim->fighting == ch )
  strcat( buf, "SENinle dövüþüyor!" );
	else if ( victim->in_room == victim->fighting->in_room )
	{
	    strcat( buf, PERS( victim->fighting, ch ) );
      strcat( buf, "ile dövüþüyor");
  	    strcat( buf, "." );	}
	else
  strcat( buf, "kim kaldýysa onunla dövüþüyor??" );
	break;
    }

    strcat( buf, "\n\r" );
    buf[0] = UPPER(buf[0]);
    send_to_char( buf, ch );
    return;
}

char *show_char_to_char_1_health_check(CHAR_DATA *ch , CHAR_DATA *victim)
{
  int percent;

  // karakterin bilgeligi onemli olsun.
  // zari tutturursa dogru sonuc ver
  if( ( number_range(1,26) < get_curr_stat(ch,STAT_WIS) ) || IS_IMMORTAL(ch) )
  {
    if ( victim->max_hit > 0 )
      percent = ( 100 * victim->hit ) / victim->max_hit;
    else
      percent = -1;
  }
  // irk tanima bilgisi de onemli olsun
  else if ( number_range(1,100) < ch->pcdata->familya[victim->race] )
  {
    if ( victim->max_hit > 0 )
      percent = ( 100 * victim->hit ) / victim->max_hit;
    else
      percent = -1;
  }
  else
  {
    percent = number_range(51,89);
  }

  /* vampire ... */
  if (percent < 90 && ch->iclass == CLASS_VAMPIRE && ch->level > 10)
  {
	   gain_condition(ch,COND_BLOODLUST,-1);
   }

  if (percent >= 100)
  {
    return (char*)"mükemmel durumda";
  }
  else if (percent >= 90)
  {
    return (char*)"birkaç çiziði var";
  }
  else if (percent >= 75)
  {
    return (char*)"birkaç küçük yarasý var";
  }
  else if (percent >=  50)
  {
    return (char*)"kanayan yaralarla kaplanmýþ";
  }
  else if (percent >= 30)
  {
    return (char*)"kötü kanýyor";
  }
  else if (percent >= 15)
  {
    return (char*)"acý içinde kývranýyor";
  }
  else if (percent >= 0 )
  {
    return (char*)"yerde sürünüyor";
  }

  return (char*)"ölmek üzere";
}

char *show_char_to_char_1_alignment(CHAR_DATA *ch , CHAR_DATA *victim)
{
  int alignment;

  // karakterin bilgeligi onemli olsun.
  // zari tutturursa dogru sonuc ver
  if( ( number_range(1,26) < get_curr_stat(ch,STAT_WIS) ) || IS_IMMORTAL(ch) )
  {
    alignment = victim->alignment;
  }
  // irk bilgisi de onemli olsun
  else if ( number_range(1,100) < ch->pcdata->familya[victim->race] )
  {
    alignment = victim->alignment;
  }
  else
  {
    alignment = number_range(-1000,1000);
  }

  if (alignment >= 350)
  {
    return (char*)"iyi";
  }
  else if (alignment <= -350)
  {
    return (char*)"kem";
  }
  else
  {
    return (char*)"yansýz";
  }

  return (char*)"yansýz";
}



void show_char_to_char_1( CHAR_DATA *victim, CHAR_DATA *ch )
{
    OBJ_DATA *obj;
    int iWear;
    bool found;
    CHAR_DATA *vict;

    vict = is_affected(victim,gsn_doppelganger) ? victim->doppel : victim;

    if ( can_see( victim, ch ) )
    {
	if (ch == victim)
  act( "$n kendisine bakýyor.",ch,NULL,NULL,TO_ROOM);
else
{
  act("$n sana bakýyor.", ch, NULL, victim, TO_VICT    );
  act( "$n $E bakýyor.",  ch, NULL, victim, TO_NOTVICT );
	}
    }
    printf_to_char(ch,"[{c%s{x] [{c%s{x] [{c%s{x] [{c%s{x]\n\r\n\r",(vict->sex==SEX_MALE?"erkek":"kadýn"),race_table[RACE(vict)].name[1], show_char_to_char_1_alignment(ch,victim),show_char_to_char_1_health_check(ch,victim));

    if ( vict->description[0] != '\0' )
    {
      printf_to_char(ch,"%s\n\r\n\r",vict->description);
    }
    else
    {
      printf_to_char(ch,"Onun hakkýnda ilgi çekici birþey bulamýyorsun.\n\r\n\r");
    }

    if ( MOUNTED(victim) )
    {
      printf_to_char(ch,"%s %s sürüyor.\n\r\n\r",PERS(victim,ch), PERS( MOUNTED(victim),ch));
    }
    if ( RIDDEN(victim) )
    {
      printf_to_char(ch,"%s %s tarafýndan sürülüyor.\n\r\n\r",PERS(victim,ch), PERS( RIDDEN(victim),ch));
    }

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
       act( "$S kullandýklarý:", ch, NULL, victim, TO_CHAR);
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
       act( "$S kullandýklarý:", ch, NULL, victim, TO_CHAR);
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
	send_to_char( "\n\rEnvantere göz atýyorsun:\n\r", ch );
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
    send_to_char( "SENi izleyen kýrmýzý gözler görüyorsun!\n\r", ch );
	    if (!IS_IMMORTAL(rch)) life_count++;
	}
	else if (!IS_IMMORTAL(rch)) life_count++;
    }

    if (life_count && CAN_DETECT(ch,DETECT_LIFE) )
	{
	 char buf[MAX_STRING_LENGTH];
   sprintf(buf,"Odada %d yaþam formu seziyorsun.\n\r",life_count);
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
      send_to_char("Hiçbir þey göremiyorsun!\n\r", ch );
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

/* RT this following section holds all the auto commands from ROM, as well as
   replacements for config */

void do_autolist(CHAR_DATA *ch, char *argument)
{
    /* lists most player flags */
    if (IS_NPC(ch))
      return;

  send_to_char("oto  durumu\n\r",ch);
    send_to_char("---------------------\n\r",ch);

  send_to_char("otodestek       ",ch);
    if (IS_SET(ch->act,PLR_AUTOASSIST))
    send_to_char("AÇIK\n\r",ch);
    else
    send_to_char("KAPALI\n\r",ch);

    send_to_char("otoçýkýþ        ",ch);
    if (IS_SET(ch->act,PLR_AUTOEXIT))
    send_to_char("AÇIK\n\r",ch);
    else
    send_to_char("KAPALI\n\r",ch);

    send_to_char("otosikke         ",ch);
    if (IS_SET(ch->act,PLR_AUTOGOLD))
    send_to_char("AÇIK\n\r",ch);
    else
    send_to_char("KAPALI\n\r",ch);

    send_to_char("otoyaðma        ",ch);
    if (IS_SET(ch->act,PLR_AUTOLOOT))
    send_to_char("AÇIK\n\r",ch);
    else
    send_to_char("KAPALI\n\r",ch);

    send_to_char("otokurban       ",ch);
    if (IS_SET(ch->act,PLR_AUTOSAC))
    send_to_char("AÇIK\n\r",ch);
    else
    send_to_char("KAPALI\n\r",ch);

    send_to_char("otodaðýt        ",ch);
    if (IS_SET(ch->act,PLR_AUTOSPLIT))
    send_to_char("AÇIK\n\r",ch);
    else
    send_to_char("KAPALI\n\r",ch);

    send_to_char("kýsa            ",ch);
    if (IS_SET(ch->comm,COMM_COMPACT))
    send_to_char("AÇIK\n\r",ch);
    else
    send_to_char("KAPALI\n\r",ch);

    send_to_char("suflör          ",ch);
    if (IS_SET(ch->comm,COMM_PROMPT))
    send_to_char("AÇIK\n\r",ch);
    else
    send_to_char("KAPALI\n\r",ch);

    send_to_char("bileþik         ",ch);
    if (IS_SET(ch->comm,COMM_COMBINE))
    send_to_char("AÇIK\n\r",ch);
    else
    send_to_char("KAPALI\n\r",ch);


    if (IS_SET(ch->act,PLR_NOSUMMON))
    send_to_char("Ancak OK aralýðýnýn çaðrý büyülerinden etkilenebilirsin.\n\r",ch);
    else
    send_to_char("Herkesin çaðrý büyüsünden etkilenebilirsin.\n\r",ch);

    if (IS_SET(ch->act,PLR_NOFOLLOW))
	send_to_char("Takipçileri kabul etmiyorsun.\n\r",ch);
    else
	send_to_char("Takipçileri kabul ediyorsun.\n\r",ch);

    if (IS_SET(ch->act,PLR_NOCANCEL))
    send_to_char("Ancak OK aralýðýýn iptal büyülerininden etkilenebilirsin.\n\r",ch);
    else
    send_to_char("Herkesin iptal büüyüsünden etkilenebilirsin.\n\r",ch);
}

void do_autoassist(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;

    if (IS_SET(ch->act,PLR_AUTOASSIST))
    {
      send_to_char("Otodestek kaldýrýldý.\n\r",ch);
      REMOVE_BIT(ch->act,PLR_AUTOASSIST);
    }
    else
    {
      send_to_char("Otodestek açýldý.\n\r",ch);
      SET_BIT(ch->act,PLR_AUTOASSIST);
    }
}

void do_autoexit(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;

    if (IS_SET(ch->act,PLR_AUTOEXIT))
    {
      send_to_char("Otoçýkýþ kaldýrýldý.\n\r",ch);
      REMOVE_BIT(ch->act,PLR_AUTOEXIT);
    }
    else
    {
      send_to_char("Otoçýkýþ açýldý.\n\r",ch);
      SET_BIT(ch->act,PLR_AUTOEXIT);
    }
}

void do_autogold(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;

    if (IS_SET(ch->act,PLR_AUTOGOLD))
    {
      send_to_char("Otosikke kaldýrýldý.\n\r",ch);
      REMOVE_BIT(ch->act,PLR_AUTOGOLD);
    }
    else
    {
      send_to_char("Otosikke açýldý.\n\r",ch);
      SET_BIT(ch->act,PLR_AUTOGOLD);
    }
}

void do_autoloot(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;

    if (IS_SET(ch->act,PLR_AUTOLOOT))
    {
      send_to_char("Otoyaðma kaldýrýldý.\n\r",ch);
      REMOVE_BIT(ch->act,PLR_AUTOLOOT);
    }
    else
    {
      send_to_char("Otoyaðma açýldý.\n\r",ch);
      SET_BIT(ch->act,PLR_AUTOLOOT);
    }
}

void do_autosac(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;

    if (IS_SET(ch->act,PLR_AUTOSAC))
    {
      send_to_char("Otokurban kaldýrýldý.\n\r",ch);
      REMOVE_BIT(ch->act,PLR_AUTOSAC);
    }
    else
    {
      send_to_char("Otokurban açýldý.\n\r",ch);
      SET_BIT(ch->act,PLR_AUTOSAC);
    }
}

void do_autosplit(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;

    if (IS_SET(ch->act,PLR_AUTOSPLIT))
    {
      send_to_char("Otodaðýt kaldýrýldý.\n\r",ch);
      REMOVE_BIT(ch->act,PLR_AUTOSPLIT);
    }
    else
    {
      send_to_char("Otodaðýt açýldý.\n\r",ch);
      SET_BIT(ch->act,PLR_AUTOSPLIT);
    }
}

void do_brief(CHAR_DATA *ch, char *argument)
{
    if (IS_SET(ch->comm,COMM_BRIEF))
    {
      send_to_char("Özet taným kapandý.\n\r",ch);
      REMOVE_BIT(ch->comm,COMM_BRIEF);
    }
    else
    {
      send_to_char("Özet taným açýldý.\n\r",ch);
      SET_BIT(ch->comm,COMM_BRIEF);
    }
}

void do_compact(CHAR_DATA *ch, char *argument)
{
    if (IS_SET(ch->comm,COMM_COMPACT))
    {
      printf_to_char(ch,"Kompakt modu kapatýldý.\n\r");
      REMOVE_BIT(ch->comm,COMM_COMPACT);
    }
    else
    {
      printf_to_char(ch,"Kompakt modu açýldý.\n\r");
      SET_BIT(ch->comm,COMM_COMPACT);
    }
}

void do_show(CHAR_DATA *ch, char *argument)
{
    if (IS_SET(ch->comm,COMM_SHOW_AFFECTS))
    {
      printf_to_char(ch,"Etkiler skor tablosunda gösterilmeyecek.\n\r");
      REMOVE_BIT(ch->comm,COMM_SHOW_AFFECTS);
    }
    else
    {
      printf_to_char(ch,"Etkiler skor tablosunda gösterilecek.\n\r");
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
      printf_to_char(ch,"Suflör kapatýldý.\n\r");
      REMOVE_BIT(ch->comm,COMM_PROMPT);
    }
    else
    {
      printf_to_char(ch,"Suflör açýldý.\n\r");
      SET_BIT(ch->comm,COMM_PROMPT);
    }
    return;
  }

  if( !strcmp( argument, "tümü" ) )
  {
    strcpy( buf, "Yp:%h/%H Mp:%m/%M Hp:%v/%V <%o>->{x ");
  }
  else
  {
    if ( strlen(argument) > 50 )
    {
      argument[50] = '\0';
    }
    strcpy( buf, argument );
    smash_tilde( buf );
    if (str_suffix("%c",buf))
    {
      strcat(buf," ");
    }
  }

  free_string( ch->prompt );
  ch->prompt = str_dup( buf );
  printf_to_char(ch,"Suflör %s olarak ayarlandý.\n\r",ch->prompt );
  send_to_char(buf,ch);
  return;
}

void do_combine(CHAR_DATA *ch, char *argument)
{
    if (IS_SET(ch->comm,COMM_COMBINE))
    {
      send_to_char("Bileþik nesne kaldýrýldý.\n\r",ch);
      REMOVE_BIT(ch->comm,COMM_COMBINE);
    }
    else
    {
      send_to_char("Bileþik nesne açýldý.\n\r",ch);
      SET_BIT(ch->comm,COMM_COMBINE);
    }
}

void do_noloot(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;

    if (IS_SET(ch->act,PLR_CANLOOT))
    {
      printf_to_char(ch,"Cesedin hýrsýzlara karþý güvende.\n\r");
      REMOVE_BIT(ch->act,PLR_CANLOOT);
    }
    else
    {
      printf_to_char(ch,"Cesedin yaðmalanabilir.\n\r");
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
      printf_to_char(ch,"Takipçileri kabul etmeye baþladýn.\n\r");
      REMOVE_BIT(ch->act,PLR_NOFOLLOW);
    }
    else
    {
      printf_to_char(ch,"Takipçileri kabul etmiyorsun.\n\r");
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
        send_to_char("Çaðrý büyülerine baðýþýklý deðilsin.\n\r",ch);
	REMOVE_BIT(ch->imm_flags,IMM_SUMMON);
      }
      else
      {
        send_to_char("Çaðrý büyülerine baðýþýklýsýn.\n\r",ch);
	SET_BIT(ch->imm_flags,IMM_SUMMON);
      }
    }
    else
    {
      if (IS_SET(ch->act,PLR_NOSUMMON))
      {
        send_to_char("Herkesin çaðrý büyüsüne açýksýn.\n\r",ch);
        REMOVE_BIT(ch->act,PLR_NOSUMMON);
      }
      else
      {
        send_to_char("Yalnýz OK aralýðýnýn çaðrý büyülerine açýksýn.\n\r",ch);
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
      send_to_char( "Yýldýzlardan baþka bir þey göremiyorsun!\n\r", ch );
	return;
    }

    if ( ch->position == POS_SLEEPING )
    {
      send_to_char( "Bir þey göremiyorsun, uyuyorsun!\n\r", ch );
	return;
    }

    if ( !check_blind( ch ) )
	{
		send_to_char( "Körsün!\n\r", ch );
		return;
	}

    if ( !IS_NPC(ch)
    &&   !IS_SET(ch->act, PLR_HOLYLIGHT)
    &&   !IS_SET(ch->act,PLR_GHOST)
    &&   room_is_dark( ch ) )
    {
      send_to_char( "Zifiri karanlýk ... \n\r", ch );
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
  printf_to_char( ch, "{c%s{x", ch->in_room->name );
  switch(ch->in_room->sector_type)
	{
			case SECT_INSIDE:
				printf_to_char(ch," [Ýçeri]");
				break;
			case SECT_CITY:
				printf_to_char(ch," [Þehir]");
				break;
			case SECT_FIELD:
				printf_to_char(ch," [Ova]");
				break;
			case SECT_FOREST:
				printf_to_char(ch," [Orman]");
				break;
			case SECT_HILLS:
				printf_to_char(ch," [Tepe]");
				break;
			case SECT_MOUNTAIN:
				printf_to_char(ch," [Dað]");
				break;
			case SECT_WATER_SWIM:
			case SECT_WATER_NOSWIM:
				printf_to_char(ch," [Su]");
				break;
			case SECT_AIR:
				printf_to_char(ch," [Hava]");
				break;
			case SECT_DESERT:
				printf_to_char(ch," [Çöl]");
				break;
			case SECT_MAX:
				printf_to_char(ch," [Zorlu]");
				break;
			default:
				printf_to_char(ch," [*Bilinmeyen*]");
				break;
	}
	printf_to_char(ch," [{y%s{x]",ch->in_room->area->name);

	if (IS_IMMORTAL(ch) && (IS_NPC(ch) || IS_SET(ch->act,PLR_HOLYLIGHT)))
	{
      printf_to_char(ch," [%d]",ch->in_room->vnum);
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
            do_exits( ch, (char*)"auto" );
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
    send_to_char( "Neyin içine bakacaksýn?\n\r", ch );
	    return;
	}

	if ( ( obj = get_obj_here( ch, arg2 ) ) == NULL )
	{
    send_to_char("Onu görmüyorsun.\n\r", ch );
	    return;
	}

	switch ( obj->item_type )
	{
	default:
  send_to_char( "O bir taþýyýcý deðil.\n\r", ch );
	    break;

	case ITEM_DRINK_CON:
	    if ( obj->value[1] <= 0 )
	    {
        send_to_char("Ýçi boþ.\n\r", ch );
		break;
	    }

      sprintf( buf, "%s %s sývýsýyla dolu.\n\r",
		obj->value[1] <     obj->value[0] / 4
    ? "Yarýdan azý" :
		obj->value[1] < 3 * obj->value[0] / 4
    ? "Yarýsý"     : "Yarýdan fazlasý",
		liq_table[obj->value[2]].liq_color
		);

	    send_to_char( buf, ch );
	    break;

	case ITEM_CONTAINER:
	case ITEM_CORPSE_NPC:
	case ITEM_CORPSE_PC:
	    if ( IS_SET(obj->value[1], CONT_CLOSED) )
	    {
        send_to_char( "Kapalý.\n\r", ch );
		break;
	    }

      act( "$p þunlarý içeriyor:", ch, obj, NULL, TO_CHAR );
	    show_list_to_char( obj->contains, ch, TRUE, TRUE );
	    break;
	}
	return;
    }

    /*
     * baslangic:
     * bak <karakter_ismi>
     */
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

        act("$n sence de tatlý deðil mi?", victim, NULL, ch, TO_VICT);
        act("$N büyülenmiþ gözlerle sana bakýyor.",victim,NULL,ch,TO_CHAR);
        act("$N büyülenmiþ gözlerle $e bakýyor.",victim,NULL,ch,TO_NOTVICT);
      }

      return;
    }
    /*
     * bitis:
     * bak <karakter_ismi>
     */


    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
	if ( can_see_obj( ch, obj ) )
	{  /* player can see object */
	    pdesc = get_extra_descr( arg3, obj->extra_descr );
	    if ( pdesc != NULL )
	    {
	    	if (++count == number)
	    	{
		    send_to_char( pdesc, ch );
		    return;
	    	}
	    	else continue;
	    	}

 	    pdesc = get_extra_descr( arg3, obj->pIndexData->extra_descr );
 	    if ( pdesc != NULL )
 	    {
 	    	if (++count == number)
 	    	{
		    send_to_char( pdesc, ch );
		    return;
	     	}
		else continue;
		}

	    if ( is_name( arg3, obj->name ) )
	    	if (++count == number)
	    	{
          send_to_char("Özel bir þey görmüyorsun.\n\r", ch);
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
      sprintf(buf,"Sadece bir %s görüyorsun.\n\r",arg3);
    	else
      sprintf(buf,"Ondan sadece %d tane görüyorsun.\n\r",count);

    	send_to_char(buf,ch);
    	return;
    }

    if ( !str_cmp( arg1, "k" ) || !str_cmp( arg1, "kuzey" ) ) door = 0;
else if ( !str_cmp( arg1, "d" ) || !str_cmp( arg1, "doðu"  ) ) door = 1;
else if ( !str_cmp( arg1, "g" ) || !str_cmp( arg1, "güney" ) ) door = 2;
else if ( !str_cmp( arg1, "b" ) || !str_cmp( arg1, "batý"  ) ) door = 3;
else if ( !str_cmp( arg1, "y" ) || !str_cmp( arg1, "yukarý"    ) ) door = 4;
else if ( !str_cmp( arg1, "a" ) || !str_cmp( arg1, "aþaðý"  ) ) door = 5;
    else
    {
      send_to_char("Onu görmüyorsun.\n\r", ch );
	return;
    }

    /* 'look direction' */
    if ( ( pexit = ch->in_room->exit[door] ) == NULL )
    {
      send_to_char("Özel bir þey yok.\n\r", ch );
	return;
    }

    if ( pexit->description != NULL && pexit->description[0] != '\0' )
	send_to_char( pexit->description, ch );
    else
    send_to_char( "Özel bir þey yok.\n\r", ch );

    if ( pexit->keyword    != NULL
    &&   pexit->keyword[0] != '\0'
    &&   pexit->keyword[0] != ' ' )
    {
	if ( IS_SET(pexit->exit_info, EX_CLOSED) )
	{
    act( "$d kapalý.", ch, NULL, pexit->keyword, TO_CHAR );
	}
	else if ( IS_SET(pexit->exit_info, EX_ISDOOR) )
	{
    act( "$d açýk.",   ch, NULL, pexit->keyword, TO_CHAR );
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
      send_to_char("Neyi inceleyeceksin?\n\r", ch );
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
          sprintf(buf,"Tuhaf...yýðýn içinde sikke yok.\n\r");
		else if (obj->value[1] == 1)
    sprintf(buf,"Vayy. Bir altýn.\n\r");
		else
    sprintf(buf,"Yýðýnda %d altýn var.\n\r",obj->value[1]);
	    }
	    else if (obj->value[1] == 0)
	    {
		if (obj->value[0] == 1)
    sprintf(buf,"Vayy. Bir akçe.\n\r");
		else
    sprintf(buf,"Yýðýnda %d akçe var.\n\r",obj->value[0]);
	    }
	    else
		sprintf(buf,
      "Yýðýnda %d altýn ve %d akçe var.\n\r",obj->value[1],obj->value[0]);
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
    extern const char * dir_name[];
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
    sprintf(buf,CLR_CYAN);
    strcat(buf,"[Çýkýþlar:");
  }
  else if (IS_IMMORTAL(ch))
  {
    sprintf(buf,"%d nolu odadan çýkýþlar:\n\r",ch->in_room->vnum);
  }
  else
  {
    sprintf(buf,"Görünen çýkýþlar:\n\r");
  }

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
          ?  "Zifiri karanlýk"
			: pexit->u1.to_room->name
		    );
		if (IS_IMMORTAL(ch))
		    sprintf(buf + strlen(buf),
			" (oda %d)\n\r",pexit->u1.to_room->vnum);
		else
		    sprintf(buf + strlen(buf), "\n\r");
	    }
	}

	if ( ( pexit = ch->in_room->exit[door] ) != NULL
	&&   pexit->u1.to_room != NULL
	&&   can_see_room(ch,pexit->u1.to_room)
	&&   IS_SET(pexit->exit_info, EX_CLOSED) )
	{
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
			" (oda %d)\n\r",pexit->u1.to_room->vnum);
		else
		    sprintf(buf + strlen(buf), "\n\r");
	    }
	}

    }

    if ( !found )
	strcat( buf, fAuto ? " hiç" : "Hiç.\n\r" );

    if ( fAuto )
	{
	  strcat( buf, "]\n\r" );
	  strcat( buf, CLR_WHITE_BOLD);
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
      printf_to_char(ch,"%ld altýnýn ve %ld akçen var.\n\r",
	    ch->gold,ch->silver);
	send_to_char(buf,ch);
	return;
    }

    printf_to_char(ch,
    "%ld altýnýn, %ld akçen ve %d Tecrübe Puanýn var (seviye atlamaya %d).\n\r",
	ch->gold, ch->silver,ch->exp,
	(ch->level + 1) * exp_per_level(ch,ch->pcdata->points) - ch->exp);
    sprintf(buf,"Þimdiye kadar %3d %s and %3d %s öldürdün.\n\r",
		ch->pcdata->has_killed,
		IS_GOOD(ch) ? "iyi olmayan" :
				IS_EVIL(ch) ? "kem olmayan" : "yansýz olmayan",
		ch->pcdata->anti_killed,
		IS_GOOD(ch) ? "iyi" :	IS_EVIL(ch) ? "kem" : "yansýz");
    send_to_char(buf, ch);

    total_played = get_total_played(ch);
    sprintf(buf,
	"Son 14 günde, %d saat ve %d dakika oynadýn.\n\r",
	(int) (total_played / 60), (total_played % 60) );
    send_to_char(buf, ch);
    if (IS_IMMORTAL(ch))
    {
	int l;

	for( l=0; l<MAX_TIME_LOG; l++)
	{
	   sprintf(buf, "Gün: %3d Oynama zamaný: %3d\n",
		ch->pcdata->log_date[l], ch->pcdata->log_time[l]);
	   send_to_char(buf, ch);
	}
    }

    return;
}

void do_score( CHAR_DATA *ch, char *argument )
{
	char sex[8];
  char dogumGunu[20];
	sex[0]='\0';
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument(argument,arg);

  if(arg[0]!='\0' && !IS_IMMORTAL(ch))
  {
     printf_to_char(ch,"Bu komutla argüman kullanýlmaz.\n\r");
     return;
  }

  if(arg[0]!='\0' && IS_IMMORTAL(ch))
  {
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
      if ( ( victim = get_char_world( ch, arg ) ) == NULL )
      {
        printf_to_char(ch, "Oyunda deðil.\n\r" );
        return;
      }
    }
  }

  if (victim != NULL)
  {
    // Mob
    if(IS_NPC(victim))
    {
       mob_score(ch,victim);
       return;
    }

    switch (victim->sex)
    {
      case 0:
        strcpy(sex,"yok");
        break;
      case 1:
        strcpy(sex,"erkek");
        break;
      case 2:
        strcpy(sex,"kadýn");
        break;
    }

    game_time_to_string(victim->pcdata->birth_time,dogumGunu);
  }
  else
  {
    switch (ch->sex)
    {
      case 0:
        strcpy(sex,"yok");
        break;
      case 1:
        strcpy(sex,"erkek");
        break;
      case 2:
        strcpy(sex,"kadýn");
        break;

    }

    game_time_to_string(ch->pcdata->birth_time,dogumGunu);
  }

  printf_to_char(ch,"{c,---------------------------------------------------------------------,{w\n\r");
  printf_to_char(ch,"{c|{w%+12s%-30s{c                           |\n\r",(victim==NULL?ch:victim)->name,(victim==NULL?ch:victim)->pcdata->title);
  printf_to_char(ch,"{c|-------------------------,-------------------------------------------,{w\n\r");
  printf_to_char(ch,"{c| Irk     : {w%-13s{c | ZIRH         | PARA                       |\n\r",race_table[(victim==NULL?ch:victim)->race].name[1]);
  printf_to_char(ch,"{c| Yaþ     : {w%-13d{c | Delici : {w%-4d{c| Altýn       : {w%-7ld{c      |\n\r",get_age(victim==NULL?ch:victim),GET_AC((victim==NULL?ch:victim),AC_PIERCE),(victim==NULL?ch:victim)->gold);
  printf_to_char(ch,"{c| Cinsiyet: {w%-13s{c | Ezici  : {w%-4d{c| Akçe        : {w%-7ld{c      |\n\r",sex,GET_AC((victim==NULL?ch:victim),AC_BASH),(victim==NULL?ch:victim)->silver);
  printf_to_char(ch,"{c| Sýnýf   : {w%-13s{c | Kesici : {w%-4d{c| Altýn(Banka): {w%-7ld{c      |\n\r",class_table[(victim==NULL?ch:victim)->iclass].name[1],GET_AC((victim==NULL?ch:victim),AC_SLASH),(victim==NULL?ch:victim)->pcdata->bank_g);
  printf_to_char(ch,"{c| Yönelim : {w%-13d{c | Egzotik: {w%-4d{c| Akçe (Banka): {w%-7ld{c      |\n\r",(victim==NULL?ch:victim)->alignment,GET_AC((victim==NULL?ch:victim),AC_EXOTIC),(victim==NULL?ch:victim)->pcdata->bank_s);
  printf_to_char(ch,"{c| Doðum   : {w%-12s{c  |              |                            |\n\r",dogumGunu);
  printf_to_char(ch,"{c|-------------------------'--------------|----------------------------,{w\n\r");
  printf_to_char(ch,"{c| Yp    : {w%-7d/%-7d{c | Güç: {w%-2d(%-2d){c  | Pratik : {w%-3d{c               |\n\r",(victim==NULL?ch:victim)->hit,(victim==NULL?ch:victim)->max_hit,(victim==NULL?ch:victim)->perm_stat[STAT_STR],get_curr_stat((victim==NULL)?ch:victim,STAT_STR),((victim==NULL)?ch:victim)->practice);
  printf_to_char(ch,"{c| Mana  : {w%-7d/%-7d{c | Zek: {w%-2d(%-2d){c  | Eðitim : {w%-3d{c               |\n\r",(victim==NULL?ch:victim)->mana, (victim==NULL?ch:victim)->max_mana,(victim==NULL?ch:victim)->perm_stat[STAT_INT],get_curr_stat((victim==NULL?ch:victim),STAT_INT),(victim==NULL?ch:victim)->train);
  printf_to_char(ch,"{c| Hp    : {w%-7d/%-7d{c | Bil: {w%-2d(%-2d){c  | GüvenS : {w%-3d{c               |\n\r",(victim==NULL?ch:victim)->move, (victim==NULL?ch:victim)->max_move,(victim==NULL?ch:victim)->perm_stat[STAT_WIS],get_curr_stat((victim==NULL?ch:victim),STAT_WIS),get_trust( (victim==NULL?ch:victim) ));
  printf_to_char(ch,"{c| Seviye: {w%-10d{c      | Çev: {w%-2d(%-2d){c  | Eþya   : {w%-3d / %-4d{c        |\n\r",(victim==NULL?ch:victim)->level,(victim==NULL?ch:victim)->perm_stat[STAT_DEX],get_curr_stat((victim==NULL?ch:victim),STAT_DEX),(victim==NULL?ch:victim)->carry_number, can_carry_n((victim==NULL?ch:victim)));
  printf_to_char(ch,"{c| Kalan : {w%-10d{c      | Bün: {w%-2d(%-2d){c  | Aðýrlýk: {w%-6ld / %-8d{c |\n\r",((victim==NULL?ch:victim)->level + 1) * exp_per_level((victim==NULL?ch:victim),(victim==NULL?ch:victim)->pcdata->points) - (victim==NULL?ch:victim)->exp,(victim==NULL?ch:victim)->perm_stat[STAT_CON],get_curr_stat((victim==NULL?ch:victim),STAT_CON),get_carry_weight((victim==NULL?ch:victim)), can_carry_w((victim==NULL?ch:victim)));
  printf_to_char(ch,"{c| TP    : {w%-12ld{c    | Kar: {w%-2d(%-2d){c  | GörevP : {w%-5d{c             |\n\r",(victim==NULL?ch:victim)->exp,(victim==NULL?ch:victim)->perm_stat[STAT_CHA],get_curr_stat((victim==NULL?ch:victim),STAT_CHA),(victim==NULL?ch:victim)->pcdata->questpoints);
  printf_to_char(ch,"{c| Korkak: {w%-10d{c      | ZZ : {w%-3d{c     | GörevZ : {w%-2d{c                |\n\r",(victim==NULL?ch:victim)->wimpy,GET_DAMROLL((victim==NULL?ch:victim)),((IS_SET((victim==NULL?ch:victim)->act, PLR_QUESTOR))?((victim==NULL?ch:victim)->pcdata->countdown):((victim==NULL?ch:victim)->pcdata->nextquest)));
  printf_to_char(ch,"{c| Ölüm  : {w%-3d{c             | VZ : {w%-3d{c     | RolP   : {w%-6ld{c            |\n\r",(victim==NULL?ch:victim)->pcdata->death,GET_HITROLL((victim==NULL?ch:victim)), (victim==NULL?ch:victim)->pcdata->rk_puani);
  printf_to_char(ch,"{c|                         |              | DinP   : {w%-6ld{c            |\n\r",(victim==NULL?ch:victim)->pcdata->din_puani);
  printf_to_char(ch,"{c|-------------------------'--------------'----------------------------|{w\n\r");
  printf_to_char(ch,"{c| {wBaðýþýklýklar, Dayanýklýlýklar, Zayýflýklar{c                         |{x\n\r");
  printf_to_char(ch,"{c| {cteshir:%s%s%s {cçaðrý  :%s%s%s {cbüyü  :%s%s%s {csilah :%s%s%s {cezici :%s%s%s {cdelici  :%s%s%s{c|{x\n\r",((victim==NULL?ch:victim)->imm_flags  & IMM_CHARM)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->res_flags  & RES_CHARM)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->vuln_flags  & VULN_CHARM)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->imm_flags  & IMM_SUMMON)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->res_flags  & RES_SUMMON)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->vuln_flags  & VULN_SUMMON)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->imm_flags  & IMM_MAGIC)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->res_flags  & RES_MAGIC)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->vuln_flags  & VULN_MAGIC)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->imm_flags  & IMM_WEAPON)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->res_flags  & RES_WEAPON)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->vuln_flags  & VULN_WEAPON)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->imm_flags  & IMM_BASH)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->res_flags  & RES_BASH)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->vuln_flags  & VULN_BASH)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->imm_flags  & IMM_PIERCE)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->res_flags  & RES_PIERCE)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->vuln_flags  & VULN_PIERCE)?"{w+{x":"{D-{x");
  printf_to_char(ch,"{c| {ckesici:%s%s%s {cemici  :%s%s%s {cateþ  :%s%s%s {cayaz  :%s%s%s {cþimþek:%s%s%s {casit    :%s%s%s{c|{x\n\r",((victim==NULL?ch:victim)->imm_flags  & IMM_SLASH)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->res_flags  & RES_SLASH)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->vuln_flags  & VULN_SLASH)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->imm_flags  & IMM_DROWNING)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->res_flags  & RES_DROWNING)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->vuln_flags  & VULN_DROWNING)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->imm_flags  & IMM_FIRE)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->res_flags  & RES_FIRE)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->vuln_flags  & VULN_FIRE)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->imm_flags  & IMM_COLD)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->res_flags  & RES_COLD)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->vuln_flags  & VULN_COLD)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->imm_flags  & IMM_LIGHTNING)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->res_flags  & RES_LIGHTNING)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->vuln_flags  & VULN_LIGHTNING)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->imm_flags  & IMM_ACID)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->res_flags  & RES_ACID)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->vuln_flags  & VULN_ACID)?"{w+{x":"{D-{x");
  printf_to_char(ch,"{c| {czehir :%s%s%s {cnegatif:%s%s%s {ckutsal:%s%s%s {cenerji:%s%s%s {czihin :%s%s%s {chastalýk:%s%s%s{c|{x\n\r",((victim==NULL?ch:victim)->imm_flags  & IMM_POISON)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->res_flags  & RES_POISON)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->vuln_flags  & VULN_POISON)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->imm_flags  & IMM_NEGATIVE)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->res_flags  & RES_NEGATIVE)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->vuln_flags  & VULN_NEGATIVE)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->imm_flags  & IMM_HOLY)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->res_flags  & RES_HOLY)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->vuln_flags  & VULN_HOLY)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->imm_flags  & IMM_ENERGY)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->res_flags  & RES_ENERGY)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->vuln_flags  & VULN_ENERGY)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->imm_flags  & IMM_MENTAL)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->res_flags  & RES_MENTAL)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->vuln_flags  & VULN_MENTAL)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->imm_flags  & IMM_DISEASE)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->res_flags  & RES_DISEASE)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->vuln_flags  & VULN_DISEASE)?"{w+{x":"{D-{x");
  printf_to_char(ch,"{c| {cýþýk  :%s%s%s {cses    :%s%s%s {ctahta :%s%s%s {cgümüþ :%s%s%s {cdemir :%s%s%s             {c|{x\n\r",((victim==NULL?ch:victim)->imm_flags  & IMM_LIGHT)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->res_flags  & RES_LIGHT)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->vuln_flags  & VULN_LIGHT)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->imm_flags  & IMM_SOUND)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->res_flags  & RES_SOUND)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->vuln_flags  & VULN_SOUND)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->imm_flags  & IMM_WOOD)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->res_flags  & RES_WOOD)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->vuln_flags  & VULN_WOOD)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->imm_flags  & IMM_SILVER)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->res_flags  & RES_SILVER)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->vuln_flags  & VULN_SILVER)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->imm_flags  & IMM_IRON)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->res_flags  & RES_IRON)?"{w+{x":"{D-{x",((victim==NULL?ch:victim)->vuln_flags  & VULN_IRON)?"{w+{x":"{D-{x");
  printf_to_char(ch,"{c|---------------------------------------------------------------------|{x\n\r");
  printf_to_char(ch,"{c| Susuzluk   : {w%-3d{c Açlýk    : {w%-3d{c Memleket Özlemi: {w%-3d{c                |\n\r",(victim==NULL?ch:victim)->pcdata->condition[COND_THIRST],(victim==NULL?ch:victim)->pcdata->condition[COND_HUNGER],(victim==NULL?ch:victim)->pcdata->condition[COND_DESIRE]);
  printf_to_char(ch,"{c| Kana susama: {w%-3d{c Sarhoþluk: {w%-3d{c Adrenalin      : {w%-5s{c              |\n\r",(victim==NULL?ch:victim)->pcdata->condition[COND_BLOODLUST],(victim==NULL?ch:victim)->pcdata->condition[COND_DRUNK],(ch->last_fight_time == -1)?"Hayýr":(((current_time - ch->last_fight_time) <FIGHT_DELAY_TIME)?"Evet":"Hayýr"));
  printf_to_char(ch,"{c'---------------------------------------------------------------------'{x\n\r");
}

void mob_score(CHAR_DATA *ch,CHAR_DATA *mob)
{
	char sex[8];
	sex[0]='\0';
	switch (mob->sex)
	{
		case 0:
			strcpy(sex,"yok");
			break;
		case 1:
			strcpy(sex,"erkek");
			break;
		case 2:
			strcpy(sex,"diþi");
			break;

	}

	printf_to_char(ch,"{c,---------------------------------------------------------------------,\n\r");
	printf_to_char(ch,"{c| {w%-12s                                 {c                       |\n\r",mob->short_descr);
	printf_to_char(ch,"{c|--------------------,------------------------------------------------|\n\r");
  printf_to_char(ch,"{c| Vnum    : {w%-8d{c | Güç: {w%-2d{c        | Delici : {w%-4d {c| Büyücü : %1s    |\n\r",mob->pIndexData->vnum, mob->perm_stat[STAT_STR],GET_AC(mob,AC_PIERCE), (IS_SET(mob->act,ACT_MAGE))?"{w+{c":"");
  printf_to_char(ch,"{c| Irk     : {w%-8s{c | Zek: {w%-2d{c        | Ezici  : {w%-4d {c| Ermiþ  : %1s    |\n\r",race_table[mob->race].name[1],mob->perm_stat[STAT_INT],GET_AC(mob,AC_BASH),(IS_SET(mob->act,ACT_CLERIC))?"{w+{c":"");
  printf_to_char(ch,"{c| Cinsiyet: {w%-8s{c | Bil: {w%-2d{c        | Kesici : {w%-4d {c| Savaþçý: %1s    |\n\r",sex,mob->perm_stat[STAT_WIS],GET_AC(mob,AC_SLASH),(IS_SET(mob->act,ACT_WARRIOR))?"{w+{c":"");
  printf_to_char(ch,"{c| Sýnýf   : {wmobil{c    | Çev: {w%-2d{c        | Egzotik: {w%-4d {c| Hýrsýz : %1s    |\n\r",mob->perm_stat[STAT_DEX],GET_AC(mob,AC_EXOTIC),(IS_SET(mob->act,ACT_THIEF))?"{w+{c":"");
  printf_to_char(ch,"{c| Yönelim : {w%-8d{c | Bün: {w%-2d{c        | ZZ     : {w%-4d {c|               |\n\r",mob->alignment,mob->perm_stat[STAT_CON],GET_DAMROLL(mob));
  printf_to_char(ch,"{c|                    | Kar: {w%-2d{c        | VZ     : {w%-4d {c|               |\n\r",mob->perm_stat[STAT_CHA],GET_HITROLL(mob));
	printf_to_char(ch,"{c|--------------------|----------------|---------------'---------------'{x\n\r");
	printf_to_char(ch,"{c| Yp    : {w%-5d/%-5d{c| Akçe : {w%-7ld {c|\n\r",mob->hit,  mob->max_hit,mob->silver);
	printf_to_char(ch,"{c| Mana  : {w%-5d/%-5d{c| Altýn: {w%-7ld {c|\n\r",mob->mana, mob->max_mana,mob->gold);
	printf_to_char(ch,"{c| Hp    : {w%-5d/%-5d{c| Beden: {w%-8d{c|\n\r",mob->move, mob->max_move,mob->size);
	printf_to_char(ch,"{c| Seviye: {w%-7ld{c    |                |\n\r",mob->level);
	printf_to_char(ch,"{c|--------------------'------------------'-----------------------------,{x\n\r");
  printf_to_char(ch,"{c| {wBaðýþýklýklar, Dayanýklýlýklar, Zayýflýklar{c                         |{x\n\r");
  printf_to_char(ch,"{c| {cteshir:%s%s%s {cçaðrý  :%s%s%s {cbüyü  :%s%s%s {csilah :%s%s%s {cezici :%s%s%s {cdelici  :%s%s%s{c|{x\n\r",(mob->imm_flags  & IMM_CHARM)?"{w+{x":"{D-{x",(mob->res_flags  & RES_CHARM)?"{w+{x":"{D-{x",(mob->vuln_flags  & VULN_CHARM)?"{w+{x":"{D-{x",(mob->imm_flags  & IMM_SUMMON)?"{w+{x":"{D-{x",(mob->res_flags  & RES_SUMMON)?"{w+{x":"{D-{x",(mob->vuln_flags  & VULN_SUMMON)?"{w+{x":"{D-{x",(mob->imm_flags  & IMM_MAGIC)?"{w+{x":"{D-{x",(mob->res_flags  & RES_MAGIC)?"{w+{x":"{D-{x",(mob->vuln_flags  & VULN_MAGIC)?"{w+{x":"{D-{x",(mob->imm_flags  & IMM_WEAPON)?"{w+{x":"{D-{x",(mob->res_flags  & RES_WEAPON)?"{w+{x":"{D-{x",(mob->vuln_flags  & VULN_WEAPON)?"{w+{x":"{D-{x",(mob->imm_flags  & IMM_BASH)?"{w+{x":"{D-{x",(mob->res_flags  & RES_BASH)?"{w+{x":"{D-{x",(mob->vuln_flags  & VULN_BASH)?"{w+{x":"{D-{x",(mob->imm_flags  & IMM_PIERCE)?"{w+{x":"{D-{x",(mob->res_flags  & RES_PIERCE)?"{w+{x":"{D-{x",(mob->vuln_flags  & VULN_PIERCE)?"{w+{x":"{D-{x");
  printf_to_char(ch,"{c| {ckesici:%s%s%s {cemici  :%s%s%s {cateþ  :%s%s%s {cayaz  :%s%s%s {cþimþek:%s%s%s {casit    :%s%s%s{c|{x\n\r",(mob->imm_flags  & IMM_SLASH)?"{w+{x":"{D-{x",(mob->res_flags  & RES_SLASH)?"{w+{x":"{D-{x",(mob->vuln_flags  & VULN_SLASH)?"{w+{x":"{D-{x",(mob->imm_flags  & IMM_DROWNING)?"{w+{x":"{D-{x",(mob->res_flags  & RES_DROWNING)?"{w+{x":"{D-{x",(mob->vuln_flags  & VULN_DROWNING)?"{w+{x":"{D-{x",(mob->imm_flags  & IMM_FIRE)?"{w+{x":"{D-{x",(mob->res_flags  & RES_FIRE)?"{w+{x":"{D-{x",(mob->vuln_flags  & VULN_FIRE)?"{w+{x":"{D-{x",(mob->imm_flags  & IMM_COLD)?"{w+{x":"{D-{x",(mob->res_flags  & RES_COLD)?"{w+{x":"{D-{x",(mob->vuln_flags  & VULN_COLD)?"{w+{x":"{D-{x",(mob->imm_flags  & IMM_LIGHTNING)?"{w+{x":"{D-{x",(mob->res_flags  & RES_LIGHTNING)?"{w+{x":"{D-{x",(mob->vuln_flags  & VULN_LIGHTNING)?"{w+{x":"{D-{x",(mob->imm_flags  & IMM_ACID)?"{w+{x":"{D-{x",(mob->res_flags  & RES_ACID)?"{w+{x":"{D-{x",(mob->vuln_flags  & VULN_ACID)?"{w+{x":"{D-{x");
  printf_to_char(ch,"{c| {czehir :%s%s%s {cnegatif:%s%s%s {ckutsal:%s%s%s {cenerji:%s%s%s {czihin :%s%s%s {chastalýk:%s%s%s{c|{x\n\r",(mob->imm_flags  & IMM_POISON)?"{w+{x":"{D-{x",(mob->res_flags  & RES_POISON)?"{w+{x":"{D-{x",(mob->vuln_flags  & VULN_POISON)?"{w+{x":"{D-{x",(mob->imm_flags  & IMM_NEGATIVE)?"{w+{x":"{D-{x",(mob->res_flags  & RES_NEGATIVE)?"{w+{x":"{D-{x",(mob->vuln_flags  & VULN_NEGATIVE)?"{w+{x":"{D-{x",(mob->imm_flags  & IMM_HOLY)?"{w+{x":"{D-{x",(mob->res_flags  & RES_HOLY)?"{w+{x":"{D-{x",(mob->vuln_flags  & VULN_HOLY)?"{w+{x":"{D-{x",(mob->imm_flags  & IMM_ENERGY)?"{w+{x":"{D-{x",(mob->res_flags  & RES_ENERGY)?"{w+{x":"{D-{x",(mob->vuln_flags  & VULN_ENERGY)?"{w+{x":"{D-{x",(mob->imm_flags  & IMM_MENTAL)?"{w+{x":"{D-{x",(mob->res_flags  & RES_MENTAL)?"{w+{x":"{D-{x",(mob->vuln_flags  & VULN_MENTAL)?"{w+{x":"{D-{x",(mob->imm_flags  & IMM_DISEASE)?"{w+{x":"{D-{x",(mob->res_flags  & RES_DISEASE)?"{w+{x":"{D-{x",(mob->vuln_flags  & VULN_DISEASE)?"{w+{x":"{D-{x");
  printf_to_char(ch,"{c| {cýþýk  :%s%s%s {cses    :%s%s%s {ctahta :%s%s%s {cgümüþ :%s%s%s {cdemir :%s%s%s             {c|{x\n\r",(mob->imm_flags  & IMM_LIGHT)?"{w+{x":"{D-{x",(mob->res_flags  & RES_LIGHT)?"{w+{x":"{D-{x",(mob->vuln_flags  & VULN_LIGHT)?"{w+{x":"{D-{x",(mob->imm_flags  & IMM_SOUND)?"{w+{x":"{D-{x",(mob->res_flags  & RES_SOUND)?"{w+{x":"{D-{x",(mob->vuln_flags  & VULN_SOUND)?"{w+{x":"{D-{x",(mob->imm_flags  & IMM_WOOD)?"{w+{x":"{D-{x",(mob->res_flags  & RES_WOOD)?"{w+{x":"{D-{x",(mob->vuln_flags  & VULN_WOOD)?"{w+{x":"{D-{x",(mob->imm_flags  & IMM_SILVER)?"{w+{x":"{D-{x",(mob->res_flags  & RES_SILVER)?"{w+{x":"{D-{x",(mob->vuln_flags  & VULN_SILVER)?"{w+{x":"{D-{x",(mob->imm_flags  & IMM_IRON)?"{w+{x":"{D-{x",(mob->res_flags  & RES_IRON)?"{w+{x":"{D-{x",(mob->vuln_flags  & VULN_IRON)?"{w+{x":"{D-{x");
  printf_to_char(ch,"{c'---------------------------------------------------------------------'{x\n\r");
}

char *	const	month_name	[] =
{
	(char*)"Albars",
	(char*)"Kadimler",
    (char*)"Büyük Acý",
	(char*)"Zeytin",
	(char*)"Yýlan",
	(char*)"Yelbüke",
	(char*)"Pusu",
	(char*)"Savaþ",
	(char*)"Albastý",
	(char*)"Gölge",
	(char*)"Kara Ölüm",
	(char*)"Alacakaranlýk",
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

	printf_to_char( ch , "Yýl  : %d\n\r", time_info.year );
	printf_to_char( ch , "Ay   : %s (%d. ay)\n\r", month_name[time_info.month-1] , time_info.month );
	printf_to_char( ch , "Gün  : %d\n\r", time_info.day );
	printf_to_char( ch , "Saat : %d\n\r", time_info.hour );

    if ( !IS_SET(ch->in_room->room_flags,ROOM_INDOORS) ||
         IS_IMMORTAL(ch) )
    {
      sprintf( buf, "$C %s vakti. $c",
         (time_info.hour>=4 && time_info.hour<6)? "þafak":
         (time_info.hour>=6 && time_info.hour<10)? "sabah":
         (time_info.hour>=10 && time_info.hour<16)? "öðlen":
         (time_info.hour>=16 && time_info.hour<20)? "akþam":
         "gece" );

      act_color( buf, ch, NULL, NULL, TO_CHAR, POS_RESTING,
         (time_info.hour>=4 && time_info.hour<6)? COLOR_DAWN:
         (time_info.hour>=6 && time_info.hour<10)? COLOR_MORNING:
         (time_info.hour>=10 && time_info.hour<16)? COLOR_DAY:
         (time_info.hour>=16 && time_info.hour<20)? COLOR_EVENING:
         COLOR_NIGHT, CLR_NORMAL );
		printf_to_char(ch,"\n\r\n\r");
    }
    if ( !IS_IMMORTAL( ch ) )
      return;
    sprintf(buf2, "%s", (char *) ctime( &boot_time ));
    sprintf(buf,"UD Mud %s tarihinde baþlatýldý.\n\rSistem zamaný, %s.\n\r",
	buf2, (char *) ctime( &current_time ) );
    send_to_char( buf, ch );

    return;
}



void do_weather( CHAR_DATA *ch, char *argument )
{

    static const char * sky_look[4] =
    {
      "Bulutsuz",
    	"Bulutlu",
    	"Yaðmurlu",
    	"Þimþekler çakýyor"
    };

    if ( !IS_OUTSIDE(ch) )
    {
      printf_to_char(ch, "Tavanýn ardýndaki gökyüzünü göremiyorsun.\n\r" );
	return;
    }

    printf_to_char( ch, "%s ve %s.",
	sky_look[weather_info.sky],
	weather_info.change >= 0
	? "ýlýk güney meltemi esiyor"
	: "soðuk kuzey borasý esiyor"
	);
    return;
}



void do_help( CHAR_DATA *ch, char *argument )
{
    HELP_DATA *pHelp;
    char argall[MAX_INPUT_LENGTH],argone[MAX_INPUT_LENGTH];

    if ( argument[0] == '\0' )
	argument = (char*)"summary";

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
		printf_to_char( ch, pHelp->keyword );
		printf_to_char( ch, "\n\r" );
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

    printf_to_char(ch, "Bu sözcükle alakalý yardým yok.\n\r" );
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
      printf_to_char(ch,"Bir isim belirtmelisin.\n\r");
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
        case MAX_LEVEL - 0 : iclass = "YRT"; break;
    		case MAX_LEVEL - 1 : iclass = "YRT";	break;
    		case MAX_LEVEL - 2 : iclass = "TNR";	break;
    		case MAX_LEVEL - 3 : iclass = "TNR";	break;
    		case MAX_LEVEL - 4 : iclass = "TNR";	break;
    		case MAX_LEVEL - 5 : iclass = "MLK";	break;
    		case MAX_LEVEL - 6 : iclass = "MLK";	break;
    		case MAX_LEVEL - 7 : iclass = "YML";	break;
    		case MAX_LEVEL - 8 : iclass = "YML";	break;
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
	  sprintf(cabalbuf, "[%s%s%s] ",
		CLR_CYAN,
		cabal_table[wch->cabal].short_name,
		CLR_WHITE_BOLD
		);
	}
	else cabalbuf[0] = '\0';
	if (wch->cabal == 0) cabalbuf[0] = '\0';

	pk_buf[0] = '\0';
	if (!((ch==wch && ch->level < KIDEMLI_OYUNCU_SEVIYESI) || is_safe_nomessage(ch,wch)))
	 {
	  sprintf(pk_buf,"%s(OK) %s",
		CLR_RED,CLR_WHITE_BOLD);
	 }
	act_buf[0] = '\0';
	sprintf(act_buf,"%s%s%s",CLR_WHITE,
		    IS_SET(wch->act, PLR_WANTED) ? "(ARANIYOR) " : "",CLR_WHITE_BOLD);

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
		sprintf(titlebuf,"%s%s%s",CLR_WHITE,tempbuf,CLR_WHITE_BOLD);
	     }
	/*
	 * Format it up.
	 */

	sprintf( level_buf, "%s%2d%s",CLR_CYAN,wch->level,CLR_WHITE_BOLD);
	sprintf(classbuf,"%s%s%s",CLR_YELLOW,iclass,CLR_WHITE_BOLD);
	    /* a little formatting */

	    if (IS_TRUSTED(ch,LEVEL_IMMORTAL) || ch==wch ||
                      wch->level >= LEVEL_HERO)

	      sprintf(buf, "[%2d %s %s] %s%s%s%s%s\n\r",
		      wch->level,
		      RACE(wch) < MAX_PC_RACE ?
		        race_table[RACE(wch)].who_name: "     ",
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
 		        race_table[RACE(wch)].who_name: "     ",
 		      ( (ch==wch && ch->level < KIDEMLI_OYUNCU_SEVIYESI) ||
 		        is_safe_nomessage(ch,wch) ) ?
 		       "" : "(OK) ",
 		      cabalbuf,
 		      IS_SET(wch->act,PLR_WANTED) ? "(ARANIYOR) " : "",
 		      wch->name,
		      titlebuf);

	    strcat(output,buf);
	}
    }

    if (!found)
    {
      printf_to_char(ch,"Bu isimde birini göremiyorum??\n\r");
	return;
    }

    page_to_char(output,ch);
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
        sprintf(buf,"Oyunda %d karakter var, bugünün rekoru.\n\r",
	    count);
    else
	sprintf(buf,"Oyunda %d karakter var, bugün en çok %d.\n\r",
	    count,max_on);

    send_to_char(buf,ch);
}

void do_inventory( CHAR_DATA *ch, char *argument )
{
    send_to_char( "Taþýdýklarýn:\n\r", ch );
    show_list_to_char( ch->carrying, ch, TRUE, TRUE );
    return;
}



void do_equipment( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    int iWear;
    bool found;

    send_to_char( "Ekipmanlarýn:\n\r", ch );
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
	send_to_char( "Hiçbir þey.\n\r", ch );

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
      printf_to_char(ch, "Neyi neyle kýyaslayacaksýn?\n\r" );
	return;
    }

    if ( ( obj1 = get_obj_carry( ch, arg1 ) ) == NULL )
    {
      printf_to_char(ch, "Ona sahip deðilsin.\n\r" );
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
    printf_to_char(ch,"Kýyaslanabilecek bir þey giymiyorsun.\n\r");
	    return;
	}
    }

    else if ( (obj2 = get_obj_carry(ch,arg2) ) == NULL )
    {
      printf_to_char(ch,"Ona sahip deðilsin.\n\r");
	return;
    }

    msg		= NULL;
    value1	= 0;
    value2	= 0;

    if ( obj1 == obj2 )
    {
	msg = (char*)"$p ile kendisini kýyaslýyorsun... Hmm, ayný gibiler.";
    }
    else if ( obj1->item_type != obj2->item_type )
    {
	msg = (char*)"$p ile $P kýyaslanamaz.";
    }
    else
    {
	switch ( obj1->item_type )
	{
	default:
	    msg = (char*)"$p ile $P kýyaslanamaz.";
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
	     if ( value1 == value2 ) msg = (char*)"$p ile $P ayný gibiler.";
	else if ( value1  > value2 ) msg = (char*)"$p daha iyi gibi.";
	else                         msg = (char*)"$p daha kötü gibi.";
    }

    act( msg, ch, obj1, obj2, TO_CHAR );
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
      send_to_char("Çok karanlýk.\n\r",ch);
      return;
    }

    if (!str_cmp(arg, "tüze"))
    {
      if (IS_SET(ch->in_room->area->area_flag, AREA_PROTECTED))
	send_to_char("Bu bölge Tüze tarafýndan korunuyor!\n\r",ch);
      else
	send_to_char("Bu bölge korunmuyor.\n\r",ch);
      return;
    }

    if (!str_cmp(arg,"ok"))
      fPKonly = TRUE;

    sprintf(pkbuf,"%s%s%s",CLR_RED,"(OK) ",CLR_WHITE_BOLD);

    if ( arg[0] == '\0' || fPKonly)
    {
	send_to_char( "Yakýnýndaki karakterler:\n\r", ch );
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
	    send_to_char( "Hiç\n\r", ch );
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
  act( "Hiç $T bulamadýn.", ch, NULL, arg, TO_CHAR );
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
      printf_to_char(ch, "Kimi tartacaksýn?\n\r" );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
      printf_to_char(ch, "Burada deðil.\n\r" );
	return;
    }

    if (is_safe(ch,victim))
    {
      printf_to_char(ch,"Bunu düþünme bile.\n\r");
	return;
    }

    diff = victim->level - ch->level;

     if ( diff <= -10 ) msg = (char*)"$G çýplak ve silahsýzken bile öldürürsün.";
else if ( diff <=  -5 ) msg = (char*)"$N sana antrenman bile yaptýramaz.";
else if ( diff <=  -2 ) msg = (char*)"$N kolayca öldürülebilir.";
else if ( diff <=   1 ) msg = (char*)"Tam diþine göre!";
else if ( diff <=   4 ) msg = (char*)"Kendini þanslý hissediyor musun?";
else if ( diff <=   9 ) msg = (char*)"$N acýmasýzca gülüyor.";
else                    msg = (char*)"Ýntihar günahtýr!";

    if (IS_EVIL(ch) && IS_EVIL(victim))
      align = (char*)"$N seninle birlikte kötücül þekilde sýrýtýyor.";
    else if (IS_GOOD(victim) && IS_GOOD(ch))
      align = (char*)"$N seni sýcak þekilde selamlýyor.";
    else if (IS_GOOD(victim) && IS_EVIL(ch))
      align = (char*)"$N sana gülümsüyor ve kem yoldan dönmen için dua ediyor.";
    else if (IS_EVIL(victim) && IS_GOOD(ch))
      align = (char*)"$N sana þeytani þekilde gülümsüyor.";
    else if (IS_NEUTRAL(ch) && IS_EVIL(victim))
      align = (char*)"$N yüzüne kötücül bir gülümseme yerleþtiriyor.";
    else if (IS_NEUTRAL(ch) && IS_GOOD(victim))
      align = (char*)"$N mutlulukla gülümsüyor.";
    else if (IS_NEUTRAL(ch) && IS_NEUTRAL(victim))
      align = (char*)"$N seninle ilgilenmiyor.";
    else
      align = (char*)"$N çok ilgisiz görünüyor.";

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


void do_titl( CHAR_DATA *ch, char *argument)
{
   printf_to_char( ch,"Lakabýný deðiþtirmek istiyorsan 'lakap' komutunu eksiksiz yazmalýsýn.\n\r" );
}

void do_title( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
        return;

    if ( CANT_CHANGE_TITLE(ch) )
	{
         send_to_char( "Lakabýný deðiþtiremezsin.\n\r", ch );
         return;
	}

  if ( argument[0] == '\0' )
  {
    printf_to_char(ch, "Ýsteðin üzerine lakabýn sýfýrlandý.\n\r");
    set_title( ch, (char*)"" );

      return;
  }

    if (!str_cmp(argument, "sýfýrla"))
    {
        char buf[MAX_STRING_LENGTH];
        sprintf(buf, ", %s", title_table[ch->iclass][ch->level]);
        set_title(ch, buf);
        printf_to_char(ch, "Lakabýn sýfýrlandý.\n\r");
        return;
    }

    if ( strlen(argument) > 45 )
        argument[45] = '\0';

    smash_tilde( argument );
    set_title( ch, argument );
    printf_to_char(ch, "Tamam.\n\r");
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
              printf_to_char(ch,"Çýkartýlacak satýr kalmadý.\n\r");
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
      printf_to_char(ch, "Tanýmýn:\n\r");
      printf_to_char(ch, ch->description ? ch->description :(char *)"(Hiç).\n\r");
                        return;
                    }
                }
            }
            buf[0] = '\0';
	    free_string(ch->description);
	    ch->description = str_dup(buf);
      printf_to_char(ch,"Taným silindi.\n\r");
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
    printf_to_char(ch, "Taným çok uzun.\n\r" );
	    return;
	}

	strcat( buf, argument );
	strcat( buf, "\n\r" );
	free_string( ch->description );
	ch->description = str_dup( buf );
  if (!IS_NPC(ch) && (strlen(ch->description)>=350) && (IS_SET(ch->act, PLR_NO_DESCRIPTION)))
    REMOVE_BIT(ch->act, PLR_NO_DESCRIPTION);
    if (!IS_NPC(ch) && (strlen(ch->description)<350) && (!IS_SET(ch->act, PLR_NO_DESCRIPTION)))
      SET_BIT(ch->act, PLR_NO_DESCRIPTION);
    }

    printf_to_char(ch, "Tanýmýn:\n\r");
    printf_to_char(ch, ch->description ? ch->description : (char *)"(Hiç).\n\r");
    return;
}



void do_report( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_INPUT_LENGTH];

    sprintf( buf, "%d/%d Yp %d/%d Mana %d/%d Hp %d Tp",
	ch->hit,  ch->max_hit,
	ch->mana, ch->max_mana,
	ch->move, ch->max_move,
	ch->exp   );
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

    if(ch->pcdata->learned[sn]<75)
    sprintf( buf, "{r%-18s %3d%%  {x",skill_table[sn].name[1], ch->pcdata->learned[sn] );
  else if(ch->pcdata->learned[sn]>=75 && ch->pcdata->learned[sn]<85)
    sprintf( buf, "{g%-18s %3d%%  {x",skill_table[sn].name[1], ch->pcdata->learned[sn] );
  else if(ch->pcdata->learned[sn]>=85 && ch->pcdata->learned[sn]<100)
    sprintf( buf, "{G%-18s %3d%%  {x",skill_table[sn].name[1], ch->pcdata->learned[sn] );
  else if(ch->pcdata->learned[sn]==100)
    sprintf( buf, "{C%-18s %3d%%  {x",skill_table[sn].name[1], ch->pcdata->learned[sn] );
  else
    sprintf( buf, "%-18s %3d%%  ",skill_table[sn].name[1], ch->pcdata->learned[sn] );
	    strcat( buf2, buf );
	    if ( ++col % 3 == 0 )
		strcat( buf2, "\n\r" );
	}

	if ( col % 3 != 0 )
	    strcat( buf2, "\n\r" );

	sprintf( buf, "%d pratik seansý var.\n\r",
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
	    send_to_char( "Rüyalarýnda mý?\n\r", ch );
	    return;
	}

	for ( mob = ch->in_room->people; mob != NULL; mob = mob->next_in_room )
	{
	    if ( IS_NPC(mob) && IS_SET(mob->act, ACT_PRACTICE) )
		break;
	}

	if ( mob == NULL )
	{
    printf_to_char(ch, "Burada yapamazsýn.\n\r" );
	    return;
	}

	if ( ch->practice <= 0 )
	{
    printf_to_char(ch, "Pratik seansýn yok.\n\r" );
	    return;
	}

	if ( ( sn = find_spell( ch,argument ) ) < 0
	|| ( !IS_NPC(ch)
	&&   (ch->level < skill_table[sn].skill_level[ch->iclass]
 	|| !RACE_OK(ch,sn) ||
(skill_table[sn].cabal != ch->cabal && skill_table[sn].cabal != CABAL_NONE) )))
	{
    printf_to_char(ch, "Onu pratik edemezsin.\n\r" );
	    return;
	}

	if (!str_cmp("vampir",skill_table[sn].name[1]) )
	{
	 send_to_char( "Bunu yalnýzca görevciden alabilirsin.\n\r",ch);
	 return;
	}

	adept = IS_NPC(ch) ? 100 : class_table[ch->iclass].skill_adept;

	if ( ch->pcdata->learned[sn] >= adept )
	{
    printf_to_char( ch, "%s konusunu çoktan öðrendin.\n\r",
  skill_table[sn].name[1] );
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
        act( "$T pratik ediyorsun.",
		    ch, NULL, skill_table[sn].name[1], TO_CHAR );
        act( "$n $T pratik ediyor.",
		    ch, NULL, skill_table[sn].name[1], TO_ROOM );
	    }
	    else
	    {
		ch->pcdata->learned[sn] = adept;
    act( "$T konusunu öðrendin.",
		    ch, NULL, skill_table[sn].name[1], TO_CHAR );
        act( "$n $T konusunu öðrendi.",
		    ch, NULL, skill_table[sn].name[1], TO_ROOM );
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

    char arg[MAX_INPUT_LENGTH];
    int wimpy;

    one_argument( argument, arg );

    if ((ch->iclass == 9) && (ch->level >=10))
	{
    printf_to_char( ch, "Korkaklýk pek sana göre deðil!\n\r" );

	 if (ch->wimpy != 0) ch->wimpy = 0;
	 return;
	}

    if ( arg[0] == '\0' )
	wimpy = ch->max_hit / 5;
    else  wimpy = atoi( arg );

    if ( wimpy < 0 )
    {
      printf_to_char(ch, "Cesaretin bilgeliðini aþýyor.\n\r" );
	return;
    }

    if ( wimpy > ch->max_hit/2 )
    {
      printf_to_char(ch, "Bu kadar korkaklýk seni hasta eder.\n\r" );
	return;
    }

    ch->wimpy	= wimpy;

    printf_to_char( ch, "Korkaklýk %d Yp'ye ayarlandý.\n\r", wimpy );
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
      printf_to_char(ch, "Yazým: þifre <eski> <yeni>.\n\r" );
	return;
    }

    if ( strcmp( crypt( arg1, ch->pcdata->pwd ), ch->pcdata->pwd ) )
    {
	WAIT_STATE( ch, 40 );
  printf_to_char(ch, "Yanlýþ þifre.  10 saniye bekle.\n\r" );
	return;
    }

    if ( strlen(arg2) < 5 )
    {
      printf_to_char(ch, "Yeni parola en az 5 karakter uzunluðunda olmalý.\n\r" );
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
    printf_to_char(ch,
  "Kabul edilemez bir parola, tekrar deneyin.\n\r" );
	    return;
	}
    }

    free_string( ch->pcdata->pwd );
    ch->pcdata->pwd = str_dup( pwdnew );
    save_char_obj( ch );
    send_to_char( "Tamam.\n\r", ch );
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
	do_scan2(ch,(char*)"");
	return;
   }

  switch (dir[0])
    {
    case 'K':    case 'k':      door = 0;      dir2 = (char*)"kuzey";	break;
    case 'D':    case 'd':      door = 1;      dir2 = (char*)"doðu";	break;
    case 'G':    case 'g':      door = 2;      dir2 = (char*)"güney";	break;
    case 'B':    case 'b':      door = 3;      dir2 = (char*)"batý";	break;
    case 'Y':    case 'y':      door = 4;      dir2 = (char*)"yukarý";	break;
    case 'A':    case 'a':      door = 5;      dir2 = (char*)"aþaðý";	break;
    default:
      send_to_char("Bu bir yön deðil.\n\r",ch);
      return;
    }

  sprintf(buf, "%s yönünü tarýyorsun.\n\r",dir2);
  send_to_char(buf,ch);
  sprintf(buf, "$n %s yönünü tarýyor.",dir2);
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
	  sprintf(buf, "***** Uzaklýk %d *****\n\r",i);
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
    send_to_char("Tekrar talep etmek için biraz bekle.\n\r",ch);
	 return;
	}


  argument = one_argument( argument, arg1 );
  argument = one_argument( argument, arg2 );

  if (IS_NPC(ch))
	return;

  if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
      send_to_char( "Kimden neyi talep edeceksin?\n\r", ch );
      return;
    }

  if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
    {
      send_to_char("Burada deðil.\n\r", ch );
      return;
    }

  if (!IS_NPC(victim))
    {
      send_to_char("Neden kendin sormuyorsun?\n\r", ch);
      return;
    }

  if (!IS_GOOD(ch))
    {
      do_say(victim, (char*)"Böyle ahlaksýz birine hiçbir þey vermem.");
      return;
    }

  if (ch->move < (50 + ch->level))
    {
      do_say(victim, (char*)"Yorgun görünüyorsun. Önce dinlenmelisin.");
      return;
    }

  WAIT_STATE(ch, PULSE_VIOLENCE);
  ch->move -= 10;
  ch->move = UMAX(ch->move, 0);

  if (victim->level >= ch->level + 10 || victim->level >= ch->level * 2)
    {
      do_say(victim, (char*)"Henüz erken, çocuðum.");
      return;
    }

  if (( ( obj = get_obj_carry(victim , arg1 ) ) == NULL
      && (obj = get_obj_wear(victim, arg1)) == NULL)
      || IS_SET(obj->extra_flags, ITEM_INVENTORY))
    {
      do_say(victim, (char*)"Üzgünüm o þeyden bende yok.");
      return;
    }

  if (!IS_GOOD(victim))
    {
      do_say(victim, (char*)"Sana birþey vermek zorunda deðilim!");
      do_murder(victim, ch->name);
      return;
    }

  if ( obj->wear_loc != WEAR_NONE )
    unequip_char(victim, obj);

  if ( !can_drop_obj( ch, obj ) )
    {
      do_say(victim, (char*)"Üzgünüm o lanetli þeyi kendimden ayýramýyorum.");
      return;
    }

  if ( ch->carry_number + get_obj_number( obj ) > can_carry_n( ch ) )
    {
      send_to_char("Ellerin dolu.\n\r", ch);
      return;
    }

  if ( ch->carry_weight + get_obj_weight( obj ) > can_carry_w( ch ) )
    {
      send_to_char( "Bu kadar aðýrlýk taþýyamazsýn.\n\r", ch);
      return;
    }

  if ( !can_see_obj( ch, obj ) )
    {
      act("O nesneyi görmüyorsun.", ch, NULL, victim, TO_CHAR );
      return;
    }

  obj_from_char( obj );
  obj_to_char( obj, ch );
  act( "$n $Z $p talep ediyor.", ch, obj, victim, TO_NOTVICT );
  act( "Sen $Z $p talep ediyorsun.",   ch, obj, victim, TO_CHAR    );
  act( "$n senden $p talep ediyor.", ch, obj, victim, TO_VICT    );



  if (IS_SET(obj->progtypes,OPROG_GIVE))
    (obj->pIndexData->oprogs->give_prog) (obj,ch,victim);

  if (IS_SET(victim->progtypes,MPROG_GIVE))
    (victim->pIndexData->mprogs->give_prog) (victim,ch,obj);



  ch->move -= (50 + ch->level);
  ch->move = UMAX(ch->move, 0);
  ch->hit -= 3 * (ch->level / 2);
  ch->hit = UMAX(ch->hit, 0);

  act("$S inancýna þükranlýk duyuyorsun.", ch, NULL, victim,TO_CHAR);
  send_to_char("Ve dünyadaki iyiliðin ýþýðýndan mutluluk duyuyorsun.\n\r",ch);

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

void do_detect_hidden( CHAR_DATA *ch, char *argument)
{
    AFFECT_DATA af;


  if (IS_NPC(ch) ||
      ch->level < skill_table[gsn_detect_hidden].skill_level[ch->iclass] )
    {
      send_to_char("Hý?\n\r", ch );
      return;
    }

    if ( CAN_DETECT(ch, DETECT_HIDDEN) )
    {
      send_to_char("Zaten olabildiðince tetiktesin. \n\r",ch);
	return;
    }
    if ( number_percent( ) > get_skill( ch, gsn_detect_hidden ) )  {
      send_to_char(
          "Dikkatle gölgelerin içine bakýyorsun, fakat birþey göremiyorsun.\n\r", ch );
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
    send_to_char( "Dikkatin artýyor.\n\r", ch );
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
      send_to_char( "Hý?\n\r", ch );
      return;
    }

    send_to_char("Ayýlarý yardýmýna çaðýrýyorsun.\n\r",ch);
    act("$n ayýlarý çaðýrýyor.",ch,NULL,NULL,TO_ROOM);

  if (is_affected(ch, gsn_bear_call))
    {
      send_to_char( "Onlarý kontrol edecek gücü nasýl çaðýracaksýn?\n\r", ch);
      return;
    }
  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  gch->pIndexData->vnum == MOB_VNUM_BEAR)
	{
    send_to_char( "Sahip olduðun ayýyla bir sorunun mu var?",ch);
	  return;
	}
    }

  if ( ch->in_room != NULL && IS_SET(ch->in_room->room_flags, ROOM_NO_MOB) )
  {
    send_to_char( "Ayýlar seni dinlemiyor.\n\r", ch );
     return;
  }

  if ( number_percent( ) > get_skill( ch, gsn_bear_call) )
  {
    send_to_char( "Ayýlar seni dinlemiyor.\n\r", ch );
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
    send_to_char( "Yardýmýna gelen ayý olmuyor.\n\r", ch );
    return;
  }

  if ( ch->mana < 125 )
  {
    send_to_char("Bu iþ için yeterli manan yok.\n\r", ch );
     return;
  }
  ch->mana -= 125;

  check_improve(ch,gsn_bear_call,TRUE,1);
  bear = create_mobile( get_mob_index(MOB_VNUM_BEAR), NULL );

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

  bear2 = create_mobile(bear->pIndexData, NULL);
  clone_mobile(bear,bear2);

  SET_BIT(bear->affected_by, AFF_CHARM);
  SET_BIT(bear2->affected_by, AFF_CHARM);
  bear->master = bear2->master = ch;
  bear->leader = bear2->leader = ch;

  char_to_room(bear,ch->in_room);
  char_to_room(bear2,ch->in_room);
  send_to_char( "Ýki ayý yardýmýna geliyor!\n\r",ch);
  act("$s yardýmýna iki ayý geliyor!",ch,NULL,NULL,TO_ROOM);

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
      send_to_char( "Bunu taþýmýyorsun.\n\r", ch );
       return;
    }

    for ( rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room )
       if (IS_NPC(rch) && rch->pIndexData->vnum == MOB_VNUM_SAGE)
          break;

    if (!rch)
    {
      send_to_char(  "Buralarda o þeyden anlayan yok.\n\r", ch);
       return;
    }

    if (IS_IMMORTAL(ch))
    act( "$n sana bakýyor!\n\r", rch, obj, ch, TO_VICT );
    else if (ch->gold < 1)
       {
         act( "$n $p'yi tanýmlamaya devam ediyor.",
               rch, obj, 0, TO_ROOM );
  	send_to_char(" En azýndan 1 altýnýn olmalý.\n\r", ch);
       return;
       }
    else
       {
       ch->gold -= 1;
       send_to_char("Para kesen hafifliyor.\n\r", ch);
       }

       act( "$n $p üzerine bilge bir bakýþ fýrlatýyor.", rch, obj, 0, TO_ROOM );
    spell_identify( 0, 0, ch, obj ,0);
}


void do_affects_col(CHAR_DATA *ch, char *argument )
{
    AFFECT_DATA *paf, *paf_last = NULL;
    char buf[MAX_STRING_LENGTH];
    bool found;

    found = FALSE;

    if ( ch->affected != NULL )
    {
      found = TRUE;
      send_to_char( "Þunlardan etkileniyorsun:\n\r", ch );
	for ( paf = ch->affected; paf != NULL; paf = paf->next )
	{
	    if (paf_last != NULL && paf->type == paf_last->type)
		if (ch->level >= 20 )
		    sprintf( buf, "                      ");
		else
		    continue;
	    else
      sprintf( buf, "%sBüyü%s: %s%-15s%s",
		CLR_RED,CLR_WHITE_BOLD,CLR_YELLOW,
		skill_table[paf->type].name[1] ,CLR_WHITE_BOLD);

	    send_to_char( buf, ch );

	    if ( ch->level >= 20 )
	    {
		sprintf( buf,
      ": %s%s%s yönünü %s%d%s deðiþtirir",
		    CLR_MAGENTA,affect_loc_name( paf->location ),CLR_WHITE_BOLD,
		    CLR_MAGENTA,paf->modifier,CLR_WHITE_BOLD);
		send_to_char( buf, ch );
		if ( paf->duration == -1 || paf->duration == -2)
    sprintf( buf, "%ssürekli%s" ,CLR_CYAN,CLR_WHITE);
		else
    sprintf( buf, "%s%d%s saat",
			CLR_MAGENTA,paf->duration ,CLR_WHITE_BOLD);
		send_to_char( buf, ch );
	    }
	    send_to_char( "\n\r", ch );
	    paf_last = paf;
	}
    }

    if (!IS_NPC(ch) && (IS_SET(ch->act,PLR_GHOST)))
    {
      found = TRUE;
      printf_to_char(ch,"{rHayalet{w: %d saat",ch->pcdata->ghost_mode_counter);
    }

    if (found == FALSE)
      send_to_char("Hiçbir þeyin etkisinde deðilsin.\n\r",ch);

    return;
}


void do_familya(CHAR_DATA *ch, char *argument )
{
	int sn,col;
	col    = 0;
	printf_to_char(ch,"Irklara iliþkin irfanýn:\n\r\n\r");
	for(sn=0;sn<MAX_RACE;sn++)
	{
		if(race_table[sn].name[1] == NULL)
			continue;
		if(str_cmp(race_table[sn].name[1],"unique"))
		{
			printf_to_char(ch,"%-18s %3d%%  ",race_table[sn].name[1], ch->pcdata->familya[sn]);
			if ( ++col % 3 == 0 )
				printf_to_char(ch, "\n\r" );
		}
	}
	if ( col % 3 != 0 )
		printf_to_char(ch, "\n\r" );
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

    send_to_char("Aslanlarý yardýmýna çaðýrýyorsun.\n\r",ch);
    act("$n aslanlarý yardýmýna çaðýrýyor.",ch,NULL,NULL,TO_ROOM);

  if (is_affected(ch, gsn_lion_call))
    {
      send_to_char( "Onlarý kontrol edecek gücü nasýl çaðýracaksýn?\n\r", ch);
      return;
    }
  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  gch->pIndexData->vnum == MOB_VNUM_LION)
	{
    send_to_char(  "Sahip olduðun aslanla bir sorunun mu var?",ch);
	  return;
	}
    }

  if ( ch->in_room != NULL && IS_SET(ch->in_room->room_flags, ROOM_NO_MOB) )
  {
    send_to_char(  "Hiçbir aslan seni dinleyemez.\n\r", ch );
     return;
  }

  if ( number_percent( ) > get_skill( ch, gsn_lion_call) )
  {
    send_to_char("Hiçbir aslan seni dinlemiyor.\n\r", ch );
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
    send_to_char("Hiçbir aslan yardýmýna gelmiyor.\n\r", ch );
    return;
  }

  if ( ch->mana < 125 )
  {
    send_to_char(  "Aslan çaðýrmak için yeterli manan yok.\n\r", ch );
     return;
  }
  ch->mana -= 125;

  bear = create_mobile( get_mob_index(MOB_VNUM_LION), NULL );

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

  bear2 = create_mobile(bear->pIndexData, NULL);
  clone_mobile(bear,bear2);

  SET_BIT(bear->affected_by, AFF_CHARM);
  SET_BIT(bear2->affected_by, AFF_CHARM);
  bear->master = bear2->master = ch;
  bear->leader = bear2->leader = ch;

  char_to_room(bear,ch->in_room);
  char_to_room(bear2,ch->in_room);
  send_to_char("Yardýmýna iki aslan geliyor!\n\r",ch);
  act("$s yardýmýna iki aslan geliyor!",ch,NULL,NULL,TO_ROOM);

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

 if      ( istat >  90 ) stat = (char*)"kusursuz";
 else if ( istat >= 80 ) stat = (char*)"iyi";
 else if ( istat >= 60 ) stat = (char*)"fena deðil";
 else if ( istat >= 40 ) stat = (char*)"vasat";
 else if ( istat >= 20 ) stat = (char*)"hasarlý";
 else                    stat = (char*)"kýrýlgan";

 return stat;
}

/* room affects */
void do_raffects(CHAR_DATA *ch, char *argument )
{
    AFFECT_DATA *paf, *paf_last = NULL;
    char buf[MAX_STRING_LENGTH];

    if (ch->in_room->affected != NULL )
    {
      send_to_char( "Oda aþaðýdaki büyülerden etkileniyor:\n\r", ch );
	for ( paf = ch->in_room->affected; paf != NULL; paf = paf->next )
	{
	    if (paf_last != NULL && paf->type == paf_last->type)
		if (ch->level >= 20 )
		    sprintf( buf, "                      ");
		else
		    continue;
	    else
      sprintf( buf, "Büyü: %-15s", skill_table[paf->type].name[1] );

	    send_to_char( buf, ch );

	    if ( ch->level >= 20 )
	    {
		sprintf( buf,
      ": %s yönünü %d deðiþtirir ",
		    raffect_loc_name( paf->location ),
		    paf->modifier);
		send_to_char( buf, ch );
		if ( paf->duration == -1 || paf->duration == -2 )
    sprintf( buf, "sürekli" );
		else
    sprintf( buf, "%d saat", paf->duration );
		send_to_char( buf, ch );
	    }
	    send_to_char( "\n\r", ch );
	    paf_last = paf;
	}
    }
    else
    send_to_char("Oda hiçbir büyünün etkisinde deðil.\n\r",ch);

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
		skill_table[sn].name[1], ch->pcdata->learned[sn] );
	    strcat( buf2, buf );
	    if ( ++col % 3 == 0 )
		strcat( buf2, "\n\r" );
	}

	if ( col % 3 != 0 )
	    strcat( buf2, "\n\r" );

      sprintf( buf, "%d pratik seansýn kaldý.\n\r",
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
    send_to_char("Rüyanda mý?\n\r", ch );
	    return;
	}

	if ( ch->practice <= 0 )
	{
    send_to_char("Pratik seansýn yok.\n\r", ch );
	    return;
	}

	if ( ( sn = find_spell( ch,argument ) ) < 0
	|| ( !IS_NPC(ch)
	&&   (ch->level < skill_table[sn].skill_level[ch->iclass]
 	|| !RACE_OK(ch,sn) ||
(skill_table[sn].cabal != ch->cabal && skill_table[sn].cabal != CABAL_NONE) )))
	{
    send_to_char("Onu pratik edemezsin.\n\r", ch );
	    return;
	}

	if (!str_cmp("vampir",skill_table[sn].name[1]) )
	{
    send_to_char( "Yalnýz görevci sana yardým edebilir.\n\r",ch);
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
    send_to_char( "Burada yapamazsýn. Daha fazla bilgi için glist ve slook komutlarýný kullan.\n\r", ch );
	    return;
	}

	adept = IS_NPC(ch) ? 100 : class_table[ch->iclass].skill_adept;

	if ( ch->pcdata->learned[sn] >= adept )
	{
    sprintf( buf, "Zaten %s konusunu öðrendin.\n\r",
		skill_table[sn].name[1] );
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
        act( "$T pratik ediyorsun.",
		    ch, NULL, skill_table[sn].name, TO_CHAR );
        act( "$n $T pratik ediyor.",
		    ch, NULL, skill_table[sn].name, TO_ROOM );
	    }
	    else
	    {
		ch->pcdata->learned[sn] = adept;
    act("$T konusunu öðrendin.",
		    ch, NULL, skill_table[sn].name, TO_CHAR );
        act("$n $T konusunu öðrendi.",
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

	if (!str_cmp(arg,"tüze"))
	  {
	    if (ch->cabal != CABAL_RULER && !IS_IMMORTAL(ch))
	      {
		send_to_char("Bu kabala üye deðilsin!\n\r",ch);
		return;
	      }
	    else
	      {
		fRulerRestrict = TRUE;
		break;
	      }
	  }
	if (!str_cmp(arg,"týlsým"))
	  {
	    if (ch->cabal != CABAL_SHALAFI && !IS_IMMORTAL(ch))
	      {
		send_to_char("Bu kabala üye deðilsin!\n\r",ch);
		return;
	      }
	    else
	      {
		fShalafiRestrict = TRUE;
		break;
	      }
	  }
	if (!str_cmp(arg,"öfke"))
	  {
	    if (ch->cabal != CABAL_BATTLE && !IS_IMMORTAL(ch))
	      {
		send_to_char("Bu kabala üye deðilsin!\n\r",ch);
		return;
	      }
	    else
	      {
		fBattleRestrict = TRUE;
		break;
	      }
	  }
	if (!str_cmp(arg,"istila"))
	  {
	    if (ch->cabal != CABAL_INVADER && !IS_IMMORTAL(ch))
	      {
		send_to_char("Bu kabala üye deðilsin!\n\r",ch);
		return;
	      }
	    else
	      {
		fInvaderRestrict = TRUE;
		break;
	      }
	  }
	if (!str_cmp(arg,"kaos"))
	  {
	    if (ch->cabal != CABAL_CHAOS && !IS_IMMORTAL(ch))
	      {
		send_to_char("Bu kabala üye deðilsin!\n\r",ch);
		return;
	      }
	    else
	      {
		fChaosRestrict = TRUE;
		break;
	      }
	  }
	if (!str_cmp(arg,"þövalye"))
	  {
	    if (ch->cabal != CABAL_KNIGHT && !IS_IMMORTAL(ch))
	      {
		send_to_char("Bu kabala üye deðilsin!\n\r",ch);
		return;
	      }
	    else
	      {
		fKnightRestrict = TRUE;
		break;
	      }
	  }
	if (!str_cmp(arg,"aslan"))
	  {
	    if (ch->cabal != CABAL_LIONS && !IS_IMMORTAL(ch))
	      {
		send_to_char("Bu kabala üye deðilsin!\n\r",ch);
		return;
	      }
	    else
	      {
		fLionsRestrict = TRUE;
		break;
	      }
	  }
	if (!str_cmp(arg,"dövme"))
	  {
	    if (get_eq_char(ch,WEAR_TATTOO) == NULL)
	      {
		send_to_char("Henüz dövmen yok!\n\r",ch);
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
		send_to_char( "Bu özellik ölümsüzler içindir.\n\r",ch);
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
		      send_to_char("Geçerli bir ýrk deðil.\n\r",ch);
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
              case MAX_LEVEL - 0 : iclass = "YRT";     break;
              case MAX_LEVEL - 1 : iclass = "YRT";     break;
              case MAX_LEVEL - 2 : iclass = "TNR";     break;
              case MAX_LEVEL - 3 : iclass = "TNR";     break;
              case MAX_LEVEL - 4 : iclass = "TNR";     break;
              case MAX_LEVEL - 5 : iclass = "MLK";     break;
              case MAX_LEVEL - 6 : iclass = "MLK";     break;
              case MAX_LEVEL - 7 : iclass = "YML";     break;
              case MAX_LEVEL - 8 : iclass = "YML";     break;
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
	if (!((ch==wch && ch->level< KIDEMLI_OYUNCU_SEVIYESI ) || is_safe_nomessage(ch,wch)))
	  sprintf(pk_buf,"%s(OK) %s", CLR_RED , CLR_WHITE_BOLD );

	act_buf[0] = '\0';
	sprintf(act_buf,"%s%s%s", CLR_WHITE ,
		    IS_SET(wch->act, PLR_WANTED) ? "(ARANIYOR) " : "",
	        CLR_WHITE_BOLD );

	if (IS_NPC(wch))
		sprintf(titlebuf,"Tanrýlara inanan.");
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

	sprintf( level_buf, "%s%3d%s", CLR_CYAN,wch->level, CLR_WHITE_BOLD);
	sprintf(classbuf,"%s%s%s",CLR_YELLOW,iclass, CLR_WHITE_BOLD);

	if (IS_TRUSTED(ch,LEVEL_IMMORTAL) || ch==wch ||
                   wch->level >= LEVEL_HERO)

	  sprintf( buf, "[%3d %8s %3s] %s%s%s%s%s\n\r",
	    wch->level,
	    RACE(wch) < MAX_PC_RACE ? race_table[RACE(wch)].who_name
				    : "     ",
	    classbuf,
	    pk_buf,
	    cabalbuf,
	    act_buf,
	    wch->name,
	    titlebuf);

	else
/*	  sprintf( buf, "[%s %s %s] %s%s%s%s%s\n\r",	*/
	  sprintf( buf, "[%3s %8s    ] %s%s%s%s%s\n\r",
		(get_curr_stat(wch, STAT_CHA) < 18 ) ? level_buf : "  ",
	    RACE(wch) < MAX_PC_RACE ? race_table[RACE(wch)].who_name
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
    sprintf( buf2, "\n\rOyuncular: %d, bugün: %d, en çok:%d.\n\r",
		nMatch,max_on,max_on_so_far );
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
      send_to_char( "Hý?\n\r", ch );
      return;
    }

  if (is_affected(ch, gsn_camp))
    {
      send_to_char("Yeni kamp bölgeleri kurmak için yeterli enerjin yok.\n\r", ch);
      return;
    }


  if ( number_percent( ) > get_skill( ch, gsn_camp) )
  {
    send_to_char("Kamp kurmayý baþaramadýn.\n\r", ch );
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
    send_to_char( "Kamp kurmak için yeterli yaprak yok.\n\r", ch );
    return;
  }

  if ( ch->mana < 150 )
  {
    send_to_char(  "Kamp kurmak için yeterli manan yok.\n\r", ch );
     return;
  }

  check_improve(ch,gsn_camp,TRUE,4);
  ch->mana -= 150;

  WAIT_STATE(ch,skill_table[gsn_camp].beats);

  send_to_char("Kamp kurmayý baþardýn.\n\r",ch);
  act("$n kamp kurmayý baþardý.",ch,NULL,NULL,TO_ROOM);

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
      send_to_char("Bunu yapamazsýn.\n\r", ch);
      return;
    }

  if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
      send_to_char( "Kimden neyi talep edeceksin?\n\r", ch );
      return;
    }

  if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
    {
      send_to_char( "Burada deðil.\n\r", ch );
      return;
    }

  if (!IS_NPC(victim))
    {
      send_to_char( "Neden ondan istemeyi denemiyorsun?\n\r", ch);
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
    do_say(victim,(char*)"Sana birþey vermek zorunda deðilim.");
         do_murder(victim, ch->name);
	 return;
	}

  if (( ( obj = get_obj_carry(victim , arg1 ) ) == NULL
      && (obj = get_obj_wear(victim, arg1)) == NULL)
      || IS_SET(obj->extra_flags, ITEM_INVENTORY))
    {
      do_say(victim,(char*)"Üzgünüm, o þeyden bende yok.");
      return;
    }


  if ( obj->wear_loc != WEAR_NONE )
    unequip_char(victim, obj);

  if ( !can_drop_obj( ch, obj ) )
    {
      do_say(victim,
	(char*)"O lanetli, kendimden ayýramýyorum. Affedin, efendim.");
      return;
    }

  if ( ch->carry_number + get_obj_number( obj ) > can_carry_n( ch ) )
    {
      send_to_char( "Ellerin dolu.\n\r", ch);
      return;
    }

  if ( ch->carry_weight + get_obj_weight( obj ) > can_carry_w( ch ) )
    {
      send_to_char( "O kadar aðýrlýk taþýyamazsýn.\n\r", ch);
      return;
    }

  if ( !can_see_obj( ch, obj ) )
    {
      act( "Onu görmüyorsun.", ch, NULL, victim, TO_CHAR );
      return;
    }

  obj_from_char( obj );
  obj_to_char( obj, ch );
  act( "$n $Z $p talep ediyor.", ch, obj, victim, TO_NOTVICT );
  act( "Sen $Z $p talep ediyorsun.",   ch, obj, victim, TO_CHAR    );
  act( "$n senden $p talep ediyor.", ch, obj, victim, TO_VICT    );



  if (IS_SET(obj->progtypes,OPROG_GIVE))
    (obj->pIndexData->oprogs->give_prog) (obj,ch,victim);

  if (IS_SET(victim->progtypes,MPROG_GIVE))
    (victim->pIndexData->mprogs->give_prog) (victim,ch,obj);

    send_to_char( "Gücün arzýn ürpermesine neden oluyor.\n\r",ch);

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
      send_to_char("Hý?\n\r", ch );
      return;
    }

  if ( arg[0] == '\0' )
    {
      send_to_char("Neyi, kimi?\n\r", ch );
      return;
    }

  if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
      send_to_char("Burada deðil.\n\r", ch );
      return;
    }

  if (race_table[ORG_RACE(victim)].pc_race)
    {
      send_to_char("Bunu moblar üzerinde denemelisin.\n\r", ch);
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
	 do_say(victim,(char*)"Seni izlemek istemiyorum.");
         do_murder(victim, ch->name);
	 return;
	}

  check_improve(ch,gsn_control_animal,TRUE,2);

  if ( victim->master )
	stop_follower( victim );
  SET_BIT(victim->affected_by,AFF_CHARM);
  victim->master = victim->leader = ch;

  act( "$n sence de þirin deðil mi?", ch, NULL, victim, TO_VICT );
  if ( ch != victim )
  act("$N etkilenmiþ gözlerle sana bakýyor.",ch,NULL,victim,TO_CHAR);

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
      send_to_char("Ok yapmayý bilmiyorsun.\n\r", ch );
      return;
    }

  if ( ch->in_room->sector_type != SECT_FIELD &&
       ch->in_room->sector_type != SECT_FOREST &&
       ch->in_room->sector_type != SECT_HILLS )
  {
    send_to_char( "Yeterince odun bulamadýn.\n\r", ch );
    return;
  }

  mana = skill_table[gsn_make_arrow].min_mana;
  wait = skill_table[gsn_make_arrow].beats;

  argument = one_argument(argument, arg);
  if (arg[0] == '\0') color = 0;
  else if (!str_prefix(arg,"yeþil")) color = gsn_green_arrow;
  else if (!str_prefix(arg,"kýzýl")) color = gsn_red_arrow;
  else if (!str_prefix(arg,"beyaz")) color = gsn_white_arrow;
  else if (!str_prefix(arg,"mavi")) color = gsn_blue_arrow;
  else
  {
    send_to_char(  "Bu tür bir ok yapmayý bilmiyorsun.\n\r", ch );
     return;
  }

  if (color != 0 )
  {
   mana += skill_table[color].min_mana;
   wait += skill_table[color].beats;
  }

  if ( ch->mana < mana )
  {
    send_to_char("Bu tür bir ok yapmak için yeterince enerjin yok.\n\r", ch );
     return;
  }
  ch->mana -= mana;
  WAIT_STATE(ch,wait);

  send_to_char("Ok yapmaya baþlýyorsun!\n\r",ch);
  act("$n ok yapmaya baþladý!",ch,NULL,NULL,TO_ROOM);
  for(count=0; count < (ch->level/5); count++)
  {
   if ( number_percent( ) > get_skill( ch, gsn_make_arrow) )
   {
     send_to_char("Yapmaya çalýþtýðýn oku kýrdýn.\n\r", ch );
	check_improve(ch,gsn_make_arrow,FALSE,3);
	continue;
   }
   send_to_char( "Ok yaptýn.\n\r",ch );
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
	 str = (char*)"yeþil";
	}
     else if (color == gsn_red_arrow)
	{
	 saf.bitvector	= WEAPON_FLAMING;
	 str = (char*)"kýzýl";
	}
     else if (color == gsn_white_arrow)
	{
	 saf.bitvector	= WEAPON_FROST;
	 str = (char*)"beyaz";
	}
     else
	{
	 saf.bitvector	= WEAPON_SHOCKING;
	 str = (char*)"mavi";
	}
   }
   else str = (char*)"tahta";

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
      send_to_char("Yay yapmayý bilmiyorsun.\n\r", ch );
      return;
    }

  if ( ch->in_room->sector_type != SECT_FIELD &&
       ch->in_room->sector_type != SECT_FOREST &&
       ch->in_room->sector_type != SECT_HILLS )
  {
    send_to_char("Yeterince odun bulamadýn.\n\r", ch );
    return;
  }

  mana = skill_table[gsn_make_bow].min_mana;
  wait = skill_table[gsn_make_bow].beats;

  if ( ch->mana < mana )
  {
    send_to_char( "Yay yapmak için yeterli enerjin yok.\n\r", ch);
     return;
  }
  ch->mana -= mana;
  WAIT_STATE(ch,wait);

  if ( number_percent( ) > get_skill( ch, gsn_make_bow) )
   {
     send_to_char( "Yapmaya çalýþtýðýn yayý kýrdýn.\n\r", ch );
	check_improve(ch,gsn_make_bow,FALSE,1);
	return;
   }
   send_to_char("Yay yaptýn.\n\r",ch );
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
    send_to_char("Ok ya da yay yapabilirsin.\n\r",ch);
   return;
  }

  if (!str_prefix(arg,"ok")) do_make_arrow(ch,argument);
  else if (!str_prefix(arg,"yay")) do_make_bow(ch,argument);
  else do_make(ch,(char*)"");
}


void do_nocancel(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;

    if (IS_SET(ch->act,PLR_NOCANCEL))
    {
      send_to_char("Baþkalarýnýn iptal büyülerini kabul ediyorsun.\n\r",ch);
      REMOVE_BIT(ch->act,PLR_NOCANCEL);
    }
    else
    {
      send_to_char("Baþkalarýnýn iptal büyülerini kabul etmiyorsun.\n\r",ch);
      SET_BIT(ch->act,PLR_NOCANCEL);
    }
}


void do_diril( CHAR_DATA *ch, char *argument )
{
  if(IS_NPC(ch))
  {
    return;
  }

  if (IS_SET(ch->act, PLR_GHOST))
  {
    REMOVE_BIT(ch->act,PLR_GHOST);
    printf_to_char(ch,"Ete kemiðe büründüðünü hissediyorsun. Arkaný kollamaya baþlasan iyi olur!\n\r");
    act ("$n ete kemiðe bürünüyor!",ch,NULL,NULL,TO_ROOM);
  }
  else
  {
    printf_to_char(ch,"Zaten oldukça dirisin!\n\r");
  }

  return;
}
