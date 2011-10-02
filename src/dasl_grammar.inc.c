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

bool parser__parse_symbol(parser_t *parser, const char *symbol);
bool parser__parse_plusminussign(parser_t *parser);
bool parser__parse_minussign(parser_t *parser);
bool parser__parse_colon(parser_t *parser);
bool parser__parse_openparen(parser_t *parser);
bool parser__parse_closeparen(parser_t *parser);
bool parser__parse_uint(parser_t *parser);
bool parser__parse_function(parser_t *parser);
bool parser__parse_eof(parser_t *parser);
bool parser__parse_double_colon(parser_t *parser);
bool parser__parse_type_identifier(parser_t *parser);
bool parser__parse_untyped_expression(parser_t *parser);
bool parser__parse_quote(parser_t *parser);
bool parser__parse_unit_identifier(parser_t *parser);
bool parser__parse_dot(parser_t *parser);
bool parser__parse_dotfunction_identifier(parser_t *parser);


void parser__evaluate_uint(parser_t *parser);
void parser__evaluate_datetime_literal(parser_t *parser);
void parser__evaluate_date_literal(parser_t *parser);
void parser__evaluate_function(parser_t *parser);
void parser__evaluate_plusminus_expression(parser_t *parser);
void parser__evaluate_interval_expression(parser_t *parser);
void parser__evaluate_unit_identifier(parser_t *parser);
void parser__evaluate_type_identifier(parser_t *parser);
void parser__evaluate_typed_term(parser_t *parser);
void parser__evaluate_dotfunction_identifier(parser_t *parser);
void parser__evaluate_dotfunction_term(parser_t *parser);

rule_t rule_start;
rule_t rule_expression;
rule_t rule_typed_term;
rule_t rule_type_identifier;
rule_t rule_double_colon;
rule_t rule_plus_expression;
rule_t rule_perhaps_composed_term;
rule_t rule_dotfunction_term;
rule_t rule_dotfunction_identifier;
rule_t rule_dot;
rule_t rule_term;
rule_t rule_interval_expression;
rule_t rule_function;
rule_t rule_date_literal;
rule_t rule_colon;
rule_t rule_quote;
rule_t rule_datetime_literal;
rule_t rule_uint; 
rule_t rule_openparen; 
rule_t rule_closeparen; 
rule_t rule_plussign;
rule_t rule_minussign;
rule_t rule_eof; 

#define NULL_RULE { repeat_off, 0, { NULL } } 

