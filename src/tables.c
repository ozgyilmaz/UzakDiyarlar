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

/***************************************************************************
 *     ANATOLIA 2.1 is copyright 1996-1997 Serdar BULUT, Ibrahim CANPUNAR  *
 *     ANATOLIA has been brought to you by ANATOLIA consortium		   *
 *	 Serdar BULUT {Chronos}		bulut@rorqual.cc.metu.edu.tr       *
 *	 Ibrahim Canpunar  {Asena}	canpunar@rorqual.cc.metu.edu.tr    *
 *	 Murat BICER  {KIO}		mbicer@rorqual.cc.metu.edu.tr	   *
 *	 D.Baris ACAR {Powerman}	dbacar@rorqual.cc.metu.edu.tr	   *
 *     By using this code, you have agreed to follow the terms of the      *
 *     ANATOLIA license, in the file Anatolia/anatolia.licence             *
 ***************************************************************************/

/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

/***************************************************************************
*	ROM 2.4 is copyright 1993-1995 Russ Taylor			   *
*	ROM has been brought to you by the ROM consortium		   *
*	    Russ Taylor (rtaylor@pacinfo.com)				   *
*	    Gabrielle Taylor (gtaylor@pacinfo.com)			   *
*	    Brian Moore (rom@rom.efn.org)				   *
*	By using this code, you have agreed to follow the terms of the	   *
*	ROM license, in the file Rom24/doc/rom.license			   *
***************************************************************************/

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <wchar.h>
#include <time.h>
#include "merc.h"
#include "tables.h"


/* for position */
const struct position_type position_table[] =
{
    {	L"dead",			L"dead"	},
    {	L"mortally wounded",	L"mort"	},
    {	L"incapacitated",	L"incap"	},
    {	L"stunned",		L"stun"	},
    {	L"sleeping",		L"sleep"	},
    {	L"resting",		L"rest"	},
    {	L"sitting",		L"sit"   },
    {	L"fighting",		L"fight"	},
    {	L"standing",		L"stand"	},
    {	NULL,			NULL	}
};

/* for sex */
const struct sex_type sex_table[] =
{
   {	L"none"		},
   {	L"male"		},
   {	L"female"	},
   {	L"either"	},
   {	NULL		}
};

/* for sizes */
const struct size_type size_table[] =
{
    {	L"tiny"		},
    {	L"small" 	},
    {	L"medium"	},
    {	L"large"		},
    {	L"huge", 	},
    {	L"giant" 	},
    {	L"gargantuan" 	},
    {	NULL		}
};

/* various flag tables */
const struct flag_type act_flags[] =
{
    {	L"npc",			A,	FALSE	},
    {	L"sentinel",		B,	TRUE	},
    {	L"scavenger",		C,	TRUE	},
    {	L"aggressive",		F,	TRUE	},
    {	L"stay_area",		G,	TRUE	},
    {	L"wimpy",		H,	TRUE	},
    {	L"pet",			I,	TRUE	},
    {	L"train",		J,	TRUE	},
    {	L"practice",		K,	TRUE	},
    {	L"undead",		O,	TRUE	},
    {	L"cleric",		Q,	TRUE	},
    {	L"mage",			R,	TRUE	},
    {	L"thief",		S,	TRUE	},
    {	L"warrior",		T,	TRUE	},
    {	L"noalign",		U,	TRUE	},
    {	L"nopurge",		V,	TRUE	},
    {	L"outdoors",		W,	TRUE	},
    {	L"indoors",		Y,	TRUE	},
    {	L"healer",		aa,	TRUE	},
    {	L"gain",			bb,	TRUE	},
    {	L"update_always",	cc,	TRUE	},
    {	L"changer",		dd,	TRUE	},
    {	NULL,			0,	FALSE	}
};

