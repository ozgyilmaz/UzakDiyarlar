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
 *  This file is a combination of:                                         *
 *		(H)unt.c, (E)nter.c, (R)epair.c and (A)uction.c            *
 *  Thus it is called ACT_HERA.C                                           *
 **************************************************************************/

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
#include <wchar.h>
#include <wctype.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <inttypes.h>
#include "merc.h"

/* command procedures needed */
DECLARE_DO_FUN(do_look		);
DECLARE_DO_FUN(do_stand		);
DECLARE_DO_FUN(do_say		);
DECLARE_DO_FUN(do_cast		);
DECLARE_DO_FUN(do_enter		);
DECLARE_DO_FUN(do_open		);

/* random room generation procedure */
ROOM_INDEX_DATA  *get_random_room(CHAR_DATA *ch)
{
    ROOM_INDEX_DATA *room;

    for ( ; ; )
    {
        room = get_room_index( number_range( 0, 65535 ) );
        if ( room != NULL )
        if ( can_see_room(ch,room)
	&&   !room_is_private(room)
        &&   !IS_SET(room->room_flags, ROOM_PRIVATE)
        &&   !IS_SET(room->room_flags, ROOM_SOLITARY)
	&&   !IS_SET(room->room_flags, ROOM_SAFE)
	&&   (IS_NPC(ch) || IS_SET(ch->act,ACT_AGGRESSIVE)
	||   !IS_SET(room->room_flags,ROOM_LAW)))
            break;
    }

    return room;
}

/* RT Enter portals */
void do_enter( CHAR_DATA *ch, wchar_t *argument)
{
    ROOM_INDEX_DATA *location;

    if ( ch->fighting != NULL )
	return;

    /* nifty portal stuff */
    if (argument[0] != '\0')
    {
        ROOM_INDEX_DATA *old_room;
	OBJ_DATA *portal;
	CHAR_DATA *fch, *fch_next, *mount;
	wchar_t buf[MAX_STRING_LENGTH];

        old_room = ch->in_room;

	portal = get_obj_list( ch, argument,  ch->in_room->contents );

	if (portal == NULL)
	{
    printf_to_char(ch,L"Öyle birşey görmüyorsun.\n\r");
	    return;
	}

	if (portal->item_type != ITEM_PORTAL
        ||  (IS_SET(portal->value[1],EX_CLOSED) && !IS_TRUSTED(ch,ANGEL)))
	{
    printf_to_char(ch,L"Bir yol bulamıyorsun.\n\r");
	    return;
	}

	if (!IS_TRUSTED(ch,ANGEL) && !IS_SET(portal->value[2],GATE_NOCURSE)
	&&  (IS_AFFECTED(ch,AFF_CURSE)
	||   IS_SET(old_room->room_flags,ROOM_NO_RECALL)
	||   IS_RAFFECTED(old_room,AFF_ROOM_CURSE) ))
	{
    printf_to_char(ch,L"Birşey ayrılmanı engelliyor...\n\r");
	    return;
	}

	if (IS_SET(portal->value[2],GATE_RANDOM) || portal->value[3] == -1)
	{
	    location = get_random_room(ch);
	    portal->value[3] = location->vnum; /* keeps record */
	}
	else if (IS_SET(portal->value[2],GATE_BUGGY) && (number_percent() < 5))
	    location = get_random_room(ch);
	else
	    location = get_room_index(portal->value[3]);

	if (location == NULL
	||  location == old_room
	||  !can_see_room(ch,location)
	||  (room_is_private(location) && !IS_TRUSTED(ch,IMPLEMENTOR)))
	{
    act( L"$p hiçbir yere ulaşmıyor.",ch,portal,NULL,TO_CHAR);
	   return;
	}

        if (IS_NPC(ch) && IS_SET(ch->act,ACT_AGGRESSIVE)
        &&  IS_SET(location->room_flags,ROOM_LAW))
        {
          printf_to_char(ch,L"Birşey ayrılmanı engelliyor...\n\r");
            return;
        }

        if (MOUNTED(ch))
        swprintf( buf, MAX_STRING_LENGTH-1, L"$n sürdüğü %s ile $p içine giriyor.",MOUNTED(ch)->short_descr );
        swprintf( buf, MAX_STRING_LENGTH-1, L"$n $p içine giriyor." );
	act(buf,ch,portal,NULL,TO_ROOM);

	if (IS_SET(portal->value[2],GATE_NORMAL_EXIT))
  act( L"$p içine giriyorsun.",ch,portal,NULL,TO_CHAR);
	else
  act( L"$p içine yürüyünce kendini başka yerde buluyorsun...",ch,portal,NULL,TO_CHAR);

	mount = MOUNTED(ch);
	char_from_room(ch);
	char_to_room(ch, location);

	if (IS_SET(portal->value[2],GATE_GOWITH)) /* take the gate along */
	{
	    obj_from_room(portal);
	    obj_to_room(portal,location);
	}

	if (IS_SET(portal->value[2],GATE_NORMAL_EXIT))
	 {
	  if (mount)
    act( L"$n $M sürerek geldi.",ch,portal,mount,TO_ROOM);
    else  act( L"$n geldi.",ch,portal,NULL,TO_ROOM);
	 }
	else
	 {
	  if (mount)
    act( L"$n $p içinden $M sürerek geldi.",ch,portal,mount,TO_ROOM);
  else  act( L"$n $p içinden geldi.",ch,portal,NULL,TO_ROOM);
	 }

	do_look(ch,(wchar_t*)"auto");

        if (mount)
	 {
	  char_from_room( mount );
	  char_to_room( mount, location);
  	  ch->riding = TRUE;
  	  mount->riding = TRUE;
	 }

	/* charges */
	if (portal->value[0] > 0)
	{
	    portal->value[0]--;
	    if (portal->value[0] == 0)
		portal->value[0] = -1;
	}

	/* protect against circular follows */
	if (old_room == location)
	    return;

    	for ( fch = old_room->people; fch != NULL; fch = fch_next )
    	{
            fch_next = fch->next_in_room;

            if (portal == NULL || portal->value[0] == -1)
	    /* no following through dead portals */
                continue;

            if ( fch->master == ch && IS_AFFECTED(fch,AFF_CHARM)
            &&   fch->position < POS_STANDING)
            	do_stand(fch,(wchar_t*)"");

            if ( fch->master == ch && fch->position == POS_STANDING)
            {

                if (IS_SET(ch->in_room->room_flags,ROOM_LAW)
                &&  (IS_NPC(fch) && IS_SET(fch->act,ACT_AGGRESSIVE)))
                {
                  act( L"$M şehre sokamazsın.",
                    ch,NULL,fch,TO_CHAR);
                  act( L"Şehirde istenmiyorsun.",
                    fch,NULL,NULL,TO_CHAR);
                    continue;
            	}

              act( L"$M takip ediyorsun.", fch, NULL, ch, TO_CHAR );
		do_enter(fch,argument);
            }
    	}

 	if (portal != NULL && portal->value[0] == -1)
	{
    act( L"$p kayboluyor.",ch,portal,NULL,TO_CHAR);
    if (ch->in_room == old_room)
  act( L"$p kayboluyor.",ch,portal,NULL,TO_ROOM);
	    else if (old_room->people != NULL)
	    {
        act( L"$p kayboluyor.",
    		    old_room->people,portal,NULL,TO_CHAR);
    		act( L"$p kayboluyor.",
		    old_room->people,portal,NULL,TO_ROOM);
	    }
	    extract_obj(portal);
	}
	return;
    }

    printf_to_char(ch,L"Hayır, bunu yapamazsın.\n\r");
    return;
}

void do_settraps( CHAR_DATA *ch, wchar_t *argument )
{
    if ( !IS_NPC(ch)
    &&   ch->level < skill_table[gsn_settraps].skill_level[ch->iclass] )
      {
        printf_to_char(ch,L"Tuzak kurmayı bilmiyorsun.\n\r");
	return;
      }

    if (!ch->in_room)	return;

    if ( IS_SET(ch->in_room->room_flags, ROOM_LAW) )
	{
    send_to_char( L"Mistik bir güç odayı koruyor.\n\r",ch);
	 return;
	}

    WAIT_STATE( ch, skill_table[gsn_settraps].beats );

    if  (  IS_NPC(ch)
    ||   number_percent( ) <  ( get_skill(ch,gsn_settraps) * 0.7 ) )
    {
      AFFECT_DATA af,af2;

      check_improve(ch,gsn_settraps,TRUE,1);

      if ( is_affected_room( ch->in_room, gsn_settraps ))
      {
        send_to_char( L"Bu odaya zaten tuzak kurulmuş.\n\r",ch);
	return;
       }

      if ( is_affected(ch,gsn_settraps))
      {
        send_to_char( L"Bu yetenek yakın zamanda kullanıldı.\n\r",ch);
	return;
      }

      af.where     = TO_ROOM_AFFECTS;
      af.type      = gsn_settraps;
      af.level     = ch->level;
      af.duration  = ch->level / 40;
      af.location  = APPLY_NONE;
      af.modifier  = 0;
      af.bitvector = AFF_ROOM_THIEF_TRAP;
      affect_to_room( ch->in_room, &af );

      af2.where     = TO_AFFECTS;
      af2.type      = gsn_settraps;
      af2.level	    = ch->level;

      if ( ch->last_fight_time != -1 && !IS_IMMORTAL(ch) &&
        	(current_time - ch->last_fight_time)<FIGHT_DELAY_TIME)
         af2.duration  = 1;
      else af2.duration = ch->level / 10;

      af2.modifier  = 0;
      af2.location  = APPLY_NONE;
      af2.bitvector = 0;
      affect_to_char( ch, &af2 );
      send_to_char( L"Tuzağı kurdun.\n\r", ch );
      act( L"$n odaya tuzak kurdu.",ch,NULL,NULL,TO_ROOM);
      return;
    }
    else check_improve(ch,gsn_settraps,FALSE,1);

   return;
}

