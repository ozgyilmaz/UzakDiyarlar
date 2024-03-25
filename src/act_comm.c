/***************************************************************************
 *                                                                         *
 * Uzak Diyarlar a��k kaynak T�rk�e Mud projesidir.                        *
 * Oyun geli�tirmesi Jai ve Maru taraf�ndan y�netilmektedir.               *
 * Unutulmamas� gerekenler: Nir, Kame, Nyah, Sint                          *
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
   **************************************************************************/

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
#include <sys/time.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "merc.h"
#include "recycle.h"
#include "tables.h"
#include "interp.h"

int unlink(const char* pathname);

/* command procedures needed */
DECLARE_DO_FUN(do_quit);
DECLARE_DO_FUN(do_quit_count);

bool quit_org	args((CHAR_DATA* ch, char* argument, bool Count, bool Remort));
bool proper_order	args((CHAR_DATA* ch, char* argument));
char* translate(CHAR_DATA* ch, CHAR_DATA* victim, char* argument);
int lang_lookup		args((const char* name));
bool cabal_area_check	args((CHAR_DATA* ch));
void update_total_played	args((CHAR_DATA* ch));


/* RT code to delete yourself */

void do_delet(CHAR_DATA* ch, char* argument)
{
    printf_to_char(ch, "�ntihar etmek i�in komutu eksiksiz yazmal�s�n.\n\r");
}

void do_delete(CHAR_DATA* ch, char* argument)
{
    char strsave[MAX_INPUT_LENGTH];

    if (IS_NPC(ch))
        return;

    if (ch->pcdata->confirm_delete)
    {
        if (argument[0] != '\0')
        {
            printf_to_char(ch, "�ntihardan vazge�ildi.\n\r");
            ch->pcdata->confirm_delete = FALSE;
            return;
        }
        else
        {
            sprintf(strsave, "%s%s", PLAYER_DIR, capitalize(ch->name));
            wiznet("$N intihar etti.", ch, NULL, 0, 0, 0);
            ch->last_fight_time = -1;
            do_quit_count(ch, (char*)"");
            unlink(strsave);
            return;
        }
    }

    if (argument[0] != '\0')
    {
        printf_to_char(ch, "Yaln�zca {Rintihar{x yaz, parametreye gerek yok.\n\r");
        return;
    }

    printf_to_char(ch, "Karakterini silmek i�in komutu tekrarlamal�s�n.\n\r");
    printf_to_char(ch, "UYARI: Bu komutun geri d�n��� yoktur.\n\r");
    printf_to_char(ch, "Komutu parametreyle kullanmak intihardan vazge�ildi�i anlam�na gelir.\n\r");
    ch->pcdata->confirm_delete = TRUE;
    wiznet("$N intihar etmek �zere.", ch, NULL, 0, 0, get_trust(ch));
}


/* RT code to display channel status */

void do_channels(CHAR_DATA* ch, char* argument)
{
    /* lists all channels and their status */
    printf_to_char(ch, "kanal       durumu\n\r");
    printf_to_char(ch, "------------------\n\r");

    printf_to_char(ch, "{dmezat{x         ");
    if (!IS_SET(ch->comm, COMM_NOAUCTION))
        printf_to_char(ch, "A�IK\n\r");
    else
        printf_to_char(ch, "KAPALI\n\r");

    if (IS_IMMORTAL(ch))
    {
        printf_to_char(ch, "{i�l�ms�z kanal�{x    ");
        if (!IS_SET(ch->comm, COMM_NOWIZ))
            printf_to_char(ch, "A�IK\n\r");
        else
            printf_to_char(ch, "KAPALI\n\r");
    }

    printf_to_char(ch, "{kanlat{x          ");
    if (!IS_SET(ch->comm, COMM_DEAF))
        printf_to_char(ch, "A�IK\n\r");
    else
        printf_to_char(ch, "KAPALI\n\r");

    printf_to_char(ch, "{kkd{x          ");
    if (!IS_SET(ch->comm, COMM_NOKD))
        printf_to_char(ch, "A�IK\n\r");
    else
        printf_to_char(ch, "KAPALI\n\r");

    printf_to_char(ch, "{kkdg{x          ");
    if (!IS_SET(ch->comm, COMM_NOKDG))
        printf_to_char(ch, "A�IK\n\r");
    else
        printf_to_char(ch, "KAPALI\n\r");

    printf_to_char(ch, "{tsessiz modu{x     ");
    if (IS_SET(ch->comm, COMM_QUIET))
        printf_to_char(ch, "A�IK\n\r");
    else
        printf_to_char(ch, "KAPALI\n\r");

    if (IS_SET(ch->comm, COMM_SNOOP_PROOF))
        printf_to_char(ch, "Snoop'tan muafs�n.\n\r");

    if (ch->lines != PAGELEN)
    {
        if (ch->lines)
        {
            printf_to_char(ch, "%d sat�rl�k skrol g�r�nt�l�yorsun.\n\r", ch->lines + 2);
        }
        else
            printf_to_char(ch, "Skrol tamponlama kapal�.\n\r");
    }


    if (IS_SET(ch->comm, COMM_NOTELL))
        printf_to_char(ch, "Anlatamazs�n.\n\r");

    if (IS_SET(ch->comm, COMM_NOCHANNELS))
        printf_to_char(ch, "Kanallar� kullanamazs�n.\n\r");

    if (IS_SET(ch->comm, COMM_NOEMOTE))
        printf_to_char(ch, "Duygular�n� g�steremezsin.\n\r");

}

void garble(char* garbled, const char* speech) {
    int i;

    // Ensure garbled buffer is large enough to hold the garbled speech
    if (strlen(speech) >= MAX_STRING_LENGTH) {
        // Handle overflow: truncate or use dynamic buffer
        // ... (Implement appropriate overflow handling)
        return; // Return for now to avoid potential overflow
    }

    for (i = 0; speech[i] != '\0' && i < MAX_STRING_LENGTH - 1; i++) {
        if (speech[i] >= 'a' && speech[i] <= 'z') {
            garbled[i] = 'a' + number_range(0, 25);
        }
        else if (speech[i] >= 'A' && speech[i] <= 'Z') {
            garbled[i] = 'A' + number_range(0, 25);
        }
        else {
            garbled[i] = speech[i];
        }
    }

    garbled[i] = '\0'; // Ensure null termination
}


/* RT deaf blocks out all shouts */

void do_deaf(CHAR_DATA* ch, char* argument)
{

    if (IS_SET(ch->comm, COMM_DEAF))
    {
        printf_to_char(ch, "Anlat�lanlar� almaya ba�lad�n.\n\r");
        REMOVE_BIT(ch->comm, COMM_DEAF);
    }
    else
    {
        printf_to_char(ch, "�u andan sonra anlat�lanlar� alamayacaks�n.\n\r");
        SET_BIT(ch->comm, COMM_DEAF);
    }
}

/* RT quiet blocks out all communication */

void do_quiet(CHAR_DATA* ch, char* argument)
{
    if (IS_SET(ch->comm, COMM_QUIET))
    {
        printf_to_char(ch, "Sessiz moddan ��k�ld�.\n\r");
        REMOVE_BIT(ch->comm, COMM_QUIET);
    }
    else
    {
        printf_to_char(ch, "Bundan b�yle yaln�z s�ylenenleri ve duygular� alg�layabileceksin.\n\r");
        SET_BIT(ch->comm, COMM_QUIET);
    }
}

void do_replay(CHAR_DATA* ch, char* argument)
{
    if (IS_NPC(ch))
    {
        printf_to_char(ch, "Tekrarlayamazs�n.\n\r");
        return;
    }

    page_to_char(buf_string(ch->pcdata->buffer), ch);
    clear_buf(ch->pcdata->buffer);
}

void do_immtalk(CHAR_DATA* ch, char* argument)
{
    DESCRIPTOR_DATA* d;

    if (argument[0] == '\0')
    {
        if (IS_SET(ch->comm, COMM_NOWIZ))
        {
            printf_to_char(ch, "�l�ms�z kanal� a��ld�.\n\r");
            REMOVE_BIT(ch->comm, COMM_NOWIZ);
        }
        else
        {
            printf_to_char(ch, "�l�ms�z kanal� kapand�.\n\r");
            SET_BIT(ch->comm, COMM_NOWIZ);
        }
        return;
    }

    REMOVE_BIT(ch->comm, COMM_NOWIZ);

    write_channel_log(ch, NULL, KANAL_IMM, argument);

    if (!is_affected(ch, gsn_deafen))
        act_new("{m[$n]: $t{x", ch, argument, NULL, TO_CHAR, POS_DEAD);
    for (d = descriptor_list; d != NULL; d = d->next)
    {
        if (d->connected == CON_PLAYING &&
            IS_IMMORTAL(d->character) &&
            !IS_SET(d->character->comm, COMM_NOWIZ))
        {
            act_new("{m[$n]: $t{x", ch, argument, d->character, TO_VICT, POS_DEAD);
        }
    }

    return;
}

