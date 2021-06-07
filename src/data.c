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
#include "interp.h"

extern int max_on;
extern int max_on_so_far;

void ud_data_write(void)
{
	FILE *data;
	int suan;

	cevrimici_oyuncu_sayisi();
	system("rm -f ../data/ud_data");
	data=fopen("../data/ud_data","a");
	fwprintf(data, L"* Cevrimici oyuncu rekoru\n");
	fwprintf(data, L"Encokcevrimici %d\n",max_on_so_far);
	fwprintf(data, L"End\n");
	fclose(data);
}

void ud_data_read(void)
{
	FILE *fp;
	wchar_t *word;
	wchar_t buf[500];

	max_on = 0;
	max_on_so_far  = 0;
	fp=fopen("../data/ud_data","r");

	for ( ; ; )
	{

		word   = feof( fp ) ? (wchar_t*)"End" : fread_word( fp );

		if (!wcscasecmp( word, L"Encokcevrimici"))
		{
			max_on_so_far = fread_number( fp );
		}
		else if(!wcscasecmp( word, L"*"))
		{
			fread_to_eol(fp);
		}
		else if(!wcscasecmp( word, L"End"))
		{
			fclose(fp);
			return;
		}
	}

	fclose(fp);
	return;
}

void write_channel_log(CHAR_DATA *ch, CHAR_DATA *vc, int kanal, wchar_t *argument)
{

	if ( argument[0] == '\0' )
		return;

	if(IS_NPC(ch))
		return;

	FILE *data;
	wchar_t filename[MAX_STRING_LENGTH];
	wchar_t buf[100];
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	if( kanal<0 || kanal>6 )
	{
		swprintf( buf, MAX_STRING_LENGTH-1, L"write_channel_log: hatali kanal %d", kanal );
		bug(buf,0);
		return;
	}

	switch(kanal)
	{
		case KANAL_SOYLE:
			swprintf(filename, MAX_STRING_LENGTH-1, L"../log/kanal/soyle_%d_%02d_%02d",tm.tm_year + 1900,tm.tm_mon + 1,tm.tm_mday);break;
		case KANAL_KD:
			swprintf(filename, MAX_STRING_LENGTH-1, L"../log/kanal/kd_%d_%02d_%02d",tm.tm_year + 1900,tm.tm_mon + 1,tm.tm_mday);break;
		case KANAL_ACEMI:
			swprintf(filename, MAX_STRING_LENGTH-1, L"../log/kanal/acemi_%d_%02d_%02d",tm.tm_year + 1900,tm.tm_mon + 1,tm.tm_mday);break;
		case KANAL_HAYKIR:
			swprintf(filename, MAX_STRING_LENGTH-1, L"../log/kanal/haykir_%d_%02d_%02d",tm.tm_year + 1900,tm.tm_mon + 1,tm.tm_mday);break;
		case KANAL_IMM:
			swprintf(filename, MAX_STRING_LENGTH-1, L"../log/kanal/imm_%d_%02d_%02d",tm.tm_year + 1900,tm.tm_mon + 1,tm.tm_mday);break;
		case KANAL_GSOYLE:
			swprintf(filename, MAX_STRING_LENGTH-1, L"../log/kanal/gsoyle_%d_%02d_%02d",tm.tm_year + 1900,tm.tm_mon + 1,tm.tm_mday);break;
		case KANAL_DUYGU:
			swprintf(filename, MAX_STRING_LENGTH-1, L"../log/kanal/duygu_%d_%02d_%02d",tm.tm_year + 1900,tm.tm_mon + 1,tm.tm_mday);break;
	}

	if (vc == NULL)
	{

	}

	data=fopen((char*)filename,"a");
	swprintf( buf, MAX_STRING_LENGTH-1, L"%02d/%02d/%02d %02d:%02d:%02d, Oda:%6d, Char: %10s, Victim: %10s, Log: %s\n",tm.tm_year + 1900,tm.tm_mon + 1,tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,ch->in_room->vnum,ch->name,(vc != NULL)?vc->name:L"None",argument);
	fwprintf(data,buf);
	fclose(data);
	return;
}
