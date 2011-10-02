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

#include<stdlib.h>
#include"dasl.h"
#include"tm.h"


tm_t *tm__add_months(tm_t *_time, unsigned int count);
tm_t *tm__add_years(tm_t *_time, unsigned int count);
tm_t *tm__subtract_months(tm_t *_time, unsigned int count);
tm_t *tm__subtract_years(tm_t *_time, unsigned int count);
bool is_leap_year(unsigned int year);

bool is_leap_year(unsigned int year) {
    if (year % 4 == 0) {
        if (year % 100 == 0) {
            if (year % 400 == 0) {
                return true;
            } else {
                return false;
            }
        } else {
            return true;
        }
    } else {
        return false;
    }
}

unsigned int get_days_of_month(unsigned int year, unsigned int month) {
    if (month == 2) {
        if (is_leap_year(year)) {
            return 29;
        } else {
            return 28;
        }
    } else if (month >= 8) {
        if (month % 2 == 0)
            return 31;
        else
            return 30;
    } else {
        if (month % 2 == 0)
            return 30;
        else
            return 31;
    }
}

time_t maketime(unsigned int year, unsigned int month, unsigned int day, unsigned int hour, unsigned int minute, unsigned int second) {
    struct tm _time;
    _time.tm_sec = second;     /* seconds (0 - 60) */
    _time.tm_min = minute;     /* minutes (0 - 59) */
    _time.tm_hour = hour;    /* hours (0 - 23) */
    _time.tm_mday = day;    /* day of month (1 - 31) */
    _time.tm_mon = month - 1;     /* month of year (0 - 11) */
    _time.tm_year = year - 1900;    /* year - 1900 */
    _time.tm_isdst = -1;         /* let glibc guess! */
    return mktime(&_time);
}

tm_t *tm__copy(tm_t *source) {
    tm_t *_time = malloc(sizeof(*_time));
    if (_time != NULL) {
        _time->tm_sec = source->tm_sec;     /* seconds (0 - 60) */
        _time->tm_min = source->tm_min;     /* minutes (0 - 59) */
        _time->tm_hour = source->tm_hour;    /* hours (0 - 23) */
        _time->tm_mday = source->tm_mday;    /* day of month (1 - 31) */
        _time->tm_mon =  source->tm_mon;     /* month of year (0 - 11) */
        _time->tm_year = source->tm_year;    /* year - 1900 */
        _time->tm_wday = source->tm_wday;    /* day of week (Sunday = 0) */
        _time->tm_yday = source->tm_yday;    /* day of year (0 - 365) */
        _time->tm_isdst = source->tm_isdst;   /* is summer time in effect? */
        _time->tm_zone = source->tm_zone;  /* abbreviation of timezone name */
        _time->tm_gmtoff = source->tm_gmtoff; /* offset from UTC in seconds */
    } else
        dasl_raise_fatal_error("Unable to allocate time structure.");
    return _time;
}

tm_t *tm__new(unsigned int year, unsigned int month, unsigned int day, unsigned int hour, unsigned int minute, unsigned int second) {
    time_t timestamp;
    tm_t *_time = NULL;
    tm_t _timesrc;
    _timesrc.tm_sec = second;     /* seconds (0 - 60) */
    _timesrc.tm_min = minute;     /* minutes (0 - 59) */
    _timesrc.tm_hour = hour;    /* hours (0 - 23) */
    _timesrc.tm_mday = day;    /* day of month (1 - 31) */
    _timesrc.tm_mon = month - 1;     /* month of year (0 - 11) */
    _timesrc.tm_year = year - 1900;    /* year - 1900 */
    _timesrc.tm_isdst = -1;         /* let glibc guess! */
    timestamp = mktime(&_timesrc);
    _time = localtime(&timestamp);
    return tm__copy(_time);
}

int tm__get_second(const tm_t *tm) {
    return tm->tm_sec;
}
int tm__get_minute(const tm_t *tm) {
    return tm->tm_min;
}
int tm__get_hour(const tm_t *tm) {
    return tm->tm_hour;
}
int tm__get_monthday(const tm_t *tm) {
    return tm->tm_mday;
}
int tm__get_month(const tm_t *tm) {
    return tm->tm_mon + 1;
}
int tm__get_year(const tm_t *tm) {
    return tm->tm_year + 1900;
}

