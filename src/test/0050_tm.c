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
#include"../tm.h"

/*
    positive tests
*/

void test_maketime() {
    time_t t;
    tm_t *_time;
    printf("%s...\n", __func__);
    t = maketime(2011, 1, 1, 0, 0, 0);
    _time = localtime(&t);
    printf("%s", asctime(_time));
    assert(_time->tm_sec == 0);
    assert(_time->tm_min == 0);
    assert(_time->tm_hour == 0);
    assert(_time->tm_mday == 1);
    assert(_time->tm_mon == 0);
    assert(_time->tm_year == 111);
    
    t = maketime(2011, 1, 31, 0, 0, 0);
    _time = localtime(&t);
    printf("%s", asctime(_time));
    assert(_time->tm_sec == 0);
    assert(_time->tm_min == 0);
    assert(_time->tm_hour == 0);
    assert(_time->tm_mday == 31);
    assert(_time->tm_mon == 0);
    assert(_time->tm_year == 111);
    
    t = maketime(2011, 2, 1, 0, 0, 0);
    _time = localtime(&t);
    printf("%s", asctime(_time));
    assert(_time->tm_sec == 0);
    assert(_time->tm_min == 0);
    assert(_time->tm_hour == 0);
    assert(_time->tm_mday == 1);
    assert(_time->tm_mon == 1);
    assert(_time->tm_year == 111);
    
    t = maketime(2011, 2, 28, 0, 0, 0);
    _time = localtime(&t);
    printf("%s", asctime(_time));
    assert(_time->tm_sec == 0);
    assert(_time->tm_min == 0);
    assert(_time->tm_hour == 0);
    assert(_time->tm_mday == 28);
    assert(_time->tm_mon == 1);
    assert(_time->tm_year == 111);
    
    t = maketime(2011, 2, 28, 23, 59, 59);
    _time = localtime(&t);
    printf("%s", asctime(_time));
    assert(_time->tm_sec == 59);
    assert(_time->tm_min == 59);
    assert(_time->tm_hour == 23);
    assert(_time->tm_mday == 28);
    assert(_time->tm_mon == 1);
    assert(_time->tm_year == 111);
    
    t = maketime(2011, 12, 31, 23, 59, 59);
    _time = localtime(&t);
    printf("%s", asctime(_time));
    assert(_time->tm_sec == 59);
    assert(_time->tm_min == 59);
    assert(_time->tm_hour == 23);
    assert(_time->tm_mday == 31);
    assert(_time->tm_mon == 11);
    assert(_time->tm_year == 111);
    
    t = maketime(1970, 1, 1, 0, 0, 0);
    _time = localtime(&t);
    printf("%s", asctime(_time));
    assert(_time->tm_sec == 0);
    assert(_time->tm_min == 0);
    assert(_time->tm_hour == 0);
    assert(_time->tm_mday == 1);
    assert(_time->tm_mon == 0);
    assert(_time->tm_year == 70);
    
    printf("ok\n");
}

void test_tm_new_delete() {
    tm_t *_time, *_time2;
    time_t timestamp;
    printf("%s...\n", __func__);

    _time = tm__new(2011,8,10,13,33,20);
    timestamp = tm__to_timestamp(_time);
    assert(_time->tm_sec == 20); 
    assert(_time->tm_min == 33); 
    assert(_time->tm_hour == 13); 
    assert(_time->tm_mday == 10); 
    assert(_time->tm_mon == 7); 
    assert(_time->tm_year == 111); 
    assert(_time->tm_wday == 3); 
    assert(_time->tm_yday == 221); 
    assert(_time->tm_isdst == 1); 
    assert(_time->tm_gmtoff == 7200); 
    assert(!strcmp(_time->tm_zone, "CEST"));
    tm__delete(_time);

    _time = tm__new_from_timestamp(timestamp);
    assert(_time->tm_sec == 20); 
    assert(_time->tm_min == 33); 
    assert(_time->tm_hour == 13); 
    assert(_time->tm_mday == 10); 
    assert(_time->tm_mon == 7); 
    assert(_time->tm_year == 111); 
    assert(_time->tm_wday == 3); 
    assert(_time->tm_yday == 221); 
    assert(_time->tm_isdst == 1); 
    assert(_time->tm_gmtoff == 7200); 
    assert(!strcmp(_time->tm_zone, "CEST"));
    tm__delete(_time);

    _time = tm__new_from_current_time();
    _time2 = tm__new_from_timestamp(time(NULL));
    assert(tm__equals(_time, _time2));
    tm__delete(_time);
    tm__delete(_time2);
    
    printf("ok\n");
}