void do_kd(CHAR_DATA* ch, char* argument) {
    char arg[MAX_INPUT_LENGTH], buf[MAX_STRING_LENGTH];
    CHAR_DATA* victim;

    if (argument[0] == '\0') {
        // Toggle KD channel status
        return;
    }

    if (IS_AFFECTED(ch, AFF_CHARM) && ch->master != NULL) {
        printf_to_char(ch, "Teshirliyken kd kanal�n� kullanamazs�n.\n\r");
        return;
    }

    if (IS_SET(ch->comm, COMM_NOKD)) {
        printf_to_char(ch, "�nce KD kanal�n� a�mal�s�n.\n\r");
        return;
    }

    argument = one_argument(argument, arg);

    if (arg[0] == '\0' || argument[0] == '\0') {
        printf_to_char(ch, "Kime ne mesaj g�ndereceksin?\n\r");
        return;
    }

    if ((victim = get_char_world(ch, arg)) == NULL || (IS_NPC(victim) && victim->in_room != ch->in_room)) {
        printf_to_char(ch, "Burada de�il.\n\r");
        return;
    }

    if (victim == ch) {
        printf_to_char(ch, "Kendine KD mesaj� atamazs�n.\n\r");
        return;
    }

    // Handling when the victim is disconnected
    if (victim->desc == NULL && !IS_NPC(victim)) {
        act("$N ba�lant�s�n� kaybetmi� g�r�n�yor...daha sonra tekrar dene.", ch, NULL, victim, TO_CHAR);
        // Safely format the message into buf, including handling for disconnected players
        snprintf(buf, sizeof(buf), "%s: %s%s%s\n\r", PERS(ch, victim), CLR_RED_BOLD, argument, CLR_NORMAL);
        buf[0] = UPPER(buf[0]);
        add_buf(victim->pcdata->buffer, buf);
        return;
    }

    // Check if the victim is receiving KD messages
    if ((IS_SET(victim->comm, COMM_NOKD)) && !IS_IMMORTAL(ch)) {
        act("$N KD kanal�n� alm�yor.", ch, 0, victim, TO_CHAR);
        return;
    }

    // Safely format the KD message into buf, ensuring no overflow
    if (is_affected(ch, gsn_garble)) {
        garble(buf, argument); // Ensure garble does not exceed MAX_STRING_LENGTH
    }
    else {
        int result = snprintf(buf, sizeof(buf), "%s", argument);
        if (result >= sizeof(buf)) {
            // Handle overflow: truncate or use dynamic buffer
            // ... (Implement appropriate overflow handling)
        }
    }

    // Check for player ranks and adjust points accordingly
    if (ch->level >= KIDEMLI_OYUNCU_SEVIYESI && victim->level >= KIDEMLI_OYUNCU_SEVIYESI) {
        ch->pcdata->rk_puani -= 1;
    }

    // Send the KD message
    if (!is_affected(ch, gsn_deafen))
        act_color("$N kd: $C$t$c", ch, buf, victim, TO_CHAR, POS_DEAD, CLR_MAGENTA_BOLD);
    act_color("$n kd: $C$t$c", ch, buf, victim, TO_VICT, POS_DEAD, CLR_RED_BOLD);

    victim->reply = ch;

    return;
}


void do_kdg(CHAR_DATA* ch, char* argument)
{
    DESCRIPTOR_DATA* d;
    char buf[MAX_INPUT_LENGTH];

    if (IS_NPC(ch))
    {
        return;
    }

    if (argument[0] == '\0')
    {
        if (IS_SET(ch->comm, COMM_NOKDG))
        {
            printf_to_char(ch, "KDG kanal� a��ld�.\n\r");
            REMOVE_BIT(ch->comm, COMM_NOKDG);
        }
        else
        {
            printf_to_char(ch, "KDG kanal� kapand�.\n\r");
            SET_BIT(ch->comm, COMM_NOKDG);
        }
        return;
    }

    if (IS_AFFECTED(ch, AFF_CHARM) && ch->master != NULL)
    {
        printf_to_char(ch, "Teshirliyken kdg kanal�n� kullanamazs�n.\n\r");
        return;
    }

    if (IS_SET(ch->comm, COMM_NOKDG))
    {
        printf_to_char(ch, "�nce kdg kanal�n� a�mal�s�n.\n\r");
        return;
    }

    if (argument[0] == '\0')
    {
        send_to_char("Oyunculara ne gibi bi'�ey s�yleyeceksin?.\n\r", ch);
        return;
    }

    if (is_affected(ch, gsn_garble))
    {
        garble(buf, argument);
    }
    else
    {
        strcpy(buf, argument);
    }

    ch->pcdata->rk_puani -= 4;


    act_color("$n kdg: $C$T$c", ch, NULL, buf, TO_CHAR, POS_DEAD, CLR_MAGENTA_BOLD);

    for (d = descriptor_list; d != NULL; d = d->next)
    {
        if (d->connected == CON_PLAYING)
        {
            if (d->character != ch)
            {
                if (!IS_SET(d->character->comm, COMM_NOKDG))
                {
                    printf_to_char(d->character, "%s kdg: {G%s{x\n\r", ch->name, buf);
                }
            }
        }
    }

    return;
}

void do_say(CHAR_DATA* ch, char* argument) {
    CHAR_DATA* room_char;
    OBJ_DATA* char_obj;
    CHAR_DATA* vch;
    char buf[MAX_STRING_LENGTH];
    char trans[MAX_STRING_LENGTH];

    if (argument[0] == '\0') {
        printf_to_char(ch, "Ne s�yleyeceksin?\n\r");
        return;
    }
    if (ch->in_room == NULL) {
        printf_to_char(ch, "Fakat bir odada de�ilsin!\n\r");
        return;
    }

    // Assuming garble function correctly limits output size
    if (is_affected(ch, gsn_garble)) {
        garble(buf, argument); // Ensure garble does not exceed MAX_STRING_LENGTH
    }
    else {
        // Safely copy argument to buf, ensuring it does not exceed buffer size
        strncpy(buf, argument, sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0'; // Ensure null-termination
    }

    // Assuming translate does not exceed MAX_STRING_LENGTH, but applying safety
    for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room) {
        if (!is_affected(vch, gsn_deafen)) {
            // Safe usage of snprintf ensures no overflow in trans buffer
            snprintf(trans, sizeof(trans), "%s", translate(ch, vch, buf));
            trans[sizeof(trans) - 1] = '\0'; // Ensure null-termination for safety
            act_color("$C$n '{y$t{x' dedi.$c", ch, trans, vch, TO_VICT, POS_RESTING, CLR_GREEN);
        }
    }

    if (!is_affected(ch, gsn_deafen)) {
        act_color("$CSen '$T' dedin.$c", ch, NULL, buf, TO_CHAR, POS_RESTING, CLR_GREEN);
    }

    // Trigger actions for room characters and objects - no string manipulation here
    for (room_char = ch->in_room->people; room_char != NULL; room_char = room_char->next_in_room) {
        if (IS_SET(room_char->progtypes, MPROG_SPEECH) && room_char != ch) {
            (room_char->pIndexData->mprogs->speech_prog)(room_char, ch, buf);
        }
    }
    for (char_obj = ch->carrying; char_obj != NULL; char_obj = char_obj->next_content) {
        if (IS_SET(char_obj->progtypes, OPROG_SPEECH)) {
            (char_obj->pIndexData->oprogs->speech_prog)(char_obj, ch, buf);
        }
    }
    for (char_obj = ch->in_room->contents; char_obj != NULL; char_obj = char_obj->next_content) {
        if (IS_SET(char_obj->progtypes, OPROG_SPEECH)) {
            (char_obj->pIndexData->oprogs->speech_prog)(char_obj, ch, buf);
        }
    }

    return;
}
size_t strlcpy(char* dst, const char* src, size_t size) {
    size_t srclen = strlen(src);
    if (size > 0) {
        size_t copylen = srclen < size - 1 ? srclen : size - 1;
        memcpy(dst, src, copylen);
        dst[copylen] = '\0';
    }
    return srclen;
}

size_t strlcat(char* dst, const char* src, size_t size) {
    size_t dstlen = strlen(dst);
    size_t srclen = strlen(src);
    if (size > 0 && dstlen < size - 1) {
        size_t copylen = srclen < size - dstlen - 1 ? srclen : size - dstlen - 1;
        memcpy(dst + dstlen, src, copylen);
        dst[dstlen + copylen] = '\0';
    }
    return dstlen + srclen;
}


// Function to escape format specifiers in a message
char* escape_format_specifiers(char* message) {
    char* escaped_message = strdup(message); // Allocate a copy to modify
    char* p = escaped_message;

    while (*p != '\0') {
        if (*p == '$') {
            // Shift characters to overwrite the '$'
            memmove(p, p + 1, strlen(p));
            // Add a backslash to escape the next character
            *p = '\\';
            p++; // Move to the next character
        }
        p++;
    }

    return escaped_message;
}