tm_t *tm__new_from_timestamp(time_t timestamp) {
    tm_t *_time = malloc(sizeof(*_time));
    if (_time != NULL) {
        tm_t *systime = localtime(&timestamp);    
        _time->tm_sec = systime->tm_sec;     /* seconds (0 - 60) */
        _time->tm_min = systime->tm_min;     /* minutes (0 - 59) */
        _time->tm_hour = systime->tm_hour;    /* hours (0 - 23) */
        _time->tm_mday = systime->tm_mday;    /* day of month (1 - 31) */
        _time->tm_mon =  systime->tm_mon;     /* month of year (0 - 11) */
        _time->tm_year = systime->tm_year;    /* year - 1900 */
        _time->tm_wday = systime->tm_wday;    /* day of week (Sunday = 0) */
        _time->tm_yday = systime->tm_yday;    /* day of year (0 - 365) */
        _time->tm_isdst = systime->tm_isdst;   /* is summer time in effect? */
        _time->tm_zone = systime->tm_zone;  /* abbreviation of timezone name */
        _time->tm_gmtoff = systime->tm_gmtoff; /* offset from UTC in seconds */
    } else
        dasl_raise_fatal_error("Unable to allocate _time structure.");
    return _time;
}

tm_t *tm__new_from_current_time() {
    tm_t *_time = malloc(sizeof(*_time));
    if (_time != NULL) {
        time_t t;
        tm_t *systime = NULL;
        t = time(NULL);
        systime = localtime(&t);
        _time->tm_sec = systime->tm_sec;     /* seconds (0 - 60) */
        _time->tm_min = systime->tm_min;     /* minutes (0 - 59) */
        _time->tm_hour = systime->tm_hour;    /* hours (0 - 23) */
        _time->tm_mday = systime->tm_mday;    /* day of month (1 - 31) */
        _time->tm_mon = systime->tm_mon;     /* month of year (0 - 11) */
        _time->tm_year = systime->tm_year;    /* year - 1900 */
        _time->tm_wday = systime->tm_wday;    /* day of week (Sunday = 0) */
        _time->tm_yday = systime->tm_yday;    /* day of year (0 - 365) */
        _time->tm_isdst = systime->tm_isdst;   /* is summer time in effect? */
        _time->tm_zone = systime->tm_zone;  /* abbreviation of timezone name */
        _time->tm_gmtoff = systime->tm_gmtoff; /* offset from UTC in seconds */
    } else
        dasl_raise_fatal_error("Unable to allocate time structure.");
    return _time;
}

void tm__delete(tm_t *tm) {
    free(tm);
}
time_t tm__to_timestamp(tm_t *_time) {
    tm_t _time2;
    _time2.tm_sec = _time->tm_sec;     /* seconds (0 - 60) */
    _time2.tm_min = _time->tm_min;     /* minutes (0 - 59) */
    _time2.tm_hour = _time->tm_hour;    /* hours (0 - 23) */
    _time2.tm_mday = _time->tm_mday;    /* day of month (1 - 31) */
    _time2.tm_mon = _time->tm_mon;     /* month of year (0 - 11) */
    _time2.tm_year = _time->tm_year;    /* year - 1900 */
    _time2.tm_wday = _time->tm_wday;    /* day of week (Sunday = 0) */
    _time2.tm_yday = _time->tm_yday;    /* day of year (0 - 365) */
    _time2.tm_isdst = _time->tm_isdst;   /* is summer time in effect? */
    _time2.tm_zone = _time->tm_zone;  /* abbreviation of timezone name */
    _time2.tm_gmtoff = _time->tm_gmtoff; /* offset from UTC in seconds */
    return mktime(&_time2);
}

bool tm__equals(tm_t *_time1, tm_t *_time2) {
    bool rv = true; 
    rv &= (_time1->tm_sec  == _time2->tm_sec);     /* seconds (0 - 60) */
    rv &= (_time1->tm_min  == _time2->tm_min);     /* minutes (0 - 59) */
    rv &= (_time1->tm_hour  == _time2->tm_hour);    /* hours (0 - 23) */
    rv &= (_time1->tm_mday  == _time2->tm_mday);    /* day of month (1 - 31) */
    rv &= (_time1->tm_mon  == _time2->tm_mon);     /* month of year (0 - 11) */
    rv &= (_time1->tm_year  == _time2->tm_year);    /* year - 1900 */
    rv &= (_time1->tm_wday  == _time2->tm_wday);    /* day of week (Sunday = 0) */
    rv &= (_time1->tm_yday  == _time2->tm_yday);    /* day of year (0 - 365) */
    rv &= (_time1->tm_isdst  == _time2->tm_isdst);   /* is summer time in effect? */
    rv &= (_time1->tm_zone  == _time2->tm_zone);  /* abbreviation of timezone name */
    rv &= (_time1->tm_gmtoff  == _time2->tm_gmtoff); /* offset from UTC in seconds */
    return rv;

}

