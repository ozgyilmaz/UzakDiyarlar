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
#include <ctype.h>
#include <stdio.h>
#include <wchar.h>
#include <wctype.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "interp.h"

#undef IMMORTALS_LOGS

bool	check_social	args( ( CHAR_DATA *ch, wchar_t *command,wchar_t *argument ) );


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
  { L"aşağı",		do_down,	POS_STANDING,	 0,  LOG_NEVER, 0, CMD_KEEP_HIDE|CMD_GHOST },
  { L"batı",		do_west,	POS_STANDING,	 0,  LOG_NEVER, 0, CMD_KEEP_HIDE|CMD_GHOST },
  { L"doğu",		do_east,	POS_STANDING,	 0,  LOG_NEVER, 0, CMD_KEEP_HIDE|CMD_GHOST },
  { L"güney",		do_south,	POS_STANDING,	 0,  LOG_NEVER, 0, CMD_KEEP_HIDE|CMD_GHOST },
  { L"kuzey",		do_north,	POS_STANDING,    0,  LOG_NEVER, 0, CMD_KEEP_HIDE|CMD_GHOST },
  { L"yukarı",		do_up,		POS_STANDING,	 0,  LOG_NEVER, 0, CMD_KEEP_HIDE|CMD_GHOST },
  { L",",		do_emote,	POS_RESTING,	 0,  LOG_NORMAL, 0, CMD_GHOST },
  { L";",		do_gtell,	POS_DEAD,	 0,  LOG_NORMAL, 0, CMD_GHOST },
  { L":",		do_immtalk,	POS_DEAD,	IM,  LOG_NORMAL, 0, CMD_KEEP_HIDE|CMD_GHOST },
  { L"'",		do_say,		POS_RESTING,	 0,  LOG_NORMAL, 0, CMD_GHOST },
  { L"aç",		do_open,	POS_RESTING,	 0,  LOG_NORMAL, 1,CMD_GHOST },
  { L"acıbana",		do_outfit,	POS_RESTING,	 0,  LOG_NORMAL, 1,CMD_GHOST},
  { L"advance",	do_advance,	POS_DEAD,	ML,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"affrooms",	do_affrooms,	POS_DEAD,	L4,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"al",		do_get,		POS_RESTING,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"allow",		do_allow,	POS_DEAD,	L2,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"andaç",         do_trophy,      POS_STANDING,    0,  LOG_NORMAL, 1,0 },
  { L"anımsa",		do_recall,	POS_FIGHTING,	 0,  LOG_NORMAL, 1,CMD_GHOST },
  { L"arakla",		do_steal,	POS_STANDING,	 0,  LOG_NORMAL, 1,0 },
  { L"aranıyor",         do_wanted,      POS_STANDING,    0,  LOG_ALWAYS, 1,0 },
  { L"ardılan",	do_backstab,	POS_STANDING,	 0,  LOG_NORMAL, 1,0 },
  { L"aslançağır",       do_lion_call,   POS_FIGHTING,    0,  LOG_NORMAL, 1,0},
  { L"at",             do_at,          POS_DEAD,       L6,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST  },
  { L"avla",           do_hunt,        POS_STANDING,    0,  LOG_NORMAL,1,0 },
  { L"ayıçağır",       do_bear_call,   POS_FIGHTING,    0,  LOG_NORMAL, 1,0},
  { L"ayrı",		do_qui,		POS_DEAD,	 0,  LOG_NORMAL, 0, CMD_KEEP_HIDE|CMD_GHOST },
  { L"ayrıl",		do_quit,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"bak",		do_look,	POS_RESTING,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"bakiye",	do_balance,	POS_STANDING,	 0,  LOG_NORMAL, 1,0 },
  { L"ban",		do_ban,		POS_DEAD,	L2,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"bayılt",       do_strangle,    POS_STANDING,    0,  LOG_NORMAL, 1,0 },
  { L"bileşik",	do_combine,	POS_DEAD,        0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"biniş",          do_mount,       POS_STANDING,    0,  LOG_NORMAL, 1,0 },
  { L"bitki",          do_herbs,       POS_STANDING,    0,  LOG_NORMAL, 1,0 },
  { L"bırak",		do_drop,	POS_RESTING,	 0,  LOG_NORMAL, 1,CMD_GHOST },
  { L"bölge",		do_areas,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"bug",		do_bug,		POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"büyü",		do_cast,	POS_FIGHTING,	 0,  LOG_NORMAL, 1,0},
  { L"büyüler",		do_spells,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"büyüyıkımı",      do_spellbane,   POS_FIGHTING,    0,  LOG_NORMAL, 0,0 },
  { L"cabal_scan",	do_cabal_scan,	POS_STANDING,    0,  LOG_NEVER, 0, CMD_KEEP_HIDE|CMD_GHOST },
  { L"çağrıyok",	do_nosummon,	POS_DEAD,        0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"çarp",		do_zap,		POS_RESTING,	 0,  LOG_NORMAL, 1,0 },
  { L"çelme",		do_trip,	POS_FIGHTING,    0,  LOG_NORMAL, 1,0 },
  { L"çevrel",         do_circle,      POS_FIGHTING,    0,  LOG_NORMAL, 1,0 },
  { L"çivi",       do_caltraps,    POS_FIGHTING,    0,  LOG_NORMAL, 1,0 },
  { L"çıkar",		do_remove,	POS_RESTING,	 0,  LOG_NORMAL, 1,CMD_GHOST },
  { L"çıkışlar",		do_exits,	POS_RESTING,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"clear",		do_clear,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"clone",		do_clone,	POS_DEAD,	L5,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"cls",		do_clear,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"çöp",           do_sacrifice,   POS_RESTING,     0,  LOG_NORMAL, 0,0 },
  { L"copla",	do_blackjack,	POS_STANDING,	 0,  LOG_NORMAL, 1,0 },
  { L"count",		do_count,	POS_SLEEPING,	HE,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"dağıt",		do_split,	POS_RESTING,	 0,  LOG_NORMAL, 1,0 },
  { L"dayanım",         do_endure,      POS_STANDING,    0,  LOG_NORMAL, 1, CMD_KEEP_HIDE },
  { L"değer",		do_value,	POS_RESTING,	 0,  LOG_NORMAL, 1,0 },
  { L"deny",		do_deny,	POS_DEAD,	L1,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"dilek",	do_dilek,       POS_RESTING,     0,  LOG_NORMAL, 1 , CMD_KEEP_HIDE | CMD_GHOST},
  { L"dinlen",		do_rest,	POS_SLEEPING,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE },
  { L"direnç",     do_resistance,  POS_FIGHTING,    0,  LOG_NORMAL, 0,0 },
  { L"diril",     do_diril,  POS_RESTING,    0,  LOG_NORMAL, 1,CMD_GHOST},
  { L"disconnect",	do_disconnect,	POS_DEAD,	L3,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"doğra",        do_butcher,     POS_STANDING,    0,  LOG_NORMAL, 1,0 },
  { L"dök",		do_pour,	POS_RESTING,	 0,  LOG_NORMAL, 1,CMD_KEEP_HIDE },
  { L"dokun",		do_vtouch,	POS_STANDING,	 0,  LOG_NORMAL, 0,0 },
  { L"doldur",		do_fill,	POS_RESTING,	 0,  LOG_NORMAL, 1,CMD_GHOST },
  { L"dua",           do_pray,        POS_DEAD,        0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"dump",		do_dump,	POS_DEAD,	ML,  LOG_ALWAYS, 0, CMD_KEEP_HIDE|CMD_GHOST },
  { L"duygu",		do_emote,	POS_RESTING,	 0,  LOG_NORMAL, 1, CMD_GHOST },
  { L"duyuru",		do_duyuru,	POS_DEAD,	IM,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"eğitim",		do_train,	POS_RESTING,	 0,  LOG_NORMAL, 1,0 },
  { L"ekipman",	do_equipment,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"emret",		do_order,	POS_RESTING,	 0,  LOG_NORMAL, 1,0},
  { L"envanter",	do_inventory,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"eniyi",	do_eniyi,	POS_RESTING,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"ücret",	do_estimate,	POS_RESTING,	 0,  LOG_NORMAL, 1, CMD_GHOST },
  { L"etkiler",	do_affects_col,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"familya",	do_familya,	POS_STANDING,	 0,  LOG_NORMAL, 1,CMD_GHOST},
  { L"find",		do_find,	POS_DEAD,	ML,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"fırlat",          do_throw,       POS_FIGHTING,    0,  LOG_NORMAL, 1,0 },
  { L"flag",		do_flag,	POS_DEAD,	L4,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"force",		do_force,	POS_DEAD,	L6,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"freeze",		do_freeze,	POS_DEAD,	L7,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"gir", 		do_enter, 	POS_STANDING,	 0,  LOG_NORMAL, 1, CMD_GHOST },
  { L"giy",		do_wear,	POS_RESTING,	 0,  LOG_NORMAL, 1,CMD_GHOST },
  { L"göm",		do_bury,	POS_STANDING,	 0,  LOG_NORMAL, 1,0 },
  { L"görev",          do_quest,       POS_RESTING,     0,  LOG_NORMAL, 1 , CMD_GHOST},
  { L"görün",	do_visible,	POS_SLEEPING,	 0,  LOG_NORMAL, 1,0 },
  { L"göster",		do_show,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"goto",           do_goto,        POS_DEAD,       L8,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"grant",		do_grant,	POS_DEAD,	L2,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"grup",          do_group,       POS_SLEEPING,    0,  LOG_NORMAL, 1, CMD_KEEP_HIDE },
  { L"grupliste",          do_glist,       POS_DEAD,        0,  LOG_NEVER,  1, 0},
  { L"ganlat",		do_gtell,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_GHOST },
  { L"günce",		do_worth,	POS_SLEEPING,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"harakiri",           do_hara,        POS_STANDING,    0,  LOG_NORMAL,1,0 },
  { L"havadurumu",	do_weather,	POS_RESTING,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"haykır",		do_yell,	POS_RESTING,	 0,  LOG_NORMAL, 1, CMD_GHOST },
  { L"hedef",		do_target,	POS_FIGHTING,    0,  LOG_NORMAL, 1,0 },
  { L"holylight",	do_holylight,	POS_DEAD,	IM,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"iç",		do_drink,	POS_RESTING,	 0,  LOG_NORMAL, 1,CMD_GHOST },
  { L"ikikat",	do_ikikat,	POS_DEAD,	L6,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"immtalk",	do_immtalk,	POS_DEAD,	IM,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"incele",	do_examine,	POS_RESTING,	 0,  LOG_NORMAL, 1, CMD_GHOST },
  { L"incognito",	do_incognito,	POS_DEAD,	IM,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"induct",		do_induct,	POS_DEAD,	 0,  LOG_ALWAYS, 1,0 },
  { L"iniş",       do_dismount,    POS_STANDING,    0,  LOG_NORMAL, 1,0 },
  { L"insan",          do_human,       POS_STANDING,    0,  LOG_NORMAL,1,0 },
  { L"intiha",		do_delet,	POS_DEAD,	 0,  LOG_ALWAYS, 0, CMD_KEEP_HIDE|CMD_GHOST },
  { L"intihar",		do_delete,	POS_STANDING,	 0,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"invis",		do_invis,	POS_DEAD,	IM,  LOG_NORMAL, 0, CMD_KEEP_HIDE|CMD_GHOST },
  { L"iptal",	do_nocancel,	POS_DEAD,        0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"irfan",           do_lore,        POS_RESTING,     0,  LOG_NORMAL, 1,0 },
  { L"iste",		do_iste,		POS_RESTING,	 0,  LOG_NORMAL, 1,0 },
  { L"itekle",		do_push,	POS_STANDING,	 0,  LOG_NORMAL, 1,0 },
  { L"ititle",		do_ititle,	POS_DEAD,	IM,  LOG_NORMAL, 1,CMD_KEEP_HIDE|CMD_GHOST },
  { L"iyileştir",		do_heal,	POS_RESTING,	 0,  LOG_NORMAL, 1,0 },
  { L"ısır",		do_vbite,	POS_STANDING,	 0,  LOG_NORMAL, 0,0 },
  { L"kaç",		do_flee,	POS_FIGHTING,	 0,  LOG_NORMAL, 1,0 },
  { L"kalk",		do_stand,	POS_SLEEPING,	 0,  LOG_NORMAL, 1,CMD_GHOST},
  { L"kalk",		do_wake,	POS_SLEEPING,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"kalkan",		do_shield,	POS_FIGHTING,	 0,  LOG_NORMAL, 1,0 },
  { L"kamp",		do_camp,  	POS_STANDING,    0,  LOG_NORMAL, 1,0 },
  { L"kamuflaj",     do_camouflage,  POS_STANDING,    0,  LOG_NORMAL, 1,0 },
  { L"kanallar",       do_channels,    POS_DEAD,        0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST},
  { L"kanasusama",	do_bloodthirst,	POS_FIGHTING,	 0,  LOG_NORMAL, 1,0 },
  { L"kanımsa",	do_crecall,	POS_FIGHTING,	 0,  LOG_NORMAL, 1,0 },
  { L"kapat",		do_close,	POS_RESTING,	 0,  LOG_NORMAL, 1,0 },
  { L"kapıkır",	do_bash_door,	POS_FIGHTING,    0,  LOG_NORMAL, 1,0 },
  { L"kaplan",		do_tiger,	POS_FIGHTING,	 0,  LOG_NORMAL, 1,0 },
  { L"katana",		do_katana, 	POS_STANDING,    0,  LOG_NORMAL, 1,0 },
  { L"katle",		do_murde,	POS_FIGHTING,	 0,  LOG_NORMAL, 0,0 },
  { L"katlet",		do_murder,	POS_FIGHTING,	 0,  LOG_ALWAYS, 1,0 },
  { L"kaydet",		do_save,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_GHOST },
  { L"kaz",		do_dig,  	POS_STANDING,    0,  LOG_NORMAL, 1,0 },
  { L"kazan",		do_gain,	POS_STANDING,	 0,  LOG_NORMAL, 1,0 },
  { L"kd",		do_kd,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_GHOST },
  /*{ L"kdg",	do_kdg,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },*/
  { L"keskingörüş",      do_truesight,   POS_FIGHTING,    0,  LOG_NORMAL, 0,0 },
  { L"kilitaç",         do_unlock,      POS_RESTING,     0,  LOG_NORMAL, 1,CMD_GHOST},
  { L"kilitle",		do_lock,	POS_RESTING,	 0,  LOG_NORMAL, 1,CMD_GHOST },
  { L"kim",		do_who_col,		POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"kimdir",		do_whois,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"kirala",		do_rent,	POS_DEAD,	 0,  LOG_NORMAL, 0,0 },
  { L"kırbaçla",		do_lash,	POS_FIGHTING,    0,  LOG_NORMAL, 1,0 },
  { L"kısayo",		do_alia,	POS_DEAD,	 0,  LOG_NORMAL, 0, CMD_KEEP_HIDE|CMD_GHOST },
  { L"kısayol",		do_alias,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"kısayolkaldır",	do_unalias,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"kıyasla",	do_compare,	POS_RESTING,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"kk",             do_cb,          POS_SLEEPING,    0,  LOG_NORMAL, 1, CMD_GHOST },
  { L"kompakt",	do_compact,	POS_DEAD,        0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"komutlar",	do_commands,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"konsantrasyon",	do_concentrate,	POS_STANDING,	 0,  LOG_NORMAL, 1, 0 },
  { L"kontrol",       	do_control,	POS_STANDING,    0,  LOG_NORMAL, 1,0 },
  { L"korkaklık",		do_wimpy,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"körlüktozu",	do_blindness_dust,POS_FIGHTING,	 0,  LOG_ALWAYS, 1,0 },
  { L"koş",		do_run,		POS_STANDING,	ML,  LOG_NEVER, 0, CMD_KEEP_HIDE|CMD_GHOST },
  { L"koy",		do_put,		POS_RESTING,	 0,  LOG_NORMAL, 1,0 },
  { L"kullan",		do_wear,	POS_RESTING,	 0,  LOG_NORMAL, 1,CMD_GHOST},
  { L"kurban",	do_sacrifice,	POS_RESTING,	 0,  LOG_NORMAL, 1,CMD_GHOST },
  { L"kurtar",		do_rescue,	POS_FIGHTING,	 0,  LOG_NORMAL, 0,0 },
  { L"kuyruk",		do_tail,	POS_FIGHTING,    0,  LOG_NORMAL, 1,0 },
  { L"laka",		do_titl,	POS_DEAD,	 0,  LOG_NORMAL, 0, CMD_KEEP_HIDE|CMD_GHOST },
  { L"lakap",		do_title,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"limited",	do_limited,	POS_DEAD,	L4,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"lisan",		do_speak,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"liste",		do_list,	POS_RESTING,	 0,  LOG_NORMAL, 1,CMD_KEEP_HIDE|CMD_GHOST },
  { L"load",		do_load,	POS_DEAD,	L4,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"log",		do_log,		POS_DEAD,	L1,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST  },
  { L"lookup",		do_slookup,	POS_DEAD,	L2,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"maximum",	do_maximum,	POS_DEAD,	ML,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"maymuncuk",		do_pick,	POS_RESTING,	 0,  LOG_NORMAL, 1,CMD_GHOST },
  { L"memory",		do_memory,	POS_DEAD,	IM,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"mezat",        do_auction,     POS_SLEEPING,    0,  LOG_NORMAL, 1, CMD_GHOST  },
  { L"mobstat",		do_mobstat,	POS_DEAD,	IM,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"mwhere",		do_mwhere,	POS_DEAD,	IM,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"nara",         do_warcry,      POS_FIGHTING,    0,  LOG_NORMAL, 1,0},
  { L"nerede",		do_where,	POS_RESTING,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"newlock",	do_newlock,	POS_DEAD,	L4,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"noaffect",	do_noaffect,	POS_DEAD,	L5,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"nochannels",	do_nochannels,	POS_DEAD,	L5,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"noemote",	do_noemote,	POS_DEAD,	L5,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"noshout",	do_noshout,	POS_DEAD,	L5,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"notell",		do_notell,	POS_DEAD,	L5,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"notitle",	do_notitle,	POS_DEAD,	L7,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"objlist",	do_objlist,	POS_DEAD,	ML,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"odaetki",	do_raffects,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"öğren",		do_learn,	POS_STANDING,	 0,  LOG_NORMAL, 1,0},
  { L"öğret",		do_teach,	POS_STANDING,	 0,  LOG_NORMAL, 1,0},
  { L"oku",		do_recite,	POS_RESTING,	 0,  LOG_NORMAL, 1,0 },
  { L"öldür",		do_kill,	POS_FIGHTING,	 0,  LOG_NORMAL, 1,0},
  { L"omuz",		do_bash,	POS_FIGHTING,    0,  LOG_NORMAL, 1,0 },
  { L"onar",	do_smithing,	POS_RESTING,	 0,  LOG_NORMAL, 1,0},
  { L"otoliste",	do_autolist,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"otoakçe",	do_autoakce,	POS_DEAD,        0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"otoçıkış",	do_autoexit,	POS_DEAD,        0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"otodağıt",	do_autosplit,	POS_DEAD,        0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"otodestek",	do_autoassist,	POS_DEAD,        0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"otokurban",	do_autosac,	POS_DEAD,        0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"otoyağma",	do_autoloot,	POS_DEAD,        0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"otur",		do_sit,		POS_SLEEPING,    0,  LOG_NORMAL, 1,CMD_GHOST},
  { L"owhere",		do_owhere,	POS_DEAD,	IM,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"özet",		do_brief,	POS_DEAD,        0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"paraçek",	do_withdraw,	POS_STANDING,    0,  LOG_NORMAL, 1,CMD_GHOST},
  { L"parayatır",	do_deposit,	POS_STANDING,	 0,  LOG_NORMAL, 1,CMD_GHOST},
  { L"patlat",	do_explode, 	POS_FIGHTING,    0,  LOG_NORMAL, 1,0 },
  { L"peace",		do_peace,	POS_DEAD,	L5,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"pençe",		do_claw,	POS_FIGHTING,	 0,  LOG_NORMAL, 1,0},
  { L"permban",	do_permban,	POS_DEAD,	L1,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"pmote",		do_pmote,	POS_RESTING,	 0,  LOG_NORMAL, 1, CMD_GHOST },
  { L"poofin",		do_bamfin,	POS_DEAD,	L8,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"poofout",	do_bamfout,	POS_DEAD,	L8,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"popularity",	do_popularity,	POS_DEAD,	L2,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"poz",		do_pose,	POS_RESTING,	 0,  LOG_NORMAL, 1, CMD_GHOST },
  //{ L"pracnew",        do_pracnew,	POS_SLEEPING,    ML, LOG_NORMAL, 1, CMD_KEEP_HIDE },
  { L"pratik",       do_practice,	POS_SLEEPING,    0,  LOG_NORMAL, 1, CMD_KEEP_HIDE },
  { L"prefi",		do_prefi,	POS_DEAD,	IM,  LOG_NORMAL, 0, CMD_KEEP_HIDE|CMD_GHOST },
  { L"prefix",		do_prefix,	POS_DEAD,	IM,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"premort",	do_premort,	POS_DEAD,	L8,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"protect",	do_protect,	POS_DEAD,	L1,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"purge",		do_purge,	POS_DEAD,	L4,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"pusu",         do_ambush,      POS_STANDING,    0,  LOG_NORMAL, 1,0 },
  { L"rapor",		do_report,	POS_RESTING,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"read",		do_read,	POS_RESTING,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"reboo",		do_reboo,	POS_DEAD,	L1,  LOG_NORMAL, 0, CMD_KEEP_HIDE|CMD_GHOST },
  { L"reboot",		do_reboot,	POS_DEAD,	L1,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"rename",		do_rename,	POS_DEAD,	L5,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"restore",	do_restore,	POS_DEAD,	L4,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"return",         do_return,      POS_DEAD,       L6,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"rezil",	do_dishonor,	POS_FIGHTING,	 0,  LOG_NORMAL, 1,0 },
  { L"rica",        do_request,     POS_STANDING,    0,  LOG_NORMAL, 1,0 },
  { L"sağır",		do_deaf,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"saklan",		do_hide,	POS_RESTING,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE },
  { L"saldır",		do_kill,	POS_FIGHTING,	 0,  LOG_NORMAL, 0,0},
  { L"sapta",         do_detect_hidden,POS_RESTING,    0,  LOG_NORMAL, 1,0},
  { L"sargı",        do_bandage,     POS_FIGHTING,    0,  LOG_NORMAL, 1,0 },
  { L"sat",		do_sell,	POS_RESTING,	 0,  LOG_NORMAL, 1,CMD_GHOST },
  { L"satınal",		do_buy,		POS_RESTING,	 0,  LOG_NORMAL, 1,CMD_GHOST},
  { L"savun",          do_guard,       POS_STANDING,    0,  LOG_NORMAL, 1,0 },
  { L"savur",	do_brandish,	POS_RESTING,	 0,  LOG_NORMAL, 1,0 },
  { L"sem",	do_envenom,	POS_RESTING,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE },
  { L"sessiz",		do_quiet,	POS_SLEEPING, 	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"set",		do_set,		POS_DEAD,	ML,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"sez",          do_sense,       POS_RESTING,     0,  LOG_NORMAL, 1,0},
  { L"shutdow",	do_shutdow,	POS_DEAD,	L1,  LOG_NORMAL, 0, CMD_KEEP_HIDE|CMD_GHOST },
  { L"shutdown",	do_shutdown,	POS_DEAD,	L1,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST},
  { L"silah",		do_weapon,	POS_FIGHTING,	 0,  LOG_NORMAL, 1,0 },
  { L"silahsızlandır",		do_disarm,	POS_FIGHTING,	 0,  LOG_NORMAL, 1,0 },
  { L"sinir",          do_nerve,       POS_FIGHTING,    0,  LOG_NORMAL, 1,0 },
  { L"sıvış",		do_escape,	POS_FIGHTING,	 0,  LOG_NORMAL,1,0 },
  { L"skor",		do_score,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"skrol",		do_scroll,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"sla",		do_sla,		POS_DEAD,	L3,  LOG_NORMAL, 0, CMD_KEEP_HIDE|CMD_GHOST },
  { L"slay",		do_slay,	POS_DEAD,	L3,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"slook",		do_slook,	POS_SLEEPING,	 0,  LOG_NORMAL, 1,CMD_KEEP_HIDE|CMD_GHOST},
  { L"smite",		do_smite,	POS_DEAD,	L7,  LOG_ALWAYS, 1,0 },
  { L"smote",		do_smote,	POS_DEAD,	IM,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"snoop",		do_snoop,	POS_DEAD,	L5,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST},
  { L"sockets",	do_sockets,	POS_DEAD,	L4,  LOG_NORMAL, 1 },
  { L"sockets",        do_sockets,	POS_DEAD,       L4,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST},
  { L"soluş",		do_fade,	POS_RESTING,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE },
  { L"sosyaller",	do_socials,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"söyle",		do_say,		POS_RESTING,	 0,  LOG_NORMAL, 1, CMD_GHOST },
  { L"stat",		do_stat,	POS_DEAD,	IM,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"string",		do_string,	POS_DEAD,	L5,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"suflör",		do_prompt,	POS_DEAD,        0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"suikast",    do_assassinate, POS_STANDING,    0,  LOG_NORMAL, 1,0 },
  { L"sürükle",		do_drag,	POS_STANDING,	 0,  LOG_NORMAL, 1,0 },
  { L"süzül",		do_sneak,	POS_STANDING,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE },
  { L"switch",		do_switch,	POS_DEAD,	L6,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"syetenek",		do_slist,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"şifalıeller",	do_layhands,	POS_RESTING,	 0,  LOG_NORMAL, 1,0 },
  { L"şifre",	do_password,	POS_DEAD,	 0,  LOG_NEVER,  1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"take",		do_get,		POS_RESTING,	 0,  LOG_NORMAL, 1,CMD_KEEP_HIDE },
  { L"takip",		do_follow,	POS_RESTING,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"takipyok",	do_nofollow,	POS_DEAD,        0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"talep",         do_demand,      POS_STANDING,    0,  LOG_NORMAL, 1,0 },
  { L"tamir",		do_repair,	POS_SLEEPING,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE },
  { L"tanım",	do_description,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"tanımla",	do_identify,	POS_STANDING,	 0,  LOG_NORMAL, 1, CMD_GHOST },
  { L"tara",           do_scan,        POS_RESTING,     0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"tart",	do_consider,	POS_RESTING,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE },
  { L"tekme",		do_kick,	POS_FIGHTING,	 0,  LOG_NORMAL, 1,0 },
  { L"tekrarla",		do_replay,	POS_SLEEPING,	 0,  LOG_NORMAL, 1, CMD_GHOST },
  { L"teleport",	do_transfer,    POS_DEAD,	L7,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"tick",		do_tick,	POS_DEAD,	ML,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"toz",		do_dirt,	POS_FIGHTING,	 0,  LOG_NORMAL, 1,0 },
  { L"track",          do_track,       POS_STANDING,    0,  LOG_NORMAL, 0,0 },
  { L"transfer",	do_transfer,	POS_DEAD,	L7,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"trust",		do_trust,	POS_DEAD,	ML,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"tut",		do_wear,	POS_RESTING,	 0,  LOG_NORMAL, 1,CMD_GHOST},
  { L"tuzak",	do_settraps,	POS_STANDING,	 0,  LOG_NORMAL, 1,0},
  { L"typo",		do_typo,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"uç",		do_fly,		POS_FIGHTING,	 0,  LOG_NORMAL, 1,0 },
  { L"uysallaştır",           do_tame,        POS_FIGHTING,    0,  LOG_NORMAL, 1,0 },
  { L"uyu",		do_sleep,	POS_SLEEPING,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"vahşileş",	do_berserk,	POS_FIGHTING,	 0,  LOG_NORMAL, 1,0 },
  { L"vampir",	do_vampire,	POS_FIGHTING,    0,  LOG_NORMAL, 1,0 },
  { L"ver",		do_give,	POS_RESTING,	 0,  LOG_NORMAL, 1,CMD_GHOST },
  { L"violate",	do_violate,	POS_DEAD,	ML,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"vnum",		do_vnum,	POS_DEAD,	L4,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"vur",          do_shoot,       POS_STANDING,    0,  LOG_NORMAL, 1,0 },
  { L"wizhelp",	do_wizhelp,	POS_DEAD,	IM,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"wizinvis",	do_invis,	POS_DEAD,	IM,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"wizlock",	do_wizlock,	POS_DEAD,	L2,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"wiznet",		do_wiznet,	POS_DEAD,	IM,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"yağmayok",		do_noloot,	POS_DEAD,        0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"yap",           do_make,        POS_STANDING,    0,  LOG_NORMAL, 1,0 },
  { L"yardım",		do_help,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"yargıla",          do_judge,       POS_RESTING,     0,  LOG_ALWAYS, 1,CMD_KEEP_HIDE },
  { L"yarma",         do_cleave,      POS_STANDING,    0,  LOG_NORMAL, 1,0 },
  { L"ye",		do_eat,		POS_RESTING,	 0,  LOG_NORMAL, 1,CMD_GHOST },
  { L"yeniyaşa",		do_remor,	POS_STANDING,	 0,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"yeniyaşam",		do_remort,	POS_STANDING,	 0,  LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"yetenekler",		do_skills,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"yokol",		do_vanish,	POS_FIGHTING,    0,  LOG_NORMAL, 1,0 },
  { L"yudumla",		do_quaff,	POS_RESTING,	 0,  LOG_NORMAL, 1,0 },
  { L"yükselt",	do_enchant, 	POS_RESTING,     0,  LOG_NORMAL, 1,0 },
  { L"zaman",		do_time,	POS_DEAD,	 0,  LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
  { L"zar",	do_zar,	POS_STANDING,	 0,  LOG_NORMAL, 1,CMD_GHOST },
  { L"zayi",		do_blink,	POS_FIGHTING,    0,  LOG_NORMAL, 1,CMD_KEEP_HIDE },
  { L"zehir",		do_poison_smoke,POS_FIGHTING,	 0,  LOG_ALWAYS, 1,0 },
  { L"",		0,		POS_DEAD,	 0,  LOG_NORMAL, 0, CMD_KEEP_HIDE|CMD_GHOST }
};




