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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
    char *filename = argv[1];

    /* Begin compilation if file was given. Otherwise, exit program. */
    if (argc < 2) {
        fprintf(stderr, "[ERROR] No input file given.\n");
        exit(EXIT_FAILURE);
    }
    else {
        /* Proceed only if a valid .bf or .b file was given. */
        if ((strcmp(strrchr(filename, '.'), ".bf") == 0)
            || strcmp(strrchr(filename, '.'), ".b") == 0) {

            egghead_eval_file(filename);
        }
        else {
            fprintf(stderr,
                    "[ERROR] File %s is not a valid brainfuck file.\n",
                    filename);

            exit(EXIT_FAILURE);
        }
    }

    return 0;
}

