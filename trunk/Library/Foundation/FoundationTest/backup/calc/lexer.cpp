#include "global.h"
#include <ctype.h>
#include <assert.h>
#include <malloc.h>
#include <string.h>

token_t* alloc_token(const char *value, TOKEN_T type, size_t x, size_t y)
{
		token_t *res = NULL;
		assert(value != NULL);
		res = (token_t*)malloc(sizeof(token_t));
		strcpy(res->value, value);
		res->type = type;
		res->coord.x = x;
		res->coord.y = y;
		res->next = NULL;
		return res;
}


token_t* free_token(token_t *token)
{
		token_t *res = token != NULL ? token->next : NULL;
		if(token != NULL)free(token);
		return res;
}

#define __INSERT_TOKEN(v,t,x,y)do{ token_t *tmp = alloc_token(v, t, x, y); if(head == NULL){head = tmp; last = tmp;}else{last->next = tmp; last = last->next;}}while(0)


token_t* lexan(const char *str)
{
		token_t *head = NULL;
		token_t *last = NULL;
		token_t *tmp = NULL;
		char buf[1024];

		const char *pstr = NULL;
		coord_t	coord;

		assert(str != NULL);
		pstr = str;
		coord.x = 1; coord.y = 1;
		
		while(*pstr != '\0')
		{
				if(*pstr == ' ' || *pstr == '\t' || *pstr == '\n' || *pstr == 'r')
				{
						coord.y++;
						if(*pstr == '\n'){coord.x++; coord.y = 0;}
						pstr++;
				}else if(isdigit((int)*pstr) || *pstr == '.')
				{
						int is_float = 0;
						size_t n = 0;
						char *beg = buf;
						TOKEN_T type;
						while(isdigit(*pstr) || *pstr == '.')
						{
								if(*pstr == '.')
								{
										if(is_float == 1)calc_error("synax error", coord.x);
										if(beg == buf)*beg++ = '0';
										is_float = 1;
								}
								*beg++ = *pstr++; n++;
						}
						
						if(*(beg-1) == '.')*beg++ = '0';
						*beg = 0;
						
						type = is_float == 1 ? FLOAT : INTEGER;
						__INSERT_TOKEN(buf, type, coord.x, coord.y);
						coord.y += n;

				}else if(isalpha((int)*pstr))
				{
						size_t n = 0;
						char *beg = buf;
						while(isalnum(*pstr)){ *beg++ = *pstr++; n++;}*beg = 0;
						__INSERT_TOKEN(buf, NAME, coord.x, coord.y);
						coord.y += n;

				}else 
				{
						switch(*pstr)
						{
						case PLUS:
						case MINUS:
						case MUL:
						case DIV:
						case MOD:
						case ASSIGN:
						case DOT:
						case LEFT_P:
						case RIGHT_P:
						case PRINT:
						case COMMA:
						{
								char *beg = buf; *beg++ = *pstr; *beg = 0;
								__INSERT_TOKEN(buf, NAME, coord.x, coord.y);
								break;
						}
						default:
								printf("C == %c\n", *pstr);
						}

						coord.y++;
						pstr++;
				}
		}

		__INSERT_TOKEN("", END, coord.x, coord.y);


		return head;
}