tm_t *tm__add_interval(tm_t *_time, unit_t unit, unsigned int count) {
    unsigned int timestamp = 0;
    timestamp = tm__to_timestamp(_time);
    switch (unit) {
        case unit_seconds:
        case unit_minutes:
        case unit_hours:
        case unit_days:
        case unit_weeks:
            timestamp += units_to_seconds(unit, count);
            return tm__new_from_timestamp(timestamp);
        case unit_months:
            return tm__add_months(_time, count);
        case unit_years:
            return tm__add_years(_time, count);
        default:
            dasl_raise_fatal_error("not implemented");
            return NULL;
    }
}

tm_t *tm__add_months(tm_t *_time, unsigned int count) {
    tm_t *_time2 = NULL;
    _time2 = tm__add_years(_time, count / 12);
    _time2->tm_mon += count % 12;
    if (_time2->tm_mon > 11) {
        _time2->tm_mon -= 12;
        _time2->tm_year++;
    }
    if (_time2->tm_mday >= get_days_of_month(_time2->tm_year + 1900, _time2->tm_mon + 1))
        _time2->tm_mday = get_days_of_month(_time2->tm_year + 1900, _time2->tm_mon + 1);
    return _time2;
}
    
tm_t *tm__add_years(tm_t *_time, unsigned int count) {
    return tm__new_from_timestamp(maketime(_time->tm_year + count + 1900, _time->tm_mon + 1, _time->tm_mday, _time->tm_hour, _time->tm_min, _time->tm_sec));
}

tm_t *tm__subtract_interval(tm_t *_time, unit_t unit, unsigned int count) {
    unsigned int timestamp = 0;
    timestamp = tm__to_timestamp(_time);
    switch (unit) {
        case unit_seconds:
        case unit_minutes:
        case unit_hours:
        case unit_days:
        case unit_weeks:
            timestamp -= units_to_seconds(unit, count);
            return tm__new_from_timestamp(timestamp);
        case unit_months:
            return tm__subtract_months(_time, count);
        case unit_years:
            return tm__subtract_years(_time, count);
        default:
            dasl_raise_fatal_error("not implemented");
            return NULL;
    }
}

tm_t *tm__subtract_months(tm_t *_time, unsigned int count) {
    tm_t *_time2 = NULL;
    _time2 = tm__subtract_years(_time, count / 12);
    _time2->tm_mon -= count % 12;
    if (_time2->tm_mon < 0) { /* NOTE: tm_XX is an int, so this is possible - but is it portable?? */
        _time2->tm_mon += 12;
        _time2->tm_year--;
    }
    if (_time2->tm_mday >= get_days_of_month(_time2->tm_year + 1900, _time2->tm_mon + 1))
        _time2->tm_mday = get_days_of_month(_time2->tm_year + 1900, _time2->tm_mon + 1);
    return _time2;
}
    
tm_t *tm__subtract_years(tm_t *_time, unsigned int count) {
    return tm__new_from_timestamp(maketime(_time->tm_year - count + 1900, _time->tm_mon + 1, _time->tm_mday, _time->tm_hour, _time->tm_min, _time->tm_sec));
}

unsigned int units_to_seconds(unit_t unit, unsigned int count) {
    switch(unit) {
        case unit_seconds:
            return count;
            break;
        case unit_minutes:
            return 60 * count;
            break;
        case unit_hours:
            return 60 * 60 * count;
            break;
        case unit_days:
            return 24 * 60 * 60 * count;
            break;
        case unit_weeks:
            return 7 * 24 * 60 * 60 * count;
            break;
        default:
            dasl_raise_fatal_error("units_to_seconds makes no sense for this unit");
    }
    return 0;
}