rule_t rule_uint = { 
    "rule_uint",
    parser__parse_uint, 
    parser__evaluate_uint, 
    {  
        NULL_RULE
    } 
};
rule_t rule_eof = { 
    "rule_eof",
    parser__parse_eof, 
    NULL, 
    {  
        NULL_RULE
    } 
};
rule_t rule_openparen = {
    "rule_openparen",
    parser__parse_openparen,
    NULL,
    {  
        NULL_RULE
    } 
};
rule_t rule_closeparen = {
    "rule_closeparen",
    parser__parse_closeparen,
    NULL,
    {  
        NULL_RULE
    } 
};
rule_t rule_plusminussign = {
    "rule_plusminussign",
    parser__parse_plusminussign,
    NULL,
    {  
        NULL_RULE
    } 
};
rule_t rule_minussign = {
    "rule_minussign",
    parser__parse_minussign,
    NULL,
    {  
        NULL_RULE
    } 
};
rule_t rule_quote = {
    "rule_quote",
    parser__parse_quote,
    NULL,
    {  
        NULL_RULE
    } 
};
rule_t rule_colon = {
    "rule_colon",
    parser__parse_colon,
    NULL,
    {  
        NULL_RULE
    } 
};
rule_t rule_datetime_literal = {
    "rule_datetime_literal",
    NULL,
    parser__evaluate_datetime_literal,
    {
        { repeat_off, 0, { &rule_quote, &rule_uint, &rule_minussign, &rule_uint, &rule_minussign, &rule_uint, 
                                &rule_uint, &rule_colon, &rule_uint, &rule_colon, &rule_uint, &rule_quote, NULL } },
        NULL_RULE
    }
};
rule_t rule_date_literal = {
    "rule_date_literal",
    NULL,
    parser__evaluate_date_literal,
    {
        { repeat_off, 0, { &rule_quote, &rule_uint, &rule_minussign, &rule_uint, &rule_minussign, &rule_uint, &rule_quote, NULL } },
        NULL_RULE
    }
};
rule_t rule_function = {
    "rule_function",
    parser__parse_function,
    parser__evaluate_function,
    {
        NULL_RULE
    }
};
rule_t rule_unit_identifier = {
    "rule_unit_identifier",
    parser__parse_unit_identifier,
    parser__evaluate_unit_identifier,
    {
        NULL_RULE
    }
};
rule_t rule_interval_expression = {
    "rule_interval_expression",
    NULL,
    parser__evaluate_interval_expression,
    {
        { repeat_off, 0, { &rule_uint, &rule_unit_identifier, NULL } },
        NULL_RULE
    }
};
rule_t rule_lhs_term = {
    "rule_lhs_term",
    NULL,
    NULL,
    {
        { repeat_off, 0, { &rule_interval_expression, NULL } },
        { repeat_off, 0, { &rule_uint, NULL } },
        { repeat_off, 0, { &rule_datetime_literal, NULL } },
        { repeat_off, 0, { &rule_date_literal, NULL } },
        { repeat_off, 0, { &rule_function, NULL } },
        { repeat_off, 0, { &rule_openparen, &rule_expression, &rule_closeparen, NULL } },
        NULL_RULE
    }
}
rule_t rule_term = {
    "rule_term",
    NULL,
    NULL,
    {
        { repeat_off, 0, { &rule_lhs_term, NULL } },
        { repeat_off, 0, { &rule_dotfunction_term, NULL } },
        NULL_RULE
    }
};
rule_t rule_leftassoc_expression = {
    "rule_leftassoc_expression",
    NULL,
    parser__evaluate_leftassoc_expression,
    {
        { repeat_on, 1, { &rule_lhs_term, &rule_leftassoc_operator, &rule_term, NULL } },
        NULL_RULE
    }
};
rule_t rule_double_colon = {
    "rule_double_colon",
    parser__parse_double_colon,
    NULL,
    {
        NULL_RULE
    }
};
rule_t rule_type_identifier = {
    "rule_type_identifier",
    parser__parse_type_identifier,
    parser__evaluate_type_identifier,
    {
        NULL_RULE
    }
};
rule_t rule_typed_term = {
    "rule_typed_term",
    NULL,
    parser__evaluate_typed_term,
    {
        { repeat_off, 0, { &rule_term, &rule_double_colon, &rule_type_identifier, NULL } },
        NULL_RULE
    }
};
rule_t rule_expression = {
    "rule_expression",
    NULL,
    NULL,
    {
        { repeat_off, 0, { &rule_plusminus_expression, NULL } },
        { repeat_off, 0, { &rule_perhaps_composed_term, NULL } },
        NULL_RULE
    }
};
rule_t rule_dot = {
    "rule_dot",
    parser__parse_dot,
    NULL,
    {
        NULL_RULE
    }
};
rule_t rule_dotfunction_identifier = {
    "rule_dot",
    parser__parse_dotfunction_identifier,
    parser__evaluate_dotfunction_identifier,
    {
        NULL_RULE
    }
};
rule_t rule_dotfunction_term = {
    "rule_dotfunction_term",
    NULL,
    parser__evaluate_dotfunction_term,
    {
        { repeat_off, 0, { &rule_dotfunction_identifier, &rule_openparen, &rule_unit_identifier, &rule_closeparen, NULL } },
        NULL_RULE
    }
};
rule_t rule_start = { 
    "rule_start",
    NULL, 
    NULL, 
    { 
        { repeat_off, 0, { &rule_expression, &rule_eof, NULL } },
        NULL_RULE
    } 
};


