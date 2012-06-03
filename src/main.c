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
#include <getopt.h>

static void license(void);
static void usage(unsigned int);

int
main(int argc, char *argv[])
{
    int   c;
    char *filename = "";

    opterr = 0;

    /* Process command-line switches. */
    while (1) {
        static struct option long_opts[] = {
            { "help",    no_argument, 0, 'h' },
            { "license", no_argument, 0, 'l' },
            { 0, 0, 0, 0 }
        };

        int opt_index = 0;

        c = getopt_long(argc, argv, "hl", long_opts, &opt_index);

        /* Detect end of options. */
        if (c == -1)
            break;

        switch (c) {
        case 'l':
            license();
            exit(EXIT_SUCCESS);
            break;

        case 'h':
            usage(EXIT_SUCCESS);
            break;

        case '?':
            fprintf(stderr, "%s: unknown option '%c'.\n", argv[0], optopt);
            /* Fall through is intentional. */

        default:
            usage(EXIT_FAILURE);
        }
    }

    /* Begin interpreter if file was given. Otherwise, exit program. */
    if (argc < 2) {
        fprintf(stderr, "[ERROR] No input file given.\n");
        exit(EXIT_FAILURE);
    }
    else {
        /* Get filename as last remaining argument. */
        if (optind < argc) {
            filename = argv[optind];
        }

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

static void
license(void)
{
    const char * const info =
        "Copyright (C) 2012, Kevin Polulak <kpolulak@gmail.com>\n"
        "License GPLv3+: GNU GPL version 3 or later "
        "<http://gnu.org/licenses/gpl.html>\n"
        "This is free software: you are free to change and redistribute it.\n"
        "There is NO WARRANTY, to the extent permitted by law.";

    puts(info);
}

static void
usage(unsigned int status)
{
    const char * const info =
        "Usage:\n"
        "   egghead [options] file\n\n"
        "Options:\n"
        "   --help       Display help message.\n"
        "   --license    Display license information.\n\n"
        "For more information, see the Egghead manpage (man egghead).\n";

    /* Dispaly message to stdout if success. Otherwise, output to stderr. */
    if (status == EXIT_SUCCESS)
        puts(info);
    else
        fprintf(stderr, "%s", info);

    exit(status);
}

