/***************************************************************************
*    Uzak Diyarlar, Anatolia 3.0 tabanlý Türkçe Mud                        *
*    http://www.uzakdiyarlar.net                                           *
*    https://github.com/yelbuke/UzakDiyarlar                               *
***************************************************************************/
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "magic.h"

DECLARE_DO_FUN( do_say );

CHAR_DATA *  find_mafya	args( ( CHAR_DATA * ch ) );

CHAR_DATA * find_mafya ( CHAR_DATA *ch )
{
    CHAR_DATA * mafya;

    for ( mafya = ch->in_room->people; mafya != NULL; mafya = mafya->next_in_room ) {
	if (!IS_NPC(mafya))
	    continue;

        if ( IS_NPC(mafya) && IS_SET(mafya->act, ACT_MAFYA) )
	    return mafya;
    }

    if ( mafya == NULL ) {
	send_to_char("Burada olmaz.\n\r", ch);
	return NULL;
    }

    return NULL;
}

void do_iste( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA * mafya;
	char arg1[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];
	int	cost;

	mafya = find_mafya(ch);
	if (!mafya)
		return;

	if (argument[0] == '\0')
	{
		do_say(mafya, (char*)"Lakap kaldýrtmak:");
		do_say(mafya, (char*)"aranýyor:  2000 altýn");
		do_say(mafya, (char*)"talep lakapkaldýr <aranýyor>");
		return;
    }

	argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

	if (!str_prefix(arg1,"lakapkaldýr"))
	{
		if (!str_prefix(arg2,"aranýyor"))
		{
			cost = 2000;
			if ( !IS_SET(ch->act, PLR_WANTED) )
			{
				do_say(mafya, (char*)"Aranmýyorsun ki! Hatta oldukça zararsýz bi tipe benziyorsun!");
				return;
			}
			if ( cost > ch->pcdata->bank_g )
			{
				do_say(mafya, (char*)"Bankada yeterli altýnýn yok.");
				return;
			}
			REMOVE_BIT( ch->act, PLR_WANTED );
			ch->pcdata->bank_g 	    -= cost;
			mafya->gold  += cost;
			do_say(mafya, (char*)"Temiz iþ oldu. Yüzbaþýnýn ofisindeki kayýtlarý sildik. Artýk aranmýyorsun!");
			do_say(mafya, (char*)"Yine bekleriz.");
			return;
		}
	}
	else
	{
		do_say(mafya, (char*)"Ne söylediðini anlamýyorum, ama þunlar ilgini çekebilir...");
		do_say(mafya, (char*)"Lakap kaldýrtmak:");
		do_say(mafya, (char*)"aranýyor:  2000 altýn");
		do_say(mafya, (char*)"talep lakapkaldýr <aranýyor>");
		return;
	}
}
