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

#include <stdio.h>
#include <wchar.h>
#include <wctype.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include <locale.h>
#include <libexplain/ferror.h>
#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/dir.h>
#endif

#include "merc.h"
#include "db.h"
#include "recycle.h"
#include "lookup.h"
#include "tables.h"

void load_limited_objects();
extern const char *explain_ferror(FILE *fp);


#if !defined(macintosh)
extern	int	_filbuf		args( (FILE *) );
#endif

#if !defined(linux)
long random();
#endif
void srandom(unsigned int);
int getpid();
time_t time(time_t *tloc);


/* externals for counting purposes */
extern	OBJ_DATA	*obj_free;
extern	CHAR_DATA	*char_free;
extern  DESCRIPTOR_DATA *descriptor_free;
extern	PC_DATA		*pcdata_free;
extern  AFFECT_DATA	*affect_free;

/*
 * Globals.
 */
HELP_DATA *		help_first;
HELP_DATA *		help_last;

SHOP_DATA *		shop_first;
SHOP_DATA *		shop_last;

wchar_t			bug_buf		[2*MAX_INPUT_LENGTH];
CHAR_DATA *		char_list;
wchar_t *			help_greeting;
wchar_t			log_buf		[2*MAX_INPUT_LENGTH];
KILL_DATA		kill_table	[MAX_LEVEL];
OBJ_DATA *		object_list;
TIME_INFO_DATA		time_info;
WEATHER_DATA		weather_info;

AUCTION_DATA	*	auction;
ROOM_INDEX_DATA	*	top_affected_room;
int			reboot_counter;
int			time_sync;
int			max_newbies;
int			max_oldies;
int			iNumPlayers;
int     ikikat_tp;
int     ikikat_gp;



/*
 * for limited objects
 */
long                    total_levels;

sh_int			gsn_backstab;
sh_int			gsn_dodge;
sh_int			gsn_envenom;
sh_int			gsn_hide;
sh_int			gsn_peek;
sh_int			gsn_pick_lock;
sh_int			gsn_sneak;
sh_int			gsn_steal;

sh_int			gsn_disarm;
sh_int			gsn_enhanced_damage;
sh_int			gsn_kick;
sh_int			gsn_parry;
sh_int			gsn_rescue;
sh_int			gsn_second_attack;
sh_int			gsn_third_attack;

sh_int			gsn_blindness;
sh_int			gsn_charm_person;
sh_int			gsn_curse;
sh_int			gsn_remove_curse;
sh_int			gsn_invis;
sh_int			gsn_mass_invis;
sh_int			gsn_poison;
sh_int			gsn_plague;
sh_int			gsn_sleep;
sh_int			gsn_sanctuary;
sh_int			gsn_fly;
sh_int			gsn_death_protection;

sh_int  gsn_fourth_attack;
sh_int  gsn_dual_backstab;
sh_int  gsn_cleave;
sh_int  gsn_counter;
sh_int  gsn_ambush;
sh_int  gsn_camouflage;
sh_int  gsn_circle;
sh_int  gsn_nerve;
sh_int  gsn_endure;
sh_int  gsn_quiet_movement;
sh_int  gsn_herbs;
sh_int  gsn_tame;
sh_int  gsn_butcher;
sh_int  gsn_assassinate;
sh_int  gsn_wanted;
sh_int  gsn_caltraps;
sh_int  gsn_throw;
sh_int  gsn_strangle;
sh_int  gsn_blackjack;
sh_int  gsn_bloodthirst;
sh_int  gsn_spellbane;
sh_int  gsn_resistance;
sh_int  gsn_deathblow;
sh_int  gsn_doppelganger;
sh_int  gsn_fade;
sh_int  gsn_garble;
sh_int  gsn_confuse;
sh_int  gsn_mirror;
sh_int  gsn_track;
sh_int  gsn_vanish;
sh_int  gsn_chaos_blade;
sh_int  gsn_terangreal;
sh_int  gsn_dispel_evil;
sh_int  gsn_dispel_good;
sh_int  gsn_wrath;
sh_int  gsn_stalker;
sh_int  gsn_tesseract;
sh_int  gsn_randomizer;
sh_int  gsn_trophy;
sh_int  gsn_truesight;
sh_int  gsn_brew;
sh_int  gsn_flamestrike;
sh_int  gsn_shadowlife;
sh_int  gsn_ruler_badge;
sh_int  gsn_remove_badge;
sh_int  gsn_dragon_strength;
sh_int  gsn_dragon_breath;
sh_int  gsn_warcry;
sh_int  gsn_entangle;
sh_int  gsn_dragonsword;
sh_int  gsn_knight;
sh_int  gsn_guard;
sh_int  gsn_guard_call;
sh_int  gsn_love_potion;
sh_int  gsn_deafen;
sh_int  gsn_protective_shield;
sh_int  gsn_protection_heat_cold;
sh_int  gsn_trance;
sh_int  gsn_demon_summon;
sh_int  gsn_nightwalker;
sh_int  gsn_bear_call;
sh_int  gsn_squire;
sh_int  gsn_lightning_bolt;
sh_int  gsn_disperse;
sh_int  gsn_bless;
sh_int  gsn_weaken;
sh_int  gsn_haste;
sh_int  gsn_cure_critical;
sh_int  gsn_cure_serious;
sh_int  gsn_burning_hands;
sh_int  gsn_acid_blast;
sh_int  gsn_ray_of_truth;
sh_int  gsn_spell_craft;
sh_int  gsn_giant_strength;
sh_int  gsn_explode;
sh_int  gsn_acid_breath;
sh_int  gsn_fire_breath;
sh_int  gsn_frost_breath;
sh_int  gsn_gas_breath;
sh_int  gsn_lightning_breath;
sh_int	gsn_cure_light;
sh_int	gsn_magic_missile;
sh_int	gsn_demonfire;
sh_int	gsn_faerie_fire;
sh_int	gsn_shield;
sh_int	gsn_chill_touch;
sh_int	gsn_second_weapon;
sh_int	gsn_target;
sh_int	gsn_sand_storm;
sh_int	gsn_scream;
sh_int	gsn_enchant_sword;
sh_int	gsn_tiger_power;
sh_int	gsn_lion_call;
sh_int  gsn_hara_kiri;
sh_int  gsn_mouse;
sh_int  gsn_enhanced_armor;
sh_int  gsn_vampire;
sh_int  gsn_vampiric_bite;
sh_int  gsn_light_res;
sh_int  gsn_blink;
sh_int  gsn_hunt;
sh_int  gsn_path_find;
sh_int  gsn_critical;
sh_int  gsn_detect_sneak;
sh_int  gsn_mend;
sh_int  gsn_shielding;
sh_int  gsn_blind_fighting;
sh_int  gsn_riding;
sh_int  gsn_smithing;
sh_int  gsn_swimming;
sh_int  gsn_move_camf;
sh_int  gsn_protection_heat;
sh_int  gsn_protection_cold;
sh_int  gsn_teleport;
sh_int  gsn_witch_curse;
sh_int	gsn_kassandra;
sh_int	gsn_sebat;
sh_int	gsn_matandra;
sh_int  gsn_armor_use;
sh_int	gsn_world_find;
sh_int  gsn_vampiric_touch;
sh_int  gsn_cure_poison;
sh_int  gsn_fire_shield;
sh_int  gsn_fear;
sh_int  gsn_settraps;
sh_int  gsn_mental_attack;
sh_int  gsn_secondary_attack;
sh_int  gsn_mortal_strike;
sh_int  gsn_shield_cleave;
sh_int  gsn_weapon_cleave;
sh_int  gsn_slow;
sh_int  gsn_imp_invis;
sh_int  gsn_camp;
sh_int  gsn_push;
sh_int  gsn_tail;
sh_int	gsn_power_stun;
sh_int	gsn_cabal_recall;
sh_int	gsn_escape;
sh_int	gsn_lay_hands;
sh_int	gsn_grip;
sh_int	gsn_concentrate;
sh_int	gsn_master_sword;
sh_int	gsn_master_hand;
sh_int	gsn_fifth_attack;
sh_int	gsn_area_attack;
sh_int	gsn_reserved;
sh_int	gsn_bandage;
sh_int	gsn_web;
sh_int	gsn_bow;
sh_int	gsn_bash_door;
sh_int	gsn_katana;
sh_int  gsn_bluefire;
sh_int	gsn_crush;
sh_int	gsn_perception;
sh_int	gsn_control_animal;
sh_int	gsn_sanctify_lands;
sh_int	gsn_deadly_venom;
sh_int	gsn_cursed_lands;
sh_int	gsn_lethargic_mist;
sh_int	gsn_black_death;
sh_int	gsn_mysterious_dream;
sh_int	gsn_sense_life;
sh_int	gsn_arrow;
sh_int	gsn_lance;
sh_int	gsn_evil_spirit;
sh_int	gsn_make_arrow;
sh_int	gsn_green_arrow;
sh_int	gsn_red_arrow;
sh_int	gsn_white_arrow;
sh_int	gsn_blue_arrow;
sh_int	gsn_make_bow;
sh_int	gsn_blindness_dust;
sh_int	gsn_poison_smoke;
sh_int	gsn_mastering_spell;
sh_int	gsn_judge;
sh_int  gsn_hand_block;
sh_int  gsn_cross_block;
sh_int	gsn_ground_strike;
sh_int	gsn_neckguard;
sh_int	gsn_lash;
sh_int	gsn_claw;
sh_int	gsn_absorb;
sh_int	gsn_earthfade;
sh_int	gsn_headguard;
sh_int	gsn_blackguard;
sh_int	gsn_mental_knife;

/* new_gsns */

sh_int  		gsn_axe;
sh_int  		gsn_dagger;
sh_int  		gsn_flail;
sh_int  		gsn_mace;
sh_int  		gsn_polearm;
sh_int			gsn_shield_block;
sh_int  		gsn_spear;
sh_int  		gsn_sword;
sh_int  		gsn_whip;

sh_int  		gsn_bash;
sh_int  		gsn_berserk;
sh_int  		gsn_dirt;
sh_int  		gsn_hand_to_hand;
sh_int  		gsn_trip;

sh_int  		gsn_fast_healing;
sh_int  		gsn_haggle;
sh_int  		gsn_lore;
sh_int  		gsn_meditation;

sh_int  		gsn_scrolls;
sh_int  		gsn_staves;
sh_int  		gsn_wands;
sh_int  		gsn_recall;
sh_int  		gsn_detect_hidden;
sh_int  gsn_pass_door;



/*
 * Locals.
 */
MOB_INDEX_DATA *	mob_index_hash		[MAX_KEY_HASH];
OBJ_INDEX_DATA *	obj_index_hash		[MAX_KEY_HASH];
ROOM_INDEX_DATA *	room_index_hash		[MAX_KEY_HASH];
wchar_t *			string_hash		[MAX_KEY_HASH];

AREA_DATA *		area_first;
AREA_DATA *		area_last;

wchar_t *			string_space;
wchar_t *			top_string;
wchar_t			str_empty	[1];

int			top_affect;
int			top_area;
int			top_ed;
int			top_exit;
int			top_help;
int			top_mob_index;
int			top_obj_index;
int			top_reset;
int			top_room;
int			top_shop;
int 			mobile_count = 0;
int			newmobs = 0;
int			newobjs = 0;


/*
 * Memory management.
 * Increase MAX_STRING if you have too.
 * Tune the others only if you understand what you're doing.
 */
#define			MAX_STRING	3500000
#define			MAX_PERM_BLOCK	150000
#define			MAX_MEM_LIST	11

void *			rgFreeList	[MAX_MEM_LIST];
const int		rgSizeList	[MAX_MEM_LIST]	=
{
    16, 32, 64, 128, 256, 1024, 2048, 4096, 8192, 16384, 32768-64
};

int			nAllocString;
int			sAllocString;
int			nAllocPerm;
int			sAllocPerm;



/*
 * Semi-locals.
 */
bool			fBootDb;
FILE *			fpArea;
wchar_t			strArea[MAX_INPUT_LENGTH];
char			strAreaChar[MAX_INPUT_LENGTH];
AREA_DATA		*Serarea;	/* currently read area */


/*
 * Local booting procedures.
*/
void    init_random_number_generator  args( ( void ) );
void	load_areadata	args( ( FILE *fp ) );
void	load_helps	args( ( FILE *fp ) );
void    load_omprogs    args( ( FILE *fp ) );
void 	load_new_mobiles	args( ( FILE *fp ) );
void	load_old_obj	args( ( FILE *fp ) );
void 	load_objects	args( ( FILE *fp ) );
void	load_resets	args( ( FILE *fp ) );
void	load_rooms	args( ( FILE *fp ) );
void	load_shops	args( ( FILE *fp ) );
void 	load_socials	args( ( FILE *fp ) );
void	load_specials	args( ( FILE *fp ) );
void	load_bans	args( ( void ) );
void    load_olimits    args( ( FILE *fp ) );
void    load_practicer  args( ( FILE *fp ) );
void    load_resetmsg   args( ( FILE *fp ) );
void    load_aflag	args( ( FILE *fp ) );

void	fix_exits	args( ( void ) );

void	reset_area	args( ( AREA_DATA * pArea ) );

int	wear_convert	args( ( int oldwear ) );

/*
 * Big mama top level function.
 */