/*
 * The main entry point for executing commands.
 * Can be recursively called from 'at', 'order', 'force'.
 */
void interpret( CHAR_DATA *ch, wchar_t *argument, bool is_order )
{
    wchar_t command[MAX_INPUT_LENGTH];
    wchar_t logline[MAX_INPUT_LENGTH];
#ifdef IMMORTALS_LOGS
    wchar_t buf[MAX_INPUT_LENGTH];
    wchar_t *strtime;
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
      send_to_char( L"{RTamamen donmuşsun!{x\n\r", ch );
	return;
    }

    /*
     * Grab the command word.
     * Special parsing so ' can be a command,
     * also no spaces needed after punctuation.
     */
    wcscpy( logline, argument );

#ifdef IMMORTALS_LOGS
    if (IS_IMMORTAL(ch))
	{
	if ( (imm_log = fopen(IMM_LOG_FILE,"a+")) == NULL )
	   {
	    bug( L"cannot open imm_log_file",0);
	   }
	 else
	 {
	  strtime = (wchar_t *) malloc(100);
	  strtime = ctime( &current_time);
	  strtime[wcslen(strtime) -1] = '\0';
	  swprintf( buf, MAX_STRING_LENGTH-1, L"%s :[%s]:%s\n", strtime,ch->name,logline);
	  fwprintf(imm_log,buf);
	  fclose(imm_log);
	  free(strtime);
	 }
	}
