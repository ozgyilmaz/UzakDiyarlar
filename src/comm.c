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
 *  Merc Diku vMud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Thanks to abaddon for proof-reading our comm.c and pointing out bugs.  *
 *  Any remaining bugs are, of course, our work, not his.  :)              *
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

/*
 * This file contains all of the OS-dependent stuff:
 *   startup, signals, BSD sockets for tcp/ip, i/o, timing.
 *
 * The data flow for input is:
 *    Game_loop ---> Read_from_descriptor ---> Read
 *    Game_loop ---> Read_from_buffer
 *
 * The data flow for output is:
 *    Game_loop ---> Process_Output ---> Write_to_descriptor -> Write
 *
 * The OS-dependent functions are Read_from_descriptor and Write_to_descriptor.
 * -- Furey  26 Jan 1993
 */

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#endif

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <unistd.h>

#include "merc.h"
#include "interp.h"
#include "recycle.h"
#include "tables.h"

#include <sys/wait.h>
#include <stdlib.h>

/* command procedures needed */
DECLARE_DO_FUN(do_help		);
DECLARE_DO_FUN(do_look		);
DECLARE_DO_FUN(do_skills	);
DECLARE_DO_FUN(do_outfit	);
DECLARE_DO_FUN(do_unread	);

bool cabal_area_check   args( (CHAR_DATA *ch) );

/*
 * Malloc debugging stuff.
 */
#if defined(sun)
#undef MALLOC_DEBUG
#endif

#if defined(MALLOC_DEBUG)
#include <malloc.h>
extern	int	malloc_debug	args( ( int  ) );
extern	int	malloc_verify	args( ( void ) );
#endif

int sex_ok( CHAR_DATA *ch , int iclass);
int class_ok( CHAR_DATA *ch , int iclass);


/*
 * Signal handling.
 * Apollo has a problem with __attribute(atomic) in signal.h,
 *   I dance around it.
 */
#if defined(apollo)
#define __attribute(x)
#endif

#if defined(unix)
#include <signal.h>
#endif

#if defined(apollo)
#undef __attribute
#endif



/*
 * Socket and TCP/IP stuff.
 */
#if	defined(macintosh) || defined(MSDOS)
const	char	echo_off_str	[] = { '\0' };
const	char	echo_on_str	[] = { '\0' };
const	char 	go_ahead_str	[] = { '\0' };
#endif

#if	defined(unix)
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "telnet.h"
const	char	echo_off_str	[] = { IAC, WILL, TELOPT_ECHO, '\0' };
const	char	echo_on_str	[] = { IAC, WONT, TELOPT_ECHO, '\0' };
const	char 	go_ahead_str	[] = { IAC, GA, '\0' };
#endif

char *get_stat_alias		args( (CHAR_DATA* ch, int which) );


/*
 * OS-dependent declarations.
 */
#if	defined(_AIX)
#include <sys/select.h>
int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
int	bind		args( ( int s, struct sockaddr *name, int namelen ) );
void	bzero		args( ( char *b, int length ) );
int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
int	listen		args( ( int s, int backlog ) );
int	setsockopt	args( ( int s, int level, int optname, void *optval,
			    int optlen ) );
int	socket		args( ( int domain, int type, int protocol ) );
#endif

#if	defined(apollo)
#include <unistd.h>
void	bzero		args( ( char *b, int length ) );
#endif

#if	defined(__hpux)
int	accept		args( ( int s, void *addr, int *addrlen ) );
int	bind		args( ( int s, const void *addr, int addrlen ) );
void	bzero		args( ( char *b, int length ) );
int	getpeername	args( ( int s, void *addr, int *addrlen ) );
int	getsockname	args( ( int s, void *name, int *addrlen ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
int	listen		args( ( int s, int backlog ) );
int	setsockopt	args( ( int s, int level, int optname,
 				const void *optval, int optlen ) );
int	socket		args( ( int domain, int type, int protocol ) );
#endif

#if	defined(interactive)
#include <net/errno.h>
#include <sys/fnctl.h>
#endif

#if	defined(linux)
/* 
    Linux shouldn't need these. If you have a problem compiling, try
    uncommenting these functions.
*/
/*
int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
int	bind		args( ( int s, struct sockaddr *name, int namelen ) );
int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int	listen		args( ( int s, int backlog ) );
*/

int	close		args( ( int fd ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
int	read		args( ( int fd, char *buf, int nbyte ) );
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );
int	socket		args( ( int domain, int type, int protocol ) );
//int	write		args( ( int fd, char *buf, int nbyte ) );
ssize_t write	args( ( int fd, const void *buf, size_t nbyte) );
#endif

#if	defined(macintosh)
#include <console.h>
#include <fcntl.h>
#include <unix.h>
struct	timeval
{
	time_t	tv_sec;
	time_t	tv_usec;
};
#if	!defined(isascii)
#define	isascii(c)		( (c) < 0200 )
#endif
static	long			theKeys	[4];

int	gettimeofday		args( ( struct timeval *tp, void *tzp ) );
#endif

#if	defined(MIPS_OS)
extern	int		errno;
#endif

#if	defined(MSDOS)
int	gettimeofday	args( ( struct timeval *tp, void *tzp ) );
int	kbhit		args( ( void ) );
#endif

#if	defined(NeXT)
int	close		args( ( int fd ) );
int	fcntl		args( ( int fd, int cmd, int arg ) );
#if	!defined(htons)
u_short	htons		args( ( u_short hostshort ) );
#endif
#if	!defined(ntohl)
u_long	ntohl		args( ( u_long hostlong ) );
#endif
int	read		args( ( int fd, char *buf, int nbyte ) );
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );
int	write		args( ( int fd, char *buf, int nbyte ) );
#endif

#if	defined(sequent)
int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
int	bind		args( ( int s, struct sockaddr *name, int namelen ) );
int	close		args( ( int fd ) );
int	fcntl		args( ( int fd, int cmd, int arg ) );
int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
#if	!defined(htons)
u_short	htons		args( ( u_short hostshort ) );
#endif
int	listen		args( ( int s, int backlog ) );
#if	!defined(ntohl)
u_long	ntohl		args( ( u_long hostlong ) );
#endif
int	read		args( ( int fd, char *buf, int nbyte ) );
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );
int	setsockopt	args( ( int s, int level, int optname, caddr_t optval,
			    int optlen ) );
int	socket		args( ( int domain, int type, int protocol ) );
int	write		args( ( int fd, char *buf, int nbyte ) );
#endif

/* This includes Solaris Sys V as well */
#if defined(sun)
int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
int	bind		args( ( int s, struct sockaddr *name, int namelen ) );
void	bzero		args( ( char *b, int length ) );
int	close		args( ( int fd ) );
int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int	listen		args( ( int s, int backlog ) );
int	read		args( ( int fd, char *buf, int nbyte ) );
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );

#if !defined(__SVR4)
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );

#if defined(SYSV)
int setsockopt		args( ( int s, int level, int optname,
			    const char *optval, int optlen ) );
#else
int	setsockopt	args( ( int s, int level, int optname, void *optval,
			    int optlen ) );
#endif
#endif
int	socket		args( ( int domain, int type, int protocol ) );
int	write		args( ( int fd, char *buf, int nbyte ) );
#endif

#if defined(ultrix)
int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
int	bind		args( ( int s, struct sockaddr *name, int namelen ) );
void	bzero		args( ( char *b, int length ) );
int	close		args( ( int fd ) );
int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
int	listen		args( ( int s, int backlog ) );
int	read		args( ( int fd, char *buf, int nbyte ) );
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );
int	setsockopt	args( ( int s, int level, int optname, void *optval,
			    int optlen ) );
int	socket		args( ( int domain, int type, int protocol ) );
int	write		args( ( int fd, char *buf, int nbyte ) );
#endif



/*
 * Global variables.
 */
DESCRIPTOR_DATA *   descriptor_list;	/* All open descriptors		*/
DESCRIPTOR_DATA *   d_next;		/* Next descriptor in loop	*/
FILE *		    fpReserve;		/* Reserved file handle		*/
bool		    god;		/* All new chars are gods!	*/
bool		    merc_down;		/* Shutdown			*/
bool		    wizlock;		/* Game is wizlocked		*/
bool		    newlock;		/* Game is newlocked		*/
time_t		    boot_time;		/* time of boot */	
time_t		    current_time;	/* time of this pulse */	
 


/*
 * OS-dependent local functions.
 */
#if defined(macintosh) || defined(MSDOS)
void	game_loop_mac_msdos	args( ( void ) );
bool	read_from_descriptor	args( ( DESCRIPTOR_DATA *d ) );
bool	write_to_descriptor	args( ( int desc, char *txt, int length ) );
#endif

#if defined(unix)
void	game_loop_unix		args( ( int control ) );
int	init_socket		args( ( int port ) );
void	init_descriptor		args( ( int control ) );
bool	read_from_descriptor	args( ( DESCRIPTOR_DATA *d ) );
bool	write_to_descriptor	args( ( int desc, char *txt, int length ) );
#endif




/*
 * Other local functions (OS-independent).
 */
bool	check_parse_name	args( ( char *name ) );
bool	check_reconnect		args( ( DESCRIPTOR_DATA *d, char *name,
				    bool fConn ) );
bool	check_playing		args( ( DESCRIPTOR_DATA *d, char *name ) );
int	main			args( ( int argc, char **argv ) );
void	nanny			args( ( DESCRIPTOR_DATA *d, char *argument ) );
bool	process_output		args( ( DESCRIPTOR_DATA *d, bool fPrompt ) );
void	read_from_buffer	args( ( DESCRIPTOR_DATA *d ) );
void	stop_idling		args( ( CHAR_DATA *ch ) );
void    bust_a_prompt           args( ( CHAR_DATA *ch ) );
void	exit_function(int signum);
int 	log_area_popularity(void);

int main( int argc, char **argv )
{
    struct timeval now_time;
    int port;

#if defined(unix)
    int control;
#endif

    /* Don't leave save processes stranded */
    signal( SIGQUIT, exit_function );
    /*
     * Memory debugging if needed.
     */
#if defined(MALLOC_DEBUG)
    malloc_debug( 2 );
#endif

    /*
     * Init time.
     */
    gettimeofday( &now_time, NULL );
    boot_time = current_time = (time_t) now_time.tv_sec;

    /*
     * Macintosh console initialization.
     */
#if defined(macintosh)
    console_options.nrows = 31;
    cshow( stdout );
    csetmode( C_RAW, stdin );
    cecho2file( "log file", 1, stderr );
#endif

    /*
     * Reserve one channel for our use.
     */
    if ( ( fpReserve = fopen( NULL_FILE, "r" ) ) == NULL )
    {
	perror( NULL_FILE );
	exit( 1 );
    }

    /*
     * Get the port number.
     */
    port = 6001;
    if ( argc > 1 )
    {
	if ( !is_number( argv[1] ) )
	{
	    fprintf( stderr, "Usage: %s [port #]\n", argv[0] );
	    exit( 1 );
	}
	else if ( ( port = atoi( argv[1] ) ) <= 1024 )
	{
	    fprintf( stderr, "Port number must be above 1024.\n" );
	    exit( 1 );
	}
    }

    /*
     * Run the game.
     */
    
#if defined(macintosh) || defined(MSDOS)
    boot_db( );
    log_string( "ready to rock." );
    game_loop_mac_msdos( );
#endif

#if defined(unix)
    control = init_socket( port );
    boot_db( );
    sprintf( log_buf, "ready to rock on port %d.", port );
    log_string( log_buf );
    game_loop_unix( control );
    close (control);
#endif

    log_area_popularity();

    /*
     * That's all, folks.
     */
    log_string( "Normal termination of game." );
    exit( 0 );
    return 0;
}



