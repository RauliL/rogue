/*
 * global variable initializaton
 *
 * @(#)extern.c	4.82 (Berkeley) 02/05/99
 *
 * Rogue: Exploring the Dungeons of Doom
 * Copyright (C) 1980-1983, 1985, 1999 Michael Toy, Ken Arnold and Glenn Wichman
 * All rights reserved.
 *
 * See the file LICENSE.TXT for full copyright and licensing information.
 */

#include <ncurses.h>

#include <roguepp/roguepp.hpp>

bool after;				/* True if we want after daemons */
bool again;				/* Repeating the last command */
int  noscore;				/* Was a wizard sometime */
bool seenstairs;			/* Have seen the stairs (for lsd) */
bool amulet = false;			/* He found the amulet */
bool door_stop = false;			/* Stop running when we pass a door */
bool fight_flush = false;		/* True if toilet input */
bool firstmove = false;			/* First move after setting door_stop */
bool got_ltc = false;			/* We have gotten the local tty chars */
bool has_hit = false;			/* Has a "hit" message pending in msg */
bool in_shell = false;			/* True if executing a shell */
bool inv_describe = true;		/* Say which way items are being used */
bool jump = false;			/* Show running as series of jumps */
bool kamikaze = false;			/* to_death really to DEATH */
bool lower_msg = false;			/* Messages should start w/lower case */
bool move_on = false;			/* Next move shouldn't pick up items */
bool msg_esc = false;			/* Check for ESC from msg's --More-- */
bool passgo = false;			/* Follow passages */
bool playing = true;			/* True until he quits */
bool q_comm = false;			/* Are we executing a 'Q' command? */
bool running = false;			/* True if player is running */
bool save_msg = true;			/* Remember last msg */
bool see_floor = true;			/* Show the lamp illuminated floor */
bool stat_msg = false;			/* Should status() print as a msg() */
bool terse = false;			/* True if we should be short */
bool to_death = false;			/* Fighting is to the death! */
bool tombstone = true;			/* Print out tombstone at end */
#ifdef MASTER
int wizard = false;			/* True if allows wizard commands */
#endif
std::array<bool, 26> pack_used =
{{
    false, false, false, false, false, false, false, false, false,
    false, false, false, false, false, false, false, false, false,
    false, false, false, false, false, false, false, false
}};
char dir_ch;				/* Direction from last get_dir() call */
char file_name[MAXSTR];			/* Save file name */
char huh[MAXSTR];			/* The last message printed */
std::array<std::optional<std::string>, MAXPOTIONS> p_colors;
char prbuf[2*MAXSTR];			/* buffer for sprintfs */
std::array<std::optional<std::string>, MAXRINGS> r_stones;
char runch;				/* Direction player is running */
std::array<std::string, MAXSCROLLS> s_names;
char take;				/* Thing she is taking */
char whoami[MAXSTR];			/* Name of player */
std::array<std::optional<std::string>, MAXSTICKS> ws_made;
std::array<std::optional<std::string>, MAXSTICKS> ws_type;
int  orig_dsusp;			/* Original dsusp char */
char fruit[MAXSTR] =			/* Favorite fruit */
		{ 's', 'l', 'i', 'm', 'e', '-', 'm', 'o', 'l', 'd', '\0' };
char home[MAXSTR] = { '\0' };		/* User's home directory */
std::array<const char*, 3> inv_t_name =
{{
    "Overwrite",
    "Slow",
    "Clear",
}};
char l_last_comm = '\0';		/* Last last_comm */
char l_last_dir = '\0';			/* Last last_dir */
char last_comm = '\0';			/* Last command typed */
char last_dir = '\0';			/* Last direction given */
std::array<const char*, NTRAPS> tr_name =
{{
    "a trapdoor",
    "an arrow trap",
    "a sleeping gas trap",
    "a beartrap",
    "a teleport trap",
    "a poison dart trap",
    "a rust trap",
    "a mysterious trap",
}};


