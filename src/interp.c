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
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "interp.h"

#undef IMMORTALS_LOGS

bool	check_social	args( ( CHAR_DATA *ch, char *command,char *argument ) );


/*
 * Command logging types.
 */
#define LOG_NORMAL	0
#define LOG_ALWAYS	1
#define LOG_NEVER	2



/*
 * Log-all switch.
 */
bool				fLogAll		= FALSE;

#ifdef IMMORTALS_LOGS
/*
 * immortals log file
 */
FILE				*imm_log;
#endif

/*
 * Command table.
 */
const	struct	cmd_type	cmd_table	[] =
{
  { "aþaðý",		do_down,	POS_STANDING,	 0,  LOG_NEVER, 0, CMD_KEEP_HIDE|CMD_GHOST },
  { "batý",		do_west,	POS_STANDING,	 0,  LOG_NEVER, 0, CMD_KEEP_HIDE|CMD_GHOST },
  { "doðu",		do_east,	POS_STANDING,	 0,  LOG_NEVER, 0, CMD_KEEP_HIDE|CMD_GHOST },
  { "güney",		do_south,	POS_STANDING,	 0,  LOG_NEVER, 0, CMD_KEEP_HIDE|CMD_GHOST },
  { "kuzey",		do_north,	POS_STANDING,    0,  LOG_NEVER, 0, CMD_KEEP_HIDE|CMD_GHOST },
  { "yukarý",		do_up,		POS_STANDING,	 0,  LOG_NEVER, 0, CMD_KEEP_HIDE|CMD_GHOST },
  { ",",		do_emote,	POS_RESTING,	 0,  LOG_NORMAL, 0, CMD_GHOST },
  { ";",		do_gtell,	POS_DEAD,	 0,  LOG_NORMAL, 0, CMD_GHOST },
  { ":",		do_immtalk,	POS_DEAD,	IM,  LOG_NORMAL, 0, CMD_KEEP_HIDE|CMD_GHOST },
  { "'",		do_say,		POS_RESTING,	 0,  LOG_NORMAL, 0, CMD_GHOST },
  { "aç",		do_open,	POS_RESTING,	 0,  LOG_NORMAL, 1,0 },
  { "acýbana",		do_outfit,	POS_RESTING,	 0,  LOG_NORMAL, 1,0},
  { "advance",	do_advance,	POS_DEAD,	ML,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "affrooms",	do_affrooms,	POS_DEAD,	L4,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "al",		do_get,		POS_RESTING,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE },
  { "allow",		do_allow,	POS_DEAD,	L2,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "andaç",         do_trophy,      POS_STANDING,    0,  LOG_NORMAL, 1,0 },
  { "anýmsa",		do_recall,	POS_FIGHTING,	 0,  LOG_NORMAL, 1,0 },
  { "anlat",		do_tell,	POS_RESTING,	 0,  LOG_NORMAL, 1, CMD_GHOST },
  { "arakla",		do_steal,	POS_STANDING,	 0,  LOG_NORMAL, 1,0 },
  { "aranýyor",         do_wanted,      POS_STANDING,    0,  LOG_ALWAYS, 1,0 },
  { "ardýlan",	do_backstab,	POS_STANDING,	 0,  LOG_NORMAL, 1,0 },
  { "aslançaðýr",       do_lion_call,   POS_FIGHTING,    0,  LOG_NORMAL, 1,0},
  { "at",             do_at,          POS_DEAD,       L6,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST  },
  { "avla",           do_hunt,        POS_STANDING,    0,  LOG_NORMAL,1,0 },
  { "ayýçaðýr",       do_bear_call,   POS_FIGHTING,    0,  LOG_NORMAL, 1,0},
  { "ayrý",		do_qui,		POS_DEAD,	 0,  LOG_NORMAL, 0, CMD_KEEP_HIDE|CMD_GHOST },
  { "ayrýl",		do_quit,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "baðýr",		do_shout,	POS_RESTING,	 3,  LOG_NORMAL, 1, CMD_GHOST },
  { "bak",		do_look,	POS_RESTING,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "bakiye",	do_balance,	POS_STANDING,	 0,  LOG_NORMAL, 1,0 },
  { "ban",		do_ban,		POS_DEAD,	L2,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "bayýlt",       do_strangle,    POS_STANDING,    0,  LOG_NORMAL, 1,0 },
  { "bileþik",	do_combine,	POS_DEAD,        0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "biniþ",          do_mount,       POS_STANDING,    0,  LOG_NORMAL, 1,0 },
  { "bitki",          do_herbs,       POS_STANDING,    0,  LOG_NORMAL, 1,0 },
  { "býrak",		do_drop,	POS_RESTING,	 0,  LOG_NORMAL, 1,0 },
  { "bölge",		do_areas,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "bug",		do_bug,		POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "büyü",		do_cast,	POS_FIGHTING,	 0,  LOG_NORMAL, 1,0},
  { "büyüler",		do_spells,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "büyüyýkýmý",      do_spellbane,   POS_FIGHTING,    0,  LOG_NORMAL, 0,0 },
  { "cabal_scan",	do_cabal_scan,	POS_STANDING,    0,  LOG_NEVER, 0, CMD_KEEP_HIDE|CMD_GHOST },
  { "çaðrýyok",	do_nosummon,	POS_DEAD,        0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "çarp",		do_zap,		POS_RESTING,	 0,  LOG_NORMAL, 1,0 },
  { "çelme",		do_trip,	POS_FIGHTING,    0,  LOG_NORMAL, 1,0 },
  { "cevapla",		do_reply,	POS_SLEEPING,	 0,  LOG_NORMAL, 1, CMD_GHOST },
  { "çevrel",         do_circle,      POS_FIGHTING,    0,  LOG_NORMAL, 1,0 },
  { "çivi",       do_caltraps,    POS_FIGHTING,    0,  LOG_NORMAL, 1,0 },
  { "çýkar",		do_remove,	POS_RESTING,	 0,  LOG_NORMAL, 1,0 },
  { "çýkýþlar",		do_exits,	POS_RESTING,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "clear",		do_clear,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "clone",		do_clone,	POS_DEAD,	L5,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "cls",		do_clear,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "çöp",           do_sacrifice,   POS_RESTING,     0,  LOG_NORMAL, 0,0 },
  { "copla",	do_blackjack,	POS_STANDING,	 0,  LOG_NORMAL, 1,0 },
  { "count",		do_count,	POS_SLEEPING,	HE,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "daðýt",		do_split,	POS_RESTING,	 0,  LOG_NORMAL, 1,0 },
  { "dayaným",         do_endure,      POS_STANDING,    0,  LOG_NORMAL, 1, CMD_KEEP_HIDE },
  { "deðer",		do_value,	POS_RESTING,	 0,  LOG_NORMAL, 1,0 },
  { "deny",		do_deny,	POS_DEAD,	L1,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "dinlen",		do_rest,	POS_SLEEPING,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE },
  { "direnç",     do_resistance,  POS_FIGHTING,    0,  LOG_NORMAL, 0,0 },
  { "disconnect",	do_disconnect,	POS_DEAD,	L3,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "doðra",        do_butcher,     POS_STANDING,    0,  LOG_NORMAL, 1,0 },
  { "dök",		do_pour,	POS_RESTING,	 0,  LOG_NORMAL, 1,CMD_KEEP_HIDE },
  { "dokun",		do_vtouch,	POS_STANDING,	 0,  LOG_NORMAL, 0,0 },
  { "doldur",		do_fill,	POS_RESTING,	 0,  LOG_NORMAL, 1,0 },
  { "dua",           do_pray,        POS_DEAD,        0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "dump",		do_dump,	POS_DEAD,	ML,  LOG_ALWAYS, 0, CMD_KEEP_HIDE|CMD_GHOST },
  { "duygu",		do_emote,	POS_RESTING,	 0,  LOG_NORMAL, 1, CMD_GHOST },
  { "duyuru",		do_duyuru,	POS_DEAD,	L4,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "eðitim",		do_train,	POS_RESTING,	 0,  LOG_NORMAL, 1,0 },
  { "ekipman",	do_equipment,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "emret",		do_order,	POS_RESTING,	 0,  LOG_NORMAL, 1,0},
  { "eniyi",	do_eniyi,	POS_RESTING,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "envanter",	do_inventory,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "estimate",	do_estimate,	POS_RESTING,	 0,  LOG_NORMAL, 1, CMD_GHOST },
  { "etkiler",	do_affects_col,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "familya",	do_familya,	POS_STANDING,	 0,  LOG_NORMAL, 1},
  { "find",		do_find,	POS_DEAD,	ML,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "fýrlat",          do_throw,       POS_FIGHTING,    0,  LOG_NORMAL, 1,0 },
  { "flag",		do_flag,	POS_DEAD,	L4,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "force",		do_force,	POS_DEAD,	L5,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "freeze",		do_freeze,	POS_DEAD,	L7,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "ganlat",		do_gtell,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_GHOST },
  { "gir", 		do_enter, 	POS_STANDING,	 0,  LOG_NORMAL, 1, CMD_GHOST },
  { "giy",		do_wear,	POS_RESTING,	 0,  LOG_NORMAL, 1,0 },
  { "göm",		do_bury,	POS_STANDING,	 0,  LOG_NORMAL, 1,0 },
  { "görev",          do_quest,       POS_RESTING,     0,  LOG_NORMAL, 1 , CMD_GHOST},
  { "görün",	do_visible,	POS_SLEEPING,	 0,  LOG_NORMAL, 1,0 },
  { "göster",		do_show,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "goto",           do_goto,        POS_DEAD,       L8,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "grant",		do_grant,	POS_DEAD,	L2,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "grup",          do_group,       POS_SLEEPING,    0,  LOG_NORMAL, 1, CMD_KEEP_HIDE },
  { "grupliste",          do_glist,       POS_DEAD,        0,  LOG_NEVER,  1, 0},
  { "günce",		do_worth,	POS_SLEEPING,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "harakiri",           do_hara,        POS_STANDING,    0,  LOG_NORMAL,1,0 },
  { "havadurumu",	do_weather,	POS_RESTING,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "haykýr",		do_yell,	POS_RESTING,	 0,  LOG_NORMAL, 1, CMD_GHOST },
  { "hedef",		do_target,	POS_FIGHTING,    0,  LOG_NORMAL, 1,0 },
  { "holylight",	do_holylight,	POS_DEAD,	IM,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "iç",		do_drink,	POS_RESTING,	 0,  LOG_NORMAL, 1,0 },
  { "immtalk",	do_immtalk,	POS_DEAD,	IM,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "incele",	do_examine,	POS_RESTING,	 0,  LOG_NORMAL, 1, CMD_GHOST },
  { "incognito",	do_incognito,	POS_DEAD,	IM,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "induct",		do_induct,	POS_DEAD,	 0,  LOG_ALWAYS, 1,0 },
  { "iniþ",       do_dismount,    POS_STANDING,    0,  LOG_NORMAL, 1,0 },
  { "insan",          do_human,       POS_STANDING,    0,  LOG_NORMAL,1,0 },
  { "intiha",		do_delet,	POS_DEAD,	 0,  LOG_ALWAYS, 0, CMD_KEEP_HIDE|CMD_GHOST },
  { "intihar",		do_delete,	POS_STANDING,	 0,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "invis",		do_invis,	POS_DEAD,	IM,  LOG_NORMAL, 0, CMD_KEEP_HIDE|CMD_GHOST },
  { "iptal",	do_nocancel,	POS_DEAD,        0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "irfan",           do_lore,        POS_RESTING,     0,  LOG_NORMAL, 1,0 },
  { "iste",		do_iste,		POS_RESTING,	 0,  LOG_NORMAL, 1 },
  { "itekle",		do_push,	POS_STANDING,	 0,  LOG_NORMAL, 1,0 },
  { "ititle",		do_ititle,	POS_DEAD,	IM,  LOG_NORMAL, 1,CMD_KEEP_HIDE|CMD_GHOST },
  { "iyileþtir",		do_heal,	POS_RESTING,	 0,  LOG_NORMAL, 1,0 },
  { "ýsýr",		do_vbite,	POS_STANDING,	 0,  LOG_NORMAL, 0,0 },
  { "kaç",		do_flee,	POS_FIGHTING,	 0,  LOG_NORMAL, 1,0 },
  { "kalk",		do_stand,	POS_SLEEPING,	 0,  LOG_NORMAL, 1,0},
  { "kalk",		do_wake,	POS_SLEEPING,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE },
  { "kalkan",		do_shield,	POS_FIGHTING,	 0,  LOG_NORMAL, 1,0 },
  { "kamp",		do_camp,  	POS_STANDING,    0,  LOG_NORMAL, 1,0 },
  { "kamuflaj",     do_camouflage,  POS_STANDING,    0,  LOG_NORMAL, 1,0 },
  { "kanallar",       do_channels,    POS_DEAD,        0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST},
  { "kanasusama",	do_bloodthirst,	POS_FIGHTING,	 0,  LOG_NORMAL, 1,0 },
  { "kanýmsa",	do_crecall,	POS_FIGHTING,	 0,  LOG_NORMAL, 1,0 },
  { "kapat",		do_close,	POS_RESTING,	 0,  LOG_NORMAL, 1,0 },
  { "kapýkýr",	do_bash_door,	POS_FIGHTING,    0,  LOG_NORMAL, 1,0 },
  { "kaplan",		do_tiger,	POS_FIGHTING,	 0,  LOG_NORMAL, 1,0 },
  { "katana",		do_katana, 	POS_STANDING,    0,  LOG_NORMAL, 1,0 },
  { "katle",		do_murde,	POS_FIGHTING,	 0,  LOG_NORMAL, 0,0 },
  { "katlet",		do_murder,	POS_FIGHTING,	 0,  LOG_ALWAYS, 1,0 },
  { "kaydet",		do_save,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_GHOST },
  { "kaz",		do_dig,  	POS_STANDING,    0,  LOG_NORMAL, 1,0 },
  { "kazan",		do_gain,	POS_STANDING,	 0,  LOG_NORMAL, 1,0 },
  { "kd",	do_kd,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "keskingörüþ",      do_truesight,   POS_FIGHTING,    0,  LOG_NORMAL, 0,0 },
  { "kilitaç",         do_unlock,      POS_RESTING,     0,  LOG_NORMAL, 1,0},
  { "kilitle",		do_lock,	POS_RESTING,	 0,  LOG_NORMAL, 1,0 },
  { "kim",		do_who_col,		POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "kimdir",		do_whois,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "kirala",		do_rent,	POS_DEAD,	 0,  LOG_NORMAL, 0,0 },
  { "kýrbaçla",		do_lash,	POS_FIGHTING,    0,  LOG_NORMAL, 1,0 },
  { "kýsayo",		do_alia,	POS_DEAD,	 0,  LOG_NORMAL, 0, CMD_KEEP_HIDE|CMD_GHOST },
  { "kýsayol",		do_alias,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "kýsayolkaldýr",	do_unalias,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "kýyasla",	do_compare,	POS_RESTING,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE },
  { "kk",             do_cb,          POS_SLEEPING,    0,  LOG_NORMAL, 1, CMD_GHOST },
  { "kompakt",	do_compact,	POS_DEAD,        0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "komutlar",	do_commands,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "konsantrasyon",	do_concentrate,	POS_STANDING,	 0,  LOG_NORMAL, 1, 0 },
  { "kontrol",       	do_control,	POS_STANDING,    0,  LOG_NORMAL, 1,0 },
  { "korkaklýk",		do_wimpy,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "körlüktozu",	do_blindness_dust,POS_FIGHTING,	 0,  LOG_ALWAYS, 1,0 },
  { "koþ",		do_run,		POS_STANDING,	ML,  LOG_NEVER, 0, CMD_KEEP_HIDE|CMD_GHOST },
  { "koy",		do_put,		POS_RESTING,	 0,  LOG_NORMAL, 1,0 },
  { "kullan",		do_wear,	POS_RESTING,	 0,  LOG_NORMAL, 1,0},
  { "kurban",	do_sacrifice,	POS_RESTING,	 0,  LOG_NORMAL, 1,0 },
  { "kurtar",		do_rescue,	POS_FIGHTING,	 0,  LOG_NORMAL, 0,0 },
  { "kuyruk",		do_tail,	POS_FIGHTING,    0,  LOG_NORMAL, 1,0 },
  { "lakap",		do_title,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "limited",	do_limited,	POS_DEAD,	L4,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "lisan",		do_speak,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "liste",		do_list,	POS_RESTING,	 0,  LOG_NORMAL, 1,CMD_KEEP_HIDE },
  { "load",		do_load,	POS_DEAD,	L4,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "log",		do_log,		POS_DEAD,	L1,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST  },
  { "lookup",		do_slookup,	POS_DEAD,	L2,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "maximum",	do_maximum,	POS_DEAD,	ML,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "maymuncuk",		do_pick,	POS_RESTING,	 0,  LOG_NORMAL, 1,0 },
  { "memory",		do_memory,	POS_DEAD,	IM,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "mezat",        do_auction,     POS_SLEEPING,    0,  LOG_NORMAL, 1, CMD_GHOST  },
  { "mwhere",		do_mwhere,	POS_DEAD,	IM,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "nara",         do_warcry,      POS_FIGHTING,    0,  LOG_NORMAL, 1,0},
  { "nerede",		do_where,	POS_RESTING,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "newlock",	do_newlock,	POS_DEAD,	L4,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "noaffect",	do_noaffect,	POS_DEAD,	L5,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "nochannels",	do_nochannels,	POS_DEAD,	L5,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "noemote",	do_noemote,	POS_DEAD,	L5,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "noshout",	do_noshout,	POS_DEAD,	L5,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "notell",		do_notell,	POS_DEAD,	L5,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "notitle",	do_notitle,	POS_DEAD,	L7,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "nskor",		do_nscore,	POS_DEAD,	 IM,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "objlist",	do_objlist,	POS_DEAD,	ML,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "odaetki",	do_raffects,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "öðren",		do_learn,	POS_STANDING,	 0,  LOG_NORMAL, 1,0},
  { "öðret",		do_teach,	POS_STANDING,	 0,  LOG_NORMAL, 1,0},
  { "oku",		do_recite,	POS_RESTING,	 0,  LOG_NORMAL, 1,0 },
  { "öldür",		do_kill,	POS_FIGHTING,	 0,  LOG_NORMAL, 1,0},
  { "omuz",		do_bash,	POS_FIGHTING,    0,  LOG_NORMAL, 1,0 },
  { "onar",	do_smithing,	POS_RESTING,	 0,  LOG_NORMAL, 1,0},
  { "otoliste",	do_autolist,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "otosikke",	do_autogold,	POS_DEAD,        0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "otoçýkýþ",	do_autoexit,	POS_DEAD,        0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "otodaðýt",	do_autosplit,	POS_DEAD,        0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "otodestek",	do_autoassist,	POS_DEAD,        0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "otokurban",	do_autosac,	POS_DEAD,        0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "otoyaðma",	do_autoloot,	POS_DEAD,        0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "otur",		do_sit,		POS_SLEEPING,    0,  LOG_NORMAL, 1,0},
  { "owhere",		do_owhere,	POS_DEAD,	IM,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "özet",		do_brief,	POS_DEAD,        0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "paraçek",	do_withdraw,	POS_STANDING,    0,  LOG_NORMAL, 1,0},
  { "parayatýr",	do_deposit,	POS_STANDING,	 0,  LOG_NORMAL, 1,0},
  { "patlat",	do_explode, 	POS_FIGHTING,    0,  LOG_NORMAL, 1,0 },
  { "peace",		do_peace,	POS_DEAD,	L5,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "pençe",		do_claw,	POS_FIGHTING,	 0,  LOG_NORMAL, 1,0},
  { "permban",	do_permban,	POS_DEAD,	L1,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "pmote",		do_pmote,	POS_RESTING,	 0,  LOG_NORMAL, 1, CMD_GHOST },
  { "poofin",		do_bamfin,	POS_DEAD,	L8,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "poofout",	do_bamfout,	POS_DEAD,	L8,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "popularity",	do_popularity,	POS_DEAD,	L2,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "poz",		do_pose,	POS_RESTING,	 0,  LOG_NORMAL, 1, CMD_GHOST },
  //{ "pracnew",        do_pracnew,	POS_SLEEPING,    ML, LOG_NORMAL, 1, CMD_KEEP_HIDE },
  { "pratik",       do_practice,	POS_SLEEPING,    0,  LOG_NORMAL, 1, CMD_KEEP_HIDE },
  { "prefi",		do_prefi,	POS_DEAD,	IM,  LOG_NORMAL, 0, CMD_KEEP_HIDE|CMD_GHOST },
  { "prefix",		do_prefix,	POS_DEAD,	IM,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "premort",	do_premort,	POS_DEAD,	L8,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "protect",	do_protect,	POS_DEAD,	L1,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "purge",		do_purge,	POS_DEAD,	L4,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "pusu",         do_ambush,      POS_STANDING,    0,  LOG_NORMAL, 1,0 },
  { "rapor",		do_report,	POS_RESTING,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "read",		do_read,	POS_RESTING,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "reboo",		do_reboo,	POS_DEAD,	L1,  LOG_NORMAL, 0, CMD_KEEP_HIDE|CMD_GHOST },
  { "reboot",		do_reboot,	POS_DEAD,	L1,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "rename",		do_rename,	POS_DEAD,	L5,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "restore",	do_restore,	POS_DEAD,	L4,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "return",         do_return,      POS_DEAD,       L6,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "rezil",	do_dishonor,	POS_FIGHTING,	 0,  LOG_NORMAL, 1,0 },
  { "rica",        do_request,     POS_STANDING,    0,  LOG_NORMAL, 1,0 },
  { "saðýr",		do_deaf,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "saklan",		do_hide,	POS_RESTING,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE },
  { "saldýr",		do_kill,	POS_FIGHTING,	 0,  LOG_NORMAL, 0,0},
  { "sapta",         do_detect_hidden,POS_RESTING,    0,  LOG_NORMAL, 1,0},
  { "sargý",        do_bandage,     POS_FIGHTING,    0,  LOG_NORMAL, 1,0 },
  { "sat",		do_sell,	POS_RESTING,	 0,  LOG_NORMAL, 1,0 },
  { "satýnal",		do_buy,		POS_RESTING,	 0,  LOG_NORMAL, 1,0},
  { "savun",          do_guard,       POS_STANDING,    0,  LOG_NORMAL, 1,0 },
  { "savur",	do_brandish,	POS_RESTING,	 0,  LOG_NORMAL, 1,0 },
  { "sem",	do_envenom,	POS_RESTING,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE },
  { "sessiz",		do_quiet,	POS_SLEEPING, 	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "set",		do_set,		POS_DEAD,	ML,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "sez",          do_sense,       POS_RESTING,     0,  LOG_NORMAL, 1,0},
  { "shutdow",	do_shutdow,	POS_DEAD,	L1,  LOG_NORMAL, 0, CMD_KEEP_HIDE|CMD_GHOST },
  { "shutdown",	do_shutdown,	POS_DEAD,	L1,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST},
  { "silah",		do_weapon,	POS_FIGHTING,	 0,  LOG_NORMAL, 1,0 },
  { "silahsýzlandýr",		do_disarm,	POS_FIGHTING,	 0,  LOG_NORMAL, 1,0 },
  { "sinir",          do_nerve,       POS_FIGHTING,    0,  LOG_NORMAL, 1,0 },
  { "sývýþ",		do_escape,	POS_FIGHTING,	 0,  LOG_NORMAL,1,0 },
  { "skor",		do_score,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "skrol",		do_scroll,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "sla",		do_sla,		POS_DEAD,	L3,  LOG_NORMAL, 0, CMD_KEEP_HIDE|CMD_GHOST },
  { "slay",		do_slay,	POS_DEAD,	L3,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "slook",		do_slook,	POS_SLEEPING,	 0,  LOG_NORMAL, 1,CMD_KEEP_HIDE|CMD_GHOST},
  { "smite",		do_smite,	POS_DEAD,	L7,  LOG_ALWAYS, 1,0 },
  { "smote",		do_smote,	POS_DEAD,	IM,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "snoop",		do_snoop,	POS_DEAD,	L5,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST},
  { "sockets",	do_sockets,	POS_DEAD,	L4,  LOG_NORMAL, 1 },
  { "sockets",        do_sockets,	POS_DEAD,       L4,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST},
  { "soluþ",		do_fade,	POS_RESTING,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE },
  { "sosyaller",	do_socials,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "söyle",		do_say,		POS_RESTING,	 0,  LOG_NORMAL, 1, CMD_GHOST },
  { "stat",		do_stat,	POS_DEAD,	IM,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "string",		do_string,	POS_DEAD,	L5,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "suflör",		do_prompt,	POS_DEAD,        0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "suikast",    do_assassinate, POS_STANDING,    0,  LOG_NORMAL, 1,0 },
  { "sürükle",		do_drag,	POS_STANDING,	 0,  LOG_NORMAL, 1,0 },
  { "süzül",		do_sneak,	POS_STANDING,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE },
  { "switch",		do_switch,	POS_DEAD,	L6,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "syetenek",		do_slist,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "þifalýeller",	do_layhands,	POS_RESTING,	 0,  LOG_NORMAL, 1,0 },
  { "þifre",	do_password,	POS_DEAD,	 0,  LOG_NEVER,  1, CMD_KEEP_HIDE|CMD_GHOST },
  { "take",		do_get,		POS_RESTING,	 0,  LOG_NORMAL, 1,CMD_KEEP_HIDE },
  { "takip",		do_follow,	POS_RESTING,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE },
  { "takipyok",	do_nofollow,	POS_DEAD,        0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "talep",         do_demand,      POS_STANDING,    0,  LOG_NORMAL, 1,0 },
  { "tamir",		do_repair,	POS_SLEEPING,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE },
  { "taným",	do_description,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "tanýmla",	do_identify,	POS_STANDING,	 0,  LOG_NORMAL, 1, CMD_GHOST },
  { "tara",           do_scan,        POS_RESTING,     0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "tart",	do_consider,	POS_RESTING,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE },
  { "tekme",		do_kick,	POS_FIGHTING,	 0,  LOG_NORMAL, 1,0 },
  { "tekrarla",		do_replay,	POS_SLEEPING,	 0,  LOG_NORMAL, 1, CMD_GHOST },
  { "teleport",	do_transfer,    POS_DEAD,	L7,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "tick",		do_tick,	POS_DEAD,	ML,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "toz",		do_dirt,	POS_FIGHTING,	 0,  LOG_NORMAL, 1,0 },
  { "track",          do_track,       POS_STANDING,    0,  LOG_NORMAL, 0,0 },
  { "transfer",	do_transfer,	POS_DEAD,	L7,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "trust",		do_trust,	POS_DEAD,	ML,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "tut",		do_wear,	POS_RESTING,	 0,  LOG_NORMAL, 1,0 },
  { "tuzak",	do_settraps,	POS_STANDING,	 0,  LOG_NORMAL, 1,0},
  { "typo",		do_typo,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "uç",		do_fly,		POS_FIGHTING,	 0,  LOG_NORMAL, 1,0 },
  { "uysallaþtýr",           do_tame,        POS_FIGHTING,    0,  LOG_NORMAL, 1,0 },
  { "uyu",		do_sleep,	POS_SLEEPING,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE },
  { "vahþileþ",	do_berserk,	POS_FIGHTING,	 0,  LOG_NORMAL, 1,0 },
  { "vampir",	do_vampire,	POS_FIGHTING,    0,  LOG_NORMAL, 1,0 },
  { "ver",		do_give,	POS_RESTING,	 0,  LOG_NORMAL, 1,0 },
  { "violate",	do_violate,	POS_DEAD,	ML,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "vnum",		do_vnum,	POS_DEAD,	L4,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "vur",          do_shoot,       POS_STANDING,    0,  LOG_NORMAL, 1,0 },
  { "wizhelp",	do_wizhelp,	POS_DEAD,	IM,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "wizinvis",	do_invis,	POS_DEAD,	IM,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "wizlock",	do_wizlock,	POS_DEAD,	L2,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "wiznet",		do_wiznet,	POS_DEAD,	IM,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "yaðmayok",		do_noloot,	POS_DEAD,        0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "yap",           do_make,        POS_STANDING,    0,  LOG_NORMAL, 1,0 },
  { "yardým",		do_help,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "yargýla",          do_judge,       POS_RESTING,     0,  LOG_ALWAYS, 1,CMD_KEEP_HIDE },
  { "yarma",         do_cleave,      POS_STANDING,    0,  LOG_NORMAL, 1,0 },
  { "ye",		do_eat,		POS_RESTING,	 0,  LOG_NORMAL, 1,0 },
  { "yeniyaþa",		do_remor,	POS_STANDING,	 0,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "yeniyaþam",		do_remort,	POS_STANDING,	 0,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "yetenekler",		do_skills,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "yokol",		do_vanish,	POS_FIGHTING,    0,  LOG_NORMAL, 1,0 },
  { "yudumla",		do_quaff,	POS_RESTING,	 0,  LOG_NORMAL, 1,0 },
  { "yükselt",	do_enchant, 	POS_RESTING,     0,  LOG_NORMAL, 1,0 },
  { "zaman",		do_time,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { "zar",	do_zar,	POS_STANDING,	 0,  LOG_NORMAL, 1 },
  { "zayi",		do_blink,	POS_FIGHTING,    0,  LOG_NORMAL, 1,CMD_KEEP_HIDE },
  { "zehir",		do_poison_smoke,POS_FIGHTING,	 0,  LOG_ALWAYS, 1,0 },
  { "",		0,		POS_DEAD,	 0,  LOG_NORMAL, 0, CMD_KEEP_HIDE|CMD_GHOST }
};




/*
 * The main entry point for executing commands.
 * Can be recursively called from 'at', 'order', 'force'.
 */
void interpret( CHAR_DATA *ch, char *argument, bool is_order )
{
    char command[MAX_INPUT_LENGTH];
    char logline[MAX_INPUT_LENGTH];
#ifdef IMMORTALS_LOGS
    char buf[MAX_INPUT_LENGTH];
    char *strtime;
#endif
    int cmd;
    int trust;
    bool found;

    /*
     * Strip leading spaces.
     */
    smash_tilde(argument);
    while ( isspace(*argument) )
	argument++;
    if ( argument[0] == '\0' )
	return;

    /*
     * Implement freeze command.
     */
    if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_FREEZE) )
    {
      send_to_char( "{RTamamen donmuþsun!{x\n\r", ch );
	return;
    }

    /*
     * Grab the command word.
     * Special parsing so ' can be a command,
     * also no spaces needed after punctuation.
     */
    strcpy( logline, argument );

