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
#include <stdlib.h>
#include <stdint.h>

void
egghead_eval_char(int8_t *chars)
{
}

void
egghead_eval_file(char *file)
{
    int          ch;
    unsigned int fsize;
    int8_t      *input;
    FILE        *fp;

    unsigned int i = 0;

    fp = fopen(file, "r");

    if (fp == NULL) {
        fprintf(stderr, "[ERROR] File %s could not be read.\n", file);
        exit(EXIT_FAILURE);
    }

    /* Determine file size. */
    fseek(fp, 0, SEEK_END);
    fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    /* Fail if file is empty. */
    if (fsize == 0) {
        fprintf(stderr, "[ERROR] File %s is empty.\n", file);
        exit(EXIT_FAILURE);
    }

    input = (int8_t *) malloc(fsize);

    /* Fail if not enough memory is available. */
    if (input == NULL) {
        fprintf(stderr, "[ERROR] Insufficient memory.\n");
        exit(EXIT_FAILURE);
    }

    /* Fill buffer with contents of file. */
    while ((ch = fgetc(fp)) != EOF)
        input[i++] = (int8_t) ch;

    fclose(fp);

    /* Begin interpretation of file contents. */
    egghead_eval_char(input);

    free(input);
}