int n_objs;				/* # items listed in inventory() call */
int ntraps;				/* Number of traps on this level */
int hungry_state = 0;			/* How hungry is he */
int inpack = 0;				/* Number of things in pack */
int inv_type = 0;			/* Type of inventory to use */
int level = 1;				/* What level she is on */
int max_hit;				/* Max damage done to her in to_death */
int max_level;				/* Deepest player has gone */
int mpos = 0;				/* Where cursor is on top line */
int no_food = 0;			/* Number of levels without food */
int a_class[MAXARMORS] = {		/* Armor class for each armor type */
	8,	/* LEATHER */
	7,	/* RING_MAIL */
	7,	/* STUDDED_LEATHER */
	6,	/* SCALE_MAIL */
	5,	/* CHAIN_MAIL */
	4,	/* SPLINT_MAIL */
	4,	/* BANDED_MAIL */
	3,	/* PLATE_MAIL */
};

int count = 0;				/* Number of times to repeat command */
FILE *scoreboard = nullptr;	/* File descriptor for score file */
int food_left;				/* Amount of food in hero's stomach */
int lastscore = -1;			/* Score before this turn */
int no_command = 0;			/* Number of turns asleep */
int no_move = 0;			/* Number of turns held in place */
int purse = 0;				/* How much gold he has */
int quiet = 0;				/* Number of quiet turns */
int vf_hit = 0;				/* Number of time flytrap has hit */

int dnum;				/* Dungeon number */
int seed;				/* Random number seed */
int e_levels[] = {
        10L,
	20L,
	40L,
	80L,
       160L,
       320L,
       640L,
      1300L,
      2600L,
      5200L,
     13000L,
     26000L,
     50000L,
    100000L,
    200000L,
    400000L,
    800000L,
   2000000L,
   4000000L,
   8000000L,
	 0L
};

coord delta;				/* Change indicated to get_dir() */
coord oldpos;				/* Position before last look() call */
coord stairs;				/* Location of staircase */

PLACE places[MAXLINES*MAXCOLS];		/* level map */

THING *cur_armor;			/* What he is wearing */
THING *cur_ring[2];			/* Which rings are being worn */
THING *cur_weapon;			/* Which weapon he is weilding */
THING *l_last_pick = nullptr;		/* Last last_pick */
THING *last_pick = nullptr;		/* Last object picked in get_item() */
THING *lvl_obj = nullptr;			/* List of objects on this level */
THING *mlist = nullptr;			/* List of monsters on the level */
THING player;				/* His stats */
					/* restart of game */

WINDOW *hw = nullptr;			/* used as a scratch window */

#define INIT_STATS { 16, 0, 1, 10, 12, "1x4", 12 }

struct stats max_stats = INIT_STATS;	/* The maximum for the player */

room* oldrp;
std::array<room, MAXROOMS> rooms;
std::array<room, MAXPASS> passages =
{{
    { {0, 0}, {0, 0}, {0, 0}, 0, ISGONE|ISDARK, 0 },
    { {0, 0}, {0, 0}, {0, 0}, 0, ISGONE|ISDARK, 0 },
    { {0, 0}, {0, 0}, {0, 0}, 0, ISGONE|ISDARK, 0 },
    { {0, 0}, {0, 0}, {0, 0}, 0, ISGONE|ISDARK, 0 },
    { {0, 0}, {0, 0}, {0, 0}, 0, ISGONE|ISDARK, 0 },
    { {0, 0}, {0, 0}, {0, 0}, 0, ISGONE|ISDARK, 0 },
    { {0, 0}, {0, 0}, {0, 0}, 0, ISGONE|ISDARK, 0 },
    { {0, 0}, {0, 0}, {0, 0}, 0, ISGONE|ISDARK, 0 },
    { {0, 0}, {0, 0}, {0, 0}, 0, ISGONE|ISDARK, 0 },
    { {0, 0}, {0, 0}, {0, 0}, 0, ISGONE|ISDARK, 0 },
    { {0, 0}, {0, 0}, {0, 0}, 0, ISGONE|ISDARK, 0 },
    { {0, 0}, {0, 0}, {0, 0}, 0, ISGONE|ISDARK, 0 },
}};

