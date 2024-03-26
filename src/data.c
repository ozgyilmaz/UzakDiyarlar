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

void write_channel_log(CHAR_DATA* ch, CHAR_DATA* vc, int kanal, char* argument) {
    if (argument[0] == '\0' || IS_NPC(ch)) {
        return;
    }

    FILE* data;
    char filename[MAX_STRING_LENGTH];
    char buf[1024]; // Increased buffer size for safe operation
    char bug_message[MAX_STRING_LENGTH]; // For formatting messages for bug()
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    if (kanal < 0 || kanal > 6) {
        snprintf(bug_message, sizeof(bug_message), "write_channel_log: invalid channel %d", kanal);
        bug(bug_message, 0);
        return;
    }

    // Construct filename based on channel
    snprintf(filename, sizeof(filename), "../log/kanal/%s_%d_%02d_%02d",
        (kanal == KANAL_SOYLE) ? "soyle" :
        (kanal == KANAL_KD) ? "kd" :
        (kanal == KANAL_ACEMI) ? "acemi" :
        (kanal == KANAL_HAYKIR) ? "haykir" :
        (kanal == KANAL_IMM) ? "imm" :
        (kanal == KANAL_GSOYLE) ? "gsoyle" : "duygu",
        tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

    data = fopen(filename, "a");
    if (!data) {
        snprintf(bug_message, sizeof(bug_message), "write_channel_log: unable to open file %s", filename);
        bug(bug_message, 0);
        return;
    }

    // Use snprintf to avoid buffer overflow and safely format the log message
    snprintf(buf, sizeof(buf), "%02d/%02d/%02d %02d:%02d:%02d, Oda:%6d, Char: %10s, Victim: %10s, Log: %s\n",
        tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
        tm.tm_hour, tm.tm_min, tm.tm_sec,
        ch->in_room ? ch->in_room->vnum : 0, // Added null check for ch->in_room
        ch->name,
        (vc != NULL) ? vc->name : "None",
        argument);

    fputs(buf, data);
    fclose(data);
}
