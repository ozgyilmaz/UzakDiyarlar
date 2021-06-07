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
 **************************************************************************/

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
#include <unistd.h>

#include "merc.h"
#include "recycle.h"
#include "tables.h"
#include "interp.h"

int unlink(const wchar_t *pathname);

/* command procedures needed */
DECLARE_DO_FUN(do_quit	);
DECLARE_DO_FUN(do_quit_count);

bool quit_org	args( (CHAR_DATA *ch, wchar_t *argument, bool Count, bool Remort));
bool proper_order	args( (CHAR_DATA *ch, wchar_t *argument ) );
wchar_t *translate(CHAR_DATA *ch, CHAR_DATA *victim, wchar_t *argument);
int lang_lookup		args( (const wchar_t *name ) );
bool cabal_area_check	args( (CHAR_DATA *ch) );
void update_total_played	args( ( CHAR_DATA *ch) );


/* RT code to delete yourself */

void do_delet( CHAR_DATA *ch, wchar_t *argument)
{
  printf_to_char(ch,L"İntihar etmek için komutu eksiksiz yazmalısın.\n\r");
}

void do_delete( CHAR_DATA *ch, wchar_t *argument)
{
   wchar_t strsave[MAX_INPUT_LENGTH];

   if (IS_NPC(ch))
	return;

   if (ch->pcdata->confirm_delete)
   {
	if (argument[0] != '\0')
	{
    printf_to_char(ch,L"İntihardan vazgeçildi.\n\r");
	    ch->pcdata->confirm_delete = FALSE;
	    return;
	}
	else
	{
    	    swprintf( strsave, MAX_INPUT_LENGTH-1, L"%S%S", PLAYER_DIR, capitalize( ch->name ) );
          wiznet( L"$N intihar etti.",ch,NULL,0,0,0);
	    ch->last_fight_time = -1;
	    do_quit_count(ch,(wchar_t *)"");
	    unlink((char*)strsave);
	    return;
 	}
    }

    if (argument[0] != '\0')
    {
      printf_to_char(ch,L"Yalnızca {Rintihar{x yaz, parametreye gerek yok.\n\r");
	return;
    }

    printf_to_char(ch,L"Karakterini silmek için komutu tekrarlamalısın.\n\r");
    printf_to_char(ch,L"UYARI: Bu komutun geri dönüşü yoktur.\n\r");
    printf_to_char(ch,L"Komutu parametreyle kullanmak intihardan vazgeçildiği anlamına gelir.\n\r");
    ch->pcdata->confirm_delete = TRUE;
    wiznet( L"$N intihar etmek üzere.",ch,NULL,0,0,get_trust(ch));
}


/* RT code to display channel status */

void do_channels( CHAR_DATA *ch, wchar_t *argument)
{
    /* lists all channels and their status */
    printf_to_char(ch,L"kanal       durumu\n\r");
    printf_to_char(ch,L"------------------\n\r");

    printf_to_char(ch,L"{dmezat{x         ");
    if (!IS_SET(ch->comm,COMM_NOAUCTION))
    printf_to_char(ch,L"AÇIK\n\r");
    else
    printf_to_char(ch,L"KAPALI\n\r");

    if (IS_IMMORTAL(ch))
    {
      printf_to_char(ch,L"{iölümsüz kanalı{x    ");
      if(!IS_SET(ch->comm,COMM_NOWIZ))
      printf_to_char(ch,L"AÇIK\n\r");
      else
      printf_to_char(ch,L"KAPALI\n\r");
    }

    printf_to_char(ch,L"{kanlat{x          ");
    if (!IS_SET(ch->comm,COMM_DEAF))
    printf_to_char(ch,L"AÇIK\n\r");
    else
    printf_to_char(ch,L"KAPALI\n\r");

    printf_to_char(ch,L"{kkd{x          ");
    if (!IS_SET(ch->comm,COMM_NOKD))
    printf_to_char(ch,L"AÇIK\n\r");
    else
    printf_to_char(ch,L"KAPALI\n\r");

    printf_to_char(ch,L"{kkdg{x          ");
    if (!IS_SET(ch->comm,COMM_NOKDG))
    printf_to_char(ch,L"AÇIK\n\r");
    else
    printf_to_char(ch,L"KAPALI\n\r");

    printf_to_char(ch,L"{tsessiz modu{x     ");
    if (IS_SET(ch->comm,COMM_QUIET))
    printf_to_char(ch,L"AÇIK\n\r");
    else
    printf_to_char(ch,L"KAPALI\n\r");

    if (IS_SET(ch->comm,COMM_SNOOP_PROOF))
    printf_to_char(ch,L"Snoop'tan muafsın.\n\r");

    if (ch->lines != PAGELEN)
    {
	if (ch->lines)
	{
    printf_to_char(ch,L"%d satırlık skrol görüntülüyorsun.\n\r",ch->lines+2);
 	}
	else
  printf_to_char(ch,L"Skrol tamponlama kapalı.\n\r");
    }


    if (IS_SET(ch->comm,COMM_NOTELL))
    printf_to_char(ch,L"Anlatamazsın.\n\r");

    if (IS_SET(ch->comm,COMM_NOCHANNELS))
    printf_to_char(ch,L"Kanalları kullanamazsın.\n\r");

    if (IS_SET(ch->comm,COMM_NOEMOTE))
    printf_to_char(ch,L"Duygularını gösteremezsin.\n\r");

}

void garble(wchar_t *garbled,wchar_t *speech)
{
  int i;

  for (i = 0; speech[i] != (char) NULL; i++) {
    if (speech[i] >= 'a' && speech[i] <= 'z')
      garbled[i] = 'a' + number_range(0,25);
    else if (speech[i] >= 'A' && speech[i] <= 'Z')
      garbled[i] = 'A' + number_range(0,25);
    else garbled[i] = speech[i];
  }
  garbled[i] = '\0';
}


/* RT deaf blocks out all shouts */

void do_deaf( CHAR_DATA *ch, wchar_t *argument)
{

   if (IS_SET(ch->comm,COMM_DEAF))
   {
     printf_to_char(ch,L"Anlatılanları almaya başladın.\n\r");
     REMOVE_BIT(ch->comm,COMM_DEAF);
   }
   else
   {
     printf_to_char(ch,L"Şu andan sonra anlatılanları alamayacaksın.\n\r");
     SET_BIT(ch->comm,COMM_DEAF);
   }
}

/* RT quiet blocks out all communication */

void do_quiet ( CHAR_DATA *ch, wchar_t * argument)
{
    if (IS_SET(ch->comm,COMM_QUIET))
    {
      printf_to_char(ch,L"Sessiz moddan çıkıldı.\n\r");
      REMOVE_BIT(ch->comm,COMM_QUIET);
    }
   else
   {
     printf_to_char(ch,L"Bundan böyle yalnız söylenenleri ve duyguları algılayabileceksin.\n\r");
     SET_BIT(ch->comm,COMM_QUIET);
   }
}

void do_replay (CHAR_DATA *ch, wchar_t *argument)
{
    if (IS_NPC(ch))
    {
      printf_to_char(ch,L"Tekrarlayamazsın.\n\r");
	return;
    }

    page_to_char(buf_string(ch->pcdata->buffer),ch);
    clear_buf(ch->pcdata->buffer);
}

void do_immtalk( CHAR_DATA *ch, wchar_t *argument )
{
    DESCRIPTOR_DATA *d;

    if ( argument[0] == '\0' )
    {
      if (IS_SET(ch->comm,COMM_NOWIZ))
      {
        printf_to_char(ch,L"Ölümsüz kanalı açıldı.\n\r");
	REMOVE_BIT(ch->comm,COMM_NOWIZ);
      }
      else
      {
        printf_to_char(ch,L"Ölümsüz kanalı kapandı.\n\r");
	SET_BIT(ch->comm,COMM_NOWIZ);
      }
      return;
    }

    REMOVE_BIT(ch->comm,COMM_NOWIZ);

    write_channel_log(ch,NULL,KANAL_IMM,argument);

   if (!is_affected(ch, gsn_deafen))
   act_new(L"{m[$n]: $t{x",ch,argument,NULL,TO_CHAR,POS_DEAD);
    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d->connected == CON_PLAYING &&
	     IS_IMMORTAL(d->character) &&
             !IS_SET(d->character->comm,COMM_NOWIZ) )
	{
    act_new(L"{m[$n]: $t{x",ch,argument,d->character,TO_VICT,POS_DEAD);
	}
    }

    return;
}

