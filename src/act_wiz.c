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
#include <sys/time.h>
#endif
#include <time.h>
#include <stdio.h>
#include <wchar.h>
#include <wctype.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "merc.h"
#include "recycle.h"
#include "tables.h"
#include "lookup.h"

int unlink(const wchar_t *pathname);

/* command procedures needed */
DECLARE_DO_FUN(do_rstat		);
DECLARE_DO_FUN(do_mstat		);
DECLARE_DO_FUN(do_ostat		);
DECLARE_DO_FUN(do_rset		);
DECLARE_DO_FUN(do_mset		);
DECLARE_DO_FUN(do_oset		);
DECLARE_DO_FUN(do_sset		);
DECLARE_DO_FUN(do_mfind		);
DECLARE_DO_FUN(do_ofind		);
DECLARE_DO_FUN(do_slookup	);
DECLARE_DO_FUN(do_mload		);
DECLARE_DO_FUN(do_oload		);
DECLARE_DO_FUN(do_force		);
DECLARE_DO_FUN(do_quit		);
DECLARE_DO_FUN(do_save		);
DECLARE_DO_FUN(do_look		);
DECLARE_DO_FUN(do_force		);
DECLARE_DO_FUN(do_stand		);
DECLARE_DO_FUN(do_grant         );
DECLARE_DO_FUN(do_limited       );
DECLARE_DO_FUN(do_help	        );
DECLARE_DO_FUN(do_wear	        );

extern int max_on;

/*
 * Local functions.
 */
ROOM_INDEX_DATA *	find_location	args( ( CHAR_DATA *ch, wchar_t *arg ) );
bool write_to_descriptor  args(( int desc, wchar_t *txt, int length ));
void update_total_played        args( ( CHAR_DATA *ch) );

void do_cabal_scan( CHAR_DATA *ch, wchar_t *argument )
{
 int i;
 wchar_t buf1[MAX_INPUT_LENGTH];
 wchar_t buf2[MAX_INPUT_LENGTH];
 OBJ_DATA *in_obj;
 int show;

 if ( !IS_IMMORTAL(ch) &&
	(ch->cabal == CABAL_NONE || IS_NPC(ch)) )
 {
    send_to_char( L"You are not a cabal member yet.\n\r", ch);
    return;
 }

  for(i=1;i<MAX_CABAL;i++)
  {
    if (IS_IMMORTAL(ch) || ch->cabal == i)
	show = 1;
    else show = 0;
    swprintf(buf1, MAX_INPUT_LENGTH-1, L" Cabal: %-11s, room %4d, item %4d, ptr: %-20s ",
	cabal_table[i].short_name,
        cabal_table[i].room_vnum,
        cabal_table[i].obj_vnum,
	cabal_table[i].obj_ptr != NULL ?
	cabal_table[i].obj_ptr->short_descr : L"(NULL)" );
    if ( cabal_table[i].obj_ptr!=NULL )
    {
	for ( in_obj = cabal_table[i].obj_ptr;
		in_obj->in_obj != NULL; in_obj = in_obj->in_obj )
		;
	if (in_obj->carried_by)
	 swprintf(buf2, MAX_INPUT_LENGTH-1, L"\n\r\t\tcarried_by: %s\n\r",PERS(in_obj->carried_by,ch));
	else
	{
	 swprintf(buf2, MAX_INPUT_LENGTH-1, L"\n\r\t\t\t\t\tin_room: %s\n\r", in_obj->in_room != NULL ?
		in_obj->in_room->name : L"BUG!!" );
	 if (in_obj->in_room != NULL
		&& in_obj->in_room->vnum == cabal_table[ch->cabal].room_vnum)
	    show = 1;
	}
    }

    if (show)
    {
	send_to_char(buf1, ch);
	send_to_char(buf2, ch);
    }
  }
  return;
}

void do_objlist( CHAR_DATA *ch, wchar_t *argument )
{
FILE *fp;
OBJ_DATA *obj;
AFFECT_DATA *paf;

   if ( (fp=fopen( "objlist.txt", "w+" ) ) == NULL )
   {
        send_to_char( L"File error.\n\r", ch );
        return;
   }

   for( obj=object_list; obj!=NULL; obj = obj->next )
   {
     if ( CAN_WEAR( obj, ITEM_WIELD )
	&& (obj->level < 25 && obj->level > 15) )
     {
       fwprintf( fp, L"\n#Obj: %s (Vnum : %d) \n", obj->short_descr ,obj->pIndexData->vnum);
    fwprintf( fp, L"Object '%s' is type %s, extra flags %s.\nWeight is %d, value is %d, level is %d.\n",

	obj->name,
	item_type_name( obj ),
	extra_bit_name( obj->extra_flags ),
	obj->weight / 10,
	obj->cost,
	obj->level
	);

    switch ( obj->item_type )
    {
    case ITEM_SCROLL:
    case ITEM_POTION:
    case ITEM_PILL:
	fwprintf( fp, L"Level %d spells of:", obj->value[0] );

	if ( obj->value[1] >= 0 && obj->value[1] < MAX_SKILL )
	{
	    fwprintf(fp, L" '%s'", skill_table[obj->value[1]].name[1]);
	}

	if ( obj->value[2] >= 0 && obj->value[2] < MAX_SKILL )
	{
	    fwprintf(fp, L" '%s'", skill_table[obj->value[2]].name[1]);
	}

	if ( obj->value[3] >= 0 && obj->value[3] < MAX_SKILL )
	{
	    fwprintf(fp, L" '%s'", skill_table[obj->value[3]].name[1]);
	}

	if (obj->value[4] >= 0 && obj->value[4] < MAX_SKILL)
	{
	    fwprintf(fp, L" '%s'", skill_table[obj->value[4]].name[1]);
	}

	fwprintf( fp, L".\n");
	break;

    case ITEM_WAND:
    case ITEM_STAFF:
	fwprintf(fp, L"Has %d charges of level %d", obj->value[2], obj->value[0]);

	if ( obj->value[3] >= 0 && obj->value[3] < MAX_SKILL )
	{
	    fwprintf(fp, L" '%s'", skill_table[obj->value[3]].name[1]);
	}

	fwprintf( fp, L".\n");
	break;

    case ITEM_DRINK_CON:
        fwprintf(fp, L"It holds %s-colored %s.\n",
	    liq_table[obj->value[2]].liq_color,
            liq_table[obj->value[2]].liq_name);
        break;

    case ITEM_CONTAINER:
	fwprintf(fp, L"Capacity: %d#  Maximum weight: %d#  flags: %s\n",
	    obj->value[0], obj->value[3], cont_bit_name(obj->value[1]));
	if (obj->value[4] != 100)
	{
	    fwprintf(fp, L"Weight multiplier: %d%%\n",
		obj->value[4]);
	}
	break;

    case ITEM_WEAPON:
 	fwprintf(fp, L"Weapon type is ");
	switch (obj->value[0])
	{
	    case(WEAPON_EXOTIC) : fwprintf(fp, L"exotic.\n");	break;
	    case(WEAPON_SWORD)  : fwprintf(fp, L"sword.\n");	break;
	    case(WEAPON_DAGGER) : fwprintf(fp, L"dagger.\n");	break;
	    case(WEAPON_SPEAR)	: fwprintf(fp, L"spear/staff.\n");	break;
	    case(WEAPON_MACE) 	: fwprintf(fp, L"mace/club.\n");	break;
	    case(WEAPON_AXE)	: fwprintf(fp, L"axe.\n");		break;
	    case(WEAPON_FLAIL)	: fwprintf(fp, L"flail.\n");	break;
	    case(WEAPON_WHIP)	: fwprintf(fp, L"whip.\n");	break;
	    case(WEAPON_POLEARM): fwprintf(fp, L"polearm.\n");	break;
	    case(WEAPON_BOW)	: fwprintf(fp, L"bow.\n");		break;
	    case(WEAPON_ARROW)	: fwprintf(fp, L"arrow.\n");	break;
	    case(WEAPON_LANCE)	: fwprintf(fp, L"lance.\n");	break;
	    default		: fwprintf(fp, L"unknown.\n");	break;
 	}
	if (obj->pIndexData->new_format)
	    fwprintf(fp, L"Damage is %dd%d (average %d).\n",
		obj->value[1],obj->value[2],
		(1 + obj->value[2]) * obj->value[1] / 2);
	else
	    fwprintf( fp, L"Damage is %d to %d (average %d).\n",
	    	obj->value[1], obj->value[2],
	    	( obj->value[1] + obj->value[2] ) / 2 );
        if (obj->value[4])  /* weapon flags */
        {
            fwprintf(fp, L"Weapons flags: %s\n",weapon_bit_name(obj->value[4]));
	}
	break;

    case ITEM_ARMOR:
	fwprintf( fp, L"Armor class is %d pierce, %d bash, %d slash, and %d vs. magic.\n",
	    obj->value[0], obj->value[1], obj->value[2], obj->value[3] );
	break;
    }
       for( paf=obj->pIndexData->affected; paf != NULL; paf = paf->next )
       {
            if ( paf == NULL ) continue;
            fwprintf( fp, L"  Affects %s by %d.\n",
                affect_loc_name( paf->location ), paf->modifier );
            if (paf->bitvector)
            {
                switch(paf->where)
                {
                    case TO_AFFECTS:
                        fwprintf(fp, L"   Adds %s affect.\n",
                            affect_bit_name(paf->bitvector));
                        break;
                    case TO_OBJECT:
                        fwprintf(fp, L"   Adds %s object flag.\n",
                            extra_bit_name(paf->bitvector));
                        break;
                    case TO_IMMUNE:
                        fwprintf(fp, L"   Adds immunity to %s.\n",
                            imm_bit_name(paf->bitvector));
                        break;
                    case TO_RESIST:
                        fwprintf(fp, L"   Adds resistance to %s.\n\r",
                            imm_bit_name(paf->bitvector));
                        break;
                    case TO_VULN:
                        fwprintf(fp, L"   Adds vulnerability to %s.\n\r",
                            imm_bit_name(paf->bitvector));
                        break;
                    case TO_DETECTS:
                        fwprintf(fp, L"   Adds %s detection.\n\r",
                            detect_bit_name(paf->bitvector));
                        break;
                    default:
                        fwprintf(fp, L"   Unknown bit %d: %d\n\r",
                            paf->where,paf->bitvector);
                        break;
                }
            }
       }
     }
   }
   fclose( fp );
   return;
}

void do_limited( CHAR_DATA *ch, wchar_t *argument )
{
  extern int top_obj_index;
  OBJ_DATA *obj;
  OBJ_INDEX_DATA *obj_index;
  wchar_t  buf[MAX_STRING_LENGTH];
  wchar_t  output[4 * MAX_STRING_LENGTH];
  int	lCount = 0;
  int	ingameCount;
  int 	nMatch;
  int 	vnum;

  if ( argument[0] != '\0' )
  {
    obj_index = get_obj_index( wcstol(argument, 0, 10 ) );
    if ( obj_index == NULL )  {
      send_to_char( L"Not found.\n\r", ch);
      return;
    }
    if ( obj_index->limit == -1 )  {
      send_to_char( L"Thats not a limited item.\n\r", ch );
      return;
    }
    nMatch = 0;
    swprintf( buf, MAX_STRING_LENGTH-1, L"%-35s [%5d]  Limit: %3d  Current: %3d\n\r",
		   obj_index->short_descr,
		   obj_index->vnum,
	           obj_index->limit,
		   obj_index->count );
    buf[0] = towupper( buf[0] );
    send_to_char( buf, ch );
    ingameCount = 0;
    for ( obj=object_list; obj != NULL; obj=obj->next )
	{
	    if ( obj->pIndexData->vnum == obj_index->vnum )
            {
	      ingameCount++;
	      if ( obj->carried_by != NULL )
		swprintf( buf, MAX_STRING_LENGTH-1, L"Carried by %-30s\n\r", obj->carried_by->name);
	      if ( obj->in_room != NULL )
		swprintf( buf, MAX_STRING_LENGTH-1, L"At %-20s [%d]\n\r", obj->in_room->name, obj->in_room->vnum);
	      if ( obj->in_obj != NULL )
		swprintf( buf, MAX_STRING_LENGTH-1, L"In %-20s [%d] \n\r", obj->in_obj->short_descr, obj->in_obj->pIndexData->vnum);
	      send_to_char( buf, ch );
	    }
	}
	    swprintf( buf, MAX_STRING_LENGTH-1, L"  %d found in game. %d should be in pFiles.\n\r",
			ingameCount, obj_index->count-ingameCount);
	    send_to_char( buf, ch );
   return;
  }

  nMatch = 0;
  output[0] = '\0';
  for ( vnum = 0; nMatch < top_obj_index; vnum++ )
      if ( ( obj_index = get_obj_index( vnum ) ) != NULL )
      {
        nMatch++;
	if ( obj_index->limit != -1 )  {
	  lCount++;
          swprintf( buf, MAX_STRING_LENGTH-1, L"%-37s [%5d]  Limit: %3d  Current: %3d\n\r",
		   obj_index->short_descr,
		   obj_index->vnum,
	           obj_index->limit,
		   obj_index->count );
	  buf[0] = towupper( buf[0] );
	  wcscat( output, buf );
	}
      }
  swprintf( buf, MAX_STRING_LENGTH-1, L"\n\r%d of %d objects are limited.\n\r", lCount, nMatch );
  wcscat( output, buf );
  page_to_char(output,ch);
  return;
}

void do_wiznet( CHAR_DATA *ch, wchar_t *argument )
{
    int flag;
    wchar_t buf[MAX_STRING_LENGTH];

    if ( argument[0] == '\0' )
    {
      	if (IS_SET(ch->wiznet,WIZ_ON))
      	{
            send_to_char( L"Signing off of Wiznet.\n\r",ch);
            REMOVE_BIT(ch->wiznet,WIZ_ON);
      	}
      	else
      	{
            send_to_char( L"Welcome to Wiznet!\n\r",ch);
            SET_BIT(ch->wiznet,WIZ_ON);
      	}
      	return;
    }

    if (!str_prefix(argument, L"on"))
    {
	send_to_char( L"Welcome to Wiznet!\n\r",ch);
	SET_BIT(ch->wiznet,WIZ_ON);
	return;
    }

    if (!str_prefix(argument, L"off"))
    {
	send_to_char( L"Signing off of Wiznet.\n\r",ch);
	REMOVE_BIT(ch->wiznet,WIZ_ON);
	return;
    }

    /* show wiznet status */
    if (!str_prefix(argument, L"status"))
    {
	buf[0] = '\0';

	if (!IS_SET(ch->wiznet,WIZ_ON))
	    wcscat(buf,L"off ");

	for (flag = 0; wiznet_table[flag].name != NULL; flag++)
	    if (IS_SET(ch->wiznet,wiznet_table[flag].flag))
	    {
		wcscat(buf,wiznet_table[flag].name);
		wcscat(buf,L" ");
	    }

	wcscat(buf,L"\n\r");

	send_to_char( L"Wiznet status:\n\r",ch);
	send_to_char(buf,ch);
	return;
    }

    if (!str_prefix(argument, L"show"))
    /* list of all wiznet options */
    {
	buf[0] = '\0';

	for (flag = 0; wiznet_table[flag].name != NULL; flag++)
	{
	    if (wiznet_table[flag].level <= get_trust(ch))
	    {
	    	wcscat(buf,wiznet_table[flag].name);
	    	wcscat(buf,L" ");
	    }
	}

	wcscat(buf,L"\n\r");

	send_to_char( L"Wiznet options available to you are:\n\r",ch);
	send_to_char(buf,ch);
	return;
    }

    flag = wiznet_lookup(argument);

    if (flag == -1 || get_trust(ch) < wiznet_table[flag].level)
    {
	send_to_char( L"No such option.\n\r",ch);
	return;
    }

    if (IS_SET(ch->wiznet,wiznet_table[flag].flag))
    {
	swprintf( buf, MAX_STRING_LENGTH-1, L"You will no longer see %s on wiznet.\n\r",
	        wiznet_table[flag].name);
	send_to_char(buf,ch);
	REMOVE_BIT(ch->wiznet,wiznet_table[flag].flag);
    	return;
    }
    else
    {
    	swprintf( buf, MAX_STRING_LENGTH-1, L"You will now see %s on wiznet.\n\r",
		wiznet_table[flag].name);
	send_to_char(buf,ch);
    	SET_BIT(ch->wiznet,wiznet_table[flag].flag);
	return;
    }

}

void wiznet(const wchar_t *string, CHAR_DATA *ch, OBJ_DATA *obj,
	    long flag, long flag_skip, int min_level)
{
    DESCRIPTOR_DATA *d;

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
        if (d->connected == CON_PLAYING
	&&  IS_IMMORTAL(d->character)
	&&  IS_SET(d->character->wiznet,WIZ_ON)
	&&  (!flag || IS_SET(d->character->wiznet,flag))
	&&  (!flag_skip || !IS_SET(d->character->wiznet,flag_skip))
	&&  get_trust(d->character) >= min_level
	&&  d->character != ch)
        {
	    if (IS_SET(d->character->wiznet,WIZ_PREFIX))
	  	send_to_char( L"--> ",d->character);
            act_new(string,d->character,obj,ch,TO_CHAR,POS_DEAD);
        }
    }

    return;
}

void do_tick( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH];

    one_argument( argument, arg );
    if ( arg[0] == '\0' )  {
	send_to_char( L"tick area : area update\n\r",ch);
	send_to_char( L"tick char : char update\n\r",ch);
	send_to_char( L"tick obj  : obj  update\n\r",ch);
	send_to_char( L"tick room : room update\n\r",ch);
	send_to_char( L"tick track: track update\n\r",ch);
	return;
    }
    if (is_name(arg, (wchar_t*)"area") )  {
	area_update( );
	send_to_char( L"Area updated.\n\r", ch );
	return;
    }
    if (is_name(arg, (wchar_t*)"char player") )  {
	char_update( );
	send_to_char( L"Players updated.\n\r", ch );
	return;
    }
    if (is_name(arg, (wchar_t*)"obj") )  {
	obj_update( );
	send_to_char( L"Obj updated.\n\r", ch );
	return;
    }
    if (is_name(arg, (wchar_t*)"room") )  {
	room_update( );
	send_to_char( L"Room updated.\n\r", ch );
	return;
    }
    if (is_name(arg, (wchar_t*)"track") )  {
	track_update( );
	send_to_char( L"Track updated.\n\r", ch );
	return;
    }
    do_tick(ch,(wchar_t*)"");
    return;
}

/* equips a character */
void do_outfit ( CHAR_DATA *ch, wchar_t *argument )
{
    OBJ_DATA *obj;
    int vnum;

    if ((ch->level > 5 || IS_NPC(ch)) && !IS_IMMORTAL(ch))
    {
	send_to_char( L"Find it yourself!\n\r",ch);
	return;
    }

    if (ch->carry_number +  1 > can_carry_n(ch))
    {
            send_to_char( L"You can't carry that many items.\n\r", ch );
            return;
    }

    if ( ( obj = get_light_char( ch ) ) == NULL )
    {
        obj = create_object( get_obj_index(OBJ_VNUM_SCHOOL_BANNER), 0 );
	obj->cost = 0;
	obj->condition = 100;
        obj_to_char( obj, ch );
    }

    if (ch->carry_number +  1 > can_carry_n(ch))
    {
            send_to_char( L"You can't carry that many items.\n\r", ch );
            return;
    }

    if ( ( obj = get_eq_char( ch, WEAR_BODY ) ) == NULL )
    {
	obj = create_object( get_obj_index(OBJ_VNUM_SCHOOL_VEST), 0 );
	obj->cost = 0;
	obj->condition = 100;
        obj_to_char( obj, ch );
    }


    if (ch->carry_number +  1 > can_carry_n(ch))
    {
            send_to_char( L"You can't carry that many items.\n\r", ch );
            return;
    }

    /* do the weapon thing */
    if ((obj = get_wield_char(ch,FALSE) ) == NULL)
    {
    	vnum = OBJ_VNUM_SCHOOL_SWORD; /* just in case! */
        vnum = class_table[ch->iclass].weapon;
    	obj = create_object(get_obj_index(vnum),0);
	obj->condition = 100;
     	obj_to_char(obj,ch);
    }

    obj = create_object( get_obj_index(OBJ_VNUM_SCHOOL_SHIELD), 0 );
    obj->cost = 0;
    obj->condition = 100;
    obj_to_char( obj, ch );

    send_to_char( L"Tanrılar sana bazı eşyalar bahşediyor.\n\r",ch);
    send_to_char( L"Taşıdığın eşyaları görüntülemek için 'envanter' yaz.\n\r",ch);
    send_to_char( L"Eşyalarını giymek için 'giy <eşya adı>' yazmayı dene.\n\r\n\r",ch);
}