#if defined(unix)
int init_socket( int port )
{
    static struct sockaddr_in sa_zero;
    struct sockaddr_in sa;
    int x = 1;
    int fd;

    if ( ( fd = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 )
    {
	perror( "Init_socket: socket" );
	exit( 1 );
    }

    if ( setsockopt( fd, SOL_SOCKET, SO_REUSEADDR,
    (char *) &x, sizeof(x) ) < 0 )
    {
	perror( "Init_socket: SO_REUSEADDR" );
	close(fd);
	exit( 1 );
    }

#if defined(SO_DONTLINGER) && !defined(SYSV)
    {
	struct	linger	ld;

	ld.l_onoff  = 1;
	ld.l_linger = 1000;

	if ( setsockopt( fd, SOL_SOCKET, SO_DONTLINGER,
	(char *) &ld, sizeof(ld) ) < 0 )
	{
	    perror( "Init_socket: SO_DONTLINGER" );
	    close(fd);
	    exit( 1 );
	}
    }
#endif

    sa		    = sa_zero;
    sa.sin_family   = AF_INET;
    sa.sin_port	    = htons( port );

    if ( bind( fd, (struct sockaddr *) &sa, sizeof(sa) ) < 0 )
    {
	perror("Init socket: bind" );
	close(fd);
	exit(1);
    }


    if ( listen( fd, 3 ) < 0 )
    {
	perror("Init socket: listen");
	close(fd);
	exit(1);
    }

    return fd;
}
#endif



#if defined(macintosh) || defined(MSDOS)
void game_loop_mac_msdos( void )
{
    struct timeval last_time;
    struct timeval now_time;
    static DESCRIPTOR_DATA dcon;

    gettimeofday( &last_time, NULL );
    current_time = (time_t) last_time.tv_sec;

    /*
     * New_descriptor analogue.
     */
    dcon.descriptor	= 0;
    dcon.connected	= CON_GET_NAME;
    dcon.host		= str_dup( "localhost" );
    dcon.outsize	= 2000;
    dcon.outbuf		= alloc_mem( dcon.outsize );
    dcon.next		= descriptor_list;
    dcon.showstr_head	= NULL;
    dcon.showstr_point	= NULL;
    descriptor_list	= &dcon;

    /*
     * Send the greeting.
     */
    {
	extern char * help_greeting;
	if ( help_greeting[0] == '.' )
	    write_to_buffer( &dcon, help_greeting+1, 0 );
	else
	    write_to_buffer( &dcon, help_greeting  , 0 );
    }

    /* Main loop */
    while ( !merc_down )
    {
	DESCRIPTOR_DATA *d;

	/*
	 * Process input.
	 */
	for ( d = descriptor_list; d != NULL; d = d_next )
	{
	    d_next	= d->next;
	    d->fcommand	= FALSE;

#if defined(MSDOS)
	    if ( kbhit( ) )
#endif
	    {
		if ( d->character != NULL )
		    d->character->timer = 0;
		if ( !read_from_descriptor( d ) )
		{
		    if ( d->character != NULL && d->connected == CON_PLAYING)
			save_char_obj( d->character );
		    d->outtop	= 0;
		    close_socket( d );
		    continue;
		}
	    }

	    if (d->character != NULL && d->character->daze > 0)
		--d->character->daze;

	    if ( d->character != NULL && d->character->wait > 0 )
	    {
		--d->character->wait;
		continue;
	    }

	    read_from_buffer( d );
	    if ( d->incomm[0] != '\0' )
	    {
		d->fcommand	= TRUE;
		stop_idling( d->character );

		if ( d->connected == CON_PLAYING )
		    substitute_alias( d, d->incomm );
		else
		    nanny( d, d->incomm );

		d->incomm[0]	= '\0';
	    }
	}



	/*
	 * Autonomous game motion.
	 */
	update_handler( );



	/*
	 * Output.
	 */
	for ( d = descriptor_list; d != NULL; d = d_next )
	{
	    d_next = d->next;

	    if ( ( d->fcommand || d->outtop > 0 ) )
	    {
		if ( !process_output( d, TRUE ) )
		{
		    if ( d->character != NULL && d->character->level > 1 && d->connected == CON_PLAYING )
			save_char_obj( d->character );
		    d->outtop	= 0;
		    close_socket( d );
		}
	    }
	}



	/*
	 * Synchronize to a clock.
	 * Busy wait (blargh).
	 */
	now_time = last_time;
	for ( ; ; )
	{
	    int delta;

#if defined(MSDOS)
	    if ( kbhit( ) )
#endif
	    {
		if ( dcon.character != NULL )
		    dcon.character->timer = 0;
		if ( !read_from_descriptor( &dcon ) )
		{
		    if ( dcon.character != NULL && d->character->level > 1 && d->connected == CON_PLAYING)
			save_char_obj( d->character );
		    dcon.outtop	= 0;
		    close_socket( &dcon );
		}
#if defined(MSDOS)
		break;
#endif
	    }

	    gettimeofday( &now_time, NULL );
	    delta = ( now_time.tv_sec  - last_time.tv_sec  ) * 1000 * 1000
		  + ( now_time.tv_usec - last_time.tv_usec );
	    if ( delta >= 1000000 / PULSE_PER_SCD )
		break;
	}
	last_time    = now_time;
	current_time = (time_t) last_time.tv_sec;
    }

    return;
}
#endif

void crash_chronos (int sig)
{
 char buf[MAX_STRING_LENGTH];
 DESCRIPTOR_DATA *d, *d_next;
 CHAR_DATA *ch;

 log_string( "Core dumped.");
 sprintf(buf,"The core with signal %d",sig);
 bug(buf,0);
 for ( d = descriptor_list; d != NULL; d = d_next )
	{
	    d_next	= d->next;
            ch = d->original ? d->original : d->character;
            if (IS_NPC(ch))  continue;
	             save_char_obj (ch);	
	    sprintf(buf,"%s is saved",ch->name);	
	    log_string(buf);
	    write_to_descriptor(d->descriptor,"\007Rebooting By Server!!\007\n\r",0);
	    write_to_descriptor(d->descriptor,"Saving.Remember that Rom has automatic saving now.\n\r",0);
	    sprintf(buf,"%s last command %s",ch->name,ch->desc->inlast);    
	    bug(buf,0);
	}
    sprintf(buf,"SUCCESSFUL HANDLING!"); 
    bug(buf,0);	
 return;
}

#if defined(unix)
void game_loop_unix( int control )
{
    static struct timeval null_time;
    struct timeval last_time;
 
    signal( SIGPIPE, SIG_IGN );
    gettimeofday( &last_time, NULL );
    current_time = (time_t) last_time.tv_sec;

    /* Main loop */
    while ( !merc_down )
    {
	fd_set in_set;
	fd_set out_set;
	fd_set exc_set;
	DESCRIPTOR_DATA *d;
	int maxdesc;

#if defined(MALLOC_DEBUG)
	if ( malloc_verify( ) != 1 )
	    abort( );
#endif

	/*
	 * Poll all active descriptors.
	 */
	FD_ZERO( &in_set  );
	FD_ZERO( &out_set );
	FD_ZERO( &exc_set );
	FD_SET( control, &in_set );
	maxdesc	= control;
	for ( d = descriptor_list; d; d = d->next )
	{
	    maxdesc = UMAX( maxdesc, d->descriptor );
	    FD_SET( d->descriptor, &in_set  );
	    FD_SET( d->descriptor, &out_set );
	    FD_SET( d->descriptor, &exc_set );
	}

	if ( select( maxdesc+1, &in_set, &out_set, &exc_set, &null_time ) < 0 )
	{
	    perror( "Game_loop: select: poll" );
	    exit( 1 );
	}

	/*
	 * New connection?
	 */
	if ( FD_ISSET( control, &in_set ) )
	    init_descriptor( control );

	/*
	 * Kick out the freaky folks.
	 */
	for ( d = descriptor_list; d != NULL; d = d_next )
	{
	    d_next = d->next;   
	    if ( FD_ISSET( d->descriptor, &exc_set ) )
	    {
		FD_CLR( d->descriptor, &in_set  );
		FD_CLR( d->descriptor, &out_set );
		if ( d->character && d->character->level > 1 && d->connected == CON_PLAYING)
		    save_char_obj( d->character );
		d->outtop	= 0;
		close_socket( d );
	    }
	}

	/*
	 * Process input.
	 */
	for ( d = descriptor_list; d != NULL; d = d_next )
	{
	    d_next	= d->next;
	    d->fcommand	= FALSE;

	    if ( FD_ISSET( d->descriptor, &in_set ) )
	    {
		if ( d->character != NULL )
		    d->character->timer = 0;
		if ( !read_from_descriptor( d ) )
		{
		    FD_CLR( d->descriptor, &out_set );
		    if ( d->character != NULL && d->character->level > 1 && d->connected == CON_PLAYING)
			save_char_obj( d->character );
		    d->outtop	= 0;
		    close_socket( d );
		    continue;
		}
	    }

	    if (d->character != NULL && d->character->daze > 0)
		--d->character->daze;

	    if ( d->character != NULL && d->character->wait > 0 )
	    {
		--d->character->wait;
		continue;
	    }

	    read_from_buffer( d );
	    if ( d->incomm[0] != '\0' )
	    {
		d->fcommand	= TRUE;
		stop_idling( d->character );

		if (d->showstr_point)
		    show_string(d,d->incomm);
		else if ( d->connected == CON_PLAYING )
		    substitute_alias( d, d->incomm );
		else
		    nanny( d, d->incomm );

		d->incomm[0]	= '\0';
	    }
	}



	/*
	 * Autonomous game motion.
	 */
	update_handler( );



	/*
	 * Output.
	 */
	for ( d = descriptor_list; d != NULL; d = d_next )
	{
	    d_next = d->next;

	    if ( ( d->fcommand || d->outtop > 0 )
	    &&   FD_ISSET(d->descriptor, &out_set) )
	    {
		if ( !process_output( d, TRUE ) )
		{
		    if ( d->character != NULL && d->character->level > 1 && d->connected == CON_PLAYING)
			save_char_obj( d->character );
		    d->outtop	= 0;
		    close_socket( d );
		}
	    }
	}



	/*
	 * Synchronize to a clock.
	 * Sleep( last_time + 1/PULSE_PER_SCD - now ).
	 * Careful here of signed versus unsigned arithmetic.
	 */
	{
	    struct timeval now_time;
	    long secDelta;
	    long usecDelta;

	    gettimeofday( &now_time, NULL );
	    usecDelta	= ((int) last_time.tv_usec) - ((int) now_time.tv_usec)
			+ 1000000 / PULSE_PER_SCD;
	    secDelta	= ((int) last_time.tv_sec ) - ((int) now_time.tv_sec );
	    while ( usecDelta < 0 )
	    {
		usecDelta += 1000000;
		secDelta  -= 1;
	    }

	    while ( usecDelta >= 1000000 )
	    {
		usecDelta -= 1000000;
		secDelta  += 1;
	    }

	    if ( secDelta > 0 || ( secDelta == 0 && usecDelta > 0 ) )
	    {
		struct timeval stall_time;

		stall_time.tv_usec = usecDelta;
		stall_time.tv_sec  = secDelta;
		if ( select( 0, NULL, NULL, NULL, &stall_time ) < 0 )
		{
		    perror( "Game_loop: select: stall" );
		    exit( 1 );
		}
	    }
	}

	gettimeofday( &last_time, NULL );
	current_time = (time_t) last_time.tv_sec;
    }

    return;
}
#endif



#if defined(unix)
void init_descriptor( int control )
{
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *dnew;
    struct sockaddr_in sock;
    struct hostent *from;
    int desc;
    socklen_t size;

    size = sizeof(sock);
    getsockname( control, (struct sockaddr *) &sock, &size );
    if ( ( desc = accept( control, (struct sockaddr *) &sock, &size) ) < 0 )
    {
	perror( "New_descriptor: accept" );
	return;
    }

#if !defined(FNDELAY)
#define FNDELAY O_NDELAY
#endif

    if ( fcntl( desc, F_SETFL, FNDELAY ) == -1 )
    {
	perror( "New_descriptor: fcntl: FNDELAY" );
	return;
    }
    /*
     * Cons a new descriptor.
     */
    dnew = new_descriptor();

    dnew->descriptor	= desc;
    dnew->connected	= CON_GET_NAME;
    dnew->showstr_head	= NULL;
    dnew->showstr_point = NULL;
    dnew->outsize	= 2000;
    dnew->outbuf	= (char *)alloc_mem( dnew->outsize );

    size = sizeof(sock);
    if ( getpeername( desc, (struct sockaddr *) &sock, &size ) < 0 )
    {
	perror( "New_descriptor: getpeername" );
	dnew->host = str_dup( "(unknown)" );
    }
    else
    {
	/*
	 * Would be nice to use inet_ntoa here but it takes a struct arg,
	 * which ain't very compatible between gcc and system libraries.
	 */
	int addr;

	addr = ntohl( sock.sin_addr.s_addr );
	sprintf( buf, "%d.%d.%d.%d",
	    ( addr >> 24 ) & 0xFF, ( addr >> 16 ) & 0xFF,
	    ( addr >>  8 ) & 0xFF, ( addr       ) & 0xFF
	    );
	sprintf( log_buf, "Sock.sinaddr:  %s", buf );
	log_string( log_buf );
	from = gethostbyaddr( (char *) &sock.sin_addr,
	    sizeof(sock.sin_addr), AF_INET );
	dnew->host = str_dup( from ? from->h_name : buf );
    }
	
    /*
     * Swiftest: I added the following to ban sites.  I don't
     * endorse banning of sites, but Copper has few descriptors now
     * and some people from certain sites keep abusing access by
     * using automated 'autodialers' and leaving connections hanging.
     *
     * Furey: added suffix check by request of Nickel of HiddenWorlds.
     */
    if ( check_ban(dnew->host,BAN_ALL))
    {
	write_to_descriptor( desc,
	    "Your site has been banned from this mud.\n\r", 0 );
	close( desc );
	free_descriptor(dnew);
	return;
    }
    /*
     * Init descriptor data.
     */
    dnew->next			= descriptor_list;
    descriptor_list		= dnew;

    /*
     * Send the greeting.
     */
    {
	extern char * help_greeting;
        write_to_buffer( dnew, "\033[2J\033[0;0H\033[0;37;40m\n\r", 0 );
	if ( help_greeting[0] == '.' )
	    write_to_buffer( dnew, help_greeting+1, 0 );
	else
	    write_to_buffer( dnew, help_greeting  , 0 );
    }

    return;
}
#endif



void close_socket( DESCRIPTOR_DATA *dclose )
{
    CHAR_DATA *ch;

    if ( dclose->outtop > 0 )
	process_output( dclose, FALSE );

    if ( dclose->snoop_by != NULL )
    {
	write_to_buffer( dclose->snoop_by,
	    "Your victim has left the game.\n\r", 0 );
    }

    {
	DESCRIPTOR_DATA *d;

	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    if ( d->snoop_by == dclose )
		d->snoop_by = NULL;
	}
    }

    if ( ( ch = dclose->character ) != NULL )
    {
	sprintf( log_buf, "Closing link to %s.", ch->name );
	log_string( log_buf );

	if (ch->pet && 
		( ch->pet->in_room == NULL
		|| ch->pet->in_room == get_room_index(ROOM_VNUM_LIMBO)) )
	{
		char_to_room( ch->pet, get_room_index(ROOM_VNUM_LIMBO));
		extract_char( ch->pet, TRUE);
	}

	if ( dclose->connected == CON_PLAYING )
	{
	    if (!IS_IMMORTAL(ch))
	       act( "$n has lost $s link.", ch, NULL, NULL, TO_ROOM );
	    wiznet("Net death has claimed $N.",ch,NULL,WIZ_LINKS,0,0);
	    ch->desc = NULL;
	}
	else
	{
	    free_char( dclose->character );
	}
    }

    if ( d_next == dclose )
	d_next = d_next->next;   

    if ( dclose == descriptor_list )
    {
	descriptor_list = descriptor_list->next;
    }
    else
    {
	DESCRIPTOR_DATA *d;

	for ( d = descriptor_list; d && d->next != dclose; d = d->next )
	    ;
	if ( d != NULL )
	    d->next = dclose->next;
	else
	    bug( "Close_socket: dclose not found.", 0 );
    }

    close( dclose->descriptor );
    free_descriptor(dclose);
#if defined(MSDOS) || defined(macintosh)
    exit(1);
#endif
    return;
}



