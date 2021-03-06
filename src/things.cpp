/*
 * Contains functions for dealing with things like potions, scrolls,
 * and other items.
 *
 * @(#)things.c	4.53 (Berkeley) 02/05/99
 *
 * Rogue: Exploring the Dungeons of Doom
 * Copyright (C) 1980-1983, 1985, 1999 Michael Toy, Ken Arnold and Glenn Wichman
 * All rights reserved.
 *
 * See the file LICENSE.TXT for full copyright and licensing information.
 */

#include <cctype>
#include <cstdio>
#include <cstring>
#include <ncurses.h>
#include "rogue.hpp"

static void set_order(int*, int);
static const char* nothing(char);
static const char* nullstr(const THING*);
static void nameit(
    THING*,
    const char*,
    const char*,
    obj_info*,
    const char*(*)(const THING*)
);

/*
 * inv_name:
 *	Return the name of something as it would appear in an
 *	inventory.
 */
char *
inv_name(THING *obj, bool drop)
{
    char *pb;
    struct obj_info *op;
    const char* sp;
    int which;

    pb = prbuf;
    which = obj->o_which;
    switch (obj->o_type)
    {
        case POTION:
	    nameit(obj, "potion", p_colors[which], &pot_info[which], nullstr);
	when RING:
	    nameit(obj, "ring", r_stones[which], &ring_info[which], ring_num);
	when STICK:
	    nameit(obj, ws_type[which], ws_made[which], &ws_info[which], charge_str);
	when SCROLL:
	    if (obj->o_count == 1)
	    {
		strcpy(pb, "A scroll ");
		pb = &prbuf[9];
	    }
	    else
	    {
		sprintf(pb, "%d scrolls ", obj->o_count);
		pb = &prbuf[strlen(prbuf)];
	    }
	    op = &scr_info[which];
	    if (op->oi_know)
		sprintf(pb, "of %s", op->oi_name);
	    else if (op->oi_guess)
		sprintf(pb, "called %s", op->oi_guess);
	    else
		sprintf(pb, "titled '%s'", s_names[which]);
	when FOOD:
	    if (which == 1)
		if (obj->o_count == 1)
		    sprintf(pb, "A%s %s", vowelstr(fruit), fruit);
		else
		    sprintf(pb, "%d %ss", obj->o_count, fruit);
	    else
		if (obj->o_count == 1)
		    strcpy(pb, "Some food");
		else
		    sprintf(pb, "%d rations of food", obj->o_count);
	when WEAPON:
	    sp = weap_info[which].oi_name;
	    if (obj->o_count > 1)
		sprintf(pb, "%d ", obj->o_count);
	    else
		sprintf(pb, "A%s ", vowelstr(sp));
	    pb = &prbuf[strlen(prbuf)];
	    if (obj->o_flags & ISKNOW)
		sprintf(pb, "%s %s", num(obj->o_hplus,obj->o_dplus,WEAPON), sp);
	    else
		sprintf(pb, "%s", sp);
	    if (obj->o_count > 1)
		strcat(pb, "s");
	    if (obj->o_label != nullptr)
	    {
		pb = &prbuf[strlen(prbuf)];
		sprintf(pb, " called %s", obj->o_label);
	    }
	when ARMOR:
	    sp = arm_info[which].oi_name;
	    if (obj->o_flags & ISKNOW)
	    {
		sprintf(pb, "%s %s [",
		    num(a_class[which] - obj->o_arm, 0, ARMOR), sp);
		if (!terse)
		    strcat(pb, "protection ");
		pb = &prbuf[strlen(prbuf)];
		sprintf(pb, "%d]", 10 - obj->o_arm);
	    }
	    else
		sprintf(pb, "%s", sp);
	    if (obj->o_label != nullptr)
	    {
		pb = &prbuf[strlen(prbuf)];
		sprintf(pb, " called %s", obj->o_label);
	    }
	when AMULET:
	    strcpy(pb, "The Amulet of Yendor");
	when GOLD:
	    sprintf(prbuf, "%d Gold pieces", obj->o_goldval);
#ifdef MASTER
	otherwise:
	    debug("Picked up something funny %s", unctrl(obj->o_type));
	    sprintf(pb, "Something bizarre %s", unctrl(obj->o_type));
#endif
    }
    if (inv_describe)
    {
	if (obj == cur_armor)
	    strcat(pb, " (being worn)");
	if (obj == cur_weapon)
	    strcat(pb, " (weapon in hand)");
	if (obj == cur_ring[LEFT])
	    strcat(pb, " (on left hand)");
	else if (obj == cur_ring[RIGHT])
	    strcat(pb, " (on right hand)");
    }
    if (drop && isupper(prbuf[0]))
	prbuf[0] = (char) tolower(prbuf[0]);
    else if (!drop && islower(*prbuf))
	*prbuf = (char) toupper(*prbuf);
    prbuf[MAXSTR-1] = '\0';
    return prbuf;
}