void do_kd( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH],buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    if ( argument[0] == '\0' )
    {
      if (IS_SET(ch->comm,COMM_NOKD))
      {
        printf_to_char(ch,L"KD kanalı açıldı.\n\r");
	REMOVE_BIT(ch->comm,COMM_NOKD);
      }
      else
      {
        printf_to_char(ch,L"KD kanalı kapandı.\n\r");
	SET_BIT(ch->comm,COMM_NOKD);
      }
      return;
    }

	if ( IS_AFFECTED(ch, AFF_CHARM) &&   ch->master != NULL )
    {
		printf_to_char( ch , L"Teshirliyken kd kanalını kullanamazsın.\n\r" );
		return;
    }

    if (IS_SET(ch->comm,COMM_NOKD))
    {
	printf_to_char(ch,L"Önce KD kanalını açmalısın.\n\r");
	return;
    }

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
	printf_to_char(ch,L"Kime ne mesaj göndereceksin?\n\r" );
	return;
    }

    /*
     * Can tell to PC's anywhere, but NPC's only in same room.
     * -- Furey
     */
     if ( ( victim = get_char_world( ch, arg ) ) == NULL
     || ( IS_NPC(victim) && victim->in_room != ch->in_room ) )
    {
	printf_to_char( ch, L"Burada değil.\n\r" );
	return;
    }

	if( victim == ch )
	{
		printf_to_char( ch, L"Kendine KD mesajı atamazsın.\n\r" );
		return;
	}

    if ( victim->desc == NULL && !IS_NPC(victim))
    {
	act( L"$N bağlantısını kaybetmiş görünüyor...daha sonra tekrar dene.",
	    ch,NULL,victim,TO_CHAR);
        swprintf( buf, MAX_STRING_LENGTH-1,L"%S: %s%S%s\n\r",PERS(ch,victim),CLR_RED_BOLD,argument,CLR_NORMAL);
        buf[0] = towupper(buf[0]);
        add_buf(victim->pcdata->buffer,buf);
	return;
    }

    if ((IS_SET(victim->comm,COMM_NOKD)) && !IS_IMMORTAL(ch))
    {
	act( L"$N KD kanalını almıyor.", ch, 0, victim, TO_CHAR );
  	return;
    }

    write_channel_log(ch,victim,KANAL_KD,argument);

    if (is_affected(ch,gsn_garble))
      garble(buf,argument);
    else
      wcscpy(buf,argument);

    if (ch->level >= KIDEMLI_OYUNCU_SEVIYESI && victim->level >= KIDEMLI_OYUNCU_SEVIYESI )
    {
      ch->pcdata->rk_puani -= 1;
    }

   if (!is_affected(ch, gsn_deafen))
     act_color( L"$N kd: $C$t$c",ch,buf,victim,TO_CHAR,POS_DEAD, CLR_MAGENTA_BOLD );

   act_color( L"$n kd: $C$t$c",ch,buf,victim,TO_VICT,POS_DEAD, CLR_RED_BOLD );

    victim->reply	= ch;

    return;
}


void do_kdg( CHAR_DATA *ch, wchar_t *argument )
{
    DESCRIPTOR_DATA *d;
    wchar_t buf[MAX_INPUT_LENGTH];

	if( IS_NPC( ch ) )
	{
		return;
	}

    if ( argument[0] == '\0' )
    {
      if (IS_SET(ch->comm,COMM_NOKDG))
      {
        printf_to_char(ch,L"KDG kanalı açıldı.\n\r");
	REMOVE_BIT(ch->comm,COMM_NOKDG);
      }
      else
      {
        printf_to_char(ch,L"KDG kanalı kapandı.\n\r");
	SET_BIT(ch->comm,COMM_NOKDG);
      }
      return;
    }

	if ( IS_AFFECTED(ch, AFF_CHARM) &&   ch->master != NULL )
    {
		printf_to_char( ch , L"Teshirliyken kdg kanalını kullanamazsın.\n\r" );
		return;
    }

	if ( IS_SET(ch->comm, COMM_NOKDG) )
    {
	printf_to_char(ch,L"Önce kdg kanalını açmalısın.\n\r" );
	return;
    }

    if (argument[0] == '\0' )
    {
      send_to_char( L"Oyunculara ne gibi bi'şey söyleyeceksin?.\n\r",ch);
      return;
    }

    if (is_affected(ch,gsn_garble))
	{
      garble(buf,argument);
  	}
    else
	{
      wcscpy(buf,argument);
  	}

      ch->pcdata->rk_puani -= 4;


     act_color( L"$n kdg: $C$T$c", ch, NULL, buf, TO_CHAR,POS_DEAD, CLR_MAGENTA_BOLD );

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
		if( d->connected == CON_PLAYING )
		{
			if( d->character != ch )
			{
				if( !IS_SET( d->character->comm , COMM_NOKDG) )
				{
					printf_to_char ( d->character , L"%s kdg: {G%s{x\n\r" , ch->name , buf  );
				}
			}
		}
    }

    return;
}



void do_say( CHAR_DATA *ch, wchar_t *argument )
{
  CHAR_DATA *room_char;
  OBJ_DATA *char_obj;
  CHAR_DATA *vch;
  wchar_t buf[MAX_STRING_LENGTH];
  wchar_t trans[MAX_STRING_LENGTH];


    if ( argument[0] == '\0' )
    {
      printf_to_char(ch,L"Ne söyleyeceksin?\n\r");
	return;
    }
    if ( ch->in_room == NULL )  {
      printf_to_char(ch,L"Fakat bir odada değilsin!\n\r");
      return;
    }

    write_channel_log(ch,NULL,KANAL_SOYLE,argument);

    if (is_affected(ch,gsn_garble))
      garble(buf,(wchar_t*)argument);
    else
      wcscpy(buf,argument);

    for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
    {
        if (!is_affected(vch, gsn_deafen))
	{
	  swprintf(trans, MAX_STRING_LENGTH-1,L"%S",translate(ch,vch,buf));
    act_color( L"$C$n '{y$t{x' dedi.$c",ch, trans, vch, TO_VICT,POS_RESTING, CLR_GREEN);
	}
    }

   if (!is_affected(ch, gsn_deafen))
    {
    act_color( L"$CSen '$T' dedin.$c",ch, NULL, buf, TO_CHAR,POS_RESTING, CLR_GREEN );
    }

  for (room_char = ch->in_room->people; room_char != NULL;
         room_char = room_char->next_in_room)
    {
      if (IS_SET(room_char->progtypes,MPROG_SPEECH) && room_char != ch)
        (room_char->pIndexData->mprogs->speech_prog) (room_char,ch,buf);
    }

    for (char_obj = ch->carrying; char_obj != NULL;
       char_obj = char_obj->next_content)
    {
      if (IS_SET(char_obj->progtypes,OPROG_SPEECH))
        (char_obj->pIndexData->oprogs->speech_prog) (char_obj,ch,buf);
    }

    for (char_obj = ch->in_room->contents; char_obj != NULL;
       char_obj = char_obj->next_content)
    {
      if (IS_SET(char_obj->progtypes,OPROG_SPEECH))
        (char_obj->pIndexData->oprogs->speech_prog) (char_obj,ch,buf);
    }

    return;
}


void do_yell( CHAR_DATA *ch, wchar_t *argument )
{
    DESCRIPTOR_DATA *d;
    wchar_t buf[MAX_INPUT_LENGTH];
    wchar_t trans[MAX_STRING_LENGTH];


    if ( argument[0] == '\0' )
    {
      printf_to_char( ch, L"Ne haykıracaksın?\n\r" );
	return;
    }

    write_channel_log(ch,NULL,KANAL_HAYKIR,argument);

    if (is_affected(ch,gsn_garble))
      garble(buf,argument);
    else
      wcscpy(buf,argument);

   if (!is_affected(ch, gsn_deafen))
   act_color( L"Sen '$C$t$c' diye haykırdın.",
             ch,buf,NULL,TO_CHAR,POS_DEAD, CLR_BROWN );

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d->connected == CON_PLAYING
	&&   d->character != ch
	&&   d->character->in_room != NULL
	&&   d->character->in_room->area == ch->in_room->area
        &&   !is_affected(d->character, gsn_deafen))
	{
	    swprintf(trans, MAX_STRING_LENGTH-1,L"%S",translate(ch,d->character,buf));
      act_color( L"$n '$C$t$c' diye haykırdı.",
                ch,trans,d->character,TO_VICT,POS_DEAD, CLR_BROWN );
	}
    }

    return;
}


void do_emote( CHAR_DATA *ch, wchar_t *argument )
{
wchar_t buf[MAX_INPUT_LENGTH];

    if ( !IS_NPC(ch) && IS_SET(ch->comm, COMM_NOEMOTE) )
    {
      printf_to_char( ch, L"Duygularını belli edemezsin.\n\r" );
        return;
    }

    if ( argument[0] == '\0' )
    {
      printf_to_char(ch,L"Nasıl bir duygu vereceksin?\n\r" );
        return;
    }

    write_channel_log(ch,NULL,KANAL_DUYGU,argument);

    if (is_affected(ch,gsn_garble))
      garble(buf,(wchar_t*)argument);
    else
      wcscpy(buf,argument);

    act( L"$n $T", ch, NULL, buf, TO_ROOM );
    act( L"$n $T", ch, NULL, buf, TO_CHAR );
    return;
}