void boot_db( void )
{
    wchar_t buf[MAX_STRING_LENGTH];

    /*
     * Init some data space stuff.
     */
    {
	if ( ( string_space = (wchar_t *)calloc( 1, MAX_STRING ) ) == NULL )
	{
	    bug( L"Boot_db: can't alloc %d string space.", MAX_STRING );
	    exit( 1 );
	}
	top_string	= string_space;
	fBootDb		= TRUE;
    }
	fwprintf( stderr, L"boot_db :: 1\n" );
    /*
     * Init random number generator.
     */
    {
        init_random_number_generator( );
    }
	fwprintf( stderr, L"boot_db :: 2\n" );
    ikikat_tp = 0;
    ikikat_gp = 0;

    /*
     * Set time and weather.
     */
    {
	time_info.year  = 1;
	time_info.month = 1;
	time_info.day   = 1;
	time_info.hour  = 0;
	fwprintf( stderr, L"boot_db :: 3\n" );
	game_time_update();
	fwprintf( stderr, L"boot_db :: 4\n" );
	     if ( time_info.hour <  5 ) weather_info.sunlight = SUN_DARK;
	else if ( time_info.hour <  6 ) weather_info.sunlight = SUN_RISE;
	else if ( time_info.hour < 19 ) weather_info.sunlight = SUN_LIGHT;
	else if ( time_info.hour < 20 ) weather_info.sunlight = SUN_SET;
	else                            weather_info.sunlight = SUN_DARK;
	fwprintf( stderr, L"boot_db :: 5\n" );
	weather_info.change	= 0;
	weather_info.mmhg	= 960;
	if ( time_info.month >= 7 && time_info.month <=12 )
	    weather_info.mmhg += number_range( 1, 50 );
	else
	    weather_info.mmhg += number_range( 1, 80 );
	fwprintf( stderr, L"boot_db :: 6\n" );
	     if ( weather_info.mmhg <=  980 ) weather_info.sky = SKY_LIGHTNING;
	else if ( weather_info.mmhg <= 1000 ) weather_info.sky = SKY_RAINING;
	else if ( weather_info.mmhg <= 1020 ) weather_info.sky = SKY_CLOUDY;
	else                                  weather_info.sky = SKY_CLOUDLESS;

    }

	/* auction */
	fwprintf( stderr, L"boot_db :: 7\n" );
	auction = (AUCTION_DATA *) malloc (sizeof(AUCTION_DATA));
	if (auction == NULL)
	 {
	  bug(L"malloc'ing AUCTION_DATA didn't give %d bytes",sizeof(AUCTION_DATA));
	  exit(1);
	 }

        auction->item = NULL; /* nothing is being sold */
	fwprintf( stderr, L"boot_db :: 8\n" );
	/* room_affect_data */
	top_affected_room = NULL;

	/* reboot counter */
	reboot_counter	= -1;	/* no default reboot */
	time_sync	= 0;	/* time_sync is not set */

	max_newbies	= MAX_NEWBIES;
	max_oldies	= MAX_OLDIES;
	iNumPlayers	= 0;

    /*
     * Assign gsn's for skills which have them.
     */
    {
	int sn;

	for ( sn = 0; sn < MAX_SKILL; sn++ )
	{
	    if ( skill_table[sn].pgsn != NULL )
		*skill_table[sn].pgsn = sn;
	}
    }
	fwprintf( stderr, L"boot_db :: 9\n" );
    /*
     * Read in all the area files.
     */
    {
	FILE *fpList;

	if ( ( fpList = fopen( AREA_LIST, "r" ) ) == NULL )
	{
	    perror( AREA_LIST );
	    exit( 1 );
	}
	fwprintf( stderr, L"boot_db :: 20\n" );
	for ( ; ; )
	{
	    wcscpy( strArea, fread_word( fpList ) );
		fwprintf( stderr, L"boot_db :: %S\n", strArea );
	    if ( strArea[0] == '$' )
		break;

	    if ( strArea[0] == '-' )
	    {
		fpArea = stdin;
	    }
	    else
	    {
		wcstombs(strAreaChar,strArea,wcslen(strArea));
		if ( ( fpArea = fopen( strAreaChar, "r" ) ) == NULL )
		{
		    perror( strAreaChar );
		    exit( 1 );
		}
	    }

	    for ( ; ; )
	    {
		wchar_t *word;

		if ( fread_letter( fpArea ) != '#' )
		{
		    bug( L"Boot_db: # not found.", 0 );
		    exit( 1 );
		}

		word = fread_word( fpArea );
		fwprintf( stderr, L"boot_db :: word :: %S\n", word );
		     if ( word[0] == '$'               )                 break;
    else if ( !wcscasecmp( word, L"AREADATA" ) ) load_areadata(fpArea);
		else if ( !wcscasecmp( word, L"HELPS"    ) ) load_helps   (fpArea);
		else if ( !wcscasecmp( word, L"NEW_MOBILES"  ) ) load_new_mobiles (fpArea);
		else if ( !wcscasecmp( word, L"OBJOLD"   ) ) load_old_obj (fpArea);
	  	else if ( !wcscasecmp( word, L"OBJECTS"  ) ) load_objects (fpArea);
		else if ( !wcscasecmp( word, L"RESETS"   ) ) load_resets  (fpArea);
		else if ( !wcscasecmp( word, L"ROOMS"    ) ) load_rooms   (fpArea);
		else if ( !wcscasecmp( word, L"SHOPS"    ) ) load_shops   (fpArea);
		else if ( !wcscasecmp( word, L"SOCIALS"  ) ) load_socials (fpArea);
		else if ( !wcscasecmp( word, L"OMPROGS"  ) ) load_omprogs (fpArea);
                else if ( !wcscasecmp( word, L"OLIMITS"  ) ) load_olimits (fpArea);
		else if ( !wcscasecmp( word, L"SPECIALS" ) ) load_specials(fpArea);
		else if ( !wcscasecmp( word, L"PRACTICERS" ) ) load_practicer(fpArea);
		else if ( !wcscasecmp( word, L"RESETMESSAGE" ) ) load_resetmsg(fpArea);
		else if ( !wcscasecmp( word, L"FLAG" ) )	 load_aflag(fpArea);
		else
		{
		    bug( L"Boot_db: bad section name.", 0 );
		    exit( 1 );
		}
	    }

	    if ( fpArea != stdin )
		fclose( fpArea );
	    fpArea = NULL;
	}
	fclose( fpList );
	Serarea = NULL;
    }
	fwprintf( stderr, L"boot_db :: 10\n" );
    {
      fix_exits( );
      load_limited_objects();
      swprintf( buf, MAX_STRING_LENGTH-1, L"Total non-immortal levels > 5: %li",total_levels);
      log_string(buf);

      fBootDb	= FALSE;
      area_update( );
      load_bans();
    }
	fwprintf( stderr, L"boot_db :: 11\n" );
    return;
}

// Bolge bilgilerini biraz daha detaylandiralim.

#if defined(KEY)
#undef KEY
#endif

#define KEY( literal, field, value )					\
				if ( !wcscasecmp( word, literal ) )	\
				{					\
				    field  = value;			\
				    fMatch = TRUE;			\
				    break;				\
				}

void load_areadata( FILE *fp )
{
	AREA_DATA *pArea;
    wchar_t *word=(wchar_t*)"End";
	bool fMatch;

	pArea		= (AREA_DATA *)alloc_perm( sizeof(*pArea) );

	pArea->reset_first	= NULL;
    pArea->reset_last	= NULL;
	pArea->age		= 15;
    pArea->nplayer	= 0;
    pArea->empty	= FALSE;
    pArea->count	= 0;
    pArea->resetmsg	= NULL;
    pArea->area_flag	= 0;
	pArea->language	= NULL;
	pArea->translator	= NULL;
	pArea->path	= NULL;
  pArea->yonelim_iyi	= 33;
  pArea->yonelim_yansiz	= 34;
  pArea->yonelim_kem	= 33;

    if ( area_first == NULL )
		area_first = pArea;
    if ( area_last  != NULL )
		area_last->next = pArea;
    area_last	= pArea;
    pArea->next	= NULL;
    Serarea = pArea;
	top_area++;

	while (true)
	{
		word   = (wchar_t *)(feof( fp ) ? L"END" : fread_word( fp ));
		fMatch = FALSE;

		switch ( towupper(word[0]) )
		{
			case 'B':
				KEY( L"Builder",		pArea->writer,		fread_string( fp ) );
			break;
			case 'E':
				if ( !wcscasecmp( word, L"END" ) )
				{
					return;
				}
			break;
			case 'F':
				KEY( L"Filename",		pArea->file_name,		fread_string( fp ) );
			break;
			case 'H':
				KEY( L"Highlevel",		pArea->high_range,		fread_number( fp ) );
			break;
			case 'L':
				KEY( L"Language",		pArea->language,		fread_string( fp ) );
				KEY( L"Lowlevel",		pArea->low_range,		fread_number( fp ) );
			break;
			case 'M':
				KEY( L"Minvnum",		pArea->min_vnum,		fread_number( fp ) );
				KEY( L"Maxvnum",		pArea->max_vnum,		fread_number( fp ) );
			break;
			case 'N':
				KEY( L"Name",		pArea->name,		fread_string( fp ) );
			break;
			case 'P':
				KEY( L"Path",		pArea->path,		fread_string( fp ) );
			break;
			case 'T':
				KEY( L"Translator",		pArea->translator,		fread_string( fp ) );
			break;
      case 'Y':
				KEY( L"Yonelim_iyi",		pArea->yonelim_iyi,		fread_number( fp ) );
        KEY( L"Yonelim_yansiz",		pArea->yonelim_yansiz,		fread_number( fp ) );
        KEY( L"Yonelim_kem",		pArea->yonelim_kem,		fread_number( fp ) );
			break;
		}

		if ( !fMatch )
		{
			bug( L"Fread_areadata: no match.", 0 );
			fread_to_eol( fp );
			return;
		}
	}
}



/*
 * Snarf a help section.
 */
void load_helps( FILE *fp )
{
    HELP_DATA *pHelp;

    for ( ; ; )
    {
	pHelp		= (HELP_DATA *)alloc_perm( sizeof(*pHelp) );
	pHelp->level	= fread_number( fp );
	pHelp->keyword	= fread_string( fp );
	fwprintf( stderr, L"boot_db :: area word :: -%S-\n", pHelp->keyword );
	if ( pHelp->keyword[0] == '$' )
	    break;
	pHelp->text	= fread_string( fp );
	fwprintf( stderr, L"boot_db :: area word :: merhaba :: test\n" );
	if ( !wcscmp( pHelp->keyword, L"merhaba" ) )
	{
		fwprintf( stderr, L"boot_db :: area word :: merhaba bulundu\n" );
	    help_greeting = pHelp->text;
	}

	if ( help_first == NULL )
	    help_first = pHelp;
	if ( help_last  != NULL )
	    help_last->next = pHelp;

	help_last	= pHelp;
	pHelp->next	= NULL;
	top_help++;
    }

    return;
}

/*
 * Snarf an obj section.  old style
 */
void load_old_obj( FILE *fp )
{
    OBJ_INDEX_DATA *pObjIndex;

    for ( ; ; )
    {
	sh_int vnum;
	wchar_t letter;
	int iHash;

	letter				= fread_letter( fp );
	if ( letter != '#' )
	{
	    bug( L"Load_objects: # not found.", 0 );
	    exit( 1 );
	}

	vnum				= fread_number( fp );
	if ( vnum == 0 )
	    break;

	fBootDb = FALSE;
	if ( get_obj_index( vnum ) != NULL )
	{
	    bug( L"Load_objects: vnum %d duplicated.", vnum );
	    exit( 1 );
	}
	fBootDb = TRUE;

	pObjIndex			= (OBJ_INDEX_DATA *)alloc_perm( sizeof(*pObjIndex) );
	pObjIndex->vnum			= vnum;
	pObjIndex->new_format		= FALSE;
	pObjIndex->reset_num	 	= 0;
	pObjIndex->name			= fread_string( fp );
	pObjIndex->short_descr		= fread_string( fp );
	pObjIndex->description		= fread_string( fp );
	/* Action description */	  fread_string( fp );

	pObjIndex->material		= (wchar_t*)"copper";
	pObjIndex->short_descr[0]	= towlower(pObjIndex->short_descr[0]);
	pObjIndex->description[0]	= towupper(pObjIndex->description[0]);
	pObjIndex->material		= str_dup( L"");

	pObjIndex->item_type		= fread_number( fp );
	pObjIndex->extra_flags		= fread_flag( fp );
	pObjIndex->wear_flags		= fread_flag( fp );
	pObjIndex->value[0]		= fread_number( fp );
	pObjIndex->value[1]		= fread_number( fp );
	pObjIndex->value[2]		= fread_number( fp );
	pObjIndex->value[3]		= fread_number( fp );
	pObjIndex->value[4]		= 0;
	pObjIndex->level		= 0;
	pObjIndex->condition 		= 100;
	pObjIndex->weight		= fread_number( fp );
	pObjIndex->cost			= fread_number( fp );	/* Unused */
	/* Cost per day */		  fread_number( fp );
	pObjIndex->limit		= -1;
	pObjIndex->oprogs		= NULL;

	if (pObjIndex->item_type == ITEM_WEAPON)
	{
	    if (is_name((wchar_t*)"two",pObjIndex->name)
	    ||  is_name((wchar_t*)"two-handed",pObjIndex->name)
	    ||  is_name((wchar_t*)"claymore",pObjIndex->name)
      ||  is_name((wchar_t*)"iki-el",pObjIndex->name)
      ||  is_name((wchar_t*)"ikiel",pObjIndex->name)
      ||  is_name((wchar_t*)"çift-el",pObjIndex->name))
		SET_BIT(pObjIndex->value[4],WEAPON_TWO_HANDS);
	}

	for ( ; ; )
	{
	    wchar_t letter;

	    letter = fread_letter( fp );

	    if ( letter == 'A' )
	    {
		AFFECT_DATA *paf;

		paf			= (AFFECT_DATA *)alloc_perm( sizeof(*paf) );
		paf->where		= TO_OBJECT;
		paf->type		= -1;
		paf->level		= 20; /* RT temp fix */
		paf->duration		= -1;
		paf->location		= fread_number( fp );
		paf->modifier		= fread_number( fp );
		paf->bitvector		= 0;
		paf->next		= pObjIndex->affected;
		pObjIndex->affected	= paf;
		top_affect++;
	    }

	    else if ( letter == 'E' )
	    {
		EXTRA_DESCR_DATA *ed;

		ed			= (EXTRA_DESCR_DATA *)alloc_perm( sizeof(*ed) );
		ed->keyword		= fread_string( fp );
		ed->description		= fread_string( fp );
		ed->next		= pObjIndex->extra_descr;
		pObjIndex->extra_descr	= ed;
		top_ed++;
	    }

	    else
	    {
		ungetwc( letter, fp );
		break;
	    }
	}

        /* fix armors */
        if (pObjIndex->item_type == ITEM_ARMOR)
        {
            pObjIndex->value[1] = pObjIndex->value[0];
            pObjIndex->value[2] = pObjIndex->value[1];
        }

	/*
	 * Translate spell "slot numbers" to internal "skill numbers."
	 */
	switch ( pObjIndex->item_type )
	{
	case ITEM_PILL:
	case ITEM_POTION:
	case ITEM_SCROLL:
	    pObjIndex->value[1] = slot_lookup( pObjIndex->value[1] );
	    pObjIndex->value[2] = slot_lookup( pObjIndex->value[2] );
	    pObjIndex->value[3] = slot_lookup( pObjIndex->value[3] );
	    pObjIndex->value[4] = slot_lookup( pObjIndex->value[4] );
	    break;

	case ITEM_STAFF:
	case ITEM_WAND:
	    pObjIndex->value[3] = slot_lookup( pObjIndex->value[3] );
	    break;
	}

	iHash			= vnum % MAX_KEY_HASH;
	pObjIndex->next		= obj_index_hash[iHash];
	obj_index_hash[iHash]	= pObjIndex;
	top_obj_index++;
    }

    return;
}


