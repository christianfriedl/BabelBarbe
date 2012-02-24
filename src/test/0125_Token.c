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
#include<cgenerics/CGAppState.h>
#include"BNFToken.h"

CGAppState* appState;

void testNewDelete() {
    printf("%s...\n", __func__);

    char* text = "xyz";
    BNFToken* token = BNFToken__new(appState, BNFTokenType_identifier, CGString__new(appState, text));
    BNFToken_delete(appState, token);

    printf("ok\n");
}
void testPrint() {
    printf("%s...\n", __func__);

    char* text = "xyz";
    BNFToken* token = BNFToken__new(appState, BNFTokenType_identifier, CGString__new(appState, text));
    BNFToken_print(appState, token);
    BNFToken_delete(appState, token);

    printf("ok\n");
}
void testGetters() {
    printf("%s...\n", __func__);

    char* text = "xyz";
    BNFToken* token = BNFToken__new(appState, BNFTokenType_identifier, CGString__new(appState, text));
    CGString* string;
    string = BNFToken_getTypeName(appState, token);
    assert(string != NULL);
    printf("'%s' should contain a stringified token type\n", string);
    CGString_delete(appState, string);
    string = BNFToken_toString(appState, token);
    assert(string != NULL);
    printf("'%s' should contain a stringified token\n", string);
    CGString_delete(appState, string);
    string = BNFToken_getText(appState, token);
    assert(string != NULL);
    assert(!CGString__compare(appState, string, CGString__new(appState, text)));
    BNFToken_delete(appState, token);

    printf("ok\n");
}

int main() {
    printf("=== %s ===\n", __FILE__);

    testNewDelete();
    testPrint();
    testGetters();

    printf("=== %s ok ===\n", __FILE__);
	return 0;
}