/* RT nochannels command, for those spammers */
void do_nochannels( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH], buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
        send_to_char( L"Nochannel whom?", ch );
        return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
        send_to_char( L"They aren't here.\n\r", ch );
        return;
    }

    if ( get_trust( victim ) >= get_trust( ch ) )
    {
        send_to_char( L"You failed.\n\r", ch );
        return;
    }

    if ( IS_SET(victim->comm, COMM_NOCHANNELS) )
    {
        REMOVE_BIT(victim->comm, COMM_NOCHANNELS);
        send_to_char( L"The gods have restored your channel priviliges.\n\r",
		      victim );
        send_to_char( L"NOCHANNELS removed.\n\r", ch );
	swprintf( buf, MAX_STRING_LENGTH-1, L"$N restores channels to %s",victim->name);
	wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
    }
    else
    {
        SET_BIT(victim->comm, COMM_NOCHANNELS);
        send_to_char( L"The gods have revoked your channel priviliges.\n\r",
		       victim );
        send_to_char( L"NOCHANNELS set.\n\r", ch );
	swprintf( buf, MAX_STRING_LENGTH-1, L"$N revokes %s's channels.",victim->name);
	wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
    }

    return;
}


void do_smote(CHAR_DATA *ch, wchar_t *argument )
{
    CHAR_DATA *vch;
    wchar_t *letter,*name;
    wchar_t last[MAX_INPUT_LENGTH], temp[MAX_STRING_LENGTH];
    size_t matches = 0;

    if ( !IS_NPC(ch) && IS_SET(ch->comm, COMM_NOEMOTE) )
    {
        send_to_char( L"You can't show your emotions.\n\r", ch );
        return;
    }

    if ( argument[0] == '\0' )
    {
        send_to_char( L"Emote what?\n\r", ch );
        return;
    }

    if (wcsstr(argument,ch->name) == NULL)
    {
	send_to_char( L"You must include your name in an smote.\n\r",ch);
	return;
    }

    send_to_char(argument,ch);
    send_to_char( L"\n\r",ch);

    for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
    {
        if (vch->desc == NULL || vch == ch)
            continue;

        if ((letter = wcsstr(argument,vch->name)) == NULL)
        {
	    send_to_char(argument,vch);
	    send_to_char( L"\n\r",vch);
            continue;
        }

        wcscpy(temp,argument);
        temp[wcslen(argument) - wcslen(letter)] = '\0';
        last[0] = '\0';
        name = vch->name;

        for (; *letter != '\0'; letter++)
        {
            if (*letter == '\'' && matches == wcslen(vch->name))
            {
                wcscat(temp,L"r");
                continue;
            }

            if (*letter == 's' && matches == wcslen(vch->name))
            {
                matches = 0;
                continue;
            }

            if (matches == wcslen(vch->name))
            {
                matches = 0;
            }

            if (*letter == *name)
            {
                matches++;
                name++;
                if (matches == wcslen(vch->name))
                {
                    wcscat(temp,L"you");
                    last[0] = '\0';
                    name = vch->name;
                    continue;
                }
                wcsncat(last,letter,1);
                continue;
            }

            matches = 0;
            wcscat(temp,last);
            wcsncat(temp,letter,1);
            last[0] = '\0';
            name = vch->name;
        }

	send_to_char(temp,vch);
	send_to_char( L"\n\r",vch);
    }

    return;
}

void do_bamfin( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t buf[MAX_STRING_LENGTH];

    if ( !IS_NPC(ch) )
    {
	smash_tilde( argument );

	if (argument[0] == '\0')
	{
	    swprintf( buf, MAX_STRING_LENGTH-1, L"Your poofin is %s\n\r",ch->pcdata->bamfin);
	    send_to_char(buf,ch);
	    return;
	}

	if ( wcsstr(argument,ch->name) == NULL)
	{
	    send_to_char( L"You must include your name.\n\r",ch);
	    return;
	}

	free_string( ch->pcdata->bamfin );
	ch->pcdata->bamfin = str_dup( argument );

        swprintf( buf, MAX_STRING_LENGTH-1, L"Your poofin is now %s\n\r",ch->pcdata->bamfin);
        send_to_char(buf,ch);
    }
    return;
}



void do_bamfout( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t buf[MAX_STRING_LENGTH];

    if ( !IS_NPC(ch) )
    {
        smash_tilde( argument );

        if (argument[0] == '\0')
        {
            swprintf( buf, MAX_STRING_LENGTH-1, L"Your poofout is %s\n\r",ch->pcdata->bamfout);
            send_to_char(buf,ch);
            return;
        }

        if ( wcsstr(argument,ch->name) == NULL)
        {
            send_to_char( L"You must include your name.\n\r",ch);
            return;
        }

        free_string( ch->pcdata->bamfout );
        ch->pcdata->bamfout = str_dup( argument );

        swprintf( buf, MAX_STRING_LENGTH-1, L"Your poofout is now %s\n\r",ch->pcdata->bamfout);
        send_to_char(buf,ch);
    }
    return;
}



void do_deny( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH],buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( L"Deny whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( L"They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( L"Not on NPC's.\n\r", ch );
	return;
    }

    if ( get_trust( victim ) >= get_trust( ch ) )
    {
	send_to_char( L"You failed.\n\r", ch );
	return;
    }

    SET_BIT(victim->act, PLR_DENY);
    send_to_char( L"You are denied access!\n\r", victim );
    swprintf( buf, MAX_STRING_LENGTH-1, L"$N denies access to %s",victim->name);
    wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
    send_to_char( L"OK.\n\r", ch );
    save_char_obj(victim);
    stop_fighting(victim,TRUE);
    do_quit( victim, (wchar_t*)"" );

    return;
}



void do_disconnect( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH];
    DESCRIPTOR_DATA *d, *d_next;
    CHAR_DATA *victim;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( L"Disconnect whom?\n\r", ch );
	return;
    }

    if (is_number(arg))
    {
	int desc;

	desc = wcstol(arg, 0, 10 );
    	for ( d = descriptor_list; d != NULL; d = d_next )
    	{
	    d_next = d->next;
            if ( d->descriptor == desc )
            {
            	close_socket( d );
            	send_to_char( L"Ok.\n\r", ch );
            	return;
            }
	}
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( L"They aren't here.\n\r", ch );
	return;
    }

    if ( victim->desc == NULL )
    {
	act( L"$N doesn't have a descriptor.", ch, NULL, victim, TO_CHAR );
	return;
    }

    for ( d = descriptor_list; d != NULL; d = d_next )
    {
	d_next = d->next;
	if ( d == victim->desc )
	{
	    close_socket( d );
	    send_to_char( L"Ok.\n\r", ch );
	    return;
	}
    }

    bug( L"Do_disconnect: desc not found.", 0 );
    send_to_char( L"Descriptor not found!\n\r", ch );
    return;
}


void do_duyuru( CHAR_DATA *ch, wchar_t *argument )
{
  DESCRIPTOR_DATA *d;

  if ( argument[0] == '\0' )
  {
    send_to_char( L"Neyi duyuracaksın?\n\r", ch );
    return;
  }

  for ( d = descriptor_list; d; d = d->next )
  {
    if ( d->connected == CON_PLAYING )
    {
      printf_to_char(d->character, L"{RDUYURU: {G%s{x\n\r",argument);
    }
  }

  return;
}


ROOM_INDEX_DATA *find_location( CHAR_DATA *ch, wchar_t *arg )
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;

    if ( is_number(arg) )
	return get_room_index( wcstol( arg, 0, 10 ) );

    if ( ( victim = get_char_world( ch, arg ) ) != NULL )
	return victim->in_room;

    if ( ( obj = get_obj_world( ch, arg ) ) != NULL )
	return obj->in_room;

    return NULL;
}



void do_transfer( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg1[MAX_INPUT_LENGTH];
    wchar_t arg2[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA *location;
    DESCRIPTOR_DATA *d, *d_next;
    CHAR_DATA *victim;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' )
    {
	send_to_char( L"Transfer whom (and where)?\n\r", ch );
	return;
    }

    if ( !wcscasecmp( arg1, L"all" ) )
    {
	for ( d = descriptor_list; d != NULL; d = d_next )
	{
	    d_next = d->next;
	    if ( d->connected == CON_PLAYING
	    &&   d->character != ch
	    &&   d->character->in_room != NULL
	    &&   can_see( ch, d->character ) )
	    {
		wchar_t buf[MAX_STRING_LENGTH];
		swprintf( buf, MAX_STRING_LENGTH-1, L"%s %s", d->character->name, arg2 );
		do_transfer( ch, buf );
	    }
	}
	return;
    }

    /*
     * Thanks to Grodyn for the optional location parameter.
     */
    if ( arg2[0] == '\0' )
    {
	location = ch->in_room;
    }
    else
    {
	if ( ( location = find_location( ch, arg2 ) ) == NULL )
	{
	    send_to_char( L"No such location.\n\r", ch );
	    return;
	}

/*	if ( !is_room_owner(ch,location) && room_is_private( location ) */
	if ( room_is_private( location )
	&&  get_trust(ch) < MAX_LEVEL)
	{
	    send_to_char( L"That room is private right now.\n\r", ch );
	    return;
	}
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( L"They aren't here.\n\r", ch );
	return;
    }

    if ( victim->in_room == NULL )
    {
	send_to_char( L"They are in limbo.\n\r", ch );
	return;
    }

    if ( victim->fighting != NULL )
	stop_fighting( victim, TRUE );
    act( L"$n disappears in a mushroom cloud.", victim, NULL, NULL, TO_ROOM );
    char_from_room( victim );
    char_to_room( victim, location );
    act( L"$n arrives from a puff of smoke.", victim, NULL, NULL, TO_ROOM );
    if ( ch != victim )
	act( L"$n has transferred you.", ch, NULL, victim, TO_VICT );
    do_look( victim, (wchar_t*)"auto" );
    send_to_char( L"Ok.\n\r", ch );
}



void do_at( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA *location;
    ROOM_INDEX_DATA *original;
    OBJ_DATA *on;
    CHAR_DATA *wch, *wch_next;

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
	send_to_char( L"At where what?\n\r", ch );
	return;
    }

    if ( ( location = find_location( ch, arg ) ) == NULL )
    {
	send_to_char( L"No such location.\n\r", ch );
	return;
    }

/*    if (!is_room_owner(ch,location) && room_is_private( location ) */
    if ( room_is_private( location )
    &&  get_trust(ch) < MAX_LEVEL)
    {
	send_to_char( L"That room is private right now.\n\r", ch );
	return;
    }

    original = ch->in_room;
    on = ch->on;
    char_from_room( ch );
    char_to_room( ch, location );
    interpret( ch, argument, FALSE );

    /*
     * See if 'ch' still exists before continuing!
     * Handles 'at XXXX quit' case.
     */
    for ( wch = char_list; wch != NULL; wch = wch_next )
    {
	wch_next = wch->next;
	if ( wch == ch )
	{
	    char_from_room( ch );
	    char_to_room( ch, original );
	    ch->on = on;
	    break;
	}
    }

    return;
}



void do_goto( CHAR_DATA *ch, wchar_t *argument )
{
    ROOM_INDEX_DATA *location;
    CHAR_DATA *rch;
    int count = 0;

    if ( argument[0] == '\0' )
    {
	send_to_char( L"Goto where?\n\r", ch );
	return;
    }

    if ( ( location = find_location( ch, argument ) ) == NULL )
    {
	send_to_char( L"No such location.\n\r", ch );
	return;
    }

    count = 0;
    for ( rch = location->people; rch != NULL; rch = rch->next_in_room )
        count++;
/*
    if (!is_room_owner(ch,location) && room_is_private(location)
    &&  (count > 1 || get_trust(ch) < MAX_LEVEL))
    {
	send_to_char( L"That room is private right now.\n\r", ch );
	return;
    } */

    if ( ch->fighting != NULL )
	stop_fighting( ch, TRUE );

    for (rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room)
    {
	if (get_trust(rch) >= ch->invis_level)
	{
	    if (ch->pcdata != NULL && ch->pcdata->bamfout[0] != '\0')
		act( L"$t",ch,ch->pcdata->bamfout,rch,TO_VICT);
	    else
		act( L"$n leaves in a swirling mist.",ch,NULL,rch,TO_VICT);
	}
    }

    char_from_room( ch );
    char_to_room( ch, location );


    for (rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room)
    {
        if (get_trust(rch) >= ch->invis_level)
        {
            if (ch->pcdata != NULL && ch->pcdata->bamfin[0] != '\0')
                act( L"$t",ch,ch->pcdata->bamfin,rch,TO_VICT);
            else
                act( L"$n appears in a swirling mist.",ch,NULL,rch,TO_VICT);
        }
    }

    do_look( ch, (wchar_t*)"auto" );
    return;
}

void do_violate( CHAR_DATA *ch, wchar_t *argument )
{
    ROOM_INDEX_DATA *location;
    CHAR_DATA *rch;

    if ( argument[0] == '\0' )
    {
        send_to_char( L"Goto where?\n\r", ch );
        return;
    }

    if ( ( location = find_location( ch, argument ) ) == NULL )
    {
        send_to_char( L"No such location.\n\r", ch );
        return;
    }

    if (!room_is_private( location ))
    {
        send_to_char( L"That room isn't private, use goto.\n\r", ch );
        return;
    }

    if ( ch->fighting != NULL )
        stop_fighting( ch, TRUE );

    for (rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room)
    {
        if (get_trust(rch) >= ch->invis_level)
        {
            if (ch->pcdata != NULL && ch->pcdata->bamfout[0] != '\0')
                act( L"$t",ch,ch->pcdata->bamfout,rch,TO_VICT);
            else
                act( L"$n leaves in a swirling mist.",ch,NULL,rch,TO_VICT);
        }
    }

    char_from_room( ch );
    char_to_room( ch, location );


    for (rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room)
    {
        if (get_trust(rch) >= ch->invis_level)
        {
            if (ch->pcdata != NULL && ch->pcdata->bamfin[0] != '\0')
                act( L"$t",ch,ch->pcdata->bamfin,rch,TO_VICT);
            else
                act( L"$n appears in a swirling mist.",ch,NULL,rch,TO_VICT);
        }
    }

    do_look( ch, (wchar_t*)"auto" );
    return;
}

/* RT to replace the 3 stat commands */

void do_stat ( CHAR_DATA *ch, wchar_t *argument )
{
   wchar_t arg[MAX_INPUT_LENGTH];
   wchar_t *string;
   OBJ_DATA *obj;
   ROOM_INDEX_DATA *location;
   CHAR_DATA *victim;

   string = one_argument(argument, arg);
   if ( arg[0] == '\0')
   {
	send_to_char( L"Syntax:\n\r",ch);
	send_to_char( L"  stat <name>\n\r",ch);
	send_to_char( L"  stat obj <name>\n\r",ch);
	send_to_char( L"  stat mob <name>\n\r",ch);
 	send_to_char( L"  stat room <number>\n\r",ch);
	return;
   }

   if (!wcscasecmp(arg, L"room"))
   {
	do_rstat(ch,string);
	return;
   }

   if (!wcscasecmp(arg, L"obj"))
   {
	do_ostat(ch,string);
	return;
   }

   if(!wcscasecmp(arg, L"char")  || !wcscasecmp(arg, L"mob"))
   {
	do_mstat(ch,string);
	return;
   }

   /* do it the old way */

   obj = get_obj_world(ch,argument);
   if (obj != NULL)
   {
     do_ostat(ch,argument);
     return;
   }

  victim = get_char_world(ch,argument);
  if (victim != NULL)
  {
    do_mstat(ch,argument);
    return;
  }

  location = find_location(ch,argument);
  if (location != NULL)
  {
    do_rstat(ch,argument);
    return;
  }

  send_to_char( L"Nothing by that name found anywhere.\n\r",ch);
}