/***************************************************************************
 ************************       hunt.c        ******************************
 ***************************************************************************/


/***************************************************************************
 *  Original idea from SillyMUD v1.1b (C)1993.                             *
 *  Modified to merc2.1 by Rip.                                            *
 *  Modified by Turtle for Merc22 (07-Nov-94).                             *
 *  Adopted to ANATOLIA by Chronos.                                        *
 ***************************************************************************/

#if	defined(linux)
void bcopy(const void *src,void *dest,int n);
void bzero(void *s,int n);
#else
void bcopy(wchar_t *s1,char* s2,int len);
void bzero(wchar_t *sp,int len);
#endif

extern const wchar_t* dir_name[];

struct hash_link
{
  int			key;
  struct hash_link	*next;
  void			*data;
};

struct hash_header
{
  int			rec_size;
  int			table_size;
  int			*keylist, klistsize, klistlen; /* this is really lame,
							  AMAZINGLY lame */
  struct hash_link	**buckets;
};

#define WORLD_SIZE	32700
#define	HASH_KEY(ht,key)((((unsigned int)(key))*17)%(ht)->table_size)



struct hunting_data
{
  char			*name;
  struct char_data	**victim;
};

struct room_q
{
  int		room_nr;
  struct room_q	*next_q;
};

struct nodes
{
  int	visited;
  int	ancestor;
};

#define IS_DIR		(get_room_index(q_head->room_nr)->exit[i])
#define GO_OK		(!IS_SET( IS_DIR->exit_info, EX_CLOSED ))
#define GO_OK_SMARTER	1



void init_hash_table(struct hash_header	*ht,int rec_size,int table_size)
{
  ht->rec_size	= rec_size;
  ht->table_size= table_size;
  ht->buckets	= (struct hash_link**)calloc(sizeof(struct hash_link**),table_size);
  ht->keylist	= (int*)malloc(sizeof(ht->keylist)*(ht->klistsize=128));
  ht->klistlen	= 0;
}

void init_world(ROOM_INDEX_DATA *room_db[])
{
  /* zero out the world */
  bzero((wchar_t *)room_db,sizeof(ROOM_INDEX_DATA *)*WORLD_SIZE);
}

CHAR_DATA *get_char_area( CHAR_DATA *ch, wchar_t *argument )
{
   wchar_t arg[MAX_INPUT_LENGTH];
   CHAR_DATA *ach;
   int number;
   int count;

   if( argument[0] == '\0' )
     return NULL;

   number = number_argument( argument, arg );
/*    if (arg[0] == NULL) return NULL; */
   if (arg[0] == '\0') return NULL;
   count = 0;

   if( ( ach = get_char_room( ch, argument ) ) != NULL )
     return ach;



   for( ach = char_list; ach != NULL; ach = ach->next )
   {
     if( ach->in_room && (ach->in_room->area != ch->in_room->area
	|| !can_see( ch, ach ) || !is_name( arg, ach->name )) )
	continue;
     if( ++count == number )
       return ach;
   }
   return NULL;
}


void destroy_hash_table(struct hash_header *ht,void (*gman)(void *))
{
  int			i;
  struct hash_link	*scan,*temp;

  for(i=0;i<ht->table_size;i++)
    for(scan=ht->buckets[i];scan;)
      {
	temp = scan->next;
	(*gman)(scan->data);
	free(scan);
	scan = temp;
      }
  free(ht->buckets);
  free(ht->keylist);
}

void _hash_enter(struct hash_header *ht,int key,void *data)
{
  /* precondition: there is no entry for <key> yet */
  struct hash_link	*temp;
  int			i;

  temp		= (struct hash_link *)malloc(sizeof(struct hash_link));
  temp->key	= key;
  temp->next	= ht->buckets[HASH_KEY(ht,key)];
  temp->data	= data;
  ht->buckets[HASH_KEY(ht,key)] = temp;
  if(ht->klistlen>=ht->klistsize)
    {
      ht->keylist = (int*)realloc(ht->keylist,sizeof(*ht->keylist)*
				   (ht->klistsize*=2));
    }
  for(i=ht->klistlen;i>=0;i--)
    {
      if(ht->keylist[i-1]<key)
	{
	  ht->keylist[i] = key;
	  break;
	}
      ht->keylist[i] = ht->keylist[i-1];
    }
  ht->klistlen++;
}

ROOM_INDEX_DATA *room_find(ROOM_INDEX_DATA *room_db[],int key)
{
  return((key<WORLD_SIZE&&key>-1)?room_db[key]:0);
}

void *hash_find(struct hash_header *ht,int key)
{
  struct hash_link *scan;

  scan = ht->buckets[HASH_KEY(ht,key)];

  while(scan && scan->key!=key)
    scan = scan->next;

  return scan ? scan->data : NULL;
}

int room_enter(ROOM_INDEX_DATA *rb[],int key,ROOM_INDEX_DATA *rm)
{
  ROOM_INDEX_DATA *temp;

  temp = room_find(rb,key);
  if(temp) return(0);

  rb[key] = rm;
  return(1);
}

int hash_enter(struct hash_header *ht,int key,void *data)
{
  void *temp;

  temp = hash_find(ht,key);
  if(temp) return 0;

  _hash_enter(ht,key,data);
  return 1;
}

ROOM_INDEX_DATA *room_find_or_create(ROOM_INDEX_DATA *rb[],int key)
{
  ROOM_INDEX_DATA *rv;

  rv = room_find(rb,key);
  if(rv) return rv;

  rv = (ROOM_INDEX_DATA *)malloc(sizeof(ROOM_INDEX_DATA));
  rb[key] = rv;

  return rv;
}

void *hash_find_or_create(struct hash_header *ht,int key)
{
  void *rval;

  rval = hash_find(ht, key);
  if(rval) return rval;

  rval = (void*)malloc(ht->rec_size);
  _hash_enter(ht,key,rval);

  return rval;
}

int room_remove(ROOM_INDEX_DATA *rb[],int key)
{
  ROOM_INDEX_DATA *tmp;

  tmp = room_find(rb,key);
  if(tmp)
    {
      rb[key] = 0;
      free(tmp);
    }
  return(0);
}

void *hash_remove(struct hash_header *ht,int key)
{
  struct hash_link **scan;

  scan = ht->buckets+HASH_KEY(ht,key);

  while(*scan && (*scan)->key!=key)
    scan = &(*scan)->next;

  if(*scan)
    {
      int		i;
      struct hash_link	*temp, *aux;

      temp	= (hash_link*)(*scan)->data;
      aux	= *scan;
      *scan	= aux->next;
      free(aux);

      for(i=0;i<ht->klistlen;i++)
	if(ht->keylist[i]==key)
	  break;

      if(i<ht->klistlen)
	{
	  bcopy((wchar_t *)ht->keylist+i+1,(wchar_t *)ht->keylist+i,(ht->klistlen-i)
		*sizeof(*ht->keylist));
	  ht->klistlen--;
	}

      return temp;
    }

  return NULL;
}

void room_iterate(ROOM_INDEX_DATA *rb[],void (*func)(int, ROOM_INDEX_DATA *, void * ),void *cdata)
{
  register int i;

  for(i=0;i<WORLD_SIZE;i++)
    {
      ROOM_INDEX_DATA *temp;

      temp = room_find(rb,i);
      if(temp)
		(*func)(i,temp,cdata);
    }
}

void hash_iterate(struct hash_header *ht,void (*func)(int, void*,void*),void *cdata)
{
  int i;

  for(i=0;i<ht->klistlen;i++)
    {
      void		*temp;
      register int	key;

      key = ht->keylist[i];
      temp = hash_find(ht,key);
      (*func)(key,temp,cdata);
      if(ht->keylist[i]!=key) /* They must have deleted this room */
	i--;		      /* Hit this slot again. */
    }
}



int exit_ok( EXIT_DATA *pexit )
{
  ROOM_INDEX_DATA *to_room;

  if ( ( pexit == NULL )
  ||   ( to_room = pexit->u1.to_room ) == NULL )
    return 0;

  return 1;
}

void donothing(void *pDummy)
{
  return;
}