void do_yell(CHAR_DATA* ch, char* argument) {
    DESCRIPTOR_DATA* d;
    char buf[MAX_STRING_LENGTH]; // Ensure this is sufficiently large for yell content.

    if (argument[0] == '\0') {
        printf_to_char(ch, "Ne hayk�racaks�n?\n\r");
        return;
    }

    write_channel_log(ch, NULL, KANAL_HAYKIR, argument);

    // Secure copying to buf, considering garble might alter argument length
    if (is_affected(ch, gsn_garble)) {
        // Assume garble safely handles string size
        garble(buf, argument);
    }
    else {
        // Use strlcpy for safer copying with null-termination
        strlcpy(buf, argument, sizeof(buf));
    }

    if (!is_affected(ch, gsn_deafen)) {
        // Direct use with buf ensures no additional string size changes
        act_color("Sen '$C$t$c' diye hayk�rd�n.", ch, buf, NULL, TO_CHAR, POS_DEAD, CLR_BROWN);
    }

    for (d = descriptor_list; d != NULL; d = d->next) {
        if (d->connected == CON_PLAYING &&
            d->character != ch &&
            d->character->in_room != NULL &&
            d->character->in_room->area == ch->in_room->area &&
            !is_affected(d->character, gsn_deafen)) {

            // Calculate maximum possible translated length (including language info)
            size_t max_trans_len = strlen(language_table[ch->language].name) + strlen(buf) + 3; // +3 for "()", space, and null terminator

            // Dynamically allocate trans buffer
            char* trans = (char*)malloc(max_trans_len);
            if (trans == NULL) {
                // Handle allocation failure
                bug("do_yell: Failed to allocate memory for trans buffer.", 0);
                continue; // Skip to the next recipient
            }

            // Translate and escape format specifiers
            snprintf(trans, max_trans_len, "%s", translate(ch, d->character, buf));
            char* escaped_trans = escape_format_specifiers(trans);

            act_color("$n '$C$t$c' diye hayk�rd�.", ch, escaped_trans, d->character, TO_VICT, POS_DEAD, CLR_BROWN);

            // Free the escaped message copy and the dynamically allocated trans buffer
            free(escaped_trans);
            free(trans);
        }
    }

    return;
}


void do_emote(CHAR_DATA* ch, char* argument)
{
    char buf[MAX_INPUT_LENGTH];

    if (!IS_NPC(ch) && IS_SET(ch->comm, COMM_NOEMOTE))
    {
        printf_to_char(ch, "Duygular�n� belli edemezsin.\n\r");
        return;
    }

    if (argument[0] == '\0')
    {
        printf_to_char(ch, "Nas�l bir duygu vereceksin?\n\r");
        return;
    }

    write_channel_log(ch, NULL, KANAL_DUYGU, argument);

    if (is_affected(ch, gsn_garble))
        garble(buf, (char*)argument);
    else
        strcpy(buf, argument);

    act("$n $T", ch, NULL, buf, TO_ROOM);
    act("$n $T", ch, NULL, buf, TO_CHAR);
    return;
}


void do_pmote(CHAR_DATA* ch, char* argument)
{
    CHAR_DATA* vch;
    char* letter, * name;
    char last[MAX_INPUT_LENGTH], temp[MAX_STRING_LENGTH];
    size_t matches = 0;

    if (!IS_NPC(ch) && IS_SET(ch->comm, COMM_NOEMOTE))
    {
        printf_to_char(ch, "Duygular�n� belli edemezsin.\n\r");
        return;
    }

    if (argument[0] == '\0')
    {
        printf_to_char(ch, "Nas�l bir duygu vereceksin?\n\r");
        return;
    }

    act("$n $t", ch, argument, NULL, TO_CHAR);

    for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
    {
        if (vch->desc == NULL || vch == ch)
            continue;

        if ((letter = strstr((char*)argument, vch->name)) == NULL)
        {
            act("$N $t", vch, argument, ch, TO_CHAR);
            continue;
        }

        strcpy(temp, argument);
        temp[strlen(argument) - strlen(letter)] = '\0';
        last[0] = '\0';
        name = vch->name;

        for (; *letter != '\0'; letter++)
        {
            if (*letter == '\'' && matches == strlen(vch->name))
            {
                strcat(temp, "r");
                continue;
            }

            if (*letter == 's' && matches == strlen(vch->name))
            {
                matches = 0;
                continue;
            }

            if (matches == strlen(vch->name))
            {
                matches = 0;
            }

            if (*letter == *name)
            {
                matches++;
                name++;
                if (matches == strlen(vch->name))
                {
                    strcat(temp, "you");
                    last[0] = '\0';
                    name = vch->name;
                    continue;
                }
                strncat(last, letter, 1);
                continue;
            }

            matches = 0;
            strcat(temp, last);
            strncat(temp, letter, 1);
            last[0] = '\0';
            name = vch->name;
        }

        act("$N $t", vch, temp, ch, TO_CHAR);
    }

    return;
}


/*
 * All the posing stuff.
 */
struct	pose_table_type
{
    const char* message[2 * MAX_CLASS];
};

const	struct	pose_table_type	pose_table[] =
{
    {
    {
    "Enerjiyle c�z�rd�yorsun.",
    "$n enerjiyle c�z�rd�yor.",
    "Kendini fevkalade kutsal hissediyorsun.",
    "$n fevkalade kutsal g�r�n�yor.",
    "Birka� k���k h�nerini g�steriyorsun.",
    "$n birka� h�nerini g�steriyor, vayy bee.",
    "�i�mi� kaslar�n� g�steriyorsun.",
    "$n �i�kin kaslar�n� sergiliyor."
    }
    },

    {
    {
    "K�sa s�reli�ine bir kelebe�e d�n���yorsun.",
    "$n k�sa s�reli�ine bir kelebe�e d�n���yor.",
    "Can�n s�k�l�nca �arab� suya d�n��t�r�yorsun.",
    "$n can s�k�nt�s�ndan �arab� suya d�n��t�r�yor.",
    "Kulaklar�n� birbirinden ba��ms�z hareket ettiriyorsun.",
    "$n tav�an gibi kulaklar�n� birbirinden ba��ms�z hareket ettiriyor.",
    "Bir f�nd��� parmaklar�nla k�r�yorsun.",
    "$n parmaklar�n�n aras�na koydu�u f�nd��� k�r�yor."
    }
    },

    {
    {
    "Parmaklar�ndan mavi k�v�lc�mlar ��k�yor.",
    "$s parmaklar�ndan mavi k�v�lc�mlar ��k�yor.",
    "Kafan�n �st�nde bir hale olu�uyor.",
    "$s kafas�n�n �st�nde bir hale olu�uyor.",
    "Kendi kendini ba�l�yorsun.",
    "$n kendisini ba�l�yor. Garip biri.",
    "Di�lerini g�steriyorsun.",
    "$n di�lerini g�steriyor. I��yyyy..."
    }
    },

    {
    {
    "G�zlerinde k���k k�rm�z� ���klar dans ediyor.",
    "$s g�zlerinde k���k k�rm�z� ���klar dans ediyor.",
    "Bilgece s�zler telaffuz ediyorsun.",
    "$n bilgece s�zler telaffuz ediyor.",
    "Kamalar ve elmalarla hokkabazl�k yap�yorsun.",
    "$n kama ve elmalarla hokkabazl�k yap�yor.",
    "Kafan� vurunca g�zlerin f�r�l f�r�l d�n�yor.",
    "$n kafas�n� vuruyor ve g�zleri f�r�l f�r�l d�nmeye ba�l�yor."
    }
    },

    {
    {
        "A slimy green monster appears before you and bows.",
        "A slimy green monster appears before $n and bows.",
        "Deep in prayer, you levitate.",
        "Deep in prayer, $n levitates.",
        "You steal the underwear off every person in the room.",
        "Your underwear is gone!  $n stole it!",
        "Crunch, crunch -- you munch a bottle.",
        "Crunch, crunch -- $n munches a bottle."
    }
    },

    {
    {
        "You turn everybody into a little pink elephant.",
        "You are turned into a little pink elephant by $n.",
        "An angel consults you.",
        "An angel consults $n.",
        "The dice roll ... and you win again.",
        "The dice roll ... and $n wins again.",
        "... 98, 99, 100 ... you do pushups.",
        "... 98, 99, 100 ... $n does pushups."
    }
    },

    {
    {
        "A small ball of light dances on your fingertips.",
        "A small ball of light dances on $n's fingertips.",
        "Your body glows with an unearthly light.",
        "$n's body glows with an unearthly light.",
        "You count the money in everyone's pockets.",
        "Check your money, $n is counting it.",
        "Arnold Schwarzenegger admires your physique.",
        "Arnold Schwarzenegger admires $n's physique."
    }
    },

    {
    {
        "Smoke and fumes leak from your nostrils.",
        "Smoke and fumes leak from $n's nostrils.",
        "A spot light hits you.",
        "A spot light hits $n.",
        "You balance a pocket knife on your tongue.",
        "$n balances a pocket knife on your tongue.",
        "Watch your feet, you are juggling granite boulders.",
        "Watch your feet, $n is juggling granite boulders."
    }
    },

    {
    {
        "The light flickers as you rap in magical languages.",
        "The light flickers as $n raps in magical languages.",
        "Everyone levitates as you pray.",
        "You levitate as $n prays.",
        "You produce a coin from everyone's ear.",
        "$n produces a coin from your ear.",
        "Oomph!  You squeeze water out of a granite boulder.",
        "Oomph!  $n squeezes water out of a granite boulder."
    }
    },

    {
    {
        "Your head disappears.",
        "$n's head disappears.",
        "A cool breeze refreshes you.",
        "A cool breeze refreshes $n.",
        "You step behind your shadow.",
        "$n steps behind $s shadow.",
        "You pick your teeth with a spear.",
        "$n picks $s teeth with a spear."
    }
    },

    {
    {
        "A fire elemental singes your hair.",
        "A fire elemental singes $n's hair.",
        "The sun pierces through the clouds to illuminate you.",
        "The sun pierces through the clouds to illuminate $n.",
        "Your eyes dance with greed.",
        "$n's eyes dance with greed.",
        "Everyone is swept off their foot by your hug.",
        "You are swept off your feet by $n's hug."
    }
    },

    {
    {
        "The sky changes color to match your eyes.",
        "The sky changes color to match $n's eyes.",
        "The ocean parts before you.",
        "The ocean parts before $n.",
        "You deftly steal everyone's weapon.",
        "$n deftly steals your weapon.",
        "Your karate chop splits a tree.",
        "$n's karate chop splits a tree."
    }
    },

    {
    {
        "The stones dance to your command.",
        "The stones dance to $n's command.",
        "A thunder cloud kneels to you.",
        "A thunder cloud kneels to $n.",
        "The Grey Mouser buys you a beer.",
        "The Grey Mouser buys $n a beer.",
        "A strap of your armor breaks over your mighty thews.",
        "A strap of $n's armor breaks over $s mighty thews."
    }
    },

    {
    {
        "The heavens and grass change colour as you smile.",
        "The heavens and grass change colour as $n smiles.",
        "The Burning Man speaks to you.",
        "The Burning Man speaks to $n.",
        "Everyone's pocket explodes with your fireworks.",
        "Your pocket explodes with $n's fireworks.",
        "A boulder cracks at your frown.",
        "A boulder cracks at $n's frown."
    }
    },

    {
    {
    "Bir s�reli�ine herkesin giysisi transparan oluyor. Ve sen her�eyi g�r�yorsun.",
    "Giysilerin transparan oluyor. $n kahkahalar at�yor.",
    "Piramitteki g�z sana g�z k�rp�yor.",
    "Piramitteki g�z $e g�z k�rp�yor.",
    "Birden herkes kaman� burunlar�n�n ucunda g�r�yorlar.",
    "$s kamas�n�n burnunun ucunda durdu�unu farkediyorsun. Ne kadar da h�zl�...",
    "Emrini yerine getirmeye paral� askerler geliyor.",
    "Paral� askerler $s emrini yerine getirmek i�in geliyor."
    }
    },

    {
    {
        "A black hole swallows you.",
        "A black hole swallows $n.",
        "Valentine Michael Smith offers you a glass of water.",
        "Valentine Michael Smith offers $n a glass of water.",
        "Where did you go?",
        "Where did $n go?",
        "Four matched Percherons bring in your chariot.",
        "Four matched Percherons bring in $n's chariot."
    }
    },

    {
    {
    "D�nya �sl���nla par�ld�yor.",
    "D�nya $s �sl���yla par�ld�yor.",
    "Tanr�lar sana bir asa veriyor.",
    "Tanr�lar $s bir asa veriyor.",
    "T�k�rt�.",
    "T�k�rt�.",
    "Atlas kendisine yard�m edip edemeyece�ini soruyor.",
    "Atlas $z y�k�n� ta��mada yard�m istiyor."
    }
    }
};



