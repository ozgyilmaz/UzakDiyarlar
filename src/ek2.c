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
 
#if defined(macintosh)
#include <types.h>
#include <time.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "merc.h"
#include "interp.h"
#include "ek2.h"
/*
hassan-da
ercüment-de
ýtýr-da
iyþi-de
bozo-da
gözö-de
kuzu-da
büdü-de
*/

const struct sonek_type sonek_table[] =
{
	{{(char*)"n",(char*)"ýn" ,(char*)"in"	,(char*)"un" ,(char*)"ün"	}},//f - un
 	{{(char*)"y",(char*)"ý"	 ,(char*)"i"	,(char*)"u"	 ,(char*)"ü"	}},//g - u
  {{(char*)"y",(char*)"a"	 ,(char*)"e"	,(char*)"a"	 ,(char*)"e"	}},//h - e
  {{(char*)""	,(char*)"da" ,(char*)"de"	,(char*)"da" ,(char*)"de"	}},//j - de
  {{(char*)""	,(char*)"dan",(char*)"den",(char*)"dan",(char*)"den"	}},//k - den
     	{{NULL	,NULL	,NULL	,NULL	,NULL	}}
};

bool bu_harf_unlu_mu(char harf)
{
	if(harf=='a' || harf=='A' || harf=='e' || harf=='E' || harf=='ý' || harf=='I' || harf=='i' || harf=='Ý' || harf=='o' || harf=='O' || harf=='ö' || harf=='Ö' || harf=='u' || harf=='U' || harf=='ü' || harf=='Ü')
		return TRUE;
	return FALSE;
}

bool son_harf_unlu_mu(char *sozcuk)
{
	const 	char 	*str;
	// son harfe ilerleyelim...
	str = sozcuk;
	while(*str != '\0')
	{
		++str;
	}
	//son harf þu:
	--str;

	if(bu_harf_unlu_mu(*str))
		return TRUE;
	return FALSE;
}

char son_unlu_harf_hangisi(char *sozcuk)
{
	const 	char 	*str;
	char unlu=0;
	str=sozcuk;
	// son harfe ilerleyelim. ilerlerken de son ünlüyü bulalým...
	while(*str != '\0')
	{
		if(bu_harf_unlu_mu(*str))
			unlu=*str;
		++str;
	}

	return unlu;
}

char *ek_olustur(char *sozcuk, char tip)
{
	char buf[MAX_STRING_LENGTH];
	char *pbuf;
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
			case 'a':case 'ý':
				sprintf(buf,"%s%s%s",sozcuk,sonek_table[i].ek[0],sonek_table[i].ek[1]);
				break;
			case 'e':case 'i':
				sprintf(buf,"%s%s%s",sozcuk,sonek_table[i].ek[0],sonek_table[i].ek[2]);
				break;
			case 'o':case 'u':
				sprintf(buf,"%s%s%s",sozcuk,sonek_table[i].ek[0],sonek_table[i].ek[3]);
				break;
			case 'ö':case 'ü':
				sprintf(buf,"%s%s%s",sozcuk,sonek_table[i].ek[0],sonek_table[i].ek[4]);
				break;
			default:
				sprintf(buf,"%s%s%s",sozcuk,sonek_table[i].ek[0],sonek_table[i].ek[1]);
				break;
		}
	}
	else
	{
		switch(son_unlu)
		{
			case 'a':case 'ý':
				sprintf(buf,"%s%s",sozcuk,sonek_table[i].ek[1]);
				break;
			case 'e':case 'i':
				sprintf(buf,"%s%s",sozcuk,sonek_table[i].ek[2]);
				break;
			case 'o':case 'u':
				sprintf(buf,"%s%s%s",sozcuk,sonek_table[i].ek[0],sonek_table[i].ek[3]);
				break;
			case 'ö':case 'ü':
				sprintf(buf,"%s%s%s",sozcuk,sonek_table[i].ek[0],sonek_table[i].ek[4]);
				break;
			default:
				sprintf(buf,"%s%s",sozcuk,sonek_table[i].ek[1]);
				break;
		}
	}
	pbuf=buf;
	return pbuf;

}