int find_path( int in_room_vnum, int out_room_vnum, CHAR_DATA *ch,
	       int depth, int in_zone )
{
  struct room_q		*tmp_q, *q_head, *q_tail;
  struct hash_header	x_room;
  int			i, tmp_room, count=0, thru_doors;
  ROOM_INDEX_DATA	*herep;
  ROOM_INDEX_DATA	*startp;
  EXIT_DATA		*exitp;

  if ( depth <0 )
    {
      thru_doors = TRUE;
      depth = -depth;
    }
  else
    {
      thru_doors = FALSE;
    }

  startp = get_room_index( in_room_vnum );

  init_hash_table( &x_room, sizeof(int), 2048 );
  hash_enter( &x_room, in_room_vnum, (void *) - 1 );

  /* initialize queue */
  q_head = (struct room_q *) malloc(sizeof(struct room_q));
  q_tail = q_head;
  q_tail->room_nr = in_room_vnum;
  q_tail->next_q = 0;

  while(q_head)
    {
      herep = get_room_index( q_head->room_nr );
      /* for each room test all directions */
      if (herep==NULL) fwprintf( stderr, L"BUG:  Null herep in hunt.c, room #%d",q_head->room_nr);
      if( herep && (herep->area == startp->area || !in_zone) )
		{
	  /* only look in this zone...
	     saves cpu time and  makes world safer for players  */
	  for( i = 0; i <= 5; i++ )
	    {
	      exitp = herep->exit[i];
	      if( exit_ok(exitp) && ( thru_doors ? GO_OK_SMARTER : GO_OK ) )
		{
		  /* next room */
		  tmp_room = herep->exit[i]->u1.to_room->vnum;
		  if( tmp_room != out_room_vnum )
		    {
		      /* shall we add room to queue ?
			 count determines total breadth and depth */
		      if( !hash_find( &x_room, tmp_room )
			 && ( count < depth ) )
			/* && !IS_SET( RM_FLAGS(tmp_room), DEATH ) ) */
			{
			  count++;
			  /* mark room as visted and put on queue */

			  tmp_q = (struct room_q *)
			    malloc(sizeof(struct room_q));
			  tmp_q->room_nr = tmp_room;
			  tmp_q->next_q = 0;
			  q_tail->next_q = tmp_q;
			  q_tail = tmp_q;

			  /* ancestor for first layer is the direction */
			  hash_enter( &x_room, tmp_room,
				     ((intptr_t)hash_find(&x_room,q_head->room_nr)== -1) ? reinterpret_cast<void*>(i+1) : hash_find(&x_room,q_head->room_nr));
			}
		    }
		  else
		    {
		      /* have reached our goal so free queue */
		      tmp_room = q_head->room_nr;
		      for(;q_head;q_head = tmp_q)
			{
			  tmp_q = q_head->next_q;
			  free(q_head);
			}
		      /* return direction if first layer */
		      if ((intptr_t)hash_find(&x_room,tmp_room)==-1)
			{
			  if (x_room.buckets)
			    {
			      /* junk left over from a previous track */
			      destroy_hash_table(&x_room, donothing);
			    }
			  return(i);
			}
		      else
			{
			  /* else return the ancestor */
			  int i;

			  i = (intptr_t)hash_find(&x_room,tmp_room);
			  if (x_room.buckets)
			    {
			      /* junk left over from a previous track */
			      destroy_hash_table(&x_room, donothing);
			    }
			  return( -1+i);
			}
		    }
		}
	    }
	}

      /* free queue head and point to next entry */
      tmp_q = q_head->next_q;
      free(q_head);
      q_head = tmp_q;
    }

  /* couldn't find path */
  if( x_room.buckets )
    {
      /* junk left over from a previous track */
      destroy_hash_table( &x_room, donothing );
    }
  return -1;
}



void do_hunt( CHAR_DATA *ch, wchar_t *argument )
{
  wchar_t buf[MAX_STRING_LENGTH];
  wchar_t arg[MAX_STRING_LENGTH];
  CHAR_DATA *victim;
  int direction,i;
  bool fArea,ok;


    if ( ch_skill_nok(ch,gsn_hunt) )
	return;

    if (!cabal_ok(ch,gsn_hunt)) return;

    if ( !IS_NPC(ch)
    &&   ch->level < skill_table[gsn_hunt].skill_level[ch->iclass] )
    {
	send_to_char( L"Hı?\n\r", ch );
	return;
    }

  one_argument( argument, arg );

  if( arg[0] == '\0' )
    {
      send_to_char( L"Kimi avlamaya çalışıyorsun?\n\r", ch );
      return;
    }

/*  fArea = ( get_trust(ch) < MAX_LEVEL ); */
  fArea = !(IS_IMMORTAL(ch));

  if ( ch->level >= skill_table[gsn_world_find].skill_level[ch->iclass] )
	{
	 if (number_percent() < get_skill(ch,gsn_world_find) )
	  {
	   fArea = 0;
	   check_improve(ch,gsn_world_find,TRUE,1);
	  }
	 check_improve(ch,gsn_world_find,FALSE,1);
	}

  if( fArea )
    victim = get_char_area( ch, arg);
  else
    victim = get_char_world( ch, arg);

  if( victim == NULL )
    {
      send_to_char( L"Etrafta bu isimde biri yok.\n\r", ch );
      return;
    }

  if( ch->in_room == victim->in_room )
    {
      act( L"$N burada!", ch, NULL, victim, TO_CHAR );
      return;
    }

  if( IS_NPC( ch  ) )
  {
    ch->hunting = victim;
    hunt_victim( ch );
    return;
  }


  /*
   * Deduct some movement.
   */
  if (!IS_IMMORTAL(ch))
{
  if( ch->endur > 2 )
    ch->endur -= 3;
  else
    {
      send_to_char( L"Birini avlayamayacak kadar yorgunsun!\n\r", ch );
      return;
    }
}

act( L"$n dikkatle toprağı inceliyor.", ch, NULL, NULL, TO_ROOM );

  WAIT_STATE( ch, skill_table[gsn_hunt].beats );
  direction = find_path( ch->in_room->vnum, victim->in_room->vnum,
			ch, -40000, fArea );

  if( direction == -1 )
    {
      act( L"Burada $E giden bir yol bulamıyorsun.",
	  ch, NULL, victim, TO_CHAR );
      return;
    }

  if( direction < 0 || direction > 5 )
    {
      send_to_char( L"Hmm... Birşeyler hatalı gibi.\n\r", ch );
      return;
    }

  /*
   * Give a random direction if the player misses the die roll.
   */
  if ( IS_NPC (ch) && number_percent () > 75)        /* NPC @ 25% */
    {
    log_string( L"Do PC hunt");
    ok=FALSE;
    for(i=0;i<6;i++) {
    	if (ch->in_room->exit[direction]!=NULL) {
    		ok=TRUE;
    		break;
    		}
    	}
    if (ok)
    {
      do
	{
	  direction = number_door();
	}
      while( ( ch->in_room->exit[direction] == NULL )
	    || ( ch->in_room->exit[direction]->u1.to_room == NULL) );
    }
else {
	  log_string( L"Do hunt, player hunt, no exits from room!");
  	  ch->hunting=NULL;
      send_to_char( L"Odanın çıkışı yok!!!!\n\r",ch);
  	  return;
  	}
  /*
   * Display the results of the search.
   */
  }
  swprintf( buf, MAX_STRING_LENGTH-1, L"$N %s yönünde.", dir_name[direction] );
  act( buf, ch, NULL, victim, TO_CHAR );
  return;
}


/*
 * revised by chronos.
 */
void hunt_victim( CHAR_DATA *ch )
{
  int		dir;
  bool		found;
  CHAR_DATA	*tmp;
  wchar_t		tBuf[MAX_INPUT_LENGTH];

  /*
   * Make sure the victim still exists.
   */
  for( found = 0, tmp = char_list; tmp && !found; tmp = tmp->next )
    if( ch->hunting == tmp )
      found = 1;

  if( !found || !can_see( ch, ch->hunting ) )
    {
     if( get_char_area( ch, ch->hunting->name) != NULL )
        {
           swprintf( tBuf, MAX_STRING_LENGTH-1, L"portal %s", ch->hunting->name );
           log_string( L"mob portal");
           do_cast( ch, tBuf );
           log_string( L"do_enter1");
           do_enter( ch, (wchar_t*)"portal" );
	   if (ch->in_room==NULL || ch->hunting==NULL) return;
	   if( ch->in_room == ch->hunting->in_room )
	    {
        act( L"$n $E dik dik bakarak diyor ki, 'Öleceksin!'",
  	  ch, NULL, ch->hunting, TO_NOTVICT );
        act( L"$n sana dik dik bakarak diyor ki, 'Öleceksin!'",
  	  ch, NULL, ch->hunting, TO_VICT );
        act( L"$E dik dik bakarak diyorsun ki, 'Öleceksin!'",
  	  ch, NULL, ch->hunting, TO_CHAR);
	      multi_hit( ch, ch->hunting, TYPE_UNDEFINED );
      	      ch->hunting = NULL; /* No more hunting, now tracking */
	      return;
	    }
	   log_string( L"done1");
	   return;
        }
       else
	{
         do_say( ch, (wchar_t*)"Ahhhh!  Avım gitti!!" );
         ch->hunting = NULL;
         return;
        }
    }   /* end if !found or !can_see */



  dir = find_path( ch->in_room->vnum, ch->hunting->in_room->vnum,
		  ch, -40000, TRUE );

  if( dir < 0 || dir > 5 )
  {
/* 1 */
    if( get_char_area( ch, ch->hunting->name) != NULL
        && ch-> level > 35 )
    {
      swprintf( tBuf, MAX_STRING_LENGTH-1, L"portal %s", ch->hunting->name );
      log_string( L"mob portal");
      do_cast( ch, tBuf );
      log_string( L"do_enter2");
      do_enter( ch, (wchar_t*)"portal" );
      if (ch->in_room==NULL || ch->hunting==NULL) return;
      if( ch->in_room == ch->hunting->in_room )
       {
         act( L"$n $E dik dik bakarak diyor ki, 'Öleceksin!'",
   	  ch, NULL, ch->hunting, TO_NOTVICT );
         act( L"$n sana dik dik bakarak diyor ki, 'Öleceksin!'",
   	  ch, NULL, ch->hunting, TO_VICT );
         act( L"$E dik dik bakarak diyorsun ki, 'Öleceksin!'",
   	  ch, NULL, ch->hunting, TO_CHAR);
        multi_hit( ch, ch->hunting, TYPE_UNDEFINED );
        ch->hunting = NULL;
        return;
       }
      log_string( L"done2");
      return;
    }
    else
    {
      act( L"$n diyor ki '$M'i kaybettim!'", ch, NULL, ch->hunting, TO_ROOM );
      ch->hunting = NULL;
      return;
    }
   } /* if dir < 0 or > 5 */


  if( ch->in_room->exit[dir] && IS_SET( ch->in_room->exit[dir]->exit_info, EX_CLOSED ) )
    {
      do_open( ch,(wchar_t *)dir_name[dir]);
      return;
    }
  if (!ch->in_room->exit[dir]) {
		log_string( L"BUG:  hunt through null door");
		ch->hunting = NULL;
		return;
		}
  move_char( ch, dir, FALSE );
  if (ch->in_room==NULL || ch->hunting==NULL) return;
  if( ch->in_room == ch->hunting->in_room )
    {
      act( L"$n $E dik dik bakarak diyor ki, 'Öleceksin!'",
	  ch, NULL, ch->hunting, TO_NOTVICT );
      act( L"$n sana dik dik bakarak diyor ki, 'Öleceksin!'",
	  ch, NULL, ch->hunting, TO_VICT );
      act( L"$E dik dik bakarak diyorsun ki, 'Öleceksin!'",
	  ch, NULL, ch->hunting, TO_CHAR);
      multi_hit( ch, ch->hunting, TYPE_UNDEFINED );
      ch->hunting = NULL;
      return;
    }
  return;
}

