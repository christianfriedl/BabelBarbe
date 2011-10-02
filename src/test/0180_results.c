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
#include"../results.h"
#include"../tm.h"

void test_result_new_delete() {
    result_t *result;
    printf("%s...\n", __func__);
    result = result__new();
    assert(result != NULL);
    assert(result->type == rt_none);
    assert(result->uint_val == 0);
    assert(result->unit_val == unit_none);
    result__delete(result);

    result = result__new_uint(456);
    assert(result != NULL);
    assert(result->type == rt_uint);
    assert(result->uint_val == 456);
    assert(result->unit_val == unit_none);
    result__delete(result);

    result = result__new_date(2011, 1, 1);
    assert(result != NULL);
    assert(result->type == rt_date);
    assert(tm__to_timestamp(result->datetime_val) == maketime(2011, 1, 1, 0, 0, 0));
    assert(result->unit_val == unit_none);
    result__delete(result);

    result = result__new_datetime(2011, 1, 1, 1, 1, 1);
    assert(result != NULL);
    assert(result->type == rt_datetime);
    assert(tm__to_timestamp(result->datetime_val) == maketime(2011, 1, 1, 1, 1, 1));
    assert(result->unit_val == unit_none);
    result__delete(result);

    result = result__new_from_current_datetime();
    assert(result != NULL);
    assert(result->type == rt_datetime);
    result__delete(result);

    result = result__new_from_current_date();
    assert(result != NULL);
    assert(result->type == rt_date);
    result__delete(result);

    result = result__new_unit(unit_days);
    assert(result != NULL);
    assert(result->type == rt_unit);
    assert(result->uint_val == 0);
    assert(result->unit_val == unit_days);
    result__delete(result);

    printf("ok\n");
}

void test_result_list_new_delete() {
    result_list_t *rl;
    result_t *result;
    printf("%s...\n", __func__);
    rl = result_list__new();

    result = result__new_datetime(2011, 1, 1, 10, 1, 1);

    result = result__new_unit(unit_days);
    assert(result != NULL);
    assert(result->type == rt_unit);
    assert(result->uint_val == 0);
    assert(result->unit_val == unit_days);
    result__delete(result);

    result_list__delete(rl);

    printf("ok\n");
}

void test_result_list_add() {
    result_t *result;
    result_list_t *rl;
    printf("%s...\n", __func__);
    rl = result_list__new();
    assert(rl->count_results == 0);

    result = result__new();
    assert(result != NULL);
    result_list__add_result(rl, result);
    assert(rl->count_results == 1);

    result = result__new();
    assert(result != NULL);
    result_list__add_result(rl, result);
    assert(rl->count_results == 2);

    result_list__clear(rl);
    assert(rl->count_results == 0);

    printf("ok\n");
}

void test_result_list_get_by_index() {
    result_t *result1, *result2, *result3;
    result_list_t *rl;
    printf("%s...\n", __func__);
    rl = result_list__new();
    assert(rl->count_results == 0);

    result1 = result__new();
    result2 = result__new();
    result3 = result__new();

    result_list__add_result(rl, result1);
    result_list__add_result(rl, result2);
    result_list__add_result(rl, result3);

    assert(rl->count_results == 3);

    assert(result_list__get_result_by_index(rl, 0) == result1);
    assert(result_list__get_result_by_index(rl, 1) == result2);
    assert(result_list__get_result_by_index(rl, 2) == result3);
    
    result_list__delete(rl);
    printf("ok\n");
}

void test_result_list_remove() {
    result_t *result1, *result2, *result3, *result;
    result_list_t *rl;
    printf("%s...\n", __func__);
    rl = result_list__new();
    assert(rl->count_results == 0);

    result1 = result__new();
    result2 = result__new();
    result3 = result__new();

    result_list__add_result(rl, result1);
    result_list__add_result(rl, result2);
    result_list__add_result(rl, result3);

    assert(rl->count_results == 3);

    result = result_list__remove_result_by_index(rl, 0);
    assert(result == result1);
    result__delete(result);
    assert(rl->count_results == 2);
    assert(result_list__get_result_by_index(rl, 0) == result2);
    assert(result_list__get_result_by_index(rl, 1) == result3);

    result = result_list__remove_result_by_index(rl, 1);
    assert(result == result3);
    result__delete(result);
    assert(rl->count_results == 1);
    assert(result_list__get_result_by_index(rl, 0) == result2);
    
    result = result_list__remove_result_by_index(rl, 0);
    assert(result == result2);
    result__delete(result);
    assert(rl->count_results == 0);

    result_list__delete(rl);
    printf("ok\n");
}


int main() {
    printf("=== %s ===\n", __FILE__);
    /*
        positive tests
    */

    test_result_new_delete();
    test_result_list_new_delete();
    test_result_list_add();
    test_result_list_get_by_index();
    test_result_list_remove();

    /*
        negative tests
    */

	return 0;
}
