/*
 * save and restore routines
 *
 * @(#)save.c	4.33 (Berkeley) 06/01/83
 *
 * Rogue: Exploring the Dungeons of Doom
 * Copyright (C) 1980-1983, 1985, 1999 Michael Toy, Ken Arnold and Glenn Wichman
 * All rights reserved.
 *
 * See the file LICENSE.TXT for full copyright and licensing information.
 */

#include <cerrno>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <ncurses.h>
#include "rogue.hpp"
#include "score.hpp"

typedef struct stat STAT;

extern char version[], encstr[];

static STAT sbuf;

/*
 * save_game:
 *	Implement the "save game" command
 */

void
save_game()
{
    FILE *savef;
    int c;
    char buf[MAXSTR];

    /*
     * get file name
     */
    mpos = 0;
over:
    if (file_name[0] != '\0')
    {
	for (;;)
	{
	    msg("save file (%s)? ", file_name);
	    c = readchar();
	    mpos = 0;
	    if (c == ESCAPE)
	    {
		msg("");
		return;
	    }
	    else if (c == 'n' || c == 'N' || c == 'y' || c == 'Y')
		break;
	    else
		msg("please answer Y or N");
	}
	if (c == 'y' || c == 'Y')
	{
	    addstr("Yes\n");
	    refresh();
	    strcpy(buf, file_name);
	    goto gotfile;
	}
    }

    do
    {
	mpos = 0;
	msg("file name: ");
	buf[0] = '\0';
	if (get_str(buf, stdscr) == QUIT)
	{
quit_it:
	    msg("");
	    return;
	}
	mpos = 0;
gotfile:
	/*
	 * test to see if the file exists
	 */
	if (stat(buf, &sbuf) >= 0)
	{
	    for (;;)
	    {
		msg("File exists.  Do you wish to overwrite it?");
		mpos = 0;
		if ((c = readchar()) == ESCAPE)
		    goto quit_it;
		if (c == 'y' || c == 'Y')
		    break;
		else if (c == 'n' || c == 'N')
		    goto over;
		else
		    msg("Please answer Y or N");
	    }
	    msg("file name: %s", buf);
	    md_unlink(file_name);
	}
	strcpy(file_name, buf);
	if ((savef = std::fopen(file_name, "wb")) == nullptr)
	    msg(strerror(errno));
    } while (savef == nullptr);

    save_file(savef);
    /* NOTREACHED */
}

/*
 * auto_save:
 *	Automatically save a file.  This is used if a HUP signal is
 *	recieved
 */

void
auto_save(int sig)
{
    FILE *savef;
    NOOP(sig);

    md_ignoreallsignals();
    if (file_name[0] != '\0' && ((savef = std::fopen(file_name, "wb")) != nullptr ||
	(md_unlink_open_file(file_name, savef) >= 0 && (savef = std::fopen(file_name, "wb")) != nullptr)))
	    save_file(savef);
    exit(0);
}

/*
 * save_file:
 *	Write the saved game on the file
 */

void
save_file(FILE *savef)
{
    char buf[80];
    mvcur(0, COLS - 1, LINES - 1, 0);
    putchar('\n');
    endwin();
    resetltchars();
    md_chmod(file_name, 0400);
    encwrite(version, strlen(version)+1, savef);
    sprintf(buf,"%d x %d\n", LINES, COLS);
    encwrite(buf,80,savef);
    rs_save_file(savef);
    fflush(savef);
    fclose(savef);
    exit(0);
}

/*
 * restore:
 *	Restore a saved game from a file with elaborate checks for file
 *	integrity from cheaters
 */
