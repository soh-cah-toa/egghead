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

void eh_cmd_help(const char *);
void eh_cmd_quit(const char *);

typedef void (*eh_cmd_func_t)(const char *);

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

/* Details about brainfuck file being debugged. */
typedef struct eh_file {
    const char *ef_file; /* Name of source file. */
    const char *ef_src;  /* Source code text. */
    size_t      ef_size; /* Size of file in bytes. */
} eh_file_t;

#endif /* _DEBUGGER_H_ */

