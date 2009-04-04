#pragma once

#define BUFFER_LEN 255 

namespace leiXure 
{ 
		struct IgnoreCaseLT 
		{ 
				bool operator()(const std::string& lhs, const std::string& rhs) const 
				{ 
						return stricmp(lhs.c_str(), rhs.c_str()) < 0; 
				} 
		}; 


class IniParser 
{ 
public: 
 typedef std::map<std::string, std::string, IgnoreCaseLT> KeyMap; 
 typedef std::map<std::string, KeyMap,      IgnoreCaseLT> SectionMap; 
 typedef KeyMap::iterator     KeyIterator; 
 typedef SectionMap::iterator SectionIterator; 
public: 
 
 // default constructor, destructor. 
 IniParser(); 
 ~IniParser(); 
 
 // construct and load from the file. 
 IniParser(const char* file_name); 
 
 // load an ini file, if the previous ini file is modified, 
 // the modification would be saved. 
 bool load(const char* file_name); 
 
 // load a C string as ini. 
 bool loadString(const char* str, unsigned length); 
 
 // save the ini to the file from which it was loaded. 
 bool save(); 
  // save the ini in a file with a name that is different 
 // from which it was loaded. 
 bool saveAs(const char* file_name); 
 // return whether the ini is modified since it's loaded 
 // or its last saving. 
 bool isModified() const { return m_modified; } 
public: // high level member function. 
 // the following member functions get the value from 
 // the section-key entry, interpret it as the specific 
 // type. def_val, if any, will be used if the 
 // section-key entry is absent, or if error occurs, 
 // such as using getInteger from an entry while the 
 // value could not be converted to integer. 
 
 // return as integer value. 
 long getInteger(const char* section, const char* key, long def_val); 
 
 // return as floating-point value. 
 float getFloat(const char* section, const char* key, float def_val); 
 
 // return how many bytes is written into buffer. 
 long getStruct(const char* section, const char* key, void* buffer, long size); 
 // save the section-key value to buffer and return length 
 // of the string. after the call, buffer is null-terminated. 
 long getString(const char* section, const char* key, const char* def_val, char* buffer, long size); 
 
 // save it in dst_str. 
 long getString(const char* section, const char* key, const char* def_val, std::string& dst_str); 
 
#ifdef _AFX 
 // save it in a MFC::CString object dst_str. 
 long getString(const char* section, const char* key, const char* def_val, CString& dst_str); 
#endif 
 
 // the following member functions set the value of the sepcific 
 // section-key as the 'value'. if the section-key is absent, 
 // a new section-key entry is created, and the ini is marked as 
 // modified. 
 
 void setInteger(const char* section, const char* key, long value); 
 
 void setFloat(const char* section, const char* key, float value); 
 
 void setStruct(const char* section, const char* key, const void* buffer, long size); 
 
 void setString(const char* section, const char* key, const char* value); 
 
 
public: // low level member function. 
 
 // return the whole section with name 'section' 
 const KeyMap& getSection(const char* section) const; 
 
 // return all sections in the ini. 
 const SectionMap& getIni() const { return m_map; } 
 
private: 
 void saveBeforeLoad(); 
 const char* key_value(const char* section, const char* key); 
 
 
private: 
 
 // disable copy ctor and assignment operator. 
 IniParser(const IniParser& copy); 
 IniParser& operator=(const IniParser& rhs); 
 
private: 
 static const KeyMap ms_emptySection; 
 
 SectionMap  m_map; 
 std::string m_file_name; 
 bool        m_modified; 
}; 
 
 
}// end of namespace leiXure 


