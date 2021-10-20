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
#include "magic.h"
#include "interp.h"

/* language staff */
const struct translation_type translation_table[]=
{
        { L"a",                   L"e"},
        { L"A",                   L"E"},
        { L"b",                   L"c"},
        { L"B",                   L"C"},
        { L"c",                   L"d"},
        { L"C",                   L"D"},
        { L"d",                   L"f"},
        { L"D",                   L"F"},
        { L"e",                   L"i"},
        { L"E",                   L"I"},
        { L"f",                   L"g"},
        { L"F",                   L"G"},
        { L"g",                   L"h"},
        { L"G",                   L"H"},
        { L"h",                   L"j"},
        { L"H",                   L"J"},
        { L"i",                   L"o"},
        { L"I",                   L"O"},
        { L"j",                   L"k"},
        { L"J",                   L"K"},
        { L"k",                   L"l"},
        { L"K",                   L"L"},
        { L"l",                   L"m"},
        { L"L",                   L"M"},
        { L"m",                   L"n"},
        { L"M",                   L"N"},
        { L"n",                   L"p"},
        { L"N",                   L"P"},
        { L"o",                   L"u"},
        { L"O",                   L"U"},
        { L"p",                   L"q"},
        { L"P",                   L"Q"},
        { L"q",                   L"r"},
        { L"Q",                   L"R"},
        { L"r",                   L"s"},
        { L"R",                   L"S"},
        { L"s",                   L"t"},
        { L"S",                   L"T"},
        { L"t",                   L"v"},
        { L"T",                   L"V"},
        { L"u",                   L"y"},
        { L"U",                   L"Y"},
        { L"v",                   L"w"},
        { L"V",                   L"W"},
        { L"w",                   L"x"},
        { L"W",                   L"X"},
        { L"x",                   L"z"},
        { L"X",                   L"Z"},
        { L"y",                   L"a"},
        { L"Y",                   L"A"},
        { L"z",                   L"b"},
        { L"Z",                   L"B"},
        { L"",            	L"" }
};

const   struct  language_type       language_table      [ MAX_LANGUAGE ] =
{
  { L"ortak",	LANG_COMMON	},
  { L"insan",	LANG_HUMAN	},
  { L"çora",	LANG_CORA	},
  { L"cüce",	LANG_DWARVISH	},
  { L"kuş",	LANG_BIRD	},
  { L"kedi",	LANG_CAT	},
  { L"asura",	LANG_ASURA	},
  { L"sürüngen",	LANG_SURUNGEN	},
  { L"astral",	LANG_ASTRAL	},
  { L"dev",	LANG_GIANT	},
  { L"ejderha",	LANG_DRAGON	},
  { L"su",	LANG_WATER	},
  { L"su",	LANG_WATER	},
  { L"köpek",	LANG_DOG	},
};

/* item type list */
const struct item_type		item_table	[]	=
{
    {	ITEM_LIGHT, L"light"		},
    {	ITEM_SCROLL, L"scroll"	},
    {	ITEM_WAND, L"wand"		},
    {   ITEM_STAFF, L"staff"		},
    {   ITEM_WEAPON, L"weapon"	},
    {   ITEM_TREASURE, L"treasure"	},
    {   ITEM_ARMOR, L"armor"		},
    {	ITEM_POTION, L"potion"	},
    {	ITEM_CLOTHING, L"clothing"	},
    {   ITEM_FURNITURE, L"furniture"	},
    {	ITEM_TRASH, L"trash"		},
    {	ITEM_CONTAINER, L"container"	},
    {	ITEM_DRINK_CON, L"drink"		},
    {	ITEM_KEY, L"key"		},
    {	ITEM_FOOD, L"food"		},
    {	ITEM_MONEY, L"money"		},
    {	ITEM_BOAT, L"boat"		},
    {	ITEM_CORPSE_NPC, L"npc_corpse"	},
    {	ITEM_CORPSE_PC, L"pc_corpse"	},
    {   ITEM_FOUNTAIN, L"fountain"	},
    {	ITEM_PILL, L"pill"		},
    {	ITEM_PROTECT, L"protect"	},
    {	ITEM_MAP, L"map"		},
    {	ITEM_PORTAL, L"portal"	},
    {	ITEM_WARP_STONE,L"warp_stone"	},
    {	ITEM_ROOM_KEY, L"room_key"	},
    {	ITEM_GEM, L"gem"		},
    {	ITEM_JEWELRY, L"jewelry"	},
    {   ITEM_JUKEBOX, L"jukebox"	},
    {   ITEM_TATTOO, L"tattoo"	},
    {   ITEM_MAYMUNCUK, L"maymuncuk"	},
    {   0,		NULL		}
};


/* weapon selection table */
const	struct	weapon_type	weapon_table	[]	=
{
   { L"sword",	OBJ_VNUM_SCHOOL_SWORD,	WEAPON_SWORD,	&gsn_sword	},
   { L"mace",	OBJ_VNUM_SCHOOL_MACE,	WEAPON_MACE,	&gsn_mace 	},
   { L"dagger",	OBJ_VNUM_SCHOOL_DAGGER,	WEAPON_DAGGER,	&gsn_dagger	},
   { L"axe",	OBJ_VNUM_SCHOOL_AXE,	WEAPON_AXE,	&gsn_axe	},
   { L"staff",	OBJ_VNUM_SCHOOL_STAFF,	WEAPON_SPEAR,	&gsn_spear	},
   { L"flail",	OBJ_VNUM_SCHOOL_FLAIL,	WEAPON_FLAIL,	&gsn_flail	},
   { L"whip",	OBJ_VNUM_SCHOOL_WHIP,	WEAPON_WHIP,	&gsn_whip	},
   { L"polearm",	OBJ_VNUM_SCHOOL_POLEARM,WEAPON_POLEARM,	&gsn_polearm	},
   { L"bow",	OBJ_VNUM_SCHOOL_BOW,	WEAPON_BOW,	&gsn_bow	},
   { L"arrow",	OBJ_VNUM_SCHOOL_POLEARM,WEAPON_ARROW,	&gsn_arrow	},
   { L"lance",	OBJ_VNUM_SCHOOL_LANCE,	WEAPON_LANCE,	&gsn_lance	},
   { NULL,	0,				0,	NULL		}
};



/* wiznet table and prototype for future flag setting */
const   struct wiznet_type      wiznet_table    []              =
{
   { L"on",           WIZ_ON,         IM },
   { L"prefix",	WIZ_PREFIX,	IM },
   { L"ticks",        WIZ_TICKS,      IM },
   { L"logins",       WIZ_LOGINS,     IM },
   { L"sites",        WIZ_SITES,      L4 },
   { L"links",        WIZ_LINKS,      L7 },
   {	L"newbies",	WIZ_NEWBIE,	IM },
   {	L"spam",		WIZ_SPAM,	L5 },
   { L"deaths",       WIZ_DEATHS,     IM },
   { L"resets",       WIZ_RESETS,     L4 },
   { L"mobdeaths",    WIZ_MOBDEATHS,  L4 },
   { L"flags",	WIZ_FLAGS,	L5 },
   {	L"penalties",	WIZ_PENALTIES,	L5 },
   {	L"saccing",	WIZ_SACCING,	L5 },
   {	L"levels",	WIZ_LEVELS,	IM },
   {	L"load",		WIZ_LOAD,	L2 },
   {	L"restore",	WIZ_RESTORE,	L2 },
   {	L"snoops",	WIZ_SNOOPS,	L2 },
   {	L"switches",	WIZ_SWITCHES,	L2 },
   {	L"secure",	WIZ_SECURE,	L1 },
   {	NULL,		0,		0  }
};

/* attack table */
const 	struct attack_type	attack_table	[]		=
{
    {	L"none", L"vuruş",		-1		},  /*  0 */
    {	L"slice",  L"dilme", 	DAM_SLASH	},
    { L"stab", L"saplama",		DAM_PIERCE	},
    {	L"slash",  L"kesici",	DAM_SLASH	},
    {	L"whip", L"kamçı",		DAM_SLASH	},
    { L"claw", L"pençe",		DAM_SLASH	},  /*  5 */
    {	L"blast",  L"patlama",	DAM_BASH	},
    { L"pound",  L"çarpma",	DAM_BASH	},
    {	L"crush",  L"ezme",	DAM_BASH	},
    { L"grep", L"kesici",		DAM_SLASH	},
    {	L"bite", L"ısırma",		DAM_PIERCE	},  /* 10 */
    { L"pierce",  L"delici",	DAM_PIERCE	},
    { L"suction",  L"emici",	DAM_BASH	},
    {	L"beating",  L"beating",	DAM_BASH	},
    { L"digestion",  L"öğütücü",	DAM_ACID	},
    {	L"charge",  L"yüklenme",	DAM_BASH	},  /* 15 */
    {	L"slap", L"tokat",		DAM_BASH	},
    {	L"punch",  L"yumruk",	DAM_BASH	},
    {	L"wrath",  L"gazap",	DAM_ENERGY	},
    {	L"magic",  L"büyü",	DAM_ENERGY	},
    { L"divine",  L"ilahi güç",	DAM_HOLY	},  /* 20 */
    {	L"cleave",  L"yarma",	DAM_SLASH	},
    {	L"scratch",  L"tırmalama",	DAM_PIERCE	},
    { L"peck", L"gaga",		DAM_PIERCE	},
    { L"peckb",  L"gaga",		DAM_BASH	},
    { L"chop", L"chop",		DAM_SLASH	},  /* 25 */
    { L"sting",  L"sokma",	DAM_PIERCE	},
    { L"smash",	 L"smash",	DAM_BASH	},
    { L"shbite",  L"şok ısırışı",DAM_LIGHTNING	},
    {	L"flbite",  L"alev ısırışı", DAM_FIRE	},
    {	L"frbite",  L"ayaz ısırışı", DAM_COLD	},  /* 30 */
    {	L"acbite",  L"asit ısırışı", 	DAM_ACID	},
    {	L"chomp",  L"çiğneme",	DAM_PIERCE	},
    {  	L"drain",  L"yaşam emici",	DAM_NEGATIVE	},
    { L"thrust",  L"thrust",	DAM_PIERCE	},
    { L"slime",  L"balçık",	DAM_ACID	},
    {	L"shock",  L"şok",	DAM_LIGHTNING	},
    { L"thwack",  L"thwack",	DAM_BASH	},
    { L"flame",  L"alev",	DAM_FIRE	},
    { L"chill",  L"ayaz",	DAM_COLD	},
    {   NULL,		NULL,		0		}
};

/* race table */
const 	struct	race_type	race_table	[]		=
{

    { { L"unique", L"eşsiz"},	L"",	0, FALSE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,{ L"" }, {0,0,0,0,0,0 } },

    { /* 1 */
	{ L"human", L"insan"},L"İnsan",LANG_HUMAN,	TRUE, 		SIZE_MEDIUM, 0,
	0,		0, 		0,
	0, 		0,		0,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, 0, { L"identify", L"steal" }, { 20, 21, 20, 21, 21 ,20}
    },

    { /* 2 */
	{ L"çora", L"çora"},L"Çora",LANG_CORA,		TRUE, 		SIZE_MEDIUM, 0,
	0,		AFF_INFRARED|AFF_SNEAK,	0,
	0,		RES_CHARM,	VULN_IRON,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, 500, { L"lore" , L"camouflage" }, { 20, 23, 20, 22, 18, 20 }
    },

    { /* 3 */
	{ L"dwarf", L"cüce"},L"Cüce",LANG_DWARVISH,	TRUE, 		SIZE_MEDIUM, 0,
	0,		AFF_INFRARED,	0,
	0,		RES_POISON|RES_DISEASE, VULN_DROWNING,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, 250, { L"berserk" }, { 21, 20, 21, 18, 23, 20 }
    },

    { /* 4 */
        { L"gamayun", L"gamayun"},L"Gamayun",LANG_BIRD,        TRUE, 		SIZE_MEDIUM, 0,
        0,              AFF_FLYING,             0,
        0,              0,              0,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K, 250, { L"" }, { 18, 23, 20, 24, 18 , 20}
    },

    { /* 5  */
  { L"pardus", L"pardus"},L"Pardus",LANG_CAT,	TRUE, 		SIZE_MEDIUM, 0,
  0,		AFF_INFRARED,	0,
  0,		RES_WOOD|RES_DISEASE,	0,
  A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, 250, { L"ambush", L"berserk", L"steal" }, { 21, 19, 20, 23, 21, 20}
    },

    {  /* 6 */
	{ L"asura", L"asura"},L"Asura",LANG_ASURA,	TRUE, 		SIZE_LARGE, 0,
	0,		AFF_REGENERATION|AFF_INFRARED,	OFF_BERSERK,
 	0,	RES_CHARM|RES_BASH,	VULN_FIRE|VULN_ACID,
	A|B|H|M|V,		A|B|C|D|E|F|G|H|I|J|K|U|V, 400, { L"fast healing", L"trance" }, { 22, 19, 19, 21, 23, 20}
    },

        { /* 7 */
          { L"yeg", L"yeg"},L"",LANG_DWARVISH,        FALSE, 		SIZE_MEDIUM, 0,
          0,                AFF_INFRARED,   0,
          0,                RES_MAGIC|RES_POISON|RES_DISEASE, VULN_DROWNING,
          A|H|M|V,          A|B|C|D|E|F|G|H|I|J|K, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },

        { /* 8 */
            { L"börü", L"börü"},L"", LANG_DOG,       FALSE, 		SIZE_MEDIUM, 0,
            0,              AFF_INFRARED,   OFF_TAIL,
            0,              RES_LIGHT|RES_COLD,     VULN_FIRE|VULN_DROWNING,
            A|H|M|V,        A|C|D|E|F|H|J|K|Q|U|V, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },

        {  /* 9 */
          { L"naga", L"naga"},L"",LANG_SURUNGEN,     FALSE, 		SIZE_MEDIUM, 0,
          0,                AFF_INFRARED,   0,
          0,                RES_MAGIC|RES_POISON|RES_DISEASE, VULN_BASH,
          A|H|M|V,          A|B|C|D|E|F|G|H|I|J|K, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },

        { /* 10 */
            { L"gnome", L"gnom"},L"",LANG_DWARVISH,        FALSE, 		SIZE_MEDIUM, 0,
            0,              AFF_INFRARED,   0,
            0,              RES_POISON,     VULN_BASH,
            A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },

        { /* 11 */
    	{ L"githyanki", L"githyanki"},L"",LANG_ASTRAL,	FALSE,  		SIZE_MEDIUM, 0,
    	0,		AFF_INFRARED,	0,
    	0, 		0,		0,
    	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },

        { /* 12 */
      { L"storm giant", L"fırtına devi"},L"",LANG_GIANT,	FALSE, 		SIZE_GIANT, 0,
      0,		AFF_FLYING,		0,
      0,		RES_LIGHTNING,	VULN_MENTAL|VULN_COLD|VULN_FIRE,
      A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },

        { /* 13 */
      { L"cloud giant", L"bulut devi"},	L"",LANG_GIANT,FALSE, 		SIZE_GIANT, 0,
      0,		AFF_FLYING,		0,
      0,		RES_WEAPON,	VULN_MENTAL|VULN_LIGHTNING,
      A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },

        { /* 14 */
      { L"fire giant", L"ateş devi"},L"",LANG_GIANT,	FALSE, 		SIZE_GIANT, 0,
      0,		0,		0,
      0,		RES_FIRE,	VULN_MENTAL|VULN_LIGHTNING|VULN_COLD,
      A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },

        { /* 15 */
      { L"frost giant", L"buz devi"},L"",LANG_GIANT,	FALSE, 		SIZE_GIANT, 0,
      0,		0,		0,
      0,		RES_COLD,	VULN_MENTAL|VULN_LIGHTNING|VULN_FIRE,
      A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },

        { /* 16 */
            { L"black dragon", L"kara ejderha"},L"",LANG_DRAGON, FALSE,  	SIZE_GARGANTUAN, 0,
    	0, 		AFF_INFRARED|AFF_FLYING,	0,
    	0,		RES_BASH|RES_CHARM|RES_ACID,	VULN_PIERCE,
            A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },

        { /* 17 */
            { L"blue dragon", L"mavi ejderha"}, L"",LANG_DRAGON,	FALSE, 		SIZE_GARGANTUAN, 0,
    	0, 		AFF_INFRARED|AFF_FLYING,	0,
    	0,		RES_BASH|RES_CHARM|RES_LIGHTNING,
    	VULN_PIERCE,
            A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },

        { /* 18 */
            { L"green dragon", L"yeşil ejderha"},L"",LANG_DRAGON, FALSE, 		SIZE_GARGANTUAN, 0,
    	0, 		AFF_INFRARED|AFF_FLYING,	0,
    	0,		RES_BASH|RES_CHARM|RES_POISON,	VULN_PIERCE,
            A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },

        { /* 19 */
            { L"red dragon", L"kızıl ejderha"},L"", LANG_DRAGON,	FALSE, 		SIZE_GARGANTUAN, 0,
    	0, 		AFF_INFRARED|AFF_FLYING,	0,
    	0,		RES_BASH|RES_CHARM|RES_FIRE,	VULN_PIERCE,
            A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },

        { /* 20 */
            { L"white dragon", L"ak ejderha"},L"",LANG_DRAGON, FALSE, 		SIZE_GARGANTUAN, 0,
    	0, 		AFF_INFRARED|AFF_FLYING,	0,
    	0,		RES_BASH|RES_CHARM|RES_COLD,	VULN_PIERCE,
            A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },

        { /* 21 */
            { L"brass dragon", L"pirinç ejderha"},L"",LANG_DRAGON, FALSE, 		SIZE_GARGANTUAN, 0,
    	0, 		AFF_INFRARED|AFF_FLYING,	0,
    	0,		RES_BASH|RES_CHARM|RES_FIRE,	VULN_PIERCE,
            A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },

        { /* 22 */
            { L"gold dragon", L"altın ejderha"},L"",LANG_DRAGON, 	FALSE, 		SIZE_GARGANTUAN, 0,
    	0, 		AFF_INFRARED|AFF_FLYING,	0,
    	0,		RES_BASH|RES_CHARM|RES_FIRE|RES_POISON,
    	VULN_PIERCE,
            A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },

        { /* 23 */
            { L"silver dragon", L"gümüş ejderha"},L"",LANG_DRAGON,FALSE, 		SIZE_GARGANTUAN, 0,
    	0, 		AFF_INFRARED|AFF_FLYING,	0,
    	0,		RES_BASH|RES_CHARM|RES_COLD,	VULN_PIERCE,
            A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },

        { /* 24 */
            { L"bronze dragon", L"tunç ejderha"},L"",LANG_DRAGON,FALSE, 		SIZE_GARGANTUAN, 0,
    	0, 		AFF_INFRARED|AFF_FLYING,	0,
    	0,		RES_BASH|RES_CHARM|RES_LIGHTNING,
    	VULN_PIERCE,
            A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },

        { /* 25 */
            { L"copper dragon", L"bakır ejderha"},L"",LANG_DRAGON,FALSE, 		SIZE_GARGANTUAN, 0,
    	0, 		AFF_INFRARED|AFF_FLYING,	0,
    	0,		RES_BASH|RES_CHARM|RES_ACID,	VULN_PIERCE,
            A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },

        { /* 26 */
    	{ L"bat", L"yarasa"},L"",LANG_BIRD,		FALSE, 		SIZE_TINY, 0,
    	0,		AFF_FLYING/*|AFF_DARK_VISION*/,	OFF_DODGE|OFF_FAST,
    	0,		0,		VULN_LIGHT,
    	A|G|V,		A|C|D|E|F|H|J|K|P, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },

        { /* 27 */
    	{ L"bear", L"ayı"},	L"",	LANG_DOG, FALSE, 		SIZE_LARGE, 0,
    	0,		0,		OFF_CRUSH|OFF_DISARM|OFF_BERSERK,
    	0,		RES_BASH|RES_COLD,	0,
    	A|G|V,		A|B|C|D|E|F|H|J|K|U|V, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },

        {  /* 28 */
    	{ L"cat", L"kedi"},L"",LANG_CAT,		FALSE, 		SIZE_SMALL, 0,
    	0,		/*AFF_DARK_VISION*/0,	OFF_FAST|OFF_DODGE,
    	0,		0,		0,
    	A|G|V,		A|C|D|E|F|H|J|K|Q|U|V, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },

        {  /* 29 */
    	{ L"centipede", L"kırkayak"},L"",LANG_SURUNGEN,	FALSE, 		SIZE_TINY, 0,
    	0,		/*AFF_DARK_VISION*/0,	0,
    	0,		RES_PIERCE|RES_COLD,	VULN_BASH,
     	A|B|G|O,		A|C|K, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },

        {  /* 30 */
    	{ L"dog", L"köpek"},L"",LANG_DOG,		FALSE, 		SIZE_SMALL, 0,
    	0,		0,		OFF_FAST,
    	0,		0,		0,
    	A|G|V,		A|C|D|E|F|H|J|K|U|V, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },

        { /* 31 */
    	{ L"doll", L"kukla"},L"",LANG_COMMON,		FALSE, 		SIZE_SMALL, 0,
    	0,		0,		0,
    	IMM_COLD|IMM_POISON|IMM_HOLY|IMM_NEGATIVE|IMM_MENTAL|IMM_DISEASE
    	|IMM_DROWNING,	RES_BASH|RES_LIGHT,
    	VULN_SLASH|VULN_FIRE|VULN_ACID|VULN_LIGHTNING|VULN_ENERGY,
    	E|J|M|cc,	A|B|C|G|H|K, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },

        { /* 32 */
    	{ L"fido", L"enik"},L"",LANG_DOG,		FALSE, 		SIZE_SMALL, 0,
    	0,		0,		OFF_DODGE|ASSIST_RACE,
    	0,		0,			VULN_MAGIC,
    	A|B|G|V,	A|C|D|E|F|H|J|K|Q|V, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },

        { /* 33 */
    	{ L"fox", L"tilki"},L"",LANG_DOG,		FALSE, 		SIZE_SMALL, 0,
    	0,		/*AFF_DARK_VISION*/0,	OFF_FAST|OFF_DODGE,
    	0,		0,		0,
    	A|G|V,		A|C|D|E|F|H|J|K|Q|V, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },

        { /* 34 */
    	{ L"goblin", L"gulyabani"},L"",LANG_CORA,	FALSE, 		SIZE_MEDIUM, 0,
    	0,		AFF_INFRARED,	0,
    	0,		RES_DISEASE,	VULN_MAGIC,
    	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },

        { /* 35 */
    	{ L"hobgoblin", L"ifrit"},L"",LANG_CORA,	FALSE, 		SIZE_MEDIUM, 0,
    	0,		AFF_INFRARED,	0,
    	0,		RES_DISEASE|RES_POISON,	0,
    	A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K|Y, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },

        {  /* 36 */
    	{ L"kobold", L"argun"},L"",LANG_CORA,	FALSE, 		SIZE_MEDIUM, 0,
    	0,		AFF_INFRARED,	0,
    	0,		RES_POISON,	VULN_MAGIC,
    	A|B|H|M|V,	A|B|C|D|E|F|G|H|I|J|K|Q, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },

        {  /* 37 */
    	{ L"lizard", L"kertenkele"},L"",LANG_SURUNGEN,	FALSE, 		SIZE_SMALL, 0,
    	0,		0,		0,
    	0,		RES_POISON,	VULN_COLD,
    	A|G|X|cc,	A|C|D|E|F|H|K|Q|V, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },

        {  /* 38 */
    	{ L"modron", L"geme"},L"",LANG_COMMON,	FALSE, 		SIZE_MEDIUM, 0,
    	0,		AFF_INFRARED,	ASSIST_RACE|ASSIST_ALIGN,
    	IMM_CHARM|IMM_DISEASE|IMM_MENTAL|IMM_HOLY|IMM_NEGATIVE,
    			RES_FIRE|RES_COLD|RES_ACID,	0,
    	H,		A|B|C|G|H|J|K, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },

        {  /* 39 */
    	{ L"orc", L"ork"},L"",LANG_CORA,		FALSE, 		SIZE_MEDIUM, 0,
    	0,		AFF_INFRARED,	0,
    	0,		RES_DISEASE,	VULN_LIGHT,
    	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },

        {  /* 40 */
    	{ L"pig", L"domuz"},L"",	LANG_DOG,	FALSE, 		SIZE_MEDIUM, 0,
    	0,		0,		0,
    	0,		0,		0,
    	A|G|V,	 	A|C|D|E|F|H|J|K, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },

        { /* 41 */
    	{ L"rabbit", L"tavşan"},L"",LANG_DOG,	FALSE, 		SIZE_SMALL, 0,
    	0,		0,		OFF_DODGE|OFF_FAST,
    	0,		0,		0,
    	A|G|V,		A|C|D|E|F|H|J|K, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },

        { /* 42 */
    	{ L"school monster", L"okul canavarı"},L"",LANG_HUMAN,	FALSE, 		SIZE_MEDIUM, 0,
    	ACT_NOALIGN,		0,		0,
    	IMM_CHARM|IMM_SUMMON,	0,		VULN_MAGIC,
    	A|M|V,		A|B|C|D|E|F|H|J|K|Q|U, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },

        {  /* 43 */
    	{ L"snake", L"yılan"},L"",LANG_SURUNGEN,	FALSE, 		SIZE_SMALL, 0,
    	0,		0,		0,
    	0,		RES_POISON,	VULN_COLD,
    	A|G|X|Y|cc,	A|D|E|F|K|L|Q|V|X, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },

        {  /* 44 */
    	{ L"song bird", L"serçe"},L"",LANG_BIRD,	FALSE, 		SIZE_TINY, 0,
    	0,		AFF_FLYING,		OFF_FAST|OFF_DODGE,
    	0,		0,		0,
    	A|G|W,		A|C|D|E|F|H|K|P, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },

        {  /* 45 */
    	{ L"water fowl", L"ördek"},L"",LANG_WATER,	FALSE, 		SIZE_SMALL, 0,
    	0,		AFF_SWIM|AFF_FLYING,	0,
    	0,		RES_DROWNING,		0,
    	A|G|W,		A|C|D|E|F|H|K|P, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },

        {  /* 46 */
    	{ L"wolf", L"kurt"},L"",LANG_DOG,		FALSE, 		SIZE_MEDIUM, 0,
    	0,		/*AFF_DARK_VISION*/0,	OFF_FAST|OFF_DODGE,
    	0,		0,		0,
    	A|G|V,		A|C|D|E|F|J|K|Q|V, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },

        {  /* 47 */
    	{ L"wyvern", L"yelbüke"},L"",LANG_DRAGON,	FALSE, 		SIZE_HUGE, DETECT_INVIS|DETECT_HIDDEN,
    	0,		AFF_FLYING,	OFF_BASH|OFF_FAST|OFF_DODGE,
    	IMM_POISON,	0,	VULN_LIGHT,
    	A|B|G|Z,		A|C|D|E|F|H|J|K|Q|V|X, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },

        { /* 48 */
            { L"dragon", L"ejderha"},L"", LANG_DRAGON,	FALSE,		SIZE_GARGANTUAN, 0,
    	0, 		AFF_INFRARED|AFF_FLYING,	0,
    	0,		RES_FIRE|RES_BASH|RES_CHARM,	VULN_PIERCE|VULN_COLD,
    	A|H|Z,		A|C|D|E|F|G|H|I|J|K|P|Q|U|V|X, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },

        { /* 49 */
    	{ L"giant", L"dev"},L"",LANG_GIANT,	FALSE, 		SIZE_GIANT, 0,
    	0,		0,		0,
    	0,		RES_FIRE|RES_COLD,	VULN_MENTAL|VULN_LIGHTNING,
    	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },

        { /* 50 */
    	{ L"golem", L"golem"},L"",LANG_HUMAN,	FALSE, 		SIZE_LARGE, 0,
    	0,		0,		0,
    	IMM_POISON|IMM_DISEASE,		RES_WEAPON,	VULN_MENTAL|VULN_MAGIC,
    	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },

        { /* 51 */
    	{ L"undead", L"hortlak"},L"",LANG_ASTRAL,	FALSE, SIZE_MEDIUM,
    	DETECT_INVIS|DETECT_HIDDEN|DETECT_GOOD|DETECT_EVIL,
    	0,		AFF_PASS_DOOR|AFF_INFRARED,	0,
    	IMM_DROWNING|IMM_DISEASE|IMM_POISON,	RES_NEGATIVE|RES_ENERGY,
    	VULN_SILVER|VULN_HOLY|VULN_LIGHT,
    	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },

        { /* 52 */
    	{ L"drow", L"rayel"},	L"",LANG_CORA,	FALSE,		SIZE_MEDIUM, 0,
    	0,		AFF_INFRARED,	0,
    	0,		RES_CHARM,		VULN_IRON,
    	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },

        { /* 53 */
    	{ L"draconian", L"luu"},L"",LANG_DRAGON,	FALSE,		SIZE_LARGE, 0,
    	0,		0,		0,
    	0,		RES_SLASH|RES_POISON,	VULN_COLD,
    	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },

        { /* 54 */
    	{ L"sprite", L"peri"},L"",LANG_CORA,	FALSE,		SIZE_SMALL, DETECT_EVIL,
    	0,		AFF_FLYING,		G,
    	0,		0,			VULN_COLD|VULN_WEAPON,
    	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },

        { /* 55 */
    	{ L"aarakocra", L"kerkes"},L"",LANG_BIRD,	FALSE,		SIZE_MEDIUM, 0,
    	ACT_SCAVENGER,	AFF_FLYING,	OFF_FAST,
    	0,		0,		0,
    	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K|P, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },

        { /*  56 */
    	{ L"beholder", L"çokgöz"},L"",LANG_COMMON,	FALSE,		SIZE_MEDIUM, 0,
    	0,		AFF_REGENERATION,		0,
    	0,		RES_MAGIC,		0,
    	A|H|M|V,	A|E|K, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },

        {  /* 57 */
    	{ L"tiger", L"kaplan"},L"",LANG_CAT,	FALSE,		SIZE_MEDIUM, 0,
    	0,	/*AFF_DARK_VISION*/0, OFF_FAST|OFF_DODGE,
    	0,		RES_WEAPON,		0,
    	A|G|V,		A|C|D|E|F|H|J|K|Q|U|V, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },

        {  /* 58 */
    	{ L"lion", L"aslan"},L"",LANG_CAT,		FALSE,		SIZE_MEDIUM, 0,
    	ACT_SENTINEL,	/*AFF_DARK_VISION*/0,	OFF_FAST|OFF_DODGE,
    	0,		RES_WEAPON,		0,
    	A|G|V,		A|C|D|E|F|H|J|K|Q|U|V, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },

        {  /* 59 */
    	{ L"death knight", L"aldacı"},L"",LANG_HUMAN,	FALSE,		SIZE_MEDIUM, 0,
    	ACT_UNDEAD,	0, 	OFF_PARRY|OFF_DISARM,
    	IMM_DISEASE|IMM_POISON|IMM_DROWNING,	RES_FIRE|RES_NEGATIVE,
    	VULN_HOLY,
    	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
        },


    { /* 60 */
    { L"dracolich", L"alavan"}, L"",LANG_DRAGON,	FALSE, 		SIZE_LARGE, 0,
    0, 		AFF_INFRARED|AFF_FLYING,	0,
    IMM_CHARM|IMM_LIGHTNING|IMM_COLD,	RES_FIRE|RES_BASH,
    VULN_PIERCE|VULN_COLD,
    A|H|Z,		A|C|D|E|F|G|H|I|J|K|P|Q|U|V|X, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
    },

    {  /* 61 */
    { L"air elemental", L"hava uşağı"},L"",LANG_ASTRAL,FALSE,		SIZE_HUGE, 0,
    0,		0,		OFF_FADE,
    0,		RES_WEAPON|RES_SUMMON,		0,
    A|H|Z,		A|D, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
    },

    {  /* 62 */
    { L"earth elemental", L"toprak uşağı"},L"",LANG_ASTRAL,	FALSE,		SIZE_HUGE, 0,
    0,		0,	OFF_FADE,
    0,		RES_WEAPON|RES_SUMMON,		0,
    A|H|Z,		A|D, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
    },

    {  /* 63 */
    { L"fire elemental", L"ateş uşağı"},L"",LANG_ASTRAL,FALSE,		SIZE_HUGE, 0,
    0,		0,	OFF_FADE,
    IMM_FIRE,		RES_WEAPON,		VULN_COLD,
    A|H|Z,		A|D, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
    },

    {  /* 64 */
    { L"water elemental", L"su uşağı"},L"",LANG_ASTRAL,	FALSE,		SIZE_HUGE, 0,
    0,		0,	OFF_FADE,
    0,		RES_WEAPON|RES_FIRE|RES_COLD,	0,
    A|H|Z,		A|D, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
    },

    {  /* 65 */
    { L"fish", L"balık"},	L"",LANG_WATER,		FALSE,		SIZE_TINY, 0,
    0,		AFF_SWIM,	0,
    0,		0,		VULN_COLD|VULN_FIRE,
    A|H|Z,		A|D|E|F|J|K|O|X, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
    },

    {  /* 66 */
    { L"gargoyle", L"togrıl"},L"",LANG_HUMAN,	FALSE,		SIZE_MEDIUM, 0,
    ACT_AGGRESSIVE,	AFF_FLYING,	OFF_FAST,
    0,		RES_WEAPON,	VULN_COLD|VULN_FIRE,
    A|G|W,		A|C|D|E|F|H|K|P, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
    },

    {  /* 67 */
    { L"ghost", L"hayalet"},L"",LANG_ASTRAL,	FALSE,		SIZE_MEDIUM, 0,
    ACT_UNDEAD,	AFF_FLYING,	OFF_FADE,
    IMM_DROWNING|IMM_NEGATIVE|IMM_DISEASE|IMM_POISON,
    RES_WEAPON, VULN_HOLY,
    A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
    },

    {  /* 68 */
    { L"ghoul", L"gul"},L"",LANG_HUMAN,	FALSE,		SIZE_MEDIUM, 0,
    0,		0,		0,
    IMM_DISEASE,	RES_CHARM,	VULN_MAGIC,
    A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
    },

    {  /* 69 */
    { L"gnoll", L"arböri"},	L"",LANG_DWARVISH,FALSE,		SIZE_MEDIUM, DETECT_INVIS|DETECT_HIDDEN,
    0,		0,		OFF_DISARM,
    0,		0,		0,
    A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
    },

    {  /* 70 */
    { L"harpy", L"harpi"},L"",	LANG_BIRD,FALSE,		SIZE_MEDIUM, DETECT_INVIS|DETECT_HIDDEN,
    0,		AFF_FLYING,		OFF_DISARM,
    0,		RES_CHARM,		0,
    A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K|P, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
    },

    {  /* 71 */
    { L"lamia", L"lamia"},L"",LANG_HUMAN,	FALSE,		SIZE_MEDIUM, 0,
    ACT_AGGRESSIVE,	0,		OFF_FAST|OFF_DODGE,
    0,		RES_CHARM,		0,
    A|G|V,		A|B|C|D|E|F|G|H|I|J|K|Q|U, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
    },

    {  /* 72 */
    { L"lich", L"yelviçin"},L"",LANG_ASTRAL,	FALSE,		SIZE_MEDIUM, 0,
    0,		0,		OFF_FAST|OFF_DODGE,
    IMM_POISON|IMM_DISEASE|IMM_CHARM|IMM_LIGHTNING|IMM_COLD,
    RES_WEAPON|RES_FIRE,	VULN_HOLY,
    A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
    },

    {  /* 73 */
    { L"minotaur", L"bogra"},L"",LANG_HUMAN,	FALSE,		SIZE_LARGE, 0,
    0,		0,		OFF_CRUSH|OFF_FAST|OFF_BASH,
    0,		0,		0,
    A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K|W, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
    },

    {  /* 74 */
    { L"ogre", L"ogre"},L"",LANG_DWARVISH,		FALSE,		SIZE_LARGE, 0,
    0,		AFF_INFRARED,	0,
    0,		RES_DISEASE,	0,
    A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
    },

    {  /* 75 */
    { L"zombie", L"hortlak"},	L"",LANG_ASTRAL,	FALSE,		SIZE_MEDIUM, 0,
    0,		AFF_SLOW,	0,
    IMM_MAGIC|IMM_POISON|IMM_CHARM,	RES_COLD, VULN_WEAPON,
    A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
    },

    {  /* 76 */
    { L"horse", L"at"},	L"",LANG_DOG,FALSE, 		SIZE_LARGE, 0,
    0,		0,		OFF_FAST|OFF_DODGE,
    0,		0,		0,
    A|G|V,		A|C|D|E|F|J|K|Q, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
    },

    {  /* 77 */
    { L"unique", L"eşsiz"},L"",0,	FALSE,		0, 0,
    0,		0,		0,
    0,		0,		0,
    0,		0, 0, { L"" }, {20, 20, 20, 20, 20, 20 }
    },

    {
	{NULL,NULL}, 0,0,0, 0, 0,
   0, 0, 0,
   0,0,0,
    0,0,0, 0, 0
  }
};


