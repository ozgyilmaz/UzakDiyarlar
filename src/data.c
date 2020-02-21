/***************************************************************************
 *                                                                         *
 * Uzak Diyarlar açýk kaynak Türkçe Mud projesidir.                        *
 * Oyun geliþtirmesi Jai ve Maru tarafýndan yönetilmektedir.               *
 * Unutulmamasý gerekenler: Nir, Kame, Nyah, Sint                          *
 *                                                                         *
 * Github  : https://github.com/yelbuke/UzakDiyarlar                       *
 * Web     : http://www.uzakdiyarlar.net                                   *
 * Discord : https://discord.gg/kXyZzv                                     *
 *                                                                         *
 ***************************************************************************/

#include <sys/types.h>
#include <stdio.h>
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
	fprintf(data,"* Cevrimici oyuncu rekoru\n");
	fprintf(data,"Encokcevrimici %d\n",max_on_so_far);
	fprintf(data,"End\n");
	fclose(data);
}

void ud_data_read(void)
{
	FILE *fp;
	char *word;

	max_on = 0;
	max_on_so_far  = 0;
	fp=fopen("../data/ud_data","r");

	for ( ; ; )
	{
		word   = feof( fp ) ? (char*)"End" : fread_word( fp );
		if (!str_cmp( word, (char*)"Encokcevrimici"))
		{
			max_on_so_far = fread_number( fp );
		}
		else if(!str_cmp( word, (char*)"*"))
		{
			fread_to_eol(fp);
		}
		else if(!str_cmp( word, (char*)"End"))
		{
			fclose(fp);
			return;
		}
	}
	fclose(fp);
	return;
}

void write_channel_log(CHAR_DATA *ch, CHAR_DATA *vc, int kanal, char *argument)
{

	if ( argument[0] == '\0' )
		return;

	if(IS_NPC(ch))
		return;

	FILE *data;
	char filename[MAX_STRING_LENGTH];
	char buf[100];
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	if( kanal<0 || kanal>6 )
	{
		sprintf( buf, "write_channel_log: hatali kanal %d", kanal );
		bug(buf,0);
		return;
	}

	switch(kanal)
	{
		case KANAL_SOYLE:
			sprintf(filename, "../log/kanal/soyle_%d_%02d_%02d",tm.tm_year + 1900,tm.tm_mon + 1,tm.tm_mday);break;
		case KANAL_KD:
			sprintf(filename, "../log/kanal/kd_%d_%02d_%02d",tm.tm_year + 1900,tm.tm_mon + 1,tm.tm_mday);break;
		case KANAL_ACEMI:
			sprintf(filename, "../log/kanal/acemi_%d_%02d_%02d",tm.tm_year + 1900,tm.tm_mon + 1,tm.tm_mday);break;
		case KANAL_HAYKIR:
			sprintf(filename, "../log/kanal/haykir_%d_%02d_%02d",tm.tm_year + 1900,tm.tm_mon + 1,tm.tm_mday);break;
		case KANAL_IMM:
			sprintf(filename, "../log/kanal/imm_%d_%02d_%02d",tm.tm_year + 1900,tm.tm_mon + 1,tm.tm_mday);break;
		case KANAL_GSOYLE:
			sprintf(filename, "../log/kanal/gsoyle_%d_%02d_%02d",tm.tm_year + 1900,tm.tm_mon + 1,tm.tm_mday);break;
		case KANAL_DUYGU:
			sprintf(filename, "../log/kanal/duygu_%d_%02d_%02d",tm.tm_year + 1900,tm.tm_mon + 1,tm.tm_mday);break;
	}

	if (vc == NULL)
	{

	}

	data=fopen(filename,"a");
	sprintf(buf,"%02d/%02d/%02d %02d:%02d:%02d, Oda:%6d, Char: %10s, Victim: %10s, Log: %s\n",tm.tm_year + 1900,tm.tm_mon + 1,tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,ch->in_room->vnum,ch->name,(vc != NULL)?vc->name:"None",argument);
	fprintf(data,buf);
	fclose(data);
	return;
}