void do_pose(CHAR_DATA* ch, char* argument)
{
    int level;
    int pose;

    if (IS_NPC(ch))
        return;

    level = UMIN(ch->level, (int)sizeof(pose_table) / (int)sizeof(pose_table[0]) - 1);
    pose = number_range(0, level);

    act(pose_table[pose].message[2 * ch->iclass + 0], ch, NULL, NULL, TO_CHAR);
    act(pose_table[pose].message[2 * ch->iclass + 1], ch, NULL, NULL, TO_ROOM);

    return;
}



void do_bug(CHAR_DATA* ch, char* argument)
{
    append_file(ch, (char*)BUG_FILE, argument);
    printf_to_char(ch, "Bug logged.\n\r");
    return;
}

void do_typo(CHAR_DATA* ch, char* argument)
{
    append_file(ch, (char*)TYPO_FILE, argument);
    printf_to_char(ch, "Typo logged.\n\r");
    return;
}

void do_rent(CHAR_DATA* ch, char* argument)
{
    printf_to_char(ch, "There is no rent here.  Just save and quit.\n\r");
    return;
}


void do_qui(CHAR_DATA* ch, char* argument)
{
    printf_to_char(ch, "Uzak Diyarlar'dan ayr�lmak istiyorsan komutu eksiksiz yazmal�s�n.\n\r");
    return;
}


void do_quit(CHAR_DATA* ch, char* argument)
{
    quit_org(ch, argument, FALSE, FALSE);
    return;
}

void do_quit_count(CHAR_DATA* ch, char* argument)
{
    quit_org(ch, argument, TRUE, FALSE);
    return;
}

void do_quit_remort(CHAR_DATA* ch, char* argument)
{
    quit_org(ch, argument, TRUE, TRUE);
    return;
}

bool quit_org(CHAR_DATA* ch, char* argument, bool Count, bool Remort)
{
    DESCRIPTOR_DATA* d, * dr, * d_next;
    CHAR_DATA* vch, * vch_next;
    OBJ_DATA* obj, * obj_next;
    int id;

    if (IS_NPC(ch))
        return FALSE;

    if (ch->position == POS_FIGHTING)
    {
        printf_to_char(ch, "�mkans�z! D�v���yorsun.\n\r");
        return FALSE;
    }

    if (IS_AFFECTED(ch, AFF_SLEEP))
    {
        printf_to_char(ch, "Yatmaya devam et! Hen�z uyanmad�n.\n\r");
        return FALSE;
    }

    if (ch->position < POS_STUNNED)
    {
        printf_to_char(ch, "Hen�z �lmedin.\n\r");
        return FALSE;
    }

    if (ch->last_fight_time != -1 && !IS_IMMORTAL(ch) &&
        (current_time - ch->last_fight_time) < FIGHT_DELAY_TIME)
    {
        printf_to_char(ch, "Adrenalin salg�larken ayr�lamazs�n! Biraz sakinle�meyi dene.\n\r");
        return FALSE;
    }

    if (IS_AFFECTED(ch, AFF_CHARM))
    {
        printf_to_char(ch, "Efendini terketmek istemiyorsun.\n\r");
        return FALSE;
    }

    if (IS_SET(ch->act, PLR_NO_EXP))
    {
        printf_to_char(ch, "Ruhunu kaybetmek istemiyorsun.\n\r");
        return FALSE;
    }

    if (auction->item != NULL && ((ch == auction->buyer) || (ch == auction->seller)))
    {
        printf_to_char(ch, "Mezat bitene kadar bekle.\n\r");
        return FALSE;
    }

    if (!IS_IMMORTAL(ch) &&
        ch->in_room && IS_RAFFECTED(ch->in_room, AFF_ROOM_ESPIRIT))
    {
        printf_to_char(ch, "B�lgenin �er g��leri ayr�lman� engelliyor.\n\r");
        return FALSE;
    }

    if (!IS_IMMORTAL(ch) &&
        ch->cabal != CABAL_INVADER && is_affected(ch, gsn_evil_spirit))
    {
        printf_to_char(ch, "��indeki �er g��ler ayr�lman� engelliyor.\n\r");
        return FALSE;
    }

    if (cabal_area_check(ch))
    {
        printf_to_char(ch, "Ba�ka bir kabal�n b�lgesinde ayr�lamazs�n.\n\r");
        return FALSE;
    }

    if (!Remort)
    {
        printf_to_char(ch, "Her g�zel �eyin bir sonu vard�r.\n\r");
        act_color("$C$n oyundan ayr�ld�.$c", ch, NULL, NULL, TO_ROOM, POS_DEAD, CLR_GREEN);
        sprintf(log_buf, "%s oyundan ayr�ld�.", ch->name);
        log_string(log_buf);
        wiznet("$N oyundan ayr�ld�.", ch, NULL, WIZ_LOGINS, 0, get_trust(ch));
    }

    update_total_played(ch);

    for (obj = object_list; obj != NULL; obj = obj_next)
    {
        obj_next = obj->next;
        if (obj->pIndexData->vnum == 84
            || obj->pIndexData->vnum == 85
            || obj->pIndexData->vnum == 86
            || obj->pIndexData->vnum == 97)
        {
            if (obj->extra_descr == NULL) extract_obj(obj);
            else if (strstr(obj->extra_descr->description, ch->name) != NULL)
                extract_obj(obj);
        }
    }

    for (obj = ch->carrying; obj; obj = obj_next)
    {
        obj_next = obj->next_content;
        if (obj->pIndexData->vnum == OBJ_VNUM_MAGIC_JAR)
        {
            extract_obj(obj);
        }
        if (obj->pIndexData->vnum == 84
            || obj->pIndexData->vnum == 85
            || obj->pIndexData->vnum == 86
            || obj->pIndexData->vnum == 97)
        {
            if (obj->extra_descr == NULL) extract_obj(obj);
            else if (strstr(obj->extra_descr->description, ch->name) != NULL)
                extract_obj(obj);
            else {
                obj_from_char(obj);
                obj_to_room(obj, ch->in_room);
            }
        }
    }

    for (vch = char_list; vch != NULL; vch = vch_next)
    {
        vch_next = vch->next;
        if (is_affected(vch, gsn_doppelganger) && vch->doppel == ch)
        {
            printf_to_char(vch, "Kurban�n ayr�l�nca sen de ger�ek formuna d�n�yorsun.\n\r");
            affect_strip(vch, gsn_doppelganger);
        }

        if (vch->guarding == ch)
        {
            act("Sen $M korumay� b�rakt�n.", vch, NULL, ch, TO_CHAR);
            act("$n seni korumay� b�rakt�.", vch, NULL, ch, TO_VICT);
            act("$n $M korumay� b�rakt�.", vch, NULL, ch, TO_NOTVICT);
            vch->guarding = NULL;
            ch->guarded_by = NULL;
        }

        if (vch->last_fought == ch)
        {
            vch->last_fought = NULL;
            back_home(vch);
        }

        if (vch->hunting == ch)
            vch->hunting = NULL;

    }

    if (ch->guarded_by != NULL)
    {
        ch->guarded_by->guarding = NULL;
        ch->guarded_by = NULL;
    }


    /*
     * After extract_char the ch is no longer valid!
     */
    save_char_obj(ch);
    id = ch->id;
    dr = d = ch->desc;
    if (Count || get_total_played(ch) < MIN_TIME_LIMIT)
        extract_char(ch, TRUE);
    else
        extract_char_nocount(ch, TRUE);

    if (d != NULL && !Remort)
        close_socket(d);


    /* toast evil cheating bastards    */
    for (d = descriptor_list; d != NULL; d = d_next)
    {
        CHAR_DATA* tch;

        d_next = d->next;
        if (Remort && dr == d)	continue;
        tch = d->original ? d->original : d->character;
        if (tch && tch->id == id)
        {
            extract_char_nocount(tch, TRUE);
            close_socket(d);
        }
    }

    iNumPlayers--;

    return TRUE;
}



