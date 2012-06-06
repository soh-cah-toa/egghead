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

#ifndef _EGGHEAD_H_
#define _EGGHEAD_H_

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct eh_opts {
    bool         eo_strict;
    unsigned int eo_cells;
} eh_opts_t;

#define STREQ(s1, s2) ((strcmp((s1), (s2)) == 0))

#define XCALLOC(type, num) ((type *) xcalloc((num),  sizeof (type)))
#define XMALLOC(type, num) ((type *) xmalloc((num)))
#define XFREE(stale) \
    do { \
        if ((stale)) { free((stale)); stale = 0; } \
    } while (0)

void egghead_eval_char(eh_opts_t *, const char * const);
void egghead_eval_file(eh_opts_t *, const char * const);

void *xcalloc(size_t, size_t);
void *xmalloc(size_t);

#endif /* _EGGHEAD_H_ */