void do_pmote( CHAR_DATA *ch, wchar_t *argument )
{
    CHAR_DATA *vch;
    wchar_t *letter,*name;
    wchar_t last[MAX_INPUT_LENGTH], temp[MAX_STRING_LENGTH];
    size_t matches = 0;

    if ( !IS_NPC(ch) && IS_SET(ch->comm, COMM_NOEMOTE) )
    {
      printf_to_char( ch, L"Duygularını belli edemezsin.\n\r" );
        return;
    }

    if ( argument[0] == '\0' )
    {
      printf_to_char(ch,L"Nasıl bir duygu vereceksin?\n\r" );
        return;
    }

    act( L"$n $t", ch, argument, NULL, TO_CHAR );

    for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
    {
	if (vch->desc == NULL || vch == ch)
	    continue;

	if ((letter = wcsstr((wchar_t*)argument,vch->name)) == NULL)
	{
	    act( L"$N $t",vch,argument,ch,TO_CHAR);
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

	act( L"$N $t",vch,temp,ch,TO_CHAR);
    }

    return;
}


/*
 * All the posing stuff.
 */
struct	pose_table_type
{
    const wchar_t *	message[2*MAX_CLASS];
};

const	struct	pose_table_type	pose_table	[]	=
{
    {
	{
    L"Enerjiyle cızırdıyorsun.",
    L"$n enerjiyle cızırdıyor.",
    L"Kendini fevkalade kutsal hissediyorsun.",
    L"$n fevkalade kutsal görünüyor.",
    L"Birkaç küçük hünerini gösteriyorsun.",
    L"$n birkaç hünerini gösteriyor, vayy bee.",
    L"Şişmiş kaslarını gösteriyorsun.",
    L"$n şişkin kaslarını sergiliyor."
	}
    },

    {
	{
    L"Kısa süreliğine bir kelebeğe dönüşüyorsun.",
    L"$n kısa süreliğine bir kelebeğe dönüşüyor.",
    L"Canın sıkılınca şarabı suya dönüştürüyorsun.",
    L"$n can sıkıntısından şarabı suya dönüştürüyor.",
    L"Kulaklarını birbirinden bağımsız hareket ettiriyorsun.",
    L"$n tavşan gibi kulaklarını birbirinden bağımsız hareket ettiriyor.",
    L"Bir fındığı parmaklarınla kırıyorsun.",
    L"$n parmaklarının arasına koyduğu fındığı kırıyor."
	}
    },

    {
	{
    L"Parmaklarından mavi kıvılcımlar çıkıyor.",
    L"$s parmaklarından mavi kıvılcımlar çıkıyor.",
    L"Kafanın üstünde bir hale oluşuyor.",
    L"$s kafasının üstünde bir hale oluşuyor.",
    L"Kendi kendini bağlıyorsun.",
    L"$n kendisini bağlıyor. Garip biri.",
    L"Dişlerini gösteriyorsun.",
    L"$n dişlerini gösteriyor. Iııyyyy..."
	}
    },

    {
	{
    L"Gözlerinde küçük kırmızı ışıklar dans ediyor.",
    L"$s gözlerinde küçük kırmızı ışıklar dans ediyor.",
    L"Bilgece sözler telaffuz ediyorsun.",
    L"$n bilgece sözler telaffuz ediyor.",
    L"Kamalar ve elmalarla hokkabazlık yapıyorsun.",
    L"$n kama ve elmalarla hokkabazlık yapıyor.",
    L"Kafanı vurunca gözlerin fırıl fırıl dönüyor.",
    L"$n kafasını vuruyor ve gözleri fırıl fırıl dönmeye başlıyor."
	}
    },

    {
	{
	    L"A slimy green monster appears before you and bows.",
	    L"A slimy green monster appears before $n and bows.",
	    L"Deep in prayer, you levitate.",
	    L"Deep in prayer, $n levitates.",
	    L"You steal the underwear off every person in the room.",
	    L"Your underwear is gone!  $n stole it!",
	    L"Crunch, crunch -- you munch a bottle.",
	    L"Crunch, crunch -- $n munches a bottle."
	}
    },

    {
	{
	    L"You turn everybody into a little pink elephant.",
	    L"You are turned into a little pink elephant by $n.",
	    L"An angel consults you.",
	    L"An angel consults $n.",
	    L"The dice roll ... and you win again.",
	    L"The dice roll ... and $n wins again.",
	    L"... 98, 99, 100 ... you do pushups.",
	    L"... 98, 99, 100 ... $n does pushups."
	}
    },

    {
	{
	    L"A small ball of light dances on your fingertips.",
	    L"A small ball of light dances on $n's fingertips.",
	    L"Your body glows with an unearthly light.",
	    L"$n's body glows with an unearthly light.",
	    L"You count the money in everyone's pockets.",
	    L"Check your money, $n is counting it.",
	    L"Arnold Schwarzenegger admires your physique.",
	    L"Arnold Schwarzenegger admires $n's physique."
	}
    },

    {
	{
	    L"Smoke and fumes leak from your nostrils.",
	    L"Smoke and fumes leak from $n's nostrils.",
	    L"A spot light hits you.",
	    L"A spot light hits $n.",
	    L"You balance a pocket knife on your tongue.",
	    L"$n balances a pocket knife on your tongue.",
	    L"Watch your feet, you are juggling granite boulders.",
	    L"Watch your feet, $n is juggling granite boulders."
	}
    },

    {
	{
	    L"The light flickers as you rap in magical languages.",
	    L"The light flickers as $n raps in magical languages.",
	    L"Everyone levitates as you pray.",
	    L"You levitate as $n prays.",
	    L"You produce a coin from everyone's ear.",
	    L"$n produces a coin from your ear.",
	    L"Oomph!  You squeeze water out of a granite boulder.",
	    L"Oomph!  $n squeezes water out of a granite boulder."
	}
    },

    {
	{
	    L"Your head disappears.",
	    L"$n's head disappears.",
	    L"A cool breeze refreshes you.",
	    L"A cool breeze refreshes $n.",
	    L"You step behind your shadow.",
	    L"$n steps behind $s shadow.",
	    L"You pick your teeth with a spear.",
	    L"$n picks $s teeth with a spear."
	}
    },

    {
	{
	    L"A fire elemental singes your hair.",
	    L"A fire elemental singes $n's hair.",
	    L"The sun pierces through the clouds to illuminate you.",
	    L"The sun pierces through the clouds to illuminate $n.",
	    L"Your eyes dance with greed.",
	    L"$n's eyes dance with greed.",
	    L"Everyone is swept off their foot by your hug.",
	    L"You are swept off your feet by $n's hug."
	}
    },

    {
	{
	    L"The sky changes color to match your eyes.",
	    L"The sky changes color to match $n's eyes.",
	    L"The ocean parts before you.",
	    L"The ocean parts before $n.",
	    L"You deftly steal everyone's weapon.",
	    L"$n deftly steals your weapon.",
	    L"Your karate chop splits a tree.",
	    L"$n's karate chop splits a tree."
	}
    },

    {
	{
	    L"The stones dance to your command.",
	    L"The stones dance to $n's command.",
	    L"A thunder cloud kneels to you.",
	    L"A thunder cloud kneels to $n.",
	    L"The Grey Mouser buys you a beer.",
	    L"The Grey Mouser buys $n a beer.",
	    L"A strap of your armor breaks over your mighty thews.",
	    L"A strap of $n's armor breaks over $s mighty thews."
	}
    },

    {
	{
	    L"The heavens and grass change colour as you smile.",
	    L"The heavens and grass change colour as $n smiles.",
	    L"The Burning Man speaks to you.",
	    L"The Burning Man speaks to $n.",
	    L"Everyone's pocket explodes with your fireworks.",
	    L"Your pocket explodes with $n's fireworks.",
	    L"A boulder cracks at your frown.",
	    L"A boulder cracks at $n's frown."
	}
    },

    {
	{
    L"Bir süreliğine herkesin giysisi transparan oluyor. Ve sen herşeyi görüyorsun.",
    L"Giysilerin transparan oluyor. $n kahkahalar atıyor.",
    L"Piramitteki göz sana göz kırpıyor.",
    L"Piramitteki göz $e göz kırpıyor.",
    L"Birden herkes kamanı burunlarının ucunda görüyorlar.",
    L"$s kamasının burnunun ucunda durduğunu farkediyorsun. Ne kadar da hızlı...",
    L"Emrini yerine getirmeye paralı askerler geliyor.",
    L"Paralı askerler $s emrini yerine getirmek için geliyor."
	}
    },

    {
	{
	    L"A black hole swallows you.",
	    L"A black hole swallows $n.",
	    L"Valentine Michael Smith offers you a glass of water.",
	    L"Valentine Michael Smith offers $n a glass of water.",
	    L"Where did you go?",
	    L"Where did $n go?",
	    L"Four matched Percherons bring in your chariot.",
	    L"Four matched Percherons bring in $n's chariot."
	}
    },

    {
	{
    L"Dünya ıslığınla parıldıyor.",
    L"Dünya $s ıslığıyla parıldıyor.",
    L"Tanrılar sana bir asa veriyor.",
    L"Tanrılar $s bir asa veriyor.",
    L"Tıkırtı.",
    L"Tıkırtı.",
    L"Atlas kendisine yardım edip edemeyeceğini soruyor.",
    L"Atlas $z yükünü taşımada yardım istiyor."
	}
    }
};



void do_pose( CHAR_DATA *ch, wchar_t *argument )
{
    int level;
    int pose;

    if ( IS_NPC(ch) )
	return;

    level = UMIN( ch->level, (int)sizeof(pose_table) / (int)sizeof(pose_table[0]) - 1 );
    pose  = number_range(0, level);

    act( pose_table[pose].message[2*ch->iclass+0], ch, NULL, NULL, TO_CHAR );
    act( pose_table[pose].message[2*ch->iclass+1], ch, NULL, NULL, TO_ROOM );

    return;
}



void do_bug( CHAR_DATA *ch, wchar_t *argument )
{
    append_file( ch, (wchar_t*)BUG_FILE, argument );
    printf_to_char(ch,L"Bug logged.\n\r");
    return;
}

void do_typo( CHAR_DATA *ch, wchar_t *argument )
{
    append_file( ch, (wchar_t*)TYPO_FILE, argument );
    printf_to_char(ch,L"Typo logged.\n\r");
    return;
}

void do_rent( CHAR_DATA *ch, wchar_t *argument )
{
    printf_to_char(ch,L"There is no rent here.  Just save and quit.\n\r");
    return;
}


void do_qui( CHAR_DATA *ch, wchar_t *argument )
{
  printf_to_char( ch, L"Uzak Diyarlar'dan ayrılmak istiyorsan komutu eksiksiz yazmalısın.\n\r" );
    return;
}


void do_quit( CHAR_DATA *ch, wchar_t *argument )
{
   quit_org(ch, argument, FALSE, FALSE );
   return;
}

void do_quit_count( CHAR_DATA *ch, wchar_t *argument )
{
   quit_org(ch, argument, TRUE, FALSE );
   return;
}

void do_quit_remort( CHAR_DATA *ch, wchar_t *argument )
{
   quit_org(ch, argument, TRUE, TRUE );
   return;
}

bool quit_org( CHAR_DATA *ch, wchar_t *argument, bool Count , bool Remort)
{
    DESCRIPTOR_DATA *d, *dr, *d_next;
    CHAR_DATA *vch, *vch_next;
    OBJ_DATA *obj,*obj_next;
    int id;

    if ( IS_NPC(ch) )
	return FALSE;

    if ( ch->position == POS_FIGHTING )
    {
      printf_to_char(ch,L"İmkansız! Dövüşüyorsun.\n\r" );
	return FALSE;
    }

    if ( IS_AFFECTED(ch, AFF_SLEEP) )
    {
      printf_to_char( ch, L"Yatmaya devam et! Henüz uyanmadın.\n\r" );
	return FALSE;
    }

    if ( ch->position  < POS_STUNNED  )
    {
      printf_to_char( ch, L"Henüz ölmedin.\n\r" );
	return FALSE;
    }

    if ( ch->last_fight_time != -1 && !IS_IMMORTAL(ch) &&
        (current_time - ch->last_fight_time)<FIGHT_DELAY_TIME)
      {
        printf_to_char(ch,L"Adrenalin salgılarken ayrılamazsın! Biraz sakinleşmeyi dene.\n\r");
        return FALSE;
      }

    if ( IS_AFFECTED( ch, AFF_CHARM ) )
    {
      printf_to_char( ch, L"Efendini terketmek istemiyorsun.\n\r" );
	return FALSE;
    }

    if ( IS_SET( ch->act, PLR_NO_EXP ) )
    {
      printf_to_char( ch, L"Ruhunu kaybetmek istemiyorsun.\n\r" );
	return FALSE;
    }

    if ( auction->item != NULL && ((ch == auction->buyer) || (ch == auction->seller)) )
    {
      printf_to_char (ch, L"Mezat bitene kadar bekle.\n\r");
        return FALSE;
    }

    if ( !IS_IMMORTAL(ch) &&
		ch->in_room && IS_RAFFECTED( ch->in_room, AFF_ROOM_ESPIRIT ) )
    {
      printf_to_char( ch, L"Bölgenin şer güçleri ayrılmanı engelliyor.\n\r" );
	return FALSE;
    }

    if ( !IS_IMMORTAL(ch) &&
	ch->cabal != CABAL_INVADER && is_affected(ch, gsn_evil_spirit ))
    {
      printf_to_char(ch,L"İçindeki şer güçler ayrılmanı engelliyor.\n\r" );
	return FALSE;
    }

    if ( cabal_area_check(ch) )
    {
      printf_to_char(ch,L"Başka bir kabalın bölgesinde ayrılamazsın.\n\r" );
	return FALSE;
    }

    if ( !Remort )
    {
      printf_to_char(ch,L"Her güzel şeyin bir sonu vardır.\n\r");
      act_color( L"$C$n oyundan ayrıldı.$c", ch, NULL, NULL,TO_ROOM ,POS_DEAD,CLR_GREEN);
      swprintf( log_buf, (2*MAX_INPUT_LENGTH)-1, L"%S oyundan ayrıldı.", ch->name );
	log_string( log_buf );
  wiznet( L"$N oyundan ayrıldı.",ch,NULL,WIZ_LOGINS,0,get_trust(ch));    }

    update_total_played( ch );

    for ( obj = object_list; obj != NULL; obj = obj_next )
     {
	obj_next = obj->next;
	if (   obj->pIndexData->vnum == 84
	    || obj->pIndexData->vnum == 85
	    || obj->pIndexData->vnum == 86
	    || obj->pIndexData->vnum == 97 )
	{
	 if (obj->extra_descr == NULL) extract_obj(obj);
	 else if ( wcsstr( obj->extra_descr->description, ch->name) != NULL )
		 extract_obj( obj);
	}
     }

    for( obj = ch->carrying; obj ; obj = obj_next)
	{
	 obj_next = obj->next_content;
	 if (obj->pIndexData->vnum == OBJ_VNUM_MAGIC_JAR)
	   {
		 extract_obj( obj);
	   }
	if (   obj->pIndexData->vnum == 84
	    || obj->pIndexData->vnum == 85
	    || obj->pIndexData->vnum == 86
	    || obj->pIndexData->vnum == 97 )
	 {
	  if (obj->extra_descr == NULL) extract_obj(obj);
	  else if ( wcsstr( obj->extra_descr->description, ch->name) !=NULL )
		 extract_obj( obj);
	  else {
		obj_from_char(obj);
		obj_to_room(obj,ch->in_room);
	       }
	 }
	}

    for (vch=char_list; vch != NULL; vch = vch_next)
    {
      vch_next = vch->next;
      if (is_affected(vch,gsn_doppelganger) && vch->doppel == ch)
      {
        printf_to_char(vch, L"Kurbanın ayrılınca sen de gerçek formuna dönüyorsun.\n\r");
        affect_strip(vch,gsn_doppelganger);
      }

      if (vch->guarding == ch)
      {
        act( L"Sen $M korumayı bıraktın.", vch, NULL, ch, TO_CHAR);
        act( L"$n seni korumayı bıraktı.", vch, NULL, ch, TO_VICT);
        act( L"$n $M korumayı bıraktı.", vch, NULL, ch, TO_NOTVICT);
          vch->guarding  = NULL;
          ch->guarded_by = NULL;
      }

      if (vch->last_fought == ch)
      {
         vch->last_fought = NULL;
	 back_home( vch );
      }

      if (vch->hunting == ch)
	vch->hunting = NULL;

    }

    if (ch->guarded_by != NULL)
      {
        ch->guarded_by->guarding = NULL;
        ch->guarded_by = NULL;
      }


    /*
     * After extract_char the ch is no longer valid!
     */
    save_char_obj( ch );
    id = ch->id;
    dr = d = ch->desc;
    if ( Count  || get_total_played(ch) < MIN_TIME_LIMIT )
      extract_char( ch, TRUE );
    else
      extract_char_nocount( ch, TRUE );

    if ( d != NULL  && !Remort )
	close_socket( d );


    /* toast evil cheating bastards    */
    for (d = descriptor_list; d != NULL; d = d_next)
    {
	CHAR_DATA *tch;

	d_next = d->next;
	if (Remort && dr == d)	continue;
	tch = d->original ? d->original : d->character;
	if (tch && tch->id == id)
	{
	    extract_char_nocount(tch,TRUE);
	    close_socket(d);
	}
    }

    iNumPlayers--;

    return TRUE;
}



void do_save( CHAR_DATA *ch, wchar_t *argument )
{
    if ( IS_NPC(ch) )
	return;

    if (ch->level < 2 && !IS_SET(ch->act, PLR_REMORTED) )
	{
    printf_to_char(ch,L"Kaydetmek için seviyen en az 2 olmalı.\n\r");
	 return;
	}
    save_char_obj( ch );
    printf_to_char(ch,L"Kaydediyor. Unutma ki otomatik kayıt vardır.\n\r");
    WAIT_STATE(ch, PULSE_VIOLENCE);
    return;
}



void do_follow( CHAR_DATA *ch, wchar_t *argument )
{
/* RT changed to allow unlimited following and follow the NOFOLLOW rules */
    wchar_t arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
      printf_to_char(ch,L"Kimi takip edeceksin?\n\r" );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
      printf_to_char(ch,L"Burada değil.\n\r" );
	return;
    }

    if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master != NULL )
    {
      act( L"En iyisi $M takip etmek!", ch, NULL, ch->master, TO_CHAR );
	return;
    }

    if ( victim == ch )
    {
	if ( ch->master == NULL )
	{
    printf_to_char( ch, L"Zaten kendini izliyorsun.\n\r" );
	    return;
	}
	stop_follower(ch);
	return;
    }

    if (!IS_NPC(victim) && IS_SET(victim->act,PLR_NOFOLLOW) && !IS_IMMORTAL(ch))
    {
      act( L"$N takipçi istemiyor.\n\r",
                 ch,NULL,victim, TO_CHAR);
        return;
    }

    REMOVE_BIT(ch->act,PLR_NOFOLLOW);

    if ( ch->master != NULL )
	stop_follower( ch );

    add_follower( ch, victim );
    return;
}


