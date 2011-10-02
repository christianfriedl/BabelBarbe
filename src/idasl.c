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
#include<stdlib.h>
#include<time.h>
#include<stdio.h>
#include<string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include"dasl.h"


void run_dasl() {
    dasl_result_t *dasl_result;
    char *input;
    bool next;
    do {
        input = readline("iDaSL> ");
        if (input != NULL && strlen(input) > 0 && strcasecmp(input, "q")) {
            next = true;
            dasl_result = dasl__run(input);
            if (dasl_result->is_parsed == true) {
                printf("%s\nok\n", dasl_result->text);
            } else {
                printf("ERROR: %s\n", dasl_result->error_text);
            }
            dasl_result__delete(dasl_result);
        } else
            next = false;
        // adding the previous input into history
        add_history(input);
        free(input);
    } while (next != false);
    printf("bye\n");
}


int main() {
    printf("Welcome to interactiveDaSL!\n\n");
    run_dasl();

    return 0;
}