/*
 * Class table.
 */
const	struct	class_type	class_table	[MAX_CLASS]	=
{

    {
        { L"invoker", L"gan"}, L"Gan",  STAT_INT,  OBJ_VNUM_SCHOOL_DAGGER,
        { 3018, 0, 0, 0, 0, 0 },  75,  18, 6,  60,  100, TRUE,
        0,{0,3,0,0,0,5},CR_ALL
    },

    {
	{ L"cleric", L"ermiş"}, L"Erm",  STAT_WIS,  OBJ_VNUM_SCHOOL_SWORD,
	{ 3003, 9619, 9619, 9619, 9619, 9619},  75,  18, 2,  70, 80, TRUE,
	100,{1,2,2,0,0,5},CR_ALL
    },

    {
	{ L"thief", L"hırsız"}, L"Hır",  STAT_DEX,  OBJ_VNUM_SCHOOL_DAGGER,
	{ 3028, 9639, 9639, 9639, 9639, 9639 },  75,  18,  -4, 100, 50, FALSE,
        0,{0,0,0,3,0,5},CR_ALL
    },

    {
	{ L"warrior", L"savaşçı"}, L"Sav",  STAT_STR,  OBJ_VNUM_SCHOOL_SWORD,
	{ 3022, 9633, 9633, 9633, 9633, 9633 },  75,  18,  -10, 100,50,FALSE,
	0,{3,0,0,0,1,5},CR_ALL
    },

    {
	{ L"paladin", L"adbolar"}, L"Adb",   STAT_STR,  OBJ_VNUM_SCHOOL_SWORD,
	{ 3063, 0, 0, 0, 0, 0 },  75,  18,  -6,  80, 70, TRUE,
	400,{2,0,1,0,2,5},CR_GOOD
    },

    {
	{ L"anti-paladin", L"kembolar"}, L"Kmb",  STAT_STR,  OBJ_VNUM_SCHOOL_SWORD,
	{ 3061, 0, 0, 0, 0, 0 },  75,  18,  -6,  80, 70, TRUE,
	300,{2,1,0,1,1,5},CR_EVIL
    },

    {
	{ L"ninja", L"ninja"}, L"Nin",  STAT_DEX,  OBJ_VNUM_SCHOOL_DAGGER,
	{ 3062, 0, 0, 0, 0, 0 },  75,  18,  -10,  90, 60, FALSE,
	300,{1,0,0,2,1,5},CR_ALL
    },

    {
	{ L"ranger", L"korucu"}, L"Krc",  STAT_STR,  OBJ_VNUM_SCHOOL_SWORD,
	{ 6156, 0, 0, 0, 0, 0 },  75,  18,  -8,  90, 60, FALSE,
	200,{1,0,0,1,1,5},CR_ALL
    },

    {
        { L"transmuter", L"biçimci"}, L"Biç",  STAT_INT,  OBJ_VNUM_SCHOOL_DAGGER,
        { 3073, 0, 0, 0, 0, 0 },  75,  18, 2,  60,  100, TRUE,
        0,{0,3,0,0,0,5},CR_ALL
    },

    {
        { L"samurai", L"samuray"}, L"Sam",  STAT_STR,  OBJ_VNUM_SCHOOL_SWORD,
        { 3075, 9633, 0, 0, 0, 0 },  75,  18,  -10,  100,60,FALSE,
        500,{1,1,1,0,1,5},CR_ALL
    },

   {
      { L"vampire", L"vampir"}, L"Vam", STAT_INT,  OBJ_VNUM_SCHOOL_DAGGER,
        { 5395, 5396, 0, 0, 0, 0 },  75,  18, -6,  80,  70, TRUE,
        300,{1,2,0,0,0,5},CR_EVIL
        },

   {
        { L"necromancer", L"karakam"}, L"Kam",  STAT_INT,  OBJ_VNUM_SCHOOL_DAGGER,
        { 5393, 5394, 0, 0, 0, 0 },  75,  18, 6,  60,  100, TRUE,
        200,{0,2,1,0,0,5},CR_EVIL
   },

   {
        { L"elementalist", L"ögeci"}, L"Öge",  STAT_INT,  OBJ_VNUM_SCHOOL_DAGGER,
        { 9618, 0, 0, 0, 0, 0 },  75,  18, 6,  60,  100, TRUE,
        0,{0,3,0,0,0,5},CR_ALL
   }


};