#ifdef IMMORTALS_LOGS
    if (IS_IMMORTAL(ch))
	{
	if ( (imm_log = fopen(IMM_LOG_FILE,"a+")) == NULL )
	   {
	    bug("cannot open imm_log_file",0);
	   }
	 else
	 {
	  strtime = (char *) malloc(100);
	  strtime = ctime( &current_time);
	  strtime[strlen(strtime) -1] = '\0';
	  sprintf(buf,"%s :[%s]:%s\n", strtime,ch->name,logline);
	  fprintf(imm_log,buf);
	  fclose(imm_log);
	  free(strtime);
	 }
	}
#endif

    if ( !isalpha(argument[0]) && !isdigit(argument[0]) && !argument[0]=='ý'
&& !argument[0]=='ð' && !argument[0]=='ü' && !argument[0]=='þ'
&& !argument[0]=='ö' && !argument[0]=='ç'&& !argument[0]=='Ý'
&& !argument[0]=='Ð'&& !argument[0]=='Ü'&& !argument[0]=='Þ'&& !argument[0]=='Ö'
&& !argument[0]=='Ç')
    {
	command[0] = argument[0];
	command[1] = '\0';
	argument++;
	while ( isspace(*argument) )
	    argument++;
    }
    else
    {
	argument = one_argument( argument, command );
    }

    /*
     * Look for command in command table.
     */
    found = FALSE;
    trust = get_trust( ch );
    for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
    {
	if ( command[0] == cmd_table[cmd].name[0]
	&&   !str_prefix( command, cmd_table[cmd].name )
	&&   cmd_table[cmd].level <= trust )
	{
         /*
          * Implement charmed mobs commands.
          */
         if ( !is_order && IS_AFFECTED(ch,AFF_CHARM) )
          {
            send_to_char(  "Pek sevgili efendini terketmek istemiyorsun.\n\r", ch );
	   return;
          }

          if ( IS_AFFECTED(ch,AFF_STUN) &&
		!(cmd_table[cmd].extra & CMD_KEEP_HIDE) )
	  {
      send_to_char("Bunu yapamayacak kadar baygýnsýn.\n\r",ch);
	   return;
	  }
          /* Come out of hiding for most commands */
          if ( IS_AFFECTED(ch, AFF_HIDE) && !IS_NPC(ch)
              && !(cmd_table[cmd].extra & CMD_KEEP_HIDE) )
          {
              REMOVE_BIT(ch->affected_by, AFF_HIDE);
              send_to_char("Gölgelerden çýkýyorsun.\n\r", ch);
              act("$n gölgelerden çýkýyor.", ch, NULL, NULL, TO_ROOM);
          }

          if ( IS_AFFECTED(ch, AFF_FADE) && !IS_NPC(ch)
              && !(cmd_table[cmd].extra & CMD_KEEP_HIDE) )
          {
              REMOVE_BIT(ch->affected_by, AFF_FADE);
              send_to_char("Gölgelerden çýkýyorsun.\n\r", ch);
              act("$n gölgelerden çýkýyor.", ch, NULL, NULL, TO_ROOM);
          }

          if ( IS_AFFECTED(ch, AFF_IMP_INVIS) && !IS_NPC(ch)
              && (cmd_table[cmd].position == POS_FIGHTING) )
          {
	      affect_strip(ch,gsn_imp_invis);
              REMOVE_BIT(ch->affected_by, AFF_IMP_INVIS);
              send_to_char("Görünür oluyorsun.\n\r", ch);
              act("$n Görünür oluyor.", ch, NULL, NULL, TO_ROOM);
          }

          if ( CAN_DETECT(ch, ADET_EARTHFADE) && !IS_NPC(ch)
              && (cmd_table[cmd].position == POS_FIGHTING) )
          {
	      affect_strip(ch,gsn_earthfade);
              REMOVE_BIT(ch->detection, ADET_EARTHFADE);
	      WAIT_STATE(ch, (PULSE_VIOLENCE / 2) );
        send_to_char("Doðal formuna dönüyorsun.\n\r", ch);
        act("$n þekilleniyor.", ch, NULL, NULL, TO_ROOM);
          }

          /* prevent ghosts from doing a bunch of commands */
          if (IS_SET(ch->act, PLR_GHOST) && !IS_NPC(ch)
              && !(cmd_table[cmd].extra & CMD_GHOST) )
            continue;

	    found = TRUE;
	    break;
	}
    }

    /*
     * Log and snoop.
     */
    if ( cmd_table[cmd].log == LOG_NEVER )
	strcpy( logline, "" );

    if ( ( ( !IS_NPC(ch) && IS_SET(ch->act, PLR_LOG) )
    ||   fLogAll
    ||   cmd_table[cmd].log == LOG_ALWAYS ) && logline[0] != '\0' &&
	 logline[0] != '\n' )
    {
	sprintf( log_buf, "Log %s: %s", ch->name, logline );
	wiznet(log_buf,ch,NULL,WIZ_SECURE,0,get_trust(ch));
	log_string( log_buf );
    }

    if ( ch->desc != NULL && ch->desc->snoop_by != NULL )
    {
	write_to_buffer( ch->desc->snoop_by, "# ",    2 );
	write_to_buffer( ch->desc->snoop_by, logline, 0 );
	write_to_buffer( ch->desc->snoop_by, "\n\r",  2 );
    }

    if ( !found )
    {
	/*
	 * Look for command in socials table.
	 */
	if ( !check_social( ch, command, argument ) )  {
    send_to_char( "Hý?\n\r", ch );
	    return;
	}
	else
	  return;
    }

    /*
     * Character not in position for command?
     */
    if ( ch->position < cmd_table[cmd].position )
    {
	switch( ch->position )
	{
	case POS_DEAD:
  send_to_char( "Yatmaya devam et; sen ÖLÜsün.\n\r", ch );
	    break;

	case POS_MORTAL:
	case POS_INCAP:
  send_to_char( "Bunu yapamayacak kan kaybettin.\n\r", ch );
	    break;

	case POS_STUNNED:
  send_to_char( "Bunu yapamayacak kadar yaralýsýn.\n\r", ch );
	    break;

	case POS_SLEEPING:
  send_to_char( "Rüyalarýnda mý?\n\r", ch );
	    break;

	case POS_RESTING:
  send_to_char( "Haayýr... Çok rahatlamýþ durumdasýn...\n\r", ch);
	    break;

	case POS_SITTING:
  send_to_char("Önce ayaða kalkmalýsýn.\n\r",ch);
	    break;

	case POS_FIGHTING:
  send_to_char( "Olmaz!  Hala dövüþüyorsun!\n\r", ch);
	    break;

	}
	return;
    }

    /*
     * Dispatch the command.
     */
    (*cmd_table[cmd].do_fun) ( ch, argument );

    tail_chain( );
    return;
}