/*
 * drop:
 *	Put something down
 */

void
drop()
{
    char ch;
    THING *obj;

    ch = chat(hero.y, hero.x);
    if (ch != FLOOR && ch != PASSAGE)
    {
	after = false;
	msg("there is something there already");
	return;
    }
    if ((obj = get_item("drop", 0)) == nullptr)
	return;
    if (!dropcheck(obj))
	return;
    obj = leave_pack(obj, true, (bool)!ISMULT(obj->o_type));
    /*
     * Link it into the level object list
     */
    attach(lvl_obj, obj);
    chat(hero.y, hero.x) = (char) obj->o_type;
    flat(hero.y, hero.x) |= F_DROPPED;
    obj->o_pos = hero;
    if (obj->o_type == AMULET)
	amulet = false;
    msg("dropped %s", inv_name(obj, true));
}

/*
 * dropcheck:
 *	Do special checks for dropping or unweilding|unwearing|unringing
 */
bool
dropcheck(THING *obj)
{
    if (obj == nullptr)
	return true;
    if (obj != cur_armor && obj != cur_weapon
	&& obj != cur_ring[LEFT] && obj != cur_ring[RIGHT])
	    return true;
    if (obj->o_flags & ISCURSED)
    {
	msg("you can't.  It appears to be cursed");
	return false;
    }
    if (obj == cur_weapon)
	cur_weapon = nullptr;
    else if (obj == cur_armor)
    {
	waste_time();
	cur_armor = nullptr;
    }
    else
    {
	cur_ring[obj == cur_ring[LEFT] ? LEFT : RIGHT] = nullptr;
	switch (obj->o_which)
	{
	    case R_ADDSTR:
		chg_str(-obj->o_arm);
		break;
	    case R_SEEINVIS:
		unsee(0);
		extinguish(unsee);
		break;
	}
    }
    return true;
}

/*
 * new_thing:
 *	Return a new thing
 */
