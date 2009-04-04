
#include "Foundation.h"
#pragma comment(lib, "Foundation.lib")


//#include "global.h"

#include <assert.h>

#include <stack>

#include <iostream>
using namespace std;


#if(0)

int main()
{


		FILE *file = fopen(".\\calc\\text.txt", "r");
		if(file == NULL)exit(-1);
		char c;
		std::string src;
		while(ferror(file) == 0)
		{
				c = fgetc(file);
				if(c == EOF)break;
				src.push_back(c);
		}
		

		fclose(file);
		
		
		token_t *token_list = lexan(src.c_str());

		assert(token_list != NULL);
#if(0)
		while(token_list != NULL)
		{
				printf("value == %s line == %d, num == %d\n", token_list->value, token_list->coord.x, token_list->coord.y);
				token_list = token_list->next;

		}
		
		
#endif

		
		printf("done\n");
		cin.get();

		while(token_list != NULL){token_list = free_token(token_list);}


		
		return 0;

}



/*�Զ����µľ������˷����Ž�*/

#define N (6) 
#define MAX_UINT (~(size_t)0)
struct mvec_t{ 
  size_t row; 
  size_t col; 
}mat_vec[N] = 
{ 
  30, 35, 
  35, 15, 
  15, 5, 
  5,  10, 
  10, 20, 
  20, 25 
}; 
typedef mvec_t mvec_t; 
size_t opt_table[N][N]; 
size_t parens_table[N][N]; 

void print_n( size_t arr[N][N]) 
{ 
  for(size_t i = 0; i < N; ++i) 
  { 
    for(size_t j = 0; j < N; ++j) 
    { 
      printf("%u        ", arr[i][j]); 
    } 
    printf("\n"); 
  } 
  printf("\n"); 
} 

size_t __lookup(size_t i, size_t j)
{
		size_t q = opt_table[i][j];

		if(q < MAX_UINT)return q;
		
		
		if(i == j)
		{
				opt_table[i][j] = 0;
		}else
		{
				for(size_t k = i; k < j ; ++k)
				{
						q = __lookup(i,k) +  __lookup(k + 1, j) 
								+ mat_vec[i].row * mat_vec[k + 1].row * mat_vec[j].col;

						if(q < opt_table[i][j]) {opt_table[i][j] = q; }
				}

		}
		
		return opt_table[i][j];

}

void memozied_chain_order()
{
		memset(opt_table, 0, N*N*sizeof(size_t));

		for(size_t i = 0; i < N; ++i)
		{
				for(size_t j = i; j < N; ++j)
				{
						opt_table[i][j] = MAX_UINT;

				}

		}

		__lookup(0,N - 1);


}




int main()
{


		memozied_chain_order();

		print_n(opt_table);



		printf("done\n");
		cin.get();
		return 0;
}


#endif


typedef struct __range_tag
{
		int		beg;
		int		end;

}range_t;

typedef struct __range_table_tag
{
		int		index[sizeof(char)];
		range_t	*set;
}range_table_t;

/*

�������һ��ֻ������״̬0->4,��ĸ��TΪ����ASCII���з���

���ڵ��л�������>= k��ֵ��ͬ��ʱ������ȡk = 3;

*/
const size_t K = 3;

int sour_table[5][sizeof(char)];


range_table_t rl_col;
range_table_t rl_row;

int			  **remain;


void init_sour() { memset(sour_table, -1, sizeof(char) * 5);}

void init_gen_col()
{

		size_t x,y, count;
		
		rl_col.set = new range_t[sizeof(char)];

		for(y = 0; y < sizeof(char); ++y)
		{
				rl_col.index[y] = y;
				
				count = 0;
				
				int prev = sour_table[0][y];

				for(x = 1; x < 5; ++x)
				{
						if(sour_table[x][y] == prev)
						{
								count++;
						}else
						{
								if(count >= K)
								{

								}
						}

				}

		}




}