void hunt_victim_old( CHAR_DATA *ch )
{
  int		dir,i;
  bool		found,ok;
  CHAR_DATA	*tmp;
  wchar_t		tBuf[MAX_INPUT_LENGTH];

  if( ch == NULL || ch->hunting == NULL || !IS_NPC(ch) )
   {
    if (IS_NPC(ch))
      {
    	if ((ROOM_INDEX_DATA*)ch->logon!=ch->in_room)
    	log_string( L"HUNT: Return creature to original home!");
       	act( L"\n\rA glowing portal appears.",ch,NULL,NULL,TO_ROOM);
    	act( L"$n steps through a glowing portal.\n\r",ch,NULL,NULL,TO_ROOM);
       	char_from_room(ch);
    	char_to_room(ch,(ROOM_INDEX_DATA*)ch->logon);
        }
    return;
   }

  /*
   * Make sure the victim still exists.
   */
  for( found = 0, tmp = char_list; tmp && !found; tmp = tmp->next )
    if( ch->hunting == tmp )
      found = 1;

  if( !found || !can_see( ch, ch->hunting ) )
    {
/*1 */  if( get_char_world( ch, ch->hunting->name) != NULL
            && ch-> level > 35 )
        {
           swprintf( tBuf,MAX_INPUT_LENGTH-1, L"portal %s", ch->hunting->name );
           log_string( L"mob portal");
           do_cast( ch, tBuf );
           log_string( L"do_enter1");
           do_enter( ch, (wchar_t*)"portal" );
  /* Deth...this shouldn't have to be here..but it got
  here in a core file with ch->hunting==null.. */
  if (ch->in_room==NULL || ch->hunting==NULL) return;
  if( ch->in_room == ch->hunting->in_room )
    {
      act( L"$n $E dik dik bakarak diyor ki, 'Öleceksin!'",
	  ch, NULL, ch->hunting, TO_NOTVICT );
      act( L"$n sana dik dik bakarak diyor ki, 'Öleceksin!'",
	  ch, NULL, ch->hunting, TO_VICT );
      act( L"$E dik dik bakarak diyorsun ki, 'Öleceksin!'",
	  ch, NULL, ch->hunting, TO_CHAR);
      multi_hit( ch, ch->hunting, TYPE_UNDEFINED );
      ch->hunting = NULL;
      return;
    }
	   log_string( L"done1");
	   	return;
        }
       else {
       if (IS_NPC(ch))
	 {
          if ( (ROOM_INDEX_DATA*)ch->logon!=ch->in_room)
          {
     	   log_string( L"HUNT: Send mob home");
    	   act( L"\n\rA glowing portal appears.",ch,NULL,NULL,TO_ROOM);
    	   act( L"$n steps through a glowing portal.\n\r",ch,NULL,NULL,TO_ROOM);
       	   char_from_room(ch);
    	   char_to_room(ch,(ROOM_INDEX_DATA*)ch->logon);
    	  }
         }

         do_say( ch, (wchar_t*)"Ahhhh!  Avım gitti!!" );
         ch->hunting = NULL;
         return;
        }
    }   /* end if !found or !can_see */



  dir = find_path( ch->in_room->vnum, ch->hunting->in_room->vnum,
		  ch, -40000, TRUE );

  if( dir < 0 || dir > 5 )
  {
/* 1 */
    if( get_char_area( ch, ch->hunting->name) != NULL
        && ch-> level > 35 )
    {
      swprintf( tBuf, MAX_STRING_LENGTH-1, L"portal %s", ch->hunting->name );
      log_string( L"mob portal");
      do_cast( ch, tBuf );
      log_string( L"do_enter2");
      do_enter( ch, (wchar_t*)"portal" );
  /* Deth...this shouldn't have to be here..but it got
  here in a core file with ch->hunting==null.. */
  if (ch->in_room==NULL || ch->hunting==NULL) return;
  if( ch->in_room == ch->hunting->in_room )
    {
      act( L"$n $E dik dik bakarak diyor ki, 'Öleceksin!'",
	  ch, NULL, ch->hunting, TO_NOTVICT );
      act( L"$n sana dik dik bakarak diyor ki, 'Öleceksin!'",
	  ch, NULL, ch->hunting, TO_VICT );
      act( L"$E dik dik bakarak diyorsun ki, 'Öleceksin!'",
	  ch, NULL, ch->hunting, TO_CHAR);
      multi_hit( ch, ch->hunting, TYPE_UNDEFINED );
      ch->hunting = NULL;
      return;
    }
      log_string( L"done2");
      return;
    }
    else
    {
     if (IS_NPC(ch))
       {
    	if ((ROOM_INDEX_DATA*)ch->logon!=ch->in_room)
        {
          log_string( L"HUNT: return creature to original room");
	  act( L"\n\rA glowing portal appears.",ch,NULL,NULL,TO_ROOM);
	  act( L"$n steps through a glowing portal.\n\r",ch,NULL,NULL,TO_ROOM);
	  char_from_room(ch);
    	  char_to_room(ch,(ROOM_INDEX_DATA*)ch->logon);
    	}
      }

      act( L"$n says 'I have lost $M!'", ch, NULL, ch->hunting, TO_ROOM );
      ch->hunting = NULL;
      return;
    }
   } /* if dir < 0 or > 5 */

  /*
   * Give a random direction if the mob misses the die roll.
   */
  if( number_percent () > 75 )        /* @ 25% */
    {
    	ok=FALSE;
        for(i=0;i<6;i++) {
    	if (ch->in_room->exit[dir]!=NULL) {
    		ok=TRUE;
    		break;
    		}
    	}
	if(ok) {
      do
        {
	  dir = number_door();
        }
      while( ( ch->in_room->exit[dir] == NULL )
	    || ( ch->in_room->exit[dir]->u1.to_room == NULL ) );
	   }
	  else {
	  log_string( L"Do hunt, player hunt, no exits from room!");
  	  ch->hunting=NULL;
  	  send_to_char( L"Your room has not exits!!!!\n\r",ch);
  	  return;
  	}
    }



  if( ch->in_room->exit[dir] && IS_SET( ch->in_room->exit[dir]->exit_info, EX_CLOSED ) )
    {
      do_open( ch,(wchar_t *)dir_name[dir]);
      return;
    }
	if (!ch->in_room->exit[dir]) {
		log_string( L"BUG:  hunt through null door");
		return;
		}
  move_char( ch, dir, FALSE );
  /* Deth...this shouldn't have to be here..but it got
  here in a core file with ch->hunting==null.. */
  if (ch->in_room==NULL || ch->hunting==NULL) return;
  if( ch->in_room == ch->hunting->in_room )
    {
      act( L"$n $E dik dik bakarak diyor ki, 'Öleceksin!'",
	  ch, NULL, ch->hunting, TO_NOTVICT );
      act( L"$n sana dik dik bakarak diyor ki, 'Öleceksin!'",
	  ch, NULL, ch->hunting, TO_VICT );
      act( L"$E dik dik bakarak diyorsun ki, 'Öleceksin!'",
	  ch, NULL, ch->hunting, TO_CHAR);
      multi_hit( ch, ch->hunting, TYPE_UNDEFINED );
      ch->hunting = NULL;
      return;
    }
  return;
}


/***************************************************************************
 ************************      repair.c       ******************************
 ***************************************************************************/


void damage_to_obj(CHAR_DATA *ch,OBJ_DATA *wield, OBJ_DATA *worn, int damage);

