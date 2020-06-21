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
#include <time.h>
#include "merc.h"
#include "tables.h"


/* for position */
const struct position_type position_table[] =
{
    {	"dead",			"dead"	},
    {	"mortally wounded",	"mort"	},
    {	"incapacitated",	"incap"	},
    {	"stunned",		"stun"	},
    {	"sleeping",		"sleep"	},
    {	"resting",		"rest"	},
    {   "sitting",		"sit"   },
    {	"fighting",		"fight"	},
    {	"standing",		"stand"	},
    {	NULL,			NULL	}
};

/* for sex */
const struct sex_type sex_table[] =
{
   {	"none"		},
   {	"male"		},
   {	"female"	},
   {	"either"	},
   {	NULL		}
};

/* for sizes */
const struct size_type size_table[] =
{
    {	"tiny"		},
    {	"small" 	},
    {	"medium"	},
    {	"large"		},
    {	"huge", 	},
    {	"giant" 	},
    {	"gargantuan" 	},
    {	NULL		}
};

/* various flag tables */
const struct flag_type act_flags[] =
{
    {	"npc",			A,	FALSE	},
    {	"sentinel",		B,	TRUE	},
    {	"scavenger",		C,	TRUE	},
    {	"aggressive",		F,	TRUE	},
    {	"stay_area",		G,	TRUE	},
    {	"wimpy",		H,	TRUE	},
    {	"pet",			I,	TRUE	},
    {	"train",		J,	TRUE	},
    {	"practice",		K,	TRUE	},
    {	"undead",		O,	TRUE	},
    {	"cleric",		Q,	TRUE	},
    {	"mage",			R,	TRUE	},
    {	"thief",		S,	TRUE	},
    {	"warrior",		T,	TRUE	},
    {	"noalign",		U,	TRUE	},
    {	"nopurge",		V,	TRUE	},
    {	"outdoors",		W,	TRUE	},
    {	"indoors",		Y,	TRUE	},
    {	"healer",		aa,	TRUE	},
    {	"gain",			bb,	TRUE	},
    {	"update_always",	cc,	TRUE	},
    {	"changer",		dd,	TRUE	},
    {	NULL,			0,	FALSE	}
};

const struct flag_type plr_flags[] =
{
    {	"npc",			A,	FALSE	},
    {	"autoassist",		C,	FALSE	},
    {	"autoexit",		D,	FALSE	},
    {	"autoloot",		E,	FALSE	},
    {	"autosac",		F,	FALSE	},
    {	"autoakce",		G,	FALSE	},
    {	"autosplit",		H,	FALSE	},
    {	"holylight",		N,	FALSE	},
    {	"can_loot",		P,	FALSE	},
    {	"nosummon",		Q,	FALSE	},
    {	"nofollow",		R,	FALSE	},
    {	"permit",		U,	TRUE	},
    {	"log",			W,	FALSE	},
    {	"deny",			X,	FALSE	},
    {	"freeze",		Y,	FALSE	},
    {	"thief",		Z,	FALSE	},
    {	"killer",		aa,	FALSE	},
    {	"questor",		bb,	FALSE	},
    {	"vampire",		cc,	FALSE	},
    {	NULL,			0,	0	}
};

const struct flag_type affect_flags[] =
{
    {	"blind",		A,	TRUE	},
    {	"invisible",		B,	TRUE	},
    {	"sanctuary",		H,	TRUE	},
    {	"faerie_fire",		I,	TRUE	},
    {	"infrared",		J,	TRUE	},
    {	"curse",		K,	TRUE	},
    {	"poison",		M,	TRUE	},
    {	"protect_evil",		N,	TRUE	},
    {	"protect_good",		O,	TRUE	},
    {	"sneak",		P,	TRUE	},
    {	"hide",			Q,	TRUE	},
    {	"sleep",		R,	TRUE	},
    {	"charm",		S,	TRUE	},
    {	"flying",		T,	TRUE	},
    {	"pass_door",		U,	TRUE	},
    {	"haste",		V,	TRUE	},
    {	"calm",			W,	TRUE	},
    {	"plague",		X,	TRUE	},
    {	"weaken",		Y,	TRUE	},
    {	"wstun",		Z,	TRUE	},
    {	"berserk",		aa,	TRUE	},
    {	"swim",			bb,	TRUE	},
    {	"regeneration",		cc,	TRUE	},
    {	"slow",			dd,	TRUE	},
    {	"camouflage",		ee,	TRUE	},
    {	NULL,			0,	0	}
};

