/* Copyright (C) 2012, Kevin Polulak <kpolulak@gmail.com>
 *
 * Egghead is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * Egghead is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _DEBUGGER_H_
#define _DEBUGGER_H_

/* Size of command-line buffer. */
#define EGGHEAD_CMD_BUF_LEN 128

/* Size of buffer allocated for source code. */
#define EGGHEAD_SRC_BUF_LEN 1024

/* Convenience macros for manipulating the state bitmask. */
#define EGGHEAD_FLAG_SET(interp, flag) \
    ((interp)->ei_state = (eh_state_t) ((interp)->ei_state | (flag)))

#define EGGHEAD_FLAG_CLEAR(interp, flag) \
    ((interp)->ei_state = (eh_state_t) ((interp)->ei_state & ~(flag)))

#define EGGHEAD_FLAG_TEST(interp, flag) \
    ((interp)->ei_state & (flag))

/* Flags used to alter and identify the current state of the debugger. */
typedef enum {
    EGGHEAD_SRC_LOADED   = 0x0001, /* Source code for debugge is loaded. */
    EGGHEAD_RUNNING      = 0x0002, /* Debugger is running. */
    EGGHEAD_STOPPED      = 0x0004, /* Debugger is stopped/paused. */
    EGGHEAD_BREAK        = 0x0008, /* Halt at next breakpoint. */
    EGGHEAD_STEP         = 0x0010, /* Step execution one instruction. */
    EGGHEAD_EXIT         = 0x0020, /* Debugger is about to exit. */
    EGGHEAD_STARTED      = 0x0040, /* Debugger has been started. */
    EGGHEAD_CMD_ENTERED  = 0x0080  /* At least one command was entered. */
} eh_state_t;

/* Details about brainfuck file being debugged. */
typedef struct eh_file {
    char       *ef_path; /* Name of source file. */
    char       *ef_src;  /* Source code text. */
    size_t      ef_size; /* Size of file in bytes. */
} eh_file_t;

/* The debugger's core type. */
typedef struct eh_interp {
    eh_state_t ei_state;    /* Status of debugger. */
    eh_file_t *ei_file;     /* File being debugged. */
    char      *ei_cur_cmd;  /* Current command. */
    char      *ei_prev_cmd; /* Previous command. */
} eh_interp_t;

void egghead_dbg_init(const char * const);
void egghead_dbg_cmd_file(eh_interp_t *, const char *);
void egghead_dbg_cmd_help(eh_interp_t *, const char *);
void egghead_dbg_cmd_list(eh_interp_t *, const char *);
void egghead_dbg_cmd_quit(eh_interp_t *, const char *);

typedef void (*eh_cmd_func_t)(eh_interp_t *, const char *);

/* Represents a particular command. */
typedef struct eh_cmd {
    const eh_cmd_func_t ec_func;       /* Command implementation. */
    const char         *ec_short_help; /* Short help message. */
    const char         *ec_help;       /* Help message. */
} eh_cmd_t;

/* Command table used to describe all recognized commands. */
typedef struct eh_cmd_tbl {
    const char     *ect_name;       /* Command name. */
    const char     *ect_short_name; /* Command abbreviation. */
    const eh_cmd_t *ect_cmd;        /* Implementation and help messages. */
} eh_cmd_tbl_t;

#endif /* _DEBUGGER_H_ */

