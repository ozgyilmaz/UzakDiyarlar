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
#include "magic.h"
#include "interp.h"

/* language staff */
const struct translation_type translation_table[]=
{
        {"a",                   "e"},
        {"A",                   "E"},
        {"b",                   "c"},
        {"B",                   "C"},
        {"c",                   "d"},
        {"C",                   "D"},
        {"d",                   "f"},
        {"D",                   "F"},
        {"e",                   "i"},
        {"E",                   "I"},
        {"f",                   "g"},
        {"F",                   "G"},
        {"g",                   "h"},
        {"G",                   "H"},
        {"h",                   "j"},
        {"H",                   "J"},
        {"i",                   "o"},
        {"I",                   "O"},
        {"j",                   "k"},
        {"J",                   "K"},
        {"k",                   "l"},
        {"K",                   "L"},
        {"l",                   "m"},
        {"L",                   "M"},
        {"m",                   "n"},
        {"M",                   "N"},
        {"n",                   "p"},
        {"N",                   "P"},
        {"o",                   "u"},
        {"O",                   "U"},
        {"p",                   "q"},
        {"P",                   "Q"},
        {"q",                   "r"},
        {"Q",                   "R"},
        {"r",                   "s"},
        {"R",                   "S"},
        {"s",                   "t"},
        {"S",                   "T"},
        {"t",                   "v"},
        {"T",                   "V"},
        {"u",                   "y"},
        {"U",                   "Y"},
        {"v",                   "w"},
        {"V",                   "W"},
        {"w",                   "x"},
        {"W",                   "X"},
        {"x",                   "z"},
        {"X",                   "Z"},
        {"y",                   "a"},
        {"Y",                   "A"},
        {"z",                   "b"},
        {"Z",                   "B"},
        {"",            	"" }
};

const   struct  language_type       language_table      [ MAX_LANGUAGE ] =
{
  { "ortak",	LANG_COMMON	},
  { "insan",	LANG_HUMAN	},
  { "çora",	LANG_CORA	},
  { "cüce",	LANG_DWARVISH	},
  { "kuþ",	LANG_BIRD	},
  { "kedi",	LANG_CAT	},
  { "asura",	LANG_ASURA	},
  { "sürüngen",	LANG_SURUNGEN	},
  { "astral",	LANG_ASTRAL	},
  { "dev",	LANG_GIANT	},
  { "ejderha",	LANG_DRAGON	},
  { "su",	LANG_WATER	},
  { "su",	LANG_WATER	},
  { "köpek",	LANG_DOG	},
};

/* item type list */
const struct item_type		item_table	[]	=
{
    {	ITEM_LIGHT,	"light"		},
    {	ITEM_SCROLL,	"scroll"	},
    {	ITEM_WAND,	"wand"		},
    {   ITEM_STAFF,	"staff"		},
    {   ITEM_WEAPON,	"weapon"	},
    {   ITEM_TREASURE,	"treasure"	},
    {   ITEM_ARMOR,	"armor"		},
    {	ITEM_POTION,	"potion"	},
    {	ITEM_CLOTHING,	"clothing"	},
    {   ITEM_FURNITURE,	"furniture"	},
    {	ITEM_TRASH,	"trash"		},
    {	ITEM_CONTAINER,	"container"	},
    {	ITEM_DRINK_CON, "drink"		},
    {	ITEM_KEY,	"key"		},
    {	ITEM_FOOD,	"food"		},
    {	ITEM_MONEY,	"money"		},
    {	ITEM_BOAT,	"boat"		},
    {	ITEM_CORPSE_NPC,"npc_corpse"	},
    {	ITEM_CORPSE_PC,	"pc_corpse"	},
    {   ITEM_FOUNTAIN,	"fountain"	},
    {	ITEM_PILL,	"pill"		},
    {	ITEM_PROTECT,	"protect"	},
    {	ITEM_MAP,	"map"		},
    {	ITEM_PORTAL,	"portal"	},
    {	ITEM_WARP_STONE,"warp_stone"	},
    {	ITEM_ROOM_KEY,	"room_key"	},
    {	ITEM_GEM,	"gem"		},
    {	ITEM_JEWELRY,	"jewelry"	},
    {   ITEM_JUKEBOX,	"jukebox"	},
    {   ITEM_TATTOO,	"tattoo"	},
    {   0,		NULL		}
};


/* weapon selection table */
const	struct	weapon_type	weapon_table	[]	=
{
   { "sword",	OBJ_VNUM_SCHOOL_SWORD,	WEAPON_SWORD,	&gsn_sword	},
   { "mace",	OBJ_VNUM_SCHOOL_MACE,	WEAPON_MACE,	&gsn_mace 	},
   { "dagger",	OBJ_VNUM_SCHOOL_DAGGER,	WEAPON_DAGGER,	&gsn_dagger	},
   { "axe",	OBJ_VNUM_SCHOOL_AXE,	WEAPON_AXE,	&gsn_axe	},
   { "staff",	OBJ_VNUM_SCHOOL_STAFF,	WEAPON_SPEAR,	&gsn_spear	},
   { "flail",	OBJ_VNUM_SCHOOL_FLAIL,	WEAPON_FLAIL,	&gsn_flail	},
   { "whip",	OBJ_VNUM_SCHOOL_WHIP,	WEAPON_WHIP,	&gsn_whip	},
   { "polearm",	OBJ_VNUM_SCHOOL_POLEARM,WEAPON_POLEARM,	&gsn_polearm	},
   { "bow",	OBJ_VNUM_SCHOOL_BOW,	WEAPON_BOW,	&gsn_bow	},
   { "arrow",	OBJ_VNUM_SCHOOL_POLEARM,WEAPON_ARROW,	&gsn_arrow	},
   { "lance",	OBJ_VNUM_SCHOOL_LANCE,	WEAPON_LANCE,	&gsn_lance	},
   { NULL,	0,				0,	NULL		}
};



/* wiznet table and prototype for future flag setting */
const   struct wiznet_type      wiznet_table    []              =
{
   {    "on",           WIZ_ON,         IM },
   {    "prefix",	WIZ_PREFIX,	IM },
   {    "ticks",        WIZ_TICKS,      IM },
   {    "logins",       WIZ_LOGINS,     IM },
   {    "sites",        WIZ_SITES,      L4 },
   {    "links",        WIZ_LINKS,      L7 },
   {	"newbies",	WIZ_NEWBIE,	IM },
   {	"spam",		WIZ_SPAM,	L5 },
   {    "deaths",       WIZ_DEATHS,     IM },
   {    "resets",       WIZ_RESETS,     L4 },
   {    "mobdeaths",    WIZ_MOBDEATHS,  L4 },
   {    "flags",	WIZ_FLAGS,	L5 },
   {	"penalties",	WIZ_PENALTIES,	L5 },
   {	"saccing",	WIZ_SACCING,	L5 },
   {	"levels",	WIZ_LEVELS,	IM },
   {	"load",		WIZ_LOAD,	L2 },
   {	"restore",	WIZ_RESTORE,	L2 },
   {	"snoops",	WIZ_SNOOPS,	L2 },
   {	"switches",	WIZ_SWITCHES,	L2 },
   {	"secure",	WIZ_SECURE,	L1 },
   {	NULL,		0,		0  }
};

/* attack table */
const 	struct attack_type	attack_table	[]		=
{
    { 	"none",		"vuruþ",		-1		},  /*  0 */
    {	"slice",	"dilme", 	DAM_SLASH	},
    {   "stab",		"saplama",		DAM_PIERCE	},
    {	"slash",	"kesici",	DAM_SLASH	},
    {	"whip",		"kamçý",		DAM_SLASH	},
    {   "claw",		"pençe",		DAM_SLASH	},  /*  5 */
    {	"blast",	"patlama",	DAM_BASH	},
    {   "pound",	"çarpma",	DAM_BASH	},
    {	"crush",	"ezme",	DAM_BASH	},
    {   "grep",		"kesici",		DAM_SLASH	},
    {	"bite",		"ýsýrma",		DAM_PIERCE	},  /* 10 */
    {   "pierce",	"delici",	DAM_PIERCE	},
    {   "suction",	"emici",	DAM_BASH	},
    {	"beating",	"beating",	DAM_BASH	},
    {   "digestion",	"öðütücü",	DAM_ACID	},
    {	"charge",	"yüklenme",	DAM_BASH	},  /* 15 */
    { 	"slap",		"tokat",		DAM_BASH	},
    {	"punch",	"yumruk",	DAM_BASH	},
    {	"wrath",	"gazap",	DAM_ENERGY	},
    {	"magic",	"büyü",	DAM_ENERGY	},
    {   "divine",	"ilahi güç",	DAM_HOLY	},  /* 20 */
    {	"cleave",	"yarma",	DAM_SLASH	},
    {	"scratch",	"týrmalama",	DAM_PIERCE	},
    {   "peck",		"gaga",		DAM_PIERCE	},
    {   "peckb",	"gaga",		DAM_BASH	},
    {   "chop",		"chop",		DAM_SLASH	},  /* 25 */
    {   "sting",	"sokma",	DAM_PIERCE	},
    {   "smash",	 "smash",	DAM_BASH	},
    {   "shbite",	"þok ýsýrýþý",DAM_LIGHTNING	},
    {	"flbite",	"alev ýsýrýþý", DAM_FIRE	},
    {	"frbite",	"ayaz ýsýrýþý", DAM_COLD	},  /* 30 */
    {	"acbite",	"asit ýsýrýþý", 	DAM_ACID	},
    {	"chomp",	"çiðneme",	DAM_PIERCE	},
    {  	"drain",	"yaþam emici",	DAM_NEGATIVE	},
    {   "thrust",	"thrust",	DAM_PIERCE	},
    {   "slime",	"balçýk",	DAM_ACID	},
    {	"shock",	"þok",	DAM_LIGHTNING	},
    {   "thwack",	"thwack",	DAM_BASH	},
    {   "flame",	"alev",	DAM_FIRE	},
    {   "chill",	"ayaz",	DAM_COLD	},
    {   NULL,		NULL,		0		}
};

