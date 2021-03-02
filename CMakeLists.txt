CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT(rogue C)

INCLUDE(CheckIncludeFiles)
INCLUDE(CheckSymbolExists)
INCLUDE(FindCurses)

CHECK_SYMBOL_EXISTS(alarm "unistd.h" HAVE_ALARM)
CHECK_INCLUDE_FILES("arpa/inet.h" HAVE_ARPA_INET_H)
CHECK_INCLUDE_FILES("curses.h" HAVE_CURSES_H)
CHECK_SYMBOL_EXISTS(erasechar "curses.h" HAVE_ERASECHAR)
CHECK_SYMBOL_EXISTS(ESCDELAY "curses.h" HAVE_ESCDELAY)
CHECK_SYMBOL_EXISTS(getgid "unistd.h" HAVE_GETGID)
CHECK_SYMBOL_EXISTS(getloadavg "stdlib.h" HAVE_GETLOADAVG)
CHECK_SYMBOL_EXISTS(getpass "unistd.h" HAVE_GETPASS)
CHECK_SYMBOL_EXISTS(getpwuid "pwd.h" HAVE_GETPWUID)
CHECK_SYMBOL_EXISTS(getuid "unistd.h" HAVE_GETUID)
CHECK_SYMBOL_EXISTS(killchar "curses.h" HAVE_KILLCHAR)
CHECK_INCLUDE_FILES("ncurses.h" HAVE_NCURSES_H)
CHECK_INCLUDE_FILES("ncurses/term.h" HAVE_NCURSES_TERM_H)
CHECK_SYMBOL_EXISTS(nlist "nlist.h" HAVE_NLIST)
CHECK_INCLUDE_FILES("nlist.h" HAVE_NLIST_H)
CHECK_INCLUDE_FILES("process.h" HAVE_PROCESS_H)
CHECK_INCLUDE_FILES("pwd.h" HAVE_PWD_H)
CHECK_SYMBOL_EXISTS(setgid "unistd.h" HAVE_SETGID)
CHECK_SYMBOL_EXISTS(setregid "unistd.h" HAVE_SETREGID)
CHECK_SYMBOL_EXISTS(setresgid "unistd.h" HAVE_SETRESGID)
CHECK_SYMBOL_EXISTS(setresuid "unistd.h" HAVE_SETRESUID)
CHECK_SYMBOL_EXISTS(setreuid "unistd.h" HAVE_SETREUID)
CHECK_SYMBOL_EXISTS(setuid "unistd.h" HAVE_SETUID)
CHECK_SYMBOL_EXISTS(spawnl "process.h" HAVE_SPAWNL)
CHECK_INCLUDE_FILES("sys/types.h" HAVE_SYS_TYPES_H)
CHECK_INCLUDE_FILES("sys/utsname.h" HAVE_SYS_UTSNAME_H)
CHECK_INCLUDE_FILES("termios.h" HAVE_TERMIOS_H)
CHECK_INCLUDE_FILES("term.h" HAVE_TERM_H)
CHECK_INCLUDE_FILES("unistd.h" HAVE_UNISTD_H)
CHECK_SYMBOL_EXISTS(_spawnl "process.h" HAVE__SPAWNL)
CHECK_INCLUDE_FILES("stdlib.h;stdarg.h;string.h;float.h" STDC_HEADERS)
CHECK_SYMBOL_EXISTS(tm "sys/time.h" TM_IN_SYS_TIME)

CONFIGURE_FILE(
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_SOURCE_DIR}/config.h"
)

# TODO: Fix compilation with "config.h". Currently produces ncurses related
# errors.
#ADD_DEFINITIONS(-DHAVE_CONFIG_H)

ADD_EXECUTABLE(
  rogue
  armor.c
  chase.c
  command.c
  daemon.c
  daemons.c
  extern.c
  fight.c
  init.c
  io.c
  list.c
  mach_dep.c
  main.c
  mdport.c
  misc.c
  monsters.c
  move.c
  new_level.c
  options.c
  pack.c
  passages.c
  potions.c
  rings.c
  rip.c
  rooms.c
  save.c
  scrolls.c
  state.c
  sticks.c
  things.c
  vers.c
  weapons.c
  wizard.c
  xcrypt.c
)

TARGET_INCLUDE_DIRECTORIES(
  rogue
  PRIVATE
    ${CURSES_INCLUDE_DIR}
)

TARGET_LINK_LIBRARIES(
  rogue
  ${CURSES_LIBRARIES}
)

INSTALL(
  TARGETS
    rogue
  RUNTIME DESTINATION
    bin
)

INSTALL(
  FILES
    rogue.desktop
  DESTINATION
    share/applications
)

INSTALL(
  FILES
    rogue.png
  DESTINATION
    share/icons/hicolor/32x32/apps
)

# TODO: Generate and install man pages.
