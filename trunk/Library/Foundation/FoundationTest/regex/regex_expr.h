#pragma once
#include "regex_expr_node.h"

/*
2：转义字符：
\^									^
\$									$

\{									{
\}									}
\[									[
\]									]
\(									(
  \)									)
  \?									?
  \+									+
  \-									-
  \*									*
  \|									|
  \\									\
  \<									<
  \>									>

  \n									#10
  \r									#13
  \t									Tab

  \d									[0-9]			【不可用在[]内】
  \D									[^0-9]			【不可用在[]内】
  \s									[ \n\r\t]		【不可用在[]内】
  \S									[^ \n\r\t]		【不可用在[]内】
  \w									[0-9a-zA-Z_]	【不可用在[]内】
  \W									[^0-9a-zA-Z_]	【不可用在[]内】
  \l									[a-zA-Z]		【不可用在[]内】
  \L									[^a-zA-Z]		【不可用在[]内】

  \.									任意字符		【不可用在[]内】


Expr			-> Branch{1,}
Branch			-> (Sequence '|' Sequence)
Sequence		-> Repeat|Simple 
Repeat			-> Simple | Simple ['?', '*', '+'] | Simple '{' __RepeatCount '}'
__RepeatCount	-> digit |digit ','| digit ',' digit
Simple			-> token |  '(' Expr ')'| '(''?' FunctionExpr ')' | '^' | '$'
FunctionExpr	-> __Pattern | :'__Pattern | '='__Pattern | '!'__Pattern
__Pattern		-> Expr | '<' name '>' | '<''#'name'>'Expr | '<''$'name'>'Expr

digit			->[0-9]
token			->[^ digit, ^,$, {,},[,],<,>, (,),?,+,-,*,|,\\, }

*/



#if(0)
Expr			-> SubExpr* | '|' SubExpr
SubExpr			-> Repeat	|  Simple+

Repeat			-> Simple ['?', '*', '+'] | Simple '{' __RepeatCount '}' | Simple '{' __RepeatCount '}' '?'
__RepeatCount	-> digit |digit ','| digit ',' digit


Simple			-> token |  '(' Expr ')'| '(''?' FunctionExpr ')' | '^' | '$'
FunctionExpr	-> __Pattern | :'__Pattern | '='__Pattern | '!'__Pattern
__Pattern		-> Expr | '<' name '>' | '<''#'name'>'Expr | '<''$'name'>'Expr

digit			->[0-9]
token			->[^ digit, ^,$, {,},[,],<,>, (,),?,+,-,*,|,\\, }

 4：功能
	    a|b									匹配a或者b的其中一个
		(?Pattern)				【扩展语法】匹配后获取
		(?:Pattern)							匹配后不获取
		(?=Pattern)				【扩展语法】正向预查，Pattern不能包含扩展语法
		(?!Pattern)				【扩展语法】负向预查，Pattern不能包含扩展语法
		其中Pattern可以是
		  表达式							普通表达式
		  <name>表达式						为表达式命名
		  <name>							如果name曾经被用来命名，则取命名的表达式，【不可递归】
		  <#name>表达式			【扩展语法】匹配后存放进name号存储区
		  <$name>				【扩展语法】匹配字符串是否等于name号存储区的某个记录，如果打开StorageAutoReplace则存储区只存放一个记录
#endif