void test_units_to_seconds() {
    printf("%s...\n", __func__);
    assert(units_to_seconds(unit_minutes, 32) == 32 * 60);
    assert(units_to_seconds(unit_hours, 32) == 32 * 60 * 60);
    assert(units_to_seconds(unit_days, 32) == 32 * 60 * 60 * 24);
    assert(units_to_seconds(unit_weeks, 32) == 32 * 60 * 60 * 24 * 7);
    printf("ok\n");
}

void test_tm__add_interval() {
    tm_t *now_tm = NULL, *then_tm = NULL;
    time_t t;
    printf("%s...\n", __func__);
    t = time(NULL);
    now_tm = tm__new_from_timestamp(t);
    printf("%s\n", asctime(now_tm));
    then_tm = tm__add_interval(now_tm, unit_seconds, 10);
    printf("... + 10s = %s", asctime(then_tm));
    tm__delete(then_tm);
    then_tm = tm__add_interval(now_tm, unit_minutes, 10);
    printf("... + 10m = %s", asctime(then_tm));
    tm__delete(then_tm);
    then_tm = tm__add_interval(now_tm, unit_hours, 10);
    printf("... + 10h = %s", asctime(then_tm));
    tm__delete(then_tm);
    then_tm = tm__add_interval(now_tm, unit_days, 10);
    printf("... + 10d = %s", asctime(then_tm));
    tm__delete(then_tm);
    then_tm = tm__add_interval(now_tm, unit_weeks, 2);
    printf("... + 2w = %s", asctime(then_tm));
    then_tm = tm__add_interval(now_tm, unit_years, 2);
    printf("... + 2y = %s", asctime(then_tm));
    tm__delete(then_tm);
    then_tm = tm__add_interval(now_tm, unit_months, 1);
    printf("... + 1mo = %s", asctime(then_tm));
    tm__delete(then_tm);
    then_tm = tm__add_interval(now_tm, unit_months, 2);
    printf("... + 2mo = %s", asctime(then_tm));
    tm__delete(then_tm);
    then_tm = tm__add_interval(now_tm, unit_months, 12);
    printf("... + 12mo = %s", asctime(then_tm));
    tm__delete(then_tm);
    then_tm = tm__add_interval(now_tm, unit_months, 24);
    printf("... + 24mo = %s", asctime(then_tm));
    tm__delete(then_tm);
    tm__delete(now_tm);
    now_tm = tm__new(2011, 10, 31, 0, 0, 0);
    printf("%s\n", asctime(now_tm));
    then_tm = tm__add_interval(now_tm, unit_months, 1);
    printf("... + 1mo = %s", asctime(then_tm));
    tm__delete(then_tm);
    then_tm = tm__add_interval(now_tm, unit_months, 2);
    printf("... + 2mo = %s", asctime(then_tm));
    tm__delete(then_tm);
    then_tm = tm__add_interval(now_tm, unit_months, 3);
    printf("... + 3mo = %s", asctime(then_tm));
    tm__delete(then_tm);
    then_tm = tm__add_interval(now_tm, unit_months, 4);
    printf("... + 4mo = %s", asctime(then_tm));
    tm__delete(then_tm);
    then_tm = tm__add_interval(now_tm, unit_months, 12);
    printf("... + 12mo = %s", asctime(then_tm));
    tm__delete(then_tm);
    then_tm = tm__add_interval(now_tm, unit_months, 13);
    printf("... + 13mo = %s", asctime(then_tm));
    tm__delete(then_tm);
    then_tm = tm__add_interval(now_tm, unit_months, 24);
    printf("... + 24mo = %s", asctime(then_tm));
    tm__delete(then_tm);

    tm__delete(now_tm);
    printf("ok\n");
}

void test_tm_getters() {
    tm_t *_time;
    printf("%s...\n", __func__);
    _time = tm__new(2011, 11, 1, 2, 3, 4);

    assert(tm__get_second(_time) == 4);
    assert(tm__get_minute(_time) == 3);
    assert(tm__get_hour(_time) == 2);
    assert(tm__get_monthday(_time) == 1);
    assert(tm__get_month(_time) == 11);
    assert(tm__get_year(_time) == 2011);

    tm__delete(_time);
    printf("ok\n");
}

int main() {
    printf("=== %s ===\n", __FILE__);
    /*
        positive tests
    */

    test_maketime();
    test_tm_new_delete();
    test_units_to_seconds();
    test_tm__add_interval();
    test_tm_getters();

    /*
        negative tests
    */

	return 0;
}