/*
 * Titles. mage=100
 */
 const wchar_t *			title_table	[MAX_CLASS][MAX_LEVEL+1]=
 {
     {
       L"Man",

       L"Büyü Çırağı"		,
       L"Büyü Öğrencisi"	,
        L"Büyü Asistanı"	,
        L"Büyü Araştıran"		,
        L"Sihirbaz"	,

        L"Büyü Katibi"	,
        L"Falcı"		,
        L"Arif"		,
        L"İlüzyonist"		,
        L"Tövbekar"		,

       L"Duacı"		,
        L"Duacı"		,
        L"Büyü Yapıcı"		,
        L"Büyü Yapıcı"		,
        L"Hokkabaz"		,

        L"Hokkabaz"		,
        L"Büyücü"		,
        L"Büyücü"		,
        L"Oluşturan"     ,
        L"Oluşturan"       ,

        L"Alim"		,
        L"Alim"		,
        L"Büyü Ustası"			,
        L"Büyü Ustası"			,
        L"Büyü Alimi"		,

        L"Büyü Alimi"		,
        L"Savaşçı Arif"		,
        L"Savaşçı Arif"		,
        L"Durendiş"	,
        L"Durendiş"	,

        L"Kıdemli Büyücü"		,
        L"Kıdemli Büyücü"		,
        L"Büyük Büyücü"		,
        L"Büyük Büyücü"	,
        L"Yüce Büyücü"		,

        L"Yüce Büyücü"	,
        L"Golem Efendisi"		,
        L"Golem Efendisi"	,
        L"Yüce Golem Efendisi"		,
        L"Yüce Golem Efendisi"	,

 	 L"Taş Efendisi"	,
 	 L"Taş Efendisi"	,
 	 L"İksir Ustası"	,
 	 L"İksir Ustası"	,
 	 L"Parşömen Ustası"	,

 	 L"Parşömen Ustası"	,
 	 L"Sopa Ustası"	,
 	 L"Sopa Ustası"		,
 	 L"Değnek Ustası"	,
 	 L"Değnek Ustası"		,

 	 L"İblis Çağıran"	,
 	 L"İblis Çağıran"		,
 	 L"Yüce İblis Çağıran"	,
 	 L"Yüce İblis Çağıran",
 	 L"Ejderha Efendisi"		,

 	 L"Ejderha Efendisi"		,
 	 L"Ejderha Efendisi"	,
 	 L"Ejderha Efendisi",
        L"Karışım Efendisi",
        L"Karışım Efendisi",

        L"Büyülerin Efendisi",
        L"Büyülerin Efendisi",
        L"Büyüleme Efendisi",
        L"Büyüleme Efendisi",
        L"Çağrı Ustası",

        L"Çağrı Ustası",
        L"Büyük Kahin",
        L"Büyük Kahin",
        L"Değişim Ustası",
        L"Değişim Ustası",

        L"Fişek Efendisi",
        L"Fişek Efendisi",
        L"Ölülerin Efendisi",
        L"Ölülerin Efendisi",
        L"Simyacı",

        L"Simyacı",
        L"Bağ Ustası",
        L"Bağ Ustası",
        L"Söz Efendisi",
        L"Söz Efendisi",

        L"Geçmişe hükmeden",
        L"Geçmişe hükmeden",
        L"Bugüne hükmeden",
        L"Bugüne hükmeden",
        L"Geleceğe hükmeden",

        L"Geleceğe hükmeden",
        L"Baş Büyücü",
        L"Baş Büyücü",
        L"Bilinmeyen",
        L"Bilinmeyen",

 	 L"Kahraman"		,
 	 L"Melek"		,
 	 L"Melek"		,
 	 L"Elçi"		,
 	 L"Ölümsüz"		,
 	L"Yüce"		,
 	L"Yüce",
 	L"Yüce",
 	L"Yüce",
 	 L"Tek"	},

     {
 	  L"Man"			 ,

 	L"İnanan"		 ,
 	L"Yardımcı"		 ,
 	L"Hizmetli"		 ,
 	L"Çırak"			 ,
 	L"Misyoner"		 ,

 	L"Usta"		 ,
 	L"Diyakoz"		 ,
 	L"Önder"			 ,
 	L"Üstat"		 ,
 	L"Orta Elçi"			 ,

 	L"Dindar"			 ,
 	L"Dindar"			 ,
 	L"Yükselen"		 ,
 	L"Yükselen"			 ,
 	L"Din Adamı"		 ,

 	L"Din Adamı"			 ,
 	L"Keşiş"			 ,
 	L"Keşiş"			 ,
 	L"İyileştiren"			 ,
 	L"İyileştiren"		 ,

 	L"Vaiz"		 ,
 	L"Vaiz"		 ,
 	L"Açıklayan"		 ,
 	L"Açıklayan"			 ,
 	L"Duyan"		 ,

 	L"Duyan"		 ,
 	L"Gören"	 ,
 	L"Gören" ,
 	L"Bilen"		 ,
 	L"Bilen"		 ,

 	L"Bilge"	 ,
 	L"Bilge"		 ,
 	L"Arif"		 ,
 	L"Arif"		 ,
 	L"Arif"	 ,

 	L"Arif"		 ,
 	L"İblis Katili"		 ,
 	L"İblis Katili"			 ,
 	L"Yüce İblis Katili"		 ,
 	L"Yüce İblis Katili"		 ,

 	L"Denizlerin Efendisi"		 ,
 	L"Denizlerin Efendisi"		 ,
 	L"Toprağın Efendisi" ,
 	L"Toprağın Efendisi"		 ,
 	L"Havanın Efendisi"		 ,

 	L"Havanın Efendisi"		 ,
 	L"Lokman Ruhu"	 ,
 	L"Lokman Ruhu"	 ,
 	L"Cennet Bekçisi"	 ,
 	L"Cennet Bekçisi"	 ,

 	L"Işıyan" ,
 	L"Işıyan" ,
 	L"Kutsal" ,
 	L"Kutsal" ,
 	L"İlahi" ,

 	L"İlahi" ,
 	L"Aziz" ,
 	L"Aziz" ,
 	L"Köklerin Efendisi" ,
 	L"Köklerin Efendisi" ,

 	L"Suyun Efendisi" ,
 	L"Suyun Efendisi" ,
 	L"Ateşin Efendisi" ,
 	L"Ateşin Efendisi" ,
 	L"Buzun Efendisi" ,

 	L"Buzun Efendisi" ,
 	L"Rüzgarın Efendisi" ,
 	L"Rüzgarın Efendisi" ,
 	L"Fırtına Efendisi" ,
 	L"Fırtına Efendisi" ,

 	L"Toprağa Hükmeden" ,
 	L"Toprağa Hükmeden" ,
 	L"Denize Hükmeden" ,
 	L"Denize Hükmeden" ,
 	L"Zamana Hükmeden" ,

 	L"Zamana Hükmeden" ,
 	L"Kutsal Ruh" ,
 	L"Kutsal Ruh" ,
 	L"Kutsal Bilge" ,
 	L"Kutsal Bilge" ,

 	L"Ölümsüz Elçisi"		 ,
 	L"Ölümsüz Elçisi"	 ,
 	L"Ölümsüz Elçisi"	 ,
 	L"Ölümsüz Elçisi"	 ,
 	L"Ölümsüz Elçisi"	 ,

 	L"Ölümsüz Elçisi"	 ,
 	L"Ölümsüz Elçisi"	 ,
 	L"Ölümsüz Elçisi"	 ,
 	L"Kutsanmış Olan" ,
 	L"Kutsanmış Olan" ,

 	L"Kahraman"		,
 	 L"Melek"		,
 	 L"Melek"		,
 	 L"Elçi"		,
 	 L"Ölümsüz"		,
 	L"Yüce"		,
 	L"Yüce",
 	L"Yüce",
 	L"Yüce",
 	 L"Tek"},

     {
 	  L"Man"			 ,

 		  L"Çocuk"			 ,
 		  L"Sessiz Adım"		 ,
 		  L"Aşıran"			 ,
 		  L"Yan Kesici"		 ,
 		  L"Süzülen"		 ,

 		 L"Kıstıran"	 ,
 		  L"Cepçi"	 ,
 		  L"Kapkaççı"	 ,
 		  L"Dolandırıcı"	 ,
 		  L"Haydut"		 ,

 		 L"Soyguncu" ,
 		  L"Soyguncu" ,
 		  L"Hilebaz"		 ,
 		  L"Hilebaz"	 ,
 		  L"Eşkiya"		 ,

 		 L"Eşkiya"	 ,
 		  L"Dessas"	 ,
 		  L"Dessas"	 ,
 		  L"Hırsız"	 ,
 		  L"Hırsız"	 ,

 		  L"Keskin Bıçak" ,
 		  L"Keskin Bıçak" ,
 		  L"Hızlı Bıçak"	 ,
 		  L"Hızlı Bıçak"	 ,
 		  L"Katil"		 ,

 		  L"Katil"			 ,
 		  L"Kanunsuz"		 ,
 		  L"Kanunsuz"		 ,
 		  L"Gırtlak Kesen"			 ,
 		  L"Gırtlak Kesen"			 ,

 		  L"İftiracı"	 ,
 		  L"İftiracı"	 ,
 		  L"Muhbir"	 ,
 		  L"Muhbir"	 ,
 		  L"Hafiye"	 ,

 		  L"Hafiye"	 ,
 		  L"Suikastçi" ,
 		  L"Suikastçi" ,
 		  L"Büyük Suikastçi" ,
 		  L"Büyük Suikastçi" ,

 		 L"Görüş Ustası"	 ,
 		  L"Görüş Ustası"	 ,
 		  L"Duyuş Ustası" ,
 		  L"Duyuş Ustası" ,
 		  L"Koku Ustası"	 ,

    		 L"Koku Ustası"	 ,
 		  L"Tat Ustası"	,
 		  L"Tat Ustası"	 ,
 		  L"Temas Ustası"	 ,
 		  L"Temas Ustası"	 ,

 		  L"Suçlu"		 ,
 		  L"Suçlu"	 ,
 		  L"Suç Ustası"	 ,
 		  L"Suç Ustası" ,
 		  L"Suç Üstadı",

 		  L"Suç Üstadı"	 ,
 		  L"Suç Efendisi"	 ,
 		  L"Suç Efendisi"		 ,
 		  L"Patron"			 ,
 		  L"Patron"		 ,

 		  L"Elebaşı" ,
 		  L"Elebaşı" ,
 		  L"Büyük Patron" ,
 		  L"Büyük Patron" ,
 		  L"Yeraltının Gölgesi" ,

 		  L"Yeraltının Gölgesi" ,
 		  L"Gölge" ,
 		  L"Gölge" ,
 		  L"Karanlığın Sesi" ,
 		  L"Karanlığın Sesi" ,

 		  L"Sessizliğin Efendisi" ,
 		  L"Sessizliğin Efendisi" ,
 		  L"Hile Efendisi" ,
 		  L"Hile Efendisi" ,
 		  L"Bıçak Efendisi" ,

 		L"Bıçak Efendisi" ,
 		L"Zehir Efendisi" ,
 		L"Zehir Efendisi" ,
 		L"Giz Efendisi" ,
 		L"Giz Efendisi" ,

 	        L"Hırsız Efendi" ,
 		L"Hırsız Efendi" ,
 		L"Suikastçi Efendi" ,
 		L"Suikastçi Efendi" ,
 		L"Görünmeyen" ,

 		L"Görünmeyen" ,
 		L"Hırsızların Efendisi" ,
 		L"Hırsızların Efendisi" ,
 		L"Suikastçilerin Efendisi" ,
 		L"Suikastçilerin Efendisi" ,

 		L"Kahraman"		,
 	 L"Melek"		,
 	 L"Melek"		,
 	 L"Elçi"		,
 	 L"Ölümsüz"		,
 	L"Yüce"		,
 	L"Yüce",
 	L"Yüce",
 	L"Yüce",
 	 L"Tek" },

     {
           L"Man"			 ,

 	  L"Acemi"		 ,
 	  L"Çaylak"		 ,
 	  L"Nöbetçi"		 ,
 	  L"Dövüşçü"		 ,
 	  L"Asker"			 ,

 	  L"Savaşan"		 ,
 	  L"Kıdemli"			 ,
 	  L"Kılıç Çeken"		 ,
 	  L"Eskrimci"	 ,
 	  L"Savaşçı" ,

         L"Kahraman"		 ,
 	  L"Kahraman"		 ,
 	  L"Şanlı"		 ,
 	  L"Şanlı"		 ,
 	  L"Gri Kalkan" ,

         L"Gri Kalkan"				 ,
 	  L"Paralı Asker"					 ,
 	  L"Paralı Asker"						 ,
 	  L"Kılıç Ustası"				 ,
 	  L"Kılıç Ustası"				 ,

 	  L"Teğmen"						 ,
 	  L"Teğmen"					 ,
 	  L"Savunan"				 ,
 	  L"Savunan"					 ,
 	  L"Dragon"					 ,

 	  L"Dragon"					 ,
 	  L"Saygısız"						 ,
 	  L"Saygısız"						 ,
 	  L"Şövalye"					 ,
 	  L"Şövalye"				 ,

 	  L"Büyük Şövalye"				 ,
 	  L"Büyük Şövalye"				 ,
 	  L"Efendi Şövalye"					 ,
 	  L"Efendi Şövalye"				 ,
 	  L"Gladyatör"					 ,

 	  L"Gladyatör"				 ,
 	  L"Efendi Gladyatör"				 ,
 	  L"Efendi Gladyatör"			 ,
 	  L"İblis Katleden"					 ,
 	  L"İblis Katleden"				 ,

 	  L"Büyük İblis Katleden"		 ,
 	  L"Büyük İblis Katleden"			 ,
 	  L"Ejder Katili"				 ,
 	  L"Ejder Katili"				 ,
 	  L"Büyük Ejder Katili" ,

         L"Büyük Ejder Katili"		 ,
 	  L"Lord"						 ,
 	  L"Lord"				 ,
 	  L"Efendi"						 ,
 	  L"Efendi"						 ,

 	  L"Baron"			 ,
 	  L"Baron"		 ,
 	  L"Rüzgar Baron"			 ,
 	  L"Rüzgar Baron"				 ,
 	  L"Fırtına Baron"			 ,

 	  L"Fırtına Baron"			 ,
 	  L"Kasırga Baron"		 ,
 	  L"Kasırga Baron"		 ,
 	  L"Kudretli Baron"	 ,
 	  L"Kudretli Baron"	 ,

 	  L"Ateş Baron"   ,
 	  L"Ateş Baron"   ,
 	  L"Buz Baron"   ,
 	  L"Buz Baron"   ,
 	  L"Şimşek Baron"  ,

 	  L"Şimşek Baron"  ,
 	  L"Elementlerin Efendisi"   ,
 	  L"Elementlerin Efendisi"   ,
 	  L"Bakır Efendi"   ,
 	  L"Bakır Efendi"  ,

 	  L"Bronz Efendi"  ,
 	  L"Bronz Efendi"   ,
 	  L"Pirinç Efendi"  ,
 	  L"Pirinç Efendi"   ,
 	  L"Demir Efendi"   ,

 	  L"Demir Efendi"   ,
 	  L"Çelik Efendi"  ,
 	  L"Çelik Efendi"   ,
 	  L"Mitril Efendi"   ,
 	  L"Mitril Efendi"  ,

 	  L"Adamantit Efendi"   ,
 	  L"Adamantit Efendi"  ,
 	  L"Yüzbaşı"   ,
 	  L"Yüzbaşı"  ,
 	  L"General"   ,

 	  L"General"  ,
 	  L"Kara Mareşali"  ,
 	  L"Kara Mareşali"  ,
 	  L"Savaş Ustası"   ,
 	  L"Savaş Ustası"  ,

 	  L"Kahraman"		,
 	 L"Melek"		,
 	 L"Melek"		,
 	 L"Elçi"		,
 	 L"Ölümsüz"		,
 	L"Yüce"		,
 	L"Yüce",
 	L"Yüce",
 	L"Yüce",
 	 L"Tek"     },

       {

         L"Man"                ,

         L"Paladin Yamağı"		  ,
         L"Paladin Çırağı"			 ,
         L"Kalkan"		   ,
         L"Kalkan"		 ,
         L"Kılıç"			 ,

         L"Yay"			 ,
         L"Taşıyıcı"		  ,
         L"Atlı"			 ,
         L"Bey Adayı"		 ,
         L"Bey"		 ,

         L"Piyade Neferi"   ,
         L"Piyade Neferi"  ,
         L"Mızrak Taşıyan" ,
         L"Mızrak Taşıyan" ,
         L"Yay Taşıyan"  ,

         L"Yay Taşıyan"   ,
         L"Kılıç Taşıyan"   ,
         L"Kılıç Taşıyan"  ,
         L"Onurlu"   ,
         L"Onurlu"   ,

         L"Soylu"   ,
         L"Soylu"  ,
         L"Güvenilir"   ,
         L"Güvenilir"   ,
         L"Doğrucu"   ,

         L"Doğrucu"  ,
         L"Yiğit"  ,
         L"Yiğit"  ,
         L"Paladin"   ,
         L"Paladin"   ,

         L"Görevci"  ,
         L"Görevci"  ,
         L"Saygın"   ,
         L"Saygın"  ,
         L"Savunan" ,

         L"Savunan"  ,
         L"Şanlı Şövalye"  ,
         L"Şanlı Şövalye"   ,
         L"Paladin Şövalye"  ,
         L"Paladin Şövalye"   ,

         L"Arna Adayı"  ,
         L"Arna Adayı"   ,
         L"Dindar Şövalye"   ,
         L"Dindar Şövalye"  ,
         L"Dindar Şövalye"  ,

         L"Dindar Şövalye"  ,
         L"Dindar Savunma"  ,
         L"Dindar Savunma"   ,
         L"Şövalye Arna"  ,
         L"Şövalye Arna"   ,

         L"Arna Çırağı"  ,
         L"Arna Çırağı"   ,
         L"Arna"   ,
         L"Arna"  ,
         L"Arna Teğmen"  ,

         L"Arna Teğmen"   ,
         L"Arna Yüzbaşı"  ,
         L"Arna Yüzbaşı"   ,
         L"Arna Albay"   ,
         L"Arna Albay"   ,

         L"Arna General"  ,
         L"Arna General"  ,
         L"Arna Kara Mareşali"   ,
         L"Arna Kara Mareşali"   ,
         L"Şifacı Şövalye"  ,

         L"Şifacı Şövalye"   ,
         L"Öc Alan"   ,
         L"Öc Alan"  ,
         L"Öncü"   ,
         L"Öncü"   ,

         L"Öncü Lideri"  ,
         L"Öncü Lideri"   ,
         L"Öncülerin Lideri"  ,
         L"Öncülerin Lideri"  ,
         L"Arna Lideri"   ,

         L"Arna Lideri"  ,
         L"Arnaların Lideri"   ,
         L"Arnaların Lideri"   ,
         L"Arnaların Şanlı Lideri"   ,
         L"Arnaların Şanlı Lideri" ,

         L"Arnaların Lordu"  ,
         L"Arnaların Lordu"  ,
         L"İnancın Çekici"   ,
         L"İnancın Çekici"  ,
         L"Kafirlerin Katili"   ,

         L"Kafirlerin Katili"  ,
         L"Dindar"   ,
         L"Dindar"   ,
         L"Kutsal Şövalye"  ,
         L"Kutsal Şövalye"   ,

         L"Kahraman"		,
 	 L"Melek"		,
 	 L"Melek"		,
 	 L"Elçi"		,
 	 L"Ölümsüz"		,
 	L"Yüce"		,
 	L"Yüce",
 	L"Yüce",
 	L"Yüce",
 	 L"Tek"
      },
       {
         L"Man"						 ,

         L"Kirli"						 ,
         L"Zorba"			 	 ,
         L"Kaba"   ,
         L"Cani"   ,
         L"Cani"  ,

         L"Yağmacı"   ,
         L"Yokeden"  ,
         L"Yakan"   ,
         L"Kiralık Katil"   ,
         L"Kanunsuz"  				 ,

         L"Paralı Asker"   ,
         L"Paralı Asker"   ,
         L"Kara Kılıç"  ,
         L"Kara Kılıç"   ,
         L"Kızıl Kılıç"  ,

         L"Kızıl Kılıç"   ,
         L"Süzülen" ,
         L"Süzülen"  ,
         L"Gaddar"   ,
         L"Gaddar" ,

         L"Araklayan"   ,
         L"Araklayan"   ,
         L"Bilinmeyen"  ,
         L"Bilinmeyen"   ,
         L"Nefret Edilen"  ,

         L"Nefret Edilen"   ,
         L"Alçak"   ,
         L"Alçak"  ,
         L"Paladin Düşmanı"   ,
         L"Paladin Düşmanı" ,

         L"Şeytani Dövüşçü"  ,
         L"Şeytani Dövüşçü"  ,
         L"Haydut Şövalye"  ,
         L"Haydut Şövalye"  ,
         L"Şeytani Savunucu"   ,

         L"Şeytani Savunucu"  ,
         L"Kanlı"  ,
         L"Kanlı"   ,
         L"Kara Şövalye"   ,
         L"Kara Şövalye"   ,

         L"Kızıl Şövalye"  ,
         L"Kızıl Şövalye"  ,
         L"Büyük Şövalye"  ,
         L"Büyük Şövalye"   ,
         L"İnançsız Şövalye"   ,

         L"İnançsız Şövalye"   ,
         L"Acının Şövalyesi"   ,
         L"Acının Şövalyesi"  ,
         L"Lejyon Şövalyesi"   ,
         L"Lejyon Şövalyesi"   ,

         L"Lejyon Şövalyesi"   ,
         L"Lejyon Şövalyesi"  ,
         L"Lejyon Savunması"  ,
         L"Lejyon Savunması"  ,
         L"Lejyon Yüzbaşısı"   ,

         L"Lejyon Yüzbaşısı"   ,
         L"Lejyon Generali"   ,
         L"Lejyon Generali"   ,
         L"Lejyon Kara Mareşali" ,
         L"Lejyon Kara Mareşali"   ,

         L"Kıyamet Şövalyesi"  ,
         L"Kıyamet Şövalyesi" ,
         L"Işığı kesen"   ,
         L"Işığı Kesen"   ,
         L"Acının Efendisi"   ,

         L"Acının Efendisi"   ,
         L"Ölü Efendisi"   ,
         L"Ölü Efendisi"   ,
         L"Karanlık Şövalye"  ,
         L"Karanlık Şövalye"   ,

         L"Karanlık Lord"   ,
         L"Karanlık Lord"   ,
         L"Karanlık Kral"   ,
         L"Karanlık Kral"   ,
         L"Ölüm Habercisi"   ,

         L"Ölüm Habercisi"   ,
         L"Ölüm Yayan"   ,
         L"Ölüm Yayan"   ,
         L"Kıtlık Getiren"   ,
         L"Kıtlık Getiren"   ,

         L"Ölüm Getiren"   ,
         L"Ölüm Getiren"   ,
         L"Onursuz İnançsız"  ,
         L"Onursuz İnançsız"  ,
         L"Katleden"   ,

         L"Katleden"   ,
         L"Karanlık Tohum"   ,
         L"Karanlık Tohum"  ,
         L"Onursuz Şövalye"   ,
         L"Onursuz Şövalye"  ,

         L"Kahraman"		,
 	 L"Melek"		,
 	 L"Melek"		,
 	 L"Elçi"		,
 	 L"Ölümsüz"		,
 	L"Yüce"		,
 	L"Yüce",
 	L"Yüce",
 	L"Yüce",
 	 L"Tek"	 },

       {
         L"Man"  ,

         L"Uke"   ,
         L"Tori"   ,
         L"Beyaz Kuşak"  ,
         L"Sarı Kuşak"   ,
         L"Yeşil Kuşak"   ,

         L"Kahverengi Üç"   ,
         L"Kahverengi İki" ,
         L"Kahverengi Bir"  ,
         L"Siyah Üç"   ,
         L"Siyah İki"   ,

         L"Okuiri"   ,
         L"Okuiri"   ,
         L"Shuto"   ,
         L"Shuto"  ,
         L"Uraken"   ,

         L"Uraken"   ,
         L"Shotei"   ,
         L"Shotei"   ,
         L"Tateken"   ,
         L"Tateken"   ,

         L"Seiken"   ,
         L"Seiken"   ,
         L"Ude"   ,
         L"Ude"  ,
         L"Empi"  ,

         L"Empi"   ,
         L"Josokutei"   ,
         L"Josokutei"   ,
         L"Kasokutei"   ,
         L"Kasokutei"  ,

         L"Atemi Waza Ustası"   ,
         L"Atemi Waza Ustası"  ,
         L"Kotegaeshi Ustası"   ,
         L"Kotegaeshi Ustası"  ,
         L"Kansetsuwaza Ustası"   ,

         L"Kansetsuwaza Ustası"  ,
         L"Taisabaki Ustası"   ,
         L"Taisabaki Ustası"  ,
         L"Kyusho Ustası"   ,
         L"Kyusho Ustası"  ,

         L"Kamae Öğrencisi"  ,
         L"Kamae Öğrencisi"   ,
         L"Kamae Ustası"  ,
         L"Kamae Ustası"  ,
         L"Ukemi Ustası"   ,

         L"Ukemi Ustası"  ,
         L"Mokuroku"   ,
         L"Mokuroku"  ,
         L"Ogoshi"  ,
         L"Ogoshi"  ,

         L"Ippon Seinage"   ,
         L"Ippon Seinage"  ,
         L"Koshi Garuma"   ,
         L"Koshi Garuma"   ,
         L"Sukuinage"   ,

         L"Sukuinage"  ,
         L"Osotogari"  ,
         L"Osotogari"   ,
         L"Uki Goshi" ,
         L"Uki Goshi"  ,

         L"Taiotoshi"   ,
         L"Taiotoshi"   ,
         L"Harai Goshi"   ,
         L"Harai Goshi"   ,
         L"Yama Arashi"   ,

         L"Yama Arashi"   ,
         L"Nage Waza Ustası"   ,
         L"Nage Waza Ustası"   ,
         L"Owazaya Giriş"  ,
         L"Owazaya Giriş"   ,

         L"Owaza Çırağı" ,
         L"Owaza Çırağı"   ,
         L"Owaza Öğrencisi"  ,
         L"Owaza Öğrencisi"   ,
         L"Owaza"   ,

         L"Owaza"   ,
         L"Owaza Ustası"  ,
         L"Owaza Ustası"  ,
         L"Menkyo"  ,
         L"Menkyo"   ,

         L"Sensei"   ,
         L"Sensei"  ,
         L"Shinan"   ,
         L"Shinan"   ,
         L"Shihan"   ,

         L"Shihan"   ,
         L"Kaiden"   ,
         L"Kaiden"   ,
         L"Miyama Ryu Ustası"  ,
         L"Miyama Ryu Ustası" ,

         L"Kahraman"		,
 	 L"Melek"		,
 	 L"Melek"		,
 	 L"Elçi"		,
 	 L"Ölümsüz"		,
 	L"Yüce"		,
 	L"Yüce",
 	L"Yüce",
 	L"Yüce",
 	 L"Tek"
      },
      {

         L"Man"	 ,

         L"Yavru Kurt"   ,
         L"Yavru Kurt"   ,
         L"İzci"   ,
         L"İzci"   ,
         L"Korucu"  ,

         L"Korucu"   ,
         L"Tuzakçı"  ,
         L"Tuzakçı"  ,
         L"Avcı"  ,
         L"Avcı"			 			 ,

         L"İz Sürücü"   ,
         L"İz Sürücü"  ,
         L"Öncü" ,
         L"Öncü"  ,
         L"Yeşil Adam"   ,

         L"Yeşil Adam"  ,
         L"Okçu"   ,
         L"Okçu"   ,
         L"Ağaç Adam"   ,
         L"Ağaç Adam"  ,

         L"Ağaç Ustası"  ,
         L"Ağaç Ustası"  ,
         L"Kolcu Çırak"  ,
         L"Kolcu Çırak"  ,
         L"Kolcu Adayı"   ,

         L"Kolcu Adayı"   ,
         L"Kolcu Kalfası"   ,
         L"Kolcu Kalfası"  ,
         L"Kolcu"   ,
         L"Kolcu"   ,

         L"Kolcu"  ,
         L"Kolcu"  ,
         L"Kolcu"  ,
         L"Kolcu"   ,
         L"Kolcu"   ,

         L"Kolcu"  ,
         L"Kolcu Yüzbaşı"  ,
         L"Kolcu Yüzbaşı"   ,
         L"Büyük Korucu"  ,
         L"Büyük Korucu"  ,

         L"Korucu Yüzbaşı" ,
         L"Korucu Yüzbaşı"   ,
         L"Korucu General"   ,
         L"Korucu General"   ,
         L"Korucuların Efendisi"   ,

         L"Korucuların Efendisi"   ,
         L"Ormanın Şövalyesi"   ,
         L"Ormanın Şövalyesi" ,
         L"Ormanın Kılıcı"  ,
         L"Ormanın Kılıcı"   ,

         L"Orman Gardı"   ,
         L"Orman Gardı"  ,
         L"Orman Lordu"   ,
         L"Orman Lordu"  ,
         L"Orman Lordu"   ,

         L"Orman Lordu"   ,
         L"Orman Baronu"   ,
         L"Orman Baronu"  ,
         L"Ormanların Baronu"  ,
         L"Ormanların Baronu"  ,

         L"Avlayan"  ,
         L"Avlayan"  ,
 	L"Sakınan" ,
 	L"Sakınan" ,
 	L"Koruyan" ,

 	L"Koruyan",
         L"Kolcu Şövalye"   ,
         L"Kolcu Şövalye" ,
         L"Kolcu Şövalye"   ,
         L"Kolcu Şövalye"  ,

         L"Kolcu Lord"   ,
         L"Kolcu Lord"  ,
         L"Kolcu Lord"  ,
         L"Kolcu Lord"  ,
         L"Kolcu Baron"   ,

         L"Kolcu Baron"  ,
         L"Kolcu Baron"   ,
         L"Kolcu Baron"   ,
 	L"Gezgin" ,
 	L"Gezgin" ,

         L"Selvan",
 	L"Selvan" ,
         L"Ormanların Sesi"  ,
         L"Ormanların Sesi"   ,
 	L"Ormanların Sesi"   ,

         L"İzleyen"   ,
         L"İzleyen"   ,
 	L"İzleyen"
 	L"İzleyen"
 	L"İzleyen"

         L"Kahraman"		,
 	 L"Melek"		,
 	 L"Melek"		,
 	 L"Elçi"		,
 	 L"Ölümsüz"		,
 	L"Yüce"		,
 	L"Yüce",
 	L"Yüce",
 	L"Yüce",
 	 L"Tek"
      },
       {
         L"Man" ,

         L"Büyü Çırağı"		 ,
         L"Büyü Öğrencisi"	 ,
         L"Büyü Asistanı"	 ,
         L"Büyü Araştıran"		 ,
         L"Sihirbaz"	 ,

         L"Büyü Katibi"	 ,
         L"Falcı"		 ,
         L"Arif"		 ,
         L"İlüzyonist"		 ,
         L"Tövbekar"		 ,

         L"Duacı"		 ,
         L"Duacı"		 ,
         L"Büyü Yapan"		 ,
         L"Büyü Yapan"		 ,
         L"Hokkabaz"		 ,

         L"Hokkabaz" ,
         L"Büyücü"	 ,
         L"Büyücü"	 ,
 	L"Oluşturan" ,
 	L"Oluşturan" ,

         L"Alim"		 ,
         L"Alim"		 ,
         L"Büyü Ustası"			 ,
         L"Büyü Ustası"			 ,
         L"Büyü Alimi"		 ,

         L"Büyü Alimi"		 ,
         L"Savaşçı Arif"		 ,
         L"Savaşçı Arif"		 ,
         L"Durendiş"	 ,
         L"Durendiş"	 ,

         L"Kıdemli Büyücü"		 ,
         L"Kıdemli Büyücü"		 ,
         L"Büyük Büyücü"		 ,
         L"Büyük Büyücü"	 ,
         L"Yüce Büyücü"		 ,

         L"Yüce Büyücü"	 ,
         L"Golem Efendisi"		 ,
         L"Golem Efendisi"	 ,
         L"Yüce Golem Efendisi"		 ,
         L"Yüce Golem Efendisi"	 ,

 	  L"Taş Efendisi"	 ,
 	  L"Taş Efendisi"	 ,
 	  L"İksir Ustası"	 ,
 	  L"İksir Ustası"	 ,
 	  L"Parşömen Ustası"	 ,

 	  L"Parşömen Ustası"	 ,
 	  L"Değnek Ustası"	 ,
 	  L"Değnek Ustası"		 ,
 	  L"Değnek Ustası"	 ,
 	  L"Değnek Ustası" ,

         L"İblis Çağıran"	 ,
 	  L"İblis Çağıran"		 ,
 	  L"Yüce İblis Çağıran"	 ,
 	  L"Yüce İblis Çağıran" ,
 	  L"Ejderha Efendisi" ,

         L"Ejderha Efendisi"		 ,
 	  L"Ejderha Efendisi"	 ,
 	  L"Ejderha Efendisi" ,
         L"Karışım Efendisi" ,
         L"Karışım Efendisi" ,

         L"Büyülerin Efendisi" ,
         L"Büyülerin Efendisi" ,
         L"Büyüleme Efendisi" ,
         L"Büyüleme Efendisi" ,
         L"Çağrı Ustası" ,

         L"Çağrı Ustası" ,
         L"Büyük Kahin" ,
         L"Büyük Kahin" ,
         L"Değişim Ustası" ,
         L"Değişim Ustası" ,

         L"Fişek Efendisi" ,
         L"Fişek Efendisi" ,
         L"Ölülerin Efendisi" ,
         L"Ölülerin Efendisi" ,
         L"Simyacı" ,

         L"Simyacı" ,
         L"Bağ Ustası" ,
         L"Bağ Ustası" ,
         L"Söz Efendisi" ,
         L"Söz Efendisi" ,

         L"Geçmişe hükmeden" ,
         L"Geçmişe hükmeden" ,
         L"Bugüne hükmeden" ,
         L"Bugüne hükmeden" ,
         L"Geleceğe hükmeden" ,

         L"Geleceğe hükmeden" ,
         L"Baş Büyücü" ,
         L"Baş Büyücü" ,
         L"Bilinmeyen" ,
         L"Bilinmeyen" ,

 	  L"Kahraman"		,
 	 L"Melek"		,
 	 L"Melek"		,
 	 L"Elçi"		,
 	 L"Ölümsüz"		,
 	L"Yüce"		,
 	L"Yüce",
 	L"Yüce",
 	L"Yüce",
 	 L"Tek"
      },
      {

           L"Man" ,

         L"Acemi"	 ,
 	  L"Çaylak"	 ,
 	  L"Nöbetçi" ,
 	  L"Dövüşçü" ,
 	  L"Asker" ,

         L"Savaşan"	 ,
 	  L"Kıdemli"		 ,
 	  L"Kılıç Çeken"	 ,
 	  L"Eskrimci"	 ,
 	  L"Savaşçı" ,

         L"Kahraman"	 ,
 	  L"Kahraman"	 ,
 	  L"Şanlı"	 ,
 	  L"Şanlı"	 ,
 	  L"Gri Kalkan"	 ,

 	  L"Gri Kalkan"		 ,
 	  L"Paralı Asker"		 ,
 	  L"Paralı Asker"		 ,
 	  L"Kılıç Ustası"	 ,
 	  L"Kılıç Ustası"	 ,

 	  L"Teğmen"		 ,
 	  L"Teğmen"		 ,
 	  L"Savunan"	 ,
 	  L"Savunan"	 ,
 	  L"Dragon" ,

         L"Dragon"			 ,
 	  L"Saygısız"		 ,
 	  L"Saygısız"	 ,
 	  L"Şövalye"		 ,
 	  L"Şövalye"	 ,

 	  L"Büyük Şövalye"				 ,
 	  L"Büyük Şövalye"				 ,
 	  L"Efendi Şövalye"					 ,
 	  L"Efendi Şövalye"				 ,
 	  L"Gladyatör"					 ,

 	  L"Gladyatör"				 ,
 	  L"Efendi Gladyatör"				 ,
 	  L"Efendi Gladyatör"			 ,
 	  L"İblis Katleden"					 ,
 	  L"İblis Katleden"				 ,

 	  L"Büyük İblis Katleden"		 ,
 	  L"Büyük İblis Katleden"			 ,
 	  L"Ejder Katili"				 ,
 	  L"Ejder Katili"				 ,
 	  L"Büyük Ejder Katili" ,

         L"Büyük Ejder Katili"		 ,
 	  L"Lord"						 ,
 	  L"Lord"				 ,
 	  L"Efendi"						 ,
 	  L"Efendi"						 ,

 	  L"Baron"			 ,
 	  L"Baron"		 ,
 	  L"Rüzgar Baron"			 ,
 	  L"Rüzgar Baron"				 ,
 	  L"Fırtına Baron"			 ,

 	  L"Fırtına Baron"			 ,
 	  L"Kasırga Baron"		 ,
 	  L"Kasırga Baron"		 ,
 	  L"Kudretli Baron"  ,
 	  L"Kudretli Baron" ,

 	  L"Ateş Baron"   ,
 	  L"Ateş Baron"   ,
 	  L"Buz Baron"   ,
 	  L"Buz Baron"   ,
 	  L"Şimşek Baron"  ,

 	  L"Şimşek Baron"  ,
 	  L"Elementlerin Efendisi"   ,
 	  L"Elementlerin Efendisi"   ,
 	  L"Bakır Efendi"   ,
 	  L"Bakır Efendi"  ,

 	  L"Bronz Efendi"  ,
 	  L"Bronz Efendi"   ,
 	  L"Pirinç Efendi"  ,
 	  L"Pirinç Efendi"   ,
 	  L"Demir Efendi"   ,

 	  L"Demir Efendi"   ,
 	  L"Çelik Efendi"  ,
 	  L"Çelik Efendi"   ,
 	  L"Mitril Efendi"   ,
 	  L"Mitril Efendi"  ,

 	  L"Adamantit Efendi"   ,
 	  L"Adamantit Efendi"  ,
 	  L"Yüzbaşı"   ,
 	  L"Yüzbaşı"  ,
 	  L"General"   ,

 	  L"General"  ,
 	  L"Kara Mareşali"  ,
 	  L"Kara Mareşali"  ,
 	  L"Savaş Ustası"   ,
 	  L"Savaş Ustası"  ,

 	  L"Kahraman"		,
 	 L"Melek"		,
 	 L"Melek"		,
 	 L"Elçi"		,
 	 L"Ölümsüz"		,
 	L"Yüce"		,
 	L"Yüce",
 	L"Yüce",
 	L"Yüce",
 	 L"Tek"
      },
       {
         L"Man" ,

         L"Büyü Çırağı"		 ,
         L"Büyü Öğrencisi"	 ,
         L"Büyü Asistanı"	 ,
         L"Büyü Araştıran"		 ,
         L"Sihirbaz"	 ,

         L"Büyü Katibi"	 ,
         L"Falcı"		 ,
         L"Arif"		 ,
         L"İlüzyonist"		 ,
         L"Tövbekar"		 ,

         L"Duacı"		 ,
         L"Duacı"		 ,
         L"Büyü Yapan"		 ,
         L"Büyü Yapan"		 ,
         L"Hokkabaz"		 ,

         L"Hokkabaz"		 ,
         L"Büyücü"		 ,
         L"Büyücü"		 ,
 	L"Oluşturan"    ,
 	L"Oluşturan" ,

         L"Alim"	 ,
         L"Alim"	 ,
         L"Büyü Ustası"		 ,
         L"Büyü Ustası"		 ,
         L"Büyü Alimi"	 ,

         L"Büyü Alimi"		 ,
         L"Savaşçı Arif"		 ,
         L"Savaşçı Arif"		 ,
         L"Durendiş"	 ,
         L"Durendiş"	 ,

         L"Kıdemli Büyücü"		 ,
         L"Kıdemli Büyücü"		 ,
         L"Büyük Büyücü"		 ,
         L"Büyük Büyücü"	 ,
         L"Yüce Büyücü"		 ,

         L"Yüce Büyücü"	 ,
         L"Golem Efendisi"		 ,
         L"Golem Efendisi"	 ,
         L"Yüce Golem Efendisi"		 ,
         L"Yüce Golem Efendisi"	 ,

 	  L"Taş Efendisi"	 ,
 	  L"Taş Efendisi"	 ,
 	  L"İksir Ustası"	 ,
 	  L"İksir Ustası"	 ,
 	  L"Parşömen Ustası"	 ,

 	  L"Parşömen Ustası"	 ,
 	  L"Değnek Ustası"	 ,
 	  L"Değnek Ustası"		 ,
 	  L"Değnek Ustası"	 ,
 	  L"Değnek Ustası" ,

         L"İblis Çağıran"	 ,
 	  L"İblis Çağıran"		 ,
 	  L"Yüce İblis Çağıran"	 ,
 	  L"Yüce İblis Çağıran" ,
 	  L"Ejderha Efendisi" ,

         L"Ejderha Efendisi"		 ,
 	  L"Ejderha Efendisi"	 ,
 	  L"Ejderha Efendisi" ,
         L"Karışım Efendisi" ,
         L"Karışım Efendisi" ,

         L"Büyülerin Efendisi" ,
         L"Büyülerin Efendisi" ,
         L"Büyüleme Efendisi" ,
         L"Büyüleme Efendisi" ,
         L"Çağrı Ustası" ,

         L"Çağrı Ustası" ,
         L"Büyük Kahin" ,
         L"Büyük Kahin" ,
         L"Değişim Ustası" ,
         L"Değişim Ustası" ,

         L"Fişek Efendisi" ,
         L"Fişek Efendisi" ,
         L"Ölülerin Efendisi" ,
         L"Ölülerin Efendisi" ,
         L"Simyacı" ,

         L"Simyacı" ,
         L"Bağ Ustası" ,
         L"Bağ Ustası" ,
         L"Söz Efendisi" ,
         L"Söz Efendisi" ,

         L"Geçmişe hükmeden" ,
         L"Geçmişe hükmeden" ,
         L"Bugüne hükmeden" ,
         L"Bugüne hükmeden" ,
         L"Geleceğe hükmeden" ,

         L"Geleceğe hükmeden" ,
         L"Baş Büyücü" ,
         L"Baş Büyücü" ,
         L"Bilinmeyen" ,
         L"Bilinmeyen" ,

 	  L"Kahraman"		,
 	 L"Melek"		,
 	 L"Melek"		,
 	 L"Elçi"		,
 	 L"Ölümsüz"		,
 	L"Yüce"		,
 	L"Yüce",
 	L"Yüce",
 	L"Yüce",
 	 L"Tek"
      },
       {

         L"Man" ,

         L"Büyü Çırağı"		 ,
         L"Büyü Öğrencisi"	 ,
         L"Büyü Asistanı"	 ,
         L"Büyü Araştıran"		 ,
         L"Sihirbaz"	 ,

         L"Büyü Katibi"	 ,
         L"Falcı"		 ,
         L"Arif"		 ,
         L"İlüzyonist"		 ,
         L"Tövbekar"		 ,

         L"Duacı"		 ,
         L"Duacı"		 ,
         L"Büyü Yapan"		 ,
         L"Büyü Yapan"		 ,
         L"Hokkabaz"		 ,

         L"Hokkabaz" ,
         L"Büyücü"	 ,
         L"Büyücü"	 ,
 	L"Oluşturan" ,
 	L"Oluşturan" ,

         L"Alim"		 ,
         L"Alim"		 ,
         L"Büyü Ustası"			 ,
         L"Büyü Ustası"			 ,
         L"Büyü Alimi"		 ,

         L"Büyü Alimi"		 ,
         L"Savaşçı Arif"		 ,
         L"Savaşçı Arif"		 ,
         L"Durendiş"	 ,
         L"Durendiş"	 ,

         L"Kıdemli Büyücü"		 ,
         L"Kıdemli Büyücü"		 ,
         L"Büyük Büyücü"		 ,
         L"Büyük Büyücü"	 ,
         L"Yüce Büyücü"		 ,

         L"Yüce Büyücü"	 ,
         L"Golem Efendisi"		 ,
         L"Golem Efendisi"	 ,
         L"Yüce Golem Efendisi"		 ,
         L"Yüce Golem Efendisi"	 ,

 	  L"Taş Efendisi"	 ,
 	  L"Taş Efendisi"	 ,
 	  L"İksir Ustası"	 ,
 	  L"İksir Ustası"	 ,
 	  L"Parşömen Ustası"	 ,

 	  L"Parşömen Ustası"	 ,
 	  L"Değnek Ustası"	 ,
 	  L"Değnek Ustası"		 ,
 	  L"Değnek Ustası"	 ,
 	  L"Değnek Ustası" ,

         L"İblis Çağıran"	 ,
 	  L"İblis Çağıran"		 ,
 	  L"Yüce İblis Çağıran"	 ,
 	  L"Yüce İblis Çağıran" ,
 	  L"Ejderha Efendisi" ,

         L"Ejderha Efendisi"		 ,
 	  L"Ejderha Efendisi"	 ,
 	  L"Ejderha Efendisi" ,
         L"Karışım Efendisi" ,
         L"Karışım Efendisi" ,

         L"Büyülerin Efendisi" ,
         L"Büyülerin Efendisi" ,
         L"Büyüleme Efendisi" ,
         L"Büyüleme Efendisi" ,
         L"Çağrı Ustası" ,

         L"Çağrı Ustası" ,
         L"Büyük Kahin" ,
         L"Büyük Kahin" ,
         L"Değişim Ustası" ,
         L"Değişim Ustası" ,

         L"Fişek Efendisi" ,
         L"Fişek Efendisi" ,
         L"Ölülerin Efendisi" ,
         L"Ölülerin Efendisi" ,
         L"Simyacı" ,

         L"Simyacı" ,
         L"Bağ Ustası" ,
         L"Bağ Ustası" ,
         L"Söz Efendisi" ,
         L"Söz Efendisi" ,

         L"Geçmişe hükmeden" ,
         L"Geçmişe hükmeden" ,
         L"Bugüne hükmeden" ,
         L"Bugüne hükmeden" ,
         L"Geleceğe hükmeden" ,

         L"Geleceğe hükmeden" ,
         L"Baş Büyücü" ,
         L"Baş Büyücü" ,
         L"Bilinmeyen" ,
         L"Bilinmeyen" ,

 	  L"Kahraman"		,
 	 L"Melek"		,
 	 L"Melek"		,
 	 L"Elçi"		,
 	 L"Ölümsüz"		,
 	L"Yüce"		,
 	L"Yüce",
 	L"Yüce",
 	L"Yüce",
 	 L"Tek"
      },
      {

         L"Man" ,

         L"Büyü Çırağı"		 ,
         L"Büyü Öğrencisi"	 ,
         L"Büyü Asistanı"	 ,
         L"Büyü Araştıran"		 ,
         L"Sihirbaz"	 ,

         L"Büyü Katibi"	 ,
         L"Falcı"		 ,
         L"Arif"		 ,
         L"İlüzyonist"		 ,
         L"Tövbekar"		 ,

         L"Duacı"		 ,
         L"Duacı"		 ,
         L"Büyü Yapan"		 ,
         L"Büyü Yapan"		 ,
         L"Hokkabaz"		 ,

         L"Hokkabaz" ,
         L"Büyücü"	 ,
         L"Büyücü"	 ,
 	L"Oluşturan" ,
 	L"Oluşturan" ,

         L"Alim"		 ,
         L"Alim"		 ,
         L"Büyü Ustası"			 ,
         L"Büyü Ustası"			 ,
         L"Büyü Alimi"		 ,

         L"Büyü Alimi"		 ,
         L"Savaşçı Arif"		 ,
         L"Savaşçı Arif"		 ,
         L"Durendiş"	 ,
         L"Durendiş"	 ,

         L"Kıdemli Büyücü"		 ,
         L"Kıdemli Büyücü"		 ,
         L"Büyük Büyücü"		 ,
         L"Büyük Büyücü"	 ,
         L"Yüce Büyücü"		 ,

         L"Yüce Büyücü"	 ,
         L"Golem Efendisi"		 ,
         L"Golem Efendisi"	 ,
         L"Yüce Golem Efendisi"		 ,
         L"Yüce Golem Efendisi"	 ,

 	  L"Taş Efendisi"	 ,
 	  L"Taş Efendisi"	 ,
 	  L"İksir Ustası"	 ,
 	  L"İksir Ustası"	 ,
 	  L"Parşömen Ustası"	 ,

 	  L"Parşömen Ustası"	 ,
 	  L"Değnek Ustası"	 ,
 	  L"Değnek Ustası"		 ,
 	  L"Değnek Ustası"	 ,
 	  L"Değnek Ustası" ,

         L"İblis Çağıran"	 ,
 	  L"İblis Çağıran"		 ,
 	  L"Yüce İblis Çağıran"	 ,
 	  L"Yüce İblis Çağıran" ,
 	  L"Ejderha Efendisi" ,

         L"Ejderha Efendisi"		 ,
 	  L"Ejderha Efendisi"	 ,
 	  L"Ejderha Efendisi" ,
         L"Karışım Efendisi" ,
         L"Karışım Efendisi" ,

         L"Büyülerin Efendisi" ,
         L"Büyülerin Efendisi" ,
         L"Büyüleme Efendisi" ,
         L"Büyüleme Efendisi" ,
         L"Çağrı Ustası" ,

         L"Çağrı Ustası" ,
         L"Büyük Kahin" ,
         L"Büyük Kahin" ,
         L"Değişim Ustası" ,
         L"Değişim Ustası" ,

         L"Fişek Efendisi" ,
         L"Fişek Efendisi" ,
         L"Ölülerin Efendisi" ,
         L"Ölülerin Efendisi" ,
         L"Simyacı" ,

         L"Simyacı" ,
         L"Bağ Ustası" ,
         L"Bağ Ustası" ,
         L"Söz Efendisi" ,
         L"Söz Efendisi" ,

         L"Geçmişe hükmeden" ,
         L"Geçmişe hükmeden" ,
         L"Bugüne hükmeden" ,
         L"Bugüne hükmeden" ,
         L"Geleceğe hükmeden" ,

         L"Geleceğe hükmeden" ,
         L"Baş Büyücü" ,
         L"Baş Büyücü" ,
         L"Bilinmeyen" ,
         L"Bilinmeyen" ,

 	 L"Kahraman"		,
 	 L"Melek"		,
 	 L"Melek"		,
 	 L"Elçi"		,
 	 L"Ölümsüz"		,
 	L"Yüce"		,
 	L"Yüce",
 	L"Yüce",
 	L"Yüce",
 	 L"Tek"
     }
 ,
   };

