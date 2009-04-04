#include <assert.h>

#include "global.h"


/*



factor := INTEGER | FLOAT | NAME | NAME =  (expr) | -factor | (expr)
term   := term * factor | term / factor | factor
expr   := expr + term | expr - term 
assign := (expr)

start := expr | ¦Å

*/


static void expr();
static void term();
static void factor();
static void assign_expr();

static bool match(TOKEN_T type);

static token_t *lookahead = NULL;



void parse(token_t *head)
{
		lookahead = head;
		assert(head != NULL);
		

		while(lookahead != NULL && lookahead->type != END)
		{
				expr();
				if(!match(PRINT))calc_error("synax error : can't match ')'\n", lookahead->coord.x);
		}
}








static bool match(TOKEN_T type)
{
		if(lookahead->type == type)
		{
				lookahead = lookahead->next;
				return true;
		}

		return false;
}

static void expr()
{


}



