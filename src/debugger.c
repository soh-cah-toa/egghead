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

#include "egghead.h"
#include "debugger.h"

#include <stdio.h>

eh_cmd_t
    cmd_help = { &eh_cmd_help,
                 "Displays a summary help message.",
                 "Displays a summary help message." },

    cmd_quit = { &eh_cmd_quit,
                 "Exits Egghead debugger.",
                 "Exits Egghead debugger." };

/* Global command table. */
eh_cmd_tbl_t cmd_tbl[] = {
    { "help", "h", &cmd_help },
    { "quit", "q", &cmd_quit }
};

void
eh_cmd_help(const char *cmd)
{
}

void
eh_cmd_quit(const char *cmd)
{
}

