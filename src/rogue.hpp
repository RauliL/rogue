/*
 * Rogue definitions and variable declarations
 *
 * @(#)rogue.h	5.42 (Berkeley) 08/06/83
 *
 * Rogue: Exploring the Dungeons of Doom
 * Copyright (C) 1980-1983, 1985, 1999 Michael Toy, Ken Arnold and Glenn Wichman
 * All rights reserved.
 *
 * See the file LICENSE.TXT for full copyright and licensing information.
 */
#pragma once

#include <array>
#include <optional>
#include "extern.hpp"

#undef lines

#define NOOP(x) (x += 0)
#define CCHAR(x) ( (char) (x & A_CHARTEXT) )

/*
 * Maximum number of different things
 */
static constexpr std::size_t MAXROOMS = 9;
static constexpr std::size_t MAXTHINGS = 9;
static constexpr std::size_t MAXOBJ = 9;
static constexpr std::size_t MAXPACK = 23;
static constexpr std::size_t MAXTRAPS = 10;
#define AMULETLEVEL	26
#define	NUMTHINGS	7	/* number of types of things */
/** Upper limit on number of passages. */
static constexpr std::size_t MAXPASS = 13;
#define	NUMLINES	24
#define	NUMCOLS		80
#define STATLINE		(NUMLINES - 1)
#define BORE_LEVEL	50

/*
 * return values for get functions
 */
#define	NORM	0	/* normal exit */
#define	QUIT	1	/* quit option setting */
#define	MINUS	2	/* back up one option */

/*
 * inventory types
 */
#define	INV_OVER	0
#define	INV_SLOW	1
#define	INV_CLEAR	2

/*
 * All the fun defines
 */
#define when		break;case
#define otherwise	break;default
#define until(expr)	while(!(expr))
#define next(ptr)	(*ptr).l_next
#define prev(ptr)	(*ptr).l_prev
#define winat(y,x)	(moat(y,x) != nullptr ? moat(y,x)->t_disguise : chat(y,x))
#define ce(a,b)		((a).x == (b).x && (a).y == (b).y)
#define hero		player.t_pos
#define pstats		player.t_stats
#define pack		player.t_pack
#define proom		player.t_room
#define max_hp		player.t_stats.s_maxhp
#define attach(a,b)	_attach(&a,b)
#define detach(a,b)	_detach(&a,b)
#define free_list(a)	_free_list(&a)
#undef max
#define max(a,b)	((a) > (b) ? (a) : (b))
#define on(thing,flag)	((bool)(((thing).t_flags & (flag)) != 0))
#define GOLDCALC	(rnd(50 + 10 * level) + 2)
#define ISRING(h,r)	(cur_ring[h] != nullptr && cur_ring[h]->o_which == r)
#define ISWEARING(r)	(ISRING(LEFT, r) || ISRING(RIGHT, r))
#define ISMULT(type) 	(type == POTION || type == SCROLL || type == FOOD)
#define INDEX(y,x)	(&places[((x) << 5) + (y)])
#define chat(y,x)	(places[((x) << 5) + (y)].p_ch)
#define flat(y,x)	(places[((x) << 5) + (y)].p_flags)
#define moat(y,x)	(places[((x) << 5) + (y)].p_monst)
#define unc(cp)		(cp).y, (cp).x
#ifdef MASTER
#define debug		if (wizard) msg
#endif

/*
 * things that appear on the screens
 */

enum object_type : int
{
    // TODO: \u00A1
    POTION = '!',
    // TODO: \u266A
    SCROLL = '?',
    // TODO: \u2663
    FOOD = ':',
    // TODO: \u2191
    WEAPON = ')',
    // TODO: \u25D8
    ARMOR = ']',
    // TODO: \u25CB
    RING = '=',
    // TODO: \u03C4
    STICK = '/',
    // TODO: \u263C
    GOLD = '*',
    // TODO: \u2640
    AMULET = ',',
};

// TODO: \u2592
#define PASSAGE		'#'
// TODO: \u256c
#define DOOR		'+'
// TODO: \u00b7
#define FLOOR		'.'
// TODO: \u263a
#define PLAYER		'@'
// TODO: \u2666
#define TRAP		'^'
// TODO: \u2261
#define STAIRS		'%'
#define MAGIC		'$'
#define CALLABLE	-1
#define R_OR_S		-2