bool parser__parse_uint(parser_t *parser) {
    bool is_parsed = false;
    token_t *token = NULL;
    parser__debug_print_enter_function(parser, __func__);
    token = tlc__get_current(parser->tlc);
    is_parsed = (token != NULL && token->type != t_eof);
    if (is_parsed) {
        if (token->type == t_literal_uint) {
            is_parsed = true;
        } else
            is_parsed = false;
    }
    parser__debug_print_exit_function(parser, __func__);
    return is_parsed;
}

bool parser__parse_eof(parser_t *parser) {
    bool is_parsed = false;
    token_t *token = NULL;
    parser__debug_print_enter_function(parser, __func__);
    token = tlc__get_current(parser->tlc);
    if (token->type == t_eof) {
        is_parsed = true;
    } else {
        is_parsed = false;
    }
    parser__debug_print_exit_function(parser, __func__);
    return is_parsed;
}

bool parser__parse_function(parser_t *parser) {
    token_t *token = NULL;
    bool is_parsed = false;
    parser__debug_print_enter_function(parser, __func__);
    token = tlc__get_current(parser->tlc);
    if (token->type == t_identifier && !strcasecmp(token->text, "TODAY")) {
        is_parsed = true;
    } else if (token->type == t_identifier && !strcasecmp(token->text, "NOW")) {
        is_parsed = true;
    } else
        is_parsed = false;
    parser__debug_print_exit_function(parser, __func__);
    return is_parsed;
}

void parser__evaluate_function(parser_t *parser) {
    token_t *token = NULL;
    token = tlc__get_current(parser->tlc);
    if (!strcasecmp(token->text, "TODAY")) {
        result_list__add_result(parser->result_list, result__new_from_current_date());
    } else if (!strcasecmp(token->text, "NOW")) {
        result_list__add_result(parser->result_list, result__new_from_current_datetime());
    }
}

void parser__evaluate_uint(parser_t *parser) {
    token_t *token = NULL;
    token = tlc__get_current(parser->tlc);
    result_list__add_result(parser->result_list, result__new_uint(atoi(token->text)));
}
void parser__evaluate_plusminus_expression(parser_t *parser) {
    result_t *operator = NULL;
    result_t *result1 = NULL;
    result_t *result2 = NULL;
    result_t *result = NULL;

    result2 = parser__remove_last_result(parser);
    operator = parser__remove_last_result(parser);
    result1 = parser__remove_last_result(parser);


    if (operator->type == rt_operator) {
        if (!strcmp(operator->string_val, "+"))
            result = add_results(result1, result2);
        else if (!strcmp(operator->string_val, "-"))
            result = subtract_results(result1, result2);
        else {
            result = NULL;
            dasl_raise_fatal_error("Bad operator.");
        }
    } else
        dasl_raise_fatal_error("Not an operator.");

    result_list__add_result(parser->result_list, result);
    result__delete(result1);
    result__delete(result2);
}
bool parser__parse_plusminussign(parser_t *parser) {
    bool is_parsed = parser__parse_symbol(parser, "+");
    if (is_parsed)
        result_list__add_result(parser->result_list, result__new_operator("+"));
    else {
        is_parsed = parser__parse_symbol(parser, "-");
        if (is_parsed)
            result_list__add_result(parser->result_list, result__new_operator("-"));
    }
    return is_parsed;
}
bool parser__parse_minussign(parser_t *parser) {
    return parser__parse_symbol(parser, "-");
}
bool parser__parse_colon(parser_t *parser) {
    return parser__parse_symbol(parser, ":");
}
bool parser__parse_openparen(parser_t *parser) {
    return parser__parse_symbol(parser, "(");
}
bool parser__parse_closeparen(parser_t *parser) {
    return parser__parse_symbol(parser, ")");
}
bool parser__parse_double_colon(parser_t *parser) {
    return parser__parse_symbol(parser, "::");
}
bool parser__parse_dot(parser_t *parser) {
    return parser__parse_symbol(parser, ".");
}
bool parser__parse_quote(parser_t *parser) {
    token_t *token = NULL;
    bool is_parsed = false;
    parser__debug_print_enter_function(parser, __func__);
    token = tlc__get_current(parser->tlc);
    is_parsed = (token != NULL && token->type != t_eof);
    if (is_parsed) {
        if (token->type == t_quote) {
            is_parsed = true;
        } else
            is_parsed = false;
    }
    parser__debug_print_exit_function(parser, __func__);
    return is_parsed;
}