/*
 * Snarf a reset section.
 */
void load_resets( FILE *fp )
{
    RESET_DATA *pReset;

    if ( area_last == NULL )
    {
	bug( L"Load_resets: no #AREA seen yet.", 0 );
	exit( 1 );
    }

    for ( ; ; )
    {
	ROOM_INDEX_DATA *pRoomIndex;
	EXIT_DATA *pexit;
	wchar_t letter;
	OBJ_INDEX_DATA *temp_index;

	if ( ( letter = fread_letter( fp ) ) == 'S' )
	    break;

	if ( letter == '*' )
	{
	    fread_to_eol( fp );
	    continue;
	}

	pReset		= (RESET_DATA *)alloc_perm( sizeof(*pReset) );
	pReset->command	= letter;
	/* if_flag */	  fread_number( fp );
	pReset->arg1	= fread_number( fp );
	pReset->arg2	= fread_number( fp );
	pReset->arg3	= (letter == 'G' || letter == 'R')
			    ? 0 : fread_number( fp );
	pReset->arg4	= (letter == 'P' || letter == 'M')
			    ? fread_number(fp) : 0;
			  fread_to_eol( fp );

	/*
	 * Validate parameters.
	 * We're calling the index functions for the side effect.
	 */
	switch ( letter )
	{
	default:
	    bug( L"Load_resets: bad command '%c'.", letter );
	    exit( 1 );
	    break;

	case 'M':
	    get_mob_index  ( pReset->arg1 );
	    get_room_index ( pReset->arg3 );
	    break;

	case 'O':
	    temp_index = get_obj_index  ( pReset->arg1 );
	    temp_index->reset_num++;
	    get_room_index ( pReset->arg3 );
	    break;

	case 'P':
	    temp_index = get_obj_index  ( pReset->arg1 );
	    temp_index->reset_num++;
	    get_obj_index  ( pReset->arg3 );
	    break;

	case 'G':
	case 'E':
	    temp_index = get_obj_index  ( pReset->arg1 );
	    temp_index->reset_num++;
	    break;

	case 'D':
	    pRoomIndex = get_room_index( pReset->arg1 );

	    if ( pReset->arg2 < 0
	    ||   pReset->arg2 > 5
	    || ( pexit = pRoomIndex->exit[pReset->arg2] ) == NULL
	    || !IS_SET( pexit->exit_info, EX_ISDOOR ) )
	    {
		bug( L"Load_resets: 'D': exit %d not door.", pReset->arg2 );
		exit( 1 );
	    }

	    if ( pReset->arg3 < 0 || pReset->arg3 > 2 )
	    {
		bug( L"Load_resets: 'D': bad 'locks': %d.", pReset->arg3 );
		exit( 1 );
	    }

	    break;

	case 'R':
	    pRoomIndex		= get_room_index( pReset->arg1 );

	    if ( pReset->arg2 < 0 || pReset->arg2 > 6 )
	    {
		bug( L"Load_resets: 'R': bad exit %d.", pReset->arg2 );
		exit( 1 );
	    }

	    break;
	}

	if ( area_last->reset_first == NULL )
	    area_last->reset_first	= pReset;
	if ( area_last->reset_last  != NULL )
	    area_last->reset_last->next	= pReset;

	area_last->reset_last	= pReset;
	pReset->next		= NULL;
	top_reset++;
    }

    return;
}



/*
 * Snarf a room section.
 */
void load_rooms( FILE *fp )
{
    ROOM_INDEX_DATA *pRoomIndex;

    if ( area_last == NULL )
    {
	bug( L"Load_resets: no #AREA seen yet.", 0 );
	exit( 1 );
    }

    for ( ; ; )
    {
	sh_int vnum;
	wchar_t letter;
	int door;
	int iHash;

	letter				= fread_letter( fp );
	if ( letter != '#' )
	{
	    bug( L"Load_rooms: # not found.", 0 );
	    exit( 1 );
	}

	vnum				= fread_number( fp );
	if ( vnum == 0 )
	    break;

	fBootDb = FALSE;
	if ( get_room_index( vnum ) != NULL )
	{
	    bug( L"Load_rooms: vnum %d duplicated.", vnum );
	    exit( 1 );
	}
	fBootDb = TRUE;

	pRoomIndex			= (ROOM_INDEX_DATA *)alloc_perm( sizeof(*pRoomIndex) );
	pRoomIndex->owner		= str_dup( L"");
	pRoomIndex->people		= NULL;
	pRoomIndex->contents		= NULL;
	pRoomIndex->extra_descr		= NULL;
        pRoomIndex->history             = NULL;
	pRoomIndex->area		= area_last;
	pRoomIndex->vnum		= vnum;
	pRoomIndex->name		= fread_string( fp );
	pRoomIndex->description		= fread_string( fp );
	/* Area number */		  fread_number( fp );
	pRoomIndex->room_flags		= fread_flag( fp );

  	if ( 3000 <= vnum && vnum < 3400)
	   SET_BIT(pRoomIndex->room_flags,ROOM_LAW);

	pRoomIndex->sector_type		= fread_number( fp );
	pRoomIndex->light		= 0;
	for ( door = 0; door <= 5; door++ )
	    pRoomIndex->exit[door] = NULL;

	/* defaults */
	pRoomIndex->heal_rate = 100;
	pRoomIndex->mana_rate = 100;
	pRoomIndex->affected = NULL;
	pRoomIndex->affected_by = 0;
	pRoomIndex->aff_next = NULL;

	for ( ; ; )
	{
	    letter = fread_letter( fp );

	    if ( letter == 'S' )
		break;

	    if ( letter == 'H') /* healing room */
		pRoomIndex->heal_rate = fread_number(fp);

	    else if ( letter == 'M') /* mana room */
		pRoomIndex->mana_rate = fread_number(fp);

	    else if ( letter == 'D' )
	    {
		EXIT_DATA *pexit;
		int locks;

		door = fread_number( fp );
		if ( door < 0 || door > 5 )
		{
		    bug( L"Fread_rooms: vnum %d has bad door number.", vnum );
		    exit( 1 );
		}

		pexit			= (EXIT_DATA *)alloc_perm( sizeof(*pexit) );
		pexit->description	= fread_string( fp );
		pexit->keyword		= fread_string( fp );
		pexit->exit_info	= 0;
		locks			= fread_number( fp );
		pexit->key		= fread_number( fp );
		pexit->u1.vnum		= fread_number( fp );

		switch ( locks )
		{
		case 1: pexit->exit_info = EX_ISDOOR;                break;
		case 2: pexit->exit_info = EX_ISDOOR | EX_PICKPROOF; break;
		case 3: pexit->exit_info = EX_ISDOOR | EX_NOPASS;    break;
		case 4: pexit->exit_info = EX_ISDOOR|EX_NOPASS|EX_PICKPROOF;
			break;
		case 5: pexit->exit_info = EX_NOFLEE;                break;
		}

		pRoomIndex->exit[door]	= pexit;
		pRoomIndex->old_exit[door] = pexit;
		top_exit++;
	    }
	    else if ( letter == 'E' )
	    {
		EXTRA_DESCR_DATA *ed;

		ed			= (EXTRA_DESCR_DATA *)alloc_perm( sizeof(*ed) );
		ed->keyword		= fread_string( fp );
		ed->description		= fread_string( fp );
		ed->next		= pRoomIndex->extra_descr;
		pRoomIndex->extra_descr	= ed;
		top_ed++;
	    }

	    else if (letter == 'O')
	    {
		if (pRoomIndex->owner[0] != '\0')
		{
		    bug( L"Load_rooms: duplicate owner.",0);
		    exit(1);
		}

		pRoomIndex->owner = fread_string(fp);
	    }

	    else
	    {
		bug( L"Load_rooms: vnum %d has flag not 'DES'.", vnum );
		exit( 1 );
	    }
	}

	iHash			= vnum % MAX_KEY_HASH;
	pRoomIndex->next	= room_index_hash[iHash];
	room_index_hash[iHash]	= pRoomIndex;
	top_room++;
    }

    return;
}



/*
 * Snarf a shop section.
 */
void load_shops( FILE *fp )
{
    SHOP_DATA *pShop;

    for ( ; ; )
    {
	MOB_INDEX_DATA *pMobIndex;
	int iTrade;

	pShop			= (SHOP_DATA *)alloc_perm( sizeof(*pShop) );
	pShop->keeper		= fread_number( fp );
	if ( pShop->keeper == 0 )
	    break;
	for ( iTrade = 0; iTrade < MAX_TRADE; iTrade++ )
	    pShop->buy_type[iTrade]	= fread_number( fp );
	pShop->profit_buy	= fread_number( fp );
	pShop->profit_sell	= fread_number( fp );
	pShop->open_hour	= fread_number( fp );
	pShop->close_hour	= fread_number( fp );
				  fread_to_eol( fp );
	pMobIndex		= get_mob_index( pShop->keeper );
	pMobIndex->pShop	= pShop;

	if ( shop_first == NULL )
	    shop_first = pShop;
	if ( shop_last  != NULL )
	    shop_last->next = pShop;

	shop_last	= pShop;
	pShop->next	= NULL;
	top_shop++;
    }

    return;
}


/*
 * Snarf spec proc declarations.
 */
void load_specials( FILE *fp )
{
    for ( ; ; )
    {
	MOB_INDEX_DATA *pMobIndex;
	wchar_t letter;

	switch ( letter = fread_letter( fp ) )
	{
	default:
	    bug( L"Load_specials: letter '%c' not *MS.", letter );
	    exit( 1 );

	case 'S':
	    return;

	case '*':
	    break;

	case 'M':
	    pMobIndex		= get_mob_index	( fread_number ( fp ) );
	    pMobIndex->spec_fun	= spec_lookup	( fread_word   ( fp ) );
	    if ( pMobIndex->spec_fun == 0 )
	    {
		bug( L"Load_specials: 'M': vnum %d.", pMobIndex->vnum );
		exit( 1 );
	    }
	    break;
	}

	fread_to_eol( fp );
    }
}


/*
 * Translate all room exits from virtual to real.
 * Has to be done after all rooms are read in.
 * Check for bad reverse exits.
 */
