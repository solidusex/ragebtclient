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

		// �ض����׼������������µĿ���̨����
		hStd = GetStdHandle(STD_INPUT_HANDLE);
		fd = _open_osfhandle(reinterpret_cast<intptr_t>(hStd), _O_TEXT); // �ı�ģʽ
		file = _fdopen(fd, "r");
		setvbuf(file, NULL, _IONBF, 0); // �޻���
		*stdin = *file;

		// �ض����׼�����������µĿ���̨����
		hStd = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD size;
		size.X = ConsoleWidth;
		size.Y = ConsoleHeight;
		SetConsoleScreenBufferSize(hStd, size);
		fd = _open_osfhandle(reinterpret_cast<intptr_t>(hStd), _O_TEXT); //�ı�ģʽ
		file = _fdopen(fd, "w");
		setvbuf(file, NULL, _IONBF, 0); // �޻���
		*stdout = *file;

		// �ض����׼������������µĿ���̨����
		hStd = GetStdHandle(STD_ERROR_HANDLE);
		fd = _open_osfhandle(reinterpret_cast<intptr_t>(hStd), _O_TEXT); // �ı�ģʽ
		file = _fdopen(fd, "w");
		setvbuf(file, NULL, _IONBF, 0); // �޻���
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