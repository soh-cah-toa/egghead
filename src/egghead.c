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
#include <ctype.h>
#include <stdlib.h>

#define NUM_CELLS 30000

void
egghead_eval_char(eh_opts_t *opts, const char * const input)
{
    char  ch;
    char *cells;

    unsigned int index    = 0;
    unsigned int nest_lvl = 0;
    unsigned int ptr      = 0;

    cells = XCALLOC(char, NUM_CELLS);

    /* Iterate through file contents, interpreting known characters. */
    while (index < (unsigned int) strlen(input)) {
        switch (input[index]) {
        case '>':    /* Increment data pointer. */
            ptr++;
            break;

        case '<':    /* Decrement data pointer. */
            ptr--;
            break;

        case '+':    /* Increment byte at data pointer. */
            cells[ptr]++;
            break;

        case '-':    /* Decrement byte at data pointer. */
            cells[ptr]--;
            break;

        case '.':    /* Output byte at data pointer. */
            putchar(cells[ptr]);
            break;

        case ',':    /* Input byte and store it in cell at data pointer. */
            cells[ptr] = (int) getchar();
            break;

        case '[':    /* Jump past matching ']' if cell at data
                        pointer is 0. */
            if (cells[ptr] == 0) {
                nest_lvl++;

                while (nest_lvl > 0) {
                    ch = input[++index];

                    if (ch == '[')
                        nest_lvl++;
                    else if (ch == ']')
                        nest_lvl--;
                }
            }

            break;

        case ']':    /* Jump back to matching '[' if cell at data
                        pointer isn't 0. */
            nest_lvl++;

            while (nest_lvl > 0) {
                ch = input[--index];

                if (ch == '[')
                    nest_lvl--;
                else if (ch == ']')
                    nest_lvl++;
            }

            index--;
            break;

        default:
            ch = input[index];

            /* Whitespace is always allowed regardless of --strict switch. */
            if (isspace(ch))
                break;

            /* Fail if invalid characters are found with --strict switch on. */
            if (opts->eo_strict == true) {
                fprintf(stderr,
                        "[ERROR] Character %c is not a valid brainfuck "
                        "command.\n",
                        ch);

                exit(EXIT_FAILURE);
            }
        }

        index++;
    }

    XFREE(cells);
}

void
egghead_eval_file(eh_opts_t *opts, const char * const file)
{
    char  ch;
    char *input;
    FILE *fp;

    unsigned int fsize = 0;
    unsigned int index = 0;

    /* Skip file calculations if reading from stdin. */
    if (STREQ(file, "-")) {
        fp    = stdin;
        fsize = 65536;
    }
    else {
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
    }

    input = XMALLOC(char, fsize);

    /* Fill buffer with contents of file. */
    while ((ch = fgetc(fp)) != EOF)
        input[index++] = (char) ch;

    if (fp != stdin)
        fclose(fp);

    egghead_eval_char(opts, input);

    XFREE(input);
}

void *
xcalloc(size_t nmemb, size_t size)
{
    void *new = calloc(nmemb, size);

    /* Fail if not enough memory is available. */
    if ((nmemb == 0) || (size == 0) || new == NULL) {
        fprintf(stderr, "[ERROR] Insufficient memory.\n");
        exit(EXIT_FAILURE);
    }

    return new;
}

void *
xmalloc(size_t size)
{
    void *new = malloc(size);

    /* Fail if not enough memory is available. */
    if ((size == 0) || (new == NULL)) {
        fprintf(stderr, "[ERROR] Insufficient memory.\n");
        exit(EXIT_FAILURE);
    }

    return new;
}

