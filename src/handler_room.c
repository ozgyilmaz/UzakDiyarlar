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

 #if defined(macintosh)
 #include <types.h>
 #else
 #include <sys/types.h>
 #endif
 #include <ctype.h>
 #include <stdlib.h>
 #include <stdio.h>
 #include <wchar.h>
 #include <string.h>
 #include <time.h>
 #include "merc.h"
 #include "magic.h"
 #include "recycle.h"
 #include "tables.h"

 /* command procedures needed */
 DECLARE_DO_FUN(do_return	);
 DECLARE_DO_FUN(do_wake		);
 DECLARE_DO_FUN(do_raffects	);
 DECLARE_DO_FUN(do_say		);
 DECLARE_DO_FUN(do_track		);

 /* visibility on a room -- for entering and exits */
 bool can_see_room( CHAR_DATA *ch, ROOM_INDEX_DATA *pRoomIndex )
 {
     if (IS_SET(pRoomIndex->room_flags, ROOM_IMP_ONLY)
     &&  get_trust(ch) < MAX_LEVEL)
 	return FALSE;

     if (IS_SET(pRoomIndex->room_flags, ROOM_GODS_ONLY)
     &&  !IS_IMMORTAL(ch))
 	return FALSE;

     if (IS_SET(pRoomIndex->room_flags, ROOM_HEROES_ONLY)
     &&  !IS_IMMORTAL(ch))
 	return FALSE;

     if (IS_SET(pRoomIndex->room_flags,ROOM_NEWBIES_ONLY)
     &&  ch->level > 5 && !IS_IMMORTAL(ch))
 	return FALSE;

     return TRUE;
 }

 /*
  * True if room is private.
  */
 bool room_is_private( ROOM_INDEX_DATA *pRoomIndex )
 {
     CHAR_DATA *rch;
     int count;

 /*
     if (pRoomIndex->owner != NULL && pRoomIndex->owner[0] != '\0')
 	return TRUE;
 */
     count = 0;
     for ( rch = pRoomIndex->people; rch != NULL; rch = rch->next_in_room )
 	count++;

     if ( IS_SET(pRoomIndex->room_flags, ROOM_PRIVATE)  && count >= 2 )
 	return TRUE;

     if ( IS_SET(pRoomIndex->room_flags, ROOM_SOLITARY) && count >= 1 )
 	return TRUE;

     if ( IS_SET(pRoomIndex->room_flags, ROOM_IMP_ONLY) )
 	return TRUE;

     return FALSE;
 }

 bool is_room_owner(CHAR_DATA *ch, ROOM_INDEX_DATA *room)
 {
     if (room->owner == NULL || room->owner[0] == '\0')
 	return FALSE;

     return is_name(ch->name,room->owner);
 }

 /*
  * Apply or remove an affect to a room.
  */
 void affect_modify_room( ROOM_INDEX_DATA *room, AFFECT_DATA *paf, bool fAdd )
 {
     int mod;

     mod = paf->modifier;

     if ( fAdd )
     {
 	switch (paf->where)
 	{
 	case TO_ROOM_AFFECTS:
 	      SET_BIT(room->affected_by, paf->bitvector);
 	    break;
 	case TO_ROOM_FLAGS:
 	      SET_BIT(room->room_flags, paf->bitvector);
 	    break;
 	case TO_ROOM_CONST:
 	    break;
 	}
     }
     else
     {
         switch (paf->where)
         {
         case TO_ROOM_AFFECTS:
               REMOVE_BIT(room->affected_by, paf->bitvector);
             break;
 	case TO_ROOM_FLAGS:
 	      REMOVE_BIT(room->room_flags, paf->bitvector);
 	    break;
         case TO_ROOM_CONST:
             break;
         }
 	mod = 0 - mod;
     }

     switch ( paf->location )
     {
     default:
 	bug( L"Affect_modify_room: unknown location %d.", paf->location );
 	return;

     case APPLY_ROOM_NONE:					break;
     case APPLY_ROOM_HEAL:	room->heal_rate += mod;		break;
     case APPLY_ROOM_MANA:	room->mana_rate += mod;		break;
     }

     return;
 }

 /*
  * Give an affect to a room.
  */
 void affect_to_room( ROOM_INDEX_DATA *room, AFFECT_DATA *paf )
 {
     AFFECT_DATA *paf_new;
     ROOM_INDEX_DATA *pRoomIndex;

     if ( ! room->affected )
     {
      if ( top_affected_room )
      {
       for ( pRoomIndex  = top_affected_room;
 	  pRoomIndex->aff_next != NULL;
 	  pRoomIndex  = pRoomIndex->aff_next )
 			continue;
       pRoomIndex->aff_next = room;
      }
      else top_affected_room = room;
      room->aff_next = NULL;
     }

     paf_new = new_affect();

     *paf_new		= *paf;
     paf_new->next	= room->affected;
     room->affected	= paf_new;

     affect_modify_room( room , paf_new, TRUE );
     return;
 }