void add_follower( CHAR_DATA *ch, CHAR_DATA *master )
{
    if ( ch->master != NULL )
    {
	bug( L"Add_follower: non-null master.", 0 );
	return;
    }

    ch->master        = master;
    ch->leader        = NULL;

    if ( can_see( master, ch ) )
    act_color( L"$C$n seni takip ediyor.$c", ch, NULL, master,
    TO_VICT,POS_RESTING,CLR_YELLOW );
    act_color( L"$CSen $M takip ediyorsun.$c",  ch, NULL, master,
    TO_CHAR,POS_RESTING,CLR_YELLOW );

    return;
}



void stop_follower( CHAR_DATA *ch )
{
    if ( ch->master == NULL )
    {
	bug( L"Stop_follower: null master.", 0 );
	return;
    }

    if ( IS_AFFECTED(ch, AFF_CHARM) )
    {
	REMOVE_BIT( ch->affected_by, AFF_CHARM );
	affect_strip( ch, gsn_charm_person );
    }

    if ( can_see( ch->master, ch ) && ch->in_room != NULL)
    {
      act_color( L"$C$n seni takip etmeyi bıraktı.$c",ch, NULL, ch->master,
      		TO_VICT,POS_RESTING,CLR_BLUE);
      act_color( L"$CSen $M takip etmeyi bıraktın.$c", ch, NULL, ch->master,
      		TO_CHAR,POS_RESTING,CLR_BLUE);
    }
    if (ch->master->pet == ch)
	ch->master->pet = NULL;

    ch->master = NULL;
    ch->leader = NULL;
    return;
}

