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
        { "common",	LANG_COMMON	},
        { "human",	LANG_HUMAN	},
        { "elvish",	LANG_ELVISH	},
        { "dwarvish",	LANG_DWARVISH	},
        { "gnomish",	LANG_GNOMISH	},
        { "giant",	LANG_GIANT	},
        { "trollish",	LANG_TROLLISH	},
        { "cat",	LANG_CAT	}
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
    { 	"none",		"hit",		-1		},  /*  0 */
    {	"slice",	"slice", 	DAM_SLASH	},	
    {   "stab",		"stab",		DAM_PIERCE	},
    {	"slash",	"slash",	DAM_SLASH	},
    {	"whip",		"whip",		DAM_SLASH	},
    {   "claw",		"claw",		DAM_SLASH	},  /*  5 */
    {	"blast",	"blast",	DAM_BASH	},
    {   "pound",	"pound",	DAM_BASH	},
    {	"crush",	"crush",	DAM_BASH	},
    {   "grep",		"grep",		DAM_SLASH	},
    {	"bite",		"bite",		DAM_PIERCE	},  /* 10 */
    {   "pierce",	"pierce",	DAM_PIERCE	},
    {   "suction",	"suction",	DAM_BASH	},
    {	"beating",	"beating",	DAM_BASH	},
    {   "digestion",	"digestion",	DAM_ACID	},
    {	"charge",	"charge",	DAM_BASH	},  /* 15 */
    { 	"slap",		"slap",		DAM_BASH	},
    {	"punch",	"punch",	DAM_BASH	},
    {	"wrath",	"wrath",	DAM_ENERGY	},
    {	"magic",	"magic",	DAM_ENERGY	},
    {   "divine",	"divine power",	DAM_HOLY	},  /* 20 */
    {	"cleave",	"cleave",	DAM_SLASH	},
    {	"scratch",	"scratch",	DAM_PIERCE	},
    {   "peck",		"peck",		DAM_PIERCE	},
    {   "peckb",	"peck",		DAM_BASH	},
    {   "chop",		"chop",		DAM_SLASH	},  /* 25 */
    {   "sting",	"sting",	DAM_PIERCE	},
    {   "smash",	 "smash",	DAM_BASH	},
    {   "shbite",	"shocking bite",DAM_LIGHTNING	},
    {	"flbite",	"flaming bite", DAM_FIRE	},
    {	"frbite",	"freezing bite", DAM_COLD	},  /* 30 */
    {	"acbite",	"acidic bite", 	DAM_ACID	},
    {	"chomp",	"chomp",	DAM_PIERCE	},
    {  	"drain",	"life drain",	DAM_NEGATIVE	},
    {   "thrust",	"thrust",	DAM_PIERCE	},
    {   "slime",	"slime",	DAM_ACID	},
    {	"shock",	"shock",	DAM_LIGHTNING	},
    {   "thwack",	"thwack",	DAM_BASH	},
    {   "flame",	"flame",	DAM_FIRE	},
    {   "chill",	"chill",	DAM_COLD	},
    {   NULL,		NULL,		0		}
};