/*

	state | e n d l s
----------------------------------------
        0 | 1 7 7 7 7
        1 | 7 2 7 2 7
        2 | 7 7 3 7 7
        3 | 7 7 7 7 7
        4 | 7 7 7 7 5
        5 | 6 7 7 7 7
        6 | 7 7 7 7 7
        7 | 7 7 7 7 7


token state default base index next check
----------------------------------------------------------------------
IDENT   0       3     5    0     7     3
IDENT 	1		3	  5	   1	 7	   3
IDENT	2		3	  8	   2     7	   3
END		3		X	  0	   3     7	   3
IDENT	4		3	  5    4     7     3
IDENT	5		3	  7	   5	 1	   0
ELSE	6		3	  X    6	 2	   1
IDENT	7		3	  X	   7     6     5

*/



/*

regular_expressions ---> compound_re*
compound_re ---> repeat_re | simple_re
repeat_re ---> simple_re [��*��|��+��|��?��]
simple_re ---> token | ��(�� regular_expression ��)��

*/

namespace {

typedef enum
{
		BEG = 0, END = 1, RP = 2, ADD = 3, SUB = 4, MUL = 5, DIV = 6, MOD = 7, LP = 999
}operator_t;

bool is_operator(const char *str)
{
		char c = *str;

		if(c == '(' || c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == ')')return true;
		return false;
}

typedef struct __lex_tag
{
		char			buf[4096];
		const char		*curr;
}lex_t;



lex_t create_lex(const char *expr)
{
		lex_t lex;
		strcpy(lex.buf, expr);
		lex.curr = lex.buf;

		while(*lex.curr != 0 && *lex.curr == ' ')lex.curr++;

		return lex;
}

typedef enum {OP, NUM }TokenT;

typedef struct __token_tag
{
		TokenT	type;
		int		val;
}token_t;


token_t get_token(lex_t *lex)
{
		token_t res;
		const char *p = lex->curr;
		while(*p != 0 && *p == ' ')p++;
		
		if(*p == 0)
		{
				res.type = OP;
				res.val = END;
		}else if(is_operator(p))
		{
				res.type = OP;
				switch(*p)
				{
				case '(':
						res.val = LP;
						break;
				case '+':
						res.val = ADD;
						break;
				case '-':
						res.val = SUB;
						break;
				case '*':
						res.val = MUL;
						break;
				case '/':
						res.val = DIV;
						break;
				case '%':
						res.val = MOD;
						break;
				case ')':
						res.val = RP;
						break;
				default:
						assert(0);
				}
				p++;
		}else
		{
				res.type = NUM;
				res.val = 0;

				while(*p != 0 && *p >= '0' && *p <= '9')
				{
						res.val *= 10;
						res.val += *p - '0';
						p++;
				}

				if(*p != 0 && *p != ' ' && !is_operator(p))
				{
						printf("%s\n", p);
						assert(0);
				}
		}
		
		lex->curr = p;
		return res;

}

#if(0)
typedef struct _ctx_tag
{
		std::stack<int> operator_stack;
		std::stack<int>	operand_stack;
		bool is_end;
}ctx_t;

void init_ctx(ctx_t *ctx)
{
		ctx->is_end = false;
		while(!ctx->operand_stack.empty())ctx->operand_stack.pop();
		while(!ctx->operator_stack.empty())ctx->operator_stack.pop();
		ctx->operator_stack.push(BEG);
}

int __calc_val(int op, int l, int r)
{
		switch(op)
		{
		case ADD:
				return l + r;
		case SUB:
				return l - r;
		case MUL:
				return l * r;
		case DIV:
				return l / r;
		case MOD:
				return l % r;
		default:
				printf("op == %d l == %d r == %d\n", op, l, r);
				assert(0);
		}
		return 0;
}


void __push_num(ctx_t *ctx, int val)
{
		ctx->operand_stack.push(val);
}


int __top_num(ctx_t *ctx)
{
		return ctx->operand_stack.top();
}


int __pop_num(ctx_t *ctx)
{
		int res = __top_num(ctx);
		ctx->operand_stack.pop();
		return res;
}

bool __is_empty_num(ctx_t *ctx)
{
		return ctx->operand_stack.size() == 0;
}

void __push_op(ctx_t *ctx, int op)
{
		ctx->operator_stack.push(op);
}

int __top_op(ctx_t *ctx)
{
		return ctx->operator_stack.top();
}

int __pop_op(ctx_t *ctx)
{
		int op = __top_op(ctx);
		ctx->operator_stack.pop();
		return op;
}

bool __is_empty_op(ctx_t *ctx)
{
		return ctx->operator_stack.size() == 0;
}

//"(0 - (2 * 3)) * (388  % (0-64))"

void __push_operator(ctx_t *ctx, int optr)
{
		while(!__is_empty_op(ctx))
		{
				printf("optr == %d\n", optr);
				
				int op = __top_op(ctx);

				if(optr > op)
				{
						if(optr == LP)optr = BEG;
						__push_op(ctx, optr);
						return;
				}else
				{
						op = __pop_op(ctx);
								
						while(op >= optr)
						{
								printf("op == %d\n", op);
								int r = __pop_num(ctx);
								int l = __pop_num(ctx);
								int res = __calc_val(op, l, r);
								__push_num(ctx, res);
								op = __pop_op(ctx);
								
								printf("res == %d\n", res);
								printf("oprand size == %d\n", ctx->operand_stack.size());
						}
						
						if(optr == RP || optr == END)
						{
								printf("op == %d\n", op);
								assert(op == BEG);
								printf("match\n");
								return;
						}
				}
		}
		
		ctx->is_end = true;
}

int calc_val(const char *expr)
{
		
		int result = 0;		
		lex_t lex = create_lex(expr);
		ctx_t ctx;
		init_ctx(&ctx);

		while(!ctx.is_end)
		{
				token_t t = get_token(&lex);
				if(t.type == OP)
				{
						__push_operator(&ctx, t.val);

				}else
				{
						__push_num(&ctx, t.val);
				}
		}
		assert(ctx.operand_stack.size() == 1);
		return ctx.operand_stack.top();

}
#endif


/*
						��Regex��NFA(1):�ݹ��½�
		�����ʱ�����ǽ��������˼������ʾ���Ϊ������һ�У�����ʵ�ǻ��Ѻܶ�ʱ��ȴ���Ŷ��������������Һ��ҵ������Ŀ���������ǰ��10�·ݣ��Ҹ�ѧ���ʱ���д��Ҳ������Ϊ�ǳ�Ϯ��
��һ�����ʽ����������ʱ����˫��ջ+���������ȼ���ɵģ���ž����ƣ�


typedef enum
{
		BEG = 0, END = 1, RP = 2, ADD = 3, SUB = 4, MUL = 5, DIV = 6, MOD = 7, LP = 999
}operator_t;

        һ��operand stack�����¼��num_stk��һ��operator_stack(���¼��optr_stk), ÿ����һ����������push��num_stk,����һ���������ͼ��optr_stk��stack_top��Ϊ�ջ������ȼ�����
��ǰoptr�ͽ���ǰ��push��ȥ�����򵯳�stack_top��������������ִ�м����õ��Ľ�������²�����push��num_stk�һ�������������������������ȼ�������Ҫpush��ȥһ����͵ķ��ţ�����
BEG,������ζ��һ���µı��ʽ��ʼ��֪������RP���±��ʽ�����滻�ɲ�������push��ȥ����˷��������õ���һ��num_stk��top���ǽ������ž�����ô�������ɡ�

		�󲿷̶ֽ̳���˵������õݹ��½�Ҫ�򵥵Ķ࣬��ʵ��Ȼ��֮������Ϊ�����������ʵ����Ϊ�������־��õݹ鸴�ӣ���ʵ�ݹ�������������˵ģ���Ϊ�ݹ��Ƿ��������߼��ģ�����ջ���ǡ�
����ʲô����ʵ�������ҵ����ú�����˼����Ϊ���������˶���˵Ҫ������ʵ��̣�������������ʵ������ȵȣ��Ҿ�����仰û�������������ˣ���Ϊ���ڳ�����˵��ʵ�ľ�Ӧ����ζ�ŷ������ೣʶ���߼�
�ģ�������˵��ΰ�3P�ķֹ���class���ֵķǳ����������������ʵ�Ķ������ǣ���Ϊ�����������������������ʱ����߼�������������Ǹ���������������Ƶ�ͼҲ����ʵ�Ķ���ջ���ǣ�������Ϊ�������
����ͼ���������������ջ������ֻ�����ǲ������ǵ��ֶζ��ѡ���Щ����NB�Ķ�����ʵ��������Ҫһ�����Ķ��ѣ�������һ������ģ�������������໥��ݹ�(mutual recursive).

		���ǻص��Ǹ�ңԶ�ı��ʽ�����ɣ��ȿ�������������Ķ����,�������ȵ������������������Ϊ�˼�ֻ����������Ҳ����[0-9]��֮���в�������[+-* /()%]�ȵȰɣ�������Щ�����������
�ͳ��˱��ʽ������1+2*3  = 1 + 6 = 7�ȵȣ�һЩɵ�Ӷ���Ķ�����OK��������Կ�������ô�ó��������ˣ�
������ôһ�����ʽ��

�� (- (2 * 3)) * (388  % -64 - (1))��

�����ȼ���������ʽ�Ѿ����ֽ�Ϊһ������ˣ�����( ΪLP -ΪSUB�ȵȣ������ճ�ʶ��������߼���Ӧ����������������ģ�ÿ�����Ź�����һ���µ��ӱ��ʽ����һ���м䲻�������ŵ��ӱ��ʽ
��Ҫ����������* / %�������+-����ô���ڿ�����������ӱ��ʽ���ɣ�������ʱ����factor
factorһ�����ǲ�ͬ����������2,3,388,64����-64�ȵȣ�  ������������ӱ��ʽ֮�д����ŵ�Ҳ�����������һ����������ôfactor�������expr(expr����һ�������ı��ʽ��
��ô������

num = '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9';

factor = -factor | (expr) | num

OK������factor�������κ����㣬ֻ�����������������������һ���ӱ��ʽ������-�������ӱ��ʽ��Ҫ���ӱ��ʽֵȡ��������ôexpr��ʲô�أ���Ȼ�Ǹ���factor��������

expr = factor + expr | factor - expr |factor * expr | factor / expr | factor % expr |factor

���ڿ��������е����������ʲô��˼�أ�û������һ�������������ģʽ�����߽в����ǣ�Ҳ������������һ���ַ��������������������ô���ǷǷ��ģ�����һ��factor������ַ���һ�������
+-* / %���߿գ�������ǷǷ��ģ�ע�⣬�����ж����ǵ�һ�����⣬���ǣ������ǲ��ǹ涨�����ԡ�

���濴�ڶ������⣬���������������˵�����������Ԥ��֪���ģ���ô��������(expr)�Ѿ�����Ϊ��һ������388�ĵ�һԪ���ˣ�������������ʾ�����ȼ���Ȼ����ߵģ���ô+-*%/����α�ʾ��
��Ϊ*%/�����ȼ���ͬ����ˣ����䵱��һ�������Ĳ���ʽ����������term

term   = factor* term | factor / term | factor % term | factor

����term����˼����Ҫô�滻��factor���������num��Ҫô������������һ��term������Ҳ���滻��num),��*%/������⵽��һ�����벻Ϊ*%/ʱ���򷵻���һ�㣬


��������������expr��ʲô����Ϊnum��*%/���������ˣ���expr�ͱ����ͨ��+ -����term�ı��ʽ�ˣ�����

expr   = term + expr | term - expr | expr

ͬtermһ����expr����ʾΪ��������һ��term������ (term + | term - ) expr�����������յ���һ��expr�Ϳ��Եõ��˱��ʽ��ֵ�ˣ�Ҳ���������˵�����е��鷳�����ǿ�������������׶��ˡ�


�ȰѴʷ������г�����


*/



int factor(lex_t *lex);
int term(lex_t *lex);
int expr(lex_t   *lex);
static token_t lookahead;


void match(token_t tok, lex_t *lex)
{
		if(tok.type != lookahead.type || tok.val != lookahead.val)
		{
				assert(0);
		}else
		{
				lookahead = get_token(lex);
		}

}

int expr(lex_t *lex)
{
		int val = term(lex);

		while(true)
		{
				assert(lookahead.type == OP);
				switch(lookahead.val)
				{
				case ADD:
						match(lookahead, lex);
						val += expr(lex);
						break;
				case SUB:
						match(lookahead, lex);
						val -= expr(lex);
						break;
				default:
						//return neg ? -val : val;
						return val;
				}
		}
}


int term(lex_t *lex)
{
		int val = factor(lex);
		while(true)
		{
				assert(lookahead.type == OP);
				switch(lookahead.val)
				{
				case MUL:
						match(lookahead, lex);
						val *= term(lex);
						break;
				case DIV:
						match(lookahead, lex);
						val /= term(lex);
						break;
				case MOD:
						match(lookahead, lex);
						val %= term(lex);
						break;
				default:
						return val;
				}

		}

		return val;
}


int factor(lex_t *lex)
{
		int val = 0;
		token_t tok;
		if(lookahead.type == OP)
		{
				tok.type = OP;

				if(lookahead.val == LP)
				{
						tok.val = LP;
						match(tok, lex);
						val = expr(lex);
						tok.val = RP;
						match(tok, lex);
				}else
				{
						tok.val = SUB;
						match(tok, lex);
						val = -factor(lex);
				}
		}else
		{
				tok.type = NUM;
				val = lookahead.val;
				tok.val = val;
				match(tok, lex);
		}

		return val;
}






int calc_val(const char *str)
{
		lex_t lex = create_lex(str);
		
		lookahead = get_token(&lex);
		
		return expr(&lex);
		
}


#if(0)
//"-3 * -4 + - 56 / 8"


void __push_operator(ctx_t *ctx, int optr)
{
		if(optr == END && ctx->operator_stack.empty())
		{
				ctx->is_end = true;
				return;
		}

		if(ctx->operator_stack.empty() || ctx->operator_stack.top() < optr)
		{
				if(optr == LP)optr = END;
				ctx->operator_stack.push(optr);
		}else
		{
				int op = ctx->operator_stack.top();
				while(!ctx->operator_stack.empty() && op >= optr)
				{
						ctx->operator_stack.pop();
						int r = __pop_operand(ctx),l = __pop_operand(ctx);

						int res = __calc_val(op, l, r);
						__push_operand(ctx, res);
						if(!ctx->operator_stack.empty())op = ctx->operator_stack.top(); 
						
				}

				if(optr == RP)
				{
						assert(op == END);
						printf("match\n");
						ctx->operator_stack.pop();
				}else if(optr == END)
				{
						printf("optr == END\n");
						assert(ctx->operator_stack.empty());
						ctx->is_end = true;
				}else
				{
						ctx->operator_stack.push(optr);
				}
		}
}
#endif



}

int main()
{
		cout << (- (2 * 3)) * -(388  % -64 - (1)) << endl;
		
		cout << calc_val("(- (2 * 3)) * -(388  % -64 - (1)) ") << endl;

		//cout << calc_val("-30") << endl;

		printf("done\n");
		cin.get();

		return 0;
}


