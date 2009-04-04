
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



/*自顶向下的矩阵链乘法最优解*/

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

这里假设一共只有五种状态0->4,字母表T为所有ASCII所有符号

相邻的行或者列有>= k个值相同的时候则提取k = 3;

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
repeat_re ---> simple_re [’*’|’+’|’?’]
simple_re ---> token | ’(’ regular_expression ’)’

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
						从Regex到NFA(1):递归下降
		大多数时候，我们仅仅听懂了几个名词就以为掌握了一切，可现实是花费很多时间却连门都摸不到，比如我和我的这个题目。大概三年前的10月份，我刚学编程时候就写（也可以认为是抄袭）
过一个表达式分析器，当时是用双堆栈+操作符优先级完成的，大概就类似：


typedef enum
{
		BEG = 0, END = 1, RP = 2, ADD = 3, SUB = 4, MUL = 5, DIV = 6, MOD = 7, LP = 999
}operator_t;

        一个operand stack（以下简称num_stk，一个operator_stack(以下简称optr_stk), 每遇到一个操作数就push到num_stk,遇到一个操作符就检查optr_stk的stack_top，为空或者优先级低于
当前optr就将当前的push进去，否则弹出stack_top和两个操作数，执行计算后得到的结果当作新操作数push到num_stk里，一个例外就是左括号有用最高优先级，但是要push进去一个最低的符号，比如
BEG,这里意味着一个新的表达式开始，知道遇到RP将新表达式彻底替换成操作数，push进去，如此反复，最后得到的一个num_stk的top就是结果。大概就是这么个东西吧。

		大部分教程都会说这个比用递归下降要简单的多，其实不然，之所以认为这玩意儿简单其实是因为不少新手觉得递归复杂，其实递归才是真正简单明了的，因为递归是符合人类逻辑的，而堆栈不是。
关于什么是现实的问题我倒觉得很有意思，因为看到不少人都在说要根据现实编程，把问题抽象成现实的问题等等，我觉得这句话没错，但是容易误导人，因为对于程序来说现实的就应该意味着符合人类常识和逻辑
的，而不是说如何把3P的分工用class划分的非常清楚，比如树是现实的而链表不是，因为广义的树树符合人类做决策时候的逻辑，而链表仅仅是个抽象的容器，类似的图也是现实的而堆栈不是，可以认为广义的树
或者图天生就在那里，而堆栈和链表只不过是操纵它们的手段而已。有些看似NB的东西其实仅仅是需要一点信心而已，仅仅是一点点信心，比如下面的这类互斥递归(mutual recursive).

		还是回到那个遥远的表达式分析吧，先看看四则混合运算的定义吧,恩，首先得有数，数的组成这里为了简单只考虑整数，也就是[0-9]，之后有操作符，[+-* /()%]等等吧，最后把这些东西赋予规则
就成了表达式，例如1+2*3  = 1 + 6 = 7等等，一些傻子都会的东西，OK，下面可以考虑下怎么用程序描述了，
考虑这么一个表达式：

“ (- (2 * 3)) * (388  % -64 - (1))”

这里先假设这个表达式已经被分解为一组符号了，例如( 为LP -为SUB等等），按照常识（人类的逻辑）应该是先算括号里面的，每个括号构成了一个新的子表达式，而一个中间不含有括号的子表达式
又要从左到右先算* / %而后计算+-，那么现在考虑最基本的子表达式构成，我们暂时叫它factor
factor一定会是不同的数，例如2,3,388,64或者-64等等，  如果这里把这个子表达式之中带括号的也当作最基本的一个数看，那么factor还会存在expr(expr代表一个完整的表达式）
那么就有了

num = '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9';

factor = -factor | (expr) | num

OK，这里factor不考虑任何运算，只考虑最基本的正负数或者另一个子表达式（或由-引导的子表达式，要对子表达式值取负），那么expr是什么呢？当然是各个factor的运算了

expr = factor + expr | factor - expr |factor * expr | factor / expr | factor % expr |factor

现在看看我们列的这个东西是什么意思呢？没错，就是一个四则混合运算的模式，或者叫产生是，也就是如果输入的一个字符串不符合这个东西，那么就是非法的，比如一个factor代表的字符串一定会跟着
+-* / %或者空，否则就是非法的，注意，这里判定的是第一个问题，就是，输入是不是规定的语言。

下面看第二个问题，运算规则，如上面所说，运算次序是预先知道的，那么这里括号(expr)已经被认为是一个类似388的单一元素了，所以括号所表示的优先级自然是最高的，那么+-*%/该如何表示呢
因为*%/的优先级相同，因此，将其当作一个单独的产生式处理，就有了term

term   = factor* term | factor / term | factor % term | factor

这里term的意思就是要么替换成factor到最基本的num，要么就连续的与下一个term（最终也被替换成num),做*%/，当检测到下一个输入不为*%/时候，则返回上一层，


现在来看看最后的expr是什么，因为num和*%/都被处理了，则，expr就变成了通过+ -连接term的表达式了，则有

expr   = term + expr | term - expr | expr

同term一样，expr被表示为最左面有一个term引导的 (term + | term - ) expr，这样，最终调用一个expr就可以得到此表达式的值了，也许这玩意儿说起来有点麻烦，但是看看代码就有容易多了。


先把词法分析列出来：


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