/* race table */
const 	struct	race_type	race_table	[]		=
{
/*
    {
	name,		pc_race?,
	act bits,	aff_by bits,	off bits,
	imm,		res,		vuln,
	form,		parts 
    },
*/
    { "unique",		FALSE, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

    { /* 1 */
	"human",	TRUE, 		0,
	0,		0, 		0,
	0, 		0,		0,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    { /* 2 */
	"elf",		TRUE, 		0,
	0,		AFF_INFRARED|AFF_SNEAK,	0,
	0,		RES_CHARM,	VULN_IRON,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    { /* 3 */
        "half-elf",     TRUE, 		0,
        0,              AFF_INFRARED,   0,
        0,              RES_CHARM,      VULN_IRON,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    { /* 4 */
        "dark-elf",     TRUE, 		0,
        0,              AFF_INFRARED|AFF_SNEAK, 0,
        0,              RES_CHARM,      VULN_IRON,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    { /* 5 */
        "rockseer",     TRUE, 		0,
        0,              AFF_INFRARED|AFF_SNEAK|AFF_PASS_DOOR, 0,
        0,              RES_CHARM,      VULN_WOOD,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    { /* 6 */
	"dwarf",	TRUE, 		0,
	0,		AFF_INFRARED,	0,
	0,		RES_POISON|RES_DISEASE, VULN_DROWNING,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {  /* 7 */
      "svirfnebli",     TRUE, 		0,
      0,                AFF_INFRARED,   0,
      0,                RES_MAGIC|RES_POISON|RES_DISEASE, VULN_BASH,
      A|H|M|V,          A|B|C|D|E|F|G|H|I|J|K
    },

    { /* 8 */
      "duergar",        TRUE, 		0,
      0,                AFF_INFRARED,   0,
      0,                RES_MAGIC|RES_POISON|RES_DISEASE, VULN_DROWNING,
      A|H|M|V,          A|B|C|D|E|F|G|H|I|J|K
    },

    { /* 9 */
        "arial",        TRUE, 		0,
        0,              AFF_FLYING,             0,
        0,              0,              0,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    { /* 10 */
        "gnome",        TRUE, 		0,
        0,              AFF_INFRARED,   0,
        0,              RES_POISON,     VULN_BASH,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    { /* 11 */
	"storm giant",	TRUE, 		0,
	0,		AFF_FLYING,		0,
	0,		RES_LIGHTNING,	VULN_MENTAL|VULN_COLD|VULN_FIRE,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    { /* 12 */
	"cloud giant",	TRUE, 		0,
	0,		AFF_FLYING,		0,
	0,		RES_WEAPON,	VULN_MENTAL|VULN_LIGHTNING,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    { /* 13 */
	"fire giant",	TRUE, 		0,
	0,		0,		0,
	0,		RES_FIRE,	VULN_MENTAL|VULN_LIGHTNING|VULN_COLD,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    { /* 14 */
	"frost giant",	TRUE, 		0,
	0,		0,		0,
	0,		RES_COLD,	VULN_MENTAL|VULN_LIGHTNING|VULN_FIRE,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    { /* 15 */
        "felar",        TRUE, 		0,
        0,              AFF_INFRARED,   OFF_TAIL,
        0,              RES_LIGHT|RES_COLD,     VULN_FIRE|VULN_DROWNING,
        A|H|M|V,        A|C|D|E|F|H|J|K|Q|U|V
    },

    { /* 16 */
	"githyanki",	TRUE,  		0,
	0,		AFF_INFRARED,	0,
	0, 		0,		0,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    { /* 17 */
	"satyr",	TRUE, 		0,
	0,		AFF_INFRARED,	0,
	0,		RES_WOOD|RES_DISEASE,	0,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {  /* 18 */
	"troll",	TRUE, 		0,/*DETECT_HIDDEN*/
	0,		AFF_REGENERATION|AFF_INFRARED,	OFF_BERSERK,
 	0,	RES_CHARM|RES_BASH,	VULN_FIRE|VULN_ACID,
	A|B|H|M|V,		A|B|C|D|E|F|G|H|I|J|K|U|V
    },

    { /* 19 */
        "black dragon", FALSE,  	0,
	0, 		AFF_INFRARED|AFF_FLYING,	0,
	0,		RES_BASH|RES_CHARM|RES_ACID,	VULN_PIERCE,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    { /* 20 */
        "blue dragon", 	FALSE, 		0,
	0, 		AFF_INFRARED|AFF_FLYING,	0,
	0,		RES_BASH|RES_CHARM|RES_LIGHTNING, 
	VULN_PIERCE,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    { /* 21 */
        "green dragon", FALSE, 		0,
	0, 		AFF_INFRARED|AFF_FLYING,	0,
	0,		RES_BASH|RES_CHARM|RES_POISON,	VULN_PIERCE,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    { /* 22 */
        "red dragon", 	FALSE, 		0, 
	0, 		AFF_INFRARED|AFF_FLYING,	0,
	0,		RES_BASH|RES_CHARM|RES_FIRE,	VULN_PIERCE,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    { /* 23 */
        "white dragon", FALSE, 		0, 
	0, 		AFF_INFRARED|AFF_FLYING,	0,
	0,		RES_BASH|RES_CHARM|RES_COLD,	VULN_PIERCE,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    { /* 24 */
        "brass dragon", FALSE, 		0, 
	0, 		AFF_INFRARED|AFF_FLYING,	0,
	0,		RES_BASH|RES_CHARM|RES_FIRE,	VULN_PIERCE,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    { /* 25 */
        "gold dragon", 	FALSE, 		0, 
	0, 		AFF_INFRARED|AFF_FLYING,	0,
	0,		RES_BASH|RES_CHARM|RES_FIRE|RES_POISON, 
	VULN_PIERCE,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    { /* 26 */
        "silver dragon",FALSE, 		0, 
	0, 		AFF_INFRARED|AFF_FLYING,	0,
	0,		RES_BASH|RES_CHARM|RES_COLD,	VULN_PIERCE,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    { /* 27 */
        "bronze dragon",FALSE, 		0, 
	0, 		AFF_INFRARED|AFF_FLYING,	0,
	0,		RES_BASH|RES_CHARM|RES_LIGHTNING, 
	VULN_PIERCE,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    { /* 28 */
        "copper dragon",FALSE, 		0, 
	0, 		AFF_INFRARED|AFF_FLYING,	0,
	0,		RES_BASH|RES_CHARM|RES_ACID,	VULN_PIERCE,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    { /* 29 */
	"bat",		FALSE, 		0,
	0,		AFF_FLYING/*|AFF_DARK_VISION*/,	OFF_DODGE|OFF_FAST,
	0,		0,		VULN_LIGHT,
	A|G|V,		A|C|D|E|F|H|J|K|P
    },

    { /* 30 */
	"bear",		FALSE, 		0,
	0,		0,		OFF_CRUSH|OFF_DISARM|OFF_BERSERK,
	0,		RES_BASH|RES_COLD,	0,
	A|G|V,		A|B|C|D|E|F|H|J|K|U|V
    },

    {  /* 31 */
	"cat",		FALSE, 		0,
	0,		/*AFF_DARK_VISION*/0,	OFF_FAST|OFF_DODGE,
	0,		0,		0,
	A|G|V,		A|C|D|E|F|H|J|K|Q|U|V
    },

    {  /* 32 */
	"centipede",	FALSE, 		0,
	0,		/*AFF_DARK_VISION*/0,	0,
	0,		RES_PIERCE|RES_COLD,	VULN_BASH,
 	A|B|G|O,		A|C|K	
    },

    {  /* 33 */
	"dog",		FALSE, 		0,
	0,		0,		OFF_FAST,
	0,		0,		0,
	A|G|V,		A|C|D|E|F|H|J|K|U|V
    },

    { /* 34 */
	"doll",		FALSE, 		0,
	0,		0,		0,
	IMM_COLD|IMM_POISON|IMM_HOLY|IMM_NEGATIVE|IMM_MENTAL|IMM_DISEASE
	|IMM_DROWNING,	RES_BASH|RES_LIGHT,
	VULN_SLASH|VULN_FIRE|VULN_ACID|VULN_LIGHTNING|VULN_ENERGY,
	E|J|M|cc,	A|B|C|G|H|K
    },

    { /* 35 */
	"fido",		FALSE, 		0,
	0,		0,		OFF_DODGE|ASSIST_RACE,
	0,		0,			VULN_MAGIC,
	A|B|G|V,	A|C|D|E|F|H|J|K|Q|V
    },		
   
    { /* 36 */
	"fox",		FALSE, 		0,
	0,		/*AFF_DARK_VISION*/0,	OFF_FAST|OFF_DODGE,
	0,		0,		0,
	A|G|V,		A|C|D|E|F|H|J|K|Q|V
    },

    { /* 37 */
	"goblin",	FALSE, 		0,
	0,		AFF_INFRARED,	0,
	0,		RES_DISEASE,	VULN_MAGIC,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    { /* 38 */
	"hobgoblin",	FALSE, 		0,
	0,		AFF_INFRARED,	0,
	0,		RES_DISEASE|RES_POISON,	0,
	A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K|Y
    },

    {  /* 39 */
	"kobold",	FALSE, 		0,
	0,		AFF_INFRARED,	0,
	0,		RES_POISON,	VULN_MAGIC,
	A|B|H|M|V,	A|B|C|D|E|F|G|H|I|J|K|Q
    },

    {  /* 40 */
	"lizard",	FALSE, 		0,
	0,		0,		0,
	0,		RES_POISON,	VULN_COLD,
	A|G|X|cc,	A|C|D|E|F|H|K|Q|V
    },

    {  /* 41 */
	"modron",	FALSE, 		0,
	0,		AFF_INFRARED,	ASSIST_RACE|ASSIST_ALIGN,
	IMM_CHARM|IMM_DISEASE|IMM_MENTAL|IMM_HOLY|IMM_NEGATIVE,
			RES_FIRE|RES_COLD|RES_ACID,	0,
	H,		A|B|C|G|H|J|K
    },

    {  /* 42 */
	"orc",		FALSE, 		0,
	0,		AFF_INFRARED,	0,
	0,		RES_DISEASE,	VULN_LIGHT,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {  /* 43 */
	"pig",		FALSE, 		0,
	0,		0,		0,
	0,		0,		0,
	A|G|V,	 	A|C|D|E|F|H|J|K
    },	

    { /* 44 */
	"rabbit",	FALSE, 		0,
	0,		0,		OFF_DODGE|OFF_FAST,
	0,		0,		0,
	A|G|V,		A|C|D|E|F|H|J|K
    },
    
    { /* 45 */
	"school monster",	FALSE, 		0,
	ACT_NOALIGN,		0,		0,
	IMM_CHARM|IMM_SUMMON,	0,		VULN_MAGIC,
	A|M|V,		A|B|C|D|E|F|H|J|K|Q|U
    },	

    {  /* 46 */
	"snake",	FALSE, 		0,
	0,		0,		0,
	0,		RES_POISON,	VULN_COLD,
	A|G|X|Y|cc,	A|D|E|F|K|L|Q|V|X
    },
 
    {  /* 47 */
	"song bird",	FALSE, 		0,
	0,		AFF_FLYING,		OFF_FAST|OFF_DODGE,
	0,		0,		0,
	A|G|W,		A|C|D|E|F|H|K|P
    },

    {  /* 48 */
	"water fowl",	FALSE, 		0,
	0,		AFF_SWIM|AFF_FLYING,	0,
	0,		RES_DROWNING,		0,
	A|G|W,		A|C|D|E|F|H|K|P
    },		
  
    {  /* 49 */
	"wolf",		FALSE, 		0,
	0,		/*AFF_DARK_VISION*/0,	OFF_FAST|OFF_DODGE,
	0,		0,		0,	
	A|G|V,		A|C|D|E|F|J|K|Q|V
    },

    {  /* 50 */
	"wyvern",	FALSE, 		DETECT_INVIS|DETECT_HIDDEN,
	0,		AFF_FLYING,	OFF_BASH|OFF_FAST|OFF_DODGE,
	IMM_POISON,	0,	VULN_LIGHT,
	A|B|G|Z,		A|C|D|E|F|H|J|K|Q|V|X
    },

    { /* 51 */
        "dragon", 	FALSE,		0,
	0, 		AFF_INFRARED|AFF_FLYING,	0,
	0,		RES_FIRE|RES_BASH|RES_CHARM,	VULN_PIERCE|VULN_COLD,
	A|H|Z,		A|C|D|E|F|G|H|I|J|K|P|Q|U|V|X	
    },

    { /* 52 */
	"giant",	FALSE, 		0,
	0,		0,		0,
	0,		RES_FIRE|RES_COLD,	VULN_MENTAL|VULN_LIGHTNING,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    { /* 53 */
	"golem",	FALSE, 		0,
	0,		0,		0,
	IMM_POISON|IMM_DISEASE,		RES_WEAPON,	VULN_MENTAL|VULN_MAGIC,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    { /* 54 */
	"undead",	FALSE,
	DETECT_INVIS|DETECT_HIDDEN|DETECT_GOOD|DETECT_EVIL,
	0,		AFF_PASS_DOOR|AFF_INFRARED,	0,
	IMM_DROWNING|IMM_DISEASE|IMM_POISON,	RES_NEGATIVE|RES_ENERGY,
	VULN_SILVER|VULN_HOLY|VULN_LIGHT,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    { /* 55 */
	"drow",		FALSE,		0,
	0,		AFF_INFRARED,	0,
	0,		RES_CHARM,		VULN_IRON,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    { /* 56 */
	"draconian",	FALSE,		0,
	0,		0,		0,
	0,		RES_SLASH|RES_POISON,	VULN_COLD,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    { /* 57 */
	"sprite",	FALSE,		DETECT_EVIL,
	0,		AFF_FLYING,		G,
	0,		0,			VULN_COLD|VULN_WEAPON,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    { /* 58 */
	"aarakocra",	FALSE,		0,
	ACT_SCAVENGER,	AFF_FLYING,	OFF_FAST,
	0,		0,		0,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K|P
    },

    { /*  59 */
	"beholder",	FALSE,		0,
	0,		AFF_REGENERATION,		0,
	0,		RES_MAGIC,		0,
	A|H|M|V,	A|E|K
    },

    {  /* 60 */
	"tiger",	FALSE,		0,
	0,	/*AFF_DARK_VISION*/0, OFF_FAST|OFF_DODGE,
	0,		RES_WEAPON,		0,
	A|G|V,		A|C|D|E|F|H|J|K|Q|U|V
    },

    {  /* 61 */
	"lion",		FALSE,		0,
	ACT_SENTINEL,	/*AFF_DARK_VISION*/0,	OFF_FAST|OFF_DODGE,
	0,		RES_WEAPON,		0,
	A|G|V,		A|C|D|E|F|H|J|K|Q|U|V
    },

    {  /* 62 */
	"centaur",	FALSE,		0,
	0,		0, OFF_BASH|OFF_KICK|OFF_KICK_DIRT|OFF_DODGE,
	0,		RES_WEAPON,		0,
	A|G|N|V,		A|B|C|D|E|F|G|H|I|J|K|Q
    },

    {  /* 63 */
	"death knight",	FALSE,		0,
	ACT_UNDEAD,	0, 	OFF_PARRY|OFF_DISARM,
	IMM_DISEASE|IMM_POISON|IMM_DROWNING,	RES_FIRE|RES_NEGATIVE,
	VULN_HOLY,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    { /* 65 */
        "dracolich", 	FALSE, 		0,
	0, 		AFF_INFRARED|AFF_FLYING,	0,
	IMM_CHARM|IMM_LIGHTNING|IMM_COLD,	RES_FIRE|RES_BASH, 
	VULN_PIERCE|VULN_COLD,
	A|H|Z,		A|C|D|E|F|G|H|I|J|K|P|Q|U|V|X	
    },

    {  /* 66 */
	"air elemental",FALSE,		0,
	0,		0,		OFF_FADE,
	0,		RES_WEAPON|RES_SUMMON,		0,
	A|H|Z,		A|D	
    },

    {  /* 67 */
	"earth elemental",	FALSE,		0,
	0,		0,	OFF_FADE,
	0,		RES_WEAPON|RES_SUMMON,		0,
	A|H|Z,		A|D	
    },

    {  /* 68 */
	"fire elemental",	FALSE,		0,
	0,		0,	OFF_FADE,
	IMM_FIRE,		RES_WEAPON,		VULN_COLD,
	A|H|Z,		A|D	
    },

    {  /* 69 */
	"water elemental",	FALSE,		0,
	0,		0,	OFF_FADE,
	0,		RES_WEAPON|RES_FIRE|RES_COLD,	0,
	A|H|Z,		A|D	
    },

    {  /* 70 */
	"fish",		FALSE,		0,
	0,		AFF_SWIM,	0,
	0,		0,		VULN_COLD|VULN_FIRE,
	A|H|Z,		A|D|E|F|J|K|O|X	
    },

    {  /* 71 */
	"gargoyle",	FALSE,		0,
	ACT_AGGRESSIVE,	AFF_FLYING,	OFF_FAST,
	0,		RES_WEAPON,	VULN_COLD|VULN_FIRE,
	A|G|W,		A|C|D|E|F|H|K|P
    },

    {  /* 72 */
	"ghost",	FALSE,		0,
	ACT_UNDEAD,	AFF_FLYING,	OFF_FADE,
	IMM_DROWNING|IMM_NEGATIVE|IMM_DISEASE|IMM_POISON,
	RES_WEAPON, VULN_HOLY,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {  /* 73 */
	"ghoul",	FALSE,		0,
	0,		0,		0,
	IMM_DISEASE,	RES_CHARM,	VULN_MAGIC,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {  /* 74 */		
	"gnoll",	FALSE,		DETECT_INVIS|DETECT_HIDDEN,
	0,		0,		OFF_DISARM,
	0,		0,		0,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {  /* 75 */		
	"harpy",	FALSE,		DETECT_INVIS|DETECT_HIDDEN,
	0,		AFF_FLYING,		OFF_DISARM,
	0,		RES_CHARM,		0,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K|P
    },

    {  /* 76 */		
	"lamia",	FALSE,		0,
	ACT_AGGRESSIVE,	0,		OFF_FAST|OFF_DODGE,
	0,		RES_CHARM,		0,
	A|G|V,		A|B|C|D|E|F|G|H|I|J|K|Q|U
    },

    {  /* 77 */		
	"lich",		FALSE,		0,
	0,		0,		OFF_FAST|OFF_DODGE,
	IMM_POISON|IMM_DISEASE|IMM_CHARM|IMM_LIGHTNING|IMM_COLD,	
	RES_WEAPON|RES_FIRE,	VULN_HOLY,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {  /* 77 */		
	"minotaur",	FALSE,		0,
	0,		0,		OFF_CRUSH|OFF_FAST|OFF_BASH,
	0,		0,		0,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K|W
    },

    {  /* 78 */		
	"ogre",		FALSE,		0,
	0,		AFF_INFRARED,	0,
	0,		RES_DISEASE,	0,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {  /* 79 */		
	"zombie",	FALSE,		0,
	0,		AFF_SLOW,	0,
	IMM_MAGIC|IMM_POISON|IMM_CHARM,	RES_COLD, VULN_WEAPON,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {  /* 80 */
	"horse",	FALSE, 		0,
	0,		0,		OFF_FAST|OFF_DODGE,
	0,		0,		0,	
	A|G|V,		A|C|D|E|F|J|K|Q
    },

    {  /* 81 */
	"unique",	FALSE,		0,
	0,		0,		0,
	0,		0,		0,		
	0,		0
    },

    {
	NULL, 0, 0, 0, 0, 0, 0
    }
};


const	struct	pc_race_type	pc_race_table	[MAX_PC_RACE]	=
{
  { "null race", "", 0, { 100, 100, 100, 100 ,100,100,100,100,100,100,100,100,100},
      { "" }, { 13, 13, 13, 13, 13 }, { 18, 18, 18, 18, 18 }, 0,0,0,0,0 },
 
/*
    {
	"race name", 	short name, 	points,	{ class multipliers },
	{ bonus skills },
	{ stat mods },	{ max stats },	size , hp_bonus, mana_bonus, 
	  practice_bonus, align
    },
    {
	"giant",	"Giant",500,{ -1, 100, -1, 100,-1,100,-1,100 ,-1, -1,-1,-1,100},
	{ "bash","enhanced damage" },
	{ 2, -4, 0, -4, 2 ,0},	{ 25, 15, 17, 16, 23,25 }, SIZE_HUGE,
	20,-20,0,CR_ALL, LANG_GIANT
    },

*/

      {
       "human",	"Human",	0,	{100,100,100,100,100,100,100,100,100 ,100,100,100,100},
       { "" },
       { 0, 0, 0, 0, 0 ,0},	{ 20, 20, 20, 20, 20,25 },SIZE_MEDIUM,
        0,0,0,CR_ALL,LANG_HUMAN
      },

      { 	
	"elf",		" Elf ",500,{ 100,100,100,100,100,-1,-1,100,100,-1 ,-1,-1,100}, 
	{ "" },
	{ -2, 5, 0, 2, -2,0 },	{ 18, 25, 21, 23, 16,25 }, SIZE_MEDIUM,
	-10,30,0,CR_GOOD,LANG_ELVISH
	},

    {
	"half-elf",	"H-Elf",200,{ 100,100,100,100,100,100,100,100,100,-1 ,-1,100,100},
	{ "" },
	{ -1, 2, 0, 1, -1,0 },	{ 19, 21, 21, 21, 19 ,25}, SIZE_MEDIUM,
	-10,10,0,CR_ALL,LANG_ELVISH
    },

    {
	"dark-elf",	"D-Elf",500,{ 100, 100, 100, 100,-1,100,100,-1,100 ,-1,-1,100,100},
	{ "" },
	{ -2, 4, 0, 2, -2 ,0},	{ 18, 24, 21, 24, 16,25 }, SIZE_MEDIUM,
	-10,20,0,CR_EVIL,LANG_ELVISH
    },

    {
	"rockseer","Rocks",500,{ 100, 100, 100, 100,-1,-1,100,100,100 ,-1,-1,-1,100},
	{ "wand" },
	{ -1, 2, 0, 4, -2 ,0},	{ 18, 24, 21, 24, 16,25 }, SIZE_MEDIUM,
	-10,20,0,CR_NEUTRAL,LANG_ELVISH
    },
    
    {
	"dwarf",	"Dwarf",250,{ 100, 100, -1, 100,-1,-1,-1,-1,100 ,-1,-1,-1,100},
	{ "berserk" },
	{ 0, 0, 1, -4, 5,0 },	{ 21, 19, 19, 18, 25,25 }, SIZE_MEDIUM,
	30,0,0,CR_ALL,LANG_DWARVISH
    },

    {
	"svirfnebli",	"Svirf",250,{ 100, 100, 100, 100,-1 ,-1,-1,-1 ,100, -1,-1,-1,100},
	{ "lore", "identify" },
	{ 0, -1, 3, 0, 0,0 },	{ 20, 19, 23, 18, 19 ,25}, SIZE_SMALL,
	10,0,2,CR_NEUTRAL,LANG_GNOMISH
    },

    {
	"duergar",	"Duerg",250,{ -1, 100, 100, 100,-1 ,100,-1,-1,-1 , -1,-1,100,-1},
	{ "berserk", "pick lock" },
	{ 2, -2, -1, 1, 1 ,0},	{ 22, 16, 18, 20, 21,25 }, SIZE_MEDIUM,
	25,-10,0,CR_EVIL,LANG_DWARVISH
    },

    {
	"arial",	"Arial",250,{ 100, 100, 100, 100,-1,100,100,-1,100 ,-1,-1,100,100},
	{ "" },
	{ -2, 3, 0, 5, -2 ,0},	{ 19, 23, 20, 25, 18,25 }, SIZE_MEDIUM,
	-10,50,0,CR_ALL,LANG_HUMAN
    },

    {
	"gnome",	"Gnome",300,{ 100, 100, 100, 100,-1,-1,-1,-1,100 ,-1,-1,-1,100},
	{ "identify" },
	{ -2, 3, 5, 0, -2 ,0},	{ 18, 23, 25, 20, 18 ,25}, SIZE_SMALL,
	0,10,5,CR_NEUTRAL,LANG_GNOMISH
    },

    {
	"storm giant",	"StGia",600,{ -1, 100, -1, 100,-1,-1,-1,100 ,-1,-1,-1,-1,-1},
	{ "bash","enhanced damage" ,"sword" },
	{ 5, -2, -2, -6, 3 ,0},	{ 24, 17, 17, 17, 21,25 }, SIZE_HUGE,
	20,-20,0,CR_GOOD,LANG_GIANT
    },

    {
	"cloud giant",	"ClGia",500,{ -1, 100, -1, 100,-1,-1,-1,100,-1,-1,-1,-1,-1},
	{ "bash","enhanced damage" , "sword"},
	{ 5, -4, -3, -6, 3 ,0},	{ 25, 16, 17, 16, 22,25 }, SIZE_HUGE,
	20,-20,0,CR_NEUTRAL,LANG_GIANT
    },

    {
	"fire giant",	"FiGia",500,{ -1, 100, -1, 100,-1,100,-1,100 ,-1, -1,-1,-1,-1},
	{ "bash","enhanced damage" , "sword"},
	{ 5, -4, -3, -6, 3 ,0},	{ 25, 15, 17, 16, 23,25 }, SIZE_HUGE,
	20,-20,0,CR_EVIL,LANG_GIANT
    },

    {
	"frost giant",	"FrGia",500,{ -1, 100, -1, 100,-1,100,-1,100 ,-1, -1,-1,-1,-1},
	{ "bash","enhanced damage","axe" },
	{ 5, -4, -3, -6, 3 ,0},	{ 25, 15, 17, 16, 23,25 }, SIZE_HUGE,
	20,-20,0,CR_EVIL,LANG_GIANT
    },


    {
	"felar",	"Felar",250,{ -1, -1, 100, 100,-1 ,100,100,100 ,100,-1,-1,-1,-1},
	{ "hand to hand" },
	{ -1, -1, 0, 2, 2 ,0},	{ 17, 17, 20, 23, 23,25 }, SIZE_MEDIUM,
	10,-10,0,CR_ALL,LANG_CAT
    },


 /*  {
	"dragon","Drago",750,{ 100, 100, -1,100,-1,-1,-1,-1,100,-1,-1,100,-1},
	{ "hand to hand" , "bash"},
	{ 2, 1, 0, -6, 2,0 },	{ 21, 19, 19, 18, 25,25 }, SIZE_GIANT,
	30,0,0,CR_ALL
    },

   {
	"black dragon","BlaDr",750,{ 100, 100, -1, 100,-1,-1,-1,-1,100 ,-1,-1,100,-1},
	{ "hand to hand" , "bash"},
	{ 2, 0, 0, -6, 2,0 },	{ 21, 19, 19, 18, 25,25 }, SIZE_GARGANTUAN,
	30,0,0,CR_EVIL
    },

    {
	"blue dragon","BluDr",750,{ 100, 100, -1, 100,-1,-1,-1,-1,100 ,-1,-1,100,-1},
	{ "hand to hand" , "bash"},
	{ 2, 1, 0, -6, 2,0 },	{ 21, 19, 19, 18, 25,25 }, SIZE_GARGANTUAN,
	30,0,0,CR_EVIL
    },

    {
	"green dragon","GReDr",750,{ 100, 100, -1, 100,-1,-1,-1,-1,100 ,-1,-1,100,-1},
	{ "hand to hand" , "bash"},
	{ 2, 1, 0, -6, 2,0 },	{ 21, 19, 19, 18, 25,25 }, SIZE_GARGANTUAN,
	30,0,0,CR_EVIL
    },

    {
	"red dragon","RedDr",1000,{ 100, 100, -1, 100,-1,-1,-1,-1,100 ,-1,-1,100,-1},
	{ "hand to hand" , "bash"},
	{ 2, 2, 0, -6, 2,0 },	{ 21, 19, 19, 18, 25,25 }, SIZE_GARGANTUAN,
	30,0,0,CR_EVIL
    },

    {
	"white dragon","WhiDr",750,{ 100, 100, -1, 100,-1,-1,-1,-1,100 ,-1,-1,100,-1},
	{ "hand to hand" , "bash"},
	{ 2, -2, 0, -6, 2,0 },	{ 21, 19, 19, 18, 25,25 }, SIZE_GIANT,
	30,0,0,CR_EVIL
    },

    {
	"brass dragon","WhiDr",750,{ 100, 100, -1, 100,-1,-1,-1,-1,100 ,-1,-1,100,-1},
	{ "hand to hand" , "bash"},
	{ 2, 0, 0, -6, 2,0 },	{ 21, 19, 19, 18, 25,25 }, SIZE_GARGANTUAN,
	30,0,0,CR_GOOD
    },

    {
	"gold dragon","WhiDr",1000,{ 100, 100, -1, 100,-1,-1,-1,-1,100 ,-1,-1,100,-1},
	{ "hand to hand" , "bash"},
	{ 2, 2, 0, -6, 2,0 },	{ 21, 19, 19, 18, 25,25 }, SIZE_GARGANTUAN,
	30,0,0,CR_GOOD
    },

    {
	"silver dragon","WhiDr",1000,{ 100, 100, -1, 100,-1,-1,-1,-1,100 ,-1,-1,100,-1},
	{ "hand to hand" , "bash"},
	{ 2, 2, 0, -6, 2,0 },	{ 21, 19, 19, 18, 25,25 }, SIZE_GARGANTUAN,
	30,0,0,CR_GOOD
    },

    {
	"bronze dragon","WhiDr",750,{ 100, 100, -1, 100,-1,-1,-1,-1,100 ,-1,-1,100,-1},
	{ "hand to hand" , "bash"},
	{ 2, 1, 0, -6, 2,0 },	{ 21, 19, 19, 18, 25,25 }, SIZE_GARGANTUAN,
	30,0,0,CR_GOOD
    },

    {
	"copper dragon","WhiDr",750,{ 100, 100, -1, 100,-1,-1,-1,-1,100 ,-1,-1,100,-1},
	{ "hand to hand" , "bash"},
	{ 2, 0, 0, -6, 2,0 },	{ 21, 19, 19, 18, 25,25 }, SIZE_GARGANTUAN,
	30,0,0,CR_GOOD
    },
*/
    {
	"githyanki",	"Githy",250,{ 100, -1, -1, 100,-1,-1,100,100,100 ,-1,-1,100,100},
	{ "sword" ,"dagger"},
	{ 0, 2, -2, 3, -1,0},	{ 19, 23, 20, 25, 18,25 }, SIZE_MEDIUM,
	0,0,0,CR_EVIL,LANG_HUMAN
    },

    {
	"satyr","Satyr",250,{ -1, -1, 100, 100,-1,-1,-1,100,-1 ,-1,-1,-1,-1},
	{ "camouflage" ,"ambush", "steal" },
	{ -1, 0, 0, 3, 0,0},	{ 19, 23, 20, 25, 18,25 }, SIZE_MEDIUM,
	0,0,0,CR_NEUTRAL,LANG_HUMAN
    },

    {
	"troll","Troll",400,{ -1,-1,-1,100,-1,100,-1,100,-1,-1,-1,-1,-1},
	{ "fast healing","trance" },
	{ 2, -2, -1, -2, 3,0},	{ 19, 23, 20, 25, 18,25 }, SIZE_LARGE,
	0,0,0,CR_EVIL,LANG_TROLLISH
    }

  };

	
      	

/*
 * Class table.
 */
const	struct	class_type	class_table	[MAX_CLASS]	=
{
    {
        "invoker", "Inv",  STAT_INT,  OBJ_VNUM_SCHOOL_DAGGER,
        { 3018, 0, 0, 0, 0, 0 },  75,  18, 6,  60,  100, TRUE,
        0,{0,3,0,0,0,5},CR_ALL
    },
      
    {
	"cleric", "Cle",  STAT_WIS,  OBJ_VNUM_SCHOOL_MACE,
	{ 3003, 9619, 9619, 9619, 9619, 9619},  75,  18, 2,  70, 80, TRUE,
	0,{0,0,3,0,0,5},CR_ALL
    },

    {
	"thief", "Thi",  STAT_DEX,  OBJ_VNUM_SCHOOL_DAGGER,
	{ 3028, 9639, 9639, 9639, 9639, 9639 },  75,  18,  -4, 100, 50, FALSE,
        0,{0,0,0,3,0,5},CR_ALL
    },

    {
	"warrior", "War",  STAT_STR,  OBJ_VNUM_SCHOOL_SWORD,
	{ 3022, 9633, 9633, 9633, 9633, 9633 },  75,  18,  -10, 100,50,FALSE,
	0,{3,0,0,0,1,5},CR_ALL
    },

    {
	"paladin", "Pal",  STAT_STR,  OBJ_VNUM_SCHOOL_SWORD,
	{ 3063, 0, 0, 0, 0, 0 },  75,  18,  -6,  80, 70, TRUE,
	400,{2,0,1,0,2,5},CR_GOOD
    },

    {
	"anti-paladin", "A-P",  STAT_STR,  OBJ_VNUM_SCHOOL_SWORD,
	{ 3061, 0, 0, 0, 0, 0 },  75,  18,  -6,  80, 70, TRUE,
	300,{2,1,0,1,1,5},CR_EVIL
    },

    {
	"ninja", "Nin",  STAT_DEX,  OBJ_VNUM_SCHOOL_DAGGER,
	{ 3062, 0, 0, 0, 0, 0 },  75,  18,  -10,  90, 60, FALSE,
	300,{1,0,0,2,0,5},CR_ALL
    },

    {
	"ranger", "Ran",  STAT_STR,  OBJ_VNUM_SCHOOL_SWORD,
	{ 6156, 0, 0, 0, 0, 0 },  75,  18,  -8,  90, 60, FALSE,
	200,{1,0,0,1,1,5},CR_ALL
    },

    {
        "transmuter", "Tra",  STAT_INT,  OBJ_VNUM_SCHOOL_DAGGER,
        { 3073, 0, 0, 0, 0, 0 },  75,  18, 2,  60,  100, TRUE,
        0,{0,3,0,0,0,5},CR_ALL
    },

    {
        "samurai", "Sam",  STAT_STR,  OBJ_VNUM_SCHOOL_SWORD,
        { 3075, 9633, 0, 0, 0, 0 },  75,  18,  -10,  100,60,FALSE,
        500,{2,1,1,1,1,5},CR_ALL
    },

   {
        "vampire", "Vam",  STAT_INT,  OBJ_VNUM_SCHOOL_DAGGER,
        { 5395, 5396, 0, 0, 0, 0 },  75,  18, -6,  80,  70, TRUE,
        300,{1,2,0,0,0,5},CR_EVIL
        },
      
   {
        "necromancer", "Nec",  STAT_INT,  OBJ_VNUM_SCHOOL_DAGGER,
        { 5393, 5394, 0, 0, 0, 0 },  75,  18, 6,  60,  100, TRUE,
        200,{0,2,1,0,0,5},CR_EVIL
   },

   {
        "elementalist", "Ele",  STAT_INT,  OBJ_VNUM_SCHOOL_DAGGER,
        { 9618, 0, 0, 0, 0, 0 },  75,  18, 6,  60,  100, TRUE,
        0,{0,3,0,0,0,5},CR_ALL
   }

      
};



/*
 * Titles. mage=100
 */
char *	const			title_table	[MAX_CLASS][MAX_LEVEL+1][2] =
{
    {
      { "Man",			"Woman"				},

      { "Apprentice of Magic",	"Apprentice of Magic"		},
      { "Spell Student",		"Spell Student"			},
      { "Scholar of Magic",		"Scholar of Magic"		},
      { "Delver in Spells",		"Delveress in Spells"		},
      { "Medium of Magic",		"Medium of Magic"		},
      
      { "Scribe of Magic",		"Scribess of Magic"		},
      { "Seer",			"Seeress"			},
      { "Sage",			"Sage"				},
      { "Illusionist",		"Illusionist"			},
      { "Abjurer",			"Abjuress"			},
      
      { "Invoker",			"Invoker"			},
      { "Invoker",			"Invoker"			},
      { "Enchanter",			"Enchantress"			},
      { "Enchanter",			"Enchantress"			},
      { "Conjurer",			"Conjuress"			},

      { "Conjurer",			"Conjuress"			},
      { "Magician",			"Witch"				},
      { "Magician",			"Witch"				},
      { "Creator",			"Creator"			},
      { "Creator",			"Creator"			},
      
      { "Savant",			"Savant"			},
      { "Savant",			"Savant"			},
      { "Magus",			"Craftess"			},
      { "Magus",			"Craftess"			},
      { "Wizard",			"Wizard"			},

      { "Wizard",			"Wizard"			},
      { "Warlock",			"War Witch"			},
      { "Warlock",			"War Witch"			},
      { "Sorcerer",			"Sorceress"			},
      { "Sorcerer",			"Sorceress"			},
      
      { "Elder Sorcerer",		"Elder Sorceress"		},
      { "Elder Sorcerer",		"Elder Sorceress"		},
      { "Grand Sorcerer",		"Grand Sorceress"		},
      { "Grand Sorcerer",		"Grand Sorceress"		},
      { "Great Sorcerer",		"Great Sorceress"		},

      { "Great Sorcerer",		"Great Sorceress"		},
      { "Golem Maker",		"Golem Maker"			},
      { "Golem Maker",		"Golem Maker"			},
      { "Greater Golem Maker",	"Greater Golem Maker"		},
      { "Greater Golem Maker",	"Greater Golem Maker"		},
      
	{ "Maker of Stones",		"Maker of Stones",		},
	{ "Maker of Stones",		"Maker of Stones",		},
	{ "Maker of Potions",		"Maker of Potions",		},
	{ "Maker of Potions",		"Maker of Potions",		},
	{ "Maker of Scrolls",		"Maker of Scrolls",		},

	{ "Maker of Scrolls",		"Maker of Scrolls",		},
	{ "Maker of Wands",		"Maker of Wands",		},
	{ "Maker of Wands",		"Maker of Wands",		},
	{ "Maker of Staves",		"Maker of Staves",		},
	{ "Maker of Staves",		"Maker of Staves",		},

	{ "Demon Summoner",		"Demon Summoner"		},
	{ "Demon Summoner",		"Demon Summoner"		},
	{ "Greater Demon Summoner",	"Greater Demon Summoner"	},
	{ "Greater Demon Summoner",	"Greater Demon Summoner"	},
	{ "Dragon Charmer",		"Dragon Charmer"		},

	{ "Dragon Charmer",		"Dragon Charmer"		},
	{ "Greater Dragon Charmer",	"Greater Dragon Charmer"	},
	{ "Greater Dragon Charmer",	"Greater Dragon Charmer"	},
      { "Master of Sorcery",  "Mistress of Sorcery"},
      { "Master of Sorcery",  "Mistress of Sorcery"},

      { "Master of Wizardry",  "Mistress of Wizardry"},
      { "Master of Wizardry",  "Mistress of Wizardry"},
      { "Master Enchanter",  "Master Enchanter"},
      { "Master Enchanter",  "Master Enchanter"},
      { "Master Summoner",  "Master Summoner"},

      { "Master Summoner",  "Master Summoner"},
      { "Master of Divination",  "Mistress of Divination"},
      { "Master of Divination",  "Mistress of Divination"},
      { "Master of Alteration",  "Mistress of Alteration"},
      { "Master of Alteration",  "Mistress of Alteration"},

      { "Master of Pyrotechniques",  "Mistress of Pyrotechniques"},
      { "Master of Pyrotechniques",  "Mistress of Pyrotechniques"},
      { "Master of Necromancy",  "Mistress of Necromancy"},
      { "Master of Necromancy",  "Mistress of Necromancy"},
      { "Master of Alchemy",  "Mistress of Alchemy"},

      { "Master of Alchemy",  "Mistress of Alchemy"},
      { "Spellbinder",  "Spellbinder"},
      { "Spellbinder",  "Spellbinder"},
      { "Master of Magic",  "Mistress of Magic"},
      { "Master of Magic",  "Mistress of Magic"},

      { "Master of the Past",  "Mistress of the Past"},
      { "Master of the Past",  "Mistress of the Past"},
      { "Master of the Present",  "Mistress of the Present"},
      { "Master of the Present",  "Mistress of the Present"},
      { "Master of the Future",  "Mistress of the Future"},

      { "Master of the Future",  "Mistress of the Future"},
      { "Arch Mage",  "Arch Mage"},
      { "Arch Mage",  "Arch Mage"},
      { "Mystical",  "Mystical"},
      { "Mystical",  "Mystical"},
      
	{ "Mage Hero",			"Mage Heroine"			},
	{ "Avatar of Magic",		"Avatar of Magic"		},
	{ "Angel of Magic",		"Angel of Magic"		},
	{ "Demigod of Magic",		"Demigoddess of Magic"		},
	{ "Immortal of Magic",		"Immortal of Magic"		},
	{ "God of Magic",		"Goddess of Magic"		},
	{ "Deity of Magic",		"Deity of Magic"		},
	{ "Supremity of Magic",		"Supremity of Magic"		},
	{ "Creator",			"Creator"			},
	{ "Implementor",		"Implementress"			}
    },

    {
	{ "Man",			"Woman"				},

	{ "Believer",			"Believer"			},
	{ "Attendant",			"Attendant"			},
	{ "Acolyte",			"Acolyte"			},
	{ "Novice",			"Novice"			},
	{ "Missionary",			"Missionary"			},

	{ "Adept",			"Adept"				},
	{ "Deacon",			"Deaconess"			},
	{ "Vicar",			"Vicaress"			},
	{ "Priest",			"Priestess"			},
	{ "Minister",			"Lady Minister"			},

	{ "Canon",			"Canon"				},
	{ "Canon",			"Canon"				},
	{ "Levite",			"Levitess"			},
	{ "Levite",			"Levitess"			},
	{ "Curate",			"Curess"			},

	{ "Curate",			"Curess"			},
	{ "Monk",			"Nun"				},
	{ "Monk",			"Nun"				},
	{ "Healer",			"Healess"			},
	{ "Healer",			"Healess"			},

	{ "Chaplain",			"Chaplain"			},
	{ "Chaplain",			"Chaplain"			},
	{ "Expositor",			"Expositress"			},
	{ "Expositor",			"Expositress"			},
	{ "Bishop",			"Bishop"			},

	{ "Bishop",			"Bishop"			},
	{ "Arch Bishop",		"Arch Lady of the Church"	},
	{ "Arch Bishop",		"Arch Lady of the Church"	},
	{ "Patriarch",			"Matriarch"			},
	{ "Patriarch",			"Matriarch"			},

	{ "Elder Patriarch",		"Elder Matriarch"		},
	{ "Elder Patriarch",		"Elder Matriarch"		},
	{ "Grand Patriarch",		"Grand Matriarch"		},
	{ "Grand Patriarch",		"Grand Matriarch"		},
	{ "Great Patriarch",		"Great Matriarch"		},

	{ "Great Patriarch",		"Great Matriarch"		},
	{ "Demon Killer",		"Demon Killer"			},
	{ "Demon Killer",		"Demon Killer"			},
	{ "Greater Demon Killer",	"Greater Demon Killer"		},
	{ "Greater Demon Killer",	"Greater Demon Killer"		},

	{ "Cardinal of the Sea",	"Cardinal of the Sea"		},
	{ "Cardinal of the Sea",	"Cardinal of the Sea"		},
	{ "Cardinal of the Earth",	"Cardinal of the Earth"		},
	{ "Cardinal of the Earth",	"Cardinal of the Earth"		},
	{ "Cardinal of the Air",	"Cardinal of the Air"		},

	{ "Cardinal of the Air",	"Cardinal of the Air"		},
	{ "Cardinal of the Ether",	"Cardinal of the Ether"		},
	{ "Cardinal of the Ether",	"Cardinal of the Ether"		},
	{ "Cardinal of the Heavens",	"Cardinal of the Heavens"	},
	{ "Cardinal of the Heavens",	"Cardinal of the Heavens"	},

	{ "Enlightened",  "Enlightened"},
	{ "Enlightened",  "Enlightened"},
	{ "Sacred",  "Sacred"},
	{ "Sacred",  "Sacred"},
	{ "Saint",  "Saint"},

	{ "Saint",  "Saint"},
	{ "Divine",  "Divine"},
	{ "Divine",  "Divine"},
	{ "Cardinal of Flowers",  "Cardinal of Flowers"},
	{ "Cardinal of Flowers",  "Cardinal of Flowers"},

	{ "Cardinal of Water",  "Cardinal of Water"},
	{ "Cardinal of Water",  "Cardinal of Water"},
	{ "Cardinal of Fire",  "Cardinal of Fire"},
	{ "Cardinal of Fire",  "Cardinal of Fire"},
	{ "Cardinal of Ice",  "Cardinal of Ice"},

	{ "Cardinal of Ice",  "Cardinal of Ice"},
	{ "Cardinal of the Winds",  "Cardinal of the Winds"},
	{ "Cardinal of the Winds",  "Cardinal of the Winds"},
	{ "Cardinal of Storms",  "Cardinal of Storms"},
	{ "Cardinal of Storms",  "Cardinal of Storms"},

	{ "Father of Earth",  "Mother of Earth"},
	{ "Father of Earth",  "Mother of Earth"},
	{ "Father of the Sea",  "Mother of the Sea"},
	{ "Father of the Sea",  "Mother of the Sea"},
	{ "Father of Time",  "Mother of Time"},

	{ "Father of Time",  "Mother of Time"},
	{ "Sacred Heart",  "Sacred Heart"},
	{ "Sacred Heart",  "Sacred Heart"},
	{ "Holy Father",  "Holy Mother"},
	{ "Holy Father",  "Holy Mother"},

	{ "Avatar of an Immortal",	"Avatar of an Immortal"		},
	{ "Avatar of an Immortal",	"Avatar of an Immortal"		},
	{ "Avatar of a Deity",		"Avatar of a Deity"		},
	{ "Avatar of a Deity",		"Avatar of a Deity"		},
	{ "Avatar of a Supremity",	"Avatar of a Supremity"		},

	{ "Avatar of a Supremity",	"Avatar of a Supremity"		},
	{ "Avatar of an Implementor",	"Avatar of an Implementor"	},
	{ "Avatar of an Implementor",	"Avatar of an Implementor"	},
	{ "Master of all Divinity",	"Mistress of all Divinity"	},
	{ "Master of all Divinity",	"Mistress of all Divinity"	},
	
	{ "Holy Hero",			"Holy Heroine"			},
	{ "Holy Avatar",		"Holy Avatar"			},
	{ "Angel",			"Angel"				},
	{ "Demigod",			"Demigoddess",			},
	{ "Immortal",			"Immortal"			},
	{ "God",			"Goddess"			},
	{ "Deity",			"Deity"				},
	{ "Supreme Master",		"Supreme Mistress"		},
        { "Creator",                    "Creator"                       },
	{ "Implementor",		"Implementress"			}
    },

    {
	{ "Man",			"Woman"				},

	{ "Pilferer",			"Pilferess"			},
	{ "Footpad",			"Footpad"			},
	{ "Filcher",			"Filcheress"			},
	{ "Pick-Pocket",		"Pick-Pocket"			},
	{ "Sneak",			"Sneak"				},

	{ "Pincher",			"Pincheress"			},
	{ "Cut-Purse",			"Cut-Purse"			},
	{ "Snatcher",			"Snatcheress"			},
	{ "Sharper",			"Sharpress"			},
	{ "Rogue",			"Rogue"				},

	{ "Robber",			"Robber"			},
	{ "Robber",			"Robber"			},
	{ "Magsman",			"Magswoman"			},
	{ "Magsman",			"Magswoman"			},
	{ "Highwayman",			"Highwaywoman"			},

	{ "Highwayman",			"Highwaywoman"			},
	{ "Burglar",			"Burglaress"			},
	{ "Burglar",			"Burglaress"			},
	{ "Thief",			"Thief"				},
	{ "Thief",			"Thief"				},

	{ "Knifer",			"Knifer"			},
	{ "Knifer",			"Knifer"			},
	{ "Quick-Blade",		"Quick-Blade"			},
	{ "Quick-Blade",		"Quick-Blade"			},
	{ "Killer",			"Murderess"			},

	{ "Killer",			"Murderess"			},
	{ "Brigand",			"Brigand"			},
	{ "Brigand",			"Brigand"			},
	{ "Cut-Throat",			"Cut-Throat"			},
	{ "Cut-Throat",			"Cut-Throat"			},

	{ "Spy",			"Spy"				},
	{ "Spy",			"Spy"				},
	{ "Grand Spy",			"Grand Spy"			},
	{ "Grand Spy",			"Grand Spy"			},
	{ "Master Spy",			"Master Spy"			},

	{ "Master Spy",			"Master Spy"			},
	{ "Assassin",			"Assassin"			},
	{ "Assassin",			"Assassin"			},
	{ "Greater Assassin",		"Greater Assassin"		},
	{ "Greater Assassin",		"Greater Assassin"		},

	{ "Master of Vision",		"Mistress of Vision"		},
	{ "Master of Vision",		"Mistress of Vision"		},
	{ "Master of Hearing",		"Mistress of Hearing"		},
	{ "Master of Hearing",		"Mistress of Hearing"		},
	{ "Master of Smell",		"Mistress of Smell"		},

	{ "Master of Smell",		"Mistress of Smell"		},
	{ "Master of Taste",		"Mistress of Taste"		},
	{ "Master of Taste",		"Mistress of Taste"		},
	{ "Master of Touch",		"Mistress of Touch"		},
	{ "Master of Touch",		"Mistress of Touch"		},

	{ "Crime Lord",			"Crime Mistress"		},
	{ "Crime Lord",			"Crime Mistress"		},
	{ "Infamous Crime Lord",	"Infamous Crime Mistress"	},
	{ "Infamous Crime Lord",	"Infamous Crime Mistress"	},
	{ "Greater Crime Lord",		"Greater Crime Mistress"	},

	{ "Greater Crime Lord",		"Greater Crime Mistress"	},
	{ "Master Crime Lord",		"Master Crime Mistress"		},
	{ "Master Crime Lord",		"Master Crime Mistress"		},
	{ "Godfather",			"Godmother"			},
	{ "Godfather",			"Godmother"			},

	{ "Crime Boss",  "Crime Boss"},
	{ "Crime Boss",  "Crime Boss"},
	{ "Kingpin",  "Kingpin"},
	{ "Kingpin",  "Kingpin"},
	{ "Lord of the Underworld",  "Lady of the Underworld"},

	{ "Lord of the Underworld",  "Lady of the Underworld"},
	{ "Shadow",  "Shadow"},
	{ "Shadow",  "Shadow"},
	{ "Master of the Night",  "Mistress of the Night"},
	{ "Master of the Night",  "Mistress of the Night"},

	{ "Master of Silence",  "Mistress of Silence"},
	{ "Master of Silence",  "Mistress of Silence"},
	{ "Master of Guile",  "Mistress of Guile"},
	{ "Master of Guile",  "Mistress of Guile"},
	{ "Master of the Blade",  "Mistress of the Blade"},

	{ "Master of the Blade",  "Mistress of the Blade"},
	{ "Master of Poison",  "Mistress of Poison"},
	{ "Master of Poison",  "Mistress of Poison"},
	{ "Master of Stealth",  "Mistress of Stealth"},
	{ "Master of Stealth",  "Mistress of Stealth"},

	{ "Master Thief",  "Master Thief"},
	{ "Master Thief",  "Master Thief"},
	{ "Master Assassin",  "Master Assassin"},
	{ "Master Assassin",  "Master Assassin"},
	{ "Unseen",  "Unseen"},

	{ "Unseen",  "Unseen"},
	{ "Grand Master of Thieves",  "Grand Mistress of Thieves"},
	{ "Grand Master of Thieves",  "Grand Mistress of Thieves"},
	{ "Grand Master of Assassins",  "Grand Mistress of Assassins"},
	{ "Grand Master of Assassins",  "Grand Mistress of Assassins"},
	
	{ "Assassin Hero",		"Assassin Heroine"		},
	{ "Avatar of Death",		"Avatar of Death",		},
	{ "Angel of Death",		"Angel of Death"		},
	{ "Demigod of Assassins",	"Demigoddess of Assassins"	},
	{ "Immortal Assasin",		"Immortal Assassin"		},
	{ "God of Assassins",		"God of Assassins",		},
	{ "Deity of Assassins",		"Deity of Assassins"		},
	{ "Supreme Master",		"Supreme Mistress"		},
        { "Creator",                    "Creator"                       },
	{ "Implementor",		"Implementress"			}
    },

    {
        { "Man",			"Woman"				},

	{ "Swordpupil",			"Swordpupil"			},
	{ "Recruit",			"Recruit"			},
	{ "Sentry",			"Sentress"			},
	{ "Fighter",			"Fighter"			},
	{ "Soldier",			"Soldier"			},

	{ "Warrior",			"Warrior"			},
	{ "Veteran",			"Veteran"			},
	{ "Swordsman",			"Swordswoman"			},
	{ "Fencer",			"Fenceress"			},
	{ "Combatant",			"Combatess"			},

	{ "Hero",			"Heroine"			},
	{ "Hero",			"Heroine"			},
	{ "Myrmidon",			"Myrmidon"			},
	{ "Myrmidon",			"Myrmidon"			},
	{ "Swashbuckler",		"Swashbuckleress"		},

	{ "Swashbuckler",		"Swashbuckleress"		},
	{ "Mercenary",			"Mercenaress"			},
	{ "Mercenary",			"Mercenaress"			},
	{ "Swordmaster",		"Swordmistress"			},
	{ "Swordmaster",		"Swordmistress"			},

	{ "Lieutenant",			"Lieutenant"			},
	{ "Lieutenant",			"Lieutenant"			},
	{ "Champion",			"Lady Champion"			},
	{ "Champion",			"Lady Champion"			},
	{ "Dragoon",			"Lady Dragoon"			},

	{ "Dragoon",			"Lady Dragoon"			},
	{ "Cavalier",			"Lady Cavalier"			},
	{ "Cavalier",			"Lady Cavalier"			},
	{ "Knight",			"Lady Knight"			},
	{ "Knight",			"Lady Knight"			},

	{ "Grand Knight",		"Grand Knight"			},
	{ "Grand Knight",		"Grand Knight"			},
	{ "Master Knight",		"Master Knight"			},
	{ "Master Knight",		"Master Knight"			},
	{ "Gladiator",			"Gladiator"			},

	{ "Gladiator",			"Gladiator"			},
	{ "Master Gladiator",		"Master Gladiator"		},
	{ "Master Gladiator",		"Master Gladiator"		},
	{ "Demon Slayer",		"Demon Slayer"			},
	{ "Demon Slayer",		"Demon Slayer"			},

	{ "Greater Demon Slayer",	"Greater Demon Slayer"		},
	{ "Greater Demon Slayer",	"Greater Demon Slayer"		},
	{ "Dragon Slayer",		"Dragon Slayer"			},
	{ "Dragon Slayer",		"Dragon Slayer"			},
	{ "Greater Dragon Slayer",	"Greater Dragon Slayer"		},

	{ "Greater Dragon Slayer",	"Greater Dragon Slayer"		},
	{ "Underlord",			"Underlord"			},
	{ "Underlord",			"Underlord"			},
	{ "Overlord",			"Overlord"			},
	{ "Overlord",			"Overlord"			},

	{ "Baron of Thunder",		"Baroness of Thunder"		},
	{ "Baron of Thunder",		"Baroness of Thunder"		},
	{ "Baron of Storms",		"Baroness of Storms"		},
	{ "Baron of Storms",		"Baroness of Storms"		},
	{ "Baron of Tornadoes",		"Baroness of Tornadoes"		},

	{ "Baron of Tornadoes",		"Baroness of Tornadoes"		},
	{ "Baron of Hurricanes",	"Baroness of Hurricanes"	},
	{ "Baron of Hurricanes",	"Baroness of Hurricanes"	},
	{ "Baron of Meteors",		"Baroness of Meteors"		},
	{ "Baron of Meteors",		"Baroness of Meteors"		},

	{ "Baron of Fire",  "Baroness of Fire"},
	{ "Baron of Fire",  "Baroness of Fire"},
	{ "Baron of Ice",  "Baroness of Ice"},
	{ "Baron of Ice",  "Baroness of Ice"},
	{ "Baron of Lightning",  "Baroness of Lightning"},

	{ "Baron of Lightning",  "Baroness of Lightning"},
	{ "Master of the Elements",  "Mistress of the Elements"},
	{ "Master of the Elements",  "Mistress of the Elements"},
	{ "Master of Copper",  "Mistress of Copper"},
	{ "Master of Copper",  "Mistress of Copper"},
	
	{ "Master of Bronze",  "Mistress of Bronze"},
	{ "Master of Bronze",  "Mistress of Bronze"},
	{ "Master of Brass",  "Mistress of Brass"},
	{ "Master of Brass",  "Mistress of Brass"},
	{ "Master of Iron",  "Mistress of Iron"},

	{ "Master of Iron",  "Mistress of Iron"},
	{ "Master of Steel",  "Mistress of Steel"},
	{ "Master of Steel",  "Mistress of Steel"},
	{ "Master of Mithril",  "Mistress of Mithril"},
	{ "Master of Mithril",  "Mistress of Mithril"},
	
	{ "Master of Adamantite",  "Mistress of Adamantite"},
	{ "Master of Adamantite",  "Mistress of Adamantite"},
	{ "Captain",  "Captain"},
	{ "Captain",  "Captain"},
	{ "General",  "General"},

	{ "General",  "General"},
	{ "Field Marshall",  "Field Marshall"},
	{ "Field Marshall",  "Field Marshall"},
	{ "Master of War",  "Mistress of War"},
	{ "Master of War",  "Mistress of War"},
	
	{ "Knight Hero",		"Knight Heroine"		},
	{ "Avatar of War",		"Avatar of War"			},
	{ "Angel of War",		"Angel of War"			},
	{ "Demigod of War",		"Demigoddess of War"		},
	{ "Immortal Warlord",		"Immortal Warlord"		},
	{ "God of War",			"God of War"			},
	{ "Deity of War",		"Deity of War"			},
	{ "Supreme Master of War",	"Supreme Mistress of War"	},
        { "Creator",                    "Creator"                       },
	{ "Implementor",		"Implementress"			}
    },

    {
      { "Man",                  	      "Woman"                   },
      { "Paladin Pupil",		  "Paladin Pupil"},
      { "Scullery Man",			  "Scullery Maid"},
      { "Squire Candidate",		  "Squire Candidate"},
      { "Shield Bearer",		  "Shield Bearer"},
      { "Sword Bearer",			  "Sword Bearer"},

      { "Bow Bearer",			  "Bow Bearer"},
      { "Standard Bearer",		  "Standard Bearer"},
      { "Horseman",			  "Horsewoman"},
      { "Squire Initiate",		  "Squire Initiate"},
      { "Squire",			  "Squire"},

      { "Footman",  "Footwoman"},
      { "Footman",  "Footwoman"},
      { "Pikeman", "Pikewoman"},
      { "Pikeman", "Pikewoman"},
      { "Bowman",  "Bowwoman"},

      { "Bowman",  "Bowwoman"},
      { "Swordsman",  "Swordswoman"},
      { "Swordsman",  "Swordswoman"},
      { "Honorable",  "Honorable"},
      { "Honorable",  "Honorable"},

      { "Noble",  "Noble"},
      { "Noble",  "Noble"},
      { "Trustworthy",  "Trustworthy"},
      { "Trustworthy",  "Trustworthy"},
      { "Truthful",  "Truthful"},

      { "Truthful",  "Truthful"},
      { "Chivalrous",  "Chivalrous"},
      { "Chivalrous",  "Chivalrous"},
      { "Paladin",  "Paladin"},
      { "Paladin",  "Paladin"},

      { "Questor",  "Questor"},
      { "Questor",  "Questor"},
      { "Cavalier",  "Cavalier"},
      { "Cavalier",  "Cavalier"},
      { "Champion",  "Champion"},

      { "Champion",  "Champion"},
      { "Knight of Renown",  "Knight of Renown"},
      { "Knight of Renown",  "Knight of Renown"},
      { "Paladin Knight",  "Paladin Lady"},
      { "Paladin Knight",  "Paladin Lady"},

      { "Templar Initiate",  "Templar Initiate"},
      { "Templar Initiate",  "Templar Initiate"},
      { "Priest-Knight",  "Priestess-Knight"},
      { "Priest-Knight",  "Priestess-Knight"},
      { "Knight of the Cross",  "Lady of the Cross"},

      { "Knight of the Cross",  "Lady of the Cross"},
      { "Champion of the Cross",  "Champion of the Cross"},
      { "Champion of the Cross",  "Champion of the Cross"},
      { "Knight Templar",  "Lady Templar"},
      { "Knight Templar",  "Lady Templar"},

      { "Templar",  "Templar"},
      { "Templar",  "Templar"},
      { "Templar Footman",  "Templar Footwoman"},
      { "Templar Footman",  "Templar Footwoman"},
      { "Templar Lieutenant",  "Templar Lieutenant"},

      { "Templar Lieutenant",  "Templar Lieutenant"},
      { "Templar Captain",  "Templar Captain"},
      { "Templar Captain",  "Templar Captain"},
      { "Templar Colonel",  "Templar Colonel"},
      { "Templar Colonel",  "Templar Colonel"},

      { "Templar General",  "Templar General"},
      { "Templar General",  "Templar General"},
      { "Templar Field Marshall",  "Templar Field Marshall"},
      { "Templar Field Marshall",  "Templar Field Marshall"},
      { "Healer-Knight",  "Healer-Lady"},

      { "Healer-Knight",  "Healer-Lady"},
      { "Avenger",  "Avenger"},
      { "Avenger",  "Avenger"},
      { "Crusader",  "Crusader"},
      { "Crusader",  "Crusader"},

      { "Leader of the Crusades",  "Leader of the Crusades"},
      { "Leader of the Crusades",  "Leader of the Crusades"},
      { "Champion of the Crusades",  "Champion of the Crusades"},
      { "Champion of the Crusades",  "Champion of the Crusades"},
      { "Champion of the Templars",  "Champion of the Templars"},

      { "Champion of the Templars",  "Champion of the Templars"},
      { "Priest of the Templars",  "Priestess of the Templars"},
      { "Priest of the Templars",  "Priestess of the Templars"},
      { "High Priest of the Templars",  "High Priestess of Templars"},
      { "High Priest of the Templars",  "High Priestess of Templars"},

      { "Lord of the Templars",  "Lady of the Templars"},
      { "Lord of the Templars",  "Lady of the Templars"},
      { "Hammer of Heretics",  "Hammer of Heretics"},
      { "Hammer of Heretics",  "Hammer of Heretics"},
      { "Slayer of Infidels",  "Slayer of Infidels"},

      { "Slayer of Infidels",  "Slayer of Infidels"},
      { "Pious",  "Pious"},
      { "Pious",  "Pious"},
      { "Holy Knight",  "Holy Lady"},
      { "Holy Knight",  "Holy Lady"},

      { "Paladin Hero",  "Paladin Heroine"},
      { "Avatar of Honor",		"Avatar of Honor"		},
      { "Angel of Honor",		"Angel of Honor"		},
      { "Demigod of Honor",		"Demigoddess of Honor"		},
      { "Immortal Knight",		"Immortal Knight"		},
      { "God of Honor",		"God of Honor"			},
      { "Deity of Honor",		"Deity of Honor"		},
      { "Supreme Master of Honor",	"Supreme Mistress of Honor"	},
      { "Creator",                    "Creator"                       },
      { "Implementor",		"Implementress"			}
    },
    {
      { "Man",				 "Woman" 			},

      { "Scum",				  "Scum"			},
      { "Bully",			  "Bully"		},
      { "Thug",  "Moll"},
      { "Brute",  "Bitch"},
      { "Ruffian",  "Butch"},

      { "Pillager",  "Pillager"},
      { "Destroyer",  "Destroyer"},
      { "Burner",  "Burner"},
      { "Hired Killer",  "Hired Killer"},
      { "Brigand",  			"Brigand"			},

      { "Mercenary",  "Mercenary"},
      { "Mercenary",  "Mercenary"},
      { "Black Sword",  "Black Sword"},
      { "Black Sword",  "Black Sword"},
      { "Crimson Sword",  "Crimson Sword"},

      { "Crimson Sword",  "Crimson Sword"},
      { "Sneaky",  "Sneaky"},
      { "Sneaky",  "Sneaky"},
      { "Cruel",  "Cruel"},
      { "Cruel",  "Cruel"},

      { "Stealer",  "Stealer"},
      { "Stealer",  "Stealer"},
      { "Infamous",  "Infamous"},
      { "Infamous",  "Infamous"},
      { "Hated",  "Hated"},

      { "Hated",  "Hated"},
      { "Complete Bastard",  "Complete Bitch"},
      { "Complete Bastard",  "Complete Bitch"},
      { "Anti-Paladin",  "Anti-Paladin"},
      { "Anti-Paladin",  "Anti-Paladin"},

      { "Evil Fighter",  "Evil Fighter"},
      { "Evil Fighter",  "Evil Fighter"},
      { "Rogue Knight",  "Rogue Lady"},
      { "Rogue Knight",  "Rogue Lady"},
      { "Evil Champion",  "Evil Champion"},

      { "Evil Champion",  "Evil Champion"},
      { "Dishonorable",  "Dishonorable"},
      { "Dishonorable",  "Dishonorable"},
      { "Black Knight",  "Black Lady"},
      { "Black Knight",  "Black Lady"},

      { "Crimson Knight",  "Crimson Lady"},
      { "Crimson Knight",  "Crimson Lady"},
      { "Knight of Brimstone",  "Lady of Brimstone"},
      { "Knight of Brimstone",  "Lady of Brimstone"},
      { "Knight of the Inverted Cross",  "Lady of the Inverted Cross"},

      { "Knight of the Inverted Cross",  "Lady of the Inverted Cross"},
      { "Knight of Pain",  "Lady of Pain"},
      { "Knight of Pain",  "Lady of Pain"},
      { "Knight of Legion",  "Lady of Legion"},
      { "Knight of Legion",  "Lady of Legion"},

      { "Footman of Legion",  "Footwoman of Legion"},
      { "Footman of Legion",  "Footwoman of Legion"},
      { "Cavalier of Legion",  "Cavalier of Legion"},
      { "Cavalier of Legion",  "Cavalier of Legion"},
      { "Captain of Legion",  "Captain of Legion"},

      { "Captain of Legion",  "Captain of Legion"},
      { "General of Legion",  "General of Legion"},
      { "General of Legion",  "General of Legion"},
      { "Field Marshall of Legion",  "Field Marshall of Legion"},
      { "Field Marshall of Legion",  "Field Marshall of Legion"},

      { "Knight of the Apocaplypse",  "Lady of the Apocalypse"},
      { "Knight of the Apocaplypse",  "Lady of the Apocalypse"},
      { "LightSlayer",  "LightSlayer"},
      { "LightSlayer",  "LightSlayer"},
      { "Invoker of Suffering",  "Invokeress of Suffering"},

      { "Invoker of Suffering",  "Invokeress of Suffering"},
      { "Necromancer",  "Necromancer"},
      { "Necromancer",  "Necromancer"},
      { "Evil Knight",  "Evil Lady"},
      { "Evil Knight",  "Evil Lady"},

      { "Evil Lord",  "Evil Baroness"},
      { "Evil Lord",  "Evil Baroness"},
      { "Evil King",  "Evil Queen"},
      { "Evil King",  "Evil Queen"},
      { "Herald of War",  "Herald of War"},

      { "Herald of War",  "Herald of War"},
      { "Spreader of Pestilence",  "Spreader of Pestilence"},
      { "Spreader of Pestilence",  "Spreader of Pestilence"},
      { "Bringer of Famine",  "Bringer of Famine"},
      { "Bringer of Famine",  "Bringer of Famine"},

      { "Harbringer of Death",  "Harbringer of Death"},
      { "Harbringer of Death",  "Harbringer of Death"},
      { "Unholy Bishop",  "Unholy Priestess"},
      { "Unholy Bishop",  "Unholy Priestess"},
      { "Slayer",  "Slayer"},

      { "Slayer",  "Slayer"},
      { "Evil Incarnate",  "Evil Incarnate"},
      { "Evil Incarnate",  "Evil Incarnate"},
      { "Unholy Knight",  "Unholy Lady"},
      { "Unholy Knight",  "Unholy Lady"},

      { "Anti-Hero",  "Anti-Heroine"},
      { "Avatar of Evil",		"Avatar of Evil"		},
      { "Angel of Evil",		"Angel of Evil"		},
      { "Demigod of Evil",		"Demigoddess of Evil"		},
      { "Immortal Anti-Knight",	"Immortal Anti-Knight"		},
      { "God of Evil",		"God of Evil"			},
      { "Deity of Evil",		"Deity of Evil"		},
      { "Supreme Master of Evil",	"Supreme Mistress of Evil"	},
      { "Creator",                    "Creator"                       },
      { "Implementor",		"Implementress"			}
    },
    {
      { "Man", "Woman" },

      { "Uke",  "Uke"},
      { "Tori",  "Tori"},
      { "White Belt",  "White Belt"},
      { "Yellow belt",  "Yellow Belt"},
      { "Green Belt",  "Green Belt"},

      { "3rd Brown",  "3rd Brown"},
      { "2nd Brown",  "2nd Brown"},
      { "1st Brown",  "1st Brown"},
      { "3rd Black",  "3rd Black"},
      { "2nd Black",  "2nd Black"},

      { "Okuiri",  "Okuiri"},
      { "Okuiri",  "Okuiri"},
      { "Shuto",  "Shuto"},
      { "Shuto",  "Shuto"},
      { "Uraken",  "Uraken"},

      { "Uraken",  "Uraken"},
      { "Shotei",  "Shotei"},
      { "Shotei",  "Shotei"},
      { "Tateken",  "Tateken"},
      { "Tateken",  "Tateken"},

      { "Seiken",  "Seiken"},
      { "Seiken",  "Seiken"},
      { "Ude",  "Ude"},
      { "Ude",  "Ude"},
      { "Empi",  "Empi"},

      { "Empi",  "Empi"},
      { "Josokutei",  "Josokotei"},
      { "Josokutei",  "Josokotei"},
      { "Kasokutei",  "Kasokutei"}, 
      { "Kasokutei",  "Kasokutei"}, 

      { "Master of Atemi Waza",  "Mistress of Atemi Waza"},
      { "Master of Atemi Waza",  "Mistress of Atemi Waza"},
      { "Master of Kotegaeshi",  "Mistress of Kotegaeshi"},
      { "Master of Kotegaeshi",  "Mistress of Kotegaeshi"},
      { "Master of Kansetsuwaza",  "Mistress of Kansetsuwaza"},

      { "Master of Kansetsuwaza",  "Mistress of Kansetsuwaza"},
      { "Master of Taisabaki",  "Mistress of Taisabaki"},
      { "Master of Taisabaki",  "Mistress of Taisabaki"},
      { "Master of Kyusho",  "Mistress of Kyusho"},
      { "Master of Kyusho",  "Mistress of Kyusho"},

      { "Student of Kamae",  "Student of Kamae"},
      { "Student of Kamae",  "Student of Kamae"},
      { "Master of Kamae",  "Mistress of Kamae"},
      { "Master of Kamae",  "Mistress of Kamae"},
      { "Master of Ukemi",  "Master of Ukemi"},

      { "Master of Ukemi",  "Master of Ukemi"},
      { "Mokuroku",  "Mokuroku"},
      { "Mokuroku",  "Mokuroku"},
      { "Ogoshi",  "Ogoshi"},
      { "Ogoshi",  "Ogoshi"},

      { "Ippon Seinage",  "Ippon Seinage"},
      { "Ippon Seinage",  "Ippon Seinage"},
      { "Koshi Garuma",  "Koshi Garuma"},
      { "Koshi Garuma",  "Koshi Garuma"},
      { "Sukuinage",  "Sukuinage"},

      { "Sukuinage",  "Sukuinage"},
      { "Osotogari",  "Osotogari"},
      { "Osotogari",  "Osotogari"},
      { "Uki Goshi",  "Uki Goshi"},	
      { "Uki Goshi",  "Uki Goshi"},	

      { "Taiotoshi",  "Taiotoshi"},
      { "Taiotoshi",  "Taiotoshi"},
      { "Harai Goshi",  "Harai Goshi"},
      { "Harai Goshi",  "Harai Goshi"},
      { "Yama Arashi",  "Yama Arashi"},

      { "Yama Arashi",  "Yama Arashi"},
      { "Master of Nage Waza",  "Mistress of Nage Waza"},
      { "Master of Nage Waza",  "Mistress of Nage Waza"},
      { "Entrance to Owaza",  "Entrance to Owaza"},
      { "Entrance to Owaza",  "Entrance to Owaza"},

      { "Novice of Owaza",  "Novice of Owaza"},
      { "Novice of Owaza",  "Novice of Owaza"},
      { "Student of Owaza",  "Student of Owaza"},
      { "Student of Owaza",  "Student of Owaza"},
      { "Learned of Owaza",  "Learned of Owaza"},

      { "Learned of Owaza",  "Learned of Owaza"},
      { "Master of Owaza",  "Mistress of Owaza"},
      { "Master of Owaza",  "Mistress of Owaza"},
      { "Menkyo",  "Menkyo"},
      { "Menkyo",  "Menkyo"},

      { "Sensei",  "Sensei"},
      { "Sensei",  "Sensei"},
      { "Shinan",  "Shinan"},
      { "Shinan",  "Shinan"},
      { "Shihan",  "Shihan"},

      { "Shihan",  "Shihan"},
      { "Kaiden",  "Kaiden"},
      { "Kaiden",  "Kaiden"},
      { "Master of the Miyama Ryu",  "Mistress of the Miyama Ryu"},
      { "Master of the Miyama Ryu",  "Mistress of the Miyama Ryu"},

      { "Ninja Hero",  "Ninja Heroine"},
	{ "Ninja Avatar",	"Ninja Avatar"		},
	{ "Ninja Angel",		"Ninja Angel"		},
	{ "Ninja Demigod",		"Ninja Demigoddess"		},
	{ "Immortal Ninja",	"Immortal Ninja"		},
	{ "Ninja God",		"Ninja God"			},
	{ "Ninja Deity",		"Ninja Deity" 		},
	{ "Supreme Ninja",	"Supreme Ninja"	},
        { "Creator",                    "Creator"                       },
	{ "Implementor",		"Implementress"			}
    },

    {
      { "Man",				 "Woman" 			},

      { "Forest Pupil",  "Forest Pupil"},
      { "Forest Recruit",  "Forest Recruit"},
      { "Forester",  "Forester"},
      { "Fletcher",  "Fletcher"},
      { "Bowyer",  "Bowyer"},

      { "Trapper",  "Trapper"},
      { "Hunter",  "Huntress"},
      { "Tracker",  "Tracker"},
      { "Seeker",  "Seeker"},
      { "Scout",			  "Scout"			},

      { "Master Scout",  "Mistress Scout"},
      { "Master Scout",  "Mistress Scout"},
      { "Green Man",  "Green Woman"},
      { "Green Man",  "Green Woman"},
      { "Archer",  "Archer"},

      { "Archer",  "Archer"},
      { "Apprentice Ranger",  "Apprentice Ranger"},
      { "Apprentice Ranger",  "Apprentice Ranger"},
      { "Woodsman",  "Woodswoman"},
      { "Woodsman",  "Woodswoman"},

      { "Master Woodsman",  "Mistress Woodswoman"},
      { "Master Woodsman",  "Mistress Woodswoman"},
      { "Ranger Initiate",  "Ranger Initiate"},
      { "Ranger Initiate",  "Ranger Initiate"},
      { "Ranger Candidate",  "Ranger Candidate"},

      { "Ranger Candidate",  "Ranger Candidate"},
      { "Ranger Squire",  "Ranger Squire"},
      { "Ranger Squire",  "Ranger Squire"},
      { "Ranger",  "Ranger"},
      { "Ranger",  "Ranger"},

      { "Ranger",  "Ranger"},
      { "Ranger",  "Ranger"},
      { "Ranger",  "Ranger"},
      { "Ranger",  "Ranger"},
      { "Ranger",  "Ranger"},

      { "Ranger",  "Ranger"},
      { "Ranger Captain",  "Ranger Captain"},
      { "Ranger Captain",  "Ranger Captain"},
      { "Warder",  "Warder"},
      { "Warder",  "Warder"},

      { "Warder Captain",  "Warder Captain"},
      { "Warder Captain",  "Warder Captain"},
      { "Warder General",  "Warder General"},
      { "Warder General",  "Warder General"},
      { "Master of Warders",  "Mistress of Warders"},

      { "Master of Warders",  "Mistress of Warders"},
      { "Knight of the Forest",  "Lady of the Forest"},
      { "Knight of the Forest",  "Lady of the Forest"},
      { "Sword of the Forest",  "Sword of the Forest"},
      { "Sword of the Forest",  "Sword of the Forest"},

      { "Guardian of the Forest",  "Guardian of the Forest"},
      { "Guardian of the Forest",  "Guardian of the Forest"},
      { "Lord of the Forest",  "Queen of the Forest"},
      { "Lord of the Forest",  "Queen of the Forest"},
      { "Overlord of the Forest",  "Overlord of the Forest"},

      { "Overlord of the Forest",  "Overlord of the Forest"},
      { "Baron of Forests",  "Baroness of Forests"},
      { "Baron of Forests",  "Baroness of Forests"},
      { "Keeper",  "Keeper"},
      { "Keeper",  "Keeper"},

      { "Huntsman",  "Huntswoman"},
      { "Huntsman",  "Huntswoman"},
      { "Caller of the Pack",  "Caller of the Pack"},
      { "Caller of the Pack",  "Caller of the Pack"},
      { "Master of the Pack",  "Mistress of the Pack"},

      { "Master of the Pack",  "Mistress of the Pack"},
      { "Ranger Knight Initiate",  "Ranger Lady Initiate"},
      { "Ranger Knight Initiate",  "Ranger Lady Initiate"},
      { "Ranger Knight",  "Ranger Lady"},
      { "Ranger Knight",  "Ranger Lady"},

      { "Ranger Lord",  "Ranger Lady"},
      { "Ranger Lord",  "Ranger Lady"},
      { "Ranger Baron", "Ranger Baroness"},
      { "Ranger Baron", "Ranger Baroness"},
      { "Ranger Prince",  "Ranger Princess"},

      { "Ranger Prince",  "Ranger Princess"},
      { "Ranger King",  "Ranger Queen"},
      { "Ranger King",  "Ranger Queen"},
      { "Warder of the Glade",  "Warder of the Glade"},
      { "Warder of the Glade",  "Warder of the Glade"},

      { "Keeper of the Glade",  "Keeper of the Glade"},
      { "Keeper of the Glade",  "Keeper of the Glade"},
      { "Knight of the Glade",  "Lady of the Glade"},
      { "Knight of the Glade",  "Lady of the Glade"},
      {"Master of the Glade",  "Mistress of the Glade"},

      {"Master of the Glade",  "Mistress of the Glade"},
      { "King of the Glade",  "Queen of the Glade"},
      { "King of the Glade",  "Queen of the Glade"},
      { "Watcher",  "Watcher"},
      { "Watcher",  "Watcher"},

      { "Ranger Hero",  "Ranger Heroine"},
	{ "Ranger Avatar",	"Ranger Avatar"		},
	{ "Ranger Angel",		"Ranger Angel"		},
	{ "Ranger Demigod",		"Ranger Demigoddess"		},
	{ "Immortal Ranger",	"Immortal Ranger"		},
	{ "Ranger God",		"Ranger God"			},
	{ "Ranger Deity",		"Ranger Deity" 		},
	{ "Supreme Ranger",	"Supreme Ranger"	},
        { "Creator",                    "Creator"                       },
	{ "Implementor",		"Implementress"			}
    },
    {
      { "Man",			"Woman"				},

      { "Apprentice of Magic",	"Apprentice of Magic"		},
      { "Spell Student",		"Spell Student"			},
      { "Scholar of Magic",		"Scholar of Magic"		},
      { "Delver in Spells",		"Delveress in Spells"		},
      { "Medium of Magic",		"Medium of Magic"		},
      
      { "Scribe of Magic",		"Scribess of Magic"		},
      { "Seer",			"Seeress"			},
      { "Sage",			"Sage"				},
      { "Illusionist",		"Illusionist"			},
      { "Abjurer",			"Abjuress"			},
      
      { "Invoker",			"Invoker"			},
      { "Invoker",			"Invoker"			},
      { "Enchanter",			"Enchantress"			},
      { "Enchanter",			"Enchantress"			},
      { "Conjurer",			"Conjuress"			},

      { "Conjurer",			"Conjuress"			},
      { "Magician",			"Witch"				},
      { "Magician",			"Witch"				},
      { "Creator",			"Creator"			},
      { "Creator",			"Creator"			},
      
      { "Savant",			"Savant"			},
      { "Savant",			"Savant"			},
      { "Magus",			"Craftess"			},
      { "Magus",			"Craftess"			},
      { "Wizard",			"Wizard"			},

      { "Wizard",			"Wizard"			},
      { "Warlock",			"War Witch"			},
      { "Warlock",			"War Witch"			},
      { "Sorcerer",			"Sorceress"			},
      { "Sorcerer",			"Sorceress"			},
      
      { "Elder Sorcerer",		"Elder Sorceress"		},
      { "Elder Sorcerer",		"Elder Sorceress"		},
      { "Grand Sorcerer",		"Grand Sorceress"		},
      { "Grand Sorcerer",		"Grand Sorceress"		},
      { "Great Sorcerer",		"Great Sorceress"		},

      { "Great Sorcerer",		"Great Sorceress"		},
      { "Golem Maker",		"Golem Maker"			},
      { "Golem Maker",		"Golem Maker"			},
      { "Greater Golem Maker",	"Greater Golem Maker"		},
      { "Greater Golem Maker",	"Greater Golem Maker"		},
      
	{ "Maker of Stones",		"Maker of Stones",		},
	{ "Maker of Stones",		"Maker of Stones",		},
	{ "Maker of Potions",		"Maker of Potions",		},
	{ "Maker of Potions",		"Maker of Potions",		},
	{ "Maker of Scrolls",		"Maker of Scrolls",		},

	{ "Maker of Scrolls",		"Maker of Scrolls",		},
	{ "Maker of Wands",		"Maker of Wands",		},
	{ "Maker of Wands",		"Maker of Wands",		},
	{ "Maker of Staves",		"Maker of Staves",		},
	{ "Maker of Staves",		"Maker of Staves",		},

	{ "Demon Summoner",		"Demon Summoner"		},
	{ "Demon Summoner",		"Demon Summoner"		},
	{ "Greater Demon Summoner",	"Greater Demon Summoner"	},
	{ "Greater Demon Summoner",	"Greater Demon Summoner"	},
	{ "Dragon Charmer",		"Dragon Charmer"		},

	{ "Dragon Charmer",		"Dragon Charmer"		},
	{ "Greater Dragon Charmer",	"Greater Dragon Charmer"	},
	{ "Greater Dragon Charmer",	"Greater Dragon Charmer"	},
      { "Master of Sorcery",  "Mistress of Sorcery"},
      { "Master of Sorcery",  "Mistress of Sorcery"},

      { "Master of Wizardry",  "Mistress of Wizardry"},
      { "Master of Wizardry",  "Mistress of Wizardry"},
      { "Master Enchanter",  "Master Enchanter"},
      { "Master Enchanter",  "Master Enchanter"},
      { "Master Summoner",  "Master Summoner"},

      { "Master Summoner",  "Master Summoner"},
      { "Master of Divination",  "Mistress of Divination"},
      { "Master of Divination",  "Mistress of Divination"},
      { "Master of Alteration",  "Mistress of Alteration"},
      { "Master of Alteration",  "Mistress of Alteration"},

      { "Master of Pyrotechniques",  "Mistress of Pyrotechniques"},
      { "Master of Pyrotechniques",  "Mistress of Pyrotechniques"},
      { "Master of Necromancy",  "Mistress of Necromancy"},
      { "Master of Necromancy",  "Mistress of Necromancy"},
      { "Master of Alchemy",  "Mistress of Alchemy"},

      { "Master of Alchemy",  "Mistress of Alchemy"},
      { "Spellbinder",  "Spellbinder"},
      { "Spellbinder",  "Spellbinder"},
      { "Master of Magic",  "Mistress of Magic"},
      { "Master of Magic",  "Mistress of Magic"},

      { "Master of the Past",  "Mistress of the Past"},
      { "Master of the Past",  "Mistress of the Past"},
      { "Master of the Present",  "Mistress of the Present"},
      { "Master of the Present",  "Mistress of the Present"},
      { "Master of the Future",  "Mistress of the Future"},

      { "Master of the Future",  "Mistress of the Future"},
      { "Arch Mage",  "Arch Mage"},
      { "Arch Mage",  "Arch Mage"},
      { "Mystical",  "Mystical"},
      { "Mystical",  "Mystical"},
      
	{ "Mage Hero",			"Mage Heroine"			},
	{ "Avatar of Magic",		"Avatar of Magic"		},
	{ "Angel of Magic",		"Angel of Magic"		},
	{ "Demigod of Magic",		"Demigoddess of Magic"		},
	{ "Immortal of Magic",		"Immortal of Magic"		},
	{ "God of Magic",		"Goddess of Magic"		},
	{ "Deity of Magic",		"Deity of Magic"		},
	{ "Supremity of Magic",		"Supremity of Magic"		},
	{ "Creator",			"Creator"			},
	{ "Implementor",		"Implementress"			}
    },

    {
        { "Man",			"Woman"				},

	{ "Swordpupil",			"Swordpupil"			},
	{ "Recruit",			"Recruit"			},
	{ "Sentry",			"Sentress"			},
	{ "Fighter",			"Fighter"			},
	{ "Soldier",			"Soldier"			},

	{ "Warrior",			"Warrior"			},
	{ "Veteran",			"Veteran"			},
	{ "Swordsman",			"Swordswoman"			},
	{ "Fencer",			"Fenceress"			},
	{ "Combatant",			"Combatess"			},

	{ "Hero",			"Heroine"			},
	{ "Hero",			"Heroine"			},
	{ "Myrmidon",			"Myrmidon"			},
	{ "Myrmidon",			"Myrmidon"			},
	{ "Swashbuckler",		"Swashbuckleress"		},

	{ "Swashbuckler",		"Swashbuckleress"		},
	{ "Mercenary",			"Mercenaress"			},
	{ "Mercenary",			"Mercenaress"			},
	{ "Swordmaster",		"Swordmistress"			},
	{ "Swordmaster",		"Swordmistress"			},

	{ "Lieutenant",			"Lieutenant"			},
	{ "Lieutenant",			"Lieutenant"			},
	{ "Champion",			"Lady Champion"			},
	{ "Champion",			"Lady Champion"			},
	{ "Dragoon",			"Lady Dragoon"			},

	{ "Dragoon",			"Lady Dragoon"			},
	{ "Cavalier",			"Lady Cavalier"			},
	{ "Cavalier",			"Lady Cavalier"			},
	{ "Knight",			"Lady Knight"			},
	{ "Knight",			"Lady Knight"			},

	{ "Grand Knight",		"Grand Knight"			},
	{ "Grand Knight",		"Grand Knight"			},
	{ "Master Knight",		"Master Knight"			},
	{ "Master Knight",		"Master Knight"			},
	{ "Gladiator",			"Gladiator"			},

	{ "Gladiator",			"Gladiator"			},
	{ "Master Gladiator",		"Master Gladiator"		},
	{ "Master Gladiator",		"Master Gladiator"		},
	{ "Demon Slayer",		"Demon Slayer"			},
	{ "Demon Slayer",		"Demon Slayer"			},

	{ "Greater Demon Slayer",	"Greater Demon Slayer"		},
	{ "Greater Demon Slayer",	"Greater Demon Slayer"		},
	{ "Dragon Slayer",		"Dragon Slayer"			},
	{ "Dragon Slayer",		"Dragon Slayer"			},
	{ "Greater Dragon Slayer",	"Greater Dragon Slayer"		},

	{ "Greater Dragon Slayer",	"Greater Dragon Slayer"		},
	{ "Underlord",			"Underlord"			},
	{ "Underlord",			"Underlord"			},
	{ "Overlord",			"Overlord"			},
	{ "Overlord",			"Overlord"			},

	{ "Baron of Thunder",		"Baroness of Thunder"		},
	{ "Baron of Thunder",		"Baroness of Thunder"		},
	{ "Baron of Storms",		"Baroness of Storms"		},
	{ "Baron of Storms",		"Baroness of Storms"		},
	{ "Baron of Tornadoes",		"Baroness of Tornadoes"		},

	{ "Baron of Tornadoes",		"Baroness of Tornadoes"		},
	{ "Baron of Hurricanes",	"Baroness of Hurricanes"	},
	{ "Baron of Hurricanes",	"Baroness of Hurricanes"	},
	{ "Baron of Meteors",		"Baroness of Meteors"		},
	{ "Baron of Meteors",		"Baroness of Meteors"		},

	{ "Baron of Fire",  "Baroness of Fire"},
	{ "Baron of Fire",  "Baroness of Fire"},
	{ "Baron of Ice",  "Baroness of Ice"},
	{ "Baron of Ice",  "Baroness of Ice"},
	{ "Baron of Lightning",  "Baroness of Lightning"},

	{ "Baron of Lightning",  "Baroness of Lightning"},
	{ "Master of the Elements",  "Mistress of the Elements"},
	{ "Master of the Elements",  "Mistress of the Elements"},
	{ "Master of Copper",  "Mistress of Copper"},
	{ "Master of Copper",  "Mistress of Copper"},
	
	{ "Master of Bronze",  "Mistress of Bronze"},
	{ "Master of Bronze",  "Mistress of Bronze"},
	{ "Master of Brass",  "Mistress of Brass"},
	{ "Master of Brass",  "Mistress of Brass"},
	{ "Master of Iron",  "Mistress of Iron"},

	{ "Master of Iron",  "Mistress of Iron"},
	{ "Master of Steel",  "Mistress of Steel"},
	{ "Master of Steel",  "Mistress of Steel"},
	{ "Master of Mithril",  "Mistress of Mithril"},
	{ "Master of Mithril",  "Mistress of Mithril"},
	
	{ "Master of Adamantite",  "Mistress of Adamantite"},
	{ "Master of Adamantite",  "Mistress of Adamantite"},
	{ "Captain",  "Captain"},
	{ "Captain",  "Captain"},
	{ "General",  "General"},

	{ "General",  "General"},
	{ "Field Marshall",  "Field Marshall"},
	{ "Field Marshall",  "Field Marshall"},
	{ "Master of War",  "Mistress of War"},
	{ "Master of War",  "Mistress of War"},
	
	{ "Knight Hero",		"Knight Heroine"		},
	{ "Avatar of War",		"Avatar of War"			},
	{ "Angel of War",		"Angel of War"			},
	{ "Demigod of War",		"Demigoddess of War"		},
	{ "Immortal Warlord",		"Immortal Warlord"		},
	{ "God of War",			"God of War"			},
	{ "Deity of War",		"Deity of War"			},
	{ "Supreme Master of War",	"Supreme Mistress of War"	},
        { "Creator",                    "Creator"                       },
	{ "Implementor",		"Implementress"			}
    },
    {
      { "Man",			"Woman"				},

      { "Apprentice of Magic",	"Apprentice of Magic"		},
      { "Spell Student",		"Spell Student"			},
      { "Scholar of Magic",		"Scholar of Magic"		},
      { "Delver in Spells",		"Delveress in Spells"		},
      { "Medium of Magic",		"Medium of Magic"		},
      
      { "Scribe of Magic",		"Scribess of Magic"		},
      { "Seer",			"Seeress"			},
      { "Sage",			"Sage"				},
      { "Illusionist",		"Illusionist"			},
      { "Abjurer",			"Abjuress"			},
      
      { "Invoker",			"Invoker"			},
      { "Invoker",			"Invoker"			},
      { "Enchanter",			"Enchantress"			},
      { "Enchanter",			"Enchantress"			},
      { "Conjurer",			"Conjuress"			},

      { "Conjurer",			"Conjuress"			},
      { "Magician",			"Witch"				},
      { "Magician",			"Witch"				},
      { "Creator",			"Creator"			},
      { "Creator",			"Creator"			},
      
      { "Savant",			"Savant"			},
      { "Savant",			"Savant"			},
      { "Magus",			"Craftess"			},
      { "Magus",			"Craftess"			},
      { "Wizard",			"Wizard"			},

      { "Wizard",			"Wizard"			},
      { "Warlock",			"War Witch"			},
      { "Warlock",			"War Witch"			},
      { "Sorcerer",			"Sorceress"			},
      { "Sorcerer",			"Sorceress"			},
      
      { "Elder Sorcerer",		"Elder Sorceress"		},
      { "Elder Sorcerer",		"Elder Sorceress"		},
      { "Grand Sorcerer",		"Grand Sorceress"		},
      { "Grand Sorcerer",		"Grand Sorceress"		},
      { "Great Sorcerer",		"Great Sorceress"		},

      { "Great Sorcerer",		"Great Sorceress"		},
      { "Golem Maker",		"Golem Maker"			},
      { "Golem Maker",		"Golem Maker"			},
      { "Greater Golem Maker",	"Greater Golem Maker"		},
      { "Greater Golem Maker",	"Greater Golem Maker"		},
      
	{ "Maker of Stones",		"Maker of Stones",		},
	{ "Maker of Stones",		"Maker of Stones",		},
	{ "Maker of Potions",		"Maker of Potions",		},
	{ "Maker of Potions",		"Maker of Potions",		},
	{ "Maker of Scrolls",		"Maker of Scrolls",		},

	{ "Maker of Scrolls",		"Maker of Scrolls",		},
	{ "Maker of Wands",		"Maker of Wands",		},
	{ "Maker of Wands",		"Maker of Wands",		},
	{ "Maker of Staves",		"Maker of Staves",		},
	{ "Maker of Staves",		"Maker of Staves",		},

	{ "Demon Summoner",		"Demon Summoner"		},
	{ "Demon Summoner",		"Demon Summoner"		},
	{ "Greater Demon Summoner",	"Greater Demon Summoner"	},
	{ "Greater Demon Summoner",	"Greater Demon Summoner"	},
	{ "Dragon Charmer",		"Dragon Charmer"		},

	{ "Dragon Charmer",		"Dragon Charmer"		},
	{ "Greater Dragon Charmer",	"Greater Dragon Charmer"	},
	{ "Greater Dragon Charmer",	"Greater Dragon Charmer"	},
      { "Master of Sorcery",  "Mistress of Sorcery"},
      { "Master of Sorcery",  "Mistress of Sorcery"},

      { "Master of Wizardry",  "Mistress of Wizardry"},
      { "Master of Wizardry",  "Mistress of Wizardry"},
      { "Master Enchanter",  "Master Enchanter"},
      { "Master Enchanter",  "Master Enchanter"},
      { "Master Summoner",  "Master Summoner"},

      { "Master Summoner",  "Master Summoner"},
      { "Master of Divination",  "Mistress of Divination"},
      { "Master of Divination",  "Mistress of Divination"},
      { "Master of Alteration",  "Mistress of Alteration"},
      { "Master of Alteration",  "Mistress of Alteration"},

      { "Master of Pyrotechniques",  "Mistress of Pyrotechniques"},
      { "Master of Pyrotechniques",  "Mistress of Pyrotechniques"},
      { "Master of Necromancy",  "Mistress of Necromancy"},
      { "Master of Necromancy",  "Mistress of Necromancy"},
      { "Master of Alchemy",  "Mistress of Alchemy"},

      { "Master of Alchemy",  "Mistress of Alchemy"},
      { "Spellbinder",  "Spellbinder"},
      { "Spellbinder",  "Spellbinder"},
      { "Master of Magic",  "Mistress of Magic"},
      { "Master of Magic",  "Mistress of Magic"},

      { "Master of the Past",  "Mistress of the Past"},
      { "Master of the Past",  "Mistress of the Past"},
      { "Master of the Present",  "Mistress of the Present"},
      { "Master of the Present",  "Mistress of the Present"},
      { "Master of the Future",  "Mistress of the Future"},

      { "Master of the Future",  "Mistress of the Future"},
      { "Arch Mage",  "Arch Mage"},
      { "Arch Mage",  "Arch Mage"},
      { "Mystical",  "Mystical"},
      { "Mystical",  "Mystical"},
      
	{ "Mage Hero",			"Mage Heroine"			},
	{ "Avatar of Magic",		"Avatar of Magic"		},
	{ "Angel of Magic",		"Angel of Magic"		},
	{ "Demigod of Magic",		"Demigoddess of Magic"		},
	{ "Immortal of Magic",		"Immortal of Magic"		},
	{ "God of Magic",		"Goddess of Magic"		},
	{ "Deity of Magic",		"Deity of Magic"		},
	{ "Supremity of Magic",		"Supremity of Magic"		},
	{ "Creator",			"Creator"			},
	{ "Implementor",		"Implementress"			}
    },

    {
      { "Man",			"Woman"				},

      { "Apprentice of Magic",	"Apprentice of Magic"		},
      { "Spell Student",		"Spell Student"			},
      { "Scholar of Magic",		"Scholar of Magic"		},
      { "Delver in Spells",		"Delveress in Spells"		},
      { "Medium of Magic",		"Medium of Magic"		},
      
      { "Scribe of Magic",		"Scribess of Magic"		},
      { "Seer",			"Seeress"			},
      { "Sage",			"Sage"				},
      { "Illusionist",		"Illusionist"			},
      { "Abjurer",			"Abjuress"			},
      
      { "Invoker",			"Invoker"			},
      { "Invoker",			"Invoker"			},
      { "Enchanter",			"Enchantress"			},
      { "Enchanter",			"Enchantress"			},
      { "Conjurer",			"Conjuress"			},

      { "Conjurer",			"Conjuress"			},
      { "Magician",			"Witch"				},
      { "Magician",			"Witch"				},
      { "Creator",			"Creator"			},
      { "Creator",			"Creator"			},
      
      { "Savant",			"Savant"			},
      { "Savant",			"Savant"			},
      { "Magus",			"Craftess"			},
      { "Magus",			"Craftess"			},
      { "Wizard",			"Wizard"			},

      { "Wizard",			"Wizard"			},
      { "Warlock",			"War Witch"			},
      { "Warlock",			"War Witch"			},
      { "Sorcerer",			"Sorceress"			},
      { "Sorcerer",			"Sorceress"			},
      
      { "Elder Sorcerer",		"Elder Sorceress"		},
      { "Elder Sorcerer",		"Elder Sorceress"		},
      { "Grand Sorcerer",		"Grand Sorceress"		},
      { "Grand Sorcerer",		"Grand Sorceress"		},
      { "Great Sorcerer",		"Great Sorceress"		},

      { "Great Sorcerer",		"Great Sorceress"		},
      { "Golem Maker",		"Golem Maker"			},
      { "Golem Maker",		"Golem Maker"			},
      { "Greater Golem Maker",	"Greater Golem Maker"		},
      { "Greater Golem Maker",	"Greater Golem Maker"		},
      
	{ "Maker of Stones",		"Maker of Stones",		},
	{ "Maker of Stones",		"Maker of Stones",		},
	{ "Maker of Potions",		"Maker of Potions",		},
	{ "Maker of Potions",		"Maker of Potions",		},
	{ "Maker of Scrolls",		"Maker of Scrolls",		},

	{ "Maker of Scrolls",		"Maker of Scrolls",		},
	{ "Maker of Wands",		"Maker of Wands",		},
	{ "Maker of Wands",		"Maker of Wands",		},
	{ "Maker of Staves",		"Maker of Staves",		},
	{ "Maker of Staves",		"Maker of Staves",		},

	{ "Demon Summoner",		"Demon Summoner"		},
	{ "Demon Summoner",		"Demon Summoner"		},
	{ "Greater Demon Summoner",	"Greater Demon Summoner"	},
	{ "Greater Demon Summoner",	"Greater Demon Summoner"	},
	{ "Dragon Charmer",		"Dragon Charmer"		},

	{ "Dragon Charmer",		"Dragon Charmer"		},
	{ "Greater Dragon Charmer",	"Greater Dragon Charmer"	},
	{ "Greater Dragon Charmer",	"Greater Dragon Charmer"	},
      { "Master of Sorcery",  "Mistress of Sorcery"},
      { "Master of Sorcery",  "Mistress of Sorcery"},

      { "Master of Wizardry",  "Mistress of Wizardry"},
      { "Master of Wizardry",  "Mistress of Wizardry"},
      { "Master Enchanter",  "Master Enchanter"},
      { "Master Enchanter",  "Master Enchanter"},
      { "Master Summoner",  "Master Summoner"},

      { "Master Summoner",  "Master Summoner"},
      { "Master of Divination",  "Mistress of Divination"},
      { "Master of Divination",  "Mistress of Divination"},
      { "Master of Alteration",  "Mistress of Alteration"},
      { "Master of Alteration",  "Mistress of Alteration"},

      { "Master of Pyrotechniques",  "Mistress of Pyrotechniques"},
      { "Master of Pyrotechniques",  "Mistress of Pyrotechniques"},
      { "Master of Necromancy",  "Mistress of Necromancy"},
      { "Master of Necromancy",  "Mistress of Necromancy"},
      { "Master of Alchemy",  "Mistress of Alchemy"},

      { "Master of Alchemy",  "Mistress of Alchemy"},
      { "Spellbinder",  "Spellbinder"},
      { "Spellbinder",  "Spellbinder"},
      { "Master of Magic",  "Mistress of Magic"},
      { "Master of Magic",  "Mistress of Magic"},

      { "Master of the Past",  "Mistress of the Past"},
      { "Master of the Past",  "Mistress of the Past"},
      { "Master of the Present",  "Mistress of the Present"},
      { "Master of the Present",  "Mistress of the Present"},
      { "Master of the Future",  "Mistress of the Future"},

      { "Master of the Future",  "Mistress of the Future"},
      { "Arch Mage",  "Arch Mage"},
      { "Arch Mage",  "Arch Mage"},
      { "Mystical",  "Mystical"},
      { "Mystical",  "Mystical"},
      
	{ "Mage Hero",			"Mage Heroine"			},
	{ "Avatar of Magic",		"Avatar of Magic"		},
	{ "Angel of Magic",		"Angel of Magic"		},
	{ "Demigod of Magic",		"Demigoddess of Magic"		},
	{ "Immortal of Magic",		"Immortal of Magic"		},
	{ "God of Magic",		"Goddess of Magic"		},
	{ "Deity of Magic",		"Deity of Magic"		},
	{ "Supremity of Magic",		"Supremity of Magic"		},
	{ "Creator",			"Creator"			},
	{ "Implementor",		"Implementress"			}
    },

    {
      { "Man",			"Woman"				},

      { "Apprentice of Magic",	"Apprentice of Magic"		},
      { "Spell Student",		"Spell Student"			},
      { "Scholar of Magic",		"Scholar of Magic"		},
      { "Delver in Spells",		"Delveress in Spells"		},
      { "Medium of Magic",		"Medium of Magic"		},
      
      { "Scribe of Magic",		"Scribess of Magic"		},
      { "Seer",			"Seeress"			},
      { "Sage",			"Sage"				},
      { "Illusionist",		"Illusionist"			},
      { "Abjurer",			"Abjuress"			},
      
      { "Invoker",			"Invoker"			},
      { "Invoker",			"Invoker"			},
      { "Enchanter",			"Enchantress"			},
      { "Enchanter",			"Enchantress"			},
      { "Conjurer",			"Conjuress"			},

      { "Conjurer",			"Conjuress"			},
      { "Magician",			"Witch"				},
      { "Magician",			"Witch"				},
      { "Creator",			"Creator"			},
      { "Creator",			"Creator"			},
      
      { "Savant",			"Savant"			},
      { "Savant",			"Savant"			},
      { "Magus",			"Craftess"			},
      { "Magus",			"Craftess"			},
      { "Wizard",			"Wizard"			},

      { "Wizard",			"Wizard"			},
      { "Warlock",			"War Witch"			},
      { "Warlock",			"War Witch"			},
      { "Sorcerer",			"Sorceress"			},
      { "Sorcerer",			"Sorceress"			},
      
      { "Elder Sorcerer",		"Elder Sorceress"		},
      { "Elder Sorcerer",		"Elder Sorceress"		},
      { "Grand Sorcerer",		"Grand Sorceress"		},
      { "Grand Sorcerer",		"Grand Sorceress"		},
      { "Great Sorcerer",		"Great Sorceress"		},

      { "Great Sorcerer",		"Great Sorceress"		},
      { "Golem Maker",		"Golem Maker"			},
      { "Golem Maker",		"Golem Maker"			},
      { "Greater Golem Maker",	"Greater Golem Maker"		},
      { "Greater Golem Maker",	"Greater Golem Maker"		},
      
	{ "Maker of Stones",		"Maker of Stones",		},
	{ "Maker of Stones",		"Maker of Stones",		},
	{ "Maker of Potions",		"Maker of Potions",		},
	{ "Maker of Potions",		"Maker of Potions",		},
	{ "Maker of Scrolls",		"Maker of Scrolls",		},

	{ "Maker of Scrolls",		"Maker of Scrolls",		},
	{ "Maker of Wands",		"Maker of Wands",		},
	{ "Maker of Wands",		"Maker of Wands",		},
	{ "Maker of Staves",		"Maker of Staves",		},
	{ "Maker of Staves",		"Maker of Staves",		},

	{ "Demon Summoner",		"Demon Summoner"		},
	{ "Demon Summoner",		"Demon Summoner"		},
	{ "Greater Demon Summoner",	"Greater Demon Summoner"	},
	{ "Greater Demon Summoner",	"Greater Demon Summoner"	},
	{ "Dragon Charmer",		"Dragon Charmer"		},

	{ "Dragon Charmer",		"Dragon Charmer"		},
	{ "Greater Dragon Charmer",	"Greater Dragon Charmer"	},
	{ "Greater Dragon Charmer",	"Greater Dragon Charmer"	},
      { "Master of Sorcery",  "Mistress of Sorcery"},
      { "Master of Sorcery",  "Mistress of Sorcery"},

      { "Master of Wizardry",  "Mistress of Wizardry"},
      { "Master of Wizardry",  "Mistress of Wizardry"},
      { "Master Enchanter",  "Master Enchanter"},
      { "Master Enchanter",  "Master Enchanter"},
      { "Master Summoner",  "Master Summoner"},

      { "Master Summoner",  "Master Summoner"},
      { "Master of Divination",  "Mistress of Divination"},
      { "Master of Divination",  "Mistress of Divination"},
      { "Master of Alteration",  "Mistress of Alteration"},
      { "Master of Alteration",  "Mistress of Alteration"},

      { "Master of Pyrotechniques",  "Mistress of Pyrotechniques"},
      { "Master of Pyrotechniques",  "Mistress of Pyrotechniques"},
      { "Master of Necromancy",  "Mistress of Necromancy"},
      { "Master of Necromancy",  "Mistress of Necromancy"},
      { "Master of Alchemy",  "Mistress of Alchemy"},

      { "Master of Alchemy",  "Mistress of Alchemy"},
      { "Spellbinder",  "Spellbinder"},
      { "Spellbinder",  "Spellbinder"},
      { "Master of Magic",  "Mistress of Magic"},
      { "Master of Magic",  "Mistress of Magic"},

      { "Master of the Past",  "Mistress of the Past"},
      { "Master of the Past",  "Mistress of the Past"},
      { "Master of the Present",  "Mistress of the Present"},
      { "Master of the Present",  "Mistress of the Present"},
      { "Master of the Future",  "Mistress of the Future"},

      { "Master of the Future",  "Mistress of the Future"},
      { "Arch Mage",  "Arch Mage"},
      { "Arch Mage",  "Arch Mage"},
      { "Mystical",  "Mystical"},
      { "Mystical",  "Mystical"},
      
	{ "Mage Hero",			"Mage Heroine"			},
	{ "Avatar of Magic",		"Avatar of Magic"		},
	{ "Angel of Magic",		"Angel of Magic"		},
	{ "Demigod of Magic",		"Demigoddess of Magic"		},
	{ "Immortal of Magic",		"Immortal of Magic"		},
	{ "God of Magic",		"Goddess of Magic"		},
	{ "Deity of Magic",		"Deity of Magic"		},
	{ "Supremity of Magic",		"Supremity of Magic"		},
	{ "Creator",			"Creator"			},
	{ "Implementor",		"Implementress"			}
    }
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
  { "Atum-Ra", "Lawful Good", OBJ_VNUM_TATTOO_APOLLON },
  { "Zeus", "Neutral Good", OBJ_VNUM_TATTOO_ZEUS },
  { "Siebele", "True Neutral", OBJ_VNUM_TATTOO_SIEBELE },
  { "Shamash", "God of Justice", OBJ_VNUM_TATTOO_HEPHAESTUS },
  { "Ahuramazda", "Chaotic Good", OBJ_VNUM_TATTOO_EHRUMEN },
  { "Ehrumen", "Chaotic Evil", OBJ_VNUM_TATTOO_AHRUMAZDA },
  { "Deimos", "Lawful Evil", OBJ_VNUM_TATTOO_DEIMOS },
  { "Phobos", "Neutral Evil", OBJ_VNUM_TATTOO_PHOBOS },
  { "Odin", "Lawful Neutral", OBJ_VNUM_TATTOO_ODIN },
  { "Teshub", "Chaotic Neutral", OBJ_VNUM_TATTOO_MARS },
  { "Ares", "God of War", OBJ_VNUM_TATTOO_ATHENA },
  { "Goktengri", "God of Honor", OBJ_VNUM_TATTOO_GOKTENGRI },
  { "Hera", "God of Hate", OBJ_VNUM_TATTOO_HERA },
  { "Venus", "God of beauty", OBJ_VNUM_TATTOO_VENUS },
  { "Seth", "God of Anger", OBJ_VNUM_TATTOO_ARES },
  { "Enki", "God of Knowledge", OBJ_VNUM_TATTOO_PROMETHEUS },
  { "Eros", "God of Love", OBJ_VNUM_TATTOO_EROS }
};

const struct ethos_type ethos_table [] =
{
  { "Null" },
  { "Lawful" },
  { "Neutral" },
  { "Chaotic" }
};

/*  altar good neut evil, recall good neut evil, pit good neut evil */
const struct hometown_type hometown_table [] = 
{
  { "Midgaard", {3070, 3054, 3072}, {3068, 3001, 3071}, {3069,3054,3072} },
  { "New Thalos", {9605, 9605, 9605}, {9609,9609,9609}, {9609,9609,9609} },
  { "Titans", {18127,18127,18127},{18126,18126,18126},{18127,18127,18127} },
  { "New Ofcol", {669, 669, 669},      {698, 698, 698},   {669, 669, 669} },
  { "Old Midgaard",{5386, 5386,5386}, {5379,5379, 5379}, {5386,5386,5386} }, 
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


/*
 * Liquid properties.
 * Used in world.obj.
 */
const	struct	liq_type	liq_table	[]	=
{
/*    name			color	proof, full, thirst, food, ssize */
    { "water",			"clear",	{   0, 1, 10, 0, 16 }	},
    { "beer",			"amber",	{  12, 1,  8, 1, 12 }	},
    { "red wine",		"burgundy",	{  30, 1,  8, 1,  5 }	},
    { "ale",			"brown",	{  15, 1,  8, 1, 12 }	},
    { "dark ale",		"dark",		{  16, 1,  8, 1, 12 }	},

    { "whisky",			"golden",	{ 120, 1,  5, 0,  2 }	},
    { "lemonade",		"pink",		{   0, 1,  9, 2, 12 }	},
    { "firebreather",		"boiling",	{ 190, 0,  4, 0,  2 }	},
    { "local specialty",	"clear",	{ 151, 1,  3, 0,  2 }	},
    { "slime mold juice",	"green",	{   0, 2, -8, 1,  2 }	},

    { "milk",			"white",	{   0, 2,  9, 3, 12 }	},
    { "tea",			"tan",		{   0, 1,  8, 0,  6 }	},
    { "coffee",			"black",	{   0, 1,  8, 0,  6 }	},
    { "blood",			"red",		{   0, 2, -1, 2,  6 }	},
    { "salt water",		"clear",	{   0, 1, -2, 0,  1 }	},

    { "coke",			"brown",	{   0, 2,  9, 2, 12 }	}, 
    { "root beer",		"brown",	{   0, 2,  9, 2, 12 }   },
    { "elvish wine",		"green",	{  35, 2,  8, 1,  5 }   },
    { "white wine",		"golden",	{  28, 1,  8, 1,  5 }   },
    { "champagne",		"golden",	{  32, 1,  8, 1,  5 }   },

    { "mead",			"honey-colored",{  34, 2,  8, 2, 12 }   },
    { "rose wine",		"pink",		{  26, 1,  8, 1,  5 }	},
    { "benedictine wine",	"burgundy",	{  40, 1,  8, 1,  5 }   },
    { "vodka",			"clear",	{ 130, 1,  5, 0,  2 }   },
    { "cranberry juice",	"red",		{   0, 1,  9, 2, 12 }	},

    { "orange juice",		"orange",	{   0, 2,  9, 3, 12 }   }, 
    { "absinthe",		"green",	{ 200, 1,  4, 0,  2 }	},
    { "brandy",			"golden",	{  80, 1,  5, 0,  4 }	},
    { "aquavit",		"clear",	{ 140, 1,  5, 0,  2 }	},
    { "schnapps",		"clear",	{  90, 1,  5, 0,  2 }   },

    { "icewine",		"purple",	{  50, 2,  6, 1,  5 }	},
    { "amontillado",		"burgundy",	{  35, 2,  8, 1,  5 }	},
    { "sherry",			"red",		{  38, 2,  7, 1,  5 }   },	
    { "framboise",		"red",		{  50, 1,  7, 1,  5 }   },
    { "rum",			"amber",	{ 151, 1,  4, 0,  2 }	},

    { "cordial",		"clear",	{ 100, 1,  5, 0,  2 }   },
    { NULL,			NULL,		{   0, 0,  0, 0,  0 }	}
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
	"reserved",	{100,100,100,100, 100,100,100,100,100 ,100,100,100,100},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,			TAR_IGNORE,		POS_STANDING,
	&gsn_reserved,			SLOT( 0),	 0,	 0,
	"",			"",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"absorb",	{ 67, 93, 93, 93, 93, 93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_absorb,		TAR_CHAR_SELF,		POS_STANDING,
	&gsn_absorb,		SLOT(707),	100,	12,
	"",		"The energy field around you fades!",
	"$p's energy field fades.",
 	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"acetum primus",	{ 34,93,93,93, 93,93,93,93,93,93,93,93,93},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_acetum_primus,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(654),	20,	12,
	"acetum primus",	"!acetum primus!", "", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	"acid arrow",		{ 48, 93, 93, 93, 93, 93,93,93,48,93,93,48,48},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_acid_arrow,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(644),	20,	12,
	"acid arrow",		"!Acid Arrow!", "", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	"acid blast",		{ 63, 93, 93, 93, 93,93,93,93,63,93,93,63,63},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_acid_blast,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_acid_blast,	SLOT(70),	40,	12,
	"acid blast",		"!Acid Blast!", "", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	"acute vision",         {  93,93,93,93, 93, 93, 93, 7 ,93,93,93,93,93},
        { 3,  2,  1,  1, 1, 1, 2, 1, 1, 1,1,1,1},
        spell_acute_vision,        TAR_CHAR_SELF,          POS_STANDING,
        NULL,                    SLOT(514),        10,       0,
	"",         "Your vision seems duller.", "",	CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"adamantite golem",{93,93,93,93,93,93,93,93,93,93,93,71,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_adamantite_golem,	TAR_IGNORE,	POS_STANDING,
	NULL,		SLOT(665),	500,		30,
	"",	"You gained enough mana to make more golems now.","",
	CABAL_NONE , 	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"aid",		{ 93, 53,93, 93,93,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_aid,        TAR_CHAR_DEFENSIVE,          POS_FIGHTING,
	NULL,		SLOT(680),	100,		12,
	"",	"You can aid more people.",	"", 
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_HEALING
    },

    {
	"amnesia",         {  93, 93, 93,93,93,93,93,93,93,93,93,93,93 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_amnesia,      TAR_CHAR_OFFENSIVE,       POS_STANDING,
	NULL,       SLOT(538),        100,       12,
	"", "!amnesia!", "",	CABAL_NONE , RACE_NONE,ALIGN_NONE, GROUP_NONE
    },

    {
	"animate dead",		{ 93, 93, 93, 93,93,93,93,93,93,93,42,38,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_animate_dead,	TAR_OBJ_CHAR_OFF,	POS_STANDING,
	NULL,		SLOT(581),	50,	12,
	"",	"You gain energy to animate new deads.", "", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"animate object",	{ 70, 93, 93, 93, 93, 93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_animate_object,	TAR_OBJ_CHAR_OFF,	POS_STANDING,
	NULL,		SLOT(709),	50,	12,
	"",		"You gain energy to animate new objects.", "",
 	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"armor",		{  13,1,93,93, 12,93,13,93 , 93, 10,13,13,13},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_armor,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT( 1),	 5,	12,
	"",			"You feel less armored.",	"", CABAL_NONE ,
	RACE_NONE, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	"assist",      {  93, 93, 93, 93,93,93,93,93,93,93,93,43,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_assist,        TAR_CHAR_DEFENSIVE,          POS_FIGHTING,
        NULL,             SLOT(670),        100,       12,
	"", "You can assist more, now.", "",	CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_HEALING
    },

    {
	"astral walk",		{31,93,93,93,93,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_astral_walk,	TAR_IGNORE,	POS_FIGHTING,
	NULL,			SLOT(622),	80,	12,
	"",			"!Astral Walk!",	"",CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_TRANSPORTATION
    },

    {
	"attract other",	{ 93, 93, 93, 93,93,93,93,93,93,93,30,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_attract_other,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
	NULL,		SLOT(580),	5,	12,
	"",	"You feel your master leaves you.",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_BEGUILING
    },

    {
	"bark skin",         {  93, 93, 93, 93, 93, 93, 93, 22 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_bark_skin,        TAR_CHAR_SELF,          POS_STANDING,
        NULL,             SLOT(515),        40,       0,
	"", "The bark on your skin flakes off.", "",	CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"black death",    { 93, 93, 93, 93,93, 64, 93, 93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_black_death,	TAR_IGNORE,	POS_STANDING,
	&gsn_black_death,	SLOT(677),	200,	24,
	"",		"!black death!",	"", 
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_MALADICTIONS
    },

    {
	"blade barrier",	{93,60,93,93,93,93,93,93,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_blade_barrier,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(679),	40,	12,
	"blade barrier",	"!Blade Barrier!",	"",CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_ATTACK
    },

    {
	"bless",		{ 93,  14, 93, 93,14,93,93,93,93,12,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_bless,		TAR_OBJ_CHAR_DEF,	POS_STANDING,
	&gsn_bless,		SLOT( 3),	 5,	12,
	"",			"You feel less righteous.", 
	"$p's holy aura fades.", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_BENEDICTIONS
    },

    {
	"bless weapon",	{  93, 93, 93, 93,55,93,93,93,93,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_bless_weapon,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(637),	100,	24,
	"",			"!Bless Weapon!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_ENCHANTMENT
    },

    {
	"blindness",	{  93,  14, 93, 93,93,14,16,93, 20,93,20,19,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_blindness,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_blindness,		SLOT( 4),	 5,	12,
	"",			"You can see again.",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_MALADICTIONS
    },

    {
	"bluefire",		{  93,37,93,93,93,93,93,93,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_bluefire,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_bluefire,			SLOT(660),	20,	12,
	"torments",		"!Bluefire!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_ATTACK
    },

    {
	"burning hands",	{  14,93,93,93, 93, 93, 93, 93 ,14,93,14,14,14},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_burning_hands,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_burning_hands,	SLOT( 5),	15,	12,
	"burning hands",	"!Burning Hands!", 	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	"call lightning",	{ 93, 44, 93, 93, 41,93,50, 93 , 93,93,93,93,33},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_call_lightning,	TAR_IGNORE,		POS_FIGHTING,
	NULL,			SLOT( 6),	15,	12,
	"lightning bolt",	"!Call Lightning!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_WEATHER
    },

    {   "calm",			{ 93, 26, 93, 93, 63, 93, 93, 93 ,93,60,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_calm,		TAR_IGNORE,		POS_FIGHTING,
	NULL,			SLOT(509),	30,	12,
	"",			"You have lost your peace of mind.",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_BENEDICTIONS
    },

    {
	"cancellation",		{ 24, 32, 93, 93,93,93,93,93 , 28,93,93,28,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_cancellation,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(507),	20,	12,
	"",			"!cancellation!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	"cause critical",	{  93,  15, 93, 93,93,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1,1, 1,1,1,1},
	spell_cause_critical,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(63),	20,	12,
	"spell",		"!Cause Critical!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_HARMFUL
    },

    {
	"cause light",		{ 93,  2, 93, 93,93,93,93,93,93 ,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_cause_light,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(62),	15,	12,
	"spell",		"!Cause Light!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_HARMFUL
    },

    {
	"cause serious",	{  93,  8, 93, 93,93,93,93,93, 93 ,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_cause_serious,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(64),	17,	12,
	"spell",		"!Cause Serious!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_HARMFUL
    },

    {
	"caustic font",	{ 93, 93, 93, 93, 93, 93, 93,93,41,93,93,93,93},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_caustic_font,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(653),	20,	12,
	"caustic font",	"!caustic font!", "", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {   
	"chain lightning",	{ 33, 93, 93, 93,93,93,93,93, 33 ,93,33,33,33},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_chain_lightning,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(500),	25,	12,
	"lightning",		"!Chain Lightning!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    }, 

    {
	"charm person",		{ 93, 93, 93, 93,93,38,93,93, 93,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_charm_person,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_charm_person,	SLOT( 7),	 5,	12,
	"",		"You feel more self-confident.",	"", 
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_BEGUILING
    },

    {
	"chromatic orb",	{ 93, 93, 93, 93, 93,93,93,93,63,93,93,93,93},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_chromatic_orb,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(714),	50,	12,
	"chromatic orb",		"!Chromatic Orb!", "", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	"control undead",      {  93, 93,93,93,93,93,93,93,93,93,93,57,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_control_undead,   TAR_CHAR_OFFENSIVE,          POS_STANDING,
        NULL,             SLOT(669),        20,       12,
	"", "You feel more self confident.", "",	CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"chill touch",		{  5, 93, 93, 93,93,6,93,93 ,5,93,93,7,8 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_chill_touch,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_chill_touch,		SLOT( 8),	15,	12,
	"chilling touch",	"You feel less cold.",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	"colour spray",		{  22, 93, 93, 93,93,93,93,93 , 22,93,25,93,22},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_colour_spray,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(10),	15,	12,
	"colour spray",		"!Colour Spray!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	"continual light",	{  11,  5, 93, 93,93,93,93,93 , 11,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_continual_light,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(57),	 7,	12,
	"",			"!Continual Light!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_CREATION
    },

    {
	"control weather",	{ 93,21,93,93, 93,93,11,33, 30,93,24,24,20},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_control_weather,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(11),	25,	12,
	"",			"!Control Weather!",	"", CABAL_NONE , 	
	RACE_NONE, ALIGN_NONE, GROUP_WEATHER
    },

    {
	"corruption",      {  93, 93, 93, 93, 93,93,93,93,93,93,93,63,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_corruption,    TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
        NULL,             SLOT(671),        20,       12,
	"corruption", "You feel yourself healthy again.", "",CABAL_NONE, 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"create food",		{ 15, 8, 93, 93,3,93,93,93 , 93,93,93,12,13},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_create_food,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(12),	 5,	12,
	"",			"!Create Food!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_CREATION
    },

    {
	"create rose",		{ 26, 93, 93, 93, 20, 93, 93, 93,93,93,26,26,26},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_create_rose,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(511),	30, 	12,
	"",			"!Create Rose!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_CREATION
    },  

    {
	"create spring",	{ 24, 27, 93, 93,93,93,93,93 ,93,93,93,31,31},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_create_spring,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(80),	20,	12,
	"",			"!Create Spring!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_CREATION
    },

    {
	"create water",		{ 8,  3, 93, 93,4,93,93,93 ,93,93,93,11,8},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_create_water,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(13),	 5,	12,
	"",			"!Create Water!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_CREATION
    },

    {
	"cure blindness",	{ 93,11,93,93, 7,93,9,13, 93,20,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_cure_blindness,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(14),	 5,	12,
	"",			"!Cure Blindness!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_CURATIVE
    },

    {
	"cure critical",	{93,  18, 93, 93,18,93,25,93,93,25,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_cure_critical,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	&gsn_cure_critical,	SLOT(15),	20,	12,
	"",			"!Cure Critical!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_HEALING
    },

    {
	"cure disease",		{  93, 19, 93, 93,33,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_cure_disease,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(501),	20,	12,
	"",			"!Cure Disease!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_CURATIVE
    },

    {
	"cure light",		{ 93,  4, 93, 93,5,11,9,93,93,10,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_cure_light,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	&gsn_cure_light,	SLOT(16),	10,	12,
	"",			"!Cure Light!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_HEALING
    },

    {
	"cure poison",		{ 93,  23, 93,93,33,93,93,93,93,35,93,93,23 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_cure_poison,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_cure_poison,		SLOT(43),	 5,	12,
	"",			"!Cure Poison!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_CURATIVE
    },

    {
	"cure serious",		{ 93,  10, 93, 93,10,93,18,93,93,18,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_cure_serious,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	&gsn_cure_serious,	SLOT(61),	15,	12,
	"",			"!Cure Serious!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_HEALING
    },

    {
	"curse",		{ 93, 33, 93,93,93,18,93,93,31,93,44,34,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_curse,		TAR_OBJ_CHAR_OFF,	POS_FIGHTING,
	&gsn_curse,		SLOT(17),	20,	12,
	"curse",		"The curse wears off.", 
	"$p is no longer impure.", CABAL_NONE , RACE_NONE, 
	ALIGN_NONE, GROUP_MALADICTIONS
    },

    {
	"cursed lands",    { 93, 41, 93, 93,93, 93, 93, 93 ,93,93,93,64,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_cursed_lands,	TAR_IGNORE,	POS_STANDING,
	&gsn_cursed_lands,	SLOT(675),	200,	24,
	"",		"!cursed lands!",	"", 
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_MALADICTIONS
    },

    {
	"deadly venom",    { 93, 93, 93, 93,93, 58, 93, 93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_deadly_venom,	TAR_IGNORE,	POS_STANDING,
	&gsn_deadly_venom,		SLOT(674),	200,	24,
	"",		"!deadly venom!",	"", 
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_MALADICTIONS
    },

    {
	"deafen",         {  93,93,93,93,93,42,93,93 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_deafen,      TAR_CHAR_OFFENSIVE,   POS_FIGHTING,
        &gsn_deafen,       SLOT(570),        40,       12,
	"deafen", "The ringing in your ears finally stops.", "", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"demonfire",		{  93, 37, 93, 93,93,93,93,93,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_demonfire,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_demonfire,			SLOT(505),	20,	12,
	"torments",		"!Demonfire!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_ATTACK
    },	

    {
	"desert fist",		{ 93, 58, 93,93,93,93,93,93,93,93,93,93,55 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_desert_fist, 	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(681),	50,	12,
	"desert fist",	"!desert fist!", "", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE,GROUP_NONE
    },

    {
	"detect evil",		{ 12,  11, 93, 93,3,93,93,93 ,12,93,93,93,13},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_detect_evil,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(18),	 5,	12,
	"",			"The red in your vision disappears.",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_DETECTION
    },

    {
	"detect good",          { 12,  11, 93, 93,3,93,93,93 ,12,93,13,13,13},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_detect_good,      TAR_CHAR_SELF,          POS_STANDING,
        NULL,                   SLOT(513),        5,     12,
	"",                     "The gold in your vision disappears.",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_DETECTION
    },

    {
	"detect hide",	{ 93, 93, 10, 93, 93, 93, 10, 93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_CHAR_SELF,		POS_STANDING,
	&gsn_detect_hidden,	SLOT(0),	 5,	12,
	"",			"You feel less aware of your surroundings.",	
	"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_DETECTION
    },

    {
	"detect invis",		{  5,  17, 93, 93,23,13,93,93 ,10,93,23,13,8},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_detect_invis,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(19),	 5,	12,
	"",			"You no longer see invisible objects.",
	"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_DETECTION
    },

    {
	"detect magic",		{  8,  12, 93, 93,93,93,93,93,6,30,93,15,12},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_detect_magic,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(20),	 5,	12,
	"",			"The detect magic wears off.",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_DETECTION
    },

    {
	"detect poison",	{ 35,  39, 93, 93,93,93,93,93,35 ,39,31,21,7},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_detect_poison,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(21),	 5,	12,
	"",			"!Detect Poison!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_DETECTION
    },

    {
	"detect undead",	{ 93, 93,93, 93,93,93,93,93 ,93,93,93,13,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_detect_undead,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,		SLOT(594),	5,	12,
	"",		"You can't detect undeads anymore.", "", 
	CABAL_NONE, 	RACE_NONE, ALIGN_NONE, GROUP_DETECTION
    },

    {
	"disenchant armor",	{ 27, 93, 93, 93, 93, 93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_disenchant_armor,		TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(705),	50,	24,
	"",		"!disenchant armor!",	"", 	CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"disenchant weapon",	{ 34, 93, 93, 93, 93, 93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_disenchant_weapon,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(706),	50,	24,
	"",		"!disenchant weapon!",	"", 	CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"disintegrate",         {  75,93,93,93,93,93,93,93,93,93 ,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_disintegrate,      TAR_CHAR_OFFENSIVE,   POS_STANDING,
        NULL,                    SLOT(574),    100,       18,
	"thin light ray", "!disintegrate!", "",	CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"dismantle",	{ 93, 93,93, 93,93,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,		SLOT(621),	200,	24,
	"",	"!621!",	"", 
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"dispel evil",		{ 93, 27, 93, 93,35,93,93,93,55 ,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_dispel_evil,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(22),	15,	12,
	"dispel evil",		"!Dispel Evil!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_ATTACK
    },

    {
	"dispel good",          {   93,  27,  93,  93,93,30,93,93,57,93,32,32,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_dispel_good,      TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,                   SLOT(512),      15,     12,
	"dispel good",          "!Dispel Good!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_ATTACK
    },

    {
	"dispel magic",		{ 36, 43, 93, 93,93,93,93,93 ,41,93,24,24,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_dispel_magic,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(59),	15,	12,
	"",			"!Dispel Magic!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	"disruption",	{ 93, 93, 93, 93, 93, 93, 93,93,93,93,93,40,93},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_disruption,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(648),	20,	12,
	"disruption",	"!disruption!", "", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	"dragon breath", {93,93,93,93,93,93,93,93,93,93,93,93,93},
	{1,1,1,1,1,1,1,1,1, 1,1,1,1},
        spell_dragon_breath,  TAR_CHAR_OFFENSIVE, POS_FIGHTING,
        &gsn_dragon_breath,  SLOT(563),   75,   12,
	"blast of fire", "!dragon breath!", "",	CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"dragon skin",	{ 10, 10,10, 10,10,10,10,10 ,10,10,10,10,10},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_dragon_skin,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(612),	50,	24,
	"",	"Your skin becomes softer.",	"", 
	CABAL_NONE , RACE_TOP, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	"dragon strength", {93,93,93,93,93,93,93,93,93,93,93,93,93},
	{1,1,1,1,1,1,1,1, 1, 1,1,1,1},
        spell_dragon_strength, TAR_CHAR_SELF, POS_FIGHTING,
        &gsn_dragon_strength, SLOT(562),    75,    12,
	"", "You feel the strength of the dragon leave you.", "",	CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"dragons breath",	{ 61, 93, 93, 93, 93, 93, 93, 93 ,61,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_dragons_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(576),	200,	24,
	"dragon breath",	"Your get healtier again.",
	"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_DRACONIAN
    },

    {
	"drain",		{ 31, 93, 93, 93, 93, 93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_drain,		TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(704),	5,	12,
	"",		"!drain!",	"", 	CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_MALADICTIONS
    },

    {
	"earthfade",		{ 93, 93, 93, 93, 93, 93,93,93,93,93,93,93,73},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_earthfade,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	&gsn_earthfade,			SLOT(702),	100,	12,
	"",		"You slowly fade to your neutral form.",	"",
 	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"earthmaw",		{ 93, 93, 93, 93, 93, 93,93,93,93,93,93,93,67},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_earthmaw,		TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(703),	30,	12,
	"earthmaw",		"!earthmaw!",	"", 	CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"earthquake",		{ 93,  19, 93, 93,93,93,93,93,93,93,93,93,19 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_earthquake,	TAR_IGNORE,		POS_FIGHTING,
	NULL,			SLOT(23),	15,	12,
	"earthquake",		"!Earthquake!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_ATTACK
    },

    {
	"elemental sphere",	{ 93, 93,93,93, 93,93,93,93 ,93,93,93,93,29},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_elemental_sphere,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(719),	75,	12,
	"",	"The protecting elemental sphere around you fades.",	"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"enchant armor",	{  26, 93, 93, 93,93,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_enchant_armor,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(510),	100,	24,
	"",			"!Enchant Armor!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_ENCHANTMENT
    },

    {
	"enchant weapon",	{  33, 93, 93, 93,93,93,93,93,17,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_enchant_weapon,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(24),	100,	24,
	"",			"!Enchant Weapon!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_ENCHANTMENT
    },

    {
	"energy drain",		{ 38,42,93,93,93,26,93,93,93,93,93,45,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_energy_drain,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(25),	35,	12,
	"energy drain",		"!Energy Drain!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_MALADICTIONS
    },

    {
	"enhanced armor",	{ 48, 33, 93,93,93,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
	spell_enhanced_armor, 	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	&gsn_enhanced_armor,	SLOT(583),	20,	12,
	"",	"You feel yourself unprotected.", "", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	"enlarge",		{  93,93,93,93, 93,93,93,93, 38,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_enlarge,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(711),	 50,	12,
	"",	"You return to your orginal size.",	"", CABAL_NONE,
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"etheral fist",		{ 93,93,93,93, 93, 50,93,93,93,93,93,93,93},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_etheral_fist,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(645),	20,	12,
	"etheral fist",		"!Etheral Fist!", "", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	"faerie fire",		{ 11,  6, 93, 93,93,93,8,93 ,15 ,93,93,93,11},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_faerie_fire,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_faerie_fire,		SLOT(72),	 5,	12,
	"faerie fire",		"The pink aura around you fades away.",
	"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_WEATHER
    },

    {
	"faerie fog",		{ 24, 31, 93, 93,93,93,93,93 ,27,93,93,93,24},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_faerie_fog,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(73),	12,	12,
	"faerie fog",		"!Faerie Fog!",		"", CABAL_NONE , 
	RACE_NONE,ALIGN_NONE, GROUP_WEATHER
    },

    {
	"farsight",		{  22, 93, 93, 93,93,93,93,93,22,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_farsight,		TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(521),	20,	12,
	"farsight",		"!Farsight!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_DETECTION
    },	

    {
	"fear",		{   93, 93, 93, 93,93,93,93,93,93 ,93,93,73,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_fear,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_fear,	SLOT(598),	50,	12,
	"",		"You feel more brave.",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_ILLUSION
    },

    {
	"fire and ice",		{ 93, 93, 93, 93, 93, 93,93,93,93,93,93,93,65},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_fire_and_ice,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(699),	40,	12,
	"fire and ice",		"!fire and ice!",	"", 	CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	"fireball",		{55,93,93,93,93,56,93,93,56,93,93,93,55},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_fireball,		TAR_IGNORE,	POS_FIGHTING,
	NULL,			SLOT(26),	25,	12,
	"fireball",		"!Fireball!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },
  
    {
	"fireproof",		{ 25, 16, 93, 93, 20, 93,93,93,21,93,93,42,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_fireproof,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(523),	10,	12,
	"",		"",	"$p's protective aura fades.", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_ENCHANTMENT
    },

    {
	"firestream",		{ 93, 93, 93, 93, 93, 93,93,93,93,93,93,93,35},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_firestream,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(692),	20,	12,
	"",		"",	"", 	CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	"fire shield",		{   93, 93, 93, 93,93,93,93,93,93 ,93,93,93,68},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_fire_shield,	TAR_IGNORE,	POS_STANDING,
	&gsn_fire_shield,		SLOT(601),	200,	24,
	"",			"!fire shield!",
	"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"flamestrike",		{93,42,93,93,32,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_flamestrike,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(65),	20,	12,
	"flamestrike",		"!Flamestrike!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_ATTACK	
    },

    {
	"fly",			{ 19, 30, 93,93,93,93,93,93,19,93,43,23,21},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_fly,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_fly,			SLOT(56),	10,	18,
	"",		"You slowly float to the ground.",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_TRANSPORTATION
    },

    {
	"floating disc",	{ 4, 10, 93, 93, 93, 93, 93, 93 ,4,93,93,93,4},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_floating_disc,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(522),	40,	24,
	"",			"!Floating disc!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"forcecage",		{ 29, 93,93,93, 93,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_forcecage,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(717),	75,	12,
	"",	"The forcecage around you fades.",	"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"frenzy",               {  93, 34, 93, 93,93,93,93,93,93,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_frenzy,           TAR_CHAR_DEFENSIVE,     POS_STANDING,
        NULL,                   SLOT(504),      30,     24,
	"",                     "Your rage ebbs.",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_BENEDICTIONS
    },

    {
	"frostbolt",		{ 93, 93, 93, 93, 93, 93,93,93,93,93,93,93,42},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_frostbolt,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(695),	20,	12,
	"frostbolt",		"!frostbolt!",	"", 	CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	"fumble",		{ 93,93,93,93, 93,93,93,93 ,36,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_fumble,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(712), 	25,	18,
	"",		"You speed up and regain your strength!",	"", 
	CABAL_NONE, RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"galvanic whip",	{ 28, 93, 93, 93, 93, 93,93,93,93,93,93,93,93},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_galvanic_whip,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(655),	20,	12,
	"galvanic whip",	"!galvanic whip!", "", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	"gate",			{93,63,93,93,93,93,93,93,27,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_gate,		TAR_IGNORE,		POS_FIGHTING,
	NULL,			SLOT(83),	80,	12,
	"",			"!Gate!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_TRANSPORTATION
    },

    {
	"giant strength",	{  20, 39, 93, 93,93,93,93,93, 30,93,93,28,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_giant_strength,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_giant_strength,		SLOT(39),	20,	12,
	"",			"You feel weaker.",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_ENHANCEMENT
    },

    {
	"grounding",		{ 93, 93, 93, 93, 93, 93,93,93,93,93,93,93,53},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_grounding,	TAR_CHAR_SELF,		POS_FIGHTING,
	NULL,			SLOT(700),	50,	12,
	"",		"You lost your grounding against electricity",	"",
 	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"group defense",	{   93, 35, 93,93,93,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_group_defense,		TAR_IGNORE,		POS_STANDING,
	NULL,		SLOT(586),	100,	36,
	"",			"You feel less protected.",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_BENEDICTIONS
    },

    {
	"group heal",		{  93,65,93,93,75,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_group_healing,	TAR_CHAR_DEFENSIVE, POS_FIGHTING,
	NULL,			SLOT(642),	500,	24,
	"",			"!Group Heal!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_HEALING
    },
  
    {
	"hallucination",	{   93, 93, 93, 93,93,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_hallucination,	TAR_CHAR_SELF,	POS_STANDING,
	NULL,		SLOT(606),	200,	12,
	"",			"You are again defenseless to magic.",
	"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_ILLUSION
    },

    {
	"hand of undead",	{ 93, 93, 93,93,93,93,93,93,93,93,93,44,93},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_hand_of_undead,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(647),	20,	24,
	"hand of undead",	"!hand of undead!", "", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	"harm",			{  93, 26, 93, 93,93,29,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_harm,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(27),	35,	12,
	"harm spell",		"!Harm!",	"", 	CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_HARMFUL
    },
  
    {
	"haste",		{ 24, 93, 93,93,93,93,93,93,24,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_haste,		TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	&gsn_haste,		SLOT(502),	30,	12,
	"",			"You feel yourself slow down.",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_ENHANCEMENT
    },

    {
	"heal",			{  93, 28, 93, 93,29,93,93,93 ,93 ,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_heal,		TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(28),	50,	12,
	"",			"!Heal!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_HEALING
    },
  
    {
	"healing light",    { 93, 41, 93, 93,93, 93, 93, 93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_healing_light,	TAR_IGNORE,	POS_STANDING,
	NULL,			SLOT(613),	200,	24,
	"",			"You can light more rooms now.",	"", 
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_BENEDICTIONS
    },

    {
	"heat metal",		{ 93,26,93,93, 93,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_heat_metal,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(516), 	25,	18,
	"spell",		"!Heat Metal!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"helical flow",		{93,93,93,93,93,93,93,93,93,93,93,93,43},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_helical_flow,		TAR_IGNORE,	POS_FIGHTING,
	NULL,			SLOT(661),	80,	12,
	"",			"!Helical Flow!",	"",CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_TRANSPORTATION
    },
    
    {
	"hellfire",         {  93, 93, 93, 93, 93, 93, 45, 93 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_hellfire,      TAR_CHAR_OFFENSIVE,          POS_FIGHTING,
        NULL,       SLOT(520),        20,       12,
	"hellfire", "!hellfire!", "",	CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_ATTACK
    },

    {
	"holy aura",	{ 93, 93,93,93,62,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_holy_aura,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,		SLOT(617),	75,	12,
	"",	"Your holy aura vanishes.",	"", 
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"holy fury",	{ 93, 93,93,93,59,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_holy_fury,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,		SLOT(682),	50,	24,
	"",	"You become more tolerable.",	"", 
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"holy word",		{ 93,48,93,93,38,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_holy_word,	TAR_IGNORE,	POS_FIGHTING,
	NULL,			SLOT(506), 	200,	24,
	"divine wrath",		"!Holy Word!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_BENEDICTIONS
    },

    {
	"hurricane",	{  93, 93, 93, 93, 93,53,93,93,93,93,93,65,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_hurricane,	TAR_IGNORE,	POS_FIGHTING,
	NULL,		SLOT(672),	200,	24,
	"helical flow",	"!Hurricane!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"hydroblast",	{  93,93,93,93, 93,93,93,46, 93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_hydroblast,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(684),	50,	12,
	"water fist",	"!Hydroblast!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"iceball",         {  93, 93, 93, 93, 93,58,93,93,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_iceball,     TAR_IGNORE,     POS_FIGHTING,
        NULL,                   SLOT(513),        25,       12,
	"iceball",         "!Iceball!", "",	CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	"identify",		{ 18, 36, 93, 93,93,93,93,93 ,25,93,93,93,18},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_identify,		TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(53),	12,	24,
	"",			"!Identify!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_DETECTION
    },

    {
	"improved detect",	{ 40, 93, 93, 93, 93,93,93, 93 ,40,93,93,40,40},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_improved_detection,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,		SLOT(626),	 20,	12,
	"",			"You feel less aware of your surroundings.",	
	"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_DETECTION
    },

    {
	"improved invis",	{  43, 93, 93, 93,93,93,93,93,43,93,93,93,43 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_improved_invis,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_imp_invis,		SLOT(627),	 20,	12,
	"",			"You are no longer invisible.",		
	"$p fades into view.", CABAL_NONE , RACE_NONE,ALIGN_NONE, GROUP_ILLUSION
    },

    {
	"infravision",		{  17,  25, 93, 93,93,93,5,93,18,93,93,21,17},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_infravision,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(77),	 5,	18,
	"",			"You no longer see in the dark.",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_ENHANCEMENT
    },

    {
	"insanity",	{ 93, 93,93, 93,93,93,93,93 ,93,93,93,59,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_insanity,		TAR_CHAR_OFFENSIVE,	POS_STANDING,
	NULL,		SLOT(616),	100,	24,
	"",	"Now you feel yourself calm down.",	"", 
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"inspire",		{   93, 49, 93,93,93,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_inspire,		TAR_IGNORE,		POS_STANDING,
	NULL,		SLOT(587),	75,	24,
	"",			"You feel less inspired",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_BENEDICTIONS
    },

    {
	"invisibility",		{  10, 93, 93, 93,93,9,93,93,13,93,93,93,10 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_invis,		TAR_OBJ_CHAR_DEF,	POS_STANDING,
	&gsn_invis,		SLOT(29),	 5,	12,
	"",			"You are no longer invisible.",		
	"$p fades into view.", CABAL_NONE , RACE_NONE,ALIGN_NONE, GROUP_ILLUSION
    },

    {
	"iron body",		{ 93, 93,93,93, 93,93,93,93 ,29,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_iron_body,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(718),	75,	12,
	"",	"The skin regains its softness.",	"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"iron golem",		{93,93,93,93,93,93,93,93,93,93,93,52,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_iron_golem,	TAR_IGNORE,		POS_STANDING,
	NULL,		SLOT(664),	400,		24,
	"",	"You gained enough mana to make more golems now.","",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"knock",		{   56,93,93,93,93,93,93,93,64,93,93,62,46},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_knock,	TAR_IGNORE,	POS_STANDING,
	NULL,		SLOT(603),	20,	24,
	"",			"!knock!",
	"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"know alignment",	{  22,  13,93,93,1,5,93,93,29,93,93,22,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_know_alignment,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(58),	 9,	12,
	"",			"!Know Alignment!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_DETECTION
    },

    {
	"lesser golem",		{93,93,93,93,93,93,93,93,93,93,93,25,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_lesser_golem,	TAR_IGNORE,		POS_STANDING,
	NULL,		SLOT(662),	200,		12,
	"",	"You gained enough mana to make more golems now.","",
	CABAL_NONE , 	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"lethargic mist",    { 52, 93,93,93,93, 62, 93,93,56,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_lethargic_mist,	TAR_IGNORE,	POS_STANDING,
	&gsn_lethargic_mist,	SLOT(676),	200,	24,
	"",		"!lethargic mist!",	"", 
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_MALADICTIONS
    },

    {
	"light arrow",	{  93, 93, 93, 93,64,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_light_arrow,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(683),	40,	12,
	"light arrow",	"!light arrow!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	"lightning bolt",	{  23, 93, 93, 93,93,13,93,93 ,23,93,93,23,23},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_lightning_bolt,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_lightning_bolt,		SLOT(30),	15,	12,
	"lightning bolt",	"!Lightning Bolt!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	"lightning shield",	{ 41, 93,93, 93,93,93,93,93 ,93,93,93,41,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_lightning_shield,	TAR_IGNORE,	POS_STANDING,
	NULL,		SLOT(614),	150,	24,
	"lightning shield",	"Now you can shield your room again.",	"", 
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"link",		{   31, 93, 93, 93,93,93,93,93,33 ,93,93,33,33},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_link,		TAR_CHAR_DEFENSIVE,		POS_STANDING,
	NULL,		SLOT(588),	125,	18,
	"",			"!link!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_MEDITATION
    },

    {
	"lion help",		{   93, 93, 93, 93,93,93,93,63,93 ,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_lion_help,	TAR_IGNORE,		POS_STANDING,
	NULL,		SLOT(595),	100,	12,
	"",	"Once again, you may send a slayer lion.",	"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"locate object",	{   30, 30, 93, 93,19,93,14,93,29,93,93,93,31},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_locate_object,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(31),	20,	18,
	"",			"!Locate Object!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_DETECTION
    },

    {
	"love potion",         { 93, 93, 93,93,93,93,93,93 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_love_potion, TAR_CHAR_SELF,       POS_STANDING,
        &gsn_love_potion,  SLOT(666),        10,       0,
	"", "You feel less dreamy-eyed.", "",	CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"magic jar",		{   93, 93, 93, 93,93,93,93,93,93 ,93,93,68,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_magic_jar,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(596),	20,	12,
	"",			"!magic jar!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_BEGUILING
    },

    {
	"magic missile",	{ 1, 93, 93, 93,93,1,93,93 ,1,93,93,2,1},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_magic_missile,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_magic_missile,		SLOT(32),	15,	12,
	"magic missile",	"!Magic Missile!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	"magic resistance",	{   68, 93, 93, 93,93,93,93,93,72 ,93,93,69,71},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_magic_resistance,	TAR_CHAR_SELF,	POS_STANDING,
	NULL,		SLOT(605),	200,	24,
	"",			"You are again defenseless to magic.",
	"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	"magnetic trust",	{ 93, 93, 93, 93, 93, 93,93,93,34,93,93,93,34},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_magnetic_trust,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(656),	20,	12,
	"magnetic trust",	"!magnetic trust!", "", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	"mass healing",		{ 93, 54, 93, 93,93,93,93,93,93 ,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_mass_healing,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(508),	100,	36,
	"",			"!Mass Healing!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_HEALING
    },

    {
	"mass invis",		{ 43, 47, 93, 93,93,93,93,93 ,42,93,93,93,46},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_mass_invis,	TAR_IGNORE,		POS_STANDING,
	&gsn_mass_invis,	SLOT(69),	20,	24,
	"",			"You are no longer invisible.",		"", CABAL_NONE , 
	RACE_NONE,ALIGN_NONE, GROUP_ILLUSION
    },

    {
	"mass sanctuary",	{   93, 62, 93, 93,93,93,93,93,93 ,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_mass_sanctuary,	TAR_IGNORE,		POS_STANDING,
	NULL,		SLOT(589),	200,	24,
	"",			"The white aura around your body fades.",
	"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	"master healing",	{  93,58,93,93,70,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_master_heal,	TAR_CHAR_DEFENSIVE,POS_FIGHTING,
	NULL,			SLOT(641),	300,	12,
	"",			"!Master Heal!",	"",CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_HEALING
    },
  
    {
	"meld into stone",	{ 20, 20,20, 20,20,20,20,20 ,20,20,20,20,20},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_meld_into_stone,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(584),	12,	18,
	"",	"The stones on your skin crumble into dust.",	"",
	CABAL_NONE , RACE_ROCKSEER, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	"mend",		{   50, 93, 93, 93,93,93,93,93,50 ,93,93,93,50},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_mend,		TAR_OBJ_INV,		POS_STANDING,
	&gsn_mend,		SLOT(590),	150,	24,
	"",			"!mend!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_ENCHANTMENT
    },

    {
	"mind light",	{ 93, 83,93, 93,93,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_mind_light,	TAR_IGNORE,		POS_STANDING,
	NULL,		SLOT(82),	200,	24,
	"",	"You can booster more rooms now.",	"", 
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"mind wrack",	{ 93, 93, 93, 93, 93, 27, 93,93,93,93,93,93,93},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_mind_wrack,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(650),	20,	12,
	"mind wrack",	"!mind wrack!", "", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	"mind wrench",	{ 93, 93, 93, 93, 93, 40, 93,93,93,93,93,93,93},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_mind_wrench,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(651),	20,	12,
	"mind wrench",	"!mind wrench!", "", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	"mist walk",	{  93,93,93,93,93,93,93,93,93,93,47,47,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_mist_walk,	TAR_IGNORE,	POS_FIGHTING,
	NULL,			SLOT(658),	80,	12,
	"",			"!Mist Walk!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_TRANSPORTATION
    },

    {
	"mummify",		{ 93, 93, 93, 93,93,93,93,93,93,93,93,44,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_mummify,	TAR_OBJ_CHAR_OFF,	POS_STANDING,
	NULL,		SLOT(715),	50,	12,
	"",	"You gain energy to give live to new corpses.", 	"", 
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"mysterious dream", 	{ 93, 93,93,93,93,35,93,93,49,93,93,27,93},
		{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_mysterious_dream,	TAR_IGNORE,	POS_STANDING,
	&gsn_mysterious_dream,	SLOT(678),	200,	24,
	"",		"!mysterous dream!",	"", 
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_BEGUILING
    },

    {
	"nexus",                { 93,93,93,93,93,93,93,93 ,60,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_nexus,            TAR_IGNORE,             POS_STANDING,
        NULL,                   SLOT(520),       150,   36,
	"",                     "!Nexus!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_TRANSPORTATION
    },

    {
	"pass door",		{  24, 32, 93, 93,93,93,93,93 ,24,24,93,27,24},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_pass_door,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(74),	20,	12,
	"",			"You feel solid again.",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_TRANSPORTATION
    },

    {
	"plague",		{  93, 93, 93, 93,93,46,93,93,33,93,93,36,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_plague,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_plague,		SLOT(503),	20,	12,
	"sickness",		"Your sores vanish.",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_MALADICTIONS
    },

    {
	"poison",		{  93,  22, 93,93,93,15,93,93,27,93,93,23,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_poison,		TAR_OBJ_CHAR_OFF,	POS_FIGHTING,
	&gsn_poison,		SLOT(33),	10,	12,
	"poison",		"You feel less sick.",	
	"The poison on $p dries up.", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_MALADICTIONS
    },

    {
	"polymorph",    { 93, 93, 93, 93,93, 93,93,93,73,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_polymorph,	TAR_IGNORE,	POS_STANDING,
	NULL,			SLOT(639),	250,	24,
	"",			"You return to your own race.","", 
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_BENEDICTIONS
    },

    {
	"portal",               { 54, 93, 93, 93, 93,93,93,93,40,93,93,43,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_portal,           TAR_IGNORE,             POS_STANDING,
        NULL,                   SLOT(519),       100,     24,
	"",                     "!Portal!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_TRANSPORTATION
    },

    {
	"protection cold",{   93, 44, 93, 93, 93,93,93,25, 93,93,93,50,44},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_protection_cold,	TAR_CHAR_SELF,	POS_STANDING,
	&gsn_protection_cold,		SLOT(600),	5,	12,
	"",			"You feel less protected",
	"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	"protection evil",	{ 93,  9, 93, 93,11,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_protection_evil,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(34), 	5,	12,
	"",			"You feel less protected.",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	"protection good",	{ 93,9,93,93,93,13,93,93,93,93,27,17,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_protection_good,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(666), 	5,	12,
	"",			"You feel less protected.",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	"protection heat",	{ 93, 39, 93, 93,93,93,93,93, 93,93,93,48,39},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_protection_heat,	TAR_CHAR_SELF,	POS_STANDING,
	&gsn_protection_heat,		SLOT(599),	5,	12,
	"",			"You feel less protected",
	"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	"protection negative",{   93, 93,93,93,93,93,93,93,93,93,93,15,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_protection_negative,	TAR_CHAR_SELF,	POS_STANDING,
	NULL,		SLOT(636),	20,	12,
	"",		"You feel less protected from your own attacks.",
	"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	"protective shield",        { 26,38,93,93, 93,93,93,93 ,26,93,93,18,26},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_protective_shield,      TAR_CHAR_SELF,   POS_STANDING,
        &gsn_protective_shield,       SLOT(572),        70,       12,
	"", "Your shield fades away.", "", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	"power word kill",	{   93, 93, 93, 93,93,93,93,93,93 ,93,93,78,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_power_kill,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
	NULL,		SLOT(604),	200,	18,
	"powerful word",		"You gain back your durability.",
	"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"power word stun", { 93, 93,93, 93,93, 62,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_power_stun,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_power_stun,		SLOT(625),	200,	24,
	"",	"You can move now.",	"", 
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"quantum spike",	{ 41, 93, 93, 93, 93, 93,93,93,93,93,93,93,41},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_quantum_spike,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(657),	20,	12,
	"quantum spike",	"!quantum spike!", "", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	"ranger staff",         {  93, 93, 93, 93, 93, 93, 93, 32 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_ranger_staff,      TAR_IGNORE,          POS_FIGHTING,
        NULL,             SLOT(519),        75,       0,
	"", "!ranger staff!", "", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"ray of truth",         { 93, 37, 93, 93, 45,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_ray_of_truth,     TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        &gsn_ray_of_truth,      SLOT(518),      20,     12,
	"ray of truth",         "!Ray of Truth!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_BENEDICTIONS
    },

    {
	"recharge",		{ 29,93,93,93, 93, 93, 93, 93, 29,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_recharge,		TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(517),	60,	24,
	"",			"!Recharge!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_ENCHANTMENT
    },

    {
	"refresh",		{  93,7, 93,93, 93,93,93,93, 93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_refresh,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(81),	12,	18,
	"refresh",		"!Refresh!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_HEALING
    },

    {
	"reincarnation",      {  93, 93, 93, 93, 93,93,93,93,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,        TAR_IGNORE,          POS_STANDING,
        NULL,             SLOT(668),        0,       0,
	"", "!!", "",	CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"remove curse",		{  93, 32, 93, 93,13,93,93,93,93,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_remove_curse,	TAR_OBJ_CHAR_DEF,	POS_STANDING,
	&gsn_remove_curse,	SLOT(35),	 5,	12,
	"",			"!Remove Curse!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_CURATIVE
    },

    {
	"remove fear",		{ 93, 34, 93,93,41,93,93,93,93,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_remove_fear, 	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,		SLOT(582),	5,	12,
	"",	"!Remove Fear!", "", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"remove tattoo",         { 93, 93, 93,93,93,93,93,93,93,93,93,93,93 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_remove_tattoo, TAR_CHAR_DEFENSIVE,       POS_STANDING,
        NULL,       SLOT(552),        10,       0,
	"", 	"!remove tattoo!", 	"",	
        CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"resilience",	{  93,93, 93, 93,56,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_resilience,	TAR_CHAR_DEFENSIVE, POS_STANDING,
	NULL,			SLOT( 638),	 50,	12,
	"",	"You feel less armored to draining attacks.", "", 
	CABAL_NONE ,RACE_NONE, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	"restoring light",    { 93, 71, 93,93,93,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_restoring_light,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(643),	50,	24,
	"",			"!restoring light!","", 
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_BENEDICTIONS
    },

    {
	"sanctify lands",    { 93, 41, 93, 93,53, 93, 93, 93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_sanctify_lands,	TAR_IGNORE,	POS_STANDING,
	NULL,			SLOT(673),	200,	24,
	"",		"!sanctify lands!",	"", 
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_BENEDICTIONS
    },

    {
	"sanctuary",		{ 93, 29, 93, 93,27,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_sanctuary,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_sanctuary,		SLOT(36),	75,	12,
	"",			"The white aura around your body fades.",
	"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	"sand storm",		{ 93, 93, 93, 93,93,93,93,93,93,93,93,93,51 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_sand_storm,	TAR_IGNORE,	POS_FIGHTING,
	&gsn_sand_storm,		SLOT(577),	200,	24,
	"storm of sand",	"The sands melts in your eyes.","", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"scream",		{ 52, 93, 93,93,93,93,93,93,48,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_scream,	TAR_IGNORE,	POS_FIGHTING,
	&gsn_scream,		SLOT(578),	200,	24,
	"scream",	"You can hear again.",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"severity force",	{ 93, 45,93,93,93, 93, 93, 93,93,93 ,93,93,41},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_severity_force,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(622),	20,	12,
	"severity force",		"!severity force!", "", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"shield",		{ 20, 35, 93, 93,93,93,19,93,93 ,93,20,20,20},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_shield,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_shield,		SLOT(67),	12,	18,
	"",	"Your force shield shimmers then fades away.", 	"", 
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	"shielding",	{   93, 93, 93, 93,93,93,93,93,53 ,93,93,53,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_shielding,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_shielding,		SLOT(591),	250,	12,
	"",			"You feel the glow of the True source in the distance",	
	"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"shocking grasp",	{ 8, 93, 93, 93,93,93,93,93 ,9,93,93,93,8},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_shocking_grasp,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(53),	15,	12,
	"shocking grasp",	"!Shocking Grasp!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	"shocking trap",	{ 37, 93,93, 93,93,93,93,93 ,40,93,93,37,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_shocking_trap,	TAR_IGNORE,	POS_STANDING,
	NULL,		SLOT(615),	150,	24,
	"shocking trap",	"Now you can trap more rooms with shocks.","", 
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"sleep",		{ 93, 93, 93, 93,93,25,93,93,39,93,93,39,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_sleep,		TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_sleep,		SLOT(38),	15,	12,
	"",			"You feel less tired.",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_BEGUILING
    },

    {
	"slow",                 { 28,38,93,93, 93, 28, 93, 93 ,23,93,28,29,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_slow,             TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        &gsn_slow,                   SLOT(515),      30,     12,
	"",                     "You feel yourself speed up.",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_MALADICTIONS
    },

    {
	"soften",		{ 93, 93, 93, 93,93,93,93,93 ,58,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_soften,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(713),	 75,	12,
	"soften",	"Your skin regains its robustness.",
	"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_WEATHER
    },

    {
	"solar flight",		{93,35,93,93,93,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_solar_flight,	TAR_IGNORE,	POS_FIGHTING,
	NULL,			SLOT(659),	80,	12,
	"",			"!Solar Flight!",	"",CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_TRANSPORTATION
    },

    {
	"sonic resonance",	{ 93, 93, 93, 93, 93, 93,93,93,93,93,93,28,93},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_sonic_resonance,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(649),	20,	12,
	"sonic resonance",	"!sonic resonance!", "", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	"soul bind",		{ 93, 93, 93, 93,93,39,93,93, 93,93,93,54,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_soul_bind,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
	NULL,		SLOT(716),	 5,	12,
	"",		"You feel more self-confident.",	"", 
	CABAL_NONE, RACE_NONE, ALIGN_NONE, GROUP_BEGUILING
    },

    {
	"spectral furor",	{ 93, 93, 93, 93, 93, 93,93,93,93,93,93,35,93},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_spectral_furor,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(646),	20,	12,
	"spectral furor",	"!spectral furor!", "", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	"stone golem",		{93,93,93,93,93,93,93,93,93,93,93,39,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_stone_golem,	TAR_IGNORE,		POS_STANDING,
	NULL,		SLOT(663),	300,		18,
	"",	"You gained enough mana to make more golems now.","",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"stone skin",		{ 25, 36, 93,93,93,93,35,93,93,93,60,30,25},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_stone_skin,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(66),	12,	18,
	"",			"Your skin feels soft again.",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	"suffocate",      {  93,93,93,93, 93,93,93,93, 48,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_suffocate,    TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
        NULL,             SLOT(714),        50,       12,
	"breathlessness",     "You can breath again.", "",CABAL_NONE, 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"sulfurus spray",	{ 93, 93, 93, 93, 93,93,93,93,28,93,93,93,93},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_sulfurus_spray,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(652),	20,	12,
	"sulfurus spray",	"!sulfurus spray!", "", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	"summon",		{  23, 22, 93, 93,51,50,93,93,24,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_summon,		TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(40),	50,	12,
	"",			"!Summon!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_TRANSPORTATION
    },

    {
	"summon air elemental", { 93, 93, 93, 93, 93, 93,93,93,93,93,93,93,42},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_summon_air_elm,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(696),	50,	12,
	"",	"You gain back the energy to summon another air elemental.",
	"", 	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"summon earth elemental",{ 93, 93, 93, 93, 93, 93,93,93,93,93,93,93,38},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_summon_earth_elm,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(693),	50,	12,
	"",	"You gain back the energy to summon another earth elemental.",
	"", 	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"summon fire elemental",{ 93, 93, 93, 93, 93, 93,93,93,93,93,93,93,58},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_summon_fire_elm,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(697),	50,	12,
	"",	"You gain back the energy to summon another fire elemental.",
	"", 	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"summon lightning elemental",{ 93,93,93,93,93, 93,93,93,93,93,93,93,68},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_summon_light_elm,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(710),	50,	12,
	"", "You gain back the energy to summon another lightning elemental.",
	"", 	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"summon water elemental",{ 93, 93, 93, 93, 93, 93,93,93,93,93,93,93,48},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_summon_water_elm,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(698),	50,	12,
	"",	"You gain back the energy to summon another water elemental.",
	"", 	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"summon shadow",	{ 93, 93,93,93,93,93,93,93,93,93,93,50,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_summon_shadow,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,		SLOT(620),	200,	24,
	"",	"You can summon more shadows, now.",	"", 
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"superior heal",	{  93, 38, 93,93,43,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_super_heal,	TAR_CHAR_DEFENSIVE,POS_FIGHTING,
	NULL,			SLOT(640),	100,	12,
	"",			"!Super Heal!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_HEALING
    },
  
    {
	"tattoo",    { 93, 93, 93,93,93,93,93,93,93,93,93,93,93},
	{1,1,1,1,1,1,1,1,1, 1,1,1,1},
        spell_tattoo, TAR_CHAR_DEFENSIVE,       POS_STANDING,
        NULL,       SLOT(551),        10,       0,
	"", "!tattoo!", "",CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"teleport",		{  22, 32, 93, 93,93,7,93,93 ,23,93,93,16,16},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_teleport,		TAR_CHAR_SELF,		POS_FIGHTING,
	&gsn_teleport,	 		SLOT( 2),	35,	12,
	"",			"!Teleport!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_TRANSPORTATION
    },

    {
	"transfer object",	{ 62, 93, 93, 93, 93, 93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_transfer_object,	TAR_IGNORE,		POS_STANDING,
	NULL,		SLOT(708),	40,	12,
	"",		"!transfer object!", "",
 	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"tsunami",		{ 93, 93, 93, 93, 93, 93,93,93,93,93,93,93,62},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_tsunami,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(701),	50,	12,
	"raging tidal wave",	"!tsunami!",	"", 	CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"turn",		{   93, 50, 93, 93,60,93,93,93,93 ,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_turn,	TAR_IGNORE,	POS_FIGHTING,
	NULL,		SLOT(597),	50,	12,
	"",	"You can handle turn spell again.",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"vampiric blast",	{ 93, 93, 93, 93, 93, 93, 93, 93,93,93 ,38,93,93},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_vam_blast,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(611),	20,	12,
	"vampiric blast",	"!Vampiric Blast!", "",CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"ventriloquate",	{   1, 93, 93, 93,93,93,93,93, 1,93,93,93,3},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_ventriloquate,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(41),	 5,	12,
	"",			"!Ventriloquate!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_ILLUSION
    },

    {
	"web",		{   93, 93, 93, 93,93,93,93,93,69 ,93,93,58,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_web,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_web,		SLOT(592),	50,	12,
	"",		"The webs around you dissolve.",
	"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_ATTACK
    },

    {
	"windwall",		{ 93, 93, 93, 93, 93, 93,93,93,93,93,93,93,47},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_windwall,		TAR_IGNORE,		POS_FIGHTING,
	NULL,			SLOT(694),	20,	12,
	"air blast",		"Your eyes feel better.",	"",
 	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_COMBAT
    },

    {
	"witch curse",		{   93, 93, 93, 93,93,93,93,93,93 ,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_witch_curse,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_witch_curse,		SLOT(602),	150,	24,
	"",			"You gain back your durability.",
	"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"wrath",         { 93, 93, 93,93,34,93,93,93,93,93,93,93,93 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_wrath, TAR_CHAR_OFFENSIVE,       POS_STANDING,
	&gsn_wrath,       SLOT(553),        20,       12,
	"heavenly wrath", "The curse wears off.", "",	CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_BENEDICTIONS
    },

    {
	"weaken",	{   93, 24, 93, 93,93,93,93,93 ,19,93,16,29,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_weaken,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_weaken,		SLOT(68),	20,	12,
	"spell",		"You feel stronger.",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_MALADICTIONS
    },

    {
	"word of recall",	{ 32, 28, 93, 93,32,93,29,93 ,32,38,10,10,32},
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
	"acid breath",		{  67, 67, 67, 67,67,67,67,67 ,67,67,67,67,67},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_acid_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_acid_breath,		SLOT(200),	100,	24,
	"blast of acid",	"!Acid Breath!",	"", CABAL_NONE , 
	RACE_TOP, ALIGN_NONE, GROUP_DRACONIAN
    },

    {
	"desert heat",		{ 69, 69, 69, 69,69,69,69,69,69,69,69,69,69 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(629),	200,	24,
	"cloud of blistering desert heat","The smoke leaves your eyes.",	"", CABAL_NONE , 
	RACE_TOP, ALIGN_NONE, GROUP_DRACONIAN
    },

    {
	"fire breath",		{ 69, 69, 69, 69,69,69,69,69,69,69,69,69,69 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_fire_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_fire_breath,		SLOT(201),	200,	24,
	"blast of flame",	"The smoke leaves your eyes.",	"", CABAL_NONE , 
	RACE_TOP, ALIGN_NONE, GROUP_DRACONIAN
    },

    {
	"frost breath",		{ 61,61,61,61,61,61,61,61,61,61,61,61,61  },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_frost_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_frost_breath,		SLOT(202),	125,	24,
	"blast of frost",	"!Frost Breath!",	"", CABAL_NONE , 
	RACE_TOP, ALIGN_NONE, GROUP_DRACONIAN
    },

    {
	"gas breath",		{ 70,70,70,70,70,70,70,70,70,70,70,70,70  },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_gas_breath,	TAR_IGNORE,		POS_FIGHTING,
	&gsn_gas_breath,	SLOT(203),	175,	24,
	"blast of gas",		"!Gas Breath!",		"", CABAL_NONE , 
	RACE_TOP, ALIGN_NONE, GROUP_DRACONIAN
    },

    {
	"lightning breath",	{ 64,64,64,64,64,64,64,64,64,64,64,64,64  },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_lightning_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_lightning_breath,		SLOT(204),	150,	24,
	"blast of lightning",	"!Lightning Breath!",	"", CABAL_NONE , 
	RACE_TOP, ALIGN_NONE, GROUP_DRACONIAN
    },

    {
	"lightning stroke",		{ 69, 69, 69, 69,69,69,69,69,69,69,69,69,69 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(632),	200,	24,
	"stroke of lightning",	"!lightning stroke!",	"", CABAL_NONE , 
	RACE_TOP, ALIGN_NONE, GROUP_DRACONIAN
    },

    {
	"luck bonus",	{ 67, 67, 67, 67,67,67,67,67,67,67,67,67,67 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,		SLOT( 630),	 20,	12,
	"",		"You feel less armored against magic.",	"", CABAL_NONE,
	RACE_TOP, ALIGN_NONE, GROUP_PROTECTIVE
    },

    {
	"paralyzation",		{ 69, 69, 69, 69,69,69,69,69,69,69,69,69,69 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_IGNORE,	POS_FIGHTING,
	NULL,		SLOT(631),	200,	24,
	"gas of paralyzation",	"You feel you can move again.",	"", CABAL_NONE , 
	RACE_TOP, ALIGN_NONE, GROUP_DRACONIAN
    },

    {
	"repulsion",		{ 65, 65, 65, 65,65,65,65,65,65,65,65,65,65 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(633),	200,	24,
	"repulsion",	"!repulsion!",	"", CABAL_NONE , 
	RACE_TOP, ALIGN_NONE, GROUP_DRACONIAN
    },

    {
	"sleep gas",	{ 67, 67, 67, 67,67,67,67,67,67,67,67,67,67 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_IGNORE,	POS_FIGHTING,
	NULL,		SLOT(628),	200,	24,
	"sleep gas",	"You feel drained.",	"", CABAL_NONE , 
	RACE_TOP, ALIGN_NONE, GROUP_DRACONIAN
    },

    {
	"slow gas",	{ 69, 69, 69, 69,69,69,69,69,69,69,69,69,69 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(634),	200,	24,
	"slow gas",	"You can move faster now.",	"", CABAL_NONE , 
	RACE_TOP, ALIGN_NONE, GROUP_DRACONIAN
    },

/*
 * Skills for mobiles. (general purpose and high explosive from
 * Glop/Erkenbrand
 */
    {
	"crush",	{93,93,93,93,93,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_crush,            	SLOT( 0),       0,      18,
	"crush",                 "!crush!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_FIGHTMASTER
    },

    {
	"general purpose",      { 93, 93, 93, 93, 93, 93, 93, 93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_general_purpose,  TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,                   SLOT(401),      0,      12,
	"general purpose ammo", "!General Purpose Ammo!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },
 
    {
	"high explosive",       { 93, 93, 93, 93, 93, 93, 93, 93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_high_explosive,   TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,                   SLOT(402),      0,      12,
	"high explosive ammo",  "!High Explosive Ammo!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"tail",			{  53, 53,53, 53,53,53,53,53,53 ,53,53,53,53},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_tail,            	SLOT( 0),       0,      18,
	"tail",                 "!Tail!",		"", CABAL_NONE , 
	RACE_TOP, ALIGN_NONE, GROUP_FIGHTMASTER
    },

/* combat and weapons skills */

    {
	"arrow",	{ 93, 93,93, 93,93,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_IGNORE,	POS_STANDING,
	&gsn_arrow,	SLOT(0),	0,	0,
	"arrow",	"!arrow!",	"", 
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_WEAPONSMASTER
    },

    {
	"axe",			{  93, 93, 93,  1,93,1,93,1 ,93 ,93,93,93,93},
	{ 2, 2, 1, 1,1,1,1,1,1, 1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_axe,            	SLOT( 0),       0,      0,
	"",                     "!Axe!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_WEAPONSMASTER
    },

    {
	"bow",	{ 93, 93,93, 44,93,93,93,35 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_IGNORE,	POS_STANDING,
	&gsn_bow,		SLOT(0),	0,	12,
	"bow",		"!bow!",	"", 
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_WEAPONSMASTER
    },

    {
	"dagger",               { 1,  93,  1,  1,1,1,1,1 ,1 ,1,1,1,1},
	{ 2, 2, 1, 1,1,1,1,1,1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_dagger,            SLOT( 0),       0,      0,
	"",                     "!Dagger!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_WEAPONSMASTER
    },
 
    {
	"flail",		{ 93,  1, 93,  1,93,1,93,93 ,93,93,93,93,1},
	{ 2, 2, 1, 1,1,1,1,1,1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_flail,            	SLOT( 0),       0,      0,
	"",                     "!Flail!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_WEAPONSMASTER
    },

    {
	"lance",	{ 33,33,33,33,33,33,33,33,33,33,33,33,33},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_IGNORE,	POS_STANDING,
	&gsn_lance,	SLOT(0),	0,	0,
	"lance",	"!lance!",	"", 
	CABAL_KNIGHT , RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	"mace",			{  93,  1,  1,  1,1,1,93,93 ,93 ,1,93,93,93},
	{ 2, 2, 1, 1,1,1,1,1,1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_mace,            	SLOT( 0),       0,      0,
	"",                     "!Mace!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_WEAPONSMASTER
    },

    {
	"polearm",		{ 93, 93, 93,  1,1,1,93,93,93 ,93,93,93,93},
	{ 2, 2, 1, 1,1,1,1,1,1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_polearm,           SLOT( 0),       0,      0,
	"",                     "!Polearm!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_WEAPONSMASTER
    },
    
    {
	"shield block",		{ 93,17,7,1,1,1,12,10,93 ,10,93,93,93},
	{ 2, 1, 1, 1,1,1,1,1,1,1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_shield_block,	SLOT(0),	0,	0,
	"",			"!Shield!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_DEFENSIVE
    },
 
    {
	"spear",		{  93,  93,  1,  1,93,1,1,1 ,93,1,8,93,93 },
	{ 2, 2, 1, 1,1,1,1,1,1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_spear,            	SLOT( 0),       0,      12,
	"spear",                     "!Spear!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_WEAPONSMASTER
    },

    {
	"sword",		{  93, 93,  1,  1,1,1,1,1,93 ,1,1,93,93},
	{ 2, 2, 1, 1,1,1,1,1,1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_sword,            	SLOT( 0),       0,      0,
	"",                     "!sword!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_WEAPONSMASTER
    },

    {
	"whip",			{  1, 1,  93,  1,93,1,93,1 ,1,93,1,1,1},
	{ 2, 2, 1, 1,1,1,1,1,1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_whip,            	SLOT( 0),       0,      0,
	"",                     "!Whip!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_WEAPONSMASTER
    },

    {
	"second weapon",	{  93,93,93,23,48,48,93,23,93,24,93,93,93},
	{ 2, 2, 1, 1,1,1,1,1,1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_second_weapon,            	SLOT( 0),       0,      0,
	"",	"!second weapon!",	"",	CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_WEAPONSMASTER
    },

    {
	"ambush",         {  93, 93, 93, 93, 93, 93, 93, 17,93 ,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,        TAR_CHAR_OFFENSIVE,          POS_STANDING,
        &gsn_ambush,                    SLOT(0),        0,       12,
	"surprise attack", "!Ambush!", "",	CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"area attack",        {93, 93, 93, 73,93,93,93,93,93,69,93,93,93},
	{1,1,1,1,1,1,1,1,1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_area_attack,     SLOT(0),        0,      0,
	"",         "!Area Attack!", 	"",	CABAL_NONE, 
	RACE_NONE, ALIGN_NONE, GROUP_FIGHTMASTER
    },

    {
	"assassinate",         {  93, 93, 93, 93, 93, 93, 65, 93 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_IGNORE,          POS_STANDING,
        &gsn_assassinate,       SLOT(0),        0,       18,
	"assassination attempt", "!assassinate!", "",	CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"backstab",             {  93, 93,  11, 93,93,93,93,93 ,93,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_backstab,          SLOT( 0),        0,     12,
	"backstab",             "!Backstab!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"bash",			{  93,93,93,1, 6,1,1,10 ,93 ,93,11,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_bash,            	SLOT( 0),       0,      18,
	"bash",                 "!Bash!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_FIGHTMASTER
    },

    {
	"bash door",	{  93, 93, 93,  1,6,1,1,93 ,93,93,11,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_bash_door,        	SLOT( 0),       0,      18,
	"bash",                 "!Bash Door!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_FIGHTMASTER
    },

    {
	"bear call",         {  93, 93, 93, 93, 93, 93, 93, 31,93,93,93,93,93 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,		TAR_IGNORE,          POS_FIGHTING,
        &gsn_bear_call,		SLOT(518),	50,       0,
	"", "You feel you can handle more bears now.", "",	CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"berserk",		{ 93, 93, 93, 18,93,93,93,93 ,93,20,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_berserk,        	SLOT( 0),       0,      24,
	"",                     "You feel your pulse slow down.",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"blackguard",	{  93, 93, 93, 93, 93, 93, 93, 93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_CHAR_OFFENSIVE,   POS_STANDING,
	&gsn_blackguard,		SLOT(0),    0,       12,
	"blackguard",		"Your blackguard fades away.","", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"blackjack",		{  93, 93, 30, 93, 93, 93, 93, 93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_CHAR_OFFENSIVE,   POS_STANDING,
	&gsn_blackjack,		SLOT(0),    0,       8,
	"blackjack",		"Your head feels better.","", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"blind fighting",	{   93, 93, 93, 13,32,93,11,43,93 ,23,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_blind_fighting,		SLOT(0),	0,	0,
	"",			"!blind fighting!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_FIGHTMASTER
    },

    {
	"blindness dust",         {  93, 93, 93, 93, 93, 93, 28, 93,93,93,93,93,93 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,   		TAR_IGNORE,             POS_FIGHTING,
        &gsn_blindness_dust,    SLOT(0),        20,       18,
	"",                     "!blindness dust!", "",	 CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },
 
    {
	"blink",                {  1, 93,  93, 93,93,93,93,93 ,1,93,93,1,1},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_blink,             SLOT( 0),        0,     0,
	"",                     "!Blink!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"butcher",         {  93, 93, 93, 93, 93, 93, 93, 13 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,        TAR_IGNORE,          POS_STANDING,
        &gsn_butcher,             SLOT(0),        0,       0,
	"", "!butcher!", "",	CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"caltraps",         {  93, 93, 93, 93, 93, 93, 18, 93 ,93,36,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_IGNORE,          POS_FIGHTING,
        &gsn_caltraps,       SLOT(0),        0,       18,
	"caltraps", "Your feet feel less sore.", "",	CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"camouflage",         {  93, 93, 93, 93, 93, 93, 93, 4 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,        TAR_IGNORE,           POS_STANDING,
        &gsn_camouflage,         SLOT(0),        0,       24,
	"",         "!Camouflage!", "",	CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"camouflage move",	{ 93, 93, 93, 93, 93,93,93,23, 93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_move_camf,		SLOT(0),	0,	12,
	"",		"!move camouflaged!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"camp",			{  93, 93, 93,  93,93,93,93,48 ,93 ,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_camp,            	SLOT( 0),       0,      24,
	"camp",          "You can handle more camps now.","", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"circle",         {  93, 93, 7, 93, 93, 93, 93, 93,93 ,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,        TAR_CHAR_OFFENSIVE,          POS_FIGHTING,
        &gsn_circle,             SLOT(0),        0,       18,
	"circle stab", "!Circle!", "",		CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"control animal",	{ 93, 93, 93, 93,93,93,93,93,93,93,43,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_control_animal,	SLOT( 0),	 5,	12,
	"",			"You feel more self-confident.",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_BEGUILING
    },

    {
	"cleave",         {  93, 93, 93, 93, 93, 55, 93, 93 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,     TAR_CHAR_OFFENSIVE,     POS_STANDING,
        &gsn_cleave,         SLOT(0),        0,       24,
	"cleave",         "!Cleave!", "",	CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"concentrate",         { 93, 93,93,59,93,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_concentrate,        SLOT( 0),        0,     18,
	"",      "You can concentrate on new fights.",	"",CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_FIGHTMASTER
    },
 
    {
	"counter",         {  93, 93, 93, 28, 35, 93, 93, 93 ,93,28,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,        TAR_IGNORE,           POS_STANDING,
        &gsn_counter,         SLOT(0),        0,       0,
	"",         "!Counter!", "",		CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_FIGHTMASTER
    },

    {
	"critical strike",	{   93, 93, 93, 93,93,93,58,93,93 ,62,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_critical,			SLOT(0),	0,	18,
	"",			"!critical strike!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"cross block",          {  93,93,93,31,56,56,93,31,93,32,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_cross_block,       SLOT( 0),        0,     0,
	"",                     "!cross block!",	"", CABAL_NONE , 
	RACE_NONE,ALIGN_NONE, GROUP_DEFENSIVE
    },

    {
	"detect hidden",	{ 93, 93, 93, 93, 93, 93, 93, 93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_detect_hidden,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,		SLOT(44),	 5,	12,
	"",			"You feel less aware of your surroundings.",	
	"", CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_DETECTION
    },

    {
	"detect sneak",		{   93, 93, 93, 93,93,93,93,93,93 ,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_CHAR_SELF,		POS_STANDING,
	&gsn_detect_sneak,		SLOT(0),	20,	18,
	"",			"!detect sneak!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_DETECTION
    },

    {
	"dirt kicking",		{ 93, 93,  3,  8,93,3,3,5,93 ,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_dirt,		SLOT( 0),	0,	12,
	"kicked dirt",		"You rub the dirt out of your eyes.",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"disarm",               {  93, 93, 28, 20,20,21,23,21,93,21,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_disarm,            SLOT( 0),        0,     18,
	"",                     "!Disarm!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_WEAPONSMASTER
    },
 
    {
	"dodge",                {  33,93,1,13, 21,21,1,9, 93,15,8,93,27},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_dodge,             SLOT( 0),        0,     0,
	"",                     "!Dodge!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_DEFENSIVE
    },
 
   {
	"dual backstab",         {  93, 93, 38, 93, 93, 93, 93, 93 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,             TAR_CHAR_OFFENSIVE,           POS_STANDING,
        &gsn_dual_backstab,       SLOT(0),        0,       0,
	"second backstab",      "!dual backstab!", "",		CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"enchant sword",	{ 93, 93, 93, 93,93,93,93,93,93,42,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_OBJ_INV,		POS_STANDING,
	&gsn_enchant_sword,	SLOT(0),	100,	24,
	"",	"!Enchant sword!",	"", CABAL_NONE , RACE_NONE, 
	ALIGN_NONE, GROUP_NONE
    },

    {
	"endure",         {  93, 93, 93, 93, 93, 93, 20, 93 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,        TAR_CHAR_SELF,          POS_FIGHTING,
        &gsn_endure,             SLOT(0),        0,       24,
	"", "You feel susceptible to magic again.", "",	CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"enhanced damage",      {  93, 33, 25,  3,6,3,16,15 ,93,8,14,93,93},
	{ 1,  2,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_enhanced_damage,   SLOT( 0),        0,     0,
	"",                     "!Enhanced Damage!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_FIGHTMASTER
    },

    {
	"entangle",         {  93,93,93,93,93,93,93,26 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_entangle,      TAR_CHAR_OFFENSIVE,   POS_FIGHTING,
        &gsn_entangle,       SLOT(568),        40,       12,
	"entanglement", "You feel less entangled.", "",	CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"envenom",		{ 93, 93, 15, 93, 93, 93, 93, 93,93,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,	  	POS_RESTING,
	&gsn_envenom,		SLOT(0),	0,	12,
	"",			"!Envenom!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"escape",		{93,93,45,93,93,93,55,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_escape,		SLOT( 0),	0,	24,
	"",	"!escape!","", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"explode",         {  93, 93, 93, 93, 93, 93, 93, 93 ,93,46,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_CHAR_OFFENSIVE,          POS_FIGHTING,
        &gsn_explode,       SLOT(0),        100,       24,
	"flame", "The smoke leaves your eyes.", "",	CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"ground strike",	{ 93,93,93,93, 93,93,93,52, 93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_ground_strike,	SLOT(0),	0,	18,
	"",			"!ground strike!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"hand block",                { 93,93,93,93,93,93,22,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_hand_block,             SLOT( 0),        0,     0,
	"",                     "!hand block!",		"", CABAL_NONE , 
	RACE_NONE,ALIGN_NONE, GROUP_DEFENSIVE
    },

    {
	"hand to hand",		{ 45,  30, 18, 9,11,18,3,24,45,12,19,45,35 },
	{ 2,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_hand_to_hand,	SLOT( 0),	0,	0,
	"",			"!Hand to Hand!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_FIGHTMASTER
    },

    {
	"hara kiri",		{ 93, 93, 93, 93,93,93,93,93,93,25,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null, 	TAR_IGNORE,	POS_FIGHTING,
	&gsn_hara_kiri,		SLOT(0),	50,	12,
	"",	"You feel you gain your life again.", "", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"headguard",	{  93, 93, 93, 93, 93, 93, 93, 93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_CHAR_OFFENSIVE,   POS_STANDING,
	&gsn_headguard,		SLOT(0),    0,       12,
	"headguard",		"Your headguard fades away.","", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"herbs",         {  93, 93, 93, 93, 93, 93, 93, 18 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,        TAR_CHAR_DEFENSIVE,          POS_STANDING,
        &gsn_herbs,             SLOT(0),        0,       30,
	"", "The herbs look more plentiful here.", "",	CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"kick",                 {  93, 12, 14,  5,93,9,1,8 ,93,2,4,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,             TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        &gsn_kick,              SLOT( 0),        0,     12,
	"kick",                 "!Kick!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_FIGHTMASTER
    },

    {
	"lash",			{ 48,48,93,93, 48,93,48,93, 48,93,48,48,48},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_lash,            	SLOT( 0),       0,      4,
	"lash",                 "!Lash!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_FIGHTMASTER
    },

    {
	"light resistance",	{  93,  93, 93, 93,93,93,93,93,93,93,50,93,93},
	{ 1,  1,  2,  2, 1, 1, 1, 1,1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_SLEEPING,
	&gsn_light_res,		SLOT( 0),	0,	0,
	"",			"Light Resistance","", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"lion call",		{ 93, 93, 93, 93,93,93,93,55,93,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null, 	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	&gsn_lion_call,		SLOT(0),	50,	12,
	"",		"!lion call!", 		"", 
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"make arrow",		{ 93, 93, 93,93,93,93,93,37,93,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_make_arrow,	SLOT(0),	 50,	24,
	"",		"!make arrow!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"make bow",		{ 93, 93, 93,93,93,93,93,36,93,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_make_bow,		SLOT(0),	 200,	24,
	"",		"!make bow!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"mental attack",	{ 93, 93,93, 93,93,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_CHAR_SELF,		POS_STANDING,
	&gsn_mental_attack,		SLOT(0),	200,	24,
	"",	"!mental attack!",	"", 
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"neckguard",	{  93, 93, 93, 93, 93, 93, 93, 93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_CHAR_OFFENSIVE,   POS_STANDING,
	&gsn_neckguard,		SLOT(0),    0,       12,
	"neckguard",		"Your neckguard fades away.","", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"nerve",         {  93, 93, 93, 93, 93, 93, 29, 93 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,        TAR_CHAR_OFFENSIVE,          POS_FIGHTING,
        &gsn_nerve,             SLOT(0),        0,       18,
	"", "Your nerves feel better.", "",	CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"parry",                { 93,25,19,1, 3,5,93,3, 23,1,22,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_parry,             SLOT( 0),        0,     0,
	"",                     "!Parry!",		"", CABAL_NONE , 
	RACE_NONE,ALIGN_NONE, GROUP_DEFENSIVE
    },

    {
	"perception",      {  93, 93, 23, 93, 93,93,93,93,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,        TAR_IGNORE,          POS_STANDING,
        &gsn_perception,             SLOT(0),        0,       0,
	"", "!perception!", "",	CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"push",			{  93, 93, 53,  93,93,93,93,93 ,93 ,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_push,            	SLOT( 0),       0,      18,
	"push",          "!push!","", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"rescue",               {  93, 93, 93,  1,1,93,93,93,93,23,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_rescue,            SLOT( 0),        0,     12,
	"",                     "!Rescue!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_DEFENSIVE
    },

    {
	"sense life",	{ 93, 93,93, 93,93,93,93,93 ,93,93,13,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_CHAR_SELF,		POS_STANDING,
	&gsn_sense_life,	SLOT(623),	20,	12,
	"",	"You lost the power to sense life.",	"", 
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"settraps",	{ 93, 93, 59, 93,93,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_CHAR_SELF,		POS_STANDING,
	&gsn_settraps,		SLOT(0),	200,	24,
	"trap",	"You can set more traps now.",	"", 
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"shield cleave",{ 93, 93, 33, 26, 29, 29, 31, 35, 93, 26, 39, 93, 93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_CHAR_SELF,		POS_STANDING,
	&gsn_shield_cleave,		SLOT(0),	200,	24,
	"",	"!shield cleave!",	"", 
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"smithing",	{   93, 93, 93, 50,93,93,93,93,93 ,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_smithing,			SLOT(0),	10,	18,
	"",			"!smithing!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"spell craft",		{ 10,30,93,93, 93,93,93,93, 10,93,93,10,10 },
	{ 1,  1,  1,  1, 1, 1, 2, 2, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_spell_craft,	SLOT(0),    0,       0,
	"spell craft",		"!spell craft!","", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"strangle",         {  93,  93,  93,  93,93,93,26,93 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_CHAR_OFFENSIVE,          POS_STANDING,
        &gsn_strangle,       SLOT(0),        0,       8,
	"strangulation", "Your neck feels better.", "",	CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"swimming",	{   93, 93, 93, 93,93,93,93,93,93 ,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_swimming,		SLOT(0),	0,	0,
	"",			"!swimming!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"target",                 {  93, 93, 93,  93,93,93,93,93 ,93,40,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,             TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        &gsn_target,              SLOT( 0),        0,     12,
	"target",                 "!Kick!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"throw",         {  93, 93, 93, 93, 93, 93, 25, 93 ,93,37,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_CHAR_OFFENSIVE,       POS_FIGHTING,
        &gsn_throw,       SLOT(0),        0,       18,
	"throw", "!throw!", "",	CABAL_NONE , RACE_NONE,ALIGN_NONE, GROUP_NONE
    },

    {
	"tiger power",		{ 93,93,93,93, 93,93,93,61, 93,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null, 	TAR_IGNORE,	POS_FIGHTING,
	&gsn_tiger_power,	SLOT(0),	0,	12,
	"",	"You feel your tigers escaped.", "", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"track",         {  93, 93, 93,93,93,93,93,39,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_IGNORE,       POS_STANDING,
        &gsn_track,       SLOT(0),        0,       12,
	"", "!track!", "",CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"trip",			{  93, 93,  3, 25,93,10,8,93 ,93,20,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_trip,		SLOT( 0),	0,	12,
	"trip",			"!Trip!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_FIGHTMASTER
    },

    {
	"vampire",		{ 93, 93, 93, 93,93,93,93,93,93,93,10,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_IGNORE,		POS_STANDING,
	&gsn_vampire,		SLOT(0),	100,	12,
	"",	"Now you are familer to other creatures.",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"vampiric bite",         {  93, 93,  93, 93,93,93,93,93 ,93,93,10,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_vampiric_bite,          SLOT( 0),        0,     12,
	"vampiric bite",             "!vampiric bite!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"vampiric touch",         {  93,  93,  93,  93,93,93,93,93 ,93,93,15,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,      TAR_CHAR_OFFENSIVE,          POS_STANDING,
	&gsn_vampiric_touch,       SLOT(0),        0,       12,
	"vampiric touch", "You wake up from nightmares.", "",	CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"vanish",         {  93, 93, 93, 93, 93, 93, 18, 93 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_CHAR_SELF,          POS_FIGHTING,
        &gsn_vanish,       SLOT(521),        25,       18,
	"", "!vanish!", "", 	CABAL_NONE , RACE_NONE,ALIGN_NONE, GROUP_NONE
    },

    {
	"warcry",         {  93, 93, 93,31,93,93,93,35,93 ,30,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_IGNORE,       POS_FIGHTING,
        &gsn_warcry,       SLOT(0),        30,       12,
	"", "Your warcry has worn off.", "",	CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"weapon cleave", { 93, 93, 37, 29, 34, 34, 35, 38, 93, 29, 44, 93, 93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_CHAR_SELF,		POS_STANDING,
	&gsn_weapon_cleave,	SLOT(0),	200,	24,
	"",	"!weapon cleave!",	"", 
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
        "second attack",        {  34, 28, 18,  10,13,14,15,13,25,10,12,34,34},
	{ 2,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_second_attack,     SLOT( 0),        0,     0,
	"",                     "!Second Attack!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_FIGHTMASTER
    },

    {
	"secondary attack",	{93,93,93,52, 61,61,93,58 ,93,55,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_CHAR_SELF,		POS_STANDING,
	&gsn_secondary_attack,	SLOT(0),	200,	24,
	"",	"!secondary attack!",	"", 
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"third attack",         { 93, 93,36,19,27,26,26,45,93,20,32,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_third_attack,      SLOT( 0),        0,     0,
	"",                     "!Third Attack!",	"", CABAL_NONE , 
	RACE_NONE,ALIGN_NONE, GROUP_FIGHTMASTER
    },

    {
	"fourth attack",        {93,93,93,30, 93,93,48,93, 93,93,93,93,93},
	{1,1,1,1,1,1,1,1,1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_fourth_attack,     SLOT(0),        0,      0,
	"",                     "!Fourth Attack!", 	"",	CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_FIGHTMASTER
    },

    {
	"fifth attack",        {93, 93, 93, 52, 93, 93, 93,93,93,93,93,93,93},
	{1,1,1,1,1,1,1,1,1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_fifth_attack,     SLOT(0),        0,      0,
	"",          "!Fifth Attack!", 	"",	CABAL_NONE, 
	RACE_NONE, ALIGN_NONE, GROUP_FIGHTMASTER
    },

/* non-combat skills */

    {
	"blue arrow",		{ 93, 93, 93,93,93,93,93,51,93,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_blue_arrow,	SLOT(0),	 50,	12,
	"",		"!blue arrow!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    { 
	"fast healing",		{  15, 9, 16,  6,7,7,9,6 ,15 ,3,9,9,9},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_SLEEPING,
	&gsn_fast_healing,	SLOT( 0),	0,	0,
	"",			"!Fast Healing!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_MEDITATION
    },

    {
	"green arrow",		{ 93, 93, 93,93,93,93,93,40,93,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_green_arrow,	SLOT(0),	 50,	12,
	"",		"!green arrow!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"grip",           { 93, 93, 93,43,93,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_grip,            SLOT( 0),        0,     18,
	"",                     "!Grip!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_WEAPONSMASTER
    },
 
    {
	"haggle",		{  7, 18,  1, 14,12,8,17,15 ,7 ,19,5,5,5},
	{ 2,  2,  1,  2, 2, 2, 1, 2, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_RESTING,
	&gsn_haggle,		SLOT( 0),	0,	0,
	"",			"!Haggle!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"hide",			{  93, 93,  1,  93,93,93,10,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_RESTING,
	&gsn_hide,		SLOT( 0),	 0,	18,
	"",			"!Hide!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"katana",	{ 93, 93, 93, 93,93,93,93,93,93,72,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_OBJ_INV,		POS_STANDING,
	&gsn_katana,		SLOT(0),	100,	24,
	"",		"You can now make another katana.",	"",
	CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"lay hands",		{93,93,93,93,63,93,93,93,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_lay_hands,		SLOT( 0),	0,	12,
	"",	"You may heal more innocents now.",	"",CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"lore",			{  93,93,13,21,19,20,19,18,93,22,20,93,93},
	{ 2,  2,  2,  2, 2, 2, 2, 2, 2, 2,2,2,2},
	spell_null,		TAR_IGNORE,		POS_RESTING,
	&gsn_lore,		SLOT( 0),	0,	36,
	"",			"!Lore!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"mastering pound",	{ 93,93,93,93,93,93,61,93,93,93,93,93,93},
	{ 2,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_master_hand,	SLOT( 0),	0,	0,
	"",			"!Master Hand!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_FIGHTMASTER
    },

    {
	"mastering sword",	{ 93,93,93,93,93,93,93,93,93,61,93,93,93},
	{ 2, 2, 1, 1,1,1,1,1,1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_master_sword,           SLOT( 0),       0,      0,
	"",                     "!master sword!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_WEAPONSMASTER
    },

    {
	"meditation",		{  6,  6, 15, 15,14,15,14,16,6,16,6,6,6},
	{ 1,  1,  2,  2, 1, 1, 1, 1,1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_SLEEPING,
	&gsn_meditation,	SLOT( 0),	0,	0,
	"",			"Meditation",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_MEDITATION
    },

    {
	"peek",			{  93, 93,  1, 93,93,93,93,93,93 ,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_peek,		SLOT( 0),	 0,	 0,
	"",			"!Peek!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"pick lock",		{ 93, 93,  17, 93,93,93,34,93,93,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_pick_lock,		SLOT( 0),	 0,	12,
	"",			"!Pick!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"poison smoke",         {  93, 93, 93, 93, 93, 93, 30, 93,93,93,93,93,93 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,     	TAR_IGNORE,            POS_FIGHTING,
        &gsn_poison_smoke,      SLOT(0),        20, 	18,
	"",                     "!poison smoke!", "",	CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"quiet movement",         {  93, 93, 93, 93, 93, 93, 93, 15 ,93,93,10,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,        TAR_IGNORE,          POS_STANDING,
        &gsn_quiet_movement,             SLOT(0),        0,       0,
	"", "!quiet movement!", "",	CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"recall",		{  1,  1,  1,  1,1,1,1,1 ,1,1,1,1,1},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_recall,		SLOT( 0),	0,	24,
	"",			"!Recall!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"red arrow",		{ 93, 93, 93,93,93,93,93,44,93,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_red_arrow,	SLOT(0),	 50,	12,
	"",		"!red arrow!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"sneak",		{  93, 93, 4, 93, 93, 93, 18, 93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_sneak,		SLOT( 0),	 0,	12,
	"",			"You no longer feel stealthy.",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"steal",		{  93, 93, 12, 93, 93, 93, 93, 93,93 ,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_steal,		SLOT( 0),	 0,	12,
	"",			"!Steal!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"scrolls",		{  1,  1,  1,  93,93,1,93,1 , 1,6,1,1,1},
	{ 1,  1,  2,  2, 2, 2, 2, 2, 2, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_scrolls,		SLOT( 0),	0,	12,
	"",			"!Scrolls!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_WIZARD
    },

    {
	"staves",		{  1,  1,  93,  93,93,93,1,1 ,1,1,1,1,1},
	{ 1,  1,  2,  2, 2, 2, 2, 2, 2, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_staves,		SLOT( 0),	0,	12,
	"",			"!Staves!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_WIZARD
    },
    
    {
	"tame",         {  93, 93, 93, 93, 93, 93, 93, 39 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,        TAR_CHAR_DEFENSIVE,          POS_FIGHTING,
        &gsn_tame,             SLOT(0),        0,       24,
	"", "!tame!", "",CABAL_NONE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"trance",         {  14,15,93,93,93,93,93,93,14 ,93,21,21,21},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_IGNORE,   POS_SLEEPING,
        &gsn_trance,       SLOT(0),        0,       0,
	"", "", "",	CABAL_NONE , RACE_NONE,ALIGN_NONE, GROUP_MEDITATION
    },

    {
	"white arrow",		{ 93, 93, 93,93,93,93,93,49,93,93,93,93,93 },
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_white_arrow,	SLOT(0),	 50,	12,
	"",		"!white arrow!",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"wands",		{  1,  1,  93,  93,93,1,93,93 ,1,5,1,1,1},
	{ 1,  1,  2,  2, 2, 2, 2, 2, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_wands,		SLOT( 0),	0,	12,
	"",			"!Wands!",		"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_WIZARD
    },

/* cabals */

    {
	"mortal strike",	{ 63,63,63,63,63,63,63,63,63,63,63,63,63},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,	TAR_CHAR_SELF,		POS_STANDING,
	&gsn_mortal_strike,		SLOT(0),	200,	24,
	"mortal strike",	"!mortal strike!",	"", 
	CABAL_BATTLE , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"disgrace",	{  33, 33, 33, 33, 33,33,33,33,33,33,33,33,33},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_disgrace,	TAR_CHAR_OFFENSIVE,		POS_STANDING,
	NULL,		SLOT(619),	200,	12,
	"",	"You feel yourself getting prouder.",	"", 
	CABAL_CHAOS , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"evil spirit",	{  33, 33, 33, 33, 33,33,33,33,33,33,33,33,33},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_evil_spirit,	TAR_IGNORE,		POS_STANDING,
	&gsn_evil_spirit,		SLOT(618),	800,	36,
	"evil spirit",	"Your body regains its full spirit.",	"", 
	CABAL_INVADER , RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"ruler aura",      {  33, 33, 33, 33, 33,33,33,33,33,33,33,33,33},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_ruler_aura,    TAR_CHAR_SELF,          POS_STANDING,
        NULL,             SLOT(667),        20,       12,
	"", "Your ruler aura fades.", 	"",	CABAL_RULER , 
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	"sword of justice",	{48,48,48,48, 48,48,48,48,48,48,48,48,48},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_sword_of_justice,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(686),	50,	12,
	"sword of justice",	"!sword of justice!", "", CABAL_RULER , 
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	"bandage",        {23, 23, 23, 23,23,23,23,23,23,23,23,23,23},
	{1,1,1,1,1,1,1,1,1,1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_bandage,     SLOT(0),        0,      0,
	"",        "You feel less healthy.", 	"",	CABAL_BATTLE, 
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	"cabal recall",		{ 10,10,10,10,10,10,10,10,10,10,10,10,10},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_cabal_recall,		SLOT( 0),	0,	24,
	"",	"You may pray for transportation again.","", CABAL_BATTLE , 
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        "wanted",               {  1,  1,  1,  1, 1, 1, 1, 1 ,1,1,1,1,1},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_DEAD,
        &gsn_wanted,                   SLOT(0),        0,       0,
        "",                     "!Wanted!", "",		CABAL_RULER,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        "judge",               {  1,  1,  1,  1, 1, 1, 1, 1 ,1,1,1,1,1},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,             TAR_IGNORE,             POS_DEAD,
        &gsn_judge,              SLOT(0),        0,       0,
        "",                     "!Judge!", "",		CABAL_RULER,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        "bloodthirst",         {  15, 15, 15, 15, 15, 15, 15, 15 ,15,15,15,15,15},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_CHAR_SELF,       POS_FIGHTING,
        &gsn_bloodthirst,       SLOT(0),        0,       12,
        "", "Your bloody rage fades away.", "",	CABAL_BATTLE,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        "spellbane",         { 10,10,10,10,10,10,10,10,10,10,10,10,10 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_CHAR_SELF,       POS_STANDING,
        &gsn_spellbane,       SLOT(0),        0,       12,
        "spellbane", "You feel less resistant to magic.", "",	CABAL_BATTLE,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        "resistance",         {  12, 12, 12,12,12,12,12,12 ,12,12,12,12,12},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_CHAR_SELF,       POS_FIGHTING,
        &gsn_resistance,       SLOT(0),        0,       24,
        "", "You feel less tough.", "",	CABAL_BATTLE,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        "deathblow",         {  20,20,20,20,20,20,20,20 ,20,20,20,20,20},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_IGNORE,       POS_STANDING,
        &gsn_deathblow,       SLOT(0),        0,       0,
        "", "!deathblow!", "",	CABAL_BATTLE,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        "transform",         {  10, 10, 10,10,10,10,10,10 ,10,10,10,10,10},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_transform,      TAR_CHAR_SELF,       POS_STANDING,
        NULL,       SLOT(522),        100,       24,
        "", "You feel less healthy.", "",	CABAL_SHALAFI,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	"mental knife",         {  15, 15, 15,93,15,15,15,15,15,15,15,15,15 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_mental_knife,      TAR_CHAR_OFFENSIVE,       POS_FIGHTING,
        &gsn_mental_knife,       SLOT(524),        35,       12,
        "mental knife", "Your mental pain dissipates.", "",CABAL_SHALAFI,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	"demon summon",         {  25, 25, 25,93,25,25,25,25 ,25,25,25,25,25},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_demon_summon,      TAR_CHAR_SELF,       POS_FIGHTING,
        &gsn_demon_summon,       SLOT(525),        100,       12,
        "", "You feel your summoning power return.", "", CABAL_SHALAFI,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	"scourge",         {  30, 30, 30,93,30,30,30,30,30,30,30,30,30 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_scourge,      TAR_IGNORE,       POS_FIGHTING,
        NULL,       SLOT(526),        50,       18,
        "Scourge of the Violet Spider", "!scourge!", "",CABAL_SHALAFI,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        "manacles",         {  20, 20, 20,20,20,20,20,20 ,20,20,20,20,20},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_manacles,      TAR_CHAR_OFFENSIVE,       POS_FIGHTING,
        NULL,       SLOT(528),        75,       12,
        "", "Your shackles dissolve.", "", 	CABAL_RULER,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        "shield of ruler",         {  10, 10, 10,10,10,10,10,10 ,10,10,10,10,10},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_shield_ruler,      TAR_IGNORE,       POS_FIGHTING,
        NULL,       SLOT(529),        100,       12,
        "", "!shield!", "",	CABAL_RULER,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        "guard",         {  22,22,22,22,22,22,22,22 ,22,22,22,22,22},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_IGNORE,       POS_STANDING,
        &gsn_guard,       SLOT(0),        0,       12,
        "", "", "",	CABAL_KNIGHT,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        "guard call",         {  15, 15, 15,15,15,15,15,15 ,15,15,15,15,15},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_guard_call,      TAR_IGNORE,       POS_FIGHTING,
	&gsn_guard_call,       SLOT(530),        75,       12,
        "", 	"You may call more guards now.", 	"",	CABAL_RULER,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        "nightwalker",         {  25, 25, 25,25,25,25,25,25 ,25,25,25,25,25},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_nightwalker,      TAR_IGNORE,       POS_FIGHTING,
	&gsn_nightwalker,       SLOT(531),        75,       12,
        "", "You feel your summoning power return.", "",CABAL_INVADER,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        "eyes of intrigue",         {  13, 13, 13,13,13,13,13,13,13,13,13,13,13 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_eyes,      TAR_IGNORE,       POS_FIGHTING,
	NULL,       SLOT(532),        75,       12,
        "", "!eyes of intrigue!", "",	CABAL_INVADER,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        "fade",         {  20, 20, 20,20,20,20,20,20 ,20,20,20,20,20},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_IGNORE,       POS_STANDING,
        &gsn_fade,       SLOT(0),        0,       24,
        "", "!fade!", "",	CABAL_INVADER,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        "shadow cloak",         {  10, 10, 10,10,10,10,10,10,10,10,10,10,10 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_shadow_cloak,      TAR_CHAR_DEFENSIVE,       POS_STANDING,
	NULL,       SLOT(533),        10,       12,
        "", "The shadows no longer protect you.", "",	CABAL_INVADER,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        "nightfall",         {  16, 16, 16,16,16,16,16,16,16,16,16,16,16 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_nightfall,      TAR_IGNORE,       POS_STANDING,
	NULL,       	SLOT(534),        50,       12,
        "", "You are now able to control lights.", "",	CABAL_INVADER,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        "aura of chaos",      {  16, 16, 16,16,16,16,16,16,16,16,16,16,16 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_aura_of_chaos,      TAR_CHAR_DEFENSIVE,       POS_STANDING,
	NULL,       	SLOT(720),        20,       12,
        "", "The gods of chaos no longer protect you.", 	"",
	CABAL_CHAOS, RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        "garble",         {  10, 10, 10,10,10,10,10,10 ,10,10,10,10,10},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_garble,      TAR_CHAR_DEFENSIVE,       POS_FIGHTING,
	&gsn_garble,       SLOT(535),        30,       12,
        "", "Your tongue untwists.", "",	CABAL_CHAOS,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        "mirror",         { 15, 15, 15,15,15,15,15,15,15,15,15,15,15 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_mirror,      TAR_CHAR_DEFENSIVE,       POS_STANDING,
	&gsn_mirror,       SLOT(536),        40,       12,
	"", "You fade away.", "",	CABAL_CHAOS,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	"confuse",         {  20, 20, 20,20,20,20,20,20 ,20,20,20,20,20},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_confuse,      TAR_CHAR_OFFENSIVE,       POS_FIGHTING,
	&gsn_confuse,       SLOT(537),        20,       12,
	"", "You feel less confused.", "",	CABAL_CHAOS,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	"doppelganger",         {  25, 25, 25,25,25,25,25,25 ,25,25,25,25,25},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_doppelganger,      TAR_CHAR_DEFENSIVE,       POS_STANDING,
	&gsn_doppelganger,       SLOT(527),        75,       12,
	"", "You return to your native form.", "",	CABAL_CHAOS,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        "chaos blade",         {  23, 23, 23,23,23,23,23,23,23,23,23,23,23 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_chaos_blade,      TAR_IGNORE,       POS_STANDING,
        &gsn_chaos_blade,       SLOT(550),        60,       12,
        "", "!chaos blade!", "",	CABAL_CHAOS,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

   {
        "stalker",         {  93, 93, 93,93,93,93,93,93 ,93,93,93,93,93},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_stalker,      TAR_IGNORE,       POS_STANDING,
        &gsn_stalker,       SLOT(554),        100,       12,
        "", "You feel up to summoning another stalker.", "",
	CABAL_RULER, 	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        "randomizer",         {  28, 28, 28,28,28,28,28,28 ,28,28,28,28,28},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_randomizer,      TAR_IGNORE,       POS_STANDING,
        &gsn_randomizer,       SLOT(555),        200,       24,
        "", "You feel your randomness regenerating.", "", CABAL_CHAOS,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	"tesseract",         {  29, 29, 29,93,29,29,29,29 ,29,29,29,29,29},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_tesseract,      TAR_IGNORE,       POS_STANDING,
        &gsn_tesseract,       SLOT(556),        150,       12,
        "", "!tesseract!", "",	CABAL_SHALAFI,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        "trophy",         {  23, 23, 23,23,23,23,23,23 ,23,23,23,23,23},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_IGNORE,       POS_STANDING,
        &gsn_trophy,       SLOT(0),        30,       12,
        "", "You feel up to making another trophy.", "",	CABAL_BATTLE,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        "truesight",         {  25, 25, 25,25,25,25,25,25 ,25,25,25,25,25},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_IGNORE,       POS_STANDING,
        &gsn_truesight,       SLOT(0),        50,       12,
        "", "Your eyes see less truly.", "",	CABAL_BATTLE,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        "brew",         {  23, 23, 23,23,23,23,23,23,23 ,23,23,23,23},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_brew,      TAR_OBJ_INV,       POS_STANDING,
        &gsn_brew,       SLOT(557),        25,       12,
        "", "You feel like you can start brewing again.", "",
	CABAL_SHALAFI,	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        "shadowlife",         {  30, 30, 30,30,30,30,30,30 ,30,30,30,30,30},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_shadowlife,      TAR_CHAR_OFFENSIVE,       POS_STANDING,
        &gsn_shadowlife,       SLOT(558),        80,       12,
        "", "Your feel more shadowy.", "",	CABAL_INVADER,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        "ruler badge",         {  25, 25, 25,25,25,25,25,25 ,25,25,25,25,25},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_ruler_badge,      TAR_CHAR_SELF,   POS_STANDING,
        &gsn_ruler_badge,       SLOT(560),        50,       12,
        "", "!ruler badge!", "",	CABAL_RULER,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        "remove badge",         {  28, 28, 28,28,28,28,28,28,28,28,28,28,28 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_remove_badge,      TAR_CHAR_SELF,   POS_STANDING,
        &gsn_remove_badge,       SLOT(561),        100,       12,
        "", "!remove badge!", "",	CABAL_RULER,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        "golden aura",         {  10,10,10,10,10,10,10,10,10,10,10,10,10 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_golden_aura,      TAR_IGNORE,   POS_STANDING,
        NULL,       SLOT(564),        25,       12,
        "", "You feel the golden aura dissipate.", "",	CABAL_KNIGHT,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        "dragonplate",         {  15,15,15,15,15,15,15,15 ,15,15,15,15,15},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_dragonplate,      TAR_IGNORE,   POS_STANDING,
        NULL,       SLOT(565),        60,       12,
        "", "", "",	CABAL_KNIGHT,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        "squire",         {  25, 25, 25,25,25,25,25,25 ,25,25,25,25,25},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_squire,      TAR_IGNORE,   POS_STANDING,
        &gsn_squire,       SLOT(566),        100,       12,
        "", "You feel up to worrying about a new squire.", "",	CABAL_KNIGHT,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        "dragonsword",         {  25,25,25,25,25,25,25,25 ,25,25,25,25,25},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_dragonsword,      TAR_IGNORE,   POS_STANDING,
        &gsn_dragonsword,       SLOT(567),        70,       12,
	"", "", "",	CABAL_KNIGHT,	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        "holy armor",         {  10,10,10,10,10,10,10,10,10 ,10,10,10,10},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_holy_armor,      TAR_CHAR_SELF,   POS_RESTING,
        &gsn_knight,       SLOT(569),        20,       12,
        "", "You are less protected from harm.", "",	CABAL_KNIGHT,	
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },


    {
        "disperse",         {  20,20,20,20,20,20,20,20,20,20,20,20,20 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_disperse,      TAR_IGNORE,   	POS_FIGHTING,
        &gsn_disperse,       SLOT(573),        100,       24,
        "", "You feel up to doing more dispersing.", "",CABAL_CHAOS,
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	"hunt",         {  13, 13, 13,13,13,13,13,13 ,13,13,13,13,13},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_IGNORE,       POS_STANDING,
        &gsn_hunt,       SLOT(0),        0,       6,
	"", 	"!hunt!", 	"",	CABAL_HUNTER, 
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	"find object",	{   25, 25, 25, 25,25,25,25,25,25 ,25,25,25,25},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_find_object,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(585),	20,	18,
	"",			"!Find Object!",	"", CABAL_HUNTER , 
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    { 
	"path find",		{  13,13 ,13,13,13,13,13,13 ,13 ,13,13,13,13},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_SLEEPING,
	&gsn_path_find,		SLOT( 0),	0,	0,
	"",			"!endur!",	"", CABAL_HUNTER , 
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	"riding",		{   30, 30, 30, 30,30,30,30,30,30 ,30,30,30,30},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_riding,			SLOT(0),	0,	6,
	"",			"!riding!",	"", CABAL_KNIGHT , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"wolf",		{   20, 20,20, 20,20,20,20,20,20 ,20,20,20,20},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_wolf,	TAR_IGNORE,		POS_STANDING,
	NULL,		SLOT(593),	100,	12,
	"",	"You feel you can handle more wolfs now.",	"", CABAL_HUNTER , 
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	"wolf spirit",	{   28, 28, 28, 28,28,28,28,28,28 ,28,28,28,28},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_wolf_spirit,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(685),	50,	12,
	"",	"The blood in your vains start to flow as normal.",	"", 
	CABAL_HUNTER , RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	"armor use",		{ 17,17,17,17,17,17,17,17,17 ,17,17,17,17},
	{ 2, 1, 1, 1,1,1,1,1,1,1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_armor_use,	SLOT(0),	0,	0,
	"",			"!Armor Use!",		"", CABAL_HUNTER , 
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    { 
	"world find",	{  53,53 ,53,53,53,53,53,53 ,53 ,53,53,53,53},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_SLEEPING,
	&gsn_world_find,	SLOT( 0),	0,	0,
	"",			"!world find!",	"", CABAL_HUNTER , 
	RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	"take revenge",	{  53,53 ,53,53,53,53,53,53 ,53 ,53,53,53,53},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_take_revenge,	TAR_IGNORE,		POS_STANDING,
	NULL,		SLOT(624),	50,		12,
	"",	"!take revenge!",	"", 
	CABAL_HUNTER , RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
	"mastering spell",	{37,37,37,37,37,37,37,37,37,37,37,37,37},
	{ 1,  1,  1,  1, 1, 1, 2, 2, 1, 1,1,1,1},
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_mastering_spell,	SLOT(0),    0,       0,
	"mastering spell",		"!mastering spell!","",
	CABAL_SHALAFI, RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        "guard dogs",         {  15, 15, 15,15,15,15,15,15 ,15,15,15,15,15},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_guard_dogs,      TAR_IGNORE,       POS_FIGHTING,
	NULL,       SLOT(687),        100,       12,
        "", 	"", 	"",	
	CABAL_LIONS, RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        "eyes of tiger",         {  25, 25, 25,25,25,25,25,25 ,25,25,25,25,25},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_eyes_of_tiger,      TAR_IGNORE,       POS_FIGHTING,
	NULL,       SLOT(688),        20,       12,
        "", 	"", 	"",	
	CABAL_LIONS, RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        "lion shield",         {  24, 24, 24,24,24,24,24,24 ,24,24,24,24,24},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_lion_shield,      TAR_IGNORE,       POS_FIGHTING,
	NULL,       SLOT(689),        200,       12,
        "", 	"", 	"",	
	CABAL_LIONS, RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        "evolve lion",         {  18, 18, 18,18,18,18,18,18 ,18,18,18,18,18},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_evolve_lion,      TAR_IGNORE,       POS_FIGHTING,
	NULL,       SLOT(690),        50,       12,
        "", 	"", 	"",	
	CABAL_LIONS, RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        "claw",         {  20, 20, 20,20,20,20,20,20 ,20,20,20,20,20},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_null,      TAR_IGNORE,       POS_FIGHTING,
	&gsn_claw,       SLOT(0),        50,       24,
        "claw", 	"", 	"",	
	CABAL_LIONS, RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

    {
        "prevent",      {  35, 35, 35,35,35,35,35,35 ,35,35,35,35,35},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
        spell_prevent,      TAR_IGNORE,       POS_FIGHTING,
	NULL,       SLOT(691),        75,       12,
        "", 	"", 	"",	
	CABAL_LIONS, RACE_NONE, ALIGN_NONE, GROUP_CABAL
    },

/* object spells */

    {
	"terangreal",		{ 93,  93, 93, 93,93,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_terangreal,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_terangreal,	SLOT(607),	 5,	12,
	"terangreal",		"You are awake again.",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"kassandra",		{ 93,  93, 93, 93,93,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_kassandra,	TAR_CHAR_SELF,		POS_STANDING,
	&gsn_kassandra,	SLOT(608),	 5,	12,
	"",		"You can heal yourself again.",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"sebat",		{ 93,  93, 93, 93,93,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_sebat,		TAR_CHAR_SELF,		POS_STANDING,
	&gsn_sebat,		SLOT(609),	 5,	12,
	"",			"You can protect yourself again.",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    },

    {
	"matandra",		{ 93,  93, 93, 93,93,93,93,93 ,93,93,93,93,93},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1,1,1,1},
	spell_matandra,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_matandra,		SLOT(610),	 5,	12,
	"holy word",	"You can use kassandra again.",	"", CABAL_NONE , 
	RACE_NONE, ALIGN_NONE, GROUP_NONE
    }

};