const struct flag_type plr_flags[] =
{
    {	L"npc",			A,	FALSE	},
    {	L"autoassist",		C,	FALSE	},
    {	L"autoexit",		D,	FALSE	},
    {	L"autoloot",		E,	FALSE	},
    {	L"autosac",		F,	FALSE	},
    {	L"autoakce",		G,	FALSE	},
    {	L"autosplit",		H,	FALSE	},
    {	L"holylight",		N,	FALSE	},
    {	L"can_loot",		P,	FALSE	},
    {	L"nosummon",		Q,	FALSE	},
    {	L"nofollow",		R,	FALSE	},
    {	L"permit",		U,	TRUE	},
    {	L"log",			W,	FALSE	},
    {	L"deny",			X,	FALSE	},
    {	L"freeze",		Y,	FALSE	},
    {	L"thief",		Z,	FALSE	},
    {	L"killer",		aa,	FALSE	},
    {	L"questor",		bb,	FALSE	},
    {	L"vampire",		cc,	FALSE	},
    {	NULL,			0,	0	}
};

const struct flag_type affect_flags[] =
{
    {	L"blind",		A,	TRUE	},
    {	L"invisible",		B,	TRUE	},
    {	L"sanctuary",		H,	TRUE	},
    {	L"faerie_fire",		I,	TRUE	},
    {	L"infrared",		J,	TRUE	},
    {	L"curse",		K,	TRUE	},
    {	L"poison",		M,	TRUE	},
    {	L"protect_evil",		N,	TRUE	},
    {	L"protect_good",		O,	TRUE	},
    {	L"sneak",		P,	TRUE	},
    {	L"hide",			Q,	TRUE	},
    {	L"sleep",		R,	TRUE	},
    {	L"charm",		S,	TRUE	},
    {	L"flying",		T,	TRUE	},
    {	L"pass_door",		U,	TRUE	},
    {	L"haste",		V,	TRUE	},
    {	L"calm",			W,	TRUE	},
    {	L"plague",		X,	TRUE	},
    {	L"weaken",		Y,	TRUE	},
    {	L"wstun",		Z,	TRUE	},
    {	L"berserk",		aa,	TRUE	},
    {	L"swim",			bb,	TRUE	},
    {	L"regeneration",		cc,	TRUE	},
    {	L"slow",			dd,	TRUE	},
    {	L"camouflage",		ee,	TRUE	},
    {	NULL,			0,	0	}
};

const struct flag_type off_flags[] =
{
    {	L"area_attack",		A,	TRUE	},
    {	L"backstab",		B,	TRUE	},
    {	L"bash",			C,	TRUE	},
    {	L"berserk",		D,	TRUE	},
    {	L"disarm",		E,	TRUE	},
    {	L"dodge",		F,	TRUE	},
    {	L"fade",			G,	TRUE	},
    {	L"fast",			H,	TRUE	},
    {	L"kick",			I,	TRUE	},
    {	L"dirt_kick",		J,	TRUE	},
    {	L"parry",		K,	TRUE	},
    {	L"rescue",		L,	TRUE	},
    {	L"tail",			M,	TRUE	},
    {	L"trip",			N,	TRUE	},
    {	L"crush",		O,	TRUE	},
    {	L"assist_all",		P,	TRUE	},
    {	L"assist_align",		Q,	TRUE	},
    {	L"assist_race",		R,	TRUE	},
    {	L"assist_players",	S,	TRUE	},
    {	L"assist_guard",		T,	TRUE	},
    {	L"assist_vnum",		U,	TRUE	},
    {	NULL,			0,	0	}
};

const struct flag_type imm_flags[] =
{
    {	L"summon",		A,	TRUE	},
    {	L"charm",		B,	TRUE	},
    {	L"magic",		C,	TRUE	},
    {	L"weapon",		D,	TRUE	},
    {	L"bash",			E,	TRUE	},
    {	L"pierce",		F,	TRUE	},
    {	L"slash",		G,	TRUE	},
    {	L"fire",			H,	TRUE	},
    {	L"cold",			I,	TRUE	},
    {	L"lightning",		J,	TRUE	},
    {	L"acid",			K,	TRUE	},
    {	L"poison",		L,	TRUE	},
    {	L"negative",		M,	TRUE	},
    {	L"holy",			N,	TRUE	},
    {	L"energy",		O,	TRUE	},
    {	L"mental",		P,	TRUE	},
    {	L"disease",		Q,	TRUE	},
    {	L"drowning",		R,	TRUE	},
    {	L"light",		S,	TRUE	},
    {	L"sound",		T,	TRUE	},
    {	L"wood",			X,	TRUE	},
    {	L"silver",		Y,	TRUE	},
    {	L"iron",			Z,	TRUE	},
    {	NULL,			0,	0	}
};