/*
 * Various constants
 */
#define BEARTIME	spread(3)
#define SLEEPTIME	spread(5)
#define HOLDTIME	spread(2)
#define WANDERTIME	spread(70)
#define BEFORE		spread(1)
#define AFTER		spread(2)
#define HEALTIME	30
#define HUHDURATION	20
#define SEEDURATION	850
#define HUNGERTIME	1300
#define MORETIME	150
#define STOMACHSIZE	2000
#define STARVETIME	850
#define ESCAPE		27
#define LEFT		0
#define RIGHT		1
#define BOLT_LENGTH	6
#define LAMPDIST	3
#ifdef MASTER
#ifndef PASSWD
#define	PASSWD		"mTBellIQOsLNA"
#endif
#endif

/*
 * Save against things
 */
#define VS_POISON	00
#define VS_PARALYZATION	00
#define VS_DEATH	00
#define VS_BREATH	02
#define VS_MAGIC	03

/*
 * Various flag bits
 */
/* flags for rooms */
#define ISDARK	0000001		/* room is dark */
#define ISGONE	0000002		/* room is gone (a corridor) */
#define ISMAZE	0000004		/* room is gone (a corridor) */

/* flags for objects */
#define ISCURSED 000001		/* object is cursed */
#define ISKNOW	0000002		/* player knows details about the object */
#define ISMISL	0000004		/* object is a missile type */
#define ISMANY	0000010		/* object comes in groups */
/*	ISFOUND 0000020		...is used for both objects and creatures */
#define	ISPROT	0000040		/* armor is permanently protected */

/* flags for creatures */
#define CANHUH	0000001		/* creature can confuse */
#define CANSEE	0000002		/* creature can see invisible creatures */
#define ISBLIND	0000004		/* creature is blind */
#define ISCANC	0000010		/* creature has special qualities cancelled */
#define ISLEVIT	0000010		/* hero is levitating */
#define ISFOUND	0000020		/* creature has been seen (used for objects) */
#define ISGREED	0000040		/* creature runs to protect gold */
#define ISHASTE	0000100		/* creature has been hastened */
#define ISTARGET 000200		/* creature is the target of an 'f' command */
#define ISHELD	0000400		/* creature has been held */
#define ISHUH	0001000		/* creature is confused */
#define ISINVIS	0002000		/* creature is invisible */
#define ISMEAN	0004000		/* creature can wake when player enters room */
#define ISHALU	0004000		/* hero is on acid trip */
#define ISREGEN	0010000		/* creature can regenerate */
#define ISRUN	0020000		/* creature is running at the player */
#define SEEMONST 040000		/* hero can detect unseen monsters */
#define ISFLY	0040000		/* creature can fly */
#define ISSLOW	0100000		/* creature has been slowed */

/*
 * Flags for level map
 */
#define F_PASS		0x80		/* is a passageway */
#define F_SEEN		0x40		/* have seen this spot before */
#define F_DROPPED	0x20		/* object was dropped here */
#define F_LOCKED	0x20		/* door is locked */
#define F_REAL		0x10		/* what you see is what you get */
#define F_PNUM		0x0f		/* passage number mask */
#define F_TMASK		0x07		/* trap number mask */

/**
 * Trap types.
 */
enum trap_type : int
{
    T_DOOR = 0,
    T_ARROW = 1,
    T_SLEEP = 2,
    T_BEAR = 3,
    T_TELEP = 4,
    T_DART = 5,
    T_RUST = 6,
    T_MYST = 7,
};

static constexpr std::size_t NTRAPS = 8;

/**
 * Potion types.
 */
enum potion_type : int
{
    P_CONFUSE = 0,
    P_LSD = 1,
    P_POISON = 2,
    P_STRENGTH = 3,
    P_SEEINVIS = 4,
    P_HEALING = 5,
    P_MFIND = 6,
    P_TFIND = 7,
    P_RAISE = 8,
    P_XHEAL = 9,
    P_HASTE = 10,
    P_RESTORE = 11,
    P_BLIND = 12,
    P_LEVIT = 13,
};

static constexpr std::size_t MAXPOTIONS = 14;

/**
 * Scroll types.
 */
