    /*
    DaSL - Datetime Specific Language, a little DSL for dealing with dates and times

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

#include<assert.h>
#include<stdio.h>
#include"../parser.h"

bool do_parse(parser_t *parser) {
    return true;
}

void do_evaluate(parser_t *parser) {
}

void test_rules__new_delete() {
    rule_t subrule_1 = { "subrule_1", NULL, NULL, { { NULL, NULL } } };
    rule_t subrule_2 = { "subrule_2", NULL, NULL, { { NULL, NULL } } };
    rule_t rule = { "rule", do_parse, do_evaluate, { { &subrule_1, NULL }, { &subrule_2, NULL }, { NULL } } };
    printf("%s...\n", __func__);

    assert(rule.parse == do_parse);
    assert(rule.evaluate == do_evaluate);
    assert(rule.alternatives[0][0] == &subrule_1);
    assert(rule.alternatives[1][0] == &subrule_2);

    printf("ok\n");
}

int main() {
    printf("=== %s ===\n", __FILE__);
    /*
        positive tests
    */

    test_rules__new_delete();

    /*
        negative tests
    */

	return 0;
}