void do_rstat( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t buf[MAX_STRING_LENGTH];
    wchar_t arg[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA *location;
    ROOM_HISTORY_DATA *rh;
    OBJ_DATA *obj;
    CHAR_DATA *rch;
    int door;

    one_argument( argument, arg );
    location = ( arg[0] == '\0' ) ? ch->in_room : find_location( ch, arg );
    if ( location == NULL )
    {
	send_to_char( L"No such location.\n\r", ch );
	return;
    }

/*    if (!is_room_owner(ch,location) && ch->in_room != location  */
    if ( ch->in_room != location
    &&  room_is_private( location ) && !IS_TRUSTED(ch,IMPLEMENTOR))
    {
	send_to_char( L"That room is private right now.\n\r", ch );
	return;
    }

    if (ch->in_room->affected_by)
    {
	swprintf( buf, MAX_STRING_LENGTH-1, L"Affected by %s\n\r",
	    raffect_bit_name(ch->in_room->affected_by));
	send_to_char(buf,ch);
    }

    if (ch->in_room->room_flags)
    {
	swprintf( buf, MAX_STRING_LENGTH-1, L"Roomflags %s\n\r",
	    flag_room_name(ch->in_room->room_flags));
	send_to_char(buf,ch);
    }

    swprintf( buf, MAX_STRING_LENGTH-1, L"Name: '%s'\n\rArea: '%s'\n\rOwner: '%s'\n\r",
	location->name,
	location->area->name ,
	location->owner );
    send_to_char( buf, ch );

    swprintf( buf, MAX_STRING_LENGTH-1, L"Vnum: %d  Sector: %d  Light: %d  Healing: %d  Mana: %d\n\r",
	location->vnum,
	location->sector_type,
	location->light,
	location->heal_rate,
	location->mana_rate );
    send_to_char( buf, ch );

    swprintf( buf, MAX_STRING_LENGTH-1, L"Room flags: %ld.\n\rDescription:\n\r%s",
	location->room_flags,
	location->description );
    send_to_char( buf, ch );

    if ( location->extra_descr != NULL )
    {
	EXTRA_DESCR_DATA *ed;

	send_to_char( L"Extra description keywords: '", ch );
	for ( ed = location->extra_descr; ed; ed = ed->next )
	{
	    send_to_char( ed->keyword, ch );
	    if ( ed->next != NULL )
		send_to_char( L" ", ch );
	}
	send_to_char( L"'.\n\r", ch );
    }

    send_to_char( L"Characters:", ch );
    for ( rch = location->people; rch; rch = rch->next_in_room )
    {
	if (can_see(ch,rch))
        {
	    send_to_char( L" ", ch );
	    one_argument( rch->name, buf );
	    send_to_char( buf, ch );
	}
    }

    send_to_char( L".\n\rObjects:   ", ch );
    for ( obj = location->contents; obj; obj = obj->next_content )
    {
	send_to_char( L" ", ch );
	one_argument( obj->name, buf );
	send_to_char( buf, ch );
    }
    send_to_char( L".\n\r", ch );

    for ( door = 0; door <= 5; door++ )
    {
	EXIT_DATA *pexit;

	if ( ( pexit = location->exit[door] ) != NULL )
	{
	    swprintf( buf, MAX_STRING_LENGTH-1, L"Door: %d.  To: %d.  Key: %d.  Exit flags: %d.\n\rKeyword: '%s'.  Description: %s",

		door,
		(pexit->u1.to_room == NULL ? -1 : pexit->u1.to_room->vnum),
	    	pexit->key,
	    	pexit->exit_info,
	    	pexit->keyword,
	    	pexit->description[0] != '\0'
		    ? pexit->description : L"(none).\n\r" );
	    send_to_char( buf, ch );
	}
    }
    send_to_char( L"Tracks:\n\r",ch);
    for (rh = location->history;rh != NULL;rh = rh->next) {
      swprintf( buf, MAX_STRING_LENGTH-1, L"%s took door %i.\n\r",rh->name,rh->went);
      send_to_char(buf,ch);
    }
    return;
}



void do_ostat( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t buf[MAX_STRING_LENGTH];
    wchar_t arg[MAX_INPUT_LENGTH];
    AFFECT_DATA *paf;
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( L"Stat what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_world( ch, argument ) ) == NULL )
    {
	send_to_char( L"Ne bu dünyada ne de diğerinde böyle bir şey yok.\n\r", ch );
	return;
    }

    swprintf( buf, MAX_STRING_LENGTH-1, L"Name(s): %s\n\r",
	obj->name );
    send_to_char( buf, ch );

    swprintf( buf, MAX_STRING_LENGTH-1, L"Vnum: %d  Format: %s  Type: %s  Resets: %d\n\r",
	obj->pIndexData->vnum, obj->pIndexData->new_format ? "new" : "old",
	item_type_name(obj), obj->pIndexData->reset_num );
    send_to_char( buf, ch );

    swprintf( buf, MAX_STRING_LENGTH-1, L"Short description: %s\n\rLong description: %s\n\r",
	obj->short_descr, obj->description );
    send_to_char( buf, ch );

    swprintf( buf, MAX_STRING_LENGTH-1, L"Wear bits: %s\n\rExtra bits: %s\n\r",
	wear_bit_name(obj->wear_flags), extra_bit_name( obj->extra_flags ) );
    send_to_char( buf, ch );

    swprintf( buf, MAX_STRING_LENGTH-1, L"Number: %d/%d  Weight: %d/%d/%d (10th pounds)\n\r",
	1,           get_obj_number( obj ),
	obj->weight, get_obj_weight( obj ),get_true_weight(obj) );
    send_to_char( buf, ch );

    swprintf( buf, MAX_STRING_LENGTH-1, L"Level: %d  Cost: %d  Condition: %d  Timer: %d Count: %d\n\r",
	obj->level, obj->cost, obj->condition, obj->timer, obj->pIndexData->count );
    send_to_char( buf, ch );

    swprintf( buf, MAX_STRING_LENGTH-1, L"In room: %d  In object: %s  Carried by: %s  Wear_loc: %d\n\r",
	obj->in_room    == NULL    ?        0 : obj->in_room->vnum,
	obj->in_obj     == NULL    ? L"(none)" : obj->in_obj->short_descr,
	obj->carried_by == NULL    ? L"(none)" :
	    can_see(ch,obj->carried_by) ? obj->carried_by->name
				 	: L"birisi",
	obj->wear_loc );
    send_to_char( buf, ch );

    swprintf( buf, MAX_STRING_LENGTH-1, L"Values: %d %d %d %d %d\n\r",
	obj->value[0], obj->value[1], obj->value[2], obj->value[3],
	obj->value[4] );
    send_to_char( buf, ch );

    /* now give out vital statistics as per identify */

    switch ( obj->item_type )
    {
    	case ITEM_SCROLL:
    	case ITEM_POTION:
    	case ITEM_PILL:
	    swprintf( buf, MAX_STRING_LENGTH-1, L"Level %d spells of:", obj->value[0] );
	    send_to_char( buf, ch );

	    if ( obj->value[1] >= 0 && obj->value[1] < MAX_SKILL )
	    {
	    	send_to_char( L" '", ch );
	    	send_to_char( skill_table[obj->value[1]].name[1], ch );
	    	send_to_char( L"'", ch );
	    }

	    if ( obj->value[2] >= 0 && obj->value[2] < MAX_SKILL )
	    {
	    	send_to_char( L" '", ch );
	    	send_to_char( skill_table[obj->value[2]].name[1], ch );
	    	send_to_char( L"'", ch );
	    }

	    if ( obj->value[3] >= 0 && obj->value[3] < MAX_SKILL )
	    {
	    	send_to_char( L" '", ch );
	    	send_to_char( skill_table[obj->value[3]].name[1], ch );
	    	send_to_char( L"'", ch );
	    }

	    if (obj->value[4] >= 0 && obj->value[4] < MAX_SKILL)
	    {
		send_to_char( L" '",ch);
		send_to_char(skill_table[obj->value[4]].name[1],ch);
		send_to_char( L"'",ch);
	    }

	    send_to_char( L".\n\r", ch );
	break;

    	case ITEM_WAND:
    	case ITEM_STAFF:
	    swprintf( buf, MAX_STRING_LENGTH-1, L"Has %d(%d) charges of level %d",
	    	obj->value[1], obj->value[2], obj->value[0] );
	    send_to_char( buf, ch );

	    if ( obj->value[3] >= 0 && obj->value[3] < MAX_SKILL )
	    {
	    	send_to_char( L" '", ch );
	    	send_to_char( skill_table[obj->value[3]].name[1], ch );
	    	send_to_char( L"'", ch );
	    }

	    send_to_char( L".\n\r", ch );
	break;

	case ITEM_DRINK_CON:
	    swprintf( buf, MAX_STRING_LENGTH-1, L"It holds %s-colored %s.\n\r",
		liq_table[obj->value[2]].liq_color,
		liq_table[obj->value[2]].liq_name);
	    send_to_char(buf,ch);
	    break;


    	case ITEM_WEAPON:
 	    send_to_char( L"Weapon type is ",ch);
	    switch (obj->value[0])
	    {
	    	case(WEAPON_EXOTIC):
		    send_to_char( L"exotic\n\r",ch);
		    break;
	    	case(WEAPON_SWORD):
		    send_to_char( L"sword\n\r",ch);
		    break;
	    	case(WEAPON_DAGGER):
		    send_to_char( L"dagger\n\r",ch);
		    break;
	    	case(WEAPON_SPEAR):
		    send_to_char( L"spear/staff\n\r",ch);
		    break;
	    	case(WEAPON_MACE):
		    send_to_char( L"mace/club\n\r",ch);
		    break;
	   	case(WEAPON_AXE):
		    send_to_char( L"axe\n\r",ch);
		    break;
	    	case(WEAPON_FLAIL):
		    send_to_char( L"flail\n\r",ch);
		    break;
	    	case(WEAPON_WHIP):
		    send_to_char( L"whip\n\r",ch);
		    break;
	    	case(WEAPON_POLEARM):
		    send_to_char( L"polearm\n\r",ch);
		    break;
	    	case(WEAPON_BOW):
		    send_to_char( L"bow\n\r",ch);
		    break;
	    	case(WEAPON_ARROW):
		    send_to_char( L"arrow\n\r",ch);
		    break;
	    	case(WEAPON_LANCE):
		    send_to_char( L"lance\n\r",ch);
		    break;
	    	default:
		    send_to_char( L"unknown\n\r",ch);
		    break;
 	    }
	    if (obj->pIndexData->new_format)
	    	swprintf( buf, MAX_STRING_LENGTH-1, L"Damage is %dd%d (average %d)\n\r",
		    obj->value[1],obj->value[2],
		    (1 + obj->value[2]) * obj->value[1] / 2);
	    else
	    	swprintf( buf, MAX_STRING_LENGTH-1, L"Damage is %d to %d (average %d)\n\r",
	    	    obj->value[1], obj->value[2],
	    	    ( obj->value[1] + obj->value[2] ) / 2 );
	    send_to_char( buf, ch );

	    swprintf( buf, MAX_STRING_LENGTH-1, L"Damage noun is %s.\n\r",
		attack_table[obj->value[3]].noun);
	    send_to_char(buf,ch);

	    if (obj->value[4])  /* weapon flags */
	    {
	        swprintf( buf, MAX_STRING_LENGTH-1, L"Weapons flags: %s\n\r",
		    weapon_bit_name(obj->value[4]));
	        send_to_char(buf,ch);
            }
	break;

    	case ITEM_ARMOR:
	    swprintf( buf, MAX_STRING_LENGTH-1, L"Armor class is %d pierce, %d bash, %d slash, and %d vs. magic\n\r",
	        obj->value[0], obj->value[1], obj->value[2], obj->value[3] );
	    send_to_char( buf, ch );
	break;

        case ITEM_CONTAINER:
            swprintf( buf, MAX_STRING_LENGTH-1, L"Capacity: %d#  Maximum weight: %d#  flags: %s\n\r",
                obj->value[0], obj->value[3], cont_bit_name(obj->value[1]));
            send_to_char(buf,ch);
            if (obj->value[4] != 100)
            {
                swprintf( buf, MAX_STRING_LENGTH-1, L"Weight multiplier: %d%%\n\r",
		    obj->value[4]);
                send_to_char(buf,ch);
            }
        break;
    }


    if ( obj->extra_descr != NULL || obj->pIndexData->extra_descr != NULL )
    {
	EXTRA_DESCR_DATA *ed;

	send_to_char( L"Extra description keywords: '", ch );

	for ( ed = obj->extra_descr; ed != NULL; ed = ed->next )
	{
	    send_to_char( ed->keyword, ch );
	    if ( ed->next != NULL )
	    	send_to_char( L" ", ch );
	}

	for ( ed = obj->pIndexData->extra_descr; ed != NULL; ed = ed->next )
	{
	    send_to_char( ed->keyword, ch );
	    if ( ed->next != NULL )
		send_to_char( L" ", ch );
	}

	send_to_char( L"'\n\r", ch );
    }

    for ( paf = obj->affected; paf != NULL; paf = paf->next )
    {
	swprintf( buf, MAX_STRING_LENGTH-1, L"Affects %s by %d, level %d",
	    affect_loc_name( paf->location ), paf->modifier,paf->level );
	send_to_char(buf,ch);
	if ( paf->duration > -1)
	    swprintf( buf, MAX_STRING_LENGTH-1, L", %d hours.\n\r",paf->duration);
	else
	    swprintf( buf, MAX_STRING_LENGTH-1, L".\n\r");
	send_to_char( buf, ch );
	if (paf->bitvector)
	{
	    switch(paf->where)
	    {
		case TO_AFFECTS:
		    swprintf( buf, MAX_STRING_LENGTH-1, L"Adds %s affect.\n",
			affect_bit_name(paf->bitvector));
		    break;
                case TO_WEAPON:
                    swprintf( buf, MAX_STRING_LENGTH-1, L"Adds %s weapon flags.\n",
                        weapon_bit_name(paf->bitvector));
		    break;
		case TO_OBJECT:
		    swprintf( buf, MAX_STRING_LENGTH-1, L"Adds %s object flag.\n",
			extra_bit_name(paf->bitvector));
		    break;
		case TO_IMMUNE:
		    swprintf( buf, MAX_STRING_LENGTH-1, L"Adds immunity to %s.\n",
			imm_bit_name(paf->bitvector));
		    break;
		case TO_RESIST:
		    swprintf( buf, MAX_STRING_LENGTH-1, L"Adds resistance to %s.\n\r",
			imm_bit_name(paf->bitvector));
		    break;
		case TO_VULN:
		    swprintf( buf, MAX_STRING_LENGTH-1, L"Adds vulnerability to %s.\n\r",
			imm_bit_name(paf->bitvector));
		    break;
		case TO_DETECTS:
		    swprintf( buf, MAX_STRING_LENGTH-1, L"Adds %s detection.\n\r",
			detect_bit_name(paf->bitvector));
		    break;
		default:
		    swprintf( buf, MAX_STRING_LENGTH-1, L"Unknown bit %d: %d\n\r",
			paf->where,paf->bitvector);
		    break;
	    }
	    send_to_char(buf,ch);
	}
    }

    if (!obj->enchanted)
    for ( paf = obj->pIndexData->affected; paf != NULL; paf = paf->next )
    {
	swprintf( buf, MAX_STRING_LENGTH-1, L"Affects %s by %d, level %d.\n\r",
	    affect_loc_name( paf->location ), paf->modifier,paf->level );
	send_to_char( buf, ch );
        if (paf->bitvector)
        {
            switch(paf->where)
            {
                case TO_AFFECTS:
                    swprintf( buf, MAX_STRING_LENGTH-1, L"Adds %s affect.\n",
                        affect_bit_name(paf->bitvector));
                    break;
                case TO_OBJECT:
                    swprintf( buf, MAX_STRING_LENGTH-1, L"Adds %s object flag.\n",
                        extra_bit_name(paf->bitvector));
                    break;
                case TO_IMMUNE:
                    swprintf( buf, MAX_STRING_LENGTH-1, L"Adds immunity to %s.\n",
                        imm_bit_name(paf->bitvector));
                    break;
                case TO_RESIST:
                    swprintf( buf, MAX_STRING_LENGTH-1, L"Adds resistance to %s.\n\r",
                        imm_bit_name(paf->bitvector));
                    break;
                case TO_VULN:
                    swprintf( buf, MAX_STRING_LENGTH-1, L"Adds vulnerability to %s.\n\r",
                        imm_bit_name(paf->bitvector));
                    break;
                case TO_DETECTS:
                    swprintf( buf, MAX_STRING_LENGTH-1, L"Adds %s detection.\n\r",
                        detect_bit_name(paf->bitvector));
                    break;
                default:
                    swprintf( buf, MAX_STRING_LENGTH-1, L"Unknown bit %d: %d\n\r",
                        paf->where,paf->bitvector);
                    break;
            }
            send_to_char(buf,ch);
        }
    }
    swprintf( buf, MAX_STRING_LENGTH-1, L"Object progs: " );
    if ( obj->pIndexData->progtypes != 0 )  {
	if ( IS_SET(obj->progtypes, OPROG_GET ) )
	   wcscat( buf, L"get " );
	if ( IS_SET(obj->progtypes, OPROG_DROP ) )
	   wcscat( buf, L"drop " );
	if ( IS_SET(obj->progtypes, OPROG_SAC ) )
	   wcscat( buf, L"sacrifice " );
	if ( IS_SET(obj->progtypes, OPROG_GIVE ) )
	   wcscat( buf, L"give " );
	if ( IS_SET(obj->progtypes, OPROG_FIGHT ) )
	   wcscat( buf, L"fight " );
	if ( IS_SET(obj->progtypes, OPROG_DEATH ) )
	   wcscat( buf, L"death " );
	if ( IS_SET(obj->progtypes, OPROG_SPEECH ) )
	   wcscat( buf, L"speech " );
	if ( IS_SET(obj->progtypes, OPROG_AREA ) )
	   wcscat( buf, L"area " );
    }
    wcscat( buf, L"\n\r" );
    send_to_char( buf, ch );
    swprintf( buf, MAX_STRING_LENGTH-1, L"Damage condition : %d (%s) ", obj->condition,
			get_cond_alias(obj) );
    send_to_char(buf,ch);
    send_to_char( L"\n\r",ch);
    return;
}


void do_mobstat( CHAR_DATA *ch, wchar_t *argument )
{
  wchar_t arg1[MAX_INPUT_LENGTH];
  CHAR_DATA *gch;

  one_argument( argument, arg1 );

  if ( arg1[0] == '\0' )
  {
    printf_to_char(ch,L"Eksik argüman.\n\r");
    return;
  }

  for ( gch = char_list; gch != NULL; gch = gch->next )
  {
      if (!IS_NPC(gch))
          continue;
      if (gch->level == wcstol(arg1, 0, 10 ))
        printf_to_char(ch,L"Level: %-3d  Damroll: %-4d  Hitroll: %-4d  Hp: %-6d Mn: %-6d Mv: %-6d\n\r",wcstol( arg1, 0, 10 ),gch->damroll,gch->hitroll,gch->hit,gch->mana,gch->move);

  }
}



void do_mstat( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t buf[MAX_STRING_LENGTH];
    wchar_t buf2[MAX_STRING_LENGTH];
    wchar_t arg[MAX_INPUT_LENGTH];
    AFFECT_DATA *paf;
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( L"Stat whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, argument ) ) == NULL )
    {
	send_to_char( L"They aren't here.\n\r", ch );
	return;
    }

    swprintf( buf, MAX_STRING_LENGTH-1, L"Name: [%s] Reset Zone: [%s] Logon: %s\r",
	victim->name,
	(IS_NPC(victim) &&victim->zone) ? victim->zone->name : L"?",
	ctime( &ch->logon ) );
    send_to_char( buf, ch );

    swprintf( buf, MAX_STRING_LENGTH-1, L"Vnum: %d  Format: %s  Race: %s(%s)  Sex: %s  Room: %d\n\r",
	IS_NPC(victim) ? victim->pIndexData->vnum : 0,
	IS_NPC(victim) ? L"UD" : L"PC",
	race_table[RACE(victim)].name[1],race_table[ORG_RACE(victim)].name[1],
	sex_table[victim->sex].name,
	victim->in_room == NULL    ?        0 : victim->in_room->vnum
	);
    send_to_char( buf, ch );

    if (IS_NPC(victim))
    {
	swprintf( buf, MAX_STRING_LENGTH-1, L"Count: %d  Killed: %d  ---  Status: %d  Cabal: %d\n\r",
	    victim->pIndexData->count,victim->pIndexData->killed,
	    victim->status, victim->cabal);
	send_to_char(buf,ch);
    }

    swprintf( buf, MAX_STRING_LENGTH-1, L"Str: %d(%d)  Int: %d(%d)  Wis: %d(%d)  Dex: %d(%d)  Con: %d(%d) Cha: %d(%d)\n\r",
	victim->perm_stat[STAT_STR],
	get_curr_stat(victim,STAT_STR),
	victim->perm_stat[STAT_INT],
	get_curr_stat(victim,STAT_INT),
	victim->perm_stat[STAT_WIS],
	get_curr_stat(victim,STAT_WIS),
	victim->perm_stat[STAT_DEX],
	get_curr_stat(victim,STAT_DEX),
	victim->perm_stat[STAT_CON],
	get_curr_stat(victim,STAT_CON),
	victim->perm_stat[STAT_CHA],
	get_curr_stat(victim,STAT_CHA) );
    send_to_char( buf, ch );


    swprintf( buf, MAX_STRING_LENGTH-1, L"Hp: %d/%d  Mana: %d/%d  Move: %d/%d  Practices: %d\n\r",
	victim->hit,         victim->max_hit,
	victim->mana,        victim->max_mana,
	victim->move,        victim->max_move,
	IS_NPC(ch) ? 0 : victim->practice );
    send_to_char( buf, ch );

    if ( IS_NPC(victim) )
      swprintf( buf2, MAX_STRING_LENGTH-1, L"%d", victim->alignment );
    else  {
      swprintf( buf2, MAX_STRING_LENGTH-1, L"%s",
	victim->ethos==1?L"Law-":
	victim->ethos==2?L"Neut-":
	victim->ethos==3?L"Cha-":L"none-" );
      wcscat( buf2,
	IS_GOOD(victim)?(wchar_t*)"Good":
	IS_NEUTRAL(victim)?(wchar_t*)"Neut":
	IS_EVIL(victim)?(wchar_t*)"Evil":(wchar_t*)"Other" );
    }
    swprintf( buf, MAX_STRING_LENGTH-1, L"It belives the religion of %s.\n\r",
	IS_NPC(victim) ? L"None" : religion_table[victim->religion].name);
    send_to_char(buf,ch);
    swprintf( buf, MAX_STRING_LENGTH-1, L"Lv: %d  Class: %s  Align: %s  Silver: %ld  Exp: %d\n\r",
	victim->level,
	IS_NPC(victim) ? L"mobile" : class_table[victim->iclass].name[1],
	buf2,
	victim->silver, victim->exp );
    send_to_char( buf, ch );

    swprintf( buf, MAX_STRING_LENGTH-1, L"Armor: pierce: %d  bash: %d  slash: %d  magic: %d\n\r",
	    GET_AC(victim,AC_PIERCE), GET_AC(victim,AC_BASH),
	    GET_AC(victim,AC_SLASH),  GET_AC(victim,AC_EXOTIC));
    send_to_char(buf,ch);

    swprintf( buf, MAX_STRING_LENGTH-1, L"Hit: %d  Dam: %d  Saves: %d  Size: %s  Position: %s  Wimpy: %d\n\r",
	GET_HITROLL(victim), GET_DAMROLL(victim), victim->saving_throw,
	size_table[victim->size].name, position_table[victim->position].name,
	victim->wimpy );
    send_to_char( buf, ch );

    if (IS_NPC(victim))
    {
	swprintf( buf, MAX_STRING_LENGTH-1, L"Damage: %dd%d  Message:  %s\n\r",
	    victim->damage[DICE_NUMBER],victim->damage[DICE_TYPE],
	    attack_table[victim->dam_type].noun);
	send_to_char(buf,ch);
    }
    swprintf( buf, MAX_STRING_LENGTH-1, L"Fighting: %s Death: %d Carry number: %d  Carry weight: %ld\n\r",
	victim->fighting ? victim->fighting->name : L"(none)"
	,IS_NPC(victim) ? 0 : victim->pcdata->death,
	victim->carry_number, get_carry_weight(victim) / 10 );
    send_to_char( buf, ch );

    if ( !IS_NPC(victim) )
    {
	swprintf( buf, MAX_STRING_LENGTH-1, L"Thirst: %d  Hunger: %d  Full: %d  Drunk: %d Bloodlust: %d Desire: %d\n\r",
	    victim->pcdata->condition[COND_THIRST],
	    victim->pcdata->condition[COND_HUNGER],
	    victim->pcdata->condition[COND_FULL],
	    victim->pcdata->condition[COND_DRUNK],
	    victim->pcdata->condition[COND_BLOODLUST],
	    victim->pcdata->condition[COND_DESIRE] );
	send_to_char( buf, ch );
    }


    if (!IS_NPC(victim))
    {
    	swprintf( buf, MAX_STRING_LENGTH-1, L"Age: %d  Played: %d  Last Level: %d  Timer: %d\n\r",
	    get_age(victim),
	    (int) (victim->played + current_time - victim->logon) / 3600,
	    victim->pcdata->last_level,
	    victim->timer );
    	send_to_char( buf, ch );
    }

    swprintf( buf, MAX_STRING_LENGTH-1, L"Act: %s\n\r",act_bit_name(victim->act));
    send_to_char(buf,ch);

    if (victim->comm)
    {
    	swprintf( buf, MAX_STRING_LENGTH-1, L"Comm: %s\n\r",comm_bit_name(victim->comm));
    	send_to_char(buf,ch);
    }

    if (IS_NPC(victim) && victim->off_flags)
    {
    	swprintf( buf, MAX_STRING_LENGTH-1, L"Offense: %s\n\r",off_bit_name(victim->off_flags));
	send_to_char(buf,ch);
    }

    if (victim->imm_flags)
    {
	swprintf( buf, MAX_STRING_LENGTH-1, L"Immune: %s\n\r",imm_bit_name(victim->imm_flags));
	send_to_char(buf,ch);
    }

    if (victim->res_flags)
    {
	swprintf( buf, MAX_STRING_LENGTH-1, L"Resist: %s\n\r", imm_bit_name(victim->res_flags));
	send_to_char(buf,ch);
    }

    if (victim->vuln_flags)
    {
	swprintf( buf, MAX_STRING_LENGTH-1, L"Vulnerable: %s\n\r", imm_bit_name(victim->vuln_flags));
	send_to_char(buf,ch);
    }

    if (victim->detection)
    {
     swprintf( buf, MAX_STRING_LENGTH-1, L"Detection: %s\n\r",
	(victim->detection) ? detect_bit_name(victim->detection) : L"(none)");
     send_to_char(buf,ch);
    }

    swprintf( buf, MAX_STRING_LENGTH-1, L"Form: %s\n\rParts: %s\n\r",
	form_bit_name(victim->form), part_bit_name(victim->parts));
    send_to_char(buf,ch);

    if (victim->affected_by)
    {
	swprintf( buf, MAX_STRING_LENGTH-1, L"Affected by %s\n\r",
	    affect_bit_name(victim->affected_by));
	send_to_char(buf,ch);
    }

    swprintf( buf, MAX_STRING_LENGTH-1, L"Master: %s  Leader: %s  Pet: %s\n\r",
	victim->master      ? victim->master->name   : L"(none)",
	victim->leader      ? victim->leader->name   : L"(none)",
	victim->pet 	    ? victim->pet->name	     : L"(none)");
    send_to_char( buf, ch );

    swprintf( buf, MAX_STRING_LENGTH-1, L"Short description: %s\n\rLong  description: %s",
	victim->short_descr,
	victim->long_descr[0] != '\0' ? victim->long_descr : L"(none)\n\r" );
    send_to_char( buf, ch );

    if ( IS_NPC(victim) && victim->spec_fun != 0 )
    {
	swprintf( buf, MAX_STRING_LENGTH-1, L"Mobile has special procedure %s.\n\r",
		spec_name(victim->spec_fun));
	send_to_char(buf,ch);
    }

    for ( paf = victim->affected; paf != NULL; paf = paf->next )
    {
	swprintf( buf, MAX_STRING_LENGTH-1, L"Spell: '%s' modifies %s by %d for %d hours with bits %s, level %d.\n\r",
	    skill_table[(int) paf->type].name[1],
	    affect_loc_name( paf->location ),
	    paf->modifier,
	    paf->duration,
	    affect_bit_name( paf->bitvector ),
	    paf->level
	    );
	send_to_char( buf, ch );
    }

    if (!(IS_NPC(victim)))
       {
	if (IS_SET(victim->act,PLR_QUESTOR))
	{
	 swprintf( buf, MAX_STRING_LENGTH-1, L"Questgiver: %d QuestPnts: %d	Questnext: %d\n\r",
		victim->pcdata->questgiver,victim->pcdata->questpoints,
		victim->pcdata->nextquest);
	 send_to_char(buf, ch);
	 swprintf( buf, MAX_STRING_LENGTH-1, L"QuestCntDown: %d	Questmob: %d\n\r",
		victim->pcdata->countdown,victim->pcdata->questmob);
	 send_to_char(buf, ch);
	}
	if  (!IS_SET(victim->act,PLR_QUESTOR))
	{
	 swprintf( buf, MAX_STRING_LENGTH-1, L"QuestPnts: %d	Questnext: %d    NOT QUESTING\n\r",
		victim->pcdata->questpoints,victim->pcdata->nextquest);
	 send_to_char(buf, ch);
	}
       }

    if ( IS_NPC(victim)  )
      if ( victim->pIndexData->progtypes != 0 )  {
        swprintf( buf, MAX_STRING_LENGTH-1, L"Mobile progs: " );
	if ( IS_SET( victim->progtypes, MPROG_BRIBE ) )
	  wcscat( buf, L"bribe " );
	if ( IS_SET( victim->progtypes, MPROG_SPEECH ) )
	  wcscat( buf, L"speech " );
	if ( IS_SET( victim->progtypes, MPROG_GIVE ) )
	  wcscat( buf, L"give " );
	if ( IS_SET( victim->progtypes, MPROG_DEATH ) )
	  wcscat( buf, L"death " );
	if ( IS_SET( victim->progtypes, MPROG_GREET ) )
	  wcscat( buf, L"greet " );
	if ( IS_SET( victim->progtypes, MPROG_ENTRY ) )
	  wcscat( buf, L"entry " );
	if ( IS_SET( victim->progtypes, MPROG_FIGHT ) )
	  wcscat( buf, L"fight " );
	if ( IS_SET( victim->progtypes, MPROG_AREA ) )
	  wcscat( buf, L"area " );
        wcscat( buf, L"\n\r" );
        send_to_char( buf, ch );
    }
    swprintf( buf, MAX_STRING_LENGTH-1, L"Last fought: %10s  Last fight time: %s",
	victim->last_fought!=NULL?victim->last_fought->name: L"none",
	ctime( &(victim->last_fight_time) )     );
    send_to_char( buf, ch );
    swprintf( buf, MAX_STRING_LENGTH-1, L"In_mind: [%s] Hunting: [%s]\n\r",
		victim->in_mind != NULL ? victim->in_mind : (wchar_t*)"none",
		victim->hunting != NULL ? victim->hunting->name : (wchar_t*)"none");
    send_to_char(buf,ch);
    return;
}

void do_vnum(CHAR_DATA *ch, wchar_t *argument)
{
    wchar_t arg[MAX_INPUT_LENGTH];
    wchar_t *string;

    string = one_argument(argument,arg);

    if (arg[0] == '\0')
    {
	send_to_char( L"Syntax:\n\r",ch);
	send_to_char( L"  vnum obj <name>\n\r",ch);
	send_to_char( L"  vnum mob <name>\n\r",ch);
	return;
    }

    if (!wcscasecmp(arg, L"obj"))
    {
	do_ofind(ch,string);
 	return;
    }

    if (!wcscasecmp(arg, L"mob") || !wcscasecmp(arg, L"char"))
    {
	do_mfind(ch,string);
	return;
    }

    /* do both */
    do_mfind(ch,argument);
    do_ofind(ch,argument);
}


void do_mfind( CHAR_DATA *ch, wchar_t *argument )
{
    extern int top_mob_index;
    wchar_t buf[MAX_STRING_LENGTH];
    wchar_t arg[MAX_INPUT_LENGTH];
    MOB_INDEX_DATA *pMobIndex;
    int vnum;
    int nMatch;
    bool fAll;
    bool found;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( L"Find whom?\n\r", ch );
	return;
    }

    fAll	= FALSE; /* !wcscasecmp( arg, L"all" ); */
    found	= FALSE;
    nMatch	= 0;

    /*
     * Yeah, so iterating over all vnum's takes 10,000 loops.
     * Get_mob_index is fast, and I don't feel like threading another link.
     * Do you?
     * -- Furey
     */
    for ( vnum = 0; nMatch < top_mob_index; vnum++ )
    {
	if ( ( pMobIndex = get_mob_index( vnum ) ) != NULL )
	{
	    nMatch++;
	    if ( fAll || is_name( argument, pMobIndex->player_name ) )
	    {
		found = TRUE;
		swprintf( buf, MAX_STRING_LENGTH-1, L"[%5d] %s\n\r",
		    pMobIndex->vnum, pMobIndex->short_descr );
		send_to_char( buf, ch );
	    }
	}
    }

    if ( !found )
	send_to_char( L"No mobiles by that name.\n\r", ch );

    return;
}



void do_ofind( CHAR_DATA *ch, wchar_t *argument )
{
    extern int top_obj_index;
    wchar_t buf[MAX_STRING_LENGTH];
    wchar_t arg[MAX_INPUT_LENGTH];
    OBJ_INDEX_DATA *pObjIndex;
    int vnum;
    int nMatch;
    bool fAll;
    bool found;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( L"Find what?\n\r", ch );
	return;
    }

    fAll	= FALSE; /* !wcscasecmp( arg, L"all" ); */
    found	= FALSE;
    nMatch	= 0;

    /*
     * Yeah, so iterating over all vnum's takes 10,000 loops.
     * Get_obj_index is fast, and I don't feel like threading another link.
     * Do you?
     * -- Furey
     */
    for ( vnum = 0; nMatch < top_obj_index; vnum++ )
    {
	if ( ( pObjIndex = get_obj_index( vnum ) ) != NULL )
	{
	    nMatch++;
	    if ( fAll || is_name( argument, pObjIndex->name ) )
	    {
		found = TRUE;
		swprintf( buf, MAX_STRING_LENGTH-1, L"[%5d] %s%s\n\r",
		    pObjIndex->vnum, pObjIndex->short_descr,
	(IS_OBJ_STAT(pObjIndex,ITEM_GLOW) && CAN_WEAR(pObjIndex,ITEM_WEAR_HEAD))
		? " [parlayan]" : "" );
		send_to_char( buf, ch );
	    }
	}
    }

    if ( !found )
	send_to_char( L"No objects by that name.\n\r", ch );

    return;
}


void do_owhere(CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t buf[MAX_INPUT_LENGTH];
    BUFFER *buffer;
    OBJ_DATA *obj;
    OBJ_DATA *in_obj;
    bool found;
    int number = 0, max_found;

    found = FALSE;
    number = 0;
    max_found = 200;

    buffer = new_buf();

    if (argument[0] == '\0')
    {
	send_to_char( L"Find what?\n\r",ch);
	return;
    }

    for ( obj = object_list; obj != NULL; obj = obj->next )
    {
        if ( !can_see_obj( ch, obj ) || !is_name( argument, obj->name )
        ||   ch->level < obj->level)
            continue;

        found = TRUE;
        number++;

        for ( in_obj = obj; in_obj->in_obj != NULL; in_obj = in_obj->in_obj )
            ;

        if ( in_obj->carried_by != NULL && can_see(ch,in_obj->carried_by)
	&&   in_obj->carried_by->in_room != NULL)
            swprintf( buf, MAX_STRING_LENGTH-1, L"%3d) %s is carried by %s [Room %d]\n\r",
                number, obj->short_descr,PERS(in_obj->carried_by, ch),
		in_obj->carried_by->in_room->vnum );
        else if (in_obj->in_room != NULL && can_see_room(ch,in_obj->in_room))
            swprintf( buf, MAX_STRING_LENGTH-1, L"%3d) %s is in %s [Room %d]\n\r",
                number, obj->short_descr,in_obj->in_room->name,
	   	in_obj->in_room->vnum);
	else
            swprintf( buf, MAX_STRING_LENGTH-1, L"%3d) %s is somewhere\n\r",number, obj->short_descr);

        buf[0] = towupper(buf[0]);
        add_buf(buffer,buf);

        if (number >= max_found)
            break;
    }

    if ( !found )
        send_to_char( L"Ne bu dünyada ne de diğerinde böyle bir şey yok.\n\r", ch );
    else
        page_to_char(buf_string(buffer),ch);

    free_buf(buffer);
}


void do_mwhere( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t buf[MAX_STRING_LENGTH];
    BUFFER *buffer;
    CHAR_DATA *victim;
    bool found;
    int count = 0;

    if ( argument[0] == '\0' )
    {
	DESCRIPTOR_DATA *d;

	/* show characters logged */

	buffer = new_buf();
	for (d = descriptor_list; d != NULL; d = d->next)
	{
	    if (d->character != NULL && d->connected == CON_PLAYING
	    &&  d->character->in_room != NULL && can_see(ch,d->character)
	    &&  can_see_room(ch,d->character->in_room))
	    {
		victim = d->character;
		count++;
		if (d->original != NULL)
		    swprintf( buf, MAX_STRING_LENGTH-1, L"%3d) %s (in the body of %s) is in %s [%d]\n\r",
			count, d->original->name,victim->short_descr,
			victim->in_room->name,victim->in_room->vnum);
		else
		    swprintf( buf, MAX_STRING_LENGTH-1, L"%3d) %s is in %s [%d]\n\r",
			count, victim->name,victim->in_room->name,
			victim->in_room->vnum);
		add_buf(buffer,buf);
	    }
	}

        page_to_char(buf_string(buffer),ch);
	free_buf(buffer);
	return;
    }

    found = FALSE;
    buffer = new_buf();
    for ( victim = char_list; victim != NULL; victim = victim->next )
    {
	if ( victim->in_room != NULL
	&&   is_name( argument, victim->name ) )
	{
	    found = TRUE;
	    count++;
	    swprintf( buf, MAX_STRING_LENGTH-1, L"%3d) [%5d] %-28s [%5d] %s\n\r", count,
		IS_NPC(victim) ? victim->pIndexData->vnum : 0,
		IS_NPC(victim) ? victim->short_descr : victim->name,
		victim->in_room->vnum,
		victim->in_room->name );
	    add_buf(buffer,buf);
	}
    }

    if ( !found )
	act( L"You didn't find any $T.", ch, NULL, argument, TO_CHAR );
    else
    	page_to_char(buf_string(buffer),ch);

    free_buf(buffer);

    return;
}



void do_reboo( CHAR_DATA *ch, wchar_t *argument )
{
    send_to_char( L"If you want to REBOOT, spell it out.\n\r", ch );
    return;
}



void do_shutdow( CHAR_DATA *ch, wchar_t *argument )
{
    send_to_char( L"If you want to SHUTDOWN, spell it out.\n\r", ch );
    return;
}



void do_shutdown( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t buf[MAX_STRING_LENGTH];

    if (ch->invis_level < LEVEL_HERO)
    swprintf( buf, MAX_STRING_LENGTH-1, L"Shutdown by %s.", ch->name );
    append_file( ch, (wchar_t*)SHUTDOWN_FILE, buf );
    wcscat(buf, L"\n\r" );
    if (ch->invis_level < LEVEL_HERO)
    	do_duyuru( ch, buf );
    reboot_uzakdiyarlar(FALSE);
    return;
}

void do_protect( CHAR_DATA *ch, wchar_t *argument)
{
    CHAR_DATA *victim;

    if (argument[0] == '\0')
    {
	send_to_char( L"Protect whom from snooping?\n\r",ch);
	return;
    }

    if ((victim = get_char_world(ch,argument)) == NULL)
    {
	send_to_char( L"You can't find them.\n\r",ch);
	return;
    }

    if (IS_SET(victim->comm,COMM_SNOOP_PROOF))
    {
	act_new( L"$N is no longer snoop-proof.",ch,NULL,victim,TO_CHAR,POS_DEAD);
	send_to_char( L"Your snoop-proofing was just removed.\n\r",victim);
	REMOVE_BIT(victim->comm,COMM_SNOOP_PROOF);
    }
    else
    {
	act_new( L"$N is now snoop-proof.",ch,NULL,victim,TO_CHAR,POS_DEAD);
	send_to_char( L"You are now immune to snooping.\n\r",victim);
	SET_BIT(victim->comm,COMM_SNOOP_PROOF);
    }
}



void do_snoop( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH];
    DESCRIPTOR_DATA *d;
    CHAR_DATA *victim;
    wchar_t buf[MAX_STRING_LENGTH];

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( L"Snoop whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( L"They aren't here.\n\r", ch );
	return;
    }

    if ( victim->desc == NULL )
    {
	send_to_char( L"No descriptor to snoop.\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( L"Cancelling all snoops.\n\r", ch );
	wiznet( L"$N stops being such a snoop.",
		ch,NULL,WIZ_SNOOPS,WIZ_SECURE,get_trust(ch));
	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    if ( d->snoop_by == ch->desc )
		d->snoop_by = NULL;
	}
	return;
    }

    if ( victim->desc->snoop_by != NULL )
    {
	send_to_char( L"Busy already.\n\r", ch );
	return;
    }

    if (!is_room_owner(ch,victim->in_room) && ch->in_room != victim->in_room
    &&  room_is_private(victim->in_room) && !IS_TRUSTED(ch,IMPLEMENTOR))
    {
        send_to_char( L"That character is in a private room.\n\r",ch);
        return;
    }

    if ( get_trust( victim ) >= get_trust( ch )
    ||   IS_SET(victim->comm,COMM_SNOOP_PROOF))
    {
	send_to_char( L"You failed.\n\r", ch );
	return;
    }

    if ( ch->desc != NULL )
    {
	for ( d = ch->desc->snoop_by; d != NULL; d = d->snoop_by )
	{
	    if ( d->character == victim || d->original == victim )
	    {
		send_to_char( L"No snoop loops.\n\r", ch );
		return;
	    }
	}
    }

    victim->desc->snoop_by = ch->desc;
    swprintf( buf, MAX_STRING_LENGTH-1, L"$N starts snooping on %s",
	(IS_NPC(ch) ? victim->short_descr : victim->name));
    wiznet(buf,ch,NULL,WIZ_SNOOPS,WIZ_SECURE,get_trust(ch));
    send_to_char( L"Ok.\n\r", ch );
    return;
}



