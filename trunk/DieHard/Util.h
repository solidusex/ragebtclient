#pragma once

#include "stdafx.h"

CString StrConv(const std::wstring &wstr);

std::wstring StrConv(const CString &cstring);

#define GET_ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))