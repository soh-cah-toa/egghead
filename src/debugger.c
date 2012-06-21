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
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

static void runloop(eh_interp_t *) __attribute__((nonnull));
static void load_src(eh_interp_t *, const char * const) __attribute__((nonnull));
static void free_src(eh_interp_t *);
static void get_cmd(eh_interp_t *) __attribute__((nonnull));
static unsigned int check_file_exists(eh_interp_t *) __attribute__((pure, warn_unused_result));
static unsigned int run_cmd(eh_interp_t *, const char *) __attribute__((nonnull));
static const eh_cmd_t *parse_cmd(const char **) __attribute__((warn_unused_result));
static const char *skip_whitespace(const char *) __attribute__((nonnull, pure, warn_unused_result));

eh_cmd_t
    cmd_file = { &egghead_dbg_cmd_file,
                 "Dynamically load FILE into debugger.",
                 "Dynamically load FILE into debugger." },

    cmd_help = { &egghead_dbg_cmd_help,
                 "Displays a summary help message.",
                 "Displays a summary help message." },

    cmd_list = { &egghead_dbg_cmd_list,
                 "Displays contents of file being debugged.",
                 "Displays contents of file being debugged." },

    cmd_quit = { &egghead_dbg_cmd_quit,
                 "Exits Egghead debugger.",
                 "Exits Egghead debugger." };

/* Global command table. */
eh_cmd_tbl_t cmd_tbl[] = {
    { "file", "f", &cmd_file },
    { "help", "h", &cmd_help },
    { "list", "l", &cmd_list },
    { "quit", "q", &cmd_quit }
};

void
egghead_dbg_init(const char * const file)
{
    eh_interp_t *interp = XMALLOC(eh_interp_t, sizeof (eh_interp_t));

    interp->ei_file = XMALLOC(eh_file_t, sizeof (eh_file_t));

    if (!STREQ(file, ""))
        load_src(interp, file);

    runloop(interp);

    XFREE(interp->ei_file);
    XFREE(interp);
}

void
egghead_dbg_cmd_file(eh_interp_t *interp, const char *cmd)
{
    /* Free previous source file (if any). */
    if (interp->ei_file)
        free_src(interp);

    load_src(interp, cmd);
}

void
egghead_dbg_cmd_help(eh_interp_t *interp, const char *cmd)
{
    UNUSED(interp);

    const eh_cmd_t *c = parse_cmd(&cmd);

    if (c)
        printf("%s\n", c->ec_help);
    else {
        if (*cmd == '\0') {
            const char  *msg;
            unsigned int i;

            puts("List of commands:\n");

            /* Iterate through global command table. */
            for (i = 0; i < (sizeof (cmd_tbl) / sizeof (eh_cmd_tbl_t)); i++) {
                const eh_cmd_tbl_t * const tbl = cmd_tbl + i;

                /* Display name and short message about each command. */
                printf("   %-12s  %s\n",
                       tbl->ect_name,
                       tbl->ect_cmd->ec_short_help);
            }

            msg =
                "\nType 'help' followed by command name for more information.\n"
                "Command name abbreviations are allowed if it's unambiguous.";

            puts(msg);
        }
        else {
            fprintf(stderr, "Undefined command: \"%s\". Try \"help\".\n", cmd);
        }
    }
}

void
egghead_dbg_cmd_list(eh_interp_t *interp, const char *cmd)
{
    UNUSED(cmd);

    if (!check_file_exists(interp))
        fprintf(stderr, "No file has been loaded. Use the 'file' command.\n");
    else {
        printf("%s\n", interp->ei_file->ef_src);
    }
}

void
egghead_dbg_cmd_quit(eh_interp_t *interp, const char *cmd)
{
    UNUSED(cmd);
    EGGHEAD_FLAG_SET(interp, EGGHEAD_EXIT);
}

static void
runloop(eh_interp_t *interp)
{
    do {
        const char *cmd;

        get_cmd(interp);

        cmd = interp->ei_cur_cmd;

        run_cmd(interp, cmd);
    } while (!(EGGHEAD_FLAG_TEST(interp, EGGHEAD_EXIT)));
}