void do_switch( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH], buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( L"Switch into whom?\n\r", ch );
	return;
    }

    if ( ch->desc == NULL )
	return;

    if ( ch->desc->original != NULL )
    {
	send_to_char( L"You are already switched.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( L"They aren't here.\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( L"Ok.\n\r", ch );
	return;
    }

    if (!IS_NPC(victim))
    {
	send_to_char( L"You can only switch into mobiles.\n\r",ch);
	return;
    }

    if (!is_room_owner(ch,victim->in_room) && ch->in_room != victim->in_room
    &&  room_is_private(victim->in_room) && !IS_TRUSTED(ch,IMPLEMENTOR))
    {
	send_to_char( L"That character is in a private room.\n\r",ch);
	return;
    }

    if ( victim->desc != NULL )
    {
	send_to_char( L"Character in use.\n\r", ch );
	return;
    }

    swprintf( buf, MAX_STRING_LENGTH-1, L"$N switches into %s",victim->short_descr);
    wiznet(buf,ch,NULL,WIZ_SWITCHES,WIZ_SECURE,get_trust(ch));

    ch->desc->character = victim;
    ch->desc->original  = ch;
    victim->desc        = ch->desc;
    ch->desc            = NULL;
    /* change communications to match */
    if (ch->prompt != NULL)
        victim->prompt = str_dup(ch->prompt);
    victim->comm = ch->comm;
    victim->lines = ch->lines;
    send_to_char( L"Ok.\n\r", victim );
    return;
}



