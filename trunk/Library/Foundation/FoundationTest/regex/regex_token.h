#pragma once



typedef wchar_t	t_char;

typedef enum { INT, OP, CHAR, STR }TOKEN_T;


typedef struct __token_tag
{
		TOKEN_T			type;
		
		union{
				size_t		integer;
				t_char		op_val;
				t_char		char_val;
		};
}token_t;


typedef enum { NO_ERROR, INVALID_TC, INVALID_OP, INVALID_NUM, END_OF_TOKEN}TOK_ERR_T;


typedef struct __lexer_tag
{
		t_char		pattern[4096];
		TOK_ERR_T	last_err;

		t_char		*curr;
		token_t		curr_tok;
}lex_t;


t_bool lex_build(lex_t *lex, const char_type *pattern);


void lexer_destroy(lexer_t *lex);

TOK_ERR_T lexer_last_error(const lexer_t *lex);

t_bool	 lexer_next(lexer_t *lex);

t_bool	 lexer_reset(lexer_t *lex, const char_type *pattern);

const token_t*  lexer_curr_tok(const lexer_t *lex);

t_bool	lexer_match(const lexer_t *lex, const token_t *tok);