static void
load_src(eh_interp_t *interp, const char * const file)
{
    int    ch;
    size_t buf_size;
    FILE  *fd;

    eh_file_t *dbg_file;

    fd = fopen(file, "r");

    if (!fd) {
        fprintf(stderr, "%s: No such file or directory.\n", file);
        return;
    }

    dbg_file          = XCALLOC(eh_file_t, 1);
    dbg_file->ef_src  = XMALLOC(char, EGGHEAD_SRC_BUF_LEN);
    dbg_file->ef_path = XMALLOC(char, strlen(file) + 1);
    buf_size          = EGGHEAD_SRC_BUF_LEN;

    strcpy(dbg_file->ef_path, file);

    do {
        /* Iterate through characters until newline is reached. */
        do {
            ch = fgetc(fd);

            if (ch == EOF)
                break;

            dbg_file->ef_src[dbg_file->ef_size] = (char) ch;

            /* Extend buffer size if it's full. */
            if (++dbg_file->ef_size >= buf_size) {
                buf_size         += EGGHEAD_SRC_BUF_LEN;
                dbg_file->ef_src  = realloc(dbg_file->ef_src, buf_size);
            }
        } while (ch != '\n');

        /* Stop if the file is empty. */
        if ((ch == EOF) &&
            ((dbg_file->ef_size == 0)
                || (dbg_file->ef_src[dbg_file->ef_size - 1] == '\n'))) {

            break;
        }

        if (ch == EOF)
            dbg_file->ef_src[dbg_file->ef_size++] = '\n';
    } while (ch != EOF);

    fclose(fd);

    interp->ei_file = dbg_file;

    EGGHEAD_FLAG_SET(interp, EGGHEAD_SRC_LOADED);
}

static void
free_src(eh_interp_t *interp)
{
    XFREE(interp->ei_file->ef_src);
    XFREE(interp->ei_file->ef_path);

    interp->ei_file->ef_size = 0;

    XFREE(interp->ei_file);
}

static void
get_cmd(eh_interp_t *interp)
{
    const char * const prompt = "(egghead) ";

    char  input[EGGHEAD_CMD_BUF_LEN];
    char *p;

    fflush(stdout);

    printf(prompt);

    p = fgets(input, EGGHEAD_CMD_BUF_LEN, stdin);

    if (p == NULL) {
        /* TODO: Handle EOF. */
    }
    else {
        /* Chomp newline character. */
        if (input[strlen(input) - 1] == '\n')
            input[strlen(input) - 1] = '\0';

        interp->ei_cur_cmd = input;
    }
}

static unsigned int
check_file_exists(eh_interp_t * interp)
{
    if (interp->ei_file && interp->ei_file->ef_size)
        return 1;
    else
        return 0;
}

static unsigned int
run_cmd(eh_interp_t *interp, const char *cmd)
{
    const eh_cmd_t *c;

    const char *orig_cmd = cmd;

    c = parse_cmd(&orig_cmd);

    if (c) {
        (*c->ec_func)(interp, orig_cmd);
        return 0;
    }
    else {
        if (*orig_cmd == '\0')
            return 0;
        else {
            fprintf(stderr, "Undefined command: \"%s\". Try \"help\".\n", cmd);
            return 1;
        }
    }
}

static const eh_cmd_t *
parse_cmd(const char **cmd)
{
    if (cmd && *cmd) {
        const char  *start;
        const char  *next;
        char         c;
        int          found = -1;
        unsigned int hits  =  0,
                     len,
                     i;

        start = skip_whitespace(*cmd);
        next  = start;

        *cmd  = start;

        /* Get command name. */
        for (i = 0; (c = *next) != '\0' && !isspace((unsigned char) c); next++)
            continue;

        len = next - start;

        if (len == 0)
            return NULL;

        /* Iterate through global command table. */
        for (i = 0; i < (sizeof (cmd_tbl) / sizeof (eh_cmd_tbl_t)); i++) {
            const eh_cmd_tbl_t * const tbl = cmd_tbl + i;

            /* Check if user entered command's abbreviation. */
            if ((len == 1) && (strncmp(*cmd, tbl->ect_short_name, len) == 0)) {
                hits  = 1;
                found = i;
                break;
            }

            /* Check if input matches current entry. */
            if (strncmp(*cmd, tbl->ect_name, len) == 0) {
                if (strlen(tbl->ect_name) == len) {
                    hits  = 1;
                    found = i;
                    break;
                }
                else {
                    hits++;
                    found = i;
                }
            }
        }

        /* Check if a match was found. */
        if (hits == 1) {
            *cmd = skip_whitespace(next);

            return (const eh_cmd_t *) cmd_tbl[found].ect_cmd;
        }
    }

    return NULL;
}

static const char *
skip_whitespace(const char *cmd)
{
    while (*cmd && isspace((unsigned char) *cmd))
        cmd++;

    return cmd;
}