void do_return( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t buf[MAX_STRING_LENGTH];

    if ( ch->desc == NULL )
	return;

    if ( ch->desc->original == NULL )
    {
	send_to_char( L"You aren't switched.\n\r", ch );
	return;
    }

    send_to_char( L"You return to your original body. Type replay to see any missed tells.\n\r",ch );
    if (ch->prompt != NULL)
    {
	free_string(ch->prompt);
	ch->prompt = NULL;
    }

    swprintf( buf, MAX_STRING_LENGTH-1, L"$N returns from %s.",ch->short_descr);
    wiznet(buf,ch->desc->original,0,WIZ_SWITCHES,WIZ_SECURE,get_trust(ch));
    ch->desc->character       = ch->desc->original;
    ch->desc->original        = NULL;
    ch->desc->character->desc = ch->desc;
    ch->desc                  = NULL;
    return;
}

/* trust levels for load and clone */
bool obj_check (CHAR_DATA *ch, OBJ_DATA *obj)
{
    if (IS_TRUSTED(ch,GOD)
	|| (IS_TRUSTED(ch,IMMORTAL) && obj->level <= 20 && obj->cost <= 1000)
	|| (IS_TRUSTED(ch,DEMI)	    && obj->level <= 10 && obj->cost <= 500)
	|| (IS_TRUSTED(ch,ANGEL)    && obj->level <=  5 && obj->cost <= 250)
	|| (IS_TRUSTED(ch,AVATAR)   && obj->level ==  0 && obj->cost <= 100))
	return TRUE;
    else
	return FALSE;
}

/* for clone, to insure that cloning goes many levels deep */
void recursive_clone(CHAR_DATA *ch, OBJ_DATA *obj, OBJ_DATA *clone)
{
    OBJ_DATA *c_obj, *t_obj;


    for (c_obj = obj->contains; c_obj != NULL; c_obj = c_obj->next_content)
    {
	if (obj_check(ch,c_obj))
	{
	    t_obj = create_object(c_obj->pIndexData,0);
	    clone_object(c_obj,t_obj);
	    obj_to_obj(t_obj,clone);
	    recursive_clone(ch,c_obj,t_obj);
	}
    }
}

/* command that is similar to load */
void do_clone(CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH];
    wchar_t *rest;
    CHAR_DATA *mob;
    OBJ_DATA  *obj;

    rest = one_argument(argument,arg);

    if (arg[0] == '\0')
    {
	send_to_char( L"Clone what?\n\r",ch);
	return;
    }

    if (!str_prefix(arg, L"object"))
    {
	mob = NULL;
	obj = get_obj_here(ch,rest);
	if (obj == NULL)
	{
	    send_to_char( L"You don't see that here.\n\r",ch);
	    return;
	}
    }
    else if (!str_prefix(arg, L"mobile") || !str_prefix(arg, L"character"))
    {
	obj = NULL;
	mob = get_char_room(ch,rest);
	if (mob == NULL)
	{
	    send_to_char( L"You don't see that here.\n\r",ch);
	    return;
	}
    }
    else /* find both */
    {
	mob = get_char_room(ch,argument);
	obj = get_obj_here(ch,argument);
	if (mob == NULL && obj == NULL)
	{
	    send_to_char( L"You don't see that here.\n\r",ch);
	    return;
	}
    }

    /* clone an object */
    if (obj != NULL)
    {
	OBJ_DATA *clone;

	if (!obj_check(ch,obj))
	{
	    send_to_char(L"Your powers are not great enough for such a task.\n\r",ch);
	    return;
	}

	clone = create_object(obj->pIndexData,0);
	clone_object(obj,clone);
	if (obj->carried_by != NULL)
	    obj_to_char(clone,ch);
	else
	    obj_to_room(clone,ch->in_room);
 	recursive_clone(ch,obj,clone);

	act( L"$n has created $p.",ch,clone,NULL,TO_ROOM);
	act( L"You clone $p.",ch,clone,NULL,TO_CHAR);
	wiznet( L"$N clones $p.",ch,clone,WIZ_LOAD,WIZ_SECURE,get_trust(ch));
	return;
    }
    else if (mob != NULL)
    {
	CHAR_DATA *clone;
	OBJ_DATA *new_obj;
	wchar_t buf[MAX_STRING_LENGTH];

	if (!IS_NPC(mob))
	{
	    send_to_char( L"You can only clone mobiles.\n\r",ch);
	    return;
	}

	if ((mob->level > 20 && !IS_TRUSTED(ch,GOD))
	||  (mob->level > 10 && !IS_TRUSTED(ch,IMMORTAL))
	||  (mob->level >  5 && !IS_TRUSTED(ch,DEMI))
	||  (mob->level >  0 && !IS_TRUSTED(ch,ANGEL))
	||  !IS_TRUSTED(ch,AVATAR))
	{
	    send_to_char(L"Your powers are not great enough for such a task.\n\r",ch);
	    return;
	}

	clone = create_mobile(mob->pIndexData, NULL);
	clone_mobile(mob,clone);

	for (obj = mob->carrying; obj != NULL; obj = obj->next_content)
	{
	    if (obj_check(ch,obj))
	    {
		new_obj = create_object(obj->pIndexData,0);
		clone_object(obj,new_obj);
		recursive_clone(ch,obj,new_obj);
		obj_to_char(new_obj,clone);
		new_obj->wear_loc = obj->wear_loc;
	    }
	}
	char_to_room(clone,ch->in_room);
        act( L"$n has created $N.",ch,NULL,clone,TO_ROOM);
        act( L"You clone $N.",ch,NULL,clone,TO_CHAR);
	swprintf( buf, MAX_STRING_LENGTH-1, L"$N clones %s.",clone->short_descr);
	wiznet(buf,ch,NULL,WIZ_LOAD,WIZ_SECURE,get_trust(ch));
        return;
    }
}

/* RT to replace the two load commands */

void do_load(CHAR_DATA *ch, wchar_t *argument )
{
   wchar_t arg[MAX_INPUT_LENGTH];

    argument = one_argument(argument,arg);

    if (arg[0] == '\0')
    {
	send_to_char( L"Syntax:\n\r",ch);
	send_to_char( L"  load mob <vnum>\n\r",ch);
	send_to_char( L"  load obj <vnum> <level>\n\r",ch);
	return;
    }

    if (!wcscasecmp(arg, L"mob") || !wcscasecmp(arg, L"char"))
    {
	do_mload(ch,argument);
	return;
    }

    if (!wcscasecmp(arg, L"obj"))
    {
	do_oload(ch,argument);
	return;
    }
    /* echo syntax */
    do_load(ch,(wchar_t*)"");
}


void do_mload( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH];
    MOB_INDEX_DATA *pMobIndex;
    CHAR_DATA *victim;
    wchar_t buf[MAX_STRING_LENGTH];

    one_argument( argument, arg );

    if ( arg[0] == '\0' || !is_number(arg) )
    {
	send_to_char( L"Syntax: load mob <vnum>.\n\r", ch );
	return;
    }

    if ( ( pMobIndex = get_mob_index( wcstol( arg, 0, 10 ) ) ) == NULL )
    {
	send_to_char( L"No mob has that vnum.\n\r", ch );
	return;
    }

    victim = create_mobile( pMobIndex , NULL);
    char_to_room( victim, ch->in_room );
    act( L"$n has created $N!", ch, NULL, victim, TO_ROOM );
    swprintf( buf, MAX_STRING_LENGTH-1, L"$N loads %s.",victim->short_descr);
    wiznet(buf,ch,NULL,WIZ_LOAD,WIZ_SECURE,get_trust(ch));
    send_to_char( L"Ok.\n\r", ch );
    return;
}



void do_oload( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg1[MAX_INPUT_LENGTH] ,arg2[MAX_INPUT_LENGTH];
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    int level;

    argument = one_argument( argument, arg1 );
    one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || !is_number(arg1))
    {
	send_to_char( L"Syntax: load obj <vnum> <level>.\n\r", ch );
	return;
    }

    level = get_trust(ch); /* default */

    if ( arg2[0] != '\0')  /* load with a level */
    {
	if (!is_number(arg2))
        {
	  send_to_char( L"Syntax: oload <vnum> <level>.\n\r", ch );
	  return;
	}
        level = wcstol(arg2, 0, 10 );
        if (level < 0 || level > get_trust(ch))
	{
	  send_to_char( L"Level must be be between 0 and your level.\n\r",ch);
  	  return;
	}
    }

    if ( ( pObjIndex = get_obj_index( wcstol( arg1, 0, 10 ) ) ) == NULL )
    {
	send_to_char( L"No object has that vnum.\n\r", ch );
	return;
    }

    obj = create_object( pObjIndex, level );
    if ( CAN_WEAR(obj, ITEM_TAKE) )
	obj_to_char( obj, ch );
    else
	obj_to_room( obj, ch->in_room );
    act( L"$n has created $p!", ch, obj, NULL, TO_ROOM );
    wiznet( L"$N loads $p.",ch,obj,WIZ_LOAD,WIZ_SECURE,get_trust(ch));
    send_to_char( L"Ok.\n\r", ch );
    return;
}



void do_purge( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH];
    wchar_t buf[100];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    DESCRIPTOR_DATA *d;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	/* 'purge' */
	CHAR_DATA *vnext;
	OBJ_DATA  *obj_next;

	for ( victim = ch->in_room->people; victim != NULL; victim = vnext )
	{
	    vnext = victim->next_in_room;
	    if ( IS_NPC(victim) && !IS_SET(victim->act,ACT_NOPURGE)
	    &&   victim != ch /* safety precaution */ )
		extract_char( victim, TRUE );
	}

	for ( obj = ch->in_room->contents; obj != NULL; obj = obj_next )
	{
	    obj_next = obj->next_content;
	    if (!IS_OBJ_STAT(obj,ITEM_NOPURGE))
	      extract_obj( obj );
	}

	act( L"$n purges the room!", ch, NULL, NULL, TO_ROOM);
	send_to_char( L"Ok.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( L"They aren't here.\n\r", ch );
	return;
    }

    if ( !IS_NPC(victim) )
    {

	if (ch == victim)
	{
	  send_to_char( L"Ho ho ho.\n\r",ch);
	  return;
	}

	if (get_trust(ch) <= get_trust(victim))
	{
	  send_to_char( L"Maybe that wasn't a good idea...\n\r",ch);
	  swprintf( buf, MAX_STRING_LENGTH-1, L"%s tried to purge you!\n\r",ch->name);
	  send_to_char(buf,victim);
	  return;
	}

	act( L"$n disintegrates $N.",ch,0,victim,TO_NOTVICT);

    	if (victim->level > 1)
	    save_char_obj( victim );
    	d = victim->desc;
    	extract_char( victim, TRUE );
    	if ( d != NULL )
          close_socket( d );

	return;
    }

    act( L"$n purges $N.", ch, NULL, victim, TO_NOTVICT );
    extract_char( victim, TRUE );
    return;
}


void do_trust( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg1[MAX_INPUT_LENGTH];
    wchar_t arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int level;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' || !is_number( arg2 ) )
    {
	send_to_char( L"Syntax: trust <char> <level>.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( L"That player is not here.\n\r", ch);
	return;
    }

    if ( ( level = wcstol( arg2, 0, 10 ) ) < 0 || level > 100 )
    {
	send_to_char( L"Level must be 0 (reset) or 1 to 100.\n\r", ch );
	return;
    }

    if ( level > get_trust( ch ) )
    {
	send_to_char( L"Limited to your trust.\n\r", ch );
	return;
    }

    victim->trust = level;
    return;
}



void do_restore( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH], buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *vch;
    DESCRIPTOR_DATA *d;

    one_argument( argument, arg );
    if (arg[0] == '\0' || !wcscasecmp(arg, L"room"))
    {
    /* cure room */

        for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
        {
            affect_strip(vch,gsn_plague);
            affect_strip(vch,gsn_poison);
            affect_strip(vch,gsn_blindness);
            affect_strip(vch,gsn_sleep);
            affect_strip(vch,gsn_curse);

            vch->hit 	= vch->max_hit;
            vch->mana	= vch->max_mana;
            vch->move	= vch->max_move;
            update_pos( vch);
            act( L"$n has restored you.",ch,NULL,vch,TO_VICT);
        }

        swprintf( buf, MAX_STRING_LENGTH-1, L"$N restored room %d.",ch->in_room->vnum);
        wiznet(buf,ch,NULL,WIZ_RESTORE,WIZ_SECURE,get_trust(ch));

        send_to_char( L"Room restored.\n\r",ch);
        return;

    }

    if ( get_trust(ch) >=  MAX_LEVEL - 1 && !wcscasecmp(arg, L"all"))
    {
    /* cure all */

        for (d = descriptor_list; d != NULL; d = d->next)
        {
	    victim = d->character;

	    if (victim == NULL || IS_NPC(victim))
		continue;

            affect_strip(victim,gsn_plague);
            affect_strip(victim,gsn_poison);
            affect_strip(victim,gsn_blindness);
            affect_strip(victim,gsn_sleep);
            affect_strip(victim,gsn_curse);

            victim->hit 	= victim->max_hit;
            victim->mana	= victim->max_mana;
            victim->move	= victim->max_move;
            update_pos( victim);
	    if (victim->in_room != NULL)
                act( L"$n has restored you.",ch,NULL,victim,TO_VICT);
        }
	send_to_char( L"All active players restored.\n\r",ch);
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( L"They aren't here.\n\r", ch );
	return;
    }

    affect_strip(victim,gsn_plague);
    affect_strip(victim,gsn_poison);
    affect_strip(victim,gsn_blindness);
    affect_strip(victim,gsn_sleep);
    affect_strip(victim,gsn_curse);
    victim->hit  = victim->max_hit;
    victim->mana = victim->max_mana;
    victim->move = victim->max_move;
    update_pos( victim );
    act( L"$n has restored you.", ch, NULL, victim, TO_VICT );
    swprintf( buf, MAX_STRING_LENGTH-1, L"$N restored %s",
	IS_NPC(victim) ? victim->short_descr : victim->name);
    wiznet(buf,ch,NULL,WIZ_RESTORE,WIZ_SECURE,get_trust(ch));
    send_to_char( L"Ok.\n\r", ch );
    return;
}


void do_freeze( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH],buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( L"Freeze whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( L"They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( L"Not on NPC's.\n\r", ch );
	return;
    }

    if ( get_trust( victim ) >= get_trust( ch ) )
    {
	send_to_char( L"You failed.\n\r", ch );
	return;
    }

    if ( IS_SET(victim->act, PLR_FREEZE) )
    {
	REMOVE_BIT(victim->act, PLR_FREEZE);
	send_to_char( L"You can play again.\n\r", victim );
	send_to_char( L"FREEZE removed.\n\r", ch );
	swprintf( buf, MAX_STRING_LENGTH-1, L"$N thaws %s.",victim->name);
	wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
    }
    else
    {
	SET_BIT(victim->act, PLR_FREEZE);
	send_to_char( L"You can't do ANYthing!\n\r", victim );
	send_to_char( L"FREEZE set.\n\r", ch );
	swprintf( buf, MAX_STRING_LENGTH-1, L"$N puts %s in the deep freeze.",victim->name);
	wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
    }

    save_char_obj( victim );

    return;
}



void do_log( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( L"Log whom?\n\r", ch );
	return;
    }

    if ( !wcscasecmp( arg, L"all" ) )
    {
	if ( fLogAll )
	{
	    fLogAll = FALSE;
	    send_to_char( L"Log ALL off.\n\r", ch );
	}
	else
	{
	    fLogAll = TRUE;
	    send_to_char( L"Log ALL on.\n\r", ch );
	}
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( L"They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( L"Not on NPC's.\n\r", ch );
	return;
    }

    /*
     * No level check, gods can log anyone.
     */
    if ( IS_SET(victim->act, PLR_LOG) )
    {
	REMOVE_BIT(victim->act, PLR_LOG);
	send_to_char( L"LOG removed.\n\r", ch );
    }
    else
    {
	SET_BIT(victim->act, PLR_LOG);
	send_to_char( L"LOG set.\n\r", ch );
    }

    return;
}



void do_noemote( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH],buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( L"Noemote whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( L"They aren't here.\n\r", ch );
	return;
    }


    if ( get_trust( victim ) >= get_trust( ch ) )
    {
	send_to_char( L"You failed.\n\r", ch );
	return;
    }

    if ( IS_SET(victim->comm, COMM_NOEMOTE) )
    {
	REMOVE_BIT(victim->comm, COMM_NOEMOTE);
	send_to_char( L"You can emote again.\n\r", victim );
	send_to_char( L"NOEMOTE removed.\n\r", ch );
	swprintf( buf, MAX_STRING_LENGTH-1, L"$N restores emotes to %s.",victim->name);
	wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
    }
    else
    {
	SET_BIT(victim->comm, COMM_NOEMOTE);
	send_to_char( L"You can't emote!\n\r", victim );
	send_to_char( L"NOEMOTE set.\n\r", ch );
	swprintf( buf, MAX_STRING_LENGTH-1, L"$N revokes %s's emotes.",victim->name);
	wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
    }

    return;
}



void do_noshout( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH],buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( L"Noshout whom?\n\r",ch);
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( L"They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( L"Not on NPC's.\n\r", ch );
	return;
    }

    if ( get_trust( victim ) >= get_trust( ch ) )
    {
	send_to_char( L"You failed.\n\r", ch );
	return;
    }

    if ( IS_SET(victim->comm, COMM_NOSHOUT) )
    {
	REMOVE_BIT(victim->comm, COMM_NOSHOUT);
	send_to_char( L"You can shout again.\n\r", victim );
	send_to_char( L"NOSHOUT removed.\n\r", ch );
	swprintf( buf, MAX_STRING_LENGTH-1, L"$N restores shouts to %s.",victim->name);
	wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
    }
    else
    {
	SET_BIT(victim->comm, COMM_NOSHOUT);
	send_to_char( L"You can't shout!\n\r", victim );
	send_to_char( L"NOSHOUT set.\n\r", ch );
	swprintf( buf, MAX_STRING_LENGTH-1, L"$N revokes %s's shouts.",victim->name);
	wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
    }

    return;
}



void do_notell( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH],buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( L"Notell whom?", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( L"They aren't here.\n\r", ch );
	return;
    }

    if ( get_trust( victim ) >= get_trust( ch ) )
    {
	send_to_char( L"You failed.\n\r", ch );
	return;
    }

    if ( IS_SET(victim->comm, COMM_NOTELL) )
    {
	REMOVE_BIT(victim->comm, COMM_NOTELL);
	send_to_char( L"You can tell again.\n\r", victim );
	send_to_char( L"NOTELL removed.\n\r", ch );
	swprintf( buf, MAX_STRING_LENGTH-1, L"$N restores tells to %s.",victim->name);
	wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
    }
    else
    {
	SET_BIT(victim->comm, COMM_NOTELL);
	send_to_char( L"You can't tell!\n\r", victim );
	send_to_char( L"NOTELL set.\n\r", ch );
	swprintf( buf, MAX_STRING_LENGTH-1, L"$N revokes %s's tells.",victim->name);
	wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
    }

    return;
}



void do_peace( CHAR_DATA *ch, wchar_t *argument )
{
    CHAR_DATA *rch;

    for ( rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room )
    {
	if ( rch->fighting != NULL )
	    stop_fighting( rch, TRUE );
	if (IS_NPC(rch) && IS_SET(rch->act,ACT_AGGRESSIVE))
	    REMOVE_BIT(rch->act,ACT_AGGRESSIVE);
    }

    send_to_char( L"Ok.\n\r", ch );
    return;
}

void do_wizlock( CHAR_DATA *ch, wchar_t *argument )
{
    extern bool wizlock;
    wizlock = !wizlock;

    if ( wizlock )
    {
	wiznet( L"$N has wizlocked the game.",ch,NULL,0,0,0);
	send_to_char( L"Game wizlocked.\n\r", ch );
    }
    else
    {
	wiznet( L"$N removes wizlock.",ch,NULL,0,0,0);
	send_to_char( L"Game un-wizlocked.\n\r", ch );
    }

    return;
}

/* RT anti-newbie code */

void do_newlock( CHAR_DATA *ch, wchar_t *argument )
{
    extern bool newlock;
    newlock = !newlock;

    if ( newlock )
    {
	wiznet( L"$N locks out new characters.",ch,NULL,0,0,0);
        send_to_char( L"New characters have been locked out.\n\r", ch );
    }
    else
    {
	wiznet( L"$N allows new characters back in.",ch,NULL,0,0,0);
        send_to_char( L"Newlock removed.\n\r", ch );
    }

    return;
}