void do_save(CHAR_DATA* ch, char* argument)
{
    if (IS_NPC(ch))
        return;

    if (ch->level < 2 && !IS_SET(ch->act, PLR_REMORTED))
    {
        printf_to_char(ch, "Kaydetmek i�in seviyen en az 2 olmal�.\n\r");
        return;
    }
    save_char_obj(ch);
    printf_to_char(ch, "Kaydediyor. Unutma ki otomatik kay�t vard�r.\n\r");
    WAIT_STATE(ch, PULSE_VIOLENCE);
    return;
}



void do_follow(CHAR_DATA* ch, char* argument)
{
    /* RT changed to allow unlimited following and follow the NOFOLLOW rules */
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA* victim;

    one_argument(argument, arg);

    if (arg[0] == '\0')
    {
        printf_to_char(ch, "Kimi takip edeceksin?\n\r");
        return;
    }

    if ((victim = get_char_room(ch, arg)) == NULL)
    {
        printf_to_char(ch, "Burada de�il.\n\r");
        return;
    }

    if (IS_AFFECTED(ch, AFF_CHARM) && ch->master != NULL)
    {
        act("En iyisi $M takip etmek!", ch, NULL, ch->master, TO_CHAR);
        return;
    }

    if (victim == ch)
    {
        if (ch->master == NULL)
        {
            printf_to_char(ch, "Zaten kendini izliyorsun.\n\r");
            return;
        }
        stop_follower(ch);
        return;
    }

    if (!IS_NPC(victim) && IS_SET(victim->act, PLR_NOFOLLOW) && !IS_IMMORTAL(ch))
    {
        act("$N takip�i istemiyor.\n\r",
            ch, NULL, victim, TO_CHAR);
        return;
    }

    REMOVE_BIT(ch->act, PLR_NOFOLLOW);

    if (ch->master != NULL)
        stop_follower(ch);

    add_follower(ch, victim);
    return;
}


void add_follower(CHAR_DATA* ch, CHAR_DATA* master)
{
    if (ch->master != NULL)
    {
        bug("Add_follower: non-null master.", 0);
        return;
    }

    ch->master = master;
    ch->leader = NULL;

    if (can_see(master, ch))
        act_color("$C$n seni takip ediyor.$c", ch, NULL, master,
            TO_VICT, POS_RESTING, CLR_YELLOW);
    act_color("$CSen $M takip ediyorsun.$c", ch, NULL, master,
        TO_CHAR, POS_RESTING, CLR_YELLOW);

    return;
}



void stop_follower(CHAR_DATA* ch)
{
    if (ch->master == NULL)
    {
        bug("Stop_follower: null master.", 0);
        return;
    }

    if (IS_AFFECTED(ch, AFF_CHARM))
    {
        REMOVE_BIT(ch->affected_by, AFF_CHARM);
        affect_strip(ch, gsn_charm_person);
    }

    if (can_see(ch->master, ch) && ch->in_room != NULL)
    {
        act_color("$C$n seni takip etmeyi b�rakt�.$c", ch, NULL, ch->master,
            TO_VICT, POS_RESTING, CLR_BLUE);
        act_color("$CSen $M takip etmeyi b�rakt�n.$c", ch, NULL, ch->master,
            TO_CHAR, POS_RESTING, CLR_BLUE);
    }
    if (ch->master->pet == ch)
        ch->master->pet = NULL;

    ch->master = NULL;
    ch->leader = NULL;
    return;
}

/* nukes charmed monsters and pets */
void nuke_pets(CHAR_DATA* ch)
{
    CHAR_DATA* pet;

    if ((pet = ch->pet) != NULL)
    {
        stop_follower(pet);
        if (pet->in_room != NULL)
            act("$N yava��a yok oluyor.", ch, NULL, pet, TO_NOTVICT);
        extract_char_nocount(pet, TRUE);
    }
    ch->pet = NULL;

    return;
}



void die_follower(CHAR_DATA* ch)
{
    CHAR_DATA* fch;
    CHAR_DATA* fch_next;

    if (ch->master != NULL)
    {
        if (ch->master->pet == ch)
            ch->master->pet = NULL;
        stop_follower(ch);
    }

    ch->leader = NULL;

    for (fch = char_list; fch != NULL; fch = fch_next)
    {
        fch_next = fch->next;
        if (fch->master == ch)
            stop_follower(fch);
        if (fch->leader == ch)
            fch->leader = fch;
    }

    return;
}


void do_order(CHAR_DATA* ch, char* argument)
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH], arg2[MAX_INPUT_LENGTH];
    CHAR_DATA* victim;
    CHAR_DATA* och;
    CHAR_DATA* och_next;
    bool found;
    bool fAll;

    argument = one_argument(argument, arg);
    one_argument(argument, arg2);

    if (!str_cmp(arg2, "intihar"))
    {
        printf_to_char(ch, "Unut bunu.\n\r");
        return;
    }

    if (arg[0] == '\0' || argument[0] == '\0')
    {
        printf_to_char(ch, "Kime ne emredeceksin?\n\r");
        return;
    }

    if (IS_AFFECTED(ch, AFF_CHARM))
    {
        printf_to_char(ch, "Emir vermekten ziyade almaktan ho�lan�yorsun.\n\r");
        return;
    }

    if (!str_cmp(arg, "t�m�"))//ozi
    {
        fAll = TRUE;
        victim = NULL;
    }
    else
    {
        fAll = FALSE;
        if ((victim = get_char_room(ch, arg)) == NULL)
        {
            printf_to_char(ch, "Burada de�il.\n\r");
            return;
        }

        if (victim == ch)
        {
            printf_to_char(ch, "Tabii...emredersiniz!\n\r");
            return;
        }

        if (!IS_AFFECTED(victim, AFF_CHARM) || victim->master != ch
            || (IS_IMMORTAL(victim) && victim->trust >= ch->trust))
        {
            printf_to_char(ch, "Kendin yap!\n\r");
            return;
        }
    }

    found = FALSE;
    for (och = ch->in_room->people; och != NULL; och = och_next)
    {
        och_next = och->next_in_room;

        if (IS_AFFECTED(och, AFF_CHARM)
            && och->master == ch
            && (fAll || och == victim))
        {
            found = TRUE;
            if (!proper_order(och, argument))
                continue;
            sprintf(buf, "$n sana '%s' diye emrediyor, ve sen de yap�yorsun.", argument);
            act(buf, ch, NULL, och, TO_VICT);
            interpret(och, argument, TRUE);
        }
    }

    if (found)
    {
        WAIT_STATE(ch, PULSE_VIOLENCE);
        printf_to_char(ch, "Tamam.\n\r");
    }
    else
        printf_to_char(ch, "Seni takip eden birini g�remiyorum.\n\r");
    return;
}

