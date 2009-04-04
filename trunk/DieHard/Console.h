#pragma once

#include <windows.h>
#include <string>
class Console
{
private:
		static bool		m_is_open;
private:
		void attach(SHORT ConsoleHeight, SHORT ConsoleWidth);
public:
		Console();

		~Console();
public:
		void Open();
		
		void Open(const std::wstring &title, short ConsoleHeight = 300, short ConsoleWidth = 80);

		void Close();

		bool IsOpen()const;
};