bool check_social( CHAR_DATA *ch, char *command, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int cmd;
    bool found;
    found  = FALSE;
    for ( cmd = 0; social_table[cmd].name[0] != '\0'; cmd++ )
    {
	if ( command[0] == social_table[cmd].name[0]
	&&   !str_prefix( command, social_table[cmd].name ) )
	{
	    found = TRUE;
	    break;
	}
    }

    if ( !found )
	return FALSE;

    if ( !IS_NPC(ch) && IS_SET(ch->comm, COMM_NOEMOTE) )
    {
      send_to_char("Sen bir antisosyalsin!\n\r", ch );
	return TRUE;
    }

    switch ( ch->position )
    {
    case POS_DEAD:
    send_to_char( "Yatmaya devam et; sen ÖLÜsün.\n\r", ch );
	return TRUE;

    case POS_INCAP:
    case POS_MORTAL:
    send_to_char("Bunu yapamayacak kan kaybettin.\n\r", ch );
	return TRUE;

    case POS_STUNNED:
    send_to_char( "Bunu yapamayacak kadar yaralýsýn.\n\r", ch );
	return TRUE;

    case POS_SLEEPING:
	/*
	 * I just know this is the path to a 12" 'if' statement.  :(
	 * But two players asked for it already!  -- Furey
	 */
	if ( !str_cmp( social_table[cmd].name, "snore" ) )
	    break;
      send_to_char( "Rüyalarýnda mý?\n\r", ch );
	return TRUE;

    }

    if ( IS_AFFECTED( ch, AFF_HIDE )  )  {
      REMOVE_BIT( ch->affected_by, AFF_HIDE );
      send_to_char( "Gölgelerden çýkýyorsun.\n\r", ch);
      act( "$n gölgelerden çýkýyor.", ch, NULL, NULL, TO_ROOM);
    }

    if ( IS_AFFECTED( ch, AFF_FADE )  )  {
      REMOVE_BIT( ch->affected_by, AFF_FADE );
      send_to_char("Gölgelerden çýkýyorsun.\n\r", ch);
      act("$n gölgelerden çýkýyor.", ch, NULL, NULL, TO_ROOM);
    }

    if ( IS_AFFECTED(ch, AFF_IMP_INVIS) && !IS_NPC(ch)
        && (cmd_table[cmd].position == POS_FIGHTING) )
    {
      affect_strip(ch,gsn_imp_invis);
      REMOVE_BIT(ch->affected_by, AFF_IMP_INVIS);
      send_to_char("Görünmeye baþlýyorsun.\n\r", ch);
      act("$n görünmeye baþlýyor.", ch, NULL, NULL, TO_ROOM);
    }

    if ( CAN_DETECT(ch, ADET_EARTHFADE) && !IS_NPC(ch)
        && (cmd_table[cmd].position == POS_FIGHTING) )
    {
      affect_strip(ch,gsn_earthfade);
      REMOVE_BIT(ch->detection, ADET_EARTHFADE);
      WAIT_STATE(ch, (PULSE_VIOLENCE / 2));
      send_to_char("Doðal formuna dönüyorsun.\n\r", ch);
      act("$n þekilleniyor.", ch, NULL, NULL, TO_ROOM);
    }

    one_argument( argument, arg );
    victim = NULL;
    if ( arg[0] == '\0' )
    {
	act( social_table[cmd].others_no_arg, ch, NULL, victim, TO_ROOM    );
	act( social_table[cmd].char_no_arg,   ch, NULL, victim, TO_CHAR    );
    }
    else if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
      send_to_char( "Burada deðil.\n\r", ch );
    }
    else if ( victim == ch )
    {
	act( social_table[cmd].others_auto,   ch, NULL, victim, TO_ROOM    );
	act( social_table[cmd].char_auto,     ch, NULL, victim, TO_CHAR    );
    }
    else
    {
	act( social_table[cmd].others_found,  ch, NULL, victim, TO_NOTVICT );
	act( social_table[cmd].char_found,    ch, NULL, victim, TO_CHAR    );
	act( social_table[cmd].vict_found,    ch, NULL, victim, TO_VICT    );

	if ( !IS_NPC(ch) && IS_NPC(victim)
	&&   !IS_AFFECTED(victim, AFF_CHARM)
	&&   IS_AWAKE(victim)
	&&   victim->desc == NULL)
	{
	    switch ( number_bits( 4 ) )
	    {
	    case 0:

	    case 1: case 2: case 3: case 4:
	    case 5: case 6: case 7: case 8:
		act( social_table[cmd].others_found,
		    victim, NULL, ch, TO_NOTVICT );
		act( social_table[cmd].char_found,
		    victim, NULL, ch, TO_CHAR    );
		act( social_table[cmd].vict_found,
		    victim, NULL, ch, TO_VICT    );
		break;

	    case 9: case 10: case 11: case 12:
      act( "$n $M tokatlýyor.",  victim, NULL, ch, TO_NOTVICT );
  		act( "Sen $M tokatlýyorsun.",  victim, NULL, ch, TO_CHAR    );
  		act( "$n seni tokatlýyor.", victim, NULL, ch, TO_VICT    );
		break;
	    }
	}
    }

    return TRUE;
}



