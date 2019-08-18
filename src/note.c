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
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "merc.h"
#include "recycle.h"
#include "tables.h"

/* globals from db.c for load_notes */
#if !defined(macintosh)
extern  int     _filbuf         args( (FILE *) );
#endif
extern FILE *                  fpArea;
extern char                    strArea[MAX_INPUT_LENGTH];

/* local procedures */
void load_thread(char *name, NOTE_DATA **list, int type, time_t free_time);
void parse_note(CHAR_DATA *ch, char *argument, int type);
bool hide_note(CHAR_DATA *ch, NOTE_DATA *pnote);

NOTE_DATA *note_list;
NOTE_DATA *idea_list;
NOTE_DATA *penalty_list;
NOTE_DATA *news_list;
NOTE_DATA *changes_list;

int count_spool(CHAR_DATA *ch, NOTE_DATA *spool)
{
    int count = 0;
    NOTE_DATA *pnote;

    for (pnote = spool; pnote != NULL; pnote = pnote->next)
	if (!hide_note(ch,pnote))
	    count++;

    return count;
}

void do_unread(CHAR_DATA *ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];
    int count;
    bool found = FALSE;

    if (IS_NPC(ch))
	return;

    if ((count = count_spool(ch,news_list)) > 0)
    {
	found = TRUE;
  sprintf(buf,"Okunmayý bekleyen %d HABER var.\n\r",count);
	send_to_char(buf,ch);
    }
    else send_to_char("Okunmayý bekleyen yeni HABER yok.\n\r",ch);

    if ((count = count_spool(ch,changes_list)) > 0)
    {
	found = TRUE;
  sprintf(buf,"Okunmayý bekleyen %d DEÐÝÞÝKLÝK var.\n\r",count);
        send_to_char(buf,ch);
    }
    else send_to_char("Okunmayý bekleyen yeni DEÐÝÞÝKLÝK yok.\n\r",ch);

    if ((count = count_spool(ch,note_list)) > 0)
    {
	found = TRUE;
  sprintf(buf,"Okunmayý bekleyen %d NOT var.\n\r",count);
	send_to_char(buf,ch);
    }
    else send_to_char("Okunmayý bekleyen yeni NOT yok.\n\r",ch);

    if ((count = count_spool(ch,idea_list)) > 0)
    {
	found = TRUE;
  sprintf(buf,"Okunmayý bekleyen %d FÝKÝR var.\n\r",count);
	send_to_char(buf,ch);
    }
    else send_to_char("Okunmayý bekleyen yeni FÝKÝR yok.\n\r",ch);

    if (IS_TRUSTED(ch,ANGEL) && (count = count_spool(ch,penalty_list)) > 0)
    {
	found = TRUE;
  sprintf(buf,"Okunmayý bekleyen %d CEZA bildirimi var",count);
	send_to_char(buf,ch);
    }

    if (!found && str_cmp( argument, "login" ) )
    send_to_char("Hiç notun yok.\n\r",ch);
}

void do_note(CHAR_DATA *ch,char *argument)
{
    parse_note(ch,argument,NOTE_NOTE);
}

void do_idea(CHAR_DATA *ch,char *argument)
{
    parse_note(ch,argument,NOTE_IDEA);
}

void do_penalty(CHAR_DATA *ch,char *argument)
{
    parse_note(ch,argument,NOTE_PENALTY);
}

void do_news(CHAR_DATA *ch,char *argument)
{
    parse_note(ch,argument,NOTE_NEWS);
}

void do_changes(CHAR_DATA *ch,char *argument)
{
    parse_note(ch,argument,NOTE_CHANGES);
}