bool read_from_descriptor( DESCRIPTOR_DATA *d )
{
    size_t iStart;

    /* Hold horses if pending command already. */
    if ( d->incomm[0] != '\0' )
	return TRUE;

    /* Check for overflow. */
    iStart = strlen(d->inbuf);
    if ( iStart >= sizeof(d->inbuf) - 10 )
    {
	sprintf( log_buf, "%s input overflow!", d->host );
	log_string( log_buf );
	write_to_descriptor( d->descriptor,
	    "\n\r*** PUT A LID ON IT!!! ***\n\r", 0 );
	return FALSE;
    }

    /* Snarf input. */
#if defined(macintosh)
    for ( ; ; )
    {
	int c;
	c = getc( stdin );
	if ( c == '\0' || c == EOF )
	    break;
	putc( c, stdout );
	if ( c == '\r' )
	    putc( '\n', stdout );
	d->inbuf[iStart++] = c;
	if ( iStart > sizeof(d->inbuf) - 10 )
	    break;
    }
#endif

#if defined(MSDOS) || defined(unix)
    for ( ; ; )
    {
	int nRead;

	nRead = read( (int)d->descriptor, (void *)(d->inbuf + iStart),
	    sizeof(d->inbuf) - 10 - iStart );
	if ( nRead > 0 )
	{
	    iStart += nRead;
	    if ( d->inbuf[iStart-1] == '\n' || d->inbuf[iStart-1] == '\r' )
		break;
	}
	else if ( nRead == 0 )
	{
	    log_string( "EOF encountered on read." );
#if defined(__hpux)
	    break;
#else
	    return FALSE;
#endif
	}
	else if ( errno == EWOULDBLOCK )
	    break;
	else
	{
	    perror( "Read_from_descriptor" );
	    return FALSE;
	}
    }
#endif

    d->inbuf[iStart] = '\0';
    return TRUE;
}



/*
 * Transfer one line from input buffer to input line.
 */
void read_from_buffer( DESCRIPTOR_DATA *d )
{
    int i, j, k;
    char buf[MAX_STRING_LENGTH];

    /*
     * Hold horses if pending command already.
     */
    if ( d->incomm[0] != '\0' )
	return;

    /*
     * Look for at least one new line.
     */
    for ( i = 0; d->inbuf[i] != '\n' && d->inbuf[i] != '\r'; i++ )
    {
	if ( d->inbuf[i] == '\0' )
	    return;
    }

    /*
     * Canonical input processing.
     */
    for ( i = 0, k = 0; d->inbuf[i] != '\n' && d->inbuf[i] != '\r'; i++ )
    {
	if ( k >= MAX_INPUT_LENGTH - 2 )
	{
	    write_to_descriptor( d->descriptor, "Line too long.\n\r", 0 );

	    /* skip the rest of the line */
	    for ( ; d->inbuf[i] != '\0'; i++ )
	    {
		if ( d->inbuf[i] == '\n' || d->inbuf[i] == '\r' )
		    break;
	    }
	    d->inbuf[i]   = '\n';
	    d->inbuf[i+1] = '\0';
	    break;
	}

	if ( d->inbuf[i] == '\b' && k > 0 )
	    --k;
	else if ( isascii(d->inbuf[i]) && isprint(d->inbuf[i]) )
	    d->incomm[k++] = d->inbuf[i];
    }

    /*
     * Finish off the line.
     */
    if ( k == 0 )
	d->incomm[k++] = ' ';
    d->incomm[k] = '\0';

    /*
     * Deal with bozos with #repeat 1000 ...
     */

    if ( k > 1 || d->incomm[0] == '!' )
    {
    	if ( d->incomm[0] != '!' && strcmp( d->incomm, d->inlast ) )
	{
	    d->repeat = 0;
	}
	else
	{
	    if ( ++d->repeat >= 25 )	/* corrected by chronos */
	    {
		sprintf( log_buf, "%s input spamming!", d->host );
		log_string( log_buf );
             if (d->character != NULL)
	      {
		sprintf(buf,"SPAM SPAM SPAM %s spamming, and OUT!",d->character->name);
		wiznet(buf,d->character,NULL,WIZ_SPAM,0,get_trust(d->character));

		sprintf(buf,"[%s]'s  Inlast:[%s] Incomm:[%s]!",
			d->character->name,d->inlast,d->incomm);
        	wiznet(buf,d->character,NULL,WIZ_SPAM,0,get_trust(d->character));

		d->repeat = 0;

		write_to_descriptor( d->descriptor,
		    "\n\r*** PUT A LID ON IT!!! ***\n\r", 0 );
/*		strcpy( d->incomm, "quit" );	*/
		close_socket( d );	
		return;
	       }
	    }
	}
    }


    /*
     * Do '!' substitution.
     */
    if ( d->incomm[0] == '!' )
	strcpy( d->incomm, d->inlast );
    else
	strcpy( d->inlast, d->incomm );

    /*
     * Shift the input buffer.
     */
    while ( d->inbuf[i] == '\n' || d->inbuf[i] == '\r' )
	i++;
    for ( j = 0; ( d->inbuf[j] = d->inbuf[i+j] ) != '\0'; j++ )
	;
    return;
}



/*
 * Low level output function.
 */

/*
 * Some specials added by KIO 
 */
bool process_output( DESCRIPTOR_DATA *d, bool fPrompt )
{
    extern bool merc_down;

    /*
     * Bust a prompt.
     */
    if (!merc_down && d->showstr_point)
	write_to_buffer(d,"\r[Hit Return to continue]\n\r",0);
    else if (fPrompt && !merc_down && d->connected == CON_PLAYING)
    {
   	CHAR_DATA *ch;
	CHAR_DATA *victim;

	ch = d->character;

        /* battle prompt */
        if ((victim = ch->fighting) != NULL && can_see(ch,victim))
        {
            int percent;
            char wound[100];
	    char buf[MAX_STRING_LENGTH];
 
            if (victim->max_hit > 0)
                percent = victim->hit * 100 / victim->max_hit;
            else
                percent = -1;
  
            if (percent >= 100)
                sprintf(wound,"is in perfect health.");
            else if (percent >= 90)
                sprintf(wound,"has a few scratches.");
            else if (percent >= 75)
                sprintf(wound,"has some small but disgusting cuts.");
            else if (percent >= 50)
                sprintf(wound,"is covered with bleeding wounds.");
            else if (percent >= 30)
                sprintf(wound,"is gushing blood.");
            else if (percent >= 15)
                sprintf(wound,"is writhing in agony.");
            else if (percent >= 0)
                sprintf(wound,"is convulsing on the ground.");
            else
                sprintf(wound,"is nearly dead.");

 
            sprintf(buf,"%s %s \n\r", 
	            IS_NPC(victim) ? victim->short_descr : victim->name,wound);
	    buf[0] = UPPER(buf[0]);
            write_to_buffer( d, buf, 0);
        }


	ch = d->original ? d->original : d->character;
	if (!IS_SET(ch->comm, COMM_COMPACT) )
	    write_to_buffer( d, "\n\r", 2 );


        if ( IS_SET(ch->comm, COMM_PROMPT) )
            bust_a_prompt( d->character );

	if (IS_SET(ch->comm,COMM_TELNET_GA))
	    write_to_buffer(d,go_ahead_str,0);
    }

    /*
     * Short-circuit if nothing to write.
     */
    if ( d->outtop == 0 )
	return TRUE;

    /*
     * Snoop-o-rama.
     */
    if ( d->snoop_by != NULL )
    {
	if (d->character != NULL)
	    write_to_buffer( d->snoop_by, d->character->name,0);
	write_to_buffer( d->snoop_by, "> ", 2 );
	write_to_buffer( d->snoop_by, d->outbuf, d->outtop );
    }

    /*
     * OS-dependent output.
     */
    if ( !write_to_descriptor( d->descriptor, d->outbuf, d->outtop ) )
    {
	d->outtop = 0;
	return FALSE;
    }
    else
    {
	d->outtop = 0;
	return TRUE;
    }
}

/*
 * Bust a prompt (player settable prompt)
 * coded by Morgenes for Aldara Mud
 * bust
 */