/*
 * Return true if an argument is completely numeric.
 */
bool is_number ( char *arg )
{

    if ( *arg == '\0' )
        return FALSE;

    if ( *arg == '+' || *arg == '-' )
        arg++;

    for ( ; *arg != '\0'; arg++ )
    {
        if ( !isdigit( *arg ) )
            return FALSE;
    }

    return TRUE;
}



/*
 * Given a string like 14.foo, return 14 and 'foo'
 */
int number_argument( char *argument, char *arg )
{
    char *pdot;
    int number;

    for ( pdot = argument; *pdot != '\0'; pdot++ )
    {
	if ( *pdot == '.' )
	{
	    *pdot = '\0';
	    number = atoi( argument );
	    *pdot = '.';
	    strcpy( arg, pdot+1 );
	    return number;
	}
    }

    strcpy( arg, argument );
    return 1;
}

/*
 * Given a string like 14*foo, return 14 and 'foo'
*/
int mult_argument(char *argument, char *arg)
{
    char *pdot;
    int number;

    for ( pdot = argument; *pdot != '\0'; pdot++ )
    {
        if ( *pdot == '*' )
        {
            *pdot = '\0';
            number = atoi( argument );
            *pdot = '*';
            strcpy( arg, pdot+1 );
            return number;
        }
    }

    strcpy( arg, argument );
    return 1;
}



