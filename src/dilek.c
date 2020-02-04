#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"

DECLARE_DO_FUN( do_say );


void do_dilek(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *wishmaster;
	char buf [MAX_STRING_LENGTH];
	char arg1 [MAX_INPUT_LENGTH];
	char arg2 [MAX_INPUT_LENGTH];

	argument = one_argument(argument, arg1);
    argument = one_argument(argument, arg2);

	if (arg1[0] == '\0')
    {
        printf_to_char(ch,"DÝLEK komutlarý: ALIÞ LÝSTE ÖZELLÝK.\n\r");
        return;
    }

	for ( wishmaster = ch->in_room->people; wishmaster != NULL; wishmaster = wishmaster->next_in_room )
    {
        if (!IS_NPC( wishmaster ))
			continue;
        if (wishmaster->spec_fun == spec_lookup( (char*)"spec_wishmaster" ))
			break;
    }

    if (wishmaster == NULL || wishmaster->spec_fun != spec_lookup( (char*)"spec_wishmaster" ))
    {
        printf_to_char(ch,"Burada dilek dileyemezsin. Bir dilek taþý uzmaný bul.\n\r");
        return;
    }

	if ( wishmaster->fighting != NULL)
    {
        printf_to_char(ch,"Dövüþ bitene kadar bekle.\n\r");
        return;
    }

	if (!strcmp(arg1, "liste"))
	{
		act( "$n $Z dilek taþlarýnýn listesini istiyor.", ch, NULL, wishmaster, TO_ROOM);
		act ("$Z dilek taþlarýnýn listesini istiyorsun.",ch, NULL, wishmaster, TO_CHAR);

		printf_to_char(ch, "Mevcut dilek taþlarý:\n\r\n\r");
		printf_to_char(ch, "  {Ctecrübe taþý{x....({ytecrübe{x)....{R3000 gp, 5000 altýn{x\n\r");
		printf_to_char(ch, "  {Cgörev taþý{x......({ygörev  {x)....{R3000 gp, 5000 altýn{x\n\r");
		return;
	}
	else if (!strcmp(arg1, "özellik"))
	{
		if (arg2[0] == '\0')
		{
			printf_to_char(ch,"Hangi dilek taþýnýn özelliklerini öðrenmek istiyorsun?\n\rKullaným: {Rdilek özellik <taþ>{x\n\r");
			return;
		}
		else if (is_name(arg2, (char*)"tecrübe"))
		{
			printf_to_char(ch,"Tecrübe taþý ile dilenen dilekten sonra kiþi her defasýnda iki kat tecrübe kazanýr.\n\r");
			return;
		}
		else if (is_name(arg2, (char*)"görev"))
		{
			printf_to_char(ch,"Görev taþý ile dilenen dilekten sonra kiþi her görev tamamlayýþýnda iki kat GP kazanýr.\n\r");
			return;
		}
		else
		{
			printf_to_char(ch,"Hangi dilek taþýnýn özelliklerini öðrenmek istiyorsun?\n\rKullaným: {Rdilek özellik <taþ>{x\n\r");
			return;
		}
	}
	else if (!strcmp(arg1, "alýþ"))
	{
		if (arg2[0] == '\0')
		{
			printf_to_char( ch , "Dilek satýn almak için {Rdilek alýþ <taþ>{x kullanýlabilir.\n\r");
			return;
		}
		else if (is_name(arg2, (char*)"tecrübe"))
		{
			if(IS_SET(ch->pcdata->dilek,DILEK_FLAG_TECRUBE))
			{
				do_say(wishmaster,(char*)"Bu dileði zaten dilemiþsin.");
				return;
			}
			if (ch->pcdata->questpoints >= 3000 && ch->gold >= 5000)
			{
				ch->pcdata->questpoints -= 3000;
				ch->gold -= 5000;
				SET_BIT(ch->pcdata->dilek,DILEK_FLAG_TECRUBE);
				do_say(wishmaster,(char*)"Ýþte bu taþ senin dileðin için...");
				printf_to_char( ch , "\n\r{CHüssam mavi bir dilek taþýna son þeklini verdikten sonra kuyuya atýyor.{x\n\r");
				printf_to_char( ch , "{CKuyudan yükselen bir ýþýk yavaþça bedenini sarýp seninle bütünleþiyor.{x\n\r");
				return;
			}
			else
			{
				sprintf(buf, "Üzgünüm %s, o kadar puanýn veya paran yok.",ch->name);
				do_say(wishmaster,buf);
				return;
			}
		}
		else if (is_name(arg2, (char*)"görev"))
		{
			if(IS_SET(ch->pcdata->dilek,DILEK_FLAG_GOREV))
			{
				do_say(wishmaster,(char*)"Bu dileði zaten dilemiþsin.");
				return;
			}
			if (ch->pcdata->questpoints >= 3000 && ch->gold >= 5000)
			{
				ch->pcdata->questpoints -= 3000;
				ch->gold -= 5000;
				SET_BIT(ch->pcdata->dilek,DILEK_FLAG_GOREV);
				do_say(wishmaster,(char*)"Ýþte bu taþ senin dileðin için...");
				printf_to_char( ch , "\n\r{CHüssam kýzýl bir dilek taþýna son þeklini verdikten sonra kuyuya atýyor.{x\n\r");
				printf_to_char( ch , "{CKuyudan yükselen bir ýþýk yavaþça bedenini sarýp seninle bütünleþiyor.{x\n\r");
				return;
			}
			else
			{
				sprintf(buf, "Üzgünüm %s, o kadar puanýn veya paran yok.",ch->name);
				do_say(wishmaster,buf);
				return;
			}
		}
		else
		{
			printf_to_char( ch , "Dilek satýn almak için {Rdilek alýþ <taþ>{x kullanýlabilir.\n\r");
			return;
		}
	}
	else
    {
        printf_to_char(ch,"DÝLEK komutlarý: ALIÞ LÝSTE ÖZELLÝK.\n\r");
        return;
    }
}
