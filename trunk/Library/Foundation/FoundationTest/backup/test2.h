#if(0)

const char* num_to_str(unsigned int num, size_t radix)
{

		static char buf[1024];
		char *pstr = buf + sizeof(buf) - 1;
		*pstr = 0;
		do{ *--pstr = "0123456789ABCDEF"[num%radix]; }while((num /= radix));

		return pstr;
}



const char* num_to_str(signed int num, size_t radix)
{
		static char buf[1024];
		bool neg = num < 0 ? num = -num : num;
		
		char *pstr = buf + sizeof(buf) - 1;
		*pstr = 0;
		do{ *--pstr = "0123456789ABCDEF"[num%radix]; }while((num /= radix));
		if(neg)*--pstr = '-';
		return pstr;
}



template<class T>
void exch(T &a, T &b)
{
		T tmp = a;
		a = b;
		b = a;

}

void print_list(const std::list<int> &lst)
{
		for(std::list<int>::const_iterator cit = lst.begin(); cit != lst.end(); ++cit)
		{
				cout << *cit << endl;

		}

		printf("count == %d\n", lst.size());

}

const char* num_to_str(t_uint64 n, size_t base)
{
		assert(base <= 16 && base > 0);
		static char buf[1024];
		buf[1023] = '\0';
		char *p = buf + 1023;
		
		do{*--p = "0123456789ABCDEF"[n % base];}while((n /= base));
		
		return p;
}

const char *num_to_str(t_int64 num, size_t base)
{
		assert(base <= 16 && base > 0);
		bool neg = num & ((t_uint64)0x01 << 63);
		if(neg) num = ~num + 1;

		static char buf[128];
		char *p = buf + 127;
		*p = '\0';

		do{*--p = "0123456789ABCDEF"[num % base];}while((num/=base));
		
		if(neg) *--p = '-';
		return p;
}


t_uint64 str_to_num(const char *str, size_t base)
{
		t_uint64 x; const char *p;

		static t_uint8 *vi = NULL;
		if(vi == NULL)
		{
				vi = new t_uint8[256];
				memset(vi, ~0, 256 * sizeof(t_uint8));
				p = "0123456789aAbBcCdDeEfF"; x = 0;
				while(x < 10)vi[*p++] = x++;
				while(x < 16) {vi[*p] = vi[*(p+1)] = x++; p+=2;}
		}
		
		for(p = str, x = 0; *p != '\0' && vi[*p] < base; p++)x = x * base + (t_uint64)vi[*p];
		
		return *p == '\0' ? x : 0;
}


int __calc_need_buf(const char *fmt, ...)
{
		int len = 0;
		va_list arg_ptr = NULL;
		
		va_start(arg_ptr,fmt);

		len = _vscprintf(fmt, arg_ptr);
		va_end(arg_ptr);
		return len;
}


