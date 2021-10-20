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
 
#if defined(macintosh)
#include <types.h>
#include <time.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#endif
#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include "merc.h"
#include "interp.h"
#include "ek2.h"
/*
hassan-da
ercüment-de
ıtır-da
iyşi-de
bozo-da
gözö-de
kuzu-da
büdü-de
*/

const struct sonek_type sonek_table[] =
{
	{{(wchar_t*)"n",(wchar_t*)"ın" ,(wchar_t*)"in"	,(wchar_t*)"un" ,(wchar_t*)"ün"	}},//f - un
 	{{(wchar_t*)"y",(wchar_t*)"ı"	 ,(wchar_t*)"i"	,(wchar_t*)"u"	 ,(wchar_t*)"ü"	}},//g - u
  {{(wchar_t*)"y",(wchar_t*)"a"	 ,(wchar_t*)"e"	,(wchar_t*)"a"	 ,(wchar_t*)"e"	}},//h - e
  {{(wchar_t*)""	,(wchar_t*)"da" ,(wchar_t*)"de"	,(wchar_t*)"da" ,(wchar_t*)"de"	}},//j - de
  {{(wchar_t*)""	,(wchar_t*)"dan",(wchar_t*)"den",(wchar_t*)"dan",(wchar_t*)"den"	}},//k - den
     	{{NULL	,NULL	,NULL	,NULL	,NULL	}}
};

bool bu_harf_unlu_mu(wchar_t harf)
{
	if(harf=='a' || harf=='A' || harf=='e' || harf=='E' || harf=='ı' || harf=='I' || harf=='i' || harf=='İ' || harf=='o' || harf=='O' || harf=='ö' || harf=='Ö' || harf=='u' || harf=='U' || harf=='ü' || harf=='Ü')
		return TRUE;
	return FALSE;
}

bool son_harf_unlu_mu(wchar_t *sozcuk)
{
	const 	wchar_t 	*str;
	// son harfe ilerleyelim...
	str = sozcuk;
	while(*str != '\0')
	{
		++str;
	}
	//son harf şu:
	--str;

	if(bu_harf_unlu_mu(*str))
		return TRUE;
	return FALSE;
}

wchar_t son_unlu_harf_hangisi(wchar_t *sozcuk)
{
	const 	wchar_t 	*str;
	wchar_t unlu=0;
	str=sozcuk;
	// son harfe ilerleyelim. ilerlerken de son ünlüyü bulalım...
	while(*str != '\0')
	{
		if(bu_harf_unlu_mu(*str))
			unlu=*str;
		++str;
	}

	return unlu;
}

wchar_t *ek_olustur(wchar_t *sozcuk, wchar_t tip)
{
	wchar_t buf[MAX_STRING_LENGTH];
	wchar_t *pbuf;
	int son_unlu,i;
	if(tip=='f')
		i=0;
	else if(tip=='g')
		i=1;
	else if(tip=='h')
		i=2;
	else if(tip=='j')
		i=3;
	else if(tip=='k')
		i=4;
	else
		i=0;
	son_unlu=son_unlu_harf_hangisi(sozcuk);

	if(son_harf_unlu_mu(sozcuk))
	{
		switch(son_unlu)
		{
			case 'a':case 'ı':
				swprintf( buf, MAX_STRING_LENGTH-1, L"%s%s%s",sozcuk,sonek_table[i].ek[0],sonek_table[i].ek[1]);
				break;
			case 'e':case 'i':
				swprintf( buf, MAX_STRING_LENGTH-1, L"%s%s%s",sozcuk,sonek_table[i].ek[0],sonek_table[i].ek[2]);
				break;
			case 'o':case 'u':
				swprintf( buf, MAX_STRING_LENGTH-1, L"%s%s%s",sozcuk,sonek_table[i].ek[0],sonek_table[i].ek[3]);
				break;
			case 'ö':case 'ü':
				swprintf( buf, MAX_STRING_LENGTH-1, L"%s%s%s",sozcuk,sonek_table[i].ek[0],sonek_table[i].ek[4]);
				break;
			default:
				swprintf( buf, MAX_STRING_LENGTH-1, L"%s%s%s",sozcuk,sonek_table[i].ek[0],sonek_table[i].ek[1]);
				break;
		}
	}
	else
	{
		switch(son_unlu)
		{
			case 'a':case 'ı':
				swprintf( buf, MAX_STRING_LENGTH-1, L"%s%s",sozcuk,sonek_table[i].ek[1]);
				break;
			case 'e':case 'i':
				swprintf( buf, MAX_STRING_LENGTH-1, L"%s%s",sozcuk,sonek_table[i].ek[2]);
				break;
			case 'o':case 'u':
				swprintf( buf, MAX_STRING_LENGTH-1, L"%s%s%s",sozcuk,sonek_table[i].ek[0],sonek_table[i].ek[3]);
				break;
			case 'ö':case 'ü':
				swprintf( buf, MAX_STRING_LENGTH-1, L"%s%s%s",sozcuk,sonek_table[i].ek[0],sonek_table[i].ek[4]);
				break;
			default:
				swprintf( buf, MAX_STRING_LENGTH-1, L"%s%s",sozcuk,sonek_table[i].ek[1]);
				break;
		}
	}
	pbuf=buf;
	return pbuf;

}
