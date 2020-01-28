#!/usr/bin/env python3
"""
Output lines selected randomly from a file

Copyright 2005, 2007 Paul Eggert.
Copyright 2010 Darrell Benjamin Carbajal.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

Please see <http://www.gnu.org/licenses/> for a copy of the license.

$Id: randline.py,v 1.4 2010/04/05 20:04:43 eggert Exp $
"""

import random, sys
from optparse import OptionParser

class shuf:
    def __init__(self, filename, r):
        if r is None:
            if filename is "-":
                self.lines = [line.rstrip('\n')+'\n' for line in sys.stdin]
            else:
                f = open(filename, 'r')
                self.lines = [line.rstrip('\n')+'\n' for line in f]
                f.close()
        else:
            self.lines = list()
            i = r[0]
            while i <= r[1]:
                self.lines.append(str(i) + "\n")
                i += 1

    def chooseline(self, filename, n, i, r):
        lnum = len(self.lines)
        x = 0
        c = 0
        while x < lnum:
            if n is not "empty" and c >= n:
                break
            line = random.choice(self.lines)
            sys.stdout.write(line)
            c += 1
            if not r:
                self.lines.remove(line)
                x += 1

def main():
    version_msg = "%prog 2.0"
    usage_msg = """%prog [OPTION]... FILE

Generate random permutations."""
    parser = OptionParser(version=version_msg,
                          usage=usage_msg)
    parser.add_option("-n", "--head-count",
                      action="store", dest="count",
                      help="output at most COUNT lines")
    parser.add_option("-i", "--input-range",
                      action="store", dest="irange",
                      help="treat each number LO through HI as an input line")
    parser.add_option("-r", "--repeat",
                      action="store_true", dest="repeat",
                      default=False,
                      help="output lines can be repeated")
    
    options, args = parser.parse_args(sys.argv[1:])
    if options.count:
        try:
            count = float(options.count)
            count = int(count)
        except:
            parser.error("invalid line count: '{0}'".
                         format(options.count))
    else:
        count = "empty"
    if type(count) is (int or float) and count < 0:
        parser.error("invalid line count: '{0}'".
                     format(count))
    repeat = bool(options.repeat)
    irange = options.irange
    if len(args) > 1:                                                                                                                  
        parser.error("extra oprand '{0}'".format(args[0]))
    elif len(args) is 1:
        if irange is None:
            input_file = args[0]
        else:
            parser.error("extra oprand '{0}'".format(args[0]))
    else:
        if irange is None:
            input_file = "-"
        else:
            try:
                input_file = None
                irange = irange.split("-")
                
                
            except:
                parser.error("invalid input range: '{0}'".
                             format(options.irange))
            try:
                irange[0] = int(irange[0])
            except:
                parser.error("invalid input range: '{0}'".
                             format(irange[0]))
            try:
                irange[1] = int(irange[1])
            except:
                parser.error("invalid input range: '{0}'".
                             format(options.irange))
            if len(irange) != 2:
                parser.error("invalid input range: '{0}'".
                             format(options.irange.
                                    split('-', 1)[1]))
            if (int(irange[1]) < int(irange[0])):
                parser.error("invalid input range: '{0}'".
                             format(options.irange))
    try:
        generator = shuf(input_file, irange)
        generator.chooseline(input_file, count, irange, repeat)
    except IOError as e:
        errno, strerror = e.args
        parser.error("{0}: {1}".
                     format(input_file, strerror))

if __name__ == "__main__":
    main()
