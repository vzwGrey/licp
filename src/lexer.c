#include "lexer.h"
#include <ctype.h>
#include <stdbool.h>

#define CURRENT_CHAR(state) ((state)->source[(state)->pos])

static bool is_ident_char(char c);
static token get_number_token(lexer_state *state);
static token get_ident_token(lexer_state *state);

lexer_state make_lexer(const char *source)
{
    return (lexer_state){
        .source = source,
        .pos = 0,
    };
}

static token make_token(token_kind kind, const char *source, size_t len)
{
    return (token){
        .kind = kind,
        .source = source,
        .len = len,
    };
}

token next_token(lexer_state *state)
{
    while (CURRENT_CHAR(state) != 0 && isspace(CURRENT_CHAR(state)))
        state->pos++;

    if (CURRENT_CHAR(state) == 0)
        return make_token(TOKEN_EOF, state->source + state->pos, 0);

    switch (CURRENT_CHAR(state))
    {
        case '(': state->pos++; return make_token(TOKEN_OPAREN, state->source + state->pos - 1, 1);
        case ')': state->pos++; return make_token(TOKEN_CPAREN, state->source + state->pos - 1, 1);
    }

    if (isdigit(CURRENT_CHAR(state)))
        return get_number_token(state);

    if (is_ident_char(CURRENT_CHAR(state)))
        return get_ident_token(state);

    return make_token(TOKEN_UNKNOWN, state->source + state->pos++, 1);
}

static token get_number_token(lexer_state *state)
{
    size_t start = state->pos;
    while (isdigit(CURRENT_CHAR(state)))
        state->pos++;
    return make_token(TOKEN_NUMBER, state->source + start, state->pos - start);
}

static bool is_ident_char(char c)
{
    return c != '(' && c != ')' && !isspace(c) && isprint(c);
}

static token get_ident_token(lexer_state *state)
{
    size_t start = state->pos;
    while (is_ident_char(CURRENT_CHAR(state)))
        state->pos++;
    return make_token(TOKEN_IDENT, state->source + start, state->pos - start);
}
