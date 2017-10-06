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


/*
 * Spell functions.
 * Defined in magic.c.
 */
DECLARE_SPELL_FUN(	spell_null		);
DECLARE_SPELL_FUN(	spell_absorb		);
DECLARE_SPELL_FUN(	spell_acid_blast	);
DECLARE_SPELL_FUN(	spell_armor		);
DECLARE_SPELL_FUN(	spell_bless		);
DECLARE_SPELL_FUN(	spell_blindness		);
DECLARE_SPELL_FUN(	spell_burning_hands	);
DECLARE_SPELL_FUN(	spell_call_lightning	);
DECLARE_SPELL_FUN(      spell_calm		);
DECLARE_SPELL_FUN(      spell_cancellation	);
DECLARE_SPELL_FUN(	spell_cause_critical	);
DECLARE_SPELL_FUN(	spell_cause_light	);
DECLARE_SPELL_FUN(	spell_cause_serious	);
DECLARE_SPELL_FUN(	spell_healing_light	);
DECLARE_SPELL_FUN(      spell_chain_lightning   );
DECLARE_SPELL_FUN(	spell_charm_person	);
DECLARE_SPELL_FUN(	spell_chromatic_orb	);
DECLARE_SPELL_FUN(	spell_chill_touch	);
DECLARE_SPELL_FUN(	spell_colour_spray	);
DECLARE_SPELL_FUN(	spell_continual_light	);
DECLARE_SPELL_FUN(	spell_control_weather	);
DECLARE_SPELL_FUN(	spell_create_food	);
DECLARE_SPELL_FUN(	spell_create_rose	);
DECLARE_SPELL_FUN(	spell_create_spring	);
DECLARE_SPELL_FUN(	spell_create_water	);
DECLARE_SPELL_FUN(	spell_cure_blindness	);
DECLARE_SPELL_FUN(	spell_cure_critical	);
DECLARE_SPELL_FUN(      spell_cure_disease	);
DECLARE_SPELL_FUN(	spell_cure_light	);
DECLARE_SPELL_FUN(	spell_cure_poison	);
DECLARE_SPELL_FUN(	spell_cure_serious	);
DECLARE_SPELL_FUN(	spell_curse		);
DECLARE_SPELL_FUN(	spell_death_protection	);
DECLARE_SPELL_FUN(      spell_demonfire		);
DECLARE_SPELL_FUN(	spell_detect_evil	);
DECLARE_SPELL_FUN(	spell_detect_good	);
DECLARE_SPELL_FUN(	spell_detect_hidden	);
DECLARE_SPELL_FUN(	spell_detect_invis	);
DECLARE_SPELL_FUN(	spell_detect_magic	);
DECLARE_SPELL_FUN(	spell_detect_poison	);
DECLARE_SPELL_FUN(	spell_detect_undead	);
DECLARE_SPELL_FUN(	spell_disintegrate	);
DECLARE_SPELL_FUN(	spell_dispel_evil	);
DECLARE_SPELL_FUN(      spell_dispel_good       );
DECLARE_SPELL_FUN(	spell_dispel_magic	);
DECLARE_SPELL_FUN(	spell_earthfade		);
DECLARE_SPELL_FUN(	spell_earthmaw		);
DECLARE_SPELL_FUN(	spell_earthquake	);
DECLARE_SPELL_FUN(	spell_enchant_armor	);
DECLARE_SPELL_FUN(	spell_enchant_weapon	);
DECLARE_SPELL_FUN(	spell_energy_drain	);
DECLARE_SPELL_FUN(      spell_enlarge		);
DECLARE_SPELL_FUN(	spell_faerie_fire	);
DECLARE_SPELL_FUN(	spell_faerie_fog	);
DECLARE_SPELL_FUN(	spell_farsight		);
DECLARE_SPELL_FUN(	spell_fire_and_ice	);
DECLARE_SPELL_FUN(	spell_fireball		);
DECLARE_SPELL_FUN(	spell_fireproof		);
DECLARE_SPELL_FUN(	spell_firestream	);
DECLARE_SPELL_FUN(	spell_flamestrike	);
DECLARE_SPELL_FUN(	spell_floating_disc	);
DECLARE_SPELL_FUN(	spell_fly		);
DECLARE_SPELL_FUN(      spell_forcecage		);
DECLARE_SPELL_FUN(      spell_frenzy		);
DECLARE_SPELL_FUN(      spell_frostbolt		);
DECLARE_SPELL_FUN(	spell_fumble		);
DECLARE_SPELL_FUN(	spell_gate		);
DECLARE_SPELL_FUN(	spell_giant_strength	);
DECLARE_SPELL_FUN(      spell_grounding		);
DECLARE_SPELL_FUN(	spell_harm		);
DECLARE_SPELL_FUN(      spell_haste		);
DECLARE_SPELL_FUN(	spell_heal		);
DECLARE_SPELL_FUN(	spell_heat_metal	);
DECLARE_SPELL_FUN(      spell_holy_word		);
DECLARE_SPELL_FUN(	spell_identify		);
DECLARE_SPELL_FUN(	spell_infravision	);
DECLARE_SPELL_FUN(	spell_invis		);
DECLARE_SPELL_FUN(	spell_know_alignment	);
DECLARE_SPELL_FUN(	spell_lightning_bolt	);
DECLARE_SPELL_FUN(	spell_locate_object	);
DECLARE_SPELL_FUN(	spell_magic_missile	);
DECLARE_SPELL_FUN(      spell_mass_healing	);
DECLARE_SPELL_FUN(	spell_mass_invis	);
DECLARE_SPELL_FUN(	spell_nexus		);
DECLARE_SPELL_FUN(	spell_pass_door		);
DECLARE_SPELL_FUN(      spell_plague		);
DECLARE_SPELL_FUN(	spell_poison		);
DECLARE_SPELL_FUN(	spell_portal		);
DECLARE_SPELL_FUN(	spell_protection_evil	);
DECLARE_SPELL_FUN(	spell_protection_good	);
DECLARE_SPELL_FUN(	spell_ray_of_truth	);
DECLARE_SPELL_FUN(	spell_recharge		);
DECLARE_SPELL_FUN(	spell_refresh		);
DECLARE_SPELL_FUN(	spell_remove_curse	);
DECLARE_SPELL_FUN(	spell_sanctuary		);
DECLARE_SPELL_FUN(	spell_shocking_grasp	);
DECLARE_SPELL_FUN(	spell_shield		);
DECLARE_SPELL_FUN(	spell_sleep		);
DECLARE_SPELL_FUN(	spell_slow		);
DECLARE_SPELL_FUN(	spell_soften		);
DECLARE_SPELL_FUN(	spell_stone_skin	);
DECLARE_SPELL_FUN(	spell_suffocate		);
DECLARE_SPELL_FUN(	spell_summon		);
DECLARE_SPELL_FUN(	spell_summon_air_elm	);
DECLARE_SPELL_FUN(	spell_summon_earth_elm	);
DECLARE_SPELL_FUN(	spell_summon_fire_elm	);
DECLARE_SPELL_FUN(	spell_summon_light_elm	);
DECLARE_SPELL_FUN(	spell_summon_water_elm	);
DECLARE_SPELL_FUN(	spell_transfer_object	);
DECLARE_SPELL_FUN(	spell_teleport		);
DECLARE_SPELL_FUN(	spell_tsunami		);
DECLARE_SPELL_FUN(	spell_ventriloquate	);
DECLARE_SPELL_FUN(	spell_weaken		);
DECLARE_SPELL_FUN(	spell_windwall		);
DECLARE_SPELL_FUN(	spell_word_of_recall	);
DECLARE_SPELL_FUN(	spell_acid_breath	);
DECLARE_SPELL_FUN(	spell_fire_breath	);
DECLARE_SPELL_FUN(	spell_frost_breath	);
DECLARE_SPELL_FUN(	spell_gas_breath	);
DECLARE_SPELL_FUN(	spell_lightning_breath	);
DECLARE_SPELL_FUN(	spell_general_purpose	);
DECLARE_SPELL_FUN(	spell_high_explosive	);
DECLARE_SPELL_FUN(      spell_poison_smoke      );
DECLARE_SPELL_FUN(      spell_mana_transfer     );
DECLARE_SPELL_FUN(      spell_mummify		);
DECLARE_SPELL_FUN(      spell_blindness_dust    );
DECLARE_SPELL_FUN(	spell_disenchant_armor	);
DECLARE_SPELL_FUN(	spell_disenchant_weapon	);
DECLARE_SPELL_FUN(      spell_dispel_good       );
DECLARE_SPELL_FUN(	spell_drain		);
DECLARE_SPELL_FUN(      spell_vanish            );
DECLARE_SPELL_FUN(      spell_transform         );
DECLARE_SPELL_FUN(      spell_demon_summon      );
DECLARE_SPELL_FUN(      spell_scourge           );
DECLARE_SPELL_FUN(      spell_soul_bind         );
DECLARE_SPELL_FUN(      spell_doppelganger      );
DECLARE_SPELL_FUN(      spell_iceball           );
DECLARE_SPELL_FUN(      spell_aura_of_chaos     );
DECLARE_SPELL_FUN(      spell_manacles          );
DECLARE_SPELL_FUN(      spell_mental_knife      );
DECLARE_SPELL_FUN(      spell_shield_ruler      );
DECLARE_SPELL_FUN(      spell_guard_call        );
DECLARE_SPELL_FUN(      spell_nightwalker       );
DECLARE_SPELL_FUN(      spell_eyes              );
DECLARE_SPELL_FUN(      spell_shadow_cloak      );
DECLARE_SPELL_FUN(      spell_nightfall         );
DECLARE_SPELL_FUN(      spell_confuse           );
DECLARE_SPELL_FUN(      spell_mirror            );
DECLARE_SPELL_FUN(      spell_garble            );
DECLARE_SPELL_FUN(      spell_terangreal        );
DECLARE_SPELL_FUN(      spell_kassandra         );
DECLARE_SPELL_FUN(      spell_matandra          );
DECLARE_SPELL_FUN(      spell_sebat             );
DECLARE_SPELL_FUN(      spell_amnesia           );
DECLARE_SPELL_FUN(      spell_chaos_blade       );
DECLARE_SPELL_FUN(      spell_tattoo            );
DECLARE_SPELL_FUN(      spell_remove_tattoo     );
DECLARE_SPELL_FUN(      spell_wrath             );
DECLARE_SPELL_FUN(      spell_randomizer        );
DECLARE_SPELL_FUN(      spell_tesseract         );
DECLARE_SPELL_FUN(      spell_stalker           );
DECLARE_SPELL_FUN(      spell_brew              );
DECLARE_SPELL_FUN(      spell_shadowlife        );
DECLARE_SPELL_FUN(      spell_ruler_badge       );
DECLARE_SPELL_FUN(      spell_remove_badge      );
DECLARE_SPELL_FUN(      spell_dragon_strength   );
DECLARE_SPELL_FUN(      spell_dragon_breath     );
DECLARE_SPELL_FUN(      spell_golden_aura       );
DECLARE_SPELL_FUN(      spell_dragonplate       );
DECLARE_SPELL_FUN(      spell_squire            );
DECLARE_SPELL_FUN(      spell_dragonsword       );
DECLARE_SPELL_FUN(      spell_entangle          );
DECLARE_SPELL_FUN(      spell_knight            );
DECLARE_SPELL_FUN(      spell_love_potion       );
DECLARE_SPELL_FUN(      spell_deafen            );
DECLARE_SPELL_FUN(      spell_protective_shield );
DECLARE_SPELL_FUN(      spell_disperse          );
DECLARE_SPELL_FUN(      spell_bark_skin         );
DECLARE_SPELL_FUN(      spell_acute_vision      );
DECLARE_SPELL_FUN(      spell_bear_call         );
DECLARE_SPELL_FUN(      spell_ranger_staff      );
DECLARE_SPELL_FUN(      spell_hellfire          );
DECLARE_SPELL_FUN(	spell_dragons_breath	);
DECLARE_SPELL_FUN(	spell_sand_storm	);
DECLARE_SPELL_FUN(	spell_scream		);
DECLARE_SPELL_FUN(	spell_vampire		);
DECLARE_SPELL_FUN(	spell_attract_other	);
DECLARE_SPELL_FUN(	spell_animate_dead	);
DECLARE_SPELL_FUN(	spell_animate_object	);
DECLARE_SPELL_FUN(	spell_know_sex		);
DECLARE_SPELL_FUN(	spell_enhanced_armor	);
DECLARE_SPELL_FUN(      spell_elemental_sphere	);
DECLARE_SPELL_FUN(	spell_meld_into_stone	);
DECLARE_SPELL_FUN(	spell_find_object	);
DECLARE_SPELL_FUN(	spell_shielding		);
DECLARE_SPELL_FUN(	spell_web		);
DECLARE_SPELL_FUN(	spell_group_defense	);
DECLARE_SPELL_FUN(	spell_inspire		);
DECLARE_SPELL_FUN(	spell_iron_body 	);
DECLARE_SPELL_FUN(	spell_link		);
DECLARE_SPELL_FUN(	spell_mass_sanctuary	);
DECLARE_SPELL_FUN(	spell_mend		);
DECLARE_SPELL_FUN(	spell_wolf		);
DECLARE_SPELL_FUN(	spell_lion_help		);
DECLARE_SPELL_FUN(	spell_magic_jar		);
DECLARE_SPELL_FUN(	spell_turn		);
DECLARE_SPELL_FUN(	spell_fear		);
DECLARE_SPELL_FUN(	spell_protection_heat	);
DECLARE_SPELL_FUN(	spell_protection_cold	);
DECLARE_SPELL_FUN(	spell_fire_shield	);
DECLARE_SPELL_FUN(	spell_witch_curse	);
DECLARE_SPELL_FUN(	spell_knock		);
DECLARE_SPELL_FUN(	spell_power_kill	);
DECLARE_SPELL_FUN(	spell_magic_resistance  );
DECLARE_SPELL_FUN(	spell_hallucination	);
DECLARE_SPELL_FUN(	spell_vam_blast		);
DECLARE_SPELL_FUN(	spell_dragon_skin	);
DECLARE_SPELL_FUN(	spell_mind_light	);
DECLARE_SPELL_FUN(	spell_lightning_shield	);
DECLARE_SPELL_FUN(	spell_shocking_trap	);
DECLARE_SPELL_FUN(	spell_insanity		);
DECLARE_SPELL_FUN(	spell_power_stun	);
DECLARE_SPELL_FUN(	spell_improved_detection);
DECLARE_SPELL_FUN(	spell_improved_invis	);
DECLARE_SPELL_FUN(	spell_holy_armor	);
DECLARE_SPELL_FUN(	spell_bluefire		);
DECLARE_SPELL_FUN(	spell_severity_force	);
DECLARE_SPELL_FUN(	spell_bless_weapon	);
DECLARE_SPELL_FUN(	spell_resilience	);
DECLARE_SPELL_FUN(	spell_super_heal	);
DECLARE_SPELL_FUN(	spell_master_heal	);
DECLARE_SPELL_FUN(	spell_group_healing	);
DECLARE_SPELL_FUN(	spell_restoring_light	);
DECLARE_SPELL_FUN(	spell_acid_arrow	);
DECLARE_SPELL_FUN(	spell_etheral_fist	);
DECLARE_SPELL_FUN(	spell_spectral_furor	);
DECLARE_SPELL_FUN(	spell_hand_of_undead	);
DECLARE_SPELL_FUN(	spell_disruption	);
DECLARE_SPELL_FUN(	spell_sonic_resonance	);
DECLARE_SPELL_FUN(	spell_mind_wrack	);
DECLARE_SPELL_FUN(	spell_mind_wrench	);
DECLARE_SPELL_FUN(	spell_sulfurus_spray	);
DECLARE_SPELL_FUN(	spell_caustic_font	);
DECLARE_SPELL_FUN(	spell_acetum_primus	);
DECLARE_SPELL_FUN(	spell_galvanic_whip	);
DECLARE_SPELL_FUN(	spell_magnetic_trust	);
DECLARE_SPELL_FUN(	spell_quantum_spike	);
DECLARE_SPELL_FUN(	spell_astral_walk	);
DECLARE_SPELL_FUN(	spell_mist_walk		);
DECLARE_SPELL_FUN(	spell_solar_flight	);
DECLARE_SPELL_FUN(	spell_helical_flow	);
DECLARE_SPELL_FUN(	spell_protection_good	);
DECLARE_SPELL_FUN(	spell_lesser_golem	);
DECLARE_SPELL_FUN(	spell_stone_golem	);
DECLARE_SPELL_FUN(	spell_iron_golem	);
DECLARE_SPELL_FUN(	spell_adamantite_golem	);
DECLARE_SPELL_FUN(	spell_corruption	);
DECLARE_SPELL_FUN(	spell_hurricane		);
DECLARE_SPELL_FUN(	spell_sanctify_lands	);
DECLARE_SPELL_FUN(	spell_deadly_venom	);
DECLARE_SPELL_FUN(	spell_cursed_lands	);
DECLARE_SPELL_FUN(	spell_lethargic_mist	);
DECLARE_SPELL_FUN(	spell_black_death	);
DECLARE_SPELL_FUN(	spell_mysterious_dream	);
DECLARE_SPELL_FUN(	spell_polymorph		);
DECLARE_SPELL_FUN(	spell_plant_form	);
DECLARE_SPELL_FUN(	spell_blade_barrier	);
DECLARE_SPELL_FUN(	spell_protection_negative);
DECLARE_SPELL_FUN(	spell_ruler_aura	);
DECLARE_SPELL_FUN(	spell_evil_spirit	);
DECLARE_SPELL_FUN(	spell_disgrace		);
DECLARE_SPELL_FUN(	spell_control_undead	);
DECLARE_SPELL_FUN(	spell_assist		);
DECLARE_SPELL_FUN(	spell_aid		);
DECLARE_SPELL_FUN(	spell_summon_shadow	);
DECLARE_SPELL_FUN(	spell_remove_fear	);
DECLARE_SPELL_FUN(	spell_desert_fist	);
DECLARE_SPELL_FUN(	spell_take_revenge	);
DECLARE_SPELL_FUN(	spell_holy_aura		);
DECLARE_SPELL_FUN(	spell_holy_fury		);
DECLARE_SPELL_FUN(	spell_light_arrow	);
DECLARE_SPELL_FUN(	spell_hydroblast	);
DECLARE_SPELL_FUN(	spell_wolf_spirit	);
DECLARE_SPELL_FUN(	spell_sword_of_justice	);
DECLARE_SPELL_FUN(	spell_guard_dogs	);
DECLARE_SPELL_FUN(	spell_eyes_of_tiger	);
DECLARE_SPELL_FUN(	spell_lion_shield	);
DECLARE_SPELL_FUN(	spell_evolve_lion	);
DECLARE_SPELL_FUN(	spell_prevent		);