/*
 * Pick off one argument from a string and return the rest.
 * Understands quotes.
 */
char *one_argument( char *argument, char *arg_first )
{
    char cEnd;

    while ( isspace(*argument) )
	argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
	cEnd = *argument++;

    while ( *argument != '\0' )
    {
	if ( *argument == cEnd )
	{
	    argument++;
	    break;
	}
	*arg_first = LOWER(*argument);
	arg_first++;
	argument++;
    }
    *arg_first = '\0';

    while ( isspace(*argument) )
	argument++;

    return argument;
}

/*
 * Contributed by Alander.
 */
void do_commands( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char output[4 * MAX_STRING_LENGTH];
    int letter;
    int cmd;
    int col;

    col = 0;
    output[0] = '\0';

    for(letter = 'a'; letter <= 'z'; letter++)
    {
      for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
      {
        if ( cmd_table[cmd].name[0] == letter
	&&   cmd_table[cmd].level <  LEVEL_HERO
        &&   cmd_table[cmd].level <= get_trust( ch )
	&&   cmd_table[cmd].show)
	{
	    sprintf( buf, "%-12s", cmd_table[cmd].name );
	    strcat( output, buf );
	    if ( ++col % 6 == 0 )
		strcat(output, "\n\r" );
	}
      }
    }

    if ( col % 6 != 0 )
	strcat( output, "\n\r" );

    page_to_char( output, ch );
    return;
}