/* nukes charmed monsters and pets */
void nuke_pets( CHAR_DATA *ch )
{
    CHAR_DATA *pet;

    if ((pet = ch->pet) != NULL)
    {
    	stop_follower(pet);
    	if (pet->in_room != NULL)
      act( L"$N yavaşça yok oluyor.",ch,NULL,pet,TO_NOTVICT);
    	extract_char_nocount(pet,TRUE);
    }
    ch->pet = NULL;

    return;
}



void die_follower( CHAR_DATA *ch )
{
    CHAR_DATA *fch;
    CHAR_DATA *fch_next;

    if ( ch->master != NULL )
    {
    	if (ch->master->pet == ch)
    	    ch->master->pet = NULL;
	stop_follower( ch );
    }

    ch->leader = NULL;

    for ( fch = char_list; fch != NULL; fch = fch_next )
    {
	fch_next = fch->next;
	if ( fch->master == ch )
	    stop_follower( fch );
	if ( fch->leader == ch )
	    fch->leader = fch;
    }

    return;
}


void do_order( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t buf[MAX_STRING_LENGTH];
    wchar_t arg[MAX_INPUT_LENGTH],arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *och;
    CHAR_DATA *och_next;
    bool found;
    bool fAll;

    argument = one_argument( argument, arg );
    one_argument(argument,arg2);

    if (!wcscasecmp(arg2,L"intihar"))
    {
      printf_to_char(ch,L"Unut bunu.\n\r");
        return;
    }

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
      printf_to_char(ch,L"Kime ne emredeceksin?\n\r" );
	return;
    }

    if ( IS_AFFECTED( ch, AFF_CHARM ) )
    {
      printf_to_char(ch,L"Emir vermekten ziyade almaktan hoşlanıyorsun.\n\r" );
	return;
    }

    if ( !wcscasecmp( arg,L"tümü" ))//ozi
    {
	fAll   = TRUE;
	victim = NULL;
    }
    else
    {
	fAll   = FALSE;
	if ( ( victim = get_char_room( ch, arg ) ) == NULL )
	{
    printf_to_char(ch,L"Burada değil.\n\r" );
	    return;
	}

	if ( victim == ch )
	{
    printf_to_char(ch,L"Tabii...emredersiniz!\n\r" );
	    return;
	}

	if (!IS_AFFECTED(victim, AFF_CHARM) || victim->master != ch
	||  (IS_IMMORTAL(victim) && victim->trust >= ch->trust))
	{
    printf_to_char( ch, L"Kendin yap!\n\r" );
	    return;
	}
    }

    found = FALSE;
    for ( och = ch->in_room->people; och != NULL; och = och_next )
    {
	och_next = och->next_in_room;

	if ( IS_AFFECTED(och, AFF_CHARM)
	&&   och->master == ch
	&& ( fAll || och == victim ) )
	{
	    found = TRUE;
	    if ( !proper_order( och, argument ) )
		continue;
    swprintf( buf, MAX_STRING_LENGTH-1, L"$n sana '%s' diye emrediyor, ve sen de yapıyorsun.", argument );
	    act( buf, ch, NULL, och, TO_VICT );
	    interpret( och, argument, TRUE );
	}
    }

    if ( found )
    {
	WAIT_STATE(ch,PULSE_VIOLENCE);
  printf_to_char(ch,L"Tamam.\n\r" );
    }
    else
    printf_to_char( ch, L"Seni takip eden birini göremiyorum.\n\r" );
    return;
}

bool proper_order( CHAR_DATA *ch, wchar_t *argument )
{
 wchar_t command[MAX_INPUT_LENGTH];
 bool found;
 int trust, cmd_num;
 DO_FUN *cmd;


  one_argument( argument, command );
  found = FALSE;

  trust = get_trust( ch );

  for ( cmd_num = 0; cmd_table[cmd_num].name[0] != '\0'; cmd_num++ )
  {
      if ( command[0] == cmd_table[cmd_num].name[0]
        &&   !str_prefix( command, cmd_table[cmd_num].name )
        &&   cmd_table[cmd_num].level <= trust )
      {
            found = TRUE;
            break;
      }
  }
  if ( !found ) return TRUE;
  cmd = cmd_table[cmd_num].do_fun;

  if ( !IS_NPC(ch) )
  {
     if (cmd == do_delete || cmd == do_remort || cmd == do_induct
	|| cmd == do_quest || cmd == do_practice || cmd == do_train)
	return FALSE;
     return TRUE;
  }

  if ( ((cmd == do_bash) || (cmd == do_dirt) || (cmd == do_kick)
	|| (cmd == do_murder) || (cmd == do_trip) )
	&& ch->fighting == NULL )
    return FALSE;

  if ( (cmd == do_assassinate) || (cmd == do_ambush) || (cmd == do_blackjack)
	|| (cmd == do_cleave) || (cmd == do_kill) || (cmd == do_murder)
	|| (cmd == do_recall) || (cmd == do_strangle) || (cmd == do_vtouch))
    return FALSE;

  if ( cmd == do_close || cmd == do_lock || cmd == do_open ||
       cmd == do_unlock  )
  {
    if ( race_table[RACE(ch)].pc_race ) return TRUE;
    if ( RACE(ch) != 34  &&		/* doll */
         RACE(ch) != 37  &&		/* goblin */
         RACE(ch) != 38  &&		/* hobgoblin */
         RACE(ch) != 39  &&		/* kobolt */
         RACE(ch) != 40  &&		/* lizard */
         RACE(ch) != 41  &&		/* modron */
         RACE(ch) != 42  )		/* orc */
      return FALSE;
    else
      return TRUE;
  }

  if ( (cmd == do_backstab) || (cmd == do_hide) || (cmd == do_pick)
	|| (cmd == do_sneak) )
     {
       if (IS_SET(ch->act, ACT_THIEF))
	    return TRUE;
       else return FALSE;
     }

  return TRUE;
}


