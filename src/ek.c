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

#include "ek.h"

const struct pers_suffix_type pers_suffix_table[] =
{
	{(wchar_t*)"sS"	,(wchar_t*)"birisinin"	,(wchar_t*)"bir ölümsüzün"	,(wchar_t*)"n"	,(wchar_t*)"ın",(wchar_t*)"in",(wchar_t*)"un",(wchar_t*)"ün"	},
	{(wchar_t*)"mM"	,(wchar_t*)"birisini"	,(wchar_t*)"bir ölümsüzü"	,(wchar_t*)"y"	,(wchar_t*)"ı"	,(wchar_t*)"i"	,(wchar_t*)"u"	,(wchar_t*)"ü"	},
	{(wchar_t*)"eE"	,(wchar_t*)"birisine"	,(wchar_t*)"bir ölümsüze"	,(wchar_t*)"y"	,(wchar_t*)"a"	,(wchar_t*)"e"	,(wchar_t*)"a"	,(wchar_t*)"e"	},
	{(wchar_t*)"yY"	,(wchar_t*)"birisinde"	,(wchar_t*)"bir ölümsüzde"	,(wchar_t*)"d"	,(wchar_t*)"a"	,(wchar_t*)"e"	,(wchar_t*)"a"	,(wchar_t*)"e"	},
	{(wchar_t*)"zZ"	,(wchar_t*)"birisinden",(wchar_t*)"bir ölümsüzden",(wchar_t*)"d"	,(wchar_t*)"an",(wchar_t*)"en",(wchar_t*)"an",(wchar_t*)"en"	},
    {NULL	,NULL			,NULL				,NULL	,NULL	,NULL	,NULL	,NULL	}
};

wchar_t *ekler (CHAR_DATA *to, CHAR_DATA *ch, wchar_t *format)
{
	int say;//pers_suffix_table'da kacinci elemanin okunacagini tutuyor.
	wchar_t *i, *ek;// i BUF'u point edecek.
	wchar_t buf[MAX_STRING_LENGTH];//return olacak bilgi burada
	/* SAY değişkenine değeri atanıyor */
	for(say=0;pers_suffix_table[say].belirtec[0] != '\0';say++)
	{
		if(	pers_suffix_table[say].belirtec[0]==*format ||
			pers_suffix_table[say].belirtec[1]==*format	)
				break;
	}
	if(say>=5)//tablodaki doğru elemanı bulamadığı için çökme olmasın diye.
		say=1;
	buf[0]='\0';
	i=NULL;
	if(can_see(to,ch))
	{
		swprintf( buf, MAX_STRING_LENGTH-1, L"%s%s",	!IS_NPC(ch)?ch->name:ch->short_descr,
							!IS_NPC(ch)?"'":"");
		/*  sondaki sessizin yumuşaması. aslında tek heceli kelimelerde
		 *  yumusama olmaz. ama tek hece kontrolü yok.
		 */
		if(IS_NPC(ch))
		{
			switch( ch->short_descr[wcslen(ch->short_descr)-1] )
			{
				case 'p':
					buf[MAX_INPUT_LENGTH-1]='b';
					break;
				case 'ç':
					buf[MAX_INPUT_LENGTH-1]='c';
					break;
				case 't':
					buf[MAX_INPUT_LENGTH-1]='d';
					break;
				case 'k':
					buf[MAX_INPUT_LENGTH-1]='g';
					break;
			}
		}
		ek=NULL;// ek ve i yerine BUF kullanilabilir mi
		ek=son_harf_unlu(ch,say);
		wcscat(buf,ek);
		i=buf;
		return i;
	}
	wcscpy(buf,pers_suffix_table[say].birisi);
	i=buf;
	return i;
}

wchar_t * son_harf_unlu(CHAR_DATA *ch, int say)
{
	wchar_t isim[MAX_STRING_LENGTH];
	wchar_t buf[MAX_STRING_LENGTH];
	wchar_t *ptr;
	unsigned int number;
	ptr=buf;

	(!IS_NPC(ch))?wcscpy(isim,ch->name):wcscpy(isim,ch->short_descr);

	buf[0]='\0';
	for(number=1;wcslen(isim)>=number;number++)
	{
		switch(isim[wcslen(isim)-number])
		{
			case 'a': case 'ı':
				if(say>2)
				{
					swprintf( buf, MAX_STRING_LENGTH-1, L"d%s",pers_suffix_table[say].bir);
					return ptr;
				}
				swprintf( buf, MAX_STRING_LENGTH-1, L"%s%s",(number==1)?(pers_suffix_table[say].sesli_eki):(L""),pers_suffix_table[say].bir);
				return ptr;
			case 'e': case 'i':
				if(say>2)
				{
					swprintf( buf, MAX_STRING_LENGTH-1, L"d%s",pers_suffix_table[say].iki);
					return ptr;
				}
				swprintf( buf, MAX_STRING_LENGTH-1, L"%s%s",(number==1)?(pers_suffix_table[say].sesli_eki):(L""),pers_suffix_table[say].iki);
				return ptr;
			case 'o': case 'u':
				if(say>2)
				{
					swprintf( buf, MAX_STRING_LENGTH-1, L"d%s",pers_suffix_table[say].uc);
					return ptr;
				}
				swprintf( buf, MAX_STRING_LENGTH-1, L"%s%s",(number==1)?(pers_suffix_table[say].sesli_eki):(L""),pers_suffix_table[say].uc);
				return ptr;
			case 'ö': case 'ü':
				if(say>2)
				{
					swprintf( buf, MAX_STRING_LENGTH-1, L"d%s",pers_suffix_table[say].dort);
					return ptr;
				}
				swprintf( buf, MAX_STRING_LENGTH-1, L"%s%s",(number==1)?(pers_suffix_table[say].sesli_eki):(L""),pers_suffix_table[say].dort);
				return ptr;
		}
	}
	swprintf( buf, MAX_STRING_LENGTH-1, L"%s",pers_suffix_table[say].bir);
	return ptr;
}
