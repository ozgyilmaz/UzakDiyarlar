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
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,	   *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *									   *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael	   *
 *  Chastain, Michael Quan, and Mitchell Tse.				   *
 *									   *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc	   *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.						   *
 *									   *
 *  Much time and thought has gone into this software and you are	   *
 *  benefitting.  We hope that you share your changes too.  What goes	   *
 *  around, comes around.						   *
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

/* this is a listing of all the commands and command related data */

/* for command types */
#define ML 	MAX_LEVEL	/* implementor */
#define L1	MAX_LEVEL - 1  	/* creator */
#define L2	MAX_LEVEL - 2	/* supreme being */
#define L3	MAX_LEVEL - 3	/* deity */
#define L4 	MAX_LEVEL - 4	/* god */
#define L5	MAX_LEVEL - 5	/* immortal */
#define L6	MAX_LEVEL - 6	/* demigod */
#define L7	MAX_LEVEL - 7	/* angel */
#define L8	MAX_LEVEL - 8	/* avatar */
#define IM	LEVEL_IMMORTAL 	/* angel */
#define HE	LEVEL_HERO	/* hero */

#define COM_INGORE	1


/*
 * Structure for a command in the command lookup table.
 */
struct	cmd_type
{
    char * const	name;
    DO_FUN *		do_fun;
    sh_int		position;
    sh_int		level;
    sh_int		log;
    sh_int              show;
    sh_int		extra;
};

/* the command table itself */
extern	const	struct	cmd_type	cmd_table	[];

/*
 * Command functions.
 * Defined in act_*.c (mostly).
 */