void save_notes(int type)
{
    FILE *fp;
    const char *name;
    NOTE_DATA *pnote;

    switch (type)
    {
	default:
	    return;
	case NOTE_NOTE:
	    name = NOTE_FILE;
	    pnote = note_list;
	    break;
	case NOTE_IDEA:
	    name = IDEA_FILE;
	    pnote = idea_list;
	    break;
	case NOTE_PENALTY:
	    name = PENALTY_FILE;
	    pnote = penalty_list;
	    break;
	case NOTE_NEWS:
	    name = NEWS_FILE;
	    pnote = news_list;
	    break;
	case NOTE_CHANGES:
	    name = CHANGES_FILE;
	    pnote = changes_list;
	    break;
    }

    fclose( fpReserve );
    if ( ( fp = fopen( name, "w" ) ) == NULL )
    {
	perror( name );
    }
    else
    {
	for ( ; pnote != NULL; pnote = pnote->next )
	{
    fprintf( fp, "Gönd     %s~\n", pnote->sender);
    fprintf( fp, "Tarih    %s~\n", pnote->date);
    fprintf( fp, "Pul      %ld\n", pnote->date_stamp);
    fprintf( fp, "Kime     %s~\n", pnote->to_list);
    fprintf( fp, "Konu     %s~\n", pnote->subject);
    fprintf( fp, "Not\n%s~\n",   pnote->text);
	}
	fclose( fp );
	fpReserve = fopen( NULL_FILE, "r" );
   	return;
    }
}
void load_notes(void)
{
    load_thread((char*)NOTE_FILE,&note_list, NOTE_NOTE, 14*24*60*60);
    load_thread((char*)IDEA_FILE,&idea_list, NOTE_IDEA, 28*24*60*60);
    load_thread((char*)PENALTY_FILE,&penalty_list, NOTE_PENALTY, 0);
    load_thread((char*)NEWS_FILE,&news_list, NOTE_NEWS, 0);
    load_thread((char*)CHANGES_FILE,&changes_list,NOTE_CHANGES, 0);
}

void load_thread(char *name, NOTE_DATA **list, int type, time_t free_time)
{
    FILE *fp;
    NOTE_DATA *pnotelast;

    if ( ( fp = fopen( name, "r" ) ) == NULL )
	return;

    pnotelast = NULL;
    for ( ; ; )
    {
	NOTE_DATA *pnote;
	char letter;

	do
	{
	    letter = getc( fp );
            if ( feof(fp) )
            {
                fclose( fp );
                return;
            }
        }
        while ( isspace(letter) );
        ungetc( letter, fp );

        pnote           = (NOTE_DATA *)alloc_perm( sizeof(*pnote) );

        if ( str_cmp( fread_word( fp ), "gönd" ) )
            break;
        pnote->sender   = fread_string( fp );

        if ( str_cmp( fread_word( fp ), "tarih" ) )
            break;
        pnote->date     = fread_string( fp );

        if ( str_cmp( fread_word( fp ), "pul" ) )
            break;
        pnote->date_stamp = fread_number(fp);

        if ( str_cmp( fread_word( fp ), "kime" ) )
            break;
        pnote->to_list  = fread_string( fp );

        if ( str_cmp( fread_word( fp ), "konu" ) )
            break;
        pnote->subject  = fread_string( fp );

        if ( str_cmp( fread_word( fp ), "not" ) )
            break;
        pnote->text     = fread_string( fp );

        if (free_time && pnote->date_stamp < current_time - free_time)
        {
	    free_note(pnote);
            continue;
        }

	pnote->type = type;

        if (*list == NULL)
            *list           = pnote;
        else
            pnotelast->next     = pnote;

        pnotelast       = pnote;
    }

    strcpy( strArea, NOTE_FILE );
    fpArea = fp;
    bug( "Load_notes: bad key word.", 0 );
    exit( 1 );
    return;
}