void do_slookup( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t buf[MAX_STRING_LENGTH];
    wchar_t arg[MAX_INPUT_LENGTH];
    int sn;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( L"Lookup which skill or spell?\n\r", ch );
	return;
    }

    if ( !wcscasecmp( arg, L"all" ) )
    {
	for ( sn = 0; sn < MAX_SKILL; sn++ )
	{
	    if ( skill_table[sn].name == NULL )
		break;
	    swprintf( buf, MAX_STRING_LENGTH-1, L"Sn: %3d  Slot: %3d  Skill/spell: '%s'\n\r",
		sn, skill_table[sn].slot, skill_table[sn].name[1] );
	    send_to_char( buf, ch );
	}
    }
    else
    {
	if ( ( sn = skill_lookup( arg ) ) < 0 )
	{
	    send_to_char( L"No such skill or spell.\n\r", ch );
	    return;
	}

	swprintf( buf, MAX_STRING_LENGTH-1, L"Sn: %3d  Slot: %3d  Skill/spell: '%s'\n\r",
	    sn, skill_table[sn].slot, skill_table[sn].name[1] );
	send_to_char( buf, ch );
    }

    return;
}

/* RT set replaces sset, mset, oset, and rset */

void do_set( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH];

    argument = one_argument(argument,arg);

    if (arg[0] == '\0')
    {
	send_to_char( L"Syntax:\n\r",ch);
	send_to_char( L"  set mob   <name> <field> <value>\n\r",ch);
	send_to_char( L"  set obj   <name> <field> <value>\n\r",ch);
	send_to_char( L"  set room  <room> <field> <value>\n\r",ch);
        send_to_char( L"  set skill <name> <spell or skill> <value>\n\r",ch);
	return;
    }

    if (!str_prefix(arg,L"mobile") || !str_prefix(arg,L"character"))
    {
	do_mset(ch,argument);
	return;
    }

    if (!str_prefix(arg,L"skill") || !str_prefix(arg,L"spell"))
    {
	do_sset(ch,argument);
	return;
    }

    if (!str_prefix(arg,L"object"))
    {
	do_oset(ch,argument);
	return;

    }

    if (!str_prefix(arg,L"room"))
    {
	do_rset(ch,argument);
	return;
    }
    /* echo syntax */
    do_set(ch,(wchar_t*)"");
}


