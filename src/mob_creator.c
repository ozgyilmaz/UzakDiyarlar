/***************************************************************************
*    Uzak Diyarlar, Anatolia tabanlý Türkçe Mud                            *
*    http://www.uzakdiyarlar.net                                           *
*    https://github.com/yelbuke/UzakDiyarlar                               *
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

int hit_roll(int level)
{
	return ( level / 2 );
}

int damage_dice_0(int level)
{
	int type, number, bonus;
	type   = level*7/4;
	number = UMIN(type/8 + 1, 5);
	type   = UMAX(2, type/number);
	bonus  = UMAX(0, level*9/4 - number*type);

	return number;
}

int damage_dice_1(int level)
{
	int type, number, bonus;
	type   = level*7/4;
	number = UMIN(type/8 + 1, 5);
	type   = UMAX(2, type/number);
	bonus  = UMAX(0, level*9/4 - number*type);

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
	int pos=4;
	switch ( number_range( 1, 4 ) )
	{
		case 1: pos = POS_SLEEPING; break;
		case 2: pos = POS_RESTING; break;
		case 3: pos = POS_SITTING; break;
		case 4: pos = POS_STANDING; break;
	}
	return pos;
}

int sex_dice(void)
{
	return number_range(1,2);
}
