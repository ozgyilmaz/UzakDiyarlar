#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "interp.h"

extern int max_on;
extern int max_on_so_far;

void data_write(void)
{
	FILE *data;
	int suan;

	cevrimici_oyuncu_sayisi();
	system("rm -f ../data/data");
	data=fopen("../data/data","a");
	fprintf(data,"* Cevrimici oyuncu sayisi\n");
	fprintf(data,"Encokcevrimici %d\n",max_on_so_far);
	fprintf(data,"End\n");
	fclose(data);
}

void data_read(void)
{
	FILE *fp;
	char *word;

	max_on = 0;
	max_on_so_far  = 0;
	fp=fopen("../data/data","r");

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