DECLARE_DO_FUN(	do_advance	);
DECLARE_DO_FUN( do_affects	);
DECLARE_DO_FUN( do_affrooms	);
DECLARE_DO_FUN( do_alia		);
DECLARE_DO_FUN( do_alias	);
DECLARE_DO_FUN(	do_allow	);
DECLARE_DO_FUN( do_ambush       );
DECLARE_DO_FUN(	do_areas	);
DECLARE_DO_FUN( do_assassinate  );
DECLARE_DO_FUN(	do_at		);
DECLARE_DO_FUN(	do_auction	);
DECLARE_DO_FUN( do_autoassist	);
DECLARE_DO_FUN( do_autoexit	);
DECLARE_DO_FUN( do_autogold	);
DECLARE_DO_FUN( do_autolist	);
DECLARE_DO_FUN( do_autoloot	);
DECLARE_DO_FUN( do_autosac	);
DECLARE_DO_FUN( do_autosplit	);
DECLARE_DO_FUN(	do_backstab	);
DECLARE_DO_FUN(	do_balance	);
DECLARE_DO_FUN(	do_bamfin	);
DECLARE_DO_FUN(	do_bamfout	);
DECLARE_DO_FUN(	do_ban		);
DECLARE_DO_FUN(	do_bandage	);
DECLARE_DO_FUN( do_bash		);
DECLARE_DO_FUN( do_bash_door	);
DECLARE_DO_FUN( do_berserk	);
DECLARE_DO_FUN( do_blink	);
DECLARE_DO_FUN(	do_blindness_dust);	
DECLARE_DO_FUN( do_bloodthirst  );
DECLARE_DO_FUN( do_blackjack    );
DECLARE_DO_FUN(	do_brandish	);
DECLARE_DO_FUN( do_brief	);
DECLARE_DO_FUN(	do_bug		);
DECLARE_DO_FUN(	do_bury		);
DECLARE_DO_FUN( do_butcher      );
DECLARE_DO_FUN(	do_buy		);
DECLARE_DO_FUN( do_cabal_scan	);
DECLARE_DO_FUN(	do_cast		);
DECLARE_DO_FUN( do_caltraps     );
DECLARE_DO_FUN(	do_cb		);
DECLARE_DO_FUN( do_camouflage   );
DECLARE_DO_FUN( do_camp		);
DECLARE_DO_FUN( do_changes	);
DECLARE_DO_FUN( do_channels	);
DECLARE_DO_FUN( do_control	);
DECLARE_DO_FUN( do_circle       );
DECLARE_DO_FUN( do_claw         );
DECLARE_DO_FUN( do_cleave       );
DECLARE_DO_FUN( do_clear	);
DECLARE_DO_FUN( do_clone	);
DECLARE_DO_FUN(	do_close	);
DECLARE_DO_FUN( do_cls		);
DECLARE_DO_FUN( do_color	);
DECLARE_DO_FUN(	do_commands	);
DECLARE_DO_FUN( do_combine	);
DECLARE_DO_FUN( do_compact	);
DECLARE_DO_FUN(	do_compare	);
DECLARE_DO_FUN(	do_concentrate	);
DECLARE_DO_FUN(	do_consider	);
DECLARE_DO_FUN( do_count	);
DECLARE_DO_FUN(	do_crecall	);
DECLARE_DO_FUN(	do_credits	);
DECLARE_DO_FUN( do_deaf		);
DECLARE_DO_FUN( do_delet	);
DECLARE_DO_FUN( do_delete	);
DECLARE_DO_FUN(	do_demand	);
DECLARE_DO_FUN(	do_deny		);
DECLARE_DO_FUN(	do_deposit	);
DECLARE_DO_FUN(	do_description	);
DECLARE_DO_FUN( do_detect_hidden);
DECLARE_DO_FUN( do_dig		);
DECLARE_DO_FUN( do_dirt		);
DECLARE_DO_FUN(	do_disarm	);
DECLARE_DO_FUN(	do_disconnect	);
DECLARE_DO_FUN(	do_dishonor	);
DECLARE_DO_FUN(	do_down		);
DECLARE_DO_FUN(	do_drag		);
DECLARE_DO_FUN(	do_drink	);
DECLARE_DO_FUN(	do_drop		);
DECLARE_DO_FUN( do_dump		);
DECLARE_DO_FUN(	do_east		);
DECLARE_DO_FUN(	do_eat		);
DECLARE_DO_FUN(	do_echo		);
DECLARE_DO_FUN(	do_emote	);
DECLARE_DO_FUN( do_enchant      );
DECLARE_DO_FUN( do_endure       );
DECLARE_DO_FUN( do_enter	);
DECLARE_DO_FUN( do_envenom	);
DECLARE_DO_FUN(	do_equipment	);
DECLARE_DO_FUN(	do_escape	);
DECLARE_DO_FUN( do_estimate	);
DECLARE_DO_FUN(	do_examine	);
DECLARE_DO_FUN(	do_exits	);
DECLARE_DO_FUN( do_explode	);
DECLARE_DO_FUN( do_fade         );
DECLARE_DO_FUN(	do_fill		);
DECLARE_DO_FUN( do_find		);
DECLARE_DO_FUN( do_flag		);
DECLARE_DO_FUN( do_fly		);
DECLARE_DO_FUN(	do_flee		);
DECLARE_DO_FUN(	do_follow	);
DECLARE_DO_FUN(	do_force	);
DECLARE_DO_FUN(	do_freeze	);
DECLARE_DO_FUN( do_gain		);
DECLARE_DO_FUN( do_grant	);
DECLARE_DO_FUN(	do_get		);
DECLARE_DO_FUN(	do_give		);
DECLARE_DO_FUN( do_glist	);
DECLARE_DO_FUN(	do_goto		);
DECLARE_DO_FUN(	do_group	);
DECLARE_DO_FUN(	do_gtell	);
DECLARE_DO_FUN( do_guard        );
DECLARE_DO_FUN( do_hara         );
DECLARE_DO_FUN( do_heal		);
DECLARE_DO_FUN(	do_help		);
DECLARE_DO_FUN( do_herbs        );
DECLARE_DO_FUN(	do_hide		);
DECLARE_DO_FUN(	do_holylight	);
DECLARE_DO_FUN( do_human        );
DECLARE_DO_FUN( do_hunt         );
DECLARE_DO_FUN(	do_idea		);
DECLARE_DO_FUN( do_identify     );
DECLARE_DO_FUN(	do_ititle	);
DECLARE_DO_FUN(	do_immtalk	);
DECLARE_DO_FUN( do_incognito	);
DECLARE_DO_FUN( do_induct	);
DECLARE_DO_FUN( do_imotd	);
DECLARE_DO_FUN(	do_inventory	);
DECLARE_DO_FUN(	do_invis	);
DECLARE_DO_FUN(	do_judge	);
DECLARE_DO_FUN(	do_katana	);
DECLARE_DO_FUN(	do_kick		);
DECLARE_DO_FUN(	do_kill		);
DECLARE_DO_FUN(	do_lash		);
DECLARE_DO_FUN(	do_layhands	);
DECLARE_DO_FUN( do_learn	);
DECLARE_DO_FUN(	do_list		);
DECLARE_DO_FUN( do_load		);
DECLARE_DO_FUN(	do_lock		);
DECLARE_DO_FUN(	do_log		);
DECLARE_DO_FUN(	do_look		);
DECLARE_DO_FUN(	do_lore		);
DECLARE_DO_FUN( do_make		);
DECLARE_DO_FUN( do_maximum	);
DECLARE_DO_FUN(	do_memory	);
DECLARE_DO_FUN(	do_mfind	);
DECLARE_DO_FUN(	do_mwhere	);
DECLARE_DO_FUN( do_motd		);
DECLARE_DO_FUN( do_mount	);
DECLARE_DO_FUN( do_dismount	);
DECLARE_DO_FUN(	do_murde	);
DECLARE_DO_FUN(	do_murder	);
DECLARE_DO_FUN( do_nerve        );
DECLARE_DO_FUN( do_newlock	);
DECLARE_DO_FUN( do_news		);
DECLARE_DO_FUN( do_noaffect	);
DECLARE_DO_FUN( do_nocancel	);
DECLARE_DO_FUN( do_nochannels	);
DECLARE_DO_FUN(	do_noemote	);
DECLARE_DO_FUN( do_nofollow	);
DECLARE_DO_FUN( do_noloot	);
DECLARE_DO_FUN(	do_north	);
DECLARE_DO_FUN(	do_noshout	);
DECLARE_DO_FUN( do_nosummon	);
DECLARE_DO_FUN(	do_note		);
DECLARE_DO_FUN(	do_notell	);
DECLARE_DO_FUN( do_notitle	);
DECLARE_DO_FUN( do_nscore	);
DECLARE_DO_FUN( do_objlist	);
DECLARE_DO_FUN(	do_ofind	);
DECLARE_DO_FUN(	do_oload	);
DECLARE_DO_FUN(	do_open		);
DECLARE_DO_FUN(	do_order	);
DECLARE_DO_FUN( do_outfit	);
DECLARE_DO_FUN( do_owhere	);
DECLARE_DO_FUN(	do_password	);
DECLARE_DO_FUN(	do_peace	);
DECLARE_DO_FUN( do_pecho	);
DECLARE_DO_FUN( do_penalty	);
DECLARE_DO_FUN( do_permban	);
DECLARE_DO_FUN(	do_pick		);
DECLARE_DO_FUN(	do_poison_smoke	);
DECLARE_DO_FUN( do_popularity	);
DECLARE_DO_FUN( do_pmote	);
DECLARE_DO_FUN(	do_pose		);
DECLARE_DO_FUN( do_pour		);
DECLARE_DO_FUN( do_pracnew	);
DECLARE_DO_FUN(	do_practice	);
DECLARE_DO_FUN(	do_pray		);
DECLARE_DO_FUN( do_prefi	);
DECLARE_DO_FUN( do_prefix	);
DECLARE_DO_FUN( do_premort	);
DECLARE_DO_FUN( do_prompt	);
DECLARE_DO_FUN( do_protect	);
DECLARE_DO_FUN(	do_purge	);
DECLARE_DO_FUN( do_push		);
DECLARE_DO_FUN(	do_put		);
DECLARE_DO_FUN(	do_quaff	);
DECLARE_DO_FUN( do_quest        );
DECLARE_DO_FUN(	do_qui		);
DECLARE_DO_FUN( do_quiet	);
DECLARE_DO_FUN(	do_quit		);
DECLARE_DO_FUN( do_raffects	);
DECLARE_DO_FUN( do_read		);
DECLARE_DO_FUN(	do_reboo	);
DECLARE_DO_FUN(	do_reboot	);
DECLARE_DO_FUN(	do_recall	);
DECLARE_DO_FUN(	do_recho	);
DECLARE_DO_FUN(	do_recite	);
DECLARE_DO_FUN(	do_remor	);
DECLARE_DO_FUN(	do_remort	);
DECLARE_DO_FUN(	do_remove	);
DECLARE_DO_FUN( do_rename       );
DECLARE_DO_FUN(	do_rent		);
DECLARE_DO_FUN( do_repair	);
DECLARE_DO_FUN( do_replay	);
DECLARE_DO_FUN(	do_reply	);
DECLARE_DO_FUN(	do_report	);
DECLARE_DO_FUN(	do_request	);
DECLARE_DO_FUN(	do_rescue	);
DECLARE_DO_FUN( do_resistance   );
DECLARE_DO_FUN(	do_rest		);
DECLARE_DO_FUN(	do_restore	);
DECLARE_DO_FUN( do_reture	);
DECLARE_DO_FUN(	do_return	);
DECLARE_DO_FUN(	do_rset		);
DECLARE_DO_FUN(	do_rstat	);
DECLARE_DO_FUN( do_rules	);
DECLARE_DO_FUN( do_run		);
DECLARE_DO_FUN(	do_sacrifice	);
DECLARE_DO_FUN(	do_save		);
DECLARE_DO_FUN(	do_say		);
DECLARE_DO_FUN(	do_scan		);
DECLARE_DO_FUN(	do_score	);
DECLARE_DO_FUN( do_scroll	);
DECLARE_DO_FUN(	do_sell		);
DECLARE_DO_FUN( do_set		);
DECLARE_DO_FUN(	do_settraps	);
DECLARE_DO_FUN(	do_sense	);
DECLARE_DO_FUN(	do_shield	);
DECLARE_DO_FUN(	do_shoot	);
DECLARE_DO_FUN(	do_shout	);
DECLARE_DO_FUN( do_show		);
DECLARE_DO_FUN(	do_shutdow	);
DECLARE_DO_FUN(	do_shutdown	);
DECLARE_DO_FUN( do_sit		);
DECLARE_DO_FUN( do_skills	);
DECLARE_DO_FUN(	do_sla		);
DECLARE_DO_FUN(	do_slay		);
DECLARE_DO_FUN(	do_sleep	);
DECLARE_DO_FUN( do_slist	);
DECLARE_DO_FUN( do_slook	);
DECLARE_DO_FUN(	do_slookup	);
DECLARE_DO_FUN( do_smite	);
DECLARE_DO_FUN( do_smithing	);
DECLARE_DO_FUN( do_smote	);
DECLARE_DO_FUN(	do_sneak	);
DECLARE_DO_FUN(	do_snoop	);
DECLARE_DO_FUN( do_speak	);
DECLARE_DO_FUN( do_socials	);
DECLARE_DO_FUN(	do_south	);
DECLARE_DO_FUN( do_sockets	);
DECLARE_DO_FUN( do_spellbane    );
DECLARE_DO_FUN( do_spells	);
DECLARE_DO_FUN(	do_split	);
DECLARE_DO_FUN(	do_stand	);
DECLARE_DO_FUN( do_stat		);
DECLARE_DO_FUN( do_limited	);
DECLARE_DO_FUN(	do_steal	);
DECLARE_DO_FUN( do_story	);
DECLARE_DO_FUN( do_strangle     );
DECLARE_DO_FUN( do_string	);
DECLARE_DO_FUN(	do_switch	);
DECLARE_DO_FUN( do_tail		);
DECLARE_DO_FUN( do_tame         );
DECLARE_DO_FUN( do_teach	);
DECLARE_DO_FUN(	do_tell		);
DECLARE_DO_FUN( do_throw        );
DECLARE_DO_FUN( do_tick         );
DECLARE_DO_FUN( do_tiger	);
DECLARE_DO_FUN(	do_time		);
DECLARE_DO_FUN(	do_title	);
DECLARE_DO_FUN(	do_track	);
DECLARE_DO_FUN(	do_train	);
DECLARE_DO_FUN(	do_transfer	);
DECLARE_DO_FUN( do_trip		);
DECLARE_DO_FUN( do_target	);
DECLARE_DO_FUN( do_trophy       );
DECLARE_DO_FUN(	do_trust	);
DECLARE_DO_FUN( do_truesight    );
DECLARE_DO_FUN(	do_typo		);
DECLARE_DO_FUN( do_unalias	);
DECLARE_DO_FUN(	do_unlock	);
DECLARE_DO_FUN( do_unread	);
DECLARE_DO_FUN(	do_up		);
DECLARE_DO_FUN(	do_value	);
DECLARE_DO_FUN( do_vampire	);
DECLARE_DO_FUN( do_vanish	);
DECLARE_DO_FUN( do_vbite	);
DECLARE_DO_FUN(	do_visible	);
DECLARE_DO_FUN( do_violate	);
DECLARE_DO_FUN( do_vnum		);
DECLARE_DO_FUN( do_vtouch	);
DECLARE_DO_FUN(	do_wake		);
DECLARE_DO_FUN( do_wanted       );
DECLARE_DO_FUN( do_warcry       );
DECLARE_DO_FUN( do_bear_call    );
DECLARE_DO_FUN( do_lion_call    );
DECLARE_DO_FUN(	do_wear		);
DECLARE_DO_FUN( do_weapon	);
DECLARE_DO_FUN(	do_weather	);
DECLARE_DO_FUN(	do_west		);
DECLARE_DO_FUN(	do_where	);
DECLARE_DO_FUN(	do_who		);
DECLARE_DO_FUN( do_whois	);
DECLARE_DO_FUN(	do_wimpy	);
DECLARE_DO_FUN(	do_withdraw	);
DECLARE_DO_FUN(	do_wizhelp	);
DECLARE_DO_FUN(	do_wizlock	);
DECLARE_DO_FUN( do_wizlist	);
DECLARE_DO_FUN( do_wiznet	);
DECLARE_DO_FUN( do_worth	);
DECLARE_DO_FUN(	do_yell		);
DECLARE_DO_FUN(	do_zap		);
DECLARE_DO_FUN(	do_zecho	);
