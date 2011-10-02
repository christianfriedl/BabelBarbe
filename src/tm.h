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

#ifndef _TM_H
#define _TM_H

#include<time.h>
#include"dasl.h"
#include"results.h"


time_t maketime(unsigned int year, unsigned int month, unsigned int day, unsigned int hour, unsigned int minute, unsigned int second);
time_t tm__to_timestamp(tm_t *time); 
tm_t *tm__new(unsigned int year, unsigned int month, unsigned int day, unsigned int hour, unsigned int minute, unsigned int second);
tm_t *tm__new_from_current_time();
tm_t *tm__new_from_timestamp(time_t timestamp);
int tm__get_second(const tm_t *tm);
int tm__get_minute(const tm_t *tm);
int tm__get_hour(const tm_t *tm);
int tm__get_monthday(const tm_t *tm);
int tm__get_month(const tm_t *tm);
int tm__get_year(const tm_t *tm);
tm_t *tm__add_interval(tm_t *_time, unit_t unit, unsigned int count);
tm_t *tm__subtract_interval(tm_t *_time, unit_t unit, unsigned int count);
bool tm__equals(tm_t *_time1, tm_t *_time2);
unsigned int get_days_of_month(unsigned int year, unsigned int month);
tm_t *tm__copy(tm_t *source);

unsigned int units_to_seconds(unit_t unit, unsigned int count);
#endif
