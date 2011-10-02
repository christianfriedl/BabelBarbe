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
#include<time.h>
#include<stdio.h>
#include<string.h>
#include"../parser.h"
#include"../results.h"

/*
    positive tests
*/

void test_dasl_run() {
    dasl_result_t *dasl_result;

    printf("%s...\n", __func__);

    dasl_result = dasl__run("'2011-01-01 00:00:00' + 1 HOUR");

    assert(dasl_result->is_parsed == true);
    assert(tm__to_timestamp(dasl_result->datetime) == maketime(2011,1,1,1,0,0));
    printf("%s\n", dasl_result->text);
    assert(!strcmp(dasl_result->text, "2011-01-01 01:00:00"));
    dasl_result__delete(dasl_result);

    printf("ok\n");

}


int main() {
    printf("=== %s ===\n", __FILE__);
    /*
        positive tests
    */

    test_dasl_run();

    /*
        negative tests
    */

	return 0;
}