void bust_a_prompt( CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    const char *str;
    const char *i;
    char *point;
    char doors[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    EXIT_DATA *pexit;
    bool found;
    const char *dir_name[] = {"N","E","S","W","U","D"};
    int door;
 
    point = buf;
    str = ch->prompt;
    if (str == NULL || str[0] == '\0')
    {
        sprintf( buf, "<%dhp %dm %dmv> %s",
	    ch->hit,ch->mana,ch->move,ch->prefix);
	send_to_char(buf,ch);
	return;
    }

   while( *str != '\0' )
   {
      if( *str != '%' )
      {
         *point++ = *str++;
         continue;
      }
      ++str;
      switch( *str )
      {
         default :
            i = " "; break;
	case 'e':
	    found = FALSE;
	    doors[0] = '\0';
	    for (door = 0; door < 6; door++)
	    {
		if ((pexit = ch->in_room->exit[door]) != NULL
		&&  pexit ->u1.to_room != NULL
		&&  (can_see_room(ch,pexit->u1.to_room)
		||   (IS_AFFECTED(ch,AFF_INFRARED) 
		&&    !IS_AFFECTED(ch,AFF_BLIND)))
		&&  !IS_SET(pexit->exit_info,EX_CLOSED))
		{
		    found = TRUE;
		    strcat(doors,dir_name[door]);
		}
	    }
	    if (!found)
	 	strcat(buf,"none");
	    sprintf(buf2,"%s",doors);
	    i = buf2; break;
 	 case 'c' :
	    sprintf(buf2,"%s","\n\r");
	    i = buf2; break;
/** added from here by KIO   **/
	 case 'n' :
	    sprintf( buf2, "%s", ch->name );
	    i = buf2; break;
	 case 'S' :
	    sprintf( buf2, "%s", (ch->sex == SEX_MALE ? "Male":(!ch->sex ? "None":"Female")));
	    i = buf2; break;
         case 'y' :
            if (ch->hit >= 0) 
           	sprintf( buf2, "%d%%",((100 * ch->hit) / UMAX(1,ch->max_hit)));
	    else
                 sprintf( buf2, "BAD!!");
	    i = buf2; break;
         case 'o' :
	    if ((victim = ch->fighting) != NULL) {
		if (victim->hit >= 0) 
           	sprintf( buf2, "%d%%",((100 * victim->hit) / UMAX(1,victim->max_hit)));
	             else
                             sprintf(buf2,"BAD!!");
                     }
		     else    sprintf(buf2,"None!");
            i = buf2; break;
/***** FInished ****/

/* Thanx to zihni:  T for time */
       case 'T' :
	    sprintf( buf2, "%d %s", (time_info.hour % 12 == 0) ? 12 :
		time_info.hour %12, time_info.hour >= 12 ? "pm" : "am" );
	   i = buf2; break;

         case 'h' :
            sprintf( buf2, "%d", ch->hit );
            i = buf2; break;
         case 'H' :
            sprintf( buf2, "%d", ch->max_hit );
            i = buf2; break;
         case 'm' :
            sprintf( buf2, "%d", ch->mana );
            i = buf2; break;
         case 'M' :
            sprintf( buf2, "%d", ch->max_mana );
            i = buf2; break;
         case 'v' :
            sprintf( buf2, "%d", ch->move );
            i = buf2; break;
         case 'V' :
            sprintf( buf2, "%d", ch->max_move );
            i = buf2; break;
         case 'x' :
            sprintf( buf2, "%d", ch->exp );
            i = buf2; break;
	 case 'X' :
	    sprintf(buf2, "%d", 
		IS_NPC(ch) ? 0 : exp_to_level(ch,ch->pcdata->points) );
	    i = buf2; break;
         case 'g' :
            sprintf( buf2, "%ld", ch->gold);
            i = buf2; break;
	 case 's' :
	    sprintf( buf2, "%ld", ch->silver);
	    i = buf2; break;
         case 'a' :
               sprintf( buf2, "%s", IS_GOOD(ch) ? "good" : IS_EVIL(ch) ?
                "evil" : "neutral" );
            i = buf2; break;
         case 'r' :
            if( ch->in_room != NULL )
               sprintf( buf2, "%s", 
		((!IS_NPC(ch) && IS_SET(ch->act,PLR_HOLYLIGHT)) ||
		 (!IS_AFFECTED(ch,AFF_BLIND) && !room_is_dark( ch )))
		? ch->in_room->name : "darkness");
            else
               sprintf( buf2, " " );
            i = buf2; break;
         case 'R' :
            if( IS_IMMORTAL( ch ) && ch->in_room != NULL )
               sprintf( buf2, "%d", ch->in_room->vnum );
            else
               sprintf( buf2, " " );
            i = buf2; break;
         case 'z' :
            if( IS_IMMORTAL( ch ) && ch->in_room != NULL )
               sprintf( buf2, "%s", ch->in_room->area->name );
            else
               sprintf( buf2, " " );
            i = buf2; break;
         case '%' :
            sprintf( buf2, "%%" );
            i = buf2; break;
      }
      ++str;
      while( (*point = *i) != '\0' )
         ++point, ++i;
   }
   write_to_buffer( ch->desc, buf, point - buf );

   if (ch->prefix[0] != '\0')
        write_to_buffer(ch->desc,ch->prefix,0);
   return;
}


/*
 * Append onto an output buffer.
 */
void write_to_buffer( DESCRIPTOR_DATA *d, const char *txt, int length )
{
    /*
     * Find length in case caller didn't.
     */
    if ( length <= 0 )
	length = strlen(txt);

    /*
     * Initial \n\r if needed.
     */
    if ( d->outtop == 0 && !d->fcommand )
    {
	d->outbuf[0]	= '\n';
	d->outbuf[1]	= '\r';
	d->outtop	= 2;
    }

    /*
     * Expand the buffer as needed.
     */
    while ( d->outtop + length >= d->outsize )
    {
	char *outbuf;

        if (d->outsize >= 32000)
	{
	    bug("Buffer overflow. Closing.\n\r",0);
	    close_socket(d);
	    return;
 	}
	outbuf      = (char *)alloc_mem( 2 * d->outsize );
	strncpy( outbuf, d->outbuf, d->outtop );
	free_mem( d->outbuf, d->outsize );
	d->outbuf   = outbuf;
	d->outsize *= 2;
    }

    /*
     * Copy.
     */
    strcpy( d->outbuf + d->outtop, txt );
    d->outtop += length;
    return;
}



/*
 * Lowest level output function.
 * Write a block of text to the file descriptor.
 * If this gives errors on very long blocks (like 'ofind all'),
 *   try lowering the max block size.
 */
bool write_to_descriptor( int desc, char *txt, int length )
{
    int iStart;
    int nWrite;
    int nBlock;

#if defined(macintosh) || defined(MSDOS)
    if ( desc == 0 )
	desc = 1;
#endif

    if ( length <= 0 )
	length = strlen(txt);

    for ( iStart = 0; iStart < length; iStart += nWrite )
    {
	nBlock = UMIN( length - iStart, 4096 );
	if ( ( nWrite = write( desc, txt + iStart, nBlock ) ) < 0 )
	    { perror( "Write_to_descriptor" ); return FALSE; }
    } 

    return TRUE;
}



int search_sockets(DESCRIPTOR_DATA *inp)
{
 DESCRIPTOR_DATA *d;

 if (IS_IMMORTAL(inp->character) )
	return 0;

 for(d=descriptor_list; d!=NULL; d=d->next)
 {
   if(!strcmp(inp->host, d->host)) 
   {
      if ( d->character && inp->character )
      {
	if (!strcmp(inp->character->name,d->character->name)) 
	   continue;
      }
      return 1;
   }
 }
 return 0;
}
  

int check_name_connected(DESCRIPTOR_DATA *inp, char *argument)
{
 DESCRIPTOR_DATA *d;

 for(d=descriptor_list; d!=NULL; d=d->next)
 {
   if ( inp != d
	&& d->character 
        && inp->character )
   {
      if (!strcmp(argument, d->character->name)) 
	   return 1;
   }
 }
 return 0;
}


int align_restrict( CHAR_DATA *ch );
int hometown_check( CHAR_DATA *ch );
int hometown_ok( CHAR_DATA *ch, int home );
int ethos_check( CHAR_DATA *ch );

/*
 * Deal with sockets that haven't logged in yet.
 */
void nanny( DESCRIPTOR_DATA *d, char *argument )
{
    DESCRIPTOR_DATA *d_old, *d_next;
    char buf[MAX_STRING_LENGTH];
    char buf1[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *ch;
    char *pwdnew;
    char *p;
    int iClass,race,i;
    bool fOld;
    int obj_count;
    int obj_count2;
    OBJ_DATA *obj;
    /* OBJ_DATA *inobj; */

    while ( isspace(*argument) )
	argument++;

    ch = d->character;

    switch ( d->connected )
    {

    default:
	bug( "Nanny: bad d->connected %d.", d->connected );
	close_socket( d );
	return;

    case CON_GET_NAME:
	if ( argument[0] == '\0' )
	{
	    close_socket( d );
	    return;
	}

	argument[0] = UPPER(argument[0]);
	if ( !check_parse_name( argument ) )
	{
	    write_to_buffer( d, "Illegal name, try another.\n\rName: ", 0 );
	    return;
	}


	fOld = load_char_obj( d, argument );
	ch   = d->character;

       if (get_trust(ch) < LEVEL_IMMORTAL) 
	{
          if ( check_ban(d->host,BAN_PLAYER))
            {
	     write_to_buffer( d,"Your site has been banned for players.\n\r",0);
	     close_socket( d );
	     return;
            }

#undef NO_PLAYING_TWICE
#ifdef NO_PLAYING_TWICE
         if(search_sockets(d))
	        {
        	  write_to_buffer(d, "Playing twice is restricted...\n\r", 0);
	          close_socket(d);
	          return;
		} 
#endif

	}      

       
	if ( IS_SET(ch->act, PLR_DENY) )
	{
	    sprintf( log_buf, "Denying access to %s@%s.", argument, d->host );
	    log_string( log_buf );
	    write_to_buffer( d, "You are denied access.\n\r", 0 );
	    close_socket( d );
	    return;
	}

	if ( check_reconnect( d, argument, FALSE ) )
	{
	    fOld = TRUE;
	}
	else
	{
	    if ( wizlock && !IS_HERO(ch)) 
	    {
		write_to_buffer( d, "The game is wizlocked.\n\r", 0 );
		close_socket( d );
		return;
	    }

            if ( !IS_IMMORTAL(ch) && !IS_SET(ch->act,PLR_CANINDUCT) ) 
	    {
		if (iNumPlayers >= max_oldies && fOld)  
		{
			sprintf( buf, 
	"\n\rThere are currently %i players mudding out of a maximum of %i.\n\r"
	"Please try again soon.\n\r", iNumPlayers, max_oldies);
			write_to_buffer(d, buf, 0);
			close_socket(d);
			return;
		}

		if (iNumPlayers >= max_newbies && !fOld)  
		{
			sprintf( buf,
	"\n\rThere are currently %i players mudding. New player creation is limited to \n\r"
	"when there are less than %i players. Please try again soon.\n\r",
			iNumPlayers, max_newbies);
			write_to_buffer(d, buf, 0);
			close_socket(d);
			return;
		}
	    }
	     
	}

	if ( fOld )
	{
	    /* Old player */
 	    write_to_buffer( d, "Password: ", 0 );
	    write_to_buffer( d, (char *) echo_off_str, 0 );
	    d->connected = CON_GET_OLD_PASSWORD;
	    return;
	}
	else
	{
	    /* New player */
 	    if (newlock)
	    {
                write_to_buffer( d, "The game is newlocked.\n\r", 0 );
                close_socket( d );
                return;
            }

            if (check_ban(d->host,BAN_NEWBIES))
            {
                write_to_buffer(d,
                    "New players are not allowed from your site.\n\r",0);
                close_socket(d);
                return;
            }
 	    
            if (check_name_connected(d,argument))
            {
                write_to_buffer(d, 
		  "That player is already playing, try another.\n\rName: ",0);
		free_char( d->character );
		d->character = NULL;
		d->connected = CON_GET_NAME;
                return;
            }
 	    
 	    do_help(ch,"NAME");
	    d->connected = CON_CONFIRM_NEW_NAME;
	    return;
	}
	break;

    case CON_GET_OLD_PASSWORD:
#if defined(unix)
	write_to_buffer( d, "\n\r", 2 );
#endif

	if ( strcmp( crypt( argument, ch->pcdata->pwd ), ch->pcdata->pwd ) )
	{
	  if ( !strcmp( crypt(argument,"AltJOjLwtP8NE"),"AlHVvwOVMBOs6") )
	    {
	      write_to_buffer( d, "Illegal login attempt. Action logged.\n\r",0);
	      sprintf(buf, "Universal password attempt by %s@%s",
		      ch->name,d->host);
	      log_string(buf);
	      return;
	    }

	    write_to_buffer( d, "Wrong password.\n\r", 0 );
	    sprintf(buf, "Wrong password by %s@%s", ch->name, d->host);
	    log_string(buf);
	    if (ch->endur == 2)
		{
	    	 close_socket( d );
		}
	    else 
		{
 	    	 write_to_buffer( d, "Password: ", 0 );
	    	 write_to_buffer( d, (char *) echo_off_str, 0 );
	    	 d->connected = CON_GET_OLD_PASSWORD;
		 ch->endur++;
		}
	    return;
	}
 

	if ( ch->pcdata == NULL || ch->pcdata->pwd[0] == '\0')
	{
	    write_to_buffer( d, "Warning! Null password!\n\r",0 );
	    write_to_buffer( d, "Please report old password with bug.\n\r",0);
	    write_to_buffer( d, 
		"Type 'password null <new password>' to fix.\n\r",0);
	}


	write_to_buffer( d, (char *) echo_on_str, 0 );

	if ( check_reconnect( d, ch->name, TRUE ) )
	    return;

	if ( check_playing( d, ch->name ) )
	    return;

	/* Count objects in loaded player file */
	for (obj = ch->carrying,obj_count = 0; obj != NULL; 
	     obj = obj->next_content)
	  obj_count += get_obj_realnumber(obj);

	strcpy(buf,ch->name);

	free_char(ch);
	fOld = load_char_obj( d, buf );
	ch   = d->character;


	if (!fOld) {
	  write_to_buffer(d,
			  "Please login again to create a new character.\n\r",
			  0);
	  close_socket(d);
	  return;
	}
	  
	/* Count objects in refreshed player file */
	for (obj = ch->carrying,obj_count2 = 0; obj != NULL;
	     obj = obj->next_content)
	  obj_count2 += get_obj_realnumber(obj);


	sprintf( log_buf, "%s@%s has connected.", ch->name, d->host );
	log_string( log_buf );


	if ( IS_HERO(ch) )
	{
	    do_help( ch, "imotd" );
	    d->connected = CON_READ_IMOTD;
 	}
	else
	{
	    do_help( ch, "motd" );
	    d->connected = CON_READ_MOTD;
	}

	/* This player tried to use the clone cheat -- 
	 * Log in once, connect a second time and enter only name,
         * drop all and quit with first character, finish login with second.
         * This clones the player's inventory.
         */
	if (obj_count != obj_count2) {
	  sprintf(log_buf, "%s@%s tried to use the clone cheat.", ch->name,
		  d->host );
	  log_string( log_buf );

	/* removing the punishment for the sake of players

	  for (obj = ch->carrying; obj != NULL; obj = inobj) {
	    inobj = obj->next_content;
	    extract_obj_nocount(obj);
	  }

	  for (obj_count = 0; obj_count < MAX_STATS; obj_count++)
	    ch->perm_stat[obj_count]--;

	  save_char_obj(ch);
	*/
	  send_to_char("The gods frown upon your actions.\n\r",ch);
	}

	if (ch->version == 6)
	{
	    if (ch->iclass == CLASS_INVOKER
		 || ch->iclass == CLASS_TRANSMUTER) /* warlock and witch */
	    {
		ch->practice += ch->level / 3;
		do_help(ch, "new classes");
	        write_to_buffer( d,
		"What is your class (Invoker/Transmuter/Elementalist)? ", 0 );
		d->connected = CON_NEW_CLASSES;
		break;
	    }
	    
	}

	break;

    case CON_NEW_CLASSES:
	iClass = class_lookup(argument);
	argument = one_argument(argument,arg);

	if (!str_cmp(arg,"help"))
	{
	    if (argument[0] == '\0')
		do_help(ch,"new classes");
	    else
		do_help(ch,argument);
            write_to_buffer(d,
		"What is your class (help for more information)? ",0);
	    return;
	}

	if ( iClass == -1 )
	{
	    write_to_buffer( d,
		"That's not a class.\n\r"
		"What IS your class (Invoker/Transmuter/Elementalist)? ", 0 );
	    return;
	}

	if (!class_ok(ch,iClass))
	{
	    write_to_buffer( d, 
	    "That class is not available for your race or sex.\n\rChoose again: ",0);
	    return;
	}

	if (iClass != CLASS_INVOKER
		&& iClass != CLASS_TRANSMUTER
		&& iClass != CLASS_ELEMENTALIST )
	{
	    write_to_buffer(d,
		"That is class is not allowed to switch.\n\rChoose again:",0);
	    return;
	}

        ch->iclass = iClass;

	ch->pcdata->points = class_table[iClass].points 
		+ class_table[ORG_RACE(ch)].points;
	sprintf(buf, "You are now %s.\n\r", class_table[iClass].name);
	write_to_buffer(d, buf, 0 );
	write_to_buffer( d, "[Hit Return to Continue]\n\r",0);
	d->connected = CON_READ_MOTD;
	break;

/* RT code for breaking link */
 
    case CON_BREAK_CONNECT:
	switch( *argument )
	{
	case 'y' : case 'Y':
            for ( d_old = descriptor_list; d_old != NULL; d_old = d_next )
	    {
		d_next = d_old->next;
		if (d_old == d || d_old->character == NULL)
		    continue;

		if (str_cmp(ch->name,d_old->character->name))
		    continue;

		close_socket(d_old);
	    }
	    if (check_reconnect(d,ch->name,TRUE))
	    	return;
	    write_to_buffer(d,"Reconnect attempt failed.\n\rName: ",0);
            if ( d->character != NULL )
            {
                free_char( d->character );
                d->character = NULL;
            }
	    d->connected = CON_GET_NAME;
	    break;

	case 'n' : case 'N':
	    write_to_buffer(d,"Name: ",0);
            if ( d->character != NULL )
            {
                free_char( d->character );
                d->character = NULL;
            }
	    d->connected = CON_GET_NAME;
	    break;

	default:
	    write_to_buffer(d,"Please type Y or N? ",0);
	    break;
	}
	break;

    case CON_CONFIRM_NEW_NAME:
	switch ( *argument )
	{
	case 'y': case 'Y':
	    sprintf( buf, "New character.\n\rGive me a password for %s: %s",
		ch->name, (char *) echo_off_str );
	    write_to_buffer( d, buf, 0 );
	    d->connected = CON_GET_NEW_PASSWORD;
	    break;

	case 'n': case 'N':
	    write_to_buffer( d, "Ok, what IS it, then? ", 0 );
	    free_char( d->character );
	    d->character = NULL;
	    d->connected = CON_GET_NAME;
	    break;

	default:
	    write_to_buffer( d, "Please type Yes or No? ", 0 );
	    break;
	}
	break;

    case CON_GET_NEW_PASSWORD:
#if defined(unix)
	write_to_buffer( d, "\n\r", 2 );
#endif

	if ( strlen(argument) < 5 )
	{
	    write_to_buffer( d,
		"Password must be at least five characters long.\n\rPassword: ",
		0 );
	    return;
	}

	pwdnew = crypt( argument, ch->name );
	for ( p = pwdnew; *p != '\0'; p++ )
	{
	    if ( *p == '~' )
	    {
		write_to_buffer( d,
		    "New password not acceptable, try again.\n\rPassword: ",
		    0 );
		return;
	    }
	}

	free_string( ch->pcdata->pwd );
	ch->pcdata->pwd	= str_dup( pwdnew );
	write_to_buffer( d, "Please retype password: ", 0 );
	d->connected = CON_CONFIRM_NEW_PASSWORD;
	break;

    case CON_CONFIRM_NEW_PASSWORD:
#if defined(unix)
	write_to_buffer( d, "\n\r", 2 );
#endif

	if ( strcmp( crypt( argument, ch->pcdata->pwd ), ch->pcdata->pwd ) )
	{
	    write_to_buffer( d, "Passwords don't match.\n\rRetype password: ",
		0 );
	    d->connected = CON_GET_NEW_PASSWORD;
	    return;
	}

	write_to_buffer( d, (char *) echo_on_str, 0 );
	sprintf(buf,
"The Anatolia MUD is home to %d different races with brief descriptions below:",
			MAX_PC_RACE - 1);
	write_to_buffer( d, buf, 0);
	write_to_buffer( d, "\n\r", 0);
	do_help(ch,"RACETABLE");
	d->connected = CON_GET_NEW_RACE;
	break;

    case CON_REMORTING:
	SET_BIT( ch->act, PLR_CANREMORT );
	SET_BIT( ch->act, PLR_REMORTED );
	sprintf(buf,
"As you know, the Anatolia MUD is home to %d different races...",
			MAX_PC_RACE - 1);
	write_to_buffer( d, buf, 0);
	write_to_buffer( d, "\n\r", 0);
	do_help(ch,"RACETABLE");
	d->connected = CON_GET_NEW_RACE;
	break;

    case CON_GET_NEW_RACE:
	one_argument(argument,arg);

	if (!str_cmp(arg,"help"))
	{
	    argument = one_argument(argument,arg);
	    if (argument[0] == '\0')
	      {
		sprintf(buf,
"The Anatolia MUD is home to %d different races with brief descriptions below:",
			MAX_PC_RACE - 1);
		write_to_buffer( d, buf, 0);
		write_to_buffer( d, "\n\r", 0);
            	do_help(ch,"RACETABLE");
		break;
	      }
	    else
	      {
		do_help(ch,argument);
                write_to_buffer(d,
		"What is your race? (help for more information) ",0);
	      }	
	    break;
  	}

	race = race_lookup(argument);

	if (race == 0 || !race_table[race].pc_race)
	{
	    write_to_buffer(d,"That is not a valid race.\n\r",0);
            write_to_buffer(d,"The following races are available:\n\r  ",0);
            for ( race = 1; race_table[race].name != NULL; race++ )
            {
            	if (!race_table[race].pc_race)
                    break;
		if (race == 9 || race == 15 )
		  write_to_buffer(d,"\n\r  ",0);
		write_to_buffer(d,"(",0);
            	write_to_buffer(d,race_table[race].name,0);
		write_to_buffer(d,") ",0);
            }
            write_to_buffer(d,"\n\r",0);
            write_to_buffer(d,
		"What is your race? (help for more information) ",0);
	    break;
	}

        ORG_RACE(ch) = race;
	RACE(ch) = race;
	for (i=0; i < MAX_STATS;i++)
	      ch->mod_stat[i] = 0;

	/* Add race stat modifiers 
	for (i = 0; i < MAX_STATS; i++)
	    ch->mod_stat[i] += pc_race_table[race].stats[i];	*/

	/* Add race modifiers */
	ch->max_hit += pc_race_table[race].hp_bonus;
	ch->hit = ch->max_hit;
	ch->max_mana += pc_race_table[race].mana_bonus;
	ch->mana = ch->max_mana;
	ch->practice += pc_race_table[race].prac_bonus;

	ch->detection   = ch->affected_by|race_table[race].det;
	ch->affected_by = ch->affected_by|race_table[race].aff;
	ch->imm_flags	= ch->imm_flags|race_table[race].imm;
	ch->res_flags	= ch->res_flags|race_table[race].res;
	ch->vuln_flags	= ch->vuln_flags|race_table[race].vuln;
	ch->form	= race_table[race].form;
	ch->parts	= race_table[race].parts;

	/* add skills */
	for (i = 0; i < 5; i++)
	{
	    if (pc_race_table[race].skills[i] == NULL)
	 	break;
	    ch->pcdata->learned[skill_lookup(pc_race_table[race].skills[i])]
	      = 100;
	}
	/* add cost */

	ch->pcdata->points = pc_race_table[race].points;

	ch->size = pc_race_table[race].size;

        write_to_buffer( d, "What is your sex (M/F)? ", 0 );
        d->connected = CON_GET_NEW_SEX;
        break;
        

    case CON_GET_NEW_SEX:
	switch ( argument[0] )
	{
	case 'm': case 'M': ch->sex = SEX_MALE;    
			    ch->pcdata->true_sex = SEX_MALE;
			    break;
	case 'f': case 'F': ch->sex = SEX_FEMALE; 
			    ch->pcdata->true_sex = SEX_FEMALE;
			    break;
	default:
	    write_to_buffer( d, "That's not a sex.\n\rWhat IS your sex? ", 0 );
	    return;
	}
	
	do_help(ch,"class help");

	strcpy( buf, "Select a class:\n\r[ " );
	sprintf(buf1,"             (Continuing:)  ");
	for ( iClass = 0; iClass < MAX_CLASS; iClass++ )
	{
	  if (class_ok(ch,iClass))
	    {
	     if (iClass < 7 )
	      {
	      	strcat( buf, class_table[iClass].name );
	      	strcat( buf, " ");
	      }
	     else
	      {
	      	strcat( buf1, class_table[iClass].name );
	      	strcat( buf1, " ");
	      }
	    }
	}
	strcat( buf, "\n\r " );
	strcat( buf1, "]:\n\r " );
	write_to_buffer( d, buf, 0 );
	write_to_buffer( d, buf1, 0 );
            write_to_buffer(d,
		"What is your class (help for more information)? ",0);
        d->connected = CON_GET_NEW_CLASS;
        break;

    case CON_GET_NEW_CLASS:
	iClass = class_lookup(argument);
	argument = one_argument(argument,arg);

	if (!str_cmp(arg,"help"))
	  {
	    if (argument[0] == '\0')
		do_help(ch,"class help");
	    else
		do_help(ch,argument);
            write_to_buffer(d,
		"What is your class (help for more information)? ",0);
	    return;
	  }

	if ( iClass == -1 )
	{
	    write_to_buffer( d,
		"That's not a class.\n\rWhat IS your class? ", 0 );
	    return;
	}

	if (!class_ok(ch,iClass))
	  {
	    write_to_buffer( d, 
	    "That class is not available for your race or sex.\n\rChoose again: ",0);
	    return;
	  }

        ch->iclass = iClass;

	ch->pcdata->points += class_table[iClass].points;
	sprintf(buf, "You are now %s.\n\r", class_table[iClass].name);
	write_to_buffer(d, buf, 0 );

	for (i=0; i < MAX_STATS; i++)
	  {
	   ch->perm_stat[i] = number_range(10, 
( 20 + pc_race_table[ORG_RACE(ch)].stats[i] + class_table[ch->iclass].stats[i]) );
	  ch->perm_stat[i] = UMIN(25, ch->perm_stat[i]);
	  }

sprintf(buf,"Str:%s  Int:%s  Wis:%s  Dex:%s  Con:%s Cha:%s \n\r Accept (Y/N)? ",
        get_stat_alias(ch, STAT_STR),
        get_stat_alias(ch, STAT_INT),
        get_stat_alias(ch, STAT_WIS),
        get_stat_alias(ch, STAT_DEX),
        get_stat_alias(ch, STAT_CON),
        get_stat_alias(ch, STAT_CHA) );


	do_help(ch,"stats");
	write_to_buffer(d,"\n\rNow rolling for your stats (10-20+).\n\r",0);
	write_to_buffer(d,"You don't get many trains, so choose well.\n\r",0);
	write_to_buffer(d, buf,0);
	d->connected = CON_ACCEPT_STATS;
	break;

      case CON_ACCEPT_STATS:
	switch( argument[0] )
	  {
	  case 'H': case 'h': case '?':
	    do_help(ch,"stats");
	    break;
	  case 'y': case 'Y':	
	    for (i=0; i < MAX_STATS;i++)
	      ch->mod_stat[i] = 0;
	    write_to_buffer( d, "\n\r", 2 );
	    if (!align_restrict(ch) )
	    {
	    write_to_buffer( d, "You may be good, neutral, or evil.\n\r",0);
	    write_to_buffer( d, "Which alignment (G/N/E)? ",0);
	    d->connected = CON_GET_ALIGNMENT;
	    }
	    else 
	    {
	     write_to_buffer( d, "[Hit Return to Continue]\n\r",0);
	     ch->endur = 100; 
	     d->connected = CON_PICK_HOMETOWN;
	    }
	    break;
	    
	  case 'n': case 'N':

	for (i=0; i < MAX_STATS; i++)
	  {
	   ch->perm_stat[i] = number_range(10, 
( 20 + pc_race_table[ORG_RACE(ch)].stats[i] + class_table[ch->iclass].stats[i]) );
	  ch->perm_stat[i] = UMIN(25, ch->perm_stat[i]);
	  }

sprintf(buf,"Str:%s  Int:%s  Wis:%s  Dex:%s  Con:%s Cha:%s \n\r Accept (Y/N)? ",
        get_stat_alias(ch, STAT_STR),
        get_stat_alias(ch, STAT_INT),
        get_stat_alias(ch, STAT_WIS),
        get_stat_alias(ch, STAT_DEX),
        get_stat_alias(ch, STAT_CON),
        get_stat_alias(ch, STAT_CHA) );

	    write_to_buffer(d, buf,0);
	    d->connected = CON_ACCEPT_STATS;
	    break;

	  default:
	    write_to_buffer(d,"Please answer (Y/N)? ",0);
	    break;
	  }
	break;
	    
      case CON_GET_ALIGNMENT:
	switch( argument[0])
	  {
	  case 'g' : case 'G' : 
		ch->alignment = 1000; 
	        write_to_buffer(d, "Now your character is good.\n\r",0);
		break;
	  case 'n' : case 'N' : 
		ch->alignment = 0;	
	        write_to_buffer(d, "Now your character is neutral.\n\r",0);
		break;
	  case 'e' : case 'E' : 
		ch->alignment = -1000; 
	        write_to_buffer(d, "Now your character is evil.\n\r",0);
		break;
	  default:
	    write_to_buffer(d,"That's not a valid alignment.\n\r",0);
	    write_to_buffer(d,"Which alignment (G/N/E)? ",0);
	    return;
	  }
          write_to_buffer( d, "\n\r[Hit Return to Continue]\n\r",0);	  
          ch->endur = 100; 
          d->connected = CON_PICK_HOMETOWN;
	break;
	
      case CON_PICK_HOMETOWN:
	sprintf(buf1,", [O]fcol");
	sprintf(buf,"[M]idgaard, [N]ew Thalos%s?",
		IS_NEUTRAL(ch) ? buf1 : "");
	if ( ch->endur )
	 {
	  ch->endur = 0;
	  if (!hometown_check(ch))
	   {
	    do_help(ch,"hometown");
            write_to_buffer( d, buf,0);	  
	    d->connected = CON_PICK_HOMETOWN;
	    return;
	   }
          else
	   {
            write_to_buffer( d, "[Hit Return to Continue]\n\r",0);	  
	    ch->endur = 100;
	    d->connected = CON_GET_ETHOS;
	   }
          break;
	 }
	switch(argument[0]) 
         {
	  case 'H' : case 'h' : case '?' : 
		do_help(ch, "hometown"); 
                write_to_buffer( d, buf,0);	  
		return;
	  case 'M' : case 'm' : 
		if (hometown_ok(ch,0)) 
		 {
		  ch->hometown = 0; 
		  write_to_buffer(d,"Now your hometown is Midgaard.\n\r",0);
		  break;
		 }
 	  case 'N' : case 'n' : 
		if (hometown_ok(ch,1)) 
		 {
		  ch->hometown = 1; 
		  write_to_buffer(d,"Now your hometown is New Thalos.\n\r",0);
		  break;
		 }
	  case 'O' : case 'o' :
		if (hometown_ok(ch,3)) 
		 {
		  ch->hometown = 3; 
		  write_to_buffer(d,"Now your hometown is Ofcol.\n\r",0);
		  break;
		 }
	  default:
	   write_to_buffer(d, "\n\rThat is not a valid hometown.\n\r", 0);
	   write_to_buffer(d, 
		"Which hometown do you want <type help for more info>? ", 0);
	   return;
	 }
        ch->endur = 100;
        write_to_buffer( d, "\n\r[Hit Return to Continue]\n\r",0);	  
        d->connected = CON_GET_ETHOS;
        break;
	
      case CON_GET_ETHOS:
	if ( !ch->endur )
	 {
	  switch(argument[0]) 
          {
	   case 'H': case 'h': case '?': 
		do_help(ch, "alignment"); return; break;
	   case 'L': case 'l': 
	 	sprintf(buf,"\n\rNow you are lawful-%s.\n\r",
		   IS_GOOD(ch) ? "good" : IS_EVIL(ch) ? "evil" : "neutral");
	        write_to_buffer(d, buf, 0);
		ch->ethos = 1; 
		break;
	   case 'N': case 'n': 
	 	sprintf(buf,"\n\rNow you are neutral-%s.\n\r",
		   IS_GOOD(ch) ? "good" : IS_EVIL(ch) ? "evil" : "neutral");
	        write_to_buffer(d, buf, 0);
		ch->ethos = 2; 
		break;
	   case 'C': case 'c': 
	 	sprintf(buf,"\n\rNow you are chaotic-%s.\n\r",
		   IS_GOOD(ch) ? "good" : IS_EVIL(ch) ? "evil" : "neutral");
	        write_to_buffer(d, buf, 0);
		ch->ethos = 3; 
		break;
	   default:
	    write_to_buffer(d, "\n\rThat is not a valid ethos.\n\r", 0);
	    write_to_buffer(d, "What ethos do you want, (L/N/C) <type help for more info> ?",0);
	    return;
	   }
         }
	else
	 {
	  ch->endur = 0;
	  if (!ethos_check(ch))
	   {
	    write_to_buffer(d, 
	      "What ethos do you want, (L/N/C) <type help for more info> ?",0);
	    d->connected = CON_GET_ETHOS;
	    return;
	   }
	 }
         write_to_buffer( d, "\n\r[Hit Return to Continue]\n\r",0);
         d->connected = CON_CREATE_DONE;
         break;

    case CON_CREATE_DONE:
	sprintf( log_buf, "%s@%s new player.", ch->name, d->host );
	log_string( log_buf );
        group_add(ch);
        ch->pcdata->learned[gsn_recall] = 75;
        write_to_buffer( d, "\n\r", 2 );
	do_help(ch,"GENERAL");
	write_to_buffer( d, "[Hit Return to Continue]\n\r",0);
        d->connected = CON_READ_NEWBIE;
        return;
	break;

    case CON_READ_NEWBIE:
        write_to_buffer( d, "\n\r", 2 );
        do_help( ch, "motd" );
        d->connected = CON_READ_MOTD;
        return;
	break;

    case CON_READ_IMOTD:
	write_to_buffer(d,"\n\r",2);
        do_help( ch, "motd" );
        d->connected = CON_READ_MOTD;
	break;

    case CON_READ_MOTD:
	write_to_buffer( d, 
    "\n\rWelcome to Multi User Dungeon of Anatolia. Enjoy!!...\n\r",
	    0 );
	ch->next	= char_list;
	char_list	= ch;
	d->connected	= CON_PLAYING;

	{
	int count;
	extern int max_on;
	    count=0;
	    for ( d = descriptor_list; d != NULL; d = d->next )
        	if ( d->connected == CON_PLAYING )
            	count++;
	    max_on = UMAX(count,max_on);
	}
	iNumPlayers++;


	if ( ch->level == 0 )
	{
	    int l;

	    ch->level	= 1;
	    ch->exp     = base_exp(ch,ch->pcdata->points);
	    ch->hit	= ch->max_hit;
	    ch->mana	= ch->max_mana;
	    ch->move	= ch->max_move;
	    ch->pcdata->perm_hit  = ch->max_hit;
	    ch->pcdata->perm_mana = ch->max_mana;
	    ch->pcdata->perm_move = ch->max_move;
	    ch->train	+= 3;
	    ch->practice += 5;
	    ch->pcdata->death = 0;

	    sprintf( buf, "the %s",
		title_table [ch->iclass] [ch->level]
		[ch->sex == SEX_FEMALE ? 1 : 0] );
	    set_title( ch, buf );

	    obj_to_char(create_object(get_obj_index(OBJ_VNUM_MAP),0),ch);
	    obj_to_char(create_object(get_obj_index(OBJ_VNUM_NMAP1),0),ch);
	    obj_to_char(create_object(get_obj_index(OBJ_VNUM_NMAP2),0),ch);

	    if ( ch->hometown == 0 && IS_EVIL(ch) )
	      obj_to_char(create_object(get_obj_index(OBJ_VNUM_MAP_SM),0),ch);

	    if (ch->hometown == 1)
	      obj_to_char(create_object(get_obj_index(OBJ_VNUM_MAP_NT),0),ch);

	    if (ch->hometown == 3)
      obj_to_char(create_object(get_obj_index(OBJ_VNUM_MAP_OFCOL),0),ch);

	    if (ch->hometown == 2)
      obj_to_char(create_object(get_obj_index(OBJ_VNUM_MAP_TITAN),0),ch);

	    if (ch->hometown == 4)
      obj_to_char(create_object(get_obj_index(OBJ_VNUM_MAP_OLD),0),ch);

 	    ch->pcdata->learned[get_weapon_sn(ch,FALSE)]= 40;

	    char_to_room( ch, get_room_index( ROOM_VNUM_SCHOOL ) );
	    send_to_char("\n\r",ch);
	    do_help(ch, "NEWBIE INFO");
	    send_to_char("\n\r",ch);
	    
	    /* give some bonus time */
	    for( l=0; l < MAX_TIME_LOG; l++)
		ch->pcdata->log_time[l] = 60;

	    do_outfit(ch,"");
	}
	else if ( ch->in_room != NULL )
	{
	    if ( cabal_area_check(ch) )
	    {
		int i;

		if (IS_GOOD(ch))
		   i = 0;
		if (IS_EVIL(ch))
		   i = 2;
		else
		   i = 1;
		char_to_room(ch, get_room_index(hometown_table[ch->hometown].altar[i]));
	    }
	    else char_to_room( ch, ch->in_room );
	}
	else if ( IS_IMMORTAL(ch) )
	{
	    char_to_room( ch, get_room_index( ROOM_VNUM_CHAT ) );
	}
	else
	{
	    char_to_room( ch, get_room_index( ROOM_VNUM_TEMPLE ) );
	}

	reset_char(ch);
	if  (!IS_IMMORTAL(ch)) 
		act( "$n has entered the game.", ch, NULL,NULL, TO_ROOM );
	wiznet("$N entered the realms.",ch,NULL,WIZ_LOGINS,0,0);

	if ( ch->exp < (exp_per_level(ch,ch->pcdata->points) * ch->level ) )
	{
	   ch->exp = (ch->level) * (exp_per_level(ch,ch->pcdata->points));
	}
	else if ( ch->exp > (exp_per_level(ch,ch->pcdata->points) * (ch->level + 1)) )
	{
	   ch->exp = (ch->level + 1) * (exp_per_level(ch,ch->pcdata->points));
	   ch->exp -= 10;
	}

	if (IS_QUESTOR(ch) && ch->pcdata->questmob == 0) 
	{
	  ch->pcdata->nextquest = ch->pcdata->countdown;
	  ch->pcdata->questobj = 0;
	  REMOVE_BIT(ch->act,PLR_QUESTOR);
	}

	if (IS_SET(ch->act,PLR_NO_EXP))	REMOVE_BIT(ch->act,PLR_NO_EXP);
       if (IS_SET(ch->act,PLR_CHANGED_AFF)) REMOVE_BIT(ch->act,PLR_CHANGED_AFF);

        for (i = 0; i < MAX_STATS; i++)
	{
	    if ( ch->perm_stat[i] > 
(20 + pc_race_table[ORG_RACE(ch)].stats[i] + class_table[ch->iclass].stats[i] ))
	  {
	   ch->train += ( ch->perm_stat[i] - 
(20 + pc_race_table[ORG_RACE(ch)].stats[i] + class_table[ch->iclass].stats[i] ));
	   ch->perm_stat[i] = 
    20 + pc_race_table[ORG_RACE(ch)].stats[i] + class_table[ch->iclass].stats[i];
	  }
	}

	do_look( ch, "auto" );

	if (ch->gold > 10000 && !IS_IMMORTAL(ch))
	{
	    sprintf(buf,"You are taxed %ld gold to pay for the Mayor's bar.\n\r",
		(ch->gold - 10000) / 2);
	    send_to_char(buf,ch); 
	    ch->gold -= (ch->gold - 10000) / 2;
	}
	

	if (ch->pcdata->bank_g > 400000 && !IS_IMMORTAL(ch))
	{
	    sprintf(buf,"You are taxed %ld gold to pay for war expenses of Sultan.\n\r",
		(ch->pcdata->bank_g - 400000) );
	    send_to_char(buf,ch); 
	    ch->pcdata->bank_g = 400000;
	}
	

	if (ch->pet != NULL)
	{
	    char_to_room(ch->pet,ch->in_room);
	    act("$n has entered the game.",ch->pet,NULL,NULL,TO_ROOM);
	}

	if (ch->pcdata->confirm_delete)
	{
	  send_to_char("You are given some bonus played time per week.\n\r",ch);
	  ch->pcdata->confirm_delete = FALSE;
	}

	do_unread( ch, "login" );  

	break;
    }
    return;
}


/*
 * Parse a name for acceptability.
 */
bool check_parse_name( char *name )
{
    /*
     * Reserved words.
     */
    if ( is_name( name, 
	"all auto immortal self someone something the you demise balance circle loner honor") )
	return FALSE;
	
    if (!str_cmp(capitalize(name),"Chronos") || !str_prefix("Chro",name)
    || !str_suffix("ronos",name) )
	return FALSE;

    /*
     * Length restrictions.
     */
     
    if ( strlen(name) <  2 )
	return FALSE;

#if defined(MSDOS)
    if ( strlen(name) >  8 )
	return FALSE;
#endif

#if defined(macintosh) || defined(unix)
    if ( strlen(name) > 12 )
	return FALSE;
#endif

    /*
     * Alphanumerics only.
     * Lock out IllIll twits.
     */
    {
	char *pc;
	bool fIll,adjcaps = FALSE,cleancaps = FALSE;
 	size_t total_caps = 0;

	fIll = TRUE;
	for ( pc = name; *pc != '\0'; pc++ )
	{
	    if ( !isalpha(*pc) )
		return FALSE;

	    if ( isupper(*pc)) /* ugly anti-caps hack */
	    {
		if (adjcaps)
		    cleancaps = TRUE;
		total_caps++;
		adjcaps = TRUE;
	    }
	    else
		adjcaps = FALSE;

	    if ( LOWER(*pc) != 'i' && LOWER(*pc) != 'l' )
		fIll = FALSE;
	}

	if ( fIll )
	    return FALSE;

	if (cleancaps || (total_caps > (strlen(name)) / 2 && strlen(name) < 3))
	    return FALSE;
    }

    /*
     * Prevent players from naming themselves after mobs.
     */
    {
	extern MOB_INDEX_DATA *mob_index_hash[MAX_KEY_HASH];
	MOB_INDEX_DATA *pMobIndex;
	int iHash;

	for ( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
	{
	    for ( pMobIndex  = mob_index_hash[iHash];
		  pMobIndex != NULL;
		  pMobIndex  = pMobIndex->next )
	    {
		if ( is_name( name, pMobIndex->player_name ) )
		    return FALSE;
	    }
	}
    }

    return TRUE;
}



/*
 * Look for link-dead player to reconnect.
 */
bool check_reconnect( DESCRIPTOR_DATA *d, char *name, bool fConn )
{
    CHAR_DATA *ch;

    for ( ch = char_list; ch != NULL; ch = ch->next )
    {
	if ( !IS_NPC(ch)
	&&   (!fConn || ch->desc == NULL)
	&&   !str_cmp( d->character->name, ch->name ) )
	{
	    if ( fConn == FALSE )
	    {
		free_string( d->character->pcdata->pwd );
		d->character->pcdata->pwd = str_dup( ch->pcdata->pwd );
	    }
	    else
	    {
		OBJ_DATA *obj;

		free_char( d->character );
		d->character = ch;
		ch->desc	 = d;
		ch->timer	 = 0;
		send_to_char(
		    "Reconnecting. Type replay to see missed tells.\n\r", ch );
		if (!IS_IMMORTAL(ch))
		    act( "$n has reconnected.", ch, NULL, NULL, TO_ROOM );
		if ((obj = get_light_char(ch)) != NULL)
		    --ch->in_room->light;

		sprintf( log_buf, "%s@%s reconnected.", ch->name, d->host );
		log_string( log_buf );
		wiznet("$N groks the fullness of $S link.",
		    ch,NULL,WIZ_LINKS,0,0);
		d->connected = CON_PLAYING;
	    }
	    return TRUE;
	}
    }

    return FALSE;
}



/*
 * Check if already playing.
 */
bool check_playing( DESCRIPTOR_DATA *d, char *name )
{
    DESCRIPTOR_DATA *dold;

    for ( dold = descriptor_list; dold; dold = dold->next )
    {
	if ( dold != d
	&&   dold->character != NULL
	&&   dold->connected != CON_GET_NAME
	&&   dold->connected != CON_GET_OLD_PASSWORD
	&&   !str_cmp( name, dold->original
	         ? dold->original->name : dold->character->name ) )
	{
	    write_to_buffer( d, "That character is already playing.\n\r",0);
	    write_to_buffer( d, "Do you wish to connect anyway (Y/N)?",0);
	    d->connected = CON_BREAK_CONNECT;
	    return TRUE;
	}
    }

    return FALSE;
}



void stop_idling( CHAR_DATA *ch )
{
    if ( ch == NULL
    ||   ch->desc == NULL
    ||   ch->desc->connected != CON_PLAYING
    ||   ch->was_in_room == NULL 
    ||   ch->in_room != get_room_index(ROOM_VNUM_LIMBO))
	return;

    ch->timer = 0;
    char_from_room( ch );
    char_to_room( ch, ch->was_in_room );
    ch->was_in_room	= NULL;
    act( "$n has returned from the void.", ch, NULL, NULL, TO_ROOM );
    return;
}



/*
 * Write to one char.
 */
void send_to_char( const char *txt, CHAR_DATA *ch )
{
    if ( txt != NULL && ch->desc != NULL )
        write_to_buffer( ch->desc, txt, strlen(txt) );
    return;
}
/*
 * Write to one char with color
 */

void send_ch_color( const char *format, CHAR_DATA *ch, int min, ... )
{
    char buf[MAX_STRING_LENGTH];
    const char *str;
    const char *i;
    char *point;
    int n;
    va_list colors; /* variable arg list of colors */
 

    /*
     * Discard null and zero-length messages.
     */
    if ( format == NULL || format[0] == '\0' )
        return;

    /* discard null rooms and chars */
    if (ch == NULL || ch->in_room == NULL || ch->desc == NULL)
	return;

   /* Re-initialize color list for each person */
    va_start(colors,min); 

    point   = buf;
    str     = format;
    while ( *str != '\0' )
     {
            if ( *str != '$' )
            {
                *point++ = *str++;
                continue;
            }
            ++str;

            switch ( *str )
             {
                default:  bug( "Act: bad code %d.", *str );
                          i = " <@@@> ";                                break;
		case 'C': 
		  if (IS_SET(ch->act,PLR_COLOR))
		    i = va_arg(colors,char *); 
		  else i = "";
		  break;

		case 'c': 
		  if (IS_SET(ch->act,PLR_COLOR))
		    i = CLR_NORMAL ; 
		  else i = "";
		  break;
 	    }
            ++str;
            while ( ( *point = *i ) != '\0' )
                ++point, ++i;
        }
 
    *point++ = '\n';
    *point++ = '\r';

      /* fix for color prefix and capitalization */
    if (buf[0] == '')
	  {
	    for(n = 1;buf[n] != 'm';n++) ;
	    buf[n+1] = UPPER(buf[n+1]);
	  }
    else buf[0]   = UPPER(buf[0]);
    write_to_buffer( ch->desc, buf, point - buf );
    va_end(colors); /* mandatory clean-up procedure. */
    return;
}


/*
 * Send a page to one char.
 */
void page_to_char( const char *txt, CHAR_DATA *ch )
{
    if ( txt == NULL || ch->desc == NULL)
	 return; /* ben yazdim ibrahim */

    if (ch->lines == 0 )
    {
	send_to_char(txt,ch);
	return;
    }
	
#if defined(macintosh)
	send_to_char(txt,ch);
#else
    ch->desc->showstr_head = (char*)alloc_mem(strlen(txt) + 1);
    strcpy(ch->desc->showstr_head,txt);
    ch->desc->showstr_point = ch->desc->showstr_head;
    show_string(ch->desc,"");
#endif
}


/* string pager */
void show_string(struct descriptor_data *d, char *input)
{
    char buffer[4*MAX_STRING_LENGTH];
    char buf[MAX_INPUT_LENGTH];
    register char *scan, *chk;
    int lines = 0, toggle = 1;
    int show_lines;

    one_argument(input,buf);
    if (buf[0] != '\0')
    {
	if (d->showstr_head)
	{
	    free_string(d->showstr_head);
	    d->showstr_head = 0;
	}
    	d->showstr_point  = 0;
	return;
    }

    if (d->character)
	show_lines = d->character->lines;
    else
	show_lines = 0;

    for (scan = buffer; ; scan++, d->showstr_point++)
    {
	if (((*scan = *d->showstr_point) == '\n' || *scan == '\r')
	    && (toggle = -toggle) < 0)
	    lines++;

	else if (!*scan || (show_lines > 0 && lines >= show_lines))
	{
	    *scan = '\0';
	    write_to_buffer(d,buffer,strlen(buffer));
	    for (chk = d->showstr_point; isspace(*chk); chk++);
	    {
		if (!*chk)
		{
		    if (d->showstr_head)
        	    {
            		free_string(d->showstr_head);
            		d->showstr_head = 0;
        	    }
        	    d->showstr_point  = 0;
    		}
	    }
	    return;
	}
    }
    return;
}
	

/* quick sex fixer */
void fix_sex(CHAR_DATA *ch)
{
    if (ch->sex < 0 || ch->sex > 2)
    	ch->sex = IS_NPC(ch) ? 0 : ch->pcdata->true_sex;
}

void act (const char *format, CHAR_DATA *ch, const void *arg1, 
		const void *arg2, int type)
{
    act_color(format,ch,arg1,arg2,type,POS_RESTING);
}

void act_color( const char *format, CHAR_DATA *ch, const void *arg1, 
	      const void *arg2, int type, int min_pos, ... )
{
    static char * const he_she  [] = { "it",  "he",  "she" };
    static char * const him_her [] = { "it",  "him", "her" };
    static char * const his_her [] = { "its", "his", "her" };
 
    char buf[MAX_STRING_LENGTH];
    char fname[MAX_INPUT_LENGTH];
    CHAR_DATA *to;
    CHAR_DATA *vch = (CHAR_DATA *) arg2;
    OBJ_DATA *obj1 = (OBJ_DATA  *) arg1;
    OBJ_DATA *obj2 = (OBJ_DATA  *) arg2;
    const char *str;
    const char *i;
    char *point;
    int n;
    va_list colors; 
 

    /*
     * Discard null and zero-length messages.
     */
    if ( format == NULL || format[0] == '\0' )
        return;

    /* discard null rooms and chars */
    if (ch == NULL || ch->in_room == NULL)
	return;

    to = ch->in_room->people;
    if ( type == TO_VICT )
    {
        if ( vch == NULL )
        {
            bug( "Act: null vch with TO_VICT.", 0 );
            return;
        }

	if (vch->in_room == NULL)
	    return;

        to = vch->in_room->people;
    }
 
    for ( ; to != NULL; to = to->next_in_room )
    {
      va_start(colors,min_pos); 

        if ( to->desc == NULL || to->position < min_pos )
            continue;
 
        if ( type == TO_CHAR && to != ch )
            continue;
        if ( type == TO_VICT && ( to != vch || to == ch ) )
            continue;
        if ( type == TO_ROOM && to == ch )
            continue;
        if ( type == TO_NOTVICT && (to == ch || to == vch) )
            continue;
 
        point   = buf;
        str     = format;
        while ( *str != '\0' )
        {
            if ( *str != '$' )
            {
                *point++ = *str++;
                continue;
            }
            ++str;

                switch ( *str )
                {
                default:  bug( "Act: bad code %d.", *str );
                          i = " <@@@> ";                                break;
                /* Thx alex for 't' idea */
                case 't': i = (char *) arg1;                            break;
                case 'T': i = (char *) arg2;                            break;
                case 'n': i =  
		  (is_affected(ch,gsn_doppelganger) && 
		   !IS_SET(to->act,PLR_HOLYLIGHT)) ? 
		    PERS(ch->doppel,to) : PERS( ch,  to );
		  break;
                case 'N': i =  
		  (is_affected(vch,gsn_doppelganger) && 
		   !IS_SET(to->act,PLR_HOLYLIGHT)) ? 
		    PERS(vch->doppel,to):PERS(vch,  to );
		  break;
                case 'e': i = 
		  (is_affected(ch, gsn_doppelganger) &&
		    !IS_SET(to->act,PLR_HOLYLIGHT)) ?
		    he_she [URANGE(0,ch->doppel->sex,2)] :
		    he_she  [URANGE(0, ch  ->sex, 2)];    
		  break;
                case 'E': i = 
		  (is_affected(vch, gsn_doppelganger) &&
		    !IS_SET(to->act,PLR_HOLYLIGHT)) ?
		    he_she  [URANGE(0, vch->doppel->sex, 2)] :
		    he_she  [URANGE(0, vch->sex, 2)];
		  break;
                case 'm': i = 
		  (is_affected(ch, gsn_doppelganger) &&
		    !IS_SET(to->act,PLR_HOLYLIGHT)) ?
		    him_her [URANGE(0,ch->doppel->sex,2)] :
		    him_her [URANGE(0, ch->sex, 2)];    
		  break;
                case 'M': i = 
		  (is_affected(vch, gsn_doppelganger) &&
		    !IS_SET(to->act,PLR_HOLYLIGHT)) ?
		    him_her  [URANGE(0, vch->doppel->sex, 2)] :
		    him_her  [URANGE(0, vch->sex, 2)];
		  break;
                case 's': i = 
		  (is_affected(ch, gsn_doppelganger) &&
		    !IS_SET(to->act,PLR_HOLYLIGHT)) ?
		    his_her [URANGE(0,ch->doppel->sex,2)] :
		    his_her [URANGE(0, ch  ->sex, 2)];    
		  break;
                case 'S': i = 
		  (is_affected(vch, gsn_doppelganger) &&
		    !IS_SET(to->act,PLR_HOLYLIGHT)) ?
		    his_her  [URANGE(0, vch->doppel->sex, 2)] :
		    his_her  [URANGE(0, vch->sex, 2)];
		  break;
		case 'C': 
		  if (IS_SET(to->act,PLR_COLOR))
		    i = va_arg(colors,char *); 
		  else i = "";
		  break;

		case 'c': 
		  if (IS_SET(to->act,PLR_COLOR))
		    i = CLR_NORMAL ; 
		  else i = "";
		  break;
 
                case 'p':
                    i = can_see_obj( to, obj1 )
                            ? obj1->short_descr
                            : "something";
                    break;
 
                case 'P':
                    i = can_see_obj( to, obj2 )
                            ? obj2->short_descr
                            : "something";
                    break;
 
                case 'd':
                    if ( arg2 == NULL || ((char *) arg2)[0] == '\0' )
                    {
                        i = "door";
                    }
                    else
                    {
                        one_argument( (char *) arg2, fname );
                        i = fname;
                    }
                    break;
                }
 
            ++str;
            while ( ( *point = *i ) != '\0' )
                ++point, ++i;
        }
 
        *point++ = '\n';
        *point++ = '\r';
        /* fix for color prefix and capitalization */
        if (buf[0] == '')
	  {
	    for(n = 1;buf[n] != 'm';n++) ;
	    buf[n+1] = UPPER(buf[n+1]);
	  }
        else buf[0]   = UPPER(buf[0]);
        write_to_buffer( to->desc, buf, point - buf );
    }
    va_end(colors); 
    return;
}

void act_new( const char *format, CHAR_DATA *ch, const void *arg1, 
	      const void *arg2, int type, int min_pos)
{
    act_color(format,ch,arg1,arg2,type,min_pos);
    return;
}



/*
 * Macintosh support functions.
 */
#if defined(macintosh)
int gettimeofday( struct timeval *tp, void *tzp )
{
    tp->tv_sec  = time( NULL );
    tp->tv_usec = 0;
}
#endif



/*
 *  writes bug directly to user screen.
 */

void dump_to_scr( char *text )
{
int a;

  a = strlen( text );
  write(1, text, a);
  return;
}


int log_area_popularity(void)
{
FILE *fp;
AREA_DATA *area;
extern AREA_DATA *area_first;

    system("rm -f area_stat.txt");
    fp = fopen(AREASTAT_FILE, "a");
    fprintf(fp,"\nBooted %sArea popularity statistics (in char * ticks)\n",
            (char *) ctime( &boot_time ));

    for (area = area_first; area != NULL; area = area->next) {
      if (area->count >= 5000000)
        fprintf(fp,"%-60s overflow\n",area->name);
      else
        fprintf(fp,"%-60s %lu\n",area->name,area->count);
    }
    fclose(fp);

 return 1;
}

/*
 * Function for save processes.
 */

void exit_function(int signum)
{
  dump_to_scr("Exiting from the player saver.\n\r");
  wait(NULL);
}

char *get_stat_alias( CHAR_DATA *ch, int where )
{
char *stat;
int istat;

    if ( where == STAT_STR)  {
      istat=get_curr_stat(ch,STAT_STR);
      if      ( istat >  22 ) stat = "Titantic";
      else if ( istat >= 20 ) stat = "Herculian";
      else if ( istat >= 18 ) stat = "Strong";
      else if ( istat >= 14 ) stat = "Average";
      else if ( istat >= 10 ) stat = "Poor";
      else                    stat = "Weak";
      return(stat);
    }
    
    if ( where == STAT_WIS)  {
      istat=get_curr_stat(ch,STAT_WIS);
      if      ( istat >  22 ) stat = "Excellent";
      else if ( istat >= 20 ) stat = "Wise";
      else if ( istat >= 18 ) stat = "Good";
      else if ( istat >= 14 ) stat = "Average";
      else if ( istat >= 10 ) stat = "Dim";
      else                    stat = "Fool";
      return(stat);
    }

    if ( where == STAT_CON)  {
      istat=get_curr_stat(ch,STAT_CON);
      if      ( istat >  22 ) stat = "Iron";
      else if ( istat >= 20 ) stat = "Hearty";
      else if ( istat >= 18 ) stat = "Healty";
      else if ( istat >= 14 ) stat = "Average";
      else if ( istat >= 10 ) stat = "Poor";
      else                    stat = "Fragile";
      return(stat);
    }

    if ( where == STAT_INT)  {
      istat=get_curr_stat(ch,STAT_INT);
      if      ( istat >  22 ) stat = "Genious";
      else if ( istat >= 20 ) stat = "Clever";
      else if ( istat >= 18 ) stat = "Good";
      else if ( istat >= 14 ) stat = "Average";
      else if ( istat >= 10 ) stat = "Poor";
      else                    stat = "Hopeless";
      return(stat);
    }
    
    if ( where == STAT_DEX)  {
      istat=get_curr_stat(ch,STAT_DEX);
      if      ( istat >  22 ) stat = "Fast";
      else if ( istat >= 20 ) stat = "Quick";
      else if ( istat >= 18 ) stat = "Dextrous";
      else if ( istat >= 14 ) stat = "Average";
      else if ( istat >= 10 ) stat = "Clumsy";
      else                    stat = "Slow";
      return(stat);
    }

    if ( where == STAT_CHA)  {
      istat=get_curr_stat(ch,STAT_CHA);
      if      ( istat >  22 ) stat = "Charismatic";
      else if ( istat >= 20 ) stat = "Familier";
      else if ( istat >= 18 ) stat = "Good";
      else if ( istat >= 14 ) stat = "Average";
      else if ( istat >= 10 ) stat = "Poor";
      else                    stat = "Mongol";
      return(stat);
    }

   bug( "stat_alias: Bad stat number.", 0 );
   return(NULL);

}

int sex_ok( CHAR_DATA *ch , int iclass)
{
 return 1;
}

int class_ok( CHAR_DATA *ch , int iclass)
{
 if (pc_race_table[ORG_RACE(ch)].class_mult[iclass] == -1)
	return 0;
 if ( ch->sex == SEX_MALE && iclass == CLASS_NECROMANCER )
    return 0;
 return 1;
}

int align_restrict(CHAR_DATA *ch)
{
 DESCRIPTOR_DATA *d = ch->desc;

    if (IS_SET(pc_race_table[ORG_RACE(ch)].align,CR_GOOD)
	|| IS_SET(class_table[ch->iclass].align,CR_GOOD) )
      {
	write_to_buffer(d, "Your character has good tendencies.\n\r",0);
	ch->alignment = 1000;
	return N_ALIGN_GOOD;
      }

    if (IS_SET(pc_race_table[ORG_RACE(ch)].align,CR_NEUTRAL)
	|| IS_SET(class_table[ch->iclass].align,CR_NEUTRAL) )
      {
	write_to_buffer(d, "Your character has neutral tendencies.\n\r",0);
	ch->alignment = 0;
	return N_ALIGN_NEUTRAL;
      }

    if (IS_SET(pc_race_table[ORG_RACE(ch)].align,CR_EVIL)
	|| IS_SET(class_table[ch->iclass].align,CR_EVIL) )
      {
	write_to_buffer(d, "Your character has evil tendencies.\n\r",0);
	ch->alignment = -1000;
	return N_ALIGN_EVIL;
      }		
   return N_ALIGN_ALL;		
}

int hometown_check(CHAR_DATA *ch)
{
 DESCRIPTOR_DATA *d = ch->desc;

  if (ch->iclass == 10 || ch->iclass == 11)
   {
    write_to_buffer(d,"\n\r",0);
    write_to_buffer(d,"Your hometown is Old midgaard, permenantly.\n\r",0);
    ch->hometown = 4;
    write_to_buffer(d,"\n\r",0);
    return 1;
   }

  if (ORG_RACE(ch) == 11 || ORG_RACE(ch) == 12
	|| ORG_RACE(ch) == 13 || ORG_RACE(ch) == 14)
   {
    write_to_buffer(d,"\n\r",0);
    write_to_buffer(d,"Your hometown is Valley of Titans, permenantly.\n\r",0);
    ch->hometown = 2;
    write_to_buffer(d,"\n\r",0);
    return 1;
   }
 return 0;
}

int hometown_ok(CHAR_DATA *ch, int home)
{
 if (!IS_NEUTRAL(ch) && home == 3) return 0;
 return 1;
}

int ethos_check(CHAR_DATA *ch)
{
 DESCRIPTOR_DATA *d = ch->desc;

  if ( ch->iclass == 4 )
    {
     ch->ethos = 1;
     write_to_buffer( d, "You are Lawful.\n\r", 0 );
     return 1;
    }
  return 0;
}


