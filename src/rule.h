/*
BNF Parser

Copyright (C) 2011  Christian Friedl

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _RULE_H
#define _RULE_H

#include"bnf.h"

struct parser;

struct rule;

struct rule {
    void (*xyz)();
    bool (*parse)(struct parser *parser);
    void (*evaluate)(struct parser *parser);
    struct rule *alternatives[20][20];
};

typedef struct rule rule_t;

#endif