const struct flag_type off_flags[] =
{
    {	"area_attack",		A,	TRUE	},
    {	"backstab",		B,	TRUE	},
    {	"bash",			C,	TRUE	},
    {	"berserk",		D,	TRUE	},
    {	"disarm",		E,	TRUE	},
    {	"dodge",		F,	TRUE	},
    {	"fade",			G,	TRUE	},
    {	"fast",			H,	TRUE	},
    {	"kick",			I,	TRUE	},
    {	"dirt_kick",		J,	TRUE	},
    {	"parry",		K,	TRUE	},
    {	"rescue",		L,	TRUE	},
    {	"tail",			M,	TRUE	},
    {	"trip",			N,	TRUE	},
    {	"crush",		O,	TRUE	},
    {	"assist_all",		P,	TRUE	},
    {	"assist_align",		Q,	TRUE	},
    {	"assist_race",		R,	TRUE	},
    {	"assist_players",	S,	TRUE	},
    {	"assist_guard",		T,	TRUE	},
    {	"assist_vnum",		U,	TRUE	},
    {	NULL,			0,	0	}
};

const struct flag_type imm_flags[] =
{
    {	"summon",		A,	TRUE	},
    {	"charm",		B,	TRUE	},
    {	"magic",		C,	TRUE	},
    {	"weapon",		D,	TRUE	},
    {	"bash",			E,	TRUE	},
    {	"pierce",		F,	TRUE	},
    {	"slash",		G,	TRUE	},
    {	"fire",			H,	TRUE	},
    {	"cold",			I,	TRUE	},
    {	"lightning",		J,	TRUE	},
    {	"acid",			K,	TRUE	},
    {	"poison",		L,	TRUE	},
    {	"negative",		M,	TRUE	},
    {	"holy",			N,	TRUE	},
    {	"energy",		O,	TRUE	},
    {	"mental",		P,	TRUE	},
    {	"disease",		Q,	TRUE	},
    {	"drowning",		R,	TRUE	},
    {	"light",		S,	TRUE	},
    {	"sound",		T,	TRUE	},
    {	"wood",			X,	TRUE	},
    {	"silver",		Y,	TRUE	},
    {	"iron",			Z,	TRUE	},
    {	NULL,			0,	0	}
};

const struct flag_type form_flags[] =
{
    {	"edible",		FORM_EDIBLE,		TRUE	},
    {	"poison",		FORM_POISON,		TRUE	},
    {	"magical",		FORM_MAGICAL,		TRUE	},
    {	"instant_decay",	FORM_INSTANT_DECAY,	TRUE	},
    {	"other",		FORM_OTHER,		TRUE	},
    {	"animal",		FORM_ANIMAL,		TRUE	},
    {	"sentient",		FORM_SENTIENT,		TRUE	},
    {	"undead",		FORM_UNDEAD,		TRUE	},
    {	"construct",		FORM_CONSTRUCT,		TRUE	},
    {	"mist",			FORM_MIST,		TRUE	},
    {	"intangible",		FORM_INTANGIBLE,	TRUE	},
    {	"biped",		FORM_BIPED,		TRUE	},
    {	"ciren",		FORM_CIREN,		TRUE	},
    {	"insect",		FORM_INSECT,		TRUE	},
    {	"spider",		FORM_SPIDER,		TRUE	},
    {	"crustacean",		FORM_CRUSTACEAN,	TRUE	},
    {	"worm",			FORM_WORM,		TRUE	},
    {	"blob",			FORM_BLOB,		TRUE	},
    {	"mammal",		FORM_MAMMAL,		TRUE	},
    {	"bird",			FORM_BIRD,		TRUE	},
    {	"reptile",		FORM_REPTILE,		TRUE	},
    {	"snake",		FORM_SNAKE,		TRUE	},
    {	"dragon",		FORM_DRAGON,		TRUE	},
    {	"amphibian",		FORM_AMPHIBIAN,		TRUE	},
    {	"fish",			FORM_FISH ,		TRUE	},
    {	"cold_blood",		FORM_COLD_BLOOD,	TRUE	},
    {	NULL,			0,			0	}
};

const struct flag_type part_flags[] =
{
    {	"head",			PART_HEAD,		TRUE	},
    {	"arms",			PART_ARMS,		TRUE	},
    {	"legs",			PART_LEGS,		TRUE	},
    {	"heart",		PART_HEART,		TRUE	},
    {	"brains",		PART_BRAINS,		TRUE	},
    {	"guts",			PART_GUTS,		TRUE	},
    {	"hands",		PART_HANDS,		TRUE	},
    {	"feet",			PART_FEET,		TRUE	},
    {	"fingers",		PART_FINGERS,		TRUE	},
    {	"ear",			PART_EAR,		TRUE	},
    {	"eye",			PART_EYE,		TRUE	},
    {	"long_tongue",		PART_LONG_TONGUE,	TRUE	},
    {	"eyestalks",		PART_EYESTALKS,		TRUE	},
    {	"tentacles",		PART_TENTACLES,		TRUE	},
    {	"fins",			PART_FINS,		TRUE	},
    {	"wings",		PART_WINGS,		TRUE	},
    {	"tail",			PART_TAIL,		TRUE	},
    {	"claws",		PART_CLAWS,		TRUE	},
    {	"fangs",		PART_FANGS,		TRUE	},
    {	"horns",		PART_HORNS,		TRUE	},
    {	"scales",		PART_SCALES,		TRUE	},
    {	"tusks",		PART_TUSKS,		TRUE	},
    {	NULL,			0,			0	}
};