/* race table */
const 	struct	race_type	race_table	[]		=
{

    { {"unique","eþsiz"},	"",	FALSE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,{ "" }, {0,0,0,0,0,0 } },

    { /* 1 */
	{"human","insan"},"Ýnsan",LANG_HUMAN,	TRUE, 		SIZE_MEDIUM, 0,
	0,		0, 		0,
	0, 		0,		0,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, 0, { "identify", "steal" }, { 20, 21, 20, 21, 21 ,20}
    },

    { /* 2 */
	{"çora","çora"},"Çora",LANG_CORA,		TRUE, 		SIZE_MEDIUM, 0,
	0,		AFF_INFRARED|AFF_SNEAK,	0,
	0,		RES_CHARM,	VULN_IRON,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, 500, { "lore" , "camouflage" }, { 20, 23, 20, 22, 18, 20 }
    },

    { /* 3 */
	{"dwarf","cüce"},"Cüce",LANG_DWARVISH,	TRUE, 		SIZE_MEDIUM, 0,
	0,		AFF_INFRARED,	0,
	0,		RES_POISON|RES_DISEASE, VULN_DROWNING,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, 250, { "berserk" }, { 21, 20, 21, 18, 23, 20 }
    },

    { /* 4 */
        {"gamayun","gamayun"},"Gamayun",LANG_BIRD,        TRUE, 		SIZE_MEDIUM, 0,
        0,              AFF_FLYING,             0,
        0,              0,              0,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K, 250, { "" }, { 18, 23, 20, 24, 18 , 20}
    },

    { /* 5  */
  {"pardus","pardus"},"Pardus",LANG_CAT,	TRUE, 		SIZE_MEDIUM, 0,
  0,		AFF_INFRARED,	0,
  0,		RES_WOOD|RES_DISEASE,	0,
  A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, 250, {  "ambush", "berserk", "steal" }, { 21, 19, 20, 23, 21, 20}
    },

    {  /* 6 */
	{"asura","asura"},"Asura",LANG_ASURA,	TRUE, 		SIZE_LARGE, 0,
	0,		AFF_REGENERATION|AFF_INFRARED,	OFF_BERSERK,
 	0,	RES_CHARM|RES_BASH,	VULN_FIRE|VULN_ACID,
	A|B|H|M|V,		A|B|C|D|E|F|G|H|I|J|K|U|V, 400, { "fast healing","trance" }, { 22, 19, 19, 21, 23, 20}
    },

    { /* 7 */
      {"yeg","yeg"},"",LANG_DWARVISH,        FALSE, 		SIZE_MEDIUM, 0,
      0,                AFF_INFRARED,   0,
      0,                RES_MAGIC|RES_POISON|RES_DISEASE, VULN_DROWNING,
      A|H|M|V,          A|B|C|D|E|F|G|H|I|J|K, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    { /* 8 */
        {"börü","börü"},"", LANG_DOG,       FALSE, 		SIZE_MEDIUM, 0,
        0,              AFF_INFRARED,   OFF_TAIL,
        0,              RES_LIGHT|RES_COLD,     VULN_FIRE|VULN_DROWNING,
        A|H|M|V,        A|C|D|E|F|H|J|K|Q|U|V, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    {  /* 9 */
      {"naga","naga"},"",LANG_SURUNGEN     FALSE, 		SIZE_MEDIUM, 0,
      0,                AFF_INFRARED,   0,
      0,                RES_MAGIC|RES_POISON|RES_DISEASE, VULN_BASH,
      A|H|M|V,          A|B|C|D|E|F|G|H|I|J|K, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    { /* 10 */
        {"gnome","gnom"},"",LANG_DWARVISH,        FALSE, 		SIZE_MEDIUM, 0,
        0,              AFF_INFRARED,   0,
        0,              RES_POISON,     VULN_BASH,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    { /* 11 */
	{"githyanki","githyanki"},"",LANG_ASTRAL,	FALSE,  		SIZE_MEDIUM, 0,
	0,		AFF_INFRARED,	0,
	0, 		0,		0,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    { /* 12 */
  {"storm giant","fýrtýna devi"},"",LANG_GIANT,	FALSE, 		SIZE_GIANT, 0,
  0,		AFF_FLYING,		0,
  0,		RES_LIGHTNING,	VULN_MENTAL|VULN_COLD|VULN_FIRE,
  A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    { /* 13 */
  {"cloud giant","bulut devi"},	"",LANG_GIANT,FALSE, 		SIZE_GIANT, 0,
  0,		AFF_FLYING,		0,
  0,		RES_WEAPON,	VULN_MENTAL|VULN_LIGHTNING,
  A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    { /* 14 */
  {"fire giant","ateþ devi"},"",LANG_GIANT,	FALSE, 		SIZE_GIANT, 0,
  0,		0,		0,
  0,		RES_FIRE,	VULN_MENTAL|VULN_LIGHTNING|VULN_COLD,
  A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    { /* 15 */
  {"frost giant","buz devi"},"",LANG_GIANT,	FALSE, 		SIZE_GIANT, 0,
  0,		0,		0,
  0,		RES_COLD,	VULN_MENTAL|VULN_LIGHTNING|VULN_FIRE,
  A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    { /* 16 */
        {"black dragon","kara ejderha"},"",LANG_DRAGON, FALSE,  	SIZE_GARGANTUAN, 0,
	0, 		AFF_INFRARED|AFF_FLYING,	0,
	0,		RES_BASH|RES_CHARM|RES_ACID,	VULN_PIERCE,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    { /* 17 */
        {"blue dragon","mavi ejderha"}, "",LANG_DRAGON,	FALSE, 		SIZE_GARGANTUAN, 0,
	0, 		AFF_INFRARED|AFF_FLYING,	0,
	0,		RES_BASH|RES_CHARM|RES_LIGHTNING,
	VULN_PIERCE,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    { /* 18 */
        {"green dragon","yeþil ejderha"},"",LANG_DRAGON, FALSE, 		SIZE_GARGANTUAN, 0,
	0, 		AFF_INFRARED|AFF_FLYING,	0,
	0,		RES_BASH|RES_CHARM|RES_POISON,	VULN_PIERCE,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    { /* 19 */
        {"red dragon","kýzýl ejderha"},"", LANG_DRAGON,	FALSE, 		SIZE_GARGANTUAN, 0,
	0, 		AFF_INFRARED|AFF_FLYING,	0,
	0,		RES_BASH|RES_CHARM|RES_FIRE,	VULN_PIERCE,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    { /* 20 */
        {"white dragon","ak ejderha"},"",LANG_DRAGON, FALSE, 		SIZE_GARGANTUAN, 0,
	0, 		AFF_INFRARED|AFF_FLYING,	0,
	0,		RES_BASH|RES_CHARM|RES_COLD,	VULN_PIERCE,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    { /* 21 */
        {"brass dragon","pirinç ejderha"},"",LANG_DRAGON, FALSE, 		SIZE_GARGANTUAN, 0,
	0, 		AFF_INFRARED|AFF_FLYING,	0,
	0,		RES_BASH|RES_CHARM|RES_FIRE,	VULN_PIERCE,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    { /* 22 */
        {"gold dragon","altýn ejderha"},"",LANG_DRAGON, 	FALSE, 		SIZE_GARGANTUAN, 0,
	0, 		AFF_INFRARED|AFF_FLYING,	0,
	0,		RES_BASH|RES_CHARM|RES_FIRE|RES_POISON,
	VULN_PIERCE,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    { /* 23 */
        {"silver dragon","gümüþ ejderha"},"",LANG_DRAGON,FALSE, 		SIZE_GARGANTUAN, 0,
	0, 		AFF_INFRARED|AFF_FLYING,	0,
	0,		RES_BASH|RES_CHARM|RES_COLD,	VULN_PIERCE,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    { /* 24 */
        {"bronze dragon","tunç ejderha"},"",LANG_DRAGON,FALSE, 		SIZE_GARGANTUAN, 0,
	0, 		AFF_INFRARED|AFF_FLYING,	0,
	0,		RES_BASH|RES_CHARM|RES_LIGHTNING,
	VULN_PIERCE,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    { /* 25 */
        {"copper dragon","bakýr ejderha"},"",LANG_DRAGON,FALSE, 		SIZE_GARGANTUAN, 0,
	0, 		AFF_INFRARED|AFF_FLYING,	0,
	0,		RES_BASH|RES_CHARM|RES_ACID,	VULN_PIERCE,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    { /* 26 */
	{"bat","yarasa"},"",LANG_BIRD,		FALSE, 		SIZE_TINY, 0,
	0,		AFF_FLYING/*|AFF_DARK_VISION*/,	OFF_DODGE|OFF_FAST,
	0,		0,		VULN_LIGHT,
	A|G|V,		A|C|D|E|F|H|J|K|P, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    { /* 27 */
	{"bear","ayý"},	"",	LANG_DOG, FALSE, 		SIZE_LARGE, 0,
	0,		0,		OFF_CRUSH|OFF_DISARM|OFF_BERSERK,
	0,		RES_BASH|RES_COLD,	0,
	A|G|V,		A|B|C|D|E|F|H|J|K|U|V, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    {  /* 28 */
	{"cat","kedi"},"",LANG_CAT,		FALSE, 		SIZE_SMALL, 0,
	0,		/*AFF_DARK_VISION*/0,	OFF_FAST|OFF_DODGE,
	0,		0,		0,
	A|G|V,		A|C|D|E|F|H|J|K|Q|U|V, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    {  /* 29 */
	{"centipede","kýrkayak"},"",LANG_SURUNGEN,	FALSE, 		SIZE_TINY, 0,
	0,		/*AFF_DARK_VISION*/0,	0,
	0,		RES_PIERCE|RES_COLD,	VULN_BASH,
 	A|B|G|O,		A|C|K, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    {  /* 30 */
	{"dog","köpek"},"",LANG_DOG,		FALSE, 		SIZE_SMALL, 0,
	0,		0,		OFF_FAST,
	0,		0,		0,
	A|G|V,		A|C|D|E|F|H|J|K|U|V, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    { /* 31 */
	{"doll","kukla"},"",LANG_COMMON		FALSE, 		SIZE_SMALL, 0,
	0,		0,		0,
	IMM_COLD|IMM_POISON|IMM_HOLY|IMM_NEGATIVE|IMM_MENTAL|IMM_DISEASE
	|IMM_DROWNING,	RES_BASH|RES_LIGHT,
	VULN_SLASH|VULN_FIRE|VULN_ACID|VULN_LIGHTNING|VULN_ENERGY,
	E|J|M|cc,	A|B|C|G|H|K, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    { /* 32 */
	{"fido","enik"},"",LANG_DOG,		FALSE, 		SIZE_SMALL, 0,
	0,		0,		OFF_DODGE|ASSIST_RACE,
	0,		0,			VULN_MAGIC,
	A|B|G|V,	A|C|D|E|F|H|J|K|Q|V, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    { /* 33 */
	{"fox","tilki"},"",LANG_DOG,		FALSE, 		SIZE_SMALL, 0,
	0,		/*AFF_DARK_VISION*/0,	OFF_FAST|OFF_DODGE,
	0,		0,		0,
	A|G|V,		A|C|D|E|F|H|J|K|Q|V, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    { /* 34 */
	{"goblin","gulyabani"},"",LANG_CORA,	FALSE, 		SIZE_MEDIUM, 0,
	0,		AFF_INFRARED,	0,
	0,		RES_DISEASE,	VULN_MAGIC,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    { /* 35 */
	{"hobgoblin","ifrit"},"",LANG_CORA,	FALSE, 		SIZE_MEDIUM, 0,
	0,		AFF_INFRARED,	0,
	0,		RES_DISEASE|RES_POISON,	0,
	A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K|Y, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    {  /* 36 */
	{"kobold","argun"},"",LANG_CORA,	FALSE, 		SIZE_MEDIUM, 0,
	0,		AFF_INFRARED,	0,
	0,		RES_POISON,	VULN_MAGIC,
	A|B|H|M|V,	A|B|C|D|E|F|G|H|I|J|K|Q, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    {  /* 37 */
	{"lizard","kertenkele"},"",LANG_SURUNGEN,	FALSE, 		SIZE_SMALL, 0,
	0,		0,		0,
	0,		RES_POISON,	VULN_COLD,
	A|G|X|cc,	A|C|D|E|F|H|K|Q|V, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    {  /* 38 */
	{"modron","geme"},"",LANG_COMMON,	FALSE, 		SIZE_MEDIUM, 0,
	0,		AFF_INFRARED,	ASSIST_RACE|ASSIST_ALIGN,
	IMM_CHARM|IMM_DISEASE|IMM_MENTAL|IMM_HOLY|IMM_NEGATIVE,
			RES_FIRE|RES_COLD|RES_ACID,	0,
	H,		A|B|C|G|H|J|K, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    {  /* 39 */
	{"orc","ork"},"",LANG_CORA,		FALSE, 		SIZE_MEDIUM, 0,
	0,		AFF_INFRARED,	0,
	0,		RES_DISEASE,	VULN_LIGHT,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    {  /* 40 */
	{"pig","domuz"},"",	LANG_DOG,	FALSE, 		SIZE_MEDIUM, 0,
	0,		0,		0,
	0,		0,		0,
	A|G|V,	 	A|C|D|E|F|H|J|K, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    { /* 41 */
	{"rabbit","tavþan"},"",LANG_DOG,	FALSE, 		SIZE_SMALL, 0,
	0,		0,		OFF_DODGE|OFF_FAST,
	0,		0,		0,
	A|G|V,		A|C|D|E|F|H|J|K, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    { /* 42 */
	{"school monster","okul canavarý"},"",LANG_HUMAN,	FALSE, 		SIZE_MEDIUM, 0,
	ACT_NOALIGN,		0,		0,
	IMM_CHARM|IMM_SUMMON,	0,		VULN_MAGIC,
	A|M|V,		A|B|C|D|E|F|H|J|K|Q|U, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    {  /* 43 */
	{"snake","yýlan"},"",LANG_SURUNGEN,	FALSE, 		SIZE_SMALL, 0,
	0,		0,		0,
	0,		RES_POISON,	VULN_COLD,
	A|G|X|Y|cc,	A|D|E|F|K|L|Q|V|X, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    {  /* 44 */
	{"song bird","serçe"},"",LANG_BIRD,	FALSE, 		SIZE_TINY, 0,
	0,		AFF_FLYING,		OFF_FAST|OFF_DODGE,
	0,		0,		0,
	A|G|W,		A|C|D|E|F|H|K|P, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    {  /* 45 */
	{"water fowl","ördek"},"",LANG_WATER,	FALSE, 		SIZE_SMALL, 0,
	0,		AFF_SWIM|AFF_FLYING,	0,
	0,		RES_DROWNING,		0,
	A|G|W,		A|C|D|E|F|H|K|P, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    {  /* 46 */
	{"wolf","kurt"},"",LANG_DOG,		FALSE, 		SIZE_MEDIUM, 0,
	0,		/*AFF_DARK_VISION*/0,	OFF_FAST|OFF_DODGE,
	0,		0,		0,
	A|G|V,		A|C|D|E|F|J|K|Q|V, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    {  /* 47 */
	{"wyvern","yelbüke"},"",LANG_DRAGON,	FALSE, 		SIZE_HUGE, DETECT_INVIS|DETECT_HIDDEN,
	0,		AFF_FLYING,	OFF_BASH|OFF_FAST|OFF_DODGE,
	IMM_POISON,	0,	VULN_LIGHT,
	A|B|G|Z,		A|C|D|E|F|H|J|K|Q|V|X, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    { /* 48 */
        {"dragon","ejderha"},"", LANG_DRAGON,	FALSE,		SIZE_GARGANTUAN, 0,
	0, 		AFF_INFRARED|AFF_FLYING,	0,
	0,		RES_FIRE|RES_BASH|RES_CHARM,	VULN_PIERCE|VULN_COLD,
	A|H|Z,		A|C|D|E|F|G|H|I|J|K|P|Q|U|V|X, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    { /* 49 */
	{"giant","dev"},"",LANG_GIANT,	FALSE, 		SIZE_GIANT, 0,
	0,		0,		0,
	0,		RES_FIRE|RES_COLD,	VULN_MENTAL|VULN_LIGHTNING,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    { /* 50 */
	{"golem","golem"},"",LANG_HUMAN,	FALSE, 		SIZE_LARGE, 0,
	0,		0,		0,
	IMM_POISON|IMM_DISEASE,		RES_WEAPON,	VULN_MENTAL|VULN_MAGIC,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    { /* 51 */
	{"undead","hortlak"},"",LANG_ASTRAL,	FALSE, SIZE_MEDIUM,
	DETECT_INVIS|DETECT_HIDDEN|DETECT_GOOD|DETECT_EVIL,
	0,		AFF_PASS_DOOR|AFF_INFRARED,	0,
	IMM_DROWNING|IMM_DISEASE|IMM_POISON,	RES_NEGATIVE|RES_ENERGY,
	VULN_SILVER|VULN_HOLY|VULN_LIGHT,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    { /* 52 */
	{"drow","rayel"},	"",LANG_CORA,	FALSE,		SIZE_MEDIUM, 0,
	0,		AFF_INFRARED,	0,
	0,		RES_CHARM,		VULN_IRON,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    { /* 53 */
	{"draconian","luu"},"",LANG_DRAGON,	FALSE,		SIZE_LARGE, 0,
	0,		0,		0,
	0,		RES_SLASH|RES_POISON,	VULN_COLD,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    { /* 54 */
	{"sprite","peri"},"",LANG_CORA,	FALSE,		SIZE_SMALL, DETECT_EVIL,
	0,		AFF_FLYING,		G,
	0,		0,			VULN_COLD|VULN_WEAPON,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    { /* 55 */
	{"aarakocra","kerkes"},"",LANG_BIRD,	FALSE,		SIZE_MEDIUM, 0,
	ACT_SCAVENGER,	AFF_FLYING,	OFF_FAST,
	0,		0,		0,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K|P, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    { /*  56 */
	{"beholder","çokgöz"},"",LANG_COMMON,	FALSE,		SIZE_MEDIUM, 0,
	0,		AFF_REGENERATION,		0,
	0,		RES_MAGIC,		0,
	A|H|M|V,	A|E|K, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    {  /* 57 */
	{"tiger","kaplan"},"",LANG_CAT,	FALSE,		SIZE_MEDIUM, 0,
	0,	/*AFF_DARK_VISION*/0, OFF_FAST|OFF_DODGE,
	0,		RES_WEAPON,		0,
	A|G|V,		A|C|D|E|F|H|J|K|Q|U|V, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    {  /* 58 */
	{"lion","aslan"},"",LANG_CAT,		FALSE,		SIZE_MEDIUM, 0,
	ACT_SENTINEL,	/*AFF_DARK_VISION*/0,	OFF_FAST|OFF_DODGE,
	0,		RES_WEAPON,		0,
	A|G|V,		A|C|D|E|F|H|J|K|Q|U|V, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    {  /* 59 */
	{"death knight","aldacý"},"",LANG_HUMAN,	FALSE,		SIZE_MEDIUM, 0,
	ACT_UNDEAD,	0, 	OFF_PARRY|OFF_DISARM,
	IMM_DISEASE|IMM_POISON|IMM_DROWNING,	RES_FIRE|RES_NEGATIVE,
	VULN_HOLY,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    { /* 60 */
    {"dracolich","alavan"}, "",LANG_DRAGON,	FALSE, 		SIZE_LARGE, 0,
	0, 		AFF_INFRARED|AFF_FLYING,	0,
	IMM_CHARM|IMM_LIGHTNING|IMM_COLD,	RES_FIRE|RES_BASH,
	VULN_PIERCE|VULN_COLD,
	A|H|Z,		A|C|D|E|F|G|H|I|J|K|P|Q|U|V|X, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    {  /* 61 */
	{"air elemental","hava uþaðý"},"",LANG_ASTRAL,FALSE,		SIZE_HUGE, 0,
	0,		0,		OFF_FADE,
	0,		RES_WEAPON|RES_SUMMON,		0,
	A|H|Z,		A|D, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    {  /* 62 */
	{"earth elemental","toprak uþaðý"},"",LANG_ASTRAL,	FALSE,		SIZE_HUGE, 0,
	0,		0,	OFF_FADE,
	0,		RES_WEAPON|RES_SUMMON,		0,
	A|H|Z,		A|D, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    {  /* 63 */
	{"fire elemental","ateþ uþaðý"},"",LANG_ASTRAL,FALSE,		SIZE_HUGE, 0,
	0,		0,	OFF_FADE,
	IMM_FIRE,		RES_WEAPON,		VULN_COLD,
	A|H|Z,		A|D, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    {  /* 64 */
	{"water elemental","su uþaðý"},"",LANG_ASTRAL,	FALSE,		SIZE_HUGE, 0,
	0,		0,	OFF_FADE,
	0,		RES_WEAPON|RES_FIRE|RES_COLD,	0,
	A|H|Z,		A|D, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    {  /* 65 */
	{"fish","balýk"},	"",LANG_WATER,		FALSE,		SIZE_TINY, 0,
	0,		AFF_SWIM,	0,
	0,		0,		VULN_COLD|VULN_FIRE,
	A|H|Z,		A|D|E|F|J|K|O|X, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    {  /* 66 */
	{"gargoyle","togrýl"},"",LANG_HUMAN,	FALSE,		SIZE_MEDIUM, 0,
	ACT_AGGRESSIVE,	AFF_FLYING,	OFF_FAST,
	0,		RES_WEAPON,	VULN_COLD|VULN_FIRE,
	A|G|W,		A|C|D|E|F|H|K|P, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    {  /* 67 */
	{"ghost","hayalet"},"",LANG_ASTRAL,	FALSE,		SIZE_MEDIUM, 0,
	ACT_UNDEAD,	AFF_FLYING,	OFF_FADE,
	IMM_DROWNING|IMM_NEGATIVE|IMM_DISEASE|IMM_POISON,
	RES_WEAPON, VULN_HOLY,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    {  /* 68 */
	{"ghoul","gul"},"",LANG_HUMAN,	FALSE,		SIZE_MEDIUM, 0,
	0,		0,		0,
	IMM_DISEASE,	RES_CHARM,	VULN_MAGIC,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    {  /* 69 */
	{"gnoll","arböri"},	"",LANG_DWARVISH,FALSE,		SIZE_MEDIUM, DETECT_INVIS|DETECT_HIDDEN,
	0,		0,		OFF_DISARM,
	0,		0,		0,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    {  /* 70 */
	{"harpy","harpi"},"",	LANG_BIRD,FALSE,		SIZE_MEDIUM, DETECT_INVIS|DETECT_HIDDEN,
	0,		AFF_FLYING,		OFF_DISARM,
	0,		RES_CHARM,		0,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K|P, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    {  /* 71 */
	{"lamia","lamia"},"",LANG_HUMAN,	FALSE,		SIZE_MEDIUM, 0,
	ACT_AGGRESSIVE,	0,		OFF_FAST|OFF_DODGE,
	0,		RES_CHARM,		0,
	A|G|V,		A|B|C|D|E|F|G|H|I|J|K|Q|U, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    {  /* 72 */
	{"lich","yelviçin"},"",LANG_ASTRAL,	FALSE,		SIZE_MEDIUM, 0,
	0,		0,		OFF_FAST|OFF_DODGE,
	IMM_POISON|IMM_DISEASE|IMM_CHARM|IMM_LIGHTNING|IMM_COLD,
	RES_WEAPON|RES_FIRE,	VULN_HOLY,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    {  /* 73 */
	{"minotaur","bogra"},"",LANG_HUMAN,	FALSE,		SIZE_LARGE, 0,
	0,		0,		OFF_CRUSH|OFF_FAST|OFF_BASH,
	0,		0,		0,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K|W, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    {  /* 74 */
	{"ogre","ogre"},"",LANG_DWARVISH,		FALSE,		SIZE_LARGE, 0,
	0,		AFF_INFRARED,	0,
	0,		RES_DISEASE,	0,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    {  /* 75 */
	{"zombie","hortlak"},	"",LANG_ASTRAL,	FALSE,		SIZE_MEDIUM, 0,
	0,		AFF_SLOW,	0,
	IMM_MAGIC|IMM_POISON|IMM_CHARM,	RES_COLD, VULN_WEAPON,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    {  /* 76 */
	{"horse","at"},	"",LANG_DOG,FALSE, 		SIZE_LARGE, 0,
	0,		0,		OFF_FAST|OFF_DODGE,
	0,		0,		0,
	A|G|V,		A|C|D|E|F|J|K|Q, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    {  /* 77 */
	{"unique","eþsiz"},"",0,	FALSE,		0, 0,
	0,		0,		0,
	0,		0,		0,
	0,		0, 0, { "" }, {20, 20, 20, 20, 20, 20 }
    },

    {
	{NULL,NULL}, NULL,0,0, 0, 0, 0, 0, 0, 0, {NULL}, {0, 0, 0, 0, 0, 0 }
    }
};


/*
 * Class table.
 */
const	struct	class_type	class_table	[MAX_CLASS]	=
{

    {
        {"invoker","gan"}, "Gan",  STAT_INT,  OBJ_VNUM_SCHOOL_DAGGER,
        { 3018, 0, 0, 0, 0, 0 },  75,  18, 6,  60,  100, TRUE,
        0,{0,3,0,0,0,5},CR_ALL
    },

    {
	{"cleric","ermiþ"}, "Erm",  STAT_WIS,  OBJ_VNUM_SCHOOL_MACE,
	{ 3003, 9619, 9619, 9619, 9619, 9619},  75,  18, 2,  70, 80, TRUE,
	0,{0,0,3,0,0,5},CR_ALL
    },

    {
	{"thief","hýrsýz"}, "Hýr",  STAT_DEX,  OBJ_VNUM_SCHOOL_DAGGER,
	{ 3028, 9639, 9639, 9639, 9639, 9639 },  75,  18,  -4, 100, 50, FALSE,
        0,{0,0,0,3,0,5},CR_ALL
    },

    {
	{"warrior","savaþçý"}, "Sav",  STAT_STR,  OBJ_VNUM_SCHOOL_SWORD,
	{ 3022, 9633, 9633, 9633, 9633, 9633 },  75,  18,  -10, 100,50,FALSE,
	0,{3,0,0,0,1,5},CR_ALL
    },

    {
	{"paladin","adbolar"}, "Adb",   STAT_STR,  OBJ_VNUM_SCHOOL_SWORD,
	{ 3063, 0, 0, 0, 0, 0 },  75,  18,  -6,  80, 70, TRUE,
	400,{2,0,1,0,2,5},CR_GOOD
    },

    {
	{"anti-paladin","kembolar"}, "Kmb",  STAT_STR,  OBJ_VNUM_SCHOOL_SWORD,
	{ 3061, 0, 0, 0, 0, 0 },  75,  18,  -6,  80, 70, TRUE,
	300,{2,1,0,1,1,5},CR_EVIL
    },

    {
	{"ninja","ninja"}, "Nin",  STAT_DEX,  OBJ_VNUM_SCHOOL_DAGGER,
	{ 3062, 0, 0, 0, 0, 0 },  75,  18,  -10,  90, 60, FALSE,
	300,{1,0,0,2,0,5},CR_ALL
    },

    {
	{"ranger","korucu"}, "Krc",  STAT_STR,  OBJ_VNUM_SCHOOL_SWORD,
	{ 6156, 0, 0, 0, 0, 0 },  75,  18,  -8,  90, 60, FALSE,
	200,{1,0,0,1,1,5},CR_ALL
    },

    {
        {"transmuter","biçimci"}, "Biç",  STAT_INT,  OBJ_VNUM_SCHOOL_DAGGER,
        { 3073, 0, 0, 0, 0, 0 },  75,  18, 2,  60,  100, TRUE,
        0,{0,3,0,0,0,5},CR_ALL
    },

    {
        {"samurai","samuray"}, "Sam",  STAT_STR,  OBJ_VNUM_SCHOOL_SWORD,
        { 3075, 9633, 0, 0, 0, 0 },  75,  18,  -10,  100,60,FALSE,
        500,{1,1,1,0,1,5},CR_ALL
    },

   {
      {  "vampire","vampir"}, "Vam", STAT_INT,  OBJ_VNUM_SCHOOL_DAGGER,
        { 5395, 5396, 0, 0, 0, 0 },  75,  18, -6,  80,  70, TRUE,
        300,{1,2,0,0,0,5},CR_EVIL
        },

   {
        {"necromancer","karakam"}, "Kam",  STAT_INT,  OBJ_VNUM_SCHOOL_DAGGER,
        { 5393, 5394, 0, 0, 0, 0 },  75,  18, 6,  60,  100, TRUE,
        200,{0,2,1,0,0,5},CR_EVIL
   },

   {
        {"elementalist","ögeci"}, "Öge",  STAT_INT,  OBJ_VNUM_SCHOOL_DAGGER,
        { 9618, 0, 0, 0, 0, 0 },  75,  18, 6,  60,  100, TRUE,
        0,{0,3,0,0,0,5},CR_ALL
   }


};



/*
 * Titles. mage=100
 */
 const char *			title_table	[MAX_CLASS][MAX_LEVEL+1]=
 {
     {
       "Man",

       "Büyü Çýraðý"		,
       "Büyü Öðrencisi"	,
        "Büyü Asistaný"	,
        "Büyü Araþtýran"		,
        "Sihirbaz"	,

        "Büyü Katibi"	,
        "Falcý"		,
        "Arif"		,
        "Ýlüzyonist"		,
        "Tövbekar"		,

       "Duacý"		,
        "Duacý"		,
        "Büyü Yapýcý"		,
        "Büyü Yapýcý"		,
        "Hokkabaz"		,

        "Hokkabaz"		,
        "Büyücü"		,
        "Büyücü"		,
        "Oluþturan"     ,
        "Oluþturan"       ,

        "Alim"		,
        "Alim"		,
        "Büyü Ustasý"			,
        "Büyü Ustasý"			,
        "Büyü Alimi"		,

        "Büyü Alimi"		,
        "Savaþçý Arif"		,
        "Savaþçý Arif"		,
        "Durendiþ"	,
        "Durendiþ"	,

        "Kýdemli Büyücü"		,
        "Kýdemli Büyücü"		,
        "Büyük Büyücü"		,
        "Büyük Büyücü"	,
        "Yüce Büyücü"		,

        "Yüce Büyücü"	,
        "Golem Efendisi"		,
        "Golem Efendisi"	,
        "Yüce Golem Efendisi"		,
        "Yüce Golem Efendisi"	,

 	 "Taþ Efendisi"	,
 	 "Taþ Efendisi"	,
 	 "Ýksir Ustasý"	,
 	 "Ýksir Ustasý"	,
 	 "Parþömen Ustasý"	,

 	 "Parþömen Ustasý"	,
 	 "Sopa Ustasý"	,
 	 "Sopa Ustasý"		,
 	 "Deðnek Ustasý"	,
 	 "Deðnek Ustasý"		,

 	 "Ýblis Çaðýran"	,
 	 "Ýblis Çaðýran"		,
 	 "Yüce Ýblis Çaðýran"	,
 	 "Yüce Ýblis Çaðýran",
 	 "Ejderha Efendisi"		,

 	 "Ejderha Efendisi"		,
 	 "Ejderha Efendisi"	,
 	 "Ejderha Efendisi",
        "Karýþým Efendisi",
        "Karýþým Efendisi",

        "Büyülerin Efendisi",
        "Büyülerin Efendisi",
        "Büyüleme Efendisi",
        "Büyüleme Efendisi",
        "Çaðrý Ustasý",

        "Çaðrý Ustasý",
        "Büyük Kahin",
        "Büyük Kahin",
        "Deðiþim Ustasý",
        "Deðiþim Ustasý",

        "Fiþek Efendisi",
        "Fiþek Efendisi",
        "Ölülerin Efendisi",
        "Ölülerin Efendisi",
        "Simyacý",

        "Simyacý",
        "Bað Ustasý",
        "Bað Ustasý",
        "Söz Efendisi",
        "Söz Efendisi",

        "Geçmiþe hükmeden",
        "Geçmiþe hükmeden",
        "Bugüne hükmeden",
        "Bugüne hükmeden",
        "Geleceðe hükmeden",

        "Geleceðe hükmeden",
        "Baþ Büyücü",
        "Baþ Büyücü",
        "Bilinmeyen",
        "Bilinmeyen",

 	 "Kahraman"		,
 	 "Melek"		,
 	 "Melek"		,
 	 "Elçi"		,
 	 "Ölümsüz"		,
 	"Yüce"		,
 	"Yüce",
 	"Yüce",
 	"Yüce",
 	 "Tek"	},

     {
 	  "Man"			 ,

 	"Ýnanan"		 ,
 	"Yardýmcý"		 ,
 	"Hizmetli"		 ,
 	"Çýrak"			 ,
 	"Misyoner"		 ,

 	"Usta"		 ,
 	"Diyakoz"		 ,
 	"Önder"			 ,
 	"Üstat"		 ,
 	"Orta Elçi"			 ,

 	"Dindar"			 ,
 	"Dindar"			 ,
 	"Yükselen"		 ,
 	"Yükselen"			 ,
 	"Din Adamý"		 ,

 	"Din Adamý"			 ,
 	"Keþiþ"			 ,
 	"Keþiþ"			 ,
 	"Ýyileþtiren"			 ,
 	"Ýyileþtiren"		 ,

 	"Vaiz"		 ,
 	"Vaiz"		 ,
 	"Açýklayan"		 ,
 	"Açýklayan"			 ,
 	"Duyan"		 ,

 	"Duyan"		 ,
 	"Gören"	 ,
 	"Gören" ,
 	"Bilen"		 ,
 	"Bilen"		 ,

 	"Bilge"	 ,
 	"Bilge"		 ,
 	"Arif"		 ,
 	"Arif"		 ,
 	"Arif"	 ,

 	"Arif"		 ,
 	"Ýblis Katili"		 ,
 	"Ýblis Katili"			 ,
 	"Yüce Ýblis Katili"		 ,
 	"Yüce Ýblis Katili"		 ,

 	"Denizlerin Efendisi"		 ,
 	"Denizlerin Efendisi"		 ,
 	"Topraðýn Efendisi" ,
 	"Topraðýn Efendisi"		 ,
 	"Havanýn Efendisi"		 ,

 	"Havanýn Efendisi"		 ,
 	"Lokman Ruhu"	 ,
 	"Lokman Ruhu"	 ,
 	"Cennet Bekçisi"	 ,
 	"Cennet Bekçisi"	 ,

 	"Iþýyan" ,
 	"Iþýyan" ,
 	"Kutsal" ,
 	"Kutsal" ,
 	"Ýlahi" ,

 	"Ýlahi" ,
 	"Aziz" ,
 	"Aziz" ,
 	"Köklerin Efendisi" ,
 	"Köklerin Efendisi" ,

 	"Suyun Efendisi" ,
 	"Suyun Efendisi" ,
 	"Ateþin Efendisi" ,
 	"Ateþin Efendisi" ,
 	"Buzun Efendisi" ,

 	"Buzun Efendisi" ,
 	"Rüzgarýn Efendisi" ,
 	"Rüzgarýn Efendisi" ,
 	"Fýrtýna Efendisi" ,
 	"Fýrtýna Efendisi" ,

 	"Topraða Hükmeden" ,
 	"Topraða Hükmeden" ,
 	"Denize Hükmeden" ,
 	"Denize Hükmeden" ,
 	"Zamana Hükmeden" ,

 	"Zamana Hükmeden" ,
 	"Kutsal Ruh" ,
 	"Kutsal Ruh" ,
 	"Kutsal Bilge" ,
 	"Kutsal Bilge" ,

 	"Ölümsüz Elçisi"		 ,
 	"Ölümsüz Elçisi"	 ,
 	"Ölümsüz Elçisi"	 ,
 	"Ölümsüz Elçisi"	 ,
 	"Ölümsüz Elçisi"	 ,

 	"Ölümsüz Elçisi"	 ,
 	"Ölümsüz Elçisi"	 ,
 	"Ölümsüz Elçisi"	 ,
 	"Kutsanmýþ Olan" ,
 	"Kutsanmýþ Olan" ,

 	"Kahraman"		,
 	 "Melek"		,
 	 "Melek"		,
 	 "Elçi"		,
 	 "Ölümsüz"		,
 	"Yüce"		,
 	"Yüce",
 	"Yüce",
 	"Yüce",
 	 "Tek"},

     {
 	  "Man"			 ,

 		  "Çocuk"			 ,
 		  "Sessiz Adým"		 ,
 		  "Aþýran"			 ,
 		  "Yan Kesici"		 ,
 		  "Süzülen"		 ,

 		 "Kýstýran"	 ,
 		  "Cepçi"	 ,
 		  "Kapkaççý"	 ,
 		  "Dolandýrýcý"	 ,
 		  "Haydut"		 ,

 		 "Soyguncu" ,
 		  "Soyguncu" ,
 		  "Hilebaz"		 ,
 		  "Hilebaz"	 ,
 		  "Eþkiya"		 ,

 		 "Eþkiya"	 ,
 		  "Dessas"	 ,
 		  "Dessas"	 ,
 		  "Hýrsýz"	 ,
 		  "Hýrsýz"	 ,

 		  "Keskin Býçak" ,
 		  "Keskin Býçak" ,
 		  "Hýzlý Býçak"	 ,
 		  "Hýzlý Býçak"	 ,
 		  "Katil"		 ,

 		  "Katil"			 ,
 		  "Kanunsuz"		 ,
 		  "Kanunsuz"		 ,
 		  "Gýrtlak Kesen"			 ,
 		  "Gýrtlak Kesen"			 ,

 		  "Ýftiracý"	 ,
 		  "Ýftiracý"	 ,
 		  "Muhbir"	 ,
 		  "Muhbir"	 ,
 		  "Hafiye"	 ,

 		  "Hafiye"	 ,
 		  "Suikastçi" ,
 		  "Suikastçi" ,
 		  "Büyük Suikastçi" ,
 		  "Büyük Suikastçi" ,

 		 "Görüþ Ustasý"	 ,
 		  "Görüþ Ustasý"	 ,
 		  "Duyuþ Ustasý" ,
 		  "Duyuþ Ustasý" ,
 		  "Koku Ustasý"	 ,

    		 "Koku Ustasý"	 ,
 		  "Tat Ustasý"	,
 		  "Tat Ustasý"	 ,
 		  "Temas Ustasý"	 ,
 		  "Temas Ustasý"	 ,

 		  "Suçlu"		 ,
 		  "Suçlu"	 ,
 		  "Suç Ustasý"	 ,
 		  "Suç Ustasý" ,
 		  "Suç Üstadý",

 		  "Suç Üstadý"	 ,
 		  "Suç Efendisi"	 ,
 		  "Suç Efendisi"		 ,
 		  "Patron"			 ,
 		  "Patron"		 ,

 		  "Elebaþý" ,
 		  "Elebaþý" ,
 		  "Büyük Patron" ,
 		  "Büyük Patron" ,
 		  "Yeraltýnýn Gölgesi" ,

 		  "Yeraltýnýn Gölgesi" ,
 		  "Gölge" ,
 		  "Gölge" ,
 		  "Karanlýðýn Sesi" ,
 		  "Karanlýðýn Sesi" ,

 		  "Sessizliðin Efendisi" ,
 		  "Sessizliðin Efendisi" ,
 		  "Hile Efendisi" ,
 		  "Hile Efendisi" ,
 		  "Býçak Efendisi" ,

 		"Býçak Efendisi" ,
 		"Zehir Efendisi" ,
 		"Zehir Efendisi" ,
 		"Giz Efendisi" ,
 		"Giz Efendisi" ,

 	        "Hýrsýz Efendi" ,
 		"Hýrsýz Efendi" ,
 		"Suikastçi Efendi" ,
 		"Suikastçi Efendi" ,
 		"Görünmeyen" ,

 		"Görünmeyen" ,
 		"Hýrsýzlarýn Efendisi" ,
 		"Hýrsýzlarýn Efendisi" ,
 		"Suikastçilerin Efendisi" ,
 		"Suikastçilerin Efendisi" ,

 		"Kahraman"		,
 	 "Melek"		,
 	 "Melek"		,
 	 "Elçi"		,
 	 "Ölümsüz"		,
 	"Yüce"		,
 	"Yüce",
 	"Yüce",
 	"Yüce",
 	 "Tek" },

     {
           "Man"			 ,

 	  "Acemi"		 ,
 	  "Çaylak"		 ,
 	  "Nöbetçi"		 ,
 	  "Dövüþçü"		 ,
 	  "Asker"			 ,

 	  "Savaþan"		 ,
 	  "Kýdemli"			 ,
 	  "Kýlýç Çeken"		 ,
 	  "Eskrimci"	 ,
 	  "Savaþçý"		 ,

 	  "Kahraman"		 ,
 	  "Kahraman"		 ,
 	  "Þanlý"		 ,
 	  "Þanlý"		 ,
 	  "Gri Kalkan"		 ,

 	  "Gri Kalkan"				 ,
 	  "Paralý Asker"					 ,
 	  "Paralý Asker"						 ,
 	  "Kýlýç Ustasý"				 ,
 	  "Kýlýç Ustasý"				 ,

 	  "Teðmen"						 ,
 	  "Teðmen"					 ,
 	  "Savunan"				 ,
 	  "Savunan"					 ,
 	  "Dragon"					 ,

 	  "Dragon"					 ,
 	  "Saygýsýz"						 ,
 	  "Saygýsýz"						 ,
 	  "Þövalye"					 ,
 	  "Þövalye"				 ,

 	  "Büyük Þövalye"				 ,
 	  "Büyük Þövalye"				 ,
 	  "Efendi Þövalye"					 ,
 	  "Efendi Þövalye"				 ,
 	  "Gladyatör"					 ,

 	  "Gladyatör"				 ,
 	  "Efendi Gladyatör"				 ,
 	  "Efendi Gladyatör"			 ,
 	  "Ýblis Katleden"					 ,
 	  "Ýblis Katleden"				 ,

 	  "Büyük Ýblis Katleden"		 ,
 	  "Büyük Ýblis Katleden"			 ,
 	  "Ejder Katili"				 ,
 	  "Ejder Katili"				 ,
 	  "Büyük Ejder Katili"		 ,

 	  "Büyük Ejder Katili"		 ,
 	  "Lord"						 ,
 	  "Lord"				 ,
 	  "Efendi"						 ,
 	  "Efendi"						 ,

 	  "Baron"			 ,
 	  "Baron"		 ,
 	  "Rüzgar Baron"			 ,
 	  "Rüzgar Baron"				 ,
 	  "Fýrtýna Baron"			 ,

 	  "Fýrtýna Baron"			 ,
 	  "Kasýrga Baron"		 ,
 	  "Kasýrga Baron"		 ,
 	  "Kudretli Baron"	 ,
 	  "Kudretli Baron"	 ,

 	  "Ateþ Baron"   ,
 	  "Ateþ Baron"   ,
 	  "Buz Baron"   ,
 	  "Buz Baron"   ,
 	  "Þimþek Baron"  ,

 	  "Þimþek Baron"  ,
 	  "Elementlerin Efendisi"   ,
 	  "Elementlerin Efendisi"   ,
 	  "Bakýr Efendi"   ,
 	  "Bakýr Efendi"  ,

 	  "Bronz Efendi"  ,
 	  "Bronz Efendi"   ,
 	  "Pirinç Efendi"  ,
 	  "Pirinç Efendi"   ,
 	  "Demir Efendi"   ,

 	  "Demir Efendi"   ,
 	  "Çelik Efendi"  ,
 	  "Çelik Efendi"   ,
 	  "Mitril Efendi"   ,
 	  "Mitril Efendi"  ,

 	  "Adamantit Efendi"   ,
 	  "Adamantit Efendi"  ,
 	  "Yüzbaþý"   ,
 	  "Yüzbaþý"  ,
 	  "General"   ,

 	  "General"  ,
 	  "Kara Mareþali"  ,
 	  "Kara Mareþali"  ,
 	  "Savaþ Ustasý"   ,
 	  "Savaþ Ustasý"  ,

 	  "Kahraman"		,
 	 "Melek"		,
 	 "Melek"		,
 	 "Elçi"		,
 	 "Ölümsüz"		,
 	"Yüce"		,
 	"Yüce",
 	"Yüce",
 	"Yüce",
 	 "Tek"     },

       {

         "Man"                ,

         "Paladin Yamaðý"		  ,
         "Paladin Çýraðý"			 ,
         "Kalkan"		   ,
         "Kalkan"		 ,
         "Kýlýç"			 ,

         "Yay"			 ,
         "Taþýyýcý"		  ,
         "Atlý"			 ,
         "Bey Adayý"		 ,
         "Bey"		 ,

         "Piyade Neferi"   ,
         "Piyade Neferi"  ,
         "Mýzrak Taþýyan" ,
         "Mýzrak Taþýyan" ,
         "Yay Taþýyan"  ,

         "Yay Taþýyan"   ,
         "Kýlýç Taþýyan"   ,
         "Kýlýç Taþýyan"  ,
         "Onurlu"   ,
         "Onurlu"   ,

         "Soylu"   ,
         "Soylu"  ,
         "Güvenilir"   ,
         "Güvenilir"   ,
         "Doðrucu"   ,

         "Doðrucu"  ,
         "Yiðit"  ,
         "Yiðit"  ,
         "Paladin"   ,
         "Paladin"   ,

         "Görevci"  ,
         "Görevci"  ,
         "Saygýn"   ,
         "Saygýn"  ,
         "Savunan" ,

         "Savunan"  ,
         "Þanlý Þövalye"  ,
         "Þanlý Þövalye"   ,
         "Paladin Þövalye"  ,
         "Paladin Þövalye"   ,

         "Arna Adayý"  ,
         "Arna Adayý"   ,
         "Dindar Þövalye"   ,
         "Dindar Þövalye"  ,
         "Dindar Þövalye"  ,

         "Dindar Þövalye"  ,
         "Dindar Savunma"  ,
         "Dindar Savunma"   ,
         "Þövalye Arna"  ,
         "Þövalye Arna"   ,

         "Arna Çýraðý"  ,
         "Arna Çýraðý"   ,
         "Arna"   ,
         "Arna"  ,
         "Arna Teðmen"  ,

         "Arna Teðmen"   ,
         "Arna Yüzbaþý"  ,
         "Arna Yüzbaþý"   ,
         "Arna Albay"   ,
         "Arna Albay"   ,

         "Arna General"  ,
         "Arna General"  ,
         "Arna Kara Mareþali"   ,
         "Arna Kara Mareþali"   ,
         "Þifacý Þövalye"  ,

         "Þifacý Þövalye"   ,
         "Öc Alan"   ,
         "Öc Alan"  ,
         "Öncü"   ,
         "Öncü"   ,

         "Öncü Lideri"  ,
         "Öncü Lideri"   ,
         "Öncülerin Lideri"  ,
         "Öncülerin Lideri"  ,
         "Arna Lideri"   ,

         "Arna Lideri"  ,
         "Arnalarýn Lideri"   ,
         "Arnalarýn Lideri"   ,
         "Arnalarýn Þanlý Lideri"   ,
         "Arnalarýn Þanlý Lideri" ,

         "Arnalarýn Lordu"  ,
         "Arnalarýn Lordu"  ,
         "Ýnancýn Çekici"   ,
         "Ýnancýn Çekici"  ,
         "Kafirlerin Katili"   ,

         "Kafirlerin Katili"  ,
         "Dindar"   ,
         "Dindar"   ,
         "Kutsal Þövalye"  ,
         "Kutsal Þövalye"   ,

         "Kahraman"		,
 	 "Melek"		,
 	 "Melek"		,
 	 "Elçi"		,
 	 "Ölümsüz"		,
 	"Yüce"		,
 	"Yüce",
 	"Yüce",
 	"Yüce",
 	 "Tek"
      },
       {
         "Man"						 ,

         "Kirli"						 ,
         "Zorba"			 	 ,
         "Kaba"   ,
         "Cani"   ,
         "Cani"  ,

         "Yaðmacý"   ,
         "Yokeden"  ,
         "Yakan"   ,
         "Kiralýk Katil"   ,
         "Kanunsuz"  				 ,

         "Paralý Asker"   ,
         "Paralý Asker"   ,
         "Kara Kýlýç"  ,
         "Kara Kýlýç"   ,
         "Kýzýl Kýlýç"  ,

         "Kýzýl Kýlýç"   ,
         "Süzülen" ,
         "Süzülen"  ,
         "Gaddar"   ,
         "Gaddar" ,

         "Araklayan"   ,
         "Araklayan"   ,
         "Bilinmeyen"  ,
         "Bilinmeyen"   ,
         "Nefret Edilen"  ,

         "Nefret Edilen"   ,
         "Alçak"   ,
         "Alçak"  ,
         "Paladin Düþmaný"   ,
         "Paladin Düþmaný" ,

         "Þeytani Dövüþçü"  ,
         "Þeytani Dövüþçü"  ,
         "Haydut Þövalye"  ,
         "Haydut Þövalye"  ,
         "Þeytani Savunucu"   ,

         "Þeytani Savunucu"  ,
         "Kanlý"  ,
         "Kanlý"   ,
         "Kara Þövalye"   ,
         "Kara Þövalye"   ,

         "Kýzýl Þövalye"  ,
         "Kýzýl Þövalye"  ,
         "Büyük Þövalye"  ,
         "Büyük Þövalye"   ,
         "Ýnançsýz Þövalye"   ,

         "Ýnançsýz Þövalye"   ,
         "Acýnýn Þövalyesi"   ,
         "Acýnýn Þövalyesi"  ,
         "Lejyon Þövalyesi"   ,
         "Lejyon Þövalyesi"   ,

         "Lejyon Þövalyesi"   ,
         "Lejyon Þövalyesi"  ,
         "Lejyon Savunmasý"  ,
         "Lejyon Savunmasý"  ,
         "Lejyon Yüzbaþýsý"   ,

         "Lejyon Yüzbaþýsý"   ,
         "Lejyon Generali"   ,
         "Lejyon Generali"   ,
         "Lejyon Kara Mareþali" ,
         "Lejyon Kara Mareþali"   ,

         "Kýyamet Þövalyesi"  ,
         "Kýyamet Þövalyesi" ,
         "Iþýðý kesen"   ,
         "Iþýðý Kesen"   ,
         "Acýnýn Efendisi"   ,

         "Acýnýn Efendisi"   ,
         "Ölü Efendisi"   ,
         "Ölü Efendisi"   ,
         "Karanlýk Þövalye"  ,
         "Karanlýk Þövalye"   ,

         "Karanlýk Lord"   ,
         "Karanlýk Lord"   ,
         "Karanlýk Kral"   ,
         "Karanlýk Kral"   ,
         "Ölüm Habercisi"   ,

         "Ölüm Habercisi"   ,
         "Ölüm Yayan"   ,
         "Ölüm Yayan"   ,
         "Kýtlýk Getiren"   ,
         "Kýtlýk Getiren"   ,

         "Ölüm Getiren"   ,
         "Ölüm Getiren"   ,
         "Onursuz Ýnançsýz"  ,
         "Onursuz Ýnançsýz"  ,
         "Katleden"   ,

         "Katleden"   ,
         "Karanlýk Tohum"   ,
         "Karanlýk Tohum"  ,
         "Onursuz Þövalye"   ,
         "Onursuz Þövalye"  ,

         "Kahraman"		,
 	 "Melek"		,
 	 "Melek"		,
 	 "Elçi"		,
 	 "Ölümsüz"		,
 	"Yüce"		,
 	"Yüce",
 	"Yüce",
 	"Yüce",
 	 "Tek"	 },

       {
         "Man"  ,

         "Uke"   ,
         "Tori"   ,
         "Beyaz Kuþak"  ,
         "Sarý Kuþak"   ,
         "Yeþil Kuþak"   ,

         "Kahverengi Üç"   ,
         "Kahverengi Ýki" ,
         "Kahverengi Bir"  ,
         "Siyah Üç"   ,
         "Siyah Ýki"   ,

         "Okuiri"   ,
         "Okuiri"   ,
         "Shuto"   ,
         "Shuto"  ,
         "Uraken"   ,

         "Uraken"   ,
         "Shotei"   ,
         "Shotei"   ,
         "Tateken"   ,
         "Tateken"   ,

         "Seiken"   ,
         "Seiken"   ,
         "Ude"   ,
         "Ude"  ,
         "Empi"  ,

         "Empi"   ,
         "Josokutei"   ,
         "Josokutei"   ,
         "Kasokutei"   ,
         "Kasokutei"  ,

         "Atemi Waza Ustasý"   ,
         "Atemi Waza Ustasý"  ,
         "Kotegaeshi Ustasý"   ,
         "Kotegaeshi Ustasý"  ,
         "Kansetsuwaza Ustasý"   ,

         "Kansetsuwaza Ustasý"  ,
         "Taisabaki Ustasý"   ,
         "Taisabaki Ustasý"  ,
         "Kyusho Ustasý"   ,
         "Kyusho Ustasý"  ,

         "Kamae Öðrencisi"  ,
         "Kamae Öðrencisi"   ,
         "Kamae Ustasý"  ,
         "Kamae Ustasý"  ,
         "Ukemi Ustasý"   ,

         "Ukemi Ustasý"  ,
         "Mokuroku"   ,
         "Mokuroku"  ,
         "Ogoshi"  ,
         "Ogoshi"  ,

         "Ippon Seinage"   ,
         "Ippon Seinage"  ,
         "Koshi Garuma"   ,
         "Koshi Garuma"   ,
         "Sukuinage"   ,

         "Sukuinage"  ,
         "Osotogari"  ,
         "Osotogari"   ,
         "Uki Goshi" ,
         "Uki Goshi"  ,

         "Taiotoshi"   ,
         "Taiotoshi"   ,
         "Harai Goshi"   ,
         "Harai Goshi"   ,
         "Yama Arashi"   ,

         "Yama Arashi"   ,
         "Nage Waza Ustasý"   ,
         "Nage Waza Ustasý"   ,
         "Owazaya Giriþ"  ,
         "Owazaya Giriþ"   ,

         "Owaza Çýraðý" ,
         "Owaza Çýraðý"   ,
         "Owaza Öðrencisi"  ,
         "Owaza Öðrencisi"   ,
         "Owaza"   ,

         "Owaza"   ,
         "Owaza Ustasý"  ,
         "Owaza Ustasý"  ,
         "Menkyo"  ,
         "Menkyo"   ,

         "Sensei"   ,
         "Sensei"  ,
         "Shinan"   ,
         "Shinan"   ,
         "Shihan"   ,

         "Shihan"   ,
         "Kaiden"   ,
         "Kaiden"   ,
         "Miyama Ryu Ustasý"  ,
         "Miyama Ryu Ustasý" ,

         "Kahraman"		,
 	 "Melek"		,
 	 "Melek"		,
 	 "Elçi"		,
 	 "Ölümsüz"		,
 	"Yüce"		,
 	"Yüce",
 	"Yüce",
 	"Yüce",
 	 "Tek"
      },
      {

         "Man"	 ,

         "Yavru Kurt"   ,
         "Yavru Kurt"   ,
         "Ýzci"   ,
         "Ýzci"   ,
         "Korucu"  ,

         "Korucu"   ,
         "Tuzakçý"  ,
         "Tuzakçý"  ,
         "Avcý"  ,
         "Avcý"			 			 ,

         "Ýz Sürücü"   ,
         "Ýz Sürücü"  ,
         "Öncü" ,
         "Öncü"  ,
         "Yeþil Adam"   ,

         "Yeþil Adam"  ,
         "Okçu"   ,
         "Okçu"   ,
         "Aðaç Adam"   ,
         "Aðaç Adam"  ,

         "Aðaç Ustasý"  ,
         "Aðaç Ustasý"  ,
         "Kolcu Çýrak"  ,
         "Kolcu Çýrak"  ,
         "Kolcu Adayý"   ,

         "Kolcu Adayý"   ,
         "Kolcu Kalfasý"   ,
         "Kolcu Kalfasý"  ,
         "Kolcu"   ,
         "Kolcu"   ,

         "Kolcu"  ,
         "Kolcu"  ,
         "Kolcu"  ,
         "Kolcu"   ,
         "Kolcu"   ,

         "Kolcu"  ,
         "Kolcu Yüzbaþý"  ,
         "Kolcu Yüzbaþý"   ,
         "Büyük Korucu"  ,
         "Büyük Korucu"  ,

         "Korucu Yüzbaþý" ,
         "Korucu Yüzbaþý"   ,
         "Korucu General"   ,
         "Korucu General"   ,
         "Korucularýn Efendisi"   ,

         "Korucularýn Efendisi"   ,
         "Ormanýn Þövalyesi"   ,
         "Ormanýn Þövalyesi" ,
         "Ormanýn Kýlýcý"  ,
         "Ormanýn Kýlýcý"   ,

         "Orman Gardý"   ,
         "Orman Gardý"  ,
         "Orman Lordu"   ,
         "Orman Lordu"  ,
         "Orman Lordu"   ,

         "Orman Lordu"   ,
         "Orman Baronu"   ,
         "Orman Baronu"  ,
         "Ormanlarýn Baronu"  ,
         "Ormanlarýn Baronu"  ,

         "Avlayan"  ,
         "Avlayan"  ,
 	"Sakýnan" ,
 	"Sakýnan" ,
 	"Koruyan" ,

 	"Koruyan",
         "Kolcu Þövalye"   ,
         "Kolcu Þövalye" ,
         "Kolcu Þövalye"   ,
         "Kolcu Þövalye"  ,

         "Kolcu Lord"   ,
         "Kolcu Lord"  ,
         "Kolcu Lord"  ,
         "Kolcu Lord"  ,
         "Kolcu Baron"   ,

         "Kolcu Baron"  ,
         "Kolcu Baron"   ,
         "Kolcu Baron"   ,
 	"Gezgin" ,
 	"Gezgin" ,

         "Selvan",
 	"Selvan" ,
         "Ormanlarýn Sesi"  ,
         "Ormanlarýn Sesi"   ,
 	"Ormanlarýn Sesi"   ,

         "Ýzleyen"   ,
         "Ýzleyen"   ,
 	"Ýzleyen"
 	"Ýzleyen"
 	"Ýzleyen"

         "Kahraman"		,
 	 "Melek"		,
 	 "Melek"		,
 	 "Elçi"		,
 	 "Ölümsüz"		,
 	"Yüce"		,
 	"Yüce",
 	"Yüce",
 	"Yüce",
 	 "Tek"
      },
       {
         "Man" ,

         "Büyü Çýraðý"		 ,
         "Büyü Öðrencisi"	 ,
         "Büyü Asistaný"	 ,
         "Büyü Araþtýran"		 ,
         "Sihirbaz"	 ,

         "Büyü Katibi"	 ,
         "Falcý"		 ,
         "Arif"		 ,
         "Ýlüzyonist"		 ,
         "Tövbekar"		 ,

         "Duacý"		 ,
         "Duacý"		 ,
         "Büyü Yapan"		 ,
         "Büyü Yapan"		 ,
         "Hokkabaz"		 ,

         "Hokkabaz" ,
         "Büyücü"	 ,
         "Büyücü"	 ,
 	"Oluþturan" ,
 	"Oluþturan" ,

         "Alim"		 ,
         "Alim"		 ,
         "Büyü Ustasý"			 ,
         "Büyü Ustasý"			 ,
         "Büyü Alimi"		 ,

         "Büyü Alimi"		 ,
         "Savaþçý Arif"		 ,
         "Savaþçý Arif"		 ,
         "Durendiþ"	 ,
         "Durendiþ"	 ,

         "Kýdemli Büyücü"		 ,
         "Kýdemli Büyücü"		 ,
         "Büyük Büyücü"		 ,
         "Büyük Büyücü"	 ,
         "Yüce Büyücü"		 ,

         "Yüce Büyücü"	 ,
         "Golem Efendisi"		 ,
         "Golem Efendisi"	 ,
         "Yüce Golem Efendisi"		 ,
         "Yüce Golem Efendisi"	 ,

 	  "Taþ Efendisi"	 ,
 	  "Taþ Efendisi"	 ,
 	  "Ýksir Ustasý"	 ,
 	  "Ýksir Ustasý"	 ,
 	  "Parþömen Ustasý"	 ,

 	  "Parþömen Ustasý"	 ,
 	  "Deðnek Ustasý"	 ,
 	  "Deðnek Ustasý"		 ,
 	  "Deðnek Ustasý"	 ,
 	  "Deðnek Ustasý"		 ,

 	  "Ýblis Çaðýran"	 ,
 	  "Ýblis Çaðýran"		 ,
 	  "Yüce Ýblis Çaðýran"	 ,
 	  "Yüce Ýblis Çaðýran" ,
 	  "Ejderha Efendisi"		 ,

 	  "Ejderha Efendisi"		 ,
 	  "Ejderha Efendisi"	 ,
 	  "Ejderha Efendisi" ,
         "Karýþým Efendisi" ,
         "Karýþým Efendisi" ,

         "Büyülerin Efendisi" ,
         "Büyülerin Efendisi" ,
         "Büyüleme Efendisi" ,
         "Büyüleme Efendisi" ,
         "Çaðrý Ustasý" ,

         "Çaðrý Ustasý" ,
         "Büyük Kahin" ,
         "Büyük Kahin" ,
         "Deðiþim Ustasý" ,
         "Deðiþim Ustasý" ,

         "Fiþek Efendisi" ,
         "Fiþek Efendisi" ,
         "Ölülerin Efendisi" ,
         "Ölülerin Efendisi" ,
         "Simyacý" ,

         "Simyacý" ,
         "Bað Ustasý" ,
         "Bað Ustasý" ,
         "Söz Efendisi" ,
         "Söz Efendisi" ,

         "Geçmiþe hükmeden" ,
         "Geçmiþe hükmeden" ,
         "Bugüne hükmeden" ,
         "Bugüne hükmeden" ,
         "Geleceðe hükmeden" ,

         "Geleceðe hükmeden" ,
         "Baþ Büyücü" ,
         "Baþ Büyücü" ,
         "Bilinmeyen" ,
         "Bilinmeyen" ,

 	  "Kahraman"		,
 	 "Melek"		,
 	 "Melek"		,
 	 "Elçi"		,
 	 "Ölümsüz"		,
 	"Yüce"		,
 	"Yüce",
 	"Yüce",
 	"Yüce",
 	 "Tek"
      },
      {

           "Man"		 ,

 	  "Acemi"	 ,
 	  "Çaylak"	 ,
 	  "Nöbetçi" ,
 	  "Dövüþçü" ,
 	  "Asker"		 ,

 	  "Savaþan"	 ,
 	  "Kýdemli"		 ,
 	  "Kýlýç Çeken"	 ,
 	  "Eskrimci"	 ,
 	  "Savaþçý"		 ,

 	  "Kahraman"	 ,
 	  "Kahraman"	 ,
 	  "Þanlý"	 ,
 	  "Þanlý"	 ,
 	  "Gri Kalkan"	 ,

 	  "Gri Kalkan"		 ,
 	  "Paralý Asker"		 ,
 	  "Paralý Asker"		 ,
 	  "Kýlýç Ustasý"	 ,
 	  "Kýlýç Ustasý"	 ,

 	  "Teðmen"		 ,
 	  "Teðmen"		 ,
 	  "Savunan"	 ,
 	  "Savunan"	 ,
 	  "Dragon"		 ,

 	  "Dragon"			 ,
 	  "Saygýsýz"		 ,
 	  "Saygýsýz"	 ,
 	  "Þövalye"		 ,
 	  "Þövalye"	 ,

 	  "Büyük Þövalye"				 ,
 	  "Büyük Þövalye"				 ,
 	  "Efendi Þövalye"					 ,
 	  "Efendi Þövalye"				 ,
 	  "Gladyatör"					 ,

 	  "Gladyatör"				 ,
 	  "Efendi Gladyatör"				 ,
 	  "Efendi Gladyatör"			 ,
 	  "Ýblis Katleden"					 ,
 	  "Ýblis Katleden"				 ,

 	  "Büyük Ýblis Katleden"		 ,
 	  "Büyük Ýblis Katleden"			 ,
 	  "Ejder Katili"				 ,
 	  "Ejder Katili"				 ,
 	  "Büyük Ejder Katili"		 ,

 	  "Büyük Ejder Katili"		 ,
 	  "Lord"						 ,
 	  "Lord"				 ,
 	  "Efendi"						 ,
 	  "Efendi"						 ,

 	  "Baron"			 ,
 	  "Baron"		 ,
 	  "Rüzgar Baron"			 ,
 	  "Rüzgar Baron"				 ,
 	  "Fýrtýna Baron"			 ,

 	  "Fýrtýna Baron"			 ,
 	  "Kasýrga Baron"		 ,
 	  "Kasýrga Baron"		 ,
 	  "Kudretli Baron"  ,
 	  "Kudretli Baron" ,

 	  "Ateþ Baron"   ,
 	  "Ateþ Baron"   ,
 	  "Buz Baron"   ,
 	  "Buz Baron"   ,
 	  "Þimþek Baron"  ,

 	  "Þimþek Baron"  ,
 	  "Elementlerin Efendisi"   ,
 	  "Elementlerin Efendisi"   ,
 	  "Bakýr Efendi"   ,
 	  "Bakýr Efendi"  ,

 	  "Bronz Efendi"  ,
 	  "Bronz Efendi"   ,
 	  "Pirinç Efendi"  ,
 	  "Pirinç Efendi"   ,
 	  "Demir Efendi"   ,

 	  "Demir Efendi"   ,
 	  "Çelik Efendi"  ,
 	  "Çelik Efendi"   ,
 	  "Mitril Efendi"   ,
 	  "Mitril Efendi"  ,

 	  "Adamantit Efendi"   ,
 	  "Adamantit Efendi"  ,
 	  "Yüzbaþý"   ,
 	  "Yüzbaþý"  ,
 	  "General"   ,

 	  "General"  ,
 	  "Kara Mareþali"  ,
 	  "Kara Mareþali"  ,
 	  "Savaþ Ustasý"   ,
 	  "Savaþ Ustasý"  ,

 	  "Kahraman"		,
 	 "Melek"		,
 	 "Melek"		,
 	 "Elçi"		,
 	 "Ölümsüz"		,
 	"Yüce"		,
 	"Yüce",
 	"Yüce",
 	"Yüce",
 	 "Tek"
      },
       {
         "Man" ,

         "Büyü Çýraðý"		 ,
         "Büyü Öðrencisi"	 ,
         "Büyü Asistaný"	 ,
         "Büyü Araþtýran"		 ,
         "Sihirbaz"	 ,

         "Büyü Katibi"	 ,
         "Falcý"		 ,
         "Arif"		 ,
         "Ýlüzyonist"		 ,
         "Tövbekar"		 ,

         "Duacý"		 ,
         "Duacý"		 ,
         "Büyü Yapan"		 ,
         "Büyü Yapan"		 ,
         "Hokkabaz"		 ,

         "Hokkabaz"		 ,
         "Büyücü"		 ,
         "Büyücü"		 ,
 	"Oluþturan"    ,
 	"Oluþturan" ,

         "Alim"	 ,
         "Alim"	 ,
         "Büyü Ustasý"		 ,
         "Büyü Ustasý"		 ,
         "Büyü Alimi"	 ,

         "Büyü Alimi"		 ,
         "Savaþçý Arif"		 ,
         "Savaþçý Arif"		 ,
         "Durendiþ"	 ,
         "Durendiþ"	 ,

         "Kýdemli Büyücü"		 ,
         "Kýdemli Büyücü"		 ,
         "Büyük Büyücü"		 ,
         "Büyük Büyücü"	 ,
         "Yüce Büyücü"		 ,

         "Yüce Büyücü"	 ,
         "Golem Efendisi"		 ,
         "Golem Efendisi"	 ,
         "Yüce Golem Efendisi"		 ,
         "Yüce Golem Efendisi"	 ,

 	  "Taþ Efendisi"	 ,
 	  "Taþ Efendisi"	 ,
 	  "Ýksir Ustasý"	 ,
 	  "Ýksir Ustasý"	 ,
 	  "Parþömen Ustasý"	 ,

 	  "Parþömen Ustasý"	 ,
 	  "Deðnek Ustasý"	 ,
 	  "Deðnek Ustasý"		 ,
 	  "Deðnek Ustasý"	 ,
 	  "Deðnek Ustasý"		 ,

 	  "Ýblis Çaðýran"	 ,
 	  "Ýblis Çaðýran"		 ,
 	  "Yüce Ýblis Çaðýran"	 ,
 	  "Yüce Ýblis Çaðýran" ,
 	  "Ejderha Efendisi"		 ,

 	  "Ejderha Efendisi"		 ,
 	  "Ejderha Efendisi"	 ,
 	  "Ejderha Efendisi" ,
         "Karýþým Efendisi" ,
         "Karýþým Efendisi" ,

         "Büyülerin Efendisi" ,
         "Büyülerin Efendisi" ,
         "Büyüleme Efendisi" ,
         "Büyüleme Efendisi" ,
         "Çaðrý Ustasý" ,

         "Çaðrý Ustasý" ,
         "Büyük Kahin" ,
         "Büyük Kahin" ,
         "Deðiþim Ustasý" ,
         "Deðiþim Ustasý" ,

         "Fiþek Efendisi" ,
         "Fiþek Efendisi" ,
         "Ölülerin Efendisi" ,
         "Ölülerin Efendisi" ,
         "Simyacý" ,

         "Simyacý" ,
         "Bað Ustasý" ,
         "Bað Ustasý" ,
         "Söz Efendisi" ,
         "Söz Efendisi" ,

         "Geçmiþe hükmeden" ,
         "Geçmiþe hükmeden" ,
         "Bugüne hükmeden" ,
         "Bugüne hükmeden" ,
         "Geleceðe hükmeden" ,

         "Geleceðe hükmeden" ,
         "Baþ Büyücü" ,
         "Baþ Büyücü" ,
         "Bilinmeyen" ,
         "Bilinmeyen" ,

 	  "Kahraman"		,
 	 "Melek"		,
 	 "Melek"		,
 	 "Elçi"		,
 	 "Ölümsüz"		,
 	"Yüce"		,
 	"Yüce",
 	"Yüce",
 	"Yüce",
 	 "Tek"
      },
       {

         "Man" ,

         "Büyü Çýraðý"		 ,
         "Büyü Öðrencisi"	 ,
         "Büyü Asistaný"	 ,
         "Büyü Araþtýran"		 ,
         "Sihirbaz"	 ,

         "Büyü Katibi"	 ,
         "Falcý"		 ,
         "Arif"		 ,
         "Ýlüzyonist"		 ,
         "Tövbekar"		 ,

         "Duacý"		 ,
         "Duacý"		 ,
         "Büyü Yapan"		 ,
         "Büyü Yapan"		 ,
         "Hokkabaz"		 ,

         "Hokkabaz" ,
         "Büyücü"	 ,
         "Büyücü"	 ,
 	"Oluþturan" ,
 	"Oluþturan" ,

         "Alim"		 ,
         "Alim"		 ,
         "Büyü Ustasý"			 ,
         "Büyü Ustasý"			 ,
         "Büyü Alimi"		 ,

         "Büyü Alimi"		 ,
         "Savaþçý Arif"		 ,
         "Savaþçý Arif"		 ,
         "Durendiþ"	 ,
         "Durendiþ"	 ,

         "Kýdemli Büyücü"		 ,
         "Kýdemli Büyücü"		 ,
         "Büyük Büyücü"		 ,
         "Büyük Büyücü"	 ,
         "Yüce Büyücü"		 ,

         "Yüce Büyücü"	 ,
         "Golem Efendisi"		 ,
         "Golem Efendisi"	 ,
         "Yüce Golem Efendisi"		 ,
         "Yüce Golem Efendisi"	 ,

 	  "Taþ Efendisi"	 ,
 	  "Taþ Efendisi"	 ,
 	  "Ýksir Ustasý"	 ,
 	  "Ýksir Ustasý"	 ,
 	  "Parþömen Ustasý"	 ,

 	  "Parþömen Ustasý"	 ,
 	  "Deðnek Ustasý"	 ,
 	  "Deðnek Ustasý"		 ,
 	  "Deðnek Ustasý"	 ,
 	  "Deðnek Ustasý"		 ,

 	  "Ýblis Çaðýran"	 ,
 	  "Ýblis Çaðýran"		 ,
 	  "Yüce Ýblis Çaðýran"	 ,
 	  "Yüce Ýblis Çaðýran" ,
 	  "Ejderha Efendisi"		 ,

 	  "Ejderha Efendisi"		 ,
 	  "Ejderha Efendisi"	 ,
 	  "Ejderha Efendisi" ,
         "Karýþým Efendisi" ,
         "Karýþým Efendisi" ,

         "Büyülerin Efendisi" ,
         "Büyülerin Efendisi" ,
         "Büyüleme Efendisi" ,
         "Büyüleme Efendisi" ,
         "Çaðrý Ustasý" ,

         "Çaðrý Ustasý" ,
         "Büyük Kahin" ,
         "Büyük Kahin" ,
         "Deðiþim Ustasý" ,
         "Deðiþim Ustasý" ,

         "Fiþek Efendisi" ,
         "Fiþek Efendisi" ,
         "Ölülerin Efendisi" ,
         "Ölülerin Efendisi" ,
         "Simyacý" ,

         "Simyacý" ,
         "Bað Ustasý" ,
         "Bað Ustasý" ,
         "Söz Efendisi" ,
         "Söz Efendisi" ,

         "Geçmiþe hükmeden" ,
         "Geçmiþe hükmeden" ,
         "Bugüne hükmeden" ,
         "Bugüne hükmeden" ,
         "Geleceðe hükmeden" ,

         "Geleceðe hükmeden" ,
         "Baþ Büyücü" ,
         "Baþ Büyücü" ,
         "Bilinmeyen" ,
         "Bilinmeyen" ,

 	  "Kahraman"		,
 	 "Melek"		,
 	 "Melek"		,
 	 "Elçi"		,
 	 "Ölümsüz"		,
 	"Yüce"		,
 	"Yüce",
 	"Yüce",
 	"Yüce",
 	 "Tek"
      },
      {

         "Man" ,

         "Büyü Çýraðý"		 ,
         "Büyü Öðrencisi"	 ,
         "Büyü Asistaný"	 ,
         "Büyü Araþtýran"		 ,
         "Sihirbaz"	 ,

         "Büyü Katibi"	 ,
         "Falcý"		 ,
         "Arif"		 ,
         "Ýlüzyonist"		 ,
         "Tövbekar"		 ,

         "Duacý"		 ,
         "Duacý"		 ,
         "Büyü Yapan"		 ,
         "Büyü Yapan"		 ,
         "Hokkabaz"		 ,

         "Hokkabaz" ,
         "Büyücü"	 ,
         "Büyücü"	 ,
 	"Oluþturan" ,
 	"Oluþturan" ,

         "Alim"		 ,
         "Alim"		 ,
         "Büyü Ustasý"			 ,
         "Büyü Ustasý"			 ,
         "Büyü Alimi"		 ,

         "Büyü Alimi"		 ,
         "Savaþçý Arif"		 ,
         "Savaþçý Arif"		 ,
         "Durendiþ"	 ,
         "Durendiþ"	 ,

         "Kýdemli Büyücü"		 ,
         "Kýdemli Büyücü"		 ,
         "Büyük Büyücü"		 ,
         "Büyük Büyücü"	 ,
         "Yüce Büyücü"		 ,

         "Yüce Büyücü"	 ,
         "Golem Efendisi"		 ,
         "Golem Efendisi"	 ,
         "Yüce Golem Efendisi"		 ,
         "Yüce Golem Efendisi"	 ,

 	  "Taþ Efendisi"	 ,
 	  "Taþ Efendisi"	 ,
 	  "Ýksir Ustasý"	 ,
 	  "Ýksir Ustasý"	 ,
 	  "Parþömen Ustasý"	 ,

 	  "Parþömen Ustasý"	 ,
 	  "Deðnek Ustasý"	 ,
 	  "Deðnek Ustasý"		 ,
 	  "Deðnek Ustasý"	 ,
 	  "Deðnek Ustasý"		 ,

 	  "Ýblis Çaðýran"	 ,
 	  "Ýblis Çaðýran"		 ,
 	  "Yüce Ýblis Çaðýran"	 ,
 	  "Yüce Ýblis Çaðýran" ,
 	  "Ejderha Efendisi"		 ,

 	  "Ejderha Efendisi"		 ,
 	  "Ejderha Efendisi"	 ,
 	  "Ejderha Efendisi" ,
         "Karýþým Efendisi" ,
         "Karýþým Efendisi" ,

         "Büyülerin Efendisi" ,
         "Büyülerin Efendisi" ,
         "Büyüleme Efendisi" ,
         "Büyüleme Efendisi" ,
         "Çaðrý Ustasý" ,

         "Çaðrý Ustasý" ,
         "Büyük Kahin" ,
         "Büyük Kahin" ,
         "Deðiþim Ustasý" ,
         "Deðiþim Ustasý" ,

         "Fiþek Efendisi" ,
         "Fiþek Efendisi" ,
         "Ölülerin Efendisi" ,
         "Ölülerin Efendisi" ,
         "Simyacý" ,

         "Simyacý" ,
         "Bað Ustasý" ,
         "Bað Ustasý" ,
         "Söz Efendisi" ,
         "Söz Efendisi" ,

         "Geçmiþe hükmeden" ,
         "Geçmiþe hükmeden" ,
         "Bugüne hükmeden" ,
         "Bugüne hükmeden" ,
         "Geleceðe hükmeden" ,

         "Geleceðe hükmeden" ,
         "Baþ Büyücü" ,
         "Baþ Büyücü" ,
         "Bilinmeyen" ,
         "Bilinmeyen" ,

 	 "Kahraman"		,
 	 "Melek"		,
 	 "Melek"		,
 	 "Elçi"		,
 	 "Ölümsüz"		,
 	"Yüce"		,
 	"Yüce",
 	"Yüce",
 	"Yüce",
 	 "Tek"
     }
 ,
   };

const struct color_type color_table[] = /*  Last entry should be NULL   */
{
  {"Black", CLR_BLACK},
  {"Green", CLR_GREEN},
  {"Brown", CLR_BROWN},
  {"Yellow", CLR_YELLOW},
  {"Red", CLR_RED},
  {"Blue", CLR_BLUE},
  {"Magenta", CLR_MAGENTA},
  {"Cyan", CLR_CYAN},
  {"White", CLR_WHITE},
  {NULL, NULL}
};

/* God's Name, name of religion, tattoo vnum  */
const struct religion_type religion_table [] =
{
  { "", "None", 0 },
  { "Kamenilik", "Kameniler", OBJ_VNUM_TATTOO_KAME },
  { "Niryanilik", "Niryaniler", OBJ_VNUM_TATTOO_NIR },
  { "Nyahilik", "Nyahiler", OBJ_VNUM_TATTOO_NYAH },
  { "Sintiyanlýk", "Sintiyanlar", OBJ_VNUM_TATTOO_SINT },
};

const struct ethos_type ethos_table [] =
{
  { "Null" },
  { "Tüze" },
  { "Yansýz" },
  { "Kaos" }
};

/*  altar good neut evil, recall good neut evil, pit good neut evil */
const struct hometown_type hometown_table [] =
{
  { "Selenge", {3070, 3054, 3072}, {3068, 3001, 3071}, {3069,3054,3072} },
  { "Yeni Thalos", {9605, 9605, 9605}, {9609,9609,9609}, {9609,9609,9609} },
  { "Titan", {18127,18127,18127},{18126,18126,18126},{18127,18127,18127} },
  { "Yeni Ofcol", {669, 669, 669},      {698, 698, 698},   {669, 669, 669} },
  { "Eski Selenge",{5386, 5386,5386}, {5379,5379, 5379}, {5386,5386,5386} },
};

/*
 * Attribute bonus tables.
 */
const	struct	str_app_type	str_app		[26]		=
{
    { -5, -4,   0,  0 },  /* 0  */
    { -5, -4,   3,  1 },  /* 1  */
    { -3, -2,   3,  2 },
    { -3, -1,  10,  3 },  /* 3  */
    { -2, -1,  25,  4 },
    { -2, -1,  55,  5 },  /* 5  */
    { -1,  0,  80,  6 },
    { -1,  0,  90,  7 },
    {  0,  0, 100,  8 },
    {  0,  0, 100,  9 },
    {  0,  0, 115, 10 }, /* 10  */
    {  0,  0, 115, 11 },
    {  0,  0, 130, 12 },
    {  0,  0, 130, 13 }, /* 13  */
    {  0,  1, 140, 14 },
    {  1,  1, 150, 15 }, /* 15  */
    {  1,  2, 165, 16 },
    {  2,  3, 180, 22 },
    {  2,  3, 200, 25 }, /* 18  */
    {  3,  4, 225, 30 },
    {  3,  5, 250, 35 }, /* 20  */
    {  4,  6, 300, 40 },
    {  4,  6, 350, 45 },
    {  5,  7, 400, 50 },
    {  5,  8, 450, 55 },
    {  6,  9, 500, 60 }  /* 25   */
};



const	struct	int_app_type	int_app		[26]		=
{
    {  3 },	/*  0 */
    {  5 },	/*  1 */
    {  7 },
    {  8 },	/*  3 */
    {  9 },
    { 10 },	/*  5 */
    { 11 },
    { 12 },
    { 13 },
    { 15 },
    { 17 },	/* 10 */
    { 19 },
    { 22 },
    { 25 },
    { 28 },
    { 31 },	/* 15 */
    { 34 },
    { 37 },
    { 40 },	/* 18 */
    { 44 },
    { 49 },	/* 20 */
    { 55 },
    { 60 },
    { 70 },
    { 80 },
    { 85 }	/* 25 */
};



const	struct	wis_app_type	wis_app		[26]		=
{
    { 0 },	/*  0 */
    { 0 },	/*  1 */
    { 0 },
    { 0 },	/*  3 */
    { 0 },
    { 1 },	/*  5 */
    { 1 },
    { 1 },
    { 1 },
    { 1 },
    { 1 },	/* 10 */
    { 1 },
    { 1 },
    { 1 },
    { 1 },
    { 2 },	/* 15 */
    { 2 },
    { 2 },
    { 3 },	/* 18 */
    { 3 },
    { 3 },	/* 20 */
    { 3 },
    { 4 },
    { 4 },
    { 4 },
    { 5 }	/* 25 */
};



const	struct	dex_app_type	dex_app		[26]		=
{
    {   60 },   /* 0 */
    {   50 },   /* 1 */
    {   50 },
    {   40 },
    {   30 },
    {   20 },   /* 5 */
    {   10 },
    {    0 },
    {    0 },
    {    0 },
    {    0 },   /* 10 */
    {    0 },
    {    0 },
    {    0 },
    {    0 },
    { - 10 },   /* 15 */
    { - 15 },
    { - 20 },
    { - 30 },
    { - 40 },
    { - 50 },   /* 20 */
    { - 60 },
    { - 75 },
    { - 90 },
    { -105 },
    { -120 }    /* 25 */
};


const	struct	con_app_type	con_app		[26]		=
{
    {  0, 20 },   /*  0 */
    {  1, 25 },   /*  1 */
    {  1, 30 },
    {  2, 35 },	  /*  3 */
    {  3, 40 },
    {  4, 45 },   /*  5 */
    {  5, 50 },
    {  6, 55 },
    {  7, 60 },
    {  8, 65 },
    {  9, 70 },   /* 10 */
    { 10, 75 },
    { 11, 80 },
    { 12, 85 },
    { 13, 88 },
    { 14, 90 },   /* 15 */
    { 15, 95 },
    { 16, 97 },
    { 17, 99 },   /* 18 */
    { 18, 99 },
    { 19, 99 },   /* 20 */
    { 20, 99 },
    { 21, 99 },
    { 22, 99 },
    { 23, 99 },
    { 24, 99 }    /* 25 */
};

const	struct	cha_app_type	cha_app		[26]		=
{
    { 10.0/20.0 },   /* 0 */
    { 10.0/20.0 },   /* 1 */
    { 10.0/20.0 },
    { 11.0/20.0 },
    { 11.0/20.0 },
    { 11.0/20.0 },   /* 5 */
    { 12.0/20.0 },
    { 12.0/20.0 },
    { 13.0/20.0 },
    { 13.0/20.0 },
    { 14.0/20.0 },   /* 10 */
    { 15.0/20.0 },
    { 16.0/20.0 },
    { 17.0/20.0 },
    { 18.0/20.0 },
    { 19.0/20.0 },   /* 15 */
    { 20.0/20.0 },
    { 21.0/20.0 },
    { 22.0/20.0 },
    { 23.0/20.0 },
    { 24.0/20.0 },   /* 20 */
    { 26.0/20.0 },
    { 28.0/20.0 },
    { 30.0/20.0 },
    { 32.0/20.0 },
    { 35.0/20.0 },    /* 25 */
};


/*
 * Liquid properties.
 * Used in world.obj.
 */
 const	struct	liq_type	liq_table	[]	=
 {
 /*    name			color	proof, full, thirst, food, ssize */
     { "water","su",			"clear",	{   0, 1, 10, 0, 16 }	},
     { "beer","bira",			"amber",	{  12, 1,  8, 1, 12 }	},
     { "red wine","kýrmýzý þarap",		"burgundy",	{  30, 1,  8, 1,  5 }	},
     { "ale","bira",			"brown",	{  15, 1,  8, 1, 12 }	},
     { "dark ale","siyah bira",		"dark",		{  16, 1,  8, 1, 12 }	},

     { "whisky","viski",			"golden",	{ 120, 1,  5, 0,  2 }	},
     { "lemonade","limonata",		"pink",		{   0, 1,  9, 2, 12 }	},
     { "firebreather","ateþnefesi",		"boiling",	{ 190, 0,  4, 0,  2 }	},
     { "local specialty","yerel içki",	"clear",	{ 151, 1,  3, 0,  2 }	},
     { "slime mold juice","balçýk suyu",	"green",	{   0, 2, -8, 1,  2 }	},

     { "milk","süt",			"white",	{   0, 2,  9, 3, 12 }	},
     { "tea","çay",			"tan",		{   0, 1,  8, 0,  6 }	},
     { "coffee",	"kahve",		"black",	{   0, 1,  8, 0,  6 }	},
     { "blood","kan",			"red",		{   0, 2, -1, 2,  6 }	},
     { "salt water","tuzlu su",		"clear",	{   0, 1, -2, 0,  1 }	},

     { "coke","kola",			"brown",	{   0, 2,  9, 2, 12 }	},
     { "root beer","kök birasý",		"brown",	{   0, 2,  9, 2, 12 }   },
     { "elvish wine","yel þarabý",		"green",	{  35, 2,  8, 1,  5 }   },
     { "white wine","beyaz þarap",		"golden",	{  28, 1,  8, 1,  5 }   },
     { "champagne","þampanya",		"golden",	{  32, 1,  8, 1,  5 }   },

     { "mead","bal likörü",			"honey-colored",{  34, 2,  8, 2, 12 }   },
     { "rose wine","gül þarabý",		"pink",		{  26, 1,  8, 1,  5 }	},
     { "benedictine wine","kutsal þarap",	"burgundy",	{  40, 1,  8, 1,  5 }   },
     { "vodka","votka",			"clear",	{ 130, 1,  5, 0,  2 }   },
     { "cranberry juice","kýzýlcýk suyu",	"red",		{   0, 1,  9, 2, 12 }	},

     { "orange juice","portakal suyu",		"orange",	{   0, 2,  9, 3, 12 }   },
     { "absinthe","raký",		"clear",	{ 200, 1,  4, 0,  2 }	},
     { "brandy","brendi",			"golden",	{  80, 1,  5, 0,  4 }	},
     { "aquavit","kimyon likörü",		"clear",	{ 140, 1,  5, 0,  2 }	},
     { "schnapps","patates likörü",		"clear",	{  90, 1,  5, 0,  2 }   },

     { "icewine","buzbað",		"purple",	{  50, 2,  6, 1,  5 }	},
     { "amontillado","kiraz likörü",		"burgundy",	{  35, 2,  8, 1,  5 }	},
     { "sherry","þeri",			"red",		{  38, 2,  7, 1,  5 }   },
     { "framboise","çilek likörü",		"red",		{  50, 1,  7, 1,  5 }   },
     { "rum","rom",			"amber",	{ 151, 1,  4, 0,  2 }	},

     { "cordial","þeftali likörü",		"clear",	{ 100, 1,  5, 0,  2 }   },
     { NULL,NULL,			NULL,		{   0, 0,  0, 0,  0 }	}
 };



/*
 * The skill and spell table.
 * Slot numbers must never be changed as they appear in #OBJECTS sections.
 */
#define SLOT(n)	n

const	struct	skill_type	skill_table	[MAX_SKILL]	=
{

/*
 * Magic spells.
 */

    {
	{"reserved","reserved"},	{100,100,100,100, 100,100,100,100,100 ,100,100,100,100},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,			TAR_IGNORE,		POS_STANDING,
	&gsn_reserved,			SLOT( 0),	 0,	 0,
	"",			"",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"absorb","soðurma"},	{ 67, 93, 93, 93, 93, 93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_absorb,		TAR_CHAR_SELF,		POS_STANDING,
	&gsn_absorb,		SLOT(707),	100,	12,
  "",		"Çevrendeki enerji alaný yokoluyor!",
	"$p'in çevresindeki enerji alaný yokoluyor.",
 	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"acetum primus","aketum primus"},	{ 34,93,93,93, 93,93,93,93,93,93,93,93,93},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_acetum_primus,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(654),	20,	12,
	"aketum primus",	"!Aketum Primus!", "", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{"accid arrow","asit oku"},		{ 48, 93, 93, 93, 93, 93,93,93,48,93,93,48,48},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_acid_arrow,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(644),	20,	12,
	"asit oku",		"!Asit Oku!", "", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{"acid blast","asit patlamasý"},		{ 63, 93, 93, 93, 93,93,93,93,63,93,93,63,63},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_acid_blast,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_acid_blast,	SLOT(70),	40,	12,
	"asit",		"!Asit!", "", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{"acute vision","güçlü görüþ"},         {  93,93,93,93, 93, 93, 93, 7 ,93,93,93,93,93},
        { 3,  2,  1,  1, 1, 1, 2, 1, 1, 1,1,1,1},
        spell_acute_vision,        TAR_CHAR_SELF,          POS_STANDING,
        NULL,                    SLOT(514),        10,       0,
	"",         "Görüþün sönükleþti.", "",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"adamantite golem","adament golem"},{93,93,93,93,93,93,93,93,93,93,93,71,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_adamantite_golem,	TAR_IGNORE,	POS_STANDING,
	NULL,		SLOT(665),	500,		30,
	"",	"Yeni golemler yapmak için yeterli gücü topladýn.","",
	CABAL_NONE , 	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"aid","ilk yardým"},			{ 93, 53,93, 93,93,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_aid,        TAR_CHAR_DEFENSIVE,          POS_FIGHTING,
	NULL,		SLOT(680),	100,		12,
	"",	"Daha fazla kiþiyi iyileþtirebilirsin.",	"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_HEALING
    },

    {
	{"amnesia","amnezi"},         {  93, 93, 93,93,93,93,93,93,93,93,93,93,93 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_amnesia,      TAR_CHAR_OFFENSIVE,       POS_STANDING,
	NULL,       SLOT(538),        100,       12,
	"", "!amnesia!", "",	CABAL_NONE , RACE_NONE,ALIGN_NONE, GROUP_NONE
    },

    {
	{"animate dead","ölü canlandýrma"},			{ 93, 93, 93, 93,93,93,93,93,93,93,42,38,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_animate_dead,	TAR_OBJ_CHAR_OFF,	POS_STANDING,
	NULL,		SLOT(581),	50,	12,
	"",	"Yeni ölüleri canlandýrmak için gerekli enerjiyi topladýn.", "", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"animate object","nesne canlandýrma"},	{ 70, 93, 93, 93, 93, 93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_animate_object,	TAR_OBJ_CHAR_OFF,	POS_STANDING,
	NULL,		SLOT(709),	50,	12,
	"",		"Yeni nesneleri canlandýrmak için gerekli enerjiyi topladýn.", "",
 	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"armor","zýrh"},		{  13,1,93,93, 12,93,13,93 , 93, 10,13,13,13},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_armor,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT( 1),	 5,	12,
	"",			"Zýrhýnýn zayýfladýðýný hissediyorsun.",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	{"assist","destek"},      {  93, 93, 93, 93,93,93,93,93,93,93,93,43,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_assist,        TAR_CHAR_DEFENSIVE,          POS_FIGHTING,
        NULL,             SLOT(670),        100,       12,
	"", "Yeniden destek olabilirsin.", "",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_HEALING
    },

    {
	{"astral walk","yýldýz yürüyüþü"},		{31,93,93,93,93,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_astral_walk,	TAR_IGNORE,	POS_FIGHTING,
	NULL,			SLOT(622),	80,	12,
	"",			"!Astral Walk!",	"",CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_TRANSPORTATION
    },

    {
	{"attract other","kiþi etkileme"},	{ 93, 93, 93, 93,93,93,93,93,93,93,30,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_attract_other,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
	NULL,		SLOT(580),	5,	12,
	"",	"Efendinin seni terkettiðini hissediyorsun.",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_BEGUILING
    },

    {
	{"bark skin","aðaç deri"},        {  93, 93, 93, 93, 93, 93, 93, 22 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_bark_skin,        TAR_CHAR_SELF,          POS_STANDING,
        NULL,             SLOT(515),        40,       0,
	"", "Aðaç derin yokoluyor.", "",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
		{"black death","kara ölüm"},    { 93, 93, 93, 93,93, 64, 93, 93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_black_death,	TAR_IGNORE,	POS_STANDING,
	&gsn_black_death,	SLOT(677),	200,	24,
	"",		"!black death!",	"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_MALADICTIONS
    },

    {
	{"blade barrier","býçak duvarý"},	{93,60,93,93,93,93,93,93,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_blade_barrier,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(679),	40,	12,
	"býçak duvarý",	"!Blade Barrier!",	"",CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_ATTACK
    },

    {
	{"bless","kutsama"},		{ 93,  14, 93, 93,14,93,93,93,93,12,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_bless,		TAR_OBJ_CHAR_DEF,	POS_STANDING,
	&gsn_bless,		SLOT( 3),	 5,	12,
  "",			"Kutsallýðýný yitiriyorsun.",
	"$p'in kutsal aylasý yokoluyor.", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_BENEDICTIONS
    },

    {
	{"bless weapon","silah kutsama"},{  93, 93, 93, 93,55,93,93,93,93,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_bless_weapon,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(637),	100,	24,
	"",			"!Bless Weapon!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_ENCHANTMENT
    },

    {
	{"blindness","körlük"},	{  93,  14, 93, 93,93,14,16,93, 20,93,20,19,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_blindness,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_blindness,		SLOT( 4),	 5,	12,
	"",			"Yeniden görebiliyorsun.",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_MALADICTIONS
    },

    {
	{"bluefire","mavi alev"},	{  93,37,93,93,93,93,93,93,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_bluefire,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_bluefire,			SLOT(660),	20,	12,
	"mavi alev",		"!Mavi Alev!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_ATTACK
    },

    {
	{"burning hands","yanan eller"},	{  14,93,93,93, 93, 93, 93, 93 ,14,93,14,14,14},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_burning_hands,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_burning_hands,	SLOT( 5),	15,	12,
  "yanan eller",	"!Burning Hands!", 	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{"call lightning","þimþek çaðrýsý"},	{ 93, 44, 93, 93, 41,93,50, 93 , 93,93,93,93,33},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_call_lightning,	TAR_IGNORE,		POS_FIGHTING,
	NULL,			SLOT( 6),	15,	12,
  "þimþek çaðrýsý",	"!Call Lightning!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_WEATHER
    },

    {
    {"calm","yatýþtýrma"},	{ 93, 26, 93, 93, 63, 93, 93, 93 ,93,60,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_calm,		TAR_IGNORE,		POS_FIGHTING,
	NULL,			SLOT(509),	30,	12,
  "",			"Sakinliðini yitiriyorsun.",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_BENEDICTIONS
    },

    {
	{"cancellation","iptal"},	{ 24, 32, 93, 93,93,93,93,93 , 28,93,93,28,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_cancellation,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(507),	20,	12,
	"",			"!cancellation!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	{"cause critical","kritik zarar"},	{  93,  15, 93, 93,93,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1,1, 1,1,1,1},
	spell_cause_critical,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(63),	20,	12,
  "kritik zarar",		"!Cause Critical!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_HARMFUL
    },

    {
	{"cause light","hafif zarar"},		{ 93,  2, 93, 93,93,93,93,93,93 ,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_cause_light,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(62),	15,	12,
  "hafif zarar",		"!Cause Light!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_HARMFUL
    },

    {
	{"cause serious","ciddi zarar"},	{  93,  8, 93, 93,93,93,93,93, 93 ,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_cause_serious,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(64),	17,	12,
  "ciddi zarar",		"!Cause Serious!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_HARMFUL
    },

    {
	{"caustic font","yakýcý su"},{ 93, 93, 93, 93, 93, 93, 93,93,41,93,93,93,93},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_caustic_font,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(653),	20,	12,
  "yakýcý su",	"!caustic font!", "", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{"chain lightning","þimþek zinciri"},		{ 33, 93, 93, 93,93,93,93,93, 33 ,93,33,33,33},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_chain_lightning,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(500),	25,	12,
  "þimþek",		"!Chain Lightning!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{"charm person","teshir"},		{ 93, 93, 93, 93,93,38,93,93, 93,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_charm_person,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_charm_person,	SLOT( 7),	 5,	12,
  "",		"Kendine güvenin artýyor.",	"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_BEGUILING
    },

    {
	{"chromatic orb","krom küre"},	{ 93, 93, 93, 93, 93,93,93,93,63,93,93,93,93},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_chromatic_orb,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(714),	50,	12,
  "krom küre",		"!Chromatic Orb!", "", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{"control undead","hortlak kontrolü"},      {  93, 93,93,93,93,93,93,93,93,93,93,57,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_control_undead,   TAR_CHAR_OFFENSIVE,          POS_STANDING,
        NULL,             SLOT(669),        20,       12,
        "", "Kendine güvenin artýyor.", "",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"chill touch","ayaz"},		{  5, 93, 93, 93,93,6,93,93 ,5,93,93,7,8 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_chill_touch,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_chill_touch,		SLOT( 8),	15,	12,
  "ayaz",	"Üþümen azalýyor.",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{"colour spray","renk spreyi"},		{  22, 93, 93, 93,93,93,93,93 , 22,93,25,93,22},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_colour_spray,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(10),	15,	12,
  "renk spreyi",		"!Colour Spray!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{"continual light","daimi ýþýk"},		{  11,  5, 93, 93,93,93,93,93 , 11,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_continual_light,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(57),	 7,	12,
	"",			"!Continual Light!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_CREATION
    },

    {
	{"control weather","hava kontrolü"},	{ 93,21,93,93, 93,93,11,33, 30,93,24,24,20},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_control_weather,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(11),	25,	12,
	"",			"!Control Weather!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_WEATHER
    },

    {
	{"corruption","çürüme"},      {  93, 93, 93, 93, 93,93,93,93,93,93,93,63,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_corruption,    TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
        NULL,             SLOT(671),        20,       12,
        "çürüme", "Kendini saðlýklý hissediyorsun.", "",CABAL_NONE,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"create food","mantar"},		{ 15, 8, 93, 93,3,93,93,93 , 93,93,93,12,13},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_create_food,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(12),	 5,	12,
	"",			"!Create Food!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_CREATION
    },

    {
	{"create rose","gül"},		{ 26, 93, 93, 93, 20, 93, 93, 93,93,93,26,26,26},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_create_rose,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(511),	30, 	12,
	"",			"!Create Rose!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_CREATION
    },

    {
	{"create spring","pýnar"},		{ 24, 27, 93, 93,93,93,93,93 ,93,93,93,31,31},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_create_spring,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(80),	20,	12,
	"",			"!Create Spring!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_CREATION
    },

    {
	{"create water","su yaratma"},	{ 8,  3, 93, 93,4,93,93,93 ,93,93,93,11,8},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_create_water,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(13),	 5,	12,
	"",			"!Create Water!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_CREATION
    },

    {
	{"cure blindness","körlük tedavi"},		{ 93,11,93,93, 7,93,9,13, 93,20,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_cure_blindness,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(14),	 5,	12,
	"",			"!Cure Blindness!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_CURATIVE
    },

    {
	{"cure critical","kritik tedavi"},	{93,  18, 93, 93,18,93,25,93,93,25,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_cure_critical,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	&gsn_cure_critical,	SLOT(15),	20,	12,
	"",			"!Cure Critical!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_HEALING
    },

    {
	{"cure disease","hastalýk tedavi"},		{  93, 19, 93, 93,33,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_cure_disease,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(501),	20,	12,
	"",			"!Cure Disease!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_CURATIVE
    },

    {
	{"cure light","hafif tedavi"},	{ 93,  4, 93, 93,5,11,9,93,93,10,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_cure_light,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	&gsn_cure_light,	SLOT(16),	10,	12,
	"",			"!Cure Light!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_HEALING
    },

    {
	{"cure poison","zehir tedavi"},			{ 93,  23, 93,93,33,93,93,93,93,35,93,93,23 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_cure_poison,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_cure_poison,		SLOT(43),	 5,	12,
	"",			"!Cure Poison!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_CURATIVE
    },

    {
	{"cure serious","ciddi tedavi"},	{ 93,  10, 93, 93,10,93,18,93,93,18,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_cure_serious,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	&gsn_cure_serious,	SLOT(61),	15,	12,
	"",			"!Cure Serious!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_HEALING
    },

    {
	{"curse","lanet"},		{ 93, 33, 93,93,93,18,93,93,31,93,44,34,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_curse,		TAR_OBJ_CHAR_OFF,	POS_FIGHTING,
	&gsn_curse,		SLOT(17),	20,	12,
  "lanet",		"Üzerindeki lanet çürüyor.",
	"$p üzerindeki lanet çürüyor.", CABAL_NONE , RACE_NONE,
	ALIGN_NONE, GROUP_MALADICTIONS
    },

    {
	{"cursed lands","lanetli topraklar"},    { 93, 41, 93, 93,93, 93, 93, 93 ,93,93,93,64,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_cursed_lands,	TAR_IGNORE,	POS_STANDING,
	&gsn_cursed_lands,	SLOT(675),	200,	24,
	"",		"!cursed lands!",	"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_MALADICTIONS
    },

    {
	{"deadly venom","ölüm zehri"},   { 93, 93, 93, 93,93, 58, 93, 93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_deadly_venom,	TAR_IGNORE,	POS_STANDING,
	&gsn_deadly_venom,		SLOT(674),	200,	24,
	"",		"!deadly venom!",	"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_MALADICTIONS
    },

    {
	{"deafen","saðýrlýk"},       {  93,93,93,93,93,42,93,93 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_deafen,      TAR_CHAR_OFFENSIVE,   POS_FIGHTING,
        &gsn_deafen,       SLOT(570),        40,       12,
        "saðýrlýk", "Kulaklarýndaki çýnlama sonunda yokoluyor.", "", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"demonfire","iblis ateþi"},		{  93, 37, 93, 93,93,93,93,93,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_demonfire,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_demonfire,			SLOT(505),	20,	12,
  "iblis ateþi",		"!Demonfire!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_ATTACK
    },

    {
	{"desert fist","çöl yumruðu"},		{ 93, 58, 93,93,93,93,93,93,93,93,93,93,55 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_desert_fist, 	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(681),	50,	12,
  "çöl yumruðu",	"!desert fist!", "", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE,GROUP_NONE
    },

    {
	{"detect evil","kem saptama"},		{ 12,  11, 93, 93,3,93,93,93 ,12,93,93,93,13},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_detect_evil,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(18),	 5,	12,
  "",			"Kem saptama sönüyor.",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_DETECTION
    },

    {
	{"detect good","iyi saptama"},         { 12,  11, 93, 93,3,93,93,93 ,12,93,13,13,13},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_detect_good,      TAR_CHAR_SELF,          POS_STANDING,
        NULL,                   SLOT(513),        5,     12,
        "",                     "Ýyiyi saptama sönüyor.",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_DETECTION
    },

    {
	{"detect hide","saklýyý saptama"},	{ 93, 93, 10, 93, 93, 93, 10, 93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_CHAR_SELF,		POS_STANDING,
	&gsn_detect_hidden,	SLOT(0),	 5,	12,
  "",			"Saklýyý saptama sönüyor.",
	"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_DETECTION
    },

    {
	{"detect invis","görünmezi saptama"},		{  5,  17, 93, 93,23,13,93,93 ,10,93,23,13,8},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_detect_invis,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(19),	 5,	12,
  "",			"Görünmezi saptama sönüyor.",
	"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_DETECTION
    },

    {
	{"detect magic","büyü saptama"},	{  8,  12, 93, 93,93,93,93,93,6,30,93,15,12},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_detect_magic,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(20),	 5,	12,
  "",			"Büyü saptama sönüyor.",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_DETECTION
    },

    {
	{"detect poison","zehir saptama"},	{ 35,  39, 93, 93,93,93,93,93,35 ,39,31,21,7},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_detect_poison,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(21),	 5,	12,
	"",			"!Detect Poison!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_DETECTION
    },

    {
{"detect undead","hortlak saptama"},	{ 93, 93,93, 93,93,93,93,93 ,93,93,93,13,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_detect_undead,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,		SLOT(594),	5,	12,
  "",		"Hortlak saptama sönüyor.", "",
	CABAL_NONE, 	RACE_NONE, ALIGN_NONE, GROUP_DETECTION
    },

    {
	{"disenchant armor","zýrh alçalt"},	{ 27, 93, 93, 93, 93, 93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_disenchant_armor,		TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(705),	50,	24,
	"",		"!disenchant armor!",	"", 	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"disenchant weapon","silah alçalt"},		{ 34, 93, 93, 93, 93, 93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_disenchant_weapon,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(706),	50,	24,
	"",		"!disenchant weapon!",	"", 	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"disintegrate","ufala"},       {  75,93,93,93,93,93,93,93,93,93 ,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_disintegrate,      TAR_CHAR_OFFENSIVE,   POS_STANDING,
        NULL,                    SLOT(574),    100,       18,
        "ince ýþýk ýþýný", "!disintegrate!", "",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"dismantle","parçala"},	{ 93, 93,93, 93,93,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,		SLOT(621),	200,	24,
	"",	"!621!",	"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"dispel evil","kem defet"},		{ 93, 27, 93, 93,35,93,93,93,55 ,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_dispel_evil,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(22),	15,	12,
  "kem defet",		"!Dispel Evil!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_ATTACK
    },

    {
	{"dispel good","iyilik defet"},          {   93,  27,  93,  93,93,30,93,93,57,93,32,32,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_dispel_good,      TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,                   SLOT(512),      15,     12,
        "iyilik defet",          "!Dispel Good!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_ATTACK
    },

    {
	{"dispel magic","büyü defet"},	{ 36, 43, 93, 93,93,93,93,93 ,41,93,24,24,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_dispel_magic,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(59),	15,	12,
	"",			"!Dispel Magic!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	{"disruption","nefes kesme"},	{ 93, 93, 93, 93, 93, 93, 93,93,93,93,93,40,93},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_disruption,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(648),	20,	12,
  "nefes kesme",	"!disruption!", "", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{"dragon breath","ejderha nefesi"}, {93,93,93,93,93,93,93,93,93,93,93,93,93},
	{1,1,1,1,1,1,1,1,1, 1,1,1,1},
        spell_dragon_breath,  TAR_CHAR_OFFENSIVE, POS_FIGHTING,
        &gsn_dragon_breath,  SLOT(563),   75,   12,
        "ejderha nefesi", "!dragon breath!", "",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"dragon skin","ejderha derisi"},	{ 10, 10,10, 10,10,10,10,10 ,10,10,10,10,10},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_dragon_skin,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(612),	50,	24,
  "",	"Derin yumuþuyor.",	"",
	CABAL_NONE , RACE_TOP, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	{"dragon strength","ejderha gücü"}, {93,93,93,93,93,93,93,93,93,93,93,93,93},
	{1,1,1,1,1,1,1,1, 1, 1,1,1,1},
        spell_dragon_strength, TAR_CHAR_SELF, POS_FIGHTING,
        &gsn_dragon_strength, SLOT(562),    75,    12,
        "", "Ejderha gücünün seni terkettiðini hissediyorsun.", "",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"dragons breath","ejderha nefesi"},	{ 61, 93, 93, 93, 93, 93, 93, 93 ,61,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_dragons_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(576),	200,	24,
  "ejderha nefesi",	"Saðlýðýna kavuþuyorsun.",
	"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_DRACONIAN
    },

    {
	{"drain","kurut"},		{ 31, 93, 93, 93, 93, 93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_drain,		TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(704),	5,	12,
	"",		"!drain!",	"", 	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_MALADICTIONS
    },

    {
	{"earthfade","arz soluþu"},		{ 93, 93, 93, 93, 93, 93,93,93,93,93,93,93,73},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_earthfade,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	&gsn_earthfade,			SLOT(702),	100,	12,
  "",		"Doðal formuna dönüyorsun.",	"",
 	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"earthmaw","fay"},		{ 93, 93, 93, 93, 93, 93,93,93,93,93,93,93,67},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_earthmaw,		TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(703),	30,	12,
	"earthmaw",		"!earthmaw!",	"", 	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"earthquake","deprem"},	{ 93,  19, 93, 93,93,93,93,93,93,93,93,93,19 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_earthquake,	TAR_IGNORE,		POS_FIGHTING,
	NULL,			SLOT(23),	15,	12,
  "deprem",		"!Deprem!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_ATTACK
    },

    {
	{"elemental sphere","uþak küresi"},	{ 93, 93,93,93, 93,93,93,93 ,93,93,93,93,29},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_elemental_sphere,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(719),	75,	12,
  "",	"Koruyucu uþak küresi yokoluyor.",	"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"enchant armor","zýrh yükselt"},	{  26, 93, 93, 93,93,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_enchant_armor,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(510),	100,	24,
	"",			"!Enchant Armor!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_ENCHANTMENT
    },

    {
	{"enchant weapon","silah yükselt"},{  33, 93, 93, 93,93,93,93,93,17,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_enchant_weapon,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(24),	100,	24,
	"",			"!Enchant Weapon!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_ENCHANTMENT
    },

    {
	{"energy drain","yaþam kurut"},		{ 38,42,93,93,93,26,93,93,93,93,93,45,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_energy_drain,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(25),	35,	12,
  "yaþam kurut",		"!Energy Drain!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_MALADICTIONS
    },

    {
	{"enhanced armor","geliþmiþ zýrh"},	{ 48, 33, 93,93,93,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
	spell_enhanced_armor, 	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	&gsn_enhanced_armor,	SLOT(583),	20,	12,
  "",	"Korunmasýz hissediyorsun.", "", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	{"enlarge","geniþleme"},		{  93,93,93,93, 93,93,93,93, 38,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_enlarge,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(711),	 50,	12,
  "",	"Doðal boyutlarýna dönüyorsun.",	"", CABAL_NONE,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"etheral fist","ruhani yumruk"},		{ 93,93,93,93, 93, 50,93,93,93,93,93,93,93},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_etheral_fist,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(645),	20,	12,
  "ruhani yumruk",		"!Etheral Fist!", "", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{"faerie fire","cin ateþi"},		{ 11,  6, 93, 93,93,93,8,93 ,15 ,93,93,93,11},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_faerie_fire,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_faerie_fire,		SLOT(72),	 5,	12,
  "cin ateþi",		"Pembe aylandan kurtuluyorsun.",
	"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_WEATHER
    },

    {
	{"faerie fog","cin sisi"},	{ 24, 31, 93, 93,93,93,93,93 ,27,93,93,93,24},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_faerie_fog,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(73),	12,	12,
  "cin sisi",		"!Faerie Fog!",		"", CABAL_NONE ,
	RACE_NONE,ALIGN_NONE, GROUP_WEATHER
    },

    {
	{"farsight","uzak görüþ"},	{  22, 93, 93, 93,93,93,93,93,22,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_farsight,		TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(521),	20,	12,
  "uzak görüþ",		"!Farsight!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_DETECTION
    },

    {
	{"fear","korku"},			{   93, 93, 93, 93,93,93,93,93,93 ,93,93,73,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_fear,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_fear,	SLOT(598),	50,	12,
  "",		"Daha cesur hissediyorsun.",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_ILLUSION
    },

    {
	{"fire and ice","ateþ ve buz"},		{ 93, 93, 93, 93, 93, 93,93,93,93,93,93,93,65},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_fire_and_ice,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(699),	40,	12,
  "ateþ ve buz",		"!fire and ice!",	"", 	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{"fireball","alev topu"},		{55,93,93,93,93,56,93,93,56,93,93,93,55},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_fireball,		TAR_IGNORE,	POS_FIGHTING,
	NULL,			SLOT(26),	25,	12,
  "alev topu",		"!Fireball!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{"fireproof","ateþ geçirmez"},		{ 25, 16, 93, 93, 20, 93,93,93,21,93,93,42,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_fireproof,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(523),	10,	12,
  "",		"",	"$p üzerindeki koruyucu ayla yokoluyor.", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_ENCHANTMENT
    },

    {
	{"firestream","alev akýmý"},	{ 93, 93, 93, 93, 93, 93,93,93,93,93,93,93,35},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_firestream,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(692),	20,	12,
	"",		"",	"", 	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{"fire shield","alev kalkaný"},			{   93, 93, 93, 93,93,93,93,93,93 ,93,93,93,68},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_fire_shield,	TAR_IGNORE,	POS_STANDING,
	&gsn_fire_shield,		SLOT(601),	200,	24,
	"",			"!fire shield!",
	"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"flamestrike","alev saldýrýsý"},			{93,42,93,93,32,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_flamestrike,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(65),	20,	12,
  "alev saldýrýsý",		"!Flamestrike!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_ATTACK
    },

    {
	{"fly","uçma"},				{ 19, 30, 93,93,93,93,93,93,19,93,43,23,21},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_fly,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_fly,			SLOT(56),	10,	18,
  "",		"Yere iniyorsun.",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_TRANSPORTATION
    },

    {
	{"floating disc","asýlý disk"},		{ 4, 10, 93, 93, 93, 93, 93, 93 ,4,93,93,93,4},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_floating_disc,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(522),	40,	24,
	"",			"!Floating disc!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"forcecage","güç kafesi"},			{ 29, 93,93,93, 93,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_forcecage,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(717),	75,	12,
  "",	"Çevrendeki güç kafesi yokoluyor.",	"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"frenzy","cinnet"},               {  93, 34, 93, 93,93,93,93,93,93,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_frenzy,           TAR_CHAR_DEFENSIVE,     POS_STANDING,
        NULL,                   SLOT(504),      30,     24,
        "",                     "Cinnetin geçiyor.",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_BENEDICTIONS
    },

    {
	{"frostbolt","ayaz topu"},		{ 93, 93, 93, 93, 93, 93,93,93,93,93,93,93,42},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_frostbolt,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(695),	20,	12,
	"ayaz topu",		"!frostbolt!",	"", 	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{"fumble","beceriksizlik"},		{ 93,93,93,93, 93,93,93,93 ,36,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_fumble,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(712), 	25,	18,
  "",		"Hýzlanýyor ve eski gücüne kavuþuyorsun!",	"",
	CABAL_NONE, RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"galvanic whip","enerji kýrbacý"},	{ 28, 93, 93, 93, 93, 93,93,93,93,93,93,93,93},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_galvanic_whip,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(655),	20,	12,
  "enerji kýrbacý",	"!galvanic whip!", "", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{"gate","geçit"},			{93,63,93,93,93,93,93,93,27,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_gate,		TAR_IGNORE,		POS_FIGHTING,
	NULL,			SLOT(83),	80,	12,
	"",			"!Gate!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_TRANSPORTATION
    },

    {
	{"giant strength","dev gücü"},{  20, 39, 93, 93,93,93,93,93, 30,93,93,28,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_giant_strength,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_giant_strength,		SLOT(39),	20,	12,
  "",			"Gücün azalýyor.",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_ENHANCEMENT
    },

    {
	{"grounding","topraklama"},		{ 93, 93, 93, 93, 93, 93,93,93,93,93,93,93,53},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_grounding,	TAR_CHAR_SELF,		POS_FIGHTING,
	NULL,			SLOT(700),	50,	12,
  "",		"Topraklamaný yitiriyorsun.",	"",
 	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"group defense","grup savunma"},	{   93, 35, 93,93,93,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_group_defense,		TAR_IGNORE,		POS_STANDING,
	NULL,		SLOT(586),	100,	36,
  "",			"Korunmasýz hissediyorsun.",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_BENEDICTIONS
    },

    {
	{"group heal","grup þifa"},			{  93,65,93,93,75,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_group_healing,	TAR_CHAR_DEFENSIVE, POS_FIGHTING,
	NULL,			SLOT(642),	500,	24,
	"",			"!Group Heal!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_HEALING
    },

    {
	{"hallucination","sanrý"},	{   93, 93, 93, 93,93,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_hallucination,	TAR_CHAR_SELF,	POS_STANDING,
	NULL,		SLOT(606),	200,	12,
  "",			"Büyüye karþý yine savunmasýzsýn.",
	"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_ILLUSION
    },

    {
	{"hand of undead","hortlak eli"},	{ 93, 93, 93,93,93,93,93,93,93,93,93,44,93},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_hand_of_undead,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(647),	20,	24,
  "hortlak eli",	"!hand of undead!", "", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{"harm","zarar"},			{  93, 26, 93, 93,93,29,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_harm,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(27),	35,	12,
  "zarar",		"!Harm!",	"", 	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_HARMFUL
    },

    {
	{"haste","hýz"},	{ 24, 93, 93,93,93,93,93,93,24,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_haste,		TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	&gsn_haste,		SLOT(502),	30,	12,
  "",			"Yavaþladýðýný hissediyorsun.",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_ENHANCEMENT
    },

    {
	{"heal","þifa"},		{  93, 28, 93, 93,29,93,93,93 ,93 ,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_heal,		TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(28),	50,	12,
	"",			"!Heal!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_HEALING
    },

    {
	{"healing light","þifa ýþýðý"},    { 93, 41, 93, 93,93, 93, 93, 93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_healing_light,	TAR_IGNORE,	POS_STANDING,
	NULL,			SLOT(613),	200,	24,
  "",			"Bir odayý yeniden ýþýðýnla doldurabilirsin.",	"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_BENEDICTIONS
    },

    {
	{"heat metal","metali ýsýtma"},		{ 93,26,93,93, 93,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_heat_metal,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(516), 	25,	18,
  "metali ýsýtma",		"!Heat Metal!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"helical flow","sarmal akým"},		{93,93,93,93,93,93,93,93,93,93,93,93,43},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_helical_flow,		TAR_IGNORE,	POS_FIGHTING,
	NULL,			SLOT(661),	80,	12,
	"",			"!Helical Flow!",	"",CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_TRANSPORTATION
    },

    {
	{"hellfire","cehennem ateþi"},     {  93, 93, 93, 93, 93, 93, 45, 93 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_hellfire,      TAR_CHAR_OFFENSIVE,          POS_FIGHTING,
        NULL,       SLOT(520),        20,       12,
        "cehennem ateþi", "!hellfire!", "",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_ATTACK
    },

    {
	{"holy aura","kutsal ayla"},{ 93, 93,93,93,62,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_holy_aura,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,		SLOT(617),	75,	12,
  "",	"Kutsal aylan yokoluyor.",	"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"holy fury","kutsal hiddet"},	{ 93, 93,93,93,59,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_holy_fury,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,		SLOT(682),	50,	24,
  "",	"Artýk daha merhametlisin.",	"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"holy word","kutsal söz"},		{ 93,48,93,93,38,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_holy_word,	TAR_IGNORE,	POS_FIGHTING,
	NULL,			SLOT(506), 	200,	24,
  "kutsal söz",		"!Holy Word!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_BENEDICTIONS
    },

    {
	{"hurricane","kasýrga"},	{  93, 93, 93, 93, 93,53,93,93,93,93,93,65,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_hurricane,	TAR_IGNORE,	POS_FIGHTING,
	NULL,		SLOT(672),	200,	24,
  "kasýrga",	"!Hurricane!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"hydroblast","su patlamasý"},	{  93,93,93,93, 93,93,93,46, 93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_hydroblast,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(684),	50,	12,
  "su patlamasý",	"!Hydroblast!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"iceball","buztopu"},        {  93, 93, 93, 93, 93,58,93,93,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_iceball,     TAR_IGNORE,     POS_FIGHTING,
        NULL,                   SLOT(513),        25,       12,
        "buztopu",         "!Iceball!", "",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{"identify","tanýmlama"},		{ 18, 36, 93, 93,93,93,93,93 ,25,93,93,93,18},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_identify,		TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(53),	12,	24,
	"",			"!Identify!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_DETECTION
    },

    {
	{"improved detect","geliþmiþ saptama"},	{ 40, 93, 93, 93, 93,93,93, 93 ,40,93,93,40,40},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_improved_detection,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,		SLOT(626),	 20,	12,
  "",			"Artýk çevrende olup bitenden daha az haberdarsýn.",
	"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_DETECTION
    },

    {
	{"improved invis","geliþmiþ görünmezlik"},	{  43, 93, 93, 93,93,93,93,93,43,93,93,93,43 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_improved_invis,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_imp_invis,		SLOT(627),	 20,	12,
  "",			"Görünmezliðin yitiyor.",
	"$p görünürleþiyor.", CABAL_NONE , RACE_NONE,ALIGN_NONE, GROUP_ILLUSION
    },

    {
	{"infravision","ýsý görüþü"},		{  17,  25, 93, 93,93,93,5,93,18,93,93,21,17},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_infravision,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(77),	 5,	18,
  "",			"Artýk karanlýkta göremeyeceksin.",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_ENHANCEMENT
    },

    {
	{"insanity","delilik"},	{ 93, 93,93, 93,93,93,93,93 ,93,93,93,59,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_insanity,		TAR_CHAR_OFFENSIVE,	POS_STANDING,
	NULL,		SLOT(616),	100,	24,
  "",	"Sakinleþtiðini hissediyorsun.",	"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"inspire","ilham"},		{   93, 49, 93,93,93,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_inspire,		TAR_IGNORE,		POS_STANDING,
	NULL,		SLOT(587),	75,	24,
  "",			"Ýlhamýn azalýyor.",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_BENEDICTIONS
    },

    {
	{"invisibility","görünmezlik"},		{  10, 93, 93, 93,93,9,93,93,13,93,93,93,10 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_invis,		TAR_OBJ_CHAR_DEF,	POS_STANDING,
	&gsn_invis,		SLOT(29),	 5,	12,
	"",			"Artýk görünmez deðilsin.",
	"$p görünür oluyor.", CABAL_NONE , RACE_NONE,ALIGN_NONE, GROUP_ILLUSION
    },

    {
{"iron body","demir gövde"},	{ 93, 93,93,93, 93,93,93,93 ,29,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_iron_body,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(718),	75,	12,
  "",	"Derin yumuþuyor.",	"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"iron golem","demir golem"},		{93,93,93,93,93,93,93,93,93,93,93,52,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_iron_golem,	TAR_IGNORE,		POS_STANDING,
	NULL,		SLOT(664),	400,		24,
  "",	"Yeni golemler yapmak için yeterli gücü topladýn.","",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"knock","taktak"},			{   56,93,93,93,93,93,93,93,64,93,93,62,46},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_knock,	TAR_IGNORE,	POS_STANDING,
	NULL,		SLOT(603),	20,	24,
	"",			"!knock!",
	"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"know alignment","yönelim saptama"},	{  22,  13,93,93,1,5,93,93,29,93,93,22,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_know_alignment,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(58),	 9,	12,
	"",			"!Know Alignment!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_DETECTION
    },

    {
	{"lesser golem","küçük golem"},		{93,93,93,93,93,93,93,93,93,93,93,25,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_lesser_golem,	TAR_IGNORE,		POS_STANDING,
	NULL,		SLOT(662),	200,		12,
  "",	"Yeni golemler yapmak için yeterli gücü topladýn.","",
	CABAL_NONE , 	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"lethargic mist","uyuþukluk sisi"},   { 52, 93,93,93,93, 62, 93,93,56,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_lethargic_mist,	TAR_IGNORE,	POS_STANDING,
	&gsn_lethargic_mist,	SLOT(676),	200,	24,
	"",		"!lethargic mist!",	"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_MALADICTIONS
    },

    {
	{"light arrow", "ýþýk oku"},	{  93, 93, 93, 93,64,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_light_arrow,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(683),	40,	12,
	"ýþýk oku",	"!light arrow!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{"lightning bolt","yýldýrým"},	{  23, 93, 93, 93,93,13,93,93 ,23,93,93,23,23},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_lightning_bolt,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_lightning_bolt,		SLOT(30),	15,	12,
  "yýldýrým",	"!Lightning Bolt!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{"lightning shield","þimþek kalkaný"},	{ 41, 93,93, 93,93,93,93,93 ,93,93,93,41,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_lightning_shield,	TAR_IGNORE,	POS_STANDING,
	NULL,		SLOT(614),	150,	24,
  "þimþek kalkaný",	"Bir odayý yeniden kalkanlayabilirsin.",	"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"link","bað"},	{   31, 93, 93, 93,93,93,93,93,33 ,93,93,33,33},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_link,		TAR_CHAR_DEFENSIVE,		POS_STANDING,
	NULL,		SLOT(588),	125,	18,
	"",			"!link!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_MEDITATION
    },

    {
	{"lion help","aslan yardýmý"},	{   93, 93, 93, 93,93,93,93,63,93 ,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_lion_help,	TAR_IGNORE,		POS_STANDING,
	NULL,		SLOT(595),	100,	12,
  "",	"Yeniden aslan yardýmýna baþvurabilirsin.",	"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"locate object","nesne bulma"},	{   30, 30, 93, 93,19,93,14,93,29,93,93,93,31},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_locate_object,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(31),	20,	18,
	"",			"!Locate Object!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_DETECTION
    },

    {
	{"love potion","aþk iksiri"},           { 93, 93, 93,93,93,93,93,93 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_love_potion, TAR_CHAR_SELF,       POS_STANDING,
        &gsn_love_potion,  SLOT(666),        10,       0,
        "", "Gözlerindeki rüya sönüyor.", "",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"magic jar","büyülü þiþe"},		{   93, 93, 93, 93,93,93,93,93,93 ,93,93,68,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_magic_jar,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(596),	20,	12,
	"",			"!magic jar!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_BEGUILING
    },

    {
	{"magic missile","fiþek"},	{ 1, 93, 93, 93,93,1,93,93 ,1,93,93,2,1},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_magic_missile,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_magic_missile,		SLOT(32),	15,	12,
	"fiþek",	"!Magic Missile!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{"magic resistance","büyü direnci"},	{   68, 93, 93, 93,93,93,93,93,72 ,93,93,69,71},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_magic_resistance,	TAR_CHAR_SELF,	POS_STANDING,
	NULL,		SLOT(605),	200,	24,
  "",			"Büyüye karþý yeniden savunmasýzsýn.",
	"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	{"magnetic trust","manyetizma"},{ 93, 93, 93, 93, 93, 93,93,93,34,93,93,93,34},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_magnetic_trust,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(656),	20,	12,
  "manyetizma",	"!magnetic trust!", "", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{"mass healing","toplu þifa"},	{ 93, 54, 93, 93,93,93,93,93,93 ,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_mass_healing,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(508),	100,	36,
	"",			"!Mass Healing!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_HEALING
    },

    {
	{"mass invis","toplu görünmezlik"},		{ 43, 47, 93, 93,93,93,93,93 ,42,93,93,93,46},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_mass_invis,	TAR_IGNORE,		POS_STANDING,
	&gsn_mass_invis,	SLOT(69),	20,	24,
  "",			"Görünmezliðin yitiyor.",		"", CABAL_NONE ,
	RACE_NONE,ALIGN_NONE, GROUP_ILLUSION
    },

    {
	{"mass sanctuary","toplu takdis"},{   93, 62, 93, 93,93,93,93,93,93 ,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_mass_sanctuary,	TAR_IGNORE,		POS_STANDING,
	NULL,		SLOT(589),	200,	24,
  "",			"Beyaz auran yokoluyor.",
	"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	{"master healing","yüksek tedavi"},	{  93,58,93,93,70,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_master_heal,	TAR_CHAR_DEFENSIVE,POS_FIGHTING,
	NULL,			SLOT(641),	300,	12,
	"",			"!Master Heal!",	"",CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_HEALING
    },

    {
	{"meld into stone","taþa dönüþme"},	{ 20, 20,20, 20,20,20,20,20 ,20,20,20,20,20},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_meld_into_stone,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(584),	12,	18,
  "",	"Derindeki taþlar kumlaþarak dökülüyor.",	"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	{"mend","onar"},	{   50, 93, 93, 93,93,93,93,93,50 ,93,93,93,50},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_mend,		TAR_OBJ_INV,		POS_STANDING,
	&gsn_mend,		SLOT(590),	150,	24,
	"",			"!mend!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_ENCHANTMENT
    },

    {
	{"mind light","zihin ýþýðý"},	{ 93, 83,93, 93,93,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_mind_light,	TAR_IGNORE,		POS_STANDING,
	NULL,		SLOT(82),	200,	24,
  "",	"Artýk yeni odalarý hýzlandýrabilirsin.",	"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"mind wrack","zihin yosunu"},{ 93, 93, 93, 93, 93, 27, 93,93,93,93,93,93,93},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_mind_wrack,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(650),	20,	12,
  "zihin yosunu",	"!mind wrack!", "", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{"mind wrench","zihin bükümü"},	{ 93, 93, 93, 93, 93, 40, 93,93,93,93,93,93,93},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_mind_wrench,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(651),	20,	12,
  "zihin bükümü",	"!mind wrench!", "", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{"mist walk","gizemli yürüyüþ"},		{  93,93,93,93,93,93,93,93,93,93,47,47,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_mist_walk,	TAR_IGNORE,	POS_FIGHTING,
	NULL,			SLOT(658),	80,	12,
	"",			"!Mist Walk!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_TRANSPORTATION
    },

    {
	{"mummify","mumyala"},	{ 93, 93, 93, 93,93,93,93,93,93,93,93,44,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_mummify,	TAR_OBJ_CHAR_OFF,	POS_STANDING,
	NULL,		SLOT(715),	50,	12,
  "",	"Yeni ölülere hayat verebilirsin.", 	"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"mysterious dream","gizemli düþ"}, 	{ 93, 93,93,93,93,35,93,93,49,93,93,27,93},
		{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_mysterious_dream,	TAR_IGNORE,	POS_STANDING,
	&gsn_mysterious_dream,	SLOT(678),	200,	24,
	"",		"!mysterous dream!",	"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_BEGUILING
    },

    {
	{"nexus","neksus"},     { 93,93,93,93,93,93,93,93 ,60,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_nexus,            TAR_IGNORE,             POS_STANDING,
        NULL,                   SLOT(520),       150,   36,
	"",                     "!Nexus!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_TRANSPORTATION
    },

    {
	{"pass door","saydamlýk"},		{  24, 32, 93, 93,93,93,93,93 ,24,24,93,27,24},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_pass_door,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(74),	20,	12,
  "",			"Cisimleþiyorsun.",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_TRANSPORTATION
    },

    {
	{"plague","veba"},		{  93, 93, 93, 93,93,46,93,93,33,93,93,36,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_plague,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_plague,		SLOT(503),	20,	12,
  "veba",		"Hastalýðýn çürüyor.",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_MALADICTIONS
    },

    {
	{"poison","zehir"},		{  93,  22, 93,93,93,15,93,93,27,93,93,23,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_poison,		TAR_OBJ_CHAR_OFF,	POS_FIGHTING,
	&gsn_poison,		SLOT(33),	10,	12,
  "zehir",		"Üzerindeki zehir kuruyor.",
	"$p üzerindeki zehir kuruyor.", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_MALADICTIONS
    },

    {
	{"polymorph","binbir þekil"},      { 93, 93, 93, 93,93, 93,93,93,73,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_polymorph,	TAR_IGNORE,	POS_STANDING,
	NULL,			SLOT(639),	250,	24,
  "",			"Doðal ýrkýna dönüyorsun.","",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_BENEDICTIONS
    },

    {
	{"portal","köprü"},                { 54, 93, 93, 93, 93,93,93,93,40,93,93,43,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_portal,           TAR_IGNORE,             POS_STANDING,
        NULL,                   SLOT(519),       100,     24,
	"",                     "!Portal!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_TRANSPORTATION
    },

    {
	{"protection cold","soðuktan korunma"},{   93, 44, 93, 93, 93,93,93,25, 93,93,93,50,44},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_protection_cold,	TAR_CHAR_SELF,	POS_STANDING,
	&gsn_protection_cold,		SLOT(600),	5,	12,
  "",			"Daha az korunmasýz hissediyorsun.",
	"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	{"protection evil","kemden korunma"},	{ 93,  9, 93, 93,11,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_protection_evil,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(34), 	5,	12,
  "",			"Daha az korunmasýz hissediyorsun.",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	{"protection good","iyiden korunma"},{ 93,9,93,93,93,13,93,93,93,93,27,17,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_protection_good,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(666), 	5,	12,
  "",			"Daha az korunmasýz hissediyorsun.",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	{"protection heat","sýcaktan korunma"},	{ 93, 39, 93, 93,93,93,93,93, 93,93,93,48,39},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_protection_heat,	TAR_CHAR_SELF,	POS_STANDING,
	&gsn_protection_heat,		SLOT(599),	5,	12,
  "",			"Daha az korunmasýz hissediyorsun.",
	"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	{"protection negative","negatiften korunma"},{   93, 93,93,93,93,93,93,93,93,93,93,15,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_protection_negative,	TAR_CHAR_SELF,	POS_STANDING,
	NULL,		SLOT(636),	20,	12,
  "",		"Kendi saldýrýlarýna karþý daha az korunmasýz hissediyorsun.",
	"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	{"protective shield","koruma kalkaný"},     { 26,38,93,93, 93,93,93,93 ,26,93,93,18,26},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_protective_shield,      TAR_CHAR_SELF,   POS_STANDING,
        &gsn_protective_shield,       SLOT(572),        70,       12,
        "", "Kalkanýn sönüyor.", "", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	{"power word kill","ölüm sözcüðü"},		{   93, 93, 93, 93,93,93,93,93,93 ,93,93,78,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_power_kill,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
	NULL,		SLOT(604),	200,	18,
  "ölüm sözcüðü",		"Dayanýklýlýðýn artýyor.",
	"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"power word stun","bayýltma sözcüðü"},{ 93, 93,93, 93,93, 62,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_power_stun,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_power_stun,		SLOT(625),	200,	24,
  "",	"Hareket edebilirsin.",	"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"quantum spike","enerji dikeni"},	{ 41, 93, 93, 93, 93, 93,93,93,93,93,93,93,41},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_quantum_spike,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(657),	20,	12,
  "enerji dikeni",	"!quantum spike!", "", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{"ranger staff","korucu asasý"},          {  93, 93, 93, 93, 93, 93, 93, 32 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_ranger_staff,      TAR_IGNORE,          POS_FIGHTING,
        NULL,             SLOT(519),        75,       0,
	"", "!ranger staff!", "", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"ray of truth","ilahi enerji"},        { 93, 37, 93, 93, 45,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_ray_of_truth,     TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        &gsn_ray_of_truth,      SLOT(518),      20,     12,
        "ilahi enerji",         "!Ray of Truth!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_BENEDICTIONS
    },

    {
	{"recharge","þarj"},		{ 29,93,93,93, 93, 93, 93, 93, 29,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_recharge,		TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(517),	60,	24,
	"",			"!Recharge!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_ENCHANTMENT
    },

    {
	{"refresh","yenileme"},		{  93,7, 93,93, 93,93,93,93, 93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_refresh,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(81),	12,	18,
  "yenileme",		"!Refresh!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_HEALING
    },

    {
	{"reincarnation","dirilme"},    {  93, 93, 93, 93, 93,93,93,93,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,        TAR_IGNORE,          POS_STANDING,
        NULL,             SLOT(668),        0,       0,
	"", "!!", "",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"remove curse","lanet kaldýr"},	{  93, 32, 93, 93,13,93,93,93,93,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_remove_curse,	TAR_OBJ_CHAR_DEF,	POS_STANDING,
	&gsn_remove_curse,	SLOT(35),	 5,	12,
	"",			"!Remove Curse!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_CURATIVE
    },

    {
	{"remove fear","korku kaldýr"},		{ 93, 34, 93,93,41,93,93,93,93,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_remove_fear, 	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,		SLOT(582),	5,	12,
	"",	"!Remove Fear!", "", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"remove tattoo","dövme kaldýr"},    { 93, 93, 93,93,93,93,93,93,93,93,93,93,93 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_remove_tattoo, TAR_CHAR_DEFENSIVE,       POS_STANDING,
        NULL,       SLOT(552),        10,       0,
	"", 	"!remove tattoo!", 	"",
        CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"resilience","esneklik"},	{  93,93, 93, 93,56,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_resilience,	TAR_CHAR_DEFENSIVE, POS_STANDING,
	NULL,			SLOT( 638),	 50,	12,
  "",	"Emici saldýrýlara karþý koruman azalýyor.", "",
	CABAL_NONE ,RACE_NONE, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	{"restoring light","iyileþtiren ýþýk"},   { 93, 71, 93,93,93,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_restoring_light,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(643),	50,	24,
	"",			"!restoring light!","",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_BENEDICTIONS
    },

    {
	{"sanctify lands","kutsal topraklar"},  { 93, 41, 93, 93,53, 93, 93, 93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_sanctify_lands,	TAR_IGNORE,	POS_STANDING,
	NULL,			SLOT(673),	200,	24,
	"",		"!sanctify lands!",	"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_BENEDICTIONS
    },

    {
	{"sanctuary","takdis"},			{ 93, 29, 93, 93,27,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_sanctuary,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_sanctuary,		SLOT(36),	75,	12,
  "",			"Beyaz auran yokoluyor.",
	"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	{"sand storm","kum fýrtýnasý"},	{ 93, 93, 93, 93,93,93,93,93,93,93,93,93,51 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_sand_storm,	TAR_IGNORE,	POS_FIGHTING,
	&gsn_sand_storm,		SLOT(577),	200,	24,
  "kum fýrtýnasý",	"Gözlerindeki kum dökülüyor.","", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"scream","çýðlýk"},		{ 52, 93, 93,93,93,93,93,93,48,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_scream,	TAR_IGNORE,	POS_FIGHTING,
	&gsn_scream,		SLOT(578),	200,	24,
  "çýðlýk",	"Yeniden duyabiliyorsun.",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"severity force","arz kýrýmý"},	{ 93, 45,93,93,93, 93, 93, 93,93,93 ,93,93,41},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_severity_force,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(622),	20,	12,
  "arz kýrýmý",		"!severity force!", "", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"shield","kalkan"},	{ 20, 35, 93, 93,93,93,19,93,93 ,93,20,20,20},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_shield,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_shield,		SLOT(67),	12,	18,
  "",	"Güç kalkanýn parlayarak yokoluyor.", 	"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	{"shielding","kalkanlama"},		{   93, 93, 93, 93,93,93,93,93,53 ,93,93,53,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_shielding,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_shielding,		SLOT(591),	250,	12,
  "",			"Gerçek Kaynak'tan uzaklaþtýðýný hissediyorsun.",
	"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"shocking grasp","þok kavrayýþý"},		{ 8, 93, 93, 93,93,93,93,93 ,9,93,93,93,8},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_shocking_grasp,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(53),	15,	12,
  "þok kavrayýþý",	"!Shocking Grasp!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{"shocking trap","þokçu tuzak"},	{ 37, 93,93, 93,93,93,93,93 ,40,93,93,37,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_shocking_trap,	TAR_IGNORE,	POS_STANDING,
	NULL,		SLOT(615),	150,	24,
  "þokçu tuzak",	"Daha fazla odayý þokla doldurabilirsin.","",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"sleep","uyku"},	{ 93, 93, 93, 93,93,25,93,93,39,93,93,39,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_sleep,		TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_sleep,		SLOT(38),	15,	12,
  "",			"Yorgunluðun geçiyor.",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_BEGUILING
    },

    {
	{"slow","yavaþlatma"},       { 28,38,93,93, 93, 28, 93, 93 ,23,93,28,29,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_slow,             TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        &gsn_slow,                   SLOT(515),      30,     12,
        "",                     "Hýzlandýðýný hissediyorsun.",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_MALADICTIONS
    },

    {
	{"soften","yumuþatma"},		{ 93, 93, 93, 93,93,93,93,93 ,58,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_soften,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(713),	 75,	12,
  "yumuþatma",	"Derin sertleþiyor.",
	"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_WEATHER
    },

    {
	{"solar flight","güneþ uçuþu"},		{93,35,93,93,93,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_solar_flight,	TAR_IGNORE,	POS_FIGHTING,
	NULL,			SLOT(659),	80,	12,
	"",			"!Solar Flight!",	"",CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_TRANSPORTATION
    },

    {
	{"sonic resonance","ses direnci"},	{ 93, 93, 93, 93, 93, 93,93,93,93,93,93,28,93},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_sonic_resonance,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(649),	20,	12,
  "ses direnci",	"!sonic resonance!", "", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{"soul bind","ruh baðý"},		{ 93, 93, 93, 93,93,39,93,93, 93,93,93,54,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_soul_bind,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
	NULL,		SLOT(716),	 5,	12,
  "",		"Kendine güvenin artýyor.",	"",
	CABAL_NONE, RACE_NONE, ALIGN_NONE, GROUP_BEGUILING
    },

    {
	{"spectral furor","hayali öfke"},	{ 93, 93, 93, 93, 93, 93,93,93,93,93,93,35,93},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_spectral_furor,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(646),	20,	12,
  "hayali öfke",	"!spectral furor!", "", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{"stone golem","taþ golem"},		{93,93,93,93,93,93,93,93,93,93,93,39,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_stone_golem,	TAR_IGNORE,		POS_STANDING,
	NULL,		SLOT(663),	300,		18,
  "",	"Yeni golemler yapmak için yeterli gücü topladýn.","",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"stone skin","taþ deri"},	{ 25, 36, 93,93,93,93,35,93,93,93,60,30,25},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_stone_skin,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(66),	12,	18,
  "",			"Derin yumuþuyor.",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	{"suffocate","boðma"},    {  93,93,93,93, 93,93,93,93, 48,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_suffocate,    TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
        NULL,             SLOT(714),        50,       12,
        "boðma",     "Yeniden soluyabilirsin.", "",CABAL_NONE,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"sulfurus spray","sülfür spreyi"},	{ 93, 93, 93, 93, 93,93,93,93,28,93,93,93,93},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_sulfurus_spray,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(652),	20,	12,
  "sülfür spreyi",	"!sulfurus spray!", "", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{"summon","çaðrý"},		{  23, 22, 93, 93,51,50,93,93,24,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_summon,		TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(40),	50,	12,
	"",			"!Çaðrý!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_TRANSPORTATION
    },

    {
	{"summon air elemental","hava uþaðý"},  { 93, 93, 93, 93, 93, 93,93,93,93,93,93,93,42},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_summon_air_elm,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(696),	50,	12,
  "",	"Hava uþaðý çaðýrabilecek gücü kazanýyorsun.",
	"", 	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"summon earth elemental","toprak uþaðý"},{ 93, 93, 93, 93, 93, 93,93,93,93,93,93,93,38},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_summon_earth_elm,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(693),	50,	12,
  "",	"Toprak uþaðý çaðýrabilecek gücü kazanýyorsun.",
	"", 	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"summon fire elemental","ateþ uþaðý"},{ 93, 93, 93, 93, 93, 93,93,93,93,93,93,93,58},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_summon_fire_elm,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(697),	50,	12,
  "",	"Ateþ uþaðý çaðýrabilecek gücü kazanýyorsun.",
	"", 	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"summon lightning elemental","enerji uþaðý"},{ 93,93,93,93,93, 93,93,93,93,93,93,93,68},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_summon_light_elm,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(710),	50,	12,
  "", "Enerji uþaðý çaðýrabilecek gücü kazanýyorsun.",
	"", 	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"summon water elemental","su uþaðý"},{ 93, 93, 93, 93, 93, 93,93,93,93,93,93,93,48},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_summon_water_elm,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(698),	50,	12,
  "",	"Su uþaðý çaðýrabilecek gücü kazanýyorsun.",
	"", 	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"summon shadow","gölge çaðrýsý"},{ 93, 93,93,93,93,93,93,93,93,93,93,50,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_summon_shadow,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,		SLOT(620),	200,	24,
  "",	"Yeni gölgeler çaðýrabilirsin.",	"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"superior heal","yüksek þifa"},{  93, 38, 93,93,43,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_super_heal,	TAR_CHAR_DEFENSIVE,POS_FIGHTING,
	NULL,			SLOT(640),	100,	12,
	"",			"!Super Heal!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_HEALING
    },

    {
	{"tattoo","dövme"},    { 93, 93, 93,93,93,93,93,93,93,93,93,93,93},
	{1,1,1,1,1,1,1,1,1, 1,1,1,1},
        spell_tattoo, TAR_CHAR_DEFENSIVE,       POS_STANDING,
        NULL,       SLOT(551),        10,       0,
	"", "!tattoo!", "",CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"teleport","nakil"},		{  22, 32, 93, 93,93,7,93,93 ,23,93,93,16,16},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_teleport,		TAR_CHAR_SELF,		POS_FIGHTING,
	&gsn_teleport,	 		SLOT( 2),	35,	12,
	"",			"!Teleport!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_TRANSPORTATION
    },

    {
	{"transfer object","nesne nakli"},	{ 62, 93, 93, 93, 93, 93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_transfer_object,	TAR_IGNORE,		POS_STANDING,
	NULL,		SLOT(708),	40,	12,
	"",		"!transfer object!", "",
 	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"tsunami","tsunami"},{ 93, 93, 93, 93, 93, 93,93,93,93,93,93,93,62},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_tsunami,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(701),	50,	12,
	"raging tidal wave",	"!tsunami!",	"", 	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"turn","devir"},		{   93, 50, 93, 93,60,93,93,93,93 ,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_turn,	TAR_IGNORE,	POS_FIGHTING,
	NULL,		SLOT(597),	50,	12,
  "",	"Devir büyüsünü yeniden kullanabilirsin.",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"vampiric blast","yarasa öfkesi"},	{ 93, 93, 93, 93, 93, 93, 93, 93,93,93 ,38,93,93},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_vam_blast,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(611),	20,	12,
  "yarasa öfkesi",	"!Vampiric Blast!", "",CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"ventriloquate","vantrolog"},	{   1, 93, 93, 93,93,93,93,93, 1,93,93,93,3},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_ventriloquate,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(41),	 5,	12,
	"",			"!Ventriloquate!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_ILLUSION
    },

    {
	{"web","að"},		{   93, 93, 93, 93,93,93,93,93,69 ,93,93,58,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_web,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_web,		SLOT(592),	50,	12,
	"",		"Seni saran aðlar eriyor.",
	"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_ATTACK
    },

    {
	{"windwall","rüzgar duvarý"},		{ 93, 93, 93, 93, 93, 93,93,93,93,93,93,93,47},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_windwall,		TAR_IGNORE,		POS_FIGHTING,
	NULL,			SLOT(694),	20,	12,
  "rüzgar duvarý",		"Gözlerin iyileþiyor.",	"",
 	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{"witch curse","cadý laneti"},	{   93, 93, 93, 93,93,93,93,93,93 ,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_witch_curse,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_witch_curse,		SLOT(602),	150,	24,
  "",			"Dayanýklýlýðýn artýyor.",
	"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"wrath","gazap"},         { 93, 93, 93,93,34,93,93,93,93,93,93,93,93 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_wrath, TAR_CHAR_OFFENSIVE,       POS_STANDING,
	&gsn_wrath,       SLOT(553),        20,       12,
  "gazap", "Lanet yokoluyor.", "",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_BENEDICTIONS
    },

    {
	{"weaken","zayýflatma"},	{   93, 24, 93, 93,93,93,93,93 ,19,93,16,29,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_weaken,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_weaken,		SLOT(68),	20,	12,
  "zayýflatma",		"Güçlendiðini hissediyorsun.",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_MALADICTIONS
    },

    {
	{"word of recall","anýmsama sözcüðü"},		{ 32, 28, 93, 93,32,93,29,93 ,32,38,10,10,32},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_word_of_recall,	TAR_CHAR_SELF,		POS_RESTING,
	NULL,			SLOT(42),	 5,	12,
	"",			"!Word of Recall!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_TRANSPORTATION
    },

/*
 * Dragon breath
 */

    {
	{"acid breath","asit nefesi"},		{  67, 67, 67, 67,67,67,67,67 ,67,67,67,67,67},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_acid_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_acid_breath,		SLOT(200),	100,	24,
  "asit nefesi",	"!Acid Breath!",	"", CABAL_NONE ,
	RACE_TOP, ALIGN_NONE, GROUP_DRACONIAN
    },

    {
	{"desert heat","çöl sýcaðý"},		{ 69, 69, 69, 69,69,69,69,69,69,69,69,69,69 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(629),	200,	24,
  "çöl sýcaðý","Gözlerindeki duman yokoluyor.",	"", CABAL_NONE ,
	RACE_TOP, ALIGN_NONE, GROUP_DRACONIAN
    },

    {
	{"fire breath","ateþ nefesi"},	{ 69, 69, 69, 69,69,69,69,69,69,69,69,69,69 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_fire_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_fire_breath,		SLOT(201),	200,	24,
  "ateþ nefesi",	"Gözlerindeki duman yokoluyor.",	"", CABAL_NONE ,
	RACE_TOP, ALIGN_NONE, GROUP_DRACONIAN
    },

    {
	{"frost breath","buz nefesi"},			{ 61,61,61,61,61,61,61,61,61,61,61,61,61  },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_frost_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_frost_breath,		SLOT(202),	125,	24,
  "buz nefesi",	"!Frost Breath!",	"", CABAL_NONE ,
	RACE_TOP, ALIGN_NONE, GROUP_DRACONIAN
    },

    {
	{"gas breath","gaz nefesi"},		{ 70,70,70,70,70,70,70,70,70,70,70,70,70  },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_gas_breath,	TAR_IGNORE,		POS_FIGHTING,
	&gsn_gas_breath,	SLOT(203),	175,	24,
  "gaz nefesi",		"!Gas Breath!",		"", CABAL_NONE ,
	RACE_TOP, ALIGN_NONE, GROUP_DRACONIAN
    },

    {
	{"lightning breath","þimþek nefesi"},	{ 64,64,64,64,64,64,64,64,64,64,64,64,64  },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_lightning_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_lightning_breath,		SLOT(204),	150,	24,
  "þimþek nefesi",	"!Lightning Breath!",	"", CABAL_NONE ,
	RACE_TOP, ALIGN_NONE, GROUP_DRACONIAN
    },

    {
	{"lightning stroke","çelik darbe"},		{ 69, 69, 69, 69,69,69,69,69,69,69,69,69,69 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(632),	200,	24,
  "çelik darbe",	"!lightning stroke!",	"", CABAL_NONE ,
	RACE_TOP, ALIGN_NONE, GROUP_DRACONIAN
    },

    {
   {"luck bonus","þans bonusu"},{ 67, 67, 67, 67,67,67,67,67,67,67,67,67,67 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,		SLOT( 630),	 20,	12,
  "",		"Büyüye karþý zýrhýnýn azaldýðýný hissediyorsun.",	"", CABAL_NONE,
	RACE_TOP, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	{"paralyzation","felç"},	{ 69, 69, 69, 69,69,69,69,69,69,69,69,69,69 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_IGNORE,	POS_FIGHTING,
	NULL,		SLOT(631),	200,	24,
  "felç",	"Yeniden hareket edebiliyorsun.",	"", CABAL_NONE ,
	RACE_TOP, ALIGN_NONE, GROUP_DRACONIAN
    },

    {
	{"repulsion","repulsion"},		{ 65, 65, 65, 65,65,65,65,65,65,65,65,65,65 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(633),	200,	24,
	"repulsion",	"!repulsion!",	"", CABAL_NONE ,
	RACE_TOP, ALIGN_NONE, GROUP_DRACONIAN
    },

    {
	{"sleep gas","uyku gazý"},	{ 67, 67, 67, 67,67,67,67,67,67,67,67,67,67 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_IGNORE,	POS_FIGHTING,
	NULL,		SLOT(628),	200,	24,
  "uyku gazý",	"Kendini kurumuþ hissediyorsun.",	"", CABAL_NONE ,
	RACE_TOP, ALIGN_NONE, GROUP_DRACONIAN
    },

    {
	{"slow gas","slow gas"},{ 69, 69, 69, 69,69,69,69,69,69,69,69,69,69 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(634),	200,	24,
  "slow gas",	"Eski hýzýný kazanýyorsun.",	"", CABAL_NONE ,
	RACE_TOP, ALIGN_NONE, GROUP_DRACONIAN
    },

/*
 * Skills for mobiles. (general purpose and high explosive from
 * Glop/Erkenbrand
 */
    {
	{"crush","baský"},	{93,93,93,93,93,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_crush,            	SLOT( 0),       0,      18,
        "baský",                 "!crush!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_FIGHTMASTER
    },

    {
	{"general purpose","genel amaç"},       { 93, 93, 93, 93, 93, 93, 93, 93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_general_purpose,  TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,                   SLOT(401),      0,      12,
	"genel amaç", "!General Purpose Ammo!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"high explosive","yüksek patlayýcý"},       { 93, 93, 93, 93, 93, 93, 93, 93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_high_explosive,   TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,                   SLOT(402),      0,      12,
        "yüksek patlayýcý",  "!High Explosive Ammo!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"tail","kuyruk"},		{  53, 53,53, 53,53,53,53,53,53 ,53,53,53,53},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_tail,            	SLOT( 0),       0,      18,
        "kuyruk",                 "!Tail!",		"", CABAL_NONE ,
	RACE_TOP, ALIGN_NONE, GROUP_FIGHTMASTER
    },

/* combat and weapons skills */

    {
	{"arrow","ok"},		{ 93, 93,93, 93,93,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_IGNORE,	POS_STANDING,
	&gsn_arrow,	SLOT(0),	0,	0,
  "ok",	"!arrow!",	"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_WEAPONSMASTER
    },

    {
	{"axe","balta"},		{  93, 93, 93,  1,93,1,93,1 ,93 ,93,93,93,93},
	{ 2, 2, 1, 1,1,1,1,1,1, 1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_axe,            	SLOT( 0),       0,      0,
	"",                     "!Axe!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_WEAPONSMASTER
    },

    {
	{"bow","yay"},{ 93, 93,93, 44,93,93,93,35 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_IGNORE,	POS_STANDING,
	&gsn_bow,		SLOT(0),	0,	12,
  "yay",		"!bow!",	"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_WEAPONSMASTER
    },

    {
	{"dagger","hançer"},             { 1,  93,  1,  1,1,1,1,1 ,1 ,1,1,1,1},
	{ 2, 2, 1, 1,1,1,1,1,1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_dagger,            SLOT( 0),       0,      0,
	"",                     "!Dagger!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_WEAPONSMASTER
    },

    {
	{"flail","döven"},		{ 93,  1, 93,  1,93,1,93,93 ,93,93,93,93,1},
	{ 2, 2, 1, 1,1,1,1,1,1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_flail,            	SLOT( 0),       0,      0,
	"",                     "!Flail!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_WEAPONSMASTER
    },

    {
	{"lance","kargý"},	{ 33,33,33,33,33,33,33,33,33,33,33,33,33},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_IGNORE,	POS_STANDING,
	&gsn_lance,	SLOT(0),	0,	0,
	"lance",	"!lance!",	"",
	CABAL_KNIGHT , RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	{"mace","topuz"},			{  93,  1,  1,  1,1,1,93,93 ,93 ,1,93,93,93},
	{ 2, 2, 1, 1,1,1,1,1,1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_mace,            	SLOT( 0),       0,      0,
	"",                     "!Mace!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_WEAPONSMASTER
    },

    {
	{"polearm","teber"},		{ 93, 93, 93,  1,1,1,93,93,93 ,93,93,93,93},
	{ 2, 2, 1, 1,1,1,1,1,1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_polearm,           SLOT( 0),       0,      0,
	"",                     "!Polearm!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_WEAPONSMASTER
    },

    {
	{"shield block","kalkan bloðu"},		{ 93,17,7,1,1,1,12,10,93 ,10,93,93,93},
	{ 2, 1, 1, 1,1,1,1,1,1,1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_shield_block,	SLOT(0),	0,	0,
	"",			"!Shield!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_DEFENSIVE
    },

    {
	{"spear","mýzrak"},			{  93,  93,  1,  1,93,1,1,1 ,93,1,8,93,93 },
	{ 2, 2, 1, 1,1,1,1,1,1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_spear,            	SLOT( 0),       0,      12,
        "mýzrak",                     "!Spear!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_WEAPONSMASTER
    },

    {
	{"sword","kýlýç"},		{  93, 93,  1,  1,1,1,1,1,93 ,1,1,93,93},
	{ 2, 2, 1, 1,1,1,1,1,1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_sword,            	SLOT( 0),       0,      0,
	"",                     "!sword!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_WEAPONSMASTER
    },

    {
	{"whip","kýrbaç"},		{  1, 1,  93,  1,93,1,93,1 ,1,93,1,1,1},
	{ 2, 2, 1, 1,1,1,1,1,1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_whip,            	SLOT( 0),       0,      0,
	"",                     "!Whip!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_WEAPONSMASTER
    },

    {
	{"second weapon","ikinci silah"},	{  93,93,93,23,48,48,93,23,93,24,93,93,93},
	{ 2, 2, 1, 1,1,1,1,1,1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_second_weapon,            	SLOT( 0),       0,      0,
	"",	"!second weapon!",	"",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_WEAPONSMASTER
    },

    {
	{"ambush","pusu"},         {  93, 93, 93, 93, 93, 93, 93, 17,93 ,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,        TAR_CHAR_OFFENSIVE,          POS_STANDING,
        &gsn_ambush,                    SLOT(0),        0,       12,
        "pusu", "!Ambush!", "",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"area attack","alan saldýrýsý"},      {93, 93, 93, 73,93,93,93,93,93,69,93,93,93},
	{1,1,1,1,1,1,1,1,1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_area_attack,     SLOT(0),        0,      0,
	"",         "!Area Attack!", 	"",	CABAL_NONE,
	RACE_NONE, ALIGN_NONE, GROUP_FIGHTMASTER
    },

    {
	{"assassinate","suikast"},       {  93, 93, 93, 93, 93, 93, 65, 93 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_IGNORE,          POS_STANDING,
        &gsn_assassinate,       SLOT(0),        0,       18,
        "suikast", "!assassinate!", "",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"backstab","ardýlan"},          {  93, 93,  11, 93,93,93,93,93 ,93,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_backstab,          SLOT( 0),        0,     12,
        "ardýlan",             "!Backstab!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"bash","omuz"},		{  93,93,93,1, 6,1,1,10 ,93 ,93,11,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_bash,            	SLOT( 0),       0,      18,
        "omuz",                 "!Bash!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_FIGHTMASTER
    },

    {
	{"bash door","kapýkýrma"},	{  93, 93, 93,  1,6,1,1,93 ,93,93,11,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_bash_door,        	SLOT( 0),       0,      18,
        "kapýkýrma",                 "!Bash Door!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_FIGHTMASTER
    },

    {
	{"bearcall","ayý çaðýrma"},      {  93, 93, 93, 93, 93, 93, 93, 31,93,93,93,93,93 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,		TAR_IGNORE,          POS_FIGHTING,
        &gsn_bear_call,		SLOT(518),	50,       0,
        "", "Yeni ayýlarla baþa çýkabilirsin.", "",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"berserk","vahþileþme"},		{ 93, 93, 93, 18,93,93,93,93 ,93,20,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_berserk,        	SLOT( 0),       0,      24,
        "",                     "Nabzýnýn yavaþladýðýný hissediyorsun.",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"blackguard","karagard"},{  93, 93, 93, 93, 93, 93, 93, 93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_CHAR_OFFENSIVE,   POS_STANDING,
	&gsn_blackguard,		SLOT(0),    0,       12,
  "karagard",		"Karagardýn yokoluyor.","", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"blackjack","copla"},	{  93, 93, 30, 93, 93, 93, 93, 93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_CHAR_OFFENSIVE,   POS_STANDING,
	&gsn_blackjack,		SLOT(0),    0,       8,
  "copla",		"Kafandaki aðrý geçiyor.","", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"blind fighting","kör dövüþü"},	{   93, 93, 93, 13,32,93,11,43,93 ,23,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_blind_fighting,		SLOT(0),	0,	0,
	"",			"!blind fighting!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_FIGHTMASTER
    },

    {
	{"blindness dust","körlük tozu"},       {  93, 93, 93, 93, 93, 93, 28, 93,93,93,93,93,93 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,   		TAR_IGNORE,             POS_FIGHTING,
        &gsn_blindness_dust,    SLOT(0),        20,       18,
	"",                     "!blindness dust!", "",	 CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"blink","zayi"},            {  1, 93,  93, 93,93,93,93,93 ,1,93,93,1,1},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_blink,             SLOT( 0),        0,     0,
	"",                     "!Blink!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"butcher","kasap"},       {  93, 93, 93, 93, 93, 93, 93, 13 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,        TAR_IGNORE,          POS_STANDING,
        &gsn_butcher,             SLOT(0),        0,       0,
	"", "!butcher!", "",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"caltraps","çivi"},        {  93, 93, 93, 93, 93, 93, 18, 93 ,93,36,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_IGNORE,          POS_FIGHTING,
        &gsn_caltraps,       SLOT(0),        0,       18,
        "çivi", "Bacaklarýndaki acý sönüyor.", "",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"camouflage","kamuflaj"},       {  93, 93, 93, 93, 93, 93, 93, 4 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,        TAR_IGNORE,           POS_STANDING,
        &gsn_camouflage,         SLOT(0),        0,       24,
	"",         "!Camouflage!", "",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"camouflage move","kamufle hareket"},	{ 93, 93, 93, 93, 93,93,93,23, 93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_move_camf,		SLOT(0),	0,	12,
	"",		"!move camouflaged!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"camp","kamp"},			{  93, 93, 93,  93,93,93,93,48 ,93 ,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_camp,            	SLOT( 0),       0,      24,
        "kamp",          "Yeni kamplar kurabilirsin.","", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"circle","çevrel"},          {  93, 93, 7, 93, 93, 93, 93, 93,93 ,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,        TAR_CHAR_OFFENSIVE,          POS_FIGHTING,
        &gsn_circle,             SLOT(0),        0,       18,
        "çevrel", "!Circle!", "",		CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"control animal","hayvan kontrolü"},	{ 93, 93, 93, 93,93,93,93,93,93,93,43,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_control_animal,	SLOT( 0),	 5,	12,
  "",			"Kendine güvenin artýyor.",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_BEGUILING
    },

    {
	{"cleave","yarma"},         {  93, 93, 93, 93, 93, 55, 93, 93 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,     TAR_CHAR_OFFENSIVE,     POS_STANDING,
        &gsn_cleave,         SLOT(0),        0,       24,
        "yarma",         "!Cleave!", "",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"concentrate","konsantrasyon"},        { 93, 93,93,59,93,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_concentrate,        SLOT( 0),        0,     18,
        "",      "Yeni dövüþlere konsantre olabilirsin.",	"",CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_FIGHTMASTER
    },

    {
	{"counter","kontra"},     {  93, 93, 93, 28, 35, 93, 93, 93 ,93,28,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,        TAR_IGNORE,           POS_STANDING,
        &gsn_counter,         SLOT(0),        0,       0,
	"",         "!Counter!", "",		CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_FIGHTMASTER
    },

    {
	{"critical strike","kritik vuruþ"},	{   93, 93, 93, 93,93,93,58,93,93 ,62,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_critical,			SLOT(0),	0,	18,
	"",			"!critical strike!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"cross block","çapraz blok"},       {  93,93,93,31,56,56,93,31,93,32,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_cross_block,       SLOT( 0),        0,     0,
	"",                     "!cross block!",	"", CABAL_NONE ,
	RACE_NONE,ALIGN_NONE, GROUP_DEFENSIVE
    },

    {
	{"detect hidden","saklýyý saptama"},	{ 93, 93, 93, 93, 93, 93, 93, 93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_detect_hidden,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,		SLOT(44),	 5,	12,
  "",			"Saklýyý saptaman sönüyor.",
	"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_DETECTION
    },

    {
	{"detect sneak","süzüleni saptama"},		{   93, 93, 93, 93,93,93,93,93,93 ,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_CHAR_SELF,		POS_STANDING,
	&gsn_detect_sneak,		SLOT(0),	20,	18,
	"",			"!detect sneak!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_DETECTION
    },

    {
	{"dirt kicking","toz atma"},	{ 93, 93,  3,  8,93,3,3,5,93 ,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_dirt,		SLOT( 0),	0,	12,
  "toz atma",		"Gözlerindeki toz dökülüyor.",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"disarm","silahsýzlandýr"},              {  93, 93, 28, 20,20,21,23,21,93,21,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_disarm,            SLOT( 0),        0,     18,
	"",                     "!Disarm!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_WEAPONSMASTER
    },

    {
	{"dodge","sýyrýlma"},           {  33,93,1,13, 21,21,1,9, 93,15,8,93,27},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_dodge,             SLOT( 0),        0,     0,
	"",                     "!Dodge!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_DEFENSIVE
    },

   {
	{"dual backstab","çift ardýlan"},       {  93, 93, 38, 93, 93, 93, 93, 93 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,             TAR_CHAR_OFFENSIVE,           POS_STANDING,
        &gsn_dual_backstab,       SLOT(0),        0,       0,
        "çift ardýlan",      "!dual backstab!", "",		CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"enchant sword","kýlýç yücelt"},		{ 93, 93, 93, 93,93,93,93,93,93,42,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_OBJ_INV,		POS_STANDING,
	&gsn_enchant_sword,	SLOT(0),	100,	24,
	"",	"!Enchant sword!",	"", CABAL_NONE , RACE_NONE,
	ALIGN_NONE, GROUP_NONE
    },

    {
	{"endure","dayaným"},          {  93, 93, 93, 93, 93, 93, 20, 93 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,        TAR_CHAR_SELF,          POS_FIGHTING,
        &gsn_endure,             SLOT(0),        0,       24,
        "", "Büyüye karþý dayanýmýn azalýyor.", "",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"enhanced damage","yüksek zarar"},      {  93, 33, 25,  3,6,3,16,15 ,93,8,14,93,93},
	{ 1,  2,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_enhanced_damage,   SLOT( 0),        0,     0,
	"",                     "!Enhanced Damage!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_FIGHTMASTER
    },

    {
	{"entangle","sarmaþýk"},        {  93,93,93,93,93,93,93,26 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_entangle,      TAR_CHAR_OFFENSIVE,   POS_FIGHTING,
        &gsn_entangle,       SLOT(568),        40,       12,
        "sarmaþýk", "Sarmaþýklardan kurtulduðunu hissediyorsun.", "",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"envenom","sem"},		{ 93, 93, 15, 93, 93, 93, 93, 93,93,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,	  	POS_RESTING,
	&gsn_envenom,		SLOT(0),	0,	12,
	"",			"!Envenom!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"escape","sývýþ"},		{93,93,45,93,93,93,55,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_escape,		SLOT( 0),	0,	24,
	"",	"!escape!","", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"explode","patlat"},      {  93, 93, 93, 93, 93, 93, 93, 93 ,93,46,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_CHAR_OFFENSIVE,          POS_FIGHTING,
        &gsn_explode,       SLOT(0),        100,       24,
        "patlat", "Gözlerindeki duman yokoluyor.", "",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
{"ground strike","zemin vuruþu"},	{ 93,93,93,93, 93,93,93,52, 93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_ground_strike,	SLOT(0),	0,	18,
	"",			"!ground strike!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"hand block","el bloðu"},            { 93,93,93,93,93,93,22,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_hand_block,             SLOT( 0),        0,     0,
	"",                     "!hand block!",		"", CABAL_NONE ,
	RACE_NONE,ALIGN_NONE, GROUP_DEFENSIVE
    },

    {
	{"hand to hand","çýplak el"},		{ 45,  30, 18, 9,11,18,3,24,45,12,19,45,35 },
	{ 2,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_hand_to_hand,	SLOT( 0),	0,	0,
	"",			"!Hand to Hand!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_FIGHTMASTER
    },

    {
	{"harakiri","harakiri"},	{ 93, 93, 93, 93,93,93,93,93,93,25,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null, 	TAR_IGNORE,	POS_FIGHTING,
	&gsn_hara_kiri,		SLOT(0),	50,	12,
  "",	"Yaþamaya yeniden baþladýðýný hissediyorsun.", "", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"headguard","kafakoruma"},	{  93, 93, 93, 93, 93, 93, 93, 93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_CHAR_OFFENSIVE,   POS_STANDING,
	&gsn_headguard,		SLOT(0),    0,       12,
  "kafakoruma",		"Kafakoruman yokoluyor.","", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"herbs","bitki"},       {  93, 93, 93, 93, 93, 93, 93, 18 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,        TAR_CHAR_DEFENSIVE,          POS_STANDING,
        &gsn_herbs,             SLOT(0),        0,       30,
        "", "Yine bitkilerle uðraþabilirsin.", "",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"kick","tekme"},           {  93, 12, 14,  5,93,9,1,8 ,93,2,4,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,             TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        &gsn_kick,              SLOT( 0),        0,     12,
        "tekme",                 "!Tekme!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_FIGHTMASTER
    },

    {
	{"lash","kýrbaçla"},			{ 48,48,93,93, 48,93,48,93, 48,93,48,48,48},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_lash,            	SLOT( 0),       0,      4,
        "kýrbaç",                 "!Lash!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_FIGHTMASTER
    },

    {
	{"light resistance","ýþýk direnci"},	{  93,  93, 93, 93,93,93,93,93,93,93,50,93,93},
	{ 1,  1,  2,  2, 1, 1, 1, 1,1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_SLEEPING,
	&gsn_light_res,		SLOT( 0),	0,	0,
	"",			"Light Resistance","", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"lion call","aslan çaðýrma"},	{ 93, 93, 93, 93,93,93,93,55,93,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null, 	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	&gsn_lion_call,		SLOT(0),	50,	12,
	"",		"!lion call!", 		"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"make arrow","ok yapma"},		{ 93, 93, 93,93,93,93,93,37,93,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_make_arrow,	SLOT(0),	 50,	24,
	"",		"!make arrow!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"make bow","yay yapma"},		{ 93, 93, 93,93,93,93,93,36,93,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_make_bow,		SLOT(0),	 200,	24,
	"",		"!make bow!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"mental attack","zihin saldýrýsý"},{ 93, 93,93, 93,93,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_CHAR_SELF,		POS_STANDING,
	&gsn_mental_attack,		SLOT(0),	200,	24,
	"",	"!mental attack!",	"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"neckguard","boyunkoruma"},	{  93, 93, 93, 93, 93, 93, 93, 93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_CHAR_OFFENSIVE,   POS_STANDING,
	&gsn_neckguard,		SLOT(0),    0,       12,
  "boyun koruma",		"Boyun koruman yokoluyor.","", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"nerve","sinir"},       {  93, 93, 93, 93, 93, 93, 29, 93 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,        TAR_CHAR_OFFENSIVE,          POS_FIGHTING,
        &gsn_nerve,             SLOT(0),        0,       18,
        "", "Sinirlerin iyileþiyor.", "",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"parry","karþýlama"},              { 93,25,19,1, 3,5,93,3, 23,1,22,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_parry,             SLOT( 0),        0,     0,
	"",                     "!Parry!",		"", CABAL_NONE ,
	RACE_NONE,ALIGN_NONE, GROUP_DEFENSIVE
    },

    {
	{"perception","algý"},  {  93, 93, 23, 93, 93,93,93,93,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,        TAR_IGNORE,          POS_STANDING,
        &gsn_perception,             SLOT(0),        0,       0,
	"", "!perception!", "",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"push","itme"},			{  93, 93, 53,  93,93,93,93,93 ,93 ,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_push,            	SLOT( 0),       0,      18,
        "itme",          "!push!","", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"rescue","kurtar"},               {  93, 93, 93,  1,1,93,93,93,93,23,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_rescue,            SLOT( 0),        0,     12,
	"",                     "!Rescue!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_DEFENSIVE
    },

    {
	{"sense life","yaþam sezme"},{ 93, 93,93, 93,93,93,93,93 ,93,93,13,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_CHAR_SELF,		POS_STANDING,
	&gsn_sense_life,	SLOT(623),	20,	12,
  "",	"Yaþam sezme gücünü yitiriyorsun.",	"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"settraps","tuzak"},	{ 93, 93, 59, 93,93,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_CHAR_SELF,		POS_STANDING,
	&gsn_settraps,		SLOT(0),	200,	24,
  "tuzak",	"Yeni tuzaklar kurabilirsin.",	"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"shield cleave","kalkan yarma"},{ 93, 93, 33, 26, 29, 29, 31, 35, 93, 26, 39, 93, 93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_CHAR_SELF,		POS_STANDING,
	&gsn_shield_cleave,		SLOT(0),	200,	24,
	"",	"!shield cleave!",	"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"smithing","demirci"},		{   93, 93, 93, 50,93,93,93,93,93 ,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_smithing,			SLOT(0),	10,	18,
	"",			"!smithing!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"spell craft","büyü tanýma"},			{ 10,30,93,93, 93,93,93,93, 10,93,93,10,10 },
	{ 1,  1,  1,  1, 1, 1, 2, 2, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_spell_craft,	SLOT(0),    0,       0,
  "büyü tanýma",		"!spell craft!","", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"strangle","bayýltma"},          {  93,  93,  93,  93,93,93,26,93 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_CHAR_OFFENSIVE,          POS_STANDING,
        &gsn_strangle,       SLOT(0),        0,       8,
        "bayýltma", "Boynunu yeniden hareket ettirebiliyorsun.", "",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"swimming","yüzme"},	{   93, 93, 93, 93,93,93,93,93,93 ,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_swimming,		SLOT(0),	0,	0,
	"",			"!swimming!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"target","hedef"},                 {  93, 93, 93,  93,93,93,93,93 ,93,40,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,             TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        &gsn_target,              SLOT( 0),        0,     12,
        "hedef",                 "!Kick!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"throw","fýrlatma"},        {  93, 93, 93, 93, 93, 93, 25, 93 ,93,37,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_CHAR_OFFENSIVE,       POS_FIGHTING,
        &gsn_throw,       SLOT(0),        0,       18,
        "fýrlatma", "!throw!", "",	CABAL_NONE , RACE_NONE,ALIGN_NONE, GROUP_NONE
    },

    {
	{"tiger power","kaplan gücü"},		{ 93,93,93,93, 93,93,93,61, 93,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null, 	TAR_IGNORE,	POS_FIGHTING,
	&gsn_tiger_power,	SLOT(0),	0,	12,
  "",	"Kaplanlarýnýn kaçtýðýný hissediyorsun.", "", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"track","iz"},       {  93, 93, 93,93,93,93,93,39,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_IGNORE,       POS_STANDING,
        &gsn_track,       SLOT(0),        0,       12,
	"", "!track!", "",CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"trip","çelme"},		{  93, 93,  3, 25,93,10,8,93 ,93,20,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_trip,		SLOT( 0),	0,	12,
  "çelme",			"!Trip!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_FIGHTMASTER
    },

    {
	{"vampire","vampir"},			{ 93, 93, 93, 93,93,93,93,93,93,93,10,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_IGNORE,		POS_STANDING,
	&gsn_vampire,		SLOT(0),	100,	12,
  "",	"Doðallaþýyorsun.",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"vampiric bite","vampir ýsýrýðý"},          {  93, 93,  93, 93,93,93,93,93 ,93,93,10,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_vampiric_bite,          SLOT( 0),        0,     12,
        "vampir ýsýrýðý",             "!vampiric bite!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"vampiric touch","vampir temasý"},      {  93,  93,  93,  93,93,93,93,93 ,93,93,15,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,      TAR_CHAR_OFFENSIVE,          POS_STANDING,
	&gsn_vampiric_touch,       SLOT(0),        0,       12,
  "vampir temasý", "Kabuslardan uyanýyorsun.", "",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"vanish","yokol"},        {  93, 93, 93, 93, 93, 93, 18, 93 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_CHAR_SELF,          POS_FIGHTING,
        &gsn_vanish,       SLOT(521),        25,       18,
	"", "!vanish!", "", 	CABAL_NONE , RACE_NONE,ALIGN_NONE, GROUP_NONE
    },

    {
	{"warcry","nara"},       {  93, 93, 93,31,93,93,93,35,93 ,30,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_IGNORE,       POS_FIGHTING,
        &gsn_warcry,       SLOT(0),        30,       12,
        "", "Naranýn etkisi yokoluyor.", "",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"weapon cleave","silah yarma"}, { 93, 93, 37, 29, 34, 34, 35, 38, 93, 29, 44, 93, 93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_CHAR_SELF,		POS_STANDING,
	&gsn_weapon_cleave,	SLOT(0),	200,	24,
	"",	"!weapon cleave!",	"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
       { "second attack", "ikili hamle"},        {  34, 28, 18,  10,13,14,15,13,25,10,12,34,34},
	{ 2,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_second_attack,     SLOT( 0),        0,     0,
	"",                     "!Second Attack!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_FIGHTMASTER
    },

    {
	{"secondary attack","ikincil hamle"},	{93,93,93,52, 61,61,93,58 ,93,55,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_CHAR_SELF,		POS_STANDING,
	&gsn_secondary_attack,	SLOT(0),	200,	24,
	"",	"!secondary attack!",	"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"third attack","üçlü hamle"},        { 93, 93,36,19,27,26,26,45,93,20,32,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_third_attack,      SLOT( 0),        0,     0,
	"",                     "!Third Attack!",	"", CABAL_NONE ,
	RACE_NONE,ALIGN_NONE, GROUP_FIGHTMASTER
    },

    {
	{"fourth attack","dörtlü hamle"},       {93,93,93,30, 93,93,48,93, 93,93,93,93,93},
	{1,1,1,1,1,1,1,1,1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_fourth_attack,     SLOT(0),        0,      0,
	"",                     "!Fourth Attack!", 	"",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_FIGHTMASTER
    },

    {
	{"fifth attack","beþli hamle"},         {93, 93, 93, 52, 93, 93, 93,93,93,93,93,93,93},
	{1,1,1,1,1,1,1,1,1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_fifth_attack,     SLOT(0),        0,      0,
	"",          "!Fifth Attack!", 	"",	CABAL_NONE,
	RACE_NONE, ALIGN_NONE, GROUP_FIGHTMASTER
    },

/* non-combat skills */

    {
	{"blue arrow","mavi ok"},			{ 93, 93, 93,93,93,93,93,51,93,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_blue_arrow,	SLOT(0),	 50,	12,
	"",		"!blue arrow!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"fast healing","hýzlý iyileþme"},	{  15, 9, 16,  6,7,7,9,6 ,15 ,3,9,9,9},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_SLEEPING,
	&gsn_fast_healing,	SLOT( 0),	0,	0,
	"",			"!Fast Healing!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_MEDITATION
    },

    {
	{"green arrow","yeþil ok"},			{ 93, 93, 93,93,93,93,93,40,93,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_green_arrow,	SLOT(0),	 50,	12,
	"",		"!green arrow!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"grip","kavrama"},            { 93, 93, 93,43,93,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_grip,            SLOT( 0),        0,     18,
	"",                     "!Grip!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_WEAPONSMASTER
    },

    {
	{"haggle","pazarlýk"},		{  7, 18,  1, 14,12,8,17,15 ,7 ,19,5,5,5},
	{ 2,  2,  1,  2, 2, 2, 1, 2, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_RESTING,
	&gsn_haggle,		SLOT( 0),	0,	0,
	"",			"!Haggle!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"hide","saklan"},		{  93, 93,  1,  93,93,93,10,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_RESTING,
	&gsn_hide,		SLOT( 0),	 0,	18,
	"",			"!Hide!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"katana","katana"},	{ 93, 93, 93, 93,93,93,93,93,93,72,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_OBJ_INV,		POS_STANDING,
	&gsn_katana,		SLOT(0),	100,	24,
  "",		"Yeni bir katana yapabilirsin.",	"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"lay hands","þifalýeller"},		{93,93,93,93,63,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_lay_hands,		SLOT( 0),	0,	12,
  "",	"Yeniden masum iyileþtirebilirsin.",	"",CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"lore","irfan"},		{  93,93,13,21,19,20,19,18,93,22,20,93,93},
	{ 2,  2,  2,  2, 2, 2, 2, 2, 2, 2,2,2,2},
	spell_null,		TAR_IGNORE,		POS_RESTING,
	&gsn_lore,		SLOT( 0),	0,	36,
	"",			"!Lore!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"mastering pound","vuruþ ustalýðý"},	{ 93,93,93,93,93,93,61,93,93,93,93,93,93},
	{ 2,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_master_hand,	SLOT( 0),	0,	0,
	"",			"!Master Hand!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_FIGHTMASTER
    },

    {
	{"mastering sword","kýlýç ustalýðý"},	{ 93,93,93,93,93,93,93,93,93,61,93,93,93},
	{ 2, 2, 1, 1,1,1,1,1,1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_master_sword,           SLOT( 0),       0,      0,
	"",                     "!master sword!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_WEAPONSMASTER
    },

    {
	{"meditation","meditasyon"},		{  6,  6, 15, 15,14,15,14,16,6,16,6,6,6},
	{ 1,  1,  2,  2, 1, 1, 1, 1,1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_SLEEPING,
	&gsn_meditation,	SLOT( 0),	0,	0,
	"",			"Meditation",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_MEDITATION
    },

    {
	{"peek","dikiz"},			{  93, 93,  1, 93,93,93,93,93,93 ,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_peek,		SLOT( 0),	 0,	 0,
	"",			"!Peek!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"pick lock","maymuncuk"},		{ 93, 93,  17, 93,93,93,34,93,93,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_pick_lock,		SLOT( 0),	 0,	12,
	"",			"!Pick!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"poison smoke","zehir bulutu"},      {  93, 93, 93, 93, 93, 93, 30, 93,93,93,93,93,93 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,     	TAR_IGNORE,            POS_FIGHTING,
        &gsn_poison_smoke,      SLOT(0),        20, 	18,
	"",                     "!poison smoke!", "",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"quiet movement","sessiz yürüyüþ"},      {  93, 93, 93, 93, 93, 93, 93, 15 ,93,93,10,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,        TAR_IGNORE,          POS_STANDING,
        &gsn_quiet_movement,             SLOT(0),        0,       0,
	"", "!quiet movement!", "",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"recall","anýmsa"},{  1,  1,  1,  1,1,1,1,1 ,1,1,1,1,1},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_recall,		SLOT( 0),	0,	24,
	"",			"!Recall!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"red arrow","kýzýl ok"},		{ 93, 93, 93,93,93,93,93,44,93,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_red_arrow,	SLOT(0),	 50,	12,
	"",		"!red arrow!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"sneak","süzül"},	{  93, 93, 4, 93, 93, 93, 18, 93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_sneak,		SLOT( 0),	 0,	12,
  "",			"Artýk süzülmüyorsun.",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"steal","arakla"},			{  93, 93, 12, 93, 93, 93, 93, 93,93 ,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_steal,		SLOT( 0),	 0,	12,
	"",			"!Steal!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"scrolls","parþömen"},		{  1,  1,  1,  93,93,1,93,1 , 1,6,1,1,1},
	{ 1,  1,  2,  2, 2, 2, 2, 2, 2, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_scrolls,		SLOT( 0),	0,	12,
	"",			"!Scrolls!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_WIZARD
    },

    {
	{"staves","týlsýmlar"},		{  1,  1,  93,  93,93,93,1,1 ,1,1,1,1,1},
	{ 1,  1,  2,  2, 2, 2, 2, 2, 2, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_staves,		SLOT( 0),	0,	12,
	"",			"!Staves!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_WIZARD
    },

    {
	{"tame","uysallaþtýr"},        {  93, 93, 93, 93, 93, 93, 93, 39 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,        TAR_CHAR_DEFENSIVE,          POS_FIGHTING,
        &gsn_tame,             SLOT(0),        0,       24,
	"", "!tame!", "",CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"trance","trans"},       {  14,15,93,93,93,93,93,93,14 ,93,21,21,21},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_IGNORE,   POS_SLEEPING,
        &gsn_trance,       SLOT(0),        0,       0,
	"", "", "",	CABAL_NONE , RACE_NONE,ALIGN_NONE, GROUP_MEDITATION
    },

    {
	{"white arrow","beyaz ok"},			{ 93, 93, 93,93,93,93,93,49,93,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_white_arrow,	SLOT(0),	 50,	12,
	"",		"!white arrow!",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"wands","deðnekler"},	{  1,  1,  93,  93,93,1,93,93 ,1,5,1,1,1},
	{ 1,  1,  2,  2, 2, 2, 2, 2, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_wands,		SLOT( 0),	0,	12,
	"",			"!Wands!",		"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_WIZARD
    },

/* cabals */

    {
	{"mortal strike","ölümcül atak"},	{ 63,63,63,63,63,63,63,63,63,63,63,63,63},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_CHAR_SELF,		POS_STANDING,
	&gsn_mortal_strike,		SLOT(0),	200,	24,
  "ölümcül atak",	"!mortal strike!",	"",
	CABAL_BATTLE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"disgrace","gözden düþür"},{  33, 33, 33, 33, 33,33,33,33,33,33,33,33,33},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_disgrace,	TAR_CHAR_OFFENSIVE,		POS_STANDING,
	NULL,		SLOT(619),	200,	12,
  "",	"Onurunu kazandýðýný hissediyorsun.",	"",
	CABAL_CHAOS , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"evil spirit","kem ruh"},	{  33, 33, 33, 33, 33,33,33,33,33,33,33,33,33},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_evil_spirit,	TAR_IGNORE,		POS_STANDING,
	&gsn_evil_spirit,		SLOT(618),	800,	36,
  "kem ruh",	"Vücudunla ruhun yeniden bir oluyor.",	"",
	CABAL_INVADER , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"ruler aura","tüze aurasý"},     {  33, 33, 33, 33, 33,33,33,33,33,33,33,33,33},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_ruler_aura,    TAR_CHAR_SELF,          POS_STANDING,
        NULL,             SLOT(667),        20,       12,
        "", "Tüze auran yokoluyor.", 	"",	CABAL_RULER ,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	{"sword of justice","adalet kýlýcý"},	{48,48,48,48, 48,48,48,48,48,48,48,48,48},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_sword_of_justice,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(686),	50,	12,
  "adalet kýlýcý",	"!sword of justice!", "", CABAL_RULER ,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	{"bandage","sargý"},       {23, 23, 23, 23,23,23,23,23,23,23,23,23,23},
	{1,1,1,1,1,1,1,1,1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_bandage,     SLOT(0),        0,      0,
	"",        "Daha az saðlýklý hissediyorsun.", 	"",	CABAL_BATTLE,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	{"cabal recall","kabal anýmsa"},	{ 10,10,10,10,10,10,10,10,10,10,10,10,10},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_cabal_recall,		SLOT( 0),	0,	24,
  "",	"Anýmsama için yeniden dua edebilirsin.","", CABAL_BATTLE ,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        {"wanted","aranýyor"},                {  1,  1,  1,  1, 1, 1, 1, 1 ,1,1,1,1,1},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_DEAD,
        &gsn_wanted,                   SLOT(0),        0,       0,
        "",                     "!Wanted!", "",		CABAL_RULER,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        {"judge","yargýlama"},               {  1,  1,  1,  1, 1, 1, 1, 1 ,1,1,1,1,1},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_DEAD,
        &gsn_judge,              SLOT(0),        0,       0,
        "",                     "!Judge!", "",		CABAL_RULER,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        {"bloodthirst","kana susama"},          {  15, 15, 15, 15, 15, 15, 15, 15 ,15,15,15,15,15},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_CHAR_SELF,       POS_FIGHTING,
        &gsn_bloodthirst,       SLOT(0),        0,       12,
        "", "Artýk kana susamýyorsun.", "",	CABAL_BATTLE,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        {"spellbane","büyüyýkýmý"},       { 10,10,10,10,10,10,10,10,10,10,10,10,10 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_CHAR_SELF,       POS_STANDING,
        &gsn_spellbane,       SLOT(0),        0,       12,
        "büyüyýkýmý", "Büyüye karþý direncin azalýyor.", "",	CABAL_BATTLE,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        {"resistance","direnç"},      {  12, 12, 12,12,12,12,12,12 ,12,12,12,12,12},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_CHAR_SELF,       POS_FIGHTING,
        &gsn_resistance,       SLOT(0),        0,       24,
        "", "Sertliðin yokoluyor.", "",	CABAL_BATTLE,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
       { "deathblow","ölüm rüzgarý"},          {  20,20,20,20,20,20,20,20 ,20,20,20,20,20},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_IGNORE,       POS_STANDING,
        &gsn_deathblow,       SLOT(0),        0,       0,
        "", "!deathblow!", "",	CABAL_BATTLE,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        {"transform", "dönüþüm"},          {  10, 10, 10,10,10,10,10,10 ,10,10,10,10,10},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_transform,      TAR_CHAR_SELF,       POS_STANDING,
        NULL,       SLOT(522),        100,       24,
        "", "Saðlýðýn azalýyor.", "",	CABAL_SHALAFI,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	{"mental knife","zihin býçaðý"},        {  15, 15, 15,93,15,15,15,15,15,15,15,15,15 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_mental_knife,      TAR_CHAR_OFFENSIVE,       POS_FIGHTING,
        &gsn_mental_knife,       SLOT(524),        35,       12,
        "zihin býçaðý", "Zihnindeki acý yokoluyor.", "",CABAL_SHALAFI,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	{"demon summon","iblis çaðrýsý"},         {  25, 25, 25,93,25,25,25,25 ,25,25,25,25,25},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_demon_summon,      TAR_CHAR_SELF,       POS_FIGHTING,
        &gsn_demon_summon,       SLOT(525),        100,       12,
        "", "Çaðrý gücünün geri geldiðini hissediyorsun.", "", CABAL_SHALAFI,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	{"scourge","ceza kýrbacý"},     {  30, 30, 30,93,30,30,30,30,30,30,30,30,30 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_scourge,      TAR_IGNORE,       POS_FIGHTING,
        NULL,       SLOT(526),        50,       18,
        "ceza kýrbacý", "!scourge!", "",CABAL_SHALAFI,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        {"manacles","kelepçe"},        {  20, 20, 20,20,20,20,20,20 ,20,20,20,20,20},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_manacles,      TAR_CHAR_OFFENSIVE,       POS_FIGHTING,
        NULL,       SLOT(528),        75,       12,
        "", "Zincirlerin yokoluyor.", "", 	CABAL_RULER,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
      {"shield of ruler","tüze kalkaný"},        {  10, 10, 10,10,10,10,10,10 ,10,10,10,10,10},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_shield_ruler,      TAR_IGNORE,       POS_FIGHTING,
        NULL,       SLOT(529),        100,       12,
        "", "!shield!", "",	CABAL_RULER,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
      {"guard","savun"},    {  22,22,22,22,22,22,22,22 ,22,22,22,22,22},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_IGNORE,       POS_STANDING,
        &gsn_guard,       SLOT(0),        0,       12,
        "", "", "",	CABAL_KNIGHT,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
      {"guard call","muhafýz çaðrýsý"},          {  15, 15, 15,15,15,15,15,15 ,15,15,15,15,15},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_guard_call,      TAR_IGNORE,       POS_FIGHTING,
	&gsn_guard_call,       SLOT(530),        75,       12,
  "", 	"Yeniden muhafýz çaðýrabilirsin.", 	"",	CABAL_RULER,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
          {"nightwalker","solucan"},        {  25, 25, 25,25,25,25,25,25 ,25,25,25,25,25},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_nightwalker,      TAR_IGNORE,       POS_FIGHTING,
	&gsn_nightwalker,       SLOT(531),        75,       12,
  "", "Çaðrý gücünün geri geldiðini hissediyorsun.", "",CABAL_INVADER,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
          {"eyes of intrigue","entrika"},          {  13, 13, 13,13,13,13,13,13,13,13,13,13,13 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_eyes,      TAR_IGNORE,       POS_FIGHTING,
	NULL,       SLOT(532),        75,       12,
        "", "!eyes of intrigue!", "",	CABAL_INVADER,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        {"fade", "soluþ"},       {  20, 20, 20,20,20,20,20,20 ,20,20,20,20,20},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_IGNORE,       POS_STANDING,
        &gsn_fade,       SLOT(0),        0,       24,
        "", "!fade!", "",	CABAL_INVADER,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
       { "shadow cloak","gölge pelerini"},        {  10, 10, 10,10,10,10,10,10,10,10,10,10,10 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_shadow_cloak,      TAR_CHAR_DEFENSIVE,       POS_STANDING,
	NULL,       SLOT(533),        10,       12,
  "", "Gölgeler seni korumuyor.", "",	CABAL_INVADER,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
      {"nightfall", "alacakaranlýk"},     {  16, 16, 16,16,16,16,16,16,16,16,16,16,16 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_nightfall,      TAR_IGNORE,       POS_STANDING,
	NULL,       	SLOT(534),        50,       12,
  "", "Iþýklarý kontrol edemiyorsun.", "",	CABAL_INVADER,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        {"aura of chaos","kaos aurasý"},    {  16, 16, 16,16,16,16,16,16,16,16,16,16,16 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_aura_of_chaos,      TAR_CHAR_DEFENSIVE,       POS_STANDING,
	NULL,       	SLOT(720),        20,       12,
  "", "Karmaþa tanrýsý artýk seni korumuyor.", 	"",
	CABAL_CHAOS, RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
       { "garble","afoni"},        {  10, 10, 10,10,10,10,10,10 ,10,10,10,10,10},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_garble,      TAR_CHAR_DEFENSIVE,       POS_FIGHTING,
	&gsn_garble,       SLOT(535),        30,       12,
  "", "Dilin normale dönüyor.", "",	CABAL_CHAOS,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
         { "mirror","ayna"},         { 15, 15, 15,15,15,15,15,15,15,15,15,15,15 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_mirror,      TAR_CHAR_DEFENSIVE,       POS_STANDING,
	&gsn_mirror,       SLOT(536),        40,       12,
  "", "Yokolma hissini yaþýyorsun.", "",	CABAL_CHAOS,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	{"confuse","þaþýrtma"},        {  20, 20, 20,20,20,20,20,20 ,20,20,20,20,20},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_confuse,      TAR_CHAR_OFFENSIVE,       POS_FIGHTING,
	&gsn_confuse,       SLOT(537),        20,       12,
  "", "Þaþkýnlýðýn azalýyor.", "",	CABAL_CHAOS,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	{"doppelganger","dönüþüm"},        {  25, 25, 25,25,25,25,25,25 ,25,25,25,25,25},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_doppelganger,      TAR_CHAR_DEFENSIVE,       POS_STANDING,
	&gsn_doppelganger,       SLOT(527),        75,       12,
  "", "Doðal formuna döndün.", "",	CABAL_CHAOS,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
       {   "chaos blade","yataðan"},      {  23, 23, 23,23,23,23,23,23,23,23,23,23,23 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_chaos_blade,      TAR_IGNORE,       POS_STANDING,
        &gsn_chaos_blade,       SLOT(550),        60,       12,
        "", "!chaos blade!", "",	CABAL_CHAOS,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

   {
          {    "stalker","avcý"},    {  93, 93, 93,93,93,93,93,93 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_stalker,      TAR_IGNORE,       POS_STANDING,
        &gsn_stalker,       SLOT(554),        100,       12,
        "", "Yeni bir avcý çaðýrabilirsin.", "",
	CABAL_RULER, 	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
           {   "random","rasgele"},          {  28, 28, 28,28,28,28,28,28 ,28,28,28,28,28},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_randomizer,      TAR_IGNORE,       POS_STANDING,
        &gsn_randomizer,       SLOT(555),        200,       24,
        "", "Yeniden rasgeleyi kullanabilirsin.", "", CABAL_CHAOS,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	{"tesseract","öte boyut"},          {  29, 29, 29,93,29,29,29,29 ,29,29,29,29,29},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_tesseract,      TAR_IGNORE,       POS_STANDING,
        &gsn_tesseract,       SLOT(556),        150,       12,
        "", "!tesseract!", "",	CABAL_SHALAFI,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
       {   "trophy","andaç"},       {  23, 23, 23,23,23,23,23,23 ,23,23,23,23,23},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_IGNORE,       POS_STANDING,
        &gsn_trophy,       SLOT(0),        30,       12,
        "", "Yeni bir andaçýn hayalini kuruyorsun.", "",	CABAL_BATTLE,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
         {   "truesight","keskingörüþ"},         {  25, 25, 25,25,25,25,25,25 ,25,25,25,25,25},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_IGNORE,       POS_STANDING,
        &gsn_truesight,       SLOT(0),        50,       12,
        "", "Gözlerin keskinliðini yitiriyor.", "",	CABAL_BATTLE,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        {  "brew","mayacý"},     {  23, 23, 23,23,23,23,23,23,23 ,23,23,23,23},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_brew,      TAR_OBJ_INV,       POS_STANDING,
        &gsn_brew,       SLOT(557),        25,       12,
        "", "Yeniden mayalamaya baþlayabilirsin.", "",
	CABAL_SHALAFI,	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        {  "shadowlife","gölgeyaþam"},         {  30, 30, 30,30,30,30,30,30 ,30,30,30,30,30},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_shadowlife,      TAR_CHAR_OFFENSIVE,       POS_STANDING,
        &gsn_shadowlife,       SLOT(558),        80,       12,
        "", "Kendini daha gölgeli hisssediyorsun.", "",	CABAL_INVADER,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
         {   "ruler badge", "tüze niþaný"},        {  25, 25, 25,25,25,25,25,25 ,25,25,25,25,25},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_ruler_badge,      TAR_CHAR_SELF,   POS_STANDING,
        &gsn_ruler_badge,       SLOT(560),        50,       12,
        "", "!tüze niþaný!", "",	CABAL_RULER,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
         {   "remove badge","niþan kaldýr"},        {  28, 28, 28,28,28,28,28,28,28,28,28,28,28 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_remove_badge,      TAR_CHAR_SELF,   POS_STANDING,
        &gsn_remove_badge,       SLOT(561),        100,       12,
        "", "!remove badge!", "",	CABAL_RULER,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
      {  "golden aura","altýn ayla"},         {  10,10,10,10,10,10,10,10,10,10,10,10,10 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_golden_aura,      TAR_IGNORE,   POS_STANDING,
        NULL,       SLOT(564),        25,       12,
        "", "Altýn aylanýn yokolduðunu hissediyorsun.", "",	CABAL_KNIGHT,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        {  "dragonplate", "ejderha zýrhý"},       {  15,15,15,15,15,15,15,15 ,15,15,15,15,15},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_dragonplate,      TAR_IGNORE,   POS_STANDING,
        NULL,       SLOT(565),        60,       12,
        "", "", "",	CABAL_KNIGHT,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
     {   "squire","bey"},       {  25, 25, 25,25,25,25,25,25 ,25,25,25,25,25},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_squire,      TAR_IGNORE,   POS_STANDING,
        &gsn_squire,       SLOT(566),        100,       12,
        "", "Yeni bir beyi düþünmeye baþlýyorsun.", "",	CABAL_KNIGHT,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
     {   "dragonsword", "ejderha kýlýcý"},          {  25,25,25,25,25,25,25,25 ,25,25,25,25,25},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_dragonsword,      TAR_IGNORE,   POS_STANDING,
        &gsn_dragonsword,       SLOT(567),        70,       12,
	"", "", "",	CABAL_KNIGHT,	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
         {   "holy armor", "kutsal zýrh"},         {  10,10,10,10,10,10,10,10,10 ,10,10,10,10},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_holy_armor,      TAR_CHAR_SELF,   POS_RESTING,
        &gsn_knight,       SLOT(569),        20,       12,
        "", "Zarara karþý daha az korumalýsýn.", "",	CABAL_KNIGHT,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },


    {
      {  "disperse","daðýtma"},       {  20,20,20,20,20,20,20,20,20,20,20,20,20 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_disperse,      TAR_IGNORE,   	POS_FIGHTING,
        &gsn_disperse,       SLOT(573),        100,       24,
        "", "Yeniden daðýtabilirsin.", "",CABAL_CHAOS,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	{"hunt","avlama"},        {  13, 13, 13,13,13,13,13,13 ,13,13,13,13,13},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_IGNORE,       POS_STANDING,
        &gsn_hunt,       SLOT(0),        0,       6,
	"", 	"!hunt!", 	"",	CABAL_HUNTER,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	{"find object","nesne ara"},	{   25, 25, 25, 25,25,25,25,25,25 ,25,25,25,25},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_find_object,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(585),	20,	18,
	"",			"!Find Object!",	"", CABAL_HUNTER ,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	{"path find","yol bulma"},		{  13,13 ,13,13,13,13,13,13 ,13 ,13,13,13,13},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_SLEEPING,
	&gsn_path_find,		SLOT( 0),	0,	0,
	"",			"!endur!",	"", CABAL_HUNTER ,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	{"riding","binicilik"},		{   30, 30, 30, 30,30,30,30,30,30 ,30,30,30,30},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_riding,			SLOT(0),	0,	6,
	"",			"!riding!",	"", CABAL_KNIGHT ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"wolf","kurt"},		{   20, 20,20, 20,20,20,20,20,20 ,20,20,20,20},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_wolf,	TAR_IGNORE,		POS_STANDING,
	NULL,		SLOT(593),	100,	12,
  "",	"Daha çok kurdu kontrol edebileceðini hissediyorsun.",	"", CABAL_HUNTER ,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	{"wolf spirit","kurt ruhu"},	{   28, 28, 28, 28,28,28,28,28,28 ,28,28,28,28},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_wolf_spirit,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(685),	50,	12,
  "",	"Damarlarýndaki kan tekrar eskisi gibi akmaya baþladý.",	"",
	CABAL_HUNTER , RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	{"armor use","zýrh kullanýmý"},		{ 17,17,17,17,17,17,17,17,17 ,17,17,17,17},
	{ 2, 1, 1, 1,1,1,1,1,1,1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_armor_use,	SLOT(0),	0,	0,
	"",			"!Armor Use!",		"", CABAL_HUNTER ,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	{"world find","diyar bulma"},	{  53,53 ,53,53,53,53,53,53 ,53 ,53,53,53,53},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_SLEEPING,
	&gsn_world_find,	SLOT( 0),	0,	0,
	"",			"!world find!",	"", CABAL_HUNTER ,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	{"take revenge","intikam"},{  53,53 ,53,53,53,53,53,53 ,53 ,53,53,53,53},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_take_revenge,	TAR_IGNORE,		POS_STANDING,
	NULL,		SLOT(624),	50,		12,
	"",	"!take revenge!",	"",
	CABAL_HUNTER , RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	{"mastering spell","büyü ustalýðý"},	{37,37,37,37,37,37,37,37,37,37,37,37,37},
	{ 1,  1,  1,  1, 1, 1, 2, 2, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_mastering_spell,	SLOT(0),    0,       0,
  "büyü ustalýðý",		"!mastering spell!","",
	CABAL_SHALAFI, RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
       { "guard dogs","muhafýz köpekler"},        {  15, 15, 15,15,15,15,15,15 ,15,15,15,15,15},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_guard_dogs,      TAR_IGNORE,       POS_FIGHTING,
	NULL,       SLOT(687),        100,       12,
        "", 	"", 	"",
	CABAL_LIONS, RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
  {  "eyes of tiger","kaplan gözleri"},        {  25, 25, 25,25,25,25,25,25 ,25,25,25,25,25},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_eyes_of_tiger,      TAR_IGNORE,       POS_FIGHTING,
	NULL,       SLOT(688),        20,       12,
        "", 	"", 	"",
	CABAL_LIONS, RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
    {  "lion shield","aslan kalkaný"},       {  24, 24, 24,24,24,24,24,24 ,24,24,24,24,24},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_lion_shield,      TAR_IGNORE,       POS_FIGHTING,
	NULL,       SLOT(689),        200,       12,
        "", 	"", 	"",
	CABAL_LIONS, RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
      {  "evolve lion","aslana dönüþme"},        {  18, 18, 18,18,18,18,18,18 ,18,18,18,18,18},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_evolve_lion,      TAR_IGNORE,       POS_FIGHTING,
	NULL,       SLOT(690),        50,       12,
        "", 	"", 	"",
	CABAL_LIONS, RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
{   "claw","pençe"},      {  20, 20, 20,20,20,20,20,20 ,20,20,20,20,20},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_IGNORE,       POS_FIGHTING,
	&gsn_claw,       SLOT(0),        50,       24,
  "pençe", 	"", 	"",
	CABAL_LIONS, RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
  {  "prevent","intikam baðý"},   {  35, 35, 35,35,35,35,35,35 ,35,35,35,35,35},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_prevent,      TAR_IGNORE,       POS_FIGHTING,
	NULL,       SLOT(691),        75,       12,
        "", 	"", 	"",
	CABAL_LIONS, RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

/* object spells */

    {
	{"terangreal","uyuþturma"},		{ 93,  93, 93, 93,93,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_terangreal,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_terangreal,	SLOT(607),	 5,	12,
  "uyuþturma",		"Artýk uyanýksýn.",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"kassandra","kasandra"},	{ 93,  93, 93, 93,93,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_kassandra,	TAR_CHAR_SELF,		POS_STANDING,
	&gsn_kassandra,	SLOT(608),	 5,	12,
  "",		"Kendini yine iyileþtirebilirsin.",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"sebat","sebat"},		{ 93,  93, 93, 93,93,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_sebat,		TAR_CHAR_SELF,		POS_STANDING,
	&gsn_sebat,		SLOT(609),	 5,	12,
  "",			"Kendini yine koruyabilirsin.",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{"matandra","matandra"},	{ 93,  93, 93, 93,93,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_matandra,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_matandra,		SLOT(610),	 5,	12,
  "matandra",	"Kasandrayý tekrar kullanabilirsin.",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    }

};