#endif

    if ( !iswalpha (argument[0]) && !iswdigit(argument[0]) )
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
            send_to_char( L"Pek sevgili efendini terketmek istemiyorsun.\n\r", ch );
	   return;
          }

          if ( IS_AFFECTED(ch,AFF_STUN) &&
		!(cmd_table[cmd].extra & CMD_KEEP_HIDE) )
	  {
      send_to_char( L"Bunu yapamayacak kadar baygınsın.\n\r",ch);
	   return;
	  }
          /* Come out of hiding for most commands */
          if ( IS_AFFECTED(ch, AFF_HIDE) && !IS_NPC(ch)
              && !(cmd_table[cmd].extra & CMD_KEEP_HIDE) )
          {
              REMOVE_BIT(ch->affected_by, AFF_HIDE);
              send_to_char( L"Gölgelerden çıkıyorsun.\n\r", ch);
              act( L"$n gölgelerden çıkıyor.", ch, NULL, NULL, TO_ROOM);
          }

          if ( IS_AFFECTED(ch, AFF_FADE) && !IS_NPC(ch)
              && !(cmd_table[cmd].extra & CMD_KEEP_HIDE) )
          {
              REMOVE_BIT(ch->affected_by, AFF_FADE);
              send_to_char( L"Gölgelerden çıkıyorsun.\n\r", ch);
              act( L"$n gölgelerden çıkıyor.", ch, NULL, NULL, TO_ROOM);
          }

          if ( IS_AFFECTED(ch, AFF_IMP_INVIS) && !IS_NPC(ch)
              && (cmd_table[cmd].position == POS_FIGHTING) )
          {
	      affect_strip(ch,gsn_imp_invis);
              REMOVE_BIT(ch->affected_by, AFF_IMP_INVIS);
              send_to_char( L"Görünür oluyorsun.\n\r", ch);
              act( L"$n Görünür oluyor.", ch, NULL, NULL, TO_ROOM);
          }

          if ( CAN_DETECT(ch, ADET_EARTHFADE) && !IS_NPC(ch)
              && (cmd_table[cmd].position == POS_FIGHTING) )
          {
	      affect_strip(ch,gsn_earthfade);
              REMOVE_BIT(ch->detection, ADET_EARTHFADE);
	      WAIT_STATE(ch, (PULSE_VIOLENCE / 2) );
        send_to_char( L"Doğal formuna dönüyorsun.\n\r", ch);
        act( L"$n şekilleniyor.", ch, NULL, NULL, TO_ROOM);
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
	wcscpy( logline, (wchar_t*)"" );

    if ( ( ( !IS_NPC(ch) && IS_SET(ch->act, PLR_LOG) )
    ||   fLogAll
    ||   cmd_table[cmd].log == LOG_ALWAYS ) && logline[0] != '\0' &&
	 logline[0] != '\n' )
    {
	swprintf( log_buf, (2*MAX_INPUT_LENGTH)-1, L"Log %s: %s", ch->name, logline );
	wiznet(log_buf,ch,NULL,WIZ_SECURE,0,get_trust(ch));
	log_string( log_buf );
    }

    if ( ch->desc != NULL && ch->desc->snoop_by != NULL )
    {
	write_to_buffer( ch->desc->snoop_by, L"# ",    2 );
	write_to_buffer( ch->desc->snoop_by, logline, 0 );
	write_to_buffer( ch->desc->snoop_by, L"\n\r",  2 );
    }

    if ( !found )
    {
	/*
	 * Look for command in socials table.
	 */
	if ( !check_social( ch, command, argument ) )  {
    send_to_char( L"Hı?\n\r", ch );
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
  send_to_char( L"Yatmaya devam et; sen ÖLÜsün.\n\r", ch );
	    break;

	case POS_MORTAL:
	case POS_INCAP:
  send_to_char( L"Bunu yapamayacak kan kaybettin.\n\r", ch );
	    break;

	case POS_STUNNED:
  send_to_char( L"Bunu yapamayacak kadar yaralısın.\n\r", ch );
	    break;

	case POS_SLEEPING:
  send_to_char( L"Rüyalarında mı?\n\r", ch );
	    break;

	case POS_RESTING:
  send_to_char( L"Haayır... Çok rahatlamış durumdasın...\n\r", ch);
	    break;

	case POS_SITTING:
  send_to_char( L"Önce ayağa kalkmalısın.\n\r",ch);
	    break;

	case POS_FIGHTING:
  send_to_char( L"Olmaz!  Hala dövüşüyorsun!\n\r", ch);
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



bool check_social( CHAR_DATA *ch, wchar_t *command, wchar_t *argument )
{
    wchar_t arg[MAX_INPUT_LENGTH];
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
      send_to_char( L"Sen bir antisosyalsin!\n\r", ch );
	return TRUE;
    }

    switch ( ch->position )
    {
    case POS_DEAD:
    send_to_char( L"Yatmaya devam et; sen ÖLÜsün.\n\r", ch );
	return TRUE;

    case POS_INCAP:
    case POS_MORTAL:
    send_to_char( L"Bunu yapamayacak kan kaybettin.\n\r", ch );
	return TRUE;

    case POS_STUNNED:
    send_to_char( L"Bunu yapamayacak kadar yaralısın.\n\r", ch );
	return TRUE;

    case POS_SLEEPING:
	/*
	 * I just know this is the path to a 12" 'if' statement.  :(
	 * But two players asked for it already!  -- Furey
	 */
	if ( !wcscasecmp( social_table[cmd].name, L"snore" ) )
	    break;
      send_to_char( L"Rüyalarında mı?\n\r", ch );
	return TRUE;

    }

    if ( IS_AFFECTED( ch, AFF_HIDE )  )  {
      REMOVE_BIT( ch->affected_by, AFF_HIDE );
      send_to_char( L"Gölgelerden çıkıyorsun.\n\r", ch);
      act( L"$n gölgelerden çıkıyor.", ch, NULL, NULL, TO_ROOM);
    }

    if ( IS_AFFECTED( ch, AFF_FADE )  )  {
      REMOVE_BIT( ch->affected_by, AFF_FADE );
      send_to_char( L"Gölgelerden çıkıyorsun.\n\r", ch);
      act( L"$n gölgelerden çıkıyor.", ch, NULL, NULL, TO_ROOM);
    }

    if ( IS_AFFECTED(ch, AFF_IMP_INVIS) && !IS_NPC(ch)
        && (cmd_table[cmd].position == POS_FIGHTING) )
    {
      affect_strip(ch,gsn_imp_invis);
      REMOVE_BIT(ch->affected_by, AFF_IMP_INVIS);
      send_to_char( L"Görünmeye başlıyorsun.\n\r", ch);
      act( L"$n görünmeye başlıyor.", ch, NULL, NULL, TO_ROOM);
    }

    if ( CAN_DETECT(ch, ADET_EARTHFADE) && !IS_NPC(ch)
        && (cmd_table[cmd].position == POS_FIGHTING) )
    {
      affect_strip(ch,gsn_earthfade);
      REMOVE_BIT(ch->detection, ADET_EARTHFADE);
      WAIT_STATE(ch, (PULSE_VIOLENCE / 2));
      send_to_char( L"Doğal formuna dönüyorsun.\n\r", ch);
      act( L"$n şekilleniyor.", ch, NULL, NULL, TO_ROOM);
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
      send_to_char( L"Burada değil.\n\r", ch );
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
	    switch ( number_range(0,15) )
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
      act( L"$n $M tokatlıyor.",  victim, NULL, ch, TO_NOTVICT );
  		act( L"Sen $M tokatlıyorsun.",  victim, NULL, ch, TO_CHAR    );
  		act( L"$n seni tokatlıyor.", victim, NULL, ch, TO_VICT    );
		break;
	    }
	}
    }

    return TRUE;
}



