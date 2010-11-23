#include "stdafx.h"
#include "Console.h"
#include <fcntl.h>
#include <cstdio>
#include <io.h>

bool Console::m_is_open = false;

void Console::Open()
{
		if (!m_is_open)
		{
				::AllocConsole();
				attach(300, 80);
				m_is_open = true;
		}
}

Console::Console()
{
		
}

void Console::Open(const std::wstring &title, short ConsoleHeight, short ConsoleWidth)
{

		if (!m_is_open)
		{
				::AllocConsole();
				::SetConsoleTitleW(title.c_str());
				attach(ConsoleHeight, ConsoleWidth);
				m_is_open = true;
		}



}

bool Console::IsOpen()const
{
		return m_is_open;
}


void Console::attach(SHORT ConsoleHeight, SHORT ConsoleWidth)
{
		HANDLE  hStd;
		int     fd;
		FILE    *file;

		// 重定向标准输入流句柄到新的控制台窗口
		hStd = GetStdHandle(STD_INPUT_HANDLE);
		fd = _open_osfhandle(reinterpret_cast<intptr_t>(hStd), _O_TEXT); // 文本模式
		file = _fdopen(fd, "r");
		setvbuf(file, NULL, _IONBF, 0); // 无缓冲
		*stdin = *file;

		// 重定向标准输出流句柄到新的控制台窗口
		hStd = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD size;
		size.X = ConsoleWidth;
		size.Y = ConsoleHeight;
		SetConsoleScreenBufferSize(hStd, size);
		fd = _open_osfhandle(reinterpret_cast<intptr_t>(hStd), _O_TEXT); //文本模式
		file = _fdopen(fd, "w");
		setvbuf(file, NULL, _IONBF, 0); // 无缓冲
		*stdout = *file;

		// 重定向标准错误流句柄到新的控制台窗口
		hStd = GetStdHandle(STD_ERROR_HANDLE);
		fd = _open_osfhandle(reinterpret_cast<intptr_t>(hStd), _O_TEXT); // 文本模式
		file = _fdopen(fd, "w");
		setvbuf(file, NULL, _IONBF, 0); // 无缓冲
		*stderr = *file;
}

void Console::Close()
{
		if(m_is_open)
		{
				FreeConsole();
				m_is_open = false;
		}
}

Console::~Console()
{
		Close();
}