namespace leiXure 
{ 
 
 
static const char left_tag   = '['; 
static const char right_tag  = ']'; 
static const char equal      = '='; 
static const char cr         = '\r'; 
static const char new_line   = '\n'; 
static const char* empty_str = ""; 
 
const IniParser::KeyMap IniParser::ms_emptySection; 
 
IniParser::IniParser() : m_modified(false) 
{ 
} 
 
IniParser::IniParser(const char* file_name) : m_modified(false) 
{ 
 load(file_name); 
} 
 
IniParser::~IniParser() 
{ 
 if(m_modified) 
  save(); 
} 
 
void IniParser::saveBeforeLoad() 
{ 
 if(m_modified) 
  save(); 
 
 m_file_name.resize(0); 
 
 m_map.clear(); 
 
 m_modified = false; 
} 
 
const char* IniParser::key_value(const char* section, const char* key) 
{ 
 SectionIterator itSection = m_map.find(section); 
 if(m_map.end() != itSection) 
 { 
  KeyIterator itKey = itSection->second.find(key); 
  if(itKey != itSection->second.end()) 
   return itKey->second.c_str(); 
 } 
 return 0; 
} 

bool IniParser::load(const char* file_name) 
{ 
		saveBeforeLoad(); 

		ifstream file(file_name); 
		if(!file) 
				return false; 

		file.seekg(0, ios::end); 
		long len = file.tellg(); 
		if(len < 0) 
				return false; 

		char* buffer = new char[len + 1]; 
		if(0 == buffer) 
				return false; 

		file.seekg(0, ios::beg); 
		file.read(buffer, len); 

		buffer[len = file.gcount()] = 0; 

		loadString(buffer, len); 
		m_file_name = file_name; 

		delete[] buffer; 

		return true; 
} 



bool IniParser::loadString(const char* str, unsigned length) 
{ 
		saveBeforeLoad(); 

		if(length && 0==str) 
				return false; 

		enum status 
		{ 
				after_left_tag, 
				after_section_name, 
				after_section_name_ws, 
				after_key_name, 
				after_key_name_ws, 
				after_equal, 
				start 
		}; 

		string section;     // current section. 
		string key;      // current key. 
		status sta          = start; // parsing state. 
		const char* p       = str;  // current parsing position. 
		const char* beg     = p;  // beginning of current element. 
		const char* last_ws = p;  // last white space character. 


		for(; length; ++p, --length) 
		{ 
				if(new_line == *p) 
				{ 
						if(after_equal == sta) 
						{ 
								if(cr == *(p - 1)) 
										--p; 

								m_map[section][key] = string(beg, p - beg); 

								if(cr == *p) 
										++p; 
						} 
						sta = start; 
				} 
				else 
				{ 
						switch(sta) 
						{ 
						case after_left_tag: 
								if(right_tag == *p) 
								{ 
										sta     = start; 
										section = empty_str; // empty section name. 
								} 
								else if(!isspace((unsigned char)*p)) 
								{ 
										sta = after_section_name; 
										beg = p; 
								} 
								break; 

						case after_section_name: 
								if(right_tag == *p) 
								{ 
										sta     = start; 
										section = string(beg, p - beg); 
								} 
								else if(isspace((unsigned char)*p)) 
								{ 
										sta     = after_section_name_ws; 
										last_ws = p; 
								} 
								break; 

						case after_section_name_ws: 
								if(right_tag == *p) 
								{ 
										sta     = start; 
										section = string(beg, last_ws - beg); 
								} 
								else if(!isspace((unsigned char)*p)) 
								{ 
										sta = after_section_name; 
								} 
								break; 

						case after_key_name: 
								if(equal == *p) 
								{ 
										sta = after_equal; 
										key = string(beg, p - beg); 
										beg = p + 1; 
								} 
								else if(isspace((unsigned char)*p)) 
								{ 
										sta     = after_key_name_ws; 
										last_ws = p; 
								} 
								break; 

						case after_key_name_ws: 
								if(equal == *p) 
								{ 
										sta = after_equal; 
										key = string(beg, last_ws - beg);
										beg = p + 1; 
								} 
								else if(!isspace((unsigned char)*p)) 
								{ 
										sta = after_key_name; 
								} 
								break; 

						case start: 
								if(left_tag == *p) 
								{ 
										sta = after_left_tag; 
								} 
								else if(equal == *p) 
								{ 
										key = empty_str; // an empty key. 
										sta = after_equal; 
										beg = p + 1; 
								} 
								else if(!isspace((unsigned char)*p)) 
								{ 
										sta = after_key_name; 
										beg = p; 
								} 
								break; 
						} 
				} 
		} 

		if(after_equal == sta) 
				m_map[section][key] = string(beg, p - beg); 
		return true; 
} 
 
bool IniParser::save() 
{ 
		if(0==m_file_name.c_str() || 0==m_file_name[0]) 
				return false; // file name invalid 

		ofstream file(m_file_name.c_str()); 
		if(!file) 
				return false; 

		for(SectionMap::iterator itApp=m_map.begin(); itApp!=m_map.end(); ++itApp) 
		{ 
				file << left_tag << itApp->first << right_tag << endl; 

				for(KeyMap::iterator itKey=itApp->second.begin(); itKey!=itApp->second.end(); ++itKey) 
						file << itKey->first << equal << itKey->second << endl; 

				file << endl; 
		} 

		m_modified = false; 

		return true; 
} 
 
bool IniParser::saveAs(const char* file_name) 
{ 
		string old_file_name = m_file_name; 

		m_file_name = file_name; 

		if(save()) 
				return true; 

		m_file_name = old_file_name; 
		return false; 
} 
 
long IniParser::getInteger(const char* section, const char* key, long def_val) 
{ 
		istrstream(key_value(section, key)) >> def_val; 
		return def_val; 
} 
 
float IniParser::getFloat(const char* section, const char* key, float def_val) 
{ 
 istrstream(key_value(section, key)) >> def_val; 
 return def_val; 
} 
 
long IniParser::getStruct(const char* section, const char* key, void* buffer, long size) 
{ 
		key = key_value(section, key); 

		if(0==key || 0==*key) 
				return 0; 

		const char* p        = key; 
		char*       dst      = (char*)buffer; 
		long        read_len = 0; 
		char        value; 

		while(*p && read_len<size) 
		{ 
				switch(*p) 
				{ 
				case '0': case '1': case '2': case '3': case '4': 
				case '5': case '6': case '7': case '8': case '9': 
						value = *p - '0'; 
						break; 
				case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': 
						value = *p - 'a' + 10; 
						break; 
				case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': 
						value = *p - 'A' + 10; 
						break; 
				default: 
						return read_len; 
				} 

				if(0 == (p - key)%2) 
						*(dst + read_len) = value << 4; 
				else 
						*(dst + read_len) = (*(dst + read_len) & 0xf0) + value; 

				if(0 == (++p - key)%2) 
						++read_len; 
		} 

		return read_len; 
} 


long IniParser::getString(const char* section, const char* key, const char* def_val, char* buffer, long size) 
{ 
 key = key_value(section, key); 
 if(0 == key) 
  key = def_val; 
 
 strncpy(buffer, key, size - 1); 
 buffer[size - 1] = 0; 
 
 return strlen(buffer); 
} 
 
long IniParser::getString(const char* section, const char* key, const char* def_val, std::string& dst_str) 
{ 
 key = key_value(section, key); 
 dst_str = key ? key : def_val; 
 return dst_str.length(); 
} 
 #ifdef _AFX 
long IniParser::getString(const char* section, const char* key, const char* def_val, CString& dst_str) 
{ 
 key = key_value(section, key); 
 dst_str = key ? key : def_val; 
 return dst_str.GetLength(); 
} 
#endif //_AFX
void IniParser::setInteger(const char* section, const char* key, long value) 
{ 
 char buffer[BUFFER_LEN + 1]; 
 ostrstream ostr(buffer, BUFFER_LEN); 
 ostr << value; 
 buffer[ostr.pcount()] = 0; 
 setString(section, key, buffer); 
} 
 
void IniParser::setFloat(const char* section, const char* key, float value) 
{ 
 char buffer[BUFFER_LEN + 1]; 
 ostrstream ostr(buffer, BUFFER_LEN); 
 ostr << value; 
 buffer[ostr.pcount()] = 0; 
 setString(section, key, buffer); 
} 
 inline char bin2hex(char bin) 
{ 
 return bin<10 ? bin+'0' : bin-10+'A'; 
} 

 void IniParser::setStruct(const char* section, const char* key, const void* buffer, long size) 
{ 
		char* dst = new char[size*2 + 1]; 
		if(dst) 
		{ 
				const char* src = (const char*)buffer; 
				long i=0;
				for(; i<size; ++i) 
				{ 
						dst[i << 1]       = bin2hex((src[i] >> 4) & 0x0f ); 
						dst[(i << 1) + 1] = bin2hex(src[i] & 0x0f); 
				} 

				dst[i << 1] = 0; 
				setString(section, key, dst); 
				delete[] dst; 
		} 
} 
 
void IniParser::setString(const char* section, const char* key, const char* value) 
{ 
 m_map[section][key] = value; 
 m_modified = true; 
} 
const IniParser::KeyMap& IniParser::getSection(const char* section) const 
{ 
 SectionMap::const_iterator itApp = m_map.find(section); 
 return m_map.end()==itApp ? ms_emptySection : itApp->second; 
} 
} 



using namespace leiXure;