const struct flag_type form_flags[] =
{
    {	L"edible",		FORM_EDIBLE,		TRUE	},
    {	L"poison",		FORM_POISON,		TRUE	},
    {	L"magical",		FORM_MAGICAL,		TRUE	},
    {	L"instant_decay",	FORM_INSTANT_DECAY,	TRUE	},
    {	L"other",		FORM_OTHER,		TRUE	},
    {	L"animal",		FORM_ANIMAL,		TRUE	},
    {	L"sentient",		FORM_SENTIENT,		TRUE	},
    {	L"undead",		FORM_UNDEAD,		TRUE	},
    {	L"construct",		FORM_CONSTRUCT,		TRUE	},
    {	L"mist",			FORM_MIST,		TRUE	},
    {	L"intangible",		FORM_INTANGIBLE,	TRUE	},
    {	L"biped",		FORM_BIPED,		TRUE	},
    {	L"ciren",		FORM_CIREN,		TRUE	},
    {	L"insect",		FORM_INSECT,		TRUE	},
    {	L"spider",		FORM_SPIDER,		TRUE	},
    {	L"crustacean",		FORM_CRUSTACEAN,	TRUE	},
    {	L"worm",			FORM_WORM,		TRUE	},
    {	L"blob",			FORM_BLOB,		TRUE	},
    {	L"mammal",		FORM_MAMMAL,		TRUE	},
    {	L"bird",			FORM_BIRD,		TRUE	},
    {	L"reptile",		FORM_REPTILE,		TRUE	},
    {	L"snake",		FORM_SNAKE,		TRUE	},
    {	L"dragon",		FORM_DRAGON,		TRUE	},
    {	L"amphibian",		FORM_AMPHIBIAN,		TRUE	},
    {	L"fish",			FORM_FISH ,		TRUE	},
    {	L"cold_blood",		FORM_COLD_BLOOD,	TRUE	},
    {	NULL,			0,			0	}
};

const struct flag_type part_flags[] =
{
    {	L"head",			PART_HEAD,		TRUE	},
    {	L"arms",			PART_ARMS,		TRUE	},
    {	L"legs",			PART_LEGS,		TRUE	},
    {	L"heart",		PART_HEART,		TRUE	},
    {	L"brains",		PART_BRAINS,		TRUE	},
    {	L"guts",			PART_GUTS,		TRUE	},
    {	L"hands",		PART_HANDS,		TRUE	},
    {	L"feet",			PART_FEET,		TRUE	},
    {	L"fingers",		PART_FINGERS,		TRUE	},
    {	L"ear",			PART_EAR,		TRUE	},
    {	L"eye",			PART_EYE,		TRUE	},
    {	L"long_tongue",		PART_LONG_TONGUE,	TRUE	},
    {	L"eyestalks",		PART_EYESTALKS,		TRUE	},
    {	L"tentacles",		PART_TENTACLES,		TRUE	},
    {	L"fins",			PART_FINS,		TRUE	},
    {	L"wings",		PART_WINGS,		TRUE	},
    {	L"tail",			PART_TAIL,		TRUE	},
    {	L"claws",		PART_CLAWS,		TRUE	},
    {	L"fangs",		PART_FANGS,		TRUE	},
    {	L"horns",		PART_HORNS,		TRUE	},
    {	L"scales",		PART_SCALES,		TRUE	},
    {	L"tusks",		PART_TUSKS,		TRUE	},
    {	NULL,			0,			0	}
};

