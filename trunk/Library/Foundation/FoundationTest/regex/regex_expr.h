#pragma once
#include "regex_expr_node.h"

/*
2��ת���ַ���
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

  \d									[0-9]			����������[]�ڡ�
  \D									[^0-9]			����������[]�ڡ�
  \s									[ \n\r\t]		����������[]�ڡ�
  \S									[^ \n\r\t]		����������[]�ڡ�
  \w									[0-9a-zA-Z_]	����������[]�ڡ�
  \W									[^0-9a-zA-Z_]	����������[]�ڡ�
  \l									[a-zA-Z]		����������[]�ڡ�
  \L									[^a-zA-Z]		����������[]�ڡ�

  \.									�����ַ�		����������[]�ڡ�


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

 4������
	    a|b									ƥ��a����b������һ��
		(?Pattern)				����չ�﷨��ƥ����ȡ
		(?:Pattern)							ƥ��󲻻�ȡ
		(?=Pattern)				����չ�﷨������Ԥ�飬Pattern���ܰ�����չ�﷨
		(?!Pattern)				����չ�﷨������Ԥ�飬Pattern���ܰ�����չ�﷨
		����Pattern������
		  ���ʽ							��ͨ���ʽ
		  <name>���ʽ						Ϊ���ʽ����
		  <name>							���name������������������ȡ�����ı��ʽ�������ɵݹ顿
		  <#name>���ʽ			����չ�﷨��ƥ����Ž�name�Ŵ洢��
		  <$name>				����չ�﷨��ƥ���ַ����Ƿ����name�Ŵ洢����ĳ����¼�������StorageAutoReplace��洢��ֻ���һ����¼
#endif




