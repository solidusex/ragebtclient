
#pragma once

#include "BencodingType.h"


namespace Rage {
/********************************************************************************************************
Class Name: BExtractor
Functional: 提取种子文件信息;
Member Function: 
				size_t analyzer(size_t beg_pos, TypeBase *&pbase);
				size_t read_string(size_t beg_pos, StringType &str_bt);
				size_t read_int(size_t beg_pos, IntegerType &int_bt);
				size_t read_list(size_t beg_pos, ListType &list_bt);
				size_t read_dict(size_t beg_pos, DictType &dict_bt);
				size_t locate_key(size_t beg_pos, const std::string &key);
				返回提取类型在bfile中所占据的长度；失败返回0；
				size_t get_content(size_t b, size_t e, std::string &content);
				返回所提取的content长度 ，失败返回0；
********************************************************************************************************/

class BencodingDecoder  : private NonCopyable
{
private:
		std::vector<t_byte>		bfile;
public:
		size_t analyzer(size_t beg_pos, TypeBase *&pbase);//供给read_list和read_dict递归提取类型
		size_t ReadString(size_t beg_pos, StringType &str_bt);
		//从指定位置开始位置(基于0)
		//将内容加入到一个ListType中,内部调用analyzer函数, 会导致递归调用
		//任何错误都将导致返回 0,直接解析失败
		//如无错误,则返回读取的内容长度,
		size_t ReadInt(size_t beg_pos, IntegerType &int_bt);
		
		size_t ReadList(size_t beg_pos, ListType &list_bt);    
		size_t ReadDict(size_t beg_pos, DictType &dict_bt);
		//根据执行起始位置(此起始位置必须为d开头的dict类型);
		size_t LocateKey(size_t beg_pos, const std::string &key);
		
public:
		 //接受一段元数据作分析 当然是bencoding file, 如果未搜索到第一个d,则返回false;
		bool SetContent(const std::vector<t_byte> &meta); 
		
		bool SetContent(const t_byte* buf, size_t len);

		size_t GetContent(size_t b, size_t e, std::vector<t_byte> &content)const; //根据执行位置提取一段元数据

		const t_byte* GetContent(size_t pos)const;

		void Clear() { bfile.clear(); }
public:
		BencodingDecoder(){};
		~BencodingDecoder(){   };
};


}

