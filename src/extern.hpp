/*
 * Defines for things used in mach_dep.c
 *
 * @(#)extern.h	4.35 (Berkeley) 02/05/99
 *
 * Rogue: Exploring the Dungeons of Doom
 * Copyright (C) 1980-1983, 1985, 1999 Michael Toy, Ken Arnold and Glenn Wichman
 * All rights reserved.
 *
 * See the file LICENSE.TXT for full copyright and licensing information.
 */
#pragma once

#include <cstdlib>
#include <string>

#include "config.hpp"

/*
 * Don't change the constants, since they are used for sizes in many
 * places in the program.
 */

#undef SIGTSTP

#define MAXSTR		1024	/* maximum length of strings */
#define MAXLINES	32	/* maximum number of screen lines used */
#define MAXCOLS		80	/* maximum number of screen columns used */

#define RN		(((seed = seed*11109+13849) >> 16) & 0xffff)
#ifdef CTRL
#undef CTRL
#endif
#define CTRL(c)		(c & 037)

/*
 * Now all the global variables
 */

extern bool	got_ltc, in_shell;
extern int	wizard;
extern char	fruit[], prbuf[], whoami[];
extern int orig_dsusp;
extern FILE	*scoreboard;

/*
 * Function types
 */

void    auto_save(int);
void come_down(int);
void doctor(int);
void	end_line();
void    endit(int sig);
void fatal(const std::string& message);
void	getltchars();
void land(int);
void    leave(int);
void my_exit(int st);
void nohaste(int);
void	playit();
void    playltchars();
void	print_disc(char);
void    quit(int);
void    resetltchars();
void rollwand(int);
void runners(int);
void sight(int);
void stomach(int);
void swander(int);
void	tstp(int ignored);
void unconfuse(int);
void unsee(int);
void visuals(int);

char add_line(const char* fmt, const char* arg);

char	*killname(char monst, bool doart);

#ifdef CHECKTIME
int	checkout();
#endif

int md_chmod(const std::string& filename, int mode);
const char* md_crypt(const std::string& key, const std::string& salt);
int	md_dsuspchar();
int	md_erasechar();
std::string md_gethomedir();
std::string md_getusername();
int	md_getuid();
char* md_getpass(const std::string& prompt);
int	md_getpid();
std::string md_getrealname(int uid);
void	md_init();
int	md_killchar();
void	md_normaluser();
void	md_raw_standout();
void	md_raw_standend();
int	md_readchar();
int	md_setdsuspchar(int c);
int	md_shellescape();
void	md_sleep(int s);
int	md_suspchar();
int	md_hasclreol();
int	md_unlink(const std::string& filename);
int md_unlink_open_file(const std::string& filename, FILE* inf);
void md_tstpsignal();
void md_tstphold();
void md_tstpresume();
void md_ignoreallsignals();
void md_onsignal_autosave();
void md_onsignal_exit();
void md_onsignal_default();