void do_group( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	CHAR_DATA *gch;
	CHAR_DATA *leader;

	leader = (ch->leader != NULL) ? ch->leader : ch;
  printf_to_char( ch, L"%s liderliğindeki grup:\n\r", PERS(leader, ch));

	for ( gch = char_list; gch != NULL; gch = gch->next )
	{
	    if ( is_same_group( gch, ch ) )
	    {
		  printf_to_char( ch,
		  L"[%2d %s] %-16s %d/%d hp %d/%d mana %d/%d mv   %5d xp\n\r",
		    gch->level,
		    IS_NPC(gch) ? L"Mob" : class_table[gch->iclass].who_name,
		    capitalize( PERS(gch, ch) ),
		    gch->hit,   gch->max_hit,
		    gch->mana,  gch->max_mana,
		    gch->move,  gch->max_move,
		    gch->exp );
	    }
	}
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
      printf_to_char(ch, L"Burada değil.\n\r" );
	return;
    }

    if ( ch->master != NULL || ( ch->leader != NULL && ch->leader != ch ) )
    {
      printf_to_char( ch, L"Fakat sen başka birini takip ediyorsun!\n\r" );
	return;
    }

    if ( victim->master != ch && ch != victim )
    {
      act( L"$N seni takip etmiyor.", ch, NULL, victim, TO_CHAR );
	return;
    }

    if (IS_AFFECTED(victim,AFF_CHARM))
    {
      printf_to_char(ch,L"Tesirli mobları grubundan ayıramazsın.\n\r");
        return;
    }

    if (IS_AFFECTED(ch,AFF_CHARM))
    {
      act( L"Efendini öylesine seviyorsun ki, terketmeyi aklından bile geçirmezsin!",ch,NULL,victim,TO_VICT);
    	return;
    }


  if ( is_same_group( victim, ch ) && ch != victim )
    {
      if (ch->guarding == victim || victim->guarded_by == ch)
        {
          act( L"Sen $M korumayı bıraktın.", ch, NULL, victim, TO_CHAR);
          act( L"$n seni korumayı bıraktı.", ch, NULL, victim, TO_VICT);
          act( L"$n $M korumayı bıraktı.", ch, NULL, victim, TO_NOTVICT);
          victim->guarded_by = NULL;
          ch->guarding       = NULL;
        }

      victim->leader = NULL;
      act_color( L"$C$n $M grubundan ayırdı.$c",   ch, NULL, victim,
      		TO_NOTVICT,POS_SLEEPING,CLR_YELLOW );
      act_color( L"$C$n seni grubundan ayırdı.$c",  ch, NULL, victim,
      		TO_VICT,POS_SLEEPING,CLR_YELLOW);
      act_color( L"$CSen $M grubundan ayırdın.$c", ch, NULL, victim,
      		TO_CHAR,POS_SLEEPING,CLR_BLUE);

      if (victim->guarded_by != NULL &&
          !is_same_group(victim,victim->guarded_by))
        {
          act( L"Sen $M korumayı bıraktın.",victim->guarded_by,NULL,victim,TO_CHAR);
          act( L"$n seni korumayı bıraktı.",victim->guarded_by,NULL,victim,TO_VICT);
          act( L"$n $M korumayı bıraktı.",victim->guarded_by,NULL,victim,TO_NOTVICT);
          victim->guarded_by->guarding = NULL;
          victim->guarded_by           = NULL;
        }
      return;
    }

  if ( ch->level - victim->level < -8
      ||   ch->level - victim->level > 8 )
    {
      act_color( L"$C$N $s grubuna giremiyor.$c",ch, NULL, victim,
      		TO_NOTVICT,POS_SLEEPING,CLR_RED_BOLD );
      act_color( L"$CSen $s grubuna giremiyorsun.$c",ch, NULL, victim,
      		TO_VICT,POS_SLEEPING,CLR_RED_BOLD    );
      act_color( L"$C$N senin grubuna giremiyor.$c",ch, NULL, victim,
      		TO_CHAR ,POS_SLEEPING,CLR_RED_BOLD   );
      return;
    }

  if (IS_GOOD(ch) && IS_EVIL(victim))
    {
      act_color( L"$CSen $s grubu için fazla şer yanlısısın.$c", ch, NULL, victim,
      	TO_VICT,POS_SLEEPING,CLR_RED);
      act_color( L"$C$N grubun için fazla şer yanlısı!$c", ch, NULL, victim,
      	TO_CHAR,POS_SLEEPING,CLR_RED);
      ch->pcdata->rk_puani -= 1;
      return;
    }

  if (IS_GOOD(victim) && IS_EVIL(ch))
    {
      act_color( L"$CSen $s grubu için fazla masumsun!$c", ch, NULL, victim,
      	TO_VICT,POS_SLEEPING,CLR_RED);
      act_color( L"$C$N grubun için fazla masum!$c", ch, NULL, victim,
      	TO_CHAR,POS_SLEEPING,CLR_RED);
      ch->pcdata->rk_puani -= 1;
      return;
    }

  if ((ch->cabal == CABAL_RULER  && victim->cabal == CABAL_CHAOS) ||
	(ch->cabal == CABAL_CHAOS  && victim->cabal == CABAL_RULER) ||
	(ch->cabal == CABAL_KNIGHT  && victim->cabal == CABAL_INVADER) ||
	(ch->cabal == CABAL_INVADER  && victim->cabal == CABAL_KNIGHT) ||
	(ch->cabal == CABAL_SHALAFI  && victim->cabal == CABAL_BATTLE) ||
	(ch->cabal == CABAL_BATTLE  && victim->cabal == CABAL_SHALAFI) )
    {
      act_color( L"$CSen $s kabalından nefret ediyorsun, onun grubuna nasıl gireceksin?!$c",
      	ch,NULL, victim,TO_VICT,POS_SLEEPING,CLR_RED);
      act_color( L"$CSen $S kabalından nefret ediyorsun, onu grubuna nasıl alabilirsin?!$c",ch, NULL, victim, TO_CHAR,POS_SLEEPING,CLR_RED);
      ch->pcdata->rk_puani -= 20;
      return;
    }


  victim->leader = ch;
  act_color( L"$C$N $s grubuna giriyor.$c", ch, NULL, victim,TO_NOTVICT,
  	POS_SLEEPING,CLR_YELLOW );
  act_color( L"$CSen $s grubuna giriyorsun.$c", ch, NULL, victim,TO_VICT,
  	POS_SLEEPING,CLR_YELLOW    );
  act_color( L"$C$N senin grubuna giriyor.$c", ch, NULL, victim, TO_CHAR,
  	POS_SLEEPING,CLR_BLUE    );
  return;

}



/*
 * 'Split' originally by Gnort, God of Chaos.
 */
void do_split( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t buf[MAX_STRING_LENGTH];
    wchar_t arg1[MAX_INPUT_LENGTH];
    CHAR_DATA *gch;
    int members;
    int amount_silver = 0;
    int share_silver;
    int extra_silver;

    one_argument( argument, arg1 );

    if ( arg1[0] == '\0' )
    {
      printf_to_char(ch,L"Ne kadar dağıtacaksın?\n\r" );
	return;
    }

    amount_silver = wcstol( arg1, NULL, 10 );

    if ( amount_silver < 0)
    {
      printf_to_char(ch,L"Grubun bundan hoşlanmaz.\n\r" );
	return;
    }

    if ( amount_silver == 0 )
    {
      printf_to_char( ch, L"Sıfır sikke dağıttın ve kimse çakozlamadı.\n\r" );
	return;
    }

    if ( ch->silver < amount_silver)
    {
      printf_to_char(ch,L"Üzerinde bu kadar yok.\n\r" );
	return;
    }

    members = 0;
    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
	if ( is_same_group( gch, ch ) && !IS_AFFECTED(gch,AFF_CHARM))
	    members++;
    }

    if ( members < 2 )
    {
      printf_to_char( ch, L"Sende kalsın.\n\r" );
	return;
    }

    share_silver = amount_silver / members;
    extra_silver = amount_silver % members;

    if ( share_silver == 0 )
    {
      printf_to_char(ch,L"Pis cimri.\n\r" );
	return;
    }

    ch->silver	-= amount_silver;
    ch->silver	+= share_silver + extra_silver;

    if (share_silver > 0)
    {
	printf_to_char(ch,
    L"Sen %d akçe dağıttın. Senin payına %d akçe düştü.\n\r",
 	    amount_silver,share_silver + extra_silver);
    }

    swprintf( buf, MAX_STRING_LENGTH-1, L"$n %d akçe dağıttı. Senin payına %d akçe düştü.",amount_silver,share_silver);


    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
	if ( gch != ch && is_same_group(gch,ch) && !IS_AFFECTED(gch,AFF_CHARM))
	{
	    act( buf, ch, NULL, gch, TO_VICT );
	    gch->silver += share_silver;
	}
    }

    return;
}