const struct color_type color_table[] = /*  Last entry should be NULL   */
{
  { L"Black", CLR_BLACK},
  { L"Green", CLR_GREEN},
  { L"Brown", CLR_BROWN},
  { L"Yellow", CLR_YELLOW},
  { L"Red", CLR_RED},
  { L"Blue", CLR_BLUE},
  { L"Magenta", CLR_MAGENTA},
  { L"Cyan", CLR_CYAN},
  { L"White", CLR_WHITE},
  {NULL, NULL}
};

/* God's Name, name of religion, tattoo vnum  */
const struct religion_type religion_table [] =
{
  { L"", L"None", 0 },
  { L"Kamenilik", L"Kameniler", OBJ_VNUM_TATTOO_KAME },
  { L"Niryanilik", L"Niryaniler", OBJ_VNUM_TATTOO_NIR },
  { L"Nyahilik", L"Nyahiler", OBJ_VNUM_TATTOO_NYAH },
  { L"Sintiyanlık", L"Sintiyanlar", OBJ_VNUM_TATTOO_SINT },
};

const struct ethos_type ethos_table [] =
{
  { L"Null" },
  { L"Tüze" },
  { L"Yansız" },
  { L"Kaos" }
};

/*  altar good neut evil, recall good neut evil, pit good neut evil */
const struct hometown_type hometown_table [] =
{
  { L"Selenge", {3070, 3054, 3072}, {3068, 3001, 3071}, {3069,3054,3072} },
  { L"Yeni Thalos", {9605, 9605, 9605}, {9609,9609,9609}, {9609,9609,9609} },
  { L"Titan", {18127,18127,18127},{18126,18126,18126},{18127,18127,18127} },
  { L"Yeni Ofcol", {669, 669, 669},      {698, 698, 698},   {669, 669, 669} },
  { L"Eski Selenge",{5386, 5386,5386}, {5379,5379, 5379}, {5386,5386,5386} },
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
     { L"water", L"su", L"clear",	{   0, 1, 10, 0, 16 }	},
     { L"beer", L"bira", L"amber",	{  12, 1,  8, 1, 12 }	},
     { L"red wine", L"kırmızı şarap", L"burgundy",	{  30, 1,  8, 1,  5 }	},
     { L"ale", L"bira", L"brown",	{  15, 1,  8, 1, 12 }	},
     { L"dark ale", L"siyah bira", L"dark",		{  16, 1,  8, 1, 12 }	},

     { L"whisky", L"viski", L"golden",	{ 120, 1,  5, 0,  2 }	},
     { L"lemonade", L"limonata", L"pink",		{   0, 1,  9, 2, 12 }	},
     { L"firebreather", L"ateşnefesi", L"boiling",	{ 190, 0,  4, 0,  2 }	},
     { L"local specialty", L"yerel içki",  L"clear",	{ 151, 1,  3, 0,  2 }	},
     { L"slime mold juice", L"balçık suyu",  L"green",	{   0, 2, -8, 1,  2 }	},

     { L"milk", L"süt", L"white",	{   0, 2,  9, 3, 12 }	},
     { L"tea", L"çay", L"tan",		{   0, 1,  8, 0,  6 }	},
     { L"coffee",  L"kahve", L"black",	{   0, 1,  8, 0,  6 }	},
     { L"blood", L"kan", L"red",		{   0, 2, -1, 2,  6 }	},
     { L"salt water", L"tuzlu su", L"clear",	{   0, 1, -2, 0,  1 }	},

     { L"coke", L"kola", L"brown",	{   0, 2,  9, 2, 12 }	},
     { L"root beer", L"kök birası", L"brown",	{   0, 2,  9, 2, 12 }   },
     { L"elvish wine", L"yel şarabı", L"green",	{  35, 2,  8, 1,  5 }   },
     { L"white wine", L"beyaz şarap", L"golden",	{  28, 1,  8, 1,  5 }   },
     { L"champagne", L"şampanya", L"golden",	{  32, 1,  8, 1,  5 }   },

     { L"mead", L"bal likörü", L"honey-colored",{  34, 2,  8, 2, 12 }   },
     { L"rose wine", L"gül şarabı", L"pink",		{  26, 1,  8, 1,  5 }	},
     { L"benedictine wine", L"kutsal şarap",  L"burgundy",	{  40, 1,  8, 1,  5 }   },
     { L"vodka", L"votka", L"clear",	{ 130, 1,  5, 0,  2 }   },
     { L"cranberry juice", L"kızılcık suyu",  L"red",		{   0, 1,  9, 2, 12 }	},

     { L"orange juice", L"portakal suyu", L"orange",	{   0, 2,  9, 3, 12 }   },
     { L"absinthe", L"rakı", L"clear",	{ 200, 1,  4, 0,  2 }	},
     { L"brandy", L"brendi", L"golden",	{  80, 1,  5, 0,  4 }	},
     { L"aquavit", L"kimyon likörü", L"clear",	{ 140, 1,  5, 0,  2 }	},
     { L"schnapps", L"patates likörü", L"clear",	{  90, 1,  5, 0,  2 }   },

     { L"icewine", L"buzbağ", L"purple",	{  50, 2,  6, 1,  5 }	},
     { L"amontillado", L"kiraz likörü", L"burgundy",	{  35, 2,  8, 1,  5 }	},
     { L"sherry", L"şeri", L"red",		{  38, 2,  7, 1,  5 }   },
     { L"framboise", L"çilek likörü", L"red",		{  50, 1,  7, 1,  5 }   },
     { L"rum", L"rom", L"amber",	{ 151, 1,  4, 0,  2 }	},

     { L"cordial", L"şeftali likörü", L"clear",	{ 100, 1,  5, 0,  2 }   },
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
	{ L"reserved", L"reserved"},	{100,100,100,100, 100,100,100,100,100 ,100,100,100,100},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,			TAR_IGNORE,		POS_STANDING,
	&gsn_reserved,			SLOT( 0),	 0,	 0,
	L"", L"", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"absorb", L"soğurma"},	{ 67, 93, 93, 93, 93, 93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_absorb,		TAR_CHAR_SELF,		POS_STANDING,
	&gsn_absorb,		SLOT(707),	100,	12,
  L"", L"Çevrendeki enerji alanı yokoluyor!",
	L"$p'in çevresindeki enerji alanı yokoluyor.",
 	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"acetum primus", L"aketum primus"},	{ 34,93,93,93, 93,93,93,93,93,93,93,93,93},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_acetum_primus,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(654),	20,	12,
	L"aketum primus",  L"!Aketum Primus!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{ L"accid arrow", L"asit oku"},		{ 48, 93, 93, 93, 93, 93,93,93,48,93,93,48,48},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_acid_arrow,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(644),	20,	12,
	L"asit oku", L"!Asit Oku!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{ L"acid blast", L"asit patlaması"},		{ 63, 93, 93, 93, 93,93,93,93,63,93,93,63,63},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_acid_blast,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_acid_blast,	SLOT(70),	40,	12,
	L"asit", L"!Asit!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{ L"acute vision", L"güçlü görüş"},         {  93,93,93,93, 93, 93, 93, 7 ,93,93,93,93,93},
        { 3,  2,  1,  1, 1, 1, 2, 1, 1, 1,1,1,1},
        spell_acute_vision,        TAR_CHAR_SELF,          POS_STANDING,
        NULL,                    SLOT(514),        10,       0,
	L"", L"Görüşün sönükleşti.", L"",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"adamantite golem", L"adament golem"},{93,93,93,93,93,93,93,93,93,93,93,71,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_adamantite_golem,	TAR_IGNORE,	POS_STANDING,
	NULL,		SLOT(665),	500,		30,
	L"",  L"Yeni golemler yapmak için yeterli gücü topladın.", L"",
	CABAL_NONE , 	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"aid", L"ilk yardım"},			{ 93, 53,93, 93,93,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_aid,        TAR_CHAR_DEFENSIVE,          POS_FIGHTING,
	NULL,		SLOT(680),	100,		12,
	L"",  L"Daha fazla kişiyi iyileştirebilirsin.",  L"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_HEALING
    },

    {
	{ L"amnesia", L"amnezi"},         {  93, 93, 93,93,93,93,93,93,93,93,93,93,93 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_amnesia,      TAR_CHAR_OFFENSIVE,       POS_STANDING,
	NULL,       SLOT(538),        100,       12,
	L"", L"!amnesia!", L"",	CABAL_NONE , RACE_NONE,ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"animate dead", L"ölü canlandırma"},			{ 93, 93, 93, 93,93,93,93,93,93,93,42,38,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_animate_dead,	TAR_OBJ_CHAR_OFF,	POS_STANDING,
	NULL,		SLOT(581),	50,	12,
	L"",  L"Yeni ölüleri canlandırmak için gerekli enerjiyi topladın.", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"animate object", L"nesne canlandırma"},	{ 70, 93, 93, 93, 93, 93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_animate_object,	TAR_OBJ_CHAR_OFF,	POS_STANDING,
	NULL,		SLOT(709),	50,	12,
	L"", L"Yeni nesneleri canlandırmak için gerekli enerjiyi topladın.", L"",
 	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"armor", L"zırh"},		{  13,1,93,93, 12,93,13,93 , 93, 10,13,13,13},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_armor,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT( 1),	 5,	12,
	L"", L"Zırhının zayıfladığını hissediyorsun.",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	{ L"assist", L"destek"},      {  93, 93, 93, 93,93,93,93,93,93,93,93,43,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_assist,        TAR_CHAR_DEFENSIVE,          POS_FIGHTING,
        NULL,             SLOT(670),        100,       12,
	L"", L"Yeniden destek olabilirsin.", L"",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_HEALING
    },

    {
	{ L"astral walk", L"yıldız yürüyüşü"},		{31,93,93,93,93,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_astral_walk,	TAR_IGNORE,	POS_FIGHTING,
	NULL,			SLOT(622),	80,	12,
	L"", L"!Astral Walk!",  L"",CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_TRANSPORTATION
    },

    {
	{ L"attract other", L"kişi etkileme"},	{ 93, 93, 93, 93,93,93,93,93,93,93,30,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_attract_other,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
	NULL,		SLOT(580),	5,	12,
	L"",  L"Efendinin seni terkettiğini hissediyorsun.",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_BEGUILING
    },

    {
	{ L"bark skin", L"ağaç deri"},        {  93, 93, 93, 93, 93, 93, 93, 22 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_bark_skin,        TAR_CHAR_SELF,          POS_STANDING,
        NULL,             SLOT(515),        40,       0,
	L"", L"Ağaç derin yokoluyor.", L"",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
		{ L"black death", L"kara ölüm"},    { 93, 93, 93, 93,93, 64, 93, 93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_black_death,	TAR_IGNORE,	POS_STANDING,
	&gsn_black_death,	SLOT(677),	200,	24,
	L"", L"!black death!",  L"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_MALADICTIONS
    },

    {
	{ L"blade barrier", L"bıçak duvarı"},	{93,60,93,93,93,93,93,93,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_blade_barrier,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(679),	40,	12,
	L"bıçak duvarı",  L"!Blade Barrier!",  L"",CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_ATTACK
    },

    {
	{ L"bless", L"kutsama"},		{ 93,  10, 93, 93,14,93,93,93,93,12,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_bless,		TAR_OBJ_CHAR_DEF,	POS_STANDING,
	&gsn_bless,		SLOT( 3),	 5,	12,
  L"", L"Kutsallığını yitiriyorsun.",
	L"$p'in kutsal aylası yokoluyor.", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_BENEDICTIONS
    },

    {
	{ L"bless weapon", L"silah kutsama"},{  93, 55, 93, 93,55,93,93,93,93,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_bless_weapon,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(637),	100,	24,
	L"", L"!Bless Weapon!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_ENCHANTMENT
    },

    {
	{ L"blindness", L"körlük"},	{  93,  14, 93, 93,93,14,16,93, 20,93,20,19,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_blindness,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_blindness,		SLOT( 4),	 5,	12,
	L"", L"Yeniden görebiliyorsun.",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_MALADICTIONS
    },

    {
	{ L"bluefire", L"mavi alev"},	{  93,37,93,93,93,93,93,93,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_bluefire,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_bluefire,			SLOT(660),	20,	12,
	L"mavi alev", L"!Mavi Alev!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_ATTACK
    },

    {
	{ L"burning hands", L"yanan eller"},	{  14,93,93,93, 93, 93, 93, 93 ,14,93,14,14,14},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_burning_hands,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_burning_hands,	SLOT( 5),	15,	12,
  L"yanan eller",  L"!Burning Hands!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{ L"call lightning", L"şimşek çağrısı"},	{ 93, 44, 93, 93, 41,93,50, 93 , 93,93,93,93,33},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_call_lightning,	TAR_IGNORE,		POS_FIGHTING,
	NULL,			SLOT( 6),	15,	12,
  L"şimşek çağrısı",  L"!Call Lightning!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_WEATHER
    },

    {
    { L"calm", L"yatıştırma"},	{ 93, 26, 93, 93, 63, 93, 93, 93 ,93,60,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_calm,		TAR_IGNORE,		POS_FIGHTING,
	NULL,			SLOT(509),	30,	12,
  L"", L"Sakinliğini yitiriyorsun.",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_BENEDICTIONS
    },

    {
	{ L"cancellation", L"iptal"},	{ 24, 26, 93, 93,93,93,93,93 , 28,93,93,28,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_cancellation,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(507),	20,	12,
	L"", L"!cancellation!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	{ L"cause critical", L"kritik zarar"},	{  93,  15, 93, 93,93,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1,1, 1,1,1,1},
	spell_cause_critical,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(63),	20,	12,
  L"kritik zarar", L"!Cause Critical!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_HARMFUL
    },

    {
	{ L"cause light", L"hafif zarar"},		{ 93,  2, 93, 93,93,93,93,93,93 ,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_cause_light,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(62),	15,	12,
  L"hafif zarar", L"!Cause Light!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_HARMFUL
    },

    {
	{ L"cause serious", L"ciddi zarar"},	{  93,  8, 93, 93,93,93,93,93, 93 ,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_cause_serious,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(64),	17,	12,
  L"ciddi zarar", L"!Cause Serious!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_HARMFUL
    },

    {
	{ L"caustic font", L"yakıcı su"},{ 93, 93, 93, 93, 93, 93, 93,93,41,93,93,93,93},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_caustic_font,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(653),	20,	12,
  L"yakıcı su",  L"!caustic font!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{ L"chain lightning", L"şimşek zinciri"},		{ 33, 93, 93, 93,93,93,93,93, 33 ,93,33,33,33},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_chain_lightning,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(500),	25,	12,
  L"şimşek", L"!Chain Lightning!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{ L"charm person", L"teshir"},		{ 93, 93, 93, 93,93,38,93,93, 93,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_charm_person,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_charm_person,	SLOT( 7),	 5,	12,
  L"", L"Kendine güvenin artıyor.",  L"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_BEGUILING
    },

    {
	{ L"chromatic orb", L"krom küre"},	{ 93, 93, 93, 93, 93,93,93,93,63,93,93,93,93},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_chromatic_orb,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(714),	50,	12,
  L"krom küre", L"!Chromatic Orb!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{ L"control undead", L"hortlak kontrolü"},      {  93, 93,93,93,93,93,93,93,93,93,93,57,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_control_undead,   TAR_CHAR_OFFENSIVE,          POS_STANDING,
        NULL,             SLOT(669),        20,       12,
        L"", L"Kendine güvenin artıyor.", L"",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"chill touch", L"ayaz"},		{  5, 93, 93, 93,93,6,93,93 ,5,93,93,7,8 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_chill_touch,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_chill_touch,		SLOT( 8),	15,	12,
  L"ayaz",  L"Üşümen azalıyor.",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{ L"colour spray", L"renk spreyi"},		{  22, 93, 93, 93,93,93,93,93 , 22,93,25,93,22},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_colour_spray,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(10),	15,	12,
  L"renk spreyi", L"!Colour Spray!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{ L"continual light", L"daimi ışık"},		{  11,  5, 93, 93,93,93,93,93 , 11,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_continual_light,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(57),	 7,	12,
	L"", L"!Continual Light!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_CREATION
    },

    {
	{ L"control weather", L"hava kontrolü"},	{ 93,21,93,93, 93,93,11,33, 30,93,24,24,20},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_control_weather,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(11),	25,	12,
	L"", L"!Control Weather!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_WEATHER
    },

    {
	{ L"corruption", L"çürüme"},      {  93, 93, 93, 93, 93,93,93,93,93,93,93,63,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_corruption,    TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
        NULL,             SLOT(671),        20,       12,
        L"çürüme", L"Kendini sağlıklı hissediyorsun.", L"",CABAL_NONE,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"create food", L"mantar"},		{ 15, 8, 93, 93,3,93,93,93 , 93,93,93,12,13},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_create_food,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(12),	 5,	12,
	L"", L"!Create Food!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_CREATION
    },

    {
	{ L"create rose", L"gül"},		{ 26, 93, 93, 93, 20, 93, 93, 93,93,93,26,26,26},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_create_rose,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(511),	30, 	12,
	L"", L"!Create Rose!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_CREATION
    },

    {
	{ L"create spring", L"pınar"},		{ 24, 27, 93, 93,93,93,93,93 ,93,93,93,31,31},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_create_spring,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(80),	20,	12,
	L"", L"!Create Spring!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_CREATION
    },

    {
	{ L"create water", L"su yaratma"},	{ 8,  3, 93, 93,4,93,93,93 ,93,93,93,11,8},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_create_water,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(13),	 5,	12,
	L"", L"!Create Water!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_CREATION
    },

    {
	{ L"cure blindness", L"körlük tedavi"},		{ 93,11,93,93, 7,93,9,13, 93,20,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_cure_blindness,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(14),	 5,	12,
	L"", L"!Cure Blindness!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_CURATIVE
    },

    {
	{ L"cure critical", L"kritik tedavi"},	{93,  18, 93, 93,18,93,25,93,93,25,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_cure_critical,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	&gsn_cure_critical,	SLOT(15),	20,	12,
	L"", L"!Cure Critical!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_HEALING
    },

    {
	{ L"cure disease", L"hastalık tedavi"},		{  93, 19, 93, 93,33,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_cure_disease,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(501),	20,	12,
	L"", L"!Cure Disease!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_CURATIVE
    },

    {
	{ L"cure light", L"hafif tedavi"},	{ 93,  4, 93, 93,5,11,9,93,93,10,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_cure_light,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	&gsn_cure_light,	SLOT(16),	10,	12,
	L"", L"!Cure Light!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_HEALING
    },

    {
	{ L"poison", L"zehir"},		{  93,  22, 93,93,93,15,93,93,27,93,93,23,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_poison,		TAR_OBJ_CHAR_OFF,	POS_FIGHTING,
	&gsn_poison,		SLOT(33),	10,	12,
  L"zehir", L"Üzerindeki zehir kuruyor.",
	L"$p üzerindeki zehir kuruyor.", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_MALADICTIONS
    },

    {
	{ L"cure poison", L"zehir tedavi"},			{ 93,  23, 93,93,33,93,93,93,93,35,93,93,23 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_cure_poison,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_cure_poison,		SLOT(43),	 5,	12,
	L"", L"!Cure Poison!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_CURATIVE
    },

    {
	{ L"cure serious", L"ciddi tedavi"},	{ 93,  10, 93, 93,10,93,18,93,93,18,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_cure_serious,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	&gsn_cure_serious,	SLOT(61),	15,	12,
	L"", L"!Cure Serious!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_HEALING
    },

    {
	{ L"curse", L"lanet"},		{ 93, 33, 93,93,93,18,93,93,31,93,44,34,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_curse,		TAR_OBJ_CHAR_OFF,	POS_FIGHTING,
	&gsn_curse,		SLOT(17),	20,	12,
  L"lanet", L"Üzerindeki lanet çürüyor.",
	L"$p üzerindeki lanet çürüyor.", CABAL_NONE , RACE_NONE,
	ALIGN_NONE, GROUP_MALADICTIONS
    },

    {
	{ L"cursed lands", L"lanetli topraklar"},    { 93, 41, 93, 93,93, 93, 93, 93 ,93,93,93,64,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_cursed_lands,	TAR_IGNORE,	POS_STANDING,
	&gsn_cursed_lands,	SLOT(675),	200,	24,
	L"", L"!cursed lands!",  L"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_MALADICTIONS
    },

    {
	{ L"deadly venom", L"ölüm zehri"},   { 93, 93, 93, 93,93, 58, 93, 93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_deadly_venom,	TAR_IGNORE,	POS_STANDING,
	&gsn_deadly_venom,		SLOT(674),	200,	24,
	L"", L"!deadly venom!",  L"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_MALADICTIONS
    },

    {
	{ L"deafen", L"sağırlık"},       {  93,93,93,93,93,42,93,93 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_deafen,      TAR_CHAR_OFFENSIVE,   POS_FIGHTING,
        &gsn_deafen,       SLOT(570),        40,       12,
        L"sağırlık", L"Kulaklarındaki çınlama sonunda yokoluyor.", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"demonfire", L"iblis ateşi"},		{  93, 37, 93, 93,93,93,93,93,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_demonfire,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_demonfire,			SLOT(505),	20,	12,
  L"iblis ateşi", L"!Demonfire!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_ATTACK
    },

    {
	{ L"desert fist", L"çöl yumruğu"},		{ 93, 58, 93,93,93,93,93,93,93,93,93,93,55 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_desert_fist, 	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(681),	50,	12,
  L"çöl yumruğu",  L"!desert fist!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE,GROUP_NONE
    },

    {
	{ L"detect evil", L"kem saptama"},		{ 12,  11, 93, 93,3,93,93,93 ,12,93,93,93,13},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_detect_evil,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(18),	 5,	12,
  L"", L"Kem saptama sönüyor.",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_DETECTION
    },

    {
	{ L"detect good", L"iyi saptama"},         { 12,  11, 93, 93,3,93,93,93 ,12,93,13,13,13},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_detect_good,      TAR_CHAR_SELF,          POS_STANDING,
        NULL,                   SLOT(513),        5,     12,
        L"", L"İyiyi saptama sönüyor.",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_DETECTION
    },

    {
	{ L"detect hide", L"saklıyı saptama"},	{ 93, 93, 10, 93, 93, 93, 10, 93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_CHAR_SELF,		POS_STANDING,
	&gsn_detect_hidden,	SLOT(0),	 5,	12,
  L"", L"Saklıyı saptama sönüyor.",
	L"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_DETECTION
    },

    {
	{ L"detect invis", L"görünmezi saptama"},		{  5,  17, 93, 93,23,13,93,93 ,10,93,23,13,8},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_detect_invis,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(19),	 5,	12,
  L"", L"Görünmezi saptama sönüyor.",
	L"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_DETECTION
    },

    {
	{ L"detect magic", L"büyü saptama"},	{  8,  12, 93, 93,93,93,93,93,6,30,93,15,12},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_detect_magic,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(20),	 5,	12,
  L"", L"Büyü saptama sönüyor.",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_DETECTION
    },

    {
	{ L"detect poison", L"zehir saptama"},	{ 35,  39, 93, 93,93,93,93,93,35 ,39,31,21,7},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_detect_poison,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(21),	 5,	12,
	L"", L"!Detect Poison!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_DETECTION
    },

    {
{ L"detect undead", L"hortlak saptama"},	{ 93, 93,93, 93,93,93,93,93 ,93,93,93,13,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_detect_undead,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,		SLOT(594),	5,	12,
  L"", L"Hortlak saptama sönüyor.", L"",
	CABAL_NONE, 	RACE_NONE, ALIGN_NONE, GROUP_DETECTION
    },

    {
	{ L"disenchant armor", L"zırh alçalt"},	{ 27, 93, 93, 93, 93, 93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_disenchant_armor,		TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(705),	50,	24,
	L"", L"!disenchant armor!",  L"", 	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"disenchant weapon", L"silah alçalt"},		{ 34, 93, 93, 93, 93, 93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_disenchant_weapon,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(706),	50,	24,
	L"", L"!disenchant weapon!",  L"", 	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"disintegrate", L"ufala"},       {  75,93,93,93,93,93,93,93,93,93 ,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_disintegrate,      TAR_CHAR_OFFENSIVE,   POS_STANDING,
        NULL,                    SLOT(574),    100,       18,
        L"ince ışık ışını", L"!disintegrate!", L"",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"arz yutağı", L"arz yutağı"},       {  93,93,93,93,93,93,93,93,93,93 ,93,93,75},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_arz_yutagi,      TAR_CHAR_OFFENSIVE,   POS_STANDING,
        NULL,                    SLOT(574),    100,       18,
        L"arz yutağı", L"!arz yutağı!", L"",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"dismantle", L"parçala"},	{ 93, 93,93, 93,93,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,		SLOT(621),	200,	24,
	L"",  L"!621!",  L"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"dispel evil", L"kem defet"},		{ 93, 27, 93, 93,35,93,93,93,55 ,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_dispel_evil,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(22),	15,	12,
  L"kem defet", L"!Dispel Evil!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_ATTACK
    },

    {
	{ L"dispel good", L"iyilik defet"},          {   93,  27,  93,  93,93,30,93,93,57,93,32,32,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_dispel_good,      TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,                   SLOT(512),      15,     12,
        L"iyilik defet",          L"!Dispel Good!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_ATTACK
    },

    {
	{ L"dispel magic", L"büyü defet"},	{ 36, 43, 93, 93,93,93,93,93 ,41,93,24,24,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_dispel_magic,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(59),	15,	12,
	L"", L"!Dispel Magic!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	{ L"disruption", L"nefes kesme"},	{ 93, 93, 93, 93, 93, 93, 93,93,93,93,93,40,93},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_disruption,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(648),	20,	12,
  L"nefes kesme",  L"!disruption!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{ L"dragon breath", L"ejderha nefesi"}, {93,93,93,93,93,93,93,93,93,93,93,93,93},
	{1,1,1,1,1,1,1,1,1, 1,1,1,1},
        spell_dragon_breath,  TAR_CHAR_OFFENSIVE, POS_FIGHTING,
        &gsn_dragon_breath,  SLOT(563),   75,   12,
        L"ejderha nefesi", L"!dragon breath!", L"",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"dragon skin", L"ejderha derisi"},	{ 10, 10,10, 10,10,10,10,10 ,10,10,10,10,10},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_dragon_skin,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(612),	50,	24,
  L"",  L"Derin yumuşuyor.",  L"",
	CABAL_NONE , RACE_TOP, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	{ L"dragon strength", L"ejderha gücü"}, {93,93,93,93,93,93,93,93,93,93,93,93,93},
	{1,1,1,1,1,1,1,1, 1, 1,1,1,1},
        spell_dragon_strength, TAR_CHAR_SELF, POS_FIGHTING,
        &gsn_dragon_strength, SLOT(562),    75,    12,
        L"", L"Ejderha gücünün seni terkettiğini hissediyorsun.", L"",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"dragons breath", L"ejderha nefesi"},	{ 61, 93, 93, 93, 93, 93, 93, 93 ,61,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_dragons_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(576),	200,	24,
  L"ejderha nefesi",  L"Sağlığına kavuşuyorsun.",
	L"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_DRACONIAN
    },

    {
	{ L"drain", L"kurut"},		{ 31, 93, 93, 93, 93, 93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_drain,		TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(704),	5,	12,
	L"", L"!drain!",  L"", 	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_MALADICTIONS
    },

    {
	{ L"earthfade", L"arz soluşu"},		{ 93, 93, 93, 93, 93, 93,93,93,93,93,93,93,73},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_earthfade,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	&gsn_earthfade,			SLOT(702),	100,	12,
  L"", L"Doğal formuna dönüyorsun.",  L"",
 	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"earthmaw", L"fay"},		{ 93, 93, 93, 93, 93, 93,93,93,93,93,93,93,67},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_earthmaw,		TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(703),	30,	12,
	L"earthmaw", L"!earthmaw!",  L"", 	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"earthquake", L"deprem"},	{ 93,  19, 93, 93,93,93,93,93,93,93,93,93,19 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_earthquake,	TAR_IGNORE,		POS_FIGHTING,
	NULL,			SLOT(23),	15,	12,
  L"deprem", L"!Deprem!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_ATTACK
    },

    {
	{ L"elemental sphere", L"uşak küresi"},	{ 93, 93,93,93, 93,93,93,93 ,93,93,93,93,29},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_elemental_sphere,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(719),	75,	12,
  L"",  L"Koruyucu uşak küresi yokoluyor.",  L"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"enchant armor", L"zırh yükselt"},	{  26, 25, 93, 93,93,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_enchant_armor,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(510),	100,	24,
	L"", L"!Enchant Armor!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_ENCHANTMENT
    },

    {
	{ L"enchant weapon", L"silah yükselt"},{  33, 25, 93, 93,93,93,93,93,17,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_enchant_weapon,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(24),	100,	24,
	L"", L"!Enchant Weapon!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_ENCHANTMENT
    },

    {
	{ L"energy drain", L"yaşam kurut"},		{ 38,42,93,93,93,26,93,93,93,93,93,45,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_energy_drain,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(25),	35,	12,
  L"yaşam kurut", L"!Energy Drain!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_MALADICTIONS
    },

    {
	{ L"enhanced armor", L"gelişmiş zırh"},	{ 48, 33, 93,93,93,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
	spell_enhanced_armor, 	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	&gsn_enhanced_armor,	SLOT(583),	20,	12,
  L"",  L"Korunmasız hissediyorsun.", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	{ L"enlarge", L"genişleme"},		{  93,93,93,93, 93,93,93,93, 38,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_enlarge,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(711),	 50,	12,
  L"",  L"Doğal boyutlarına dönüyorsun.",  L"", CABAL_NONE,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"etheral fist", L"ruhani yumruk"},		{ 93,93,93,93, 93, 50,93,93,93,93,93,93,93},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_etheral_fist,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(645),	20,	12,
  L"ruhani yumruk", L"!Etheral Fist!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{ L"faerie fire", L"cin ateşi"},		{ 11,  6, 93, 93,93,93,8,93 ,15 ,93,93,93,11},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_faerie_fire,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_faerie_fire,		SLOT(72),	 5,	12,
  L"cin ateşi", L"Pembe aylandan kurtuluyorsun.",
	L"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_WEATHER
    },

    {
	{ L"faerie fog", L"cin sisi"},	{ 24, 31, 93, 93,93,93,93,93 ,27,93,93,93,24},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_faerie_fog,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(73),	12,	12,
  L"cin sisi", L"!Faerie Fog!", L"", CABAL_NONE ,
	RACE_NONE,ALIGN_NONE, GROUP_WEATHER
    },

    {
	{ L"farsight", L"uzak görüş"},	{  22, 93, 93, 93,93,93,93,93,22,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_farsight,		TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(521),	20,	12,
  L"uzak görüş", L"!Farsight!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_DETECTION
    },

    {
	{ L"fear", L"korku"},			{   93, 93, 93, 93,93,93,93,93,93 ,93,93,73,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_fear,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_fear,	SLOT(598),	50,	12,
  L"", L"Daha cesur hissediyorsun.",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_ILLUSION
    },

    {
	{ L"fire and ice", L"ateş ve buz"},		{ 93, 93, 93, 93, 93, 93,93,93,93,93,93,93,65},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_fire_and_ice,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(699),	40,	12,
  L"ateş ve buz", L"!fire and ice!",  L"", 	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{ L"fireball", L"alev topu"},		{55,93,93,93,93,56,93,93,56,93,93,93,55},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_fireball,		TAR_IGNORE,	POS_FIGHTING,
	NULL,			SLOT(26),	25,	12,
  L"alev topu", L"!Fireball!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{ L"fireproof", L"ateş geçirmez"},		{ 25, 16, 93, 93, 20, 93,93,93,21,93,93,42,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_fireproof,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(523),	10,	12,
  L"", L"",  L"$p üzerindeki koruyucu ayla yokoluyor.", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_ENCHANTMENT
    },

    {
	{ L"firestream", L"alev akımı"},	{ 93, 93, 93, 93, 93, 93,93,93,93,93,93,93,35},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_firestream,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(692),	20,	12,
	L"", L"",  L"", 	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{ L"fire shield", L"alev kalkanı"},			{   93, 93, 93, 93,93,93,93,93,93 ,93,93,93,68},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_fire_shield,	TAR_IGNORE,	POS_STANDING,
	&gsn_fire_shield,		SLOT(601),	200,	24,
	L"", L"!fire shield!",
	L"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"flamestrike", L"alev saldırısı"},			{93,42,93,93,32,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_flamestrike,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(65),	20,	12,
  L"alev saldırısı", L"!Flamestrike!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_ATTACK
    },

    {
	{ L"fly", L"uçma"},				{ 19, 30, 93,93,93,93,93,93,19,93,43,23,21},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_fly,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_fly,			SLOT(56),	10,	18,
  L"", L"Yere iniyorsun.",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_TRANSPORTATION
    },

    {
	{ L"floating disc", L"asılı disk"},		{ 4, 10, 93, 93, 93, 93, 93, 93 ,4,93,93,93,4},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_floating_disc,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(522),	40,	24,
	L"", L"!Floating disc!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"forcecage", L"güç kafesi"},			{ 29, 93,93,93, 93,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_forcecage,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(717),	75,	12,
  L"",  L"Çevrendeki güç kafesi yokoluyor.",  L"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"frenzy", L"cinnet"},               {  93, 34, 93, 93,93,93,93,93,93,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_frenzy,           TAR_CHAR_DEFENSIVE,     POS_STANDING,
        NULL,                   SLOT(504),      30,     24,
        L"", L"Cinnetin geçiyor.",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_BENEDICTIONS
    },

    {
	{ L"frostbolt", L"ayaz topu"},		{ 93, 93, 93, 93, 93, 93,93,93,93,93,93,93,42},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_frostbolt,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(695),	20,	12,
	L"ayaz topu", L"!frostbolt!",  L"", 	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{ L"fumble", L"beceriksizlik"},		{ 93,93,93,93, 93,93,93,93 ,36,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_fumble,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(712), 	25,	18,
  L"", L"Hızlanıyor ve eski gücüne kavuşuyorsun!",  L"",
	CABAL_NONE, RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"galvanic whip", L"enerji kırbacı"},	{ 28, 93, 93, 93, 93, 93,93,93,93,93,93,93,93},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_galvanic_whip,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(655),	20,	12,
  L"enerji kırbacı",  L"!galvanic whip!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{ L"gate", L"geçit"},			{93,63,93,93,93,93,93,93,27,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_gate,		TAR_IGNORE,		POS_FIGHTING,
	NULL,			SLOT(83),	80,	12,
	L"", L"!Gate!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_TRANSPORTATION
    },

    {
	{ L"giant strength", L"dev gücü"},{  20, 39, 93, 93,93,93,93,93, 30,93,93,28,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_giant_strength,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_giant_strength,		SLOT(39),	20,	12,
  L"", L"Gücün azalıyor.",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_ENHANCEMENT
    },

    {
	{ L"grounding", L"topraklama"},		{ 93, 93, 93, 93, 93, 93,93,93,93,93,93,93,53},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_grounding,	TAR_CHAR_SELF,		POS_FIGHTING,
	NULL,			SLOT(700),	50,	12,
  L"", L"Topraklamanı yitiriyorsun.",  L"",
 	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"group defense", L"grup savunma"},	{   93, 35, 93,93,93,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_group_defense,		TAR_IGNORE,		POS_STANDING,
	NULL,		SLOT(586),	100,	36,
  L"", L"Korunmasız hissediyorsun.",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_BENEDICTIONS
    },

    {
	{ L"group heal", L"grup şifa"},			{  93,65,93,93,75,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_group_healing,	TAR_CHAR_DEFENSIVE, POS_FIGHTING,
	NULL,			SLOT(642),	500,	24,
	L"", L"!Group Heal!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_HEALING
    },

    {
	{ L"hallucination", L"sanrı"},	{   93, 93, 93, 93,93,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_hallucination,	TAR_CHAR_SELF,	POS_STANDING,
	NULL,		SLOT(606),	200,	12,
  L"", L"Büyüye karşı yine savunmasızsın.",
	L"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_ILLUSION
    },

    {
	{ L"hand of undead", L"hortlak eli"},	{ 93, 93, 93,93,93,93,93,93,93,93,93,44,93},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_hand_of_undead,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(647),	20,	24,
  L"hortlak eli",  L"!hand of undead!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{ L"harm", L"zarar"},			{  93, 26, 93, 93,93,29,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_harm,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(27),	35,	12,
  L"zarar", L"!Harm!",  L"", 	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_HARMFUL
    },

    {
	{ L"haste", L"hız"},	{ 24, 93, 93,93,93,93,93,93,24,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_haste,		TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	&gsn_haste,		SLOT(502),	30,	12,
  L"", L"Yavaşladığını hissediyorsun.",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_ENHANCEMENT
    },

    {
	{ L"heal", L"şifa"},		{  93, 28, 93, 93,29,93,93,93 ,93 ,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_heal,		TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(28),	50,	12,
	L"", L"!Heal!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_HEALING
    },

    {
	{ L"healing light", L"şifa ışığı"},    { 93, 41, 93, 93,93, 93, 93, 93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_healing_light,	TAR_IGNORE,	POS_STANDING,
	NULL,			SLOT(613),	200,	24,
  L"", L"Bir odayı yeniden ışığınla doldurabilirsin.",  L"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_BENEDICTIONS
    },

    {
	{ L"heat metal", L"metali ısıtma"},		{ 93,26,93,93, 93,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_heat_metal,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(516), 	25,	18,
  L"metali ısıtma", L"!Heat Metal!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"helical flow", L"sarmal akım"},		{93,93,93,93,93,93,93,93,93,93,93,93,43},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_helical_flow,		TAR_IGNORE,	POS_FIGHTING,
	NULL,			SLOT(661),	80,	12,
	L"", L"!Helical Flow!",  L"",CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_TRANSPORTATION
    },

    {
	{ L"hellfire", L"cehennem ateşi"},     {  93, 93, 93, 93, 93, 93, 45, 93 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_hellfire,      TAR_CHAR_OFFENSIVE,          POS_FIGHTING,
        NULL,       SLOT(520),        20,       12,
        L"cehennem ateşi", L"!hellfire!", L"",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_ATTACK
    },

    {
	{ L"holy aura", L"kutsal ayla"},{ 93, 93,93,93,62,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_holy_aura,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,		SLOT(617),	75,	12,
  L"",  L"Kutsal aylan yokoluyor.",  L"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"holy fury", L"kutsal hiddet"},	{ 93, 93,93,93,59,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_holy_fury,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,		SLOT(682),	50,	24,
  L"",  L"Artık daha merhametlisin.",  L"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"holy word", L"kutsal söz"},		{ 93,48,93,93,38,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_holy_word,	TAR_IGNORE,	POS_FIGHTING,
	NULL,			SLOT(506), 	200,	24,
  L"kutsal söz", L"!Holy Word!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_BENEDICTIONS
    },

    {
	{ L"hurricane", L"kasırga"},	{  93, 93, 93, 93, 93,53,93,93,93,93,93,65,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_hurricane,	TAR_IGNORE,	POS_FIGHTING,
	NULL,		SLOT(672),	200,	24,
  L"kasırga",  L"!Hurricane!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"hydroblast", L"su patlaması"},	{  93,93,93,93, 93,93,93,46, 93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_hydroblast,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(684),	50,	12,
  L"su patlaması",  L"!Hydroblast!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"iceball", L"buztopu"},        {  93, 93, 93, 93, 93,58,93,93,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_iceball,     TAR_IGNORE,     POS_FIGHTING,
        NULL,                   SLOT(513),        25,       12,
        L"buztopu", L"!Iceball!", L"",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{ L"identify", L"tanımlama"},		{ 18, 36, 93, 93,93,93,93,93 ,25,93,93,93,18},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_identify,		TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(53),	12,	24,
	L"", L"!Identify!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_DETECTION
    },

    {
	{ L"improved detect", L"gelişmiş saptama"},	{ 40, 93, 93, 93, 93,93,93, 93 ,40,93,93,40,40},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_improved_detection,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,		SLOT(626),	 20,	12,
  L"", L"Artık çevrende olup bitenden daha az haberdarsın.",
	L"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_DETECTION
    },

    {
	{ L"improved invis", L"gelişmiş görünmezlik"},	{  43, 93, 93, 93,93,93,93,93,43,93,93,93,43 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_improved_invis,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_imp_invis,		SLOT(627),	 20,	12,
  L"", L"Görünmezliğin yitiyor.",
	L"$p görünürleşiyor.", CABAL_NONE , RACE_NONE,ALIGN_NONE, GROUP_ILLUSION
    },

    {
	{ L"infravision", L"ısı görüşü"},		{  17,  25, 93, 93,93,93,5,93,18,93,93,21,17},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_infravision,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(77),	 5,	18,
  L"", L"Artık karanlıkta göremeyeceksin.",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_ENHANCEMENT
    },

    {
	{ L"insanity", L"delilik"},	{ 93, 93,93, 93,93,93,93,93 ,93,93,93,59,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_insanity,		TAR_CHAR_OFFENSIVE,	POS_STANDING,
	NULL,		SLOT(616),	100,	24,
  L"",  L"Sakinleştiğini hissediyorsun.",  L"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"inspire", L"ilham"},		{   93, 49, 93,93,93,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_inspire,		TAR_IGNORE,		POS_STANDING,
	NULL,		SLOT(587),	75,	24,
  L"", L"İlhamın azalıyor.",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_BENEDICTIONS
    },

    {
	{ L"invisibility", L"görünmezlik"},		{  10, 93, 93, 93,93,9,93,93,13,93,93,93,10 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_invis,		TAR_OBJ_CHAR_DEF,	POS_STANDING,
	&gsn_invis,		SLOT(29),	 5,	12,
	L"", L"Artık görünmez değilsin.",
	L"$p görünür oluyor.", CABAL_NONE , RACE_NONE,ALIGN_NONE, GROUP_ILLUSION
    },

    {
{ L"iron body", L"demir gövde"},	{ 93, 93,93,93, 93,93,93,93 ,29,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_iron_body,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(718),	75,	12,
  L"",  L"Derin yumuşuyor.",  L"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"iron golem", L"demir golem"},		{93,93,93,93,93,93,93,93,93,93,93,52,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_iron_golem,	TAR_IGNORE,		POS_STANDING,
	NULL,		SLOT(664),	400,		24,
  L"",  L"Yeni golemler yapmak için yeterli gücü topladın.", L"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"knock", L"taktak"},			{   56,93,93,93,93,93,93,93,64,93,93,62,46},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_knock,	TAR_IGNORE,	POS_STANDING,
	NULL,		SLOT(603),	20,	24,
	L"", L"!knock!",
	L"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"know alignment", L"yönelim saptama"},	{  22,  13,93,93,1,5,93,93,29,93,93,22,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_know_alignment,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(58),	 9,	12,
	L"", L"!Know Alignment!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_DETECTION
    },

    {
	{ L"lesser golem", L"küçük golem"},		{93,93,93,93,93,93,93,93,93,93,93,25,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_lesser_golem,	TAR_IGNORE,		POS_STANDING,
	NULL,		SLOT(662),	200,		12,
  L"",  L"Yeni golemler yapmak için yeterli gücü topladın.", L"",
	CABAL_NONE , 	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"lethargic mist", L"uyuşukluk sisi"},   { 52, 93,93,93,93, 62, 93,93,56,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_lethargic_mist,	TAR_IGNORE,	POS_STANDING,
	&gsn_lethargic_mist,	SLOT(676),	200,	24,
	L"", L"!lethargic mist!",  L"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_MALADICTIONS
    },

    {
	{ L"light arrow", L"ışık oku"},	{  93, 93, 93, 93,64,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_light_arrow,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(683),	40,	12,
	L"ışık oku",  L"!light arrow!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{ L"lightning bolt", L"yıldırım"},	{  23, 93, 93, 93,93,13,93,93 ,23,93,93,23,23},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_lightning_bolt,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_lightning_bolt,		SLOT(30),	15,	12,
  L"yıldırım",  L"!Lightning Bolt!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{ L"lightning shield", L"şimşek kalkanı"},	{ 41, 93,93, 93,93,93,93,93 ,93,93,93,41,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_lightning_shield,	TAR_IGNORE,	POS_STANDING,
	NULL,		SLOT(614),	150,	24,
  L"şimşek kalkanı",  L"Bir odayı yeniden kalkanlayabilirsin.",  L"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"link", L"bağ"},	{   31, 93, 93, 93,93,93,93,93,33 ,93,93,33,33},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_link,		TAR_CHAR_DEFENSIVE,		POS_STANDING,
	NULL,		SLOT(588),	125,	18,
	L"", L"!link!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_MEDITATION
    },

    {
	{ L"lion help", L"aslan yardımı"},	{   93, 93, 93, 93,93,93,93,63,93 ,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_lion_help,	TAR_IGNORE,		POS_STANDING,
	NULL,		SLOT(595),	100,	12,
  L"",  L"Yeniden aslan yardımına başvurabilirsin.",  L"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"locate object", L"nesne bulma"},	{   30, 30, 93, 93,19,93,14,93,29,93,93,93,31},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_locate_object,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(31),	20,	18,
	L"", L"!Locate Object!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_DETECTION
    },

    {
	{ L"love potion", L"aşk iksiri"},           { 93, 93, 93,93,93,93,93,93 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_love_potion, TAR_CHAR_SELF,       POS_STANDING,
        &gsn_love_potion,  SLOT(666),        10,       0,
        L"", L"Gözlerindeki rüya sönüyor.", L"",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"magic jar", L"büyülü şişe"},		{   93, 93, 93, 93,93,93,93,93,93 ,93,93,68,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_magic_jar,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(596),	20,	12,
	L"", L"!magic jar!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_BEGUILING
    },

    {
	{ L"magic missile", L"fişek"},	{ 1, 93, 93, 93,93,1,93,93 ,1,93,93,2,1},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_magic_missile,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_magic_missile,		SLOT(32),	15,	12,
	L"fişek",  L"!Magic Missile!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{ L"magic resistance", L"büyü direnci"},	{   68, 93, 93, 93,93,93,93,93,72 ,93,93,69,71},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_magic_resistance,	TAR_CHAR_SELF,	POS_STANDING,
	NULL,		SLOT(605),	200,	24,
  L"", L"Büyüye karşı yeniden savunmasızsın.",
	L"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	{ L"magnetic trust", L"manyetizma"},{ 93, 93, 93, 93, 93, 93,93,93,34,93,93,93,34},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_magnetic_trust,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(656),	20,	12,
  L"manyetizma",  L"!magnetic trust!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{ L"mass healing", L"toplu şifa"},	{ 93, 54, 93, 93,93,93,93,93,93 ,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_mass_healing,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(508),	100,	36,
	L"", L"!Mass Healing!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_HEALING
    },

    {
	{ L"mass invis", L"toplu görünmezlik"},		{ 43, 47, 93, 93,93,93,93,93 ,42,93,93,93,46},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_mass_invis,	TAR_IGNORE,		POS_STANDING,
	&gsn_mass_invis,	SLOT(69),	20,	24,
  L"", L"Görünmezliğin yitiyor.", L"", CABAL_NONE ,
	RACE_NONE,ALIGN_NONE, GROUP_ILLUSION
    },

    {
	{ L"mass sanctuary", L"toplu takdis"},{   93, 62, 93, 93,93,93,93,93,93 ,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_mass_sanctuary,	TAR_IGNORE,		POS_STANDING,
	NULL,		SLOT(589),	200,	24,
  L"", L"Beyaz auran yokoluyor.",
	L"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	{ L"master healing", L"yüksek tedavi"},	{  93,58,93,93,70,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_master_heal,	TAR_CHAR_DEFENSIVE,POS_FIGHTING,
	NULL,			SLOT(641),	300,	12,
	L"", L"!Master Heal!",  L"",CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_HEALING
    },

    {
	{ L"meld into stone", L"taşa dönüşme"},	{ 20, 20,20, 20,20,20,20,20 ,20,20,20,20,20},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_meld_into_stone,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(584),	12,	18,
  L"",  L"Derindeki taşlar kumlaşarak dökülüyor.",  L"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	{ L"mend", L"onar"},	{   50, 93, 93, 93,93,93,93,93,50 ,93,93,93,50},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_mend,		TAR_OBJ_INV,		POS_STANDING,
	&gsn_mend,		SLOT(590),	150,	24,
	L"", L"!mend!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_ENCHANTMENT
    },

    {
	{ L"mind light", L"zihin ışığı"},	{ 93, 83,93, 93,93,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_mind_light,	TAR_IGNORE,		POS_STANDING,
	NULL,		SLOT(82),	200,	24,
  L"",  L"Artık yeni odaları hızlandırabilirsin.",  L"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"mind wrack", L"zihin yosunu"},{ 93, 93, 93, 93, 93, 27, 93,93,93,93,93,93,93},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_mind_wrack,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(650),	20,	12,
  L"zihin yosunu",  L"!mind wrack!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{ L"mind wrench", L"zihin bükümü"},	{ 93, 93, 93, 93, 93, 40, 93,93,93,93,93,93,93},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_mind_wrench,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(651),	20,	12,
  L"zihin bükümü",  L"!mind wrench!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{ L"mist walk", L"gizemli yürüyüş"},		{  93,93,93,93,93,93,93,93,93,93,47,47,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_mist_walk,	TAR_IGNORE,	POS_FIGHTING,
	NULL,			SLOT(658),	80,	12,
	L"", L"!Mist Walk!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_TRANSPORTATION
    },

    {
	{ L"mummify", L"mumyala"},	{ 93, 93, 93, 93,93,93,93,93,93,93,93,44,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_mummify,	TAR_OBJ_CHAR_OFF,	POS_STANDING,
	NULL,		SLOT(715),	50,	12,
  L"",  L"Yeni ölülere hayat verebilirsin.", L"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"mysterious dream", L"gizemli düş"}, 	{ 93, 93,93,93,93,35,93,93,49,93,93,27,93},
		{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_mysterious_dream,	TAR_IGNORE,	POS_STANDING,
	&gsn_mysterious_dream,	SLOT(678),	200,	24,
	L"", L"!mysterous dream!",  L"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_BEGUILING
    },

    {
	{ L"nexus", L"neksus"},     { 93,93,93,93,93,93,93,93 ,60,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_nexus,            TAR_IGNORE,             POS_STANDING,
        NULL,                   SLOT(520),       150,   36,
	L"", L"!Nexus!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_TRANSPORTATION
    },

    {
	{ L"pass door", L"saydamlık"},		{  24, 32, 93, 93,93,93,93,93 ,24,24,93,27,24},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_pass_door,	TAR_CHAR_SELF,		POS_STANDING,
	&gsn_pass_door,			SLOT(74),	20,	12,
  L"", L"Cisimleşiyorsun.",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_TRANSPORTATION
    },

    {
	{ L"plague", L"veba"},		{  93, 93, 93, 93,93,46,93,93,33,93,93,36,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_plague,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_plague,		SLOT(503),	20,	12,
  L"veba", L"Hastalığın çürüyor.",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_MALADICTIONS
    },

    {
	{ L"polymorph", L"binbir şekil"},      { 93, 93, 93, 93,93, 93,93,93,73,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_polymorph,	TAR_IGNORE,	POS_STANDING,
	NULL,			SLOT(639),	250,	24,
  L"", L"Doğal ırkına dönüyorsun.", L"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_BENEDICTIONS
    },

    {
	{ L"portal", L"köprü"},                { 54, 93, 93, 93, 93,93,93,93,40,93,93,43,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_portal,           TAR_IGNORE,             POS_STANDING,
        NULL,                   SLOT(519),       100,     24,
	L"", L"!Portal!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_TRANSPORTATION
    },

    {
	{ L"protection cold", L"soğuktan korunma"},{   93, 44, 93, 93, 93,93,93,25, 93,93,93,50,44},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_protection_cold,	TAR_CHAR_SELF,	POS_STANDING,
	&gsn_protection_cold,		SLOT(600),	5,	12,
  L"", L"Daha az korunmasız hissediyorsun.",
	L"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	{ L"protection evil", L"kemden korunma"},	{ 93,  9, 93, 93,11,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_protection_evil,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(34), 	5,	12,
  L"", L"Daha az korunmasız hissediyorsun.",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	{ L"protection good", L"iyiden korunma"},{ 93,9,93,93,93,13,93,93,93,93,27,17,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_protection_good,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(666), 	5,	12,
  L"", L"Daha az korunmasız hissediyorsun.",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	{ L"protection heat", L"sıcaktan korunma"},	{ 93, 39, 93, 93,93,93,93,93, 93,93,93,48,39},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_protection_heat,	TAR_CHAR_SELF,	POS_STANDING,
	&gsn_protection_heat,		SLOT(599),	5,	12,
  L"", L"Daha az korunmasız hissediyorsun.",
	L"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	{ L"protection negative", L"negatiften korunma"},{   93, 93,93,93,93,93,93,93,93,93,93,15,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_protection_negative,	TAR_CHAR_SELF,	POS_STANDING,
	NULL,		SLOT(636),	20,	12,
  L"", L"Kendi saldırılarına karşı daha az korunmasız hissediyorsun.",
	L"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	{ L"protective shield", L"koruma kalkanı"},     { 26,38,93,93, 93,93,93,93 ,26,93,93,18,26},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_protective_shield,      TAR_CHAR_SELF,   POS_STANDING,
        &gsn_protective_shield,       SLOT(572),        70,       12,
        L"", L"Kalkanın sönüyor.", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	{ L"power word kill", L"ölüm sözcüğü"},		{   93, 93, 93, 93,93,93,93,93,93 ,93,93,78,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_power_kill,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
	NULL,		SLOT(604),	200,	18,
  L"ölüm sözcüğü", L"Dayanıklılığın artıyor.",
	L"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"power word stun", L"bayıltma sözcüğü"},{ 93, 93,93, 93,93, 62,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_power_stun,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_power_stun,		SLOT(625),	200,	24,
  L"",  L"Hareket edebilirsin.",  L"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"quantum spike", L"enerji dikeni"},	{ 41, 93, 93, 93, 93, 93,93,93,93,93,93,93,41},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_quantum_spike,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(657),	20,	12,
  L"enerji dikeni",  L"!quantum spike!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{ L"ranger staff", L"korucu asası"},          {  93, 93, 93, 93, 93, 93, 93, 32 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_ranger_staff,      TAR_IGNORE,          POS_FIGHTING,
        NULL,             SLOT(519),        75,       0,
	L"", L"!ranger staff!", L"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"ray of truth", L"ilahi enerji"},        { 93, 37, 93, 93, 45,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_ray_of_truth,     TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        &gsn_ray_of_truth,      SLOT(518),      20,     12,
        L"ilahi enerji", L"!Ray of Truth!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_BENEDICTIONS
    },

    {
	{ L"recharge", L"şarj"},		{ 29,93,93,93, 93, 93, 93, 93, 29,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_recharge,		TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(517),	60,	24,
	L"", L"!Recharge!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_ENCHANTMENT
    },

    {
	{ L"refresh", L"yenileme"},		{  93,7, 93,93, 93,93,93,93, 93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_refresh,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(81),	12,	18,
  L"yenileme", L"!Refresh!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_HEALING
    },

    {
	{ L"reincarnation", L"dirilme"},    {  93, 93, 93, 93, 93,93,93,93,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,        TAR_IGNORE,          POS_STANDING,
        NULL,             SLOT(668),        0,       0,
	L"", L"!!", L"",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"remove curse", L"lanet kaldır"},	{  93, 32, 93, 93,13,93,93,93,93,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_remove_curse,	TAR_OBJ_CHAR_DEF,	POS_STANDING,
	&gsn_remove_curse,	SLOT(35),	 5,	12,
	L"", L"!Remove Curse!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_CURATIVE
    },

    {
	{ L"remove fear", L"korku kaldır"},		{ 93, 34, 93,93,41,93,93,93,93,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_remove_fear, 	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,		SLOT(582),	5,	12,
	L"",  L"!Remove Fear!", L"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"remove tattoo", L"dövme kaldır"},    { 93, 93, 93,93,93,93,93,93,93,93,93,93,93 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_remove_tattoo, TAR_CHAR_DEFENSIVE,       POS_STANDING,
        NULL,       SLOT(552),        10,       0,
	L"", L"!remove tattoo!", L"",
        CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"resilience", L"esneklik"},	{  93,93, 93, 93,56,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_resilience,	TAR_CHAR_DEFENSIVE, POS_STANDING,
	NULL,			SLOT( 638),	 50,	12,
  L"",  L"Emici saldırılara karşı koruman azalıyor.", L"",
	CABAL_NONE ,RACE_NONE, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	{ L"restoring light", L"iyileştiren ışık"},   { 93, 71, 93,93,93,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_restoring_light,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(643),	50,	24,
	L"", L"!restoring light!", L"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_BENEDICTIONS
    },

    {
	{ L"sanctify lands", L"kutsal topraklar"},  { 93, 41, 93, 93,53, 93, 93, 93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_sanctify_lands,	TAR_IGNORE,	POS_STANDING,
	NULL,			SLOT(673),	200,	24,
	L"", L"!sanctify lands!",  L"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_BENEDICTIONS
    },

    {
	{ L"sanctuary", L"takdis"},			{ 93, 29, 93, 93,27,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_sanctuary,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_sanctuary,		SLOT(36),	75,	12,
  L"", L"Beyaz auran yokoluyor.",
	L"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	{ L"sand storm", L"kum fırtınası"},	{ 93, 93, 93, 93,93,93,93,93,93,93,93,93,51 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_sand_storm,	TAR_IGNORE,	POS_FIGHTING,
	&gsn_sand_storm,		SLOT(577),	200,	24,
  L"kum fırtınası",  L"Gözlerindeki kum dökülüyor.", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"scream", L"çığlık"},		{ 52, 93, 93,93,93,93,93,93,48,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_scream,	TAR_IGNORE,	POS_FIGHTING,
	&gsn_scream,		SLOT(578),	200,	24,
  L"çığlık",  L"Yeniden duyabiliyorsun.",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"severity force", L"arz kırımı"},	{ 93, 45,93,93,93, 93, 93, 93,93,93 ,93,93,41},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_severity_force,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(622),	20,	12,
  L"arz kırımı", L"!severity force!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"shield", L"kalkan"},	{ 20, 35, 93, 93,93,93,19,93,93 ,93,20,20,20},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_shield,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_shield,		SLOT(67),	12,	18,
  L"",  L"Güç kalkanın parlayarak yokoluyor.", L"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	{ L"shielding", L"kalkanlama"},		{   93, 93, 93, 93,93,93,93,93,53 ,93,93,53,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_shielding,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_shielding,		SLOT(591),	250,	12,
  L"", L"Gerçek Kaynak'tan uzaklaştığını hissediyorsun.",
	L"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"shocking grasp", L"şok kavrayışı"},		{ 8, 93, 93, 93,93,93,93,93 ,9,93,93,93,8},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_shocking_grasp,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(53),	15,	12,
  L"şok kavrayışı",  L"!Shocking Grasp!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{ L"shocking trap", L"şokçu tuzak"},	{ 37, 93,93, 93,93,93,93,93 ,40,93,93,37,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_shocking_trap,	TAR_IGNORE,	POS_STANDING,
	NULL,		SLOT(615),	150,	24,
  L"şokçu tuzak",  L"Daha fazla odayı şokla doldurabilirsin.", L"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"sleep", L"uyku"},	{ 93, 93, 93, 93,93,25,93,93,39,93,93,39,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_sleep,		TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_sleep,		SLOT(38),	15,	12,
  L"", L"Yorgunluğun geçiyor.",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_BEGUILING
    },

    {
	{ L"slow", L"yavaşlatma"},       { 28,38,93,93, 93, 28, 93, 93 ,23,93,28,29,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_slow,             TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        &gsn_slow,                   SLOT(515),      30,     12,
        L"", L"Hızlandığını hissediyorsun.",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_MALADICTIONS
    },

    {
	{ L"soften", L"yumuşatma"},		{ 93, 93, 93, 93,93,93,93,93 ,58,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_soften,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(713),	 75,	12,
  L"yumuşatma",  L"Derin sertleşiyor.",
	L"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_WEATHER
    },

    {
	{ L"solar flight", L"güneş uçuşu"},		{93,35,93,93,93,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_solar_flight,	TAR_IGNORE,	POS_FIGHTING,
	NULL,			SLOT(659),	80,	12,
	L"", L"!Solar Flight!",  L"",CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_TRANSPORTATION
    },

    {
	{ L"sonic resonance", L"ses direnci"},	{ 93, 93, 93, 93, 93, 93,93,93,93,93,93,28,93},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_sonic_resonance,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(649),	20,	12,
  L"ses direnci",  L"!sonic resonance!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{ L"soul bind", L"ruh bağı"},		{ 93, 93, 93, 93,93,39,93,93, 93,93,93,54,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_soul_bind,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
	NULL,		SLOT(716),	 5,	12,
  L"", L"Kendine güvenin artıyor.",  L"",
	CABAL_NONE, RACE_NONE, ALIGN_NONE, GROUP_BEGUILING
    },

    {
	{ L"spectral furor", L"hayali öfke"},	{ 93, 93, 93, 93, 93, 93,93,93,93,93,93,35,93},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_spectral_furor,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(646),	20,	12,
  L"hayali öfke",  L"!spectral furor!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{ L"stone golem", L"taş golem"},		{93,93,93,93,93,93,93,93,93,93,93,39,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_stone_golem,	TAR_IGNORE,		POS_STANDING,
	NULL,		SLOT(663),	300,		18,
  L"",  L"Yeni golemler yapmak için yeterli gücü topladın.", L"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"stone skin", L"taş deri"},	{ 25, 36, 93,93,93,93,35,93,93,93,60,30,25},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_stone_skin,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(66),	12,	18,
  L"", L"Derin yumuşuyor.",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	{ L"suffocate", L"boğma"},    {  93,93,93,93, 93,93,93,93, 48,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_suffocate,    TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
        NULL,             SLOT(714),        50,       12,
        L"boğma", L"Yeniden soluyabilirsin.", L"",CABAL_NONE,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"sulfurus spray", L"sülfür spreyi"},	{ 93, 93, 93, 93, 93,93,93,93,28,93,93,93,93},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_sulfurus_spray,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(652),	20,	12,
  L"sülfür spreyi",  L"!sulfurus spray!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{ L"summon", L"çağrı"},		{  23, 22, 93, 93,51,50,93,93,24,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_summon,		TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(40),	50,	12,
	L"", L"!Çağrı!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_TRANSPORTATION
    },

    {
	{ L"summon air elemental", L"hava uşağı"},  { 93, 93, 93, 93, 93, 93,93,93,93,93,93,93,42},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_summon_air_elm,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(696),	50,	12,
  L"",  L"Hava uşağı çağırabilecek gücü kazanıyorsun.",
	L"", 	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"summon earth elemental", L"toprak uşağı"},{ 93, 93, 93, 93, 93, 93,93,93,93,93,93,93,38},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_summon_earth_elm,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(693),	50,	12,
  L"",  L"Toprak uşağı çağırabilecek gücü kazanıyorsun.",
	L"", 	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"summon fire elemental", L"ateş uşağı"},{ 93, 93, 93, 93, 93, 93,93,93,93,93,93,93,58},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_summon_fire_elm,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(697),	50,	12,
  L"",  L"Ateş uşağı çağırabilecek gücü kazanıyorsun.",
	L"", 	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"summon lightning elemental", L"enerji uşağı"},{ 93,93,93,93,93, 93,93,93,93,93,93,93,68},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_summon_light_elm,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(710),	50,	12,
  L"", L"Enerji uşağı çağırabilecek gücü kazanıyorsun.",
	L"", 	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"summon water elemental", L"su uşağı"},{ 93, 93, 93, 93, 93, 93,93,93,93,93,93,93,48},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_summon_water_elm,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(698),	50,	12,
  L"",  L"Su uşağı çağırabilecek gücü kazanıyorsun.",
	L"", 	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"summon shadow", L"gölge çağrısı"},{ 93, 93,93,93,93,93,93,93,93,93,93,50,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_summon_shadow,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,		SLOT(620),	200,	24,
  L"",  L"Yeni gölgeler çağırabilirsin.",  L"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"superior heal", L"yüksek şifa"},{  93, 38, 93,93,43,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_super_heal,	TAR_CHAR_DEFENSIVE,POS_FIGHTING,
	NULL,			SLOT(640),	100,	12,
	L"", L"!Super Heal!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_HEALING
    },

    {
	{ L"tattoo", L"dövme"},    { 93, 93, 93,93,93,93,93,93,93,93,93,93,93},
	{1,1,1,1,1,1,1,1,1, 1,1,1,1},
        spell_tattoo, TAR_CHAR_DEFENSIVE,       POS_STANDING,
        NULL,       SLOT(551),        10,       0,
	L"", L"!tattoo!", L"",CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"teleport", L"nakil"},		{  22, 32, 93, 93,93,7,93,93 ,23,93,93,16,16},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_teleport,		TAR_CHAR_SELF,		POS_FIGHTING,
	&gsn_teleport,	 		SLOT( 2),	35,	12,
	L"", L"!Teleport!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_TRANSPORTATION
    },

    {
	{ L"transfer object", L"nesne nakli"},	{ 62, 93, 93, 93, 93, 93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_transfer_object,	TAR_IGNORE,		POS_STANDING,
	NULL,		SLOT(708),	40,	12,
	L"", L"!transfer object!", L"",
 	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"tsunami", L"tsunami"},{ 93, 93, 93, 93, 93, 93,93,93,93,93,93,93,62},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_tsunami,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(701),	50,	12,
	L"raging tidal wave",  L"!tsunami!",  L"", 	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"turn", L"devir"},		{   93, 50, 93, 93,60,93,93,93,93 ,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_turn,	TAR_IGNORE,	POS_FIGHTING,
	NULL,		SLOT(597),	50,	12,
  L"",  L"Devir büyüsünü yeniden kullanabilirsin.",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"vampiric blast", L"yarasa öfkesi"},	{ 93, 93, 93, 93, 93, 93, 93, 93,93,93 ,38,93,93},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_vam_blast,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(611),	20,	12,
  L"yarasa öfkesi",  L"!Vampiric Blast!", L"",CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"ventriloquate", L"vantrolog"},	{   1, 93, 93, 93,93,93,93,93, 1,93,93,93,3},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_ventriloquate,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(41),	 5,	12,
	L"", L"!Ventriloquate!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_ILLUSION
    },

    {
	{ L"web", L"ağ"},		{   93, 93, 93, 93,93,93,93,93,69 ,93,93,58,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_web,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_web,		SLOT(592),	50,	12,
	L"", L"Seni saran ağlar eriyor.",
	L"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_ATTACK
    },

    {
	{ L"windwall", L"rüzgar duvarı"},		{ 93, 93, 93, 93, 93, 93,93,93,93,93,93,93,47},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_windwall,		TAR_IGNORE,		POS_FIGHTING,
	NULL,			SLOT(694),	20,	12,
  L"rüzgar duvarı", L"Gözlerin iyileşiyor.",  L"",
 	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	{ L"witch curse", L"cadı laneti"},	{   93, 93, 93, 93,93,93,93,93,93 ,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_witch_curse,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_witch_curse,		SLOT(602),	150,	24,
  L"", L"Dayanıklılığın artıyor.",
	L"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"wrath", L"gazap"},         { 93, 93, 93,93,34,93,93,93,93,93,93,93,93 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_wrath, TAR_CHAR_OFFENSIVE,       POS_STANDING,
	&gsn_wrath,       SLOT(553),        20,       12,
  L"gazap", L"Lanet yokoluyor.", L"",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_BENEDICTIONS
    },

    {
	{ L"weaken", L"zayıflatma"},	{   93, 24, 93, 93,93,93,93,93 ,19,93,16,29,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_weaken,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_weaken,		SLOT(68),	20,	12,
  L"zayıflatma", L"Güçlendiğini hissediyorsun.",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_MALADICTIONS
    },

    {
	{ L"word of recall", L"anımsama sözcüğü"},		{ 32, 28, 93, 93,32,93,29,93 ,32,38,10,10,32},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_word_of_recall,	TAR_CHAR_SELF,		POS_RESTING,
	NULL,			SLOT(42),	 5,	12,
	L"", L"!Word of Recall!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_TRANSPORTATION
    },

/*
 * Dragon breath
 */

    {
	{ L"acid breath", L"asit nefesi"},		{  67, 67, 67, 67,67,67,67,67 ,67,67,67,67,67},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_acid_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_acid_breath,		SLOT(200),	100,	24,
  L"asit nefesi",  L"!Acid Breath!",  L"", CABAL_NONE ,
	RACE_TOP, ALIGN_NONE, GROUP_DRACONIAN
    },

    {
	{ L"desert heat", L"çöl sıcağı"},		{ 69, 69, 69, 69,69,69,69,69,69,69,69,69,69 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(629),	200,	24,
  L"çöl sıcağı", L"Gözlerindeki duman yokoluyor.",  L"", CABAL_NONE ,
	RACE_TOP, ALIGN_NONE, GROUP_DRACONIAN
    },

    {
	{ L"fire breath", L"ateş nefesi"},	{ 69, 69, 69, 69,69,69,69,69,69,69,69,69,69 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_fire_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_fire_breath,		SLOT(201),	200,	24,
  L"ateş nefesi",  L"Gözlerindeki duman yokoluyor.",  L"", CABAL_NONE ,
	RACE_TOP, ALIGN_NONE, GROUP_DRACONIAN
    },

    {
	{ L"frost breath", L"buz nefesi"},			{ 61,61,61,61,61,61,61,61,61,61,61,61,61  },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_frost_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_frost_breath,		SLOT(202),	125,	24,
  L"buz nefesi",  L"!Frost Breath!",  L"", CABAL_NONE ,
	RACE_TOP, ALIGN_NONE, GROUP_DRACONIAN
    },

    {
	{ L"gas breath", L"gaz nefesi"},		{ 70,70,70,70,70,70,70,70,70,70,70,70,70  },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_gas_breath,	TAR_IGNORE,		POS_FIGHTING,
	&gsn_gas_breath,	SLOT(203),	175,	24,
  L"gaz nefesi", L"!Gas Breath!", L"", CABAL_NONE ,
	RACE_TOP, ALIGN_NONE, GROUP_DRACONIAN
    },

    {
	{ L"lightning breath", L"şimşek nefesi"},	{ 64,64,64,64,64,64,64,64,64,64,64,64,64  },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_lightning_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_lightning_breath,		SLOT(204),	150,	24,
  L"şimşek nefesi",  L"!Lightning Breath!",  L"", CABAL_NONE ,
	RACE_TOP, ALIGN_NONE, GROUP_DRACONIAN
    },

    {
	{ L"lightning stroke", L"çelik darbe"},		{ 69, 69, 69, 69,69,69,69,69,69,69,69,69,69 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(632),	200,	24,
  L"çelik darbe",  L"!lightning stroke!",  L"", CABAL_NONE ,
	RACE_TOP, ALIGN_NONE, GROUP_DRACONIAN
    },

    {
   { L"luck bonus", L"şans bonusu"},{ 67, 67, 67, 67,67,67,67,67,67,67,67,67,67 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,		SLOT( 630),	 20,	12,
  L"", L"Büyüye karşı zırhının azaldığını hissediyorsun.",  L"", CABAL_NONE,
	RACE_TOP, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	{ L"paralyzation", L"felç"},	{ 69, 69, 69, 69,69,69,69,69,69,69,69,69,69 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_IGNORE,	POS_FIGHTING,
	NULL,		SLOT(631),	200,	24,
  L"felç",  L"Yeniden hareket edebiliyorsun.",  L"", CABAL_NONE ,
	RACE_TOP, ALIGN_NONE, GROUP_DRACONIAN
    },

    {
	{ L"repulsion", L"repulsion"},		{ 65, 65, 65, 65,65,65,65,65,65,65,65,65,65 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(633),	200,	24,
	L"repulsion",  L"!repulsion!",  L"", CABAL_NONE ,
	RACE_TOP, ALIGN_NONE, GROUP_DRACONIAN
    },

    {
	{ L"sleep gas", L"uyku gazı"},	{ 67, 67, 67, 67,67,67,67,67,67,67,67,67,67 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_IGNORE,	POS_FIGHTING,
	NULL,		SLOT(628),	200,	24,
  L"uyku gazı",  L"Kendini kurumuş hissediyorsun.",  L"", CABAL_NONE ,
	RACE_TOP, ALIGN_NONE, GROUP_DRACONIAN
    },

    {
	{ L"slow gas", L"slow gas"},{ 69, 69, 69, 69,69,69,69,69,69,69,69,69,69 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(634),	200,	24,
  L"slow gas",  L"Eski hızını kazanıyorsun.",  L"", CABAL_NONE ,
	RACE_TOP, ALIGN_NONE, GROUP_DRACONIAN
    },

/*
 * Skills for mobiles. (general purpose and high explosive from
 * Glop/Erkenbrand
 */
    {
	{ L"crush", L"baskı"},	{93,93,93,93,93,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_crush,            	SLOT( 0),       0,      18,
        L"baskı", L"!crush!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_FIGHTMASTER
    },

    {
	{ L"general purpose", L"genel amaç"},       { 93, 93, 93, 93, 93, 93, 93, 93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_general_purpose,  TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,                   SLOT(401),      0,      12,
	L"genel amaç", L"!General Purpose Ammo!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"high explosive", L"yüksek patlayıcı"},       { 93, 93, 93, 93, 93, 93, 93, 93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_high_explosive,   TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,                   SLOT(402),      0,      12,
        L"yüksek patlayıcı",  L"!High Explosive Ammo!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"tail", L"kuyruk"},		{  53, 53,53, 53,53,53,53,53,53 ,53,53,53,53},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_tail,            	SLOT( 0),       0,      18,
        L"kuyruk", L"!Tail!", L"", CABAL_NONE ,
	RACE_TOP, ALIGN_NONE, GROUP_FIGHTMASTER
    },

/* combat and weapons skills */

    {
	{ L"arrow", L"ok"},		{ 93, 93,93, 93,93,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_IGNORE,	POS_STANDING,
	&gsn_arrow,	SLOT(0),	0,	0,
  L"ok",  L"!arrow!",  L"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_WEAPONSMASTER
    },

    {
	{ L"axe", L"balta"},		{  93, 93, 93,  1,93,1,93,1 ,93 ,93,93,93,93},
	{ 2, 2, 1, 1,1,1,1,1,1, 1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_axe,            	SLOT( 0),       0,      0,
	L"", L"!Axe!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_WEAPONSMASTER
    },

    {
	{ L"bow", L"yay"},{ 93, 93,93, 44,93,93,93,35 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_IGNORE,	POS_STANDING,
	&gsn_bow,		SLOT(0),	0,	12,
  L"yay", L"!bow!",  L"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_WEAPONSMASTER
    },

    {
	{ L"dagger", L"hançer"},             { 1,  1,  1,  1,1,1,1,1 ,1 ,1,1,1,1},
	{ 2, 2, 1, 1,1,1,1,1,1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_dagger,            SLOT( 0),       0,      0,
	L"", L"!Dagger!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_WEAPONSMASTER
    },

    {
	{ L"flail", L"döven"},		{ 93,  1, 93,  1,93,1,93,93 ,93,93,93,93,1},
	{ 2, 2, 1, 1,1,1,1,1,1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_flail,            	SLOT( 0),       0,      0,
	L"", L"!Flail!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_WEAPONSMASTER
    },

    {
	{ L"lance", L"kargı"},	{ 33,33,33,33,33,33,33,33,33,33,33,33,33},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_IGNORE,	POS_STANDING,
	&gsn_lance,	SLOT(0),	0,	0,
	L"lance",  L"!lance!",  L"",
	CABAL_KNIGHT , RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	{ L"mace", L"topuz"},			{  93,  1,  1,  1,1,1,93,93 ,93 ,1,93,93,93},
	{ 2, 2, 1, 1,1,1,1,1,1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_mace,            	SLOT( 0),       0,      0,
	L"", L"!Mace!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_WEAPONSMASTER
    },

    {
	{ L"polearm", L"teber"},		{ 93, 93, 93,  1,1,1,93,93,93 ,93,93,93,93},
	{ 2, 2, 1, 1,1,1,1,1,1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_polearm,           SLOT( 0),       0,      0,
	L"", L"!Polearm!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_WEAPONSMASTER
    },

    {
	{ L"shield block", L"kalkan bloğu"},		{ 93,17,7,1,1,1,12,10,93 ,10,93,93,93},
	{ 2, 1, 1, 1,1,1,1,1,1,1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_shield_block,	SLOT(0),	0,	0,
	L"", L"!Shield!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_DEFENSIVE
    },

    {
	{ L"spear", L"mızrak"},			{  93,  93,  1,  1,93,1,1,1 ,93,1,8,93,93 },
	{ 2, 2, 1, 1,1,1,1,1,1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_spear,            	SLOT( 0),       0,      12,
        L"mızrak", L"!Spear!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_WEAPONSMASTER
    },

    {
	{ L"sword", L"kılıç"},		{  93, 1,  1,  1,1,1,1,1,93 ,1,1,93,93},
	{ 2, 2, 1, 1,1,1,1,1,1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_sword,            	SLOT( 0),       0,      0,
	L"", L"!sword!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_WEAPONSMASTER
    },

    {
	{ L"whip", L"kırbaç"},		{  1, 1,  93,  1,93,1,93,1 ,1,93,1,1,1},
	{ 2, 2, 1, 1,1,1,1,1,1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_whip,            	SLOT( 0),       0,      0,
	L"", L"!Whip!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_WEAPONSMASTER
    },

    {
	{ L"second weapon", L"ikinci silah"},	{  93,93,93,23,48,48,93,23,93,24,93,93,93},
	{ 2, 2, 1, 1,1,1,1,1,1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_second_weapon,            	SLOT( 0),       0,      0,
	L"",  L"!second weapon!",  L"",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_WEAPONSMASTER
    },

    {
	{ L"ambush", L"pusu"},         {  93, 93, 93, 93, 93, 93, 93, 17,93 ,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,        TAR_CHAR_OFFENSIVE,          POS_STANDING,
        &gsn_ambush,                    SLOT(0),        0,       12,
        L"pusu", L"!Ambush!", L"",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"area attack", L"alan saldırısı"},      {93, 93, 93, 73,93,93,93,93,93,69,93,93,93},
	{1,1,1,1,1,1,1,1,1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_area_attack,     SLOT(0),        0,      0,
	L"", L"!Area Attack!", L"",	CABAL_NONE,
	RACE_NONE, ALIGN_NONE, GROUP_FIGHTMASTER
    },

    {
	{ L"assassinate", L"suikast"},       {  93, 93, 93, 93, 93, 93, 65, 93 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_IGNORE,          POS_STANDING,
        &gsn_assassinate,       SLOT(0),        0,       18,
        L"suikast", L"!assassinate!", L"",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"backstab", L"ardılan"},          {  93, 93,  11, 93,93,93,93,93 ,93,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_backstab,          SLOT( 0),        0,     12,
        L"ardılan", L"!Backstab!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"bash", L"omuz"},		{  93,93,93,1, 6,1,1,10 ,93 ,93,11,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_bash,            	SLOT( 0),       0,      18,
        L"omuz", L"!Bash!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_FIGHTMASTER
    },

    {
	{ L"bash door", L"kapıkırma"},	{  93, 93, 93,  1,6,1,1,93 ,93,93,11,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_bash_door,        	SLOT( 0),       0,      18,
        L"kapıkırma", L"!Bash Door!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_FIGHTMASTER
    },

    {
	{ L"bearcall", L"ayı çağırma"},      {  93, 93, 93, 93, 93, 93, 93, 31,93,93,93,93,93 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,		TAR_IGNORE,          POS_FIGHTING,
        &gsn_bear_call,		SLOT(518),	50,       0,
        L"", L"Yeni ayılarla başa çıkabilirsin.", L"",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"berserk", L"vahşileşme"},		{ 93, 93, 93, 18,93,93,93,93 ,93,20,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_berserk,        	SLOT( 0),       0,      24,
        L"", L"Nabzının yavaşladığını hissediyorsun.",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"blackguard", L"karagard"},{  93, 93, 93, 93, 93, 93, 93, 93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_CHAR_OFFENSIVE,   POS_STANDING,
	&gsn_blackguard,		SLOT(0),    0,       12,
  L"karagard", L"Karagardın yokoluyor.", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"blackjack", L"copla"},	{  93, 93, 30, 93, 93, 93, 93, 93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_CHAR_OFFENSIVE,   POS_STANDING,
	&gsn_blackjack,		SLOT(0),    0,       8,
  L"copla", L"Kafandaki ağrı geçiyor.", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"blind fighting", L"kör dövüşü"},	{   93, 93, 93, 13,32,93,11,43,93 ,23,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_blind_fighting,		SLOT(0),	0,	0,
	L"", L"!blind fighting!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_FIGHTMASTER
    },

    {
	{ L"blindness dust", L"körlük tozu"},       {  93, 93, 93, 93, 93, 93, 28, 93,93,93,93,93,93 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,   		TAR_IGNORE,             POS_FIGHTING,
        &gsn_blindness_dust,    SLOT(0),        20,       18,
	L"", L"!blindness dust!", L"",	 CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"blink", L"zayi"},            {  1, 93,  93, 93,93,93,93,93 ,1,93,93,1,1},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_blink,             SLOT( 0),        0,     0,
	L"", L"!Blink!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"butcher", L"kasap"},       {  93, 93, 93, 93, 93, 93, 93, 13 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,        TAR_IGNORE,          POS_STANDING,
        &gsn_butcher,             SLOT(0),        0,       0,
	L"", L"!butcher!", L"",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"caltraps", L"çivi"},        {  93, 93, 93, 93, 93, 93, 18, 93 ,93,36,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_IGNORE,          POS_FIGHTING,
        &gsn_caltraps,       SLOT(0),        0,       18,
        L"çivi", L"Bacaklarındaki acı sönüyor.", L"",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"camouflage", L"kamuflaj"},       {  93, 93, 93, 93, 93, 93, 93, 4 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,        TAR_IGNORE,           POS_STANDING,
        &gsn_camouflage,         SLOT(0),        0,       24,
	L"", L"!Camouflage!", L"",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"camouflage move", L"kamufle hareket"},	{ 93, 93, 93, 93, 93,93,93,23, 93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_move_camf,		SLOT(0),	0,	12,
	L"", L"!move camouflaged!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"camp", L"kamp"},			{  93, 93, 93,  93,93,93,93,48 ,93 ,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_camp,            	SLOT( 0),       0,      24,
        L"kamp",          L"Yeni kamplar kurabilirsin.", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"circle", L"çevrel"},          {  93, 93, 7, 93, 93, 93, 93, 93,93 ,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,        TAR_CHAR_OFFENSIVE,          POS_FIGHTING,
        &gsn_circle,             SLOT(0),        0,       18,
        L"çevrel", L"!Circle!", L"",		CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"control animal", L"hayvan kontrolü"},	{ 93, 93, 93, 93,93,93,93,93,93,93,43,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_control_animal,	SLOT( 0),	 5,	12,
  L"", L"Kendine güvenin artıyor.",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_BEGUILING
    },

    {
	{ L"cleave", L"yarma"},         {  93, 93, 93, 93, 93, 55, 93, 93 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,     TAR_CHAR_OFFENSIVE,     POS_STANDING,
        &gsn_cleave,         SLOT(0),        0,       24,
        L"yarma", L"!Cleave!", L"",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"concentrate", L"konsantrasyon"},        { 93, 93,93,59,93,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_concentrate,        SLOT( 0),        0,     18,
        L"", L"Yeni dövüşlere konsantre olabilirsin.",  L"",CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_FIGHTMASTER
    },

    {
	{ L"counter", L"kontra"},     {  93, 93, 93, 28, 35, 93, 93, 93 ,93,28,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,        TAR_IGNORE,           POS_STANDING,
        &gsn_counter,         SLOT(0),        0,       0,
	L"", L"!Counter!", L"",		CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_FIGHTMASTER
    },

    {
	{ L"critical strike", L"kritik vuruş"},	{   93, 93, 93, 93,93,93,58,93,93 ,62,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_critical,			SLOT(0),	0,	18,
	L"", L"!critical strike!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"cross block", L"çapraz blok"},       {  93,93,93,31,56,56,93,31,93,32,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_cross_block,       SLOT( 0),        0,     0,
	L"", L"!cross block!",  L"", CABAL_NONE ,
	RACE_NONE,ALIGN_NONE, GROUP_DEFENSIVE
    },

    {
	{ L"detect hidden", L"saklıyı saptama"},	{ 93, 93, 93, 93, 93, 93, 93, 93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_detect_hidden,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,		SLOT(44),	 5,	12,
  L"", L"Saklıyı saptaman sönüyor.",
	L"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_DETECTION
    },

    {
	{ L"detect sneak", L"süzüleni saptama"},		{   93, 93, 93, 93,93,93,93,93,93 ,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_CHAR_SELF,		POS_STANDING,
	&gsn_detect_sneak,		SLOT(0),	20,	18,
	L"", L"!detect sneak!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_DETECTION
    },

    {
	{ L"dirt kicking", L"toz atma"},	{ 93, 93,  3,  8,93,3,3,5,93 ,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_dirt,		SLOT( 0),	0,	12,
  L"toz atma", L"Gözlerindeki toz dökülüyor.",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"disarm", L"silahsızlandır"},              {  93, 93, 28, 20,20,21,23,21,93,21,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_disarm,            SLOT( 0),        0,     18,
	L"", L"!Disarm!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_WEAPONSMASTER
    },

    {
	{ L"dodge", L"sıyrılma"},           {  33,93,1,13, 21,21,1,9, 93,15,8,93,27},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_dodge,             SLOT( 0),        0,     0,
	L"", L"!Dodge!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_DEFENSIVE
    },

   {
	{ L"dual backstab", L"çift ardılan"},       {  93, 93, 38, 93, 93, 93, 93, 93 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,             TAR_CHAR_OFFENSIVE,           POS_STANDING,
        &gsn_dual_backstab,       SLOT(0),        0,       0,
        L"çift ardılan", L"!dual backstab!", L"",		CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"enchant sword", L"kılıç yücelt"},		{ 93, 93, 93, 93,93,93,93,93,93,42,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_OBJ_INV,		POS_STANDING,
	&gsn_enchant_sword,	SLOT(0),	100,	24,
	L"",  L"!Enchant sword!",  L"", CABAL_NONE , RACE_NONE,
	ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"endure", L"dayanım"},          {  93, 93, 93, 93, 93, 93, 20, 93 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,        TAR_CHAR_SELF,          POS_FIGHTING,
        &gsn_endure,             SLOT(0),        0,       24,
        L"", L"Büyüye karşı dayanımın azalıyor.", L"",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"enhanced damage", L"yüksek zarar"},      {  93, 33, 25,  3,6,3,16,15 ,93,8,14,93,93},
	{ 1,  2,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_enhanced_damage,   SLOT( 0),        0,     0,
	L"", L"!Enhanced Damage!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_FIGHTMASTER
    },

    {
	{ L"entangle", L"sarmaşık"},        {  93,93,93,93,93,93,93,26 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_entangle,      TAR_CHAR_OFFENSIVE,   POS_FIGHTING,
        &gsn_entangle,       SLOT(568),        40,       12,
        L"sarmaşık", L"Sarmaşıklardan kurtulduğunu hissediyorsun.", L"",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"envenom", L"sem"},		{ 93, 93, 15, 93, 93, 93, 93, 93,93,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,	  	POS_RESTING,
	&gsn_envenom,		SLOT(0),	0,	12,
	L"", L"!Envenom!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"escape", L"sıvış"},		{93,93,45,93,93,93,55,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_escape,		SLOT( 0),	0,	24,
	L"",  L"!escape!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"explode", L"patlat"},      {  93, 93, 93, 93, 93, 93, 93, 93 ,93,46,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_CHAR_OFFENSIVE,          POS_FIGHTING,
        &gsn_explode,       SLOT(0),        100,       24,
        L"patlat", L"Gözlerindeki duman yokoluyor.", L"",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
{ L"ground strike", L"zemin vuruşu"},	{ 93,93,93,93, 93,93,93,52, 93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_ground_strike,	SLOT(0),	0,	18,
	L"", L"!ground strike!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"hand block", L"el bloğu"},            { 93,93,93,93,93,93,22,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_hand_block,             SLOT( 0),        0,     0,
	L"", L"!hand block!", L"", CABAL_NONE ,
	RACE_NONE,ALIGN_NONE, GROUP_DEFENSIVE
    },

    {
	{ L"hand to hand", L"çıplak el"},		{ 45,  30, 18, 9,11,18,3,24,45,12,19,45,35 },
	{ 2,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_hand_to_hand,	SLOT( 0),	0,	0,
	L"", L"!Hand to Hand!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_FIGHTMASTER
    },

    {
	{ L"harakiri", L"harakiri"},	{ 93, 93, 93, 93,93,93,93,93,93,25,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null, 	TAR_IGNORE,	POS_FIGHTING,
	&gsn_hara_kiri,		SLOT(0),	50,	12,
  L"",  L"Yaşamaya yeniden başladığını hissediyorsun.", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"headguard", L"kafakoruma"},	{  93, 93, 93, 93, 93, 93, 93, 93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_CHAR_OFFENSIVE,   POS_STANDING,
	&gsn_headguard,		SLOT(0),    0,       12,
  L"kafakoruma", L"Kafakoruman yokoluyor.", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"herbs", L"bitki"},       {  93, 93, 93, 93, 93, 93, 93, 18 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,        TAR_CHAR_DEFENSIVE,          POS_STANDING,
        &gsn_herbs,             SLOT(0),        0,       30,
        L"", L"Yine bitkilerle uğraşabilirsin.", L"",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"kick", L"tekme"},           {  93, 12, 14,  5,93,9,1,8 ,93,2,4,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,             TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        &gsn_kick,              SLOT( 0),        0,     12,
        L"tekme", L"!Tekme!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_FIGHTMASTER
    },

    {
	{ L"lash", L"kırbaçla"},			{ 48,48,93,93, 48,93,48,93, 48,93,48,48,48},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_lash,            	SLOT( 0),       0,      4,
        L"kırbaç", L"!Lash!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_FIGHTMASTER
    },

    {
	{ L"light resistance", L"ışık direnci"},	{  93,  93, 93, 93,93,93,93,93,93,93,50,93,93},
	{ 1,  1,  2,  2, 1, 1, 1, 1,1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_SLEEPING,
	&gsn_light_res,		SLOT( 0),	0,	0,
	L"", L"Light Resistance", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"lion call", L"aslan çağırma"},	{ 93, 93, 93, 93,93,93,93,55,93,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null, 	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	&gsn_lion_call,		SLOT(0),	50,	12,
	L"", L"!lion call!", 		L"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"make arrow", L"ok yapma"},		{ 93, 93, 93,93,93,93,93,37,93,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_make_arrow,	SLOT(0),	 50,	24,
	L"", L"!make arrow!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"make bow", L"yay yapma"},		{ 93, 93, 93,93,93,93,93,36,93,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_make_bow,		SLOT(0),	 200,	24,
	L"", L"!make bow!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"mental attack", L"zihin saldırısı"},{ 93, 93,93, 93,93,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_CHAR_SELF,		POS_STANDING,
	&gsn_mental_attack,		SLOT(0),	200,	24,
	L"",  L"!mental attack!",  L"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"neckguard", L"boyunkoruma"},	{  93, 93, 93, 93, 93, 93, 93, 93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_CHAR_OFFENSIVE,   POS_STANDING,
	&gsn_neckguard,		SLOT(0),    0,       12,
  L"boyun koruma", L"Boyun koruman yokoluyor.", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"nerve", L"sinir"},       {  93, 93, 93, 93, 93, 93, 29, 93 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,        TAR_CHAR_OFFENSIVE,          POS_FIGHTING,
        &gsn_nerve,             SLOT(0),        0,       18,
        L"", L"Sinirlerin iyileşiyor.", L"",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"parry", L"karşılama"},              { 93,25,19,1, 3,5,93,3, 23,1,22,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_parry,             SLOT( 0),        0,     0,
	L"", L"!Parry!", L"", CABAL_NONE ,
	RACE_NONE,ALIGN_NONE, GROUP_DEFENSIVE
    },

    {
	{ L"perception", L"algı"},  {  93, 93, 23, 93, 93,93,93,93,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,        TAR_IGNORE,          POS_STANDING,
        &gsn_perception,             SLOT(0),        0,       0,
	L"", L"!perception!", L"",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"push", L"itme"},			{  93, 93, 53,  93,93,93,93,93 ,93 ,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_push,            	SLOT( 0),       0,      18,
        L"itme",          L"!push!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"rescue", L"kurtar"},               {  93, 93, 93,  1,1,93,93,93,93,23,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_rescue,            SLOT( 0),        0,     12,
	L"", L"!Rescue!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_DEFENSIVE
    },

    {
	{ L"sense life", L"yaşam sezme"},{ 93, 93,93, 93,93,93,93,93 ,93,93,13,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_CHAR_SELF,		POS_STANDING,
	&gsn_sense_life,	SLOT(623),	20,	12,
  L"",  L"Yaşam sezme gücünü yitiriyorsun.",  L"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"settraps", L"tuzak"},	{ 93, 93, 59, 93,93,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_CHAR_SELF,		POS_STANDING,
	&gsn_settraps,		SLOT(0),	200,	24,
  L"tuzak",  L"Yeni tuzaklar kurabilirsin.",  L"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"shield cleave", L"kalkan yarma"},{ 93, 93, 33, 26, 29, 29, 31, 35, 93, 26, 39, 93, 93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_CHAR_SELF,		POS_STANDING,
	&gsn_shield_cleave,		SLOT(0),	200,	24,
	L"",  L"!shield cleave!",  L"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"smithing", L"demirci"},		{   93, 93, 93, 50,93,93,93,93,93 ,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_smithing,			SLOT(0),	10,	18,
	L"", L"!smithing!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"spell craft", L"büyü tanıma"},			{ 10,30,93,93, 93,93,93,93, 10,93,93,10,10 },
	{ 1,  1,  1,  1, 1, 1, 2, 2, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_spell_craft,	SLOT(0),    0,       0,
  L"büyü tanıma", L"!spell craft!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"strangle", L"bayıltma"},          {  93,  93,  93,  93,93,93,26,93 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_CHAR_OFFENSIVE,          POS_STANDING,
        &gsn_strangle,       SLOT(0),        0,       8,
        L"bayıltma", L"Boynunu yeniden hareket ettirebiliyorsun.", L"",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"swimming", L"yüzme"},	{   93, 93, 93, 93,93,93,93,93,93 ,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_swimming,		SLOT(0),	0,	0,
	L"", L"!swimming!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"target", L"hedef"},                 {  93, 93, 93,  93,93,93,93,93 ,93,40,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,             TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        &gsn_target,              SLOT( 0),        0,     12,
        L"hedef", L"!Kick!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"throw", L"fırlatma"},        {  93, 93, 93, 93, 93, 93, 25, 93 ,93,37,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_CHAR_OFFENSIVE,       POS_FIGHTING,
        &gsn_throw,       SLOT(0),        0,       18,
        L"fırlatma", L"!throw!", L"",	CABAL_NONE , RACE_NONE,ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"tiger power", L"kaplan gücü"},		{ 93,93,93,93, 93,93,93,61, 93,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null, 	TAR_IGNORE,	POS_FIGHTING,
	&gsn_tiger_power,	SLOT(0),	0,	12,
  L"",  L"Kaplanlarının kaçtığını hissediyorsun.", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"track", L"iz"},       {  93, 93, 93,93,93,93,93,39,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_IGNORE,       POS_STANDING,
        &gsn_track,       SLOT(0),        0,       12,
	L"", L"!track!", L"",CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"trip", L"çelme"},		{  93, 93,  3, 25,93,10,8,93 ,93,20,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_trip,		SLOT( 0),	0,	12,
  L"çelme", L"!Trip!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_FIGHTMASTER
    },

    {
	{ L"vampire", L"vampir"},			{ 93, 93, 93, 93,93,93,93,93,93,93,10,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_IGNORE,		POS_STANDING,
	&gsn_vampire,		SLOT(0),	100,	12,
  L"",  L"Doğallaşıyorsun.",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"vampiric bite", L"vampir ısırığı"},          {  93, 93,  93, 93,93,93,93,93 ,93,93,10,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_vampiric_bite,          SLOT( 0),        0,     12,
        L"vampir ısırığı", L"!vampiric bite!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"vampiric touch", L"vampir teması"},      {  93,  93,  93,  93,93,93,93,93 ,93,93,15,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,      TAR_CHAR_OFFENSIVE,          POS_STANDING,
	&gsn_vampiric_touch,       SLOT(0),        0,       12,
  L"vampir teması", L"Kabuslardan uyanıyorsun.", L"",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"vanish", L"yokol"},        {  93, 93, 93, 93, 93, 93, 18, 93 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_CHAR_SELF,          POS_FIGHTING,
        &gsn_vanish,       SLOT(521),        25,       18,
	L"", L"!vanish!", L"", 	CABAL_NONE , RACE_NONE,ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"warcry", L"nara"},       {  93, 93, 93,31,93,93,93,35,93 ,30,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_IGNORE,       POS_FIGHTING,
        &gsn_warcry,       SLOT(0),        30,       12,
        L"", L"Naranın etkisi yokoluyor.", L"",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"weapon cleave", L"silah yarma"}, { 93, 93, 37, 29, 34, 34, 35, 38, 93, 29, 44, 93, 93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_CHAR_SELF,		POS_STANDING,
	&gsn_weapon_cleave,	SLOT(0),	200,	24,
	L"",  L"!weapon cleave!",  L"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
       { L"second attack", L"ikili hamle"},        {  34, 28, 18,  10,13,14,15,13,25,10,12,34,34},
	{ 2,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_second_attack,     SLOT( 0),        0,     0,
	L"", L"!Second Attack!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_FIGHTMASTER
    },

    {
	{ L"secondary attack", L"ikincil hamle"},	{93,93,93,52, 61,61,93,58 ,93,55,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_CHAR_SELF,		POS_STANDING,
	&gsn_secondary_attack,	SLOT(0),	200,	24,
	L"",  L"!secondary attack!",  L"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"third attack", L"üçlü hamle"},        { 93, 93,36,19,27,26,26,45,93,20,32,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_third_attack,      SLOT( 0),        0,     0,
	L"", L"!Third Attack!",  L"", CABAL_NONE ,
	RACE_NONE,ALIGN_NONE, GROUP_FIGHTMASTER
    },

    {
	{ L"fourth attack", L"dörtlü hamle"},       {93,93,93,30, 93,93,48,93, 93,93,93,93,93},
	{1,1,1,1,1,1,1,1,1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_fourth_attack,     SLOT(0),        0,      0,
	L"", L"!Fourth Attack!", L"",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_FIGHTMASTER
    },

    {
	{ L"fifth attack", L"beşli hamle"},         {93, 93, 93, 52, 93, 93, 93,93,93,93,93,93,93},
	{1,1,1,1,1,1,1,1,1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_fifth_attack,     SLOT(0),        0,      0,
	L"",          L"!Fifth Attack!", L"",	CABAL_NONE,
	RACE_NONE, ALIGN_NONE, GROUP_FIGHTMASTER
    },

/* non-combat skills */

    {
	{ L"blue arrow", L"mavi ok"},			{ 93, 93, 93,93,93,93,93,51,93,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_blue_arrow,	SLOT(0),	 50,	12,
	L"", L"!blue arrow!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"fast healing", L"hızlı iyileşme"},	{  15, 9, 16,  6,7,7,9,6 ,15 ,3,9,9,9},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_SLEEPING,
	&gsn_fast_healing,	SLOT( 0),	0,	0,
	L"", L"!Fast Healing!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_MEDITATION
    },

    {
	{ L"green arrow", L"yeşil ok"},			{ 93, 93, 93,93,93,93,93,40,93,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_green_arrow,	SLOT(0),	 50,	12,
	L"", L"!green arrow!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"grip", L"kavrama"},            { 93, 93, 93,43,93,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_grip,            SLOT( 0),        0,     18,
	L"", L"!Grip!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_WEAPONSMASTER
    },

    {
	{ L"haggle", L"pazarlık"},		{  7, 18,  1, 14,12,8,17,15 ,7 ,19,5,5,5},
	{ 2,  2,  1,  2, 2, 2, 1, 2, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_RESTING,
	&gsn_haggle,		SLOT( 0),	0,	0,
	L"", L"!Haggle!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"hide", L"saklan"},		{  93, 93,  1,  93,93,93,10,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_RESTING,
	&gsn_hide,		SLOT( 0),	 0,	18,
	L"", L"!Hide!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"katana", L"katana"},	{ 93, 93, 93, 93,93,93,93,93,93,72,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_OBJ_INV,		POS_STANDING,
	&gsn_katana,		SLOT(0),	100,	24,
  L"", L"Yeni bir katana yapabilirsin.",  L"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"lay hands", L"şifalıeller"},		{93,93,93,93,63,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_lay_hands,		SLOT( 0),	0,	12,
  L"",  L"Yeniden masum iyileştirebilirsin.",  L"",CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"lore", L"irfan"},		{  93,93,13,21,19,20,19,18,93,22,20,93,93},
	{ 2,  2,  2,  2, 2, 2, 2, 2, 2, 2,2,2,2},
	spell_null,		TAR_IGNORE,		POS_RESTING,
	&gsn_lore,		SLOT( 0),	0,	36,
	L"", L"!Lore!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"mastering pound", L"vuruş ustalığı"},	{ 93,93,93,93,93,93,61,93,93,93,93,93,93},
	{ 2,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_master_hand,	SLOT( 0),	0,	0,
	L"", L"!Master Hand!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_FIGHTMASTER
    },

    {
	{ L"mastering sword", L"kılıç ustalığı"},	{ 93,93,93,93,93,93,93,93,93,61,93,93,93},
	{ 2, 2, 1, 1,1,1,1,1,1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_master_sword,           SLOT( 0),       0,      0,
	L"", L"!master sword!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_WEAPONSMASTER
    },

    {
	{ L"meditation", L"meditasyon"},		{  6,  6, 15, 15,14,15,14,16,6,16,6,6,6},
	{ 1,  1,  2,  2, 1, 1, 1, 1,1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_SLEEPING,
	&gsn_meditation,	SLOT( 0),	0,	0,
	L"", L"Meditation", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_MEDITATION
    },

    {
	{ L"peek", L"dikiz"},			{  93, 93,  1, 93,93,93,93,93,93 ,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_peek,		SLOT( 0),	 0,	 0,
	L"", L"!Peek!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"pick lock", L"maymuncuk"},		{ 93, 93,  17, 93,93,93,34,93,93,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_pick_lock,		SLOT( 0),	 0,	12,
	L"", L"!Pick!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"poison smoke", L"zehir bulutu"},      {  93, 93, 93, 93, 93, 93, 30, 93,93,93,93,93,93 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,     	TAR_IGNORE,            POS_FIGHTING,
        &gsn_poison_smoke,      SLOT(0),        20, 	18,
	L"", L"!poison smoke!", L"",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"quiet movement", L"sessiz yürüyüş"},      {  93, 93, 93, 93, 93, 93, 93, 15 ,93,93,10,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,        TAR_IGNORE,          POS_STANDING,
        &gsn_quiet_movement,             SLOT(0),        0,       0,
	L"", L"!quiet movement!", L"",	CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"recall", L"anımsa"},{  1,  1,  1,  1,1,1,1,1 ,1,1,1,1,1},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_recall,		SLOT( 0),	0,	24,
	L"", L"!Recall!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"red arrow", L"kızıl ok"},		{ 93, 93, 93,93,93,93,93,44,93,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_red_arrow,	SLOT(0),	 50,	12,
	L"", L"!red arrow!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"sneak", L"süzül"},	{  93, 93, 4, 93, 93, 93, 18, 93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_sneak,		SLOT( 0),	 0,	12,
  L"", L"Artık süzülmüyorsun.",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"steal", L"arakla"},			{  93, 93, 12, 93, 93, 93, 93, 93,93 ,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_steal,		SLOT( 0),	 0,	12,
	L"", L"!Steal!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"scrolls", L"parşömen"},		{  1,  1,  1,  93,93,1,93,1 , 1,6,1,1,1},
	{ 1,  1,  2,  2, 2, 2, 2, 2, 2, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_scrolls,		SLOT( 0),	0,	12,
	L"", L"!Scrolls!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_WIZARD
    },

    {
	{ L"staves", L"tılsımlar"},		{  1,  1,  93,  93,93,93,1,1 ,1,1,1,1,1},
	{ 1,  1,  2,  2, 2, 2, 2, 2, 2, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_staves,		SLOT( 0),	0,	12,
	L"", L"!Staves!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_WIZARD
    },

    {
	{ L"tame", L"uysallaştır"},        {  93, 93, 93, 93, 93, 93, 93, 39 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,        TAR_CHAR_DEFENSIVE,          POS_FIGHTING,
        &gsn_tame,             SLOT(0),        0,       24,
	L"", L"!tame!", L"",CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"trance", L"trans"},       {  14,15,93,93,93,93,93,93,14 ,93,21,21,21},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_IGNORE,   POS_SLEEPING,
        &gsn_trance,       SLOT(0),        0,       0,
	L"", L"", L"",	CABAL_NONE , RACE_NONE,ALIGN_NONE, GROUP_MEDITATION
    },

    {
	{ L"white arrow", L"beyaz ok"},			{ 93, 93, 93,93,93,93,93,49,93,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_white_arrow,	SLOT(0),	 50,	12,
	L"", L"!white arrow!",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"wands", L"değnekler"},	{  1,  1,  93,  93,93,1,93,93 ,1,5,1,1,1},
	{ 1,  1,  2,  2, 2, 2, 2, 2, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_wands,		SLOT( 0),	0,	12,
	L"", L"!Wands!", L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_WIZARD
    },

/* cabals */

    {
	{ L"mortal strike", L"ölümcül atak"},	{ 63,63,63,63,63,63,63,63,63,63,63,63,63},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_CHAR_SELF,		POS_STANDING,
	&gsn_mortal_strike,		SLOT(0),	200,	24,
  L"ölümcül atak",  L"!mortal strike!",  L"",
	CABAL_BATTLE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"disgrace", L"gözden düşür"},{  33, 33, 33, 33, 33,33,33,33,33,33,33,33,33},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_disgrace,	TAR_CHAR_OFFENSIVE,		POS_STANDING,
	NULL,		SLOT(619),	200,	12,
  L"",  L"Onurunu kazandığını hissediyorsun.",  L"",
	CABAL_CHAOS , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"evil spirit", L"kem ruh"},	{  33, 33, 33, 33, 33,33,33,33,33,33,33,33,33},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_evil_spirit,	TAR_IGNORE,		POS_STANDING,
	&gsn_evil_spirit,		SLOT(618),	800,	36,
  L"kem ruh",  L"Vücudunla ruhun yeniden bir oluyor.",  L"",
	CABAL_INVADER , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"ruler aura", L"tüze aurası"},     {  33, 33, 33, 33, 33,33,33,33,33,33,33,33,33},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_ruler_aura,    TAR_CHAR_SELF,          POS_STANDING,
        NULL,             SLOT(667),        20,       12,
        L"", L"Tüze auran yokoluyor.", L"",	CABAL_RULER ,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	{ L"sword of justice", L"adalet kılıcı"},	{48,48,48,48, 48,48,48,48,48,48,48,48,48},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_sword_of_justice,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(686),	50,	12,
  L"adalet kılıcı",  L"!sword of justice!", L"", CABAL_RULER ,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	{ L"bandage", L"sargı"},       {23, 23, 23, 23,23,23,23,23,23,23,23,23,23},
	{1,1,1,1,1,1,1,1,1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_bandage,     SLOT(0),        0,      0,
	L"", L"Daha az sağlıklı hissediyorsun.", L"",	CABAL_BATTLE,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	{ L"cabal recall", L"kabal anımsa"},	{ 10,10,10,10,10,10,10,10,10,10,10,10,10},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_cabal_recall,		SLOT( 0),	0,	24,
  L"",  L"Anımsama için yeniden dua edebilirsin.", L"", CABAL_BATTLE ,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        { L"wanted", L"aranıyor"},                {  1,  1,  1,  1, 1, 1, 1, 1 ,1,1,1,1,1},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_DEAD,
        &gsn_wanted,                   SLOT(0),        0,       0,
        L"", L"!Wanted!", L"",		CABAL_RULER,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        { L"judge", L"yargılama"},               {  1,  1,  1,  1, 1, 1, 1, 1 ,1,1,1,1,1},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_DEAD,
        &gsn_judge,              SLOT(0),        0,       0,
        L"", L"!Judge!", L"",		CABAL_RULER,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        { L"bloodthirst", L"kana susama"},          {  15, 15, 15, 15, 15, 15, 15, 15 ,15,15,15,15,15},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_CHAR_SELF,       POS_FIGHTING,
        &gsn_bloodthirst,       SLOT(0),        0,       12,
        L"", L"Artık kana susamıyorsun.", L"",	CABAL_BATTLE,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        { L"spellbane", L"büyüyıkımı"},       { 10,10,10,10,10,10,10,10,10,10,10,10,10 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_CHAR_SELF,       POS_STANDING,
        &gsn_spellbane,       SLOT(0),        0,       12,
        L"büyüyıkımı", L"Büyüye karşı direncin azalıyor.", L"",	CABAL_BATTLE,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        { L"resistance", L"direnç"},      {  12, 12, 12,12,12,12,12,12 ,12,12,12,12,12},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_CHAR_SELF,       POS_FIGHTING,
        &gsn_resistance,       SLOT(0),        0,       24,
        L"", L"Sertliğin yokoluyor.", L"",	CABAL_BATTLE,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
       { L"deathblow", L"ölüm rüzgarı"},          {  20,20,20,20,20,20,20,20 ,20,20,20,20,20},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_IGNORE,       POS_STANDING,
        &gsn_deathblow,       SLOT(0),        0,       0,
        L"", L"!deathblow!", L"",	CABAL_BATTLE,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        { L"transform", L"dönüşüm"},          {  10, 10, 10,10,10,10,10,10 ,10,10,10,10,10},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_transform,      TAR_CHAR_SELF,       POS_STANDING,
        NULL,       SLOT(522),        100,       24,
        L"", L"Sağlığın azalıyor.", L"",	CABAL_SHALAFI,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	{ L"mental knife", L"zihin bıçağı"},        {  15, 15, 15,93,15,15,15,15,15,15,15,15,15 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_mental_knife,      TAR_CHAR_OFFENSIVE,       POS_FIGHTING,
        &gsn_mental_knife,       SLOT(524),        35,       12,
        L"zihin bıçağı", L"Zihnindeki acı yokoluyor.", L"",CABAL_SHALAFI,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	{ L"demon summon", L"iblis çağrısı"},         {  25, 25, 25,93,25,25,25,25 ,25,25,25,25,25},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_demon_summon,      TAR_CHAR_SELF,       POS_FIGHTING,
        &gsn_demon_summon,       SLOT(525),        100,       12,
        L"", L"Çağrı gücünün geri geldiğini hissediyorsun.", L"", CABAL_SHALAFI,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	{ L"scourge", L"ceza kırbacı"},     {  30, 30, 30,93,30,30,30,30,30,30,30,30,30 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_scourge,      TAR_IGNORE,       POS_FIGHTING,
        NULL,       SLOT(526),        50,       18,
        L"ceza kırbacı", L"!scourge!", L"",CABAL_SHALAFI,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        { L"manacles", L"kelepçe"},        {  20, 20, 20,20,20,20,20,20 ,20,20,20,20,20},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_manacles,      TAR_CHAR_OFFENSIVE,       POS_FIGHTING,
        NULL,       SLOT(528),        75,       12,
        L"", L"Zincirlerin yokoluyor.", L"", 	CABAL_RULER,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
      { L"shield of ruler", L"tüze kalkanı"},        {  10, 10, 10,10,10,10,10,10 ,10,10,10,10,10},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_shield_ruler,      TAR_IGNORE,       POS_FIGHTING,
        NULL,       SLOT(529),        100,       12,
        L"", L"!shield!", L"",	CABAL_RULER,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
      { L"guard", L"savun"},    {  22,22,22,22,22,22,22,22 ,22,22,22,22,22},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_IGNORE,       POS_STANDING,
        &gsn_guard,       SLOT(0),        0,       12,
        L"", L"", L"",	CABAL_KNIGHT,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
      { L"guard call", L"muhafız çağrısı"},          {  15, 15, 15,15,15,15,15,15 ,15,15,15,15,15},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_guard_call,      TAR_IGNORE,       POS_FIGHTING,
	&gsn_guard_call,       SLOT(530),        75,       12,
  L"", L"Yeniden muhafız çağırabilirsin.", L"",	CABAL_RULER,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
          { L"nightwalker", L"solucan"},        {  25, 25, 25,25,25,25,25,25 ,25,25,25,25,25},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_nightwalker,      TAR_IGNORE,       POS_FIGHTING,
	&gsn_nightwalker,       SLOT(531),        75,       12,
  L"", L"Çağrı gücünün geri geldiğini hissediyorsun.", L"",CABAL_INVADER,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
          { L"eyes of intrigue", L"entrika"},          {  13, 13, 13,13,13,13,13,13,13,13,13,13,13 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_eyes,      TAR_IGNORE,       POS_FIGHTING,
	NULL,       SLOT(532),        75,       12,
        L"", L"!eyes of intrigue!", L"",	CABAL_INVADER,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        { L"fade", L"soluş"},       {  20, 20, 20,20,20,20,20,20 ,20,20,20,20,20},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_IGNORE,       POS_STANDING,
        &gsn_fade,       SLOT(0),        0,       24,
        L"", L"!fade!", L"",	CABAL_INVADER,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
       { L"shadow cloak", L"gölge pelerini"},        {  10, 10, 10,10,10,10,10,10,10,10,10,10,10 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_shadow_cloak,      TAR_CHAR_DEFENSIVE,       POS_STANDING,
	NULL,       SLOT(533),        10,       12,
  L"", L"Gölgeler seni korumuyor.", L"",	CABAL_INVADER,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
      { L"nightfall", L"alacakaranlık"},     {  16, 16, 16,16,16,16,16,16,16,16,16,16,16 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_nightfall,      TAR_IGNORE,       POS_STANDING,
	NULL,       	SLOT(534),        50,       12,
  L"", L"Işıkları kontrol edemiyorsun.", L"",	CABAL_INVADER,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        { L"aura of chaos", L"kaos aurası"},    {  16, 16, 16,16,16,16,16,16,16,16,16,16,16 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_aura_of_chaos,      TAR_CHAR_DEFENSIVE,       POS_STANDING,
	NULL,       	SLOT(720),        20,       12,
  L"", L"Karmaşa tanrısı artık seni korumuyor.", L"",
	CABAL_CHAOS, RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
       { L"garble", L"afoni"},        {  10, 10, 10,10,10,10,10,10 ,10,10,10,10,10},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_garble,      TAR_CHAR_DEFENSIVE,       POS_FIGHTING,
	&gsn_garble,       SLOT(535),        30,       12,
  L"", L"Dilin normale dönüyor.", L"",	CABAL_CHAOS,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
         { L"mirror", L"ayna"},         { 15, 15, 15,15,15,15,15,15,15,15,15,15,15 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_mirror,      TAR_CHAR_DEFENSIVE,       POS_STANDING,
	&gsn_mirror,       SLOT(536),        40,       12,
  L"", L"Yokolma hissini yaşıyorsun.", L"",	CABAL_CHAOS,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	{ L"confuse", L"şaşırtma"},        {  20, 20, 20,20,20,20,20,20 ,20,20,20,20,20},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_confuse,      TAR_CHAR_OFFENSIVE,       POS_FIGHTING,
	&gsn_confuse,       SLOT(537),        20,       12,
  L"", L"Şaşkınlığın azalıyor.", L"",	CABAL_CHAOS,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	{ L"doppelganger", L"dönüşüm"},        {  25, 25, 25,25,25,25,25,25 ,25,25,25,25,25},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_doppelganger,      TAR_CHAR_DEFENSIVE,       POS_STANDING,
	&gsn_doppelganger,       SLOT(527),        75,       12,
  L"", L"Doğal formuna döndün.", L"",	CABAL_CHAOS,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
       { L"chaos blade", L"yatağan"},      {  23, 23, 23,23,23,23,23,23,23,23,23,23,23 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_chaos_blade,      TAR_IGNORE,       POS_STANDING,
        &gsn_chaos_blade,       SLOT(550),        60,       12,
        L"", L"!chaos blade!", L"",	CABAL_CHAOS,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

   {
          { L"stalker", L"avcı"},    {  93, 93, 93,93,93,93,93,93 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_stalker,      TAR_IGNORE,       POS_STANDING,
        &gsn_stalker,       SLOT(554),        100,       12,
        L"", L"Yeni bir avcı çağırabilirsin.", L"",
	CABAL_RULER, 	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
           { L"random", L"rasgele"},          {  28, 28, 28,28,28,28,28,28 ,28,28,28,28,28},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_randomizer,      TAR_IGNORE,       POS_STANDING,
        &gsn_randomizer,       SLOT(555),        200,       24,
        L"", L"Yeniden rasgeleyi kullanabilirsin.", L"", CABAL_CHAOS,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	{ L"tesseract", L"öte boyut"},          {  29, 29, 29,93,29,29,29,29 ,29,29,29,29,29},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_tesseract,      TAR_IGNORE,       POS_STANDING,
        &gsn_tesseract,       SLOT(556),        150,       12,
        L"", L"!tesseract!", L"",	CABAL_SHALAFI,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
       { L"trophy", L"andaç"},       {  23, 23, 23,23,23,23,23,23 ,23,23,23,23,23},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_IGNORE,       POS_STANDING,
        &gsn_trophy,       SLOT(0),        30,       12,
        L"", L"Yeni bir andaçın hayalini kuruyorsun.", L"",	CABAL_BATTLE,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
         { L"truesight", L"keskingörüş"},         {  25, 25, 25,25,25,25,25,25 ,25,25,25,25,25},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_IGNORE,       POS_STANDING,
        &gsn_truesight,       SLOT(0),        50,       12,
        L"", L"Gözlerin keskinliğini yitiriyor.", L"",	CABAL_BATTLE,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        { L"brew", L"mayacı"},     {  23, 23, 23,23,23,23,23,23,23 ,23,23,23,23},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_brew,      TAR_OBJ_INV,       POS_STANDING,
        &gsn_brew,       SLOT(557),        25,       12,
        L"", L"Yeniden mayalamaya başlayabilirsin.", L"",
	CABAL_SHALAFI,	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        { L"shadowlife", L"gölgeyaşam"},         {  30, 30, 30,30,30,30,30,30 ,30,30,30,30,30},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_shadowlife,      TAR_CHAR_OFFENSIVE,       POS_STANDING,
        &gsn_shadowlife,       SLOT(558),        80,       12,
        L"", L"Kendini daha gölgeli hisssediyorsun.", L"",	CABAL_INVADER,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
         { L"ruler badge", L"tüze nişanı"},        {  25, 25, 25,25,25,25,25,25 ,25,25,25,25,25},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_ruler_badge,      TAR_CHAR_SELF,   POS_STANDING,
        &gsn_ruler_badge,       SLOT(560),        50,       12,
        L"", L"!tüze nişanı!", L"",	CABAL_RULER,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
         { L"remove badge", L"nişan kaldır"},        {  28, 28, 28,28,28,28,28,28,28,28,28,28,28 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_remove_badge,      TAR_CHAR_SELF,   POS_STANDING,
        &gsn_remove_badge,       SLOT(561),        100,       12,
        L"", L"!remove badge!", L"",	CABAL_RULER,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
      { L"golden aura", L"altın ayla"},         {  10,10,10,10,10,10,10,10,10,10,10,10,10 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_golden_aura,      TAR_IGNORE,   POS_STANDING,
        NULL,       SLOT(564),        25,       12,
        L"", L"Altın aylanın yokolduğunu hissediyorsun.", L"",	CABAL_KNIGHT,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        { L"dragonplate", L"ejderha zırhı"},       {  15,15,15,15,15,15,15,15 ,15,15,15,15,15},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_dragonplate,      TAR_IGNORE,   POS_STANDING,
        NULL,       SLOT(565),        60,       12,
        L"", L"", L"",	CABAL_KNIGHT,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
     { L"squire", L"bey"},       {  25, 25, 25,25,25,25,25,25 ,25,25,25,25,25},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_squire,      TAR_IGNORE,   POS_STANDING,
        &gsn_squire,       SLOT(566),        100,       12,
        L"", L"Yeni bir beyi düşünmeye başlıyorsun.", L"",	CABAL_KNIGHT,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
     { L"dragonsword", L"ejderha kılıcı"},          {  25,25,25,25,25,25,25,25 ,25,25,25,25,25},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_dragonsword,      TAR_IGNORE,   POS_STANDING,
        &gsn_dragonsword,       SLOT(567),        70,       12,
	L"", L"", L"",	CABAL_KNIGHT,	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
         { L"holy armor", L"kutsal zırh"},         {  10,10,10,10,10,10,10,10,10 ,10,10,10,10},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_holy_armor,      TAR_CHAR_SELF,   POS_RESTING,
        &gsn_knight,       SLOT(569),        20,       12,
        L"", L"Zarara karşı daha az korumalısın.", L"",	CABAL_KNIGHT,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },


    {
      { L"disperse", L"dağıtma"},       {  20,20,20,20,20,20,20,20,20,20,20,20,20 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_disperse,      TAR_IGNORE,   	POS_FIGHTING,
        &gsn_disperse,       SLOT(573),        100,       24,
        L"", L"Yeniden dağıtabilirsin.", L"",CABAL_CHAOS,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	{ L"hunt", L"avlama"},        {  13, 13, 13,13,13,13,13,13 ,13,13,13,13,13},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_IGNORE,       POS_STANDING,
        &gsn_hunt,       SLOT(0),        0,       6,
	L"", L"!hunt!", L"",	CABAL_HUNTER,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	{ L"find object", L"nesne ara"},	{   25, 25, 25, 25,25,25,25,25,25 ,25,25,25,25},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_find_object,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(585),	20,	18,
	L"", L"!Find Object!",  L"", CABAL_HUNTER ,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	{ L"path find", L"yol bulma"},		{  13,13 ,13,13,13,13,13,13 ,13 ,13,13,13,13},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_SLEEPING,
	&gsn_path_find,		SLOT( 0),	0,	0,
	L"", L"!endur!",  L"", CABAL_HUNTER ,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	{ L"riding", L"binicilik"},		{   30, 30, 30, 30,30,30,30,30,30 ,30,30,30,30},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_riding,			SLOT(0),	0,	6,
	L"", L"!riding!",  L"", CABAL_KNIGHT ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"wolf", L"kurt"},		{   20, 20,20, 20,20,20,20,20,20 ,20,20,20,20},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_wolf,	TAR_IGNORE,		POS_STANDING,
	NULL,		SLOT(593),	100,	12,
  L"",  L"Daha çok kurdu kontrol edebileceğini hissediyorsun.",  L"", CABAL_HUNTER ,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	{ L"wolf spirit", L"kurt ruhu"},	{   28, 28, 28, 28,28,28,28,28,28 ,28,28,28,28},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_wolf_spirit,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(685),	50,	12,
  L"",  L"Damarlarındaki kan tekrar eskisi gibi akmaya başladı.",  L"",
	CABAL_HUNTER , RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	{ L"armor use", L"zırh kullanımı"},		{ 17,17,17,17,17,17,17,17,17 ,17,17,17,17},
	{ 2, 1, 1, 1,1,1,1,1,1,1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_armor_use,	SLOT(0),	0,	0,
	L"", L"!Armor Use!", L"", CABAL_HUNTER ,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	{ L"world find", L"diyar bulma"},	{  53,53 ,53,53,53,53,53,53 ,53 ,53,53,53,53},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_SLEEPING,
	&gsn_world_find,	SLOT( 0),	0,	0,
	L"", L"!world find!",  L"", CABAL_HUNTER ,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	{ L"take revenge", L"intikam"},{  53,53 ,53,53,53,53,53,53 ,53 ,53,53,53,53},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_take_revenge,	TAR_IGNORE,		POS_STANDING,
	NULL,		SLOT(624),	50,		12,
	L"",  L"!take revenge!",  L"",
	CABAL_HUNTER , RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	{ L"mastering spell", L"büyü ustalığı"},	{37,37,37,37,37,37,37,37,37,37,37,37,37},
	{ 1,  1,  1,  1, 1, 1, 2, 2, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_mastering_spell,	SLOT(0),    0,       0,
  L"büyü ustalığı", L"!mastering spell!", L"",
	CABAL_SHALAFI, RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
       { L"guard dogs", L"muhafız köpekler"},        {  15, 15, 15,15,15,15,15,15 ,15,15,15,15,15},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_guard_dogs,      TAR_IGNORE,       POS_FIGHTING,
	NULL,       SLOT(687),        100,       12,
        L"", L"", L"",
	CABAL_LIONS, RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
  { L"eyes of tiger", L"kaplan gözleri"},        {  25, 25, 25,25,25,25,25,25 ,25,25,25,25,25},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_eyes_of_tiger,      TAR_IGNORE,       POS_FIGHTING,
	NULL,       SLOT(688),        20,       12,
        L"", L"", L"",
	CABAL_LIONS, RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
    { L"lion shield", L"aslan kalkanı"},       {  24, 24, 24,24,24,24,24,24 ,24,24,24,24,24},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_lion_shield,      TAR_IGNORE,       POS_FIGHTING,
	NULL,       SLOT(689),        200,       12,
        L"", L"", L"",
	CABAL_LIONS, RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
      { L"evolve lion", L"aslana dönüşme"},        {  18, 18, 18,18,18,18,18,18 ,18,18,18,18,18},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_evolve_lion,      TAR_IGNORE,       POS_FIGHTING,
	NULL,       SLOT(690),        50,       12,
        L"", L"", L"",
	CABAL_LIONS, RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
{ L"claw", L"pençe"},      {  20, 20, 20,20,20,20,20,20 ,20,20,20,20,20},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_IGNORE,       POS_FIGHTING,
	&gsn_claw,       SLOT(0),        50,       24,
  L"pençe", L"", L"",
	CABAL_LIONS, RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
  { L"prevent", L"intikam bağı"},   {  35, 35, 35,35,35,35,35,35 ,35,35,35,35,35},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_prevent,      TAR_IGNORE,       POS_FIGHTING,
	NULL,       SLOT(691),        75,       12,
        L"", L"", L"",
	CABAL_LIONS, RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

/* object spells */

    {
	{ L"terangreal", L"uyuşturma"},		{ 93,  93, 93, 93,93,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_terangreal,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_terangreal,	SLOT(607),	 5,	12,
  L"uyuşturma", L"Artık uyanıksın.",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"kassandra", L"kasandra"},	{ 93,  93, 93, 93,93,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_kassandra,	TAR_CHAR_SELF,		POS_STANDING,
	&gsn_kassandra,	SLOT(608),	 5,	12,
  L"", L"Kendini yine iyileştirebilirsin.",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"sebat", L"sebat"},		{ 93,  93, 93, 93,93,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_sebat,		TAR_CHAR_SELF,		POS_STANDING,
	&gsn_sebat,		SLOT(609),	 5,	12,
  L"", L"Kendini yine koruyabilirsin.",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	{ L"matandra", L"matandra"},	{ 93,  93, 93, 93,93,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_matandra,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_matandra,		SLOT(610),	 5,	12,
  L"matandra",  L"Kasandrayı tekrar kullanabilirsin.",  L"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    }

};