#define ___ 1
#define XX 10
struct monster monsters[26] =
    {
/* Name		 CARRY	FLAG    str, exp, lvl, amr, hpt, dmg */
{ "aquator",	   0,	ISMEAN,	{ XX, 20,   5,   2, ___, "0x0/0x0" } },
{ "bat",	   0,	ISFLY,	{ XX,  1,   1,   3, ___, "1x2" } },
{ "centaur",	  15,	0,	{ XX, 17,   4,   4, ___, "1x2/1x5/1x5" } },
{ "dragon",	 100,	ISMEAN,	{ XX,5000, 10,  -1, ___, "1x8/1x8/3x10" } },
{ "emu",	   0,	ISMEAN,	{ XX,  2,   1,   7, ___, "1x2" } },
{ "venus flytrap", 0,	ISMEAN,	{ XX, 80,   8,   3, ___, "%%%x0" } },
	/* NOTE: the damage is %%% so that xstr won't merge this */
	/* string with others, since it is written on in the program */
{ "griffin",	  20,	ISMEAN|ISFLY|ISREGEN, { XX,2000, 13,   2, ___, "4x3/3x5" } },
{ "hobgoblin",	   0,	ISMEAN,	{ XX,  3,   1,   5, ___, "1x8" } },
{ "ice monster",   0,	0,	{ XX,  5,   1,   9, ___, "0x0" } },
{ "jabberwock",   70,	0,	{ XX,3000, 15,   6, ___, "2x12/2x4" } },
{ "kestrel",	   0,	ISMEAN|ISFLY,	{ XX,  1,   1,   7, ___, "1x4" } },
{ "leprechaun",	   0,	0,	{ XX, 10,   3,   8, ___, "1x1" } },
{ "medusa",	  40,	ISMEAN,	{ XX,200,   8,   2, ___, "3x4/3x4/2x5" } },
{ "nymph",	 100,	0,	{ XX, 37,   3,   9, ___, "0x0" } },
{ "orc",	  15,	ISGREED,{ XX,  5,   1,   6, ___, "1x8" } },
{ "phantom",	   0,	ISINVIS,{ XX,120,   8,   3, ___, "4x4" } },
{ "quagga",	   0,	ISMEAN,	{ XX, 15,   3,   3, ___, "1x5/1x5" } },
{ "rattlesnake",   0,	ISMEAN,	{ XX,  9,   2,   3, ___, "1x6" } },
{ "snake",	   0,	ISMEAN,	{ XX,  2,   1,   5, ___, "1x3" } },
{ "troll",	  50,	ISREGEN|ISMEAN,{ XX, 120, 6, 4, ___, "1x8/1x8/2x6" } },
{ "black unicorn", 0,	ISMEAN,	{ XX,190,   7,  -2, ___, "1x9/1x9/2x9" } },
{ "vampire",	  20,	ISREGEN|ISMEAN,{ XX,350,   8,   1, ___, "1x10" } },
{ "wraith",	   0,	0,	{ XX, 55,   5,   4, ___, "1x6" } },
{ "xeroc",	  30,	0,	{ XX,100,   7,   7, ___, "4x4" } },
{ "yeti",	  30,	0,	{ XX, 50,   4,   6, ___, "1x6/1x6" } },
{ "zombie",	   0,	ISMEAN,	{ XX,  6,   2,   8, ___, "1x8" } }
    };
#undef ___
#undef XX

struct obj_info things[NUMTHINGS] = {
    { nullptr,			26 },	/* potion */
    { nullptr,			36 },	/* scroll */
    { nullptr,			16 },	/* food */
    { nullptr,			 7 },	/* weapon */
    { nullptr,			 7 },	/* armor */
    { nullptr,			 4 },	/* ring */
    { nullptr,			 4 },	/* stick */
};