void do_gtell( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t buf[MAX_STRING_LENGTH];
    CHAR_DATA *gch;
    int i;

    if ( argument[0] == '\0' )
    {
      printf_to_char(ch,L"Gruba ne anlatacaksın?\n\r" );
	return;
    }

    if ( IS_SET( ch->comm, COMM_NOTELL ) )
    {
      printf_to_char(ch,L"Mesajın ulaşmadı!\n\r" );
	return;
    }

    write_channel_log(ch,NULL,KANAL_GSOYLE,argument);

    if (is_affected(ch,gsn_garble))
      garble(buf,argument);
    else
      wcscpy(buf,argument);

    ch->pcdata->rk_puani -= 1;


    /*
     * Note use of printf_to_char, so gtell works on sleepers.
     */

    for ( i = 0, gch = char_list; gch != NULL; gch = gch->next )
    {
      if ( is_same_group( gch, ch ) && !is_affected(gch, gsn_deafen))
        {
          act_color( L"$C$n gruba '$t' dedi.$c",
                  ch,buf,gch,TO_VICT,POS_DEAD,CLR_MAGENTA);
          i++;
        }
    }

    if (i > 1 && !is_affected(ch, gsn_deafen))
    act_color( L"$CSen gruba '$t' dedin.$c",
              ch,buf,NULL,TO_CHAR,POS_DEAD,CLR_CYAN);
  else printf_to_char( ch, L"Kendi kendine konuşmayı bırak.");

    return;
}



/*
 * It is very important that this be an equivalence relation:
 * (1) A ~ A
 * (2) if A ~ B then B ~ A
 * (3) if A ~ B  and B ~ C, then A ~ C
 */
bool is_same_group_old( CHAR_DATA *ach, CHAR_DATA *bch )
{
    if ( ach == NULL || bch == NULL)
	return FALSE;

    if ( ach->leader != NULL ) ach = ach->leader;
    if ( bch->leader != NULL ) bch = bch->leader;
    return ach == bch;
}


/*
 * New is_same_group by chronos
 */
bool is_same_group( CHAR_DATA *ach, CHAR_DATA *bch )
{
 CHAR_DATA *ch, *vch, *ch_next, *vch_next;
 int count, vcount;

 count = vcount = 0;
 for(ch = ach; ch != NULL; ch = ch_next)
 {
   ch_next = ch->leader;
   for(vch = bch; vch != NULL; vch = vch_next)
   {
    vch_next = vch->leader;
    if (ch == vch) return TRUE;
    if (++vcount > 6) break;	/* cyclic loop! */
   }
   if (++count > 6) break;	/* cyclic loop! */
 }
 return FALSE;
}


void do_cb( CHAR_DATA *ch, wchar_t *argument )
{
    DESCRIPTOR_DATA *d;
    wchar_t buf[MAX_STRING_LENGTH];
    wchar_t buf2[MAX_INPUT_LENGTH];

    if ( !(ch->cabal) )
      {
        printf_to_char(ch,L"Bir kabala üye değilsin.\n\r");
	return;
      }

      swprintf( buf, MAX_STRING_LENGTH-1, L"[%s] $n: $C$t$c",cabal_table[ch->cabal].short_name);

    if (is_affected(ch,gsn_garble))
      garble(buf2,argument);
    else
      wcscpy(buf2,argument);

    if(!IS_NPC(ch))
      ch->pcdata->rk_puani -= 2;

   if (!is_affected(ch, gsn_deafen))
     act_color(buf, ch, argument, NULL, TO_CHAR,POS_DEAD, CLR_BROWN);

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d->connected == CON_PLAYING &&
	     (d->character->cabal == ch->cabal) &&
/*             !IS_SET(d->character->comm,COMM_NOCB) &&   */
	     !is_affected(d->character, gsn_deafen))
	{
	    act_color(buf,
		      ch,buf2,d->character,TO_VICT,POS_DEAD, CLR_BROWN );
	}
    }

    return;
}

void do_pray(CHAR_DATA *ch, wchar_t *argument)
{
    DESCRIPTOR_DATA *d;

    if (IS_SET(ch->comm,COMM_NOCHANNELS))
	{
    printf_to_char(ch,L"Tanrılar seni dinlemek istemiyorlar.");
	  return;
	}

  printf_to_char(ch,L"Yüzünü gökyüzüne dönüp kadimlerin yardımı için dua ediyorsun!\n\r");
  printf_to_char(ch,L"Bu bir duygu değil, ölümsüzlerin diyarına uzanan bir kıvılcım.\n\r");

    for (d = descriptor_list; d != NULL; d = d->next)
      {
	if (d->connected == CON_PLAYING && IS_IMMORTAL(d->character) &&
	    !IS_SET(d->character->comm,COMM_NOWIZ))
	  {
	    if (argument[0] == '\0')
	      {
          act_color( L"$C$n DUA ediyor: Oyunda hiç tanrı var mı?$c",
      			ch,argument,d->character,TO_VICT,POS_DEAD,CLR_CYAN);
	      }
	    else
	      {
          act_color( L"$C$n DUA ediyor: $t$c",
      			ch,argument,d->character,TO_VICT,POS_DEAD,CLR_CYAN);
	      }
	  }
      }
   return;
}

wchar_t char_lang_lookup( wchar_t c )
{
 int i;

 for(i=0; translation_table[i].common[0] != '\0'; i++)
  if (translation_table[i].common[0] == c)
	 return translation_table[i].language[0];
 return c;
}

/*
 * ch says
 * victim hears
 */
wchar_t *translate(CHAR_DATA *ch, CHAR_DATA *victim, wchar_t *argument)
{
  static wchar_t trans[MAX_STRING_LENGTH];
  wchar_t buf[MAX_STRING_LENGTH];
  wchar_t c;
  int i;

  if (*argument == '\0'
      || (ch == NULL) || (victim == NULL)
      || IS_NPC(ch) || IS_NPC(victim)
      || IS_IMMORTAL(ch) || IS_IMMORTAL(victim)
      || ch->language == LANG_COMMON
      || ch->language == race_table[ORG_RACE(victim)].language)
   {
    if (IS_IMMORTAL(victim))
	swprintf(trans, MAX_STRING_LENGTH-1, L"(%s) %s",language_table[ch->language].name,argument);
    else wcscpy(trans,argument);
    return trans;
   }

  for(i=0 ; *argument != '\0'; argument++,i++)
    {
      c = char_lang_lookup( *argument );
      buf[i] = c;
    }
  buf[i] = '\0';

  swprintf(trans, MAX_STRING_LENGTH-1, L"(%s) %s",language_table[ch->language].name,buf);
  return trans;
}


void do_speak( CHAR_DATA *ch, wchar_t *argument )
{
 wchar_t arg[MAX_INPUT_LENGTH];
 int language;

 if (IS_NPC(ch)) return;

 argument = one_argument(argument,arg);
 if (arg[0] == '\0')
     {
       printf_to_char(ch,L"Şu an %s dili konuşuyorsun.\n\r",
     			language_table[ch->language].name);
        printf_to_char(ch,L"You can speak :\n\r");
        printf_to_char(ch,L"       ortak, %s\n\r",
      		language_table[race_table[ORG_RACE(ch)].language].name);
        return;
     }

 language = lang_lookup(arg);

 if (language == -1)
     {
       printf_to_char( ch, L"Böyle bir dil duymadın.\n\r");
        return;
     }

 if (language >= MAX_LANGUAGE)
  ch->language = race_table[ORG_RACE(ch)].language;
 else ch->language = language;

 printf_to_char(ch,L"Artık %s dili konuşuyorsun.\n\r",language_table[ch->language].name);
}