THING *
new_thing()
{
    THING *cur;
    int r;

    cur = new_item();
    cur->o_hplus = 0;
    cur->o_dplus = 0;
    strncpy(cur->o_damage, "0x0", sizeof(cur->o_damage));
    strncpy(cur->o_hurldmg, "0x0", sizeof(cur->o_hurldmg));
    cur->o_arm = 11;
    cur->o_count = 1;
    cur->o_group = 0;
    cur->o_flags = 0;
    /*
     * Decide what kind of object it will be
     * If we haven't had food for a while, let it be food.
     */
    switch (no_food > 3 ? 2 : pick_one(things, NUMTHINGS))
    {
	case 0:
	    cur->o_type = POTION;
	    cur->o_which = pick_one(pot_info, MAXPOTIONS);
	when 1:
	    cur->o_type = SCROLL;
	    cur->o_which = pick_one(scr_info, MAXSCROLLS);
	when 2:
	    cur->o_type = FOOD;
	    no_food = 0;
	    if (rnd(10) != 0)
		cur->o_which = 0;
	    else
		cur->o_which = 1;
	when 3:
	    init_weapon(cur, pick_one(weap_info, MAXWEAPONS));
	    if ((r = rnd(100)) < 10)
	    {
		cur->o_flags |= ISCURSED;
		cur->o_hplus -= rnd(3) + 1;
	    }
	    else if (r < 15)
		cur->o_hplus += rnd(3) + 1;
	when 4:
	    cur->o_type = ARMOR;
	    cur->o_which = pick_one(arm_info, MAXARMORS);
	    cur->o_arm = a_class[cur->o_which];
	    if ((r = rnd(100)) < 20)
	    {
		cur->o_flags |= ISCURSED;
		cur->o_arm += rnd(3) + 1;
	    }
	    else if (r < 28)
		cur->o_arm -= rnd(3) + 1;
	when 5:
	    cur->o_type = RING;
	    cur->o_which = pick_one(ring_info, MAXRINGS);
	    switch (cur->o_which)
	    {
		case R_ADDSTR:
		case R_PROTECT:
		case R_ADDHIT:
		case R_ADDDAM:
		    if ((cur->o_arm = rnd(3)) == 0)
		    {
			cur->o_arm = -1;
			cur->o_flags |= ISCURSED;
		    }
		when R_AGGR:
		case R_TELEPORT:
		    cur->o_flags |= ISCURSED;
	    }
	when 6:
	    cur->o_type = STICK;
	    cur->o_which = pick_one(ws_info, MAXSTICKS);
	    fix_stick(cur);
#ifdef MASTER
	otherwise:
	    debug("Picked a bad kind of object");
	    wait_for(' ');
#endif
    }
    return cur;
}

/*
 * pick_one:
 *	Pick an item out of a list of nitems possible objects
 */
int
pick_one(struct obj_info *info, int nitems)
{
    struct obj_info *end;
    struct obj_info *start;
    int i;

    start = info;
    for (end = &info[nitems], i = rnd(100); info < end; info++)
	if (i < info->oi_prob)
	    break;
    if (info == end)
    {
#ifdef MASTER
	if (wizard)
	{
	    msg("bad pick_one: %d from %d items", i, nitems);
	    for (info = start; info < end; info++)
		msg("%s: %d%%", info->oi_name, info->oi_prob);
	}
#endif
	info = start;
    }
    return (int)(info - start);
}

/*
 * discovered:
 *	list what the player has discovered in this game of a certain type
 */
static int line_cnt = 0;

static bool newpage = false;

static const char* lastfmt;
static const char* lastarg;


void
discovered()
{
    char ch;
    bool disc_list;

    do {
	disc_list = false;
	if (!terse)
	    addmsg("for ");
	addmsg("what type");
	if (!terse)
	    addmsg(" of object do you want a list");
	msg("? (* for all)");
	ch = readchar();
	switch (ch)
	{
	    case ESCAPE:
		msg("");
		return;
	    case POTION:
	    case SCROLL:
	    case RING:
	    case STICK:
	    case '*':
		disc_list = true;
		break;
	    default:
		if (terse)
		    msg("Not a type");
		else
		    msg("Please type one of %c%c%c%c (ESCAPE to quit)", POTION, SCROLL, RING, STICK);
	}
    } while (!disc_list);
    if (ch == '*')
    {
	print_disc(POTION);
	add_line("", nullptr);
	print_disc(SCROLL);
	add_line("", nullptr);
	print_disc(RING);
	add_line("", nullptr);
	print_disc(STICK);
	end_line();
    }
    else
    {
	print_disc(ch);
	end_line();
    }
}

/*
 * print_disc:
 *	Print what we've discovered of type 'type'
 */

#define MAX4(a,b,c,d)	(a > b ? (a > c ? (a > d ? a : d) : (c > d ? c : d)) : (b > c ? (b > d ? b : d) : (c > d ? c : d)))


