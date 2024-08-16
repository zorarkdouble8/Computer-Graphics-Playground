#pragma once

#include <string>
#include <WinUser.h>

using namespace std;

LPCWSTR stringToLPCWSTR(const string& convert)
{
	wstring* wSTR = new wstring(convert.begin(), convert.end());
	return wSTR->c_str();
}

LPCWSTR stringToLPCWSTR(const string&& convert)
{
	wstring* wSTR = new wstring(convert.begin(), convert.end());
	return wSTR->c_str();
}