void do_sset( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg1 [MAX_INPUT_LENGTH];
    wchar_t arg2 [MAX_INPUT_LENGTH];
    wchar_t arg3 [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int value;
    int sn;
    bool fAll;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    argument = one_argument( argument, arg3 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
    {
	send_to_char( L"Syntax:\n\r",ch);
	send_to_char( L"  set skill <name> <spell or skill> <value>\n\r", ch);
	send_to_char( L"  set skill <name> all <value>\n\r",ch);
	send_to_char( L"   (use the name of the skill, not the number)\n\r",ch);
	return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( L"They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( L"Not on NPC's.\n\r", ch );
	return;
    }

    fAll = !wcscasecmp( arg2, L"all" );
    sn   = 0;
    if ( !fAll && ( sn = skill_lookup( arg2 ) ) < 0 )
    {
	send_to_char( L"No such skill or spell.\n\r", ch );
	return;
    }

    /*
     * Snarf the value.
     */
    if ( !is_number( arg3 ) )
    {
	send_to_char( L"Value must be numeric.\n\r", ch );
	return;
    }

    value = wcstol( arg3, 0, 10 );
    if ( value < 0 || value > 100 )
    {
	send_to_char( L"Value range is 0 to 100.\n\r", ch );
	return;
    }

    if ( fAll )
    {
	for ( sn = 0; sn < MAX_SKILL; sn++ )
	{
	    if ( ( skill_table[sn].name != NULL )
		&& ( (victim->cabal == skill_table[sn].cabal )
		|| (skill_table[sn].cabal == CABAL_NONE) )
		&& ( RACE_OK(victim,sn) )
		)
		victim->pcdata->learned[sn]	= value;
	}
    }
    else
    {
	victim->pcdata->learned[sn] = value;
    }

    return;
}


void do_string( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t type [MAX_INPUT_LENGTH];
    wchar_t arg1 [MAX_INPUT_LENGTH];
    wchar_t arg2 [MAX_INPUT_LENGTH];
    wchar_t arg3 [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;

    smash_tilde( argument );
    argument = one_argument( argument, type );
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    wcscpy( arg3, argument );

    if ( type[0] == '\0' || arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
    {
	send_to_char( L"Syntax:\n\r",ch);
	send_to_char( L"  string char <name> <field> <string>\n\r",ch);
	send_to_char( L"    fields: name short long desc title spec\n\r",ch);
	send_to_char( L"  string obj  <name> <field> <string>\n\r",ch);
	send_to_char( L"    fields: name short long extended\n\r",ch);
	return;
    }

    if (!str_prefix(type,L"character") || !str_prefix(type,L"mobile"))
    {
    	if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    	{
	    send_to_char( L"They aren't here.\n\r", ch );
	    return;
    	}

	/* clear zone for mobs */
	victim->zone = NULL;

	/* string something */

     	if ( !str_prefix( arg2, L"name" ) )
    	{
	    if ( !IS_NPC(victim) )
	    {
	    	send_to_char( L"Not on PC's.\n\r", ch );
	    	return;
	    }
	    free_string( victim->name );
	    victim->name = str_dup( arg3 );
	    return;
    	}

    	if ( !str_prefix( arg2, L"description" ) )
    	{
    	    free_string(victim->description);
    	    victim->description = str_dup(arg3);
    	    return;
    	}

    	if ( !str_prefix( arg2, L"short" ) )
    	{
	    free_string( victim->short_descr );
	    victim->short_descr = str_dup( arg3 );
	    return;
    	}

    	if ( !str_prefix( arg2, L"long" ) )
    	{
	    free_string( victim->long_descr );
	    wcscat(arg3, L"\n\r");
	    victim->long_descr = str_dup( arg3 );
	    return;
    	}

    	if ( !str_prefix( arg2, L"title" ) )
    	{
	    if ( IS_NPC(victim) )
	    {
	    	send_to_char( L"Not on NPC's.\n\r", ch );
	    	return;
	    }

	    set_title( victim, arg3 );
	    return;
    	}

    	if ( !str_prefix( arg2, L"spec" ) )
    	{
	    if ( !IS_NPC(victim) )
	    {
	    	send_to_char( L"Not on PC's.\n\r", ch );
	    	return;
	    }

	    if ( ( victim->spec_fun = spec_lookup( arg3 ) ) == 0 )
	    {
	    	send_to_char( L"No such spec fun.\n\r", ch );
	    	return;
	    }

	    return;
    	}
    }

    if (!str_prefix(type,L"object"))
    {
    	/* string an obj */

   	if ( ( obj = get_obj_world( ch, arg1 ) ) == NULL )
    	{
	    send_to_char( L"Ne bu dünyada ne de diğerinde böyle bir şey yok.\n\r", ch );
	    return;
    	}

        if ( !str_prefix( arg2, L"name" ) )
    	{
	    free_string( obj->name );
	    obj->name = str_dup( arg3 );
	    return;
    	}

    	if ( !str_prefix( arg2, L"short" ) )
    	{
	    free_string( obj->short_descr );
	    obj->short_descr = str_dup( arg3 );
	    return;
    	}

    	if ( !str_prefix( arg2, L"long" ) )
    	{
	    free_string( obj->description );
	    obj->description = str_dup( arg3 );
	    return;
    	}

    	if ( !str_prefix( arg2, L"ed" ) || !str_prefix( arg2, L"extended"))
    	{
	    EXTRA_DESCR_DATA *ed;

	    argument = one_argument( argument, arg3 );
	    if ( argument == NULL )
	    {
	    	send_to_char( L"Syntax: oset <object> ed <keyword> <string>\n\r",
		    ch );
	    	return;
	    }

 	    wcscat(argument,L"\n\r");

	    ed = new_extra_descr();

	    ed->keyword		= str_dup( arg3     );
	    ed->description	= str_dup( argument );
	    ed->next		= obj->extra_descr;
	    obj->extra_descr	= ed;
	    return;
    	}
    }


    /* echo bad use message */
    do_string(ch,(wchar_t*)"");
}



void do_oset( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg1 [MAX_INPUT_LENGTH];
    wchar_t arg2 [MAX_INPUT_LENGTH];
    wchar_t arg3 [MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int value;

    smash_tilde( argument );
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    wcscpy( arg3, argument );

    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
    {
	send_to_char( L"Syntax:\n\r",ch);
	send_to_char( L"  set obj <object> <field> <value>\n\r",ch);
	send_to_char( L"  Field being one of:\n\r",				ch );
	send_to_char( L"    value0 value1 value2 value3 value4 (v1-v4)\n\r",	ch );
	send_to_char( L"    extra wear level weight cost timer\n\r",		ch );
	return;
    }

    if ( ( obj = get_obj_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( L"Ne bu dünyada ne de diğerinde böyle bir şey yok.\n\r", ch );
	return;
    }

    /*
     * Snarf the value (which need not be numeric).
     */
    value = wcstol( arg3, 0, 10 );

    /*
     * Set something.
     */
    if ( !wcscasecmp( arg2, L"value0" ) || !wcscasecmp( arg2, L"v0" ) )
    {
	obj->value[0] = UMIN(50,value);
	return;
    }

    if ( !wcscasecmp( arg2, L"value1" ) || !wcscasecmp( arg2, L"v1" ) )
    {
	obj->value[1] = value;
	return;
    }

    if ( !wcscasecmp( arg2, L"value2" ) || !wcscasecmp( arg2, L"v2" ) )
    {
	obj->value[2] = value;
	return;
    }

    if ( !wcscasecmp( arg2, L"value3" ) || !wcscasecmp( arg2, L"v3" ) )
    {
	obj->value[3] = value;
	return;
    }

    if ( !wcscasecmp( arg2, L"value4" ) || !wcscasecmp( arg2, L"v4" ) )
    {
	obj->value[4] = value;
	return;
    }

    if ( !str_prefix( arg2, L"extra" ) )
    {
	obj->extra_flags = value;
	return;
    }

    if ( !str_prefix( arg2, L"wear" ) )
    {
	obj->wear_flags = value;
	return;
    }

    if ( !str_prefix( arg2, L"level" ) )
    {
	obj->level = value;
	return;
    }

    if ( !str_prefix( arg2, L"weight" ) )
    {
	obj->weight = value;
	return;
    }

    if ( !str_prefix( arg2, L"cost" ) )
    {
	obj->cost = value;
	return;
    }

    if ( !str_prefix( arg2, L"timer" ) )
    {
	obj->timer = value;
	return;
    }

    /*
     * Generate usage message.
     */
    do_oset( ch, (wchar_t*)"" );
    return;
}



void do_rset( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg1 [MAX_INPUT_LENGTH];
    wchar_t arg2 [MAX_INPUT_LENGTH];
    wchar_t arg3 [MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA *location;
    int value;

    smash_tilde( argument );
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    wcscpy( arg3, argument );

    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
    {
	send_to_char( L"Syntax:\n\r",ch);
	send_to_char( L"  set room <location> <field> <value>\n\r",ch);
	send_to_char( L"  Field being one of:\n\r",			ch );
	send_to_char( L"    flags sector\n\r",				ch );
	return;
    }

    if ( ( location = find_location( ch, arg1 ) ) == NULL )
    {
	send_to_char( L"No such location.\n\r", ch );
	return;
    }

/*    if (!is_room_owner(ch,location) && ch->in_room != location  */
    if ( ch->in_room != location
    &&  room_is_private(location) && !IS_TRUSTED(ch,IMPLEMENTOR))
    {
        send_to_char( L"That room is private right now.\n\r",ch);
        return;
    }

    /*
     * Snarf the value.
     */
    if ( !is_number( arg3 ) )
    {
	send_to_char( L"Value must be numeric.\n\r", ch );
	return;
    }
    value = wcstol( arg3, 0, 10 );

    /*
     * Set something.
     */
    if ( !str_prefix( arg2, L"flags" ) )
    {
	location->room_flags	= value;
	return;
    }

    if ( !str_prefix( arg2, L"sector" ) )
    {
	location->sector_type	= value;
	return;
    }

    /*
     * Generate usage message.
     */
    do_rset( ch, (wchar_t*)"" );
    return;
}



void do_sockets( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t buf[2 * MAX_STRING_LENGTH];
    wchar_t buf2[MAX_STRING_LENGTH];
    wchar_t arg[MAX_INPUT_LENGTH];
    DESCRIPTOR_DATA *d;
    int count;

    count	= 0;
    buf[0]	= '\0';

    one_argument(argument,arg);
    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d->character != NULL && can_see( ch, d->character )
	&& (arg[0] == '\0' || is_name(arg,d->character->name)
			   || (d->original && is_name(arg,d->original->name))))
	{
	    count++;
	    swprintf( buf, MAX_STRING_LENGTH-1, L"[%3d %2d] %s@%s\n\r",
		d->descriptor,
		d->connected,
		d->original  ? d->original->name  :
		d->character ? d->character->name : L"(none)",
		d->host
		);
	}
    }
    if (count == 0)
    {
	send_to_char( L"No one by that name is connected.\n\r",ch);
	return;
    }

    swprintf( buf2, MAX_STRING_LENGTH-1, L"%d user%s\n\r", count, count == 1 ? L"" : L"s" );
    wcscat(buf,buf2);
    page_to_char( buf, ch );
    return;
}



/*
 * Thanks to Grodyn for pointing out bugs in this function.
 */
void do_force( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t buf[MAX_STRING_LENGTH];
    wchar_t arg[MAX_INPUT_LENGTH];
    wchar_t arg2[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
	send_to_char( L"Force whom to do what?\n\r", ch );
	return;
    }

    one_argument(argument,arg2);

    if (!wcscasecmp(arg2, L"delete"))
    {
	send_to_char( L"That will NOT be done.\n\r",ch);
	return;
    }

    swprintf( buf, MAX_STRING_LENGTH-1, L"$n forces you to '%s'.", argument );

    if ( !wcscasecmp( arg, L"all" ) )
    {
	CHAR_DATA *vch;
	CHAR_DATA *vch_next;

	if (get_trust(ch) < MAX_LEVEL - 3)
	{
	    send_to_char( L"Not at your level!\n\r",ch);
	    return;
	}

	for ( vch = char_list; vch != NULL; vch = vch_next )
	{
	    vch_next = vch->next;

	    if ( !IS_NPC(vch) && get_trust( vch ) < get_trust( ch ) )
	    {
		act( buf, ch, NULL, vch, TO_VICT );
		interpret( vch, argument, TRUE );
	    }
	}
    }
    else if (!wcscasecmp(arg, L"players"))
    {
        CHAR_DATA *vch;
        CHAR_DATA *vch_next;

        if (get_trust(ch) < MAX_LEVEL - 2)
        {
            send_to_char( L"Not at your level!\n\r",ch);
            return;
        }

        for ( vch = char_list; vch != NULL; vch = vch_next )
        {
            vch_next = vch->next;

            if ( !IS_NPC(vch) && get_trust( vch ) < get_trust( ch )
	    &&	 vch->level < LEVEL_HERO)
            {
                act( buf, ch, NULL, vch, TO_VICT );
                interpret( vch, argument, FALSE );
            }
        }
    }
    else if (!wcscasecmp(arg, L"gods"))
    {
        CHAR_DATA *vch;
        CHAR_DATA *vch_next;

        if (get_trust(ch) < MAX_LEVEL - 2)
        {
            send_to_char( L"Not at your level!\n\r",ch);
            return;
        }

        for ( vch = char_list; vch != NULL; vch = vch_next )
        {
            vch_next = vch->next;

            if ( !IS_NPC(vch) && get_trust( vch ) < get_trust( ch )
            &&   vch->level >= LEVEL_HERO)
            {
                act( buf, ch, NULL, vch, TO_VICT );
                interpret( vch, argument, FALSE );
            }
        }
    }
    else
    {
	CHAR_DATA *victim;

	if ( ( victim = get_char_world( ch, arg ) ) == NULL )
	{
	    send_to_char( L"They aren't here.\n\r", ch );
	    return;
	}

	if ( victim == ch )
	{
	    send_to_char( L"Aye aye, right away!\n\r", ch );
	    return;
	}

    	if (!is_room_owner(ch,victim->in_room)
	&&  ch->in_room != victim->in_room
        &&  room_is_private(victim->in_room) && !IS_TRUSTED(ch,IMPLEMENTOR))
    	{
            send_to_char( L"That character is in a private room.\n\r",ch);
            return;
        }

	if ( get_trust( victim ) >= get_trust( ch ) )
	{
	    send_to_char( L"Do it yourself!\n\r", ch );
	    return;
	}

	if ( !IS_NPC(victim) && get_trust(ch) < MAX_LEVEL -3)
	{
	    send_to_char( L"Not at your level!\n\r",ch);
	    return;
	}

	act( buf, ch, NULL, victim, TO_VICT );
	interpret( victim, argument, FALSE );
    }

    send_to_char( L"Ok.\n\r", ch );
    return;
}



/*
 * New routines by Dionysos.
 */
void do_invis( CHAR_DATA *ch, wchar_t *argument )
{
    int level;
    wchar_t arg[MAX_STRING_LENGTH];

    /* RT code for taking a level argument */
    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    /* take the default path */

      if ( ch->invis_level)
      {
	  ch->invis_level = 0;
	  act( L"$n slowly fades into existence.", ch, NULL, NULL, TO_ROOM );
	  send_to_char( L"You slowly fade back into existence.\n\r", ch );
      }
      else
      {
	  ch->invis_level = get_trust(ch);
	  act( L"$n slowly fades into thin air.", ch, NULL, NULL, TO_ROOM );
	  send_to_char( L"You slowly vanish into thin air.\n\r", ch );
      }
    else
    /* do the level thing */
    {
      level = wcstol(arg, 0, 10 );
      if (level < 2 || level > get_trust(ch))
      {
	send_to_char( L"Invis level must be between 2 and your level.\n\r",ch);
        return;
      }
      else
      {
	  ch->reply = NULL;
          ch->invis_level = level;
          act( L"$n slowly fades into thin air.", ch, NULL, NULL, TO_ROOM );
          send_to_char( L"You slowly vanish into thin air.\n\r", ch );
      }
    }

    return;
}


void do_incognito( CHAR_DATA *ch, wchar_t *argument )
{
    int level;
    wchar_t arg[MAX_STRING_LENGTH];

    /* RT code for taking a level argument */
    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    /* take the default path */

      if ( ch->incog_level)
      {
          ch->incog_level = 0;
          act( L"$n is no longer cloaked.", ch, NULL, NULL, TO_ROOM );
          send_to_char( L"You are no longer cloaked.\n\r", ch );
      }
      else
      {
          ch->incog_level = get_trust(ch);
          act( L"$n cloaks $s presence.", ch, NULL, NULL, TO_ROOM );
          send_to_char( L"You cloak your presence.\n\r", ch );
      }
    else
    /* do the level thing */
    {
      level = wcstol(arg, 0, 10 );
      if (level < 2 || level > get_trust(ch))
      {
        send_to_char( L"Incog level must be between 2 and your level.\n\r",ch);
        return;
      }
      else
      {
          ch->reply = NULL;
          ch->incog_level = level;
          act( L"$n cloaks $s presence.", ch, NULL, NULL, TO_ROOM );
          send_to_char( L"You cloak your presence.\n\r", ch );
      }
    }

    return;
}



void do_holylight( CHAR_DATA *ch, wchar_t *argument )
{
    if ( IS_NPC(ch) )
	return;

    if ( IS_SET(ch->act, PLR_HOLYLIGHT) )
    {
	REMOVE_BIT(ch->act, PLR_HOLYLIGHT);
	send_to_char( L"Holy light mode off.\n\r", ch );
    }
    else
    {
	SET_BIT(ch->act, PLR_HOLYLIGHT);
	send_to_char( L"Holy light mode on.\n\r", ch );
    }

    return;
}

/* prefix command: it will put the string typed on each line typed */

void do_prefi (CHAR_DATA *ch, wchar_t *argument)
{
    send_to_char( L"You cannot abbreviate the prefix command.\r\n",ch);
    return;
}

void do_prefix (CHAR_DATA *ch, wchar_t *argument)
{
    wchar_t buf[MAX_INPUT_LENGTH];

    if (argument[0] == '\0')
    {
	if (ch->prefix[0] == '\0')
	{
	    send_to_char( L"You have no prefix to clear.\r\n",ch);
	    return;
	}

	send_to_char( L"Prefix removed.\r\n",ch);
	free_string(ch->prefix);
	ch->prefix = str_dup(L"");
	return;
    }

    if (ch->prefix[0] != '\0')
    {
	swprintf( buf, MAX_STRING_LENGTH-1, L"Prefix changed to %s.\r\n",argument);
	free_string(ch->prefix);
    }
    else
    {
	swprintf( buf, MAX_STRING_LENGTH-1, L"Prefix set to %s.\r\n",argument);
    }

    ch->prefix = str_dup(argument);
}



/* RT nochannels command, for those spammers */
void do_grant( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
        send_to_char( L"Grant whom induct privileges?", ch );
        return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
        send_to_char( L"They aren't here.\n\r", ch );
        return;
    }

    if (IS_SET(victim->act,PLR_CANINDUCT))
    {
      REMOVE_BIT(victim->act, PLR_CANINDUCT);
      send_to_char( L"You have the lost the power to INDUCT.\n\r",victim );
      send_to_char( L"INDUCT powers removed.\n\r", ch );
    }
    else {
      SET_BIT(victim->act, PLR_CANINDUCT);
      send_to_char( L"You have been given the power to INDUCT.\n\r",victim);
      send_to_char( L"INDUCT powers given.\n\r", ch);
    }

    return;
}

void do_cecho( CHAR_DATA *ch, wchar_t *argument )
{
    DESCRIPTOR_DATA *d;
    wchar_t buf[MAX_INPUT_LENGTH];
    wchar_t color[MAX_INPUT_LENGTH];
    bool bFound = FALSE;
    int i;

    if ( argument[0] == '\0' )
    {
	send_to_char( L"Color echo what?\n\r", ch );
	return;
    }

    argument = one_argument(argument, color);

    for (i=0;color_table[i].name != NULL;i++) {
      if (!wcscasecmp(color,color_table[i].name)) {
	bFound = TRUE;
	break;
      }
    }

    if (!bFound) {
      swprintf( buf, MAX_STRING_LENGTH-1, L"Usage: cecho <color> <message>.\n\rChoose from one of these colors:\n\r");

      for (i=0;color_table[i].name != NULL;i++) {
           wcscat(buf,color_table[i].name);
           wcscat(buf,L" ");
      }
      send_to_char(buf, ch);
      return;
    }


    for ( d = descriptor_list; d; d = d->next )
    {
	if ( d->connected == CON_PLAYING )
	{
	    if (get_trust(d->character) >= get_trust(ch))
		send_to_char( L"global> ",d->character);
	    act_color( L"$C$t$c",d->character,argument,NULL,TO_CHAR,POS_RESTING,
		      color_table[i].code);
	}
    }

    return;
}


void do_advance( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg1[MAX_INPUT_LENGTH];
    wchar_t arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int level;
    int iLevel;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' || !is_number( arg2 ) )
    {
	send_to_char( L"Syntax: advance <char> <level>.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    {
	send_to_char( L"That player is not here.\n\r", ch);
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( L"Not on NPC's.\n\r", ch );
	return;
    }

    if ( ( level = wcstol( arg2, 0, 10 ) ) < 1 || level > 100 )
    {
	send_to_char( L"Level must be 1 to 100.\n\r", ch );
	return;
    }

    if ( level > get_trust( ch ) )
    {
	send_to_char( L"Limited to your trust level.\n\r", ch );
	return;
    }


    /* Level counting */
    if (ch->level <= 5 || ch->level > LEVEL_HERO)
    {
      if (5 < level && level <= LEVEL_HERO)
	total_levels += level - 5;
    }
    else
    {
      if (5 < level && level <= LEVEL_HERO)
	total_levels += level - ch->level;
      else total_levels -= (ch->level - 5);
    }

    /*
     * Lower level:
     *   Reset to level 1.
     *   Then raise again.
     *   Currently, an imp can lower another imp.
     *   -- Swiftest
     */
    if ( level <= victim->level )
    {
        int temp_prac;

	send_to_char( L"Lowering a player's level!\n\r", ch );
	send_to_char( L"**** OOOOHHHHHHHHHH  NNNNOOOO ****\n\r", victim );
	temp_prac = victim->practice;
	victim->level    = 1;
	victim->exp      = exp_to_level(victim,victim->pcdata->points);
	victim->max_hit  = 10;
	victim->max_mana = 100;
	victim->max_move = 100;
	victim->practice = 0;
	victim->hit      = victim->max_hit;
	victim->mana     = victim->max_mana;
	victim->move     = victim->max_move;
	advance_level( victim );
	victim->practice = temp_prac;
    }
    else
    {
	send_to_char( L"Raising a player's level!\n\r", ch );
	send_to_char( L"**** OOOOHHHHHHHHHH  YYYYEEEESSS ****\n\r", victim );
    }

    for ( iLevel = victim->level ; iLevel < level; iLevel++ )
    {
	send_to_char( L"You raise a level!!  ", victim );
	victim->exp += exp_to_level(victim,victim->pcdata->points);
	victim->level += 1;
	advance_level( victim );
    }
    victim->trust = 0;
    save_char_obj(victim);
    return;
}

void do_ikikat( CHAR_DATA *ch, wchar_t *argument )
{
  wchar_t arg1 [MAX_INPUT_LENGTH];
  wchar_t arg2 [MAX_INPUT_LENGTH];
  int value;

  smash_tilde( argument );
  argument = one_argument( argument, arg1 );
  argument = one_argument( argument, arg2 );

  if ( arg1[0] == '\0')
  {
    send_to_char( L"Kullanım:\n\r",ch);
    send_to_char( L"  ikikat <tp | gp> <dakika>\n\r",ch);
    send_to_char( L"Dakika olarak 0 veya pozitif bir değer verilmelidir.\n\r",ch);
    return;
  }

  value = is_number( arg2 ) ? wcstol( arg2, 0, 10 ) : 0;

  if (value < 0 )
  {
    send_to_char( L"Kullanım:\n\r",ch);
    send_to_char( L"  ikikat <tp | gp> <dakika>\n\r",ch);
    send_to_char( L"Dakika olarak 0 veya pozitif bir değer verilmelidir.\n\r",ch);
    return;
  }

  if ( !wcscasecmp( arg1, L"tp" ) )
  {
    ikikat_tp = value;
    if (value != 0)
      printf_to_char(ch,L"İki kat TP %d dakikalığına açıldı.", value);
    else
      printf_to_char(ch,L"İki kat TP kapatıldı.");
    return;
  }

  if ( !wcscasecmp( arg1, L"gp" ) )
  {
    ikikat_gp = value;
    if (value != 0)
      printf_to_char(ch,L"İki kat GP %d dakikalığına açıldı.", value);
    else
      printf_to_char(ch,L"İki kat GP kapatıldı.");
    return;
  }

  return;

}

void do_mset( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg1 [MAX_INPUT_LENGTH];
    wchar_t arg2 [MAX_INPUT_LENGTH];
    wchar_t arg3 [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int value,sn;

    smash_tilde( argument );
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    wcscpy( arg3, argument );

    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
    {
	send_to_char( L"Syntax:\n\r",ch);
	send_to_char( L"  set char <name> <field> <value>\n\r",ch);
	send_to_char( L"  Field being one of:\n\r",			ch );
	send_to_char( L"    str int wis dex con cha sex class level rkp\n\r",ch );
	send_to_char( L"    race silver hp mana move practice align\n\r",	ch );
	send_to_char( L"    train thirst drunk full hometown ethos\n\r",	ch );

/*** Added By KIO ***/
	send_to_char( L"    questp questt relig bloodlust desire\n\r",	ch );
/*** Added By KIO ***/
	return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( L"They aren't here.\n\r", ch );
	return;
    }

    /*
     * Snarf the value (which need not be numeric).
     */
    value = is_number( arg3 ) ? wcstol( arg3, 0, 10 ) : -1;

    /*
     * Set something.
     */
    if ( !wcscasecmp( arg2, L"str" ) )
    {
    	if ( value < 3 || value > get_max_train(victim,STAT_STR) )
    	{
        printf_to_char(ch,L"Güç 3 ile %d arasında olabilir.\n\r",get_max_train(victim,STAT_STR));
        return;
    	}

    	victim->perm_stat[STAT_STR] = value;
      printf_to_char(ch,L"Tamam.\n\r");
    	return;
    }

    if ( !wcscasecmp( arg2, L"int" ) )
    {
    	if ( value < 3 || value > get_max_train(victim,STAT_INT) )
    	{
        printf_to_char(ch,L"Zeka 3 ile %d arasında olabilir.\n\r",get_max_train(victim,STAT_INT));
        return;
    	}

    	victim->perm_stat[STAT_INT] = value;
      printf_to_char(ch,L"Tamam.\n\r");
    	return;
    }

    if ( !wcscasecmp( arg2, L"wis" ) )
    {
    	if ( value < 3 || value > get_max_train(victim,STAT_WIS) )
    	{
        printf_to_char(ch,L"Bilgelik 3 ile %d arasında olabilir.\n\r",get_max_train(victim,STAT_WIS));
        return;
    	}

    	victim->perm_stat[STAT_WIS] = value;
      printf_to_char(ch,L"Tamam.\n\r");
    	return;
    }

    if ( !wcscasecmp( arg2, L"dex" ) )
    {
    	if ( value < 3 || value > get_max_train(victim,STAT_DEX) )
    	{
        printf_to_char(ch,L"Çeviklik 3 ile %d arasında olabilir.\n\r",get_max_train(victim,STAT_DEX));
        return;
    	}

    	victim->perm_stat[STAT_DEX] = value;
      printf_to_char(ch,L"Tamam.\n\r");
    	return;
    }

    if ( !wcscasecmp( arg2, L"con" ) )
    {
    	if ( value < 3 || value > get_max_train(victim,STAT_CON) )
    	{
        printf_to_char(ch,L"Bünye 3 ile %d arasında olabilir.\n\r",get_max_train(victim,STAT_CON));
        return;
    	}

    	victim->perm_stat[STAT_CON] = value;
      printf_to_char(ch,L"Tamam.\n\r");
    	return;
    }

    if ( !wcscasecmp( arg2, L"cha" ) )
    {
    	if ( value < 3 || value > get_max_train(victim,STAT_CHA) )
    	{
        printf_to_char(ch,L"Karizma 3 ile %d arasında olabilir.\n\r",get_max_train(victim,STAT_CHA));
        return;
    	}

    	victim->perm_stat[STAT_CHA] = value;
      printf_to_char(ch,L"Tamam.\n\r");
    	return;
    }

/*** Added By KIO  ***/
    if ( !wcscasecmp( arg2, L"questp" ) )
    {
	 if ( value == -1) value = 0;
	 if (!IS_NPC(victim)) victim->pcdata->questpoints = value;
	return;
    }
    if ( !wcscasecmp( arg2, L"questt" ) )
    {
	 if ( value == -1) value = 30;
	 if (!IS_NPC(victim))
    {
      victim->pcdata->nextquest = value;
      if (value == 0)
      {
        victim->pcdata->questgiver = 0;
        victim->pcdata->questmob = 0;
        victim->pcdata->questroom = 0;
      }
    }

	return;
    }
    if ( !wcscasecmp( arg2, L"relig" ) )
    {
	 if ( value == -1) value = 0;
	 victim->religion = value;
	return;
    }
/*** Added By KIO ***/



    if ( !str_prefix( arg2, L"sex" ) )
    {
	if ( value < 0 || value > 2 )
	{
	    send_to_char( L"Sex range is 0 to 2.\n\r", ch );
	    return;
	}
	if (( victim->iclass == 0) || ( victim->iclass == 8) )
	{
	    send_to_char( L"You can't change their sex.\n\r", ch );
	    return;
	}
	victim->sex = value;
	if (!IS_NPC(victim))
	    victim->pcdata->true_sex = value;
	return;
    }

    if ( !str_prefix( arg2, L"class" ) )
    {
	int iclass;

	if (IS_NPC(victim))
	{
	    send_to_char( L"Mobiles have no class.\n\r",ch);
	    return;
	}

	iclass = class_lookup(arg3);
	if ( iclass == -1 )
	{
	    wchar_t buf[MAX_STRING_LENGTH];

        	wcscpy( buf, L"Possible classes are: " );
        	for ( iclass = 0; iclass < MAX_CLASS; iclass++ )
        	{
            	    if ( iclass > 0 )
                    	wcscat(buf, L" " );
            	    wcscat( buf, class_table[iclass].name[1] );
        	}
            wcscat(buf, L".\n\r" );

	    send_to_char(buf,ch);
	    return;
	}

	victim->iclass = iclass;
	victim->exp = victim->level * exp_per_level(victim, 0);
	return;
    }

    if ( !str_prefix( arg2, L"level" ) )
    {
	if ( !IS_NPC(victim) )
	{
	    send_to_char( L"Not on PC's.\n\r", ch );
	    return;
	}

	if ( value < 0 || value > 100 )
	{
	    send_to_char( L"Level range is 0 to 100.\n\r", ch );
	    return;
	}
	victim->level = value;
	return;
    }

    if ( !IS_NPC(victim) && !str_prefix( arg2, L"rkp" ) )
    {
	victim->pcdata->rk_puani += value;
	return;
    }

    if ( !str_prefix( arg2, L"silver" ) )
    {
	victim->silver = value;
	return;
    }

    if ( !str_prefix( arg2, L"hp" ) )
    {
	if ( value < -10 || value > 30000 )
	{
	    send_to_char( L"Hp range is -10 to 30,000 hit points.\n\r", ch );
	    return;
	}
	victim->max_hit = value;
        if (!IS_NPC(victim))
            victim->pcdata->perm_hit = value;
	return;
    }

    if ( !str_prefix( arg2, L"mana" ) )
    {
	if ( value < 0 || value > 60000 )
	{
	    send_to_char( L"Mana range is 0 to 60,000 mana points.\n\r", ch );
	    return;
	}
	victim->max_mana = value;
        if (!IS_NPC(victim))
            victim->pcdata->perm_mana = value;
	return;
    }

    if ( !str_prefix( arg2, L"move" ) )
    {
	if ( value < 0 || value > 60000 )
	{
	    send_to_char( L"Move range is 0 to 60,000 move points.\n\r", ch );
	    return;
	}
	victim->max_move = value;
        if (!IS_NPC(victim))
            victim->pcdata->perm_move = value;
	return;
    }

    if ( !str_prefix( arg2, L"practice" ) )
    {
	if ( value < 0 || value > 250 )
	{
	    send_to_char( L"Practice range is 0 to 250 sessions.\n\r", ch );
	    return;
	}
	victim->practice = value;
	return;
    }

    if ( !str_prefix( arg2, L"train" ))
    {
	if (value < 0 || value > 50 )
	{
	    send_to_char( L"Training session range is 0 to 50 sessions.\n\r",ch);
	    return;
	}
	victim->train = value;
	return;
    }

    if ( !str_prefix( arg2, L"align" ) )
    {
	if ( value < -1000 || value > 1000 )
	{
	    send_to_char( L"Alignment range is -1000 to 1000.\n\r", ch );
	    return;
	}
	victim->alignment = value;
	send_to_char( L"Remember to check their hometown.\n\r", ch);
	return;
    }

    if (!str_prefix(arg2, L"ethos"))
      {
	if (IS_NPC(victim))
	  {
	    send_to_char( L"Mobiles don't have an ethos.\n\r", ch);
	    return;
	  }
	if (value < 0 || value > 3)
	  {
	send_to_char( L"The values are Lawful - 1, Neutral - 2, Chaotic - 3\n\r",
			ch);
	    return;
	  }

	victim->ethos = value;
	return;
      }

    if ( !str_prefix( arg2, L"thirst" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( L"Not on NPC's.\n\r", ch );
	    return;
	}

	if ( value < -1 || value > 100 )
	{
	    send_to_char( L"Thirst range is -1 to 100.\n\r", ch );
	    return;
	}

	victim->pcdata->condition[COND_THIRST] = value;
	return;
    }

    if ( !str_prefix( arg2, L"drunk" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( L"Not on NPC's.\n\r", ch );
	    return;
	}

	if ( value < -1 || value > 100 )
	{
	    send_to_char( L"Drunk range is -1 to 100.\n\r", ch );
	    return;
	}

	victim->pcdata->condition[COND_DRUNK] = value;
	return;
    }

    if ( !str_prefix( arg2, L"full" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( L"Not on NPC's.\n\r", ch );
	    return;
	}

	if ( value < -1 || value > 100 )
	{
	    send_to_char( L"Full range is -1 to 100.\n\r", ch );
	    return;
	}

	victim->pcdata->condition[COND_FULL] = value;
	return;
    }

    if ( !str_prefix( arg2, L"bloodlust" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( L"Not on NPC's.\n\r", ch );
	    return;
	}

	if ( value < -1 || value > 100 )
	{
	    send_to_char( L"Full range is -1 to 100.\n\r", ch );
	    return;
	}

	victim->pcdata->condition[COND_BLOODLUST] = value;
	return;
    }

    if ( !str_prefix( arg2, L"desire" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( L"Not on NPC's.\n\r", ch );
	    return;
	}

	if ( value < -1 || value > 100 )
	{
	    send_to_char( L"Full range is -1 to 100.\n\r", ch );
	    return;
	}

	victim->pcdata->condition[COND_DESIRE] = value;
	return;
    }

    if (!str_prefix( arg2, L"race" ) )
    {
	int race;

	race = race_lookup(arg3);

	if ( race == 0)
	{
	    send_to_char( L"That is not a valid race.\n\r",ch);
	    return;
	}

	if (!IS_NPC(victim) && !race_table[race].pc_race)
	{
	    send_to_char( L"That is not a valid player race.\n\r",ch);
	    return;
	}

	if (!IS_NPC(victim))
	for ( sn = 0; sn < MAX_SKILL; sn++ )
	{
	    if ( ( skill_table[sn].name != NULL )
		&& !RACE_OK(victim,sn)	)
		victim->pcdata->learned[sn]	= 0;

	    if ( ( skill_table[sn].name != NULL )
		&&  (ORG_RACE(victim) == skill_table[sn].race )
		)
		victim->pcdata->learned[sn]	= 70;
	}

	if (ORG_RACE(victim) == RACE(victim)) RACE(victim) = race;
	ORG_RACE(victim) = race;

	victim->exp = victim->level * exp_per_level(victim, 0);
	return;
    }


    /*
     * Generate usage message.
     */
    do_mset( ch, (wchar_t*)"" );
    return;
}

void do_induct( CHAR_DATA *ch, wchar_t *argument)
{
  wchar_t arg1[MAX_INPUT_LENGTH];
  wchar_t *arg2;
  wchar_t buf[MAX_STRING_LENGTH];
  wchar_t *cabal;
  CHAR_DATA *victim;
  int sn;
  int i, prev_cabal=0;

  cabal = NULL;
  argument = one_argument( argument, arg1);
  arg2 = argument;

  if (arg1[0] == '\0'|| *arg2 == '\0')
    {
      send_to_char( L"Usage: induct <player> <cabal>\n\r", ch);
      return;
    }


  if ((victim = get_char_world(ch, arg1)) == NULL)
    {
      send_to_char( L"That player isn't on.\n\r", ch);
      return;
    }

  if (IS_NPC(victim))
    {
      act( L"$N is not smart enough to join a cabal.",ch,NULL,victim,TO_CHAR);
      return;
    }

  if ( CANT_CHANGE_TITLE( victim) )
   {
     act( L"$N has tried to join a cabal, but failed.",ch,NULL,victim,TO_CHAR);
     return;
   }

  if ( (i = cabal_lookup(arg2) ) == -1 )
	{
      send_to_char( L"I've never heard of that cabal.\n\r",ch);
      return;
	}

  if (victim->iclass == CLASS_WARRIOR  && i == CABAL_SHALAFI )
  {
    act( L"Fakat $N pis bir büyücü!",ch,NULL,victim,TO_CHAR);
    return;
  }

  if (i == CABAL_RULER && get_curr_stat(victim,STAT_INT) < 19)
  {
   act( L"$N Tüze için yeterince zeki değil!",ch,NULL,victim,TO_CHAR);
   return;
  }

  if ( IS_TRUSTED(ch,LEVEL_IMMORTAL) ||
	(IS_SET(ch->act,PLR_CANINDUCT) &&
	      ((i==CABAL_NONE && (ch->cabal == victim->cabal))
	       ||
	       (i!=CABAL_NONE && ch->cabal==i && victim->cabal==CABAL_NONE))) )
	    {
	      prev_cabal = victim->cabal;
	      victim->cabal = i;
	      REMOVE_BIT(victim->act,PLR_CANINDUCT);
	      cabal = (wchar_t*)cabal_table[i].long_name;
	    }
  else {
	    send_to_char( L"You do not have that power.\n\r",ch);
	    return;
	}

  /* set cabal skills to 70, remove other cabal skills */
  for (sn = 0; sn < MAX_SKILL; sn++)
    {
      if ( (victim->cabal) && (skill_table[sn].cabal == victim->cabal))
	victim->pcdata->learned[sn] = 70;
      else if ( skill_table[sn].cabal != CABAL_NONE &&
	       victim->cabal != skill_table[sn].cabal )
	victim->pcdata->learned[sn] = 0;
    }


  swprintf( buf, MAX_STRING_LENGTH-1, L"$n has been inducted into %s.", cabal);
  act(buf,victim, NULL, NULL, TO_NOTVICT);
  swprintf( buf, MAX_STRING_LENGTH-1, L"You have been inducted into %s.", cabal);
  act(buf, victim, NULL, NULL, TO_CHAR);
  if (ch->in_room != victim->in_room)
    {
      swprintf( buf, MAX_STRING_LENGTH-1, L"%s has been inducted into %s.\n\r",
	      IS_NPC(victim) ? victim->short_descr : victim->name, cabal);
      send_to_char(buf, ch);
    }
  if (victim->cabal == CABAL_NONE && prev_cabal != CABAL_NONE )
    {
     	wchar_t name[100];

 	switch( prev_cabal )
	{
	 default:
	  return;
	 case CABAL_BATTLE:
	  swprintf(name,100-1, L"The LOVER OF MAGIC.");
	  break;
	 case CABAL_SHALAFI:
	  swprintf(name,100-1, L"The HATER OF MAGIC.");
	  break;
	 case CABAL_KNIGHT:
	  swprintf(name,100-1, L"The UNHONOURABLE FIGHTER.");
	  break;
	 case CABAL_INVADER:
	 case CABAL_CHAOS:
	 case CABAL_LIONS:
	 case CABAL_HUNTER:
	 case CABAL_RULER:
	  swprintf(name,100-1, L"NO MORE CABALS.");
	  break;
	}
	set_title( victim, name );
	SET_BIT(victim->act, PLR_NO_TITLE);
    }
}

void do_desocket(CHAR_DATA *ch, wchar_t *argument)
{
  DESCRIPTOR_DATA *d, *d_next;
  int socket;
  wchar_t arg[MAX_INPUT_LENGTH];

  one_argument(argument, arg);

  if (!is_number(arg))

    {
      send_to_char( L"The argument must be a number.\n\r", ch);
      return;
    }

  if (arg[0] == '\0')
    {
      send_to_char( L"Disconnect which socket?\n\r", ch);
      return;
    }

  else
    {
      socket = wcstol(arg, 0, 10 );
      for ( d = descriptor_list; d != NULL; d = d_next )
	{
	  d_next = d->next;
	  if (d->descriptor == socket)
	    {
	      if (d->character == ch)
		{
		  send_to_char( L"It would be foolish to disconnect yourself.\n\r", ch);
		  return;
		}
	      if (d->connected == CON_PLAYING)
		{
		  send_to_char( L"Why don't you just use disconnect?\n\r", ch);
		  return;
		}
	      write_to_descriptor(d->descriptor,
				  (wchar_t*)"You are being disconnected by an immortal.",
				  0);
	      close_socket(d);
	      send_to_char( L"Done.\n\r", ch);
	      return;
	    }
	}
      send_to_char( L"No such socket is connected.\n\r", ch);
      return;
    }
}

void do_smite(CHAR_DATA *ch, wchar_t *argument)
{
  CHAR_DATA *victim;

  if (argument[0] == '\0')
    {
      send_to_char( L"You are so frustrated you smite yourself!  OWW!\n\r",
		ch);
      return;
    }

  if ((victim = get_char_world(ch, argument)) == NULL)
    {
      send_to_char( L"You'll have to smite them some other day.\n\r", ch);
      return;
    }

  if (IS_NPC(victim))
    {
      send_to_char( L"That poor mob never did anything to you.\n\r", ch);
      return;
    }

  if (victim->trust > ch->trust)
    {
      send_to_char( L"How dare you!\n\r", ch);
      return;
    }

  if (victim->position < POS_SLEEPING)
    {
      send_to_char( L"Take pity on the poor thing.\n\r", ch);
      return;
    }

  act( L"A bolt comes down out of the heavens and smites you!", victim, NULL,
	ch, TO_CHAR);
  act( L"You reach down and smite $n!", victim, NULL, ch, TO_VICT);
  act( L"A bolt from the heavens smites $n!", victim, NULL, ch, TO_NOTVICT);
  victim->hit = victim->hit / 2;
  return;
}

void do_popularity( CHAR_DATA *ch, wchar_t *argument )
{
wchar_t buf[4 * MAX_STRING_LENGTH];
wchar_t buf2[MAX_STRING_LENGTH];
AREA_DATA *area;
extern AREA_DATA *area_first;
int i;

    swprintf( buf, MAX_STRING_LENGTH-1, L"Area popularity statistics (in wchar_t * ticks)\n\r" );

    for (area = area_first,i=0; area != NULL; area = area->next,i++) {
      if (area->count >= 5000000)
        swprintf(buf2, MAX_STRING_LENGTH-1, L"%-20s overflow       ",area->name);
      else
        swprintf(buf2, MAX_STRING_LENGTH-1, L"%-20s %-8lu       ",area->name,area->count);
      if ( i % 2 == 0)
	wcscat(buf, L"\n\r" );
      wcscat( buf, buf2 );
    }
    wcscat(buf, L"\n\r\n\r");
    page_to_char( buf, ch );
    return;
}

void do_ititle( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' )  {
	send_to_char( L"Change whose title to what?\n\r", ch );
	return;
    }

    victim = get_char_world( ch, arg );
    if (victim == NULL)  {
	send_to_char( L"Nobody is playing with that name.\n\r", ch );
	return;
    }

    if ( IS_NPC(ch) )
	return;

    if ( argument[0] == '\0' )
    {
	send_to_char( L"Change the title to what?\n\r", ch );
	return;
    }

    if ( wcslen(argument) > 45 )
	argument[45] = '\0';

    smash_tilde( argument );
    set_title( victim, argument );
    send_to_char( L"Ok.\n\r", ch );
}

/*
 * .gz files are checked for too, just in case.
 */

bool check_parse_name (wchar_t* name);

void do_rename (CHAR_DATA* ch, wchar_t* argument)
{
	wchar_t old_name[MAX_INPUT_LENGTH], new_name[MAX_INPUT_LENGTH];
	char strsave [MAX_INPUT_LENGTH];
  OBJ_DATA *obj=NULL, *obj_next;
	CHAR_DATA* victim;
	FILE* file;

	argument = one_argument(argument, old_name);
	one_argument (argument, new_name);

	if (!old_name[0])
	{
		send_to_char ( L"Rename who?\n\r",ch);
		return;
	}

	victim = get_char_world (ch, old_name);

	if (!victim)
	{
		send_to_char ( L"There is no such a person online.\n\r",ch);
		return;
	}

	if (IS_NPC(victim))
	{
		send_to_char ( L"You cannot use Rename on NPCs.\n\r",ch);
		return;
	}

	if ( (victim != ch) && (get_trust (victim) >= get_trust (ch)) )
	{
		send_to_char ( L"You failed.\n\r",ch);
		return;
	}

	if (!victim->desc || (victim->desc->connected != CON_PLAYING) )
	{
		send_to_char ( L"This player has lost his link or is inside a pager or the like.\n\r",ch);
		return;
	}

	if (!new_name[0])
	{
		send_to_char ( L"Rename to what new name?\n\r",ch);
		return;
	}

/*
	if (victim->cabal)
	{
	 send_to_char ( L"This player is member of a cabal, remove him from there first.\n\r",ch);
	 return;
	}
*/

	new_name[0] = towupper(new_name[0]);
	if (!check_parse_name(new_name))
	{
		send_to_char ( L"The new name is illegal.\n\r",ch);
		return;
	}

        sprintf( strsave, "%s%s", PLAYER_DIR, capitalize( new_name ) );

	fclose (fpReserve);
	file = fopen (strsave, "r");
	if (file)
	{
		send_to_char ( L"A player with that name already exists!\n\r",ch);
		fclose (file);
    	fpReserve = fopen( NULL_FILE, "r" );
		return;
	}
   	fpReserve = fopen( NULL_FILE, "r" );

	/* Check .gz file ! */
        sprintf( strsave, "%s%s.gz", PLAYER_DIR, capitalize( new_name ) );

	fclose (fpReserve);
	file = fopen (strsave, "r");
	if (file)
	{
		send_to_char ( L"A player with that name already exists in a compressed file!\n\r",ch);
		fclose (file);
    	fpReserve = fopen( NULL_FILE, "r" );
		return;
	}
   	fpReserve = fopen( NULL_FILE, "r" );

	if (get_char_world(ch,new_name))
	{
		send_to_char ( L"A player with the name you specified already exists!\n\r",ch);
		return;
	}

	sprintf( strsave, "%s%s", PLAYER_DIR, capitalize( victim->name ) );

  for(obj = object_list; obj != NULL; obj = obj_next)
  {
    obj_next = obj->next;
    if ( gorev_ekipmani_mi( obj ) )
    {
      if(wcsstr( obj->short_descr, victim->name))
      {
        extract_obj( obj );
      }
    }
  }

  /*
   * NOTE: Players who are level 1 do NOT get saved under a new name
   */
	free_string (victim->name);
	victim->name = str_dup (capitalize(new_name));

	save_char_obj (victim);

	unlink (strsave);
	send_to_char ( L"Character renamed.\n\r",ch);
	victim->position = POS_STANDING;
	act ( L"$n has renamed you to $N!",ch,NULL,victim,TO_VICT);

}

void do_notitle( CHAR_DATA *ch, wchar_t *argument )
{
  wchar_t arg[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;

    if ( !IS_IMMORTAL(ch) )
        return;
    argument = one_argument(argument,arg);

    if ( arg[0] == '\0' )
    {
	send_to_char( L"Usage:\n\r  notitle <player>\n\r", ch);
	return;
    }

    if ( (victim = get_char_world(ch ,arg)) == NULL )
    {
        send_to_char( L"He is not currently playing.\n\r", ch );
        return;
    }

   if (IS_SET(victim->act, PLR_NO_TITLE) )
	{
   REMOVE_BIT(victim->act,PLR_NO_TITLE);
   send_to_char( L"You can change your title again.\n\r",victim);
   send_to_char( L"Ok.\n\r", ch );
	}
   else
	{
   SET_BIT(victim->act,PLR_NO_TITLE);
   send_to_char( L"You won't be able to change your title anymore.\n\r",victim);
   send_to_char( L"Ok.\n\r", ch );
	}
   return;
}


void do_noaffect( CHAR_DATA *ch, wchar_t * argument)
{
  AFFECT_DATA *paf,*paf_next;
  wchar_t arg[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;

	 if ( !IS_IMMORTAL(ch) )
        	return;

	 argument = one_argument(argument,arg);


	if ( arg[0] == '\0' )
	{
	 send_to_char( L"Usage:\n\r  noaffect <player>\n\r", ch);
	 return;
	}

        if ( (victim = get_char_world(ch ,arg)) == NULL )
        {
         send_to_char( L"He is not currently playing.\n\r", ch );
         return;
        }


	for ( paf = victim->affected; paf != NULL; paf = paf_next )
	{
	    paf_next	= paf->next;
	    if ( paf->duration >= 0 )
	    {
              if ( paf->type > 0 && skill_table[paf->type].msg_off )
		    {
			send_to_char( skill_table[paf->type].msg_off, victim );
			send_to_char( L"\n\r", victim );
		    }

		affect_remove( victim, paf );
	    }
	}

}

void do_affrooms(CHAR_DATA *ch, wchar_t *argument)
 {
    ROOM_INDEX_DATA *room;
    ROOM_INDEX_DATA *room_next;
    wchar_t buf[MAX_STRING_LENGTH];
    int count=0;

    if ( !top_affected_room)
	{
	 send_to_char( L"No affected room.\n\r",ch);
	}
    for ( room = top_affected_room; room ; room = room_next )
    {
	room_next = room->aff_next;
	count++;
	swprintf( buf, MAX_STRING_LENGTH-1, L"%d) [Vnum : %5d] %s\n\r",
		count, room->vnum , room->name);
	send_to_char(buf,ch);
    }
  return;
 }

void do_find( CHAR_DATA *ch, wchar_t *argument )
{
    ROOM_INDEX_DATA *location;
    wchar_t buf[MAX_STRING_LENGTH];
    wchar_t lbuf[MAX_STRING_LENGTH];
    wchar_t arg[MAX_INPUT_LENGTH];

    argument = one_argument(argument, arg);
    if ( arg[0] == '\0' )
    {
	send_to_char( L"Ok. But what I should find?\n\r", ch );
	return;
    }

    if ( ( location = find_location( ch, arg ) ) == NULL )
    {
	send_to_char( L"No such location.\n\r", ch );
	return;
    }

    swprintf( buf, MAX_STRING_LENGTH-1, L"%s.\n\r",find_way(ch,ch->in_room,location));
    send_to_char(buf,ch);
    swprintf(lbuf, MAX_STRING_LENGTH-1, L"From %d to %d: %s",ch->in_room->vnum,location->vnum,buf);
    log_string(lbuf);
    return;
}


void do_reboot( CHAR_DATA *ch, wchar_t *argument )
{
  wchar_t arg[MAX_INPUT_LENGTH];
  wchar_t buf[MAX_STRING_LENGTH];

  argument = one_argument(argument,arg);

 if (arg[0] == '\0')
    {
      send_to_char( L"Usage: reboot now\n\r",ch);
      send_to_char( L"Usage: reboot <ticks to reboot>\n\r",ch);
      send_to_char( L"Usage: reboot cancel\n\r",ch);
      send_to_char( L"Usage: reboot status\n\r",ch);
      return;
    }

    if (is_name(arg,(wchar_t*)"cancel"))
     {
      if (time_sync)
      {
	send_to_char( L"Time synchronization is activated, you cannot cancel the reboot.\n\r",ch);
	return;
      }
      reboot_counter = -1;
      send_to_char( L"Reboot canceled.\n\r",ch);
      return;
    }

    if (is_name(arg, (wchar_t*)"now"))
     {
      reboot_uzakdiyarlar(TRUE);
      return;
    }

    if (is_name(arg, (wchar_t*)"status"))
    {
      if (time_sync)
      {
	send_to_char( L"Time synchronization is activated.\n\r",ch);
	return;
      }
      if (reboot_counter == -1)
	swprintf( buf, MAX_STRING_LENGTH-1, L"Only time synchronization reboot is activated.\n\r");
      else
	swprintf( buf, MAX_STRING_LENGTH-1, L"Reboot in %i minutes.\n\r",reboot_counter);
      send_to_char(buf,ch);
      return;
    }

    if (is_number(arg))
    {
      if (time_sync)
      {
	send_to_char( L"Time synchronization is activated, you cannot change the reboot.\n\r",ch);
	return;
      }
     reboot_counter = wcstol(arg, 0, 10 );
     printf_to_char(ch,L"{cUzak Diyarlar %i dakika sonra yeniden başlatılacak.{x\n\r",reboot_counter);
     return;
    }

 do_reboot(ch,(wchar_t*)"");
}


void reboot_uzakdiyarlar( bool fmessage )
{
    extern bool merc_down;
    DESCRIPTOR_DATA *d,*d_next;

    swprintf( log_buf, (2*MAX_INPUT_LENGTH)-1, L"Oyun yeniden baslatiliyor.");
    log_string(log_buf);
    merc_down = TRUE;
    for ( d = descriptor_list; d != NULL; d = d_next )
    {
	d_next = d->next;
	if (fmessage)
	   write_to_buffer( d, L"***** UD ŞİMDİ YENİDEN BAŞLATILIYOR! *****\n\r",0);
        if (d->character != NULL)
	{
	   update_total_played(d->character);
	   save_char_obj(d->character);
	}
    	close_socket(d);
    }
    return;
}


void do_premort( CHAR_DATA *ch, wchar_t *argument )
{
  wchar_t arg[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;

    if ( !IS_IMMORTAL(ch) )
        return;
    argument = one_argument(argument,arg);

    if ( arg[0] == '\0' )
    {
	send_to_char( L"Usage:\n\r  premort <player>\n\r", ch);
	return;
    }

    if ( (victim = get_char_world(ch ,arg)) == NULL )
    {
        send_to_char( L"He is not currently playing.\n\r", ch );
        return;
    }

    if ( IS_NPC(victim) || victim->level < LEVEL_HERO )
    {
        send_to_char( L"You cannot give remorting permissions to that!.\n\r", ch);
        return;
    }
   if (IS_SET(victim->act, PLR_CANREMORT) )
        {
   REMOVE_BIT(victim->act,PLR_CANREMORT);
   send_to_char( L"You have lost your remorting permission.\n\r",victim);
   send_to_char( L"Ok.\n\r", ch );
        }
   else
        {
   SET_BIT(victim->act,PLR_CANREMORT);
   send_to_char( L"You are given the permission to remort.\n\r",victim);
   send_to_char( L"Ok.\n\r", ch );
        }
   return;
}

void do_maximum( CHAR_DATA *ch, wchar_t *argument )
{
  wchar_t arg[MAX_INPUT_LENGTH];
  wchar_t buf[MAX_STRING_LENGTH];

  argument = one_argument(argument,arg);

  if (arg[0] == '\0')
    {
      send_to_char( L"Usage: maximum status\n\r",ch);
      send_to_char( L"Usage: maximum reset\n\r",ch);
      send_to_char( L"Usage: maximum newbies <number of newbies>\n\r",ch);
      send_to_char( L"Usage: maximum oldies <number of oldies>\n\r",ch);
      return;
    }

  if (is_name(arg, (wchar_t*)"status"))
    {
      swprintf( buf, MAX_STRING_LENGTH-1, L"Maximum oldies allowed: %d.\n\r", max_oldies);
      send_to_char(buf,ch);
      swprintf( buf, MAX_STRING_LENGTH-1, L"Maximum newbies allowed: %d.\n\r", max_newbies);
      send_to_char(buf,ch);
      swprintf( buf, MAX_STRING_LENGTH-1, L"Current number of players: %d.\n\r", iNumPlayers);
      send_to_char(buf,ch);
      return;
    }

  if (is_name(arg, (wchar_t*)"reset"))
    {
      max_newbies = MAX_NEWBIES;
      max_oldies = MAX_OLDIES;
      swprintf( buf, MAX_STRING_LENGTH-1, L"Maximum newbies and oldies have been reset.\n\r");
      send_to_char(buf,ch);
      do_maximum( ch, (wchar_t*)"status" );
      return;
    }

  if (is_name(arg, (wchar_t*)"newbies"))
    {
      argument = one_argument(argument, arg);
      if ( !is_number(arg) )
	{
	 do_maximum(ch, (wchar_t*)"");
	 return;
	}
      max_newbies = wcstol(arg, 0, 10 );
      if ( max_newbies < 0 )
        swprintf( buf, MAX_STRING_LENGTH-1, L"No newbies are allowed!!!\n\r");
      else
        swprintf( buf, MAX_STRING_LENGTH-1, L"Now maximum newbies allowed: %d.\n\r", max_newbies);
      send_to_char(buf,ch);
      return;
    }

  if (is_name(arg, (wchar_t*)"oldies"))
    {
      argument = one_argument(argument, arg);
      if ( !is_number(arg) )
	{
	 do_maximum(ch, (wchar_t*)"");
	 return;
	}
      max_oldies = wcstol(arg, 0, 10 );
      if ( max_oldies < 0 )
        swprintf( buf, MAX_STRING_LENGTH-1, L"No oldies are allowed!!!\n\r");
      else
        swprintf( buf, MAX_STRING_LENGTH-1, L"Now maximum oldies allowed: %d.\n\r", max_oldies);
      send_to_char(buf,ch);
      return;
    }

 do_maximum( ch, (wchar_t*)"" );
}