/*
 * Return true if an argument is completely numeric.
 */
bool is_number ( wchar_t *arg )
{

    if ( *arg == '\0' )
        return FALSE;

    if ( *arg == '+' || *arg == '-' )
        arg++;

    for ( ; *arg != '\0'; arg++ )
    {
        if ( !iswdigit( *arg ) )
            return FALSE;
    }

    return TRUE;
}

/*
 * Return true if an argument is completely numeric.
 */
bool is_number_char ( char *arg )
{

    if ( *arg == '\0' )
        return FALSE;

    if ( *arg == '+' || *arg == '-' )
        arg++;

    for ( ; *arg != '\0'; arg++ )
    {
        if ( !iswdigit( *arg ) )
            return FALSE;
    }

    return TRUE;
}



/*
 * Given a string like 14.foo, return 14 and 'foo'
 */
int number_argument( wchar_t *argument, wchar_t *arg )
{
    wchar_t *pdot;
    int number;

    for ( pdot = argument; *pdot != '\0'; pdot++ )
    {
	if ( *pdot == '.' )
	{
	    *pdot = '\0';
	    number = wcstol( argument, 0, 10 );
	    *pdot = '.';
	    wcscpy( arg, pdot+1 );
	    return number;
	}
    }

    wcscpy( arg, argument );
    return 1;
}

