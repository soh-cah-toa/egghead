#!/usr/bin/env perl

# Copyright (C) 2012, Kevin Polulak <kpolulak@gmail.com>
#
# Egghead is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# any later version.
#
# Egghead is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

use strict;
use warnings;

use Test::More;
use IPC::Open3;
use IO::Select;
use File::Spec;

my $egghead = File::Spec->join('bin', 'egghead');

# Test --license switch
{
    my $expected = <<'EOF';
Copyright (C) 2012, Kevin Polulak <kpolulak@gmail.com>
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.
EOF

    my $pid = open3(undef,
                    \*EH_STDOUT,
                    undef,
                    "$egghead --license");

    my $select = IO::Select->new();
    $select->add(\*EH_STDOUT);

    my $got = '';

    while (my @fh_ready = $select->can_read()) {
        foreach my $fh (@fh_ready) {
            next unless defined $fh;

            $got .= <$fh>;
            $select->remove($fh) if eof $fh;

            next unless defined $got;
        }
    }

    waitpid $pid, 0;

    is($got, $expected, 'Testing --license switch');
}

# Test --help switch
{
    my $expected = <<'EOF';
Usage: egghead [OPTION]... [FILE]...
A GNU-compliant brainfuck interpreter.

   --strict     Interpreter only recognizes valid brainfuck commands.
   --help       Display help message.
   --license    Display license information.

With no FILE, or when FILE is -, reads standard input.

For more information, see the Egghead manpage (man egghead).
EOF

    my $pid = open3(undef,
                    \*EH_STDOUT,
                    undef,
                    "$egghead --help");

    my $select = IO::Select->new();
    $select->add(\*EH_STDOUT);

    my $got = '';

    while (my @fh_ready = $select->can_read()) {
        foreach my $fh (@fh_ready) {
            next unless defined $fh;

            $got .= <$fh>;
            $select->remove($fh) if eof $fh;

            next unless defined $got;
        }
    }

    waitpid $pid, 0;

    is($got, $expected, 'Testing --help switch');
}
done_testing();

# vim: ft=perl
