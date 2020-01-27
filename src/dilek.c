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
        printf_to_char(ch,"D�LEK komutlar�: ALI� L�STE �ZELL�K.\n\r");
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
        printf_to_char(ch,"Burada dilek dileyemezsin. Bir dilek ta�� uzman� bul.\n\r");
        return;
    }

	if ( wishmaster->fighting != NULL)
    {
        printf_to_char(ch,"D�v�� bitene kadar bekle.\n\r");
        return;
    }

	if (!strcmp(arg1, "liste"))
	{
		act( "$n $Z dilek ta�lar�n�n listesini istiyor.", ch, NULL, wishmaster, TO_ROOM);
		act ("$Z dilek ta�lar�n�n listesini istiyorsun.",ch, NULL, wishmaster, TO_CHAR);

		printf_to_char(ch, "Mevcut dilek ta�lar�:\n\r\n\r");
		printf_to_char(ch, "  {Ctecr�be ta��{x....({ytecr�be{x)....{R3000 gp, 5000 alt�n{x\n\r");
		printf_to_char(ch, "  {Cg�rev ta��{x......({yg�rev  {x)....{R3000 gp, 5000 alt�n{x\n\r");
		return;
	}
	else if (!strcmp(arg1, "�zellik"))
	{
		if (arg2[0] == '\0')
		{
			printf_to_char(ch,"Hangi dilek ta��n�n �zelliklerini ��renmek istiyorsun?\n\rKullan�m: {Rdilek �zellik <ta�>{x\n\r");
			return;
		}
		else if (is_name(arg2, (char*)"tecr�be"))
		{
			printf_to_char(ch,"Tecr�be ta�� ile dilenen dilekten sonra ki�i her defas�nda iki kat tecr�be kazan�r.\n\r");
			return;
		}
		else if (is_name(arg2, (char*)"g�rev"))
		{
			printf_to_char(ch,"G�rev ta�� ile dilenen dilekten sonra ki�i her g�rev tamamlay���nda iki kat GP kazan�r.\n\r");
			return;
		}
		else
		{
			printf_to_char(ch,"Hangi dilek ta��n�n �zelliklerini ��renmek istiyorsun?\n\rKullan�m: {Rdilek �zellik <ta�>{x\n\r");
			return;
		}
	}
	else if (!strcmp(arg1, "al��"))
	{
		if (arg2[0] == '\0')
		{
			printf_to_char( ch , "Dilek sat�n almak i�in {Rdilek al�� <ta�>{x kullan�labilir.\n\r");
			return;
		}
		else if (is_name(arg2, (char*)"tecr�be"))
		{
			if(IS_SET(ch->pcdata->dilek,DILEK_FLAG_TECRUBE))
			{
				do_say(wishmaster,(char*)"Bu dile�i zaten dilemi�sin.");
				return;
			}
			if (ch->pcdata->questpoints >= 3000 && ch->gold >= 5000)
			{
				ch->pcdata->questpoints -= 3000;
				ch->gold -= 5000;
				SET_BIT(ch->pcdata->dilek,DILEK_FLAG_TECRUBE);
				do_say(wishmaster,(char*)"��te bu ta� senin dile�in i�in...");
				printf_to_char( ch , "\n\r{CH�ssam mavi bir dilek ta��na son �eklini verdikten sonra kuyuya at�yor.{x\n\r");
				printf_to_char( ch , "{CKuyudan y�kselen bir ���k yava��a bedenini sar�p seninle b�t�nle�iyor.{x\n\r");
				return;
			}
			else
			{
				sprintf(buf, "�zg�n�m %s, o kadar puan�n veya paran yok.",ch->name);
				do_say(wishmaster,buf);
				return;
			}
		}
		else if (is_name(arg2, (char*)"g�rev"))
		{
			if(IS_SET(ch->pcdata->dilek,DILEK_FLAG_GOREV))
			{
				do_say(wishmaster,(char*)"Bu dile�i zaten dilemi�sin.");
				return;
			}
			if (ch->pcdata->questpoints >= 3000 && ch->gold >= 5000)
			{
				ch->pcdata->questpoints -= 3000;
				ch->gold -= 5000;
				SET_BIT(ch->pcdata->dilek,DILEK_FLAG_GOREV);
				do_say(wishmaster,(char*)"��te bu ta� senin dile�in i�in...");
				printf_to_char( ch , "\n\r{CH�ssam k�z�l bir dilek ta��na son �eklini verdikten sonra kuyuya at�yor.{x\n\r");
				printf_to_char( ch , "{CKuyudan y�kselen bir ���k yava��a bedenini sar�p seninle b�t�nle�iyor.{x\n\r");
				return;
			}
			else
			{
				sprintf(buf, "�zg�n�m %s, o kadar puan�n veya paran yok.",ch->name);
				do_say(wishmaster,buf);
				return;
			}
		}
		else
		{
			printf_to_char( ch , "Dilek sat�n almak i�in {Rdilek al�� <ta�>{x kullan�labilir.\n\r");
			return;
		}
	}
	else
    {
        printf_to_char(ch,"D�LEK komutlar�: ALI� L�STE �ZELL�K.\n\r");
        return;
    }
}