/*
 * Given a string like 14*foo, return 14 and 'foo'
*/
int mult_argument(wchar_t *argument, wchar_t *arg)
{
    wchar_t *pdot;
    int number;

    for ( pdot = argument; *pdot != '\0'; pdot++ )
    {
        if ( *pdot == '*' )
        {
            *pdot = '\0';
            number = wcstol( argument, 0, 10 );
            *pdot = '*';
            wcscpy( arg, pdot+1 );
            return number;
        }
    }

    wcscpy( arg, argument );
    return 1;
}



/*
 * Pick off one argument from a string and return the rest.
 * Understands quotes.
 */
wchar_t *one_argument( wchar_t *argument, wchar_t *arg_first )
{
    wchar_t cEnd;

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
	*arg_first = towlower(*argument);
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
void do_commands( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t buf[MAX_STRING_LENGTH];
    wchar_t output[4 * MAX_STRING_LENGTH];
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
	    swprintf( buf, MAX_STRING_LENGTH-1, L"%-12s", cmd_table[cmd].name );
	    wcscat( output, buf );
	    if ( ++col % 6 == 0 )
		wcscat(output, L"\n\r" );
	}
      }
    }

    if ( col % 6 != 0 )
	wcscat( output, L"\n\r" );

    page_to_char( output, ch );
    return;
}