void append_note(NOTE_DATA *pnote)
{
    FILE *fp;
    const char *name;
    NOTE_DATA **list;
    NOTE_DATA *last;

    switch(pnote->type)
    {
	default:
	    return;
	case NOTE_NOTE:
	    name = NOTE_FILE;
	    list = &note_list;
	    break;
	case NOTE_IDEA:
	    name = IDEA_FILE;
	    list = &idea_list;
	    break;
	case NOTE_PENALTY:
	    name = PENALTY_FILE;
	    list = &penalty_list;
	    break;
	case NOTE_NEWS:
	     name = NEWS_FILE;
	     list = &news_list;
	     break;
	case NOTE_CHANGES:
	     name = CHANGES_FILE;
	     list = &changes_list;
	     break;
    }

    if (*list == NULL)
	*list = pnote;
    else
    {
	for ( last = *list; last->next != NULL; last = last->next);
	last->next = pnote;
    }

    fclose(fpReserve);
    if ( ( fp = fopen(name, "a" ) ) == NULL )
    {
        perror(name);
    }
    else
    {
      fprintf( fp, "Gönd    %s~\n", pnote->sender);
      fprintf( fp, "Tarih   %s~\n", pnote->date);
      fprintf( fp, "Pul     %ld\n", pnote->date_stamp);
      fprintf( fp, "Kime    %s~\n", pnote->to_list);
      fprintf( fp, "Konu    %s~\n", pnote->subject);
      fprintf( fp, "Not\n%s~\n", pnote->text);
        fclose( fp );
    }
    fpReserve = fopen( NULL_FILE, "r" );
}

bool is_note_to( CHAR_DATA *ch, NOTE_DATA *pnote )
{
    if ( !str_cmp( ch->name, pnote->sender ) )
	return TRUE;

    if ( !str_cmp( "tümü", pnote->to_list ) )
	return TRUE;

    if ( IS_IMMORTAL(ch) && is_name( (char*)"ölümsüz", pnote->to_list ) )
	return TRUE;

    if ( is_name( ch->name, pnote->to_list ) )
	return TRUE;

    if ( is_name( (char*)cabal_table[ch->cabal].short_name, pnote->to_list ) )
	return TRUE;

    return FALSE;
}



void note_attach( CHAR_DATA *ch, int type )
{
    NOTE_DATA *pnote;

    if ( ch->pnote != NULL )
	return;

    pnote = new_note();

    pnote->next		= NULL;
    pnote->sender	= str_dup( ch->name );
    pnote->date		= str_dup( "" );
    pnote->to_list	= str_dup( "" );
    pnote->subject	= str_dup( "" );
    pnote->text		= str_dup( "" );
    pnote->type		= type;
    ch->pnote		= pnote;
    return;
}



void note_remove( CHAR_DATA *ch, NOTE_DATA *pnote, bool idelete)
{
    char to_new[MAX_INPUT_LENGTH];
    char to_one[MAX_INPUT_LENGTH];
    NOTE_DATA *prev;
    NOTE_DATA **list;
    char *to_list;

    if (!idelete)
    {
	/* make a new list */
        to_new[0]	= '\0';
        to_list	= pnote->to_list;
        while ( *to_list != '\0' )
        {
    	    to_list	= one_argument( to_list, to_one );
    	    if ( to_one[0] != '\0' && str_cmp( ch->name, to_one ) )
	    {
	        strcat( to_new, " " );
	        strcat( to_new, to_one );
	    }
        }
        /* Just a simple recipient removal? */
       if ( str_cmp( ch->name, pnote->sender ) && to_new[0] != '\0' )
       {
	   free_string( pnote->to_list );
	   pnote->to_list = str_dup( to_new + 1 );
	   return;
       }
    }
    /* nuke the whole note */

    switch(pnote->type)
    {
	default:
	    return;
	case NOTE_NOTE:
	    list = &note_list;
	    break;
	case NOTE_IDEA:
	    list = &idea_list;
	    break;
	case NOTE_PENALTY:
	    list = &penalty_list;
	    break;
	case NOTE_NEWS:
	    list = &news_list;
	    break;
	case NOTE_CHANGES:
	    list = &changes_list;
	    break;
    }

    /*
     * Remove note from linked list.
     */
    if ( pnote == *list )
    {
	*list = pnote->next;
    }
    else
    {
	for ( prev = *list; prev != NULL; prev = prev->next )
	{
	    if ( prev->next == pnote )
		break;
	}

	if ( prev == NULL )
	{
	    bug( "Note_remove: pnote not found.", 0 );
	    return;
	}

	prev->next = pnote->next;
    }

    save_notes(pnote->type);
    free_note(pnote);
    return;
}