void damage_to_obj(CHAR_DATA *ch,OBJ_DATA *wield, OBJ_DATA *worn, int damage)
{
 wchar_t buf[MAX_STRING_LENGTH];

 if ( damage == 0) return;

 if( gorev_ekipmani_mi( worn ) )
 {
   act_color( L"$C$p tarafından verilecek hasarı $P engelliyor.$c",ch,wield,worn,TO_ROOM,POS_RESTING,CLR_RED);
   return;
 }
 worn->condition -= damage;

 act_color( L"$C$p $P üzerine hasar bırakıyor.$c",
	ch,wield,worn,TO_ROOM,POS_RESTING,CLR_RED);

 if (worn->condition < 1)
	{
    act_color( L"$C$P parçalara ayrılıyor.$c",
	ch,wield,worn,TO_ROOM,POS_RESTING,CLR_WHITE);
	extract_obj( worn );
	return;
	}

 if ( (IS_SET(wield->extra_flags,ITEM_ANTI_EVIL)
	&& IS_SET(wield->extra_flags,ITEM_ANTI_NEUTRAL) )
     && (IS_SET(worn->extra_flags,ITEM_ANTI_EVIL)
	&& IS_SET(worn->extra_flags,ITEM_ANTI_NEUTRAL) ) )
 {
   swprintf( buf, MAX_STRING_LENGTH-1, L"$C$p $P'ye karşı dövüşmek istemiyor.$c");
   act_color(buf,ch,wield,worn,TO_ROOM,POS_RESTING,CLR_GREEN);
   swprintf( buf, MAX_STRING_LENGTH-1, L"$C$p kendisini senden kurtarıyor!$c.");
   act_color(buf,ch,wield,worn,TO_CHAR,POS_RESTING,CLR_GREEN);
   swprintf( buf, MAX_STRING_LENGTH-1, L"$C$p kendisini $z kurtarıyor$c.");
  act_color(buf,ch,wield,worn,TO_ROOM,POS_RESTING,CLR_GREEN);
  unequip_char( ch, wield );
  return;
 }

 if (IS_SET(wield->extra_flags,ITEM_ANTI_EVIL)
	&& IS_SET(worn->extra_flags,ITEM_ANTI_EVIL))
 {
   swprintf( buf, MAX_STRING_LENGTH-1, L"$C$p $P üzerine zarar uygulamaktan korkuyor.$c");
  act_color(buf,ch,wield,worn,TO_ROOM,POS_RESTING,CLR_GREEN);
  return;
 }
 return;
}


void check_weapon_destroy(CHAR_DATA *ch, CHAR_DATA *victim,bool second)
{
 OBJ_DATA *wield,*destroy;
 int skill,chance=0,sn,i;

 if (IS_NPC(victim) || number_percent() < 94 )  return;

 if ( (wield = get_wield_char(ch,second)) == NULL)
   return;
 sn = get_weapon_sn(ch,second);
 skill = get_skill(ch, sn );

 if (is_metal(wield))
      {
	for (i=0;i < MAX_WEAR; i++)
	{
	 if ( ( destroy = get_eq_char(victim,i) ) == NULL
		|| number_percent() > 95
		|| number_percent() > 94
	 	|| number_percent() > skill
		|| ch->level < (victim->level - 10)
		|| check_material(destroy,(wchar_t*)"platinum")
		|| destroy->pIndexData->limit != -1
		|| (i == WEAR_LEFT || i== WEAR_RIGHT || i== WEAR_BOTH
			|| i == WEAR_TATTOO || i == WEAR_STUCK_IN) )
	 continue;

	 chance += 20;
	 if ( check_material(wield, (wchar_t*)"platinium") ||
	      check_material(wield, (wchar_t*)"titanium") )
	 chance += 5;

	 if ( is_metal(destroy) )  chance -= 20;
	 else 			chance += 20;

	 chance += ( (ch->level - victim->level) / 5);

	 chance += ( (wield->level - destroy->level) / 2 );

	/* sharpness	*/
	 if ( IS_WEAPON_STAT(wield,WEAPON_SHARP) )
		chance += 10;

	 if ( sn == gsn_axe ) chance += 10;
	/* spell affects */
	 if ( IS_OBJ_STAT( destroy, ITEM_BLESS) ) chance -= 10;
	 if ( IS_OBJ_STAT( destroy, ITEM_MAGIC) ) chance -= 20;

	 chance += skill - 85 ;
	 chance += get_curr_stat( ch, STAT_STR);

/*	 chance /= 2;	*/
	 if (number_percent() < chance && chance > 50)
		{
		 damage_to_obj(ch,wield,destroy, (chance / 5) );
		 break;
		}
	}
      }
 else {
	for (i=0;i < MAX_WEAR;i++)
	{
	 if ( ( destroy = get_eq_char(victim,i) ) == NULL
		|| number_percent() > 95
		|| number_percent() > 94
	 	|| number_percent() < skill
		|| ch->level < (victim->level - 10)
		|| check_material(destroy,(wchar_t*)"platinum")
		|| destroy->pIndexData->limit != -1
		|| (i == WEAR_LEFT || i== WEAR_RIGHT || i==WEAR_BOTH
			|| i == WEAR_TATTOO || i == WEAR_STUCK_IN ) )
	 continue;

	 chance += 10;

	 if ( is_metal(destroy) )  chance -= 20;

	 chance += (ch->level - victim->level);

	 chance += (wield->level - destroy->level);

	/* sharpness	*/
	 if ( IS_WEAPON_STAT(wield,WEAPON_SHARP) )
		chance += 10;

	 if ( sn == gsn_axe ) chance += 10;

	/* spell affects */
	 if ( IS_OBJ_STAT( destroy, ITEM_BLESS) ) chance -= 10;
	 if ( IS_OBJ_STAT( destroy, ITEM_MAGIC) ) chance -= 20;

	 chance += skill - 85 ;
	 chance += get_curr_stat( ch, STAT_STR);

/*	 chance /= 2;	*/
	 if (number_percent() < chance && chance > 50)
		{
		 damage_to_obj(ch,wield,destroy, chance / 5 );
		 break;
		}
	}
      }

 return;
}


