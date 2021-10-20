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
 
struct sonek_type
{
	wchar_t *ek[5];
};

extern	const	struct	sonek_type	sonek_table[];


bool	bu_harf_unlu_mu		args((wchar_t harf));
bool	son_harf_unlu_mu	args((wchar_t *sozcuk));
wchar_t	son_unlu_harf_hangisi	args((wchar_t *sozcuk));
wchar_t	*ek_olustur		args((wchar_t *sozcuk, wchar_t tip));