void do_wizhelp( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t buf[MAX_STRING_LENGTH];
    wchar_t output[4 * MAX_STRING_LENGTH];
    wchar_t letter;
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
	    swprintf( buf, MAX_STRING_LENGTH-1, L"%-12s", cmd_table[cmd].name );
	    wcscat(output, buf);
	    if ( ++col % 6 == 0 )
		wcscat( output, L"\n\r");
	}
      }
    }

    if ( col % 6 != 0 )
	wcscat(output, L"\n\r");

    page_to_char(output, ch);
    return;
}


void do_reture( CHAR_DATA *ch, wchar_t *argument)
{
  send_to_char( L"Tamam.\n\r",ch);
  return;
}

/*********** alias.c **************/

/* does aliasing and other fun stuff */
void substitute_alias(DESCRIPTOR_DATA *d, wchar_t *argument)
{
    CHAR_DATA *ch;
    wchar_t buf[MAX_STRING_LENGTH],prefix[MAX_INPUT_LENGTH],name[MAX_INPUT_LENGTH];
    wchar_t *point;
    int alias;

    ch = d->original ? d->original : d->character;

    /* check for prefix */
    if (ch->prefix[0] != '\0' && str_prefix( L"prefix",argument))
    {
	if (wcslen(ch->prefix) + wcslen(argument) > MAX_INPUT_LENGTH)
	    send_to_char( L"Line to long, prefix not processed.\r\n",ch);
	else
	{
	    swprintf(prefix,MAX_INPUT_LENGTH-1, L"%s %s",ch->prefix,argument);
	    argument = prefix;
	}
    }

    if (IS_NPC(ch) || ch->pcdata->alias[0] == NULL
    ||	!str_prefix( L"kısayol",argument) || !str_prefix( L"kısayolkaldır",argument)
    ||  !str_prefix( L"prefix",argument))
    {
	interpret(d->character,argument, FALSE);
	return;
    }

    wcscpy(buf,argument);

    for (alias = 0; alias < MAX_ALIAS; alias++)	 /* go through the aliases */
    {
	if (ch->pcdata->alias[alias] == NULL)
	    break;

	if (!str_prefix(ch->pcdata->alias[alias],argument))
	{
	    point = one_argument(argument,name);
	    if (!wcscmp(ch->pcdata->alias[alias],name))
	    {
		buf[0] = '\0';
		wcscat(buf,ch->pcdata->alias_sub[alias]);
		wcscat(buf, L" ");
		wcscat(buf,point);
		break;
	    }
	    if (wcslen(buf) > MAX_INPUT_LENGTH)
	    {
		send_to_char( L"Alias substitution too long. Truncated.\r\n",ch);
		buf[MAX_INPUT_LENGTH -1] = '\0';
	    }
	}
    }
    interpret(d->character,buf, FALSE);
}