bool proper_order(CHAR_DATA* ch, char* argument)
{
    char command[MAX_INPUT_LENGTH];
    bool found;
    int trust, cmd_num;
    DO_FUN* cmd;


    one_argument(argument, command);
    found = FALSE;

    trust = get_trust(ch);

    for (cmd_num = 0; cmd_table[cmd_num].name[0] != '\0'; cmd_num++)
    {
        if (command[0] == cmd_table[cmd_num].name[0]
            && !str_prefix(command, cmd_table[cmd_num].name)
            && cmd_table[cmd_num].level <= trust)
        {
            found = TRUE;
            break;
        }
    }
    if (!found) return TRUE;
    cmd = cmd_table[cmd_num].do_fun;

    if (!IS_NPC(ch))
    {
        if (cmd == do_delete || cmd == do_remort || cmd == do_induct
            || cmd == do_quest || cmd == do_practice || cmd == do_train)
            return FALSE;
        return TRUE;
    }

    if (((cmd == do_bash) || (cmd == do_dirt) || (cmd == do_kick)
        || (cmd == do_murder) || (cmd == do_trip))
        && ch->fighting == NULL)
        return FALSE;

    if ((cmd == do_assassinate) || (cmd == do_ambush) || (cmd == do_blackjack)
        || (cmd == do_cleave) || (cmd == do_kill) || (cmd == do_murder)
        || (cmd == do_recall) || (cmd == do_strangle) || (cmd == do_vtouch))
        return FALSE;

    if (cmd == do_close || cmd == do_lock || cmd == do_open ||
        cmd == do_unlock)
    {
        if (race_table[RACE(ch)].pc_race) return TRUE;
        if (RACE(ch) != 34 &&		/* doll */
            RACE(ch) != 37 &&		/* goblin */
            RACE(ch) != 38 &&		/* hobgoblin */
            RACE(ch) != 39 &&		/* kobolt */
            RACE(ch) != 40 &&		/* lizard */
            RACE(ch) != 41 &&		/* modron */
            RACE(ch) != 42)		/* orc */
            return FALSE;
        else
            return TRUE;
    }

    if ((cmd == do_backstab) || (cmd == do_hide) || (cmd == do_pick)
        || (cmd == do_sneak))
    {
        if (IS_SET(ch->act, ACT_THIEF))
            return TRUE;
        else return FALSE;
    }

    return TRUE;
}


void do_group(CHAR_DATA* ch, char* argument)
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA* victim;

    one_argument(argument, arg);

    if (arg[0] == '\0')
    {
        CHAR_DATA* gch;
        CHAR_DATA* leader;

        leader = (ch->leader != NULL) ? ch->leader : ch;
        printf_to_char(ch, "%s liderli�indeki grup:\n\r", PERS(leader, ch));

        for (gch = char_list; gch != NULL; gch = gch->next)
        {
            if (is_same_group(gch, ch))
            {
                printf_to_char(ch,
                    "[%2d %s] %-16s %d/%d hp %d/%d mana %d/%d mv   %5d xp\n\r",
                    gch->level,
                    IS_NPC(gch) ? "Mob" : class_table[gch->iclass].who_name,
                    capitalize(PERS(gch, ch)),
                    gch->hit, gch->max_hit,
                    gch->mana, gch->max_mana,
                    gch->move, gch->max_move,
                    gch->exp);
            }
        }
        return;
    }

    if ((victim = get_char_room(ch, arg)) == NULL)
    {
        printf_to_char(ch, "Burada de�il.\n\r");
        return;
    }

    if (ch->master != NULL || (ch->leader != NULL && ch->leader != ch))
    {
        printf_to_char(ch, "Fakat sen ba�ka birini takip ediyorsun!\n\r");
        return;
    }

    if (victim->master != ch && ch != victim)
    {
        act("$N seni takip etmiyor.", ch, NULL, victim, TO_CHAR);
        return;
    }

    if (IS_AFFECTED(victim, AFF_CHARM))
    {
        printf_to_char(ch, "Tesirli moblar� grubundan ay�ramazs�n.\n\r");
        return;
    }

    if (IS_AFFECTED(ch, AFF_CHARM))
    {
        act("Efendini �ylesine seviyorsun ki, terketmeyi akl�ndan bile ge�irmezsin!", ch, NULL, victim, TO_VICT);
        return;
    }


    if (is_same_group(victim, ch) && ch != victim)
    {
        if (ch->guarding == victim || victim->guarded_by == ch)
        {
            act("Sen $M korumay� b�rakt�n.", ch, NULL, victim, TO_CHAR);
            act("$n seni korumay� b�rakt�.", ch, NULL, victim, TO_VICT);
            act("$n $M korumay� b�rakt�.", ch, NULL, victim, TO_NOTVICT);
            victim->guarded_by = NULL;
            ch->guarding = NULL;
        }

        victim->leader = NULL;
        act_color("$C$n $M grubundan ay�rd�.$c", ch, NULL, victim,
            TO_NOTVICT, POS_SLEEPING, CLR_YELLOW);
        act_color("$C$n seni grubundan ay�rd�.$c", ch, NULL, victim,
            TO_VICT, POS_SLEEPING, CLR_YELLOW);
        act_color("$CSen $M grubundan ay�rd�n.$c", ch, NULL, victim,
            TO_CHAR, POS_SLEEPING, CLR_BLUE);

        if (victim->guarded_by != NULL &&
            !is_same_group(victim, victim->guarded_by))
        {
            act("Sen $M korumay� b�rakt�n.", victim->guarded_by, NULL, victim, TO_CHAR);
            act("$n seni korumay� b�rakt�.", victim->guarded_by, NULL, victim, TO_VICT);
            act("$n $M korumay� b�rakt�.", victim->guarded_by, NULL, victim, TO_NOTVICT);
            victim->guarded_by->guarding = NULL;
            victim->guarded_by = NULL;
        }
        return;
    }

    if (ch->level - victim->level < -8
        || ch->level - victim->level > 8)
    {
        act_color("$C$N $s grubuna giremiyor.$c", ch, NULL, victim,
            TO_NOTVICT, POS_SLEEPING, CLR_RED_BOLD);
        act_color("$CSen $s grubuna giremiyorsun.$c", ch, NULL, victim,
            TO_VICT, POS_SLEEPING, CLR_RED_BOLD);
        act_color("$C$N senin grubuna giremiyor.$c", ch, NULL, victim,
            TO_CHAR, POS_SLEEPING, CLR_RED_BOLD);
        return;
    }

    if (IS_GOOD(ch) && IS_EVIL(victim))
    {
        act_color("$CSen $s grubu i�in fazla �er yanl�s�s�n.$c", ch, NULL, victim,
            TO_VICT, POS_SLEEPING, CLR_RED);
        act_color("$C$N grubun i�in fazla �er yanl�s�!$c", ch, NULL, victim,
            TO_CHAR, POS_SLEEPING, CLR_RED);
        ch->pcdata->rk_puani -= 1;
        return;
    }

    if (IS_GOOD(victim) && IS_EVIL(ch))
    {
        act_color("$CSen $s grubu i�in fazla masumsun!$c", ch, NULL, victim,
            TO_VICT, POS_SLEEPING, CLR_RED);
        act_color("$C$N grubun i�in fazla masum!$c", ch, NULL, victim,
            TO_CHAR, POS_SLEEPING, CLR_RED);
        ch->pcdata->rk_puani -= 1;
        return;
    }

    if ((ch->cabal == CABAL_RULER && victim->cabal == CABAL_CHAOS) ||
        (ch->cabal == CABAL_CHAOS && victim->cabal == CABAL_RULER) ||
        (ch->cabal == CABAL_KNIGHT && victim->cabal == CABAL_INVADER) ||
        (ch->cabal == CABAL_INVADER && victim->cabal == CABAL_KNIGHT) ||
        (ch->cabal == CABAL_SHALAFI && victim->cabal == CABAL_BATTLE) ||
        (ch->cabal == CABAL_BATTLE && victim->cabal == CABAL_SHALAFI))
    {
        act_color("$CSen $s kabal�ndan nefret ediyorsun, onun grubuna nas�l gireceksin?!$c",
            ch, NULL, victim, TO_VICT, POS_SLEEPING, CLR_RED);
        act_color("$CSen $S kabal�ndan nefret ediyorsun, onu grubuna nas�l alabilirsin?!$c", ch, NULL, victim, TO_CHAR, POS_SLEEPING, CLR_RED);
        ch->pcdata->rk_puani -= 20;
        return;
    }


    victim->leader = ch;
    act_color("$C$N $s grubuna giriyor.$c", ch, NULL, victim, TO_NOTVICT,
        POS_SLEEPING, CLR_YELLOW);
    act_color("$CSen $s grubuna giriyorsun.$c", ch, NULL, victim, TO_VICT,
        POS_SLEEPING, CLR_YELLOW);
    act_color("$C$N senin grubuna giriyor.$c", ch, NULL, victim, TO_CHAR,
        POS_SLEEPING, CLR_BLUE);
    return;

}



/*
 * 'Split' originally by Gnort, God of Chaos.
 */
