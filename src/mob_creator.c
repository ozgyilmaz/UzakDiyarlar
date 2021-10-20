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
#include <math.h>
#include "merc.h"
#include "interp.h"

int hitroll_damroll_hesapla(int level)
{
	int seviyenin_yarisi;
	int seviyenin_onda_biri;

	seviyenin_yarisi = UMAX(1, (ceil) ((float)(level/2)) );
	seviyenin_onda_biri = UMAX(1, (ceil) ((float)(level/10)) );

	return ( number_range( UMAX(1,seviyenin_yarisi-seviyenin_onda_biri),UMAX(1,seviyenin_yarisi+(2*seviyenin_onda_biri))) );
}

int damage_dice_0(int level)
{
	int type, number;

	type   = level*7/4;
	number = UMIN(type/8 + 1, 5);

	return number;
}

int damage_dice_1(int level)
{
	int type, number;

	type   = level*7/4;
	number = UMIN(type/8 + 1, 5);
	type   = UMAX(2, type/number);

	return type;
}

int damage_dice_2(int level)
{
	int type, number, bonus;

	type   = level*7/4;
	number = UMIN(type/8 + 1, 5);
	type   = UMAX(2, type/number);
	bonus  = UMAX(0, level*9/4 - number*type);

	return bonus;
}

int dam_type_dice(void)
{
	int type=3;
	switch ( number_range( 1, 7 ) )
	{
		case (1): type =  3;       break;  /* slash  */
		case (2): type =  7;       break;  /* pound  */
		case (3): type = 11;       break;  /* pierce */
		case (4): type = 13;       break;  /* beating */
		case (5): type = 17;       break;  /* punch */
		case (6): type = 16;       break;  /* slap */
		case (7): type = 8;        break;  /* crush */
	}

	return type;
}

int ac_dice(int i,int level)
{
	int k;
	switch(i)
	{
		case 0:
		case 1:
		case 2:
			k = interpolate( level, 100, -100);
		break;
		case 3:
			k = interpolate( level, 100, 0);
		break;
		default:
			k = interpolate( level, 100, 0);
	}
	return k;
}

int position_dice(void)
{
	int dice;

  dice = number_range( 1, 100 );

  if(dice < 50)
    return POS_STANDING;
  else if(dice < 75)
    return POS_SITTING;
  else if(dice < 90)
    return POS_RESTING;
  else
    return POS_SLEEPING;
}

int sex_dice(void)
{
	return number_range(1,2);
}