void do_wizhelp( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char output[4 * MAX_STRING_LENGTH];
    char letter;
    int cmd;
    int col;

    col = 0;
    output[0] = '\0';

    for(letter = 'a'; letter <= 'z'; letter++)
    {
      for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
      {
        if ( cmd_table[cmd].name[0] == letter
	&&   cmd_table[cmd].level >= LEVEL_HERO
        &&   cmd_table[cmd].level <= get_trust( ch )
        &&   cmd_table[cmd].show)
	{
	    sprintf( buf, "%-12s", cmd_table[cmd].name );
	    strcat(output, buf);
	    if ( ++col % 6 == 0 )
		strcat( output, "\n\r");
	}
      }
    }

    if ( col % 6 != 0 )
	strcat(output, "\n\r");

    page_to_char(output, ch);
    return;
}


void do_reture( CHAR_DATA *ch, char *argument)
{
  send_to_char("Tamam.\n\r",ch);
  return;
}

/*********** alias.c **************/

/* does aliasing and other fun stuff */
void substitute_alias(DESCRIPTOR_DATA *d, char *argument)
{
    CHAR_DATA *ch;
    char buf[MAX_STRING_LENGTH],prefix[MAX_INPUT_LENGTH],name[MAX_INPUT_LENGTH];
    char *point;
    int alias;

    ch = d->original ? d->original : d->character;

    /* check for prefix */
    if (ch->prefix[0] != '\0' && str_prefix("prefix",argument))
    {
	if (strlen(ch->prefix) + strlen(argument) > MAX_INPUT_LENGTH)
	    send_to_char("Line to long, prefix not processed.\r\n",ch);
	else
	{
	    sprintf(prefix,"%s %s",ch->prefix,argument);
	    argument = prefix;
	}
    }

    if (IS_NPC(ch) || ch->pcdata->alias[0] == NULL
    ||	!str_prefix("kýsayol",argument) || !str_prefix("kýsayolkaldýr",argument)
    ||  !str_prefix("prefix",argument))
    {
	interpret(d->character,argument, FALSE);
	return;
    }

    strcpy(buf,argument);

    for (alias = 0; alias < MAX_ALIAS; alias++)	 /* go through the aliases */
    {
	if (ch->pcdata->alias[alias] == NULL)
	    break;

	if (!str_prefix(ch->pcdata->alias[alias],argument))
	{
	    point = one_argument(argument,name);
	    if (!strcmp(ch->pcdata->alias[alias],name))
	    {
		buf[0] = '\0';
		strcat(buf,ch->pcdata->alias_sub[alias]);
		strcat(buf," ");
		strcat(buf,point);
		break;
	    }
	    if (strlen(buf) > MAX_INPUT_LENGTH)
	    {
		send_to_char("Alias substitution too long. Truncated.\r\n",ch);
		buf[MAX_INPUT_LENGTH -1] = '\0';
	    }
	}
    }
    interpret(d->character,buf, FALSE);
}