void affect_check_room(ROOM_INDEX_DATA *room,int where,int vector)
 {
     AFFECT_DATA *paf;

     if (vector == 0)
 	return;

     for (paf = room->affected; paf != NULL; paf = paf->next)
 	if (paf->where == where && paf->bitvector == vector)
 	{
 	    switch (where)
 	    {
 	        case TO_ROOM_AFFECTS:
 		      SET_BIT(room->affected_by,vector);
 		    break;
 		case TO_ROOM_FLAGS:
 	      	      SET_BIT(room->room_flags, vector);
 	    	    break;
 	        case TO_ROOM_CONST:
 		    break;
 	    }
 	    return;
 	}
 }

 /*
  * Remove an affect from a room.
  */
 void affect_remove_room( ROOM_INDEX_DATA *room, AFFECT_DATA *paf )
 {
     int where;
     int vector;


     if ( room->affected == NULL )
     {
 	bug( L"Affect_remove_room: no affect.", 0 );
 	return;
     }

     affect_modify_room( room, paf, FALSE );
     where = paf->where;
     vector = paf->bitvector;

     if ( paf == room->affected )
     {
 	room->affected	= paf->next;
     }
     else
     {
 	AFFECT_DATA *prev;

 	for ( prev = room->affected; prev != NULL; prev = prev->next )
 	{
 	    if ( prev->next == paf )
 	    {
 		prev->next = paf->next;
 		break;
 	    }
 	}

 	if ( prev == NULL )
 	{
 	    bug( L"Affect_remove_room: cannot find paf.", 0 );
 	    return;
 	}
     }

     if ( !room->affected )
     {
      ROOM_INDEX_DATA *prev;

      if (top_affected_room  == room)
 	{
 	 top_affected_room = room->aff_next;
 	}
      else
         {
          for(prev = top_affected_room; prev->aff_next; prev = prev->aff_next )
 	  {
 	    if ( prev->aff_next == room )
 	    {
 		prev->aff_next = room->aff_next;
 		break;
 	    }
 	  }
 	 if ( prev == NULL )
 	  {
 	    bug( L"Affect_remove_room: cannot find room.", 0 );
 	    return;
 	  }
         }
       room->aff_next = NULL;

      }

     free_affect(paf);

     affect_check_room(room,where,vector);
     return;
 }

 /*
  * Strip all affects of a given sn.
  */
 void affect_strip_room( ROOM_INDEX_DATA *room, int sn )
 {
     AFFECT_DATA *paf;
     AFFECT_DATA *paf_next;

     for ( paf = room->affected; paf != NULL; paf = paf_next )
     {
 	paf_next = paf->next;
 	if ( paf->type == sn )
 	    affect_remove_room( room, paf );
     }

     return;
 }

 /*
  * Return true if a room is affected by a spell.
  */
 bool is_affected_room( ROOM_INDEX_DATA *room, int sn )
 {
     AFFECT_DATA *paf;

     for ( paf = room->affected; paf != NULL; paf = paf->next )
     {
 	if ( paf->type == sn )
 	    return TRUE;
     }

     return FALSE;
 }



 /*
  * Add or enhance an affect.
  */
 void affect_join_room( ROOM_INDEX_DATA *room, AFFECT_DATA *paf )
 {
     AFFECT_DATA *paf_old;
     int level;

     for ( paf_old = room->affected; paf_old != NULL; paf_old = paf_old->next )
     {
 	if ( paf_old->type == paf->type )
 	{
      level = (paf->level + paf_old->level) / 2;
 	    paf->level = (sh_int) level;
 	    paf->duration += paf_old->duration;
 	    paf->modifier += paf_old->modifier;
 	    affect_remove_room( room, paf_old );
 	    break;
 	}
     }

     affect_to_room( room, paf );
     return;
 }

 /*
  * Return ascii name of an raffect location.
  */
 wchar_t *raffect_loc_name( int location )
 {
     switch ( location )
     {
     case APPLY_ROOM_NONE:	return (wchar_t*)"none";
     case APPLY_ROOM_HEAL:	return (wchar_t*)"heal rate";
     case APPLY_ROOM_MANA:	return (wchar_t*)"mana rate";
     }

     bug( L"Affect_location_name: unknown location %d.", location );
     return (wchar_t*)"(unknown)";
 }


 /*
  * Return ascii name of an affect bit vector.
  */
 wchar_t *raffect_bit_name( int vector )
 {
     static wchar_t buf[512];

     buf[0] = '\0';
     if ( vector & AFF_ROOM_SHOCKING  ) wcscat(buf, L" shocking"        );
     if ( vector & AFF_ROOM_L_SHIELD  ) wcscat(buf, L" lightning_shield");
     if ( vector & AFF_ROOM_THIEF_TRAP) wcscat(buf, L" thief_trap"      );
     if ( vector & AFF_ROOM_CURSE     ) wcscat(buf, L" curse"           );
     if ( vector & AFF_ROOM_POISON    ) wcscat(buf, L" poison"          );
     if ( vector & AFF_ROOM_PLAGUE    ) wcscat(buf, L" plague"          );
     if ( vector & AFF_ROOM_SLEEP     ) wcscat(buf, L" sleep"           );
     if ( vector & AFF_ROOM_SLOW      ) wcscat(buf, L" slow"            );
     return (wchar_t *)(( buf[0] != '\0' ) ? buf+1 : L"none");
 }


 bool is_safe_rspell_nom(int level, CHAR_DATA *victim )
 {
   /* ghosts are safe */
   if ( !IS_NPC(victim) && IS_SET(victim->act, PLR_GHOST))
     return TRUE;

   /* link dead players who do not have rushing adrenalin are safe */
   if (!IS_NPC(victim) && ((victim->last_fight_time == -1) ||
 	((current_time - victim->last_fight_time) > FIGHT_DELAY_TIME)) &&
 	victim->desc == NULL)
     return TRUE;

   if  ( victim->level < 5  && !IS_NPC(victim) )
     return TRUE;

   if ( !IS_NPC(victim) &&
       (victim->last_death_time != -1 && current_time - 	victim->last_death_time < 600))
     return TRUE;


   if ( !IS_NPC(victim) &&
       ((level >= victim->level + 5) || (victim->level >= level + 5)))
     return TRUE;

   return FALSE;
 }


 bool is_safe_rspell(int level, CHAR_DATA *victim)
 {
   if (is_safe_rspell_nom(level,victim))
     {
       act( L"Tanrılar $m koruyor.",victim,NULL,NULL,TO_CHAR);
       act( L"Tanrılar $m odanın büyüsünden koruyor.",victim,NULL,NULL,TO_ROOM);
       return TRUE;
     }
   else return FALSE;
 }


 void raffect_to_char( ROOM_INDEX_DATA *room, CHAR_DATA *ch)
  {
   AFFECT_DATA *paf;

   if ( IS_ROOM_AFFECTED( room, AFF_ROOM_L_SHIELD) )
   {
      int sn;
      CHAR_DATA *vch;

      if ( (sn = skill_lookup( L"lightning shield")) == -1 )
 	{ bug( L"Bad sn for lightning shield",0); return; }

      for (vch=room->people;vch;vch=vch->next_in_room)
 	{
 	 if (is_room_owner(vch,room)) break;
 	}

     if ( !vch )
 	{
 	 bug( L"Owner of lightning shield left the room.",0);
 	 free_string(room->owner);
 	 room->owner = str_dup( L"");
 	 affect_strip_room( room,sn);
 	}
      else
      {
        send_to_char( L"Odadaki koruma kalkanı seni engelliyor.\n\r",ch);
        act( L"$N odaya girdi.",vch,NULL,ch,TO_CHAR);
       do_wake(vch,(wchar_t*)"");

       if ( (paf = affect_find(room->affected,sn)) == NULL )
 	 { bug( L"Bad paf for lightning shield",0); return; }

       if (!is_safe_rspell(paf->level,ch))
 	{

          if (CAN_DETECT(ch, ADET_GROUNDING))
          {
            send_to_char( L"Enerji düşmanlarında yitip gidiyor.\n\r",ch);
            act( L"Bir yıldırım $S düşmanlarında yitip gidiyor.\n\r",ch, NULL, ch, TO_ROOM);
          }
 	 else
 	 {
 	   damage( vch,ch,dice(paf->level,4)+12,sn,DAM_LIGHTNING, TRUE);
 	   free_string(room->owner);
 	   room->owner = str_dup( L"");
 	   affect_remove_room( room , paf);
 	 }
 	}
      }
    }

   if ( IS_ROOM_AFFECTED( room, AFF_ROOM_SHOCKING) )
   {
      int sn;

      if ( (sn = skill_lookup( L"shocking trap")) == -1 )
 	{ bug( L"Bad sn for shocking shield",0); return; }

   send_to_char( L"Odadaki şok dalgaları seni şokluyor.\n\r",ch);

      if ( (paf = affect_find(room->affected,sn)) == NULL )
 	 { bug( L"Bad paf for shocking shield",0); return; }

      if (!is_safe_rspell(paf->level,ch))
 	{
 	 if ( check_immune(ch, DAM_LIGHTNING) != IS_IMMUNE )
 	 damage( ch,ch,dice(paf->level,4)+12,TYPE_HUNGER,DAM_TRAP_ROOM, TRUE);
 	 affect_remove_room( room , paf);
 	}
    }

   if ( IS_ROOM_AFFECTED( room, AFF_ROOM_THIEF_TRAP) )
   {
     send_to_char( L"Birisi tarafından kurulmuş tuzak seni engelliyor.\n\r",ch);

      if ( (paf = affect_find(room->affected,gsn_settraps)) == NULL )
 	 { bug( L"Bad paf for settraps",0); return; }

      if (!is_safe_rspell(paf->level,ch))
 	{
 	 if ( check_immune(ch, DAM_PIERCE) != IS_IMMUNE )
 	 damage( ch,ch,dice(paf->level,5)+12,TYPE_HUNGER,DAM_TRAP_ROOM, TRUE);
 	 affect_remove_room( room , paf);
 	}
    }

   if ( IS_ROOM_AFFECTED( room, AFF_ROOM_SLOW)
 	|| IS_ROOM_AFFECTED( room, AFF_ROOM_SLEEP) )
   send_ch_color(L"$CHavayı dolduran gizemi hissediyorsun.$c\n\r",ch,POS_SLEEPING,CLR_YELLOW);

   return;
  }