const struct flag_type comm_flags[] =
{
    {	L"quiet",		COMM_QUIET,		TRUE	},
    {	L"deaf",			COMM_DEAF,		TRUE	},
    {	L"nowiz",		COMM_NOWIZ,		TRUE	},
    {	L"nogossip",		COMM_NOGOSSIP,		TRUE	},
    {	L"noquestion",		COMM_NOQUESTION,	TRUE	},
    {	L"nomusic",		COMM_NOMUSIC,		TRUE	},
    {	L"noquote",		COMM_NOQUOTE,		TRUE	},
    {	L"shoutsoff",		COMM_SHOUTSOFF,		TRUE	},
    {	L"true_trust",		COMM_TRUE_TRUST,	TRUE	},
    {	L"compact",		COMM_COMPACT,		TRUE	},
    {	L"brief",		COMM_BRIEF,		TRUE	},
    {	L"prompt",		COMM_PROMPT,		TRUE	},
    {	L"combine",		COMM_COMBINE,		TRUE	},
    {	L"telnet_ga",		COMM_TELNET_GA,		TRUE	},
    {	L"show_affects",		COMM_SHOW_AFFECTS,	TRUE	},
    {	L"nograts",		COMM_NOGRATS,		TRUE	},
    {	L"noemote",		COMM_NOEMOTE,		FALSE	},
    {	L"noshout",		COMM_NOSHOUT,		FALSE	},
    {	L"notell",		COMM_NOTELL,		FALSE	},
    {	L"nochannels",		COMM_NOCHANNELS,	FALSE	},
    {	L"snoop_proof",		COMM_SNOOP_PROOF,	FALSE	},
    {	L"afk",			COMM_AFK,		TRUE	},
    {	NULL,			0,			0	}
};

/* <longname> <shortname> <cabal item> <cabal shrine> NULL */
struct cabal_type cabal_table [] =
{
  { L"None", L"None",0,0,NULL },
  { L"Yargıçlar",	L"tüze",	511,512,NULL	},
  { L"İstilacılar",	L"istila",	561,568,NULL	},
  { L"Karmaşanın efendileri", 		L"kaos",	552,554,NULL	},
  { L"Büyü sanatlarının efendileri", 	L"tılsım",	531,530,NULL 	},
  { L"Dövüş sanatlarının efendileri",	L"öfke",	541,548,NULL 	},
  { L"Şövalyeler", 		L"şövalye",	522,524,NULL 	},
  { L"Ormanların efendileri", 		L"aslan",		502,504,NULL 	},
  { L"Tüccarlar",	L"avcı",	571,573,NULL 	}
};

const   struct  prac_type    prac_table[] =
{
    {	L"none",		L"group_none",		GROUP_NONE		},
    {	L"weaponsmaster",L"group_weaponsmaster",	GROUP_WEAPONSMASTER	},
    {	L"attack",	L"group_attack",		GROUP_ATTACK		},
    {	L"beguiling",	L"group_beguiling",	GROUP_BEGUILING		},
    {	L"benedictions",	L"group_benedictions",	GROUP_BENEDICTIONS	},
    {	L"combat",	L"group_combat",		GROUP_COMBAT		},
    {	L"creation",	L"group_creation",	GROUP_CREATION		},
    {	L"curative",	L"group_curative",	GROUP_CURATIVE		},
    {	L"detection",	L"group_detection",	GROUP_DETECTION		},
    {	L"draconian",	L"group_draconian",	GROUP_DRACONIAN		},
    {	L"enchantment",	L"group_enchantment",	GROUP_ENCHANTMENT	},
    {	L"enhancement",	L"group_enhancement",	GROUP_ENHANCEMENT	},
    {	L"harmful",	L"group_harmful",	GROUP_HARMFUL		},
    {	L"healing",	L"group_healing",	GROUP_HEALING		},
    {	L"illusion",	L"group_illusion",	GROUP_ILLUSION		},
    {	L"maladictions",	L"group_maladictions",	GROUP_MALADICTIONS	},
    {	L"protective",	L"group_protective",	GROUP_PROTECTIVE	},
    {	L"transportation",L"group_transportation",GROUP_TRANSPORTATION	},
    {	L"weather",	L"group_weather",	GROUP_WEATHER		},
    {	L"fightmaster",	L"group_fightmaster",	GROUP_FIGHTMASTER	},
    {	L"suddendeath",	L"group_suddendeath",	GROUP_SUDDENDEATH	},
    {	L"meditation",	L"group_meditation",	GROUP_MEDITATION	},
    {	L"cabal",	L"group_cabal",		GROUP_CABAL		},
    {	L"defensive",	L"group_defensive",	GROUP_DEFENSIVE		},
    {	L"wizard",	L"group_wizard",		GROUP_WIZARD		},
    {	NULL,		NULL,			0			}
};