void do_split(CHAR_DATA* ch, char* argument)
{
    char buf[MAX_STRING_LENGTH];
    char arg1[MAX_INPUT_LENGTH];
    CHAR_DATA* gch;
    int members;
    int amount_silver = 0;
    int share_silver;
    int extra_silver;

    one_argument(argument, arg1);

    if (arg1[0] == '\0')
    {
        printf_to_char(ch, "Ne kadar da��tacaks�n?\n\r");
        return;
    }

    amount_silver = atoi(arg1);

    if (amount_silver < 0)
    {
        printf_to_char(ch, "Grubun bundan ho�lanmaz.\n\r");
        return;
    }

    if (amount_silver == 0)
    {
        printf_to_char(ch, "S�f�r sikke da��tt�n ve kimse �akozlamad�.\n\r");
        return;
    }

    if (ch->silver < amount_silver)
    {
        printf_to_char(ch, "�zerinde bu kadar yok.\n\r");
        return;
    }

    members = 0;
    for (gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room)
    {
        if (is_same_group(gch, ch) && !IS_AFFECTED(gch, AFF_CHARM))
            members++;
    }

    if (members < 2)
    {
        printf_to_char(ch, "Sende kals�n.\n\r");
        return;
    }

    share_silver = amount_silver / members;
    extra_silver = amount_silver % members;

    if (share_silver == 0)
    {
        printf_to_char(ch, "Pis cimri.\n\r");
        return;
    }

    ch->silver -= amount_silver;
    ch->silver += share_silver + extra_silver;

    if (share_silver > 0)
    {
        printf_to_char(ch,
            "Sen %d ak�e da��tt�n. Senin pay�na %d ak�e d��t�.\n\r",
            amount_silver, share_silver + extra_silver);
    }

    sprintf(buf, "$n %d ak�e da��tt�. Senin pay�na %d ak�e d��t�.", amount_silver, share_silver);


    for (gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room)
    {
        if (gch != ch && is_same_group(gch, ch) && !IS_AFFECTED(gch, AFF_CHARM))
        {
            act(buf, ch, NULL, gch, TO_VICT);
            gch->silver += share_silver;
        }
    }

    return;
}



void do_gtell(CHAR_DATA* ch, char* argument)
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA* gch;
    int i;

    if (argument[0] == '\0')
    {
        printf_to_char(ch, "Gruba ne anlatacaks�n?\n\r");
        return;
    }

    if (IS_SET(ch->comm, COMM_NOTELL))
    {
        printf_to_char(ch, "Mesaj�n ula�mad�!\n\r");
        return;
    }

    write_channel_log(ch, NULL, KANAL_GSOYLE, argument);

    if (is_affected(ch, gsn_garble))
        garble(buf, argument);
    else
        strcpy(buf, argument);

    ch->pcdata->rk_puani -= 1;


    /*
     * Note use of printf_to_char, so gtell works on sleepers.
     */

    for (i = 0, gch = char_list; gch != NULL; gch = gch->next)
    {
        if (is_same_group(gch, ch) && !is_affected(gch, gsn_deafen))
        {
            act_color("$C$n gruba '$t' dedi.$c",
                ch, buf, gch, TO_VICT, POS_DEAD, CLR_MAGENTA);
            i++;
        }
    }

    if (i > 1 && !is_affected(ch, gsn_deafen))
        act_color("$CSen gruba '$t' dedin.$c",
            ch, buf, NULL, TO_CHAR, POS_DEAD, CLR_CYAN);
    else printf_to_char(ch, "Kendi kendine konu�may� b�rak.");

    return;
}



/*
 * It is very important that this be an equivalence relation:
 * (1) A ~ A
 * (2) if A ~ B then B ~ A
 * (3) if A ~ B  and B ~ C, then A ~ C
 */
bool is_same_group_old(CHAR_DATA* ach, CHAR_DATA* bch)
{
    if (ach == NULL || bch == NULL)
        return FALSE;

    if (ach->leader != NULL) ach = ach->leader;
    if (bch->leader != NULL) bch = bch->leader;
    return ach == bch;
}


/*
 * New is_same_group by chronos
 */
bool is_same_group(CHAR_DATA* ach, CHAR_DATA* bch)
{
    CHAR_DATA* ch, * vch, * ch_next, * vch_next;
    int count, vcount;

    count = vcount = 0;
    for (ch = ach; ch != NULL; ch = ch_next)
    {
        ch_next = ch->leader;
        for (vch = bch; vch != NULL; vch = vch_next)
        {
            vch_next = vch->leader;
            if (ch == vch) return TRUE;
            if (++vcount > 6) break;	/* cyclic loop! */
        }
        if (++count > 6) break;	/* cyclic loop! */
    }
    return FALSE;
}


void do_cb(CHAR_DATA* ch, char* argument)
{
    DESCRIPTOR_DATA* d;
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_INPUT_LENGTH];

    if (!(ch->cabal))
    {
        printf_to_char(ch, "Bir kabala �ye de�ilsin.\n\r");
        return;
    }

    sprintf(buf, "[%s] $n: $C$t$c", cabal_table[ch->cabal].short_name);

    if (is_affected(ch, gsn_garble))
        garble(buf2, argument);
    else
        strcpy(buf2, argument);

    if (!IS_NPC(ch))
        ch->pcdata->rk_puani -= 2;

    if (!is_affected(ch, gsn_deafen))
        act_color(buf, ch, argument, NULL, TO_CHAR, POS_DEAD, CLR_BROWN);

    for (d = descriptor_list; d != NULL; d = d->next)
    {
        if (d->connected == CON_PLAYING &&
            (d->character->cabal == ch->cabal) &&
            /*             !IS_SET(d->character->comm,COMM_NOCB) &&   */
            !is_affected(d->character, gsn_deafen))
        {
            act_color(buf,
                ch, buf2, d->character, TO_VICT, POS_DEAD, CLR_BROWN);
        }
    }

    return;
}

void do_pray(CHAR_DATA* ch, char* argument)
{
    DESCRIPTOR_DATA* d;

    if (IS_SET(ch->comm, COMM_NOCHANNELS))
    {
        printf_to_char(ch, "Tanr�lar seni dinlemek istemiyorlar.");
        return;
    }

    printf_to_char(ch, "Y�z�n� g�ky�z�ne d�n�p kadimlerin yard�m� i�in dua ediyorsun!\n\r");
    printf_to_char(ch, "Bu bir duygu de�il, �l�ms�zlerin diyar�na uzanan bir k�v�lc�m.\n\r");

    for (d = descriptor_list; d != NULL; d = d->next)
    {
        if (d->connected == CON_PLAYING && IS_IMMORTAL(d->character) &&
            !IS_SET(d->character->comm, COMM_NOWIZ))
        {
            if (argument[0] == '\0')
            {
                act_color("$C$n DUA ediyor: Oyunda hi� tanr� var m�?$c",
                    ch, argument, d->character, TO_VICT, POS_DEAD, CLR_CYAN);
            }
            else
            {
                act_color("$C$n DUA ediyor: $t$c",
                    ch, argument, d->character, TO_VICT, POS_DEAD, CLR_CYAN);
            }
        }
    }
    return;
}

char char_lang_lookup(char c) {
    int i;
    for (i = 0; translation_table[i].common[0] != '\0' && translation_table[i].language[0] != '\0'; i++) {
        if (translation_table[i].common[0] == c) {
            return translation_table[i].language[0];
        }
    }
    return c; // Return the original character if not found
}

/*
 * ch says
 * victim hears
 */
char* translate(CHAR_DATA* ch, CHAR_DATA* victim, char* argument) {
    static char trans[MAX_STRING_LENGTH];
    char translated_buf[MAX_STRING_LENGTH]; // Buffer for translation
    int i, length;

    // Early return for simple cases
    if (*argument == '\0' || !ch || !victim || IS_NPC(ch) || IS_NPC(victim) ||
        IS_IMMORTAL(ch) || IS_IMMORTAL(victim) || ch->language == LANG_COMMON ||
        ch->language == race_table[ORG_RACE(victim)].language) {
        if (IS_IMMORTAL(victim)) {
            snprintf(trans, sizeof(trans), "(%s) %s", language_table[ch->language].name, argument);
        }
        else {
            strncpy(trans, argument, sizeof(trans) - 1);
            trans[sizeof(trans) - 1] = '\0'; // Ensure null termination
        }
        return trans;
    }

    // Calculate maximum possible translated length (including language info)
    size_t max_trans_len = strlen(language_table[ch->language].name) + strlen(argument) + 3; // +3 for "()", space, and null terminator

    // Check if translation would overflow
    if (max_trans_len >= sizeof(trans)) {
        // Handle overflow: truncate or use dynamic buffer
        // ... (Implement appropriate overflow handling)
        return trans; // Return empty or truncated string for now
    }

    // Translate the argument character by character
    for (i = 0; argument[i] != '\0' && i < sizeof(translated_buf) - 1; i++) {
        translated_buf[i] = char_lang_lookup(argument[i]);
    }
    translated_buf[i] = '\0'; // Ensure null termination

    // Safely format the translated text with language info
    int result = snprintf(trans, sizeof(trans), "(%s) %s", language_table[ch->language].name, translated_buf);
    if (result >= sizeof(trans)) {
        // Handle overflow: truncate or use dynamic buffer
       // ... (Implement appropriate overflow handling)
    }
    return trans;
}

void do_speak(CHAR_DATA* ch, char* argument)
{
    char arg[MAX_INPUT_LENGTH];
    int language;

    if (IS_NPC(ch)) return;

    argument = one_argument(argument, arg);
    if (arg[0] == '\0')
    {
        printf_to_char(ch, "�u an %s dili konu�uyorsun.\n\r",
            language_table[ch->language].name);
        printf_to_char(ch, "You can speak :\n\r");
        printf_to_char(ch, "       ortak, %s\n\r",
            language_table[race_table[ORG_RACE(ch)].language].name);
        return;
    }

    language = lang_lookup(arg);

    if (language == -1)
    {
        printf_to_char(ch, "B�yle bir dil duymad�n.\n\r");
        return;
    }

    if (language >= MAX_LANGUAGE)
        ch->language = race_table[ORG_RACE(ch)].language;
    else ch->language = language;

    printf_to_char(ch, "Art�k %s dili konu�uyorsun.\n\r", language_table[ch->language].name);
}

