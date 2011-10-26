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

#include"bnf.h"
#include"rule.h"
#include"parser.h"

rule_t *rule__new() {
    rule_t *rule = malloc(sizeof(*rule));
    if (rule) {
        rule->parse = NULL;
        rule->evaluate = NULL;
        rule->alternatives = NULL;
    } else
        bnf__raise_fatal_error("Unable to allocate rule.");
    return rule;
}
rule_t *rule__new_with_values(bool (*parse)(struct parser *parser), void (*evaluate)(struct parser *parser), rule_t ***alternatives) {
    rule_t *rule = rule__new();
    rule->parse = parse;
    rule->evaluate = evaluate;
    rule->alternatives = alternatives;
    return rule;
}
void rule__delete(rule_t *rule) {
    free(rule);
}