void do_alia(CHAR_DATA *ch, wchar_t *argument)
{
  send_to_char( L"Üzgünüm, KISAYOL komutu tam girilmeli.\n\r",ch);
    return;
}

void do_alias(CHAR_DATA *ch, wchar_t *argument)
{
    CHAR_DATA *rch;
    wchar_t arg[MAX_INPUT_LENGTH],buf[MAX_STRING_LENGTH];
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
    send_to_char( L"Tanımlı kısayolun yok.\n\r",ch);
	    return;
	}
  send_to_char( L"Kısayolların:\n\r",ch);

	for (pos = 0; pos < MAX_ALIAS; pos++)
	{
	    if (rch->pcdata->alias[pos] == NULL
	    ||	rch->pcdata->alias_sub[pos] == NULL)
		break;

	    swprintf( buf, MAX_STRING_LENGTH-1, L"    %s:  %s\n\r",rch->pcdata->alias[pos],
		    rch->pcdata->alias_sub[pos]);
	    send_to_char(buf,ch);
	}
	return;
    }

    if (!str_prefix( L"kısayolkaldır",arg) || !wcscasecmp( L"kısayol",arg))
    {
      send_to_char( L"Üzgünüm, ayrılmış(reserverd) sözcükler olmaz.\n\r",ch);
	return;
    }

    if (argument[0] == '\0')
    {
	for (pos = 0; pos < MAX_ALIAS; pos++)
	{
	    if (rch->pcdata->alias[pos] == NULL
	    ||	rch->pcdata->alias_sub[pos] == NULL)
		break;

	    if (!wcscasecmp(arg,rch->pcdata->alias[pos]))
	    {
        swprintf( buf, MAX_STRING_LENGTH-1, L"%s = '%s'.\n\r",rch->pcdata->alias[pos],
    			rch->pcdata->alias_sub[pos]);
		send_to_char(buf,ch);
		return;
	    }
	}

  send_to_char( L"Bu kısayol tanımlanmamış.\n\r",ch);
	return;
    }

    if (!str_prefix(argument, L"intihar") || !str_prefix(argument, L"prefix"))
    {
      send_to_char( L"Olmaz!\n\r",ch);
	return;
    }

    for (pos = 0; pos < MAX_ALIAS; pos++)
    {
	if (rch->pcdata->alias[pos] == NULL)
	    break;

	if (!wcscasecmp(arg,rch->pcdata->alias[pos])) /* redefine an alias */
	{
	    free_string(rch->pcdata->alias_sub[pos]);
	    rch->pcdata->alias_sub[pos] = str_dup(argument);
      swprintf( buf, MAX_STRING_LENGTH-1, L"%s artık '%s' demek.\n\r",arg,argument);
	    send_to_char(buf,ch);
	    return;
	}
     }

     if (pos >= MAX_ALIAS)
     {
       send_to_char( L"Üzgünüm, maksimum kısayol sayısını aşamazsınız.\n\r",ch);
	return;
     }

     /* make a new alias */
     rch->pcdata->alias[pos]		= str_dup(arg);
     rch->pcdata->alias_sub[pos]	= str_dup(argument);
     swprintf( buf, MAX_STRING_LENGTH-1, L"%s artık '%s' demek.\n\r",arg,argument);
     send_to_char(buf,ch);
}


void do_unalias(CHAR_DATA *ch, wchar_t *argument)
{
    CHAR_DATA *rch;
    wchar_t arg[MAX_INPUT_LENGTH];
    int pos;
    bool found = FALSE;

    if (ch->desc == NULL)
	rch = ch;
    else
	rch = ch->desc->original ? ch->desc->original : ch;

    if (IS_NPC(rch))
	return;

    argument = one_argument(argument,arg);

    if (arg == NULL)
    {
      send_to_char( L"Hangi kısayolu kaldıracaksınız?\n\r",ch);
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

	if(!wcscmp(arg,rch->pcdata->alias[pos]))
	{
    send_to_char( L"Kısayol kaldırıldı.\n\r",ch);
	    free_string(rch->pcdata->alias[pos]);
	    free_string(rch->pcdata->alias_sub[pos]);
	    rch->pcdata->alias[pos] = NULL;
	    rch->pcdata->alias_sub[pos] = NULL;
	    found = TRUE;
	}
    }

    if (!found)
    send_to_char( L"O isimde bir kısayol yok.\n\r",ch);
}