/* Thanx zihni@karmi.emu.edu.tr for the code of do_judge */
void do_judge( CHAR_DATA *ch, wchar_t *argument )
{
  wchar_t arg[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;

  one_argument( argument, arg );

 if ( ch->cabal != CABAL_RULER )
    {
      printf_to_char( ch, L"Hı?\n\r" );
     return;
    }

  if (!cabal_ok(ch,gsn_judge)) return;

  if ( arg[0] == '\0' )
    {
      printf_to_char( ch, L"Kimi yargılayacaksın?\n\r" );
     return;
    }

  /* judge thru world */
  if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
      printf_to_char( ch, L"Burada değil.\n\r" );
      return;
    }


   if (IS_NPC(victim))
    {
      printf_to_char(ch,L"Bir mob olmaz.\n\r" );
      return;
    }

   if (IS_IMMORTAL(victim) && !IS_IMMORTAL(ch))
    {
      printf_to_char( ch, L"Ölümsüzleri yargılayabilecek gücü kendinde göremiyorsun.\n\r");
        return;
    }

    printf_to_char(ch,L"%s'in etiği %s ve yönelimi %s.\n\r",
	victim->name,
        victim->ethos == 1 ? "Tüze" :
        victim->ethos == 2 ? "Yansız" :
        victim->ethos == 3 ? "Kaos" : "bilinmiyor",
        IS_GOOD(victim) ? "İyi" :
        IS_EVIL(victim) ? "Kem" : "Yansız" );
    return;
}

void do_remor( CHAR_DATA *ch, wchar_t *argument )
{
  printf_to_char(ch,L"Yeniyaşam istiyorsan eksiksiz yaz.\n\r" );
    return;
}

void do_remort( CHAR_DATA *ch, wchar_t *argument )
{
    DESCRIPTOR_DATA *d;
    char remstr[MAX_INPUT_LENGTH];
    char mkstr[MAX_INPUT_LENGTH];
    wchar_t pbuf[MAX_STRING_LENGTH];
    wchar_t name[MAX_STRING_LENGTH];
    int banks, qp, silver, old_tra, old_pra, rkp;

    if ( IS_NPC(ch) || ( d = ch->desc ) == NULL )
        return;

    if ( ch->level != LEVEL_HERO)
    {
      printf_to_char( ch, L"Yeniyaşam için KAHRAMAN olmalısın.\n\r" );
        return;
    }

    if ( !IS_SET(ch->act, PLR_CANREMORT) && !IS_SET(ch->act, PLR_REMORTED))
    {
      printf_to_char( ch, L"Yeniyaşam için ölümsüzlerden izin almalısın.\n\r" );
        return;
    }

    if (argument[0] != '\0')
    {
	if (!ch->pcdata->confirm_remort)
  printf_to_char(ch,L"Sadece yeniyaşam yaz, argüman kullanma.\n\r");
ch->pcdata->confirm_remort = FALSE;
        return;
    }

    if ( ch->pcdata->confirm_remort )
    {
	SET_BIT(ch->act, PLR_REMORTED);
  printf_to_char(ch,L"\n\r...YENİYAŞAM...\n\r");
	printf_to_char(ch,L"Yeni ırk, sınıf ve zarlarla yeni bir karakter yaratacaksın.\n\r");
	printf_to_char(ch,L"Bu sırada muddan koparsan veya mud çökerse:\n\r");
	printf_to_char(ch,L"    AYNI İSİMLE YENİ BİR KARAKTER YARAT VE ÖLÜMSÜZLERE DURUMU BİLDİR.\n\r");
	printf_to_char(ch,L"Unutma ki, eski karaktere ait aşağıdaki özellikler aynen korunur:\n\r");
	printf_to_char(ch,L"        bankadakiler dahil tüm akçe\n\r");
	printf_to_char(ch,L"        pratik, eğitim seansları ve görev puanı\n\r");
	printf_to_char(ch,L"Yeni yaşamında 6 yüzük takabileceksin.\n\r");
	printf_to_char(ch,L"             Ve fazladan 10 eğitim seansın olacak.\n\r");

	swprintf( pbuf, MAX_STRING_LENGTH-1, L"%s", ch->pcdata->pwd );
	sprintf( remstr, "%s%s", PLAYER_DIR, capitalize( ch->name ) );
	sprintf( mkstr, "%s%s", REMORT_DIR, capitalize( ch->name ) );
	swprintf( name, MAX_STRING_LENGTH-1, L"%s", ch->name );
	d = ch->desc;
	banks	= ch->pcdata->bank_s;
	qp	= ch->pcdata->questpoints;
	silver	= ch->silver;
  old_tra = ch->train;
  old_pra = ch->practice;
  rkp = ch->pcdata->rk_puani;

	if (!quit_org(ch, argument, TRUE, TRUE ))	return;

	link( remstr, mkstr );
	unlink( remstr );

	load_char_obj( d, name );

	ch = d->character;
	ch->pcdata->pwd = str_dup( pbuf );
	d->connected = CON_REMORTING;

        /* give the remorting bonus */
	ch->pcdata->bank_s += banks;
	ch->silver	+= silver;
	ch->pcdata->questpoints	+= qp;
  ch->practice += old_pra;
	ch->train	+= (5 + old_tra);
  ch->pcdata->rk_puani += rkp;

  write_to_buffer( d, L"\n\r[Devam etmek için ENTER]\n\r",0);
	return;
    }

    printf_to_char(ch,L"Komutu onaylamak için tekrar yeniyaşam yaz.\n\r");
    printf_to_char(ch,L"UYARI: bu komutun geri dönüşü yoktur.\n\r");
    printf_to_char(ch,L"Yeniyaşam komutunu argümanla yazmak yeniyaşam durumunu iptal edecektir.\n\r");
    printf_to_char(ch,L"Unutma ki, eski karaktere ait aşağıdaki özellikler aynen korunur:\n\r");
  	printf_to_char(ch,L"        bankadakiler dahil tüm akçe\n\r");
  	printf_to_char(ch,L"        pratik, eğitim seansları ve görev puanı\n\r");
  	printf_to_char(ch,L"Yeni yaşamında 6 yüzük takabileceksin.\n\r");
  	printf_to_char(ch,L"             Ve fazladan 10 eğitim seansın olacak.\n\r");
    ch->pcdata->confirm_remort = TRUE;
    wiznet( L"$N yeniyaşam almak üzere.",ch,NULL,0,0,get_trust(ch));

}


bool cabal_area_check(CHAR_DATA *ch)
{
   if (ch->in_room == NULL || IS_IMMORTAL(ch))	return FALSE;

   if ( ch->cabal != CABAL_RULER &&
	!wcscasecmp(ch->in_room->area->name,L"Tüze Konağı"))
	return TRUE;
   else if ( ch->cabal != CABAL_INVADER &&
	!wcscasecmp(ch->in_room->area->name,L"İstila"))
	return TRUE;
   else if ( ch->cabal != CABAL_CHAOS &&
	!wcscasecmp(ch->in_room->area->name,L"Kaos"))
	return TRUE;
   else if ( ch->cabal != CABAL_SHALAFI &&
	!wcscasecmp(ch->in_room->area->name,L"Tılsım"))
	return TRUE;
   else if ( ch->cabal != CABAL_BATTLE &&
	!wcscasecmp(ch->in_room->area->name,L"Öfke"))
	return TRUE;
   else if ( ch->cabal != CABAL_KNIGHT &&
	!wcscasecmp(ch->in_room->area->name,L"Şövalye"))
	return TRUE;
   else if ( ch->cabal != CABAL_HUNTER &&
	!wcscasecmp(ch->in_room->area->name,L"Avcı"))
	return TRUE;
   else if ( ch->cabal != CABAL_LIONS &&
	!wcscasecmp(ch->in_room->area->name,L"Aslan"))
	return TRUE;
   else return FALSE;
}

bool is_at_cabal_area(CHAR_DATA *ch)
{
   if (ch->in_room == NULL || IS_IMMORTAL(ch))	return FALSE;

   if ( !wcscasecmp(ch->in_room->area->name,L"Tüze Konağı") ||
	!wcscasecmp(ch->in_room->area->name,L"İstila") ||
	!wcscasecmp(ch->in_room->area->name,L"Kaos") ||
	!wcscasecmp(ch->in_room->area->name,L"Tılsım") ||
	!wcscasecmp(ch->in_room->area->name,L"Öfke") ||
	!wcscasecmp(ch->in_room->area->name,L"Şövalye") ||
	!wcscasecmp(ch->in_room->area->name,L"Avcı") ||
	!wcscasecmp(ch->in_room->area->name,L"Aslan") )
	return TRUE;
   else return FALSE;
}

/*
 * Should be called only in quit_org and reboot_anatolia
 */
void update_total_played( CHAR_DATA *ch )
{
   int l;
   int today = parse_date( current_time );

   /* add the player's new time */
   for( l=0; l<MAX_TIME_LOG; l++)
   {
     if ( today == ch->pcdata->log_date[l] )
     {
	ch->pcdata->log_time[l] += parse_time_spec(current_time - ch->logon);
	ch->logon = current_time;
	return;
     }
   }

   /* so new day has started */
   for( l=MAX_TIME_LOG-1; l>0; l--)
	ch->pcdata->log_time[l] = ch->pcdata->log_time[l-1];

   ch->pcdata->log_time[0] = parse_time_spec(current_time - ch->logon);
   ch->logon = current_time;

}