struct obj_info arm_info[MAXARMORS] = {
    { "leather armor",		 20,	 20, nullptr, false },
    { "ring mail",		 15,	 25, nullptr, false },
    { "studded leather armor",	 15,	 20, nullptr, false },
    { "scale mail",		 13,	 30, nullptr, false },
    { "chain mail",		 12,	 75, nullptr, false },
    { "splint mail",		 10,	 80, nullptr, false },
    { "banded mail",		 10,	 90, nullptr, false },
    { "plate mail",		  5,	150, nullptr, false },
};
struct obj_info pot_info[MAXPOTIONS] = {
    { "confusion",		 7,   5, nullptr, false },
    { "hallucination",		 8,   5, nullptr, false },
    { "poison",			 8,   5, nullptr, false },
    { "gain strength",		13, 150, nullptr, false },
    { "see invisible",		 3, 100, nullptr, false },
    { "healing",		13, 130, nullptr, false },
    { "monster detection",	 6, 130, nullptr, false },
    { "magic detection",	 6, 105, nullptr, false },
    { "raise level",		 2, 250, nullptr, false },
    { "extra healing",		 5, 200, nullptr, false },
    { "haste self",		 5, 190, nullptr, false },
    { "restore strength",	13, 130, nullptr, false },
    { "blindness",		 5,   5, nullptr, false },
    { "levitation",		 6,  75, nullptr, false },
};
struct obj_info ring_info[MAXRINGS] = {
    { "protection",		 9, 400, nullptr, false },
    { "add strength",		 9, 400, nullptr, false },
    { "sustain strength",	 5, 280, nullptr, false },
    { "searching",		10, 420, nullptr, false },
    { "see invisible",		10, 310, nullptr, false },
    { "adornment",		 1,  10, nullptr, false },
    { "aggravate monster",	10,  10, nullptr, false },
    { "dexterity",		 8, 440, nullptr, false },
    { "increase damage",	 8, 400, nullptr, false },
    { "regeneration",		 4, 460, nullptr, false },
    { "slow digestion",		 9, 240, nullptr, false },
    { "teleportation",		 5,  30, nullptr, false },
    { "stealth",		 7, 470, nullptr, false },
    { "maintain armor",		 5, 380, nullptr, false },
};
struct obj_info scr_info[MAXSCROLLS] = {
    { "monster confusion",		 7, 140, nullptr, false },
    { "magic mapping",			 4, 150, nullptr, false },
    { "hold monster",			 2, 180, nullptr, false },
    { "sleep",				 3,   5, nullptr, false },
    { "enchant armor",			 7, 160, nullptr, false },
    { "identify potion",		10,  80, nullptr, false },
    { "identify scroll",		10,  80, nullptr, false },
    { "identify weapon",		 6,  80, nullptr, false },
    { "identify armor",		 	 7, 100, nullptr, false },
    { "identify ring, wand or staff",	10, 115, nullptr, false },
    { "scare monster",			 3, 200, nullptr, false },
    { "food detection",			 2,  60, nullptr, false },
    { "teleportation",			 5, 165, nullptr, false },
    { "enchant weapon",			 8, 150, nullptr, false },
    { "create monster",			 4,  75, nullptr, false },
    { "remove curse",			 7, 105, nullptr, false },
    { "aggravate monsters",		 3,  20, nullptr, false },
    { "protect armor",			 2, 250, nullptr, false },
};
struct obj_info weap_info[MAXWEAPONS + 1] = {
    { "mace",				11,   8, nullptr, false },
    { "long sword",			11,  15, nullptr, false },
    { "short bow",			12,  15, nullptr, false },
    { "arrow",				12,   1, nullptr, false },
    { "dagger",				 8,   3, nullptr, false },
    { "two handed sword",		10,  75, nullptr, false },
    { "dart",				12,   2, nullptr, false },
    { "shuriken",			12,   5, nullptr, false },
    { "spear",				12,   5, nullptr, false },
    { nullptr, 0 },	/* DO NOT REMOVE: fake entry for dragon's breath */
};
struct obj_info ws_info[MAXSTICKS] = {
    { "light",			12, 250, nullptr, false },
    { "invisibility",		 6,   5, nullptr, false },
    { "lightning",		 3, 330, nullptr, false },
    { "fire",			 3, 330, nullptr, false },
    { "cold",			 3, 330, nullptr, false },
    { "polymorph",		15, 310, nullptr, false },
    { "magic missile",		10, 170, nullptr, false },
    { "haste monster",		10,   5, nullptr, false },
    { "slow monster",		11, 350, nullptr, false },
    { "drain life",		 9, 300, nullptr, false },
    { "nothing",		 1,   5, nullptr, false },
    { "teleport away",		 6, 340, nullptr, false },
    { "teleport to",		 6,  50, nullptr, false },
    { "cancellation",		 5, 280, nullptr, false },
};