void
print_disc(char type)
{
    struct obj_info *info = nullptr;
    int i, maxnum = 0, num_found;
    static THING obj;
    static int order[MAX4(MAXSCROLLS, MAXPOTIONS, MAXRINGS, MAXSTICKS)];

    switch (type)
    {
	case SCROLL:
	    maxnum = MAXSCROLLS;
	    info = scr_info;
	    break;
	case POTION:
	    maxnum = MAXPOTIONS;
	    info = pot_info;
	    break;
	case RING:
	    maxnum = MAXRINGS;
	    info = ring_info;
	    break;
	case STICK:
	    maxnum = MAXSTICKS;
	    info = ws_info;
	    break;
    }
    set_order(order, maxnum);
    obj.o_count = 1;
    obj.o_flags = 0;
    num_found = 0;
    for (i = 0; i < maxnum; i++)
	if (info[order[i]].oi_know || info[order[i]].oi_guess)
	{
	    obj.o_type = type;
	    obj.o_which = order[i];
	    add_line("%s", inv_name(&obj, false));
	    num_found++;
	}
    if (num_found == 0)
	add_line(nothing(type), nullptr);
}

/*
 * set_order:
 *	Set up order for list
 */
static void
set_order(int* order, int numthings)
{
    for (int i = 0; i < numthings; ++i)
    {
        order[i] = i;
    }

    for (int i = numthings; i > 0; --i)
    {
        const auto r = rnd(i);
        const auto t = order[i - 1];

        order[i - 1] = order[r];
        order[r] = t;
    }
}

/*
 * add_line:
 *	Add a line to the list of discoveries
 */
/* VARARGS1 */
char
add_line(const char* fmt, const char* arg)
{
    WINDOW *tw, *sw;
    int x, y;
    const char* prompt = "--Press space to continue--";
    static int maxlen = -1;

    if (line_cnt == 0)
    {
	    wclear(hw);
	    if (inv_type == INV_SLOW)
		mpos = 0;
    }
    if (inv_type == INV_SLOW)
    {
	if (*fmt != '\0')
	    if (msg(fmt, arg) == ESCAPE)
		return ESCAPE;
	line_cnt++;
    }
    else
    {
	if (maxlen < 0)
	    maxlen = (int) strlen(prompt);
	if (line_cnt >= LINES - 1 || fmt == nullptr)
	{
	    if (inv_type == INV_OVER && fmt == nullptr && !newpage)
	    {
		msg("");
		refresh();
		tw = newwin(line_cnt + 1, maxlen + 2, 0, COLS - maxlen - 3);
		sw = subwin(tw, line_cnt + 1, maxlen + 1, 0, COLS - maxlen - 2);
                for (y = 0; y <= line_cnt; y++)
                {
                    wmove(sw, y, 0);
                    for (x = 0; x <= maxlen; x++)
                        waddch(sw, mvwinch(hw, y, x));
                }
		wmove(tw, line_cnt, 1);
		waddstr(tw, prompt);
		/*
		 * if there are lines below, use 'em
		 */
		if (LINES > NUMLINES)
		{
		    if (NUMLINES + line_cnt > LINES)
			mvwin(tw, LINES - (line_cnt + 1), COLS - maxlen - 3);
		    else
			mvwin(tw, NUMLINES, 0);
		}
		touchwin(tw);
		wrefresh(tw);
		wait_for(' ');
                if (md_hasclreol())
		{
		    werase(tw);
		    leaveok(tw, true);
		    wrefresh(tw);
		}
		delwin(tw);
		touchwin(stdscr);
	    }
	    else
	    {
		wmove(hw, LINES - 1, 0);
		waddstr(hw, prompt);
		wrefresh(hw);
		wait_for(' ');
		clearok(curscr, true);
		wclear(hw);
		touchwin(stdscr);
	    }
	    newpage = true;
	    line_cnt = 0;
	    maxlen = (int) strlen(prompt);
	}
	if (fmt != nullptr && !(line_cnt == 0 && *fmt == '\0'))
	{
	    mvwprintw(hw, line_cnt++, 0, fmt, arg);
	    getyx(hw, y, x);
	    if (maxlen < x)
		maxlen = x;
	    lastfmt = fmt;
	    lastarg = arg;
	}
    }
    return ~ESCAPE;
}

