/***************************************************************************
*    Uzak Diyarlar, Rom2.4b6 tabanlý Türkçe Mud                            *
*    http://www.uzakdiyarlar.com                                           *
*    http://sourceforge.net/projects/uzakdiyarlar                          *
*    Özgür Yýlmaz (yelbuke@gmail.com)                                      *
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
#include <ctype.h>
#include "merc.h"
#include "interp.h"


void do_zar (CHAR_DATA * ch, char *argument)
{
	char arg1[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];
	int zar_tipi, ch_zar, mekanci_zar;
	long yatirilan,kazanc;

	if (!IS_SET(ch->in_room->room_flags, ROOM_KUMAR))
    {
      printf_to_char (ch,"Kumarhanede deðilsin.\n\r");
      return;
    }
	if (ch->gold < 5)
    {
        printf_to_char (ch,"Zar oynamak için en az 5 altýna ihtiyacýn var.\n\r");
        return;
    }

	argument = one_argument (argument, arg1);
	argument = one_argument (argument, arg2);

	if( arg1[0]=='\0' )
	{
		printf_to_char (ch,"Hangi zardan ne kadar oynayacaksýn? Mevcutlarý görmek için {Rzar liste{x yaz.\n\r");
        return;
	}
	if(!str_cmp(arg1,"liste"))
	{
		printf_to_char (ch,"Zar türleri:\n\r");
		printf_to_char (ch,"Zar Tipi  Açýklama\n\r");
		printf_to_char (ch,"--------  ------------\n\r");
		printf_to_char (ch,"altý      Altýlýk zar\n\r");
		printf_to_char (ch,"on        Onluk zar\n\r");
		printf_to_char (ch,"yirmi     Yirmilik zar\n\r");
		printf_to_char (ch,"yüz       Yüzlük zar\n\r\n\r");
		printf_to_char (ch,"Oynamak için {Gzar <zar_tipi> <yatýrýlan_akçe>\n\r");
		return;
	}
	if( arg2[0]=='\0' )
	{
		printf_to_char (ch,"Hangi zardan ne kadar oynayacaksýn? Mevcutlarý görmek için {Rzar liste{x yaz.\n\r");
        return;
	}
	zar_tipi=0;
	if(!str_cmp(arg1,"altý"))
		zar_tipi=6;
	else if(!str_cmp(arg1,"on"))
		zar_tipi=10;
	else if(!str_cmp(arg1,"yirmi"))
		zar_tipi=20;
	else if(!str_cmp(arg1,"yüz"))
		zar_tipi=100;
	else
	{
		printf_to_char (ch,"Hangi zardan ne kadar oynayacaksýn? Mevcutlarý görmek için {Rzar liste{x yaz.\n\r");
        return;
	}
	yatirilan=advatoi (arg2);
	if(ch->gold<yatirilan)
	{
		send_to_char ("Bu oyun lafla deðil altýnla oynanýr!\n\r", ch);
        	return;
	}
	if( (yatirilan>50000)||(yatirilan<5) )
	{
		send_to_char ("Zar en az 5, en çok 50,000 altýnla oynanýr.\n\r", ch);
        return;
	}
	ch->gold -= yatirilan;
	ch_zar = 0;
	mekanci_zar = 0;

	while(mekanci_zar==ch_zar)
	{
		ch_zar=number_range( 1, zar_tipi );
		mekanci_zar=number_range( 1, zar_tipi );
	}
	WAIT_STATE (ch, PULSE_VIOLENCE);

	printf_to_char(ch,"Mekancý %d yüzlü bir zarý sallayýp atýyor... %d geldi.\n\r",zar_tipi,mekanci_zar);
	printf_to_char(ch,"%d yüzlü bir zarý sallayýp atýyorsun... %d geldi.\n\r",zar_tipi,ch_zar);
    act ("$n ve mekancý ellerindeki zarlarý sallayarak masaya atýyorlar.\n\r", ch,NULL,NULL,TO_ROOM);
	if (mekanci_zar > ch_zar)
    {
        printf_to_char (ch,"Kaybettin.\n\r");
        act ("$n kaybetti.", ch, NULL, NULL, TO_ROOM);
        return;
    }
	/* ch kazandý */
	kazanc=2*yatirilan;
    printf_to_char (ch, "%d altýn KAZANDIN.\n\r", yatirilan);
    act ("$n zar attý ve kazandý. Vay beee...\n\r", ch, NULL, NULL, TO_ROOM);
    ch->gold += kazanc;
    return;
}