struct h_list helpstr[] = {
    {'?',	"	prints help",				true},
    {'/',	"	identify object",			true},
    {'h',	"	left",					true},
    {'j',	"	down",					true},
    {'k',	"	up",					true},
    {'l',	"	right",					true},
    {'y',	"	up & left",				true},
    {'u',	"	up & right",				true},
    {'b',	"	down & left",				true},
    {'n',	"	down & right",				true},
    {'H',	"	run left",				false},
    {'J',	"	run down",				false},
    {'K',	"	run up",				false},
    {'L',	"	run right",				false},
    {'Y',	"	run up & left",				false},
    {'U',	"	run up & right",			false},
    {'B',	"	run down & left",			false},
    {'N',	"	run down & right",			false},
    {CTRL('H'),	"	run left until adjacent",		false},
    {CTRL('J'),	"	run down until adjacent",		false},
    {CTRL('K'),	"	run up until adjacent",			false},
    {CTRL('L'),	"	run right until adjacent",		false},
    {CTRL('Y'),	"	run up & left until adjacent",		false},
    {CTRL('U'),	"	run up & right until adjacent",		false},
    {CTRL('B'),	"	run down & left until adjacent",	false},
    {CTRL('N'),	"	run down & right until adjacent",	false},
    {'\0',	"	<SHIFT><dir>: run that way",		true},
    {'\0',	"	<CTRL><dir>: run till adjacent",	true},
    {'f',	"<dir>	fight till death or near death",	true},
    {'t',	"<dir>	throw something",			true},
    {'m',	"<dir>	move onto without picking up",		true},
    {'z',	"<dir>	zap a wand in a direction",		true},
    {'^',	"<dir>	identify trap type",			true},
    {'s',	"	search for trap/secret door",		true},
    {'>',	"	go down a staircase",			true},
    {'<',	"	go up a staircase",			true},
    {'.',	"	rest for a turn",			true},
    {',',	"	pick something up",			true},
    {'i',	"	inventory",				true},
    {'I',	"	inventory single item",			true},
    {'q',	"	quaff potion",				true},
    {'r',	"	read scroll",				true},
    {'e',	"	eat food",				true},
    {'w',	"	wield a weapon",			true},
    {'W',	"	wear armor",				true},
    {'T',	"	take armor off",			true},
    {'P',	"	put on ring",				true},
    {'R',	"	remove ring",				true},
    {'d',	"	drop object",				true},
    {'c',	"	call object",				true},
    {'a',	"	repeat last command",			true},
    {')',	"	print current weapon",			true},
    {']',	"	print current armor",			true},
    {'=',	"	print current rings",			true},
    {'@',	"	print current stats",			true},
    {'D',	"	recall what's been discovered",		true},
    {'o',	"	examine/set options",			true},
    {CTRL('R'),	"	redraw screen",				true},
    {CTRL('P'),	"	repeat last message",			true},
    {ESCAPE,	"	cancel command",			true},
    {'S',	"	save game",				true},
    {'Q',	"	quit",					true},
    {'!',	"	shell escape",				true},
    {'F',	"<dir>	fight till either of you dies",		true},
    {'v',	"	print version number",			true},
    {0,		nullptr }
};
