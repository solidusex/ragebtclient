#ifndef CALC_GLOBAL_H__
#define CALC_GLOBAL_H__

#include <stdio.h>
#include <stdlib.h>


typedef enum 
{
		NAME,			INTEGER,		FLOAT,	END, 
		PLUS = '+',		MINUS = '-',	MUL = '*', 
		DIV = '/',		MOD   = '%',    ASSIGN = '=',	
		DOT = '.',		LEFT_P = '(',	RIGHT_P = ')',
		PRINT = ';',	COMMA  = ',',	NONE = -1
}TOKEN_T;




void	calc_error(const char *str, size_t line);




typedef struct __coord_tag
{
		size_t	x;
		size_t	y;
}coord_t;


typedef struct __token_tag
{
		char			value[1024];
		TOKEN_T			type;
		coord_t			coord;
		__token_tag		*next;
}token_t;

token_t* alloc_token(const char *value, TOKEN_T type, size_t x, size_t y);

token_t* free_token(token_t *token);

token_t* lexan(const char *str);








typedef struct __symbol_tag
{
		char			name[1024];
		coord_t			src;
		TOKEN_T			type;
		union{
				__int64		num_i;
				double		num_f;
		};

}symbol_t;




bool			insert_symbol(const char *key, const symbol_t *s);
symbol_t*		lookup(const char *key);



void emit(const symbol_t *symbol);




















#endif