/* Thanx zihni@karmi.emu.edu.tr for the code of do_judge */
void do_judge(CHAR_DATA* ch, char* argument)
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA* victim;

    one_argument(argument, arg);

    if (ch->cabal != CABAL_RULER)
    {
        printf_to_char(ch, "H�?\n\r");
        return;
    }

    if (!cabal_ok(ch, gsn_judge)) return;

    if (arg[0] == '\0')
    {
        printf_to_char(ch, "Kimi yarg�layacaks�n?\n\r");
        return;
    }

    /* judge thru world */
    if ((victim = get_char_world(ch, arg)) == NULL)
    {
        printf_to_char(ch, "Burada de�il.\n\r");
        return;
    }


    if (IS_NPC(victim))
    {
        printf_to_char(ch, "Bir mob olmaz.\n\r");
        return;
    }

    if (IS_IMMORTAL(victim) && !IS_IMMORTAL(ch))
    {
        printf_to_char(ch, "�l�ms�zleri yarg�layabilecek g�c� kendinde g�remiyorsun.\n\r");
        return;
    }

    printf_to_char(ch, "%s'in eti�i %s ve y�nelimi %s.\n\r",
        victim->name,
        victim->ethos == 1 ? "T�ze" :
        victim->ethos == 2 ? "Yans�z" :
        victim->ethos == 3 ? "Kaos" : "bilinmiyor",
        IS_GOOD(victim) ? "�yi" :
        IS_EVIL(victim) ? "Kem" : "Yans�z");
    return;
}

void do_remor(CHAR_DATA* ch, char* argument)
{
    printf_to_char(ch, "Yeniya�am istiyorsan eksiksiz yaz.\n\r");
    return;
}

void do_remort(CHAR_DATA* ch, char* argument)
{
    DESCRIPTOR_DATA* d;
    char remstr[MAX_INPUT_LENGTH];
    char mkstr[MAX_INPUT_LENGTH];
    char pbuf[MAX_STRING_LENGTH];
    char name[MAX_STRING_LENGTH];
    int banks, qp, silver, old_tra, old_pra, rkp;

    if (IS_NPC(ch) || (d = ch->desc) == NULL)
        return;

    if (ch->level != LEVEL_HERO)
    {
        printf_to_char(ch, "Yeniya�am i�in KAHRAMAN olmal�s�n.\n\r");
        return;
    }

    if (!IS_SET(ch->act, PLR_CANREMORT) && !IS_SET(ch->act, PLR_REMORTED))
    {
        printf_to_char(ch, "Yeniya�am i�in �l�ms�zlerden izin almal�s�n.\n\r");
        return;
    }

    if (argument[0] != '\0')
    {
        if (!ch->pcdata->confirm_remort)
            printf_to_char(ch, "Sadece yeniya�am yaz, arg�man kullanma.\n\r");
        ch->pcdata->confirm_remort = FALSE;
        return;
    }

    if (ch->pcdata->confirm_remort)
    {
        SET_BIT(ch->act, PLR_REMORTED);
        printf_to_char(ch, "\n\r...YEN�YA�AM...\n\r");
        printf_to_char(ch, "Yeni �rk, s�n�f ve zarlarla yeni bir karakter yaratacaks�n.\n\r");
        printf_to_char(ch, "Bu s�rada muddan koparsan veya mud ��kerse:\n\r");
        printf_to_char(ch, "    AYNI �S�MLE YEN� B�R KARAKTER YARAT VE �L�MS�ZLERE DURUMU B�LD�R.\n\r");
        printf_to_char(ch, "Unutma ki, eski karaktere ait a�a��daki �zellikler aynen korunur:\n\r");
        printf_to_char(ch, "        bankadakiler dahil t�m ak�e\n\r");
        printf_to_char(ch, "        pratik, e�itim seanslar� ve g�rev puan�\n\r");
        printf_to_char(ch, "Yeni ya�am�nda 6 y�z�k takabileceksin.\n\r");
        printf_to_char(ch, "             Ve fazladan 10 e�itim seans�n olacak.\n\r");

        sprintf(pbuf, "%s", ch->pcdata->pwd);
        sprintf(remstr, "%s%s", PLAYER_DIR, capitalize(ch->name));
        sprintf(mkstr, "%s%s", REMORT_DIR, capitalize(ch->name));
        sprintf(name, "%s", ch->name);
        d = ch->desc;
        banks = ch->pcdata->bank_s;
        qp = ch->pcdata->questpoints;
        silver = ch->silver;
        old_tra = ch->train;
        old_pra = ch->practice;
        rkp = ch->pcdata->rk_puani;

        if (!quit_org(ch, argument, TRUE, TRUE))	return;

        link(remstr, mkstr);
        unlink(remstr);

        load_char_obj(d, name);

        ch = d->character;
        ch->pcdata->pwd = str_dup(pbuf);
        d->connected = CON_REMORTING;

        /* give the remorting bonus */
        ch->pcdata->bank_s += banks;
        ch->silver += silver;
        ch->pcdata->questpoints += qp;
        ch->practice += old_pra;
        ch->train += (5 + old_tra);
        ch->pcdata->rk_puani += rkp;

        write_to_buffer(d, "\n\r[Devam etmek i�in ENTER]\n\r", 0);
        return;
    }

    printf_to_char(ch, "Komutu onaylamak i�in tekrar yeniya�am yaz.\n\r");
    printf_to_char(ch, "UYARI: bu komutun geri d�n��� yoktur.\n\r");
    printf_to_char(ch, "Yeniya�am komutunu arg�manla yazmak yeniya�am durumunu iptal edecektir.\n\r");
    printf_to_char(ch, "Unutma ki, eski karaktere ait a�a��daki �zellikler aynen korunur:\n\r");
    printf_to_char(ch, "        bankadakiler dahil t�m ak�e\n\r");
    printf_to_char(ch, "        pratik, e�itim seanslar� ve g�rev puan�\n\r");
    printf_to_char(ch, "Yeni ya�am�nda 6 y�z�k takabileceksin.\n\r");
    printf_to_char(ch, "             Ve fazladan 10 e�itim seans�n olacak.\n\r");
    ch->pcdata->confirm_remort = TRUE;
    wiznet("$N yeniya�am almak �zere.", ch, NULL, 0, 0, get_trust(ch));

}


bool cabal_area_check(CHAR_DATA* ch)
{
    if (ch->in_room == NULL || IS_IMMORTAL(ch))	return FALSE;

    if (ch->cabal != CABAL_RULER &&
        !str_cmp(ch->in_room->area->name, "T�ze Kona��"))
        return TRUE;
    else if (ch->cabal != CABAL_INVADER &&
        !str_cmp(ch->in_room->area->name, "�stila"))
        return TRUE;
    else if (ch->cabal != CABAL_CHAOS &&
        !str_cmp(ch->in_room->area->name, "Kaos"))
        return TRUE;
    else if (ch->cabal != CABAL_SHALAFI &&
        !str_cmp(ch->in_room->area->name, "T�ls�m"))
        return TRUE;
    else if (ch->cabal != CABAL_BATTLE &&
        !str_cmp(ch->in_room->area->name, "�fke"))
        return TRUE;
    else if (ch->cabal != CABAL_KNIGHT &&
        !str_cmp(ch->in_room->area->name, "��valye"))
        return TRUE;
    else if (ch->cabal != CABAL_HUNTER &&
        !str_cmp(ch->in_room->area->name, "Avc�"))
        return TRUE;
    else if (ch->cabal != CABAL_LIONS &&
        !str_cmp(ch->in_room->area->name, "Aslan"))
        return TRUE;
    else return FALSE;
}

bool is_at_cabal_area(CHAR_DATA* ch)
{
    if (ch->in_room == NULL || IS_IMMORTAL(ch))	return FALSE;

    if (!str_cmp(ch->in_room->area->name, "T�ze Kona��") ||
        !str_cmp(ch->in_room->area->name, "�stila") ||
        !str_cmp(ch->in_room->area->name, "Kaos") ||
        !str_cmp(ch->in_room->area->name, "T�ls�m") ||
        !str_cmp(ch->in_room->area->name, "�fke") ||
        !str_cmp(ch->in_room->area->name, "��valye") ||
        !str_cmp(ch->in_room->area->name, "Avc�") ||
        !str_cmp(ch->in_room->area->name, "Aslan"))
        return TRUE;
    else return FALSE;
}

/*
 * Should be called only in quit_org and reboot_anatolia
 */
void update_total_played(CHAR_DATA* ch)
{
    int l;
    int today = parse_date(current_time);

    /* add the player's new time */
    for (l = 0; l < MAX_TIME_LOG; l++)
    {
        if (today == ch->pcdata->log_date[l])
        {
            ch->pcdata->log_time[l] += parse_time_spec(current_time - ch->logon);
            ch->logon = current_time;
            return;
        }
    }


    /* so new day has started */
    for (l = MAX_TIME_LOG - 1; l > 0; l--)
        ch->pcdata->log_time[l] = ch->pcdata->log_time[l - 1];

    ch->pcdata->log_time[0] = parse_time_spec(current_time - ch->logon);
    ch->logon = current_time;

}