enum scroll_type : int
{
    S_CONFUSE = 0,
    S_MAP = 1,
    S_HOLD = 2,
    S_SLEEP = 3,
    S_ARMOR = 4,
    S_ID_POTION = 5,
    S_ID_SCROLL = 6,
    S_ID_WEAPON = 7,
    S_ID_ARMOR = 8,
    S_ID_R_OR_S = 9,
    S_SCARE = 10,
    S_FDET = 11,
    S_TELEP = 12,
    S_ENCH = 13,
    S_CREATE = 14,
    S_REMOVE = 15,
    S_AGGR = 16,
    S_PROTECT = 17,
};

static constexpr std::size_t MAXSCROLLS = 18;

/**
 * Weapon types.
 */
enum weapon_type : int
{
    MACE = 0,
    SWORD = 1,
    BOW = 2,
    ARROW = 3,
    DAGGER = 4,
    TWOSWORD = 5,
    DART = 6,
    SHIRAKEN = 7,
    SPEAR = 8,
    /** Fake entry for dragon breath (ick). */
    FLAME = 9,
};

// This should equal FLAME.
static constexpr std::size_t MAXWEAPONS = 9;

/**
 * Armor types.
 */
enum armor_type : int
{
    LEATHER = 0,
    RING_MAIL = 1,
    STUDDED_LEATHER = 2,
    SCALE_MAIL = 3,
    CHAIN_MAIL = 4,
    SPLINT_MAIL = 5,
    BANDED_MAIL = 6,
    PLATE_MAIL = 7,
};

static constexpr std::size_t MAXARMORS = 8;

/**
 * Ring types.
 */
enum ring_type : int
{
    R_PROTECT = 0,
    R_ADDSTR = 1,
    R_SUSTSTR = 2,
    R_SEARCH = 3,
    R_SEEINVIS = 4,
    R_NOP = 5,
    R_AGGR = 6,
    R_ADDHIT = 7,
    R_ADDDAM = 8,
    R_REGEN = 9,
    R_DIGEST = 10,
    R_TELEPORT = 11,
    R_STEALTH = 12,
    R_SUSTARM = 13,
};

static constexpr std::size_t MAXRINGS = 14;

/*
 * Rod/Wand/Staff types.
 */
enum stick_type : int
{
    WS_LIGHT = 0,
    WS_INVIS = 1,
    WS_ELECT = 2,
    WS_FIRE = 3,
    WS_COLD = 4,
    WS_POLYMORPH = 5,
    WS_MISSILE = 6,
    WS_HASTE_M = 7,
    WS_SLOW_M = 8,
    WS_DRAIN = 9,
    WS_NOP = 10,
    WS_TELAWAY = 11,
    WS_TELTO = 12,
    WS_CANCEL = 13,
};

static constexpr std::size_t MAXSTICKS = 14;

/*
 * Now we define the structures and types
 */

/*
 * Help list
 */
struct h_list
{
    char h_ch;
    const char* h_desc;
    bool h_print;
};

/**
 * Coordinate data type.
 */
struct coord
{
    int x;
    int y;
};

typedef unsigned int str_t;

/*
 * Stuff about objects
 */
struct obj_info
{
    const char* oi_name;
    int oi_prob;
    int oi_worth;
    char* oi_guess;
    bool oi_know;
};

/*
 * Room structure
 */
struct room {
    coord r_pos;			/* Upper left corner */
    coord r_max;			/* Size of room */
    coord r_gold;			/* Where the gold is */
    int r_goldval;			/* How much the gold is worth */
    short r_flags;			/* info about the room */
    int r_nexits;			/* Number of exits */
    coord r_exit[12];			/* Where the exits are */
};

/*
 * Structure describing a fighting being
 */
struct stats {
    str_t s_str;			/* Strength */
    int s_exp;				/* Experience */
    int s_lvl;				/* level of mastery */
    int s_arm;				/* Armor class */
    int s_hpt;			/* Hit points */
    char s_dmg[13];			/* String describing damage done */
    int  s_maxhp;			/* Max hit points */
};

/*
 * Structure for monsters and player
 */