void do_alia(CHAR_DATA *ch, char *argument)
{
  send_to_char("Üzgünüm, KISAYOL komutu tam girilmeli.\n\r",ch);
    return;
}

void do_alias(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *rch;
    char arg[MAX_INPUT_LENGTH],buf[MAX_STRING_LENGTH];
    int pos;

    smash_tilde( argument );

    if (ch->desc == NULL)
	rch = ch;
    else
	rch = ch->desc->original ? ch->desc->original : ch;

    if (IS_NPC(rch))
	return;

    argument = one_argument(argument,arg);


    if (arg[0] == '\0')
    {

	if (rch->pcdata->alias[0] == NULL)
	{
    send_to_char("Tanýmlý kýsayolun yok.\n\r",ch);
	    return;
	}
  send_to_char("Kýsayollarýn:\n\r",ch);

	for (pos = 0; pos < MAX_ALIAS; pos++)
	{
	    if (rch->pcdata->alias[pos] == NULL
	    ||	rch->pcdata->alias_sub[pos] == NULL)
		break;

	    sprintf(buf,"    %s:  %s\n\r",rch->pcdata->alias[pos],
		    rch->pcdata->alias_sub[pos]);
	    send_to_char(buf,ch);
	}
	return;
    }

    if (!str_prefix("kýsayolkaldýr",arg) || !str_cmp("kýsayol",arg))
    {
      send_to_char("Üzgünüm, ayrýlmýþ(reserverd) sözcükler olmaz.\n\r",ch);
	return;
    }

    if (argument[0] == '\0')
    {
	for (pos = 0; pos < MAX_ALIAS; pos++)
	{
	    if (rch->pcdata->alias[pos] == NULL
	    ||	rch->pcdata->alias_sub[pos] == NULL)
		break;

	    if (!str_cmp(arg,rch->pcdata->alias[pos]))
	    {
        sprintf(buf,"%s = '%s'.\n\r",rch->pcdata->alias[pos],
    			rch->pcdata->alias_sub[pos]);
		send_to_char(buf,ch);
		return;
	    }
	}

  send_to_char("Bu kýsayol tanýmlanmamýþ.\n\r",ch);
	return;
    }

    if (!str_prefix(argument,"intihar") || !str_prefix(argument,"prefix"))
    {
      send_to_char("Olmaz!\n\r",ch);
	return;
    }

    for (pos = 0; pos < MAX_ALIAS; pos++)
    {
	if (rch->pcdata->alias[pos] == NULL)
	    break;

	if (!str_cmp(arg,rch->pcdata->alias[pos])) /* redefine an alias */
	{
	    free_string(rch->pcdata->alias_sub[pos]);
	    rch->pcdata->alias_sub[pos] = str_dup(argument);
      sprintf(buf,"%s artýk '%s' demek.\n\r",arg,argument);
	    send_to_char(buf,ch);
	    return;
	}
     }

     if (pos >= MAX_ALIAS)
     {
       send_to_char("Üzgünüm, maksimum kýsayol sayýsýný aþamazsýnýz.\n\r",ch);
	return;
     }

     /* make a new alias */
     rch->pcdata->alias[pos]		= str_dup(arg);
     rch->pcdata->alias_sub[pos]	= str_dup(argument);
     sprintf(buf,"%s artýk '%s' demek.\n\r",arg,argument);
     send_to_char(buf,ch);
}