const struct yp_tip yp_tablo[] =
{
	{1,1},//level 0
	{6,30},
	{13,34},
	{26,64},
	{32,62},
	{44,81},
	{55,85},
	{67,120},
	{90,127},
	{98,150},
	{83,212},//10
	{124,168},
	{129,330},
	{150,199},
	{162,247},
	{165,250},
	{213,268},
	{205,300},
	{268,333},
	{301,438},
	{310,450},//20
	{344,470},
	{382,510},
	{416,689},
	{470 , 548},
	{522 , 600},
	{574 , 706},
	{650 , 850},
	{680 , 1098},
	{740 , 1022},
	{724 , 1050},//30
	{949 , 1066},
	{913 , 1132},
	{879 , 1362},
	{908 , 1400},
	{1150 , 1500},
	{1434 , 1520},
	{1442 , 1572},
	{1638 , 1963},
	{1743 , 2100},
	{1580 , 2200},//40
	{1800 , 2146},
	{1950 , 2350},
	{2200 , 2700},
	{2632 , 2911},
	{2900 , 3100},
	{3200 , 3400},
	{3400,4000},
	{3600,4200},
	{4000,4600},
	{4300,5000},//50
	{4600,5100},
	{4900,5300},
	{5200,5700},
	{5500,6000},
	{5800,6200},
	{6100,6600},
	{6400,7000},
	{6600,7400},
	{6800,7500},
	{7000,7800},//60
	{7250,8000},
	{7500,8250},
	{7750,8500},
	{8000,9000},
	{8250,9250},
	{8500,9500},
	{8750,9750},
	{9000,10000},
	{9250,10250},
	{9500,10500},//70
	{9850,11000},
	{10150,12000},
	{10500,12500},
	{10800,12700},
	{11100,13000},
	{11700,13500},
	{12300,13500},
	{12900,14200},
	{13300,15000},
	{13500,15200},//80
	{13900,15500},
	{14250,16000},
	{14600,16200},
	{14950,16500},
	{15300,17000},
	{15650,17500},
	{16000,18000},
	{16500,18500},
	{16800,19000},
	{17100,19500},//90
	{18000,20000},
	{18700,20700},
	{19300,21300},
	{20100,22100},
	{21000,23200},
	{21800,24000},
	{22700,24700},
	{24000,26300},
	{26000,28700},
	{28000,31000},//100
	{30000,35000},
  {32000,37000},
  {34000,39000},
  {36000,41000},
  {38000,43000},
  {40000,45000},
  {42000,47000},
  {44000,49000},
  {46000,51000},
  {48000,53000},//110
  {51000,56000},
  {54000,59000},
  {57000,62000},
  {60000,65000},
  {63000,68000},
  {66000,71000},
  {69000,74000},
  {72000,77000},
  {75000,80000},
  {78000,83000},//120
  {79000,84000},
  {80000,85000},
  {81000,86000},
  {82000,87000},
  {83000,88000},
  {84000,89000},
  {85000,90000},
  {86000,91000},
  {87000,92000},
  {88000,93000}//130
};