union thing {
    struct {
	union thing *_l_next, *_l_prev;	/* Next pointer in link */
	coord _t_pos;			/* Position */
	bool _t_turn;			/* If slowed, is it a turn to move */
	char _t_type;			/* What it is */
	char _t_disguise;		/* What mimic looks like */
	char _t_oldch;			/* Character that was where it was */
	coord *_t_dest;			/* Where it is running to */
	short _t_flags;			/* State word */
	struct stats _t_stats;		/* Physical description */
	struct room *_t_room;		/* Current room for thing */
	union thing *_t_pack;		/* What the thing is carrying */
        int _t_reserved;
    } _t;
    struct {
	union thing *_l_next, *_l_prev;	/* Next pointer in link */
	int _o_type;			/* What kind of object it is */
	coord _o_pos;			/* Where it lives on the screen */
	char *_o_text;			/* What it says if you read it */
	int  _o_launch;			/* What you need to launch it */
	char _o_packch;			/* What character it is in the pack */
	char _o_damage[8];		/* Damage if used like sword */
	char _o_hurldmg[8];		/* Damage if thrown */
	int _o_count;			/* count for plural objects */
	int _o_which;			/* Which object of a type it is */
	int _o_hplus;			/* Plusses to hit */
	int _o_dplus;			/* Plusses to damage */
	int _o_arm;			/* Armor protection */
	int _o_flags;			/* information about objects */
	int _o_group;			/* group number for this object */
	char *_o_label;			/* Label for object */
    } _o;
};

typedef union thing THING;

#define l_next		_t._l_next
#define l_prev		_t._l_prev
#define t_pos		_t._t_pos
#define t_turn		_t._t_turn
#define t_type		_t._t_type
#define t_disguise	_t._t_disguise
#define t_oldch		_t._t_oldch
#define t_dest		_t._t_dest
#define t_flags		_t._t_flags
#define t_stats		_t._t_stats
#define t_pack		_t._t_pack
#define t_room		_t._t_room
#define t_reserved      _t._t_reserved
#define o_type		_o._o_type
#define o_pos		_o._o_pos
#define o_text		_o._o_text
#define o_launch	_o._o_launch
#define o_packch	_o._o_packch
#define o_damage	_o._o_damage
#define o_hurldmg	_o._o_hurldmg
#define o_count		_o._o_count
#define o_which		_o._o_which
#define o_hplus		_o._o_hplus
#define o_dplus		_o._o_dplus
#define o_arm		_o._o_arm
#define o_charges	o_arm
#define o_goldval	o_arm
#define o_flags		_o._o_flags
#define o_group		_o._o_group
#define o_label		_o._o_label

/*
 * describe a place on the level map
 */
typedef struct {
    char p_ch;
    char p_flags;
    THING *p_monst;
} PLACE;

/*
 * Array containing information on all the various types of monsters
 */
struct monster {
    /** What to call the monster. */
    const char* m_name;
    int m_carry;			/* Probability of carrying something */
    short m_flags;			/* things about the monster */
    struct stats m_stats;		/* Initial stats */
};

struct delayed_action
{
    using callback_type = void(*)(int);

    int d_type;
    callback_type d_func;
    int d_arg;
    int d_time;
};

/*
 * External variables
 */

extern bool	after, again, allscore, amulet, door_stop, fight_flush,
		firstmove, has_hit, inv_describe, jump, kamikaze,
		lower_msg, move_on, msg_esc,
		passgo, playing, q_comm, running, save_msg, see_floor,
		seenstairs, stat_msg, terse, to_death, tombstone;

/** Is the character used in the pack? */
extern std::array<bool, 26> pack_used;

extern char	dir_ch, file_name[], home[], huh[],
		l_last_comm, l_last_dir, last_comm, last_dir, *Numname,
		outbuf[], runch, take;

extern std::string release;
extern std::array<const char*, 3> inv_t_name;
/** Names of the traps. */
extern std::array<const char*, NTRAPS> tr_name;
/** Colors of the potions. */
extern std::array<std::optional<std::string>, MAXPOTIONS> p_colors;
/** Stone settings of the rings. */
extern std::array<std::optional<std::string>, MAXRINGS> r_stones;
/** Names of the scrolls. */
extern std::array<std::string, MAXSCROLLS> s_names;
/** What sticks are made of. */
extern std::array<std::optional<std::string>, MAXSTICKS> ws_made;
/** Is it a wand or a staff? */
extern std::array<std::optional<std::string>, MAXSTICKS> ws_type;

extern int	a_class[], count, food_left, hungry_state, inpack,
		inv_type, lastscore, level, max_hit, max_level, mpos,
		n_objs, no_command, no_food, no_move, noscore, ntraps, purse,
		quiet, vf_hit;

extern unsigned int	numscores;