bool
restore(const char* file, char** envp)
{
    extern char** environ;
    FILE* inf;
    char buf[MAXSTR];
    int syml;
    STAT sbuf2;
    int lines;
    int cols;

    if (!std::strcmp(file, "-r"))
    {
        file = file_name;
    }

    md_tstphold();

    if (!(inf = std::fopen(file, "rb")))
    {
        std::perror(file);

        return false;
    }

    stat(file, &sbuf2);
    syml = is_symlink(file);

    std::fflush(stdout);
    encread(buf, std::strlen(version) + 1, inf);
    if (std::strcmp(buf, version))
    {
        std::printf("Sorry, saved game is out of date.\n");

        return false;
    }
    encread(buf, 80, inf);
    std::sscanf(buf, "%d x %d\n", &lines, &cols);

    // Start up cursor package
    initscr();
    keypad(stdscr, 1);

    if (lines > LINES)
    {
        endwin();
        std::printf(
            "Sorry, original game was played on a screen with %d lines.\n",
            lines
        );
        std::printf(
            "Current screen only has %d lines. Unable to restore game\n",
            LINES
        );

        return false;
    }
    if (cols > COLS)
    {
        endwin();
        std::printf(
            "Sorry, original game was played on a screen with %d columns.\n",
            cols
        );
        std::printf(
            "Current screen only has %d columns. Unable to restore game\n",
            COLS
        );

        return false;
    }

    hw = newwin(LINES, COLS, 0, 0);
    setup();

    rs_restore_file(inf);
    /*
     * we do not close the file so that we will have a hold of the
     * inode for as long as possible
     */

    if (
#if defined(MASTER)
    !wizard &&
#endif
        md_unlink_open_file(file, inf) < 0)
    {
        std::printf("Cannot unlink file\n");

        return false;
    }
    mpos = 0;

    clearok(stdscr,true);
    /*
     * defeat multiple restarting from the same place
     */
#if defined(MASTER)
    if (!wizard)
#endif
    if (sbuf2.st_nlink != 1 || syml)
    {
        endwin();
        std::printf("\nCannot restore from a linked file\n");

        return false;
    }

    if (pstats.s_hpt <= 0)
    {
        endwin();
        std::printf("\n\"He's dead, Jim\"\n");

        return false;
    }

    md_tstpresume();

    environ = envp;
    std::strcpy(file_name, file);
    clearok(curscr, true);
    std::srand(md_getpid());
    msg("file name: %s", file);
    playit();

    return true;
}

/*
 * encwrite:
 *	Perform an encrypted write
 */
std::size_t
encwrite(const char* start, std::size_t size, FILE* outf)
{
    extern char statlist[];
    const std::size_t o_size = size;
    const char* e1 = encstr;
    const char* e2 = statlist;
    char fb = 0;

    while (size)
    {
        int temp;

        if (std::fputc(*start++ ^ *e1 ^ *e2 ^ fb, outf) == EOF)
        {
            break;
        }

        temp = *e1++;
        fb = fb + static_cast<char>(temp * *e2++);
        if (*e1 == '\0')
        {
            e1 = encstr;
        }
        if (*e2 == '\0')
        {
            e2 = statlist;
        }
        --size;
    }

    return o_size - size;
}

/*
 * encread:
 *	Perform an encrypted read
 */
std::size_t
encread(char* start, std::size_t size, FILE* inf)
{
    extern char statlist[];
    char fb = 0;
    const char* e1 = encstr;
    const char* e2 = statlist;
    const auto read_size = std::fread(start, 1, size, inf);

    if (read_size < size)
    {
        return read_size;
    }

    while (size--)
    {
        int temp;

        *start++ ^= *e1 ^ *e2 ^ fb;
        temp = *e1++;
        fb = fb + static_cast<char>(temp * *e2++);
        if (*e1 == '\0')
        {
            e1 = encstr;
        }
        if (*e2 == '\0')
        {
            e2 = statlist;
        }
    }

    return read_size;
}

static char scoreline[100];
/*
 * read_scrore
 *	Read in the score file
 */
void
rd_score(SCORE *top_ten)
{
    unsigned int i;

	if (scoreboard == nullptr)
		return;

	rewind(scoreboard);

	for(i = 0; i < numscores; i++)
    {
        encread(top_ten[i].sc_name, MAXSTR, scoreboard);
        encread(scoreline, 100, scoreboard);
        sscanf(scoreline, " %u %d %u %hu %d %x \n",
            &top_ten[i].sc_uid, &top_ten[i].sc_score,
            &top_ten[i].sc_flags, &top_ten[i].sc_monster,
            &top_ten[i].sc_level, &top_ten[i].sc_time);
    }

	rewind(scoreboard);
}

/*
 * write_scrore
 *	Read in the score file
 */
void
wr_score(SCORE *top_ten)
{
    unsigned int i;

	if (scoreboard == nullptr)
		return;

	rewind(scoreboard);

    for(i = 0; i < numscores; i++)
    {
          memset(scoreline,0,100);
          encwrite(top_ten[i].sc_name, MAXSTR, scoreboard);
          sprintf(scoreline, " %u %d %u %hu %d %x \n",
              top_ten[i].sc_uid, top_ten[i].sc_score,
              top_ten[i].sc_flags, top_ten[i].sc_monster,
              top_ten[i].sc_level, top_ten[i].sc_time);
          encwrite(scoreline,100,scoreboard);
    }

	rewind(scoreboard);
}