/*
 * end_line:
 *	End the list of lines
 */

void
end_line()
{
    if (inv_type != INV_SLOW)
    {
	if (line_cnt == 1 && !newpage)
	{
	    mpos = 0;
	    msg(lastfmt, lastarg);
	}
	else
	    add_line(nullptr, nullptr);
    }
    line_cnt = 0;
    newpage = false;
}

/*
 * nothing:
 *	Set up prbuf so that message for "nothing found" is there
 */
static const char*
nothing(char type)
{
    const char* tystr = nullptr;

    if (terse)
    {
	    std::sprintf(prbuf, "Nothing");
    } else {
	    std::sprintf(prbuf, "Haven't discovered anything");
    }
    if (type != '*')
    {
	    auto sp = &prbuf[strlen(prbuf)];

        switch (type)
        {
            case POTION: tystr = "potion"; break;
            case SCROLL: tystr = "scroll"; break;
            case RING: tystr = "ring"; break;
            case STICK: tystr = "stick"; break;
        }
        std::sprintf(sp, " about any %ss", tystr);
    }

    return prbuf;
}

/*
 * nameit:
 *	Give the proper name to a potion, stick, or ring
 */
static void
nameit(
    THING* obj,
    const char* type,
    const char* which,
    obj_info *op,
    const char* (*prfunc)(const THING*)
)
{
    char *pb;

    if (op->oi_know || op->oi_guess)
    {
	if (obj->o_count == 1)
	    sprintf(prbuf, "A %s ", type);
	else
	    sprintf(prbuf, "%d %ss ", obj->o_count, type);
	pb = &prbuf[strlen(prbuf)];
	if (op->oi_know)
	    sprintf(pb, "of %s%s(%s)", op->oi_name, (*prfunc)(obj), which);
	else if (op->oi_guess)
	    sprintf(pb, "called %s%s(%s)", op->oi_guess, (*prfunc)(obj), which);
    }
    else if (obj->o_count == 1)
	sprintf(prbuf, "A%s %s %s", vowelstr(which), which, type);
    else
	sprintf(prbuf, "%d %s %ss", obj->o_count, which, type);
}

/*
 * nullstr:
 *	Return a pointer to a null-length string
 */
static const char*
nullstr(const THING*)
{
    return "";
}

# ifdef	MASTER
/*
 * pr_list:
 *	List possible potions, scrolls, etc. for wizard.
 */

void
pr_list()
{
    int ch;

    if (!terse)
	addmsg("for ");
    addmsg("what type");
    if (!terse)
	addmsg(" of object do you want a list");
    msg("? ");
    ch = readchar();
    switch (ch)
    {
	case POTION:
	    pr_spec(pot_info, MAXPOTIONS);
	when SCROLL:
	    pr_spec(scr_info, MAXSCROLLS);
	when RING:
	    pr_spec(ring_info, MAXRINGS);
	when STICK:
	    pr_spec(ws_info, MAXSTICKS);
	when ARMOR:
	    pr_spec(arm_info, MAXARMORS);
	when WEAPON:
	    pr_spec(weap_info, MAXWEAPONS);
	otherwise:
	    return;
    }
}

/*
 * pr_spec:
 *	Print specific list of possible items to choose from
 */

void
pr_spec(struct obj_info *info, int nitems)
{
    struct obj_info *endp;
    int i, lastprob;

    endp = &info[nitems];
    lastprob = 0;
    for (i = '0'; info < endp; i++)
    {
	if (i == '9' + 1)
	    i = 'a';
	sprintf(prbuf, "%c: %%s (%d%%%%)", i, info->oi_prob - lastprob);
	lastprob = info->oi_prob;
	add_line(prbuf, info->oi_name);
	info++;
    }
    end_line();
}
# endif	/* MASTER */