void do_repair(CHAR_DATA *ch, wchar_t *argument)
{
    CHAR_DATA *mob;
    wchar_t arg[MAX_INPUT_LENGTH];
    wchar_t buf[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int cost;

    for ( mob = ch->in_room->people; mob; mob = mob->next_in_room )
    {
        if (!IS_NPC(mob)) continue;
	if (mob->spec_fun == spec_lookup((wchar_t*)"spec_repairman") )
            break;
    }

    if ( mob == NULL )
    {
      send_to_char( L"Burada yapamazsın.\n\r", ch );
        return;
    }

    one_argument(argument,arg);

    if (arg[0] == '\0')
    {
	do_say(mob,(wchar_t*)"Senin için bir silahı onarabilirim. Tabii ücret karşılığında.");
	send_to_char( L"Type estimate <weapon> to be assessed for damage.\n\r",ch);
	return;
    }
    if (( obj = get_obj_carry(ch, arg)) == NULL)
    {
	do_say(mob,(wchar_t*)"Sende bu eşya yok.");
	return;
    }

    if (obj->pIndexData->vnum == OBJ_VNUM_HAMMER)
    {
     do_say(mob,(wchar_t*)"Bu çekiç benim ustalığımın üstünde.");
     return;
    }

    if (obj->condition >= 100)
    {
	do_say(mob,(wchar_t*)"Bu eşya zaten iyi durumda.");
        return;
    }

    if (obj->cost == 0)
    {
	swprintf( buf, MAX_STRING_LENGTH-1, L"%s tamir edilemeyecek durumda.\n\r", obj->short_descr);
	do_say(mob,buf);
   	return;
    }

    cost = ( (obj->level * 10) +
		((obj->cost * (100 - obj->condition)) /100)    );

    if (cost > ch->silver)
    {
	do_say(mob,(wchar_t*)"Hizmetlerimden yararlanmak için yeterince paran yok.");
	return;
    }

    WAIT_STATE(ch,PULSE_VIOLENCE);

    ch->silver -= cost;
    mob->silver += cost;
    swprintf( buf, MAX_STRING_LENGTH-1, L"$N $n'dan %s'ı alıyor, tamir ediyor ve $n'a geri veriyor.", obj->short_descr);
    act(buf,ch,NULL,mob,TO_ROOM);
    swprintf( buf, MAX_STRING_LENGTH-1, L"%s %s'ı alıp, tamir edip sana geri veriyor.\n\r", mob->short_descr, obj->short_descr);
    send_to_char(buf, ch);
    obj->condition = 100;
}

void do_estimate(CHAR_DATA *ch, wchar_t *argument)
{
    wchar_t buf[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    CHAR_DATA *mob;
    wchar_t arg[MAX_INPUT_LENGTH];
    int cost;

    for ( mob = ch->in_room->people; mob; mob = mob->next_in_room )
    {
        if (!IS_NPC(mob)) continue;
	if (mob->spec_fun == spec_lookup((wchar_t*)"spec_repairman") )
            break;
    }

    if ( mob == NULL )
    {
        send_to_char( L"Burada yapamazsın..\n\r", ch );
        return;
    }

    one_argument(argument, arg);

    if (arg[0] == '\0')
    {
	do_say(mob,(wchar_t*)"Ücret <eşya> yazmayı dene.");
   	return;
    }
    if ((obj = (get_obj_carry(ch, arg))) == NULL)
    {
	do_say(mob,(wchar_t*)"Sende bu eşya yok.");
	return;
    }
    if (obj->pIndexData->vnum == OBJ_VNUM_HAMMER)
	{
	    do_say(mob,(wchar_t*)"Bu çekiç benim ustalığımın üstünde.");
	    return;
	}
    if (obj->condition >= 100)
    {
	do_say(mob,(wchar_t*)"Bu eşya zaten iyi durumda.");
	return;
    }
    if (obj->cost == 0)
    {
	do_say(mob,(wchar_t*)"Bu eşya tamir edilemeyecek durumda.");
    	return;
    }

    cost = ( (obj->level * 10) +
		((obj->cost * (100 - obj->condition)) /100)    );

    swprintf( buf, MAX_STRING_LENGTH-1, L"Bu eşyayı tamir etmek sana %d akçeye patlar.", cost);
    do_say(mob,buf);
}

void do_restring( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t buf  [MAX_INPUT_LENGTH];
    wchar_t arg  [MAX_INPUT_LENGTH];
    wchar_t arg1 [MAX_INPUT_LENGTH];
    wchar_t arg2 [MAX_INPUT_LENGTH];
    CHAR_DATA *mob;
    OBJ_DATA *obj;
    int cost = 2000;

    for ( mob = ch->in_room->people; mob; mob = mob->next_in_room )
    {
        if ( IS_NPC(mob) && IS_SET(mob->act, ACT_IS_HEALER) )
            break;
    }

    if ( mob == NULL )
    {
      send_to_char( L"Burada yapamazsın.\n\r", ch );
        return;
    }

    smash_tilde( argument );
    argument = one_argument( argument, arg );
    argument = one_argument( argument, arg1 );
    wcscpy( arg2, argument );

	if ( arg[0] == '\0' || arg1[0] == '\0' || arg2[0] == '\0' )
	{
    send_to_char( L"Yazım:\n\r",ch);
		send_to_char( L"  restring <obj> <field> <string>\n\r",ch);
		send_to_char( L"    fields: name short long\n\r",ch);
		return;
	}

    if ((obj = (get_obj_carry(ch, arg))) == NULL)
    {
      send_to_char( L"Stringer '`Sende ondan yok`` dedi.'\n\r",ch);
		return;
    }

	cost += (obj->level * 1500);

    if (cost > ch->silver)
    {
      act( L"$N 'Hizmetlerim için yeterli akçen yok,' dedi.",
		  ch,NULL,mob,TO_CHAR);
		return;
    }

	if ( !str_prefix( arg1, L"name" ) )
	{
		free_string( obj->name );
		obj->name = str_dup( arg2 );
	}
	else
	if ( !str_prefix( arg1, L"short" ) )
	{
		free_string( obj->short_descr );
        obj->short_descr = str_dup(arg2);
	}
	else
	if ( !str_prefix( arg1, L"long" ) )
	{
		free_string( obj->description );
		obj->description = str_dup( arg2 );
	}
	else
	{
    send_to_char( L"Geçerli bir Field değil.\n\r",ch);
		return;
	}

    WAIT_STATE(ch,PULSE_VIOLENCE);

    ch->silver -= cost;
    mob->silver += cost;
    swprintf( buf, MAX_STRING_LENGTH-1, L"$N takes $n's item, tinkers with it, and returns it to $n.");
	act(buf,ch,NULL,mob,TO_ROOM);
  swprintf( buf, MAX_STRING_LENGTH-1, L"%s takes your item, tinkers with it, and returns %s to you.\n\r", mob->short_descr, obj->short_descr);
  send_to_char(buf,ch);
  send_to_char( L"Remember, if we find your new string offensive, we will not be happy.\n\r", ch);
  send_to_char( L" This is your ONE AND ONLY Warning.\n\r", ch);
}

void check_shield_destroyed(CHAR_DATA *ch, CHAR_DATA *victim,bool second)
{
 OBJ_DATA *wield,*destroy;
 int skill,chance=0,sn;

 if (IS_NPC(victim) || number_percent() < 94 )  return;

 if ( (wield = get_wield_char(ch,second)) == NULL)
   return;
 sn = get_weapon_sn(ch,second);
 skill = get_skill(ch, sn );

 destroy = get_shield_char(victim);

 if (destroy == NULL) return;

 if (is_metal(wield))
      {
	 if (   number_percent() > 94
	 	|| number_percent() > skill
		|| ch->level < (victim->level - 10)
		|| check_material(destroy,(wchar_t*)"platinum")
		|| destroy->pIndexData->limit != -1 )
	 return;

	 chance += 20;
	 if ( check_material(wield, (wchar_t*)"platinium") ||
	      check_material(wield, (wchar_t*)"titanium") )
	 chance += 5;

	 if ( is_metal(destroy) )  chance -= 20;
	 else 			chance += 20;

	 chance += ( (ch->level - victim->level) / 5);

	 chance += ( (wield->level - destroy->level) / 2 );

	/* sharpness	*/
	 if ( IS_WEAPON_STAT(wield,WEAPON_SHARP) )
		chance += 10;

	 if ( sn == gsn_axe ) chance += 10;
	/* spell affects */
	 if ( IS_OBJ_STAT( destroy, ITEM_BLESS) ) chance -= 10;
	 if ( IS_OBJ_STAT( destroy, ITEM_MAGIC) ) chance -= 20;

	 chance += skill - 85 ;
	 chance += get_curr_stat( ch, STAT_STR);

/* 	 chance /= 2;	*/
	 if (number_percent() < chance && chance > 20 )
		{
		 damage_to_obj(ch,wield,destroy, (chance / 4) );
		 return;
		}
      }
 else {
	 if (   number_percent() > 94
	 	|| number_percent() < skill
		|| ch->level < (victim->level - 10)
		|| check_material(destroy,(wchar_t*)"platinum")
		|| destroy->pIndexData->limit != -1 )
	 return;

	 chance += 10;

	 if ( is_metal(destroy) )  chance -= 20;

	 chance += (ch->level - victim->level);

	 chance += (wield->level - destroy->level);

	/* sharpness	*/
	 if ( IS_WEAPON_STAT(wield,WEAPON_SHARP) )
		chance += 10;

	 if ( sn == gsn_axe ) chance += 10;

	/* spell affects */
	 if ( IS_OBJ_STAT( destroy, ITEM_BLESS) ) chance -= 10;
	 if ( IS_OBJ_STAT( destroy, ITEM_MAGIC) ) chance -= 20;

	 chance += skill - 85 ;
	 chance += get_curr_stat( ch, STAT_STR);

/*	 chance /= 2;	*/
	 if (number_percent() < chance && chance > 20)
		{
		 damage_to_obj(ch,wield,destroy, (chance / 4) );
		 return;
		}
      }
 return;
}

void check_weapon_destroyed(CHAR_DATA *ch, CHAR_DATA *victim,bool second)
{
 OBJ_DATA *wield,*destroy;
 int skill,chance=0,sn;

 if (IS_NPC(victim) || number_percent() < 94 )  return;

 if ( (wield = get_wield_char(ch,second)) == NULL)
   return;
 sn = get_weapon_sn(ch, second);
 skill = get_skill(ch, sn );

 destroy = get_wield_char(victim,FALSE);
 if (destroy == NULL ) return;

 if (is_metal(wield))
      {
	 if (   number_percent() > 94
	 	|| number_percent() > skill
		|| ch->level < (victim->level - 10)
		|| check_material(destroy,(wchar_t*)"platinum")
		|| destroy->pIndexData->limit != -1 )
	 return;

	 chance += 20;
	 if ( check_material(wield, (wchar_t*)"platinium") ||
	      check_material(wield, (wchar_t*)"titanium") )
	 chance += 5;

	 if ( is_metal(destroy) )  chance -= 20;
	 else 			chance += 20;

	 chance += ( (ch->level - victim->level) / 5);

	 chance += ( (wield->level - destroy->level) / 2 );

	/* sharpness	*/
	 if ( IS_WEAPON_STAT(wield,WEAPON_SHARP) )
		chance += 10;

	 if ( sn == gsn_axe ) chance += 10;
	/* spell affects */
	 if ( IS_OBJ_STAT( destroy, ITEM_BLESS) ) chance -= 10;
	 if ( IS_OBJ_STAT( destroy, ITEM_MAGIC) ) chance -= 20;

	 chance += skill - 85 ;
	 chance += get_curr_stat( ch, STAT_STR);

/*	 chance /= 2;	*/
	 if (number_percent() < (chance / 2) && chance > 20 )
		{
		 damage_to_obj(ch,wield,destroy, (chance / 4) );
		 return;
		}
      }
 else {
	 if (   number_percent() > 94
	 	|| number_percent() < skill
		|| ch->level < (victim->level - 10)
		|| check_material(destroy,(wchar_t*)"platinum")
		|| destroy->pIndexData->limit != -1 )
	 return;

	 chance += 10;

	 if ( is_metal(destroy) )  chance -= 20;

	 chance += (ch->level - victim->level);

	 chance += (wield->level - destroy->level);

	/* sharpness	*/
	 if ( IS_WEAPON_STAT(wield,WEAPON_SHARP) )
		chance += 10;

	 if ( sn == gsn_axe ) chance += 10;

	/* spell affects */
	 if ( IS_OBJ_STAT( destroy, ITEM_BLESS) ) chance -= 10;
	 if ( IS_OBJ_STAT( destroy, ITEM_MAGIC) ) chance -= 20;

	 chance += skill - 85 ;
	 chance += get_curr_stat( ch, STAT_STR);

/*	 chance /= 2;	*/
	 if (number_percent() < (chance / 2) && chance > 20 )
		{
		 damage_to_obj(ch,wield,destroy, chance / 4 );
		 return;
		}
      }
 return;
}


void do_smithing(CHAR_DATA *ch, wchar_t *argument)
{
    wchar_t arg[MAX_INPUT_LENGTH];
    wchar_t buf[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *hammer;

    if ( IS_NPC(ch)
    ||   ch->level < skill_table[gsn_smithing].skill_level[ch->iclass] )
    {
	send_to_char( L"Hı?\n\r", ch );
	return;
    }


    if ( ch->fighting )
    {
      send_to_char( L"Dövüş bitene kadar bekle.\n\r", ch );
        return;
    }

    one_argument(argument,arg);

    if (arg[0] == '\0')
    {
      send_to_char( L"Neyi tamir etmek istiyorsun?\n\r",ch);
	return;
    }

    if (( obj = get_obj_carry(ch, arg)) == NULL)
    {
      send_to_char( L"Onu taşımıyorsun.\n\r",ch);
	return;
    }

   if (obj->condition >= 100)
    {
      send_to_char( L"Eşyan zarar görmemiş.\n\r",ch);
	return;
    }

    if (( hammer = get_hold_char(ch)) == NULL)
    {
      send_to_char( L"Bir çekiç taşımıyorsun.\n\r",ch);
	return;
    }

    if ( hammer->pIndexData->vnum != OBJ_VNUM_HAMMER )
    {
      send_to_char( L"Elindeki çekiç uygun değil.\n\r",ch);
	return;
    }

    WAIT_STATE(ch,2 * PULSE_VIOLENCE);
    if ( number_percent() > get_skill(ch,gsn_smithing) )
     {
      check_improve(ch,gsn_smithing,FALSE,8);
      swprintf( buf, MAX_STRING_LENGTH-1, L"$n %s eşyasını çekiçle tamir etmeyi denedi fakat beceremedi.", obj->short_descr);
    act(buf,ch,NULL,obj,TO_ROOM);
    swprintf( buf, MAX_STRING_LENGTH-1, L"%s eşyasını tamir etmeyi beceremedin.\n\r", obj->short_descr);
    send_to_char(buf, ch);
    hammer->condition -= 25;
     }
    else
     {
    check_improve(ch,gsn_smithing,TRUE,4);
    swprintf( buf, MAX_STRING_LENGTH-1, L"$n %s eşyasını çekiçle tamir etti.", obj->short_descr);
    act(buf,ch,NULL,NULL,TO_ROOM);
    swprintf( buf, MAX_STRING_LENGTH-1, L"%s eşyasını tamir ettin.\n\r", obj->short_descr);
    send_to_char(buf, ch);
    obj->condition = UMAX( 100 ,
	 obj->condition + ( get_skill(ch,gsn_smithing) / 2) );
    hammer->condition -= 25;
     }
   if (hammer->condition < 1)  extract_obj( hammer );
   return;
}

/***************************************************************************
 ************************      auction.c      ******************************
 ***************************************************************************/

/***************************************************************************
 *  This snippet was orginally written by Erwin S. Andreasen.              *
 *	erwin@pip.dknet.dk, http://pip.dknet.dk/~pip1773/                  *
 *  Adopted to Anatolia MUD by chronos.                                    *
 ***************************************************************************/

DECLARE_SPELL_FUN(	spell_identify	);

void talk_auction(wchar_t *argument)
{
    DESCRIPTOR_DATA *d;
    wchar_t buf[MAX_STRING_LENGTH];
    CHAR_DATA *original;

    swprintf (buf,MAX_INPUT_LENGTH-1, L"MEZAT: %s", argument);

    for (d = descriptor_list; d != NULL; d = d->next)
    {
        original = d->original ? d->original : d->character; /* if switched */
        if ((d->connected == CON_PLAYING) && !IS_SET(original->comm,COMM_NOAUCTION) )
            act (buf, original, NULL, NULL, TO_CHAR);

    }
}


/*
  This function allows the following kinds of bets to be made:

  Absolute bet
  ============

  bet 14k, bet 50m66, bet 100k

  Relative bet
  ============

  These bets are calculated relative to the current bet. The '+' symbol adds
  a certain number of percent to the current bet. The default is 25, so
  with a current bet of 1000, bet + gives 1250, bet +50 gives 1500 etc.
  Please note that the number must follow exactly after the +, without any
  spaces!

  The '*' or 'x' bet multiplies the current bet by the number specified,
  defaulting to 2. If the current bet is 1000, bet x  gives 2000, bet x10
  gives 10,000 etc.

*/

int advatoi (const wchar_t *s)
/*
  util function, converts an 'advanced' ASCII-number-string into a number.
  Used by parsebet() but could also be used by do_give or do_wimpy.

  Advanced strings can contain 'k' (or 'K') and 'm' ('M') in them, not just
  numbers. The letters multiply whatever is left of them by 1,000 and
  1,000,000 respectively. Example:

  14k = 14 * 1,000 = 14,000
  23m = 23 * 1,000,0000 = 23,000,000

  If any digits follow the 'k' or 'm', the are also added, but the number
  which they are multiplied is divided by ten, each time we get one left. This
  is best illustrated in an example :)

  14k42 = 14 * 1000 + 14 * 100 + 2 * 10 = 14420

  Of course, it only pays off to use that notation when you can skip many 0's.
  There is not much point in writing 66k666 instead of 66666, except maybe
  when you want to make sure that you get 66,666.

  More than 3 (in case of 'k') or 6 ('m') digits after 'k'/'m' are automatically
  disregarded. Example:

  14k1234 = 14,123

  If the number contains any other characters than digits, 'k' or 'm', the
  function returns 0. It also returns 0 if 'k' or 'm' appear more than
  once.

*/

{

/* the pointer to buffer stuff is not really necessary, but originally I
   modified the buffer, so I had to make a copy of it. What the hell, it
   works:) (read: it seems to work:)
*/

  wchar_t string[MAX_INPUT_LENGTH]; /* a buffer to hold a copy of the argument */
  wchar_t *stringptr = string; /* a pointer to the buffer so we can move around */
  wchar_t tempstring[2];       /* a small temp buffer to pass to atoi*/
  int number = 0;           /* number to be returned */
  int multiplier = 0;       /* multiplier used to get the extra digits right */


  wcscpy (string,s);        /* working copy */

  while ( iswdigit (*stringptr)) /* as long as the current character is a digit */
  {
      wcsncpy (tempstring,stringptr,1);           /* copy first digit */
      number = (number * 10) + wcstol (tempstring, NULL, 10); /* add to current number */
      stringptr++;                                /* advance */
  }

  switch (towupper(*stringptr)) {
      case 'K'  : multiplier = 1000;    number *= multiplier; stringptr++; break;
      case 'M'  : multiplier = 1000000; number *= multiplier; stringptr++; break;
      case '\0' : break;
      default   : return 0; /* not k nor m nor NUL - return 0! */
  }

  while ( iswdigit (*stringptr) && (multiplier > 1)) /* if any digits follow k/m, add those too */
  {
      wcsncpy (tempstring,stringptr,1);           /* copy first digit */
      multiplier = multiplier / 10;  /* the further we get to right, the less are the digit 'worth' */
      number = number + (wcstol (tempstring, NULL, 10) * multiplier);
      stringptr++;
  }

  if (*stringptr != '\0' && !iswdigit(*stringptr)) /* a non-digit character was found, other than NUL */
    return 0; /* If a digit is found, it means the multiplier is 1 - i.e. extra
                 digits that just have to be ignore, liked 14k4443 -> 3 is ignored */


  return (number);
}


int parsebet (const int currentbet, const wchar_t *argument)
{

  int newbet = 0;                /* a variable to temporarily hold the new bet */
  wchar_t string[MAX_INPUT_LENGTH]; /* a buffer to modify the bet string */
  wchar_t *stringptr = string;      /* a pointer we can move around */
  wchar_t buf2[MAX_STRING_LENGTH];

  wcscpy (string,argument);      /* make a work copy of argument */


  if (*stringptr)               /* check for an empty string */
  {

    if (iswdigit (*stringptr)) /* first char is a digit assume e.g. 433k */
      newbet = advatoi (stringptr); /* parse and set newbet to that value */

    else
      if (*stringptr == '+') /* add ?? percent */
      {
        if (wcslen (stringptr) == 1) /* only + specified, assume default */
          newbet = (currentbet * 125) / 100; /* default: add 25% */
        else
          newbet = (currentbet * (100 + wcstol (++stringptr, NULL, 10))) / 100; /* cut off the first wchar_t */
      }
      else
        {
        swprintf (buf2,MAX_STRING_LENGTH-1,L"considering: * x \n\r");
        if ((*stringptr == '*') || (*stringptr == 'x')) /* multiply */
        {
          if (wcslen (stringptr) == 1) /* only x specified, assume default */
            newbet = currentbet * 2 ; /* default: twice */
          else /* user specified a number */
            newbet = currentbet * wcstol (++stringptr, NULL, 10); /* cut off the first wchar_t */
        }
        }
  }

  return newbet;        /* return the calculated bet */
}



void auction_update (void)
{
    wchar_t buf[MAX_STRING_LENGTH];
    wchar_t bufc[MAX_STRING_LENGTH];

    if (auction->item != NULL)
        if (--auction->pulse <= 0) /* decrease pulse */
        {
            auction->pulse = PULSE_AUCTION;
            switch (++auction->going) /* increase the going state */
            {
            case 1 : /* going once */
            case 2 : /* going twice */
            if (auction->bet > 0)
            swprintf (buf, MAX_STRING_LENGTH-1, L"%s: %d akçeye gidiyor.", auction->item->short_descr,auction->bet);
            else
            swprintf (buf, MAX_STRING_LENGTH-1, L"%s: henüz teklif yok.", auction->item->short_descr);
	          swprintf( bufc, MAX_STRING_LENGTH-1, L"%s%s%s",CLR_CYAN,buf,CLR_WHITE_BOLD);
            talk_auction (bufc);
            break;

            case 3 : /* SOLD! */

            if (auction->bet > 0)
            {
              swprintf (buf, MAX_STRING_LENGTH-1, L"%s: %s %d akçeye satın aldı.",
                    auction->item->short_descr,
                    IS_NPC(auction->buyer) ? auction->buyer->short_descr : auction->buyer->name,
                    auction->bet);
	        swprintf( bufc, MAX_STRING_LENGTH-1, L"%s%s%s",CLR_CYAN,buf,CLR_WHITE_BOLD);
                talk_auction(bufc);
                obj_to_char (auction->item,auction->buyer);
                act ( L"Mezatçı yanında belirerek $p eşyasını sana veriyor.",
                     auction->buyer,auction->item,NULL,TO_CHAR);
                act ( L"Mezatçı beliriyor ve $p eşyasını $e veriyor.",
                     auction->buyer,auction->item,NULL,TO_ROOM);

                auction->seller->silver += auction->bet; /* give him the money */

                auction->item = NULL; /* reset item */

            }
            else /* not sold */
            {
              swprintf (buf, MAX_STRING_LENGTH-1, L"%s teklif gelmediği için mezattan çekildi.",auction->item->short_descr);
			  swprintf( bufc, MAX_STRING_LENGTH-1, L"%s%s%s",CLR_CYAN,buf,CLR_WHITE_BOLD);
              talk_auction(bufc);
              swprintf (buf, MAX_STRING_LENGTH-1, L"Mezatçı satılmayan eşyayı depoya kaldırıyor.");
			  swprintf( bufc, MAX_STRING_LENGTH-1, L"%s%s%s",CLR_RED,buf,CLR_WHITE_BOLD);
              talk_auction(bufc);
                extract_obj(auction->item);
                auction->item = NULL; /* clear auction */

            } /* else */

            } /* switch */
        } /* if */
} /* func */


void do_auction (CHAR_DATA *ch, wchar_t *argument)
{
    OBJ_DATA *obj;
    wchar_t arg1[MAX_INPUT_LENGTH];
    wchar_t buf[MAX_STRING_LENGTH];
    wchar_t bufc[MAX_STRING_LENGTH];
    wchar_t betbuf[MAX_STRING_LENGTH];
    int i;

    argument = one_argument (argument, arg1);

	if (IS_NPC(ch))    /* NPC extracted can't auction! */
		return;

    if (IS_SET(ch->comm,COMM_NOAUCTION))
	{
    if (!wcscasecmp(arg1,L"açık") )
	 {
     send_to_char( L"Mezat kanalı açıldı.\n\r",ch);
	  REMOVE_BIT(ch->comm,COMM_NOAUCTION);
	  return;
	 }
	 else
	 {
     send_to_char( L"Mezat kanalı kapandı.\n\r",ch);
 	  send_to_char( L"Mezat kanalın açık değil.\n\r",ch);
	  return;
	 }
	}

    if (arg1[0] == '\0')
    {
        if (auction->item != NULL)
        {
            /* show item data here */
            if (auction->bet > 0)
            swprintf (buf, MAX_STRING_LENGTH-1, L"Bu objeye son verilen teklif %d akçe.\n\r",auction->bet);
            else
            swprintf (buf, MAX_STRING_LENGTH-1, L"Henüz teklif verilmedi.\n\r");
	    swprintf( bufc, MAX_STRING_LENGTH-1, L"%s%s%s",CLR_GREEN,buf,CLR_WHITE_BOLD);
            send_to_char (bufc,ch);
	    spell_identify(0, 0, ch, auction->item,0);
            return;
        }
        else
        {
          printf_to_char(ch,L"{rNeyi mezata çıkaracaksın?{x\n\r");
            return;
        }
    }

    if (!wcscasecmp(arg1,L"kapalı") )
	{
    send_to_char( L"Mezat kanalı kapandı.\n\r",ch);
	 SET_BIT(ch->comm,COMM_NOAUCTION);
	 return;
	}

  if (IS_IMMORTAL(ch) && !wcscasecmp(arg1,L"durdur"))
    {
    if (auction->item == NULL)
    {
      send_to_char ( L"Durdurabileceğin bir mezat yürümüyor.\n\r",ch);
        return;
    }
    else /* stop the auction */
    {
      swprintf( buf, MAX_STRING_LENGTH-1, L"%s objesinin satışı bir ölümsüz tarafından durduruldu.",auction->item->short_descr);
	swprintf( bufc, MAX_STRING_LENGTH-1, L"%s%s%s",CLR_WHITE,buf,CLR_WHITE_BOLD);
        talk_auction(bufc);
        obj_to_char(auction->item, auction->seller);
        auction->item = NULL;
        if (auction->buyer != NULL) /* return money to the buyer */
        {
            auction->buyer->silver += auction->bet;
            printf_to_char (auction->buyer, L"%d akçen iade edildi.\n\r",auction->bet);
        }
        return;
    }
    }

    if  (!wcscasecmp(arg1,L"teklif") )
    {
	if (auction->item != NULL)
        {
            int newbet;

	if ( ch == auction->seller )
		{
      send_to_char( L"Kendi malın için artırım yapamazsın...:)\n\r",ch);
	return;
		}
            /* make - perhaps - a bet now */
            if (argument[0] == '\0')
            {
              send_to_char ( L"Ne kadar teklif vereceksin?\n\r",ch);
                return;
            }

            newbet = parsebet (auction->bet, argument);
            swprintf (betbuf,MAX_STRING_LENGTH-1, L"Teklif: %d\n\r",newbet);

            if (newbet < (auction->bet + 1))
            {
              send_to_char ( L"Teklifin son tekliften 1 akçe fazla ya da açılış fiyatı kadar olmalı.\n\r",ch);
                return;
            }

            if (newbet > ch->silver)
            {
              send_to_char ( L"O kadar paran yok!\n\r",ch);
                return;
            }

            /* the actual bet is OK! */

            /* acik artirma devam ediyor. son yapilan teklifin
             * uzerine cikildi. son teklifi yapana parasini geri ver.
             */
            if (auction->buyer != NULL)
                auction->buyer->silver += auction->bet;

            ch->silver -= newbet; /* substract the silver - important :) */
            auction->buyer = ch;
            auction->bet   = newbet;
            auction->going = 0;
            auction->pulse = PULSE_AUCTION; /* start the auction over again */

            swprintf (buf,MAX_INPUT_LENGTH-1, L"%s için %d akçe teklif edildi.\n\r",auction->item->short_descr,newbet);
	    swprintf( bufc, MAX_STRING_LENGTH-1, L"%s%s%s",CLR_MAGENTA,buf,CLR_WHITE_BOLD);
            talk_auction (bufc);
            return;


        }
        else
        {
          send_to_char ( L"Mezatta birşey yok.\n\r",ch);
            return;
        }
        }

    /* finally... */

    obj = get_obj_carry (ch, arg1 ); /* does char have the item ? */

    if (obj == NULL)
    {
      send_to_char ( L"Onu taşımıyorsun.\n\r",ch);
        return;
    }

    if (obj->pIndexData->vnum < 100)
    {
      send_to_char ( L"Onu mezata süremezsin.\n\r",ch);
        return;
    }

    for (i=1; i < MAX_CABAL; i++)
    {
      if (obj->pIndexData->vnum == cabal_table[i].obj_vnum)
      {
        send_to_char( L"İsteğin tanrıları sinirlendiriyor.\n\r", ch);
	return;
      }
    }

    if (auction->item == NULL)
    switch (obj->item_type)
    {

    default:
    act_color ( L"$C$T eşyaları mezata sürülemez.$c",ch, NULL, item_type_name(obj),
		TO_CHAR,POS_SLEEPING,CLR_RED);
        return;

    case ITEM_WEAPON:
    case ITEM_ARMOR:
    case ITEM_STAFF:
    case ITEM_WAND:
    case ITEM_SCROLL:
        obj_from_char (obj);
        auction->item = obj;
        auction->bet = 0; 	/* obj->cost / 100 */
        auction->buyer = NULL;
        auction->seller = ch;
        auction->pulse = PULSE_AUCTION;
        auction->going = 0;

        swprintf( buf, MAX_STRING_LENGTH-1, L"%s mezatta.", obj->short_descr);
	swprintf( bufc, MAX_STRING_LENGTH-1, L"%s%s%s",CLR_YELLOW,buf,CLR_WHITE_BOLD);
        talk_auction(bufc);

        return;

    } /* switch */
    else
    {
      act ( L"Şu an mezatta $p eşyası var, daha sonra tekrar dene!",ch,auction->item,NULL,TO_CHAR);
        return;
    }
}
