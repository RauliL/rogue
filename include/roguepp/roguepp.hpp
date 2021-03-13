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

#include <roguepp/extern.hpp>
#include <roguepp/limits.hpp>
#include <roguepp/types.hpp>

#undef lines

#define NOOP(x) (x += 0)
#define CCHAR(x) ( (char) (x & A_CHARTEXT) )

#define AMULETLEVEL	26
#define	NUMTHINGS	7	/* number of types of things */
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
    POTION = '!',
    SCROLL = '?',
    FOOD = ':',
    WEAPON = ')',
    ARMOR = ']',
    RING = '=',
    STICK = '/',
    GOLD = '*',
    AMULET = ',',
};

#define PASSAGE		'#'
#define DOOR		'+'
#define FLOOR		'.'
#define PLAYER		'@'
#define TRAP		'^'
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
		l_last_comm, l_last_dir, last_comm, last_dir,
		outbuf[], runch, take;

extern const char* Numname;

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
void	add_str(stats::str_t* sp, int amt);
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
extern const std::array<STONE, NSTONES> stones;
extern const std::array<std::string, NWOOD> wood;
extern const std::array<std::string, NMETAL> metal;
