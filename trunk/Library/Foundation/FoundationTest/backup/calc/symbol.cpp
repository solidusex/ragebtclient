
#include "global.h"


#include <malloc.h>
#include <string.h>

#include <map>
#include <string>
#include <cassert>

typedef std::map<std::string, symbol_t*>			symbol_table_t;

static symbol_table_t	__table;






static symbol_t*		alloc_symbol(const char *name, const token_t *token)
{
		symbol_t *res = NULL;
		assert(name != NULL);
		res = (symbol_t*)malloc(sizeof(symbol_t));
		memset(res, 0, sizeof(symbol_t));
		
		if(name != NULL)strcpy(res->name, name);
		if(token != NULL)
		{
				res->src = token->coord;
				res->type = token->type;
				if(res->type == INTEGER)
				{
						sscanf(token->value, "%g", res->num_f);
				}else if(res->type == FLOAT)
				{
						sscanf(token->value, "%I64d", res->num_i);
				}
		}
		
		return res;

}

static void			free_symbol(symbol_t *ptr)
{
		if(ptr != NULL)free(ptr);
		

}

static symbol_t*				copy_symbol(const symbol_t *self)
{
		symbol_t *res = NULL;
		assert(self != NULL);

		res =  alloc_symbol(NULL,NULL);
		memcpy(res, self, sizeof(symbol_t));
		return res;
}


bool			insert_symbol(const char *key, const symbol_t *s)
{
		std::string str;
		assert(key != NULL && s != NULL);
		
		str = key;

		if(__table.count(str) == 1)return false;
		__table[str] = copy_symbol(s);
		return true;
}


symbol_t*		lookup(const char *key)
{
		
		std::string str;
		assert(key != NULL);

		str = key;

		symbol_table_t::iterator it = __table.find(str);
		if(it != __table.end())
		{
				return it->second;
		}

		return NULL;

}
