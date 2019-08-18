/***************************************************************************
*    Uzak Diyarlar, Rom2.4b6 tabanlý Türkçe Mud                            *
*    http://www.uzakdiyarlar.com                                           *
*    http://sourceforge.net/projects/uzakdiyarlar                          *
*    Özgür Yýlmaz (yelbuke@gmail.com)                                      *
***************************************************************************/
#include "ek.h"

const struct pers_suffix_type pers_suffix_table[] =
{
	{(char*)"sS"	,(char*)"birisinin"	,(char*)"bir ölümsüzün"	,(char*)"n"	,(char*)"ýn",(char*)"in",(char*)"un",(char*)"ün"	},
	{(char*)"mM"	,(char*)"birisini"	,(char*)"bir ölümsüzü"	,(char*)"y"	,(char*)"ý"	,(char*)"i"	,(char*)"u"	,(char*)"ü"	},
	{(char*)"eE"	,(char*)"birisine"	,(char*)"bir ölümsüze"	,(char*)"y"	,(char*)"a"	,(char*)"e"	,(char*)"a"	,(char*)"e"	},
	{(char*)"yY"	,(char*)"birisinde"	,(char*)"bir ölümsüzde"	,(char*)"d"	,(char*)"a"	,(char*)"e"	,(char*)"a"	,(char*)"e"	},
	{(char*)"zZ"	,(char*)"birisinden",(char*)"bir ölümsüzden",(char*)"d"	,(char*)"an",(char*)"en",(char*)"an",(char*)"en"	},
    {NULL	,NULL			,NULL				,NULL	,NULL	,NULL	,NULL	,NULL	}
};

char *ekler (CHAR_DATA *to, CHAR_DATA *ch, char *format)
{
	int say;//pers_suffix_table'da kacinci elemanin okunacagini tutuyor.
	char *i, *ek;// i BUF'u point edecek.
	char buf[MAX_STRING_LENGTH];//return olacak bilgi burada
	/* SAY deðiþkenine deðeri atanýyor */
	for(say=0;pers_suffix_table[say].belirtec[0] != '\0';say++)
	{
		if(	pers_suffix_table[say].belirtec[0]==*format ||
			pers_suffix_table[say].belirtec[1]==*format	)
				break;
	}
	if(say>=5)//tablodaki doðru elemaný bulamadýðý için çökme olmasýn diye.
		say=1;
	buf[0]='\0';
	i=NULL;
	if(can_see(to,ch))
	{
		sprintf(buf,"%s%s",	!IS_NPC(ch)?ch->name:ch->short_descr,
							!IS_NPC(ch)?"'":"");
		/*  sondaki sessizin yumuþamasý. aslýnda tek heceli kelimelerde
		 *  yumusama olmaz. ama tek hece kontrolü yok.
		 */
		if(IS_NPC(ch))
		{
			switch( ch->short_descr[strlen(ch->short_descr)-1] )
			{
				case 'p':
					buf[strlen(buf)-1]='b';
					break;
				case 'ç':
					buf[strlen(buf)-1]='c';
					break;
				case 't':
					buf[strlen(buf)-1]='d';
					break;
				case 'k':
					buf[strlen(buf)-1]='g';
					break;
			}
		}
		ek=NULL;// ek ve i yerine BUF kullanilabilir mi
		ek=son_harf_unlu(ch,say);
		strcat(buf,ek);
		i=buf;
		return i;
	}
	strcpy(buf,pers_suffix_table[say].birisi);
	i=buf;
	return i;
}

char * son_harf_unlu(CHAR_DATA *ch, int say)
{
	char isim[MAX_STRING_LENGTH];
	char buf[MAX_STRING_LENGTH];
	char *ptr;
	unsigned int number;
	ptr=buf;

	(!IS_NPC(ch))?strcpy(isim,ch->name):strcpy(isim,ch->short_descr);

	buf[0]='\0';
	for(number=1;strlen(isim)>=number;number++)
	{
		switch(isim[strlen(isim)-number])
		{
			case 'a': case 'ý':
				if(say>2)
				{
					sprintf(buf,"d%s",pers_suffix_table[say].bir);
					return ptr;
				}
				sprintf(buf,"%s%s",(number==1)?(pers_suffix_table[say].sesli_eki):(""),pers_suffix_table[say].bir);
				return ptr;
			case 'e': case 'i':
				if(say>2)
				{
					sprintf(buf,"d%s",pers_suffix_table[say].iki);
					return ptr;
				}
				sprintf(buf,"%s%s",(number==1)?(pers_suffix_table[say].sesli_eki):(""),pers_suffix_table[say].iki);
				return ptr;
			case 'o': case 'u':
				if(say>2)
				{
					sprintf(buf,"d%s",pers_suffix_table[say].uc);
					return ptr;
				}
				sprintf(buf,"%s%s",(number==1)?(pers_suffix_table[say].sesli_eki):(""),pers_suffix_table[say].uc);
				return ptr;
			case 'ö': case 'ü':
				if(say>2)
				{
					sprintf(buf,"d%s",pers_suffix_table[say].dort);
					return ptr;
				}
				sprintf(buf,"%s%s",(number==1)?(pers_suffix_table[say].sesli_eki):(""),pers_suffix_table[say].dort);
				return ptr;
		}
	}
	sprintf(buf,"%s",pers_suffix_table[say].bir);
	return ptr;
}