BOOL FormatV(LPCTSTR lpszFormat, va_list argList)
{
		assert(lpszFormat != NULL);


		enum _FormatModifiers
		{
				FORCE_ANSI =	0x10000,
				FORCE_UNICODE =	0x20000,
				FORCE_INT64 =	0x40000
		};

		va_list argListSave = argList;

		// make a guess at the maximum length of the resulting string
		int nMaxLen = 0;
		for (LPCTSTR lpsz = lpszFormat; *lpsz != _T('\0'); lpsz = ::CharNext(lpsz))
		{
				// handle '%' character, but watch out for '%%'
				if (*lpsz != _T('%') || *(lpsz = ::CharNext(lpsz)) == _T('%'))
				{
						nMaxLen += (int)(::CharNext(lpsz) - lpsz);
						continue;
				}

				int nItemLen = 0;

				// handle '%' character with format
				int nWidth = 0;
				for (; *lpsz != _T('\0'); lpsz = ::CharNext(lpsz))
				{
						// check for valid flags
						if (*lpsz == _T('#'))
								nMaxLen += 2;   // for '0x'
						else if (*lpsz == _T('*'))
								nWidth = va_arg(argList, int);
						else if (*lpsz == _T('-') || *lpsz == _T('+') || *lpsz == _T('0') || *lpsz == _T(' '))
								;
						else // hit non-flag character
								break;
				}
				// get width and skip it
				if (nWidth == 0)
				{
						// width indicated by
						nWidth = _cstrtoi(lpsz);
						for (; *lpsz != _T('\0') && _cstrisdigit(*lpsz); lpsz = ::CharNext(lpsz))
								;
				}
				assert(nWidth >= 0);

				int nPrecision = 0;
				if (*lpsz == _T('.'))
				{
						// skip past '.' separator (width.precision)
						lpsz = ::CharNext(lpsz);

						// get precision and skip it
						if (*lpsz == _T('*'))
						{
								nPrecision = va_arg(argList, int);
								lpsz = ::CharNext(lpsz);
						}
						else
						{
								nPrecision = _cstrtoi(lpsz);
								for (; *lpsz != _T('\0') && _cstrisdigit(*lpsz); lpsz = ::CharNext(lpsz))
										;
						}
						ATLASSERT(nPrecision >= 0);
				}

				// should be on type modifier or specifier
				int nModifier = 0;
				if(lpsz[0] == _T('I') && lpsz[1] == _T('6') && lpsz[2] == _T('4'))
				{
						lpsz += 3;
						nModifier = FORCE_INT64;
				}
				else
				{
						switch (*lpsz)
						{
								// modifiers that affect size
						case _T('h'):
								nModifier = FORCE_ANSI;
								lpsz = ::CharNext(lpsz);
								break;
						case _T('l'):
								nModifier = FORCE_UNICODE;
								lpsz = ::CharNext(lpsz);
								break;

								// modifiers that do not affect size
						case _T('F'):
						case _T('N'):
						case _T('L'):
								lpsz = ::CharNext(lpsz);
								break;
						}
				}

				// now should be on specifier
				switch (*lpsz | nModifier)
				{
						// single characters
				case _T('c'):
				case _T('C'):
						nItemLen = 2;
						va_arg(argList, TCHAR);
						break;
				case _T('c') | FORCE_ANSI:
				case _T('C') | FORCE_ANSI:
						nItemLen = 2;
						va_arg(argList, char);
						break;
				case _T('c') | FORCE_UNICODE:
				case _T('C') | FORCE_UNICODE:
						nItemLen = 2;
						va_arg(argList, WCHAR);
						break;

						// strings
				case _T('s'):
						{
								LPCTSTR pstrNextArg = va_arg(argList, LPCTSTR);
								if (pstrNextArg == NULL)
								{
										nItemLen = 6;  // "(null)"
								}
								else
								{
										nItemLen = lstrlen(pstrNextArg);
										nItemLen = max(1, nItemLen);
								}
								break;
						}

				case _T('S'):
						{
								LPCSTR pstrNextArg = va_arg(argList, LPCSTR);
								if (pstrNextArg == NULL)
								{
										nItemLen = 6; // "(null)"
								}
								else
								{
										nItemLen = lstrlenA(pstrNextArg);
										nItemLen = max(1, nItemLen);
								}

								break;
						}

				case _T('s') | FORCE_ANSI:
				case _T('S') | FORCE_ANSI:
						{
								LPCSTR pstrNextArg = va_arg(argList, LPCSTR);
								if (pstrNextArg == NULL)
								{
										nItemLen = 6; // "(null)"
								}
								else
								{


										nItemLen = lstrlenA(pstrNextArg);
										nItemLen = max(1, nItemLen);
								}
								break;
						}

				case _T('s') | FORCE_UNICODE:
				case _T('S') | FORCE_UNICODE:
						{
								LPWSTR pstrNextArg = va_arg(argList, LPWSTR);
								if (pstrNextArg == NULL)
								{
										nItemLen = 6; // "(null)"
								}
								else
								{
										nItemLen = (int)wcslen(pstrNextArg);
										nItemLen = max(1, nItemLen);
								}
								break;
						}
				}

				// adjust nItemLen for strings
				if (nItemLen != 0)
				{
						nItemLen = max(nItemLen, nWidth);
						if (nPrecision != 0)
								nItemLen = min(nItemLen, nPrecision);
				}
				else
				{
						switch (*lpsz)
						{
								// integers
						case _T('d'):
						case _T('i'):
						case _T('u'):
						case _T('x'):
						case _T('X'):
						case _T('o'):
								if (nModifier & FORCE_INT64)
										va_arg(argList, __int64);
								else
										va_arg(argList, int);
								nItemLen = 32;
								nItemLen = max(nItemLen, nWidth + nPrecision);
								break;

						case _T('e'):
						case _T('E'):
						case _T('g'):
						case _T('G'):
								va_arg(argList, double);
								nItemLen = 128;
								nItemLen = max(nItemLen, nWidth + nPrecision);
								break;
						case _T('f'):
								{
										double f = va_arg(argList, double);
										// 312 == strlen("-1+(309 zeroes).")
										// 309 zeroes == max precision of a double
										// 6 == adjustment in case precision is not specified,
										//   which means that the precision defaults to 6
										int cchLen = max(nWidth, 312 + nPrecision + 6);
										CTempBuffer<TCHAR, _WTL_STACK_ALLOC_THRESHOLD> buff;
										LPTSTR pszTemp = buff.Allocate(cchLen);
										if(pszTemp != NULL)
										{
												SecureHelper::sprintf_x(pszTemp, cchLen, _T("%*.*f"), nWidth, nPrecision + 6, f);
												nItemLen = (int)_tcslen(pszTemp);
										}
										else
										{
												nItemLen = cchLen;
										}
								}
								break;

						case _T('p'):
								va_arg(argList, void*);
								nItemLen = 32;
								nItemLen = max(nItemLen, nWidth + nPrecision);
								break;

								// no output
						case _T('n'):
								va_arg(argList, int*);
								break;

						default:
								ATLASSERT(FALSE);  // unknown formatting option
						}
				}

				// adjust nMaxLen for output nItemLen
				nMaxLen += nItemLen;
		}

		if(GetBuffer(nMaxLen) == NULL)
				return FALSE;

		int nRet = SecureHelper::vsprintf_x(m_pchData, GetAllocLength() + 1, lpszFormat, argListSave);

		nRet;   // ref
		ATLASSERT(nRet <= GetAllocLength());
		ReleaseBuffer();

		va_end(argListSave);
		return TRUE;
}



#endif