bool hide_note (CHAR_DATA *ch, NOTE_DATA *pnote)
{
    time_t last_read;

    if (IS_NPC(ch))
	return TRUE;

    switch (pnote->type)
    {
	default:
	    return TRUE;
	case NOTE_NOTE:
	    last_read = ch->pcdata->last_note;
	    break;
	case NOTE_IDEA:
	    last_read = ch->pcdata->last_idea;
	    break;
	case NOTE_PENALTY:
	    last_read = ch->pcdata->last_penalty;
	    break;
	case NOTE_NEWS:
	    last_read = ch->pcdata->last_news;
	    break;
	case NOTE_CHANGES:
	    last_read = ch->pcdata->last_changes;
	    break;
    }

    if (pnote->date_stamp <= last_read)
	return TRUE;

    if (!str_cmp(ch->name,pnote->sender))
	return TRUE;

    if (!is_note_to(ch,pnote))
	return TRUE;

    return FALSE;
}

void update_read(CHAR_DATA *ch, NOTE_DATA *pnote)
{
    time_t stamp;

    if (IS_NPC(ch))
	return;

    stamp = pnote->date_stamp;

    switch (pnote->type)
    {
        default:
            return;
        case NOTE_NOTE:
	    ch->pcdata->last_note = UMAX(ch->pcdata->last_note,stamp);
            break;
        case NOTE_IDEA:
	    ch->pcdata->last_idea = UMAX(ch->pcdata->last_idea,stamp);
            break;
        case NOTE_PENALTY:
	    ch->pcdata->last_penalty = UMAX(ch->pcdata->last_penalty,stamp);
            break;
        case NOTE_NEWS:
	    ch->pcdata->last_news = UMAX(ch->pcdata->last_news,stamp);
            break;
        case NOTE_CHANGES:
	    ch->pcdata->last_changes = UMAX(ch->pcdata->last_changes,stamp);
            break;
    }
}