void do_unalias(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *rch;
    char arg[MAX_INPUT_LENGTH];
    int pos;
    bool found = FALSE;

    if (ch->desc == NULL)
	rch = ch;
    else
	rch = ch->desc->original ? ch->desc->original : ch;

    if (IS_NPC(rch))
	return;

    argument = one_argument(argument,arg);

    if (arg == '\0')
    {
      send_to_char("Hangi kýsayolu kaldýracaksýnýz?\n\r",ch);
	return;
    }

    for (pos = 0; pos < MAX_ALIAS; pos++)
    {
	if (rch->pcdata->alias[pos] == NULL)
	    break;

	if (found)
	{
	    rch->pcdata->alias[pos-1]		= rch->pcdata->alias[pos];
	    rch->pcdata->alias_sub[pos-1]	= rch->pcdata->alias_sub[pos];
	    rch->pcdata->alias[pos]		= NULL;
	    rch->pcdata->alias_sub[pos]		= NULL;
	    continue;
	}

	if(!strcmp(arg,rch->pcdata->alias[pos]))
	{
    send_to_char("Kýsayol kaldýrýldý.\n\r",ch);
	    free_string(rch->pcdata->alias[pos]);
	    free_string(rch->pcdata->alias_sub[pos]);
	    rch->pcdata->alias[pos] = NULL;
	    rch->pcdata->alias_sub[pos] = NULL;
	    found = TRUE;
	}
    }

    if (!found)
    send_to_char("O isimde bir kýsayol yok.\n\r",ch);
}
