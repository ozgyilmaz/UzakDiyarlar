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
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "recycle.h"

void do_tell_quest( CHAR_DATA *ch, CHAR_DATA *victim, char *argument);
CHAR_DATA *get_quest_world( CHAR_DATA *ch, MOB_INDEX_DATA *victim );
extern	MOB_INDEX_DATA	*mob_index_hash	[MAX_KEY_HASH];

/* Object vnums for object quest 'tokens': object quest. */

#define QUEST_OBJQUEST1 84 
#define QUEST_OBJQUEST2 85 
#define QUEST_OBJQUEST3 86 
#define QUEST_OBJQUEST4	97

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

void do_quest(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *questman;
    OBJ_DATA *obj=NULL, *obj_next;
    OBJ_INDEX_DATA *questinfoobj;
    MOB_INDEX_DATA *questinfo;
    char buf [MAX_STRING_LENGTH];
    char bufvampire[100];
    char bufsamurai[100];
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    int sn,trouble_vnum=0,trouble_n;

    argument = one_argument(argument, arg1);
    argument = one_argument(argument, arg2);

    if (IS_NPC(ch) )  return;

    if (!strcmp(arg1, "info"))
    {
	if (IS_SET(ch->act, PLR_QUESTOR))
	{
	    if (ch->pcdata->questmob == -1 )
	    {
		sprintf(buf, "Your quest is ALMOST complete!\n\rGet back to Questor before your time runs out!\n\r");
		send_to_char(buf, ch);
	    }
	    else if (ch->pcdata->questobj > 0)
	    {
                questinfoobj = get_obj_index(ch->pcdata->questobj);
		if (questinfoobj != NULL)
		{
		    sprintf(buf, "You are on a quest to recover the fabled %s!\n\r",questinfoobj->name);
		    send_to_char(buf, ch);
		}
		else send_to_char("You aren't currently on a quest.\n\r",ch);
		return;
	    }
	    else if (ch->pcdata->questmob > 0)
	    {
                questinfo = get_mob_index(ch->pcdata->questmob);
		if (questinfo != NULL)
		{
	            sprintf(buf, "You are on a quest to slay the dreaded %s!\n\r",questinfo->short_descr);
		    send_to_char(buf, ch);
		}
		else send_to_char("You aren't currently on a quest.\n\r",ch);
		return;
	    }
	}
	else
	    send_to_char("You aren't currently on a quest.\n\r",ch);
	return;
    }
    if (!strcmp(arg1, "points"))
    {
	sprintf(buf, "You have %d quest points.\n\r",ch->pcdata->questpoints);
	send_to_char(buf, ch);
	return;
    }
    else if (!strcmp(arg1, "time"))
    {
	if (!IS_SET(ch->act, PLR_QUESTOR))
	{
	    send_to_char("You aren't currently on a quest.\n\r",ch);
	    if (ch->pcdata->nextquest > 1)
	    {
		sprintf(buf, "There are %d minutes remaining until you can go on another quest.\n\r",ch->pcdata->nextquest);
		send_to_char(buf, ch);
	    }
	    else if (ch->pcdata->nextquest == 1)
	    {
		sprintf(buf, "There is less than a minute remaining until you can go on another quest.\n\r");
		send_to_char(buf, ch);
	    }
	}
        else if (ch->pcdata->countdown > 0)
        {
	    sprintf(buf, "Time left for current quest: %d\n\r",ch->pcdata->countdown);
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
        if (questman->spec_fun == spec_lookup( (char*)"spec_questmaster" )) break;
    }

    if (questman == NULL || questman->spec_fun != spec_lookup( (char*)"spec_questmaster" ))
    {
        send_to_char("You can't do that here.\n\r",ch);
        return;
    }

    if ( questman->fighting != NULL)
    {
	send_to_char("Wait until the fighting stops.\n\r",ch);
        return;
    }

    ch->pcdata->questgiver = questman->pIndexData->vnum;

/* And, of course, you will need to change the following lines for YOUR
   quest item information. Quest items on Moongate are unbalanced, very
   very nice items, and no one has one yet, because it takes awhile to
   build up quest points :> Make the item worth their while. */

    if (!strcmp(arg1, "list"))
    {
        act( "$n asks $N for a list of quest items.", ch, NULL, questman, TO_ROOM); 
	act ("You ask $N for a list of quest items.",ch, NULL, questman, TO_CHAR);
/*
1000qp.........The COMFY CHAIR!!!!!!\n\r\
850qp..........Sword of Vassago\n\r\
750qp..........Amulet of Vassago\n\r\
750qp..........Shield of Vassago\n\r\
550qp..........Decanter of Endless Water\n\r\
*/
	bufvampire[0] = '\0';
	if ( ch->iclass == CLASS_VAMPIRE )
	sprintf(bufvampire,"    50qp.........Vampire skill (vampire)\n\r");
	bufsamurai[0] = '\0';
	if ( ch->iclass == 9 )
	sprintf(bufsamurai,"%s%s%s",
		"   100qp.........Katana quest (katana)\n\r",
		"   100qp.........Second katana quest(sharp)\n\r",
		"    50qp.........Decrease number of death (death)\n\r");
	sprintf(buf, "Current Quest Items available for Purchase:\n\r \
  5000qp.........the silk-adamantite backpack (backpack)\n\r \
  1000qp.........the Girth of Real Heroism (girth)\n\r \
  1000qp.........the Ring of Real Heroism (ring)\n\r\
  1000qp.........the Real Hero's Weapon (weapon)\n\r\
  1000qp.........100 Practices (practice)\n\r\
   500qp.........Decanter of Endless Water (decanter)\n\r\
   500qp.........350,000 gold pieces (gold)\n\r\
   250qp.........1 constitution (con)\n\r\
   200qp.........tattoo of your religion (tattoo)\n\r%s%s\
    50qp.........remove tattoo of your religion (remove)\n\r\
    50qp.........set religion to none (set)\n\r\
To buy an item, type 'QUEST BUY <item>'.\n\r",
	bufsamurai, bufvampire);
	send_to_char(buf, ch);
	return;
    }

    else if (!strcmp(arg1, "buy"))
    {
	if (arg2[0] == '\0')
	{
	    send_to_char("To buy an item, type 'QUEST BUY <item>'.\n\r",ch);
	    return;
	}

	else if (is_name(arg2, (char*)"backpack"))
	{
	    if (ch->pcdata->questpoints >= 5000)
	    {
		ch->pcdata->questpoints -= 5000;
	        obj = create_object(get_obj_index(QUEST_ITEM4),ch->level);
	 	if (IS_SET(ch->quest,QUEST_BACKPACK) ||
		    IS_SET(ch->quest,QUEST_BACKPACK2) ||
		    IS_SET(ch->quest,QUEST_BACKPACK3) ) 
		do_tell_quest(ch,questman,
		(char*)"This quest item is beyond the trouble option.");
		else SET_BIT(ch->quest,QUEST_BACKPACK);
	    }
	    else
	    {
		sprintf(buf, "Sorry, %s, but you don't have enough quest points for that.",ch->name);
		do_tell_quest(ch,questman,buf);
		return;
	    }
	}

	else if (is_name(arg2, (char*)"decanter"))
	{
	    if (ch->pcdata->questpoints >= 500)
	    {
		ch->pcdata->questpoints -= 500;
	        obj = create_object(get_obj_index(QUEST_ITEM5),ch->level);
	 	if (IS_SET(ch->quest,QUEST_DECANTER) ||
		    IS_SET(ch->quest,QUEST_DECANTER2) ||
		    IS_SET(ch->quest,QUEST_DECANTER3) ) 
		do_tell_quest(ch,questman,
		(char*)"This quest item is beyond the trouble option.");
		else SET_BIT(ch->quest,QUEST_DECANTER);
	    }
	    else
	    {
		sprintf(buf, "Sorry, %s, but you don't have enough quest points for that.",ch->name);
		do_tell_quest(ch,questman,buf);
		return;
	    }
	}

	else if (is_name(arg2, (char*)"girth"))
	{
	    if (ch->pcdata->questpoints >= 1000)
	    {
		ch->pcdata->questpoints -= 1000;
	        obj = create_object(get_obj_index(QUEST_ITEM1),ch->level);
	 	if (IS_SET(ch->quest,QUEST_GIRTH) ||
		    IS_SET(ch->quest,QUEST_GIRTH2) ||
		    IS_SET(ch->quest,QUEST_GIRTH3) ) 
		do_tell_quest(ch,questman,
		(char*)"This quest item is beyond the trouble option.");
		else SET_BIT(ch->quest,QUEST_GIRTH);
	    }
	    else
	    {
		sprintf(buf, "Sorry, %s, but you don't have enough quest points for that.",ch->name);
		do_tell_quest(ch,questman,buf);
		return;
	    }
	}
	else if (is_name(arg2, (char*)"ring"))
	{
	    if (ch->pcdata->questpoints >= 1000)
	    {
		ch->pcdata->questpoints -= 1000;
	        obj = create_object(get_obj_index(QUEST_ITEM2),ch->level);
	 	if (IS_SET(ch->quest,QUEST_RING) ||
		    IS_SET(ch->quest,QUEST_RING2) ||
		    IS_SET(ch->quest,QUEST_RING3) ) 
		do_tell_quest(ch,questman,
		(char*)"This quest item is beyond the trouble option.");
		else SET_BIT(ch->quest,QUEST_RING);
	    }
	    else
	    {
		sprintf(buf, "Sorry, %s, but you don't have enough quest points for that.",ch->name);
		do_tell_quest(ch,questman,buf);
		return;
	    }
	}
	else if (is_name(arg2, (char*)"weapon"))
	{
	    if (ch->pcdata->questpoints >= 1000)
	    {
		ch->pcdata->questpoints -= 1000;
	        obj = create_object(get_obj_index(QUEST_ITEM3),ch->level);
	 	if (IS_SET(ch->quest,QUEST_WEAPON) ||
		    IS_SET(ch->quest,QUEST_WEAPON2) ||
		    IS_SET(ch->quest,QUEST_WEAPON3) ) 
		do_tell_quest(ch,questman,
		(char*)"This quest item is beyond the trouble option.");
		else SET_BIT(ch->quest,QUEST_WEAPON);
	    }
	    else
	    {
		sprintf(buf, "Sorry, %s, but you don't have enough quest points for that.",ch->name);
		do_tell_quest(ch,questman,buf);
		return;
	    }
	}
	else if (is_name(arg2, (char*)"practices pracs prac practice"))
	{
	    if (IS_SET(ch->quest, QUEST_PRACTICE))
	    {
		sprintf(buf, "Sorry, %s, but you had already got enough practices!",ch->name);
		do_tell_quest(ch,questman,buf);
		return;
	    }

	    if (ch->pcdata->questpoints >= 1000)
	    {
		ch->pcdata->questpoints -= 1000;
	        ch->practice += 100;
    	        act( "$N gives 100 practices to $n.", ch, NULL, questman, TO_ROOM );
    	        act( "$N gives you 100 practices.",   ch, NULL, questman, TO_CHAR );
		SET_BIT(ch->quest, QUEST_PRACTICE);
	        return;
	    }
	    else
	    {
		sprintf(buf, "Sorry, %s, but you don't have enough quest points for that.",ch->name);
		do_tell_quest(ch,questman,buf);
		return;
	    }
	}
	else if (is_name(arg2, (char*)"vampire"))
	{
	  if (ch->iclass != CLASS_VAMPIRE) 
	  {
	    sprintf(buf, "You cannot gain this skill, %s.",ch->name);
	    do_tell_quest(ch,questman,buf);
	    return;
	  }
	  if (ch->pcdata->questpoints >= 50)
	    {
		ch->pcdata->questpoints -= 50;
		sn = skill_lookup("vampire");
		ch->pcdata->learned[sn] = 100;
    	        act( "$N gives secret of undead to $n.", ch, NULL, questman, TO_ROOM );
  	        act( "$N gives you SECRET of undead.",   ch, NULL, questman, TO_CHAR );
		act_color( "$CLightning flashes in the sky.$c",   ch, NULL, 
			questman, TO_ALL,POS_SLEEPING,CLR_BLUE );
	        return;
	    }
	    else
	    {
		sprintf(buf, "Sorry, %s, but you don't have enough quest points for that.",ch->name);
		do_tell_quest(ch,questman,buf);
		return;
	    }
	}
	else if (is_name(arg2, (char*)"con constitution"))
	{
	    if (ch->perm_stat[STAT_CON] >= get_max_train2(ch, STAT_CON))
	    {
		sprintf(buf, "Sorry, %s, you have already sufficienn constitution.",ch->name);
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
		sprintf(buf, "Sorry, %s, but you don't have enough quest points for that.",ch->name);
		do_tell_quest(ch,questman,buf);
		return;
	    }
	}
	else if (is_name(arg2, (char*)"dead samurai death"))
	{
	    if (ch->iclass != 9)
	    {
		sprintf(buf, "Sorry, %s, but you are not a samurai.",ch->name);
		do_tell_quest(ch,questman,buf);
		return;
	    }

	    if (ch->pcdata->death < 1)
	    {
		sprintf(buf, "Sorry, %s, you haven't god any death yet.",ch->name);
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
		sprintf(buf, "Sorry, %s, but you don't have enough quest points for that.",ch->name);
		do_tell_quest(ch,questman,buf);
		return;
	    }
	}
	else if (is_name(arg2, (char*)"katana sword"))
	{
	  AFFECT_DATA af;
	  OBJ_DATA *katana;
	    if (ch->iclass != 9)
	    {
		sprintf(buf, "Sorry, %s, but you are not a samurai.",ch->name);
		do_tell_quest(ch,questman,buf);
		return;
	    }

	    if ( (katana = get_obj_list(ch,(char*)"katana",ch->carrying)) == NULL)
	    {
		 sprintf(buf, "Sorry, %s, but you don't have your katana with you.",ch->name);
		 do_tell_quest(ch,questman,buf);
	         return;
	    }

	    if (IS_WEAPON_STAT(katana,WEAPON_KATANA))
	    {
		sprintf(buf, "Sorry, %s, but your katana has already passed the first quest.",ch->name);
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
		sprintf(buf, "As you wield it, you will feel that it is power will increase, continuously.");
		do_tell_quest(ch,questman,buf);
	    }
	    else
	    {
		sprintf(buf, "Sorry, %s, but you don't have enough quest points for that.",ch->name);
		do_tell_quest(ch,questman,buf);
		return;
	    }
	}
	else if (is_name(arg2, (char*)"sharp second"))
	{
	  AFFECT_DATA af;
	  OBJ_DATA *katana;

	    if (ch->iclass != 9)
	    {
		sprintf(buf, "Sorry, %s, but you are not a samurai.",ch->name);
		do_tell_quest(ch,questman,buf);
		return;
	    }

	    if ( (katana = get_obj_list(ch,(char*)"katana",ch->carrying)) == NULL)
	    {
		 sprintf(buf, "Sorry, %s, but you don't have your katana with you.",ch->name);
		 do_tell_quest(ch,questman,buf);
	         return;
	    }

	    if (!IS_WEAPON_STAT(katana,WEAPON_KATANA))
	    {
		sprintf(buf, "Sorry, %s, but your katana hasn't passed the first quest.",ch->name);
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
		sprintf(buf, "From now on, your katana will be as sharp as blades of titans.");
		do_tell_quest(ch,questman,buf);
	    }
	    else
	    {
		sprintf(buf, "Sorry, %s, but you don't have enough quest points for that.",ch->name);
		do_tell_quest(ch,questman,buf);
		return;
	    }
	}
	else if (is_name(arg2, (char*)"tattoo religion"))
	{
	 OBJ_DATA *tattoo;
	if (!(ch->religion) )
	{
  send_to_char("You don't have a religion to have a tattoo.\n\r", ch);
	return;
	}
	 tattoo = get_eq_char(ch, WEAR_TATTOO);
   	 if (tattoo != NULL)
    	    {
	send_to_char("But you have already your tattoo!.\n\r",ch);
      	      return;
    	    }

	    if (ch->pcdata->questpoints >= 200)
	    {
	      ch->pcdata->questpoints -= 200;

   tattoo = create_object(get_obj_index(religion_table[ch->religion].vnum),100);

      	      obj_to_char(tattoo,ch);
      	      equip_char(ch, tattoo, WEAR_TATTOO);
    	        act( "$N tattoos $n with $p!.", ch, tattoo, questman, TO_ROOM );
    	        act( "$N tattoos you with $p!.",   ch, tattoo, questman, TO_CHAR );
	        return;
	    }
	    else
	    {
		sprintf(buf, "Sorry, %s, but you don't have enough quest points for that.",ch->name);
		do_tell_quest(ch,questman,buf);
		return;
	    }
	}
	else if (is_name(arg2, (char*)"gold gp"))
	{
	    if (ch->pcdata->questpoints >= 500)
	    {
		ch->pcdata->questpoints -= 500;
	        ch->gold += 350000;
    	        act( "$N gives 350,000 gold pieces to $n.", ch, NULL, questman, TO_ROOM );
    	        act( "$N has 350,000 in gold transfered from $s Swiss account to your balance.",   ch, NULL, questman, TO_CHAR );
	        return;
	    }
	    else
	    {
		sprintf(buf, "Sorry, %s, but you don't have enough quest points for that.",ch->name);
		do_tell_quest(ch,questman,buf);
		return;
	    }
	}
	else if (is_name(arg2, (char*)"remove"))
	{
	 OBJ_DATA *tattoo;

	    if (ch->pcdata->questpoints >= 50)
	    {
		if ( (tattoo = get_eq_char(ch, WEAR_TATTOO)) == NULL)
		{
		 sprintf(buf, "But you don't have any tattoo!");
		 do_tell_quest(ch,questman,buf);
		 return;
		}
		
		ch->pcdata->questpoints -= 50;
		extract_obj(tattoo);
		act("Through a painful process, your tattoo has been destroyed by $n.", questman, NULL, ch, TO_VICT);	
		act("$N's tattoo is destroyed by $n.", questman, NULL, ch, TO_NOTVICT);
		return;
	    }
	    else
	    {
		sprintf(buf, "Sorry, %s, but you don't have enough quest points for that.",ch->name);
		do_tell_quest(ch,questman,buf);
		return;
	    }
	}
	else if (is_name(arg2, (char*)"set"))
	{
	    if (ch->pcdata->questpoints >= 50)
	    {
		if ( get_eq_char(ch, WEAR_TATTOO) != NULL)
		{
		 sprintf(buf, "Sorry, %s, but you have to remove your tattoo first!",ch->name);
		 do_tell_quest(ch,questman,buf);
		 return;
		}
		if ( ch->religion == RELIGION_NONE )
		{
		 sprintf(buf, "But you are already an atheist!");
		 do_tell_quest(ch,questman,buf);
		 return;
		}
		ch->pcdata->questpoints -= 50;
		ch->religion = RELIGION_NONE;
		send_to_char("You don't believe any religion now.\n\r", ch);
		act("$N's religion is set to NONE!.", questman, NULL, ch, TO_NOTVICT);
	    }
	    else
	    {
		sprintf(buf, "Sorry, %s, but you don't have enough quest points for that.",ch->name);
		do_tell_quest(ch,questman,buf);
		return;
	    }
	}
	else
	{
	    sprintf(buf, "I don't have that item, %s.",ch->name);
	    do_tell_quest(ch,questman,buf);
	}
	if (obj != NULL)
	{
	 if ( obj->pIndexData->vnum == QUEST_ITEM4
	      || obj->pIndexData->vnum == QUEST_ITEM5)
	 {
	  sprintf( buf,obj->pIndexData->extra_descr->description,ch->name );
	  obj->extra_descr = new_extra_descr();
	  obj->extra_descr->keyword =
		str_dup( obj->pIndexData->extra_descr->keyword );
	  obj->extra_descr->description = str_dup( buf );
	  obj->extra_descr->next = NULL;
	 }
	 if ( obj->pIndexData->vnum == QUEST_ITEM1
	      || obj->pIndexData->vnum == QUEST_ITEM2
	      || obj->pIndexData->vnum == QUEST_ITEM3)
	 {
	  sprintf( buf, obj->short_descr, 
		IS_GOOD(ch) ? "holy" : 
		IS_NEUTRAL(ch) ? "blue-green" : "evil", ch->name);
	  free_string( obj->short_descr );
	  obj->short_descr = str_dup( buf );	  
	 }
    	 act( "$N gives $p to $n.", ch, obj, questman, TO_ROOM );
    	 act( "$N gives you $p.",   ch, obj, questman, TO_CHAR );
	 obj_to_char(obj, ch);
	}
	return;
    }
    else if (!strcmp(arg1, "request"))
    {
        act( "$n asks $N for a quest.", ch, NULL, questman, TO_ROOM); 
	act ("You ask $N for a quest.",ch, NULL, questman, TO_CHAR);
	if (IS_SET(ch->act, PLR_QUESTOR))
	{
	    sprintf(buf, "But you're already on a quest!");
	    do_tell_quest(ch,questman,buf);
	    return;
	}
	if (ch->pcdata->nextquest > 0)
	{
	    sprintf(buf, "You're very brave, %s, but let someone else have a chance.",ch->name);
	    do_tell_quest(ch,questman,buf);
	    sprintf(buf, "Come back later.");
	    do_tell_quest(ch,questman,buf);
	    return;
	}

	sprintf(buf, "Thank you, brave %s!",ch->name);
	do_tell_quest(ch,questman,buf);

	generate_quest(ch, questman);

        if (ch->pcdata->questmob > 0 || ch->pcdata->questobj > 0)
	{
            ch->pcdata->countdown = number_range(15,30);
	    SET_BIT(ch->act, PLR_QUESTOR);
	    sprintf(buf, "You have %d minutes to complete this quest.",ch->pcdata->countdown);
	    do_tell_quest(ch,questman,buf);
	    sprintf(buf, "May the gods go with you!");
	    do_tell_quest(ch,questman,buf);
	}
	return;
    }
    else if (!strcmp(arg1, "complete"))
    {
        act( "$n informs $N $e has completed $s quest.", ch, NULL, questman, TO_ROOM); 
	act ("You inform $N you have completed $s quest.",ch, NULL, questman, TO_CHAR);
	if (ch->pcdata->questgiver != questman->pIndexData->vnum)
	{
	    sprintf(buf, "I never sent you on a quest! Perhaps you're thinking of someone else.");
	    do_tell_quest(ch,questman,buf);
	    return;
	}

	if (IS_SET(ch->act, PLR_QUESTOR))
	{
	    if (ch->pcdata->questmob == -1 && ch->pcdata->countdown > 0)
	    {
		int reward=0, pointreward=0, pracreward=0,level;

		level = ch->level;
		reward = 100 + dice( level, 20);
		reward = UMAX( 180 , reward );
		pointreward = number_range(20,40);

		sprintf(buf, "Congratulations on completing your quest!");
		do_tell_quest(ch,questman,buf);
		sprintf(buf,"As a reward, I am giving you %d quest points, and %d gold.",pointreward,reward);
		do_tell_quest(ch,questman,buf);
		if (chance(2))
		{
		    pracreward = number_range(1,6);
		    sprintf(buf, "You gain %d practices!\n\r",pracreward);
		    send_to_char(buf, ch);
		    ch->practice += pracreward;
		}

	        REMOVE_BIT(ch->act, PLR_QUESTOR);
	        ch->pcdata->questgiver = 0;
	        ch->pcdata->countdown = 0;
	        ch->pcdata->questmob = 0;
		ch->pcdata->questobj = 0;
	        ch->pcdata->nextquest = 5;
		ch->gold += reward;
		ch->pcdata->questpoints += pointreward;

	        return;
	    }
	    else if (ch->pcdata->questobj > 0 && ch->pcdata->countdown > 0)
	    {
		bool obj_found = FALSE;

    		for (obj = ch->carrying; obj != NULL; obj= obj_next)
    		{
        	    obj_next = obj->next_content;
        
	    if (obj != NULL && obj->pIndexData->vnum == ch->pcdata->questobj
		&& strstr( obj->extra_descr->description, ch->name ) != NULL )  		
		    {
			obj_found = TRUE;
            	        break;
		    }
        	}
		if (obj_found == TRUE)
		{
		    int reward, pointreward, pracreward;

		    reward = 200 + number_range(1, 20 * ch->level);
		    pointreward = number_range(15,40);

		    act("You hand $p to $N.",ch, obj, questman, TO_CHAR);
		    act("$n hands $p to $N.",ch, obj, questman, TO_ROOM);

	    	    sprintf(buf, "Congratulations on completing your quest!");
		    do_tell_quest(ch,questman,buf);
		    sprintf(buf,"As a reward, I am giving you %d quest points, and %d gold.",pointreward,reward);
		    do_tell_quest(ch,questman,buf);
		    if (chance(15))
		    {
		        pracreward = number_range(1,6);
		        sprintf(buf, "You gain %d practices!\n\r",pracreward);
		        send_to_char(buf, ch);
		        ch->practice += pracreward;
		    }

	            REMOVE_BIT(ch->act, PLR_QUESTOR);
	            ch->pcdata->questgiver = 0;
	            ch->pcdata->countdown = 0;
	            ch->pcdata->questmob = 0;
		    ch->pcdata->questobj = 0;
	            ch->pcdata->nextquest = 5;
		    ch->gold += reward;
		    ch->pcdata->questpoints += pointreward;
		    extract_obj(obj);
		    return;
		}
		else
		{
		    sprintf(buf, "You haven't completed the quest yet, but there is still time!");
		    do_tell_quest(ch,questman,buf);
		    return;
		}
		return;
	    }
	    else if ((ch->pcdata->questmob > 0 || ch->pcdata->questobj > 0) && ch->pcdata->countdown > 0)
	    {
		sprintf(buf, "You haven't completed the quest yet, but there is still time!");
		do_tell_quest(ch,questman,buf);
		return;
	    }
	}
	if (ch->pcdata->nextquest > 0)
	    sprintf(buf,"But you didn't complete your quest in time!");
	else sprintf(buf, "You have to REQUEST a quest first, %s.",ch->name);
	do_tell_quest(ch,questman,buf);
	return;
    }
    else if (!strcmp(arg1, "trouble"))
    {
	if (arg2[0] == '\0')
	{
	    send_to_char("To correct a quest award's trouble, type: quest trouble <award>'.\n\r",ch);
	    return;
	}

	trouble_n = 0;
	if (is_name(arg2, (char*)"girth"))
	{
	  if (IS_SET(ch->quest,QUEST_GIRTH))
	    {
		REMOVE_BIT(ch->quest,QUEST_GIRTH);
		SET_BIT(ch->quest,QUEST_GIRTH2);
		trouble_n = 1;
	    }
	  else if (IS_SET(ch->quest,QUEST_GIRTH2))
	    {
		REMOVE_BIT(ch->quest,QUEST_GIRTH2);
		SET_BIT(ch->quest,QUEST_GIRTH3);
		trouble_n = 2;
	    }
	  else if (IS_SET(ch->quest,QUEST_GIRTH3))
	    {
		REMOVE_BIT(ch->quest,QUEST_GIRTH3);
		trouble_n = 3;
	    }
	 if (trouble_n) trouble_vnum = QUEST_ITEM1;
	}
	else if (is_name(arg2, (char*)"backpack"))
	{
	  if (IS_SET(ch->quest,QUEST_BACKPACK))
	    {
		REMOVE_BIT(ch->quest,QUEST_BACKPACK);
		SET_BIT(ch->quest,QUEST_BACKPACK2);
		trouble_n = 1;
	    }
	  else if (IS_SET(ch->quest,QUEST_BACKPACK2))
	    {
		REMOVE_BIT(ch->quest,QUEST_BACKPACK2);
		SET_BIT(ch->quest,QUEST_BACKPACK3);
		trouble_n = 2;
	    }
	  else if (IS_SET(ch->quest,QUEST_BACKPACK3))
	    {
		REMOVE_BIT(ch->quest,QUEST_BACKPACK3);
		trouble_n = 3;
	    }
	 if (trouble_n) trouble_vnum = QUEST_ITEM4;
	}
	else if (is_name(arg2, (char*)"decanter"))
	{
	  if (IS_SET(ch->quest,QUEST_DECANTER))
	    {
		REMOVE_BIT(ch->quest,QUEST_DECANTER);
		SET_BIT(ch->quest,QUEST_DECANTER2);
		trouble_n = 1;
	    }
	  else if (IS_SET(ch->quest,QUEST_DECANTER2))
	    {
		REMOVE_BIT(ch->quest,QUEST_DECANTER2);
		SET_BIT(ch->quest,QUEST_DECANTER3);
		trouble_n = 2;
	    }
	  else if (IS_SET(ch->quest,QUEST_DECANTER3))
	    {
		REMOVE_BIT(ch->quest,QUEST_DECANTER3);
		trouble_n = 3;
	    }
	 if (trouble_n) trouble_vnum = QUEST_ITEM5;
	}
	else if (is_name(arg2, (char*)"weapon"))
	{
	  if (IS_SET(ch->quest,QUEST_WEAPON))
	    {
		REMOVE_BIT(ch->quest,QUEST_WEAPON);
		SET_BIT(ch->quest,QUEST_WEAPON2);
		trouble_n = 1;
	    }
	  else if (IS_SET(ch->quest,QUEST_WEAPON2))
	    {
		REMOVE_BIT(ch->quest,QUEST_WEAPON2);
		SET_BIT(ch->quest,QUEST_WEAPON3);
		trouble_n = 2;
	    }
	  else if (IS_SET(ch->quest,QUEST_WEAPON3))
	    {
		REMOVE_BIT(ch->quest,QUEST_WEAPON3);
		trouble_n = 3;
	    }
	 if (trouble_n) trouble_vnum = QUEST_ITEM3;
	}
	else if (is_name(arg2, (char*)"ring"))
	{
	  if (IS_SET(ch->quest,QUEST_RING))
	    {
		REMOVE_BIT(ch->quest,QUEST_RING);
		SET_BIT(ch->quest,QUEST_RING2);
		trouble_n = 1;
	    }
	  else if (IS_SET(ch->quest,QUEST_RING2))
	    {
		REMOVE_BIT(ch->quest,QUEST_RING2);
		SET_BIT(ch->quest,QUEST_RING3);
		trouble_n = 2;
	    }
	  else if (IS_SET(ch->quest,QUEST_RING3))
	    {
		REMOVE_BIT(ch->quest,QUEST_RING3);
		trouble_n = 3;
	    }
	 if (trouble_n) trouble_vnum = QUEST_ITEM2;
	}
	if (!trouble_n)
	{
	    sprintf(buf,"Sorry %s, but you haven't bought that quest award, yet.\n\r",ch->name);
	    do_tell_quest(ch,questman,buf);
	    return;
	}

        for(obj = object_list; obj != NULL; obj = obj_next)
	{ 
	 obj_next = obj->next;
	 if ( obj->pIndexData->vnum == trouble_vnum &&
		strstr( obj->short_descr, ch->name))
	 {
	  extract_obj( obj );
	  break;
	 }
	}
        obj = create_object(get_obj_index(trouble_vnum),ch->level);
	if ( obj->pIndexData->vnum == QUEST_ITEM4
	      || obj->pIndexData->vnum == QUEST_ITEM5)
	 {
	  sprintf( buf,obj->pIndexData->extra_descr->description,ch->name );
	  obj->extra_descr = new_extra_descr();
	  obj->extra_descr->keyword =
		str_dup( obj->pIndexData->extra_descr->keyword );
	  obj->extra_descr->description = str_dup( buf );
	  obj->extra_descr->next = NULL;
	 }
	 if ( obj->pIndexData->vnum == QUEST_ITEM1
	      || obj->pIndexData->vnum == QUEST_ITEM2
	      || obj->pIndexData->vnum == QUEST_ITEM3)
	 {
	  sprintf( buf, obj->short_descr, 
		IS_GOOD(ch) ? "holy" : 
		IS_NEUTRAL(ch) ? "blue-green" : "evil", ch->name);
	  free_string( obj->short_descr );
	  obj->short_descr = str_dup( buf );	  
	 }
    	act( "$N gives $p to $n.", ch, obj, questman, TO_ROOM );
    	act( "$N gives you $p.",   ch, obj, questman, TO_CHAR );
	obj_to_char(obj, ch);
	sprintf(buf,"This is the %i%s time that i am giving that award back.",
	trouble_n,(trouble_n == 1) ? "st" : (trouble_n == 2) ? "nd" : "rd" );
	do_tell_quest(ch,questman,buf);
	if (trouble_n == 3)
	{
	 sprintf(buf,"And I won't give you that again, with trouble option.\n\r");
	 do_tell_quest(ch,questman,buf);
	}
     return;
    }

    send_to_char("QUEST COMMANDS: points info time request complete list buy trouble.\n\r",ch);
    send_to_char("For more information, type: help quests.\n\r",ch);
    return;
}

void generate_quest(CHAR_DATA *ch, CHAR_DATA *questman)
{
    CHAR_DATA *victim;
    MOB_INDEX_DATA *vsearch;
    ROOM_INDEX_DATA *room;
    OBJ_DATA *eyed;
    char buf [MAX_STRING_LENGTH];
    int level_diff, i;
    int mob_buf[300],mob_count;
    int found;

    room	=	(ROOM_INDEX_DATA *)alloc_perm( sizeof (*room));

    mob_count = 0;
    for ( i=0; i< MAX_KEY_HASH; i++)
    {
     if ((vsearch  = mob_index_hash[i]) == NULL) continue;
     level_diff = vsearch->level - ch->level;
     if ( (ch->level < 51 && (level_diff > 4 || level_diff < -1))
	   || (ch->level > 50 && (level_diff > 6 || level_diff < 0))
	   || vsearch->pShop != NULL 
    	   || IS_SET(vsearch->act,ACT_TRAIN)
    	   || IS_SET(vsearch->act,ACT_PRACTICE)
    	   || IS_SET(vsearch->act,ACT_IS_HEALER)
    	   || IS_SET(vsearch->act,ACT_NOTRACK)
	   || IS_SET(vsearch->imm_flags, IMM_SUMMON))
	continue;
     mob_buf[mob_count] = vsearch->vnum;
     mob_count++;
     if (mob_count > 299) break;
    }

    if (chance(40))
    {
       int objvnum = 0;
       int i;

       if (mob_count > 0)
       {
	 found = number_range(0,mob_count-1);
	 for( i=0; i< mob_count; i++)
	 {
	    if ((vsearch = get_mob_index( mob_buf[found] )) == NULL ) 
	    {
		bug("Unknown mob in generate_obj_quest: %d",mob_buf[found]);
		found++;
		if ( found > (mob_count-1) ) break;
		else continue;
	    }
	    else break;
    	 }
       }
       else vsearch = NULL;

       if ( vsearch == NULL || ( victim = get_quest_world( ch, vsearch ) ) == NULL )
       {
	sprintf(buf, "I'm sorry, but I don't have any quests for you at this time.");
	do_tell_quest(ch,questman,buf);
	sprintf(buf, "Try again later.");
	do_tell_quest(ch,questman,buf);
	ch->pcdata->nextquest = 5;
        return;
       }

       if ( (room = victim->in_room) == NULL )
       {
	sprintf(buf, "I'm sorry, but I don't have any quests for you at this time.");
	do_tell_quest(ch,questman,buf);
	sprintf(buf, "Try again later.");
	do_tell_quest(ch,questman,buf);
	ch->pcdata->nextquest = 5;
        return;
       }

	switch(number_range(0,3))
	{
	    case 0:
	    objvnum = QUEST_OBJQUEST1;
	    break;

	    case 1:
	    objvnum = QUEST_OBJQUEST2;
	    break;

	    case 2:
	    objvnum = QUEST_OBJQUEST3;
	    break;

	    case 3:
	    objvnum = QUEST_OBJQUEST4;
	    break;

	}


	if (IS_GOOD(ch))
		i=0;
	else if (IS_EVIL(ch))
		i=2;
	else i = 1;
	
        eyed = create_object( get_obj_index(objvnum), ch->level );
	eyed->owner = str_dup(ch->name);
	eyed->from = str_dup(ch->name);
	eyed->altar = hometown_table[ch->hometown].altar[i];
	eyed->pit = hometown_table[ch->hometown].pit[i];
	eyed->level = ch->level;

	sprintf( buf, eyed->description, ch->name	 );
	free_string( eyed->description );
	eyed->description = str_dup( buf );

        sprintf( buf, eyed->pIndexData->extra_descr->description, ch->name );
	eyed->extra_descr = new_extra_descr();
	eyed->extra_descr->keyword =
		str_dup( eyed->pIndexData->extra_descr->keyword );
	eyed->extra_descr->description = str_dup( buf );
	eyed->extra_descr->next = NULL;

	eyed->cost = 0;
	eyed->timer = 30;

	obj_to_room(eyed, room);
	ch->pcdata->questobj = eyed->pIndexData->vnum;

	sprintf(buf, "Vile pilferers have stolen %s from the royal treasury!",eyed->short_descr);
	do_tell_quest(ch,questman,buf);
	do_tell_quest(ch,questman, (char*)"My court wizardess, with her magic mirror, has pinpointed its location.");

	/* I changed my area names so that they have just the name of the area
	   and none of the level stuff. You may want to comment these next two
	   lines. - Vassago */

	sprintf(buf, "Look in the general area of %s for %s!",room->area->name, room->name);
	do_tell_quest(ch,questman,buf);
	return;
    }

    /* Quest to kill a mob */

    else 
    {
     if (mob_count > 0)
     {
	found = number_range(0,mob_count-1);
	for( i=0; i< mob_count; i++)
	{
	   if ((vsearch = get_mob_index( mob_buf[found] )) == NULL 
	      || (IS_EVIL(vsearch) && IS_EVIL(ch))
	      || (IS_GOOD(vsearch) && IS_GOOD(ch))
	      || (IS_NEUTRAL(vsearch) && IS_NEUTRAL(ch)) )
		{
		 if (vsearch == NULL)
			bug("Unknown mob in mob_quest: %d",mob_buf[found]);
		 found++;
		 if ( found > (mob_count-1) ) 
		 {
		   vsearch = NULL;
		   break;
		 }
		 else continue;
		}
	   else break;
    	}
     }
     else vsearch = NULL;

     if ( vsearch == NULL 
	 || (victim = get_quest_world(ch, vsearch)) == NULL 
	 || (room = victim->in_room ) == NULL
	 || IS_SET(room->area->area_flag,AREA_HOMETOWN))
     {
	sprintf(buf, "I'm sorry, but I don't have any quests for you at this time.");
	do_tell_quest(ch,questman,buf);
	sprintf(buf, "Try again later.");
	do_tell_quest(ch,questman,buf);
	ch->pcdata->nextquest = 5;
        return;
     }

    if (IS_GOOD(ch))
       {	
	sprintf(buf, "Rune's most heinous criminal, %s,	has escaped from the dungeon!",victim->short_descr);
	do_tell_quest(ch,questman,buf);
	sprintf(buf, "Since the escape, %s has murdered %d civillians!",victim->short_descr, number_range(2,20));
	do_tell_quest(ch,questman,buf);
	do_tell_quest(ch,questman,(char*)"The penalty for this crime is death, and you are to deliver the sentence!");
        }
    else
       {
        sprintf(buf, "An enemy of mine, %s, is making vile threats against the crown.",victim->short_descr);
        do_tell_quest(ch,questman,buf);
        sprintf(buf, "This threat must be eliminated!");
        do_tell_quest(ch,questman,buf);
       }

     if (room->name != NULL)
     {
        sprintf(buf, "Seek %s out in vicinity of %s!",victim->short_descr,room->name);
        do_tell_quest(ch,questman,buf);

	/* I changed my area names so that they have just the name of the area
	   and none of the level stuff. You may want to comment these next two
	   lines. - Vassago */

	sprintf(buf, "That location is in the general area of %s.",room->area->name);
	do_tell_quest(ch,questman,buf);
     }
     ch->pcdata->questmob = victim->pIndexData->vnum;
    }
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
	        send_to_char("You may now quest again.\n\r",ch);
	        continue;
	    }
	}
        else if (IS_SET(ch->act,PLR_QUESTOR))
        {
	    if (--ch->pcdata->countdown <= 0)
	    {
    	        char buf [MAX_STRING_LENGTH];

	        ch->pcdata->nextquest = 0;
	        sprintf(buf, "You have run out of time for your quest!\n\rYou may now quest again.\n\r");
	        send_to_char(buf, ch);
	        REMOVE_BIT(ch->act, PLR_QUESTOR);
                ch->pcdata->questgiver = 0;
                ch->pcdata->countdown = 0;
                ch->pcdata->questmob = 0;
		ch->pcdata->questobj = 0;
	    }
	    if (ch->pcdata->countdown > 0 && ch->pcdata->countdown < 6)
	    {
	        send_to_char("Better hurry, you're almost out of time for your quest!\n\r",ch);
	        continue;
	    }
        }
    }
    return;
}

void do_tell_quest( CHAR_DATA *ch, CHAR_DATA *victim, char *argument )
{
char buf[MAX_STRING_LENGTH];

	sprintf(buf,"%s tells you %s\n\r",victim->name,argument);
        send_to_char( buf, ch );
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
