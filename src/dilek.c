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

#include <sys/types.h>
#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"

DECLARE_DO_FUN( do_say );


void do_dilek(CHAR_DATA *ch, wchar_t *argument)
{
	CHAR_DATA *wishmaster;
	wchar_t buf [MAX_STRING_LENGTH];
	wchar_t arg1 [MAX_INPUT_LENGTH];
	wchar_t arg2 [MAX_INPUT_LENGTH];

	argument = one_argument(argument, arg1);
    argument = one_argument(argument, arg2);

	if (arg1[0] == '\0')
    {
        printf_to_char(ch,L"DİLEK komutları: ALIŞ LİSTE ÖZELLİK.\n\r");
        return;
    }

	for ( wishmaster = ch->in_room->people; wishmaster != NULL; wishmaster = wishmaster->next_in_room )
    {
        if (!IS_NPC( wishmaster ))
			continue;
        if (wishmaster->spec_fun == spec_lookup( (wchar_t*)"spec_wishmaster" ))
			break;
    }

    if (wishmaster == NULL || wishmaster->spec_fun != spec_lookup( (wchar_t*)"spec_wishmaster" ))
    {
        printf_to_char(ch,L"Burada dilek dileyemezsin. Bir dilek taşı uzmanı bul.\n\r");
        return;
    }

	if ( wishmaster->fighting != NULL)
    {
        printf_to_char(ch,L"Dövüş bitene kadar bekle.\n\r");
        return;
    }

	if (!wcscmp(arg1, L"liste"))
	{
		act( L"$n $Z dilek taşlarının listesini istiyor.", ch, NULL, wishmaster, TO_ROOM);
		act ( L"$Z dilek taşlarının listesini istiyorsun.",ch, NULL, wishmaster, TO_CHAR);

		printf_to_char(ch,L"Mevcut dilek taşları:\n\r\n\r");
		printf_to_char(ch,L"  {Ctecrübe taşı{x....({ytecrübe{x)....{R3000 gp, 50000 akçe{x\n\r");
		printf_to_char(ch,L"  {Cgörev taşı{x......({ygörev  {x)....{R3000 gp, 50000 akçe{x\n\r");
		return;
	}
	else if (!wcscmp(arg1, L"özellik"))
	{
		if (arg2[0] == '\0')
		{
			printf_to_char(ch,L"Hangi dilek taşının özelliklerini öğrenmek istiyorsun?\n\rKullanım: {Rdilek özellik <taş>{x\n\r");
			return;
		}
		else if (is_name(arg2, (wchar_t*)"tecrübe"))
		{
			printf_to_char(ch,L"Tecrübe taşı ile dilenen dilekten sonra kişi her defasında iki kat tecrübe kazanır.\n\r");
			return;
		}
		else if (is_name(arg2, (wchar_t*)"görev"))
		{
			printf_to_char(ch,L"Görev taşı ile dilenen dilekten sonra kişi her görev tamamlayışında iki kat GP kazanır.\n\r");
			return;
		}
		else
		{
			printf_to_char(ch,L"Hangi dilek taşının özelliklerini öğrenmek istiyorsun?\n\rKullanım: {Rdilek özellik <taş>{x\n\r");
			return;
		}
	}
	else if (!wcscmp(arg1, L"satınal"))
	{
		if (arg2[0] == '\0')
		{
			printf_to_char( ch , L"Dilek satın almak için {Rdilek satınal <taş>{x kullanılabilir.\n\r");
			return;
		}
		else if (is_name(arg2, (wchar_t*)"tecrübe"))
		{
			if(IS_SET(ch->pcdata->dilek,DILEK_FLAG_TECRUBE))
			{
				do_say(wishmaster,(wchar_t*)"Bu dileği zaten dilemişsin.");
				return;
			}
			if (ch->pcdata->questpoints >= 3000 && ch->silver >= 50000)
			{
				ch->pcdata->questpoints -= 3000;
				ch->silver -= 50000;
				SET_BIT(ch->pcdata->dilek,DILEK_FLAG_TECRUBE);
				do_say(wishmaster,(wchar_t*)"İşte bu taş senin dileğin için...");
				printf_to_char( ch , L"\n\r{CHüssam mavi bir dilek taşına son şeklini verdikten sonra kuyuya atıyor.{x\n\r");
				printf_to_char( ch , L"{CKuyudan yükselen bir ışık yavaşça bedenini sarıp seninle bütünleşiyor.{x\n\r");
				return;
			}
			else
			{
				swprintf( buf, MAX_STRING_LENGTH-1, L"Üzgünüm %s, o kadar puanın veya paran yok.",ch->name);
				do_say(wishmaster,buf);
				return;
			}
		}
		else if (is_name(arg2, (wchar_t*)"görev"))
		{
			if(IS_SET(ch->pcdata->dilek,DILEK_FLAG_GOREV))
			{
				do_say(wishmaster,(wchar_t*)"Bu dileği zaten dilemişsin.");
				return;
			}
			if (ch->pcdata->questpoints >= 3000 && ch->silver >= 50000)
			{
				ch->pcdata->questpoints -= 3000;
				ch->silver -= 50000;
				SET_BIT(ch->pcdata->dilek,DILEK_FLAG_GOREV);
				do_say(wishmaster,(wchar_t*)"İşte bu taş senin dileğin için...");
				printf_to_char( ch , L"\n\r{CHüssam kızıl bir dilek taşına son şeklini verdikten sonra kuyuya atıyor.{x\n\r");
				printf_to_char( ch , L"{CKuyudan yükselen bir ışık yavaşça bedenini sarıp seninle bütünleşiyor.{x\n\r");
				return;
			}
			else
			{
				swprintf( buf, MAX_STRING_LENGTH-1, L"Üzgünüm %s, o kadar puanın veya akçen yok.",ch->name);
				do_say(wishmaster,buf);
				return;
			}
		}
		else
		{
			printf_to_char( ch , L"Dilek satın almak için {Rdilek satınal <taş>{x kullanılabilir.\n\r");
			return;
		}
	}
	else
    {
        printf_to_char(ch,L"DİLEK komutları: SATINAL LİSTE ÖZELLİK.\n\r");
        return;
    }
}