extern int	dnum, e_levels[], seed;

extern WINDOW	*hw;

extern coord	delta, oldpos, stairs;

extern PLACE	places[];

extern THING	*cur_armor, *cur_ring[], *cur_weapon, *l_last_pick,
		*last_pick, *lvl_obj, *mlist, player;

extern struct h_list	helpstr[];

extern struct room	*oldrp, passages[], rooms[];

extern struct stats	max_stats;

extern struct monster	monsters[];

extern struct obj_info	arm_info[], pot_info[], ring_info[],
			scr_info[], things[], ws_info[], weap_info[];

/*
 * Function types
 */
void	_attach(THING **list, THING *item);
void	_detach(THING **list, THING *item);
void	_free_list(THING **ptr);
void addmsg(const char* fmt, ...);
bool	add_haste(bool potion);
void	add_pack(THING *obj, bool silent);
void	add_pass();
void	add_str(str_t *sp, int amt);
void	accnt_maze(int y, int x, int ny, int nx);
void	aggravate();
int	attack(THING *mp);
void	badcheck(char *name, struct obj_info *info, int bound);
void	call();
void	call_it(struct obj_info *info);
bool	cansee(int y, int x);
void	chg_str(int amt);
void	check_level();
void	conn(int r1, int r2);
void	command();
void	create_obj();

void current(THING* cur, const char* how, const char* where);
void	d_level();
void	death(char monst);
char	death_monst();
void	dig(int y, int x);
void	discard(THING *item);
void	discovered();
int	dist(int y1, int x1, int y2, int x2);
int	dist_cp(coord *c1, coord *c2);
int	do_chase(THING *th);
void	do_daemons(int flag);
void	do_fuses(int flag);
void	do_motion(THING *obj, int ydelta, int xdelta);
void	do_move(int dy, int dx);
void	do_passages();
void	do_pot(int type, bool knowit);
void	do_rooms();
void	do_run(char ch);
void	do_zap();
void	door(struct room *rm, coord *cp);
void	door_open(struct room *rp);
void	drain();
void	drop();
void	eat();
size_t  encread(char *start, size_t size, FILE *inf);
std::size_t encwrite(const char* start, std::size_t size, FILE* outf);
int	endmsg();
void	enter_room(coord *cp);
void erase_lamp(const coord& pos, const room& rp);
int	exp_add(THING *tp);
void extinguish(const delayed_action::callback_type& func);
void	fall(THING *obj, bool pr);
void fire_bolt(const coord* start, coord* dir, const char* name);
char	floor_at();
void	flush_type();
int	fight(coord *mp, THING *weap, bool thrown);
void fix_stick(THING& cur);
void fuse(const delayed_action::callback_type& func, int arg, int time, int type);
bool	get_dir();
int	gethand();
void	give_pack(THING *tp);
void	help();
void	leave_room(coord *cp);
void lengthen(const delayed_action::callback_type& func, int xtime);
void	look(bool wakeup);
int	hit_monster(int y, int x, THING *obj);
void	identify();
void	illcom(int ch);
void	init_check();
void	init_colors();
void	init_materials();
void	init_names();
void	init_player();
void	init_probs();
void	init_stones();
void	init_weapon(THING *weap, int which);
bool	inventory(THING *list, int type);
void	invis_on();
void	killed(THING *tp, bool pr);
void kill_daemon(const delayed_action::callback_type& func);
bool	lock_sc();
void	missile(int ydelta, int xdelta);
void	money(int value);
int	move_monst(THING *tp);
void	move_msg(THING *obj);
int msg(const char* fmt, ...);
void	new_level();
void	new_monster(THING *tp, char type, coord *cp);
void	numpass(int y, int x);
void	option();
void	open_score();
void	parse_opts(char *str);
void 	passnum();
int	pick_one(struct obj_info *info, int nitems);
void	pick_up(char ch);
void	picky_inven();
void	pr_spec(struct obj_info *info, int nitems);
void	pr_list();
void	put_bool(void *b);
void	put_inv_t(void *ip);
void	put_str(void *str);
void	put_things();
void	putpass(coord *cp);
void	quaff();
void	raise_level();
char	randmonster(bool wander);
void	read_scroll();
void    relocate(THING *th, coord *new_loc);
void	remove_mon(coord *mp, THING *tp, bool waskill);
void	reset_last();
bool restore(const char* file, char** envp);
int	ring_eat(int hand);
void	ring_on();
void	ring_off();
int	rnd(int range);
int	rnd_room();
int	roll(int number, int sides);
bool rs_save_file(FILE* savef);
bool rs_restore_file(FILE* inf);
void	runto(coord *runner);
void	rust_armor(THING *arm);
int	save(int which);
void	save_file(FILE *savef);
void	save_game();
int	save_throw(int which, THING *tp);
void	score(int amount, int flags, char monst);
void	search();
void	set_know(THING *obj, struct obj_info *info);
void	set_oldch(THING *tp, coord *cp);
void	setup();
void	shell();
bool	show_floor();
void	show_map();
void show_win(const std::string& message);
int	sign(int nm);
int	spread(int nm);
void start_daemon(const delayed_action::callback_type& func, int arg, int type);
void	start_score();
void	status();
int	step_ok(int ch);
void strucpy(char* s1, const char* s2, std::size_t len);
int	swing(int at_lvl, int op_arm, int wplus);
void	take_off();
void	teleport();
void	total_winner();
void	treas_room();
void	turnref();
void	u_level();
void	uncurse(THING *obj);
void	unlock_sc();
void	wait_for(int ch);
THING  *wake_monster(int y, int x);
void	wanderer();
void	waste_time();
void	wear();
void	whatis(bool insist, int type);
void	wield();