void fix_exits( void )
{
    extern const sh_int rev_dir [];
    wchar_t buf[MAX_STRING_LENGTH];
    ROOM_INDEX_DATA *pRoomIndex;
    ROOM_INDEX_DATA *to_room;
    EXIT_DATA *pexit;
    EXIT_DATA *pexit_rev;
    int iHash;
    int door;

    for ( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
    {
	for ( pRoomIndex  = room_index_hash[iHash];
	      pRoomIndex != NULL;
	      pRoomIndex  = pRoomIndex->next )
	{
	    bool fexit;

	    fexit = FALSE;
	    for ( door = 0; door <= 5; door++ )
	    {
		if ( ( pexit = pRoomIndex->exit[door] ) != NULL )
		{
		    if ( pexit->u1.vnum <= 0
		    || get_room_index(pexit->u1.vnum) == NULL)
			pexit->u1.to_room = NULL;
		    else
		    {
		   	fexit = TRUE;
			pexit->u1.to_room = get_room_index( pexit->u1.vnum );
		    }
		}
	    }
	    if (!fexit)
		SET_BIT(pRoomIndex->room_flags,ROOM_NO_MOB);
	}
    }

    for ( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
    {
	for ( pRoomIndex  = room_index_hash[iHash];
	      pRoomIndex != NULL;
	      pRoomIndex  = pRoomIndex->next )
	{
	    for ( door = 0; door <= 5; door++ )
	    {
		if ( ( pexit     = pRoomIndex->exit[door]       ) != NULL
		&&   ( to_room   = pexit->u1.to_room            ) != NULL
		&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != NULL
		&&   pexit_rev->u1.to_room != pRoomIndex
		&&   (pRoomIndex->vnum < 1200 || pRoomIndex->vnum > 1299))
		{
		    swprintf( buf, MAX_STRING_LENGTH-1, L"Fix_exits: %d:%d -> %d:%d -> %d.",
			pRoomIndex->vnum, door,
			to_room->vnum,    rev_dir[door],
			(pexit_rev->u1.to_room == NULL)
			    ? 0 : pexit_rev->u1.to_room->vnum );
		    bug( buf, 0 );
		}
	    }
	}
    }

    return;
}



/*
 * Repopulate areas periodically.
 */
void area_update( void )
{
    AREA_DATA *pArea;
    DESCRIPTOR_DATA *d;
    wchar_t buf[MAX_STRING_LENGTH];

    for ( pArea = area_first; pArea != NULL; pArea = pArea->next )
    {

	if ( ++pArea->age < 3 )
	    continue;

	/*
	 * Check age and reset.
	 * Note: Mud School resets every 3 minutes (not 15).
	 */
	if ( (!pArea->empty && (pArea->nplayer == 0 || pArea->age >= 15))
	||    pArea->age >= 31)
	{
	    ROOM_INDEX_DATA *pRoomIndex;

	    reset_area( pArea );
      swprintf( buf, MAX_STRING_LENGTH-1, L"%s bölgesi reset'lendi.",pArea->name);
	    wiznet(buf,NULL,NULL,WIZ_RESETS,0,0);

	    if (pArea->resetmsg)
	    	 swprintf( buf, MAX_STRING_LENGTH-1, L"%s\n\r",pArea->resetmsg);
	    else
      swprintf( buf, MAX_STRING_LENGTH-1, L"Yaşamın yeni seslerini duyuyorsun...\n\r");

            for ( d = descriptor_list; d != NULL; d = d->next )
	       {
             	 if ( d->connected == CON_PLAYING
              	    &&   IS_AWAKE(d->character )
		    &&   d->character->in_room )
		    if ( d->character->in_room->area == pArea )
			send_to_char( buf, d->character );
	       }

	    pArea->age = number_range( 0, 3 );
	    pRoomIndex = get_room_index( 200 );
	    if ( pRoomIndex != NULL && pArea == pRoomIndex->area )
		pArea->age = 15 - 2;
	    pRoomIndex = get_room_index( 210 );
	    if ( pRoomIndex != NULL && pArea == pRoomIndex->area )
		pArea->age = 15 - 2;
	    pRoomIndex = get_room_index( 220 );
	    if ( pRoomIndex != NULL && pArea == pRoomIndex->area )
		pArea->age = 15 - 2;
	    pRoomIndex = get_room_index( 230 );
	    if ( pRoomIndex != NULL && pArea == pRoomIndex->area )
		pArea->age = 15 - 2;
	    pRoomIndex = get_room_index( ROOM_VNUM_SCHOOL );
	    if ( pRoomIndex != NULL && pArea == pRoomIndex->area )
		pArea->age = 15 - 2;
	    else if (pArea->nplayer == 0)
		pArea->empty = TRUE;
	}
    }

    return;
}



/*
 * Reset one area.
 */
void reset_area( AREA_DATA *pArea )
{
  RESET_DATA *pReset;
  CHAR_DATA *mob;
  bool last;
  int level;
  int i;
  ROOM_INDEX_DATA *room;
  DESCRIPTOR_DATA *d;
  CHAR_DATA *ch;

  if ( weather_info.sky == SKY_RAINING )
  {
    for ( d = descriptor_list; d!=NULL; d=d->next)
    {
      if ( d->connected != CON_PLAYING )
      {
        continue;
      }
      ch = ( d->original != NULL ) ? d->original : d->character;
      if ( (ch->in_room->area == pArea) &&
        ( get_skill(ch, gsn_track)>50) &&
        ( !IS_SET(ch->in_room->room_flags, ROOM_INDOORS) ) )
      {
        send_to_char( L"Yağmur izleri temizliyor.\n\r", ch );
      }
    }
    for (i=pArea->min_vnum; i<pArea->max_vnum; i++)
    {
      room = get_room_index(i);
      if (room == NULL)
      {
        continue;
      }
      if (IS_SET(room->room_flags, ROOM_INDOORS))
      {
        continue;
      }
      room_record( (wchar_t*)"erased", room, -1 );
      if (number_percent() < 50)
      {
        room_record( (wchar_t*)"erased", room, -1 );
      }
    }
  }
    mob 	= NULL;
    last	= TRUE;
    level	= 0;
    for ( pReset = pArea->reset_first; pReset != NULL; pReset = pReset->next )
    {
	ROOM_INDEX_DATA *pRoomIndex;
	MOB_INDEX_DATA *pMobIndex;
	OBJ_INDEX_DATA *pObjIndex;
	OBJ_INDEX_DATA *pObjToIndex;
	OBJ_INDEX_DATA *cabal_item;
	EXIT_DATA *pexit;
	OBJ_DATA *obj;
	OBJ_DATA *obj_to;
	int count, limit, ci_vnum=0;

	switch ( pReset->command )
	{
	default:
	    bug( L"Reset_area: bad command %c.", pReset->command );
	    break;

	case 'M':
	    if ( ( pMobIndex = get_mob_index( pReset->arg1 ) ) == NULL )
	    {
		bug( L"Reset_area: 'M': bad vnum %d.", pReset->arg1 );
		continue;
	    }

	    if ( ( pRoomIndex = get_room_index( pReset->arg3 ) ) == NULL )
	    {
		bug( L"Reset_area: 'R': bad vnum %d.", pReset->arg3 );
		continue;
	    }

	    if ( pMobIndex->count >= pReset->arg2 )
	    {
		last = FALSE;
		break;
	    }

	    count = 0;
	    for (mob = pRoomIndex->people; mob != NULL; mob = mob->next_in_room)
		if (mob->pIndexData == pMobIndex)
		{
		    count++;
		    if (count >= pReset->arg4)
		    {
		    	last = FALSE;
		    	break;
		    }
		}

	    if (count >= pReset->arg4)
		break;

	    mob = create_mobile( pMobIndex , pRoomIndex->area);

	    /*
	     * Check for pet shop.
	     */
	    {
		ROOM_INDEX_DATA *pRoomIndexPrev;
		pRoomIndexPrev = get_room_index( pRoomIndex->vnum - 1 );
		if ( pRoomIndexPrev != NULL
		&&   IS_SET(pRoomIndexPrev->room_flags, ROOM_PET_SHOP) )
		    SET_BIT(mob->act, ACT_PET);
	    }

	    /* set area */
	    mob->zone = pRoomIndex->area;

	    char_to_room( mob, pRoomIndex );
	    level = URANGE( 0, mob->level - 2, LEVEL_HERO - 1 );
	    last  = TRUE;
	    break;

	case 'O':
	    if ( ( pObjIndex = get_obj_index( pReset->arg1 ) ) == NULL )
	    {
		bug( L"Reset_area: 'O': bad vnum %d.", pReset->arg1 );
		continue;
	    }

	    if ( ( pRoomIndex = get_room_index( pReset->arg3 ) ) == NULL )
	    {
		bug( L"Reset_area: 'R': bad vnum %d.", pReset->arg3 );
		continue;
	    }

	    if ( pArea->nplayer > 0
	    ||   count_obj_list( pObjIndex, pRoomIndex->contents ) > 0 )
	    {
		last = FALSE;
		break;
	    }

	    switch( pObjIndex->vnum )
	    {
		case OBJ_VNUM_RULER_STAND:
		  ci_vnum = cabal_table[CABAL_RULER].obj_vnum;
		  break;
		case OBJ_VNUM_INVADER_SKULL:
		  ci_vnum = cabal_table[CABAL_INVADER].obj_vnum;
		  break;
		case OBJ_VNUM_SHALAFI_ALTAR:
		  ci_vnum = cabal_table[CABAL_SHALAFI].obj_vnum;
		  break;
		case OBJ_VNUM_CHAOS_ALTAR:
		  ci_vnum = cabal_table[CABAL_CHAOS].obj_vnum;
		  break;
		case OBJ_VNUM_KNIGHT_ALTAR:
		  ci_vnum = cabal_table[CABAL_KNIGHT].obj_vnum;
		  break;
		case OBJ_VNUM_LIONS_ALTAR:
		  ci_vnum = cabal_table[CABAL_LIONS].obj_vnum;
		  break;
		case OBJ_VNUM_BATTLE_THRONE:
		  ci_vnum = cabal_table[CABAL_BATTLE].obj_vnum;
		  break;
		case OBJ_VNUM_HUNTER_ALTAR:
		  ci_vnum = cabal_table[CABAL_HUNTER].obj_vnum;
		  break;
	    }

	    cabal_item = get_obj_index( ci_vnum );
	    if ( ci_vnum &&  cabal_item->count > 0 )
	     {
		last = FALSE;
		break;
	     }

            if ( ( pObjIndex->limit != -1 ) &&
                 ( pObjIndex->count >= pObjIndex->limit ) )
              {
                last = FALSE;
                break;
              }

	    obj       = create_object( pObjIndex, UMIN(number_fuzzy(level),
						       LEVEL_HERO - 1) );
	    obj->cost = 0;
	    obj_to_room( obj, pRoomIndex );
	    last = TRUE;
	    break;

	case 'P':
	    if ( ( pObjIndex = get_obj_index( pReset->arg1 ) ) == NULL )
	    {
		bug( L"Reset_area: 'P': bad vnum %d.", pReset->arg1 );
		continue;
	    }

	    if ( ( pObjToIndex = get_obj_index( pReset->arg3 ) ) == NULL )
	    {
		bug( L"Reset_area: 'P': bad vnum %d.", pReset->arg3 );
		continue;
	    }

            if (pReset->arg2 > 50) 		/* old format */
                limit = 6;
            else if (pReset->arg2 == -1) 	/* no limit */
                limit = 999;
            else
                limit = pReset->arg2;

	    if (pArea->nplayer > 0
	    || (obj_to = get_obj_type( pObjToIndex ) ) == NULL
	    || (obj_to->in_room == NULL && !last)
	    || ( pObjIndex->count >= limit && number_range(0,4) != 0)
	    || (count = count_obj_list(pObjIndex,obj_to->contains))
		> pReset->arg4 )
	    {
		last = FALSE;
		break;
	    }

            if ( ( pObjIndex->limit != -1               ) &&
                 ( pObjIndex->count >= pObjIndex->limit ) )
              {
                last = FALSE;
                dump_to_scr((wchar_t*)"Reseting area: [P] OBJ limit reached\n\r");
                break;
              }

	    while (count < pReset->arg4)
	    {
	        obj = create_object( pObjIndex, number_fuzzy(obj_to->level) );
	    	obj_to_obj( obj, obj_to );
		count++;
		if (pObjIndex->count >= limit)
		    break;
	    }
	    /* fix object lock state! */
	    obj_to->value[1] = obj_to->pIndexData->value[1];
	    last = TRUE;
	    break;

	case 'G':
	case 'E':
	    if ( ( pObjIndex = get_obj_index( pReset->arg1 ) ) == NULL )
	    {
		bug( L"Reset_area: 'E' or 'G': bad vnum %d.", pReset->arg1 );
		continue;
	    }

	    if ( !last )
		break;

	    if ( mob == NULL )
	    {
		bug( L"Reset_area: 'E' or 'G': null mob for vnum %d.",
		    pReset->arg1 );
		last = FALSE;
		break;
	    }

	    if ( mob->pIndexData->pShop != NULL )
	    {
		int olevel = 0,i,j;

		if (!pObjIndex->new_format)
		    switch ( pObjIndex->item_type )
		{
		case ITEM_PILL:
		case ITEM_POTION:
		case ITEM_SCROLL:
		    olevel = MAX_LEVEL - 7;
		    for (i = 1; i < 5; i++)
		    {
			if (pObjIndex->value[i] > 0)
			{
		    	    for (j = 0; j < MAX_CLASS; j++)
			    {
				olevel = UMIN(olevel,
				         skill_table[pObjIndex->value[i]].
						     skill_level[j]);
			    }
			}
		    }

		    olevel = UMAX(0,(olevel * 3 / 4) - 2);
		    break;
		case ITEM_WAND:		olevel = number_range( 10, 20 ); break;
		case ITEM_STAFF:	olevel = number_range( 15, 25 ); break;
		case ITEM_ARMOR:	olevel = number_range(  5, 15 ); break;
		case ITEM_WEAPON:	olevel = number_range(  5, 15 ); break;
		case ITEM_TREASURE:	olevel = number_range( 10, 20 ); break;
		}

		obj = create_object( pObjIndex, olevel );
		SET_BIT( obj->extra_flags, ITEM_INVENTORY );
	    }

	    else
	    {
                if ( (pObjIndex->limit == -1)  ||
                  (pObjIndex->count < pObjIndex->limit) )
                  obj=create_object(pObjIndex,UMIN(number_fuzzy(level),
                                                   LEVEL_HERO - 1) );
                else break;

	    }

	    obj_to_char( obj, mob );
	    if ( pReset->command == 'E' )
	     {
	      int iWear =  wear_convert( pReset->arg3 );
	      if ( iWear != WEAR_NONE)	equip_char( mob, obj, iWear );
	     }
	    last = TRUE;
	    break;

	case 'D':
	    if ( ( pRoomIndex = get_room_index( pReset->arg1 ) ) == NULL )
	    {
		bug( L"Reset_area: 'D': bad vnum %d.", pReset->arg1 );
		continue;
	    }

	    if ( ( pexit = pRoomIndex->exit[pReset->arg2] ) == NULL )
		break;

	    switch ( pReset->arg3 )
	    {
	    case 0:
		REMOVE_BIT( pexit->exit_info, EX_CLOSED );
		REMOVE_BIT( pexit->exit_info, EX_LOCKED );
		break;

	    case 1:
		SET_BIT(    pexit->exit_info, EX_CLOSED );
		REMOVE_BIT( pexit->exit_info, EX_LOCKED );
		break;

	    case 2:
		SET_BIT(    pexit->exit_info, EX_CLOSED );
		SET_BIT(    pexit->exit_info, EX_LOCKED );
		break;
	    }

	    last = TRUE;
	    break;

	case 'R':
	    if ( ( pRoomIndex = get_room_index( pReset->arg1 ) ) == NULL )
	    {
		bug( L"Reset_area: 'R': bad vnum %d.", pReset->arg1 );
		continue;
	    }

	    {
		int d0;
		int d1;

		for ( d0 = 0; d0 < pReset->arg2 - 1; d0++ )
		{
		    d1                   = number_range( d0, pReset->arg2-1 );
		    pexit                = pRoomIndex->exit[d0];
		    pRoomIndex->exit[d0] = pRoomIndex->exit[d1];
		    pRoomIndex->exit[d1] = pexit;
		}
	    }
	    break;
	}
    }


  return;
}



/*
 * Create an instance of a mobile.
 */
CHAR_DATA *create_mobile( MOB_INDEX_DATA *pMobIndex , AREA_DATA *	pArea)
{
    CHAR_DATA *mob;
    AFFECT_DATA af;
    int i;

    mobile_count++;

    if ( pMobIndex == NULL )
    {
      bug( L"Create_mobile: NULL pMobIndex.", 0 );
      exit( 1 );
    }

    mob = new_char();

    mob->pIndexData	= pMobIndex;

    mob->name		= pMobIndex->player_name;
    mob->id		= get_mob_id();
    mob->short_descr	= pMobIndex->short_descr;
    mob->long_descr	= pMobIndex->long_descr;
    mob->description	= pMobIndex->description;
    mob->spec_fun	= pMobIndex->spec_fun;
    mob->prompt		= NULL;
    mob->progtypes	= pMobIndex->progtypes;
    mob->riding		= FALSE;
    mob->mount		= NULL;
    mob->hunting	= NULL;
    mob->endur		= 0;
    mob->in_mind	= NULL;
    mob->cabal		= CABAL_NONE;
    mob->iclass		= CLASS_CLERIC;

    mob->silver = number_range(mob->level/5,mob->level*2);

  mob->act 		= pMobIndex->act | ACT_IS_NPC;
  mob->comm		= COMM_NOCHANNELS|COMM_NOSHOUT|COMM_NOTELL;
  mob->affected_by	= pMobIndex->affected_by;
  mob->detection		= pMobIndex->detection;

  /*
   * Yonelim ayarlama
   */
  if ( pArea == NULL )
  {
    mob->alignment		= number_range(-1000,1000);
  }
  else
  {
    i = number_range(1,100);
    if (i < pArea->yonelim_iyi)
    {
      mob->alignment = number_range(350,1000);
    }
    else if (i < ( pArea->yonelim_iyi + pArea->yonelim_yansiz ) )
    {
      mob->alignment = number_range(-349,349);
    }
    else
    {
      mob->alignment = number_range(-1000,-350);
    }
  }
  /*
   * Yonelim ayarlama bitti
   */

  mob->level		= pMobIndex->level;
  mob->hitroll		= hitroll_damroll_hesapla(pMobIndex->level);
  mob->damroll		= hitroll_damroll_hesapla(pMobIndex->level);
  mob->max_hit		= number_range( yp_tablo[ pMobIndex->level ].min_yp , yp_tablo[ pMobIndex->level ].max_yp );
  mob->hit		= mob->max_hit;
  mob->max_mana		= number_range( yp_tablo[ pMobIndex->level ].min_yp , yp_tablo[ pMobIndex->level ].max_yp );
  mob->mana		= mob->max_mana;
  mob->damage[DICE_NUMBER]= damage_dice_0(pMobIndex->level);
  mob->damage[DICE_TYPE]	= damage_dice_1(pMobIndex->level);
  mob->damage[DICE_BONUS]	= damage_dice_2(pMobIndex->level);
  mob->dam_type		= dam_type_dice();
  mob->status		= 0;
  if (mob->dam_type == 0)
    switch(number_range(1,3))
    {
    case (1): mob->dam_type = 3;        break;  /* slash */
    case (2): mob->dam_type = 7;        break;  /* pound */
    case (3): mob->dam_type = 11;       break;  /* pierce */
    }
  for (i = 0; i < 4; i++)
    mob->armor[i]	= pMobIndex->ac[i];
  mob->armor[AC_PIERCE]	= ac_dice(AC_PIERCE,pMobIndex->level);
  mob->armor[AC_BASH]		= ac_dice(AC_BASH,pMobIndex->level);
  mob->armor[AC_SLASH]		= ac_dice(AC_SLASH,pMobIndex->level);
  mob->armor[AC_EXOTIC]	= ac_dice(AC_EXOTIC,pMobIndex->level);

  mob->off_flags		= race_table[pMobIndex->race].off;
  mob->imm_flags		= race_table[pMobIndex->race].imm;
  mob->res_flags		= race_table[pMobIndex->race].res;
  mob->vuln_flags		= race_table[pMobIndex->race].vuln;

  mob->start_pos		= position_dice();
  mob->default_pos		= position_dice();
  mob->sex				= sex_dice();

  mob->race		= pMobIndex->race;
  mob->form		= race_table[pMobIndex->race].form;
  mob->parts		= race_table[pMobIndex->race].parts;
  mob->size		= race_table[pMobIndex->race].size;
  mob->material		= str_dup( L"none");
  mob->extracted		= FALSE;
  mob = mob_assign_perm_stats(mob);

  /* let's get some spell action */
  if (IS_AFFECTED(mob,AFF_SANCTUARY))
  {
    af.where	 = TO_AFFECTS;
    af.type      = skill_lookup( L"sanctuary");
    af.level     = mob->level;
    af.duration  = -1;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_SANCTUARY;
    affect_to_char( mob, &af );
  }

  if (IS_AFFECTED(mob,AFF_HASTE))
  {
    af.where	 = TO_AFFECTS;
    af.type      = skill_lookup( L"haste");
    af.level     = mob->level;
    af.duration  = -1;
    af.location  = APPLY_DEX;
    af.modifier  = 1 + (mob->level >= 18) + (mob->level >= 25) + (mob->level >= 32);
    af.bitvector = AFF_HASTE;
    affect_to_char( mob, &af );
  }

  if (IS_AFFECTED(mob,AFF_PROTECT_EVIL))
  {
    af.where	 = TO_AFFECTS;
    af.type	 = skill_lookup( L"protection evil");
    af.level	 = mob->level;
    af.duration	 = -1;
    af.location	 = APPLY_SAVES;
    af.modifier	 = -1;
    af.bitvector = AFF_PROTECT_EVIL;
    affect_to_char(mob,&af);
  }

  if (IS_AFFECTED(mob,AFF_PROTECT_GOOD))
  {
    af.where	 = TO_AFFECTS;
    af.type      = skill_lookup( L"protection good");
    af.level     = mob->level;
    af.duration  = -1;
    af.location  = APPLY_SAVES;
    af.modifier  = -1;
    af.bitvector = AFF_PROTECT_GOOD;
    affect_to_char(mob,&af);
  }

  mob->position = mob->start_pos;

  /* link the mob to the world list */
  mob->next		= char_list;
  char_list		= mob;
  pMobIndex->count++;
  return mob;
}

CHAR_DATA * mob_assign_perm_stats(CHAR_DATA* mob)
{
  int i;
  for (i = 0; i < MAX_STATS; i ++)
        mob->perm_stat[i] = UMIN(25,11 + mob->level/4);

  if (IS_SET(mob->act,ACT_WARRIOR))
  {
      mob->perm_stat[STAT_STR] += 3;
      mob->perm_stat[STAT_INT] -= 1;
      mob->perm_stat[STAT_CON] += 2;
  }

  if (IS_SET(mob->act,ACT_THIEF))
  {
      mob->perm_stat[STAT_DEX] += 3;
      mob->perm_stat[STAT_INT] += 1;
      mob->perm_stat[STAT_WIS] -= 1;
  }

  if (IS_SET(mob->act,ACT_CLERIC))
  {
      mob->perm_stat[STAT_WIS] += 3;
      mob->perm_stat[STAT_DEX] -= 1;
      mob->perm_stat[STAT_STR] += 1;
  }

  if (IS_SET(mob->act,ACT_MAGE))
  {
      mob->perm_stat[STAT_INT] += 3;
      mob->perm_stat[STAT_STR] -= 1;
      mob->perm_stat[STAT_DEX] += 1;
  }

  if (IS_SET(mob->off_flags,OFF_FAST))
      mob->perm_stat[STAT_DEX] += 2;

  return mob;
}

/* duplicate a mobile exactly -- except inventory */
void clone_mobile(CHAR_DATA *parent, CHAR_DATA *clone)
{
    int i;
    AFFECT_DATA *paf;

    if ( parent == NULL || clone == NULL || !IS_NPC(parent))
	return;

    /* start fixing values */
    clone->name 	= str_dup(parent->name);
    clone->short_descr	= str_dup(parent->short_descr);
    clone->long_descr	= str_dup(parent->long_descr);
    clone->description	= str_dup(parent->description);
    clone->sex		= parent->sex;
    clone->iclass	= parent->iclass;
    clone->race		= parent->race;
    clone->level	= parent->level;
    clone->trust	= 0;
    clone->timer	= parent->timer;
    clone->wait		= parent->wait;
    clone->hit		= parent->hit;
    clone->max_hit	= parent->max_hit;
    clone->mana		= parent->mana;
    clone->max_mana	= parent->max_mana;
    clone->move		= parent->move;
    clone->max_move	= parent->max_move;
    clone->silver	= parent->silver;
    clone->exp		= parent->exp;
    clone->act		= parent->act;
    clone->comm		= parent->comm;
    clone->imm_flags	= parent->imm_flags;
    clone->res_flags	= parent->res_flags;
    clone->vuln_flags	= parent->vuln_flags;
    clone->invis_level	= parent->invis_level;
    clone->affected_by	= parent->affected_by;
    clone->detection	= parent->detection;
    clone->position	= parent->position;
    clone->practice	= parent->practice;
    clone->train	= parent->train;
    clone->saving_throw	= parent->saving_throw;
    clone->alignment	= parent->alignment;
    clone->hitroll	= parent->hitroll;
    clone->damroll	= parent->damroll;
    clone->wimpy	= parent->wimpy;
    clone->form		= parent->form;
    clone->parts	= parent->parts;
    clone->size		= parent->size;
    clone->material	= str_dup(parent->material);
    clone->extracted	= parent->extracted;
    clone->off_flags	= parent->off_flags;
    clone->dam_type	= parent->dam_type;
    clone->start_pos	= parent->start_pos;
    clone->default_pos	= parent->default_pos;
    clone->spec_fun	= parent->spec_fun;
    clone->progtypes	= parent->progtypes;
    clone->status	= parent->status;
    clone->hunting	= NULL;
    clone->endur	= 0;
    clone->in_mind	= NULL;
    clone->cabal	= CABAL_NONE;

    for (i = 0; i < 4; i++)
    	clone->armor[i]	= parent->armor[i];

    for (i = 0; i < MAX_STATS; i++)
    {
	clone->perm_stat[i]	= parent->perm_stat[i];
	clone->mod_stat[i]	= parent->mod_stat[i];
    }

    for (i = 0; i < 3; i++)
	clone->damage[i]	= parent->damage[i];

    /* now add the affects */
    for (paf = parent->affected; paf != NULL; paf = paf->next)
        affect_to_char(clone,paf);

}



/*
 * Create an object with modifying the count
 */
OBJ_DATA *create_object( OBJ_INDEX_DATA *pObjIndex, int level )
{
  return create_object_org(pObjIndex,level,TRUE);
}

/*
 * for player load/quit
 * Create an object and do not modify the count
 */
OBJ_DATA *create_object_nocount(OBJ_INDEX_DATA *pObjIndex, int level )
{
  return create_object_org(pObjIndex,level,FALSE);
}

/*
 * Create an instance of an object.
 */
OBJ_DATA *create_object_org( OBJ_INDEX_DATA *pObjIndex, int level, bool Count )
{
    AFFECT_DATA *paf;
    OBJ_DATA *obj;
    int i;


    if ( pObjIndex == NULL )
    {
	bug( L"Create_object: NULL pObjIndex.", 0 );
	exit( 1 );
    }

    obj = new_obj();

    obj->pIndexData	= pObjIndex;
    obj->in_room	= NULL;
    obj->enchanted	= FALSE;

    for (i=1;i < MAX_CABAL;i++)
    {
      if (pObjIndex->vnum == cabal_table[i].obj_vnum)
      {
        /*
        if ( count_obj_list( pObjIndex, object_list) > 0 )
        return(NULL);
        */
        cabal_table[i].obj_ptr = obj;
        break;
      }
    }
    if ( ( obj->pIndexData->limit != -1 )  &&
	 ( obj->pIndexData->count >= obj->pIndexData->limit ) )

    if ( pObjIndex->new_format == 1 )
       dump_to_scr( (wchar_t*)"" );

    if ( pObjIndex->new_format == 1 )
 	obj->level = pObjIndex->level;
    else
	obj->level		= UMAX(0,level);
    obj->wear_loc	= -1;


    obj->name		= pObjIndex->name;
    obj->short_descr	= pObjIndex->short_descr;
    obj->description	= pObjIndex->description;
    obj->material	= str_dup(pObjIndex->material);
    obj->item_type	= pObjIndex->item_type;
    obj->extra_flags	= pObjIndex->extra_flags;
    obj->wear_flags	= pObjIndex->wear_flags;
    obj->value[0]	= pObjIndex->value[0];
    obj->value[1]	= pObjIndex->value[1];
    obj->value[2]	= pObjIndex->value[2];
    obj->value[3]	= pObjIndex->value[3];
    obj->value[4]	= pObjIndex->value[4];
    obj->weight		= pObjIndex->weight;
    obj->extracted	= FALSE;
    obj->progtypes	= pObjIndex->progtypes;
    obj->from           = str_dup( L""); /* used with body parts */
    obj->pit            = OBJ_VNUM_PIT; /* default for corpse decaying */
    obj->altar          = ROOM_VNUM_ALTAR; /* default for corpses */
    obj->condition	= pObjIndex->condition;

    if (level == -1 || pObjIndex->new_format)
	obj->cost	= pObjIndex->cost;
    else
    	obj->cost	= number_fuzzy( 10 )
			* number_fuzzy( level ) * number_fuzzy( level );

    /*
     * Mess with object properties.
     */
    switch ( obj->item_type )
    {
    default:
	bug( L"Read_object: vnum %d bad type.", pObjIndex->vnum );
	break;

    case ITEM_LIGHT:
	if (obj->value[2] == 999)
		obj->value[2] = -1;
	break;

    case ITEM_FURNITURE:
    case ITEM_TRASH:
    case ITEM_CONTAINER:
    case ITEM_DRINK_CON:
    case ITEM_KEY:
    case ITEM_MAYMUNCUK:
    case ITEM_FOOD:
    case ITEM_BOAT:
    case ITEM_CORPSE_NPC:
    case ITEM_CORPSE_PC:
    case ITEM_FOUNTAIN:
    case ITEM_MAP:
    case ITEM_CLOTHING:
    case ITEM_PORTAL:
	if (!pObjIndex->new_format)
	    obj->cost /= 5;
	break;

    case ITEM_TREASURE:
    case ITEM_WARP_STONE:
    case ITEM_ROOM_KEY:
    case ITEM_GEM:
    case ITEM_JEWELRY:
    case ITEM_TATTOO:
	break;

    case ITEM_JUKEBOX:
	for (i = 0; i < 5; i++)
	   obj->value[i] = -1;
	break;

    case ITEM_SCROLL:
	if (level != -1 && !pObjIndex->new_format)
	    obj->value[0]	= number_fuzzy( obj->value[0] );
	break;

    case ITEM_WAND:
    case ITEM_STAFF:
	if (level != -1 && !pObjIndex->new_format)
	{
	    obj->value[0]	= number_fuzzy( obj->value[0] );
	    obj->value[1]	= number_fuzzy( obj->value[1] );
	    obj->value[2]	= obj->value[1];
	}
	if (!pObjIndex->new_format)
	    obj->cost *= 2;
	break;

    case ITEM_WEAPON:
	if (level != -1 && !pObjIndex->new_format)
	{
	    obj->value[1] = number_fuzzy( number_fuzzy( 1 * level / 4 + 2 ) );
	    obj->value[2] = number_fuzzy( number_fuzzy( 3 * level / 4 + 6 ) );
	}
	break;

    case ITEM_ARMOR:
	if (level != -1 && !pObjIndex->new_format)
	{
	    obj->value[0]	= number_fuzzy( level / 5 + 3 );
	    obj->value[1]	= number_fuzzy( level / 5 + 3 );
	    obj->value[2]	= number_fuzzy( level / 5 + 3 );
	}
	break;

    case ITEM_POTION:
    case ITEM_PILL:
	if (level != -1 && !pObjIndex->new_format)
	    obj->value[0] = number_fuzzy( number_fuzzy( obj->value[0] ) );
	break;

    case ITEM_MONEY:
	if (!pObjIndex->new_format)
	    obj->value[0]	= obj->cost;
	break;
    }

    for (paf = pObjIndex->affected; paf != NULL; paf = paf->next)
	if ( paf->location == APPLY_SPELL_AFFECT )
	    affect_to_obj(obj,paf);

    obj->next		= object_list;
    object_list		= obj;
    if ( Count )
      pObjIndex->count++;
    return obj;
}

/* duplicate an object exactly -- except contents */
void clone_object(OBJ_DATA *parent, OBJ_DATA *clone)
{
    int i;
    AFFECT_DATA *paf;
    EXTRA_DESCR_DATA *ed,*ed_new;

    if (parent == NULL || clone == NULL)
	return;

    /* start fixing the object */
    clone->name 	= str_dup(parent->name);
    clone->short_descr 	= str_dup(parent->short_descr);
    clone->description	= str_dup(parent->description);
    clone->item_type	= parent->item_type;
    clone->extra_flags	= parent->extra_flags;
    clone->wear_flags	= parent->wear_flags;
    clone->weight	= parent->weight;
    clone->cost		= parent->cost;
    clone->level	= parent->level;
    clone->condition	= parent->condition;
    clone->material	= str_dup(parent->material);
    clone->timer	= parent->timer;
    clone->from         = parent->from;
    clone->extracted    = parent->extracted;
    clone->pit          = parent->pit;
    clone->altar        = parent->altar;

    for (i = 0;  i < 5; i ++)
	clone->value[i]	= parent->value[i];

    /* affects */
    clone->enchanted	= parent->enchanted;

    for (paf = parent->affected; paf != NULL; paf = paf->next)
	affect_to_obj(clone,paf);

    /* extended desc */
    for (ed = parent->extra_descr; ed != NULL; ed = ed->next)
    {
        ed_new                  = new_extra_descr();
        ed_new->keyword    	= str_dup( ed->keyword);
        ed_new->description     = str_dup( ed->description );
        ed_new->next           	= clone->extra_descr;
        clone->extra_descr  	= ed_new;
    }

}



/*
 * Clear a new character.
 */
void clear_char( CHAR_DATA *ch )
{
    static CHAR_DATA ch_zero;
    int i;

    *ch				= ch_zero;
    ch->name			= &str_empty[0];
    ch->short_descr		= &str_empty[0];
    ch->long_descr		= &str_empty[0];
    ch->description		= &str_empty[0];
    ch->prompt                  = &str_empty[0];
    ch->logon			= current_time;
    ch->lines			= PAGELEN;
    for (i = 0; i < 4; i++)
    	ch->armor[i]		= 100;
    ch->position		= POS_STANDING;
    ch->hit			= 20;
    ch->max_hit			= 20;
    ch->mana			= 100;
    ch->max_mana		= 100;
    ch->move			= 100;
    ch->max_move		= 100;
    ch->last_fought             = NULL;
    ch->last_fight_time         = -1;
    ch->last_death_time         = -1;
    ch->on			= NULL;
    for (i = 0; i < MAX_STATS; i ++)
    {
	ch->perm_stat[i] = 13;
	ch->mod_stat[i] = 0;
    }
    return;
}

/*
 * Get an extra description from a list.
 */
wchar_t *get_extra_descr( const wchar_t *name, EXTRA_DESCR_DATA *ed )
{
    for ( ; ed != NULL; ed = ed->next )
    {
	if ( is_name( (wchar_t *) name, ed->keyword ) )
	    return ed->description;
    }
    return NULL;
}



/*
 * Translates mob virtual number to its mob index struct.
 * Hash table lookup.
 */
MOB_INDEX_DATA *get_mob_index( int vnum )
{
    MOB_INDEX_DATA *pMobIndex;

    for ( pMobIndex  = mob_index_hash[vnum % MAX_KEY_HASH];
	  pMobIndex != NULL;
	  pMobIndex  = pMobIndex->next )
    {
	if ( pMobIndex->vnum == vnum )
	    return pMobIndex;
    }

    if ( fBootDb )
    {
	bug( L"Get_mob_index: bad vnum %d.", vnum );
	exit( 1 );
    }

    return NULL;
}



/*
 * Translates mob virtual number to its obj index struct.
 * Hash table lookup.
 */
OBJ_INDEX_DATA *get_obj_index( int vnum )
{
    OBJ_INDEX_DATA *pObjIndex;

    for ( pObjIndex  = obj_index_hash[vnum % MAX_KEY_HASH];
	  pObjIndex != NULL;
	  pObjIndex  = pObjIndex->next )
    {
	if ( pObjIndex->vnum == vnum )
	    return pObjIndex;
    }

    if ( fBootDb )
    {
	bug( L"Get_obj_index: bad vnum %d.", vnum );
	exit( 1 );
    }

    return NULL;
}



/*
 * Translates mob virtual number to its room index struct.
 * Hash table lookup.
 */
ROOM_INDEX_DATA *get_room_index( int vnum )
{
    ROOM_INDEX_DATA *pRoomIndex;

    for ( pRoomIndex  = room_index_hash[vnum % MAX_KEY_HASH];
	  pRoomIndex != NULL;
	  pRoomIndex  = pRoomIndex->next )
    {
	if ( pRoomIndex->vnum == vnum )
	    return pRoomIndex;
    }

    if ( fBootDb )
    {
	bug( L"Get_room_index: bad vnum %d.", vnum );
	exit( 1 );
    }

    return NULL;
}



/*
 * Read a letter from a file.
 */
wchar_t fread_letter( FILE *fp )
{
    wchar_t c;

    do
    {
	c = getwc( fp );
    }
    while ( isspace(c) );

    return c;
}



/*
 * Read a number from a file.
 */
int fread_number( FILE *fp )
{
    int number;
    bool sign;
    wchar_t c;

    do
    {
	c = getwc( fp );
    }
    while ( isspace(c) );

    number = 0;

    sign   = FALSE;
    if ( c == '+' )
    {
	c = getwc( fp );
    }
    else if ( c == '-' )
    {
	sign = TRUE;
	c = getwc( fp );
    }

    if ( !iswdigit(c) )
    {
	bug( L"Fread_number: bad format.", 0 );
	exit( 1 );
    }

    while ( iswdigit(c) )
    {
	number = number * 10 + c - '0';
	c      = getwc( fp );
    }

    if ( sign )
	number = 0 - number;

    if ( c == '|' )
	number += fread_number( fp );
    else if ( c != ' ' )
	ungetwc( c, fp );

    return number;
}

long fread_flag( FILE *fp)
{
    int number;
    wchar_t c;
    bool negative = FALSE;

    do
    {
	c = getwc(fp);
    }
    while ( isspace(c));

    if (c == '-')
    {
	negative = TRUE;
	c = getwc(fp);
    }

    number = 0;

    if (!iswdigit(c))
    {
	while (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z'))
	{
	    number += flag_convert(c);
	    c = getwc(fp);
	}
    }

    while (iswdigit(c))
    {
	number = number * 10 + c - '0';
	c = getwc(fp);
    }

    if (c == '|')
	number += fread_flag(fp);

    else if  ( c != ' ')
	ungetwc(c,fp);

    if (negative)
	return -1 * number;

    return number;
}

long flag_convert(wchar_t letter )
{
    long bitsum = 0;
    wchar_t i;

    if ('A' <= letter && letter <= 'Z')
    {
	bitsum = 1;
	for (i = letter; i > 'A'; i--)
	    bitsum *= 2;
    }
    else if ('a' <= letter && letter <= 'z')
    {
	bitsum = 67108864; /* 2^26 */
	for (i = letter; i > 'a'; i --)
	    bitsum *= 2;
    }

    return bitsum;
}




/*
 * Read and allocate space for a string from a file.
 * These strings are read-only and shared.
 * Strings are hashed:
 *   each string prepended with hash pointer to prev string,
 *   hash code is simply the string length.
 *   this function takes 40% to 50% of boot-up time.
 */
wchar_t *fread_string( FILE *fp )
{
    wchar_t *plast;
    wchar_t c;

    plast = top_string + sizeof(wchar_t *);
    if ( plast > &string_space[MAX_STRING - MAX_STRING_LENGTH] )
    {
	bug( L"Fread_string: MAX_STRING %d exceeded.", MAX_STRING );
	exit( 1 );
    }

    /*
     * Skip blanks.
     * Read first char.
     */
    do
    {
	c = getwc( fp );
    }
    while ( isspace(c) );
	fwprintf( stderr, L"boot_db :: fread_string-1\n" );
    if ( ( *plast++ = c ) == '~' )
	return &str_empty[0];
	fwprintf( stderr, L"boot_db :: fread_string-2\n" );
    for ( ;; )
    {
        /*
         * Back off the char type lookup,
         *   it was too dirty for portability.
         *   -- Furey
         */
		fwprintf( stderr, L"boot_db :: test-1\n" );
		setlocale(LC_ALL, "tr_TR.UTF-8");
		if ( setlocale( LC_CTYPE, "tr_TR.UTF-8" ) == NULL)
		{
			fwprintf( stderr, L"Locale tr_TR.UTF-8 yapilamadi.\n" );
		}
		*plast = fgetwc(fp);
		
		if (ferror(fp) < 0)
		{
			fprintf(stderr, "fgetwc hatasi: %s\n", explain_ferror(fp));
			clearerr(fp);
			exit(EXIT_FAILURE);
		}
		fwprintf( stderr, L"boot_db :: test-2\n" );
		if (*plast == L'İ')
		{
			fwprintf( stderr, L"boot_db :: test-ii\n" );
			fwprintf( stderr, L"boot_db :: fread_string -%C-\n", *plast );
            plast++;
		}
		else if (*plast == WEOF)
		{
			fwprintf( stderr, L"boot_db :: test-3\n" );
			fwprintf( stderr, L"boot_db :: fread_string -%ld-\n", *plast );
			//bug( L"Fread_string: WEOF", 0 );
			return NULL;
		}
		else if (*plast == L'\n')
		{
			fwprintf( stderr, L"boot_db :: test-4\n" );
			plast++;
            *plast++ = '\r';
		}
		else if (*plast == L'\r')
		{
			fwprintf( stderr, L"boot_db :: test-5\n" );
		}
		else if (*plast == L'~')
		{
			fwprintf( stderr, L"boot_db :: test-6\n" );
			plast++;
			{
				union
				{
					wchar_t *	pc;
					wchar_t	rgc[sizeof(wchar_t *)];
				} u1;
				size_t ic;
				int iHash;
				wchar_t *pHash;
				wchar_t *pHashPrev;
				wchar_t *pString;

				plast[-1] = L'\0';
				iHash     = UMIN( MAX_KEY_HASH - 1, plast - 1 - top_string );
				for ( pHash = string_hash[iHash]; pHash; pHash = pHashPrev )
				{
					for ( ic = 0; ic < sizeof(wchar_t *); ic++ )
					u1.rgc[ic] = pHash[ic];
					pHashPrev = u1.pc;
					pHash    += sizeof(wchar_t *);

					if ( top_string[sizeof(wchar_t *)] == pHash[0]
					&&   !wcscmp( top_string+sizeof(wchar_t *)+1, pHash+1 ) )
					return pHash;
				}

				if ( fBootDb )
				{
					pString		= top_string;
					top_string		= plast;
					u1.pc		= string_hash[iHash];
					for ( ic = 0; ic < sizeof(wchar_t *); ic++ )
					pString[ic] = u1.rgc[ic];
					string_hash[iHash]	= pString;

					nAllocString += 1;
					sAllocString += top_string - pString;
					return pString + sizeof(wchar_t *);
				}
				else
				{
					return str_dup( top_string + sizeof(wchar_t *) );
				}
			}
		}
		else
		{
			fwprintf( stderr, L"boot_db :: test-7\n" );
			fwprintf( stderr, L"boot_db :: fread_string -%ld-\n", *plast );
            plast++;
		}
    }
}

wchar_t *fread_string_eol( FILE *fp )
{
    static bool char_special[256-EOF];
    wchar_t *plast;
    wchar_t c;

    if ( char_special[EOF-EOF] != TRUE )
    {
        char_special[EOF -  EOF] = TRUE;
        char_special['\n' - EOF] = TRUE;
        char_special['\r' - EOF] = TRUE;
    }

    plast = top_string + sizeof(wchar_t *);
    if ( plast > &string_space[MAX_STRING - MAX_STRING_LENGTH] )
    {
        bug( L"Fread_string: MAX_STRING %d exceeded.", MAX_STRING );
        exit( 1 );
    }

    /*
     * Skip blanks.
     * Read first char.
     */
    do
    {
        c = getwc( fp );
    }
    while ( isspace(c) );

    if ( ( *plast++ = c ) == '\n')
        return &str_empty[0];

    for ( ;; )
    {
        if ( !char_special[ ( *plast++ = getwc( fp ) ) - EOF ] )
            continue;

        switch ( plast[-1] )
        {
        default:
            break;

        case EOF:
            bug( L"Fread_string_eol  EOF", 0 );
            exit( 1 );
            break;

        case '\n':  case '\r':
            {
                union
                {
                    wchar_t *      pc;
                    wchar_t        rgc[sizeof(wchar_t *)];
                } u1;
                size_t ic;
                int iHash;
                wchar_t *pHash;
                wchar_t *pHashPrev;
                wchar_t *pString;

                plast[-1] = '\0';
                iHash     = UMIN( MAX_KEY_HASH - 1, plast - 1 - top_string );
                for ( pHash = string_hash[iHash]; pHash; pHash = pHashPrev )
                {
                    for ( ic = 0; ic < sizeof(wchar_t *); ic++ )
                        u1.rgc[ic] = pHash[ic];
                    pHashPrev = u1.pc;
                    pHash    += sizeof(wchar_t *);

                    if ( top_string[sizeof(wchar_t *)] == pHash[0]
                    &&   !wcscmp( top_string+sizeof(wchar_t *)+1, pHash+1 ) )
                        return pHash;
                }

                if ( fBootDb )
                {
                    pString             = top_string;
                    top_string          = plast;
                    u1.pc               = string_hash[iHash];
                    for ( ic = 0; ic < sizeof(wchar_t *); ic++ )
                        pString[ic] = u1.rgc[ic];
                    string_hash[iHash]  = pString;

                    nAllocString += 1;
                    sAllocString += top_string - pString;
                    return pString + sizeof(wchar_t *);
                }
                else
                {
                    return str_dup( top_string + sizeof(wchar_t *) );
                }
            }
        }
    }
}



/*
 * Read to end of line (for comments).
 */
void fread_to_eol( FILE *fp )
{
    wchar_t c;

    do
    {
	c = getwc( fp );
    }
    while ( c != '\n' && c != '\r' );

    do
    {
	c = getwc( fp );
    }
    while ( c == '\n' || c == '\r' );

    ungetwc( c, fp );
    return;
}



/*
 * Read one word (into static buffer).
 */
wchar_t *fread_word( FILE *fp )
{
    static wchar_t word[MAX_INPUT_LENGTH];
    wchar_t *pword;
    wchar_t cEnd;

    do
    {
	cEnd = getwc( fp );
    }
    while ( isspace( cEnd ) );

    if ( cEnd == '\'' || cEnd == '"' )
    {
	pword   = word;
    }
    else
    {
	word[0] = cEnd;
	pword   = word+1;
	cEnd    = ' ';
    }

    for ( ; pword < word + MAX_INPUT_LENGTH; pword++ )
    {
	*pword = getwc( fp );
	if ( cEnd == ' ' ? isspace(*pword) : *pword == cEnd )
	{
	    if ( cEnd == ' ' )
		ungetwc( *pword, fp );
	    *pword = '\0';
	    return word;
	}
    }
    bug( L"Fread_word: word too long.", 0 );
    exit( 1 );
    return NULL;
}

/*
 * Allocate some ordinary memory,
 *   with the expectation of freeing it someday.
 */
void *alloc_mem( int sMem )
{
    void *pMem;
    long *magic;
    long iList;

    sMem += sizeof(*magic);

    for ( iList = 0; iList < MAX_MEM_LIST; iList++ )
    {
        if ( sMem <= rgSizeList[iList] )
            break;
    }

    if ( iList == MAX_MEM_LIST )
    {
        bug( L"Alloc_mem: size %d too large.", sMem );
        exit( 1 );
    }

    if ( rgFreeList[iList] == NULL )
    {
        pMem              = alloc_perm( rgSizeList[iList] );
    }
    else
    {
        pMem              = rgFreeList[iList];
        rgFreeList[iList] = * ((void **) rgFreeList[iList]);
    }

    magic = (long *) pMem;
    *magic = MAGIC_NUM;
    //pMem += sizeof(*magic);
	pMem = (void *) ((long) pMem + (long) (sizeof(*magic)));

    return pMem;
}



/*
 * Free some memory.
 * Recycle it back onto the free list for blocks of that size.
 */
void free_mem( void *pMem, int sMem )
{
    long iList;
    long *magic;

    pMem = (void *) ((long) pMem - (long) sizeof(*magic));
    magic = (long *) pMem;

    if (*magic != MAGIC_NUM)
    {
        bug( L"Attempt to recyle invalid memory of size %d.",sMem);
        bug((wchar_t*) pMem + sizeof(*magic),0);
        return;
    }

    *magic = 0;
    sMem += sizeof(*magic);

    for ( iList = 0; iList < MAX_MEM_LIST; iList++ )
    {
        if ( sMem <= rgSizeList[iList] )
            break;
    }

    if ( iList == MAX_MEM_LIST )
    {
        bug( L"Free_mem: size %d too large.", sMem );
        exit( 1 );
    }

    * ((void **) pMem) = rgFreeList[iList];
    rgFreeList[iList]  = pMem;

    return;
}


/*
 * Allocate some permanent memory.
 * Permanent memory is never freed,
 *   pointers into it may be copied safely.
 */
void *alloc_perm( int sMem )
{
    static wchar_t *pMemPerm;
    static int iMemPerm;
    void *pMem;

    while ( sMem % sizeof(long) != 0 )
	sMem++;
    if ( sMem > MAX_PERM_BLOCK )
    {
	bug( L"Alloc_perm: %d too large.", sMem );
	exit( 1 );
    }

    if ( pMemPerm == NULL || iMemPerm + sMem > MAX_PERM_BLOCK )
    {
	iMemPerm = 0;
	if ( ( pMemPerm = (wchar_t *)calloc( 1, MAX_PERM_BLOCK ) ) == NULL )
	{
	    perror( "Alloc_perm" );
	    exit( 1 );
	}
    }

    pMem        = pMemPerm + iMemPerm;
    iMemPerm   += sMem;
    nAllocPerm += 1;
    sAllocPerm += sMem;
    return pMem;
}



/*
 * Duplicate a string into dynamic memory.
 * Fread_strings are read-only and shared.
 */
wchar_t *str_dup( const wchar_t *str )
{
    wchar_t *str_new;

    if ( str[0] == '\0' )
	return &str_empty[0];

    if ( str >= string_space && str < top_string )
	return (wchar_t *) str;

    str_new = (wchar_t *)alloc_mem( wcslen(str) + 1 );
    wcscpy( str_new, str );
    return str_new;
}



/*
 * Free a string.
 * Null is legal here to simplify callers.
 * Read-only shared strings are not touched.
 */
void free_string( wchar_t *pstr )
{
    if ( pstr == NULL
    ||   pstr == &str_empty[0]
    || ( pstr >= string_space && pstr < top_string ) )
	return;

    free_mem( pstr, wcslen(pstr) + 1 );
    return;
}


void do_areas( CHAR_DATA *ch, wchar_t *argument )
{
    AREA_DATA *pArea;

    printf_to_char(ch,L"Bölgeler:\n\r\n\r");
    for ( pArea = area_first; pArea != NULL; pArea = pArea->next )
    {
        printf_to_char(ch,L"[{W%2d %3d{x] {c%25s{x - {c%s{x\n\r",pArea->low_range,pArea->high_range,pArea->name,pArea->path);
    }

    return;
}



void do_memory( CHAR_DATA *ch, wchar_t *argument )
{
    wchar_t buf[MAX_STRING_LENGTH];

    swprintf( buf, MAX_STRING_LENGTH-1, L"Affects %5d\n\r", top_affect    ); send_to_char( buf, ch );
    swprintf( buf, MAX_STRING_LENGTH-1, L"Areas   %5d\n\r", top_area      ); send_to_char( buf, ch );
    swprintf( buf, MAX_STRING_LENGTH-1, L"ExDes   %5d\n\r", top_ed        ); send_to_char( buf, ch );
    swprintf( buf, MAX_STRING_LENGTH-1, L"Exits   %5d\n\r", top_exit      ); send_to_char( buf, ch );
    swprintf( buf, MAX_STRING_LENGTH-1, L"Helps   %5d\n\r", top_help      ); send_to_char( buf, ch );
    swprintf( buf, MAX_STRING_LENGTH-1, L"Socials %5d\n\r", social_count  ); send_to_char( buf, ch );
    swprintf( buf, MAX_STRING_LENGTH-1, L"Mobs    %5d(%d new format)\n\r", top_mob_index,newmobs );
    send_to_char( buf, ch );
    swprintf( buf, MAX_STRING_LENGTH-1, L"(in use)%5d\n\r", mobile_count  ); send_to_char( buf, ch );
    swprintf( buf, MAX_STRING_LENGTH-1, L"Objs    %5d(%d new format)\n\r", top_obj_index,newobjs );
    send_to_char( buf, ch );
    swprintf( buf, MAX_STRING_LENGTH-1, L"Resets  %5d\n\r", top_reset     ); send_to_char( buf, ch );
    swprintf( buf, MAX_STRING_LENGTH-1, L"Rooms   %5d\n\r", top_room      ); send_to_char( buf, ch );
    swprintf( buf, MAX_STRING_LENGTH-1, L"Shops   %5d\n\r", top_shop      ); send_to_char( buf, ch );

    swprintf( buf, MAX_STRING_LENGTH-1, L"Strings %5d strings of %7d bytes (max %d).\n\r",
	nAllocString, sAllocString, MAX_STRING );
    send_to_char( buf, ch );

    swprintf( buf, MAX_STRING_LENGTH-1, L"Perms   %5d blocks  of %7d bytes.\n\r",
	nAllocPerm, sAllocPerm );
    send_to_char( buf, ch );

    return;
}

void do_dump( CHAR_DATA *ch, wchar_t *argument )
{
    int count,count2,num_pcs,aff_count;
    CHAR_DATA *fch;
    MOB_INDEX_DATA *pMobIndex;
    PC_DATA *pc;
    OBJ_DATA *obj;
    OBJ_INDEX_DATA *pObjIndex;
    ROOM_INDEX_DATA *room;
    EXIT_DATA *exit;
    DESCRIPTOR_DATA *d;
    AFFECT_DATA *af;
    FILE *fp;
    int vnum,nMatch = 0;

    /* open file */
    fclose(fpReserve);
    fp = fopen("mem.dmp","w");

    /* report use of data structures */

    num_pcs = 0;
    aff_count = 0;

    /* mobile prototypes */
    fwprintf(fp, L"MobProt	%4d (%8d bytes)\n",
	top_mob_index, (int)(top_mob_index * (sizeof(*pMobIndex))));

    /* mobs */
    count = 0;  count2 = 0;
    for (fch = char_list; fch != NULL; fch = fch->next)
    {
	count++;
	if (fch->pcdata != NULL)
	    num_pcs++;
	for (af = fch->affected; af != NULL; af = af->next)
	    aff_count++;
    }
    for (fch = char_free; fch != NULL; fch = fch->next)
	count2++;

    fwprintf(fp, L"Mobs	%4d (%8d bytes), %2d free (%d bytes)\n",
	count, (int)(count * (sizeof(*fch))), count2, (int)(count2 * (sizeof(*fch))));

    /* pcdata */
    count = 0;
    for (pc = pcdata_free; pc != NULL; pc = pc->next)
	count++;

    fwprintf(fp, L"Pcdata	%4d (%8d bytes), %2d free (%d bytes)\n",
	num_pcs, (int)(num_pcs * (sizeof(*pc))), count, (int)(count * (sizeof(*pc))));

    /* descriptors */
    count = 0; count2 = 0;
    for (d = descriptor_list; d != NULL; d = d->next)
	count++;
    for (d= descriptor_free; d != NULL; d = d->next)
	count2++;

    fwprintf(fp, L"Descs	%4d (%8d bytes), %2d free (%d bytes)\n",
	count, (int)(count * (sizeof(*d))), count2, (int)(count2 * (sizeof(*d))));

    /* object prototypes */
    for ( vnum = 0; nMatch < top_obj_index; vnum++ )
        if ( ( pObjIndex = get_obj_index( vnum ) ) != NULL )
        {
	    for (af = pObjIndex->affected; af != NULL; af = af->next)
		aff_count++;
            nMatch++;
        }

    fwprintf(fp, L"ObjProt	%4d (%8d bytes)\n",
	top_obj_index, (int)(top_obj_index * (sizeof(*pObjIndex))));


    /* objects */
    count = 0;  count2 = 0;
    for (obj = object_list; obj != NULL; obj = obj->next)
    {
	count++;
	for (af = obj->affected; af != NULL; af = af->next)
	    aff_count++;
    }
    for (obj = obj_free; obj != NULL; obj = obj->next)
	count2++;

    fwprintf(fp, L"Objs	%4d (%8d bytes), %2d free (%d bytes)\n",
	count, (int)(count * (sizeof(*obj))), count2, (int)(count2 * (sizeof(*obj))));

    /* affects */
    count = 0;
    for (af = affect_free; af != NULL; af = af->next)
	count++;

    fwprintf(fp, L"Affects	%4d (%8d bytes), %2d free (%d bytes)\n",
	aff_count, (int)(aff_count * (sizeof(*af))), count, (int)(count * (sizeof(*af))));

    /* rooms */
    fwprintf(fp, L"Rooms	%4d (%8d bytes)\n",
	top_room, (int)(top_room * (sizeof(*room))));

     /* exits */
    fwprintf(fp, L"Exits	%4d (%8d bytes)\n",
	top_exit, (int)(top_exit * (sizeof(*exit))));

    fclose(fp);

    /* start printing out mobile data */
    fp = fopen("mob.dmp","w");

    fwprintf(fp, L"\nMobile Analysis\n");
    fwprintf(fp, L"---------------\n");
    nMatch = 0;
    for (vnum = 0; nMatch < top_mob_index; vnum++)
	if ((pMobIndex = get_mob_index(vnum)) != NULL)
	{
	    nMatch++;
	    fwprintf(fp, L"#%-4d %3d active %3d killed     %s\n",
		pMobIndex->vnum,pMobIndex->count,
		pMobIndex->killed,pMobIndex->short_descr);
	}
    fclose(fp);

    /* start printing out object data */
    fp = fopen("obj.dmp","w");

    fwprintf(fp, L"\nObject Analysis\n");
    fwprintf(fp, L"---------------\n");
    nMatch = 0;
    for (vnum = 0; nMatch < top_obj_index; vnum++)
	if ((pObjIndex = get_obj_index(vnum)) != NULL)
	{
	    nMatch++;
	    fwprintf(fp, L"#%-4d %3d active %3d reset      %s\n",
		pObjIndex->vnum,pObjIndex->count,
		pObjIndex->reset_num,pObjIndex->short_descr);
	}

    /* close file */
    fclose(fp);
    fpReserve = fopen( NULL_FILE, "r" );
}



/*
 * Stick a little fuzz on a number.
 */
int number_fuzzy( int number )
{
    switch ( number_range(0,3) )
    {
    case 0:  number -= 1; break;
    case 3:  number += 1; break;
    }

    return UMAX( 1, number );
}



/*
 * UD'deki rastgele sayi uretiminin temel yeri.
 * Modula ile yapilan hesaplamanin problemini
 * asmaya calisiyoruz.
 */
int number_range( int from, int to )
{
  int x;
  int fark;

  fark = to - from;

  if( fark == 0 )
  {
    return from;
  }

  do {
    x = random();
  } while (x >= (RAND_MAX - (RAND_MAX % (fark+1))));

  x %= (fark+1);

  return x + from;
}



/*
 * Generate a percentile roll.
 */
int number_percent( void )
{
    return number_range(1,100);
}



/*
 * Generate a random door.
 */
int number_door( void )
{
    return number_range(0,5);
}

void init_random_number_generator( )
{
  srandom(time(NULL)^getpid());
  return;
}

/*
 * Roll some dice.
 */
int dice( int number, int size )
{
    int idice;
    int sum;

    switch ( size )
    {
    case 0: return 0;
    case 1: return number;
    }

    for ( idice = 0, sum = 0; idice < number; idice++ )
	sum += number_range( 1, size );

    return sum;
}



/*
 * Simple linear interpolation.
 */
int interpolate( int level, int value_00, int value_32 )
{
    return value_00 + level * (value_32 - value_00) / 32;
}


/*
 * Removes the tildes from a string.
 * Used for player-entered strings that go into disk files.
 */
void smash_tilde( wchar_t *str )
{
    for ( ; *str != '\0'; str++ )
    {
	if ( *str == '~' )
	    *str = '-';
    }

    return;
}



/*
 * Compare strings, case insensitive, for prefix matching.
 * Return TRUE if astr not a prefix of bstr
 *   (compatibility with historical functions).
 */
bool str_prefix( const wchar_t *astr, const wchar_t *bstr )
{
    if ( astr == NULL )
    {
	bug( L"Strn_cmp: null astr.", 0 );
	return TRUE;
    }

    if ( bstr == NULL )
    {
	bug( L"Strn_cmp: null bstr.", 0 );
	return TRUE;
    }

    for ( ; *astr; astr++, bstr++ )
    {
	if ( towlower(*astr) != towlower(*bstr) )
	    return TRUE;
    }

    return FALSE;
}



/*
 * Compare strings, case insensitive, for match anywhere.
 * Returns TRUE is astr not part of bstr.
 *   (compatibility with historical functions).
 */
bool str_infix( const wchar_t *astr, const wchar_t *bstr )
{
    int sstr1;
    int sstr2;
    int ichar;
    wchar_t c0;

    if ( ( c0 = towlower(astr[0]) ) == '\0' )
	return FALSE;

    sstr1 = wcslen(astr);
    sstr2 = wcslen(bstr);

    for ( ichar = 0; ichar <= sstr2 - sstr1; ichar++ )
    {
	if ( c0 == towlower(bstr[ichar]) && !str_prefix( astr, bstr + ichar ) )
	    return FALSE;
    }

    return TRUE;
}



/*
 * Compare strings, case insensitive, for suffix matching.
 * Return TRUE if astr not a suffix of bstr
 *   (compatibility with historical functions).
 */
bool str_suffix( const wchar_t *astr, const wchar_t *bstr )
{
    int sstr1;
    int sstr2;

    sstr1 = wcslen(astr);
    sstr2 = wcslen(bstr);
    if ( sstr1 <= sstr2 && !wcscasecmp( astr, bstr + sstr2 - sstr1 ) )
	return FALSE;
    else
	return TRUE;
}



/*
 * Returns an initial-capped string.
 */
wchar_t *capitalize( const wchar_t *str )
{
    static wchar_t strcap[MAX_STRING_LENGTH];
    int i;

    for ( i = 0; str[i] != '\0'; i++ )
	strcap[i] = towlower(str[i]);
    strcap[i] = '\0';
    strcap[0] = towupper(strcap[0]);
    return strcap;
}


/*
 * Append a string to a file.
 */
void append_file( CHAR_DATA *ch, wchar_t *file, wchar_t *str )
{
    FILE *fp;

    if ( IS_NPC(ch) || str[0] == '\0' )
	return;

    fclose( fpReserve );
    if ( ( fp = fopen( (char*)file, "a" ) ) == NULL )
    {
	perror( (char*)file );
	send_to_char( L"Dosya açılamadı!\n\r", ch );
    }
    else
    {
	fwprintf( fp, L"[%5d] %s: %s\n",
	    ch->in_room ? ch->in_room->vnum : 0, ch->name, str );
	fclose( fp );
    }

    fpReserve = fopen( NULL_FILE, "r" );
    return;
}



/*
 * Reports a bug.
 */
void bug( const wchar_t *str, int param )
{
    wchar_t buf[MAX_STRING_LENGTH-500];

    if ( fpArea != NULL )
    {
	int iLine;
	int iChar;

	if ( fpArea == stdin )
	{
	    iLine = 0;
	}
	else
	{
	    iChar = ftell( fpArea );
	    fseek( fpArea, 0, 0 );
	    for ( iLine = 0; ftell( fpArea ) < iChar; iLine++ )
	    {
		while ( getwc( fpArea ) != '\n' )
		    ;
	    }
	    fseek( fpArea, iChar, 0 );
	}

	swprintf( buf, MAX_STRING_LENGTH-1, L"[*****] FILE: %s LINE: %d", strArea, iLine );
	log_string( buf );
/* RT removed because we don't want bugs shutting the mud
	if ( ( fp = fopen( "shutdown.txt", "a" ) ) != NULL )
	{
	    fwprintf( fp, L"[*****] %s\n", buf );
	    fclose( fp );
	}
*/
    }

    //wcscpy( buf, L"[*****] BUG: " );
	//log_string( str );
    swprintf( buf,MAX_INPUT_LENGTH-1, L"[*****] BUG: %S", str );
    log_string( buf );
/* RT removed due to bug-file spamming
    fclose( fpReserve );
    if ( ( fp = fopen( BUG_FILE, "a" ) ) != NULL )
    {
	fwprintf( fp, L"%s\n", buf );
	fclose( fp );
    }
    fpReserve = fopen( NULL_FILE, "r" );
*/

    return;
}



/*
 * Writes a string to the log.
 */
void log_string( const wchar_t *str )
{
    wchar_t *strtime;

    strtime                    = (wchar_t*)ctime( &current_time );
    strtime[wcslen(strtime)-1] = '\0';
    fwprintf( stderr, L"%s :: %S\n", strtime, str );
    return;
}



/*
 * This function is here to aid in debugging.
 * If the last expression in a function is another function call,
 *   gcc likes to generate a JMP instead of a CALL.
 * This is called "tail chaining."
 * It hoses the debugger call stack for that call.
 * So I make this the last call in certain critical functions,
 *   where I really need the call stack to be right for debugging!
 *
 * If you don't understand this, then LEAVE IT ALONE.
 * Don't remove any calls to tail_chain anywhere.
 *
 * -- Furey
 */
void tail_chain( void )
{
    return;
}


void load_olimits(FILE *fp)
{
  int vnum;
  int limit;
  wchar_t ch;
  OBJ_INDEX_DATA *pIndex;

  for (ch = fread_letter(fp); ch != 'S'; ch = fread_letter(fp) )
    {
      switch(ch)
	{
	case 'O':
	  vnum = fread_number(fp);
	  limit = fread_number(fp);
	  if ( (pIndex = get_obj_index(vnum)) == NULL)
	    {
	      bug( L"Load_olimits: bad vnum %d",vnum);
	      exit(1);
	    }
	  else pIndex->limit = limit;
	  break;

	case '*':
	  fread_to_eol(fp);
	  break;
	default:
	  bug( L"Load_olimits: bad command '%c'",ch);
	  exit(1);
	}
    }
}


/*
 * Add the objects in players not logged on to object count
 */
void load_limited_objects()
{
  #if defined(linux)
  struct dirent *dp;
  #else
  struct direct *dp;
  #endif

  int i;
  DIR *dirp;
  FILE *pfile;
  wchar_t letter;
  wchar_t *word;
  wchar_t buf[MAX_INPUT_LENGTH];
  bool fReadLevel;
  int vnum;
  int tplayed;

  total_levels = 0;

  if ( (dirp = opendir(PLAYER_DIR)) == NULL)
  {
    bug( L"Load_limited_objects: unable to open player directory.",0);
    exit(1);
  }

  for (dp = readdir(dirp); dp != NULL; dp = readdir(dirp) )
  {
    if (strlen(dp->d_name) >= 3)
    {
      swprintf( buf, MAX_STRING_LENGTH-1, L"%s/",PLAYER_DIR);
      wcscat(buf, (wchar_t*)dp->d_name);
      fReadLevel = FALSE;
      tplayed = 0;

      if ( (pfile = fopen((char*)buf, "r")) == NULL)
      {
        bug( L"Load_limited_objects: Can't open player file.",0);
      }
      else
      {
        for (letter = fread_letter(pfile);letter != EOF;letter = fread_letter(pfile) )
        {
          if (letter == 'L')
          {
            if (!fReadLevel)
            {
              word = fread_word(pfile);

              if (!wcscasecmp(word, L"evl") || !wcscasecmp(word,L"ev") || !wcscasecmp(word, L"evel"))
              {
                i = fread_number(pfile);
                fReadLevel = TRUE;
                total_levels += UMAX(0,i - 5);
                swprintf(log_buf,(2*MAX_INPUT_LENGTH)-1, L"[%s]'s file +: %d\n\r",buf, UMAX(0,i-5));
                dump_to_scr(log_buf);
                continue;
              }
            }
          }
          else if (letter == 'P')
          {
            word = fread_word(pfile);

            if (!wcscasecmp(word, L"layLog") )
            {
              int d, t;
              int today = parse_date( current_time );

              fread_number(pfile);	/* read the version */
              while (1)
              {
                if ( (d = fread_number(pfile)) < 0 )
                {
                  break;
                }
                t = fread_number(pfile);
                if ( today > 14 )
                {
                  if (d <= today && d > (today - 14))
                  {
                    tplayed += t;
                  }
                }
                else
                {
                  if ( d < today )
                  {
                    d+= 365;
                  }
                  if (d<=(today + 365) && d>(today + 351))
                  {
                    tplayed += t;
                  }
                }
              }
            }
          }
          else if (letter == '#')
          {
            word = fread_word(pfile);
            if (!wcscasecmp(word, L"O") || !wcscasecmp(word, L"OBJECT"))
            {
              if ( tplayed < MIN_TIME_LIMIT )
              {
                swprintf(log_buf,(2*MAX_INPUT_LENGTH)-1, L"Discarding the player %s for limited equipments!.\n",buf);
                dump_to_scr( log_buf );
                break;
              }
              fread_word(pfile);
              fBootDb = FALSE;
              vnum = fread_number(pfile);
              if (get_obj_index(vnum) != NULL)
              {
                get_obj_index(vnum)->count++;
              }
              fBootDb = TRUE;
            }
          }
          else fread_to_eol(pfile);
        }
        fclose(pfile);
      }
    }
  }
  closedir(dirp);
}

/*
 * Given a name, return the appropriate prac fun.
 */
long prac_lookup( const wchar_t *name )
{
   int i;

   for ( i = 0; prac_table[i].name != NULL; i++)
   {
        if (towlower(name[0]) == towlower(prac_table[i].name[0])
        &&  !str_prefix( name,prac_table[i].name))
            return (1 << prac_table[i].number);
   }

    return 0;
}

/*
 * Snarf can prac declarations.
 */
void load_practicer( FILE *fp )
{
    for ( ; ; )
    {
	MOB_INDEX_DATA *pMobIndex;
	wchar_t letter;

	switch ( letter = fread_letter( fp ) )
	{
	default:
	    bug( L"Load_specials: letter '%c' not *MS.", letter );
	    exit( 1 );

	case 'S':
	    return;

	case '*':
	    break;

	case 'M':
	    pMobIndex	= get_mob_index	( fread_number ( fp ) );
	    SET_BIT(pMobIndex->practicer,prac_lookup( fread_word(fp) ) );
	    if ( pMobIndex->practicer == 0 )
	    {
		bug( L"Load_practicers: 'M': vnum %d.", pMobIndex->vnum );
		exit( 1 );
	    }
	    break;
	}

	fread_to_eol( fp );
    }
}


void load_resetmsg( FILE *fp )
{
    Serarea->resetmsg	= fread_string(fp);
    return;
}

void load_aflag( FILE *fp )
{
    Serarea->area_flag	= fread_flag(fp);
    return;
}