void parser__evaluate_datetime_literal(parser_t *parser) {
    unsigned int year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0;
    result_t *result = NULL;

    result = parser__remove_last_result(parser);
    second = result->uint_val;
    
    result = parser__remove_last_result(parser);
    minute = result->uint_val;

    result = parser__remove_last_result(parser);
    hour = result->uint_val;

    result = parser__remove_last_result(parser);
    day = result->uint_val;

    result = parser__remove_last_result(parser);
    month = result->uint_val;
    
    result = parser__remove_last_result(parser);
    year = result->uint_val;

    result_list__add_result(parser->result_list, result__new_datetime(year, month, day, hour, minute, second));
}
void parser__evaluate_date_literal(parser_t *parser) {
    unsigned int year = 0, month = 0, day = 0;
    result_t *result = NULL;

    result = parser__remove_last_result(parser);
    day = result->uint_val;

    result = parser__remove_last_result(parser);
    month = result->uint_val;
    
    result = parser__remove_last_result(parser);
    year = result->uint_val;

    result_list__add_result(parser->result_list, result__new_date(year, month, day));
}


bool parser__parse_symbol(parser_t *parser, const char *symbol) {
    token_t *token = NULL;
    bool is_parsed = false;
    parser__debug_print_enter_function(parser, __func__);
    token = tlc__get_current(parser->tlc);
    is_parsed = (token != NULL && token->type != t_eof);
    if (is_parsed) {
        if (token->type == t_symbol && !strcmp(token->text, symbol)) {
            is_parsed = true;
        } else
            is_parsed = false;
    }
    parser__debug_print_exit_function(parser, __func__);
    return is_parsed;
}
bool parser__parse_unit_identifier(parser_t *parser) {
    bool is_parsed = false;
    token_t *token = NULL;
    parser__debug_print_enter_function(parser, __func__);
    token = tlc__get_current(parser->tlc);
    is_parsed = (token != NULL && token->type != t_eof);
    if (is_parsed) {
        is_parsed = false;
        if (!strcasecmp(token->text, "SECOND") || !strcasecmp(token->text, "SECONDS")) {
            is_parsed = true;
        }
        if (!strcasecmp(token->text, "MINUTE") || !strcasecmp(token->text, "MINUTES")) {
            is_parsed = true;
        }
        if (!strcasecmp(token->text, "HOUR") || !strcasecmp(token->text, "HOURS")) {
            is_parsed = true;
        }
        if (!strcasecmp(token->text, "DAY") || !strcasecmp(token->text, "DAYS")) {
            is_parsed = true;
        }
        if (!strcasecmp(token->text, "WEEK") || !strcasecmp(token->text, "WEEKS")) {
            is_parsed = true;
        }
        if (!strcasecmp(token->text, "MONTH") || !strcasecmp(token->text, "MONTHS")) {
            is_parsed = true;
        }
        if (!strcasecmp(token->text, "YEAR") || !strcasecmp(token->text, "YEARS")) {
            is_parsed = true;
        }
    }
    parser__debug_print_exit_function(parser, __func__);
    return is_parsed;
}
void parser__evaluate_unit_identifier(parser_t *parser) {
    token_t *token = NULL;
    token = tlc__get_current(parser->tlc);
    if (!strcasecmp(token->text, "SECOND") || !strcasecmp(token->text, "SECONDS")) {
        result_list__add_result(parser->result_list, result__new_unit(unit_seconds));
    }
    if (!strcasecmp(token->text, "MINUTE") || !strcasecmp(token->text, "MINUTES")) {
        result_list__add_result(parser->result_list, result__new_unit(unit_minutes));
    }
    if (!strcasecmp(token->text, "HOUR") || !strcasecmp(token->text, "HOURS")) {
        result_list__add_result(parser->result_list, result__new_unit(unit_hours));
    }
    if (!strcasecmp(token->text, "DAY") || !strcasecmp(token->text, "DAYS")) {
        result_list__add_result(parser->result_list, result__new_unit(unit_days));
    }
    if (!strcasecmp(token->text, "WEEK") || !strcasecmp(token->text, "WEEKS")) {
        result_list__add_result(parser->result_list, result__new_unit(unit_weeks));
    }
    if (!strcasecmp(token->text, "MONTH") || !strcasecmp(token->text, "MONTHS")) {
        result_list__add_result(parser->result_list, result__new_unit(unit_months));
    }
    if (!strcasecmp(token->text, "YEAR") || !strcasecmp(token->text, "YEARS")) {
        result_list__add_result(parser->result_list, result__new_unit(unit_years));
    }
}

