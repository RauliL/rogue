#pragma once

#include <cstddef>

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

// Now we define the structures and types.

/**
 * Help list.
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

/**
 * Stuff about objects.
 */
struct obj_info
{
    const char* oi_name;
    int oi_prob;
    int oi_worth;
    char* oi_guess;
    bool oi_know;
};

/**
 * Room structure.
 */
struct room
{
    static constexpr std::size_t max_exits = 12;

    /** Upper left corner. */
    coord r_pos;
    /** Size of room. */
    coord r_max;
    /** Where the gold is. */
    coord r_gold;
    /** How much the gold is worth. */
    int r_goldval;
    /** Info about the room. */
    short r_flags;
    /** Number of exits. */
    int r_nexits;
    /** Where the exits are. */
    coord r_exit[max_exits];
};

/**
 * Structure describing a fighting being.
 */
struct stats
{
    using str_t = unsigned int;

    /** Strength. */
    str_t s_str;
    /** Experience. */
    int s_exp;
    /** Level of mastery. */
    int s_lvl;
    /** Armor class. */
    int s_arm;
    /** Hit points. */
    int s_hpt;
    /** String describing damage done. */
    char s_dmg[13];
    /** Max hit points. */
    int  s_maxhp;
};

/**
 * Structure for monsters and player.
 */
union thing
{
    struct
    {
        /** Next pointer in link. */
        thing* _l_next;
        thing* _l_prev;
        /** Position. */
        coord _t_pos;
        /** If slowed, is it a turn to move. */
        bool _t_turn;
        /** What it is. */
        char _t_type;
        /** What mimic looks like. */
        char _t_disguise;
        /** Character that was where it was. */
        char _t_oldch;
        /** Where it is running to. */
        coord* _t_dest;
        /** State word. */
        short _t_flags;
        /** Physical description. */
        stats _t_stats;
        /** Current room for thing. */
        room* _t_room;
        /** What the thing is carrying. */
        thing* _t_pack;
        int _t_reserved;
    } _t;
    struct
    {
        /** Next pointer in link. */
        thing* _l_next;
        thing* _l_prev;
        /** What kind of object it is. */
        int _o_type;
        /** Where it lives on the screen. */
        coord _o_pos;
        /** What it sayf if you read it. */
        char* _o_text;
        /** What you need to launch it. */
        int _o_launch;
        /** What character it is in the pack. */
        char _o_packch;
        /** Damage if used like sword. */
        char _o_damage[8];
        /** Damage if thrown. */
        char _o_hurldmg[8];
        /** Count for plural objects. */
        int _o_count;
        /** Which object of a type it is. */
        int _o_which;
        /** Plusses to hit. */
        int _o_hplus;
        /** Plusses to damage. */
        int _o_dplus;
        /** Armor protection. */
        int _o_arm;
        /** Information about objects. */
        int _o_flags;
        /** Group number for this object. */
        int _o_group;
        /** Label for object. */
        char* _o_label;
    } _o;
};

typedef union thing THING;

/**
 * Describe a place on the level map.
 */
struct PLACE
{
    char p_ch;
    char p_flags;
    thing* p_monst;
};

/**
 * Array containing information on all the various types of monsters.
 */
struct monster
{
    /** What to call the monster. */
    const char* m_name;
    /** Probability of carrying something. */
    int m_carry;
    /** Things about the monster. */
    short m_flags;
    /** Initial stats. */
    stats m_stats;
};

struct delayed_action
{
    using callback_type = void(*)(int);

    int d_type;
    callback_type d_func;
    int d_arg;
    int d_time;
};
