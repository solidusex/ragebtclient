#pragma once
#include "afxext.h"

class CSplitterWndEx :	public CSplitterWnd
{
public:
		CSplitterWndEx(void);
		~CSplitterWndEx(void);

public:
		BOOL ReplaceView(int row, int col,CRuntimeClass * pViewClass,SIZE size);
};
