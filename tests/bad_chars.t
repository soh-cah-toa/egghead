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

# Test invalid characters without --strict
{
    my $expected = "Hello World!";

    my $pid = open3(undef,
                    \*EH_STDOUT,
                    undef,
                    "$egghead tests/testlib/hello2.bf");

    my $select = IO::Select->new();
    $select->add(\*EH_STDOUT);

    my $got = '';

    while (my @fh_ready = $select->can_read()) {
        foreach my $fh (@fh_ready) {
            next unless defined $fh;

            $got = <$fh>;
            $select->remove($fh) if eof $fh;

            next unless defined $got;
        }
    }

    waitpid $pid, 0;

    is($got, $expected, 'Testing invalid characters without --strict');
}

# Test invalid characters with --strict
{
    my $expected = <<'EOF';
[ERROR] Character T is not a valid brainfuck command.
EOF

    my $pid = open3(undef,
                    undef,
                    \*EH_STDERR,
                    "$egghead --strict tests/testlib/hello2.bf");

    my $select = IO::Select->new();
    $select->add(\*EH_STDERR);

    my $got = '';

    while (my @fh_ready = $select->can_read()) {
        foreach my $fh (@fh_ready) {
            next unless defined $fh;

            $got = <$fh>;
            $select->remove($fh) if eof $fh;

            next unless defined $got;
        }
    }

    waitpid $pid, 0;

    is($got, $expected, 'Testing invalid characters with --strict');
}

done_testing();

# vim: ft=perl