void parser__evaluate_interval_expression(parser_t *parser) {
    unsigned int count = 0;
    result_t *countresult = NULL;
    result_t *unitresult = NULL;

    unitresult = parser__remove_last_result(parser);
    countresult = parser__remove_last_result(parser);
    count = countresult->uint_val;
    result__delete(countresult);
    unitresult->uint_val = count;
    result_list__add_result(parser->result_list, unitresult);
}

bool parser__parse_type_identifier(parser_t *parser) {
    bool is_parsed = false;
    token_t *token = NULL;
    parser__debug_print_enter_function(parser, __func__);
    token = tlc__get_current(parser->tlc);
    is_parsed = (token != NULL && token->type != t_eof);
    if (is_parsed) {
        is_parsed = false;
        if (!strcasecmp(token->text, "date")) {
            is_parsed = true;
        }
        if (!strcasecmp(token->text, "datetime")) {
            is_parsed = true;
        }
        if (!strcasecmp(token->text, "uint")) {
            is_parsed = true;
        }
    }
    parser__debug_print_exit_function(parser, __func__);
    return is_parsed;
}
void parser__evaluate_type_identifier(parser_t *parser) {
    token_t *token = NULL;
    token = tlc__get_current(parser->tlc);
    
    if (!strcasecmp(token->text, "date")) {
        result_list__add_result(parser->result_list, result__new_type(type_date));
    } else if (!strcasecmp(token->text, "datetime")) {
        result_list__add_result(parser->result_list, result__new_type(type_datetime));
    } else if (!strcasecmp(token->text, "uint")) {
        result_list__add_result(parser->result_list, result__new_type(type_uint));
    } else {
        printf("%s\n", token->text);
        dasl_raise_fatal_error("Something is wrong here.");
    }
}

void parser__evaluate_typed_term(parser_t *parser) {
    result_t *type_result = NULL;
    result_t *value_result = NULL;

    type_result = parser__remove_last_result(parser);
    value_result = parser__remove_last_result(parser);

    if (type_result->type != rt_type) {
        result_list__add_result(parser->result_list, result__new_error(e_generic_error));
    }
    if (type_result->type_val == type_date) {
        if (value_result->type == rt_datetime)
            result_list__add_result(parser->result_list, result__new_date(tm__get_year(value_result->datetime_val), tm__get_month(value_result->datetime_val), tm__get_monthday(value_result->datetime_val)));
        else
            result_list__add_result(parser->result_list, result__new_error(e_cannot_cast_to_date));
    } else if (type_result->type_val == type_datetime) {
        if (value_result->type == rt_date)
            result_list__add_result(parser->result_list, result__new_datetime(tm__get_year(value_result->datetime_val), tm__get_month(value_result->datetime_val), tm__get_monthday(value_result->datetime_val),
                    tm__get_hour(value_result->datetime_val), tm__get_minute(value_result->datetime_val), tm__get_second(value_result->datetime_val)));
        else
            result_list__add_result(parser->result_list, result__new_error(e_cannot_cast_to_datetime));

    } else if (type_result->type_val == type_uint) {
        result_list__add_result(parser->result_list, result__new_error(e_cannot_cast_to_uint));
    } else {
        result_list__add_result(parser->result_list, result__new_error(e_generic_error));
    }
}