const struct flag_type comm_flags[] =
{
    {	"quiet",		COMM_QUIET,		TRUE	},
    {   "deaf",			COMM_DEAF,		TRUE	},
    {   "nowiz",		COMM_NOWIZ,		TRUE	},
    {   "nogossip",		COMM_NOGOSSIP,		TRUE	},
    {   "noquestion",		COMM_NOQUESTION,	TRUE	},
    {   "nomusic",		COMM_NOMUSIC,		TRUE	},
    {   "noquote",		COMM_NOQUOTE,		TRUE	},
    {   "shoutsoff",		COMM_SHOUTSOFF,		TRUE	},
    {   "true_trust",		COMM_TRUE_TRUST,	TRUE	},
    {   "compact",		COMM_COMPACT,		TRUE	},
    {   "brief",		COMM_BRIEF,		TRUE	},
    {   "prompt",		COMM_PROMPT,		TRUE	},
    {   "combine",		COMM_COMBINE,		TRUE	},
    {   "telnet_ga",		COMM_TELNET_GA,		TRUE	},
    {   "show_affects",		COMM_SHOW_AFFECTS,	TRUE	},
    {   "nograts",		COMM_NOGRATS,		TRUE	},
    {   "noemote",		COMM_NOEMOTE,		FALSE	},
    {   "noshout",		COMM_NOSHOUT,		FALSE	},
    {   "notell",		COMM_NOTELL,		FALSE	},
    {   "nochannels",		COMM_NOCHANNELS,	FALSE	},
    {   "snoop_proof",		COMM_SNOOP_PROOF,	FALSE	},
    {   "afk",			COMM_AFK,		TRUE	},
    {	NULL,			0,			0	}
};

/* <longname> <shortname> <cabal item> <cabal shrine> NULL */
struct cabal_type cabal_table [] =
{
  { "None", "None",0,0,NULL },
  { "Yargýçlar",	"tüze",	511,512,NULL	},
  { "Ýstilacýlar",	"istila",	561,568,NULL	},
  { "Karmaþanýn efendileri", 		"kaos",	552,554,NULL	},
  { "Büyü sanatlarýnýn efendileri", 	"týlsým",	531,530,NULL 	},
  { "Dövüþ sanatlarýnýn efendileri",	"öfke",	541,548,NULL 	},
  { "Þövalyeler", 		"þövalye",	522,524,NULL 	},
  { "Ormanlarýn efendileri", 		"aslan",		502,504,NULL 	},
  { "Tüccarlar",	"avcý",	571,573,NULL 	}
};

const   struct  prac_type    prac_table[] =
{
    {	"none",		"group_none",		GROUP_NONE		},
    {	"weaponsmaster","group_weaponsmaster",	GROUP_WEAPONSMASTER	},
    {	"attack",	"group_attack",		GROUP_ATTACK		},
    {	"beguiling",	"group_beguiling",	GROUP_BEGUILING		},
    {	"benedictions",	"group_benedictions",	GROUP_BENEDICTIONS	},
    {	"combat",	"group_combat",		GROUP_COMBAT		},
    {	"creation",	"group_creation",	GROUP_CREATION		},
    {	"curative",	"group_curative",	GROUP_CURATIVE		},
    {	"detection",	"group_detection",	GROUP_DETECTION		},
    {	"draconian",	"group_draconian",	GROUP_DRACONIAN		},
    {	"enchantment",	"group_enchantment",	GROUP_ENCHANTMENT	},
    {	"enhancement",	"group_enhancement",	GROUP_ENHANCEMENT	},
    {	"harmful",	"group_harmful",	GROUP_HARMFUL		},
    {	"healing",	"group_healing",	GROUP_HEALING		},
    {	"illusion",	"group_illusion",	GROUP_ILLUSION		},
    {	"maladictions",	"group_maladictions",	GROUP_MALADICTIONS	},
    {	"protective",	"group_protective",	GROUP_PROTECTIVE	},
    {	"transportation","group_transportation",GROUP_TRANSPORTATION	},
    {	"weather",	"group_weather",	GROUP_WEATHER		},
    {	"fightmaster",	"group_fightmaster",	GROUP_FIGHTMASTER	},
    {	"suddendeath",	"group_suddendeath",	GROUP_SUDDENDEATH	},
    {	"meditation",	"group_meditation",	GROUP_MEDITATION	},
    {	"cabal",	"group_cabal",		GROUP_CABAL		},
    {	"defensive",	"group_defensive",	GROUP_DEFENSIVE		},
    {	"wizard",	"group_wizard",		GROUP_WIZARD		},
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
