#pragma once

#define		ZHLIB_API		 __declspec(dllexport)

#include <io.h>
#include <fcntl.h>
#include <locale.h>
#include <iostream>
ZHLIB_API void InitConsoleWindow();

typedef unsigned char byte;

#ifdef WIN32
void ConvertGBKToUtf8(std::string& amp, std::string strGBK);
void ConvertUtf8ToGBK(std::string&amp, std::string strUtf8);
ZHLIB_API wchar_t * UTF8ToUnicode(const char* str);
ZHLIB_API char * UnicodeToUTF8(const wchar_t* str);
ZHLIB_API char *  UnicodeToANSI(const wchar_t* str);
ZHLIB_API char * UTF8ToANSI(const char* str);
ZHLIB_API char * UnicodeToANSI(const wchar_t* str);
ZHLIB_API wchar_t * ANSIToUnicode(const char* str);
ZHLIB_API void echo_hexStr(char buf[], int len, const char* info);
ZHLIB_API void echoBinString(char in[], int len);
#else
#include <iconv.h>

// ---- from GBK to UTF8 
char* convert_gbk2utf8(char* in);
#endif

ZHLIB_API char* w2m(const wchar_t* wcs);
ZHLIB_API wchar_t* m2w(const char* mbs);

ZHLIB_API unsigned char ToHex(unsigned char x);
ZHLIB_API unsigned char FromHex(unsigned char x);
ZHLIB_API std::string UrlEncode(const std::string& str);
ZHLIB_API std::string UrlDecode(const std::string& str);

//////////////////////////////////////////////////////////////////////////

typedef enum _COLOR_DEF
{
	CL_LITE_BLUE = 1,
	CL_BLUE = CL_LITE_BLUE + 8,

	CL_LITE_GREEN = 2,
	CL_GREEN = CL_LITE_GREEN + 8,

	CL_LITE_RED = 4,
	CL_RED = CL_LITE_RED + 8,

	CL_LITE_RED2 = 5,
	CL_RED2 = CL_LITE_RED2 + 8,

	CL_LITE_YELLOW = 6,
	CL_YELLOW = CL_LITE_YELLOW + 8,

	CL_LITE_WHITE = 7,
	CL_WHITE = CL_LITE_WHITE + 8,
};
// WZ's color log print, buf no more than 4096 !!!!!!!!
ZHLIB_API void zclog(int color, const char* fmt, ...);
ZHLIB_API void zctlog(int color, const char* fmt, ...); // with thread id print

// color print functions declaration
ZHLIB_API void ConPrint(char *CharBuffer, int len);
ZHLIB_API void ConPrintAt(int x, int y, char *CharBuffer, int len);
ZHLIB_API void gotoXY(int x, int y);
ZHLIB_API void ClearConsole(void);
ZHLIB_API void ClearConsoleToColors(int ForgC, int BackC);
ZHLIB_API void SetColorAndBackground(int ForgC, int BackC);
ZHLIB_API void SetColor(int ForgC);
ZHLIB_API void HideTheCursor(void);
ZHLIB_API void ShowTheCursor(void);
//////////////////////////////////////////////////////////////////////////