bool parser__parse_dotfunction_identifier(parser_t *parser) {
    bool is_parsed = false;
    token_t *token = NULL;
    parser__debug_print_enter_function(parser, __func__);
    token = tlc__get_current(parser->tlc);
    is_parsed = (token != NULL && token->type != t_eof);
    if (is_parsed) {
        is_parsed = false;
        if (!strcasecmp(token->text, "BEGIN")) {
            is_parsed = true;
        }
        if (!strcasecmp(token->text, "END")) {
            is_parsed = true;
        }
    }
    parser__debug_print_exit_function(parser, __func__);
    return is_parsed;
}
void parser__evaluate_dotfunction_identifier(parser_t *parser) {
    token_t *token = NULL;
    token = tlc__get_current(parser->tlc);
    
    if (!strcasecmp(token->text, "BEGIN")) {
        result_list__add_result(parser->result_list, result__new_identifier("BEGIN"));
    } else if (!strcasecmp(token->text, "END")) {
        result_list__add_result(parser->result_list, result__new_identifier("END"));
    } else {
        printf("%s\n", token->text);
        dasl_raise_fatal_error("Something is wrong here.");
    }
}

void parser__evaluate_dotfunction_term(parser_t *parser) {
    result_t *value_result = NULL;
    result_t *ident_result = NULL;
    result_t *unit_result = NULL;
    result_t *result = NULL;
    unsigned int second = 0, minute = 0, hour = 0, day = 0, month = 0, year = 0;

    unit_result = parser__remove_last_result(parser);
    ident_result = parser__remove_last_result(parser);
    value_result = parser__remove_last_result(parser);

    second = tm__get_second(value_result->datetime_val);
    minute = tm__get_minute(value_result->datetime_val);
    hour = tm__get_hour(value_result->datetime_val);
    day = tm__get_monthday(value_result->datetime_val);
    month = tm__get_month(value_result->datetime_val);
    year = tm__get_year(value_result->datetime_val);

    if (!strcasecmp(ident_result->string_val, "BEGIN")) {
        switch (unit_result->unit_val) {
            case unit_years:
                month = 1;
            case unit_months:
                day = 1;
            case unit_days:
                hour = 0;
            case unit_hours:
                minute = 0;
            case unit_minutes:
                second = 0;
                break;
            case unit_seconds:
                dasl_raise_fatal_error("useless!");
                break;
            case unit_weeks:
                dasl_raise_fatal_error("not implemented!");
                break;
            default:
                dasl_raise_fatal_error("unknown unit");
        }
    } else if (!strcasecmp(ident_result->string_val, "END")) {
        switch (unit_result->unit_val) {
            case unit_years:
                month = 12;
            case unit_months:
                day = get_days_of_month(year, month);
            case unit_days:
                hour = 23;
            case unit_hours:
                minute = 59;
            case unit_minutes:
                second = 59;
                break;
            case unit_seconds:
                dasl_raise_fatal_error("useless!");
                break;
            case unit_weeks:
                dasl_raise_fatal_error("not implemented!");
                break;
            default:
                dasl_raise_fatal_error("unknown unit");
        }
    } else {
        dasl_raise_fatal_error("Something is wrong here.");
    }

    if (value_result->type == rt_datetime)
        result = result__new_datetime(year, month, day, hour, minute, second);
    else if (value_result->type ==rt_date)
        result = result__new_date(year, month, day);
    else
        parser__raise_fatal_error("Wrong type for dotfunction");

    
    result_list__add_result(parser->result_list, result);
}
