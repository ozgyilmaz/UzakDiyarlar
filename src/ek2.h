/***************************************************************************
*    Uzak Diyarlar, Rom2.4b6 tabanlý Türkçe Mud                            *
*    http://www.uzakdiyarlar.net                                           *
***************************************************************************/
struct sonek_type
{
	char *ek[5];
};

extern	const	struct	sonek_type	sonek_table[];


bool	bu_harf_unlu_mu		args((char harf));
bool	son_harf_unlu_mu	args((char *sozcuk));
char	son_unlu_harf_hangisi	args((char *sozcuk));
char	*ek_olustur		args((char *sozcuk, char tip));