bool	chase(THING *tp, coord *ee);
bool	diag_ok(coord *sp, coord *ep);
bool	dropcheck(THING *obj);
bool	fallpos(coord *pos, coord *newpos);
bool	find_floor(struct room *rp, coord *cp, int limit, bool monst);
bool	is_magic(THING *obj);
bool is_symlink(const std::string& filename);
bool	levit_check();
bool	pack_room(bool from_floor, THING *obj);
bool	roll_em(THING *thatt, THING *thdef, THING *weap, bool hurl);
bool	see_monst(THING *mp);
bool	seen_stairs();
bool	turn_ok(int y, int x);
bool	turn_see(bool turn_off);
bool	is_current(THING *obj);
int	passwd();

char	be_trapped(coord *tc);
char	floor_ch();
char	readchar();
char	rnd_thing();

std::string charge_str(const THING& obj);

/*
 * choose_str:
 *	Choose the first or second string depending on whether it the
 *	player is tripping
 */
inline const char*
choose_str(const char* ts, const char* ns)
{
    return on(player, ISHALU) ? ts : ns;
}

static constexpr std::size_t NCOLORS = 27;
extern const std::array<std::string, NCOLORS> rainbow;

inline const std::string&
random_color()
{
    return rainbow[rnd(NCOLORS)];
}

/*
 * pick_color:
 *	If he is halucinating, pick a random color name and return it,
 *	otherwise return the given color.
 */
inline const std::string&
pick_color(const std::string& col)
{
    return on(player, ISHALU) ? random_color() : col;
}

char	*inv_name(THING *obj, bool drop);
char	*num(int n1, int n2, char type);
std::string ring_num(const THING& obj);
const char* set_mname(THING* tp);
const char* vowelstr(const std::string& str);

int	get_bool(void *vp, WINDOW *win);
int	get_inv_t(void *vp, WINDOW *win);
int	get_num(void *vp, WINDOW *win);
int	get_sf(void *vp, WINDOW *win);
int	get_str(void *vopt, WINDOW *win);
int	trip_ch(int y, int x, int ch);

coord	*find_dest(THING *tp);
coord	*rndmove(THING *who);

THING	*find_obj(int y, int x);
THING* get_item(const char* purpose, int type);
THING	*leave_pack(THING *obj, bool newobj, bool all);
THING	*new_item();
THING	*new_thing();

struct room	*roomin(coord *cp);

static constexpr std::size_t MAXDAEMONS = 20;

extern delayed_action d_list[MAXDAEMONS];

struct STONE
{
    std::string st_name;
    int st_value;
};

extern int      total;
extern int      between;
extern int      group;
extern coord    nh;
static constexpr std::size_t NSTONES = 26;
extern const std::array<STONE, NSTONES> stones;
static constexpr std::size_t NWOOD = 33;
extern const std::array<std::string, NWOOD> wood;
static constexpr std::size_t NMETAL = 22;
extern const std::array<std::string, NMETAL> metal;
