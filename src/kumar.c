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
#include <time.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#endif
#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "merc.h"
#include "interp.h"


void do_zar (CHAR_DATA * ch, wchar_t *argument)
{
	wchar_t arg1[MAX_INPUT_LENGTH];
	wchar_t arg2[MAX_INPUT_LENGTH];
	int zar_tipi, ch_zar, mekanci_zar;
	long yatirilan,kazanc;

	if (!IS_SET(ch->in_room->room_flags, ROOM_KUMAR))
    {
      printf_to_char (ch, L"Kumarhanede değilsin.\n\r");
      return;
    }
	if (ch->silver < 5)
    {
        printf_to_char (ch, L"Zar oynamak için en az 5 akçen ihtiyacın var.\n\r");
        return;
    }

	argument = one_argument (argument, arg1);
	argument = one_argument (argument, arg2);

	if( arg1[0]=='\0' )
	{
		printf_to_char (ch, L"Hangi zardan ne kadar oynayacaksın? Mevcutları görmek için {Rzar liste{x yaz.\n\r");
        return;
	}
	if(!wcscasecmp(arg1, L"liste"))
	{
		printf_to_char (ch, L"Zar türleri:\n\r");
		printf_to_char (ch, L"Zar Tipi  Açıklama\n\r");
		printf_to_char (ch, L"--------  ------------\n\r");
		printf_to_char (ch, L"altı      Altılık zar\n\r");
		printf_to_char (ch, L"on        Onluk zar\n\r");
		printf_to_char (ch, L"yirmi     Yirmilik zar\n\r");
		printf_to_char (ch, L"yüz       Yüzlük zar\n\r\n\r");
		printf_to_char (ch, L"Oynamak için {Gzar <zar_tipi> <yatırılan_akçe>\n\r");
		return;
	}
	if( arg2[0]=='\0' )
	{
		printf_to_char (ch, L"Hangi zardan ne kadar oynayacaksın? Mevcutları görmek için {Rzar liste{x yaz.\n\r");
        return;
	}
	zar_tipi=0;
	if(!wcscasecmp(arg1, L"altı"))
		zar_tipi=6;
	else if(!wcscasecmp(arg1, L"on"))
		zar_tipi=10;
	else if(!wcscasecmp(arg1, L"yirmi"))
		zar_tipi=20;
	else if(!wcscasecmp(arg1, L"yüz"))
		zar_tipi=100;
	else
	{
		printf_to_char (ch, L"Hangi zardan ne kadar oynayacaksın? Mevcutları görmek için {Rzar liste{x yaz.\n\r");
        return;
	}
	yatirilan=advatoi (arg2);
	if(ch->silver<yatirilan)
	{
		send_to_char ( L"Bu oyun lafla değil akçeyle oynanır!\n\r", ch);
        	return;
	}
	if( (yatirilan>50000)||(yatirilan<5) )
	{
		send_to_char ( L"Zar en az 5, en çok 50,000 akçeyle oynanır.\n\r", ch);
        return;
	}
	ch->silver -= yatirilan;
	ch_zar = 0;
	mekanci_zar = 0;

	while(mekanci_zar==ch_zar)
	{
		ch_zar=number_range( 1, zar_tipi );
		mekanci_zar=number_range( 1, zar_tipi );
	}
	WAIT_STATE (ch, PULSE_VIOLENCE);

	printf_to_char(ch,L"Mekancı %d yüzlü bir zarı sallayıp atıyor... %d geldi.\n\r",zar_tipi,mekanci_zar);
	printf_to_char(ch,L"%d yüzlü bir zarı sallayıp atıyorsun... %d geldi.\n\r",zar_tipi,ch_zar);
    act ( L"$n ve mekancı ellerindeki zarları sallayarak masaya atıyorlar.\n\r", ch,NULL,NULL,TO_ROOM);
	if (mekanci_zar > ch_zar)
    {
        printf_to_char (ch, L"Kaybettin.\n\r");
        act ( L"$n kaybetti.", ch, NULL, NULL, TO_ROOM);
        return;
    }
	/* ch kazandı */
	kazanc=2*yatirilan;
    printf_to_char (ch, L"%d akçe KAZANDIN.\n\r", yatirilan);
    act ( L"$n zar attı ve kazandı. Vay beee...\n\r", ch, NULL, NULL, TO_ROOM);
    ch->silver += kazanc;
    return;
}