void raffect_back_char( ROOM_INDEX_DATA *room, CHAR_DATA *ch)
  {
   if ( IS_ROOM_AFFECTED( room, AFF_ROOM_L_SHIELD) )
   {
    int sn;

     if ( (sn = skill_lookup( L"lightning shield")) == -1 )
 	{ bug( L"Bad sn for lightning shield",0); return; }
     if (is_room_owner(ch,room))
 	{
 	 free_string(room->owner);
 	 room->owner = str_dup( L"");
 	 affect_strip_room( room,sn);
 	}
    }
   return;
  }


 /*
  * Return ascii name of an affect bit vector.
  */
 wchar_t *flag_room_name( int vector )
 {
     static wchar_t buf[512];

     buf[0] = '\0';
     if ( vector & ROOM_DARK         ) wcscat(buf, L" dark"         );
     if ( vector & ROOM_NO_MOB        ) wcscat(buf, L" nomob"        );
     if ( vector & ROOM_INDOORS      ) wcscat(buf, L" indoors"      );
     if ( vector & ROOM_PRIVATE      ) wcscat(buf, L" private"      );
     if ( vector & ROOM_SAFE         ) wcscat(buf, L" safe"         );
     if ( vector & ROOM_SOLITARY     ) wcscat(buf, L" solitary"     );
     if ( vector & ROOM_PET_SHOP     ) wcscat(buf, L" petshop"      );
     if ( vector & ROOM_NO_RECALL    ) wcscat(buf, L" norecall"     );
     if ( vector & ROOM_IMP_ONLY     ) wcscat(buf, L" imp_only"     );
     if ( vector & ROOM_GODS_ONLY    ) wcscat(buf, L" god_only"     );
     if ( vector & ROOM_HEROES_ONLY  ) wcscat(buf, L" heroes"       );
     if ( vector & ROOM_NEWBIES_ONLY ) wcscat(buf, L" newbies"      );
     if ( vector & ROOM_LAW          ) wcscat(buf, L" law"          );
     if ( vector & ROOM_NOWHERE      ) wcscat(buf, L" nowhere"      );
     if ( vector & ROOM_BANK         ) wcscat(buf, L" bank"         );
     if ( vector & ROOM_NO_MAGIC     ) wcscat(buf, L" nomagic"      );
     if ( vector & ROOM_NOSUMMON     ) wcscat(buf, L" nosummon"     );
     if ( vector & ROOM_REGISTRY     ) wcscat(buf, L" registry"     );
     return (wchar_t *)(( buf[0] != '\0' ) ? buf+1 : L"none");
 }

 bool room_dark( ROOM_INDEX_DATA *pRoomIndex )
 {
     if ( pRoomIndex->light > 0 )
 	return FALSE;

     if ( IS_SET(pRoomIndex->room_flags, ROOM_DARK) )
 	return TRUE;

     if ( pRoomIndex->sector_type == SECT_INSIDE
     ||   pRoomIndex->sector_type == SECT_CITY )
 	return FALSE;

     if ( weather_info.sunlight == SUN_SET
     	   || weather_info.sunlight == SUN_DARK )
 	return TRUE;

     return FALSE;
 }

 /*
  * True if room is dark.
  */
 bool room_is_dark( CHAR_DATA *ch )
 {
     ROOM_INDEX_DATA * pRoomIndex = ch->in_room;

     if ( IS_VAMPIRE (ch) )
 	{
          return FALSE;
 	}

     if ( pRoomIndex->light > 0 )
 	return FALSE;

     if ( IS_SET(pRoomIndex->room_flags, ROOM_DARK) )
 	return TRUE;

     if ( pRoomIndex->sector_type == SECT_INSIDE
     ||   pRoomIndex->sector_type == SECT_CITY )
 	return FALSE;

     if ( weather_info.sunlight == SUN_SET
     	   || weather_info.sunlight == SUN_DARK )
 	return TRUE;

     return FALSE;
 }

 /*
  * Find a char in the room.
  */
 CHAR_DATA *get_char_room( CHAR_DATA *ch, wchar_t *argument )
 {
     wchar_t arg[MAX_INPUT_LENGTH];
     CHAR_DATA *rch;
     int number;
     int count;
     int ugly;

     number = number_argument( argument, arg );
     count  = 0;
     ugly   = 0;
     if ( !wcscasecmp( arg, L"self" ) || !wcscasecmp( arg, L"kendimi" ) || !wcscasecmp( arg, L"bana" ) )
 	   return ch;
     if ( !wcscasecmp( arg, L"ugly" ) || !wcscasecmp( arg, L"çirkin" ) )
 	   ugly = 1;

     for ( rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room )
     {
 	if ( !can_see( ch, rch ) )
 	    continue;

 	if ( ugly && (count + 1) == number && IS_VAMPIRE(rch) )
 	   return rch;

         if ( (is_affected(rch,gsn_doppelganger)
               && !IS_SET(ch->act,PLR_HOLYLIGHT))?
             !is_name(arg,rch->doppel->name):!is_name(arg,rch->name) )
           continue;

 	if ( ++count == number )
 	    return rch;
     }

     return NULL;
 }



 /*
  * Find a char in the room.
  * Chronos uses in act_move.c
  */
 CHAR_DATA *get_char_room2( CHAR_DATA *ch, ROOM_INDEX_DATA *room, wchar_t *argument, int *number )
 {
     CHAR_DATA *rch;
     int count;
     int ugly;

     if (room == NULL ) return NULL;
     count  = 0;
     ugly   = 0;

     if ( !wcscasecmp( argument, L"ugly" ) )
 	ugly = 1;

     for ( rch = room->people; rch != NULL; rch = rch->next_in_room )
     {
 	if ( !can_see( ch, rch ) )
 	    continue;

 	if ( ugly && (count + 1) == *number && IS_VAMPIRE(rch) )
 	   return rch;

         if ( (is_affected(rch,gsn_doppelganger)
               && !IS_SET(ch->act,PLR_HOLYLIGHT))?
 	!is_name(argument,rch->doppel->name):!is_name(argument,rch->name) )
           continue;

 	if ( ++count == *number )
 	    return rch;
     }

     *number -= count;
     return NULL;
 }

 /*
  * Move an obj out of a room.
  */
 void obj_from_room( OBJ_DATA *obj )
 {
     ROOM_INDEX_DATA *in_room;
     CHAR_DATA *ch;

     if ( ( in_room = obj->in_room ) == NULL )
     {
 	bug( L"obj_from_room: NULL.", 0 );
 	return;
     }

     for (ch = in_room->people; ch != NULL; ch = ch->next_in_room)
 	if (ch->on == obj)
 	    ch->on = NULL;

     if ( obj == in_room->contents )
     {
 	in_room->contents = obj->next_content;
     }
     else
     {
 	OBJ_DATA *prev;

 	for ( prev = in_room->contents; prev; prev = prev->next_content )
 	{
 	    if ( prev->next_content == obj )
 	    {
 		prev->next_content = obj->next_content;
 		break;
 	    }
 	}

 	if ( prev == NULL )
 	{
 	    bug( L"Obj_from_room: obj not found.", 0 );
 	    return;
 	}
     }

     obj->in_room      = NULL;
     obj->next_content = NULL;
     return;
 }



 /*
  * Move an obj into a room.
  */
 void obj_to_room( OBJ_DATA *obj, ROOM_INDEX_DATA *pRoomIndex )
 {
     int i;

     obj->next_content		= pRoomIndex->contents;
     pRoomIndex->contents	= obj;
     obj->in_room		= pRoomIndex;
     obj->carried_by		= NULL;
     obj->in_obj			= NULL;

     if ( IS_WATER(pRoomIndex) )
     {
 	if ( may_float( obj ) )
 	  obj->water_float = -1;
 	else
 	  obj->water_float = floating_time( obj );
 	  }

     if (obj->pIndexData->vnum < 600)
     {
       for (i=1;i < MAX_CABAL; i++)
 	if (cabal_table[i].obj_vnum == obj->pIndexData->vnum)
 	   break;

       if (i < MAX_CABAL)
       {
         for (i=1;i < MAX_CABAL; i++)
 	  if (cabal_table[i].room_vnum == pRoomIndex->vnum)
 	    break;

         if (i < MAX_CABAL)
         {
 	  obj->timer = -1;
 	  if (pRoomIndex->people)
 	  {
       act( L"$p saydamlığını yitirerek katılaşıyor.",
 		pRoomIndex->people, obj, NULL, TO_CHAR);
 	    act( L"$p saydamlığını yitirerek katılaşıyor.",
 		pRoomIndex->people, obj, NULL, TO_ROOM);
 	  }
         }
       }
     }
     return;
 }

 /*
  * Move a char into a room.
  */
 void char_to_room( CHAR_DATA *ch, ROOM_INDEX_DATA *pRoomIndex )
 {
     OBJ_DATA *obj;

     if ( pRoomIndex == NULL )
     {
 	ROOM_INDEX_DATA *room;

 	bug( L"Char_to_room: NULL.", 0 );

 	if ((room = get_room_index(ROOM_VNUM_TEMPLE)) != NULL)
 	    char_to_room(ch,room);

 	return;
     }

     ch->in_room		= pRoomIndex;
     ch->next_in_room	= pRoomIndex->people;
     pRoomIndex->people	= ch;

     if ( !IS_NPC(ch) )
     {
 	if (ch->in_room->area->empty)
 	{
 	    ch->in_room->area->empty = FALSE;
 	    ch->in_room->area->age = 0;
 	}
 	++ch->in_room->area->nplayer;
     }

     if ( ( obj = get_light_char( ch ) ) != NULL )
 /*
     &&   obj->item_type == ITEM_LIGHT
     &&   obj->value[2] != 0 )
 */
 	++ch->in_room->light;

     while (IS_AFFECTED(ch,AFF_PLAGUE))
     {
         AFFECT_DATA *af, plague;
         CHAR_DATA *vch;

         for ( af = ch->affected; af != NULL; af = af->next )
         {
             if (af->type == gsn_plague)
                 break;
         }

         if (af == NULL)
         {
             REMOVE_BIT(ch->affected_by,AFF_PLAGUE);
             break;
         }

         if (af->level == 1)
             break;

 	plague.where		= TO_AFFECTS;
         plague.type 		= gsn_plague;
         plague.level 		= af->level - 1;
         plague.duration 	= number_range(1,2 * plague.level);
         plague.location		= APPLY_STR;
         plague.modifier 	= -5;
         plague.bitvector 	= AFF_PLAGUE;

         for ( vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
         {
             if (!saves_spell(plague.level - 2,vch,DAM_DISEASE)
 	    &&  !IS_IMMORTAL(vch) &&
             	!IS_AFFECTED(vch,AFF_PLAGUE) && number_range(0,63) == 0)
             {
               send_to_char( L"Ateşinin yükseldiğini hissediyorsun.\n\r",vch);
             	act( L"$n çok hasta görünüyor.",vch,NULL,NULL,TO_ROOM);
             	affect_join(vch,&plague);
             }
         }
 	break;
     }

     if ( ch->in_room->affected_by )
 	{
 	 if (IS_IMMORTAL(ch))
 		do_raffects(ch,(wchar_t*)"");
 	 else raffect_to_char( ch->in_room, ch);
 	}

     return;
 }

 /*
  * Move a char out of a room.
  */
 void char_from_room( CHAR_DATA *ch )
 {
     OBJ_DATA *obj;
     ROOM_INDEX_DATA *prev_room = ch->in_room;

     if ( ch->in_room == NULL )
     {
 	bug( L"Char_from_room: NULL.", 0 );
 	return;
     }

     if ( !IS_NPC(ch) )
 	--ch->in_room->area->nplayer;

     if ( ( obj = get_light_char( ch ) ) != NULL
 /*
     &&   obj->item_type == ITEM_LIGHT
     &&   obj->value[2] != 0
 */
     &&   ch->in_room->light > 0 )
 	--ch->in_room->light;

     if ( ch == ch->in_room->people )
     {
 	ch->in_room->people = ch->next_in_room;
     }
     else
     {
 	CHAR_DATA *prev;

 	for ( prev = ch->in_room->people; prev; prev = prev->next_in_room )
 	{
 	    if ( prev->next_in_room == ch )
 	    {
 		prev->next_in_room = ch->next_in_room;
 		break;
 	    }
 	}

 	if ( prev == NULL )
 	    bug( L"Char_from_room: ch not found.", 0 );
     }

     ch->in_room      = NULL;
     ch->next_in_room = NULL;
     ch->on 	     = NULL;  /* sanity check! */

     if (MOUNTED(ch))
     {
      ch->mount->riding	= FALSE;
      ch->riding		= FALSE;
     }

     if (RIDDEN(ch))
     {
      ch->mount->riding	= FALSE;
      ch->riding		= FALSE;
     }

     if ( prev_room && prev_room->affected_by )
 	  raffect_back_char( prev_room, ch);

     return;
 }

 /*
  * Room record:
  * For less than 5 people in room create a new record.
  * Else use the oldest one.
  */

 void room_record(wchar_t *name,ROOM_INDEX_DATA *room,sh_int door)
 {
   ROOM_HISTORY_DATA *rh;
   int i=0;

   for (rh = room->history; i < 5 && rh != NULL; i++,rh = rh->next);

   if (i < 5)
     rh = (ROOM_HISTORY_DATA *)alloc_perm(sizeof(ROOM_HISTORY_DATA));
   else
   {
     rh = room->history->next->next->next->next;
     room->history->next->next->next->next = NULL;
     free_string(rh->name);
   }

   rh->next = room->history;
   room->history = rh;
   rh->name = str_dup(name);
   rh->went = door;
 }