void parse_note( CHAR_DATA *ch, char *argument, int type )
{
    BUFFER *buffer;
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    NOTE_DATA *pnote;
    NOTE_DATA **list;
    const char *list_name;
    int vnum;
    int anum;

    if ( IS_NPC(ch) )
	return;

    switch(type)
    {
	default:
	    return;
        case NOTE_NOTE:
            list = &note_list;
	    list_name = "notlar";
            break;
        case NOTE_IDEA:
            list = &idea_list;
	    list_name = "fikir";
            break;
        case NOTE_PENALTY:
            list = &penalty_list;
	    list_name = "ceza";
            break;
        case NOTE_NEWS:
            list = &news_list;
	    list_name = "haber";
            break;
        case NOTE_CHANGES:
            list = &changes_list;
	    list_name = "deðiþiklik";
            break;
    }

    argument = one_argument( argument, arg );
    smash_tilde( argument );

    if ( arg[0] == '\0' || !str_prefix( arg, "oku" ) )
    {
        bool fAll;

        if ( !str_cmp( argument, "tümü" ) )
        {
            fAll = TRUE;
            anum = 0;
        }

        else if ( argument[0] == '\0' || !str_prefix(argument, "sonraki"))
        /* read next unread note */
        {
            vnum = 0;
            for ( pnote = *list; pnote != NULL; pnote = pnote->next)
            {
                if (!hide_note(ch,pnote))
                {
                    sprintf( buf, "[%3d] %s: %s\n\r%s\n\rTo: %s\n\r",
                        vnum,
                        pnote->sender,
                        pnote->subject,
                        pnote->date,
                        pnote->to_list);
                    send_to_char( buf, ch );
                    page_to_char( pnote->text, ch );
                    update_read(ch,pnote);
                    return;
                }
                else if (is_note_to(ch,pnote))
                    vnum++;
            }
            sprintf(buf,"Senin %s bölümünde okunmamýþ notun yok.\n\r",list_name);
	    send_to_char(buf,ch);
            return;
        }

        else if ( is_number( argument ) )
        {
            fAll = FALSE;
            anum = atoi( argument );
        }
        else
        {
          send_to_char( "Kaç numaralý notu okuyacaksýn?\n\r", ch );
            return;
        }

        vnum = 0;
        for ( pnote = *list; pnote != NULL; pnote = pnote->next )
        {
            if ( is_note_to( ch, pnote ) && ( vnum++ == anum || fAll ) )
            {
                sprintf( buf, "[%3d] %s: %s\n\r%s\n\rTo: %s\n\r",
                    vnum - 1,
                    pnote->sender,
                    pnote->subject,
                    pnote->date,
                    pnote->to_list
                    );
                send_to_char( buf, ch );
                page_to_char( pnote->text, ch );
		update_read(ch,pnote);
                return;
            }
        }

        sprintf(buf,"Bu kadar %s yok.\n\r",list_name);
	send_to_char(buf,ch);
        return;
    }

    if ( !str_prefix( arg, "liste" ) )
    {
	vnum = 0;
	for ( pnote = *list; pnote != NULL; pnote = pnote->next )
	{
	    if ( is_note_to( ch, pnote ) )
	    {
		sprintf( buf, "[%3d%s] %s: %s\n\r",
		    vnum, hide_note(ch,pnote) ? " " : "N",
		    pnote->sender, pnote->subject );
		send_to_char( buf, ch );
		vnum++;
	    }
	}
	return;
    }

    if ( !str_prefix( arg, "kaldýr" ) )
    {
        if ( !is_number( argument ) )
        {
          send_to_char( "Kaç numaralý notu kaldýracaksýn?\n\r", ch );
            return;
        }

        anum = atoi( argument );
        vnum = 0;
        for ( pnote = *list; pnote != NULL; pnote = pnote->next )
        {
            if ( is_note_to( ch, pnote ) && vnum++ == anum )
            {
                note_remove( ch, pnote, FALSE );
                send_to_char( "Tamam.\n\r", ch );
                return;
            }
        }

        sprintf(buf,"Bu kadar %s yok.",list_name);
	send_to_char(buf,ch);
        return;
    }

    if ( !str_prefix( arg, "sil" ) && get_trust(ch) >= MAX_LEVEL - 1)
    {
        if ( !is_number( argument ) )
        {
          send_to_char( "Hangi numaradakini sileyim?\n\r", ch );
            return;
        }

        anum = atoi( argument );
        vnum = 0;
        for ( pnote = *list; pnote != NULL; pnote = pnote->next )
        {
            if ( is_note_to( ch, pnote ) && vnum++ == anum )
            {
                note_remove( ch, pnote,TRUE );
                send_to_char( "Tamam.\n\r", ch );
                return;
            }
        }

        sprintf(buf,"Bu kadar %s yok.",list_name);
	send_to_char(buf,ch);
        return;
    }

    if (!str_prefix(arg,"catchup"))
    {
	switch(type)
	{
	    case NOTE_NOTE:
		ch->pcdata->last_note = current_time;
		break;
	    case NOTE_IDEA:
		ch->pcdata->last_idea = current_time;
		break;
	    case NOTE_PENALTY:
		ch->pcdata->last_penalty = current_time;
		break;
	    case NOTE_NEWS:
		ch->pcdata->last_news = current_time;
		break;
	    case NOTE_CHANGES:
		ch->pcdata->last_changes = current_time;
		break;
	}
	return;
    }

    /* below this point only certain people can edit notes */
    if ((type == NOTE_NEWS && !IS_TRUSTED(ch,ANGEL))
    ||  (type == NOTE_CHANGES && !IS_TRUSTED(ch,CREATOR)))
    {
      sprintf(buf,"Senin %s yazmak için yeterli seviyen yok.",list_name);
	return;
    }

    if ( !str_cmp( arg, "+" ) )
    {
	note_attach( ch,type );
	if (ch->pnote->type != type)
	{
    send_to_char("Sen baþka bir not yazmaya baþlamýþsýn.\n\r",ch);
	    return;
	}
 	buffer = new_buf();

	if (strlen(ch->pnote->text)+strlen(argument) >= 4096)
	{
    send_to_char( "Not çok uzun.\n\r", ch );
	    return;
	}

	add_buf(buffer,ch->pnote->text);
	add_buf(buffer,argument);
	add_buf(buffer,(char*)"\n\r");
	free_string( ch->pnote->text );
	ch->pnote->text = str_dup( buf_string(buffer) );
	free_buf(buffer);
	send_to_char( "Tamam.\n\r", ch );
	return;
    }

    if (!str_cmp(arg,"-"))
    {
 	int len;
	bool found = FALSE;

	note_attach(ch,type);
        if (ch->pnote->type != type)
        {
          send_to_char("Zaten baþka bir nota baþlamýþsýn.\n\r",ch);
            return;
        }

	if (ch->pnote->text == NULL || ch->pnote->text[0] == '\0')
	{
    send_to_char("Silmek için satýr kalmadý.\n\r",ch);
	    return;
	}

	strcpy(buf,ch->pnote->text);

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
		    free_string(ch->pnote->text);
		    ch->pnote->text = str_dup(buf);
		    return;
		}
	    }
	}
	buf[0] = '\0';
	free_string(ch->pnote->text);
	ch->pnote->text = str_dup(buf);
	return;
    }

    if ( !str_prefix( arg, "konu" ) )
    {
	note_attach( ch,type );
        if (ch->pnote->type != type)
        {
          send_to_char("Zaten baþka bir notla meþgülsün.\n\r",ch);
            return;
        }

	free_string( ch->pnote->subject );
	ch->pnote->subject = str_dup( argument );
  send_to_char( "Tamam.\n\r", ch );
	return;
    }

    if ( !str_prefix( arg, "kime" ) )
    {
	if (is_name(argument,(char*)"tümü") &&
		!(IS_IMMORTAL(ch) || IS_SET(ch->act,PLR_CANINDUCT)) )
	{
    send_to_char("Sadece ölümsüzler ve klan liderleri herkese not atabilir.\n\r",ch);
            return;
	}
	note_attach( ch,type );
        if (ch->pnote->type != type)
        {
          send_to_char("Zaten baþka bir nota baþlamýþsýn.\n\r",ch);
            return;
        }
	free_string( ch->pnote->to_list );
	ch->pnote->to_list = str_dup( argument );
  send_to_char( "Tamam.\n\r", ch );
	return;
    }

    if ( !str_prefix( arg, "temizle" ) )
    {
	if ( ch->pnote != NULL )
	{
	    free_note(ch->pnote);
	    ch->pnote = NULL;
	}

  send_to_char( "Tamam.\n\r", ch );
	return;
    }

    if ( !str_prefix( arg, "göster" ) )
    {
	if ( ch->pnote == NULL )
	{
    send_to_char( "Gösterilecek notun yok.\n\r", ch );
	    return;
	}

	if (ch->pnote->type != type)
	{
    send_to_char("Sen bu türden bir notla uðraþmýyorsun.\n\r",ch);
	    return;
	}

	sprintf( buf, "%s: %s\n\rKime: %s\n\r",
	    ch->pnote->sender,
	    ch->pnote->subject,
	    ch->pnote->to_list
	    );
	send_to_char( buf, ch );
	send_to_char( ch->pnote->text, ch );
	return;
    }

    if ( !str_prefix( arg, "gönder" ) || !str_prefix(arg, "yolla"))
    {
	char *strtime;

	if ( ch->pnote == NULL )
	{
    send_to_char( "Uðraþtýðýn bir notun yok.\n\r", ch );
	    return;
	}

        if (ch->pnote->type != type)
        {
          send_to_char("Bu türden bir notla çalýþmýyorsun.\n\r",ch);
            return;
        }

	if (!str_cmp(ch->pnote->to_list,""))
	{
    send_to_char("Kime göndereceðini belirtmelisin (isim, herkes, veya ölümsüz).\n\r",
		ch);
	    return;
	}

	if (!str_cmp(ch->pnote->subject,""))
	{
    send_to_char("Bir konu belirtmen gerek.\n\r",ch);
	    return;
	}

	ch->pnote->next			= NULL;
	strtime				= ctime( &current_time );
	strtime[strlen(strtime)-1]	= '\0';
	ch->pnote->date			= str_dup( strtime );
	ch->pnote->date_stamp		= current_time;

	append_note(ch->pnote);
	ch->pnote = NULL;
	return;
    }

    send_to_char( "Bunu yapamazsýn.\n\r", ch );
    return;
}
